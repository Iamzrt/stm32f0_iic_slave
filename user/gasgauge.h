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
#ifndef  GASGAUGE_H
#define  GASGAUGE_H


	
/*-- defined  -----------------------------------------------------------------*/
#define       BQ40ZX_DEV_ADDR           (u8_t)0x16


#define       BQ40ZX_MAX_CELL_NUM                (u8_t)4
#define       BQ40ZX_MAX_TEMPER_NUM              (u8_t)4
#define       BQ40ZX_MAX_MANUF_INFO_NUM          (u8_t)32


#define       DEFAULT_UNSEAL_KEY1       (u16_t )0x0414
#define       DEFAULT_UNSEAL_KEY2       (u16_t )0x3672

#define       FULL_ACCESS_KEY1          (u16_t )0xFFFF
#define       FULL_ACCESS_KEY2          (u16_t )0xFFFF

/*< SBS Commands. >*/
#define       SUM_OF_VOLT_CMD           (u8_t )0x09
#define       CURRENT_CMD               (u8_t )0x0A
#define       AVG_CURRENT_CMD           (u8_t )0x0B
#define       RE_SOC_CMD                (u8_t )0x0D
#define       AB_SOC_CMD                (u8_t )0x0E
#define       REMAIN_CAP_CMD						(u8_t )0x0F
#define       FULL_CHG_CAP_CMD					(u8_t )0x10
#define       BATTERY_STATUS_CMD        (u8_t )0x16
#define       CYCLE_COUNT_CMD           (u8_t )0x17
#define       DESIGN_CAP_CMD           	(u8_t )0x18
#define       CELL4_VOLT_CMD           	(u8_t )0x3C
#define       CELL3_VOLT_CMD           	(u8_t )0x3D
#define       CELL2_VOLT_CMD           	(u8_t )0x3E
#define       CELL1_VOLT_CMD           	(u8_t )0x3F
#define       BLOCK_ACCESS_CMD          (u8_t )0x44
#define       SOH_CMD                   (u8_t )0x4F
#define       SAFETY_STATUS_CMD         (u8_t )0x51
#define       OPERATE_STATUS_CMD        (u8_t )0x54
#define       MANUF_STATUS_CMD          (u8_t )0x57
#define       MANUF_INFO_CMD						(u8_t )0x70
#define       DA_STATUS1_CMD						(u8_t )0x71
#define       DA_STATUS2_CMD						(u8_t )0x72
//#define       SOH_CMD						        (u8_t )0x77
#define       DA_STATUS3_CMD						(u8_t )0x7B


/* Manufacture Access command. */
#define       MAC_SHUTDOWN_CMD              (u16_t )0x0010
#define       MAC_SLEEP_CMD                 (u16_t )0x0011
#define       MAC_FET_CTRL_CMD              (u16_t )0x0022
#define       MAC_SEAL_DEV_CMD              (u16_t )0x0030
#define       MAC_SAFETY_STATUS_CMD         (u16_t )0x0051
#define       MAC_OPERATE_STATUS_CMD        (u16_t )0x0054
#define       MAC_MANUF_STATUS_CMD          (u16_t )0x0057
#define       MAC_DA_STATUS1_CMD						(u16_t )0x0071
#define       MAC_DA_STATUS2_CMD						(u16_t )0x0072
#define       MAC_SOH_CMD										(u16_t )0x0077
#define       MAC_DA_STATUS3_CMD						(u16_t )0x007B



/*-- function  ----------------------------------------------------------------*/
extern   SysStatusType  drv_bq40zxx_get_temper(u16_t* temper);
extern   SysStatusType  drv_bq40zxx_get_relative_soc(u8_t* soc);
extern   SysStatusType  drv_bq40zxx_get_sum_of_volt(u16_t* volt);
extern   SysStatusType  drv_bq40zxx_get_design_capacity(u16_t* cap);
extern   SysStatusType  drv_bq40zxx_get_remain_capacity(u16_t* cap);
extern   SysStatusType  drv_bq40zxx_get_current(s16_t* curr);
extern   SysStatusType  drv_bq40zxx_get_cell1_volt(u16_t* volt);
extern   SysStatusType  drv_bq40zxx_get_cell2_volt(u16_t* volt);
extern   SysStatusType  drv_bq40zxx_get_cell3_volt(u16_t* volt);


#endif

/*------------------ end of file ---------------------------------------------*/

