/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTDUMMY_H
#define GUIIMAGECACHEREPLACEMENTDUMMY_H

#include "GUIImageCacheReplacementPolicy.h"

class CGUIImageCacheReplacementManager;

/**
    Images can be added, but not removed.
    @brief Dummy replacement policy for use with the Guiliani image cache.
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementDummy : public CGUIImageCacheReplacementPolicy
{
public:
    /** Adds an image to the pool of replaceable images.
        The image is added as the first item.
        @param eImageID the image's ID
        @param uiSize the image's size
        @return True if the image was successfully added
    */
    eC_Bool Add(const ImageResource_t& eImageID, eC_UInt uiSize);

    /** Does not remove any images.
        @return DUMMY_IMAGE
    */
    ImageResource_t Remove();

    /** Returns the type of this replacement policy.
        @return IC_REPLACEMENT_POLICY_DUMMY
    */
    const inline ReplacementPolicyType_t GetPolicyType() const
    {
        return IC_REPLACEMENT_POLICY_DUMMY;
    }

    /// Constructor.
    CGUIImageCacheReplacementDummy(CGUIImageCacheReplacementManager* pManager);

    /// Destructor.
    ~CGUIImageCacheReplacementDummy();
};
#endif //GUIIMAGECACHEREPLACEMENTDUMMY_H
