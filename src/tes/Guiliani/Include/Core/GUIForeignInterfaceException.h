/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIFOREIGNINTERFACEEXCEPTION_H__
#define __GUIFOREIGNINTERFACEEXCEPTION_H__

#include "GUIException.h"

/** This exception can be used to indicate an errors when calling
    the interface of a foreign library.
    @brief Error from a foreign library
*/
class CGUIForeignInterfaceException : public CGUIException
{
public:

    /** Standard-constructor.
        @param kForeignInterfaceName Name of the exception
    */
    CGUIForeignInterfaceException(const eC_String& kForeignInterfaceName);

    /** Destructor.
    */
    ~CGUIForeignInterfaceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIForeignInterfaceException(const CGUIForeignInterfaceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIForeignInterfaceException& operator = (const CGUIForeignInterfaceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

private:
    /** The name of the foreign interface.*/
    eC_String m_kForeignInterfaceName;
};
#endif //__GUIFOREIGNINTERFACEEXCEPTION_H__
