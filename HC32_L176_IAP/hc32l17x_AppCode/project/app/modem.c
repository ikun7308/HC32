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
#include "lpuart.h"
#include "flash.h"
#include "main.h"
#include "uart_init.h"
extern uint32_t g_u32UpgradePacketsSize;//需要升级完成的数据包大小
static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length);



/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8FrameData[FRAME_MAX_SIZE];       //帧存储缓存
uint8_t recvData = 0;
uint32_t u32FrameDataIndex;                //帧存储缓存索引
//uint32_t u32FrameSize;
//uint16_t u16Crc16 = 0;                     //crc16计算值，检验数据包值

uint32_t u32FrameRecvOverTime;             //帧接收超时计数器，在(1ms)定时器中断中计数，在串口中断中清零
en_frame_recv_status_t enFrameRecvStatus;


uint8_t u8TxBuffer[32];
uint8_t u8AckUpgPackets[4] = {0x00,0x00,0x4F,0x4B};//升级响应数据包

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint16_t FLASH_PageNumber(uint32_t u32Size);


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
void Modem_SendTotalNumofBytesReceived(uint32_t u32ReceiveBytes)
{
	_Packet_UPG_TypeDef *_DataPacket = NULL;	
  
  _DataPacket = (_Packet_UPG_TypeDef *)&u8TxBuffer[0];
	_DataPacket->pComm.u8Head0 = FRAME_HEAD_L;
	_DataPacket->pComm.u8Head1 = FRAME_HEAD_H;
	_DataPacket->pComm.u8Command = FRAME_READ_CMD;
	_DataPacket->pComm.u8Regaddr = FRAME_REG_TYPE_DATA;
	_DataPacket->pComm.u32Lenth = 0x0C;
	_DataPacket->pComm.u32NumofPackets = 0x04;
	_DataPacket->Data = u32ReceiveBytes;
	_DataPacket->pComm.u16crc16 = crc16Algorithm((uint8_t *)&_DataPacket->pComm.u8Command,_DataPacket->pComm.u32Lenth-2);
	
	UART_SendData((uint8_t *)_DataPacket, _DataPacket->pComm.u32Lenth + 6);
} 



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
void Modem_SendReadytoUpgrade(uint32_t u32PacketSize)
{
	_Packet_UPG_TypeDef *_DataPacket = NULL;	
  
  _DataPacket = (_Packet_UPG_TypeDef *)&u8TxBuffer[0];
	_DataPacket->pComm.u8Head0 = FRAME_HEAD_L;
	_DataPacket->pComm.u8Head1 = FRAME_HEAD_H;
	_DataPacket->pComm.u8Command = FRAME_READ_CMD;
	_DataPacket->pComm.u8Regaddr = FRAME_REG_TYPE_UPG;
	_DataPacket->pComm.u32Lenth = 0x0C;
	_DataPacket->pComm.u32NumofPackets = 0x04;
	_DataPacket->Data = u32PacketSize;
	_DataPacket->pComm.u16crc16 = crc16Algorithm((uint8_t *)&_DataPacket->pComm.u8Command,_DataPacket->pComm.u32Lenth-2);
	
	UART_SendData((uint8_t *)_DataPacket, _DataPacket->pComm.u32Lenth + 6);
} 


void Modem_MasterSendUpgradeCommand(void)
{
	_Packet_UPG_TypeDef *_DataPacket = NULL;	
  
  _DataPacket = (_Packet_UPG_TypeDef *)&u8TxBuffer[0];
	_DataPacket->pComm.u8Head0 = FRAME_HEAD_L;
	_DataPacket->pComm.u8Head1 = FRAME_HEAD_H;
	_DataPacket->pComm.u8Command = FRAME_READ_CMD;
	_DataPacket->pComm.u8Regaddr = FRAME_REG_TYPE_UPG;
	_DataPacket->pComm.u32Lenth = 0x0C;
	_DataPacket->pComm.u32NumofPackets = 0x04;
	_DataPacket->Data = 0x00004125;
	_DataPacket->pComm.u16crc16 = crc16Algorithm((uint8_t *)&_DataPacket->pComm.u8Command,_DataPacket->pComm.u32Lenth-2);
	
	UART_SendData((uint8_t *)_DataPacket, _DataPacket->pComm.u32Lenth + 6);
} 

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
uint32_t u32FlashAddress = BOOT_PARA_ADDRESS;//从其实地址开始
//uint32_t u32FlashDataFlag = 0;


uint8_t Modem_Process(void)
{

//    
    return 1;  //无升级指令
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
static uint16_t FLASH_PageNumber(uint32_t u32Size)
{
    uint16_t u32PageNum = u32Size / FLASH_SECTOR_SIZE;

    if ((u32Size % FLASH_SECTOR_SIZE) != 0)
    {
        u32PageNum += 1u;
    }

    return u32PageNum;
}


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
	  en_result_t ret = Error;
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
//void LpUart1_IRQHandler(void)
//{
//    if(LPUart_GetStatus(M0P_LPUART1, LPUartTC))
//    {
//        LPUart_ClrStatus(M0P_LPUART1, LPUartTC);   ///<清发送中断请求
//        LPUart_DisableIrq(M0P_LPUART1,LPUartTxIrq);///<禁止发送中断
//    }
//    
//    if(LPUart_GetStatus(M0P_LPUART1, LPUartRC))    ///接收数据
//    {
//        LPUart_ClrStatus(M0P_LPUART1, LPUartRC);   ///<清接收中断请求
//        recvData = LPUart_ReceiveData(M0P_LPUART1);///读取数据
//			  u32FrameRecvOverTime = 0;
//			 switch(enFrameRecvStatus)
//        {
//            case FRAME_RECV_IDLE_STATUS:                           //????????
//                if (recvData == FRAME_HEAD_L)                           //?????????
//                {
//                    u8FrameData[FRAME_HEAD_H_INDEX] = recvData;         //????
//                    enFrameRecvStatus = FRAME_RECV_HEADER_STATUS;       //?????????:  ????
//                }
//                break;
//            case FRAME_RECV_HEADER_STATUS:                              //??????????
//                if (recvData == FRAME_HEAD_H)                           //?????????
//                {
//                    u8FrameData[FRAME_HEAD_L_INDEX] = recvData;         //????
//                    u32FrameDataIndex = FRAME_NUM_INDEX;                //????????????????
//                    enFrameRecvStatus = FRAME_RECV_DATA_STATUS;         //?????????:  ???????
//                }
//								else if (recvData == FRAME_HEAD_L)                     //?????????
//                {
//                    u8FrameData[FRAME_HEAD_H_INDEX] = recvData;        //????
//                    enFrameRecvStatus = FRAME_RECV_HEADER_STATUS;      //?????????
//                }
//								else                                                   //????
//                {
//                    enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;        //??????????:  ????
//                }
//                break;
//            case FRAME_RECV_DATA_STATUS:                               //???????????
//                u8FrameData[u32FrameDataIndex++] = recvData;
//                if (u32FrameDataIndex == FRAME_LENGTH_INDEX)       //?????????
//                {
//                    u32FrameSize = (uint32_t)( (u8FrameData[FRAME_LENGTH_INDEX-1]<<24) + (u8FrameData[FRAME_LENGTH_INDEX-2]<<16) + (u8FrameData[FRAME_LENGTH_INDEX-3]<<8) + u8FrameData[FRAME_LENGTH_INDEX-4] );  //?????4B
//                    if ((u32FrameSize < FRAME_MIN_SIZE) || (u32FrameSize > FRAME_MAX_SIZE))  //??????????
//                    {
//                        enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;     //??????????
//                        return;                                         //????
//                    }
//                }
//								else if ((u32FrameDataIndex > FRAME_PACKET_INDEX) && (u32FrameDataIndex == u32FrameSize + FRAME_LENGTH_INDEX))                       //???????
//                {
//                    u16Crc16 = (uint16_t)(u8FrameData[FRAME_PACKET_INDEX-2] + (u8FrameData[FRAME_PACKET_INDEX-1]<<8) );
//                    if (crc16Algorithm(&u8FrameData[FRAME_PACKET_INDEX], u32FrameSize - 2) == u16Crc16)               //??CRC????
//                    {
//                        enFrameRecvStatus = FRAME_RECV_PROC_STATUS;     //?????????:  ?????                    
//                    }
//										else                                               //????
//                    {
//                        enFrameRecvStatus = FRAME_RECV_IDLE_STATUS;     //??????????
//                        return;                                         //????
//                    }
//                }
//                break;
//            case FRAME_RECV_PROC_STATUS:                           //?????????
//                break;
//        }
//    }
//}


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


void UART_SendData(uint8_t *pu8TxBuff, uint16_t u16Length)
{
    while (u16Length--)
    {
			  LPUart_SendData(M0P_LPUART1, *pu8TxBuff);
        pu8TxBuff++;
    }
}

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
