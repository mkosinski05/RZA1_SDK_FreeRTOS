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
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_pmod_sample_main.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-RZv14.01-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : PMOD display board sample 
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.08.2015 1.00    Initial Version
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "r_task_priority.h"

#include "command.h"
#include "console.h"
#include "trace.h"

/* GNU Compiler settings */
#include "compiler_settings.h"
#include "r_pmod_samplemain.h"

/* PMOD display API */
#include "r_pmod_lcd_drv_api.h"

#include "main.h"
#include "r_src_resource.h"

/* Comment this line out to turn ON module trace in this file */
/* #undef _TRACE_ON_ */

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/******************************************************************************
Macro definitions
******************************************************************************/

static st_pmod_config_t * sp_pmod_pactive_if = 0;

/* handle to PMOD driver */
static int_t s_pmod_lcdh = (-1);

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
static int16_t cmd_pmod_demo (int iArgCount, char **ppszArgument, pst_comset_t pCom);
static void task_execute_pmod (void *parameters);

/* Table that associates command letters, function pointer and a little
 description of what the command does */
st_cmdfnass_t g_pcmd_pmod_okaya_sample[] =
{
    {
        "pmoddemo",
        cmd_pmod_demo,
        "<CR> - Run PMOD sample"
    }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_pmod_app =
{
    "PMOD Display Commands",
    g_pcmd_pmod_okaya_sample,
    ((sizeof(g_pcmd_pmod_okaya_sample))/(sizeof(st_cmdfnass_t)))
};

/******************************************************************************
Global variables and functions
******************************************************************************/

extern unsigned char g_ac_desert[];
extern unsigned char g_ac_hydrangeas[];
extern unsigned char g_ac_penguins[];

/******************************************************************************
* Function Name: cmd_pmod_demo
* Description  : 
* Arguments    : none
* Return Value : none
******************************************************************************/
static int16_t cmd_pmod_demo (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    /* Parameter not used */
    UNUSED_PARAM(iArgCount);

    /* Parameter not used */
    UNUSED_PARAM(ppszArgument);


    if (0 == sp_pmod_pactive_if)
    {
        /* Initialse the control structure (only needed once) */
        sp_pmod_pactive_if = R_OS_AllocMem(sizeof(st_pmod_config_t),R_REGION_LARGE_CAPACITY_RAM);

        if (0 != sp_pmod_pactive_if)
        {
            memset(sp_pmod_pactive_if, 0, sizeof(st_pmod_config_t));

            sp_pmod_pactive_if->active = false;
            sp_pmod_pactive_if->image_angle = 1;

        }
    }

    if(false == sp_pmod_pactive_if->active)
    {
        if(true == R_SCR_ResourceLockDisplay())
        {
            s_pmod_lcdh = open(DEVICE_INDENTIFIER "pmod okaya", 0);

            R_SCR_ResourceUnlockDisplay();

            if((-1) != s_pmod_lcdh)
            {
                sp_pmod_pactive_if->active = true;

                sp_pmod_pactive_if->taskid =  R_OS_CreateTask(
                        "PMOD Demo",
                        task_execute_pmod,
                        pCom,
                        R_OS_ABSTRACTION_PRV_SMALL_STACK_SIZE,
                        TASK_PMOD_APP_PRI);

                R_OS_TaskSleep(3200);
            }
            else
            {
                fprintf(pCom->p_out, "cmd_pmod_start pmod driver can not be loaded\r\n");
            }
        }
    }
    else
    {
        fprintf(pCom->p_out, "cmd_pmod_start task running\r\n");
    }
    return 0;
}
/******************************************************************************
* End of function cmd_pmod_demo
******************************************************************************/

/******************************************************************************
* Function Name: task_execute_pmod
* Description  : Test task to flash the LEDs
* Arguments    : none
* Return Value : none
******************************************************************************/
static void task_execute_pmod (void *parameters)
{
    /* perform visual inspection of task creation call to ensure a pst_comset_t is passed as parameter */
    pst_comset_t p_com = (pst_comset_t)parameters;
    int32_t      i_led_map = 0;
    uint32_t i_sleep_count = 1000UL;
    e_colour_code_pmod_lcd_t col = R_PMOD_COL_WHITE;
    st_image_def_pmod_lcd_t new_image;

    new_image.image_height = 128;
    new_image.image_width  = 128;
    new_image.loc_x = 0;
    new_image.loc_y = 0;

    /* Lock Pin use (if required) */
    R_SCR_ResourceLockDisplay();

    control(s_pmod_lcdh, CTL_PMOD_LCD_R_LOCK, 0);
    control(s_pmod_lcdh, CTL_PMOD_LCD_R_DISPLAY_ON, 0);

    while(true == sp_pmod_pactive_if->active)
    {
        switch(i_led_map)
        {
            case 0:
            {
                new_image.image = &g_ac_desert[0];
                control(s_pmod_lcdh, CTRL_PMOD_LCD_DRAW_BMP_IMAGE, &new_image);
                fprintf(p_com->p_out, "PMOD Display Image g_ac_desert\r\n");
                i_led_map++;
            }
            break;
            case 1:
            {
                new_image.image = &g_ac_hydrangeas[0];
                control(s_pmod_lcdh, CTRL_PMOD_LCD_DRAW_BMP_IMAGE, &new_image);
                fprintf(p_com->p_out, "PMOD Display Image g_ac_hydrangeas\r\n");
                i_led_map++;
            }
            break;
            case 2:
            {
                new_image.image = &g_ac_penguins[0];
                control(s_pmod_lcdh, CTRL_PMOD_LCD_DRAW_BMP_IMAGE, &new_image);
                fprintf(p_com->p_out, "PMOD Display Image g_ac_penguins\r\n");
                i_led_map = 0;
                sp_pmod_pactive_if->active = false;
            }
            break;
            default:
            {
                i_led_map = -1;
            }
        }
        control(s_pmod_lcdh, CTL_PMOD_LCD_UPDATE_DISPLAY, 0);
        R_OS_TaskSleep(i_sleep_count);
    }

    control(s_pmod_lcdh, CTRL_PMOD_LCD_DISPLAY_CLEAR, &col);
    control(s_pmod_lcdh, CTL_PMOD_LCD_UPDATE_DISPLAY, 0);
    control(s_pmod_lcdh, CTL_PMOD_LCD_R_DISPLAY_OFF, 0);
    control(s_pmod_lcdh, CTL_PMOD_LCD_R_UNLOCK, 0);

    /* close the driver, so that re-opening it does not hog resources */
    close(s_pmod_lcdh);

    R_SCR_ResourceUnlockDisplay();

    R_OS_DeleteTask(R_OS_GetCurrentTask());
}
/*******************************************************************************
 End of function task_execute_pmod
 ******************************************************************************/

/* End of File */

