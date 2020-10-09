/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASEBUTTON__H_
#define GUIBASEBUTTON__H_

#include "GUIObject.h"
#include "GUITextResource.h"
#include "eC_Types.h"
#include "GUICommand.h"

class CGUI;
class CGUIText;

///Base class for button controls.

/** Like all the "BaseXXXXX"-Classes, this serves primarily as base-class, from
    which you can derive your own customized objects.
    For a Button implementation which uses images to visualize itself, please refer
    to CGUIButton.

    A button is an object that looks differently depending on its state
    (normal, highlighted, focused, pressed or grayed-out). Typically, a button
    does something when it is clicked. The behavior of a button can be
    implemented mainly in three ways:
    - The most elegant solution is to attach a CGUICommand object to the
      button. The command is executed each time the button is clicked. This is
      also a very modular approach since commands can be changed any time, so
      special derived button classes can be re-used easily. See SetCommand()
      and constructors.
    - A less elegant and less modular approach is to derive a class from this one
      and re-implement DoClick(). Since the implementation will tend to be very
      specific, the button will not be very reusable. It is of course possible
      to add further attributes to make it more flexible, but this could also
      more easily be done with commands.
    - The last way is to simply ignore DoClick() and not attach any commands.
      In this case, the Guiliani framework will call the button's parent
      composite object's DoClick() in which it can be determined which control
      was clicked (by its ID, or worse, by pointer comparison). Since this will
      usually lead to bloated and hard-to-maintain code, it is strongly
      discouraged to use this approach.

      Example:
@code
    // Create base button
    CGUIBaseButton *pBaseButton = new CGUIBaseButton(this,
                                         eC_FromInt(0),
                                         eC_FromInt(0),
                                         eC_FromInt(50),
                                         eC_FromInt(50),
                                         "Button1",
                                         NULL,  // Replace the NULL with your command.
                                         OBJ_BT_BUTTON1);

    // Set the text of the button.
    pBaseButton->SetLabel("ok");

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseButton :
    public CGUIObject
{
public:
    /** Constructs a button from relative coordinates and a text ID.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the button.
        @param vHeight Height of the button.
        @param eTextID ID of text string to be displayed on the button.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this button (choose NO_HANDLE if none is required).
    */
    CGUIBaseButton(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const TextResource_t &eTextID, CGUICommandPtr pCommandObject, const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a button from a rectangle and a text ID.
        @param pParent Pointer to the designated parent object
        @param kRect Boundary rectangle relative to parent.
        @param eTextID ID of text string to be displayed on the button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIBaseButton(CGUICompositeObject *const pParent, const CGUIRect &kRect, const TextResource_t &eTextID, CGUICommandPtr pCommandObject, const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a button from relative coordinates and a string.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the button.
        @param vHeight Height of the button.
        @param kText Text string that will be displayed on the button.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this button
    */
    CGUIBaseButton(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const eC_String& kText, CGUICommandPtr pCommandObject, const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a button from a rectangle and a string.
        @param pParent Pointer to the designated parent object.
        @param kRect Boundary rectangle relative to parent.
        @param kText Text string that will be displayed on the button.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this button
    */
    CGUIBaseButton(CGUICompositeObject *const pParent, const CGUIRect &kRect, const eC_String& kText, CGUICommandPtr pCommandObject, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIBaseButton standard constructor.
        @see CGUIObject().
    */
    CGUIBaseButton();

    /** CGUIBaseButton copy-constructor.
        Command is set to NULL.
        @param kSource Source object to be copied
    */
    CGUIBaseButton(const CGUIBaseButton& kSource);

    /** CGUIBaseButton operator= to copy the whole CGUIBaseButton.
        Command is set to NULL.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIBaseButton& operator=(const CGUIBaseButton& kSource);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Destructor, deletes command associated with DoClick() */
    virtual ~CGUIBaseButton(void);

    /** Sets the text label of this button.
        @see CGUIText::SetTextString()
        @param rkValue The CGUIValue containing the new value
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue);

    virtual eC_Bool DoDraw();

    /// If a command object is attached to the button, it will be executed when the button gets clicked.
    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /** Attaches a command to this button. This command is executed whenever
        the button is clicked.
        @param pCommand The command to attach to this button.
    */
    inline void SetCommand(CGUICommandPtr pCommand) { m_pCommandObject = pCommand; }

    /** Returns the command object that is currently attached to this button.
        @return The command object that is currently attached to this button,
        or NULL if this button does not have a command.
    */
    CGUICommandPtr GetCommand() const { return m_pCommandObject; }

    /** @return A pointer to the text object that represents the caption of the button.
        To accomplish a "SetLabel" functionality to change the label, you can use
        these calls: GetLabel()->SetTextString(newString)
    */
    CGUIText *GetLabel() const;

    /** Changes the label on the button to a new CGUIText with string.
        The attributes of the text such as its font, color, size etc. will be
        copied. Subsequent calls to GetLabel() will return a CGUIText
        object.
        @param kString The new caption of the button.
        @remark If this button does not have a label at the time this method is
                called, a new CGUIText object with default attributes is created.
    */
    void SetLabel(const eC_String& kString);

    /** Changes the label on the button to a new CGUIText with ID.
        The attributes of the text such as its font, color, size etc. will be
        copied. Subsequent calls to GetLabel() will return a CGUIText
        object.
        @param eTextID The new caption of the button.
        @remark If this button does not have a label at the time this method is
                called, a new CGUIText object with default attributes is created.
    */
    void SetLabel(const TextResource_t& eTextID);

    /** Change the internal text object. This method deletes the old text.
        Use this to exchange the default CGUIText control created by this
        control object with another (possibly user-implemented) text.
        If you only want to change the text content, use SetLabel().
        @param pkText The new text object.
      */
    void SetText (CGUIText* pkText);

    /** Calls base class implementation and adjusts the size of the CGUIText
        with the same width difference (i.e., the pixel distance of the text's
        right edge to the button's right edge stays the same).
        @param vNewWidth The new width.
      */
    virtual void SetWidth(const eC_Value& vNewWidth);

    /** Calls base class implementation and adjusts the size of the CGUIText
        with the same height difference (i.e., the pixel distance of the text's
        bottom edge to the button's bottom edge stays the same).
        @param vNewHeight The new height.
      */
    virtual void SetHeight(const eC_Value& vNewHeight);

protected:
    /// Holds the attached command
    CGUICommandPtr m_pCommandObject;

    /// Holds the label
    CGUIText       *m_pkLabel;
private:
    /** CGUIBaseButton CopyAttributes method for copy the attributes in this class
        @param kSource Source object
    */
    void CopyAttributes(const CGUIBaseButton& kSource);

    void Init(const TextResource_t &eTextID);
    void Init(const eC_String& kText);

    /** Performs de-initializations that need to be done on destruction or
        before reading from stream. */
    void DeInit();
};
#endif
