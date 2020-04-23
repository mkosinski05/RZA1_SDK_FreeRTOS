/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICHECKBOX__H_
#define GUICHECKBOX__H_

#include "GUIBaseCheckBox.h"
#include "GUIImageResource.h"


/// Check box that draws itself with images.

// @guiliani_doxygen toplevel_control Check Box
/**
    <table border="0">
        <tr>
            <td width="200">@image html check_box.png</td>
            <td>
                The "check box" control can be checked or unchecked to set a condition. It uses images for visualizing its different states. The "check box" control can be used to define bivalent settings or options (Class: CGUICheckBox).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    Supports five states (normal, highlighted, focused, pressed and grayed-out)
    and two images for each state (for selected and unselected checkboxes).

    Example:
@code

    // Create a check box.
    CGUICheckBox* pkCheckBox = new CGUICheckBox(this,
        eC_FromInt(0), eC_FromInt(0), eC_FromInt(20),
        eC_FromInt(20), false, OBJ_CHECKBOX);

    // Set the images of the check box.
    // Before calling this method, please add image IDs to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
    pkCheckBox->SetImages(
                    IMG_SEL_FOCUSED,
                    IMG_SEL_HIGHLIGHTED,
                    IMG_SEL_PRESSED,
                    IMG_SEL_STANDARD,
                    IMG_SEL_GRAYEDOUT,
                    IMG_NOTSEL_FOCUSED,
                    IMG_NOTSEL_HIGHLIGHTED,
                    IMG_NOTSEL_PRESSED,
                    IMG_NOTSEL_STANDARD,
                    IMG_NOTSEL_GRAYEDOUT );

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUICheckBox :
    public CGUIBaseCheckBox
{
public:

    /**    Standard constructor
        @see CGUIObject().
    */
    CGUICheckBox();

    // Destructor
    virtual ~CGUICheckBox();

    /** CGUICheckBox constructor.
    To set the standard Guiliani images, please call the SetStandardImages() method.
    To Set the user specific images, please use the SetImages() method.
    @param pParent Pointer to parent object.
    @param vX X position relative to its parent object.
    @param vY Y position relative to its parent object.
    @param vWidth Width of the image checkbox.
    @param vHeight Height of the image checkbox.
    @param bChecked Status of the image checkbox to indicate whether the image checkbox is checked or not. Checked, if true, not checked, otherwise
    @param eID Object Identifier of this image checkbox
    */
    CGUICheckBox(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Bool &bChecked = false,
        const ObjectHandle_t &eID = NO_HANDLE);


   /** CGUICheckBox constructor.
    @param pParent Pointer to parent object.
    @param kRect Structure containing x- and y-coordinates as well as width and height.
    @param bChecked Status of the checkbox to indicate whether the image checkbox is checked or not. Checked, if true, not checked, otherwise
    @param eID Object Identifier of this image checkbox
    */
    CGUICheckBox(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Bool &bChecked = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUICheckBox class.
        @param kSource Source object to be copied.
    */
    CGUICheckBox(const CGUICheckBox& kSource);

    /** Operator= method of the CGUICheckBox class.
        @param kSource Source object to be copied.
        @return Destination of copy
    */
    CGUICheckBox& operator=(const CGUICheckBox& kSource);

    eC_Bool DoDraw();

    /** Set the images for the different states
        @param eSelectedFocussed Image when selected and focused
        @param eSelectedHighlighted Image when selected and highlighted
        @param eSelectedPressed Image when selected and pressed
        @param eSelectedNormal Image when selected and normal
        @param eSelectedGrayedOut Image when selected and grayed out
        @param eNotSelectedFocussed Image when deselected and focused
        @param eNotSelectedHighlighted Image when deselected and highlighted
        @param eNotSelectedPressed Image when deselected and pressed
        @param eNotSelectedNormal Image when deselected and normal
        @param eNotSelectedGrayedOut Image when deselected and grayed out
    */
    void SetImages(
        const ImageResource_t &eSelectedFocussed,
        const ImageResource_t &eSelectedHighlighted,
        const ImageResource_t &eSelectedPressed,
        const ImageResource_t &eSelectedNormal,
        const ImageResource_t &eSelectedGrayedOut,
        const ImageResource_t &eNotSelectedFocussed,
        const ImageResource_t &eNotSelectedHighlighted,
        const ImageResource_t &eNotSelectedPressed,
        const ImageResource_t &eNotSelectedNormal,
        const ImageResource_t &eNotSelectedGrayedOut);

    /// Set the standard Guiliani images to this image checkbox.
    void SetStandardImages();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:

    //Images for the situation that the box is selected.
    ImageResource_t m_eSelectedNormal; ///< the selected normal image
    ImageResource_t m_eSelectedHighlighted;  ///< the selected highlighted image
    ImageResource_t m_eSelectedPressed; ///< the selected pressed image
    ImageResource_t m_eSelectedGrayedOut; ///< the selected grayed out image
    ImageResource_t m_eSelectedFocussed;  ///< the selected focused image

    //Images for the situation that the box is not selected.
    ImageResource_t m_eNotSelectedNormal;///< the selected normal image
    ImageResource_t m_eNotSelectedHighlighted;///< the selected highlighted image
    ImageResource_t m_eNotSelectedPressed;///< the selected pressed image
    ImageResource_t m_eNotSelectedGrayedOut;///< the selected grayed out image
    ImageResource_t m_eNotSelectedFocussed;///< the selected focused image

    /** Helper function for constructors. */
    void Init();

private:

    /** Copy attributes method of the CGUICheckBox class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUICheckBox& kSource);

    // This private method assigns dummy images to all attributes intended for images.
    void SetDummyImages(void);

    /// Helper for destructor and ReadFromStream.
    void DeInit();
};

#endif
