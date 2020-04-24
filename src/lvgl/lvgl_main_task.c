#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "freertos.h"

#include "lvgl/lvgl.h"
#include "porting/lv_port_disp.h"
#include "lv_demo_hello_world/lv_tutorial_hello_world.h"

#define LVGL_TASK_STACK_SIZE 20480

static void prvLittlevGLTask( void *pvParameters );

void lvgl_start ( void ) {

	os_task_t *p_os_task;
	p_os_task = R_OS_CreateTask("Guiliani", prvLittlevGLTask, NULL, LVGL_TASK_STACK_SIZE, 1);


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

}

static void prvLittlevGLTask( void *pvParameters ) {

	// Initialize lvgl and Display
	lv_init();
	lv_port_disp_init();

	//touchpad_init();

	lv_tutorial_hello_world();

	while (1) {
		// Called every millisecond by the
		lv_task_handler();
		R_OS_TaskSleep(1);
	}
}
