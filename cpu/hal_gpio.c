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
#define            PIN_AF_RESERVED             0x0F  


#define            KEY_DECT_PORT               GPIOB
#define            KEY_DECT_PIN                GPIO_PIN_1
#define            KEY_DECT_MODE               GPIO_MODE_INPUT
#define            KEY_DECT_PULL               GPIO_PULLUP
#define            KEY_DECT_SPEED              GPIO_SPEED_FREQ_LOW 
#define            KEY_DECT_AF                 PIN_AF_RESERVED 

#define            LED_LINE3_PORT              GPIOA
#define            LED_LINE3_PIN               GPIO_PIN_4
#define            LED_LINE3_MODE              GPIO_MODE_INPUT
#define            LED_LINE3_PULL              GPIO_NOPULL 
#define            LED_LINE3_SPEED             GPIO_SPEED_FREQ_LOW 
#define            LED_LINE3_AF                PIN_AF_RESERVED 

#define            LED_LINE2_PORT              GPIOA
#define            LED_LINE2_PIN               GPIO_PIN_5
#define            LED_LINE2_MODE              GPIO_MODE_INPUT
#define            LED_LINE2_PULL              GPIO_NOPULL 
#define            LED_LINE2_SPEED             GPIO_SPEED_FREQ_LOW 
#define            LED_LINE2_AF                PIN_AF_RESERVED 

#define            LED_LINE1_PORT              GPIOA
#define            LED_LINE1_PIN               GPIO_PIN_3
#define            LED_LINE1_MODE              GPIO_MODE_INPUT
#define            LED_LINE1_PULL              GPIO_NOPULL  
#define            LED_LINE1_SPEED             GPIO_SPEED_FREQ_LOW 
#define            LED_LINE1_AF                PIN_AF_RESERVED 

#define            DEBUG_TX_PORT               GPIOA
#define            DEBUG_TX_PIN                GPIO_PIN_9
#define            DEBUG_TX_MODE               GPIO_MODE_AF_PP
#define            DEBUG_TX_PULL               GPIO_NOPULL 
#define            DEBUG_TX_SPEED              GPIO_SPEED_FREQ_LOW
#define            DEBUG_TX_AF                 GPIO_AF1_USART1

#define            DEBUG_RX_PORT               GPIOA
#define            DEBUG_RX_PIN                GPIO_PIN_10
#define            DEBUG_RX_MODE               GPIO_MODE_AF_PP
#define            DEBUG_RX_PULL               GPIO_NOPULL
#define            DEBUG_RX_SPEED              GPIO_SPEED_FREQ_LOW
#define            DEBUG_RX_AF                 GPIO_AF1_USART1

#define            IIC1_CLK_PORT               GPIOB
#define            IIC1_CLK_PIN                GPIO_PIN_6
#define            IIC1_CLK_MODE               GPIO_MODE_AF_OD
#define            IIC1_CLK_PULL               GPIO_NOPULL 
#define            IIC1_CLK_SPEED              GPIO_SPEED_FREQ_LOW
#define            IIC1_CLK_AF                 GPIO_AF1_I2C1 

#define            IIC1_DAT_PORT               GPIOB
#define            IIC1_DAT_PIN                GPIO_PIN_7
#define            IIC1_DAT_MODE               GPIO_MODE_AF_OD
#define            IIC1_DAT_PULL               GPIO_NOPULL
#define            IIC1_DAT_SPEED              GPIO_SPEED_FREQ_LOW
#define            IIC1_DAT_AF                 GPIO_AF1_I2C1 

#define            SMB_CLK_PORT                GPIOA
#define            SMB_CLK_PIN                 GPIO_PIN_7
#define            SMB_CLK_MODE                GPIO_MODE_OUTPUT_PP //GPIO_MODE_AF_PP //GPIO_MODE_OUTPUT_OD //
#define            SMB_CLK_PULL                GPIO_NOPULL //GPIO_PULLUP //GPIO_PULLDOWN //
#define            SMB_CLK_SPEED               GPIO_SPEED_FREQ_HIGH
#define            SMB_CLK_AF                  PIN_AF_RESERVED 

#define            SMB_DAT_PORT                GPIOA
#define            SMB_DAT_PIN                 GPIO_PIN_6
#define            SMB_DAT_MODE                GPIO_MODE_OUTPUT_PP //GPIO_MODE_AF_PP //GPIO_MODE_OUTPUT_OD //
#define            SMB_DAT_PULL                GPIO_NOPULL //GPIO_PULLUP //GPIO_PULLDOWN //
#define            SMB_DAT_SPEED               GPIO_SPEED_FREQ_HIGH
#define            SMB_DAT_AF                  PIN_AF_RESERVED 


/*-- typedef -----------------------------------------------------------------*/
typedef struct  HalGpioConfigStruct
{
  GPIO_TypeDef*         port;
  u16_t                 pin;
  u32_t                 mode;
	u32_t                 pull;
	u32_t                 speed;
	u8_t                  af; 
}HalGpioConfigStruct;


/*-- private variables -------------------------------------------------------*/
static  const  HalGpioConfigStruct  gpioConfigArray[] = 
{
	[LED_LINE3_GPIO] = 
	{
	  LED_LINE3_PORT, LED_LINE3_PIN, LED_LINE3_MODE, LED_LINE3_PULL, LED_LINE3_SPEED, LED_LINE3_AF
	},
	
	[LED_LINE2_GPIO] = 
	{
	  LED_LINE2_PORT, LED_LINE2_PIN, LED_LINE2_MODE, LED_LINE2_PULL, LED_LINE2_SPEED, LED_LINE2_AF
	},
	
	[LED_LINE1_GPIO] = 
	{
	  LED_LINE1_PORT, LED_LINE1_PIN, LED_LINE1_MODE, LED_LINE1_PULL, LED_LINE1_SPEED, LED_LINE1_AF
	},
	
	[KEY_DECT_GPIO] = 
	{
	  KEY_DECT_PORT, KEY_DECT_PIN, KEY_DECT_MODE, KEY_DECT_PULL, KEY_DECT_SPEED, KEY_DECT_AF
	},
	
	[DEBUG_TX_GPIO] = 
  {
    DEBUG_TX_PORT, DEBUG_TX_PIN, DEBUG_TX_MODE, DEBUG_TX_PULL, DEBUG_TX_SPEED, DEBUG_TX_AF
  },
	
	[DEBUG_RX_GPIO] = 
  {
    DEBUG_RX_PORT, DEBUG_RX_PIN, DEBUG_RX_MODE, DEBUG_RX_PULL, DEBUG_RX_SPEED, DEBUG_RX_AF
  },
	
	[IIC1_CLK_GPIO] =
	{
	  IIC1_CLK_PORT, IIC1_CLK_PIN, IIC1_CLK_MODE, IIC1_CLK_PULL, IIC1_CLK_SPEED, IIC1_CLK_AF
	},
	
  [IIC1_DAT_GPIO] =
	{
	  IIC1_DAT_PORT, IIC1_DAT_PIN, IIC1_DAT_MODE, IIC1_DAT_PULL, IIC1_DAT_SPEED, IIC1_DAT_AF
	},
  
  [SMB_CLK_GPIO] =
	{
	  SMB_CLK_PORT, SMB_CLK_PIN, SMB_CLK_MODE, SMB_CLK_PULL, SMB_CLK_SPEED, SMB_CLK_AF
	},
	
  [SMB_DAT_GPIO] =
	{
	  SMB_DAT_PORT, SMB_DAT_PIN, SMB_DAT_MODE, SMB_DAT_PULL, SMB_DAT_SPEED, SMB_DAT_AF
	},
};

#define      GPIO_SIZE     sizeof(gpioConfigArray)/sizeof(HalGpioConfigStruct)
	

/*-- functions ---------------------------------------------------------------*/


/** 
  * @brief    
  * @param    NONE
  * @return   NONE
  */
void  hal_gpio_init(void)
{
  u8_t  i = 0;
  GPIO_InitTypeDef   hgpio;

	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
  for(i=0; i<GPIO_SIZE; i++)
  {
		hgpio.Mode  = gpioConfigArray[i].mode;
		hgpio.Pin   = gpioConfigArray[i].pin;
		hgpio.Pull  = gpioConfigArray[i].pull;
		hgpio.Speed = gpioConfigArray[i].speed;
		hgpio.Alternate = gpioConfigArray[i].af;
    HAL_GPIO_Init(gpioConfigArray[i].port, &hgpio);
  }
}


/** 
  * @brief    
  * @param    gpio     @see HalGpioSelect
  * @param    level    @see HalGpioOutLevel
  * @return   NONE
  */
void  hal_gpio_write(HalGpioSelect gpio, HalGpioOutLevel level)
{
  if(level == O_LOW_LEVEL)
  {
		HAL_GPIO_WritePin(gpioConfigArray[gpio].port, gpioConfigArray[gpio].pin, GPIO_PIN_RESET);
  }
  else if(level == O_HIGH_LEVEL)
  {
    HAL_GPIO_WritePin(gpioConfigArray[gpio].port, gpioConfigArray[gpio].pin, GPIO_PIN_SET);
  }
  else if(level == O_REVERSE_LEVEL)
  {
    HAL_GPIO_TogglePin(gpioConfigArray[gpio].port, gpioConfigArray[gpio].pin);
  }
}


/** 
  * @brief    
  * @param    gpio  @see HalGpioSelect
  * @return   @see HalGpioInLevel
  */
HalGpioInLevel  hal_gpio_read(HalGpioSelect gpio)
{
  if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(gpioConfigArray[gpio].port, gpioConfigArray[gpio].pin))
  {
    return  I_LOW_LEVEL;
  }
  else
  {
    return  I_HIGH_LEVEL;
  }
}


/** 
  * @brief    
  * @param    
  * @return   
  */
void*  hal_gpio_ctrl(HalGpioCtrlType cmd, void* param)
{
	void*    ret = NULL;
	GPIO_InitTypeDef    hgpio;
	HalChangeIOStruct*  changeIOStruct;
	
	switch(cmd)
	{
	  case CHANGE_IO_MODE:
		{
			changeIOStruct = (HalChangeIOStruct*)param;
			/* It must init all structure param. */
			hgpio.Mode     = changeIOStruct->mode;		
			hgpio.Pull     = changeIOStruct->pupd;
			hgpio.Pin      = gpioConfigArray[changeIOStruct->gpio].pin;
			hgpio.Speed    = gpioConfigArray[changeIOStruct->gpio].speed; 
			HAL_GPIO_Init(gpioConfigArray[changeIOStruct->gpio].port, &hgpio);
		}break;
		
		default:
			break;
	}
	
	return  ret;
}


/*------------------ end of file ---------------------------------------------*/
