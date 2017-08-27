#ifndef CCP2CONF_H
#define CCP2CONF_H
//=============================================================================
//
//       COPYRIGHT, 2003, Delphi Technologies, Inc. All Rights reserved
//
//       Delphi Confidential
//
//===========================================================================
// %name:            ccp2conf.h %
//
// created_by:       Jeffrey W. Davis
//
// date_created:     
//
// %derived_by:      zzrfyj %
//
// %date_modified:          %
//
// %version:         ctc_pt3#7 %
//
//=============================================================================

//===========================================================================
// Include Files
//===========================================================================
#include "reuse.h"
#include "ccp2main.h"
#include "stdint.h"

//===========================================================================
// Macro Definitions (application specific)
//===========================================================================

// Memory Map of Product RAM
#define CAL_START_ADDRESS    ((uint8_t*)(0xFE0000 ))
#define CAL_STOP_ADDRESS     ((uint8_t*)(0xFE03FF))

#define FLASH_START_ADDRESS    ((uint8_t*)(0x08000000 ))
#define FLASH_STOP_ADDRESS     ((uint8_t*)(0x08010000))

#define RAM_START_ADDRESS      ((uint8_t*)(0x20000000 ))
#define RAM_STOP_ADDRESS       ((uint8_t*)(0x20004000))

#define REGISTER_START_ADDRESS ((uint8_t*)(0x000000 ))
#define REGISTER_STOP_ADDRESS  ((uint8_t*)(0x000FFF))

#define EEPROM_START_ADDRESS ((uint8_t*)(  0x100000 ))
#define EEPROM_STOP_ADDRESS  ((uint8_t*)(  0x1001FF))

// DAQ lists are based on 16-bit addresses to save RAM.  This is the base address
// to be added to DAQ list pointers.
#define CCP_RAM_BASE_ADDRESS             (0x20000000)


// Obsolete Function Names
#define CCP_BackgroundTask     CCP_Periodic_Task

// CAN IDs for CCP
#define CCP_CANID_CRO       (0x7F0)
#define CCP_CANID_DTO       (0x7F4)
#define CCP_CANID_DAQ0      (0x7F5)
#define CCP_CANID_DAQ1      (0x7F6)
#define CCP_CANID_DAQ2      (0x7F7)
#define CCP_CANID_DAQ3      (0x7F8)


//===========================================================================
// Macro Definitions
//===========================================================================
#define INITIAL_SESSION_STATUS_STATE (0x00)

// ECU CCP definition            
#define CCP_CRO             (CCP_CANID_CRO)  // CCP CRO CAN Identifier     
#define CCP_DTO             (CCP_CANID_DTO)  // CCP DTO CAN Identifier     
#define CCP_STATION_ADDRESS (0x0000)         // CCP station address for ECU

// Older versions of the ITS software only support 24-bit addressing.  To make
// use of it with 32-bit addresses, the ITS_ADDRESS_MASK will supply the upper
// byte of the address.
// if ITS_PATCH_IS_REQUIRED == true, then
// address = address | ITS_ADDRESS_MASK
#define ITS_PATCH_IS_REQUIRED          (false)
#define ITS_ADDRESS_MASK               (0xA0000000)

// Applicable to little-endian CPUs only.  Depending on the instrumentation software,
// it may be necessary to transmit DAQ data in big endian format.  This may need to
// be true for ITS use.
#define FORCE_MSB_FIRST_DAQ_PROCESSING (false)

// TO BE REMOVED WHEN FUNCTIONS ARE ACTUALLY DEFINED!!!
#define CCP_Write_To_EEPROM_Memory_User( a, b ) { PARAM_NOT_USED(a); PARAM_NOT_USED(b); }
#define CCP_Write_To_FLASH_Memory_User( a, b ) { PARAM_NOT_USED(a); PARAM_NOT_USED(b); }


#define INST_Get_NVM_Failure()                  (true)
#define INST_Is_RAM_Present()                  (false)
#define INST_Get_Address_Active_Cal_Page()         (0)

//===========================================================================
// #defines for the 'dimensions' of the ODT's and DAQ's
//===========================================================================
#define NUMBER_OF_DAQ_LISTS ( 0x04 ) // Max. 0xFE DAQ lists total
#define NUMBER_OF_RESUMEABLE_DAQ_LISTS ( 0x04 )

//===========================================================================
// Enumeration Definitions (must be prior to constant definition)
//===========================================================================

//===========================================================================
// CCP 2.1 Command Codes
//
// The following commands are optional, if the ECU does not support DAQ:
//
//    GET_DAQ_SIZE,
//    SET_DAQ_PTR,
//    WRITE_DAQ,
//    START_STOP.
//
// If SELECT_CAL_PAGE is implemented, GET_ACTIVE_CAL_PAGE is required.
//===========================================================================
typedef enum CCP_Command_Code_tag
{
   CC_CONNECT             = 0x01,
   CC_SET_MTA             = 0x02,
   CC_DNLOAD              = 0x03,
   CC_UPLOAD              = 0x04,
   CC_TEST                = 0x05, // OPTIONAL COMMAND
   CC_START_STOP          = 0x06,
   CC_DISCONNECT          = 0x07,
   CC_START_STOP_ALL      = 0x08, // OPTIONAL COMMAND
   CC_GET_ACTIVE_CAL_PAGE = 0x09, // OPTIONAL COMMAND
   CC_SET_S_STATUS        = 0x0C, // OPTIONAL COMMAND
   CC_GET_S_STATUS        = 0x0D, // OPTIONAL COMMAND
   CC_BUILD_CHKSUM        = 0x0E, // OPTIONAL COMMAND
   CC_SHORT_UP            = 0x0F, // OPTIONAL COMMAND
   CC_CLEAR_MEMORY        = 0x10, // OPTIONAL COMMAND
   CC_SELECT_CAL_PAGE     = 0x11, // OPTIONAL COMMAND
   CC_GET_SEED            = 0x12, // OPTIONAL COMMAND
   CC_UNLOCK              = 0x13, // OPTIONAL COMMAND
   CC_GET_DAQ_SIZE        = 0x14,
   CC_SET_DAQ_PTR         = 0x15,
   CC_WRITE_DAQ           = 0x16,
   CC_EXCHANGE_ID         = 0x17,
   CC_PROGRAM             = 0x18, // OPTIONAL COMMAND
   CC_MOVE                = 0x19, // OPTIONAL COMMAND
   CC_GET_CCP_VERSION     = 0x1B,
   CC_DIAG_SERVICE        = 0x20, // OPTIONAL COMMAND
   CC_ACTION_SERVICE      = 0x21, // OPTIONAL COMMAND
   CC_PROGRAM_6           = 0x22, // OPTIONAL COMMAND
   CC_DNLOAD_6            = 0x23  // OPTIONAL COMMAND

}  CCP_Command_Code_T;


//===========================================================================
// Working Page/Reference Page code
//===========================================================================
typedef enum
{
   INST_CALIBRATION_PAGE_REFERENCE,
   INST_CALIBRATION_PAGE_WORKING
}  INST_Calibration_Page_T;


//===========================================================================
// Constant Definitions
//===========================================================================
// This defines the number of CCP command messages available.
#define CCP_COMMAND_QUANTITY (CC_DNLOAD_6 + 1)

// Constant array of function pointers for CCP commands
extern CCP_CRC_T (* const CCP_COMMAND_FCN_PTR[CCP_COMMAND_QUANTITY])( void );

#endif // CCP2CONF_H
