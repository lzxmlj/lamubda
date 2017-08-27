#ifndef CCP2MEM_H
#define CCP2MEM_H
/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 * This module contains support for the following CCP 2.1 commands:
 *
 *    SET_MTA
 *    DNLOAD
 *    DNLOAD_6
 *    UPLOAD
 *    SHORT_UP
 *    BUILD_CHKSUM
 *    CLEAR_MEMORY
 *    PROGRAM
 *    PROGRAM_6
 *    MOVE
 *
 * MODULE NAME:
 * ccp2mem.h
 *
 * SPECIFICATION REVISION:
 * ASAP Standard Can Calibration Protocol Version 2.1, 18, February 1999
 *
 *===========================================================================
 * REUSE:
 * DO NOT MODIFY THIS FILE.  Coordinate any desired changes with the re-use
 * group (contact Jeffrey W. Davis, 765-451-0478).
 *
\*===========================================================================*/

/*===========================================================================*\
 * Include Files
\*===========================================================================*/

#include "ccp2main.h"
#include "ccp2conf.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

#ifdef CPU_IS_LITTLE_ENDIAN
#error
#endif

   /* No conversion necessary */
   #define FIX_BIG_ENDIAN32( a ) ( a )
   #define FIX_BIG_ENDIAN31( a ) ( (((a) & 0x000000FFUL) << 24) | \
                                   (((a) & 0x0000FF00UL) <<  8) | \
                                   (((a) & 0x00FF0000UL) >>  8) | \
                                   (((a) & 0xFF000000UL) >> 24) )

/* No conversion necessary */
   #define FIX_BIG_ENDIAN16( a ) ( a )
   /* Convert 16-bit value stored bytewise in little endian to big endian */
   #define FIX_LITTLE_ENDIAN16( a ) ( (uint16_t)(((uint16_t)((a) & 0x00FF) << 8) | \
                                                 ((uint16_t)((a) & 0xFF00) >> 8)) )

/*
 * This macro is used to read bytes [5..2] of the CRO which are a
 * 32-bit longword, but not aligned on a longword boundary in
 * memory.
 */
#define READ_UNALIGNED_MEMORY_SIZE() \
   ( ( (uint32_t)ccp.command_data.memory_size.number_of_bytes32_uub << 24 ) | \
     ( (uint32_t)ccp.command_data.memory_size.number_of_bytes32_umb << 16 ) | \
     ( (uint32_t)ccp.command_data.memory_size.number_of_bytes32_lmb << 8  ) | \
     ( (uint32_t)ccp.command_data.memory_size.number_of_bytes32_llb ) )

/*
 * ITS software only provides 24-bit addressing.
 * The upper byte is supplied via ITS_ADDRESS_MASK
 */
#ifndef ITS_PATCH_IS_REQUIRED
#error
#endif

#if ( ITS_PATCH_IS_REQUIRED )
   #define FIX_ADDRESS( a ) ( FIX_BIG_ENDIAN32( a ) | ITS_ADDRESS_MASK )
#else
   #define FIX_ADDRESS( a ) ( FIX_BIG_ENDIAN32( a ) )
#endif

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

typedef enum
{
   MTA0_POINTER,
   MTA1_POINTER

}  MTA_Pointer_T;

/* This enumerated type defines the different types of memory. */

typedef enum
{
   REGISTER,
   RAM,
   EEPROM,
   FLASH_O,
   CAL_FLASH,
   OTHER

}  MemoryArea_T;

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Location_Of_Address_User
 * Purpose:      Application specific function will return an enumerated type
 *               that indicates the type of memory where the input address is
 *               located.
 * Parameters:   uint8_t*, This is the address of the location to be determined.
 * Return Value: MemoryArea_T, memory area type
\*===========================================================================*/
MemoryArea_T CCP_Location_Of_Address_User( HUGE uint8_t* );

/*===========================================================================*\
 * Name:         Get_MTA
 * Purpose:      This function will return the address of the MTA pointer
 *               specified.
 * Parameters:   MTA_Pointer_T, This is the MTA pointer to return.
 * Return Value: void*
\*===========================================================================*/
HUGE void* Get_MTA( MTA_Pointer_T );

/*===========================================================================*\
 * Name:         Move_MTA
 * Purpose:      This function moves the MTA pointer to the input address.
 * Parameters:   uint8_t*, This is the desired new address of the MTA pointer.
 *               uint8_t, This references the MTA pointer that is being used.
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T Move_MTA( HUGE uint8_t*, uint8_t );

/*===========================================================================*\
 * Name:         CCP_Set_MTA
 * Purpose:      Set Memory Transfer Address.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Set_MTA( void );

/*===========================================================================*\
 * Name:         CCP_Dnload
 * Purpose:      Data Download up to 5 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Dnload( void );

/*===========================================================================*\
 * Name:         CCP_Dnload_6
 * Purpose:      Data Download 6 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Dnload_6( void );

/*===========================================================================*\
 * Name:         CCP_Upload
 * Purpose:      Data Upload up to 5 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Upload( void );

/*===========================================================================*\
 * Name:         CCP_Short_Up
 * Purpose:      Data Upload up to 5 bytes from specified source address.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Short_Up( void );

/*===========================================================================*\
 * Name:         CCP_Move
 * Purpose:      Move memory block.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Move( void );

/*===========================================================================*\
 * Name:         CCP_Move_Periodic
 * Purpose:      Complete MOVE in the background and respond.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
void CCP_Move_Periodic( void );

/*===========================================================================*\
 * Name:         CCP_Build_Chksum
 * Purpose:      Build Checksum.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Build_Chksum( void );

/*===========================================================================*\
 * Name:         CCP_Build_Chksum_Periodic
 * Purpose:      Complete BUILD_CHKSUM in the background and respond.
 * Parameters:   None
 * Return Value: None
\*===========================================================================*/
void CCP_Build_Chksum_Periodic( void );

/*===========================================================================*\
 * Name:         CCP_Clear_Memory
 * Purpose:      This command may be used to erase FLASH EPROM prior to
 *               reprogramming.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Clear_Memory( void );

/*===========================================================================*\
 * Name:         CCP_Program
 * Purpose:      Program up to 5 bytes of data into non-volatile memory.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Program( void );

/*===========================================================================*\
 * Name:         CCP_Program_6
 * Purpose:      Program 6 bytes of data into non-volatile memory.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
\*===========================================================================*/
CCP_CRC_T CCP_Program_6( void );

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

#endif /* CCP2MEM_H */
