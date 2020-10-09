/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIBASEBITMAP__H_
#define GUIBASEBITMAP__H_

#include "eC.h"
#include "GUITrace.h"

class CGfxWrap;

/** @defgroup GUILIANI_BITMAPPLANES Bitmap planes
    @brief Bitmap planes allow offscreen rendering and displaying of offscreen surfaces
    with additional transformations in three dimensions.

    BitmapPlanes are used to create "Snapshots" of objects (even CGUICompositeObject).
    These can then be used e.g. to create advanced graphical effects such as screen transitions.
    Another use-case for snapshots is performance optimization. It might be faster to create snapshots
    of complex scenes and to render only these snapshots instead of all contained sub-elements.

    A bitmap plane can be set as the render target in the graphics wrapper (see CGfxWrap::DrawToBitmap()).
    All successive render calls will affect the bitmap plane instead of the framebuffer.

    Each bitmap plane is coupled to a CGUIObject. The GUIObjects spans a right-handed coordinate system
    with the x-axis pointing to the right and the y-axis pointing to the bottom. The center of this
    system is the center of the CGUIObject.
    As the bitmap plane uses graphics library specific drawing code (e.g. Textures in OpenGL, eGML_Bitmaps in
    eGML etc.) it is necessary to have platform-specific bitmap plane implementations.
    That is being handled transparently for user code, so that your offscreen rendering code stays portable.

    Example 1:
    The following code snippet demonstrates how to create a bitmap plane, render graphics into it, and
    how to use it later on:
@code
    // Create a new offscreen bitmap
    m_kBitmapPlaneID = GETGFX.CreateBitmapPlane( *this, GetWidth(), GetHeight() );
    // The new offscreen bitmap shall be the render target for all succeeding render operations
    GETGFX.DrawToBitmap( m_kBitmapPlaneID );
    // Now blit an image into the offscreen bitmap
    GETGFX.BlitImgExt(  m_eImageID,
                        0,0,
                        GETGFX.GetImgWidth( m_eImageID), GETGFX.GetImgHeight( m_eImageID),
                        0,0,
                        GetWidth(), GetHeight(),
                        255);
    // Rendering to offscreen bitmap is done. Continue rendering to screen again.
    GETGFX.DrawToScreen();
    // If you later wish to display the offscreen bitmap which you just created,
    // you can use this code inside of DoDraw()
    GETGFX.GetBitmapPlane(m_kBitmapPlaneID).DoDraw();
@endcode

    Example 2:
    This example shows how you can take a "snapshot" of an object (and all contained objects if it is a CGUICompositeObject):
@code
    // Use the CreateSnapshot interface to render the object as it is into an offscreen bitmap.
    // This will use the object's own internal bitmap plane and clear it with the specified color.
    // Note: Please be aware that many graphic APIs do not support clearing of alpha buffers,
    // which results in visual artifacts if you are trying to create partially transparent offscreen bitmaps.
    CreateSnapshot(0xFF000000);
    // If you wish to display the snapshot instead of calling the object's usual
    // drawing code in DoDraw(), use the following interface:
    EnableSnapshotDrawing();
@endcode

    Example 3:
    Another way to render an object into a bitmap plane, is to use the RenderToBitmap(...) interface. This will temporarily set the supplied
    bitmap plane as the render target, and call the object's (and potential child-object's) DoDraw() methods.
@code
    // Clear the bitmap plane with some color
    GETGFX.GetBitmapPlane( BitmapPlaneID).Clear(255, 255, 255, 0);
    // Tell object to render itself (and all its children) into the BitmapPlane
    pObject->RenderToBitmap( BitmapPlaneID );
@endcode

@section sec_cguibitmapplane_screen_transitions Screen Transitions
    Screen transitions such as blend and shrink, blend fade, rotate, etc. involve capturing snapshots of the current
	screen and the screen to be shown after the transition, and then rendering these snapshots according to type of
	the transition. To capture a snapshot is to draw the objects of the screen on a bitmap plane. One bitmap plane for
	each snapshot is created. The bitmap planes created are set as a render target and all the images of the screens
	are blitted on their respective bitmap planes. Then, based on the type of the transition, the bitmap planes are
	drawn on the frame buffer and the frame buffer is displayed, resulting in expected transition effect. For example,
	for blend and shrink transition, the bitmap plane of the current screen is continuously shrunk and drawn on the
	frame buffer, while the bitmap plane of next screen is continuously enlarged and drawn on the frame buffer. Please
	note that not all types of transition require bitmap planes. For example, the transitions like push from top, push
	from bottom, push from left, push from right, dissolve, etc. do not require bitmap planes.
    <p>
    During transition of screens, the interactions among the different classes of Guiliani are shown in the
    sequence diagram below.

    @image html Sequence_Diagram_Bitmap_Planes.png

The numbers on the leftmost part of the sequence diagram represent the sequence numbers and the associated operations
for these numbers are described as follows:
-# CGUITransition class is responsible for initialization, update and end of a transition. It commands CGUIObject to
create a snapshot of the current screen. CGUIObject signals graphics library specific wrapper class (e.g. CGfxWrapGL,
CGfxWrapeGML, etc.) to create an object of bitmap plane. As an example, the sequence diagram shows the wrapper class
CGfxWrapGL and bitmap plane class CGUIBitmapPlane_GL for OpenGL library. The wrapper class creates a bitmap plane of
expected size and returns ID 1. Each bitmap plane is assigned an ID. The IDs are assigned in increasing order starting
from number 1. The screen or the frame buffer has always the ID 0.
-# The bitmap plane created in the last step, is set as a render target in the wrapper class by calling DrawToBitmap
method. All blitting operations will now affect the bitmap plane 1 instead of frame buffer.
-# CGUIObject calls the DoDraw methods of CGUIImage or other objects on the screen such as CGUIButton, CGUIScrollBar,
CGUIComboBox, etc. The CGUIImage class (or a child class of CGUIObject present on the screen) calls BlitImage method
of the wrapper class. The wrapper class blits the images or objects on the bitmap plane.
-# The frame buffer is set as a render target.
-# Snapshot drawing is enabled for the bitmap plane to draw the contents of the bitmap plane on the frame buffer.
-# CGUITransition creates a snapshot of a screen to be shown after the transition. This is done by following the steps
from 1 to 5 explained above.
-# The snapshots of the two screens are ready by this step. The CGUI class now commands wrapper class to redraw GUI.
Since drawing of snapshots is already enabled, snapshots or bitmap planes are drawn. The CGUIObject class calls the
DoDraw method of each of the bitmap plane objects. The contents of the bitmap planes are drawn by CGUIBitmapPlane_GL
class on the frame buffer. The dimensions and the alpha values of bitmap planes are continuously updated according to
type of transition. The Refresh method of wrapper class is called to show the drawn contents in the frame buffer on the
screen.
-# Snapshot drawing is disabled for both the screens.
-# Remaining drawing operation of the second screen is completed and the transition operation is finished.


@section sec_cguibitmapplane_alpha Alpha transparency and bitmap planes
    Be aware that using alpha transparency (both per-pixel and global alpha) within BitmapPlanes can lead to unexpected results.
    Depending on the underlying graphics system it might be that alpha functionality is not supported. Please refer to your corresponding platform
    documentation to find out about any limitations.
    <p>
    Creating snapshots of objects which have a global alpha value (i.e. they are not fully opaque) will lead to visual results that differ from the
    non-snapshot appearance of the object. This is because while rendering to an offscreen buffer all graphics will be blended against the buffer's
    background. Even if this background is colored fully transparent black (color value 0x00000000) this will still blend the RGB color components 
    against black. The resulting bitmapplane will thus appear darker than the original object.
    <p>
    A similar effect can be observed when taking a snapshot of an object with text (e.g. a button) and then drawing the snapshot at 50% alpha.
    If you compare the visual result of the snapshot with that of the original object being drawn at 50% alpha, you will notice that the text of the
    original object will let the button's background shine through, whereas the snapshot's text will have the background underneath the button 
    shining through. The explanation of this observation is, that the snapshot will be blended as one monolithic graphic, while all elements of the 
    original object (The button image, as well as the text glyphs) will be blended individually.
*/

/// Used for unique identifiers of bitmap planes.
typedef eC_UInt GUIBitmapPlaneID_t;

/// The bitmap plane base class.
/** This class defines the standard interface for every BitmapPlane.
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane
{
    friend class CGfxWrap;

public:
    // ---------------- 2D interface ----------------------------
    /** @name 2D positioning */
    ///@{
    /** Sets the Position of this BitmapPlane in screen coordinates.
        Position will be set relative to the upper left corner of the parent GUIObject.
        The Z Coordinate will be set in that way that a 10x10 BitmapPlane has the size of 10x10 pixel on the screen
        @param vXPos x coordinate relative to the upper left corner of the parent GUIObject
        @param vYPos y coordinate relative to the upper left corner of the parent GUIObject
    */
    void SetPos2D(eC_Value vXPos, eC_Value vYPos);

    /** Sets the X-Position of this BitmapPlane in screen coordinates.
        Position will be set relative to the upper left corner of the parent GUIObject.
        The Z Coordinate will be set in that way that a 10x10 BitmapPlane has the size of 10x10 pixel on the screen
        @param vXPos x coordinate relative to the upper left corner of the parent GUIObject
    */
    void SetXPos2D(eC_Value vXPos);

    /** Sets the Y-Position of this BitmapPlane in screen coordinates.
        Position will be set relative to the upper left corner of the parent GUIObject.
        The Z Coordinate will be set in that way that a 10x10 BitmaPlane has the size of 10x10 pixel on the screen
        @param vYPos y coordinate relative to the upper left corner of the parent GUIObject
    */
    void SetYPos2D(eC_Value vYPos);

    /** Accesor for the X-Position in screen coordinates.
        @return x coordinate relative to the upper left corner of the parent GUIObject.
    */
    eC_Value GetXPos2D() const;

    /** Accesor for the Y-Position in screen coordinates.
        @return y coordinate relative to the upper left corner of the parent GUIObject.
    */
    eC_Value GetYPos2D() const;
    ///@}

    // ------------  3D interface  -----------------------
    /** @name 3D positioning */
    ///@{
    /** Sets the Position of this BitmapPlane in 3D space.
        Point of origin is the center of the parent GUIObject.
        @param vXPos x coordinate
        @param vYPos y coordinate
        @param vZPos z coordinate
    */
    inline void SetPos3D(eC_Value vXPos, eC_Value vYPos, eC_Value vZPos)
    {
        SetXPos3D(vXPos);
        SetYPos3D(vYPos);
        SetZPos3D(vZPos);
    }

    /** Sets the X-Position of this BitmapPlane in 3D space.
        Point of origin is the center of the parent GUIObject.
        @param vXPos x coordinate
    */
    inline void SetXPos3D(const eC_Value vXPos){m_vXPos = vXPos;}

    /** Sets the YPosition of this BitmapPlane in 3D space.
        Point of origin is the center of the parent GUIObject.
        @param vYPos y coordinate
    */
    inline void SetYPos3D(const eC_Value vYPos){m_vYPos = vYPos;}

    /** Sets the Z-Position of this BitmapPlane in 3D space.
        Point of origin is the center of the parent GUIObject.
        @param vZPos z coordinate
    */
    inline void SetZPos3D(const eC_Value vZPos){m_vZPos = vZPos;}


    /** Accesor for the X-Position in 3D space.
        @return x coordinate.
    */
    inline eC_Value GetXPos() const {return m_vXPos;}

    /** Accesor for the y-Position in 3D space.
        @return y coordinate.
    */
    inline eC_Value GetYPos() const {return m_vYPos;}

    /** Accesor for the Z-Position in 3D space.
        @return z coordinate.
    */
    inline eC_Value GetZPos() const {return m_vZPos;}
    ///@}
    /** @name Rotation */
    ///@{
    // ------------  Rotation interface  ---------------
    /** Sets the X-Rotation of this BitmapPlane. Object will be rotated around its center.
        @param vXAngle X-Angle in degree
    */
    inline void RotateX(const eC_Value vXAngle) {m_vXAngle = vXAngle;}

    /** Sets the Y-Rotation of this BitmapPlane. Object will be rotated around its center.
        @param vYAngle Y-Angle in degree
    */
    inline void RotateY(const eC_Value vYAngle) {m_vYAngle = vYAngle;}

    /** Sets the Z-Rotation of this BitmapPlane. Object will be rotated around its center.
        @param vZAngle Z-Angle in degree
    */
    inline void RotateZ(const eC_Value vZAngle) {m_vZAngle = vZAngle;}

    /** Accessor for X-Angle.
        @return X-Angle in degree.
    */
    inline eC_Value GetXAngle() const {return m_vXAngle;}

    /** Accessor for Y-Angle.
        @return Y-Angle in degree.
    */
    inline eC_Value GetYAngle() const {return m_vYAngle;}

    /** Accessor for Z-Angle.
        @return Z-Angle in degree.
    */
    inline eC_Value GetZAngle() const {return m_vZAngle;}

    /** Resets all three rotation angles.
    */
    void ResetRotation();
    ///@}
   
    /** Accessor for the unique BitmapPlane ID.
        @return unique BitmapPlane ID.
    */
    inline GUIBitmapPlaneID_t GetID() const {return m_uiID;}
    /** @name Size */
    ///@{
    /** Setter for the width. The scale parameter which will be used for the autoscale mechanism will be updated.
        @param vWidth new width of this BitmapPlane.
        @param bExpandCentered if true Plane will be expanded to left and right, otherwise only to the right. default is false
    */
    virtual void SetWidth(eC_Value vWidth, eC_Bool bExpandCentered = false);

    /** Accessor for the width. The scale parameter which will be used for the autoscale mechanism will be updated.
        @return width of this BitmapPlane.
    */
    inline eC_Value GetWidth() const {return m_vWidth;}

    /** Accessor for the width, corrected by the deformation of the parent GUIObject.
        @return corrected width of this BitmapPlane.
    */
    eC_Value GetWidthCorrected() const;

    /** Setter for the height.
        @param vHeight new height of this BitmapPlane.
        @param bExpandCentered if true Plane will be expanded to top and bottom, otherwise only to the bottom. default is false
    */
    virtual void SetHeight(eC_Value vHeight, eC_Bool bExpandCentered = false);

    /** Accessor for the height.
        @return height of this BitmapPlane.
    */
    inline eC_Value GetHeight() const {return m_vHeight;}

    /** Accessor for the height, corrected by the deformation of the parent GUIObject.
        @return corrected height of this BitmapPlane.
    */
    eC_Value GetHeightCorrected() const;

    /** Enables/Disables the AutoScale mode.
    * If AutoScale mode is active the BitmapPlane is resized if the parent GUIObject is resized
    * By default AutoScale mode is active
    * @param bAutoScale False if the autoscale mode should be disabled otherwise true
    */
    inline void SetAutoScale(const eC_Bool bAutoScale) { m_bAutoScale = bAutoScale; }
    ///@}
    /** @name Drawing */
    ///@{
    /** Draws this BitmapPlane.
        The Bitmap will be drawn to the current RenderTarget, with the set rotation and position.
        The set cliprect also applies to this drawing.
    */
    virtual void DoDraw();
    
    /** Clears this bitmap with the given color.
        Attention: Please note that clearing Alpha values is unfortunately not supported by all Graphics APIs!
        @param ubRed is the red part of the clear color
        @param ubGreen is the green part of the clear color
        @param ubBlue is the blue part of the clear color
        @param ubAlpha is the alpha part of the clear color
    */
    virtual void Clear(const eC_UByte ubRed = 0, const eC_UByte ubGreen = 0, const eC_UByte ubBlue = 0, const eC_UByte ubAlpha = 0){}

    /** Accessor for parent GUIObject
        @return const reference to the parent GUIObject
    */
    inline const CGUIObject& GetParentGUIObject() const {return m_rkParentObject;}

    /** Sets the alpha values for all corners of the Bitmap plane.
        @param ubTopLeft     alpha value for the upper left corner
        @param ubBottomLeft  alpha value for the lower left corner
        @param ubBottomRight alpha value for the lower right corner
        @param ubTopRight    alpha value for the upper right corner
    */
    virtual void SetAlphaValues(const eC_UByte ubTopLeft, const eC_UByte ubBottomLeft,
                                const eC_UByte ubBottomRight, const eC_UByte ubTopRight)
    {
    }

    /**
        Set one alpha value for all corners of the Bitmap plane.
        @param ubAlpha The Alpha value for the Bitmap plane.
    */
    void SetAlphaValue(const eC_UByte ubAlpha)
    {
        SetAlphaValues(ubAlpha, ubAlpha, ubAlpha, ubAlpha);
    }
    ///@}
    // -------------- UV interface --------------------------------
    //---- Top Left --
    /** @name Texturing Top Left */
    ///@{
    /** Sets the U Value of the upper left corner of the BitmapPlane.
        The U value describes which point of the Bitmap is mapped onto the plane in x direction.
        The value has to be between 0 and 1, where zero means the upper left corner of the plane maps directly
        on the left edge of the bitmap and 1 the upper left corner maps on the right edge
        of the bitmap. All values in between will be interpolated.
        @param vU new U value for the upper left corner. [0 1]
    */
    virtual void SetTopLeftU(eC_Value vU);

    /** Accessor for the upper left U value.
        @returns U value of the upper left corner of the bitmap plane.
    */
    virtual eC_Value GetTopLeftU();

    /** Sets the V Value of the upper left corner of the BitmapPlane.
        The V value describes which point of the Bitmap is mapped onto the plane in y direction.
        The value has to be between 0 and 1, where zero means the upper left corner of the plane maps directly
        on the upper edge of the bitmap and 1 the upper left corner maps on the lower edge
        of the bitmap. All values in between will be interpolated.
        @param vV new V value for the upper left corner. [0 1]
    */
    virtual void SetTopLeftV(eC_Value vV);

    /** Accessor for the upper left V value.
        @returns U value of the upper left corner of the bitmap plane.
    */
    virtual eC_Value GetTopLeftV();

    /** Sets the U/V values for the top left corner of the bitmap plane.
        @param vU new U value for the top left corner
        @param vV new V value for the top left corner
        @see SetTopLeftV, SetTopLeftU
    */
    inline void SetTopLeftUV(eC_Value vU, eC_Value vV)
    {
        SetTopLeftU(vU);
        SetTopLeftV(vV);
    }
    ///@}

    //---- Bottom Left --
    /** @name Texturing Bottom Left */
    ///@{
    /** Sets the U Value of the lower left corner of the BitmapPlane.
        The U value describes which point of the Bitmap is mapped onto the plane in x direction.
        The value has to be between 0 and 1, where zero means the lower left corner of the plane maps directly
        on the left edge of the bitmap and 1 the lower left corner maps on the right edge
        of the bitmap. All values in between will be interpolated.
        @param vU new U value for the lower left corner. [0 1]
    */
    virtual void SetBottomLeftU(eC_Value vU);

    /** Accessor for the lower left U value.
        @returns U value of the lower left corner of the bitmap plane.
    */
    virtual eC_Value GetBottomLeftU();

    /** Sets the V Value of the lower left corner of the BitmapPlane.
        The V value describes which point of the Bitmap is mapped onto the plane in y direction.
        The value has to be between 0 and 1, where zero means the lower left corner of the plane maps directly
        on the upper edge of the bitmap and 1 the lower left corner maps on the lower edge
        of the bitmap. All values in between will be interpolated.
        @param vV new V value for the lower left corner. [0 1]
    */
    virtual void SetBottomLeftV(eC_Value vV);

    /** Accessor for the lower left V value.
        @returns U value of the lower left corner of the bitmap plane.
    */
    virtual eC_Value GetBottomLeftV();

    /** Sets the U/V values for the lower left corner of the bitmap plane.
        @param vU new U value for the lower left corner
        @param vV new V value for the lower left corner
        @see SetBottomLeftV, SetBottomLeftU
    */
    inline void SetBottomLeftUV(eC_Value vU, eC_Value vV)
    {
        SetBottomLeftU(vU);
        SetBottomLeftV(vV);
    }
    ///@}

    //--- Bottom Right
    /** @name Texturing Bottom Right */
    ///@{
    /** Sets the U Value of the bottom right corner of the BitmapPlane.
        The U value describes which point of the Bitmap is mapped onto the plane in x direction.
        The value has to be between 0 and 1, where zero means the lower right corner of the plane maps directly
        on the left edge of the bitmap and 1 the lower right corner maps on the right edge
        of the bitmap. All values in between will be interpolated.
        @param vU new U value for the lower right corner. [0 1]
    */
    virtual void SetBottomRightU(eC_Value vU);

    /** Accesoor for the lower right U value.
        @returns U value of the lower right corner of the bitmap plane.
    */
    virtual eC_Value GetBottomRightU();

    /** Sets the V Value of the lower left corner of the BitmapPlane.
        The V value describes which point of the Bitmap is mapped onto the plane in y direction.
        The value has to be between 0 and 1, where zero means the lower left corner of the plane maps directly
        on the upper edge of the bitmap and 1 the lower left corner maps on the lower edge
        of the bitmap. All values in between will be interpolated.
        @param vV new V value for the lower left corner. [0 1]
    */
    virtual void SetBottomRightV(eC_Value vV);

    /** Accessor for the lower right V value.
        @returns U value of the lower right corner of the bitmap plane.
    */
    virtual eC_Value GetBottomRightV();

    /** Sets the U/V values for the lower right corner of the bitmap plane.
        @param vU new U value for the lower right corner
        @param vV new V value for the lower right corner
        @see SetBottomRightV, SetBottomRightU
    */
    inline void SetBottomRightUV(eC_Value vU, eC_Value vV)
    {
        SetBottomRightU(vU);
        SetBottomRightV(vV);
    }
    ///@}

    //---- Top Right ----
    /** @name Texturing Top Right */
    ///@{
    /** Sets the U Value of the upper right corner of the BitmapPlane.
        The U value describes which point of the Bitmap is mapped onto the plane in x direction.
        The value has to be between 0 and 1, where zero means the upper right corner of the plane maps directly
        on the left edge of the bitmap and 1 the upper right corner maps on the right edge
        of the bitmap. All values in between will be interpolated.
        @param vU new U value for the upper left corner. [0 1]
    */
    virtual void SetTopRightU(eC_Value vU);

    /** Accessor for the upper right U value.
        @returns U value of the upper right corner of the bitmap plane.
    */
    virtual eC_Value GetTopRightU();

    /** Sets the V Value of the upper right corner of the BitmapPlane.
        The V value describes which point of the Bitmap is mapped onto the plane in y direction.
        The value has to be between 0 and 1, where zero means the upper right corner of the plane maps directly
        on the upper edge of the bitmap and 1 the upper right corner maps on the lower edge
        of the bitmap. All values in between will be interpolated.
        @param vV new V value for the upper left corner. [0 1]
    */
    virtual void SetTopRightV(eC_Value vV);

    /** Accessor for the upper right V value.
        @returns U value of the upper right corner of the bitmap plane.
    */
    virtual eC_Value GetTopRightV();

    /** Sets the U/V values for the upper right corner of the bitmap plane.
        @param vU new U value for the upper right corner
        @param vV new V value for the upper right corner
        @see SetTopRightV, SetTopRightU
    */
    inline void SetTopRightUV(eC_Value vU, eC_Value vV)
    {
        SetTopRightU(vU);
        SetTopRightV(vV);
    }
    ///@}
    /** @name Projection */
    ///@{
    /** Enables the DepthTest for this Plane.
        If DepthCorrection is enabled the BitmapPlane may be partly or completely invisible,
        even if its parent Object is completely visible, because if the DepthTest is enabled
        the visibility depends on the Z-Position.
        DepthTest is not supported on all Graphics Wrappers.
        @see EnableDepthTest()
    */
    inline void EnableDepthTest(){m_bDepthTest = true;}

    /** Disables the DepthTest for this Plane.
        If DepthCorrection is disabled the BitmapPlane will always be visible if the parent object is visible.
        The Z Position of the Plane is ignored for visibility check.
        DepthTest is not supported on all Graphics Wrappers.
        @see DisableDepthTest()
    */
    inline void DisableDepthTest(){m_bDepthTest = false;}

    /** Checks whether the DepthTest is enabled or not.
        @return True if DepthTest is enabled, otherwise False.
        @see EnableDepthTest()
        @see DisableDepthTest()
    */
    inline eC_Bool IsDepthTestEnabled() const {return m_bDepthTest;}

    /** Calculates the distance where 10, 10 rect has the size of 10, 10 pixel
        @return z-distance of the 2D plane
    */
    virtual eC_Value Get2DPlaneDistance();

    /// @return static Near/Far Plane and Field of View, these values are the same for all CGUIBitmaps
    inline eC_Value GetFOV() const {return m_vFOV;}

    /// @param vFOV static Near/Far Plane and Field of View, these values are the same for all CGUIBitmaps
    inline void SetFOV(const eC_Value vFOV)
    {
        m_vFOV = vFOV;
        //we need to recalculate the 2dplane distance and therefore the z pos
        eC_Value vZ = m_vZPos - m_v2DPlaneDistance;
        m_v2DPlaneDistance = eC_FromInt(0);
        m_uiScreenWidth = 0;
        m_uiScreenHeight = 0;
        m_vZPos = Get2DPlaneDistance() + vZ;
    }

    /// @return near plane for viewing frustum
    inline eC_Value GetNear() const {return ms_vNear;}

    /// @return far plane for viewing frustum
    inline eC_Value GetFar() const {return ms_vFar;}
    ///@}
protected:
    /// only the derived classes and GfxWrapper are allowed to create or destroy BitmapPlanes.
    CGUIBitmapPlane(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth = 0, eC_Value vHeight = 0);

    virtual ~CGUIBitmapPlane();
    /** @name Avtivation/Deactivation */
    ///@{
    /// Called by the framework when the drawing operation into the bitmapplane has finished.
    virtual eC_Bool FinishDrawToBitmap() { return true; }
    /// Called by the framework when the drawing operation into the bitmapplane is about to begin.
    virtual eC_Bool DrawToBitmap() { return true; }
    /// Called by the framework when subsequent drawing operations shall happen on the normal UI screen.
    virtual eC_Bool DrawToScreen() { return true; }
    ///@}


    /** Accessor for the x coordinate of the center of projection.
        The center of projection is center of origin, by default it is the center of the parent GUIObject
        @return x coordinate of the Center of projection
    */
    eC_Value GetCenterOfProjectionAbsX() const;

    /** Accessor for the y coordinate of the center of projection.
        The center of projection is center of origin, by default it is the center of the parent GUIObject
        @return y coordinate of the Center of projection
    */
    eC_Value GetCenterOfProjectionAbsY() const;

    /** Sets a new value for the used texture part
        @param vNewTexUsedU new u value representing the used part of the texture
        @param vNewTexUsedV new v value representing the used part of the texture*/
    void SetTexUsed(eC_Value vNewTexUsedU, eC_Value vNewTexUsedV);

    /// Width of the GUIObject at creation time of this BitmapPlane.
    eC_Value m_vGUIObjectInitWidth;

    /// Height of the GUIObject at creation time of this BitmapPlane.
    eC_Value m_vGUIObjectInitHeight;

    eC_Bool m_bAutoScale; ///< flag describing a bitmap plane will be automatically be scaled on GUIObject scale

    eC_TArray<eC_Value> m_avUVValues; ///< array containing the uv values for all four plane corners

    /// Used space/unused space in x direction in the Bitmap, due to Power of two limitations [0 1]
    eC_Value m_vTexUsedU;
    /// Used space/unused space in y direction in the Bitmap, due to Power of two limitations [0 1]
    eC_Value m_vTexUsedV;

    eC_UInt m_uiScreenWidth;     ///< screen width, used to identify screen changes on calculation of 2DPlaneDistance
    eC_UInt m_uiScreenHeight;    ///< screen height, used to identify screen changes on calculation of 2DPlaneDistance
    eC_Value m_v2DPlaneDistance; ///< 2DPlaneDistance
private:
    /** Default-constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
    */
    CGUIBitmapPlane();

    GUIBitmapPlaneID_t  m_uiID; ///< unique ID of this BitmapPlane, set by the GFXWrapper

    const CGUIObject& m_rkParentObject; ///< reference to parent GUIObject

    eC_Value m_vXPos; ///< X Coordinate in 3D Space Relative to the Center of Projection
    eC_Value m_vYPos; ///< Y Coordinate in 3D Space Relative to the Center of Projection
    eC_Value m_vZPos; ///< Z Coordinate in 3D Space Relative to the Center of Projection

    eC_Value m_vXAngle; ///< X angle in degree
    eC_Value m_vYAngle; ///< Y angle in degree
    eC_Value m_vZAngle; ///< Z angle in degree

    eC_Value m_vWidth;  ///< width of this BitmapPlane
    eC_Value m_vHeight; ///< height of this BitmapPlane

    eC_Bool m_bDepthTest; ///< defines whether depth-testing is enabled

    eC_Value m_vFOV;  ///< field of view for viewing frustum
    static eC_Value ms_vNear; ///< near plane for viewing frustum
    static eC_Value ms_vFar;  ///< far plane for viewing frustum
};

#endif
