/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/
#ifndef GUIFAKEMOUSECURSOR_H
#define GUIFAKEMOUSECURSOR_H
#include "eC_Types.h"
/// Software mouse cursor
/** The CGUIFakeMouseCursor class emulates a mouse cursor by software.
    To use it, call CreateInstance. If the mouse cursor isn't needed anymore call DeleteInstance.

    The image IMG_STDCTRL_FAKE_MOUSE_CURSOR can be used to supply an image used as mouse cursor.
    Otherwise a built-in cursor will be used.
*/
class CGUIFakeMouseCursor
{
    /// needed for invalidate and DoDraw, which only need to be accessed by CGUI
    friend class CGUI;
public:
    /** Creates the fake mouse cursor
    */
    static void CreateInstance();

    /** Static member function to get the Mouse cursor instance.
        @return Handle to this instance
    */
    static CGUIFakeMouseCursor* GetInstance();

    /// Deletes the fake mouse cursor
    static void DeleteInstance();

protected:
    /// Invalidates the mouse cursor
    void Invalidate();

    /// Draws the mouse cursor
    void DoDraw();

private:
    /// Singleton instance
    static CGUIFakeMouseCursor* s_pkInstance;

    /// X position of mouse pointer
    eC_Value m_vPosX;

    /// Y position of mouse pointer
    eC_Value m_vPosY;

    /// width of rendered cursor
    const eC_UInt m_cSW_CURSOR_WIDTH;

    /// height of rendered cursor
    const eC_UInt m_cSW_CURSOR_HEIGHT;

    /// Standard constructor
    CGUIFakeMouseCursor();

    /// Standard destructor
    ~CGUIFakeMouseCursor();
};
#endif //GUIFAKEMOUSECURSOR_H
