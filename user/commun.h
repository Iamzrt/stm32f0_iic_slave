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
#ifndef  COMMUN_H
#define  COMMUN_H


#include "./main.h"


	
/*-- defined  -----------------------------------------------------------------*/

#define     READ_REMAIN_CAP_ALARM_CMD        0x01
#define     READ_REMAIN_TIME_ALARM_CMD       0x02
#define     READ_TEMPER_CMD                  0x08
#define     READ_BATT_VOLT_CMD               0x09
#define     READ_CURRENT_CMD                 0x0A
#define     READ_AVG_CURRENT_CMD             0x0B
#define     READ_RSOC_CMD                    0x0D
#define     READ_ASOC_CMD                    0x0E
#define     READ_REMAIN_CAP_CMD              0x0F
#define     READ_FULL_CAP_CMD                0x10
#define     READ_CHG_RATED_CURRENT_CMD       0x14
#define     READ_CHG_RATED_VOLT_CMD          0x15
#define     READ_BATT_STATUS_CMD             0x16
#define     READ_CYCCLE_CMD                  0x17
#define     READ_DESIGN_CAP_CMD              0x18
#define     READ_DESIGN_VOLT_CMD             0x19
#define     READ_MANUF_DATE_CMD              0x1B
#define     READ_SERIAL_NUM_CMD              0x1C
#define     READ_MANUF_NAME_CMD              0x20
#define     READ_DEV_NAME_CMD                0x21
#define     READ_DEV_CHEM_CMD                0x22
#define     READ_MANUF_DATA_CMD              0x23
#define     READ_CELL1_VOLT_CMD              0x3F
#define     READ_CELL2_VOLT_CMD              0x3E
#define     READ_CELL3_VOLT_CMD              0x3D
#define     READ_FIREWARE_CMD                0x7F
#define     READ_AUTH_DATA_CMD               0x37
#define     READ_SHUTDOWN_REMIND_CMD         0xFD
#define     LED_CTRL_CMD                     0x39
#define     SELF_DSG_CTRL_CMD                0x40
#define     SELF_DSG_ON_OFF_CMD              0x41
#define     DEEP_SLEEP_CMD                   0x42
#define     TRANSPORT_PROT_CTRL_CMD          0xEE



typedef  struct
{
  u8_t     rSoc;
  u16_t    battVolt;
  u16_t    designCap;
  u16_t    remainCap;
  s16_t    current;
  u16_t    cellVolt[3];
  u16_t    temper;
}CommunData;



/*-- function  ----------------------------------------------------------------*/
extern   void    commun_init(void);
extern   void    commun_data_register(CommunData* dataPt); 


#endif

/*------------------ end of file ---------------------------------------------*/

