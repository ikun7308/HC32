//******************************************************************************
//
//*	文件名： 系统时钟配置
//
//*	功能描述: 
//
//* 作者: 王婵媛   版本: 1.0 	日期: 2018.5.30
//
//*	说明:  外部晶振16M，主频66M.   
//*****************************************************************************


#include "include.h"

uint8_t systime1ms_flag=0;
uint8_t systime100ms_flag=0;
uint8_t systime10min_flag=0;

//===========================================================================//
//函数名称 : void (void) 
//函数功能 : 延时ms级
//输入变量：
//返回值：
//调试记录：

//===========================================================================//
void Delayms(uint32_t unCnt)
{
    SYST_RVR = unCnt*63200;
    SYST_CVR = 0;
    SYST_CSR |=1UL<<0;
    while((SYST_CSR & 1UL<<16)==0); 
}


//==============================================================================
//函数名称：void Watchdog_Init(void)
//功能说明：看门狗初始化
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void WDOG_Init(void)
{
	WDT_Open(WDT_TIMEOUT_2POW18, WDT_RESET_DELAY_1026CLK, TRUE, TRUE);
	WDT_EnableInt();	
}
//==============================================================================
//函数名称：void SysInit(void)
//功能说明：系统时钟初始化
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void sysclock_init(void)
{
	SYS_UnlockReg();

	CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;																//使能内部时钟
	while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));								//等待内部时钟稳定
	CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
	CLK->CLKSEL0 |= CLK_CLKSEL0_HCLK_S_HIRC;
	CLK->CLKDIV &= ~CLK_CLKDIV_HCLK_N_Msk;
	/* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
	CLK->PLLCON |= CLK_PLLCON_PD_Msk;
	/* Enable external 12MHz XTAL, internal 22.1184MHz */
	 CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk| CLK_PWRCON_OSC10K_EN_Msk;
	/* Enable PLL and Set PLL frequency */
	CLK->PLLCON = CLK_PLLCON_50MHz_HXT;
	/* Waiting for clock ready */
	while(!(CLK->CLKSTATUS & (CLK_CLKSTATUS_PLL_STB_Msk | CLK_CLKSTATUS_XTL12M_STB_Msk | CLK_CLKSTATUS_OSC22M_STB_Msk| CLK_CLKSTATUS_IRC10K_STB_Msk)));    
	SYST_CSR =0x4;//选择系统定时器的时钟来源		
	/* Switch HCLK clock source to PLL, STCLK to HCLK/2 */
	CLK->CLKSEL0 =  CLK_CLKSEL0_HCLK_S_PLL;		
	CLK->APBCLK |= CLK_APBCLK_WDT_EN_Msk;
	CLK->CLKSEL1 |= CLK_CLKSEL1_WDT_S_LIRC;	
	Delayms(100);//等待系统稳定	
    WDOG_Init();    
	return;
}



//==============================================================================
//函数名称：void Watchdog_Init(void)
//功能说明：喂狗
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void WDOG_Feed(void)
{
    WDT_RESET_COUNTER();	
}



//==============================================================================
//函数名称：void TIMER0_Init(void)
//功能说明：定时器0初始化
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void TIMER0_Init(void)
{
    CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;
    CLK->CLKSEL1|= CLK_CLKSEL1_TMR0_S_HIRC;
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);//1KHZ
    TIMER_EnableInt(TIMER0);
    NVIC_SetPriority(TMR0_IRQn,2);//优先级设置为1，比UART中断低一个
    NVIC_EnableIRQ(TMR0_IRQn); 
    TIMER_Start(TIMER0);
}

//==============================================================================
//函数名称：void Timer0_IRQhandler(void)
//功能说明：定时器0定时响应中断（1ms）
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void TMR0_IRQHandler(void)
{
    static uint8_t HundredMs_Count = 0;                                    //100ms计时
    static uint8_t Second_Count = 0;                                       //秒计时
    static uint16_t Minute_Count=0;
    uint16_t j=0;
    if( TIMER_GetIntFlag(TIMER0)==1)					        		   //1ms定时响应中断
    {
        TIMER_ClearIntFlag(TIMER0);
        systime1ms_flag = 1;      
        HundredMs_Count++;
      
        if(HundredMs_Count>=100)                                            //100ms
        {
            HundredMs_Count=0;
            systime100ms_flag =1;
            Second_Count++;
            if(Second_Count >= 10)                                          //1S
            {
                Second_Count = 0;
                Minute_Count++;
                if(Minute_Count>=60)
                {
                    Minute_Count=0;
                    systime10min_flag=systime10min_flag+1;
                    //温度上报计时：
                    if(TH_Upload_Time_Count>=1)
                    {
                        TH_Upload_Time_Count++;
                        if(TH_Upload_Time_Count>=TH_Upload_Time)
                        {
                            TH_Upload_Time_Count=1;
                            TH_Upload_f.bits.THtiming_Upload_f=1;
                        }
                    }
                }
                
                //心跳包上报计时：
                if(MQTT_PING_waitms>=1)
                {
                    MQTT_PING_waitms++;
                    if(MQTT_PING_waitms>=300)
                    {
                        MQTT_PING_waitms=1;
                        MQTT_PINGUpload_f = 1;
                    }
                }
                
                //心跳包回复计时：
                if(MQTT_PINGACK_waitms >= 1)         
                {
                    MQTT_PINGACK_waitms++;
                    if(MQTT_PINGACK_waitms>=280)                            //心跳包回复计时，超时断开连接重连
                    {
                       GSM_MQTT_STATE = MQTT_COMM_DisCONNECT; 
                       MQTT_PINGACK_waitms = 0;
                    }
                }                
                
            }
        }
        
        //AT指令通讯等待开始：计时
        if(waittime_ATCOMM )
        {
            waittime_ATCOMM ++;
        }
        
        //GSM模块：UART接收等待计时
        if(GSM_receive_f)
        {
            GSM_receive_T++;
            if(GSM_receive_T>=800)
            {
				GSM_receive_f = 0;
				GSM_receive_T = 0;
				GSM_receive_finish=1;                
            }
 			//收到CLOSED,TCP连接断开
			for(j=0;j < GSM_SERVER_rec_buf_count_MAX;j++)
			{
				if((GSM_SERVER_rec_buf[j]==',')
					&&(GSM_SERVER_rec_buf[j+1]==' ')
					&&(GSM_SERVER_rec_buf[j+2]=='C')
					&&(GSM_SERVER_rec_buf[j+3]=='L')
					&&(GSM_SERVER_rec_buf[j+4]=='O')
					&&(GSM_SERVER_rec_buf[j+5]=='S')
					&&(GSM_SERVER_rec_buf[j+6]=='E')
					&&(GSM_SERVER_rec_buf[j+7]=='D'))
                 {
					GSM_Module_STATE = GSM_STATE_Reset;
				 }
				
				if((GSM_SERVER_rec_buf[j]=='+')
                    &&(GSM_SERVER_rec_buf[j+1]=='C')
                    &&(GSM_SERVER_rec_buf[j+2]=='P')
                    &&(GSM_SERVER_rec_buf[j+3]=='I')
                    &&(GSM_SERVER_rec_buf[j+4]=='N')
                    &&(GSM_SERVER_rec_buf[j+5]==':')
                    &&(GSM_SERVER_rec_buf[j+6]==' ')
                    &&(GSM_SERVER_rec_buf[j+7]=='N')
                    &&(GSM_SERVER_rec_buf[j+8]=='O')
                    &&(GSM_SERVER_rec_buf[j+9]=='T')
                    &&(GSM_SERVER_rec_buf[j+10]==0x20)
                    &&(GSM_SERVER_rec_buf[j+11]=='R')
                    &&(GSM_SERVER_rec_buf[j+12]=='E')
                    &&(GSM_SERVER_rec_buf[j+13]=='A')
                    &&(GSM_SERVER_rec_buf[j+14]=='D')
                    &&(GSM_SERVER_rec_buf[j+15]=='Y'))
				{
					if(Wrong_Num == 0)
					{
						Wrong_Num = 2;
					}
					GSM_Module_STATE = GSM_STATE_Reset;
				}
			}           
            
            
        }
        
        //上位机：UART通讯等待计时
 		if(RS232_REC_f == 1)
		{
			RS232_REC_T ++;
			if(RS232_REC_T >= 500)
			{
				RS232_REC_finish =1;
				RS232_REC_f=0;
			}
		}       
        
        //MQTT通讯
        if(MQTT_COMM_Waitms>=1)
        {
            MQTT_COMM_Waitms++;
        }
        
        //GSM通讯
        if(sv_wait_ms>=1)
        {
            sv_wait_ms++;
        }
        
        //格林尼治
        unix_time_count();
        
    }
}










