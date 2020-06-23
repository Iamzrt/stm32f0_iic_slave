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

#ifndef   HAL_UART_H
#define   HAL_UART_H

/* C++ */
#ifdef __cplusplus
extern "C" {
#endif
  
  
/*-- includes ----------------------------------------------------------------*/

 

  
/*-- typedef  ----------------------------------------------------------------*/
typedef  void  (*UartRxIrqCallbackFunc)(u16_t  dat);
	
  
/*-- External functions ------------------------------------------------------*/
extern        void   hal_uart1_putchar(u8_t dat);
extern        void   hal_uart1_string(u8_t* dat, u8_t size);
extern        void   hal_uart1_init(u32_t baud,  UartRxIrqCallbackFunc  uartRxIrqCallbackPt);	


#endif
  
#ifdef __cplusplus
}
#endif


/*------------------ end of file ---------------------------------------------*/
