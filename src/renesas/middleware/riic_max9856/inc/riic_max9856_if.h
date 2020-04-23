/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file          riic_ch1_if.h
* $Rev: 8700 $
* $Date:: 2018-06-29 15:46:04 +0100#$
* @brief         RIIC CH1 driver API header
******************************************************************************/

#ifndef RIIC_CH1_IF_H
#define RIIC_CH1_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#include "cmsis_os.h"
#include "r_typedefs.h"
#include "r_errno.h"
#include "riic_max9856_drv.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief   Open RIIC CH1 and initialize synchronous resource
 * 
 * @retval        DEVDRV_SUCCESS: Success.
 * @retval        error code:     Failure.
 */
extern int32_t R_RIIC_MAX9856_Open(void);

/**
 * @brief    Close RIIC CH1 and free synchronous resource
 * 
 * @retval        DEVDRV_SUCCESS: Success.
 * @retval        error code:     Failure.
 */
extern int32_t R_RIIC_MAX9856_Close(void);

/** 
 * @brief         Write register to MAX9856
 *            
 * @param[in]     addr:  Register address 0-28
 * @param[in]     dat:   Register data 0-255
 * 
 * @retval        DEVDRV_SUCCESS:   Success.
 * @retval        error code:       Failure.
 */
extern int32_t R_RIIC_MAX9856_Write(const uint8_t addr, const uint8_t dat);

/**
 * @brief         Read register from MAX9856
 *                
 * @param[in]     addr:    register address 0-28
 * @param[out]    p_dat:   store address of register data 0-255
 *
 * @retval        DEVDRV_SUCCESS:  Success.
 * @retval        error code:      Failure.
 */
extern int32_t R_RIIC_MAX9856_Read(const uint8_t addr, uint8_t* const p_dat);

/**
 * @brief         Get RIIC_CH1 driver version.

 * @retval        driver version:
 *                    upper 8bit=MAJOR
 *                    lower 8bit=MINOR
 */
extern uint16_t RIIC_MAX9856_GetVersion(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#else /* (TARGET_BOARD == TARGET_BOARD_STREAM_IT2) */
#endif

#endif /* RIIC_CH1_IF_H */

