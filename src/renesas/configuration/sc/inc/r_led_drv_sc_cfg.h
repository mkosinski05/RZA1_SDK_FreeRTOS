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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_led_drv_sc_cfg.h
 * @brief          LED low layer driver configuration header, part of the low
 *                 layer driver.
 *                 This file uses the same name for any low layer driver that is
 *                 supported by a compatible high layer driver.
 *                 File is included in the high layer driver to allow the low
 *                 layer driver to be swapped at build time and share the same
 *                 high layer code.
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SRC_RENESAS_CONFIG_R_LED_DRV_SC_CFG_H_
#define SRC_RENESAS_CONFIG_R_LED_DRV_SC_CFG_H_

/**************************************************************************//**
 * @ingroup Interface_Library
 * @ingroup R_SW_PKG_93_SC_CFG
 * @ingroup R_SW_PKG_93_LED_API
 * @defgroup LED_SC_IF SC LED API Interface
 * @brief Interface between Smart Configurator and the LED module.
 *
 * @anchor LED_SC_IF
 * @par SC_SUMMARY Summary
 * @brief The interface allows Smart Configurator (SC) to manage the
 * configuration of the LED driver.
 * Do not edit this file if using SC
 *
 * @anchor LED_SC_IF_INSTANCES
 * @par Known Implementations
 * This driver is used in the RZA1LU Software Package.
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

#include "r_led_drv_link.h"

/** List LEDS supported */
#define R_CFG_LEDS_SUPPORTED         ( R_CH0 )

#endif /* SRC_RENESAS_CONFIG_R_LED_DRV_SC_CFG_H_ */

/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/

/**************************************************************************//**
  End  Of File
 *****************************************************************************/
