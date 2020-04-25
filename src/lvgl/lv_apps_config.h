/*
 * lv_apps_config.h
 *
 *  Created on: Apr 24, 2020
 *      Author: Oceania
 */

#ifndef LVGL_LV_APPS_CONFIG_H_
#define LVGL_LV_APPS_CONFIG_H_

#define APP_ENABLE	1
#define APP_DISABLE	0

#define LV_USE_TUTORIALS	APP_ENABLE

#define LV_TUTORIAL_HELLOWORLD  0
#define LV_TUTORIAL_OBJECTS		1
#define LV_TUTORIAL_STYLES		2
#define LV_TUTORIAL_THEMES		3
#define LV_TUTORIAL_ANTIALIASING 4
#define LV_TUTORIAL_IMAGES		5
#define LV_TUTORIAL_FONTS		6
#define LV_TUTORIAL_ANIMATIONS	7
#define LV_TUTORIAL_RESPONSIVE	8

#define LV_EXAMPLE_APP LV_TUTORIAL_RESPONSIVE

#if (LV_EXAMPLE_APP == LV_TUTORIAL_HELLOWORLD)
	#include "1_hello_world/lv_tutorial_hello_world.h"
	#define lv_example	lv_tutorial_hello_world
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_OBJECTS)
	#include "2_objects/lv_tutorial_objects.h"
	#define lv_example lv_tutorial_objects
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_STYLES)
	#include "3_styles/lv_tutorial_styles.h"
	#define lv_example lv_tutorial_styles
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_THEMES)
	#include "4_themes/lv_tutorial_themes.h"
	#define lv_example lv_tutorial_themes
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_ANTIALIASING)
	#include "5_antialiasing/lv_tutorial_antialiasing.h"
	#define lv_example lv_tutorial_antialiasing
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_IMAGES)
	#include "6_images/lv_tutorial_images.h"
	#define lv_example lv_tutorial_image
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_FONTS)
	#include "7_fonts/lv_tutorial_fonts.h"
	#define lv_example lv_tutorial_fonts
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_ANIMATIONS)
	#include "8_animations/lv_tutorial_animations.h"
	#define lv_example lv_tutorial_animations
#elif (LV_EXAMPLE_APP == LV_TUTORIAL_RESPONSIVE)
	#include "9_responsive/lv_tutorial_responsive.h"
	#define lv_example lv_tutorial_responsive
#endif




#endif /* LVGL_LV_APPS_CONFIG_H_ */
