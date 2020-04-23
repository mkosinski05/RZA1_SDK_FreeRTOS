/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICENTERFOCUSCONTAINER__H_
#define GUICENTERFOCUSCONTAINER__H_

#include "GUICompositeObject.h"
#include "SafeGUIObjectPtr.h"

/// Container object which automatically centers on the currently focused child object.

// @guiliani_doxygen toplevel_control Center Focus Container
/**
    <table border="0">
        <tr>
            <td width="200">@image html center_focus_container.png</td>
            <td>
                The "center focus" control is a container that serves as a container which automatically centers 
                the currently focused control at the supplied position. This centering can be animated with effects 
                like speed-up or slow-down  (Class: CGUICenterFocusContainer).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The CGUICenterFocusContainer serves as a container-object which automatically centers the currently
    focused object on the supplied position, if it is a descendant in the GUI-hierarchy. It does so by
    moving itself (and therefore all children) to the new position.

    Example:
@code

    // Create a Container.
    CGUICenterFocusContainer* pkCenterFocusContainer = new CGUICenterFocusContainer(this,
            eC_FromInt(0), eC_FromInt(0), eC_FromInt(150), eC_FromInt(150),
            eC_FromInt(50), eC_FromInt(50),
            OBJ_CONTAINER);

    // Get the X position (in relative coordinates) of the center point.
    eC_Value vXPos = pkCenterFocusContainer->GetFocusCenterRelX();

    // Get the Y position (in relative coordinates) of the center point.
    eC_Value vYPos = pkCenterFocusContainer->GetFocusCenterRelY();

    // Calling this method will center the given object
    // on the supplied center position - even when it is currently not focused.
    CGUIObject* pObject = new CGUIObject( pkCenterFocusContainer,
                                eC_FromInt(0), eC_FromInt(0),
                                eC_FromInt(50), eC_FromInt(50),
                                NO_HANDLE);
    pkCenterFocusContainer->SetCenterObject(pObject);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUICenterFocusContainer
    : public CGUICompositeObject
{
public:
    /** CGUICompositeObject constructor.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the composite object
        @param vHeight Height of the composite object
        @param vCenterX Horizontal center position in relative coordinates, on which to center the focused object
        @param vCenterY Vertical center position in relative coordinates, on which to center the focused object
        @param pParent Pointer to the designated parent object
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUICenterFocusContainer(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Value &vCenterX, const eC_Value &vCenterY,
        const ObjectHandle_t &eID = NO_HANDLE);

    /**    Standard constructor
        @see CGUIObject().
    */
    CGUICenterFocusContainer();

    virtual ~CGUICenterFocusContainer();

    /// Helper function for initialization
    void Init();

    /** This is being called when an object, which is a descendant of the CGUICenterFocusContainer, receives the focus
        In case SetCenterOnImmediateChildrenOnly() has been set to false (which is the default), the CGUICenterFocusContainer
        will move if any object within its tree of children receives the focus, regardless of whether it is a direct descendant
        or one which lies deeper within the object tree. If SetCenterOnImmediateChildrenOnly() is set to true, the container will
        only center on immediate children - in other words children whose direct parent it is.
    */
    void GetFocus();

    /** The animation-implementation will interpolate the container's position so that the focused object gets centered on the
        defined center position
        @param vTimes Correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Calling this method will center the given object on the supplied center position - even when it is currently not focused.
        @param pObject Pointer to a child object of this container. The container will move to center pObject on the given center position.
    */
    void SetCenterObject( CGUIObject* const pObject);

    /** Returns the current center object.
        @return Object on which the CGUICenterFocusContainer is currently focused */
    CGUIObject* GetCenterObject() const { return m_pCenterObject; }

    /** The inertia value specifies how quick the container will move to a new position.
        Supplying bigger inertia values will slow down the speed.
        @param vInertiaX Inertia value in horizontal direction (no animation in X axis if <= 0)
        @param vInertiaY Inertia value in vertical direction (no animation in Y axis if <= 0)*/
    void SetInertia( eC_Value vInertiaX, eC_Value vInertiaY) { m_vInertiaX = vInertiaX; m_vInertiaY = vInertiaY; }

    /** Immediately centers the current center object on the given center-position without animation.
       
        Be aware that if relative coordinates of any object on the path to the root object of the
        center object are changed after calling this function, then most probably the center object
        will be positioned incorrectly because the positioning of JumpToCenterObject() is based on
        the current absolute position.
        The absolute position during redraw is the sum of relative positions of the parents on the
        path to the root object so it depends on such relative positions.
    */
    void JumpToCenterObject();

    /** Retrieves the current Y position (in relative coordinates) of the center point, relative to
        the parent of this container.
        @return The Y position
    */
    eC_Value GetFocusCenterRelY() const;

    /** Retrieves the current X position (in relative coordinates) of the center point, relative to
        the parent of this container.
        @return The X position
    */
    eC_Value GetFocusCenterRelX() const;

    /** Sets the coordinates of the center point. This is the position (in coordinates relative to the CenterFocusContainer's parent)
        on which focused child objects will be centered.
        @param vCenterX X position (in relative coordinates) of the center point.
        @param vCenterY Y position (in relative coordinates) of the center point. */
    void SetFocusCenter( eC_Value vCenterX, eC_Value vCenterY);

    /** This specifies whether only immediate children (i.e. children whose parent is the CGUICenterFocusContainer itself)
        will be centered on the center-position once they receive the focus, or if child objects which are descendants of the
        container deeper in the object-tree will also be centered.
        @param bCenterOnImmediateChildrenOnly TRUE will only center immediate children, FALSE will center any descendant. */
    void SetCenterOnImmediateChildrenOnly( eC_Bool bCenterOnImmediateChildrenOnly);

    /// This implements dragging for the CenterFocusContainer
    eC_Bool DoDrag( const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);
    eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Returns the current X position (in absolute coordinates) of the center point.
        @return absolute x coordinate of the center point
    */
    eC_Value GetFocusCenterCurrentAbsX() const;

    /** Retrieves the current Y position (in absolute coordinates) of the center point.
        @return absolute y coordinate of the center point
    */
    eC_Value GetFocusCenterCurrentAbsY() const;

    /** Sets the sensitivity of the CenterFocusContainer for drag events.
        The user can scroll through the child objects of the center focus container with the mouse/finger by dragging them.
        When dragging, the sensitivity values specify after which pixel distance the CenterFocusContainer will move the focus
        to the next child object. E.g., a vertical drag sensitivity of 50 will move the focus to the next child if the user
        moves the mouse/finger at least 50 pixels in vertical direction.
        You can disable dragging in either vertical or horizontal direction by setting the corresponding sensitivity to 0.
        @param uiHorizontalSensitivity horizontal pixel distance after which the focus is moved to the next child
        @param uiVerticalSensitivity vertical pixel distance after which the focus is moved to the next child
    */
    void SetDragSensitivity( eC_UInt uiHorizontalSensitivity, eC_UInt uiVerticalSensitivity)
    {
        m_uiHorDragSensitivity=uiHorizontalSensitivity;
        m_uiVertDragSensitivity=uiVerticalSensitivity;
    }

private:
    void StartDrag(eC_Value vAbsX, eC_Value vAbsY);

    CSafeGUIObjectPtr m_pCenterObject;
    eC_Bool     m_bCenterOnImmediateChildrenOnly;
    eC_Value    m_vInertiaX;
    eC_Value    m_vInertiaY;
    eC_Value    m_vFocusCenterX;
    eC_Value    m_vFocusCenterY;
    eC_Value    m_vTotalDragX;
    eC_Value    m_vTotalDragY;
    eC_UInt     m_uiVertDragSensitivity;
    eC_UInt     m_uiHorDragSensitivity;
    eC_Bool     m_bStopFocusState;
    eC_Bool     m_bDragging;
};
#endif
