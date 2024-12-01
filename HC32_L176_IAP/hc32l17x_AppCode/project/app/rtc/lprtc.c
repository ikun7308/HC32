#include "include.h"
#include "rtc.h"

#define RTCWAKEUPTIM Rtc_1Min

//uint8_t RTC_timing = 0;

//==============================================================================
//函数名称：void Timing_Parament_Init(void)
//功能说明：RTC周期中断服务函数
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void Timing_Parament_Init(void)
{
	MQTT_Upload_f.bits.MQTT_Upload_WatseInfo_timing_f=1;	  
	
	Surplus_f.bits.Sensor_Change_Flag = 1;					//重量预警监测
}
//==============================================================================
//函数名称：void RtcCycCb(void)
//功能说明：RTC周期中断服务函数
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
static void RtcCycCb(void)
{
	unix_time_count();
}

//==============================================================================
//函数名称：void lpRTC_Config(void)
//功能说明：RTC初始化
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void lpRTC_Config(void)
{
    stc_rtc_config_t stcRtcConfig; 
    stc_rtc_irq_cb_t stcIrqCb;              
    stc_rtc_alarmset_t stcAlarm;
    stc_rtc_cyc_sel_t   stcCycSel;
//    stc_rtc_time_t  stcTime;
	
    DDL_ZERO_STRUCT(stcRtcConfig);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcAlarm);
    DDL_ZERO_STRUCT(stcCycSel);
//    DDL_ZERO_STRUCT(stcTime);

	Sysctrl_SetFunc(SysctrlClkFuncRTCLpmEn, 1u);
	Sysctrl_SetFunc(SysctrlEXTLEn, 0u);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);   //RTC模块时钟打开
    
    Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    
    stcRtcConfig.enClkSel = RtcClk32768;   //外部32.768k
    stcRtcConfig.enAmpmSel = Rtc24h;       //Rtc12h;//

//    stcTime.u8Year = 0x19;
//    stcTime.u8Month = 0x12;
//    stcTime.u8Day = 0x14;
//    stcTime.u8Hour = 0x13;
//    stcTime.u8Minute = 0x59;
//    stcTime.u8Second = 0x59;
//    stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
//    stcRtcConfig.pstcTimeDate = &stcTime;

    stcCycSel.enCyc_sel = RtcPradx;        //由PRDX产生周期中断
    stcCycSel.u8Prdx = 1u;                 //每隔1s产生一次周期中断      30S   59u    

    stcRtcConfig.pstcCycSel = &stcCycSel;
      
    stcIrqCb.pfnAlarmIrqCb = NULL;
    stcIrqCb.pfnTimerIrqCb = RtcCycCb;		//RTC中断服务函数
    stcRtcConfig.pstcIrqCb = &stcIrqCb;
    stcRtcConfig.bTouchNvic = TRUE;
    
    Rtc_DisableFunc(RtcCount);
    Rtc_Init(&stcRtcConfig); 
    
    Rtc_EnableFunc(RtcCount);
} 
