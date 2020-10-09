/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_DISSOLVE__H_
#define GUI_TRANSITION_DISSOLVE__H_

#include "GUITransitionBase.h"

const eC_UInt cuiHorizontalTiles=10;
const eC_UInt cuiVerticalTiles=10;


/** This class dissolves the source-screen by splitting it up into a number of rectangular tiles, and overblending
    it with the destination-screen tile by tile.

    Attention: Please do NOT Invalidate any area overlapping with the transition while it is in progress. This WILL result in artefacts.
    Please note: This transition does not require BitmapPlanes to be supported on the target platform.

    @brief Class to compute a dissolving transition from a source dialog to a destination dialog.
    @ingroup GUILIANI_ANIMATIONS
    */
class CGUITransitionDissolve :
    public CGUITransitionBase
{
public:

    /// Constructs a Dissolving Transition.
    /**
        @param uiTransitionTime Transition time in ms.
        @param bAutoDelete The transition instance is deleted after transition has
        finished by default
    */
    CGUITransitionDissolve(eC_UInt uiTransitionTime, eC_Bool bAutoDelete = true);
    ~CGUITransitionDissolve();

protected:
    void InitTransition();
    void EndTransition();
    void UpdateTransition(eC_Value vProgress);

private:
    // Backup of original sizes of objects involved in transition
    CGUIRect    m_srcRect;
    CGUIRect    m_destRect;
    // For the transition, the transition-area is split up into a number of Tiles.
    // Those will initially show the source-object and over time eventually switch over to show the destination object
    eC_Bool     m_abTiles[cuiHorizontalTiles][cuiVerticalTiles];
    eC_Bool     m_bFirstStepOfTransition;
};

#endif
