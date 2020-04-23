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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_switch_monitor.c
 * Version      : 1.00
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * OS           : None
 * H/W Platform : Stream it! v2 board
 * Description  : Monitors the user switch
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 11.06.2018 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "r_switch_monitor.h"
#include "r_switch_driver.h"

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

extern char gs_default_prompt[];

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static event_t gs_switch_event;
static FILE *gsp_output;

static void switch_pressed(void);

/***********************************************************************************************************************
 * Function Name: switch_task
 * Description  : Switch monitoring task
 * Arguments    : void *parameters
 * Return Value : none
 ***********************************************************************************************************************/
static void switch_task (void *parameters)
{
    UNUSED_PARAM(parameters);

    R_OS_CreateEvent(&gs_switch_event);

    /* endless loop */
    while (1)
    {
        R_OS_WaitForEvent(&gs_switch_event, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
        printf("USER switch was pushed!!\r\n%s ", gs_default_prompt);
        fflush(gsp_output);
    }
}
/***********************************************************************************************************************
 End of function switch_task
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: switch_pressed
 * Description  : Switch pressed callback - this is called from an interrupt routine
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
static void switch_pressed (void)
{
    /* notify the switch task that the switch has been pressed */
    R_OS_SetEvent(&gs_switch_event);
}
/***********************************************************************************************************************
 End of function switch_pressed
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: initialise_switch_monitor_task
 * Description  : Initialises the switch driver and creates the switch monitor task
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void initialise_switch_monitor_task (FILE *p_out)
{
    os_task_t *p_os_task;

    gsp_output = p_out;

    R_SWITCH_Init(true);
    R_SWITCH_SetPressCallback(switch_pressed);

    /* Create a task to monitor the switch */
    p_os_task = R_OS_CreateTask("Switch", switch_task, NULL, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE, TASK_SWITCH_TASK_PRI);

    /* NULL signifies that no task was created by R_OS_CreateTask */
    if (NULL == p_os_task)
    {
        /* Debug message */
    }
}
/***********************************************************************************************************************
 End of function initialise_switch_monitor_task
 ***********************************************************************************************************************/
