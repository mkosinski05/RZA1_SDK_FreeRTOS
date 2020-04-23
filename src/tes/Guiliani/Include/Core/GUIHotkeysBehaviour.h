/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_HOTKEYS_BEHAVIOUR__H_)
#define GUI_HOTKEYS_BEHAVIOUR__H_

#include "GUIBehaviourDecorator.h"
#include "GUIObjectHandleResource.h"
#include "GUICommand.h"
#include "eC_TList_doubleLinked.h"


/// Maps key codes to object IDs and 'clicks' on objects.

/**
This behaviour maintains a list that maps key codes (<i>hot</i> keys) to object
IDs or commands. Whenever one of the key codes is received in an ET_KEYDOWN
event in DoKeyDown(), this behaviour searches the object it is attached to for
the corresponding object ID and, if found, checks for an attached behaviour
decorator. If one is there, its DoClick method is called; if there is none,
the object's own DoClick method is called.

Additionally, in case of a hotkey match, an optional CGUICommand is executed,
which is assigned to the hotkey also. 

@note This command is not used when streaming this behaviour, 
i.e. ReadFromStream() and WriteToStream() ignore the command.

Objects of this behaviour class must be attached to a composite object.
Otherwise an exeception is thrown.

The following example demonstrates the use of this behaviour:

If a hotkey behaviour has a command that is executed, the event slots always
return true, i.e. no other objects are invoked to handle the pressed key.

@code
// Create a container.
CGUICompositeObject* pkComposite = new CGUICompositeObject(...);
// Create a button with the ID OBJ_BTN_EXAMPLE.
pkComposite->AddObject(new CGUIButton(..., OBJ_BTN_EXAMPLE));
// Create a behaviour that maps a hotkey to the button.
CGUIHotkeysBehaviour pkHKB = new CGUIHotkeysBehaviour();
// Map a key (e.g. the F1 key) to the button:
pkHKB->AddHotKeyMap(GK_F1, 0, false, OBJ_BTN_EXAMPLE, false);
// Alternatively, independent of the object, a command can be assigned to a hotkey.
pkHKB->AddHotKeyMap(GK_S, CGUIEvent::GKM_CONTROL, true, NO_HANDLE, false, new CGUISaveCmd());
// Attach the behaviour to the composite containing the button.
pkComposite->SetBehaviour(pkHKB);
@endcode
*/

class CGUIHotkeysBehaviour : public CGUIBehaviourDecorator
{
public:
    /** Maps a GK_ key identifier to a string representation.
        @param eKeyIdentifier Input key.
        @return Printable string representation of the key.
     */
    static eC_String MapKeyToString(GUIKeyIdentifier_t eKeyIdentifier);

    /** Constructor.
        @param bMarkAlwaysAsHandled If true the behaviour will always return
         false after an event has been handled.
     */
    CGUIHotkeysBehaviour(eC_Bool bMarkAlwaysAsHandled = false);

    /** Copy-constructor of the CGUIHotkeysBehaviour class.
        @param kSource Source object to be copied.
    */
    CGUIHotkeysBehaviour(const CGUIHotkeysBehaviour& kSource);

    /** Operator= method of the CGUIHotkeysBehaviour class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIHotkeysBehaviour& operator=(const CGUIHotkeysBehaviour& kSource);

    virtual ~CGUIHotkeysBehaviour() {}

    virtual CGUIBehaviourDecorator* Clone() const;

    /** Adds a key mapping to the list of hot keys.
        @param eKeyIdentifier Hotkey identifier to be mapped.
        @param uiModifiers Which modifier is to consider (CTRL, ALT, etc.)
        @param bCheckForModifiers Should the modifier be considered while
         evaluating the hotkey event?
        @param eMappedObjID Object ID of the object whose DoClick method may be
         called when the key code uiKeyContent is received in an
         ET_KEYDOWN event.
        @param bAbstractObj Indicate whether the object is abstract or not.
         A button which is visible in the GUI is not abstract object. If the
         object is an abstract object, the DoClick method will be executed at
         once if the hot key was pressed. Otherwise the object will first be
         checked whether it is clickable (CGUIObject::IsClickable).
        @param pCommand (optional) This command will be executed additionally if
         the hotkey event occurres. This command is not written to streaming
         files by WriteToStream().
        @param bActivated (optional) Is the hotkey activated, i.e. should events
         associated with the hotkey be handled.
    */
    void AddHotKeyMap(GUIKeyIdentifier_t eKeyIdentifier,
        eC_UInt uiModifiers,
        eC_Bool bCheckForModifiers,
        ObjectHandle_t eMappedObjID,
        eC_Bool bAbstractObj,
        CGUICommandPtr pCommand = NULL,
        eC_Bool bActivated = true);

    /** Sets the given modifier and the flag whether the modifiers should be
        checked or not for given key event.
        @param eKey The key identifier.
        @param uiModifiers The modifier to set this hotkey should handle.
        @param bCheckForModifiers Should modifiers be considered while
         evaluating a hotkey event?
     */
    void SetModifiers(GUIKeyIdentifier_t eKey,
        eC_UInt uiModifiers,
        eC_Bool bCheckForModifiers);

    /** Sets the flag deciding if the slots should always return true or not.
        @param bMarkAlwaysAsHandled If true, the behaviour will always return
         true when an hotkey event is handled. If false, the behaviour will
         return the value that is returned by the called object or its attached
         behaviour.
     */
    void SetMarkAlwaysAsHandled(eC_Bool bMarkAlwaysAsHandled);

    /** Whether the hotkey is activated or not. A deactivated hotkey does not
        handle the specific hotkey events.
        @param eKeyContent The hotkey that is to activate or deactivate.
        @param uiModifiers The modifier corresponding to the hotkey.
        @param bActivated Set true to activate the hotkey or false to deactivate
         it.
     */
    void SetActivated(GUIKeyIdentifier_t eKeyContent,
        eC_UInt uiModifiers,
        eC_Bool bActivated);

    /** Find first occurrence of a hotkey mapping to an object.
        @param reKeyIdentifier [out] If a hot assigned to eMappedObjID is found,
         the key value is returned here.
        @param ruiModifiers [out] If a hot assigned to eMappedObjID is found,
         the modifier is returned here.
        @param eMappedObjID Find the hotkey assigned to this object.
        @return True, if the hotkey map has been found, False otherwise.
     */
    eC_Bool GetHotKey(GUIKeyIdentifier_t& reKeyIdentifier,
        eC_UInt& ruiModifiers,
        ObjectHandle_t eMappedObjID) const;

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /** @brief Definition of one hot key mapping with all necessary parameters.
      */
    struct HotKeyMap_t
    {
        /** Hotkey content. */
        GUIKeyIdentifier_t m_eKeyID;

        /** Modifier */
        eC_UInt m_uiModifiers;

        /** If true the modifier will be evaluated during event handling. */
        eC_Bool m_bCheckForModifiers;

        /** Object ID with which the object pointer can be obtained,
            the DoClick method may be executed of this object.
        */
        ObjectHandle_t m_eMappedObjID;

        /** Indicate whether the object is abstract object or not.
            The button which will be shown in the GUI is not abstract object.
            If the object is abstract object, by clicking the hot key, the DoClick
            method will be executed at once. Otherwise the object will be firstly
            checked whether it is clickable.
        */
        eC_Bool m_bAbstractObj;

        /** Contains a command to be executed, when this hotkey event occurres.
            This is an alternative to eMappedObjID::DoClick() calls and is
            independent of dialogs. The command to be executed is enqueued to
            the GUI command event list.
         */
        CGUICommandPtr m_pCommand;

        /** Set hotkey activated or not. An deactivated hotkey does not handle
            events.
        */
        eC_Bool m_bActivated;

        /** Constructor
            @param eKeyContent Key content
            @param uiModifiers Key modifiers
            @param bCheckForModifiers Whether to check for modifiers
            @param eMappedObjID ID of mapped object
            @param bAbstractObj Whether it's an abstract object
            @param pCommand Command pointer
            @param bActivated True if activated
        */
        HotKeyMap_t(GUIKeyIdentifier_t eKeyContent = GK_NONE,
            eC_UInt uiModifiers = 0,
            eC_Bool bCheckForModifiers = false,
            ObjectHandle_t eMappedObjID = NO_HANDLE,
            eC_Bool bAbstractObj = false,
            CGUICommandPtr pCommand = NULL,
            eC_Bool bActivated = true)
            : m_eKeyID(eKeyContent),
              m_uiModifiers(uiModifiers),
              m_bCheckForModifiers(bCheckForModifiers),
              m_eMappedObjID(eMappedObjID),
              m_bAbstractObj(bAbstractObj),
              m_pCommand(pCommand),
              m_bActivated(bActivated)
        { }
    };
    ///The hot key mappings are saved in this list.
    eC_TListDoubleLinked<HotKeyMap_t> m_kHotKeyMapList;

    /** If an ET_KEYDOWN with one of the added key codes is received, the
        mapped object's DoClick method is called.
        @param eKeyIdentifier The key identifier of the pressed key.
        @param uiModifiers Whether a modifier key is pressed (Ctrl, ALT, etc.).
        @return Has the event been handled.
      */
    virtual eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyIdentifier,
        const eC_UInt& uiModifiers);

    /** If an ET_KEYUP with one of the added key codes is received, the
        mapped object's DoKeyUp method is called.
        @param eKeyIdentifier The key identifier of the released key.
        @param uiModifiers Whether a modifier key is pressed (Ctrl, ALT, etc.).
        @return Has the event been handled.
     */
    virtual eC_Bool DoKeyUp(
        const GUIKeyIdentifier_t& eKeyIdentifier,
        const eC_UInt& uiModifiers);

private:
    /** Possible event slots to be called by HandleHotKeyContent(). */
    enum CallSlot
    {
        CS_DOCLICK,
        CS_DOKEYUP
    };

    /** Copy attributes method of the CGUIHotkeysBehaviour class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIHotkeysBehaviour& kSource);

    /// Helper for DoKeyDown.
    eC_Bool HandleHotKeyContent(GUIKeyIdentifier_t eKeyIdentifier,
        eC_UInt uiModifiers,
        CallSlot eSlot);

    /// Set true to set all hotkey events as handled.
    eC_Bool m_bMarkAlwaysAsHandled;
};
#endif //#ifndef GUI_HOTKEYS_BEHAVIOUR__H_
