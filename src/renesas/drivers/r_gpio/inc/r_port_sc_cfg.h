/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_gpio_drv_sc_cfg.h
* Version      : 1.0.0
* Device(s)    : R7S921053
* Description  : Pin Configuration.
* Creation Date: 2019-04-04
***********************************************************************************************************************/

#ifndef _R_GPIO_CFG_H_
#define _R_GPIO_CFG_H_

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

/******************************************************************************
Variable Externs
******************************************************************************/
#endif  /* _R_GPIO_CFG_H_ */
static const st_port_config_t GPIO_SC_TABLE_scf0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf0 = { 0,	0,	&GPIO_SC_TABLE_scf0[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf1 = { 0,	0,	&GPIO_SC_TABLE_scf1[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf2 = { 0,	0,	&GPIO_SC_TABLE_scf2[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf3[] =
{
	{P7_10,	FUNCTION_MODE5,	PIN_INPUT},	// Pin Function : RxD3
	{P7_11,	FUNCTION_MODE5,	PIN_OUTPUT},	// Pin Function : TxD3
};
static const st_port_init_config_t GPIO_SC_INIT_scf3 = { 0,	2,	&GPIO_SC_TABLE_scf3[0]	};
static const st_port_config_t GPIO_SC_TABLE_scf4[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_scf4 = { 0,	0,	&GPIO_SC_TABLE_scf4[0]	};
static const st_port_config_t GPIO_SC_TABLE_sci0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci0 = { 0,	0,	&GPIO_SC_TABLE_sci0[0]	};
static const st_port_config_t GPIO_SC_TABLE_sci1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sci1 = { 0,	0,	&GPIO_SC_TABLE_sci1[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi0 = { 0,	0,	&GPIO_SC_TABLE_rspi0[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi1[] =
{
	{P6_12,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : RSPCK1
	{P6_13,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSL10
	{P6_15,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : MISO1
	{P6_14,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : MOSI1
};
static const st_port_init_config_t GPIO_SC_INIT_rspi1 = { 0,	4,	&GPIO_SC_TABLE_rspi1[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspi2 = { 0,	0,	&GPIO_SC_TABLE_rspi2[0]	};
static const st_port_config_t GPIO_SC_TABLE_rspb0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_rspb0 = { 0,	0,	&GPIO_SC_TABLE_rspb0[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c0[] =
{
	{P1_1,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SDA
	{P1_0,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC0SCL
};
static const st_port_init_config_t GPIO_SC_INIT_i2c0 = { 0,	2,	&GPIO_SC_TABLE_i2c0[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c1[] =
{
	{P1_2,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC1SCL
	{P1_3,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : RIIC1SDA
};
static const st_port_init_config_t GPIO_SC_INIT_i2c1 = { 0,	2,	&GPIO_SC_TABLE_i2c1[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_i2c2 = { 0,	0,	&GPIO_SC_TABLE_i2c2[0]	};
static const st_port_config_t GPIO_SC_TABLE_i2c3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_i2c3 = { 0,	0,	&GPIO_SC_TABLE_i2c3[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi0[] =
{
	{P6_9,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSIWS0
	{P6_8,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SSISCK0
	{P6_11,	FUNCTION_MODE3,	PIN_INPUT},	// Pin Function : SSIRxD0
	{P6_10,	FUNCTION_MODE3,	PIN_OUTPUT},	// Pin Function : SSITxD0
};
static const st_port_init_config_t GPIO_SC_INIT_ssi0 = { 0,	4,	&GPIO_SC_TABLE_ssi0[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi1 = { 0,	0,	&GPIO_SC_TABLE_ssi1[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi2[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi2 = { 0,	0,	&GPIO_SC_TABLE_ssi2[0]	};
static const st_port_config_t GPIO_SC_TABLE_ssi3[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ssi3 = { 0,	0,	&GPIO_SC_TABLE_ssi3[0]	};
static const st_port_config_t GPIO_SC_TABLE_irq[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_irq = { 0,	0,	&GPIO_SC_TABLE_irq[0]	};
static const st_port_config_t GPIO_SC_TABLE_mlb[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mlb = { 0,	0,	&GPIO_SC_TABLE_mlb[0]	};
static const st_port_config_t GPIO_SC_TABLE_can0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_can0 = { 0,	0,	&GPIO_SC_TABLE_can0[0]	};
static const st_port_config_t GPIO_SC_TABLE_iebus[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_iebus = { 0,	0,	&GPIO_SC_TABLE_iebus[0]	};
static const st_port_config_t GPIO_SC_TABLE_spdif[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_spdif = { 0,	0,	&GPIO_SC_TABLE_spdif[0]	};
static const st_port_config_t GPIO_SC_TABLE_lin[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_lin = { 0,	0,	&GPIO_SC_TABLE_lin[0]	};
static const st_port_config_t GPIO_SC_TABLE_et[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_et = { 0,	0,	&GPIO_SC_TABLE_et[0]	};
static const st_port_config_t GPIO_SC_TABLE_an[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_an = { 0,	0,	&GPIO_SC_TABLE_an[0]	};
static const st_port_config_t GPIO_SC_TABLE_usb0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_usb0 = { 0,	0,	&GPIO_SC_TABLE_usb0[0]	};
static const st_port_config_t GPIO_SC_TABLE_usb1[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_usb1 = { 0,	0,	&GPIO_SC_TABLE_usb1[0]	};
static const st_port_config_t GPIO_SC_TABLE_vdc[] =
{
	{P8_1,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA1
	{P8_0,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA0
	{P8_5,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA5
	{P8_4,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA4
	{P8_3,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA3
	{P8_2,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA2
	{P6_1,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA9
	{P6_0,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA8
	{P8_7,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA7
	{P8_6,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_DATA6
	{P6_5,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA13
	{P6_6,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA14
	{P6_7,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA15
	{P6_2,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA10
	{P6_3,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA11
	{P6_4,	FUNCTION_MODE2,	PIN_OUTPUT},	// Pin Function : LCD0_DATA12
	{P7_4,	FUNCTION_MODE6,	PIN_OUTPUT},	// Pin Function : LCD0_CLK
	{P8_11,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_TCON3
	{P8_10,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_TCON2
	{P8_12,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : LCD0_TCON4
};
static const st_port_init_config_t GPIO_SC_INIT_vdc = { 0,	20,	&GPIO_SC_TABLE_vdc[0]	};
static const st_port_config_t GPIO_SC_TABLE_ceu[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_ceu = { 0,	0,	&GPIO_SC_TABLE_ceu[0]	};
static const st_port_config_t GPIO_SC_TABLE_sd0[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_sd0 = { 0,	0,	&GPIO_SC_TABLE_sd0[0]	};
static const st_port_config_t GPIO_SC_TABLE_sd1[] =
{
	{P7_0,	FUNCTION_MODE3,	PIN_INPUT},	// Pin Function : SD_CD_1
	{P7_6,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SD_D3_1
	{P7_7,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SD_D2_1
	{P7_2,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SD_D1_1
	{P7_3,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SD_D0_1
	{P7_5,	FUNCTION_MODE3,	PIN_IO},	// Pin Function : SD_CMD_1
};
static const st_port_init_config_t GPIO_SC_INIT_sd1 = { 0,	6,	&GPIO_SC_TABLE_sd1[0]	};
static const st_port_config_t GPIO_SC_TABLE_mmc[] =
{
};
static const st_port_init_config_t GPIO_SC_INIT_mmc = { 0,	0,	&GPIO_SC_TABLE_mmc[0]	};
static const st_port_config_t GPIO_SC_TABLE_bsc[] =
{
	{P3_0,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A1
	{P3_1,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A2
	{P3_2,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A3
	{P3_3,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A4
	{P3_4,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A5
	{P3_5,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A6
	{P3_6,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A7
	{P3_7,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A8
	{P3_8,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A9
	{P5_0,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D0
	{P5_1,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D1
	{P5_2,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D2
	{P5_3,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D3
	{P5_4,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D4
	{P5_5,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D5
	{P5_6,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D6
	{P5_7,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D7
	{P5_8,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D8
	{P5_9,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D9
	{P2_6,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : RD/WR#
	{P2_1,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : RAS#
	{P2_3,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : CKE
	{P5_10,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D10
	{P5_12,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D12
	{P5_11,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D11
	{P5_14,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D14
	{P5_13,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D13
	{P5_15,	FUNCTION_MODE1,	PIN_IO},	// Pin Function : D15
	{P2_4,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : WE0#/DQMLL
	{P3_9,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A10
	{P3_10,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A11
	{P3_11,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A12
	{P3_12,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A13
	{P3_13,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A14
	{P3_14,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : A15
	{P2_5,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : WE1#/WE#/DQMLU
	{P2_0,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : CS3#
	{P2_2,	FUNCTION_MODE1,	PIN_OUTPUT},	// Pin Function : CAS#
};
static const st_port_init_config_t GPIO_SC_INIT_bsc = { 0,	38,	&GPIO_SC_TABLE_bsc[0]	};

/* End of File */
