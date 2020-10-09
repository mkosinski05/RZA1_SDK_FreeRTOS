/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_BLINKING__H_
#define GUIANIMATION_BLINKING__H_

#include "GUIAnimation.h"

/**
   @brief This animation implements a blinking effect.

    It makes the object invisible after a specified "On-time" and visible again
    after a specified "Off-time".

    This animation does not use CGUIEasing functions.

    Example:
    @code

    eC_UInt uiDuration = 0; // The animation shall run infinitely.
    eC_UInt uiOnTime = 500; // On time is 500 ms.
    eC_UInt uiOffTime = 300; // Off time is 300 ms.

    // Create the instance of the animation.
    CGUIAnimationBlinking* pAnimation = new CGUIAnimationBlinking(pkAnimatedObject, uiDuration, uiOnTime, uiOffTime);

    // To start the animation call the StartAnimation() method of the animation.
    pAnimation->StartAnimation();

    @endcode
    @ingroup GUILIANI_ANIMATIONS

*/
class CGUIAnimationBlinking : public CGUIAnimation
{
public:
    /**
        Constructor
        @param pAnimatedObject The Object that shall be animated.
        @param uiDuration The duration defines how long the animation will be running. Use 0 for endless animation.
        @param uiOnTime The time interval in which the object shall be visible in milliseconds.
        @param uiOffTime The time interval in which the object shall be invisible in milliseconds. When no time or 0 is passed, OnTime will be used.
    */
    CGUIAnimationBlinking(CGUIObject* pAnimatedObject,
                      eC_UInt uiDuration,
                      eC_UInt uiOnTime,
                      eC_UInt uiOffTime = 0);

#if defined(GUILIANI_STREAM_GUI)
    /**
        Default constructor for streaming.
    */
    CGUIAnimationBlinking();
#endif

   /**
    This function contains the animation logic. It typically accesses the animation's
    CGUIEasing functions to get the current values and sets these values in the animated object.
    This function is called by DoAnimate()
    @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    void ApplyAnimation(eC_UInt uiElapsedTime);

    /**
        Makes sure that the object is visible when the animation is stopped or deleted.
        @param eStatus The new status of the animation.
    */
    void OnStatusChanged(AnimationStatus_t eStatus);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// The class version of the implementation.
    static const eC_UInt BLINKING_ANIMATION_CLASS_VERSION;
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:

    eC_UInt m_uiOnTime; ///< The time for which the object shall be visible in milliseconds.
    eC_UInt m_uiOffTime; ///< The time for which the object shall be invisible in milliseconds.
};

#endif
