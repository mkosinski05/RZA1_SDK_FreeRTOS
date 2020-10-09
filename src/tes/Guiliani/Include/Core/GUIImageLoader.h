/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGELOADER__H_
#define GUIIMAGELOADER__H_

#include "eC_Types.h"
#include "eC_String.h"
#include "eC_TList_doubleLinked.h"

class CGUIImageData;
class CGUIImageDecoder;

#define GETIMGLOADER       CGUIImageLoader::GetInstance()

/// Manages a list of instances of CGUIImageDecoder.

/**
    Each decoder decides on its own if it matches the file format. The decoders will also be
    sorted by an access counter in order to speed up access time to the most used decoders.
*/

class CGUIImageLoader
{
public:
    /// return image loader instance
    static inline CGUIImageLoader& GetInstance() { return ms_kImageLoader; }

    /** Emptys the list and deletes all decoder pointers. Should not be called from user code. */
    void DeInit();

    /** Method for loading an image. Decoders in the list will be requested to decode the file.
        The first one, which decodes the file will be used and no other decoder will be called.
        @param pImageData Pointer to the CGUIImageData object, which will include the image information.
        @param kFilePath Path to the image file.
        @return False if load fails.
        */
    eC_Bool LoadImg(CGUIImageData &pImageData, const eC_String &kFilePath);

    /** Adds a decoder to the sorted list of accessible decoders.
        @param pImageDecoder Pointer to decoder to add.
    */
    void AddDecoder(CGUIImageDecoder *pImageDecoder);

    /** Adds all supported filetypes for all registered decoders
        @param kExtensions stringlist containing file-extensions
    */
    void AddSupportedFileExtensions(eC_TListDoubleLinked<eC_String>& kExtensions);

#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    /** Private CGUIImageLoader destructor */
    ~CGUIImageLoader();

private:
    /** Struct to hold the image decoder and the number of calls of the image decoder to sort the
        list.
    */
    struct ImageDecoderEntry_t
    {
        /// Pointer to the image decoder.
        CGUIImageDecoder *m_pImageDecoder;

        /// Reference counter: Counts the nummber of calls of this decoder (successful calls of LoadImg).
        eC_UInt m_uiNumCalls;

        /** Overloaded operator== function for the search method of eC_TListDoublyLinked. */
        eC_Bool operator==(ImageDecoderEntry_t &kSource) const
        {
            return m_uiNumCalls == kSource.m_uiNumCalls;
        }

        /** Overloaded operator> function for the search method of eC_TListDoublyLinked. */
        eC_Bool operator>(const ImageDecoderEntry_t &kSource) const
        {
            return m_uiNumCalls > kSource.m_uiNumCalls;
        }
    };

    /** Private CGUIImageLoader constructor */
    CGUIImageLoader() {}

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default */
    CGUIImageLoader(const CGUIImageLoader& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIImageLoader& operator=(const CGUIImageLoader& kSource);

    eC_TListDoubleLinked<ImageDecoderEntry_t> m_kDecoderList;                   /// List of decoders.

    static CGUIImageLoader ms_kImageLoader;                                     /// Static private attribute to this class.
};

#endif
