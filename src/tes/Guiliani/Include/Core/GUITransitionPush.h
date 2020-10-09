/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_PUSH__H_
#define GUI_TRANSITION_PUSH__H_

#include "GUITransitionBase.h"
#include "GUIEasing.h"

/// Class to compute a push transition from a source dialog to a destination dialog.
/** This class moves out a source dialog while a destination dialog is moved in.
    Push direction is selectable by animMode parameter.
    For basic transition behaviour see: @see CGUITransitionBase

    Example:
@code

    // Create a transition with a duration of 1000ms that pushes out a source while pulling in a destination dialog.
       pkTrasnition will delete itself after transition is finished.
    CGUITransitionPush* pkTransition = new CGUITransitionPush(1000, CGUITransitionPush::FROM_LEFT, true);

    // Start the transition. The source dialog is deleted by the transition
       after completion.
    pkTransition->StartTransition(pSource, pDest, true);

@endcode
@ingroup GUILIANI_ANIMATIONS
*/
class CGUITransitionPush : public CGUITransitionBase
{
public:
    /** Used to specify the direction of push transition.
    */
    enum eAnimMode
    {
        FROM_LEFT,      ///< Push from left to right
        FROM_RIGHT,     ///< Push from right to left
        FROM_TOP,       ///< Push from top to bottom
        FROM_BOTTOM     ///< Push from bottom to top
    };

    /// Constructs a PushTransition.
    /**
        @param uiTransitionTime Transition time in ms.
        @param animMode The direction of push transition
        @param animMode The direction of push transition
        @param eEasingType The easing type of the push transition
        @param bAutoDelete The transition instance is deleted after transition has
        finished by default
    */
    CGUITransitionPush(eC_UInt uiTransitionTime, eAnimMode animMode, CGUIEasing::EasingType_t eEasingType = CGUIEasing::EASE_LINEAR, eC_Bool bAutoDelete = true);
    ~CGUITransitionPush();

protected:
    void InitTransition();
    void EndTransition();
    void UpdateTransition(eC_Value vProgress);

private:
    eAnimMode   m_animMode;
    eC_Value    m_vSrcPosX;
    eC_Value    m_vSrcPosY;
    CGUIEasing::EasingType_t m_eEasingType;
};

#endif
