/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_ROTATION__H_
#define GUI_TRANSITION_ROTATION__H_

#include "GUITransitionBase.h"
#include "GUIEasing.h"

/// Class to compute a rotation transition from a source dialog to a destination dialog.
/** This class rotates out a source dialog while a destination dialog is rotated in.
    Rotation axis and direction are selectable by animMode parameter.
    For basic transition behaviour see: @see CGUITransitionBase

    Example:
@code

    // Create a transition with a duration of 1000ms that rotates out a source and rotates in a destination dialog.
       The rotation is invoked clockwise at y-axis. pkTrasnition will delete itself after transition is finished.
    CGUITransitionRotation* pkTransition = new CGUITransitionRotation(1000, CGUITransitionRotation::Y_AXIS_CW, true);

    // Start the transition. The source dialog is deleted by the transition
       after completetion.
    pkTransition->StartTransition(pSource, pDest, true);

@endcode
@ingroup GUILIANI_ANIMATIONS
*/

class CGUITransitionRotation : public CGUITransitionBase
{
public:
    /** Used to specify the direction of push transition.
    */
    enum eAnimMode
    {
        X_AXIS_CW,          ///< Rotate clockwise at x-axis
        X_AXIS_CCW,         ///< Rotate counter clockwise at x-axis
        Y_AXIS_CW,          ///< Rotate clockwise at y-axis
        Y_AXIS_CCW,         ///< Rotate counter clockwise at y-axis
    };

    /// Constructs a RotationTransition.
    /**
        @param uiTransitionTime Transition time in ms.
        @param animMode The axis and direction of rotation transition
        @param eEasingType The easing type of the push transition
        @param bAutoDelete The transition instance is deleted after transition has
        finished by default
    */
    CGUITransitionRotation(eC_UInt uiTransitionTime, eAnimMode animMode, CGUIEasing::EasingType_t eEasingType = CGUIEasing::EASE_LINEAR, eC_Bool bAutoDelete = true);
    ~CGUITransitionRotation();

protected:
    void InitTransition();
    void EndTransition();
    void UpdateTransition(eC_Value vProgress);

private:
    eAnimMode   m_animMode;
    CGUIEasing::EasingType_t m_eEasingType;
    CGUIObject* m_pRotationObj;
    eC_Value    m_vRotationOffset;
};

#endif
