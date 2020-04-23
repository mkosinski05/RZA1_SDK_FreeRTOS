/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_BASE64_H)
#define GUI_BASE64_H

#include "eC_Types.h"

/**
 * All binary input is supported, not just ASCII.
 * @brief Base64 encoder/decoder for Guiliani.
 */
class CGUIBase64
{
public:
    /** Returns the maximum size of a Base64 encoded datapackage
        Base64 encodes three bytes into four bytes. The output is padded to a multiple of four. */
    #define GETBASE64BUFFERSIZE(DataSize) (((DataSize+2)/3)*4)

    /**
     Encodes a stream of values to a base64 data stream.
    
     To encode a value stream (pubIn) to a base64 stream, it is necessary to
     allocate the output buffer before.
    
     The output buffer's (uiBufLen) size should be at least
     <i>uiSrcLen * 4 / 3 + 4Byte-Padding</i> bytes.
     You can use the GETBASE64BUFFERSIZE helper macro to get a sufficient size.
    
     @param pcBuf [out] Pointer to the preallocated output buffer. After
      this method finished, this buffer contains the zero terminated base64
      string.
     @param uiBufLen The size of the output buffer in bytes.
     @param pubIn Input buffer to encode.
     @param uiSrcLen Number of characters of the input buffer to be encoded.
     @return Pointer to output buffer (same as pcBuf).
    */
    static eC_Char* Encode(eC_Char* pcBuf,
        const eC_UInt uiBufLen,
        const eC_UByte* pubIn,
        const eC_UInt uiSrcLen);

    /**
     Decodes a base64 data stream to a stream of characters.
    
     The output buffer's (pubBuf) size should be at least
     <i>strlen(pcIn) * 3 / 4</i> bytes. Check return value afterwards
     to get the exact number of decoded bytes.
    
     @param pubBuf [out] Output buffer containing decoded data.
     @param uiBufLen The size of the output buffer in bytes.
     @param pcIn Input base64 data stream.
     @return Number of bytes decoded to pubBuf, or -1 in case of error
    */
    static eC_Int Decode(eC_UByte* pubBuf,
        const eC_UInt uiBufLen,
        const eC_Char* pcIn);

private:
    /** Decoding character map. */
    static const eC_UByte sc_map[];
};

#endif // GUI_BASE64_H
