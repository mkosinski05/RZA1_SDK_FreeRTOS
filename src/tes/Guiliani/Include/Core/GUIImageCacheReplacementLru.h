/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTLRU_H
#define GUIIMAGECACHEREPLACEMENTLRU_H

#include "GUIImageCacheReplacementPolicy.h"

class CGUIImageCacheReplacementManager;

/** @brief Least Recently Used replacement policy for use with the Guiliani image cache.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementLru : public CGUIImageCacheReplacementPolicy
{
public:
    /** Adds an image to the pool of replaceable images.
        The image is added as the first item.
        @param eImageID the image's ID
        @param uiSize the image's size
        @return True if the image was successfully added
    */
    eC_Bool Add(const ImageResource_t& eImageID, eC_UInt uiSize);

    /** Removes an image from the pool of replaceable images.
        The last image is selected for removal.
        @return the removed image's ID
    */
    ImageResource_t Remove();

    /** Marks an image as recently used.
        Moves the image to the front.
        @param eImageID the image's ID
        @return True
    */
    eC_Bool Bump(const ImageResource_t& eImageID);

    /** Returns the type of this replacement policy.
        @return IC_REPLACEMENT_POLICY_LRU
    */
    const inline ReplacementPolicyType_t GetPolicyType() const
    {
        return IC_REPLACEMENT_POLICY_LRU;
    }

    /// Destructor.
    ~CGUIImageCacheReplacementLru();

    /// Constructor.
    CGUIImageCacheReplacementLru(CGUIImageCacheReplacementManager* pManager);
};
#endif //GUIIMAGECACHEREPLACEMENTLRU_H
