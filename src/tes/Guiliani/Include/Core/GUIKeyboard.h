/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/
#ifndef GUIKEYBOARD_H_
#define GUIKEYBOARD_H_

#include "GUICompositeObject.h"
#include "GUIBehaviourDecorator.h"
class CGUIButton;

/// A basic on-screen keyboard control

// @guiliani_doxygen toplevel_control Keyboard
/**
    <table border="0">
        <tr>
            <td width="200">@image html onscreen_keyboard.png</td>
            <td>
                The "keyboard" control is a fully customizable. Offering all necessary means for standard QWERTY layout up to phone-style keyboards.
                Application specific symbols or images can be integrated easily.
                There are no limitations in the number or sizes of the keyboard buttons,
                making usage of Guiliani's intelligent layouters for a perfect positioning (Class: CGUIKeyboard).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    This implements a basic OnScreen-Keyboard with four different layouts. (ASCII characters, ASCII capital Letters, Numerical, Symbolic)
    The keyboard's buttons send their corresponding keycodes to a defineable CGUIEdit in the GUI. The CGUIEdit is chosen via the CGUIKeyboard::SetTargetObject() API and
    will be the target for ALL subsequent key presses within ALL existing keyboard layouts.

    The four default layouts can be created via the CreateDefaultLayouts() method. 
    Alternatively you can create custom layouts by attaching a CGUIKeyboardBehaviour to your own custom controls.
    If you wish to use the existing default layouts, but change their visualization, you may inherit your own keyboard from CGUIKeyboard and override the
    CreateKeyboardButton() method. This will be called for all the buttons by CreateDefaultLayouts(). 

    Example showing the creation of a keyboard with default layouts:
@code
    // Create keyboard
    CGUIKeyboard* pkKeyboard = new CGUIKeyboard( pkMainWin, 0,0, eC_FromInt(300), eC_FromInt(100), OBJ_ID_OF_GUIEDIT, OBJ_KEYBOARD);
    // Use default keyboard layouts
    pkKeyboard->CreateDefaultLayouts();
    // Set object which shall receive the keyboard output
    CGUIKeyboard::SetTargetObject( OBJ_ID_OF_CGUIEDIT );
@endcode

    Example of a custom keyboard which uses CGUIBlendButton instead of plain CGUIButton:
@code
class CMyKeyboard : public CGUIKeyboard
{
public:
    CMyKeyboard(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t eTargetObjectID, 
        const ObjectHandle_t &eID = NO_HANDLE) 
        : CGUIKeyboard(pParent, vX, vY, vWidth, vHeight, eTargetObjectID, eID)
    {};

    // Gets called for every button during the creation of the default layouts...
    CGUIButton* CreateKeyboardButton( const eC_String& kLabel, const eC_UInt uiKeyCode, CGUICompositeObject* pParent)
    {
        // Create a CGUIBlendButton
        CGUIBlendButton* pButton = new CGUIBlendButton(pParent, 0,0,0,0, kLabel, NULL, NO_HANDLE);
        if( pButton)
        {
            // Set button attributes, such as Font, Images, NinePatch...
            if( pButton->GetLabel())
            {
                pButton->GetLabel()->SetFont( FNT_KEYBOARD);
            }
            pButton->SetImages( IMG_STDCTRL_KEYBOARDBTN_STANDARD, IMG_STDCTRL_KEYBOARDBTN_PRESSED, IMG_STDCTRL_KEYBOARDBTN_HIGHLIGHTED, IMG_STDCTRL_KEYBOARDBTN_GRAYED_OUT, IMG_STDCTRL_KEYBOARDBTN_FOCUSED);
            pButton->GetNinePatch().Assign(5,5,5,5);
            // Assign a CGUIKeyboardBehaviour which sends the given uiKeyCode when being pressed
            pButton->SetBehaviour( new CGUIKeyboardBehaviour(uiKeyCode) );
        }
        return pButton;
    }
};
@endcode


@ingroup GUILIANI_CONTROLS
*/
class CGUIKeyboard
    : public CGUICompositeObject
{
public:
    /** CGUIKeyboard constructor.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the object
        @param vHeight Height of the object
        @param pParent Pointer to the designated parent object
        @param eTargetObjectID Handle to a CGUIEdit which shall receive the output of the keyboard
        @param eID of this object 
    */
    CGUIKeyboard(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t eTargetObjectID, 
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Standard constructor
    */
    CGUIKeyboard();

    virtual ~CGUIKeyboard();

    /// Creates the default keyboard layouts ( ABC, abc, 123, Symbolic..) and attaches them as separate CGUICompositeObjects to the Keyboard.
    virtual void CreateDefaultLayouts();

    /** Creates a button with the default keyboard-button design, which has a CGUIKeyboardBehaviour attached to it.  
        The CGUIKeyboardBehaviour will write the first letter of the button's label into the target CGUIEdit
        @param kLabel Label to show on the button AND letter to be written into the CGUIEdit.
        @param pParent Parent object into which to add the button.
        @return newly created button object
        */
    virtual CGUIButton* CreateKeyboardButton( const eC_String& kLabel, CGUICompositeObject* pParent);

    /** Creates a button with the default keyboard-button design, which has a CGUIKeyboardBehaviour attached to it.  
        The CGUIKeyboardBehaviour will write uiKeyCode into the target CGUIEdit.
        It is recommended to reimplement this method in derived classes to realize customized keyboard designs.
        @param kLabel Label to show on the button
        @param uiKeyCode UTF16 key code to write into the CGUIEdit
        @param pParent Parent object into which to add the button.
        @return newly created button object
        */
    virtual CGUIButton* CreateKeyboardButton( const eC_String& kLabel, const eC_UInt uiKeyCode, CGUICompositeObject* pParent);

    /** Creates one row of buttons in the keyboard.
        @param pParent Parent object to which the newly created buttons will be attached
        @param uiTotalNOFRowsInKeyboard Total number of rows in the keyboard
        @param uiRowIndex Index of the new row in the keyboard (Must be >= 0 and smaller than uiTotalNOFRowsInKeyboard)
        @param vXOffset Horizontal offset of the first button in pixels from left border of the parent object
        @param uiNOFButtons Number of buttons in this row.
        @param pWidths Array of eC_Values defining the width of the buttons. The array MUST have EXACTLY uiNOFButtons entries and the sum of all widths
                       must equal 1. The widths define the percentual width of the buttons relative to their parent. (see CGUILayouterPercentage) 
        @param pLabels Array of Strings defining the labels to be shown on the buttons. The array MUST have EXACTLY uiNOFButtons entries .
        @return Index of the first created button within pParent's list of child objects.
        */
    eC_UInt CreateKeyboardRow( CGUICompositeObject* pParent, const eC_UInt uiTotalNOFRowsInKeyboard, const eC_UInt uiRowIndex, const eC_Value vXOffset, const eC_UInt uiNOFButtons, const eC_Value* pWidths, const eC_String* pLabels);

    /** Sets the CGUIEdit which will receive the output from all existing keyboards (or to be precise, from all CGUIKeyboardBehaviour instances).
        If no ID was explicitly given, the output will instead be sent to the currently focused object.
        @note If you choose to send the output to the currently focused object, you should make sure that they keyboard and its buttons are NOT focusable!
        @param eTargetObject ID of an instance of CGUIEdit
    */
    static void SetTargetObject(const ObjectHandle_t eTargetObject) { ms_eTargetObject = eTargetObject; }

    /** Forwards the given KeyCode into the corresponding DoKeyDown and DoChar slots of the CGUIEdit identified by SetTargetObject().<br/>
        Note that the following keycodes will be handled by calling DoKeyDown on the CGUIEdit:<br/>
        <table>
            <tr><th>KeyCode</th><th>Name</th>
            <tr><td>0x0008</td><td>GK_BACKSPACE</td>
            <tr><td>0x000D</td><td>GK_ACTION</td>
            <tr><td>0x007F</td><td>GK_DELETE</td>
            <tr><td>0x2190</td><td>GK_LEFT</td>
            <tr><td>0x2192</td><td>GK_RIGHT</td>
        </table>
        @param uiKeycode UTF16 KeyCode which shall be handled.
    */
    static void HandleKey(const eC_UInt uiKeycode);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Initializes the keyboard.
    void Init();

    /// ID of CGUIEdit which shall receive the keyboard's output
    static ObjectHandle_t   ms_eTargetObject;

private:
    /** Dummy implementation to avoid unintended use of compiler-generated default */
    CGUIKeyboard(const CGUIKeyboard& kSource);

    /** Dummy implementation to avoid unintended use of compiler-generated default */
    CGUIKeyboard& operator=(const CGUIKeyboard& kSource);
};


#include "GUIEventHandler.h"
/** Behaviour for use within an OnScreen-Keyboard / Virtual Keyboard.
    This behaviour sends a UTF16 keycode to the Keyboard in reaction to a ButtonUp-Event.
*/
class CGUIKeyboardBehaviour : public CGUIBehaviourDecorator
{
public:
    /** Constructor.
        @param uiKeyCode UTF16 KeyCode (e.g 0x0041 for 'A') which will be sent when this behaviour gets triggered.
    */
    CGUIKeyboardBehaviour(const eC_UInt uiKeyCode=0)
        :CGUIBehaviourDecorator()
        ,m_uiKeyCode(uiKeyCode)
    {
        SetXMLTag("GUIKeyboardBehaviour");
    }

    eC_Bool DoButtonUp(const eC_Value &vAbsX, const eC_Value &vAbsY)
    {
        if( GETEVENTHDL.GetPressedObject() != GetAssociatedObject()) // Avoid handling the key twice, when it was already done in DoClick()
        {
            CGUIKeyboard::HandleKey(m_uiKeyCode);
        }
        return false;
    }
    
    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY)
    {
        CGUIKeyboard::HandleKey(m_uiKeyCode);
        return false;
    }

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Unicode of key which shall be sent to the keyboard (e.g 0x0041 for 'A')
    eC_UInt       m_uiKeyCode;
};

#endif
