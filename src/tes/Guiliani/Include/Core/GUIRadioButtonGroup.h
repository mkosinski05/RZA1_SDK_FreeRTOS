/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRADIOBUTTONGROUP__H_
#define GUIRADIOBUTTONGROUP__H_

#include "GUICompositeObject.h"
#include "GUIBaseRadioButton.h" // needed for static_cast


/// Container for radio buttons.

/**
    This is a composite (container) object for radio buttons.
    If you wish to use radio-buttons in your user-interface you _must_ place them inside a CGUIRadioButtonGroup
    in order for them to work correctly.

    Note that <i>only</i> radio buttons can be added to a CGUIRadioButtonGroup.

@code
    // Create a radiobuttongroup
    CGUIRadioButtonGroup *pkRadioButtonGroup = new CGUIRadioButtonGroup(this, eC_FromInt(0), eC_FromInt(165), eC_FromInt(240), eC_FromInt(50), OBJ_RADIOBUTTONGROUP);

    // Add some radiobuttons to the group
    CGUIBaseRadioButton *pkBaseRadioButton1 = new CGUIBaseRadioButton(pkRadioButtonGroup, eC_FromInt(0), eC_FromInt(0), eC_FromInt(60), eC_FromInt(20), true, OBJ_BASERADIOBUTTON1);
    CGUIBaseRadioButton *pkBaseRadioButton2 = new CGUIBaseRadioButton(pkRadioButtonGroup, eC_FromInt(70), eC_FromInt(0), eC_FromInt(60), eC_FromInt(20), false, OBJ_BASERADIOBUTTON2);

    // This will FAIL!
    // CGUIBaseButton *pBaseButton = new CGUIBaseButton(this, eC_FromInt(0), eC_FromInt(0), eC_FromInt(50), eC_FromInt(50), "Not a radiobutton", NULL );
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIRadioButtonGroup :
    public CGUICompositeObject
{
public:
    /** Constructor
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the object.
        @param vHeight Height of the object.
        @param eID Object Identifier of this object.
    */
    CGUIRadioButtonGroup(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor
        @param pParent Pointer to the designated parent object.
        @param kRect A CGUIRect describing position and size
        @param eID Object Identifier of this object.
    */
    CGUIRadioButtonGroup(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Tries to find an object with a specific ID.
        @see CGUICompositeObject::GetAndCheckObjectByID()
        @param eObjID The ID of the object to be searched.
        @param kObjectName The name of the object. Will be shown in the message
               that is generated when throwing an exception.
        @return Returns pointer to the object if it was found, otherwise NULL
    */
    CGUIBaseRadioButton* GetAndCheckObjectByID(ObjectHandle_t eObjID, const eC_String& kObjectName)
    {
        return static_cast <CGUIBaseRadioButton*>(
            CGUICompositeObject::GetAndCheckObjectByID(eObjID, kObjectName));
    }

    /** Sets the selected radio button of this radio button group.
        @param rkValue The CGUIValue containing the index of the radio button to select
        @return True if value can be set otherwise false
    */
    eC_Bool SetValue(const CGUIValue& rkValue);

    /** Ensures that only one radio button is selected at once.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return always False.
    */
    eC_Bool DoClick(const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Adds a radio button to this group.

        @note the original method CGUICompositeObject::AddObject() is hidden in
        this class because it is not allowed to add children that are not
        instances of CGUIBaseRadioButton.
        @param pRadioButton The radio button to be added.
        @return True if the button was added successfully, otherwise False.
      */
    virtual eC_Bool AddObject(CGUIBaseRadioButton *pRadioButton);

    /** Standard constructor
        @see CGUIObject().
    */
    CGUIRadioButtonGroup();

    /** Set given radio button as selected one. Unselect all other ones being
        part of this radio group.
        @param rkRadioButton Radio button being part of this group to set
         selected.
    */
    void SetSelectedRadioButton(CGUIBaseRadioButton& rkRadioButton);

    /** Returns a pointer to the currently selected radio button.
        @return pointer to the currently selected radio button or NULL if no
        radio button is selected.
      */
    CGUIBaseRadioButton* GetSelectedRadioButton() const;

    /** Add an observer to get informed when the selected radio button in the group is changed.
        void NotifyObservers (const CGUIObject* const pkUpdatedObject)
        will be called whenever the selection changes.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddSelectionObserver(CGUIObserver* pObserver)
    {
        m_kSelectionSubject.AddObserver(pObserver);
    }

    /** Unsubscribe an observer that subscribed as selection observer.
            @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveSelectionObserver(CGUIObserver* pObserver)
    {
        m_kSelectionSubject.RemoveObserver(pObserver);
    }

#ifdef GUILIANI_STREAM_GUI
    /** Reads the radio button group's attributes from the currently active
        input stream.

        This method does not catch CGUIIllegalChildException which is thrown
        if the base class CGUICompositeObject tries to add a child that is not
        a CGUIBaseRadioButton. This can only happen if GUILIANI_USE_RTTI is
        defined.
    */
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Copy-constructor of the CGUIRadioButtonGroup class.
        Copies the object without its children!
        @param kSource Source object to be copied.
    */
    CGUIRadioButtonGroup(const CGUIRadioButtonGroup& kSource);

    /** Operator= method of the CGUIRadioButtonGroup class.
        Copies the object without its children!
        @param kSource Source object to be copied.
        @return Copied object without its children.
    */
    CGUIRadioButtonGroup& operator=(const CGUIRadioButtonGroup& kSource);

private:
    /** This function shall only be called by code of any of this class's
        base classes. If it is called by any other class, then this is
        considered to be a misusage.
        @param pObject The child object to be added.
        @return True if the object was added successfully, otherwise False.
        @throw CGUIIllegalChildException if the object is not a
               CGUIBaseRadioButton. The check is only performed if
               GUILIANI_USE_RTTI is defined.
      */
    eC_Bool AddObject(CGUIObject *pObject);

    ///Subject informing observers whenever the selected radio button changes.
    CGUISubject m_kSelectionSubject;
};

#endif
