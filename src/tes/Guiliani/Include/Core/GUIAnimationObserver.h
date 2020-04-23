/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_OBSERVER__H_
#define GUIANIMATION_OBSERVER__H_

#include "GUIAnimation.h"
#include "GUIAnimationHandler.h"

/**
   @brief This interface is used to observe the status of an animation.

   It is possible to set an AnimationObserver for each animation by calling CGUIAnimation::SetAnimationObserver.

   @note When reading the GUI from stream, the CGUIAnimation::ReadFromStream() will not set its observer pointer.
         It is the responsibility of the animation observer to set itself as observer in the animation again when it is reading from stream.
         To do so, use the id of the animation which shall be observed.

    @ingroup GUILIANI_ANIMATIONS
*/
class CGUIAnimationObserver
{
public:
    /**
        This callback function is called by the observed animation whenever the status changes.
        @param eStatus The new status of the animation.
        @param pAnimation The Animation whose status is updated.
    */
    virtual void OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus,
                                     CGUIAnimation* pAnimation) = 0;

    /**
        Destructor
        Informs the animation handler about the deleted observer.
    */
    virtual ~CGUIAnimationObserver()
    {
        GETANIMATIONHDL.NotifyOfDestruction(this);
    }
};

#endif // GUIANIMATION_OBSERVER__H_
