#ifndef REUSE_H
#define REUSE_H
#ifdef __QAC__
// Deviation for OSEK usage through include of standard osek.h header.
#pragma PRQA_MESSAGES_OFF 1011
#endif
//=============================================================================
//
//       COPYRIGHT, 2002, Delphi Technologies, Inc. All Rights reserved
//
//       Delphi Confidential
//
// ============================================================================
// %full_filespec:   reuse.h~1:incl:ctc_pt3#9 %
//
// created_by:       rskearne
//
// date_created:     Thu Mar 23 11:10:25 2006
//
// %derived_by:      zzrfyj %
//
// %date_modified:        %
//
// %version:         1 %
//
// ============================================================================
// @doc
//
// @module reuse.h | reuse.h
//
// The purpose of this module is to provide common definitions of types for all
// processor/compiler sets.
//
// <nl> Put a brief description here 
//   This module defines the standard reusable types and functions
//   common to all reusable software:
//
// @normal Copyright <cp> 2002,2006, Delphi Technologies, Inc. All Rights reserved
//
// SPECIFICATION REVISION:
//    IFS_ProjectCompiler.doc-004
//    INTERNATIONAL STANDARD ©ISO/IEC ISO/IEC 9899:1999 (E) Programming languages-C
//
// ============================================================================
// Configurable Development Software Module:
// DO NOT MODIFY THIS FILE. It contains no configurable parameters.
//=============================================================================

//=============================================================================
// Processor and Compiler types
//=============================================================================
// Processor Types

//=============================================================================
// /------------------------------------------------------------------------
// |                 Standard Preprocessor Definitions
// \------------------------------------------------------------------------
// ============================================================================
// StandardPreprocessor Definitions are placed here to allow specific
// compiler/processor choices to override the defaults.
//=============================================================================
#include "stdint.h"
#include "stm32f37x.h"
#define HUGE
#define NULL  ( (void*) 0 )
#define PARAM_NOT_USED(param) 

typedef uint8_t                     bitfield8_t;           /* bitfield, 8 bits */
typedef uint16_t                    bitfield16_t;          /* bitfield, 16 bits */
typedef uint32_t                    bitfield32_t;          /* bitfield, 32 bits */

typedef unsigned short              WORD;              /* 16 bit unsigned */

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

#ifndef SIG_ATOMIC_SIZE
#define SIG_ATOMIC_SIZE    (8*sizeof(sig_atomic_t))
#endif
#ifndef SIG_ATOMIC_MIN
#define SIG_ATOMIC_MIN     UINT8_MIN
#endif
#ifndef SIG_ATOMIC_MAX
#define SIG_ATOMIC_MAX     UINT8_MAX
#endif


//
// Max function.
//
#ifndef Max
#define Max(a,b) ( ( (a)>(b) )?(a):(b) ) 
#endif

//
// Min function.
//
#ifndef Min
#define Min(a,b) ( ( (a)<(b) )?(a):(b) ) 
#endif

#ifndef BIT_T
#define BIT_T
//
// @enum Bit_T | Defines the position of the bits
//
typedef enum
{
   BIT_0,   // @emem Bit Position  0
   BIT_1,   // @emem Bit Position  1
   BIT_2,   // @emem Bit Position  2
   BIT_3,   // @emem Bit Position  3
   BIT_4,   // @emem Bit Position  4
   BIT_5,   // @emem Bit Position  5
   BIT_6,   // @emem Bit Position  6
   BIT_7,   // @emem Bit Position  7
   BIT_8,   // @emem Bit Position  8
   BIT_9,   // @emem Bit Position  9
   BIT_10,  // @emem Bit Position 10
   BIT_11,  // @emem Bit Position 11
   BIT_12,  // @emem Bit Position 12
   BIT_13,  // @emem Bit Position 13
   BIT_14,  // @emem Bit Position 14
   BIT_15,  // @emem Bit Position 15
   BIT_16,  // @emem Bit Position 16
   BIT_17,  // @emem Bit Position 17
   BIT_18,  // @emem Bit Position 18
   BIT_19,  // @emem Bit Position 19
   BIT_20,  // @emem Bit Position 20
   BIT_21,  // @emem Bit Position 21
   BIT_22,  // @emem Bit Position 22
   BIT_23,  // @emem Bit Position 23
   BIT_24,  // @emem Bit Position 24
   BIT_25,  // @emem Bit Position 25
   BIT_26,  // @emem Bit Position 26
   BIT_27,  // @emem Bit Position 27
   BIT_28,  // @emem Bit Position 28
   BIT_29,  // @emem Bit Position 29
   BIT_30,  // @emem Bit Position 30
   BIT_31,  // @emem Bit Position 31
   BIT_MAX  // @emem 32 Bits

} bit_t;

#endif
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
#define Cast_bitfield32(x) ((bitfield32_t)(x))
#define Cast_bitfield16(x) ((bitfield16_t)(x))
#define Cast_bitfield8(x)  ( (bitfield8_t)(x))

#define Cast_uint8(x)          ( (uint8_t)(x))
#define Cast_uint16(x)         ((uint16_t)(x))
#define Cast_uint32(x)         ((uint32_t)(x))

#define Cast_int8(x)            ( (int8_t)(x))
#define Cast_int16(x)           ((int16_t)(x))
#define Cast_int32(x)           ((int32_t)(x))

#endif // REUSE_H
