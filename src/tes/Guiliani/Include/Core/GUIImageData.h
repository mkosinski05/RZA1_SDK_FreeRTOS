/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGEDATA__H_
#define GUIIMAGEDATA__H_

#include "eC_Types.h"

// TODO: concept for supporting vector and bitmap-images
//new for SVGs
#include <vector>
#include "SVGShape.h"

/// The number of entries in the ImageType_t enum. It's very important to set this always to the right number of entries!
const eC_UInt cuiNumOfImageTypes = 13;

/// Contains information about an image loaded by CGUIImageLoader.
/** Typical use case for this class is that a graphics wrapper creates an
    object of this class, sets some attributes to make sure the image uses the
    correct format etc., then passes it to the image loader which hands it over
    to the individual image decoders that fill the data section with the
    decoded image data.
  */
class CGUIImageData
{
public:
    /** Enumeration ImageType_t for the loaded format.<br/>
        A = alpha, R = red, G = green, B = blue. The order of the letters in the abbreviation defines the order of a pixel.
        The number means the number of bits of each color channel.<br/>
        For example: RGBA_8888 means Red, Green, Blue, Alpha and every channel has 8 bits. So
        the whole pixel has 32 bits. Red has the most significant bit and alpha the least significant bits.
        The order is considered big endian so bytewise read, it forms 0xAA 0xRR 0xGG 0xBB.
        */
    enum ImageType_t
    {
        IMAGE_FORMAT=0, ///< Original format of the image.

        // Trick: We use the upper byte to indicate how many bytes are used per pixel.
        // 1 byte per pixel.
        ALPHA_8 = 1<<24, ///< 8-bit alpha

        // 2 bytes per pixel.
        RGB_565 = 2<<24, ///< 16-bit RGB565
        BGR_565, ///< 16-bit BGR565
        ABGR_4444, ///< 16-bit ABGR4444
        BGRA_4444, ///< 16-bit BGRA4444

        // 3 bytes per pixel.
        RGB_888 = 3<<24, ///< 24-bit RGB888
        BGR_888, ///< 24-bit BGR888

        // 4 bytes per pixel.
        ARGB_8888 = 4<<24,///< 32-bit ARGB8888
        ABGR_8888,///< 32-bit ABGR8888
        RGBA_8888,///< 32-bit RGBA8888
        BGRA_8888,///< 32-bit BGRA8888
        RGB_8888///< 32-bit RGB8888 (no alpha)

        // When changing the number of elements be sure to adapt cuiNumOfImageTypes.
    };

    /** CGUIImageData constructor */
    CGUIImageData();

    /** CGUIImageData destructor */
    ~CGUIImageData();

    /** Sets the width of the image. Should only be called by an image decoder.
        @param uiWidth Width of the image.
      */
    inline void SetWidth(const eC_UInt &uiWidth)                             {m_uiWidth = uiWidth;}

    /** Sets the height of the image. Should only be called by an image decoder.
        @param uiHeight Height of the image.
      */
    inline void SetHeight(const eC_UInt &uiHeight)                           {m_uiHeight = uiHeight;}

    /** Sets the data pointer of the image. Should only be called by an image
        decoder.
        @param *pubData Pointer to memory to be used for storing a decoded image.
      */
    inline void SetData(eC_UByte *pubData)                                     {m_pubData = pubData;}

    /** Sets the data-pointer of an SVG-image. Should only be called by an image.
    @param pkSVGShapes Pointer to std::vector<CSVGShape>.
    @todo calculae size and m_pubData has to be set but won't work with this approach.
    */
    inline void SetDataSVG(std::vector<CSVGShape>* pkSVGShapes)
    {
        m_pkSVGVector = pkSVGShapes;
    }

    /** Gets the data pointer of an SVG-image.
    @param pkSVGShapes Pointer to std::vector<CSVGShape>.
    @todo calculae size!
    */
    inline std::vector<CSVGShape>* GetDataSVG()
    {
        return m_pkSVGVector;
    }

    /** Sets the 'filler' value. It is typically used by decoders that load
        non-alpha images but store it in an alpha-enabled format. This
        'filler' is then used as the alpha value.
        This method should be called before CGUIImageDecoder::LoadImg().
        @param ubFillerByte Value used for the alpha filler.
      */
    inline void SetFiller(const eC_UByte &ubFillerByte) {m_ubFillerByte = ubFillerByte;}

    /** Sets the image format type.
        @param eImageType The format type.
      */
    inline void SetImageType(const ImageType_t &eImageType) {m_eImageType = eImageType;}

    /** Sets the destructor's behaviour.
        @param bDeleteDataOnDestruction Pass true to have the destructor delete
               the decoded image data.
      */
    inline void SetDeleteDataOnDestruction(const eC_Bool bDeleteDataOnDestruction) {m_bDeleteData = bDeleteDataOnDestruction;}

    /// @return The width of the image.
    inline eC_UInt GetWidth() const {return m_uiWidth;}

    /// @return The height of the image.
    inline eC_UInt GetHeight() const {return m_uiHeight;}

    /// @return The format type of the image.
    inline ImageType_t GetImageType() const {return m_eImageType;}

    /** @return Value of the 'filler' byte.
        For an explanation of this value, see SetFiller().
      */
    inline eC_UByte GetFiller() const {return m_ubFillerByte;}

    /// @return Pointer to the decoded image data.
    inline eC_UByte* GetData() const {return m_pubData;}

    /// @return The bit depth of the image.
    inline eC_UByte GetBytePerPixel() const {return (m_eImageType>>24);}

    /// @return True if this image data deletes its contents upon destruction.
    inline eC_Bool GetDeleteDataOnDestruction() const {return m_bDeleteData;}

    /** Returns the size (in bytes) of this image.
        Width, height and bits per pixel must have been set correctly before.
        @return Image size in bytes.
    */
    inline eC_UInt GetImageSize() const { return m_uiWidth * m_uiHeight * (m_eImageType>>24); }

    /** Returns whether the image has alpha channel or not.
        @return True if the image has alpha data, otherwise False.
    */
    eC_Bool HasAlpha() const;

    /** Deletes the image data array. */
    void DeleteData();

    /** Operator= method. This does a flat copy of the given source CGUIImageData object, which means that both
        GUIImageData objects will then reference the SAME DATA in memory.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIImageData& operator=(const CGUIImageData& kSource)
    {
        m_uiWidth= kSource.m_uiWidth;
        m_uiHeight= kSource.m_uiHeight;
        m_eImageType= kSource.m_eImageType;
        m_ubFillerByte= kSource.m_ubFillerByte;
        m_pubData= kSource.m_pubData;
        m_bDeleteData= kSource.m_bDeleteData;
        m_pkSVGVector = kSource.m_pkSVGVector;
        return (*this);
    }

private:
    /** Copy-constructor. Do not use.
        Dummy declaration without implementation, just to hide it.
        @param kSource Source object to be copied.
    */
    CGUIImageData(const CGUIImageData& kSource);

    eC_UInt                      m_uiWidth;         ///< Width.
    eC_UInt                      m_uiHeight;        ///< Height.
    ImageType_t                  m_eImageType;      ///< Format.

    /// Filler byte replaces the alpha value, if you load an image with alpha, but you don't want to use it.
    eC_UByte                     m_ubFillerByte;

    eC_UByte*                    m_pubData;         ///< Image data.
    eC_Bool                      m_bDeleteData;     ///< should the data be deleted on destruction?

    std::vector<CSVGShape>*       m_pkSVGVector;
};

#endif
