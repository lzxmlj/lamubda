#ifndef CCP2NVMD_H
#define CCP2NVMD_H
/*===========================================================================*\
 *
 *       COPYRIGHT, 2002, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 *
 * MODULE NAME:
 * ccp2nvmd.h
 *
 * SPECIFICATION REVISION:
 * ASAP Standard Can Calibration Protocol Version 2.1, 18, February 1999
 *
 *===========================================================================
 * REUSE:
 * This module is a TEMPLATE and should be modified to suit the application
 * (contact Jeffrey W. Davis, 765-451-0478).
\*===========================================================================*/

/*===========================================================================*\
 * Include Files
\*===========================================================================*/

#include "ccp2conf.h"
#include "ccp2daq.h"

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/
#define DAQ_ELEMENTS_PER_ODT  (0x07)

#define DAQ0_NUMBER_OF_ODTS   (0x02)
#define DAQ0_ELEMENTS_PER_ODT (DAQ_ELEMENTS_PER_ODT)

#define DAQ1_NUMBER_OF_ODTS   (0x02)
#define DAQ1_ELEMENTS_PER_ODT (DAQ_ELEMENTS_PER_ODT)

#define DAQ2_NUMBER_OF_ODTS   (0x04)
#define DAQ2_ELEMENTS_PER_ODT (DAQ_ELEMENTS_PER_ODT)

#define DAQ3_NUMBER_OF_ODTS   (0x04)
#define DAQ3_ELEMENTS_PER_ODT (DAQ_ELEMENTS_PER_ODT)

extern uint16_t daq0_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ0_NUMBER_OF_ODTS,
                                                                              DAQ0_ELEMENTS_PER_ODT ) ];
extern uint16_t daq1_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ1_NUMBER_OF_ODTS,
                                                                              DAQ1_ELEMENTS_PER_ODT ) ];
extern uint16_t daq2_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ2_NUMBER_OF_ODTS,
                                                                              DAQ2_ELEMENTS_PER_ODT ) ];
extern uint16_t daq3_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ3_NUMBER_OF_ODTS,
                                                                              DAQ3_ELEMENTS_PER_ODT ) ];

extern CCP_DAQ_Resume_Data_NVM_T CCP_DAQ_Resume_Data_NVM[ NUMBER_OF_RESUMEABLE_DAQ_LISTS ];

extern uint8_t ccp_session_configuration_id_NVM[ 4 ];

extern uint8_t ccp_session_status_NVM;

#endif //CCP2NVMD_H
