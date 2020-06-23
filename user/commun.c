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
#include "./commun.h"

/*-- defined -----------------------------------------------------------------*/
#define   DB_LOG(fmt, ...)       LOG("[COMM]");LOG(fmt, ##__VA_ARGS__)


#define     POLYNOME                  0x07

#define     I2C_REC_BYTES            64
#define     I2C_TX_BYTES             64




typedef  struct
{
  BOOL      cmdNotify;
  u8_t      txCmd;
  u8_t      txLen;
  u8_t      txCounter;
  u8_t      rxCounter; 
}Commun;


/*-- private variables -------------------------------------------------------*/
static   u8_t     slaveTxBuff[I2C_TX_BYTES] = {0xE0, 0x2E};
static   u8_t     slaveRxBuff[I2C_REC_BYTES] = {0};

static   Commun       commun;
static   CommunData*  communDataPt = NULL;


/*-- private functions -------------------------------------------------------*/


/*-- functions ---------------------------------------------------------------*/
static   void   commun_tx_match(u8_t cmd);



/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_addr_cplt_callback(u32_t transferDir, u16_t addrMatchCode)
{
  if(transferDir)
  {   
#if 1
    i2c1_tx_trig(slaveTxBuff[0]);
    commun.txCounter = 1; 
#else
    commun.txCounter = 0;
    i2c1_tx_trig(slaveTxBuff[commun.txCounter]);
    commun.txCounter++;     
#endif    
  }  
  else
  {
    commun.rxCounter = 0;
  }
}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_rx_cplt_callback(u8_t dat)
{
  if(commun.rxCounter < I2C_REC_BYTES)  
  {
    slaveRxBuff[commun.rxCounter] = dat;
    
    /* Get the comand. */
    if(commun.rxCounter == 0)
    {
      commun.txCmd = dat;
      commun_tx_match(commun.txCmd);
    }
    
    commun.rxCounter++;
  }
}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_tx_callback(void)
{
  if(commun.txCounter < I2C_TX_BYTES)
  {
    i2c1_putchar(slaveTxBuff[commun.txCounter++]);	
  } 
  else 
  {
    i2c1_putchar(0); 
  }   
}


/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_af_callback(void)
{

}

/** 
  * @brief   
  * @param     
  * @return  
  */
void  i2c1_stop_callback(void)
{
  commun.cmdNotify = TRUE;
  commun.txCounter = 0;
  commun.rxCounter = 0;
  commun.txLen = 0;
  commun.txCmd = 0xff;
}


/** @function  
  * @brief    
  * @param    
  * @return  
  */
void   commun_init(void)
{
  commun.cmdNotify = FALSE;
  commun.txCounter = 0;
  commun.rxCounter = 0;
  commun.txLen = 0; 
  commun.txCmd = 0xff;  
}


/** @function  
  * @brief    
  * @param    
  * @return  
  */
void  commun_data_register(CommunData* dataPt)
{
  communDataPt = dataPt;
}

/** @function  
  * @brief    
  * @param    
  * @return  
  */
static  void  commun_tx_match(u8_t cmd)
{
  u8_t  counter = 0;
  
  if(communDataPt == NULL)
    return;
  
  switch(cmd)
  {
    case READ_REMAIN_CAP_ALARM_CMD:
    {
      slaveTxBuff[counter++] = 0x2C;
      slaveTxBuff[counter++] = 0x01;  
    }break;
    
    case READ_REMAIN_TIME_ALARM_CMD:
    {
      slaveTxBuff[counter++] = 0x0A;
      slaveTxBuff[counter++] = 0x00;   
    }break;    
      
    case READ_TEMPER_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->temper&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->temper&0xff00)>>8;
    }break;
		
    case READ_BATT_VOLT_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->battVolt&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->battVolt&0xff00)>>8;
    }break;
    
    case READ_CURRENT_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->current&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->current&0xff00)>>8;  
    }break;
    
    case READ_AVG_CURRENT_CMD:
    {
      slaveTxBuff[counter++] = 0xE8;
      slaveTxBuff[counter++] = 0xFB;     
    }break;  

    case READ_RSOC_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->rSoc&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->rSoc&0xff00)>>8;    
    }break; 

    case READ_ASOC_CMD:
    {
      slaveTxBuff[counter++] = 0x50;
      slaveTxBuff[counter++] = 0x00;  
    }break;   

    case READ_REMAIN_CAP_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->remainCap&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->remainCap&0xff00)>>8;    
    }break;   

    case READ_FULL_CAP_CMD:
    {
      slaveTxBuff[counter++] = 0x26;
      slaveTxBuff[counter++] = 0x0F;   
    }break;  

    case READ_CHG_RATED_CURRENT_CMD:
    {
      slaveTxBuff[counter++] = 0xD8;
      slaveTxBuff[counter++] = 0x0E;    
    }break; 

    case READ_CHG_RATED_VOLT_CMD:
    {
      slaveTxBuff[counter++] = 0x1E;
      slaveTxBuff[counter++] = 0x2D;    
    }break; 
    
		case READ_BATT_STATUS_CMD:
    {
      slaveTxBuff[counter++] = 0xC0;
      slaveTxBuff[counter++] = 0x00;    
    }break;
    
    case READ_CYCCLE_CMD:
    {
      slaveTxBuff[counter++] = 0x0F;
      slaveTxBuff[counter++] = 0x00;   
    }break;
    
    case READ_DESIGN_CAP_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->designCap&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->designCap&0xff00)>>8;    
    }break;
    
    case READ_DESIGN_VOLT_CMD:
    {
      slaveTxBuff[counter++] = 0x1E;
      slaveTxBuff[counter++] = 0x2D;   
    }break;
    
    case READ_MANUF_DATE_CMD:
    {
      slaveTxBuff[counter++] = 0x09;
      slaveTxBuff[counter++] = 0x32;
      slaveTxBuff[counter++] = 0x30;   
      slaveTxBuff[counter++] = 0x31;
      slaveTxBuff[counter++] = 0x39;
      slaveTxBuff[counter++] = 0x2D;
      slaveTxBuff[counter++] = 0x38;
      slaveTxBuff[counter++] = 0x2D;
      slaveTxBuff[counter++] = 0x30;
      slaveTxBuff[counter++] = 0x38;
      slaveTxBuff[counter++] = 0x08;      
    }break;
    
    case READ_SERIAL_NUM_CMD:
    {
    
    }break;
    
    case READ_MANUF_NAME_CMD:
    {
    
    }break;
    
     case READ_DEV_NAME_CMD:
    {
    
    }break;
    
    case READ_DEV_CHEM_CMD:
    {
    
    }break;
    
    case READ_MANUF_DATA_CMD:
    {
    
    }break;  

    case READ_CELL1_VOLT_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->cellVolt[0]&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->cellVolt[0]&0xff00)>>8;     
    }break;     

    case READ_CELL2_VOLT_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->cellVolt[1]&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->cellVolt[1]&0xff00)>>8;      
    }break;  

    case READ_CELL3_VOLT_CMD:
    {
      slaveTxBuff[counter++] = communDataPt->cellVolt[2]&0x00ff;
      slaveTxBuff[counter++] = (communDataPt->cellVolt[2]&0xff00)>>8;      
    }break;   

    case READ_FIREWARE_CMD:
    {
    
    }break;   

    case READ_AUTH_DATA_CMD:
    {
    
    }break;  

    case READ_SHUTDOWN_REMIND_CMD:
    {
    
    }break; 

    case LED_CTRL_CMD:
    {
    
    }break;

    case SELF_DSG_CTRL_CMD:
    {
    
    }break;  

    case SELF_DSG_ON_OFF_CMD:
    {
    
    }break;    

    case DEEP_SLEEP_CMD:
    {
    
    }break; 

    case TRANSPORT_PROT_CTRL_CMD:
    {
    
    }break;      
    
    default:
      break;
  }

  slaveTxBuff[counter] = CRC8(slaveTxBuff, counter, POLYNOME);  
}


/*------------------ end of file ---------------------------------------------*/

