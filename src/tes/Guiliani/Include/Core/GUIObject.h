/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIOBJECT__H_
#define GUIOBJECT__H_

#include "GUIBehaviour.h"
#include "GUIObjectHandleResource.h"
#include "GUIRect.h"
#include "GUIBitmapPlane.h"
#include "GUILayouter.h"
#include "GUIAnimatable.h"
#include "GUIDataPool.h"

class CGUI;
class CGUIBehaviourDecorator;
class CGUILayouter;
class CGUICompositeObject;

/** @defgroup GUILIANI_CONTROLS Controls
    @brief Controls (widgets) within Guiliani

Controls (also called widgets in other GUI frameworks) are the basic building
blocks from which graphical user interfaces are built. Guiliani's standard
controls range from simple, static controls (like CGUIImage) to complex ones
that deal with user input.

All controls are derived from CGUIObject. Controls are arranged by parent-child relationship. 
To achieve this relationship the CGUICompositeObject acts as a container for other controls.

Each CGUICompositeObject can have zero or more children. 
A GUI is usually built in the following way with Guiliani:
- Derive a class from CGUI (which also is a composite object).
- In the derived class' Init() method, additional controls can be added. These
can also be composite objects with their own children.

Each control has a position that is relative to its parent.
*/

/// This is the Guiliani base class all controls are derived from.

/**
    Additional behavior of specific controls (e.g. the observer feature) can be added by means of multiple
    inheritance.
    The position setter methods like SetRelXPos(), and the status setter methods such as SetGrayedOut() are virtual
    so that there is a possibility for user to overwrite these methods and build special functionalities.
    An example is that if the user the control changes width of a control and he wants to inform other controls
    about this change, or if the user wants to prevent the change of the width in the control.

    Example:
@code

    // Create an object.
    CGUIObject* pkObject = new CGUIObject(this,
                                    eC_FromInt(0), eC_FromInt(0),
                                    eC_FromInt(100), eC_FromInt(100),
                                    OBJ_OBJECT1);

    // Sets this object's current relative X-position.
    pkObject->SetRelXPos(eC_FromInt(10));

    // Sets this object's current width.
    pkObject->SetWidth(eC_FromInt(1000));

    // Set the object's status to invisible.
    pkObject->SetInvisible(true);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIObject
    : public CGUIBehaviour, public CGUIAnimatable
{
    /** CompositeObject's HandleDraw() should be able to update the absolute coordinates
        before DoDraw() is called. No derived class should manipulate absolute coordinates
        as they are recalculated automatically by the framework.

        Also CGUIObject::FourWayNext() will be called by CompositeObject's FourWayNext() and CGUI's FourWayFocusNext()
    */
    friend class CGUICompositeObject;
    friend class CGUI;

public:
    /** Used by the four-way focusing functions to specify the direction.
        @see CGUI::FourWayFocusNext
    */
    enum FourWayFocus_t
    {
        FOCUS_LEFT,
        FOCUS_RIGHT,
        FOCUS_UP,
        FOCUS_DOWN
    };

    /** CGUIObject constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object, i.e. the x-offset from the left border of the parent object
        @param vY Y-position relative to its parent object, i.e. the y-offset from the upper border of the parent object
        @param vWidth Width of the object
        @param vHeight Height of the object
        @param eID Object identifier of this object (choose NO_HANDLE if none is required).
    */
    CGUIObject(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIObject constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle of the object, relative to parent.
        @param eID Object identifier of this object (choose NO_HANDLE if none is required).
    */
    CGUIObject(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIObject constructor. The only non-derived class that should call
        this constructor is GUIStandardFactory.
        CGUIObject has to read attributes from streaming file by using
        ReadFromStream() afterwards.
        Attention: Please DO NOT use this to create a concrete object
                   outside the GUIStandardFactory.
    */
    CGUIObject();

    /** CGUIObject copy-constructor.
        Behaviour and Layout are set to NULL, because they cannot be copied.
        @param kSource Source object to be copied
    */
    CGUIObject(const CGUIObject& kSource);

    /** CGUIObject operator= to copy the whole CGUIObject.
        Behaviour and Layout are set to NULL, because they cannot be copied.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIObject& operator=(const CGUIObject& kSource);

    /** CGUIObject destructor.
        Any behaviour object, which belongs to this CUGIObject will be DELETED during destruction.
        This destructor is virtual as we want to delete derived objects by using a base class pointer.
        Destructors of derived classes will automatically be declared virtual, there is no need to
        declare such destructor "virtual" explicitly.
    */
    virtual ~CGUIObject();

    /** Sets the bounding rectangle of the object in coordinates that are
        relative to the parent object.
        @param kNewRect Rectangle describing the new size and position of
               the object in relative coordinates
    */
    inline void SetRelRect( const CGUIRect& kNewRect)
    {
        SetRelXPos( kNewRect.GetX1() );
        SetRelYPos( kNewRect.GetY1() );
        SetWidth( kNewRect.GetWidth() );
        SetHeight( kNewRect.GetHeight() );
    }

    /** Sets this object's current relative X-position.
        @param vX The position
    */
    virtual void SetRelXPos(const eC_Value &vX);

    /** Sets this object's current relative Y-position.
        @param vY The position
    */
    virtual void SetRelYPos(const eC_Value &vY);

    /** Sets this object's current relative X-position of the object's lower
        right corner + 1 to the given X-position 'vX' by modifying X-position
        of the object accordingly. These positions are relative to parent's
        origin.
        @note This is not necessarily the relative X-position on screen later on.
        @param vX The position
    */
    inline void SetRelXPosREdge(const eC_Value &vX) {SetRelXPos(vX - m_vWidth);}

    /** Sets this object's current relative Y-position of the object's lower
        right corner + 1 to the given Y-position 'vY' by modifying Y-position
        of the object accordingly. These positions are relative to parent's
        origin.
        @note This is not necessarily the relative Y-position on screen later on.
        @param vY The position
    */
    inline void SetRelYPosBEdge(const eC_Value &vY) {SetRelYPos(vY - m_vHeight);}

    /** Sets this object's current relative X-position of the object's center
        to the given X-position 'vXCenter' by modifying X-position
        of the object accordingly. These positions are relative to parent's
        origin.
        @note This is not necessarily the relative X-position on screen later on.
        @param vXCenter The position
    */
    inline void SetRelXPosCenter(const eC_Value &vXCenter) {SetRelXPos(vXCenter  - GetHalfWidth());}

    /** Sets this object's current relative Y-position of the object's center
        to the given Y-position 'vYCenter' by modifying Y-position
        of the object accordingly. These positions are relative to parent's
        origin.
        @note This is not necessarily the relative Y-position on screen later on.
        @param vYCenter The position
    */
    inline void SetRelYPosCenter(const eC_Value &vYCenter) {SetRelYPos(vYCenter  - GetHalfHeight());}

    /** Sets this object's width to a new value.
        @param vX the new width
    */
    virtual void SetWidth(const eC_Value &vX);

    /** Sets this object's height to a new value.
        @param vY the new height
    */
    virtual void SetHeight(const eC_Value &vY);

    /** Returns the distance from this object's right edge to the parent's right edge.
        If the parent pointer is NULL it will just return 0 in the release build and
        in the debug build an assertion is violated additionally.
        @return The distance in pixel
    */
    eC_Value GetDistanceRight() const;

    /** Returns the distance from this object's bottom edge to the parent's bottom edge.
        If the parent pointer is NULL it will just return 0 in the release build and
        in the debug build an assertion is violated additionally.
        @return The distance in pixel
    */
    eC_Value GetDistanceBottom() const;

    /** Returns the bounding rectangle of the object in absolute coordinates.
        Note that the rectangle will be one pixel higher
        and wider than the actual object. This is because Guiliani's drawing functions exclude
        the rightmost and lowermost edge of rectangles.
        For an CGUIObject on Pos(0;0) with a width of 9 pixels and a height of 9 pixesls
        this will return a CGUIRect(0,0,9,9);
        @see GetAbsXPosREdge()
        @see GetAbsYPosBEdge()
        @see CGfxWrap::Rect()
        @return The bounding rectangle of the object in absolute coordinates
    */
    inline CGUIRect GetAbsRect() const
    {
        return CGUIRect(
            GetAbsXPos(), GetAbsYPos(),
            GetAbsXPosREdge(), GetAbsYPosBEdge());
    }

    /** Returns a copy of the bounding rectangle of this object in coordinates that are
        relative to the parent object.
        @return The bounding rectangle of the object in relative coordinates
    */
    inline CGUIRect GetRelRect() const
    {
        return CGUIRect(
            GetRelXPos(), GetRelYPos(),
            GetRelXPosREdge(), GetRelYPosBEdge());
    }

    /** Returns a copy of the bounding rectangle of this object as if it would be placed
        at the origin.
        @return The bounding rectangle of the object as if in origin
    */
    inline CGUIRect GetOriginRect() const
    {
        return CGUIRect(
            eC_FromInt(0), eC_FromInt(0),
            GetWidth(), GetHeight());
    }

    /** Returns this object's current width.
        @note This is not necessarily the width of this object on screen.
        @return The width
    */
    inline eC_Value GetWidth() const {return m_vWidth;}

    /** Returns this object's current height.
        @note This is not necessarily the height of this object on screen.
        @return The height
    */
    inline eC_Value GetHeight() const {return m_vHeight;}

    /** Returns this object's current relative X-position.
        @note This is not necessarily the relative X-position of this object on screen.
        @return The x-position
    */
    inline eC_Value GetRelXPos() const {return m_vXPosRel;}

    /** Returns this object's current relative Y-position.
        @note This is not necessarily the relative Y-position of this object on screen.
        @return The Y position
    */
    inline eC_Value GetRelYPos() const {return m_vYPosRel;}

    /** Returns this object's current relative X-position of the object's
        lower right corner + 1. This position is relative to parent's origin.
        @note This is not necessarily the relative X-position on screen.
        @return The X position
    */
    inline eC_Value GetRelXPosREdge() const {return m_vXPosRel + m_vWidth;}

    /** Returns this object's current relative Y-position of the object's
        lower right corner + 1. This position is relative to parent's origin.
        @note This is not necessarily the relative X-position on screen.
        @return eC_Value The Y position
    */
    inline eC_Value GetRelYPosBEdge() const {return m_vYPosRel + m_vHeight;}

    /** Returns the X-position of the object's lower right corner + 1 in screen
        coordinates (absolute position) at the time of the last redraw.
        If the function is called in DoDraw() the result will reflect the state
        of the upcoming redraw.
        Why + 1 ? Because Guiliani's drawing functions do not draw the last pixel.
        e.g. FilledRect(0, 0, 10, 10 ); the rect is drawn from (0,0) to (9,9)
        (both included)
        But if you draw a rectangle with (FilledRect(GetAbsXPos(),GetAbsYPos(),
        GetAbsXPosREdge(), GetAbsYPosBEdge()) then the lower right corner of the
        object will be drawn, because GetAbsXPosREdge() returns the last pixel +1
         @return eC_Value The X position
   */
    inline eC_Value GetAbsXPosREdge() const {return GetAbsXPos() + m_vWidthVisible;}

    /** Returns the Y-position of the object's lower right corner + 1 in screen
        coordinates (absolute position) at the time of the last redraw.
        If the function is called in DoDraw() the result will reflect the state
        of the upcoming redraw.
        Why + 1 ? Because Guiliani's drawing functions do not draw the last pixel.
        e.g. FilledRect(0, 0, 10, 10 ); the rect is drawn from (0,0) to (9,9)
        (both included)
        But if you draw a rectangle with (FilledRect(GetAbsXPos(),GetAbsYPos(),
        GetAbsXPosREdge(), GetAbsYPosBEdge()) then the lower right corner of the
        object will be drawn, because GetAbsYPosBEdge() returns the last pixel +1
        @return eC_Value The Y position
    */
    inline eC_Value GetAbsYPosBEdge() const {return GetAbsYPos() + m_vHeightVisible;}

    /** Returns this object's absolute X-position at the time of the last redraw.
        Custom controls should tend to call this function in DoDraw() only. The result
        reflects the visible origin of this object on screen, thus it will not change
        between redraws.

        This function is for example used by the 4-way navigation, which uses the
        geometric layout of all objects at the time of the last redraw to
        determine the next object to focus.

        @note In custom controls you should prefer to calculate with relative positions,
        that is, you should not rely on where any of your parents are positioned.
        Thus, custom controls should always consider their left/upper corner to be at
        a logical position of (0,0). Only at the very end of drawing, layouting or
        event handling relative coordinates need to be converted to absolute coordinates
        by the framework.

        If you need to calculate the distance between objects on the screen or similar
        operations that need absolute coordinates (outside of DoDraw), you may have to
        use this function but keep in mind, that these reflect the state of this object
        at the time of the passed redraw.

        m_vXPosAbsParent is the sum of all relative positions of all parents at the
        time of the last redraw and m_vXPosRelVisible is the relative X-coordinate
        at the time of the last redraw.
        @see GetAbsYPos(), GetCurrentAbsXPos(), GetRelXPos()
        @return The X position
    */
    inline eC_Value GetAbsXPos() const {return m_vXPosAbsParent + m_vXPosRelVisible;}

    /** Returns this object's absolute Y-position at the time of the last redraw.
        @see GetAbsXPos, GetCurrentAbsYPos(),GetRelYPos
        @return The Y position
    */
    inline eC_Value GetAbsYPos() const {return m_vYPosAbsParent + m_vYPosRelVisible;}

    /** Calculates the current horizontal center of the object relative to the parent.
        @return The X position
    */
    inline eC_Value GetRelXPosCenter() const { return GetRelXPos() + GetHalfWidth();}

    /** Calculates the current horizontal center of the object relative to itself.
        @return The half width
    */
    inline eC_Value GetHalfWidth() const { return eC_Div(m_vWidth, eC_FromInt(2));}

    /** Calculates the current vertical center of the object relative to the parent.
        @return The Y position
    */
    inline eC_Value GetRelYPosCenter() const { return GetRelYPos() + GetHalfHeight();}

    /** Calculates the current vertical center of the object relative to itself.
        @return The half height
    */
    inline eC_Value GetHalfHeight() const { return eC_Div(m_vHeight, eC_FromInt(2));}

    /** Calculates the horizontal center of the object at the time of the last redraw.
        @return The X position
    */
    inline eC_Value GetAbsXPosCenter() const { return GetAbsXPos() + eC_Div(m_vWidthVisible, eC_FromInt(2));}

    /** Calculates the vertical center of the object at the time of the last redraw.
        @return The Y position
    */
    inline eC_Value GetAbsYPosCenter() const { return GetAbsYPos() + eC_Div(m_vHeightVisible, eC_FromInt(2));}

    /** Get the current absolute X position of this object, recalculated from
        the relative positions of all parents.
        Be aware: If relative X coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is the sum
        of the current relative X positions of all its parents.
        @return The current absolute X position of the given object.
        @see GetAbsXPos(), GetCurrentAbsYPos(), GetRelXPos()
    */
    eC_Value GetCurrentAbsXPos() const;

    /** Get the current absolute Y position of this object, recalculated from
        the relative positions of all parents.
        Be aware: If relative Y coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is the sum
        of the current relative Y positions of all its parents.
        @return The current absolute Y position of the given object.
        @see GetAbsYPos(), GetCurrentAbsXPos(), GetRelYPos()
    */
    eC_Value GetCurrentAbsYPos() const;

    /** Get the current absolute X position of the right edge of this object,
        recalculated from the current relative positions of all its parents.
        Be aware: If relative X coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is based on the sum
        of the current relative X positions of all its parents.
        @return The current absolute Y position of the given object.
    */
    inline eC_Value GetCurrentAbsXPosREdge() const
    {
        return GetCurrentAbsXPos() + GetWidth();
    }

    /** Get the current absolute Y position of the bottom edge of this object,
        recalculated from the current relative positions of all its parents.
        Be aware: If relative Y coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is based on the sum
        of the current relative Y positions of all its parents.
        @return The current absolute Y position of the given object.
    */
    inline eC_Value GetCurrentAbsYPosBEdge() const
    {
        return GetCurrentAbsYPos() + GetHeight();
    }

    /** Get the current absolute bounding rectangle of this object,
        recalculated from the current relative positions of all its parents.
        @return The absolute bounding rectangle of the given object.
        Be aware: If relative coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is based on the sum
        of the current relative positions of all its parents.
    */
    CGUIRect GetCurrentAbsRect() const;

    /** Calculates the current horizontal center of the object.
        Be aware: If relative X coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is based on the sum
        of the current relative X positions of all its parents.
        @return The current absolute X position
    */
    inline eC_Value GetCurrentAbsXPosCenter() const { return GetCurrentAbsXPos() + GetHalfWidth();}

    /** Calculates the current vertical center of the object.
        Be aware: If relative Y coordinates of any parents of the reference object change then the
        position returned by this function will most likely also change, because it is based on the sum
        of the current relative Y positions of all its parents.
        @return The current absolute Y position
    */
    inline eC_Value GetCurrentAbsYPosCenter() const { return GetCurrentAbsYPos() + GetHalfHeight();}

    /** Assign this object to a different parent.
        This will automatically remove it from the current parent's childlist, and
        append it to the new parent.
        @param pParent Pointer to new parent object
    */
    virtual void SetParent( CGUICompositeObject* const pParent);

    /** Returns a pointer to this object's parent object.

        There is another implementation of the same method with a slightly
        different signature, GetParent() const. The compiler will choose the
        appropriate method depending on what return type the caller expects.
        @return this object's parent.
    */
    inline CGUICompositeObject * GetParent() {return m_pParentObject;}

    /** Returns a pointer to this object's parent object.

        There is another implementation of the same method with a slightly
        different signature, GetParent(). The compiler will choose the
        appropriate method depending on what return type the caller expects.
        @return this object's parent.
    */
    inline const CGUICompositeObject * GetParent() const {return m_pParentObject;}

    /** This method checks if a given object is an ancestor of this object
        @param pAncestor Pointer to object of which this might be a descendant
        @return True if this is a descendant of pAncestor, otherwise False.
                Also, False if pAncestor is NULL.
    */
    eC_Bool IsDescendantOf(const CGUIObject* const pAncestor) const;

    /** Searches for an object by its ID.

        There is another implementation of the same method with a slightly
        different signature, GetObjectByID(const ObjectHandle_t&) const. The
        compiler will choose the appropriate method depending on what return
        type the caller expects.
        @param eID ID of the object for which to search.
        @return Returns this-pointer if this object's ID equals the given eID, otherwise NULL
    */
    inline CGUIObject* GetObjectByID(const ObjectHandle_t &eID)
    {
        // Call the const-GetObjectByID function, which is the real implementation.
        return const_cast<CGUIObject*>(static_cast<const CGUIObject*const>(this)->GetObjectByID(eID));
    }

    /** Searches for an object by its ID.

        There is another implementation of the same method with a slightly
        different signature, GetObjectByID(). The compiler will choose the
        appropriate method depending on what return type the caller expects.
        @param eID ID of the object for which to search.
        @return Returns this-pointer if this object's ID equals the given eID, otherwise NULL
    */
    const CGUIObject* GetObjectByID(const ObjectHandle_t &eID) const;

    /** Retrieves a pointer to the object, which has the top most position at the given coordinates.
        This will be called recursively along the object tree.
        This method is virtual, so that users are free to implement controls, which have a sensitive area
        that differs from the actual object's rectangular shape.
        The standard implementation will check if the given coordinates lie inside of the object's
        absolute rectangle and return the object's 'this' pointer if yes, and NULL otherwise.
        Objects that are invisible or that have the 'ClickThrough' attribute set, will not be found
        and therefore return NULL.
        @param vAbsX Absolute X coordinate for which the topmost object shall be returned
        @param vAbsY Absolute Y coordinate for which the topmost object shall be returned
        @return pointer to the topmost object at the given position
    */
    virtual CGUIObject *FindObject(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Sets the ID of the object.
        @param eID Object ID to set
    */
    inline void SetID(const ObjectHandle_t &eID) {m_eID = eID;}

    /** Gets the ID of the object.
        @return ID of the object
    */
    inline ObjectHandle_t GetID() const { return m_eID; }

    /** Set focus to any focusable child object in the subtree of this object, including the object itself
        @return eC_Bool True if focus was obtained, False otherwise
    */
    virtual eC_Bool FocusAny() {return RequestFocus();}

    /** Set focus to the next focusable child object after the currently focused one.
        The user should call this function on the currently focused object with no
        arguments.
        @see CGUICompositeObject::FocusNext()
        @param pRefObj Is usually a child of this object which indicates the current position in the
            child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under pRefObj has
            already been asked to take focus, or if not.
        @return eC_Bool True if focus was obtained, False otherwise
    */
    virtual eC_Bool FocusNext(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    /** Set focus to the previous focusable child object before the currently focused one.
        The user should call this function on the currently focused object with no
        arguments.
        @see CGUICompositeObject::FocusPrevious()
        @param pRefObj Is usually a child of this object which indicates the current position in the
            child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under pRefObj has
            already been asked to take focus, or if not.
        @return eC_Bool True if focus was obtained, False otherwise
    */
    virtual eC_Bool FocusPrevious(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    /** Visualizes the object. It is forbidden to call SetRelXPos, SetRelYPos,
        SetWidth or SetHeight in DoDraw, i.e., it is not allowed to change the layout.
        @return True if anything was drawn, False otherwise
    */
    virtual eC_Bool DoDraw();

    /** Visualizes the object by drawing its rendered texture.
        @return True if anything was drawn, False otherwise
    */
    virtual eC_Bool DoDrawSnapshot();

    /** This method requests the focus for the current object. It will only receive
        the focus, if the currently focused object is willing to release it.
        @return True if the focus was received, False otherwise.
        @see ClaimFocus
    */
    eC_Bool RequestFocus();

    /** Called if the object is requested to lose the focus. It must return False if the
        object is willing to lose the focus and True otherwise. Please notice that the
        focus may get moved to another control even if the focused object claims the
        focus, as the event handler may for example force the focus into a modal dialog.
        @return True if the focus was received, False otherwise.
        @see RequestFocus
    */
    virtual eC_Bool ClaimFocus();

    /** Invalidates the area covered by this object, thus causing DoDraw() to be called
        during the GUI's next redraw. If the object is invisible, this method does not
        invalidate anything.
    */
    void InvalidateArea();

    /** Intersects the given rect with the rects of all parent objects in the
        object hierarchy. The invisibility of all parents will be checked as well
        and returned.

        Invalidation only makes sense if all parents are visible, as well as this
        object!

        If the given rect is not the absolute rect of this object, the given rect should be
        intersected with the absolute rect of this object before this call.
        @param[in,out] rRect rect that should be used for calculation; returned will be the
                       calculated invalidation rect
        @return Whether this object is invisible and it would therefore not
                make sense to invalidate its area. This means that the return
                value is True if the object itself is invisible or any parent
                is invisible, or if the object is not a child of the GUI (not
                within the object tree).
    */
    eC_Bool CalculateInvalidatedRect(CGUIRect& rRect) const;

    /** Sets the right neighbor, that is used by the 4-way focusing algorithm.
        @param eNewNeighborRightID The ID of the object to the right ('east') of this object.
    */
    inline void SetNeighborRightID(const ObjectHandle_t eNewNeighborRightID)
        { m_eNeighborRightID = eNewNeighborRightID; }

    /** Sets the left neighbor, that is used by the 4-way focusing algorithm.
        @param eNewNeighborLeftID The ID of the object to the left ('west') of this object.
    */
    inline void SetNeighborLeftID(const ObjectHandle_t eNewNeighborLeftID)
        { m_eNeighborLeftID = eNewNeighborLeftID; }

    /** Sets the top neighbor, that is used by the 4-way focusing algorithm.
        @param eNewNeighborTopID The ID of the object to the top ('north') of this object.
    */
    inline void SetNeighborTopID(const ObjectHandle_t eNewNeighborTopID)
        { m_eNeighborTopID = eNewNeighborTopID; }

    /** Sets the bottom neighbor, that is used by the 4-way focusing algorithm.
        @param eNewNeighborBottomID The ID of the object to the bottom ('south') of this object.
    */
    inline void SetNeighborBottomID(const ObjectHandle_t eNewNeighborBottomID)
        { m_eNeighborBottomID = eNewNeighborBottomID; }

    /** Sets all neighbors, that are used by the 4-way focusing algorithm.
        @param eNewNeighborLeftID The ID of the object to the left ('west') of this object.
        @param eNewNeighborRightID The ID of the object to the right ('east') of this object.
        @param eNewNeighborTopID The ID of the object to the top ('north') of this object.
        @param eNewNeighborBottomID The ID of the object to the bottom ('south') of this object.
    */
    inline void SetNeighborIDs(
        const ObjectHandle_t eNewNeighborLeftID,
        const ObjectHandle_t eNewNeighborRightID,
        const ObjectHandle_t eNewNeighborTopID,
        const ObjectHandle_t eNewNeighborBottomID)
    {
        m_eNeighborLeftID = eNewNeighborLeftID;
        m_eNeighborRightID = eNewNeighborRightID;
        m_eNeighborTopID = eNewNeighborTopID;
        m_eNeighborBottomID = eNewNeighborBottomID;
    }

    /** Used by the 4-way focusing algorithm.
        @return The ID of the object to the right of this object.
    */
    inline ObjectHandle_t GetNeighborRightID() const { return m_eNeighborRightID; }

    /** Used by the 4-way focusing algorithm.
        @return The ID of the object to the left of this object.
    */
    inline ObjectHandle_t GetNeighborLeftID() const { return m_eNeighborLeftID; }

    /** Used by the 4-way focusing algorithm.
        @return The ID of the object above ('north') of this object.
    */
    inline ObjectHandle_t GetNeighborTopID() const { return m_eNeighborTopID; }

    /** Used by the 4-way focusing algorithm.
        @return The ID of the object below ('south') of this object.
    */
    inline ObjectHandle_t GetNeighborBottomID() const { return m_eNeighborBottomID; }

    /** Attaches the supplied behaviour to this object.

        @note This will delete any former behaviour attached to this object.
        @param pBehaviour Pointer to CGUIBehaviourDecorator object which is to be appended
    */
    void SetBehaviour( CGUIBehaviourDecorator* pBehaviour);

    /** Returns the pointer to the current behaviour.
        @return Pointer to this object's behaviour.
    */
    inline CGUIBehaviourDecorator * GetBehaviour()
    {
        return m_pBehaviour;
    }

    /** Returns the pointer to the current behaviour.
        @return Pointer to this object's behaviour.
    */
    inline const CGUIBehaviourDecorator * GetBehaviour() const
    {
        return m_pBehaviour;
    }

    /** Sets the layouter for this object.
        <b>Attention:</b> This will delete any other layouter which is currently attached to this object.
        @param pLayouter Pointer to CGUILayouter instance, which will be attached to this object
        @see CGUILayouter::SetObject()
    */
    void SetLayouter(CGUILayouter *pLayouter);

    /** Returns the pointer to the current layouter.
        @return Pointer to this object's layouter.
    */
    inline CGUILayouter* GetLayouter()
    {
        return m_pLayouter;
    }

    /** Returns a const pointer to the current layouter.
        @return const Pointer to this object's layouter.
    */
    inline const CGUILayouter* GetLayouter() const
    {
        return m_pLayouter;
    }

    /** Recalculates the object's layout.
        @param eMovedEdges This indicates in which direction the distance to the parent object's borders has changed
                           (Only relevant for layouters, which depend on their parent object's size, such as CGUILayouterAnchor)
    */
    virtual void RefreshLayout(CGUILayouter::eMovedEdges_t eMovedEdges) const;

    /** Set the object's status to grayed out (true) or <i>not</i> grayed out (false).
        @param bGrayOut Boolean value indicating the new state.
    */
    virtual void SetGrayedOut(const eC_Bool &bGrayOut);

    /** Set the object's status to invisible (true) or <i>not</i> invisible (false).
        @param bInvisible Boolean value indicating the new state.
    */
    virtual void SetInvisible(const eC_Bool &bInvisible);

    /** Set the object's status to disabled (true) or <i>not</i> disabled (false).
        Does not influence the appearance of the control. Therefore, the control
        will not be drawn again after changing this flag.
        @param bDisabled Boolean value indicating the new state.
    */
    virtual void SetDisabled(const eC_Bool &bDisabled);

    /** Set the object's state to focusable (true) or <i>not</i> focusable (false).
        @param bFocussable Boolean value indicating the new state.
    */
    virtual void SetFocussable(const eC_Bool &bFocussable);

    /** Prevents the object from moving the focus upwards in the GUI Object tree.
        Setting the StopFocus flag for a CompositeObject will prevent the focus
        from moving out of this CompositeObject once it is inside.
        @param bDontFocusToParent TRUE if the focus is not allowed to leave this object.
    */
    inline void SetStopFocus(const eC_Bool &bDontFocusToParent)
    {
        m_bStopFocus = bDontFocusToParent;
    }

    /** Set m_bClickThrough to a new value.
        This specifies whether this object can be clicked. Else clicks will go right through
        it and are thereby forwarded to an object lying under it (in z-order).
        The typical use case for this flag is to pass click events to a brother of this
        object when this and the brother object overlap and the overlapped
        part of the brother is below this object.
        Attention: This attribute is not streamed
        @param bClickThrough the new value
    */
    inline void SetClickThrough( const eC_Bool &bClickThrough)
    {
        m_bClickThrough = bClickThrough;
    }

    /** This method checks if an object is focusable. This refers to whether it can be focused with the
        mouse/keyboard via functions like FocusNext().
        @return True if focusable, False otherwise
    */
    eC_Bool IsFocussable() const;

    /** This method checks if an object is highlightable. This refers to whether it can be highlighted via
        the event handler.  If true the event handler may mark this object as the currently highlighted one.
        Each Guiliani control should have a configurable visual representation for the highlighted state,
        via configuration files, so that it may be used as generic controls and still be able not to use
        the highlighted representation if the highlighted state and the non-highlighted state are set to
        the same. E.g. if a control is used in a scrollable list, which may have any control as member
        (i.e. a generic Guiliani object) the user should have the possibility to tell the contained objects
        that there is no special highlighted visual representation, via configuration files, e.g. by setting
        the highlighted image for the contained image buttons to the same as the standard image of each.
        Thus, there is no special flag in CGUIObject that determines if an object is highlightable.
        @return True if highlightable, False otherwise
    */
    inline eC_Bool IsHighlightable() const { return !m_bIsGrayedOut && !m_bIsInvisible && !m_bIsDisabled; }

    /** This method checks if an object is able to be dragged. This refers to whether it can be dragged with the
        mouse/keyboard via the event handler. If true, the event handler may call DoDrag of this object.
        @return True if draggable, False otherwise
    */
    inline eC_Bool IsDragable() const { return IsClickable(); }

    /** This method checks if an object is able to be clicked. This refers to whether it can be clicked with the
        mouse/keyboard via the event handler. If true the event handler may call DoClick or DoDoubleClick of
        this object.
        @return True if clickable, False otherwise
    */
    eC_Bool IsClickable() const;

    /** Checks if the object is the one which is in focus.
        @return True if the object is in focus, False otherwise
    */
    eC_Bool IsFocussed() const;

    /** Checks if the object is being highlighted.
        @return True if the object is highlighted, False otherwise
    */
    eC_Bool IsHighlighted() const;

    /** Checks if the object is currently being pressed.
        @return True if the object is pressed, False otherwise
    */
    eC_Bool IsPressed() const;

    /** Checks if the object is currently being dragged.
        @return True if the object is dragged, False otherwise
    */
    eC_Bool IsDragged() const;

    /** Checks if the grayed out object state is set. By default, if an object
        is grayed out it will be displayed with a different visual shape than
        usual in order to give the user a hint that it will never be
        clickable or highlighted but may be dragged if it supports dragging.
        @return True if highlightable, False otherwise
    */
    inline eC_Bool IsGrayedOut() const { return m_bIsGrayedOut; }

    /** Checks if the invisible object state is set. If an object is invisible
        it will not be drawn, i.e., its DoDraw() will not be called.
        @return True if invisible, False otherwise
    */
    inline eC_Bool IsInvisible() const { return m_bIsInvisible; }

    /** Checks if the disabled object state is set. By default, if an object
        is disabled then it will never get the focus and is not clickable.
        DoClick() and RequestFocus() will not be called on disabled objects.
        @return True if disabled, False otherwise
    */
    inline eC_Bool IsDisabled() const{ return m_bIsDisabled; }

    /** @see SetClickThrough()
        @return True if click-through, False otherwise
    */
    inline eC_Bool IsClickThrough() const
    {
        return m_bClickThrough;
    }

     /** Checks if the parent object may be asked to get focus via FocusNext() and
        FocusPrevious(). The flag that is returned alters the algorithm used to search
        in the creation tree.
        @return True if parent supports it
    */
    inline eC_Bool IsStopFocus() const { return m_bStopFocus; }

    /** Checks if a control is derived from CGUICompositeObject or if it is just
        derived from CGUIObject. Thus, this function should not be overridden by
        any other class than CGUICompositeObject but may be called by user code.
        @return True if composite 
    */
    virtual eC_Bool IsCompositeObject() const {return false;}


    /** Accessor for the alpha (transparency) value of the object
        @return value in the range of 0 (transparent) to 255 (opaque) */
    inline eC_UByte GetAlpha() const {return m_ubAlpha;}

    /** Sets the alpha (transparency) value of the object
        @param ubAlpha Value in the range of 0 (transparent) to 255 (opaque) */
    void SetAlpha(eC_UByte ubAlpha);

    /** Renders this GUIObject into the bitmapPlane with given ID and the given X and Y Offset.
        @param uiBitmapID ID of the BitmapPlane in which this control should be drawn.
        @param vXOffset Draw offset in x direction. Drawing of this control starts at position vXOffset
        @param vYOffset Draw offset in y direction. Drawing of this control starts at position vYOffset
   */
    void RenderToBitmap(eC_UInt uiBitmapID, eC_Value vXOffset = eC_FromInt(0), eC_Value vYOffset = eC_FromInt(0));

    /** Creates a Snapshot of this control. That means this control will be rendered to its own BitmapPlane. If this
        Control does not have a BitmapPlane so far, one will be created.
        @param uiBackColor Color, which will be used as background for the snapshot. By default, it's set to 0x00, 
        but for GfxWrapers which do not support alpha BitmapPlanes another back color maybe needed.
    */
    virtual void CreateSnapshot(eC_UInt uiBackColor = 0x00);

    /** Deletes the BitmapPlane of this GUIObject.
        This BitmapPlane has been created due to EnableSnapshotDrawing() or by CreateSnapshot.
    */
    void DeleteSnapshot();

    /** Accessor for the ID of the BitmapPlane of this GUIObject
        @returns ID of the BitmapPlane of this GUIObject
    */
    GUIBitmapPlaneID_t GetBitmapID() const {return m_uiBitmapID;}

    /** Accessor for the BitmapPlane of this GUIObject
        @returns BitmapPlane of the GUIObject
    */
    CGUIBitmapPlane& GetBitmapPlane() const;

    /** Enables SnapshotDrawing. If Snapshotdrawing is enabled, the BitmapPlane of this control will be drawn.
        Instead of DoDraw(), DoDrawSnapshot() will be called.
    */
    virtual void EnableSnapshotDrawing();

    /** Disables SnapshotDrawing. If Snapshotdrawing is disabled, this control will be drawn as usual.
        By default, the existing BitmapPlane will be deleted.
        @param bDeleteSnapshot If true the BitmapPlane of this control will be deleted
    */
    virtual void DisableSnapshotDrawing(eC_Bool bDeleteSnapshot = true);

    /** Accessor for the Snapshot-Draw-Status.
        @returns True if snapshot drawing is enabled, otherwise False
    */
    inline eC_Bool IsDrawingSnapshot() const {return m_bDrawSnapshot;}

    /** Adds an observer to get informed when the value is changed.
        virtual void CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0)
        will be called whenever the value changes.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddValueObserver(CGUIObserver* pObserver)
    {
        GetDataPoolConnector().AddObserver(pObserver);
    }

    /** Unsubscribe an observer that subscribed as value observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveValueObserver(CGUIObserver* pObserver)
    {
        if( m_pkDataPoolConnector)
            m_pkDataPoolConnector->RemoveObserver(pObserver);
    }

    /** Notifies all observing DataPoolEntries of this object's DataPoolConnector that it's value has changed
        @param kValue The new value
        @param uiX X-Index within the DataPoolEntries at which to set the new Value 
        @param uiY Y-Index within the DataPoolEntries at which to set the new Value 
    */
    void NotifyValueObservers(const CGUIValue& kValue, const eC_UInt uiX=0, const eC_UInt uiY=0)
    {
        if( m_pkDataPoolConnector)
            m_pkDataPoolConnector->NotifyObservers(kValue, this, uiX, uiY);
    }

    /** Returns this object's DataPoolConnector and creates it, if it did not previously exist.
        @return Reference to this object's DataPoolConnector */
    CDataPoolConnector& GetDataPoolConnector();

    /** Indicates whether this object has a DataPoolConnector. This will NOT automatically create one.
        @see GetDataPoolConnector()
        @return true if a DataPoolConnector exists, false otherwise. */
    eC_Bool DataPoolConnectorExists() const 
    {
        return (m_pkDataPoolConnector != NULL);
    }

    /** Generic interface for setting object values.
        The SetValue() API enables you to forward <i>any</i> data of type CGUIValue to <i>any</i> object.
        How this value finally gets interpreted is up to the receiving object's implementation.<p>
        This concept is particularly useful for linking GUI-objects to an external data source. You can for
        instance forward an integer value as a CGUIValue object to a GUIObject without the need to know whether
        it is a CGUITextField, or a CGUIProgressBar, or something completely different. The receiving 
        object will interpret the value in the most sensible way. In the given example the integer will be
        displayed as a string in the Textfield, while it will cause the ProgressBar to fill up to the respective value.
        @param rkValue DataPoolEntry containing the value to be handed over to the object on which SetValue was called.
        @return Always False in base class, normally True if value was applied and False in case of error.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        NotifyValueObservers( rkValue);
        // Default returns false to notify caller that the value could not be interpreted by this control
        return false; 
    }

#ifndef GUILIANI_NO_DATAPOOL
    /** Generic interface used by the DataPool for setting values in GUIObjects.
        By default, this maps directly onto the SetValue() API dealing with a single CGUIValue.
        @param rkValue Reference to the DataPoolEntry used for retrieving the value(s)
        @return The return value of the called API (Usually True if OK and False in case of error).
    */
    virtual eC_Bool SetValue( CDataPoolEntry& rkValue)
    {
        return SetValue( rkValue.Get());
    }
#endif

#ifdef GUILIANI_STREAM_GUI
    /**
        Reads all object attributes from streaming file. Only for use with GUIFactory.
        GUIStreamingFileReader has to be set in CGUI::Init().
        @see CGUIFactory
    */
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    /**
        Writes all object attributes to the streaming file. StreamWriter has to be initialized.
        Only for use with GUIStreamWriter.
        @param bWriteClassID This flag is used to select if writing of ControlID, leading and trailing tags
            is performed. Only GUIStreamWriter and GUICompositeObject have to set this flag true.
        @see CGUIStreamWriter
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a control class ID into a stream.
    static const eC_Char XMLTAG_CONTROLCLASSID[];

    /// XML tag to be used writing overridden neighbors into a stream.
    static const eC_Char XMLTAG_OVERRIDDENNEIGHBORS[];

    /// XML tag to be used writing object id into a stream.
    static const eC_Char XMLTAG_OBJECTID[];

#endif

protected:
    /** Called when the focus should be set to the next control that is in the
        given direction of the currently focused object.
        @param eFocusDirection The focus direction.
        @return If True, then the algorithm breaks immediately and will have
            no effect. Thus, custom controls may override this function slot
            in order to set the focus to a specific control and afterwards
            force the 4-way navigation algorithm to stop. The automatism will
            always return False.
        @see CGUI::FourWayFocusNext()
    */
    virtual eC_Bool FourWayNext(FourWayFocus_t eFocusDirection);

    /** Creates a BitmapPlane for this control.
    The ID of this BitmapPlane can be accessed via GetBitmapID(). The
    BitmapPlane can be accessed with GetBitmapPlane().
    If SnapshotDrawing is enabled (EnableSnapshotDrawing()), this BitmapPlane
    is drawn during DoDrawSnapshot().
    The method CreateSnapshot() also renders to this BitmapPlane.
    @return True if a BitmapPlane has been created, otherwise False.
            This happens if a BitmapPlane for this control already exists.
    */
    virtual eC_Bool CreateBitmapPlane();

private:
    /** Sets this object's parent to the given CGUICompositeObject and removes it from its former parent.
        (This is an internal method which must only be called by CGUICompositeObject)
        @param pParent Pointer to new parent object
    */
    void SetParentInternal(CGUICompositeObject* const pParent);

    /** This function should never be called by user code.
        For a simple object, to handle a draw request means to actually draw it.
        For a standardized event handling mechanism, this method must exist.
        Note that erroneous overriding of this method may result in corrupted
        graphical output of the GUI.
    */
    virtual void HandleDraw(const CGUIRect &ClipRect);

    /** This function should not be called by user code.
        It does what is necessary before HandleDraw() is called by
        the framework.
    */
    void UpdateInvalidation();

    /**
        Method-slot, overridden in CGUICompositeObject.
        This function is only called from FocusNext().

        It requests the focus and if it is rejected, it will call
        the parent's FocusNext().

        @return The return value shows if the first focusable object took the focus or has
            been unable to take it.
        @see FocusNext()
    */
    virtual eC_Bool FocusNextDown();

    /**
        Method-slot, overridden in CGUICompositeObject.
        This function is only called from FocusPrevious().

        It requests the focus and if it is rejected, it will call
        the parent's FocusPrevious().

        @return The return value shows if the last focusable object took the focus or has
            been unable to take it.
        @see FocusPrevious()
    */
    virtual eC_Bool FocusPreviousDown();

    /** Helper function for the constructors. It initializes all members.
    */
    void Init(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID);

    /** Helper function for the four way focus to decide whether the object is in sector.
        @param eFocusDirection The focus direction.
    */
    eC_Bool IsInSector(FourWayFocus_t eFocusDirection) const;

    /**
    this method notifies the datapool of a new object and updates the pointer
    if the object-id has been registered, the value of the control is also updated.
    */
    void NotifyOfCreation();

    /** Relative position of this object. Can be understood as an offset relative to
        its parent object's absolute position.
    */
    eC_Value m_vXPosRel, m_vYPosRel;

    /// Width and Height of the object
    eC_Value m_vWidth, m_vHeight;

    /// pointer to the parent object
    CGUICompositeObject     *m_pParentObject;

    /// pointer to the attached behavioural decorator. This will be DELETED when the CGUIObject's destructor is called.
    CGUIBehaviourDecorator  *m_pBehaviour;

    /// pointer to layout object
    CGUILayouter            *m_pLayouter;

    /// ID of the object.
    ObjectHandle_t m_eID;

    /// Manually overridden neighbor to the right, NO_HANDLE if not overridden.
    ObjectHandle_t m_eNeighborRightID;

    /// Manually overridden neighbor to the left, NO_HANDLE if not overridden.
    ObjectHandle_t m_eNeighborLeftID;

    /// Manually overridden neighbor to the bottom, NO_HANDLE if not overridden.
    ObjectHandle_t m_eNeighborBottomID;

    /// Manually overridden neighbor to the top, NO_HANDLE if not overridden.
    ObjectHandle_t m_eNeighborTopID;

    /** This pointer is a helper that is used to decide if the FocusNext(),
        FocusPrevious() and their helper functions shall respect the
        m_bCyclicFocus or m_bStopFocus flag at their current object in the
        tree.
        The so-called "terminating object" in this context is the focused object
        itself or the first parent in direction to the root that has either the
        m_bCyclicFocus or m_bStopFocus flag set.
        The cyclic and StopFocus flags shall not be considered when
        searching in the subtree of the terminating object.
        The recursion shall not get closer to the root than this terminating
        object, that is, it shall never jump up over the terminating object.
    */
    static const CGUIObject* ms_pFocusTerminatingObj;

    /** Used by FocusNext(), FocusPrevious() and their helper functions:
        The algorithm stops if RequestFocus() is called a second time on the
        ms_pFirstRequestedObj, as this is when the whole tree has been searched
        through. Thus, you may not delete this object when a recursion of these
        functions is running.
    */
    static const CGUIObject* ms_pFirstRequestedObj;

    /** Width and Height of the object at the time of the last redraw. These values
        are valid right after the object is created via the non-streaming constructor
        or streamed in.
    */
    eC_Value m_vWidthVisible, m_vHeightVisible;

    /** Relative position of parent object at the time of the last redraw. These values
        are valid right after the object is created via the non-streaming constructor
        or streamed in.
    */
    eC_Value m_vXPosRelVisible, m_vYPosRelVisible;

    /** Absolute position of parent object. Results from the combination of its parent's
        absolute and relative coordinates at the time of the passed redraw. This
        position gets updated by the framework in UpdateInvalidation() so before
        this function is called they reflect the state of the passed redraw and
        afterwards they reflect the state of the upcoming redraw.

        Thus, the absolute position for all objects right after being loaded by the
        streaming mechanism is zero, as nothing is visible on screen at this time.
        The only point in time when these values are updated is in UpdateInvalidation(),
        which is called right before DoDraw(). In DoDraw() the absolute positions of all
        objects in the object tree are valid, not just those of current branch of the
        object being redrawn, but usercode should avoid to rely on this.

	@note User code should never alter these values or unexpected results may occur.
    */
    eC_Value m_vXPosAbsParent, m_vYPosAbsParent;

    /** Defines if the parent's subtree including the parent object is part of 
        this object's focus selection order.
        This means that the focus may leave this object and be forwarded to its parent
        by using FocusNext() or FocusPrevious().
    */
    eC_Bool m_bStopFocus;

    /** @see SetClickThrough()
    */
    eC_Bool m_bClickThrough;

    /** If this is set and the object is not in "disabled" state then the focus may be
        offered to this object.
    */
    eC_Bool m_bIsFocussable;

    /// Status information. Is the object grayed out?
    eC_Bool m_bIsGrayedOut;

    /// Status information. Is the object Invisible?
    eC_Bool m_bIsInvisible;

    /// Status information. Is the object disabled?
    /// This flag does not influence the appearance of the control.
    eC_Bool m_bIsDisabled;

    /// If True then this object's old position (the position at the time of
    /// the last redraw) has been invalidated and the new position will be
    /// invalidated right before HandleDraw(), because at that time the final
    /// position for the next redraw can be determined but not before.
    /// @note No user code should alter this variable directly.
    eC_Bool m_bMarkedForInvalidation;

    /// Status flag indicating whether the object is visualized using a formerly taken snapshot (that means a BitmapPlane)
    /// or if its DoDraw() method will be called (which is the default case).
    eC_Bool m_bDrawSnapshot;

    /// ID of associated snapshot bitmap plane
    GUIBitmapPlaneID_t m_uiBitmapID;

    /// Alpha value
    eC_UByte m_ubAlpha;

    /// DataPoolConnector object, which allows objects to observe the DataPool and to be observed by the DataPool
    CDataPoolConnector* m_pkDataPoolConnector;
};
#endif
