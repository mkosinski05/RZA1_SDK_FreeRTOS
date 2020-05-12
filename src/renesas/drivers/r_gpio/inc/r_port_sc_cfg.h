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
 * File Name     : r_port_sc_cfg.h
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : May 7, 2020 1.00
 *******************************************************************************/
#ifndef RENESAS_DRIVERS_R_GPIO_INC_R_PORT_SC_CFG_H_
#define RENESAS_DRIVERS_R_GPIO_INC_R_PORT_SC_CFG_H_

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_port_if.h"

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

static const PinMap GPIO_SC_TABLE_riic0[] =
{
		{P1_0, FUNCTION_MODE1, PIN_IO},
		{P1_1, FUNCTION_MODE1, PIN_IO}
};

static const PinMap GPIO_SC_TABLE_riic1[] =
{
		{P1_2, FUNCTION_MODE1, PIN_IO},
		{P1_3, FUNCTION_MODE1, PIN_IO}
};
static const PinMap GPIO_SC_TABLE_rssi0[] =
{
		{P6_8,  FUNCTION_MODE3, PIN_IO},
		{P6_9,  FUNCTION_MODE3, PIN_IO},
		{P6_10, FUNCTION_MODE3, PIN_OUTPUT},
		{P6_11, FUNCTION_MODE3, PIN_INPUT}
};

static const PinMap GPIO_SC_TABLE_rsci3[] =
{
		{P7_11, FUNCTION_MODE5, PIN_OUTPUT},
		{P7_10, FUNCTION_MODE5, PIN_INPUT}
};

static const PinMap GPIO_SC_TABLE_rvdc0[] =
{
		{P7_4,  FUNCTION_MODE6, PIN_OUTPUT},
		{P8_10, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_12, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_11, FUNCTION_MODE1, PIN_OUTPUT},

		{P6_0, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_1, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_2, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_3, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_4, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_5, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_6, FUNCTION_MODE2, PIN_OUTPUT},
		{P6_7, FUNCTION_MODE2, PIN_OUTPUT},

		{P8_0, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_1, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_2, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_3, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_4, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_5, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_6, FUNCTION_MODE1, PIN_OUTPUT},
		{P8_7, FUNCTION_MODE1, PIN_OUTPUT}

};
static const PinMap GPIO_SC_TABLE_rspi0[] =
{
		{P1_9,  FUNCTION_MODE2, PIN_INPUT},
		/*{P3_15, FUNCTION_MODE1, PIN_OUTPUT},*/
		/*{P9_5, FUNCTION_MODE1, PIN_OUTPUT},*/
		{P6_12, FUNCTION_MODE3, PIN_IO},
		/*{P6_13, FUNCTION_MODE1, PIN_OUTPUT},*/
		{P6_14, FUNCTION_MODE3, PIN_IO},
		{P6_15, FUNCTION_MODE3, PIN_IO}
};
#endif /* RENESAS_DRIVERS_R_GPIO_INC_R_PORT_SC_CFG_H_ */
