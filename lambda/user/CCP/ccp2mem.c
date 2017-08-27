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
 * ccp2mem.c
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
#include "io_config.h"
#include "ccp2mem.h"
#include "ccp2conf.h"

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*
 *checksum calculate by byte,CHK_SUM_LENGTH=1 ;by word, CHK_SUM_LENGTH=2 
 */
#define CHK_SUM_LENGTH				   2
/*
 * For ease of reading...
 */
#define TOTAL_MTA_POINTERS             2

#define MAX_NUMBER_OF_READ_WRITE_BYTES 6
#define READ_WRITE_START_BYTE          3
#define READ_WRITE_6_START_BYTE        2

/* This macro determines if the input number references a valid MTA Pointer.
 */
#define MTA_Number_Is_Valid( MTA_number ) ( (MTA_number) < TOTAL_MTA_POINTERS )

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/

/*===========================================================================*\
 * Static Variables
\*===========================================================================*/

static HUGE uint8_t* MTA_ptr[ TOTAL_MTA_POINTERS ];

/*===========================================================================*\
 * Function Prototypes
\*===========================================================================*/

/*===========================================================================*\
 * Inline Functions
\*===========================================================================*/

/*===========================================================================*\
 * Static Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         Read_From_RAM_Or_Register
 * Purpose:      This function read a one-byte data from RAM or Register
 *               memory.
 * Parameters:   uint8_t*, This references the address where data is read from.
 *               uint8_t*, This is the output parameter where the read data is output.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  This function is only capable of reading from the location
 *               specified by the parameter address. It will check to
 *               ensure that the address is positioned in RAM or Register memory,
 *               if it is not it will return an error. This function is only
 *               capable of reading a one-byte data element.
\*===========================================================================*/
static CCP_CRC_T Read_From_RAM_Or_Register( HUGE uint8_t* address,uint8_t* data )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   if ( RAM == CCP_Location_Of_Address_User( address ))
   {
      *data =  *address ;
   }
   else if ( REGISTER == CCP_Location_Of_Address_User ( address ))
   {
      *data = CCP_Read_From_REGISTER_Memory_User(address);
   }
   else if ( EEPROM == CCP_Location_Of_Address_User ( address ))
   {
      *data =  *address ;
   }
   else if ( FLASH_O== CCP_Location_Of_Address_User ( address ))
   {
      *data =  *address ;
   }
   else if ( CAL_FLASH == CCP_Location_Of_Address_User ( address ))
   {
#ifdef INSTRUMENTATION_ACTIVE
      *data =  *((uint8_t *)(address - CAL_STARTADDR + INSTRUMENTATION_CAL_RAM_STARTADDR)) ;
#else
      *data =  *address ;
#endif
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;

}

/*===========================================================================*\
 * Name:         Write_To_RAM_Or_Register
 * Purpose:      This function writes the input byte to RAM or Register
 *               memory.
 * Parameters:   uint8_t, This references the MTA pointer being used.
 *               uint8_t, This is the data that will be written to the address
 *                        referenced by the MTA pointer.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  This function is only capable of writing to the location
 *               specified by the MTA Pointer number. It will check to
 *               ensure that the MTA is positioned in RAM or Register memory,
 *               if it is not it will return an error. This function is only
 *               capable of writing a one byte data element.
\*===========================================================================*/
static CCP_CRC_T Write_To_RAM_Or_Register( uint8_t MTA_number,
                                           uint8_t data_to_write )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   if ( !MTA_Number_Is_Valid( MTA_number ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else if ( RAM == CCP_Location_Of_Address_User( MTA_ptr[ MTA_number ] ) )
   {
      *MTA_ptr[ MTA_number ] = data_to_write;
   }
#ifdef INSTRUMENTATION_ACTIVE
   else if ( CAL_FLASH == CCP_Location_Of_Address_User ( MTA_ptr[ MTA_number ] ))
   {
      *((uint8_t*)(MTA_ptr[ MTA_number ] - CAL_STARTADDR + INSTRUMENTATION_CAL_RAM_STARTADDR)) =  data_to_write;
   }
#endif
   else if ( REGISTER == CCP_Location_Of_Address_User( MTA_ptr[ MTA_number] ) )
   {
      error_to_return = CCP_Write_To_REGISTER_Memory_User( MTA_ptr[ MTA_number ], data_to_write );
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Write_To_Flash
 * Purpose:      This function writes the input data to the current location
 *               of the MTA pointer referenced by the MTA number.
 * Parameters:   uint8_t, This references the MTA pointer being used.
 *               uint8_t, This is the data that will be written to the address
 *                        referenced by the MTA pointer.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Any invalid MTA number will return an error. Before writing
 *               it will first check to ensure that the MTA pointer is
 *               positioned in Flash, if it is not it will return an error.
 *               This function assumes that the active memory page is the
 *               target. It is the calling modules responsibility to ensure
 *               that the input address and the active memory page are
 *               correct. It only supports uint8_t data writes.
\*===========================================================================*/
static CCP_CRC_T Write_To_Flash( uint8_t MTA_number, uint8_t data_to_write )
{
   CCP_CRC_T  error_to_return = CRC_NO_ERROR;

   if( ! MTA_Number_Is_Valid( MTA_number ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else if( FLASH_O == CCP_Location_Of_Address_User( MTA_ptr[ MTA_number ] ) )
   {
      CCP_Write_To_FLASH_Memory_User( MTA_ptr[ MTA_number ], data_to_write );
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Write_To_EEPROM
 * Purpose:      This function writes the input data to the current location
 *               of the MTA pointer referenced by the MTA number.
 * Parameters:   uint8_t, This references the MTA pointer being used.
 *               uint8_t, This is the data that will be written to the address
 *                        referenced by the MTA pointer.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Any invalid MTA number will return an error. Before writing
 *               it will first check to ensure that the MTA pointer is
 *               positioned in EEPROM, if it is not it will return an error.
 *               This function only supports uint8_t data writes.
\*===========================================================================*/
static CCP_CRC_T Write_To_EEPROM( uint8_t MTA_number, uint8_t data_to_write )
{
   CCP_CRC_T  error_to_return = CRC_NO_ERROR;
   
   if( ! MTA_Number_Is_Valid( MTA_number ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else if( EEPROM == CCP_Location_Of_Address_User( MTA_ptr[ MTA_number ] ) )
   {
      CCP_Write_To_EEPROM_Memory_User( MTA_ptr[ MTA_number ], data_to_write );
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Erase_All_Of_Flash
 * Purpose:      This function erases all of Flash.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  THIS IS A STUB FUNCTION!!!  It will most likely never need
 *               to be implemented.  It returns a positive response to avoid
 *               any potential tool issues.
\*===========================================================================*/
static CCP_CRC_T Erase_All_Of_Flash( void )
{
   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         Download_Data
 * Purpose:      This function writes up to six bytes of data to memory
 *               starting at the location pointed to by MTA0.
 * Parameters:   uint8_t, The byte in the CAN message which has the first bit
 *                        of relevant information.
 *               uint8_t, The number of bytes of relevant information.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  It is called by CCP_Dnload and CCP_Dnload_6.
\*===========================================================================*/
static CCP_CRC_T Download_Data( uint8_t start_byte, uint8_t length )
{
   HUGE uint8_t*  temp_MTA_ptr    = MTA_ptr[ MTA0_POINTER ];
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint8_t   loop_index;

   for ( loop_index = start_byte; loop_index < ( start_byte + length ); loop_index++ )
   {
      error_to_return = Write_To_RAM_Or_Register( MTA0_POINTER,
                                                  ccp.command_data.data_byte[ loop_index ] );
      temp_MTA_ptr++;
      /*
       * Do not report an error if incrmented to an invalid memory location.
       * It causes a problem when accessing the last byte of a memory
       * segment.
       */
      Move_MTA( temp_MTA_ptr, MTA0_POINTER );
   }

   ccp.response_data.address.address_location32 = FIX_ADDRESS( (uint32_t)temp_MTA_ptr );

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Program_Data
 * Purpose:      This function writes up to six bytes of data to non-volatile
 *               memory starting at the location pointed to by MTA0.
 * Parameters:   uint8_t, The byte in the CAN message which has the first bit
 *                        of relevant information.
 *               uint8_t, The number of bytes of relevant information.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  It is called by CCP_Program and CCP_Program_6.
\*===========================================================================*/
static CCP_CRC_T Program_Data( uint8_t start_byte, uint8_t length )
{
   HUGE uint8_t*  temp_MTA_ptr    = MTA_ptr[ MTA0_POINTER ];
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint8_t   loop_index;

   for ( loop_index = start_byte; loop_index < ( start_byte + length ); loop_index++ )
   {
      if ( EEPROM == CCP_Location_Of_Address_User( MTA_ptr[ MTA0_POINTER ] ) )
      {
         error_to_return = Write_To_EEPROM( MTA0_POINTER,
                                            ccp.command_data.data_byte[ loop_index ] );
      }
      else if ( FLASH_O == CCP_Location_Of_Address_User( MTA_ptr[ MTA0_POINTER ] ) )
      {
         error_to_return = Write_To_Flash( MTA0_POINTER,
                                           ccp.command_data.data_byte[ loop_index ] );
      }
      else
      {
         error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
      }
   
      temp_MTA_ptr++;
      /*
       * Do not report an error if incrmented to an invalid memory location.
       * It causes a problem when accessing the last byte of a memory
       * segment.
       */
      Move_MTA( temp_MTA_ptr, MTA0_POINTER );
   }

   return error_to_return;
}

/*===========================================================================*\
 * Global Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         Get_MTA
 * Purpose:      This function will return the address of the MTA pointer
 *               specified.
 * Parameters:   MTA_Pointer_T, This is the MTA pointer to return.
 * Return Value: void*
\*===========================================================================*/
HUGE void* Get_MTA( MTA_Pointer_T mta )
{
   return MTA_ptr[ mta ];
}

/*===========================================================================*\
 * Name:         Move_MTA
 * Purpose:      This function moves the MTA pointer to the input address.
 * Parameters:   uint8_t*, This is the desired new address of the MTA pointer.
 *               uint8_t, This references the MTA pointer thatis being used.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
\*===========================================================================*/
CCP_CRC_T Move_MTA( HUGE uint8_t* address, uint8_t MTA_number )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   if ( !MTA_Number_Is_Valid( MTA_number ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else if ( OTHER == CCP_Location_Of_Address_User( address ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else
   {
      MTA_ptr[ MTA_number ] = address;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Set_MTA
 * Purpose:      Set Memory Transfer Address.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  This command will initialize a base pointer (32Bit +
 *               extension) for all following memory transfers. The address
 *               extension depends on the slave controller's organization and
 *               may identify a switchable memory bank or a memory segment.
 *
 *               The MTA number (handle) is used to identify different
 *               transfer address locations (pointers). MTA0 is used by the
 *               commands DNLOAD, UPLOAD, DNLOAD_6, SELECT_CAL_PAGE, CLEAR_
 *               MEMORY, PROGRAM, and PROGRAM_6. MTA1 is used by the MOVE
 *               command. See also command 'MOVE'.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = SET_MTA 0x02
 * 1           byte           Command Counter = CTR
 * 2           byte           Memory transfer address MTA number (0,1)
 * 3           byte           Address extension
 * 4..7        bytes          Address
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Set_MTA( void )
{
   return Move_MTA( (HUGE uint8_t*)FIX_ADDRESS( ccp.command_data.address.address_location32 ),
                    ccp.command_data.address.MTA_or_size );
}

/*===========================================================================*\
 * Name:         CCP_Dnload
 * Purpose:      Data Download up to 5 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The data block with the specified length (size) contained
 *               in the CRO will be copied into memory, starting at the
 *               current Memory Transfer Address0 (MTA0). The MTA0 pointer
 *               will be post-incremented by the value of 'size'.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = DNLOAD 0x03
 * 1           byte           Command Counter = CTR
 * 2           byte           size of data block to follow in bytes
 * 3..7        bytes          data to be transferred (up to 5 bytes)
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           MTA0 extension (after post-increment)
 * 4..7        bytes          MTA0 address (after post-increment)
\*===========================================================================*/
CCP_CRC_T CCP_Dnload( void )
{
   return Download_Data( READ_WRITE_START_BYTE, ccp.command_data.dnload_5.size );
}

/*===========================================================================*\
 * Name:         CCP_Dnload_6
 * Purpose:      Data Download 6 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The data block with the fixed length (size) of 6 bytes
 *               contained in the CRO will be copied into memory, starting
 *               at the current Memory Transfer Address0 (MTA0). The MTA0
 *               pointer will be post-incremented by the value of 6.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = DNLOAD_6 0x23
 * 1           byte           Command Counter = CTR
 * 2..7        bytes          6 bytes of data to be transferred
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           MTA0 extension (after post-increment)
 * 4..7        bytes          MTA0 address (after post-increment)
\*===========================================================================*/
CCP_CRC_T CCP_Dnload_6( void )
{
   return Download_Data( READ_WRITE_6_START_BYTE, MAX_NUMBER_OF_READ_WRITE_BYTES );
}

/*===========================================================================*\
 * Name:         CCP_Upload
 * Purpose:      Data Upload up to 5 Bytes.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  A data block of the specified length (size), starting at
 *               current MTA0, will be copied into the corresponding DTO data
 *               field. The MTA0 pointer will be post-incremented by the
 *               value of 'size'.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = UPLOAD 0x04
 * 1           byte           Command Counter = CTR
 * 2           byte           size of data block to be uploaded in bytes
 * 3..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          requested data bytes
\*===========================================================================*/
CCP_CRC_T CCP_Upload( void )
{
   HUGE uint8_t*  temp_MTA_ptr    = MTA_ptr[ MTA0_POINTER ];
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint8_t   loop_index;
   uint8_t   data;

   for( loop_index = READ_WRITE_START_BYTE;
        loop_index < ( ccp.command_data.dnload_5.size + READ_WRITE_START_BYTE );
        loop_index++ )
   {
      error_to_return = Read_From_RAM_Or_Register( temp_MTA_ptr, &data );
      
      if ( CRC_PARAMETER_OUT_OF_RANGE != error_to_return )
      {
         ccp.response_data.data_byte[ loop_index ] = data ;
      }

      temp_MTA_ptr++;
   }

   /*
    * Do not report an error if incrmented to an invalid memory location.
    * It causes a problem when accessing the last byte of a memory
    * segment.
    */
   Move_MTA( temp_MTA_ptr, MTA0_POINTER );

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Short_Up
 * Purpose:      Data Upload up to 5 bytes from specified source address.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  A data block of the specified length (size), starting at
 *               source address will be copied into the corresponding DTO
 *               data field. The MTA0 pointer remains unchanged.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = SHORT_UP 0x0F
 * 1           byte           Command Counter = CTR
 * 2           byte           size of data block to be uploaded in bytes
 *                            (1..5)
 * 3           byte           Address extension
 * 4..7        unsigned long  Address
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          requested data bytes
\*===========================================================================*/
CCP_CRC_T CCP_Short_Up( void )
{
   HUGE uint8_t*  temp_MTA_ptr    = (HUGE uint8_t*)FIX_ADDRESS( ccp.command_data.address.address_location32 );
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint8_t   loop_index;
   uint8_t   data;

   for ( loop_index = 0; (loop_index < ccp.command_data.address.MTA_or_size) && (CRC_NO_ERROR == error_to_return); loop_index++ )
   {
      error_to_return = Read_From_RAM_Or_Register( temp_MTA_ptr, &data ) ;

      if ( CRC_PARAMETER_OUT_OF_RANGE != error_to_return )
      {
         ccp.response_data.data_byte[ READ_WRITE_START_BYTE + loop_index ] = data ;
         temp_MTA_ptr++;
      }
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Move
 * Purpose:      Move memory block.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  A data block of the specified length (size) will be copied
 *               from the address defined by MTA0 (source pointer) to the
 *               address defined by MTA1 (destination pointer).
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = MOVE 0x19
 * 1           byte           Command Counter = CTR
 * 2..5        long           Size (number of bytes) of data block to move
 * 6..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Move( void )
{
   CCP_CRC_T    error_to_return = CRC_NO_ERROR;
   MemoryArea_T loc_mta0, loc_mta1;

   loc_mta0 = CCP_Location_Of_Address_User( MTA_ptr[ MTA0_POINTER ] );
   loc_mta1 = CCP_Location_Of_Address_User( MTA_ptr[ MTA1_POINTER ] );

   if ( (OTHER == loc_mta0) || (OTHER == loc_mta1) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else
   {
      /*
       * Init ccp.MoveSize and indicate delayed response to main
       */
      ccp.MoveSize         = READ_UNALIGNED_MEMORY_SIZE();
      ccp.delayed_response = true;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Move_Periodic
 * Purpose:      Complete MOVE in the background and respond.
 * Parameters:   None
 * Return Value: None
 * Design Info:  Setting ccp.MoveSize to a non-zero value will initiate
 *               this background routine to copy data and send the response
 *               to the MOVE command.
\*===========================================================================*/
void CCP_Move_Periodic( void )
{
   uint32_t counter;

   if ( ccp.MoveSize )
   {
      if ( ccp.MoveSize <= 255UL )
      {
         counter = ccp.MoveSize;
         ccp.MoveSize = 0;
      }
      else
      {
         counter = 256UL;
         ccp.MoveSize -= 256UL;
      }

      do
      {
         *MTA_ptr[ MTA1_POINTER ] = *MTA_ptr[ MTA0_POINTER ];
         MTA_ptr[ MTA0_POINTER ]++;
         MTA_ptr[ MTA1_POINTER ]++;

      } while ( --counter );

      if ( ccp.MoveSize == 0UL )
      {
         /*
          * Complete the delayed CRM
          */
         CCP_Send_Delayed_CRM( CRC_NO_ERROR );
      }
   }
}

/*===========================================================================*\
 * Name:         CCP_Build_Chksum
 * Purpose:      Build Checksum.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Returns a checksum result of the memory block that is
 *               defined by MTA0 (memory transfer area start address) and
 *               block 'size'. The checksum algorithm may be manufacturer
 *               and/or project specific, it is not part of this
 *               specification.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = BUILD_CHKSUM 0x0E
 * 1           byte           Command Counter = CTR
 * 2..5        unsigned long  block size in bytes
 * 6..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           size of checksum data
 * 4..7        bytes          checksum data (implementation specific)
\*===========================================================================*/
CCP_CRC_T CCP_Build_Chksum( void )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   if ( OTHER == CCP_Location_Of_Address_User( MTA_ptr[ MTA0_POINTER ] ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else
   {
      /*
       * Init ccp.ChecksumSize and indicate delayed response to main
       */
      ccp.Checksum         = 0;
	  //get check sum counts
	  ccp.ChecksumSize	   = (READ_UNALIGNED_MEMORY_SIZE()/CHK_SUM_LENGTH);
      ccp.delayed_response = true;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Build_Chksum_Periodic
 * Purpose:      Complete BUILD_CHKSUM in the background and respond.
 * Parameters:   None
 * Return Value: None
 * Design Info:  Setting ccp.ChecksumSize to a non-zero value will initiate
 *               this background routine to compute the checksum and send the
 *               response to the BUILD_CHKSUM command.
 *               For ETAS compatibility, this routine computes a 16-bit
 *               checksum, 256 16-bit words per invocation.
\*===========================================================================*/
void CCP_Build_Chksum_Periodic( void )
{
   uint16_t temp_checksum;
   uint32_t counter;
   HUGE uint16_t* dyn_addr;

   if ( ccp.ChecksumSize )
   {
      if ( ccp.ChecksumSize <= 255UL )
      {
         counter = ccp.ChecksumSize;
         ccp.ChecksumSize = 0;
      }
      else
      {
         counter = 256UL;
         ccp.ChecksumSize -= 256UL;
      }

      temp_checksum = ccp.Checksum;
      dyn_addr = (HUGE uint16_t *)(MTA_ptr[MTA0_POINTER]);

      do
      {
		  temp_checksum += (uint16_t)(*dyn_addr);
		  dyn_addr++;
      } while ( --counter );
      //move MTA
      MTA_ptr[MTA0_POINTER]= (HUGE uint8_t *)dyn_addr;
      ccp.Checksum = temp_checksum;

      if ( ccp.ChecksumSize == 0UL )
      {
         ccp.response_data.return_checksum.checksum_size = 2;
         ccp.response_data.return_checksum.size16 = FIX_BIG_ENDIAN16( ccp.Checksum );

         /*
          * Complete the delayed CRM
          */
         CCP_Send_Delayed_CRM( CRC_NO_ERROR );
      }
   }
}

/*===========================================================================*\
 * Name:         CCP_Clear_Memory
 * Purpose:      This command may be used to erase FLASH EPROM prior to
 *               reprogramming.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The MTA0 pointer points to the memory location to be erased.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = CLEAR_MEMORY 0x10
 * 1           byte           Command Counter = CTR
 * 2..5        bytes          Memory size
 * 6..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
 *
 * This function clears the block of memory defined by the location of
 * the MTA pointer #0 and the input block size.  The size is in bytes.
 *
 * This function relies on the current position of MTA pointer #0.  The
 * calling function is responsible for ensuring that the correct memory page
 * is active (if clearing paged Flash) or that MTA pointer #0 is positioned
 * correctly.
 *
 * When clearing a block of memory, this function enters a loop and keeps
 * clearing bytes of memory until a block of the input size has been
 * cleared.  If the calling module is using this function to clear a block
 * of RAM and it reaches the end of RAM before this loop has been completed,
 * the function stops clearing memory and returns to the calling application
 * with an error.
 *
 * When clearing Flash, the calling module must clear all of Flash at the
 * same time.  To do this the user must position MTA pointer #0 on the first
 * address of paged flash, memory page number 0, and call this command.  All
 * of Flash will then be erased.
 *
 * This function uses MTA pointer #0 to move through memory and clear it.
 * This is because the Utilities to move MTA pointers do extensive error
 * checking to ensure that the MTA pointer is being moved to a valid memory
 * address or memory page.  Even though it is used to transverse the memory
 * block to be cleared, MTA pointer #0 is moved back to its original
 * location after the operation.
\*===========================================================================*/
CCP_CRC_T CCP_Clear_Memory( void )
{
   /* Only the MTA pointer address is needed, not the entire structure. */
   HUGE uint8_t*     saved_MTA_ptr   = MTA_ptr[ MTA0_POINTER ];
   HUGE uint8_t*     temp_MTA_ptr    = MTA_ptr[ MTA0_POINTER ];
   MemoryArea_T temp_location   = CCP_Location_Of_Address_User( MTA_ptr[ MTA0_POINTER ] );
   CCP_CRC_T    error_to_return = CRC_NO_ERROR;
   uint32_t     loop_index, max_index;

   if ( FLASH_O == temp_location )
   {
      error_to_return = Erase_All_Of_Flash();
   }
   else if ( (RAM    == temp_location) ||
             (EEPROM == temp_location) )
   {
      max_index = READ_UNALIGNED_MEMORY_SIZE();
      for ( loop_index = 0; loop_index <= max_index; loop_index++ )
      {
         if ( RAM == temp_location )
         {
            error_to_return = Write_To_RAM_Or_Register( MTA0_POINTER, CLEAR_DATA );
         }
         else
         {
            error_to_return = Write_To_EEPROM( MTA0_POINTER, CLEAR_DATA );
         }

         temp_MTA_ptr++;

         /*
          * Do not report an error if incrmented to an invalid memory location.
          * It causes a problem when accessing the last byte of a memory
          * segment.
          */
         Move_MTA( temp_MTA_ptr, MTA0_POINTER );
      }

      Move_MTA( saved_MTA_ptr, MTA0_POINTER );
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Program
 * Purpose:      Program up to 5 bytes of data into non-volatile memory.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The data block of the specified length (size) contained in
 *               the CRO will be programmed into non-volatile memory (FLASH,
 *               EEPROM), starting at current MTA0. The MTA0 pointer will be
 *               post-incremented by the value of 'size'.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = PROGRAM 0x18
 * 1           byte           Command Counter = CTR
 * 2           byte           Size of data block to follow (bytes)
 * 2..7        bytes          Data to be programmed (6 bytes)
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           MTA0 extension (after post-increment)
 * 4..7        bytes          MTA0 address (after post-increment)
\*===========================================================================*/
CCP_CRC_T CCP_Program( void )
{
   return Program_Data( READ_WRITE_START_BYTE, ccp.command_data.address.MTA_or_size );
}

/*===========================================================================*\
 * Name:         CCP_Program_6
 * Purpose:      Program 6 bytes of data into non-volatile memory.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The data block of the length (size) of 6 bytes contained in
 *               the CRO will be programmed into non-volatile memory (FLASH,
 *               EEPROM), starting at current MTA0. The MTA0 pointer will be
 *               post-incremented by 6.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = PROGRAM_6 0x22
 * 1           byte           Command Counter = CTR
 * 2           byte           Size of data block to follow (bytes)
 * 3..7        bytes          Data to be programmed (max. 5 bytes)
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           MTA0 extension (after post-increment)
 * 4..7        bytes          MTA0 address (after post-increment)
\*===========================================================================*/
CCP_CRC_T CCP_Program_6( void )
{
   return Program_Data( READ_WRITE_6_START_BYTE, MAX_NUMBER_OF_READ_WRITE_BYTES );
}
