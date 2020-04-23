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
 * @headerfile     r_adc_app.h
 * @brief          Application that uses the ADC connected to P1 on the Stream-it
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_ADC_APP_H
#define R_ADC_APP_H

/**************************************************************************//**
 * @ingroup RENESAS_APPLICATION_SOFTWARE_PACKAGE Software Package
 * @defgroup R_SW_PKG_93_ADC_APP ADC Application
 * @brief Application that uses the ADC connected to P1 on the RZA1LU Stream-it
 * board.
 * @anchor R_SW_PKG_93_ADC_APP_SUMMARY
 * @par Summary
 * @brief Application to read the ADC and display its value on the console.<br>
 * API for ADC driver supports open close and read functionality.<br><br>
 * Example interaction<br>
 * <em><br>
 * RZ/A1LU RZ/A Software Package Ver.3.01.0001<br>
 * Copyright (C) Renesas Electronics Europe.<br>
 * REE> adcdemo                                               </em><- Invoke the demonstration application<br><em>
 * ADC sample program start<br>
 * Press any key to terminate demo<br>
 * Rotate Potentiometer to see effect on adc input (AN2)<br>
 * Potentiometer(AN2) = 4093                                  </em><- Rotate Potentiometer<br><e>
 * ADC sample program terminated                              </em><- Press any key on keyboard to terminate<br><em>
 * REE><br>
 * </em>
 *
 * @anchor R_SW_PKG_93_ADC_APP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see R_SW_PKG_93_ADC_API
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

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
extern void R_ADC_SampleMain(FILE *pIn, FILE *pOut);

#endif /* R_ADC_APP_H */

/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
