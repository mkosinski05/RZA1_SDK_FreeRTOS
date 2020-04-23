/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : cmd_camerasdk.c
 * Version      : 1.0
 * Device(s)    : RZ/A1H (R7S721001)
 * Tool-Chain   : GCC ARM Embedded v6.3
 * OS           : FREERTOS
 * H/W Platform : Renesas Starter Kit+ for RZ/A1H
 * Description  : The SDK for Camera  specific commands
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 17.02.2018 1.00 First Release
 *              : 24.04.2019 2.00 Porting for RZ/A1H
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 System Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/******************************************************************************
 User Includes
 ******************************************************************************/

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "compiler_settings.h"
#include "control.h"
#include "command.h"
#include "r_task_priority.h"
#include "main.h"
#include "r_os_abstraction_api.h"
#include "r_cui.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
 External functions
 ******************************************************************************/
extern void R_MSG_WarningConfig(pst_comset_t p_com, char_t *msg);
/*****************************************************************************
 External Variables
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

static int16_t cmd_camerasdk(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/*****************************************************************************
 Global Variables
 ******************************************************************************/

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t gs_cmd_camerasdk[] =
{
     {
      "sdk",
      cmd_camerasdk,
      "<CR> - run the SDK for camera demo, requires screen and camera modules",
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_camerasdk =
{
    "Camera SDK Commands",
    gs_cmd_camerasdk,
    ((sizeof(gs_cmd_camerasdk)) / sizeof(st_cmdfnass_t))
};

/******************************************************************************
 Function Name : cmd_camerasdk
 Description   : Run the camera SDK demo, needs the LCD and Camera to be connected
                 NOTE Ethernet modules must be disabled when using LCD
                 This is because some pins are shared between modules.
 Arguments     : IN  iArgCount - The number of arguments in the argument list
                 IN  ppszArgument - The argument list
                 IN  pCom - Pointer to the command object
 Return value  : CMD_OK for success
 ******************************************************************************/
static int16_t cmd_camerasdk(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void) ppszArgument;
    (void) iArgCount;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#if R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES
    R_MSG_WarningConfig(pCom, "R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES");
#else
    R_SDK_ApplicationMain(pCom);
#endif

#elif (TARGET_BOARD == TARGET_BOARD_RSK)
    R_SDK_ApplicationMain(pCom);
#endif /* TARGET_BOARD */

	return CMD_OK;
}
/******************************************************************************
 End of function  cmd_camerasdk
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
