#include "include.h"
#include "rtc.h"

#define RTCWAKEUPTIM Rtc_1Min

//uint8_t RTC_timing = 0;

//==============================================================================
//�������ƣ�void Timing_Parament_Init(void)
//����˵����RTC�����жϷ�����
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void Timing_Parament_Init(void)
{
	MQTT_Upload_f.bits.MQTT_Upload_WatseInfo_timing_f=1;	  
	
	Surplus_f.bits.Sensor_Change_Flag = 1;					//����Ԥ�����
}
//==============================================================================
//�������ƣ�void RtcCycCb(void)
//����˵����RTC�����жϷ�����
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
static void RtcCycCb(void)
{
	unix_time_count();
}

//==============================================================================
//�������ƣ�void lpRTC_Config(void)
//����˵����RTC��ʼ��
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
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

    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);   //RTCģ��ʱ�Ӵ�
    
    Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    
    stcRtcConfig.enClkSel = RtcClk32768;   //�ⲿ32.768k
    stcRtcConfig.enAmpmSel = Rtc24h;       //Rtc12h;//

//    stcTime.u8Year = 0x19;
//    stcTime.u8Month = 0x12;
//    stcTime.u8Day = 0x14;
//    stcTime.u8Hour = 0x13;
//    stcTime.u8Minute = 0x59;
//    stcTime.u8Second = 0x59;
//    stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
//    stcRtcConfig.pstcTimeDate = &stcTime;

    stcCycSel.enCyc_sel = RtcPradx;        //��PRDX���������ж�
    stcCycSel.u8Prdx = 1u;                 //ÿ��1s����һ�������ж�      30S   59u    

    stcRtcConfig.pstcCycSel = &stcCycSel;
      
    stcIrqCb.pfnAlarmIrqCb = NULL;
    stcIrqCb.pfnTimerIrqCb = RtcCycCb;		//RTC�жϷ�����
    stcRtcConfig.pstcIrqCb = &stcIrqCb;
    stcRtcConfig.bTouchNvic = TRUE;
    
    Rtc_DisableFunc(RtcCount);
    Rtc_Init(&stcRtcConfig); 
    
    Rtc_EnableFunc(RtcCount);
} 
