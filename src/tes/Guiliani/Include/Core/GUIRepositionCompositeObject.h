/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUI_REPOSITION_COMPOSITE_OBJECT__H__
#define __GUI_REPOSITION_COMPOSITE_OBJECT__H__

#include "GUICompositeObject.h"
#include "GUILayoutHelper.h"

/// Can reposition its visible children in different directions.

// @guiliani_doxygen toplevel_control Reposition Composite Object
/**
    <table border="0">
        <tr>
            <td width="200">@image html reposition_composite_object.png</td>
            <td>
                The "reposition composite object" is a special composite object. It arranges all its child according to a predefined rule, e.g. from top to bottom (Class: CGUIRepositionCompositeObject).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    By default, it does not reposition the children.

    The children will be repositioned if RepositionChildren() is called and
    the base point is not set to CGUILayoutHelper::REPOSITION_ALIGN_INVALID.
    The children are repositioned based on a base point. E.g., if it is set to
    CGUILayoutHelper::REPOSITION_ALIGN_BOTTOM, the first child is positioned at
    the bottom, with the following children stacked upwards. Keep this in mind
    when adding children to bottom- or right-aligned reposition containers.

    This control uses the method CGUILayoutHelper::RepositionChildren().

    Example:

@code
    // Create a reposition composite object.
    CGUIRepositionCompositeObject* pkRepositionCompositeObject = new CGUIRepositionCompositeObject(this,
                                                                            eC_FromInt(0), eC_FromInt(0),
                                                                            eC_FromInt(100), eC_FromInt(300),
                                                                            CGUILayoutHelper::REPOSITION_ALIGN_TOP,
                                                                            eC_FromInt(0),
                                                                            NO_HANDLE);

    // Sets BasePoint on which repositioning is based
    pkRepositionCompositeObject->SetBasePoint(CGUILayoutHelper::REPOSITION_ALIGN_BOTTOM);

    // Sets Gap between child objects
    pkRepositionCompositeObject->SetGap(eC_FromInt(5));

    // Generate some dummy content
    for( int i=0; i<10; i++)
        new CGUIButton(pkRepositionCompositeObject, 0,0, eC_FromInt(100), eC_FromInt(20), "Button", NULL);

    // Repositions the children based on the current base point.
    pkRepositionCompositeObject->RepositionChildren();
@endcode

    @ingroup GUILIANI_CONTROLS
    @ingroup GUILIANI_LAYOUTER
    */
class CGUIRepositionCompositeObject
    : public CGUICompositeObject
{
public:
    /** CGUIRepositionCompositeObject constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the composite object.
        @param vHeight Height of the composite object.
        @param eBasePoint The base point on which the repositioning is based.
        @param vGap The gap between the children.
        @param eID Object Identifier of this composite object.
    */
    CGUIRepositionCompositeObject(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const CGUILayoutHelper::RepositionAlignment_t &eBasePoint = CGUILayoutHelper::REPOSITION_ALIGN_INVALID,
        const eC_Value &vGap = eC_FromInt(0),
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIRepositionCompositeObject constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Size and position relative to parent.
        @param eBasePoint The base point on which the repositioning is based.
        @param vGap The gap between the children.
        @param eID Object Identifier of this composite object.
    */
    CGUIRepositionCompositeObject(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const CGUILayoutHelper::RepositionAlignment_t &eBasePoint = CGUILayoutHelper::REPOSITION_ALIGN_INVALID,
        const eC_Value &vGap = eC_FromInt(0),
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIRepositionCompositeObject destructor. */
    virtual ~CGUIRepositionCompositeObject() {}

    /** Repositions the children based on a base point.
        Calls CGUILayoutHelper::RepositionChildren().
      */
    void RepositionChildren();

    /// Sets point on which repositioning is based.
    void SetBasePoint( CGUILayoutHelper::RepositionAlignment_t eNewBasePoint) { m_eBasePoint = eNewBasePoint; }

    /// Set border spacing (gap between parent edges and first/last child, respectively).
    void SetBorder( const eC_Value vBorder) { m_vBorder = vBorder; }

    /** Set gap between child objects.
        @param vGap Gap between child objects in pixels.
      */
    void SetGap( const eC_Value vGap) { m_vGap = vGap; }

    /// @return Gap between child objects.
    eC_Value GetGap() const { return m_vGap; }

    /**    Standard constructor.
        @see CGUIObject().
    */
    CGUIRepositionCompositeObject();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    ///Base point mode
    CGUILayoutHelper::RepositionAlignment_t m_eBasePoint;
    ///Gap between two children.
    eC_Value m_vGap;
    ///Space between top/left border and first child object, and right/bottom border and last child object
    eC_Value m_vBorder;
};
#endif
