/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file modem.h
 **
 **   - 2019-05-29  1.0  yangjp  First version for modem function.
 **
 ******************************************************************************/
#ifndef __MODEM_H__
#define __MODEM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
//#include "hc32_common.h"
#include "ddl.h"
#include "main.h"
//#include "cmsis_armcc.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
#define __packed __attribute__((packed))
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
 typedef struct {
	 uint8_t u8Head0;
	 uint8_t u8Head1;
	 uint32_t u32Lenth;
	 uint16_t u16crc16;
	 uint8_t u8Command;
	 uint8_t u8Regaddr;
	 uint32_t u32NumofPackets;
}__packed _Frame_Comm_TypeDef;
 

 typedef struct {
	 _Frame_Comm_TypeDef pComm;//协议头文件
	 uint32_t Data;//字节数据
}__packed _Packet_UPG_TypeDef;
 
/**
 *******************************************************************************
 ** \brief Packet status enumeration
 ******************************************************************************/



/**
 *******************************************************************************
 ** \brief Packet command type enumeration
 ******************************************************************************/
typedef enum
{
    FRAME_REG_TYPE_UPG   = 0x20,
    FRAME_REG_TYPE_DATA  = 0x21,
} en_frame_cmd_type_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define FRAME_HEAD_L                        0xA5u
#define FRAME_HEAD_H                        0xA5u
#define FRAME_WRITE_CMD                     0xA0u
#define FRAME_READ_CMD                      0xA1u

/* Frame and packet size */
#define PACKET_INSTRUCT_SEGMENT_SIZE        12u
#define PACKET_CRC16_SHELL_SIZE             8u
#define PACKET_DATA_SEGMENT_SIZE            512
#define FRAME_MIN_SIZE                      PACKET_INSTRUCT_SEGMENT_SIZE
#define FRAME_MAX_SIZE                      (PACKET_DATA_SEGMENT_SIZE + PACKET_INSTRUCT_SEGMENT_SIZE)

/* Frame structure defines */
#define FRAME_HEAD_H_INDEX                  0x00
#define FRAME_HEAD_L_INDEX                  0x01
#define FRAME_NUM_INDEX                     0x02
#define FRAME_XORNUM_INDEX                  0x03
#define FRAME_LENGTH_INDEX                  0x06
#define FRAME_PACKET_INDEX                  0x08

#define FRAME_RECV_TIMEOUT                  5               // ms
#define FRAME_NUM_XOR_BYTE                  0xFF

/* Packet structure defines */
#define PACKET_CMD_INDEX                   (FRAME_PACKET_INDEX + 0x00)

#define PACKET_TYPE_INDEX                  (FRAME_PACKET_INDEX + 0x01)
#define PACKET_DATA_INDEX                  (FRAME_PACKET_INDEX + FRAME_LENGTH_INDEX)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
uint8_t Modem_Process(void);
void Modem_SendTotalNumofBytesReceived(uint32_t u32ReceiveBytes);
void Modem_SendReadytoUpgrade(uint32_t u32PacketSize);
void Modem_MasterSendUpgradeCommand(void);
void Modem_RamInit(void);
void UART_SendData(uint8_t *pu8TxBuff, uint16_t u16Length);
 static uint16_t crc16Algorithm(uint8_t *pBuffer, uint32_t length);
en_result_t FLASH_SectorLockSet(uint32_t uFlashadd);

#ifdef __cplusplus
}
#endif

#endif /* __MODEM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
