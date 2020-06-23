/**
  ******************************************************************************
  * @file     
  * @author   
  * @version  
  * @date    
  * @attention  
	* @warning	
  ******************************************************************************  
  * 
  * 
  ******************************************************************************
  */
/*-- includes ----------------------------------------------------------------*/
#include "./main.h"
#include "./bms.h"
#include "./gasgauge.h"
#include "./led.h"
#include "./commun.h"

/*-- defined -----------------------------------------------------------------*/
#define   DB_LOG(fmt, ...)       LOG("[BMS]");LOG(fmt, ##__VA_ARGS__)






/*-- typedef -----------------------------------------------------------------*/



/*-- private variables -------------------------------------------------------*/
static   CommunData   communData;



/*-- functions ---------------------------------------------------------------*/
/** @function
	* @brief
	* @param
	* @return 
	*/
void   bms_init(void)
{
  commun_data_register(&communData);
}  


/** @function
	* @brief
	* @param
	* @return 
	*/
void   bms_loop(void)
{
  u8_t   soc = 0;
  u16_t  volt = 0;
  u16_t  cap = 0;
  s16_t  curr = 0;
  u16_t  temper = 0;
  
  led2_toggle();
  
  if(drv_bq40zxx_get_relative_soc(&soc) == OPERATE_OK)
  {
    led1_toggle();
    communData.rSoc++;// = soc;
  }

  if(drv_bq40zxx_get_design_capacity(&cap) == OPERATE_OK)
  {
    communData.designCap = cap;
  }
  
  if(drv_bq40zxx_get_sum_of_volt(&volt) == OPERATE_OK)
  {
    communData.battVolt = 10421;//volt;
  }
  
  if(drv_bq40zxx_get_remain_capacity(&cap) == OPERATE_OK)
  {
    communData.remainCap = cap;
  }
  
  if(drv_bq40zxx_get_current(&curr) == OPERATE_OK)
  {
    communData.current = 5;//curr;
  }
  
  if(drv_bq40zxx_get_cell1_volt(&volt) == OPERATE_OK)
  {
    communData.cellVolt[0] = 3300;//volt;
  }
  
  if(drv_bq40zxx_get_cell2_volt(&volt) == OPERATE_OK)
  {
    communData.cellVolt[1] = 3321;//volt;
  }
  
  if(drv_bq40zxx_get_cell3_volt(&volt) == OPERATE_OK)
  {
    communData.cellVolt[2] = 3324;//volt;
  }
  
  if(drv_bq40zxx_get_temper(&temper) == OPERATE_OK)
  {
    communData.temper = 2742;//temper;
  }
  
  HAL_Delay(500);
  
}  

/*------------------ end of file ---------------------------------------------*/

