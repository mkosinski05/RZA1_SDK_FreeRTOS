/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUICRITICALINSTANCEEXCEPTION_H__
#define __GUICRITICALINSTANCEEXCEPTION_H__

#include "GUIException.h"

/** This is the exception class used to indicate a critical problem about
    the instance creation.
    @brief Critical instance creation error
*/
class CGUICriticalInstanceException : public CGUIException
{
public:

    /** Standard-constructor.
        @param kCriticalInstanceName Name of the exception
    */
    CGUICriticalInstanceException(const eC_String& kCriticalInstanceName);

    /** Destructor.
    */
    ~CGUICriticalInstanceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUICriticalInstanceException(const CGUICriticalInstanceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUICriticalInstanceException& operator = (const CGUICriticalInstanceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

private:
    /** The name of the critical instance.*/
    eC_String m_kCriticalInstanceName;
};
#endif //__GUICRITICALINSTANCEEXCEPTION_H__
