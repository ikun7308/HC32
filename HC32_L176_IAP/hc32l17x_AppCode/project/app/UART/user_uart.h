/*****************************************************************************/
/** \file user_uart.h
 **

 **
 *****************************************************************************/

#ifndef __USER_UART_H__
#define __USER_UART_H__
/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"
#include "spi.h"
#include "gpio.h"
#include "reset.h"
#include "data.h"
#include "user_uart.h"
#ifdef __cplusplus
extern "C"
{
#endif

//extern const uint8_t* IP_const = (uint8_t*)"http://148.70.149.125:9092/";
//extern const uint8_t* TYPE_const = (uint8_t*)"TYPE=";
//extern const uint8_t* CMD_const =(uint8_t*)"CMD=";
//extern const uint8_t* NAME_const =(uint8_t*)"NAME=FLJ1000000001&";
//extern const uint8_t* BAG_const =(uint8_t*)"BAG=";
//extern const uint8_t* TOTAL_const =(uint8_t*)"TOTAL=";
//extern const uint8_t* YEZHU_const =(uint8_t*)"YEZHU=swz&TYPE=2";
extern uint8_t one_paper[7];//首页
extern uint8_t tow_paper[7];//正在称重界面
extern uint8_t three_paper[7];//重量显示界面       
extern uint8_t four_paper[7];//确认显示界面   
extern uint8_t display[8];//写重量数据
extern uint8_t display_id[24];
	
extern uint8_t sound1_play[];	
extern uint8_t sound2_play[];	
extern uint8_t sound3_play[];	
extern uint8_t sound4_play[];	
extern uint8_t sound5_play[];	
extern uint8_t sound6_play[];	
extern uint8_t sound7_play[];	
extern uint8_t sound8_play[];	
extern uint8_t sound9_play[];	
extern uint8_t sound10_play[];	
extern uint8_t sound11_play[];	
extern uint8_t sound12_play[];	
extern uint8_t sound13_play[];	
extern uint8_t sound14_play[];	
extern uint8_t sound15_play[];	
extern uint8_t sound16_play[];	
extern uint8_t sound17_play[];	
extern uint8_t sound18_play[];	
extern uint8_t soundshi_play[];	
extern uint8_t soundbai_play[];	
extern uint8_t sounddian_play[];	
extern uint8_t soundkg_play[];	
extern uint8_t sound23_play[];	
extern uint8_t sound24_play[];	
extern uint8_t sound25_play[];	
extern uint8_t sound26_play[];	
extern uint8_t sound27_play[];	
extern uint8_t sound28_play[];	
extern uint8_t sound29_play[];	
extern uint8_t sound30_play[];	
extern uint8_t sound31_play[];	
extern uint8_t sound32_play[];	
	
extern uint8_t yuyin_set[];
typedef struct
{
	uint8_t  wan_wei;
	uint8_t  qian_wei;
	uint8_t  bai_wei;
	uint8_t  shi_wei;
	uint8_t  ge_wei;
}Weigh_data;	

typedef struct
{
	uint8_t ad[7];
}Send_sound_data;
extern Send_sound_data Sound_play_data[10];
extern Weigh_data Weigh01_data;
extern Weigh_data Weigh02_data;
extern Weigh_data Weigh03_data;
extern Weigh_data Weigh04_data;

 





extern uint8_t one_times,two_times,three_times,four_times;
extern void Sound_init(void);
extern void Sound_Loop_Play1(void);
extern void Sound_Loop_Play2(void);
extern void Sound_Loop_Play3(void);
extern void Sound_Loop_Play4(void);
//@} // UartGroup

#ifdef __cplusplus
#endif

#endif /* __USER_UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



