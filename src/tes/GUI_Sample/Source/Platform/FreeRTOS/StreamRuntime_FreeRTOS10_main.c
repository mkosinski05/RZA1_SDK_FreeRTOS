/* Dependencies */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "freertos.h"
#include "iodefine_cfg.h"

#include "compiler_settings.h"
#include "control.h"
#include "version.h"

#include "spibsc_iobitmask.h"
#include "cpg_iobitmask.h"
#include "gpio_iobitmask.h"

/* STDIO devlink driver drivers */
#include "r_led_drv_api.h"
/* End of STDIO devlink driver drivers */

#include "command.h"
#include "r_task_priority.h"

static st_iostr_t gs_iostr[3];

extern void GuilianiStart(void);
extern void GUI_Sample(void);

extern void GRAPHIC_init_screen(void);

static void prvGuilianiTask( void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    pst_comset_t pCom = (pst_comset_t)pvPortMalloc(sizeof(pst_comset_t));
    /* Initialise the console for login */
    memset(pCom, 0, sizeof(pst_comset_t));
    pCom->p_in = gs_iostr[0].p_in;
    pCom->p_out = gs_iostr[0].p_out;

    GRAPHIC_init_screen();

    GuilianiStart();

    while (1)
    {
    }
}

int_t gui_main(void)
{
    /* BEGIN -- enable NEON */
#if defined(__ICCARM__)
    asm (
     "MRC p15,0,r0,c1,c0,2  \n"  // Read CP Access register
     "ORR r0,r0,#0x00f00000 \n"  // Enable full access to NEON/VFP (Coprocessors 10 and 11)
     "MCR p15,0,r0,c1,c0,2  \n"  // Write CP Access register
     "ISB                     \n"
     "MOV r3,#0x40000000    \n"  // Switch on the VFP and NEON hardware
     //MSR FPEXC,r3              // Set EN bit in FPEXC
     "DC32 0xeee83a10      \n"  /* vmsr FPEXC, r3 */
    );
#else
    /* BEGIN -- enable NEON */
    __asm__ (
     "MRC p15,0,r0,c1,c0,2  \n"  // Read CP Access register
     "ORR r0,r0,#0x00f00000 \n"  // Enable full access to NEON/VFP (Coprocessors 10 and 11)
     "MCR p15,0,r0,c1,c0,2  \n"  // Write CP Access register
     "ISB                     \n"
     "MOV r3,#0x40000000    \n"  // Switch on the VFP and NEON hardware
     //MSR FPEXC,r3              // Set EN bit in FPEXC
     ".long 0xeee83a10      \n"  /* vmsr FPEXC, r3 */
    );
#endif
    /* END -- enable NEON */

    gs_iostr[0].p_in = stdin;
    gs_iostr[0].p_out = stdout;

    os_task_t *p_os_task;
    p_os_task = R_OS_CreateTask("Guiliani", prvGuilianiTask, NULL, GUILIANI_TASK_STACK_SIZE, 1);

    /* Failed to create the task? */
    if (R_OS_ABSTRACTION_PRV_INVALID_HANDLE == (int)p_os_task)
    {
        /* Debug message */
        printf("Failed to create task!\r\n");
    }

    /* NULL signifies that no task was created by R_OS_CreateTask */
    if (NULL == p_os_task)
    {
        /* Debug message */
        printf("Failed to create task!\r\n");
    }

    /* Need to determine system state is running */
    if (R_OS_GetNumberOfTasks())
    {
        while (1)
        {
            R_OS_TaskSleep(10000);
        }
    }

    // todo
    /* Shut down all system resources */

    /* This function should never return */
    while (1)
    {
    }
    return 0;
}
