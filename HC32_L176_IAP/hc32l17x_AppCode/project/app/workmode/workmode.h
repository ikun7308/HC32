#ifndef _WORKMODE_H_
#define _WORKMODE_H_
#include "include.h"


///****************************************/
//״̬�л�����
typedef enum
{
	DEEPSLEEP_MODE		    =   1,										//����ģʽ
	CLOSEDOOR_MODE		    =	2,										//����ģʽ
	OPENDOOR_MODE			=	3,									//����ģʽ
	UPLOADTIMING_MODE		=	4									    //��ʱ�ϱ�ģʽ
}Module_STATE_t;
extern Module_STATE_t Module_STATE;


typedef union
{
	uint8_t byte;
	struct
	{	
		uint8_t WarnW_fir            :1;              //ͨ��1����Ԥ��
		uint8_t Temp_Warn			 :1;					//�¶�Ԥ��
		
	} bits;    
} Warn_Upload_f_t;

typedef union
{
	uint32_t byte;
	struct
	{
		uint8_t MQTT_Upload_WatseInfo_timing_f		:1;    	    	//��ʱ�ϱ���־λ
		uint8_t MQTT_Upload_WatseTran_f				:1;	      		//������ɱ�־λ   		
        uint8_t MQTT_Upload_MachInfo_f          	:1;             //�豸��Ϣ�ϱ���־λ
        uint8_t MQTT_Upload_Parament_f      		:1;             //������Ϣ�ϱ���־λ		3150
		
        uint8_t MQTT_Upload_WatseInfo_f      		:1;             //һ�������ϱ�
		uint8_t MQTT_Upload_Errmsg_f      			:1;             //�ϴ�����Ԥ����־λ
		uint8_t MQTT_Upload_voltageInfo_f			:1; 			//�����ϱ���־λ
		uint8_t MQTT_Upload_WarnvoltageInfo_f		:1; 			//����Ԥ���ϱ���־λ
		
		uint8_t MQTT_ParamentAck_f      			:1;             //������Ϣ���ûظ���־λ	3151
		uint8_t MQTT_Upload_TempInfo_f				:1;				//�¶��ϴ���־λ
		uint8_t MQTT_Upload_TempWInfo_f				:1;				//�¶�Ԥ���ϴ���־λ
		uint8_t MQTT_Upload_SensorWInfo_f			:1;				//Sensor�ϴ���־λ
		
		uint8_t MQTT_Upload_UserID_f				:1;				//�û�ID�ϴ���־λ
		uint8_t MQTT_Upload_DownloadEnableInfo_Ack_f	:1;			//�豸����Ӧƽ̨�·����û�IDȷ��������
		uint8_t MQTT_Upload_WasteAdd_f				:1;				//�û�����Ͷ������������־λ
		uint8_t MQTT_Upload_WasteReduce_f			:1;				//����Ա������������(����)��־λ
		
		uint8_t MQTT_Upload_MachReduce_f			:1;				//�豸�쳣й¶��־λ
		uint8_t MQTT_Upload_UnknowChangeWeight_f	:1;				//δ֪ԭ�������仯�ϱ���־λ

		uint8_t MQTT_Upload_GasWarnInfo_f			:1;				//�������ϱ���־λ
		uint8_t MQTT_Upload_FillWarnInfo_f			:1;				//���������ϱ���־λ
		uint8_t MQTT_Upload_Mach_Restart_Ack_f		:1;
		
	} bits;   
} MQTT_Upload_f_t;

//typedef union
//{
//	uint32_t  byte;
//	struct
//	{
//      	uint8_t Displaywrong_f          	:1;					//���ϴ���1����ʾ���ϣ���е3.0����ʾ���ϼ��
//		uint8_t CheckSIMStatewrong_f        :1;					//���ϴ���2��SIM��״̬���ʧ��
//		uint8_t GetCCIDNumwrong_f          	:1;					//���ϴ���2����ȡCCID����ʧ��
//		uint8_t GetGPRSwrong_f            	:1;					//���ϴ���2����վ��ַ��ȡʧ��
//		uint8_t OpenCCIRwrong_f             :1;					//���ϴ���2�������ƶ�����ʧ��
//		uint8_t GSMCOMwrong_f             	:1;					//���ϴ���3��GSMͨ��ģ����ϣ�ATָ��ͨѶʧ��
//       	uint8_t Signalwrong_f            	:1;					//���ϴ���4��ͨѶ�ź�����ͨѶ�ź���           
//       	uint8_t Networking1wrong_f          :1;					//���ϴ���5������1ʧ��
//		uint8_t Networking2wrong_f          :1;					//���ϴ���6������2ʧ��
//		uint8_t Networking3wrong_f          :1;					//���ϴ���7������3ʧ��
//		uint8_t FLASH_Wrong_f       		:1;	 				//���ϴ���8��FLASH���ϣ���е3.0��Flash���ϼ��

//       	uint8_t Sensor_Wrong_f_1          	:1;					//���ϴ���9��1 ͨ������������
//		uint8_t Sensor_Wrong_f_2          	:1;					//���ϴ���10��2 ͨ������������
//		uint8_t Sensor_Wrong_f_3          	:1;					//���ϴ���11��3 ͨ������������
//		uint8_t Sensor_Wrong_f_4       		:1;	 				//���ϴ���12��4 ͨ������������
//       	uint8_t Leak_Wrong_f_1          	:1;					//���ϴ���13���ɻ������� й¶  
//		uint8_t Leak_Wrong_f_2          	:1;					//���ϴ���14���к����� й¶  
//		uint8_t Leak_Wrong_f_3          	:1;					//���ϴ���15���������� й¶
//		uint8_t Leak_Wrong_f_4       		:1;	 				//���ϴ���16����������й¶
//	
//		uint8_t SensorWarning				:1;					//sensorԤ��
//		uint8_t reserve1					:4;					//����
//		uint8_t reserve2					:8;					//����
//	} bits;
//} Wrong_Num_f_t;				//ͨ�Ź���
extern uint8_t Voltage_Warn_old;              //����Ԥ����־λ

typedef union
{
	uint8_t  byte;
	struct
	{
		uint8_t SensorWarning_fir       			:1;              //ͨ��1��������ʧ
		
	} bits;
} Sensor_WrongNum_f_t;				//ͨ�Ź���


typedef union
{
	uint8_t byte;
	struct
	{
        uint8_t systime1ms_flag           		:1;             //1ms�жϱ�־λ
		uint8_t systime10ms_flag      			:1;             //10ms�жϱ�־λ
        uint8_t systime50ms_flag      			:1;             //50ms�жϱ�־λ 
        uint8_t systime100ms_flag      			:1;             //100ms�жϱ�־λ
		uint8_t systime200ms_flag      			:1;             //200ms�жϱ�־λ 
		uint8_t systime500ms_flag      			:1;             //200ms�жϱ�־λ 
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
       	uint8_t Upgrad_f           				:1;    				//����������־λ        
     	uint8_t RTC_wakeup_f           			:1;   				//RTC���ѱ�־λ
		uint8_t IO_wakeup_f           			:1;   				//IO���ѱ�־λ
		uint8_t Deepsleep_f           			:1;  				//������߱�־λ
		uint8_t mainloop_1ms_f					:1;
		
																  
	} bits;
} Mode_f_t;

typedef union
{
	unsigned char byte;
	struct
	{
		uint8_t Voltage_Get      		:1;             //������ȡ��־λ
		uint8_t Sensor_Change_Flag      :1;             //�������ı��־λ
		uint8_t door_open_flag			:1;				//�Źر�־λ
		uint8_t Door_Timeout_f         	:1;  			//�����ų�ʱ���־λ
		uint8_t IO_interrupt_f         	:1;  			//�����ų�ʱ���־λ
		uint8_t RTC_interrupt_f		    :1;				//
		uint8_t Temp_Get_f		    	:1;				//�¶Ȳɼ���־λ
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
extern uint8_t Last_DOORSTATE;    //��һ����״̬
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
