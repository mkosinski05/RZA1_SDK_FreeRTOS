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
 * @headerfile     cmd_tcp_shell.h
 * @brief          TCP command shell but without any telnet protocol
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef TCPCMDSHELL_H_
#define TCPCMDSHELL_H_

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_CONSOLE
 * @defgroup R_SW_PKG_93_CONSOLE_TCP Console TCP Shell
 * @brief TCP shell for for the console.
 *
 * @anchor R_SW_PKG_93_CONSOLE_TCP_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/

/******************************************************************************
 Macro definitions
 *****************************************************************************/

/******************************************************************************
 Typedef definitions
 *****************************************************************************/

/* The data structure is encapsulated */
typedef struct _TCSH *PTCSH;

/******************************************************************************
 Variable External definitions and Function External definitions
 *****************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
 *****************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Function to start a TCP command shell
 * @param usPortNumber - The port number to listen on
 * @return Pointer to the TCP command shell object or null on error
 */
extern PTCSH tcpCmdShellStart (uint16_t usPortNumber);

/**
 * @brief Function to stop the TCP command shell
 * @param pShell - Pointer to the shell object to stop
 */
extern void tcpCmdShellStop (PTCSH pShell);

/**
 * @brief Function called back by the driver when the idle time-out has expired.
 * @param uiTaskID - The ID of the task passed in the IDLECB struct
 * @param pvParameter - The parameter passed  the IDLECB struct
 */
extern void tcpShellTimeOut(uint32_t uiTaskID, void* pvParameter);

#ifdef __cplusplus
}
#endif

#endif /* TCPCMDSHELL_H_ */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
 End  Of File
 *****************************************************************************/
