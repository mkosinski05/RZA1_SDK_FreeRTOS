/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIPRIMITIVE_BUTTON__H_
#define GUIPRIMITIVE__H_

#include "GUICompositeObject.h"
#include "GUITextResource.h"
#include "GUICommand.h"
#include "GUIText.h"
/// Button specialization that uses images for graphical representation.

// @guiliani_doxygen toplevel_control Button
/**
    <table border="0">
        <tr>
            <td width="200">@image html button.png</td>
            <td>
                The "button" control is used to trigger commands. The individual states of the button are visualized by different images. 
                Five different states are supported: Normal, highlighted, pressed, grayed out and focused. The "button" control is one of the central elements 
                of a touchscreen based application (Class: CGUIButton). The "button" control is offering comfortable means to zoom a button with only minimal impact on
                the button borders. The button can be divided into fully user-defined areas (Top, Bottom, Left, Right).
                The corner areas will remain completely untouched, while the border patches will be stretched in one direction only. 
                Only the center area will be stretched in two dimensions (CGUINinePatch).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    Please refer to the base class (GUIBaseButton) for a general explanation of the button object.
    This specialization differs from its base class by visualizing itself using images. You may assign
    one image to each of the button's states using the SetImages() method.
    Note that the supplied images will be stretched to the Button's size, which may result in reduced visual
    quality if the image's size and the button's size differ significantly.
    If you need to stretch your button, consider using its Nine Patch functionality. (see CGUINinePatch)

    Example:
@code

    // Create a button.
    CGUIButton* pkButton = new CGUIButton(this,
                                    eC_FromInt(0), eC_FromInt(0),
                                    eC_FromInt(15), eC_FromInt(15),
                                    "OK",
                                    NULL,   // Replace the NULL with your command.
                                    OBJ_BUTTON);
    // Set the images of the button.
    // Before calling this method, please add image IDs to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() method to register images for the IDs.
    pkButton->SetImages(
                    IMG_BT_STANDARD,
                    IMG_BT_PRESSED,
                    IMG_BT_HIGHLIGHTED,
                    IMG_BT_GRAYEDOUT,
                    IMG_BT_FOCUSED );

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIPrimitiveButton :
    public CGUICompositeObject
{
public:
    /** Constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the button
        @param vHeight Height of the button
        @param rkText Text string that will be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button
    */
    CGUIPrimitiveButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor.
        @param pParent Pointer to the designated parent object
        @param kRect Boundary rectangle of the button relative to the parent.
        @param rkText Text string that will be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button
    */
    CGUIPrimitiveButton(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the button
        @param vHeight Height of the button
        @param eTextID ID of text string to be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIPrimitiveButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor.
        @param pParent Pointer to the designated parent object
        @param kRect Boundary rectangle of the button relative to the parent.
        @param eTextID ID of text string to be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIPrimitiveButton(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton constructor which sets width and height depending on the
        size of the given standard image. 
        The objects will be added to the CompositeObject.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param eTextID ID of text string to be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed
               in reaction to a click
        @param eBtnStandard ImageID for button in standard state
        @param eBtnPressed ImageID for button in pressed state
        @param eBtnHighlighted ImageID for button in highlight state
        @param eBtnGrayedOut ImageID for button in grayed out state
        @param eBtnFocused ImageID for button in focused state
        @param eID Object Identifier of this button.
    */
    CGUIPrimitiveButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eBtnStandard,
        const ObjectHandle_t &eBtnPressed = NO_HANDLE,
        const ObjectHandle_t &eBtnHighlighted = NO_HANDLE,
        const ObjectHandle_t &eBtnGrayedOut = NO_HANDLE,
        const ObjectHandle_t &eBtnFocused = NO_HANDLE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor.
        @param kSource Source object to be copied
    */
    CGUIPrimitiveButton(const CGUIPrimitiveButton& kSource);

    /** operator= to copy the whole CGUIPrimitiveButton.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIPrimitiveButton& operator=(const CGUIPrimitiveButton& kSource);

    /** Standard constructor
        @see CGUIObject().
    */
    CGUIPrimitiveButton();

    /** Destructor */
    virtual ~CGUIPrimitiveButton();

    /**
        Sets object handles for the button states. 
        Five handles for the five supported states of an image button.
        The handles will be added to the CompositeObject.
        @param eBtnStandard ImageID for button in standard state
        @param eBtnPressed ImageID for button in pressed state
        @param eBtnHighlighted ImageID for button in highlight state
        @param eBtnGrayedOut ImageID for button in grayed out state
        @param eBtnFocused ImageID for button in focused state
    */
    void SetHandles(
        ObjectHandle_t eBtnStandard,
        ObjectHandle_t eBtnPressed,
        ObjectHandle_t eBtnHighlighted,
        ObjectHandle_t eBtnGrayedOut,
        ObjectHandle_t eBtnFocused);

    /** Retrieves currently set object handles for the button states.
        @param eBtnStandard ImageID for button in standard state
        @param eBtnPressed ImageID for button in pressed state
        @param eBtnHighlighted ImageID for button in highlight state
        @param eBtnGrayedOut ImageID for button in grayed out state
        @param eBtnFocused ImageID for button in focused state
    */
    void GetHandles(
        ObjectHandle_t& eBtnStandard,
        ObjectHandle_t& eBtnPressed,
        ObjectHandle_t& eBtnHighlighted,
        ObjectHandle_t& eBtnGrayedOut,
        ObjectHandle_t& eBtnFocused);

    /** @return A pointer to the text object that represents the caption of the button.
        To accomplish a "SetLabel" functionality to change the label, you can use
        these calls: GetLabel()->SetTextString(newString)
    */
    CGUIText *GetLabel() const;

    /** Attaches a command to this button. This command is executed whenever
    the button is clicked.
    @param pCommand The command to attach to this button.
    */
    inline void SetCommand(CGUICommandPtr pCommand) { m_pkCommandObject = pCommand; }

    /** Returns the command object that is currently attached to this button.
    @return The command object that is currently attached to this button,
    or NULL if this button does not have a command.
    */
    CGUICommandPtr GetCommand() const { return m_pkCommandObject; }

    /// Overloaded methods
    virtual eC_Bool DoDraw();
    
    virtual eC_Bool DoClick(const eC_Value & vAbsX = eC_FromInt(-1), const eC_Value & vAbsY = eC_FromInt(-1));
   
    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY); 

    virtual eC_Bool DoButtonUp(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoMouseEnter(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoMouseLeave(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoMouseMove(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual void GetFocus();

    virtual void LoseFocus();
    
    virtual void SetGrayedOut(const eC_Bool &bGrayOut);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Button States
    enum ButtonState_t
    {
        BUTTON_STATE_PRESSED,         ///< Pressed state
        BUTTON_STATE_HIGHLIGHTED,     ///< Highlighted state
        BUTTON_STATE_STANDARD,        ///< Standard state
        BUTTON_STATE_GRAYEDOUT,       ///< Grayed out state
        BUTTON_STATE_FOCUSED          ///< Focused state
    };

    /// Current button state
    ButtonState_t m_eButtonState;

    /** Sets visibility state if the Object Id, which represents the given button state 
        to visible. All others will be set to invisible.
    */
    virtual void SetButtonState();

    ObjectHandle_t m_eBtnStandard;    ///< the standard Object ID
    ObjectHandle_t m_eBtnPressed;     ///< the pressed Object ID
    ObjectHandle_t m_eBtnHighlighted; ///< the highlighted Object ID
    ObjectHandle_t m_eBtnGrayedOut;   ///< the grayed out Object ID
    ObjectHandle_t m_eBtnFocused;     ///< the focused Object ID
    CGUICommandPtr m_pkCommandObject; ///< Holds the attached command
    
    CGUIText* m_pkLabel;              ///< Label of button

private:
    /// Helper for constructors.
    void Init(const eC_String& kText);
    /// Helper for constructors.
    void Init(const TextResource_t &eTextID);
    /// Helper for destructor and ReadFromStream.
    void DeInit();
};
#endif //GUIPRIMITIVE_BUTTON__H_
