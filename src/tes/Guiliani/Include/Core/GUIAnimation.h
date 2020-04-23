/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION__H_
#define GUIANIMATION__H_

#include "GUIObject.h"
#include "GUIStreamableObject.h"

class CGUIAnimationObserver;
class CGUIAnimationHandler;
class CGUIObject;

// @guiliani_doxygen toplevel_module Animations
/**
    <table border="0">
    <tr>
        <td width="200">@image html animation.png</td>
            <td>
                The "animations module" is providing standard animations for animation of objects.
                The object can appear in different ways:
                It can be
                - shown immediately
                - dropped from above fast
                - dropped slowly from above
                - dropped from above but it will be bouncing like a rubber ball
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/** @brief Base class for modular Guiliani animations.

An animation object typically manipulates values of a GUIObject.
After a specified step time, a timer event triggers the animation callback DoAnimate.
GUIAnimations are using CGUIEasing functions to calculate the current attribute values.

DoAnimate triggers the CGUIEasing functions to recalculate the current value for attributes based on the elapsed time.
Afterwards ApplyAnimation is called. This function reads the current values of the CGUIEasing functions and sets them
in the animated object.

Basically there are two types of animations.

1. Animations with a specified duration.
The CGUIEasing functions calculate the step size based on the duration, the StartValue and the TargetValue.

StepSize = (TargetValue - StartValue)/Duration;


2. Endless Animations.
Endless animations do not have a defined end. They only have a step time after which DoAnimate is triggered.
To have an endless animation the duration has to be set to 0.
The CGUIEasing functions will calculate the step size for each animation step without using the duration.

StepSize = (TargetValue - StartValue);

It is possible to set an AnimationObserver which is informed whenever the status of the animation changes.
Use the SetAnimationObserver() interface to register an observer.

@note When reading the GUI from stream, the ReadFromStream() method will not set its observer pointer.
      It is the responsibility of the animation observer to set itself as observer in the animation again when it is reading from stream.

    @ingroup GUILIANI_ANIMATIONS

*/
class CGUIAnimation : public CGUIAnimatable, public CGUIStreamableObject
{
    friend class CGUIAnimationHandler;

public:

    /** Called by the GUITimer after the callback time expired.
        Calls ApplyAnimation().
        @param vTimes Ignored by base class, used by implemented animation to compensate for jitter
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /**
    This function contains the animation logic. It typically accesses the animation's
    CGUIEasing functions to get the current values and sets these values in the animated object.
    This function is called by DoAnimate()
    @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    virtual void ApplyAnimation(eC_UInt uiElapsedTime) = 0;

    /**
    Start the animation. This function registers a timer callback.
    ApplyAnimation() will be called after StepTime expired.
    */
    void StartAnimation();

    /**
    Called by StartAnimation(). Reimplement this method to initialize animation attributes
    before the animation is started if necessary.
    */
    virtual void InitAttributes() {}

    /**
    Stop the animation. The timer callback will be removed.
    */
    void StopAnimation();

    /**
    Pause the animation. The timer callback will be removed.
    */
    void PauseAnimation();

    /**
    Continue the animation. The timer callback will be restored (installed again).
    */
    void ContinueAnimation();

    /**
        Checks if the animation is currently paused.
        @return True if an animation is currently paused
    */
    eC_Bool IsPaused() const
    {
        return (m_uiExpiredTime != 0);
    }

    /**
        Set the animated object.
        @param pkObject The object which will be animated.
    */
    void SetAnimatedObject(CGUIObject* pkObject) {m_pAnimatedObject = pkObject;}

    /**
    @return the animated GUIObject.
    */
    CGUIObject* GetAnimatedObject() const {return m_pAnimatedObject;}

    /**
    @return The total duration of the animation in milliseconds. If the animation has no defined end, the duration is 0.
    */

    eC_UInt GetDuration() const;

    /**
    Set the duration for the Animation in milliseconds.
    @param uiDuration The new duration time.
    */
    void SetDuration(eC_UInt uiDuration);

    /**
    @return The time for each animation step in milliseconds.
    */
    eC_UInt GetStepTime() const {return m_uiStepTime;}

    /**
    @return The unique id of the animation which is set when adding the animation to the CGUIAnimationHandler.
    */
    eC_UInt GetID() const {return m_uiID;}

    /**
    @return True if this animation will be deleted once it is finished.
    This is only applicable for animations with a defined end.
    If no duration was set, the animation has no defined end.
    */
    eC_Bool IsDeletedAfterFinish() const {return m_bDeletedAfterFinish;}

    /**
    Specifies if this animation will be deleted once it is finished.
    This is only applicable for animations with a defined end.
    If no duration was set, the animation has no defined end.
    @param bDeletedAfterFinish If True, delete the animation after is has finshed
    */
    void SetDeletedAfterFinish(const eC_Bool bDeletedAfterFinish) {m_bDeletedAfterFinish = bDeletedAfterFinish;}

    /**
    Set the animation observer. The observer will be informed
    by calling CGUIAnimationObserver::OnStatusChanged, whenever the status of the animation changes.
    @param pObserver The observer of the animation.
    */
    void SetAnimationObserver(CGUIAnimationObserver* pObserver)
    {
        m_pAnimationObserver = pObserver;
    }

    /**
    Status message which is used for the animation observer which is set with SetAnimationObserver().
    */
    enum AnimationStatus_t
    {
        ANIMATION_RUNNING,  ///< The animation has just started running.
        ANIMATION_PAUSED,   ///< The animation has just been paused.
        ANIMATION_STOPPED,  ///< The animation has stopped. Either forcefully, or because it has ended.
        ANIMATION_FINISHED, ///< The animation has reached its end, and does therefore finish animating.
        ANIMATION_DELETED,  ///< The animation has been deleted.
    };

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a animation class ID into a stream.
    static const eC_Char XMLTAG_ANIMATIONCLASSID[];

    /// XML tag to be used when writing a animation ID into a stream.
    static const eC_Char XMLTAG_ANIMATIONID[];
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);

#endif

    /**
    Destructor
    */
    virtual ~CGUIAnimation();

protected:

#if defined(GUILIANI_STREAM_GUI)
    /**
        Default constructor for streaming.
        This constructor does not add the animation to the AnimationHandler
        since this is done after the AnimationHandler read the animation from stream.
    */
    CGUIAnimation();
#endif

    /**
    Constructor
    Adds the animation to the AnimationHandler.
    @param pkAnimatedObject The animated GUIObject. See GetAnimatedObject().
    @param uiDuration The duration of the animation in milliseconds.
    @param uiStepTime The step time of each animation step in milliseconds. See GetStepTime().
    @param bDeletedAfterFinish True if the animation shall be deleted after it was finished. (only for animations with a defined end).
    */
    CGUIAnimation(
        CGUIObject* pkAnimatedObject,
        eC_UInt uiDuration,
        eC_UInt uiStepTime = 25,
        eC_Bool bDeletedAfterFinish = false);

    /**
    Called when the animation status changes.
    Informs the animation observer and removes the animation
    callback when the animation is no longer running.
    @note If overriding this function make sure that the base implementation is called to inform the observers.
    @param eStatus The new status of the animation.
    */
    virtual void OnStatusChanged(AnimationStatus_t eStatus);

private:

    eC_UInt m_uiID;        ///< The unique ID of the animation which is provided by the AnimationHandler when adding the animation.

    eC_UInt m_uiDuration;  ///< The duration of the animation in milliseconds.

    eC_UInt m_uiStepTime;  ///< The time between two animation steps in milliseconds.

    eC_UInt m_uiStartTime; ///< Holds the start time of the animation. This is used to stop an animation which has no CGUIEasing functions.

    eC_UInt m_uiExpiredTime; ///< Holds the already expired time of the animation. This is used to continue a previously paused animation.

    /// The object that is animated by the animation.
    CGUIObject*         m_pAnimatedObject;

    /// Holds a pointer to the animation observer that shall be informed when the animation status changes.
    CGUIAnimationObserver* m_pAnimationObserver;

    /// Determines if the animation is deleted after it is finished.
    eC_Bool            m_bDeletedAfterFinish;
};

#endif
