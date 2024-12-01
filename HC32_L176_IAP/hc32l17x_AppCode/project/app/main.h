#ifndef _MAIN_H_
#define _MAIN_H_
#include "ddl.h"
#include "lpuart.h"
#include "uart.h"
#include "lpm.h"
#include "gpio.h"
#include "sysctrl.h"
#include "timer3.h"
#include "SPI_FLASH.h"
#include "spi.h"
#include "data.h"
#include "bsp_sensor.h"
#include "flash.h"
#include "user_Sensor.h"
#include "math.h"
#include <stdlib.h>
#include "wdt.h"
#include "uart_init.h"
#include "dj_init.h"
#include "user_uart.h"
#include "gsm_bsp.h"
#include "tim_3.h"
#include "adc.h"
#include "bgr.h"
#include "modem.h"
#include "iap.h"
#define FLASH_SECTOR_SIZE           0x200ul                           //??sector???512B
#define FLASH_BASE                  ((uint32_t)0x00000000)            //flash???
#define FLASH_SIZE                  (256u * FLASH_SECTOR_SIZE)        //flash??128KB
/*************************RAM?????*************************************************/
#define SRAM_BASE                   ((uint32_t)0x20000000)            //RAM???
#define RAM_SIZE                    0x4000ul                          //RAM??16KB
/*************************BootLoader flash?????*********************************/
#define BOOT_SIZE                   (16*FLASH_SECTOR_SIZE)            //BootLoader flash??8KB
#define BOOT_PARA_ADDRESS           (FLASH_BASE + BOOT_SIZE - 0x200u) //BootLoader para????
#define PACKCT_SIZE_ADDRESS         (FLASH_BASE + BOOT_SIZE - 0x200u + 4U) //????????????
/*************************APP flash?????*****************************************/
#define APP_UPG_FLAG                ((uint32_t)0xAAAA5555)            //?BootLoader para?????,??APP????
#define APP_ADDRESS                 (uint32_t)(FLASH_BASE + BOOT_SIZE)          //APP???????
#define APP_PACKETS_SIZE            (uint32_t)(FLASH_SIZE - APP_ADDRESS)//app?????
extern uint16_t reset_F;
extern uint8_t systime1ms_flag   ;   
extern uint8_t systime10ms_flag  ;   
extern uint8_t systime50ms_flag  ;   
extern uint8_t systime100ms_flag ;   
extern uint8_t systime200ms_flag ;   
extern uint8_t systime500ms_flag ;   
extern uint8_t systime1s_flag		 ;
extern uint8_t systime5s_flag		 ;
extern uint8_t systime10s_flag   ;
extern char server_adress[50] ;
extern char server_URL[30] ;
extern char server_pro[30] ;
extern char server_version[30] ;
extern uint16_t u16Arr ;
extern uint16_t u16Arr1;

//LED???
extern uint8_t    LED_f;    //0 ??     1???5?? 
extern uint8_t    LED_S;
extern uint16_t   LED_T;

//接收计时标志


extern int32_t temp1;

extern uint32_t          u32Addr1;
extern uint8_t           u8TestData;  

 
extern uint32_t XinTiao_T;
extern uint8_t XinTiao_F;


extern uint16_t KJDD_T;

extern volatile uint32_t u32AdcRestult_SOC;
extern volatile uint32_t u32AdcRestult_SOC_SUM;
extern volatile uint32_t u32AdcRestultwd;
extern volatile uint32_t u32AdcRestultsd;
extern volatile uint32_t u32AdcRestult_gz1_sum;
extern volatile uint32_t u32AdcRestult_gz2_sum;
extern volatile uint32_t u32AdcRestult_gz1;
extern volatile uint32_t u32AdcRestult_gz2;
extern volatile uint16_t u32AdcRestult_current;
extern volatile uint32_t u32AdcRestult_current_sum;
extern volatile uint16_t u32AdcRestult_yushui;
extern volatile uint32_t u32AdcRestult_yushui_sum;


extern uint16_t gz1Tem[30];
extern uint16_t gz2Hum[30];

extern uint8_t gz1Tem_t;
extern uint8_t gz2Hum_t;
extern uint8_t MQTT_client_id_F;

extern uint32_t DLAVG;
extern uint16_t DLTem[30];
extern uint8_t DLTem_t;
extern uint8_t http_set_F;
extern uint8_t MWD_MODE_AT_F;
extern uint8_t MWD_MODE_TC_F;
extern uint8_t MWD_GPSEN_ON_F;
extern uint8_t MWD_GPS_F;
extern uint8_t MWD_SX_F;
extern uint16_t MWD_SX_T;
extern uint8_t MWD_KZHF_F;
extern uint8_t MWD_ZTHF_F;
extern uint8_t MWD_DW_F;
extern uint8_t MWD_DW0_F;

extern uint8_t LOAD1_STATE_F;
extern uint8_t LOAD2_STATE_F;
extern uint8_t INF_NUM;

extern uint16_t MWD_FSCS_T;

extern uint16_t MWD_SJHQ_t;
extern uint8_t msgnum[5];
extern uint32_t msg_num;
extern uint8_t FS_type; 
extern uint8_t FLASH_F;

extern uint16_t work_time;
extern uint16_t opentime;
extern uint16_t closetime;
extern uint16_t heartbeat;
extern uint16_t cutoff_vol;
extern uint16_t LOAD1;
extern uint16_t LOAD2;
extern uint16_t start_vol;
extern uint8_t work_state;
extern uint8_t work_f;
extern uint16_t TYN_K_T ;
extern uint16_t TYN_G_T ;
extern uint32_t work_t;
extern uint16_t LOW_BATTERY_K_t;
extern uint16_t LOW_BATTERY_G_t;
extern uint16_t yushui_t;
extern uint16_t SOC_t;
extern void App_LEDPortCfg(void);
extern void App_Timer3Cfg(uint16_t u16Period);

extern void Workmode_Init(void);
extern void Flash_Init_U(void);
extern void Flash_Para_Init(void);

extern void App_ClkDivInit(void);
extern void App_SystemClkInit_RCH(en_sysctrl_rch_freq_t enRchFreq);
extern void App_SystemClkInit_PLL48M_byRCH(void);
//extern static void App_WdtInit(void);
#endif
