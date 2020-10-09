/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_ILLEGAL_CHILD_EXCEPTION_H)
#define GUI_ILLEGAL_CHILD_EXCEPTION_H

#include "GUIException.h"

/// Indicates wrong child types for a specific parent (composite) object.
class CGUIIllegalChildException : public CGUIException
{
};

#endif // GUI_ILLEGAL_CHILD_EXCEPTION_H
