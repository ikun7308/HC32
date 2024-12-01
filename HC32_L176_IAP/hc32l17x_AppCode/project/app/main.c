/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "string.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
///<µç»ú2


typedef union
{
	uint8_t byte;
	struct
	{
    uint8_t systime1ms_flag           	:1;             //1msÖÐ¶Ï±êÖ¾Î»
		uint8_t systime10ms_flag      			:1;             //10msÖÐ¶Ï±êÖ¾Î»
    uint8_t systime50ms_flag      			:1;             //50msÖÐ¶Ï±êÖ¾Î» 
    uint8_t systime100ms_flag      			:1;             //100msÖÐ¶Ï±êÖ¾Î»
		uint8_t systime200ms_flag      			:1;             //200msÖÐ¶Ï±êÖ¾Î» 
		uint8_t systime500ms_flag      			:1;             //200msÖÐ¶Ï±êÖ¾Î» 
		uint8_t systime1s_flag							:1;
	  uint8_t systime10s_flag             :1;
//	uint8_t systime1min_flag						:1;

		

	} bits;   
} Systime_f_t;

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t systime1ms_flag           	= 0;   
uint8_t systime10ms_flag      			= 0;   
uint8_t systime50ms_flag      			= 0;   
uint8_t systime100ms_flag      			= 0;   
uint8_t systime200ms_flag      			= 0;   
uint8_t systime500ms_flag      			= 0;   
uint8_t systime1s_flag							= 0;
uint8_t systime5s_flag             = 0;
uint8_t systime10s_flag             = 0;
uint8_t upgrade_F=0;
uint16_t reset_F=0;
//uint8_t reset_F=0;

uint16_t                    u16Arr = 0;
uint16_t                    u16Arr1 = 0;
uint8_t client_len=0;
uint8_t i=0;
uint8_t temp_r=0;
uint8_t ret=0;
uint8_t http_set_F=0;
uint16_t http_set_T=0;

 


uint32_t XinTiao_T = 0;
uint8_t XinTiao_F = 0;
uint8_t MQTT_select_F=0;




uint16_t gz1Tem[30] = {0};
uint16_t gz2Hum[30] = {0};
uint8_t mqtt_set_F=0;
uint32_t mqtt_set_Z=0;


uint8_t msgnum[5] = {0};
char server_adress[50] = {0};
char server_URL[30] = {0};
char server_pro[30] = {0};
char server_version[30] = {0};
uint32_t msg_num = 10000;
uint8_t fail__count_F=0;
uint16_t fail__count_T=0;

uint16_t LOW_BATTERY_K_t = 0;
uint16_t LOW_BATTERY_G_t = 0;
uint16_t yushui_t = 0;
uint16_t SOC_t = 0;
uint8_t FLASH_F = 0;
uint32_t read_temp1=0;
static uint16_t  count_ms = 0;
uint8_t FS_type = 0; 


void App_AdcPortInit(void);
void App_AdcInit(void);
void App_AdcSglCfg(void);
void App_AdcSg17Cfg(void);
void App_AdcJqrCfg(void);
void App_AdcSQRCfg(void);
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

enum en_result u8Ret1;
void App_LEDPortCfg(void);
void App_Timer3Cfg(uint16_t u16Period);

void Workmode_Init(void);
void Flash_Init_U(void);
void Flash_Para_Init(void);
static void App_WdtInit(void);
//static void App_UserKeyInit(void);
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
		Workmode_Init(); 	
    Wdt_Start();
		Wdt_Feed();///< 启动 WDT
		Flash_SectorErase(BOOT_PARA_ADDRESS);
    while(1)
    {
			Tongxin_485();
		}
			
}

static void App_WdtInit(void)
{
    ///< 开启WDT外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
    ///< WDT 初始化
    Wdt_Init(WdtResetEn, WdtT1s64);
}


//==============================================================================
void Workmode_Init(void)
{
	    //时钟分频初始化
    App_ClkDivInit();
	//	App_SystemClkInit_RCH(SysctrlRchFreq24MHz);
		App_SystemClkInit_PLL48M_byRCH();  
    App_PortInit();					//串口屏串口IO初始化	
		App_Port0Init();	
		App_Uart0Cfg();
		App_UartCfg();					//安卓屏串口初始化

		
		Flash_SectorErase(BOOT_PARA_ADDRESS);
    while(Ok != Flash_Init(6, TRUE))
    {
        ;
    }	
	  App_Timer3Cfg(3000);//TIM3 运行。
		Tim3_M0_Run(); 
	 ///< WDT 初始化
    App_WdtInit();
}



/*******************************************************************************
 * TIM3中断服务函数
 ******************************************************************************/
void Tim3_IRQHandler(void)
{
    static uint8_t i;
    
    //Timer3 模式0 计时溢出中断
    if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
    {
			//Uart_SendDataIt(M0P_UART0,'B');
				count_ms++;
	      systime1ms_flag = 1;
			if(reset_F){           //判断是否连接HTTP服务器标志位，重复发送{"project":"test","version":"1.0.0","packNo":"0"} 去判断是否连接成功
			reset_F++;
				if(reset_F>=10000){
				memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_send(GSM_Upload_send_buff1);
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);	
					fail__count_F++;
					reset_F=1;	
				}
			
			}
			if(fail__count_F>=15){   //累计15次没有收到返回指令，连接HTTP服务器失败，切换回MQTT模式
				reset_F=0;
				mqtt_set_F=1;
				fail__count_F=0;
			}
			
        if(GSM_receive_f)
        {
            GSM_receive_T++;
            if(GSM_receive_T>=50)
            {
								GSM_receive_f = 0;
								GSM_receive_T = 0;
								GSM_receive_finish=1; 
				
            }	
				}				
				if(http_set_F){   //HTTP设置标志位
				http_set_T++;
					if(http_set_T>=2000 && http_set_F==1){
					http_set_F=2;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = AT_T(GSM_Upload_send_buff1);//进入指令模式
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=3000&& http_set_F==2){
					http_set_F=3;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_post(GSM_Upload_send_buff1);// 设置POST请求
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=4000&& http_set_F==3){
					http_set_F=4;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
						GSM_Upload_send_buff_len1 = http_ssl(GSM_Upload_send_buff1);//SSL不使能
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=5000&& http_set_F==4){
					http_set_F=5;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_url(GSM_Upload_send_buff1);//设置URL
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=6000&& http_set_F==5){
					http_set_F=6;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_head(GSM_Upload_send_buff1);//设置头标签
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=7000&& http_set_F==6){
					http_set_F=7;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_server(GSM_Upload_send_buff1);
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=8000&& http_set_F==7){
					http_set_F=8;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_overtime(GSM_Upload_send_buff1);//设置超时时间 1s
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=9000&& http_set_F==8){
					http_set_F=9;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_filter_HEAD(GSM_Upload_send_buff1);//过滤头信息
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					
					if(http_set_T>=10000&& http_set_F==9){
					http_set_F=10;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = AT_SS(GSM_Upload_send_buff1);//保存
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=18000&& http_set_F==10){
					http_set_F=11;
						//http_set_T=0;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = AT_T(GSM_Upload_send_buff1);  //重启
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=18500&& http_set_F==11){
					http_set_F=12;
						//http_set_T=0;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = http_mode(GSM_Upload_send_buff1);//HTTP模式切换
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
					
					}
					if(http_set_T>=19000&& http_set_F==12){
					http_set_F=0;
						http_set_T=0;
						systime5s_flag = 0;
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = AT_SS(GSM_Upload_send_buff1);
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);//保存并重启
					reset_F=1;
					}
				
					
				}
				
				if((count_ms%10)==0)
        {
					systime10ms_flag = 1;
				}				
				
				if((count_ms%50)==0)
        {
					systime50ms_flag = 1;
				}	
        if((count_ms%100)==0)
        {
					systime100ms_flag = 1;
				}			
        if((count_ms%500)==0)
        {
					systime500ms_flag = 1;
				}									
        if((count_ms%1000)==0)
        {
					systime1s_flag = 1;
				}
				if((count_ms%5000)==0)
        {
					systime5s_flag = 1;
				}
				if((count_ms%15000)==0 && mqtt_set_F==1)// <MQTT模式设置，上电自动执行
        {
					mqtt_set_F=2;
				memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = AT_T(GSM_Upload_send_buff1);//进入指令模式
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
				}
				if((count_ms%16000)==0 && mqtt_set_F==2)
        {
					mqtt_set_F=3;
				memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = mqtt_mode(GSM_Upload_send_buff1);//MQTT模式切换
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
				}
				if((count_ms%17000)==0 && mqtt_set_F==3)
        {
					//mqtt_set_F=4;	
		memset(GSM_Upload_send_buff1,0x00,150);																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											 
				GSM_Upload_send_buff_len1 = AT_SS(GSM_Upload_send_buff1);//保存并重启
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);	
						mqtt_set_F=0;		
				}
        if((count_ms%60000)==0)
        {
					count_ms=0;			
				}		
        
        Tim3_ClearIntFlag(Tim3UevIrq);  //Timer3模式0 中断标志清除
    }
}
//Timer3 配置
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


 //时钟初始化配置
void App_ClkDivInit(void)
{
    //时钟分频设置
    Sysctrl_SetHCLKDiv(SysctrlHclkDiv1);
    Sysctrl_SetPCLKDiv(SysctrlPclkDiv1);
}                                                     
void App_SystemClkInit_RCH(en_sysctrl_rch_freq_t enRchFreq)
{  
    ///< RCH时钟不同频率的切换，需要先将时钟切换到RCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_SetRCLStableTime(SysctrlRclStableCycle64);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    ///< 加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(enRchFreq);
    ///< 使能RCH
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    ///< 时钟切换到RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    
    ///< HCLK不超过24M：此处设置FLASH读等待周期为0 cycle
    Flash_WaitCycle(FlashWaitCycle0);
    
}
void App_SystemClkInit_PLL48M_byRCH(void)
{
    stc_sysctrl_pll_cfg_t stcPLLCfg;    
    
    ///< RCH时钟不同频率的切换，需要先将时钟切换到RCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_SetRCLStableTime(SysctrlRclStableCycle64);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    ///< 加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    ///< 使能RCH
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  //PLL 输出
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //输入时钟源选择RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul12;            //4MHz x 12 = 48MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg); 
    
    ///< 当使用的时钟源HCLK大于24M：设置FLASH 读等待周期为1 cycle(默认值也为1 cycle)
    Flash_WaitCycle(FlashWaitCycle1);    
    
    ///< 使能PLL
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
    ///< 时钟切换到PLL
    Sysctrl_SysClkSwitch(SysctrlClkPLL);

}  



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


