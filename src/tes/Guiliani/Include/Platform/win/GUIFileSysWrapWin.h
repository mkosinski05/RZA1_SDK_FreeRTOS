/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_FILESYSWRAP_WIN__H_
#define GUI_FILESYSWRAP_WIN__H_

#include "GUIFileSysWrap.h"
#include "windows.h"

/// Implementation of CGUIFileSysWrap class for Windows systems.
class CGUIFileSysWrapWin
    : public CGUIFileSysWrap
{
public:

    /** Constructor.
    */
    CGUIFileSysWrapWin();

    /** Destructor.
    */
    ~CGUIFileSysWrapWin(){}

    /** Get the current user's home path. This method tries to obtain the value of the
        environment variable USERPROFILE. If it does not exist, the method tries
        to form the home path by concatenating the environment variables
        HOMEDRIVE and HOMEPATH. If they don't exist, the root path of
        the operating system is returned.
        @return Home path of the current user.
        @remark On PocketPC, this method always returns the root path.
    */
    virtual eC_String GetHomePath() const;

    /** Gets the absolute path for the root directory.
        It returns "C:" for the Windows operating system.
        @return The absolute path for the root directory.
    */
    virtual const eC_String& GetRootPath() const;

    /** Gets the directory separator "\" under Windows system.
        @return The directory separator.
    */
    virtual const eC_String& GetDirSeparator () const;

    /** Sets the application's current working directory to a specified path.
        If the path is not an absolute path, it does not change the current working
        directory and returns False.

        Under Pocket pc 2003, the SetCurrentDirectory is not supported,
        we just return false to indicate that the current path is not changed.
        There is no concept of current working directory under pocket pc 2003.
        @param kPath The path
        @return True if the directory was successfully changed; otherwise False.
    */
    virtual eC_Bool SetCurrentPath (const eC_String& kPath);

    /** Allocate a CGUIDirectoryWin instance using the given path.
        The CGUIDirectory should be deleted after the user finishes using it.
        @param kPath The directory path; directly passed to the CGUIDirectory
               constructor.
        @param kNameFilter The filter string; directly passed to the CGUIDirectory
               constructor.
        @return Pointer to the allocated CGUIDirectory instance.
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath = "", const eC_String& kNameFilter = "") const;

    /** Allocate a CGUIDirectoryWin instance for the given path.
        The instance should be deleted after the user finishes using it.
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
        @return The resulting directory instance
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath,
        const CGUIDirectory::StringList_t& kNameFilter,
        eC_UInt uiFilter = CGUIDirectory::ALL,
        CGUIDirectory::SortFlag_t eSort = CGUIDirectory::NAME,
        eC_Bool bCaseInsensitive = true) const;

    /** Adapt the path with the correct path separator "\" so that it conforms
        to Windows path naming.
        There is no trailing backslash in the returned path.
        @param kOrgPath The original path to be adapted.
        @return The adapted result path.
    */
    virtual eC_String MakePlatformPath(const eC_String& kOrgPath) const;

    /** Gets a list of the drives on this system.
        In pocket pc 2003, we just returns a list contains the root directory.
        @return The list of the drives. It returns a number of
                CGUIDirInfo objects of the path "C:", "D:", and so on
                for Windows systems.
    */
    virtual const CGUIDirInfo::DirInfoPtrList_t& GetDrives();

    /** Help function to check whether the given path string is an absolute
        path or not.
        @param kStr The given path.
        @return True if it is an absolute path; otherwise False.
    */
    virtual eC_Bool IsStrAbsolute(const eC_String& kStr) const;

    /** Check whether the given directory string represents a drive or not.
        @param kDirStr The directory string to be checked.
        @return True if the directory name represents a drive; otherwise False.
    */
    virtual eC_Bool IsDrive(const eC_String& kDirStr) const;

    /** Copy a file. All file data and
        file meta information are copied from the source file to the destination file.

        If the given destination is a directory that differs from the source directory,
        the source file is copied to the destination. If the source and destination directories
        are the same, this method does nothing and returns False.

        If the given destination is a file and it is the same file as the source file, this method
        does nothing and returns False. Otherwise, it checks whether the destination file exists. If the
        destination file does not exist, a new file is created. Otherwise it checks whether the destination
        file can be overwritten. If that is the case, the destination file is overwritten if specified.
        @param kSource The source file name including path. If it is a relative path, the current
                       working directory of the file system wrapper is used to complete the path.
        @param kDest   The destination directory or file. If it is a relative path the current
                       working directory of the file system wrapper is used to complete the path.
        @param bOverwrite Pass true to overwrite the destination file if it already exists.
        @param bCopyEvenIfIdentical True if we want to copy the files even if they already exist and are identical (modification date, modification time and size).
                                    False if we want to skip copying the files which already exists and have the same date,time of modification and size.
        @return True if successful, otherwise False.
    */
    virtual eC_Bool CpFile(eC_String kSource, eC_String kDest, eC_Bool bOverwrite = false, eC_Bool bCopyEvenIfIdentical = true);

    /** Opens the given file from the platform's filesystem.
        This method is useful for getting write-access to the filesystem. If you are looking for plain read-access and
        you wish to support Guiliani's Resource-File and Resource-Header feature, you should use the CGUIResourceFileHander and
        it's Open()-Interface instead.
        @param kPath Full filename (including path) of the file which shall be opened
        @param uiMode Requested access mode (see eC_File for details)
        @return Pointer to newly allocated and opened file. Note that it is in the caller's responsibility to close and free this file!
                You should check the returned file for errors using eC_File's interfaces.
    */
    virtual eC_File* OpenPlatformFile(const eC_String& kPath, eC_Flag uiMode = eC_File::FS_WRITEACCESS | eC_File::FS_READACCESS) const;

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

    /** Get the last time of modification of the file
        @note Not supported on Windows CE, return always -1
        @param kPath Full filename (including path) of the file which is getting the last modification
        @return time and date of the last modification
    */
    virtual time_t GetFileDateTime(const eC_String& kPath) const;

    /** Get the size of the file
        @note Not supported on Windows CE, return always -1
        @param kPath Full filename (including path) of the file which is getting the size
        @return size of the file
    */
    virtual long GetFileSize(const eC_String& kPath) const;

    /** Removes the file specified with the given absolute path name
        @param kPath Name of the file to be deleted
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RemoveFile(const eC_String& kPath) const;


private:
    /** Get an environment variable value for a given variable name.
        @return The environment variable value string.
    */
    eC_String GetEnvStr(LPCTSTR lpEnvName) const;

    /** Get the current working directory and set the m_kCurrentPath to the same.
    */
    void GetCurrentWorkingDir();

    /** Retrieves the file attributes of a given path. Used to shorten the CheckIf...Exists methods.
        @param OUT dwAttribute The file/directory attributes (Windows File Attribute Constants).
        @return True for success, False if no file or dirctory for given path exists.
    */
    eC_Bool GetAttributeHelper(const eC_String& kPath, DWORD &dwAttribute) const;
};

#endif
