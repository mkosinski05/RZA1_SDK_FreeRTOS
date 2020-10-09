/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHE_H
#define GUIIMAGECACHE_H

#include "eC_String.h"
#include "GUIImageResource.h"
#include "GUIImageCacheReplacementPolicy.h"

class CGUIImageCacheReplacementManager;

/** Returns a pointer to the image cache instance.
    This pointer is NULL if no image cache instance has been created.
  */
#define GETIMGCACHE (CGUIImageCache::GetInstance())

// Uncomment for lots of debug messages
//#define GUI_IMAGECACHE_DEBUG
/**
    @defgroup IMAGE_CACHE Image caching
    @brief Caches currently used and preloaded images

    The cache contains images currently used in the application (used list) and
    unused "permanent" images that are cached for later use (free list).

    Images are loaded to the cache until the size limit is reached. When
    another image has to be loaded, space in the cache is freed by unloading
    unused "permanent" images. The order of unloading is determined by a
    replacement policy - lru (last recently used), fifo (first in first out, 
    for images in the free list this equals lru) or greedy (largest image is unloaded
    first).

    By default, the cache has a size limit of 100 MB and uses LRU policies for
    both lists.

    No caching is applied to dynamic images, which were registered without supplying
    a filename. Since they can not be unloaded/reloaded on request, caching can not
    be done for those (see CGUIResourceManager::RegisterDynamicImage()).

    Note that this 'cache' only works as a real cache for images that have the
    'permanent' flag set (see CGUIResourceManager::RegisterImageResource()).
    Its main functionality is limiting the amount of memory used by images by
    explicitly unloading images when the size limit is reached and re-loading
    them when needed.

    The example below shows the initialization of the GUIImageCache. 
    The cache size and the policies should be adapted to the project needs.
@code
    // Create image cache of 2 MB (size in bytes) with greedy policy for used and fifo for free images
    GETIMGCACHE->CreateInstance(1024*1024*2, IC_REPLACEMENT_POLICY_GREEDY, IC_REPLACEMENT_POLICY_FIFO);
@endcode
    Alternatively the ImageCache can configure itself by Loading a XML configuration.
@code
    ReadCacheSettingsFromFile("CacheConfig.xml");
@endcode
    Recognized XML Statements shall be surrounded by a \<CacheConfig\> element. 
    In this Tag a \<FileVersion\> element is mandatory. Currently only version 1 is valid. 
    Also an \<Entries\> element is mandatory. It specifies the number of configurations. 
    These number of \<ImageCacheConfig\> elements are expected also in the \<CacheConfig\> element. 
    For each ImageCacheConfig elements one \<Resolution\> element, one \<CacheSize\> element, 
    one \<ReplacementFree\> element and one \<ReplacementUsed\> element is needed.

    In the resolution element a string describing the resolution in the form "(X-Resolution)x(Y-Resolution)" 
    is mandatory. A string "_(AppendString)" my be added to the resolution string. 
    To Load a resolution with AppendString, the AppendString needs to be specified using 
    SetReadResolutionAppendix(const eC_String& kAppendString).

    The \<CacheSize\> element specifies the cache size in bytes

    \<ReplacementFree\> element specifies the ReplacementPolicyType_t for Free replacement policy casted to a number

    \<ReplacementUsed\> element specifies the ReplacementPolicyType_t for Used replacement policy casted to a number

    A valid XML file looks like that:
@code{.xml}
    <CacheConfig>
        <FileVersion>1</FileVersion>
        <Entries>2</Entries>
        <ImageCacheConfig>
            <Resolution>240x320</Resolution>
            <CacheSize>65535</CacheSize>
            <ReplacementFree>3</ReplacementFree>
            <ReplacementUsed>3</ReplacementUsed>
        </ImageCacheConfig>
        <ImageCacheConfig>
            <Resolution>480x640_AppendString</Resolution>
            <CacheSize>1048576</CacheSize>
            <ReplacementFree>2</ReplacementFree>
            <ReplacementUsed>1</ReplacementUsed>
        </ImageCacheConfig>
    </CacheConfig>
@endcode

*/
/** @brief Image cache base class
    @ingroup IMAGE_CACHE
*/
class CGUIImageCache
{
    /** To access methods involved with loading and unloading of images, the
        Resource Manager must be a friend class.
      */
    friend class CGUIResourceManager;
    friend class CGUIBackgroundImageLoaderThread;
    /// The graphics wrapper calls NotifyImageAccess.
    friend class CGfxWrap;

public:
    /** Creates the only instance of this class.
        @param uiSize The initial size of the cache in bytes. Default is 100MB.
        @param eUsed Policy for used images
        @param eFree Policy for free images
      */
    static void CreateInstance(eC_UInt uiSize = 100 * 1024 * 1024, 
        ReplacementPolicyType_t eUsed = IC_REPLACEMENT_POLICY_LRU, ReplacementPolicyType_t eFree = IC_REPLACEMENT_POLICY_LRU );

    /** Retrieves the only instance of this class.
        @return pointer to the only instance
    */
    static inline CGUIImageCache* GetInstance()
    {
        return ms_pInstance;
    }

    /// Deletes the only instance of this class.
    static void DeleteInstance();

#ifdef GUILIANI_STREAM_GUI
    /** Reads the cache settings from a file. GUIStreamReader has to be initialized.
        @param kPath to the destination file.
        @return True if the configuration was read successfully.
        @see CGUIStreamReader
    */
    virtual eC_Bool ReadCacheSettingsFromFile(const eC_String& kPath) =0;

    /** Tells the image cache to append a string to the resolution when
        reading its configuration.
        The method ReadCacheSettingsFromFile() decides with configuration entry
        it reads by querying the screen resolution from the graphics wrapper
        and appending this string.
        Needs to be called before replacement policies are initialized.
        @param kAppendString the String to be appended to the resolution while
        reading the configuration.
    */
    void SetReadResolutionAppendix(const eC_String& kAppendString)
    {
        m_kAppendString = kAppendString;
    }

#endif //GUILIANI_STREAM_GUI

    /** Sets the size limit for the image cache.
        @param uiCacheSizeLimit the new maximum allowed size of the cache in
               bytes
        @return True if new size limit could be set.
        Setting of size limit can fail if
        - not enough are allowed to be unloaded to decrease cache size or
        - not enough memory left to increase cache size.
        In case of failure the size limit is set to the lowest (or highest,
        respectively) possible value.
    */
    eC_Bool SetCacheSizeLimit(eC_UInt uiCacheSizeLimit);

    /** Determines the amount of memory used by the cached items.
        @return used size in bytes
    */
    virtual eC_UInt GetCurrentMemoryUsage() const=0;

    /** Returns the size limit for the image cache.
        @return the maximum allowed size of the cache in bytes
    */
    inline eC_UInt GetCacheSizeLimit() const { return m_uiCacheSizeLimit; }

    /** Get the size of an image resource in memory.
        @param eImageID ID of the image.
        @return Size of the image resource when loaded into memory
    */
    virtual eC_UInt GetImageSize(const ImageResource_t& eImageID) const=0;

    /** Set the replacement policy for the image cache.
        If a previous policy exists, this method deletes it.
        Calling this when the cache already contains elements may result in undefined behavior.
        It is therefore advisable to only call this method once, at startup.
        @param pPolicy the replacement policy
        @deprecated Function call may result in undefined behaviour, use CreateInstance-parameters instead
    */
    virtual void SetFreeReplacementPolicy(CGUIImageCacheReplacementPolicy* pPolicy);

    /** Set the replacement policy for the image cache.
        If a previous policy exists, this method deletes it.
        Calling this when the cache already contains elements may result in undefined behavior.
        It is therefore advisable to only call this method once, at startup.
        @param pPolicy the replacement policy
        @deprecated Function call may result in undefined behaviour, use CreateInstance-parameters instead
    */
    virtual void SetUsedReplacementPolicy(CGUIImageCacheReplacementPolicy* pPolicy);

protected:
    /** Notifies the cache about an access (e.g. blitting,...) to an image.
        @param eImageID the image that was accessed.
    */
    static void NotifyImageAccess(const ImageResource_t& eImageID)
    {
        if (ms_pInstance != NULL)
        {
            ms_pInstance->NotifyImageAccessInternal(eImageID);
        }
    }

    /** Deletes images from the cache until its size is reduced by <strong>uiBytes</strong>.
        @param uiSpaceToFree the amount of memory to be freed
        @return True if the full amount of memory could be freed.
        Freeing space can fail if not enough images are allowed to be unloaded.
        In case of failure the maximum possible amount of space is freed.
    */
    virtual eC_Bool FreeSpace(eC_UInt uiSpaceToFree) =0;

    /** Tries to load an image and adds it to the cache.
        Needed space is freed before adding, if necessary.
        @param kPath the path to the image file.
        Used for determining the size and for loading the image.
        @param eImageID the image's ID
        @return True if the image was successfully loaded.
        Loading can fail if not enough space for the image can be freed.
    */
    virtual eC_Bool LoadImg(const eC_String& kPath, const ImageResource_t& eImageID)=0;

    /** Special version of LoadImage for preloading images at start-up.
        Once the cache is full, needed space is not freed.
        @param psPath the path to the image file
        used for determining the size and for the actual loading
        @param eImageID the image's ID
        @return True if the image was successfully loaded.
        Loading fails if the cache is full.
    */
    virtual eC_Bool PreloadImg(const eC_String& psPath, const ImageResource_t& eImageID)=0;

    /** Marks an (already loaded) image as used.
        The image is moved from the free list to the used list.
        @param eImageID the image's ID
    */
    virtual void MarkAsUsed(const ImageResource_t& eImageID) =0;

    /** Moves an (already loaded) image to the list of cached (but unused) images.
        The image is moved from the used list to the free list.
        @param eImageID the image's ID
    */
    virtual void MarkAsFree(const ImageResource_t& eImageID) =0;

    /** Unloads a specific image and removes it from the free list.
        @param eImageID the image's ID
        @return True if the image was successfully unloaded,
        False if it was not in memory in the first place.
    */
    virtual eC_Bool UnloadSpecificImage(const ImageResource_t& eImageID) =0;

    /** Copies the cache contents of one replacement policy to the other.
        This is needed for changing replacement policies at runtime.
        Used as a hook in the template methods SetFreeReplacgementPolicy and
        SetUsedReplacementPolicy.
        @param pOldPolicy old replacement policy object
        @param pNewPolicy new replacement policy object
    */
    virtual void CopyCacheContents(CGUIImageCacheReplacementPolicy* pOldPolicy, CGUIImageCacheReplacementPolicy* pNewPolicy) =0;

    /** Notifies the cache about an access (e.g. blitting,...) to an image.
        @param eImageID the image that was accessed.
    */
    virtual void NotifyImageAccessInternal(const ImageResource_t& eImageID)=0;

    /// Destructor.
    virtual ~CGUIImageCache();

    /// Pointer to singleton instance.
    static CGUIImageCache* ms_pInstance;

    /// Maximum amount of memory used by images.
    eC_UInt m_uiCacheSizeLimit;

    /// Extension to the resolution for config reading.
    eC_String m_kAppendString;

    /// Pointer to the current free list replacement policy.
    CGUIImageCacheReplacementPolicy* m_pFreeReplacementPolicy;

    /// Pointer to the current used list replacement policy.
    CGUIImageCacheReplacementPolicy* m_pUsedReplacementPolicy;

    /// Pointer to the current free list manager.
    CGUIImageCacheReplacementManager* m_pFreeListManager;

    /// Pointer to the current used list manager.
    CGUIImageCacheReplacementManager* m_pUsedListManager;
};
#endif //GUIIMAGECACHE_H
