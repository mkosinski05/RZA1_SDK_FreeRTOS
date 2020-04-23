/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIPOINT__H_
#define GUIPOINT__H_

#include "eC_Types.h"

/// CGUIPoint class to hold two values (x, y) like a vector.

class CGUIPoint
{
public:
    CGUIPoint();

    /** Constructor
        @param vX X-value
        @param vY Y-value
    */
    CGUIPoint(const eC_Value &vX, const eC_Value &vY);

    /** Assignment operator
        @param srcPt Where to copy from
        @return Where to assign to
    */
    CGUIPoint& operator=(const CGUIPoint& srcPt);

    /** Copy constructor
        @param kPt Where to copy from
    */
    CGUIPoint(const CGUIPoint &kPt);

    /** Minus operator
        @return Result of the operation
    */
    CGUIPoint& operator-();

#ifdef GUILIANI_STREAM_GUI
    /// Helper method to stream GUIPoint attributes.
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    ///Helper method to write GUIPoint attributes.
    void WriteToStream();
#endif

    eC_Value m_vX; ///< X-Value
    eC_Value m_vY; ///< Y-Value
};
#endif
