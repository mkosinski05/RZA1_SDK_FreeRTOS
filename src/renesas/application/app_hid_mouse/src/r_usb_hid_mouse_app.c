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
* File Name     : r_usb_hid_demo_app.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-RZv14.01-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : CDC display board sample
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.08.2015 1.00    Initial Version
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
#include "r_usb_hid.h"

#include "r_usb_hid_mouse_app.h"
#include "r_usb_mouse_descriptors.h"

#include "main.h"
#include "trace.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
#undef TRACE
#define TRACE(x)
#endif

/******************************************************************************
Macro definitions
******************************************************************************/

/*Size of buffer - the USB packet size is an efficient size to choose*/
#define BUFFER_SIZE (BULK_OUT_PACKET_SIZE)

/* Size of legacy LCD display, used so host can update PMOD display */
#define NUMB_CHARS_PER_LINE     (8)

/* Specify size of table m_mousemove_speed (below) */
#define HID_MOUSE_MOVE_SPEED_TABLE_SIZE (0x05)

/* Number of key repetitions needed to step towards the next
   step speed of mouse movement */
#define MOUSE_MOVE_LAG_BASE (12)


/******************************************************************************
Enumerated Types
******************************************************************************/

typedef enum _hidm_event_t
{
    HID_SAMPLE_CREATED = 0,
    HID_COMMAND_ACTIVE,
    HID_COMMAND_ISR_TRIG,
    HID_NUM_SIGNALS
} hidm_event_t;

/******************************************************************************
Typedefs
******************************************************************************/

typedef struct _hid_mouse_config_t
{
    PEVENT          ppev_signals[HID_NUM_SIGNALS];
    _Bool           bf_active;
    uint32_t        taskid;
} hid_mouse_config_t;

typedef struct _hid_mouse_config_t *phid_mouse_config_t;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern void R_MSG_WarningConfig(pst_comset_t p_com, char_t *msg);

/******************************************************************************
Private global variables and functions
******************************************************************************/

/* Create the console to run the commands */
static pst_comset_t m_pcom = NULL;

/*In Report buffer*/
static uint8_t  m_in_report[HID_IN_REPORT_SIZE];

/* handle to peripheral */
static int_t    m_iusbfdevice = -1;

/******************************************************************************
Function Prototypes
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

static uint8_t  m_usb_real_name[][8]    = {"USB_D_0","USB_D_1"};
static uint8_t  m_usb_device_name[][10] = {"usbf0_hid","usbf1_hid"};

static int16_t cmd_hid_mouse_demo (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom);
void mse_send_in_report(bool_t _button_state, int8_t _xval, int8_t _yval);
static void hid_mouse_console(FILE* pIn, FILE *pOut, char *pszPrompt);


/******************************************************************************
* Table that associates command letters, function pointer and a little
* description of what the command does
******************************************************************************/
static st_cmdfnass_t gs_pcmd_hid_mouse[] =
{
     {
      /* Introduction Banner */
      "hidmouse",
      cmd_hid_mouse_demo,
      "<CR>    - Provides HID mouse implementation, follow on screen instructions\r\n"
     }
};


/* Table that points to the above table and contains the number of entries */
const st_command_table_t g_cmd_tbl_mouse =
{
    "USB HID class Mouse Commands",
    gs_pcmd_hid_mouse,
    ((sizeof(gs_pcmd_hid_mouse))/(sizeof(st_cmdfnass_t)))
};

extern const st_command_table_t g_cmd_tbl_UsbMouse;
extern const st_command_table_t g_cmd_tbl_core;

/* Define the command tables required */
pst_command_table_t spp_mse_Commands[] =
{
     /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_core,

     /* cast to pst_command_table_t */
    (pst_command_table_t) &g_cmd_tbl_UsbMouse
};

int32_t s_num_commands = (sizeof(spp_mse_Commands)) / sizeof(pst_command_table_t);



/******************************************************************************
user Program Code
******************************************************************************/


/******************************************************************************
* Function Name  : create_in_report
* Description    : Create the IN report based on supplied params.
*
* Argument       : _led:        TRUE if providing new LED value.
*                  _adc:        TRUE if providing new ADC value.
*                  _sw_pressed: TRUE if switch pressed.
*                  _adc_value:   ADC Value.
* Return value   : none
******************************************************************************/
static void create_in_report(bool_t _button_state, int8_t _xval, int8_t _yval)
{

    /*Set button Bit*/
       m_in_report[0] = (uint8_t)_button_state;

    /*Set new X POS value */
       m_in_report[1] = (uint8_t) _xval;

    /*Set new X POS value */
       m_in_report[2] = (uint8_t) _yval;
}
/******************************************************************************
End of function create_in_report
******************************************************************************/

/******************************************************************************
* Function Name  : send_in_report
* Description    : Create and then send IN report
* Argument       : none
* Return value   : none
******************************************************************************/
void mse_send_in_report(bool_t _button_state, int8_t _xval, int8_t _yval)
{
    /*Create report*/
    create_in_report(_button_state, _xval, _yval);

    /*Send to host*/
    if((-1) != m_iusbfdevice)
    {
        control(m_iusbfdevice, CTL_USBF_SEND_HID_REPORTIN, &m_in_report);
    }
}
/******************************************************************************
End of function send_in_report
******************************************************************************/

/*******************************************************************************
* Function Name: show_splash_screen
* Description  :
* Arguments    : index -
*                    Where to start looking
* Return Value : none
*******************************************************************************/
static void show_splash_screen(pst_comset_t pCom, uint8_t _hid_device_select, st_usbf_user_configuration_t * config)
{
    uint8_t rem_remport = 0;

    /* Splash Screen */
    fprintf(pCom->p_out,"HID Configuration\r\n");
    fprintf(pCom->p_out,"config.in_report_size [%d]\r\n", config->descriptors.report_in.length);
    fprintf(pCom->p_out,"config.pin_report     [");

    for(rem_remport = 0; rem_remport < config->descriptors.report_in.length; rem_remport++)
    {
        if(rem_remport)
        {
            fprintf(pCom->p_out,",");
        }
        fprintf(pCom->p_out,"0x%x", *(config->descriptors.report_in.puc_data + rem_remport));
    }
    fprintf(pCom->p_out,"]\r\n");

    fprintf(pCom->p_out,"config.p_cbout_report [0x%lx]\r\n", (uint32_t)*(config->pcbout_report));

    fprintf(pCom->p_out,"\r\nHID mouse demonstration\r\n");
    fprintf(pCom->p_out,"===========================\r\n");
    fprintf(pCom->p_out,"Connect PC to USB function connector %s\r\n",  m_usb_real_name[_hid_device_select]);
    fprintf(pCom->p_out,"executing ....\r\n");

}
/*******************************************************************************
End of function show_splash_screen
*******************************************************************************/


/******************************************************************************
 Function Name: hid_mouse_console
 Description:   Function to process the console commands
 Arguments:     IN  pIn - Pointer to the input file stream
                IN  pOut - Pointer to the file stream for echo of command input
                IN  pszPrompt - Pointer to a command prompt
 Return value:  none
 ******************************************************************************/
static void hid_mouse_console(FILE* pIn, FILE *pOut, char *pszPrompt)
{
    pst_comset_t com = R_OS_AllocMem(sizeof(st_comset_t), R_REGION_LARGE_CAPACITY_RAM);
    int32_t pos;

    s_num_commands = (sizeof(spp_mse_Commands)) / sizeof(pst_command_table_t);

    /* remove unused console command lists */
    for (pos = (s_num_commands); pos > 0; pos--)
    {
        /* check for null entry */
        if (NULL == spp_mse_Commands[pos])
        {
            s_num_commands--;
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

            /* Process commands until the link is lost or the command returns a code greater than CMD_ERROR_IN_IO */
            while (cmd_err < CMD_USER_EXIT)
            {
                /* cast gppCommands to cpst_command_table_t * */
                cmd_err = console(com, (cpst_command_table_t *) spp_mse_Commands, s_num_commands, pIn, pOut, pszPrompt);
            }

            /* Check for the "exit<CR>" command return code */
            if (CMD_LOG_OUT == cmd_err)
            {
                fprintf(com->p_out, "\r\nBye!\r\n");
                fflush(pOut);

                /* Do "exit" code */
                 R_OS_FreeMem(com);
                break;
            }
        }
    }
}
/******************************************************************************
 End of function hid_mouse_console
 ******************************************************************************/


/******************************************************************************
* Function Name: cmd_hid_mouse_demo
* Description  : Test task to flash the LEDs
* Arguments    : none
* Return Value : none
******************************************************************************/
static int16_t cmd_hid_mouse_demo (int_t iArgCount, char_t **ppszArgument, pst_comset_t pCom)
{
    (void)ppszArgument;
    (void)iArgCount;

    volatile uint8_t hid_device_select = 0; /* Select usbf0_hid by default */

    volatile int_t i_in = R_DEVLINK_FilePtrDescriptor(pCom->p_in);
    static char prompt[] = "MSE>";

    st_usbf_user_configuration_t config = {};
    int8_t  hasaborted = 0;  /* -1 user abort; 0 default state; 1 cable disconnected */

    m_pcom = pCom;

#if R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER
    R_MSG_WarningConfig(pCom, "R_SELF_LOAD_MIDDLEWARE_USB_HOST_CONTROLLER");
    return CMD_OK;
#endif

    m_iusbfdevice = open((char *)DEVICE_INDENTIFIER "usbf0_hid", O_RDWR, _IONBF);

    if((-1) == m_iusbfdevice)
    {
        /* usbf1_hid device selected */
        hid_device_select = 1;
        m_iusbfdevice = open((char *)DEVICE_INDENTIFIER "usbf1_hid", O_RDWR, _IONBF);
    }

    if((-1) != m_iusbfdevice)
    {
        fprintf(pCom->p_out,"\r\n*** HID open %s ***\r\n", m_usb_device_name[hid_device_select]);

        /* Get the default configuration for HID device */
        if(0 == control(m_iusbfdevice, CTL_USBF_GET_CONFIGURATION, &config))
        {
            /* Update the Reports and call backs */
            create_in_report(0,0,0);

            config.pcbout_report  = NULL;

            config.hi_speed_enable = 0; /* Disable hi_spped mode as mouse only supports Full Speed mode */

            config.descriptors.config.puc_data               = g_usbf_md_config_desc.puc_data;
            config.descriptors.config.length                 = g_usbf_md_config_desc.length;
            config.descriptors.dev_qualifier.puc_data        = g_usbf_md_device_qualifier_desc.puc_data;
            config.descriptors.dev_qualifier.length          = g_usbf_md_device_qualifier_desc.length;
            config.descriptors.device.puc_data               = g_usbf_md_device_desc.puc_data;
            config.descriptors.device.length                 = g_usbf_md_device_desc.length;
            config.descriptors.other_speed_config.puc_data   = g_usbf_md_other_speed_cfg_desc.puc_data;
            config.descriptors.other_speed_config.length     = g_usbf_md_other_speed_cfg_desc.length;
            config.descriptors.string_manufacturer.puc_data  = g_usbf_md_str_desc_manufacturer.puc_data;
            config.descriptors.string_manufacturer.length    = g_usbf_md_str_desc_manufacturer.length;
            config.descriptors.string_product.puc_data       = g_usbf_md_str_desc_product.puc_data;
            config.descriptors.string_product.length         = g_usbf_md_str_desc_product.length;
            config.descriptors.string_serial.puc_data        = g_usbf_md_str_desc_serial_num.puc_data;
            config.descriptors.string_serial.length          = g_usbf_md_str_desc_serial_num.length;

            config.descriptors.hid_report_in.puc_data         = g_usbf_md_hid_report_descriptor.puc_data;
            config.descriptors.hid_report_in.length             = g_usbf_md_hid_report_descriptor.length;

            config.descriptors.report_in.puc_data              = g_usbf_md_inreport.puc_data;
            config.descriptors.report_in.length                 = g_usbf_md_inreport.length;;

            config.descriptors.report_out.puc_data             = g_usbf_md_outreport.puc_data;
            config.descriptors.report_out.length             = g_usbf_md_outreport.length;;


            /* Set the configuration for the HID device */
            if(0 == control(m_iusbfdevice, CTL_USBF_SET_CONFIGURATION, &config))
            {
                /* Flush any remaining characters from the buffer */
                while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) > 0)
                {
                    fgetc(pCom->p_in);
                }

                /* Start usb function device */
                control(m_iusbfdevice, CTL_USBF_START, NULL);

                show_splash_screen(pCom, hid_device_select, &config);

                /* allow usb to enumerate if cable is connected */
                R_OS_TaskSleep(600);

                if(FALSE == control(m_iusbfdevice, CTL_USBF_IS_CONNECTED, NULL))
                {
                    fprintf(pCom->p_out,"*** waiting for cable to be connected ***\r\n");
                }

                while(0 == hasaborted)
                {
                    if(1 == control(m_iusbfdevice, CTL_USBF_IS_CONNECTED, NULL))
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

                if((-1) == hasaborted)
                {
                    fprintf(pCom->p_out,"*** %s user abort ***\r\n", m_usb_device_name[hid_device_select]);
                }

                if(1 == hasaborted)
                {
                    fprintf(pCom->p_out,"*** %s cable connected *** \r\n", m_usb_device_name[hid_device_select]);
                    hasaborted = 0;
                }

                fprintf(pCom->p_out,"usb mouse function active\r\n");
                fprintf(pCom->p_out,"type 'help' ot '?' for list of commands\r\n");
                fprintf(pCom->p_out,"type 'logout' to terminate demo\r\n");

                hid_mouse_console(pCom->p_in, pCom->p_out, prompt);

                printf("\r\nHID Mouse terminated.\r\n");
            }

            control(m_iusbfdevice, CTL_USBF_STOP, NULL);

            close(m_iusbfdevice);
            m_iusbfdevice    = -1;

        }
    }
    else
    {
        fprintf(pCom->p_out,"\r\n*** HID failed to open %s or %s  ***\r\n", m_usb_device_name[0], m_usb_device_name[1]);
        fprintf(pCom->p_out,"maybe device isn't registered in devlink.c or the device is in use.\r\n");
    }
    return CMD_OK;
}
/******************************************************************************
End of function  cmd_hid_mouse_demo
******************************************************************************/

/* End of File */

