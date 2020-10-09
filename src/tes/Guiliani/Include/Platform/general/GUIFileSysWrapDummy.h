/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_FILESYSWRAP_DUMMY__H_
#define GUI_FILESYSWRAP_DUMMY__H_

#include "GUIFileSysWrap.h"

/// Dummy implementation of CGUIFileSysWrap.

/** This class is used for systems
    that have no file system. Its methods return default values.
  */
class CGUIFileSysWrapDummy
    : public CGUIFileSysWrap
{
public:

    /** Constructor.
    */
    CGUIFileSysWrapDummy();

    /** Destructor.
    */
    ~CGUIFileSysWrapDummy(){}

    /** Returns the saved list in the file system class that contains no items inside.
        @return The saved list in the file system class that contains no items inside.
    */
    virtual const CGUIDirInfo::DirInfoPtrList_t& GetDrives();

    /** Get the current user's home path. It returns empty string.
        @return Empty string as default value.
    */
    virtual eC_String GetHomePath() const;

    /** Gets the absolute path for the root directory.
        It returns empty string.
        @return Empty string as default value.
    */
    virtual const eC_String& GetRootPath() const;

    /** Gets the directory separator.
        @return Empty string as default value.
    */
    virtual const eC_String& GetDirSeparator () const;

    /** Sets the application's current working directory to a specified path.
        It does nothing and just return true.
        @param kPath Ignored.
        @return True.
    */
    virtual eC_Bool SetCurrentPath (const eC_String& kPath);

    /** Return a NULL pointer.
        @param kPath The directory path; directly passed to the CGUIDirectory
               constructor.
        @param kNameFilter The filter string; directly passed to the CGUIDirectory
               constructor.
        @return NULL pointer.
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath = "", const eC_String& kNameFilter = "") const;

    /** Returns a NULL pointer.
        @param kPath The kPath for the directory that is passed as a parameter
               of the CGUIDirectory constructor.
        @param kNameFilter The name filters to filter the directory that is passed as a parameter
               of the CGUIDirectory constructor.
        @param uiFilter The filter attribute to filter the directory that is passed as a parameter
               of the CGUIDirectory constructor.
        @param eSort The sort kind to sort the directory that is passed as a parameter
               of the CGUIDirectory constructor.
        @param bCaseInsensitive The bool value to indicate whether sort the elements in the directory case-insensitive
               or not. This is also a parameter of the CGUIDirectory constructor.
        @return NULL pointer.
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath,
        const CGUIDirectory::StringList_t& kNameFilter,
        eC_UInt uiFilter = CGUIDirectory::ALL,
        CGUIDirectory::SortFlag_t eSort = CGUIDirectory::NAME,
        eC_Bool bCaseInsensitive = true) const;

    /** Returns an empty string.
        @param kOrgPath The original path to be adapted.
        @return The original path.
    */
    virtual eC_String MakePlatformPath(const eC_String& kOrgPath) const;

    /** Return false per default.
        @param kStr The given path.
        @return False.
    */
    virtual eC_Bool IsStrAbsolute(const eC_String& kStr) const;

    /** Return false per default.
        @param kDirStr The directory string to be checked.
        @return False.
    */
    virtual eC_Bool IsDrive(const eC_String& kDirStr) const;

    /** Dummy implementation of the copy file functionality.
        @param kSource The source file name including path. If it is a relative path, the current
                       working directory of the file system wrapper is used to complete the path.
        @param kDest   The destination directory or file. If it is a relative path the current
                       working directory of the file system wrapper is used to complete the path.
        @param bOverwrite Pass true to overwrite the destination file if it already exists.
        @param bCopyEvenIfIdentical True if we want to copy the files even if they already exist and are identical (modification date, modification time and size).
                                    False if we want to skip copying the files which already exists and have the same date,time of modification and size.
        @return always False.
    */
    virtual eC_Bool CpFile(eC_String kSource, eC_String kDest, eC_Bool bOverwrite = false, eC_Bool bCopyEvenIfIdentical = true);

    /** Checks if a given path exists. The object specified might be a file or directory.
        @param kPath Path to a file or directory for which the existence should be checked.
        @return True If the object specified via the path exists otherwise False.
    */
    virtual eC_Bool CheckIfFileOrDirectoryExists(const eC_String& kPath) const;

    /** Checks if a file at a given path exists.
        @param kPath Path to a file for which the existence should be checked.
        @return True If the file specified via the path exists or
                False if it is not a file (for example a directory) or a symlink.
    */
    virtual eC_Bool CheckIfFileExists(const eC_String& kPath) const;

    /** Checks if a directory at given path exists.
        @param kPath Path to directory for which the existence should be checked.
        @return True If the directory specified via the path exists or
                False if its not a directory (for example file).
    */
    virtual eC_Bool CheckIfDirectoryExists(const eC_String& kPath) const;

    /** Removes the file specified with the given absolute path name
        @param kPath Name of the file to be deleted
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RemoveFile(const eC_String& kPath) const;
};

#endif
