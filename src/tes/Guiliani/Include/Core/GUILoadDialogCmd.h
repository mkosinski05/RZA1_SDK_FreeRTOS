/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_LOAD_DIALOG_CMD_H)
#define GUI_LOAD_DIALOG_CMD_H

#include "GUICommand.h"
#include "GUIObjectHandleResource.h"

/// Loads a GUI definition from a file.
/** This command loads a GUI definition which results in a new CGUIObject. This
    object is then attached to a specific parent object or the GUI root object.

    This command may also delete an object with a specific ID.

    If the given file name is an empty string, this command does not try to
    load a GUI definition but deletes the specified object anyway.

    If something goes wrong while streaming (loading the GUI definition), this
    command does not delete any GUI objects.

    @ingroup GUILIANI_COMMANDS
  */
class CGUILoadDialogCmd : public CGUICommand
{
public:
#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /** XML tag used to indicate a target dialog filename to load. */
    static const eC_Char XMLTAG_DESTDIALOGFILENAME[];
#endif

    /// Constructs a LoadDialogCmd.
    /** @param rkFileName File name of the dialog to be loaded.
        @param eParent Object ID of the designated parent of the loaded dialog.
               If NO_HANDLE is passed here or no object with this ID is found
               when this command is executed, the GUI root object is used as
               the parent. If the object is found but is no composite object,
               its parent is used as the new dialog's parent instead.
        @param eDelete ID of an object to be deleted when this command is
               executed. If NO_HANDLE is passed or no object with this ID is
               found, nothing gets deleted.
      */
    CGUILoadDialogCmd(const eC_String& rkFileName,
        ObjectHandle_t eParent = NO_HANDLE,
        ObjectHandle_t eDelete = NO_HANDLE);

#if defined(GUILIANI_STREAM_GUI)
    /// Constructor for factory. Do not use this constructor in user code.
    CGUILoadDialogCmd();

    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    ~CGUILoadDialogCmd();
    virtual void Do();

    /// File name of the dialog to be loaded.
    eC_String m_kFileName;

    /// ID of the designated parent object of the new dialog.
    ObjectHandle_t m_eParent;

    /// ID of object to be deleted.
    ObjectHandle_t m_eDelete;

    /// temporary pointer which is set when the dialog is loaded. Use carefully.
    CGUIObject* m_pkLoadedDialog;
};

#endif // GUI_LOAD_DIALOG_CMD_H
