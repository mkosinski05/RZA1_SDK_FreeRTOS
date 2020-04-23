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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * @file         pal.h
 * @version      0.01
 * $Rev:  $
 * $Date:: #$
 * @brief        Camera for vdc5 definition header
 ******************************************************************************/
#ifndef PAL_H
#define PAL_H

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include    "r_vdc5.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* External input format select */
#define CAP_VDC5_MODE       (VDC5_EXTIN_FORMAT_BT601)    /* Not use */

/* Polarity of a signal */
#define CAP_VDC5_HPOL       (VDC5_SIG_POL_NOT_INVERTED)  /* Not use */
#define CAP_VDC5_VPOL       (VDC5_SIG_POL_NOT_INVERTED)  /* Not use */

/* Y/Cb/Y/Cr data string start timing */
#define CAP_VDC5_DTARY      (VDC5_EXTIN_H_POS_CBYCRY)    /* Not use */

/* Capture timing */
#define CAP_VDC5_VS         (16u)                        /* use */
#define CAP_VDC5_HS         (122u * 2u)                  /* use */
#define CAP_VDC5_VW         (560u/2u)                    /* use */
#define CAP_VDC5_HW         (720u * 2u)                  /* use */

/* Clock edge select for capturing external input video image signalsl */
#define CAP_VDC5_DATAEGDE   (VDC5_EDGE_RISING)           /* Not use */

/* External input B/R signal swap on/off control */
#define CAP_VDC5_PORTSWAP   (VDC5_OFF)                   /* Not use */

/* External input bit endian change on/off control */
#define CAP_VDC5_PORTENDIAN (VDC5_OFF)                   /* Not use */

/* Field operating mode select */
#define CAP_VDC5_FIELD      (VDC5_RES_INTER_INTERLACE)   /* use */

/* Capture resolution */
#define CAP_VDC5_SIZE_VW    (280u)                       /* use */
#define CAP_VDC5_SIZE_HW    (720u)                       /* use */

/* Frame buffer video-signal writing format */
#define CAP_VDC5_WFORMAT    (VDC5_RES_MD_YCBCR422)       /* use */

#endif  /*  PAL_H */
