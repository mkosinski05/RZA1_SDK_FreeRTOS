/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Grobi.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:17:23 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGOB_ELLIPSE__H_
#define EGOB_ELLIPSE__H_
//---------------------------------------------------------------
#include <stdio.h>

#include "eC.h"
#include "eGML.h"

//---------------------------------------------------------------
//The number of steps for a complete circle (360 degree)
#define eGML_GROBI_ANGLE_STEPS 1024

//---------------------------------------------------------------
/** @name eGML_Grobi_Structures
@memo
Buffers supported by the Graphical Object Builder Interface (Grobi).
@doc
This is a collection of buffer structures used by the \Ref{eGML_Grobi} class. Normaly these buffers are used for storing boundary parameters
of the Grobi objects themselves.
@see eGML_Grobi, ARC_TYPE
*/

//@{

//---------------------------------------------------------------
/**
@memo Predefined struct for eGML_Grobi objects.
@doc Rectangle for defining the <I>boundary</I> and <I>position</I> of a Grobi-object inside of a screen or bitmap. Used for creating an object.
@see eGML_Grobi */
struct eGML_GROBI_RECT 
{
  /// Left upper corner (x min value).
  eC_Value x1;
  /// Left upper corner (y min value).
  eC_Value y1;
  /// Right lower corner (x max value).
  eC_Value x2;
  /// Right lower corner (y max value).
  eC_Value y2;
};

//---------------------------------------------------------------
/* NOT FINISHED YET ?
@memo Predefined struct for eGML_Grobi 3D objects.
@doc Rectangle for defining the boudary of a 3D-Grobi-object. Used for creating an object.
@see eGML_Grobi, eGML_Vector3 */
struct eGML_GROBI_RECT_3D 
{
  // Left upper corner.
  eGML_Vector3 vecLeftUpperFrontPoint;
  // Right lower corner.
  eGML_Vector3 vecRightBottomBackPoint;
};

//---------------------------------------------------------------
/* NOT FINISHED YET ?
@memo Predefined struct for multiple eGML_Grobi 3D objects buffer.
@doc Buffer containing simple 3D primitive surface objects called faces.
@see eGML_Grobi, eGML_Primitive */
struct eGML_GROBI_FACE_BUFFER_3D 
{
  // Number of faces.
  eC_UInt uiNbOfFaces;
  // Array of primitive objects.
  eGML_Primitive * ppkFace[100];
};

// end of eGML_Grobi_Structures

//@}

//---------------------------------------------------------------
/**
@memo
The Grobi means <B>Gr</B>aphical-<B>O</B>bject-<B>B</B>uilder-<B>I</B>nterface.
@doc
With this interface \Ref{eGML_Primitive} based shapes can be created (e.g. ellipse). The return value of all Create-Functions is a
\Ref{eGML_Primitive} object, that just contains the vertices for drawing the desired figure. The Primitive object does not have any
further information about the figure itself but the information of rendering (see \Ref{eGML_RenderState}). So it is possible to combine the
properties of renderstates and primitives in a simple way. <BR>
For getting more information about \Ref{eGML_Grobi} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Grobi_1.cpp]{examples/example_Grobi_1.cpp.html} <BR>
@see eGML_Primitive, eGML_Bitmap, eGML_Screen, eGML_RenderState
@see Tutorial_eGML
*/
class eGML_Grobi
{
  public:

    /** The types of ellipse or arcs.
    This flag determines the behavior of a not 360 degree ellipse (an arc). */
    enum ARC_TYPE
    {
      /// No special treat, the endpoint will not connected.
      ARC_OPEN,
      /// The endpoint will connected over the midpoint (pie-piece).
      ARC_MIDPOINT_CLOSED,
      /// The endpoints will connected with a staight line.
      ARC_ENDPOINT_CLOSED
    };

    // Grobi was reimplemented to be a pure static utility class
    // trigonometric functions will used from eC Platform now (there is a sin/cos lookup table available)

    ///** Standard constructor. */
    //eGML_Grobi();

    ///** Destructor. */
    //~eGML_Grobi();

    /** Creates an ellipse stored in a \Ref{eGML_Primitive} object.
    The returned Primitive object just contains the vertices for drawing an ellipse
    with the number of edges and the treat of an open ellipse.
    @param pScreen Pointer to the current screen.
    @param sArcRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the ellipse.
    @param fArcType Ellipse type, special treat of not closed ellipses (see \Ref{ARC_TYPE}).
    @param usVertices Optional number of vertices for the visible ellipse between start and end angle.
    @param vStartAngle Optional DEG-angle where the ellipse should start.
    @param vEndAngle Optional DEG-angle where the ellipse should end.
    @param vRotAngle Optional DEG-angle of ellipses rotation.
    @param pkExistingPrimitive Optional Pointer to an exisiting Primitive to fill verteces
    @return Pointer to the created or given ellipse primitive. (NULL on any error)
    @see eGML_Primitive, ARC_TYPE,  eGML_RenderState, eGML_Grobi_Structures */
    static eGML_Primitive *CreateEllipse(eGML_Screen *pScreen,
                                  const eGML_GROBI_RECT & rsArcRect,
                                  eC_Flag fArcType = 0,
                                  eC_UShort usVertices = 32,
                                  eC_Value vStartAngle = eC_FromFloat(0),
                                  eC_Value vEndAngle = eC_FromFloat(360),
                                  eC_Value vRotAngle = eC_FromFloat(0),
                                  eGML_Primitive * pkExistingPrimitive = NULL);

    /** calculate ellipse verteces and store it to a given \Ref{eGML_Primitive} object.
    The returned Primitive object just contains the vertices for drawing an ellipse
    with the number of edges and the treat of an open ellipse.
    @param pScreen Pointer to the current screen.
    @param sArcRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the ellipse.
    @param pkExistingPrimitive Optional Pointer to an exisiting Primitive to fill verteces
    @param fArcType Ellipse type, special treat of not closed ellipses (see \Ref{ARC_TYPE}).
    @param usVertices Optional number of vertices for the visible ellipse between start and end angle.
    @param vStartAngle Optional DEG-angle where the ellipse should start.
    @param vEndAngle Optional DEG-angle where the ellipse should end.
    @param vRotAngle Optional DEG-angle of ellipses rotation.
    @return success od calculation true / false
    @see eGML_Primitive, ARC_TYPE,  eGML_RenderState, eGML_Grobi_Structures */
    static eC_Bool CalculateEllipse(eGML_Screen *pScreen,
                                  const eGML_GROBI_RECT & rsArcRect,
                                  eGML_Primitive * pkExistingPrimitive,
                                  eC_Flag fArcType = 0,
                                  eC_UShort usVertices = 32,
                                  eC_Value vStartAngle = eC_FromFloat(0),
                                  eC_Value vEndAngle = eC_FromFloat(360),
                                  eC_Value vRotAngle = eC_FromFloat(0))
    {
       if (pkExistingPrimitive == NULL) return false;
       return (NULL != CreateEllipse(pScreen, rsArcRect, fArcType, usVertices, vStartAngle, vEndAngle, vRotAngle, pkExistingPrimitive));
    }

    /** Creates a box stored in a \Ref{eGML_Primitive} object. The returned primitive object just contains the vertices
    for drawing a box with four edges, there are no further information about the box, all properties like color or line
    width are set by the <TT><B>Set</B></TT> method of \Ref{eGML_RenderState} interface.
    @param pScreen Pointer to the current screen.
    @param sBoxRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the box.
    @param pkExistingPrimitive Optional Pointer to an exisiting Primitive to fill verteces
    @param pkExistingPrimitive Optional Pointer to an exisiting Primitive to fill verteces
    @return Pointer to the created or given box primitive. (NULL on any error)
    @see eGML_Primitive, eGML_RenderState, eGML_Grobi_Structures */
    static eGML_Primitive *CreateBox(eGML_Screen *pScreen, const eGML_GROBI_RECT & rsBoxRect, eGML_Primitive * pkExistingPrimitive = NULL);
    
    /* !!! NOT FINISHED YET ?
    Creates a triangle stored in a \Ref{eGML_Primitive} object. The returned primitive object just contains the vertices
    for drawing a triangle.
    @param pScreen Pointer to the current screen.
    @param sTriangleRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the triangle.
    @param pkExistingPrimitive Optional Pointer to an exisiting Primitive to fill verteces
    @return Pointer to the created or given triangle primitive. (NULL on any error)
    @see eGML_Primitive, eGML_RenderState, eGML_Grobi_Structures */
    static eGML_Primitive *CreateTriangle(eGML_Screen *pScreen, const eGML_GROBI_RECT & rsTriangleRect, eGML_Primitive * pkExistingPrimitive = NULL);
    
    /* !!! NOT FINISHED YET ?
    Creates a 3D sphere stored in a \Ref{eGML_Primitive} object. The returned primitive object just contains the vertices
    for drawing a sphere.
    @param pScreen Pointer to the current screen.
    @param sSphereRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the sphere.
    @return Pointer to the created sphere primitive.
    @see eGML_Primitive, eGML_RenderState, eGML_Grobi_Structures */
    static eGML_GROBI_FACE_BUFFER_3D *CreateSphere(eGML_Screen *pScreen,
                                            const eGML_GROBI_RECT_3D & rsSphereRect,
                                            eC_Flag fArcType = 0,
                                            eC_UShort usVertices = 32,
                                            eC_Value vStartAngle = eC_FromFloat(0),
                                            eC_Value vEndAngle = eC_FromFloat(360));
    
    /* !!! NOT FINISHED YET ?
    Creates a 3D box stored in a \Ref{eGML_Primitive} object. The returned primitive object just contains the vertices
    for drawing a box.
    @param pScreen Pointer to the current screen.
    @param sSphereRect The \Ref{eGML_GROBI_RECT} that defines the boundary of the box.
    @return Pointer to the created box primitive.
    @see eGML_Primitive, eGML_RenderState, eGML_Grobi_Structures */
    static eGML_GROBI_FACE_BUFFER_3D *CreateCuboid(eGML_Screen *pScreen, const eGML_GROBI_RECT_3D & rsBoxRect);

};

//---------------------------------------------------------------
#endif
