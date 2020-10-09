/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIIMAGECACHEIMPL_H
#define GUIIMAGECACHEIMPL_H

#include "GUIImageResource.h"
#include "GUIImageCache.h"

class CGUIImageCacheReplacementManager;

/**
    @brief Implementation of Cache for Guiliani image resources.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheImpl : public CGUIImageCache
{
    friend class CGUIImageCache;

public:

#ifdef GUILIANI_STREAM_GUI
    /** Reads the cache settings from a file. GUIStreamReader has to be initialized.
        @param rkPath to the destination file.
        @return True if the configuration was read successfully.
        @see CGUIStreamReader
    */
    eC_Bool ReadCacheSettingsFromFile(const eC_String& rkPath);
#endif //GUILIANI_STREAM_GUI

    /** Determines the amount of memory used by the cached images.
        @return used size in bytes
    */
    eC_UInt GetCurrentMemoryUsage() const;

    /** Determines the amount of memory used by the cached images.
        @return used size in bytes
    */
    eC_UInt GetFreeListSize() const;

    /** Determines the amount of memory used by the cached images in the used list.
        @return used list size in bytes
    */
    eC_UInt GetUsedListSize() const;

    /** Get the size of an image resource in memory.
        @param eImageID ID of the image.
        @return Size of the image resource when loaded into memory
    */
    eC_UInt GetImageSize(const ImageResource_t& eImageID) const;

protected:
    /** Constructor.
      * @param uiCacheSizeLimit The maximum amount of memory that can be used
      *        by images.
      * @param eUsed Policy for used images
      * @param eFree Policy for free images
      */
    CGUIImageCacheImpl(eC_UInt uiCacheSizeLimit, ReplacementPolicyType_t eUsed, ReplacementPolicyType_t eFree);

    /// Destructor.
    ~CGUIImageCacheImpl();

    /** Deletes images from the cache until its size is reduced by <strong>uiSpaceToFree</strong>.
        @param uiSpaceToFree the amount of memory to be freed
        @return True if the full amount of memory could be freed
        Freeing space can fail if not enough images are allowed to be unloaded.
        In case of failure the maximum possible amount of space is freed.
    */
    eC_Bool FreeSpace(eC_UInt uiSpaceToFree);

    /** Tries to load an image and adds it to the cache.
        Needed space is freed before adding, if necessary.
        @param psPath the path to the image file.
        Used for determining the size and for the actual loading.
        @param eImageID the image's ID
        @return True if the image was successfully loaded.
        Loading can fail if not enough space for the image can be freed.
    */
    eC_Bool LoadImg(const eC_String& psPath, const ImageResource_t& eImageID);

    /** Special version of LoadImage for preloading images at start-up.
        Once the cache is full, needed space is not freed and no more images are preloaded.
        @param psPath the path to the image file
        used for determining the size and for the actual loading
        @param eImageID the image's ID
        @return True if the image was successfully loaded.
        Loading fails if the cache is full.
    */
    eC_Bool PreloadImg(const eC_String& psPath, const ImageResource_t& eImageID);

    /** Unloads a specific image and removes it from the free list.
        @param eImageID the image's ID
        @return True if the image was successfully unloaded,
        False if it was not in memory in the first place.
    */
    eC_Bool UnloadSpecificImage(const ImageResource_t& eImageID);

    /** Moves an (already loaded) image to the list of cached (but unused) images.
        The image is moved from the used list to the free list.
        @param eImageID the image's ID
    */
    void MarkAsFree(const ImageResource_t& eImageID);

    /** Marks an (already loaded) image as used.
        The image is moved from the free list to the used list.
        @param eImageID the image's ID
    */
    void MarkAsUsed(const ImageResource_t& eImageID);

    /** Copies the cache contents of one replacement policy to the other.
        This is needed for changing replacement policies at runtime.
        Used as a hook in the template methods SetFreeReplacgementPolicy and
        SetUsedReplacementPolicy.
        @param pOldPolicy old replacement policy object
        @param pNewPolicy new replacement policy object
    */
    void CopyCacheContents(CGUIImageCacheReplacementPolicy* pOldPolicy, CGUIImageCacheReplacementPolicy* pNewPolicy);

    void NotifyImageAccessInternal(const ImageResource_t& eImageID);

private:
    /** Unloads the next image (according to the replacement policy).
        @return the number of bytes freed
    */
    eC_UInt UnloadImage();

    /** Adds an image to the free list.
        This method takes care of correct sorting according to the cache replacement policy.
        @param eImageID the image to be added
    */
    void AddToFreeList(const ImageResource_t& eImageID);

    /** Adds an image to the used list.
        This method takes care of correct sorting according to the cache replacement policy.
        @param eImageID the image to be added
    */
    void AddToUsedList(const ImageResource_t& eImageID);

    /** Removes an image from the free list.
        @param eImageID the image to be removed
    */
    void RemoveFromFreeList(const ImageResource_t& eImageID);

    /** Removes an image from the used list.
        @param eImageID the image to be removed
    */
    void RemoveFromUsedList(const ImageResource_t& eImageID);

    /** The image ID that was most recently passed to NotifyImageAccess() to
        avoid multiple list accesses.
      */
    ImageResource_t m_eLastAccessedImage;

#ifdef GUI_IMAGECACHE_DEBUG
    eC_UInt m_uiMaxCacheSize;
#endif
};

#endif //GUIIMAGECACHEIMPL_H
