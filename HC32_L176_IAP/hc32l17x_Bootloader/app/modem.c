/******************************************************************************/
/** \file modem.c
 **
 **   - 2024-01-21  1.0  chengjianLIU  First version for modem function.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "modem.h"
#include "iap.h"
#include "uart.h"
#include "flash.h"

extern uint32_t g_u32UpgradePacketsSize;//需要升级完成的数据包大小
static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length);
extern uint8_t  bootstr[13];
extern uint8_t  TX_count;
extern uint8_t TX_max;
extern uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len);
extern uint8_t ctrl_data(uint8_t *buf,uint8_t *str,uint8_t len,uint32_t data);
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8FrameData[FRAME_MAX_SIZE]; 
//uint8_t Data[530];//帧存储缓存
uint8_t recvData = 0;
uint8_t recvData1 = 0;
uint8_t recvData_F = 0;
uint32_t u32FrameDataIndex;                //帧存储缓存索引
uint32_t u32FrameSize;
uint16_t u16Crc16 = 0;                     //crc16计算值，检验数据包值
uint8_t* ok_str  =(uint8_t*)"ready to update";//15
uint32_t u32FrameRecvOverTime;             //帧接收超时计数器，在(1ms)定时器中断中计数，在串口中断中清零
extern uint8_t systime5s_flag;
en_frame_recv_status_t enFrameRecvStatus;

uint8_t u8TxBuffer[32];
uint8_t u8AckUpgPackets[4] = {0x00,0x00,0x4F,0x4B};//升级响应数据包
uint16_t ii=0;

static uint8_t j=0;
uint8_t arr[3]={0x00};
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
uint8_t size_Data[20] = {0x00};
uint8_t pack_num_Data[20] = {0x00};
uint8_t data[530]={0x00};
uint8_t size_F=0;
uint8_t pack_F=0;
uint8_t head_f=0;
uint32_t size=0;
uint32_t packnum=0;
uint8_t cover(uint8_t  num);
extern uint8_t send_num_F;
extern uint8_t receive_F;
extern uint8_t before_F;
extern uint8_t post_num;
/**
 *******************************************************************************
 ** \brief crc16 校验算法
 **
 ** \param [out] u8TxBuff               待校验缓存
 ** \param [in] u16TxLength             待校验字节数
 **
 ** \retval None
 **
 ******************************************************************************/
static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length)
{
	unsigned char i=0;
	uint16_t k=0,crc=0xFFFF;
	
	while(length--)
	{
		crc ^= pBuffer[k];
		k++;
		for(i=0;i<8;i++)
		{
			if(crc&0x01)
			crc=(crc>>1)^0xA001;
			else
			crc=crc>>1;
		}
	}
	
	return(crc);
}


/**
 *******************************************************************************
 ** \brief 应答帧处理,通知上位机已接收到的总字节数
 **
 ** \param 
 ** \param [in]  u32ReceiveBytes   已接收数据包字节数
 **
 ** \retval None
 **
 ******************************************************************************/



/**
 *******************************************************************************
 ** \brief 应答,表示已准备好升级
 **
 ** \param 
 ** \param 
 **
 ** \retval None
 **
 ******************************************************************************/





/**
 *******************************************************************************
 ** \brief 上位机数据帧解析及处理
 **
 ** \param [in] None             
 **
 ** \retval Ok                          APP程序升级完成，并接受到跳转至APP命令
 ** \retval OperationInProgress         数据处理中
 ** \retval Error                       通讯错误
 **
 ******************************************************************************/
uint32_t u32ReceiveTotalDataBytes = 0;
uint32_t u32FlashAddress = APP_ADDRESS;//从其实地址开始


uint8_t Modem_Process(void)
{
    uint8_t  u8Cmd, u8Ret;
	  uint32_t u32PacketsLenth = 0;
    _Frame_Comm_TypeDef *Pvar = (_Frame_Comm_TypeDef *)&u8FrameData[0];
	
    if (enFrameRecvStatus == FRAME_RECV_PROC_STATUS)                //有数据帧待处理, enFrameRecvStatus值在串口中断中调整
    {
        u8Cmd = u8FrameData[PACKET_CMD_INDEX];                      //获取帧指令码
			  u32PacketsLenth = Pvar->u32NumofPackets;//接收到的数据包大小
        if ( (u8Cmd == FRAME_WRITE_CMD)&&(Pvar->u8Regaddr == FRAME_REG_TYPE_DATA) )                           //如果是数据指令
        {
					  if( (u32FlashAddress & 0x1FF) == 0)
						{
	           
						 u8Ret = FLASH_SectorLockSet(u32FlashAddress);
						  u8Ret = Flash_OpModeConfig(FlashSectorEraseMode);
							u8Ret = Flash_SectorErase(u32FlashAddress);
						}
						u8Ret = Flash_OpModeConfig(FlashWriteMode);
						u8Ret = Flash_Write8(u32FlashAddress, &u8FrameData[PACKET_DATA_INDEX], u32PacketsLenth);
						if(u8Ret != Ok)
						{
						  enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;//命令出错，重新接收
							return Error;
						}
						u32ReceiveTotalDataBytes += u32PacketsLenth;
						u32FlashAddress += u32PacketsLenth;
						enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;//准备接收新的一帧数据
						
						receive_F=1;
						before_F=	post_num;
						send_num_F=0;
						if(u32ReceiveTotalDataBytes >= g_u32UpgradePacketsSize)//接收数据已经完成
						{
							  Flash_Init(6, TRUE);
                Flash_OpModeConfig(FlashReadMode);
	              Flash_LockAll();
							  enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;//更新完成,重新接收
						    return Ok;
						}
        }
				if ( (u8Cmd == FRAME_WRITE_CMD)&&(Pvar->u8Regaddr == FRAME_REG_TYPE_UPG) )//升级包
				{
					  _Packet_UPG_TypeDef *Pdata = (_Packet_UPG_TypeDef *)&u8FrameData[0];
					 // g_u32UpgradePacketsSize = Pdata->Data;
					  enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;//命令出错，重新接收
					 // Modem_SendReadytoUpgrade(g_u32UpgradePacketsSize);//通知上位机准备好了
				}
				else
				{
				    enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;//命令出错，重新接收A5 A5 0C 00 00 00 68 DB A0 20 04 00 00 00 48 3D 0000
				}
    }
    
    return OperationInProgress;  //返回，APP更新中。。。
}


/**
 *******************************************************************************
 ** \brief 计算有多少sector页待擦除
 **
 ** \param [out] None
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/



/**
 *******************************************************************************
 ** \brief 设置FLASH解锁配置
 **
 ** \param [out] None
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
en_result_t FLASH_SectorLockSet(uint32_t uFlashadd)
{
    uint16_t u32PageNum = uFlashadd / FLASH_SECTOR_SIZE;
	  uint8_t  u8ShellNum = 0;//移位位数
	  uint32_t u32LockReg = 0;
	  en_result_t ret = 0;
	  if ((uFlashadd % FLASH_SECTOR_SIZE) != 0)
    {
        u32PageNum += 1u;
    }

    if(u32PageNum < 128)
		{
		   u8ShellNum = u32PageNum/4;
			 u32LockReg = (uint32_t)(1 << u8ShellNum);
			 ret = Flash_LockSet(FlashLock0, u32LockReg);
		}else {
			 u8ShellNum = (u32PageNum - 128)/4;
			 u32LockReg = (uint32_t)(1 << u8ShellNum);
			 ret = Flash_LockSet(FlashLock1, u32LockReg);
		}

    return ret;
}



/**
 *******************************************************************************
 ** \brief 串口中断，接收上位机发送帧
 **
 ** \param [out] None
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/

///<LPUART1 中断服务函数
void Uart0_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART0, UartTC))
    {
       Uart_ClrStatus(M0P_UART0, UartTC); 
			TX_count++;
			if(TX_count >= TX_max)
			{
				TX_count=0;
			}
			else
			{
				M0P_UART0->SBUF_f.DATA = bootstr[TX_count];						
			}		        //清中断状态位
    }
    
    if(Uart_GetStatus(M0P_UART0, UartRC))    ///接收数据
    {
        Uart_ClrStatus(M0P_UART0, UartRC);   ///<清接收中断请求
        recvData1 = Uart_ReceiveData(M0P_UART0);///读取数据
			if(recvData1=='A'){
				
				recvData_F=1;}
			
			if(ii<=526){data[ii++]= recvData1;}
			if(j==0 &&  recvData_F==1){
			arr[j]=recvData1;
				j++;
				return ;
			}
			if(j==1){
				arr[j]=recvData1;
				recvData=(cover(arr[0])<<4)|cover(arr[1]);
			j=0;	
			}
			
			
			  u32FrameRecvOverTime = 0;
			  switch(enFrameRecvStatus)
        {
            case FRAME_RECV_IDLE_STATUS:                           //当前处于空闲状态
                if (recvData == FRAME_HEAD_L)                           //收到帧头第一个字节
                {
                    u8FrameData[FRAME_HEAD_H_INDEX] = recvData;         //保存数据
                    enFrameRecvStatus = FRAME_RECV_HEADER_STATUS;       //帧接收进入下一状态: 接收帧头状态
                }
                break;
            case FRAME_RECV_HEADER_STATUS:                              //当前处于接收帧头状态
                if (recvData == FRAME_HEAD_H)                           //收到帧头第二个字节
                {
                    u8FrameData[FRAME_HEAD_L_INDEX] = recvData;         //保存数据
                    u32FrameDataIndex = FRAME_NUM_INDEX;                //数组下标从帧头的下一位置开始计数
                    enFrameRecvStatus = FRAME_RECV_DATA_STATUS;         //帧接收进入下一状态:  接收帧数据状态
                }
								else if (recvData == FRAME_HEAD_L)                     //收到帧头第一个字节
                {
                    u8FrameData[FRAME_HEAD_H_INDEX] = recvData;        //保存数据
                    enFrameRecvStatus = FRAME_RECV_HEADER_STATUS;      //帧接收进入下一状态
                }
								else                                                   //数据错误
                {
                    enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;        //帧接收恢复到初始状态:  空闲状态
									
								//////////////////////////A5 A5 0C 00 00 00 D7 1B A0 20 04 00 00 00 28 3F 00 00
                }
                break;
            case FRAME_RECV_DATA_STATUS:                               //当前处于接收帧数据状态
                u8FrameData[u32FrameDataIndex++] = recvData;
					      if(u32FrameDataIndex==17 && post_num==0){
								g_u32UpgradePacketsSize =(uint32_t)( (u8FrameData[14]) + (u8FrameData[15]<<8) + 
										(u8FrameData[16]<<16) + (u8FrameData[17]<<24) );
									receive_F=1;
									before_F=	post_num;
									enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;  
									return ;
									
								}	
					
                if (u32FrameDataIndex == FRAME_LENGTH_INDEX)       //已经收到包长度数据
                {
                    u32FrameSize = (uint32_t)( (u8FrameData[FRAME_LENGTH_INDEX-1]<<24) + (u8FrameData[FRAME_LENGTH_INDEX-2]<<16) + 
									(u8FrameData[FRAME_LENGTH_INDEX-3]<<8) 
									+ u8FrameData[FRAME_LENGTH_INDEX-4] );  //帧长度字节4B
                    if ((u32FrameSize < FRAME_MIN_SIZE) || (u32FrameSize > FRAME_MAX_SIZE))  //帧长度不在有效范围内
                    {
                        enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;     //帧接收恢复到初始状态
                        return;                                         //错误返回
                    }
                }
								else if ((u32FrameDataIndex > FRAME_PACKET_INDEX) && (u32FrameDataIndex == u32FrameSize + FRAME_LENGTH_INDEX))                       //帧数据接收完毕
                {
                    u16Crc16 = (uint16_t)(u8FrameData[FRAME_PACKET_INDEX-2] + (u8FrameData[FRAME_PACKET_INDEX-1]<<8) );
                    if (crc16Algorithm(&u8FrameData[FRAME_PACKET_INDEX], u32FrameSize - 2) == u16Crc16)               //如果CRC校验通过
                    {
                        enFrameRecvStatus = FRAME_RECV_PROC_STATUS;     //帧接收进入下一状态:  帧处理状态                    
                    }
										else                                               //校验失败
                    {
                        enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;     //帧接收恢复到初始状态
                        return;                                         //错误返回
                    }
                }
                break;
            case FRAME_RECV_PROC_STATUS:                           //当前处于帧处理状态
                break;
        }
    }
}


/**
 *******************************************************************************
 ** \brief 定时器中断函数，1ms中断一次，用于监测串口数据接收超时，及帧数据处理超时
 **
 ** \param [out] None
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Tim_IRQHandler(void)
{
#if 0
    if (HC32_GetTimUIFStatus())                                         //获取定时器溢出中断标志位
    {
        HC32_ClrTimUIFStatus();                                         //清除定时器溢出中断标志位
        
        u32FrameRecvOverTime++;        
        if ((enFrameRecvStatus == FRAME_RECV_HEADER_STATUS) || (enFrameRecvStatus == FRAME_RECV_DATA_STATUS))   //处于帧接收过程中
        {
            if (u32FrameRecvOverTime++ > 10)                            //超过10ms没有收到到数据，异常
            {
                enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;             //帧接收恢复到初始状态
            }
        }else if (enFrameRecvStatus == FRAME_RECV_PROC_STATUS)          //处于帧处理状态
        {
            if (u32FrameRecvOverTime++ > 4500)                          //超过4.5s没有收到到数据，异常 (上位机超过5s没收到应答帧，会重发数据帧)
            {
                enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;             //帧接收恢复到初始状态
            }
        }
    }
#endif
}

/**
 *******************************************************************************
 ** \brief modem文件中相关变量参数初始化
 **
 ** \param [out] None
 ** \param [in]  None
 **
 ** \retval None
 **
 ******************************************************************************/
void Modem_RamInit(void)
{    
    uint32_t i;
    
    enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;  //帧状态初始化为空闲状态
    
    for (i=0; i<FRAME_MAX_SIZE; i++)
    {
        u8FrameData[i] = 0;                      //帧数据缓存初始化为零
    }
		
		for (i=0; i<32; i++)
		{
		    u8TxBuffer[i] = 0;
		}
    
    u32FrameDataIndex = 0;                      //帧缓存数组索引值初始化为零
		enFrameRecvStatus = FRAME_RECV_IDLE_STATUS; //数据帧处于空闲状态    
}

void writeLInt(unsigned char** pptr, uint32_t anInt)
{
    if(anInt<10)
    {

		//	**pptr = 0x30;
		//	(*pptr)++;
		//	**pptr = '.';
		//	(*pptr)++;    
			//**pptr = 0x30;
			//(*pptr)++; 
		//	**pptr = 0x30;
		//	(*pptr)++; 			
      **pptr = anInt+0x30;
      (*pptr)++;
    }
    else if((anInt>=10)&&(anInt<100))
    {
			//**pptr = 0x30;
		//	(*pptr)++;
		//	**pptr = '.';
			//(*pptr)++;    
		//	**pptr = 0x30;
			//(*pptr)++; 			
			
      **pptr = anInt/10+0x30;
      (*pptr)++;
            
      **pptr = anInt%10+0x30;
      (*pptr)++; 
    }
    else if((anInt>=100)&&(anInt<1000))
    {
			//**pptr = 0x30;
			//(*pptr)++;
		//	**pptr = '.';
			//(*pptr)++; 
			
      **pptr = anInt/100+0x30;
      (*pptr)++;
      **pptr = anInt%100/10+0x30;
      (*pptr)++;  
              
      **pptr = anInt%10+0x30;
      (*pptr)++;        
    }
    else if((anInt>=1000)&&(anInt<10000))
    {
      **pptr = anInt/1000+0x30;
   //   (*pptr)++;
		//	**pptr = '.';
			(*pptr)++;			
      **pptr = anInt%1000/100+0x30;
      (*pptr)++;
      **pptr = anInt%100/10+0x30;
      (*pptr)++; 
             
      **pptr = anInt%10+0x30;
      (*pptr)++;        
    }
  
   
}
void UART_SendData(uint8_t *pu8TxBuff, uint16_t u16Length)
{
    while (u16Length--)
    {
			  Uart_SendDataIt(M0P_UART0, *pu8TxBuff);
        pu8TxBuff++;
    }
}

uint8_t number_sucess(uint8_t num){
		
	if(num=='0' || num=='1' || num=='2' || num=='3' || num=='4' || num=='5' || num=='6' || num=='7' || num=='8' || num=='9' )return 1;
		else {return 0;}

}

uint8_t cover(uint8_t  num){
	if( num>=0x30 && num<=0x39)
		return num-0x30;
	if( num>=0x41 && num<=0x5A)
		return num-0x37;
	return 0;
}

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
