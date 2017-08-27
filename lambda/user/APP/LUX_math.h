#ifndef LUX_MATH_H
#define LUX_MATH_H

/*===========================================================================*/
/* FILE: LUX_math.h                                                          */
/*===========================================================================*/
/* COPYRIGHT 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            LUX_math.h %                                            */
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
/*   LUX_math                                                                */
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
typedef unsigned long     LONGCARD;
#define MIN_LONGCARD          (0uL)     /*--- 000000000h ---*/
#define MAX_LONGCARD (4294967295uL)     /*--- 0FFFFFFFFh ---*/

typedef unsigned char    SHORTCARD;

typedef long signed int    LONGINT;

/*===========================================================================*\
 * Exported Macro
\*===========================================================================*/

/*--- NOT TO BE DEFINED SOMEWHERE! ---*/
SHORTCARD CrashFunction (void);

#define FactorAbs(Factor) (((Factor) >= 0.0) ? (Factor) : - (Factor))

#define RoundUp (0.5)

#define FactorRound(Factor) ( \
 ((Factor) >= 0.0) ? ((Factor) + RoundUp) : ((Factor) - RoundUp) \
 )

#define FactorCasted(Factor) ( \
 ((Factor) >= 0.0) ? \
 ((LONGCARD) FactorRound (Factor)) : ((LONGINT) FactorRound (Factor)) \
 )
 
#define FixLimitDiv(Var,Limit) (((Var) > (Limit)) ? (Limit) : (Var))

#define FixMaxDefConst(Type) \
 ((Type) FactorRound ((Max_##Type - Min_##Type) / Prec_##Type))

/*--- FixDefConst converts a physical value fixed point constant ---*/
/*--- to a binary value fixed point constant                     ---*/
/*--- WARNING: Make sure that the floating value is not outside  ---*/
/*---  the boundaries of the Fixed Point type.                   ---*/
/*---  The internal value result is not rounded to the Min or Max---*/
/*---  of the type in that case!                                 ---*/
#define FixDefConst(ConstFloat,TypeName) \
((TypeName) FactorRound (((ConstFloat) - Min_##TypeName) / Prec_##TypeName))


#define FixNonBiasedDiv(VarN,TypeN,VarD,TypeD,TypeQ) ((TypeQ) ( \
 (((Prec_##TypeN / Prec_##TypeD) / Prec_##TypeQ) >= 1.0) ? \
  ((FactorCasted ((Prec_##TypeN / Prec_##TypeD) / Prec_##TypeQ) * (VarN)) / \
     (VarD)) : \
  ((VarN) / (FactorCasted (Prec_##TypeQ / (Prec_##TypeN / Prec_##TypeD)) * \
     (VarD))) \
  ))

/*--- FixNonBiasedDivide gives the quotient of two fixed point variable, ---*/
/*--- the result is of a different fixed point type                      ---*/
#define FixNonBiasedDivide(VarN,TypeN,VarD,TypeD,TypeQ) ( (TypeQ) ( \
 (Min_##TypeN != 0.0 || Min_##TypeD != 0.0 || Min_##TypeQ != 0.0) ? \
   CrashFunction () : \
   FixLimitDiv (FixNonBiasedDiv(VarN,TypeN,VarD,TypeD,TypeQ), \
     FixMaxDefConst (TypeQ)) \
  ))


/*--- FixMultiply does the product of two fixed point variable,     ---*/
/*--- the result is of a different fixed point type.                ---*/
/*--- Remark: When used for asymptotic decaying, the 0 value is not ---*/
/*---         always reached, use FixDecayMult0To1 in that case.    ---*/
#define FixMultiply(VarOrgA,TypeOrgA,VarOrgB,TypeOrgB,TypeDest) \
((TypeDest) \
(((FactorAbs ((Prec_##TypeOrgA * Prec_##TypeOrgB) / Prec_##TypeDest) < 1.0) ? \
   (((LONGCARD) (VarOrgA) * (VarOrgB)) / \
   FactorCasted (Prec_##TypeDest / (Prec_##TypeOrgA * Prec_##TypeOrgB))) : \
   (((LONGCARD) (VarOrgA) * (VarOrgB)) * \
   FactorCasted ((Prec_##TypeOrgA * Prec_##TypeOrgB) / Prec_##TypeDest))) \
 + \
 ((Min_##TypeOrgB > 0.0) ? \
   ((FactorAbs ((Prec_##TypeOrgA * Min_##TypeOrgB) / Prec_##TypeDest) < 1.0) ? \
     ((LONGCARD) (VarOrgA) / \
    FactorCasted (Prec_##TypeDest / (Prec_##TypeOrgA * Min_##TypeOrgB))) : \
    ((LONGCARD) (VarOrgA) * \
    FactorCasted ((Prec_##TypeOrgA * Min_##TypeOrgB) / Prec_##TypeDest)) \
   ) : \
  ((Min_##TypeOrgB < 0.0) ? \
   ((FactorAbs ((Prec_##TypeOrgA * (Min_##TypeOrgB)) / Prec_##TypeDest) < 1.0) ? \
     - ((LONGCARD) (VarOrgA) / \
    FactorCasted (Prec_##TypeDest / (Prec_##TypeOrgA * (-Min_##TypeOrgB)))) : \
    - ((LONGCARD) (VarOrgA) * \
    FactorCasted ((Prec_##TypeOrgA * (-Min_##TypeOrgB)) / Prec_##TypeDest)) \
   ) : \
   ((LONGCARD) (0)))) \
 + \
 ((Min_##TypeOrgA > 0.0) ? \
   ((FactorAbs ((Min_##TypeOrgA * Prec_##TypeOrgB) / Prec_##TypeDest) < 1.0) ? \
     ((LONGCARD) (VarOrgB) / \
    FactorCasted (Prec_##TypeDest / (Min_##TypeOrgA * Prec_##TypeOrgB))) : \
    ((LONGCARD) (VarOrgB) * \
    FactorCasted ((Min_##TypeOrgA * Prec_##TypeOrgB) / Prec_##TypeDest)) \
   ) : \
  ((Min_##TypeOrgA < 0.0) ? \
   ((FactorAbs ((Min_##TypeOrgA * Prec_##TypeOrgB) / Prec_##TypeDest) < 1.0) ? \
     - ((LONGCARD) (VarOrgB) / \
    FactorCasted (Prec_##TypeDest / ((-Min_##TypeOrgA) * Prec_##TypeOrgB))) : \
    - ((LONGCARD) (VarOrgB) * \
    FactorCasted (((-Min_##TypeOrgA) * Prec_##TypeOrgB) / Prec_##TypeDest)) \
   ) : \
   ((LONGCARD) (0)))) \
 + \
 FactorCasted ((Min_##TypeOrgA * Min_##TypeOrgB - Min_##TypeDest) / Prec_##TypeDest) \
))

/*--- FixConvert converts a fixed point variable to ---*/
/*--- another fixed point type.                     ---*/
#define FixConvert(VarOrg,TypeOrg,TypeDest) \
((TypeDest) \
(((VarOrg) * FactorCasted (Prec_##TypeOrg * RFactor_##TypeOrg##_##TypeDest) + \
  FactorCasted ((Min_##TypeOrg - Min_##TypeDest) * RFactor_##TypeOrg##_##TypeDest)) / \
  FactorCasted (Prec_##TypeDest * RFactor_##TypeOrg##_##TypeDest)))

/*--- returns the absolute difference                 ---*/
#define AbsSubtract(VarA,VarB, Type) \
((Type)( (VarA>VarB) ? (VarA-VarB) : (VarB-VarA) ))

/*===========================================================================
 * File revision history (top to bottom, first revision to last revision
 *===========================================================================
 *                                                                   
 * Rev.  YYMMDD  Who    RSM#   Changes
 * ----  ------  -----  ----   -------
 * 1.0   130220  sjl    n/a    Created the initial version
 *===========================================================================*/
#endif //LUX_MATH_H

