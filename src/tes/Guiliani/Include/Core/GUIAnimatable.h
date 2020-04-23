/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_ANIMATABLE_H_
#define GUI_ANIMATABLE_H_

#include "GUITrace.h"
#include "GUITimer.h"

/** @defgroup GUILIANI_ANIMATIONS Animations
    @brief Group of classes within Guiliani which are related to animation and timing functionality

Animations are one of the key features of Guiliani.

The GUITimer together with the GUIAnimatable interface gives the possibility
to implement animations based on timer callbacks.

Each GUIObject itself is an animatable and can react on timer callbacks.

For modular animations use CGUIAnimation and CGUIAnimationChain.
Animations are using CGUIEasing functions to calculate the current value for an object's attribute.
In each animation step a Guiliani animation updates the attributes of its animated object with this
current value.

For more details please check the class documentation of CGUIAnimation.

Examples for animations (including example code) can be found here:

CGUIAnimationMove

CGUIAnimationMoveInOut

CGUIAnimationSize
*/

/** @brief Base interface for animation callbacks.

    When registering a timer callback for an animatable by calling CGUITimer::AddAnimationCallback
    the GUITimer uses this interface when the callback time expires.

    Typically, the implementation of CGUIAnimation adds the animation callback when starting the animation.

    @ingroup GUILIANI_ANIMATIONS
*/
class CGUIAnimatable
{
public:

    /// Destructor. Removes all existing animation callbacks.
    virtual ~CGUIAnimatable()
    {
        if (GETTIMER.IsAnimating(this) == true)
        {
            GETTIMER.RemoveAnimationCallback( this);
        }
    }

    /** Called when the timer of an animation callback belonging to this animatable has expired.
    @param  vTimes The factor of the registered callback time in relation to the actually elapsed time.
    For example, the elapsed time is 80ms, and the standard time is 50ms, then
    vTimes is 1.6. If the given time in GETTIMER.AddAnimationCallback is 0,
    It will be set to -1.
    */
    virtual void DoAnimate(const eC_Value &vTimes = eC_FromInt(1))
    {
        GUILOG( GUI_TRACE_DEBUG," GUIAnimatable's default DoAnimate() method has just been called. \n"
            "Could it be that you planned to call an overridden DoAnimate()-method, but the overridden method's name or parameter list does not match? \n");
    }

    /**
        Start the animation. This function typically registers a timer callback.
        The base class just defines the interface and does nothing.
        @see CGUIAnimation
    */
    virtual void StartAnimation()
    {
        GUILOG( GUI_TRACE_DEBUG," GUIAnimatable's default StartAnimation() method has just been called. \n"
            "Could it be that you planned to call an overridden StartAnimation()-method, but the overridden method's name or parameter list does not match? \n");
    };

    /**
        Stop the animation. This function typically removes a timer callback.
        The base class just defines the interface and does nothing.
        @see CGUIAnimation
    */
    virtual void StopAnimation()
    {
        GUILOG( GUI_TRACE_DEBUG," GUIAnimatable's default StopAnimation() method has just been called. \n"
            "Could it be that you planned to call an overridden StopAnimation()-method, but the overridden method's name or parameter list does not match? \n");
    };

    /**
        Checks if the animatable is currently animating.
        @return True if an animation callback is registered, otherwise False.
    */
    virtual eC_Bool IsAnimating() const
    {
        return GETTIMER.IsAnimating(this);
    }
};


#endif // GUI_ANIMATABLE_H_
