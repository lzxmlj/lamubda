#ifndef REUSE_H
#define REUSE_H

/*===========================================================================*/
/* FILE: reuse.h                                                             */
/*===========================================================================*/
/* COPYRIGHT, 2013, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:             reuse.h %                                              */
/*                                                                           */
/* created_by:                                                               */
/*                                                                           */
/* date_created:     20 Feb 2013                                             */
/*                                                                           */
/* %derived_by:       rz65p6 %                                               */
/*                                                                           */
/* %date_modified:   Wen Feb 20 12:30:56 2013 %                                                       */
/*                                                                           */
/* %version:          1 %                                                    */
/*                                                                           */
/*===========================================================================*/
/* DESCRIPTION:                                                              */
/*   This file contains macros to make up for functionality which is not     */
/*   present in ANSI-C and has different implementation in various compilers.*/
/*   It is used to make the code more or less ANSI-C compliant. It also maps */
/*   types to ensure we comply to the standards.                             */
/*                                                                           */
/* ABBREVIATIONS:                                                            */
/* None.                                                                     */
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
/* None.                                                                     */
/*                                                                           */
/*===========================================================================*/
/* REUSE: DO NOT MODIFY THIS FILE.                                           */
/* Co-ordinate any desired changes with the Software Forward Engineering and */
/* Building Blocks group                                                     */
/*===========================================================================*/


/*===========================================================================*/
/* Local preprocessor #define constants and macros                           */
/*===========================================================================*/


/*-------------------------------------------------------------------*/
/* Bit definitions                                                   */
/*-------------------------------------------------------------------*/
#define BIT00     0x01
#define BIT01     0x02
#define BIT02     0x04
#define BIT03     0x08
#define BIT04     0x10
#define BIT05     0x20
#define BIT06     0x40
#define BIT07     0x80

#define BIT08    (1UL<<8)
#define BIT09    (1UL<<9)
#define BIT010   (1UL<<10)
#define BIT011   (1UL<<11)
#define BIT012   (1UL<<12)
#define BIT013   (1UL<<13)
#define BIT014   (1UL<<14)
#define BIT015   (1UL<<15)

#define BIT016   (1UL<<16)
#define BIT017   (1UL<<17)
#define BIT018   (1UL<<18)
#define BIT019   (1UL<<19)
#define BIT020   (1UL<<20)
#define BIT021   (1UL<<21)
#define BIT022   (1UL<<22)
#define BIT023   (1UL<<23)

/*=========================================================================*/
/* Local type declarations                                                 */
/*=========================================================================*/


/* --------------------------------------------------------
 *  definition of exact width integer types for Cosmic C
 * --------------------------------------------------------*/

typedef unsigned char               uint8_t;               /* 8 bit unsigned */
//typedef char                        int8_t;                /* 8 bit signed */
typedef unsigned short              uint16_t;              /* 16 bit unsigned */
//typedef int                         int16_t;               /* 16 bit signed */
//typedef unsigned long               uint32_t;              /* 32 bit unsigned */
//typedef long                        int32_t;               /* 32 bit signed */

typedef uint8_t                     bitfield8_t;           /* bitfield, 8 bits */
typedef uint16_t                    bitfield16_t;          /* bitfield, 16 bits */
//typedef uint32_t                    bitfield32_t;          /* bitfield, 32 bits */

typedef unsigned short              WORD;              /* 16 bit unsigned */
/* --------------------------------------------------------
 *  Standard type definitions
 * --------------------------------------------------------*/

typedef enum bool_tag
{
    false,
    true
} bool;

typedef enum 
{
   CbFALSE = 0x00,
   CbTRUE  = 0x01
}  TbBOOL;

#define TRUE     (true)
#define FALSE   (false)

#define uint8_T uint8_t
#define uint16_T uint16_t
#define uint32_T uint32_t

#define sint8_T int8_t
#define sint16_T int16_t
#define sint32_T int32_t

#define bitfield8_T bitfield8_t
#define bitfield16_T bitfield16_t
#define bitfield32_T bitfield32_t

#define unsigned8_T uint8_t
#define unsigned16_T uint16_t
#define unsigned32_T uint32_t

#define signed8_T int8_t
#define signed16_T int16_t
#define signed32_T int32_t
#define boolean_T bool

#define T_AXIS uint16_t

//=============================================================================
// /------------------------------------------------------------------------
// |                 Standard Preprocessor Definitions
// \------------------------------------------------------------------------
// ============================================================================
// StandardPreprocessor Definitions are placed here to allow specific
// compiler/processor choices to override the defaults.
//=============================================================================

//per 7.18 of the ANSI/ISO Standard
#ifndef UINT8_MIN
#define UINT8_MIN      (0)             // Minimum value for uint8_t
#endif
#ifndef UINT8_MAX
#define UINT8_MAX      (0xffU)         // Maximum value for uint8_t
#endif

#ifndef UINT16_MIN
#define UINT16_MIN     (0)             // Minimum value for uint16_t
#endif
#ifndef UINT16_MAX
#define UINT16_MAX     (0xffffU)       // Maximum value for uint16_t
#endif

#ifndef UINT24_MIN
#define UINT24_MIN     (0)             // Minimum value for uint32_t containing 24 bits
#endif
#ifndef UINT24_MAX
#define UINT24_MAX     (0x00ffffffU)   // Maximum value for uint32_t containing 24 bits
#endif

#ifndef UINT32_MIN
#define UINT32_MIN     (0)             // Minimum value for uint32_t
#endif
#ifndef UINT32_MAX
#define UINT32_MAX     (0xffffffffU)   // Maximum value for uint32_t
#endif

#ifndef INT8_MIN
#define INT8_MIN      (-INT8_MAX-1)    // Minimum value for int8_t
#endif
#ifndef INT8_MAX
#define INT8_MAX      (127)            // Maximum value for int8_t
#endif

#ifndef INT16_MIN
#define INT16_MIN     (-INT16_MAX-1)   // Minimum value for int16_t
#endif
#ifndef INT16_MAX
#define INT16_MAX     (32767)          // Maximum value for int16_t
#endif

#ifndef INT24_MIN
#define INT24_MIN     (-INT24_MAX-1)   // Minimum value for int32_t containing 24 bits
#endif
#ifndef INT24_MAX
#define INT24_MAX     (8388607)        // Maximum value for int32_t containing 24 bits
#endif

#ifndef INT32_MIN
#define INT32_MIN     (-INT32_MAX-1)   // Minimum value for int32_t
#endif
#ifndef INT32_MAX
#define INT32_MAX     (2147483647)     // Maximum value for int32_t
#endif

#define BYTE_SHIFT      ( 8 )
#define HALF_BYTE_SHIFT ( 4 )

/* -------------------------------------------------------- *\
 * For HCS12
\* -------------------------------------------------------- */
typedef enum interrupt_state_tag
{
   interrupts_enabled  = 0x00,
   interrupts_disabled = 0x10
} interrupt_state_T;

// to be fully verified before being used !!!!!!!!!!!!!!!!
//#define Get_Interrupt_State(irq_state)   {__asm (TFR CCL,D5); __asm (AND D5,#16);  __asm (ST D5, irq_state );}
//#define Set_Interrupt_State(irq_state)   {__asm (TFR CCL,D5); __asm (AND D5,#239); __asm (OR D5, irq_state ); __asm (TFR D5, CCL );}

#define EnableInterrupts()   {__asm (CLI);}
#define DisableInterrupts()  {__asm (SEI);}


#define NULL  ( (void*) 0 )
#define Max(a,b) (((a)>(b))?(a):(b))

#define HUGE
#define PARAM_NOT_USED(param) 


/* ============================================================================ *\
 * File revision history (top to bottom, first revision to last revision
 * ============================================================================
 *
 * Date         user id     SCR       (description on following lines)
 * ----------   -------     ---
 * 20/02/13     sjl         -
 * + Baseline - Created the baseline.
 *
\* ============================================================================ */

#endif /* REUSE_H */

