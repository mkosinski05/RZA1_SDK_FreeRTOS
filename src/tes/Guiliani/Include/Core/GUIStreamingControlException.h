/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUISTREAMINGCONTROLEXCEPTION_H__
#define __GUISTREAMINGCONTROLEXCEPTION_H__

#include "GUIStreamingException.h"

class CGUIObject;

/// Indicates an error while streaming a control.
/** In addition to its base class, this exception carries an object pointer.
  */
class CGUIStreamingControlException : public CGUIStreamingException
{
public:

    /** Constructor.
        @param kStreamingFilePath The streaming file path. If unknown at
               construction time, it can be set later (see
               SetStreamingFilePath()).
        @param uiLineNo The line in the streaming file where the error occurred.
        @param kExpectedTag The expected tag that should have been read.
        @param kActualTag The tag that actually was read from the file.
        @param pkObj Pointer to a GUI object.
    */
    CGUIStreamingControlException(
        const eC_String& kStreamingFilePath,
        eC_UInt uiLineNo,
        const eC_String& kExpectedTag="",
        const eC_String& kActualTag="",
        CGUIObject* pkObj = NULL);

    /** Destructor.
    */
    ~CGUIStreamingControlException();

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIStreamingControlException(const CGUIStreamingControlException& kSource);

    /** Constructs a streaming control exception from a base streaming exception.
        @param kSource Source object to be copied.
     */
    CGUIStreamingControlException(const CGUIStreamingException& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIStreamingControlException& operator = (const CGUIStreamingControlException& kSource);

    /** Sets the object pointer.
        @param pkObj Pointer to a GUI object.
      */
    inline void SetGUIObject(CGUIObject* pkObj) { m_pkObject = pkObj; }

    /** Returns the pointer set by SetGUIObject().
        @return Pointer to a GUI object.
      */
    inline CGUIObject* GetGUIObject() const { return m_pkObject; }

private:
    /** The ID of the control that is streamed.*/
    CGUIObject* m_pkObject;
};
#endif //__GUISTREAMINGCONTROLEXCEPTION_H__
