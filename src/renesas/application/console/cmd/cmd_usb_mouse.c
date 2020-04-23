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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************
 * File Name    : cmd_usb_mouse.c
 * Version      : 0
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The platform specific commands
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              :                 First Release
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
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

/******************************************************************************
 User Includes
 ******************************************************************************/
#include "command.h"
#include "console.h"
#include "cmd_usb_mouse.h"
#include "r_typedefs.h"
#include "r_os_abstraction_api.h"
#include "trace.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif


/******************************************************************************
 Defines
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/
static int16_t cmdMoveMouseX(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmdMoveMouseY(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmdMoveMouseXY(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmdToggleButton1(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmdToggleButton2(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmdMouseLogout(int iArgCount, char **ppszArgument, pst_comset_t pCom);

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t gs_cmd_usb_mouse[] =
{
     {
        "movex",
        cmdMoveMouseX,
        "X<CR> - Move mouse position to this X coordinate.",
     },
     {
         "movey",
        cmdMoveMouseY,
        "Y<CR> - Move mouse position to this Y coordinate.",
     },
     {
        "movexy",
        cmdMoveMouseXY,
        "X Y<CR> - Move mouse position to these X, Y coordinates.",
     },
     {
        "button1",
        cmdToggleButton1,
        "<CR> - Press Button 1 on mouse.",
     },
     {
        "button2",
        cmdToggleButton2,
        "<CR> - Press Button 2 on mouse",
     },
     {
        "logout",
        cmdMouseLogout,
        "<CR - Quit Mouse Application.>"
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_UsbMouse =
{
        "USB Mouse Commands",
        gs_cmd_usb_mouse,
        ( (sizeof(gs_cmd_usb_mouse)) / (sizeof(st_cmdfnass_t)) )
};


static st_r_mouse_state_t s_mstate = {};

extern void mse_send_in_report(bool_t _button_state, int8_t _xval, int8_t _yval);


/**
 *
 */
static void reset_relpos(void)
{
    s_mstate.relative_x = 0;
    s_mstate.relative_y = 0;
}

/**
 *
 */
static void update_msepos(void)
{
    int_t remain_x = s_mstate.relative_x;
    int_t remain_y = s_mstate.relative_y;
    int8_t data_x   = 0;
    int8_t data_y   = 0;
    bool_t in_progress = true;
    int_t dir;

    s_mstate.current_x += s_mstate.relative_x;
    s_mstate.current_y += s_mstate.relative_y;

    do
    {
        /* Process x-axis */
        dir = s_mstate.relative_x/abs(s_mstate.relative_x);
        if(abs(s_mstate.relative_x) >= 8)
        {
            data_x = (int8_t)(dir * 8);
            remain_x -= data_x;
            s_mstate.relative_x = remain_x;
        }
        else
        {
            data_x = (int8_t)remain_x;
            remain_x = 0;
        }

        /* Process y-axis */
        dir = s_mstate.relative_y/abs(s_mstate.relative_y);
        if(abs(s_mstate.relative_y) >= 8)
        {
            data_y = (int8_t)(dir * 8);
            remain_y -= data_y;
            s_mstate.relative_y = remain_y;
        }
        else
        {
            data_y = (int8_t)remain_y;
            remain_y = 0;
        }

        if((0 == remain_x) & (0 == remain_y))
        {
            in_progress = false;
        }

        /*Send IN Report to host */
        {
            mse_send_in_report(s_mstate.button1_state, data_x, data_y);
            TRACE(("Mouse rem [%d] [%d,%d]\t[%d,%d]\r\n", in_progress ,remain_x, remain_y, data_x, data_y));
        }
        R_OS_TaskSleep(10);
    }
    while(true == in_progress);
    R_OS_TaskSleep(10);
    mse_send_in_report(s_mstate.button1_state, data_x, data_y);
}

/**
 * @brief Function to move mouse X position
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdMoveMouseX(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_OK;
    int_t new_x = 0;

    if (iArgCount < 2)
    {
        fprintf(pCom->p_out, "Need one argument\r\n");
    }
    else
    {
        /* Get location from command line */
        sscanf(ppszArgument[1], "%d", &new_x);
        printf("Argument: %d \r\n", new_x);

        reset_relpos();
        s_mstate.relative_x = new_x;
        update_msepos();
    }

    return ret;
}
/*****************************************************************************
End of function  cmdMoveMouseX
******************************************************************************/

/**
 * @brief Function to move mouse Y position.
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdMoveMouseY(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_OK;
    int_t new_y = 0;

    if (iArgCount < 2)
    {
        fprintf(pCom->p_out, "Need one argument\r\n");
    }
    else
    {

        sscanf(ppszArgument[1], "%d", &new_y);
        printf("Argument: %d \r\n", new_y);

        reset_relpos();
        s_mstate.relative_y = new_y;
        update_msepos();
    }
    return ret;
}
/*****************************************************************************
End of function  cmdMoveMouseY
******************************************************************************/

/**
 * @brief Function to move mouse X & Y position.
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdMoveMouseXY(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_OK;
    int_t new_x = 0;
    int_t new_y = 0;

    if (iArgCount < 3)
    {
        fprintf(pCom->p_out, "Need two arguments\r\n");
    }
    else
    {
        sscanf(ppszArgument[1], "%d", &new_x);
        sscanf(ppszArgument[2], "%d", &new_y);
        printf("Argument: %d, %d\r\n", new_x, new_y);

        reset_relpos();
        s_mstate.relative_x = new_x;
        s_mstate.relative_y = new_y;
        update_msepos();
    }

    return ret;
}
/*****************************************************************************
End of function  cmdMoveMouseXY
******************************************************************************/

/**
 * @brief Function to press mouse button 1.
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdToggleButton1(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_OK;

    /*Unused Argument*/
    (void) iArgCount;
    (void) ppszArgument;
    (void) pCom;

    reset_relpos();
    s_mstate.button1_state  = !s_mstate.button1_state;
    update_msepos();

    return ret;
}
/*****************************************************************************
End of function  cmdToggleButton1
******************************************************************************/

/**
 * @brief Function to press mouse button 2.
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdToggleButton2(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_OK;

    /*Unused Argument*/
    (void) iArgCount;
    (void) pCom;
    (void) ppszArgument;

    return ret;
}
/*****************************************************************************
End of function  cmdToggleButton2
******************************************************************************/

/**
 * @brief Function to quit usb mouse application.
 * @param iArgCount
 * @param ppszArgument
 * @param pCom
 * @return
 */
static int16_t cmdMouseLogout(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int16_t ret = CMD_LOG_OUT;

    /*Unusued Argument*/
    (void) iArgCount;
    (void) pCom;
    (void) ppszArgument;

    return ret;
}
/*****************************************************************************
End of function  cmdMouseLogout
******************************************************************************/
