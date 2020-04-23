/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGECACHEREPLACEMENTPOLICY_H
#define GUIIMAGECACHEREPLACEMENTPOLICY_H

#include "GUIImageResource.h"
#include "eC_Types.h"

class CGUIImageCacheReplacementManager;

/** Enumeration of replacement policies
    @ingroup IMAGE_CACHE
*/
enum ReplacementPolicyType_t
{
    IC_REPLACEMENT_POLICY_DUMMY  = 0,  ///< Images can be added, but not removed.
    IC_REPLACEMENT_POLICY_GREEDY = 1,  ///< Delete biggest image first
    IC_REPLACEMENT_POLICY_FIFO   = 2,  ///< First in first out 
    IC_REPLACEMENT_POLICY_LRU    = 3   ///< Last recently used
};

/** @brief Abstract replacement policy for use with the Guiliani image cache
    @class CGUIImageCacheReplacementPolicy
    @ingroup IMAGE_CACHE
*/
class CGUIImageCacheReplacementPolicy
{
public:

    /** Creates a replacement policy object for a specific policy type
        @param eType the type of replacement policy
        @param pManager the replacement manager object to be used with the new policy
        @return the newly created replacement policy
    */
    static CGUIImageCacheReplacementPolicy* CreateReplacementPolicyForType(const ReplacementPolicyType_t& eType, CGUIImageCacheReplacementManager* pManager);

    /** Get the image cache replacement manager of the policy
        @return A pointer to the image cache manager
    */
    CGUIImageCacheReplacementManager* GetImageCacheReplacementManager(void);

    /** Adds an image to the pool of replaceable images
        The image has to be added in the appropriate way for the implemented replacement policy
        @param eImageID the image's ID
        @param uiSize the image's size
        @return True if the image was successfully added
    */
    virtual eC_Bool Add(const ImageResource_t& eImageID, eC_UInt uiSize ) = 0;

    /** Removes an image from the pool of replaceable images
        The image to be removed is selected in accordance to the replacement policy
        @return the removed image's ID
    */
    virtual ImageResource_t Remove() =0;

    /** Marks an image as recently used, if supported by the replacement policy
        (Only valid for LRU replacement)
        @param eImageID the image's ID
        @return True if any action was performed
    */
    virtual eC_Bool Bump(const ImageResource_t& eImageID);

    /** Returns the type of this replacement policy
        @return the policy type
    */
    virtual const ReplacementPolicyType_t GetPolicyType() const=0;

    /// Destructor.
    virtual ~CGUIImageCacheReplacementPolicy() {}

protected:
    /// The replacement manager (bridge implementation)
    CGUIImageCacheReplacementManager* m_pCacheManager;
};

#endif //GUIIMAGECACHEREPLACEMENTPOLICY_H
