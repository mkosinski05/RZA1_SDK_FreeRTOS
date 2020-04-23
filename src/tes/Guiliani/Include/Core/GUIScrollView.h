/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUISCROLLVIEW__H_
#define __GUISCROLLVIEW__H_

#include "GUICompositeObject.h"
class CGUIScrollBar;

/// Container object which can be scrolled if required.
// @guiliani_doxygen toplevel_control Scroll View
/**
    <table border="0">
        <tr>
            <td width="200">@image html scroll_view.png</td>
            <td>
                The "scroll view" serves as a container which gives a scrollable view onto its embedded controls. Its content is typically larger than the scroll view itself, so that the user can scroll through its content (Class: CGUIScrollView).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    This class serves as a container object which gives a scrollable view onto another object, which is typically
    larger than the CGUIScrollView object itself.

    The ScrollView consists of four objects:
    - A vertical Scrollbar, positioned on the right
    - A horizontal Scrollbar, positioned on the bottom
    - A Clipping-region, which clips the view onto the container object
    - A composite object container (child of the clipper), which contains the objects which will actually be scrolled

    Objects which are added to the ScrollView will actually be added to the internal container-object. The container
    will automatically resize itself if new children are added to it, so that all contained objects are visible.

    Example:
@code

    // Create a scroll view.
    CGUIScrollView* pkScrollView = new CGUIScrollView(this,
                                            eC_FromInt(0), eC_FromInt(0),
                                            eC_FromInt(100), eC_FromInt(100),
                                            OBJ_SCROLLVIEW);

    // Add a new child Object.
    CGUIObject *pkChildObject = new CGUIObject(this,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(100),
                                        OBJ_OBJECT1);
    pkScrollView->AddObject(pkChildObject);

    // Sets the scrollbar policy for the horizontal scrollbar.
    pkScrollView->SetHorizontalScrollbarPolicy(CGUIScrollView::AUTOMATIC);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIScrollView
    : public CGUICompositeObject
{
public:

    /// Scrollbar policies. These specify under which circumstances a scrollbar is visible.
    enum ScrollBarPolicy {
        AUTOMATIC,          ///< Scrollbar will automatically be displayed as required (default)
        ALWAYS_VISIBLE,     ///< Scrollbar will always be visible
        NEVER_VISIBLE       ///< Scrollbar will never be visible
    };

    /** Constructor
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the object.
        @param vHeight Height of the object.
        @param eID Object Identifier of this object
    */
    CGUIScrollView(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor taking a rectangle as input.
        @param pParent Pointer to the designated parent object.
        @param kRect Rectangle describing position and size
        @param eID Object Identifier of this object
    */
    CGUIScrollView(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Copy constructor. This will copy the ScrollView's attributes. (Note that this will NOT copy any child objects)
    CGUIScrollView(const CGUIScrollView& rkSource);

    /// Assignment operator. This will copy the ScrollView's attributes. (Note that this will NOT copy any child objects)
    CGUIScrollView& operator=(const CGUIScrollView& rkSource);

    /** Sets the scrollbar policy for the vertical scrollbar.
        @param ePolicy Policy for the vertical scrollbar (@see ScrollBarPolicy)
    */
    virtual void SetVerticalScrollbarPolicy( const ScrollBarPolicy ePolicy)
    {
        m_eVerticalScrollBarPolicy = ePolicy;
    }

    /** Sets the scrollbar policy for the horizontal scrollbar.
        @param ePolicy Policy for the horizontal scrollbar (@see ScrollBarPolicy)
    */
    virtual void SetHorizontalScrollbarPolicy( const ScrollBarPolicy ePolicy)
    {
        m_eHorizontalScrollBarPolicy = ePolicy;
    }

    /** Returns a pointer to the vertical scrollbar.
        Use this in case you wish to change the scrollbar's graphics. Note that the
        ScrollView will automatically override some of the ScrollBar's attributes, like
        its position / size though.
        @return Pointer to the vertical scrollbar */
    CGUIScrollBar* GetVerticalScrollbar() { return m_pVerScrollBar; }

    /** Returns a pointer to the horizontal scrollbar.
        @see GetVerticalScrollbar
        @return Pointer to the horizontal scrollbar */
    CGUIScrollBar* GetHorizontalScrollbar() { return m_pHorScrollBar; }

    /** It is not allowed to add any objects directly to the ScrollableObject.
        Instead all added objects will automatically be added to the container object.
        @param pObject Pointer to object which will be added to the container object's childlist.
        @return True if the object was added, otherwise False. */
    virtual eC_Bool AddObject( CGUIObject* pObject);

    /// Removes an object from the scrolled container.
    /** Objects are removed from the internal container of scrolled objects.
        @param pObject Object to be removed.
        @see AddObject */
    virtual void RemoveObject(CGUIObject *pObject);

    /** This implementation of the SetWidth method calls the base class' standard
        implementation but also assures that attached ScrollBars are adjusted with regard to the new width.
        @param vWidth The new width
    */
    virtual void SetWidth(const eC_Value &vWidth);

    /** This implementation of the SetHeight method calls the base class' standard
        implementation but also assures that attached ScrollBars are adjusted with regard to the new height.
        @param vHeight The new height
    */
    virtual void SetHeight(const eC_Value &vHeight);

    /** This updates the visibility and sizes of the attached ScrollBars.
        Typically, this needs to be called whenever the ScrollView's content has changed. */
    virtual void AdjustScrollBars();
    
    /** This ensures that children of the ScrollView will automatically be scrolled into the visible area if they
        receive the focus. It will automatically be called by AdjustScrollBars(). You may reimplement this method
        in derived classes if you wish to avoid this default behaviour. */
    virtual void ScrollToFocussedObject();

    /** Scroll to the given object, that is part of scroll view.
        @param pkObject The object to scroll to (make visible as good as possible).
     */
    virtual void ScrollToObject(CGUIObject* pkObject);

    /** This ensures that a child object of this container will always be scrolled into the visible area
        if it receives the focus. */
    virtual void GetFocus()
    {
        AdjustScrollBars();
        CGUICompositeObject::GetFocus();
    }

    /** Movement inertia of scrolling animation. Higher inertia values will result in slower animation.
        @param vInertia Inertia value. Must be greater than one, and the greater the value, the slower the scrolling.*/
    virtual void SetScrollingInertia( const eC_Value vInertia);

    /** Defines the viewport into which the focused object will be scrolled. This is useful if you wish to
        commence scrolling before the focus actually leaves the visible area.
        @param vX Border in horizontal direction in pixels by which the viewport is smaller than the visible area.
        @param vY Border in vertical direction in pixels by which the viewport is smaller than the visible area.*/
    virtual void SetScrollingViewport( const eC_Value vX, const eC_Value vY);

    /** Implementation of scrolling animation. This will scroll the currently focused object 
        (if it is a descendant of the scrollview) into view.
        @param vTimes the correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Retrieves the pointer to the ScrollView's container object. This is the composite object which actually
        contains the scrollable content of the ScrollView.
        @return pointer to the scrollview's container object*/
    virtual CGUICompositeObject* GetContainer() const;

    /** Retrieves the pointer to the ScrollView's clipper object. This is the composite object against which 
        all content is clipped. It is thus also the parent of the Container composite.
        @return pointer to the scrollview's clipper object*/
    virtual CGUICompositeObject* GetClipper() const;

    /// Stores the scroll positions (values of both scroll bars)
    virtual void SaveScrollPosition();

    /** Restores the positions of both scroll bars to the ones that were saved
        with the last call to SaveScrollPosition().
      */
    virtual void RestoreScrollPosition();

    /// Event handling slots for scrolling
    virtual eC_Bool DoScrollUp(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoScrollDown(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoScrollLeft(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoScrollRight(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Specifies whether the vertical ScrollBar will be positioned at the right or left side of the Scrollview.
        @param bIsAtRight true positions the scrollbar at the right side, false positions the scrollbar at the left side
    */
    virtual void SetVerticalScrollBarAtRight(const eC_Bool bIsAtRight);

    /** Specifies whether the horizontal ScrollBar will be positioned at the top or bottom side of the Scrollview.
        @param bIsAtBottom true positions the scrollbar at the bottom, false positions the scrollbar at the top
    */
    virtual void SetHorizontalScrollBarAtBottom(const eC_Bool bIsAtBottom);

    /// @return True if vertical Scrollbar is at the right of the ScrollView (and there IS a scrollbar), False otherwise
    virtual eC_Bool IsVerticalScrollBarAtRight() const;
    
    /// @return True if horizontal Scrollbar is at the bottom of the ScrollView (and there IS a scrollbar), False otherwise
    virtual eC_Bool IsHorizontalScrollBarAtBottom() const;

    /** Enable/Disable automatic layout of the Clipper and ScrollBars. 
        By default, the Clipper and ScrollBars will automatically adapt their size/position when the size of the scrollview changes, their visibility changes
        or their horizontal/vertical layout changes.
        @param bAutoAdjustScrollViewLayout The new value
    */
    void SetAutoAdjustScrollViewLayout(const eC_Bool bAutoAdjustScrollViewLayout) { m_bAutoAdjustScrollViewLayout=bAutoAdjustScrollViewLayout; }

    /** Enable/Disable automatic resize of the container to be large enough to encapsulate all child objects. 
        By default, the container will automatically adapt its size e.g. when new children are being added to it.
        @param bAutoAdjustContainerSize The new value
    */
    void SetAutoAdjustContainerSize(const eC_Bool bAutoAdjustContainerSize) { m_bAutoAdjustContainerSize = bAutoAdjustContainerSize; }

    /// Called when the Scrollview wishes to start scrolling. Overriding methods should call the base class implementation. 
    virtual void StartScrollingAnimation();

    /// Called when the scrolling animation is over. Overriding methods should call the base class implementation. 
    virtual void StopScrollingAnimation();

#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor. Only to be called by factory.
        No user code should call this constructor, not even in
        streaming mode (that is, when GUILIANI_STREAM_GUI is defined)!
        @see CGUISlider() */
    CGUIScrollView();

    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// General initialization
    void Init();

    /** Helper method for AdjustScrollBars. Called after setting the scroll bar
        visibility. This method adjusts the size of the clipping container so
        that it does not overlap the scroll bars.
      */
    virtual void AdjustClipperSize();

    /** Automatically resizes the container, so that it fully encapsulates all its children */
    virtual void AdjustContainerSize();

    /** Composite object that clips the contained (scrolled) items. Its only
        child is m_pContainer.
        @note This object is valid to be NULL.*/
    CGUICompositeObject*    m_pClipper;
    /** Horizontal scrollbar
        @note This object is valid to be NULL.*/
    CGUIScrollBar*          m_pHorScrollBar;
    /** vertical scrollbar
        @note This object is valid to be NULL.
    */
    CGUIScrollBar*          m_pVerScrollBar;
    /// Scrollbar policy defining whether a vertical scrollbar is visible or not
    ScrollBarPolicy         m_eVerticalScrollBarPolicy;
    /// Scrollbar policy defining whether a horizontal scrollbar is visible or not
    ScrollBarPolicy         m_eHorizontalScrollBarPolicy;
    /// Inertia value for soft scrolling. Legal values are positive and greater than 1.
    eC_Value                m_vScrollingInertia;
    /// Horizontal (in X direction) gap between visible area and the area outside of which scrolling will occur
    eC_Value                m_vViewportXBorder;
    /// Vertical (in Y direction) gap between visible area and the area outside of which scrolling will occur
    eC_Value                m_vViewportYBorder;
    /// Stored position of the horizontal scroll bar.
    eC_Int                  m_iHorScrollPos;
    /// Stored position of the vertical scroll bar.
    eC_Int                  m_iVerScrollPos;
    /// Position of vertical scrollbar (true=right, false=left)
    eC_Bool                 m_bIsVerticalScrollBarAtRight;
    /// Position of horizontal scrollbar (true=bottom, false=top)
    eC_Bool                 m_bIsHorizontalScrollBarAtBottom;
    /** Specifies if the ScrollBars and the Clipper shall automatically adapt their size/position when the dimension of the ScrollView
        or the visibility of the ScrollBars changes */
    eC_Bool                 m_bAutoAdjustScrollViewLayout;
    /** Specifies if the Container shall automatically be resized to be large enough to encapsulate all children */
    eC_Bool                 m_bAutoAdjustContainerSize;
};
#endif
