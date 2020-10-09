/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_VALUE__H_
#define GUI_VALUE__H_

#include "GUIStreamableObject.h"
#include "eC_Types.h"
#include "eC_String.h"

/**
    It provides conversion of the stored value between the supported types and
    comparison.

    For the supported types see GUIValue_t.
    @brief CGUIValue stores a single value in one specific type.
*/
class CGUIValue: public CGUIStreamableObject
{
public:

    /** Constructs a CGUIValue.
        The initial value is an integer 0.
    */
    CGUIValue();

    /// Constructs a CGUIValue from boolean.
    CGUIValue(const eC_Bool bValue);
    /// Constructs a CGUIValue from byte.
    CGUIValue(const eC_Byte bValue);
    /// Constructs a CGUIValue from unsigned byte.
    CGUIValue(const eC_UByte ubValue);
    /// Constructs a CGUIValue from character.
    CGUIValue(const eC_Char cValue);
    /// Constructs a CGUIValue from wide character.
    CGUIValue(const eC_UTF16 wcValue);
    /// Constructs a CGUIValue from short.
    CGUIValue(const eC_Short sValue);
    /// Constructs a CGUIValue from long.
    CGUIValue(const eC_Int iValue);
    /// Constructs a CGUIValue from unsigned long.
    CGUIValue(const eC_UInt uiValue);
    /// Constructs a CGUIValue from unsigned integer.
    CGUIValue(const unsigned int uiValue);
    /// Constructs a CGUIValue from integer.
    CGUIValue(const int uiValue);

    /** Constructs a CGUIValue from float.
        The value is stored as eC_Float, so there is a possible loss of precision.
        @param dValue The value to construct from
    */
    CGUIValue(const double dValue);

    /** Constructs a CGUIValue from c-string. The string is interpreted as ASCII.
        The type of the constructed CGUIValue is EC_STRING.
        @param pcStringValue The value to construct from
    */
    CGUIValue(const eC_Char* pcStringValue);

    /** Constructs a CGUIValue from string. The string format stays unchanged.
        @param StringValue The value to construct from
    */
    CGUIValue(const eC_String& StringValue);



    /// This enumeration indicates the type (format) of the CGUIValue.
    enum GUIValue_t
    {
        EC_INT,
        EC_FLOAT,
        EC_STRING,
        EC_HEX,
        EC_BOOL,
        EC_BYTE,
        EC_UBYTE,
        EC_CHAR,
        EC_WCHAR,
        EC_SHORT,
        EC_USHORT,
        EC_UINT,
        EC_VALUE,
        EC_FIXED
    };

    /** Returns the current type of the stored value. This means that the value
        has been constructed as or converted to this type and the according
        To[type] method will not have to convert the value again.
        @return The current type of this value.
      */
    inline GUIValue_t GetType() const {return m_eType;}

    /** Converts the current value.
        @param eType The type to convert to.
        @return True if the conversion was successful, otherwise False.
    */
    eC_Bool ConvertTo(const GUIValue_t& eType);

    /** Checks if the current type is convertible to another type.
        The values are not checked. The check is only done based on the types.
        @param eType The type to convert to.
        @return True if the types can be converted.

    */
    eC_Bool IsConvertibleTo(const GUIValue_t& eType) const;

    /** Converts the current value to a boolean.
        All numerical values not equal to zero are true.
        A string is True if it is equal to ms_TrueString.
        @return the boolean value.
    */
    eC_Bool ToBool() const;

    /**
        Converts the current value to a byte.
        For characters and strings the string value is converted to integer
        and then cut to 8 bit value.
        @return the signed byte value.
    */
    eC_Byte ToByte() const;

    /**
        Converts the current value to an unsigned byte.
        For characters and strings the string value is converted to integer
        and then cut to 8 bit value.
        @return the unsigned byte value.
    */
    eC_UByte ToUByte() const;

    /**
        Converts the current value to a character by converting the current
        value to an ASCII string (if necessary) and returning the first
        character.
        @return The converted value, 0 if the value could not be converted.
    */
    eC_Char ToChar() const;

    /**
        Converts the current value to a UTF-16 character by converting the
        current value to an UTF-16 string (if necessary) and returning the
        first character.
        @return The converted value, 0 if the value could not be converted.
    */
    eC_UTF16 ToWChar() const;

    /**
        Converts the current value to a signed short.
        For characters and strings the string value is converted to integer
        and then cut to 16 bit value.
        @return the signed short value.
    */
    eC_Short ToShort() const;

    /**
        Converts the current value to an unsigned short.
        For characters and strings the string value is converted to integer
        and then cut to 16 bit value.
        @return the unsigned short value.
    */
    eC_UShort ToUShort() const;

    /**
        Converts the current value to a signed integer.
        For characters and strings the string value is converted to integer
        and then cut to 32 bit value.
        @return the signed integer value.
    */
    eC_Int ToInt() const;

    /**
        Converts the current value to an unsigned integer.
        For characters and strings the string value is converted to integer
        and then cut to 32 bit value.
        @return the unsigned integer value.
    */
    eC_UInt ToUInt() const;

    /**
        Converts the current value to a float.
        For characters and strings the string value is converted to float.
        @return the float value.
    */
    eC_Float ToFloat() const;

    /**
        Converts the current value to a eC_Value.
        For characters and strings the string value is converted to float
        and then transformed to eC_Value.
        @return the eC_Value.
    */
    eC_Value ToValue() const;

    /**
        Converts the current value to a fix point value.
        For characters and strings the string value is converted to float
        and then transformed to fix point representation.
        @return the fix point value.
    */
    eC_Fixed ToFix() const;

    /**
        Converts the current value to a string.
        For characters and strings the string value is converted to float
        and then transformed to fix point representation.
        @return the String representation of the current value.
    */
    eC_String ToString() const;

    /**
       Sets the value and type.
       @param Value The value to set.
       @return Reference to the new Value.
    */
    CGUIValue& operator= (const CGUIValue& Value);

#ifdef GUILIANI_STREAM_GUI
    /**
        Reads a CGUIValue object from stream.
        @see CGUIFactory
    */
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    /**
        Writes all object attributes to the streaming file. StreamWriter has to be initialized.
        Only for use with GUIStreamWriter.
        @param bWriteClassID This flag is used to select if writing of command
               class ID, leading and trailing tags is performed.
        @see CGUIStreamWriter
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

private:

    /// Used for representing boolean value 'false' in Strings.
    static const eC_String ms_FalseString;
    /// Used for representing boolean value 'true' in Strings.
    static const eC_String ms_TrueString;

    /// holds the type of the stored data.
    GUIValue_t  m_eType;
    /// holds the string value if m_eType is EC_STRING.
    eC_String   m_kStringValue;
    /// union used to store base data types.
    union BaseType_t
    {
        eC_Bool boolValue;
        eC_Byte byteValue;
        eC_UByte uByteValue;
        eC_Char cValue;
        eC_UTF16 wcValue;
        eC_Short sValue;
        eC_UShort usValue;
        eC_Int iValue;
        eC_UInt uiValue;
        eC_Float fValue;
        eC_Value vValue;
        eC_Fixed fixValue;
    };
    /// holds the current value if m_eType is not equal to EC_STRING.
    BaseType_t m_BaseTypeValue;
};

/**
    Compares the current value with another value.
    The parameter is converted to the current type
    and then compared to the currently stored one.
    @param Value1 The value to compare against Value2.
    @param Value2 The value to compare against Value1.
    @return True if the converted value of the parameter
            is equal to the current one, otherwise False.
*/
bool operator== (const CGUIValue& Value1, const CGUIValue& Value2);

/**
    Compares the current value with another value.
    The parameter is converted to the current type
    and then compared to the currently stored one.
    @param Value1 The value to compare against Value2.
    @param Value2 The value to compare against Value1.
    @return True if the converted value of the parameter
            is <i>not</i> equal to the current one, otherwise False.
*/
bool operator!= (const CGUIValue& Value1, const CGUIValue& Value2);

#endif // GUI_VALUE__H_
