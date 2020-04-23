/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(SCROLLING_TEXT)
#define SCROLLING_TEXT

#include "GUIText.h"
#include "GUIAnimatable.h"

class CGUIObject;

/**
    The scrollable text is a specialization of the GUIText which implements text scrolling.
    It is possible to scroll always, only when the parent object is focused, or when the text
    is truncated.
    
    It is possible to scroll the text horizontally or vertically.
    The animation speed can be configured by setting the animation interval and the animation step size.
    
    When scrolling, the text will always be clipped on the original text dimensions.
    
<h3>Platform Specific Limitations </h3>
*/
// @guiliani_doxygen limitations CGUIScrollingText    
/**
    GUIScrollingText Text alignments will have no effect in the direction of scrolling. I.e., vertical alignment will be ignored for vertically scrolling text.(Class CGUIScrollingText)
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CGUIScrollingText "CGUIScrollingText Platform Specific Limitations". 
    @brief A specialization of the GUIText which implements text scrolling
*/
class CGUIScrollingText : public CGUIText, public CGUIAnimatable
{
public:

     /** Constructs a scrolling string text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param kText         Reference of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIScrollingText(
        const CGUIObject* const pParentObject,
        const eC_String &kText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Constructs a scrolling string text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param pkText        Pointer of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
     CGUIScrollingText(
        const CGUIObject* const pParentObject,
        const eC_String *const pkText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

     /** Constructs a scrolling ID text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param eTextID       ID of the internationalized text.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIScrollingText(
        const CGUIObject* const pParentObject,
        const TextResource_t &eTextID,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /**
        Constructs a Scrolling text using the attributes of a GUIText.
        @param rkText The GUIText to copy from.
    */
    CGUIScrollingText(const CGUIText& rkText);

    /**
        Copy-constructor.
        @param rkScrollingText The GUIScrollingText to copy from.
    */
    CGUIScrollingText(const CGUIScrollingText& rkScrollingText);

    /** Default constructor used for streaming. */
    CGUIScrollingText();

    /// Possible animation conditions.
    enum ScrollingCondition_t
    {
        SCROLL_ALWAYS,  ///< The text will always be scrolled.
        SCROLL_FOCUSED, ///< The text will be scrolled when its parent object is focused.
        SCROLL_CUT_TEXT_ALWAYS, ///< The text will be scrolled when the text is truncated.
        SCROLL_CUT_TEXT_FOCUSED, ///< The text will be scrolled when the text is truncated and the parent object is focused.
        SCROLL_MANUALLY          ///< The Animation can be started or stopped manually by calling SetAnimationActive().
    };

    /**
        Set the scrolling condition.
        @param eScrollingCondition The Scrolling condition.
        The animation will be started as soon as the condition is fulfilled.
    */
    void SetAnimationMode(const ScrollingCondition_t eScrollingCondition);

    /** Animates the smooth scrolling text.
        @param vTimes The factor of the elapsed time based on the set animation interval.
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Sets the direction of the scrolling text.
        @param bScrollVertical True for vertical scrolling text, false for horizontal scrolling.
    */
    void SetScrollVertical(const eC_Bool bScrollVertical);

    /** Sets the time between to animation steps.
        @param uiAnimationInterval The time between two animation steps in ms.
    */
    void SetAnimationInterval(const eC_UInt uiAnimationInterval);

    /** Sets the animation step size.
        @param iAnimationStepSize The step size in pixel for each animation step.
    */
    void SetAnimationStepSize(const eC_Int iAnimationStepSize);

    /** Activates or deactivates scrolling of the text.
        This call is ignored when the scrolling condition is not set to SCROLL_MANUALLY.
        In this case the animation is ONLY triggered internally.
        @param bAnimationActive True if text should animate, False if not.
    */
    void SetAnimationActive(const eC_Bool bAnimationActive);

    /**
        When not animating the base implementation is called to set the text width.
        When animating, the width of the scroll area is updated.
        The scrolling conditions are checked when the next redraw is triggered.
        @param vTextWidth The new width for the text.
    */
    void SetTextWidth(const eC_Value &vTextWidth);

    /**
        When not animating the base implementation is called to set the text height.
        When animating, the height of the scroll area is updated.
        The scrolling conditions are checked when the next redraw is triggered.
        @param vTextHeight The new height for the text.
    */
    void SetTextHeight(const eC_Value &vTextHeight);

    /** Get the width of the text area.
        @return The width of the text area.
    */
    eC_Value GetTextWidth() const;

    /** Get the height of the text area.
        @return The height of the text area.
    */
    eC_Value GetTextHeight() const;


    /**
        When not animating the base implementation is called to set the text position.
        When animating, the position of the scroll area is updated.
        @param vRelXPos The new text position.
    */
    void SetRelXPos(const eC_Value &vRelXPos);

    /**
        When not animating the base implementation is called to set the text position.
        When animating, the position of the scroll area is updated.
        @param vRelYPos The new text position.
    */
    void SetRelYPos(const eC_Value &vRelYPos);

    /** Get the x-position of the text area.
        @return The width of the text area.
    */
    eC_Value GetRelXPos() const;

    /** Get the y-position of the text area.
        @return The height of the text area.
    */
    eC_Value GetRelYPos() const;

    /**
        Calls the base implementation and checks if the cut conditions have
        changed by calling CheckScrollingCondition().
    */
    void AdaptText();

    /**
        Reset the text position and the text size to original values,
        as it is done when the animation is stopped.
    */
    void ResetTextPosition();

    /** Clones the instance of the GUIScrollingText object.
        @return Pointer to the cloned GUIScrollingText object. */
    virtual CGUIScrollingText* Clone() const;

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)

    virtual void WriteToStream(const eC_Bool bWriteTextType=false);
#endif

protected:

    /** Calls CheckScrollingCondition() and calls base class implementation.
        The cliprect is set to ensure that the text is not drawn outside of the original (non animating) text area.
    */
    void PreparePrint();

    /// Calls the base implementation and resets the cliprect that is manipulated in PreparePrint().
    void PrintText();

    /** Checks the scrolling conditions and calls SetAnimationActiveInternal() to start or stop the animation.
        This method has to be called when conditions changed.
    */
    void CheckScrollingCondition();

    /// Helper function for AdaptText() that updates m_bIsTextCut.
    void UpdateIsTextCut();

    /// Determines if the text would be cut if it was not scrolling.
    eC_Bool IsScrollingTextCut() const {return m_bIsTextCut;}

private:
    /// Activates or deactivates the animation. Helper function for SetAnimationActive and PreparePrint.
    void SetAnimationActiveInternal(const eC_Bool bAnimationActive);

    /// Stores the text's original width or height if it is animated.
    eC_Value m_vNonAnimatedTextSize;

    /// Stores the text's original XPos or YPos if it is animated.
    eC_Value m_vNonAnimatedTextPos;

    /// Determines when the animation is started.
    ScrollingCondition_t m_eScrollingCondition;

    /// True for scrolling vertical, else False.
    eC_Bool m_bScrollVertical;

    /// Time between animation steps in milliseconds.
    eC_UInt m_uiAnimationInterval;

    /// The animation speed in pixel per animation step.
    eC_Int m_iAnimationStepSize;

    /// Holds the information if the text is cut or not. This member is updated in AdaptText().
    eC_Bool m_bIsTextCut;

    /// Holds the information if the dimensions of the text where updated.
    eC_Bool m_bTextAttributesUpdated;
};

#endif // SCROLLING_TEXT
