/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIGEOMETRYOBJECT_H_
#define GUIGEOMETRYOBJECT_H_

#include "GUIObject.h"

/// Simple control for displaying geometrical primitives.

// @guiliani_doxygen toplevel_control Geometry Object Control
/**
<table border="0">
<tr>
<td width="200">@image html geometry_object.png</td>
<td>
The "geometry control" control offers a simple way to add geometrical primitives, such as lines or rectangles, to your user-interface
    without writing customized drawing code (Class: CGUIGeometryObject).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen

/** This class offers a simple way to add geometrical primitives, such as lines or rectangles, to your user-interface
    without writing customized drawing code.
*/
class CGUIGeometryObject : public CGUIObject
{
public:
    /// Enumeration for object shapes
    enum PrimitiveShape_t
    {
        PS_LINE1,  ///< Diagonal line, from top/left to bottom/right corner of the object
        PS_LINE2,  ///< Diagonal line, from top/right to bottom/left corner of the object
        PS_VERTICAL_LINE, ///< Vertical line, centered within the object
        PS_HORIZONTAL_LINE, ///< Horizontal line, centered within the object
        PS_ELLIPSE, ///< Ellipse, centered within the objecz
        PS_RECTANGLE ///< Rectangle, Covering the entire object
    };

    /** Constructor
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the geometry object
        @param vHeight Height of the geometry object
        @param uiColor Color of drawing
        @param eShape Shape descriptor. Valid values are defined in CGUIGeometryObject::PrimitiveShape_t
        @param vThickness Thickness of pen
        @param bFilled Fill-out object (Currently only supported for rectangles)
        @param eID Object identifier of this object (choose NO_HANDLE if none is required)
    */
    CGUIGeometryObject(
        CGUICompositeObject* const pParent,
        const eC_Value& vX,
        const eC_Value& vY,
        const eC_Value& vWidth,
        const eC_Value& vHeight,
        const eC_UInt&  uiColor,
        const PrimitiveShape_t eShape,
        const eC_Value& vThickness = eC_FromInt(1),
        const eC_Bool&  bFilled = false,
        const ObjectHandle_t& eID = NO_HANDLE);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_Char XMLTAG_GEOMETRYOBJECT_COLOR[]; ///< XML tag for color
    static const eC_Char XMLTAG_GEOMETRYOBJECT[]; ///< XML tag for color
#endif

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

    /** Standard constructor. */
    CGUIGeometryObject();

    /** Destructor */
    virtual ~CGUIGeometryObject();

    eC_Bool DoDraw();

    /** Returns the currently set color
        @return currently set color */
    eC_UInt GetColor() const;

    /** Sets the color of the primitive as a 32 Bit hexadecimal value (0xAARRGGBB).
        @param uiColor Color of drawing */
    void SetColor(const eC_UInt& uiColor);

    /** Sets the shape of the primitive as an enum.
        @param eShape Shape descriptor. Valid values are defined in CGUIGeometryObject::PrimitiveShape_t */
    void SetShape(const PrimitiveShape_t eShape);

    /** Sets the drawing thickness of the primitive.
        @param vThickness Thickness (in pixels) of pen */
    void SetThickness(const eC_Value& vThickness);

    /** Determines whether the object should be filled.
        @param bFilled Fill-out object. (Currently only supported for rectangles). */
    void SetFilled(const eC_Bool& bFilled);

private:
    void Init();

    PrimitiveShape_t m_eShape;
    eC_Value         m_vThickness;
    eC_Bool          m_bFilled;
    eC_UInt          m_uiColor;
};

#endif // GEOMETRYOBJECT_H_
