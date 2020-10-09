/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPDAVEWIN__H_
#define GFXWRAPDAVEWIN__H_


#include "GfxWrapDave.h"
#include "GfxEnvWin.h"

/// D/AVE implementation of the graphics wrapper for Windows.

/**
This is the D/AVE implementation of the graphics wrapper for Windows.
For details see desription of its base class CGfxWrapDave.
*/
class CGfxWrapDAVEWin :
    public CGfxWrapDAVE, protected CGfxEnvWin
{
public:
    /**
        Creates an instance of CGfxWrapDAVEWin object and sets it as the active graphics wrapper
        @param hWnd The window instance for this wrapper.
        @param uiScreenWidth The screen width.
        @param uiScreenHeight The screen height.
        @return Indicate whether by this call, an instance is created.
                The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd, const eC_UInt &uiScreenWidth, const eC_UInt &uiScreenHeight);

    virtual void SetCliprect(const CGUIRect &NewClipRect);
    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    virtual void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

private:
    // Copies the contents of the D/AVE buffer to the windows memory DC
    void RefreshMemoryDC();

    CGfxWrapDAVEWin(const HWND &hWnd);
    virtual ~CGfxWrapDAVEWin(void);
};
#endif
