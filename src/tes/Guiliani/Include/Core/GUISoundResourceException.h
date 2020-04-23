/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUISOUNDRESOURCEEXCEPTION_H__
#define __GUISOUNDRESOURCEEXCEPTION_H__

#include "GUIResourceException.h"
#include "GUISoundResource.h"

/** 
    It will report which sound and what file have problem.
    @brief This is an exception class for handling the sound resources
*/
class CGUISoundResourceException : public CGUIResourceException
{
public:

    /** Standard-constructor.
        @param kResourcePath The font resource path.
        @param eSndID The sound resource id.
    */
    CGUISoundResourceException(const eC_String& kResourcePath,
                               const SoundResource_t eSndID);

    /** Destructor.
    */
    ~CGUISoundResourceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUISoundResourceException(const CGUISoundResourceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUISoundResourceException& operator = (const CGUISoundResourceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

 private:
    /** The ID of the sound which will be reported.*/
    SoundResource_t m_eSndID;

};
#endif //__GUISOUNDRESOURCEEXCEPTION_H__
