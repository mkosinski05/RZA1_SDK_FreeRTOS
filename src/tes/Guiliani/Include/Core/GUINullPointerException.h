/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUINULLPOINTEREXCEPTION_H__
#define __GUINULLPOINTEREXCEPTION_H__

#include "GUIException.h"

/** @brief This exception can be used to indicate the errors if a pointer is null.
*/
class CGUINullPointerException : public CGUIException
{
public:

    /** Standard-constructor.
        @param kNullPointerName Name of the exception
    */
    CGUINullPointerException(const eC_String& kNullPointerName);

    /** Destructor.
    */
    ~CGUINullPointerException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUINullPointerException(const CGUINullPointerException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUINullPointerException& operator = (const CGUINullPointerException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

private:
    /** The name of the null pointer name.*/
    eC_String m_kNullPointerName;
};
#endif //__GUINULLPOINTEREXCEPTION_H__
