/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if defined(USE_STREAM_IT_RZ)

/*********************
 *      INCLUDES
 *********************/
#include "FreeRTOS.h"
#include "mcu_board_select.h"
#include "r_vdc_portsetting.h"
#include "r_rvapi_header.h"
#include "r_display_init.h"
#include "lv_conf.h"
#include "src/lv_misc/lv_color.h"
#include "src/lv_hal/lv_hal_disp.h"

/*********************
 *      DEFINES
 *********************/
/* Display area */
#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
#define DATA_SIZE_PER_PIC      (2u)
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
#define DATA_SIZE_PER_PIC      (4u)
#else
#error "Set bits per pixel"
#endif

#define FRAMEBUFFER_WIDTH   480
#define FRAMEBUFFER_HEIGHT  272

#define FRAMEBUFFER_LAYER_NUM      (2u)
#define FRAMEBUFFER_STRIDE  (((FRAMEBUFFER_WIDTH * DATA_SIZE_PER_PIC) + 31u) & ~31u)
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static volatile int32_t vsync_count = 0;
static int draw_buffer_index = 0;
static int gui_buffer_index = 1;

uint8_t framebuffer[FRAMEBUFFER_LAYER_NUM][FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT] __attribute__ ((section(".VRAM_SECTION0")));


static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
#if LV_USE_GPU
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
        const lv_area_t * fill_area, lv_color_t color);
#endif

// graphic buffer cursor_buffer is already defined in BSP/src/renesas/application/app_touchscreen/r_drawrectangle.c
volatile uint8_t *RZAFrameBuffers[2] =
{
    framebuffer[1],
    framebuffer[0],
};

static void IntCallbackFunc_LoVsync(vdc_int_type_t int_type)
{
    if (vsync_count > 0)
    {
        vsync_count--;
    }
}

static void Wait_Vsync(const int32_t wait_count)
{
    vsync_count = wait_count;
    while (vsync_count > 0)
    {
        vTaskDelay( 2 / portTICK_PERIOD_MS);
    }
}

/* Set / switch framebuffer */
static void GrpDrv_SetFrameBuffer(void * ptr)
{
    if (draw_buffer_index == 1) {
        draw_buffer_index = 0;
        gui_buffer_index = 1;
    } else {
        draw_buffer_index = 1;
        gui_buffer_index = 0;
    }

    vdc_error_t error = R_RVAPI_GraphChangeSurfaceVDC(VDC_CHANNEL_0, VDC_LAYER_ID_0_RD, (void*)framebuffer[draw_buffer_index]);
    if (VDC_OK == error)
    {
         Wait_Vsync(1);
    }
}

void GRAPHIC_PutPixel(uint32_t x, uint32_t y, uint8_t *pColor)
{

    for (int byte = 0; byte < DATA_SIZE_PER_PIC; ++byte)
    {
        framebuffer[gui_buffer_index][(y * FRAMEBUFFER_STRIDE) + x * DATA_SIZE_PER_PIC + byte] = pColor[byte];

    }
}

void GRAPHIC_Clear(void)
{
    memset(framebuffer[draw_buffer_index], 0x0, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);
}

static void GRAPHIC_init_screen(void)
{
    vdc_error_t error;
    vdc_channel_t vdc_ch = VDC_CHANNEL_0;

    /***********************************************************************/
    /* display init (VDC output setting) */
    /***********************************************************************/
    {
        error = r_display_init (vdc_ch);
    }
    if (error == VDC_OK)
    {
        error = R_RVAPI_InterruptEnableVDC(vdc_ch, VDC_INT_TYPE_S0_LO_VSYNC, 0, IntCallbackFunc_LoVsync);
    }
    /***********************************************************************/
    /* Graphic Layer 2 CLUT8 */
    /***********************************************************************/
    if (error == VDC_OK)
    {
        gr_surface_disp_config_t gr_disp_cnf;
        uint32_t  clut_table[4] = {
                0x00000000, /* No.0 transparent color  */
                0xFF000000, /* No.1 black */
                0xFF00FF00, /* No.2 green */
                0xFFFF0000  /* No.3 red */
        };

        /* buffer clear */
        // Set frame buffer to black
        memset((void*)framebuffer[0], 0x00, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);
        memset((void*)framebuffer[1], 0x00, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);

#if (1) /* not use camera captured layer */
        gr_disp_cnf.layer_id         = VDC_LAYER_ID_0_RD;
#else   /* blend over camera captured image */
        gr_disp_cnf.layer_id         = VDC_LAYER_ID_2_RD;
#endif
        gr_disp_cnf.disp_area.hs_rel = 0;
        gr_disp_cnf.disp_area.hw_rel = FRAMEBUFFER_WIDTH;
        gr_disp_cnf.disp_area.vs_rel = 0;
        gr_disp_cnf.disp_area.vw_rel = FRAMEBUFFER_HEIGHT;
        gr_disp_cnf.fb_buff          = &framebuffer[0];
        gr_disp_cnf.fb_stride        = FRAMEBUFFER_STRIDE;
        gr_disp_cnf.read_format      = VDC_GR_FORMAT_CLUT8;
#if defined FRAME_BUFFER_BITS_PER_PIXEL_16
        gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB565;
#elif defined FRAME_BUFFER_BITS_PER_PIXEL_32
        gr_disp_cnf.read_format      = VDC_GR_FORMAT_RGB888;
#endif
        gr_disp_cnf.clut_table       = clut_table;
        gr_disp_cnf.read_ycc_swap    = VDC_GR_YCCSWAP_CBY0CRY1;
        gr_disp_cnf.read_swap        = VDC_WR_RD_WRSWA_32_16BIT;
#if (1) /* not use camera captured data */
        gr_disp_cnf.disp_mode        = VDC_DISPSEL_CURRENT;
#else   /* blend over camera captured image */
        gr_disp_cnf.disp_mode        = VDC_DISPSEL_BLEND;
#endif
        error = R_RVAPI_GraphCreateSurfaceVDC(vdc_ch, &gr_disp_cnf);

        GRAPHIC_Clear();
    }

    /* Image Quality Adjustment */
    if (VDC_OK == error)
    {
        error = r_image_quality_adjustment(vdc_ch);
    }

    /* Enable signal output */
    if (VDC_OK == error)
    {
        /* Wait for register update */
        R_OS_TaskSleep(5);

        R_RVAPI_DispPortSettingVDC(vdc_ch, &VDC_LcdPortSetting);
    }
}
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
	vdc_error_t error;
	vdc_channel_t vdc_ch = VDC_CHANNEL_0;
    /*-------------------------
     * Initialize your display
     * -----------------------*/
	GRAPHIC_init_screen();


    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /* LittlevGL requires a buffer where it draws the objects. The buffer's has to be greater than 1 display row
     *
     * 
     * 3. Create TWO screen-sized buffer: 
     *      Similar to 2) but the buffer have to be screen sized. When LittlevGL is ready it will give the
     *      whole frame to display. This way you only need to change the frame buffer's address instead of
     *      copying the pixels.
     * */


    /* Example for 3) */
    static lv_disp_buf_t disp_buf_3;
    static lv_color_t *pbuf3_1=framebuffer[0];            /*A screen sized buffer*/
    static lv_color_t *pbuf3_2=framebuffer[1];            /*An other screen sized buffer*/
    lv_disp_buf_init(&disp_buf_3, pbuf3_1, pbuf3_2, LV_HOR_RES_MAX * LV_VER_RES_MAX);   /*Initialize the display buffer*/


    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf_3;

#if LV_USE_GPU
    /*Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)*/

    /*Blend two color array using opacity*/
    disp_drv.gpu_blend_cb = gpu_blend;

    /*Fill a memory array with a color*/
    disp_drv.gpu_fill_cb = gpu_fill;
#endif

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished. */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
        	GRAPHIC_PutPixel(x,y,color_p);
            color_p++;
        }
    }

    GrpDrv_SetFrameBuffer(NULL);
    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}


/*OPTIONAL: GPU INTERFACE*/
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/
    uint32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
                    const lv_area_t * fill_area, lv_color_t color)
{
    /*It's an example code which should be done by your GPU*/
    int32_t x, y;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

    for(y = fill_area->y1; y <= fill_area->y2; y++) {
        for(x = fill_area->x1; x <= fill_area->x2; x++) {
            dest_buf[x] = color;
        }
        dest_buf+=dest_width;    /*Go to the next line*/
    }
}

#endif  /*LV_USE_GPU*/

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
