/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_INT_VALIDATOR_H)
#define GUI_INT_VALIDATOR_H

#include "GUIValidator.h"
#include <limits.h>

/// A CGUIValidator for an integer range.
class CGUIIntValidator : public CGUIValidator
{
public:
    /** Constructor.
        @param iMin The valid minimum value.
        @param iMax The valid maximum value.
      */
    CGUIIntValidator(eC_Int iMin = INT_MIN, eC_Int iMax = INT_MAX);

    /**
       Check whether a given string represents an integer in the range this
       object validates.
       @param rValue String to check for a valid integer number.
       @return True if the passed string can be converted to an integer value
        in the range that has been specified in the constructor, false
        otherwise.
      */
    virtual eC_Bool IsValid(const eC_String& rValue) const;

    /** The passed value is converted to an integer. If it is out of range, it
        is set to the nearest valid value.
        @param [in,out] rValue The value to be tested and changed if necessary.
      */
    virtual void MakeValid(eC_String& rValue) const;

    /// Get minimum allowed value
    eC_Int GetMinimum() const { return m_iMin; }

    /// Get maximum allowed value
    eC_Int GetMaximum() const { return m_iMax; }

protected:
    /// The valid minimum value.
    eC_Int m_iMin;

    /// The valid maximum value.
    eC_Int m_iMax;
};

#endif // GUI_INT_VALIDATOR_H
