/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_DIRECTORY__H_
#define GUI_DIRECTORY__H_

#include "GUIDirInfo.h"

/// Allows accessing a directory, listing its contents and manipulate the contained files and directories.

/** An instance of a CGUIDirectory can be obtained by calling CGUIFileSysWrap::DirAlloc.
    It is possible to specify name filters, filter attributes and sort mechanism when creating
    a CGUIDirectory instance.
    The name filters, filter attributes and sort flags can be retrieved by the calling corresponding
    getter methods.

    The absolute path and name of this directory can be determined with the method GetAbsPath()
    and GetDirName() methods. You can also check the directory's attributes to see whether it
    is readable, is a root directory and if it does exist at all in the file system. MakePathAbs()
    provides the possibility to make the path of the directory absolute.

    For accessing the contents of a directory, call the method GetEntryInfoList(). A list of only the file
    names without additional information can be also obtained by calling the method GetEntryList().
    Whether the special directory entries "." and ".." is determined by a flag that can be set with
    SetListSpecialFileFlag(). A single CGUIDirInfo pointer can be obtained by calling the method
    GetDirInfo(). It is possible to create sub directory, remove a file or subdirectory and
    rename files from this directory with the methods CreateSubDir(), RemoveFile(), RemoveSubDir()
    and RenameFile().
*/

class CGUIDirectory
{
public:

    /// A list of strings.
    typedef eC_TListDoubleLinked<eC_String> StringList_t;

    /** Filters for directory contents.
        They are meant to be used as bit masks for bitwise-OR when creating a
        filter for the SetFilter() method.
        To check a single filter attribute for a directory, call GetFilterAttr() and
        bitwise-AND the single attribute with the result.
    */
    enum Filter_t
    {
        DIRECTORIES = 0x001, ///< List the directories.
        FILES = 0x002, ///< List the files.
        NOSYMLINK = 0x004, ///< Do not include symlinks.
        READABLE = 0x008, ///< List the files that are readable.
        WRITABLE = 0x010, ///< List the files that are writable.
        EXECUTABLE = 0x020, ///< List files for which the application has execute access.
        HIDDEN = 0x040,  ///< List the hidden files.
        SYSTEM = 0x080,  ///< List the system files.
        SYMLINK = 0x100, ///< List the symlinks.
        ALL = 0x200 ///< List all the files and directories.
    };

    /** The sort flag type that is specified to sort the
        CGUIDirInfo objects in the list.
    */
    enum SortFlag_t
    {
        NAME = 0x00, ///< Sort by name.
        TIME = 0x01, ///< Sort by modification time.
        SIZE = 0x02, ///< Sort by file size.
        TYPE = 0x04  ///< Sort by file type.
    };

    /** Builds up a directory with given path and name filter. If the given path is empty,
        the current working directory is used. If the name filter is empty,
        it means including all files.
        @param kPath The given directory path.
        @param kNameFilter The filter string to filter the directory, this is saved in the name filter list.
    */
    CGUIDirectory(const eC_String& kPath = "", const eC_String& kNameFilter = "");

    /**
        Constructs a directory with path, that filters its entries by name filters and
        attributes using uiFilter. It also sorts the names using eSort.
        If path is an empty string,  the current working directory is used.
        If the name filters are empty, that means to list all the files.
        @param kPath The given directory path.
        @param kNameFilter The name filter to filter the files in the directory.
        @param uiFilter The filter attribute to filter the files in the directory,
                        it is a combination of the Filter_t elements.
        @param eSort The sort flag to sort the files in the directory.
        @param bSortCaseInsensitive Combined with sort flag sort the dir info case-insensitive or not.
    */
    CGUIDirectory(const eC_String &kPath, const StringList_t& kNameFilter,
         eC_UInt uiFilter = ALL, SortFlag_t eSort = NAME,
         eC_Bool bSortCaseInsensitive = true);

    /** Copy constructor.
        @param kDirectory The source to be copied.
    */
    CGUIDirectory(const CGUIDirectory& kDirectory);

    /** Assignment operator.
        @param kDirectory The source to be copied.
        @return The refence of this object.
    */
    CGUIDirectory& operator= (const CGUIDirectory& kDirectory);

    /** Destructor.
    */
    virtual ~CGUIDirectory();

    /** Gets the absolute path of this directory.
        @return Absolute path of this directory.
    */
    eC_String GetAbsPath() const;

    /** Changes the current directory to the given directory.
        If the given directory does not exist or is not readable,
        this method does nothing and returns False.

        @note This method does not work relatively to the current path of this
        directory object. Instead, the given directory name is run through
        MakePlatformPath() and used as the new directory here.

        @param kDirName The directory name that the current directory switches to.
        @return True if the new directory exists and is readable; otherwise False.
    */
    eC_Bool ChangeDirectory(const eC_String& kDirName);

    /** Changes the current directory by moving one directory up from the current directory.
        If it is already the root node, it will not change anymore and return False.
        If the given directory does not exist and is not readable, this method does nothing and returns False.
        @return True if the new directory exists and is readable; otherwise returns False.
    */
    eC_Bool ChangeToParentDir();

    /** This method forces the refresh of the directory to recreate the directory info list.
    */
    void RefreshList();

    /** Counts the total number of directories and files in this directory after
        the directory is filtered and sorted.
        @return The total number of directories and files in this directory.
    */
    inline eC_UInt CountFiles() {ListFiles(); return m_kDirInfoList.GetQuantity();}

    /** Gets the name of the directory without the path.
        For instance if the current directory is '/usr/bin', the returned is 'bin'.
        If the directory has no name, because for instance it is the root
        directory, an empty string is returned.
        @return The directory name without the path.
    */
    eC_String GetDirName() const;

    /** Gets the list of names of this directory's entries
        according to the name filters, filter attributes and sort flag
        stored in the object.
        @return The string list of the entries in the directory.
    */
    StringList_t GetEntryList ();

    /** Gets the list of directory infos related to the fields in the directory
        according to the name filters, filter attributes and sort flag stored
        in the object.
        @return The CGUIDirInfo list of the files in the directory.
    */
    inline const CGUIDirInfo::DirInfoPtrList_t& GetEntryInfoList ()
    {
        ListFiles();
        return m_kDirInfoList;
    }

    /** Gets a list of file name excluding leading path
        for the files in the directories according to the name filters,
        filter attributes and sort flag stored in the object.
        @return The string list of the files in the directory.
    */
    StringList_t GetEntryFileNameList ();

    /** Checks whether this directory exists in the file system.
        @return True if the directory exists, otherwise False.
    */
    virtual eC_Bool CheckExist() const = 0;

    /** Gets the filter attribute for filtering this directory.
        This result is a bitwise-ORed integer using the values of Filter_t.
        @return The filter attribute.
        @see Filter_t.
    */
    eC_UInt GetFilterAttr() const
    {
        return m_uiFilter;
    }

    /** Checks whether this directory's path name is an absolute path or not.
        @return True if the directory's path is absolute, otherwise False.
    */
    virtual eC_Bool IsAbsPath() const;

    /** Checks whether the given directory is readable or not.
        @return True if the directory is readable, otherwise False.
    */
    virtual eC_Bool IsReadable() const = 0;

    /** Checks whether the given directory is a root directory or not.
        @return True if the directory is the root directory, otherwise False.
    */
    virtual eC_Bool IsRoot() const = 0;

    /** Converts the directory path to an absolute path. If it is already absolute
        nothing happens.
    */
    void MakePathAbs();

    /** Creates a sub-directory in this directory.
        @param kDirName The given directory name.
        @return True on success; otherwise False.
    */
    virtual eC_Bool CreateSubDir(const eC_String& kDirName) = 0;

    /** Gets the name filter string list.
        @return The name filter string list.
    */
    inline StringList_t GetNameFilters() const
    {
        return m_kNameFilters;
    }

    /** Sets one name filter string to filter the directory. The name filter
        list then contains only this string.
        @param kNameFilter The given name filter string.
    */
    void SetNameFilters(const eC_String& kNameFilter);

    /** Sets the name filter string list.
        @param kNameFilter The given name filter string list.
    */
    void SetNameFilters(const StringList_t& kNameFilter);

    /** Removes the file with the given name.
        The list of the dir info must then be regenerated.
        @param kFileName The given file name that is to be removed.
        @return True if the file is removed successfully; otherwise False.
    */
    virtual eC_Bool RemoveFile(const eC_String& kFileName) = 0;

    /** Renames a file or directory.
        The list of the dir info must be regenerated.
        @param kOldName Name of the file that is to be renamed.
        @param kNewName The new file name.
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RenameFile(const eC_String &kOldName, const eC_String & kNewName) = 0;

    /** Removes the sub directory specified with the given directory name.

        To delete the sub directory or its contents, the current user needs write permissions.
        @param kDirName Name of the directory to be deleted.
        @param bRecursive If true, all sub directories and files are deleted. If false, the
               directory must be empty for this method to succeed.
        @return True if successful; otherwise False.
    */
    virtual eC_Bool RemoveSubDir(const eC_String& kDirName, eC_Bool bRecursive = false) = 0;

    /** Sets the filter attribute that is used to filter the files in the directory.
        @param uiFilter The given filter attribute. Use bitwise-OR-ed values of
               Filter_t to create this value.
        @see Filter_t.
    */
    void SetFilter (eC_UInt uiFilter);

    /** Sets the sort flag to sort the files in the directory.
        @param eSort Sort criterion to be applied when sorting the entry list.
    */
    void SetSortFlag (SortFlag_t eSort);

    /** Gets the currently set sort criterion.
        @return The sort flag.
    */
    inline SortFlag_t GetSortFlag () const
    {
        return m_eSortFlag;
    }

    /** Sets the sort case-insensitive flag to indicate whether to sort
        the dir info list with case-insensitive or not.
        @param bSortCaseInsensitive The case-insensitive flag.
    */
    void SetSortCaseInSensitive(eC_Bool bSortCaseInsensitive);

    /** Gets the sort case-insensitive flag.
        @return The case-insensitive flag.
    */
    inline eC_Bool GetSortCaseInSensitive() const
    {
        return m_bSortCaseInsensitive;
    }

    /** Reverts the order of dir info list.
    */
    void RevertSortList();

    /** Gets the CGUIDirInfo pointer for a given file path.
        If no file or directory with that name can be found, this method returns NULL.
        !!! Attention performance critical!!!. An internal directory listing will be generated.
        If the additional functionality of GUIDirInfo is not needed please use for example
        GETFILESYS.CheckIfFileOrDirectoryExists or similar.
        @param kFilePath The file path that corresponds to the CGUIDirInfo pointer to be found. Attention: This path is case-sensitive!
        @return The CGUIDirInfo pointer, or NULL no file or sub directory with the given name exists.
    */
    const CGUIDirInfo* GetDirInfo(const eC_String& kFilePath);

    /** Sets the list special file flag to indicate whether the special directory
        "." and ".." should be included in the CGUIDirInfo list or not.
        @param bListSpecialFile Pass true to enable listing of special entries.
    */
    inline void SetListSpecialFileFlag(eC_Bool bListSpecialFile) {m_bListSpecialFile = bListSpecialFile;}

protected:

    /** Creates the list of CGUIDirInfo pointers.
    */
    virtual void ListFiles() = 0;

    /** Helper function for sorting the directory list or file list.
        @param kList The list to sort
    */
    void SortSubList(CGUIDirInfo::DirInfoPtrList_t& kList);

    /** Helper function for sorting the dir info list.
        It calls SortSubList and reorders the list.
    */
    void SortDirInfoList();

    /** Frees the list of CGUIDirInfo, which is allocated in this class.
    */
    void FreeDirInfoList();

    /** Helper function for filtering the list. Specializations of this class use
        this method as a helper for ListFiles.
     */
    void FilterList();

    /** Stores this directory's path.
    */
    eC_String m_kDirPath;

    /** The name filters.
    */
    StringList_t m_kNameFilters;

    /** The filter attribute for this directory.
        Combination of the Filter_t flags.
    */
    eC_UInt m_uiFilter;

    /** The sort flag for this directory.
    */
    SortFlag_t m_eSortFlag;

    /** Combined with sort flag to sort the dir info list case-insensitive or not.
    */
    eC_Bool m_bSortCaseInsensitive;

    /** Indicate in current directory sorting order, if the directories
        are placed in front of the files or not.
    */
    eC_Bool m_bSortDirsFirst;

    /** The list stores the CGUIDirInfo object pointers,
        this is a combination of the directories list and
        files list.
    */
    CGUIDirInfo::DirInfoPtrList_t m_kDirInfoList;

    /** The list stores only the directories.
    */
    CGUIDirInfo::DirInfoPtrList_t m_kDirectoriesList;

    /** The list stores only the files (excluding the directories).
    */
    CGUIDirInfo::DirInfoPtrList_t m_kFilesList;

    /** Indicates whether the entry list must be recreated or not
        inside the ListFiles method. Normally, before the user tries to
        get information about a file in the directory, ListFiles function
        must be called. This function checks this parameter to see if
        the list should be regenerated or not.
    */
    eC_Bool m_bListNeedRecreation;

    /** Indicates whether the special file "." and ".." should be included in the
        directory list or not. By default, they will not be included in the list.
    */
    eC_Bool m_bListSpecialFile;

    /** Stores the current working directory path.
    */
    eC_String m_kCurrentWorkingDir;

private:
    /** Converts the original string to upper case.
        If the string is a unicode string, it is not converted
        and the original string is returned.
        @param kOrgStr The original string
        @return The result string that is already converted.
    */
    eC_String UpperString(const eC_String& kOrgStr) const;

    /** Using the quick sort algorithm to sort the given array from smaller to larger
        and the given DirInfo-List is also sorted.
        @param kArray The array to be sorted.
        @param iFirst The first index of array for sorting.
        @param iLast The last index of array for sorting.
        @param kList The dir info list that is to be sorted.
    */
    template<class T>
    void QuickSort(eC_TArray<T>& kArray, eC_Int iFirst, eC_Int iLast, CGUIDirInfo::DirInfoPtrList_t& kList);

};

#endif
