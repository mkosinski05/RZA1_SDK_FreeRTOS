/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Vector2.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:20:44 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_VECTOR2__H_
#define EGML_VECTOR2__H_
//----------------------------------------------------------------------------
#include "eC_Math.h"
#include "eGML_Config.h"

//----------------------------------------------------------------------------
/**
@memo
Basic class for convenient and effective working with 2D coordinates.
@doc
The class eGML_Vector2 provides effective handling of 2D-coordinates for every vector operation. Therefor every coordinate
has to be interpretated as a position vector with point of origin in coordinate (0,0). For getting access to the components
(\Ref{x}- and \Ref{y}-coordinate) of an vector in a flexible way, you are able to <BR>
1. use the <TT>.x</TT> or <TT>.y</TT> attribut by itself or <BR>
2. use indeces like in a normal array (<TT>[0]</TT> for <TT>x</TT>, <TT>[1]</TT> for <TT>y</TT>) <BR>
Analog to the following display format this documentation is based on: <BR><BR>
\IMG{pictures/vector2_general.gif} <BR><BR>
For getting more information about \Ref{eGML_Vector2} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Vector2_1.cpp]{examples/example_Vector2_1.cpp.html} <BR>
@see eGML_Vector3, eGML_Matrix2
@see Tutorial_eGML
*/
class eGML_Vector2
{
  public:

    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

    /// Member access. Allows <TT>v.x</TT> or <TT>v[0]</TT>.
    eC_Value x;
    /// Member access. Allows <TT>v.y</TT> or <TT>v[1]</TT>.
    eC_Value y;

    // special vectors for basic operations
    
    /// Zero vector (0, 0).
    static eGML_Vector2 ZERO;
    /// Unit vector (1, 0).
    static eGML_Vector2 UNIT_X;
    /// Unit vector (0, 1).
    static eGML_Vector2 UNIT_Y;

    // !!! INTERNAL USE ONLY !!!
    static void InitGlobals();

    /** Standard constructor. For effecient construction of large vector arrays - there is no initialisation. */
    inline eGML_Vector2 ();

    /** Constructor. With initialisation of \Ref{x}- and \Ref{y}-component of new vector.
    @param fX X-component of new vector.
    @param fY Y-component of new vector.
    @see x, y */
    inline eGML_Vector2 (eC_Value fX, eC_Value fY);

    /** Constructor. With initialisation of <TT>v[0]</TT> and <TT>v[1]</TT> component.
    @param afCoordinate Array containing both components of new vector.
    @see x, y */
    inline eGML_Vector2 (eC_Value afCoordinate[2]);

    /** Copyconstructor.
    @param rkVector Vector to copy from for new vector. */
    inline eGML_Vector2 (const eGML_Vector2& rkVector);

    /** Provides access to one of the components of the vector, as part of an array. There is no checkup if the index makes
    sense. Only the values 0 (for <TT>v.\Ref{x}</TT>) and 1 (for <TT>v.\Ref{y}</TT>) are reasonable.
    @param i Index to indicate if X- or Y-component shall be returned (only 0 and 1 are defined).
    @return Component <TT>v.x</TT> if index was 0 and <TT>v.y</TT> if index was 1. All other indices are undefined.
    @see x, y */
    inline eC_Value& operator[] (eC_Int i) const;

    /** Provides access to one of the components of the vector, as part of an array. There is no checkup if the index makes
    sense. Only the values 0 (for <TT>v.\Ref{x}</TT>) and 1 (for <TT>v.\Ref{y}</TT>) are reasonable.
    @param i Index to indicate if X- or Y-component shall be returned (only 0 and 1 are defined).
    @return Component <TT>v.x</TT> if index was 0 and <TT>v.y</TT> if index was 1. All other indices are undefined.
    @see x, y */
    inline eC_Value& operator[] (int i) const;

    /** Cast operator to provide the addresses of the components themselves (not of the object!).
    @return Pointer to <TT>v.x</TT> as the beginning of the array <TT>v[0..1]</TT>.
    @see x, y */
    inline operator eC_Value* ();
    
    // assignment and comparison

    /** Reinitialize this vector with the data of another vector.
    @param rkVector Vector to copy the data from.
    @return Reinitialized vector. */
    inline eGML_Vector2& operator= (const eGML_Vector2& rkVector);

    /** Compares two vectors and checks if all their components are equal.
    @param rkVector Vector to compare with.
    @return Only if all components are equal <B>true</B> will be returned. */
    inline bool operator== (const eGML_Vector2& rkVector) const;

    /** Compares two vectors and checks if one component is not equal.
    @param rkVector Vector to compare with.
    @return Only if all components are equal <B>false</B> will be returned. */
    inline bool operator!= (const eGML_Vector2& rkVector) const;
    
    // arithmetic operations
    
    /** Calculates the sum of two vectors.
    @param rkVector Second addend.
    @return Sum of the two vectors. */
    inline eGML_Vector2 operator+ (const eGML_Vector2& rkVector) const;

    /** Calculates the difference between two vectors.
    @param rkVector Subtrahend.
    @return Result of subtraction (difference). */
    inline eGML_Vector2 operator- (const eGML_Vector2& rkVector) const;

    /** Calculates the division of two vectors.
    There is no checkup if the components of the divisor vector is equal zero.
    @param rkVector Divisor vector.
    @return Result of division. */
    inline eGML_Vector2 operator/ (const eGML_Vector2& rkVector) const;

    /** Calculates the multiplication of two vectors.
    @param rkVector Second multiplicator vector.
    @return Result of multiplication. */
    inline eGML_Vector2 operator* (const eGML_Vector2& rkVector) const;

    /** Calculates the multiplication of the vector by a scalar.
    @param fScalar Scalar multiplicator.
    @return Result of multiplication. */
    inline eGML_Vector2 operator* (eC_Value fScalar) const;

    /** Calculates the multiplication of a scalar by another vector.
    So it is possible now to make for example following operation: <BR>
    <TT>v1 = 2 * v2</TT>.
    @param fScalar Scalar multiplicator.
    @param rkVector Second vector multiplicator.
    @return Result of multiplication. */
    inline friend eGML_Vector2 operator* (eC_Value fScalar, const eGML_Vector2& rkVector);

    /** Calculates the division of the vector by a scalar.
    If there is a division by zero, no exception is thrown but the maximum value of <B>eGML</B> is returned.
    @param fScalar Scalar divisor.
    @return Result of division. */
    inline eGML_Vector2 operator/ (eC_Value fScalar) const;

    /** Calculates the inverse vector of this vector.
    That means the resulting vector has got the same {@link Length length} but the opposite direction.
    @return Inverse vector.
    @see Length */
    inline eGML_Vector2 operator- () const;

    // arithmetic updates
    
    /** Calculates the sum of two vectors and stores the result.
    @param rkVector Second addend.
    @return Sum of the two vectors. */
    inline eGML_Vector2& operator+= (const eGML_Vector2& rkVector);

    /** Calculates the subtraction of two vectors and stores the result.
    @param rkVector Subtrahend.
    @return Result of subtraction. */
    inline eGML_Vector2& operator-= (const eGML_Vector2& rkVector);

    /** Calculates the division of the vector by a scalar and stores the result.
    If there is a division by zero, no exception is thrown but the maximum value of <B>eGML</B> is returned.
    @param fScalar Scalar divisor.
    @return Result of division. */
    inline eGML_Vector2& operator/= (eC_Value fScalar);

    /** Calculates the multiplication of the vector by a scalar and stores the result.
    @param fScalar Scalar multiplicator.
    @return Result of multiplication. */
    inline eGML_Vector2& operator*= (eC_Value fScalar);

    // vector operations

    /** Calculates the length (the absolut value) of the vector by using the theorem of Pythagoras. <BR>
    <TT>Length = sqrt(v.x^2 + v.y^2)</TT>.
    @return Length of the vector.
    @see x, y, SquaredLength */
    inline eC_Value Length () const;

    /** Calculates the squared {@link Length length} of the vector. <BR>
    <TT>SquaredLength = v.x^2 + v.y^2</TT>.
    @return Squared length of the vector.
    @see x, y, Length */
    inline eC_Value SquaredLength () const;

    /** Calculates the scalar product of two vectors. <BR>
    <TT>Dot = v1.x * v2.x + v1.y * v2.y</TT>.
    @param rkVecor Vector to build scalar product with.
    @return Scalar product of two vectors.
    @see Orthogonal, Cross */
    inline eC_Value Dot (const eGML_Vector2& rkVector) const;

    /** Calculates the 2D-cross-product (vector product) of two vectors.
    A 2D-cross-product is defined by rotating one vector counterclockwise by 90 degrees and taking the {@link Dot dot} product
    between both vectors. The result is the surface of a parallelogram that is formed by these two vectors. <BR>
    <TT>Cross = v1.x * v2.y - v1.y * v2.x</TT>
    @param rkVector Second vector of the parallelogram.
    @return 2D-cross-product (means the surface of the parallelogram).
    @see x, y, Dot */
    inline eC_Value Cross (const eGML_Vector2& rkVector) const;

    /** Calculates the unitized vector out of this vector. Therefor the {@link Length length} of the vector has to be calculated.
    If the length of the vector is <I>smaller</I> than the given tolerance, this operation returns the \Ref{UNIT_X} vector.
    There is no checkup if the length of vector equals zero.
    The resulting vector has got the characteristics: length = 1, components: (<TT>cos a</TT>, <TT>cos b</TT>);
    <TT><B>a</B></TT> is the angle between vector and X-axis, <TT><B>b</B></TT> is the angle between vector and Y-axis.
    @param fTolerance Precision of checking for correct calculation.
    @return Unitized vector (length = 1, same direction of the original vector) or \Ref{UNIT_X} if operation fails.
    @see Length, UNIT_X */
    eGML_Vector2 Unitize (eC_Value fTolerance = eC_FromFloat(1e-06f));

    /** Calculates a new vector that is orthogonal to this vector. The result is rotated by 90 degrees (scalar product has to be zero).
    The orthogonal vector to (<TT>v.x</TT>, <TT>v.y</TT>) is (<TT>v.y</TT>, <TT>v.-x</TT>).
    @return Orthogonal vector to the source.
    @see x, y, Dot */
    inline eGML_Vector2 Orthogonal () const;
};

//----------------------------------------------------------------------------
inline eGML_Vector2::eGML_Vector2 ()
{
    // For efficiency in construction of large arrays of vectors, the
    // default constructor does not initialize the vector.
}

//----------------------------------------------------------------------------
inline eGML_Vector2::eGML_Vector2 (eC_Value fX, eC_Value fY)
{
    x = fX;
    y = fY;
}

//----------------------------------------------------------------------------
inline eGML_Vector2::eGML_Vector2 (eC_Value afCoordinate[2])
{
    x = afCoordinate[0];
    y = afCoordinate[1];
}

//----------------------------------------------------------------------------
inline eGML_Vector2::eGML_Vector2 (const eGML_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
}

//----------------------------------------------------------------------------
inline eC_Value& eGML_Vector2::operator[] (eC_Int i) const
{
    // assert:  0 <= i < 2; x and y are packed into 2*sizeof(eC_Value) bytes
    return (eC_Value&) *(&x + i);
}

//----------------------------------------------------------------------------
inline eC_Value& eGML_Vector2::operator[] (int i) const
{
    // assert:  0 <= i < 2; x and y are packed into 2*sizeof(eC_Value) bytes
    return (eC_Value&) *(&x + i);
}

//----------------------------------------------------------------------------
inline eGML_Vector2::operator eC_Value* ()
{
    return &x;
}

//----------------------------------------------------------------------------
inline eGML_Vector2& eGML_Vector2::operator= (const eGML_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
    return *this;
}

//----------------------------------------------------------------------------
inline bool eGML_Vector2::operator== (const eGML_Vector2& rkVector) const
{
    return ( x == rkVector.x && y == rkVector.y );
}

//----------------------------------------------------------------------------
inline bool eGML_Vector2::operator!= (const eGML_Vector2& rkVector) const
{
    return ( x != rkVector.x || y != rkVector.y );
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator+ (const eGML_Vector2& rkVector) const
{
    eGML_Vector2 kSum;
    kSum.x = x + rkVector.x;
    kSum.y = y + rkVector.y;
    return kSum;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator- (const eGML_Vector2& rkVector) const
{
    eGML_Vector2 kDiff;
    kDiff.x = x - rkVector.x;
    kDiff.y = y - rkVector.y;
    return kDiff;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator/ (const eGML_Vector2& rkVector) const
{
    eGML_Vector2 kDiv;
    kDiv.x = eC_Div(x , rkVector.x); // by Morten: check for division by zero or eC_MINVALUE?
    kDiv.y = eC_Div(y , rkVector.y); // by Morten: check for division by zero or eC_MINVALUE?
    return kDiv;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator* (const eGML_Vector2& rkVector) const
{
    eGML_Vector2 kDiv;
    kDiv.x = eC_Mul(x , rkVector.x);
    kDiv.y = eC_Mul(y , rkVector.y);
    return kDiv;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator* (eC_Value fScalar) const
{
    eGML_Vector2 kProd;
    kProd.x = eC_Mul(fScalar, x);
    kProd.y = eC_Mul(fScalar, y);
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator- () const
{
    eGML_Vector2 kNeg;
    kNeg.x = -x;
    kNeg.y = -y;
    return kNeg;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 operator* (eC_Value fScalar, const eGML_Vector2& rkVector)
{
    eGML_Vector2 kProd;
    kProd.x = eC_Mul(fScalar, rkVector.x);
    kProd.y = eC_Mul(fScalar, rkVector.y);
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector2& eGML_Vector2::operator+= (const eGML_Vector2& rkVector)
{
    x += rkVector.x;
    y += rkVector.y;
    return *this;
}

//----------------------------------------------------------------------------
inline eGML_Vector2& eGML_Vector2::operator-= (const eGML_Vector2& rkVector)
{
    x -= rkVector.x;
    y -= rkVector.y;
    return *this;
}

//----------------------------------------------------------------------------
inline eGML_Vector2& eGML_Vector2::operator*= (eC_Value fScalar)
{
    x = eC_Mul(x, fScalar);
    y = eC_Mul(y, fScalar);
    return *this;
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector2::SquaredLength () const
{
    return eC_Mul(x, x) + eC_Mul(y, y);
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector2::Length () const
{
    return eC_Len2( x, y );
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector2::Dot (const eGML_Vector2& rkVector) const
{
    return eC_Mul(x, rkVector.x) + eC_Mul(y, rkVector.y);
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Vector2::operator/ (eC_Value fScalar) const
{
    eGML_Vector2 kQuot;

    if ( fScalar != eC_FromInt(0) ) //by Morten: whats about the check with eC_MINVALUE ?
    {
        eC_Value fInvScalar = eC_ReciQ( fScalar );
        kQuot.x = eC_Mul( fInvScalar, x );
        kQuot.y = eC_Mul( fInvScalar, y );
        return kQuot;
    }
    else
    {
        return eGML_Vector2(eC_MAXVALUE , eC_MAXVALUE); //INFINITY
    }
}

//----------------------------------------------------------------------------
inline eGML_Vector2& eGML_Vector2::operator/= (eC_Value fScalar)
{
    if ( fScalar != eC_FromInt(0) ) //by Morten: whats about the check with eC_MINVALUE ?
    {
        eC_Value fInvScalar = eC_ReciQ( fScalar );
        x = eC_Mul( x, fInvScalar );
        y = eC_Mul( y, fInvScalar );
    }
    else
    {
        x = eC_MAXVALUE;	//INFINITY
        y = eC_MAXVALUE;
    }

    return *this;
}

//----------------------------------------------------------------------------
eGML_Vector2 eGML_Vector2::Orthogonal () const
{
    // vector orthogonal to (x,y) is (y,-x)
    eGML_Vector2 kOrtho;
    kOrtho.x = y;
    kOrtho.y = -x;
    return kOrtho;
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector2::Cross (const eGML_Vector2& rkVector) const
{
    return eC_Mul(x, rkVector.y) - eC_Mul(y, rkVector.x);
}

//----------------------------------------------------------------------------
#endif
