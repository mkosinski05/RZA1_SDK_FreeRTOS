/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIANIMATION_STDGUIOBJECT__H_
#define GUIANIMATION_STDGUIOBJECT__H_

#include "GUIAnimation.h"
#include "GUIEasing.h"

/// The attributes of an animation
enum Animated_Attributes_t
{
    ANI_ATTR_XPOS,
    ANI_ATTR_YPOS,
    ANI_ATTR_WIDTH,
    ANI_ATTR_HEIGHT,
    ANI_ATTR_ALPHA,
    NOF_ANIMATED_ATTRIBUTES // Has to be at the end of the enumeration
};

/**
    This animation animates a standard CGUIObject object through giving access to all
    important object attributes which are x position, y position, width, height and alpha value.
    For all of these attributes a separate CGUIEasing functions is used.

    This example moves and changes the size of an object using CGUIEasing functions.

    @code

    // The animation shall be done in 5 seconds.
    const eC_UInt cuiDuration = 5000;

    // Now create the instance of the animation.
    CGUIAnimationStdGUIObject* pAnimation = new CGUIAnimationStdGUIObject(pkAnimatedObject,
                                                            CGUIEasing::EASE_LINEAR, CGUIEasing::EASE_LINEAR,
                                                            CGUIEasing::EASE_LINEAR, CGUIEasing::EASE_LINEAR,
                                                            CGUIEasing::EASE_LINEAR,
                                                            cuiDuration);

    // To start the animation call the StartAnimation() method of the animation.
    pAnimation->StartAnimation();

    //If the instance of the animation is not available directly use the AnimationHandler.
    eC_UInt uiMyAnimationID = pAnimation->GetID();

    GETANIMATIONHDL.GetAnimation(uiMyAnimationID);

    @endcode
    @brief Generic animation of a standard object
*/
class CGUIAnimationStdGUIObject : public CGUIAnimation
{
public:

#if defined(GUILIANI_STREAM_GUI)
    CGUIAnimationStdGUIObject();
#endif
    ~CGUIAnimationStdGUIObject();

    /**
        Constructor
        @param pAnimatedObject The GUIObject that will be moved.
        @param eEasingX The easing type that is used for the XPos of the object.
        @param eEasingY The easing type that is used for the YPos of the object.
        @param eEasingWidth The easing type that is used for the width of the object.
        @param eEasingHeight The easing type that is used for the height of the object.
        @param eEasingAlpha The easing type that is used for the alpha of the object.
        @param uiDuration The total duration of the animation in milliseconds. Based on the
                          distance the duration determines the animation speed.
        @param uiStepTime The time between two animation steps in milliseconds.
    */
    CGUIAnimationStdGUIObject(CGUIObject* pAnimatedObject,
                            CGUIEasing::EasingType_t eEasingX,
                            CGUIEasing::EasingType_t eEasingY,
                            CGUIEasing::EasingType_t eEasingWidth,
                            CGUIEasing::EasingType_t eEasingHeight,
                            CGUIEasing::EasingType_t eEasingAlpha,
                            eC_UInt uiDuration = 1000,
                            eC_UInt uiStepTime = 25);

    virtual void ApplyAnimation(eC_UInt uiElapsedTime);

    /** Retrieves the target values for this animation from the current status of the supplied GUIObject.
        @param pObj Object whose current attributes will serve as the new target values for this animation
    */
    void SetTargetValues( const CGUIObject* pObj);

    /** Retrieves the start values for this animation from the current status of the supplied GUIObject.
        @param pObj Object whose current attributes will serve as the start values for this animation
    */
    void SetStartValues( const CGUIObject* pObj);

    /** Get the easing type for the nth attribute
        @param uiN index
        @return easing type
    */
    inline CGUIEasing::EasingType_t GetEasingForNthAttribute(eC_UInt uiN) { return m_eEasingType[uiN]; }

    /** Set the easing type type for the nth attribute
        @param uiN index
        @param eEasingType easing type
    */
    void SetEasingForNthAttribute( eC_UInt uiN, CGUIEasing::EasingType_t eEasingType);

    /** Get the start value type for the nth attribute
        @param uiN index
        @return start value
    */
    inline eC_Value GetStartValueForNthAttribute(eC_UInt uiN) { return m_vStart[uiN]; }

    /** Set the start value type for the nth attribute
        @param uiN index
        @param vStart start value
    */
    inline void SetStartValueForNthAttribute(eC_UInt uiN, eC_Value vStart) { m_vStart[uiN] = vStart; }

    /** Get the target value type for the nth attribute
        @param uiN index
        @return target value
    */
    inline eC_Value GetTargetValueForNthAttribute(eC_UInt uiN) { return m_vTarget[uiN]; }

    /** Set the start value type for the nth attribute
        @param uiN index
        @param vTarget target value
    */
    inline void SetTargetValueForNthAttribute(eC_UInt uiN, eC_Value vTarget) { m_vTarget[uiN] = vTarget; }

    /** Get the nth attribute
        @param uiN index
        @return attribute
    */
    eC_Value GetAnimatedObjectNthAttribute(eC_UInt uiN);

    /** Set the nth attribute
        @param uiN index
        @param vValue attribute
    */
    void SetAnimatedObjectNthAttribute(eC_UInt uiN, eC_Value vValue);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// The class version of the implementation.
    static const eC_UInt STDGUIOBJECT_ANIMATION_CLASS_VERSION;
#endif

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    CGUIEasing::EasingType_t m_eEasingType[NOF_ANIMATED_ATTRIBUTES];  ///< The easing types for the nth attribute.
    eC_Value m_vStart[NOF_ANIMATED_ATTRIBUTES];  ///< The start value of the nth attribute.
    eC_Value m_vTarget[NOF_ANIMATED_ATTRIBUTES];  ///< The target value of the nth attribute.
};

#endif
