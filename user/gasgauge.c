/**
  ******************************************************************************
  * @file     gasgauge.c
  * @author   ZRT
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
#include "./gasgauge.h"


/*-- defined -----------------------------------------------------------------*/
#define   DB_LOG(fmt, ...)       //LOG("[GAS]");LOG(fmt, ##__VA_ARGS__)


#define   SMBUS_IO_ACK                    1
#define   SMBUS_IO_NO_ACK                 0


#define   DEFAULT_ERR_COUNT_MAX           (4) 

#define   MAX_BLOCK_READ                  96   

#define   CRC_KEY    7


/*-- typedef -----------------------------------------------------------------*/
typedef enum
{
	SMBUS_IO_W_0 = 0,                /* write 0 pin(output mode) */
	SMBUS_IO_W_1 = 1,                /* write 1 pin(output mode) */
	SMBUS_IO_READ,                   /* read pin(input mode) */
	SMBUS_IO_IN_PULLUP,              /* set input mode pullup on */
	SMBUS_IO_OUT_NOPULL_OD,          /* set output mode  pullup off */
}SmbusIOCtrlEnum;


/*-- private variables -------------------------------------------------------*/



/*-- functions ---------------------------------------------------------------*/


/*-- private variables -------------------------------------------------------*/
static     u8_t   bufferRW[MAX_BLOCK_READ];



/*-- functions ---------------------------------------------------------------*/

/* delay time to adjust iic speed. */
static  void  gpio_iic_delay(u32_t delay)
{
	u32_t i=0;
	
	for(i=0; i<(20+delay); i++)
  {
    ;
  }
}



/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static u32_t gpio_iic_scl_ctrl(SmbusIOCtrlEnum  opr)
{
	u32_t    ret = 0;
  HalChangeIOStruct io;

  io.gpio = SMB_CLK_GPIO;
	if (opr == SMBUS_IO_W_0)
	{   
	  io.mode = MODE_OUTPUT_PP;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
		hal_gpio_write(SMB_CLK_GPIO, O_LOW_LEVEL);
	}else if (opr == SMBUS_IO_W_1)
	{
    io.mode = MODE_OUTPUT_PP;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
		hal_gpio_write(SMB_CLK_GPIO, O_HIGH_LEVEL);
	}else if (opr == SMBUS_IO_READ)
	{
		ret = hal_gpio_read(SMB_CLK_GPIO);
	}else if (opr == SMBUS_IO_IN_PULLUP)
	{
    io.mode = MODE_INPUT;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);    
	}else if (opr == SMBUS_IO_OUT_NOPULL_OD)
	{
	  io.mode = MODE_OUTPUT_PP;
    io.pupd = NOPULL;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);      
	}

	return ret;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
static u32_t gpio_iic_sda_ctrl(SmbusIOCtrlEnum  opr)
{
	u32_t    ret = 0;
  HalChangeIOStruct io;

  io.gpio = SMB_DAT_GPIO;
  
	if (opr == SMBUS_IO_W_0)
	{
	  io.mode = MODE_OUTPUT_PP;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);    
		hal_gpio_write(SMB_DAT_GPIO, O_LOW_LEVEL);
	}else if (opr == SMBUS_IO_W_1)
	{		
	  io.mode = MODE_OUTPUT_PP;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);    
		hal_gpio_write(SMB_DAT_GPIO, O_HIGH_LEVEL);
	}else if (opr == SMBUS_IO_READ)
	{   
		ret = hal_gpio_read(SMB_DAT_GPIO);
	}else if (opr == SMBUS_IO_IN_PULLUP)
	{
    io.mode = MODE_INPUT;
    io.pupd = PULLUP;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);    
	}else if (opr == SMBUS_IO_OUT_NOPULL_OD)
	{
	  io.mode = MODE_OUTPUT_PP;
    io.pupd = NOPULL;
	  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);  
	}

	return ret;
}


/**             
  * @brief     
  * @param                
  * @return      
  */
static   void  i2c_io_start(void)
{    
	gpio_iic_sda_ctrl(SMBUS_IO_W_1);
	gpio_iic_scl_ctrl(SMBUS_IO_W_1);
	gpio_iic_delay(2);
	
	gpio_iic_sda_ctrl(SMBUS_IO_W_0);
	gpio_iic_delay(1);
	
	gpio_iic_scl_ctrl(SMBUS_IO_W_0);
	gpio_iic_delay(1);
	
}

/**             
  * @brief     
  * @param                
  * @return      
  */
static   void  i2c_io_stop(void)
{	 
	gpio_iic_delay(1);
	gpio_iic_sda_ctrl(SMBUS_IO_W_0);
	gpio_iic_delay(1);
	
	gpio_iic_scl_ctrl(SMBUS_IO_W_1);
	gpio_iic_delay(1);

	gpio_iic_sda_ctrl(SMBUS_IO_W_1);
	gpio_iic_delay(1);
}



/**             
  * @brief     
  * @param                
  * @return      
  */
static  u32_t   i2c_io_write(u8_t dat)
{
	u32_t     retry = 0;
	u32_t     i;
	u32_t     ret       = SMBUS_IO_NO_ACK;
		 
	gpio_iic_scl_ctrl(SMBUS_IO_W_0);
	gpio_iic_delay(1);
	
	for (i=0; i<8u; i++)
	{
		if ((dat & 0x80) == 0)
		{
			 gpio_iic_sda_ctrl(SMBUS_IO_W_0);
		}else
		{
			 gpio_iic_sda_ctrl(SMBUS_IO_W_1);
		}
		
		gpio_iic_delay(1); //adjust for smbus  
		gpio_iic_scl_ctrl(SMBUS_IO_W_1);
		
		/* Clock Stretching. */
		retry = 1000;  //for debug
		while(gpio_iic_scl_ctrl(SMBUS_IO_READ) == 0)
		{
			gpio_iic_delay(10); 
			if(retry > 0)   retry--;
			else            break;
		}
		
		gpio_iic_delay(1);  //adjust for smbus 
		gpio_iic_scl_ctrl(SMBUS_IO_W_0); 
		
		dat <<= 1;      
	}
	
	gpio_iic_delay(1);
	gpio_iic_sda_ctrl(SMBUS_IO_IN_PULLUP);
	gpio_iic_scl_ctrl(SMBUS_IO_W_1);
	gpio_iic_delay(1);
	
	/* Clock Stretching. */
	//gpio_iic_delay(5);
	retry = 5000;
	while(gpio_iic_scl_ctrl(SMBUS_IO_READ) == 0)
	{
		gpio_iic_delay(10); 
		if(retry > 0)   retry--;
		else            break;
	}
	
	//gpio_iic_delay(50);
	if (gpio_iic_sda_ctrl(SMBUS_IO_READ) == 0)
	{
		ret = SMBUS_IO_ACK;
	}


	gpio_iic_delay(1);
	gpio_iic_scl_ctrl(SMBUS_IO_W_0); 
	gpio_iic_sda_ctrl(SMBUS_IO_OUT_NOPULL_OD);
	gpio_iic_sda_ctrl(SMBUS_IO_W_0); 
	gpio_iic_delay(1);
	
	return ret;
}


/**             
  * @brief     
  * @param                
  * @return      
  */
static  u8_t   i2c_io_read(BOOL bAck)
{
	u32_t     retry = 0;
	u32_t     i;
	u8_t      rDate = 0;

	gpio_iic_sda_ctrl(SMBUS_IO_W_1);
	gpio_iic_sda_ctrl(SMBUS_IO_IN_PULLUP);

	for (i=0; i<8u; i++)
	{
		gpio_iic_delay(1); 
		gpio_iic_scl_ctrl(SMBUS_IO_W_1);
		gpio_iic_delay(1); 
		
		/* Clock Stretching. */
		retry = 1000;  //for debug
		while(gpio_iic_scl_ctrl(SMBUS_IO_READ) == 0)
		{
			gpio_iic_delay(10); 
			if(retry > 0)   retry--;
			else            break;
		}
	
		rDate  <<=  1;
		if (gpio_iic_sda_ctrl(SMBUS_IO_READ) == 1)
		{
			rDate |= 0x01;
		}
		gpio_iic_scl_ctrl(SMBUS_IO_W_0); 
	}

	gpio_iic_sda_ctrl(SMBUS_IO_OUT_NOPULL_OD);
	gpio_iic_delay(1);
	
	if (bAck == SMBUS_IO_ACK)
	{
		gpio_iic_sda_ctrl(SMBUS_IO_W_0); 
		gpio_iic_delay(1);
		gpio_iic_scl_ctrl(SMBUS_IO_W_1);
		gpio_iic_delay(1);
		gpio_iic_scl_ctrl(SMBUS_IO_W_0); 
		gpio_iic_delay(1);               
	}else
	{
		gpio_iic_sda_ctrl(SMBUS_IO_W_1); 
		gpio_iic_delay(1);
		gpio_iic_scl_ctrl(SMBUS_IO_W_1);
		gpio_iic_delay(1);
		gpio_iic_scl_ctrl(SMBUS_IO_W_0);
		gpio_iic_delay(1);
		gpio_iic_sda_ctrl(SMBUS_IO_W_0); 
		gpio_iic_delay(1);
	}

	return  rDate;  
}


/**             
  * @brief     
  * @param                
  * @return      
  */
u32_t   co_gpio_smbus_write_buff(u8_t dev, u8_t wAdr, const u8_t* pBuff, u32_t  numByteToWrite)
{
	u32_t  errCnt = 0;
	u32_t  ret    = 1;

	for (errCnt=0; errCnt<DEFAULT_ERR_COUNT_MAX; errCnt++)
	{
		i2c_io_start();

		if (i2c_io_write(dev&0xfe) == SMBUS_IO_NO_ACK)
		{
			i2c_io_stop();
			DB_LOG("err! dev(0x%x) write no ack.cnt %d\r\n", dev, errCnt);
		}
		else
		{
			if (i2c_io_write(wAdr) == SMBUS_IO_NO_ACK)
			{
				i2c_io_stop();
				DB_LOG("err2! wAdr(0x%x) write no ack.(%d)\r\n", wAdr, errCnt);    
			}
			else
			{			
				break;
			}
		}
	}
	
	if (errCnt == DEFAULT_ERR_COUNT_MAX)
	{
		return 0;
	}

	while(numByteToWrite)  
	{
		if (i2c_io_write(*pBuff) == SMBUS_IO_NO_ACK)
		{
				DB_LOG("w data Not getAck 0x%x\r\n", dev);
				ret = 0;
		}
		pBuff++; 
		numByteToWrite--;
	}

	i2c_io_stop();

	return  ret;
}



/**             
  * @brief     
  * @param                
  * @return      
  */
u32_t   co_gpio_smbus_write_buff_PEC(u8_t dev, u8_t wAdr, const u8_t* pBuff, u32_t  numByteToWrite)
{
	u8_t  valCRC = 0;
	u8_t  i = 0;
	
	/*  CRC calculate:
			1: addrW + CMD + data1 + data2 + dataN 
			2: addrW + CMD + addrR + data1 + data2 + dataN
	*/
	bufferRW[0] = dev;
	bufferRW[1] = wAdr;
	for(i=0; i<numByteToWrite; i++)
	{
		bufferRW[2+i] = pBuff[i];
	}
	valCRC = CRC8(bufferRW, numByteToWrite+2, CRC_KEY);
	bufferRW[2+numByteToWrite] = valCRC;
	
	if(co_gpio_smbus_write_buff(dev, wAdr, &bufferRW[2], numByteToWrite+1) == 0)
	{
		return 0;
	}
	
	return 1;
}


/**             
  * @brief     
  * @param                
  * @return      
  */
u32_t   co_gpio_smbus_read_buff(u8_t dev, u8_t wAdr, u32_t  numByteToRead, u8_t* pBuff)
{
	u32_t  errCnt = 0;

	for (errCnt=0; errCnt<DEFAULT_ERR_COUNT_MAX; errCnt++)
	{	
		i2c_io_start();
		if (i2c_io_write(dev&0xfe) == SMBUS_IO_NO_ACK)  /* write */
		{
			i2c_io_stop();
			DB_LOG("err1! dev(0x%x) read no ack.(%d)\r\n", dev, errCnt);    
		}
		else
		{			
			if (i2c_io_write(wAdr) == SMBUS_IO_NO_ACK)
			{
				i2c_io_stop();
				DB_LOG("err2! wAdr(0x%x) read no ack.(%d)\r\n", wAdr, errCnt);    
			}
			else
			{		
				i2c_io_start();
				if (i2c_io_write(dev|0x01) == SMBUS_IO_NO_ACK)  /* read */
				{
					i2c_io_stop();
					DB_LOG("err3! dev(0x%x) read no ack.(%d)\r\n", dev, errCnt);    
				}else
				{					
					break;
				}
				
			}
		}
	}
	
	if (errCnt == DEFAULT_ERR_COUNT_MAX)
	{
		i2c_io_stop();
		return 0;
	}

	while(numByteToRead)
	{   
		numByteToRead--;

		if(numByteToRead)
		{
			*pBuff = i2c_io_read(TRUE);
		}else
		{
			*pBuff = i2c_io_read(FALSE);
		}

		pBuff++;
	}

	i2c_io_stop();

	return  1;
}




/**             
  * @brief     
  * @param                
  * @return      
  */
u32_t   co_gpio_smbus_read_buff_PEC(u8_t dev, u8_t wAdr, u32_t  numByteToRead, u8_t* pBuff)
{
	u8_t  valCRC = 0;
	u8_t  retVal = 0;

	if(numByteToRead >= MAX_BLOCK_READ)
	{
		return 0;
	}

	for(u8_t i=0; i<MAX_BLOCK_READ; i++)
	{
		bufferRW[i] = 0;
	}
	
	/*  CRC calculate:
			1: addrW + CMD + data1 + data2 + dataN 
			2: addrW + CMD + addrR + data1 + data2 + dataN
	*/
	bufferRW[0] = dev;
	bufferRW[1] = wAdr;
	bufferRW[2] = dev|0x01;
	retVal = co_gpio_smbus_read_buff(dev, wAdr, 1+numByteToRead, &bufferRW[3]);	
	if(retVal == 0)
	{
		return 0;
	}

	valCRC = CRC8(bufferRW, numByteToRead+3, CRC_KEY);

	if (valCRC != bufferRW[numByteToRead+3])
	{
		return 0;
	}
	
	for(u8_t i=0; i<numByteToRead; i++)
	{
		pBuff[i] = bufferRW[3+i];
	}
	
	return 1;
}



/** @function
	* @brief      disable  fet
	* @param
	* @return 
	* @note
	*/
void  drv_bq40zxx_PRES_high(void)
{
//	HalChangeIOStruct io;
//	
//  io.gpio = A_BQ_PRES_GPIO;
//  io.mode = MODE_OUTPUT_PP;
//  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
//  hal_gpio_write(A_BQ_PRES_GPIO, O_HIGH_LEVEL);
}


void  drv_bq40zxx_PRES_float(void)
{
//	HalChangeIOStruct io;

//	io.gpio = A_BQ_PRES_GPIO;
//	io.mode = MODE_INPUT;
//	hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
}

/** @function
	* @brief      enable  fet
	* @param
	* @return 
	* @note
	*/
void  drv_bq40zxx_PRES_low(void)
{
//	HalChangeIOStruct io;
//	
//  io.gpio = A_BQ_PRES_GPIO;
//  io.mode = MODE_OUTPUT_PP;
//  hal_gpio_ctrl(CHANGE_IO_MODE, (void*)&io);
//  hal_gpio_write(A_BQ_PRES_GPIO, O_LOW_LEVEL); 
}



/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
static  SysStatusType  bq40zxx_read_buff(u8_t wAdr, u32_t  numByteToRead, u8_t* pBuff)
{
	SysStatusType status = OPERATE_OK;

	if(co_gpio_smbus_read_buff_PEC(BQ40ZX_DEV_ADDR, wAdr, numByteToRead, pBuff) == 0)
	{
		status =  OPERATE_FAIL;
	}
	
	return status;
}


/** 
  * @brief   
  * @param    
  * @param    
  * @return  
  */
static  SysStatusType  bq40zxx_write_buff(u8_t wAdr, u32_t  numByteToRead, u8_t* pBuff)
{
	SysStatusType status = OPERATE_OK;

	if(co_gpio_smbus_write_buff_PEC(BQ40ZX_DEV_ADDR, wAdr, pBuff, numByteToRead) == 0)
	{
		status =  OPERATE_FAIL;
	}
	
	return status;
}




/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_df_read(u16_t dfAddr, u8_t data[32])
{
	u8_t cnt = 0;
	u8_t buff[32+3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = dfAddr&0xff;
	buff[2] = (dfAddr>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	if(bq40zxx_read_buff(BLOCK_ACCESS_CMD, 32+3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	/* buff[0]=len, buff[1]=addr low 8 bits, buff[2]=addr high 8 bits, buff[3]~ = data.*/
	if((buff[1] != (dfAddr&0xff)) || (buff[2] != ((dfAddr>>8)&0xff)))
	{
	  return OPERATE_FAIL;
	}
	
  for(cnt=0; cnt<32; cnt++)	
	{
		data[cnt] = buff[3+cnt];
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_df_write(u16_t dfAddr, u8_t* data, u8_t size)
{
	u8_t cnt = 0;
	u8_t buff[32+3];
	
	if(size > 32)			return OPERATE_FAIL;
	
	buff[0] = 2+size; /* number to write. */
	buff[1] = dfAddr&0xff;
	buff[2] = (dfAddr>>8)&0xff;
	
	for(cnt=0; cnt<size; cnt++)
	{
		buff[3+cnt] = data[cnt]; 
	}
	
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3+size, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_seal_dev(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_SEAL_DEV_CMD&0xff;
	buff[2] = (MAC_SEAL_DEV_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_unseal_dev(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = DEFAULT_UNSEAL_KEY1&0xff;
	buff[2] = (DEFAULT_UNSEAL_KEY1>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	buff[0] = 2; /* number to write. */
	buff[1] = DEFAULT_UNSEAL_KEY2&0xff;
	buff[2] = (DEFAULT_UNSEAL_KEY2>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note    If you want to get into full access mode, you have to get into unseal mode first
	*/
SysStatusType  drv_bq40zxx_full_access_dev(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = FULL_ACCESS_KEY1&0xff;
	buff[2] = (FULL_ACCESS_KEY1>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	buff[0] = 2; /* number to write. */
	buff[1] = FULL_ACCESS_KEY2&0xff;
	buff[2] = (FULL_ACCESS_KEY2>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     0x16 0x44 0x02 0x22 0x00 0xE8
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_fet_ctrl(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_FET_CTRL_CMD&0xff;
	buff[2] = (MAC_FET_CTRL_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_shutdown(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_SHUTDOWN_CMD&0xff;
	buff[2] = (MAC_SHUTDOWN_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief     
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_sleep(void)
{
	u8_t buff[3];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_SLEEP_CMD&0xff;
	buff[2] = (MAC_SLEEP_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_operation_status(u32_t* status)
{
	u8_t buff[1+6];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_OPERATE_STATUS_CMD&0xff;
	buff[2] = (MAC_OPERATE_STATUS_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	if(bq40zxx_read_buff(BLOCK_ACCESS_CMD, 1+6, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*status = buff[3] | (buff[4]<<8) | (buff[5]<<16) | (buff[6]<<24);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_manuf_status(u16_t* status)
{
	u8_t buff[1+4];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_MANUF_STATUS_CMD&0xff;
	buff[2] = (MAC_MANUF_STATUS_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	if(bq40zxx_read_buff(BLOCK_ACCESS_CMD, 1+4, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*status = buff[3] | (buff[4]<<8);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_safety_status(u32_t* status)
{
	u8_t buff[1+6];
	
	buff[0] = 2; /* number to write. */
	buff[1] = MAC_SAFETY_STATUS_CMD&0xff;
	buff[2] = (MAC_SAFETY_STATUS_CMD>>8)&0xff;
  if(bq40zxx_write_buff(BLOCK_ACCESS_CMD, 3, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	if(bq40zxx_read_buff(BLOCK_ACCESS_CMD, 1+6, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*status = buff[3] | (buff[4]<<8) | (buff[5]<<16) | (buff[6]<<24);
	
	return OPERATE_OK;
}



/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_cell1_volt(u16_t* volt)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(0x3F, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}  
	*volt = (u16_t)((buff[1]<<8) | buff[0]);
  
	return OPERATE_OK;
}

SysStatusType  drv_bq40zxx_get_cell2_volt(u16_t* volt)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(0x3E, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}  
	*volt = (u16_t)((buff[1]<<8) | buff[0]);
  
	return OPERATE_OK;
}

SysStatusType  drv_bq40zxx_get_cell3_volt(u16_t* volt)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(0x3D, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}  
	*volt = (u16_t)((buff[1]<<8) | buff[0]);
  
	return OPERATE_OK;
}

/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_temper(u16_t* temper)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(0x08, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}  
  
	*temper = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}




/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_battery_status(u16_t* status)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(BATTERY_STATUS_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*status = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}



/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_cycle_count(u16_t* count)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(CYCLE_COUNT_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*count = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_relative_soc(u8_t* soc)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(RE_SOC_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*soc = buff[0];
	
	return OPERATE_OK;
}

/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_absolute_soc(u8_t* soc)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(AB_SOC_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*soc = buff[0];
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_soh(u8_t* soh)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(SOH_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*soh = buff[0];
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_full_capacity(u16_t* cap)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(FULL_CHG_CAP_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*cap = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_design_capacity(u16_t* cap)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(DESIGN_CAP_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*cap = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_remain_capacity(u16_t* cap)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(REMAIN_CAP_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*cap = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_sum_of_volt(u16_t* volt)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(SUM_OF_VOLT_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*volt = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_current(s16_t* curr)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(CURRENT_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*curr = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}


/** @function
	* @brief
	* @param
	* @return 
	* @note
	*/
SysStatusType  drv_bq40zxx_get_avg_current(s16_t* curr)
{
	u8_t buff[2];
	
  if(bq40zxx_read_buff(CURRENT_CMD, 2, buff) != OPERATE_OK)
	{
	  return OPERATE_FAIL;
	}
	
	*curr = (u16_t)((buff[1]<<8) | buff[0]);
	
	return OPERATE_OK;
}




/*------------------ end of file ---------------------------------------------*/

