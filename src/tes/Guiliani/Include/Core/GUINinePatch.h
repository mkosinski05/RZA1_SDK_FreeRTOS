/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUININEPATCH__H_
#define GUININEPATCH__H_
#include "eC_Math.h"

/// Helper class to for storing Nine-Patch images

/** A Nine Patch is a smart way of scaling up bitmaps without the usual quality losses by subdividing the source image into
    nine sections and blitting the border sections unstretched, while only stretching the center section.
    This is particularly useful for mostly rectangular images in use-cases such as a scaled button, a progressbar or a scrollbar knob.
    <p>
    The Nine Patch is defined by the width/height of its <b>top, bottom, left and right borders</b>. 
    These are given in pixels and relative to the image's top/left, and bottom/right corners.
    Typically, you will wish to set these values so that they encapsulate the areas of your source-image which shall remain in their
    original size, such as the edges or drop shadows of a button. The following image illustrates this concept.

@image html NinePatch_Borders.png

    These examples show how the original image appears when being stretched, with regard to different Nine Patch borders.
    You can clearly see how much the appearance of the stretched button has improved. 
    <p>
    Please note that this comes at a slight performance cost, since a total of 9 blit operations has to be performed. 
    Therefore, if you wish to avoid this, either blit the source image in its original size, or set all border widths to zero, 
    which will result in one simple stretch-blit for the entire image.
    <p>
    Supplying the Nine Patch with illegal values, such as border sizes which exceed the original image's dimensions, will result
    in a visual output as shown in the two examples at the bottom. Also, make sure, that you leave a center section of at least 1 pixel,
    so that the center area will be filled and not remain empty.

@image html NinePatch_Examples.png
*/
class CGUINinePatch
{
public:
    /** Standard constructor. Initializes all Borders with 0. */
    CGUINinePatch();

    /** Constructor. 
        @param uiTop    Width of top border in pixels
        @param uiBottom Width of uiBottom border in pixels
        @param uiLeft   Width of uiLeft border in pixels
        @param uiRight  Width of uiRight border in pixels
        */
    CGUINinePatch(const eC_UInt uiTop, const eC_UInt uiBottom, const eC_UInt uiLeft, const eC_UInt uiRight);

    /** Assign values for all four borders to this Nine Patch.
        @param uiTop    Width of top border in pixels
        @param uiBottom Width of uiBottom border in pixels
        @param uiLeft   Width of uiLeft border in pixels
        @param uiRight  Width of uiRight border in pixels
    */
    void Assign(const eC_UInt uiTop, const eC_UInt uiBottom, const eC_UInt uiLeft, const eC_UInt uiRight)
    {
        m_uiTopBorder=      uiTop;
        m_uiBottomBorder=   uiBottom;
        m_uiLeftBorder=     uiLeft;
        m_uiRightBorder =   uiRight;
    }

    /// @return The current top border in pixels
    inline eC_UInt GetTop() const    { return m_uiTopBorder; }
    /// @return The current bottom border in pixels
    inline eC_UInt GetBottom() const { return m_uiBottomBorder; }
    /// @return The current left border in pixels
    inline eC_UInt GetLeft() const   { return m_uiLeftBorder; }
    /// @return The current right border in pixels
    inline eC_UInt GetRight()  const { return m_uiRightBorder; }

#ifdef GUILIANI_STREAM_GUI 
    /**
        Please refer to the description of classes using this class, i.e. CGUIButton.
        @see CGUIFactory, CGUIButton::ReadFromStream().
    */
    void ReadFromStream();
#endif 

#ifdef GUILIANI_WRITE_GUI 
    /**
        Please refer to the description of classes using this class, i.e. CGUIButton.
        @see CGUIStreamWriter, CGUIButton::WriteToStream().
    */
    void WriteToStream();
#endif

protected:
    
    eC_UInt m_uiTopBorder;///< Nine Patch Top Border size in pixels
    eC_UInt m_uiBottomBorder;///< Nine Patch Bottom Border size in pixels
    eC_UInt m_uiLeftBorder;///< Nine Patch Left Border size in pixels
    eC_UInt m_uiRightBorder;///< Nine Patch Right Border size in pixels
};
#endif
