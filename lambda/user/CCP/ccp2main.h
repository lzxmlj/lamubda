#ifndef CCP2MAIN_H
#define CCP2MAIN_H
/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 *
 * MODULE NAME:
 * ccp2main.h
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

#include "reuse.h"
#include "stdint.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/
#define NUMBER_OF_INST_COMMAND_BYTES   (0x08)

#define CCP_VERSION_MAJOR              (0x02)
#define CCP_VERSION_MINOR              (0x01)

/*
 * This is used for clearing memory and CAN messages.
 */
#define CLEAR_DATA                     (0x00)
                                 
/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

/*
 * Command Return Codes
 */
typedef enum CCP_CRC_tag {
   CRC_NO_ERROR               = 0x00,
   CRC_DAQ_PROCESSOR_OVERLOAD = 0x01,
   CRC_COMMAND_PROCESSOR_BUSY = 0x10,
   CRC_DAQ_PROCESSOR_BUSY     = 0x11,
   CRC_INTERNAL_TIMEOUT       = 0x12,
   CRC_KEY_REQUEST            = 0x18,
   CRC_SESSION_STATUS_REQUEST = 0x19,
   CRC_COLD_START_REQUEST     = 0x20,
   CRC_CAL_DATA_INIT_REQUEST  = 0x21,
   CRC_DAQ_LIST_INIT_REQUEST  = 0x22,
   CRC_CODE_UPDATE_REQUEST    = 0x23,
   CRC_UNKNOWN_COMMAND        = 0x30,
   CRC_COMMAND_SYNTAX         = 0x31,
   CRC_PARAMETER_OUT_OF_RANGE = 0x32,
   CRC_ACCESS_DENIED          = 0x33,
   CRC_OVERLOAD               = 0x34,
   CRC_ACCESS_LOCKED          = 0x35,
   CRC_RESOURCE_NOT_AVAILABLE = 0x36

}  CCP_CRC_T;

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*
 * These data types are used to easily reference specific parts of the CAN
 * CCP messages.  Since this is a serial protocol, the endian of the CPU
 * must be considered for all accesses larger than byte.  Therefore, any
 * reference of 16 or 32-bits will have a suffix of 16 or 32 to make this
 * identification easier.
 */

typedef struct data_acquisition_tag
{
   uint8_t    packet_ID;
   uint8_t    data[ 7 ];

}  data_acquisition_T;

typedef struct received_command_message_tag
{
   uint8_t    command_code;
   uint8_t    command_counter;
   uint8_t    parameters[ 6 ];

}  received_command_message_T;

typedef struct command_return_message_tag
{
   uint8_t    packet_ID;
   uint8_t    command_return_code;
   uint8_t    echo_command_counter;
   uint8_t    parameters[ 5 ];

}  command_return_message_T;

typedef struct address_message_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    MTA_or_size;
   uint8_t    address_extension;
   uint32_t   address_location32;

}  address_message_T;

/*
 * Due to memory alignment, a longword read cannot be used here
 */
typedef struct memory_size_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    number_of_bytes32_uub;
   uint8_t    number_of_bytes32_umb;
   uint8_t    number_of_bytes32_lmb;
   uint8_t    number_of_bytes32_llb;
   uint8_t    unused_2[ 2 ];

}  memory_size_T;

typedef struct return_checksum_tag
{
   uint8_t    unused_1[ 3 ];
   uint8_t    checksum_size;
   uint16_t   size16;
   uint8_t    unused_2[ 2 ];

}  return_checksum_T;

typedef struct DAQ_number_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    number;
   uint8_t    number_of_ODTs;
   uint8_t    PID_or_element;
   uint8_t    unused_2[ 3 ];

}  DAQ_number_T;

typedef struct DAQ_size_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    number;
   uint8_t    unused_2;
   uint32_t   DTO_ID32;

}  DAQ_size_T;

typedef struct start_stop_message_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    mode;
   uint8_t    DAQ_list_number;
   uint8_t    last_ODT_number;
   uint8_t    event_channel_number;
   uint16_t   transmission_rate_prescaler16;

}  start_stop_message_T;

typedef struct seed_protection_tag
{
   uint8_t    unused_1[ 3 ];
   uint8_t    protection;
   uint8_t    unused_2[ 4 ];

}  seed_protection_T;

typedef struct session_status_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    receive_status;
   uint8_t    send_status;
   uint8_t    additional_information;
   uint8_t    DAQ_number;
   uint8_t    unused_2[ 2 ];

}  session_status_T;

typedef struct parameter_data_tag
{
   uint8_t    partition_PID;
   uint8_t    operation;
   uint8_t    PID;
   uint8_t    parameter_data;
   uint8_t    unused[ 4 ];

}  parameter_data_T;

typedef struct connect_message_tag
{
   uint8_t    unused_1[ 2 ];
   uint16_t   station_address16;
   uint8_t    unused_2[ 4 ];

}  connect_message_T;

typedef struct disconnect_message_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    disconnect_status;
   uint8_t    unused_2[ 1 ];
   uint16_t   station_address16;
   uint8_t    unused_3[ 1 ];

}  disconnect_message_T;

typedef struct unlock_command_tag
{
   uint8_t    unused_1[ 2 ];
   uint16_t   key16;
   uint8_t    unused_2[ 4 ];

}  unlock_command_T;

typedef struct dnload_5_tag
{
   uint8_t    unused_1[ 2 ];
   uint8_t    size;
   uint8_t    data[ 5 ];

}  dnload_5_T;

/*===========================================================================*\
 * This is the data type used for CAN message payloads, which can be any of
 * the previous CAN types.
\*===========================================================================*/

typedef union CCP_Message_tag
{
   uint8_t                    data_byte[ 8 ];
   address_message_T          address;
   memory_size_T              memory_size;
   return_checksum_T          return_checksum;
   DAQ_number_T               DAQ;
   DAQ_size_T                 DTO;
   start_stop_message_T       start_stop;
   seed_protection_T          seed;
   session_status_T           session_status;
   parameter_data_T           parameter_data;
   data_acquisition_T         data_acquisition_message;
   received_command_message_T received_command_message;
   command_return_message_T   command_return_message;
   connect_message_T          connect_message;
   disconnect_message_T       disconnect_message;
   unlock_command_T           unlock_command;
   dnload_5_T                 dnload_5;

}  CCP_Message_T;

/*
 * Main CCP information structure type
 */
typedef struct CCP_tag
{
   CCP_Message_T  command_data;
   CCP_Message_T  response_data;
   CCP_Message_T  DTM_data;         /* used for DAQ data */
   uint32_t       message_id;       /* used for DAQ transmission */
   uint32_t       ChecksumSize;
   uint32_t       MoveSize;
   uint16_t       Checksum;
   uint8_t        delayed_response; /* flag for long duration commands */
   uint8_t        session_status;
   uint8_t        send_status;

}  CCP_T;

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*
 * Main CCP information structure
 */
extern CCP_T ccp;

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Process_Command
 * Purpose:      Translate CCP message and formulate response.
 * Parameters:   uint8_t*, pointer to 8 bytes of received CAN data
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
extern CCP_CRC_T CCP_Process_Command( HUGE uint8_t* ccp_command_ptr );

/*===========================================================================*\
 * Name:         CCP_DTM_Send_Callback
 * Purpose:      Called by application after successful transmission of a Data
 *               Transmission Message requested by CCP_Send_User(). Calling
 *               this function will immediately queue another DTM message if
 *               one is pending.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
extern void CCP_DTM_Send_Callback( void );

/*===========================================================================*\
 * Name:         CCP_Send_Delayed_CRM
 * Purpose:      Send a delayed command response message.
 * Parameters:   CCP_CRC_T, command response code
 * Return Value: None
\*===========================================================================*/
extern void CCP_Send_Delayed_CRM( CCP_CRC_T );

/*===========================================================================*\
 * Name:         CCP_Send_DTM
 * Purpose:      Send a Data Transmission Message.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
extern void CCP_Send_DTM( void );

/*===========================================================================*\
 * Name:         CCP_Periodic_Task
 * Purpose:      Perform periodic tasks such as BUILD_CHKSUM and MOVE.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
extern void CCP_Periodic_Task( void );

/*===========================================================================*\
 * Name:         CCP_Initialize
 * Purpose:      Initialize the CCP software subsystem.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
extern void CCP_Initialize( void );

/*===========================================================================*\
 * Function Prototypes (user defined)
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Send_User
 * Purpose:      Application specific function to transmit a CAN message.
 * Parameters:   uint8_t*, pointer to 8 bytes of data
 *               uint32_t, CAN ID
 * Return Value: bool, false on success
\*===========================================================================*/
extern bool CCP_Send_User( uint8_t*, uint32_t );

/*===========================================================================*\
 * Name:         CCP_Write_To_Register_User
 * Purpose:      Application specific register handling code.
 * Parameters:   uint8_t*, destination address
 *               uint8_t, data
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
extern CCP_CRC_T CCP_Write_To_REGISTER_Memory_User( HUGE uint8_t*, uint8_t );

/*===========================================================================*\
 * Name:         CCP_Read_Session_Status_User
 * Purpose:      Application specific NVM read of session status byte.
 * Parameters:   None
 * Return Value: uint8_t, session_status
\*===========================================================================*/
extern uint8_t CCP_Read_Session_Status_User( void );

/*===========================================================================*\
 * Name:         CCP_Initialize_User
 * Purpose:      Application specific initialization code.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
extern void CCP_Initialize_User( void );

/*===========================================================================*\
 * Name:         CCP_Read_From_REGISTER_Memory_User
 * Purpose:      Application specific register handling code.
 * Parameters:   uint8_t * - address               
 * Return Value: uint8_t - data
\*===========================================================================*/
extern uint8_t CCP_Read_From_REGISTER_Memory_User( HUGE uint8_t* address);

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

#endif /* CCP2MAIN_H */
