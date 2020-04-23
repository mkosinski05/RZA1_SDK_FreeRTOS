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
 * File Name    : cmd_platform.c
 * Version      : 1.10
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The platform specific commands
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 17.08.2015 1.00 First Release
 *              : 11.02.2015 1.10 Updated to use new DMA API. Default size 0.5M
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

#include "compiler_settings.h"
#include "control.h"
#include "command.h"
#include "r_task_priority.h"
#include "r_stream_it_wdt_lld.h"
#include "r_timer.h"

#if R_OPTION_DISABLE
    #include "sysUsage.h"
#endif

#include "r_led_drv_api.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 External functions
 ******************************************************************************/

/******************************************************************************
 Local functions
 ******************************************************************************/
static int16_t cmd_memory_performance_test (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
static int16_t cmd_led (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
static int16_t cmd_restart (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
#if R_OPTION_DISABLE
static int16_t cmd_sri_get (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
#endif
static int16_t cmd_logout (int iArgCount, char **ppszArgument, pst_comset_t pCom);

extern void cmd_show_data_rate (FILE *pFile, float fTransferTime, size_t st_length);

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t gs_cmd_platform[] =
{
{ "mperf", cmd_memory_performance_test, "<CR> - Check the speed of writing to RAM", },
{ "led", cmd_led, "a s <CR> - Sets LED a state to On s = 1,\r\n"
        "               Off s = 0, Toggle s = ^", },
{ "restart", cmd_restart, "<CR> - Restart the system with a WDT reset", },
#if R_OPTION_DISABLE
        {
            "sriget",
            cmd_sri_get,
            "i<CR> - Get the system resource information i"
        },
#endif

};

static st_cmdfnass_t gs_cmd_platform_extended[] =
{
{ "mperf", cmd_memory_performance_test, "<CR> - Check the speed of writing to RAM", },
{ "led", cmd_led, "a s <CR> - Sets LED a state to On s = 1,\r\n"
        "               Off s = 0, Toggle s = ^", },
{ "restart", cmd_restart, "<CR> - Restart the system with a WDT reset", },
#if R_OPTION_DISABLE
        {
            "sriget",
            cmd_sri_get,
            "i<CR> - Get the system resource information i"
        },
#endif
        { "logout", cmd_logout, "<CR> - Exit a login shell", }

};


/*****************************************************************************
 External Variables
 ******************************************************************************/

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_platform =
{ "Platform Commands", gs_cmd_platform, ((sizeof(gs_cmd_platform)) / sizeof(st_cmdfnass_t)) };

const st_command_table_t g_cmd_tbl_platform_extended =
{ "Platform Commands", gs_cmd_platform_extended, ((sizeof(gs_cmd_platform_extended)) / sizeof(st_cmdfnass_t)) };


/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_memory_performance_test
 Description:
 Arguments:     IN  iArgCount - The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_memory_performance_test (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    size_t st_length = (1024 * 1024);
    uint8_t *p_bytest;

    /* Get user specified length */
    if (iArgCount > 1)
    {
        float f_test_size = 1.0f;

        /* Get the desired file size */
        sscanf(ppszArgument[1], "%f", &f_test_size);

        /* Calculate the test file size - always a multiple of four bytes */
        st_length = (size_t) (f_test_size * 1024.0f * 1024.0f);

        /* Limit to a maximum size */
        if (st_length > (size_t) (1024.0f * 1024.0f * 10.0f))
        {
            /* cast to size_t */
            st_length = (size_t) (1024.0f * 1024.0f * 10.0f);
        }

        /* Make a multiple of 4 bytes */
        st_length += (st_length % sizeof(uint32_t));
    }

    /* Allocate memory */
    p_bytest = (uint8_t *) R_OS_AllocMem(st_length, R_REGION_LARGE_CAPACITY_RAM);
    if (p_bytest)
    {
        float f_time;
        TMSTMP perf_timer;

        timerStartMeasurement( &perf_timer);
        memset(p_bytest, 0x00A5, st_length);
        f_time = timerStopMeasurement( &perf_timer);
        cmd_show_data_rate(pCom->p_out, f_time, st_length);

        fprintf(pCom->p_out, "mperf allocate memory for test @ addr 0x%08x\r\n", (unsigned int) p_bytest);

        R_OS_FreeMem(p_bytest);

        fflush(pCom->p_out);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to allocate memory for test\r\n");
        fflush(pCom->p_out);
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmd_memory_performance_test
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_led
 Description:   Command to switch the user LED on / off
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_led (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    uint16_t led = LED0;
    int_t led_handle = ( -1);

    led_handle = open( DEVICE_INDENTIFIER "led", O_RDWR);

    if (iArgCount > 1)
    {
        /* LED number */
        switch ( *ppszArgument[1])
        {
            case '0':
            {
                /* Operation on LED */
                switch ( *ppszArgument[2])
                {
                    case '0':
                    {
                        control(led_handle, CTL_SET_LED_OFF, &led);
                    }
                    break;

                    case '1':
                    {
                        control(led_handle, CTL_SET_LED_ON, &led);
                    }
                    break;

                    case '^':
                    {
                        control(led_handle, CTL_SET_LED_TOGGLE, &led);
                    }
                    break;

                    default:
                    {
                        fprintf(pCom->p_out, "Expected 1, 0 or ^\r\n");
                    }
                    break;
                }
            }
            break;

            default:
            {
                fprintf(pCom->p_out, "Unrecognised LED number\r\n");
            }
        }
    }

    close(led_handle);
    return CMD_OK;
}
/******************************************************************************
 End of function  cmd_led
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_restart
 Description:   Command to restart the system with a WDT reset
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_restart (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING
    ;
    int_t watchdog_handle;

    watchdog_handle = open(DEVICE_INDENTIFIER "wdt", O_RDWR);

    /* Call the watch dog timer reset - All timer controls require a
     parameter - even if it is not used */
    control(watchdog_handle, CTL_WDT_RESET, &ppszArgument);

    /* This code should never be executed */
    fprintf(pCom->p_out, "Restart failed!\r\n");

    return CMD_OK;
}
/******************************************************************************
 End of function  cmd_restart
 ******************************************************************************/
#if R_OPTION_DISABLE
/******************************************************************************
 Function Name: cmd_sri_get
 Description:   Command to get the system information
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_sri_get (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    char_t *pszBuffer;
    int32_t iType = 0;

    sscanf(ppszArgument[1], "%d", &iType);
    pszBuffer = sriGet(iType);
    if (pszBuffer)
    {
        fprintf(pCom->pOut, "%s", pszBuffer);
        free(pszBuffer);
    }
    else
    {
        fprintf(pCom->pOut, "sriGet failed\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sri_get
 ******************************************************************************/
#endif

/******************************************************************************
 Function Name: cmd_logout
 Description:   Command to log out of the console
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmd_logout (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    (void) ppszArgument;
    (void) iArgCount;

    int ctl_result;

    ctl_result = control(R_DEVLINK_FilePtrDescriptor(pCom->p_in), CTL_STREAM_TCP,
    NULL);

    /* Only log out of streams that require authentication */
    if (0 == ctl_result)
    {
        return CMD_LOG_OUT;
    }
    else
    {
        return CMD_OK;
    }
}
/******************************************************************************
 End of function cmd_logout
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/
