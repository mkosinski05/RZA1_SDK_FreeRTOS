/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIBITMAPEGML__H_
#define GUIBITMAPEGML__H_

#include "GUIBitmapPlane.h"

#include "eGML_Vector3.h"
class eGML_Texture;
class eGML_Primitive;
class CGfxWrapeGML;
class eGML_Bitmap;

/** @brief The BitmapPlane Class for eGML
<h3>Platform Specific Limitations </h3>
*/

// @guiliani_doxygen limitations CGfxWrapeGML
/**
 Transparencies are not supported for GUIBitmapPlane_eGML. (Class CGUIBitmapPlane_eGML)
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CGfxWrapeGML "CGUIBitmapPlane_eGML Platform Specific Limitations"
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_eGML : public CGUIBitmapPlane
{
    friend class CGfxWrapeGML;
public:

    void DoDraw();

    virtual void SetWidth(eC_Value vWidth, eC_Bool bExpandCentered = false)
    {
        CGUIBitmapPlane::SetWidth(vWidth, bExpandCentered);
        UpdateVertices();
        UpdateUVValues();
    }

    virtual void SetHeight(eC_Value vHeight, eC_Bool bExpandCentered = false)
    {
        CGUIBitmapPlane::SetHeight(vHeight, bExpandCentered);
        UpdateVertices();
        UpdateUVValues();
    }


    /// Please note: In eGML only ONE alpha value for the whole plane is supported. This implementation takes the value in "ubTopLeft"
    virtual void SetAlphaValues(const eC_UByte ubTopLeft, const eC_UByte ubBottomLeft,
                                    const eC_UByte ubBottomRight, const eC_UByte ubTopRight)
    {
        m_ubAlpha = ubTopLeft;
    }


    virtual void SetTopLeftU(eC_Value vU)
    {
        CGUIBitmapPlane::SetTopLeftU(vU);
        UpdateUVValues();
    }

    virtual void SetTopLeftV(eC_Value vV)
    {
        CGUIBitmapPlane::SetTopLeftV(vV);
        UpdateUVValues();
    }

    virtual void SetBottomLeftU(eC_Value vU)
    {
        CGUIBitmapPlane::SetBottomLeftU(vU);
        UpdateUVValues();
    }


    virtual void SetBottomLeftV(eC_Value vV)
    {
        CGUIBitmapPlane::SetBottomLeftV(vV);
        UpdateUVValues();
    }


    virtual void SetBottomRightU(eC_Value vU)
    {
        CGUIBitmapPlane::SetBottomRightU(vU);
        UpdateUVValues();
    }


    virtual void SetBottomRightV(eC_Value vV)
    {
        CGUIBitmapPlane::SetBottomRightV(vV);
        UpdateUVValues();
    }


    virtual void SetTopRightU(eC_Value vU)
    {
        CGUIBitmapPlane::SetTopRightU(vU);
        UpdateUVValues();
    }


    virtual void SetTopRightV(eC_Value vV)
    {
        CGUIBitmapPlane::SetTopRightV(vV);
        UpdateUVValues();
    }


    virtual void Clear(const eC_UByte ubRed, const eC_UByte ubGreen, const eC_UByte ubBlue, const eC_UByte ubAlpha);

    /** Retrieve the internal bitmap of the BitmapPlane.
        This bitmap can be deleted by the framework at any time and must not be stored by user code.
        @return Pointer to internal eGML Bitmap of this bitmap plane.
    */
    const eGML_Bitmap* GetInternalBitmap();

protected:
    /** Constructor
        @param uiNewBitmapID Bitmap ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_eGML(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth, eC_Value vHeight);
    virtual ~CGUIBitmapPlane_eGML();

    ///Creates the eGMLTexture
    void CreateTexture();

    /// Calculates the vertices in the vertex array.
    void UpdateVertices();
    /// Updates the Texture Start and the eGML U and V vector
    void UpdateUVValues();

    /// Sets the Texture for this BitmapPlane
    void SetTexture(eGML_Texture* pNewTexture);

    virtual eC_Bool DrawToBitmap();
    virtual eC_Bool FinishDrawToBitmap();

private:
    eGML_Texture*   m_pkTexture; ///< eGML Texture object
    eGML_Primitive* m_pkPrimitive; ///< eGML primitive with 4 vertices
    eGML_Vector3    m_v3TexStart; ///< pointer to 3D Vector pointing to the start point of the texture
    eGML_Vector3    m_v3U; ///< U Coordinate
    eGML_Vector3    m_v3V; ///< V Coordinate
    eC_UByte        m_ubAlpha; /// Alpha value 0-255
};

#endif
