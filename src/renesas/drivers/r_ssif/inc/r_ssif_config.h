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
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name     : r_ssif_config.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : May 15, 2020 1.00
 *******************************************************************************/
#ifndef RENESAS_DRIVERS_R_SSIF_INC_R_SSIF_CONFIG_H_
#define RENESAS_DRIVERS_R_SSIF_INC_R_SSIF_CONFIG_H_


/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "ssif_if.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/ 

/******************************************************************************
 Macro definitions
 ******************************************************************************/
 
/*****************************************************************************
 Constant Data
 ******************************************************************************/
 
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

ssif_channel_cfg_t gs_ssif_cfg[SSIF_NUM_CHANS] =
{
		// Channel 0
		{
			false,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_CLK,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
		// Channel 1
		{
			false,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_CLK,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
		// Channel 2
		{
			true,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_X1,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
		// Channel 3
		{
			false,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_X1,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
#if (TARGET_RZA1 <= TARGET_RZA1H)
		// Channel 4
		{
			false,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_X1,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
		// Channel 5
		{
			false,
			ISR_SSIF_IRQ_PRIORITY,
			false,
			44100u,
			SSIF_CFG_CKS_AUDIO_X1,
			SSIF_CFG_MULTI_CH_1,
			SSIF_CFG_DATA_WORD_16,
			SSIF_CFG_SYSTEM_WORD_32,
			SSIF_CFG_FALLING,
			SSIF_CFG_WS_LOW,
			SSIF_CFG_PADDING_LOW,
			SSIF_CFG_DATA_FIRST,
			SSIF_CFG_LEFT,
			SSIF_CFG_DELAY,
			SSIF_CFG_DISABLE_NOISE_CANCEL,
			SSIF_CFG_DISABLE_TDM
		},
#endif

};
#endif /* RENESAS_DRIVERS_R_SSIF_INC_R_SSIF_CONFIG_H_ */
