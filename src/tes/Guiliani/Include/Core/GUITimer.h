/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITIMER__H_
#define GUITIMER__H_
#include "eC_TList_doubleLinked.h"

/// Retrieve pointer to Timer instance
#define GETTIMER        CGUITimer::GetInstance()

class CGUI;
class CGUIAnimatable;

/// Structure to hold timer information
struct CTimerCallback
{
    /// The temporary time used to calculate the time left after each call of CGUITimer::Process() in milliseconds.
    /** If it is less than the ElapsedTime, it is reset to the uiStartTime, and the associated
        animatable's DoAnimate() method is called. */
    eC_UInt m_uiRestTime;

    /// The time set by CGUITimer::AddAnimationCallback() in milliseconds.
    /// It will not be changed unless the user changes it.
    eC_UInt m_uiDuration;

    /// The animatable whose DoAnimate method has to be called.
    CGUIAnimatable *m_pAnimatable;

    /** The time in milliseconds that is obtained in the last call to Process() or as this callback added to the list.
    */
    eC_UInt m_uiOldTime;

    /** Constructor.
     */
    CTimerCallback():
        m_uiRestTime(0), m_uiDuration(0), m_pAnimatable(NULL), m_uiOldTime(0)
    {
    }

    /// comparison operator "equal to"
    eC_Bool operator == (const CTimerCallback& T) const { if (m_uiDuration == T.m_uiDuration && m_pAnimatable == T.m_pAnimatable ) return true; return false; }

    /// comparison operator "not equal to"
    eC_Bool operator != (const CTimerCallback& T) const { return !operator==(T); }

    /// comparison operator "larger than"
    eC_Bool operator >  (const CTimerCallback& T) const { if (m_uiRestTime > T.m_uiRestTime && m_pAnimatable == T.m_pAnimatable ) return true; return false; }
};

/// Timer used for timer callbacks to instances of CGUIAnimatable.

/**
    This timer class is useful if you wish to initiate actions in certain (maybe cyclically repeating)
    time intervals. You may add a virtually unlimited number of timer-callbacks to the timer instance and
    whenever one of them expires, the registered animatable's CGUIAnimatable::DoAnimate() method is called.

    Example:
@code
// Create an instance of a custom control.
CMyAnimatedWidget* pkWidget = new CMyAnimatedWidget( eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100) );
// Register the widget for a 500 ms callback.
GETTIMER.AddAnimationCallback( 500, pkWidget );
@endcode

    This example's widget would need to have an implementation of the
    CGUIAnimatable::DoAnimate() method:
@code
void CMyAnimatedWidget::DoAnimate(const eC_Value& vTimes)
{
    InvalidateArea(); // Invalidate the old area covered by this object.
    // ...
    // Change this widget's size or position or other attributes.
    // ...
    InvalidateArea(); // Invalidate the new area as well if the object has been moved or resized.

    if ( some_condition )
    {
        GETTIMER.RemoveAnimationCallback(this); // End the animation if some_condition is true.
    }
}
@endcode
    For a more comprehensive example of a control with animations, please see @ref page_widgetlibrary.

@ingroup GUILIANI_ANIMATIONS

*/
class CGUITimer
{
public:
    /// @return pointer to the singleton timer instance.
    static inline CGUITimer& GetInstance() { return ms_kTimer; }

    /* This function may not call GUICommandHandler functions or a deadlock will occur */
    /** Returns the current 'Guiliani time'. Internally, this is mapped to a tick
        count and should therefore only be used for difference calculations.
        @return The current Guiliani time (milliseconds).
      */
    virtual eC_UInt GetTime();

    /** Traverses the registered timer callbacks and executes an animatable's
      CGUIAnimatable::DoAnimate() method if its timer has expired.
    */
    void Process();

    /** Adds a new callback to the list.
        If another callback for the same animatable exists, it will be updated with the new time.
        @param uiTime The minimum amount of time that has to pass before the
               timer expires (milliseconds).
        @param pAnimatable The animatable whose DoAnimate method is called when the
               timer expires.
      */
    void AddAnimationCallback(eC_UInt uiTime, CGUIAnimatable *pAnimatable);

    /** Removes all callbacks for the given animatable.
        @param pAnimatable The animatable to be removed.
      */
    void RemoveAnimationCallback(CGUIAnimatable *pAnimatable);

    /** Checks whether an animation callback for a given animatable is registered
        @param pAnimatable Pointer to animatable.
        @return True if an animation callback for this animatable is currently registered, False otherwise
        */
    eC_Bool IsAnimating(const CGUIAnimatable *pAnimatable) const;

    /** Returns the time span in ms until the next animation timer expires.
        @return Time until the next timer expires (milliseconds).
      */
    eC_UInt GetIdleTime();

#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    virtual ~CGUITimer();

private:
    /// Hidden constructor.
    CGUITimer();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUITimer(const CGUITimer& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUITimer& operator=(const CGUITimer& kSource);

    /// Singleton instance.
    static CGUITimer ms_kTimer;

    /// List of active timers.
    eC_TListDoubleLinked<CTimerCallback> m_AnimationCallbackList;
};
#endif
