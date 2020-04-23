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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : cmd_tcp_shell.c
* Version      : 1.00
* Device(s)    : Renesas
* Tool-Chain   : N/A
* OS           : N/A
* H/W Platform : RSK+
* Description  : TCP command shell but without any telnet protocol
******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 04.02.2010 1.00 First Release
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compiler_settings.h"

#include "command.h"
#include "socket.h"
#include "cmd_tcp_shell.h"
#include "lwIP_Interface.h"
#include "r_task_priority.h"
#include "console.h"

#include "trace.h"

/*****************************************************************************
Function Macros
******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/*****************************************************************************
Enumerated Types
******************************************************************************/

typedef enum _SHSIG
{
    TCSH_LINK_STATUS_CHANGE = 0,
    TCSH_TASK_STARTED,
    TCSH_NUM_LINK_EVENTS
} SHSIG;

/*****************************************************************************
Typedefs
******************************************************************************/

typedef struct _TCSH
{
    /* The file descriptor of the master socket */
    int         iSocket;
    /* The socket used for IO */
    FILE        *pSocket;
    /* The desired port number */
    uint16_t    usPortNumber;
    /* The client IP and port */
    SOCKADDR_IN clientIP;
    /* The server (us) IP and port */
    SOCKADDR_IN serverIP;
    /* The link status change events */
    PEVENT      ppEventList[TCSH_NUM_LINK_EVENTS];
    /* The ID of the master socket montor task */
    uint32_t    uiSocketMonTaskID;
    /* The name of the task */
    int8_t      pszTaskName[R_OS_ABSTRACTION_PRV_MAX_TASK_NAME_SIZE];
} TCSH;

/*****************************************************************************
Function Prototypes
******************************************************************************/

static void tcpShellSockMon(PTCSH pShell);

/*****************************************************************************
Public Functions
******************************************************************************/

/*****************************************************************************
Function Name: tcpCmdShellStart
Description:   Function to start a TCP command shell
Arguments:     IN  usPortNumber - The port number to listen on
Return value:  Pointer to the TCP command shell object or null on error
*****************************************************************************/
PTCSH tcpCmdShellStart(uint16_t usPortNumber)
{
    PTCSH   pShell = R_OS_AllocMem(sizeof(TCSH), R_REGION_LARGE_CAPACITY_RAM);
    if (pShell)
    {
        SHSIG  shellSignals;
        memset(pShell, 0, sizeof(TCSH));

        /* Create the events */
        shellSignals = (SHSIG)eventCreate(pShell->ppEventList,
                                          TCSH_NUM_LINK_EVENTS);
        if (!shellSignals)
        {
            /* Register the link status change event */
            if (ipAddLinkMonitorEvent(pShell->
                                      ppEventList[TCSH_LINK_STATUS_CHANGE]))
            {
                /* Set the default port number */
                pShell->usPortNumber = usPortNumber;
                pShell->iSocket = -1;

                /* No bounds checking and change in TC_MAX_TASK_NAME_SIZE made
                 * this overrun the end of the allocated memory.
                 */
                snprintf((char *) pShell->pszTaskName,
                         R_OS_ABSTRACTION_PRV_MAX_TASK_NAME_SIZE,
                         "Telnet %*u",
                         5, usPortNumber);

                pShell->uiSocketMonTaskID =  *(uint32_t*)R_OS_CreateTask ((char *) pShell->pszTaskName, (os_task_code_t)tcpShellSockMon, pShell, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE, TASK_TELNET_MON_PRI);

                if (pShell->uiSocketMonTaskID != 0)
                {
                    return pShell;
                }
            }
        }
        else
        {
            /* Destroy any events that were created */
            eventDestroy(pShell->ppEventList,
                         (uint32_t)(TCSH_NUM_LINK_EVENTS - shellSignals));
        }
        R_OS_FreeMem(pShell);
    }
    return NULL;
}
/*****************************************************************************
End of function  tcpCmdShellStart
******************************************************************************/

/*****************************************************************************
Function Name: tcpCmdShellStop
Description:   Function to stop the TCP command shell
Arguments:     IN  pShell - Pointer to the shell object to stop
Return value:  non
*****************************************************************************/
void tcpCmdShellStop(PTCSH pShell)
{
    R_OS_DeleteTask (&pShell->uiSocketMonTaskID);

    ipRemoveLinkMonitorEvent(pShell->ppEventList[TCSH_LINK_STATUS_CHANGE]);
    if (pShell->iSocket >= 0)
    {
        close(pShell->iSocket);
    }
    R_OS_FreeMem(pShell);
}
/*****************************************************************************
End of function  tcpCmdShellStop
******************************************************************************/

/*****************************************************************************
Private Functions
******************************************************************************/

/*****************************************************************************
Function Name: tcpShellTimeOut
Description:   Function called back by the driver when the idle time-out has
               expired. 
Arguments:     IN  uiTaskID - The ID of the task passed in the IDLECB struct
               IN  pvParameter - The parameter passed  the IDLECB struct
Return value:  none
*****************************************************************************/
void tcpShellTimeOut(uint32_t uiTaskID, void* pvParameter)
{
    FILE *pSocket = (FILE *)pvParameter;
    TRACE(("tcpShellTimeOut: File stream 0x%p Task %u\r\n", pSocket, uiTaskID));
    /* The driver has called this function because the stream has exceeded the
       idle time period. Give the task some time to finish what it is doing
       and close the socket */
    R_OS_TaskSleep(1000UL);

    /* Check to see if the task is still running */
    if (R_OS_GetTaskState((char*)uiTaskID) != NULL)
    {
        int     iSocket = R_DEVLINK_FilePtrDescriptor(pSocket);

        /* Kill the task */
        TRACE(("tcpShellTimeOut: Destroying Task %u\r\n", uiTaskID));

        if ((-1) != R_OS_TaskGetPriority(uiTaskID))
        {
            R_OS_DeleteTask(&uiTaskID);

            TRACE(("Task was alive, cleaning up the mess\r\n"));

            /* If the task is still alive then it will not have closed
               the file */
            TRACE(("tcpShellTimeOut: Closing file %p\r\n", pSocket));

            fclose(pSocket);

            TRACE(("tcpShellTimeOut: Closing descriptor %u\r\n", iSocket));
            close(iSocket);
        }
    }
}
/*****************************************************************************
End of function  tcpShellTimeOut
******************************************************************************/

/*****************************************************************************
Function Name: tcpCommandShell
Description:   Task to perform the TCP command shell
Arguments:     IN  pShell - Pointer to shell management data
Return value:  none
*****************************************************************************/
static void tcpCommandShell(PTCSH pShell)
{
    R_OS_TaskUsesFloatingPoint();

    /* Take a copy of the socket to the client - this one is just wrapped
       by a file stream so it can be used with the console code */
    FILE *pSocket = pShell->pSocket;
    TRACE(("tcpCommandShell: File stream 0x%p Task ID %d\r\n", pSocket, R_OS_GetCurrentTask()));

    /* Let the task know that we have the socket info */
    eventSet(pShell->ppEventList[TCSH_TASK_STARTED]);

    /* Set an idle time-out call-back to close the console when the user has
       lost interest */
    {
        IDLECB  idleCallBack;
        int     iSocket = R_DEVLINK_FilePtrDescriptor(pSocket);

        /* Set the idle call back information */
        idleCallBack.pCallBack   = tcpShellTimeOut;
        idleCallBack.uiTaskID    = *(uint32_t *)R_OS_GetCurrentTask();
        idleCallBack.pvParameter = pSocket;

        /* Set the socket idle time-out call back function */
        control(iSocket,
                CTL_STREAM_SET_CONNECTION_IDLE_CALL_BACK,
                &idleCallBack);
    }

    /* Do the command console using the socket for input and output */
    show_welcome_msg(pSocket, true);

    cmd_console(pSocket, pSocket, "TCP>");

    /* Close the socket */
    fclose(pSocket);

    R_OS_TaskSleep(1100);

    TRACE(("tcpCommandShell: Exit 0x%p\r\n", pSocket));
    TRACE(("tcpCommandShell: File stream 0x%p Task ID %d\r\n", pSocket, R_OS_GetCurrentTask()));

    R_OS_DeleteTask(NULL);
}
/*****************************************************************************
End of function  tcpCommandShell
******************************************************************************/

/*****************************************************************************
Function Name: tcpShellAcceptClient
Description:   Function to accept a connection
Arguments:     IN  pShell - Pointer to shell management data
Return value:  0 for success and -1 on error
*****************************************************************************/
static int tcpShellAcceptClient(PTCSH pShell)
{
    socklen_t   clientIPLength = sizeof(SOCKADDR_IN);

    /* Special version of accept function that returns a file pointer
       for use with the console functions */
    pShell->pSocket = faccept(pShell->iSocket,
                              (PSOCKADDR)&pShell->clientIP,
                              &clientIPLength);
    if (pShell->pSocket)
    {
        os_task_t * ptask = NULL;

        eventReset(pShell->ppEventList[TCSH_TASK_STARTED]);

        /* Create a task to do the command shell */
        ptask = R_OS_CreateTask ("TCP/IP CmdShell", (os_task_code_t)tcpCommandShell, pShell, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE, TASK_TCP_IP_CONSOLE_PRI);
        if( NULL == ptask)
        {
            TRACE(("tcpShellAcceptClient: Failed to create console task\r\n"));
            return -1;
        }

        /* The task tcpCommandShell will set this event when it has
           taken a copy of the master socket and file stream */
        eventWait(&pShell->ppEventList[TCSH_TASK_STARTED], 1, true);
        pShell->pSocket = NULL;
        return 0;
    }
    else
    {
        TRACE(("tcpShellAcceptClient: Failed to accept socket file\r\n"));
        return -1;
    }
}
/*****************************************************************************
End of function  tcpShellAcceptClient
******************************************************************************/

/*****************************************************************************
Function Name: tcpShellSockMon
Description:   Function to monitor the link status and manage the socket
Arguments:     IN  pShell - Pointer to shell management data
Return value:  none
*****************************************************************************/
static void tcpShellSockMon(PTCSH pShell)
{
    R_OS_TaskUsesFloatingPoint();

    while (true)
    {
        /* Check for an IP link */
        while (ipLink())
        {
            /* Check to see if the socket needs opening */
            if (pShell->iSocket < 0)
            {
                /* Open a TCP socket */
                int iSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (iSocket >= 0)
                {
                    /* Set the socket for our port number and any IP address */
                    pShell->serverIP.sin_family = AF_INET;
                    pShell->serverIP.sin_addr.s_addr = INADDR_ANY;
                    pShell->serverIP.sin_port = htons(pShell->usPortNumber);
                    if (bind(iSocket,
                             (PSOCKADDR)&pShell->serverIP,
                             sizeof(SOCKADDR_IN)) == 0)
                    {
                        pShell->iSocket = iSocket;
                        /* Listen for connections */
                        if (!listen(iSocket, 1))
                        {
                            /* Accept client connections */
                            while (true)
                            {
                                if (tcpShellAcceptClient(pShell))
                                {
                                    break;
                                }
                            }
                            close(iSocket);
                        }
                    }
                    else
                    {
                        TRACE(("tcpShellSockMon: Failed to bind socket\r\n"));
                        close(iSocket);
                    }
                }
                else
                {
                    TRACE(("tcpShellSockMon: Failed to open socket\r\n"));
                    R_OS_TaskSleep(100UL);
                }
            }
        }
        /* Wait here for a link status change signal */
        eventWait(&pShell->ppEventList[TCSH_LINK_STATUS_CHANGE], 1, true);
    }
}
/*****************************************************************************
End of function  tcpShellSockMon
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
