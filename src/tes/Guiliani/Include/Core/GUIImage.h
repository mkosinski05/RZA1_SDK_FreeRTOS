/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIIMAGE__H_
#define GUIIMAGE__H_

#include "GUIObject.h"
#include "GUIImageResource.h"


/// Displays a static image in the GUI.
// @guiliani_doxygen toplevel_control Image
/**
    <table border="0">
        <tr>
            <td width="200">@image html image.png</td>
            <td>
                The "image" control is used to display a static image. The position and dimension of the image can be set freely. The scaling of the images is arbitrary in x and y dimension -  so the aspect ratio can be changed. The "image" control is typically used to display e.g., a background image or brand icons (Class: CGUIImage).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    This control displays an image. The image can be stretched if the control
    size does not match the image size.

    Example:
@code

    // Create an image.
    CGUIImage* pkImage = new CGUIImage(this,
                                eC_FromInt(0), eC_FromInt(0),
                                eC_FromInt(30), eC_FromInt(30),
                                IMG_IMAGE1,
                                true,
                                OBJ_IMAGE);

    // Set image.
    // Before calling this method, please add image id to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() to register an image for image id.
    pkImage->SetImage(IMG_IMAGE2,true);

    // Sets the alpha (transparency) value of the image
    pkImage->SetAlpha(12);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIImage :
    public CGUIObject
{
public:
    /** Constructs a CGUIImage object.
        Note that if the given width and height do not match the actual size
        of the image it will be stretched when drawing it, resulting in a
        potential performance loss.
        @param pParent Pointer to the parent object of this image.
        @param vX The x coordinate of the upper left corner of this image.
        @param vY The y coordinate of the upper left corner of this image.
        @param vWidth The width of this image.
        @param vHeight The height of this image.
        @param eImageID The ID of the image represented by this object.
        @param eID This image's object ID.
        @param bStretchBlit If true image will be stretched to the size of the object, if false image will be centered.
        */
    CGUIImage(
        CGUICompositeObject* const pParent,
        const eC_Value& vX,
        const eC_Value& vY,
        const eC_Value& vWidth,
        const eC_Value& vHeight,
        const ImageResource_t& eImageID,
        const eC_Bool& bStretchBlit = true,
        const ObjectHandle_t& eID = NO_HANDLE);

    /** Constructs a CGUIImage object.
        Note that if the given width and height do not match the actual size
        of the image it will be stretched when drawing it, resulting in a
        potential performance loss.
        @param pParent Pointer to the parent object of this image.
        @param kRect The bounding rectangle, relative to parent.
        @param eImageID The ID of the image represented by this object.
        @param eID This image's object ID.
        @param bStretchBlit If true image will be stretched to the size of the object, if false image will be centered.
        */
    CGUIImage(
        CGUICompositeObject* const pParent,
        const CGUIRect &kRect,
        const ImageResource_t& eImageID,
        const eC_Bool& bStretchBlit = true,
        const ObjectHandle_t& eID = NO_HANDLE);

    /** Constructs a CGUIImage object and determines its size by the image.
        @param pParent Pointer to the parent object of this image.
        @param vX The x coordinate of the upper left corner of this image.
        @param vY The y coordinate of the upper left corner of this image.
        @param eImageID The ID of the image represented by this object. The
               size of this object is initialized with the dimensions of the
               image.
        @param eID This image's object ID.
        */
    CGUIImage (
        CGUICompositeObject* const pParent,
        const eC_Value& vX,
        const eC_Value& vY,
        const ImageResource_t& eImageID,
        const ObjectHandle_t& eID = NO_HANDLE);

    /** Copy-constructor of the CGUIImage class.
        @param kSource Source object to be copied.
    */
    CGUIImage(const CGUIImage& kSource);

    /** Operator= method of the CGUIImage class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIImage& operator=(const CGUIImage& kSource);

    /** Standard constructor.
        @see CGUIObject().
    */
    CGUIImage();
    ~CGUIImage();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif
    bool DoDraw();
    /** Sets the ID of the image that will be drawn by this control.
        @param eImage An image ID. If it is the same ID as the current one,
               this method does nothing.
        @param bFitToImage Specifies whether the CGUIImage object shall be automatically
               resized to match the given image's dimensions.
    */
    void SetImage(const ImageResource_t &eImage, const eC_Bool &bFitToImage = false);

    /**
        Get the ImageID.
        @return ImageID
    */
    inline ImageResource_t GetImageID() const { return m_eImage; }


    /** Sets the stretch blit attribute of the image
        @param bStretchBlit Specifies whether the CGUIImage object shall be automatically
               resized to match the given image's dimensions. */
    inline void SetStretchBlit( eC_Bool bStretchBlit) { m_bStretchBlit = bStretchBlit; }

    /** Sets the stretch blit attribute of the image
        @return True if image will be stretched */
    inline eC_Bool GetStretchBlit() const {return m_bStretchBlit;}

private:
    /** Copy attributes method of the CGUIImage class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIImage& kSource);

    /** Helper function for constructors. */
    void Init(
        const ImageResource_t& eImageID,
        const eC_Bool& bStretchBlit);

    /// Helper function for destructor and ReadFromStream.
    void DeInit();

    ImageResource_t     m_eImage;
    eC_Bool             m_bStretchBlit;
};
#endif
