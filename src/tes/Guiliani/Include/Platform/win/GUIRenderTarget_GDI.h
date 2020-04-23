/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIRENDERTARGET_GDI_H__
#define GUIRENDERTARGET_GDI_H__

#include "GUIRenderTarget.h"
#include <windows.h>

/// @brief Render target for GDI graphics
class CGUIRenderTarget_GDI : public CGUIRenderTarget
{
public:
    /** Constructor
        @param hRenderDC Render device context
    */
    CGUIRenderTarget_GDI(HDC hRenderDC):m_hRenderDC(hRenderDC){}

    ///@return Render device context
    inline HDC GetRenderBitmap(){return m_rRenderBitmap;}

private:
    HDC m_hRenderDC;
};

#endif //GUIRENDERTARGET_GDI_H__
