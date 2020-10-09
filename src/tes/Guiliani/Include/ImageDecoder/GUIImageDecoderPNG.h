/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDECODERPNG__H_
#define GUIIMAGEDECODERPNG__H_


#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"

class CGUIImageData;


/// Loads an image and returns a pointer to a GUIImageData object.

class CGUIImageDecoderPNG:
    public CGUIImageDecoder
{
public:
    CGUIImageDecoderPNG();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);
};

#endif
