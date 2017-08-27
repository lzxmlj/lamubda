//===========================================================================
//
//       COPYRIGHT, 2005, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
//
//===========================================================================
// %name:            ccp2conf.c %
//
// created_by:      
//
// date_created:     
//
// %derived_by:      zzrfyj %
//
// %date_modified:           %
//
// %version:         ctc_pt3#8 %
//
//===========================================================================
// DESCRIPTION:
//
// SPECIFICATION REVISION:
// ASAP Standard Can Calibration Protocol Version 2.1, 18, February 1999
//
//===========================================================================

//===========================================================================
// Include Files
//===========================================================================
#include "ccp2conf.h"
#include "ccp2daq.h"
#include "ccp2main.h"
#include "ccp2mem.h"
#include "ccp2mode.h"
#include "ccp2nvmd.h"
#include "can_xfer.h"
#include "stdint.h"
//===========================================================================
// Function Prototypes (must be prior to constant definition)
//===========================================================================
CCP_CRC_T CCP_Command_Unavailable( void );
CCP_CRC_T CCP_Get_Active_Cal_Page( void );
CCP_CRC_T CCP_Select_Cal_Page( void );
CCP_CRC_T CCP_Exchange_ID( void );

static bool CCP_WorkingPage_Switch_Rquest(uint8_t* address);
//===========================================================================
// Type Definitions
//===========================================================================
// ECU Identification Structure (INCA 4.x)
typedef struct EIS_tag
{
   uint8_t version;
   uint8_t unused1[6];
   uint8_t unused2[6];
   uint8_t unused3;
   uint8_t session_configuration_id[4];
   uint8_t unused4[8];
   uint8_t unused5[8];
   uint8_t unused6[2];

}  EIS_T;

//===========================================================================
// Macro Definitions
//===========================================================================
#define EIS_VERSION (0x02)
#define CCP_M_PGM 0x40
#define CCP_M_DAQ 0x02
#define CCP_M_CAL 0x01


//===========================================================================
// Variables
//===========================================================================
static EIS_T   CCP_EIS;    // Storage for the EIS
static uint8_t daq0_data_buffer[ CCP_DATA_BUFFER_SIZE_IN_BYTES( DAQ0_NUMBER_OF_ODTS ) ];
static uint8_t daq1_data_buffer[ CCP_DATA_BUFFER_SIZE_IN_BYTES( DAQ1_NUMBER_OF_ODTS ) ];
static uint8_t daq2_data_buffer[ CCP_DATA_BUFFER_SIZE_IN_BYTES( DAQ2_NUMBER_OF_ODTS ) ];
static uint8_t daq3_data_buffer[ CCP_DATA_BUFFER_SIZE_IN_BYTES( DAQ3_NUMBER_OF_ODTS ) ];

//===========================================================================
// Constant Definitions
//===========================================================================
// Constant array of function pointers for CCP commands
CCP_CRC_T (* const CCP_COMMAND_FCN_PTR[CCP_COMMAND_QUANTITY])( void ) =
{  &CCP_Command_Unavailable,  // 0x00
   &CCP_Connect,              // 0x01 CC_CONNECT
   &CCP_Set_MTA,              // 0x02 CC_SET_MTA
   &CCP_Dnload,               // 0x03 CC_DNLOAD
   &CCP_Upload,               // 0x04 CC_UPLOAD
   &CCP_Command_Unavailable,  // 0x05 CC_TEST
   &CCP_Start_Stop,           // 0x06 CC_START_STOP
   &CCP_Disconnect,           // 0x07 CC_DISCONNECT
   &CCP_Start_Stop_All,       // 0x08 CC_START_STOP_ALL
   &CCP_Get_Active_Cal_Page,  // 0x09 CC_GET_ACTIVE_CAL_PAGE
   &CCP_Command_Unavailable,  // 0x0A
   &CCP_Command_Unavailable,  // 0x0B
   &CCP_Set_S_Status,         // 0x0C CC_SET_S_STATUS
   &CCP_Get_S_Status,         // 0x0D CC_GET_S_STATUS
   &CCP_Build_Chksum,         // 0x0E CC_BUILD_CHKSUM
   &CCP_Short_Up,             // 0x0F CC_SHORT_UP
   &CCP_Clear_Memory,         // 0x10 CC_CLEAR_MEMORY
   &CCP_Select_Cal_Page,      // 0x11 CC_SELECT_CAL_PAGE
   &CCP_Get_Seed,             // 0x12 CC_GET_SEED
   &CCP_Unlock,               // 0x13 CC_UNLOCK
   &CCP_Get_DAQ_Size,         // 0x14 CC_GET_DAQ_SIZE
   &CCP_Set_DAQ_Ptr,          // 0x15 CC_SET_DAQ_PTR
   &CCP_Write_DAQ,            // 0x16 CC_WRITE_DAQ
   &CCP_Exchange_ID,          // 0x17 CC_EXCHANGE_ID
   &CCP_Program,              // 0x18 CC_PROGRAM
   &CCP_Move,                 // 0x19 CC_MOVE
   &CCP_Command_Unavailable,  // 0x1A
   &CCP_Get_CCP_Version,      // 0x1B CC_GET_CCP_VERSION
   &CCP_Command_Unavailable,  // 0x1C
   &CCP_Command_Unavailable,  // 0x1D
   &CCP_Command_Unavailable,  // 0x1E
   &CCP_Command_Unavailable,  // 0x1F
   &CCP_Command_Unavailable,  // 0x20 CC_DIAG_SERVICE
   &CCP_Command_Unavailable,  // 0x21 CC_ACTION_SERVICE
   &CCP_Program_6,            // 0x22 CC_PROGRAM_6
   &CCP_Dnload_6              // 0x23 CC_DNLOAD_6
};

const CCP_DAQ_Configuration_T CCP_DAQ_Configuration[ NUMBER_OF_DAQ_LISTS ] =
{
   // #ODTs, #elements/ODT, config buffer, data buffer, resume_supported
   {  // DAQ0 
      DAQ0_NUMBER_OF_ODTS,
      DAQ0_ELEMENTS_PER_ODT,
      &daq0_config_buffer_NVM[0],
      &daq0_data_buffer[0],
      CCP_RESUME_SUPPORTED
   },
   {  // DAQ1
      DAQ1_NUMBER_OF_ODTS,
      DAQ1_ELEMENTS_PER_ODT,
      &daq1_config_buffer_NVM[0],
      &daq1_data_buffer[0],
      CCP_RESUME_SUPPORTED
   },
   {  // DAQ2
      DAQ2_NUMBER_OF_ODTS,
      DAQ2_ELEMENTS_PER_ODT,
      &daq2_config_buffer_NVM[0],
      &daq2_data_buffer[0],
      CCP_RESUME_SUPPORTED
   },
   {  // DAQ3
      DAQ3_NUMBER_OF_ODTS,
      DAQ3_ELEMENTS_PER_ODT,
      &daq3_config_buffer_NVM[0],
      &daq3_data_buffer[0],
      CCP_RESUME_SUPPORTED
   }
};

//===========================================================================
// Global Functions
//===========================================================================

//===========================================================================
// Name:         CCP_Command_Unavailable
// Purpose:      Stub function for unavailable command codes.
// Parameters:   None
// Return Value: CCP_CRC_T, command response code
// Design Info:  None
//===========================================================================
CCP_CRC_T CCP_Command_Unavailable( void )
{
   return CRC_UNKNOWN_COMMAND;
}
//===========================================================================
// Name:         CCP_Clear_Session_Configuration_ID
// Purpose:      None
// Parameters:   None
// Return Value: None
// Design Info:  Application specific functionality
//===========================================================================
static void CCP_Clear_Session_Configuration_ID( void )
{
   // Clear NVM Session Configuration ID
   ccp_session_configuration_id_NVM[0] = 0xEF;
   ccp_session_configuration_id_NVM[1] = 0xBE;
   ccp_session_configuration_id_NVM[2] = 0xAD;
   ccp_session_configuration_id_NVM[3] = 0xDE;
}

//===========================================================================
// Name:         CCP_Exchange_ID
// Purpose:      None
// Parameters:   None
// Return Value: CCP_CRC_T, command response code
// Design Info:  Application specific functionality
//
//               Mask bits
//               Bit 7 reserved
//               Bit 6 PGM
//               Bit 5 reserved
//               Bit 4 reserved
//               Bit 3 reserved
//               Bit 2 reserved
//               Bit 1 DAQ
//               Bit 0 CAL
//
//               Resource Availability: if bit=true specified resource is
//               available.
//               Resource Protection: if bit=true specified resource is
//               protected against unauthorized access (needs UNLOCK)
//===========================================================================
CCP_CRC_T CCP_Exchange_ID( void )
{
   ccp.response_data.command_return_message.parameters[0] = sizeof( EIS_T );
   ccp.response_data.command_return_message.parameters[1] =
            0x00;   // data type qualifier of slave device ID
   ccp.response_data.command_return_message.parameters[2] =
            CCP_M_PGM | CCP_M_DAQ | CCP_M_CAL; // resource availability mask
   ccp.response_data.command_return_message.parameters[3] =
            0x00; // resource protection mask

   return Move_MTA( (uint8_t*)&CCP_EIS, MTA0_POINTER );
}

//===========================================================================
// Name:         CCP_Set_S_Status_User
// Purpose:      Application specific action when session status byte is modified
// Parameters:   uint8_t old_session_status - session status prior to being updated
//               uint8_t session_status - current session status
// Return Value: None
// Design Info:  On SS_RESUME set, store the session configuration ID to NVM.
//               On SS_RESUME clear, clear the NVM session configuration ID.
//===========================================================================
void CCP_Set_S_Status_User( uint8_t old_session_status, uint8_t session_status )
{
   int i; // coding standard exception 

   if( !((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & old_session_status)) &&
       ((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & session_status))  )
   {
      // Store Session Configuration ID
      for ( i = 0; i < 4; i++ )
      {
         ccp_session_configuration_id_NVM[i] = CCP_EIS.session_configuration_id[i];
      }
   }
   else if( ((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & old_session_status)) &&
            !((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & session_status)) )
   {
      // Clear Session Configuration ID
      CCP_Clear_Session_Configuration_ID();
   }
   else
   {
      // do nothing
   }

   ccp_session_status_NVM = session_status;
}

//===========================================================================
// Name:         CCP_Initialize_User
// Purpose:      Application specific initialization code.
// Parameters:   None
// Return Value: None
// Design Info:  INCA 4.x requires that the session configuration ID in the
//               EIS be maintained across a powerdown for resume support. The
//               initialization and restoration of the session configuration
//               ID is handled here. The CCP resume data is also initialized
//               in the event of a NVM failure.
//===========================================================================
void CCP_Initialize_User( void )
{

   int i; // coding standard exception

   // Initialize NVM Data on NVM Failure
   if( INST_Get_NVM_Failure() )
   {
      CCP_Initialize_Resume_Data();
      CCP_Clear_Session_Configuration_ID();
      ccp_session_status_NVM = INITIAL_SESSION_STATUS_STATE;
   }

   // Restore EIS
   CCP_EIS.version = EIS_VERSION;

   for( i = 0; i < 4; i++ )
   {
      CCP_EIS.session_configuration_id[i] = ccp_session_configuration_id_NVM[i];
   }

}

//===========================================================================
// Name:         CCP_Read_Session_Status_User
// Purpose:      Application specific NVM read of session status byte.
// Parameters:   None
// Return Value: uint8_t - session_status
//===========================================================================
uint8_t CCP_Read_Session_Status_User( void )
{
   return ccp_session_status_NVM;
}

//===========================================================================
// Name:         CCP_Select_Cal_Page
// Purpose:      Select Calibration Data Page.
// Parameters:   None
// Return Value: CCP_CRC_T, command response code
// Design Info:  This command's function depends on the ECU implementation.
//               The previously initialized MTA0 points to the start of the
//               calibration data page that is selected as the currently
//               active page by this command.
//
//               Using two blocks of ECU memory for calibration, the SET_MTA
//               and the SELECT_CAL_PAGE commands can be used for a kind of
//               'emergency interrupt' from the master device in order to
//               bring the slave system into a 'safe state' by preparing the
//               change of these two memory blocks with the SET_MTA command
//               and executing the change immediately with SELECT_CAL_PAGE.
//
// Structure of data in CRO:
// Position    Type           Description
// =========== ============== ==============================================
// 0           byte           Command Code = SELECT_CAL_PAGE 0x11
// 1           byte           Command Counter = CTR
// 2..7        bytes          don't care
//
// Structure of data in DTO:
// Position    Type           Description
// =========== ============== ==============================================
// 0           byte           Packet ID: 0xFF
// 1           byte           Command Return Code
// 2           byte           Command Counter = CTR
// 3..7        bytes          don't care
//===========================================================================
CCP_CRC_T CCP_Select_Cal_Page( void )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   INST_Calibration_Page_T requested_page;
   
   requested_page = INST_CALIBRATION_PAGE_REFERENCE;

   // one page for CAL only
//   INST_Set_Active_Calibration_Page( requested_page );

   return error_to_return;
}

//===========================================================================
// Name:         CCP_Get_Active_Cal_Page
// Purpose:      Get currently active Calibration Page.
// Parameters:   None
// Return Value: CCP_CRC_T, command response code
// Design Info:  This command returns the start address of the calibration
//               page that is currently active in the slave device.
//
// Structure of data in CRO:
// Position    Type           Description
// =========== ============== ==============================================
// 0           byte           Command Code = GET_ACTIVE_CAL_PAGE 0x09
// 1           byte           Command Counter = CTR
// 2..7        bytes          don't care
//
// Structure of data in DTO:
// Position    Type           Description
// =========== ============== ==============================================
// 0           byte           Packet ID: 0xFF
// 1           byte           Command Return Code
// 2           byte           Command Counter = CTR
// 3           byte           Address extension
// 4..7        unsigned long  Address
//===========================================================================
CCP_CRC_T CCP_Get_Active_Cal_Page( void )
{
   ccp.response_data.address.address_location32 =
      FIX_ADDRESS( INST_Get_Address_Active_Cal_Page() );

   return CRC_NO_ERROR;
}

//===========================================================================
// Name:         CCP_Location_Of_Address_User
// Purpose:      Application specific function will return an enumerated type
//               that indicates the type of memory where the input address is
//               located.
// Parameters:   uint8_t*, This is the address of the location to be determined.
// Return Value: MemoryArea_T
// Design Info:  None
//===========================================================================
MemoryArea_T CCP_Location_Of_Address_User( uint8_t* address )
{
   MemoryArea_T location_to_return;

   if( ( RAM_START_ADDRESS <= address ) &&
       ( RAM_STOP_ADDRESS  >= address ) )
   {
      location_to_return = RAM;
   }
   else if( ( FLASH_START_ADDRESS <= address ) &&
            ( FLASH_STOP_ADDRESS  >= address ) )
   {
      location_to_return = FLASH_O;
   }
   else if( ( CAL_START_ADDRESS <= address ) &&
            ( CAL_STOP_ADDRESS  >= address ) )
   {
      location_to_return = CAL_FLASH;
   }
   else if( ( REGISTER_START_ADDRESS <= address ) &&
            ( REGISTER_STOP_ADDRESS  > address ) )
   {
      location_to_return = REGISTER;
   }
   else if( ( EEPROM_START_ADDRESS <= address ) &&
            ( EEPROM_STOP_ADDRESS  > address ) )
   {
      location_to_return = EEPROM;
   }
   else
   {
      //location_to_return = RAM;
      location_to_return = OTHER;
   }

   return location_to_return;
}

//===========================================================================
// Name:         CCP_Write_To_REGISTER_Memory_User
// Purpose:      Application specific register handling code.
// Parameters:   uint8_t * - address
//               uint8_t - data
// Return Value: CCP_CRC_T, command response code
//===========================================================================
CCP_CRC_T CCP_Write_To_REGISTER_Memory_User( uint8_t* address, uint8_t data )
{
   CCP_CRC_T error_to_return = CRC_RESOURCE_NOT_AVAILABLE;

   // Writes to calibration page need be offset from the 80040000 segment to
   // the yyyyyy segment.
   if( INST_Is_RAM_Present() )
   {
      *address = data;

      error_to_return = CRC_NO_ERROR;
   }

   return error_to_return;
}

//===========================================================================
// Name:         CCP_Send_User
// Purpose:      Application specific function to transmit CAN message
// Parameters:   uint8_t*, pointer to 8 bytes of data
//               uint32_t, CAN ID
// Return Value: bool, false if successful
// Design Info:  Transmit the 8-byte CAN message using the ID supplied.
//               After message has been successfully transmitted call
//               CCP_DTM_Send_Callback() only for data acquisition messages.
//===========================================================================
bool CCP_Send_User( uint8_t* ccp_response_ptr, uint32_t CAN_ID )
{
   // Transmit_Message returns true if successful. 
   return (bool) CAN1_Send_Msg( CAN_ID, ccp_response_ptr );
}
/*===========================================================================*\
 * Name:         CCP_Read_From_REGISTER_Memory_User
 * Purpose:      Application specific register handling code.
 * Parameters:   uint8_t * - address               
 * Return Value: uint8_t - data
\*===========================================================================*/
uint8_t CCP_Read_From_REGISTER_Memory_User( HUGE uint8_t* address)
{
   return (*address);
}

/*===========================================================================*\
 * Name:         CCP_WorkingPage_Switch_Rquest
 * Purpose:      Application specific register handling code.
 * Parameters:   uint8_t * - address               
 * Return Value: bool - 1: switch to working page request
 *                      0: switch to reference page request
\*===========================================================================*/
static bool CCP_WorkingPage_Switch_Rquest(uint8_t* address)
{
bool return_code;

      return_code = true;

   return return_code;
}
