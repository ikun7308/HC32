/******************************************************************************
 * Copyright (C) 2021, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************/

/******************************************************************************
 * @file   pca.h
 *
 * @brief  Header file for PCA functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __PCA_H__
#define __PCA_H__

/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup PcaGroup Programmable Counter Array (PCA)
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PCA  模块选择
 ** \note 
 ******************************************************************************/
typedef enum
{
    PcaModule0 = 0,      //  PCA_0
    PcaModule1 = 1,      //  PCA_1
    PcaModule2 = 2,      //  PCA_2
    PcaModule3 = 3,      //  PCA_3
    PcaModule4 = 4       //  PCA_4
}en_pca_module_t;

/**
 *******************************************************************************
 ** \brief PCA  中断标志位  中断清除位
 ** \note           PCA_CCON中的CCF0-CCF4与CF; PCA_ICLR中的CCF0-CCF4与CF
 ******************************************************************************/
typedef enum
{
    PcaCcf0 = 0,
    PcaCcf1 = 1,
    PcaCcf2 = 2,
    PcaCcf3 = 3,
    PcaCcf4 = 4,
    PcaCf   = 7
}en_pca_ccficlr_t;

/**
 *******************************************************************************
 ** \brief PCA  时钟分频选择及时钟源选择
 ** \note       PCA_CMOD   CPS[2:0]
 ******************************************************************************/
typedef enum
{
    PcaPclkdiv32 = 0,      // PCLK/32
    PcaPclkdiv16 = 1,      // PCLK/16
    PcaPclkdiv8  = 2,      // PCLK/8
    PcaPclkdiv4  = 3,      // PCLK/4
    PcaPclkdiv2  = 4,      // PCLK/2
    PcaTim0ovf   = 5,      // timer0 overflow
    PcaTim1ovf   = 6,      // timer1 overflow
    PcaEci       = 7       // ECI外部时钟
}en_pca_clksrc_t;

/**
 *******************************************************************************
 ** \brief PCA  允许比较器功能控制
 ** \note       PCA_CCAPMx   ECOM
 ******************************************************************************/
typedef enum
{
    PcaEcomDisable = 0,
    PcaEcomEnable  = 1
}en_pca_ecom_t;

/**
 *******************************************************************************
 ** \brief PCA  正沿捕获控制位
 ** \note       PCA_CCAPMx  CAPP
 ******************************************************************************/
typedef enum
{
    PcaCappDisable = 0,
    PcaCappEnable  = 1
}en_pca_capp_t;

/**
 *******************************************************************************
 ** \brief PCA  负沿捕获控制位
 ** \note       PCA_CCAPMx  CAPN
 ******************************************************************************/
typedef enum
{
    PcaCapnDisable = 0,
    PcaCapnEnable  = 1
}en_pca_capn_t;

/**
 *******************************************************************************
 ** \brief PCA  匹配控制位
 ** \note       PCA_CCAPMx  MAT
 ******************************************************************************/
typedef enum
{
    PcaMatDisable = 0,
    PcaMatEnable  = 1
}en_pca_mat_t;

/**
 *******************************************************************************
 ** \brief PCA  翻转控制位
 ** \note       PCA_CCAPMx  TOG
 ******************************************************************************/
typedef enum
{
    PcaTogDisable = 0,
    PcaTogEnable  = 1
}en_pca_tog_t;

/**
 *******************************************************************************
 ** \brief PCA  PWM控制位
 ** \note       PCA_CCAPMx  PWM
 ******************************************************************************/
typedef enum
{
    PcaPwm8bitDisable = 0,
    PcaPwm8bitEnable  = 1
}en_pca_pwm8bit_t;

/**
 *******************************************************************************
 ** \brief PCA  EPWM控制位
 ** \note       PCA_EPWM  EPWM
 ******************************************************************************/
typedef enum
{
    PcaEpwmDisable = 0,
    PcaEpwmEnable  = 1
}en_pca_epwm_t;

/**
 *******************************************************************************
 ** \brief PCA  初始化配置的结构体
 ** \note       
 ******************************************************************************/
typedef struct
{
    en_pca_clksrc_t     pca_clksrc;           // PCA_CMOD CPS[2:0] 
    boolean_t           pca_cidl;             // PCA_CMOD CIDL
    en_pca_ecom_t       pca_ecom;             // PCA_CCAPMx ECOM
    en_pca_capp_t       pca_capp;             // PCA_CCAPMx CAPP
    en_pca_capn_t       pca_capn;             // PCA_CCAPMx CAPN
    en_pca_mat_t        pca_mat;              // PCA_CCAPMx MAT
    en_pca_tog_t        pca_tog;              // PCA_CCAPMx TOG
    en_pca_pwm8bit_t    pca_pwm;              // PCA_CCAPMx PWM
    en_pca_epwm_t       pca_epwm;             // PCA_EPWM
    uint16_t            pca_ccap;             // PCA_CCAP
    uint8_t             pca_ccapl;            // PCA_CCAPL  PCA_CCAP的低字节
    uint8_t             pca_ccaph;            // PCA_CCAPH  PCA_CCAP的高字节
    uint16_t            pca_carr;             // PCA_CARR
}stc_pcacfg_t;
/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
extern boolean_t Pca_GetItStatus(en_pca_ccficlr_t It_Src);
extern void Pca_ClrItStatus(en_pca_ccficlr_t It_Src);
extern void Pca_StartPca(boolean_t NewStatus);
extern void Pca_SetCidl(boolean_t NewStatus);
extern void Pca_Set4Wdte(boolean_t NewStatus);
extern void Pca_ConfPcaIt(boolean_t NewStatus);
extern void Pca_ConfModulexIt(en_pca_module_t Modulex, boolean_t NewStatus);
extern void Pca_M0Init(stc_pcacfg_t* InitStruct);
extern void Pca_M1Init(stc_pcacfg_t* InitStruct);
extern void Pca_M2Init(stc_pcacfg_t* InitStruct);
extern void Pca_M3Init(stc_pcacfg_t* InitStruct);
extern void Pca_M4Init(stc_pcacfg_t* InitStruct);
extern uint16_t Pca_GetCnt(void);
extern void Pca_SetCnt(uint16_t cnt);
extern boolean_t Pca_GetOut(en_pca_module_t Modulex);
extern void Pca_SetCcap(en_pca_module_t Modulex, uint16_t Value);
extern uint16_t Pca_GetCcap(en_pca_module_t Modulex);
extern void Pca_SetCarr(uint16_t Value);
extern uint16_t Pca_GetCarr(void);
extern void Pca_SetCcapHL(en_pca_module_t Modulex, uint8_t ValueH, uint8_t ValueL);
extern void Pca_GetCcapHL(en_pca_module_t Modulex, uint8_t *ValueH, uint8_t *ValueL);

//@} // PcaGroup

#ifdef __cplusplus
#endif

#endif /* __PCA_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
