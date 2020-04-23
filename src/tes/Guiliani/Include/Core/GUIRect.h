/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRECT__H_
#define GUIRECT__H_

#include "GUIPoint.h"
#include "eC_Math.h"

/// Helper class to supply a platform independent rectangle implementation.

/** The CGUIRect class implements basic geometry-handling for rectangles, including
    functionalities such as intersection or checks for overlaps.
    Please note that the lower right corner of a rectangle is not considered to be "inside"
    of the rectangle itself.
    In other words, a rectangle reaching from positions (0,0) to (10,0) will have a height of zero
    and therefore be considered "not normalized".
    <p>
    If a rectangle is incomprehensive this means that it has no valid coordinates.
    When a rectangle is copied the comprehensive flag is always copied too.
    After a rectangle is created it will always be comprehensive until it 
    is explicitly changed by m_bComprehensive = false;
    No function that accepts separate coordinate values will automatically
    normalise them internally in this class.
    <p>
    Speaking of normalised coordinates here means, that the X-coordinate of the 
    top left corner is smaller than the X-coordinate of the bottom right corner 
    AND
    the Y-coordinate of the top left corner is smaller than the Y-coordinate of the bottom right corner.
    <p>
    Whenever the coordinates of a rectangle get non-normalised then the rectangle
    will in turn get incomprehensive, that is, no comprehensive rectangle may
    be non-normalised. Incomprehensive rectangles on the other hand may have
    normalised or non-normalised coordinates as the coordinates are invalid anyway.
    No function will make an incomprehensive rectangle to be comprehensive, except
    when forcing this via SetComprehensive(true) or assignment or construction.
    Thus, no function of this class that does not alter the coordinates, needs to
    check whether the coordinates are normalised; for these functions it is 
    sufficient to check for comprehensiveness.
    
    Example:
@code

    // Create a rect.
    CGUIRect *pRect = new CGUIRect(eC_FromInt(0),eC_FromInt(0),eC_FromInt(100),eC_FromInt(20));

    // Sets the width of the rectangle.
    pRect->SetWidth(eC_FromInt(50));

    // Assign
    pRect->Assign(eC_FromInt(10),eC_FromInt(20),eC_FromInt(100),eC_FromInt(20));

@endcode

*/
class CGUIRect
{
public:
    /** Standard constructor. Initializes all coordinates with 0. The rectangle will 
        be incomprehensive as the rectangle that covers zero space is invalid.
    */
    CGUIRect();

    /** Constructor
        @param kLeftUpperPt left upper point
        @param kRightLowerPt right lower point
    */
    CGUIRect(const CGUIPoint &kLeftUpperPt, const CGUIPoint &kRightLowerPt);

    /** Constructor
        @param kOriginPt origin point
        @param vWidth width
        @param vHeight height
    */
    CGUIRect(const CGUIPoint &kOriginPt, const eC_Value &vWidth, const eC_Value &vHeight);

    /** Constructs a rectangle from four coordinates. The rectangle will be 
        comprehensive if the covered space is not zero.
        @param vX1 X-coordinate of the top left corner
        @param vY1 Y-coordinate of the top left corner
        @param vX2 X-coordinate of the bottom right corner
        @param vY2 Y-coordinate of the bottom right corner
    */
    CGUIRect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);

    /** Checks if this rectangle is fully to the right of 'Rect' or
        the relevant edges touch each other.
        @param Rect The rectangle to check
        @return True if fully to the right
    */
    eC_Bool IsRightOf(const CGUIRect &Rect) const
    {
        return m_vX1 >= Rect.GetX2();
    }

    /** Checks if this rectangle is fully to the left of 'Rect' or
        the relevant edges touch each other.
        @param Rect The rectangle to check
        @return True if fully to the left
    */
    eC_Bool IsLeftOf(const CGUIRect &Rect) const
    {
        return m_vX2 <= Rect.GetX1();
    }

    /** Checks if this rectangle is fully top of 'Rect' or
        the relevant edges touch each other.
        @param Rect The rectangle to check
        @return True if fully to the top
    */
    eC_Bool IsTopOf(const CGUIRect &Rect) const
    {
        return m_vY2 <= Rect.GetY1();
    }

    /** Checks if this rectangle is fully at the bottom of 'Rect' or
        the relevant edges touch each other.
        @param Rect The rectangle to check
        @return True if fully to the bottom
    */
    eC_Bool IsBottomOf(const CGUIRect &Rect) const
    {
        return m_vY1 >= Rect.GetY2();
    }

    /// Returns the rectangle as if it would be placed at the origin (0, 0).
    inline CGUIRect GetOriginRect() const
    {
        return CGUIRect(eC_FromInt(0), eC_FromInt(0), GetWidth(), GetHeight());
    }

    /// @return The center point (middle) of the rectangle.
    inline CGUIPoint GetCenter() const
    {
        return CGUIPoint(
            m_vX1 + eC_Div(m_vX2 - m_vX1, eC_FromInt(2)),
            m_vY1 + eC_Div(m_vY2 - m_vY1, eC_FromInt(2)));
    }

    /// @return The upper left vertex of the rectangle.
    inline CGUIPoint GetTopLeft() const
    {
        return CGUIPoint(m_vX1, m_vY1);
    }

    /// @return The lower right vertex of the rectangle.
    inline CGUIPoint GetBottomRight() const
    {
        return CGUIPoint(m_vX2, m_vY2);
    }

    /** Returns the width of the rectangle. This function only returns the expected value
        if it is used on a comprehensive (and normalized) rectangle, so please check this.
        This function also only works correctly if the coordinates are not more far away from
        each other than eC_MAXVALUE so please be careful that your rectangles don't get too big.
        @return The width
    */
    inline eC_Value GetWidth() const
    {
        return m_vX2 - m_vX1;
    }

    /** Returns the height of the rectangle. This function only returns the expected value
        if it is used on a comprehensive (and normalized) rectangle, so please check this.
        This function also only works correctly if the coordinates are not more far away from
        each other than eC_MAXVALUE so please be careful that your rectangles don't get too big.
        @return The height
    */
    inline eC_Value GetHeight() const
    {
        return m_vY2 - m_vY1;
    }

    /** Sets the new width of the rectangle. Right corner will be extended.
        @param vWidth new width of the rectangle
    */
    inline void SetWidth(const eC_Value vWidth)
    {
        m_vX2 = m_vX1 + vWidth;
    }

    /** Sets the new height of the rectangle. Bottom corner will be extended.
        @param vHeight new height of the rectangle
    */
    inline void SetHeight(const eC_Value vHeight)
    {
        m_vY2 = m_vY1 + vHeight;
    }

    inline eC_Value GetX1() const { return m_vX1; } ///< @return X of vertex 1
    inline eC_Value GetY1() const { return m_vY1; } ///< @return Y of vertex 1
    inline eC_Value GetX2() const { return m_vX2; } ///< @return X of vertex 2
    inline eC_Value GetY2() const { return m_vY2; } ///< @return Y of vertex 2
    inline void SetX1(const eC_Value &vX1) { m_vX1 = vX1; }///< @param vX1 New X of vertex 1
    inline void SetY1(const eC_Value &vY1) { m_vY1 = vY1; }///< @param vY1 New X of vertex 1
    inline void SetX2(const eC_Value &vX2) { m_vX2 = vX2; }///< @param vX2 New Y of vertex 2
    inline void SetY2(const eC_Value &vY2) { m_vY2 = vY2; }///< @param vY2 New Y of vertex 2

    /** If true then it is guaranteed that the rectangle is also normalised.
        @return True if comprehensive
    */
    inline eC_Bool IsComprehensive() const { return m_bComprehensive; }

    /** If a non-normalised rectangle is forced to be comprehensive via
        this function, then it also gets normalised automatically.
        @param bComprehensive true if comprehensive
    */
    inline void SetComprehensive(const eC_Bool &bComprehensive)
    { 
        if (!IsNormalised() && bComprehensive)
        {
            m_bComprehensive = true;
            Normalise(); // only normalised rectangles may be comprehensive
        }
        else
        {
            m_bComprehensive = bComprehensive;
    }
    }

    /** Constructs a rectangle from another rectangle / copy constructor.
        @param srcRect Rectangle from which a duplicate will be created.
    */
    CGUIRect(const CGUIRect &srcRect);

    /** Assigns an existing rectangle from another rectangle.
        @param srcRect Rectangle from which a duplicate will be created.
        @return Assignment destination
    */
    CGUIRect& operator=(const CGUIRect& srcRect);

    /** Checks whether two rectangles are identical. They are identical if the coordinates 
        of their upper left and lower right corners are identical.
        @param rkRect Rectangle to compare with.
        @return True if the rectangles are identical, otherwise False.
    */
    inline eC_Bool operator==(const CGUIRect& rkRect) const
    { 
        return  (m_vX1  == rkRect.GetX1()) &&
                (m_vY1  == rkRect.GetY1()) &&
                (m_vX2  == rkRect.GetX2()) &&
                (m_vY2  == rkRect.GetY2()); 
    }

    /** Checks whether two rectangles are identical. They are identical if the coordinates 
        of their upper left and lower right corners are identical.
        @param rkRect Rectangle to compare with.
        @return True if the rectangles are identical, otherwise False.
    */
    inline eC_Bool operator!=(const CGUIRect& rkRect) const
    {
        return !operator==(rkRect);
    }

    /** Assigns new coordinates to an existing rectangle.
        The rectangle will be comprehensive if the covered area of the new 
        rectangle is not negative.
        @param kOriginPt X- and Y-coordinate of the left upper corner.
        @param vWidth The width of the rectangle.
        @param vHeight The height of the rectangle.
        @return This rectangle after assignment
    */
    CGUIRect& Assign(const CGUIPoint &kOriginPt, const eC_Value &vWidth, const eC_Value &vHeight);

    /** Assigns new coordinates to an existing rectangle.
        The rectangle will be comprehensive if the covered area greater than zero.
        @param vX1 X-coordinate of the top left corner
        @param vY1 Y-coordinate of the top left corner
        @param vX2 X-coordinate of the bottom right corner
        @param vY2 Y-coordinate of the bottom right corner
        @return This rectangle after assignment
    */
    CGUIRect& Assign(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);

    /** Does an overlap-check between two rectangles.
        @param Rect The rectangle to check
        @return True if the two rectangles overlap, False otherwise. No incomprehensive rectangle
        overlaps another.
        @throws CGUIException if one of the rectangles is not normalised (only in _DEBUG build)
    */
    eC_Bool Overlaps(const CGUIRect &Rect) const;

    /** Merges the rectangle with another rectangle. The resulting rectangle is the smallest 
        enclosing rectangle around the two input rectangles and is stored within this object. 
        Doing this function on an incomprehensive rectangle that is passed as an argument 
        doesn't change any rectangle. 
        If this rectangle is incomprehensive and the rectangle that is passed as an argument is 
        comprehensive then the argument will be copied to this rectangle. This is because not
        just a usual comprehensive rectangle but also an incomprehensive rectangle should be 
        able to get bigger when a union with a comprehensive rectangle is performed. This is 
        especially useful for enlarging the invalidated rectangle in reset (= incomprehensive) 
        state.
        A rectangle with covers zero space will have no effect for a union operation. 
        @throws CGUIException if one of the rectangles is not normalised (only in _DEBUG build)
        @param Rect The rectangle to check
        @return This rectangle after operation
    */
    CGUIRect& Union(const CGUIRect &Rect);

    /** Calculates the area where two rectangles overlap. The resulting rectangle is stored within
        this object. If the rectangles do not overlap then "this" rectangle will be incomprehensive.
        Doing this function on an incomprehensive "this" rectangle doesn't change any rectangle.
        Calling this function on a comprehensive "this" rectangle and an incomprehensive argument
        rectangle will make "this" rectangle incomprehensive. 
        @throws CGUIException if one of the rectangles is not normalised (only in _DEBUG build)
        @param Rect The rectangle to check
        @return This rectangle after operation
    */
    CGUIRect& InterSect(const CGUIRect &Rect);

    /** Checks whether a point at a given coordinate is inside a rectangle or not.
        Only comprehensive rectangles contain points.
        @param vX X-coordinate
        @param vY Y-coordinate
        @return True if it is inside, False otherwise.
    */
    inline eC_Bool IsInside(const eC_Value &vX, const eC_Value &vY) const 
    {
        if (m_bComprehensive)
        {
            return ( (vX >= m_vX1) && (vX < m_vX2) && (vY >= m_vY1) && (vY < m_vY2) );
        }
        else
        {
            return false;
        }
    }

    /** Checks whether a point at a given coordinate is inside a rectangle or not.
        Only comprehensive rectangles contain points.
        @param kPt The point to check
        @return True if it is inside, False otherwise.
    */
    inline eC_Bool IsInside(const CGUIPoint &kPt) const
    {
        return IsInside(kPt.m_vX, kPt.m_vY);
    }

    /** Check if the given rectangle is completely within this rectangle. 
        True is also returned for exactly same positions and sizes.
        @param kRect The rectangle to check
        @return True if inside, False otherwise
    */
    inline eC_Bool IsInside(const CGUIRect &kRect) const
    {
        return kRect.IsComprehensive() && IsInside(kRect.GetTopLeft()) && (kRect.GetY2() <= m_vY2) && (kRect.GetX2() <= m_vX2);
    }

    ///enum of expansion types
    enum ExpandDirection_t
    {
        EXPAND_HORIZONTAL, ///<horizontal only
        EXPAND_VERTICAL,   ///<vertical only
        EXPAND_ALL         ///<both horizontal and vertical (default)
    };

    /** Expand the rectangle. Depending on sign this results in expanding or shrinking the rect
        @param vDif value to add on all sides
        @param enDirection
        @return This rectangle after operation
    */
    inline CGUIRect& Expand(const eC_Value vDif, const ExpandDirection_t enDirection = EXPAND_ALL)
    {
        if ((enDirection == EXPAND_ALL) || (enDirection == EXPAND_HORIZONTAL))
        {
            m_vX1 = m_vX1 - vDif;
            m_vX2 = m_vX2 + vDif;
        }

        if ((enDirection == EXPAND_ALL) || (enDirection == EXPAND_VERTICAL))
        {
            m_vY1 = m_vY1 - vDif;
            m_vY2 = m_vY2 + vDif;
        }
        
        if ((vDif < eC_FromInt(0)) && (!IsNormalised()))
        {
            // avoid non-normalised state of "this" rectangle,
            // alternatively we could call Normalise() here but the calculated coordinates make no sense 
            // anyway -> "this" rectangle is now invalid
            m_bComprehensive = false;
        }
        return *this;
    }
    
    /** Moves the rectangle. 
        @param kOffsetPt Pixels to move in x/y direction
        @return This rectangle after operation
    */
    inline CGUIRect& Move(const CGUIPoint &kOffsetPt)
    {
        m_vX1 += kOffsetPt.m_vX;
        m_vX2 += kOffsetPt.m_vX;

        m_vY1 += kOffsetPt.m_vY;
        m_vY2 += kOffsetPt.m_vY;

        return *this;
    }

    /** Moves the rectangle. 
        @param vDeltaX Pixels to move in x direction
        @param vDeltaY Pixels to move in y direction
        @return This rectangle after operation
    */
    inline CGUIRect& Move(const eC_Value vDeltaX, const eC_Value vDeltaY)
    {
        m_vX1 += vDeltaX;
        m_vX2 += vDeltaX;

        m_vY1 += vDeltaY;
        m_vY2 += vDeltaY;

        return *this;
    }

    /** Moves the rectangle to an absolute position
        @param vAbsX Target position in x direction
        @param vAbsY Target position in y direction
        @return This rectangle after operation
    */
    inline CGUIRect& MoveTo(const eC_Value vAbsX, const eC_Value vAbsY)
    {
        eC_Value vXDif = vAbsX-m_vX1;
        eC_Value vYDif = vAbsY-m_vY1;
        m_vX1 += vXDif;
        m_vX2 += vXDif;

        m_vY1 += vYDif;
        m_vY2 += vYDif;

        return *this;
    }

    /** Interpolate between two rectangles.
        @param kTargetRect Target of interpolation
        @param vInterpolationFactor Value between 0 (this rectangle stays unchanged) 
               and 1 (identical to target rectangle)
        @return This rectangle after operation
    */
    inline CGUIRect& Interpolate(const CGUIRect& kTargetRect, const eC_Value vInterpolationFactor)
    {
        m_vX1 = m_vX1 + eC_Mul( vInterpolationFactor, (kTargetRect.GetX1() - m_vX1));
        m_vY1 = m_vY1 + eC_Mul( vInterpolationFactor, (kTargetRect.GetY1() - m_vY1));
        m_vX2 = m_vX2 + eC_Mul( vInterpolationFactor, (kTargetRect.GetX2() - m_vX2));
        m_vY2 = m_vY2 + eC_Mul( vInterpolationFactor, (kTargetRect.GetY2() - m_vY2));
        if(vInterpolationFactor < eC_FromInt(0) && !IsNormalised())
        {
            // avoid non-normalised state of "this" rectangle,
            // alternatively we could call Normalise() here but the calculated coordinates make no sense 
            // anyway -> "this" rectangle is now invalid
            m_bComprehensive = false;
        }
        return *this;
    }

    /**
        This normalises the rectangle so that the functions and operations
        of this class may operate on it successfully. Only comprehensive
        rectangles will be made normalised as it makes no sense to normalise
        invalid coordinates.
    */
    void Normalise();

#ifdef GUILIANI_STREAM_GUI 
    /// Helper method to stream CGUIRect attributes.
    void ReadFromStream();
#endif 

#ifdef GUILIANI_WRITE_GUI 
    /// Helper method to write CGUIRect attributes.
    void WriteToStream();
#endif

private:
    /** 
        Checks whether the rectangle is normalized but does not consider the
        comprehensiveness. User code will use IsComprehensive() to determine this.
    */
    inline eC_Bool IsNormalised() const
    {
        return !((m_vX1 >= m_vX2) || (m_vY1 >= m_vY2));
    }

    // Member variables
    eC_Bool m_bComprehensive; ///< if this flag is false then all coordinates of this class are invalid to use
    eC_Value m_vX1, m_vY1, m_vX2, m_vY2; ///< coordinates
};
#endif
