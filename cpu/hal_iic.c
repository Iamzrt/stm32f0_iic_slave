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
#define     SLAVE_ADDR               0x16



/*-- private variables -------------------------------------------------------*/
static   I2C_HandleTypeDef  hi2c1;


/*-- functions ---------------------------------------------------------------*/

 

/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void   hal_iic_init(void)
{
	u32_t  tmpisr = 0;
	
	/* Peripheral clock enable */
  __HAL_RCC_I2C1_CLK_ENABLE();
	
	hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = SLAVE_ADDR;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE; //I2C_NOSTRETCH_DISABLE;  //
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    
  }
	
  /* Configure Analogue filter */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    
  }
	
  /* Configure Digital filter */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    
  }
	
	/* I2C1 interrupt Init */
  HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_IRQn);
	
	/**
		I2C_IT_ADDRI:     Address match interrupt.
		I2C_IT_STOPI:     STOP detection interrupt.
		I2C_IT_NACKI:     NACK received interrupt.
		I2C_IT_ERRI:      Errors interrupt.
	  I2C_IT_TCI:       TC interrupt. 
	  I2C_XFER_RX_IT:   REC interrupt. 
	*/
  tmpisr |= I2C_IT_ADDRI | I2C_IT_STOPI | I2C_IT_NACKI | I2C_IT_ERRI | I2C_IT_RXI | I2C_IT_TXI;
	__HAL_I2C_ENABLE_IT(&hi2c1, tmpisr);    
}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_putchar(u8_t dat)
{
  I2C1->TXDR = dat;
}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_tx_trig(u8_t dat)
{
  I2C1->ISR |= I2C_ISR_TXE;
  I2C1->TXDR = dat;
}



/** 
  * @brief   
  * @param     
  * @return  
  */
_WEAK  void  i2c1_addr_cplt_callback(u32_t transferDir, u16_t addrMatchCode)
{
  
}


/** 
  * @brief   
  * @param     
  * @return  
  */
_WEAK  void  i2c1_rx_cplt_callback(u8_t dat)
{

}


/** 
  * @brief   
  * @param     
  * @return  
  */
_WEAK  void  i2c1_tx_callback(void)
{
  
}


/** 
  * @brief   
  * @param     
  * @return  
  */
_WEAK  void  i2c1_af_callback(void)
{

}


/** 
  * @brief   
  * @param     
  * @return  
  */
_WEAK  void  i2c1_stop_callback(void)
{

}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c_err_irq_handler(I2C_HandleTypeDef *hi2c, u32_t isrFlag)     
{ 
  if((isrFlag & I2C_FLAG_ARLO) == I2C_FLAG_ARLO)
  { /* ARLO Err */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ARLO);
  }
    
  if((isrFlag & I2C_FLAG_BERR) == I2C_FLAG_BERR)
  { /* BERR Err */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_BERR);
  }
    
  if((isrFlag & I2C_FLAG_OVR) == I2C_FLAG_OVR)
  { /* OVR Err */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_OVR);
  }	
}



/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c_evt_irq_handler(I2C_HandleTypeDef *hi2c, u32_t isrFlag)     
{ 
	u8_t   dat = 0;
	u32_t  transferDir = 0;
	
	if(hi2c->Instance == hi2c1.Instance)
	{
    if((isrFlag & I2C_FLAG_ADDR) == I2C_FLAG_ADDR)
		{ /* EV1 */
			transferDir = isrFlag & I2C_ISR_DIR;                          
      i2c1_addr_cplt_callback(transferDir, SLAVE_ADDR);      
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ADDR);      
		} 
    
    if((isrFlag & I2C_FLAG_TXIS) == I2C_FLAG_TXIS)
		{ /* EV3 */	 
			i2c1_tx_callback();
		}
    
    if((isrFlag & I2C_FLAG_RXNE) == I2C_FLAG_RXNE)
		{ /* EV2 */ 	
			dat = hi2c->Instance->RXDR;
			i2c1_rx_cplt_callback(dat);
		}
    
    if((isrFlag & I2C_FLAG_STOPF) == I2C_FLAG_STOPF)
		{ /* EV4 */
      i2c1_stop_callback();
      //hi2c1.Instance->ISR |= I2C_ISR_TXE; 
      //hi2c1.Instance->ISR |= I2C_ISR_TXIS;
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);       
		}
    
    if((isrFlag & I2C_FLAG_AF) == I2C_FLAG_AF)
		{ /* EV3_2 */
      i2c1_af_callback();
			__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);     
		}
	}
} 



/** 
  * @brief   This function handles I2C1 global interrupt.
  * @param    
  * @param    
  * @return  
  */
void I2C1_IRQHandler(void)
{
  u32_t isrFlag = 0;
  
  isrFlag = hi2c1.Instance->ISR;
  
  if (isrFlag & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) 
	{
    //HAL_I2C_ER_IRQHandler(&hi2c1);
    i2c_err_irq_handler(&hi2c1, isrFlag);
  } 
	else 
	{
    //HAL_I2C_EV_IRQHandler(&hi2c1);
		i2c_evt_irq_handler(&hi2c1, isrFlag);
  }
}


/*------------------ end of file ---------------------------------------------*/
