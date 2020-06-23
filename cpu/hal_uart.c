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
static   UART_HandleTypeDef  huart1;


/*-- functions ---------------------------------------------------------------*/

/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void hal_uart1_putchar(u8_t dat)
{
	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}
	huart1.Instance->TDR = (dat & (u16_t)0x01FF);
}

/** @function  
  * @brief   
  * @param    	
  * @return  
  */
int fputc(int ch,FILE *f)
{
	hal_uart1_putchar(ch);
	
	return ch;
}
	
/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void hal_uart1_string(u8_t* dat, u8_t size)
{
	HAL_UART_Transmit(&huart1, dat, size, 500);
}


/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void   hal_uart1_init(u32_t baud,  UartRxIrqCallbackFunc  uartRxIrqCallbackPt)
{
	/* Enable USARTx clock */
  __HAL_RCC_USART1_CLK_ENABLE();
	
	/* UART configuration  */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = baud;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
  }
    	
//	__HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
//	__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
	
	__HAL_UART_ENABLE(&huart1);
}


/*------------------ end of file ---------------------------------------------*/
