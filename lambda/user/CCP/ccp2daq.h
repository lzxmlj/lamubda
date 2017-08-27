#ifndef CCP2DAQ_H
#define CCP2DAQ_H
/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 * This module contains support for the following CCP 2.1 commands:
 *
 *    GET_DAQ_SIZE
 *    GET_DAQ_PTR
 *    WRITE_DAQ
 *    START_STOP
 *    START_STOP_ALL
 *
 * MODULE NAME:
 * ccp2daq.h
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
#include "ccp2main.h"
#include "ccp2conf.h"
#include "stdint.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*
 * These macros are used for calculating the required buffer sizes in the
 * configuration files (ccp2conf)
 */
#define CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( number_of_odts, elements_per_odt ) \
   ( ( (number_of_odts) * (elements_per_odt) ) + \
     ( ( ( (number_of_odts) * (elements_per_odt) ) + 7 ) / 8) )

#define CCP_DATA_BUFFER_SIZE_IN_BYTES( number_of_odts ) \
   ( (number_of_odts) * 7 )

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

typedef enum CCP_Resume_Supported_tag
{
   CCP_RESUME_NOT_SUPPORTED,
   CCP_RESUME_SUPPORTED

}  CCP_Resume_Supported_T;

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

typedef struct CCP_DAQ_Configuration_tag
{
   uint8_t                number_of_ODTs;
   uint8_t                number_of_elements_per_ODT;
   HUGE uint16_t*              config_buffer_ptr;
   HUGE uint8_t*               data_buffer_ptr;
   CCP_Resume_Supported_T resume_supported;

}  CCP_DAQ_Configuration_T;

typedef struct CCP_DAQ_Resume_Data_NVM_tag
{
   uint32_t message_id;                  /* from GET_DAQ_SIZE message */
   uint16_t transmission_rate_prescaler; /* from START_STOP message   */
   uint8_t  event_channel_number;        /* from START_STOP message   */
   uint8_t  first_PID;                   /* from GET_DAQ_SIZE message */
   uint8_t  last_ODT_to_transmit;        /* from START_STOP message   */
   uint8_t  started;                     /* flag DAQ has been started */
   uint8_t  dummy1;                      /* assure 32-bit alignment   */
   uint8_t  dummy2;                      /* assure 32-bit alignment   */

}  CCP_DAQ_Resume_Data_NVM_T;

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*
 * DAQ list definitions
 */
extern const CCP_DAQ_Configuration_T CCP_DAQ_Configuration[ NUMBER_OF_DAQ_LISTS ];

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Name:         Initialize_DAQ_Parameters
 * Purpose:      This function initializes all of the control parameters for
 *               the DAQ lists.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
void Initialize_DAQ_Parameters( void ); 

/*===========================================================================*\
 * Name:         CCP_Trigger_Event_Channel
 * Purpose:      Initiate the transmission of a DAQ list.
 * Parameters:   uint8_t, Event channel number
 * Return Value: none
\*===========================================================================*/
void CCP_Trigger_Event_Channel( uint8_t );

/*===========================================================================*\
 * Name:         Transmit_Next_DAQ_Packet
 * Purpose:      Transmit next available DAQ ODT.
 * Parameters:   none
 * Return Value: none
\*===========================================================================*/
void Transmit_Next_DAQ_Packet( void );

/*===========================================================================*\
 * Name:         CCP_Get_DAQ_Size
 * Purpose:      Returns the size of the specified DAQ list as the number of
 *               available ObjectDescriptorTables (ODTs) and clears the
 *               current list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Get_DAQ_Size( void );

/*===========================================================================*\
 * Name:         CCP_Set_DAQ_Ptr
 * Purpose:      Initializes the DAQ list pointer for a subsequent write to
 *               a DAQ list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Set_DAQ_Ptr( void );

/*===========================================================================*\
 * Name:         CCP_Write_DAQ
 * Purpose:      Writes one entry (description of single DAQ element) to a
 *               DAQ list defined by the DAQ list pointer (see SET_DAQ_PTR).
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Write_DAQ( void );

/*===========================================================================*\
 * Name:         Start_Stop
 * Purpose:      This command is used to start or to stop the data
 *               acquisition or to prepare a synchronized start of the
 *               specified DAQ list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Start_Stop( void );

/*===========================================================================*\
 * Name:         Start_Stop_All
 * Purpose:      This command is used to start the periodic transmission of
 *               all DAQ lists configured with the previously sent START_STOP
 *               command (start/stop mode = 2) as "prepared to start" in a
 *               synchronized manner.  The command is used to stop the
 *               periodic transmission of all DAQ lists, including those not
 *               started synchronized.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Start_Stop_All( void );

/*===========================================================================*\
 * Name:         CCP_Store_Resume_Data
 * Purpose:      This command will copy the resume data to the user supplied
 *               NVM buffer CCP_DAQ_Resume_Data_NVM.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
void CCP_Store_Resume_Data( void );

/*===========================================================================*\
 * Name:         CCP_Initialize_Resume_Data
 * Purpose:      This command will initialize the user supplied NVM buffer
 *               CCP_DAQ_Resume_Data_NVM.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
void CCP_Initialize_Resume_Data( void );

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

#endif /* CCP2DAQ_H */

