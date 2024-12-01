#include "include.h"

Module_STATE_t Module_STATE;
Warn_Upload_f_t Warn_Upload_f_old;
Warn_Upload_f_t Warn_Upload_f;
MQTT_Upload_f_t MQTT_Upload_f;
Sensor_WrongNum_f_t Sensor_WrongNum_f;
Mode_f_t Mode_f;
Surplus_f_t Surplus_f;

Wrong_Num_f_t Wrong_Num_f;
Wrong_Num_f_t Wrong_Num_f_Old;

Systime_f_t Systime_f;
uint8_t Voltage_Warn_old = 0;  
uint8_t Temp_Warn_old = 0; 
uint8_t Sensor_Warn_old = 0; 
uint8_t Last_DOORSTATE = 0;    //上一个门状态  首次开机初始化为3

uint8_t TM7711_wait_count = 0;

//==============================================================================
//函数名称：void RtcCycCb(void)
//功能说明：RTC周期中断服务函数
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void wdh0_init(void)
{
	stc_wdt_config_t  stcWdt_Config;
		
	DDL_ZERO_STRUCT(stcWdt_Config);
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);//
	
	stcWdt_Config.u8LoadValue = 0x0f;//13.2s
	stcWdt_Config.enResetEnable = WRESET_EN;//WINT_EN;//WRESET_EN;////WRESET_EN
	Wdt_Init(&stcWdt_Config);
	Wdt_Start();
	
	
}
//==============================================================================
//函数名称：void Poweron_Parament_Init(void)
//功能说明：开机参数初始化
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void Poweron_Parament_Init(void)
{
    memset(MQTT_REC_BUFF,0x00,512);
	//错误码清零
	Wrong_Num_f.byte = Wrong_Num_f.byte & 0x0000;
	//Sensor_WrongNum_f.bits.SensorWarning_fir = 0;
	//Wrong_Num_f.bits.SensorWarning = 0;
	
	Surplus_f.byte = Surplus_f.byte & 0x00;
	Surplus_f.bits.Temp_Get_f = 1;
	//时间基准标志位清零
	Systime_f.byte = Systime_f.byte & 0x00;
	//电量预警阈值设置
	//ADC_Para.Warn_value = 10;
	//温度预警阈值设置
	//Temp_Para.Warn_value = 60;
	//上报命令码清零
    Comm_Para.cmd =0;
	//GSM模块复位
	GSM_Module_STATE = GSM_STATE_Reset;
	//上报标志位配置
	MQTT_Upload_f.bits.MQTT_Upload_voltageInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WatseInfo_timing_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WatseTran_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_MachInfo_f = 1;		//开机上报一次
	MQTT_Upload_f.bits.MQTT_Upload_Parament_f = 1;		//开机上报一次
//	MQTT_Upload_f.bits.MQTT_Upload_WatseInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_Errmsg_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WarnvoltageInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_TempInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_TempWInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_SensorWInfo_f = 0;
	
	MQTT_Upload_f.bits.MQTT_Upload_UserID_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_DownloadEnableInfo_Ack_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WasteAdd_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WasteReduce_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_MachReduce_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_UnknowChangeWeight_f = 0;

	MQTT_Upload_f.bits.MQTT_Upload_GasWarnInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_FillWarnInfo_f = 0;
	
	MQTT_Upload_f.bits.MQTT_Upload_Mach_Restart_Ack_f = 0;
	
	display_flag=1; 
	
	//上一次传感器状态置一，确保联网后会重新上报故障数据
    //Sensor_WrongNum_f_old.byte = (~Sensor_WrongNum_f.byte)&0x0f;
	Voltage_Warn_old = 1;	
	Temp_Warn_old = 1;
	Sensor_Warn_old = 1;
	Upload_f = 1;
	SigSTR_retry_Counter = 0;
	Get_time_f = 1;
	
	User_Msg.WasteID = 0x10;			//开机初始化为非0/1的值，
	
	User_Msg.UserID_Result = 0x00;		//初始状态default
	
	Door_State.state = DOOR_CLOSE;		//初始化2个门状态为关闭
	
	MQTT_Busy_Flag = 0;		//心跳包标志初始化为空闲状态
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
void Workmode_Init(void)
{
	//SYSTint();
	SYS_GPIO_Init();		    //IO初始化   外设，GSM供电，数码管IO
	wdh0_init();      			//初始化看门狗并开看门狗
	uart_init();			    //UART0:串口屏		UART1:GSM串口初始化 波特率9600
	lpuart_init();				//LPUART0:二维码	LPUART1:IC卡
	I2C_GPIO_Init();			//称重I2C配置
	Sensor_ParaInit();			//称重参数初始化
	W_TIM0_int();		        //1ms中断初始化
	ADC_init();					//ADC初始化  电量采集和温度采集
	SPI0_Init();				//初始化外部flash所用SPI0	
//	TIM3_Init();				//蜂鸣器初始化
	RUN_Waitms = 1;				//3分钟超时计时开始
	RUN_Stop_f = 1;
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
void First_Poweron_Init(void)
{
	Workmode_Init();							//外设初始化
	Flash_Init_U();								//外部FLASH读写
	Screen_RS232_INT(); 						//串口屏参数初始化
	Poweron_Parament_Init();					//开机参数初始化

	//初始状态，关闭两个门
	BSP_TG_Choose_Deal(TG_CH_1, CLOSE);
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
void Opendoor_Mode(void)
{
	//1ms 任务
	if(Systime_f.bits.systime1ms_flag == 1)
	{
		Systime_f.bits.systime1ms_flag = 0;
		//联网
		GSM_Module();
	}
	
    //50ms任务  串口屏通信
	if(Systime_f.bits.systime50ms_flag==1)
	{
		Systime_f.bits.systime50ms_flag=0;				
		Screen_RS232_Module(); 	
	}
	//100ms任务  
	if(Systime_f.bits.systime100ms_flag == 1)
	{
		Systime_f.bits.systime100ms_flag=0;
		if(TM7711_wait_count == 0)
		{
			WB_Get();    //称重
			TM7711_wait_count = 1;
		}
		else if((TM7711_wait_count != 0)&&(TM7711_wait_count < 5))
		{
			TM7711_wait_count++; 
		}
		if(TM7711_wait_count >= 5)
		{
			WB_Get(); 
		}
		Door_Check(); //门检测
		Display_main();		//显示	
		//去皮和清零
		Remove_the_peel();		
	}
}
