#ifndef IO_CONFIG_H
#define IO_CONFIG_H
/******************************************************************************
 *
 * Filename:          io_config.h
 *
 * Description:       header file for io_config.h file.
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
 * %full_name:     mt20u2#2/incl/io_config.h/8 %
 * %date_created:  Wed Jan 11 15:08:35 2012 %
 * %version:       8 %
 *
 *****************************************************************************/

/******************************************************************************
*  System Include Files
******************************************************************************/
#include "io_can_config.h"
#include "io_analog_config.h"


/* ==========================================================================*\
 * Exported Macro Defines
\* ==========================================================================*/
#define INSTRUMENTATION_ACTIVE

#define INSTRUMENTATION_CAL_RAM_STARTADDR    ( 0x001000 )
#define CAL_STARTADDR                        ( 0xFE0000 )
#define CAL_SIZE_BYTES                          ( 0x400 )

#define KERNEL_RAM_ADDR                        ( 0x1000 )
#define KERNEL_SIZE                             ( 0x800 )

#define APP_STARTADDR                        ( 0xFE0000 )
#define APP_ENDADDR                          ( 0xFFEFFF )

#define BOOT_STARTADDR                       ( 0xFFF000 )
#define BOOT_ENDADDR                         ( 0xFFFFFF )

#define Get_Reprogram_Conditions_Met()           ( true )
#define PROF_ID                                  ( 0xFB )

// support up to TCNT range
#endif
/**********************************************************************
*
* Revision History:
*
* Rev.   YYMMDD Who RSM# Change 
* ------ ------ --- ---- ----------------------------------------------
*
**********************************************************************/
