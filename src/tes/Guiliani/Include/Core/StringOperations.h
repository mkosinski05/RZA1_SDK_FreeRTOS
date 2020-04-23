/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(STRING_OPERATIONS_H)
#define STRING_OPERATIONS_H

#include "eC_Types.h"
#include "eC_TList_doubleLinked.h"

class eC_String;

typedef eC_TListDoubleLinked<eC_String> StringList;

/// Provides string manipulation functions.
namespace StringOperations
{

/** Replaces one or more characters in a string.

    Example:
@code
    eC_String text("123456");
    ReplaceCharacters(text, 3, 2, "9");
    // text now contains "12956"
@endcode

    @param[in,out] rkText The string to be modified.
    @param[in] uiPos Start position at which characters are replaced.
    @param[in] uiLen Number of characters to be replaced.
    @param[in] rkReplacement Text to replace the characters with.
  */
void ReplaceCharacters(eC_String& rkText, eC_UInt uiPos, eC_UInt uiLen,
    const eC_String& rkReplacement);

/** Replaces all occurrences of a given substring by another string.

    Example:
    @code
    eC_String text("abc abc abc");
    ReplaceAll(text, "ab", "D");
    // text now contains "Dc Dc Dc"
    @endcode

    @param[in,out] rkText The string to be modified.
    @param[in] rkSearchText The string of which all occurrences ought to be
               replaced.
    @param[in] rkReplacement The string with which the search text is
               replaced.
  */
void ReplaceAll(eC_String& rkText, const eC_String& rkSearchText,
    const eC_String& rkReplacement);

/** Replaces all occurrences of special characters by the according XML
    predefined entities:
    - @verbatim & becomes &amp;  @endverbatim
    - @verbatim < becomes &lt;   @endverbatim
    - @verbatim > becomes &gt;   @endverbatim
    - @verbatim ' becomes &apos; @endverbatim
    - @verbatim " becomes &quot; @endverbatim
    @param[in,out] rkText The string to be modified.
  */
void EscapeXML(eC_String& rkText);

/** Replaces all occurrences of XML predefined entities by the according
    characters:
    - @verbatim &amp;  becomes & @endverbatim
    - @verbatim &lt;   becomes < @endverbatim
    - @verbatim &gt;   becomes > @endverbatim
    - @verbatim &apos; becomes ' @endverbatim
    - @verbatim &quot; becomes " @endverbatim
    @param[in,out] rkText The string to be modified.
  */
void UnescapeXML(eC_String& rkText);

/** Splits a string up into a list of substrings.
    The given separator character specifies where to split up the original string.
    @param ckString Original string which shall be split up into substrings
    @param ccSeparator Special character which separates the individual substrings within the original string
    @return List of substrings (they will not include any separators) */
StringList TokenizeString( const eC_String& ckString, const eC_Char ccSeparator);

}

#endif // STRING_OPERATIONS_H
