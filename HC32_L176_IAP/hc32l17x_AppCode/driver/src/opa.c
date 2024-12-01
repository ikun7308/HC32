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
 * @file   opa.c
 *
 * @brief  Source file for OPA functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "opa.h"

/**
 ******************************************************************************
 ** \addtogroup OPAGroup
 ******************************************************************************/
//@{

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *****************************************************************************/

/**
******************************************************************************
    ** \brief  OPA 通道使能
    **
    ** \param  NewStatus : TRUE FALSE    
    ** \retval 无
    **
******************************************************************************/
void Opa_Cmd(boolean_t NewStatus)
{
    SetBit((uint32_t)(&(M0P_OPA->CR0)), 0, NewStatus);
}

void Opa_CmdBuf(boolean_t NewStatus)
{
    SetBit((uint32_t)(&(M0P_OPA->CR0)), 2, NewStatus);
}

/**
******************************************************************************
    ** \brief  OPA零点校准配置
    **
    ** \param  InitZero :  
    ** \retval 无
    **
******************************************************************************/
void Opa_SetZero(stc_opa_zcfg_t* InitZero)
{
    M0P_OPA->CR0_f.AZEN       = InitZero->bAzen;
    M0P_OPA->CR1_f.CLK_SW_SET = InitZero->bClk_sw_set;
    M0P_OPA->CR1_f.AZ_PULSE   = InitZero->bAz_pulse;
    M0P_OPA->CR1_f.TRIGGER    = InitZero->bTrigger;
    M0P_OPA->CR1_f.ADCTR_EN   = InitZero->bAdctr_en; 
}

/**
******************************************************************************
    ** \brief  使能输出OUTX
    **
    ** \param  onex : en_opa_oenx_t定义的元素
    ** \retval 无
    **
******************************************************************************/
void Opa_CmdOnex(en_opa_oenx_t onex, boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_OPA->CR0)), onex, NewState);  //使能OP3输出X使能
}

/**
******************************************************************************
    ** \brief  配置校零相关的位
    **
    ** \param  CtrlBit : en_opa_set0ctrl_t定义的元素
    ** \param  NewState: TRUE 或 FALSE
    ** \retval 无
    **
******************************************************************************/
void Opa_ZeroBitCtrl(en_opa_set0ctrl_t CtrlBit, boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_OPA->CR1)), CtrlBit, NewState);  
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

