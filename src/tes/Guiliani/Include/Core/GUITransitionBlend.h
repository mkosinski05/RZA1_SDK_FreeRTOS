/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_BLEND__H_
#define GUI_TRANSITION_BLEND__H_

#include "GUITransitionBase.h"
#include "GUIEasing.h"

/// Class to compute a blend transition from a source dialog to a destination dialog.
/** This class fades out a source dialog while a destination dialog is faded in.
    Optionally the size of source and destination dialog might be faded in / out by
    BLEND_AND_SIZE animation mode.
    For basic transition behaviour see: @see CGUITransitionBase

    Example:
@code

    // Create a transition with a duration of 1000ms that blends and sizes a source and a destination dialog.
       pkTrasnition will delete itself after transition is finished.
    CGUITransitionBlend* pkTransition = new CGUITransitionBlend(1000, CGUITransitionBlend::BLEND_AND_SIZE, true);

    // Start the transition. The source dialog is deleted by the transition
       after completetion.
    pkTransition->StartTransition(pSource, pDest, true);

@endcode
@ingroup GUILIANI_ANIMATIONS
*/

class CGUITransitionBlend :
    public CGUITransitionBase
{
public:
    /** Used to specify the mode of blend transition.
    */
    enum eAnimMode
    {
        BLEND_ONLY,         ///< Alpha blending only. This will smoothly overblend source- and destination object.
        BLEND_FADE,         ///< Alpha blending only. This will fade out source object, then fade in destination object
        BLEND_AND_SHRINK,   ///< Alpha blending and sizing (source dialog gets smaller and destination dialog bigger)
        BLEND_AND_ZOOM      ///< Alpha blending and sizing (source dialog gets bigger and destination dialog bigger)
    };
    /// Constructs a BlendTransition.
    /**
        @param uiTransitionTime Transition time in ms.
        @param animMode The blend mode of transition
        @param eEasingType The easing type of the push transition
        @param bAutoDelete The transition instance is deleted after transition has
        finished by default
    */
    CGUITransitionBlend(eC_UInt uiTransitionTime, eAnimMode animMode, CGUIEasing::EasingType_t eEasingType = CGUIEasing::EASE_LINEAR, eC_Bool bAutoDelete = true);
    ~CGUITransitionBlend();

protected:
    void InitTransition();
    void EndTransition();
    void UpdateTransition(eC_Value vProgress);

private:
    eAnimMode   m_animMode;
    CGUIEasing::EasingType_t m_eEasingType;
    CGUIRect    m_srcRect;
    CGUIRect    m_destRect;
};

#endif
