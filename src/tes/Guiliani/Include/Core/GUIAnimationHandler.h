/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if ! defined GUIANIMATIONHANDLER__H_
#define GUIANIMATIONHANDLER__H_

#include "eC_TList_doubleLinked.h"
#include "GUIAnimation.h"

class CGUIObject;
class CGUIAnimationObserver;
class CGUIAnimationChain;

#define GETANIMATIONHDL CGUIAnimationHandler::GetInstance()

/// Version of animation files.
const eC_Int ANIMATIONS_FILE_VERSION = 1;

/** @brief The AnimationHandler is the management class for animations.

It takes ownership for all animations and animation chains.
When a GUIObject is deleted, the AnimationHandler is informed
and deletes all animations that are animating this object.

When an animation or an animation chain is created, they are registered in the animation handler.
The AnimationHandler will generate a unique id and sets it in the generated animation or animation chain.

When an animation, an animation chain or an animation observer is deleted, the animation handler is informed.
The respective destructor calls NotifyOfDestruction().

The animation handler as owner of animations provides the interface to access an animation or an animation chain
based on the unique id which was set when the object was instantiated

Store the unique id when creating the animation.
@code
CGUIAnimationMove* pAnimation = new CGUIAnimationMove(pkAnimatedObject, CGUIEasing::EASE_NONE, CGUIEasing::EASE_NONE, uiDuration);
m_myAnimationID = pAnimation->GetID();
@endcode

Later access the animation by the ID to start it.
@code
CGUIAnimationMove* pAnimation = GETANIMATIONHDL.GetAnimation(m_myAnimationID);

if (pAnimation != NULL)
{
    pAnimation->StartAnimation();
}
@endcode

It is also possible to check if an GUIObject has animations (i.e. if there are animations that are animating the object)
by calling ObjHasAnimations() and to get the list of all animations by calling GetAnimations().

@ingroup GUILIANI_ANIMATIONS
*/

class CGUIAnimationHandler
{
    friend class CGUIAnimation;
    friend class CGUIAnimationChain;
    friend class CGUILoadAnimationsCmd;
public:

    /// The animation pointer list is a double-linked list of animation pointers
    typedef eC_TListDoubleLinked<CGUIAnimation*> AnimationPtrList;

    /// The animation chain pointer list is a double-linked list of animation chain pointers
    typedef eC_TListDoubleLinked<CGUIAnimationChain*> AnimationChainPtrList;

    /// @return The instance handle
    static inline CGUIAnimationHandler& GetInstance(){return ms_kAnimationHandler;}

    /**
        Delete and remove all remaining animations and animation chains.
    */
    void DeInit();

    /**
    Called by the destructor of CGUIObject.
    Removes all animations of the object.
    @param pDeletedObject The object which was deleted.
    */
    void NotifyOfDestruction(CGUIObject* pDeletedObject);

    /** Called by the destructor of CGUIAnimation.
    The Animation will be removed from the internal list.
    @param pAnimation The destroyed animation.
    */
    void NotifyOfDestruction(CGUIAnimation* pAnimation);

    /** Called by the destructor of CGUIAnimationChain.
    The Animation will be removed from the internal list.
    @param pAnimationChain The destroyed animation chain.
    */
    void NotifyOfDestruction(CGUIAnimationChain* pAnimationChain);

    /**
    Called whenever a AnimationObserver is deleted.
    Sets the observer pointer for all animations that are using this observer to NULL.
    @param pObserver The animation observer which will be deleted.
    */
    void NotifyOfDestruction(CGUIAnimationObserver* pObserver);

    /**
    @param uiID The unique ID of the Animation.
    @return Pointer to the animation that is mapped to the id
            or NULL if the id is invalid.
    */
    CGUIAnimation* GetAnimation(eC_UInt uiID) const;

    /**
        Returns a list of all animations that are animating an object.
        @param pkObject The object whose animations shall be returned.
        @return A list of animation pointers
    */
    AnimationPtrList GetAnimationsOfObject(const CGUIObject* pkObject);

    /**
    @param uiID The unique ID of the AnimationChain.
    @return Pointer to the animation chain that is mapped to the id
            or NULL if the id is invalid.
    */
    CGUIAnimationChain* GetAnimationChain(eC_UInt uiID) const;

    /**
    Get all existing animation chains.
    @return Pointer to a list of all existing animation chains.
            Value is NULL if no chain exists.
    */
    const AnimationChainPtrList *GetAnimationChains(void) const;

    /**
    Check if an animation was added that animates an object.
    @param pObjToCheck The object to check.
    @return True if animations for pObjToCheck are available, otherwise False.
    */
    eC_Bool ObjHasAnimations(const CGUIObject* pObjToCheck) const;

    /**
    Get all animations that are mapped to an object.
    @param pObjToCheck The GUIObject that is animated.
    @return A list of all animations that are animating the object.
    */
    AnimationPtrList GetAnimations(const CGUIObject* pObjToCheck) const;

    /**
    Stops all animations that are animating a specific GUIObject.
    @param pAnimatedObject The Object that is animated by the animations that shall be stopped.
    */
    void StopAllAnimations(CGUIObject* pAnimatedObject);

    /**
    Stops all animations that were added to the animation handler.
    */
    void StopAllAnimations();

#ifdef GUILIANI_STREAM_GUI
    /**
    Read all animations and animation chains from file and adds them to the internal animation list.
    @param kPath The path to the animation file.
    @return True if all animations were successfully read, otherwise False.
    */
    eC_Bool ReadAnimationsFromFile(const eC_String& kPath);
#endif

#ifdef GUILIANI_WRITE_GUI
    /**
    Write all animations and animation chains to file.
    @param kPath The path to the animation file.
    @return True if all animations were successfully written, otherwise False.
    */
    eC_Bool WriteAnimationsToFile(const eC_String& kPath);
#endif

private:
    /**
    Constructor.
    */
    CGUIAnimationHandler() {}

    /**
    Destructor.
    */
    ~CGUIAnimationHandler();

    /**
    Add a new animation to the animation handler.
    This is done in the constructor of the animation base class.
    The animation is owned by the AnimationHandler.
    An unique animation id is created and AddAnimationInternal is called to add the animation to the internal list.
    @param pNewAnimation The new animation.
    @return the unique id for the animation.
    */
    eC_UInt AddAnimation(CGUIAnimation* pNewAnimation);

    /**
    Add a new animation to the internal animation list.
    @param pNewAnimation The new animation.
    */
    void AddAnimationInternal(CGUIAnimation* pNewAnimation);

    /**
    Add a new animation chain to the animation handler.
    This is done in the constructor of the animation chain.
    The animation chain is owned by the AnimationHandler.
    An unique animation chain id is created and AddAnimationChainInternal
    is called to add the animation chain to the internal list.
    @param pAnimationChain The new animation chain.
    @return the unique id for the animation chain.
    */
    eC_UInt AddAnimationChain(CGUIAnimationChain* pAnimationChain);

    /**
    Adds an animation chain to the internal list.
    The animation chain is owned by the AnimationHandler.
    @param pAnimationChain The new animation chain.
    */
    void AddAnimationChainInternal(CGUIAnimationChain* pAnimationChain);

    /**
    Remove an animation from the animation handler.
    The animation will be deleted.
    @param pAnimationToRemove Pointer to the animation that shall be removed.
    @return True if the animation was removed, False if the animation was not contained in the AnimationHandler.
    */
    eC_Bool RemoveAnimation(CGUIAnimation* pAnimationToRemove);

    /**
    Remove an animation from the animation handler.
    The AnimationChain will be deleted.
    @param pAnimationChainToRemove Pointer to the animation chain that shall be removed.
    @return True if the animation chain was removed, False if the animation chain was not contained in the AnimationHandler.
    */
    eC_Bool RemoveAnimationChain(CGUIAnimationChain* pAnimationChainToRemove);

    /**
    Remove all animations that are animating a specific GUIObject.
    @param pAnimatedObject The Object that is animated by the animations that shall be removed.
    */
    void RemoveAllAnimations(CGUIObject* pAnimatedObject);

    /**
    Removes all animations that were added to the animation handler.
    */
    void RemoveAllAnimations();
    /**
    Removes all animation chains that were added to the animation handler.
    */
    void RemoveAllAnimationChains();

    AnimationPtrList m_Animations; ///< Internal list of all added animations.

    AnimationChainPtrList m_AnimationChains; ///< Internal list of all animation chains.

    static CGUIAnimationHandler ms_kAnimationHandler; ///< Static instance.
};

#endif
