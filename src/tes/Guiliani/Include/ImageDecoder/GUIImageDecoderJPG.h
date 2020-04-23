/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDECODERJPG__H_
#define GUIIMAGEDECODERJPG__H_


#include "eC_Types.h"
#include "eC_String.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"


class CGUIImageData;
struct jpeg_decompress_struct;
struct jpeg_error_mgr;


/// Loads an image and returns a pointer to a GUIImageData object.

class CGUIImageDecoderJPG : public CGUIImageDecoder
{
public:
    /// Constructor.
    CGUIImageDecoderJPG();

    /// Destructor
    virtual ~CGUIImageDecoderJPG();

    /** Attempts to load an image.
        @param rkImageData [out] Receives the decoded image data.
        @param pTempImageFile The opened image file (should be a JPEG image).
    */
    virtual eC_Bool LoadImg(CGUIImageData &rkImageData, eC_File *pTempImageFile);

    static eC_File* m_pFile;
    static eC_UByte* m_pubBuffer;
    static eC_Bool m_bStartOfFile;

private:
    /// This struct contains the JPEG decompression parameters and pointers to working space (which is allocated as needed by the JPEG library).
    jpeg_decompress_struct* m_psInfo;
    jpeg_error_mgr* m_psJErr;
};
#endif
