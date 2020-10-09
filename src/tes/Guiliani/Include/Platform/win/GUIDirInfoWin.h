/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_DIR_INFO_WIN__H_
#define GUI_DIR_INFO_WIN__H_

#include "GUIDirInfo.h"

/// Implementation of CGUIDirInfo for Windows systems.

class CGUIDirInfoWin:
    public CGUIDirInfo
{
public:

    /// Only the CGUIFileSysWrapWin and CGUIDirectoryWin can construct an object of this class.
    friend class CGUIFileSysWrapWin;

    /// Only the CGUIFileSysWrapWin and CGUIDirectoryWin can construct an object of this class.
    friend class CGUIDirectoryWin;

    /** Destructor.
    */
    ~CGUIDirInfoWin(){}

    /** Check whether the file is executable or not.
        @return True if the file is executable, otherwise False.
        @remark May return incorrect values on PocketPC.
    */
    virtual eC_Bool IsExecutable() const;

    /** Get the time when the file was created.
        @return The time when the file was created.
    */
    virtual GUIFileTime_t GetTimeCreated() const;

    /** Get the time when the file was modified last time.
        @return The time when the file was modified last time.
    */
    virtual GUIFileTime_t GetTimeLatestModified() const;

    /** Get the time when the file was read last time.
        @return The time when the file was read last time.
    */
    virtual GUIFileTime_t GetTimeLatestRead() const;

    /** Get the group name of this file.
        Unsupported for Windows.
        @return Empty string.
    */
    virtual eC_String GetGroupName() const;

    /** Get the group id of this file.
        Unsupported for Windows.
        @return ID_UNKNOWN.
    */
    virtual eC_UInt GetGroupID() const;

    /** Get the owner name of the file.
        Unsupported for Windows.
        @return Empty string.
    */
    virtual eC_String GetOwnerName() const;

    /** Get the owner id of the file.
        Unsupported for Windows.
        @return ID_UNKNOWN.
    */
    virtual eC_UInt GetOwnerID() const;

    /** Check whether the file is a directory.
        @return True if it is a directory, otherwise False.
    */
    virtual eC_Bool IsDir() const;

    /** Check whether the file is a file (not a directory).
        @return True if it is a file, otherwise False.
    */
    virtual eC_Bool IsFile() const;

    /** Check whether the file is a symlink.
        @return True if it is a symlink, otherwise False.
    */
    virtual eC_Bool IsSymLink() const;

    /** Check whether the file is a hidden file.
        @return True if it is a hidden file, otherwise False.
    */
    virtual eC_Bool IsHidden() const;

    /** Check whether the file is a system file.
        @return True if it is a system file, otherwise False.
    */
    virtual eC_Bool IsSystem() const;

    /** Get the name a symlink (shortcut on Windows) points to,
        or an empty string if the object isn't a symbolic link.
        @return The file name that the symlink points to.
        @remark On PocketPC 2003, this method always returns an empty string.
    */
    virtual eC_String ReadLink() const;

    /** Get the size of the file.
        @return The size of the file.
    */
    virtual long long int GetSize() const;

    /** Check whether the given file is a root directory or not.
        @return True if the directory is the root directory; otherwise False.
    */
    virtual eC_Bool IsRoot() const;

    /** Check whether the file does exist in the file system.
        @return True if the file does exist, otherwise False.
    */
    virtual eC_Bool CheckExist() const;

    /** Check whether the file is readable or not.
        @return True if the file is readable, otherwise False.
    */
    virtual eC_Bool IsReadable() const;

    /** Check whether the file is writable or not.
        @return True if the file is readable, otherwise False.
    */
    virtual eC_Bool IsWritable() const;

protected:

    /** Get the time according to the given file time type.
        @param eFileTime The file time type, i.e. Created, LatestModifed or LatestRead.
        @return Time obtained.
    */
    virtual GUIFileTime_t GetTime(FileTime_t eFileTime) const;

private:
    /** Default constructor.
    */
    CGUIDirInfoWin();

    /** Constrcutor with the given file path name.
        @param kFileName The path of the given file.
        @param kCurrentWorkingDirPath The path of the current working directory.
    */
    CGUIDirInfoWin(const eC_String& kFileName, const eC_String& kCurrentWorkingDirPath);

    /** Copy constructor.
    */
    CGUIDirInfoWin(const CGUIDirInfoWin& kDirInfo);

    /** Assignment operator.
    */
    CGUIDirInfoWin& operator=(const CGUIDirInfoWin& kDirInfo);
};

#endif
