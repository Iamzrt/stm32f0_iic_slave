/**
  ******************************************************************************
  * @file     led.c
  * @author   ZRT
  * @version  V1.0
  * @date     2020/01/10
  * @attention  
	* @warning	
  ******************************************************************************  
  * 
  * 
  ******************************************************************************
  */
/*-- includes ----------------------------------------------------------------*/
#include "./main.h"
#include "./led.h"



/*-- defined -----------------------------------------------------------------*/
#define   DB_LOG(fmt, ...)       LOG("[LED]");LOG(fmt, ##__VA_ARGS__)



#define    LED_SCAN_SPEED        1   /* n+1 ms */


/*-- typedef -----------------------------------------------------------------*/
typedef    u16_t   led_status_size_t;

#define    LED_BIT_STATUS_MASK     (led_status_size_t)0x0001

typedef   union
{
  led_status_size_t  ledStatus;
	struct  
	{
	  led_status_size_t     led1Bit:    1;
		led_status_size_t     led2Bit:    1;
		led_status_size_t     led3Bit:    1;
		led_status_size_t     led4Bit:    1;
		led_status_size_t     led5Bit:    1;
		led_status_size_t     led6Bit:    1;
		led_status_size_t     led7Bit:    1;
	}LedStatusBit;
}LedStatusType;


typedef   void  (*LedOnOffFunc)(void);

typedef   struct
{
  u8_t          offState;
	LedOnOffFunc  offFunc;
	u8_t          onState;
	LedOnOffFunc  onFunc;
}LedStatusDist;


/*-- private variables -------------------------------------------------------*/
static   LedStatusType  allLedStatus;


static					void   	hw_led1_on(void);
static					void   	hw_led1_off(void);
static         	void   	hw_led2_on(void);
static					void   	hw_led2_off(void);
static					void   	hw_led3_on(void);
static					void   	hw_led3_off(void);
static         	void   	hw_led4_on(void);
static					void   	hw_led4_off(void);


static   LedStatusDist   ledOnOffArray[] = 
{
  [LED1] =
	{
	  LED_OFF,   hw_led1_off,   LED_ON,   hw_led1_on
	},
	
	[LED2] =
	{
	  LED_OFF,   hw_led2_off,   LED_ON,   hw_led2_on
	},
	
	[LED3] =
	{
	  LED_OFF,   hw_led3_off,   LED_ON,   hw_led3_on
	},
	
	[LED4] =
	{
	  LED_OFF,   hw_led4_off,   LED_ON,   hw_led4_on
	},
};



/*-- functions ---------------------------------------------------------------*/
static  void  hw_led_all_off(void)
{
	HalChangeIOStruct io;

	io.gpio = LED_LINE1_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE2_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE3_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE4_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
}
	

/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static  void   hw_led1_on(void)
{
  HalChangeIOStruct io;

	io.gpio = LED_LINE3_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE4_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE1_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE2_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
  hal_gpio_write(LED_LINE2_GPIO, O_HIGH_LEVEL);
	hal_gpio_write(LED_LINE1_GPIO, O_LOW_LEVEL);	
}

static  void   hw_led1_off(void)
{
	hw_led_all_off();	
}



/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static  void   hw_led2_on(void)
{	
	HalChangeIOStruct io;

	io.gpio = LED_LINE3_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE4_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE1_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE2_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
  hal_gpio_write(LED_LINE1_GPIO, O_HIGH_LEVEL);
	hal_gpio_write(LED_LINE2_GPIO, O_LOW_LEVEL);
}

static  void   hw_led2_off(void)
{
	hw_led_all_off();
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static  void   hw_led3_on(void)
{
	HalChangeIOStruct io;

	io.gpio = LED_LINE1_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE4_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE2_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE3_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);

  hal_gpio_write(LED_LINE2_GPIO, O_HIGH_LEVEL);
	hal_gpio_write(LED_LINE3_GPIO, O_LOW_LEVEL);
}



static  void   hw_led3_off(void)
{
	hw_led_all_off();
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static  void   hw_led4_on(void)
{
	HalChangeIOStruct io;

	io.gpio = LED_LINE1_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE4_GPIO;
	io.mode = MODE_INPUT;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE2_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
	io.gpio = LED_LINE3_GPIO;
	io.mode = MODE_OUTPUT_PP;
	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
	
  hal_gpio_write(LED_LINE3_GPIO, O_HIGH_LEVEL);
	hal_gpio_write(LED_LINE2_GPIO, O_LOW_LEVEL);
}


static  void   hw_led4_off(void)
{
	hw_led_all_off();
}


/** @function  
  * @brief   
  * @param    
  * @param    
  * @return  
  */
void   led1_on(void)
{
  allLedStatus.LedStatusBit.led1Bit = 1;
}

void   led1_off(void)
{
  allLedStatus.LedStatusBit.led1Bit = 0;
}

void   led1_toggle(void)
{
	if(allLedStatus.LedStatusBit.led1Bit == 0)
		allLedStatus.LedStatusBit.led1Bit = 1;
	else
		allLedStatus.LedStatusBit.led1Bit = 0;
}

void   led2_on(void)
{
  allLedStatus.LedStatusBit.led2Bit = 1;
}

void   led2_off(void)
{
  allLedStatus.LedStatusBit.led2Bit = 0;
}

void   led2_toggle(void)
{
	if(allLedStatus.LedStatusBit.led2Bit == 0)
		allLedStatus.LedStatusBit.led2Bit = 1;
	else
		allLedStatus.LedStatusBit.led2Bit = 0;
}

void   led3_on(void)
{
  allLedStatus.LedStatusBit.led3Bit = 1;
}

void   led3_off(void)
{
  allLedStatus.LedStatusBit.led3Bit = 0;
}

void   led3_toggle(void)
{
	if(allLedStatus.LedStatusBit.led3Bit == 0)
		allLedStatus.LedStatusBit.led3Bit = 1;
	else
		allLedStatus.LedStatusBit.led3Bit = 0;
}

void   led4_on(void)
{
  allLedStatus.LedStatusBit.led4Bit = 1;
}

void   led4_off(void)
{
  allLedStatus.LedStatusBit.led4Bit = 0;
}

void   led4_toggle(void)
{
	if(allLedStatus.LedStatusBit.led4Bit == 0)
		allLedStatus.LedStatusBit.led4Bit = 1;
	else
		allLedStatus.LedStatusBit.led4Bit = 0;
}



/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static   void   led_on_off_scan(void)
{
	static   u8_t   time = 0;
	static   led_status_size_t bitPos = 0;
	
	if(time < LED_SCAN_SPEED)
	{
		time++;
	}
	else
	{
		if(bitPos < sizeof(ledOnOffArray)/sizeof(LedStatusDist))
		{
			if(((allLedStatus.ledStatus>>bitPos) & LED_BIT_STATUS_MASK) == ledOnOffArray[bitPos].onState)
			{
				ledOnOffArray[bitPos].onFunc();
			}
			else
			{
			  ledOnOffArray[bitPos].offFunc();
			}
			bitPos++;
		}
		else
		{
			bitPos = 0;
		}
		time = 0;
	}
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  led_init(void)
{
	SysStatusType  status = OPERATE_OK;
	
	if(hal_timer6_update_callback_register(led_on_off_scan) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	allLedStatus.ledStatus = LED_OFF;
	
	return status;
}


/*------------------ end of file ---------------------------------------------*/

