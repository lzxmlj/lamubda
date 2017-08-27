/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 *
 * MODULE NAME:
 * ccp2main.c
 *
 * SPECIFICATION REVISION:
 * ASAP Standard Can Calibration Protocol Version 2.1, 18, February 1999
 *
 *===========================================================================
 * REUSE:
 * DO NOT MODIFY THIS FILE.  Coordinate any desired changes with the re-use
 * group (contact Jeffrey W. Davis, 765-451-0478).
 *
\*===========================================================================*/

/*===========================================================================*\
 * Include Files
\*===========================================================================*/
#include "ccp2conf.h"
#include "ccp2main.h"
#include "ccp2mem.h"
#include "ccp2mode.h"
#include "ccp2daq.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

CCP_T ccp;

/*===========================================================================*\
 * Static Variables
\*===========================================================================*/

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/
void CCP_Send_CRM( void );

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Send_CRM
 * Purpose:      Send a Command Response Message.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void CCP_Send_CRM( void )
{
   CCP_Send_User( (HUGE uint8_t*)&ccp.response_data, CCP_DTO );
}

/*===========================================================================*\
 * Static Functions
\*===========================================================================*/

/*===========================================================================*\
 * Global Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Send_DTM
 * Purpose:      Send a Data Transmission Message.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void CCP_Send_DTM( void )
{
   CCP_Send_User( (HUGE uint8_t*)&ccp.DTM_data, ccp.message_id );
}

/*===========================================================================*\
 * Name:         CCP_DTM_Send_Callback
 * Purpose:      Called by application after successful transmission of a Data
 *               Transmission Message requested by CCP_Send_User(). Calling
 *               this function will immediately queue another DTM message if
 *               one is pending.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void CCP_DTM_Send_Callback( void )
{
   Transmit_Next_DAQ_Packet();
}

/*===========================================================================*\
 * Name:         CCP_Process_Command
 * Purpose:      Translate CCP message and formulate response.
 * Parameters:   uint8_t*, pointer to 8 bytes of received CAN data
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Setting ccp.delayed_response to true will inhibit the
 *               command processor from processing any further CCP commands
 *               until the flag is cleared.
\*===========================================================================*/
CCP_CRC_T CCP_Process_Command( HUGE uint8_t* ccp_command_ptr )
{
   CCP_CRC_T    error_to_return = CRC_UNKNOWN_COMMAND;
   unsigned int loop_index; /* coding standard exception */
   uint8_t      previous_status;
   bool         delay_override = false;

   /*
    * Record the current instrumentation mode
    */
   previous_status = ccp.session_status;

   /*
    * Check to see if we are processing another command
    */
   if ( !ccp.delayed_response )
   {
      /*
       * The command pointer data must be longword aligned.  A copy of the
       * data is made here to assure this occurs.  At the same time the
       * response message data buffer is cleared.
       */
      for ( loop_index = 0; loop_index < sizeof(CCP_Message_T); loop_index++ )
      {
         ccp.command_data.data_byte[  loop_index ] = ccp_command_ptr[ loop_index ];
         ccp.response_data.data_byte[ loop_index ] = CLEAR_DATA;
      }

      /*
       * Process CCP command
       */
      if ( (uint8_t)SS_RUN & ccp.session_status )
      {
         if ( CCP_COMMAND_QUANTITY > ccp.command_data.received_command_message.command_code )
         {
            /*
             * Execute CCP command
             */
            error_to_return = CCP_COMMAND_FCN_PTR[ccp.command_data.received_command_message.command_code]();
         }
         else
         {
            /*
             * Command out of range - error
             */
         }
      }
      else if( CC_CONNECT == ccp.command_data.received_command_message.command_code )
      {
         error_to_return = CCP_Connect();
      }
      else
      {
         /*
          * Command invalid - error
          */
      }
   }
   else
   {
      /*
       * Command in progress, indicate COMMAND_PROCESSOR_BUSY
       */
      error_to_return = CRC_COMMAND_PROCESSOR_BUSY;
      delay_override  = true;
   }

   /*
    * Only transmit a respond if connected or in response to a disconnect
    */
   if ( ((uint8_t)SS_RUN == ((uint8_t)SS_RUN & ccp.session_status)) ||
        ((uint8_t)SS_RUN == ((uint8_t)SS_RUN & previous_status)) )
   {
      /*
       * Fill in return message command code, command counter, return code, and
       * transmit the response message.
       */
      ccp.response_data.command_return_message.command_return_code = (uint8_t)error_to_return;
      ccp.response_data.command_return_message.echo_command_counter =
         ccp.command_data.received_command_message.command_counter;
      ccp.response_data.received_command_message.command_code = 0xFF;

      /*
       * Transmit message if not delayed or if delayed response is overridden
       */
      if ( !ccp.delayed_response || delay_override )
      {
         CCP_Send_CRM();
      }
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Send_Delayed_CRM
 * Purpose:      Send a delayed command response message.
 * Parameters:   CCP_CRC_T, command response code
 * Return Value: None
 * Design Info:  If a message requires a delayed response, the
 *               ccp.delayed_response flag should be set to true during
 *               processing of the command. When the response is ready to
 *               send, this function will complete and transmit the response,
 *               clearing the ccp.delayed_response flag.
\*===========================================================================*/
void CCP_Send_Delayed_CRM( CCP_CRC_T error_to_return )
{
   /*
    * Fill in return message command code, command counter, return code, and
    * transmit the response message.
    */
   ccp.response_data.command_return_message.command_return_code = (uint8_t)error_to_return;
   ccp.response_data.command_return_message.echo_command_counter =
      ccp.command_data.received_command_message.command_counter;
   ccp.response_data.received_command_message.command_code = 0xFF;
   CCP_Send_CRM();
   ccp.delayed_response = false;
}

/*===========================================================================*\
 * Name:         CCP_Periodic_Task
 * Purpose:      Perform periodic tasks such as BUILD_CHKSUM and MOVE.
 * Parameters:   None
 * Return Value: None
 * Design Info:  This task performs the background functions 256 bytes per
 *               call. It must be called at a fast enough rate to complete
 *               the task and send the CRM before the instrumentation tool
 *               times-out.
\*===========================================================================*/
void CCP_Periodic_Task( void )
{
   CCP_Build_Chksum_Periodic();
   CCP_Move_Periodic();
}

/*===========================================================================*\
 * Name:         CCP_Initialize
 * Purpose:      Initialize the CCP software subsystem.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void CCP_Initialize( void )
{
   CCP_Initialize_User();
   ccp.session_status = CCP_Read_Session_Status_User();
   ccp.ChecksumSize = 0;
   ccp.MoveSize     = 0;
   Initialize_DAQ_Parameters();
}
