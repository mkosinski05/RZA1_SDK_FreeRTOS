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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_usb_cdc_app.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GCC ARM Embedded v6.3
* H/W Platform  : Renesas Starter Kit+ for RZ/A1H
* Description   : CDC display board sample
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.08.2015 1.00    Initial Version
*               : 24.04.2019 2.00    Porting for RZ/A1H
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>

#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "compiler_settings.h"

#include "console.h"
#include "command.h"
#include "control.h"

#include "usb_common.h"
#include "r_usb_hal.h"
#include "r_event.h"
#include "r_usbf_core.h"
#include "r_usb_cdc_app.h"

#include "main.h"
#include "trace.h"

#include "mcu_board_select.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

#if R_SELF_INSERT_APP_CDC_SERIAL_PORT

#define CDC_ASYNC_DEMO (0)
#define CDC_NORMAL_DEMO (1)

#if R_SELF_APP_CDC_ASYNC_ENABLE
#define CDC_RUN_DEMO (CDC_ASYNC_DEMO)
#else
#define CDC_RUN_DEMO (CDC_NORMAL_DEMO)
#endif

/*Size of buffer - the USB packet size is an efficient size to choose*/
#define BUFFER_SIZE (BULK_OUT_PACKET_SIZE)

/* Delay between repeating initial message */
#ifdef RELEASE
    #define KEY_DELAY_VALUE (0x00010000UL)
#else
    #define KEY_DELAY_VALUE (0x00000100UL)
#endif

typedef enum _cdc_event_t
{
    CDC_SAMPLE_CREATED = 0,
    CDC_COMMAND_ACTIVE,
    CDC_COMMAND_ISR_TRIG,
    CDC_NUM_SIGNALS
} cdc_event_t;

/******************************************************************************
Typedefs
******************************************************************************/

typedef struct _cdc_config_t
{
    PEVENT          ppev_signals[CDC_NUM_SIGNALS];
    _Bool           bf_active;
    uint32_t        taskid;
} cdc_config_t;

typedef struct _cdc_config_t *pcdc_config_t;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/*Data Buffers*/
static uint8_t buffer1[BUFFER_SIZE];
static uint8_t buffer2[BUFFER_SIZE];
static uint8_t* pbuff_empty = buffer1;
static uint8_t* pbuff_full = buffer2;

#if (CDC_RUN_DEMO == CDC_ASYNC_DEMO)
static void console_async_demo(int_t _in, FILE * _p_out);
#else
static void console_normal_demo(int_t _in, FILE * _p_out);
static volatile usb_err_t  m_read_error = USB_ERR_OK;
#endif /* (CDC_RUN_DEMO == CDC_ASYNC_DEMO) */

/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t    m_echo = FALSE;

static volatile usb_err_t  m_read_num_bytes = 0;

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
static uint8_t  m_usb_real_name[][8]    = {"CN2","UNKNOWN"};
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
static uint8_t  m_usb_real_name[][8]    = {"CN6","UNKNOWN"};
#endif /* TARGET_BOARD */

static uint8_t  m_usb_device_name[][10] = {"usbf0_cdc","usbf1_cdc"};

static int16_t cmd_cdc_console (int iArgCount, char **ppszArgument, pst_comset_t pCom);

static int_t    m_iusbf = -1;

/******************************************************************************
Function Prototypes
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void R_MSG_WarningConfig(pst_comset_t p_com, char_t *msg);

/******************************************************************************
* Table that associates command letters, function pointer and a little
* description of what the command does
******************************************************************************/
static st_cmdfnass_t g_pcmd_cdc_sample[] =
{
     {
        "cdcconsole",
        cmd_cdc_console,
                   "<CR> - Run CDC console, requires a terminal program\r\n"
        "                  (ie Hyperterminal) to connect to RSK\r\n"
        "                  Terminal settings can be defined by host"
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_cdc_sample[] =
{
     {
      "USB CDC class ",
      g_pcmd_cdc_sample,
      ((sizeof(g_pcmd_cdc_sample))/(sizeof(st_cmdfnass_t)))
     }
};

/******************************************************************************
user Program Code
******************************************************************************/


/******************************************************************************
Function Name : cbdone_read_cdc
Description   : Callback called when a USBCDC_Read_Async request
                has completed. i.e. Have read some data.
                If in echo mode then write data just read back out.
Parameters    : _err : Error code.
                _NumBytes : Number of bytes read.
Return value:    -
*******************************************************************************/
static void cbdone_read_cdc(usb_err_t _err, uint32_t _num_bytes)
{
    UNUSED_PARAM (_err);

    /*Toggle buffers - as now the empty buffer has been filled
    by this read completing*/

    if(pbuff_empty == buffer2)
    {
        pbuff_empty = buffer1;
        pbuff_full = buffer2;
    }
    else
    {
        pbuff_empty = buffer2;
        pbuff_full = buffer1;
    }

    m_read_num_bytes = _num_bytes;
}
/******************************************************************************
End of function cbdone_read_cdc
******************************************************************************/

/******************************************************************************
Function Name  : cbdone_write
Description    : Callback called when a USBCDC_Write_Async request
                 has completed. i.e. Have written some data.
Parameters:    :  _err: Error code.
Return value:    -
******************************************************************************/
static void cbdone_write(usb_err_t _err)
{
    /*Write has completed*/

    assert(USB_ERR_OK == _err);
}
/******************************************************************************
End of function cbdone_write
******************************************************************************/

#if (CDC_RUN_DEMO == CDC_ASYNC_DEMO)
/*******************************************************************************
* Function Name: cdc_console_non_blocking_task
* Description  : ASYNC version of serial port simulation over USB task
* Arguments    : parameters - NOT USED
* Return Value : none
*******************************************************************************/
static void cdc_console_non_blocking_task (void *parameters)
{
    UNUSED_PARAM(parameters);
    m_read_num_bytes = 0;

    while(true)
    {
        /*Setup another read*/
        if(0 == m_read_num_bytes)
        {
            m_read_num_bytes = read(m_iusbf, pbuff_empty,1);
        }

        /*Write back data if at least 1 byte has been read*/
        if(m_read_num_bytes > 0)
        {
            m_read_num_bytes--;

            /* Create new read */
            if(TRUE == m_echo)
            {
                /*Write back data if at least 1 byte has been read*/
                write(m_iusbf, pbuff_full,1);
            }
        }

        R_OS_Yield();
    }
}
/*******************************************************************************
End of function cdc_console_non_blocking_task
*******************************************************************************/

/*******************************************************************************
* Function Name: console_async_demo
* Description  : ASYNC version of serial port simulation over USB
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void console_async_demo(int_t _in, FILE * _p_out)
{
    os_task_t *p_os_task;
    static BOOL  terminate_requetsed = FALSE;

    fprintf(_p_out,"\r\nCDC_ASYNC_DEMO demonstration selected\r\n");
    fprintf(_p_out,"I/O in this mode is non-blocking\r\n");
    fprintf(_p_out,"This demonstration can only be terminated by keyboard input\r\n");
    fprintf(_p_out,"from this terminal, no need for data to be received from host \r\n");


    m_echo = TRUE;
    terminate_requetsed = FALSE;

    p_os_task = R_OS_CreateTask("cdc non-blocking", cdc_console_non_blocking_task, NULL, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE, 1);

    if(NULL !=p_os_task)
    {
        while(FALSE == terminate_requetsed)
        {
            if(FALSE == control(m_iusbf, CTL_USBF_IS_CONNECTED, NULL))
            {
                terminate_requetsed =  TRUE;
            }

            /* If key press then abort sample */
            if(0 != control(_in, CTL_GET_RX_BUFFER_COUNT, NULL))
            {
                terminate_requetsed =  TRUE;
            }

            R_OS_TaskSleep(10);
        }
        control(m_iusbf, CTL_USBF_START, NULL);

        R_OS_TaskSleep(100);

        control(m_iusbf, CTL_USBF_STOP, NULL);

        R_OS_DeleteTask(p_os_task);
    }
}
/*******************************************************************************
End of function console_async_demo
*******************************************************************************/

#else

/*******************************************************************************
* Function Name: cdc_console_blocking_task
* Description  : NORMAL version of serial port simulation over USB task
* Arguments    : parameters - NOT USED
* Return Value : none
*******************************************************************************/
static void cdc_console_blocking_task (void *parameters)
{
    m_read_num_bytes = 0;
    m_read_error = 0;

    while(true)
    {
        if(0 == m_read_num_bytes)
        {
            m_read_num_bytes = read(m_iusbf, pbuff_full,BUFFER_SIZE);
        }

        /*Write back data if at least 1 byte has been read*/
        if(0 != m_read_num_bytes)
        {
            /* Create new read */
            if(TRUE == m_echo)
            {
                write(m_iusbf, pbuff_full, m_read_num_bytes);
            }
            m_read_num_bytes = 0;
        }
        R_OS_Yield();
    }
}
/*******************************************************************************
End of function console_async_demo
*******************************************************************************/



/*******************************************************************************
* Function Name: console_normal_demo
* Description  : Blocking (normal) version of serial port simulation over USB
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void console_normal_demo(int_t _in, FILE * _p_out)
{
    static BOOL  terminate_requetsed = FALSE;

    os_task_t *p_os_task;

    fprintf(_p_out,"\r\nCDC_NORMAL_DEMO demonstration selected *** \r\n");
    fprintf(_p_out,"I/O in this mode is blocking\r\n");
    fprintf(_p_out,"This demonstration can only be terminated by keyboard input\r\n");
    fprintf(_p_out,"from this terminal, no need for data to be received from host \r\n");

    m_echo = TRUE;
    terminate_requetsed = FALSE;

    p_os_task = R_OS_CreateTask("cdc console blocking", cdc_console_blocking_task, NULL, R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE, 1);

    if(NULL !=p_os_task)
    {
        while(FALSE == terminate_requetsed)
        {
            if(FALSE == control(m_iusbf, CTL_USBF_IS_CONNECTED, NULL))
            {
                terminate_requetsed =  TRUE;
            }

            /* If key press then abort sample */
            if(0 != control(_in, CTL_GET_RX_BUFFER_COUNT, NULL))
            {
                terminate_requetsed =  TRUE;
            }

            R_OS_TaskSleep(10);
        }

        control(m_iusbf, CTL_USBF_START, NULL);

        R_OS_TaskSleep(100);

        control(m_iusbf, CTL_USBF_STOP, NULL);

        R_OS_DeleteTask(p_os_task);

    }
}
/*******************************************************************************
End of function console_normal_demo
*******************************************************************************/

#endif /*(CDC_RUN_DEMO == CDC_ASYNC_DEMO) */

/******************************************************************************
* Function Name: cmd_cdc_console
* Description  : Test task to flash the LEDs
* Arguments    : none
* Return Value : none
******************************************************************************/
static int16_t cmd_cdc_console (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int8_t        hasaborted = 0;  /* -1 user abort; 0 default state; 1 cable disconnected */
    volatile uint8_t device_select = 0; /* Select usbf0_cdc by default */
    int_t i_in = R_DEVLINK_FilePtrDescriptor(pCom->p_in);

    UNUSED_PARAM(iArgCount);
    UNUSED_PARAM(ppszArgument);

#if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER
    R_MSG_WarningConfig(pCom, "R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER");
    return CMD_OK;
#endif

    /* open interface to usb function device */
    m_iusbf = open((char *)DEVICE_INDENTIFIER "usbf0_cdc", O_RDWR, _IONBF);
    if((-1) == m_iusbf)
    {
        /* usbf1_hid device selected */
        device_select = 1;
        m_iusbf = open((char *)DEVICE_INDENTIFIER "usbf1_cdc", O_RDWR, _IONBF);
    }

    if((-1) != m_iusbf)
    {
        st_usbf_asyn_config_t rw_config;

        /* Set rw_mode for function device (note NORMAL mode is set by default) */
#if (CDC_RUN_DEMO == CDC_ASYNC_DEMO)
        rw_config.mode = USBF_ASYNC;
        rw_config.pin_done_async  = cbdone_write;
        rw_config.pout_done_async = cbdone_read_cdc;
#else
        rw_config.mode = USBF_NORMAL;
        rw_config.pin_done_async  = NULL;
        rw_config.pout_done_async = NULL;
#endif
        control(m_iusbf, CTL_USBF_SET_RW_MODE, &rw_config);
        control(m_iusbf, CTL_USBF_START, NULL);

        fprintf(pCom->p_out,"CDC Class Sample Application\r\n");
        fprintf(pCom->p_out,"Connect PC to USB connector %s\r\n",  m_usb_real_name[device_select]);
        fprintf(pCom->p_out,"\r\n*** CDC open %s ***\r\n", m_usb_device_name[device_select]);
        fprintf(pCom->p_out,"Failure to connect to host will cause application to fail \r\n");
        fprintf(pCom->p_out,"Serial port connection speeds supported up to 115200 \r\n");

        /* Flush any remaining characters from the buffer */
        while(control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
        {
            fgetc(pCom->p_in);
        }


        hasaborted = 0;
         m_echo = FALSE;

         /* Wait for user to confirm that USB cable is connected */
         while(0 == hasaborted)
        {
            if(FALSE == control(m_iusbf, CTL_USBF_IS_CONNECTED, NULL))
            {
                /* Check USB cable to be connected */
                hasaborted = 1;
            }

            /* If key press then abort sample */
            if(control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
            {
                hasaborted = -1;
            }
        }

        /* allow usb to enumerate */
        R_OS_TaskSleep(600);

        if(FALSE == control(m_iusbf, CTL_USBF_IS_CONNECTED, NULL))
        {
            fprintf(pCom->p_out,"*** waiting for cable to be connected ***\r\n");

            /* waiting for cable to be connected */
            while (1)
            {
                if(TRUE == control(m_iusbf, CTL_USBF_IS_CONNECTED, NULL))
                {
                    break;
                }
            }
        }

        /* Flush any remaining characters from the buffer */
        while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
        {
            fgetc(pCom->p_in);
        }

#if (CDC_RUN_DEMO == CDC_ASYNC_DEMO)

        console_async_demo(i_in, pCom->p_out);
#else

        console_normal_demo(i_in, pCom->p_out);
#endif

    }
    else
    {
        fprintf(pCom->p_out,
            "CDC function device can not be opened on channel  %s or  %s\r\n",
            m_usb_real_name[0],
            m_usb_real_name[1]);
    }
    close(m_iusbf);
    m_iusbf = -1;

    fprintf(pCom->p_out,"CDC demonstration terminated.\r\n");
    return CMD_OK;
}
/******************************************************************************
End of function  cmd_cdc_console
******************************************************************************/

#endif

/* End of File */

