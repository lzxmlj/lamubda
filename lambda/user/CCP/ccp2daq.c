/*===========================================================================*\
 *
 *       COPYRIGHT, 2001, DELPHI DELCO ELECTRONICS SYSTEMS CORPORATION
 *
 *===========================================================================
 * DESCRIPTION:
 * This module contains support for the following CCP 2.1 commands:
 *
 *    GET_DAQ_SIZE
 *    GET_DAQ_PTR
 *    WRITE_DAQ
 *    START_STOP
 *    START_STOP_ALL
 *
 * MODULE NAME:
 * ccp2daq.c
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

#include "ccp2daq.h"
#include "ccp2mem.h"
#include "ccp2mode.h"

#ifdef NUMBER_OF_RESUMEABLE_DAQ_LISTS
#if ( NUMBER_OF_RESUMEABLE_DAQ_LISTS > 0 )
   #include "ccp2nvmd.h"
#endif
#endif

/*===========================================================================*\
 * Macro Definitions
\*===========================================================================*/

/*
 * These are for ease of reading and are simply textual representations of zero
 */
#define FIRST_DAQ_NUMBER         (0x00)
#define FIRST_ODT_NUMBER         (0x00)
#define FIRST_ODT_ELEMENT_NUMBER (0x00)

/*===========================================================================*\
 * This macro is used to detect if a reference (of type DAQ_reference_T) is the
 * last element of an ODT.  If it is it returns true, if not it returns
 * false.
\*===========================================================================*/
#define DAQ_Reference_Is_Last_Element_Of_ODT( reference ) \
   ( ( CCP_DAQ_Configuration[ (reference).DAQ_number ].number_of_elements_per_ODT - 1 ) \
     == ( (reference).element_number ) )

/*===========================================================================*\
 * This macro is used to determine the number of ODTs in the specified DAQ
 * list.
\*===========================================================================*/
#define Number_Of_ODTs_In_DAQ_List( DAQ_number ) \
   ( CCP_DAQ_Configuration[ (DAQ_number) ].number_of_ODTs )

/*===========================================================================*\
 * This macro is used to determine the number of elements in an ODT for the
 * specified DAQ list.
\*===========================================================================*/
#define Number_Of_Elements_In_ODT( DAQ_number ) \
   ( CCP_DAQ_Configuration[ (DAQ_number) ].number_of_elements_per_ODT )

/*===========================================================================*\
 * This macro is used to determine if a DAQ number is a valid one.
\*===========================================================================*/
#define DAQ_Number_Is_Valid( DAQ_number ) \
   ( (DAQ_number) < NUMBER_OF_DAQ_LISTS )

/*===========================================================================*\
 * This macro is used to determine if a reference (of type DAQ_reference_T) has
 * a valid DAQ number, ODT number, and element number.
\*===========================================================================*/
#define DAQ_Reference_Is_Valid( reference ) \
   ( DAQ_Number_Is_Valid( (reference).DAQ_number ) && \
     ( (reference).ODT_number < Number_Of_ODTs_In_DAQ_List( (reference).DAQ_number ) ) && \
     ( (reference).element_number < CCP_DAQ_Configuration[ (reference).DAQ_number ].number_of_elements_per_ODT ) )

/*===========================================================================*\
 * This macro checks if the reference (of type DAQ_reference_T) is the final ODT
 * table in the DAQ list.  If it is it returns true, if not it returns false.
\*===========================================================================*/
#define DAQ_Reference_Is_Last_ODT( reference ) \
  ( ( Number_Of_ODTs_In_DAQ_List( (reference).DAQ_number ) - 1 ) == ( (reference).ODT_number ) )

/*===========================================================================*\
 * Constant Definitions
\*===========================================================================*/

/*===========================================================================*\
 * Enumeration Definitions
\*===========================================================================*/
HUGE uint8_t *destination_ptr;

/*
 * DAQ element transfer sizes (defined in CCP 2.1 documentation)
 */
typedef enum
{
   NO_SIZE_XFER  = 0x00,
   BYTE8_XFER    = 0x01,
   WORD16_XFER   = 0x02,
   LONG32_XFER   = 0x04

}  DAQElementSize_T;

/*
 * Type for the starting and stopping of DAQ Lists (defined in CCP 2.1 documentation)
 */
typedef enum
{
   STOP               = 0x00,
   START              = 0x01,
   START_SYNCHRONIZED = 0x02

}  DAQStartStop_T;

/*===========================================================================*\
 * Type Definitions
\*===========================================================================*/

/*
 * The following data structure contains all relevant additional information
 * for a specific DAQ List
 */
typedef struct DAQ_List_Status_tag
{
   uint32_t message_id;                  /* from GET_DAQ_SIZE message          */
   uint16_t transmission_rate_prescaler; /* from START_STOP message            */
   uint16_t prescaler_counter;           /* counter for prescaler              */
   uint16_t trigger_overrun_counter;     /* counter for number of overruns     */
   uint8_t  event_channel_number;        /* from START_STOP message            */
   uint8_t  current_ODT;                 /* counter for current ODT            */
   uint8_t  first_PID;                   /* from GET_DAQ_SIZE message          */
   uint8_t  last_ODT_to_transmit;        /* from START_STOP message            */
   uint8_t  sending;                     /* flag DAQ is currently transmitting */
   uint8_t  started;                     /* flag DAQ has been started          */
   uint8_t  start_synchronized;          /* from START_STOP message            */
   uint8_t  reserved[3];                 /* pad structure to even boundary     */

}  DAQ_List_Status_T;

/*
 * The following type allows us to 'address' into the DAQ Lists by specifying
 * an exact element, inside an exact ODT inside an exact DAQ.  This is the
 * data type that is used to input data to DAQ functions.
 */
typedef struct DAQ_Reference_tag
{
   uint8_t DAQ_number;
   uint8_t ODT_number;
   uint8_t element_number;

}  DAQ_Reference_T;

/*
 * The following is the data type for an ODT element
 */
typedef struct ODT_Element_tag
{
   HUGE uint8_t* address;
   uint8_t  size;

}  ODT_Element_T;

/*===========================================================================*\
 * Global Variables
\*===========================================================================*/
   uint32_t      temp_address1;

/*********************************************************
*-ccp2daq.DAQ_List_Status{
*-  OBJECTS_CLASS: parameter_es_class;
*-  label                    = "DAQOVRN0";
*-  variable_location.symbol = ccp2daq.DAQ_List_Status;
*-  variable_location.offset = 0x08;
*-  ds_precision             = "4.0";
*-  ds_scaling_mode          = "Ax + B";
*-  ds_const_a               = "1.000000000000";
*-  ds_const_b               = "0.000000000000";
*-  ds_scaling_unit          = "Counts";
*-  description              = "DAQ0_Overrun_Counter:DAQ 0 trigger overrun counter. Reduce amount of logged data if non-zero. Range = 0 to 65535";
*-}
**********************************************************/

/*********************************************************
*-ccp2daq.DAQ_List_Status{
*-  OBJECTS_CLASS: parameter_es_class;
*-  label                    = "DAQOVRN1";
*-  variable_location.symbol = ccp2daq.DAQ_List_Status;
*-  variable_location.offset = 0x1c;
*-  ds_precision             = "4.0";
*-  ds_scaling_mode          = "Ax + B";
*-  ds_const_a               = "1.000000000000";
*-  ds_const_b               = "0.000000000000";
*-  ds_scaling_unit          = "Counts";
*-  description              = "DAQ1_Overrun_Counter:DAQ 1 trigger overrun counter. Reduce amount of logged data if non-zero. Range = 0 to 65535";
*-}
**********************************************************/

/*********************************************************
*-ccp2daq.DAQ_List_Status{
*-  OBJECTS_CLASS: parameter_es_class;
*-  label                    = "DAQOVRN2";
*-  variable_location.symbol = ccp2daq.DAQ_List_Status;
*-  variable_location.offset = 0x30;
*-  ds_precision             = "4.0";
*-  ds_scaling_mode          = "Ax + B";
*-  ds_const_a               = "1.000000000000";
*-  ds_const_b               = "0.000000000000";
*-  ds_scaling_unit          = "Counts";
*-  description              = "DAQ2_Overrun_Counter:DAQ 2 trigger overrun counter. Reduce amount of logged data if non-zero. Range = 0 to 65535";
*-}
**********************************************************/

/*********************************************************
*-ccp2daq.DAQ_List_Status{
*-  OBJECTS_CLASS: parameter_es_class;
*-  label                    = "DAQOVRN3";
*-  variable_location.symbol = ccp2daq.DAQ_List_Status;
*-  variable_location.offset = 0x44;
*-  ds_precision             = "4.0";
*-  ds_scaling_mode          = "Ax + B";
*-  ds_const_a               = "1.000000000000";
*-  ds_const_b               = "0.000000000000";
*-  ds_scaling_unit          = "Counts";
*-  description              = "DAQ3_Overrun_Counter:DAQ 3 trigger overrun counter. Reduce amount of logged data if non-zero. Range = 0 to 65535";
*-}
**********************************************************/

/*===========================================================================*\
 * Static Variables
\*===========================================================================*/
static const uint16_t remainder_mask[15] = {
	3U << 0,
	3U << 1,
	3U << 2,
	3U << 3,
	3U << 4,
	3U << 5,
	3U << 6,
	3U << 7,
	3U << 8,
	3U << 9,
	3U << 10,
	3U << 11,
	3U << 12,
	3U << 13,
	3U << 14
};

/*
 * This array is used to keep track of all additional relevant information
 * for a specific DAQ List.
 */
DAQ_List_Status_T DAQ_List_Status[ NUMBER_OF_DAQ_LISTS ];

/*
 * This variable is used to keep track of the current DAQ reference.
 */
static DAQ_Reference_T current_DAQ_ptr_reference;

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
 * Name:         DAQ_Starting_PID
 * Purpose:      Compute starting PID for DAQ list.
 * Parameters:   uint8_t, DAQ number
 * Return Value: uint8_t, starting PID for DAQ list
 * Design Info:  None
\*===========================================================================*/
static uint8_t DAQ_Starting_PID( uint8_t DAQ_number )
{
   uint8_t DAQ_counter = 0;
   uint8_t PID_number  = 0;

   while ( DAQ_counter < DAQ_number )
   {
      PID_number += CCP_DAQ_Configuration[ DAQ_counter ].number_of_ODTs;
      DAQ_counter++;
   }

   return PID_number;
}

/*===========================================================================*\
 * Name:         Update_Session_Status_LOGGING
 * Purpose:      Update SS_LOGGING bit in session status.
 * Parameters:   None
 * Return Value: None
 * Design Info:  SS_LOGGING is set if any DAQ lists are started, otherwise the
 *               SS_LOGGING bit is cleared.
\*===========================================================================*/
static void Update_Session_Status_LOGGING( void )
{
uint8_t   DAQ_number = FIRST_DAQ_NUMBER;
uint8_t   old_status = ccp.session_status;
bool      started    = false;

   while ( !started && DAQ_Number_Is_Valid(DAQ_number) )
   {
      if ( DAQ_List_Status[ DAQ_number ].started )
      {
         started = true;
      }
      else
      {
         DAQ_number++;
      }
   }

   if ( started )
   {
      ccp.session_status |= (uint8_t)SS_LOGGING;
   }
   else
   {
      ccp.session_status &= (uint8_t)((~SS_LOGGING)&SS_MASK);
   }

   CCP_Set_S_Status_User( old_status, ccp.session_status );
}

/*===========================================================================*\
 * Name:         Read_DAQ_Address_Size
 * Purpose:      Read address and size from current DAQ reference.
 * Parameters:   uint8_t, DAQ number - reference to element
 *               uint8_t, ODT number - reference to element
 *               uint8_t, Element number - reference to element
 *               ODT_Element_T, DAQ_address_size - referenced struct which includes the required address and size 
 * Return Value: void
 * Design Info:  See Write_to_Current_DAQ_Pointer
\*===========================================================================*/
static void Read_DAQ_Address_Size( uint8_t DAQ_number, uint8_t ODT_number, uint8_t element_number,
                                  ODT_Element_T* DAQ_address_size )
{
   uint16_t  offset;
   HUGE uint16_t* address;
   uint16_t  size;
   uint8_t   remainder;

   offset = (uint16_t)( ( DAQ_ELEMENTS_PER_ODT * ODT_number )
                         + element_number );

   /*
    * Read size
    */
   address = CCP_DAQ_Configuration[ DAQ_number ].config_buffer_ptr
      +
      ( DAQ_ELEMENTS_PER_ODT *
        CCP_DAQ_Configuration[ DAQ_number ].number_of_ODTs )
      +
      ( offset / 8 );

   remainder = (uint8_t)((offset % 8) * 2);

   size = (uint16_t)(*address & remainder_mask[remainder]);
   size >>= remainder;

   DAQ_address_size->address = (HUGE uint8_t*)((uint32_t)(*( CCP_DAQ_Configuration[ DAQ_number ].config_buffer_ptr + offset )) );
   if ( size < 3 )
   {
      DAQ_address_size->size = size;
   }
   else
   {
      DAQ_address_size->size = LONG32_XFER;
   }
   /* add base address*/
   DAQ_address_size->address += CCP_RAM_BASE_ADDRESS;
 
   return;
}

/*===========================================================================*\
 * Name:         Write_to_Current_DAQ_Pointer
 * Purpose:      Store pointer and size to current DAQ reference.
 * Parameters:   ODT_Element_T, element reference
 * Return Value: None
 * Design Info:  The config_buffer_ptr is arranged such that the addresses are
 *               stored in the first section of the buffer.  The second section
 *               contains the sizes, which are 2-bit values packed.
 *               size 0 = no size
 *               size 1 = 1 byte
 *               size 2 = 2 bytes
 *               size 3 = 4 bytes
\*===========================================================================*/
static void Write_to_Current_DAQ_Pointer( ODT_Element_T element )
{
   uint8_t   DAQ_number = current_DAQ_ptr_reference.DAQ_number;

   uint16_t  offset = (uint16_t)( ( CCP_DAQ_Configuration[ DAQ_number ].number_of_elements_per_ODT *
                                    current_DAQ_ptr_reference.ODT_number )
                                  +
                                  current_DAQ_ptr_reference.element_number );
   HUGE uint16_t* address;
   uint16_t  size;
   uint8_t   remainder;
 
   /*
    * Store address
    */
   
   *( CCP_DAQ_Configuration[ DAQ_number ].config_buffer_ptr + offset ) = 
      (0 == element.size ) ? (uint16_t)0U : (uint16_t)(((uint32_t)element.address & 0x0000FFFFUL));
   
   /*
    * Store size
    */
   address = CCP_DAQ_Configuration[ DAQ_number ].config_buffer_ptr
      +
      ( CCP_DAQ_Configuration[ DAQ_number ].number_of_elements_per_ODT *
        CCP_DAQ_Configuration[ DAQ_number ].number_of_ODTs )
      +
      ( offset / 8 );

   remainder = (uint8_t)((offset % 8) * 2);
   if ( element.size < 3 )
   {
         size =  element.size;
   }
   else
   {
      size = (uint16_t)3U;
   }
   *address &= (uint16_t)~(3U << remainder);
   *address |= (uint16_t)(size << remainder);
}

/*===========================================================================*\
 * Name:         Clear_DAQ_List
 * Purpose:      This function clears the DAQ List whose number is passed to
 *               it. When a DAQ List is cleared all it's element values are
 *               set to CLEAR_DATA.
 * Parameters:   uint8_t, This is the number of the DAQ List to clear
 * Return Value: void
 * Design Info:  input value DAQ_number must be valid
\*===========================================================================*/
static void Clear_DAQ_List( uint8_t DAQ_number )
{
   uint16_t  buffer_size = (uint16_t)CCP_CONFIG_BUFFER_SIZE_IN_SHORTWORDS(
      Number_Of_ODTs_In_DAQ_List( DAQ_number ),
      Number_Of_Elements_In_ODT( DAQ_number ) );
   HUGE uint16_t* buffer_ptr = CCP_DAQ_Configuration[ DAQ_number ].config_buffer_ptr;
   uint16_t  buffer_counter;

   for ( buffer_counter = 0; buffer_counter < buffer_size; buffer_counter++ )
   {
      *buffer_ptr = CLEAR_DATA;

      buffer_ptr++;
   }
}

/*===========================================================================*\
 * Name:         DAQ_Transfer_Coherent_List
 * Purpose:      Populate data array for coherent DAQ list.
 * Parameters:   uint8_t, DAQ number - reference to element
 * Return Value: None
 * Design Info:  See non-coherent DAQ list processing
\*===========================================================================*/
static void DAQ_Transfer_Coherent_List( uint8_t DAQ_number )
{
#ifndef FORCE_MSB_FIRST_DAQ_PROCESSING
#error
#endif

   ODT_Element_T  DAQ_address_size ;

#if ( FORCE_MSB_FIRST_DAQ_PROCESSING )
   uint32_t temp_value;
#else
   HUGE uint8_t *temp_ptr;
   int      i; /* coding standard exception */
#endif
   uint8_t message_counter, element_counter, message_byte_counter;

   /* Get a pointer to coherent DAQ list storage */
   destination_ptr = CCP_DAQ_Configuration[ DAQ_number ].data_buffer_ptr;

   /* Process all ODTs */
   for ( message_counter = FIRST_ODT_NUMBER;
         message_counter <= DAQ_List_Status[ DAQ_number ].last_ODT_to_transmit;
         message_counter++ )
   {
      message_byte_counter = 0;

      for( element_counter = FIRST_ODT_ELEMENT_NUMBER;
           element_counter < Number_Of_Elements_In_ODT( DAQ_number );
           element_counter++ )
      {
         Read_DAQ_Address_Size( DAQ_number, message_counter, element_counter, &DAQ_address_size ); 

         if ( NO_SIZE_XFER == DAQ_address_size.size ) 
         {
            break;
         }
         message_byte_counter = 0;
         temp_ptr = (HUGE uint8_t *)DAQ_address_size.address ;
         for ( i = 0; i < DAQ_address_size.size; i++ )
         {
            destination_ptr[ message_byte_counter ] = *temp_ptr;

            message_byte_counter++;
            temp_ptr++;
         }

      }

      /*
       * Clear remaining message buffer, if any
       */
      while ( message_byte_counter < (NUMBER_OF_INST_COMMAND_BYTES - 1) )
      {
         destination_ptr[ message_byte_counter++ ] = CLEAR_DATA;
      }

      destination_ptr += (NUMBER_OF_INST_COMMAND_BYTES - 1);
   }
}

/*===========================================================================*\
 * Name:         CCP_Read_Resume_Data
 * Purpose:      This function will restore the DAQ resume data from NVM and
 *               then update session status logging.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
static void CCP_Read_Resume_Data( void )
{
#ifdef NUMBER_OF_RESUMEABLE_DAQ_LISTS
#if ( NUMBER_OF_RESUMEABLE_DAQ_LISTS > 0 )
   int i, j = 0; /* coding standard exception */

   for ( i = 0; i < NUMBER_OF_DAQ_LISTS; i++ )
   {
      if ( CCP_RESUME_SUPPORTED == CCP_DAQ_Configuration[ i ].resume_supported )
      {
         DAQ_List_Status[ i ].message_id                  = CCP_DAQ_Resume_Data_NVM[ j ].message_id;
         DAQ_List_Status[ i ].transmission_rate_prescaler = CCP_DAQ_Resume_Data_NVM[ j ].transmission_rate_prescaler;
         DAQ_List_Status[ i ].event_channel_number        = CCP_DAQ_Resume_Data_NVM[ j ].event_channel_number;
         DAQ_List_Status[ i ].first_PID                   = CCP_DAQ_Resume_Data_NVM[ j ].first_PID;
         DAQ_List_Status[ i ].last_ODT_to_transmit        = CCP_DAQ_Resume_Data_NVM[ j ].last_ODT_to_transmit;
         DAQ_List_Status[ i ].started                     = CCP_DAQ_Resume_Data_NVM[ j ].started;
         DAQ_List_Status[ i ].prescaler_counter           = 1;
         
         j++;
      }
   }

   Update_Session_Status_LOGGING();
#endif
#endif
}

/*===========================================================================*\
 * Name:         Move_DAQ_Pointer
 * Purpose:      This function moves the DAQ list pointer reference to the
 *               location in the DAQ list designated by the input reference.
 * Parameters:   DAQ_Reference_T, DAQ reference
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
\*===========================================================================*/
static CCP_CRC_T Move_DAQ_Pointer( DAQ_Reference_T reference )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;

   if ( DAQ_Reference_Is_Valid( reference ) )
   {
      current_DAQ_ptr_reference = reference;
   }
   else
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Move_DAQ_Pointer_To_Next_ODT
 * Purpose:      This function moves the DAQ Pointer to the first element of
 *               the next ODT. The DAQ Pointer cannot move to another DAQ
 *               list or beyond the end of the current DAQ list using this
 *               command.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
\*===========================================================================*/
static CCP_CRC_T Move_DAQ_Pointer_To_Next_ODT( void )
{
   DAQ_Reference_T temp_DAQ_ptr_reference;
   CCP_CRC_T       error_to_return = CRC_NO_ERROR;

   temp_DAQ_ptr_reference = current_DAQ_ptr_reference;

   if ( DAQ_Reference_Is_Last_ODT( temp_DAQ_ptr_reference ) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }
   else
   {
      temp_DAQ_ptr_reference.ODT_number++;
      temp_DAQ_ptr_reference.element_number = FIRST_ODT_ELEMENT_NUMBER;
      Move_DAQ_Pointer( temp_DAQ_ptr_reference );
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Move_DAQ_Pointer_Forward_One_Element
 * Purpose:      This function moves the DAQ pointer forward one element. It
 *               also checks to ensure there is enough room in the ODT table
 *               and that the DAQ pointer is not currently at the end of a
 *               DAQ.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
\*===========================================================================*/
static CCP_CRC_T Move_DAQ_Pointer_Forward_One_Element( void )
{
   DAQ_Reference_T temp_DAQ_ptr_reference = current_DAQ_ptr_reference;
   CCP_CRC_T       error_to_return;

   if ( DAQ_Reference_Is_Last_Element_Of_ODT( temp_DAQ_ptr_reference ) )
   {
      if ( DAQ_Reference_Is_Last_ODT( temp_DAQ_ptr_reference ) )
      {
         /* do nothing, element is last element of DAQ list, do not post increment, no error */
         error_to_return = CRC_NO_ERROR;
      }
      else
      {
         /* move to next ODT */
         error_to_return = Move_DAQ_Pointer_To_Next_ODT();
      }
   }
   else
   {
      /* move to next element */
      temp_DAQ_ptr_reference.element_number++;
      error_to_return = Move_DAQ_Pointer( temp_DAQ_ptr_reference );
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Number_Of_Bytes_Left_In_ODT
 * Purpose:      This function returns the room left in the ODT table that
 *               the DAQ Pointer is currently pointing to. The total size of
 *               elements that can be represented in an ODT table is equal
 *               to Number_Of_Elements_In_ODT. For sake of example, assume
 *               this is 7 bytes. This function totals the number of bytes
 *               that is represented by the element recorded in the table and
 *               subtracts that number from seven.
 *
 *               For instance, assume a table contains two entries, one that
 *               is a uint8_t and another that is a uint16_t, the total data
 *               being represented in the table is three bytes and this
 *               function would return a value of four indicating that
 *               there were four bytes of 'room' left in the table.
 *
 * Parameters:   None
 * Return Value: uint8_t, This function returns a value between 0 and
 *                        Number_Of_Elements_In_ODT.
 * Design Info:  None
\*===========================================================================*/
static uint8_t Number_Of_Bytes_Left_In_ODT( void )
{
   DAQ_Reference_T reference                = current_DAQ_ptr_reference;
   uint8_t         element_counter          = FIRST_ODT_ELEMENT_NUMBER;
   uint8_t         count_of_bytes_allocated = 0;
   ODT_Element_T   DAQ_address_size;

   Read_DAQ_Address_Size(  reference.DAQ_number
                        , reference.ODT_number
                        , element_counter
                        , &DAQ_address_size );

   while ( ( NO_SIZE_XFER != DAQ_address_size.size )
           && ( Number_Of_Elements_In_ODT( reference.DAQ_number ) != element_counter ) )
   {
      Read_DAQ_Address_Size(  reference.DAQ_number
                           , reference.ODT_number
                           , element_counter
                           , &DAQ_address_size );
      
      count_of_bytes_allocated += DAQ_address_size.size ;
      element_counter++;
   }

   return ( Number_Of_Elements_In_ODT( reference.DAQ_number ) - count_of_bytes_allocated );
}

/*===========================================================================*\
 * Name:         Initialize_DAQ_List_Status
 * Purpose:      This function clears all members of the DAQ_List_Status
 *               structure for the specified DAQ List.
 * Parameters:   uint8_t, Number of DAQ List to clear.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Resetting these values will stop the DAQ list transmission.
\*===========================================================================*/
static void Initialize_DAQ_List_Status( uint8_t DAQ_number )
{
   HUGE uint8_t*     ptr;
   unsigned int i; /* coding standard exception */

   ptr = (HUGE uint8_t*)&DAQ_List_Status[ DAQ_number ];

   for ( i = 0; i < sizeof( DAQ_List_Status_T ); i++ )
   {
      *ptr = 0;

      ptr++;
   }
}

/*===========================================================================*\
 * Name:         Reset_DAQ_List_Status
 * Purpose:      This function resets all members of the DAQ_List_Status
 *               structure for the specified DAQ List with the following values
 *               preserved (init parameters from GET_DAQ_SIZE message):
 *                  message_id
 *                  first_PID
 * Parameters:   uint8_t, Number of DAQ List to clear.
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  Resetting these values will stop the DAQ list transmission.
\*===========================================================================*/
static void Reset_DAQ_List_Status( uint8_t DAQ_number )
{
   uint32_t message_id; /* from GET_DAQ_SIZE message */
   uint8_t  first_PID;  /* from GET_DAQ_SIZE message */

   message_id = DAQ_List_Status[ DAQ_number ].message_id;
   first_PID  = DAQ_List_Status[ DAQ_number ].first_PID;
   Initialize_DAQ_List_Status( DAQ_number );
   DAQ_List_Status[ DAQ_number ].message_id = message_id;
   DAQ_List_Status[ DAQ_number ].first_PID  = first_PID;
}

/*===========================================================================*\
 * Name:         Transmit_Next_ODT
 * Purpose:      This function outputs the next ODT of the input DAQ list on
 *               the communication bus.
 * Parameters:   uint8_t, Number of DAQ list to process
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
\*===========================================================================*/
static CCP_CRC_T Transmit_Next_ODT( uint8_t DAQ_number )
{
#ifndef FORCE_MSB_FIRST_DAQ_PROCESSING
#error
#endif

   ODT_Element_T  DAQ_address_size ;

#if ( FORCE_MSB_FIRST_DAQ_PROCESSING )
   uint32_t temp_value;
#else
   HUGE uint8_t *temp_ptr;
   int      i; /* coding standard exception */
#endif
   uint8_t  message_counter;
   uint8_t  element_counter;
   uint8_t  message_byte_counter;

   /*
    * Set DAQ CAN ID
    */
   ccp.message_id = DAQ_List_Status[ DAQ_number ].message_id;

   /*
    * Retrieve current_ODT
    */
   message_counter = DAQ_List_Status[ DAQ_number ].current_ODT;

   if ( message_counter <= DAQ_List_Status[ DAQ_number ].last_ODT_to_transmit )
   {
      /*
       * Fill in ODT
       */
      ccp.DTM_data.command_return_message.packet_ID =
         DAQ_List_Status[ DAQ_number ].first_PID + message_counter;

      /*
       * The ODTs are transmitted starting with byte number 1, since byte
       * number 0 is used for identification.
       */
      message_byte_counter = 1;

      if ( CCP_DAQ_Configuration[ DAQ_number ].data_buffer_ptr )
      {
         /* Coherent DAQ list processing */
         HUGE uint8_t* temp_data_ptr = CCP_DAQ_Configuration[ DAQ_number ].data_buffer_ptr + (7*message_counter);

         while ( message_byte_counter < NUMBER_OF_INST_COMMAND_BYTES )
         {
            ccp.DTM_data.data_byte[ message_byte_counter ] = *temp_data_ptr;

            message_byte_counter++;
            temp_data_ptr++;
         }
      }
      else
      {
         /* non-coherent DAQ list processing */
         for( element_counter = FIRST_ODT_ELEMENT_NUMBER;
              element_counter < Number_Of_Elements_In_ODT( DAQ_number );
              element_counter++ )
         {
            Read_DAQ_Address_Size( DAQ_number, message_counter, element_counter, &DAQ_address_size ); 

            if ( NO_SIZE_XFER == DAQ_address_size.size )
            {
               break;
            }
            message_byte_counter = 0;
            temp_ptr = (HUGE uint8_t *)DAQ_address_size.address ;
            for ( i = 0; i < DAQ_address_size.size; i++ )
            {
               ccp.DTM_data.data_byte[ message_byte_counter ] = *temp_ptr;

               message_byte_counter++;
               temp_ptr++;
            }
   
         }
   
         /*
          * Clear remaining message buffer, if any
          */
         while ( message_byte_counter < NUMBER_OF_INST_COMMAND_BYTES )
         {
            ccp.DTM_data.data_byte[ message_byte_counter++ ] = CLEAR_DATA;
         }
      }

      /*
       * Transmit message buffer on appropriate CAN ID (DAQ_Message_ID( DAQ_number ))
       */
      CCP_Send_DTM();

      /*
       * Point to next ODT
       */
      DAQ_List_Status[ DAQ_number ].current_ODT++;

      if ( DAQ_List_Status[ DAQ_number ].current_ODT >
           DAQ_List_Status[ DAQ_number ].last_ODT_to_transmit )
      {
         DAQ_List_Status[ DAQ_number ].sending = false;
      }
   }

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Global Functions
\*===========================================================================*/

/*===========================================================================*\
 * Name:         CCP_Trigger_Event_Channel
 * Purpose:      Initiate the transmission of a DAQ list.
 * Parameters:   uint8_t, Event channel number
 * Return Value: None
 * Design Info:  Transmit any started DAQ lists on specified event channel.
\*===========================================================================*/
void CCP_Trigger_Event_Channel( uint8_t event_channel_number )
{
   uint8_t DAQ_number = FIRST_DAQ_NUMBER;

   if ( (uint8_t)SS_LOGGING & ccp.session_status )
   {
      while ( DAQ_Number_Is_Valid(DAQ_number) )
      {
         if ( DAQ_List_Status[ DAQ_number ].started &&
              (event_channel_number == DAQ_List_Status[ DAQ_number ].event_channel_number) )
         {
            if ( DAQ_List_Status[ DAQ_number ].sending )
            {
               /*
                * WARNING - DAQ list overrun
                * Increment overrun counter for DAQ list
                */
               DAQ_List_Status[ DAQ_number ].trigger_overrun_counter++;
            }
            else /* Do not restart DAQ lists with overruns, allow to complete */
            {
               if ( 1 == DAQ_List_Status[ DAQ_number ].prescaler_counter )
               {
                  DAQ_List_Status[ DAQ_number ].current_ODT       = FIRST_ODT_NUMBER;
                  DAQ_List_Status[ DAQ_number ].sending           = true;
                  DAQ_List_Status[ DAQ_number ].prescaler_counter =
                     DAQ_List_Status[ DAQ_number ].transmission_rate_prescaler;
               
                  if ( CCP_DAQ_Configuration[ DAQ_number ].data_buffer_ptr )
                  {
                     /* If DAQ list is coherent, all data must be read now */
                     DAQ_Transfer_Coherent_List( DAQ_number );
                  }
                  
                  /*
                   * Start transmitting by sending first DAQ packet
                   */
                  Transmit_Next_DAQ_Packet();
               }
               else
               {
                  DAQ_List_Status[ DAQ_number ].prescaler_counter--;
               }
            }
         }

         DAQ_number++;
      }
   }
}

/*===========================================================================*\
 * Name:         Transmit_Next_DAQ_Packet
 * Purpose:      Transmit next available DAQ ODT.
 * Parameters:   None
 * Return Value: None
 * Design Info:  This function will transmit the next ODT giving priority to
 *               the lower numbered DAQ list.
\*===========================================================================*/
void Transmit_Next_DAQ_Packet( void )
{
uint8_t DAQ_number = FIRST_DAQ_NUMBER;
bool    done       = false;

   while ( !done && DAQ_Number_Is_Valid(DAQ_number) )
   {
      if ( DAQ_List_Status[ DAQ_number ].sending )
      {
         Transmit_Next_ODT( DAQ_number );
         done = true;
      }
      else
      {
         DAQ_number++;
      }
   }
}

/*===========================================================================*\
 * Name:         CCP_Get_DAQ_Size
 * Purpose:      Returns the size of the specified DAQ list as the number of
 *               available ObjectDescriptorTables (ODTs) and clears the
 *               current list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  If the specified list number is not available, size = 0
 *               should be returned. The DAQ list is initialized and data
 *               acquisition by this list is stopped.
 *
 *               An individual CAN Identifier may be assigned to a DAQ list
 *               to configure multi ECU data acquisition. This feature is
 *               optional. If the given identifier isn't possible, an error
 *               code is returned. 29 bit CAN identifiers are marked by the
 *               most significant bit set.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = GET_DAQ_SIZE 0x14
 * 1           byte           Command Counter = CTR
 * 2           byte           DAQ list number (0,1,...)
 * 3           byte           don't care
 * 4..7        unsigned long  CAN Identifier of DTO dedicated to list number
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3           byte           DAQ list size (= number of ODTs in this list)
 * 4           byte           First PID of DAQ list
 * 5..7        bytes          don't care
 *
 * The PID of a specific ODT of a DAQ list is determined by:
 * PID = First PID of DAQ list + ODT number
 *
 * NOTE: Verification of CAN ID and privelege level checking are not yet
 *       implemented (JWD)
\*===========================================================================*/
CCP_CRC_T CCP_Get_DAQ_Size( void )
{
   DAQ_Reference_T new_DAQ_list;
   CCP_CRC_T       error_to_return;
   
   /*
    * Build a new DAQ list reference
    */
   new_DAQ_list.DAQ_number     = ccp.command_data.DAQ.number;
   new_DAQ_list.ODT_number     = FIRST_ODT_NUMBER;
   new_DAQ_list.element_number = FIRST_ODT_ELEMENT_NUMBER;
   
   if ( DAQ_Reference_Is_Valid( new_DAQ_list ) )
   {
      /*
       * Initialize DAQ list and stop data acquisition
       */
      Clear_DAQ_List( new_DAQ_list.DAQ_number );

      ccp.response_data.DAQ.number_of_ODTs = Number_Of_ODTs_In_DAQ_List( new_DAQ_list.DAQ_number );

      /*
       * Compute first PID of DAQ list
       */
      DAQ_List_Status[ new_DAQ_list.DAQ_number ].first_PID =
         ccp.response_data.DAQ.PID_or_element = DAQ_Starting_PID( new_DAQ_list.DAQ_number );

      error_to_return = Move_DAQ_Pointer( new_DAQ_list );

      /*
       * CAN Identifier of DTO dedicated to list number is stored
       * big endian
       */
      DAQ_List_Status[ new_DAQ_list.DAQ_number ].message_id =
         FIX_BIG_ENDIAN32( ccp.command_data.DTO.DTO_ID32 );
   }
   else
   {
      ccp.response_data.DAQ.number_of_ODTs = FIRST_ODT_NUMBER;
      error_to_return = CRC_NO_ERROR;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         CCP_Set_DAQ_Ptr
 * Purpose:      Initializes the DAQ list pointer for a subsequent write to
 *               a DAQ list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  None
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = SET_DAQ_PTR 0x15
 * 1           byte           Command Counter = CTR
 * 2           byte           DAQ list number (0,1,...)
 * 3           byte           Object Descriptor Table ODT number (0,1,...)
 * 4           byte           Element number within ODT (0,1,...)
 * 5..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
\*===========================================================================*/
CCP_CRC_T CCP_Set_DAQ_Ptr( void )
{
   DAQ_Reference_T reference;

   reference.DAQ_number     = ccp.command_data.DAQ.number;
   reference.ODT_number     = ccp.command_data.DAQ.number_of_ODTs;
   reference.element_number = ccp.command_data.DAQ.PID_or_element;

   return Move_DAQ_Pointer( reference );
}

/*===========================================================================*\
 * Name:         CCP_Write_DAQ
 * Purpose:      Writes one entry (description of single DAQ element) to a
 *               DAQ list defined by the DAQ list pointer (see SET_DAQ_PTR).
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The following DAQ element sizes are defined: 1 byte,
 *               2 bytes (type word), 4 bytes (type long/Float).
 *
 *               An ECU may not support individual address extensions for
 *               each element and 2 or 4 byte element sizes. It is the
 *               responsibility of the slave device, that all bytes of a DAQ
 *               element are consistent upon transmission.
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = WRITE_DAQ 0x16
 * 1           byte           Command Counter = CTR
 * 2           byte           Size of DAQ element in bytes {1,2,4}
 * 3           byte           Address extension of DAQ element
 * 4..7        bytes          Address of DAQ element
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
 *
 * This function writes the input address and size of the element tha the
 * DAQ pointer is currently pointing to.
 *
 * This function writes an ODT Element Type (address and size) to the ODT
 * Element that is pointed to by the DAQ Pointer. This function post
 * increments the DAQ pointer to the next element in the ODT.
 *
 * This function first checks to ensure that there is enough 'room' left
 * in the ODT before it writes the input variable. If there is not enough
 * room in the ODT that the DAQ pointer is currently pointing to it moves to
 * the beginning of the next ODT and writes the input variable there.
\*===========================================================================*/
CCP_CRC_T CCP_Write_DAQ( void )
{
   CCP_CRC_T     error_to_return = CRC_NO_ERROR;
   ODT_Element_T new_ODT_element;

   temp_address1  = FIX_BIG_ENDIAN31( ccp.command_data.address.address_location32 );
   new_ODT_element.size = ccp.command_data.address.MTA_or_size;

   /*
    * Validate element size
    */
   if ( (new_ODT_element.size != BYTE8_XFER ) &&
        (new_ODT_element.size != WORD16_XFER) &&
        (new_ODT_element.size != LONG32_XFER) )
   {
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   /*
    * Validate and add ODT element
    */
   if ( CRC_NO_ERROR == error_to_return )
   {
      new_ODT_element.address = (HUGE uint8_t *)temp_address1;

      if ( OTHER == CCP_Location_Of_Address_User( new_ODT_element.address ) )
      {
         error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
      }
      else if ( new_ODT_element.size > Number_Of_Bytes_Left_In_ODT() )
      {
         error_to_return = Move_DAQ_Pointer_To_Next_ODT();
      }
      else
      {
         /* do nothing */
      }

      if ( CRC_NO_ERROR == error_to_return )
      {
         Write_to_Current_DAQ_Pointer( new_ODT_element );
         error_to_return = Move_DAQ_Pointer_Forward_One_Element();
      }
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Start_Stop
 * Purpose:      This command is used to start or to stop the data
 *               acquisition or to prepare a synchronized start of the
 *               specified DAQ list.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:  The Last ODT number specifies which ODTs (From 0 to Last ODT
 *               number) of this DAQ list should be transmitted. The Event
 *               Channel No. specifies the generic signal source that
 *               effectively determines the data transmission timing. To
 *               allow reduction of the desired transmission rate, a
 *               prescaler may be applied to the Event Channel. The prescaler
 *               value factor must be greater than or equal to 1. The mode
 *               parameter is defined as follows: 0x00 stops specified DAQ
 *               list, 0x01 starts specified DAQ list, 0x02 prepares DAQ list
 *               for synchronized start.
 *               The start/stop mode parameter = 0x02 (prepare to start data
 *               transmission) configures the DAQ list with the provided
 *               parameters but does not start the data acquisition of the
 *               specified list. This parameter is used for a synchronized
 *               start of all configured DAQ lists. In case the slave device
 *               is not capable of performing the synchronized start of the
 *               data acquisition, the slave device may then start data
 *               transmission if this parameter is true (not zero).
 *               The ECU specific properties of event channels and DAQ lists
 *               may be described in the slave device description (ASAP2).
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = START_STOP 0x06
 * 1           byte           Command Counter = CTR
 * 2           byte           Mode: start/stop/prepare data transmission
 * 3           byte           DAQ list number
 * 4           byte           Last ODT number
 * 5           byte           Event Channel No.
 * 6,7         word           Transmission rate prescaler
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
 *
\*===========================================================================*/
CCP_CRC_T CCP_Start_Stop( void )
{
   CCP_CRC_T error_to_return = CRC_NO_ERROR;
   uint8_t   DAQ_number      = ccp.command_data.start_stop.DAQ_list_number;
   uint16_t  tmp_prescaler;

   if ( DAQ_Number_Is_Valid( DAQ_number ) )
   {
      Reset_DAQ_List_Status( DAQ_number );

      if ( (START              == ccp.command_data.start_stop.mode) ||
           (START_SYNCHRONIZED == ccp.command_data.start_stop.mode) )
      {
         /*
          * Configure last ODT number
          */
         if ( ccp.command_data.start_stop.last_ODT_number >=
              Number_Of_ODTs_In_DAQ_List( DAQ_number ) )
         {
            /* last ODT parameter is out of range */
            error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
         }
         else
         {
            DAQ_List_Status[ DAQ_number ].last_ODT_to_transmit =
               ccp.command_data.start_stop.last_ODT_number;
         }

         if ( CRC_NO_ERROR == error_to_return )
         {
            /*
             * Configure event channel number
             */
            DAQ_List_Status[ DAQ_number ].event_channel_number =
               ccp.command_data.start_stop.event_channel_number;

            /*
             * Configure transmission rate prescaler
             */
            tmp_prescaler = FIX_BIG_ENDIAN16( ccp.command_data.start_stop.transmission_rate_prescaler16 );
            if ( 1 > tmp_prescaler )
            {
               /* WARNING - value is out of range */
               DAQ_List_Status[ DAQ_number ].transmission_rate_prescaler = 1;
            }
            else
            {
               DAQ_List_Status[ DAQ_number ].transmission_rate_prescaler =
                  tmp_prescaler;
            }

            /*
             * Initialize prescaler counter
             */
            DAQ_List_Status[ DAQ_number ].prescaler_counter =
               DAQ_List_Status[ DAQ_number ].transmission_rate_prescaler;

            /*
             * Start DAQ and update session status if requested
             */
            if ( START == ccp.command_data.start_stop.mode )
            {
               DAQ_List_Status[ DAQ_number ].started = true;
            }
            else
            {
               DAQ_List_Status[ DAQ_number ].start_synchronized = true;
            }
         }
      }
      else /* STOP */
      {
         DAQ_List_Status[ DAQ_number ].started = false;
      }

      Update_Session_Status_LOGGING();
   }
   else
   {
      /* DAQ number is invalid */
      error_to_return = CRC_PARAMETER_OUT_OF_RANGE;
   }

   return error_to_return;
}

/*===========================================================================*\
 * Name:         Start_Stop_All
 * Purpose:      This command is used to start the periodic transmission of
 *               all DAQ lists configured with the previously sent START_STOP
 *               command (start/stop mode = 2) as "prepared to start" in a
 *               synchronized manner.  The command is used to stop the
 *               periodic transmission of all DAQ lists, including those not
 *               started synchronized.
 * Parameters:   None
 * Return Value: CCP_CRC_T, command response code
 * Design Info:
 *
 * Structure of data in CRO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Command Code = START_STOP_ALL 0x08
 * 1           byte           Command Counter = CTR
 * 2           byte           0x00 stops, 0x01 starts data transmission
 * 3..7        bytes          don't care
 *
 * Structure of data in DTO:
 * Position    Type           Description
 * =========== ============== ==============================================
 * 0           byte           Packet ID: 0xFF
 * 1           byte           Command Return Code
 * 2           byte           Command Counter = CTR
 * 3..7        bytes          don't care
 *
\*===========================================================================*/
CCP_CRC_T CCP_Start_Stop_All( void )
{
   uint8_t DAQ_number = FIRST_DAQ_NUMBER;

   if ( STOP != ccp.command_data.start_stop.mode )
   {
      while ( DAQ_Number_Is_Valid(DAQ_number) )
      {
         if ( DAQ_List_Status[ DAQ_number ].start_synchronized )
         {
            DAQ_List_Status[ DAQ_number ].started = true;
         }
         DAQ_number++;
      }
   }
   else /* STOP */
   {
      while ( DAQ_Number_Is_Valid(DAQ_number) )
      {
         DAQ_List_Status[ DAQ_number ].started = false;
         DAQ_number++;
      }
   }

   Update_Session_Status_LOGGING();

   return CRC_NO_ERROR;
}

/*===========================================================================*\
 * Name:         CCP_Store_Resume_Data
 * Purpose:      This command will copy the resume data to the user supplied
 *               NVM buffer CCP_DAQ_Resume_Data_NVM.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void CCP_Store_Resume_Data( void )
{
#ifdef NUMBER_OF_RESUMEABLE_DAQ_LISTS
#if ( NUMBER_OF_RESUMEABLE_DAQ_LISTS > 0 )
   int i, j = 0; /* coding standard exception */

   for ( i = 0; i < NUMBER_OF_DAQ_LISTS; i++ )
   {
      if ( CCP_RESUME_SUPPORTED == CCP_DAQ_Configuration[ i ].resume_supported )
      {
         CCP_DAQ_Resume_Data_NVM[ j ].message_id                  = DAQ_List_Status[ i ].message_id;
         CCP_DAQ_Resume_Data_NVM[ j ].transmission_rate_prescaler = DAQ_List_Status[ i ].transmission_rate_prescaler;
         CCP_DAQ_Resume_Data_NVM[ j ].event_channel_number        = DAQ_List_Status[ i ].event_channel_number;
         CCP_DAQ_Resume_Data_NVM[ j ].first_PID                   = DAQ_List_Status[ i ].first_PID;
         CCP_DAQ_Resume_Data_NVM[ j ].last_ODT_to_transmit        = DAQ_List_Status[ i ].last_ODT_to_transmit;
         CCP_DAQ_Resume_Data_NVM[ j ].started                     = DAQ_List_Status[ i ].started;

         j++;
      }
   }
#endif
#endif
}

/*===========================================================================*\
 * Name:         CCP_Initialize_Resume_Data
 * Purpose:      This command will initialize the user supplied NVM buffer
 *               CCP_DAQ_Resume_Data_NVM.
 * Parameters:   None
 * Return Value: None
 * Design Info:  This function should be called in the event of a NVM failure.
\*===========================================================================*/
void CCP_Initialize_Resume_Data( void )
{
#ifdef NUMBER_OF_RESUMEABLE_DAQ_LISTS
#if ( NUMBER_OF_RESUMEABLE_DAQ_LISTS > 0 )
   HUGE uint8_t*     ptr;
   unsigned int i; /* coding standard exception */

   ptr = (HUGE uint8_t *)&CCP_DAQ_Resume_Data_NVM[ 0 ];

   for ( i = 0; i < (sizeof( CCP_DAQ_Resume_Data_NVM_T ) * NUMBER_OF_RESUMEABLE_DAQ_LISTS); i++ )
   {
      *ptr = 0;

      ptr++;
   }
#endif
#endif
}

/*===========================================================================*\
 * Name:         Initialize_DAQ_Parameters
 * Purpose:      This function initializes all of the control parameters for
 *               the DAQ lists.
 * Parameters:   None
 * Return Value: None
 * Design Info:  None
\*===========================================================================*/
void Initialize_DAQ_Parameters( void )
{
   uint8_t DAQ_number = FIRST_DAQ_NUMBER;

   while ( DAQ_Number_Is_Valid(DAQ_number) )
   {
      Initialize_DAQ_List_Status( DAQ_number );
      DAQ_number++;
   }

   CCP_Read_Resume_Data();
}
