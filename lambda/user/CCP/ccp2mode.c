/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 * This module contains support for the following CCP 2.1 commands:
 *
 *    CONNECT
 *    DISCONNECT
 *    GET_CCP_VERSION
 *    GET_SEED
 *    UNLOCK
 *    GET_S_STATUS
 *    SET_S_STATUS
 *
 * MODULE NAME:
 * ccp2mode.c
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

#include "ccp2main.h"
#include "ccp2mem.h"
#include "ccp2mode.h"
#include "ccp2daq.h"
#include "ccp2conf.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

/* These are used for locking and unlocking the ECU; since there is no
 * unlocking for this program, both are set to zero.
 */
typedef enum
{
   NOT_PROTECTED = 0x00,
   PROTECTED     = 0x00

} SeedProtectionStates_T;

/* These are used for assigning privilege levels for Unlock.
 * Only FULL_PRIVILEGE_LEVEL is used in this program.
 */
typedef enum
{
   NO_PRIVILEGE                        = 0x00,
   READ_PRIVILEGE_LEVEL                = 0x02,
   READ_AND_CALIBRATE_PRIVILEGE_LEVEL  = 0x03,
   FULL_PRIVILEGE_LEVEL                = 0x43 /* program, calibrate, & read access */

}  ECU_PrivilegeLevel_T;

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*===========================================================================*\
 * Static Variables
\*===========================================================================*/

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

/*===========================================================================*\
 * Static Functions
\*===========================================================================*/

/*===========================================================================*\
 * Global Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Connect
 * Purpose:      This command establishes a continuous logical point-to-point
 *               connection with the selected slave station for the master-
 *               slave command protocol.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  All following protocol commands refer to this station only,
 *               until another station is selected. A CONNECT command to
 *               another station temporarily disconnects the active station
 *               (see DISCONNECT). A CONNECT command to an already connected
 *               station is acknowledged. A slave device does not respond to
 *               any commands unless being addressed by a prior CONNECT
 *               command with the correct station address. The station
 *               address is specified as a number in little-endian byte order
 *               (Intel format, low byte first).
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = CONNECT 0x01
 * 1           byte           Command Counter = CTR
 * 2,3         word           station address (Intel format)
 * 3..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Connect( void )
{
   uint8_t old_status = ccp.session_status;

   /*
    * station_address16 is stored little endian via CCP
    */
   if ( CCP_STATION_ADDRESS ==
        FIX_LITTLE_ENDIAN16( ccp.command_data.connect_message.station_address16 ) )
   {
      ccp.session_status |= (uint8_t)SS_RUN | SS_CAL;
      ccp.session_status &= (uint8_t)((~SS_TMP_DISCONNECTED)&SS_MASK);
   }
   else if ( (uint8_t)SS_RUN & ccp.session_status )
   {
      /* Other station selected, temporary disconnect */
      ccp.session_status |= (uint8_t)SS_TMP_DISCONNECTED;
      ccp.session_status &= (uint8_t)((~SS_RUN)&SS_MASK);
   }
   else
   {
      /* do nothing */
   }

   CCP_Set_S_Status_User( old_status, ccp.session_status );

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Get_CCP_Version
 * Purpose:      This command performs a mutual identification of the
 *               protocol version used in the master and in the slave device
 *               to agree on a common protocol version.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  This command is expected to be executed prior to the
 *               EXCHANGE_ID command.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = GET_CCP_VERSION 0x1B
 * 1           byte           Command Counter = CTR
 * 2           byte           Main Protocol version (desired)
 * 3           byte           Release within version (desired)
 * 4..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           Main Protocol version as implemented
 * 4           byte           Release within version as implemented
 * 5..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Get_CCP_Version( void )
{
   ccp.response_data.command_return_message.parameters[0] = CCP_VERSION_MAJOR;
   ccp.response_data.command_return_message.parameters[1] = CCP_VERSION_MINOR;

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Get_Seed
 * Purpose:      Get seed for key.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Only one resource or function may be requested with one
 *               GET_SEED command. If more than one resource is requested,
 *               the GET_SEED command together with the following UNLOCK
 *               command has to be performed multiple times.
 *
 *               Returns 'seed' data for a seed&key algorithm for computing
 *               the 'key' to unlock the requested function for authorized
 *               access.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = GET_SEED 0x12
 * 1           byte           Command Counter = CTR
 * 2           byte           Requested slave resource or function (Resource
 *                            Mask)
 * 3..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           Protection status (true or false)
 * 4..7        bytes          'seed' data
 *
 * NOTE: Protection is currently not supported. Currently returns a constant
 *       protection status (JWD).
\*===========================================================================*/
CCP_CRC_T CCP_Get_Seed( void )
{
   ccp.response_data.seed.protection = NOT_PROTECTED;

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Unlock
 * Purpose:      Unlocks the slave devices security protection (if
 *               applicable) using a 'key' computed from 'seed'.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = UNLOCK 0x13
 * 1           byte           Command Counter = CTR
 * 2...        bytes          'key'
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           Current Provilege Status (Resource Mask)
 * 4..7        bytes          don't care
 *
 * NOTE: Protection is currently not supported. The privilege status
 *       resource mask returned is constant (JWD).
\*===========================================================================*/
CCP_CRC_T CCP_Unlock( void )
{
   ccp.response_data.command_return_message.parameters[ 0 ] = FULL_PRIVILEGE_LEVEL;

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Disconnect
 * Purpose:      Disconnects the slave device.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The disconnect can be temporary, setting the slave device in
 *               an "off line" state or with parameter 0x01 terminating the
 *               calibration session.
 *
 *               Terminating the session invalidates all state information
 *               and resets the slave protection status.
 *
 *               A temporary disconnect doesn't stop the transmission of DAQ
 *               messages. The MTA values, the DAQ setup, the session status,
 *               and the protection status are unaffected by the temporary
 *               disconnect and remain unchanged.
 *
 *               If the ECU supports the resume feature and the resume bit
 *               was set with a SET_SESSION_STATUS command, the DAQ related
 *               functions behave like in a temporary disconnect. The
 *               protection status for DAQ remains unlocked.
 *
 *               Station address is specified as a number in little-endian
 *               byte order (Intel format, low byte first).
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = DISCONNECT 0x07
 * 1           byte           Command Counter = CTR
 * 2           byte           0x00 temporary, 0x01 end of session
 * 3           byte           don't care
 * 4,5         word           Station address (Intel format)
 * 6..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Disconnect( void )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint16_t  temp_station_address;
   uint8_t   old_status = ccp.session_status;

   /*
    * station_address is stored little endian via CCP
    */
   temp_station_address = FIX_LITTLE_ENDIAN16( ccp.command_data.disconnect_message.station_address16 );

   if( CCP_STATION_ADDRESS == temp_station_address )
   {
      if ( 0x00 == ccp.command_data.disconnect_message.disconnect_status )
      {
         /*
          * Temporary disconnect
          */
         ccp.session_status |= (uint8_t)SS_TMP_DISCONNECTED;
         ccp.session_status &= (uint8_t)((~SS_RUN)&SS_MASK);
         CCP_Set_S_Status_User( old_status, ccp.session_status );
      }
      else if ( 0x01 == ccp.command_data.disconnect_message.disconnect_status )
      {
         /*
          * Terminate session
          */
         ccp.session_status &= (uint8_t)((~SS_LOGGING)&SS_MASK);
         ccp.session_status &= (uint8_t)((~SS_RUN)&SS_MASK);
         CCP_Set_S_Status_User( old_status, ccp.session_status );
      }
      else
      {
         error_to_return = CRC_COMMAND_SYNTAX;
      }
   }
   else
   {
      /*
       * CCP specification does not clearly define errors or error actions
       */
      error_to_return = CRC_COMMAND_SYNTAX;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Set_S_Status
 * Purpose:      Keeps the slave node informed about the current state of
 *               the calibration session.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The session status bits are read/write to the slave and are
 *               to be cleared on power-up, on session log-off and in
 *               applicable fault conditions.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = SET_S_STATUS 0x0C
 * 1           byte           Command Counter = CTR
 * 2           byte           Session status bits (see table below)
 * 3..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
 *
 * Session status bits:
 * 0  SS_CAL              Calibration data initialized
 * 1  SS_DAQ              DAQ list(s) initialized
 * 2  SS_RESUME           Request to save DAQ setup during shutdown in ECU.
 * 3  SS_RESERVED         reserved (in CCP specification)
 * 4  SS_TMP_DISCONNECTED reserved (in CCP specification)
 * 5  SS_LOGGING          reserved (in CCP specification)
 * 6  SS_STORE            Request to save calibration data during shut-down
 * 7  SS_RUN              Session in progress
 *
 * NOTE: Bits 3,4,5 are reserved in the specification. In this implementation
 *       these bits are re-used to indicate data acquisition and temporary
 *       disconnection.  Therefore, we must not allow these bits to be
 *       cleared by this routine.
\*===========================================================================*/
CCP_CRC_T CCP_Set_S_Status( void )
{
   uint8_t old_status = ccp.session_status;

   /*
    * Clear bits that we will allow to be modified
    */
   ccp.session_status &= (uint8_t)((~SS_VALID_MASK)&SS_MASK);

   /*
    * Set allowed bits
    */
   ccp.session_status |= ccp.command_data.session_status.receive_status & (uint8_t)SS_VALID_MASK;

   if ( !((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & old_status))
        && ((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & ccp.session_status)) )
   {
      CCP_Store_Resume_Data();
   }
   else if ( ((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & old_status))
             && !((uint8_t)SS_RESUME == ((uint8_t)SS_RESUME & ccp.session_status)) )
   {
      CCP_Initialize_Resume_Data();
   }
   else
   {
      /* do nothing */
   }

   CCP_Set_S_Status_User( old_status, ccp.session_status );

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Get_S_Status
 * Purpose:      Get Session Status.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Note: the use of additional status information is
 *               manufacturer and/or project specific, it is not part of this
 *               protocol specification. For example, additional status
 *               information could contain an incremental checksum result,
 *               that keeps track of the current session activities. If the
 *               return information does not contain additional status
 *               information, the additional status information qualifier
 *               has to be false (0). If the additional status information
 *               is not false, it may be used to determine the type of
 *               additional status information.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = GET_S_STATUS 0x0D
 * 1           byte           Command Counter = CTR
 * 2..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           Session status
 * 4           byte           additional status information qualifier
 * 5..7        bytes          additional status information (optional)
 *
\*===========================================================================*/
CCP_CRC_T CCP_Get_S_Status( void )
{
   ccp.response_data.session_status.send_status            = ccp.session_status;
   ccp.response_data.session_status.additional_information = 0;

   return CRC_NO_ERROR;
}
