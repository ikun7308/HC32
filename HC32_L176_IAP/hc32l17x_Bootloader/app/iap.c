/******************************************************************************/
/** \file iap.c
 **
 **   - 2024-01-21  1.0  chengjianLIU  First version for IAP function.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "iap.h"
#include "modem.h"
#include "flash.h"
#include "lpuart.h"
#include "uart.h"
#include "ddl.h"
#include "gpio.h"
extern void App_ClkCfg(void);
extern uint8_t TX_max;
uint8_t* uping  =(uint8_t*)"wait upgrad!";//12
uint8_t* Infrared11  =(uint8_t*)"boot start!";
//uint8_t* ZT  =(uint8_t*)"wait upgrad!";//12
uint8_t  ZTstr[15] = {0x00};
uint32_t g_u32UpgradePacketsSize = 0 ;//需要升级完成的数据包大小
uint32_t JumpAddress;
func_ptr_t JumpToApplication;
extern uint8_t ctrl(uint8_t *buf,uint8_t *str,uint8_t len);
extern uint8_t  bootstr[15];
extern uint8_t TX_count;

extern uint8_t systime1s_flag;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t IAP_JumpToApp(uint32_t u32Addr);
static void IAP_ResetConfig(void);
static void DeinitlpUart(void);
extern en_frame_recv_status_t enFrameRecvStatus;
extern uint8_t led_trg;

/**
 *******************************************************************************
 ** \brief  IAP 初始化
 **
 ** \param  [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
void IAP_Init(void)
{
    Modem_RamInit();
}

/**
 *******************************************************************************
 ** \brief  IAP APP程序升级主函数.
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void IAP_Main(void)
{
    en_result_t enRet;

    while (1)
    {
        enRet = Modem_Process();                       //APP程序更新处理

			
        if (Ok == enRet)
        {
            if (Error == IAP_JumpToApp(APP_ADDRESS))   //如果跳转失败
            {
								
                while(1);
            }
        }
				if(led_trg==1)
					Gpio_WriteOutputIO(STK_LED_PORT,STK_LED_PIN,1-Gpio_ReadOutputIO(STK_LED_PORT,STK_LED_PIN));
				led_trg=0;
			
    }
}
void IAP_JumpToApp_cmd( void){

IAP_JumpToApp(APP_ADDRESS);
}
/**
 *******************************************************************************
 ** \brief  检查BootPara标记区数据值，判断是否需要升级APP程序.
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void IAP_UpdateCheck(void)
{
    uint32_t u32AppFlag;
    
    u32AppFlag = *((volatile uint32_t*)BOOT_PARA_ADDRESS); //读出BootLoader para区标记值
    if (APP_UPG_FLAG != u32AppFlag)                       //如果标记值不等于APP_FLAG,表示不需要升级APP程序
    {
        IAP_JumpToApp(APP_ADDRESS);                   //则直接跳转至APP
    }
    else                                              //读取需要完成升级的数据包大小
		{
//g_u32UpgradePacketsSize = *(__IO uint32_t *)PACKCT_SIZE_ADDRESS;//需要下载的数据包大小
//				FLASH_SectorLockSet(BOOT_PARA_ADDRESS);
//				Flash_OpModeConfig(FlashSectorEraseMode);
//				Flash_SectorErase(BOOT_PARA_ADDRESS);//将需要升级的标志信息清除


		}			
}

/**
 *******************************************************************************
 ** \brief  IAP跳转函数
 **
 ** \param  [in] u32Addr                    APP 首地址
 **
 ** \retval Error                           APP 地址错误
 **
 ******************************************************************************/
static en_result_t IAP_JumpToApp(uint32_t u32Addr)
{
    uint32_t u32StackTop = *((__IO uint32_t *)u32Addr);  //读取APP程序栈顶地址

    /* 判断栈顶地址有效性 */
    if ((u32StackTop > SRAM_BASE) && (u32StackTop <= (SRAM_BASE + RAM_SIZE)))
    {
			  IAP_ResetConfig(); 
        /* 配置跳转到用户程序复位中断入口 */
        JumpAddress = *(__IO uint32_t *)(u32Addr + 4);
        JumpToApplication = (func_ptr_t)JumpAddress;
        /* 初始化用户程序的栈顶指针 */
        __set_MSP(*(__IO uint32_t *)u32Addr);
        JumpToApplication();
    }

    return Error;
}

/**
 *******************************************************************************
 ** \brief BootLoader复位配置
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void IAP_ResetConfig(void)
{
//	  uint8_t flag = 0xff;
	  
	  App_ClkCfg();
	  DeinitlpUart();
	  Flash_Init(6, TRUE);
	  Flash_LockAll();
}


static void DeinitlpUart(void)
{
  stc_uart_cfg_t  stcCfg;

	DDL_ZERO_STRUCT(stcCfg);
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);///<外设模块时钟使能
	//Uart_Init(M0P_UART0, &stcCfg);
	 
	///<LPUART 中断使能
	Uart_ClrStatus(M0P_UART0,UartRC);          ///<清接收中断请求
	//Uart_ClrStatus(M0P_UART0,UartTC);          ///<清发送中断请求
	Uart_DisableIrq(M0P_UART0,UartRxIrq);      ///<禁止接收中断
	//Uart_DisableIrq(M0P_UART0,UartTxIrq);       ///<失能发送中断
	EnableNvic(UART0_2_IRQn,IrqLevel1,FALSE);         ///<系统中断失能
}

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
