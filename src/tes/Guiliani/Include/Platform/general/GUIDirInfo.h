/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_DIR_INFO__H_
#define GUI_DIR_INFO__H_

#include "eC_TList_doubleLinked.h"
#include "eC_String.h"


/// Describes a file in the file system.
/** Each object of this class stores the path of a file. It can be changed with ChangeFilePath().
    The absolute path of the file can be obtained
    by the method GetAbsPath_File(). GetAbsPath() gets the path of the file without the
    file name. The corresponding non-absolute paths can be obtained with the methods GetPath_File() and GetPath().
    Whether the given path is an absolute path or not is checked by the method IsAbsPath().
    The path can be changed to be absolute by the method MakePathAbs().
    Use GetFileName() to get the file name. The base name and suffix
    information are obtained by the methods GetFileBaseName() and GetFileSuffix().

    The type of file is checked by the methods IsDir(), IsFile() and IsSymLink(). If the file is a symlink,
    you can get the file that the symlink points to by the method ReadLink(). The group name, group id,
    owner name and owner id are checked by the method GetGroupName(), GetGroupID(), GetOwnerName() and GetOwnerID().

    Each object also provides information about the time when the file was created, latest modified, or accessed by
    the methods GetTimeCreated(), GetTimeLatestModified() and GetTimeLatestRead(). As long as the target platform provides time zones and daylight savings time handling, the time returned is UTC. The access permissions are checked by
    the methods IsExecutable(), IsReadable(), IsWritable(). Use GetSize() to determine the size of the file.
*/

class CGUIDirInfo
{
public:
    /// Type define for the CGUIDirInfo list.
    typedef eC_TListDoubleLinked<CGUIDirInfo*> DirInfoPtrList_t;

    /** Contains a file timestamp. If one of the values is -1, the value is
     *  is not provided by the specific platform.
     *  @brief Timestamp of a file
     */
    typedef struct
    {
        eC_Time64 rawTimeStamp; ///< Raw file time provided by platform wrapper.
        eC_Int iYear; ///< 4 digit year.
        eC_Int iMonth; ///< month [1; 12]
        eC_Int iDay; ///< day of the month [1; 31]
        eC_Int iHour; ///< hour [0; 23]
        eC_Int iMin; ///< minutes [0; 59]
        eC_Int iSec; ///< seconds [0; 59]
        eC_Int iDayOfWeek; ///< day of the week [0; 6], 0 = Sunday
    } GUIFileTime_t;

    /** Default constructor.
        The file path is set to be empty string.
    */
    CGUIDirInfo();

    /** Constructor with the given file path name.
        @param kFileName The path of the given file.
        @param kCurrentWorkingDirPath The path of the current working directory.
    */
    CGUIDirInfo( const eC_String& kFileName , const eC_String& kCurrentWorkingDirPath);

    /** Copy constructor.
        @param kDirInfo The source object to be copied.
    */
    CGUIDirInfo(const CGUIDirInfo& kDirInfo);

    /** Assignment operator.
        @param kDirInfo The source object to be copied.
        @return Reference to this object.
    */
    CGUIDirInfo& operator=(const CGUIDirInfo& kDirInfo);

    /** Change old file path to new one.
        @param kNewFilePath New path to be set.
    */
    void ChangeFilePath(const eC_String& kNewFilePath);

    /** Destructor.
    */
    virtual ~CGUIDirInfo(){}

    /** Get the absolute file path including the path and file name.
        Example: for the construct new CGUIDirInfo("filename.ext", "path/subpath")
                 GetAbsPath_File  will return   path/subpath/filename.ext
        @return The absolute path including the path and file name.
    */
    eC_String GetAbsPath_File() const;

    /** Get the absolute path excluding the file name.
        Example: for the construct new CGUIDirInfo("filename.ext", "path/subpath")
                 GetAbsPath_File  will return   path/subpath
        @return The absolute path excluding the file name.
    */
    eC_String GetAbsPath() const;

    /** Get the file path including the file's name.
        This may be a relative path or absolute path according to
        the path given in constructor or ChangeFilePath.
        @return The file path including the file's name.
    */
    eC_String GetPath_File() const;

    /** Get the file path excluding the file's name.
        This may be a relative path or absolute path according to
        the path given in constructor or ChangeFilePath. If the file path is
        a drive or root path, it is returned.
        @return The file path excluding the file's name.
    */
    eC_String GetPath() const;

    /** Check whether the given file path is an absolute path or not.
        @return True if it is an absolute path, False if it is relative.
    */
    virtual eC_Bool IsAbsPath() const;

    /** Make the path to be absolute if it is a relative path.
        If the path is already absolute, do nothing.
    */
    void MakePathAbs();

    /** Get the file name excluding the path of this file.
        Example:  path/subpath/filename.ext  will become  filename.ext
        @return The file name excluding path.
    */
    eC_String GetFileName() const;

    /** Get the base name of the file name until the last '.'.
        Example:  path/subpath/filename.ext.ext2  will become  filename.ext
        @return The base name of the file name until the last '.'.
    */
    eC_String GetFileBaseName() const;

    /** Get suffix of the file name from the last '.'.
        Example:  path/subpath/filename.ext.ext2  will become  ext2
        If there is no suffix, an empty string is returned.
        @return Suffix of the file name from the last '.'.
    */
    eC_String GetFileSuffix() const;

    /** Get the time when the file was created.
        @return The time (UTC) when the file was created.
    */
    virtual GUIFileTime_t GetTimeCreated() const = 0;

    /** Get the time when the file was modified last time.
        @return The time (UTC) when the file was modified last time.
    */
    virtual GUIFileTime_t GetTimeLatestModified() const  = 0;

    /** Get the time when the file is read last time.
        @return The time (UTC) when the file was read last time.
    */
    virtual GUIFileTime_t GetTimeLatestRead() const  = 0;

    /** Check whether the file does exist in the file system.
        @return True if the file does exist, otherwise False.
    */
    virtual eC_Bool CheckExist() const = 0;

    /** Get the group name of this file.
        Returns an empty string if groups are not supported.
        @return The group name of this file.
    */
    virtual eC_String GetGroupName() const = 0;

    /** Get the group id of this file.
        Returns ID_UNKNOWN if groups are not supported.
        @return The group id of this file.
    */
    virtual eC_UInt GetGroupID() const = 0;

    /** Get the name of the file's owner.
        Returns an empty string if owner names are not supported.
        @return The name of the file's owner.
    */
    virtual eC_String GetOwnerName() const = 0;

    /** Get the ID of the file's owner.
        Returns ID_UNKNOWN if owner IDs are not supported.
        @return the ID of the file's owner.
    */
    virtual eC_UInt GetOwnerID() const = 0;

    /** Check whether the file is a directory.
        @return True if it is a directory, otherwise False.
    */
    virtual eC_Bool IsDir() const = 0;

    /** Check whether the file is a file (not a directory).
        @return True if it is a file, otherwise False.
    */
    virtual eC_Bool IsFile() const = 0;

    /** Check whether the file is a symlink.
        @return True if it is a symlink, otherwise False.
    */
    virtual eC_Bool IsSymLink() const = 0;

    /** Check whether the file is a hidden file.
        @return True if it is a hidden file, otherwise False.
    */
    virtual eC_Bool IsHidden() const = 0;

    /** Check whether the file is a system file.
        @return True if it is a hidden file, otherwise False.
    */
    virtual eC_Bool IsSystem() const = 0;

    /** Check whether the file is executable or not.
        @return True if the file is executable, otherwise False.
    */
    virtual eC_Bool IsExecutable() const = 0;

    /** Check whether the file is readable or not.
        @return True if the file is readable, otherwise False.
    */
    virtual eC_Bool IsReadable() const = 0;

    /** Check whether the file is writable or not.
        @return True if the file is writable, otherwise False.
    */
    virtual eC_Bool IsWritable() const = 0;

    /** Get the name a symlink points to,
        or an empty string if the object isn't a symbolic link.
        @return The file name that the symlink points to.
    */
    virtual eC_String ReadLink() const = 0;

    /** Get the size of the file.
        @return The size of the file.
    */
    virtual long long int GetSize() const = 0;

    /** Check whether the given file is a root directory or not.
        @return True if the directory is a root directory; otherwise False.
    */
    virtual eC_Bool IsRoot() const = 0;

    /// Value used as "unknown" ID
    static const eC_UInt ID_UNKNOWN = static_cast<eC_UInt>(-1);

protected:

    /** Stores the file path.
    */
    eC_String  m_kFilePath;

    /** Stores the path of the current working directory.
    */
    eC_String m_kCurrentWorkingDirPath;

    /// The file time enumeration used in GetTime private method
    /// to identify, which time to get.
    enum FileTime_t {CreatedTime, ModifiedTime, ReadTime};

    /** Get the time according to the given file time type.
        @param eFileTime The file time type, i.e. Created, LatestModifed or LatestRead.
        @return Time obtained at UTC.
    */
    virtual GUIFileTime_t GetTime(FileTime_t eFileTime) const = 0;
};

#endif
