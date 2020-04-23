/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUILAYOUTER_ALIGN_TO_PARENT__H_
#define GUILAYOUTER_ALIGN_TO_PARENT__H_

#include "GUILayouter.h"
class CGUIObject;

/// This layouter aligns an object to its parent.

/**
@note The Layouter is only triggered to refresh the layout when the size or the position
of the associated object is changed. Changing the size of the parent will not trigger a
refresh of the layout.

The typical usage of a layouter is as follows:
-# Instantiate the layouter.
-# Set the alignment as required.
-# Finally, attach it to an object.
The object will automatically gain ownership of the layouter and delete it upon its own destruction.
(Note that there is always a <b>One on One</b> relationship between objects and layouters.)
@code
CGUILayouterAlignToParent* pLayouter = new CGUILayouterAlignToParent();
pLayouter->SetAlignment( CGUILayouterAlignToParent::ALIGN_H_CENTER, CGUILayouterAlignToParent::ALIGN_V_CENTER);
pObject->SetLayouter( pLayouter);
@endcode

@ingroup GUILIANI_LAYOUTER
*/
class CGUILayouterAlignToParent
    : public CGUILayouter
{
public:

    /// Enumeration for the horizontal alignment
    enum HorizontalAlignment_t {
        ALIGN_H_LEFT   = 1,
        ALIGN_H_CENTER = 2,
        ALIGN_H_RIGHT  = 3,
        ALIGN_H_NONE   = 4
    };

    /// Enumeration for the vertical alignment
    enum VerticalAlignment_t {
        ALIGN_V_TOP    = 1,
        ALIGN_V_CENTER = 2,
        ALIGN_V_BOTTOM = 3,
        ALIGN_V_NONE   = 4
    };

    /** Constructs an AlignToParent-Layouter.
    @param pObject Pointer to the object to which the layouter is attached.
    */
    CGUILayouterAlignToParent( CGUIObject * const pObject = NULL );

    /// Destructs an AlignToParent-Layouter.
    ~CGUILayouterAlignToParent() {}

    virtual eC_Bool IsDependentOnParentSize() {return false;}

    /** Set the horizontal alignment and the vertical alignment.
        Note: This will not directly refresh the layout. Call DoLayout to refresh the layout.
    @param eHorizontalAlignment The horizontal alignment.
    @param eVerticalAlignment The vertical alignment.
    */
    void SetAlignment(
        const HorizontalAlignment_t &eHorizontalAlignment, const VerticalAlignment_t &eVerticalAlignment);

    /// This function is called when relative positions of the associated object are changed.
    void InitLayouter(eMovedEdges_t eMovedEdges){DoLayout(eMovedEdges);}

    /// Recalculate layout
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_ALIGN_TO_PARENT_CLASS_VERSION;///< Class version of align-to-parent layouter class
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

private:

    /// Initializes the Layouter
    inline void Init();
    /// The horizontal alignment.
    HorizontalAlignment_t m_eHorizontalAlignment;
    /// The vertical alidnment.
    VerticalAlignment_t m_eVerticalAlignment;
};
#endif
