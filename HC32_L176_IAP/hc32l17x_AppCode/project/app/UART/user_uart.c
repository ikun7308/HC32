/** \file user_uart.c
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
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
//#include "bsp_sensor.h"
#include "flash.h"
#include "user_Sensor.h"
#include "math.h"
#include <stdlib.h>
#include "wdt.h"
#include "uart_init.h"
//#include "dj_init.h"
#include "user_uart.h"
#include "gsm_bsp.h"
/**
 ******************************************************************************
 ** \addtogroup UartGroup
 ******************************************************************************/
//@{
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
const uint8_t* IP_const = (uint8_t*)"http://148.70.149.125:9092/";
const uint8_t* TYPE_const = (uint8_t*)"TYPE=";
const uint8_t* CMD_const =(uint8_t*)"CMD=";
const uint8_t* NAME_const =(uint8_t*)"NAME=FLJ1000000001&";
const uint8_t* BAG_const =(uint8_t*)"BAG=";
const uint8_t* TOTAL_const =(uint8_t*)"TOTAL=";
const uint8_t* YEZHU_const =(uint8_t*)"YEZHU=swz&TYPE=2";

const uint8_t* AT_UART_const =(uint8_t*)"YEZHU=swz&TYPE=2";


uint8_t Baudrate_set[24] = {'A','T','*','U','A','R','T','=','1','1','5','2','0','0',',','8',',','0',',','1',',','0','#',0x0D};
uint8_t Workmode_set[13] = {'A','T','*','W','K','M','O','D','E','=','9','#',0x0D};


uint8_t one_paper[7]	={0x5a,0xa5,0x04,0x80,0x03,0x00,0x00};//��ҳ
uint8_t tow_paper[7]	={0x5a,0xa5,0x04,0x80,0x03,0x00,0x01};//
uint8_t three_paper[7]={0x5a,0xa5,0x04,0x80,0x03,0x00,0x01};//���ڳ��ؽ���     
uint8_t four_paper[7]	={0x5a,0xa5,0x04,0x80,0x03,0x00,0x02};//������ʾ����    
uint8_t display[8]		={0x5a,0xa5,0x05,0x82,0x00,0x10,0x00,0x64};//д��������
uint8_t display_id[24]		={0x5a,0xa5,0x12,0x82,0x10,0x00,0x46,0x4c,0x4a,0x31,0x42,0x33,0x32,0x30,0x39,0x40,0x30,0x2E,0x31,0x30,0x32};//д��������
//FLJ1B3209A00122

uint8_t sound1_play[]     =  	{0x7E,0x05,0xA0,0x00,0x01,0xA6,0xEF};     //��һ��������
uint8_t sound2_play[]     =  	{0x7E,0x05,0xA0,0x00,0x02,0xA7,0xEF};     //�ڶ���������
uint8_t sound3_play[]     =   {0x7E,0x05,0xA0,0x00,0x03,0xA8,0xEF};     //��������ֽ�������Ѵ�
uint8_t sound4_play[]     =   {0x7E,0x05,0xA0,0x00,0x04,0xA9,0xEF};     //�����������������Ѵ�
uint8_t sound5_play[]     =   {0x7E,0x05,0xA0,0x00,0x05,0xAA,0xEF};     //�����������������Ѵ�
uint8_t sound6_play[]     =   {0x7E,0x05,0xA0,0x00,0x06,0xAB,0xEF};     //�����������������Ѵ�
uint8_t sound7_play[]     =   {0x7E,0x05,0xA0,0x00,0x07,0xAC,0xEF};      //�����������ع��� �뵽�ͽ��豸Ͷ��
uint8_t sound8_play[]     =   {0x7E,0x05,0xA0,0x00,0x08,0xAD,0xEF};      //�ڰ��������ڳ��أ����Ժ�
uint8_t sound9_play[]     =   {0x7E,0x05,0xA0,0x00,0x09,0xAE,0xEF};      //��Ͷ�ݵ�����Ϊ
uint8_t sound10_play[]    =   {0x7E,0x05,0xA0,0x00,0x0A,0xAF,0xEF};    //һ
uint8_t sound11_play[]    =   {0x7E,0x05,0xA0,0x00,0x0B,0xB0,0xEF};    //��
uint8_t sound12_play[]    =   {0x7E,0x05,0xA0,0x00,0x0C,0xB1,0xEF};    //��
uint8_t sound13_play[]    =   {0x7E,0x05,0xA0,0x00,0x0D,0xB2,0xEF};    //��
uint8_t sound14_play[]    =   {0x7E,0x05,0xA0,0x00,0x0E,0xB3,0xEF};    //��
uint8_t sound15_play[]    =   {0x7E,0x05,0xA0,0x00,0x0F,0xB4,0xEF};    //��
uint8_t sound16_play[]    =   {0x7E,0x05,0xA0,0x00,0x10,0xB5,0xEF};    //��
uint8_t sound17_play[]    =   {0x7E,0x05,0xA0,0x00,0x11,0xB6,0xEF};    //��
uint8_t sound18_play[]    =   {0x7E,0x05,0xA0,0x00,0x12,0xB7,0xEF};    //��
uint8_t soundshi_play[]   =   {0x7E,0x05,0xA0,0x00,0x13,0xB8,0xEF};    //ʮ
uint8_t soundbai_play[]   =   {0x7E,0x05,0xA0,0x00,0x14,0xB9,0xEF};    //��
uint8_t sounddian_play[]  =   {0x7E,0x05,0xA0,0x00,0x15,0xBA,0xEF};    //��
uint8_t soundkg_play[]   	=   {0x7E,0x05,0xA0,0x00,0x16,0xBB,0xEF};    //kg
uint8_t sound23_play[]   	=   {0x7E,0x05,0xA0,0x00,0x17,0xBC,0xEF};    //��
uint8_t sound24_play[]   	=   {0x7E,0x05,0xA0,0x00,0x18,0xBD,0xEF};    //��ȷ�������Ƿ�غ�
uint8_t sound25_play[]   	=   {0x7E,0x05,0xA0,0x00,0x19,0xBe,0xEF};  //ֽ��У׼����
uint8_t sound26_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1a,0xBf,0xEF};    //����У׼����
uint8_t sound27_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1b,0xc0,0xEF};   //����У׼����
uint8_t sound28_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1c,0xc1,0xEF};  //����У׼����
uint8_t sound29_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1d,0xc2,0xEF};    //У׼���
uint8_t sound30_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1e,0xc3,0xEF};    //����ȡ�߻�������
uint8_t sound31_play[]   	=   {0x7E,0x05,0xA0,0x00,0x1f,0xc4,0xEF};    //��ر����ź���ɨ�뿪��
uint8_t sound32_play[]   	=   {0x7E,0x05,0xA0,0x00,0x20,0xc5,0xEF};    //�����Ѿ��򿪣������ظ�ɨ��

uint8_t yuyin_set[]     =   {0x7E,0x05,0xB6,0x01,0xBB,0xEF};    //�����Ѿ��򿪣������ظ�ɨ��
Weigh_data DKC_Weigh_data;
Weigh_data Weigh01_data;
Weigh_data Weigh02_data;
Weigh_data Weigh03_data;
Weigh_data Weigh04_data;
Send_sound_data Sound_play_data[10];
uint8_t one_times=0,two_times=0,three_times=0,four_times=0;

void Sound_init(void)
{
  uint8_t i=0;
	for(i=0;i<7;i++)
	{
	  Sound_play_data[0].ad[i]=sound23_play[i];
	  Sound_play_data[1].ad[i]=sound10_play[i];
	  Sound_play_data[2].ad[i]=sound11_play[i];
	  Sound_play_data[3].ad[i]=sound12_play[i];
		Sound_play_data[4].ad[i]=sound13_play[i];
		Sound_play_data[5].ad[i]=sound14_play[i];
		Sound_play_data[6].ad[i]=sound15_play[i];
		Sound_play_data[7].ad[i]=sound16_play[i];
		Sound_play_data[8].ad[i]=sound17_play[i];
		Sound_play_data[9].ad[i]=sound18_play[i];
	}
}
void Sound_Loop_Play1(void)//������������
{

}
void Sound_Loop_Play2(void)//������������
{

}
void Sound_Loop_Play3(void)//������������
{

}

void Sound_Loop_Play4(void)//������������
{

}


//@} // UartGroup      
