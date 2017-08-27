/**
  ******************************************************************************
  * @file    SDADC/SDADC_Voltmeter/stm32f37x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x_it.h"
#include "sdadc.h"
#include "systick.h"
#include "reuse.h"
#include "io_can_config.h"
#include "ccp2main.h"
/** @addtogroup STM32F37x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SDADC_Voltmeter
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t InjectedConvData;
u8 Uart2_Buffer[255];
u16 VfStep_Count = 0x8000;
u32 VfStep_Count_pre;
uint8_t Uart2_Rx;
uint8_t Uart2_Tx;
bool Vbstarttoreceive;
uint8_t uart_length;
CanRxMsg RxMessage;
uint16_t InjectedConvData1[10], InjectedConvData3[10];
extern uint16_t InjectedConvData2;
bool VbRTI_enable;
extern u16 spi_message;
bool rxmsgisok;
uint32_t Uarttimeout;
uint32_t test1, test2, test3;
u8 count_temp;
bool test_can;
u8 received_msg[8];
extern uint16_t VcFault;
u16 AD[14];
uint16_t UR, UA, AD11, AD12;
u16 SPI_feedback;
u16 avgsdadc[10];
u16 numbersdadc;
u16 inputcapture[12], inputcapture_pre[12], inputcapture_diff[12];
extern u16 SPIByte(u16 byte);
u16 SPI2_ReadByte(u16 TxData);
u16 Master_Temp[100];
u16 UR2_7, UA3_7, UW3_5, UC2_5, UD_3_5;
bool Vbleft, Vbright;

extern bool Get_Message( CAN_message_ID_T can_message_id, uint8_t * message_address );
extern CCP_CRC_T CCP_Process_Command( HUGE uint8_t* ccp_command_ptr );

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f37x.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles SDADC1 interrupt request.
  * @param  None
  * @retval : None
  */
void DMA2_Channel1_IRQHandler(void)
{
  uint32_t ChannelIndex;

  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    test1 = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
  }
}
void DMA2_Channel2_IRQHandler(void)
{
   uint32_t ChannelIndex;
  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    test2 = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
  }
}
void DMA2_Channel3_IRQHandler(void)
{
   uint32_t ChannelIndex;
  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    test3 = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
  }
}
void SDADC3_IRQHandler(void)
{
   uint32_t ChannelIndex2;

  if(SDADC_GetFlagStatus(SDADC3, SDADC_FLAG_JEOC) != RESET)
  {
    /* Get the converted value */
    InjectedConvData2 = SDADC_GetInjectedConversionValue(SDADC3, &ChannelIndex2);
    if(ChannelIndex2 == SDADC_Channel_4)
    {

       AD11 = InjectedConvData2;
    }
    else if(ChannelIndex2 == SDADC_Channel_5)
    {
       AD12 = InjectedConvData2;
    }

//    SDADC_SoftwareStartConv(SDADC3);

  //SDADC_StartCalibration(SDADC3);

  }
}
void SDADC1_IRQHandler(void)
{
  uint32_t ChannelIndex;
  uint16_t temp;
  #if 0
  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    /* Get the converted value */
    temp = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
    #if 0
    if( ChannelIndex == SDADC_Channel_8)
    {
      InjectedConvData3[0] = temp;

    }
    else if( ChannelIndex == SDADC_Channel_7)
    {
      InjectedConvData3[1] = temp;
    }
    else
    #endif
    if( ChannelIndex == SDADC_Channel_2)
    {
      InjectedConvData3[2] = temp;
            
      UW3_5 = (u16)(InjectedConvData3[2] + 32768) * 2.048 /65535;
       SDADC_Cmd(SDADC3, DISABLE);
       SDADC_Cmd(SDADC2, ENABLE);
       SDADC_Cmd(SDADC1, DISABLE);
       SDADC_SoftwareStartInjectedConv(SDADC2);
    }
        VcFault = 6;

  }
  #endif
}

void SDADC2_IRQHandler(void)
{
   uint32_t ChannelIndex,ChannelIndex2;

  if(SDADC_GetFlagStatus(SDADC2, SDADC_FLAG_JEOC) != RESET)
  {
    /* Get the converted value */
    InjectedConvData = SDADC_GetInjectedConversionValue(SDADC2, &ChannelIndex2);
    if(ChannelIndex2 == SDADC_Channel_1)
    {
       UR = InjectedConvData;
    }
     else if(ChannelIndex2 == SDADC_Channel_2)
    {
       UA = InjectedConvData;
    }

  }

}
void CAN1_RX0_IRQHandler(void)
{
        //CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
         //if( Get_Message( RxMessage.StdId, RxMessage ) )
         //if(RxMessage.StdId == 0x7F0)
         //{
           //CCP_Process_Command( RxMessage.Data );
         //}
         VcFault = 5;

         CAN1_Receive_Msg(&received_msg);
         if(RxMessage.StdId == 0x05)
            test_can = true;
         else
            test_can = false;
         //CAN_ClearFlag(CAN1,CAN_FLAG_FMP0);
         //CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
 //
}
void TIM2_IRQHandler(void)
{
   uint16_t i;
   if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
   {
     VbRTI_enable = true;
     TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);

	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
   }
}
void TIM5_IRQHandler(void)
{
   uint16_t i;
   if(TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)
   {
       TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);

	  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
   }
}
void TIM12_IRQHandler(void)
{
   	uint16_t i;
	if(TIM_GetITStatus(TIM12, TIM_IT_CC1)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1);   
		TIM_ClearFlag(TIM12, TIM_IT_CC1);

		inputcapture[4] = TIM_GetCapture1(TIM12);
		if(inputcapture[4] > inputcapture_pre[4])
			inputcapture_diff[4] = (inputcapture[4]- inputcapture_pre[4]) * 32 /25;
		else if(inputcapture[4] <  inputcapture_pre[4])
			inputcapture_diff[4] =( inputcapture[4] + 0xFFFF - inputcapture_pre[4]) * 32 /25;
		inputcapture_pre[4] = inputcapture[4];
	}
	else if(TIM_GetITStatus(TIM12, TIM_IT_CC2)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM12, TIM_IT_CC2);   
		TIM_ClearFlag(TIM12, TIM_IT_CC2);

		inputcapture[5] = TIM_GetCapture2(TIM12);
		if(inputcapture[5] > inputcapture_pre[5])
			inputcapture_diff[5] = (inputcapture[5] - inputcapture_pre[5]) * 32 /25;
		else if(inputcapture[5] <  inputcapture_pre[5])
			inputcapture_diff[5] =( inputcapture[5] + 0xFFFF - inputcapture_pre[5]) * 32 /25;
		inputcapture_pre[5] = inputcapture[5];
	}
}
void TIM19_IRQHandler(void)
{
   	uint16_t i;
	if(TIM_GetITStatus(TIM19, TIM_IT_CC1)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM19, TIM_IT_CC1);   
		TIM_ClearFlag(TIM19, TIM_IT_CC1);

		inputcapture[0] = TIM_GetCapture1(TIM19);
		if(inputcapture[0] > inputcapture_pre[0])
			inputcapture_diff[0] = (inputcapture[0]- inputcapture_pre[0]) * 32 /25;
		else if(inputcapture[0] <  inputcapture_pre[0])
			inputcapture_diff[0] =( inputcapture[0] + 0xFFFF - inputcapture_pre[0]) * 32 /25;
		inputcapture_pre[0] = inputcapture[0];
	}
	else if(TIM_GetITStatus(TIM19, TIM_IT_CC2)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM19, TIM_IT_CC2);   
		TIM_ClearFlag(TIM19, TIM_IT_CC2);

		inputcapture[1] = TIM_GetCapture2(TIM19);
		if(inputcapture[1] > inputcapture_pre[1])
			inputcapture_diff[1] = (inputcapture[1] - inputcapture_pre[1]) * 32 /25;
		else if(inputcapture[1] <  inputcapture_pre[1])
			inputcapture_diff[1] =( inputcapture[1] + 0xFFFF - inputcapture_pre[1]) * 32 /25;
		inputcapture_pre[1] = inputcapture[1];
	}
	else if(TIM_GetITStatus(TIM19, TIM_IT_CC3)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM19, TIM_IT_CC3);   
		TIM_ClearFlag(TIM19, TIM_IT_CC3);

		inputcapture[2] = TIM_GetCapture3(TIM19);
		if(inputcapture[2] > inputcapture_pre[2])
			inputcapture_diff[2]= (inputcapture[2] - inputcapture_pre[2]) * 32 /25;
		else if(inputcapture[2] <  inputcapture_pre[2])
			inputcapture_diff[2] =( inputcapture[2] + 0xFFFF - inputcapture_pre[2]) * 32 /25;
		inputcapture_pre[2] = inputcapture[2];
	}
	else if(TIM_GetITStatus(TIM19, TIM_IT_CC4)!= RESET)
	{
	   	TIM_ClearITPendingBit(TIM19, TIM_IT_CC4);   
		TIM_ClearFlag(TIM19, TIM_IT_CC4);

		inputcapture[3] = TIM_GetCapture4(TIM19);
		if(inputcapture[3] > inputcapture_pre[3])
			inputcapture_diff[3] = (inputcapture[3] - inputcapture_pre[3]) * 32 /25;
		else if(inputcapture[3] <  inputcapture_pre[3])
			inputcapture_diff[3] =( inputcapture[3] + 0xFFFF - inputcapture_pre[3]) * 32 /25;
		inputcapture_pre[3]= inputcapture[3];
	}
}

void TIM3_IRQHandler(void)
{

}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6); //清除标志
    }
}
void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4); //清除标志
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) == 0)
          {
            Vbleft = 1;
            Vbright = 0;
            if(VfStep_Count  != 0)
            VfStep_Count = VfStep_Count - 1;
          }
          else
         {
            Vbleft = 0;
            Vbright = 1;
            if(VfStep_Count  != 0xffff)
            VfStep_Count = VfStep_Count + 1;
          }
    }
}
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10); //清除标志
         if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4 )== 0)
          {
            Vbleft = 0;
            Vbright = 1;
            if(VfStep_Count  != 0xffff)
            VfStep_Count = VfStep_Count + 1;

          }
         else
         {
            if(VfStep_Count  != 0)
            VfStep_Count = VfStep_Count - 1;

            Vbleft = 1;
            Vbright = 0;
          }
    }
}
void USART3_IRQHandler(void)
{  
   uint8_t temp;
   if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) //中断产生
   {
      Uarttimeout = 20000;
      USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志
      temp = USART_ReceiveData(USART3);
      if((Uart2_Rx == 0) && ((temp == 7 )|| (temp == 9)))
      {
         Vbstarttoreceive = 1;
      }
      if(Vbstarttoreceive == 1)
      {
         Uart2_Buffer[Uart2_Rx] = USART_ReceiveData(USART3);
         if((Uart2_Buffer[1] == 0x10) && (Uart2_Rx == 6))
         {
            uart_length = 13;
         }
         else if((Uart2_Buffer[1] == 0x03) )
         {
            uart_length = 8;
         }
         else if((Uart2_Buffer[1] == 0x06) )
         {
            uart_length = 8;
         }
         Uart2_Rx++;
         if((Uart2_Rx % uart_length) == 0)
         {
            rxmsgisok = true;
            Uart2_Rx = 0;
         }
      }
   }         
   if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET) //溢出
   {
      USART_ClearFlag(USART3,USART_FLAG_ORE); //?SR
      USART_ReceiveData(USART3); //?DR
   }
  // while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET){}
   
}

u16 SPI2_ReadByte(u16 TxData)
{  
     u8 retry=0;    
     // while((SPI2->SR&1<<0)==0) //???????byte  
     //{
     //     retry++;
     //     if(retry>200)return 0;
     //}             
     return SPI_I2S_ReceiveData16(SPI2);          //???????        
}
void SPI2_IRQHandler(void)
{ 
     if((SPI2->SR&1<<0)==1) 
     {  
         GPIO_SetBits(GPIOA, GPIO_Pin_11);

          if(count_temp != 99 )
            count_temp = count_temp + 1;
            Master_Temp[count_temp] = SPI2_ReadByte(0x00);

     }  
}
u16 SPI1_ReadByte(u16 TxData)
{  
     u8 retry=0;    
      while((SPI1->SR&1<<0)==0) //???????byte  
     {
          retry++;
          if(retry>200)return 0;
     }             
     return SPI_ReceiveData8(SPI1);          //???????        
}
void SPI1_IRQHandler(void)
{ 
     if((SPI1->SR&1<<0)==1) 
     {  
          if(count_temp < 99 )
            count_temp = count_temp + 1;
					else
						count_temp=2;
					
            Master_Temp[count_temp] = SPI1_ReadByte(0x00);
          if(Master_Temp[count_temp] == 0x10)
             GPIO_WriteBit(GPIOF,GPIO_Pin_9,1);
          if((count_temp % 2 ) == 0)
          {
            SPI_feedback = SPI_feedback * 0x100  +  Master_Temp[count_temp];
            GPIO_WriteBit(GPIOB,GPIO_Pin_0,0);
           }
          else
          {
             SPI_feedback = Master_Temp[count_temp];
          }
     }  
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
