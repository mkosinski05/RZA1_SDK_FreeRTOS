/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_LAYOUT_HELPER__H_
#define GUI_LAYOUT_HELPER__H_

#include "eC_Types.h"
#include "eC_Math.h"
#include "eC_TList_doubleLinked.h"
#include "GUIObject.h"

class CGUICompositeObject;
class CGUIRect;

/// Retrieve pointer to layout helper instance.
#define GET_LAYOUT_HELPER           CGUILayoutHelper::GetInstance()

/// A collection of methods to help layout a GUI.
/** The following layout methods are implemented in this class:
    -# AlignChildren():      Align all children in a composite object left, right,
                              top, bottom or center to parent.
    -# AlignObject():        Align an object left, right, top, bottom or center to a reference object.
    -# AlignObjects():       Align a list of objects left, right, top, bottom or center to a reference object.
    -# AlignToGrid():        Align all children in a composite object to grid.
    -# AdaptWidth():         Change a single object or a list of objects to have same width as the reference object.
    -# AdaptHeight():        Change a single object or a list of objects to have same height as the reference object.
    -# ResizeObject():       Change a single object to have same width or height as the reference object.
    -# ResizeObjects():      Change a list of objects to have same width or height as the reference object.
    -# SpreadHorizontally(): Align children in a composite object so that horizontally each object has same
                              distance to the nearby object.
    -# SpreadVertically():   Align children in a composite object so that vertically each object has same
                              distance to the nearby object.
    -# AlignToAbsXPos():     Align an object to an absolute x position.
    -# AlignToAbsYPos():     Align an object to an absolute y position.
    @ingroup GUILIANI_LAYOUTER
    */

class CGUILayoutHelper
{
public:
    /** Base point on which the list positioning is based.
     */
    enum ListAlignment_t
    {
        LIST_ALIGN_HORIZONTAL=0,
        LIST_ALIGN_VERTICAL,
        LIST_ALIGN_INVALID
    };

    /** Base point on which the reposition is based.
     */
    enum RepositionAlignment_t
    {
        REPOSITION_ALIGN_TOP=0,
        REPOSITION_ALIGN_BOTTOM,
        REPOSITION_ALIGN_LEFT,
        REPOSITION_ALIGN_RIGHT,
        REPOSITION_ALIGN_INVALID
    };

    /** Definitions of the possible alignment types.
    */
    enum LayoutAlignment_t
    {
        LAYOUT_ALIGN_LEFT,
        LAYOUT_ALIGN_RIGHT,
        LAYOUT_ALIGN_TOP,
        LAYOUT_ALIGN_BOTTOM,
        LAYOUT_ALIGN_HORIZONTAL_CENTERED,
        LAYOUT_ALIGN_VERTICAL_CENTERED,
        LAYOUT_DISTRIBUTE_HORIZONTALLY,
        LAYOUT_DISTRIBUTE_VERTICALLY
    };
    

    /// return pointer to alignment handler instance
    static inline CGUILayoutHelper& GetInstance() { return ms_kLayoutHelper; }

    /// Destructor
    ~CGUILayoutHelper(){}

    /** Align all children to the given parent according to the alignment
        criteria and the relative distance.
        @param pkParent The given parent composite object.
        @param eAlignment The alignment criteria.
        @param vRelDistance The relative distance to range of the parent.
    */
    void AlignChildren(const CGUICompositeObject* pkParent,
        LayoutAlignment_t eAlignment, const eC_Value& vRelDistance = eC_FromInt(0)) const;

    /** Aligns the given object to the reference object according to the alignment
        criteria.
        @param pkAlignedObj The given object to be aligned.
        @param eAlignment The alignment criteria.
        @param pkRefObj The reference object the given object aligned to.
    */
    void AlignObject(CGUIObject* pkAlignedObj,
        LayoutAlignment_t eAlignment, const CGUIObject* pkRefObj) const;

    /** Aligns a list of objects to the reference object according to the alignment
        criteria.
        @param pkAlignedObjs The given list of object to be aligned.
        @param eAlignment The alignment criteria.
        @param pkRefObj The reference object the given object aligned to.
    */
    void AlignObjects(eC_TListDoubleLinked<CGUIObject*>& pkAlignedObjs,
        LayoutAlignment_t eAlignment, const CGUIObject* pkRefObj) const;

    /** Aligns the all children of a given parent object to grid. The grid is
        decided from the row and column number. That means the width of the grid
        is determined by dividing the width of the parent object to the number of
        the column and the height of the grid is determined by such kind of algorithm
        too. After the children are aligned, they are centered in the
        grid. If the given bMakeSameSize is true, the children change their
        dimension to be same as the grid.
        @param pkParentObj The given parent object.
        @param uiRow The number of rows.
        @param uiColumn The number of columns.
        @param bMakeSameSize Indicate whether the children change their
            dimensions to be the same as the grid or not.
    */
    void AlignToGrid(const CGUICompositeObject* pkParentObj, eC_UInt uiRow,
        eC_UInt uiColumn, eC_Bool bMakeSameSize) const;

    /** Change the given object to have the same width and height as the given
        reference object.
        @param pkObj The object whose dimension is to be changed.
        @param pkRefObj The reference object.
    */
    void ResizeObject(CGUIObject* pkObj, const CGUIObject* pkRefObj) const;

    /** Change a list of objects to have the same width and height as the given
        reference object.
        @param pkObjs A list of objects whose dimensions are to be changed.
        @param pkRefObj The reference object.
    */
    void ResizeObjects(eC_TListDoubleLinked<CGUIObject*>& pkObjs, const CGUIObject* pkRefObj) const;

    /** Change a list of objects to have the same horizontal/vertical distance to each other.
        The leftmost/top position of an object position is used as left/top border and the 
        rightmost/bottom object position is used as right/bottom border of space where the objects
        will be distributed. The order of the object inside the list determines the horizontal/vertical
        order of the objects after redistribution.
        @param pkObjs A list of objects who will be distributed.
        @param bHorizontal True if the objects should be distributed horizontally.
        @param bVertical True if the objects should be distributed vertically.
    */
    void DistributeObjects(eC_TListDoubleLinked<CGUIObject*>& pkObjs, eC_Bool bHorizontal, eC_Bool bVertical) const;

    /** Change the width of given object to be same as the given
        reference object.
        @param pkObj The object whose width is to be changed.
        @param pkRefObj The reference object.
    */
    void AdaptWidth(CGUIObject* pkObj, const CGUIObject* pkRefObj) const;

    /** Change the width of object in the object list to be same as the given
        reference object.
        @param pkObjs A list of objects whose widths are to be changed.
        @param pkRefObj The reference object.
    */
    void AdaptWidth(eC_TListDoubleLinked<CGUIObject*>& pkObjs, const CGUIObject* pkRefObj) const;

    /** Change the height of given object to be same as the given
        reference object.
        @param pkObj The object whose width is to be changed.
        @param pkRefObj The reference object.
    */
    void AdaptHeight(CGUIObject* pkObj, CGUIObject* pkRefObj) const;

    /** Change the height of object in the object list to be same as the given
        reference object.
        @param pkObjs A list of objects whose heights are to be changed.
        @param pkRefObj The reference object.
    */
    void AdaptHeight(eC_TListDoubleLinked<CGUIObject*>& pkObjs, CGUIObject* pkRefObj) const;

    /** Arrange all children in the composite object to have same space between each other in horizontally.
        This takes the left most object and then make same space between two objects.
        @param pkParent The parent object.
        @param vSpace The space between two objects.
    */
    void SpreadHorizontally(const CGUICompositeObject* pkParent, const eC_Value& vSpace) const;

    /** Arrange all children in the composite object to have same space between each other vertically
        This takes the top most object and then make same space between two objects.
        @param pkParent The parent object.
        @param vSpace The space between two objects.
    */
    void SpreadVertically(const CGUICompositeObject* pkParent, const eC_Value& vSpace) const;

    /** Moves the given object to the given absolute position by changing the relative
        position of pkObject. Calculation is based on absolute coordinates of (last)
        redraw time, so just use this function if you evaluate events or other occurrences
        of the past. For things in the future better use MoveToCurrentAbsXPos() or
        MoveToCurrentAbsYPos() because absolute positions of the last redraw are probably
        already outdated.
        @param pkObject The object that is to be moved.
        @param vAbsXPos The absolute x position that the given object is moved to.
    */
    void MoveToAbsXPos(CGUIObject* pkObject, const eC_Value& vAbsXPos) const;

    /** Moves the given object to the given absolute position by changing the relative
        position of pkObject. Calculation is based on absolute coordinates of (last)
        redraw time, so just use this function if you evaluate events or other occurrences
        of the past. For things in the future better use MoveToCurrentAbsXPos() or
        MoveToCurrentAbsYPos() because absolute positions of the last redraw are probably
        already outdated. 
        @param pkObject The object that is to be moved. 
        @param vAbsYPos The absolute y position that the given object is moved to.
    */
    void MoveToAbsYPos(CGUIObject* pkObject, const eC_Value& vAbsYPos) const;

    /** Moves the given object to the given absolute x position by changing the relative
        x position of pkObject. Calculation is based on current absolute coordinates.
        @param pkObject The object that is to be moved.
        @param vAbsXPos The absolute x position that the given object is moved to.
    */
    void MoveToCurrentAbsXPos(CGUIObject* pkObject, const eC_Value& vAbsXPos) const;

    /** Moves the given object under the given absolute y position by changing the relative
        y position of pkObject.
        @param pkObject The object that is to be moved.
        @param vAbsYPos The absolute y position that the given object is moved to.
    */
    void MoveToCurrentAbsYPos(CGUIObject* pkObject, const eC_Value& vAbsYPos) const;

    /** Get the current absolute X position of the given object, recalculated from
        the relative positions of all parents.
        @param pkObject The given object pointer.
        @return The absolute X position of the given object.
    */
    eC_Value GetCurrentAbsXPos(const CGUIObject* pkObject) const;

    /** Get the current absolute Y position of the given object, recalculated from
        the relative positions of all parents.
        @param pkObject The given object pointer.
        @return The absolute Y position of the given object.
    */
    eC_Value GetCurrentAbsYPos(const CGUIObject* pkObject) const;

    /** Get the current absolute bounding rectangle of the given object,
        recalculated with the relative positions of all parents.
        @param pkObject The given object pointer.
        @return The absolute Y position of the given object.
    */
    CGUIRect GetCurrentAbsRect(const CGUIObject* pkObject) const;

    /** Get the current difference of the absolute X position of the given objects,
        using the current absolute X positions, not the absolute X positions of the
        last redraw.
        Absolute X position of pkObjectRight minus absolute X position of pkObjectLeft
        is calculated, so the difference is negative if pkObjectRight is left of pkObjectLeft.
        @param pkObjectOnRight An object pointer of any GUI control.
        @param pkObjectOnLeft An object pointer of any GUI control.
        @return The difference of X positions of the two GUI controls, either positive or negative.
    */
    inline eC_Value GetCurrentAbsXDistance(
        const CGUIObject* pkObjectOnRight, const CGUIObject* pkObjectOnLeft) const
    {
        return GetCurrentAbsXPos(pkObjectOnRight) - GetCurrentAbsXPos(pkObjectOnLeft);
    }

    /** Get the current difference of the absolute Y position of the given objects,
        using the current absolute Y positions, not the absolute Y positions of the
        last redraw.
        Absolute Y position of pkObjectAtBottom minus absolute Y position of pkObjectAtTop
        is calculated, so the difference is negative if pkObjectAtTop is bottom of pkObjectAtBottom.
        @param pkObjectAtBottom An object pointer of any GUI control.
        @param pkObjectAtTop An object pointer of any GUI control.
        @return The difference of Y positions of the two GUI controls, either positive or negative.
    */
    inline eC_Value GetCurrentAbsYDistance(
        const CGUIObject* pkObjectAtBottom, const CGUIObject* pkObjectAtTop) const
    {
        return GetCurrentAbsYPos(pkObjectAtBottom) - GetCurrentAbsYPos(pkObjectAtTop);
    }

    /** Moves the given parent of the reference object so that the reference object will be
        positioned at the given current absolute coordinate.
        Be aware: If relative coordinate of any parent of the reference object changes afterwards then the
        drawing will most likely not correspond to the positioning via this function.
        @param pkParent Ancestor object of pkRefObject. This ancestor object will be moved.
        @param pkRefObject Object used to determine the new relative position of the parent (and therefore
               the absolute position of the reference object will also change).
        @param vAbsXPos The new absolute position for the reference object.
    */
    void MoveToCurrentAbsXPosByParentMovement(
        CGUICompositeObject* pkParent, const CGUIObject *pkRefObject, const eC_Value &vAbsXPos) const;

    /** Moves the given parent of the reference object so that the reference object will be
        positioned at the given current absolute coordinate.
        Be aware: If relative coordinate of any parent of the reference object changes afterwards then the
        drawing will most likely not correspond to the positioning via this function.
        @param pkParent Ancestor object of pkRefObject. This ancestor object will be moved.
        @param pkRefObject Object used to determine the new relative position of the parent (and therefore
               the absolute position of the reference object will also change).
        @param vAbsYPos The new absolute position for the reference object.
    */
    void MoveToCurrentAbsYPosByParentMovement(
        CGUICompositeObject* pkParent, const CGUIObject *pkRefObject, const eC_Value &vAbsYPos) const;

    /** Moves the given parent of the reference object so that the center of the reference object
        will currently be positioned at the given absolute coordinate.
        Be aware: If relative coordinate of any parent of the reference object changes afterwards then the
        drawing will most likely not correspond to the positioning via this function.
        @param pkParent Ancestor object of pkRefObject. This ancestor object will be moved.
        @param pkRefObject Object used to determine the new relative position of the parent (and therefore
               the absolute position of the reference object's center will also change).
        @param vAbsXPos The new absolute position for the center of the reference object.
    */
    void MoveCenterToCurrentAbsXPosByParentMovement(
        CGUICompositeObject *pkParent, const CGUIObject *pkRefObject, const eC_Value &vAbsXPos) const;

    /** Moves the given parent of the reference object so that the center of the reference object
        will currently be positioned at the given absolute coordinate.
        Be aware: If relative coordinate of any parent of the reference object changes afterwards then the
        drawing will most likely not correspond to the positioning via this function.
        @param pkParent Ancestor object of pkRefObject. This ancestor object will be moved.
        @param pkRefObject Object used to determine the new relative position of the parent (and therefore
               the absolute position of the reference object's center will also change).
        @param vAbsyPos The new absolute position for the center of the reference object.
    */
    void MoveCenterToCurrentAbsYPosByParentMovement(
        CGUICompositeObject *pkParent, const CGUIObject *pkRefObject, const eC_Value &vAbsyPos) const;

    /** Resizes and moves pkParent to bounding rectangle of children.
        @param pkParent The parent object
    */
    void UseBoundingRectangleOfChildren(CGUICompositeObject *pkParent) const;

    /** Repositions a composite object's visible children based on a base point and minimizes
        the composite's size. E.g. if the base point is REPOSITION_ALIGN_BOTTOM,
        the first child is positioned at the bottom, the next one on top of it, and so
        forth, and the height of the parent is minimized to match the top and bottom edges
        of the visible repositioned child objects.
        @param pkParent The parent object.
        @param vBorder Gap between outer children and parent's edges.
        @param vGap The gap between the children.
        @param eBasePoint The base point on which the repositioning is based.
        @param bResizeParent If true, the parent will be resized to be big enough to encapsulate all children. 
               If false, the parent's size will remain untouched.
      */
    void RepositionChildren(
        CGUICompositeObject *pkParent,
        eC_Value vBorder,
        eC_Value vGap,
        RepositionAlignment_t eBasePoint,
        eC_Bool bResizeParent = true) const;

    /** Arranges the children of the supplied compositeobject in a list, which is either aligned vertically (column by column) or horizontally (row by row).
        The LayoutHelper will arrange the objects within the area given in kAvailableArea, which is a rectangle in RELATIVE coordinates within the CompositeObject.
        Invisible child-objects will not be taken into account while re-arranging the children.
        @param pkParent CompositeObject whose children shall be arranged
        @param kAvailableArea Rectangle in coordinates relative to pkParent in which the children will be arranged.
        @param bUnifyItemSize If set to true, all objects will automatically be resized to be the same size as the largest object
        @param eBasePoint Specifices whether the objects will be arranged vertically or horizontally.
      */
    void AdjustListArrangement(
        CGUICompositeObject*    pkParent,
        const CGUIRect&         kAvailableArea,
        const eC_Bool&          bUnifyItemSize,
        ListAlignment_t         eBasePoint) const;

private:

    /** Layouthelper constructor.
    */
    CGUILayoutHelper(){}

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUILayoutHelper(const CGUILayoutHelper& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUILayoutHelper& operator=(const CGUILayoutHelper& kSource);

    /// The singleton instance.
    static CGUILayoutHelper   ms_kLayoutHelper;
};
#endif
