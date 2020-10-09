/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_LAYOUTER__H_
#define GUI_LAYOUTER__H_

#include "GUIStreamableObject.h"
#include "GUIRect.h"

class CGUIObject;
/** @defgroup GUILIANI_LAYOUTER Layouter
@brief Group of classes within Guiliani which are related handling or layout

Layouting helps to order controls or container controls in a visual appealing way.

The following type of lay outers are available:
- Anchor
- Percentage
- List
- Grid
- Group
- Reposition

Examples for layouters (including example code) can be found here:

CGUILayouterAnchor

CGUILayouterPercentage

CGUIRepositionCompositeObject
*/


/// This is the Guiliani base class for layouters.
/// @ingroup GUILIANI_LAYOUTER
// @guiliani_doxygen toplevel_module Layouter
/**
    <table border="0">
    <tr>
        <td width="200">@image html layouter.png</td>
            <td>
                The "Layouter" offers several methods to set the layout behavior of a control or container. One way is to set a layout rule to arrange all object in a specified
                order, e.g., bottom to top. Another way is to anchor an object to one or more other objects to set its position or/and size relative to the other objects (Class: CGUILayouter).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen

class CGUILayouter
    : public CGUIStreamableObject
{
public:
    /// Enumerate possible reasons for calling layout, @see DoLayout @see InitLayouter
    enum eMovedEdges_t
    {
        CHANGED_WIDTH, ///< distance to the parent's right border changed, that is, SetWidth() was called on associated object
        CHANGED_HEIGHT, ///< distance to the parent's bottom border changed, that is, SetHeight() was called on associated object
        CHANGED_RELXPOS, ///< distance to the parent's left and right borders changed, that is, SetRelXPos() was called on associated object
        CHANGED_RELYPOS, ///< distance to the parent's top and bottom borders changed, that is, SetRelYPos() was called on associated object
        CHANGED_EDGE_ALL ///< distance to all edges changed
    };

    /// Destructor.
    /** De-registers the layouter from its associated object.
      */
    virtual ~CGUILayouter();

    /** This function shall determine if the layouter shall initially be called due to resizing
        the parent of the associated object or if it shall initially be called by
        resizing the associated object.
        If it returns false the framework will call DoLayout() on this layouter of
        the object that is triggering the layout procedure, that is, the composite object on
        which the first (in the current call stack) SetWidth() or SetHeight() is called.

        Returning false is the typical use case for a group layouter that does not
        consider the layout of the parent of the associated object.
        In this case the initially resized object starting the layout procedure is the 'group
        object' itself, therefore you can say changing the 'group object' starts the layout
        procedure in this case.

        If it returns true, the first DoLayout() is called on a child of the initially resized
        object. This is the typical use case for a child layouter or a group layouter
        that is considering the layout of 'its' parent. Therefore, you can say changing the
        parent is starting the layout procedure in this case.

        @return eC_Bool True if dependend, False otherwise
    */
    virtual eC_Bool IsDependentOnParentSize() = 0;

    /** Called when the size of the parent of the associated object has changed.

        This method must be re-implemented in derived classes to perform the
        actual layout change, typically adjustments to the associated object's
        dimension or position.

        It is called by the framework every time the width or
        height of the parent of the associated object has changed. The framework
        first calls DoLayout() on the associated object itself and then on all
        children of the associated object in any depth in the hierarchy.

        There are two valid types of layouters:
        Either:
        Implementations of DoLayout() shall only adjust the associated object
        and/or direct children of it. No other decendants of the associated
        object shall be changed in position or size!
        This is called a group layouter.
        CGUILayouterReposition and CGUILayouterGrid are examples of group layouters.
        Or:
        Implementations of DoLayout() shall only adjust the associated object
        depending on the layout of its parent object. No other objects shall
        be changed in position or size!
        This is called a child layouter.
        CGUILayouterAnchor is an example of a child layouter.

        Implementing other types of layouters may compromise the framework architecture.

        The two use cases may overlap:
        If a direct child of a group layouter composite object has a child layouter
        then while streaming in the composite object associated with the group layouter,
        the composite will first call DoLayout() on its layouter and then call
        InitLayouter() on the layouters of all direct children of that composite object.
        Therefore, the group layouter will re-initialize the child layouter. For an
        anchor layouter this implies that the child layouter will have no effect and
        should be removed. Still the child layouter's DoLayout() will be called
        after the parent layouter's DoLayout() later on for any change of the size of the
        parent so that the child layouter might change (override) the layouting of the group
        layouter concerning its child layout, for example if using hardcoded values
        for anchor distances.
        @param eMovedEdges Reason for layouter call
    */
    virtual void DoLayout(eMovedEdges_t eMovedEdges) = 0;

    /** This function is useful for child layouters.
        Called when the layout of the associated object itself has changed.
        It is called by the framework every time the width or
        height or x or y relative position of the associated object is changed.
        That is, for example when the new distance to parent's border(s) shall be set
        in a child layouter to base upcoming layouting on this data the
        framework calls this method.

        InitLayouter() is only called on the layouter of the associated object
        for one change of the position or size of the associated object.
        DoLayout() in contrast is called on the layouter of the associated object
        and on the layouters of all decendants of the associated object
        for one change of the size of the associated object.

        This function usually does not implement changes to any layout.
        It is typically used to remember layout data to be used
        in DoLayout() later on (useful for child layouters that depend
        on changes of their associated object's parent).
        @param eMovedEdges Reason for layouter call
    */
    virtual void InitLayouter(eMovedEdges_t eMovedEdges) {}

    /** Get the object that contains this layouter.
        @return This layouter's associated object.
    */
    inline CGUIObject* GetAssociatedObject() const {return m_pObject;}

    /** Sets the associated object, that means the object on which the layouter will be working.
        Note that there is always a one-to-one relation between a layouter and a GUIObject. Assigning a layouter
        to an object <i>A</i> first, and then assigning it to an object <i>B</i> will actually leave object <i>A</i> without a layouter.
        @see CGUIObject::SetLayouter()
        @param pObject The object to be associated  
    */
    virtual void SetAssociatedObject(CGUIObject * const pObject );


#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a layouter class ID into a stream.
    static const eC_Char XMLTAG_LAYOUTERCLASSID[];
#endif

protected:
    /// Protected constructor. Called by derived classes.
    CGUILayouter( CGUIObject * const pObject = NULL);

private:

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUILayouter(const CGUILayouter& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUILayouter& operator=(const CGUILayouter& kSource);

    /// Pointer to associated object
    CGUIObject *m_pObject;
};

#endif
