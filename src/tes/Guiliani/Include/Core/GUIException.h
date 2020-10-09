/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIEXCEPTION__H_
#define GUIEXCEPTION__H_

#include "eC_String.h"

/// This is the base class for Guiliani exceptions.

class CGUIException
{
public:
    /**
        Standard constructor.
    */
    CGUIException();

    /**
        Destructor.
    */
    virtual ~CGUIException();

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;
};

#endif
