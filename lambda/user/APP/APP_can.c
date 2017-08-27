/*===========================================================================*/
/* FILE: APP_can.c                                                          */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            APP_can.c %                                            */
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
/*   APP_can                                                                */
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
#include "can_xfer.h"
#include "LUX_types.h"
#include "APP_can.h"

/*===========================================================================*\
 * Global Object Definitions
\*===========================================================================*/

// Reception Signals
T_CSA_ANGLEa    VfCOMM_ECU_InitializationCSA; /* HALL initialization */
T_CSA_ANGLEa    VfCOMM_ECU_DesiredSA;         /* VLC acts as CSA follower */
bool            VbCOMM_ECU_StopLiftControl;   /* for motor shut down */
bool            VbCOMM_ECU_ToMinMechStop;     /* for VLS zero CSA learning  */
bool            VbCOMM_ECU_ToMaxMechStop;     /* for VLS max CSA learning */
bool            VbCOMM_ECU_VLS_MinLearn_Completed;
bool            VbCOMM_ECU_VLS_MaxLearn_Completed;

// Transmission Signals
T_CSA_ANGLEa    VfCOMM_VLC_CalcedCSA;
bool            VbCOMM_VLC_HALL_Init_Completed;
bool            VbCOMM_VLC_HD_Volt_TooHigh;
bool            VbCOMM_VLC_HD_Volt_TooLow;
bool            VbCOMM_VLC_PowerSupply_Volt_TooLow;
bool            VbCOMM_VLC_PowerSupply_Volt_TooHigh;

// misc
TEST_ECU_ID100_T      VsTEST_ID100_ECU_Command;
TEST_VLC_ID101_T      VsTEST_ID101_VLC_Data;
ECU_TO_CVVL_T		VsCAN_ECU_TO_CVVL_Command;
CVVL_TO_ECM_T		VsCAN_CVVL_TO_ECU_Command;
VLC_OPERATING_MODE_T  VeVLC_OperatingMode;
bool                  VbCAN_Message_Valid[CAN_NUMBER_OF_RECEIVE_MESSAGES];

uint8_t         VcCAN_Message_StaleCount[CAN_NUMBER_OF_RECEIVE_MESSAGES];

/*===========================================================================*\
 * Local Object Definitions
\*===========================================================================*/
static const uint8_t CcCAN_Message_TimeOut_Threshold[CAN_NUMBER_OF_RECEIVE_MESSAGES]=
                                                                         {10};
uint32_t SCIMessage;
extern void PutChar(char ch);

/*===========================================================================*\
 * Local Function Definitions
\*===========================================================================*/
static void APP_PeriodicTask_CANDiag(bool msg_updated, CAN_Recv_Message_List_T index);



/*****************************************************************************
*
* Function: void APP_PeriodicTask_CANDiag(void)
*
* Description: CAN comm diag
*
*****************************************************************************/
static void APP_PeriodicTask_CANDiag(bool msg_updated, CAN_Recv_Message_List_T index)
{

   if ( VcCAN_Message_StaleCount[index] < 0xFF )
   {
      VcCAN_Message_StaleCount[index]++;
   }
   
   if ( VcCAN_Message_StaleCount[index] >= CcCAN_Message_TimeOut_Threshold[index] )
   {
      VbCAN_Message_Valid[index] = false;
   }
   else
   {
      VbCAN_Message_Valid[index] = true;
   }

}


/*****************************************************************************
*
* Function: void APP_CAN_Init(void)
*
* Description: CAN comm diag
*
*****************************************************************************/
void APP_CAN_Init(void)
{
uint8_t index;

    for (index=0; index<CAN_NUMBER_OF_RECEIVE_MESSAGES; index++)
    {
        VbCAN_Message_Valid[index] = false;
        VcCAN_Message_StaleCount[index] = CcCAN_Message_TimeOut_Threshold[index];
    }
}


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 * 1.1   130506  sj	   n/a    Add the 2 can MSG.CAN_ECU_TO_CVVL, CAN_CVVL_TO_ECU
 *===========================================================================*/
