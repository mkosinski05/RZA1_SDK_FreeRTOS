/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef RGA_PLATFORM_H_
#define RGA_PLATFORM_H_

#define RGA_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.66f)
#define RGA_DEFAULT_SCREEN_WIDTH  800
#define RGA_DEFAULT_SCREEN_HEIGHT 480
#define RGA_DEFAULT_BUFFERS 2

#ifdef FRAME_BUFFER_BITS_PER_PIXEL_16
    #define RGA_DEFAULT_BITMAP_TYPE PIXEL_FORMAT_RGB565
    #define RGA_DEFAULT_BIT_DEPTH 2
#else
    #define RGA_DEFAULT_BITMAP_TYPE PIXEL_FORMAT_ARGB8888
    #define RGA_DEFAULT_BIT_DEPTH 4
#endif

#endif /* RGA_PLATFORM_H_ */
