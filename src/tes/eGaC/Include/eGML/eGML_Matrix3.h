/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Matrix3.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:17:55 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_MATRIX3__H_
#define EGML_MATRIX3__H_
//----------------------------------------------------------------------------
#include "eC_Math.h"
#include "eGML_Vector3.h"
#include "eDBG.h"
#include <stdlib.h>
#include "eGML_Config.h"
#include "eGML_Matrix2.h"

//----------------------------------------------------------------------------
/**
@memo
Basic class for convenient and effective working with 3D matrices.
@doc
The class eGML_Matrix3 provides effective handling of {@link eGML_Vector3 3D-vector}-transformation by using matrix operations. Therefor
it is possible to effect 3D-matrix-operations with other eGML_Matrix3 objects, eGML_Vector3 objects and scalars. Vectors in this context have
to be interpretated as position vectors with point of origin in coordinate (0,0,0) and their <TT><B>x</B></TT>-, <TT><B>y</B></TT>- and
<TT><B>z</B></TT>-coodinate. To get more information about vectors see \Ref{eGML_Vector2} or \Ref{eGML_Vector3}. <BR>
Similar to the usage of vector attributs you are able to get access to the coefficients of a matrix directly, but only
by using indices: <BR><BR>
\IMG{pictures/matrix3_general.gif} <BR><BR>
on the left side M3 is the symbol of a 3D-matrix, in the middle you can see the mathematical display format of a matrix with its coefficients
and their indices in the typical way. On the right side the coefficients are part of a 3x3-array. The indices show their position in the array how
it is used in the whole <B>eGML</B> and in matrix objects (the following documentation refers to this display format). <BR>
Note that the (x,y,z) coordinate system is assumed to be right-handed, so coordinate axis rotation matrices are of the form <BR><BR>
\IMG{pictures/matrix3_rotation_X.gif} <BR><BR>
where <I>delta</I> is > 0 indicates a counterclockwise rotation in the yz-plane, <BR><BR>
\IMG{pictures/matrix3_rotation_Y.gif} <BR><BR>
where <I>delta</I> is > 0 indicates a counterclockwise rotation in the zx-plane and <BR><BR>
\IMG{pictures/matrix3_rotation_Z.gif} <BR><BR>
where <I>delta</I> is > 0 indicates a counterclockwise rotation in the xy-plane. <BR><BR>
For getting more information about \Ref{eGML_Matrix3} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Matrix3_1.cpp]{examples/example_Matrix3_1.cpp.html} <BR>
@see eGML_Vector3, eGML_Matrix2
@see Tutorial_eGML
*/
class eGML_Matrix3
{
public:

    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

    // special vectors for basic operations
    
    /* !!! INTERNAL USE ONLY !!!
    Minimum value of eC_Value. */
    static eC_Value EPSILON;
    /// Zero matrix. All coefficents are 0.
    static eGML_Matrix3 ZERO;
    /// Unit or identity matrix. <TT>m[0][0]</TT>, <TT>m[1][1]</TT> and <TT>m[2][2]</TT> are 1, the other coefficients are 0.
    static eGML_Matrix3 IDENTITY;

    // !!! INTERNAL USE ONLY !!!
    static void InitGlobals();

   // construction

    /** Standard constructor. For efficient construction of large matrix arrays, there is no initialisation. */
    eGML_Matrix3 ();

    /** Constructor. With initialisation of all coefficients by a row of parameters.
    @param vEntry00 Coefficient <TT>m[0][0]</TT> of new matrix.
    @param vEntry01 Coefficient <TT>m[0][1]</TT> of new matrix.
    @param vEntry02 Coefficient <TT>m[0][2]</TT> of new matrix.
    @param vEntry10 Coefficient <TT>m[1][0]</TT> of new matrix.
    @param vEntry11 Coefficient <TT>m[1][1]</TT> of new matrix.
    @param vEntry12 Coefficient <TT>m[1][2]</TT> of new matrix.
    @param vEntry20 Coefficient <TT>m[2][0]</TT> of new matrix.
    @param vEntry21 Coefficient <TT>m[2][1]</TT> of new matrix.
    @param vEntry22 Coefficient <TT>m[2][2]</TT> of new matrix. */
    eGML_Matrix3 (eC_Value vEntry00, eC_Value vEntry01, eC_Value vEntry02,
                  eC_Value vEntry10, eC_Value vEntry11, eC_Value vEntry12,
                  eC_Value vEntry20, eC_Value vEntry21, eC_Value vEntry22);

    /** Constructor. With initialisation of all coefficients by an array.
    @param aavEntry Array containing the coefficients of new matrix. */
    eGML_Matrix3 (const eC_Value aavEntry[3][3]);

    /** Constructor. With initialisation of all coefficients by three \Ref{eGML_Vector3}s.
    @param rkLeft Vector for the left column of the matrix.
    @param rkUp   Vector for the column in the middle of the matrix.
    @param rkDir  Vector for the right column of the matrix. */
    eGML_Matrix3 (const eGML_Vector3& rkLeft, const eGML_Vector3& rkUp, const eGML_Vector3& rkDir);

	/** Constructor. Top left 2x2 submatrix is initialized with values from rkMatrix. 
	Additional row/column is initialized with (0,0,1)
    @param rkMatrix existing eGML_Matrix2. */
	eGML_Matrix3 (const eGML_Matrix2& rkMatrix);

    /** Copyconstructor.
    @param rkMatrix Matrix to copy from for new matrix. */
    eGML_Matrix3 (const eGML_Matrix3& rkMatrix);

    /** Provides access to each coefficient of the matrix. Therefor this operator returns the pointer of the chosen <I>row</I> of the
    matrix; evaluating a reasonable index of this pointer provides the <I>column</I>. So member access by <I>m[row][column]</I>
    is possible. But there is no checkup if the index makes sense. Only the values 0 to 2 for row and column are reasonable.
    @param iRow Row index of an coefficient, only 0 to 2 are defined.
    @return Pointer to the chosen row of the matrix. */
    eC_Value* operator[] (eC_Int iRow) const;

    /** Provides access to each coefficient of the matrix. Therefor this operator returns the pointer of the chosen <I>row</I> of the
    matrix; evaluating a reasonable index of this pointer provides the <I>column</I>. So member access by <I>m[row][column]</I>
    is possible. But there is no checkup if the index makes sense. Only the values 0 to 2 for row and column are reasonable.
    @param iRow Row index of an coefficient, only 0 to 2 are defined.
    @return Pointer to the chosen row of the matrix. */
    eC_Value* operator[] (int iRow) const;

    /** Cast operator to provide the addresses of the components - the array of coefficient themselves (not of the object!).
    @return Pointer to the coefficient array. */
    operator eC_Value* ();

    /** Provides access to each column of the matrix as an \Ref{eGML_Vector3}.
    @param iCol Column index of the matrix (There is a checkup of the index: only 0, 1 or 2 are allowed).
    @return Coefficents in the chosen column as an eGML_Vector3.
    @see eGML_Vector3 */
    eGML_Vector3 GetColumn (eC_Int iCol) const;

    /** Provides access to each column of the matrix as an \Ref{eGML_Vector3}.
    @param iCol Column index of the matrix (There is a checkup of the index: only 0, 1 or 2 are allowed).
    @return Coefficents in the chosen column as an eGML_Vector3.
    @see eGML_Vector3 */
    eGML_Vector3 GetColumn (int iCol) const;

    /** Provides access to each row of the matrix as an \Ref{eGML_Vector3}.
    @param iRow Row index of the matrix (There is a checkup of the index: only 0, 1 or 2 are allowed).
    @return Coefficents in the chosen row as an eGML_Vector3.
    @see eGML_Vector3 */
    eGML_Vector3 GetRow (eC_Int iRow) const;

    /** Provides access to each row of the matrix as an \Ref{eGML_Vector3}.
    @param iRow Row index of the matrix (There is a checkup of the index: only 0, 1 or 2 are allowed).
    @return Coefficents in the chosen row as an eGML_Vector3.
    @see eGML_Vector3 */
    eGML_Vector3 GetRow (int iRow) const;

    /** Reinitialise this matrix with the data of another matrix.
    @param rkMatrix Matrix to copy the data from.
    @return Reinitialised matrix. */
    eGML_Matrix3& operator= (const eGML_Matrix3& rkMatrix);

    /** Compares two matrices and checks if all their coefficients are equal.
    @param rkMatrix Matrix to compare with.
    @return Only if all coefficients are equal <B>true</B> will be returned. */
    eC_Bool operator== (const eGML_Matrix3& rkMatrix) const;

    /** Compares two matrices and checks if one coefficient is not equal.
    @param rkMatrix Matrix to compare with.
    @return Only if all coefficients are equal <B>false</B> will be returned. */
    eC_Bool operator!= (const eGML_Matrix3& rkMatrix) const;

    // arithmetic operations

    /** Calculates the sum of two matrices.
    @param rkMatrix Second addend.
    @return Sum of the two matrices. */
    eGML_Matrix3 operator+ (const eGML_Matrix3& rkMatrix) const;

    /** Calculates the difference between two matrices.
    @param rkMatrix Subtrahend.
    @return Result of subtraction (difference). */
    eGML_Matrix3 operator- (const eGML_Matrix3& rkMatrix) const;

    /** Calculates the multiplication of two matrices.
    @param rkMatrix Second multiplicator matrix.
    @return Result of multiplication. */
    eGML_Matrix3 operator* (const eGML_Matrix3& rkMatrix) const;

    /** Calculates the multiplication of the matrix by a scalar.
    @param vScalar Scalar multiplicator.
    @return Result of multiplication. */
    eGML_Matrix3 operator* (eC_Value vScalar) const;

    /** Calculates the multiplication of the matrix by an \Ref{eGML_Vector3}.
    @param rkVector Vector multiplicator.
    @return Result of multiplication - an eGML_Vector3.
    @see eGML_Vector3 */
    eGML_Vector3 operator* (const eGML_Vector3& rkVector) const;

    /** Calculates the multiplication of an \Ref{eGML_Vector3} by another matrix.
    So it is possible now to make for example following operation: <BR>
    <TT>M1 = V * M2</TT>.
    @param rkVector Vector multiplicator.
    @param rkMatrix Matrix multiplicator.
    @return Result of multiplication - an eGML_Vector3.
    @see eGML_Vector3 */
    friend eGML_Vector3 operator* (const eGML_Vector3& rkVector, const eGML_Matrix3& rkMatrix);

    /** Calculates the multiplication of a scalar by another matrix.
    So it is possible now to make for example following operation: <BR>
    <TT>M1 = 2 * M2</TT>.
    @param vScalar Scalar multiplicator.
    @param rkMatrix Matrix multiplicator.
    @return Result of multiplication. */
    friend eGML_Matrix3 operator* (eC_Value vScalar, const eGML_Matrix3& rkMatrix);

    /** Calculates the negative matrix of this matrix. This operation is similar to the multiplication of this matrix with the scalar '-1'.
    @return Negative matrix. */
    eGML_Matrix3 operator- () const;

    // vector operations, utilities

    /** Calculates the transpose matrix of this matrix. Therefor all columns and rows are interchanged.
    @return Transpose matrix.
    @see Inverse */
    eGML_Matrix3 Transpose () const;
    
    /** Calculates the inverse matrix of another matrix. Therefor the determinant of that matrix has to be calculated.
    If the determinant is smaller than the given tolerance, this method returns <B>false</B>.
    @param rkInverse Matrix to calculate the inverse from.
    @param vTolerance Precision of checking for correct calculation.
    @return If the derterminant of the matrix is <I>smaller</I> than the given tolerance <B>false</B> will be returned.
    @see Inverse, Transpose */
    eC_Bool Inverse (eGML_Matrix3& rkInverse, eC_Value vTolerance = eC_FromFloat(1e-06)) const;

    /** Calculates the inverse matrix of this matrix. Therefor the determinant of the matrix has to be calculated.
    If the determinant is smaller than the given tolerance, this method returns the \Ref{ZERO} matrix.
    @param vTolerance Precision of checking for correct calculation.
    @return Inverse matrix or \Ref{ZERO} matrix if the determinant is <I>smaller</I> than the given tolerance.
    @see Inverse, Transpose, ZERO */
    eGML_Matrix3 Inverse (eC_Value vTolerance = eC_FromFloat(1e-06)) const;

    /** Provides axis and radiants of this rotation matrix. 
    @param rkAxis Resulting axis vector (return value).
    @param rvRadians Resulting angle in radians (return value).
    @see FromAxisAngle, ToEulerAnglesXYZ */
    void ToAxisAngle (eGML_Vector3& rkAxis, eC_Value& rvRadians) const;

    /** Creates rotation matrix out of an axis and radiant.
    @param rkAxis Axis vector to calculate with.
    @param rvRadians Radiant angle.
    @see ToAxisAngle, FromEulerAnglesXYZ */
    void FromAxisAngle (const eGML_Vector3& rkAxis, eC_Value vRadians);

    /** Provides XYZ-based euler angles in radiant.
    @param rfXAngle Resulting X-angle (return value).
    @param rfYAngle Resulting Y-angle (return value).
    @param rfZAngle Resulting Z-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
    eC_Bool ToEulerAnglesXYZ (eC_Value& rvXAngle, eC_Value& rvYAngle, eC_Value& rvZAngle) const;

    /** Provides XZY-based euler angles in radiant.
    @param rfXAngle Resulting X-angle (return value).
    @param rfZAngle Resulting Z-angle (return value).
    @param rfYAngle Resulting Y-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
    eC_Bool ToEulerAnglesXZY (eC_Value& rvXAngle, eC_Value& rvZAngle, eC_Value& rvYRAngle) const;

    /** Provides YXZ-based euler angles in radiant.
    @param rfYAngle Resulting Y-angle (return value).
    @param rfXAngle Resulting X-angle (return value).
    @param rfZAngle Resulting Z-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYZX, ToEulerAnglesZXY, ToEulerAnglesZYX */
    eC_Bool ToEulerAnglesYXZ (eC_Value& rfYAngle, eC_Value& rfXAngle, eC_Value& rfZAngle) const;

    /** Provides YZX-based euler angles in radiant.
    @param rfYAngle Resulting Y-angle (return value).
    @param rfZAngle Resulting Z-angle (return value).
    @param rfXAngle Resulting X-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesZXY, ToEulerAnglesZYX */
    eC_Bool ToEulerAnglesYZX (eC_Value& rvYAngle, eC_Value& rvZAngle, eC_Value& rvXAngle) const;

    /** Provides ZXY-based euler angles in radiant.
    @param rfZAngle Resulting Z-angle (return value).
    @param rfXAngle Resulting X-angle (return value).
    @param rfYAngle Resulting Y-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZYX */
    eC_Bool ToEulerAnglesZXY (eC_Value& rvZAngle, eC_Value& rvXAngle, eC_Value& rvYAngle) const;

    /** Provides ZYX-based euler angles in radiant.
    @param rfZAngle Resulting Z-angle (return value).
    @param rfYAngle Resulting Y-angle (return value).
    @param rfXAngle Resulting X-angle (return value).
    @return Only if the result is unique <B>true</B> will be returned.
    @see FromEulerAnglesXYZ, ToEulerAnglesXYZ, ToEulerAnglesXZY, ToEulerAnglesYXZ, ToEulerAnglesYZX, ToEulerAnglesZXY */
    eC_Bool ToEulerAnglesZYX (eC_Value& rfYAngle, eC_Value& rfPAngle, eC_Value& rfRAngle) const;

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mx*My*Mz.
    @param vXAngle X-angle for rotation matrix.
    @param vYAngle Y-angle for rotation matrix.
    @param vZAngle Z-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
    void FromEulerAnglesXYZ (eC_Value vXAngle, eC_Value vYAngle, eC_Value vZAngle);

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mx*Mz*My.
    @param vXAngle X-angle for rotation matrix.
    @param vZAngle Z-angle for rotation matrix.
    @param vYAngle Y-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
    void FromEulerAnglesXZY (eC_Value vXAngle, eC_Value vZAngle, eC_Value vYAngle);

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together My*Mx*Mz.
    @param vYAngle Y-angle for rotation matrix.
    @param vXAngle X-angle for rotation matrix.
    @param vZAngle Z-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYZX, FromEulerAnglesZXY, FromEulerAnglesZYX*/
    void FromEulerAnglesYXZ (eC_Value vYAngle, eC_Value vXAngle, eC_Value vZAngle);

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together My*Mz*Mx.
    @param vYAngle Y-angle for rotation matrix.
    @param vZAngle Z-angle for rotation matrix.
    @param vXAngle X-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesZXY, FromEulerAnglesZYX*/
    void FromEulerAnglesYZX (eC_Value vYAngle, eC_Value vZAngle, eC_Value vXAngle);

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mz*Mx*My.
    @param vZAngle Z-angle for rotation matrix.
    @param vXAngle X-angle for rotation matrix.
    @param vYAngle Y-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZYX*/
    void FromEulerAnglesZXY (eC_Value vZAngle, eC_Value vXAngle, eC_Value vYAngle);

    /** Creates rotation matrix out of euler angles. Internally three matrices will be created and multiplied together Mz*My*Mx.
    @param vZAngle Z-angle for rotation matrix.
    @param vYAngle Y-angle for rotation matrix.
    @param vXAngle X-angle for rotation matrix.
    @see ToEulerAnglesXYZ,  FromEulerAnglesXYZ, FromEulerAnglesXZY, FromEulerAnglesYXZ, FromEulerAnglesYZX, FromEulerAnglesZXY*/
    void FromEulerAnglesZYX (eC_Value vZAngle, eC_Value vYAngle, eC_Value vXAngle);

protected:

    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    // member access.
    eC_Value m_aafEntry[3][3];
};

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 ()
{
    // For efficiency reasons, do not initialize matrix.
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 (const eC_Value aafEntry[3][3])
{
    memcpy(m_aafEntry, aafEntry, 9*sizeof(eC_Value));
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 (const eGML_Matrix3& rkMatrix)
{
    memcpy(m_aafEntry, rkMatrix.m_aafEntry, 9*sizeof(eC_Value));
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 (eC_Value fEntry00, eC_Value fEntry01, eC_Value fEntry02,
                                   eC_Value fEntry10, eC_Value fEntry11, eC_Value fEntry12,
                                   eC_Value fEntry20, eC_Value fEntry21, eC_Value fEntry22)
{
    m_aafEntry[0][0] = fEntry00;
    m_aafEntry[0][1] = fEntry01;
    m_aafEntry[0][2] = fEntry02;
    m_aafEntry[1][0] = fEntry10;
    m_aafEntry[1][1] = fEntry11;
    m_aafEntry[1][2] = fEntry12;
    m_aafEntry[2][0] = fEntry20;
    m_aafEntry[2][1] = fEntry21;
    m_aafEntry[2][2] = fEntry22;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 (const eGML_Matrix2& rkMatrix)
{
    m_aafEntry[0][0] = rkMatrix[0][0];
    m_aafEntry[0][1] = rkMatrix[0][1];
    m_aafEntry[0][2] = eC_FromInt(0);
    m_aafEntry[1][0] = rkMatrix[1][0];
    m_aafEntry[1][1] = rkMatrix[1][1];
    m_aafEntry[1][2] = eC_FromInt(0);
    m_aafEntry[2][0] = eC_FromInt(0);
    m_aafEntry[2][1] = eC_FromInt(0);
    m_aafEntry[2][2] = eC_FromInt(1);
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::eGML_Matrix3 (const eGML_Vector3& rkLeft, const eGML_Vector3& rkUp, const eGML_Vector3& rkDir)
{	
    m_aafEntry[0][0] = rkLeft.x;
    m_aafEntry[0][1] = rkUp.x;
    m_aafEntry[0][2] = rkDir.x;
    m_aafEntry[1][0] = rkLeft.y;
    m_aafEntry[1][1] = rkUp.y;
    m_aafEntry[1][2] = rkDir.y;
    m_aafEntry[2][0] = rkLeft.z;
    m_aafEntry[2][1] = rkUp.z;
    m_aafEntry[2][2] = rkDir.z;
}

//----------------------------------------------------------------------------
inline eC_Value* eGML_Matrix3::operator[] (eC_Int iRow) const
{
    return (eC_Value*)&m_aafEntry[iRow][0];
}

//----------------------------------------------------------------------------
inline eC_Value* eGML_Matrix3::operator[] (int iRow) const
{
    return (eC_Value*)&m_aafEntry[iRow][0];
}

//----------------------------------------------------------------------------
inline eGML_Matrix3::operator eC_Value* ()
{
    return &m_aafEntry[0][0];
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Matrix3::GetColumn (eC_Int iCol) const
{
    eDBG_ASSERT( 0 <= iCol && iCol < 3, "Column index out of bounds" );
    return eGML_Vector3(m_aafEntry[0][iCol],m_aafEntry[1][iCol], m_aafEntry[2][iCol]);
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Matrix3::GetColumn (int iCol) const
{
    eDBG_ASSERT( 0 <= iCol && iCol < 3, "Column index out of bounds" );
    return eGML_Vector3(m_aafEntry[0][iCol], m_aafEntry[1][iCol], m_aafEntry[2][iCol]);
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Matrix3::GetRow (eC_Int iRow) const
{
    eDBG_ASSERT( 0 <= iRow && iRow < 3, "Row index out of bounds" );
    return eGML_Vector3(m_aafEntry[iRow][0],m_aafEntry[iRow][1], m_aafEntry[iRow][2]);
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Matrix3::GetRow (int iRow) const
{
    eDBG_ASSERT( 0 <= iRow && iRow < 3, "Row index out of bounds" );
    return eGML_Vector3(m_aafEntry[iRow][0],m_aafEntry[iRow][1], m_aafEntry[iRow][2]);
}

//----------------------------------------------------------------------------
inline eGML_Matrix3& eGML_Matrix3::operator= (const eGML_Matrix3& rkMatrix)
{
    memcpy(m_aafEntry, rkMatrix.m_aafEntry, 9*sizeof(eC_Value));
    return *this;
}

//----------------------------------------------------------------------------
inline eC_Bool eGML_Matrix3::operator== (const eGML_Matrix3& rkMatrix) const
{
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
        {
            if ( m_aafEntry[iRow][iCol] != rkMatrix.m_aafEntry[iRow][iCol] )
                return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------
inline eC_Bool eGML_Matrix3::operator!= (const eGML_Matrix3& rkMatrix) const
{
    return !operator==(rkMatrix);
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 eGML_Matrix3::operator+ (const eGML_Matrix3& rkMatrix) const
{
    eGML_Matrix3 kSum;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
        {
            kSum.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] +
                rkMatrix.m_aafEntry[iRow][iCol];
        }
    }
    return kSum;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 eGML_Matrix3::operator- (const eGML_Matrix3& rkMatrix) const
{
    eGML_Matrix3 kDiff;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
        {
            kDiff.m_aafEntry[iRow][iCol] = m_aafEntry[iRow][iCol] -
                rkMatrix.m_aafEntry[iRow][iCol];
        }
    }
    return kDiff;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 eGML_Matrix3::operator* (const eGML_Matrix3& rkMatrix) const
{
    eGML_Matrix3 kProd;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
        {
            kProd.m_aafEntry[iRow][iCol] =
                eC_Mul(m_aafEntry[iRow][0], rkMatrix.m_aafEntry[0][iCol]) +
                eC_Mul(m_aafEntry[iRow][1], rkMatrix.m_aafEntry[1][iCol]) +
                eC_Mul(m_aafEntry[iRow][2], rkMatrix.m_aafEntry[2][iCol]);
        }
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 eGML_Matrix3::operator* (const eGML_Vector3& rkPoeC_Int) const
{
    eGML_Vector3 kProd;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        kProd[iRow] =
            eC_Mul(m_aafEntry[iRow][0], rkPoeC_Int[0]) +
            eC_Mul(m_aafEntry[iRow][1], rkPoeC_Int[1]) +
            eC_Mul(m_aafEntry[iRow][2], rkPoeC_Int[2]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Vector3 operator* (const eGML_Vector3& rkPoeC_Int, const eGML_Matrix3& rkMatrix)
{
    eGML_Vector3 kProd;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        kProd[iRow] =
            eC_Mul(rkPoeC_Int[0], rkMatrix.m_aafEntry[0][iRow]) +
            eC_Mul(rkPoeC_Int[1], rkMatrix.m_aafEntry[1][iRow]) +
            eC_Mul(rkPoeC_Int[2], rkMatrix.m_aafEntry[2][iRow]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 eGML_Matrix3::operator- () const
{
    eGML_Matrix3 kNeg;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
            kNeg[iRow][iCol] = -m_aafEntry[iRow][iCol];
    }
    return kNeg;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 eGML_Matrix3::operator* (eC_Value fScalar) const
{
    eGML_Matrix3 kProd;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
            kProd[iRow][iCol] = eC_Mul(fScalar, m_aafEntry[iRow][iCol]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
inline eGML_Matrix3 operator* (eC_Value fScalar, const eGML_Matrix3& rkMatrix)
{
    eGML_Matrix3 kProd;
    for (eC_Int iRow = 0; iRow < 3; iRow++)
    {
        for (eC_Int iCol = 0; iCol < 3; iCol++)
            kProd[iRow][iCol] = eC_Mul(fScalar, rkMatrix.m_aafEntry[iRow][iCol]);
    }
    return kProd;
}

//----------------------------------------------------------------------------
#endif
