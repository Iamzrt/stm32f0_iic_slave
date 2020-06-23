/**
  ******************************************************************************
  * @file     main.c
  * @author   ZRT
  * @version  V1.0
  * @date     2020/01/09
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
#include "./commun.h"
#include "./gasgauge.h"
#include "./bms.h"


/*-- defined -----------------------------------------------------------------*/
#define   DB_LOG(fmt, ...)       LOG("[MAIN]");LOG(fmt, ##__VA_ARGS__)


/*-- private variables -------------------------------------------------------*/


/*-- private functions -------------------------------------------------------*/
//u32_t freq = 0;

/*-- functions ---------------------------------------------------------------*/

/** @function  
  * @brief   
  * @param    
  * @param    
  * @return  
  */
int main(void)
{
  /* When system start, it should be disable the global interrupt. */
  GLOBAL_INTERRUPT_DISABLE();

  /* mcu common init. */
  hal_mcu_init();
  //freq = hal_get_sys_freq();

  /* Enable global interrupt. */
  GLOBAL_INTERRUPT_ENABLE();

  /* user init process. */
  led_init();
  bms_init();
  commun_init();

  /* Loop. */
  while(1)
  {
	  bms_loop();
  }
}


/**             
  * @brief     
  * @param                
  * @return      
  */
u8_t CRC8(u8_t *ptr, u8_t len, u8_t key)
{
	u8_t i;
	u8_t crc=0;
	while(len-- != 0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= key;
			}
			else
				crc *= 2;

			if((*ptr & i)!=0)
				crc ^= key;
		}
		ptr++;
	}
	return(crc);
}
/*------------------ end of file ---------------------------------------------*/

