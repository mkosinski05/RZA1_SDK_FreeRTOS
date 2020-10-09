/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPGDIPocketPC__H_
#define GFXWRAPGDIPocketPC__H_

#include "..\GfxWrapGDI.h"


/// This is the GDI-Windows CE implementation of the graphics wrapper.

/**
For details see desription of its base class CGfxWrap.
*/
class CGfxWrapGDIPocketPC :
    public CGfxWrapGDI
{
public:
    /**
        Singleton for creating a GfxWrap-GDI-PocketPC object and change the pointer in
        base class to this graphic wrap.
        @param hWnd Window handle
        @return Indicate whether by this call, an instance is created.
                The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd);

    virtual void LoadImgImpl(const eC_String& psPath, const ImageResource_t &eID);

    virtual void Ellipse(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2,
        const eC_Bool &bFilled);
protected:
    /** Constructor
        @param hwnd Window handle
    */
    CGfxWrapGDIPocketPC(const HWND& hwnd);

    virtual ~CGfxWrapGDIPocketPC(void);

};
#endif
