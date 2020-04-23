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
 * File Name    : cmdUSB.c
 * Version      : 1.0
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The USB specific commands
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
#include <ctype.h>
#include <fcntl.h>

/******************************************************************************
User Includes
******************************************************************************/

#include "trace.h"
#include "command.h"
#include "console.h"
#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

bool_t g_usbkbd = false;
static os_task_t *ps_os_usb_kbdid;
static os_task_t *ps_os_main_id;

/******************************************************************************
 External functions
 ******************************************************************************/

extern void cmdShowDataRate(FILE *pFile, float fTransferTime, size_t stLength);

/*****************************************************************************
 External Variables
 ******************************************************************************/

extern const st_command_table_t g_cmd_tbl_platform_extended;

extern const st_command_table_t g_cmd_tbl_platform;

/******************************************************************************
 Private Functions
 ******************************************************************************/
static void cmdUSBConsole(FILE* p_in, FILE *p_out, char *ps_prompt);

/******************************************************************************
Function Name: cmdUSBConsole
Description:   Function to process the console commands for USB keyboard
Arguments:     IN  p_in - Pointer to the input file stream
               IN  p_out - Pointer to the file stream for echo of command input
               IN  ps_prompt - Pointer to a command prompt
Return value:  none
******************************************************************************/
static void cmdUSBConsole(FILE* p_in, FILE *p_out, char *ps_prompt)
{
    (void) ps_prompt;

    pst_comset_t pCom = R_OS_AllocMem(sizeof(st_comset_t), R_REGION_LARGE_CAPACITY_RAM);

    if (pCom)
    {
        st_comset_t    * pCmdSet;

        pCmdSet = R_OS_AllocMem(sizeof(st_comset_t), R_REGION_LARGE_CAPACITY_RAM);

        /* Print a welcome message */
        fprintf(p_out,
            "WebEngine USB console\r\n"
            "Type 'logout' to exit console\r\n");
        fflush(p_out);

        /* Initialise the console for login */
        memset(pCom, 0, sizeof(st_comset_t));
        pCom->p_in = p_in;
        pCom->p_out = p_out;

        /* add "logout" command */
        gppCommands[1] = (pst_command_table_t) &g_cmd_tbl_platform_extended;

        console(pCmdSet,
                (cpst_command_table_t *) gppCommands,
                g_num_commands,
                pCom->p_in,
                pCom->p_out,
                "USB>");

        gppCommands[1] = (pst_command_table_t) &g_cmd_tbl_platform;

        /* Do "exit" code */
        R_OS_FreeMem(pCom);
        R_OS_FreeMem(pCmdSet);
        g_usbkbd = false;
    }
    else
    {
        g_usbkbd = false;

        /* Print error message */
        fprintf(p_out, "Can not create WebEngine USB console\r\n");
        fflush(p_out);
    }
}
/******************************************************************************
End of function  cmdUSBConsole
******************************************************************************/


/*****************************************************************************
* Function Name: usb_shell_task
* Description:   Task to perform a control console
* Arguments:     IN  pIoStr - Pointer to the IO streams to use
* Return value:  none
*****************************************************************************/
static void usb_shell_task (pst_iostr_t p_iostr)
{
    R_OS_SuspendTask(ps_os_main_id);

    /* Do the console function */
    cmdUSBConsole(p_iostr->p_in, p_iostr->p_out, "USB>");

    /* resume main task */
    R_OS_ResumeTask(ps_os_main_id);

    /* delete this task */
    R_OS_DeleteTask(NULL);
}
/******************************************************************************
End of function  usb_shell_task
******************************************************************************/

/******************************************************************************
Function Name: cmdConsoleUSB
Description:   Command to create the USB console,
               NOTE waits for console to complete before exiting.
Arguments:     IN  iArgCount The number of arguments in the argument list
               IN  ppszArgument - The argument list
               IN  pCom - Pointer to the command object
Return value:  0 for success otherwise error code
******************************************************************************/
static int cmdConsoleUSB(int iArgCount, int8_t **ppszArgument, pst_comset_t p_com)
{
    (void) iArgCount;
    (void) ppszArgument;

    st_iostr_t iostr;

    if (control(R_DEVLINK_FilePtrDescriptor(p_com->p_in), CTL_STREAM_TCP, NULL) == 0)
    {
        /* Print a error message */
        fprintf(p_com->p_out,"Unable to execute command:\r\n The 'usbk' command can only be launched from the serial console keyboard.\r\n");
        fflush(p_com->p_out);
    }
    else
    {
        ps_os_main_id = R_OS_GetCurrentTask();
        {
            /* Perform a test open directly to the USB Keyboard, to ascertain if one is
             * connected         */
            int iStdIn = open(DEVICE_INDENTIFIER "HID Keyboard",
            O_RDWR,
            _IONBF);

            if ((-1) != iStdIn)
            {
                /* Keyboard is available */
                close (iStdIn);    /* Close it, it'll be re-opened as part of the KBD task*/

                /* Assign the keyboard stream for the usb console */
                iostr.p_in = fopen(DEVICE_INDENTIFIER "usb_std_in", "r");
                iostr.p_out = p_com->p_out;

                /* Create a task to run the command shell */
                 ps_os_usb_kbdid = R_OS_CreateTask("USB Console", (os_task_code_t) usb_shell_task, (void *) &iostr, R_OS_ABSTRACTION_PRV_SMALL_STACK_SIZE, TASK_USB_CONSOLE_PRI);
                R_OS_TaskSleep(100);
            }
            else
            {
                fprintf(p_com->p_out, "Failed to open USB Keyboard.\r\n");
                return CMD_OK;
            }
        }

        /* Close the keyboard stream */
        fclose(iostr.p_in);

        fprintf(p_com->p_out, "Returning to serial console\r\n\r\nBye!\r\n");
        fflush(p_com->p_out);
    }

    return CMD_OK;
}
/******************************************************************************
End of function  cmdConsoleUSB
******************************************************************************/

/******************************************************************************
 Function Name: cmdMonitorMouse
 Description:   Command to monitor the data from a mouse
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int cmdMonitorMouse(int iArgCount, int8_t **ppszArgument, pst_comset_t p_com)
{
    (void) iArgCount;
    (void) ppszArgument;

    int iMse;

    /* Abort if a pure TCP console is being used, but allow if a USB console */
    if ((control(R_DEVLINK_FilePtrDescriptor(p_com->p_in),CTL_STREAM_TCP,NULL) == 0) &&
        (control(R_DEVLINK_FilePtrDescriptor(p_com->p_in),CTL_HID_KEYBOARD,NULL) != 0))
    {
        /* Print a error TCP message */
        fprintf(p_com->p_out,"USB Mouse can not be used via TCP\r\n");
        fflush(p_com->p_out);
    }
    else /* Running on serial Console */
    {
        iMse = open(DEVICE_INDENTIFIER "HID Mouse", O_RDWR, _IONBF);
        if ((-1) != iMse)
        {
            bool_t b_error = false;
            int_t iResult = 0;
            int_t iX = 0;
            int_t iY = 0;
            int_t iS = 0;
            int_t iIn = R_DEVLINK_FilePtrDescriptor(p_com->p_in);

            fprintf(p_com->p_out, "Mouse monitor, press any key to stop.\r\n"
                    "Left Middle Right        X        Y        S\r\n"
                    "0    0      0     +00000000 +00000000 +00000000");
            fflush(p_com->p_out);
            while ((control(iIn, CTL_GET_RX_BUFFER_COUNT, NULL) == 0) && (false == b_error))
            {
                MDAT mouseData;
                iResult = control(iMse, CTL_GET_MOUSE_DATA, &mouseData);

                /* If there is new mouse data */
                if ((1) == iResult)
                {
                    iX += mouseData.Part.deltaX;
                    iY += mouseData.Part.deltaY;
                    iS += mouseData.Part.deltaScroll;
                    fprintf(p_com->p_out, "\r%.1d    %.1d      %.1d"
                                        "     %+.8d %+.8d %+.8d",
                           (int) mouseData.Part.Buttons.Field.Left,
                           (int) mouseData.Part.Buttons.Field.Middle,
                           (int) mouseData.Part.Buttons.Field.Right,
                           iX, iY, iS);
                    fflush(p_com->p_out);
                }
                else if ((-1) == iResult)
                {
                    b_error = true;
                }
            } /* end of while loop */

            if ((-1) != iResult)
            {
                /* Throw away the key press to stop */
                fgetc(p_com->p_in);
            }

            close(iMse);
            fprintf(p_com->p_out, "\r\n");
        }
        else
        {
            fprintf(p_com->p_out, "No mouse found\r\n");
        }
    }

    return CMD_OK;
}
/******************************************************************************
End of function  cmdMonitorMouse
******************************************************************************/


#if 0

/******************************************************************************
Function Name: cmdBatch
Description:   Command to monitor the data from a mouse
Arguments:     IN  iArgCount The number of arguments in the argument list
               IN  ppszArgument - The argument list
               IN  pCom - Pointer to the command object
Return value:  CMD_OK for success
******************************************************************************/
static int cmdBatch(int iArgCount, char **ppszArgument, PCOMSET pCom)
{
    FILE *pFile;

    /* Look for a command and one parameter only */
    if ((2) == iArgCount)
    {
        /* Attempt to open the input file */
        pFile = fopen(ppszArgument[1], "r");
        if (pFile)
        {
            /* Create the console to run the commands */
            COMSET * pCmdSet;
            pCmdSet = malloc(sizeof(COMSET));
            if (pCmdSet)
            {
                Console(pCmdSet,
                        gppCommands,
                        giNumCommands,
                        pFile,
                        pCom->pOut,
                        "BAT> ");

                /* Release the memory when completed */
                free(pCmdSet);
            }
            else
            {
                fprintf(pCom->pOut, "Failed to allocate memory\r\n");
            }

            /* Make sure to close the opened file */
            fclose(pFile);
        }
        else
        {
            fprintf(pCom->pOut, "Cannot open file\r\n");
        }
    }
    else
    {
        int iTable = 0;

        /* Find the help string and print it */
        while (iTable < pCom->iNumTables)
        {
            int iCommand = 0;
            PCMDFNASS pCommand = pCom->ppFn[iTable]->pCommandList;
            while (iCommand < pCom->ppFn[iTable]->dwNumberOfCommands)
            {
                if ((pCommand->pFunction == cmdBatch)
                &&  (pCommand->pszCommandDescription))
                {
                    fprintf(pCom->pOut, "%s %s\r\n",
                           pCommand->pszCommand,
                           pCommand->pszCommandDescription);
                }
                pCommand++;
                iCommand++;
            }
            iTable++;
        }
    }
    return CMD_OK;
}

/******************************************************************************
 End of function  cmdBatch
 ******************************************************************************/

#endif

/* Table that associates command letters, function pointer and a little
 description of what the command does */
const st_cmdfnass_t gpcmdUSB[] =
{
    {
        "usbm",
        (const CMDFUNC) cmdMonitorMouse,
        "<CR> - Monitors the data from a mouse"
    },

    {
        "usbk",
        (const CMDFUNC) cmdConsoleUSB,
        "<CR> - Invokes USB console reading data from a USB keyboard"
    },
/*
    "batch",
    cmdBatch,
    "f<CR> where f is a fully qualified path.\r\n"
    "For Example:  a:\\file.ext\r\n"
    "n:<CR> - change working disk drive to \"n\" "
*/
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_usb =
{
    "USB Commands",
    (pst_cmdfnass_t) gpcmdUSB,
    (sizeof(gpcmdUSB) / sizeof(st_cmdfnass_t)),
};

/******************************************************************************
End  Of File
******************************************************************************/
