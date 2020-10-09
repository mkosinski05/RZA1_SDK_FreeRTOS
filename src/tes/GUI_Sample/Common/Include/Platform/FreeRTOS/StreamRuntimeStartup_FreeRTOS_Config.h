/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#define    GUILIANI_TASK_PRIORITY        ( tskIDLE_PRIORITY + 3 )

// Resources FLASH section, size can be adapted here if needed
extern const uint8_t __attribute__ ((section (".RES_FLASH"))) __guilianiResources[0x3000000];

// Size of frame buffer
#ifdef PLATFORM_RSK_RZA1
    #define FRAME_BUFFER_WIDTH    800
    #define FRAME_BUFFER_HEIGHT   480
#else
    #define FRAME_BUFFER_WIDTH    480
    #define FRAME_BUFFER_HEIGHT   272
#endif
