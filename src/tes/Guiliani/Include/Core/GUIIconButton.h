/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIICONBUTTON__H_
#define CGUIICONBUTTON__H_

#include "GUIImageResource.h"
#include "GUIButton.h"


/// Button specialization that shows a state-dependent icon.
// @guiliani_doxygen toplevel_control Icon Button
/**
<table border="0">
<tr>
<td width="200">@image html icon_button.png</td>
<td>
The "icon button" control is an enhanced button. It uses additional to its text an image as icon. This image can be changed separately and can be located at an arbitrary position and dimension on the button (Class: CGUIIconButton).
The "icon button" supports nine patch images (CGUINinePatch).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**

This control shows an additional icon on the button. The user is responsible
for the icon image size and its position.

Example:
@code
// Create an icon button.
CGUIIconButton* pkIconButton = new CGUIIconButton(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(30), eC_FromInt(30),
eC_FromInt(0), eC_FromInt(0),
"",
NULL,   // Replace the NULL with your command.
OBJ_ICONBUTTON);

// Set images of icon.
// Before calling this method, please add image IDs to UserImageResource.h.
// After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
pkIconButton->SetIconImages(
ICON_STANDARD,
ICON_PRESSED,
ICON_HIGHLIGHTED,
ICON_GRAYEDOUT,
ICON_FOCUSED );

// Set position of icon inside the control.
pkIconButton->SetIconPos(eC_FromInt(10), eC_FromInt(10));

@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUIIconButton : public CGUIButton
{
public:
    /**CGUIIconButton constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object.
    @param vY Y-position relative to its parent object.
    @param vWidth Width of the button.
    @param vHeight Height of the button.
    @param vXIconPos X-position of image relative this object.
    @param vYIconPos Y-position of image relative this object.
    @param pkText Text string that will be displayed on the Button.
    @param pCommandObject Pointer to command object, which will be executed in reaction to a click.
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required).
    */
    CGUIIconButton(CGUICompositeObject *const pParent,
                   const eC_Value &vX, const eC_Value &vY,
                   const eC_Value &vWidth,
                   const eC_Value &vHeight,
                   const eC_Value &vXIconPos,
                   const eC_Value &vYIconPos,
                   const eC_String& pkText,
                   CGUICommandPtr pCommandObject,
                   const ObjectHandle_t &eID = NO_HANDLE);


    /**CGUIIconButton constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect dimensions of object with respect to parent object.
    @param vXIconPos X-position of image relative this object.
    @param vYIconPos Y-position of image relative this object.
    @param rkText Text string that will be displayed on the Button.
    @param pCommandObject Pointer to command object, which will be executed in reaction to a click.
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required).
    */
    CGUIIconButton(CGUICompositeObject *const pParent,
                   const CGUIRect &kRect,
                   const eC_Value &vXIconPos,
                   const eC_Value &vYIconPos,
                   const eC_String& rkText,
                   CGUICommandPtr pCommandObject,
                   const ObjectHandle_t &eID = NO_HANDLE);

    /**CGUIIconButton constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object.
    @param vY Y-position relative to its parent object.
    @param vWidth Width of the button.
    @param vHeight Height of the button.
    @param vXIconPos X-position of image relative this object.
    @param vYIconPos Y-position of image relative this object.
    @param eTextID ID of text string to be displayed on the Button.
    @param pCommandObject Pointer to command object, which will be executed in reaction to a click.
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required).
    */
    CGUIIconButton(CGUICompositeObject *const pParent,
                   const eC_Value &vX, const eC_Value &vY,
                   const eC_Value &vWidth,
                   const eC_Value &vHeight,
                   const eC_Value &vXIconPos,
                   const eC_Value &vYIconPos,
                   const TextResource_t &eTextID,
                   CGUICommandPtr pCommandObject,
                   const ObjectHandle_t &eID = NO_HANDLE);

    /**CGUIIconButton constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect dimensions of object with respect to parent object.
    @param vXIconPos X-position of image relative this object.
    @param vYIconPos Y-position of image relative this object.
    @param eTextID ID of text string to be displayed on the Button.
    @param pCommandObject Pointer to command object, which will be executed in reaction to a click.
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required).
    */
    CGUIIconButton(CGUICompositeObject *const pParent,
                   const CGUIRect &kRect,
                   const eC_Value &vXIconPos,
                   const eC_Value &vYIconPos,
                   const TextResource_t &eTextID,
                   CGUICommandPtr pCommandObject,
                   const ObjectHandle_t &eID = NO_HANDLE);

    /**Copy-constructor of the CGUIProgressBar class.
    @param kSource Source object to be copied.
    */
    CGUIIconButton(const CGUIIconButton& kSource);


    /**Operator= method of the CGUIProgressBar class.
    @param kSource Source object to be copied.
    @return Copied object.
    */
    CGUIIconButton& operator=(const CGUIIconButton& kSource);

    ///    Standard draw function.
    eC_Bool DoDraw();

    /**Function to set icons of the controls.
    @param eIconFocussed Image of the icon when button is in Focussed state.
    @param eIconHighlighted Image of the icon when button is in Highlighted state.
    @param eIconPressed Image of the icon when button is in Pressed state.
    @param eIconNormal Image of the icon when button is in Normal state.
    @param eIconGrayedOut Image of the icon when button is in Gryed-Out state.
    */
    void SetIconImages(
        const ImageResource_t eIconNormal,
        const ImageResource_t eIconPressed,
        const ImageResource_t eIconHighlighted,
        const ImageResource_t eIconGrayedOut,
        const ImageResource_t eIconFocussed);

    /**Function to get icons of the controls.
    @param eIconFocussed Image of the icon when button is in Focussed state.
    @param eIconHighlighted Image of the icon when button is in Highlighted state.
    @param eIconPressed Image of the icon when button is in Pressed state.
    @param eIconNormal Image of the icon when button is in Normal state.
    @param eIconGrayedOut Image of the icon when button is in Gryed-Out state.
    */
    void GetIconImages(
        ImageResource_t &eIconNormal,
        ImageResource_t &eIconPressed,
        ImageResource_t &eIconHighlighted,
        ImageResource_t &eIconGrayedOut,
        ImageResource_t &eIconFocussed) const
    {
        eIconFocussed = m_eIconFocussed;
        eIconHighlighted = m_eIconHighlighted;
        eIconPressed = m_eIconPressed;
        eIconNormal = m_eIconNormal;
        eIconGrayedOut = m_eIconGrayedOut;
    }

    /**Function to set position of icon inside the control.
    @param vIconPosX X Position of icon relative to control.
    @param vIconPosY Y Position of icon relative to control.
    */
    void SetIconPos(const eC_Value &vIconPosX, const eC_Value &vIconPosY);

    /** Returns the position of the icon.
    @param[out] vIconPosX X Position of icon relative to control.
    @param[out] vIconPosY Y Position of icon relative to control.
    */
    void GetIconPos(eC_Value &vIconPosX, eC_Value &vIconPosY) const;

    /**
    Sets the transparency, which is used for the icon blit in all button states.
    @param[in] ubIconAlpha The alpha value to use.
    */
    void SetIconAlpha(const eC_UByte ubIconAlpha)
    {
        m_ubIconAlpha = ubIconAlpha;
    }

    /**
    Gets the transparency, which is used for the icon blit in all button states.
    @return The alpha value which is used for the icon blit.
    */
    eC_UByte GetIconAlpha()
    {
        return m_ubIconAlpha;
    }

    /// Destructor.
    ~CGUIIconButton();

#ifdef GUILIANI_STREAM_GUI
    CGUIIconButton();
#endif
protected:
#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    ///m_eIconNormal is used to draw the icon when icon is in Normal state.
    ///@see SetIconImages function
    ImageResource_t m_eIconNormal;

    ///m_eIconHighlighted is used to draw the icon when icon is in Highlighted state.
    ///@see SetIconImages function
    ImageResource_t m_eIconHighlighted;

    ///m_eIconPressed is used to draw the icon when icon is in Pressed state.
    ///@see SetIconImages function
    ImageResource_t m_eIconPressed;

    ///m_eIconGrayedOut is used to draw the icon when icon is in Grayed-Out state.
    ///@see SetIconImages function
    ImageResource_t m_eIconGrayedOut;

    ///m_eIconFocussed is used to draw the icon when icon is in Focussed state.
    ///@see SetIconImages function
    ImageResource_t m_eIconFocussed;

    ///m_vIconPosX stores X-Position of icon. This is relative to the control.
    eC_Value m_vIconPosX;

    ///m_vIconPosY stores Y-Position of icon. This is relative to the control.
    eC_Value m_vIconPosY;

    ///m_ubIconAlpha stores the alpha value which is used for the icon blit in all button states.
    eC_UByte m_ubIconAlpha;

private:
    /// This function sets most parameters and this is called by constructors.
    void Init(ImageResource_t eIconIDNormal = DUMMY_IMAGE,
              ImageResource_t eIconIDPressed = DUMMY_IMAGE,
              ImageResource_t eIconIDHighlighted = DUMMY_IMAGE,
              ImageResource_t eIconIDGrayedOut = DUMMY_IMAGE,
              ImageResource_t eIconIDFocussed = DUMMY_IMAGE);

    /**Copy attributes method of the CGUIProgressBar class.
    @param kSource Source object to be copied.
    @return Copied object.
    */
    void CopyAttributes(const CGUIIconButton& kSource);

    /// Helper for destructor and ReadFromStream.
    void DeInit();
};
#endif
