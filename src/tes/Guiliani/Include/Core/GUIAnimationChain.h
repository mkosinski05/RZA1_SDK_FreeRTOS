/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_CHAIN__H_
#define GUIANIMATION_CHAIN__H_


#include "GUIAnimatable.h"
#include "GUIAnimationObserver.h"
#include "GUIStreamableObject.h"
#include "eC_TList_doubleLinked.h"


class CGUIAnimation;

/** @brief An AnimationChain is a combination of several independent animations.

The animations can be started one after the other, or in parallel.
For each animation which is added an optional time offset can be defined.

For an AnimationChain which allows parallel animations (AnimationChain_t::PARALLEL_ANIMATION),
this offset is relative to the start time of the AnimationChain.
For an AnimationChain which does not allow parallel execution(AnimationChain_t::CHAINED_ANIMATION),
the time offset is relative to the end of the previous animation.

Each animation which is added to the animation chain will be added to the CGUIAnimationHandler.

Example:

@code

    CGUIAnimation* pAnimation;
    CGUIAnimationChain* pChain;

    pChain = new CGUIAnimationChain(CGUIAnimationChain::CHAINED_ANIMATION);

    pAnimation = new CGUIAnimationMove(pkMessageBox, CGUIEasing::EASE_IN_OUT_SINE, CGUIEasing::EASE_IN_OUT_SINE,
                                    GETGUI.GetHalfWidth() - pkMessageBox->GetHalfWidth(),
                                    GETGUI.GetHalfHeight() - pkMessageBox->GetHalfHeight(),
                                    eC_FromInt(0),
                                    eC_FromInt(0), 1000);

    pChain->AddAnimation(pAnimation,0);

    pAnimation = new CGUIAnimationMove(pkMessageBox, CGUIEasing::EASE_IN_OUT_SINE, CGUIEasing::EASE_NONE,
                                    eC_FromInt(0),
                                    eC_FromInt(0),
                                    GETGUI.GetWidth() - pkMessageBox->GetWidth(),
                                    eC_FromInt(0), 1000);

    pChain->AddAnimation(pAnimation,2000);

    // Start the animation chain.
    pChain->StartAnimation();

@endcode

@ingroup GUILIANI_ANIMATIONS
*/
class CGUIAnimationChain : public CGUIAnimatable, public CGUIAnimationObserver, public CGUIStreamableObject
{

    friend class CGUIAnimationHandler;
public:
    /// Defines the chain type.
    enum AnimationChain_t
    {
        PARALLEL_ANIMATION, ///< Each animation is started based on its offset relative to the start of the animation chain.
        CHAINED_ANIMATION, ///< The animations are started one after the other. The offset is used as time between the animations.
    };

    /**
        @brief Holds the animation id and the offset for each animation.
    */
    struct AnimationInfo_t
    {
        /** The animation */
        CGUIAnimation* m_pAnimation;

        /** The time offset which was set for the animation. */
        eC_UInt m_uiOffset;
    };

    /// The animation list is a double-linked list of animation info structs
    typedef eC_TListDoubleLinked<AnimationInfo_t> AnimationList_t;

    /**
    Constructor
    @param eType The type of the animation chain.
    @param bCyclic Defines if the chain starts at the beginning again, when the last animation is finished.
    */
    CGUIAnimationChain(const AnimationChain_t eType, const eC_Bool bCyclic = false);

    ///Destructor
    virtual ~CGUIAnimationChain();

    /**
    Add an animation. The animation is owned and handled by the AnimationChain.
    @param pAnimation The animation to add.
    @param uiTimeOffset The offset relative to the previous animation in case of CHAINED_ANIMATION
    or relative to the animation start in case of PARALLEL_ANIMATION.
    */
    void AddAnimation(CGUIAnimation* pAnimation, eC_UInt uiTimeOffset = 0);

    /**
    Start the animation chain.
    All contained animations will be triggered based on their offsets
    and on the order in which they were added ( only for CHAINED_ANIMATION).
    */
    void StartAnimation();

    /**
    Stops all animations which are part of this chain.
    */
    void StopAnimation();

    /**
    Pause the execution of an animation chain.
    This includes pausing of the gaps (time offset) between animations.
    The timer callback will be removed.
    */
    void PauseAnimation();

    /**
    Continue the execution of the animation chain. The timer callback will be restored (installed again).
    */
    void ContinueAnimation();

    /**
        Checks if the animation chain is currently paused.
        @return True if an animation chain is currently paused
    */
    eC_Bool IsPaused() const;

    /**
        Checks if the animation chain is currently animating.
        @return True if an animation callback is registered, otherwise False.
    */
    virtual eC_Bool IsAnimating() const;

    /**
        Timer callback for the animation chain which is used to handle the animation offsets.
        @param vTimes Ignored by base class, used by implemented animation to compensate for jitter
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /**
        Activate or deactivate cyclic animation. The chain will restart at the beginning after the last animation finished,
        if cyclic animation is activated.
        @param bCyclicAnimation True to activate cyclic animation, false to deactivate.
    */
    void SetCyclicAnimation(const eC_Bool bCyclicAnimation) { m_bCyclic = bCyclicAnimation; }

    /**
        @return True if cyclic animation is activated, else False.
    */
    eC_Bool GetCyclicAnimation() const { return m_bCyclic;}

    /**
        Called by the animations when they are finished or deleted.
        This is used for chained animations.
        @param eStatus The new status of the animation.
        @param pAnimation The Animation whose status is updated.
    */
    void OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus,
        CGUIAnimation* pAnimation);

    /**
    @return The unique id of the animation chain.
    */
    eC_UInt GetID() {return m_uiID;}

    /**
    @return The type of the animation chain.
    */
    AnimationChain_t GetChainType() {return m_eChainType;}

    /**
    @param uiID The animation ID
    @return The animation with the ID "uiID" if part of the animation chain else NULL
    */
    const CGUIAnimation* GetAnimation(eC_UInt uiID);

    /**
    @return The list of animations which are part of the chain.
    */
    const AnimationList_t* GetAnimations() {return &m_AnimationList;}

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt ANIMATION_CHAIN_CLASS_VERSION; ///< The class version of the animation chain class
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();

    /**
    Default constructor used by the factory.
    */
    CGUIAnimationChain();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);

#endif

private:

    ///Start the next animation and register a timer for the next one.
    void StartNextAnimation();

    /// Returns the animation that will finish as the last animation in the chain.
    AnimationList_t::Iterator GetLastAnimation() const;

    /// Holds all animations that where added.
    AnimationList_t m_AnimationList;

    /// Iterator pointing to the last started animation.
    AnimationList_t::SafeIterator m_NextAnimation;

    /// The type of the animation chain.
    AnimationChain_t m_eChainType;

    /// Holds the time when the animation chain was started (in ms).
    eC_UInt m_uiStartTime;

    ///< Holds the already expired time of the animation chain. This is used to continue a previously paused animation chain (!= 0 means paused).
    eC_UInt m_uiExpiredTime;

    ///< Holds the time when an animation chain callback is registered. Used to calculate remaining time.
    eC_UInt m_uiOffsetSave;

    ///< Holds the remaining time of the animation chain if a chain is paused. This is used to continue a previously paused animation chain.
    eC_UInt m_uiOffsetRemaining;

    /// If true the animation chain triggers the first animation again, when the last animation is finished.
    eC_Bool m_bCyclic;

    /// Unique ID of the AnimationChain
    eC_UInt m_uiID;
};

#endif //GUIANIMATION_CHAIN__H_
