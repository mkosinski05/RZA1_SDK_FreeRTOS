/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_LOAD_ANIMATIONS_CMD_H)
#define GUI_LOAD_ANIMATIONS_CMD_H

#include "GUICommand.h"

/** This command loads animations and animation chains including their
    CGUIEasing functions from a file. The loaded objects are all managed by the
    animation handler. Please respect, that the GUIObjects referenced by the loaded
    animations have to be existent. If the connection between the animation
    and the GUIObject is not possible, the animation cannot work.
    All existing animations and animation chains can be deleted before loading
    the new ones if specified.

    If the given file name is an empty string, this command does not try to
    load animations but deletes all already existing if specified anyways.

    @ingroup GUILIANI_COMMANDS
    @brief Loads animations and animation chains from a file and deletes all existing ones.
*/
class CGUILoadAnimationsCmd : public CGUICommand
{
public:
#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /** XML tag used to indicate a target dialog filename to load. */
    static const eC_Char XMLTAG_DESTANIMATIONSFILENAME[];
#endif

    /// Constructs a LoadAnimationsCmd.
    /** @param rkFileName File name (complete path) of the file containing
               the animations.
        @param bDeleteExistingAnimations True if all existing animations and
               animation chains should be deleted.
      */
    CGUILoadAnimationsCmd(const eC_String& rkFileName,
        bool bDeleteExistingAnimations = true);

#if defined(GUILIANI_STREAM_GUI)
    /// Constructor for factory. Do not use this constructor in user code.
    CGUILoadAnimationsCmd();

    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    ~CGUILoadAnimationsCmd();
    virtual void Do();

    /// File name (full path if needed) of the animations to be loaded.
    eC_String m_kFileName;

    /// Used to decide if all animations and animation chains should be deleted before loading.
    bool m_bDeleteExistingAnimations;
};

#endif // GUI_LOAD_ANIMATIONS_CMD_H
