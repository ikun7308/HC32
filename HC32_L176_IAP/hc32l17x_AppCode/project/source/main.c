/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "uart.h"
#include "lpuart.h"
#include "gpio.h"
#include "flash.h"
#include "sysctrl.h"
#include "iap.h"
#include "modem.h"
#include "bt.h"
#include "string.h"
#include "adc.h"
#include "timer3.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
#define     T1_PORT                 (3)
#define     T1_PIN                  (3)

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
//volatile static uint8_t u8RxData[20];
//volatile static uint8_t u8TxData[]={0x6D,0xAC,0x00,0xFF,0x0A,0x00,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCF,0x68};
//volatile static uint8_t u8TxCnt=0;
//volatile static uint8_t u8RxCnt=0;
volatile static uint32_t u32UpgData=0x67890123;
static   uint8_t  u8FlagNeedUpagrade = 1;
uint8_t* Infrared1  =(uint8_t*)"app start!";
uint8_t* Infrared2  =(uint8_t*)"app RRRRRR";
	static uint16_t  count_ms = 0;
uint8_t  appstr[15] = {0x00};
uint8_t  TX_count=0;
uint8_t TX_max=0;
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void App_UartCfg(void);
void App_PortInit(void);
void App_ClkCfg(void);
//void App_LpUartCfg(void);
//void App_LpUartPortCfg(void);
void App_Timer0Cfg(uint16_t u16Period);
uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len);
void UART_SendData(uint8_t *pu8TxBuff, uint16_t u16Length);
void App_Timer3Cfg(uint16_t u16Period);
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int32_t main(void)
{    
  
    App_ClkCfg();
	//App_Timer0Cfg(46875);//时钟配置
    App_PortInit();//串口引脚配置
    App_UartCfg();//串口配置
	App_Timer3Cfg(3000);
   // App_LpUartPortCfg();///< 端口配置
  //  App_LpUartCfg();///< LPUART配置
    Flash_Init(6, TRUE);//flash擦写初始化
	//Bt_M0_Run(TIM0);
	//Bt_M0_Stop(TIM0);
	  delay1ms(100);
	Tim3_M0_Run();
	// UART_SendData((uint8_t *)"app start!\r\n", 14);
	TX_max=ctrl(appstr,Infrared1,10);
	Uart_SendDataIt(M0P_UART0,appstr[TX_count]);
    while(1)
    {        
        u8FlagNeedUpagrade = Modem_Process();
			  if(u8FlagNeedUpagrade == Ok)
				{
				  NVIC_SystemReset();  //软件复位MCU
				
				}
//       	Gpio_WriteOutputIO(STK_LED_PORT,STK_LED_PIN,1-Gpio_ReadOutputIO(STK_LED_PORT,STK_LED_PIN));
//					 delay1ms(1000);
				//Gpio_SetIO(STK_LED_PORT,STK_LED_PIN);
		
    }
}



void App_Timer0Cfg(uint16_t u16Period)
{
   uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
    
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE); //Base Timer外设时钟使能
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //定时器模式
    stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 //PCLK/256
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //自动重载16位计数器/定时器
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化
    
    u16ArrValue = 0x10000 - u16Period;
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //设置重载值(ARR = 0x10000 - 周期)
    
    u16CntValue = 0x10000 - u16Period;
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //设置计数初值
    
    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //清中断标志   
    Bt_Mode0_EnableIrq(TIM0);                               //使能TIM0中断(模式0时只有一个中断)
    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0中断使能  
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

//串口引脚配置
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf1);         //配置PA09 端口为URART0_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf1);         //配置PA10 端口为URART0_RX
    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
   
///< GPIO IO LED端口初始化
    

    ///< LED关闭
  //  Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcGpioCfg);
    
}

//串口配置
void App_UartCfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///< 开启外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);///<使能uart1模块时钟
    
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
 Uart_EnableIrq(M0P_UART0,UartTxIrq);	///<使能串口接收中断
    EnableNvic(UART0_2_IRQn, IrqLevel3, TRUE);       ///<系统中断使能

}


///< LPUART配置
void App_LpUartCfg(void)
{
//	stc_lpuart_cfg_t  stcCfg;

//	DDL_ZERO_STRUCT(stcCfg);
//	
//	///<外设模块时钟使能
//	Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart1,TRUE);    
//	
//	///<LPUART 初始化
//	stcCfg.enStopBit = LPUart1bit;                   ///<1停止位    
//	stcCfg.enMmdorCk = LPUartEven;                   ///<偶校验LPUartEven
//	stcCfg.stcBaud.enSclkSel = LPUartMskPclk;        ///<传输时钟源
//	stcCfg.stcBaud.u32Sclk = Sysctrl_GetPClkFreq();  ///<PCLK获取
//	stcCfg.stcBaud.enSclkDiv = LPUartMsk4Or8Div;     ///<采样分频
//	stcCfg.stcBaud.u32Baud = 9600;                   ///<波特率
//	stcCfg.enRunMode = LPUartMskMode1;               ///<工作模式
//	LPUart_Init(M0P_LPUART1, &stcCfg);
//	 
//	///<LPUART 中断使能
//	LPUart_ClrStatus(M0P_LPUART1,LPUartRC);          ///<清接收中断请求
//	LPUart_ClrStatus(M0P_LPUART1,LPUartTC);          ///<清发送中断请求
//	LPUart_EnableIrq(M0P_LPUART1,LPUartRxIrq);      ///<禁止接收中断
////    LPUart_EnableIrq(M0P_LPUART1,LPUartTxIrq);       ///<使能发送中断
//	EnableNvic(LPUART1_IRQn,IrqLevel1,TRUE);         ///<系统中断使能
}

///< 端口配置
void App_LpUartPortCfg(void)
{
//    stc_gpio_cfg_t stcGpioCfg;
//    
//    DDL_ZERO_STRUCT(stcGpioCfg);
//    
//    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
//    
//    ///<TX
//    stcGpioCfg.enDir =  GpioDirOut;
//    Gpio_Init(GpioPortA,GpioPin0,&stcGpioCfg);
//    Gpio_SetAfMode(GpioPortA,GpioPin0,GpioAf2); //配置PA00为LPUART1_TX
//    
//    //<RX
//    stcGpioCfg.enDir =  GpioDirIn;
//    Gpio_Init(GpioPortA,GpioPin1,&stcGpioCfg);
//    Gpio_SetAfMode(GpioPortA,GpioPin1,GpioAf2); //配置PA01为LPUART1_RX
}

uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len){
	uint8_t *ptr = buf;
	uint8_t rc;	
	 memcpy(ptr,(char*)str,len);
		ptr += len;
	rc = ptr-buf;
	return rc;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
void Tim3_IRQHandler(void)
{

	if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
    {
			count_ms++;
			if(count_ms>=1000){
				count_ms=0;
			TX_max=ctrl(appstr,Infrared2,10);
	Uart_SendDataIt(M0P_UART0,appstr[TX_count]);
			}
			
			Tim3_ClearIntFlag(Tim3UevIrq);
		}
}

void App_Timer3Cfg(uint16_t u16Period)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_cfg_t     stcTim3BaseCfg;
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    
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