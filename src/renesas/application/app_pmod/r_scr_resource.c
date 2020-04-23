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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_src_resource.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-RZv14.01-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Provide support for locking access to the multiple screen 
*                 types connected to the RSK, allowing usercode to prohibit 
*                 PMOD / LCD & DVI use. Needed due to PMOD and LCD / DVI
*                 share data pins and writing to one can corrupt data on other.
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.08.2015 1.00    Initial Version
*******************************************************************************/

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

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "compiler_settings.h"
#include "r_src_resource.h"


/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/******************************************************************************
Constant Data
******************************************************************************/
volatile  os_task_t * gp_pmod_lcd_task_id = 0;

/******************************************************************************
Public Functions
******************************************************************************/

/*******************************************************************************
* Function Name : R_SCR_ResourceLockDisplay
* Description   : Attempts to lock screen resources for requesting task.
* Argument      : none
* Return value  :  true for success, false if the event could not be created
*                  or the wait timed out
*******************************************************************************/
_Bool R_SCR_ResourceLockDisplay (void)
{
    _Bool ret = false;

    if(0 == gp_pmod_lcd_task_id)
    {
        ret = true;
        gp_pmod_lcd_task_id = R_OS_GetCurrentTask();
    }
    else
    {
        TRACE("can not complete PMOD or LCD display is in use.\r\n");
        R_OS_TaskSleep(1);
    }
    return ret;
}
/*******************************************************************************
* End of function R_SCR_ResourceLockDisplay
*******************************************************************************/

/*******************************************************************************
* Function Name : R_SCR_ResourceUnlockDisplay
* Description   : Frees screen resources
* Argument      : none
* Return value  : none
*******************************************************************************/
void R_SCR_ResourceUnlockDisplay (void)
{
    if(0 != gp_pmod_lcd_task_id)
    {

        if(R_OS_GetCurrentTask() == gp_pmod_lcd_task_id)
        {
            gp_pmod_lcd_task_id = 0;
        }
    }
}
/*******************************************************************************
* End of function R_SCR_ResourceUnlockDisplay
*******************************************************************************/


/*******************************************************************************
* Function Name : R_SCR_ResourceDisplayStatus
* Description   : Check to see if Display is in use
* Argument      : none
* Return value  : 0 Not in use
*                 non-zero taskID of display resource holder
*******************************************************************************/
uint32_t R_SCR_ResourceDisplayStatus (void)
{
    uint32_t ret = 0;

    if(0 != gp_pmod_lcd_task_id)
    {
        ret = (uint32_t)gp_pmod_lcd_task_id;
    }
    return ret;
}
/*******************************************************************************
* End of function R_SCR_ResourceDisplayStatus
*******************************************************************************/
