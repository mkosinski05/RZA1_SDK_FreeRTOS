/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASECHECKBOX__H_
#define GUIBASECHECKBOX__H_

#include "GUIBaseButton.h"

/// Base class for checkboxes.

/** This is the base class for checkboxes and radiobuttons. In addition to the text label, which it inherits from its base class CGUIBaseButton,
    it adds an additional "selected"-state.

    This implementation draws itself using plain lines and rectangles. It serves as a base-class from which you
    can derive customized checkbox classes. For an implementation that uses bitmaps to visualize itself, please
    refer to CGUICheckBox.

    Example:
@code
    // Create base check box.
    CGUIBaseCheckBox *pkBaseCheckBox = new CGUIBaseCheckBox(this,
        eC_FromInt(100), eC_FromInt(10), eC_FromInt(20),
        eC_FromInt(20), false, OBJ_BASECHECKBOX);
@endcode

    An elegant way to react when the user selects / deselects the checkbox is by registering an observer.
    The following example shows how this can be done:

    Example:
@code
    // Implement your own observer
    class CMyCheckBoxObserver : public CGUIObserver
    {
    public:
        // Called when checkbox status has changed
        void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
        {
            if( kObservedValue.ToBool() )
            {
                // Checkbox is selected
            }
            else
            {
                // Checkbox is not selected
            }
        }
    };

    // Somewhere after the creation of the checkbox, register your observer within the checkbox
    CGUIObserver* pObserver = new CMyCheckBoxObserver()
    pkBaseCheckBox->AddSelectionObserver( pObserver );
    // Note: Do not forget to delete your observer later!

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseCheckBox
    : public CGUIBaseButton
{
public:

    /// Enumeration for checkbox layout styles
    enum CheckBoxLayout_t
    {
        MANUAL_LAYOUT,              ///< No automatic layout. Values set by user will remain untouched
        ICON_LEFT_AUTOSCALE,        ///< Icon left and scaled automatically - Text resides right 
        ICON_RIGHT_AUTOSCALE,       ///< Icon right and scaled automatically - Text resides left 
        ICON_LEFT_CENTERED,         ///< Icon left and not scaled - Text resides right 
        ICON_RIGHT_CENTERED,        ///< Icon right and not scaled- Text resides left 
        ICON_FILL_OBJECT            ///< The Icon will fill the entire area of the object
    };


    /** CGUIBaseCheckBox standard constructor.
        @see CGUIObject().
    */
    CGUIBaseCheckBox();

    /** CGUIBaseCheckBox constructor.
    @param pParent Pointer to parent object.
    @param vX X position relative to its parent object.
    @param vY Y position relative to its parent object.
    @param vWidth Width of the checkbox.
    @param vHeight Height of the checkbox.
    @param bSelected Status of the image checkbox to indicate whether the checkbox is checked or not. Checked, if true, not checked, otherwise
    @param eID Object Identifier of this checkbox
    */
    CGUIBaseCheckBox(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIBaseCheckBox constructor.
    @param pParent Pointer to parent object.
    @param kRect Structure containing x- and y-coordinates as well as width and height.
    @param bSelected Status of the checkbox to indicate whether the checkbox is checked or not. Checked, if true, not checked, otherwise
    @param eID Object Identifier of this checkbox
    */
    CGUIBaseCheckBox(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIBaseCheckBox class.
        @param kSource Source object to be copied.
    */
    CGUIBaseCheckBox(const CGUIBaseCheckBox& kSource);

    /** Operator= method of the CGUIBaseCheckBox class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIBaseCheckBox& operator=(const CGUIBaseCheckBox& kSource);

    /// Destructor
    virtual ~CGUIBaseCheckBox();

    /** Sets the selected state of this CheckBox
        @see SetSelected()
        @param rkValue The CGUIValue containing the new value(s)
        @return Always True.
    */
    virtual eC_Bool SetValue(const CGUIValue& rkValue)
    {
        SetSelected(rkValue.ToBool());
        CGUIObject::SetValue(rkValue);
        return true;
    }

    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);
    eC_Bool DoDraw();

    /** Set the object's status to selected (true) or NOT selected (false).
        Mote: Whem called on a RadioButton, this will NOT ensure unique selection of just ONE button in the RadioButtonGroup.
        @param bSelect Boolean value indicating the new state.    */
    virtual void SetSelected(eC_Bool bSelect)
    {
        if (m_bIsSelected != bSelect)
        {
            m_bIsSelected = bSelect;
            InvalidateArea();
            m_kSelectionSubject.NotifyObservers(CGUIValue(m_bIsSelected), this);
            NotifyValueObservers(m_bIsSelected);
        }
    }

    /** Checks if the object is currently selected
        @return True if selected, False otherwise
    */
    virtual eC_Bool IsSelected() const
    {
        return m_bIsSelected;
    }

    /** Add an observer to get informed when the selection is changed.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject)
        will be called whenever the selection changes.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddSelectionObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as selection observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveSelectionObserver(CGUIObserver* pObserver);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Calculates the size and position of the Icon area with regard to the chosen checkbox layout.
        The resulting rectangle will be stored in m_kIconRelRect
        @return The rectangle in relative coordinates which should be covered by the Icon
    */
    virtual CGUIRect CalculateIconRect();

    /** Calculates the size and position of the label area with regard to the chosen checkbox layout
        and the associated icon's position and dimension.
        @param kIconRect Rectangle in relative coordinates which is covered by the icon
        @return The rectangle in absolute which should be covered by the text label
    */
    virtual CGUIRect CalculateLabelRect(const CGUIRect &kIconRect);

    /** Sets the layout for this checkbox. Please refer to CheckBoxLayout_t for a description of the supported layouts.
        @param eLayout The desired layout.
    */
    void SetCheckBoxLayout(const CheckBoxLayout_t eLayout);

    void SetIconRect(const CGUIRect& kRect);

protected:
    /** Helper function for constructors.
        @param bSelected Initial value of m_bIsSelected
    */
    void Init(const eC_Bool &bSelected);

    /// Position of text label relative to checkbox element
    CheckBoxLayout_t  m_eCheckBoxLayout;

    /// Icon size/position in relative coordinates
    CGUIRect    m_kIconRelRect;

private:
    /** Copy attributes method of the CGUIBaseCheckBox class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIBaseCheckBox& kSource);

    /// Status information.  Is the object selected?
    eC_Bool m_bIsSelected;

    ///Subject informing observers whenever the selection state changes.
    CGUISubject m_kSelectionSubject;
};

#endif
