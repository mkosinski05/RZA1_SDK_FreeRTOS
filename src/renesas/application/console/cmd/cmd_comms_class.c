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
 * File Name    : cmdCommsClass.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : Renesas
 * Description  : Commands to test the CDC device driver
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 26.01.2016 1.00 First issued release
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
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

/******************************************************************************
 User Includes
 ******************************************************************************/
#include "command.h"
#include "control.h"
#include "trace.h"
#include "cmd_comms_class.h"
#include "r_sci_drv_api.h"
#include "r_timer.h"
#include "r_os_abstraction_api.h"
#include "r_devlink_wrapper.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

#define CMD_TEST_DATA_SIZE (8192UL)
#define NO_DEVICE (-1)
#define MAX_LOOPBACK_PORTS (4)      /* max ports for the sloopall command */
#define TX_ALL_SIZE (64)            /* number of bytes to serve each port in TX all test */
#define STEST_TX_TIMEOUT (3000UL)   /* TX timeout used for the stest command */
#define STEST_MAX_SIZE   (32000UL)  /* maximum number of bytes for 'stest' */

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Global Variables
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* the file descriptor of the CDC device. Set to -1UL for no device */
static int_t gi_device = NO_DEVICE;

/* indicates a test is in progress, ensures multiple tests are not run simultaneously */
static int_t command_in_progress = 0;

/* set to -1UL for no TX timeout */
static uint32_t gdw_tx_timeout = -1UL;

/* the buffers used for loop-back tests */
static uint8_t gby_rxbuffer[2048];
static uint8_t gby_txbuffer[2048];

/******************************************************************************
 Constant Data
 ******************************************************************************/
static const uint32_t baud_rates[] =
{ 300UL, 600UL, 1200UL, 2400UL, 4800UL, 9600UL, 19200UL, 38400UL, 57600UL, 115200UL, };

/* string that is very easy to spot if it is wrong */
static const char psz_big_hello[] = "\r\n"
        "  #     #                                   ###\r\n"
        "  #     #  ######  #       #        ####    ###\r\n"
        "  #     #  #       #       #       #    #   ###\r\n"
        "  #######  #####   #       #       #    #    # \r\n"
        "  #     #  #       #       #       #    #      \r\n"
        "  #     #  #       #       #       #    #   ###\r\n"
        "  #     #  ######  ######  ######   ####    ###\r\n"
        "                      "
        "                         \r\n";

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/* table that associates command letters, function pointer and a little
   description of what the command does */
const st_cmdfnass_t g_pcmd_usb_commsclass[] =
{
    /* Introduction Banner */
    {
        "",
        NULL,
        "\r\n        USB CDC Serial Port Commands"
    },

    {
        "",
        NULL,
        "Examples of using a USB CDC to RS232 converter",
    },

    {
        "",
        NULL,
        "----------------------------------------------",
    },

    {
        "sopen",
        (const CMDFUNC) cmd_sci_open,
        "<CR> - Opens a CDC device"
    },

    {
        "sclose",
        (const CMDFUNC) cmd_sci_close,
        "<CR> - Closes an open CDC device"
    },

    {
        "sctltst",
        (const CMDFUNC) cmd_sci_ctrl_api_test,
        "<CR> - Perform control API tests for the CDC driver"
    },

    {
        "sttx",
        (const CMDFUNC) cmd_sci_test_tx,
        "n<CR> - Transmit n k bytes of data through the CDC device"
    },

    {
        "sloop",
        (const CMDFUNC) cmd_sci_loop_back,
        "<CR> - Loops-back received characters through the CDC device"
    },

    {
        "sbaud",
        (const CMDFUNC) cmd_sci_set_baud,
        "n<CR> - Set the baud rate to n"
    },

    {
        "scontrol",
        (const CMDFUNC) cmd_sci_set_control_lines,
        "n<CR> - Assert / Deassert RTS/DTR control signals n = 1 or 0"
    },

    {
        "sparity",
        (const CMDFUNC) cmd_sci_set_parity,
        "p<CR> - Sets the parity to N = none, E = Even, O = Odd"
    },

    {
        "sstop",
        (const CMDFUNC) cmd_sci_set_stop_bits,
        "s<CR> - Sets the number of stop bits 1, 1.5 or 2"
    },

    {
        "sline",
        (const CMDFUNC) cmd_sci_get_line_status,
        "<CR> - Returns the line status"
    },

    {
        "sbreak",
        (const CMDFUNC) cmd_sci_break_signal,
        "n<CR> - Sets / clears the break signal"
    },

    {
        "stest",
        (const CMDFUNC) cmd_sci_test_all,
        "s<CR> - Test all CDC driver functions with a loop-back connector"
    },

    {
        "sloopall",
        (const CMDFUNC) cmd_sci_loop_all,
        "<CR> - Loop-back test on max. 4 CDC devices (default baud rate)"
    }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_usb_commsclass =
{
    "CDC Commands",
    (pst_cmdfnass_t) g_pcmd_usb_commsclass,
    ((sizeof(g_pcmd_usb_commsclass)) / sizeof(st_cmdfnass_t))
};

/*****************************************************************************
 Function Name: cmd_sci_is_open
 Description:   Function to make sure that the device is open
 Parameters:    IN  pStatus - File stream to print the status to
 Return value:  true if the device is open
 *****************************************************************************/
static _Bool cmd_sci_is_open(FILE *pStatus)
{
    _Bool bf_result = false;

    /* check if there is a valid pStatus */
    if (pStatus)
    {
        /* check to see if the device is already open */
        if (gi_device >= 0)
        {
            _Bool b_attached = false;

            /* the device is open - see if it is still there */
            if ((control(gi_device, CTL_USB_ATTACHED, &b_attached) == 0) && (b_attached))
            {
                fprintf(pStatus, "The device is already open\r\n");
                bf_result = true;
            }
            else
            {

                /* the device has been removed so close it */
                close(gi_device);
                gi_device = NO_DEVICE;
            }
        }

        /* check to see if the device should be opened */
        if (NO_DEVICE == gi_device)
        {
            /* attempt to open the device */
            gi_device = open("\\\\.\\USBCDC", O_RDWR, _IONBF);

            if (gi_device >= 0)
            {
                fprintf(pStatus, "Device opened\r\n");
                bf_result = true;
            }
            else
            {
                fprintf(pStatus, "Device not found!\r\n");
            }
        }
    }

    return bf_result;
}
/*****************************************************************************
 End of function  cmd_sci_is_open
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_sci_print_error_string
 Description:   Function to get the last error code and print the string
 Parameters:    IN  l_result - The result code from the test
                IN  lPass - The pass code
                IN  pOut - Pointer to the file stream to print to
 Return value:  none
 *****************************************************************************/
static void cmd_sci_print_error_string(int32_t l_result, int32_t lPass, FILE *pOut)
{
    ERRSTR err_strl;

    err_strl.iErrorCode = 0;

    if (l_result == lPass)
    {
        fprintf(pOut, "PASSED\r\n");
    }
    else
    {
        fprintf(pOut, "FAILED\r\n");
    }

    /* get the last error into the error code field */
    if (!control(gi_device, CTL_GET_LAST_ERROR, &err_strl.iErrorCode))
    {
        /* get the error string pointer */
        if (!control(gi_device, CTL_GET_ERROR_STRING, &err_strl))
        {
            fprintf(pOut, "%s %d\r\n", err_strl.pszErrorString, err_strl.iErrorCode);
        }
        else
        {
            fprintf(pOut, "Failed to get error string\r\n");
        }
    }
    else
    {
        fprintf(pOut, "Failed to get last error code\r\n");
    }
}
/*****************************************************************************
 End of function  cmd_sci_print_error_string
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_open
 Description:   Command to open the device
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_open(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* avoid unused warning */
    (void) arg_count;
    (void) ppsz_argument;
    (void) p_com;

    cmd_sci_is_open(p_com->p_out);
    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_open
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_close
 Description:   Command to open the device
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_close(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* standard API - avoid unused warning */
    (void) ppsz_argument;
    (void) arg_count;

    /* check to see if the device is open */
    if (NO_DEVICE != gi_device)
    {
        /* close the device */
        close(gi_device);
        gi_device = NO_DEVICE;
        fprintf(p_com->p_out, "Device closed\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "Device not open\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_close
 ******************************************************************************/

/*****************************************************************************
 Function Name: sci_test_configuration
 Description:   subCommand to test the configuration set/get of the device driver
 Parameters:    IN  p_com - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void sci_test_configuration(pst_comset_t p_com)
{
    int32_t l_result;
    SCICFG sci_get_config;
    SCICFG sci_set_config =
    { 115200UL,            // Baud rate

      /* Line Coding - asynchronous with no flow control (N,8,1) */
      ((SCI_PARITY_NONE | SCI_DATA_BITS_EIGHT ) | SCI_ONE_STOP_BIT ), };

    /* Test the control codes */
    fprintf(p_com->p_out, "Testing invalid control code: ");
    l_result = control(gi_device, (CTLCODE) -1, NULL);
    cmd_sci_print_error_string(l_result, -1, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_SCI_SET_CONFIGURATION: ");
    l_result = control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_set_config);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing invalid CTL_SCI_SET_CONFIGURATION: ");
    memset(&sci_get_config, 0, sizeof(SCICFG));
    l_result = control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_get_config);
    cmd_sci_print_error_string(l_result, -1, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_SCI_GET_CONFIGURATION: ");
    l_result = control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_get_config);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);

    if (memcmp(&sci_get_config, &sci_set_config, sizeof(SCICFG)) == 0)
    {
        fprintf(p_com->p_out, "Configuration values OK\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "Configuration values NG\r\n");
    }
}
/******************************************************************************
 End of function sci_test_configuration
 ******************************************************************************/

/*****************************************************************************
 Function Name: sci_test_timeout_attach
 Description:   subCommand to test set/get timeout & USB Attachment
 Parameters:    IN  p_com - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void sci_test_timeout_attach(pst_comset_t p_com)
{
    uint32_t dw_tx_timeout;
    _Bool b_attached = false;
    int32_t l_result;

    fprintf(p_com->p_out, "Testing invalid CTL_GET_TIME_OUT: ");
    l_result = control(gi_device, CTL_GET_TIME_OUT, NULL);
    cmd_sci_print_error_string(l_result, -1, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_GET_TIME_OUT: ");
    l_result = control(gi_device, CTL_GET_TIME_OUT, &dw_tx_timeout);
    cmd_sci_print_error_string (l_result, 0, p_com->p_out);
    dw_tx_timeout = 100;
    fprintf(p_com->p_out, "Testing valid CTL_SET_TIME_OUT: ");
    l_result = control(gi_device, CTL_SET_TIME_OUT, &dw_tx_timeout);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing default CTL_SET_TIME_OUT: ");
    l_result = control(gi_device, CTL_SET_TIME_OUT, &gdw_tx_timeout);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing invalid CTL_USB_ATTACHED: ");
    l_result = control(gi_device, CTL_USB_ATTACHED, NULL);
    cmd_sci_print_error_string(l_result, -1, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_USB_ATTACHED: ");
    l_result = control(gi_device, CTL_USB_ATTACHED, &b_attached);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);

    if (b_attached)
    {
        fprintf(p_com->p_out, "Device is attached\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "Device has been removed\r\n");
    }
}
/******************************************************************************
 End of function sci_test_timeout_attach
 ******************************************************************************/

/*****************************************************************************
 Function Name: sci_test_break
 Description:   subCommand to test set/get break command
 Parameters:    IN  p_com - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void sci_test_break(pst_comset_t p_com)
{
    int32_t l_result;
    SCILST sci_line_status;

    fprintf(p_com->p_out, "Testing valid CTL_SCI_SET_BREAK: ");
    l_result = control(gi_device, CTL_SCI_SET_BREAK, NULL);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing invalid CTL_SCI_GET_LINE_STATUS: ");
    l_result = control(gi_device, CTL_SCI_GET_LINE_STATUS, NULL);
    cmd_sci_print_error_string(l_result, -1, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_SCI_GET_LINE_STATUS: ");
    l_result = control(gi_device, CTL_SCI_GET_LINE_STATUS, &sci_line_status);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);

    if (sci_line_status.breakOutput)
    {
        fprintf(p_com->p_out, "Break ON: PASS\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "Break OFF: FAIL\r\n");
    }

    fprintf(p_com->p_out, "Testing valid CTL_SCI_CLEAR_BREAK: ");
    l_result = control(gi_device, CTL_SCI_CLEAR_BREAK, NULL);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_SCI_GET_LINE_STATUS: ");
    l_result = control(gi_device, CTL_SCI_GET_LINE_STATUS, &sci_line_status);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);

    if (sci_line_status.breakOutput)
    {
        fprintf(p_com->p_out, "Break ON: FAIL\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "Break OFF: PASS\r\n");
    }
}
/******************************************************************************
 End of function sci_test_break
 ******************************************************************************/

/*****************************************************************************
 Function Name: sci_test_purge
 Description:   subCommand to test purge buffer command
 Parameters:    IN  p_com - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void sci_test_purge(pst_comset_t p_com)
{
    int32_t l_result;

    fprintf(p_com->p_out, "Testing valid CTL_SCI_PURGE_BUFFERS: ");
    l_result = control(gi_device, CTL_SCI_PURGE_BUFFERS, NULL);
    cmd_sci_print_error_string(l_result, 0, p_com->p_out);
    fprintf(p_com->p_out, "Testing valid CTL_SCI_PURGE_BUFFERS: ");
    l_result = control(gi_device, CTL_GET_RX_BUFFER_COUNT, NULL);

    if (l_result >= 0)
    {
        fprintf(p_com->p_out, "PASS\r\n");
    }
    else
    {
        fprintf(p_com->p_out, "FAIL\r\n");
    }
}
/******************************************************************************
 End of function sci_test_purge
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_sci_ctrl_api_test
 Description:   Command to test the basic control API of the device driver
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_ctrl_api_test(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* avoid unused warning */
    (void) arg_count;
    (void) ppsz_argument;
    (void) p_com;

    if (0 == command_in_progress)
    {
        command_in_progress = 1;

        if (cmd_sci_is_open(p_com->p_out))
        {
            sci_test_configuration(p_com);
            sci_test_timeout_attach(p_com);
            sci_test_break(p_com);
            sci_test_purge(p_com);
        }
        else
        {
            fprintf(p_com->p_out, "Device not found\r\n");
        }

        command_in_progress = 0;
    }
    else
    {
        fprintf(p_com->p_out, "Failed: CDC test already in progress\r\n");
    }

    return CMD_OK;
}
/*****************************************************************************
 End of function  cmd_sci_ctrl_api_test
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_sci_test_tx
 Description:   Command to send a specified quantity of data (in k Bytes)
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 *****************************************************************************/
int_t cmd_sci_test_tx(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    if (0 == command_in_progress)
    {
        command_in_progress = 1;

        if (cmd_sci_is_open(p_com->p_out))
        {
            size_t st_size = 1024;
            uint8_t *pby_string;

            /* get user specified length */
            if (arg_count > 1)
            {
                float f_filesize = 1.0f;

                /* get the desired file size */
                sscanf((char*) ppsz_argument[1], "%f", &f_filesize);

                /* calculate the test file size - always a multiple of four bytes */
                st_size = (size_t) (f_filesize * 1024.0f);

                /* limit to a maximum size of 0.5 MBytes */
                if (st_size > (size_t) ((1024.0f * 1024.0f) * 0.50f))
                {
                    st_size = (size_t) ((1024.0f * 1024.0f) * 0.50f);
                }

                /* make a multiple of 4 bytes */
                st_size += (st_size % sizeof(uint32_t));
            }

            /* allocate a buffer for the transmission */
            pby_string = (uint8_t *) R_OS_AllocMem(st_size, R_REGION_LARGE_CAPACITY_RAM);

            if (pby_string)
            {
                /* initialise the memory with the big hello */
                size_t st_length = st_size;
                uint8_t *pfill = pby_string;
                TMSTMP perf_timer;
                float f_time;
                int_t result;

                while (st_length)
                {
                    size_t st_copy_len = (st_length > (sizeof(psz_big_hello))) ? (sizeof(psz_big_hello)) : st_length;
                    memcpy(pfill, psz_big_hello, st_copy_len);
                    st_length -= st_copy_len;
                    pfill += st_copy_len;
                }

                /* at this point we cannot set a realistic timeout as the user may have requested
                 * up to 524288 bytes and could be at 300 baud ~17476 seconds */
                /* set the tx-time out value to infinite */
                gdw_tx_timeout = -1UL;
                control(gi_device, CTL_SET_TIME_OUT, &gdw_tx_timeout);
                fprintf(p_com->p_out, "Writing %d bytes\r\n", st_size);

                /* start a performance timer */
                timerStartMeasurement(&perf_timer);

                /* write the data */
                result = write(gi_device, pby_string, st_size);

                /* stop the performance timer */
                f_time = timerStopMeasurement(&perf_timer);
                fprintf(p_com->p_out, "Data sent with return code %d ", result);

                if (result <= 0)
                {
                    ERRSTR err_strl;
                    err_strl.iErrorCode = 0;
                    control(gi_device, CTL_GET_LAST_ERROR, &err_strl.iErrorCode);
                    control(gi_device, CTL_GET_ERROR_STRING, &err_strl);
                    fprintf(p_com->p_out, "(%s)\r\n", err_strl.pszErrorString);
                }
                else
                {
                    fprintf(p_com->p_out, "\r\n");

                    /* print the transfer rate */
                    cmd_show_data_rate(p_com->p_out, f_time, (size_t) result);
                }

                R_OS_FreeMem(pby_string);
            }
            else
            {
                fprintf(p_com->p_out, "**Error: Failed to allocate memory\r\n");
            }
        }
        else
        {
            fprintf(p_com->p_out, "Device not found\r\n");
        }

        command_in_progress = 0;
    }
    else
    {
        fprintf(p_com->p_out, "Failed: CDC Test Already in Progress\r\n");
    }

    return CMD_OK;
}
/*****************************************************************************
 End of function  cmd_sci_test_tx
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_sci_loop_back
 Description:   Command to loop-back all data received
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 *****************************************************************************/
int_t cmd_sci_loop_back(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* avoid unused warning */
    (void) arg_count;
    (void) ppsz_argument;
    (void) p_com;

    if (0 == command_in_progress)
    {
        command_in_progress = 1;

        if (cmd_sci_is_open(p_com->p_out))
        {
            int_t i_total = 0;
            int_t i_read;
            int_t i_in = R_DEVLINK_FilePtrDescriptor(p_com->p_in);

            /* flush any remaining characters from the buffer */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
            {
                fgetc(p_com->p_in);
            }

            fprintf(p_com->p_out, "CDC Loop-back test, press any key to stop.\r\n");

            /* ensure no data is in any of the CDC buffers */
            control(gi_device, CTL_SCI_PURGE_BUFFERS, NULL);

            /* while there is no key press on the input stream */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) == 0)
            {

                /* if there is data that has been received from the device*/
                if (control(gi_device, CTL_GET_RX_BUFFER_COUNT, NULL))
                {

                    /* Ref FTDI AN232B-04, requesting => 64 bytes can create a
                     * large packet to build up in the USB driver before passing to
                     * this task. This knocks on to a write packet that exceeds
                     * the buffers of the function device and causes data loss. This is
                     * typically seen at high processor loads */
                    i_read = read(gi_device, gby_rxbuffer, sizeof(gby_rxbuffer));

                    if (i_read > 0)
                    {
                        int_t i_write = write(gi_device, gby_rxbuffer, (size_t) i_read);
                        i_total += i_read;

                        if (i_write != i_read)
                        {
                            fprintf(p_com->p_out, "\rError");
                        }
                    }
                }
                else
                {
                    R_OS_TaskSleep(1);            /* small sleep to allow other tasks some processor time */
                }
            }

            /* Throw away the key press to stop */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
            {
                fgetc(p_com->p_in);
            }

            fprintf(p_com->p_out, "%d bytes sent/received\r\n", i_total);
            fprintf(p_com->p_out, "\r\n");
        }
        else
        {
            fprintf(p_com->p_out, "Device not found\r\n");
        }

        command_in_progress = 0;
    }
    else
    {
        fprintf(p_com->p_out, "Failed: CDC Test Already in Progress\r\n");
    }

    return CMD_OK;
}
/*****************************************************************************
 End of function  cmd_sci_loop_back
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_set_baud
 Description:   Command to set the baud rate
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_set_baud(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* make sure the driver has been opened */
    if (cmd_sci_is_open(p_com->p_out))
    {
        SCICFG sci_config;

        if (!control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config))
        {
            fprintf(p_com->p_out, "Current Baud rate %lu\r\n", sci_config.dwBaud);
        }
        else
        {
            fprintf(p_com->p_out, "Failed to get configuration\r\n");
        }

        /* if the user specified a rate to set */
        if (arg_count > 1)
        {
            /* find the baud rate in the users input string */
            if (sscanf((char *) ppsz_argument[1], "%lu", &sci_config.dwBaud))
            {
                if (!control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config))
                {
                    fprintf(p_com->p_out, "Baud rate set to %lu\r\n", sci_config.dwBaud);
                }
                else
                {
                    fprintf(p_com->p_out, "Failed to set baud rate to %lu\r\n", sci_config.dwBaud);
                    control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config);
                    fprintf(p_com->p_out, "Baud rate is %lu\r\n", sci_config.dwBaud);
                }
            }
            else
            {
                fprintf(p_com->p_out, "Expecting baud rate got \"%s\"\r\n", ppsz_argument[1]);
            }
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_set_baud
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_set_control_lines
 Description:   Command to assert or de-assert both RTS and DTR control signals
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_set_control_lines(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* make sure the driver has been opened */
    if (cmd_sci_is_open (p_com->p_out))
    {

        SCICFG sci_config;
        static const char * const ppsz_dis_ena[] =
        { "Deasserted", "Asserted" };

        /* get the current configuration */
        if (control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config))
        {
            fprintf(p_com->p_out, "Failed to get configuration\r\n");
        }

        /* if the user provided a setting */
        if (arg_count > 1)
        {
            int_t ihardware;

            /* find the baud rate in the user's input string */
            if (sscanf((char *) ppsz_argument[1], "%d", &ihardware))
            {
                if (ihardware)
                {
                    /* set RTS & DTR control lines on */
                    sci_config.dwConfig |= SCI_DTR_ASSERT;
                    sci_config.dwConfig |= SCI_RTS_ASSERT;
                }
                else
                {
                    /* set RTS & DTR control lines off */
                    sci_config.dwConfig &= (~SCI_DTR_ASSERT);
                    sci_config.dwConfig &= (~SCI_RTS_ASSERT);
                }

                /* attempt to set the new configuration */
                if (!control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config))
                {
                    _Bool bf_dtr = (sci_config.dwConfig & SCI_DTR_ASSERT) ? true : false;
                    _Bool bf_rts = (sci_config.dwConfig & SCI_RTS_ASSERT) ? true : false;
                    fprintf(p_com->p_out, "Flow control DTR %s\r\n", ppsz_dis_ena[bf_dtr]);
                    fprintf(p_com->p_out, "Flow control RTS %s\r\n", ppsz_dis_ena[bf_rts]);
                }
                else
                {
                    fprintf(p_com->p_out, "Failed to set control lines\r\n");
                }
            }
            else
            {
                fprintf(p_com->p_out, "Expecting 1 or 0, got \"%s\"\r\n", ppsz_argument[1]);
            }
        }
        else
        {
            /* no user setting was provided to report the current status */
            _Bool bf_dtr = (sci_config.dwConfig & SCI_DTR_ASSERT) ? true : false;
            _Bool bf_rts = (sci_config.dwConfig & SCI_RTS_ASSERT) ? true : false;
            fprintf(p_com->p_out, "Control Line DTR %s\r\n", ppsz_dis_ena[bf_dtr]);
            fprintf(p_com->p_out, "Control Line RTS %s\r\n", ppsz_dis_ena[bf_rts]);
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_set_control_lines
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_set_parity
 Description:   Command to set the parity
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_set_parity(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* make sure the driver has been opened */
    if (cmd_sci_is_open(p_com->p_out))
    {
        SCICFG sci_config;

        /* get the current configuration */
        if (control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config))
        {
            fprintf(p_com->p_out, "Failed to get configuration\r\n");
        }

        /* if the user provided a setting */
        if (arg_count > 1)
        {
            switch (ppsz_argument[1][0])
            {
                case 'N':
                case 'n':
                    sci_config.dwConfig &= (~(SCI_PARITY_EVEN | SCI_PARITY_ODD));
                    sci_config.dwConfig |= SCI_PARITY_NONE;
                break;

                case 'E':
                case 'e':
                    sci_config.dwConfig &= (~(SCI_PARITY_NONE | SCI_PARITY_ODD));
                    sci_config.dwConfig |= SCI_PARITY_EVEN;
                break;

                case 'O':
                case 'o':
                    sci_config.dwConfig &= (~(SCI_PARITY_NONE | SCI_PARITY_EVEN));
                    sci_config.dwConfig |= SCI_PARITY_ODD;
                break;

                default:
                    fprintf(p_com->p_out, "Expecting N (none), E (even) or O (odd)\r\n");
                break;
            }

            /* set the new configuration */
            if (control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config))
            {
                fprintf(p_com->p_out, "Failed to set parity\r\n");
            }
        }

        /* report the current status */
        if (sci_config.dwConfig & SCI_PARITY_NONE)
        {
            fprintf(p_com->p_out, "Parity = NONE\r\n");
        }

        if (sci_config.dwConfig & SCI_PARITY_EVEN)
        {
            fprintf(p_com->p_out, "Parity = EVEN\r\n");
        }

        if (sci_config.dwConfig & SCI_PARITY_ODD)
        {
            fprintf(p_com->p_out, "Parity = ODD\r\n");
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_set_parity
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_set_stop_bits
 Description:   Command to set the stop bits
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_set_stop_bits(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* make sure the driver has been opened */
    if (cmd_sci_is_open(p_com->p_out))
    {
        SCICFG sci_config;

        /* get the current configuration */
        if (control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config))
        {
            fprintf(p_com->p_out, "Failed to get configuration\r\n");
        }

        /* if the user provided a setting */
        if (arg_count > 1)
        {
            float f_stop = 0.0f;
            sscanf((char *) ppsz_argument[1], "%f", &f_stop);

            switch ((int_t) (f_stop * 10))                /* convert to integer for switch */
            {
                case (10): /* 1.0 * 10 */
                {
                    sci_config.dwConfig &= (~(SCI_ONE_HALF_STOP_BIT | SCI_TWO_STOP_BIT));
                    sci_config.dwConfig |= SCI_ONE_STOP_BIT;
                    break;
                }
                case (15): /* 1.5 * 10 */
                {
                    sci_config.dwConfig &= (~(SCI_ONE_STOP_BIT | SCI_TWO_STOP_BIT));
                    sci_config.dwConfig |= SCI_ONE_HALF_STOP_BIT;
                    break;
                }
                case (20): /* 2.0 * 10 */
                {
                    sci_config.dwConfig &= (~(SCI_ONE_STOP_BIT | SCI_ONE_HALF_STOP_BIT));
                    sci_config.dwConfig |= SCI_TWO_STOP_BIT;
                    break;
                }
                default:
                {
                    fprintf(p_com->p_out, "Expecting 1, 1.5 or 2\r\n");
                    break;
                }
            }

            /* set the new configuration */
            if (control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config))
            {
                fprintf(p_com->p_out, "Failed to set stop bits\r\n");
            }
        }

        /* report the current status */
        if (sci_config.dwConfig & SCI_ONE_STOP_BIT)
        {
            fprintf(p_com->p_out, "Stop bits = 1\r\n");
        }

        if (sci_config.dwConfig & SCI_ONE_HALF_STOP_BIT)
        {
            fprintf(p_com->p_out, "Stop bits = 1.5\r\n");
        }

        if (sci_config.dwConfig & SCI_TWO_STOP_BIT)
        {
            fprintf(p_com->p_out, "Stop bits = 2\r\n");
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_set_stop_bits
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_get_line_status
 Description:   Command to
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_get_line_status(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* avoid unused warning */
    (void) arg_count;
    (void) ppsz_argument;
    (void) p_com;

    /* make sure the driver has been opened */
    if (cmd_sci_is_open(p_com->p_out))
    {
        SCILST sci_line_status;

        if (!control(gi_device, CTL_SCI_GET_LINE_STATUS, &sci_line_status))
        {
            fprintf(p_com->p_out, "Clear To Send = %d\r\n", sci_line_status.clearToSend);
            fprintf(p_com->p_out, "Data Set Ready = %d\r\n", sci_line_status.dataSetReady);
            fprintf(p_com->p_out, "Ring Indicator = %d\r\n", sci_line_status.ringIndicator);
            fprintf(p_com->p_out, "Receive Line Signal Detect = %d\r\n", sci_line_status.receiveLineSignalDetect);
            fprintf(p_com->p_out, "Data Ready = %d\r\n", sci_line_status.dataReady);
            fprintf(p_com->p_out, "Overrun = %d\r\n", sci_line_status.overrunError);
            fprintf(p_com->p_out, "Parity Error = %d\r\n", sci_line_status.parityError);
            fprintf(p_com->p_out, "Frame Error = %d\r\n", sci_line_status.frameError);
            fprintf(p_com->p_out, "Break Indicator = %d\r\n", sci_line_status.breakSignal);
            fprintf(p_com->p_out, "Break Output = %d\r\n", sci_line_status.breakOutput);
        }
        else
        {
            fprintf(p_com->p_out, "Failed to get line status\r\n");
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_get_line_status
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_break_signal
 Description:   Command to set/clear the break output. If you want to pulse a
                "break-signal" set the baud rate to half of the present rate and
                write a zero - that's the old fashioned way.
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_break_signal(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    /* make sure the driver has been opened */
    if (cmd_sci_is_open(p_com->p_out))
    {
        int_t ibreaksignal = 0;

        /* if the user provided a setting */
        if (arg_count > 1)
        {
            sscanf((char *) ppsz_argument[1], "%d", &ibreaksignal);
        }

        if (ibreaksignal)
        {
            /* set the Break output */
            if (!control(gi_device, CTL_SCI_SET_BREAK, NULL))
            {
                fprintf(p_com->p_out, "Setting Break output ON\r\n");
            }
            else
            {
                fprintf(p_com->p_out, "Error asserting break\r\n");
            }
        }
        else
        {
            /* clear the Break output */
            if (!control(gi_device, CTL_SCI_CLEAR_BREAK, NULL))
            {
                fprintf(p_com->p_out, "Setting Break output OFF\r\n");
            }
            else
            {
                fprintf(p_com->p_out, "Error de-asserting break\r\n");
            }
        }
    }
    else
    {
        fprintf(p_com->p_out, "Device not found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_break_signal
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_sci_loop_back_test
 Description:   Function to perform a loop-back test for a quantity of data
 Parameters:    IN  st_length - The length of data to test
                IN  pOut - Pointer to the file stream to print to
                IN  pIn - Pointer to the input stream
 Return value:  0 for success or -1 on error
 *****************************************************************************/
static int_t cmd_sci_loop_back_test(size_t st_length, FILE *pOut, FILE *pIn)
{
    size_t result = 0;

    if (0 == command_in_progress)
    {
        command_in_progress = 1;
        fprintf(pOut, "Testing loop-back of %d bytes of data\r\n", st_length);
        fprintf(pOut, "Press any key to abort test\r\n");
        fflush(pOut);

        /* fill the test buffer with data */
        size_t st_count = sizeof(gby_txbuffer);
        uint8_t *pby_dest = gby_txbuffer;
        uint8_t by_value = 0;

        while (st_count--)
        {
            *pby_dest++ = by_value++;
        }

        /* for the length of data */
        while (result < st_length)
        {
            size_t st_read = 0;
            int_t i_in = R_DEVLINK_FilePtrDescriptor(pIn);
            size_t st_checkedlength = (st_length > (sizeof(gby_txbuffer))) ? (sizeof(gby_txbuffer)) : st_length;
            size_t st_written = (size_t) write(gi_device, gby_txbuffer, st_checkedlength);

            if (st_written != st_checkedlength)
            {
                ERRSTR err_strl;
                err_strl.iErrorCode = 0;
                fprintf(pOut, "Loop-back test (write): FAILED\r\n");

                /* get the last error into the error code field */
                if (!control(gi_device, CTL_GET_LAST_ERROR, &err_strl.iErrorCode))
                {
                    if (!control(gi_device, CTL_GET_ERROR_STRING, &err_strl))
                    {
                        fprintf(pOut, "%s %d\r\n", err_strl.pszErrorString, err_strl.iErrorCode);
                    }
                }

                command_in_progress = 0;
                return -1;
            }

            /* loop while checking for a key-press on the stdin stream */
            while ((control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) == 0) && (st_read < st_written)) // And All the data has been sent and received
            {
                st_read += (size_t) read(gi_device, gby_rxbuffer + st_read, (st_written - st_read));
                R_OS_TaskSleep(1);        /* Small sleep to allow other tasks some processor time */
            }

            /* if the stdin buffer is empty there was no key press */
            if (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL))
            {
                fprintf(pOut, "Loop-back test (aborted by user): FAILED\r\n");

                /* flush any characters from the buffer */
                while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
                {
                    fgetc(pIn);
                }

                command_in_progress = 0;
                return -1;
            }

            /* check the quantity of data matches */
            if (st_read != st_written)
            {
                ERRSTR err_strl;
                err_strl.iErrorCode = 0;
                fprintf(pOut, "Loop-back test (read): FAILED\r\n");

                /* get the last error into the error code field */
                if (!control(gi_device, CTL_GET_LAST_ERROR, &err_strl.iErrorCode))
                {

                    /* get the error string pointer */
                    if (!control(gi_device, CTL_GET_ERROR_STRING, &err_strl))
                    {
                        fprintf(pOut, "%s %d\r\n", err_strl.pszErrorString, err_strl.iErrorCode);
                    }
                }

                command_in_progress = 0;
                return -1;
            }

            /* compare the data buffers for errors */
            if (memcmp(gby_rxbuffer, gby_txbuffer, st_written))
            {
                fprintf(pOut, "Loop-back data miss-match\r\n");
                command_in_progress = 0;
                return -1;
            }

            result += st_written;
        }

        fprintf(pOut, "Loop-back test: PASSED\r\n");
        command_in_progress = 0;
    }
    else
    {
        fprintf(pOut, "Failed: CDC Test Already in Progress\r\n");
    }

    return 0;
}
/*****************************************************************************
 End of function  cmd_sci_loop_back_test
 ******************************************************************************/

/*****************************************************************************
 Function Name: sci_test_loopback
 Description:   Called by cmd_sci_test_all this function tests and validates
                loop-back data and includes a performance measurement and
                baud rate setting.
 Parameters:    IN  st_length - The length of data to test
                IN  p_com - Pointer to the file stream to print to
                IN  st_count - index for baud rate to use
 Return value:  true for success or false on error
 *****************************************************************************/
static _Bool sci_test_loopback(pst_comset_t p_com, size_t st_count, size_t st_length)
{
    _Bool bf_pass = true;

    SCICFG sci_config;

    if (!control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config))
    {
        sci_config.dwBaud = baud_rates[st_count];
        fprintf(p_com->p_out, "Setting baud rate to %lu\r\n", sci_config.dwBaud);

        if (!control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config))
        {
            TMSTMP perf_timer;
            float f_time;

            /* as the baud rate gets slower reduce the test size */
            size_t st_size = (size_t) ((float) st_length * ((float) sci_config.dwBaud / 115200.0f));

            /* start a performance timer */
            timerStartMeasurement(&perf_timer);

            if (cmd_sci_loop_back_test(st_size, p_com->p_out, p_com->p_in))
            {

                /* the test failed, so stop the performance timer */
                timerStopMeasurement(&perf_timer);
                bf_pass = false;
            }
            else /* loop-back test was successful - display stats */
            {
                /* stop the performance timer */
                f_time = timerStopMeasurement(&perf_timer);
                cmd_show_data_rate(p_com->p_out, f_time, st_size);
            }
        }
        else
        {
            fprintf(p_com->p_out, "Failed to set baud rate\r\n");
            bf_pass = false;
        }
    }
    else
    {
        fprintf(p_com->p_out, "Failed to get configuration\r\n");
        bf_pass = false;
    }

    return (bf_pass);
}
/*****************************************************************************
 End of function  sci_test_loopback
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_test_all
 Description:   Command to test all the main functions of the CDC driver
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_test_all(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    int_t i_in = R_DEVLINK_FilePtrDescriptor(p_com->p_in);

    if (0 == command_in_progress)
    {
        command_in_progress = 1;

        /* make sure the driver has been opened */
        if (cmd_sci_is_open(p_com->p_out))
        {
            SCICFG sci_config;
            uint32_t dw_length = CMD_TEST_DATA_SIZE;
            size_t st_count = (sizeof(baud_rates)) / sizeof(uint32_t);
            _Bool bf_pass = true;

            /* if the user provided a setting */
            if (arg_count > 1)
            {
                sscanf((char *) ppsz_argument[1], "%lu", &dw_length);

                /* limit the maximum size so that timeout is not hit at low bauds */
                if (dw_length > STEST_MAX_SIZE)
                {
                    dw_length = STEST_MAX_SIZE;
                }
            }

            /* flush any remaining characters from the buffer */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
            {
                fgetc(p_com->p_in);
            }

            fprintf(p_com->p_out, "Please make sure the CDC device is plugged in and a loop-back\r\n"
                     "connector is connected. Press any key to start the tests.\r\n");
            fflush(p_com->p_out);

            /* wait for the key-press */
            fgetc(p_com->p_in);

            /* flush any remaining characters from the buffer */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
            {
                fgetc(p_com->p_in);
            }

            /* check the control functions (code coverage) */
            command_in_progress = 0;    /* de-assert to allow the API test to run */
            cmd_sci_ctrl_api_test(arg_count, ppsz_argument, p_com);


            /* user can change settings for the test (like flow control and other
               options). Make sure that the TX timeout is set for STEST_TX_TIMEOUT */
            gdw_tx_timeout = STEST_TX_TIMEOUT;
            control(gi_device, CTL_SET_TIME_OUT, &gdw_tx_timeout);

            /* get the current configuration */
            control(gi_device, CTL_SCI_GET_CONFIGURATION, &sci_config);

            /* ensure no data is in any of the buffers */
            control(gi_device, CTL_SCI_PURGE_BUFFERS, NULL);

            /* check each of the baud rates */
            while ((st_count--) && (bf_pass))
            {
                bf_pass = sci_test_loopback(p_com, st_count, (size_t) dw_length);
            }

            command_in_progress = 1;

            /* return to the previous configuration */
            control(gi_device, CTL_SCI_SET_CONFIGURATION, &sci_config);

            if (bf_pass)
            {
                fprintf(p_com->p_out, "CDC Driver has PASSED!\r\n");
            }
            else
            {
                fprintf(p_com->p_out, "CDC Driver has FAILED!\r\n");
            }
        }
        else
        {
            fprintf(p_com->p_out, "Device not found\r\n");
        }
    }
    else
    {
        fprintf(p_com->p_out, "Failed: CDC Test Already in Progress\r\n");
    }

    command_in_progress = 0;
    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_test_all
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_sci_loop_all
 Description:   Command to perform a loop back on attached CDC devices
                Note: Uses default CDC baud rate.
 Parameters:    IN  arg_count The number of arguments in the argument list
                IN  ppsz_argument - The argument list
                IN  p_com - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
int_t cmd_sci_loop_all(int_t arg_count, char **ppsz_argument, pst_comset_t p_com)
{
    (void) arg_count;
    (void) ppsz_argument;

    volatile int_t device_number[MAX_LOOPBACK_PORTS];
    int_t i_read;
    int_t i_in;
    uint8_t device_count = 0;
    uint8_t loop_count;
    uint32_t time_out;

    i_in = R_DEVLINK_FilePtrDescriptor(p_com->p_in);

    if (0 == command_in_progress)
    {
        command_in_progress = 1;

        fprintf(p_com->p_out, "CDC Loop-back all test, press any key to stop.\r\n");

        /* close the global device if open */
        if (gi_device > 0)
        {
            close(gi_device);
        }

        time_out = 25;   /* 25ms ~ 114 bytes */

        /* attempt to open attached CDC devices */
        for (loop_count = 0; loop_count < MAX_LOOPBACK_PORTS; loop_count++)
        {
            device_number[loop_count] = open("\\\\.\\USBCDC", O_RDWR, _IONBF);

            /* count the opened ports and set their timeout value */
            if (device_number[loop_count] > 0)
            {
                device_count++;
                control(device_number[loop_count], CTL_SET_TIME_OUT, &time_out);

                /* ensure no data is in any of the CDC buffers */
                control(device_number[loop_count], CTL_SCI_PURGE_BUFFERS, NULL);
            }
        }

        if (device_count > 0)
        {
            fprintf(p_com->p_out, "Opened %d CDC devices for loop back.\r\n", device_count);

            /* while there is no key press on the input stream */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) == 0)
            {

                /* if there is data received from the device */
                for (loop_count = 0; loop_count < device_count; loop_count++)
                {

                    if (control(device_number[loop_count], CTL_GET_RX_BUFFER_COUNT, NULL))
                    {
                        i_read = read(device_number[loop_count], gby_rxbuffer, sizeof(gby_rxbuffer));

                        if (i_read > 0)
                        {
                            int_t i_write = write(device_number[loop_count], gby_rxbuffer, (size_t) i_read);

                            if (i_read != i_write)
                            {
                                fprintf(p_com->p_out, "Port %d Read: %d but write %d\r\n", loop_count, i_read, i_write);
                            }
                        }
                    }
                }

                R_OS_TaskSleep(1);        /* small sleep to allow other tasks some processor time */
            }

            /* throw away the key press to stop */
            while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
            {
                fgetc(p_com->p_in);
            }

            fprintf(p_com->p_out, "\r\n");

            /* close all the opened ports */
            for (loop_count = 0; loop_count < device_count; loop_count++)
            {
                close(device_number[loop_count]);
            }
        }
        else
        {
            fprintf(p_com->p_out, "Failed: No CDC devices opened \r\n");
        }

        command_in_progress = 0;
    }
    else
    {
        fprintf(p_com->p_out, "Failed: CDC Test Already in Progress\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_sci_loop_all
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

