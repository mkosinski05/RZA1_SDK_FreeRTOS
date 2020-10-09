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
 * @headerfile     cmdCommsClass.h
 * @brief          Header file for cmdCommsClass.c
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef CMD_COMMS_CLASS_H
#define CMD_COMMS_CLASS_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_CONSOLE
 * @defgroup R_SW_PKG_93_CONSOLE_CMD_COMMS Console Comms
 * @brief Comms setup for the console.
 *
 * @anchor R_SW_PKG_93_CONSOLE_CMD_COMMS_INSTANCES
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

/******************************************************************************
 Variable External definitions and Function External definitions
 *****************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
 *****************************************************************************/

/**
 * @brief Command to set/clear the break output.
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_break_signal(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to set the baud rate
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_set_baud(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Get line status
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_get_line_status(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to set the stop bits
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_set_stop_bits(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to set the parity
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_set_parity(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to assert or de-assert both RTS and DTR control signals
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_set_control_lines(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to loop-back all data received
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_loop_back(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to send a specified quantity of data (in k Bytes)
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_test_tx(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to test the basic control API of the device driver
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_ctrl_api_test(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to open the device
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_close(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to open the device
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_open(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to test all the main functions of the CDC driver
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_test_all(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

/**
 * @brief Command to perform a loop back on attached CDC devices
          Note: Uses default CDC baud rate.
 * @param arg_count - The number of arguments in the argument list
 * @param ppsz_argument - The argument list
 * @param p_com - Pointer to the command object
 * @return CMD_OK for success
 */
extern int_t cmd_sci_loop_all(int_t arg_count, char **ppsz_argument, pst_comset_t p_com);

#endif // CMD_COMMS_CLASS_H
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
 End  Of File 
 *****************************************************************************/
