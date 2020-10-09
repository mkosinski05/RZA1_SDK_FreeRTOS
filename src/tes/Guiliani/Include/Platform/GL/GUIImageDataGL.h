/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDATAGL__H_
#define GUIIMAGEDATAGL__H_

#include "GUIImageData.h"

/// OpenGL Version of CGUIImageData for textures.
class CGUIImageDataGL :
    public CGUIImageData
{
public:
    /** CGUIImageData constructor */
    CGUIImageDataGL();

    /** CGUIImageData destrcutor */
    ~CGUIImageDataGL();

    inline void SetID(const eC_UInt uiTextureID)
    {
        m_uiTextureID = uiTextureID;
    }///<@param uiTextureID The texture ID
    inline void SetTextureWidth(const eC_UInt uiTexWidth)
    {
        m_uiTextureWidth = uiTexWidth;
    }///<@param uiTexWidth The texture width
    inline void SetTextureHeight(const eC_UInt uiTexHeight)
    {
        m_uiTextureHeight = uiTexHeight;
    }///<@param uiTexHeight The texture height

    inline eC_UInt GetTextureWidth() const
    {
        return m_uiTextureWidth;
    }///<@return The texture width
    inline eC_UInt GetTextureHeight() const
    {
        return m_uiTextureHeight;
    }///<@return The texture height

    inline const eC_UInt GetTextureID() const
    {
        return m_uiTextureID;
    }///<@return The texture ID

private:
    /** Copy-constructor
    Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIImageDataGL(const CGUIImageDataGL& kSource);

    /** Operator= method.
    Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIImageData& operator=(const CGUIImageDataGL& kSource);

    eC_UInt m_uiTextureID; // OpenTexture identifier
    eC_UInt m_uiTextureWidth; // texture size for calculating texture coordinates
    eC_UInt m_uiTextureHeight;
};

#endif
