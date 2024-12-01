//******************************************************************************
//
//*	文件名：  
//
//*	功能描述:
//
//* 作者: 王婵媛  版本: 	日期: 2018.12.24
//
//*	说明:  
//
//******************************************************************************
#include "main.h"
#include "modem.h"
//==============================================================================
uint16_t u16Crc16 = 0; 
uint32_t u32FrameSize=0;
uint32_t u32packSize=0;
uint32_t u32FlashDataFlag = 0;
uint8_t u8Ret=0;
uint32_t read_temp=0;
uint16_t GSM_Upload_send_buff_count=0;
uint16_t ANDROID_send_buff_count=0;
uint16_t ANDROID_send_buff_len1 = 0;
uint8_t ANDROID_SEND_RUN_Flag=0;
uint8_t usemsg1[usemsg_MAX] = {0x00};
uint8_t usemsg2[usemsg_MAX] = {0x00};
uint8_t usemsg3[usemsg_MAX] = {0x00};
uint8_t usemsg4[usemsg_MAX] = {0x00};
uint8_t imgurl1[10] = {0x00};
uint8_t imgurl2[10] = {0x00};
uint8_t imgurl3[10] = {0x00};
uint8_t imgurl4[10] = {0x00};

uint8_t opengate[8] = {'o','p','e','n','g','a','t','e'};
uint8_t usemsg[6] = {'u','s','e','m','s','g'};
uint8_t garbagetype[11] = {'g','a','r','b','a','g','e','t','y','p','e'};
uint8_t photogate[9] = {'p','h','o','t','o','g','a','t','e'};
uint8_t imgurl[6] = {'i','m','g','u','r','l'};

uint8_t     RWM_SERVER_rec_buf[GSM_SERVER_rec_buf_count_MAX] = {0};
uint8_t  		ANDROID_send_buff1[ANDROIN_SERVER_rec_buf_count_MAX] = {0x00};
uint8_t     RWM_SERVER_rec_buf_info[16] = {0};
uint16_t    RWM_SERVER_rec_buf_count = 0;   
uint16_t    RWM_SERVER_rec_buf_count1 = 0; //????? 
uint8_t     RWM_receive_finish = 0;  
uint8_t		  RWM_receive_f = 0;	
uint16_t	  RWM_receive_T = 0; 
uint8_t     GSM_SERVER_rec_buf[GSM_SERVER_rec_buf_count_MAX] = {0};
uint16_t    GSM_SERVER_rec_buf_count = 0;  
uint16_t    GSM_buf_len;  
uint16_t    GSM_SERVER_rec_buf_count1 = 0; //????? 
uint8_t     GSM_receive_finish = 0;                                                                 //???????
uint16_t		GSM_receive_T = 0;    					 	   											//?????
uint8_t			GSM_receive_f = 0;	
uint8_t			woke_mode = 0;
uint8_t			Door = 0;
uint8_t 		imgurl_count = 0;
uint8_t 		imgur_count[4] = {0};

uint8_t garbagetype0 = 0x00;
uint8_t garbagetype1 = 0x00;
uint8_t garbagetype2 = 0x00;
uint8_t garbagetype3 = 0x00;

volatile static uint8_t u8RxData1;
volatile static uint8_t u8TxCnt=0;
volatile static uint8_t u8RxCnt=0;


uint8_t u8TxData[2] = {0x00,0x55};
uint8_t u8RxData = 00;

uint8_t     YY_buf[YY_buf_count_MAX] = {0};
uint8_t 		YY_count = 0;
uint8_t 		YY_buf_len = 0;


uint8_t     CKP_buf[CKP_buf_count_MAX] = {0};
uint8_t 		CKP_count = 0;
uint8_t 		CKP_buf_len = 0;

uint8_t JKFS0 = 0x00;
uint8_t JKFS1 = 0x00;
uint8_t JKFS2 = 0x00;
uint8_t JKFS3 = 0x00;

uint16_t msgNo = 0;

uint8_t LAT[9] = {0};
uint8_t LON[9] = {0};


void LpUart0_IRQHandler(void)
{
    if(LPUart_GetStatus(M0P_LPUART0, LPUartTC))
    {
			LPUart_ClrStatus(M0P_LPUART0, LPUartTC);
			CKP_count++;
			if(CKP_count >= CKP_buf_len)
			{
				CKP_count = 0;
				memset(CKP_buf,0x00,CKP_buf_len);
				CKP_buf_len = 0;
//				Uart_DisableIrq(M0P_UART3,UartTxIrq);///<??????
//				Uart_ClrStatus(M0P_UART3, UartTC);
//        Uart_EnableIrq(M0P_UART3,UartTxIrq); ///<??????
				
			}
			else
			{
				M0P_LPUART0->SBUF_f.DATA = CKP_buf[CKP_count]; 							
			}	
    }
    
    if(LPUart_GetStatus(M0P_LPUART0, LPUartRC))    ///????
    {
        LPUart_ClrStatus(M0P_LPUART0, LPUartRC);   ///<???????
        u8RxData = LPUart_ReceiveData(M0P_LPUART0);///????
     
    }
}

void Uart0_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART0, UartRC))         //UART1????
    {
        Uart_ClrStatus(M0P_UART0, UartRC);        //??????

				u8RxData1 = Uart_ReceiveData(M0P_UART0);   //??????		 

        GSM_SERVER_rec_buf[GSM_SERVER_rec_buf_count]= u8RxData1;
				GSM_SERVER_rec_buf_count++;
				
				if(GSM_SERVER_rec_buf_count >= GSM_SERVER_rec_buf_count_MAX)			//防止溢出
				{
					GSM_SERVER_rec_buf_count = 0;
				}
				GSM_receive_f = 1;					
    }
    
    if(Uart_GetStatus(M0P_UART0, UartTC))         //UART1????
    {
			Uart_ClrStatus(M0P_UART0, UartTC); 
			
			GSM_Upload_send_buff_count++;
			if(GSM_Upload_send_buff_count >= GSM_Upload_send_buff_len1)
			{
				GSM_Upload_send_buff_count=0;
				GSM_SEND_RUN_Flag = 0;
					
			}
			else
			{
				M0P_UART0->SBUF_f.DATA = GSM_Upload_send_buff1[GSM_Upload_send_buff_count];						
			}	

    }

}
//==============================================================================
//????:void App_Port3Init(void)
//????:Uart3
//????:语音模块
//????:
//????:
//????:
//????:
//=============================================================================

//??????
void App_Port0Init(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //??GPIO????
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf1);          //??PA02 ???URART1_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
//    stcGpioCfg.enDrv = GpioDrvL;
//    ///< ???????->?
//    stcGpioCfg.enPu = GpioPuDisable;
//    stcGpioCfg.enPd = GpioPdDisable;
//    ///< ????????->??????
//    stcGpioCfg.enOD = GpioOdDisable;	
    Gpio_Init(GpioPortA, GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf1);          //??PA03 ???URART1_RX
}
//==============================================================================
//????:void App_Uart3Cfg(void)
//????:Uart3
//????:语音模块
//????:
//????:
//????:
//????:
//=============================================================================

//????
void App_Uart0Cfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///< ??????
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);///<??uart1????
    
    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;          ///<??3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bit???
    stcCfg.enMmdorCk        = UartMskDataOrAddr;           ///<???
    stcCfg.stcBaud.u32Baud  = 115200;                  ///<???9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<????????
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<??????(PCLK)???
    Uart_Init(M0P_UART0, &stcCfg);                   ///<?????
    
    ///<UART????
    Uart_ClrStatus(M0P_UART0,UartRC);                ///<?????
    Uart_ClrStatus(M0P_UART0,UartTC);                ///<?????
    Uart_EnableIrq(M0P_UART0,UartRxIrq);             ///<????????
    Uart_EnableIrq(M0P_UART0,UartTxIrq);             ///<????????    
    EnableNvic(UART0_2_IRQn, IrqLevel3, TRUE);       ///<??????

}
//==============================================================================
//????:void App_LpUartCfg(void)
//????:LpUart1
//????:二维码
//????:
//????:
//????:
//????:
//=============================================================================

///< LPUART??
void App_LpUartCfg(void)
{
    stc_lpuart_cfg_t  stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///<????????
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart1,TRUE);    
    
    ///<LPUART ???
    stcCfg.enStopBit = LPUart1bit;                   ///<1???    
    stcCfg.enMmdorCk = LPUartDataOrAddr;                   ///<???
    stcCfg.stcBaud.enSclkSel = LPUartMskPclk;        ///<?????
    stcCfg.stcBaud.u32Sclk = Sysctrl_GetPClkFreq();  ///<PCLK??
    stcCfg.stcBaud.enSclkDiv = LPUartMsk4Or8Div;     ///<????
    stcCfg.stcBaud.u32Baud = 115200;                   ///<???
    stcCfg.enRunMode = LPUartMskMode1;               ///<????
    LPUart_Init(M0P_LPUART1, &stcCfg);
     
    ///<LPUART ????
    LPUart_ClrStatus(M0P_LPUART1,LPUartRC);          ///<???????
    LPUart_ClrStatus(M0P_LPUART1,LPUartTC);          ///<???????
    LPUart_EnableIrq(M0P_LPUART1,LPUartRxIrq);      ///<??????
    LPUart_EnableIrq(M0P_LPUART1,LPUartTxIrq);       ///<??????
    EnableNvic(LPUART1_IRQn,IrqLevel1,TRUE);         ///<??????
}
//==============================================================================
//????:void App_LpUartPortCfg(void)
//????:LpUart1
//????:二维码
//????:
//????:
//????:
//????:
//=============================================================================

///< ????
void App_LpUartPortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    
    ///<TX
    stcGpioCfg.enDir =  GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin0,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin0,GpioAf2); //??PA00?LPUART1_TX
    
    //<RX
    stcGpioCfg.enDir =  GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin1,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin1,GpioAf2); //??PA01?LPUART1_RX
}
//==============================================================================
//????:void App_LpUart0Cfg(void)
//????:LpUart0
//????:串口屏
//????:
//????:
//????:
//????:
//=============================================================================

///< LPUART??
void App_LpUart0Cfg(void)
{
    stc_lpuart_cfg_t  stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///<????????
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart0,TRUE);    
    
    ///<LPUART ???
    stcCfg.enStopBit = LPUart1bit;                   ///<1???    
    stcCfg.enMmdorCk = LPUartDataOrAddr;                   ///<???
    stcCfg.stcBaud.enSclkSel = LPUartMskPclk;        ///<?????
    stcCfg.stcBaud.u32Sclk = Sysctrl_GetPClkFreq();  ///<PCLK??
    stcCfg.stcBaud.enSclkDiv = LPUartMsk4Or8Div;     ///<????
    stcCfg.stcBaud.u32Baud = 115200;                   ///<???
    stcCfg.enRunMode = LPUartMskMode1;               ///<????
    LPUart_Init(M0P_LPUART0, &stcCfg);
     
    ///<LPUART ????
    LPUart_ClrStatus(M0P_LPUART0,LPUartRC);          ///<???????
    LPUart_ClrStatus(M0P_LPUART0,LPUartTC);          ///<???????
    LPUart_EnableIrq(M0P_LPUART0,LPUartRxIrq);      ///<??????
    LPUart_EnableIrq(M0P_LPUART0,LPUartTxIrq);       ///<??????
    EnableNvic(LPUART0_IRQn,IrqLevel3,TRUE);         ///<??????
}
//==============================================================================
//????:void LpUart0_IRQHandler(void)
//????:LpUart0
//????:串口屏
//????:
//????:
//????:
//????:
//=============================================================================

///< ????
void App_LpUart0PortCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    
    ///<TX
    stcGpioCfg.enDir =  GpioDirOut;
    Gpio_Init(GpioPortB,GpioPin12,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortB,GpioPin12,GpioAf3); //??PA00?LPUART1_TX
    
    //<RX
    stcGpioCfg.enDir =  GpioDirIn;
    Gpio_Init(GpioPortB,GpioPin11,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortB,GpioPin11,GpioAf3); //??PA01?LPUART1_RX
}
//==============================================================================
//????:void App_PortInit(void)
//????:Uart1
//????:4G模块/485
//????:
//????:
//????:
//????:
//=============================================================================

//??????
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //??GPIO????
    
    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin2, GpioAf1);          //??PA02 ???URART1_TX
    
    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin3, GpioAf1);          //??PA03 ???URART1_RX
}
//==============================================================================
//????:void App_UartCfg(void)
//????:Uart1
//????:4G模块
//????:
//????:
//????:
//????:
//=============================================================================

//????
void App_UartCfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);
    
    ///< ??????
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);///<??uart1????
    
    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;          ///<??3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bit???
    stcCfg.enMmdorCk        = UartMskDataOrAddr;           ///<???
    stcCfg.stcBaud.u32Baud  = 9600;                  ///<???9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<????????
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<??????(PCLK)???
    Uart_Init(M0P_UART1, &stcCfg);                   ///<?????
    
    ///<UART????
    Uart_ClrStatus(M0P_UART1,UartRC);                ///<?????
    Uart_ClrStatus(M0P_UART1,UartTC);                ///<?????
    Uart_EnableIrq(M0P_UART1,UartRxIrq);             ///<????????
    Uart_EnableIrq(M0P_UART1,UartTxIrq);             ///<????????    
    EnableNvic(UART1_3_IRQn, IrqLevel2, TRUE);       ///<??????

}
void RS232_Tongxin(void)
{

}
uint8_t jianyan(void)
{
	uint8_t i = 0,j = 0,k = 0,b=0;
	if(RWM_SERVER_rec_buf[2] == 'o')
	{
		for(i = 0,j = 0,k = 0; i < 8; i++,j++)
		{	
			if(RWM_SERVER_rec_buf[2 + i] == opengate[j])
			{
				k++;
			}
		}
		if(k == 8)
		{
			if(usemsg1[0] == 0x00)
			{
				for(i = 0; i < 19; i++)
				{	
					usemsg1[i] = RWM_SERVER_rec_buf[27 + i];
				}	
				return 1;	
			}
			else
			{
				for(i = 0,j = 0,b = 0; i < 19; i++,j++)
				{	
					if(usemsg1[i] == RWM_SERVER_rec_buf[27 + i])
					{
						b++;
					}
				}		
				if(b == 19)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	else if(RWM_SERVER_rec_buf[2] == 'p')
	{
		for(i = 0,j = 0,k = 0; i < 9; i++,j++)
		{	
			if(RWM_SERVER_rec_buf[2 + i] == photogate[j])
			{
				k++;
			}
		}
		if(k == 9)
		{		
			for(i = 0,j = 0,b = 0; i < 19; i++,j++)
			{	
				if(usemsg1[i] == RWM_SERVER_rec_buf[28 + i])
				{
					b++;
				}
			}
			
			if(b == 19)
			{
				return 2;
			}
			else
			{
				return 0;
			}				
		}	
		else
		{
			return 0;
		}
	}	
	else
	{
		return 0;			
	}

}
uint8_t jianyan_jw(void)
{
	uint8_t i = 0,j = 0,b = 0;

	for(i = 0; i < 150; i++)
	{
		if(RWM_SERVER_rec_buf[i] == '}')
		{
			i = i-60;
			return i;
		}
		
	}
	return 0;
}

void CZSCQR(void)
{
 
}


uint8_t GSM_RXDATA_success(uint8_t *RecP)
{
	uint8_t i = 0;

	for(i = 0; i < 20; i++)
	{
//		Wdt_Feed();
		
		if((RecP[i] == 'T')&&(RecP[i+1] == 'H')&&(RecP[i+2] == 'R')&&(RecP[i+3] == 'O')&&(RecP[i+4] == 'W'))
		{
			if((RecP[i+5] == '\"')&&(RecP[i+6] == ',')&&(RecP[i+7] == '\"')&&(RecP[i+8] == 'm')&&(RecP[i+9] == 's')&&(RecP[i+10] == 'g')&&(RecP[i+11] == '\"')&&(RecP[i+12] == ':')&&(RecP[i+13] == '\"'))
			{	
				if((RecP[i+14] == 's')&&(RecP[i+15] == 'u'))  //判断是否为结果返回
				{
					if((RecP[i+16] == 'c')&&(RecP[i+17] == 'c'))
					{
						if((RecP[i+18] == 'e')&&(RecP[i+19] == 's'))//返回OK
						{
							if(RecP[i+20] == 's')
							return 1;//此处不返回，设置IP地址时先接收"OK",后接收"CONNECT"
						}
					}
				}
			}
		}
	}
	return 0;	
}
void Tongxin_485(void)
{
	uint8_t i = 0,j = 0,k = 0,t=0,u=0,a=0,x=0,z=0;
  if(GSM_receive_finish ==1)
	{
		
		for(z=0;z<GSM_SERVER_rec_buf_count;z++)	{
		if((GSM_SERVER_rec_buf[z] == 'A')&& (GSM_SERVER_rec_buf[z+1] == '5') && (GSM_SERVER_rec_buf[z+2] =='A') && (GSM_SERVER_rec_buf[z+3] =='5'))//收到A5A5指令代表成功连接HTTP服务器，写标志位再重启
		{
			u32FlashDataFlag = APP_UPG_FLAG;
							u8Ret= Flash_Init(12, TRUE);
							 u8Ret=  Flash_SectorErase(BOOT_PARA_ADDRESS);
							  if(u8Ret == Ok)
								{
						       u8Ret = Flash_WriteWord(BOOT_PARA_ADDRESS, u32FlashDataFlag);
									read_temp = *((volatile uint32_t*)BOOT_PARA_ADDRESS);
	              }	
		}						NVIC_SystemReset();
			
					if((GSM_SERVER_rec_buf[7] == '2')&& (GSM_SERVER_rec_buf[8] == '0') && (GSM_SERVER_rec_buf[9] =='1') && (GSM_SERVER_rec_buf[10] =='5'))
	      {

					for(t=0;t<GSM_SERVER_rec_buf_count;t++){
						
							for(t=0;t<GSM_SERVER_rec_buf_count;t++){
						
						
						
						
						if(GSM_SERVER_rec_buf[t]=='h' && GSM_SERVER_rec_buf[t+1]=='t'  && GSM_SERVER_rec_buf[t+2]=='t' && GSM_SERVER_rec_buf[t+3]=='p' && GSM_SERVER_rec_buf[t+4]==':'
						&& GSM_SERVER_rec_buf[t+5]=='/' && GSM_SERVER_rec_buf[t+6]=='/'){//2015 升级指令 
							a=t+7;
							k=0;
							while(GSM_SERVER_rec_buf[a]!='/'){	//提取域名
							server_adress[k]=GSM_SERVER_rec_buf[a];
								k++;
								a++;
							}
							u=a;
							k=0;
							while(GSM_SERVER_rec_buf[u]!='"'){//提取URL
								if(GSM_SERVER_rec_buf[u]=='}')break;
							server_URL[k]=GSM_SERVER_rec_buf[u];
								k++;
								u++;
							}
						
						}
					
					}
					
			}
						memset(GSM_Upload_send_buff1,0x00,150);
				GSM_Upload_send_buff_len1 = MIEWD_upgrade(GSM_Upload_send_buff1);//发送开始升级指令
				Uart_SendDataIt(M0P_UART0,GSM_Upload_send_buff1[GSM_Upload_send_buff_count]);
											http_set_F=1;
											LED_T = 100;
											FLASH_F = 0;
		}

		}
		
	memset(GSM_SERVER_rec_buf,0x00,150);
	GSM_SERVER_rec_buf_count = 0;
	GSM_receive_finish = 0;			
	}

}

//static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length)
//{
//	unsigned char i=0;
//	uint16_t k=0,crc=0xFFFF;
//	
//	while(length--)
//	{
//		crc ^= pBuffer[k];
//		k++;
//		for(i=0;i<8;i++)
//		{
//			if(crc&0x01)
//			crc=(crc>>1)^0xA001;
//			else
//			crc=crc>>1;
//		}
//	}
//	
//	return(crc);
//}
/****************************** END OF C FILE ********************************************/	

