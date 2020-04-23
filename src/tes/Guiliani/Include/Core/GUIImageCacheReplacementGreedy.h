/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTGREEDY_H
#define GUIIMAGECACHEREPLACEMENTGREEDY_H

#include "GUIImageCacheReplacementPolicy.h"

class CGUIImageCacheReplacementManager;

/** @brief Greedy replacement policy for use with the Guiliani image cache.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementGreedy : public CGUIImageCacheReplacementPolicy
{
public:
    /** Adds an image to the pool of replaceable images.
        The image is added sorted by size (ascending).
        @param eImageID the image's ID
        @param uiSize the image's size
        @return True if the image was successfully added
    */
    eC_Bool Add(const ImageResource_t& eImageID, eC_UInt uiSize);

    /** Removes an image from the pool of replaceable images.
        The largest image is selected for removal.
        @return the removed image's ID
    */
    ImageResource_t Remove();

    /** Returns the type of this replacement policy.
        @return IC_REPLACEMENT_POLICY_GREEDY
    */
    const inline ReplacementPolicyType_t GetPolicyType() const
    {
        return IC_REPLACEMENT_POLICY_GREEDY;
    }

    /// Constructor.
    CGUIImageCacheReplacementGreedy(CGUIImageCacheReplacementManager* pManager);

    /// Destructor.
    ~CGUIImageCacheReplacementGreedy();
};
#endif //GUIIMAGECACHEREPLACEMENTGREEDY_H
