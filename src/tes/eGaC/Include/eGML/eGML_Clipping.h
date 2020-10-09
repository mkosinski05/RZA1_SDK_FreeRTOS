/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Clipping.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:16:47 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
  28-08-08     | hh04058       | modified the clipline-methode, to "flag" lines for pixel-wise clipping
******************************************************************************/


#ifndef EGML_CLIPPING__H_
#define EGML_CLIPPING__H_
//---------------------------------------------------------------	

#include "eDBG_Statistic.h"
#include "eC_Math.h"
#include "eGML_Vector2.h"
#include "eGML_Vector3.h"
#include "eGML_Primitive.h"

//---------------------------------------------------------------	
// undef windows.h macros that might cause collisions
#ifdef eC_TARGET_ENV_WIN32
#ifdef CP_NONE
#undef CP_NONE
#endif
#endif

#ifdef eC_TARGET_ENV_POCKET
#ifdef CP_NONE
#undef CP_NONE
#endif
#endif

//---------------------------------------------------------------	

class eGML_Clipper
{
public:
  
	enum ClipFlags
    {
      CF_NOTVIS  = 0,				// NOSEGM
      CF_VIS     = 1,				// SEGM
      CF_CLIPPED = 2,				// CLIP			
	  CB_OUTOFBOUND = 4,            // PIXEL-CLIPPING
	  CF_OVERFLOW = 8            // IS SET, IF A PREDEFINED MARGIN IS VIOLATED
	};
    
    enum RegionCodes
    {
      CL_INSIDE = 0,
      CL_RIGHT  = 1,
      CL_TOP    =	2,
      CL_LEFT   =	4,
      CL_BOTTOM =	8,	  
      CL_2BIT   =	0x100			// TWOBITS
    };
    
    enum TurningPoints
    {						
      TP_LL = 0,				// lower left			
      TP_LR =	1,				// lower right			
      TP_UL =	2,				// upper left
      TP_UR =	3,				// upper right
        
      TP_NONE = -1				// marking usused offsets
    };
    
    enum PlaneCodes
    {
      CP_NONE = 0,
      CP_MIN  = 1,
      CP_MAX  = 2
    };       
    
    static inline void InitRegion(eC_UInt uiXmin, eC_UInt uiYmin, eC_UInt uiXmax, eC_UInt uiYmax, eC_Value vEpsilon = eC_FromFloat(1e-04f));
    static void InitRegionV(eC_Value vXmin, eC_Value vYmin, eC_Value vXmax, eC_Value vYmax, eC_Value vEpsilon = eC_FromFloat(1e-04f));
    
    static inline void InitRange(eC_UInt uiMin, eC_UInt uiMax, eC_Value vEpsilon = eC_FromFloat(1e-04f));
    static void InitRangeV(eC_Value vMin, eC_Value vMax, eC_Value vEpsilon = eC_FromFloat(1e-04f));
    
    static eC_Flag ClipLine( eGML_Vector2 &kStart, eGML_Vector2 &kEnd, eC_Bool antiAA= false, eC_Value x1 = 0, eC_Value y1 = 0, eC_Value x2 = 0, eC_Value y2 = 0 );
    static bool IsLineVisible( eGML_Vector2 &kStart, eGML_Vector2 &kEnd );
    static eC_Flag ClipLinePara( eGML_Vector2 &kStart, eGML_Vector2 &kEnd, eC_Value &vStart, eC_Value &vEnd );
    static eGML_Coord2D *ClipPolygon( const void *pvInput, eC_UInt uiCount, eC_UInt uiPitch, eC_UInt &uiNewCount );
    
    static eC_Flag ZClipLine( eGML_Vector3 &kStart, eGML_Vector3 &kEnd );
    static eC_Flag ZClipLinePara( eGML_Vector3 &kStart, eGML_Vector3 &kEnd, eC_Value &vStart, eC_Value &vEnd );
    
    static eC_Flag ZClipPolyline( const eGML_Vector3 *pkSource, eGML_Vector3 *pkDest, eC_UShort &usCount, eC_Bool bClosed );

	static void SetOverFlowMargin(eC_Value margin);
    
private:
  
  struct eGML_CPoint 
  {
    eGML_Vector2 point;
    eC_Int       code;
  };
  
    static eC_Flag ClassifyPoint(const eGML_Vector2 &rkPoint);		
    static inline eC_Flag QuickClassifyPoint( const eGML_Vector2 &vP );
    static inline eC_Flag QuickClassifyPoint( eGML_CPoint &kP );
	static inline eC_Bool CheckBoundaries( const eGML_Vector2 &vP, eC_Value x1, eC_Value y1, eC_Value x2, eC_Value y2);
    static inline eC_Int BorderClassify( const eGML_CPoint &kP );
    
	static eC_Flag ClipLineSegment( eGML_Vector2 &kA, eGML_Vector2 &kB, eC_Flag uiCodeA, eC_Flag uiCodeB, eC_Value &vTa, eC_Value &vTb, eC_Bool antiAA= false, eC_Value x1 = 0, eC_Value y1 = 0, eC_Value x2 = 0, eC_Value y2 = 0, eC_Flag uiOverFlowFlag = 0);
    
    static eGML_Vector2 m_akRegion[4];
    static const eC_Int	m_aiTurnIndex[16];
    static const eC_Int	m_aiRegionIndex[16];
    static const eC_Int m_aiRegionBorder[4];
    static eC_Value     m_vRangeMin;
    static eC_Value     m_vRangeMax;
    static eC_Value     m_vXMin;
    static eC_Value     m_vYMin;
    static eC_Value     m_vXMax;
    static eC_Value     m_vYMax;

	static eC_Value     m_overFlowProtectionMargin;
};

//---------------------------------------------------------------	
//
inline eC_Flag eGML_Clipper::QuickClassifyPoint( const eGML_Vector2 &vP )
{
  eC_Flag ulCode = CL_INSIDE;
  
  if (vP.x < m_akRegion[ TP_UL ].x) ulCode |= CL_LEFT;	else
    if (vP.x > m_akRegion[ TP_LR ].x) ulCode |= CL_RIGHT; 
    if (vP.y < m_akRegion[ TP_UL ].y) ulCode |= CL_TOP;	    else
      if (vP.y > m_akRegion[ TP_LR ].y) ulCode |= CL_BOTTOM; 
      
      return ulCode;
} 


inline eC_Bool eGML_Clipper::CheckBoundaries( const eGML_Vector2 &vP, eC_Value x1, eC_Value y1, eC_Value x2, eC_Value y2)
{ 
  return ((vP.x < (m_akRegion[ TP_UL ].x + x1))  ||
	 (vP.x > (m_akRegion[ TP_LR ].x + x2)) ||
     (vP.y < (m_akRegion[ TP_UL ].y + y1)) ||
     (vP.y > (m_akRegion[ TP_LR ].y + y2)) );
} 


//---------------------------------------------------------------	
//
inline eC_Int eGML_Clipper::BorderClassify( const eGML_CPoint &kP )
{
  eC_Int ulCode = CL_INSIDE; 
  
  if (kP.code & CL_LEFT) 
  {
    if (kP.point.x == m_akRegion[ TP_UL ].x) ulCode |= CL_LEFT;       
  } 
  else if (kP.code & CL_RIGHT) 
  {
    if (kP.point.x == m_akRegion[ TP_LR ].x) ulCode |= CL_RIGHT;
  }
  
  if (kP.code & CL_TOP) 
  {
    if (kP.point.y == m_akRegion[ TP_UL ].y) ulCode |= CL_TOP;       
  } 
  else if (kP.code & CL_BOTTOM) 
  {
    if (kP.point.y == m_akRegion[ TP_LR ].y) ulCode |= CL_BOTTOM;
  }
  
  return ulCode;
}

//---------------------------------------------------------------	
//
inline eC_Flag eGML_Clipper::QuickClassifyPoint( eGML_CPoint &kP )
{
  return (kP.code = ClassifyPoint( kP.point ));
}

//---------------------------------------------------------------	
//
inline void eGML_Clipper::InitRegion(eC_UInt uiXmin, eC_UInt uiYmin, eC_UInt uiXmax, eC_UInt uiYmax, eC_Value vEpsilon)
{
  InitRegionV( eC_FromInt( uiXmin ), eC_FromInt( uiYmin ), eC_FromInt( uiXmax ), eC_FromInt( uiYmax ), vEpsilon );
}

//---------------------------------------------------------------	
//
inline void eGML_Clipper::InitRange(eC_UInt uiMin, eC_UInt uiMax, eC_Value vEpsilon)
{
  InitRangeV( eC_FromInt(uiMin), eC_FromInt(uiMax), vEpsilon );
}

//---------------------------------------------------------------	
#endif
