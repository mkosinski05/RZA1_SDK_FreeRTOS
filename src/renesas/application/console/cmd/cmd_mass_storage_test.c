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
* File Name    : cmdMassStorageTest.c
* Version      : 1.01
* Device(s)    : Renesas
* Tool-Chain   : N/A
* OS           : N/A
* H/W Platform : RSK+
* Description  : The commands for test of MS devices
*******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 01.08.2009 1.00 First Release
*              : 12.01.2016 1.01 cmdWriteReadPerfTest: added feedback for read
*                                file open failure.
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

#include "command.h"
#include "trace.h"
#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"
#include "r_timer.h"
#include "cmdMassStorage.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

#define MAX_TEST_FILE_SIZE        (1024.0f * 1024.0f * 1000.0f)
#define MAX_BUFFER_SIZE            (256 * 1024)

/******************************************************************************
Constant Data
******************************************************************************/

const st_command_table_t gcmdUsbMassStorageTest;

/* Define the engineering exp symbols for Kilo, Mega, Giga, Tera, Peta,  Exa, Zetta and Yotta */
static const char * gpszEngMultChars = "#kMGTPEZY";

static TMSTMP perfTimer;
static int32_t start_time;

/******************************************************************************
External functions
******************************************************************************/

extern void cmdShowDataRate(FILE *pFile, float fTransferTime, size_t stLength);

/******************************************************************************
Private Functions
******************************************************************************/

static _Bool cmdCheckFileData(uint32_t *pdwFileData, size_t stLength, uint32_t *dwData, FILE *p_out);
static int32_t get_rtc_time (void);
static void start_timer (void);
static float stop_timer (FILE *p_out);
static void cmdMakeWRTestFileName(char *pszDestFileName, float fFileSize, pst_comset_t pCom);
static void cmdMakeReadTestFilePath(char *pszDestFileName, char *name, pst_comset_t pCom);

/******************************************************************************
 Function Name: cmdCheckFileData
 Description:   Function to check the file data
 Arguments:     OUT pdwFileData - Pointer to the file data
                IN  stLength - The length of the file in DWORDS
                OUT p_out - Pointer to the file stream to print to
 Return value:  true if the data matches
 ******************************************************************************/
static _Bool cmdCheckFileData(uint32_t *pdwFileData, size_t stLength, uint32_t *dwData, FILE *p_out)
{
    uint32_t iCount = 0;

    while (stLength--)
    {
        if (*pdwFileData != *dwData)
        {
            fprintf(p_out, "Error at %lu - Expecting 0x%.8lX got 0x%.8lX\r\n", (iCount * sizeof(uint32_t)), *dwData, *pdwFileData);
            fflush(p_out);
            return false;
        }

        pdwFileData++;
        (*dwData)++;
        iCount++;
    }

    return true;
}
/******************************************************************************
 End of function cmdCheckFileData
 ******************************************************************************/

/******************************************************************************
 Function Name: get_rtc_time
 Description:   Elapsed time from the RTC in seconds since the start of the month
 Arguments:     None
 Return value:  Number of seconds
 ******************************************************************************/
static int32_t get_rtc_time()
{
    DATE date;
    int_t rtc_handle;
    int32_t time;

    rtc_handle = open(DEVICE_INDENTIFIER "rtc", O_RDWR);

    control(rtc_handle, CTL_GET_DATE, &date);

    if ((-1) == rtc_handle)
    {
        return 0;
    }

    close(rtc_handle);

    time = date.Field.Second;
    time += date.Field.Minute * 60;
    time += date.Field.Hour * 60 * 60;
    time += (date.Field.Day - 1) * 60 * 60 * 24;

    return time;
}
/******************************************************************************
 End of function get_rtc_time
 ******************************************************************************/

/******************************************************************************
 Function Name: start_timer
 Description:   Start the performance timer
 Arguments:     None
 Return value:  None
 ******************************************************************************/
static void start_timer (void)
{
    timerStartMeasurement(&perfTimer);
    start_time = get_rtc_time();
}
/******************************************************************************
 End of function start_timer
 ******************************************************************************/

/******************************************************************************
 Function Name: stop_timer
 Description:   Start the performance timer
 Arguments:     p_out - output stream for printf
 Return value:  Number of seconds since start_timer() was called
 ******************************************************************************/
static float stop_timer (FILE *p_out)
{
    float fTime = timerStopMeasurement(&perfTimer);
    int32_t stop_time = get_rtc_time();
    stop_time -= start_time;

    /* if the performance timer has overflowed, then use the RTC */
    if (stop_time > fTime + 1)
    {
        fTime = (float) stop_time;
    }

    fprintf(p_out, "Time taken %g seconds\r\n", fTime);

    return fTime;
}
/******************************************************************************
 End of function stop_timer
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdReadPerfTest
 Description:   Function to perform a file read performance test
 Arguments:     IN  iArgCount The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdReadPerfTest(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    _Bool bfSuccess;
    uint32_t buffer_size;
    uint32_t bytes_remaining;
    uint32_t start_value;
    uint32_t bytes_done;
    uint32_t chunk_size;

    if (iArgCount > 1)
    {
        if (pCom->working_drive > 0)
        {
            int iCount = 1;
            sscanf(ppszArgument[2], "%d", &iCount);

            do
            {
                int iFile;

                iCount--;
                bfSuccess = false;

                cmdMakeReadTestFilePath(pCom->temp_file1, ppszArgument[1], pCom);

                fprintf(pCom->p_out, "Opening file \"%s\"\r\n", pCom->temp_file1);
                fflush(pCom->p_out);

                iFile = open(pCom->temp_file1, O_RDWR, _IONBF);

                if (iFile > 0)
                {
                    uint32_t uiFileSize = 0U;

                    /* Get the file size */
                    control(iFile, CTL_FILE_SIZE, &uiFileSize);

                    if (uiFileSize > 0)
                    {
                        if (uiFileSize > MAX_BUFFER_SIZE)
                        {
                            buffer_size = MAX_BUFFER_SIZE;
                        }
                        else
                        {
                            buffer_size = uiFileSize;
                        }

                        /* Allocate the data for the file and cast to uint8_t * */
                        uint8_t * pbyData = (uint8_t *) R_OS_AllocMem(buffer_size, R_REGION_LARGE_CAPACITY_RAM);

                        if (pbyData)
                        {
                            int iReadResult;

                            fprintf(pCom->p_out, "Reading %lu bytes\r\n", uiFileSize);
                            fflush(pCom->p_out);

                            start_timer();

                            bytes_remaining = uiFileSize;
                            start_value = 0;
                            bytes_done = 0;
                            _Bool data_validated = true;

                            do
                            {
                                if (bytes_remaining > MAX_BUFFER_SIZE)
                                {
                                    chunk_size = MAX_BUFFER_SIZE;
                                }
                                else
                                {
                                    chunk_size = bytes_remaining;
                                }

                                iReadResult = read(iFile, pbyData, chunk_size);

                                if (iReadResult <= 0)
                                {
                                    fprintf(pCom->p_out, "Error in read (%u)\r\n", iReadResult);
                                    fflush(pCom->p_out);
                                }
                                else
                                {
                                    /* only output the first failed data match */
                                    if (data_validated)
                                    {
                                        if (!cmdCheckFileData((uint32_t *) pbyData, (size_t)(chunk_size / sizeof(uint32_t)), &start_value, pCom->p_out))
                                        {
                                            data_validated = false;
                                        }
                                    }

                                    bytes_remaining -= (uint_t) iReadResult;
                                    bytes_done += (uint_t) iReadResult;
                                }

                                update_progress_bar(bytes_done, uiFileSize, "File read", pCom->p_out);
                            }
                            while ((bytes_remaining > 0) && (iReadResult >= 0));

                            float fTime = stop_timer(pCom->p_out);

                            if (bytes_remaining == 0)
                            {
                                cmd_show_data_rate(pCom->p_out, fTime, (size_t) uiFileSize);

                                if (data_validated)
                                {
                                    fprintf(pCom->p_out, "Data match OK\r\n");
                                    bfSuccess = true;
                                }
                            }
                            else
                            {
                                fprintf(pCom->p_out, "File read failed\r\n");
                            }

                            R_OS_FreeMem(pbyData);
                        }
                        else
                        {
                            fprintf(pCom->p_out, "Failed to allocate memory %lu for file\r\n", uiFileSize);
                        }
                    }
                    else
                    {
                        fprintf(pCom->p_out, "File size is zero!\r\n");
                    }

                    /* Close the file */
                    close(iFile);
                }
                else
                {
                    fprintf(pCom->p_out, "File not found\r\n");
                }

            /* Repeat the test several times if required */
            }
            while ((iCount > 0) && (bfSuccess));
        }
        else
        {
            fprintf(pCom->p_out, "No working drive selected\r\n");
        }
    }
    else
    {
        fprintf(pCom->p_out, "File name required\r\n");
    }

    fflush(pCom->p_out);
    return CMD_OK;
}
/******************************************************************************
 End of function cmdReadPerfTest
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdMakeWRTestFileName
 Description:   Function to make a file name based on the file size
 Arguments:     OUT  pszDestFileName - Pointer to the destination file name
                                       string
                IN   fFileSize - The desired file size
                IN   pCom - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void cmdMakeWRTestFileName(char *pszDestFileName, float fFileSize, pst_comset_t pCom)
{
    const char * pszEngMult = gpszEngMultChars;
    static const char * const pszFormat = "%c:%s%s%s%d%c";
    char *leading_backslash;
    char *trailing_backslash;
    size_t working_directory_length;

    while (fFileSize >= 1024.0f)
    {
        fFileSize /= 1024.0f;
        pszEngMult++;
    }

    leading_backslash = (pCom->working_dir[0] == '\\') ? "" : "\\";

    working_directory_length = strlen(pCom->working_dir);

    if ((working_directory_length > 0) && ('\\' != pCom->working_dir[working_directory_length - 1]))
    {
        trailing_backslash = "\\";
    }
    else
    {
        trailing_backslash = "";
    }

    sprintf(pszDestFileName, pszFormat, pCom->working_drive, leading_backslash, pCom->working_dir, trailing_backslash, (int) fFileSize, *pszEngMult);
}
/******************************************************************************
 End of function cmdMakeWRTestFileName
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdMakeReadTestFilePath
 Description:   Function to make a file name based on the file name
 Arguments:     OUT  pszDestFileName - Pointer to the destination file name
                                       string
                IN   name - The filename
                IN   pCom - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void cmdMakeReadTestFilePath(char *pszDestFileName, char *name, pst_comset_t pCom)
{
    static const char * const pszFormat = "%c:%s%s%s%s";
    char *leading_backslash;
    char *trailing_backslash;
    size_t working_directory_length;

    leading_backslash = (pCom->working_dir[0] == '\\') ? "" : "\\";

    working_directory_length = strlen(pCom->working_dir);

    if ((working_directory_length > 0) && ('\\' != pCom->working_dir[working_directory_length - 1]))
    {
        trailing_backslash = "\\";
    }
    else
    {
        trailing_backslash = "";
    }

    sprintf(pszDestFileName, pszFormat, pCom->working_drive, leading_backslash, pCom->working_dir, trailing_backslash, name);
}
/******************************************************************************
 End of function cmdMakeReadTestFilePath
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdFillFileData
 Description:   Function to fill the file data buffer with known data
 Arguments:     OUT pdwFileData - Pointer to the file data
                IN  stLength - The length of the file in DWORDS
 Return value:  none
 ******************************************************************************/
static uint32_t cmdFillFileData(uint32_t * pdwFileData, size_t stLength, uint32_t start_value)
{
    uint32_t dwData = start_value;

    while (stLength--)
    {
        *pdwFileData++ = dwData++;
    }

    return dwData;
}
/******************************************************************************
 End of function cmdFillFileData
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdWriteReadPerfTest
 Description:   Command to perform a write read performance test
 Arguments:     IN  iArgCount The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdWriteReadPerfTest(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;

    float fTestFileSize = 1.0f;
    uint32_t uiTestFileSize;
    uint32_t buffer_size;
    uint32_t bytes_remaining;
    uint8_t * pbyFileData;

    /* A working drive must have been selected */
    if (pCom->working_drive == -1)
    {
        fprintf(pCom->p_out, "No working disk\r\n");
        fflush(pCom->p_out);
        return CMD_OK;
    }

    /* Get the desired file size */
    sscanf(ppszArgument[1], "%f", &fTestFileSize);

    /* Calculate the test file size - always a multiple of four bytes */
    fTestFileSize = fTestFileSize * 1024.0f * 1024.0f;

    /* Limit to a maximum size */
    if (fTestFileSize > MAX_TEST_FILE_SIZE)
    {
        fTestFileSize = MAX_TEST_FILE_SIZE;
    }

    /* Limit to a minimum file size */
    if (fTestFileSize < sizeof(uint32_t))
    {
        fTestFileSize = (float) sizeof(uint32_t);
    }

    uiTestFileSize = (uint32_t)(fTestFileSize);
    uiTestFileSize += (uiTestFileSize % sizeof(uint32_t));

    /* Make a file name */
    cmdMakeWRTestFileName(pCom->temp_file1, fTestFileSize, pCom);

    if (uiTestFileSize > MAX_BUFFER_SIZE)
    {
        buffer_size = MAX_BUFFER_SIZE;
    }
    else
    {
        buffer_size = uiTestFileSize;
    }

    /* Allocate the data for the file and cast to uint8_t * */
    pbyFileData = (uint8_t *) R_OS_AllocMem(buffer_size, R_REGION_LARGE_CAPACITY_RAM);

    fprintf(pCom->p_out, "Temp file stored in address 0x%lx, size requested [0x%lx bytes]\r\n", (unsigned long) pbyFileData, uiTestFileSize);
    fflush(pCom->p_out);

    if (pbyFileData)
    {
        int_t iFile;

        fprintf(pCom->p_out, "Opening file %s to write %lu bytes\r\n", pCom->temp_file1, uiTestFileSize);
        fflush(pCom->p_out);

        /* Open the file for writing */
        iFile = open(pCom->temp_file1, O_WRONLY | O_TRUNC, _IONBF);

        if (iFile > 0)
        {
            int_t iResult;
            uint32_t start_value;
            uint32_t bytes_done;
            uint32_t chunk_size;
            float fTime;

            fprintf(pCom->p_out, "Writing file\r\n");
            fflush(pCom->p_out);

            bytes_remaining = uiTestFileSize;
            start_value = 0;
            bytes_done = 0;

            start_timer();

            do
            {
                if (bytes_remaining > MAX_BUFFER_SIZE)
                {
                    chunk_size = MAX_BUFFER_SIZE;
                }
                else
                {
                    chunk_size = bytes_remaining;
                }

                /* Fill with known data */
                start_value = cmdFillFileData((uint32_t *) pbyFileData, (size_t) (chunk_size / sizeof(uint32_t)), start_value);

                /* write the data */
                iResult = write(iFile, pbyFileData, chunk_size);

                if (iResult > 0)
                {
                    bytes_remaining -= (uint_t) iResult;
                    bytes_done += (uint_t) iResult;
                }

                update_progress_bar(bytes_done, uiTestFileSize, "File written", pCom->p_out);
            }
            while ((bytes_remaining > 0) && (iResult >= 0));

            close(iFile); /* To flush out buffers */

            /* Stop the timer */
            fTime = stop_timer(pCom->p_out);

            /* Check write was successful */
            if (0 == bytes_remaining)
            {
                /* Print the data rate */
                cmd_show_data_rate(pCom->p_out, fTime, (size_t) uiTestFileSize);

                /* open the file for reading */
                iFile = open(pCom->temp_file1, O_RDONLY, _IONBF);

                if (iFile > 0)
                {
                    /* seek to the beginning of the file */
                    FILESEEK fileSeek;
                    fileSeek.lOffset = 0L;
                    fileSeek.iBase = SEEK_SET;
                    control(iFile, CTL_FILE_SEEK, &fileSeek);

                    fprintf(pCom->p_out, "Reading file\r\n");
                    fflush(pCom->p_out);

                    /* read the data back in */
                    bytes_remaining = uiTestFileSize;
                    start_value = 0;
                    bytes_done = 0;
                    _Bool data_validated = true;

                    start_timer();

                    do
                    {
                        if (bytes_remaining > MAX_BUFFER_SIZE)
                        {
                            chunk_size = MAX_BUFFER_SIZE;
                        }
                        else
                        {
                            chunk_size = bytes_remaining;
                        }

                        iResult = read(iFile, pbyFileData, chunk_size);

                        if (iResult <= 0)
                        {
                            fprintf(pCom->p_out, "Error in read (%u)\r\n", iResult);
                            fflush(pCom->p_out);
                        }
                        else
                        {
                            /* only output the first failed data match */
                            if (data_validated)
                            {
                                if (!cmdCheckFileData((uint32_t *) pbyFileData, (size_t) (chunk_size / sizeof(uint32_t)), &start_value, pCom->p_out))
                                {
                                    data_validated = false;
                                }
                            }

                            bytes_remaining -= (uint_t) iResult;
                            bytes_done += (uint_t) iResult;
                        }

                        update_progress_bar(bytes_done, uiTestFileSize, "File read", pCom->p_out);
                    }
                    while ((bytes_remaining > 0) && (iResult >= 0));

                    fTime = stop_timer(pCom->p_out);

                    if (bytes_remaining == 0)
                    {
                        /* print the data rate */
                        cmd_show_data_rate(pCom->p_out, fTime, (size_t) uiTestFileSize);

                        if (data_validated)
                        {
                            fprintf(pCom->p_out, "File data OK\r\n");
                        }
                        else
                        {
                            fprintf(pCom->p_out, "Error in file data\r\n");
                        }
                    }
                    else
                    {
                        fprintf(pCom->p_out, "Error in read file\r\n");
                    }

                    close(iFile);
                }
                else
                {
                    fprintf(pCom->p_out, "Error failed to open file for read\r\n");
                }
            }
            else
            {
                fprintf(pCom->p_out, "Error in write %d\r\n", iResult);
            }
        }
        else
        {
            fprintf(pCom->p_out, "Failed to open file for write\r\n");
        }

        R_OS_FreeMem(pbyFileData);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to allocate memory for the file data\r\n");
    }

    fflush(pCom->p_out);

    return CMD_OK;
}
/******************************************************************************
 End of function cmdWriteReadPerfTest
 ******************************************************************************/

/* Table that associates command letters, function pointer and a little
   description of what the command does */
st_cmdfnass_t gpcmdUsbMassStorageTest[] =
{
     {
        "rperf",
        cmdReadPerfTest,
        "f<CR> - Read file performance test",
     },
     {
          "wrperf",
          cmdWriteReadPerfTest,
          "s<CR> - Write file performance test",
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t gcmdUsbMassStorageTest =
{
    "USB MS Test Commands",
    gpcmdUsbMassStorageTest,
    (sizeof(gpcmdUsbMassStorageTest) / sizeof(st_cmdfnass_t))
};

/******************************************************************************
 End Of File
 ******************************************************************************/
