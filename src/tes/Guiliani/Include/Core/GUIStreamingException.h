/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUISTREAMINGEXCEPTION_H__
#define __GUISTREAMINGEXCEPTION_H__

#include "GUIException.h"

/// Base class for all streaming exceptions.
/** Has attributes to store a filename, expected tag, read tag and line number.
*/
class CGUIStreamingException : public CGUIException
{
public:

    /** Constructor.
        @param kStreamingFilePath The streaming file path. If unknown at
               construction time, it can be set later (see
               SetStreamingFilePath()).
        @param uiLineNo The line in the streaming file where the error occurred.
        @param kExpectedTag The expected tag that should have been read.
        @param kActualTag The tag that actually was read from the file.
    */
    CGUIStreamingException(const eC_String& kStreamingFilePath,
                           eC_UInt uiLineNo,
                           const eC_String& kExpectedTag="",
                           const eC_String& kActualTag="");

    /** Destructor.
    */
    ~CGUIStreamingException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIStreamingException(const CGUIStreamingException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIStreamingException& operator = (const CGUIStreamingException& kSource);

    /** Report the message of the exception.
        @return Formatted message of the exception.
    */
    virtual eC_String Report() const;

    /// Set the path which was read while the exception occurred.
    inline void SetStreamingFilePath( const eC_String& kPath)
    {
       m_kStreamingFilePath = kPath;
    }

    /// Set the line number in which the exception occurred.
    inline void SetLineNo(const eC_UInt uiLine)
    {
        m_uiLineNo = uiLine;
    }

protected:
    /** Report the common message about the streaming exception.
        @return The message
    */
    eC_String ReportCommonMessage() const;

private:
    /** The streaming file path.*/
    eC_String m_kStreamingFilePath;
    /// The line number in the streaming file.
    eC_UInt m_uiLineNo;
    /// The expected tag that should have been read from file.
    eC_String m_kExpectedTag;
    /// The actual tag that was read from the file.
    eC_String m_kActualTag;
};
#endif //__GUISTREAMINGEXCEPTION_H__
