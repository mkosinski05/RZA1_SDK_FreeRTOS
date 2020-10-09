/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTMANAGER_H
#define GUIIMAGECACHEREPLACEMENTMANAGER_H

#include "GUIImageResource.h"
#include "eC_Types.h"

/**
    Provides the methods used by replacement policies to provide image cache
    implementation specific services.
    @brief Replacement manager for the image cache.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementManager
{
public:
    /** Adds an image as the "first" item
        @param eImageID the ID of the image to be added.
        @param uiSize the size of the image to be added
        @return True if the image could be added
    */
    virtual eC_Bool AddAtFront(const ImageResource_t& eImageID, eC_UInt uiSize ) =0;

    /** Adds an image sorted by size
        @param eImageID the ID of the image to be added
        @param uiSize the size of the image to be added
        @return True if the image could be added
    */
    virtual eC_Bool AddSortedBySize(const ImageResource_t& eImageID, eC_UInt uiSize ) =0;

    /** Moves an image to be the first item
        @param eImageID the ID of the image to be moved
        @return True if the image could be moved
    */
    virtual eC_Bool BringToFront(const ImageResource_t& eImageID) =0;

    /** Determines if an image resides in the part of the cache managed by the manager
        @param eImageID the image's ID
        @return True if the image resides in the manager's part of the cache
    */
    virtual eC_Bool Contains(const ImageResource_t& eImageID) const =0;

    /** Determines the size of the part of the cache managed by the manager
        @return the size of the manager's part of the cache
    */
    virtual eC_UInt GetSize() const = 0;

    /** Determines the number of items the part of the cache managed by the manager
        @return the number of items in the manager's part of the cache
    */
    virtual eC_UInt GetQuantity() const = 0;

    /** Determines if the part of the cache managed by the manager is empty
        @return True if the manager's part of the cache is empty
    */
    virtual eC_Bool IsEmpty() const = 0;

    /** Removes a specific image
        @param eImageID the ID of the image to be removed
        @return True if the image was successfully removed
    */
    virtual eC_Bool Remove(const ImageResource_t& eImageID) = 0;

    /** Removes the "last" image
        @return the removed image's ID (or DUMMY_IMAGE if no image could be removed)
    */
    virtual ImageResource_t RemoveFromEnd() =0;

    /// Destructor.
    virtual ~CGUIImageCacheReplacementManager() {}
};
#endif //GUIIMAGECACHEREPLACEMENTMANAGER_H
