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
 * @headerfile     cmdMassStorage.h
 * @brief          The commands for support of MS devices include file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_CMD_MASS_STORAGE_H
    #define R_CMD_MASS_STORAGE_H

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_CONSOLE
 * @defgroup R_SW_PKG_93_CONSOLE_MS_USB Console USB Mass Storage Commands
 * @brief Console USB Mass Storage commands.
 *
 * @anchor R_SW_PKG_93_CONSOLE_MS_USB_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1LU Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
 System Includes
 ******************************************************************************/

/******************************************************************************
 User Includes
 ******************************************************************************/
    #include "command.h"
    #include "console.h"
    #include "control.h"

/******************************************************************************
 Defines
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/
/* Table that points to the command entries */
extern const st_command_table_t gcmdUsbMassStorage;

/******************************************************************************
 Exported global functions (to be accessed by other files)
 *****************************************************************************/

/**
 * @brief Function to display a progress indicator during file copies
 * @param bytes_done - Number of bytes completed
 * @param total_bytes - Total number of bytes in the file being copied
 * @param finished_string - pointer to string to output when progress is 100%
 * @param p_out - Pointer to the command object
 */
void update_progress_bar (uint32_t bytes_done, uint32_t total_bytes, const char *finished_string, FILE *p_out);

/**
 * @brief Function to make sure a path string has a \ on the front and not on
 * the end.
 * @param pszDest - Pointer to the destination
 * @param pszSrc - Pointer to the source
 */
void cmdTrimSlash (char *pszDest, char *pszSrc);

#endif /* R_CMD_MASS_STORAGE_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
 End  Of File
 ******************************************************************************/
