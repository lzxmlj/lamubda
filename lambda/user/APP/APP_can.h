#ifndef APP_CAN_H
#define APP_CAN_H

/*===========================================================================*/
/* FILE: APP_can.h                                                          */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            APP_can.h %                                            */
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
#include "reuse.h"
#include "LUX_types.h"
#include "io_can_config.h"
#include "stdint.h"

/*===========================================================================*\
 * Global Types Definitions
\*===========================================================================*/
typedef enum {
  CeTEST_DisArm                     = 0,
  CeTEST_FixedDC_Continous          = 1,
  CeTEST_FixedDC_Stall              = 2,
  CeTEST_FixedDC_NoHALL_Continuous  = 3,
  CeTEST_FixedDC_NoHALL_Stop        = 4,
  CeTEST_FixedDC_Acc_Dec         = 0x10,
  CeTEST_FixedDC_TargetPosition  = 0x11,
  CeTEST_FixedDC_SingleStep      = 0x12,
  CeTEST_SingleStep_For_Minilearn = 0x15,
  CeTEST_VLC_Info                = 0xFF
} TEST_CMD_T;

typedef enum {
   CeMax_Learning		=1,
   CeMin_Learning		=2,
   CeIgn_On				=0x40,
   Ceenable_control		=0x80
} Lift_Control_T;
typedef enum {
   CeMax_Learning_ar		=0,
   CeMin_Learning_ar		=1,
   CeVLC_state	=0xF0
} CVVL_State_T;
typedef union {
  struct {      
     TEST_CMD_T             Test_Command;    /*  Byte 0*/
     uint8_t                PWM_DC;
     uint8_t                MOTOR_Dir;
     uint8_t                MOTOR_Steps;     
     uint8_t                COMMU_Period;     
     uint8_t                HALL_State;     
     uint8_t                reserved_1 ;     
     uint8_t                reserved_2 ;     /* Byte 7 */
  } bf;
  uint8_t byte[8];     
}TEST_ECU_ID100_T;
typedef union {
  struct {      
     uint8_t                Desired_Lift_Angle_Hi;
     uint8_t                Desired_Lift_Angle_Lo;
     uint8_t	   	   Lift_Control;
     uint8_t                Actual_Lift_Angle_Hi;     
     uint8_t                Actual_Lift_Angle_Lo;     
     uint8_t                Coolant_Temperature;     
     uint8_t                Engine_Speed_Lo;     
     uint8_t                Engine_Speed_Hi;     /* Byte 7 */
  } bf;
  uint8_t byte[8];     
}ECU_TO_CVVL_T;

typedef union {
  struct {      
     uint16_t               MOTOR_Rpm ;     
     uint16_t               COMMU_Freq;     
     uint8_t                CURR_I;     
     uint8_t                HALL_State_U_V_W;     
     uint8_t                Transient_Steps;     
     uint8_t                GAP_Steps;
  } bf;
  uint8_t byte[8];     
}TEST_VLC_ID101_T;
typedef union {
  struct {      
     uint8_t               MOTOR_Steps_Hi;     
     uint8_t               MOTOR_Steps_Lo;
     CVVL_State_T	  CVVL_State;
     uint8_t                reserved;     
     uint8_t                reserved_1;     
     uint8_t                reserved_2;
     uint8_t		   MOTOR_Duty_Cycle;
     uint8_t                reserved_3;
  } bf;
  uint8_t byte[8];     
}CVVL_TO_ECM_T;

typedef enum {
  CeNORMAL_MODE,
  CeTEST_MODE
} VLC_OPERATING_MODE_T;

/*===========================================================================*\
 * Global Object Definitions
\*===========================================================================*/

// Reception Signals
extern T_CSA_ANGLEa    VfCOMM_ECU_InitializationCSA; /* HALL initialization */
extern T_CSA_ANGLEa    VfCOMM_ECU_DesiredSA;         /* VLC acts as CSA follower */
extern bool            VbCOMM_ECU_StopLiftControl;   /* for motor shut down */
extern bool            VbCOMM_ECU_ToMinMechStop;     /* for VLS zero CSA learning  */
extern bool            VbCOMM_ECU_ToMaxMechStop;     /* for VLS max CSA learning */
extern bool            VbCOMM_ECU_VLS_MinLearn_Completed;
extern bool            VbCOMM_ECU_VLS_MaxLearn_Completed;

// Transmission Signals
extern T_CSA_ANGLEa    VfCOMM_VLC_CalcedCSA;
extern bool            VbCOMM_VLC_HALL_Init_Completed;
extern bool            VbCOMM_VLC_HD_Volt_TooHigh;
extern bool            VbCOMM_VLC_HD_Volt_TooLow;
extern bool            VbCOMM_VLC_PowerSupply_Volt_TooLow;
extern bool            VbCOMM_VLC_PowerSupply_Volt_TooHigh;

// misc
extern TEST_ECU_ID100_T      VsTEST_ID100_ECU_Command;
extern TEST_VLC_ID101_T      VsTEST_ID101_VLC_Data;
extern VLC_OPERATING_MODE_T  VeVLC_OperatingMode;
extern bool                  VbCAN_Message_Valid[CAN_NUMBER_OF_RECEIVE_MESSAGES];
extern uint8_t               VcCAN_Message_StaleCount[CAN_NUMBER_OF_RECEIVE_MESSAGES];
extern ECU_TO_CVVL_T		VsCAN_ECU_TO_CVVL_Command;
extern CVVL_TO_ECM_T		VsCAN_CVVL_TO_ECU_Command;
/*===========================================================================*\
 * Exported Macro
\*===========================================================================*/
#define TEST_DC_PRECISION (0.01)

/*===========================================================================*\
 * Exported Function Prototypes
\*===========================================================================*/
extern void APP_CAN_Init(void);


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 * 1.1   130506  sj	   n/a    Add the two CAN MSG States.CVVL_TO_ECM_T,ECU_TO_CVVL_T,Lift_Control_T,CVVL_State_T
 *===========================================================================*/
#endif //APP_CAN_H

