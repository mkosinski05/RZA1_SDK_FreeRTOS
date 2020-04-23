/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUISCROLLINGTEXTFIELD_H)
#define GUISCROLLINGTEXTFIELD_H

#include "GUITextField.h"

/** @brief Draws a smooth scrolling text field with configurable speed.
@deprecated
    With the introduction of CGUIScrollingText this class is redundant and will be removed in future releases.
*/
class CGUIScrollingTextField : public CGUITextField
{
public:
    /** Constructs a scrolling text field from a string and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the textfield
        @param vHeight Height of the textfield
        @param kText text string to be displayed on the textfield
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param bAnimationActive Used for saving the current state of animation (on/off).
        @param bScrollVertical True for scrolling vertical, else False.
        @param iAnimationTime The time between two animating steps.
        @param iAnimationStepSize StepSize in pixels for each animation step.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
     */
    CGUIScrollingTextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_Bool& bAnimationActive = false,
        const eC_Bool& bScrollVertical = false,
        const eC_Int &iAnimationTime = 30,
        const eC_Int &iAnimationStepSize = 1,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a scrolling text field from a string and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param kText text string to be displayed on the textfield
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param bAnimationActive Used for saving the current state of animation (on/off).
        @param bScrollVertical True for scrolling vertical, else False.
        @param iAnimationTime The time between two animating steps.
        @param iAnimationStepSize StepSize in pixels for each animation step.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIScrollingTextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_Bool& bAnimationActive = false,
        const eC_Bool& bScrollVertical = false,
        const eC_Int &iAnimationTime = 30,
        const eC_Int &iAnimationStepSize = 1,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a scrolling text field from a text ID and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the textfield
        @param vHeight Height of the textfield
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param bAnimationActive Used for saving the current state of animation (on/off).
        @param bScrollVertical True for scrolling vertical, else False.
        @param iAnimationTime The time between two animatin steps.
        @param iAnimationStepSize StepSize in pixels for each animation step.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIScrollingTextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_Bool& bAnimationActive = false,
        const eC_Bool& bScrollVertical = false,
        const eC_Int &iAnimationTime = 30,
        const eC_Int &iAnimationStepSize = 1,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a scrolling text field from a text ID and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param bAnimationActive Used for saving the current state of animation (on/off).
        @param bScrollVertical True for scrolling vertical, else False.
        @param iAnimationTime The time between two animating steps.
        @param iAnimationStepSize StepSize in pixels for each animation step.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIScrollingTextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_Bool& bAnimationActive = false,
        const eC_Bool& bScrollVertical = false,
        const eC_Int &iAnimationTime = 30,
        const eC_Int &iAnimationStepSize = 1,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Default constructor to be used by the factory.
    CGUIScrollingTextField();

    /** Common code for all constructors
        @param bAnimationActive Whether animation is active
        @param bScrollVertical Whether to scroll vertically
        @param iAnimationTime Animation time
        @param iAnimationStepSize Animation step size
    */
    void Init(const eC_Bool bAnimationActive, const eC_Bool bScrollVertical, const eC_Int iAnimationTime, const eC_Int iAnimationStepSize);

    /** Copy constructor
        @param kSource Where to copy from
    */
    CGUIScrollingTextField(const CGUIScrollingTextField& kSource);

    /** Assignment operator
        @param kSource Where to copy from
        @return Where to assign to
    */
    CGUIScrollingTextField& operator =(const CGUIScrollingTextField& kSource);

    /// Destructor
    ~CGUIScrollingTextField();

    /** Activates or deactivates scrolling of the text
        @param bAnimationActive True if text should animate, False if not.
      */
    void SetAnimationActive(eC_Bool bAnimationActive);

    /// Returns if scrolling is active.
    eC_Bool GetAnimationActive() const;

    /** Sets the direction of the scrolling text.
        @param bScrollVertical True for vertical scrolling text, false for horizontal scrolling
      */
    void SetScrollVertical(eC_Bool bScrollVertical);

#if defined(GUILIANI_STREAM_GUI)
    /** Reads all object attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all object attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of ControlID,
               leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

};

#endif // GUISCROLLINGTEXTFIELD_H
