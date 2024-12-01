#include "include.h"


//==============================================================================
//函数名称：void Gpio_Set_IN(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//功能说明：配置管脚为输入管脚
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void Gpio_Set_IN(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	stcGpioCfg.enDir = GpioDirIn; 												//端口方向配置->输入
	stcGpioCfg.enDrv = GpioDrvH;                                                //端口驱动能力配置->高驱动能力
	stcGpioCfg.enPuPd = GpioPu;	                                                //端口上下拉配置->上拉 
	stcGpioCfg.enOD = GpioOdEnable;											//端口开漏输出配置->开漏输出关闭
	stcGpioCfg.enCtrlMode = GpioAHB;											//端口输入/输出值寄存器总线控制模式配置->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    
	
}
//==============================================================================
//函数名称：void Gpio_Set_IN_D(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//功能说明：配置管脚为输入管脚
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void Gpio_Set_IN_D(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	stcGpioCfg.enDir = GpioDirIn; 												//端口方向配置->输入
	stcGpioCfg.enDrv = GpioDrvL;                                                //端口驱动能力配置->高驱动能力
	stcGpioCfg.enPuPd = GpioPd;	                                                //端口上下拉配置->上拉 
	stcGpioCfg.enOD = GpioOdDisable;											//端口开漏输出配置->开漏输出关闭
	stcGpioCfg.enCtrlMode = GpioAHB;											//端口输入/输出值寄存器总线控制模式配置->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    

}
//==============================================================================
//函数名称：void Gpio_Set_OUT(en_gpio_port_t enPort, en_gpio_pin_t enPin)
//功能说明：配置管脚为输出管脚
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void Gpio_Set_OUT(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
	stc_gpio_config_t	stcGpioCfg;
	stcGpioCfg.enDir = GpioDirOut; 												//端口方向配置->输入
	stcGpioCfg.enDrv = GpioDrvL;                                                //端口驱动能力配置->高驱动能力
	stcGpioCfg.enPuPd = GpioPu;	                                                //端口上下拉配置->上拉 
	stcGpioCfg.enOD = GpioOdDisable;											//端口开漏输出配置->开漏输出关闭
	stcGpioCfg.enCtrlMode = GpioAHB;											//端口输入/输出值寄存器总线控制模式配置->AHB
	Gpio_Init(enPort,enPin, &stcGpioCfg);    
}
//==============================================================================
//函数名称：void KeyWakeup_Config(void)
//功能说明：按键中断配置
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
//==============================================================================
void KeyWakeup_Config(void)
{
//	Gpio_Set_IN(GpioPortB, GpioPin13);
//    Gpio_SfIrqModeConfig(GpioSfIrqDpslpMode);	                                //深度休眠模式下响应端口中断   
//    Gpio_EnableIrq(GpioPortB, GpioPin13, GpioIrqFalling);	                    //打开并配置PB13为下降沿中断,倾倒开关                
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
//函数名称：void PORTD_IRQHandler(void)
//功能说明：IO1唤醒以及进入低功耗
//输入变量：无
//返回变量：无
//内部变量：
//调用位置：
//调试记录：
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




