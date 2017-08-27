#ifndef CCP2MODE_H
#define CCP2MODE_H
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
 * ccp2mode.h
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

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*
 * Session status byte masks
 */
#define SS_CAL                (0x01U)
#define SS_DAQ                (0x02U)
#define SS_RESUME             (0x04U)
#define SS_RESERVED           (0x08U) /* Reserved in specification */
#define SS_TMP_DISCONNECTED   (0x10U) /* Reserved in specification */
#define SS_LOGGING            (0x20U) /* Reserved in specification */
#define SS_STORE              (0x40U)
#define SS_RUN                (0x80U)
#define SS_MASK               (0xFFU) /* mask for session status   */

/*
 * Mask for session status bits which are allowed to be changed by SET_STATUS
 */
#define SS_VALID_MASK         (SS_CAL|SS_DAQ|SS_RESUME|SS_STORE)

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Connect
 * Purpose:      This command establishes a continuous logical point-to-point
 *               connection with the selected slave station for the master-
 *               slave command protocol.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Connect( void );

/*===========================================================================*\
 * Name:         CCP_Get_CCP_Version
 * Purpose:      This command performs a mutual identification of the
 *               protocol version used in the master and in the slave device
 *               to agree on a common protocol version.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Get_CCP_Version( void );

/*===========================================================================*\
 * Name:         CCP_Get_Seed
 * Purpose:      Get seed for key.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Get_Seed( void );

/*===========================================================================*\
 * Name:         CCP_Unlock
 * Purpose:      Unlocks the slave devices security protection (if
 *               applicable) using a 'key' computed from 'seed'.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Unlock( void );

/*===========================================================================*\
 * Name:         CCP_Disconnect
 * Purpose:      Disconnects the slave device.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Disconnect( void );

/*===========================================================================*\
 * Name:         CCP_Set_S_Status
 * Purpose:      Keeps the slave node informed about the current state of
 *               the calibration session.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Set_S_Status( void );

/*===========================================================================*\
 * Name:         CCP_Get_S_Status
 * Purpose:      Get Session Status.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Get_S_Status( void );

/*===========================================================================*\
 * Function Prototypes (user defined)
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Set_S_Status_User
 * Purpose:      User defined function for custom action upon SET_STATUS.
 * Parameters:   uint8_t, Old session status
 *               uint8_t, New session status
 * Return Value: None
\*===========================================================================*/
void CCP_Set_S_Status_User( uint8_t, uint8_t );

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

#endif /* CCP2MODE_H */
