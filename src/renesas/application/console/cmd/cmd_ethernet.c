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
* File Name    : cmd_ethernet.c
* Version      : 1.00
* Device(s)    : Renesas
* Tool-Chain   : N/A
* OS           : N/A
* H/W Platform : RSK+
* Description  : The Ethernet function commands
*******************************************************************************
* History      : DD.MM.YYYY Ver. Description
*              : 01.08.2009 1.00 First Release
*              : 17.06.2015 1.10 IP/MAC Delineators changed in command text
*                                Added auto save on MAC change and JP5 note.
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
#include <math.h>

/******************************************************************************
User Includes
******************************************************************************/

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "control.h"
#include "command.h"
#include "r_task_priority.h"

#include "lwIP_interface.h"

/******************************************************************************
External Variables
******************************************************************************/

/******************************************************************************
Private Functions
******************************************************************************/

/*****************************************************************************
Function Name: cmdIpConfig
Description:   Command to set the IP settings
Arguments:     IN  iArgCount The number of arguments in the argument list
               IN  ppszArgument - The argument list
               IN  pCom - Pointer to the command object
Return value:  0 for success otherwise error code
******************************************************************************/
static int16_t cmdIpConfig(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    if (iArgCount > 1)
    {
        _Bool    bfShow = true;
        _Bool    bfSave = false;
        int iArgument = 1;
        while (iArgument < iArgCount)
        {
            if (('-' == ppszArgument[iArgument][0])
            || ('\\' == ppszArgument[iArgument][0])
            || ('/' == ppszArgument[iArgument][0]))
            {
                switch (ppszArgument[iArgument][1])
                {
                    case 'r':
                    {
                        ipReconfigure(NULL, (PNVDHCP) &gDefaultIpConfigV1);
                        break;
                    }
                    case 's':
                    {
                        bfSave = true;
                        break;
                    }
                    case 'i':
                    {
                        char chDelim;
                        int  piAddress[4];
                        if (sscanf(&ppszArgument[iArgument][2],
                                   "%c%d%c%d%c%d%c%d",
                                   &chDelim, &piAddress[0],
                                   &chDelim, &piAddress[1],
                                   &chDelim, &piAddress[2],
                                   &chDelim, &piAddress[3]) == 8)
                        {
                            NVDHCP  ipConfig;
                            ipGetConfiguration(NULL, &ipConfig);
                            ipConfig.pbyIpAddress[0] = (uint8_t)piAddress[0];
                            ipConfig.pbyIpAddress[1] = (uint8_t)piAddress[1];
                            ipConfig.pbyIpAddress[2] = (uint8_t)piAddress[2];
                            ipConfig.pbyIpAddress[3] = (uint8_t)piAddress[3];
                            ipReconfigure(NULL, &ipConfig);
                        }
                        break;
                    }
                    case 'm':
                    {
                        char chDelim;
                        int  piMask[4];
                        if (sscanf(&ppszArgument[iArgument][2],
                                   "%c%d%c%d%c%d%c%d",
                                   &chDelim, &piMask[0],
                                   &chDelim, &piMask[1],
                                   &chDelim, &piMask[2],
                                   &chDelim, &piMask[3]) == 8)
                        {
                            NVDHCP  ipConfig;
                            ipGetConfiguration(NULL, &ipConfig);
                            ipConfig.pbyAddressMask[0] = (uint8_t)piMask[0];
                            ipConfig.pbyAddressMask[1] = (uint8_t)piMask[1];
                            ipConfig.pbyAddressMask[2] = (uint8_t)piMask[2];
                            ipConfig.pbyAddressMask[3] = (uint8_t)piMask[3];
                            ipReconfigure(NULL, &ipConfig);
                        }
                        break;
                    }
                    case 'g':
                    {
                        char chDelim;
                        int  piAddress[4];
                        if (sscanf(&ppszArgument[iArgument][2],
                                   "%c%d%c%d%c%d%c%d",
                                   &chDelim, &piAddress[0],
                                   &chDelim, &piAddress[1],
                                   &chDelim, &piAddress[2],
                                   &chDelim, &piAddress[3]) == 8)
                        {
                            NVDHCP  ipConfig;
                            ipGetConfiguration(NULL, &ipConfig);
                            ipConfig.pbyGatewayAddress[0] = (uint8_t)piAddress[0];
                            ipConfig.pbyGatewayAddress[1] = (uint8_t)piAddress[1];
                            ipConfig.pbyGatewayAddress[2] = (uint8_t)piAddress[2];
                            ipConfig.pbyGatewayAddress[3] = (uint8_t)piAddress[3];
                            ipReconfigure(NULL, &ipConfig);
                        }
                        break;
                    }
                    case 'o':
                    {
                        if ('n' == ppszArgument[iArgument][2])
                        {
                            NVDHCP  ipConfig;
                            ipGetConfiguration(NULL, &ipConfig);
                            ipConfig.byEnableDHCP = true;
                            ipReconfigure(NULL, &ipConfig);
                        }
                        else if ('f' == ppszArgument[iArgument][2])
                        {
                            NVDHCP  ipConfig;
                            ipGetConfiguration(NULL, &ipConfig);
                            ipConfig.byEnableDHCP = false;
                            ipReconfigure(NULL, &ipConfig);
                        }
                        else
                        {
                            bfShow = false;
                            fprintf(pCom->p_out, "Unknown option %s\r\n", ppszArgument[iArgument]);
                        }
                        break;
                    }
                    case 'a':
                    {
                        break;
                    }
                    default:
                    {
                        bfShow = false;
                        fprintf(pCom->p_out, "Unknown option %s\r\n", ppszArgument[iArgument]);
                        break;
                    }
                }
                iArgument++;
            }
            else
            {
                fprintf(pCom->p_out, "Invalid option %s\r\n", ppszArgument[iArgument]);
                break;
            }
        }
        if (bfSave)
        {
            NVDHCP  ipConfig;
            ipGetConfiguration(NULL, &ipConfig);
            if (nvStore(NVDT_DHCP_SETTINGS_V1, &ipConfig, sizeof(NVDHCP)))
            {
                fprintf(pCom->p_out, "Error saving settings\r\n");
            }
        }
        if (bfShow)
        {
            static const char * const pszEnabled[] = 
            {
                "Disabled",
                "Enabled"
            };
            NVDHCP  ipConfig;
            ipGetConfiguration(NULL, &ipConfig);
            fprintf(pCom->p_out, "DHCP    = %s\r\n"
                   "Address = %*d.%*d.%*d.%*d\r\n"
                   "Mask    = %*d.%*d.%*d.%*d\r\n"
                   "Gateway = %*d.%*d.%*d.%*d\r\n",
                   pszEnabled[(ipConfig.byEnableDHCP) ? 1 : 0],
                   3, ipConfig.pbyIpAddress[0],
                   3, ipConfig.pbyIpAddress[1],
                   3, ipConfig.pbyIpAddress[2],
                   3, ipConfig.pbyIpAddress[3],
                   3, ipConfig.pbyAddressMask[0],
                   3, ipConfig.pbyAddressMask[1],
                   3, ipConfig.pbyAddressMask[2],
                   3, ipConfig.pbyAddressMask[3],
                   3, ipConfig.pbyGatewayAddress[0],
                   3, ipConfig.pbyGatewayAddress[1],
                   3, ipConfig.pbyGatewayAddress[2],
                   3, ipConfig.pbyGatewayAddress[3]);
        }
    }
    else
    {
        int_t table_id = 0;

        /* Find the help string and print it */
        while (table_id < pCom->num_tables)
        {
            uint32_t  commnad_id = 0;
            st_cmdfnass_t * pCommand = pCom->p_function[table_id]->command_list;
            while (commnad_id < pCom->p_function[table_id]->number_of_commands)
            {
                if ((pCommand->function == cmdIpConfig)
                &&  (pCommand->p_command_description))
                {
                    fprintf(pCom->p_out, "%s %s\r\n",
                           pCommand->p_command,
                           pCommand->p_command_description);
                }
                pCommand++;
                commnad_id++;
            }
            table_id++;
        }
    }
    fflush(pCom->p_out);
    return CMD_OK;
}
/*****************************************************************************
End of function  cmdIpConfig
******************************************************************************/

/*****************************************************************************
* Function Name: cmdSetMac
* Description  : Command to set the MAC address into ROM
*   "setmacaddress",
*   cmdSetMac,
*   "-o<CR> where o is one of the following options",
*   "         -a:xx.xx.xx.xx.xx.xx = Set and store MAC address\r\n"
*   "         (no option) = Command list display",
* Arguments    : IN  iArgCount The number of arguments in the argument list
*                IN  ppszArgument - The argument list
*                IN  pCom - Pointer to the command object
* Return Value : 0 for success otherwise error code
******************************************************************************/
static int16_t cmdSetMac(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    uint8_t  pbyMAC[6];
    FILE    *pFile;
    pFile = fopen("\\\\.\\eeprom", "r");
    if (pFile)
    {
        if (fseek(pFile, 1, SEEK_SET) == 0)
        {
            fread(pbyMAC, 1UL, sizeof(pbyMAC), pFile);
        }
        fclose(pFile);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to open EEPROM\r\n");
        return CMD_OK;
    }
    if (iArgCount > 1)
    {
        _Bool    bfShow = true;
        _Bool    bfSave = false;
        int iArgument = 1;
        while (iArgument < iArgCount)
        {
            if ('-' == ppszArgument[iArgument][0])
            {
                switch (ppszArgument[iArgument][1])
                {
                    case 'a':
                    {
                        char chDelim;
                        int  piAddress[6];
                        if (sscanf(&ppszArgument[iArgument][2],
                                   "%c%x%c%x%c%x%c%x%c%x%c%x",
                                   &chDelim, &piAddress[0],
                                   &chDelim, &piAddress[1],
                                   &chDelim, &piAddress[2],
                                   &chDelim, &piAddress[3],
                                   &chDelim, &piAddress[4],
                                   &chDelim, &piAddress[5]) == 12)
                        {
                            pbyMAC[0] = (uint8_t)piAddress[0];
                            pbyMAC[1] = (uint8_t)piAddress[1];
                            pbyMAC[2] = (uint8_t)piAddress[2];
                            pbyMAC[3] = (uint8_t)piAddress[3];
                            pbyMAC[4] = (uint8_t)piAddress[4];
                            pbyMAC[5] = (uint8_t)piAddress[5];

                            bfSave = true;
                        }
                        break;
                    }
                    case 's':
                    {
                        bfSave = true;
                        break;
                    }
                    default:
                    {
                        bfShow = false;
                        fprintf(pCom->p_out, "Unknown option %s\r\n", ppszArgument[iArgument]);
                        break;    
                    }
                }
                iArgument++;
            }
            else
            {
                fprintf(pCom->p_out, "Invalid option %s\r\n", ppszArgument[iArgument]);
                iArgument++;
            }
        }
        if (bfSave)
        {
            pFile = fopen("\\\\.\\eeprom", "w");
            if (pFile)
            {
                if (fseek(pFile, 1, SEEK_SET) == 0)
                {
                    if (fwrite(pbyMAC, 1UL, 6, pFile) == 6)
                    {
                        /* Write to the device now */
                        fflush(pFile);
                        if (fseek(pFile, 0, SEEK_SET) == 0)
                        {                        
                            uint8_t byData = 0xA5;
                            if (fwrite(&byData, 1UL, 1, pFile) == 1)
                            {
                                /* Write to the device now */
                                fflush(pFile);
                                fprintf(pCom->p_out, "Written MAC to EEPROM\r\n");
                            }
                            else
                            {
                                fprintf(pCom->p_out, "Error writing EEPROM\r\n");
                            }
                        }
                        else
                        {
                            fprintf(pCom->p_out, "Error seeking EEPROM\r\n");
                        }
                    }
                    else
                    {
                        fprintf(pCom->p_out, "Error writing EEPROM\r\n");
                    }
                }
                else
                {
                    fprintf(pCom->p_out, "Error seeking EEPROM\r\n");
                }
                fclose(pFile);
            }
            else
            {
                fprintf(pCom->p_out, "Failed to open EEPROM\r\n");
            }
        }
        if (bfShow)
        {
            /* Print the MAC address */
            fprintf(pCom->p_out,
                    "\r\nMAC Address = %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n",
                    pbyMAC[0], pbyMAC[1],
                    pbyMAC[2], pbyMAC[3],
                    pbyMAC[4], pbyMAC[5]);

            /* Print a message to re-start the system */
            fprintf(pCom->p_out,
                    "\r\nPlease restart the system for the changes to take effect\r\n");
        }
    }
    else
    {
        int_t table_id = 0;

        /* Find the help string and print it */
        while (table_id < pCom->num_tables)
        {
            uint32_t commnad_id = 0;
            st_cmdfnass_t * pCommand = pCom->p_function[table_id]->command_list;

            while (commnad_id < pCom->p_function[table_id]->number_of_commands)
            {
                if ((pCommand->function == cmdSetMac)
                &&  (pCommand->p_command_description))
                {
                    fprintf(pCom->p_out, "%s %s\r\n",
                           pCommand->p_command,
                           pCommand->p_command_description);
                }
                pCommand++;
                commnad_id++;
            }
            table_id++;
        }
    }
    return CMD_OK;
}
/*****************************************************************************
End of function  cmdSetMac
******************************************************************************/

/*****************************************************************************
* Function Name: cmdPrintFileSegment
* Description  : Command to show memory in format
*                XX XX XX XX XX XX XX XX : AAAAAAAAAAAAAAAAAA
*                where XX is a H rep & AA is ascii rep of byte
* Arguments    : OUT pFile - Pointer to the file stream to print to
*                IN  pbyView - pointer to the memory to diaplay
*                IN  bLength - The number of bytes to display
*                IN  stFileOffsert - The offset into the file
* Return Value : address of last printed byte
******************************************************************************/
static uint8_t* cmdPrintFileSegment(FILE       *pFile,
                                    uint8_t    *pbyView,
                                    uint8_t    bLength,
                                    size_t     stFileOffsert)
{
    uint8_t      byCount, byData;
    fprintf(pFile, "%.8lX  ", (uint32_t) stFileOffsert);

    /* Do hex segment */
    for (byCount = (uint8_t)0; byCount < bLength; byCount++)
    {
        fprintf(pFile, "%.2X ", *(pbyView + byCount));
    }

    /* Print separator */
    fprintf(pFile, " : ");

    /* Do ASCII segment */
    for (byCount = (uint8_t)0; byCount < bLength; byCount++)
    {
        byData = * (pbyView + byCount);

        /* Print . for non-ascii data */
        if ((byData < 0x7F) && (byData >= ' '))
        {
            fprintf(pFile, "%c", byData);
        } else {
            fprintf(pFile, ".");
        }
    }
    fprintf(pFile, "\r\n");

    return (pbyView + byCount);
}
/*****************************************************************************
End of function cmdPrintFileSegment
******************************************************************************/

/******************************************************************************
* Function Name: cmdReadRom
* Description  : Command to read the EEPROM and show on the terminal
* Arguments    : IN  iArgCount The number of arguments in the argument list
*                IN  ppszArgument - The argument list
*                IN  pCom - Pointer to the command object
* Return Value : 0 for success otherwise error code
******************************************************************************/
static int16_t cmdReadRom(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    FILE    *pFile;
    uint8_t  pbyeeprom[16];
    AVOID_UNUSED_WARNING;
    pFile = fopen("\\\\.\\eeprom", "r");
    if (pFile)
    {
        size_t  stFileOffset = 0UL;
        size_t  stFileLength;
        fseek(pFile, 0L, SEEK_END);
        stFileLength = (size_t)ftell(pFile);
        fseek(pFile, 0L, SEEK_SET);
        fprintf(pCom->p_out, "Address   File data in hexadecimal"
               "                         File data in ASCII\r\n");
        /* Show the entire file */
        while (stFileOffset < stFileLength)
        {
            if (fread(pbyeeprom, 1UL, sizeof(pbyeeprom), pFile) > 0)
            {
                cmdPrintFileSegment(pCom->p_out, pbyeeprom, sizeof(pbyeeprom), stFileOffset);
                stFileOffset += 16;
            }
            else
            {
                fprintf(pCom->p_out, "Error reading EEPROM\r\n");
                break;
            }
        }
        fclose(pFile);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to open device\r\n");
    }
    return CMD_OK;
}
/******************************************************************************
End of function  cmdReadRom
******************************************************************************/

/******************************************************************************
* Function Name: cmdResetRom
* Description  : Command to erase all user data in the EEPROM
* Arguments    : IN  iArgCount The number of arguments in the argument list
*                IN  ppszArgument - The argument list
*                IN  pCom - Pointer to the command object
* Return Value : 0 for success otherwise error code
******************************************************************************/
static int16_t cmdResetRom(int iArgCount, char **ppszArgument, pst_comset_t pCom)
{
    FILE    *pFile;
    uint8_t  pbyeeprom[16];
    AVOID_UNUSED_WARNING;
    pFile = fopen("\\\\.\\eeprom", "w");
    if (pFile)
    {
        /* The MAC address is programmed into the first 7 bytes (0 to 6)*/
        size_t  stFilePosition = 7UL;
        size_t  stFileLength;
        fseek(pFile, 0L, SEEK_END);
        stFileLength = (size_t)ftell(pFile);

        {
            /* The ethernet controller will only auto-load the MAC address
               if the first byte in the EEROM is 0xA5. Make sure that it is
               always present */
            uint8_t byData = 0xA5;
            fseek(pFile, 0UL, SEEK_SET);
            fwrite(&byData, 1UL, 1UL, pFile);
        }

        /* Set the map of the eeprom memory to the erased value */
        memset(pbyeeprom, 0xFF,  sizeof(pbyeeprom));
        /* Seek to the end of the MAC address */
        if (fseek(pFile, (int32_t)stFilePosition, SEEK_SET) == 0)
        {
            while (stFilePosition < stFileLength)
            {
                volatile size_t  stLength = sizeof(pbyeeprom);
                volatile size_t  iWritten;
                if ((stFileLength - stFilePosition) < stLength)
                {
                    stLength = (stFileLength - stFilePosition);
                }
                iWritten = fwrite(pbyeeprom, 1UL, stLength, pFile);
                if (iWritten != stLength)
                {
                    fprintf(pCom->p_out, "Error in write at %lu\r\n", (long)stFilePosition);
                    break;
                }
                stFilePosition += stLength;
            }
        }
        fclose(pFile);
    }
    else
    {
        fprintf(pCom->p_out, "Failed to open device\r\n");
    }
    return CMD_OK;
}
/******************************************************************************
End of function  cmdResetRom
******************************************************************************/

/* Table that associates command letters, function pointer and a little
   description of what the command does */
static st_cmdfnass_t gs_cmd_ethernet[] =
{
     {
          "readrom",
          cmdReadRom,
          "<CR> - Read the EEPROM",
     },
     {
         "rstrom",
         cmdResetRom,
         "<CR> - Reset all user data in the EEPROM\r\n",
     },
     {
         "ipconfig",
         cmdIpConfig,
         "-o<CR> where o is one of the or more following options\r\n"
         "         -r = Reset to default settings\r\n"
        "         -s = Save current settings\r\n"
        "         -i:xxx.xxx.xxx.xxx = Set IP address\r\n"
        "         -m:xxx.xxx.xxx.xxx = Set IP address mask\r\n"
        "         -g:xxx.xxx.xxx.xxx = Set DHCP server gateway address\r\n"
        "         -on = Enable DHCP\r\n"
        "         -off = Disable DHCP\r\n"
        "         -all = Display current settings\r\n"
        "         (no option) = Command list display",
     },
     {
        "ifconfig",
        cmdIpConfig,
        "-o<CR> where o is one of the above options",
     },
     {
        "setmacaddress",
        cmdSetMac,
        "-o<CR> where o is one of the following options\r\n"
        "         -a:xx:xx:xx:xx:xx:xx = Set MAC address and save\r\n"
        "         -s = Save current settings\r\n"
        "         (no option) = Command list display\r\n"
        " *** Caution - Changing the MAC address should not be completed\r\n"
        " ***           unless you are restoring the allocated number or\r\n"
        " ***           have your own address range allocated from the\r\n"
        " ***           relevant authority."
    }
};

/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_ethernet =
{
    "Ethernet Platform Commands",
    gs_cmd_ethernet,
    (sizeof(gs_cmd_ethernet)/sizeof(st_cmdfnass_t))
};

/******************************************************************************
End  Of File
******************************************************************************/
