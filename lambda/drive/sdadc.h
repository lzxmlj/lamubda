#ifndef __UART_H
#define	__UART_H

#include "stm32f37x.h"
#include "systick.h"


#define POT_GPIO_PORT        GPIOE
#define POT_GPIO_PIN         GPIO_Pin_0
#define POT_GPIO_CLK         RCC_AHBPeriph_GPIOE
#define POT_SDADC            SDADC2
#define POT_SDADC_CLK        RCC_APB2Periph_SDADC2
#define POT_SDADC_PWR        PWR_SDADCAnalog_2
#define POT_SDADC_VREF       SDADC_VREF_Ext /* External reference is selected */
#define POT_SDADC_GAIN       SDADC_Gain_1   /* Internal gain 1 is seleted: 
                                               SDADC_GAIN must be updated according to
                                               POT_SDADC_GAIN */
#define SDADC_GAIN           (uint32_t) 1  /* SDADC internal gain is set to 1: update this define
                                              according to POT_SDADC_GAIN */
#define POT_SDADC_CHANNEL    ( SDADC_Channel_0 |SDADC_Channel_1 | SDADC_Channel_2 | SDADC_Channel_3 |  SDADC_Channel_4 |SDADC_Channel_5 | SDADC_Channel_6)
#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT   30 /* ~ about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT    4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF           (float) 3300  /* SDADC external reference is set to 3.3V */

#define POT_GPIO_PORT3        GPIOD
#define POT_GPIO_PIN3         GPIO_Pin_0
#define POT_GPIO_CLK3         RCC_AHBPeriph_GPIOD
#define POT_SDADC3            SDADC3
#define POT_SDADC_CLK3        RCC_APB2Periph_SDADC3
#define POT_SDADC_PWR3        PWR_SDADCAnalog_3
#define POT_SDADC_VREF       SDADC_VREF_Ext /* External reference is selected */
//#define POT_SDADC_VREF       SDADC_VREF_VREFINT2 /* External reference is selected */
#define POT_SDADC_GAIN       SDADC_Gain_1   /* Internal gain 1 is seleted: 
                                               SDADC_GAIN must be updated according to
                                               POT_SDADC_GAIN */
#define SDADC_GAIN           (uint32_t) 1  /* SDADC internal gain is set to 1: update this define
                                              according to POT_SDADC_GAIN */
#define POT_SDADC_CHANNEL3    (SDADC_Channel_0 | SDADC_Channel_1 | SDADC_Channel_2 | SDADC_Channel_3 | SDADC_Channel_7)
#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT   30 /* ~ about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT    4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF           (float) 3300  /* SDADC external reference is set to 3.3V */


 uint32_t SDADC2_Config(void);

 uint32_t SDADC1_Config(void);

#endif /* __UART_H */
