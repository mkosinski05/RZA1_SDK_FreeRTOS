/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUILAYOUTERANCHOR__H_
#define GUILAYOUTERANCHOR__H_

#include "GUILayouter.h"
class CGUIObject;

/// This is a layouter, which implements the "anchor"-concept.

/** Anchor-Layoutes allow the user to specify the way in which a GUI layout adapts to changes of the screen-, window- or
    Object-sizes. Hence, you can define whether an object gets scaled along with its containing object, or whether it moves
    along with it. Visually speaking, an anchor "glues" a side of an object to the respective side of its parent object, and
    whenever that parent object changes its dimensions, the child object's layouter will scale/move the child accordingly.<p>

    The following table gives you some examples on which behaviour will result from which anchor-settings:

    <table border="1" cellspacing="2">
        <tr>
            <th>top</th>
            <th>bottom</th>
            <th>left</th>
            <th>right</th>
            <th>result</th>
        </tr>
        <tr>
            <td>ON</td>
            <td>ON</td>
            <td>ON</td>
            <td>ON</td>
            <td>Object gets stretched in X/Y directions</td>
        </tr>
        <tr>
            <td>off</td>
            <td>off</td>
            <td>off</td>
            <td>off</td>
            <td>Object will not get stretched, but will scale its position along with the change in size of its parent.
                i.e. If the width of the parent object gets doubled, the relative X position of the child object will double as well.</td>
        </tr>
        <tr>
            <td>ON</td>
            <td>ON</td>
            <td>ON</td>
            <td>off</td>
            <td>Object gets stretched in Y direction and is locked to the left border of its parent</td>
        </tr>
        <tr>
            <td>ON</td>
            <td>ON</td>
            <td>off</td>
            <td>ON</td>
            <td>Object gets stretched in Y direction and is locked to the right border of its parent</td>
        </tr>
        <tr>
            <td>ON</td>
            <td>ON</td>
            <td>off</td>
            <td>off</td>
            <td>Object gets stretched in Y direction and will scale its X position along with the change in size of its parent/td>
        </tr>
    </table>

    <p>
    The typical usage of a layouter is as follows:
    -# You instantiate the layouter...
    -# set its anchors as required...
    -# and attach it to an object.
    The object will automatically gain ownership of the layouter and delete it upon its own destruction.
    (Note that there is always a <b>One on One</b> relationship between objects and layouters.)
@code
    CGUILayouterAnchor* pLayouter = new CGUILayouterAnchor();
    pLayouter->SetAnchors( false, false, true, true);
    pObject->SetLayouter( pLayouter);
@endcode

@ingroup GUILIANI_LAYOUTER
*/
class CGUILayouterAnchor
    : public CGUILayouter
{
public:

    /// Enumeration for anchors to all four sides of an object
    enum Anchors_t {
        ANCHOR_TOP    = 0x0001,
        ANCHOR_BOTTOM = 0x0002,
        ANCHOR_LEFT   = 0x0004,
        ANCHOR_RIGHT  = 0x0008
    };

    /** Constructs an Anchor-Layouter.
        @param pObject Pointer to the object to which the layouter is attached.
    */
    CGUILayouterAnchor( CGUIObject * const pObject = NULL );

    /// Destructs an Anchor-Layouter.
    ~CGUILayouterAnchor() {}

    virtual eC_Bool IsDependentOnParentSize() {return true;}

    /** Set the status of all four anchors.
        TRUE means that an anchor is activated, FALSE marks the respective anchor as deactivated.
        @param bTop    Specifies whether the top anchor is activated
        @param bBottom Specifies whether the bottom anchor is activated
        @param bLeft   Specifies whether the left anchor is activated
        @param bRight  Specifies whether the right anchor is activated
    */
    void SetAnchors(
        const eC_Bool &bTop, const eC_Bool &bBottom, const eC_Bool &bLeft, const eC_Bool &bRight);

    /** Set the status of all four anchors.
        Combine the respective anchors by a logical OR to set several anchors.
        @param ubAnchors States of all four anchors, by combining the values in Anchors_t using logical ORs
    */
    void SetAnchors( const eC_UByte &ubAnchors);

    /** Recalculate the distances of the associated object's
        right border to the parent's right border and of the
        associated object's bottom border to the parent's bottom
        border and set these to be the new anchor lengths.
        @note This is useful for example when the height or width
        of the associated object changes so that when resizing the
        parent later on, the current anchor length are used and not
        those that were saved when the anchors were chosen.
        @param eMovedEdges Reason for layouter call
    */
    virtual void InitLayouter(eMovedEdges_t eMovedEdges);

    /** Returns whether the specified anchor is set or not
        @param eAnchor Enumeration identifying the anchor
        @return True if the anchor is activated, False otherwise. */
    inline eC_Bool IsAnchorSet( const Anchors_t eAnchor) const
    {
        return (m_ubAnchors & eAnchor)!=0;
    };

    /// Recalculate layout
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_ANCHOR_CLASS_VERSION;///< Class version of anchor layouter class
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

private:

    /// Initializes the Anchor
    inline void Init();

    // Helper attributes required for layouting
    eC_Value m_vDistanceBottom;
    eC_Value m_vDistanceRight;

    // Scaling factors in vertical and horizontal direction
    eC_Value m_vXFactor;
    eC_Value m_vYFactor;

    // Member to store the state of all four anchors
    eC_UByte m_ubAnchors;
};

#endif
