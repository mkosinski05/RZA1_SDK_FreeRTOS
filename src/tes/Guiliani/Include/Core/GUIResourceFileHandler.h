/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRESOURCEFILE_HANDLER__H_
#define GUIRESOURCEFILE_HANDLER__H_

#include "eC_Types.h"
#include "eC_TList_doubleLinked.h"
#include "eC_String.h"

class eC_File;

/// Retrieve pointer to resource handler instance
#define GETRESHANDLER       CGUIResourceFileHandler::GetInstance()

/// Class for file handling in Guiliani.

/**
This class abstracts file access within Guiliani.
Essentially you have four choices on how to store your application's resources:
-# As independent files on a file-system
-# As one big resource-file archive
-# Compiled into the application's executable (as a header)
-# Flashed to some fixed address on the target board

The GUIResourceFileHandler makes the handling of these different modes transparent to the
application. In other words, the application can simply open a given file (read only) without needing to know
whether it exists as a standalone file on the file system, whether it resides inside a resource-archive,
or whether it was compiled directly into the executable.

Additionally, it is also possible to define the order in which the GUIResourceFileHandler will be searching for
a resource. Suppose for example that you have all your resources in one big resource-file archive, but wish to
exchange one single png-image. In this case you can simply copy the png-image onto the file system, and then call
the SetFileOpenOrder interface with the parameter set to RESOURCE_FILE_LAST. This effectively tells the
GUIResourceFileHandler to search for resources on disk first, and only afterwards search for them inside a
resource-file archive or resource-header. Searching for files on disk first and after in resource-files is the default behavior.

The methods SetDiskBuffering and SetHeaderBuffering can be used to set a buffering strategy when loading files from
disk or from a statically linked resource. This can be helpful when the resourcefile-header is located in slow flash and
you want to optimise access speed. By default, buffering for opening files from disk is true and from resource-headers is
false, to prevent unnecessary memory usage.
It's possible to override the previously set buffering strategy by calling the Open-method with a BufferingStrategy_t parameter.

For creating resource-files or resource-headers please use the GUIResourceCompiler(see @ref sec_CGUIResourceCompiler) tool that comes with Guiliani.
Please note that ResourceFiles will _always_ be in big endian. They need to have one common endianess across all
possible targets to ensure exchangability of resource files between platforms.

@section sec_cguiresourcefilehandler_example Usage example

The following example code demonstrates how to use resource-files and resource-headers in your application.
You will typically place these calls somewhere within your startup code:
@code
    // Use of unified resource file:
    GUILOG(GUI_TRACE_DEBUG,"RESOURCE FILE ACTIVE!\n");
    GETRESHANDLER.SetResourceFile("Resources.dat");
@endcode

@code
    // Use resources which were compiled directly into the executable
    GUILOG(GUI_TRACE_DEBUG,"RESOURCE HEADER ACTIVE!\n");
    GETRESHANDLER.SetResourceData(GUIResourceData);
@endcode

@code
    // Use resources which are flashed to some fixed address (in this example 0xDEADBEEF)
    GUILOG(GUI_TRACE_DEBUG,"RESOURCE HEADER ACTIVE IN FLASH!\n");
    GETRESHANDLER.SetResourceData( 0xDEADBEEF );
@endcode

@section sec_cguiresourcefilehandler_usingmemoryresources Using resources directly from memory

In some applications the user has a memory location where a device, e.g. a camera, places image data which should be used within Guiliani.
To achieve this the functions AddMemoryResource and RemoveMemoryResource can be used. This method of memory-usage does not interfere with
using Resource-Files or Resource-Headers as shown previously, instead it enhances the existing possibilities within Guiliani.

AddMemoryResource takes a unique identifier-string, the size of the resource and the address of the memory where the data resides and adds it
to an internal structure so Guiliani can handle it. Subsequent calls to AddMemoryResource with the same identifier replaces the resource previously loaded with this ID.
RemoveMemoryResource removes the memory-resource with the given identifier.

Both functions do NOT allocate or free any memory related to the specified memory-location. So all memory-management has to be done by the user.

Example:
@code
    // somewhere a memory buffer was allocated and an image with 12345 bytes length was loaded
    AddMemoryResource("DataFromCamera", 12345, MemoryLocationOfCameraData);

    // bind an image-ID for use in Guilani to the memory-resource
    ImageResource_t ImageIDofCameraData = GETRESMANAGER.RegisterDynamicImage("DataFromCamera");

    // use the memory-resource for an image in the GUI
    CGUIImage* image = new CGUIImage(parent, 0, 0, ImageIDofCameraData);
@endcode

@section sec_cguiresourcefilehandler_resourcepathprefix The ResourcePathPrefix

In most cases you will have your resources in some dedicated subfolder, or in a location relative to your
executable file. To avoid having Pathnames hard-coded in all your filenames you should use the SetResourcePathPrefix
interface. The prefix you supply there will then be added to the beginning of your filenames during all "file open"-calls.
This also comes in handy if you are working on a system which requires all file operations to be using absolute paths.
@code
    // Setting a path prefix, which points to the location where resources are stored
    GETRESHANDLER.SetResourcePathPrefix("./Resources/");
@endcode

@section sec_CGUIResourceCompiler CGUIResourceCompiler

Guiliani Resource Compiler is a tool which allows you to create in one file all the information that will be needed for your application.

@subsection sec_WhyToUseIt Why use one file instead of the exact files that you need?

Some devices don't have a file system included. Therefore, you just need one file to load in the memory device instead of all the files.

@subsection sec_HowToUseIt How to use it?

This is just an example to show how to use this tool.

As you can see, the next picture below shows the different options:

@image html GUIResourceCompiler.png

In this case we will choose the second option, "add paths from the input file". Then
you need to write the file where you have the paths of all files that you need. For example, "FilesNeeded.txt". Keep in mind that there can be only one path per line in the input file.


@image html GUIResourceCompilerHowToUseIt_12.png

Then you write the name of the input file:

@image html GUIResourceCompilerHowToUseIt_1.png

If everything is good the tool will tell you that the files were added.

@image html GUIResourceCompilerHowToUseIt_2.png

To see the files you have already added you can use the option "show file list" and it will show the files like the next picture shows:

@image html GUIResourceCompilerHowToUseIt_3.png

Perhaps you will realise that there are more files that need to be added. If you want to add some more files, you can again do it with "add paths from the input file".
The new paths will add with the paths which you added before. Moreover, you can do it manually each file with the first option "add path to file list".

@image html GUIResourceCompilerHowToUseIt_31.png

To see the new file included we again use the option "show file list"


@image html GUIResourceCompilerHowToUseIt_32.png


When you are done with adding all the files, you can then create the output file. As you can see there are two different options to create the file:

- "create resource file"
- "create header file"

The main difference is the way how they have been created. The first one, the executable file will need the resource file with the information.
In the second one the header file must be included and compiled into your project before and there will be no other files than the executable file.

Here are the pictures which show how is structure the information inside of the output file according to the option that you choose.

@subsubsection sec_ResourceFile Resource File

@image html GUIResourceCompilerTable.png

On the top of the file there is the information about the files and then the content of them. See also CGUIResourceFileHandler::SetResourceData(eC_String & pkRCPath)


@warning

The paths of the input file have to be the same than the file name in the content.

@subsubsection sec_HeaderFile Header File

@image html GUIResourceCompilerTableHeader.png

A resource header file is a C++ header which defines an array (eC_UByte GUIResourceFileData[]).  See also CGUIResourceFileHandler::SetResourceData(eC_UByte *pucResourceData)


@warning
This option is the same as if you generate a resource file. The input file paths have to be the same than the file name in the content.

Finally, you choose one of these options and write the name of the output file.

@image html GUIResourceCompilerHowToUseIt_4.png

*/
class CGUIResourceFileHandler
{
public:
    /// Enumeration for the order in which files are opened.
    enum FileOpenOrder_t
    {
        RESOURCE_FILE_FIRST, ///< First try opening from resource file.
        RESOURCE_FILE_LAST ///< First try opening from disk.
    };

    /// Enumeration for the buffering strategy to use when opening a file
    enum BufferingStrategy_t
    {
        BUFFERING_OFF = 0,  ///< Do not use buffering
        BUFFERING_ALWAYS,   ///< Always use
        BUFFERING_DEFAULT   ///< Use previously set buffering strategy
    };

    /// return pointer to resource file handler instance
    static inline CGUIResourceFileHandler& GetInstance() { return ms_kResHandler; }

    /// Should only be called by the destructor. As a workaround for memory leak
    /// detection ~CGUI() may call it too, but it is forbidden to be called by
    /// other code.
    void DeInit();

    /** Opens a file. This will either load the file from disk, from a resource-file or from a resource-header.
        Please refer to SetFileOpenOrder() for details on how to specify the order in which to search for a file on disk, in a resource-file or a resource-header.
        All files will be opened in READ-ONLY mode. If you wish to open a file from disk with WRITE-ACCESS, use CGUIFileSysWrap instead.
        In your application use eC_File* pMyFile = GETRESHANDLER.Open(kPath);
        You must close the file using GETRESHANDLER.Close(pMyFile) after use.
        @param kPath eC_String with path of file (will be automatically prefixed with current ResourcePathPrefix)
        @param eBufferingStrategy Specifies the buffering strategy the resource file handler uses to open the file. BUFFERING_OFF, BUFFERING_ALWAYS or BUFFERING_DEFAULT
        When BUFFERING_DEFAULT is used the previously set buffering-strategy for loading from disk or header is used.
        @return Pointer to eC_File if file was found and opened, NULL if not.
    */
    eC_File* Open(const eC_String& kPath, const BufferingStrategy_t eBufferingStrategy = BUFFERING_DEFAULT);

    /** Close files which were opened with the handler. This function deletes buffers which are used in RESOURCE_FILE mode, and deletes the eC_File.
        The pointer to eC_File is invalid after closing, because it was deleted by ResourceFileHandler.
        To close a file in Guiliani use GETRESHANDLER.Close(pMyFile);

        @param file pointer to eC_File, which should be closed (deleted)
        @return True if file was closed(deleted), False if file was not opened with ResourceFileHandler.
    */
    eC_Bool Close(eC_File* file);

    /** Orders the ResourceFileHander to use the given Resource-header.
        The Resource-header is essentially resource-file residing in memory (e.g. linked into the executable)
        Multiple calls to this method will result in multiple memory regions being loaded
        @see Open,SetFileOpenOrder
        @param pubResourceData pointer to resource data.
        @return True if loading successful, False if not
    */
    eC_Bool SetResourceData(const eC_UByte *pubResourceData);

    /** Loads the given resource-file archive.
        All subsequent "open file"-operations will search for the files within this resource-file.
        @see Open,SetFileOpenOrder
        @param pkRCPath Path name to resource file. (Will automatically be prefixed with ResourcePathPrefix)
        @return True if loading successful, False if not
    */
    eC_Bool SetResourceFile(const eC_String& pkRCPath);

    /** Reset the ResourcefileHandler.
    Unloads all ResouceFileData (deletes open files and buffers and headerdata).
    After calling this function a new resourcefile can be set with SetResourceData(...).
    */
    void UnloadResourceFile();

    /** Add a new memory resource
        this inserts a memory location to the internal lookup, so a call to Open results
        in returning an eC_File pointing to this memory location.
        By using this interface GraphicsWrapper and other resource-users can also deal with
        previously loaded files or memory locations filled by other devices.
        If a memory-location was already added with the given name it will be overwritten.
        @param kResourceName the name of the resource which will be used when calling Open
        @param uiResourceSize the size of the resource in bytes
        @param pubResourceLocation byte-pointer to the actual memory
        @return true if successful otherwise false
    */
    eC_Bool AddMemoryResource(const eC_String& kResourceName, const eC_UInt uiResourceSize, eC_UByte *pubResourceLocation);

    /** Removes a previously loaded memory-resource from internal lookup
        this does NOT free the attached memory!
        @param kResourceName the name of the resource to remove
        @return true if successful otherwise false
    */
    eC_Bool RemoveMemoryResource(const eC_String& kResourceName);

    /** Retrieves information about a file within a ResourceHeader.
        A ResourceHeader needs to be set via SetResourceData() prior to calling GetFileInformation().
        The file with the given name will be searched within the ResourceHeader, and its size will be returned in uiFileSize.
        @param kPath Path of file (will not be prefixed with ResourcePrefix)
        @param uiFileSize File size in byte in case of success. Undefined in case of failure.
        @param ppubData Pointer to first byte of file data in case of success, otherwise NULL.
        @return True if the file was found within the ResourceHeader, False otherwise. */
    eC_Bool GetFileInformation(const eC_String& kPath, eC_UInt &uiFileSize, const eC_UByte** ppubData);

    /** Sets a path which will used as a prefix during all subsequent calls to Open().
        E.g. First supplying "C:/Data/" as a ResourcePathPrefix and then calling Open("MyFile.png") will effectively
        try to open a file "C:/Data/MyFile.png".
        @param kResourcePrefix The new path prefix
    */
    void SetResourcePathPrefix(const eC_String& kResourcePrefix);

    /** Returns the resource path prefix.
        @return The path prefix that is used for all resources.
    */
    inline const eC_String& GetResourcePathPrefix() const { return m_kResourcePrefix; }

    /** Sets the file open order.
        @param eFileOpenOrder Which order for file opening: RESOURCE_FILE_FIRST, RESOURCE_FILE_LAST.
    */
    inline void SetFileOpenOrder(const FileOpenOrder_t eFileOpenOrder) { m_eFileOpenOrder = eFileOpenOrder; }

    /**
    Set buffering strategy for loading files from disk.
    @param bBufferFileFromDisk Specifies whether the resource file handler should try to buffer the file in memory.
    */
    inline void SetDiskBuffering(eC_Bool bBufferFileFromDisk) { m_bBufferFileFromDisk = bBufferFileFromDisk; }

    /**
    Set buffering strategy for loading files from resourcefile-header.
    @param bBufferHeader Specifies whether the resource file handler should try to buffer the data in RAM.
    */
    inline void SetHeaderBuffering(eC_Bool bBufferHeader) { m_bBufferHeader = bBufferHeader; }

#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    /** destructor: deletes dynamic data
    */
    ~CGUIResourceFileHandler();

private:
    //struct of resource header
    struct ResourceFileHeader_t
    {
        eC_UInt uiResourceVersion;
        eC_UInt uiHeaderLength;
        eC_UInt uiFileCnt;
        eC_UInt uiFileSize;
    };

    struct ResourceFileInfo_t
    {
        ResourceFileHeader_t kResourceFileHeader;   // header information of resource-file
        eC_File *pkResourceFileHandle;              // file handle to either memory-file or platform-file
        const eC_String *pkResourceFileName;        // name of resource-file if container was loaded
        const eC_UByte *pubResourceData;            // resource-data in memory from statically linked byte-array
    };

    // struct to handle memory locations and treat them as files
    // the actual pointer and size of the data is managed by eC_MemoryFile
    struct MemoryFileInfo_t
    {
        const eC_String *pkMemoryFileName;  // name of resource-file to identify it
        eC_UInt uiMemoryFileSize;           // size of attached memory-file
        eC_UByte *pubMemoryFileLocation;    // pointer to memory-location
    };

    //struct of pointers to header info for each file in ResourceFile
    struct FileSearchEntry_t
    {
        eC_String kPath;            // full name of file as given during Resourcefile creation. This is used for search.
        eC_UInt uiFileSize;         // size of file in bytes
        eC_UInt uiArrayOffset;      // offset in bytes from beginning of ResourceData to the first byte of the given file's data
        eC_UByte ubSegmentIndex;    // used to identify the segment within the resources
    };

    // each opened file has some management informations
    struct GUIFileHandle_t
    {
        eC_File* pkFileHandle;              // The FileHandle which was returned when it was opened.
        eC_UByte* pubFileBuffer;            // The buffer where the file is loaded in ResouceFile mode
        eC_Int iIndexInAvailableFiles;      // Used to check if file was opened before
        eC_Bool operator==(const GUIFileHandle_t &theFile) const { return pkFileHandle == theFile.pkFileHandle; }
    };

    // returned when searching for a file
    struct FileSearchResult_t
    {
        eC_Int iFileIndex;
        eC_UByte ubSegmentIndex;
    };

private:
    /** constructor: initializes datastructure
    */
    CGUIResourceFileHandler(void);

    /**
    Copy-constructor. Should not be used.
    Dummy declaration with no implementation just to hide the operator.
    @param kSource Source object to be copied.
    */
    CGUIResourceFileHandler(const CGUIResourceFileHandler& kSource);

    /**
    Operator= method. Should not be used.
    Dummy declaration with no implementation just to hide the operator.
    @param kSource Source object to be copied.
    @return This object.
    */
    CGUIResourceFileHandler& operator=(const CGUIResourceFileHandler& kSource);

    // comparator function for FileSearchEntry_t
    static int FileSearchEntryComparator(const void* first, const void* second)
    {
        const FileSearchEntry_t *firstEntry = static_cast<const FileSearchEntry_t*>(first);
        const FileSearchEntry_t *secondEntry = static_cast<const FileSearchEntry_t*>(second);

        if (firstEntry->kPath < secondEntry->kPath)
            return -1;
        else if (firstEntry->kPath > secondEntry->kPath)
            return 1;
        else
            return 0;
    }

    /**
    Returns a ResourceFileHeader_t struct read out from the provided file.
    @param pkHeaderFile Handle of file to read
    */
    ResourceFileHeader_t ReadResourceHeader(eC_File *pkHeaderFile);

    /**
    Sets up the search table for all files within the loaded resource
    @param pkResourcefile handle of resource file to process
    @param uiFileCnt number of files within resource file
    */
    void SetupSearchTable(eC_File* pkResourceFile, const eC_UInt uiFileCnt);

    /**
    Searchs for a File in the array of available files
    @param kFilePath Path of the file
    @param ubSegment Reference for segment
    @return FileSearchResult_t with filled members. uiFileIndex == FILE_NOT_FOUND_INDEX if file was not found, index of the file else.
    */
    FileSearchResult_t SearchForFile(const eC_String& kFilePath);

    /**
    Opens file from disk. The member eBufferingStrategy is used to determine whether the Resource File Handler will try to buffer the file in memory.
    @param kPath Path of the file.
    @param newFile File handle.
    @param eBufferingStrategy The buffering strategy to use for load
    @return Error code FILE_OPEN_FAILURE if open fails.
    */
    eC_Int OpenFileFromDisk(const eC_String& kPath, GUIFileHandle_t &newFile, const BufferingStrategy_t eBufferingStrategy);

    /**
    Opens file from resource file. The member eBufferingStrategy is used to determine whether the Resource File Handler will try to buffer the file in memory.
    @param kPath Path of the file.
    @param newFile File handle.
    @param eBufferingStrategy The buffering strategy to use for load
    @return Error code FILE_OPEN_FAILURE if open fails.
    */
    eC_Int OpenFileFromResourceFile(const eC_String& kPath, GUIFileHandle_t &newFile, const BufferingStrategy_t eBufferingStrategy);

    /**
    Gets a previously attached memory-resource
    @param kResourceName the name which was used in AddMemoryResource
    @return eC_File-pointer of loaded memory-resource or NULL if not found
    */
    eC_File* GetMemoryResource(const eC_String& kResourceName);

    /**
    Opens file from resource file. The member eBufferingStrategy is not used
    @param kPath Path of the file.
    @param newFile File handle.
    @return Error code FILE_OPEN_SUCCESS if file was found or FILE_OPEN_FAILURE if open fails.
    */
    eC_Int OpenFileFromMemory(const eC_String& kPath, GUIFileHandle_t &newFile);

private:
    /** Enumeration with different modes in which the resource file handler may run. They define from where resources will be loaded.
        -# RESOURCE_NONE:   In this case the handler tries to open the file directly from disk.
        -# RESOURCE_HEADER: Headerfile created with the GUIResourceCompiler must be included!
                            This headerfile contains an array with all files added to resourcefile.
                            So no filesystem is needed if all used resources were added to resourcefile.
        -# RESOURCE_FILE:   Resourcefile was compiled to one file on disk. The file path has to be passed to constructor.
                            GUIResourceFileHandler opens this resourcefile from disk (at runtime) and writes resources to buffers if needed.
    */
    enum GUIResourceMode_t {RESOURCE_NONE, RESOURCE_HEADER, RESOURCE_FILE};

    // Return codes for OpenFileFromResourceFile
    enum
    {
        FILE_OPEN_SUCCESS = 0,
        FILE_OPEN_FAILURE = -1
    };

    enum
    {
        FILE_NOT_FOUND_INDEX = -1,
        RESOURCE_READ_BUFFER_SIZE = 2048
    };

    FileOpenOrder_t m_eFileOpenOrder;                               // internal mode for resources (e.g. resource-container, memoryblocks, ...)
    GUIResourceMode_t m_eMode;                                      // stores the currently used type of resource
    static CGUIResourceFileHandler ms_kResHandler;                  // static resource file hander instance
    eC_TArray<ResourceFileInfo_t> m_pkResourceFileInfo;             // information for each loaded segment
    eC_TListDoubleLinked<MemoryFileInfo_t> m_ListMemoryFileInfo;    // lookup-table for all loaded memory locations
    FileSearchEntry_t *m_ListOfAvailableResources;                  // table sorted by filename for searching individual files
    eC_UInt m_uiFileSearchEntryCount;                               // number of search entries stored in array
    eC_String m_kResourcePrefix;                                    // The resource prefix which is prepended to all file-open operations
    eC_TListDoubleLinked<GUIFileHandle_t> m_ListOfOpenedFiles;      // list of opened files
    eC_UByte m_ubLoadedSegmentCount;                                // number of loaded segments
    eC_Bool m_bBufferFileFromDisk;                                  // loading strategy for data resided on disk
    eC_Bool m_bBufferHeader;                                        // loading strategy for data in header-files
};

#endif
