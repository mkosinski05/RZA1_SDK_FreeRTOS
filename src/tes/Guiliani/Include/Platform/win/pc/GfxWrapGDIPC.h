/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPGDIPC__H_
#define GFXWRAPGDIPC__H_

#include "GfxWrapGDI.h"


/// This is the GDI-Windows implementation of the graphics wrapper.

/**
    For details see desription of its base class CGfxWrap.
*/
class CGfxWrapGDIPC :
    public CGfxWrapGDI
{
public:
    /**
    Singleton for creating a GfxWrap-GDI-PC object and change the pointer in
    base class to this graphics wrapper.
    @param hWnd The window instance for this wrapper.
    @param uiScreenWidth The screen width.
    @param uiScreenHeight The screen height.
    @return Indicate whether by this call, an instance is created.
            The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd, const eC_UInt &uiScreenWidth, const eC_UInt &uiScreenHeight);

    virtual void LoadImgImpl (const eC_String& psPath, const ImageResource_t &eID);
    virtual void Ellipse(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2, const eC_Bool &bFilled);

protected:
    /** Constructor
        @param hWnd Window handle
    */
    CGfxWrapGDIPC(const HWND &hWnd);
    virtual ~CGfxWrapGDIPC(void);

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapGDIPC(const CGfxWrapGDIPC& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapGDIPC& operator=(const CGfxWrapGDIPC& kSource);
};
#endif
