/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDECODER__H_
#define GUIIMAGEDECODER__H_

#include "eC_Types.h"
#include "eC_File.h"
#include "eC_TList_doubleLinked.h"
#include "eC_String.h"
#include "GUIImageData.h"


/// Base class for the image decoders.

/** All decoder classes must be derived from this class. */

class CGUIImageDecoder
{
public:
    /** Virtual destructor. */
    virtual ~CGUIImageDecoder()
    {
    }

    /** Must be implemented in each derived class to decode an image.
        @param kImageData The decoded image has to be stored here by each
               derived implementation.
        @param pTmpImgFile Pointer to image file to load.
        @return Implementations must return True if the image was decoded
                successfully, False if anything went wrong and the contents of
                kImageData are invalid.
    */
    virtual eC_Bool LoadImg(CGUIImageData &kImageData, eC_File *pTmpImgFile) = 0;

    eC_TListDoubleLinked<eC_String>& GetSupportedFileExtensions()
    {
        return m_kSupportedFileExtensions;
    }

protected:
    /// Struct which holds the information about the supported image formats of the decoder.
    /**
        You must explicit set the supported image format to true, otherwise it will be ignored by the
        loader.
    */
    struct SupportedImageTypes_t
    {
        CGUIImageData::ImageType_t eImgType;///< Image type
        eC_Bool bSupported; ///< True if supported
    };

    /** Constructor of CGUIImageDecoder, which sets all supported image types to False. */
    CGUIImageDecoder()
    {
        for (eC_UInt i = 0; i<sizeof(m_kSupportedImageTypes)/sizeof(m_kSupportedImageTypes[0]); i++)
        {
            m_kSupportedImageTypes[i].bSupported = false;
        }
        m_kSupportedImageTypes[0].eImgType = CGUIImageData::IMAGE_FORMAT;
        m_kSupportedImageTypes[1].eImgType = CGUIImageData::ALPHA_8;
        m_kSupportedImageTypes[2].eImgType = CGUIImageData::RGB_565;
        m_kSupportedImageTypes[3].eImgType = CGUIImageData::BGR_565;
        m_kSupportedImageTypes[4].eImgType = CGUIImageData::ABGR_4444;
        m_kSupportedImageTypes[5].eImgType = CGUIImageData::BGRA_4444;
        m_kSupportedImageTypes[6].eImgType = CGUIImageData::RGB_888;
        m_kSupportedImageTypes[7].eImgType = CGUIImageData::BGR_888;
        m_kSupportedImageTypes[8].eImgType = CGUIImageData::ARGB_8888;
        m_kSupportedImageTypes[9].eImgType = CGUIImageData::ABGR_8888;
        m_kSupportedImageTypes[10].eImgType = CGUIImageData::RGBA_8888;
        m_kSupportedImageTypes[11].eImgType = CGUIImageData::BGRA_8888;
        m_kSupportedImageTypes[12].eImgType = CGUIImageData::RGB_8888;
    }

    /** Swaps 16-bit endianess of the referenced value.
    @param[in,out] ubData Given value.
    */
    inline void SwapEndianess16(eC_UShort &ubData)
    {
        eC_UShort usTmp = ubData;
        ubData= ((usTmp&0x00FF)<<8)|((usTmp&0xFF00)>>8);
    }

    /** Swaps 32-bit endianess of the referenced value.
    @param[in,out] ubData Given value.
    */
    inline void SwapEndianess32(eC_UInt &uiData)
    {
        eC_UInt uiTmp = uiData;
        uiData = (((uiTmp&0x000000FF)<<24)|((uiTmp&0x0000FF00)<<8)|((uiTmp&0x00FF0000)>>8)|((uiTmp&0xFF000000)>>24));
    }

    SupportedImageTypes_t m_kSupportedImageTypes[cuiNumOfImageTypes];///< Array of supported image types
    eC_TListDoubleLinked<eC_String> m_kSupportedFileExtensions;
};

#endif
