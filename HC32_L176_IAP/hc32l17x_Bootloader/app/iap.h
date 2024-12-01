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
/** \file iap.h
 **
 **   - 2019-08-21  1.0  Chenw  First version for IAP function.
 **
 ******************************************************************************/
#ifndef __IAP_H__
#define __IAP_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
//#include "hc32_common.h"

/********************************Flash��غ궨��***************************************/
#define FLASH_SECTOR_SIZE           0x200ul                           //һ��sector�ĳߴ�512B
#define FLASH_BASE                  ((uint32_t)0x00000000)            //flash����ַ
#define FLASH_SIZE                  (256u * FLASH_SECTOR_SIZE)        //flash�ߴ�128KB
/*************************RAM��غ궨��*************************************************/
#define SRAM_BASE                   ((uint32_t)0x20000000)            //RAM����ַ
#define RAM_SIZE                    0x4000ul                          //RAM�ߴ�16KB
/*************************BootLoader flash��غ궨��*********************************/
#define BOOT_SIZE                   (16*FLASH_SECTOR_SIZE)            //BootLoader flash�ߴ�8KB
#define BOOT_PARA_ADDRESS           (FLASH_BASE + BOOT_SIZE - 0x200u) //BootLoader para�洢��ַ
#define PACKCT_SIZE_ADDRESS         (FLASH_BASE + BOOT_SIZE - 0x200u + 4U) //��Ҫ������ɵ����ݰ���С
/*************************APP flash��غ궨��*****************************************/
#define APP_UPG_FLAG                ((uint32_t)0xAAAA5555)            //��BootLoader para��������ֵ����ʾAPP��Ҫ����
#define APP_ADDRESS                 (uint32_t)(FLASH_BASE + BOOT_SIZE)          //APP����洢����ַ
#define APP_PACKETS_SIZE            (uint32_t)(FLASH_SIZE - APP_ADDRESS)//app�������С

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void IAP_Init(void);
void IAP_Main(void);
void IAP_UpdateCheck(void);
void IAP_JumpToApp_cmd( void);
#ifdef __cplusplus
}
#endif

#endif /* __IAP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
