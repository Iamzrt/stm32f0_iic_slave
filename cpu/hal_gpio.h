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

#ifndef   HAL_GPIO_H
#define   HAL_GPIO_H

/* C++ */
#ifdef __cplusplus
extern "C" {
#endif
  
  
/*-- includes ----------------------------------------------------------------*/

 

  
/*-- typedef  ----------------------------------------------------------------*/

	
/*-- typedef ----------------------------------------------------------------*/ 
/* gpio interface defined */
typedef  enum
{                  
	LED_LINE4_GPIO,
	LED_LINE3_GPIO,
	LED_LINE2_GPIO,
	LED_LINE1_GPIO,
	KEY_DECT_GPIO,
	DEBUG_TX_GPIO,
  DEBUG_RX_GPIO, 
	IIC1_CLK_GPIO,
  IIC1_DAT_GPIO,
  SMB_CLK_GPIO,
  SMB_DAT_GPIO,
}HalGpioSelect;
  

/* output level */
typedef  enum
{
  O_LOW_LEVEL     = 0,
  O_HIGH_LEVEL    = 1,
  O_REVERSE_LEVEL = 2,  
}HalGpioOutLevel;

/* input level */
typedef  enum
{
  I_LOW_LEVEL  = 0,
  I_HIGH_LEVEL = 1,
}HalGpioInLevel;

/* ctrl type. */
typedef  enum
{
  CHANGE_IO_MODE = 0,
}HalGpioCtrlType;
  
/* gpio mode type */
typedef  enum
{
  MODE_INPUT                  =  0x00000000U,   /*!< Input Floating Mode     */
	MODE_OUTPUT_PP              =  0x00000001U,   /*!< Output Push Pull Mode   */
	MODE_OUTPUT_OD              =  0x00000011U,   /*!< Output Open Drain Mode  */
	MODE_ANALOG                 =  0x00000003U,   /*!< Analog Mode  */
	MODE_IT_RISING              =  0x10110000U,   /*!< External Interrupt Mode with Rising edge trigger detection */
}HalGoioMode;

/* gpio pull up or pull down */
typedef  enum
{
	NOPULL     =   0x00000000U,   /*!< No Pull-up or Pull-down activation  */
	PULLUP     =   0x00000001U,   /*!< Pull-up activation                  */
	PULLDOWN   =   0x00000002U,   /*!< Pull-down activation                */
}HalGpioPuPd;


typedef  struct
{
  HalGpioSelect     gpio;
	HalGoioMode       mode;
	HalGpioPuPd       pupd;
}HalChangeIOStruct;


/*-- External functions ------------------------------------------------------*/  
void            			hal_gpio_init(void);  
void            			hal_gpio_write(HalGpioSelect gpio, HalGpioOutLevel level);
HalGpioInLevel  			hal_gpio_read(HalGpioSelect gpio);
void*  								hal_gpio_ctrl(HalGpioCtrlType cmd, void* param);


#ifdef __cplusplus
}
#endif


#endif

/*------------------ end of file ---------------------------------------------*/
