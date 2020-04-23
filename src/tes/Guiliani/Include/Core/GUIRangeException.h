/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIRANGEEXCEPTION_H__
#define __GUIRANGEEXCEPTION_H__

#include "GUIException.h"

/** @brief The exception is used to indicate the range errors.
*/
class CGUIRangeException : public CGUIException
{
public:

    /** Constructor.
        @param vMax The maximum value of the range.
        @param vMin The minimum value of the range.
    */
    CGUIRangeException(const eC_Value& vMax = eC_MAXVALUE, const eC_Value& vMin = eC_MINVALUE);

    /** Destructor.
    */
    ~CGUIRangeException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIRangeException(const CGUIRangeException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIRangeException& operator = (const CGUIRangeException& kSource);

    /**
        Report the message of the exception.
        @return The message of the exception.
    */
    virtual eC_String Report() const;

private:
    /** The maximum value of the range.*/
    eC_Value m_vMax;
    /** The minimum value of the range.*/
    eC_Value m_vMin;
};
#endif //__GUIRANGEEXCEPTION_H__
