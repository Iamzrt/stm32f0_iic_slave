/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date   
  * @brief   
  ******************************************************************************  
  * 
  * 
  ******************************************************************************
  */
#ifndef  LED_H
#define  LED_H


#include "../cpu/hal_mcu.h"


#define            LED_OFF                         0
#define            LED_ON                          1
	
/*-- typedef  ----------------------------------------------------------------*/
typedef   enum
{
  LED1  = 0,
	LED2,
	LED3,
	LED4,
}DrvLedDefined;



extern   SysStatusType    led_init(void);
extern   void             led1_on(void);
extern   void             led1_off(void);
extern   void             led1_toggle(void);
extern   void             led2_on(void);
extern   void             led2_off(void);
extern   void             led2_toggle(void);
extern   void             led3_on(void);
extern   void             led3_off(void);
extern   void             led3_toggle(void);
extern   void             led4_on(void);
extern   void             led4_off(void);
extern   void             led4_toggle(void);

#endif

/*------------------ end of file ---------------------------------------------*/

