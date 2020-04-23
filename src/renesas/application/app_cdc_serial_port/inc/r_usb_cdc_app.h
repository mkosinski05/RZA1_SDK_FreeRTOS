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
 * @headerfile     usb_cdc_app.c
 * @brief          Application that uses the USB CDC class.
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef USB_CDC_APP_H
#define USB_CDC_APP_H

/**************************************************************************//**
 * @ingroup RENESAS_APPLICATION_SOFTWARE_PACKAGE Software Package
 * @defgroup R_SW_PKG_93_USB_CDC_APP USB CDC Application
 * @brief Application that uses the USB CDC class.
 * @anchor R_SW_PKG_93_USB_CDC_APP_SUMMARY
 * @par Summary
 * @brief Application that uses the USB CDC class.<br>
 * The device will appear as a virtual COM port to the host.
 * When the host connects to this virtual COM port using a
 * terminal program such as MS HyperTerminal, instructions will appear.
 * A menu is provided that can be controlled by pressing the RSK switches.
 *
 * @anchor R_SW_PKG_93_USB_CDC_APP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
/* Following header file provides definition common to Upper and Low Level USB 
   driver. */
#include "usb_common.h"


/******************************************************************************
Enumerated Types
******************************************************************************/


/******************************************************************************
Constant Data
******************************************************************************/

/******************************************************************************
External Variables
******************************************************************************/

/******************************************************************************
Global Variables
******************************************************************************/

/******************************************************************************
Public Functions
******************************************************************************/

/**
 *  @brief ADC Sample Application
 *  @param[in] pIn  : Input stream
 *  @param[in] pOut : Output stream
 */
void R_CDC_SampleMain(FILE *pIn, FILE *pOut);

#endif /* USB_CDC_APP_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
