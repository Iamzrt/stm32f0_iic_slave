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

#ifndef   HAL_TIMER_H
#define   HAL_TIMER_H

/* C++ */
#ifdef __cplusplus
extern "C" {
#endif
  
  
/*-- includes ----------------------------------------------------------------*/

 

  
/*-- typedef  ----------------------------------------------------------------*/
typedef  void  (*HalTimerUpdateIntCallBack)(void);
	
	
  
/*-- External functions ------------------------------------------------------*/  
void 								hal_timer6_init(void);
SysStatusType  			hal_timer6_update_callback_register(HalTimerUpdateIntCallBack call);


#endif
  
#ifdef __cplusplus
}
#endif


/*------------------ end of file ---------------------------------------------*/
