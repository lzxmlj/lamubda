#ifndef DD_CAN_H
#define DD_CAN_H

/*===========================================================================*/
/* FILE: dd_can.h                                                            */
/*===========================================================================*/
/* COPYRIGHT 2004, DELPHI ELECTRONICS & SAFETY SYSTEMS, All Rights reserved  */
/* Delphi Confidential                                                       */
/*===========================================================================*/
/* %name:            dd_can.h %                                              */
/*                                                                           */
/* created_by:       rz65p6                                                  */
/*                                                                           */
/* date_created:     26 April 2004                                           */
/*                                                                           */
/* %derived_by:      wzf9ks %                                                */
/*                                                                           */
/* %date_modified:   Wed Jan 11 15:20:16 2012 %                                                       */
/*                                                                           */
/* %version:         4 %                                                     */
/*                                                                           */
/*===========================================================================*/
/* DESCRIPTION:                                                              */
/*   This file provides the input output functionalities for all the CAN     */
/*   Functinality.                                                           */
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
#include "stm32f37x.h"
#include "stdint.h"

/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/
#include "io_can_config.h"


/*===========================================================================*\
 * Local Preprocessor #define MACROS
\*===========================================================================*/
/* user update                 */
#define CAN_BAUD_RATE                    500000UL
#define CAN1_BAUD_RATE_PRESCALER_VALUE   (CLOCK_OSC_HZ / CAN_BAUD_RATE / (CAN1_TSEG1_VALUE + CAN1_TSEG2_VALUE +1))	//G.H @ Mar. 24th, 08' 

/* user must specify synchronization jump width, */
/* see motorola hardware reference for details.  */
#define SET_1_TIME_QUANTUM          0
#define SET_2_TIME_QUANTA           0x40
#define SET_3_TIME_QUANTA           0x80
#define SET_4_TIME_QUANTA           0xc0
#define CAN1_SJW_M                  SET_3_TIME_QUANTA

/* user must specify number of samples per bit, */
/* see motorola hardware reference for details. */
#define SET_1_SAMPLE_PER_BIT        0
#define SET_3_SAMPLES_PER_BIT       0x80
#define SAMPLE_M                    SET_1_SAMPLE_PER_BIT

#define CAN1_TSEG1_VALUE            12
#define CAN1_TSEG2_VALUE            3

#define CAN_ACCEPTANCE_MASK1        0xffff
#define CAN_CARE_ABOUT_BITS1        0xffff
#define CAN_ACCEPTANCE_MASK2        0xffbf
#define CAN_CARE_ABOUT_BITS2        0xffbf
#define CAN_ACCEPTANCE_MASK3        0xfffb
#define CAN_CARE_ABOUT_BITS3        0xfffb
#define CAN_ACCEPTANCE_MASK4        0xffff
#define CAN_CARE_ABOUT_BITS4        0xfff7


#define MESSAGE_ID_SHIFT              5
#define UNUSED_MASK_BITS              7


/* C1MCR0 Register bits definition */
#define SFTRES_M           ((uint8_t)0x01)
#define SLPRQ_M            ((uint8_t)0x02)
#define SLPAK_M            ((uint8_t)0x04)
#define TLNKEN_M           ((uint8_t)0x08)
#define SYNCH_M            ((uint8_t)0x10)
#define CSWAI_M            ((uint8_t)0x20)

/* C1MCR1 Register bits definition */
#define CLKSRC_M          ((uint8_t)0x01)
#define WUPM_M            ((uint8_t)0x02)
#define LOOPB_M           ((uint8_t)0x04)

/* C1BTR0 Register bits definition */
#define BRP0_M            ((uint8_t)0x01)
#define BRP1_M            ((uint8_t)0x02)
#define BRP2_M            ((uint8_t)0x04)
#define BRP3_M            ((uint8_t)0x08)
#define BRP4_M            ((uint8_t)0x10)
#define BRP5_M            ((uint8_t)0x20)
#define SJW0_M            ((uint8_t)0x40)
#define SJW1_M            ((uint8_t)0x80)

/* C1BTR1 Register bits definition */
#define TSEG10_M          ((uint8_t)0x01)
#define TSEG11_M          ((uint8_t)0x02)
#define TSEG12_M          ((uint8_t)0x04)
#define TSEG13_M          ((uint8_t)0x08)
#define TSEG20_M          ((uint8_t)0x10)
#define TSEG21_M          ((uint8_t)0x20)
#define TSEG22_M          ((uint8_t)0x40)
#define SAMP_M            ((uint8_t)0x80)


/* C1RFLG Register bits definition */
#define RXF_M             ((uint8_t)0x01)
#define OVRIF_M           ((uint8_t)0x02)
#define TSTAT_M           ((uint8_t)0x0C)
#define RSTAT_M           ((uint8_t)0x30)
#define CSCIF_M           ((uint8_t)0x40)
#define WUPIF_M           ((uint8_t)0x80)

/* C1RIER Register bits definition */
#define RXFIE_M          ((uint8_t)0x01)
#define OVRIE_M          ((uint8_t)0x02)
#define TSTATE0_M        ((uint8_t)0x04)
#define TSTATE1_M        ((uint8_t)0x08)
#define RSTATE0_M        ((uint8_t)0x10)
#define RSTATE1_M        ((uint8_t)0x20)
#define CSCIE_M          ((uint8_t)0x40)
#define WUPIE_M          ((uint8_t)0x80)

/* C1TFLG Register bits definition */
#define TXE0_M           ((uint8_t)0x01)
#define TXE1_M           ((uint8_t)0x02)
#define TXE2_M           ((uint8_t)0x04)
#define ABTAK0_M         ((uint8_t)0x10)
#define ABTAK1_M         ((uint8_t)0x20)
#define ABTAK2_M         ((uint8_t)0x40)

/* C1TCR Register bits definition */
#define TXEIE0_M         ((uint8_t)0x01)
#define TXEIE1_M         ((uint8_t)0x02)
#define TXEIE2_M         ((uint8_t)0x04)
#define ABTRQ0_M         ((uint8_t)0x10)
#define ABTRQ1_M         ((uint8_t)0x20)
#define ABTRQ2_M         ((uint8_t)0x40)

/* C1IDAC Register bits definition */
#define IDHIT0_M         ((uint8_t)0x01)
#define IDHIT1_M         ((uint8_t)0x02)
#define IDHIT2_M         ((uint8_t)0x04)
#define IDAM0_M          ((uint8_t)0x10)
#define IDAM1_M          ((uint8_t)0x20)

/* PCTLCAN1 Register bits definition */
#define RDPCAN_M         ((uint8_t)0x01)
#define PUPCAN_M         ((uint8_t)0x02)


#define C1MCR0_INITIAL_VALUE ( 0x00| /* undefined bit value        */\
           CSWAI_M   | /* 1=CAN stops in wait mode.                */\
          !SYNCH_M   | /* 1=msCAN12 is syncd to can bus-READ ONLY  */\
          !TLNKEN_M  | /* 1=msCAN12 timer routed to ECT.           */\
          !SLPAK_M   | /* 1=msCAN12 is in sleep mode-READ ONLY     */\
          !SLPRQ_M   | /* 1=msCAN12 will go into sleep mode.       */\
           SFTRES_M  ) /* 1=Soft Reset. Must be set to write to C1BTR0 */

#define C1MCR1_INITIAL_VALUE ( 0x00| /* undefined bit value                 */\
          !LOOPB_M   | /* 1=Activate  loop back self test mode.            */\
          !WUPM_M    | /* 1=Apply Filter to protect from spurious wakeups. */\
          !CLKSRC_M  ) /* 0=EXTALi 1=ECLK                                  */

/* USER: Select one of the following for C1BTR0_INITIAL_VALUE
#define CAN1_SJW_1_TQ_CLOCK_CYCLE_M  0
#define CAN1_SJW_2_TQ_CLOCK_CYCLE_M  ( SJW0_M )
#define CAN1_SJW_3_TQ_CLOCK_CYCLE_M  ( SJW1_M )
#define CAN1_SJW_4_TQ_CLOCK_CYCLE_M  ( SJW1_M|SJW0_M )
*/

/* Note: This register can only be written to if the SFTRES_M is set       */
#define C1BTR0_INITIAL_VALUE   ( 0x00| /* undefined bit value          */\
            CAN1_SJW_M               | /* SJW Selection from above.    */\
           ( CAN1_BAUD_RATE_PRESCALER_VALUE - 1 ) )
                                           /* Prescaler value from above.  */

#define C1BTR1_INITIAL_VALUE  ( 0x00| /* undefined bit value                */\
           SAMPLE_M                 | /* 1= 3 samples per bit. 0= 1 sample  */\
          ( CAN1_TSEG2_VALUE-1)<<4  | /* TSEG1 mask from above define*/\
          ( CAN1_TSEG1_VALUE-1)     ) /* TSEG2 mask from above define*/

/* error handling has not yet been implemented */
#define C1RFLG_INITIAL_VALUE ( 0x00| /* undefined bit value          */\
          WUPIF_M    | /* Wakeup interrupt flag.                    */\
          CSCIF_M   | /* Receiver warning interrupt flag.          */\
          RSTAT1_M   | /* Transmitter warning interrupt flag.       */\
          RSTAT0_M   | /* Receiver error passive interrupt flag.    */\
          TSTAT1_M   | /* Transmitter error passive interrupt flag. */\
          BOFFIF_M   | /* Bus off interrupt flag.                   */\
          OVRIF_M    | /* Overrun interrupt flag.                   */\
          RXF_M      ) /* Receive buffer full flag.                 */


/* NOTE: SFTRES must be clear to modify this register. Setting SFTRES will
 * clear this register.
 */

#define C1RIER_INITIAL_VALUE ( 0x00| /* undefined bit value              */\
          !WUPIE_M    | /* 1=Enable wakeup interrupt.                    */\
            CSCIE_M   | /* 1=Enable receiver warning interrupt.          */\
          !RSTATE1_M   | /* 1=Enable Transmitter warning interrupt.       */\
            RSTATE0_M   | /* 1=Enable Receiver error passive interrupt.    */\
          !TSTATE1_M   | /* 1=Enable Transmitter error passive interrupt. */\
           TSTATE0_M   | /* 1=Enable Bus off interrupt.                   */\
           OVRIE_M    | /* 1=Enable Overrun interrupt.                   */\
           RXFIE_M    ) /* 1=Enable Receive buffer full interrupt.       */

/* SPECIAL TURN OFF ERROR INTERRUPTS FOR EE_EXEC since there's no
 * error vector to jump to. Duh. */
#define C1RIER_EE_INITIAL_VALUE ( 0x00| /* undefined bit value              */\
          !WUPIE_M    | /* 1=Enable wakeup interrupt.                    */\
          !CSCIE_M   | /* 1=Enable receiver warning interrupt.          */\
          !RSTATE1_M   | /* 1=Enable Transmitter warning interrupt.       */\
          !RSTATE0_M   | /* 1=Enable Receiver error passive interrupt.    */\
          !TSTATE1_M   | /* 1=Enable Transmitter error passive interrupt. */\
           TSTATE0_M   | /* 1=Enable Bus off interrupt.                   */\
           OVRIE_M    | /* 1=Enable Overrun interrupt.                   */\
           RXFIE_M    ) /* 1=Enable Receive buffer full interrupt.       */

/* Note: Clear flags by writing 1 to the bit position. 0 has no effect.
 * Note: The CPU must "clear" the TXEx flag after the message has been set-up
 * in the transmit buffer. This will schedule the message for transmission.
 */
#define C1TFLG_INITIAL_VALUE ( 0x00| /* undefined bit value */\
          !ABTAK2_M   | /* 1=Identifies that message was aborted successful*/\
          !ABTAK1_M   | /* 1=Identifies that message was aborted successful*/\
          !ABTAK0_M   | /* 1=Identifies that message was aborted successful*/\
           TXE2_M     | /* 1=Transmit buffer empty. */\
           TXE1_M     | /* 1=Transmit buffer empty.*/\
           TXE0_M     ) /* 1=Transmit buffer empty.*/

/* SFTRST MUST=0 to change this register! */

#define C1TCR_INITIAL_VALUE ( 0x00| /* undefined bit value */\
           !ABTRQ2_M   | /* 1=Request that CPU abort message scheduled */\
           !ABTRQ1_M   | /* 1=Request that CPU abort message scheduled */\
           !ABTRQ0_M   | /* 1=Request that CPU abort message scheduled */\
           !TXEIE2_M   | /* 1=Generate an interrupt when transmitter empty.*/\
           !TXEIE1_M   | /* 1=Generate an interrupt when transmitter empty.*/\
           !TXEIE0_M   ) /* 1=Generate an interrupt when transmitter empty.*/

/* USER: Pick one of the following for C1IDAC_INITIAL_VALUE. */
#define CAN1_TWO_32_BIT_ACCEPTANCE_FILTERS_M  ( 0 )
#define CAN1_FOUR_16_BIT_ACCEPTANCE_FILTERS_M  ( IDAM0_M )
#define CAN1_EIGHT_8_BIT_ACCEPTANCE_FILTERS_M  ( IDAM1_M )
#define CAN1_FILTER_CLOSED_M  ( IDAM0_M | IDAM1_M )

/* filters are not yet set to filter messages */
#define C1IDAC_INITIAL_VALUE ( 0x00| /* undefined bit value */\
           CAN1_FOUR_16_BIT_ACCEPTANCE_FILTERS_M  | /* acceptance mode*/\
           !IDHIT2_M   | /* Filter Hit Bit 2. Indicates Filter 0-7.   */\
           !IDHIT1_M   | /* Filter Hit Bit 1.  */\
           !IDHIT0_M   ) /* Filter Hit Bit 0.  */

/* The C0IDARx registers can only be written if the SFTRES bit in the
 * control register is set. */

#define CAN1_BANK1_ACCEPTENCE_ID CAN_ACCEPTANCE_MASK1  /* 16 BITS */
#define CAN1_BANK2_ACCEPTENCE_ID CAN_ACCEPTANCE_MASK2  /* 16 BITS */
#define CAN1_BANK3_ACCEPTENCE_ID CAN_ACCEPTANCE_MASK3  /* 16 BITS */
#define CAN1_BANK4_ACCEPTENCE_ID CAN_ACCEPTANCE_MASK4  /* 16 BITS */

/* Note: if standard identifiers are used, it is required to program the mask
 * register's last 3 bits to don't care. */

/* bits: 0=Match the corresponding id bits. 1=don't care. */
#define CAN1_BANK1_MASK_REGISTER  CAN_CARE_ABOUT_BITS1
#define CAN1_BANK2_MASK_REGISTER  CAN_CARE_ABOUT_BITS2
#define CAN1_BANK3_MASK_REGISTER  CAN_CARE_ABOUT_BITS3
#define CAN1_BANK4_MASK_REGISTER  CAN_CARE_ABOUT_BITS4

#define BUFFER_EMPTY 0xffff
#define NUMBER_OF_CAN_TRANSMIT_BUFFERS 3

#define PCTLCAN1_INITIAL_VALUE ( 0x00| /* undefined bit value */\
           RDPCAN_M   | /*1=Pull up enabled for CAN port. */\
          !PUPCAN_M   ) /*1=Reduced drive enabled for CAN port. */

/* Pointer to the beginning of the can 0 registers. */
#define CAN0_RX_PTR        ( ( message_buffer_T * ) & CAN0RXIDR0)
#define CAN1_RX_PTR        ( ( message_buffer_T * ) & CAN1RXIDR0 )
#define CAN4_RX_PTR        ( ( message_buffer_T*  ) & CAN4RXIDR0 )

/* Pointer to the beginning of the can 0 data area. */
#define CAN0_RX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN0RXIDR0)->data[ 0 ] )
#define CAN1_RX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN1RXIDR0 )->data[ 0 ] )
#define CAN4_RX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN4RXIDR0 )->data[ 0 ] )

/* Pointer to the beginning of the can 0 registers. */
#define CAN0_TX_PTR        ( ( message_buffer_T * ) & CAN0TXIDR0 )
#define CAN4_TX_PTR        ( ( message_buffer_T * ) & CAN4TXIDR0 )
#define CAN1_TX_PTR        ( ( message_buffer_T * ) & CAN1TXIDR0 )

/* Pointer to the beginning of the can 0 data area. */
#define CAN0_TX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN0TXIDR0)->data[ 0 ] )
#define CAN4_TX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN4TXIDR0 )->data[ 0 ] )
#define CAN1_TX_DATA_PTR   ( & ( ( message_buffer_T* ) & CAN1TXIDR0 )->data[ 0 ] )


/* ============================================================================ *\
 * Exported INLINE functions and #define function-like macros.
\* ============================================================================ */
/* Note: Clear flags by writing 1 to the bit position. 0 has no effect. */


/*.***************************************************************************
*. Name:  CAN_Disable_Soft_Reset()  MACRO
*.
*.   Description: clears the soft reset bit, enabling normal operation,
*.                of the CAN bus, and disabling the ability to write
*.                to many CAN registers.
*.
*.   Warning:     None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
******************************************************************************/


                    
/*.***************************************************************************
*. Name:  Hw_CAN_Bus_Off_Error  MACRO
*.
*. Description: Invoking this macro will turn on  the interrupt enable .
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
******************************************************************************/
#define Hw_CAN_Bus_Off_Error()\
                    ( CAN_ESR_BOFF )


/*.***************************************************************************
*. Name:  Hw_Clear_CAN_Bus_Off_Error  MACRO
*.
*. Description: Invoking this macro will clear the bus off interrupt flag,
*.                     which will acknowledge the interrupt
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
******************************************************************************/
#define Hw_Clear_CAN_Bus_Off_Error()\
                    (CAN_ESR_BOFF = 0)


/*.***************************************************************************
*. Name:  Hw_Enable_CAN_Bus_Off_Interrupt  MACRO
*.
*. Description: Invoking this macro will turn on  the interrupt enable .
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
******************************************************************************/
#define Hw_Enable_CAN_Bus_Off_Interrupt()\
                    ( CAN_IER_BOFIE = 1 )


/*.***************************************************************************
*. Name:  Hw_Disable_CAN_Bus_Off_Interrupt  MACRO
*.
*. Description: Invoking this macro will turn off the interrupt enable .
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
******************************************************************************/
#define Hw_Disable_CAN_Bus_Off_Interrupt()\
                    ( CAN_IER_BOFIE = 0 )


/*.***************************************************************************
*. Name:  Hw_Turn_Off_CAN_Receive_Interrupt  MACRO
*.
*. Description: Invoking this macro will turn off  the interrupt enable bit
*.              for the CAN receive buffer.
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
*.
******************************************************************************/


/*.***************************************************************************
*. Name:  Hw_Turn_On_CAN_Receive_Interrupt  MACRO
*.
*. Description: Invoking this macro will turn on  the interrupt enable bit
*.              for the CAN receive buffer.
*.
*. Warning: None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
*.
******************************************************************************/


/*
 * The following macros are intended to allow the code to be
 * written at a higher level of abstraction, hopefully resulting
 * in code which is easier to read and maintain.  Of necessity,
 * higher levels of abstraction hide some detail.  In this case,
 * the detail that is hidden is the fact that references to message
 * buffer parameters are references to a pointer to  array of structures.
 * This fact will have to be kept in mind when reading and maintaining
 * the code.  It is always necessary to have CAN_transmit_message_number
 * and/or CAN_receive_message_number defined within the scope of the use
 * of these macros.
 */
#define TRANSMIT_REQUESTED \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->transmit_requested

#define TRANSMIT_BUFFER_FULL \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->buffer_full

#define TRANSMIT_BLOCK_MESSAGES_LEFT \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->block_messages_left

#define TRANSMIT_NUMBER_OF_MESSAGES_IN_BUFFER \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->\
 number_of_messages_in_buffer

#define MESSAGE_IN_TRANSMIT_BUFFER \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->\
 message_in_transmit_buffer

#define TRANSMIT_NEXT_DATA_IN_PTR \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->next_data_in_ptr

#define TRANSMIT_NEXT_DATA_OUT_PTR \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->next_data_out_ptr

#define TRANSMIT_BUFFER_MAX_ADDRESS \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->buffer_max_address

#define TRANSMIT_BUFFER_BASE_ADDRESS \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->buffer_base_address

#define TRANSMIT_PONG_BUFFER_ADDRESS \
 Transmit_Buffer_Ctl_Ptr[CAN_transmit_message_number]->pong_buffer_address

#define RECEIVE_MESSAGE_STALE \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->stale

#define RECEIVE_BUFFER_FULL \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->buffer_full

#define RECEIVE_MESSAGE_LOST \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]-> message_lost

#define RECEIVE_INACTIVE_MESSAGE \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->inactive_message

#define NEVER_RECEIVED_MESSAGE \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->never_received_message

#define RECEIVE_BLOCK_MESSAGES_LEFT \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->block_messages_left

#define RECEIVE_NUMBER_OF_MESSAGES_IN_BUFFER \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->\
 number_of_messages_in_buffer

#define RECEIVE_NEXT_DATA_IN_PTR \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->next_data_in_ptr

#define RECEIVE_NEXT_DATA_OUT_PTR \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->next_data_out_ptr

#define RECEIVE_BUFFER_MAX_ADDRESS \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->buffer_max_address

#define RECEIVE_BUFFER_BASE_ADDRESS \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->buffer_base_address

#define RECEIVE_PONG_BUFFER_ADDRESS \
 Receive_Buffer_Ctl_Ptr[CAN_receive_message_number]->pong_buffer_address


/**************************
 * Function Macros
 **************************/
/*.***************************************************************************
*. Name:  No_Hardware_Buffer_Loaded()  MACRO
*.
*.   Description: Returns FALSE if any hardware transmit buffer is loaded
*.                with the interrupt enabled for any buffer, otherwise
*.                it returns TRUE. Used to see if it is safe for a non
*.                interrupt process to call the routine which transmits
*.                messages. It is not safe for a non interrupt process to
*.                call Hw_Transmit_Message if an interrupt can go off, causing
*.                the interrupt routine to also call Hw_Transmit_Message.
*.
*.   Warning:     None
*.
*. Shared Variables: none
*.
*. Parameters: none
*.
*. Return Value: TRUE or FALSE
*.
*.
*. Updated: 01/06/98 btc created.
******************************************************************************/

#define No_Hardware_Buffer_Loaded( ) \
                                          0
//        ( CAN0TFLG_TXE_MASK == CAN0TFLG_TXE )

/*.***************************************************************************
*. Name:  Hw_Initialize_CAN_Ctl_Registers   MACRO
*. Description:  This procedure loads the CAN Ctl registers with initial values
*.               preparatory to synchronizing the controller to the CAN
*.               bus.
*. Warning:      None
*.
*. Shared Variables: none
*.
*. Parameters: can_port
*.
*. Return Value: none
*.
******************************************************************************/
/*.***************************************************************************
*. Name:  Hw_Initialize_CAN_Mask_Registers   MACRO
*. Description:  This procedure loads the CAN Mask registers with initial
*.               values preparatory to synchronizing the controller to the CAN
*.               bus.
*. Warning:      None
*.
*. Shared Variables: none
*.
*. Parameters: none
*.
*. Return Value: none
*.
******************************************************************************/






typedef struct
{
   uint16_t id;
   uint16_t unused;
}  standard_id_T;

typedef struct
{
   uint32_t id;
}  extended_id_T;

typedef union
{
   standard_id_T standard;
   extended_id_T extended;
}  CAN_id_T;

/* This structure is used for both transmit and receive buffer although the
 * transmit_priority is NA for receive buffers.
 */
typedef struct
{
   CAN_id_T CAN;       /* Named so that dotted name makes sense. */
   uint8_t data[8];
   uint8_t length;
   uint8_t transmit_priority; /* Lower number is higher priority. */
}  message_buffer_T;

typedef struct
{
   bool    buffer_full;
   uint8_t number_of_messages_in_buffer;
   bool    message_in_transmit_buffer;
   uint8_t *next_data_in_ptr;
   uint8_t *next_data_out_ptr;
   uint8_t *buffer_max_address;
   uint8_t *buffer_base_address; /* ping buffer address*/
   uint8_t *pong_buffer_address; /* pong buffer address*/
   bool    transmit_requested;
}transmit_buffer_ctl_T;

typedef struct
{
   bool    stale;
   bool    buffer_full;
   bool    message_lost;
   bool    inactive_message;
   bool    never_received_message;
   uint8_t number_of_messages_in_buffer;
   uint8_t *next_data_in_ptr;
   uint8_t *next_data_out_ptr;
   uint8_t *buffer_max_address;
   uint8_t *buffer_base_address; /* ping buffer address */
   uint8_t *pong_buffer_address; /* pong buffer address */
}receive_buffer_ctl_T;


/* ============================================================================ *\
 * Exported variables. (global to physical and transfer layers)
\* ============================================================================ */
extern receive_buffer_ctl_T *Receive_Buffer_Ctl_Ptr[];
extern transmit_buffer_ctl_T *Transmit_Buffer_Ctl_Ptr[];


/* ============================================================================ *\
 * Exported Function prototypes
 * ============================================================================ */
extern void Hw_Transmit_Message( void );
extern void CAN_Receive_Interrupt( void );
extern void CAN_Error_Handler( void );
extern void Do_Init_CAN( void );


/* ==========================================================================*/
/* File revision history (top to bottom, first revision to last revision     */
/* ==========================================================================*/
/*                                                                           */
/* Date         user id     SCR     (description on following lines)         */
/* ----------   -------     ---     ----------------------------------       */
/* ==========================================================================*/
#endif /* DD_CAN_H */
