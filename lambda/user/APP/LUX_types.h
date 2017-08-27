#ifndef LUX_TYPES_H
#define LUX_TYPES_H

/*===========================================================================*/
/* FILE: LUX_types.h                                                         */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            LUX_types.h %                                           */
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
/*   LUX_types                                                               */
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
#include "stdint.h"


/*===========================================================================*\
 * Type Defines
\*===========================================================================*/
typedef uint16_t               T_HERTZ ;
#define Prec_T_HERTZ                            ( 1/2.0 )
#define Min_T_HERTZ                               ( 0.0 )
#define Max_T_HERTZ             ( 0xFFFF * Prec_T_HERTZ )

typedef uint32_t               T_HERTZ_L ;
#define Prec_T_HERTZ_L                          ( 1/2.0 )
#define Min_T_HERTZ_L                             ( 0.0 )
#define Max_T_HERTZ_L     ( 0xFFFFFFFF * Prec_T_HERTZ_L )

typedef uint16_t        Multiplier_0_to_1_W;
#define Prec_Multiplier_0_to_1_W              (1/65536.0)
#define Min_Multiplier_0_to_1_W                     (0.0)
#define Max_Multiplier_0_to_1_W   (65535.0 * Prec_Multiplier_0_to_1_W)

typedef uint16_t              T_PERCENT_MULTa;
#define Prec_T_PERCENT_MULTa                ( 0.000030517578125 / 2)
#define Min_T_PERCENT_MULTa                               ( 0.0 )
#define Max_T_PERCENT_MULTa     ( 0xFFFF * Prec_T_PERCENT_MULTa )

#define Prec_uint16_t                              ( 1.0 )
#define Min_uint16_t                               ( 0.0 )
#define Max_uint16_t            ( 0xFFFF * Prec_uint16_t )

#define Prec_uint32_t                              ( 1.0 )
#define Min_uint32_t                               ( 0.0 )
#define Max_uint32_t        ( 0xFFFFFFFF * Prec_uint32_t )

typedef uint16_t               T_RPM ;
#define V_RPM(val)   (WORD)(val*4)
#define Prec_T_RPM                                    ( 1/4.0 )
#define Min_T_RPM                                       ( 0.0 )
#define Max_T_RPM                       ( 0xFFFF * Prec_T_RPM )

typedef uint16_t               T_TIME_TCNT ;
#define Prec_T_TIME_TCNT                                  ( 1 )
#define Min_T_TIME_TCNT                                 ( 0.0 )
#define Max_T_TIME_TCNT           ( 0xFFFF * Prec_T_TIME_TCNT )

typedef uint16_t               T_TIME_US ;
#define Prec_T_TIME_US                                  ( 1.0 )
#define Min_T_TIME_US                                   ( 0.0 )
#define Max_T_TIME_US               ( 0xFFFF * Prec_T_TIME_US )

typedef uint16_t               T_STEP_COU;
#define V_COUNT_WORD(val)                        (V_UW_00(val))
#define Min_T_STEP_COU                       ( (T_STEP_COU)0x00 )
#define Max_T_STEP_COU               	( (T_STEP_COU)0xFF)
#define V_UW_00(val)                             ((T_STEP_COU)((val)*1))
typedef uint16_t               T_Delay_COU;
#define V_COUNT_WORD(val)                        (V_UW_00(val))
#define Min_T_Delay_COU                       ( (T_STEP_COU)0x00 )
#define Max_T_Delay_COU               	( (T_STEP_COU)0xFF)
#define V_UW_00(val)                             ((T_STEP_COU)((val)*1))

typedef uint16_t               T_Counter_Time;
#define V_COUNT_TIME(val)                        (V_UW_08(val))
#define Min_T_COUNT_TIME                       ( (T_Counter_Time)0x0000 )
#define Max_T_COUNT_TIME               	( (T_Counter_Time)0xFFFF)
#define V_UW_08(val)                             ((T_Counter_Time)((val ) * 125))

typedef uint32_t               T_TIME_US_L ;
#define Prec_T_TIME_US_L                                ( 1.0 )
#define Min_T_TIME_US_L                                 ( 0.0 )
#define Max_T_TIME_US_L           ( 0xFFFF * Prec_T_TIME_US_L )

#define RFactor_T_TIME_TCNT_T_TIME_US     (1000000UL/1000000UL)

/* electronic potential (volts), range: [0, 31.999] */
typedef uint16_t             T_VOLTb;
#define V_VOLTb(val)                    (uint16_t)((val)*2048)    // for value in volts
#define Min_T_VOLTb                               ( 1/2048.0 )
#define Max_T_VOLTb                   ( 0xFFFF * Min_T_VOLTb )

typedef uint16_t               T_CSA_ANGLEa;
#define Prec_T_CSA_ANGLEa                         ( 6 / 10000.0 )
#define Min_T_CSA_ANGLEa                                  ( 0.0 )
#define Max_T_CSA_ANGLEa           ( 0xFFFF * Prec_T_CSA_ANGLEa )

typedef uint16_t               T_MILLI_AMPARE;
#define Prec_T_MILLI_AMPARE                                 1.0 )
#define Min_T_MILLI_AMPARE                                ( 0.0 )
#define Max_T_MILLI_AMPARE       ( 0xFFFF * Prec_T_MILLI_AMPARE )

typedef uint16_t               T_MILLI_VOLT;
#define Prec_T_MILLI_VOLT                                 ( 1.0 )
#define Min_T_MILLI_VOLT                                  ( 0.0 )
#define Max_T_MILLI_VOLT           ( 0xFFFF * Prec_T_MILLI_VOLT )

typedef struct
{ uint16_t    steps;
  uint16_t    lower;
  uint16_t    upper;
} LINEAR_UW;

typedef struct {
   WORD x_length;
   WORD y_length;
   WORD data[6][12];
} MAP6x12_UW;

typedef struct {
         WORD length;
         WORD data[12];
 } MAP12_UW;

typedef struct {
         WORD length;
         WORD data[6];
 } MAP6_UW;

typedef struct {
         WORD length;
         WORD data[];
 } MAPxx_UW;


/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 * 1.1   130506  sj	   n/a    Add a new unit for the new step
 *===========================================================================*/
#endif //LUX_TYPES_H

