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
 * @headerfile     r_switch_monitor.h
 * @brief          Monitors the user switch
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RENESAS_APPLICATION_APP_SWITCH_INC_R_SWITCH_MONITOR_H_
#define RENESAS_APPLICATION_APP_SWITCH_INC_R_SWITCH_MONITOR_H_

/**************************************************************************//**
 * @ingroup RENESAS_APPLICATION_SOFTWARE_PACKAGE Software Package
 * @defgroup R_SW_PKG_93_SWITCH_APP Switch Application
 * @brief Application that monitors the user switch on the RZA1LU Stream-it
 * board.
 * @anchor R_SW_PKG_93_SWITCH_APP_SUMMARY
 * @par Summary
 * @brief Application that monitors the user switch on the RZA1LU Stream-it
 * board.<br>
 *
 * This application task is run automatically, being invoked in the main task.
 *
 * The console will report the following when the user switch is pressed:<br>
 * <em><br>
 * REE> USER switch was pushed!!<br></em>
 * <br>
 *
 * @anchor R_SW_PKG_93_SWITCH_APP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @see R_SW_PKG_93_SWITCH_API
 * @{
 *****************************************************************************/

/******************************************************************************
User Includes
******************************************************************************/
#include <stdio.h>

/******************************************************************************
 Function prototypes
 ******************************************************************************/

/**
 * @brief Initialises the switch driver and creates the switch monitor task
 * @param p_out : output to console
 */
void initialise_switch_monitor_task (FILE *p_out);

#endif /* RENESAS_APPLICATION_APP_SWITCH_INC_R_SWITCH_MONITOR_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
