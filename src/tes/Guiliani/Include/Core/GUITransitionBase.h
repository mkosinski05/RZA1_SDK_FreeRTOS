/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_BASE__H_
#define GUI_TRANSITION_BASE__H_

#include "GUICompositeObject.h"
#include "GUITimer.h"

/// Abstract base class to transit from a source dialog to a destination dialog.
// @guiliani_doxygen toplevel_module Dialog Transitions
/**
    <table border="0">
    <tr>
        <td width="200">@image html dialog_transitions.png</td>
            <td>
                The "dialog transitions" uses transitions to change from one dialog to another. Guiliani provides the following transitions (Class: CGUITransitionBase):
                - Blend: fading out a source dialog while a destination dialog will be faded in
                - Dissolve: dissolving the source-screen by splitting it up into a number of rectangular tiles, and overblending it with the destination-screen tile by tile.
                - Push: moving out a source dialog while a destination dialog is moved in
                - Rotation: rotating out a source dialog while a destination dialog is rotated in.
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/** This class serves as base class for all dialog transitions. Inherited classes
    must override:  InitTransition EndTransition and UpdateTransition

    To compute a transition StartTransition must be called. The source dialog needs
    to have a parent container. The destination dialog should have a parent which normally
    should be the parent of the source.
    The source dialog is set
    to be invisible when the transition has finished and if specified by bDeleteSrc parameter
    the source dialog is deleted.

    A GUIObject can only be part of one transition at the same time. If an object is already
    part of a transition all further calls to StartTransition are ignored.
    @ingroup GUILIANI_ANIMATIONS
    */

class CGUITransitionBase : public CGUIObject
{
public:
    /// Constructs a Transition.
    /**
        @param uiTransitionTime Transition time in ms.
        @param bAutoDelete The transition instance is deleted by default after transition has
        finished
    */
    CGUITransitionBase(eC_UInt uiTransitionTime, eC_Bool bAutoDelete = true);
    virtual ~CGUITransitionBase();

    /// Starts a Transition.
    /** @param pSource The dialog that is already visible and where transition starts. The GUIObject
               must have a valid parent
        @param pDest The dialog that is not visible at transition start but after transition
               has finished. This object is not allowed to have a parent at transition start. It will be
               put to source's parent by the transition class.
        @param bDeleteSrc If true the source object will be deleted after transition has finished.
               Otherwise it will be set to be invisible.
    */
    void StartTransition(CGUIObject *pSource,
                         CGUIObject *pDest,
                         eC_Bool bDeleteSrc = true);

    /// Sets the transition time in ms.
    /** @param uiTransitionTime Transition time in ms.
    */
    void SetTransitionTime(eC_UInt uiTransitionTime);

    /// Gets the transition time in ms.
    /** @return Transition time in ms.
    */
    inline eC_UInt GetTransitionTime() const {return m_uiTransitionTime;}

    /** Returns if the transition is still in progress.
    @return True if the transition is still in progress
    otherwise False
    */
    bool IsActive();

protected:
    /// animation callback
    virtual void DoAnimate(const eC_Value &vTimes = eC_FromInt(1));

    /// Initializes a Transition.
    virtual void InitTransition() = 0;

    /// Cleans up a Transition.
    virtual void EndTransition() = 0;

    /// Initializes a Transition.
    /**@param vProgress The current transition progress (0..1)
    */
    virtual void UpdateTransition(eC_Value vProgress) = 0;

protected:
    CGUICompositeObject*    m_pParent; ///<Pointer to the parent composite object
    CGUIObject*             m_pSource; ///<Pointer to the source object
    CGUIObject*             m_pDest; ///<Pointer to the destination object
    eC_UInt                 m_uiFrameRate; ///<Frame rate

private:
    eC_Value                m_vElapsed;
    eC_UInt                 m_uiTransitionTime;
    eC_Bool                 m_bDeleteSrc;
    eC_Bool                 m_bAutoDelete;
    eC_Bool                 m_bTransitionActive;

    // Static list of pointers to objects which are already part of a transition
    static ObjectPtrList    ms_kUsedTransObjList;
};

#endif
