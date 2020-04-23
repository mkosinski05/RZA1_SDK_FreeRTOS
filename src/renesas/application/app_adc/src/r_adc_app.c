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
* File Name     : r_usb_cdc_app.c
* Description   : Application to read the ADC and display its value,
*                 API to ADC driver supporting open close and read functionality
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 24.05.2018 1.00    Initial Version
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "r_typedefs.h"
#include "iodefine_cfg.h"
#include "compiler_settings.h"

#include "console.h"
#include "command.h"
#include "control.h"

#include "r_adc.h"
#include "r_adc_app.h"


void R_ADC_SampleMain(FILE *pIn, FILE *pOut)
{
    uint16_t adc_val   = 0;
    int8_t  hasaborted = 0;  /* -1 user abort; 0 default state; */
    int_t i_in         = R_DEVLINK_FilePtrDescriptor(pIn);
    r_adc_ch ch        = R_ADC_CH2;
    static uint16_t refcount = 0;
    r_adc_cfg adc_setting;

    adc_setting.ch = ch;
    adc_setting.trgs = ADC_NO_TRIGGER;
    adc_setting.cks = ADC_256TCYC;
    adc_setting.mds = ADC_SINGLE;

    if(0 == refcount)
    {
        refcount++;
        fprintf(pOut, "ADC sample program start\r\n");
        fprintf(pOut, "Press any key to terminate demo\r\n");
        fprintf(pOut, "Rotate Potentiometer to see effect on adc input (AN2)\r\n");

        /* flush any remaining input */
        while(control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
        {
            fgetc(pIn);
        }

        R_ADC_Open(&adc_setting);

        while(0 == hasaborted)
        {
            R_ADC_Read(&adc_val, ch, 1);
            fprintf(pOut, "\rPotentiometer(AN2) = %04ld", adc_val);
            fflush(pOut);

            /* If key press then abort sample */
            if(control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
            {
                hasaborted = -1;
                fgetc(pIn);
            }

            R_OS_TaskSleep(5);
        }

        fprintf(pOut, "\r\n");
        fprintf(pOut, "ADC sample program terminated\r\n");

        R_ADC_Close(ch);
        refcount--;
    }
    else
    {
        fprintf(pOut, "ADC sample program in use, please terminate other adc command\r\n");
    }
}


/* End of File */

