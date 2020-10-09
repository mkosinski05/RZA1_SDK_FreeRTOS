/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIABSTRACTBAR__H_
#define GUIABSTRACTBAR__H_

#include "GUIObject.h"
#include "GUIRange.h"

/// Base class for bars.

/**
    It is meant to be the base class for controls like progress bar and
    slider and does not include any visualization code therefore the
    class cannot be used standalone.

    A key feature of this class is that it calls InvalidateArea() for all
    functions that might change the status of the derived control in such
    a way that the visual appearance needs to be updated. Especially all
    get-functions do not imply such graphics updates therefore they do not
    have corresponding interface functions in this class but they may rather
    be called directly on the CGUIRange member of this class.

    @ingroup GUILIANI_CONTROLS
*/
class CGUIAbstractBar : public CGUIObject
{
public:
    /** Sets the value of this bar.
        @see CGUIRange::SetValue()
        @param rkValue The CGUIValue containing the new value
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        m_kRange.SetValue(rkValue.ToInt());
        CheckForValueChange(); // This will call NotifyValueObservers
        return true; 
    }

    /** Only constant functions like get-functions of CGUIRange may be called
        directly on m_kRange from outside of this class. This is why a "const
        CGUIRange&" is returned. All others must be called via this wrapper
        class.
        @return const CGUIRange& The current range
    */
    inline const CGUIRange& GetRange() const {return m_kRange;}

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetLevel(eC_Value vPercent)
    {
        m_kRange.SetLevel(vPercent);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetLevelPercentage(eC_UInt uiPercentage)
    {
        m_kRange.SetLevelPercentage(uiPercentage);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetMinValue(eC_Int iMinValue)
    {
        m_kRange.SetMinValue(iMinValue);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetMaxValue(eC_Int iMaxValue)
    {
        m_kRange.SetMaxValue(iMaxValue);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    void SetRange(eC_Int iMinValue, eC_Int iMaxValue)
    {
        m_kRange.SetRange(iMinValue, iMaxValue);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetStepSizeLevelPercentage(eC_UInt uiPercentage)
    {
        m_kRange.SetStepSizeLevelPercentage(uiPercentage);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetStepSizeLevel(eC_Value vPercent)
    {
        m_kRange.SetStepSizeLevel(vPercent);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetStepSize(eC_UInt uiIntValues)
    {
        m_kRange.SetStepSize(uiIntValues);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    eC_Bool Increase()
    {
        if (m_kRange.Increase())
        {
            CheckForValueChange();
        }
        return false;
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    eC_Bool Decrease()
    {
        if (m_kRange.Decrease())
        {
            CheckForValueChange();
        }
        return false;
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetBase(CGUIRange::Base_t eBase)
    {
        m_kRange.SetBase(eBase);
        CheckForValueChange();
    }

    /// Calls the corresponding CGUIRange function and InvalidateArea().
    inline void SetMaximumLevelIsAlwaysValid(eC_Bool bNotOnlyMultiplesOfStepSize)
    {
        m_kRange.SetMaximumLevelIsAlwaysValid(bNotOnlyMultiplesOfStepSize);
        CheckForValueChange();
    }

    /** Virtual method which gets called whenever the value of the corresponding GUIRange changes.
        This will be done prior to notifying any observers subscribed to this GUIAbstractBar.
        Derived classes can override this method to implement specific behavior in reaction to a change
        of the contained value.

        In particular, it is possible to override the default invalidation-behavior, which will invalidate
        the entire object. If a derived class implements an optimized invalidation algorithm, it must
        return TRUE, indicating to the base-class that it has already taken care of invalidation.
        If you are not familiar with Guiliani's invalidation mechanisms, return FALSE.

        @return True if an implementation of a derived class has implemented its own invalidation algorithm, False otherwise
    */
    virtual eC_Bool OnValueChange()
    {
        return false;
    }

    /** Sets speed for animation.
        This is the delta by which the value will be changed when animating to a value via AnimateToValue()
        @param ruiAnimationSpeed Value delta per animation step
    */
    inline void SetAnimationSpeed(const eC_UInt &ruiAnimationSpeed)  { m_uiAnimationSpeed = ruiAnimationSpeed; }

    /** Returns the speed for animated scrolling
        @return Value delta per animation step
    */
    inline eC_UInt GetAnimationSpeed() const { return m_uiAnimationSpeed; }
    
    /** Sets timer interval in milliseconds for animation.
        @param ruiAnimationInterval Millisecond interval for animation
    */
    inline void SetAnimationInterval(const eC_UInt &ruiAnimationInterval)  { m_uiAnimationInterval = ruiAnimationInterval; }

    /** Returns the timer interval in milliseconds for animation.
        @return Millisecond interval for animation
    */
    inline eC_UInt GetAnimationInterval() const { return m_uiAnimationInterval; }

    /** Animates to the given target value. 
        This is done by increasing/decreasing the current value in steps whose size is given by SetAnimationSpeed().
        The given target value is automatically clipped to the current range and rounded to the current step size.
        @param iTargetValue Value to which to animate.
    */
    void AnimateToValue(const eC_Int iTargetValue);

    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

protected:
    /** CGUIAbstractBar standard constructor.
        @see CGUIObject().
    */
    CGUIAbstractBar();

    /** CGUIAbstractBar constructor.
        @param pParent Pointer to parent object.
        @param vX X position relative to its parent object.
        @param vY Y position relative to its parent object.
        @param vWidth Width of this object.
        @param vHeight Height of this object.
        @param eID Object Identifier of this object
    */
    CGUIAbstractBar(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIAbstractBar constructor.
        @param pParent Pointer to parent object.
        @param kRect Structure containing x- and y-coordinates as well as width and height.
        @param eID Object Identifier of this object
    */
    CGUIAbstractBar(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIAbstractBar class.
        @param kSource Source object to be copied.
    */
    CGUIAbstractBar(const CGUIAbstractBar& kSource);

    /** Assignment operator of the CGUIAbstractBar class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIAbstractBar& operator=(const CGUIAbstractBar& kSource);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    /** Helper function for constructors. */
    void Init();

    /** Copy attributes method of the CGUIAbstractBar class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIAbstractBar& kSource);

    /** Checks if value changed and invalidates and notifies observers in that case. */
    void CheckForValueChange();

    /// The object that handles and saves the derived bar's range.
    CGUIRange m_kRange;

    ///Stores old value to check if something changed and only notifiy/invalidate in that case.
    eC_Int m_iOldValue;

    /// Value delta when animating
    eC_UInt m_uiAnimationSpeed;

    /// Animation interval in milliseconds
    eC_UInt m_uiAnimationInterval;

    /// Target value for animation
    eC_Int m_iTargetValue;         

};
#endif
