/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIBITMAPGL__H_
#define GUIBITMAPGL__H_

#include "GUIBitmapPlane.h"

class CGfxWrapGL;

/** @brief The BitmapPlane Class for GL with pseudo offscreen rendering.
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_GL : public CGUIBitmapPlane
{
    friend class CGfxWrapGL;

public:
    void DoDraw();

    virtual void SetWidth(eC_Value vWidth, eC_Bool bExpandCentered = false)
    {
        CGUIBitmapPlane::SetWidth(vWidth, bExpandCentered);
        UpdateVertices();
    }

    virtual void SetHeight(eC_Value vHeight, eC_Bool bExpandCentered = false)
    {
        CGUIBitmapPlane::SetHeight(vHeight, bExpandCentered);
        UpdateVertices();
    }

    virtual void SetAlphaValues(const eC_UByte ubTopLeft, const eC_UByte ubBottomLeft,
                                    const eC_UByte ubBottomRight, const eC_UByte ubTopRight)
    {
        if(m_aubColor.GetQuantity() == 0)
        {
            // Four vertices with 4 Byte (RGBA) each
            m_aubColor.SetQuantity(16, false);
            m_aubColor[0] = 255;
            m_aubColor[1] = 255; 
            m_aubColor[2] = 255;
            m_aubColor[4] = 255;
            m_aubColor[5] = 255;
            m_aubColor[6] = 255;
            m_aubColor[8] = 255;
            m_aubColor[9] = 255;
            m_aubColor[10] = 255;
            m_aubColor[12] = 255;
            m_aubColor[13] = 255;
            m_aubColor[14] = 255;
        }
        m_aubColor[3] = ubTopLeft;
        m_aubColor[7] = ubBottomLeft;
        m_aubColor[11] = ubBottomRight;
        m_aubColor[15] = ubTopRight;
    }
    
    virtual void SetTopLeftV(eC_Value vV)
    {
        CGUIBitmapPlane::SetTopLeftV(eC_FromInt(1) - vV);
    }
    virtual eC_Value GetTopLeftV()
    {
        return eC_FromInt(1) - CGUIBitmapPlane::GetTopLeftV();
    }

    
    virtual void SetBottomLeftV(eC_Value vV)
    {
        CGUIBitmapPlane::SetBottomLeftV(eC_FromInt(1) - vV);
    }
    virtual eC_Value GetBottomLeftV()
    {
        return eC_FromInt(1) - CGUIBitmapPlane::GetBottomLeftV();
    }

    
    virtual void SetBottomRightV(eC_Value vV)
    {
        CGUIBitmapPlane::SetBottomRightV(eC_FromInt(1) - vV);
    }
    virtual eC_Value GetBottomRightV()
    {
        return eC_FromInt(1) - CGUIBitmapPlane::GetBottomRightV();
    }
    

    virtual void SetTopRightV(eC_Value vV)
    {
        CGUIBitmapPlane::SetTopRightV(eC_FromInt(1) - vV);
    }
    virtual eC_Value GetTopRightV()
    {
        return eC_FromInt(1) - CGUIBitmapPlane::GetTopRightV();
    }
    
    /** Accessor for the OpenGL Texture ID.
        @return ID of the OpenGL texture
     */
    const eC_UInt GetTextureID() const;

    virtual eC_Value Get2DPlaneDistance();

    virtual void Clear(const eC_UByte ubRed, const eC_UByte ubGreen, const eC_UByte ubBlue, const eC_UByte ubAlpha);

protected:
    /** Protected constructor
        @param uiNewBitmapID Bitmap plane ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_GL(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth, eC_Value vHeight);
    virtual ~CGUIBitmapPlane_GL();

    /// Generates and binds a texture to this bitmap plane.
    void CreateTexture();

    /** Deletes the corresponding texture id */
    void DeleteTexture();

    /// Inits the Texture Object by actually allocating memory for it.
    virtual void CreateRenderSurface();

    /// Called by the framework to allow derived classes to free memory allocated by the associated render surface.
    virtual void DeleteRenderSurface() {}

    /// Calculates the vertices in the vertex array.
    inline void UpdateVertices()
    {
        m_avVertices[0] = -eC_Div(GetWidth(),  eC_FromInt(2));
        m_avVertices[1] =  eC_Div(GetHeight(), eC_FromInt(2));

        m_avVertices[2] = -eC_Div(GetWidth(),  eC_FromInt(2));
        m_avVertices[3] = -eC_Div(GetHeight(), eC_FromInt(2));

        m_avVertices[4] =  eC_Div(GetWidth(),  eC_FromInt(2));
        m_avVertices[5] = -eC_Div(GetHeight(), eC_FromInt(2));

        m_avVertices[6] =  eC_Div(GetWidth(),  eC_FromInt(2));
        m_avVertices[7] =  eC_Div(GetHeight(), eC_FromInt(2));
    }

    virtual eC_Bool DrawToBitmap();

    virtual eC_Bool DrawToScreen();

private:
    eC_UInt m_uiGLTextureID;          /// OpenGL TextureID;
    eC_TArray<eC_Value> m_avVertices; /// array of the 4 OpenGL Vertices
    eC_TArray<eC_UByte> m_aubColor;   /// array holding the color and alpha information for each vertex
    eC_Value m_vDepthCorrection;
};
#endif
