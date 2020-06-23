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



//static   u16_t   stmFlashBuff [HAL_SECTOR_SIZE/2];   /* Max 2K bytes. */



///*-- functions ---------------------------------------------------------------*/

///** 
//  * @brief   
//  * @param    addr: read address
//  * @return   flash value,a word
//  */
//u32_t hal_flash_read_word(u32_t addr)
//{
//  return *(__IO u32_t *)addr;
//}  


///** 
//  * @brief   
//  * @param    addr: read address
//  * @return   flash value,a half word
//  */
//u16_t hal_flash_read_half_word (u32_t addr)
//{
//  return *(__IO u16_t*)addr; 
//}

///** 
//  * @brief   
//  * @param    addr: read address
//  * @return   flash value,a byte
//  */
//u8_t hal_flash_read_byte(u32_t addr)
//{
//  return *(__IO u8_t *)addr;
//}  



///** 
//  * @brief   
//  * @param    
//  * @return   
//  */
//void hal_flash_read_byte_buff(u32_t addr, u8_t *pBuffer, u32_t num)   	
//{
//	u8_t i;

//	for(i=0; i<num; i++)
//	{
//		pBuffer[i] = (u8_t)hal_flash_read_byte(addr);
//		addr += 1;
//	}
//}


///** 
//  * @brief   
//  * @param    
//  * @return   
//  */
//void hal_flash_read_half_word_buff(u32_t addr, u16_t *pBuffer, u32_t num)   	
//{
//	u16_t i;
//	
//	for(i=0; i<num; i++)
//	{
//		pBuffer[i] = hal_flash_read_half_word(addr);
//		addr += 2;
//	}
//}



///** 
//  * @brief   
//  * @param    
//  * @return   
//  */
//void hal_flash_read_word_buff(u32_t addr, u32_t *pBuffer, u32_t num)   	
//{
//	u32_t i;
//	
//	for(i=0; i<num; i++)
//	{
//		pBuffer[i]=hal_flash_read_word(addr);
//		addr += 4;
//	}
//}


///** 
//  * @brief   
//  * @param    
//  * @return   
//  */
//SysStatusType  hal_flash_erase(u32_t startAddr, u16_t num)
//{
//	SysStatusType ret = OPERATE_OK;
//	FLASH_EraseInitTypeDef  EraseInitStruct;
//	u32_t  pageError;

//	/* Unlock the Flash to enable the flash control register access *************/
//  HAL_FLASH_Unlock();
//	
//	/* Fill EraseInit structure*/
//	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
//	EraseInitStruct.PageAddress = startAddr;
//	EraseInitStruct.NbPages = num;  //(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
//	if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK)
//	{
//		return OPERATE_FAIL;
//	}
//	
//	/* Lock the Flash to disable the flash control register access (recommended
//     to protect the FLASH memory against possible unwanted operation) *********/
//  HAL_FLASH_Lock();
//	
//	return ret;
//}


///** 
//  * @brief   No check to write.
//	* @param   addr: start address.
//	* @param   pBuffer: data pointer.
//	* @param   num: 
//  * @return  None.
//  */
//static void hal_flash_write_halfword_no_check (u32_t addr, u16_t * pBuffer, u16_t num)   
//{                                           
//	u16_t i;        

//	for(i=0; i<num; i++)
//	{
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr,pBuffer[i]);
//		addr += 2;                                  
//	}  
//} 


///** 
//  * @brief   stm32 flash can only write 16-bit data.
//	* @param   addr: start address(The address must be a multiple of 2.).
//	* @param   pBuffer: data pointer.
//	* @param   num: 
//  * @return   
//  */
//void  hal_flash_write_half_word_buff( u32_t addr, u16_t * pBuffer, u16_t num )        
//{
//	u16_t sectOffset;          
//	u16_t sectRemain;          
//	u16_t i;    
//	u32_t sectPos;           
//	u32_t offAddr;   /* base 0x08000000 */

//	if(addr<FLASH_BASE||(addr>=(FLASH_BASE+1024*HAL_FLASH_SIZE)))  return;  /* illegal addr. */

//	HAL_FLASH_Unlock();                                               
//	
//	offAddr = addr-FLASH_BASE;             
//	sectPos = offAddr/HAL_SECTOR_SIZE;                        
//	sectOffset = (offAddr%HAL_SECTOR_SIZE)/2;                
//	sectRemain = HAL_SECTOR_SIZE/2 - sectOffset;                 
//	if(num <= sectRemain)
//	{
//		sectRemain=num;
//	}

//	while(1) 
//	{        
//		/* Read the whole page data. */
//		hal_flash_read_half_word_buff(sectPos*HAL_SECTOR_SIZE+FLASH_BASE, stmFlashBuff, HAL_SECTOR_SIZE/2);
//		for(i=0; i<sectRemain; i++)
//		{
//			if(stmFlashBuff[sectOffset+i] != 0xFFFF)
//			{ /* need to erase. */
//				break;   
//			}				
//		}
//		
//		if(i < sectRemain)
//		{
//			/* need to erase. */
//			hal_flash_erase(sectPos*HAL_SECTOR_SIZE+FLASH_BASE, 1);
//			HAL_FLASH_Unlock();
//			
//			for(i=0;i<sectRemain;i++)
//			{ /* copy */
//				stmFlashBuff[i+sectOffset] = pBuffer[i];          
//			}
//			hal_flash_write_halfword_no_check(sectPos*HAL_SECTOR_SIZE+FLASH_BASE,stmFlashBuff,HAL_SECTOR_SIZE/2);
//		}
//		else 
//		{
//			hal_flash_write_halfword_no_check(addr,pBuffer,sectRemain); 
//		}
//		
//		if(num == sectRemain)
//		{ /* Write finish. */
//			break;
//		}
//		else
//		{
//			sectPos++;        /* Page++. */
//			sectOffset = 0;                                         
//			pBuffer += sectRemain;      
//			//addr += sectRemain; 
//			addr += (sectRemain*2);			
//			num -= sectRemain;      
//			if(num > (HAL_SECTOR_SIZE/2))
//			{
//				sectRemain = HAL_SECTOR_SIZE/2;
//			}
//			else 
//			{
//				sectRemain = num;
//			}
//		}         
//	};      
//  
//	HAL_FLASH_Lock();
//}


/*------------------ end of file ---------------------------------------------*/
