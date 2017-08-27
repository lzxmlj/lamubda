/*===========================================================================*/
/* FILE: dd_can.c                                                            */
/*===========================================================================*/
/* COPYRIGHT 2004, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            dd_can.c %                                              */
/*                                                                           */
/* created_by:       rz65p6                                                  */
/*                                                                           */
/* date_created:     26 April 2004                                           */
/*                                                                           */
/* %derived_by:      wzf9ks %                                                */
/*                                                                           */
/* %date_modified:   Wed Jan 11 15:19:09 2012 %                                                       */
/*                                                                           */
/* %version:         8 %                                                     */
/*                                                                           */
/*===========================================================================*/
/* DESCRIPTION:                                                              */
/*   This file provides the input output functionalities for all the CAN     */
/*   Functinality.                                                           */
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
#include "dd_can.h"

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/
/* none */


/*===========================================================================*\
 * Local Type Declarations
\*===========================================================================*/
/* none */


/*===========================================================================*\
 * Exported Object Definitions
\*===========================================================================*/
receive_buffer_ctl_T  *Receive_Buffer_Ctl_Ptr[1];
transmit_buffer_ctl_T *Transmit_Buffer_Ctl_Ptr[5];

/*===========================================================================*\
 * Local Object Definitions
\*===========================================================================*/
/* none */


/*===========================================================================*\
 * Local Function Prototypes
\*===========================================================================*/
/* none */


/* ============================================================================ *\
 * Local INLINE functions and #define function-like macros.
\* ============================================================================ */

/*.***************************************************************************
*. Name: Check_Transmit_Out_Buffer_Pointer_For_Wrap MACRO
*.
*.   Description:
*.      Checks to see if the next_data_out_ptr in the transmit buffer for
*.      the specified message should be wrapped and does the wrapping if
*.      required.
*.
*.   Warning: Is is assumed that CAN_transmit_message_number is defined
*.            in scope when this macro is invoked.
*.
*. Shared Variables: none
*.
*. Parameters: none
*.
*. Return Value: none
*.
*.
*. Updated: 12/22/98 btc - created macro

******************************************************************************/

#define Check_Transmit_Out_Buffer_Pointer_For_Wrap()\
   do\
   {\
      if( TRANSMIT_NEXT_DATA_OUT_PTR == TRANSMIT_BUFFER_MAX_ADDRESS )\
      {\
         TRANSMIT_NEXT_DATA_OUT_PTR = TRANSMIT_BUFFER_BASE_ADDRESS;\
      }\
   }while( 0 )

/*.***************************************************************************
*. Name: Check_FIFO_Recieve_Message_In_For_Full  MACRO
*.
*. Description: If the buffer is full, the buffer full flag will
*.              be set.
*.
*. Warning: This procedure assumes that CAN_receive_message_number is
*.          defined within the scope of this macro.
*.
*. Shared Variables: CAN_Receive_Message_Parameter_Table
*.                   Receive_Buffer_Ctl_Ptr
*.
*. Parameters: none
*.
*. Return Value: none
*.
*. Updated: 11/23/98 btc
*.          12/22/98 btc - stop using indexes, just character pointers instead
******************************************************************************/

#define Check_FIFO_Recieve_Message_In_For_Full()\
     do\
     {\
        if( RECEIVE_CAN_BUFFER_DEPTH == RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER )\
        {\
           RECEIVE_BUFFER_FULL = true;\
        }\
     }while( 0 )

/*.***************************************************************************
*. Name: Check_Receive_Message_In_Pointer_For_Wrap MACRO
*.
*. Description: This macro takes care of the processing associated with
*.              incrementing the buffer pointer for a circular buffer.

*. Warning: This procedure assumes that CAN_receive_message_number is
*.          defined within the scope of this macro.
*.
*. Shared Variables: Receive_Buffer_Ctl_Ptr
*.                   CAN_Receive_Message_Parameter_Table
*. Parameters: none
*.
*. Return Value: none
*.
*. Updated: 11/23/98 btc
*.          12/22/98 btc stop using indexes into buffers,use pointers instead
******************************************************************************/

#define Check_Receive_Message_In_Pointer_For_Wrap()\
     do\
     {\
        if( RECEIVE_NEXT_DATA_IN_PTR == RECEIVE_BUFFER_MAX_ADDRESS )\
        {\
           RECEIVE_NEXT_DATA_IN_PTR = RECEIVE_BUFFER_BASE_ADDRESS ;\
        }\
     }while( 0 )


/*.***************************************************************************
*. Name: Set_Ping_Pong_Receive_Message_Out_Pointer MACRO
*.
*. Description: For real time messages, this will set the recieve buffer out
*.              data pointer to ping(base address)  or pong.

*. Warning: This procedure assumes that CAN_receive_message_number is
*.          defined within the scope of this macro.
*.
*. Shared Variables: Receive_Buffer_Ctl_Ptr
*.                   CAN_Receive_Message_Parameter_Table
*. Parameters: none
*.
*. Return Value: none
*.
*. Updated: 11/23/98 btc
*.          12/22/98 btc stop using indexes into buffers,use pointers instead
******************************************************************************/

#define  Set_Ping_Pong_Receive_Message_Out_Pointer() \
   do \
   {\
      if( RECEIVE_NEXT_DATA_IN_PTR == RECEIVE_BUFFER_BASE_ADDRESS )\
      {\
         RECEIVE_NEXT_DATA_OUT_PTR = RECEIVE_PONG_BUFFER_ADDRESS;\
      }\
      else\
      {\
         RECEIVE_NEXT_DATA_OUT_PTR = RECEIVE_BUFFER_BASE_ADDRESS;\
      }\
   }while( 0 )


/*.***************************************************************************
*. Name:   Transfer_Data_From_Hardware_To_Software_Buffer  MACRO
*.
*. Description: This macro transfers data from the CAN hardware receive
*.              buffer to the software buffer for the designated message.

*. Warning: This procedure assumes that CAN_receive_message_number and
*.          byte_counter are defined within the scope of this macro.
*.
*. Shared Variables: Receive_Buffer_Ctl_Ptr
*.
*. Parameters: none
*.
*. Return Value: none
*.
*. Updated: 11/23/98 btc
******************************************************************************/
#define Transfer_Data_From_Hardware_To_Software_Buffer()\
    do\
    {\
       for( byte_counter = 0;\
            byte_counter < RECEIVE_CAN_MESSAGE_LENGTH;\
            byte_counter++ )\
       {\
           *RECEIVE_NEXT_DATA_IN_PTR  = *source_address_ptr++;\
              RECEIVE_NEXT_DATA_IN_PTR++;\
       }\
    }while( 0 )


/* ============================================================================ *\
 * Function definitions
 * ============================================================================ */

/*.***************************************************************************
*. Name:  Hw_Transmit_Message
*.
*. Description: This function attempts to send a message if one is
*.              available to be sent, and there exists an empty
*.              hardware buffer, and if no message of the same ID
*.              is already present in one of the CAN hardware buffers.
*.
*. Warning: This procedure is called by both the transmit interrupt service
*.          routine, and the transfer layer when either wants to transmit
*.          a message.
*.
*. Shared Variables: Transmit_Buffer_Ctl_Ptr
*. Parameters: none
*.
*. Return Value: none
*.
*. Updated: 11/23/98 btc
******************************************************************************/
void Hw_Transmit_Message( void )
{
   /* ID of message being transmitted in each of the three CAN buffers */
volatile uint8_t     txbuffsel;
uint8_t  CAN_transmit_message_number;
uint8_t  *destination_address;
uint8_t  *source_address;
uint8_t  byte_counter;
bool     transmit_requested;
  
  transmit_requested = false;
  
//  CAN0TBSEL = CAN0TFLG;
 // txbuffsel = CAN0TBSEL;
   /* turn off interrupt for empty buffer */
  //CAN0TIER = CAN0TIER & ( ~txbuffsel );

   for ( CAN_transmit_message_number = 0;
         CAN_transmit_message_number < CAN_NUMBER_OF_TRANSMIT_MESSAGES;
         CAN_transmit_message_number++)
   {
        if ( TRANSMIT_REQUESTED )
        {
            // to get CAN buffer with highest priority with data transmission request pending
            transmit_requested = true;
            break;
        }
        else
        {
            // return to search the next one
        }
   }

   if ( transmit_requested )
   {
    //    destination_address  = (uint8_t *)(&CAN0TXDSR0);
    //    CAN0TXIDR01 = TRANSMIT_CAN_MESSAGE_ID << MESSAGE_ID_SHIFT;
    //    CAN0TXDLR  = TRANSMIT_CAN_MESSAGE_LENGTH;

        source_address = (uint8_t *) TRANSMIT_NEXT_DATA_OUT_PTR;

        for( byte_counter = 0;
             byte_counter < TRANSMIT_CAN_MESSAGE_LENGTH;
             byte_counter++ )
        {
             *destination_address++ = *source_address++;
        }

        //TRANSMIT_NEXT_DATA_OUT_PTR += TRANSMIT_CAN_MESSAGE_LENGTH;
        
        // informs the CAN hardware that a valid message has been loaded into the zero buffer,
        // and is ready to be sent.
      //  CAN0TFLG = txbuffsel;
        
        if(TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER > 0)
        {
            TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER--;
        }
        if(TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER == 0)
        {
            TRANSMIT_REQUESTED = false;
        }
        
        TRANSMIT_BUFFER_FULL = false;
        //Check_Transmit_Out_Buffer_Pointer_For_Wrap( );
 
        /* Turn Transmit Interrupts on */
        //CAN0TIER = CAN0TIER  | txbuffsel;

        if( TRANSMIT_NOTIFIER_FUNCTION_PTR )
        {
            // call notifier function if data buffer is empty
            ( *TRANSMIT_NOTIFIER_FUNCTION_PTR ) (TRANSMIT_CAN_MESSAGE_ID);
        }

	 TRANSMIT_NEXT_DATA_OUT_PTR += TRANSMIT_CAN_MESSAGE_LENGTH;

	 Check_Transmit_Out_Buffer_Pointer_For_Wrap( );
	 
   }
   else
   {
        // no data to be transmitted through this CAN port
   }

}


/*.******************************************************************
*. Name: CAN_Receive_Interrupt
*.    This routine is the interrupt handler for receiving
*.    a message.
*.
*. Shared Variables: Receive_Buffer_Ctl_Ptr
*.                   CAN_Receive_Message_Parameter_Table
*.
*. Parameters: none.
*. Return Value: none.
*.
*  Updates:
*   11/10/98 btc
********************************************************************/
void CAN_Receive_Interrupt( void ) /* INTERRUPT void CAN_Receive_Interrupt ( void ) */
{
CAN_message_ID_T   message_id;
uint8_t CAN_receive_message_number;
uint8_t byte_counter;
uint8_t *source_address_ptr;
bool    recieve_honored;

   recieve_honored = false;
   // get 11 bits standard CAN ID only
 //  message_id = (CAN_message_ID_T)CAN0RXIDR01 >> MESSAGE_ID_SHIFT;

   for ( CAN_receive_message_number = 0;
         CAN_receive_message_number < CAN_NUMBER_OF_RECEIVE_MESSAGES;
         CAN_receive_message_number++)
   {
        if ( CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].CAN_message_ID == message_id )
        {
            recieve_honored = true;
            break;
        }
   }

   if( recieve_honored &&
       !RECEIVE_INACTIVE_MESSAGE )
   {
      //   source_address_ptr = (uint8_t *)(&CAN0RXDSR0);
         NEVER_RECEIVED_MESSAGE = false;

         if(!RECEIVE_BUFFER_FULL)
         {
                 if( RECEIVE_MESSAGE_STALE )
                  {
                     RECEIVE_NEXT_DATA_OUT_PTR = RECEIVE_NEXT_DATA_IN_PTR;
                     RECEIVE_MESSAGE_STALE = false;
                  }
                  Transfer_Data_From_Hardware_To_Software_Buffer();
                  
                  // Releases the receive buffer for the CAN hardware to load
                  // it with more data.
               //   CAN0RFLG |= CAN0RFLG_RXF_MASK;

                  RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER++;
                  Check_FIFO_Recieve_Message_In_For_Full( );

                  if ( !RECEIVE_BUFFER_FULL )
                  {
                     Check_Receive_Message_In_Pointer_For_Wrap( );
                  }
                  else
                  {
                     RECEIVE_NEXT_DATA_IN_PTR = ( RECEIVE_NEXT_DATA_IN_PTR -
                                                  ( RECEIVE_CAN_MESSAGE_LENGTH *
                                                    sizeof( uint8_t) ) );
                  }

            }
            else
            {
               Transfer_Data_From_Hardware_To_Software_Buffer( );
                  
               // Releases the receive buffer for the CAN hardware to load
               // it with more data.
               //CAN0RFLG |= CAN0RFLG_RXF_MASK;

               RECEIVE_NEXT_DATA_IN_PTR = ( RECEIVE_NEXT_DATA_IN_PTR -
                                            ( RECEIVE_CAN_MESSAGE_LENGTH  *
                                              sizeof( uint8_t ) ) );
         }

         // call notifier function
         if( 0 != RECEIVE_NOTIFIER_FUNCTION_PTR )
         {
             ( *RECEIVE_NOTIFIER_FUNCTION_PTR ) ( message_id );
         }
   
   }/* end if */
   else
   {
        // Releases the receive buffer for the CAN hardware to load
        // it with more data.
     //   CAN0RFLG |= CAN0RFLG_RXF_MASK;
   }

}


/*.******************************************************************
*. Name: CAN_Error_Handler
*.    This routine is the interrupt handler for handling the
*.    receive buffer overwrite, temporary bus off and permanent
*.    bus off exceptions.
*.
*. Shared Variables: none.
*.
*. Parameters: none.
*.
*. Return Value: none.
*.
*. Updates:
*.  02/03/00 can@cgs created new contents
********************************************************************/

void CAN_Error_Handler( void )
{

   /* CAN hardware receive buffer overwrite */
//   if ( CAN0RFLG & OVRIF_M )
  // {
         /* clear the overrun interrupt flag */
         /* Note: Here we don't have to worry about other bits because, writing
          *       0 to other bits won't change that bit.
          */
   //    CAN0RFLG = OVRIF_M;
//   }
  // else if ( CAN0RFLG & CSCIF_M  )
   //{
        // acknowledge the interrupt
      // CAN0RFLG = CSCIF_M;

         /* CAN status being changed */
   //}
   //else
   //{ /* unexpected error, return */
   //}

}


/*===========================================================================*/
/* FUNCTION: Do_Init_CAN                                                     */
/*===========================================================================*/
/* RETURN VALUE:                                                             */
/*   None.                                                                   */
/*                                                                           */
/* INPUT PARAMETERS:                                                         */
/*   None.                                                                   */
/*                                                                           */
/* EXTERNAL REFERENCES:                                                      */
/*   None.                                                                   */
/*                                                                           */
/* DEVIATIONS FROM STANDARDS:                                                */
/*   None.                                                                   */
/*                                                                           */
/* --------------------------------------------------------------------------*/
/* ABSTRACT:                                                                 */
/* --------------------------------------------------------------------------*/
/* This function will initialize the CAN Module.                             */
/*===========================================================================*/


/* ==========================================================================*/
/* File revision history (top to bottom, first revision to last revision     */
/* ==========================================================================*/
/*                                                                           */
/* Date         user id     SCR       (description on following lines)       */
/* ----------   -------     ---     ----------------------------------       */
/* ==========================================================================*/
