/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIIMAGERESOURCEEXCEPTION_H__
#define __GUIIMAGERESOURCEEXCEPTION_H__

#include "GUIResourceException.h"
#include "GUIImageResource.h"

/** @brief Exception class for handling the image resources
    It will report which image and what file have problem.
*/
class CGUIImageResourceException : public CGUIResourceException
{
public:

    /** Standard-constructor.
        @param kResourcePath The image resource path.
        @param eImageID The image resource id.
    */
    CGUIImageResourceException(const eC_String& kResourcePath,
                               const ImageResource_t eImageID);

    /** Destructor.
    */
    ~CGUIImageResourceException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIImageResourceException(const CGUIImageResourceException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIImageResourceException& operator = (const CGUIImageResourceException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;


 private:
    /** The ID of the image which will be reported.*/
    ImageResource_t m_eImageID;
};
#endif //__GUIIMAGERESOURCEEXCEPTION_H__
