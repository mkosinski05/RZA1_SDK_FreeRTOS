/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_VALIDATOR_H)
#define GUI_VALIDATOR_H

#include "eC_String.h"

/// Interface for validators that provide checking and changing of user input.

/** Specializations have to implement the method IsValid() to return a boolean
    value that indicates whether the input was valid. In addition, the method
    MakeValid() should be implemented to change an input text in a way that
    makes it valid.

    Validators are meant to be used with input fields (see CGUIBaseInputField).
  */
class CGUIValidator
{
public:
    /// Default constructor.
    CGUIValidator() {}

    /// Destructor.
    virtual ~CGUIValidator() {}

    /** Checks a value for validity.
        @param rValue The value to be checked.
        @return True if the value is accepted by the validator, otherwise False.
      */
    virtual eC_Bool IsValid(const eC_String& rValue) const = 0;

    /** Tries to make a value valid.
        @param[in,out] rValue The value to be checked and made valid, if
                       possible. If the implementation is not able to correct
                       the value or if it is already valid, the value can also
                       be left unchanged.
      */
    virtual void MakeValid(eC_String& rValue) const = 0;
};

#endif // GUI_VALIDATOR_H
