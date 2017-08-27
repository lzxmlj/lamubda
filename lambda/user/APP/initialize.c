/*===========================================================================*/
/* FILE: initialize.c                                                        */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            initialize.c %                                          */
/*                                                                           */
/* created_by:       sjl                                                     */
/*                                                                           */
/* date_created:     20 Feb 2013                                             */
/*                                                                           */
/* %derived_by:      rz65p6 %                                                */
/*                                                                           */
/* %date_modified:   Wed Feb 20 11:46:46 2013 %                              */
/*                                                                           */
/* %version:         1 %                                                     */
/*                                                                           */
/*===========================================================================*/
/* DESCRIPTION:                                                              */
/*   initialize                                                              */
/*                                                                           */
/* ABBREVIATIONS:                                                            */
/*   None.                                                                   */
/*                                                                           */
/* TRACEABILITY INFO:                                                        */
/*   Design Document:                                                        */
/*                                                                           */
/*   Requirements Document(s):                                               */
/*                                                                           */
/*   Applicable Standards:                                                   */
/*   C Coding Standards:        SW_REF 264.15D.                              */
/*                                                                           */
/* DEVIATIONS FROM STANDARDS:                                                */
/*   None.                                                                   */
/*                                                                           */
/*===========================================================================*/

/*===========================================================================*\
 * Standard Header Files
\*===========================================================================*/
#include "initialize.h"   /* for  */
#include "dd_can.h"   /* for Hw_Initialize_CAN_RegPtr */
#include "stm32f37x.h"   /* for DO_Init_PTU */
#include "reuse.h"
/*===========================================================================*\
 * Local Definitions
struct _tagStartup _startupData;	/* read-only: */
					/* _startupData is allocated in ROM and */
					/* initialized by the linker */

/*===========================================================================*\
 * Global Object Definitions
\*===========================================================================*/


/*===========================================================================*\
 * Local Func Declarations
\*===========================================================================*/

/*===========================================================================*\
 * _Startup - required by CW
\*===========================================================================*/

/*****************************************************************************
*
* Function: 
*
* Description: 
*
*****************************************************************************/
/*===========================================================================*\

/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 *===========================================================================*/
