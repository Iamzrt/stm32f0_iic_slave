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

#ifndef   HAL_FLASH_H
#define   HAL_FLASH_H

/* C++ */
#ifdef __cplusplus
extern "C" {
#endif
  
  
/*-- includes ----------------------------------------------------------------*/
	
	
/*-- defined ----------------------------------------------------------------*/
#define HAL_FLASH_SIZE        256   /* STM32 FLASH capacity(unit: K) */

#if HAL_FLASH_SIZE < 256
  #define HAL_SECTOR_SIZE         1024   /* bytes */
#else 
  #define HAL_SECTOR_SIZE         2048
#endif
 

  
/*-- typedef  ----------------------------------------------------------------*/





/*-- External functions ------------------------------------------------------*/  
u8_t 						hal_flash_read_byte(u32_t addr);	
u16_t 					hal_flash_read_half_word (u32_t addr);
u32_t 					hal_flash_read_word(u32_t addr);
void 						hal_flash_read_byte_buff(u32_t addr, u8_t *pBuffer, u32_t num);
void 						hal_flash_read_half_word_buff(u32_t addr, u16_t *pBuffer, u32_t num);
void 						hal_flash_read_word_buff(u32_t addr, u32_t *pBuffer, u32_t num);
void  					hal_flash_write_half_word_buff( u32_t addr, u16_t * pBuffer, u16_t num );
SysStatusType  	hal_flash_erase(u32_t startAddr, u16_t num);


  
#ifdef __cplusplus
}
#endif



#endif
/*------------------ end of file ---------------------------------------------*/
