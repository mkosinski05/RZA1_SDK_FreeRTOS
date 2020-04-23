/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_MOVE__H_
#define GUIANIMATION_MOVE__H_

#include "GUIAnimation.h"
#include "GUIEasing.h"

/**
   @brief This animation translates a CGUIObject from a start position to a target position.

    The duration for the animation determines the animation's speed.

    The animation is achieved by interpolating between the start position and the target position.
    For the interpolation CGUIEasing functions are used. The CGUIEasing functions are passed in the constructor.

    Example:
    @code

    // The animation shall be done in 5 seconds.
    const eC_Value vStartX = eC_FromInt(100);
    const eC_Value vStartY = eC_FromInt(100);
    const eC_Value vTargetX = eC_FromInt(200);
    const eC_Value vTargetY = eC_FromInt(200);
    const eC_UInt uiDuration = 5000;

    // Now create the instance of the animation.
    CGUIAnimationMove* pAnimation = new CGUIAnimationMove(pkAnimatedObject, CGUIEasing::EASE_LINEAR, CGUIEasing::EASE_LINEAR,
                                                            vStartX, vStartY, vTargetX, vTargetY, uiDuration);

    // To start the animation call the StartAnimation() method of the animation.
    pAnimation->StartAnimation();

    //If the instance of the animation is not available directly use the AnimationHandler.
    eC_UInt uiMyAnimationID = pAnimation->GetID();

    GETANIMATIONHDL.GetAnimation(uiMyAnimationID);

    @endcode
    @ingroup GUILIANI_ANIMATIONS

*/


class CGUIAnimationMove : public CGUIAnimation
{
public:
    /**
        Constructor
        @param pAnimatedObject The Object that shall be moved.
        @param eEasingX The easing type that is used for the XPos of the object.
        @param eEasingY The easing type that is used for the YPos of the object.
        @param vStartX X-position at start of animation.
        @param vStartY Y-position at start of animation.
        @param vTargetX Target X-position of animation.
        @param vTargetY Target Y-position of animation.
        @param uiDuration The duration for the complete animation in ms.
        @param bUseCurrentPosition If set to true the start position of the animation will be set to the current position of the animated object.
        @param uiStepTime The time between two animation steps in ms.
    */
    CGUIAnimationMove(CGUIObject* pAnimatedObject,
                        CGUIEasing::EasingType_t eEasingX,
                        CGUIEasing::EasingType_t eEasingY,
                        eC_Value vStartX,
                        eC_Value vStartY,
                        eC_Value vTargetX,
                        eC_Value vTargetY,
                        eC_UInt uiDuration,
                        eC_Bool bUseCurrentPosition = false,
                        eC_UInt uiStepTime = 25);

#if defined(GUILIANI_STREAM_GUI)
    /**
        Default constructor for streaming.
    */
    CGUIAnimationMove();
#endif

    /**
    This function contains the animation logic. It typically accesses the animation's
    CGUIEasing functions to get the current values and sets these values in the animated object.
    This function is called by DoAnimate()
    @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    void ApplyAnimation(eC_UInt uiElapsedTime);

    /**
    Sets the animated object to the start position.
    */
    void InitAttributes();

    /**
        Set the start position of the move animation.
        @param vStartX X coordinate of the starting point of the animation.
        @param vStartY Y coordinate of the starting point of the animation.
    */
    void SetStartPos(const eC_Value vStartX, const eC_Value vStartY);

    /**
        Set the target position of the move animation.
        @param vTargetX X coordinate of the target point of the animation.
        @param vTargetY Y coordinate of the target point of the animation.
    */
    void SetTargetPos(const eC_Value vTargetX, const eC_Value vTargetY);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// The class version of the implementation.
    static const eC_UInt MOVE_ANIMATION_CLASS_VERSION;
    /// The minimal class version allowed.
    static const eC_UInt MOVE_ANIMATION_CLASS_MIN_VERSION;
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    CGUIEasing::EasingType_t m_eEasingX;    ///< Holds the type of the XPos easing.
    CGUIEasing::EasingType_t m_eEasingY;    ///< Holds the type of the YPos easing.
    eC_Value    m_vStartX;                  ///< X-position at start of animation.
    eC_Value    m_vStartY;                  ///< Y-position at start of animation.
    eC_Value    m_vTargetX;                 ///< Target X-position of animation.
    eC_Value    m_vTargetY;                 ///< Target Y-position of animation.
};

#endif
