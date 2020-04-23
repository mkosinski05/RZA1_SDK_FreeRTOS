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
 * File Name    : cmd_core.c
 * Version      : 1.10
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The core console commands
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 23.04.2018 1.00 First Release
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

/* This needs tp be removed / and xTaskStatusType should be removed */
#include "FreeRTOS.h"
#include "FreeRTOSconfig.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
 External functions
 ******************************************************************************/

/******************************************************************************
 Local functions
 ******************************************************************************/

static int16_t cmd_help(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_last_command_line(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_do_last_command_line(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_sys_info(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_idle(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_system_usage(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_list_tasks(int iArgCount, char **ppszArgument, pst_comset_t pCom);

extern int16_t cmd_list_devlink_tbl_content(int iArgCount, char **ppszArgument, pst_comset_t pCom);
extern int16_t cmd_list_device_open_handles(int iArgCount, char **ppszArgument, pst_comset_t pCom);

/* Table that associates command letters, function pointer and a little
 description of what the command does */
static st_cmdfnass_t gs_cmd_core[] =
{
     {
          "sys",
          cmd_system_usage,
          "<CR> - Shows the system resource usage information",
     },
     {
          "task",
          cmd_list_tasks,
          "<CR> - List tasks",
     },
     {
         "help",
         cmd_help,
         "<CR> - Show the help screen ('?' also used)",
     },
     {
         "?",
         cmd_help,
         NULL,
     },
     {
         /* Commands for doskey emulation of function keys */
         "[11",                          /* F1 - Print help strings */
         cmd_help,
         NULL,
     },
     {
         "OP",
         cmd_help,
         NULL,
     },
     {
         "op",
         cmd_help,
         NULL,
     },
     {
         "[12",                          /* F2 - get last command line */
         cmd_last_command_line,
         NULL,
     },
     {
         "OQ",
         cmd_last_command_line,
         NULL,
     },
     {
         "oq",
         cmd_last_command_line,
         NULL,
     },
     {
         "[13",                          /* F3 - do last command */
         cmd_do_last_command_line,
         NULL,
     },
     {
         "OR",
         cmd_do_last_command_line,
         NULL,
     },
     {
         "or",
         cmd_do_last_command_line,
         NULL,
     },
     {
         "[14",                          /* F4 - System info */
         cmd_sys_info,
         NULL,
     },
     {
         "OS",
         cmd_sys_info,
         NULL,
     },
     {
         "os",
         cmd_sys_info,
         NULL,
     },
     {
         "[A",                           /* Up arrow */
         cmd_idle,
         NULL,
     },
     {
         "[B",                           /* Down arrow */
         cmd_idle,
         NULL,
     },
     {
         "[C",                           /* Right arrow */
         cmd_idle,
         NULL,
     },
     {
         "[D",                           /* Left arrow */
         cmd_idle,
         NULL
     }
};

/*****************************************************************************
 External Variables
 ******************************************************************************/

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_core =
{
    "",
    gs_cmd_core,
    ((sizeof(gs_cmd_core)) / sizeof(st_cmdfnass_t))
};


/******************************************************************************
 Private Functions
 ******************************************************************************/


static void printformattedint (int32_t n, pst_comset_t pCom)
{
    if (n < 0)
    {
        fprintf (pCom->p_out, "-");
        printformattedint (-n, pCom);
        return;
    }
    if (n < 1000)
    {
        fprintf (pCom->p_out, "%d", (int)n);
        return;
    }
    printformattedint (n/1000, pCom);

    fprintf (pCom->p_out, ",%03d", (int)n%1000);
}


/******************************************************************************
 Function Name: cmd_system_usage
 Description:   Function to show the heap and event usage information
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmd_system_usage(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;


    fprintf(pCom->p_out,"\r\nMemory Usage Summary\r\n");
    fprintf(pCom->p_out,"Current Memory Available = ");
    printformattedint((int32_t)xPortGetFreeHeapSize(), pCom);
    fprintf(pCom->p_out," Bytes\r\n");

    fprintf(pCom->p_out,"Lowest Every Free Memory = ");
    printformattedint((int32_t)xPortGetMinimumEverFreeHeapSize(), pCom);
    fprintf(pCom->p_out," Bytes\r\n");


#if R_OPTION_DISABLE // todo rc
    SYSUSAGE    sysUsage;
    HEAPUSAGE   memUsage;
    MEMTYPE     memType;

    /* For each heap */
    for (memType = 0; memType < NUMBER_OF_HEAPS; memType++)
    {
        /* Get the memory usage information */
        heapUsage(&memUsage, memType);

        /* Print the heap number */
        fprintf(pCom->pOut, "Heap %d: ", memType);
        fprintf(pCom->pOut, "Name %s:\r\n", memUsage.chName);

        /* Print the heap usage */
        fprintf(pCom->pOut, "    Free ");
        cmdShowMemorySize(pCom->pOut, memUsage.stTotalFree);
        fprintf(pCom->pOut, ", Used ");
        cmdShowMemorySize(pCom->pOut, memUsage.stTotalAllocated);
        fprintf(pCom->pOut, ", Total ");
        cmdShowMemorySize(pCom->pOut, memUsage.stTotalHeap);
        fprintf(pCom->pOut, "\r\n    Largest free segment ");
        cmdShowMemorySize(pCom->pOut, memUsage.stLargestFreeSegment);
        fprintf(pCom->pOut, ", Overhead ");
        cmdShowMemorySize(pCom->pOut, memUsage.stOverhead);
        fprintf(pCom->pOut, "\r\n");

        /* Print the fragmentation */
        fprintf(pCom->pOut, "    Fragmentation %.3f%%\r\n",
                (float)((1.0f - ((float)memUsage.stLargestFreeSegment /
                                 (float)memUsage.stTotalFree)) * 100));
    }

    /* Get the system usage information */
    systemUsage(&sysUsage);

    /* Print the results */
    fprintf(pCom->pOut, "System:\r\n    Files %lu/%lu\r\n"
            "    Events %lu/%lu\r\n    Timers %lu/%lu\r\n",
            sysUsage.stFilesInUse,
            sysUsage.stFileTotal,
            sysUsage.stEventsInUse,
            sysUsage.stEventTotal,
            sysUsage.stTimersInUse,
            sysUsage.stTimerTotal);
#endif /* if R_OPTION_DISABLE */

    return CMD_OK;
}
/******************************************************************************
End of function cmd_system_usage
******************************************************************************/


/*****************************************************************************
 Function Name: cmd_list_tasks
 Description:   Command to list all the running tasks
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 *****************************************************************************/
static int16_t cmd_list_tasks(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    /* set p_task_status_array to NULL */
    xTaskStatusType *p_task_status_array = NULL;
    uint16_t ux_array_size = 0;
    uint16_t i;
    uint16_t j;
    uint16_t id;

    static const char *p_status_name[] =
    {
        [eRunning]   = "Running",
        [eReady]     = "Ready",
        [eBlocked]   = "Blocked",
        [eSuspended] = "Suspended",
        [eDeleted]   = "Deleted",
        [eInvalid]   = "Invalid"
    };

    do
    {
        if (ux_array_size > 0)
        {
            R_OS_FreeMem(p_task_status_array);
        }

        ux_array_size = (uint16_t)uxTaskGetNumberOfTasks();

        if (ux_array_size > 0)
        {
            p_task_status_array = R_OS_AllocMem(ux_array_size * sizeof(xTaskStatusType), R_REGION_LARGE_CAPACITY_RAM);
        }

        /* check for null pointer */
        if (NULL == p_task_status_array)
        {
            printf("Cannot get memory for threads list\r\n");
            return CMD_OK;
        }

        /* pass null pointer for total run time */
        ux_array_size = (uint16_t)uxTaskGetSystemState(p_task_status_array, ux_array_size, NULL);
    }
    while (0 == ux_array_size);

    fprintf(pCom->p_out,"Found %d threads:\r\n", ux_array_size);
    fprintf(pCom->p_out,"%3s %20s %9s %2s %5s %9s %9s\r\n", "id", "Name", "State", "Pr", "Stack", "Stack Base", "Cycles");
    fprintf(pCom->p_out,"=== ==================== ========= == ===== ========== =========\r\n");

    for (i = 0, id = 0; i < ux_array_size; id++)
    {
        for (j = 0; j < ux_array_size; j++)
        {
            xTaskStatusType * p_xt = p_task_status_array + j;

            if (p_xt->xTaskNumber == id)
            {
                /* Display to user */
                fprintf(pCom->p_out," %2lu %20s %9s %2lu %5u %9p %9lu\r\n",
                    p_xt->xTaskNumber, p_xt->pcTaskName, p_status_name[p_xt->eCurrentState],
                    p_xt->uxCurrentPriority, p_xt->usStackHighWaterMark, p_xt->pxStackBase, p_xt->ulRunTimeCounter);

                i++;
            }
        }
    }

    if (ux_array_size > 0)
    {
        R_OS_FreeMem(p_task_status_array);
    }

    return CMD_OK;
}
/*****************************************************************************
End of function cmd_list_tasks
******************************************************************************/


/******************************************************************************
 Function Name: cmd_help
 Description:   Command to show the command help strings
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_help(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;
    int16_t table;

    fprintf(pCom->p_out, "The commands available are:\r\n"
           "F1 or ? <CR> - print the command help strings\r\n"
           "F2 - brings up the last command line\r\n"
           "F3 - repeats the last command\r\n");

    for (table = 0; table < pCom->num_tables; table++)
    {
        int16_t command = 0;
        pst_cmdfnass_t pcommand = pCom->p_function[table]->command_list;

        if (0 != sizeof(pCom->p_function[table]->group_name))
        {
            fprintf(pCom->p_out, "\r\n%s\r\n", pCom->p_function[table]->group_name);
        }
        else
        {
            fprintf(pCom->p_out, "\r\n");
        }

        while (command < (signed)pCom->p_function[table]->number_of_commands)
        {
            /* Only print out if there is a string */
            if (pcommand->p_command_description)
            {
                fprintf(pCom->p_out, "%s %s\r\n", pcommand->p_command, pcommand->p_command_description);
            }

            fflush(pCom->p_out);
            R_OS_Yield();

            pcommand++;
            command++;
        }
    }

#if (R_SELF_PERFORM_SELF_TEST == R_OPTION_ENABLE)
    R_SELF_RunViaConsoleHelp(pCom);
#endif

    return CMD_OK;
}
/******************************************************************************
End of function cmd_help
******************************************************************************/

/******************************************************************************
 Function Name: cmd_last_command_line
 Description:   Command to bring up the last typed command
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_NO_PROMPT
 ******************************************************************************/
int16_t cmd_last_command_line(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    con_get_last_command_line(pCom);
    return CMD_NO_PROMPT;
}
/******************************************************************************
End of function cmd_last_command_line
******************************************************************************/

/******************************************************************************
 Function Name: cmd_do_last_command_line
 Description:   Command to bring up the last typed command
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmd_do_last_command_line(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    return con_do_last_command_line(pCom);
}
/******************************************************************************
End of function cmd_do_last_command_line
******************************************************************************/

extern void R_INTC_Display_TaskTable(FILE *p_out);

/******************************************************************************
 Function Name: cmd_sys_info
 Description:   Command to show the system information
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmd_sys_info(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{

    fprintf(pCom->p_out, "\r\n");
    cmd_list_devlink_tbl_content(iArgCount, ppszArgument, pCom);

    fprintf(pCom->p_out, "\r\n");
    cmd_list_device_open_handles(iArgCount, ppszArgument, pCom);


    fprintf(pCom->p_out, "\r\n");
    cmd_list_tasks(iArgCount, ppszArgument, pCom);
    fprintf(pCom->p_out, "\r\n");
    cmd_system_usage(iArgCount, ppszArgument, pCom);

    R_INTC_Display_TaskTable(pCom->p_out);

    return CMD_OK;
}
/******************************************************************************
End of function cmd_sys_info
******************************************************************************/

/******************************************************************************
 Function Name: cmd_idle
 Description:   Command to do nothing
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmd_idle(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    fprintf(pCom->p_out, "\r\n");

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_idle
 ******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
