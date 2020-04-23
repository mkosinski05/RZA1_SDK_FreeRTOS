/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIINPUTDISPLAYIT_H_
#define GUIINPUTDISPLAYIT_H_

#include "eC_Types.h"
#include "GUIInputMedia.h"

class CGUIEvent;

/**
    @brief Used to parse the events from Nios Board.
*/
class CGUIInputDisplayIt
    : public CGUIInputMedia
{
public:
    /**
        Singleton for creating an input object and change the pointer in
        base class to input window.
     */
    static void CreateInstance();

    CGUIEvent* GetEvent(eC_UInt uiIdleTime);

protected:
    CGUIInputDisplayIt();

    ~CGUIInputDisplayIt();

    void StopIdle();

    eC_Bool m_bLeftButtonPressed;       ///< Parameter to indicate that the left button is pressed.
    eC_Bool m_bMouseMoved;              ///< Parameter to indicate that the mouse was moved

    eC_Int m_iLBtnPressedMouseX;        ///< Attribute to store former left button pressed mouse-X position
    eC_Int m_iLBtnPressedMouseY;        ///< Attribute to store former left button pressed mouse-Y position

    eC_Bool m_bClicked;                 ///< Attribute to store former button click
    eC_Bool m_bStopIdle;				///< Attribute to store if idle mode is stoped

private:
    /**
        Copy-constructor.
        @param kSource Source object to be copied.
     */
    CGUIInputDisplayIt(const CGUIInputDisplayIt& kSource) {}

    /**
        Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
     */
    CGUIInputDisplayIt& operator=(const CGUIInputDisplayIt& kSource) {return *this;}
};
#endif // GUIINPUTDISPLAYIT_H_
