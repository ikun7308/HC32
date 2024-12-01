#ifndef _WORKMODE_H_
#define _WORKMODE_H_
#include "include.h"


///****************************************/
//状态切换常量
typedef enum
{
	DEEPSLEEP_MODE		    =   1,										//休眠模式
	CLOSEDOOR_MODE		    =	2,										//关门模式
	OPENDOOR_MODE			=	3,									//开门模式
	UPLOADTIMING_MODE		=	4									    //定时上报模式
}Module_STATE_t;
extern Module_STATE_t Module_STATE;


typedef union
{
	uint8_t byte;
	struct
	{	
		uint8_t WarnW_fir            :1;              //通道1重量预警
		uint8_t Temp_Warn			 :1;					//温度预警
		
	} bits;    
} Warn_Upload_f_t;

typedef union
{
	uint32_t byte;
	struct
	{
		uint8_t MQTT_Upload_WatseInfo_timing_f		:1;    	    	//定时上报标志位
		uint8_t MQTT_Upload_WatseTran_f				:1;	      		//运输完成标志位   		
        uint8_t MQTT_Upload_MachInfo_f          	:1;             //设备信息上报标志位
        uint8_t MQTT_Upload_Parament_f      		:1;             //参数信息上报标志位		3150
		
        uint8_t MQTT_Upload_WatseInfo_f      		:1;             //一般重量上报
		uint8_t MQTT_Upload_Errmsg_f      			:1;             //上传重量预警标志位
		uint8_t MQTT_Upload_voltageInfo_f			:1; 			//电量上报标志位
		uint8_t MQTT_Upload_WarnvoltageInfo_f		:1; 			//电量预警上报标志位
		
		uint8_t MQTT_ParamentAck_f      			:1;             //参数信息设置回复标志位	3151
		uint8_t MQTT_Upload_TempInfo_f				:1;				//温度上传标志位
		uint8_t MQTT_Upload_TempWInfo_f				:1;				//温度预警上传标志位
		uint8_t MQTT_Upload_SensorWInfo_f			:1;				//Sensor上传标志位
		
		uint8_t MQTT_Upload_UserID_f				:1;				//用户ID上传标志位
		uint8_t MQTT_Upload_DownloadEnableInfo_Ack_f	:1;			//设备端响应平台下发的用户ID确认命令码
		uint8_t MQTT_Upload_WasteAdd_f				:1;				//用户垃圾投放增加重量标志位
		uint8_t MQTT_Upload_WasteReduce_f			:1;				//管理员清理运输垃圾(减少)标志位
		
		uint8_t MQTT_Upload_MachReduce_f			:1;				//设备异常泄露标志位
		uint8_t MQTT_Upload_UnknowChangeWeight_f	:1;				//未知原因重量变化上报标志位

		uint8_t MQTT_Upload_GasWarnInfo_f			:1;				//烟雾报警上报标志位
		uint8_t MQTT_Upload_FillWarnInfo_f			:1;				//溢满报警上报标志位
		uint8_t MQTT_Upload_Mach_Restart_Ack_f		:1;
		
	} bits;   
} MQTT_Upload_f_t;

//typedef union
//{
//	uint32_t  byte;
//	struct
//	{
//      	uint8_t Displaywrong_f          	:1;					//故障代码1：显示故障，机械3.0无显示故障检测
//		uint8_t CheckSIMStatewrong_f        :1;					//故障代码2：SIM卡状态检测失败
//		uint8_t GetCCIDNumwrong_f          	:1;					//故障代码2：读取CCID卡号失败
//		uint8_t GetGPRSwrong_f            	:1;					//故障代码2：基站地址读取失败
//		uint8_t OpenCCIRwrong_f             :1;					//故障代码2：激活移动场景失败
//		uint8_t GSMCOMwrong_f             	:1;					//故障代码3：GSM通信模块故障，AT指令通讯失败
//       	uint8_t Signalwrong_f            	:1;					//故障代码4：通讯信号弱，通讯信号弱           
//       	uint8_t Networking1wrong_f          :1;					//故障代码5：联网1失败
//		uint8_t Networking2wrong_f          :1;					//故障代码6：联网2失败
//		uint8_t Networking3wrong_f          :1;					//故障代码7：联网3失败
//		uint8_t FLASH_Wrong_f       		:1;	 				//故障代码8：FLASH故障，机械3.0无Flash故障检测

//       	uint8_t Sensor_Wrong_f_1          	:1;					//故障代码9：1 通道传感器故障
//		uint8_t Sensor_Wrong_f_2          	:1;					//故障代码10：2 通道传感器故障
//		uint8_t Sensor_Wrong_f_3          	:1;					//故障代码11：3 通道传感器故障
//		uint8_t Sensor_Wrong_f_4       		:1;	 				//故障代码12：4 通道传感器故障
//       	uint8_t Leak_Wrong_f_1          	:1;					//故障代码13：可回收垃圾 泄露  
//		uint8_t Leak_Wrong_f_2          	:1;					//故障代码14：有害垃圾 泄露  
//		uint8_t Leak_Wrong_f_3          	:1;					//故障代码15：厨余垃圾 泄露
//		uint8_t Leak_Wrong_f_4       		:1;	 				//故障代码16：其他垃圾泄露
//	
//		uint8_t SensorWarning				:1;					//sensor预警
//		uint8_t reserve1					:4;					//保留
//		uint8_t reserve2					:8;					//保留
//	} bits;
//} Wrong_Num_f_t;				//通信故障
extern uint8_t Voltage_Warn_old;              //电量预警标志位

typedef union
{
	uint8_t  byte;
	struct
	{
		uint8_t SensorWarning_fir       			:1;              //通道1传感器丢失
		
	} bits;
} Sensor_WrongNum_f_t;				//通信故障


typedef union
{
	uint8_t byte;
	struct
	{
        uint8_t systime1ms_flag           		:1;             //1ms中断标志位
		uint8_t systime10ms_flag      			:1;             //10ms中断标志位
        uint8_t systime50ms_flag      			:1;             //50ms中断标志位 
        uint8_t systime100ms_flag      			:1;             //100ms中断标志位
		uint8_t systime200ms_flag      			:1;             //200ms中断标志位 
		uint8_t systime500ms_flag      			:1;             //200ms中断标志位 
		uint8_t systime1s_flag					:1;
//		uint8_t systime10s_flag                 :1;
		uint8_t systime1min_flag				:1;

		

	} bits;   
} Systime_f_t;

typedef union
{
	uint8_t byte;
	struct
	{
       	uint8_t Upgrad_f           				:1;    				//程序升级标志位        
     	uint8_t RTC_wakeup_f           			:1;   				//RTC唤醒标志位
		uint8_t IO_wakeup_f           			:1;   				//IO唤醒标志位
		uint8_t Deepsleep_f           			:1;  				//深度休眠标志位
		uint8_t mainloop_1ms_f					:1;
		
																  
	} bits;
} Mode_f_t;

typedef union
{
	unsigned char byte;
	struct
	{
		uint8_t Voltage_Get      		:1;             //电量获取标志位
		uint8_t Sensor_Change_Flag      :1;             //传感器改变标志位
		uint8_t door_open_flag			:1;				//门关标志位
		uint8_t Door_Timeout_f         	:1;  			//开关门超时检标志位
		uint8_t IO_interrupt_f         	:1;  			//开关门超时检标志位
		uint8_t RTC_interrupt_f		    :1;				//
		uint8_t Temp_Get_f		    	:1;				//温度采集标志位
		uint8_t Opendoor_deepsleep_f	:1;
	} bits;
} Surplus_f_t;
extern Warn_Upload_f_t Warn_Upload_f;
extern Warn_Upload_f_t Warn_Upload_f_old;
extern MQTT_Upload_f_t MQTT_Upload_f;
extern Sensor_WrongNum_f_t Sensor_WrongNum_f;

//extern Wrong_Num_f_t Wrong_Num_f;
//extern Wrong_Num_f_t Wrong_Num_f_Old;

extern Mode_f_t Mode_f;
extern Surplus_f_t Surplus_f;
extern Systime_f_t Systime_f;
extern uint8_t Last_DOORSTATE;    //上一个门状态
extern uint8_t Temp_Warn_old; 
extern uint8_t Sensor_Warn_old; 
extern uint8_t TM7711_wait_count;

extern void clrwspowere(void);
extern void Poweron_Parament_Init(void);
extern void Workmode_Init(void);
extern void Opendoor_Parament_Init(void);
extern void First_Poweron_Init(void);
extern void Opendoor_Mode(void);
extern void Closedoor_Mode(void);
extern void Deepsleep_Mode(void);
extern void Uploadtiming_Mode(void);
#endif
