/******************************************************************************
 * @file   main.c
 *
 * @brief  Source file for LPUART example
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "lpuart.h"
#include "lpm.h"
#include "gpio.h"
#include "flash.h"
#include "sysctrl.h"
#include "iap.h"
#include "modem.h"
#include "uart.h"
#include "timer3.h"
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t u8TxData[2] = {0x00,0x55};
uint8_t u8RxDataBuffer[20] = {0};
uint8_t u8RxDataBufferLenth = 0;
uint8_t u8RxData = 00;
uint32_t u32DataPvar  = 0;
uint8_t* Infrared1  =(uint8_t*)"boot start!";//11
uint8_t  bootstr[100] = {0x00};
uint8_t  TX_count=0;
uint8_t TX_max=0;
uint8_t* post1  =(uint8_t*)"{\"project\":\"test\",\"version\":\"1.0.0\",\"packNo\":\"";//46
const uint8_t* ATcmd = (uint8_t*)"+++";//3
const uint8_t* MQTT_mode_cmd = (uint8_t*)"AT+WKMOD=MQTT\n";//14
const uint8_t* MQTT_reback_cmd = (uint8_t*)"upgrade fail\n";//14
const uint8_t* AT_S = (uint8_t*)"adminAT+S\n";//10
uint8_t post_num=0;
uint8_t* post2=(uint8_t*)"\"}\n";//3
static uint16_t  count_ms = 0;
uint8_t systime5s_flag= 0;
uint8_t delay5s_F=0;
uint8_t receive_F=0;
char before_F=0;
uint16_t before_T=0;
uint8_t led_trg=0;
uint8_t send_num_F=0;
uint16_t send_num_T=0;
uint8_t send_num_Flag=0;
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void App_PortInit(void);
void App_UartCfg(void);
void App_ClkCfg(void);
uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len);
void App_LEDPortCfg(void);
uint8_t AT_SS(uint8_t *buf);
uint8_t AT_T(uint8_t *buf);
void App_Timer3Cfg(uint16_t num);
uint8_t mqtt_mode(uint8_t *buf);
uint8_t mqtt_send(uint8_t *buf);
int32_t main(void)
{  
	  uint8_t ret=0;
	  App_ClkCfg();
	App_LEDPortCfg();
    App_PortInit();///< 端口配置
    App_UartCfg();///< LPUART配置
    ret = Flash_Init(6, TRUE);
	
	  if(ret != Ok)
		{
		  ret = Flash_Init(6, TRUE);
		}
	
		
			delay1ms(200);
		 App_Timer3Cfg(3000);//TIM3 运行。
		Tim3_M0_Run();
		IAP_Init();
		
    while(1)
    {
			
       if(u8RxDataBufferLenth == 4)
			 {
				 u32DataPvar = *(uint32_t *)&u8RxDataBuffer[0];
			 }
			 IAP_UpdateCheck();
			 IAP_Main();
			 
    }
}


///< LPUART配置
void App_UartCfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///< 开启外设时钟
     Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);
    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;          ///<模式1
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bit停止位
    stcCfg.enMmdorCk        = UartMskDataOrAddr;     ///<无奇偶检验
    stcCfg.stcBaud.u32Baud  = 115200;                ///<波特率115200
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<通道采样分频配置
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<获得外设时钟（PCLK）频率值
    Uart_Init(M0P_UART0, &stcCfg);                   ///<串口初始化
    
    ///<UART中断使能
    Uart_ClrStatus(M0P_UART0,UartRC);                ///<清接收请求
    Uart_ClrStatus(M0P_UART0,UartTC);                ///<清接收请求
    Uart_EnableIrq(M0P_UART0,UartRxIrq);
 Uart_EnableIrq(M0P_UART0,UartTxIrq);	///<使能串口接收中断	///<使能串口接收中断
    EnableNvic(UART0_2_IRQn, IrqLevel3, TRUE);       ///<系统中断使能
}

///< 端口配置
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
     stc_gpio_cfg_t stcGpioCfggg;
    DDL_ZERO_STRUCT(stcGpioCfg);
    DDL_ZERO_STRUCT(stcGpioCfggg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf1);         //配置PA09 端口为URART0_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf1);         //配置PA10 端口为URART0_RX
	//stcGpioCfg.enDir = GpioDirOut;
	
	
	
}


//时钟配置为24MHz
void App_ClkCfg(void)
{
    M0P_SYSCTRL->RCH_CR_f.TRIM = (*((volatile uint16_t*) (0x00100C08ul)));    //Loader 4MHz Trimming value 
    M0P_SYSCTRL->RCH_CR_f.TRIM = (*((volatile uint16_t*) (0x00100C06ul)));    //Loader 8MHz Trimming value 
    M0P_SYSCTRL->RCH_CR_f.TRIM = (*((volatile uint16_t*) (0x00100C04ul)));    //Loader 16MHz Trimming value 
    M0P_SYSCTRL->RCH_CR_f.TRIM = (*((volatile uint16_t*) (0x00100C00ul)));    //Loader 24MHz Trimming value 
    SystemCoreClock = 24000000;
}

uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len){
	uint8_t *ptr = buf;
	uint8_t rc;	
	 memcpy(ptr,(char*)str,len);
		ptr += len;
	writeLInt(&ptr,post_num);
	memcpy(ptr,(char*)post2,3);
	ptr +=3;
	rc = ptr-buf;
	return rc;
}





void App_Timer3Cfg(uint16_t u16Period)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_cfg_t     stcTim3BaseCfg;
    
    //结构体初始化清零
   // DDL_ZERO_STRUCT(stcTim3BaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); //Base Timer外设时钟使能
    
    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //定时器模式
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //定时器功能，计数时钟为内部PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv16;              //PCLK/16
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //自动重载16位计数器/定时器
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;
    
    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 的模式0功能初始化
        
    u16ArrValue = 0x10000 - u16Period ;
    
    Tim3_M0_ARRSet(u16ArrValue);                            //设置重载值(ARR = 0x10000 - 周期)
    
    u16CntValue = 0x10000 - u16Period;
    
    Tim3_M0_Cnt16Set(u16CntValue);                          //设置计数初值
    
    Tim3_ClearIntFlag(Tim3UevIrq);                          //清中断标志
    Tim3_Mode0_EnableIrq();                                 //使能TIM3中断(模式0时只有一个中断)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 开中断 
}
void Tim3_IRQHandler(void)
{ 
    //Timer3 模式0 计时溢出中断
    if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
    {
				count_ms++;
			if(count_ms%120==0){
			led_trg=1;                                      //LED翻转
			}
			
			if(count_ms>=7000 && systime5s_flag==0 ){
				
					memset(bootstr,0x00,100);
      TX_max=ctrl(bootstr,post1,46);
	    Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);//7S 后开始发送第一个POST，获取升级包的大小
			systime5s_flag=1;
			before_T=1;
			before_F=-1;				
			}
			
			if( receive_F==1 ){                          //receive_F 只有在收到上一个数据包并写入完成才会置1，开始发送下一个数据包
				receive_F=0;
				post_num++;
				memset(bootstr,0x00,100);
        TX_max=ctrl(bootstr,post1,46);
	      Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
		    before_T=1;
			}
			
			
			if(before_T){                              //before_T  记录单个POST发送超时时间，9S后没有收到数据包再次发送
				before_T++;
				if(before_T>9000){
					before_T=1;
					if(before_F!=post_num){
						memset(bootstr,0x00,100);
            TX_max=ctrl(bootstr,post1,46);
	          Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
						send_num_F++;
					}
				}
			}
			
			if(send_num_F>=15){                    //send_num_F  记录单个POST发送次数，15次后认定升级失败，切换回MQTT模式并发送upgrade fail，最后跳回APP程序
			send_num_T++;
				if(send_num_T>=1000 && send_num_Flag==0){
				send_num_Flag=1;
					memset(bootstr,0x00,100);
	        TX_max=AT_T(bootstr);             // 进入指令模式
	        Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
			}
				if(send_num_T>=2000 && send_num_Flag==1){
				  send_num_Flag=2;
					memset(bootstr,0x00,100);
	        TX_max=mqtt_mode(bootstr);       //MQTT模式切换
	        Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
			}
				if(send_num_T>=3000 && send_num_Flag==2){
				send_num_Flag=3;
				memset(bootstr,0x00,100);
	      TX_max=AT_SS(bootstr);            //保存并重启
	      Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
			}
				if(send_num_T>=13000 && send_num_Flag==3){
				send_num_Flag=4;
				memset(bootstr,0x00,100);
	      TX_max=mqtt_send(bootstr);      // 发送升级失败信息
	      Uart_SendDataIt(M0P_UART0,bootstr[TX_count]);
			}
				if(send_num_T>=14000 && send_num_Flag==4){
					send_num_T=0;
			    IAP_JumpToApp_cmd();         //跳转APP程序
			}
		}
			if(count_ms>=60000){
			  count_ms=0;}
			  Tim3_ClearIntFlag(Tim3UevIrq);
		}
	}

void App_LEDPortCfg(void)
{
    stc_gpio_cfg_t           stcLEDPortCfg;
    stc_gpio_cfg_t           stcMotorPortCfg;
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcLEDPortCfg);
    DDL_ZERO_STRUCT(stcMotorPortCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO 外设时钟使能
    
    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcLEDPortCfg);   //PD14设置为LED输出
    stcMotorPortCfg.enDir  = GpioDirOut;
	  stcMotorPortCfg.enDrv  = FALSE;
	  stcMotorPortCfg.enPu  = TRUE;
	  stcMotorPortCfg.enPd  = FALSE;	
	  stcMotorPortCfg.enOD  = FALSE;	

			
		Gpio_Init(GpioPortB, GpioPin4, &stcMotorPortCfg);
		
		Gpio_WriteOutputIO(GpioPortB, GpioPin4, TRUE);
}
uint8_t AT_SS(uint8_t *buf)
{
    uint8_t *ptr = buf; 
//    uint8_t state1=0;
    uint8_t rc;
    memcpy(ptr,(char*)AT_S,10);
    ptr += 10;		
    rc = ptr-buf;
   
	return rc;
}
uint8_t AT_T(uint8_t *buf)
{
    uint8_t *ptr = buf; 
//    uint8_t state1=0;
    uint8_t rc;
    memcpy(ptr,(char*)ATcmd,3);
    ptr += 3;		
    rc = ptr-buf;

	return rc;
}
uint8_t mqtt_mode(uint8_t *buf)
{
    uint8_t *ptr = buf; 
//    uint8_t state1=0;
    uint8_t rc;
    memcpy(ptr,(char*)MQTT_mode_cmd,14);
    ptr += 14;		
    rc = ptr-buf;
   
	return rc;
}
uint8_t mqtt_send(uint8_t *buf)
{
    uint8_t *ptr = buf; 
  //  uint8_t state1=0;
    uint8_t rc;
    memcpy(ptr,(char*)MQTT_reback_cmd,14);
    ptr += 14;		
    rc = ptr-buf;
   
	return rc;
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


