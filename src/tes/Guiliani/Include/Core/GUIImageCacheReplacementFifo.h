/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTFIFO_H
#define GUIIMAGECACHEREPLACEMENTFIFO_H

#include "GUIImageCacheReplacementPolicy.h"

class CGUIImageCacheReplacementManager;

/** @brief FIFO replacement policy for use with the Guiliani image cache
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementFifo : public CGUIImageCacheReplacementPolicy
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

    /** Returns the type of this replacement policy.
        @return IC_REPLACEMENT_POLICY_FIFO
    */
    inline const ReplacementPolicyType_t GetPolicyType() const
    {
        return IC_REPLACEMENT_POLICY_FIFO;
    }

    /// Constructor.
    CGUIImageCacheReplacementFifo(CGUIImageCacheReplacementManager* pManager);

    /// Destructor.
    ~CGUIImageCacheReplacementFifo();
};
#endif //GUIIMAGECACHEREPLACEMENTFIFO_H
