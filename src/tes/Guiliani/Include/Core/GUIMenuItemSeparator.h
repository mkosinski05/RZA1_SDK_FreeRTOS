/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENUITEMSEPARATOR_H_
#define CGUIMENUITEMSEPARATOR_H_

#include "GUIObject.h"

/// A separator item to be used in CGUIMenu.

/** It is derived from GUIObject and draws a line in the middle of the control.

    This control is not focusable.
    Use SetLineColor to set the line color.

    Example:
@code

    // Create a menu item separator.
    CGUIMenuItemSeparator* pkMenuItemSeparator = new CGUIMenuItemSeparator(this,
                                                        eC_FromInt(0), eC_FromInt(0),
                                                        eC_FromInt(100), eC_FromInt(1),
                                                        OBJ_MENUITEMSEPARATOR);

    // Set the line color.
    pkMenuItemSeparator->SetColors(0xffffff00);

@endcode

*/
class CGUIMenuItemSeparator
    :public CGUIObject
{
public:
    /** CGUIMenuItemSeparator constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object, i.e. the x-offset from the left border of the parent object
        @param vY Y-position relative to its parent object, i.e. the y-offset from the upper border of the parent object
        @param vWidth Width of the object
        @param vHeight Height of the object.
        @param eID Object identifier of this object (choose NO_HANDLE if none is required).
    */
    CGUIMenuItemSeparator(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuItemSeparator constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Boundary rectangle of this control, relative to parent.
        @param eID Object identifier of this object (choose NO_HANDLE if none is required).
    */
    CGUIMenuItemSeparator(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID);

    /** Standard constructor.
        @see CGUIObject().
    */
    CGUIMenuItemSeparator();

     /** CGUIMenuItemSeparator copy constructor.
         @param kSource Source object to be copied
    */
    CGUIMenuItemSeparator(const CGUIMenuItemSeparator& kSource);

    /** Assignment operator to copy the whole CGUIMenuItemSeparator.
        @param kSource Source object to be copied
        @return This object.
    */
    CGUIMenuItemSeparator& operator=(const CGUIMenuItemSeparator& kSource);

    /** CGUIMenuItemSeparator destructor.*/
    virtual ~CGUIMenuItemSeparator(){}

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID);
#endif

    /// Draw a line with different colors. Line width is 1 pixel.
    eC_Bool DoDraw();

    /** Set the line color for this control.
        @param uiLineColor the color in ARGB coding(Form: 0xAARRGGBB,e.g.0xFFFF0000 = Red(fully opaque)).
    */
    inline void SetColors(const eC_UInt &uiLineColor)
    {
        m_uiLineColor = uiLineColor;
        InvalidateArea();
    }

private:
    ///The line color.
    eC_UInt m_uiLineColor;

    /** Helper function for constructor.*/
    void Init();

    /** For copying the attributes in this class.*/
    void CopyAttributes(const CGUIMenuItemSeparator & kSource);
};
#endif
