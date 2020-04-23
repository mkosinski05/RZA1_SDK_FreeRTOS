/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_MOVE_INOUT__H_
#define GUIANIMATION_MOVE_INOUT__H_

#include "GUIAnimation.h"
#include "GUIEasing.h"

/**
    This animation moves an object into or out of the visible area of its parent using
    its current position as start or end position.
    The Direction determines where the object is moved to or from.

    For the move direction TOP and BOTTOM the object is translated vertically,
    for LEFT and RIGHT it is translated horizontally.

    The flag bMoveOutOnStart determines if the object is moved from the current position out of the visible area of its parent (true),
    or from outside of the parents visible area to the current position (false).

    The object is set visible when the animation is constructed and therefore initialized.

    This example moves an object into the middle of the screen
    assuming that the animated object is a direct child of the GUI.

    @code
    pAnimatedObject->SetRelXPos(GETGUI.GetHalfWidth() - pAnimatedObject->GetHalfWidth());
    pAnimatedObject->SetRelYPos(GETGUI.GetHalfHeight() - pAnimatedObject->GetHalfHeight());

    CGUIAnimation* pAnimation = new CGUIAnimationMoveInOut(pAnimatedObject,
         CGUIEasing::EASE_LINEAR, CGUIEasing::EASE_LINEAR, CGUIAnimationMoveInOut::MOVE_TOP, false, 2000);

    @endcode
    @brief Animation moving an object into or out of the visible area
*/
class CGUIAnimationMoveInOut : public CGUIAnimation
{
public:

    /** Determies the move direction*/
    enum MoveDirection_t
    {
        MOVE_TOP,
        MOVE_BOTTOM,
        MOVE_LEFT,
        MOVE_RIGHT
    };

#if defined(GUILIANI_STREAM_GUI)
    /**
        Default constructor for streaming.
    */
    CGUIAnimationMoveInOut();
#endif


    /**
        Constructor
        @param pAnimatedObject The GUIObject that will be moved.
        @param eEasingX The easing type that is used for the XPos of the object.
        @param eEasingY The easing type that is used for the YPos of the object.
        @param eDirection The move direction.
        @param bMoveOutOnStart True to move the object into the screen, false to move it out of the screen.
        @param uiDuration The total duraiton of the animation in milliseconds. Based on the distance the duration determines the animation speed.
        @param uiStepTime The time between two animation steps in millisectionds.
    */
    CGUIAnimationMoveInOut(CGUIObject* pAnimatedObject,
                           CGUIEasing::EasingType_t eEasingX,
                           CGUIEasing::EasingType_t eEasingY,
                           MoveDirection_t eDirection,
                           eC_Bool bMoveOutOnStart = false,
                           eC_UInt uiDuration = 1000,
                           eC_UInt uiStepTime = 25);


    /**
    This function contains the animation logic. It typically accesses the animation's
    CGUIEasing functions to get the current values and sets these values in the animated object.
    This function is called by DoAnimate()
    Adapts the objects position after each animation step based on the current value of the CGUIEasing functions.
    @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    void ApplyAnimation(eC_UInt uiElapsedTime);

    /**
        @return the move operation. True if the object is moving out of its parent's area,
        False if it is moving into the parent's area.
    */
    eC_Bool IsMovingOut(){return m_bMoveOut;}

    /**
        @return The original XPosition of the object before the animation is started.
                This position is the start position (bMoveOutOnStart = false) / the destinaion position (bOutOnStart = true) for the animation.
    */
    eC_Value GetOriginalXPos(){return m_vOriginalXPos;}

    /**
        @return The original YPosition of the object before the animation is started.
                This position is the start position (bMoveOutOnStart = false) / the target position (bOutOnStart = true) for the animation.
    */
    eC_Value GetOriginalYPos(){return m_vOriginalYPos;}

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// The class version of the implementation.
    static const eC_UInt MOVEINOUT_ANIMATION_CLASS_VERSION;
    /// The minimal class version allowed.
    static const eC_UInt MOVEINOUT_ANIMATION_CLASS_MIN_VERSION;
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /**
        Makes the object invisible when it is moving out of the screen after the animation was finished.
        Also calls the base implementation to inform the animation observers.
        @param eStatus The new status of the animation.
    */
    void OnStatusChanged(AnimationStatus_t eStatus);

    /**
        Initializes the object position and sets the object visible.
    */
    void InitAttributes();

private:
    MoveDirection_t m_eDirection;           ///< The move direction.
    eC_Value m_vOriginalXPos;               ///< The original x position (used as start or target position).
    eC_Value m_vOriginalYPos;               ///< The original y position (used as start or target position).
    CGUIEasing::EasingType_t m_eEasingX;    ///< Holds the type of the easing in x direction.
    CGUIEasing::EasingType_t m_eEasingY;    ///< Holds the type of the easing in y direction.
    eC_Value m_vOutOfScreenX;               ///< Holds the x-position outside of screen.
    eC_Value m_vOutOfScreenY;               ///< Holds the y-position outside of screen.
    eC_Bool m_bMoveOut;                     ///< Determines if the animation is moving the object out of the screen or into the screen.
};

#endif
