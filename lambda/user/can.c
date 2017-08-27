#include "stm32f37x_conf.h"
#include "stdint.h"
#include "stm32f37x_can.h"
#include "io_can_config.h"
uint16_t crc=0xffff;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
TestStatus status;
uint32_t ret = 0; /* for return of the interrupt handling */
uint32_t can_message_id1;
extern 	CanRxMsg RxMessage;
extern float g_cala[14],g_calb[14];
extern u8 Rx_ADC_float[4];
extern  u8 Rx_ADC_float1[112];
float temp_float;
u8 Rxmessage10[8], RxMessage1[8];
bool VbNew_Message_Received;
bool VbNew_Message10_Received, VbSend_RD_DIAG, VbSend_WR_INIT1, VbSend_WR_INIT2;
u8 VcWR_INIT1, VcWR_INIT2;
extern CanTxMsg TxMessage,TxMessage1;
extern CanRxMsg RxMessage0;
extern void CAN_DeInit(CAN_TypeDef* CANx);

extern void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, FunctionalState NewState);
extern uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
extern uint8_t CAN_MessagePending(CAN_TypeDef* CANx, uint8_t FIFONumber);

extern void CAN_Receive(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg* RxMessage);
extern void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
extern uint8_t CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
extern uint8_t CAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox);

/* Private function prototypes -----------------------------------------------*/
void CAN_Config(void);
void calcrc(uint8_t *crcbuf,uint8_t size,uint8_t count);
TestStatus Receive(CanRxMsg *RxMessage0);
TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);
#if 0
int main(void)
{
  TestStatus status;              
  /* CAN configuration */
  CAN_Config();
        
        status=CAN_Polling();
        if(status)
        {
                CAN_Transmit(CAN1,&TxMessage);
        }
  
        status=CAN_Interrupt();
        if(status)
        {
                CAN_Transmit(CAN1,&TxMessage);
        }
        
}
#endif
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Connect CAN pins to AF7 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_7); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* NVIC configuration *******************************************************/
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  //RCC_APB1PeriphClockCmd(RCC_APB1ENR_CAN1EN, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 100KBps (CAN clocked at 36 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 8;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init "FIFO0" */

  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  

  /* Transmit Structure preparation */
/*  TxMessage.StdId = 0x1;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;  
        TxMessage.Data[0] = 0x01;
        TxMessage.Data[1] = 0x03;
        TxMessage.Data[2] = 0x00;
        TxMessage.Data[3] = 0x25;
        TxMessage.Data[4] = 0x00;
        TxMessage.Data[5] = 0x03;
        calcrc(&TxMessage.Data[0],20,6);*/

  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  //CAN_ITConfig(CAN1, CAN_IT_FF0, ENABLE);

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * CRC
  */

void calcrc(uint8_t *crcbuf,uint8_t size,uint8_t count)
{
        uint8_t bit=0;
        uint8_t i=0;
        uint8_t pos=0;
        if(size-count<2)
                while(1);
        
        while(pos<count)
        {
                crc ^= crcbuf[pos];
                for(i=0; i<8; i++)
                {
        
                        bit = crc&0x1;
                        crc >>= 1;
                        crc=crc & 0x7fff;
                        if(1==bit)
                        {
                                crc ^= 0xa001;
                        }
                        crc=crc &0xffff;
                }
                pos++;
        }
        crcbuf[pos++]=crc & 0x00ff;
        crcbuf[pos]=crc & 0xff00;
}

/**
  * 
  */


TestStatus CAN_Polling(void)
{

  uint8_t TransmitMailbox = 0;
        int i=0;

  TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CAN1, TransmitMailbox)  !=  CANTXOK) && (i  !=  0xFFFF))
  {
    i++;
  }

  i = 0;
  while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i  !=  0xFFFF))
  {
    i++;
  }

  /* receive */
  RxMessage0.StdId = 0x00;
  RxMessage0.IDE = CAN_ID_STD;
  RxMessage0.DLC = 0;
  RxMessage0.Data[0] = 0x00;
  RxMessage0.Data[1] = 0x00;
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage0);

  if (RxMessage0.StdId != 0x01)
  {
    return FAILED;  
  }

  if (RxMessage0.IDE != CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage0.DLC != 8)
  {
    return FAILED;  
  }

  
  return PASSED; /* Test Passed */
}

TestStatus CAN_Interrupt(void)
{
  uint32_t i = 0;

  /* transmit 1 message */
  TxMessage.StdId = 0;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xDE;
  TxMessage.Data[1] = 0xCA;
  CAN_Transmit(CAN1, &TxMessage);

  /* initialize the value that will be returned */
  ret = 0xFF;
       
  /* Receive message with interrupt handling */
  i = 0;
  while((ret ==  0xFF) && (i < 0xFFF))
  {
    i++;
  }
  
  if (i ==  0xFFF)
  {
    ret = 0;  
  }

  /* disable interrupt handling */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);

  return (TestStatus)ret;
}
u8 CAN1_Send_Msg(uint32_t can_message_id, u8* msg)
{	
  u8 mbox, CAN_transmit_message_number;
  u16 i=0;
  uint8_t requested;
  CanTxMsg TxMessage;
  can_message_id1 = can_message_id;
  requested = 0;
  if ( can_message_id == 0xf5070000)
   can_message_id = 0x7f5;
     for( CAN_transmit_message_number = 0;
        CAN_transmit_message_number <  CAN_NUMBER_OF_TRANSMIT_MESSAGES;
        CAN_transmit_message_number++ )
   {
      if( TRANSMIT_CAN_MESSAGE_ID == can_message_id )
      {
             requested = 1;
            break;
      }
   }
		requested = 1;
  if( requested == 1)
  {
     TxMessage.StdId= can_message_id;	 // 标准标识符为0
     TxMessage.IDE=0;		  // 使用扩展标识符
     TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
     TxMessage.DLC=8;							 // 发送两帧信息
     for(i=0;i<8;i++)
     TxMessage.Data[i]=msg[i];				 // 第一帧信息          
     mbox= CAN_Transmit(CAN1, &TxMessage);   
     i=0;
     while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
     if(i>=0XFFF)return 1;
     return 0;		
   }
}
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    if(RxMessage.StdId == 0x7FE)
     {
          for(i=0;i<RxMessage.DLC;i++)
          Rxmessage10[i]=RxMessage.Data[i];  
	Rx_ADC_float[0] = Rxmessage10[2];
	Rx_ADC_float[1] = Rxmessage10[3];
	Rx_ADC_float[2] = Rxmessage10[4];
	Rx_ADC_float[3] = Rxmessage10[5];
          if( Rxmessage10[1] == 0xA)
      	{
               g_cala[Rxmessage10[0]] = *(float *)(&Rx_ADC_float);
               Rx_ADC_float1[Rxmessage10[0] * 4] = Rxmessage10[2];
               Rx_ADC_float1[Rxmessage10[0] * 4 + 1] = Rxmessage10[3];
               Rx_ADC_float1[Rxmessage10[0] * 4 + 2] = Rxmessage10[4];
               Rx_ADC_float1[Rxmessage10[0] * 4 + 3] = Rxmessage10[5];
      	}
	else if( Rxmessage10[1] == 0xB)
      	{
              g_calb[Rxmessage10[0]] = *(float *)(&Rx_ADC_float);
              Rx_ADC_float1[Rxmessage10[0] * 4 + 56] = Rxmessage10[2];
              Rx_ADC_float1[Rxmessage10[0] * 4 + 57] = Rxmessage10[3];
              Rx_ADC_float1[Rxmessage10[0] * 4 + 58] = Rxmessage10[4];
              Rx_ADC_float1[Rxmessage10[0] * 4 + 59] = Rxmessage10[5];
      	}
   
      STMFLASH_Write(STARTADDR ,(uint16_t *)Rx_ADC_float1,56);
          VbNew_Message_Received = true;
     }
    else if(RxMessage.StdId == 0x66)
        {
              for(i=0;i<RxMessage.DLC;i++)
              buf[i]=RxMessage.Data[i];  
              if(buf[0] != 0)
              {
                VbSend_RD_DIAG = true;
              }
              else if(buf[1] != 0)
              {
                VbSend_WR_INIT1 = true;
                 VcWR_INIT1 = buf[1];
              }
              else if(buf[2] != 0)
              {
                VbSend_WR_INIT2 = true;
                 VcWR_INIT2 = buf[2];
              }
        }
     else
     {
          for(i=0;i<RxMessage.DLC;i++)
          buf[i]=RxMessage.Data[i];  
     }
	return RxMessage.DLC;	
}
