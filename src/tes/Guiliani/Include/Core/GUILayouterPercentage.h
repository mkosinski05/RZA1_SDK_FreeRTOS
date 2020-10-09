/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUILAYOUTERPERCENTAGE_H_
#define GUILAYOUTERPERCENTAGE_H_

#include "GUILayouter.h"
class CGUIObject;

/// This is a layouter, which resizes and repositions an object with regard to its parent

/** This layouter class is useful for designing dynamic layouts, where objects shall e.g., resize themselves when
    their containing window gets resized.

    A CGUILayouterPercentage will resize and reposition its associated object according to the size of its parent object.
    This allows for an easy way to let an object e.g., always span across the entire width of its parent, or to always
    cover the parent's lower right quadrant etc.

    Example:
@code
    CGUILayouterPercentage* pLayouter = new CGUILayouterPercentage();

    // This lets the object cover the entire width of the parent. Position and height remain unchanged.
    pLayouter->SetLayoutParameters( eC_FromFloat(0), eC_FromFloat(0), eC_FromFloat(1), eC_FromFloat(0));

    // This lets the object cover the lower right quadrant
    pLayouter->SetLayoutParameters( eC_FromFloat(0.5), eC_FromFloat(0.5), eC_FromFloat(0.5), eC_FromFloat(0.5));

    // This lets the object cover the lower 10 percent of the screen
    pLayouter->SetLayoutParameters( eC_FromFloat(0), eC_FromFloat(0.9), eC_FromFloat(1), eC_FromFloat(0.1));
@endcode
@ingroup GUILIANI_LAYOUTER
*/
class CGUILayouterPercentage
    : public CGUILayouter
{
public:

    /** Constructor.
        @param pObject Pointer to the object to which the layouter is attached.
        @param vXPosPercentage The associated object's relative x position is calculated as vXPosPercentage * Parent-Width
        @param vYPosPercentage The associated object's relative y position is calculated as vYPosPercentage * Parent-Height
        @param vWidthPercentage The associated object's width is calculated as vWidthPercentage * Parent-Width
        @param vHeightPercentage The associated object's height is calculated as vHeightPercentage * Parent-Height
    */
    CGUILayouterPercentage( CGUIObject * const pObject = NULL, 
        const eC_Value vXPosPercentage = eC_FromInt(-1), const eC_Value vYPosPercentage = eC_FromInt(-1), 
        const eC_Value vWidthPercentage = eC_FromInt(-1), const eC_Value vHeightPercentage = eC_FromInt(-1) );

    /// Destructs an Anchor-Layouter.
    virtual ~CGUILayouterPercentage() {}

    virtual eC_Bool IsDependentOnParentSize() {return true;}

    /** Sets the layout parameters by which the object is scaled and positioned.
        Legal values are in the range of 0 to 1. 
        The values will be used as factors by which the respective attributes get multiplied.
        A value of <=0 means that the corresponding attribute will not be affected by layouting.

        @param vXPosPercentage The associated object's relative x position is calculated as vXPosPercentage * Parent-Width
        @param vYPosPercentage The associated object's relative y position is calculated as vYPosPercentage * Parent-Height
        @param vWidthPercentage The associated object's width is calculated as vWidthPercentage * Parent-Width
        @param vHeightPercentage The associated object's height is calculated as vHeightPercentage * Parent-Height
    */
    void SetLayoutParameters(
        const eC_Value &vXPosPercentage, const eC_Value &vYPosPercentage,
        const eC_Value &vWidthPercentage, const eC_Value &vHeightPercentage);

    /// Recalculate layout
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_PERCENTAGE_CLASS_VERSION; ///< Class version for streaming
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

private:

    // Scaling factors in width, height, vertical and horizontal direction
    eC_Value m_vXPosPercentage;
    eC_Value m_vYPosPercentage;
    eC_Value m_vWidthPercentage;
    eC_Value m_vHeightPercentage;
};

#endif
