/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_QUIT_CMD_H)
#define GUI_QUIT_CMD_H

#include "GUICommand.h"

/// Quits the Guiliani application by ending the main loop.
/** This command calls CGUI::Quit.

    @ingroup GUILIANI_COMMANDS
  */
class CGUIQuitCmd : public CGUICommand
{
public:
    CGUIQuitCmd();

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    ~CGUIQuitCmd();
    virtual void Do();
};

#endif // GUI_QUIT_CMD_H
