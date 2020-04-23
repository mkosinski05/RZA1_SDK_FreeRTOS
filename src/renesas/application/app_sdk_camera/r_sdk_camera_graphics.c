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
 * http://www.renesas.com/disclaimer*
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/

/*******************************************************************************
* File Name   : r_sdk_camera_graphics.c
* Version     : 1.00 <- Optional as long as history is shown below
* Description : This module provides function of register value changing
                regarding image quality adjustment.
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2017 1.05 first version
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "r_typedefs.h"

#include "r_rvapi_header.h"
#include "lcd_panel.h"
#include "r_camera_if.h"
#include "r_camera_module.h"
#include "r_cui_api.h"
#include "mcu_board_select.h"
#include "r_image_config.h"
#include "r_vdc_portsetting.h"
#include "r_display_init.h"
#include "r_sdk_camera_graphics.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define GRAPHICS_TSK_PRI   (osPriorityNormal)

/* Frame buffer stride: Frame buffer stride should be set to a multiple of 32 or 128
 in accordance with the frame buffer burst transfer mode. */
#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
#define     VIDEO_BUFFER_STRIDE         (((CAP_CEU_SIZE_HW * 2u) + 31u) & ~31u)
#define     VIDEO_BUFFER_HEIGHT         (CAP_CEU_SIZE_VW)
#define     VIDEO_BUFFER_NUM            (1u)
#elif ( TARGET_BOARD == TARGET_BOARD_RSK )
#define     VIDEO_BUFFER_STRIDE         (((CAP_VDC_SIZE_HW * 2u) + 31u) & ~31u)
#define     VIDEO_BUFFER_HEIGHT         (CAP_VDC_SIZE_VW)
#define     VIDEO_BUFFER_NUM            (2u)
#else
#error ERROR: Invalid board defined.
#endif

/* Display area */
#define     DISP_AREA_HS                (80u)
#define     DISP_AREA_VS                (16u)
#define     DISP_AREA_HW                (320u)
#define     DISP_AREA_VW                (240u)

void sdk_camera_graphics_sample_task(void const *pArg);

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
extern uint8_t video_buffer[(VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT) * VIDEO_BUFFER_NUM] __attribute__ ((section(".VRAM_SECTION0")));

#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
extern ceu_error_t video_init_ceu(void);
#elif ( TARGET_BOARD == TARGET_BOARD_RSK )
extern vdc_error_t video_init_vdc(const vdc_channel_t vdc_ch);
#endif
/***********************************************************************************************************************
 * Function Name: graphics_sample_task
 * Description  : Creates touch screen task
 * Arguments    : PCOMSET pCOM
 * Return Value : none
 ***********************************************************************************************************************/
void sdk_camera_graphics_sample_task(void const *pArg)
{
    UNUSED_PARAM(pArg);
    vdc_error_t error;
    vdc_channel_t vdc_ch = VDC_CHANNEL_0;

    /***********************************************************************/
    /* display init (VDC5 output setting) */
    /***********************************************************************/
    {
        error = r_display_init (vdc_ch);
    }

    /***********************************************************************/
    /* Video init (CEU setting / VDC5 input setting) */
    /***********************************************************************/
    if (error == VDC_OK)
    {
#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
        error = video_init_ceu ();
#elif ( TARGET_BOARD == TARGET_BOARD_RSK )
        error = video_init_vdc(vdc_ch);
#else
#error ERROR: Invalid board defined.
#endif
    }

#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
    /***********************************************************************/
    /* Camera init */
    /***********************************************************************/
    if (error == VDC_OK)
    {
#if ( INPUT_SELECT == CAMERA_OV7670 )
        R_CAMERA_Ov7670Init(GRAPHICS_CAM_IMAGE_SIZE);
#elif ( INPUT_SELECT == CAMERA_OV7740 )
        R_CAMERA_Ov7740Init(GRAPHICS_CAM_IMAGE_SIZE);
#else
#error ERROR: Invalid INPUT_SELECT.
#endif
    }
#endif

    /***********************************************************************/
    /* Graphic Layer 0 VDC_GR_FORMAT_YCBCR422 */
    /***********************************************************************/
    if (error == VDC_OK)
    {
#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
        gr_surface_disp_config_t gr_disp_cnf;

        gr_disp_cnf.layer_id  = VDC_LAYER_ID_0_RD; /* Layer ID                        */
        gr_disp_cnf.fb_buff   = (void *) video_buffer; /* Frame buffer address            */
        gr_disp_cnf.fb_stride = VIDEO_BUFFER_STRIDE; /* Frame buffer stride             */

        /* Display Area               */
        gr_disp_cnf.disp_area.hs_rel = DISP_AREA_HS;
        gr_disp_cnf.disp_area.hw_rel = DISP_AREA_HW;
        gr_disp_cnf.disp_area.vs_rel = DISP_AREA_VS;
        gr_disp_cnf.disp_area.vw_rel = DISP_AREA_VW;

        gr_disp_cnf.read_format   = VDC_GR_FORMAT_YCBCR422; /* Read Format                     */
        gr_disp_cnf.read_ycc_swap = VDC_GR_YCCSWAP_Y0CBY1CR; /* Read Swap for YCbCr422     */
        gr_disp_cnf.read_swap     = VDC_WR_RD_WRSWA_NON; /* Read Swap 8bit/16bit/32bit */

        gr_disp_cnf.clut_table = NULL;  /* Setting if Read Format is CLUT. */

        gr_disp_cnf.disp_mode = VDC_DISPSEL_CURRENT;   /* Display mode select        */

        error = R_RVAPI_GraphCreateSurfaceVDC(vdc_ch, &gr_disp_cnf);

#elif ( TARGET_BOARD == TARGET_BOARD_RSK )
        v_surface_config_t v_cnf;
        v_surface_disp_config_t v_disp_cnf;

        /* Capture area */
        v_cnf.cap_area.vs = CAP_VDC_VS;
        v_cnf.cap_area.vw = CAP_VDC_VW;
        v_cnf.cap_area.hs = CAP_VDC_HS;
        v_cnf.cap_area.hw = CAP_VDC_HW;

        /* Write */
        v_cnf.layer_id = VDC_LAYER_ID_0_WR; /* Layer ID */
        v_cnf.fb_buff = video_buffer; /* Video frame buffer address  */
        v_cnf.fb_stride = VIDEO_BUFFER_STRIDE; /* Write buffer stride[byte]   */
        v_cnf.fb_offset = VIDEO_BUFFER_STRIDE * VIDEO_BUFFER_HEIGHT; /* Write buffer offset[byte] */
        v_cnf.fb_num = VIDEO_BUFFER_NUM; /* Frame buffer num            */
        v_cnf.write_format = CAP_VDC_WFORMAT; /* Write format               */
        v_cnf.write_swap = VDC_WR_RD_WRSWA_NON; /* Write swap 8bit/16bit/32bit */
        v_cnf.write_fb_vw = CAP_VDC_SIZE_VW; /* 1/1 Capture (480u/2u) -> scale down -> 240u out */
        v_cnf.write_fb_hw = CAP_VDC_SIZE_HW; /* 1/2 Capture (720u*2u) -> scale down -> 720u out */
        v_cnf.res_inter = CAP_VDC_FIELD; /* Field operating mode select */
        v_cnf.write_rot = IMGC_ROTATION_MODE; /* Field operating mode select */

        /* display area */
        if (CAP_VDC_SIZE_HW < LCD_CH0_DISP_HW)
        {
            v_disp_cnf.disp_area.vs_rel = 0u;
            v_disp_cnf.disp_area.vw_rel = CAP_VDC_SIZE_VW * 2;
            v_disp_cnf.disp_area.hs_rel = 0u;
            v_disp_cnf.disp_area.hw_rel = CAP_VDC_SIZE_HW;
        }
        else
        {
            v_disp_cnf.disp_area.vs_rel = 0u;
            v_disp_cnf.disp_area.vw_rel = LCD_CH0_DISP_VW;
            v_disp_cnf.disp_area.hs_rel = 0u;
            v_disp_cnf.disp_area.hw_rel = LCD_CH0_DISP_HW;
        }

        /* read */
        v_disp_cnf.read_ycc_swap = VDC_GR_YCCSWAP_CBY0CRY1; /* Read Swap for YCbCr422 */
        v_disp_cnf.read_swap = VDC_WR_RD_WRSWA_NON; /* Read Swap 8bit/16bit/32bit */
        error = R_RVAPI_VideoCreateSurfaceVDC (vdc_ch, &v_cnf, &v_disp_cnf);

#else
#error ERROR: Invalid board defined.
#endif
    }

#if ( TARGET_BOARD == TARGET_BOARD_RSK )
    /* Video Quality Adjustment */
    if (VDC_OK == error)
    {
        error = video_quality_adjustment (vdc_ch);
    }
#endif

    /* Image Quality Adjustment */
    if (VDC_OK == error)
    {
        error = r_image_quality_adjustment(vdc_ch);
    }

    /* Enable signal output */
    if (VDC_OK == error)
    {
        /* Wait for register update */
        R_OS_TaskSleep (16);

        R_RVAPI_DispPortSettingVDC(vdc_ch, &VDC_LcdPortSetting);
    }

    while (1)
    {
#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
        R_RVAPI_CaptureStartCEU ((void *) video_buffer, NULL, VIDEO_BUFFER_STRIDE);
        while (R_RVAPI_CaptureStatusCEU () == CAP_BUSY)
        {
            R_OS_TaskSleep (2);
        }

#elif ( TARGET_BOARD == TARGET_BOARD_RSK )
        R_OS_TaskSleep (5);
#else
#error ERROR: Invalid board defined.
#endif
    }
}
/*******************************************************************************
 End of function graphics_sample_task
 ******************************************************************************/
/****************************************************************************/
/* Function Name : sdk_camera_graphics_uninit                               */
/* Explanation   : Un-initialize screen for SDK for Camera application      */
/* PARAMETERS    : none                                                     */
/* RETURNS       : none                                                     */
/* NOTES         : Especially, none.                                        */
/****************************************************************************/
void sdk_camera_graphics_uninit(void)
{
    vdc_error_t error;
    vdc_channel_t vdc_ch = VDC_CHANNEL_0;

    /***********************************************************************/
    /* display init (VDC output setting) */
    /***********************************************************************/
    {
#if ( TARGET_BOARD == TARGET_BOARD_RSK )
        error = R_RVAPI_VideoDestroySurfaceVDC(vdc_ch, VDC_LAYER_ID_0_WR);
        R_OS_TaskSleep(100);
#endif
        error = R_RVAPI_TerminateVDC(vdc_ch);
        (void)error;

#if ( TARGET_BOARD == TARGET_BOARD_STREAM_IT2 )
        R_RVAPI_TerminateCEU();
#endif
    }

}
