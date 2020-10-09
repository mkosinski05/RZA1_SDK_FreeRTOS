/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef RGAIMAGEDATA_H_
#define RGAIMAGEDATA_H_

#include "eC_Types.h"
#include "eC_File.h"
#include "RGA_Platform.h"
#include "RGA.h"

#define PNG_WIDTH_BYTE_POS       0x10
#define PNG_HEIGHT_BYTE_POS      0x14
#define PNG_COLOR_TYPE_BYTE_POS  0x19
#define PNG_COLOR_TYPE_GA        0x4 // Grayscale with alpha
#define PNG_COLOR_TYPE_RGBA      0x6 // RGB with alpha
#define JPEG_OFFSET_TO_WIDTH     0x5

/// Contains information about an image loaded by CGfxWrapRGA.
/** The graphics wrapper CGfxWrapRGA creates an object of this class. This class can decode images of PNG and JPEG
    format. RGA for RZA1H uses hardware JPEG Codec Unit (JCU) for decompression of JPEG images and libPNG for
    decompression of PNG images whereas RGA for RZA1L uses libjpeg for decompression of JPEG images. So the entire
    data of JPEG or PNG image file can be directly specified as an argument to drawing functions of RGA.
    The only Purpose of this class is to calculate width and height and detect alpha channel of the JPEG or PNG images,
    which are required by Guiliani to blit the images. No image format conversions are performed here.
  */
class CRGAImageData
{
public:

    /** CRGAImageData constructor
        @param pTempImageFile Image file to be decoded.
     */
    CRGAImageData(eC_File *pTempImageFile = NULL);

    /** CRGAImageData destructor */
    ~CRGAImageData();

    ///@return The image format
    inline pixel_format_t GetImageType() const {return m_ePixelFormat;}

    ///@return Pointer to image data
    inline eC_UByte* GetData() const {return m_pubImageData;}

    ///@return The width of the image
    inline eC_UInt GetWidth() const {return m_uiWidth;}

    ///@return The height of the image
    inline eC_UInt GetHeight() const {return m_uiHeight;}

    /** Returns whether the image has alpha channel or not.
        @return True if the image has alpha data, otherwise False.
     */
    inline eC_Bool HasAlpha() const {return m_bHasAlpha;}

    /** Checks the format of the image and sets the data for image argument of
     *  draw functions of RGA.
        @return True if the image format is valid, otherwise False.
     */
    eC_Bool DecodeImage();

private:
    eC_UByte  *m_pubImageData; ///< Pointer to the image data
    eC_UInt    m_uiWidth; ///< The width of the image
    eC_UInt    m_uiHeight; ///< The height of the image
    eC_Bool    m_bHasAlpha; ///< Status of alpha channel in the image
    pixel_format_t m_ePixelFormat; ///< Pixel format of the image
    eC_File   *m_pkFile; ///< The image file to load and decode
};

#endif /* RGAIMAGEDATA_H_ */
