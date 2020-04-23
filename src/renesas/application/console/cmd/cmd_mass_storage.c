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
 * File Name    : cmdMassStorage.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : N/A
 * OS           : N/A
 * H/W Platform : RSK+
 * Description  : The commands for support of MS devices
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.08.2009 1.00 First Release
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
#include "control.h"
#include "r_devlink_wrapper_cfg.h"
#include "wild_compare.h"
#include "dskManager.h"
#include "r_fatfs_abstraction.h"
#include "wild_compare.h"
#include "trace.h"
#include "ff.h"
#include "cmdMassStorage.h"
#include "r_os_abstraction_api.h"

/******************************************************************************
 Defines
 ******************************************************************************/
/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
    #undef TRACE
    #define TRACE(x)
#endif

/* Define the number of characters printed on a line for the "mem" command */
#define TEMP_FILE_BUFFER_SIZE   (4096)

/******************************************************************************
 Constant Data
 ******************************************************************************/

const st_command_table_t gcmdUsbMassStorage;

static const char * const gpszFatType[] =
{ "FAT12", "FAT16", "FAT32", "EXFAT", "UNKNOWN_FORMAT" };

/* Define the engineering exp symbols for Kilo, Mega, Giga, Tera, Peta,
 Exa, Zetta and Yotta */
static const char *gpszEngMultChars = "#kMGTPEZY";

/******************************************************************************
 External functions
 ******************************************************************************/

extern uint8_t *cmdViewMemory (uint8_t *pbyView, uint8_t bLength, FILE *p_out);
extern uint8_t *cmdShowBin (uint8_t *pbyView, size_t iLength, size_t stOffset, FILE *p_out);
extern void cmdShowDataRate (FILE *pFile, float fTransferTime, size_t stLength);

/******************************************************************************
 Public functions
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdTrimSlash
 Description:   Function to make sure a path string has a \ on the front and
 not on the end.
 Arguments:     OUT pszDest - Pointer to the destination
 IN  pszSrc - Pointer to the source
 Return value:  none
 ******************************************************************************/
void cmdTrimSlash (char *pszDest, char *pszSrc)
{
    char *pszSlash = NULL;

    /* Make sure there is always a slash on the root */
    if ( *pszSrc != '\\')
    {
        *pszDest++ = '\\';
    }

    /* Copy the root string and remember the last slash */
    while ( *pszSrc)
    {
        if ( *pszSrc == '\\')
        {
            pszSlash = pszDest;
        }

        *pszDest++ = *pszSrc++;
    }

    /* If the last slash was on the end */
    if ((pszSlash) && ((pszDest - pszSlash) == 1))
    {
        /* Remove the slash and terminate the string */
        pszDest--;
        *pszDest = '\0';
    }
    else
    {
        /* Terminate the string */
        *pszDest = '\0';
    }
}
/******************************************************************************
 End of function  cmdTrimSlash
 ******************************************************************************/


/***********************************************************************************************************************
 * Function Name: check_console_type
 * Description  : Check which type of console is active in calling function
 * Arguments    : IN  p_out - output context
 * Return Value :
 *                CTL_STREAM_TCP : Telnet console
 *                CMD_OK         : Serial console
 **********************************************************************************************************************/
static uint32_t check_console_type(FILE *p_out)
{
    uint32_t ret = CMD_OK;

    if (control(R_DEVLINK_FilePtrDescriptor(p_out), CTL_STREAM_TCP, 0) == 0)
    {
        /* Print a error message */
        fprintf(p_out,"Unable to execute, command can only be launched from serial console.\r\n");
        fflush(p_out);
        ret = CTL_STREAM_TCP;
    }
    return (ret);
}
/***********************************************************************************************************************
 End of function check_console_type
 **********************************************************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: update_progress_bar
 Description:   Function to display a progress indicator during file copies
 Arguments:     IN  bytes_done  - Number of bytes completed
 IN  total_bytes - Total number of bytes in the file being copied
 IN  pCom        - Pointer to the command object
 Return value:  none
 ******************************************************************************/
void update_progress_bar (uint32_t bytes_done, uint32_t total_bytes, const char *finished_string, FILE *p_out)
{
    static uint8_t spinner = 0;
    const char spin_chars[4] =
    { '-', '\\', '|', '/' };
    uint64_t percentage;

    /* no point of progress indicator for really small files */
    if (total_bytes < (1024 * 128))
    {
        return;
    }

    /* calculation performed in steps to avoid truncation during type conversions */

    if (bytes_done == total_bytes)
    {
        fprintf(p_out, "\r%s\r\n", finished_string);
    }
    else
    {
        percentage = bytes_done;
        percentage *= 100;
        percentage /= total_bytes;

        fprintf(p_out, "\r %c  %2ld%% ", spin_chars[spinner], (uint32_t) percentage);

        spinner++;
        if (spinner > 3)
        {
            spinner = 0;
        }
    }

    fflush(p_out);
}
/******************************************************************************
 End of function update_progress_bar
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdTypeFile
 Description:   Function to print the contents of a text file to the console.
 This is to demonstrate the C standard file IO functions
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmdTypeFile (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    AVOID_UNUSED_WARNING;
    FILE *pFile;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount > 1)
    {
        if (pCom->working_drive > 0)
        {
            if (NULL != strstr(ppszArgument[1], ":\\"))
            {
                /*No need for path manipulation */
                pFile = fopen(ppszArgument[1], "r");
            }
            else
            {

                /* Format the file name sting including the current path */
                sprintf(pCom->temp_file1, "%c:\\%s", pCom->working_drive, pCom->working_dir);
                if (pCom->temp_file1[(strlen(pCom->temp_file1) - 1)] != '\\')
                {
                    strcat(pCom->temp_file1, "\\");
                }

                /* Add the file name on the end */
                strcat(pCom->temp_file1, ppszArgument[1]);

                fprintf(pCom->p_out, "Opening file \"%s\"\r\n", pCom->temp_file1);
                fflush(pCom->p_out);
            }

            pFile = fopen(pCom->temp_file1, "r");

            if (pFile)
            {
                /* Allocate a data buffer */
                uint8_t *pbyData = (uint8_t *) R_OS_AllocMem(TEMP_FILE_BUFFER_SIZE, R_REGION_LARGE_CAPACITY_RAM);
                if (pbyData)
                {
                    size_t stRead;
                    do
                    {
                        /* Read the data */
                        stRead = fread(pbyData, sizeof(uint8_t),
                        TEMP_FILE_BUFFER_SIZE, pFile);

                        if (stRead > 0)
                        {
                            fwrite(pbyData, sizeof(uint8_t), stRead, pCom->p_out);
                        }
                    } while (stRead > 0);
                    R_OS_FreeMem(pbyData);
                }

                /* Close the file */
                if (fclose(pFile))
                {
                    fprintf(pCom->p_out, "Error on close file\r\n");
                }
            }
            else
            {
                fprintf(pCom->p_out, "File not found\r\n");
            }
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
 End of function  cmdTypeFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdShowBinaryFile
 Description:   Function to show a binary file
 Arguments:     IN  iFile - The file descriptor
 OUT p_out - Pointer to the output file stream
 Return value:  none
 ******************************************************************************/
static void cmdShowBinaryFile (FILE *pFile, FILE *p_out)
{
    uint8_t *pbyData = R_OS_AllocMem(TEMP_FILE_BUFFER_SIZE, R_REGION_LARGE_CAPACITY_RAM);

    if (pbyData)
    {
        size_t stOffset = 0;
        size_t iRead;

        do
        {
            uint8_t *pbyView = pbyData;
            size_t iCount;

            /* Read the data */
            iRead = fread(pbyData, sizeof(uint8_t), TEMP_FILE_BUFFER_SIZE, pFile);

            iCount = iRead;

            /* While there is data to print */
            while (iCount > 0)
            {
                size_t iLength = (iCount > 16) ? 16 : iCount;

                /* Show one line at a time */
                pbyView = cmdShowBin(pbyView, iLength, stOffset, p_out);
                stOffset += iLength;
                iCount -= iLength;
            }
        } while (iRead > 0);

        R_OS_FreeMem(pbyData);
    }
    else
    {
        fprintf(p_out, "Failed to allocate memory\r\n");
        fflush(p_out);
    }
}
/******************************************************************************
 End of function  cmdShowBinaryFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd
 File
 Description:   Command to view a file
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdViewFile (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    AVOID_UNUSED_WARNING;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount > 1)
    {
        if (pCom->working_drive > 0)
        {
            FILE *pFile;

            if (NULL != strstr(ppszArgument[1], ":\\"))
            {
                /* No need for path manipulation */
                pFile = fopen(ppszArgument[1], "r");
            }
            else
            {
                fprintf(pCom->p_out, "Drive: [%c] ", pCom->working_drive);
                fprintf(pCom->p_out, "Directory: [%s] ", pCom->working_dir);

                /* Format the file name sting including the current path */
                sprintf(pCom->temp_file1, "%c:\\%s", pCom->working_drive, pCom->working_dir);
                printf(pCom->temp_file1);

                if (pCom->temp_file1[(strlen(pCom->temp_file1) - 1)] != '\\')
                {
                    strcat(pCom->temp_file1, "\\");
                }

                /* Add the file name on the end */
                strcat(pCom->temp_file1, ppszArgument[1]);
                fprintf(pCom->p_out, "Opening file \"%s\"\r\n", pCom->temp_file1);
                fflush(pCom->p_out);

                pFile = fopen(pCom->temp_file1, "r");
            }

            if (pFile)
            {
                /* Print the file to the screen */
                cmdShowBinaryFile(pFile, pCom->p_out);

                /* Close the file */
                if (fclose(pFile))
                {
                    fprintf(pCom->p_out, "Error on close file\r\n");
                }
            }
            else
            {
                fprintf(pCom->p_out, "File not found\r\n");
            }
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
 End of function  cmdViewFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdPrintDirectoryEntry
 Description:   Function to print a directory entry
 Arguments:     IN  pEntry - Pointer to the FAT directory structure
 OPT p_out - Pointer to the file stream to print to
 Return value:  none
 ******************************************************************************/
static void cmdPrintDirectoryEntry (PFATENTRY pEntry, FILE *p_out)
{
    char pszAttrubute[] = "----";
    if (pEntry->Attrib & FAT_ATTR_READONLY)
    {
        pszAttrubute[0] = 'R';
    }
    if (pEntry->Attrib & FAT_ATTR_HIDDEN)
    {
        pszAttrubute[1] = 'H';
    }
    if (pEntry->Attrib & FAT_ATTR_SYSTEM)
    {
        pszAttrubute[2] = 'S';
    }
    if (pEntry->Attrib & FAT_ATTR_DIR)
    {
        pszAttrubute[3] = 'D';
    }
    fprintf(p_out, "%s %.2d/%.2d/%.4d %.2d:%.2d %9u %s\r\n", pszAttrubute, pEntry->CreateTime.Day,
            pEntry->CreateTime.Month, pEntry->CreateTime.Year, pEntry->CreateTime.Hour, pEntry->CreateTime.Minute,
            pEntry->Filesize, pEntry->FileName);
    fflush(p_out);
}
/******************************************************************************
 End of function  cmdPrintDirectoryEntry
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdListDirectory
 Description:   Command to list the working directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdListDirectory (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    AVOID_UNUSED_WARNING;

    _Bool bfHeaderPrinted = false;
    char full_path[CMD_MAX_PATH];

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (pCom->working_drive > 0)
    {
        /* Try to get a pointer to the drive */
        void *pDrive = dskGetDrive(pCom->working_drive);

        if (pDrive)
        {
            FATENTRY fatEntry;
            FRESULT fatResult;
            int iCount = 0;

            /* Find the first entry */

            DIR dir;

            full_path[0] = pCom->working_drive;
            full_path[1] = ':';
            full_path[2] = '\\';
            full_path[3] = '\0';
            strcat(full_path, pCom->working_dir);

            fatResult = R_FAT_FindFirst( &dir, &fatEntry, full_path, "*");

            if (('\0' == fatEntry.FileName[0]) || (FR_OK != fatResult))
            {
                fatResult = 1;
                fprintf(pCom->p_out, "Nothing found\r\n");
            }

            while (0 == fatResult)
            {
                /* Print the entry */
                if (iArgCount > 1)
                {
                    if (wild_compare(ppszArgument[1], fatEntry.FileName))
                    {
                        if ( !bfHeaderPrinted)
                        {
                            fprintf(pCom->p_out, "Attr Date       Time"
                                    "          Size Name\r\n");
                            bfHeaderPrinted = true;
                        }

                        cmdPrintDirectoryEntry( &fatEntry, pCom->p_out);
                        iCount++;
                    }
                }
                else
                {
                    if ( !bfHeaderPrinted)
                    {
                        fprintf(pCom->p_out, "Attr Date       Time"
                                "          Size Name\r\n");
                        bfHeaderPrinted = true;
                    }

                    cmdPrintDirectoryEntry( &fatEntry, pCom->p_out);
                    iCount++;
                }

                /* Get the next one */
                fatResult = R_FAT_FindNext( &dir, &fatEntry);

                if ('\0' == fatEntry.FileName[0])
                {
                    fatResult = 1;
                }
            }

            if (0 != iCount)
            {
                fprintf(pCom->p_out, "Found %d items\r\n", iCount);
            }
            else
            {
                fprintf(pCom->p_out, "No files found\r\n");
            }
        }
        else
        {
            fprintf(pCom->p_out, "Drive \"%c\" not found\r\n", pCom->working_drive);
            pCom->working_drive = -1;
            pCom->p_prompt = pCom->default_prompt;
        }
    }
    else
    {
        fprintf(pCom->p_out, "No working drive selected\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdListDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdPrintWorkingDirectory
 Description:   Command to print the working directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdPrintWorkingDirectory (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;
    (void) ppszArgument;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (pCom->working_drive > 0)
    {
        fprintf(pCom->p_out, "%s\r\n", pCom->working_dir);
    }
    else
    {
        fprintf(pCom->p_out, "No working drive selected\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdPrintWorkingDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdRemoveFirstInstance
 Description:   Function to remove a sub string from a path string
 Arguments:     OUT pszDest Pointer to the destination
 IN  pszSrc - Pointer to the source string
 IN  pszDel - Pointer to the string to delete
 Return value:  Pointer to the removed instance
 ******************************************************************************/
static char *cmdRemoveFirstInstance (char *pszDest, char *pszSrc, char *pszDel)
{
    char *pszRemove = NULL;
    pszRemove = strstr(pszSrc, pszDel);
    if (pszRemove)
    {
        if (pszRemove != pszSrc)
        {
            strncpy(pszDest, pszSrc, (size_t) (pszRemove - pszSrc));
        }
        else
        {
            *pszDest = '\0';
        }
        pszRemove += strlen(pszDel);
        if ( *pszRemove)
        {
            strcat(pszDest, pszRemove);
        }
    }
    else
    {
        strcpy(pszDest, pszSrc);
    }
    return pszRemove;
}
/******************************************************************************
 End of function  cmdRemoveFirstInstance
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdCleanPath
 Description:   Function to clean the global variable "pCom->temp_file2" string of
 unwanted slashes and dots
 Arguments:     IN  pCom - Pointer to the command object
 Return value:  none
 ******************************************************************************/
static void cmdCleanPath (pst_comset_t pCom)
{
    /* Remove "\.\" */
    memset(pCom->temp_file1, 0, sizeof(pCom->temp_file1));
    cmdRemoveFirstInstance(pCom->temp_file1, pCom->temp_file2, "\\.\\");
    /* Remove "\\" */
    memset(pCom->temp_file2, 0, sizeof(pCom->temp_file2));
    cmdRemoveFirstInstance(pCom->temp_file2, pCom->temp_file1, "\\\\");
}
/******************************************************************************
 End of function  cmdCleanPath
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdToUpper
 Description:   Function to make a string upper case
 Arguments:     IN/OUT pszString - Pointer to the string
 Return value:  none
 ******************************************************************************/
static void cmdToUpper (char *pszString)
{
    while ( *pszString)
    {
        *pszString = (char) toupper( *pszString);
        pszString++;
    }
}
/******************************************************************************
 End of function  cmdToUpper
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdChangeDirectory
 Description:   Command to change the working directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmdChangeDirectory (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;
    char full_path[256];
    memset(full_path, 0, 255);

    void *pDrive = dskGetDrive(pCom->working_drive);

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (pDrive)
    {
        /* Path that starts with a slash means from the root */
        if ( *ppszArgument[1] == '\\')
        {
            /* So copy the whole line in */
            strcpy(pCom->temp_file2, ppszArgument[1]);
        }
        else
        {
            /* Append the new part to the current working path */
            sprintf(pCom->temp_file2, "%s%s", pCom->working_dir, ppszArgument[1]);
        }

        /* Roundup the path for dots and slashes */
        cmdCleanPath(pCom);

        if (0 != strstr(ppszArgument[1], ":\\"))
        {
            strcat(full_path, ppszArgument[1]);
        }
        else
        {

            full_path[0] = pCom->working_drive;
            full_path[1] = ':';

            if (pCom->working_dir[0] != '\\')
            {
                strcat(full_path, "\\");
                strcat(full_path, pCom->working_dir);
                strcat(full_path, "\\");
                strcat(full_path, ppszArgument[1]);
            }
            else
            {
                strcat(full_path, pCom->working_dir);
                strcat(full_path, ppszArgument[1]);
            }
        }

        /* See if the directory exists */
        if (R_FAT_FindDirectory(full_path, pCom->temp_file1))
        {
            /* Trim the slash off the end */
            cmdTrimSlash(pCom->temp_file2, pCom->temp_file1);

            /* Make it all upper case */
            cmdToUpper(pCom->temp_file2);
            if ( ! *pCom->temp_file2)
            {
                strcpy(pCom->temp_file2, "\\");
            }

            /* Copy to the current working directory */
            strcpy(pCom->working_dir, pCom->temp_file1);

            /* Format a prompt with the path in */
            sprintf(pCom->working_dir_prompt, "%c:%s>", pCom->working_drive, pCom->temp_file2);
        }
        else
        {
            fprintf(pCom->p_out, "Path \"%s\" not found\r\n", ppszArgument[1]);
        }
    }
    else
    {
        if (pCom->working_drive > 0)
        {
            fprintf(pCom->p_out, "Drive \"%c\" not found\r\n", pCom->working_drive);
            pCom->p_prompt = pCom->default_prompt;
            pCom->working_drive = -1;
        }
        else
        {
            fprintf(pCom->p_out, "No working disk\r\n");
        }
    }

    /* the working directory should not be an empty string */
    if ('\0' == pCom->working_dir[0])
    {
    	pCom->working_dir[0] = '\\';
    	pCom->working_dir[1] = '\0';
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdChangeDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdDeleteFile
 Description:   Command to delete a file
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdDeleteFile (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    AVOID_UNUSED_WARNING;
    char full_path[CMD_MAX_PATH];
    FRESULT fatResult;
    int iCount = 0;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount < 1)
    {
        fprintf(pCom->p_out, "Need one argument\r\n");
        return 0;
    }

    if (pCom->working_drive > 0)
    {
        /* Try to get a pointer to the drive */
        void *pDrive = dskGetDrive(pCom->working_drive);

        if (pDrive)
        {
            FATENTRY fatEntry;
            DIR dir;

            memset(full_path, 0, sizeof(full_path));
            full_path[0] = pCom->working_drive;
            full_path[1] = ':';
            full_path[2] = '\0';
            strcat(full_path, "\\");
            strcat(full_path, pCom->working_dir);

            if (NULL != strstr(ppszArgument[1], "*"))
            {
                fatResult = R_FAT_FindFirst( &dir, &fatEntry, full_path, "*");

                if ((fatEntry.FileName[0] == '\0') || (FR_OK != fatResult))
                {
                    fatResult = 1;
                    fprintf(pCom->p_out, "Nothing found\r\n");
                }

                while (0 == fatResult)
                {
                    /* Print the entry */
                    printf("Filename: %s \n\r", fatEntry.FileName);
                    if (wild_compare(ppszArgument[1], fatEntry.FileName))
                    {
                        fatResult = R_FAT_RemoveFile(fatEntry.FileName);
                        iCount++;
                    }

                    /* Get the next one */
                    fatResult = R_FAT_FindNext( &dir, &fatEntry);

                    if ('\0' == fatEntry.FileName[0])
                    {
                        fatResult = 1;
                    }
                }
            }
            else
            {
                if (NULL != strstr(ppszArgument[1], ":\\"))
                {
                    strcpy(full_path, ppszArgument[1]);
                }
                else
                {
                    full_path[0] = pCom->working_drive;
                    full_path[1] = ':';
                    full_path[2] = '\0';
                    strcat(full_path, "\\");
                    strcat(full_path, ppszArgument[1]);
                }

                fatResult = R_FAT_RemoveFile(full_path);
                if (FR_OK == fatResult)
                {
                    iCount++;
                }
            }
        }
    }
    else
    {
        fprintf(pCom->p_out, "No working drive selected\r\n");
    }

    if (iCount)
    {
        fprintf(pCom->p_out, "\r\nDeleted %d items\r\n", iCount);
    }
    else
    {
        fprintf(pCom->p_out, "No files found\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdDeleteFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmsMakeDirectory
 Description:   Command to make a directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmsMakeDirectory (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    PDRIVE pDrive;
    char full_path[256];
    memset(full_path, 0, 255);

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount < 2)
    {
        fprintf(pCom->p_out, "File name required\r\n");
        return CMD_OK;
    }
    if (pCom->working_drive < 0)
    {
        fprintf(pCom->p_out, "No working disk\r\n");
        return CMD_OK;
    }

    if (0 != strstr(ppszArgument[1], ":\\"))
    {
        strcat(full_path, ppszArgument[1]);
    }
    else
    {
        full_path[0] = pCom->working_drive;
        full_path[1] = ':';

        if (pCom->working_dir[0] != 0x5c)
        {
            strcat(full_path, "\\");
            strcat(full_path, pCom->working_dir);
            strcat(full_path, "\\");
            strcat(full_path, ppszArgument[1]);
        }
        else
        {
            strcat(full_path, pCom->working_dir);
            strcat(full_path, ppszArgument[1]);
        }
    }

    /* Try to get a pointer to the drive */
    pDrive = dskGetDrive(pCom->working_drive);
    if (pDrive)
    {

        FATERR fatResult = R_FAT_MakeDirectory(full_path);

        if (fatResult)
        {

            fprintf(pCom->p_out, "Failed to make dir [%s]\n\r", ppszArgument[1]);
        }
        else
        {
            fprintf(pCom->p_out, "Created [%s] directory\n\r", ppszArgument[1]);
        }
    }
    else
    {
        fprintf(pCom->p_out, "Working drive is invalid\r\n");
        pCom->p_prompt = pCom->default_prompt;
        pCom->working_drive = -1;
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmsMakeDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdDeleteDirectory
 Description:   Command to delete a directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdDeleteDirectory (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    PDRIVE pDrive;
    FATERR fatResult;
    char full_path[256];
    memset(full_path, 0, 255);

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount < 2)
    {
        fprintf(pCom->p_out, "File name required\r\n");
        return CMD_OK;
    }
    if (pCom->working_drive < 0)
    {
        fprintf(pCom->p_out, "No working disk\r\n");
        return CMD_OK;
    }

    if (0 != strstr(ppszArgument[1], ":\\"))
     {
         strcat(full_path, ppszArgument[1]);
     }
     else
     {
         full_path[0] = pCom->working_drive;
         full_path[1] = ':';

         if (pCom->working_dir[0] != 0x5c)
         {
             strcat(full_path, "\\");
             strcat(full_path, pCom->working_dir);
             strcat(full_path, "\\");
             strcat(full_path, ppszArgument[1]);
         }
         else
         {
             strcat(full_path, pCom->working_dir);
             strcat(full_path, ppszArgument[1]);
         }
     }

    /* Try to get a pointer to the drive */
    pDrive = dskGetDrive(pCom->working_drive);
    if (pDrive)
    {

        fatResult = R_FAT_RemoveDirectory(full_path);

        if (fatResult)
        {
            fprintf(pCom->p_out, "Failed to delete directory [%s]\n\r", ppszArgument[1]);
        }
        else
        {
            fprintf(pCom->p_out, "Directory deleted\r\n");
        }
    }
    else
    {
        fprintf(pCom->p_out, "Working drive is invalid\r\n");
        pCom->p_prompt = pCom->default_prompt;
        pCom->working_drive = -1;
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdDeleteDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdRename
 Description:   Function to rename a file or directory
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 ******************************************************************************/
static int16_t cmdRename (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    PDRIVE pDrive;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    if (iArgCount < 3)
    {
        fprintf(pCom->p_out, "Source and destination file names required\r\n");
        return CMD_OK;
    }
    if (pCom->working_drive < 0)
    {
        fprintf(pCom->p_out, "No working disk\r\n");
        return CMD_OK;
    }
    /* Try to get a pointer to the drive */
    pDrive = dskGetDrive(pCom->working_drive);
    if (pDrive)
    {
        FATERR fatResult;

        strcpy(pCom->temp_file1, ppszArgument[1]);
        /* Copy the working directory to a temp string */

        strcpy(pCom->temp_file2, ppszArgument[2]);
        /* Try to re-name the file */

        fatResult = R_FAT_ReName(pCom->temp_file1, pCom->temp_file2);

        if (fatResult)
        {
            fprintf(pCom->p_out, "Rename operation failed\n\r");
        }
        else
        {

            printf("Temp file1 [%s]", pCom->temp_file1);
            printf("Temp file2 [%s]", pCom->temp_file2);
            fprintf(pCom->p_out, "OK\r\n");
        }
    }
    else
    {
        fprintf(pCom->p_out, "Working drive is invalid\r\n");
        pCom->p_prompt = pCom->default_prompt;
        pCom->working_drive = -1;
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdRename
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdGetDrivePathFile
 Description:   Function to get the full drive path and file from an argument
 Arguments:     OUT pszDrivePathFile - Pointer to the destination
 IN  pszArgument - Pointer to the file name argument
 IN  pCom - Pointer to the command object
 Return value:  true if it looks valid
 ******************************************************************************/
static _Bool cmdGetDrivePathFile (char *pszDrivePathFile, char *pszArgument, pst_comset_t pCom)
{
    char *pszColon = memchr(pszArgument, ':', strlen(pszArgument));

    /* If there is a colon in the name then this indicates it is a full drive and path */
    if (pszColon)
    {
        /* If it is only 1 from the beginning then this should be the drive */
        if (((pszColon - pszArgument) == 1)
                /* In that case the next character should be a slash */
                && (pszArgument[2] == '\\'))
        {
            /* Trust that the command line is correct */
            strcpy(pszDrivePathFile, pszArgument);
            printf("pszDrivePathFile: [%s]\r\n", pszDrivePathFile);
            return true;
        }

        /* A colon in a file name is not valid */
        return false;
    }

    if (pCom->working_drive != -1)
    {
        /* If the name starts with a \ */
        if (*pszArgument == '\\')
        {
            /* Make into full drive and path - assuming it is correct */
            sprintf(pszDrivePathFile, "%c:%s", pCom->working_drive, pszArgument);
        }
        else
        {
            /* Use the current drive and working directory */
            if (*pCom->working_dir == '\\')
            {
                /* Root folder */
                sprintf(pszDrivePathFile, "%c:%s%s", pCom->working_drive, pCom->working_dir, pszArgument);
            }
            else
            {
                /* Sub folder */
                sprintf(pszDrivePathFile, "%c:\\%s\\%s", pCom->working_drive, pCom->working_dir, pszArgument);
            }
        }

        printf("pszDrivePathFile: [%s]\r\n", pszDrivePathFile);
        return true;
    }

    return false;
}
/******************************************************************************
 End of function  cmdGetDrivePathFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdGetPointerToLastSlash
 Description:   Function to get a pointer to the last slash in a fila path
 Arguments:     IN  pszDrivePathAndName - Pointer to the drive path and name
 Return value:  Pointer to slash file name or NULL if not found
 ******************************************************************************/
static char *cmdGetPointerToLastSlash (char *pszDrivePathAndName)
{
    char *pLastSlash = NULL;
    while ( *pszDrivePathAndName)
    {
        if ( *pszDrivePathAndName == '\\')
        {
            pLastSlash = pszDrivePathAndName;
        }
        pszDrivePathAndName++;
    }
    return pLastSlash;
}
/******************************************************************************
 End of function  cmdGetPointerToLastSlash
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdIsDirectory
 Description:   Function to find out if a path is to a directory
 Arguments:     IN  pDrive - Pointer to the FAT IOMAN object
 IN  pszPath - Pointer to a path
 Return value:  true if it is a directory
 ******************************************************************************/
static _Bool cmdIsDirectory (PDRIVE pDrive, char *pszPath)
{
    (void) pDrive;

    TCHAR buffer[CMD_MAX_PATH];

    f_getcwd(buffer, CMD_MAX_PATH);
    FRESULT result = f_chdir(pszPath);
    f_chdir(buffer);

    return (result == FR_OK);
}
/******************************************************************************
 End of function  cmdIsDirectory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdCopySingleFile
 Description:   Function to copy a single file
 Arguments:     IN  pszSrc - Pointer to the source drive path and file string
 IN  pszDest - Pointer to the destination drive path and file string
 IN  pvBuffer - Pointer to a buffer to use for the copy
 IN  stLength - The length of the buffer in bytes
 Return value:  true if the file was copied
 ******************************************************************************/
static _Bool cmdCopySingleFile (char *pszSrc, char *pszDest, void *pvBuffer, size_t stLength, FILE *p_out)
{
    _Bool bfResult = true;
    uint32_t bytes_done;
    uint32_t total_bytes;

    if (0 != strcmp(pszSrc, pszDest))
    {
        int iSrc;

        /* Open the source file for read */
        iSrc = open(pszSrc, O_RDONLY);

        /* Get the file size */
        control(iSrc, CTL_FILE_SIZE, &total_bytes);
        bytes_done = 0;

        if (iSrc > 0)
        {
            /* Open the destination file for write */
            int iDest = open(pszDest, O_WRONLY | O_TRUNC, _IONBF);

            if (iDest > 0)
            {
                int iRead;

                do
                {
                    /* Read the data from the source file */
                    iRead = read(iSrc, pvBuffer, (uint32_t) stLength);

                    if (iRead > 0)
                    {
                        bytes_done += (unsigned int) iRead;
                        update_progress_bar(bytes_done, total_bytes, "File copied", p_out);

                        int iWrite = write(iDest, pvBuffer, (uint32_t) iRead);
                        if (iRead != iWrite)
                        {
                            /* Stop the copy */
                            iRead = 0;
                            bfResult = false;
                        }
                    }
                    else if (iRead < 0)
                    {
                        bfResult = false;
                    }

                } while (iRead > 0);

                /* Close the destination file */
                close(iDest);
            }
            else
            {
                bfResult = false;
            }

            /* Close the source file */
            close(iSrc);
        }
        else
        {
            bfResult = false;
        }
    }
    else
    {
        bfResult = false;
    }

    return bfResult;
}
/******************************************************************************
 End of function  cmdCopySingleFile
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdCopyPath
 Description:   Function to copy the path only
 Arguments:     OUT pszPath - Pointer to the destination path string
 IN  pszPathAndFileName - Pointer to a path and file name
 Return value:  none
 ******************************************************************************/
static void cmdCopyPath (char *pszPath, char *pszPathAndFileName)
{
    char *pszLastSlash = cmdGetPointerToLastSlash(pszPathAndFileName);

    while (pszPathAndFileName < pszLastSlash)
    {
        *pszPath++ = *pszPathAndFileName++;
    }

    *pszPath = '\0';
}
/******************************************************************************
 End of function  cmdCopyPath
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdMatchFileSpec
 Description:   Function to match the file spec
 Arguments:     IN  pszFileSpec - Pointer to the file specification
 IN  pDirEntry - Pointer to the directory entry
 Return value:  true if the file spec matches
 ******************************************************************************/
static _Bool cmdMatchFileSpec (char *pszFileSpec, FATENTRY *pDirEntry)
{
    if (((pDirEntry->Attrib & FAT_ATTR_DIR) == 0) && (wild_compare(pszFileSpec, pDirEntry->FileName)))
    {
        return true;
    }

    return false;
}
/******************************************************************************
 End of function  cmdMatchFileSpec
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdCopyMultipleFiles
 Description:   Function to copy multiple files not including sub directories
 Arguments:     IN  pszSrc - Pointer to the source drive and file path with wild
 card
 IN  pszDest - Pointer to the destination drive and path
 OUT p_out - Pointer to the file stream to print to
 Return value:  The number of files copied
 ******************************************************************************/
static int cmdCopyMultipleFiles (char *pszSrc, char *pszDest, FILE *p_out)
{
    const size_t stLength = (1024 * 512);
    void *pvBuffer;
    pvBuffer = R_OS_AllocMem(stLength, R_REGION_LARGE_CAPACITY_RAM);
    int iCount = 0;
    DIR dir;

    if (pvBuffer)
    {
        /* Try to get a pointer to the drive */
        void *pDrive = dskGetDrive((char) toupper( *pszSrc));
        if (pDrive)
        {
            static char pszSrcPath[2600];
            static char pszSrcFile[2600];
            static char pszDestFile[2600];
            static char temp_file[2600];
            FATENTRY fatEntry;
            FATERR fatResult;
            char *pszFileSpec = cmdGetPointerToLastSlash(pszSrc) + 1;
            _Bool bfRoot = false;

            /* Check if the destination is the root */
            if (strlen(pszDest + 2) == 1)
            {
                bfRoot = true;
            }

            cmdCopyPath(pszSrcPath, pszSrc);

            fatResult = R_FAT_FindFirst( &dir, &fatEntry, pszSrcPath, "*");

            while (0 == fatResult)
            {
                if (cmdMatchFileSpec(pszFileSpec, &fatEntry))
                {
                    strcpy(pszDestFile, pszDest);

                    /* Check for root directory (*/
                    if (bfRoot)
                    {
                        /* Add source file name on the end without the slash */
                        strcat(pszDestFile, fatEntry.FileName);
                    }
                    else
                    {
                        /* Add the source file name on to the end with a slash */
                        strcat(pszDestFile, "\\");
                        strcat(pszDestFile, fatEntry.FileName);
                    }

                    strcpy(pszSrcFile, pszSrcPath);
                    strcat(pszSrcFile, "\\");
                    strcat(pszSrcFile, fatEntry.FileName);

                    /* Convert drive name from number('0'-'7') to caractor('A'-'H') at once to output info */
                    pszSrcFile[0] = (char) (pszSrcFile[0] - '0');
                    pszSrcFile[0] = (char) (pszSrcFile[0] + 'A');
                    fprintf(p_out, "Copying file \"%s\" to \"%s\"\r\n", pszSrcFile, pszDestFile);
                    /* Revert drive name from caractor('A'-'H') to number('0'-'7') */
                    pszSrcFile[0] = (char) (pszSrcFile[0] - 'A');
                    pszSrcFile[0] = (char) (pszSrcFile[0] + '0');

                    if (cmdCopySingleFile(pszSrcFile, pszDestFile, pvBuffer, stLength, p_out))
                    {
                        iCount++;
                    }
                    else
                    {
                        fprintf(p_out, "Error in file copy\r\n");
                        break;
                    }
                }

                /* Get the next one */
                fatResult = R_FAT_FindNext( &dir, &fatEntry);

                if (0 == strcmp(temp_file, fatEntry.FileName))
                {
                    fatResult = 1;
                }

                strcpy(temp_file, fatEntry.FileName);
            }
        }
        else
        {
            fprintf(p_out, "Source drive not found\r\n");
        }

        R_OS_FreeMem(pvBuffer);
    }
    else
    {
        fprintf(p_out, "Failed to allocate file copy buffer\r\n");
    }

    return iCount;
}
/******************************************************************************
 End of function  cmdCopyMultipleFiles
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdCopyFiles
 Description:   Command to copy files
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdCopyFiles (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    _Bool bfResult = true; /*Result of transfer*/

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    /* Source and destination files must be specified */
    if (iArgCount < 3)
    {
        fprintf(pCom->p_out, "Source and destination must be specified\r\n");
        return CMD_OK;
    }

    if (0 == strcmp(ppszArgument[1], ppszArgument[2]))
    {
        fprintf(pCom->p_out, "Cannot copy a file/folder to itself\r\n");
        return CMD_OK;
    }


    /* Try to get the source file */
    if (cmdGetDrivePathFile(pCom->temp_file1, ppszArgument[1], pCom))
    {
        /* Get the drive */
        PDRIVE pSrcDrive = dskGetDrive((char) toupper( *pCom->temp_file1));

        if (pSrcDrive)
        {
            /* Check that the source file is not a directory */
            if (cmdIsDirectory(pSrcDrive, pCom->temp_file1 + 2))
            {
                fprintf(pCom->p_out, "Source file \"%s\" is a directory\r\n", pCom->temp_file1);
                return CMD_OK;
            }

            /* Try to get the destination file */
            if (cmdGetDrivePathFile(pCom->temp_file2, ppszArgument[2], pCom))
            {
                /* Get the drive */
                PDRIVE pDestDrive = dskGetDrive((char) toupper( *pCom->temp_file2));

                if (pDestDrive)
                {
                    /* Check to see if the source file name has the wild card in it */
                    if (memchr(pCom->temp_file1, '*', strlen(pCom->temp_file1)))
                    {
                        /* Check to see if the destination is a directory */
                        if (cmdIsDirectory(pDestDrive, pCom->temp_file2 + 2))
                        {
                            int iCount;

                            /* Make sure there is no slash on the end */
                            char *pszEndChar = pCom->temp_file2 + strlen(pCom->temp_file2) - 1;
                            if ( *pszEndChar == '\\')
                            {
                                *pszEndChar = '\0';
                            }

                            /* Copy more than one file */
                            iCount = cmdCopyMultipleFiles(pCom->temp_file1, pCom->temp_file2, pCom->p_out);
                            if (iCount)
                            {
                                fprintf(pCom->p_out, "Copied %d files.\r\n", iCount);
                            }
                        }
                        else
                        {
                            fprintf(pCom->p_out, "The destination must be a directory when the "
                                    "source file contains the * wild card\r\n");
                            return CMD_OK;
                        }
                    }
                    else
                    {
                        void *pvBuffer;

                        /* 0.5MB Buffer size */
                        const size_t stBufferSize = (512 * 1024);

                        /* Check to see if the destination is a directory */
                        if (cmdIsDirectory(pDestDrive, pCom->temp_file2 + 2))
                        {
                            /* Check for root directory (*/
                            if (strlen(pCom->temp_file2 + 2) == 1)
                            {
                                /* Add source file name on the end without the slash */
                                strcat(pCom->temp_file2, (cmdGetPointerToLastSlash(pCom->temp_file1) + 1));
                            }
                            else
                            {
                                /* Add the source file name on to the end */
                                strcat(pCom->temp_file2, cmdGetPointerToLastSlash(pCom->temp_file1));
                            }
                        }

                        /* Allocate memory to copy the file with */
                        pvBuffer = R_OS_AllocMem(stBufferSize, R_REGION_LARGE_CAPACITY_RAM);

                        if (pvBuffer)
                        {
                            /* Copy the file */
                            bfResult = cmdCopySingleFile(pCom->temp_file1, pCom->temp_file2, pvBuffer, stBufferSize, pCom->p_out);

                            if (bfResult)
                            {
                                printf("Copied single file\r\n");
                            }
                            else
                            {
                                printf("\nFailed to open file source file\r\n");
                            }

                            /* Free the buffer */
                            R_OS_FreeMem(pvBuffer);
                        }
                        else
                        {
                            fprintf(pCom->p_out, "Failed to allocate file copy buffer\r\n");
                        }
                    }
                }
                else
                {
                    fprintf(pCom->p_out, "Drive %c not found\r\n", *pCom->temp_file2);
                }
            }
            else
            {
                fprintf(pCom->p_out, "Error in destination file \"%s\"\r\n", ppszArgument[2]);
            }
        }
        else
        {
            fprintf(pCom->p_out, "Drive %c not found\r\n", *pCom->temp_file1);
        }
    }
    else
    {
        fprintf(pCom->p_out, "Error in source file \"%s\"\r\n", ppszArgument[1]);
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdCopyFiles
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdChangeDrive
 Description:   Command to change the current drive
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdChangeDrive (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;
    char buffer[10];
    char working_drive = (char) toupper( *ppszArgument[0]);

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    /* Make sure all available drives are mounted */
    dskMountAllDevices();

    if (dskGetDrive(working_drive))
    {
        strcpy(pCom->working_dir, "\\");
        pCom->working_drive = working_drive;
        sprintf(pCom->working_dir_prompt, "%c:%s>", pCom->working_drive, pCom->working_dir);
        pCom->p_prompt = pCom->working_dir_prompt;
        sprintf(buffer, "%d:\\", (pCom->working_drive - 65));
        f_chdrive(buffer);
    }
    else
    {
        fprintf(pCom->p_out, "Drive \"%c\" not found\r\n", working_drive);
        pCom->p_prompt = pCom->default_prompt;
        pCom->working_drive = -1;
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdChangeDrive
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdVolume
 Description:   Print out the parameters of the current disk volume.
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdVolume (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    UNUSED_PARAM(ppszArgument);
    UNUSED_PARAM(iArgCount);

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }


    if ( -1 != pCom->working_drive)
    {
        PDRIVE pDrive = dskGetDrive(pCom->working_drive);

        if (pDrive)
        {
            DRIVEINFO driveInfo;
            float fVolumeSize;
            char *pszEngMult = (char *) gpszEngMultChars;

            R_FAT_GetDriveInfo(pCom->working_drive, pDrive, &driveInfo);

            fVolumeSize = (float) driveInfo.llVolumeSize;
            fprintf(pCom->p_out, "\r\nDrive %c: \r\n", pCom->working_drive);
            fprintf(pCom->p_out, "FAT Type: %s\r\n", gpszFatType[driveInfo.fatType]);
            fprintf(pCom->p_out, "Block Size: %d\r\n", driveInfo.iBlockSize);
            fprintf(pCom->p_out, "Cluster Size: %dkB\r\n", driveInfo.iSectorSizeIn_k);
            {
                float fFree = (float) driveInfo.llFreeDisk;
                float fUsedPercent = ((fVolumeSize - fFree) / fVolumeSize) * 100.0f;
                /* Scale for engineering multiplier */
                while (fVolumeSize > 1024.0f)
                {
                    fVolumeSize /= 1024.0f;
                    pszEngMult++;
                }
                fprintf(pCom->p_out, "Volume Size: %.2f%cB\r\n", fVolumeSize, *pszEngMult);
                pszEngMult = (char *) gpszEngMultChars;
                /* Scale for engineering multiplier */
                while (fFree > 1024.0f)
                {
                    fFree /= 1024.0f;
                    pszEngMult++;
                }
                fprintf(pCom->p_out, "Free: %.2f%cB\r\n", fFree, *pszEngMult);
                fprintf(pCom->p_out, "Usage: %.2f%%\r\n", fUsedPercent);
            }
        }
        else
        {
            fprintf(pCom->p_out, "Drive \"%c\" not found\r\n", pCom->working_drive);
            pCom->p_prompt = pCom->default_prompt;
            pCom->working_drive = -1;
        }
    }
    else
    {
        fprintf(pCom->p_out, "No working drive selected\r\n");
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdVolume
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdListDisks
 Description:   Command to list the currently mounted disk drives
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdListDisks (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    char chDisk = 'A';
    int iCount = 0;
    AVOID_UNUSED_WARNING;
    dskMountAllDevices();
    while (chDisk <= 'Z')
    {
        _Bool bfMediaPresent, bfWriteProtected;
        if (dskGetMediaState(chDisk, &bfMediaPresent, &bfWriteProtected))
        {
            PDRIVE pDrive = dskGetDrive(chDisk);
            if ((pDrive) && (bfMediaPresent))
            {
                DRIVEINFO driveInfo;
                float fVolumeSize;
                char *pszEngMult = (char *) gpszEngMultChars;

                R_FAT_GetDriveInfo(pCom->working_drive, pDrive, &driveInfo);

                fVolumeSize = (float) driveInfo.llVolumeSize;
                /* Scale for engineering multiplier */
                while (fVolumeSize > 1024.0f)
                {
                    fVolumeSize /= 1024.0f;
                    pszEngMult++;
                }
                fprintf(pCom->p_out, "\r\nDrive %c: %s\r\n", chDisk, (bfWriteProtected) ? "Write protected" : "");
                fprintf(pCom->p_out, "FAT Type: %s\r\n", gpszFatType[driveInfo.fatType]);
                fprintf(pCom->p_out, "Block Size: %d\r\n", driveInfo.iBlockSize);
                fprintf(pCom->p_out, "Cluster Size: %dkB\r\n", driveInfo.iSectorSizeIn_k);
                fprintf(pCom->p_out, "Volume Size: %.2f%cB\r\n", fVolumeSize, *pszEngMult);
                iCount++;
            }
        }
        chDisk++;
    }
    /* Print a message if no drives are attached */
    if ( !iCount)
    {
        fprintf(pCom->p_out, "\r\nNo drives found, use the \"mount\" command to add drives\r\n");
        pCom->p_prompt = pCom->default_prompt;
        pCom->working_drive = -1;
    }
    else
    {
        fprintf(pCom->p_out, "\r\n%d drives found\r\n", iCount);
    }

    return CMD_OK;
}
/******************************************************************************
 End of function  cmdListDisks
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdEjectDisk
 Description:
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdEjectDisk (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    char chDriveLetter = pCom->working_drive;
    AVOID_UNUSED_WARNING;

    if (CTL_STREAM_TCP == check_console_type(pCom->p_out))
    {
        return CMD_OK;
    }

    /* Get the first letter after the commad */
    if (iArgCount > 1)
    {
        chDriveLetter = (char) toupper( *ppszArgument[1]);
    }
    /* Check to see if the drive letter is valid */
    if (chDriveLetter > 0)
    {
        /* Try to eject the media */
        if (dskEjectMedia(chDriveLetter))
        {
            fprintf(pCom->p_out, "Ejected disk %c\r\n", chDriveLetter);
            /* If it was the working drive */
            if (pCom->working_drive == chDriveLetter)
            {
                /* Set back to default prompt */
                pCom->p_prompt = pCom->default_prompt;
                pCom->working_drive = -1;
            }
        }
        else
        {
            fprintf(pCom->p_out, "Disk %c not found\r\n", chDriveLetter);
        }
    }
    else
    {
        fprintf(pCom->p_out, "Invalid drive letter\r\n");
    }
    return CMD_OK;
}
/******************************************************************************
 End of function  cmdEjectDisk
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdDismountDisk
 Description:   Command to dismount a disk
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdDismountDisk (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    AVOID_UNUSED_WARNING;

    /* Make sure all available drives are mounted. */
    dskDismountAllDevices();

    /* Set back to default prompt */
    pCom->p_prompt = pCom->default_prompt;
    pCom->working_drive = -1;
    return CMD_OK;
}
/******************************************************************************
 End of function  cmdDismountDisk
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdMountDisk
 Description:   Command to mount a disk
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmdMountDisk (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    int iDevCount;
    AVOID_UNUSED_WARNING;

    /* Set back to default prompt */
    pCom->p_prompt = pCom->default_prompt;
    pCom->working_drive = -1;

    /* Mount all devices */
    iDevCount = dskMountAllDevices();
    if (iDevCount)
    {
        cmdListDisks(iArgCount, ppszArgument, pCom);
    }
    else
    {
        fprintf(pCom->p_out, "No drives mounted\r\n");
    }
    return CMD_OK;
}
/******************************************************************************
 End of function  cmdMountDisk
 ******************************************************************************/

/******************************************************************************
 Function Name: cmdShowBin
 Description:   Command to show memory in the format
 XX XX XX XX XX XX XX XX : AAAAAAAAAAAAAAAAAA
 where XX is a H rep & AA is ascii rep of byte
 Arguments:     IN  pbyView - pointer to the memory to diaplay
 IN  iLength - The number of bytes to display
 IN  stOffset - The file offset
 OUT pOut - Pointer to the file stream to print to
 Return value:  address of last printed byte
 ******************************************************************************/
uint8_t *cmdShowBin (uint8_t *pbyView, size_t iLength, size_t stOffset, FILE *pOut)
{
    size_t iCount;

    fprintf(pOut, "%.8X  ", stOffset);

    /* Do hex segment */
    for (iCount = 0; iCount < iLength; iCount++)
    {
        fprintf(pOut, "%.2X ", *(pbyView + iCount));
    }

    /* Print separator */
    fprintf(pOut, " : ");

    /* Do ASCII segment */
    for (iCount = 0; iCount < iLength; iCount++)
    {
        uint8_t byData = *(pbyView + iCount);

        /* Print . for non-ascii data */
        if ((byData < 0x7F) && (byData >= ' '))
        {
            fprintf(pOut, "%c", byData);
        }
        else
        {
            fprintf(pOut, ".");
        }
    }
    fprintf(pOut, "\r\n");
    fflush(pOut);
    return (pbyView + iCount);
}
/******************************************************************************
 End of function cmdShowBin
 ******************************************************************************/

/* Table that associates command letters, function pointer and a little
 description of what the command does */
st_cmdfnass_t gpcmdUsbMassStorage[] =
{
     {
    "vol", cmdVolume, "<CR> - Volume information for the working drive",
     },
     {
    "type", cmdTypeFile, "f<CR> - Write text file f to the console",
    },
    {
    "copy", cmdCopyFiles, "s d<CR> - Copy file \"s\" to destination \"d\"",
    },
    {
    "view", cmdViewFile, "f<CR> - view contents of file \"f\"",
    },
    {
    "dir", cmdListDirectory, "<CR> - List the working directory",
    },
    {
    "pwd", cmdPrintWorkingDirectory, "<CR> - Print the working directory",
    },
    {
    "cd", cmdChangeDirectory, "d<CR> - Change working directory to \"d\"",
    },
    {
    "del", cmdDeleteFile, "f<CR> - Delete file \"f\"",
    },
    {
    "md", cmsMakeDirectory, "n<CR> - Make \"n\" in the working directory",
    },
    {
    "rd", cmdDeleteDirectory, "d<CR> - Delete directory \"d\"",
    },
    {
    "ren", cmdRename, "s d<CR> - Rename / move file \"s\" to \"d\"",
    },
    {
    "*:", cmdChangeDrive, NULL,
    },
    {
    "disk", cmdListDisks, "<CR> - List the available disk drives",
    },
    {
    "eject", cmdEjectDisk, "d<CR> - Eject disk \"d\"",
    },
    {
    "dismount", cmdDismountDisk, "<CR> - Dismount all mounted drives",
    },
    {
    "mount", cmdMountDisk, "<CR> - Mount all Mass Storage devices",
    },
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t gcmdUsbMassStorage =
{
     "USB Mass Storage Commands",
     gpcmdUsbMassStorage, (sizeof(gpcmdUsbMassStorage) / sizeof(st_cmdfnass_t))
};

/******************************************************************************
 End  Of File
 ******************************************************************************/
