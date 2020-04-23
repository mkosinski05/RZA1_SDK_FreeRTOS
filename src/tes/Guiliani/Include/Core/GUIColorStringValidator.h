/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_COLORSTRING_VALIDATOR_H)
#define GUI_COLORSTRING_VALIDATOR_H

#include "GUIValidator.h"

/// A CGUIValidator for a color hex string in the form 0xAARRGGBB.
class CGUIColorStringValidator : public CGUIValidator
{
public:
    /** Constructor.
      */
    CGUIColorStringValidator();

    /**
       Check whether a given string represents valid hex color value.
       @param rValue String to check for a valid color string.
       @return True if the passed string is a valid hex color string in the format '0xAARRGGBB', else False.
      */
    virtual eC_Bool IsValid(const eC_String& rValue) const;

    /** The passed value is converted to a color hex string. If it is out of range, it
        is set to '0xFFFFFFFF'.
        @param [in,out] rValue The value to be tested and changed if necessary.
      */
    virtual void MakeValid(eC_String& rValue) const;


protected:
};

#endif // GUI_COLORSTRING_VALIDATOR_H
