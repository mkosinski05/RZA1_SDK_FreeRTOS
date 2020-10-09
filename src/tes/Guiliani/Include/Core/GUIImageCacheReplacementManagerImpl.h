/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIIMAGECACHEREPLACEMENTMANAGERIMPL_H
#define CGUIIMAGECACHEREPLACEMENTMANAGERIMPL_H

#include "GUIImageCacheReplacementManager.h"
#include "GUIImageCache.h"
#include "eC_TList_doubleLinked.h"

struct ImageCacheItem_t; // forward declaration, defined at bottom of file

/**
    Provides the methods used by replacement policies to provide image cache
    implementation specific services.
    @brief Replacement manager for the image cache.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementManagerImpl : public CGUIImageCacheReplacementManager
{
public:
    /** Adds an image as the first item of the list.
        @param eImageID the ID of the image to be added
        @param uiSize the size of the image to be added
        @return True if the image could be added
    */
    eC_Bool AddAtFront(const ImageResource_t& eImageID, eC_UInt uiSize );

    /** Adds an image sorted by size
        @param eImageID the ID of the image to be added
        @param uiSize the size of the image to be added
        @return True if the image could be added
    */
    eC_Bool AddSortedBySize(const ImageResource_t& eImageID, eC_UInt uiSize );

    /** Moves an image to the front of the list
        @param eImageID the ID of the image to be moved
        @return True if the image could be moved
    */
    eC_Bool BringToFront(const ImageResource_t& eImageID);

    /** Determines if an image resides in the list
        @param eImageID the image's ID
        @return True if the image resides in the manager's part of the cache
    */
    eC_Bool Contains(const ImageResource_t& eImageID) const;

    /** Determines the size of the list
        @return the size of the manager's part of the cache
    */
    eC_UInt GetSize() const;

    /** Determines the number of items the part of the cache managed by the manager
        @return the number of items in the manager's part of the cache
    */
    eC_UInt GetQuantity() const;

    /** Determines if the list is empty
        @return True if the manager's part of the cache is empty
    */
    eC_Bool IsEmpty() const;

    /** Removes a specific image
        @param eImageID the ID of the image to be removed
        @return True if the image was successfully removed
    */
    eC_Bool Remove(const ImageResource_t& eImageID);

    /** Removes the last image of the list
        @return the removed image's ID (or DUMMY_IMAGE if no image could be removed)
    */
    ImageResource_t RemoveFromEnd();

    /** Constructor. */
    CGUIImageCacheReplacementManagerImpl();

    /** Destructor. */
    ~CGUIImageCacheReplacementManagerImpl();

private:
    eC_TListDoubleLinked<ImageCacheItem_t>* m_pCacheList;

    /// Accumulated size of all images in the cache list.
    eC_UInt m_uiListSize;
};

/** @brief List Entry for one Cache Item */
struct ImageCacheItem_t
{
    /// Image ID
    ImageResource_t m_eImageID;

    /// Constructor
    ImageCacheItem_t()
    {
        m_eImageID = DUMMY_IMAGE;
    }

    /** Comparison operator
        @param pItem1 Item to be compared with
        @return True if equal, False otherwise
    */
    eC_Bool operator== (const ImageCacheItem_t& pItem1) const
    {
        return (m_eImageID == pItem1.m_eImageID);
    }

    /** Larger-than operator
        @param pItem1 Item to be compared with
        @return True if larger than, False otherwise
    */
    eC_Bool operator> (const ImageCacheItem_t& pItem1) const
    {
        return (GETIMGCACHE->GetImageSize(m_eImageID) > GETIMGCACHE->GetImageSize(pItem1.m_eImageID));
    }
};

#endif //CGUIIMAGECACHEREPLACEMENTMANAGERIMPL_H
