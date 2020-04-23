/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIANIMATEDIMAGE__H_
#define GUIANIMATEDIMAGE__H_

// necessary includes
#include "GUIImage.h"
#include "GUIImageResource.h"

/// Displays an animated image in the GUI.
// @guiliani_doxygen toplevel_control Animated Image
/**
    <table border="0">
    <tr>
            <td width="200">@image html animated_image.png</td>
            <td>
                The "animated image" control displays an image, which is animated in a flip-book manner. It supports an arbitrary number of images and a free settable animation time. The "animated image" control can be used e.g., for simple animations based on a collection of single images (Class: CGUIAnimatedImage).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    This control displays an image which is animated in a flip-book manner.
    You can add an arbitrary number of images to the animation, choose the speed of animation
    and whether the animation should loop endlessly or stop after one animation cycle.

    Example:
@code
    // Create animated image
    CGUIAnimatedImage* pImage = new CGUIAnimatedImage(
                        this,
                        eC_FromInt(10), eC_FromInt(10),
                        eC_FromInt(100), eC_FromInt(50)),
                        false, 80, OBJ_ANIMATED_EXAMPLE_IMAGE);
    // Fill array of images
    eC_TArray<ImageResource_t> aImages;
            aImages6.AddElement( IMG_LOGO1);
            aImages6.AddElement( IMG_LOGO2);
            aImages6.AddElement( IMG_LOGO3);
            aImages6.AddElement( IMG_LOGO4);
            aImages6.AddElement( IMG_LOGO5);
    // Set images
    pImage->SetImages( aImages);
    // Do not loop animation
    pImage->SetRepeat( false);
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIAnimatedImage
    : public CGUIImage
{
public:
    /** Constructs a CGUIAnimatedImage object.
        Note that if the given width and height do not match the actual size
        of the image it will be stretched when drawing it, resulting in a
        potential performance loss.
        @param pParent Pointer to the parent object of this image.
        @param vX The x coordinate of the upper left corner of this image.
        @param vY The y coordinate of the upper left corner of this image.
        @param vWidth The width of this image.
        @param vHeight The height of this image.
        @param bStretchBlit If true image will be stretched to the size of the object, if false image will be centered.
        @param uiMs Time in milliseconds before displaying the next image.
        @param eID Object Identifier of this object
        */
    CGUIAnimatedImage(
        CGUICompositeObject* const pParent,
        const eC_Value& vX,
        const eC_Value& vY,
        const eC_Value& vWidth,
        const eC_Value& vHeight,
        const eC_Bool& bStretchBlit = true,
        const eC_UInt uiMs = 100,
        const ObjectHandle_t& eID = NO_HANDLE);

    /** Constructs a CGUIAnimatedImage object.
        Note that if the given width and height do not match the actual size
        of the image it will be stretched when drawing it, resulting in a
        potential performance loss.
        @param pParent Pointer to the parent object of this image.
        @param kRect Boundary rectangle of the animated image, relative to parent.
        @param bStretchBlit If true image will be stretched to the size of the object, if false image will be centered.
        @param uiMs Time in milliseconds before displaying the next image.
        @param eID Object Identifier of this object
        */
    CGUIAnimatedImage(
        CGUICompositeObject* const pParent,
        const CGUIRect& kRect,
        const eC_Bool& bStretchBlit,
        const eC_UInt uiMs,
        const ObjectHandle_t& eID);

    CGUIAnimatedImage();

    virtual ~CGUIAnimatedImage();

    /** Sets the animation index to the given value.
        @see SetAnimationIndex()
        @param rkValue The CGUIValue containing the new value
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        // Show image at index given by value
        SetAnimationIndex( rkValue.ToInt() );
        CGUIObject::SetValue( rkValue);
        return true; 
    }

    eC_Bool DoDraw();

    /// Animation implementation which cycles through all images within this instance of CGUIAnaimatedImage.
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Sets the images which will be displayed while animating this control. You can supply any number of images,
        which will then be shown in a flip-book manner at a configurable speed (@see SetFrameDelay).
        The images will be displayed in the order in which they are present within the
        array that is being handed over to SetImages. Any formerly set images will be overwritten.
        @param aImages Array of ImageResource identifiers of images.
    */
    void SetImages( eC_TArray<ImageResource_t> &aImages);

    /** Sets the time in milliseconds before displaying the next image of the animation, thus defining the animation speed.
        @param uiMs Time in milliseconds before displaying the next image. (Supplying 0 will halt the animation)
    */
    void SetFrameDelay( const eC_UInt uiMs);

    /** Returns the current frame delay.
        @return The time in milliseconds between individual images.
        @see SetFrameDelay
      */
    eC_UInt GetFrameDelay() const { return m_uiFrameDelay; }

    /// Immediately stops the animation.
    void StopAnimation();

    /// Starts the animation.
    void StartAnimation();

    /** Forces the animation to continue with the image at the given index.
        The index refers to the position of the image in the array which was handed over during the call to SetImages().
        Illegal index values will be clipped by a modulo operation to the range of the array.
        @param uiIndex Index of image within the array of images for animation
        */
    void SetAnimationIndex( const eC_UInt uiIndex);

    /** This enables/disables automatic looping of the animation.
        Setting "repeat" to true will cause the animation to restart from the first image once the last image has been shown.
        Setting "repeat" to false will stop the animation once the last image has been shown.
        @param bRepeat Turn automatic looping of the animation on/off.
        */
    void SetRepeat( const eC_Bool bRepeat) { m_bRepeat = bRepeat;}

    /** Returns the image stored at the given index of the array or DUMMY_IMAGE if index is out of range.
        @param uiIndex Image at position to return.
        @return image stored at the given index of the array or DUMMY_IMAGE if index is out of range.
        */
    ImageResource_t GetImage(eC_UInt uiIndex) const;

    /** Returns the index of the currently visible image within the array of images. See SetImages().
        @return Index of the currently visible image
    */
    eC_UInt GetIndexOfVisibleImage() const { return m_uiVisibleImageIndex; }

    /** Returns the number of images currently loaded to determine the framedelay when having a constant cycle-time.
        @return Number of currently used images
    */
    inline eC_UInt GetNumberOfImages() const { return m_aImageIDs.GetQuantity(); }

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Array of image ids within animation
    eC_TArray<ImageResource_t>  m_aImageIDs;
    /// boolean value indicating whether this object is currently animating
    eC_Bool                     m_bRepeat;
    /// boolean value for automatically starting the animation
    eC_Bool                     m_bAutoStart;
    /// Time in milliseconds how long a single image of the animation will be displayed
    eC_UInt                     m_uiFrameDelay;
    /// Index of currently displayed image in array of images
    eC_UInt                     m_uiVisibleImageIndex;

private:
    /// Internal initialization. Helper for constructors.
    void Init();

    /// Releases all images. Helper for destructor and ReadFromStream().
    void DeInit();
};

#endif
