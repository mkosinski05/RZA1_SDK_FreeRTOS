/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIFONTRESOURCEEXCEPTION_H__
#define __GUIFONTRESOURCEEXCEPTION_H__

#include "GUIResourceException.h"
#include "GUIFontResource.h"

class eC_String;

/** This is an exception class for handling the font resources.
    It will report which font and what file have problem.
    @brief Font handling error
*/
class CGUIFontResourceException : public CGUIResourceException
{
public:

    /** Standard-constructor.
        @param kResourcePath The font resource path.
        @param eFntID The font resource id.
    */
    CGUIFontResourceException(const eC_String& kResourcePath,
                              const FontResource_t eFntID);

    /** Destructor.
    */
    ~CGUIFontResourceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIFontResourceException(const CGUIFontResourceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIFontResourceException& operator = (const CGUIFontResourceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

 private:
    /** The ID of the font which will be reported.*/
    FontResource_t m_eFntID;
};
#endif //__GUIFONTRESOURCEEXCEPTION_H__
