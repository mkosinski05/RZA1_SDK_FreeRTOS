/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDECODERGIF__H_
#define GUIIMAGEDECODERGIF__H_

#include "gif_lib.h"

#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"

class CGUIImageData;


/// Class GUIImageDecoderGIF which loads an image and returns a pointer to a GUIImageData object.

class CGUIImageDecoderGIF:
    public CGUIImageDecoder
{
public:
    CGUIImageDecoderGIF();
    ~CGUIImageDecoderGIF();
    eC_Bool Init(eC_File *pkFile);
    eC_UInt GetWidth() {return m_uiWidth;}
    eC_UInt GetHeight(){return m_uiHeight;}
    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);
    eC_Bool HasAlpha(){return m_bHasAlpha;}

private:
    eC_UInt m_uiWidth;
    eC_UInt m_uiHeight;
    eC_Bool m_bHasAlpha;
    GifFileType *m_pGFT;
    static int ReadFile( GifFileType *, GifByteType *buffer, int length);
    eC_Bool Load2(CGUIImageData &pImageData);
};

#endif
