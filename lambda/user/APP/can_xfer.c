/*===========================================================================*/
/* FILE: can_xfer.c                                                            */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            can_xfer.c %                                              */
/*                                                                           */
/* created_by:       sjl                                                     */
/*                                                                           */
/* date_created:     20 Feb 2013                                             */
/*                                                                           */
/* %derived_by:      rz65p6 %                                                */
/*                                                                           */
/* %date_modified:   Wed Feb 20 11:46:46 2013 %                              */
/*                                                                           */
/* %version:         1 %                                                     */
/*                                                                           */
/*===========================================================================*/
/* DESCRIPTION:                                                              */
/*   CAN - MSG management                                                    */
/*                                                                           */
/* ABBREVIATIONS:                                                            */
/*   None.                                                                   */
/*                                                                           */
/* TRACEABILITY INFO:                                                        */
/*   Design Document:                                                        */
/*                                                                           */
/*   Requirements Document(s):                                               */
/*                                                                           */
/*   Applicable Standards:                                                   */
/*   C Coding Standards:        SW_REF 264.15D.                              */
/*                                                                           */
/* DEVIATIONS FROM STANDARDS:                                                */
/*   None.                                                                   */
/*                                                                           */
/*===========================================================================*/
/* REUSE: DO NOT MODIFY THIS FILE.                                           */
/* Co-ordinate any desired changes with the Software Forward Engineering and */
/* Building Blocks group                                                     */
/*===========================================================================*/


/*===========================================================================*\
 * Standard Header Files
\*===========================================================================*/
#include "reuse.h"
#include "io_can_config.h"
#include "dd_can.h"
#include "can_xfer.h"

#include "stdint.h"

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/


/* ============================================================================ *\
 * Local INLINE functions and #define function-like macros.
\* ============================================================================ */

/*.***************************************************************************
*. Name: Set_Real_Time_Ping_Pong_Buffer_Pointers MACRO
*.
*.   Description:
*.      Toggles the real time ping pong buffer pointers between ping
*.      and pong.
*.
*.   Warning: CAN_transmit_message_number must be defined within the scope
*.            of the invocation of this macro.
*.
*. Shared Variables: Transmit_Buffer_Ctl_Ptr structure (variable)
*.
*. Parameters: None
*.
*. Return Value: none
*.
*.
******************************************************************************/
#define Set_Real_Time_Ping_Pong_Buffer_Pointers( ) \
   do\
   {\
      if( TRANSMIT_NEXT_DATA_IN_PTR == TRANSMIT_BUFFER_MAX_ADDRESS )\
      {\
         TRANSMIT_NEXT_DATA_IN_PTR = TRANSMIT_BUFFER_BASE_ADDRESS;\
         TRANSMIT_NEXT_DATA_OUT_PTR = TRANSMIT_PONG_BUFFER_ADDRESS;\
      }\
      else\
      {\
         TRANSMIT_NEXT_DATA_IN_PTR  = TRANSMIT_PONG_BUFFER_ADDRESS;\
         TRANSMIT_NEXT_DATA_OUT_PTR = TRANSMIT_BUFFER_BASE_ADDRESS ;\
      }\
   }while( 0 )


/*.***************************************************************************
*. Name:  Check_FIFO_Transmit_Buffer_Pointers_For_Wrap MACRO
*.
*.   Description:
*.      Checks to see if the buffer in pointer needs to wrap for a transmit
*.      message. Wrap the pointer if required.
*.
*.   Warning: CAN_transmit_message_number must be defined within the scope
*.            of the code that invokes this macro.
*.
*. Shared Variables: Transmit_Buffer_Ctl_Ptr structure (variable)
*.
*. Parameters: None
*.
*. Return Value: none
*.
*.
******************************************************************************/

#define  Check_FIFO_Transmit_Buffer_Pointers_For_Wrap( ) \
   do \
   {\
      if( TRANSMIT_NEXT_DATA_IN_PTR == TRANSMIT_BUFFER_MAX_ADDRESS )\
      {\
         TRANSMIT_NEXT_DATA_IN_PTR = TRANSMIT_BUFFER_BASE_ADDRESS;\
      }\
   }while( 0 )

/*.***************************************************************************
*. Name: Check_For_FIFO_Transmit_Buffer_Full MACRO
*.
*.   Description:
*.      Checks for buffer full, and sets flag if necessary
*.
*.   Warning: CAN_transmit_message_number must be defined within the scope
*.            of the invocation of this macro.
*.
*. Shared Variables: Transmit_Buffer_Ctl_Ptr structure (variable)
*.
*. Parameters: none
*. Return Value: none
*.
*.
******************************************************************************/

#define Check_For_FIFO_Transmit_Buffer_Full( ) \
   do\
   {\
      if ( TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER == TRANSMIT_CAN_BUFFER_DEPTH )\
      { \
         TRANSMIT_BUFFER_FULL = true; \
      } \
   }while( 0 )
/*****************************************************************************
*. Name:  Check_Receive_Message_Out_Pointer_For_Wrap   MACRO
*.
*.   Description:
*.      Because of the conditional processing associated with the circular
*.      buffer, checking the buffer for wrapping is best abstracted to a
*.      function call macro.
*.
*.   Warning: CAN_receive_message_number must be defined at the point of
*.            invoking this macro.
*.
*. Shared Variables: Receive_Buffer_Ctl_Ptr structure (variable)
*.                   CAN_Receive_Parameter_Table (constant)
*. Parameters: none
*.
*. Return Value: none
*.
******************************************************************************/

#define Check_Receive_Message_Out_Pointer_For_Wrap( )\
   do\
   {\
      if( RECEIVE_NEXT_DATA_OUT_PTR == RECEIVE_BUFFER_MAX_ADDRESS )\
      {\
         RECEIVE_NEXT_DATA_OUT_PTR = RECEIVE_BUFFER_BASE_ADDRESS;\
      }\
   }while( 0 )


/*===========================================================================*\
 * Function Definitions
\*===========================================================================*/
static uint8_t *Allocate_Buffer_Space( uint16_t requested_allocation );


/*===========================================================================*/
/* FUNCTION: Get_Message                                                     */
/*===========================================================================*/
/* RETURN VALUE:                                                             */
/*   bool - true: message is valid; false: message is invalid                */
/*                                                                           */
/* INPUT PARAMETERS:                                                         */
/*   CAN_Msg_Id /message_address                                             */
/*                                                                           */
/* --------------------------------------------------------------------------*/
/* ABSTRACT:                                                                 */
/* --------------------------------------------------------------------------*/
/* This function is to get CAN MSG from driver                               */
/*===========================================================================*/
bool Get_Message( CAN_message_ID_T can_message_id, uint8_t * message_address )
{
uint8_t            CAN_receive_message_number;
uint8_t            loop_counter;
bool               request_honored;
   
   // look up the index in the msg table
   for( CAN_receive_message_number = 0;
        CAN_receive_message_number <  CAN_NUMBER_OF_RECEIVE_MESSAGES;
        CAN_receive_message_number++ )
   {
      if( RECEIVE_CAN_MESSAGE_ID == can_message_id )
      {
            break;
            // buffer index has been found
      }
   }
   
   if (CAN_receive_message_number < CAN_NUMBER_OF_RECEIVE_MESSAGES)
   { // check if the index is legal or not

      if( NEVER_RECEIVED_MESSAGE )
      {
         request_honored = false;
      }
      else if( RECEIVE_INACTIVE_MESSAGE )
      {
         request_honored = false;
      }
      else
      {
//         DisableInterrupts();

         for( loop_counter = 0;
              loop_counter < RECEIVE_CAN_MESSAGE_LENGTH;
              loop_counter++ )
         {
            *message_address++ = *RECEIVE_NEXT_DATA_OUT_PTR;
            RECEIVE_NEXT_DATA_OUT_PTR++;
         }


         RECEIVE_BUFFER_FULL = false;
         if(RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER != 0)
         {
            RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER--;
         }
         if( 0 == RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER )
         {
            RECEIVE_MESSAGE_STALE = true;
            RECEIVE_NEXT_DATA_OUT_PTR -= RECEIVE_CAN_MESSAGE_LENGTH;
         }
         else
         {
            Check_Receive_Message_Out_Pointer_For_Wrap( );
         }
  //       EnableInterrupts();

         request_honored = true;
      }
   }
   else
   {
      // CAN_receive_message_number >= CAN_NUMBER_OF_TRANSMIT_MESSAGES:
      // no valid message ID found in the parameter table
   }

   return( request_honored );
   
}
#if 0
/*===========================================================================*/
/* FUNCTION: Transmit_Message                                                */
/*===========================================================================*/
/* RETURN VALUE:                                                             */
/*   bool - true: message has been loaded to buffer; false: buffer is full   */
/*                                                                           */
/* INPUT PARAMETERS:                                                         */
/*   CAN_Msg_Id /message_address                                             */
/*                                                                           */
/* --------------------------------------------------------------------------*/
/* ABSTRACT:                                                                 */
/* --------------------------------------------------------------------------*/
/* This function is to get CAN MSG from driver                               */
/*===========================================================================*/
bool Transmit_Message( CAN_message_ID_T can_message_id, uint8_t * message_address )
{
uint8_t            CAN_transmit_message_number;
uint8_t            loop_counter;
bool               request_honored;
   
   // to reduce table scan time
   for( CAN_transmit_message_number = 0;
        CAN_transmit_message_number <  CAN_NUMBER_OF_TRANSMIT_MESSAGES;
        CAN_transmit_message_number++ )
   {
      if( TRANSMIT_CAN_MESSAGE_ID == can_message_id )
      {
            break;
      }
   }

   if (CAN_transmit_message_number < CAN_NUMBER_OF_TRANSMIT_MESSAGES)
   {

       // check bus off error flag
        
//       DisableInterrupts();

       if( TRANSMIT_BUFFER_FULL )
       {
          /* can't transmit when FIFO buffer is full */
          request_honored = false;
       }
       else
       {
           for( loop_counter = 0;
                loop_counter < TRANSMIT_CAN_MESSAGE_LENGTH;
                loop_counter++ )
           {
              /* the next two statements should be able to be coded
               * as a single statement, but it appears a compiler
               * bug prevents the single statement version from working.
               */
              *TRANSMIT_NEXT_DATA_IN_PTR = *message_address++;
              TRANSMIT_NEXT_DATA_IN_PTR++;
           }

           TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER++;
           Check_For_FIFO_Transmit_Buffer_Full( );
           Check_FIFO_Transmit_Buffer_Pointers_For_Wrap( );

           TRANSMIT_REQUESTED = true;
           request_honored = true;

       }
       
       if( No_Hardware_Buffer_Loaded( ) )
       {
         /* don't call this routine if a message is in
          * a hardware buffer, and an interrupt can go
          * off, resulting in this routine being called
          * again before it is exited.
          */
          Hw_Transmit_Message();
       }
//       EnableInterrupts() ;

   }
   else
   {
       // not a message with valid ID, being ignored
       request_honored = false;
   }

   return( request_honored );
}
#endif
/*===========================================================================*/
/* FUNCTION: CAN_Sw_Buffer_Init                                              */
/*===========================================================================*/
/* RETURN VALUE:                                                             */
/*   none                                                                    */
/*                                                                           */
/* INPUT PARAMETERS:                                                         */
/*   none                                                                    */
/*                                                                           */
/* --------------------------------------------------------------------------*/
/* ABSTRACT:                                                                 */
/* --------------------------------------------------------------------------*/
/* This function is to get CAN MSG from driver                               */
/*===========================================================================*/
void CAN_Reset_Init( void )
{
   uint8_t loop_counter;

   /* initialize CAN buffer and CAN buffer ctl pointers and structures */

   for( loop_counter =0; loop_counter < 9;
        loop_counter++ )
   {
         Transmit_Buffer_Ctl_Ptr[loop_counter]  = ( transmit_buffer_ctl_T * )Allocate_Buffer_Space( (uint16_t)sizeof(transmit_buffer_ctl_T ) );

         Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_full = false;

         Transmit_Buffer_Ctl_Ptr[loop_counter]->number_of_messages_in_buffer = 0;

         Transmit_Buffer_Ctl_Ptr[loop_counter]->message_in_transmit_buffer = false;

         Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address =
            Allocate_Buffer_Space( CAN_Transmit_Message_Parameter_Table
                                 [loop_counter].CAN_message_length *
                                  ( CAN_Transmit_Message_Parameter_Table
                                [loop_counter].CAN_buffer_depth + 1 ) );

         Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_max_address =
            Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address +
            ( CAN_Transmit_Message_Parameter_Table[loop_counter].CAN_message_length *
                                  (CAN_Transmit_Message_Parameter_Table
                                  [loop_counter].CAN_buffer_depth + 1 ) );

         Transmit_Buffer_Ctl_Ptr[loop_counter]->pong_buffer_address =
         ( uint8_t * )( ( uint16_t )Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address +
            CAN_Transmit_Message_Parameter_Table[loop_counter].CAN_message_length );

         Transmit_Buffer_Ctl_Ptr[loop_counter]->next_data_in_ptr =
         Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address;
         Transmit_Buffer_Ctl_Ptr[loop_counter]->next_data_out_ptr =
         Transmit_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address;
     }
    
   for( loop_counter =0; loop_counter <1;
        loop_counter++ )
   {
         Receive_Buffer_Ctl_Ptr[loop_counter]  = ( receive_buffer_ctl_T * )
                 Allocate_Buffer_Space( sizeof( receive_buffer_ctl_T ) );
         Receive_Buffer_Ctl_Ptr[loop_counter]-> stale = true;
         Receive_Buffer_Ctl_Ptr[loop_counter]-> buffer_full = false;


         Receive_Buffer_Ctl_Ptr[loop_counter]-> inactive_message = false;
         Receive_Buffer_Ctl_Ptr[loop_counter]->
                                         never_received_message = true;
                                         
         Receive_Buffer_Ctl_Ptr[loop_counter]-> buffer_base_address =
            Allocate_Buffer_Space( CAN_Receive_Message_Parameter_Table
                                 [loop_counter].CAN_message_length *
                                  ( CAN_Receive_Message_Parameter_Table
                                 [loop_counter].CAN_buffer_depth + 1 ) );
                                 
         Receive_Buffer_Ctl_Ptr[loop_counter]-> buffer_max_address =
            Receive_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address +
                                 ( CAN_Receive_Message_Parameter_Table
                                 [loop_counter].CAN_message_length *
                                  ( CAN_Receive_Message_Parameter_Table
                                 [loop_counter].CAN_buffer_depth + 1 ) );

         Receive_Buffer_Ctl_Ptr[loop_counter]->pong_buffer_address =
            ( uint8_t * )
            ( ( uint16_t )Receive_Buffer_Ctl_Ptr[loop_counter]->
                                  buffer_base_address +
             CAN_Receive_Message_Parameter_Table[loop_counter].
                                  CAN_message_length );

         Receive_Buffer_Ctl_Ptr[loop_counter]->next_data_in_ptr =
            Receive_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address;
         Receive_Buffer_Ctl_Ptr[loop_counter]->next_data_out_ptr =
            Receive_Buffer_Ctl_Ptr[loop_counter]->buffer_base_address;
   }
}

/*.***************************************************************************
*. Name: Allocate_Buffer_Space
*.
*.   Description:
*.      A single character array is allocated in can_aif.c to buffer all
*.      CAN messages.  This space is then allocated to individual messages
*.      at execution time.  This scheme is used since each message may be
*.      of a differing length, and differing depth, making the use of
*.      run time allocation of memory for  each message necessary.
*.
*.   Warning: This routine does not check array bounds.  If the application
*.      fails to allocate enough memory in the single character arrary, then
*.      overruning this memory could corrupt other areas of memory.  Because
*.      of this, the CAN wizard will compute and update the amount of memory
*.      required, and update the can_aif.c file as required, if requested.
*.
*. Shared Variables: CAN_Buffer
*.
*. Parameters: requested allocation in bytes
*.
*. Return Value: pointer to beginning address of allocated memory
*.
*.
******************************************************************************/
static uint8_t *Allocate_Buffer_Space( uint16_t requested_allocation )
{
u8 *allocation_pointer;
static uint8_t *buffer_pointer; /* have to be static */
   if(NULL == buffer_pointer)
	 {
		buffer_pointer = ( uint8_t * )&CAN_Buffer;
	 }
   allocation_pointer = buffer_pointer;
   
   buffer_pointer = ( uint8_t * ) ( ( uint16_t ) buffer_pointer +
                                   requested_allocation );
   return( allocation_pointer );

}


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 *===========================================================================*/

