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
 * File Name    : cmd_guiliani.c
 * Version      : 1.0
 * Device(s)    : RZ/A1H (R7S721001)
 * Tool-Chain   : GCC ARM Embedded v6.3
 * OS           : FREERTOS
 * H/W Platform : Renesas Starter Kit+ for RZ/A1H
 * Description  : The guiliani  specific commands
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

/******************************************************************************
 User Includes
 ******************************************************************************/

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "compiler_settings.h"
#include "control.h"
#include "r_os_abstraction_api.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
 External functions
 ******************************************************************************/
extern void R_MSG_WarningConfig(pst_comset_t p_com, char_t *msg);
extern int_t gui_main(void);

/*****************************************************************************
 External Variables
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

static int16_t cmd_guiliani_demo(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t gs_cmd_guiliani[] =
{
     {
          "gui",
          cmd_guiliani_demo,
          "<CR> - run the touch screen demo",
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_guiliani[] =
{
     {
      "GUI Commands",
      gs_cmd_guiliani,
      ((sizeof(gs_cmd_guiliani)) / sizeof(st_cmdfnass_t))
     }
};

/******************************************************************************
Function Name : cmd_guiliani_demo
Description   : Run the touch screen demo, needs the LCD to be connected
Arguments     : IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
Return value  : CMD_OK for success
******************************************************************************/
static int16_t cmd_guiliani_demo(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void) ppszArgument;
    (void) iArgCount;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

#if R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES
    R_MSG_WarningConfig(pCom, "R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES");
#else
    pst_comset_t console = pCom;

    fprintf(console->p_out,
            "Starting TES Guiliani demo.\r\n"
            "Program will never return to command console.\r\n");
    gui_main();
#endif

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

	pst_comset_t console = pCom;

    fprintf(console->p_out,
            "Starting TES Guiliani demo.\r\n"
            "Program will never return to command console.\r\n");
    gui_main();

#endif /* TARGET_BOARD */

	return CMD_OK;
}
/******************************************************************************
 End of function cmd_guiliani_demo
 ******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
