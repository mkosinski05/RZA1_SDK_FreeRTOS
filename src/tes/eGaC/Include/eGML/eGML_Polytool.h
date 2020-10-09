/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Polytool.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:18:39 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_POLYTOOL__H_
#define EGML_POLYTOOL__H_
//---------------------------------------------------------------	

#include "eGML_Primitive.h"


/*
Test relation of a point to an infinite line.
@param kP0 first point of line
@param kP1 second point of line
@param kP2 point to classify
@return >0 for p2 left side of line, =0 for on the line and <0 for right side of line
*/
inline eC_Value eGML_ClassifyPointLineQuick( const eGML_Vector2 &kP0, const eGML_Vector2 &kP1, const eGML_Vector2 &kP2 )
{
  eGML_Vector2 kV1;
  eGML_Vector2 kV2;
 
  kV1 = kP1-kP0;
  kV2 = kP2-kP0;

  kV1.x = eC_RShift(kV1.x,8);
  kV1.y = eC_RShift(kV1.y,8);

  kV2.x = eC_RShift(kV2.x,8);
  kV2.y = eC_RShift(kV2.y,8);

  return ( eC_Mul(kV1.x , kV2.y) - eC_Mul(kV1.y , kV2.x) );
}

inline eC_Value eGML_ClassifyPointLine( const eGML_Vector2 &kP0, const eGML_Vector2 &kP1, const eGML_Vector2 &kP2 )
{
    return ( eC_Mul((kP1.x - kP0.x) , (kP2.y - kP0.y)) - eC_Mul((kP1.y - kP0.y) , (kP2.x - kP0.x)) );
}

inline eC_Value eGML_ClassifyPointLineSave( const eGML_Vector2 &kP0, const eGML_Vector2 &kP1, const eGML_Vector2 &kP2 )
{
    float fRet = ( (eC_ToFloat(kP1.x - kP0.x) * eC_ToFloat(kP2.y - kP0.y)) - (eC_ToFloat(kP1.y - kP0.y) * eC_ToFloat(kP2.x - kP0.x)) );
    if (fRet>= 32767)
      fRet= 32767;

    if (fRet<-32767)
      fRet= -32767;

    return eC_FromFloat(fRet);
}


// orientation2D_Triangle(): test the orientation of a triangle
//    Input:  three vertex points V0, V1, V2
//    Return: false for counterclockwise (/degenerate)
//            true for clockwise
inline eC_Bool eGML_Orientation2D_Triangle( const eGML_Vector2 &kV0, const eGML_Vector2 &kV1, const eGML_Vector2 &kV2 )
{
    return (eGML_ClassifyPointLineQuick(kV0, kV1, kV2) < eC_FromInt(0));
}

// internal use only. true for counterclockwise, false for clockwise and degenerate
inline eC_Bool eGML_Orientation2D_Triangle_Inv( const eGML_Vector2 &kV0, const eGML_Vector2 &kV1, const eGML_Vector2 &kV2 )
{
    return (eGML_ClassifyPointLineQuick(kV0, kV1, kV2) > eC_FromInt(0));
}


// area2D_Triangle(): compute the area of a triangle
//    Input:  three vertex points V0, V1, V2
//    Return: the (float) area of T
inline eC_Value eGML_Area2D_Triangle( const eGML_Vector2 &kV0, const eGML_Vector2 &kV1, const eGML_Vector2 &kV2 )
{
    return eGML_ClassifyPointLine(kV0, kV1, kV2) / 2;
}

//---------------------------------------------------------------	

extern eC_Bool eGML_Orientation2D_Polygon( eC_UInt uiCount, const eGML_Vector2 *akVertices );
extern eC_Bool eGML_Orientation2D_Polygon( eC_UInt uiCount, const eGML_Vector3 *akVertices );
extern eC_Bool eGML_PointIn2D_Polygon( eC_UInt uiCount, const eGML_Vector2 *akVertices, const eGML_Vector2 &kPoint );
extern eC_Bool eGML_PointIn2D_Polygon( eC_UInt uiCount, const eGML_Vector3 *akVertices, const eGML_Vector2 &kPoint );


//extern eC_UInt FrustumInsersectPlane(eGML_Vector3 *akFrustum, const eGML_Vector3 &rkP, const eGML_Vector3 &rkN, eGML_Vector3 *akResult);

//---------------------------------------------------------------	
#endif
