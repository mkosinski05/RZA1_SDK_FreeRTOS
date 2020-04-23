/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_SIZE__H_
#define GUIANIMATION_SIZE__H_

#include "GUIAnimation.h"
#include "GUIEasing.h"

/**
@brief This animation resizes a CGUIObject from a start dimension to a target dimension.

The duration for the animation determines the animation's speed.

Example:
@code

// This example resizes the object to half width and half height.

// The animation shall be done in 5 seconds.
const eC_UInt uiDuration = 5000;
const eC_Value vStartWidth = pkAnimatedObject->GetWidth();
const eC_Value vStartHeight = pkAnimatedObject->GetHeight();
const eC_Value vTargetWidth = pkAnimatedObject->GetHalfWidth();
const eC_Value vTargetHeight = pkAnimatedObject->GetHalfHeight();

// Now create the instance of the animation.
CGUIAnimationSize* pAnimation = new CGUIAnimationSize(pkAnimatedObject, CGUIEasing::EASE_LINEAR, CGUIEasing::EASE_LINEAR,
                vStartWidth, vStartHeight, vTargetWidth, vTargetHeight, uiDuration);

// To start the animation call the StartAnimation() method of the animation.
pAnimation->StartAnimation();

//If the instance of the animation is not available directly use the AnimationHandler.
GETANIMATIONHDL.GetAnimation(uiMyAnimationID);
@endcode


@ingroup GUILIANI_ANIMATIONS

*/
class CGUIAnimationSize : public CGUIAnimation
{
public:
    /**
    Constructor
    @param pAnimatedObject The Object that shall be resized.
    @param eEasingWidth The easing type that is used for the width of the object.
    @param eEasingHeight The easing type that is used for the height of the object.
    @param vStartWidth Width at start of animation.
    @param vStartHeight Height at start of animation.
    @param vTargetWidth Target width of animated object.
    @param vTargetHeight Target height of animated object.
    @param bKeepCenterPosition If TRUE then the object's center position remains unchanged while resizing.
                               if FALSE then the position of the object's top/left corner remains unchanged.
    @param uiDuration The duration for the complete animation in ms.
    @param uiStepTime The time between two animation steps in ms.
    */
    CGUIAnimationSize(CGUIObject* pAnimatedObject,
        CGUIEasing::EasingType_t eEasingWidth,
        CGUIEasing::EasingType_t eEasingHeight,
        eC_Value vStartWidth,
        eC_Value vStartHeight,
        eC_Value vTargetWidth,
        eC_Value vTargetHeight,
        eC_Bool bKeepCenterPosition,
        eC_UInt uiDuration,
        eC_UInt uiStepTime = 25);

#if defined(GUILIANI_STREAM_GUI)
    /**
    Default constructor for streaming.
    */
    CGUIAnimationSize();
#endif

    /**
    This function contains the animation logic. It typically accesses the animation's
    CGUIEasing functions to get the current values and sets these values in the animated object.
    Sets the width and height of the animated object using the CGUIEasing functions' current values.
    This function is called by DoAnimate()
    @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    void ApplyAnimation(eC_UInt uiElapsedTime);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt SIZE_ANIMATION_CLASS_VERSION; ///< The class version of the size animation class
    /// The minimal class version allowed.
    static const eC_UInt SIZE_ANIMATION_CLASS_MIN_VERSION;
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:

    CGUIEasing::EasingType_t m_eEasingWidth;     ///< Holds the type of the easing used for the width.
    CGUIEasing::EasingType_t m_eEasingHeight;    ///< Holds the type of the easing used for the height.
    eC_Value    m_vStartWidth;                   ///< Width at start of animation.
    eC_Value    m_vStartHeight;                  ///< Height at start of animation.
    eC_Value    m_vTargetWidth;                  ///< Target width of animated object.
    eC_Value    m_vTargetHeight;                 ///< Target height of animated object.
    eC_Bool     m_bKeepCenterPosition;           ///< Indicates if the object's center-point shall remain unchanged during animation
};

#endif
