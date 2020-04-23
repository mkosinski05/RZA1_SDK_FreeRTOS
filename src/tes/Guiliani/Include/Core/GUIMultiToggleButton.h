/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIMULTITOGGLEBUTTON__H_
#define GUIMULTITOGGLEBUTTON__H_

#include "GUICompositeObject.h"


/// Base class for multi toggle buttons.
// @guiliani_doxygen toplevel_control Multi Toggle Button
/**
    <table border="0">
        <tr>
            <td width="200">@image html multi_toggle_button.png</td>
            <td>
                The "multi toggle button" is a container which cycles through all its contained controls, by rendering them (in)visible and ensuring that only one of them is visible at a time. On each click on it, it will set the currently active control to invisible, while at the same time setting the next control to visible. The "multi toggle button" can be used if e.g. a selectable option has multiple states, like a traffic light (Class: CGUIMultiToggleButton). 
                The "multi toggle button" supports nine patch images (CGUINinePatch).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    A MultiToggleButton is a container object which cycles through all its contained child objects, by rendering
    them (in)visible and ensuring that only one of them is visible at a time. On each received click event, it will set
    the currently active child object to invisible, while at the same time setting the next child to visible.
    While it is generally possible to place any type of object inside a MultiToggleButton, you should make sure that the
    children you place therein are clickable, since otherwise you will not be able to move on to the next contained child.

    A use case for this control would for example be a touchscreen driven application in which the user shall choose between
    five different skins. You could place five CGUIImage objects representing the different visual styles inside of a
    multitoggle button. Do not forget to enable clicking on them by setting their Disabled-state to False. Now whenever the
    user touches the MultiToggleButton it will cycle through these five Image-object one by one, allowing you to switch
    skins on every click.

    Example:
@code
    // Create multi toggle button.
    CGUIMultiToggleButton* pkMultiToggleButton = new CGUIMultiToggleButton( this,
                                                        eC_FromInt(0), eC_FromInt(0),
                                                        eC_FromInt(200), eC_FromInt(200),
                                                        NO_HANDLE);

    // Add button
    CGUIButton* pkButton1 = new CGUIButton(pkMultiToggleButton,
                                    eC_FromInt(0), eC_FromInt(0),
                                    eC_FromInt(100), eC_FromInt(20),
                                    "button1",
                                    NULL,
                                    NO_HANDLE);

    // Add another button
    CGUIButton* pkButton2 = new CGUIButton(pkMultiToggleButton,
                                    eC_FromInt(50), eC_FromInt(0),
                                    eC_FromInt(100), eC_FromInt(20),
                                    "button2",
                                    NULL,
                                    NO_HANDLE);

    // Add an image
    CGUIImage* pImage = new CGUIImage(pkMultiToggleButton,
                                    eC_FromInt(0), eC_FromInt(0),
                                    eC_FromInt(100), eC_FromInt(100),
                                    IMG_STDCTRL_PLACEHOLDER);
    pImage->SetDisabled( false);
@endcode

    @ingroup GUILIANI_CONTROLS
*/

class CGUIMultiToggleButton
    : public CGUICompositeObject
{
public:
    /** Constructor
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the object.
        @param vHeight Height of the object.
        @param eID Object Identifier of this object
    */
    CGUIMultiToggleButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor
        @param pParent Pointer to the designated parent object.
        @param kRect CGUIRect describing size and position.
        @param eID Object Identifier of this object
    */
    CGUIMultiToggleButton(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID = NO_HANDLE);

    CGUIMultiToggleButton();
    ~CGUIMultiToggleButton();

    /** Adds an object into the MultiToggleButton.
        Please refer to the overview description of this class for an explanation on how it works
        @param pkObject Object to add
        @return True if successful, False otherwise
    */
    eC_Bool AddObject(CGUIObject *pkObject);

    /// Clicking on an object within the MultiToggleButton will toggle to the next contained object
    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Returns the index of the currently active child object.
        (The child which was added first will have index 0)
        @return Index of the currently active child object */
    eC_UInt GetActiveIndex() const;

    /** Returns a pointer to the currently active child object.
        @return Pointer to the currently active child object */
    CGUIObject *GetActiveObject() const;

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif
};
#endif
