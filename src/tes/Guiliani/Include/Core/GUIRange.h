/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRANGE__H_
#define GUIRANGE__H_

#include "GUIStreamableObject.h"
#include "eC_Math.h"


/// Helper class to supply a platform independent range implementation.

/**

    A range is an interval of integer values which is bound by a minimum
    and a maximum value. The current value inside the range may only be
    set to valid values. Invalid values will be rounded to valid values.

    The base for the level calculations may be set so that the level either
    grows from the maximum to the minimum value or vice versa. When the current
    value is set via SetValue() the passed argument is interpreted as an
    absolute value and the corresponding level is calculated. Now when the
    base is changed this value will be adjusted in order to represent the
    same level as before.

    All set functions adjust the passed arguments if they are invalid and
    update all dependent member variables.

    The level and range value will always be rounded to the next valid step
    as determined by the base and step size automatically.

    The recommended call sequence is this:
    -# SetBase()
    -# SetMaximumLevelIsAlwaysValid()
    -# SetMinValue()
    -# SetMaxValue()
    -# SetStepSize()
    -# SetValue()

    Example:
@code

    // Create a range.
    CGUIRange *pRange = CGUIRange(0,0,100,1,CGUIRange::BASE_AT_MINIMUM,false);

    // Sets the current value of the range to a new absolute value.
    pRange->SetValue((eC_Int)90);

    // Sets the percent value of the level in the range of 0.0 to 1.0.
    pRange->SetLevel(0.8);

    // Sets the percentage value of the level in the range of 0 to 100.
    pRange->SetLevelPercentage(80);

    // Sets the style base at maximum value.
    pRange->SetBase(CGUIRange::BASE_AT_MAXIMUM);

@endcode
*/
class CGUIRange : public CGUIStreamableObject
{
public:

    /// Whether base is at minimum or maximum of the range
    enum Base_t
    {
        BASE_AT_MINIMUM,
        BASE_AT_MAXIMUM
    };

    /** Standard constructor. Initializes all variables to sensible values.
    */
    CGUIRange()
    :
        m_vPercentValue(eC_FromInt(0)),
        m_iValue(0),
        m_uiStepSize(1),
        m_iMaxValue(1),
        m_iMinValue(0),
        m_eBase(BASE_AT_MINIMUM),
        m_bMaximumLevelIsAlwaysValid(false)
    {}

    /** CGUIRange constructor.
        @param iValue The current value of the range.
        @param iMinValue The minimum value of iValue.
        @param iMaxValue The maximum value of iValue.
        @param uiStepSize The step size in units of the range.
        @param eBase The base of the level and the step size.
        @param bExtremeLevelsAreAlwaysValid True, if the maximum is also a valid current value, no matter what the step size is. False, else.
    */
    CGUIRange(
        eC_Int iValue,
        eC_Int iMinValue,
        eC_Int iMaxValue,
        eC_UInt uiStepSize = 1,
        Base_t eBase = BASE_AT_MINIMUM,
        eC_Bool bExtremeLevelsAreAlwaysValid = false)
    :
        m_vPercentValue(eC_FromInt(0)),
        m_iValue(0),
        m_uiStepSize(1),
        m_iMaxValue(1),
        m_iMinValue(0),
        m_eBase(BASE_AT_MINIMUM),
        m_bMaximumLevelIsAlwaysValid(bExtremeLevelsAreAlwaysValid)
    {
        SetMinValue(iMinValue);
        SetMaxValue(iMaxValue);
        SetStepSize(uiStepSize);
        SetBase(eBase);
        SetValue(iValue);
    }

    /** CGUIRange constructor.
        @param iLevelPercentage The percentage level [0..100] to be converted to the current value.
        @param uiStepSizeLevelPercentage The step size given as a percentage level [0..100].
        @param iMinValue The minimum value of iValue.
        @param iMaxValue The maximum value of iValue.
        @param eBase The base of the level and the step size.
        @param bExtremeLevelsAreAlwaysValid True, if the maximum is also a valid current value, no matter what the step size is. False, else.
    */
    CGUIRange(
        eC_Int iLevelPercentage,
        eC_UInt uiStepSizeLevelPercentage,
        eC_Int iMinValue,
        eC_Int iMaxValue,
        Base_t eBase = BASE_AT_MINIMUM,
        eC_Bool bExtremeLevelsAreAlwaysValid = false)
    :
        m_vPercentValue(eC_FromInt(0)),
        m_iValue(0),
        m_uiStepSize(1),
        m_iMaxValue(1),
        m_iMinValue(0),
        m_eBase(BASE_AT_MINIMUM),
        m_bMaximumLevelIsAlwaysValid(bExtremeLevelsAreAlwaysValid)
    {
        SetMinValue(iMinValue);
        SetMaxValue(iMaxValue);
        SetStepSizeLevelPercentage(uiStepSizeLevelPercentage);
        SetBase(eBase);
        SetLevelPercentage(iLevelPercentage);
    }

    /** CGUIRange constructor.
        @param vLevel The percent level [0.0 .. 1.0] to be converted to the current value.
        @param iMinValue The minimum value of iValue.
        @param iMaxValue The maximum value of iValue.
        @param vStepSizeLevel The step size given as a percent value [0.0 .. 1.0].
        @param eBase The base of the level and the step size.
        @param bExtremeLevelsAreAlwaysValid True, if the maximum is also a valid current value, no matter what the step size is. False, else.
    */
    CGUIRange(
        eC_Value vLevel,
        eC_Int iMinValue,
        eC_Int iMaxValue,
        eC_Value vStepSizeLevel,
        Base_t eBase = BASE_AT_MINIMUM,
        eC_Bool bExtremeLevelsAreAlwaysValid = false)
    :
        m_vPercentValue(eC_FromInt(0)),
        m_iValue(0),
        m_uiStepSize(1),
        m_iMaxValue(1),
        m_iMinValue(0),
        m_eBase(BASE_AT_MINIMUM),
        m_bMaximumLevelIsAlwaysValid(bExtremeLevelsAreAlwaysValid)
    {
        SetMinValue(iMinValue);
        SetMaxValue(iMaxValue);
        SetStepSizeLevel(vStepSizeLevel);
        SetBase(eBase);
        SetLevel(vLevel);
    }

    /** Copy-constructor of the CGUIRange class.
        @param kSource Source object to be copied.
    */
    CGUIRange(const CGUIRange& kSource) {CopyAttributes(kSource);}

    /** Operator= method of the CGUIRange class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIRange& operator=(const CGUIRange& kSource)
    {
        CopyAttributes(kSource);
        return *this;
    }

    /**
        Sets the current value of the range to a new absolute value. The
        value will be rounded according to the step size and base in use
        and will be clipped to the bounds as set by SetMinValue and
        SetMaxValue.
        This value should not be higher than half of eC_MAXVALUE or lower than
        half of -eC_MAXVALUE.
        @see SetStepSize, SetBase, SetMaximumLevelIsAlwaysValid,
        @see SetMinValue, SetMaxValue
        @param iNewValue The new value
    */
    void SetValue(eC_Int iNewValue);

    /**
        Gets the current value of the range.
        @see SetValue
        @return The value
    */
    inline eC_Int GetValue() const {return m_iValue;}

    /** Returns the cached percent value of the level in the range of 0.0 to
        1.0.
        @return The percent value corresponding to m_iValue. The
            return value has been rounded to the machine accuracy of eC_Value
            therefore it may always be inaccurate. Conversion from decimal
            floating point numbers to binary floating point numbers may also
            introduce inaccuracies, e.g. 0.1 decimal cannot be saved as an exact
            binary IEEE 754 floating point number as it has an infinite number
            of digits to the right of the dot when saved as a binary floating
            point number.
    */
    inline eC_Value GetLevel() const {return m_vPercentValue;}

    /** Returns the percentage value of the level in the range of 0 to
        100.
        @return The integer percentage value corresponding to m_iValue.
            The intermediate value of the calculation (m_vPercentValue) may
            be inacurate and the final result is rounded to an integer value
            therefore the return value may always be slightly inaccurate.
        @see GetLevel
    */
    inline eC_UInt GetLevelPercentage() const
    {
        return eC_ToInt(eC_Mul(m_vPercentValue, eC_FromInt(100)));
    }

    /** Sets the percent value of the level in the range of 0.0 to 1.0.
        @param vPercent Percent value that determines m_iValue.
            Invalid values will be rounded to the next valid value as
            determined by the step size of the algorithm.
        @see SetValue
    */
    void SetLevel(eC_Value vPercent);

    /** Sets the percentage value of the level in the range of 0 to 100.
        This value will be saved as m_iValue and thus rounded
        to the corresponding integer value of the range.
        @see SetLevel
        @param uiPercentage The percentage
    */
    void SetLevelPercentage(eC_UInt uiPercentage);

    /// @return The minimum value
    inline eC_Int GetMinValue() const {return m_iMinValue;}

    /** SetMinValue() sets the minimum value. This value corresponds
        to the controls internal level of 0%.
        If the new minimum is bigger than the set maximum,
        the minimum is set to maximum, thus minimum and maximum are equal.
        The current value will also be adjusted so that it is inside the bounds.
        This value should not be higher than half of eC_MAXVALUE or lower than
        half of -eC_MAXVALUE.
        @see SetValue
        @param iMinValue The minimum value
    */
    void SetMinValue(eC_Int iMinValue)
    {
        SetRange( iMinValue, m_iMaxValue);
    }

    /// @return The maximum value
    inline eC_Int GetMaxValue() const {return m_iMaxValue;}

    /** SetMaxValue() sets the maximum value. This value corresponds
        to the controls internal level of 100%.
        If the new maximum is smaller than the set minimum,
        the maximum is set to minimum, thus maximum and minimum are equal.
        The current value will also be adjusted so that it is inside the
        bounds.
        This value should not be higher than half of eC_MAXVALUE or lower than
        half of -eC_MAXVALUE.
        @see SetValue
        @param iMaxValue The maximum value
    */
    void SetMaxValue(eC_Int iMaxValue)
    {
        SetRange( m_iMinValue, iMaxValue);
    }

    /** Sets the minimum and maximum allowed values of this range.
        The current value will also be adjusted so that it is inside the
        bounds.
        @param iMinValue New minimum allowed value in the range
        @param iMaxValue New maximum allowed value in the range
        @remark If iMinValue is greater than iMaxValue, the values are swapped
                so that the range is valid.
    */
    void SetRange(eC_Int iMinValue, eC_Int iMaxValue);

    /** The step size uiPercentage will be converted to the next match of the
        step size for range values so that the level may be Increase()d and
        Decrease()d in these steps.
        @note The minimum and maximum range values should be set before calling
        this function.
        @param uiPercentage The step size in the interval [0..100] by that the
            percentage level value is altered approximately. This value is
            interpreted as a percentage value, that is, the internal level.
        @see SetStepSize
    */
    void SetStepSizeLevelPercentage(eC_UInt uiPercentage);

    /** The step size vPercent will be converted to the best match of the step
        size for range values so that the level may be Increase()d and
        Decrease()d in these steps.
        @param vPercent The step size in the interval [0.0 ... 1.0] by that the
            percentage level value is altered approximately. This value is
            interpreted as a percentage value, that is, the internal level.
        @see SetStepSize
    */
    void SetStepSizeLevel(eC_Value vPercent);

    /// @return Step size @see SetStepSize
    inline eC_UInt GetStepSize() const {return m_uiStepSize;}

    /** The level m_iValue may be Increase()d and Decrease()d in steps as
        set by the following function. The step size must be at least one
        and at most as big as the range, if not it will be clipped to these
        bounds.
        @param uiIntValues The distance between the valid values of the range.
            These absolute values are also called steps.
        @see SetBase, SetValue, SetLevel
    */
    void SetStepSize(eC_UInt uiIntValues);

    /** @see CGUIBehaviour::Increase
        @return True if within range, False otherwise
    */
    eC_Bool Increase();

    /** @see CGUIBehaviour::Decrease
        @return True if within range, False otherwise
    **/
    eC_Bool Decrease();

    /// @return Whether base is at minimum or maximum of the range
    inline const Base_t& GetBase() const {return m_eBase;}

    /** Determines if the base is at the lowest or highest value.
        SetBase() divides the range into steps top down or bottom up.
        In derived classes this setting might be used to determine if a control
        fills from left to right or bottom to top or vice versa.
        SetValue() always works with absolute values so that when passing
        the minimum value to it, it always sets the corresponding member
        variable to the valid minimum. GetValue() will return this absolute
        value which may correspond to 0% or 100% level. 
        When changing the base from one border to the other, then m_iValue will
        be adjusted to the next valid step in direction to the new base point.
        @see SetStepSize
        @param eBase Whether base is at minimum or maximum of the range
    */
    void SetBase(Base_t eBase);

    /** @see m_bMaximumLevelIsAlwaysValid
        @return True if maximum level is always valid
    */
    inline eC_Bool GetMaximumLevelIsAlwaysValid() const
    {
        return m_bMaximumLevelIsAlwaysValid;
    }

    /** @see m_bMaximumLevelIsAlwaysValid
        @param bNotOnlyMultiplesOfStepSize True for "always valid"
    */
    inline void SetMaximumLevelIsAlwaysValid(
        eC_Bool bNotOnlyMultiplesOfStepSize)
    {
        m_bMaximumLevelIsAlwaysValid = bNotOnlyMultiplesOfStepSize;
    }

    /** Clips passed value to set min and max values
        @param iValue the value to clip
    */
    inline void ClipToBounds(eC_Int &iValue) const
    {
        if (iValue < m_iMinValue ) iValue = m_iMinValue;
        else if (iValue > m_iMaxValue ) iValue = m_iMaxValue;
    }

    /** Rounds the given value to the next valid value with regard to the current stepsize.
        Rounding will be done in direction to the base.
        @param iRangeValue Value to round
    */
    void RoundToNextStep(eC_Int &iRangeValue) const;

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    /** Copy attributes method of the CGUIRange class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    inline void CopyAttributes(const CGUIRange& kSource)
    {
        m_vPercentValue = kSource.m_vPercentValue;
        m_iValue = kSource.m_iValue;
        m_uiStepSize = kSource.m_uiStepSize;
        m_iMaxValue = kSource.m_iMaxValue;
        m_iMinValue = kSource.m_iMinValue;
        m_eBase = kSource.m_eBase;
        m_bMaximumLevelIsAlwaysValid = kSource.m_bMaximumLevelIsAlwaysValid;
    }

    /// Updates the internal level that is corresponding to the current value.
    void CalcLevel();

    /// Forces the argument into the valid interval.
    inline void ClipPercentage(eC_UInt &uiPercentage) const
    {
        if (uiPercentage > 100) uiPercentage = 100;
    }

    /// Forces the argument into the valid interval.
    inline void ClipLevel(eC_Value &vPercent) const
    {
        if (vPercent > eC_FromInt(1)) vPercent = eC_FromInt(1);
        else if (vPercent < eC_FromInt(0)) vPercent = eC_FromInt(0);
    }

    /** Rounds up a value to multiples of the specified step size.
        @param iValueToRound Value to round.
        @param iStepSize Step size to be used to round. Should never be 0.
        @return Rounded value or given value if step size equals 0.
    */
    inline eC_Int RoundUpToMultiplesOf(eC_Int iValueToRound, eC_Int iStepSize) const
    {
        if (iStepSize != 0)
            // round up to next multiple of step size, steps based at zero
            return ((iValueToRound + (iStepSize - 1)) / iStepSize)
                * iStepSize;
        else
            return iValueToRound;
    }

    /** Rounds down a value to multiples of the specified step size.
        @param iValueToRound Value to round.
        @param iStepSize Step size to be used to round. Should never be 0.
        @return Rounded value or given value if step size equals 0.
    */
    inline eC_Int RoundDownToMultiplesOf(eC_Int iValueToRound, eC_Int iStepSize) const
    {
        if (iStepSize != 0)
            // round down to next multiple of step size, steps based at zero
            return (iValueToRound / iStepSize) * iStepSize;
        else
            return iValueToRound;
    }

    /// Cached value of current percent level value as determined by
    /// m_iValue.
    eC_Value m_vPercentValue;

    /// Current range value, always multiple(s) of step size away from base
    /// point.
    eC_Int m_iValue;

    /// Determines which values are valid for m_iValue.
    /// Generally speaking, all values with a distance of multiples of the step
    /// size away from the base point are valid.
    /// If GetMaximumLevelIsAlwaysValid() is true then additionally
    /// m_iMaxValue or m_iMinValue is a valid value for
    /// m_iValue even if it is no multiple of the step size away from
    /// the base point.
    eC_UInt m_uiStepSize;

    /// highest possible value of m_iValue
    eC_Int m_iMaxValue;

    /// lowest possible value of m_iValue
    eC_Int m_iMinValue;

    /// A range's base defines whether the range starts at the minimum or maximum position
    Base_t m_eBase;

    /// As an exception the highest value of the range may be a valid
    /// value even if it is no multiple of the step size, if true.
    eC_Bool m_bMaximumLevelIsAlwaysValid;
};
#endif
