/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Primitive.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Thu Sep 08 13:04:18 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_PRIMITIVE__H_
#define EGML_PRIMITIVE__H_

//---------------------------------------------------------------
#include "eDBG.h"
#include "eC_TList.h"
#include "eGML_Config.h"
#include "eGML_Vector2.h"
#include "eGML_Vector3.h"
#include "eGML_Matrix2.h"
#include "eGML_Matrix3.h"

//---------------------------------------------------------------
/** @name eGML_Primitive_Buffer_Types
@memo
Buffer layouts supported by <B>eGML</B>.
@doc
This is a collection of all buffer layout structures you can use with <B>eGML</B> particularly by \Ref{eGML_Primitive} objects.
@see eGML_Primitive, Vertex_Layout_Fragments
*/

//@{


//---------------------------------------------------------------
/**
@memo Structure providing 2D-coordinates.
@doc In this structure you can store X- and Y-coordinates. Its part o the \Ref{Buffer_Types} collection.
@see eGML_Coord3D, eGML_Vector2 */
struct eGML_Coord2D
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

	/// X-component.
  eC_Value x;
  /// Y-component.
  eC_Value y;
};

//---------------------------------------------------------------
/**
@memo Structure providing 3D-coordinates.
@doc In this structure you can store X-, Y- and Z-coordinates. Its part o the \Ref{Buffer_Types} collection.
@see eGML_Coord2D, eGML_Normal, eGML_Vector3 */
struct eGML_Coord3D
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// X-component.
  eC_Value x;
  /// Y-component.
  eC_Value y;
  /// Z-component.
  eC_Value z;
};


//---------------------------------------------------------------
/**
@memo Structure providing color information.
@doc This structure contains color value.  Its part o the \Ref{Buffer_Types} collection.
Prefer using <TT><B>ColorFromRGB</B></TT> method of \Ref{eGML_Bitmap}/\Ref{eGML_Screen} to get eGML_Color format.
@see eGML_Color, eGML_VColUV1, eGML_VColUV2 */
struct eGML_VRL
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  //Reference Length
  eC_Value reflength;
};

//---------------------------------------------------------------
/**
@memo Structure providing color information.
@doc This structure contains color value.  Its part o the \Ref{Buffer_Types} collection.
Prefer using <TT><B>ColorFromRGB</B></TT> method of \Ref{eGML_Bitmap}/\Ref{eGML_Screen} to get eGML_Color format.
@see eGML_Color, eGML_VColUV1, eGML_VColUV2 */
struct eGML_VCol
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Color value.
  eGML_Color col;
};


//---------------------------------------------------------------
/**
@memo Structure providing color information.
@doc This structure contains color and reference length.  Its part o the \Ref{Buffer_Types} collection.
Prefer using <TT><B>ColorFromRGB</B></TT> method of \Ref{eGML_Bitmap}/\Ref{eGML_Screen} to get eGML_Color format.
@see eGML_Color, eGML_VColUV1, eGML_VColUV2 */
struct eGML_VColRL
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Color value.
  eGML_Color col;
  
  //Reference Length
  eC_Value reflength;
};

//---------------------------------------------------------------
/**
@memo Structure providing UV-coordinates.
@doc In this structure you can store U- and V-coordinates. Its part o the \Ref{Buffer_Types} collection.
@see eGML_Normal, eGML_VColUV1, eGML_VColUV2 */
struct eGML_VCoordUV
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// U-component.
  eC_Value u;
  /// V-component.
  eC_Value v;
};

//---------------------------------------------------------------
/**
@memo Structure providing UV-coordinates.
@doc In this structure you can store U- and V-coordinates. Its part o the \Ref{Buffer_Types} collection.
@see eGML_Normal, eGML_VColUV1, eGML_VColUV2 */
struct eGML_VCoordUV2
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// U1-component.
  eC_Value u1;
  /// V1-component.
  eC_Value v1;

  /// U2-component.
  eC_Value u2;
  /// V2-component.
  eC_Value v2;

};

//---------------------------------------------------------------
/**
@memo Structure providing UV-coordinates and color information.
@doc In this structure you can store U- and V-coordinates and additionally a color value. Its part o the \Ref{Buffer_Types} collection.
@see eGML_VCoordUV, eGML_VColUV2, eGML_VCol */
struct eGML_VColUV1
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Color value.
  eGML_Color col;
  /// U-component.
  eC_Value u;
  /// V-component.
  eC_Value v;
};

//---------------------------------------------------------------
/**
@memo Structure providing UV1- and UV2-coordinates and color information.
@doc In this structure you can store two U- and V-coordinates and additionally a color value.
Its part o the \Ref{Buffer_Types} collection.
@see eGML_VColUV1, eGML_VCoordUV, eGML_VCol */
struct eGML_VColUV2
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Color value.
  eGML_Color col;
  /// 1st U-component.
  eC_Value u1;
  /// 1st V-component.
  eC_Value v1;
  /// 2nd U-component.
  eC_Value u2;
  /// 2nd V-component.
  eC_Value v2;
};

//---------------------------------------------------------------
/**
@memo Structure providing normal vector.
@doc In this structure you can store normal vector components for X-, Y- and Z-direction. Its part o the \Ref{Buffer_Types} collection.
@see eGML_Coord3D, eGML_VCoordUV */
struct eGML_Normal
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// X-component.
  eC_Value nx;
  /// Y-component.
  eC_Value ny;
  /// Z-component.
  eC_Value nz;
};

// //---------------------------------------------------------------
// /* !!! INTERNAL USE ONLY !!!
// @memo Structure providing 2D-coordinates in a packed storage style for economizing memory.
// @doc In this structure you can store X- and Y-coordinates as 16 bit instead of 32 bit values.
// Its part o the \Ref{Buffer_Types} collection.
// @see eGML_Coord2D */
// struct eGML_PackedCoord2D
// {
//   /* !!! INTERNAL USE ONLY !!!
//   if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
// #undef new
//     EGML_NEW_DELETE_OPERATOR_OVERWRITING
// #define new eC_NEW
//
//   // X-component.
//   eC_Short x;
//   // Y-component.
//   eC_Short y;
// };

// //---------------------------------------------------------------
// /* !!! INTERNAL USE ONLY !!!
// @memo Structure providing 3D-coordinates in a packed storage style for economizing memory.
// @doc In this structure you can store X-, Y- and Z-coordinates as 16 bit instead of 32 bit values.
// Its part o the \Ref{Buffer_Types} collection.
// @see eGML_Coord3D */
// struct eGML_PackedCoord3D
// {
//  /* !!! INTERNAL USE ONLY !!!
//   if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
// #undef new
//     EGML_NEW_DELETE_OPERATOR_OVERWRITING
// #define new eC_NEW
//   // X-component.
//   eC_Short x;
//   // Y-component.
//   eC_Short y;
//   // Z-component.
//   eC_Short z;
// };

//---------------------------------------------------------------
/**
@memo Structure providing index list for drawing lines.
@doc In this structure you can store two members representing indices of vertices: start- and end point of the line to draw.
Its part o the \Ref{Buffer_Types} collection.
@see eGML_TriList */
struct eGML_LineList
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Index of start point of the line to draw.
  eC_UShort usId1;
  /// Index of end point of the line to draw.
  eC_UShort usId2;
};

//---------------------------------------------------------------
/**
@memo Structure providing index list for drawing triangles.
@doc In this structure you can store three members representing indices of vertices: points of the triangle to draw.
Its part o the \Ref{Buffer_Types} collection.
@see eGML_LineList */
struct eGML_TriList
{
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

  /// Index of first point of the triangle to draw.
  eC_UShort usId1;
  /// Index of second point of the triangle to draw.
  eC_UShort usId2;
  /// Index of third point of the triangle to draw.
  eC_UShort usId3;
};


//@}

//---------------------------------------------------------------
eDBG_STAT_DEF( TransVertices );
eDBG_STAT_DEF( ProjVertices );

eDBG_PERF_DEF( Transform2d );
eDBG_PERF_DEF( Transform3d );
eDBG_PERF_DEF( Project3d );

//---------------------------------------------------------------
/**
@memo
Geometry container.
@doc
The eGML_Primitive buffers are a set of vertices and their connectivity to enable high speed rendering of simple geometric objects.
Simple in this context means using a single material/style definition and transformation for the entire object. <BR>
While it is of course possible to render e.g. a polyline using <I>n</I> consecutive calls to <TT><B>DrawLine</B></TT>, performance can be increased
(due to reduced overhead) by putting all vertices of the polyline into an eGML_Primitive and render with <TT><B>DrawPrimitive</B></TT> (both marked
methods are \Ref{eGML_Bitmap}/\Ref{eGML_Screen} members). In case a complex topology is used (additional UV maps required for
{@link eGML_Texture texturemapping}, vertex normals etc.) primitives can use additional buffers to hold these information. <BR>
Each buffer contains one class of elements (one for positional data, one for vertex attributes,..). The number of buffers and layout
of each buffer can be specified when creating the primitive or afterwards you can expand the buffer amount dynamically by the
\Ref{AddBuffer} method. Primitives are created by <TT><B>CreatePrimitive</B></TT> method of \Ref{eGML_Screen} class. <BR>
For getting more information about \Ref{eGML_Primitive} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Primitive_1.cpp]{examples/example_Primitive_1.cpp.html} <BR>
@see eGML_Bitmap, eGML_Screen, eGML_Texture
@see Tutorial_eGML
*/
class eGML_Primitive
{
public:
  
    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW


    /**
    @memo Storage style of primitive data.
    @doc List of all possible storage styles of {@link eGML_Primitive primitive} data. You have to evaluate one of these styles
    when creating a primitive object by <TT><B>CreatePrimitive</B></TT> method in \Ref{eGML_Screen} class.
    @see eGML_Primitive, Buffer_Types */
    enum Vertex_Layout_Fragments
    {
      /// 2D coordinates.
      CLF_XY       	=  1,
      /// Depth information.
      CLF_Z           =  2,
      /// 3D coordinates. Is (\Ref{CLF_XY} | \Ref{CLF_Z}).
      CLF_XYZ        =  3,
      //Use 16 bit packed input coordinates (signed short int).
      //CLF_PACKED16 = 128,
      /// Color per vertex.
      ALF_COLOR     =  4,
      /// First additional mapping.
      ALF_UV1         = 16,
      // Second addtional mapping.
      ALF_UV2         = 32,
      // Add RefLength
      ALF_RL          = 64,
      /// Vertex normals.
      NLF_NORMAL   =  8
    };
      
    /** List of all possible primitive buffer types used for creation/construction and locking of a primitive buffer.
    @see Lock, AddBuffer, CheckBuffer, ShareBufferFrom, GetCount, GetPitch */
    enum Buffer_Types
    {
      /// Buffer containing positional data. Usually [x,y] or [x,y,z].
      BT_COORD,
      /// Buffer for vertex attributes. Color, uv mappings: [c] or [c,uv1] or [c,uv1,uv2].
      BT_ATTRIB,
      /// Normal vector buffer per vertex.
      BT_NORMAL,
      /// Buffer for transformed positional data. Always 3D: [x,y,z].
      BT_TRANSFORMED,
      /// Buffer for indices: trilist, linelist, stripe, fan.
      BT_INDEX,
      /// Attribut buffer for every index.
      BT_INDEXATTRIB,
      /// Normal vector buffer per index.
      BT_INDEXNORMAL,
      // !!! INTERNAL USE ONLY !!! just for dimensioning array
      BT_QUANTITY
    };

    /** List of all possible translation modes. Using these flags means mainly restriction of translation possibilities
    to provide saver displaying of primitive buffers.
    @see Transform2d, Transform3d */
    enum Transform_Flags
    {
      /// Hint that the translation vector is zero.
      TF_NO_TRANSLATION   = 1,
      /// Hint that the transformation matrix is identity.
      TF_TRANSLATION_ONLY = 2,
      /// Hint that only the main diagonal matrix elements are non zero.
      TF_SCALE_ONLY       = 4,
      /// Hint that transformation matrix is identity and translation is (0,0,z).
      TF_ASSIGN_DEPTH     = 8+2
    };
    
    /** List of all possible lock modes. Controlling access to buffer data.
    @see Lock, Unlock */
    enum Lock_Flags
    {
      /// Full access (default).
      LF_READWRITE = 0,
      /// Readonly access. If the buffer is not modified specifieing this flag will improve performance.
      LF_READONLY  = 1,
      /// Writeonly access. If the buffercontent is not read back this flag might improve performance.
      LF_WRITEONLY = 2
    };
    
    /* !!! INTERNAL USE ONLY !!!
    List of all possible buffer status. */
    enum State_Flags
    {
      // No flags set.
      SF_EMPTY         =  0,
      // Primitive does contain data. Assumed after unlock coords.
      SF_FILLED        =  1,
      // Buffer is currently locked.
      SF_LOCKED        =  2,
      // Transformed data is present. Z-component is unused.
      SF_TRANSFORMED2D =  4,
      // Full 3D transformed data is present.
      SF_TRANSFORMED3D =  8,
      // Merge of both transformations. Is (\Ref{SF_TRANSFORMED2D} | \Ref{SF_TRANSFORMED3D}).
      SF_TRANSFORMED   = 12,
      // Primitive has been projected.
      SF_PROJECTED     = 16,
      // Just for dimensioning array.
      SF_QUANTITY      = 32
    };

    /** List of all possible buffer index interpretation styles.
    @see AddBuffer, CheckBuffer, DrawPrimitive */
    enum Index_Layout_Types
    {
      /// Indice buffer should be interpreted as: trilist.
      IBLT_TRILIST  = SF_QUANTITY,
      /// Indice buffer should be interpreted as: linelist.
      IBLT_LINELIST = IBLT_TRILIST * 2,
      /// Indice buffer should be interpreted as: stripe.
      IBLT_STRIPE   = IBLT_LINELIST * 2,
      /// Indice buffer should be interpreted as: fan.
      IBLT_FAN      = IBLT_STRIPE * 2,
      // !!! INTERNAL USE ONLY !!! just for dimensioning array.
      IBLT_QUANTITY = IBLT_FAN*2
    };

    
    /** List of all possible buffer indexattribute interpretation styles.
    @see AddBuffer, CheckBuffer, DrawPrimitive */
    enum IndexAttrib_Layout_Fragments
    {
      /// Color per index.
      IALF_COLOR = IBLT_QUANTITY
    };

    /* !!! INTERNAL USE ONLY !!!
    List of all possible projection flags. */
    enum eGML_Projection_Flags
    {
      // Use  present 'transformed' buffer if possible (include rotation not allowed).
      PF_REUSE_TRANSFORM    =  1,
      // Projection matrix contains full 3x3 matrix.
        PF_INCLUDE_ROTATION =  2,
      // Do not create a unique buffer for projected vertices.
        PF_SHARED_BUFFER    =  4,
      // Create width information based on distance [not recommended].
        PF_STORE_WIDTH      =  8,
      // Z-clip polyline before projecting vertices.
        PF_ZCLIP            = 16,
      // Closed z-clip polyline.
        PF_ZCLIP_CLOSED     = 32
    };

#ifdef eGML_PRIMITIVE_ALL_DISABLE  
    static void InitGlobals();
    static void RemoveGlobalBuffer();

    eGML_Primitive(eGML_Screen *pkScreen, eC_Flag uiLayout, eC_UShort usSize){;}
    virtual ~eGML_Primitive(){;}

#else
    // !!! INTERNAL USE ONLY !!!
    static void InitGlobals();
    
    /** Adds dynamically a new buffer. Usually not called with standard buffers like \Ref{BT_COORD} or \Ref{BT_ATTRIB} since
    those are created upon construction. Calling AddBuffer on an existing buffertyp will perform a resize.
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @param usSize Number of elements this buffer should be able to contain.
    @param uiLayout Layout of this buffer (see \Ref{Index_Layout_Types}).
    @return Only if buffer was succesfully added <B>true</B> will be returned.
    @see CheckBuffer, ShareBufferFrom, Buffer_Types, Index_Layout_Types */
    eC_Bool AddBuffer(eC_UInt uiType, eC_UShort usSize, eC_Flag uiLayout = 0);
    
    /** Checks for presence of a certain buffer.
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @param usSize Optional parameter: minimal size of buffer, smaller buffers will be reported as not present.
    @param uiLayout Optional parameter: checks for buffer with specified layout (see \Ref{Index_Layout_Types}).
    @return Only if the buffer is present and size is not '0' <B>true</B> will be returned.
    @see AddBuffer, ShareBufferFrom, Buffer_Types, Index_Layout_Types */
    eC_Bool CheckBuffer(eC_UInt uiType, eC_UShort usSize = 0, eC_Flag uiLayout = 0);
    
    /** Locks a primitive buffer. Before any data within the primitive can be modified, the corresponding buffer has to be locked. 
    To access the data you have to cast the returned pointer, depending on the layout you can use one of the
    {@link Buffer_Types predefined} types, or define your own.
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @param uiFlag Additional flags to hint special lock conditions (see \Ref{Lock_Flags}).
    @return Pointer to the first data element (cast as needed) or NULL if the lock failed.
    @see Unlock, Buffer_Types, Lock_Flags, Buffer_Types */
    void *Lock(eC_UInt uiType, eC_Flag uiFlag = LF_READWRITE);
    
    /** Signals end of buffer manipulation. After locking and modifying the buffer it has to be unlocked until <B>eGML</B> can
    render from this primitive again. In case transformed vertex data (an \Ref{BT_TRANSFORMED} Buffer) is present it is
    removed (invalidated) and you have to retransform since <B>eGML</B> assumes the data has changed after an Lock/Unlock operation.
    Additional buffers <B>do not</B> have to be invalidated if the buffer content did not change. You can lock a buffer for
    {@link Lock_Flags readonly} access to indicate this (performance gain).
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @see Lock, Buffer_Types */
    void Unlock(eC_UInt uiType);
    
    /** Transforms vertices of primitive. Untransformed (original) vertices are <I>not</I> overwritten by this operation.
    A new buffer is created (or re-used if one already exists) instead, to hold the transformed data. You can read back the
    vertices after transformation by using \Ref{BT_TRANSFORMED} instead of \Ref{BT_COORD} when {@link Lock locking}.
    The buffer layout of transformed vertices is always \Ref{eGML_Coord3D} (x,y,z). <BR>
    This operation always performs a 3D transformation since the matrix- and/or the vector-parameter have Z-coordinates,
    see \Ref{Transform2d} for 2D transformation.
    When rendering the primitive while transformed data is present the renderer will use the transformed information
    otherwise the 'original' data are applied.
    @param kRotation 3x3-{@link eGML_Matrix3 matrix} describing rotation and scaling.
    @param kTranslation 3D-{@link eGML_Vector3 vector} describing translation.
    @param uiFlags Optional hints to speed up transformation (see \Ref{Transform_Flags}).
    @see Lock, Unlock, Transform2d, eGML_Matrix3, eGML_Vector3, Transform_Flags */
    void Transform3d(const eGML_Matrix3 &kRotation, const eGML_Vector3 &kTranslation, eC_Flag uiFlags = 0);
    
    /** Transforms vertices of primitive. Untransformed (original) vertices are <I>not</I> overwritten by this operation.
    A new buffer is created (or re-used if one already exists) instead, to hold the transformed data. You can read back the
    vertices after transformation by using \Ref{BT_TRANSFORMED} instead of \Ref{BT_COORD} when {@link Lock locking}.
    The buffer layout of transformed vertices is always \Ref{eGML_Coord3D} (x,y,z). <BR>
    This operation always performs a 2D transformation, Z-coordinates are untouched, see \Ref{Transform3d} for 3D transformation.
    When rendering the primitive while transformed data is present the renderer will use the transformed information
    otherwise the 'original' data are applied.
    @param kRotation 2x2-{@link eGML_Matrix2 matrix} describing rotation and scaling.
    @param kTranslation 2D-{@link eGML_Vector2 vector} describing translation.
    @param uiFlags Optional hints to speed up transformation (see \Ref{Transform_Flags}).
    @see Lock, Unlock, Transform3d, eGML_Matrix2, eGML_Vector2, Transform_Flags */
    void Transform2d(const eGML_Matrix2 &kRotation, const eGML_Vector2 &kTranslation, eC_Flag uiFlags = 0);
    
    /** Retrieves buffersize. Gets number of elements from buffer. Since buffers do not change their sizes, frequent calls are not necessary.
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @return Number of elements this buffer can contain (0 for non existant buffers).
    @see Buffer_Types, GetBasesize */
    eC_UShort GetCount(eC_UInt uiType) const;
    
    /** Gets number of vertices. The <I>basesize</I> is the size of all vertex based buffers like \Ref{BT_COORD} or \Ref{BT_ATTRIB}.
    It is equal to the size specified when creating the primitive (see <TT><B>CreatePrimitive</B></TT> of \Ref{eGML_Screen}).
    @return Number of vertices this primitive can contain.
    @see GetPitch, GetCount, CreatePrimitive */
    inline eC_UShort GetBasesize() const;
    
    /** Gets number of bytes per element. Usually querying the pitch of a certain buffer should not be necessary, as they are
    predefined or specified by the application when calling <TT><B>CreatePrimitive</B></TT> of \Ref{eGML_Screen}.
    @param uiType Buffer type (see \Ref{Buffer_Types}).
    @return Number of bytes for each element (one element per vertex for vertexbase information).
    @see GetBasesize, CreatePrimitive */
    inline eC_UByte GetPitch(eC_UInt uiType) const;
    
    /** Shares an input buffer. It is possible to share a coordinate or attribute buffer between several primitives.
    In this case one primitive is the <I>source</I> and owns the buffer. It will be responsible for buffer creation and destruction. 
    Other primitives that wish to use the same set of vertices can share this buffer. When deleting the source all shared
    primitives will lose the buffer and report 0 on size and <B>false</B> on \Ref{CheckBuffer}.
    @param pkSource Owner of buffer .
    @param uiType Buffer to share (see \Ref{Buffer_Types}) - sharing temporary buffers like \Ref{BT_TRANSFORMED} is <I>not</I> allowed.
    @see AddBuffer, CheckBuffer, Buffer_Types */
    void ShareBufferFrom(eGML_Primitive *pkSource, eC_UInt uiType);

    /*
    */
    void FillRefLengths();

    /*
    */
    eC_Bool ResizeCoordBuffer( eC_UShort usSize );



    /*
    */
    eC_Bool AddFaceNormals();
    
    /** Deletes primitive object. Calls private destructor. */
    void Release();

  protected:
    
    friend class eGML_Bitmap;
    friend class eGML_Screen;
    friend class eGML;
    friend class eGML_FGDC_Bitmap;
    
    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    //
    static void ResizeGlobalCoordBuffer(eC_UShort usSize);

    //
    static void ResizeGlobalProjectedBuffer(eC_UShort usSize);

    //
    static void RemoveGlobalBuffer();
    
    // protected constructor
    eGML_Primitive(eGML_Screen *pkScreen, eC_Flag uiLayout, eC_UShort usSize);
    
    // protected destructor
    virtual ~eGML_Primitive();

    //
    void Transform3d(const eGML_Matrix3 &kRotation, const eGML_Vector3 &kTranslation, void *pDst, eC_Flag uiFlags );

    //
    void Transform2d(const eGML_Matrix2 &kRotation, const eGML_Vector2 &kTranslation, void *pDst, eC_Flag uiFlags );

    //
    void Project3d(const eGML_Matrix3 &kScale, const eGML_Vector2 &kCenter, const eC_Value vNear, const eC_Value vFar,
                               eGML_Coord3D *pSrc, eGML_Coord3D *pDest,eC_UInt uiCount , eC_Flag uiFlags );

    //
    inline void DelBuffer(eC_UInt uiType);
    
    //
    void *GetBuffer(eC_UInt uiType, eC_Enum eLayout=0);
    
    //
    inline eC_Flag GetState() const;
    
    //
    inline eC_Flag GetLayout() const;
    
    //
    inline eC_Bool IsCoord3d() const;
    
    //
//    inline eC_Bool IsPacked16() const;
      
    //
    virtual void *GetCoord(eC_UInt &ruiPitch);
    
    //
    static void *GetSharedCoords(eC_UInt &ruiPitch);

    //
    static void *GetSharedProjected(eC_UInt &ruiPitch);
        
    //
    void DropShares(eGML_Primitive *pkSource);

  private:
    
    inline void *GetMem(eC_UInt uiSize);
    inline void FreeMem(void *pAddr);


    // !!! INTERNAL USE ONLY !!!
    // all following private members are for internal use only

#ifdef eGML_USEEMEM    
    eMem_Heap *m_pkHeap;
#endif

    // store data layout as passed on construction
    eC_Flag m_uiLayout;

    // store creator
    eGML_Screen * m_pkScreen;
    
    // state flags
    eC_Flag m_uiState;
    
    //
    eC_Flag m_uiLockflags;
    
    //
    eC_UInt m_uiLockcount;
    
    // primitive data stored in fragment buffers
    eC_UByte * m_apubBuffer[BT_QUANTITY];
    
    // fragment entry sizes
    eC_UByte m_aubPitch[BT_QUANTITY];
    
    // number of entries stored in buffer
    eC_UShort m_auiCount[BT_QUANTITY];
    
    // flag shared primitive buffers
    eC_Bool m_abShared[BT_QUANTITY];

    //
    eC_UShort m_usBasesize;

    //
    eC_UShort m_usCoordBuffersize;

    // shared buffer owner management
    eGML_Primitive * m_pkShareSource;
    
    //
    eC_TList<eGML_Primitive *> m_kShareTargets;
#endif
};


#ifndef eGML_PRIMITIVE_ALL_DISABLE

//---------------------------------------------------------------
inline void *eGML_Primitive::GetMem(eC_UInt uiSize)
{
#ifdef eGML_USEEMEM    
  return m_pkHeap->Alloc( uiSize );
#else
  return eGML_GetMem( uiSize );
#endif
}

//---------------------------------------------------------------
inline void eGML_Primitive::FreeMem(void *pAddr)
{
#ifdef eGML_USEEMEM    
  m_pkHeap->Free( pAddr );
#else
  eGML_FreeMem( pAddr );
#endif
}

//---------------------------------------------------------------
inline void eGML_Primitive::DelBuffer( eC_UInt uiType )
{
  eDBG_Assert( uiType < BT_QUANTITY, "Illegal type %d", uiType );
  eDBG_Assert( !m_abShared[uiType], "Clients can not remove shared buffers (%d)", uiType );
  if (m_apubBuffer[ uiType ])
  {
    FreeMem(m_apubBuffer[ uiType ]);
    m_apubBuffer[ uiType ]= 0;
  }
}

//---------------------------------------------------------------
inline eC_UByte eGML_Primitive::GetPitch( eC_UInt uiType ) const
{
  eDBG_Assert( uiType < BT_QUANTITY, "Illegal type %d", uiType );
  return m_aubPitch[ uiType ];
}

//---------------------------------------------------------------
inline eC_Flag eGML_Primitive::GetState() const
{
  return m_uiState;
}

//---------------------------------------------------------------
inline eC_UShort eGML_Primitive::GetBasesize() const
{
  return m_usBasesize;
}

//---------------------------------------------------------------
inline eC_Flag eGML_Primitive::GetLayout() const
{
  return m_uiLayout;
}

//---------------------------------------------------------------
inline eC_Bool eGML_Primitive::IsCoord3d() const
{
  if (m_uiState & SF_TRANSFORMED)
  {
    return (m_uiState & SF_TRANSFORMED3D) != 0;
  }
  else 
  {
    return (m_uiLayout & CLF_Z) != 0;
  }
}

//---------------------------------------------------------------
//inline eC_Bool eGML_Primitive::IsPacked16() const
//{
//  return (m_uiLayout & CLF_PACKED16) != 0;
//}

//---------------------------------------------------------------
#endif //eGML_PRIMITIVE_ALL_DISABLE

//---------------------------------------------------------------
#endif
