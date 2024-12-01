#ifndef __UART_INIT_H_
#define __UART_INIT_H_

#include "ddl.h"
#include "spi.h"
#include "gpio.h"
#include "reset.h"
#include "data.h"

#define     GSM_SERVER_rec_buf_count_MAX 150
#define     ANDROIN_SERVER_rec_buf_count_MAX 100
#define     YY_buf_count_MAX 15
#define     CKP_buf_count_MAX 15
#define     usemsg_MAX 20

extern uint8_t u8TxData[2];
extern uint8_t u8RxData;

extern uint16_t GSM_Upload_send_buff_count;

extern uint8_t     RWM_SERVER_rec_buf[GSM_SERVER_rec_buf_count_MAX];
extern uint8_t     RWM_SERVER_rec_buf_info[16];
extern uint8_t  		ANDROID_send_buff1[ANDROIN_SERVER_rec_buf_count_MAX];
extern uint16_t    RWM_SERVER_rec_buf_count;   
extern uint16_t    RWM_SERVER_rec_buf_count1; //????? 
extern uint8_t     RWM_receive_finish; 
extern uint8_t		 RWM_receive_f;
extern uint16_t		 RWM_receive_T; 
extern uint8_t     GSM_SERVER_rec_buf[GSM_SERVER_rec_buf_count_MAX];
extern uint16_t    GSM_SERVER_rec_buf_count;  
extern uint16_t    GSM_buf_len;  
extern uint16_t    GSM_SERVER_rec_buf_count1; //????? 
extern uint8_t     GSM_receive_finish;                                                                 //???????
extern uint16_t		GSM_receive_T;    					 	   											//?????
extern uint8_t		GSM_receive_f;	

extern uint16_t ANDROID_send_buff_count;
extern uint16_t ANDROID_send_buff_len1;
extern uint8_t ANDROID_SEND_RUN_Flag;

extern uint8_t    YY_buf[YY_buf_count_MAX];
extern uint8_t 		YY_count;
extern uint8_t 		YY_buf_len;


extern uint8_t    CKP_buf[CKP_buf_count_MAX];
extern uint8_t 		CKP_count;
extern uint8_t 		CKP_buf_len;



extern uint8_t 		imgur_count[4];
extern uint16_t msgNo;



extern void App_LpUart0PortCfg(void);
extern void App_LpUart0Cfg(void);

extern void App_LpUartPortCfg(void);
extern void App_LpUartCfg(void);

extern void App_UartCfg(void);
extern void App_PortInit(void);

extern void App_Uart0Cfg(void);
extern void App_Port0Init(void);

static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length);
#endif


/****************************** END OF C FILE ********************************/	


