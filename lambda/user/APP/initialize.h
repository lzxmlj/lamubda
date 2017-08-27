#ifndef INITIALIZE_H
#define INITIALIZE_H

/*===========================================================================*/
/* FILE: initialize.h                                                        */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            initialize.h %                                          */
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
#include "reuse.h" /* include peripheral declarations */
#include "stdint.h"

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/


/*===========================================================================*\
 * Exported Type Defines
\*===========================================================================*/
typedef enum S12Z_Reset_Type{

    POWER_ON_RESET_MASK      = 0x01,
    LOW_VOLTAGE_RESET_MASK   = 0x02,
    COP_RESET_MASK           = 0x04,
    OSCILLATOR_CM_RESET_MASK = 0x08,
    PLL_CM_RESET_MASK        = 0x10
}S12Z_Reset_T;


/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/
extern void Initialize_Hardware(void);


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 *===========================================================================*/
#endif //INITIALIZE_H

