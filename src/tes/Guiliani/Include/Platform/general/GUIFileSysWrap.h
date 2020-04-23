/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_FILE_SYS_WRAP_H__
#define GUI_FILE_SYS_WRAP_H__

#include <time.h>
#include "GUIDirInfo.h"
#include "GUIDirectory.h"
#include "eC_File.h"

/// Returns the singleton instance of the file system wrapper.
#define GETFILESYS CGUIFileSysWrap::GetInstance()

/// The CGUIFileSysWrap class provides an abstract way of accessing directories and files.
/** It is an abstract class. There are implementations for different operating systems.
    Use the GetInstance() static member function to obtain the platform-specific instance.

    A starting point for working with directories is the method DirAlloc() which creates a CGUIDirectory
    instance. From this instance it is possible to access the contents of a directory. The instance
    must be deleted if it is not used anymore.

    This class provides the possibility to get the general paths of the application and operating system.
    The GetCurrentPath() method gets the working directory of the current process.
    The home path is obtained by calling GetHomePath(). It is possible to get a list of drives of the
    operating system by calling the GetDrives() method. A method called IsDrive checks whether the given string
    stands for a drive or not.

    Since the directory separators can differ between various operating systems,
    you can call GetDirSeparator() to get the directory separator for the operating system that Guiliani is
    currently running on.
    All paths used in the CGUIDirectory, CGUIDirInfo, CGUIFileSysWrap and their derived classes can be adapted
    to conform to the current operating system
    by calling the MakePlatformPath() method of this class.

    There is a helper function called SeparateString() to split a string into two parts at a given separator.
    This method is used in separating the path and file name, or base name and suffix.
*/

class CGUIFileSysWrap
{
public:
    // Guiliani supports two different separators. Backslash (\) and Slash (/). The constants defined here enables consistent access out of FileSysWrappers.
    static const eC_String ms_DirSepWin; ///< Directory separator used for windows file system
    static const eC_String ms_DirSepPOSIX;  ///< Directory seperator used for *NIX file system

    /** Static member function to get the platform-specific instance of the file system wrapper.
        This method is not implemented in this base class but in the platform-specific specializations.
        @return Handle to this instance
    */
    static CGUIFileSysWrap& GetInstance();

    /** Constructor.
    */
    CGUIFileSysWrap(){}

    /// Destructor, frees the drives list.
    virtual ~CGUIFileSysWrap();

    /** Returns the absolute path name of the current working directory.

        @note Under Windows CE there is no concept of a "current working directory".
        Only the directory where the application was started from can be returned.
        @return The absolute path of the application's current directory.
    */
    inline eC_String GetCurrentPath() const {return m_kCurrentPath;}

    /** Gets a list of the root directories on this system.
        @return The list of the drives.
    */
    virtual const CGUIDirInfo::DirInfoPtrList_t& GetDrives() = 0;

    /** Get the current user's home path.
        @return User's home path.
    */
    virtual eC_String GetHomePath() const = 0;

    /** Gets the absolute path for the root directory.
        @return The absolute path for the root directory.
    */
    virtual const eC_String& GetRootPath() const = 0;

    /** Returns the windows directory separator.
        @return An eC_String containing a backslash.
    */
    const eC_String& GetWinDirSeparator() const {return ms_DirSepWin;}

    /** Returns the *NIX directory separator.
        @return An eC_String containing a slash.
    */
    const eC_String& GetPOSIXDirSeparator() const {return ms_DirSepPOSIX;}

    /** Gets the directory separator.
        @return The directory separator.
    */
    virtual const eC_String &GetDirSeparator() const = 0;

    /** Sets the application's current working directory to path.
        If the path is not an absolute path, it does not change the current working
        directory and returns False.
        @param kPath The path to set
        @return True if the directory was successfully changed; otherwise False.
    */
    virtual eC_Bool SetCurrentPath(const eC_String& kPath) = 0;

    /** Allocate a CGUIDirectory instance using the given path.
        The instance should be deleted after the user finishes using it.
        @param kPath The directory path; directly passed to the CGUIDirectory
               constructor.
        @param kNameFilter The filter string; directly passed to the CGUIDirectory
               constructor.
        @return Pointer to the allocated CGUIDirectory instance.
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath = "", const eC_String& kNameFilter = "") const = 0;

    /** Allocate a CGUIDirectory instance by giving the path.
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
        @return Pointer to the allocated CGUIDirectory instance.
    */
    virtual CGUIDirectory* DirAlloc(const eC_String &kPath, const CGUIDirectory::StringList_t& kNameFilter,
         eC_UInt uiFilter = CGUIDirectory::ALL,
         CGUIDirectory::SortFlag_t eSort = CGUIDirectory::NAME,
         eC_Bool bCaseInsensitive = true) const = 0;

    /** Help function used to split a string at a given separator. The result
        strings do not contain the separator. If the bFromLeft is true, it
        checks the first appearance of the separator from the beginning (left)
        of the string. If it is false, it checks the first appearance
        of the separator from end (right) of the string.
        If the separator character is not found, the kLeftStr result string takes
        the original one, and kRightStr is left empty.
        @param kOrgStr The original string that needs to be analyzed.
        @param kLeftStr The left part of the result string.
        @param kRightStr The right part of the result string.
        @param kSeparator The separator character stored as a string.
        @param bFromLeft Indicate whether the first appearance of the separator is
               searched from left or right (beginning or end) of kOrgStr.
    */
    void SeparateString(const eC_String& kOrgStr, eC_String& kLeftStr,
        eC_String &kRightStr, const eC_String& kSeparator, eC_Bool bFromLeft) const;

    /** Adjust the path with the correct path separator according to the operating system.
        After the path is adjusted, the returned path does not contain a trailing directory
        separator except when it is a root path (i.e. "/" or "C:\").
        @param kOrgPath The original path to be adjusted.
        @return The resulting platform-conforming path.
    */
    virtual eC_String MakePlatformPath(const eC_String& kOrgPath) const = 0;

    /** Adjust the path with the correct path separator according to the operating system
        and remove all occurrences of "..". (i.e. "C:\projects\x\..\y\project" becomes
        "C:\projects\y\project").
        After the path is adjusted, the returned path does not contain a trailing directory
        separator except when it is a root path (i.e. "/" or "C:\").
        @param kOrgPath The original path to be adjusted.
        @param kResultPath The resulting path.
        @return True if successful, False otherwise.
    */
    virtual eC_Bool MakeNormalizedPlatformPath(const eC_String& kOrgPath, eC_String& kResultPath) const;

    /** Converts a relative path to an absolute path using the current directory.
        @param kRelPath relative path
        @param kAbsPath absolute path as return value
        @return true, if successful, false otherwise
    */
    eC_Bool MakeAbsolutePath(const eC_String& kRelPath, eC_String& kAbsPath) const;

    /** Create a relative path from one directory to another directory.
        Example:
           From:    "C:\projects\GSE\my_streamruntime\bin"
           To:      "C:\projects\TestPlatform\testdata"
           Result:  "..\..\..\TestPlatform\testdata"
        The returned path does not contain a trailing directory separator.
        @param kFromPath The directory the result path will be relative to.
        @param kToPath The destination directory.
        @param kResultPath The resulting path.
        @return True if successful, False otherwise (i.e. if "From" and "To" path are on different drives).
    */
    virtual eC_Bool CreateRelativePath(const eC_String& kFromPath, const eC_String& kToPath, eC_String& kResultPath) const;

    /** Adds a dir separator at the end of the given path, if there isn't already one.
        @param kPath Path to adjust.
        @return The resulting string
    */
    virtual eC_String MakeDirectoryPath(const eC_String& kPath) const;

    /** Help function to check whether the given path string is an absolute
        path or not.
        @param kStr The given path.
        @return True, if it is an absolute path; False otherwise.
    */
    virtual eC_Bool IsStrAbsolute(const eC_String& kStr) const = 0;

    /** Frees all internally allocated resources. Do not call this method from
        user code, it is called by Guiliani when shutting down.
    */
    void DeInit();

    /** Check whether the given directory string represents a drive or not.
        @param kDirStr The directory string to be checked.
        @return True if the directory name represents a drive; otherwise False.
    */
    virtual eC_Bool IsDrive(const eC_String& kDirStr) const = 0;

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
    virtual eC_Bool CpFile(eC_String kSource, eC_String kDest, eC_Bool bOverwrite = false, eC_Bool bCopyEvenIfIdentical = true) = 0;

    /// Extracts the path from a file name that includes a path.
    /** @param rkFileName The full file name to be examined.
        @return The path without the file name.
        @remarks This method only performs string operations. It does not check
                 for existence of paths or files.
      */
    eC_String GetPath(const eC_String& rkFileName) const;

    /// Extracts the file name from a file name that includes a path.
    /** Example:  path/subpath/filename.ext  will become  filename.ext
        @param rkFileName The full file name to be examined.
        @return The file name without the path.
        @remarks This method only performs string operations. It does not check
                 for existence of paths or files.
      */
    eC_String GetFileName(const eC_String& rkFileName) const;

    /// Extracts the base name of a file from a name that includes a path.
    /** Example:  path/subpath/filename.ext  will become  filename
        @param rkFileName The full file name to be examined.
        @return The file's base name without path and extension (extension begins after the filename's last dot)
        @remarks This method only performs string operations. It does not check
                 for existence of paths or files.
      */
    eC_String GetFileBaseName(const eC_String& rkFileName) const;

    /// Extracts the extension of a file name.
    /** Example:  path/subpath/filename.ext.ext2  will become  ext2
        @param rkFileName The full file name to be examined.
        @return The extension without path and file name.
        @remarks This method only performs string operations. It does not check
                 for existence of paths or files.
      */
    eC_String GetFileSuffix(const eC_String& rkFileName) const;

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
    virtual eC_Bool CheckIfFileOrDirectoryExists(const eC_String& kPath) const = 0;

    /** Checks if a file at a given path exists.
        @param kPath Path to a file for which the existence should be checked.
        @return True If the file specified via the path exists or
                False if it is not a file (for example a directory) or a symlink.
    */
    virtual eC_Bool CheckIfFileExists(const eC_String& kPath) const = 0;

    /** Checks if a directory at given path exists.
        @param kPath Path to directory for which the existence should be checked.
        @return True If the directory specified via the path exists or
                False if its not a directory (for example file).
    */
    virtual eC_Bool CheckIfDirectoryExists(const eC_String& kPath) const = 0;

    /** Get the last time of modification of the file
        @param kPath Full filename (including path) of the file which is getting the last modification
        @return time and date of the last modification
    */
    virtual time_t GetFileDateTime(const eC_String& kPath) const;

    /** Get the size of the file
        @param kPath Full filename (including path) of the file which is getting the size
        @return size of the file
    */
    virtual long GetFileSize(const eC_String& kPath) const;

    /** Removes the file specified with the given absolute path name
        @param kPath Name of the file to be deleted
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RemoveFile(const eC_String& kPath) const = 0;


protected:

    /** The current application path.
    */
    eC_String m_kCurrentPath;

    /** The list of drives in the system. It is initialized
        every time the user calls GetDrives.
    */
    CGUIDirInfo::DirInfoPtrList_t m_kDrivesList;

private:
    /** Counts the number of directory separators in the passed path.
        @param kPath The path to be counted as a null-terminated UTF8 string.
        @return Number of directory separators found or -1 in case of error.
    */
    eC_Int CountNumberOfSeparators(const eC_UTF8* pPath) const;

};

#endif
