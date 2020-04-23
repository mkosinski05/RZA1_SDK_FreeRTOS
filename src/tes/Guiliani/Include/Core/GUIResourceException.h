/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIRESOURCEEXCEPTION_H__
#define __GUIRESOURCEEXCEPTION_H__

#include "GUIException.h"

/** 
    It will report which resource path is given.
    @brief This is the exception class handling resources
*/
class CGUIResourceException : public CGUIException
{
public:

    /** Standard-constructor.
        @param kResourcePath The resource path
    */
    CGUIResourceException(const eC_String& kResourcePath);

    /** Destructor.
    */
    ~CGUIResourceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIResourceException(const CGUIResourceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIResourceException& operator = (const CGUIResourceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

protected:

    /** String to indicate the resource file path.*/
    eC_String m_kResourcePath;

};
#endif //__GUIRESOURCEEXCEPTION_H__
