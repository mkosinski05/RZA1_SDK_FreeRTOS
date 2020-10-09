/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Matrix2.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:17:45 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_MATRIX2__H_
#define EGML_MATRIX2__H_
//----------------------------------------------------------------------------
#include "eC_Math.h"
#include "eGML_Vector2.h"
#include "eDBG.h"
#include "eGML_Config.h"

//----------------------------------------------------------------------------
/**
@memo
Basic class for convenient and effective working with 2D matrices.
@doc
The class eGML_Matrix2 provides effective handling of {@link eGML_Vector2 2D-vector}-transformation by using matrix operations. Therefor
it is possible to effect 2D-matrix-operations with other eGML_Matrix2 objects, eGML_Vector2 objects and scalars. Vectors in this context have
to be interpretated as position vectors with point of origin in coordinate (0,0) and their <TT><B>x</B></TT>- and <TT><B>y</B></TT>-coodinate.
To get more information about vectors see \Ref{eGML_Vector2} or \Ref{eGML_Vector3}. <BR>
Similar to the usage of vector attributs you are able to get access to the coefficients of a matrix directly, but only
by using indices: <BR><BR>
\IMG{pictures/matrix2_general.gif} <BR><BR>
on the left side M2 is the symbol of a 2D-matrix, in the middle you can see the mathematical display format of a matrix with its coefficients
and their indices in the typical way. On the right side the coefficients are part of a 2x2-array. The indices show their position in the array how
it is used in the whole <B>eGML</B> and in matrix objects. The following documentation refers to this display format. <BR>
For getting more information about \Ref{eGML_Matrix2} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Matrix2_1.cpp]{examples/example_Matrix2_1.cpp.html} <BR>
@see eGML_Vector2, eGML_Matrix3
@see Tutorial_eGML
*/
class eGML_Matrix2
{
public:

    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

    // special vectors for basic operations
    
    /* !!! INTERNAL USE ONLY !!!
    Minimum value of eC_Value type. */
    static eC_Value EPSILON;
    /// Zero matrix. All coefficents are 0.
    static eGML_Matrix2 ZERO;
    /// Unit or identity matrix. <TT>m[0][0]</TT> and <TT>m[1][1]</TT> are 1, the other coefficients are 0.
    static eGML_Matrix2 IDENTITY;

    // !!! INTERNAL USE ONLY !!!
    static void InitGlobals();

    // construction

    /** Standard constructor. For efficient construction of large matrix arrays, there is no initialisation. */
    eGML_Matrix2 ();

    /** Constructor. With initialisation of all coefficients by a row of parameters.
    @param vEntry00 Coefficient <TT>m[0][0]</TT> of new matrix.
    @param vEntry01 Coefficient <TT>m[0][1]</TT> of new matrix.
    @param vEntry10 Coefficient <TT>m[1][0]</TT> of new matrix.
    @param vEntry11 Coefficient <TT>m[1][1]</TT> of new matrix. */
    eGML_Matrix2 (eC_Value vEntry00, eC_Value vEntry01, eC_Value vEntry10, eC_Value vEntry11);

    /** Constructor. With initialisation of all coefficients by an array.
    @param aavEntry Array containing the coefficients of new matrix. */
    eGML_Matrix2 (const eC_Value aavEntry[2][2]);

    /** Copyconstructor.
    @param rkMatrix Matrix to copy from for new matrix. */
    eGML_Matrix2 (const eGML_Matrix2& rkMatrix);

    /** Provides access to each coefficient of the matrix. Therefor this operator returns the pointer of the chosen <I>row</I> of the
    matrix; evaluating a reasonable index of this pointer provides the <I>column</I>. So member access by <I>m[row][column]</I>
    is possible. But there is no checkup if the index makes sense. Only the values 0 and 1 for row and column are reasonable.
    @param iRow Row index of an coefficient, only 0 or 1 are defined.
    @return Pointer to the chosen row of the matrix. */
    eC_Value* operator[] (eC_Int iRow) const;

    /** Provides access to each coefficient of the matrix. Therefor this operator returns the pointer of the chosen <I>row</I> of the
    matrix; evaluating a reasonable index of this pointer provides the <I>column</I>. So member access by <I>m[row][column]</I>
    is possible. But there is no checkup if the index makes sense. Only the values 0 and 1 for row and column are reasonable.
    @param iRow Row index of an coefficient, only 0 or 1 are defined.
    @return Pointer to the chosen row of the matrix. */
    eC_Value* operator[] (int iRow) const;

    /** Cast operator to provide the addresses of the components - the array of coefficients themselves (not of the object!).
    @return Pointer to the coefficient array. */
    operator eC_Value* ();

    /** Provides access to each column of the matrix as a \Ref{eGML_Vector2}.
    @param iCol Column index of the matrix (There is a checkup of the index: only 0 or 1 are allowed, if an error occurs there will be an assertion).
    @return Coefficents in the chosen column in form of a eGML_Vector2.
    @see eGML_Vector2 */
    eGML_Vector2 GetColumn (eC_Int iCol) const;

    /** Reinitialises this matrix with the data of another matrix.
    @param rkMatrix Matrix to copy the data from.
    @return Reinitialised matrix. */
    eGML_Matrix2& operator= (const eGML_Matrix2& rkMatrix);

    /** Compares two matrices and checks if all their coefficients are equal.
    @param rkMatrix Matrix to compare with.
    @return Only if all coefficients are equal <B>true</B> will be returned. */
    eC_Bool operator== (const eGML_Matrix2& rkMatrix) const;

    /** Compares two matrices and checks if one coefficient is not equal.
    @param rkMatrix Matrix to compare with.
    @return Only if all coefficients are equal <B>false</B> will be returned. */
    eC_Bool operator!= (const eGML_Matrix2& rkMatrix) const;

    // arithmetic operations

    /** Calculates the sum of two matrices.
    @param rkMatrix Second addend.
    @return Sum of the two matrices. */
    eGML_Matrix2 operator+ (const eGML_Matrix2& rkMatrix) const;

    /** Calculates the difference between two matrices.
    @param rkMatrix Subtrahend.
    @return Result of subtraction (difference). */
    eGML_Matrix2 operator- (const eGML_Matrix2& rkMatrix) const;

    /** Calculates the multiplication of two matrices.
    @param rkMatrix Second multiplicator matrix.
    @return Result of multiplication. */
    eGML_Matrix2 operator* (const eGML_Matrix2& rkMatrix) const;

    /** Calculates the multiplication of the matrix by a scalar.
    @param vScalar Scalar multiplicator.
    @return Result of multiplication. */
    eGML_Matrix2 operator* (eC_Value vScalar) const;

    /** Calculates the multiplication of the matrix by an \Ref{eGML_Vector2}.
    @param rkVector Vector multiplicator.
    @return Result of multiplication - an eGML_Vector2.
    @see eGML_Vector2 */
    eGML_Vector2 operator* (const eGML_Vector2& rkVector) const;

    /** Calculates the multiplication of an \Ref{eGML_Vector2} by another matrix.
    So it is possible now to make for example following operation: <BR>
    <TT>M1 = V * M2</TT>.
    @param rkVector Vector multiplicator.
    @param rkMatrix Matrix multiplicator.
    @return Result of multiplication - an eGML_Vector2.
    @see eGML_Vector2 */
    friend eGML_Vector2 operator* (const eGML_Vector2& rkVector, const eGML_Matrix2& rkMatrix);

    /** Calculates the multiplication of a scalar by another matrix.
    So it is possible now to make for example following operation: <BR>
    <TT>M1 = 2 * M2</TT>.
    @param vScalar Scalar multiplicator.
    @param rkMatrix Matrix multiplicator.
    @return Result of multiplication. */
    friend eGML_Matrix2 operator* (eC_Value vScalar, const eGML_Matrix2& rkMatrix);

    /** Calculates the negative matrix of this matrix. This operation is similar to the multiplication of this matrix with the scalar '-1'.
    @return Negative matrix. */
    eGML_Matrix2 operator- () const;

    // vector operations, utilities

    /** Calculates the transpose matrix of this matrix. Therefor all columns and rows are interchanged.
    @return Transpose matrix.
    @see Inverse, ToAngle */
    eGML_Matrix2 Transpose () const;

    /** Calculates the inverse matrix of another matrix. Therefor the determinant of that matrix has to be calculated.
    If the determinant is smaller than the given tolerance, this method returns <B>false</B>.
    @param rkInverse Matrix to calculate the inverse from.
    @param vTolerance Precision of checking for correct calculation.
    @return If the derterminant of the matrix is <I>smaller</I> than the given tolerance <B>false</B> will be returned.
    @see Inverse, Transpose */
    eC_Bool Inverse (eGML_Matrix2& rkInverse, eC_Value vTolerance = eC_FromFloat(1e-06)) const;

    /** Calculates the inverse matrix of this matrix. Therefor the determinant of the matrix has to be calculated.
    If the determinant is smaller than the given tolerance, this method returns the \Ref{ZERO} matrix.
    @param vTolerance Precision of checking for correct calculation.
    @return Inverse matrix or \Ref{ZERO} matrix if the determinant is <I>smaller</I> than the given tolerance.
    @see Inverse, Transpose, ZERO */
    eGML_Matrix2 Inverse (eC_Value vTolerance = eC_FromFloat(1e-06)) const;
    
    /** Calculates the radians value of the rotation angle of this matrix. A rotation matrix with an angle <I>delta</I> has got the form: <BR><BR>
    \IMG{pictures/matrix2_rotation.gif} <BR><BR>
    Therefor the matrix has to be orthonormal, that means <BR>
    1. its determinant is 1 <BR>
    2. the multiplication of it and its {@link Transpose transpose} results the \Ref{IDENTITY} matrix.
    @param rvRadians Contains the resulting radians value of the angle of this rotation matrix.
    @see FromAngle, Transpose, IDENTITY */
    void ToAngle (eC_Value& rvRadians) const;

    /** Calculates the rotation matrix out of a given angle and stores the result. The angle value is in radians.
    @param rvRadians Radians value of the angle.
    @see ToAngle */
    void FromAngle (eC_Value vRadians);

protected:

    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    // member access.
    eC_Value m_aafEntry[2][2];
};

//----------------------------------------------------------------------------
inline eGML_Matrix2::eGML_Matrix2 ()
{
    // For efficiency reasons, do not initialise matrix.
}

//----------------------------------------------------------------------------
inline eGML_Matrix2::eGML_Matrix2 (const eC_Value aafEntry[2][2])
{
    memcpy(m_aafEntry, aafEntry, 4*sizeof(eC_Value));
}

//----------------------------------------------------------------------------
inline eGML_Matrix2::eGML_Matrix2 (const eGML_Matrix2& rkMatrix)
{
    memcpy(m_aafEntry, rkMatrix.m_aafEntry, 4*sizeof(eC_Value));
}

//----------------------------------------------------------------------------
inline eGML_Matrix2::eGML_Matrix2 (eC_Value fEntry00, eC_Value fEntry01,
                                   eC_Value fEntry10, eC_Value fEntry11)
{
    m_aafEntry[0][0] = fEntry00;
    m_aafEntry[0][1] = fEntry01;
    m_aafEntry[1][0] = fEntry10;
    m_aafEntry[1][1] = fEntry11;
}

//----------------------------------------------------------------------------
inline eC_Value* eGML_Matrix2::operator[] (eC_Int iRow) const
{
    return (eC_Value*)&m_aafEntry[iRow][0];
}

//----------------------------------------------------------------------------
inline eC_Value* eGML_Matrix2::operator[] (int iRow) const
{
    return (eC_Value*)&m_aafEntry[iRow][0];
}

//----------------------------------------------------------------------------
inline eGML_Matrix2::operator eC_Value* ()
{
    return &m_aafEntry[0][0];
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Matrix2::GetColumn (eC_Int iCol) const
{
    eDBG_ASSERT( 0 <= iCol && iCol < 2 , "Column index out of bounds" );
    return eGML_Vector2(m_aafEntry[0][iCol], m_aafEntry[1][iCol]);
}

//----------------------------------------------------------------------------
inline eGML_Matrix2& eGML_Matrix2::operator= (const eGML_Matrix2& rkMatrix)
{
    memcpy(m_aafEntry, rkMatrix.m_aafEntry, 4*sizeof(eC_Value));
    return *this;
}

//----------------------------------------------------------------------------
inline eC_Bool eGML_Matrix2::operator== (const eGML_Matrix2& rkMatrix) const
{
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
        {
            if ( m_aafEntry[iRow][iCol] != rkMatrix.m_aafEntry[iRow][iCol] )
                return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------
inline eC_Bool eGML_Matrix2::operator!= (const eGML_Matrix2& rkMatrix) const
{
    return !operator==(rkMatrix);
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 eGML_Matrix2::operator+ (const eGML_Matrix2& rkMatrix) const
{
    eGML_Matrix2 kSum;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
        {
            kSum.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] +
                                          rkMatrix.m_aafEntry[iRow][iCol];
        }
    }
    return kSum;
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 eGML_Matrix2::operator- (const eGML_Matrix2& rkMatrix) const
{
    eGML_Matrix2 kDiff;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
        {
            kDiff.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] -
                                           rkMatrix.m_aafEntry[iRow][iCol];
        }
    }
    return kDiff;
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 eGML_Matrix2::operator* (const eGML_Matrix2& rkMatrix) const
{
    eGML_Matrix2 kProd;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
        {
            kProd.m_aafEntry[iRow][iCol] =
              eC_Mul(m_aafEntry[iRow][0], rkMatrix.m_aafEntry[0][iCol]) +
              eC_Mul(m_aafEntry[iRow][1], rkMatrix.m_aafEntry[1][iCol]);
        }
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector2 eGML_Matrix2::operator* (const eGML_Vector2& rkVector) const
{
   return eGML_Vector2( eC_Mul(m_aafEntry[0][0], rkVector[0]) +
                        eC_Mul(m_aafEntry[0][1], rkVector[1]),
						eC_Mul(m_aafEntry[1][0], rkVector[0]) +
                        eC_Mul(m_aafEntry[1][1], rkVector[1]) );
}

//----------------------------------------------------------------------------
inline eGML_Vector2 operator* (const eGML_Vector2& rkVector, const eGML_Matrix2& rkMatrix)
{
    return eGML_Vector2( eC_Mul(rkVector[0], rkMatrix.m_aafEntry[0][0]) + 
		                 eC_Mul(rkVector[1], rkMatrix.m_aafEntry[1][0]),
						 eC_Mul(rkVector[0], rkMatrix.m_aafEntry[0][1]) + 
						 eC_Mul(rkVector[1], rkMatrix.m_aafEntry[1][1]) );
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 eGML_Matrix2::operator- () const
{
    eGML_Matrix2 kNeg;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
            kNeg[iRow][iCol] = -m_aafEntry[iRow][iCol];
    }
    return kNeg;
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 eGML_Matrix2::operator* (eC_Value fScalar) const
{
    eGML_Matrix2 kProd;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
            kProd[iRow][iCol] = eC_Mul(fScalar, m_aafEntry[iRow][iCol]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Matrix2 operator* (eC_Value fScalar, const eGML_Matrix2& rkMatrix)
{
    eGML_Matrix2 kProd;
    for (eC_Int iRow = 0; iRow < 2; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 2; iCol++)
            kProd[iRow][iCol] = eC_Mul(fScalar, rkMatrix.m_aafEntry[iRow][iCol]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
#endif
