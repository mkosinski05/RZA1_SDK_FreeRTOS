/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIRENDERTARGET_EGMLGDI_H__
#define GUIRENDERTARGET_EGMLGDI_H__

#include "GUIRenderTarget_eGML.h"
#include "GUIRenderTarget_GDI.h"

/// @brief Render target for eGML GDI graphics
class CGUIRenderTarget_eGMLGDI : public CGUIRenderTarget_eGML, CGUIRenderTarget_GDI
{
public:
    /** Constructor
        @param hRenderDC Render device context
        @param rRenderBitmap Render bitmap
    */
    CGUIRenderTarget_eGMLGDI(eGML_Bitmap& rRenderBitmap, HDC hRenderDC) :
    CGUIRenderTarget_eGML(rRenderBitmap),
    CGUIRenderTarget_GDI(hRenderDC)
    {};
};

#endif //GUIRENDERTARGET_EGMLGDI_H__
