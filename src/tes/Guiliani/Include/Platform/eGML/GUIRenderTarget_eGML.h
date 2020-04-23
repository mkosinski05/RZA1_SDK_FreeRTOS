/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIRENDERTARGET_EGML_H__
#define GUIRENDERTARGET_EGML_H__

#include "GUIRenderTarget.h"
class eGML_Bitmap;

/// @brief Render target for eGML graphics
class CGUIRenderTarget_eGML : public CGUIRenderTarget
{
public:
    /** Constructor
        @param rRenderBitmap The render bitmap
    */
    CGUIRenderTarget_eGML(eGML_Bitmap& rRenderBitmap):m_rRenderBitmap(rRenderBitmap){}

    ///@return The render bitmap
    inline eGML_Bitmap& GetRenderBitmap() const {return m_rRenderBitmap;}

private:
    /** Default-constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
    */
    CGUIRenderTarget_eGML();

    eGML_Bitmap& m_rRenderBitmap;
};

#endif //GUIRENDERTARGET_EGML_H__
