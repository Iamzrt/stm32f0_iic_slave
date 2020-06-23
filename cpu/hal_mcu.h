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

#ifndef   HAL_MCU_H
#define   HAL_MCU_H

/* C++ */
#ifdef __cplusplus
extern "C" {
#endif
  
  
/*-- includes ----------------------------------------------------------------*/
#include "../includes/_sys_std.h"
#include "../includes/_sys_config.h"
#include "../cpu/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h"
	

#include "./hal_clk.h" 
#include "./hal_flash.h" 
#include "./hal_gpio.h"
#include "./hal_timer.h"
#include "./hal_uart.h"
#include "./hal_iic.h"	
	
  
/*-- typedef  ----------------------------------------------------------------*/
#define      GLOBAL_INTERRUPT_DISABLE()      do{                            \
	                                                __disable_irq();          \
																								}while(0)

#define      GLOBAL_INTERRUPT_ENABLE()       do{                            \
	                                                __enable_irq();           \
																								}while(0)
 

typedef   void   (*SysTickCallBackFunc)(void);																								

																								
/*-- External functions ------------------------------------------------------*/  
extern    void    hal_mcu_init(void);
extern    u32_t   hal_get_sys_freq(void);


  
#ifdef __cplusplus
}
#endif

#endif

/*------------------ end of file ---------------------------------------------*/
