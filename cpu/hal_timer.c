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
#define     MAX_TIMER6_CALL_ARRAY      (u8_t)3 


/*-- private variables -------------------------------------------------------*/
static    TIM_HandleTypeDef   htim6;

static    HalTimerUpdateIntCallBack   timer6UpdateCallBackArray[MAX_TIMER6_CALL_ARRAY];


/*-- functions ---------------------------------------------------------------*/


/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
SysStatusType  hal_timer6_update_callback_register(HalTimerUpdateIntCallBack call)
{
	static  u8_t  callNum = 0;
	SysStatusType  ret = OPERATE_OK;
	
	if(callNum < MAX_TIMER6_CALL_ARRAY)
	{
	  timer6UpdateCallBackArray[callNum] = call;
	}
	else
	{
		return OPERATE_FAIL;
	}
	
	callNum++;
	
	return ret;
}

	

/** 
  * @brief    TIM7 init function. Counter is 1us base.
	            Note that the timer7 clock source is come from APB1.
  * @param    
  * @param    
  * @return  
  */
void hal_timer6_init(void)
{
	u8_t   num = 0;
	
	for(num = 0; num < MAX_TIMER6_CALL_ARRAY; num++)
	{
		timer6UpdateCallBackArray[num] = NULL;
	}
	
	__HAL_RCC_TIM6_CLK_ENABLE();
	
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = HAL_RCC_GetPCLK1Freq()/1000000;  //HAL_RCC_GetPCLK1Freq()/(1000000/2); /* 1us base count. If (APB1CLKDivider!=1), timer freq must be x2 */
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 2000;  /* 1ms */
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
  }

	HAL_NVIC_SetPriority(TIM6_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(TIM6_IRQn);
	
	__HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
	
	__HAL_TIM_ENABLE(&htim6);
}




/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
INTERRUPT_FUNC void TIM6_IRQHandler(void)
{
	u8_t   num = 0;
	
	if(__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) == TRUE)
	{
		for(num = 0; num < MAX_TIMER6_CALL_ARRAY; num++)
		{
			if(timer6UpdateCallBackArray[num] != NULL)
			{
				timer6UpdateCallBackArray[num]();
			}
		}
		
		__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
	}
}

/*------------------ end of file ---------------------------------------------*/
