/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIEASING__H_
#define GUIEASING__H_

#include "eC_Types.h"

/** @brief A CGUIEasing can be used for Guiliani animations.

It provides access to the most commonly used easing algorithms.
They calculate the current value dependent on its time
based on 3 parameters (begin value, change value and duration) and
of course the easing equation.

@code
void CGUIAnimationMove::ApplyAnimation(eC_UInt uiElapsedTime)
{
    CGUIEasing  kEasing;
    CGUIObject* pAnimatedObj = GetAnimatedObject();

    if (pAnimatedObj != NULL)
    {
        pAnimatedObj->InvalidateArea();

        if (m_eEasingX != CGUIEasing::EASE_NONE)
            pAnimatedObj->SetRelXPos(kEasing(m_eEasingX, eC_FromInt(uiElapsedTime), m_vStartX, m_vTargetX - m_vStartX, eC_FromInt(GetDuration())));
        if (m_eEasingY != CGUIEasing::EASE_NONE)
            pAnimatedObj->SetRelYPos(kEasing(m_eEasingY, eC_FromInt(uiElapsedTime), m_vStartY, m_vTargetY - m_vStartY, eC_FromInt(GetDuration())));

        pAnimatedObj->InvalidateArea();
    }
}
@endcode

@ingroup GUILIANI_ANIMATIONS

*/
class CGUIEasing
{
public:
    /// The differnet easing types
    enum EasingType_t
    {
        EASE_NONE,

        EASE_LINEAR,

        EASE_IN_SINE,
        EASE_OUT_SINE,
        EASE_IN_OUT_SINE,
        EASE_OUT_IN_SINE,

        EASE_IN_QUAD,
        EASE_OUT_QUAD,
        EASE_IN_OUT_QUAD,
        EASE_OUT_IN_QUAD,

        EASE_IN_CUBIC,
        EASE_OUT_CUBIC,
        EASE_IN_OUT_CUBIC,
        EASE_OUT_IN_CUBIC,

        EASE_IN_QUART,
        EASE_OUT_QUART,
        EASE_IN_OUT_QUART,
        EASE_OUT_IN_QUART,

        EASE_IN_EXPO,
        EASE_OUT_EXPO,
        EASE_IN_OUT_EXPO,
        EASE_OUT_IN_EXPO,

        EASE_IN_QUINT,
        EASE_OUT_QUINT,
        EASE_IN_OUT_QUINT,
        EASE_OUT_IN_QUINT,

        EASE_IN_CIRC,
        EASE_OUT_CIRC,
        EASE_IN_OUT_CIRC,
        EASE_OUT_IN_CIRC,

        EASE_IN_ELASTIC,
        EASE_OUT_ELASTIC,
        EASE_IN_OUT_ELASTIC,
        EASE_OUT_IN_ELASTIC,

        EASE_IN_BACK,
        EASE_OUT_BACK,
        EASE_IN_OUT_BACK,
        EASE_OUT_IN_BACK,

        EASE_IN_BOUNCE,
        EASE_OUT_BOUNCE,
        EASE_IN_OUT_BOUNCE,
        EASE_OUT_IN_BOUNCE,
        EASE_NUM_OF
    };

    /// Constructor
    CGUIEasing() {}

    /// Destructor
    virtual ~CGUIEasing(){}

    /**
        This function object is called by the animation to calculate the new value
        of an attribute depending on time (vTime) and chosen easing function.

        @param eEasingType ID of chosen easing type.
        @param vTime Time elapsed (in ms) since start of animation.
        @param vBegin Start value of the animated attribute.
        @param vChange At the end of the animation (duration reached) the
        animated attribute gain vBegin + vChange.
        @param vDuration Duration of animation (in ms). This defines the valid time interval
        (0/duration) used for the easing interpolation.
        @return The result of the chosen easing function (interpolation) .
    */
    virtual eC_Value operator()(EasingType_t eEasingType, eC_Value vTime, eC_Value vBegin, eC_Value vChange, eC_Value vDuration);

    /**
        This function is called by the animation to calculate the new value
        of an attribute depending on time (vTime) and chosen easing function.

        @param eEasingType ID of chosen easing type.
        @param vTime Time elapsed (in ms) since start of animation.
        @param vBegin Start value of the animated attribute.
        @param vChange At the end of the animation (duration reached) the
        animated attribute gain vBegin + vChange.
        @param vDuration Duration of animation (in ms). This defines the valid time interval
        (0/duration) used for the easing interpolation.
        @return The result of the chosen easing type (interpolation).
    */
    virtual eC_Value Ease(EasingType_t eEasingType, eC_Value vTime, eC_Value vBegin, eC_Value vChange, eC_Value vDuration);

private:
    /**
    This function calculates result via linear easing equation.

    @param vTime Time elapsed (in ms) since start of animation.
    @param vBegin Start value of the animated attribute.
    @param vChange At the end of the animation (duration reached) the
    animated attribute gain vBegin + vChange.
    @param vDuration Duration of animation (in ms). This defines the valid time interval
    (0/duration) used for the easing interpolation.
    @return The result of the linear easing function (interpolation) .
    */
    const eC_Value EasingLinear(const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInSine (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutSine (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutSine (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInSine (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInQuad (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutQuad (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutQuad (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInQuad (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInCubic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutCubic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutCubic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInCubic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInQuart (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutQuart (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutQuart (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInQuart (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInExpo (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutExpo (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutExpo (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInExpo (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInQuint (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutQuint (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutQuint (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInQuint (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInCircular (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutCircular (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutCircular (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInCircular (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInElastic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutElastic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutElastic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInElastic (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInBack (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutBack (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutBack (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInBack (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);

    const eC_Value EaseInBounce (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutBounce (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseInOutBounce (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
    const eC_Value EaseOutInBounce (const eC_Value vTime, const eC_Value vBegin, const eC_Value vChange, const eC_Value vDuration);
};

#endif
