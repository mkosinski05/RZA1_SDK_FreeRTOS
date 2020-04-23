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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : command.c
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : GNUARM-NONE-EABI v14.02
* OS           : N/A
* H/W Platform : RSK+
* Description  : The entry point of the main command handler and associated
*                commands
*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.02.2010 1.00    First Release
*              : 10.06.2010 1.01    Updated type definitions.
*              : 09.06.2015 1.10    Updated high-ASCII password for GNU TC
*                                   Added FAT Stub check and warning
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
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "compiler_settings.h"
#include "control.h"
#include "version.h"
#include "command.h"

/* This needs tp be removed / and xTaskStatusType should be removed */
#include "FreeRTOS.h"
#include "FreeRTOSconfig.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "nonVolatileData.h"



/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/* Define the number of characters printed on a line for the "mem" command */
#define CMD_PRV_MEM_COMMAND_CHAR_COUNT      (16)
#define CMD_LOGIN_AND_PASSWORD_STRING_PRV_   (32)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

extern const st_command_table_t g_cmd_tbl_platform;

extern const st_command_table_t g_cmd_tbl_platform_extended;

extern const st_command_table_t g_cmd_tbl_core;

#if (R_SELF_INSERT_APP_TOUCH_SCREEN)
extern const st_command_table_t g_cmd_tbl_touchscreen;
#endif

#if (R_SELF_INSERT_APP_GUI)
extern const st_command_table_t g_cmd_tbl_guiliani;
#endif

#if (R_SELF_INSERT_APP_SDK_CAMERA)
extern const st_command_table_t g_cmd_tbl_camerasdk;
#endif

#if (R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES)
extern const st_command_table_t g_cmd_tbl_ethernet;
#endif

#if (R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER)
extern const st_command_table_t g_cmd_tbl_usb;
extern const st_command_table_t gcmdUsbMassStorage;
extern const st_command_table_t gcmdUsbMassStorageTest;

#if (R_SELF_INSERT_APP_HOST_CDC_CONSOLE)
extern const st_command_table_t g_cmd_usb_commsclass;
#endif

#endif /* (R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER) */

#if (R_SELF_INSERT_APP_ADC)
extern const st_command_table_t g_cmd_tbl_adc_app;
#endif

#if (R_SELF_INSERT_APP_PMOD)
extern const st_command_table_t g_cmd_tbl_pmod_app;
#endif

#if (R_SELF_INSERT_APP_SOUND)
extern const st_command_table_t g_cmd_tbl_sound_app;
#endif

#if (R_SELF_INSERT_APP_HID_MOUSE)
extern const st_command_table_t g_cmd_tbl_mouse;
#endif

#if    (R_SELF_INSERT_APP_CDC_SERIAL_PORT)
    /* cast to pst_command_table_t */
extern const st_command_table_t g_cmd_cdc_sample;
#endif

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/


/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static int16_t cmd_set_date(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_set_time (int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_read_memory (int iArgCount, char **ppszArgument, pst_comset_t pCom);
int16_t cmd_list_devlink_tbl_content(int iArgCount, char **ppszArgument, pst_comset_t pCom);
int16_t cmd_list_device_open_handles(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static int16_t cmd_version(int iArgCount, char **ppszArgument, pst_comset_t pCom);
static void cmdLoadLogin(void);
_Bool cmdCheckUserNameAndPassword(char *pszUserName, char *pszPassword);
int cmdUserName (int iArgCount, char **ppszArgument, pst_comset_t pCom);
int cmdChangePassword (int iArgCount, char **ppszArgument, pst_comset_t pCom);




/* Define the engineering exp symbols for Kilo, Mega, Giga, Tera, Peta, Exa, Zetta and Yotta */
static char * const gsp_eng_mult_chars = " kMGTPEZY";
/*****************************************************************************
Global Variables
******************************************************************************/
char gpszLoginsAndPasswords[CMD_LOGIN_AND_PASSWORD_STRING_PRV_];
_Bool  gbfLoginsLoaded = false;

/******************************************************************************
Public Functions
******************************************************************************/

/* Table that associates command letters, function pointer and a little
   description of what the command does */
static st_cmdfnass_t gs_cmd_command[] =
{
     {
        "date",
        cmd_set_date,
        "DD/MM/YYYY<CR> - Set the date",
     },
     {
        "time",
        cmd_set_time,
        "hh:mm:ss<CR> - Set the time",
     },
     {
        "mem",
        cmd_read_memory,
        "a l<CR> - Reads memory from address H'a length H'l",
     },
     {
        "drivers",
        cmd_list_devlink_tbl_content,
        "<CR> - List driver table note driver may not be loaded",
     },
     {
        "handles",
        cmd_list_device_open_handles,
        "<CR> - List opened driver information",
     },

     {
        "ver",
        cmd_version,
        "<CR> - Show the application version",
     }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t gs_cmd_tbl_command =
{
    "General Commands",
    (pst_cmdfnass_t) gs_cmd_command,
    ((sizeof(gs_cmd_command)) / sizeof(st_cmdfnass_t))
};

/* Define the command tables required */
pst_command_table_t gppCommands[32] =
{
    /* cast to pst_command_table_t */
    (pst_command_table_t) &gs_cmd_tbl_command,

    /* cast to pst_command_table_t */
	/* Fix this menu into this location so it can be
	 * replaced when using logout supporting consoles */
    (pst_command_table_t) &g_cmd_tbl_platform,

    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_core,

    /* Added space for additional 'sample code' */

#if (R_SELF_INSERT_APP_ADC)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_adc_app,
#endif

#if (R_SELF_INSERT_APP_PMOD)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_pmod_app,
#endif

#if (R_SELF_INSERT_APP_SOUND)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_sound_app,
#endif

#if (R_SELF_INSERT_APP_TOUCH_SCREEN)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_touchscreen,
#endif

#if (R_SELF_INSERT_APP_SDK_CAMERA)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_camerasdk,
#endif

#if (R_SELF_INSERT_APP_GUI)
    (pst_command_table_t) &g_cmd_tbl_guiliani,
#endif
#if (R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_usb,

    /* cast to pst_command_table_t */
    (pst_command_table_t) &gcmdUsbMassStorage,

    /* cast to pst_command_table_t */
    (pst_command_table_t) &gcmdUsbMassStorageTest,

#if (R_SELF_INSERT_APP_HOST_CDC_CONSOLE)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_usb_commsclass,
#endif

#endif

#if    (R_SELF_INSERT_APP_HID_MOUSE)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_mouse,
#endif

#if    (R_SELF_INSERT_APP_CDC_SERIAL_PORT)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_cdc_sample,
#endif

#if (R_SELF_LOAD_MIDDLEWARE_ETHERNET_MODULES)
    /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_ethernet,
#endif

};

int32_t g_num_commands = (sizeof(gppCommands)) / sizeof(pst_command_table_t);

/* The default user name is admin and the password is password */
/* Password is xor 129 protected */
static const char gpszDefaultUser[] = "admin:\xF1\xE0\xF2\xF2\xF6\xEE\xF3\xE5";

/******************************************************************************
 Function Name: cmd_console
 Description:   Function to process the console commands
 Arguments:     IN  pIn - Pointer to the input file stream
                IN  pOut - Pointer to the file stream for echo of command input
                IN  pszPrompt - Pointer to a command prompt
 Return value:  none
 ******************************************************************************/
void cmd_console(FILE* pIn, FILE *pOut, char *pszPrompt)
{
    pst_comset_t com = R_OS_AllocMem(sizeof(st_comset_t), R_REGION_LARGE_CAPACITY_RAM);
    int32_t pos;

    g_num_commands = (sizeof(gppCommands)) / sizeof(pst_command_table_t);

    /* ensure TCP consoles support the logout command */
    if (0 == strcmp(pszPrompt,"TCP>"))
    {
    	/* Fix this menu into this location so it can be
    	 * replaced when using logout supporting consoles */
    	gppCommands[1] = (pst_command_table_t) &g_cmd_tbl_platform_extended;
    }

    /* remove unused console command lists */
    for (pos = (g_num_commands - 1); pos > 0; pos--)
    {
        /* check for null entry */
        if (NULL == gppCommands[pos])
        {
            g_num_commands--;
        }
    }

    if (com)
    {
        while (1)
        {
            e_cmderr_t cmd_err = CMD_OK;

            /* Initialise the console for login */
            memset(com, 0, sizeof(st_comset_t));
            com->p_in = pIn;
            com->p_out = pOut;

            /* Print a welcome message */
            show_welcome_msg(com->p_out, true);

            fflush(pOut);

            /* Process commands until the link is lost or the command returns a code greater than CMD_ERROR_IN_IO */
            while (cmd_err < CMD_USER_EXIT)
            {
                /* cast gppCommands to cpst_command_table_t * */
                cmd_err = console(com, (cpst_command_table_t *) gppCommands, g_num_commands, pIn, pOut, pszPrompt);
            }

            /* CMD_ERROR_IN_IO is returned when the client disconnects check for an IP stream */
            if ((CMD_ERROR_IN_IO == cmd_err) && (control(R_DEVLINK_FilePtrDescriptor(com->p_in), CTL_STREAM_TCP, NULL) == 0))
            {
                /* Log out */
                cmd_err = CMD_LOG_OUT;
            }

            /* Check for the "exit<CR>" command return code */
            if (CMD_LOG_OUT == cmd_err)
            {
                fprintf(com->p_out, "\r\nBye!\r\n");
                fflush(pOut);

                /* Do "exit" code */
                R_OS_FreeMem(com);
                return;
            }
        }
    }
}
/******************************************************************************
 End of function cmd_console
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_view_memory
 Description:   Function to show memory in format
                XX XX XX XX XX XX XX XX : AAAAAAAAAAAAAAAAAA
                where XX is a H rep & AA is ASCII rep of byte
 Arguments:     IN  pbyView - pointer to the memory to display
                IN  bLength - The number of bytes to display
                OUT pOut = Pointer to the file stream to print to
 Return value:  address of last printed byte
 ******************************************************************************/
uint8_t * cmd_view_memory(uint8_t *pbyView, uint8_t bLength, FILE *pOut)
{
    uint8_t count;
    uint8_t data;

    fprintf(pOut, "%.8lX  ", (unsigned long) pbyView);

    /* Do hex segment */
    for (count = 0; count < bLength; count++)
    {
        fprintf(pOut, "%.2X ", *(pbyView + count));
    }

    /* Print separator */
    fprintf(pOut, " : ");

    /* Do ASCII segment */
    for (count = 0; count < bLength; count++)
    {
        data = *(pbyView + count);

        /* Print '.' for non-ASCII data */
        if ((data < 0x7F) && (data >= ' '))
        {
            fprintf(pOut, "%c", data);
        }
        else
        {
            fprintf(pOut, ".");
        }
    }

    fprintf(pOut, "\r\n");

    return (pbyView + count);
}
/******************************************************************************
 End of function cmd_view_memory
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_show_data_rate
 Description:   Function to print the data transfer rate (in engineering units)
 Arguments:     IN  pFile - Pointer to the file to print to
 IN  fTransferTime - The transfer time in seconds
 IN  stLength - The length of data transferred
 Return value:  none
 ******************************************************************************/
void cmd_show_data_rate (FILE *pFile, float fTransferTime, size_t stLength)
{
    if (fTransferTime > 0.0f)
    {
        float transfer_rate;
        char * p_eng_mult = gsp_eng_mult_chars;

        /* Calculate the transfer rate */
        transfer_rate = (float) stLength / fTransferTime;

        /* Scale for engineering multiplier */
        while (transfer_rate > 1000.0f)
        {
            transfer_rate /= 1000.0f;
            p_eng_mult++;
        }

        /* Print the details */
        fprintf(pFile, "Transferred %u bytes at %.2f %c Bytes per Second\r\n", stLength, transfer_rate, *p_eng_mult);
    }
    else
    {
        fprintf(pFile, "Invalid transfer time\r\n");
    }

    fflush(pFile);
}
/******************************************************************************
 End of function cmd_show_data_rate
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_show_memory_size
 Description:   Function to print a memory size in engineering units
 Arguments:     IN  pFile - Pointer to the file to print to
 IN  stLength - The length of the memory
 Return value:  none
 *****************************************************************************/
void cmd_show_memory_size (FILE *pFile, size_t stLength)
{
    /* cast stLength to float */
    float size = (float) stLength;
    char * p_eng_mult = gsp_eng_mult_chars;

    /* Scale for engineering multiplier */
    while (size > 1024.0f)
    {
        size /= 1024.0f;
        p_eng_mult++;
    }

    /* Print the details */
    fprintf(pFile, "%.3f%cB", size, *p_eng_mult);
    fflush(pFile);
}
/*****************************************************************************
 End of function cmd_show_memory_size
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_show_bin
 Description:   Command to show memory in the format
                XX XX XX XX XX XX XX XX : AAAAAAAAAAAAAAAAAA
                where XX is a H rep & AA is ASCII rep of byte
 Arguments:     IN  pbyView - pointer to the memory to display
                IN  iLength - The number of bytes to display
                IN  stOffset - The file offset
                OUT pOut - Pointer to the file stream to print to
 Return value:  address of last printed byte
 ******************************************************************************/
uint8_t * cmd_show_bin(uint8_t *pbyView, int iLength, size_t stOffset, FILE *pOut)
{
    int16_t count;

    fprintf(pOut, "%.8uX  ", stOffset);

    /* Do hex segment */
    for (count = 0; count < iLength; count++)
    {
        fprintf(pOut, "%.2X ", *(pbyView + count));
    }

    /* Print separator */
    fprintf(pOut, " : ");

    /* Do ASCII segment */
    for (count = 0; count < iLength; count++)
    {
        uint8_t data = *(pbyView + count);

        /* Print '.' for non-ASCII data */
        if ((data < 0x7F) && (data >= ' '))
        {
            fprintf(pOut, "%c", data);
        }
        else
        {
            fprintf(pOut, ".");
        }
    }

    fprintf(pOut, "\r\n");
    fflush(pOut);

    return (pbyView + count);
}
/******************************************************************************
 End of function cmd_show_bin
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_set_date
 Description:   Command to show and set the date
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_set_date(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    DATE date;
    int_t rtc_handle;

    rtc_handle = open(DEVICE_INDENTIFIER "rtc", O_RDWR);

    if (control(rtc_handle, CTL_GET_DATE, &date) == 0)
    {
        static char_t * const p_week_day[] =
        {
            "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
        };
        static char_t * const p_month[] =
        {
            "None", "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        if (iArgCount >= 2)
        {
            char_t delim;
            int_t day;
            int_t month;
            int_t year;

            if (sscanf(ppszArgument[1], "%d%c%d%c%d", &day, &delim, &month, &delim, &year) != (-1))
            {
                date.Field.Day = (unsigned char)(day & 0x1f);
                date.Field.Month = (unsigned) (month & 0xf);
                date.Field.Year = (unsigned) year;

                if (control(rtc_handle, CTL_SET_DATE, &date))
                {
                    fprintf(pCom->p_out, "Failed to set date\r\n");
                }
            }
        }

        /* Get the date again */
        control(rtc_handle, CTL_GET_DATE, &date);

        /* Print the date */
        fprintf(pCom->p_out, "%s %.2d %s %.4d\r\n", p_week_day[date.Field.WeekDay], date.Field.Day, p_month[date.Field.Month], (int16_t) date.Field.Year);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to get date\r\n");
    }

    close(rtc_handle);

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_set_date
 ******************************************************************************/

/******************************************************************************
 Function Name: cmd_set_time
 Description:   Command to show and set the time
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_set_time (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    DATE date;
    int_t rtc_handle;

    rtc_handle = open(DEVICE_INDENTIFIER "rtc", O_RDWR);

    if (control(rtc_handle, CTL_GET_DATE, &date) == 0)
    {
        if (iArgCount >= 2)
        {
            char delim;
            int_t hour;
            int_t minute;
            int_t second;

            if (sscanf(ppszArgument[1], "%d%c%d%c%d", &hour, &delim, &minute, &delim, &second) != (-1))
            {
            	date.Field.Hour = (unsigned) (hour & 0x1f);
                date.Field.Minute = (unsigned) (minute & 0x3f);
                date.Field.Second = (unsigned) (second & 0x3f);

                if (control(rtc_handle, CTL_SET_DATE, &date))
                {
                    fprintf(pCom->p_out, "Failed to set time\r\n");
                }
            }
        }

        /* Get the time again */
        control(rtc_handle, CTL_GET_DATE, &date);

        /* Print the time */
        fprintf(pCom->p_out, "%.2d:%.2d:%.2d\r\n", (int_t) date.Field.Hour, (int_t) date.Field.Minute, (int_t) date.Field.Second);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to get time\r\n");
    }

    close(rtc_handle);

    return CMD_OK;
}
/******************************************************************************
End of function cmd_set_time
******************************************************************************/

/******************************************************************************
 Function Name: cmd_read_memory
 Description:   Command to view memory
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 ******************************************************************************/
static int16_t cmd_read_memory (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    uint32_t total = 21;
    uint8_t *p_memory = 0;
    uint8_t *p_end;

    /* Get address from command line */
    sscanf(ppszArgument[1], "%lX", (long unsigned int*)&p_memory);

    /* Get length from the command line */
    sscanf(ppszArgument[2], "%lX", &total);

    /* Show what we are reading */
    fprintf(pCom->p_out, "View memory Address = H'%lX, Lines = H'%lX\r\n", (long unsigned int)p_memory, total);

    /* calculate the last memory address */
    p_end = p_memory + (total * CMD_PRV_MEM_COMMAND_CHAR_COUNT);

    while (p_memory < p_end)
    {
        /* Show the memory */
        p_memory = cmd_view_memory(p_memory, CMD_PRV_MEM_COMMAND_CHAR_COUNT, pCom->p_out);
    }

    return CMD_OK;
}
/******************************************************************************
 End of function cmd_read_memory
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmd_list_devlink_tbl_content
 Description:   Command to list r_devlink_wrapper devices available, note they may not be loaded
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 *****************************************************************************/
int16_t cmd_list_devlink_tbl_content(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;
    (void) ppszArgument;

    R_DEVLINK_ListDevlinkTblContent(true, pCom);

    return CMD_OK;
}
/******************************************************************************
End of function cmd_list_devlink_tbl_content
******************************************************************************/

/*****************************************************************************
 Function Name: cmd_list_device_open_handles
 Description:   Command to list r_devlink_wrapper loaded driver information
 Arguments:     IN  iArgCount - The number of arguments in the argument list
                IN  ppszArgument - The argument list
                IN  pCom - Pointer to the command object
 Return value:  0 for success otherwise error code
 *****************************************************************************/
int16_t cmd_list_device_open_handles(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void) iArgCount;
    (void) ppszArgument;

    R_DEVLINK_ListDeviceOpenHandles(true, pCom);

    return CMD_OK;
}
/******************************************************************************
End of function cmd_list_device_open_handles
******************************************************************************/

/******************************************************************************
Function Name: cmd_version
Description:   Command to show the version
Arguments:     IN  iArgCount - The number of arguments in the argument list
               IN  ppszArgument - The argument list
               IN  pCom - Pointer to the command object
Return value:  CMD_OK for success
******************************************************************************/
static int16_t cmd_version(int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    /* cast parameters to void to suppress unused parameter warning */
    AVOID_UNUSED_WARNING;

    show_welcome_msg(pCom->p_out, false);
    return CMD_OK;
}
/******************************************************************************
End of function cmd_version
******************************************************************************/

/*****************************************************************************
Function Name: cmdLoadLogin
Description:   Function to load the login information from EEROM
Arguments:     none
Return value:  none
*****************************************************************************/
static void cmdLoadLogin(void)
{
    if (!gbfLoginsLoaded)
    {
        /* Load the logins and passwords */
        if (nvLoad(NVDT_LOGINS_AND_PASSWORDS_V1,
                   gpszLoginsAndPasswords,
                   sizeof(gpszLoginsAndPasswords)))
        /* TODO: DP : Replace load of pwd from EEPROM */
        {
            /* When there is no user information in the non volatile memory then
               admin and password will be default */
            strcpy(gpszLoginsAndPasswords, gpszDefaultUser);
        }
        gbfLoginsLoaded = true;
    }
}
/*****************************************************************************
End of function  cmdLoadLogin
******************************************************************************/


/*****************************************************************************
Function Name: cmdCheckUserNameAndPassword
Description:   Function to check the user name and password
Arguments:     IN  pszUserName - Pointer to the user name string
               IN  pszPassword - Pointer to the password
Return value:  true if the user name and password are correct
*****************************************************************************/
_Bool cmdCheckUserNameAndPassword(char *pszUserName, char *pszPassword)
{
    char *pszLogin;
    cmdLoadLogin();

    /* Put the delimiter on the end of the user name to make the login */
    strcat(pszUserName, ":");

    /* Look for the user name */
    pszLogin = strstr(gpszLoginsAndPasswords, pszUserName);
    if (pszLogin)
    {
        bool_t b_stop = false;
        char pszBuffer[CMD_LOGIN_AND_PASSWORD_STRING_PRV_];
        char *pszPass = pszLogin + strlen(pszUserName);
        char *pszTemp = pszPass;
        char *pszDest = pszBuffer;
        memset(pszBuffer, 0, sizeof(pszBuffer));

        /* Replace the delimiter with a terminator & decypher the password */
        while ((*pszTemp) && (false == b_stop))
        {
            if ((!*pszTemp) || ('|' == *pszTemp))
            {
                *pszDest = '\0';
                b_stop = true;
            }
            else
            {
                *pszDest = (char) (*pszTemp ^ 129);
                pszTemp++;
                pszDest++;
            }
        } /* end of while loop */
        if (strcmp(pszPassword, pszBuffer) == 0)
        {
            return true;
        }
    }
    return false;
}
/*****************************************************************************
 End of function  cmdCheckUserNameAndPassword
 ******************************************************************************/


/*****************************************************************************
 Function Name: cmdUserName
 Description:   Command to change the user name
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 *****************************************************************************/
int cmdUserName (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    if (3 == iArgCount)
    {
        char *pszLogin;
        size_t stLoginLen;

        /* Load the logins and passwords */
        cmdLoadLogin();
        stLoginLen = strlen(ppszArgument[1]);
        strcat(ppszArgument[1], ":");

        /* Look for the user name */
        pszLogin = strstr(gpszLoginsAndPasswords, ppszArgument[1]);
        if (pszLogin)
        {
            char pszNewLoginsAndPasswords[CMD_LOGIN_AND_PASSWORD_STRING_PRV_];
            char *pszSrc = gpszLoginsAndPasswords;
            char *pszDest = pszNewLoginsAndPasswords;
            char *pszNewLogin = ppszArgument[2];
            memset(pszNewLoginsAndPasswords, 0,
                   sizeof(pszNewLoginsAndPasswords));
            /* Copy any data in the string before the login */
            while (pszSrc < pszLogin)
            {
                *pszDest++ = *pszSrc++;
            }

            /* Copy in the new user name */
            while (*pszNewLogin)
            {
                *pszDest++ = *pszNewLogin++;
            }

            /* Copy in the remainder of the string */
            pszSrc += stLoginLen;
            while (*pszSrc)
            {
                *pszDest++ = *pszSrc++;
            }
            if (!nvStore(NVDT_LOGINS_AND_PASSWORDS_V1,
                         pszNewLoginsAndPasswords,
                         sizeof(gpszLoginsAndPasswords)))
            {
                memcpy(gpszLoginsAndPasswords,
                       pszNewLoginsAndPasswords,
                       sizeof(gpszLoginsAndPasswords));
                if (pCom)
                {
                    fprintf(pCom->p_out, "OK\r\n");
                }
            }
            else if (pCom)
            {
                fprintf(pCom->p_out, "**Error: Failed to store logins and passwords\r\n");
            }
        }
        else if (pCom)
        {
            fprintf(pCom->p_out, "Invalid user name\r\n");
        }
    }
    else if (pCom)
    {
        fprintf(pCom->p_out, "Expecting <old_user_name> <new_user_name>\r\n");
    }
    return CMD_OK;
}
/*****************************************************************************
 End of function  cmdUserName
 ******************************************************************************/

/*****************************************************************************
 Function Name: cmdChangePassword
 Description:   Command to change the user name
 Arguments:     IN  iArgCount The number of arguments in the argument list
 IN  ppszArgument - The argument list
 IN  pCom - Pointer to the command object
 Return value:  CMD_OK for success
 *****************************************************************************/
int cmdChangePassword (int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
//TODO: List arguments and order somewhere
    /* Only accept a call with 4 arguments */
    if ((4 == iArgCount) && (cmdCheckUserNameAndPassword(ppszArgument[1],
                                                         ppszArgument[2])))
    {
        char *pszLogin;
        size_t stLoginLen;

        /* Load the logins and passwords */
        stLoginLen = strlen(ppszArgument[1]);

        /* Look for the user name */
        pszLogin = strstr(gpszLoginsAndPasswords, ppszArgument[1]);
        if (pszLogin)
        {
            bool_t b_stop = false;
            char pszNewLoginsAndPasswords[CMD_LOGIN_AND_PASSWORD_STRING_PRV_];
            char *pszSrc = gpszLoginsAndPasswords;
            char *pszDest = pszNewLoginsAndPasswords;
            char *pszNewPassword = ppszArgument[3];
            memset(pszNewLoginsAndPasswords, 0,
                   sizeof(pszNewLoginsAndPasswords));
            /* Copy any data until the end of the login */
            pszLogin += stLoginLen;
            while (pszSrc < pszLogin)
            {
                *pszDest++ = *pszSrc++;
            }

            /* Copy in the new password */
            while (*pszNewPassword)
            {
                *pszDest++ = (char) (*pszNewPassword++ ^ 129);
            }

            /* Point to the end of the old password */
            pszSrc += stLoginLen;
            while ((*pszSrc) && (false == b_stop))
            {
                if ('|' == *pszSrc)
                {
                    b_stop = true;
                }
                else
                {
                    pszSrc++;
                }
            }

            /* Copy in the remainder of the string */
            while (*pszSrc)
            {
                *pszDest++ = *pszSrc++;
            }
            if (!nvStore(NVDT_LOGINS_AND_PASSWORDS_V1,
                         pszNewLoginsAndPasswords,
                         sizeof(gpszLoginsAndPasswords)))
            {
                memcpy(gpszLoginsAndPasswords,
                       pszNewLoginsAndPasswords,
                       sizeof(gpszLoginsAndPasswords));
                if (pCom)
                {
                    fprintf(pCom->p_out, "OK\r\n");
                }
            }
            else if (pCom)
            {
                fprintf(pCom->p_out,
                        "**Error: Failed to store logins and passwords\r\n");
            }
        }
        else if (pCom)
        {
            fprintf(pCom->p_out, "Invalid user name\r\n");
        }
    }
    else if (pCom)
    {
        fprintf(pCom->p_out,
                "Expecting <user_name> <password> <new_password>\r\n");
    }
    return CMD_OK;
}
/*****************************************************************************
End of function  cmdChangePassword
******************************************************************************/



/******************************************************************************
 End  Of File
 ******************************************************************************/
