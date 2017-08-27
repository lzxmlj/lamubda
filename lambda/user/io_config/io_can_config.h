#ifndef IO_CAN_CONFIG_H
#define IO_CAN_CONFIG_H
/******************************************************************************
 *
 * Filename:          io_can_config.h
 *
 * Description:       header file for io_can_config.h file.
 *
 * Global Functions Defined:
 *                    None
 * Static Functions Defined:
 *                    None
 * Inline Functions Defined:
 *                    None
 *
 * Forward Engine Management Systems
 *
 * Copyright 2006-2009, Delphi Technologies, Inc.
 * All rights reserved.
 *
 *****************************************************************************
 *
 * Current Module Info:
 * %full_name:     mt20u2#2/incl/io_can_config.h/8 %
 * %date_created:  Wed Jan 11 15:08:35 2012 %
 * %version:       8 %
 *
 *****************************************************************************/

/******************************************************************************
*  System Include Files
******************************************************************************/
#include "reuse.h"
#include "stdint.h"


/* ==========================================================================*\
 * Exported type declarations
\* ==========================================================================*/
typedef uint8_t   CAN_message_length_T;
typedef uint8_t   CAN_buffer_depth_T;
typedef uint32_t  CAN_message_ID_T;

typedef enum CAN_NODE_Tag
{
   CAN_NODE_A,
   CAN_NODE_B,
   CAN_NODE_C,
   CAN_NODE_D
}  CAN_NODE_T;

typedef enum
{
   REAL_TIME_CAN_MESSAGE,
   FIFO_CAN_MESSAGE
}  CAN_message_type_T;
typedef enum 
{
   CAN_PORT_0,
   CAN_PORT_4,
   CAN_PORT_NUMBER
}  CAN_PORT_T;

typedef struct
{
   CAN_message_ID_T CAN_message_ID;
   CAN_message_length_T CAN_message_length;
   void( *notifier_function_ptr )(uint16_t msg_id);
   CAN_message_type_T CAN_message_type;
   CAN_buffer_depth_T CAN_buffer_depth;
   CAN_PORT_T *can_port; // CAN port for message transmission
}  CAN_Transmit_message_parameter_T;

typedef struct
{
   CAN_message_ID_T CAN_message_ID;
   CAN_message_length_T CAN_message_length;
   void( *notifier_function_ptr )(uint16_t msg_id);
   CAN_message_type_T CAN_message_type;
   CAN_buffer_depth_T CAN_buffer_depth;
   uint8_t            CAN_monitor_index;
}  CAN_Receive_message_parameter_T;


/*===========================================================================*/
/* The following macros are intended to allow the code to be                 */
/* written at a higher level of abstraction, hopefully resulting             */
/* in code which is easier to read and maintain.  Of necessity,              */
/* higher levels of abstraction hide some detail.  In this case,             */
/* the detail that is hidden is the fact that references to message          */
/* parameters are references to an element of an array of structures.        */
/* This fact will have to be kept in mind when reading and maintaining       */
/* the code.  It is always necessary to have CAN_transmit_message_number     */
/* and/or CAN_receive_message_number defined within the scope of the use     */
/* of these macros.                                                          */
/*===========================================================================*/
#define TRANSMIT_CAN_MESSAGE_ID \
 CAN_Transmit_Message_Parameter_Table[CAN_transmit_message_number].\
 CAN_message_ID
#define TRANSMIT_CAN_MESSAGE_LENGTH \
 CAN_Transmit_Message_Parameter_Table[CAN_transmit_message_number].\
 CAN_message_length
#define TRANSMIT_NOTIFIER_FUNCTION_PTR \
 CAN_Transmit_Message_Parameter_Table[CAN_transmit_message_number].\
 notifier_function_ptr
#define TRANSMIT_CAN_MESSAGE_TYPE \
 CAN_Transmit_Message_Parameter_Table[CAN_transmit_message_number].\
 CAN_message_type
#define TRANSMIT_CAN_BUFFER_DEPTH \
 CAN_Transmit_Message_Parameter_Table[CAN_transmit_message_number].\
 CAN_buffer_depth

#define RECEIVE_CAN_MESSAGE_ID \
 CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].\
 CAN_message_ID
#define RECEIVE_CAN_MESSAGE_LENGTH \
 CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].\
 CAN_message_length
#define RECEIVE_NOTIFIER_FUNCTION_PTR \
 CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].\
 notifier_function_ptr
#define RECEIVE_CAN_MESSAGE_TYPE \
 CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].\
 CAN_message_type
#define RECEIVE_CAN_BUFFER_DEPTH \
 CAN_Receive_Message_Parameter_Table[CAN_receive_message_number].\
 CAN_buffer_depth


//=============================================================================
//       Macro Definitions
//=============================================================================
typedef enum 
{
   CAN_CCP_CMD,
   CAN_NUMBER_OF_RECEIVE_MESSAGES
}  CAN_Recv_Message_List_T;

#define CAN_RECV_MSG_TEST_ID   ((CAN_message_ID_T)(0x100))
#define CAN_RECV_MSG_ID        ((CAN_message_ID_T)(0x200))
#define RECEIVE_CANID_7FB      ((CAN_message_ID_T)(0x7FB))


#define ECM_TO_CVVL_ID  ((CAN_message_ID_T)(0x31E))
// Transmit CAN Messages ID
//
typedef enum 
{
   CAN_modbus,
   CAN_modbus1,
   CAN_modbus2,
   CAN_modbus3,
   CAN_modbus4,
   CAN_modbus5,
   CAN_modbus6,
   CAN_modbus7,
   CAN_modbus8,
   CAN_modbus9,
   CAN_modbus10,
   CAN_modbus11,
   CAN_modbus12,
   CAN_modbus13,
   CAN_modbus14,
   CAN_modbus15,
   CAN_CCP_DTO,
   CAN_CCP_DAQ1,
   CAN_CCP_DAQ2,
   CAN_CCP_DAQ3,
   CAN_CCP_DAQ4,
   CAN_NUMBER_OF_TRANSMIT_MESSAGES
}  CAN_Trans_Message_List_T;

#define CAN_TRANS_MSG_TEST_ID  ((CAN_message_ID_T)(0x101))
#define KW2_OVER_CAN_TRAN_ID   (0x7FA)

// Buffer length in total for all Receive/Transmit messages
// should be no less than: SUM(CAN msg length * (buffer depth+1))
//                       + sizeof(transmit_buffer_ctl_T ) (14) * number of msg transmitted
//                       + sizeof( receive_buffer_ctl_T ) (16) * number of msg received
// !!! check the address of Buffer_Pointer after memory allocation(2244)
#define CCP_BUFFERLENGTH         ( 2500 )//shenjian


/*===========================================================================*\
 * Exported variables
\*===========================================================================*/
extern const CAN_Transmit_message_parameter_T CAN_Transmit_Message_Parameter_Table[CAN_NUMBER_OF_TRANSMIT_MESSAGES];
extern const CAN_Receive_message_parameter_T CAN_Receive_Message_Parameter_Table[CAN_NUMBER_OF_RECEIVE_MESSAGES];
extern uint8_t CAN1_Send_Msg(uint32_t can_message_id, uint8_t* msg);

extern uint8_t    CAN_Buffer[CCP_BUFFERLENGTH];


#endif
/**********************************************************************
*
* Revision History:
*
* Rev.   YYMMDD Who RSM# Change 
* 1.1   130506  sj	   n/a    Add 2 New MSG
* ------ ------ --- ---- ----------------------------------------------
*
**********************************************************************/
