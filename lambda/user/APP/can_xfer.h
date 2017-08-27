#ifndef CAN_XFER_H
#define CAN_XFER_H

/*===========================================================================*/
/* FILE: can_xfer.h                                                            */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            can_xfer.h %                                              */
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
/*   CAN - MSG management                                                    */
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
/* REUSE: DO NOT MODIFY THIS FILE.                                           */
/* Co-ordinate any desired changes with the Software Forward Engineering and */
/* Building Blocks group                                                     */
/*===========================================================================*/


/*===========================================================================*\
 * Standard Header Files
\*===========================================================================*/
#include "reuse.h"
#include "io_can_config.h"

#include "stdint.h"

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/


/*===========================================================================*\
 * Exported Preprocessor #define MACROS
\*===========================================================================*/


/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/
extern bool Get_Message(      CAN_message_ID_T CAN_Msg_Id, uint8_t * message_address );
extern bool Transmit_Message( CAN_message_ID_T CAN_Msg_Id, uint8_t * message_address );
extern uint8_t  CAN1_Send_Msg(uint32_t can_message_id, uint8_t* msg);
extern void CAN_Reset_Init( void );


/*===========================================================================*\
 * Exported INLINE functions and #define function-like macros
\*===========================================================================*/


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 *===========================================================================*/
#endif /* CAN_XFER_H */
