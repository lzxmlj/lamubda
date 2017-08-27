//=============================================================================
//       COPYRIGHT 2005, Delphi Technologies, Inc. All Rights reserved
//                             Delphi Confidential
//
// --------------------------------------------------------------------------
// %name:            io_can_config.c %
//
// created_by:       qznx5b
//
// date_created:     Wed. Mar. 2 2005
//
// %derived_by:      rzf76d %
//
// %date_modified:   Mon May 28 13:08:51 2012 %
//
// %version:         11 %
//
//=============================================================================
#include "io_config.h" /* KeHWIO_CCP_CanPort */
#include "dd_can.h"  /* TRANSMIT_NEXT_DATA_OUT_PTR */
#include "stm32f37x.h"
#include "can_xfer.h"
#include "ccp2conf.h"
#include "reuse.h"

/* ============================================================================ *\
 * Global Var.
\* ============================================================================ */
uint8_t           CAN_Buffer[CCP_BUFFERLENGTH];


#define NULL_NOTIFIER_PTR ( ( void ( * )( uint16_t msg_id ) ) 0 )
/* ============================================================================ *\
 * Declared Objects
\* ============================================================================ */
void Callback_Application( CAN_message_ID_T message_id );
void Notify_Application(   CAN_message_ID_T message_id );
void Callback_Application_CCP(CAN_message_ID_T  msg_id );
void Callback_Application_KW2_CAN(CAN_message_ID_T  msg_id );


//=============================================================================
//       CAN Application Data Configuration
//=============================================================================

extern uint8_t  VcCAN_Message_StaleCount[CAN_NUMBER_OF_RECEIVE_MESSAGES];
/* ============================================================================ *\
 * !!!!!!Local Consts. This const must be located in shared bank
\* ============================================================================ */
const CAN_Transmit_message_parameter_T CAN_Transmit_Message_Parameter_Table[CAN_NUMBER_OF_TRANSMIT_MESSAGES] =
{

 /* ID                      MSG  NOTIFIER FUNCTION,   BUF    
  *                         LEN                       DEPTH
  */
    {1,                                 8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2,  0},        // ID: 0x7D4
    {2,                                 8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2,  0},        // ID: 0x7D4
    {3,                                 8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2,  0},        // ID: 0x7D4
    {4,                                 8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2,  0},        // ID: 0x7D4
    {5,				  8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {6,				 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {7,				 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {8,				 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {9,				8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {10,				 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {11,			 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {12,			 8,  NULL_NOTIFIER_PTR, 			 FIFO_CAN_MESSAGE,	 2,  0},		// ID: 0x7D4
    {13,			  8,  NULL_NOTIFIER_PTR,			  FIFO_CAN_MESSAGE,   2,  0},		 // ID: 0x7D4
    {14,			  8,  NULL_NOTIFIER_PTR,			  FIFO_CAN_MESSAGE,   2,  0},		 // ID: 0x7D4
    {15,			   8,  NULL_NOTIFIER_PTR,			   FIFO_CAN_MESSAGE,   2,  0},		  // ID: 0x7D4
    {16,			   8,  NULL_NOTIFIER_PTR,			   FIFO_CAN_MESSAGE,   2,  0},		  // ID: 0x7D4
    {CCP_CANID_DTO,                     8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2,  0},        // ID: 0x7D4
    {CCP_CANID_DAQ0,                    8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2, 0},        // ID: 0x7D5
    {CCP_CANID_DAQ1,                    8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2, 0},        // ID: 0x7D6
    {CCP_CANID_DAQ2,                    8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2, 0},        // ID: 0x7D7
    {CCP_CANID_DAQ3,                    8,  NULL_NOTIFIER_PTR,              FIFO_CAN_MESSAGE,   2, 0}        // ID: 0x7D8
};

/* The following table is to be read by wizard to get application parameters.
 *
 * !!!! MESSAGES IN THIS TABLE MUST APPEAR IN ASCENDING NUMERICAL ORDER
 */
const CAN_Receive_message_parameter_T CAN_Receive_Message_Parameter_Table[CAN_NUMBER_OF_RECEIVE_MESSAGES] =
{

 /* ID                       MSG    NOTIFIER FUNCTION,            BUF
  *                          LEN                                 DEPTH
  */
   {CCP_CANID_CRO,                8,    NULL_NOTIFIER_PTR,            FIFO_CAN_MESSAGE,       2   ,1 }  // ID: 0x7D0
};


void Notify_Application( CAN_message_ID_T  message_id )
{
}
static void Delay (int cnt) 
{
  while (cnt--);
}

void Callback_Application( CAN_message_ID_T   CanId)
{
uint8_t CAN_receive_message_number;

   for ( CAN_receive_message_number = 0;
         CAN_receive_message_number < CAN_NUMBER_OF_RECEIVE_MESSAGES;
         CAN_receive_message_number++)
   {
        if ( CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].CAN_message_ID == CanId )
        {
            VcCAN_Message_StaleCount[CAN_receive_message_number%CAN_NUMBER_OF_RECEIVE_MESSAGES] = 0;
            break;
        }
   }

}

void Callback_Application_CCP(CAN_message_ID_T  msg_id )
{
uint8_t receive_message[8];

   if( Get_Message( msg_id, receive_message ) )
   {
      CCP_Process_Command( receive_message );
   }
}

void Callback_Application_KW2_CAN(CAN_message_ID_T  msg_id )
{
uint16_t reset_reprog_pattern;
uint8_t  index, transmit_bytes[8];
uint8_t  carrier;

   if ( Get_Reprogram_Conditions_Met() )
   {
       for (index=0; index<8; index++)
       {
          transmit_bytes[index] = 0;
       }
       transmit_bytes[0] = PROF_ID;
       transmit_bytes[1] = 0x02;
       transmit_bytes[2] = 0x50;
       transmit_bytes[3] = 0x84;
       
       CAN1_Send_Msg( KW2_OVER_CAN_TRAN_ID, (uint8_t *)transmit_bytes);
       
       // to make sure message transmission has been completed
       Delay(2000UL);

//       DisableInterrupts();
   }
}


/**********************************************************************
*
* Revision History:                                                 
*                                                                    
* Rev.   YYMMDD Who RSM# Change      
* 1.1   130506  sj	   n/a    Add 2 New MSG
* ------ ------ --- ---- ---------------------------------------
*
**********************************************************************/
