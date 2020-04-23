/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBACKGROUNDIMAGELOADERTHREAD__H_
#define GUIBACKGROUNDIMAGELOADERTHREAD__H_

#include "GUIThread.h"

/** The BackgroundImageLoaderThread loads images in a separate low-priority thread.
    It is created upon the first use of AddToPrefetchQueue() and uses the Graphics-Wrapper's LoadImg() method
    to load the requested images.
    @brief Load images in a separate thread
*/
class CGUIBackgroundImageLoaderThread
    : public CGUIThread
{
    public:
        CGUIBackgroundImageLoaderThread();
        virtual void Execute(void *pArg);
        static eC_UInt ms_uiDelay;///< loop delay
};


#endif
