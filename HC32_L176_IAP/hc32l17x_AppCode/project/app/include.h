/******************************************************************************/
/** \file include.h
 **
 **
 *****************************************************************************/

#ifndef  __INCLUDE_H__
#define  __INCLUDE_H__

#include "ddl.h"
#include "uart.h"
#include "gpio.h"
#include "sysctrl.h"
#include "flash.h"
#include "lpm.h"
//#include "lpt.h"
#include "spi.h"
#include "rtc.h"
#include "bt.h"
#include "lpuart.h"

//#include "user_uart.h"
#include "uart_init.h"
#include "wdt.h"
//#include "Tim_int.h"

//#include "BSP_GPIO.h"
#include "SPI_FLASH.h"

#include "bsp_sensor.h"
#include "user_Sensor.h"
#include "data.h"
//#include "sysclock.h"
#include "uart_init.h"
#include "gsm_bsp.h"
#include "gsm_module.h"
#include "mqtt_module.h"
//#include "upgrade.h"
//#include "MQTTPacket.h"

//#include "lpmode.h"
//#include "tm1637.h"
#include "timer3.h"
//#include "display.h"

#include "lprtc.h"
#include "wakeup.h"
#include "workmode.h"
#include "user_adc.h"
#include "adc.h"
#include "door_check.h"
#include "screen_rs232.h"
#include "USER_BEEP.h"
#include "math.h"
#include <stdlib.h>
#include "string.h"
#include "stdint.h"

extern uint8_t MachNum;
extern uint32_t test0_GSM ;

extern uint8_t MQTT_DisConnect(void);
extern void Para_Init(void);

//extern uint8_t chack_mode_f;
extern uint16_t Fornum;
extern void TM1637_DIS(uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4,uint8_t data5,uint8_t data6);


extern void	Flash_Init_U(void);					
extern void	wspowerinit(void);
extern void	setwspowere(void);

extern void wdh0_init(void);



#define Hard_Version "\"U01_HVer_1.0\""
#define Soft_Version "\"U01_SVer_1.0\""

#define  MCU_SoftRest()      { NVIC_SystemReset();  }  
//***************GSM_232通讯口配置****************　//


/*******************************系统参数**************************************/
#define     EnableInterrupts()	   __enable_irq()	// 开总中断
#define     DisableInterrupts()	   __disable_irq() // 关总中断


///*******************************8 Sysem Tick******************************** */
//#define     SYST_CSR             (*((volatile unsigned long *) (SCS_BA+0x00000010)))
//#define     SYST_RVR             (*((volatile unsigned long *) (SCS_BA+0x00000014)))
//#define     SYST_CVR             (*((volatile unsigned long *) (SCS_BA+0x00000018)))
//#define     SCS_BA                0xE000E000UL
#endif /* __INCLUDE_H__ */

/*******************************************************************************
 * EOF (not truncated)                                                        
 ******************************************************************************/
