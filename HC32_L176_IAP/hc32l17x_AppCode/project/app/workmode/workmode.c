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
uint8_t Last_DOORSTATE = 0;    //��һ����״̬  �״ο�����ʼ��Ϊ3

uint8_t TM7711_wait_count = 0;

//==============================================================================
//�������ƣ�void RtcCycCb(void)
//����˵����RTC�����жϷ�����
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
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
//�������ƣ�void Poweron_Parament_Init(void)
//����˵��������������ʼ��
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void Poweron_Parament_Init(void)
{
    memset(MQTT_REC_BUFF,0x00,512);
	//����������
	Wrong_Num_f.byte = Wrong_Num_f.byte & 0x0000;
	//Sensor_WrongNum_f.bits.SensorWarning_fir = 0;
	//Wrong_Num_f.bits.SensorWarning = 0;
	
	Surplus_f.byte = Surplus_f.byte & 0x00;
	Surplus_f.bits.Temp_Get_f = 1;
	//ʱ���׼��־λ����
	Systime_f.byte = Systime_f.byte & 0x00;
	//����Ԥ����ֵ����
	//ADC_Para.Warn_value = 10;
	//�¶�Ԥ����ֵ����
	//Temp_Para.Warn_value = 60;
	//�ϱ�����������
    Comm_Para.cmd =0;
	//GSMģ�鸴λ
	GSM_Module_STATE = GSM_STATE_Reset;
	//�ϱ���־λ����
	MQTT_Upload_f.bits.MQTT_Upload_voltageInfo_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WatseInfo_timing_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_WatseTran_f = 0;
	MQTT_Upload_f.bits.MQTT_Upload_MachInfo_f = 1;		//�����ϱ�һ��
	MQTT_Upload_f.bits.MQTT_Upload_Parament_f = 1;		//�����ϱ�һ��
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
	
	//��һ�δ�����״̬��һ��ȷ��������������ϱ���������
    //Sensor_WrongNum_f_old.byte = (~Sensor_WrongNum_f.byte)&0x0f;
	Voltage_Warn_old = 1;	
	Temp_Warn_old = 1;
	Sensor_Warn_old = 1;
	Upload_f = 1;
	SigSTR_retry_Counter = 0;
	Get_time_f = 1;
	
	User_Msg.WasteID = 0x10;			//������ʼ��Ϊ��0/1��ֵ��
	
	User_Msg.UserID_Result = 0x00;		//��ʼ״̬default
	
	Door_State.state = DOOR_CLOSE;		//��ʼ��2����״̬Ϊ�ر�
	
	MQTT_Busy_Flag = 0;		//��������־��ʼ��Ϊ����״̬
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
void Workmode_Init(void)
{
	//SYSTint();
	SYS_GPIO_Init();		    //IO��ʼ��   ���裬GSM���磬�����IO
	wdh0_init();      			//��ʼ�����Ź��������Ź�
	uart_init();			    //UART0:������		UART1:GSM���ڳ�ʼ�� ������9600
	lpuart_init();				//LPUART0:��ά��	LPUART1:IC��
	I2C_GPIO_Init();			//����I2C����
	Sensor_ParaInit();			//���ز�����ʼ��
	W_TIM0_int();		        //1ms�жϳ�ʼ��
	ADC_init();					//ADC��ʼ��  �����ɼ����¶Ȳɼ�
	SPI0_Init();				//��ʼ���ⲿflash����SPI0	
//	TIM3_Init();				//��������ʼ��
	RUN_Waitms = 1;				//3���ӳ�ʱ��ʱ��ʼ
	RUN_Stop_f = 1;
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
void First_Poweron_Init(void)
{
	Workmode_Init();							//�����ʼ��
	Flash_Init_U();								//�ⲿFLASH��д
	Screen_RS232_INT(); 						//������������ʼ��
	Poweron_Parament_Init();					//����������ʼ��

	//��ʼ״̬���ر�������
	BSP_TG_Choose_Deal(TG_CH_1, CLOSE);
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
void Opendoor_Mode(void)
{
	//1ms ����
	if(Systime_f.bits.systime1ms_flag == 1)
	{
		Systime_f.bits.systime1ms_flag = 0;
		//����
		GSM_Module();
	}
	
    //50ms����  ������ͨ��
	if(Systime_f.bits.systime50ms_flag==1)
	{
		Systime_f.bits.systime50ms_flag=0;				
		Screen_RS232_Module(); 	
	}
	//100ms����  
	if(Systime_f.bits.systime100ms_flag == 1)
	{
		Systime_f.bits.systime100ms_flag=0;
		if(TM7711_wait_count == 0)
		{
			WB_Get();    //����
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
		Door_Check(); //�ż��
		Display_main();		//��ʾ	
		//ȥƤ������
		Remove_the_peel();		
	}
}
