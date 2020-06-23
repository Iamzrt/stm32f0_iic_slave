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
/*-- includes ----------------------------------------------------------------*/
#include "./hal_mcu.h"


/*-- defined -----------------------------------------------------------------*/


/*-- private variables -------------------------------------------------------*/
static   SysTickCallBackFunc sysTickCallFunc = NULL;


/*-- functions ---------------------------------------------------------------*/

/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void  hal_mcu_init(void)
{
  HAL_Init();
	
	hal_clk_init();
	hal_gpio_init();
	hal_timer6_init();
	hal_iic_init();
	
	hal_uart1_init(115200, NULL);
}

/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void  hal_mcu_reset(void)
{
//	__set_FAULTMASK(1);
//  HAL_NVIC_SystemReset();
}


/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
u32_t   hal_get_tick(void)
{
  return  HAL_GetTick();
}

/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void  hal_tick_delay(u32_t delay)
{
	HAL_Delay(delay);
}


/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void   hal_systick_update_callback_register(SysTickCallBackFunc func)
{
  sysTickCallFunc = func;
}



/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
u32_t  hal_get_sys_freq(void)
{
	u32_t freq = HAL_RCC_GetSysClockFreq();
  return  freq;
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */ 
INTERRUPT_FUNC  void SysTick_Handler(void)
{
	HAL_IncTick();
	
	if(sysTickCallFunc)		
	{
		sysTickCallFunc();
	}
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
INTERRUPT_FUNC void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
INTERRUPT_FUNC void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		hal_mcu_reset();
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
INTERRUPT_FUNC void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
INTERRUPT_FUNC void PendSV_Handler(void)
{
}
/*------------------ end of file ---------------------------------------------*/
