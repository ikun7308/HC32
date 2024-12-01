#include "include.h"


//==============================================================================
//�������ƣ�void Gpio_Set_IN(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//����˵�������ùܽ�Ϊ����ܽ�
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void Gpio_Set_IN(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	stcGpioCfg.enDir = GpioDirIn; 												//�˿ڷ�������->����
	stcGpioCfg.enDrv = GpioDrvH;                                                //�˿�������������->����������
	stcGpioCfg.enPuPd = GpioPu;	                                                //�˿�����������->���� 
	stcGpioCfg.enOD = GpioOdEnable;											//�˿ڿ�©�������->��©����ر�
	stcGpioCfg.enCtrlMode = GpioAHB;											//�˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    
	
}
//==============================================================================
//�������ƣ�void Gpio_Set_IN_D(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//����˵�������ùܽ�Ϊ����ܽ�
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void Gpio_Set_IN_D(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	stcGpioCfg.enDir = GpioDirIn; 												//�˿ڷ�������->����
	stcGpioCfg.enDrv = GpioDrvL;                                                //�˿�������������->����������
	stcGpioCfg.enPuPd = GpioPd;	                                                //�˿�����������->���� 
	stcGpioCfg.enOD = GpioOdDisable;											//�˿ڿ�©�������->��©����ر�
	stcGpioCfg.enCtrlMode = GpioAHB;											//�˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    

}
//==============================================================================
//�������ƣ�void Gpio_Set_OUT(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//����˵�������ùܽ�Ϊ����ܽ�
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void Gpio_Set_OUT(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	stcGpioCfg.enDir = GpioDirOut; 												//�˿ڷ�������->����
	stcGpioCfg.enDrv = GpioDrvL;                                                //�˿�������������->����������
	stcGpioCfg.enPuPd = GpioPu;	                                                //�˿�����������->���� 
	stcGpioCfg.enOD = GpioOdDisable;											//�˿ڿ�©�������->��©����ر�
	stcGpioCfg.enCtrlMode = GpioAHB;											//�˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    
}
//==============================================================================
//�������ƣ�void KeyWakeup_Config(void)
//����˵���������ж�����
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================
void KeyWakeup_Config(void)
{
//	Gpio_Set_IN(GpioPortB, GpioPin13);
//    Gpio_SfIrqModeConfig(GpioSfIrqDpslpMode);	                                //�������ģʽ����Ӧ�˿��ж�   
//    Gpio_EnableIrq(GpioPortB, GpioPin13, GpioIrqFalling);	                    //�򿪲�����PB13Ϊ�½����ж�,�㵹����                
//    EnableNvic(PORTB_IRQn, IrqLevel3, TRUE);
}


void Opendoor_Parament_Init(void)
{
	MQTT_Upload_f.bits.MQTT_Upload_voltageInfo_f = 1;
	MQTT_DisCONNECT_STA = 0;
}

void Closedoor_Parament_Init(void)
{
	MQTT_Upload_f.bits.MQTT_Upload_voltageInfo_f = 1;	
	MQTT_DisCONNECT_STA = 0;
}

//==============================================================================
//�������ƣ�void PORTD_IRQHandler(void)
//����˵����IO1�����Լ�����͹���
//�����������
//���ر�������
//�ڲ�������
//����λ�ã�
//���Լ�¼��
//==============================================================================

		
void Gpio_IRQHandler(uint8_t u8Param)
{
//	if(1 == u8Param)
//    {
////        if(TRUE == Gpio_GetIrqStatus(GpioPortB, GpioPin13))
////        {            
////			W_TIM0_int();
////			
////			//Surplus_f.bits.Door_Timeout_f = 1;
////	
////			Gpio_ClearIrq(GpioPortB, GpioPin13);
////			
////		}
//	}
}




