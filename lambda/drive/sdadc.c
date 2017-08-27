#include "sdadc.h"
__IO uint16_t SDADCConvertedValue[10]; 

extern void DMA_Config(void);
extern void DMA1_Config(void);
 uint32_t SDADC1_Config(void)
{
  SDADC_AINStructTypeDef  SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  SDADC_InitTypeDef SDADC_InitStructure;
  uint32_t SDADCTimeout = 0;

  /* SDADCÕ‚…Ë ±÷”≈‰÷√ */
  RCC_APB2PeriphClockCmd(POT_SDADC_CLK|POT_SDADC_CLK3 | RCC_APB2Periph_SDADC1, ENABLE);
  //RCC_APB2PeriphClockCmd(POT_SDADC_CLK, ENABLE);
  
  /* PWR ±÷” πƒ‹ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /*  πƒ‹ƒ£ƒ‚Õ‚…Ë*/
  PWR_SDADCAnalogCmd(POT_SDADC_PWR | POT_SDADC_PWR3 | PWR_SDADCAnalog_1, ENABLE);
  //PWR_SDADCAnalogCmd(POT_SDADC_PWR, ENABLE);

  /* …Ë÷√ SDADC ∑÷∆µ: SDADC “™≈‹‘⁄ @6MHz */
  /* »Áπ˚œµÕ≥ ±÷” 72MHz, SDADC”¶∏√12∑÷∆µ */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div12);

  /* GPIO ±÷”Õ‚…Ë πƒ‹ */
  RCC_AHBPeriphClockCmd(POT_GPIO_CLK, ENABLE);
  RCC_AHBPeriphClockCmd(POT_GPIO_CLK3 , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* SDADC io∂Àø⁄…Ë÷√5P (PB1) */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_7;
  //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_Init(POT_GPIO_PORT3, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* —°‘ÒÕ‚≤ø≤Œøº£∫ The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(POT_SDADC_VREF);

  /* Insert delay equal to ~5 ms */
  Delay_ms(5);
  
  /*  πƒ‹ POT_SDADC */
 // SDADC_Cmd(SDADC1, ENABLE);
  SDADC_Cmd(POT_SDADC, ENABLE);
  SDADC_Cmd(POT_SDADC3, ENABLE);
  //SDADC_Cmd(SDADC1, ENABLE);
  
  /* Ω¯»Î≥ı ºªØƒ£ Ω  */
  SDADC_InitModeCmd(POT_SDADC, ENABLE);
  SDADC_InitModeCmd(POT_SDADC3, ENABLE);
  //SDADC_InitModeCmd(SDADC1, ENABLE);
  //SDADC_InitModeCmd(SDADC1, ENABLE);
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  /*µ»¥˝÷±µΩ≥ı ºªØÕÍ≥… wait for INITRDY flag to be set */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
    return 1;
  }
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  //while((SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  //if(SDADCTimeout == 0)
 // {
    /* INITRDY flag can not set */
  // return 1;
  //}
  SDADCTimeout = SDADC_INIT_TIMEOUT;

  while((SDADC_GetFlagStatus(POT_SDADC3, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
    return 1;
  }
  /* ƒ£ƒ‚ ‰»Î≈‰÷√Analog Input configuration conf0: use single ended zero reference */
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_SEZeroReference;
  SDADC_AINStructure.SDADC_Gain = POT_SDADC_GAIN;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VSSA;
  SDADC_AINStructure.SDADC_Offset = 0;
  //SDADC_AINInit(SDADC1, SDADC_Conf_0, &SDADC_AINStructure);
  SDADC_AINInit(POT_SDADC, SDADC_Conf_0, &SDADC_AINStructure);
  SDADC_AINInit(POT_SDADC3, SDADC_Conf_0, &SDADC_AINStructure);
  //SDADC_DMAConfig(POT_SDADC, SDADC_DMATransfer_Injected, ENABLE);
  //SDADC_DMAConfig(POT_SDADC3, SDADC_DMATransfer_Injected, ENABLE);

  /* select POT_SDADC channel 5 to use conf0 */
  //SDADC_ChannelConfig(SDADC1, SDADC_Channel_2, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_0, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_1, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_2, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_3, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_4, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_5, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_6, SDADC_Conf_0);
  //SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_2, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_0, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_1, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_2, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_3, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_4, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_5, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC3, SDADC_Channel_6, SDADC_Conf_0);
  /* select channel 5 */
  //SDADC_InjectedChannelSelect(SDADC1, SDADC_Channel_2);
  SDADC_InjectedChannelSelect(POT_SDADC, POT_SDADC_CHANNEL);
  SDADC_InjectedChannelSelect(POT_SDADC3, POT_SDADC_CHANNEL);
  /* Enable continuous mode */
  SDADC_InjectedContinuousModeCmd(POT_SDADC, ENABLE);
  SDADC_InjectedContinuousModeCmd(POT_SDADC3, ENABLE);
  //SDADC_InjectedContinuousModeCmd(SDADC1, ENABLE);
  /* Exit initialization mode */
  SDADC_InitModeCmd(POT_SDADC, DISABLE);
  SDADC_InitModeCmd(POT_SDADC3, DISABLE);
  //SDADC_InitModeCmd(SDADC1, DISABLE);

 // SDADC_CalibrationSequenceConfig(SDADC1, SDADC_CalibrationSequence_1);
  SDADC_CalibrationSequenceConfig(POT_SDADC, SDADC_CalibrationSequence_1);
  SDADC_CalibrationSequenceConfig(POT_SDADC3, SDADC_CalibrationSequence_1);
  /* start POT_SDADC Calibration */
 // SDADC_StartCalibration(SDADC1);
  SDADC_StartCalibration(POT_SDADC);
  SDADC_StartCalibration(POT_SDADC3);
  /* Set calibration timeout: 5.12 ms at 6 MHz in a single calibration sequence */
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  /* µ»¥˝÷±µΩ–£’˝ÕÍ≥…wait for POT_SDADC Calibration process to end */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
  if(SDADCTimeout == 0)
  {
    /* EOCAL flag can not set */
    return 2;
  }
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  /* µ»¥˝÷±µΩ–£’˝ÕÍ≥…wait for POT_SDADC Calibration process to end */
  //while((SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
 // if(SDADCTimeout == 0)
 // {
    /* EOCAL flag can not set */
  //  return 2;
 // }
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  while((SDADC_GetFlagStatus(POT_SDADC3, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
  if(SDADCTimeout == 0)
  {
    /* EOCAL flag can not set */
    return 2;
  }
  #if 0
  NVIC_InitStructure.NVIC_IRQChannel = SDADC1_IRQn;
   //???????1??(???????ìvoid USART1_IRQHandler(void)î???)
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //??????0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //???????0 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //????
   NVIC_Init(&NVIC_InitStructure);
   #endif
  NVIC_InitStructure.NVIC_IRQChannel = SDADC2_IRQn;
   //???????1??(???????ìvoid USART1_IRQHandler(void)î???)
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //??????0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //???????0 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //????
   NVIC_Init(&NVIC_InitStructure);
   NVIC_InitStructure.NVIC_IRQChannel = SDADC3_IRQn;
   //???????1??(???????ìvoid USART1_IRQHandler(void)î???)
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //??????0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //???????0 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //????
   NVIC_Init(&NVIC_InitStructure);

  /* Enable end of injected conversion interrupt */
  SDADC_ITConfig(POT_SDADC, SDADC_IT_JEOC, ENABLE);
  SDADC_ITConfig(POT_SDADC3, SDADC_IT_JEOC, ENABLE);
 //SDADC_ITConfig(SDADC1, SDADC_IT_JEOC, ENABLE);
  //SDADC_SoftwareStartInjectedConv(SDADC1);
  SDADC_SoftwareStartInjectedConv(SDADC2);
  SDADC_SoftwareStartInjectedConv(SDADC3);
  SDADC_Cmd(POT_SDADC3, DISABLE);
  //SDADC_SoftwareStartInjectedConv(SDADC1);
 /* Start a software start conversion */
  //DMA_Config();
  #if 0
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  #endif
  return 0;
}
 uint32_t SDADC2_Config(void)
{
  SDADC_AINStructTypeDef  SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  DMA_InitTypeDef     DMA_InitStruct;
  uint32_t SDADCTimeout = 0;

  /* SDADCÕ‚…Ë ±÷”≈‰÷√ */
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_SDADC2, ENABLE);
  
  /* PWR ±÷” πƒ‹ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /*  πƒ‹ƒ£ƒ‚Õ‚…Ë*/
  PWR_SDADCAnalogCmd(PWR_SDADCAnalog_2, ENABLE);

  /* …Ë÷√ SDADC ∑÷∆µ: SDADC “™≈‹‘⁄ @6MHz */
  /* »Áπ˚œµÕ≥ ±÷” 72MHz, SDADC”¶∏√12∑÷∆µ */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div12);

  /* GPIO ±÷”Õ‚…Ë πƒ‹ */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

  /* SDADC io∂Àø⁄…Ë÷√5P (PB1) */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_13;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  SDADC_DeInit(SDADC2);
  /* —°‘ÒÕ‚≤ø≤Œøº£∫ The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(POT_SDADC_VREF);

  /* Insert delay equal to ~5 ms */
  Delay_ms(5);
  
  /*  πƒ‹ POT_SDADC */
  SDADC_Cmd(SDADC2, ENABLE);

  /* Ω¯»Î≥ı ºªØƒ£ Ω  */
  SDADC_InitModeCmd(SDADC2, ENABLE);
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  /*µ»¥˝÷±µΩ≥ı ºªØÕÍ≥… wait for INITRDY flag to be set */
  while((SDADC_GetFlagStatus(SDADC2, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
    return 1;
  }

  /* ƒ£ƒ‚ ‰»Î≈‰÷√Analog Input configuration conf0: use single ended zero reference */
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_SEZeroReference;
  SDADC_AINStructure.SDADC_Gain = POT_SDADC_GAIN;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VSSA;
  SDADC_AINStructure.SDADC_Offset = 0;
  SDADC_AINInit(SDADC2, SDADC_Conf_0, &SDADC_AINStructure);

  /* select POT_SDADC channel 5 to use conf0 */
  SDADC_ChannelConfig(SDADC2, SDADC_Channel_1, SDADC_Conf_0);
  SDADC_ChannelConfig(SDADC2, SDADC_Channel_2, SDADC_Conf_0);
  
  /* select channel 5 */
  //SDADC_InjectedChannelSelect(POT_SDADC3, POT_SDADC_CHANNEL3);
  SDADC_InjectedChannelSelect(SDADC2, SDADC_Channel_1 | SDADC_Channel_2);
  /* Enable continuous mode */
  SDADC_InjectedContinuousModeCmd(SDADC2, ENABLE);

  /* Exit initialization mode */
  SDADC_InitModeCmd(SDADC2, DISABLE);
  
  SDADC_CalibrationSequenceConfig(SDADC2, SDADC_CalibrationSequence_1);
  /* start POT_SDADC Calibration */
  SDADC_StartCalibration(SDADC2);
  /* Set calibration timeout: 5.12 ms at 6 MHz in a single calibration sequence */
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  /* µ»¥˝÷±µΩ–£’˝ÕÍ≥…wait for POT_SDADC Calibration process to end */
  while((SDADC_GetFlagStatus(SDADC2, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
  if(SDADCTimeout == 0)
  {
    /* EOCAL flag can not set */
    return 2;
  }
  NVIC_InitStructure.NVIC_IRQChannel = SDADC2_IRQn;
  //???????1??(???????ìvoid USART1_IRQHandler(void)???)
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //??????0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //???????0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //????
  NVIC_Init(&NVIC_InitStructure);
  SDADC_ITConfig(SDADC2, SDADC_IT_JEOC, ENABLE);
  SDADC_SoftwareStartInjectedConv(SDADC2);

  /* Enable end of injected conversion interrupt */

  /* Start a software start conversion */
  //DMA_Config();
  #if 0
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  #endif
  return 0;
}

