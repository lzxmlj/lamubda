#include "uart.h"
#include <stdarg.h>
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/* Private functions ---------------------------------------------------------*/

void USART_Configuration(void)//串口初始化函数
  {  

        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
                
        RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE );
        RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOF, ENABLE);
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_7);
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_7);        
        /*
        *  USART1_TX -> PA9 , USART1_RX ->        PA10
        */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOF, &GPIO_InitStructure);        

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;                 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOC, &GPIO_InitStructure);        
       
        USART_InitStructure.USART_BaudRate = 38400;//设置串口波特率
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位
        USART_InitStructure.USART_Parity = USART_Parity_No;//设置效验位
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置工作模式
        USART_Init(USART3, &USART_InitStructure); //配置入结构体
        USART_Cmd(USART3, ENABLE);//使能串口1

		}			

void UART_send_byte(uint8_t byte) //发送1字节数据
{
 while(!((USART3->ISR)&(1<<7)));
 USART3->TDR=byte;	
}		

void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART3->ISR)&(1<<7)));//等待发送完
		USART3->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}

uint8_t UART_Recive(void)
{	
	while(!(USART3->ISR & (1<<5)));//等待接收到数据
	return(USART3->RDR);			 //读出数据
}


PUTCHAR_PROTOTYPE 
{
/* 将Printf内容发往串口 */
  USART_SendData(USART3,(uint8_t)  ch);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
	{}
 
  return (ch);
}


