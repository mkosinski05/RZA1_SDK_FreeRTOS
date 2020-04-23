/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_File.h
* %version:        134.0 %
*
* author:          hh04031
* creation date:   Tue Aug 09 12:19:41 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_FILE__H_
#define EC_FILE__H_

//-----------------------------------------------------------------------------------------
#include "eC_Config.h"
#include "eC_Platform.h"

//-----------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!

#define eDBG_ASSERT_SETERR(b, e) \
  if(!(b)) \
  { \
    SetError(e); \
    /* eDBG_ASSERT(false); */ \
  }

#define eDBG_ASSERT_SETERR_RETURN(b, e, r) \
  if(!(b)) \
  { \
    SetError(e); \
    /* eDBG_ASSERT(false); */ \
    return (r); \
  }

#define eDBG_ASSERT_SETERR_RETURN_VOID(b, e) \
  if(!(b)) \
  { \
    SetError(e); \
    /* eDBG_ASSERT(false); */ \
    return; \
  }

//-----------------------------------------------------------------------------------------
/**@name eC_Files
@memo
File I/O interface.
@doc
This class provides platform and storage type independent I/O streaming interface.
Currently there are two implementations of \Ref{eC_File} class: <BR>
\Ref{eC_MemoryFile} and \Ref{eC_PlatformFile}. <BR>
To get more information about \Ref{eC_Files} and its handling, look at the \Ref{Tutorial_eC}.
@see Tutorial_eC
*/

//@{

//-----------------------------------------------------------------------------------------
/**
@memo
Abstract file I/O base class.
@doc
The class provides collection of standard methods to perform platform independent file I/O operations with no regard to the type of the storage.
By default all I/O opeartions perform in platform's native endian mode unless it is set explicitly by either \Ref{FS_BIGENDIAN} or
\Ref{FS_LITTLEENDIAN} flags. <BR>
To get more information about \Ref{eC_File} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_MemoryFile, eC_PlatformFile
@see Tutorial_eC
*/
class eC_File
{
  public:

    /** Flags of eC_File to set file data access. Used when calling the constructor.
    @see eC_File_Errors */
    enum eC_File_Flags
    {
      /// Requests the file to be opened with read access.
      FS_READACCESS   = 1,
      /// Requests the file to be opened with write access.
      FS_WRITEACCESS  = 2,
      /** Requests all file operations to be performed in big endian mode.
      When platform's native mode is big endian, no data conversion is performed. */
      FS_BIGENDIAN    = 4,
      /** Requests all file operations to be performed in little endian mode.
      When platform's native mode is little endian, no data conversion is performed. */
      FS_LITTLEENDIAN = 8,
      // Request to list the directory.
      FS_ENUMFILES    = 16,
    };

    /** Error codes of eC_File.
    @see eC_File_Flags */
    enum eC_File_Errors
    {
      /// No error.
      ERR_NONE = 0,
      /// Not open.
      ERR_NOTOPEN,
      /// No write access.
      ERR_NOWRITEACCESS,
      /// No read access.
      ERR_NOREADACCESS,
      /// End-of-file is reached.
      ERR_ENDOFFILE,
      /// Last write access of file failed.
      ERR_NOSPACE,
      /// Directory was opened for file search.
      ERR_FILESEARCH,
      /// Unknown parameter.
      ERR_UNKNOWNPARAM
    };

    /** Destructor. Note: the constructor for this class is protected member and <I>not</I> documented. */
    virtual ~eC_File();

    /** Reads one or more byte(s) out of the file.
    @param pubData Pointer to the buffer to be filled while reading the file.
    @param uiCount Number of bytes to be read. If not specified one byte is assumed.
    @return Actual number of bytes read. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Read16, Read32, Write8 */
    virtual eC_UInt Read8(eC_UByte *pubData, eC_UInt uiCount = 1) = 0;

    /** Reads one or more short word(s) out of the file.
    @param pusData Pointer to the buffer to be filled while reading the file.
    @param uiCount Number of shorts to be read. If not specified one short word is assumed.
    @return Actual number of bytes read. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Read8, Read32, Write16 */
    virtual eC_UInt  Read16(eC_UShort *pusData, eC_UInt uiCount = 1) = 0;

    /** Reads one or more word(s) out of the file.
    @param puiData Pointer to the buffer to be filled while reading the file.
    @param uiCount Number of longs to be read. If not specified one long word is assumed.
    @return Actual number of bytes read. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Read8, Read16, Write32 */
    virtual eC_UInt  Read32(eC_UInt *puiData, eC_UInt uiCount = 1) = 0;

    /** Writes one or more byte(s) into the file.
    @param pubData Pointer to buffer to be written into the file.
    @param uiCount Number of bytes to be written. If not specified one byte is assumed.
    @return Actual number of bytes written. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Write16, Write32, Read8 */
    virtual eC_UInt Write8(eC_UByte *pubData, eC_UInt uiCount = 1) = 0;

    /** Writes one or more short words(s) into the file.
    @param pusData Pointer to buffer to be written into the file.
    @param uiCount Number of shorts to be written. If not specified one short word is assumed.
    @return Actual number of bytes written. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Write8, Write32, Read16 */
    virtual eC_UInt Write16(eC_UShort *pusData, eC_UInt uiCount = 1) = 0;

    /** Writes one or more word(s) into the file.
    @param puiData Pointer to buffer to be written into the file.
    @param uiCount Number of longs to be written. If not specified one long word is assumed.
    @return Actual number of bytes written. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see Write8, Write16, Read32 */
    virtual eC_UInt Write32(eC_UInt *puiData, eC_UInt uiCount = 1) = 0;

    /** Sets the file position indicator.
    @param uiOffset Specifies the offset from <TT>uiWhence</TT> origin.
    @param uiWhence Specifies initial position to which <TT>uiOffset</TT> is applied. Can be one of the following:
    <UL>
    <LI>eC_SEEK_BEGIN   - Specifies beginning of the file. </LI>
    <LI>eC_SEEK_CURRENT - Specifies current position in the file.</LI>
    <LI>eC_SEEK_END     - Specifies end-of-file position. </LI>
    </UL>
    @return If successfull <B>true</B> is returned and error code is <I>reseted</I>, else <B>false</B> is returned and \Ref{ERR_NOSPACE} error code is set.
    @see Reset, GetPosition */
    virtual eC_Bool Seek(eC_UInt uiOffset, eC_UInt uiWhence = eC_SEEK_BEGIN) = 0;

    /** Sets file position indicator at the beginning of the file.
    @return Only on successfull calls <B>true</B> is returned and error code is <I>reseted</I>.
    @see Seek, GetPosition */
    virtual eC_Bool Reset(void);

    /** Calculates the actual file size.
    @return Total number of bytes of the file.
    @see GetRemainingBytes */
    virtual eC_UInt GetFilesize(void) = 0;

    /** Calculates the remaining number of bytes.
    @return Number of bytes between current file position indicator and end-of-file (EOF).
    @see GetFilesize */
    virtual eC_UInt GetRemainingBytes(void);

    /** Retrieves the value of current file position indicator.
    @return Value of current file position indicator.
    @see Seek, Reset */
    virtual eC_UInt GetPosition(void) = 0;

    /** Method to get direct access to File Data
        Method is only valid if eC_File wraps a constant memory block actually (ROM)
    @return Pointer to constant data.*/
    virtual void * GetConstantData(void)
    {
        return NULL;
    }

    /** Checks whether the file is open.
    @return Only if file is open <B>true</B> will be returned.
    @see eC_File_Errors */
    inline eC_Bool IsOpen(void);

    /** Checks whether the file operations are set to be performed in little endian mode. <BR>
    Note: Before using this method, you should check by calling \Ref{IsOpen}, if file is opened.
    @return If little endian mode is active <B>true</B> is returned.
    @see IsBigEndian, SetEndian */
    inline eC_Bool IsLittleEndian(void);

    /** Checks whether the file operations are set to be performed in big endian mode. <BR>
    Note: Before using this method, you should check by calling \Ref{IsOpen}, if file is opened.
    @return If big endian mode is active <B>true</B> is returned.
    @see IsLittleEndian, SetEndian */
    inline eC_Bool IsBigEndian(void);

	/** Changes endianess of the file operations.
	Passing any other value than FS_BIGENDIAN or FS_LITTLEENDIAN (e.g. like 0) will reset the
	endianess to the host cpu's native format.
    @param uiMode accepts either FS_BIGENDIAN or FS_LITTLEENDIAN as parameter.
	@see IsBigEndian, IsLittleEndian */
	void SetEndian(eC_Flag uiMode);

    /** Checks whether the end-of-file (EOF) is reached. <BR>
    Note: Before using this method, you should check by calling \Ref{IsOpen}, if file is opened.
    @return Only if EOF is reached <B>true</B> is returned.
    @see GetPosition */
    inline eC_Bool IsEOF(void);

    /** Provides the error code of last performed operation.
    @return Current error code, see \Ref{eC_File_Errors} for possible values.
    @see eC_File_Errors */
    inline eC_Flag GetError(void);   	

	//
    inline eC_UByte Read8i();
    inline eC_UShort Read16i();
    inline eC_UInt Read32i();	
    inline void Write8i(eC_UByte ubData);
    inline void Write16i(eC_UShort usData);
    inline void Write32i(eC_UInt uiData);

    eC_UInt ReadString(eC_Char *pcBuffer, eC_UInt uiLen=255);
    void WriteString( const eC_Char *pcBuffer);

  protected:

    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only

    //
    eC_File(void);

    //
    inline void SetError(eC_Flag uiError);

    //
    static inline eC_UShort Swap16(eC_UShort usValue);

    //
    static inline eC_UInt Swap32(eC_UInt uiValue);

    //
    eC_Flag m_uiStatus;

    //
    eC_Bool m_bNeedSwap;

    //
    eC_File_Errors m_eError;
};

//---------------------------------------------------------------
inline eC_Bool eC_File::IsOpen()                    { return (m_uiStatus != 0); }
inline eC_Bool eC_File::IsLittleEndian()            { return ((m_uiStatus & FS_BIGENDIAN) == 0); }
inline eC_Bool eC_File::IsBigEndian()               { return ((m_uiStatus & FS_BIGENDIAN) != 0); }
inline eC_Bool eC_File::IsEOF()                     { return (m_eError == eC_File::ERR_ENDOFFILE) || (GetRemainingBytes() == 0 ); }
inline eC_Flag eC_File::GetError()                  { return m_eError; }
inline void eC_File::SetError(eC_Flag uiError)      { m_eError = (eC_File_Errors) uiError; }
inline eC_UShort eC_File::Swap16(eC_UShort usValue) { return ((eC_UShort)(((usValue >> 8) & 0xff) | (usValue << 8))); }
inline eC_UInt eC_File::Swap32(eC_UInt uiValue)     { return Swap16((eC_UShort) (uiValue >> 16UL)) | ((eC_UInt)Swap16((eC_UShort) uiValue) << 16UL); }
inline eC_UByte eC_File::Read8i()                   { eC_UByte ub; ub = 0u; Read8(&ub,1); return ub; }
inline eC_UShort eC_File::Read16i()                 { eC_UShort us; us = 0u; Read16(&us,1); return us; }
inline eC_UInt eC_File::Read32i()                   { eC_UInt ui; ui = 0u; Read32(&ui,1); return ui; }
inline void eC_File::Write8i(eC_UByte ubData)       { eC_UByte ub = ubData; Write8(&ub,1); }
inline void eC_File::Write16i(eC_UShort usData)     { eC_UShort us = usData; Write16(&us,1); }
inline void eC_File::Write32i(eC_UInt uiData)       { eC_UInt ui = uiData; Write32(&ui,1); }

//---------------------------------------------------------------

/**
@memo
Disk file class.
@doc
The class represents the eC_File implementation for disk files.
For the complete documentation of public methods reference to \Ref{eC_File}. <BR>
To get more information about \Ref{eC_PlatformFile} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_File, eC_MemoryFile
@see Tutorial_eC
*/
class eC_PlatformFile : public eC_File
{
  public:

    /** Constructor.
    @param acName Specifies the name of the file to be opened.
    @param uiMode Specifies the access and endian mode.
    @see eC_File_Flags */
    eC_PlatformFile(const eC_Char *acName, eC_Flag uiMode);

    /** Constructor for 16bit chars filenames .
    @param acName Specifies the name of the file to be opened.
    @param uiMode Specifies the access and endian mode.
    @see eC_File_Flags */
    eC_PlatformFile(const eC_WChar *acName, eC_Flag uiMode);

    /** Constructor for file search.
    @see findFirstFile find NextFile findClose */
    eC_PlatformFile();

    /** Destructor. */
    virtual ~eC_PlatformFile();

    // read access

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt  Read8(eC_UByte *pubData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt  Read16(eC_UShort *pusData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt  Read32(eC_UInt *puiData, eC_UInt uiCount = 1);

    // write access

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write8(eC_UByte *pubData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write16(eC_UShort *pusData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write32(eC_UInt *puiData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt GetPosition(void);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt GetFilesize(void);

    /** Sets the file position indicator. Note:
    <UL>
    <LI>Depending on specific platform implementation, succesfull <B>Seek</B> calls may (or may not) clear the end-of-file indicator for the stream. </LI>
    <LI>In (FS_WRITEACCESS) or (FS_READACCESS|FS_WRITEACCESS) mode the seek function is allowed to set file-position indicator beyond the end
    of existing data in the file. If data are written later at this point, subsequent reads in the gap will return bytes with value 0 until data
    are actually written into the gap.</LI>
    </UL>
    @param uiPosition Specifies the offset from <TT>uiWhence</TT> origin.
    @param uiWhence Specifies initial position to which <TT>uiOffset</TT> is applied. Can be one of the following:
    <UL>
    <LI>eC_SEEK_BEGIN   - Specifies beginning of the file. </LI>
    <LI>eC_SEEK_CURRENT - Specifies current position in the file.</LI>
    <LI>eC_SEEK_END     - Specifies end-of-file position. </LI>
    </UL>
    @return If successfull <B>true</B> is returned and error code is <I>reseted</I>, else <B>false</B> is returned and \Ref{ERR_NOSPACE} error code is set.
    @see Reset, GetPosition */
    virtual eC_Bool Seek(eC_UInt uiPosition, eC_UInt uiWhence = eC_SEEK_BEGIN);

    /** Find first file in given directory for 16bit chars filenames .
    @param acName Specifies the name of the directory to be opened.
    @param fileData specicifies information about the founded file
    @return Only if directory could be open and a file could be found <B>true</B> will be returned. If an error occured the function sets an error indicator (refer to \Ref{eC_File_Errors}).
    @see findNextFile findClose */
    eC_Bool findFirstFile(const eC_WChar *acName, eC_PlatformFileIO::FILE_FIND_DATA& fileData);

    /** Find next file in directory opened with FindFirstFile.
    @param fileData specicifies information about the founded file
    @return If a file could be found <B>true</B> will be returned.
    @see findFirstFile findClose */
    eC_Bool findNextFile(eC_PlatformFileIO::FILE_FIND_DATA& fileData);

    /** Close a directory opened with FindFirstFile.
    @return If the function succeeds <B>true</B> will be returned.
    @see findFirstFile findNextFile*/
    eC_Bool findClose();

  private:

    // !!! INTERNAL USE ONLY !!!
    // all following private members are for internal use only

    // platform filehandle
    void * m_pvHandle;
};

//---------------------------------------------------------------
/**
@memo
Memory file class.
@doc
The class represents the eC_File imlementation for RAM files.
For the complete documentation of public methods reference to \Ref{eC_File}. <BR>
To get more information about \Ref{eC_MemoryFile} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_File, eC_PlatformFile
@see Tutorial_eC
*/
class eC_MemoryFile : public eC_File
{
  public:

    /** Constructor. Creates memory file using the specified buffer and its size.
    @param pvData Specifies the base buffer on which the memory file is to be created.
    @param uiSize Specifies the maximum size of memory file (should not exceed the base buffer size).
    @param uiMode Specifies the access and endian mode.
    @see eC_File_Flags */
    eC_MemoryFile(void *pvData, eC_UInt uiSize, eC_Flag uiMode);

    /** Destructor. */
    virtual ~eC_MemoryFile();

    // read access

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Read8(eC_UByte *pubData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Read16(eC_UShort *pusData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Read32(eC_UInt *puiData, eC_UInt uiCount = 1);

    // write access

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write8(eC_UByte *pubData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write16(eC_UShort *pusData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt Write32(eC_UInt *puiData, eC_UInt uiCount = 1);

    // !!! INHERITED !!! documentation in base class
    virtual eC_Bool Seek(eC_UInt uiOffset, eC_UInt uiWhence = eC_SEEK_BEGIN);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt GetPosition(void);

    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt GetFilesize(void);

  protected:

    // !!! INTERNAL USE ONLY !!!
    // all following private members are for internal use only

    // Maximum buffer size.
    eC_UInt m_uiSize;

    // Base pointer of buffer.
    eC_UByte * m_pubBase;

    // Current read/write opsition.
    eC_UInt m_uiPosition;
};

//---------------------------------------------------------------
/**
@memo
Memory file class.
@doc
The class represents the eC_File imlementation for ROM files.
For the complete documentation of public methods reference to \Ref{eC_File}. <BR>
To get more information about \Ref{eC_MemoryFile} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_File, eC_MemoryFile, eC_PlatformFile
@see Tutorial_eC
*/
class eC_ROMFile : public eC_MemoryFile
{
public:

    /** Constructor. Creates memory file containg ROM Data using the specified buffer and its size.
    @param pvData Specifies the base buffer on which the memory file is to be created.
    @param uiSize Specifies the maximum size of memory file (should not exceed the base buffer size).
    @param uiMode Specifies the access and endian mode.
    @see eC_File_Flags */
    eC_ROMFile(void *pvData, eC_UInt uiSize, eC_Flag uiMode) :
        eC_MemoryFile(pvData, uiSize, uiMode)
    {
    }

    /** Destructor. */
    virtual ~eC_ROMFile()
    {
    }

    // override GetConstantData method for ROMfiles: 
    // ONLY HERE it's valid to use Datapointer even outside the scope of eC_ROMFile
    // a usecase is blitting images directly from RAW ROM Images
    virtual void * GetConstantData()
    {
        return m_pubBase;
    }
};

//@}

//---------------------------------------------------------------
#endif
