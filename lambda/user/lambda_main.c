
#include "stm32f37x.h"
#include "sdadc.h"
#include "uart.h"
#include "systick.h"
#include <stdio.h>
#include "stm32f37x_can.h"
#include "stm32f37x_iwdg.h"
#include "stm32f37x_dma.h"
#include "stm32f37x_adc.h"
#include "stm32f37x_flash.h"
#include "adc.h"
#include "stm32f37x_spi.h"
#include "stm32f37x_rcc.h"
#include "reuse.h"
#include "stm32f37x_gpio.h"
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define STM32_FLASH_SIZE 256 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
#define Default_Grating_Value 0x800000
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u32 temp_count;
extern uint16_t UR, UA, AD11, AD12;

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint16_t  InjectedConvData = 0;
uint16_t InjectedConvData2 = 0;
u16 Output_V;
__IO uint32_t TimingDelay = 0;
uint8_t test_dlc[8];
uint8_t uart_tx_buff[50];
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
extern bool VbRTI_enable;
ITStatus state_key;
u32 start_time;
bool Vbfirst_run, htr_loop;
uint16_t abc;
u32 duty_cycle_temp;
uint8_t abc_delay, VcWatch_dog;
extern u8 Rxmessage10[8];
uint8_t count_loop, uarttransmitlen;
bool uarttransmit, VbtestForwork, VbtestForwork1,VbtestForwork_start;
extern u16 UW3_5, UA3_7,UR2_7, UC2_5,UD_3_5;
float KO2_V_LamCalLoThres, KO2_V_LamCalHiThres, KO2_t_LamReCal, KO2_V_HtrEffDewPt, KO2_V_HtrEffHiLim;
float KO2_T_HtrEffDewPtTiming, KO2_V_HtrEffInit;
extern u32 VfStep_Count_pre;
__IO uint16_t  ADC1ConvertedVoltage = 0;
extern uint8_t Uart2_Rx;
extern uint8_t Uart2_Tx;
extern u16 VfStep_Count;
u8* ADC_float;
u8 Rx_ADC_float[4];
u8 Rx_CAL_A[4];
u8 Rx_CAL_B[4];
u8 Rx_ADC_float1[112];
u8 Rx_ADC_float2[4];
extern bool VbSend_RD_DIAG, VbSend_WR_INIT1, VbSend_WR_INIT2;
extern u8 VcWR_INIT1, VcWR_INIT2;

extern u16 inputcapture_diff[12], UA;
extern uint32_t Uarttimeout;
uint16_t  VcErrorCount, SDADC1_DR_Address = 0x20000;
extern u8 Uart2_Buffer[255];
uint8_t Uartsent_Buffer[50];
uint8_t Uartsent_Buffer_old[21];
u8 testarray[10];
__IO uint16_t RegularConvData_Tab1;
__IO uint16_t RegularConvData_Tab11;
extern uint16_t InjectedConvData1[10];
extern bool VbNew_Message_Received, VbNew_Message10_Received;
u8 outputdata[56];
float g_cala[14],g_calb[14], Vfsd1_9, Vfsd1_7, LAMOFST, histroy_cal;

bool Vbauto_action, Vbmachine_reset, VbAuto_Start, VbAuto_Stop, Vbmachine_Stop;
extern bool rxmsgisok;
bool Vbcalcbypass, Vbmuryo_Check,Vbbypass_con, Vbtapebreak_Check, Vbauto_stop_alarm, VbPoint_abnormal, VbHigh_limit_abn, VbLow_limit_abn,VbHigh_limit_lost;
bool VbBuffer_overlength, VbCount_reverse, VbCount_abnormal,testa, testb, testc,testd, teste,testf;
__IO float modbusbuffer[11], VfIpmeas, Vfvalue_air, Vua, Vur,Vfair_fuel;
float temp_test_v, PT_VALUE, VF1, VF2;
float modbusbuffer1[8];
float modbusbuffer2[8];
uint16_t VcFault, VcReset;
u8 GPIO_Status[8], Vfindex_of_tables;
float SIFX_OUT;
uint8_t numberofupdated, startbyte;
uint8_t CANbps, stmCANID, uart485bps, uart485id;
uint16_t I2c_eepbuffer[2], temp2;
uint16_t NumDataRead = 0;
u16 spi_message;
extern u16 Master_Temp[100];
#define GetIO_High_Switch()  (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12))
#define GetIO_Low_Switch()  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))
#define GetIO_W_Feed_Switch() (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
#define GetIO_tapebreak_Switch() GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)
#define GetIO_muryo_Switch() (!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11))

//#define CAL_APP 1
#define APP     1
#define RD_DIAG 0x78
#define WR_INIT1 0x56
#define RD_INIT1 0x6C
#define WR_INIT2 0x5A
#define RD_INIT2 0x7E
typedef enum
{
  BitReset = 0,
  BitSet = 1
}IOE_BitValue_TypeDef;

typedef struct ADdata
{
	unsigned short vad;//???12?AD????*16???,????0~65535
	float vout;//????????
	float iir;//???0.0~1.0;???1.0;
	float a,b,c,d;//?????;???a=0,b=0,c=1,d=0;
} AdData_TYPE;
extern void UART_send_byte(uint8_t byte); //发送1字节数据
float   int2float(uint32_t i);
extern u8 SPI2_ReadByte(u8 TxData);
void DAC_Config(void);
void DAC_Config1(void);
void Update_From_CAN(void);

#define RX_485  GPIO_ResetBits(GPIOF,GPIO_Pin_6);  
  
u8 testTemp[10] = {1, 2, 3, 4, 6, 5, 8, 1, 2};
#define TX_485  GPIO_SetBits(GPIOF,GPIO_Pin_6);  
uint16_t InputVoltageMv1;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
u8 TxMessage[8], TxMessage1[8];
CanRxMsg RxMessage0 = {0};
extern u8 RxMessage1[8];
extern u16 AD[14];
extern CanRxMsg RxMessage;
float data1, data2, vad2, a1,b1,c1,a2,b2,c2,ref, RI,Rpmes;
u16 vad1;
extern u16 avgsdadc[10];
float Adc, vf, VRi, output_pwm;
u32 VfDcelay_Time, ScHour;
u16 old_rx_msg1,old_rx_msg, spi_status, ScOffset_work;
u8 ScWork_flow;
u8 CANID_Merge;
u16 ADC_value;
u8 ScBackVIC_Clamp_time, ScFrontVIC_Loosen_time, VcBuffer_B_Feed,ScFrontVIC_Clamp_time, ScBackVIC_Loosen_time;
u8 ScSecond, ScMinute;
bool SbSwitch_OnOff, VbLostSensor_Saw, VbLostSensor_Pump, VbLostSensor_Press;
u16 count_time, work_count,VcMotor_DC_count, count_time1, Scdelay_time, VcTestRUN, VcRuntime;
u8 VcLoop_Count, VcLoop_Count_Act, ScWork_AutoRun, virtual_pwm, virtual_periods = 100;
u8 VcLoop_Count, VcLoop_Count_Act, ScWork_AutoRun, virtual_pwm1, virtual_periods1 = 100;
u8 VcMotor_DC,VcMotor_DC_Cur, VcMotor_DC_pre, Vcdelay_time, Vcdelay_time1;
bool VbMotor_Start, Vbupdateforbutter, Vbneedgearoil, VbPumpOn_Pre, VbSawheel_high, Vbpump_high, VbAPP_Stop_up, Vbstart_roll;
bool VbRun_test, Vbtest_stop, VbRun_test_pre;
float Kfvalue_air[9] = {0.65, 0.70, 0.80, 0.90, 1.016, 1.18, 1.43, 1.70, 2.42};
float KfIpmeas[9] = {-2.22, -1.82, -1.11, -0.50, 0.00, 0.33, 0.67, 0.94, 1.38};
__IO uint16_t ADCConvertedValue[130]; 
uint32_t ADCConvertedValue_total;
typedef union {
  struct {      
     bitfield8_t            VL:1;     /* byte 2 bit 0 */
     bitfield8_t            :1;     /* byte 2 bit 1 */
     bitfield8_t            LA:1;                    /* byte 2 bit 2 */
     bitfield8_t            EN_F3K :1;     /* byte 2 bit 3 */
     bitfield8_t            RA:1;                    /* byte 2 bit 4-5 */
     bitfield8_t            :1;              /* byte 2 bit 6 */
     bitfield8_t            PA:1;                    /* byte 2 bit 7 */
     bitfield8_t            EN_HOLD:1;                    /* byte 2 bit 7 */
  } bf;
  uint8_t byte;    
}LBD_INIT1;
typedef union {
  struct {      
     bitfield8_t            Ua10:1;     /* byte 2 bit 0 */
     bitfield8_t            Ua20:1;     /* byte 2 bit 1 */
     bitfield8_t            Ua40:1;                    /* byte 2 bit 2 */
     bitfield8_t            Ua80 :1;     /* byte 2 bit 3 */
     bitfield8_t            ENSCUN:1;                    /* byte 2 bit 4-5 */
     bitfield8_t            SET_DIA_Q:1;              /* byte 2 bit 6 */
     bitfield8_t            SRESET:1;                    /* byte 2 bit 7 */
     bitfield8_t            :1;                    /* byte 2 bit 7 */
  } bf;
  uint8_t byte;    
}LBD_INIT2;

void ENC_Init(void);
extern void CAN_Config(void);
extern void ADC1_DMA_Init(void);
extern void CCP_Initialize(void);
u16 SPIByte(u16 word);
void Timer_Configuration(void);
void Timer_Configuration_IC(void);
void RTE_INITIAL(void);
void virtual_PWM_OUTPUT(u8 dc);

void App_Auto_Action(void);
void NVIC_Configuration(void);
void TIM4_19_config(void);
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);   
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
uint32_t STMFLASH_ReadLong(uint32_t faddr);
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);  	
void DMA_Config(void);
void DMA1_Config(void);
void SPI_Configuration(void);
void SPI1_Configuration(void);
void update_machine_status(void);
extern void IWDG_ReloadCounter(void);
void UART2_TX485_Puts(uint8_t str);
void ADC1_Init(void);
void IO_Configuration_base(void);
void Diagnostic(void);
void STMFLASH_Write_SJ(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
__IO uint16_t  ADC1ConvertedValuetemp = 0;

static void delay (int cnt) 
{
  while (cnt--);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 
   u8 canbuf[8];
   u8 res;
   u8 key;
   u8 i, offset, address_eep, j, c = 0;
   u16 checksum, temp_value;
  RCC_ClocksTypeDef RCC_Clocks;
  __IO float InputVoltageMv = 0,Vch1 = 0, Vch2 = 0;
    TestStatus status;
    	 uint16_t  ADC1ConvertedValue = 0;
  rxmsgisok = false;
  uarttransmit = false;
  ScWork_flow = 0;
  VcTestRUN = 50;
  VcLoop_Count_Act = 1;
  VcLoop_Count = 1;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  CAN_Config();
  //ENC_Init();
  
  CCP_Initialize();
  Vbfirst_run = true;

  IO_Configuration_base();
  abc =0xFF;
  count_loop = 1;
  VcErrorCount = 1;
  VcReset = 600;
  VcErrorCount = STMFLASH_ReadHalfWord(STARTADDR);
  SPI1_Configuration();
  SDADC2_Config();
  RTE_INITIAL();
  virtual_pwm = 1;
  virtual_periods = 1;
  virtual_pwm1 = 1;
  virtual_periods1 = 1;
  
   
   j = 0;
   for(i = 0; i < 14; i++)
  {
        Rx_CAL_A[0] = STMFLASH_ReadHalfWord(STARTADDR + j);
        Rx_CAL_A[1] = STMFLASH_ReadHalfWord(STARTADDR + j +1);
        Rx_CAL_A[2] = STMFLASH_ReadHalfWord(STARTADDR+ j+2);
        Rx_CAL_A[3] = STMFLASH_ReadHalfWord(STARTADDR + j +3);
         Rx_ADC_float1[j] = Rx_CAL_A[0];
         Rx_ADC_float1[j+1] = Rx_CAL_A[1];
         Rx_ADC_float1[j+2] = Rx_CAL_A[2];
         Rx_ADC_float1[j+3] = Rx_CAL_A[3];
        j = j + 4;
        g_cala[i] =*(float *)(&Rx_CAL_A);
  }
   j = 56;
  
   for(i = 0; i < 14; i++)
  {
        Rx_CAL_B[0] = STMFLASH_ReadHalfWord(STARTADDR + j);
        Rx_CAL_B[1] = STMFLASH_ReadHalfWord(STARTADDR + j +1);
        Rx_CAL_B[2] = STMFLASH_ReadHalfWord(STARTADDR+ j+2);
        Rx_CAL_B[3] = STMFLASH_ReadHalfWord(STARTADDR + j +3);
         Rx_ADC_float1[j] = Rx_CAL_B[0];
         Rx_ADC_float1[j+1] = Rx_CAL_B[1];
         Rx_ADC_float1[j+2] = Rx_CAL_B[2];
         Rx_ADC_float1[j+3] = Rx_CAL_B[3];
        j = j + 4;
        g_calb[i] =*(float *)(&Rx_CAL_B);
  }
#if APP
   GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
   abc = SPIByte(0x5688);
   delay(200);
#endif
#if CAL_APP
   GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
   abc = SPIByte(0x569D);
   delay(200);
#endif
   GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
   abc = SPIByte(0x6C00);
   delay(200);
   GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
   abc = SPIByte(0x5A02);
   delay(200);
   GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
   abc = SPIByte(0x7E00);

    while (1)
    {
      if(VbRTI_enable == true)
       {
		  virtual_PWM_OUTPUT(g_cala[9]);
       CANID_Merge = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) << 3 | GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) <<2
	   		| GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) <<1|GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5);
       VbRTI_enable = false;
       count_time++;
      count_time1++;

       if((count_time1 % 5000) == 0)
       {
    	  if((g_cala[10] != histroy_cal))
    	  {
    		  GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
    		  abc = SPIByte(0x7800);
    	  }    
		  
		  histroy_cal = g_cala[10];
		  	
	   if(GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_9) == CbTRUE)
          GPIO_WriteBit(GPIOF,GPIO_Pin_9,0);
       else
          GPIO_WriteBit(GPIOF,GPIO_Pin_9,1);
         #if 0
         Vfvalue_air = (UA  + 32767) * 3.3;
          if(Vfvalue_air > Kfvalue_air[9])
          {
             VfIpmeas = KfIpmeas[9];
          }
          else if(Vfvalue_air < Kfvalue_air[0])
          {
             VfIpmeas = KfIpmeas[0];
          }
          else
          {
            for(i =0; i < 9; i++)
            {
               if(Vfvalue_air == Kfvalue_air[i])
               {
                  VfIpmeas = KfIpmeas[i];
               }
               else if((Vfvalue_air > Kfvalue_air[i]) & (Vfvalue_air < Kfvalue_air[i + 1]))
               {

                  VfIpmeas = (KfIpmeas[i+1] -KfIpmeas[i]) /  (Kfvalue_air[i+1] -Kfvalue_air[i]) * Vfvalue_air;
               }
            }
          }
          Vua = 0.3 * 5 + 17 *VfIpmeas *  61.9;
          Vur = 5 / 17 + 15.5 * 184 * 300;
          
          GPIO_WriteBit(GPIOB,GPIO_Pin_0, 1);
          abc = SPIByte(0x7E00);
#endif
          count_time1 = 0;
       }
       if((count_time % 100 )== 0)
      {

         j = 0;
         KO2_V_LamCalLoThres = g_cala[1];
         KO2_V_LamCalHiThres = g_cala[2];
         KO2_t_LamReCal = g_cala[3];
         KO2_V_HtrEffDewPt =  g_cala[4];
         KO2_V_HtrEffHiLim =  g_cala[5];
		 KO2_T_HtrEffDewPtTiming = g_cala[6]; // 15s
 		
		 KO2_V_HtrEffInit = g_cala[7];//8.5V
		 LAMOFST = g_cala[8];//1.5
		 #if 0
         //Vfsd1_9 = 47 /122 * g_cala[0];
         ADC_float = (u8 *)(&Vfsd1_9);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         //Vfsd1_7 = 47 / 122 * (5 /17 + 15.5 * 158* 301 / 1000000);
         j = 4;
         ADC_float = (u8 *)(&Vfsd1_7);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
		 #endif
#if APP
         Rpmes = (float)((u16)(UA + 32768)* 2.5/ 65535);

		 //VfIpmeas = (float)( Rpmes - 1.5) / (1052.3);//17
		 VfIpmeas = (float)( Rpmes - 1.5) / (495.2);//8
		 if(VfIpmeas > KfIpmeas[8])
		 {
			Vfair_fuel = Kfvalue_air[8];
		 }
		 else if(VfIpmeas < KfIpmeas[0])
		 {
			Vfair_fuel = Kfvalue_air[0];
		 }
		 else
		 {
		   for(i =0; i < 7; i++)
		   {
			  if(VfIpmeas == KfIpmeas[i])
			  {
				 Vfair_fuel = Kfvalue_air[i];
				 i = 99;
			  }
			  else if((VfIpmeas > KfIpmeas[i]) & (VfIpmeas < KfIpmeas[i + 1]))
			  {
		 
				 Vfair_fuel = (Kfvalue_air[i+1] -Kfvalue_air[i]) / (KfIpmeas[i+1] -KfIpmeas[i]) * VfIpmeas + Kfvalue_air[i];
				 i = 99;
			  }
		   }
		 }
         ADC_float = (u8 *)(&Vfair_fuel);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
		 j = j + 4;
         ADC_float = (u8 *)(&VfIpmeas);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         CAN1_Send_Msg(0x64,&outputdata[0]);
		 j = j + 4;
		 
         Vfvalue_air = (float)((u16)(UR + 32768) * 2.5 / 65535 );
		 RI = (float)((float)(Vfvalue_air - 0.2941 ) / (float)(2449) * 1000000);
         ADC_float = (u8 *)(&RI);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         CAN1_Send_Msg(0x65,&outputdata[8]);
		 j = j + 4;
         ADC_float = (u8 *)(&Rpmes);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         CAN1_Send_Msg(0x66,&outputdata[12]);
		 j = j + 4;
         ADC_float = (u8 *)(&Vfvalue_air);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         CAN1_Send_Msg(0x66,&outputdata[16]);
		 j = j + 4;
		#endif
 #ifdef CAL_APP
         Vfvalue_air = (float)((u16)(UR + 32768) * 2.52 / 65535 );
		 RI = (float)((float)(Vfvalue_air - 0.2941 ) / (float)(15.5 * 158) * 1000000);
         ADC_float = (u8 *)(&RI);
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         Rpmes = (float)((u16)(UA + 32768)* 2.52/ 65535);
		 VfIpmeas = ( (Rpmes - 1.5) / (61.9 * 17) );
         ADC_float = (u8 *)(&VfIpmeas);
		 j = 4;
         outputdata[j] = ADC_float[0];
         outputdata[j+1] = ADC_float[1];
         outputdata[j+2] = ADC_float[2];
         outputdata[j+3] = ADC_float[3];
         //CAN1_Send_Msg(0x63,testTemp[i]);
         CAN1_Send_Msg(0x64,&outputdata[0]);
         #endif
         count_time = 0;
      }
	   

  	   	//virtual_PWM_OUTPUT(g_cala[9]);

	   #if 0
	   start_time = start_time + 1;
	   if(Vbfirst_run)
	   {
		   if((start_time != (KO2_T_HtrEffDewPtTiming * 10000)))
		   {
		   	output_pwm = KO2_V_HtrEffDewPt * KO2_V_HtrEffDewPt * 100 / (KO2_V_HtrEffHiLim * KO2_V_HtrEffHiLim);
			
	       //virtual_PWM_OUTPUT(KO2_V_HtrEffDewPt * KO2_V_HtrEffDewPt * 100 / (KO2_V_HtrEffHiLim * KO2_V_HtrEffHiLim) );
		   }
		   else
		   {

			start_time = 0;
			if(output_pwm < KO2_V_HtrEffHiLim)
			{
				output_pwm = output_pwm + 0.000004;
			}
			else
			{
				Vbfirst_run = false;
			}
			virtual_PWM_OUTPUT(output_pwm );
	   	   }
	   	}
	   
	   if(RI < 730)
	   {
		virtual_PWM_OUTPUT(40);
	   }
	   #endif
     }
    }
}


void Update_From_CAN(void)
{
   u8 i;
   if(VbNew_Message_Received == true)
   {
      ScWork_flow = Rxmessage10[1];
      SbSwitch_OnOff = (bool) (Rxmessage10[0] == 0xFF);
      VfStep_Count = ((Rxmessage10[3] == 0xFF) ? 0:VfStep_Count);
      VcWatch_dog = Rxmessage10[4];
      VbNew_Message_Received = false;
   }
}



void ADC1_Init(void)
{
  ADC_InitTypeDef     ADC_InitStruct;
  GPIO_InitTypeDef    GPIO_InitStruct;
  DMA_InitTypeDef DMA_InitStructure; 

  RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* Configure ADC Channel9 as analog input */
  //GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6 |GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  DMA_DeInit(DMA1_Channel1);  
	  //外设地址  
	  DMA_InitStructure.DMA_PeripheralBaseAddr = 0x4001244C;  
	  //内存地址  
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCConvertedValue;  
	  //dma传输方向单向  
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
	  //设置DMA在传输时缓冲区的长度  
	  DMA_InitStructure.DMA_BufferSize = 130;  
	  //设置DMA的外设递增模式，一个外设  
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	  //设置DMA的内存递增模式  
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	  //外设数据字长  
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	  //内存数据字长  
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
	  //设置DMA的传输模式：连续不断的循环模式  
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
	  //设置DMA的优先级别  
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	  //设置DMA的2个memory中的变量互相访问	
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
		
	  //使能通道1  
	  DMA_Cmd(DMA1_Channel1, ENABLE);  

  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStruct);
  /* Configure the ADC1 in continuous mode */
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfChannel = 13;
  ADC_Init(ADC1, &ADC_InitStruct);
  //ADC_TempSensorVrefintCmd(ENABLE); //channel17
  /* Convert the ADC1 Channel 9 with 55.5 Cycles as sampling time */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_55Cycles5);
  //使能ADC1的DMA  
  ADC_DMACmd(ADC1, ENABLE); 

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);  
  
  /* ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  /* ADC1 calibration start */
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1)); 

  ADC_SoftwareStartConv(ADC1);
  //ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);   

}

//////////////////////////////////////////////////////////////////////////     
// CRC MODBUS ??     
// ????: pDataIn: ????     
//           iLenIn: ????                
// ????: pCRCOut: 2?????      
    
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
}
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //????
	uint16_t secoff;	   //???????(16????)
	uint16_t secremain; //???????(16????)	   
 	uint16_t i;    
	uint32_t offaddr;   //??0X08000000????
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//????
	FLASH_Unlock();						//??
	offaddr=WriteAddr-STM32_FLASH_BASE;		//??????.
	secpos=offaddr/STM_SECTOR_SIZE;			//????  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//???????(2????????.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//????????   
	if(NumToWrite<=secremain)secremain=NumToWrite;//????????
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//?????????
		for(i=0;i<secremain;i++)//????
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//????  	  
		}
		if(i<secremain)//????
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//??????
			for(i=0;i<secremain;i++)//??
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,NumToWrite);//??????  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//???????,??????????. 				   
		if(NumToWrite==secremain)break;//?????
		else//?????
		{
			secpos++;				//?????1
			secoff=0;				//?????0 	 
		   	pBuffer+=secremain;  	//????
			WriteAddr+=secremain;	//?????	   
		   	NumToWrite-=secremain;	//??(16?)???
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//??????????
			else secremain=NumToWrite;//??????????
		}	 
	};	
	FLASH_Lock();//??
}

void STMFLASH_Write_SJ(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
        FLASH_Unlock();  //解锁FLASH编程擦除控制器
        FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);//清除标志位
        FLASH_ErasePage(WriteAddr);     //擦除指定地址页
        STMFLASH_Write_NoCheck(WriteAddr,pBuffer,NumToWrite);//??????  
        FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);//清除标志位
        FLASH_Lock();    //锁定FLASH编程擦除控制器

}
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(vu16*)faddr; 
}
uint32_t STMFLASH_ReadLong(uint32_t faddr)
{
	return *(vu32*)faddr; 
}
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

void DMA_Config(void)
{
DMA_InitTypeDef   DMA_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

DMA_DeInit(DMA2_Channel1); //恢复默认值，这个是必要的

/* DMA channel1 configuration ----------------------------------------------*/
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SDADC2->JDATAR;
DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t) &RegularConvData_Tab1;
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 1;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; // DMA_MemoryInc_Disable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA2_Channel1, &DMA_InitStructure);
//NVIC_Configuration();

NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel1_IRQn; 
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);

DMA_ITConfig(DMA2_Channel1, DMA_IT_TC, ENABLE);        //DMA通道1传输完成中断
 
/* Enable DMA channel1 */
DMA_Cmd(DMA2_Channel1, ENABLE);

}
void DMA1_Config(void)
{
DMA_InitTypeDef   DMA_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

DMA_DeInit(DMA1_Channel1); //恢复默认值，这个是必要的

/* DMA channel1 configuration ----------------------------------------------*/
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SDADC2->JDATAR;
DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t) &RegularConvData_Tab11;
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 3;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // DMA_MemoryInc_Disable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//NVIC_Configuration();

NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);

DMA_ITConfig(DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE);        //DMA通道1传输完成中断
 
/* Enable DMA channel1 */
DMA_Cmd(DMA1_Channel1, ENABLE);

}

void CalADdata(AdData_TYPE *pD)
{
	float temp = pD->a*pD->vad*pD->vad*pD->vad + pD->b*pD->vad*pD->vad + pD->c*pD->vad + pD->d;
	pD->vout = pD->iir*temp + (1.0f-pD->iir)*pD->vout;
}
void SPI_Configuration(void)
{
   SPI_InitTypeDef SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOF, ENABLE);
  /* Connect CAN pins to AF7 */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //??????
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //??????
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
   GPIO_Init(GPIOF, &GPIO_InitStructure);   
   //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   //GPIO_Init(GPIOF, &GPIO_InitStructure);   
   

   GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_5);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_5); 
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_5); 
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_5); 


   //SPI_I2S_DeInit(SPI2);
   //SPI_Cmd(SPI2, DISABLE);
   //--------------------- SPI1 configuration ------------------
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode   = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL   = SPI_CPOL_Low;
   SPI_InitStructure.SPI_CPHA   = SPI_CPHA_2Edge;
   SPI_InitStructure.SPI_NSS   = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
   SPI_InitStructure.SPI_FirstBit    = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial  = 7;
   SPI_Init(SPI2, &SPI_InitStructure);
   //SPI_SSOutputCmd(SPI2, ENABLE);
   //SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
   //--------- SPI2 configuration ------------------
   //SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   //SPI_Init(SPI2, &SPI_InitStructure);
   //--------- Enable SPI1 TXE interrupt ------------
   //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
   //--------- Enable SPI2 RXNE interrupt -------------
//   SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE | SPI_I2S_IT_OVR, ENABLE);
   //----- Enable SPI2 ------
    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
   //SPI_SSOutputCmd(SPI2, ENABLE);
#if 1

   SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  #endif
   SPI_Cmd(SPI2, ENABLE);
//  GPIO_ResetBits(GPIOA, GPIO_Pin_11);
   //----- Enable SPI1 ----
   //SPI_Cmd(SPI1, ENABLE);
}
   void SPI1_Configuration(void)
   {
      SPI_InitTypeDef SPI_InitStructure;
      GPIO_InitTypeDef GPIO_InitStructure;
      NVIC_InitTypeDef NVIC_InitStructure;
   
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);
     /* Connect CAN pins to AF7 */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
   
      SPI_I2S_DeInit(SPI1);
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //??????
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
       GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //??????
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
      //GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //??????
      //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     // GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
     // GPIO_Init(GPIOF, &GPIO_InitStructure);   
      //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
      //GPIO_Init(GPIOF, &GPIO_InitStructure);   
      
   
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5); 
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5); 
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5); 
      //GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_5); 
   
   
      //SPI_I2S_DeInit(SPI2);
      //SPI_Cmd(SPI2, DISABLE);
      //--------------------- SPI1 configuration ------------------
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      SPI_InitStructure.SPI_Mode   = SPI_Mode_Master;
      SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
      SPI_InitStructure.SPI_CPOL   = SPI_CPOL_Low;
      SPI_InitStructure.SPI_CPHA   = SPI_CPHA_2Edge;
      SPI_InitStructure.SPI_NSS   = SPI_NSS_Soft;
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
      SPI_InitStructure.SPI_FirstBit    = SPI_FirstBit_MSB;
      SPI_InitStructure.SPI_CRCPolynomial  = 7;
      SPI_Init(SPI1, &SPI_InitStructure);
      //SPI_SSOutputCmd(SPI2, ENABLE);
      //SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
      //--------- SPI2 configuration ------------------
      //SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      //SPI_Init(SPI2, &SPI_InitStructure);
      //--------- Enable SPI1 TXE interrupt ------------
      //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
      //--------- Enable SPI2 RXNE interrupt -------------
   //   SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE | SPI_I2S_IT_OVR, ENABLE);
      //----- Enable SPI2 ------
       SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
      //SPI_SSOutputCmd(SPI2, ENABLE);
#if 1
      SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
       NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
       NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
  #endif
      SPI_Cmd(SPI1, ENABLE);
      GPIO_WriteBit(GPIOC, GPIO_Pin_0,0);
      GPIO_WriteBit(GPIOB, GPIO_Pin_0,1);
      //----- Enable SPI1 ----
      //SPI_Cmd(SPI1, ENABLE);
   }


void TIM4_19_config()
{
   GPIO_InitTypeDef GPIO_InitStructure; 
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
   TIM_OCInitTypeDef  TIM_OCInitStructure;  
     u16 CCR1= 36000;          
    u16 CCR2= 36000;
  /* GPIOA and GPIOB clock enable */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);  
 
  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           // ?????? 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_2);     /* PWM??????? */  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_2);     /* PWM??????? */  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           // ?????? 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2); 

    /*PCLK1??2?????TIM3??????72MHz*/  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =0xF100;  
    TIM_TimeBaseStructure.TIM_Prescaler = 8;                                    //?????:???=2,??72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //????????:???  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //????????  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //???PWM??1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //?????????CCR1?????  
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);                                    //????1      
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  
    /* PWM1 Mode configuration: Channel2 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_Pulse = CCR2;                                       //????2??????,??????????PWM  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                    //?????????CCR2????? 
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);                                    //????2  
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM4, ENABLE);                                         //??TIM3?????ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM4, ENABLE);                                                      //??TIM3   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM19, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =0xF100;  
    TIM_TimeBaseStructure.TIM_Prescaler = 8;                                    //?????:???=2,??72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //????????:???  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //????????  
    TIM_TimeBaseInit(TIM19, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //???PWM??1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //?????????CCR1?????  
    TIM_OC1Init(TIM19, &TIM_OCInitStructure);                                    //????1      
    TIM_OC1PreloadConfig(TIM19, TIM_OCPreload_Enable);  
    /* PWM1 Mode configuration: Channel2 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_Pulse = CCR2;                                       //????2??????,??????????PWM  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                    //?????????CCR2????? 
    TIM_OC2Init(TIM19, &TIM_OCInitStructure);                                    //????2  
    TIM_OC2PreloadConfig(TIM19, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM19, ENABLE);                                         //??TIM3?????ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM19, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* Time base configuration */                                            
    //TIM_TimeBaseStructure.TIM_Period =0xF100;  
    TIM_TimeBaseStructure.TIM_Period =0x1000;  
    //TIM_TimeBaseStructure.TIM_Prescaler = 8;                                    //?????:???=2,??72/3=24MHz  
    TIM_TimeBaseStructure.TIM_Prescaler = 3600;                                    //?????:???=2,??72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //????????:???  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //????????  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //???PWM??1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    //TIM_OCInitStructure.TIM_Pulse = 36000;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_Pulse = 0x800;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //?????????CCR1?????  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                                    //????1      
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
    /* PWM1 Mode configuration: Channel2 */  
    TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //??TIM3?????ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM3, ENABLE);                                                      //??TIM3   
}


u16 SPIByte(u16 word)
{
   u16 retry = 0;
	u8 temp;
   //while (!(SPI2->SR & (1 << 1)));
   //(SPI2->DR) = byte;
   //while (!(SPI2->SR & 1));
   //return SPI2->DR;
   //return 1;
   	// Loop while DR register in not emplty


   	while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET )
      { 
      }	
     // (SPI1->DR) = word;
      //spi_status =  SPI_GetReceptionFIFOStatus(SPI1);
   
	// Send byte through the SPI1 peripheral
	SPI_SendData8( SPI1, word / 0x100) ;
//			   return SPI1->DR;
         while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE ) == RESET )
         { 
         }  
        // (SPI1->DR) = word;
         //spi_status =  SPI_GetReceptionFIFOStatus(SPI1);
      
      // Send byte through the SPI1 peripheral
      SPI_SendData8( SPI1, word & 0xFF) ;
   //          return SPI1->DR;

	#if 1
   retry = 0;
	while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE ) == RESET )
   { 
   retry++; 
   if(retry>=0x200)return 0;	//超时退出 
   }	 
	// Return the byte read from the SPI bus
	//return SPI_I2S_ReceiveData16( SPI2 ) ;
	 while( SPI1->SR & SPI_I2S_FLAG_BSY ); 

   return SPI1->DR;
   #endif
  //return SPI_ReceiveData8(SPI2);
} 
void Timer_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

   TIM_TimeBaseInitTypeDef Time_TimeBaseStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM5 , ENABLE);

   TIM_DeInit(TIM2);
   Time_TimeBaseStructure.TIM_Period = 40;
   Time_TimeBaseStructure.TIM_Prescaler = 1439;
   Time_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   Time_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, &Time_TimeBaseStructure);
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);
   TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
   TIM_Cmd(TIM2,ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
   TIM_DeInit(TIM5);
   Time_TimeBaseStructure.TIM_Period = 4 ;
   Time_TimeBaseStructure.TIM_Prescaler = 1439;
   Time_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   Time_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM5, &Time_TimeBaseStructure);
   TIM_ClearFlag(TIM5, TIM_FLAG_Update);
   TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);
   TIM_Cmd(TIM5,ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}
void RTE_INITIAL(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
   
         RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
   	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 152;
	TIM_TimeBaseStructure.TIM_Prescaler = 35;//1439
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}
void Timer_Configuration_IC(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);	
	
	/*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // ?????? 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_11); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_11); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_11); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_11); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_10); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_10); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM19, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 | RCC_APB1Periph_TIM2, ENABLE);
   TIM_DeInit(TIM12);


   TIM_DeInit(TIM19);
   TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
   TIM_TimeBaseStructure.TIM_Prescaler = 35;
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);	
   TIM_TimeBaseInit(TIM19, &TIM_TimeBaseStructure);	
   //TIM2通道的捕捉初始化 
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//通道选择
   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//下降沿 
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//管脚与寄存器对应关系
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
   TIM_ICInitStructure.TIM_ICFilter = 0x0;         //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
   TIM_ICInit(TIM19,&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//通道选择
   TIM_ICInit(TIM19,&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;//通道选择
   TIM_ICInit(TIM19,&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//通道选择
   TIM_ICInit(TIM19,&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//通道选择
   TIM_ICInit(TIM12,&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//通道选择
   TIM_ICInit(TIM12,&TIM_ICInitStructure);
   TIM_SelectInputTrigger(TIM12, TIM_TS_TI2FP2);    //选择时钟触发源
   TIM_SelectInputTrigger(TIM12, TIM_TS_TI1FP1);    //选择时钟触发源

   TIM_SelectInputTrigger(TIM19, TIM_TS_TI2FP2);    //选择时钟触发源
   TIM_SelectInputTrigger(TIM19, TIM_TS_TI1FP1);    //选择时钟触发源
   TIM_SelectSlaveMode(TIM19, TIM_SlaveMode_Reset);//触发方式  
   TIM_SelectMasterSlaveMode(TIM19, TIM_MasterSlaveMode_Enable); //启动定时器的被动触发
   TIM_SelectSlaveMode(TIM12, TIM_SlaveMode_Reset);//触发方式  
   TIM_SelectMasterSlaveMode(TIM12, TIM_MasterSlaveMode_Enable); //启动定时器的被动触发
   TIM_Cmd(TIM19, ENABLE); 
   TIM_Cmd(TIM12, ENABLE); 
   TIM_ITConfig(TIM19, TIM_IT_CC1, ENABLE);         //打开中断
   TIM_ITConfig(TIM19, TIM_IT_CC2, ENABLE);         //打开中断
   TIM_ITConfig(TIM19, TIM_IT_CC3, ENABLE);         //打开中断
   TIM_ITConfig(TIM19, TIM_IT_CC4, ENABLE);         //打开中断
   TIM_ITConfig(TIM12, TIM_IT_CC1, ENABLE);         //打开中断
   TIM_ITConfig(TIM12, TIM_IT_CC2, ENABLE);         //打开中断
   NVIC_InitStructure.NVIC_IRQChannel = TIM19_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   
   NVIC_Init(&NVIC_InitStructure);
   
   NVIC_InitStructure.NVIC_IRQChannel = TIM12_IRQn;
   NVIC_Init(&NVIC_InitStructure);
}

void IO_Configuration_base(void)
{
  GPIO_InitTypeDef    GPIO_InitStruct;
  EXTI_InitTypeDef   EXTI_InitStructure, EXTI_InitStructure1;
  NVIC_InitTypeDef NVIC_InitStructure,NVIC_InitStructure1;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

   RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOB, ENABLE);
 
  /* Configure ADC Channel9 as analog input */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOF, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;
  
  GPIO_Init(GPIOC , &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_1;
  
  GPIO_Init(GPIOB , &GPIO_InitStruct);
#if 0
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3  |GPIO_Pin_4;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Pin =   GPIO_Pin_2 | GPIO_Pin_3  |GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  

}//往左是4 往右是10
void DAC_Config(void)
{
  DAC_InitTypeDef    DAC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  /* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);
  
  /* DAC1 channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC1, DAC_Channel_1, &DAC_InitStructure);
  /* Enable DAC1 Channel1 */
  DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC1, DAC_Align_12b_R, 1024);

}
void DAC_Config1(void)
{
  DAC_InitTypeDef    DAC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  /* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);
  
  /* DAC1 channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC1, DAC_Channel_2, &DAC_InitStructure);
  /* Enable DAC1 Channel1 */
  DAC_Cmd(DAC1, DAC_Channel_2, ENABLE);
  DAC_SetChannel2Data(DAC1, DAC_Align_12b_R, 1024);

}


void ENC_Init(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   TIM_ICInitTypeDef TIM_ICInitStructure;

   GPIO_InitTypeDef  GPIO_InitStructure;
   NVIC_InitTypeDef  NVIC_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
   GPIO_StructInit(&GPIO_InitStructure);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2); 

  /* NVIC configuration *******************************************************/
  //NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

  //Timer configuration in Encoder mode
  TIM_DeInit(TIM3);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_Period = 10000;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,
   TIM_ICPolarity_Rising);
   TIM_ICStructInit(&TIM_ICInitStructure);
   TIM_ICInitStructure.TIM_ICFilter = 6;
   TIM_ICInit(TIM3, &TIM_ICInitStructure);

   TIM_ClearFlag(TIM3,TIM_FLAG_Update);
   TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
   TIM_SetCounter(TIM3, 1000);
   TIM_Cmd(TIM3, ENABLE);
}
void virtual_PWM_OUTPUT(u8 dc)
{

	if(dc < 100)
	{
		dc = dc * 2;
		if((virtual_pwm  <= dc))
        {
                GPIO_SetBits(GPIOB, GPIO_Pin_1);
        }
        else if(virtual_pwm  > dc)
        {
           GPIO_ResetBits(GPIOB, GPIO_Pin_1);
        }
        if(virtual_periods == 200)
        {
           virtual_periods = 0;
           virtual_pwm = 0;
        }
        else
        {
           virtual_periods = virtual_periods + 1;
        }
 	   
 	   virtual_pwm++;
		   #if 0
	       if((virtual_pwm1  <= dc))
	       {
	               virtual_pwm1++;
	               GPIO_SetBits(GPIOC, GPIO_Pin_2);
	       }
	       else if(virtual_pwm1  > dc)
	       {
	          virtual_pwm1++;
	          GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	       }
	       if(virtual_periods1 == 100)
	       {
	          virtual_periods1 = 0;
	          virtual_pwm1 = 0;
	       }
	       else
	       {
	          virtual_periods1 = virtual_periods1 + 1;
	       }
		   #endif
		}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
