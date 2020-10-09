/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICOMPOSITEOBJECT__H_
#define GUICOMPOSITEOBJECT__H_

#include "GUIObject.h"
#include "eC_TList_doubleLinked.h"

/// Type used for all kinds of GUI object lists.
typedef eC_TListDoubleLinked<CGUIObject*> ObjectPtrList;

/// This is the Guiliani base class for all composite objects.

// @guiliani_doxygen toplevel_control Composite Object
/**
    <table border="0">
        <tr>
            <td width="200">@image html composite_object.png</td>
            <td>
                The "composite object" serves as a container, which can contain and manage several other controls. It provides an own coordinate-system and clipping area to its children. The "composite object" control can be used to create e.g. pop-ups or windows (Class: CGUICompositeObject).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    CompositeObjects serve as <i>containers</i>, which can contain and manage several child objects.
    A CGUICompositeObject inherits all basic features from CGUIObject and adds child management.
    Complex objects, which consist of several sub-objects such as ScrollViews, RadioButton-Groups or Menus,
    are typically implemented as CompositeObjects.

    The CGUICompositeObject itself defines...
    <ul>
        <li>a local coordinate system for its children.</li>
        <li>a bounding rectangle, which clips all graphic operations of the children.</li>
    </ul>

    The fact that a CompositeObject defines a local coordinate system for its child objects means that all children
    will be positioned <b>relative to their parent composite object</b>. You can think of having a new coordinate system
    with (0,0) at the upper left edge of the CompositeObject. The coordinates that you pass to the constructor of the
    child are the offsets within the composite object's bounding rectangle. That means these are coordinates relative to
    the parent and <b>not absolute coordinates of the screen</b>.
    Please refer to @ref page_coordinate_systems for a comprehensive explanation of Guiliani's coordinate systems.

    The clipping area spanned by the CompositeObject defines the area on the screen in which the composite object itself,
    and all its children (and their children) are allowed to draw. <b>Everything outside of the area of the CompositeObject
    will be clipped away and therefore not be visible.</b> It is perfectly valid though, to place child objects outside of
    the parent's region (even at negative coordinates). A standard use-case for this is scrolling, where you will intentionally
    place objects at positions outside the visible area, and then gradually scroll them into view.

    A note on a CGUICompositeObject's default state upon creation:
    By default CGUICompositeObjects will be <b>disabled</b> and <b>not focusable</b>. This means they will not
    receive the focus and will not react to any events (e.g. will not react to clicks). Please use the SetDisabled() and
    SetFocussable() methods if you desire a different behaviour.
    Be aware that they <b>will</b> receive events triggered on child-objects, though. E.g. if a child object receives a DoClick()
    event this will also be forwarded to a disabled CGUICompositeObject - following the standard chain-of-responsibility.

    @ingroup GUILIANI_CONTROLS
*/
class CGUICompositeObject
    : public CGUIObject
{
public:
    friend class CGfxWrap;
    friend class CGUIObject;

    /** CGUICompositeObject standard constructor.
        @see CGUIObject().
    */
    CGUICompositeObject();

    /** CGUICompositeObject constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the composite object
        @param vHeight Height of the composite object
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUICompositeObject(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUICompositeObject constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle relative to parent.
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUICompositeObject(CGUICompositeObject *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUICompositeObject();

    /** Copy-constructor of CGUICompositeObject.
        The child-objects-list and the draw-order-list will not be copied, because you don't know, which
        object pointers are within the lists. So there is no possibility to make real copies (=deep copies) of the objects
        within the list.
        @param kSource Source object to be copied.
    */
    CGUICompositeObject(const CGUICompositeObject& kSource);

    /** Operator = method of CGUICompositeObject.
        The old child-objects entries in the list will be deleted. The child-objects-list and the draw-order-list
        will be cleared. The source child-objects-list and the draw-order-list will not be copied to the lists of this object,
        because you don't know, which object pointers are within the lists. So there is no possibility to make deep copies of
        the objects within the list.
        @param kSource Source object to be copied.
        @return Copied object (without draw-order- and child-objects-list).
    */
    CGUICompositeObject& operator=(const CGUICompositeObject& kSource);

#ifdef GUILIANI_STREAM_GUI
    /** Reads composite object attributes and children from stream.
        @copydoc CGUIStreamableObject::ReadFromStream
        @throws CGUIStreamingControlException When catching a
                CGUIStreamingControlException from a child, this method adds
                the object from the exception, resets the object pointer to
                'this', then re-throws.
    */
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    /** Overridden SetWidth method.
        A composite object will call DoLayout() on itself  when its width/height is changed
        if its associated layouter is independent of the parent's size. Afterwards in any case
        its direct children's DoLayout() of their layouters are called.
        @param vX The new width
    */
    virtual void SetWidth(const eC_Value &vX);

    /** Overridden SetHeight method.
        A composite object will call DoLayout() on itself  when its width/height is changed
        if its associated layouter is independent of the parent's size. Afterwards in any case
        its direct children's DoLayout() of their layouters are called.
        @param vY The new height
    */
    virtual void SetHeight(const eC_Value &vY);

    /** Add a new Object to the list of child objects.
        This will automatically set the child's parent-pointer to this object.
        @param pObject The object to be added
        @return False if the object is already a child of this control or pObject
                is NULL, otherwise true. */
    virtual eC_Bool AddObject(CGUIObject *pObject);

    /** Removes an object from the list of child objects.
        This will not delete the object! If you wish to physically delete an
        object, you should use the C++ delete operator. The destructor of the
        object will then remove the object from the child list by calling this
        method.
        @param pObject The object to be removed
    */
    virtual void RemoveObject(CGUIObject *pObject);

    /** Retrieves a pointer to the object, which has the topmost position at the given coordinates.
        'Topmost' refers to the control last drawn (z order).
        If the object is invisible, or its 'ClickThrough' attribute is set to TRUE, it will not be
        found by this operation. In this case the search is continued among any other existing
        Objects at the given position.
        @param vAbsX Absolute X coordinate for which the topmost object shall be returned
        @param vAbsY Absolute Y coordinate for which the topmost object shall be returned
        @return Pointer to the object at the position. Can be NULL if no object
                was found at that position or if there are only invisible
                objects at the position.
    */
    virtual CGUIObject* FindObject(const eC_Value &vAbsX, const eC_Value &vAbsY);

   /** Set focus to the next focusable child object after the currently focused one.
        Usually this function will be called on the currently focused object with no
        arguments.

        Attention: It is forbidden to manipulate the creation tree, especially to delete the
        object referenced by ms_pFirstRequestedObj or change the cyclic or StopFocus flags
        of the object referenced by ms_pFocusTerminatingObj when a FocusNext search procedure
        is running so don't manipulate the creation tree in RequestFocus() or a derived
        FocusNext(), FocusNextDown() or similar (in any function called by FocusNext()).

        Algorithm:
        Next FocusNext() in the recursion is called on this or parent object.
        FocusNextDown() is called on this or child object.
        We decide where to move forwards (up/down/sideways) (up shall be in
        direction to the root) by the following information:
        Subtree (RefObj and its subtree) has been processed?
        Which child is currently referenced?
        We always use the following prioritization to determine the next step:
        -# down on reference object (initially first child, plus indicating
            subtree is not yet considered)
        -# sideways to next child (sibling of reference object, plus indicating
            subtree is not yet considered)
        -# up to parent (using this object as next reference object, plus
            indicating subtree has already been considered)

        @param pRefObj usually is a child of this object which indicates the current position in the
            child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under pRefObj has
            already been asked to take focus, or if not.
        @return True if focus was obtained, False otherwise
    */
    virtual eC_Bool FocusNext(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    /** Set focus to the previous focusable child object before the currently focused one,
        also please refer to FocusNext().
        @see FocusNext

        Attention: It is forbidden to manipulate the creation tree, especially to delete the
        object referenced by ms_pFirstRequestedObj or change the cyclic or StopFocus flags
        of the object referenced by ms_pFocusTerminatingObj when a FocusPrevious search procedure
        is running, so don't manipulate the creation tree in RequestFocus() or a derived
        FocusPrevious(), FocusPreviousDown() or similar (in any function called by FocusPrevious()).

        Next FocusPrevious() is called on this or parent object.
        FocusPreviousDown() is called on this or child object.
        We decide where to move forwards (up/down/sideways) (up shall be in
        direction to the root) by the following information:
        Subtree (RefObj and its subtree) has been processed?
        Which child is currently referenced?
        We always use the following prioritization to determine the next step:
        -# down on reference object (initially last child, plus indicating
            subtree is not yet considered)
        -# sideways to next child (sibling of reference object, plus indicating
            subtree is not yet considered)
        -# up to parent (using this object as next reference object, plus
            indicating subtree has already been considered)

        @param pRefObj usually is a child of this object which indicates the current position in the
            child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under pRefObj and the
            subtrees of pRefObj's elder (usually in creation time) sibling have already been asked to take
            focus, or if not.
        @return True if focus was obtained, False otherwise
    */
    virtual eC_Bool FocusPrevious(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    eC_Bool FocusAny();

    /** This slot may be implemented by derived classes to add visualization code which is executed after any
        child object have been drawn.
        Unlike DoDraw, which is being called BEFORE any children are being drawn, this method will be automatically
        called by the framework AFTER all children have been drawn.
        @return True if anything was drawn, False otherwise
    */
    virtual eC_Bool DoPostDraw();

    /** Moves the given Object to the top of the drawing order, thus making it appear in the
        foreground after the next redraw.
        @param pObject The object to move to the top
    */
    void DrawOnTop(CGUIObject* pObject);

    /** Moves the given Object's position within the drawing order, so that it gets drawn
        on top of the object pointed to by pDrawOnTopOf. Note that both objects must be children
        of this CompositeObject.<p>
        Special case: If you want pObject to be drawn before all other children, that is only atop
        of its parent, but beneath all other children, you can do so by supplying its parent-pointer
        as pDrawOnTopOf.
        @param pObject Pointer to object whose drawing order is to be changed
        @param pDrawOnTopOf Pointer to the object above which pObject will be drawn from now on */
    void DrawOnTopOf( CGUIObject*const pObject, CGUIObject*const pDrawOnTopOf);

    /** Searches for an object by its ID.

        There is another implementation of the same method with a slightly
        different signature, GetObjectByID(const ObjectHandle_t&) const.
        The compiler will choose the appropriate method depending on what
        return type the caller expects.
        @param eID ID of the object for which to search.
        @return Returns pointer to the object if it was found, otherwise NULL.
                Searching for NO_HANDLE will return NULL.*/
    inline CGUIObject* GetObjectByID(const ObjectHandle_t &eID)
    {
        // Call the const-GetObjectByID function, which is the real
        // implementation.
        // Casting away constness is valid here, because all
        // CGUIObjects in Guiliani by rule shall be declared non-const
        // at construction time.
        return const_cast<CGUIObject*>(static_cast<const CGUICompositeObject* const>(this)->GetObjectByID(eID));
    }

    /** Searches for an object by its ID.

        There is another implementation of the same method with a slightly
        different signature, GetObjectByID().
        The compiler will choose the appropriate method depending on what
        return type the caller expects.
        @param eID ID of the object for which to search.
        @return Returns pointer to the object if it was found, otherwise NULL.
                Searching for NO_HANDLE will return NULL.*/
    const CGUIObject* GetObjectByID(const ObjectHandle_t &eID) const;

    /** Tries to find an object with a specific ID. The object is searched in a
        specific parent composite object. If the object could not be found, an
        exception is thrown.

        There is another implementation of the same method with a slightly
        different signature, GetAndCheckObjectByID(ObjectHandle_t, const eC_String&).
        The compiler will choose the appropriate method depending on what
        return type the caller expects.
        @param eObjID The ID of the object to be searched.
        @param kObjectName The name of the object. Will be shown in the message
               that is generated when throwing an exception.
        @return Returns pointer to the object if it was found, otherwise NULL
        @throw CGUIException if no object with the given ID was found.
    */
    const CGUIObject* GetAndCheckObjectByID(ObjectHandle_t eObjID, const eC_String& kObjectName) const;

    /** Tries to find an object with a specific ID. The object is searched in a
        specific parent composite object. If the object could not be found, an
        exception is thrown.

        There is another implementation of the same method with a slightly
        different signature, GetAndCheckObjectByID(ObjectHandle_t, const eC_String&) const.
        The compiler will choose the appropriate method depending on what
        return type the caller expects.
        @param eObjID The ID of the object to be searched.
        @param kObjectName The name of the object. Will be shown in the message
               that is generated when throwing an exception.
        @return Returns pointer to the object if it was found, otherwise NULL
        @throw CGUIException if no object with the given ID was found.
    */
    inline CGUIObject* GetAndCheckObjectByID(ObjectHandle_t eObjID, const eC_String& kObjectName)
    {
        // Call the const-GetObjectByID function, which is the real implementation.
        return const_cast<CGUIObject*>(static_cast<const CGUICompositeObject* const>(this)->GetAndCheckObjectByID(eObjID, kObjectName));
    }

    /** Decide whether the returned GUIObject is CompositeObject or not.
        Please DO NOT override this method anymore in the derived class, because
        it is only used in the CGUIObject and CGUICompositeObject to indicate whether an
        object is a composite object or not.
        @return Returns true.
    */
    virtual eC_Bool IsCompositeObject() const {return true;}

    /** Set only this composite object disabled or enabled. This method simply
        calls the base implementation CGUIObject::SetDisabled().
        If you also want to set the children's enabled state, use
        SetDisabled(const eC_Bool&, const eC_Bool&) instead.
        @param bDisabled The new enabled state of this composite object.
        @see SetDisabled(const eC_Bool&, const eC_Bool&)
      */
    virtual void SetDisabled(const eC_Bool& bDisabled) { CGUIObject::SetDisabled(bDisabled); }

    /** Set this control disable or not. Whether the children will be set or not depends on parameter bChildrenDisabled
        @param  bDisabled    Disabled, when true. otherwise, false
        @param  bChildrenDisabled State of the children.
      */
    virtual void SetDisabled(const eC_Bool &bDisabled, const eC_Bool &bChildrenDisabled);

    /** Set only this composite object invisible or visible. This method simply
        calls the base implementation CGUIObject::SetInvisible().
        If you also want to set the children's visibility, use
        SetInvisible(const eC_Bool&, const eC_Bool&) instead.
        @param bInvisible The new visibility state of this composite object.
        @see SetInvisible(const eC_Bool&, const eC_Bool&)
      */
    virtual void SetInvisible(const eC_Bool& bInvisible) { CGUIObject::SetInvisible(bInvisible); }

    /** Set this composite object and its children invisible or visible.
        @param  bInvisible   If true, this composite object will be invisible,
                if false, the composite object will be visible.
        @param  bChildrenInvisible If true, all children (and their children)
                will be invisible. If false, the children will be visible.
      */
    virtual void SetInvisible(const eC_Bool &bInvisible, const eC_Bool &bChildrenInvisible);

    /** Set only this composite object grayed-out. This method simply
        calls the base implementation CGUIObject::SetGrayedOut().
        If you also want to set the children's state, use
        SetGrayedOut(const eC_Bool&, const eC_Bool&) instead.
        @param bGrayedOut The new grayed-out state of this composite object.
        @see SetGrayedOut(const eC_Bool&, const eC_Bool&)
      */
    virtual void SetGrayedOut(const eC_Bool &bGrayedOut) { CGUIObject::SetGrayedOut(bGrayedOut); }

    /** Set this control and its children grayedout or not.
        @param  bGrayedOut     Grayedout, when true. otherwise, false
        @param  bChildrenGrayedOut Gray out the children or 'un-gray-out' them.
      */
    virtual void SetGrayedOut( const eC_Bool &bGrayedOut, const eC_Bool &bChildrenGrayedOut);

    /** Set only this composite object focusable or not. This method simply
        calls the base implementation CGUIObject::SetFocussable().
        If you also want to set the children's focusability, use
        SetFocussable(const eC_Bool&, const eC_Bool&) instead.
        @param bFocussable The new focusable state of this composite object.
        @see SetFocussable(const eC_Bool&, const eC_Bool&)
      */
    virtual void SetFocussable(const eC_Bool &bFocussable) { CGUIObject::SetFocussable(bFocussable); }

    /** Set this composite object and its children focusable or not.
        @param bFocussable Set this composite object to be focusable if true.
        @param bChildrenFocussable Set all children of this composite object to
               be focusable.
      */
    virtual void SetFocussable(const eC_Bool &bFocussable, const eC_Bool &bChildrenFocussable);

    /** Set only this composite object clickthrough. This method simply
        calls the base implementation CGUIObject::SetClickThrough().
        If you also want to set the children's clickthrough-state, use
        SetClickThrough(const eC_Bool&, const eC_Bool&) instead.
        @param bClickThrough The new clickthrough state of this composite object.
        @see SetClickThrough(const eC_Bool&, const eC_Bool&)
      */
    virtual void SetClickThrough(const eC_Bool &bClickThrough) { CGUIObject::SetClickThrough(bClickThrough); }

    /** Set this composite object and its children clickthrough or not.
        @param bClickThrough Set this composite object to clickthrough if true.
        @param bChildrenClickThrough Set all children of this composite object to cklickthrough.
      */
    virtual void SetClickThrough(const eC_Bool &bClickThrough, const eC_Bool &bChildrenClickThrough);

    /** Returns a pointer to the specified child.

        There is another implementation of the same method with a slightly
        different signature, GetChild(const eC_UInt). The compiler will choose the
        appropriate method depending on what return type the caller expects.
        @param uIndex Index of the child, zero-based, that is, first child has index zero.
        @return Returns pointer to the child if it was found, otherwise NULL.
    */
    CGUIObject* GetChild(const eC_UInt uIndex) const; // callable on a const composite object

    /** Returns a pointer to the specified child.

        There is another implementation of the same method with a slightly
        different signature, GetChild(const eC_UInt) const. The compiler will choose the
        appropriate method depending on what return type the caller expects.
        @param uIndex Index of the child, zero-based, that is, first child has index zero.
        @return Returns pointer to the child if it was found, otherwise NULL.
    */
    inline CGUIObject* GetChild(const eC_UInt uIndex) // callable on a non-const composite object
    {
        // Call the const-GetObjectByID function, which is the real implementation.
        return const_cast<CGUIObject*>(static_cast<const CGUICompositeObject* const>(this)->GetChild(uIndex));
    }

    /**
        Returns the corresponding index to a pointer to a child.
        @param pObject Pointer to the child.
        @return Returns the index of the child if it was found, otherwise -1.
    */
    eC_Int GetIndexOfChild(const CGUIObject* pObject) const;

    /** Returns the number of children.
        @return Number of children in this composite object.
    */
    inline eC_UInt GetNumberOfChildren() const { return m_kChildObjectsList.GetQuantity (); }

    /** Removes and deletes all children from the composite object and empties the draw order list. */
    void RemoveAllChildren();

    /** Inserts a child at the given index within the child list.
        Inserting a child, which is already part of this object will move it to the given index.
        Supplying an illegal index will add the object at the end.
        @param cuiIndex The index of insertion.
        @param pkChild The child which has to be inserted.
        @return True if successful, False otherwise 
    */
    const eC_Bool InsertChildAt(const eC_UInt cuiIndex, CGUIObject* pkChild);

    /** Sets the FindNext()'s and FindPrevious()'s ability to try to focus the children
        in a cyclic manner, i.e. the first child follows the last and the last child preceeds
        the first.
        @param bCyclicFocus Boolean value indicating the new state. TRUE if the algorithm
                            may jump to the child at the other end of the list.
                            FALSE if the algorithm should stop at
                            the end of the child list, thus not jump up to the parent.*/
    inline void SetCyclicFocus(const eC_Bool &bCyclicFocus)
    {
        m_bCyclicFocus = bCyclicFocus;
    };

    /** Method to check whether this composite object can be cyclic focused.
        That is whether the focus restarts from the beginning when it passes the last object within
        the object's child list (and vice-versa) in case of two-way focusing.
        @return True, the object can be cyclic focused, False otherwise.
     */
    inline eC_Bool CanBeCyclicFocussed() const {return m_bCyclicFocus;}

    /**
        Checks if the given object is a child of the current object.
        Therefore, the whole child tree will be browsed, so that also not only direct children can be found.
        @param pkObject Pointer to the object that might be a child of the current object.
        @return True if the given object is a child (not only a direct one) of the current object or the object
            itself.
    */
    eC_Bool IsAncestorOf (const CGUIObject* pkObject) const;

    /** Retrieves a reference to this object's list of child objects.
        Note that the returned pointer is "const" to assure that no external object may
        change the list itself. You may use the returned pointer to access the objects
        therein, though.

        @code
        // Get Copy of child list
        const CGUICompositeObject* pConstComposite = pSomeCompositeObject;
        ObjectPtrList kList = pConstComposite->GetChildObjectsList();
        ObjectPtrList::Iterator kIter;
        // Iterate over list
        FOR_ALL_FORWARD( kIter, kList)
        {
            // do something with the child
        }
        @endcode

        @return const reference to this object's list of child objects
    */
    inline const ObjectPtrList& GetChildObjectsList() const
    {
        return m_kChildObjectsList;
    }

    /** Retrieves a reference to this object's draw-order list.
        This list defines the order in which child-objects will be drawn during
        the redraw cycle. The foremost object will be the last entry within the list.
        You are not allowed to make any changes to this list, since it is entirely controlled
        by the CGUICompositeObject base-class, but you may investigate the list's content, if desired.
        @return const reference to this object's draw-order list
    */
    inline const ObjectPtrList& GetDrawOrderList() const
    {
        return m_kDrawOrderList;
    }

    /// Invalidates all chidren, but not this parent.
    void InvalidateChildren();

    /** Call this method to refresh this object's and all of its children's layouts.
        Changing this object's width or height will automatically invoke this method.
        @note Layouters usually don't need to inform their object's
        parent's layouter as layouting steadily goes from parent to child.
        That is, parent's DoLayout() is called before its children's
        DoLayout()s are called.
        @param eMovedEdges This indicates in which direction the distance to the parent object's borders has changed
                           (Only relevant for layouters, which depend on their parent object's size, such as CGUILayouterAnchor)
    */
    virtual void RefreshLayout(CGUILayouter::eMovedEdges_t eMovedEdges) const;

    /**
    send object to back of container, so it is below all other children
    @param pkObject object to send to back
    @return true if successful false on error
    */
    eC_Bool SendToBack(const CGUIObject* pkObject);

    /**
    bring object to front of container, so it is above all other children
    @param pkObject object to bring to front
    @return true if successful false on error
    */
    eC_Bool BringToFront(const CGUIObject* pkObject);

    /**
    raise object one level, so it switches positions with the object directly above it
    @param pkObject object to raise one level
    @return true if successful false on error
    */
    eC_Bool RaiseOneLevel(const CGUIObject* pkObject);

    /**
    lower object one level, so it switchs positions with the object directly below it
    @param pkObject object to lower one level
    @return true if successful false on error
    */
    eC_Bool LowerOneLevel(const CGUIObject* pkObject);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML comment tag to be used before writing children to a stream.
    static const eC_Char XMLTAG_CHILDREN[];

    /// XML comment tag to be used after writing children to a stream.
    static const eC_Char XMLTAG_ENDCHILDREN[];
#endif

protected:
    /** Retrieves a pointer to this object's list of child objects.
        @return pointer to this object's list of child objects
    */
    inline ObjectPtrList& GetChildObjectsList()
    {
        return m_kChildObjectsList;
    }

    /** Helper function. Declared protected because you may want to add extra
        functionality to this function in a derived class and then call the base
        implementation afterwards.
        Called when the focus should be set to the next control that is to the given direction
        of the currently focused object. Please use CGUI::FourWayFocusNext() if you
        want to focus the next control that is in the specific direction of the
        currently focused object and not this function.
        @return If true, then the algorithm breaks immediately and will have
            no effect. Thus, custom controls may override this function slot
            in order to set the focus to a specific control and afterwards
            force the 4-way navigation algorithm to stop. The automatism will
            always return False.
        @param eFocusDirection The focus direction
        @see CGUI::FourWayFocusNext() */
    virtual eC_Bool FourWayNext(FourWayFocus_t eFocusDirection);

    /** This method visualizes this object and the objects it contains (child objects).
        Internally this does all necessary clipping and checking against the currently invalidated region.
        Note that erroneous overriding of this method may result in corrupted graphical output of the GUI.
        @param ClipRect The clipping rectangle
    */
    virtual void HandleDraw( const CGUIRect &ClipRect);

private:

    /// List of pointers to this object's child objects
    ObjectPtrList m_kChildObjectsList;

    /// This list specifies the order in which child objects are drawn
    ObjectPtrList m_kDrawOrderList;

    /** Please refer to description of base class function. */
    void UpdateInvalidation();

    /** This function should only be called from CGUICompositeObject::FocusNext().

        It tries to go down, if there are children. If not then it tries to jump
        up and request focus before jumping.

        @see FocusNext()
    */
    eC_Bool FocusNextDown();

    /** This function should only be called from CGUICompositeObject::FocusPrevious().

        It tries to go down, if there are children. If not then it tries to jump
        up and request focus before jumping.

        @see FocusPrevious()
    */
    eC_Bool FocusPreviousDown();

    /** CGUICompositeObject method which copies the attributes of this class.
        @param kSource Source object to be copied.
    */
    void CopyAttributes(const CGUICompositeObject& kSource);

    /** Helper function for constructors. */
    void Init();

    /** defines if the focus selection order is cyclic, that is if the first child object
        follows the last in the selection order, and vice versa.
        Attention: This attribute is not streamed, thus if you want to change the default
            then you have to use some other mechanism, like deriving a class or use
            a command to change the attribute of this object. */
    eC_Bool    m_bCyclicFocus;
};
#endif
