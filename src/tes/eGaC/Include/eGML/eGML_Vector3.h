/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Vector3.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:20:55 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_VECTOR3__H_
#define EGML_VECTOR3__H_
//----------------------------------------------------------------------------
#include "eC_Math.h"
#include "eGML_Config.h"

//----------------------------------------------------------------------------
/**
@memo
Basic class for convenient and effective working with 3D coordinates.
@doc
The class eGML_Vector3 provides effective handling of 3D-coordinates for every vector operation. Therefor every coordinate
has to be interpretated as a position vector with point of origin in coordinate (0,0,0). For getting access to the componenents
(\Ref{x}-, \Ref{y}- and \Ref{z}-coordinate) of a vector in a flexible way, you are able to <BR>
1. use the <TT>.x</TT>, <TT>.y</TT>, or <TT>.z</TT> attribut by itself or <BR>
2. use indeces like in a normal array (<TT>[0]</TT> for <TT>x</TT>, <TT>[1]</TT> for <TT>y</TT>, <TT>[2]</TT> for <TT>z</TT>) <BR>
analog to the following display format this documentation is based on: <BR><BR>
\IMG{pictures/vector3_general.gif} <BR><BR>
For getting more information about \Ref{eGML_Vector3} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Vector3_1.cpp]{examples/example_Vector3_1.cpp.html} <BR>
@see eGML_Vector2, eGML_Matrix3
@see Tutorial_eGML
*/
class eGML_Vector3
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
    /// Member access. Allows <TT>v.z</TT> or <TT>v[2]</TT>.
    eC_Value z;

    // special vectors for basic operations

    /// Zero vector (0, 0, 0).
    static eGML_Vector3 ZERO;
    /// Unit vector (1, 0, 0).
    static eGML_Vector3 UNIT_X;
    /// Unit vector (0, 1, 0).
    static eGML_Vector3 UNIT_Y;
    /// Unit vector (0, 0, 1).
    static eGML_Vector3 UNIT_Z;

    // !!! INTERNAL USE ONLY !!!
    static void InitGlobals();

    /** Standard constructor. For effecient construction of large vector arrays - there is no initialisation. */
    inline eGML_Vector3 ();

    /** Constructor. With initialisation of \Ref{x}-, \Ref{y}- and \Ref{z}-component of new vector.
    @param fX X-component of new vector.
    @param fY Y-component of new vector.
    @param fZ Z-component of new vector.
    @see x, y, z */
    inline eGML_Vector3 (eC_Value fX, eC_Value fY, eC_Value fZ);

    /** Constructor. With initialisation of <TT>v[0]</TT> to <TT>v[2]</TT> component
    @param afCoordinate Array containing all three components of new vector.
    @see x, y, z */
    inline eGML_Vector3 (eC_Value afCoordinate[3]);

    /** Copyconstructor.
    @param rkVector Vector to copy from for new vector. */
    inline eGML_Vector3 (const eGML_Vector3& rkVector);

    /** Provides access to one of the components of the vector, as part of an array. There is no checkup if the index makes
    sense. Only the values 0 (for <TT>v.\Ref{x}</TT>) to 2 (for <TT>v.\Ref{z}</TT>) are reasonable.
    @param i Index to indicate if X-, Y- or Z-component shall be returned (only 0 to 2 are defined).
    @return component <TT>v.x</TT> if index was 0, <TT>v.y</TT> if index was 1 and <TT>v.z</TT> if index was 2. All other indices are undefined.
    @see x, y, z */
    inline eC_Value& operator[] (eC_Int i) const;

    /** Provides access to one of the components of the vector, as part of an array. There is no checkup if the index makes
    sense. Only the values 0 (for <TT>v.\Ref{x}</TT>) to 2 (for <TT>v.\Ref{z}</TT>) are reasonable.
    @param i Index to indicate if X-, Y- or Z-component shall be returned (only 0 to 2 are defined).
    @return component <TT>v.x</TT> if index was 0, <TT>v.y</TT> if index was 1 and <TT>v.z</TT> if index was 2. All other indices are undefined.
    @see x, y, z */
    inline eC_Value& operator[] (int i) const;

    /** Cast operator to provide the addresses of the components themselves (not of the object!).
    @return Pointer to <TT>v.x</TT> as the beginning of the array <TT>v[0..2]</TT>.
    @see x, y, z */
    inline operator eC_Value* ();

    // assignment and comparison

    /** Reinitialize this vector with the data of another vector.
    @param rkVector Vector to copy the data from.
    @return Reinitialized vector. */
    inline eGML_Vector3& operator= (const eGML_Vector3& rkVector);

    /** Compares two vectors and checks if all their components are equal.
    @param rkVector Vector to compare with.
    @return Only if all components are equal <B>true</B> will be returned. */
    inline bool operator== (const eGML_Vector3& rkVector) const;

    /** Compares two vectors and checks if one component is not equal.
    @param rkVector Vector to compare with.
    @return Only if all components are equal <B>false</B> will be returned. */
    inline bool operator!= (const eGML_Vector3& rkVector) const;

    // arithmetic operations

    /** Calculates the sum of two vectors.
    @param rkVector Second addend.
    @return Sum of the two vectors. */
    inline eGML_Vector3 operator+ (const eGML_Vector3& rkVector) const;

    /** Calculates the difference between two vectors.
    @param rkVector Subtrahend.
    @return Result of subtraction (difference). */
    inline eGML_Vector3 operator- (const eGML_Vector3& rkVector) const;

    /** Calculates the division of two vectors.
    There is no checkup if the components of the divisor vector is equal zero.
    @param rkVector Divisor vector.
    @return Result of division. */
    inline eGML_Vector3 operator/ (const eGML_Vector3& rkVector) const;

    /** Calculates the multiplication of two vectors.
    @param rkVector Second multiplicator vector.
    @return Result of multiplication. */
    inline eGML_Vector3 operator* (const eGML_Vector3& rkVector) const;

    /** Calculates the multiplication of the vector by a scalar.
    @param fScalar Scalar multiplicator.
    @return Result of multiplication. */
    inline eGML_Vector3 operator* (eC_Value fScalar) const;

    /** Calculates the multiplication of a scalar by another vector.
    So it is possible now to make for example following operation: <BR>
    <TT>v1 = 2 * v2</TT>.
    @param fScalar Scalar multiplicator.
    @param rkVector Second vector multiplicator.
    @return Result of multiplication. */
    inline friend eGML_Vector3 operator* (eC_Value fScalar, const eGML_Vector3& rkVector);

    /** Calculates the division of the vector by a scalar.
    If there is a division by zero, no exception is thrown but the maximum value of <B>eGML</B> is returned.
    @param fScalar Scalar divisor.
    @return Result of division. */
    inline eGML_Vector3 operator/ (eC_Value fScalar) const;

    /** Calculates the inverse vector of this vector.
    That means the resulting vector has got the same {@link Length length} but the opposite direction.
    @return Inverse vector.
    @see Length*/
    inline eGML_Vector3 operator- () const;

    // arithmetic updates

    /** Calculates the sum of two vectors and stores the result.
    @param rkVector Second addend.
    @return Sum of the two vectors. */
    inline eGML_Vector3& operator+= (const eGML_Vector3& rkVector);

    /** Calculates the subtraction of two vectors and stores the result.
    @param rkVector Subtrahend.
    @return Result of subtraction. */
    inline eGML_Vector3& operator-= (const eGML_Vector3& rkVector);

    /** Calculates the division of the vector by a scalar and stores the result.
    If there is a division by zero, no exception is thrown but the maximum value of <B>eGML</B> is returned.
    @param fScalar Scalar divisor.
    @return Result of division. */
    inline eGML_Vector3& operator/= (eC_Value fScalar);

    /** Calculates the multiplication of the vector by a scalar and stores the result.
    @param fScalar Scalar multiplicator.
    @return Result of multiplication. */
    inline eGML_Vector3& operator*= (eC_Value fScalar);

    // vector operations		

    /** Calculates the length (the absolut value) of the vector by using the theorem of Pythagoras. <BR>
    <TT>Length = sqrt(v.x^2 + v.y^2 + v.z^2)</TT>.
    @return Length of the vector.
    @see x, y, z, SquaredLength */
    inline eC_Value Length () const;

    /** Calculates the squared {@link Length length} of the vector. <BR>
    <TT>SquaredLength = v.x^2 + v.y^2 + v.z^2</TT>.
    @return Squared length of the vector.
    @see x, y, z, Length */
    inline eC_Value SquaredLength () const;

    /** Calculates the scalar product of two vectors. <BR>
    <TT>Dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z</TT>.
    @param rkVecor Vector to build scalar product with.
    @return Scalar product of two vectors.
    @see Cross, UnitCross */
    inline eC_Value Dot (const eGML_Vector3& rkVector) const;

    /** Calculates the 3D-cross-product (vector product) of two vectors.
    The result of a 3D-cross-product is a vector that is orthogonal to both used vectors. So the return value of a 3D-cross-product
    is a eGML_Vector3 in contrast to the <TT><B>Cross</B></TT> operation in \Ref{eGML_Vector2} which returns an \Ref{eC_Value}
    as the absolute value. If you calculate the {@link Length length} of the resulting 3D-cross-product you get the surface
    of a parallelogram that is formed by both used vectors.
    @param rkVector Second vector.
    @return 3D-cross-product (means the vector that is orthogonal to both used vectors).
    @see Dot, Length, UnitCross */
    eGML_Vector3 Cross (const eGML_Vector3& rkVector) const;

    /** Calculates the {@link Cross 3D-cross-product} (vector product) of two vectors and {@link Unitize unitizes} it.
    The result of this 3D-cross-product is a vector that is orthogonal to both used vectors and has got the {@link Length length} = 1.
    @param rkVector Second vector.
    @return Unitized 3D-cross-product (means the vector that is orthogonal to both used vectors with the length = 1).
    @see Dot, Length, Unitize, Cross */
    eGML_Vector3 UnitCross (const eGML_Vector3& rkVector) const;

    /** Calculates the unitized vector out of this vector. Therefor the {@link Length length} of the vector has to be calculated.
    If the length of the vector is <I>smaller</I> than the given tolerance, this operation returns the \Ref{UNIT_X} vector.
    There is no checkup if the length of vector equals zero.
    The resulting vector has got the characteristics: length = 1, components: (<TT>cos a</TT>, <TT>cos b</TT>, <TT>cos c</TT>);
    <TT><B>a</B></TT> is the angle between vector and X-axis, <TT><B>b</B></TT> is the angle between vector and Y-axis,
    <TT><B>c</B></TT> is the angle between vector and Z-axis.
    @param fTolerance Precision of checking for correct calculation.
    @return Unitized vector (length = 1, same direction of the original vector) or \Ref{UNIT_X} vector if operation fails.
    @see Length, UNIT_X, UNIT_Y, UNIT_Z, UnitCross */
    eGML_Vector3 Unitize (eC_Value fTolerance = eC_FromFloat(1e-06f));
};

//----------------------------------------------------------------------------
inline eGML_Vector3::eGML_Vector3 ()
{
    // For efficiency in construction of large arrays of vectors, the
    // default constructor does not initialize the vector.
}

//----------------------------------------------------------------------------
inline eGML_Vector3::eGML_Vector3 (eC_Value fX, eC_Value fY, eC_Value fZ)
{
    x = fX;
    y = fY;
    z = fZ;
}

//----------------------------------------------------------------------------
inline eGML_Vector3::eGML_Vector3 (eC_Value afCoordinate[3])
{
    x = afCoordinate[0];
    y = afCoordinate[1];
    z = afCoordinate[2];
}

//----------------------------------------------------------------------------
inline eGML_Vector3::eGML_Vector3 (const eGML_Vector3& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
    z = rkVector.z;
}

//----------------------------------------------------------------------------
inline eC_Value& eGML_Vector3::operator[] (eC_Int i) const
{
    // assert:  0 <= i < 3; x and y are packed into 3*sizeof(eC_Value) bytes
    return (eC_Value&) *(&x + i);
}

//----------------------------------------------------------------------------
inline eC_Value& eGML_Vector3::operator[] (int i) const
{
    // assert:  0 <= i < 3; x and y are packed into 3*sizeof(eC_Value) bytes
    return (eC_Value&) *(&x + i);
}

//----------------------------------------------------------------------------
inline eGML_Vector3::operator eC_Value* ()
{
    return &x;
}

//----------------------------------------------------------------------------
inline eGML_Vector3& eGML_Vector3::operator= (const eGML_Vector3& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
    z = rkVector.z;
    return *this;
}

//----------------------------------------------------------------------------
inline bool eGML_Vector3::operator== (const eGML_Vector3& rkVector) const
{
    return ( x == rkVector.x && y == rkVector.y  && z == rkVector.z );
}

//----------------------------------------------------------------------------
inline bool eGML_Vector3::operator!= (const eGML_Vector3& rkVector) const
{
    return ( x != rkVector.x || y != rkVector.y  || z != rkVector.z );
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator+ (const eGML_Vector3& rkVector) const
{
    eGML_Vector3 kSum;
    kSum.x = x + rkVector.x;
    kSum.y = y + rkVector.y;
    kSum.z = z + rkVector.z;
    return kSum;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator- (const eGML_Vector3& rkVector) const
{
    eGML_Vector3 kDiff;
    kDiff.x = x - rkVector.x;
    kDiff.y = y - rkVector.y;
    kDiff.z = z - rkVector.z;
    return kDiff;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator/ (const eGML_Vector3& rkVector) const
{
    eGML_Vector3 kDiv;
    kDiv.x = eC_Div(x , rkVector.x); // by Morten: check for division by zero or eC_MINVALUE?
    kDiv.y = eC_Div(y , rkVector.y); // by Morten: check for division by zero or eC_MINVALUE?
    kDiv.z = eC_Div(z , rkVector.z); // by Morten: check for division by zero or eC_MINVALUE?
    return kDiv;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator* (const eGML_Vector3& rkVector) const
{
    eGML_Vector3 kDiv;
    kDiv.x = eC_Mul(x , rkVector.x);
    kDiv.y = eC_Mul(y , rkVector.y);
    kDiv.z = eC_Mul(z , rkVector.z);
    return kDiv;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator* (eC_Value fScalar) const
{
    eGML_Vector3 kProd;
    kProd.x = eC_Mul(fScalar, x);
    kProd.y = eC_Mul(fScalar, y);
    kProd.z = eC_Mul(fScalar, z);
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator- () const
{
    eGML_Vector3 kNeg;
    kNeg.x = -x;
    kNeg.y = -y;
    kNeg.z = -z;
    return kNeg;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 operator* (eC_Value fScalar, const eGML_Vector3& rkVector)
{
    eGML_Vector3 kProd;
    kProd.x = eC_Mul(fScalar, rkVector.x);
    kProd.y = eC_Mul(fScalar, rkVector.y);
    kProd.z = eC_Mul(fScalar, rkVector.z);
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector3& eGML_Vector3::operator+= (const eGML_Vector3& rkVector)
{
    x += rkVector.x;
    y += rkVector.y;
    z += rkVector.z;
    return *this;
}

//----------------------------------------------------------------------------
inline eGML_Vector3& eGML_Vector3::operator-= (const eGML_Vector3& rkVector)
{
    x -= rkVector.x;
    y -= rkVector.y;
    z -= rkVector.z;
    return *this;
}

//----------------------------------------------------------------------------
inline eGML_Vector3& eGML_Vector3::operator*= (eC_Value fScalar)
{
    x = eC_Mul(x, fScalar);
    y = eC_Mul(y, fScalar);
    z = eC_Mul(z, fScalar);
    return *this;
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector3::SquaredLength () const
{
    return eC_Mul(x, x) + eC_Mul(y, y) + eC_Mul(z, z);
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector3::Length () const
{
    return eC_Len3( x, y, z );
}

//----------------------------------------------------------------------------
inline eC_Value eGML_Vector3::Dot (const eGML_Vector3& rkVector) const
{
    return eC_Mul(x, rkVector.x) + eC_Mul(y, rkVector.y) + eC_Mul(z, rkVector.z);
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Vector3::operator/ (eC_Value fScalar) const
{
    eGML_Vector3 kQuot;

    if ( fScalar != eC_FromInt(0) ) //by Morten: whats about the check with eC_MINVALUE ?
    {
        eC_Value fInvScalar = eC_ReciQ( fScalar );
        kQuot.x = eC_Mul( fInvScalar, x );
        kQuot.y = eC_Mul( fInvScalar, y );
        kQuot.z = eC_Mul( fInvScalar, z );
        return kQuot;
    }
    else
    {
      return eGML_Vector3(eC_MAXVALUE , eC_MAXVALUE, eC_MAXVALUE); //INFINITY
    }
}

//----------------------------------------------------------------------------
inline eGML_Vector3& eGML_Vector3::operator/= (eC_Value fScalar)
{
    if ( fScalar != eC_FromInt(0) ) //by Morten: whats about the check with eC_MINVALUE ?
    {
        eC_Value fInvScalar = eC_ReciQ( fScalar );
        x = eC_Mul( x, fInvScalar );
        y = eC_Mul( y, fInvScalar );
        z = eC_Mul( z, fInvScalar );
    }
    else
    {
        x = eC_MAXVALUE;	//INFINITY
        y = eC_MAXVALUE;
        z = eC_MAXVALUE;
    }

    return *this;
}

//----------------------------------------------------------------------------
#endif
