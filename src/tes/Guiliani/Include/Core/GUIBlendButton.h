/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBLENDBUTTON__H_
#define GUIBLENDBUTTON__H_

#include "GUIButton.h"
#include "GUIImageResource.h"

/// Button specialization that softly blends between the button's different states

// @guiliani_doxygen toplevel_control Blend Button
/**
    <table border="0">
        <tr>
            <td width="200">@image html blendbutton.png</td>
            <td>
                The "Blend Button" is a specialized version of the standard CGUIButton, which softly blends between the different button bitmaps if the button's state changes.
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The "blend button" is a specialization of the standard CGUIButton class, which softly blends between the various state images instead of simply switching them.
    Be aware that alpha-blending can be time-consuming on low end platforms, in particular if blending is done using a software-renderer (Class: CGUIBlendButton).
    Please refer to the base class (CGUIButton) for a general explanation of the button object. The "blend button" supports nine patch images (CGUINinePatch).

    Example:
@code

    // Create a Blend button.
    CGUIBlendButton* pkButton = new CGUIBlendButton(this,
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
    
    // Set Duration of blending transition to 500ms
    pkButton->SetBlendDuration(500);

@endcode

    @ingroup GUILIANI_CONTROLS
*/

class CGUIBlendButton : public CGUIButton
{
public:
    /** CGUIBlendButton constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the button
        @param vHeight Height of the button
        @param rkText Text string that will be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button
    */
    CGUIBlendButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIBlendButton constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the button
        @param vHeight Height of the button
        @param eTextID ID of text string to be displayed on the Button
        @param pCommandObject Pointer to command object which will be executed in reaction to a click
        @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIBlendButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** copy-constructor.
        @param kSource Source object to be copied
    */
    CGUIBlendButton(const CGUIBlendButton& kSource);

    /** CGUIBlendButton operator= to copy the whole CGUIBlendButton.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIBlendButton& operator=(const CGUIBlendButton& kSource);

    virtual eC_Bool DoDraw();

    /** Sets the duration of blending from one state image to the next
        @param uiBlendDuration Duration of blending in milliseconds */
    void SetBlendDuration( const eC_UInt uiBlendDuration) 
    {
        m_uiBlendDuration = uiBlendDuration;
    }

    /** Sets the blending mode.
        If cross-fading is enabled the "old" image will be fading out while the "new" image is fading in.
        With cross-fading disabled, the "old" image is drawn fully opaque until the "new" image has been blended in entirely.
        @param bCrossFade True if cross-fading shall be enabled, false otherwise. */
    void SetCrossFade( const eC_Bool bCrossFade)
    {
        m_bCrossFade = bCrossFade;
    }

#ifdef GUILIANI_STREAM_GUI
    CGUIBlendButton();
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:

    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /// Helper for initialization
    void Init();

    ImageResource_t     m_ePreviousImage;   ///< ImageResource FROM which to blend
    ImageResource_t     m_eLastFrameImage;  ///< Image which was shown during last frame (used to detect state changes)
    eC_Value            m_vBlendFactor;     ///< Progress of blending transition (0 to m_uiBlendDuration)
    eC_UInt             m_uiBlendDuration;  ///< Duration of blend transition in milliseconds
    eC_Bool             m_bCrossFade;       ///< When crossfading the old and new image will be simultaneously blended. Otherwise the new old image is drown fully opaque.
};

#endif
