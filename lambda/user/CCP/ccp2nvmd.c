/*===========================================================================*\
 *
 *       COPYRIGHT, 2002, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 *
 * MODULE NAME:
 * ccp2nvmd.c
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

#include "ccp2nvmd.h"

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

uint16_t daq0_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ0_NUMBER_OF_ODTS,
                                                                       DAQ0_ELEMENTS_PER_ODT ) ];
uint16_t daq1_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ1_NUMBER_OF_ODTS,
                                                                       DAQ1_ELEMENTS_PER_ODT ) ];
uint16_t daq2_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ2_NUMBER_OF_ODTS,
                                                                       DAQ2_ELEMENTS_PER_ODT ) ];
uint16_t daq3_config_buffer_NVM[ CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS( DAQ3_NUMBER_OF_ODTS,
                                                                       DAQ3_ELEMENTS_PER_ODT ) ];

CCP_DAQ_Resume_Data_NVM_T CCP_DAQ_Resume_Data_NVM[ NUMBER_OF_RESUMEABLE_DAQ_LISTS ];
