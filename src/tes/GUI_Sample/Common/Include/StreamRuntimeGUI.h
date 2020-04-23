/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(STREAMRUNTIMEGUI_H)
#define STREAMRUNTIMEGUI_H

#include "GUI.h"
#include "GUIRefCntPtr.h"
#include "GUIStreamReader.h"
#include "GUIAutoPtr.h"

/** The NStreamRuntime Namespace contains generic helper classes for initializing a default StreamRuntime application.
    The code herein typically remains widely unchanged across most StreamRuntimes.
    Feel free to remove these classes from your own application and replace them with custom code, if you wish.*/
namespace NStreamRuntime
{

/** StreamRuntime specific CGUI instance. 
    Serves as a base-class for the application implemented by customer. */
class CStreamRuntimeGUI
    :public CGUI
{
public:
    CStreamRuntimeGUI(eC_Value x, eC_Value y, eC_Value width, eC_Value height, ObjectHandle_t eID);
    virtual ~CStreamRuntimeGUI();

    /** Default initialization routine which reads configuration data from the config XML file and
        initializes Guiliani accordingly */
    void Init();

    void StreamGUI();

    /// fills the application background with a rectangle in the color read from the config XML file.
    eC_Bool DoDraw();

private:
    /// Streamreader used by the StreamRuntime during startup. The config file specifies which type of reader to use.
    CGUIAutoPtr<CGUIStreamReader> m_pkStreamReader;
};
}

#endif //#ifndef STREAMRUNTIMEGUI_H
