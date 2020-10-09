/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDECODERBMP__H_
#define GUIIMAGEDECODERBMP__H_

#include "eC_Types.h"
#include "eC_String.h"
#include "eC_File.h"
#include "GUIImageData.h"

#include "GUIImageDecoder.h"


/// Class GUIImageDecoderBMP which loads an image and returns a pointer to a GUIImageData object.

class CGUIImageDecoderBMP :
    public CGUIImageDecoder
{
public:
    CGUIImageDecoderBMP();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File* pTempImageFile);

private:
    /** Reverts the bytes of the whole data pointer.
    ATTENTION: If use this method, please ensure that
    the pubData has really the given length.
    @param pubData pointer to the data.
    @param uiLen Length of the data pointed to in bytes.
    */
    inline void RevertBytes(eC_UByte* pubData, eC_UInt uiLen)
    {
        if (pubData)
        {
            eC_UInt uiCount1 = 0;
            eC_UInt uiCount2 = uiLen - 1;
            while (uiCount1<uiCount2)
            {
                eC_UByte uiTmpCount1 = pubData[uiCount1];
                eC_UByte uiTmpCount2 = pubData[uiCount2];
                pubData[uiCount1] = uiTmpCount2;
                pubData[uiCount2] = uiTmpCount1;
                uiCount1++;
                uiCount2--;
            }
        }
    }

    /** Struct for the Bitmap header. */
    struct BitmapHeader_t
    {
        char acType[2];
        unsigned char acSize[4];
        char acR1[4];
        char acOffBits[4];
    };

    /** Struct for the Bitmap info */
    struct BitmapInfo_t
    {
        unsigned long ulSize;
        long lWidth;
        long lHeight;
        unsigned short usPlanes;
        unsigned short usBitCount;
        unsigned long ulCompression;
        unsigned long ulSizeImage;
        long lXPPM;
        long lYPPM;
        unsigned long ulCU;
        unsigned long ulCI;
    };
};

#endif
