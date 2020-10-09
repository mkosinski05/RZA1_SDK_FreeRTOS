/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_String.h
* %version:        134.0 %
*
* author:          hh04031
* creation date:   Tue Aug 23 13:30:33 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/
#ifndef EC_STRING__H_
#define EC_STRING__H_

#include "eC_TArray.h"

/**
@memo
Base class for string handling.
@doc
This class provides a convenient way to handle strings using additional operators that expand the standard string operations of C/C++ library.
So you can simply use e. g. the "+" operator for concatenating two strings or comparing strings by "==" operator.
This simplifies the overall string handling and allows the code to become more readable. <BR>
To get more information about \Ref{eC_String} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_Types
@see Tutorial_eC
*/

// sanity check
#ifndef eC_STRING_UNICODE_UTF8
#ifndef eC_STRING_UNICODE_UTF16
#ifndef eC_STRING_UNICODE_UTF32
// Note: eC_STRING_UNICODE_UTF8 means that the current platform's unicode API (external to the application) uses UTF-8 encoding.
//       eC_STRING_UNICODE_UTF16 means that the current platform's unicode API (external to the application) uses UTF-16 encoding, e.g. WIN32 does so.
//       eC_STRING_UNICODE_UTF32 means that the current platform's unicode API (external to the application) uses UTF-32 encoding. Be aware that this is not yet supported by the eC_String class.
//       This does not mean that unicode API is used when calling the current platform's system functions.
//       To use platform's unicode version of the system API, and not the ASCII functions, you need to define the "UNICODE" preprocessor define as usual.
//       To automatically select between unicode and ASCII version use the T definitions, like eC_TChar, ToTChar, FromTChar, EC_TEXT and so on.
#error Please define eC_STRING_UNICODE_UTF8, eC_STRING_UNICODE_UTF16 or eC_STRING_UNICODE_UTF32 matching your platforms API unicode encoding.
#endif
#endif
#endif

#if (defined eC_STRING_UNICODE_UTF32) & (defined UNICODE)
#error "Be aware that UTF-32 encoding is not yet supported by the eC_String class. If you activate UNICODE and eC_STRING_UNICODE_UTF32 it seems that you are trying to use the UTF-32 API interfaces which CANNOT be used together with the eC_String class. You can comment out this error message, if you understand what you are doing and you are totally sure that you want this behaviour."
#endif

/* ********************
 TES internal note:
 All of the following definitions (eC_TChar, ToTChar, ...) that determine the compile-time mapping for T-type strings should not be used by eC_String.cpp itself, as this would make the eGML library UNICODE or !UNICODE specific and contradict the idea of a generic library, that can be linked as is, for unicode and ascii programs (per platform). They should only be used by calling functions of this class. For the same reason the UNICODE define shall not be used inside eC_String.cpp.
 **********************

    The following typedef and preprocessor defines are useful to automatically select between unicode and ASCII version of the current platform's API, as in the following code for example:
        eC_TChar* pString=estrPrefix.ToTChar_Alloc();
        delete[] pString;

    Such strings can be passed to and can be read from the platform's system API later on.

    eC_TChar is a string either encoded in the platforms UNICODE encoding or ASCII encoding.
    EC_TEXT is a macro to define a string literal in the platform's UNICODE encoding (determined by the compiler) or ASCII encoding.
*/
#ifdef UNICODE
typedef eC_Unicode eC_TChar;
#define EC_TEXT(quote) L##quote
#define ToTChar ToUnicode
#define ToTChar_Alloc ToUnicode_Alloc
#define FromTChar FromUnicode
#define eTChar eUnicode
#define GetInternalTChar GetInternalUnicode
//< Warning: GetInternalTChar() obviously only works if EC_STRING_FORMAT_XXX matches EC_UNICODE_UTFXX, otherwise an assertion will be raised at runtime
#else   /* UNICODE */
typedef eC_Char eC_TChar;
#define EC_TEXT(quote) quote
#define ToTChar ToASCII
#define ToTChar_Alloc ToASCII_Alloc
#define FromTChar FromASCII
#define eTChar eASCII
#define GetInternalTChar GetInternalASCII
//< Warning: GetInternalTChar() obviously only works if EC_STRING_FORMAT_XXX matches EC_UNICODE_UTFXX, otherwise an assertion will be raised at runtime
#endif /* UNICODE */

/**
    FromTChar() preprocessor define:
    Converts a string of compile-time type (e.g. eC_TChar or TCHAR on WIN32) to the internal format.
    This is useful if you want to convert a string that may either be platform's UNICODE or ASCII format depending on the compile-time settings to an eC_String.
    E.g.:
            eC_String estrPlz;
            estrPlz.FromTChar(EC_TEXT("60594"));

    GetInternalXXX() functions:
    Return the string with internal representation.
    This is useful if you want to use the internal buffer of a string object as a constant "native" C++ string type, like UFT-16 or ASCII.
    That is, you can use these functions to assign eC_String's buffer to an external const eC_UTF16* or const eC_Char* pointer.
    This works well if the internal format of eC_String matches the function, otherwise an assertion is raised.
    E.g.:
        static eC_String string("This string shall be "
            "UTF-16 inside eC_String instance");
        eC_UTF16 *estrForExternalAPI;
        estrForExternalAPI = string.GetInternalUTF16(); //< estrForExternalAPI will point to eC_String's internal memory, no string format conversion is done

        static eC_String string("This string shall be "
            "ASCII inside eC_String instance");
        eC_Char *estrForExternalAPI;
        estrForExternalAPI = string.GetInternalASCII(); //< estrForExternalAPI will point to eC_String's internal memory, no string format conversion is done

    The following source code only works if using GUIliani graphical user interface library.
    Please ignore the following example use case when using eC without GUIliani.
    Nevertheless the general idea to use reference counting pointers still might be a good choice.

    If you are using the GUIliani graphical user interface framework when calling a native UNICODE API a good idea might be to use auto pointers in order to automatically release the memory allocation, like this:
        #include "GUIAutoPtr.h"
        eC_String kCaption(EC_TEXT("Text to be saved in string object and later to be copied out and possibly converted and to be passed to system API"));
        CGUIAutoPtr<eC_TChar> pcCaption(kCaption.ToTChar_Alloc()); // copy and possibly convert to new buffer
        SetWindowText(m_hWnd, pcCaption.get()); // pass to Windows API

    The following preprocessor defines belong to eStringFormat_t and can be used like demonstrated in the following sample code snippet to get a unicode string out of the string object if its internal encoding is equal to the platform's unicode encoding:
        if(pkString->GetFormat() == eC_String::eUNICODE)
        {
            pcString = pkString->GetInternalUnicode(); // get pointer, no conversion, encoding must match
        }
        else // internal string object format does not match platform's unicode encoding, must convert if we need to get a unicode string for the platform API
        {
            eC_UInt uiLengthByte = pkString->ToUnicode(pAlreadyAllocatedUnicodeBuffer); // get pointer and convert string if encoding did not match, copy to previously allocated buffer, buffer must be big enough
        }
*/
#ifdef eC_STRING_UNICODE_UTF8
#define eUNICODE eUTF8
#endif
#ifdef eC_STRING_UNICODE_UTF16
#define eUNICODE eUTF16
#endif

class eC_StringConverter;

class eC_String
{
public:
    /**
        Internal string formats: eASCII, eUTF8 or eUTF16. eUTF32 not yet supported.
    */
    enum eStringFormat_t
    {
        eASCII=0,
        eUTF8,
        eUTF16
    };

    /** basic constructor. Initialises eC_String object with a string that can be
        an ordinary ascii string or a utf-8 string or a utf-16 string.
        @param pcString pointer to string, if it is NULL an empty string is created.
        @param uiLength Length of the string in characters. Can be different from pcString length, <BR>
                        in which case the string is cut respectively. Default is <BR>
                        0, in which case the actual string length is used.
        @param eFormat  Format of the string. Default is eASCII. Can be eASCII, eUTF8 or eUFT16.
    */
    eC_String(const eC_Char *pcString = 0, eC_UInt uiLength=0, eStringFormat_t eFormat=eASCII);

    /** basic constructor. Initialises eC_String object with a string that is UTF16 string.
        @param pwcString pointer to string, can not be NULL, otherwise there is a conflict in construtors.
        @param uiLength Length of the string in characters. Can be different from pwcString length, <BR>
                        in which case the string is cut respectively. Default is <BR>
                        0, in which case the actual string length is used.
    */
    eC_String(const eC_UTF16 *pwcString, eC_UInt uiLength=0);

#ifndef eC_STRING_UNICODE_UTF32
    /** basic constructor. Initialises eC_String object with a string that is UNICODE encoded.
        @param pwcString pointer to string, can not be NULL, otherwise there is a conflict in construtors.
        @param uiLength Length of the string in characters. Can be different from pwcString length, <BR>
                        in which case the string is cut respectively. Default is <BR>
                        0, in which case the actual string length is used.
    */
    eC_String(const eC_Unicode *pwcString, eC_UInt uiLength=0);
#endif

    /** copy-constructor.
        @param rkString is the string object to copy from */
    eC_String(const eC_String& rkString);

    /** constructor. Initialises eC_String object with a specified length, all positions filled with the same character.
        @param cData character to fill the complete string with
        @param iSize length of the string / number of positions to fill */
    eC_String(const eC_Char cData, const eC_Int iSize);

    /** constructor for initialisation with an integer number. Conversion works like an "IntToStr" method.
        @param iNumber number to convert to string */
    eC_String(const eC_Int &uiNumber);
    eC_String(const eC_UInt &uiNumber);
    eC_String(const int &iNumber);
    eC_String(const eC_Flag &uiNumber);

    /** constructor for initialisation with a float number.
        @param iNumber number to convert to string */
    eC_String(const eC_Float &fNumber);

    /** destructor, frees string object. */
    ~eC_String();

    /** assigns a string to this string object.
        @param rkString string object to assign
        @return this string object */
    eC_String& operator= (const eC_String& rkString);

    /** compares this string with another string in sensitive kind for equality.
        @param rkString string object to compare with
        @return only if both length and content of the two strings are identical <B>true</B> will be returned */
    bool operator== (const eC_String& rkString) const;

    /** compares this string with another string in sensitive kind for inequality.
        @param rkString string object to compare with
        @return if length or content of the two strings are <I>not</I> identical <B>true</B> will be returned */
    bool operator!= (const eC_String& rkString) const;

    bool operator < (const eC_String& rkString) const;
    bool operator > (const eC_String& rkString) const;

    inline bool operator <= (const eC_String& rkString) const { return (operator==(rkString) || operator<(rkString)); }
    inline bool operator >= (const eC_String& rkString) const { return (operator==(rkString) || operator>(rkString)); }

    /** provides the current length of the string in characters.
        @return current string length in character (excluding the terminal character)
    */
    eC_UInt GetLength() const;

    /** provides the current size of the string in bytes.
        @return current string length in byte (excluding the terminal character)
    */
    eC_UInt GetSize() const;

    /** resets this string object to empty string "". */
    void Empty();

    /** concatenates this string with another string.
        @param rkString string to concatenate with
        @return the resulting string, notice: this string is <I>not</I> stored in a persistant object */
    eC_String operator+ (const eC_String& rkString) const;

    /** concatenates this string with a standard string (const char *)  .
        @param pcString string to concatenate with (Attention: must be the ASCII format)
        @return the resulting string, notice: this string is <I>not</I> stored in a persistant object */
    eC_String operator+ (const eC_Char *pcString) const;

    /** concatenates two strings.
        @param pcString first string - kind of (const char *)(Attention: must be the ASCII format)
        @param rkString second string - reference to a eC_String object
        @return the resulting string, notice: this string is <I>not</I> stored in a persistant object */
    friend eC_String operator+ (const eC_Char *pcString, const eC_String& rkString) ;

    /** concatenates this string with another string and stores the result.
        @param rkString string to concatenate with
        @return the resulting string object */
    eC_String& operator+= (const eC_String& rkString);

    /** concatenates this string with a standard string (const char *) and stores the result.
        @param pcString standard string to concatenate with (Attention: must be the ASCII format)
        @return the resulting string object */
    eC_String& operator+= (const eC_Char *pcString);

    /** concatenates this string with a character and stores the result.
        @param cChar standard character to concatenate with (Attention: must be the ASCII format)
        @return the resulting string object */
    eC_String& operator+= (eC_Char cChar);
    eC_String& operator+= (eC_UTF16 cChar);

    // Error return value for Find().
    enum { notfound = 0xffffffff,
           npos = notfound };

    // Empty string
    static const eC_String ms_ckEmptyString;

    /** Find one or more characters in a string from an offset.
        @param  kToFind Characters to be found.
        @param  uiOffset Offset from which the search starts. (In characters from the beginning of the string)
        @param  bReverse If true, the search will be done in reverse direction. From the end of the string towards the beginning.
                If uiOffset is 0, a reverse search will automatically start at the end of the string.
        @return The index of the first specified character or
                eC_String::notfound if the specified characters were not found.
      */
    eC_UInt Find(const eC_String &kToFind, eC_UInt uiOffset=0, const eC_Bool bReverse = false) const;

    // support read-only access to data saved in internal representation
#ifndef eC_STRING_UNICODE_UTF32
    const eC_Unicode* GetInternalUnicode() const; //< only valid to use if internal eC_String encoding matches platform's UNICODE encoding
#endif
    const eC_UTF8 *GetInternalUTF8() const; //< only valid to use if internal eC_String encoding is UTF-8, no conversion whatsoever
    const eC_UTF16 *GetInternalUTF16() const; //< only valid to use if internal eC_String encoding is UTF-16, no conversion whatsoever
    const eC_Char *GetInternalASCII() const; //< only valid to use if internal eC_String encoding is ASCII, no conversion whatsoever

    /** GetArray is obsolete and will be removed from a future release of eC.
        Please do not use it anymore. GetInternal() functions shall be used instead.
    */
    inline eC_Char *GetArray() const                    { return m_acData.GetArray(); }

    // support stl like access
    inline eC_UInt length() const                       { return GetLength(); }
    inline const eC_Char *c_str() const                 { return m_acData.GetArray(); } // Warning: Be aware that you get access to a string stored in the internal representation, which might be ASCII, UTF-8 or UTF-16.
    inline void append(const char *str, int len )       { *this += eC_String( str, len); }
    inline void append(const char *str)                 { *this += str; }
    bool IsEmpty() const;
    inline void reserve(eC_UInt uiSize)                 { eC_Int iDif = uiSize - m_acData.GetQuantity(); if( iDif>0) { m_acData.SetQuantity(uiSize, true); memset(m_acData.GetArray(),0,iDif); }}
    inline void resize(eC_UInt uiCharacters);

    /** Get Byte position of provided character position regarding internal encoding type 
        @param pcSource Pointer to character-array
        @param uiCharacterIndex Index of character
        @param eFormat Internal used encoding of character-array
        @return The position in bytes
    */
    eC_UInt CharacterIndexToByteIndex(const eC_Char* pcSource, const eC_UInt uiCharacterIndex, eStringFormat_t eFormat) const;

    /** Get the internal string encoding.
        @return The internal string format(ASCII, UTF8 or UTF16).
    */
    eStringFormat_t GetFormat() const;

    /** Converts and copies the given UTF-8 string to the string object. Converting it to its internal encoding.
        @param pcSource Pointer to UTF-8 string.
        @return Amount of bytes of the converted string (excluding terminating 0).
    */
    eC_UInt FromUTF8(const eC_UTF8 *pcSource);

    /** Converts and copies the given UTF-16 string to the string object. Converting it to its internal encoding.
        @param pcSource Pointer to UTF-16 string.
        @return Amount of bytes of the converted string (excluding terminating 0).
    */
    eC_UInt FromUTF16(const eC_UTF16 *pcSource);

    /** Converts and copies the given ASCII string to the string object. Converting it to its internal encoding.
        @param pcSource Pointer to ASCII string.
        @return Amount of bytes of the converted string (excluding terminating 0).
    */
    eC_UInt FromASCII(const eC_Char *pcSource);

    /** Converts an Integer to string
        @param iNumber Number to convert.
    */
    void FromInt(const eC_Int &iNumber);

    /** Converts an Unsigned Integer to string
    @param iuNumber Number to convert.
    */
    void FromUInt(const eC_UInt &iuNumber);

    /** Converts an Float to string. Values bigger than 3.4020000E38 and smaller than -3.4020000E38f are not supported. 
        The decimal fraction is limited to 6 digits. Further digits will be rounded.
        @param fNumber Number to convert.
    */
    void FromFloat(const eC_Float &fNumber);

#ifndef eC_STRING_UNICODE_UTF32
    /** Converts and copies the given UNICODE string (encoded according to the platform's UNICODE encoding) to the string object. Converting it to its internal encoding.
        @param pcSource Pointer to UTF or ASCII string, depending on the preprocessor flag UNICODE.
        @return Amount of bytes of the converted string (excluding terminating 0).
    */
    eC_UInt FromUnicode(const eC_Unicode *pcSource)
    {
#ifdef eC_STRING_UNICODE_UTF8
        return FromUTF8(reinterpret_cast<const eC_UTF8*>(pcSource));
#endif
#ifdef eC_STRING_UNICODE_UTF16
        return FromUTF16(reinterpret_cast<const eC_UTF16*>(pcSource));
#endif
    }
#endif

    /** Converts and copies the internally stored string to the given UTF-8 buffer.
        Memory allocation for destination is up to the caller.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF-8 stream (excluding terminating 0).
    */
    eC_UInt ToUTF8(eC_UTF8 *pcDest) const;

    /** Converts and copies the internally stored string to a newly allocated UTF-8 buffer.
        Memory allocation for destination is done by this function, but deallocation is
        up to the caller! The caller should use "delete" to deallocate the buffer after use.
        @return Buffer consisting of UTF-8 data corresponding to the object. Must be
                deallocated by the caller to prevent memory leaks.
    */
    eC_UTF8 *ToUTF8_Alloc() const;

    /** Converts and copies the internally stored string to the given UTF-16 buffer.
        Memory allocation for destination is up to the caller.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF-16 stream (excluding terminating 00).
    */
    eC_UInt ToUTF16(eC_UTF16 *pcDest) const;

    /** Converts and copies the internally stored string to a newly allocated UTF-16 buffer.
        Memory allocation for destination is done by this function, but deallocation is
        up to the caller! The caller should use "delete" to deallocate the buffer after use.
        @return Buffer consisting of UTF-16 data corresponding to the object. Must be
                deallocated by the caller to prevent memory leaks.
    */
    eC_UTF16 *ToUTF16_Alloc() const;

    /** Converts and copies the internally stored string to the given ASCII buffer.
        Memory allocation for destination is up to the caller.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in ASCII stream (excluding terminating 0).
    */
    eC_UInt ToASCII(eC_Char * pcDest) const;

    /** Converts and copies the internally stored string to a newly allocated ASCII buffer.
        Memory allocation for destination is done by this function, but deallocation is
        up to the caller! The caller should use "delete" to deallocate the buffer after use.
        @return Buffer consisting of ASCII data corresponding to the object. Must be
                deallocated by the caller to prevent memory leaks.
    */
    eC_Char *ToASCII_Alloc() const;

#ifndef eC_STRING_UNICODE_UTF32
    /** Converts and copies the internally stored string to the given buffer encoded in the platform's unicode encoding.
        Memory allocation for destination is up to the caller.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in unicode stream (excluding terminating 0).
    */
    eC_UInt ToUnicode(eC_Unicode *pcDest) const;

    /** Converts and copies the internally stored string to a newly allocated buffer encoded in the platform's unicode encoding.
        Memory allocation for destination is done by this function, but deallocation is
        up to the caller! The caller should use "delete" to deallocate the buffer after use.
        @return Buffer consisting of unicode stream corresponding to the object. Must be
                deallocated by the caller to prevent memory leaks.
    */
    eC_Unicode *ToUnicode_Alloc() const;
#endif

    /** Converts this string to an integer.
        Behaviour is undefined if the converted value would be out of range.
        @param iRetVal An integer variable in which the result shall be stored.
        @return true if the conversion was successful.
    */
    eC_Bool ToInt(eC_Int & iRetVal) const;

    /** Converts this string to a float.
        Behaviour is undefined if the converted value would be out of range.
        @param fRetVal A float variable in which the result shall be stored.
        @return true if the conversion was successful.
    */
    eC_Bool ToFloat(eC_Float & fRetVal) const;

    /** Cut a string at a specified position or at a specified character.
        If no character is given:
          If the given length is larger or equal than the string length, this method
          returns the whole string. If a length is given, this method cuts the string
          at the specified point (measured in characters from the beginning of the string).
        If a character is given:
          This methods starts searching for the specified character starting at the
          specified point (in characters from the beginning of the string) and moving backward until
          the beginning of the string is reached or the character is found. The string
          is then cut either immediately to the right of the found character or
          (if not found) at position 0. Supply kString.GetLength() to search the entire string.
        @param uiLength Character index in this string at which to cut, or at which to start searching for"cUntilChar". See explanation above.
        @param pResultStr The resulting string (everything to the left of the point where the cut occurs).
        @param pRestStr The rest string (everything to the right of the point where the cut occurs).
        @param cUntilChar The character to search for.
        @return The length of the resulting string in bytes. If the string encoding is unknown, 0 is returned.
    */
    eC_UInt CutString(eC_UInt uiLength,eC_String* pResultStr, eC_String* pRestStr, eC_UTF32 utf32UntilChar = 0) const;

    /** Returns a substring of the string at a specified position and a given length OR until a specified character.
        If no character is given:
          The sub string from StartPosition to StartPosition + uiChars (including) is copied to the new string
        If a character is given:
          This methods starts searching for the specified character starting at offset uiStartPos.
          If either utf32UntilChar was found, or uiChars have been searched, the substring from uiStartPos to the
          current position is returned.
        @param uiStartPos    Character index of the first character which shall be copied to the new string
        @param uiChars       Maximum number of characters to be copied. (0 equals the entire string length) (if the utf32UntilChar is given and found, Substring will end there)
        @param utf32UntilChar A utf32 character which will terminate the new string (this character is not part of the returned string)
        @return The requested SubString
    */
    eC_String SubString(eC_UInt uiStartPos, eC_UInt uiChars = 0, eC_UTF32 utf32UntilChar = 0) const;

    /** Provides the Unicode of the character at 0 based index uiIndex. Format is unencoded 4 Byte Unicode Charset (UTF32)
        @param uiIndex the index of the character which is needed.
        @return utf32 character at given index, or 0 in case of an illegal character.
    */
    eC_UTF32 GetCharacterUTF32At(const eC_UInt uiIndex) const;

    /** Provides the charecter at 0 based index uiIndex.
        @param uiIndex the index of the character which is needed.
        @return emptyString if index was invalid, a string containing the character in case of success.
    */
    eC_String GetCharacterAt(const eC_UInt uiIndex) const;

    /** provides the size of the first character in the stream in bytes.
        @param pcSourceParam Pointer to stream.
        @param eStringFormat indicates the format of the stream (ASCII, UTF8, UTF16).
        @return size of character in byte, or 0 in case of an illegal character.
    */
    static eC_UInt GetCharacterSize(const eC_Char* pcSourceParam, eStringFormat_t eStringFormat);

    /** provides the decoded Unicode of the first character in the supplied stream.
        @param pcSourceParam Pointer to stream.
        @param eStringFormat indicates the format of the stream (ASCII, UTF8, UTF16).
        @return utf32 character, or 0 in case of an illegal character.
    */
    static eC_UTF32 GetCharacterUTF32(const eC_Char* pcSourceParam, eStringFormat_t eStringFormat);

    /** Allows to convert the string inplace using a functor passed as parameter.
    @param ConvFunc Functor class which is called to convert the internal string.
    */
    void ApplyCustomConversion(eC_StringConverter &ConvFunc);
protected:

    /// Array containing the raw byte-data of the string
    eC_TArray<eC_Char> m_acData;

    /** Assigns a buffer of raw data to the string. The raw string data MUST match the internal string format.
        @param pcString Pointer to raw string data (Must be nul-Terminated!)
        @param uiByteSize Size of pcString in bytes (including terminator)
        @bCopy bCopy If TRUE Data of pcString will be copied, if FALSE the new string will point to pcString
    */
    void AssignBuffer(eC_Char* pcString, eC_UInt uiByteSize, eC_Bool bCopy);

    /** Helper function for constructors.
        @param pcString Pointer to raw string data
        @param uiLength Maximum length (in characters) of resulting string (can be less than raw string data)
        @param eFormat Format in which raw string data shall be interpreted
    */
    void ConstructFromString(const eC_Char *pcString, eC_UInt uiLength, eStringFormat_t eFormat);

    /** provides the current length of the ASCII string in characters.
        @param pcSourceParam Pointer to string. <BR>
                             If NULL, which is the default, the length of the internal string <BR>
                             is returned, otherwise the length of the string passed as parameter.
        @return current string length in character (excluding the terminal character)
    */
    eC_UInt GetLengthASCII(const eC_Char* pcSourceParam = NULL) const;

    /** provides the current length of the UTF8 stream in characters.
        @param pcSourceParam Pointer to stream. <BR>
                             If NULL, which is the default, the length of the internal stream <BR>
                             is returned, otherwise the length of the stream passed as parameter.
        @return current string length in character (excluding the terminal character)
    */
    eC_UInt GetLengthUTF8(const eC_UTF8* pcSourceParam = NULL) const;

    /** provides the current length of the UTF16 stream in characters.
        @param pcSourceParam Pointer to stream. <BR>
                             If NULL, which is the default, the length of the internal stream <BR>
                             is returned, otherwise the length of the stream passed as parameter.
        @return current string length in character (excluding the terminal character)
    */
    eC_UInt GetLengthUTF16(const eC_UTF16* pcSourceParam = NULL) const;

    /** provides the current size of the ASCII string in bytes.
        @param pcSourceParam Pointer to string. <BR>
                             If NULL, which is the default, the length of the internal string <BR>
                             is returned, otherwise the length of the string passed as parameter.
        @return current string length in byte (excluding the terminal character)
    */
    eC_UInt GetSizeASCII(const eC_Char* pcSourceParam = NULL) const;

    /** provides the current size of the UTF8 stream in bytes.
        @param pcSourceParam Pointer to stream. <BR>
                             If NULL, which is the default, the size of the internal stream <BR>
                             is returned, otherwise the size of the stream passed as parameter.
        @return current string length in byte (excluding the terminal character)
    */
    eC_UInt GetSizeUTF8(const eC_UTF8* pcSourceParam = NULL) const;

    /** provides the current size of the UTF16 stream in bytes.
        @param pcSourceParam Pointer to stream. <BR>
                             If NULL, which is the default, the size of the internal stream <BR>
                             is returned, otherwise the size of the stream passed as parameter.
        @return current string length in byte (excluding the terminal character)
    */
    eC_UInt GetSizeUTF16(const eC_UTF16* pcSourceParam = NULL) const;

    /** Converts an ASCII-String to UTF-8.
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to ASCII string to convert.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF-8 stream (excluding terminating 0).
    */
    eC_UInt ASCIIToUTF8(const eC_Char *pcSource, eC_UTF8 *pcDest) const;

    /** Converts a UTF-8 stream to ASCII-String.
        Memory allocation for destination is up to the caller.
        In case of the character that can not be converted, it will be discarded,
        @param pcSource Pointer to UTF-8 source stream to convert.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>.
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in ASCII-String (excluding terminate character 0).
    */
    eC_UInt UTF8ToASCII(const eC_UTF8 *pcSource, eC_Char *pcDest) const;

    /** Converts a UTF-16 stream to UTF-8.
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to UTF-16 stream.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF8 stream (excluding terminate character 0).
    */
    eC_UInt UTF16ToUTF8(const eC_UTF16 *pcSource, eC_UTF8 *pcDest) const;

    /** Converts a UTF-8 stream to a UTF-16 stream.
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to UTF-8 stream.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>.
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF16 stream (excluding terminate character 00).
    */
    eC_UInt UTF8ToUTF16(const eC_UTF8 *pcSource, eC_UTF16 *pcDest) const;

    /** Converts an ASCII String to a UTF-16 stream.
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to UTF-16 stream.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>.
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in UTF16 stream (excluding terminate character 00),
    */
    eC_UInt ASCIIToUTF16(const eC_Char *pcSource, eC_UTF16 *pcDest) const;

    /** Copies an UTF-8 stream from source to destination.<BR>
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to ASCII string.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>.
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in ASCII string (excluding terminate character 0).
    */
    eC_UInt ASCIIToASCII(const eC_Char *pcSource, eC_Char *pcDest) const;

    /** Copies an UTF-8 stream from source to destination.<BR>
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to UTF-8 stream.
        @param pcDest   Pointer to destination.<BR>
                        If NULL, then only the amount of bytes required  <BR>
                        for destination buffer is returned.
        @return Amount of bytes in UTF-8 stream (excluding terminating 0).
    */
    eC_UInt UTF8ToUTF8(const eC_UTF8 *pcSource, eC_UTF8 *pcDest) const;

    /** Copies an UTF-16 stream from source to destination.<BR>
        Memory allocation for destination is up to the caller.
        @param pcSource Pointer to UTF-16 stream.
        @param pcDest   Pointer to memory destination.<BR>
                        If NULL, then only the amount of bytes required  <BR>
                        for destination buffer is returned.
        @return Amount of bytes in UTF-16 stream (excluding terminating 00).
    */
    eC_UInt UTF16ToUTF16(const eC_UTF16 *pcSource, eC_UTF16 *pcDest) const;

    /** Converts a UTF-16 stream to ASCII. Only bytes < 0xFF can be treated. <BR>
        Bytes > 0xFF are being skipped.<BR>
        Memory allocation for destination is up to the caller.
        @param pcDest Pointer to UTF-16 stream.
        @param pcDest   Pointer to memory where to put the converted string to.<BR>
                        If NULL, then only the amount of bytes required for <BR>
                        destination buffer is returned.
        @return Amount of bytes in ASCII stream (excluding terminating 0).
    */
    eC_UInt UTF16ToASCII(const eC_UTF16 *pcSource, eC_Char *pcDest) const;
};

/** Abstract base class for string conversion functor used by ApplyCustomConversion.
To use it derive yuor own functor class and implement the operator() with
your own conversion function. The operator() will be called from ApplyCustomConversion.
*/
class eC_StringConverter
{
public:
    /** Constructor
    */
    eC_StringConverter() {};
    /** Conversion functor method.
    @param kData Direct access to the eC_String internal eC_TArray which contains the
    characters. The encoding depends on the next parameter.
    @param eFormat The internal format of the string.
    @param uiLength length of the string in characters.
    */
    virtual void operator() (eC_TArray<eC_Char> &kData, eC_String::eStringFormat_t eFormat, eC_UInt uiLength) = 0;
};


#endif
