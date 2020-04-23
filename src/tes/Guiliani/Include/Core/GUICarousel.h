/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICAROUSEL__H_
#define GUICAROUSEL__H_

// necessary includes
#include "GUICompositeObject.h"

/// This composite object arranges and animates its contained children in a "carousel" fashion.
// @guiliani_doxygen toplevel_control Carousel
/**
    <table border="0">
        <tr>
            <td width="200">@image html carousel.png</td>
            <td>
                The "carousel" control is a container which automatically arranges all its children in an elliptic (Pseudo 3D) manner. 
                It supports animation effects like speed-up or slow-down during changing the focus between its children. 
                The carousel is very common to present e.g., Coverflows (Class: CGUICarousel).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The CGUICarousel class is a container which automatically arranges all its children in an elliptic (Pseudo 3D) manner.
    This is a useful control for allowing users to choose between several entries in a menu, or to browse through huge number
    of entries (e.g. cover-art browsing) when deploying the carousel in "flow-mode". (see below for further details)

    <p>
    The Following code creates a simple carousel and fills it with buttons
@code
    // Create the carousel
    CGUICarousel* pCarousel = new CGUICarousel( pParent, 0,0, GetWidth(), GetHeight() );

    // Create some child objects. Note that their positions will be overwritten anyway.
    new CGUIBaseButton( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100), "1", NULL);
    new CGUIBaseButton( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100), "2", NULL);
    new CGUIBaseButton( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100), "3", NULL);
    new CGUIBaseButton( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100), "4", NULL);
    new CGUIBaseButton( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100), "5", NULL);

    // Set a radius, so that the buttons do not appear too crowded
    pCarousel->SetRadius( eC_FromInt(100) );
@endcode

@section sec_cguicarousel_flow_mode Using the carousel in flow-mode
    When working with huge numbers of children in a carousel, you will notice that the carousel tends to appear very crowded.
    This is where the "flow mode" comes in handy. When used in "flow mode", the carousel can contain very large numbers of entries
    of which only a few are visible at any time. You can specify the number of virtual entries in the carousel, which will affect
    the spacing between the visual children, and therefore also number of simultaneously visible ones.
    If for instance you are having a total of 1000 entries in your carousel, and set the number of virtual entries to 10, the carousel
    will arrange its content as if it had only 10 children, by spreading out these 10 along the carousel's 360 degrees.
    But in fact you will only see the currently selected (=frontmost) child object, plus the two children to its left and the two
    children to it's right side.

    <p>
    The Following code creates a carousel in flow mode
@code
    // Create the carousel
    CGUICarousel* pCarousel = new CGUICarousel( pkPage2, 0, 0, eC_FromInt(500), eC_FromInt(200) );

    // Add a lot of content
    CGUIObject* pCarouselEntry = NULL;
    for( int i=0; i < 100; i++)
    {
        pCarouselEntry = new CGUIImage( pCarousel, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(50), SOME_IMAGE_ID, true);
        // The entries shall receive click events, and forward drag events to the carousel
        pCarouselEntry->SetDisabled( false);
    }

    // Set a sensible radius
    pCarousel->SetRadius( eC_FromInt(200) );
    // Tilt the carousel
    pCarousel->SetTiltAngle( eC_FromInt(75) );
    // Arrange the children as if the carousel had only 10 entries
    pCarousel->SetNumberOfFlowModeEntries( 10 );
    // Switch to flow mode
    pCarousel->EnableFlowMode( true);
@endcode

@section sec_cguicarousel_technical_details Technical details regarding the CGUICarousel
    To achieve the carousel effect, the size and position of added child object's will be overwritten (for zooming and rotating).
    The user may then cycle through the child objects, which causes the carousel to rotate around its vertical axis.
    One object - namely the one in the front - is always the currently selected one. All objects except for the selected one
    will be set to non-focusable to enhance focusing behaviour. This ensures that at any time, only the selected object will
    receive the focus.

    @ingroup GUILIANI_CONTROLS
*/
class CGUICarousel
    : public CGUICompositeObject
{
public:
    /** Constructor
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the slider
        @param vHeight Height of the slider
        @param eID Object identifier
    */
    CGUICarousel(
            CGUICompositeObject *const pParent,
            const eC_Value &vX, const eC_Value &vY,
            const eC_Value &vWidth, const eC_Value &vHeight,
            const ObjectHandle_t &eID = NO_HANDLE);

    CGUICarousel();
    virtual ~CGUICarousel() {}

    /** Rotates the carousel to the child at the given Index.
        @see RotateToObject()
        @param rkValue The CGUIValue containing the new value
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        // Rotate to child object with index given by value
        RotateToObject( GetChild( rkValue.ToInt()) );
        CGUIObject::SetValue( rkValue);
        return true; 
    }

    /// when sizes of carousel get changed the children need to be re-arranged
    virtual void SetWidth(const eC_Value &vX);
    virtual void SetHeight(const eC_Value &vX);

    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /// Overloaded Add/Remove methods which are required to store child object's original sizes (for zooming)
    eC_Bool AddObject(CGUIObject* pObject);
    void RemoveObject(CGUIObject* pObject);

    /** Sets the radius of the carousel
        The carousel's radius is set to half of the carousel's width per default during construction.
        @param vRadius Radius in pixels */
    void SetRadius( const eC_Value vRadius) { m_vRadius = vRadius; ArrangeChildren(); }

    /// Returns the carousel's current radius
    eC_Value GetRadius() const
    {
        return m_vRadius;
    }

    /// Returns the carousel's current tilt-angle
    eC_Value GetTiltAngle() const
    {
        return m_vTiltAngle;
    }

    /// Rotate carousel entries counter-clockwise
    eC_Bool Decrease();

    /// Rotate carousel entries clockwise
    eC_Bool Increase();

    /** Selects the given object and smoothly rotates the carousel, so that the given object is at the front
        @param pObject  Pointer to child object of carousel which shall be rotated to the front */
    void RotateToObject( CGUIObject* pObject );

    /** Selects the given object and Immediately (without animation) rotates the carousel, so that the given object is at the front
        @param pObject  Pointer to child object of carousel which shall be rotated to the front */
    void JumpToObject( CGUIObject* pObject );

    /** Get child object which is currently in front.
        @return Pointer to child object which is currently in front */
    CGUIObject* GetSelectedObject() { return GetChild(m_iCarouselItemIndex); }

    /** Get index of child-object which is currently in front.
        @return index of child object which is currently in front */
    eC_Int GetSelectedIndex() const
    {
        return m_iCarouselItemIndex;
    }

    /** Get original size of object at given index. Since the width and height of the
        child-objects within the carousel get recalculated according to perspective.
        @param uiIndex index of object
        @param vWidth width of object
        @param vHeight height of object
    */
    void GetOriginalSize(const eC_UInt uiIndex, eC_Value& vWidth, eC_Value& vHeight)
    {
        vWidth = eC_FromInt(0);
        vHeight = eC_FromInt(0);

        if (uiIndex < GetNumberOfChildren())
        {
            sSizeStorage kSize = (*m_OriginalSizes.GetAt(uiIndex));
            vWidth = kSize.kRect.GetWidth();
            vHeight = kSize.kRect.GetHeight();
        }
    }

    /** Used to operate carousel via 4-way-navigation keys, if desired.
        @param eKeyID The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                false Else.
    */
    eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    /// Enables control via mouse
    eC_Bool DoDrag( const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);
    eC_Bool DoDragEnd( const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Sets the angle by which the carousel is tilted. (By default along the X-Axis. If SetVertical() has been set to True the carousel
        is tilted along the Y-Axis.
        @param vTiltAngle Tilting angle in Degrees (0-180) with 0 being NO tilt, and 180 degrees being the maximum. */
    void SetTiltAngle( const eC_Value vTiltAngle);

    /** Enables / Disables flow mode for this carousel.
        @param bEnable TRUE if flow mode shall be enabled, FALSE to disable it */
    void EnableFlowMode( const eC_Bool bEnable) { m_bFlowMode = bEnable; }

    /** Sets number of "virtual" entries within carousel in flow mode.
        Please refer to CGUICarousel's class documentation and examples for further information.
        @param uiNOFEntries Number of virtual carousel entries. */
    void SetNumberOfFlowModeEntries( const eC_UInt uiNOFEntries) { m_uiNOFEntries = uiNOFEntries; }

    /** Specifies the strength of perspective distortion which is applied to objects within tilted carousels.
        Objects in the background will appear smaller than those in the foreground.
        Objects in untilted carousels, and objects that reside on the X-Axis of tilted carousels, will be displayed
        in their original size.
        Sensible values are typically in the range of >0 to 1, where 0 means no perspective effect at all, and larger
        values result in stronger zooming / shrinking. Note that Z-Ordering will not work if perspective-Factor is 0.
        @param vPerspectiveFactor Perspective zooming factor (sensible range of values is typically >0 to 1) */
    void SetPerspectiveFactor( const eC_Value vPerspectiveFactor) { m_vPerspectiveFactor = vPerspectiveFactor; }

    /** Switches between vertical and horizontal arrangement of the carousel.
        @param bVertical True arranges the carousel vertically, False arranges it horizontally.
    */
    void SetVertical( const eC_Bool bVertical) { m_bVertical = bVertical; }

    /** Inverts keyboard input direction for all carousels.
        @param bInvert True Inverts keyboard input direction for all carousels.
    */
    static void SetInvertInput( const eC_Bool bInvert) { ms_bInvertInput = bInvert; }

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Helper structure to store original sizes (before any zooming) of child objects
    struct sSizeStorage {
        /// Pointer to object (key value)
        CGUIObject* pObj;
        /// rectangle with stored original size
        CGUIRect     kRect;
    };

    /// Helper structure to store carousel items and their corresponding scaling factors for zordering
    struct ZOrderEntry {
        /// Pointer to object (key value)
        CGUIObject* pObject;
        /// Scaling factor (Larger values represent objects in the foreground)
        eC_Value    vScaleFactor;
    };

    /** Sets the selected object without actually rotating the object.
        @param pObject  Pointer to child object of carousel which shall be rotated to the front */
    void SetSelectedObject( CGUIObject* pObject);

    /** Calculates the number of degrees per child object.
        @returns 360/NumberOfChildren in normal-mode, and 360/NOFEntries in flowmode
    */
    eC_Value GetDegreesPerChild();

    /// Arranges the child objects in an elliptic shape
    virtual void ArrangeChildren();

    /// Ensures that only the selected child object is focusable
    void EnforceFocusBehaviour();

    /// List of original object sizes (one entry per child object)
    eC_TListDoubleLinked<sSizeStorage> m_OriginalSizes;
    /// Current rotation angle (in degrees) of carousel
    eC_Value    m_vRotationAngle;
    /// Desired target angle (in degrees). This varies from the current rotation angle during rotation
    eC_Value    m_vTargetAngle;
    /// Value by which the carousel is tilted around the X-Axis (internally stored in range 0-1)
    eC_Value    m_vTiltAngle;
    /// Index of currently focused child object within child list
    eC_Int      m_iCarouselItemIndex;
    /// Flag indicating whether left/right cursor keys should be used to rotate the carousel as well
    /// This might interfere with standard focusing behaviour.
    eC_Bool     m_bRotateByCursorKeys;
    /// Pixel radius of carousel
    eC_Value    m_vRadius;
    /// Internal flag indicating whether the carousel is currently being dragged
    eC_Bool     m_bDragging;
    /** The perspectivefactor defines the perspective scaling which is applied to carousel children.
        Larger factors result in a stronger perspective effect, while values close to 0 will result in an almost
        orthogonal projection. */
    eC_Value    m_vPerspectiveFactor;
    /** Flowmode enables the carousel to contain more children than it physically could in reality.
        This enables user to create carousels with a virtually unlimited number of entries in them. */
    eC_Bool     m_bFlowMode;
    /** Only relevant in FlowMode. This defines a virtual number of entries in the carousel, used e.g. for calculating
        the spacing between them. */
    eC_UInt     m_uiNOFEntries;
    /// Indicates if the current animation is "kinetic" as the result of a preceeding drag event.
    eC_Bool     m_bKineticAnimation;
    /// Speed of the last drag event. Relevant for setting the speed of a subsequent kinetic animation.
    eC_Value    m_vDragSpeed;
    /// Indicates if the carousel is arranged horizontally or vertically
    eC_Bool     m_bVertical;
    /// Inverts the direction of keyboard input for ALL carousels
    static eC_Bool ms_bInvertInput;

};
#endif
