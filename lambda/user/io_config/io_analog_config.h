#ifndef IO_ANALOG_CONFIG_H
#define IO_ANALOG_CONFIG_H
/******************************************************************************
 *
 * Filename:          io_analog_config.h
 *
 * Description:       header file for io_analog_config.h file.
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
 * %full_name:     mt20u2#2/incl/io_analog_config.h/8 %
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
 * Exported Macro Defines
\* ==========================================================================*/
#define ADC_REF_VOLT                         (   5.0 )
#define SENSE_RESISTOR_OHM                   ( 0.005 )  /* 2W -> 20A max */
#define SENSE_A_VALUE       (  22.1 )
#define SENSE_AMPLIFIER     ((uint16_t)( 1+((SENSE_A_VALUE*SENSE_A_VALUE)/(SENSE_A_VALUE+1))) )

#define FLTBATT_DIVEDER                        ( 5.0 )

/* Analog */
#define ADC_VOLT_SENS       ADC_PAD0
#define ADC_VOLT_VLS         ADC_PAD4
#define ADC_1_VOLT_VHD      ADC_1_GDU_DC_LINK_VOLT


#endif
/**********************************************************************
*
* Revision History:
*
* Rev.   YYMMDD Who RSM# Change 
* ------ ------ --- ---- ----------------------------------------------
*
**********************************************************************/
