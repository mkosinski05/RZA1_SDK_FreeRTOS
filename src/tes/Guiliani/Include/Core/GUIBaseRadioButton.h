/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASERADIOBUTTON__H_
#define GUIBASERADIOBUTTON__H_

#include "GUIBaseCheckBox.h"

class CGUIRadioButtonGroup;

/// Base class for radio buttons.

/** Like all the "BaseXXXXX"-Classes, this serves primarily as base-class, from
    which you can derive your own customized objects.
    For a radio button implementation which uses images to visualize itself, please refer
    to CGUIRadioButton.
    
    A radio button, like a check box, can have two states, 'selected' and
    'not selected'. Radio buttons in addition must be children of a
    CGUIRadioButtonGroup. Only one of the radio buttons in a group can be
    selected at a time.

    Example:
@code

    // Create base radio button.
    CGUIBaseRadioButton *pkBaseRadioButton = new CGUIBaseRadioButton(
                                                     pkRadioButtonGroup, // parent must be a CGUIRadioButtonGroup
                                                     eC_FromInt(0), eC_FromInt(0),
                                                     eC_FromInt(60), eC_FromInt(20),
                                                     false, OBJ_BASERADIOBUTTON);
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseRadioButton
    : public CGUIBaseCheckBox
{
    friend class CGUIRadioButtonGroup;
public:
    /** CGUIBaseRadioButton constructor.
    @param pParent Pointer to parent object which must be a button group.
    @param vX X position relative to its parent object.
    @param vY Y position relative to its parent object.
    @param vWidth Width of the checkbox.
    @param vHeight Height of the checkbox.
    @param bSelected Status of the radio button. Selected, if true, not selected, otherwise
    @param eID Object Identifier of this radio button
    */
    CGUIBaseRadioButton(
        CGUIRadioButtonGroup *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

   /** CGUIBaseRadioButton constructor.
    @param pParent Pointer to parent object which must be a button group.
    @param kRect Structure containing x- and y-coordinates as well as width and height.
    @param bSelected Status of the radio button. Selected if true, not selected otherwise.
    @param eID Object Identifier of this radio button
    */
    CGUIBaseRadioButton(
        CGUIRadioButtonGroup *const pParent,
        const CGUIRect &kRect,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIBaseRadioButton class.
        @param kSource Source object to be copied.
    */
    CGUIBaseRadioButton(const CGUIBaseRadioButton& kSource);

    /** Operator= method of the CGUIBaseRadioButton class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIBaseRadioButton& operator=(const CGUIBaseRadioButton& kSource);

    /**    Standard constructor.
        @see CGUIObject().
    */
    CGUIBaseRadioButton();

    virtual ~CGUIBaseRadioButton();

    eC_Bool DoDraw();

    /** Set radio button selection status true.
        If another radio button in the same group is selected, it will become unselected before.
     */
    void Select();

    /** Must be implemented to avoid calling of DoClick() in CGUIBaseCheckBox
        base class. This is because the CGUIRadioButtonGroup should handle toggling 
        the selection state of its children.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation it always returns False.
    */
    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif


private:
    /** Copy attributes method of the CGUIBaseRadioButton class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIBaseRadioButton& kSource);

    /** Helper function for constructors. */
    void Init();

    /** Associated radio button group. */
    CGUIRadioButtonGroup* m_pkRbGroup;
};
#endif
