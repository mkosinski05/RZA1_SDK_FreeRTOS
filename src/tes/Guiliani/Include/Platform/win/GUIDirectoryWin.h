/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_DIRECTORY_WIN__H_
#define GUI_DIRECTORY_WIN__H_

#include "GUIDirectory.h"

/// Specific implementation of CGUIDirectory for Windows systems.

class CGUIDirectoryWin
    : public CGUIDirectory
{
public:
    /// Only the CGUIFileSysWrapWin can construct an object of this class.
    friend class CGUIFileSysWrapWin;

    /** Destructor.
    */
    ~CGUIDirectoryWin(){}

    /** Check whether the given directory is a root directory or not.
        @return True if the directory is the root directory, otherwise False.
    */
    virtual eC_Bool IsRoot() const;

    /** Removes the file with the given name.
        The list of the dir info must be regenerated.
        @param kFileName The given file name that is to be removed.
        @return True if the file is removed successfully, otherwise False.
    */
    virtual eC_Bool RemoveFile(const eC_String& kFileName);

    /** Renames a file or directory.
        The list of the dir info must be regenerated.
        @param kOldName The old file name that is to be renamed.
        @param kNewName The new file name.
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RenameFile(const eC_String &kOldName, const eC_String & kNewName);

    /** Removes the sub directory specified by the given directory name.
        @param kDirName Name of the directory to be deleted.
        @param bRecursive If true, all sub directories and files are also deleted.
               If false, the directory must be empty for this method to succeed.
        @return True if successful, otherwise False.
    */
    virtual eC_Bool RemoveSubDir(const eC_String& kDirName, eC_Bool bRecursive = false);

    /** Creates a sub-directory in this directory.
        @param kDirName Name of the new directory.
        @return True on success, otherwise False.
    */
    virtual eC_Bool CreateSubDir(const eC_String& kDirName);

    /** Check whether the given directory is readable or not.
        @return True if the directory is readable, otherwise False.
    */
    virtual eC_Bool IsReadable() const;

    /** Check whether this directory does exist in the file system.
        @return True if the directory exists, otherwise False.
    */
    virtual eC_Bool CheckExist() const;

protected:

    /** Create the list that stores the CGUIDirInfo pointers.
    */
    virtual void ListFiles();

private:
    /** Build up a directory with given path and name filter. If the given string is empty,
        the current working directory is used. If the name filter is empty,
        it means including all files.
        @param kPath The given directory path.
        @param kNameFilter The filter string to filter the directory, this is saved in the name filter list.
    */
    CGUIDirectoryWin(const eC_String& kPath = "", const eC_String& kNameFilter = "");

    /**
        Constructs a directory with path, that filters its entries by name filters and
        attributes using uiFilter. It also sorts the names using eSort.
        If path is an empty string,  the current working directory is used.
        If the name filters are empty, that means to list all the files.
        @param kPath The given directory path.
        @param kNameFilter The name filter to filter the files in the directory.
        @param uiFilter The filter attribute to filter the files in the directory,
                        it is a combination of Filter_t values.
        @param eSort The sort flag to sort the files in the directory.
        @param bSortCaseInsensitive Combined with sort flag sort the dir info case-insensitive or not.
    */
    CGUIDirectoryWin(const eC_String &kPath, const StringList_t& kNameFilter,
        eC_UInt uiFilter = CGUIDirectory::ALL, CGUIDirectory::SortFlag_t eSort = CGUIDirectory::NAME,
         eC_Bool bSortCaseInsensitive = true);

    /** Copy constructor.
        @param kDirectory The source to be copied.
    */
    CGUIDirectoryWin(const CGUIDirectoryWin& kDirectory);

    /** Assignment operator.
        @param kDirectory The source to be copied.
        @return The refence of this object.
    */
    CGUIDirectoryWin& operator= (const CGUIDirectoryWin& kDirectory);
};

#endif
