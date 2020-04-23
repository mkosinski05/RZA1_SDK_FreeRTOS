/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(EXAMPLE_CONTROL_H)
#define EXAMPLE_CONTROL_H

#include "GUIObject.h"

/** An example CGUIObject implementation that draws a rectangle with
    configureable border width and configurable colors.
  */
class ExampleControl : public CGUIObject
{
public:
    /** Constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object, i.e. the x-offset from the left border of the parent object
        @param vY Y-position relative to its parent object, i.e. the y-offset from the upper border of the parent object
        @param vWidth Width of the object
        @param vHeight Height of the object
        @param uiInnerColor Color to be used for drawing this control's center
               rectangle (everything except the border) in ARGB format.
        @param uiBorderColor Color to be used for drawing this control's frame
               in ARGB format.
        @param vBorderWidth The width of the border in pixels.
        @param eID Object identifier of this object (choose NO_HANDLE if none is required).
    */
    ExampleControl(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        eC_UInt uiInnerColor, eC_UInt uiBorderColor,
        eC_Value vBorderWidth = eC_FromInt(1),
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Default constructor to be used by the factory.
    ExampleControl();

    /// Returns the center color.
    inline eC_UInt GetCenterColor() const { return m_uiInnerColor; }

    /// Returns the border color.
    inline eC_UInt GetBorderColor() const { return m_uiBorderColor; }

    /// Returns the border width.
    inline eC_Value GetBorderWidth() const { return m_vBorderWidth; }

    /** Sets a new center color and invalidates this control's area to have it
        repainted by the framework.
        @param uiInnerColor The new color to be used for the center.
      */
    void SetCenterColor(eC_UInt uiInnerColor);

    /** Sets a new center color and invalidates this control's area to have it
        repainted by the framework.
        @param uiBorderColor The new color to be used for the center.
      */
    void SetBorderColor(eC_UInt uiBorderColor);

    /** Sets a new border width and invalidates this control's area to have it
        repainted by the framework.
        @param vBorderWidth The new border width.
      */
    void SetBorderWidth(eC_Value vBorderWidth);

#if defined(GUILIANI_STREAM_GUI)
    /** Reads all object attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all object attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of ControlID,
               leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    /// Called by the Guiliani framework when this control should paint itself.
    eC_Bool DoDraw();

private:
    /// The color used for drawing the center of this control.
    eC_UInt m_uiInnerColor;

    /// The color used for drawing the border of this control.
    eC_UInt m_uiBorderColor;

    /// The width of the border in (sub-) pixels.
    eC_Value m_vBorderWidth;
};

#endif // EXAMPLE_CONTROL_H
