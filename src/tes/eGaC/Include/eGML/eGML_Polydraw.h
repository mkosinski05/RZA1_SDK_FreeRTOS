/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Polydraw.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Tue May 02 11:14:24 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_POLYDRAW__H_
#define EGML_POLYDRAW__H_
//---------------------------------------------------------------	

#define EGML_CAST(t,v)  (*((t*)  ((void *)&v)))

//---------------------------------------------------------------	

#include "eDBG_Statistic.h"
#include "eC_Math.h"
#include "eGML_Vector2.h"
#include "eGML_Bitmap.h"
#include "eGML_Primitive.h"
#include "eGML_Spandraw.h"
#include "eGML_SpandrawTex.h"
#include "eGML_Clipping.h"
#include "eGML_RenderState.h"
#include "eGML_Texture.h"
#include "eGML_PolydrawAA.h"

eDBG_STAT_DEF( NumberOfPolygons );
eDBG_STAT_DEF( RejectedPolygons );
eDBG_STAT_DEF( CrossingEdges );
eDBG_STAT_DEF( EdgeBuckets );
eDBG_STAT_DEF( PolyEdges );

eDBG_PERF_DEF( Polygons );



//---------------------------------------------------------------	
struct eGML_Polygon_Edge
{
  eC_Value			m_vX1, m_vY1;
  eC_Value			m_vX2, m_vY2;
  eC_Value			m_vDx;
  union { eC_Value	m_vDy, m_vSpan; };
  eC_Value			m_vX,  m_vY, m_vIncX;
  
  eC_Bool				m_bLeft;	
  
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
};

struct eGML_Polygon_EdgeBucket
{
  eGML_Polygon_EdgeBucket	*m_pkNext;
  
  eC_Int					 m_iTop;
  eGML_Polygon_Edge		*m_pkLeftEdge;
  eGML_Polygon_Edge		*m_pkRightEdge;	

#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
};

struct eGML_Polygon_ActiveEdge
{
  eGML_Polygon_ActiveEdge	*m_pkNext;
  eGML_Polygon_Edge		*m_pkEdge;
  eC_Int					 m_iBottom;

#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
};

struct eGML_Polygon_TextureFrame
{
  union unnamed {
    struct fixed
    {
      eC_Fixed            m_fxU0, m_fxUX, m_fxUY;
      eC_Fixed            m_fxV0, m_fxVX, m_fxVY;
      // used in case of perspecitve mapping only
      eC_Fixed            m_fxZ0, m_fxZX, m_fxZY;
    } fix;
    
    struct floating
    {
      eC_Float            m_fU0, m_fUX, m_fUY;
      eC_Float            m_fV0, m_fVX, m_fVY;
      eC_Float            m_fZ0, m_fZX, m_fZY;
    } flt;
  } dat;

#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
};

//---------------------------------------------------------------

extern eGML_Polygon_Edge * eGML_PolyGetEdgeBuffer( eC_UInt uiQuantity );
extern eGML_Polygon_EdgeBucket * eGML_PolyGetBucketBuffer( eC_UInt uiQuantity );
extern eGML_Polygon_ActiveEdge * eGML_PolyGetAETBuffer( eC_UInt uiQuantity );
extern eGML_Coord2D * eGML_PolyGetClippedBuffer1( eC_UInt uiQuantity );
extern eGML_Coord2D * eGML_PolyGetClippedBuffer2( eC_UInt uiQuantity );

extern void eGML_InitPolyDraw();
extern void eGML_ExitPolyDraw();

extern void eGML_Extrude_Polygon( eC_UByte *pubValue, eC_UInt uiPitch, eC_UInt uiNb, eC_UInt uiFlags );
//---------------------------------------------------------------
// Polygondraw code for not antialiased, simple concave polygons. 
//
template <
typename P1,		// Pixeltype
class S1			// Spanrender methods
> 
class eGML_Polydraw_NoAA
{
public:
  
  static void Polygon(eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, P1 pxColor, eC_UByte ubAlpha )
  {				
    eGML_Polygon_Edge		*akEdges;	
    eGML_Polygon_EdgeBucket	*akBuckets;
    eGML_Polygon_ActiveEdge	*akAEs;
    eC_UInt		uiEdgeCount = 0;
    eC_UInt		uiBucketCount = 0;		
    eC_UInt		uiAECount = 0;
    eGML_Polygon_EdgeBucket *pkFirstBucket = NULL;
    eGML_Polygon_ActiveEdge *pkAET = NULL;
    eGML_Polygon_Edge	   *pkEdge = NULL;
    const eGML_Polygon_Edge *pkEdgeMin,*pkEdgeMax,*pkLastEdge;
    const eGML_Coord2D *pkVertices = (eGML_Coord2D*) pvValues;
    
    eC_UInt		i;
    eC_Value	vX0,vY0,vX1,vY1;
    
    // prepare edge buffer
    akEdges = eGML_PolyGetEdgeBuffer( uiCount+1 );
    
    // get last vertex
    vX0 = ((const eGML_Coord2D *)((const eC_UByte*)pkVertices + ((uiCount-1)*uiPitch)))->x + eC_FromFloat(0.1f);
    vY0 = ((const eGML_Coord2D *)((const eC_UByte*)pkVertices + ((uiCount-1)*uiPitch)))->y + eC_FromFloat(0.5f);
    
    // create edges from vertextable
    for (i=0; i<uiCount; i++)
    {
      eC_Value vDx,vDy;
      
      vX1 = pkVertices->x + eC_FromFloat(0.1f);
      vY1 = pkVertices->y + eC_FromFloat(0.5f);
      vDx = vX0 - vX1;
      vDy = vY0 - vY1;
      
      // skip horizontals and dublicate vertices			
      if ((eC_Floor(vY0) != eC_Floor(vY1)) && (eC_Abs(vDy) > eC_FromFloat(0.01f)))
      {
        // alloc edge
        pkEdge = &akEdges[uiEdgeCount++];
        //pkEdge->iIndex=uiEdgeCount-1;
        
        // detect winding, add edge
        if (vY0 > vY1)
        {
          pkEdge->m_bLeft = true;
          pkEdge->m_vX1 = vX1;
          pkEdge->m_vY1 = vY1;
          pkEdge->m_vDx = vDx;
          pkEdge->m_vDy = vDy;
        } else {
          pkEdge->m_bLeft = false;
          pkEdge->m_vX1 = vX0;
          pkEdge->m_vY1 = vY0;
          pkEdge->m_vDx = -vDx;
          pkEdge->m_vDy = -vDy;
        }
        
        eC_Value vYoff;
        // find slope
        pkEdge->m_vIncX = eC_Div( pkEdge->m_vDx , pkEdge->m_vDy );
        
        // offset to lower pixelborder edge
        vYoff = eC_FromInt(1) - (pkEdge->m_vY1 - eC_Floor(pkEdge->m_vY1));
        // snap linestart to lower pixel edge						
        pkEdge->m_vX = pkEdge->m_vX1 + eC_Mul(vYoff, pkEdge->m_vIncX);
        pkEdge->m_vY = pkEdge->m_vY1 + vYoff;	
        // store endpos
        pkEdge->m_vX2 = pkEdge->m_vX1 + pkEdge->m_vDx; 
        pkEdge->m_vY2 = pkEdge->m_vY1 + pkEdge->m_vDy;
        // dont use lower edge xpos if outside line
        if (pkEdge->m_vDx < 0)
        {					
          if (pkEdge->m_vX < pkEdge->m_vX2) pkEdge->m_vX = pkEdge->m_vX2; 
        } else {
          if (pkEdge->m_vX > pkEdge->m_vX2) pkEdge->m_vX = pkEdge->m_vX2;
        }
        
        /*
        //-----DEBUG
        char acName[8];
        ltoa(uiEdgeCount-1, acName, 10);
        
          pBitmap->DrawLine(eGML_Vector2((vX0+vX1)/2,(vY0+vY1)/2), eGML_Vector2((vX0+vX1)/2 - vDy/4,(vY0+vY1)/2 + vDx/4), 0x888888, 0, eC_FromInt(1));
          pBitmap->DebugText(acName, (unsigned long)((vX0+vX1)/2 - vDy/4), (unsigned long)((vY0+vY1)/2 + vDx/4), 0xffffff);
          //-----DEBUG
        */
      }
      
      // next edge
      vX0 = vX1;
      vY0 = vY1;
      pkVertices = (const eGML_Coord2D *) (((const eC_UByte*)pkVertices) + uiPitch);
    }
    
    // early out on invisible polys
    if (uiEdgeCount < 2) 
    {
      eDBG_STAT_ADD(RejectedPolygons, 1);
      return;
    }
    eDBG_STAT_ADD(PolyEdges, uiEdgeCount);        
    
    // mark end of edgelist
    pkEdgeMin = akEdges;
    pkEdgeMax = pkEdge;
    
    // prepare edgebucket buffer
    akBuckets = eGML_PolyGetBucketBuffer( uiEdgeCount );
    
    // create edgebuckets from edgelist
    pkFirstBucket = NULL;
    
    pkLastEdge = pkEdgeMin;
    eC_Bool bLastLeft = akEdges[0].m_bLeft;
    i=0;		
    do 
    {					
      // wraparound counter
      if ((++i) == uiEdgeCount) i=0;
      pkEdge = &akEdges[i];
      
      eC_Bool bLeft = pkEdge->m_bLeft;
      // detect corner 
      if ((bLastLeft) && (!bLeft))
      {			
        // alloc edgebucket
        eGML_Polygon_EdgeBucket *pkBucket = &akBuckets[uiBucketCount++];
        
        // set first edgepair
        pkBucket->m_pkLeftEdge = (eGML_Polygon_Edge*) pkLastEdge;
        pkBucket->m_pkRightEdge = pkEdge;		
        
        pkBucket->m_iTop = eC_ToInt( pkEdge->m_vY1 );	//top = y or top = y1 ?
        
        // insert bucket (sorted by top-y)		
        eGML_Polygon_EdgeBucket *pkNode = pkFirstBucket;
        eGML_Polygon_EdgeBucket **ppBase = &pkFirstBucket;
        while ((pkNode) && (pkNode->m_iTop < pkBucket->m_iTop))
        {		
          ppBase = &(pkNode->m_pkNext);
          pkNode = *ppBase;
        }
        
        *ppBase = pkBucket;
        pkBucket->m_pkNext = pkNode;
      }
      bLastLeft = bLeft;
      pkLastEdge = pkEdge;
    } while (i>0);
    
    // early out on illegal polys
    if (uiBucketCount < 1) 
    {
      eDBG_STAT_ADD(RejectedPolygons, 1);
      return;
    }
    
    eDBG_STAT_ADD(EdgeBuckets, uiBucketCount);
    eDBG_STAT_ADD(NumberOfPolygons, 1);
    
    // prepare active edge table
    akAEs = eGML_PolyGetAETBuffer( uiEdgeCount );
    
    // topmost vertex
    eC_Int	iYpos = pkFirstBucket->m_iTop;
    
    do
    {		
      // find new active edgebuckets 			
      eGML_Polygon_EdgeBucket *pkBucket;
      // enter only when ytop of currently first bucket is reached
      while ((pkBucket = pkFirstBucket) && (pkBucket->m_iTop <= iYpos))
      {
        // remove bucket
        pkFirstBucket = pkBucket->m_pkNext;		
        // assign bucket startedges
        eGML_Polygon_Edge *pkEdge1, *pkEdge2;
        eC_Value vDelta;
        // compare startpoint (inequal in case of removed flat edges)
        // and slope (in common case of equal startpoint) to decide order
        vDelta = pkBucket->m_pkLeftEdge->m_vX1 - pkBucket->m_pkRightEdge->m_vX1;
        if (( vDelta < 0) ||
          (( vDelta == 0 ) && ( pkBucket->m_pkLeftEdge->m_vIncX <= pkBucket->m_pkRightEdge->m_vIncX )))
        {
          pkEdge1 = pkBucket->m_pkLeftEdge;
          pkEdge2 = pkBucket->m_pkRightEdge;
        } else {
          pkEdge1 = pkBucket->m_pkRightEdge;
          pkEdge2 = pkBucket->m_pkLeftEdge;
        }
        
        // create edgecontainers
        eGML_Polygon_ActiveEdge	*pkActive1 = &akAEs[uiAECount++];
        eGML_Polygon_ActiveEdge	*pkActive2 = &akAEs[uiAECount++];
        pkActive1->m_pkEdge = pkEdge1;
        pkActive1->m_pkNext = pkActive2;
        pkActive1->m_iBottom = eC_ToInt(pkEdge1->m_vY2);  //ceil or floor ?
        pkActive2->m_pkEdge = pkEdge2;
        pkActive2->m_iBottom = eC_ToInt(pkEdge2->m_vY2);  //ceil or floor ?
        
        // find edge insert position (just one search per bucket)
        eGML_Polygon_ActiveEdge *pkNode;
        eGML_Polygon_ActiveEdge **ppAET = &pkAET;
        while ((pkNode = *ppAET) && (pkNode->m_pkEdge->m_vX <= pkEdge1->m_vX))
        {
          // specialcase equal m_vX by slope comparison
          if (pkNode->m_pkEdge->m_vX == pkEdge1->m_vX)
          {
            if (pkNode->m_pkEdge->m_vIncX > pkEdge1->m_vIncX) break;
          }
          ppAET = &(pkNode->m_pkNext);
        }
        
        // insert edges
        pkActive2->m_pkNext = pkNode;
        *ppAET = pkActive1;
      }
      
      // loop all active edges
      eGML_Polygon_ActiveEdge **ppAET = &pkAET;
      eGML_Polygon_ActiveEdge *pkAEdge;
      eC_Value				 vX,vXold=0;
      
      while ((pkAEdge = *ppAET))
      {
        pkEdge = pkAEdge->m_pkEdge;
        
        // check if edge is finished
        eC_Bool	bLeft = pkEdge->m_bLeft;
        if (iYpos >= pkAEdge->m_iBottom)
        {
          // advance to next edge [circular]
          if (bLeft)
          {
            if ( (--pkEdge) < pkEdgeMin) pkEdge = (eGML_Polygon_Edge*) pkEdgeMax;
          } else {
            if ( (++pkEdge) > pkEdgeMax) pkEdge = (eGML_Polygon_Edge*) pkEdgeMin;
          }
          // check for final edge in this bucket
          if (pkEdge->m_bLeft != bLeft)
          {
            // remove edge from aet
            *ppAET = pkAEdge->m_pkNext;
            continue;
          }
          // activate next edge 
          pkAEdge->m_pkEdge = pkEdge;
          pkAEdge->m_iBottom = eC_ToInt(pkEdge->m_vY2); //ceil or floor ?
        }
        
        // clip spanborder to lineendpoint
        vX = pkEdge->m_vX;
        if (pkEdge->m_vDx < 0)
        {					
          if (vX < pkEdge->m_vX2) vX = pkEdge->m_vX2; 
        } else {
          if (vX > pkEdge->m_vX2) vX = pkEdge->m_vX2;
        }
        
        // store spanborder 
        pkEdge->m_vSpan = vX;
        
        // step edge
        pkEdge->m_vX += pkEdge->m_vIncX; 
        
        if (vX < vXold) 
        {
          // out of order span found [happens only in case of crossing(illegal) edges]
          // remove misplaced edge 
          *ppAET = pkAEdge->m_pkNext;
          // find proper place [usually just swap with previous]
          eGML_Polygon_ActiveEdge **ppAET2 = &pkAET;
          eGML_Polygon_ActiveEdge *pkAEdge2;
          while ((pkAEdge2 = *ppAET2) && (pkAEdge2->m_pkEdge->m_vSpan < vX)) ppAET2 = &(pkAEdge2->m_pkNext);					
          // reinsert edge
          pkAEdge->m_pkNext = pkAEdge2;					
          *ppAET2 = pkAEdge;
          // reget last spanposition [warning: assumes edge* is following next* in ActiveEdge struct]
          vXold = (*(eGML_Polygon_Edge**)(ppAET+1))->m_vSpan;
          
          eDBG_STAT_ADD(CrossingEdges, 1);
        } else {
          vXold = vX;
          // next edge
          ppAET = &(pkAEdge->m_pkNext);
        }							
      }
      
      // find and render spans
      pkAEdge = pkAET;			
      while (pkAEdge)
      {
        eC_Value vStartX, vStopX;
        
        vStartX = pkAEdge->m_pkEdge->m_vSpan;
        pkAEdge = pkAEdge->m_pkNext;
        if (pkAEdge)
        {
          vStopX = pkAEdge->m_pkEdge->m_vSpan - eC_FromFloat(0.5f);
		  if (vStopX > vStartX)
		  {	          
			#ifndef eGML_POLYGONALPHA_DISABLE
			if (ubAlpha != 0xff)
			{
				S1::RenderSpanAlpha( pBitmap, eC_FixFromInt(iYpos), eC_FixFromValue(vStartX), eC_FixFromValue(vStopX), pxColor, ubAlpha );
			} else 
			#endif
			{
				S1::RenderSpan( pBitmap, eC_FixFromInt(iYpos), eC_FixFromValue(vStartX), eC_FixFromValue(vStopX), pxColor );
			}
		  }
          
          pkAEdge = pkAEdge->m_pkNext;
        }
      }
      
      // next scanline
      iYpos++;
    } while (pkAET);
  }
};

#ifndef eGML_POLYGONTEXTURE_DISABLE
//---------------------------------------------------------------
// Polygondraw code for textured, not antialiased, simple concave polygons. 
//
template <
typename P1,		// Pixeltype (target&source - unless blendmode enforces a8 src)
typename P2,		// Offsettype for target
typename P3,		// Offsettype fpr source
class A1,			// Accessmethods for target
class A2,			// Accessmethods for source
class S1			// Spanrender methods
> 
class eGML_Polydraw_Tex_NoAA
{
public:
  
  static void Polygon(eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, 
    eGML_Texture *pkTexture, const eGML_Polygon_TextureFrame &kFrame, bool bPerspective, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
    ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif
    )
  {				
    eGML_Polygon_Edge		*akEdges;	
    eGML_Polygon_EdgeBucket	*akBuckets;
    eGML_Polygon_ActiveEdge	*akAEs;
    eC_UInt		uiEdgeCount = 0;
    eC_UInt		uiBucketCount = 0;		
    eC_UInt		uiAECount = 0;
    eGML_Polygon_EdgeBucket *pkFirstBucket = NULL;
    eGML_Polygon_ActiveEdge *pkAET = NULL;
    eGML_Polygon_Edge	   *pkEdge = NULL;
    const eGML_Polygon_Edge *pkEdgeMin,*pkEdgeMax,*pkLastEdge;
    const eGML_Coord2D *pkVertices = (eGML_Coord2D*) pvValues;
    eGML_Bitmap *pkTexMap;
#ifdef eGML_TEXTUREFLOAT
    eC_Float fU = 0.0f, fV = 0.0f, fUX = 0.0f, fVX = 0.0f;
    eC_Float fZ = 0.0f, fZX = 0.0f, fU0 = 0.0f, fV0 = 0.0f, fZ0 = 0.0f;
#else
    eC_Fixed fxZ = 0, fxZX = 0, fxZ0 = 0;
#endif
    eC_Fixed fxU = 0, fxV = 0, fxUX = 0, fxVX = 0, fxU0 = 0, fxV0 = 0;
    eC_UInt		i;
    eC_Value	vX0,vY0,vX1,vY1;
    
    // prepare edge buffer
    akEdges = eGML_PolyGetEdgeBuffer( uiCount+1 );
    
    // get last vertex
    vX0 = ((const eGML_Coord2D *)((const eC_UByte*)pkVertices + ((uiCount-1)*uiPitch)))->x + eC_FromFloat(0.1f);
    vY0 = ((const eGML_Coord2D *)((const eC_UByte*)pkVertices + ((uiCount-1)*uiPitch)))->y + eC_FromFloat(0.5f);
    
    // create edges from vertextable
    for (i=0; i<uiCount; i++)
    {
      eC_Value vDx,vDy;
      
      vX1 = pkVertices->x + eC_FromFloat(0.1f);;
      vY1 = pkVertices->y + eC_FromFloat(0.5f);;			
      vDx = vX0 - vX1;
      vDy = vY0 - vY1;
      
      // skip horizontals and dublicate vertices
      if ((eC_Floor(vY0) != eC_Floor(vY1)) && (eC_Abs(vDy) > eC_FromFloat(0.01f)))
      {
        // alloc edge
        pkEdge = &akEdges[uiEdgeCount++];
        
        // detect winding, add edge
        if (vY0 > vY1)
        {
          pkEdge->m_bLeft = true;
          pkEdge->m_vX1 = vX1;
          pkEdge->m_vY1 = vY1;
          pkEdge->m_vDx = vDx;
          pkEdge->m_vDy = vDy;
        } else {
          pkEdge->m_bLeft = false;
          pkEdge->m_vX1 = vX0;
          pkEdge->m_vY1 = vY0;
          pkEdge->m_vDx = -vDx;
          pkEdge->m_vDy = -vDy;
        }
        
        eC_Value vYoff;
        // find slope
        pkEdge->m_vIncX = eC_Div( pkEdge->m_vDx , pkEdge->m_vDy );
        
        // offset to lower pixelborder edge
        vYoff = eC_FromInt(1) - (pkEdge->m_vY1 - eC_Floor(pkEdge->m_vY1));
        // snap linestart to lower pixel edge						
        pkEdge->m_vX = pkEdge->m_vX1 + eC_Mul(vYoff, pkEdge->m_vIncX);
        pkEdge->m_vY = pkEdge->m_vY1 + vYoff;	
        // store endpos
        pkEdge->m_vX2 = pkEdge->m_vX1 + pkEdge->m_vDx; 
        pkEdge->m_vY2 = pkEdge->m_vY1 + pkEdge->m_vDy;
        // dont use lower edge xpos if outside line
        if (pkEdge->m_vDx < 0)
        {					
          if (pkEdge->m_vX < pkEdge->m_vX2) pkEdge->m_vX = pkEdge->m_vX2; 
        } else {
          if (pkEdge->m_vX > pkEdge->m_vX2) pkEdge->m_vX = pkEdge->m_vX2;
        }
      }
      
      // next edge
      vX0 = vX1;
      vY0 = vY1;
      pkVertices = (const eGML_Coord2D *) (((const eC_UByte*)pkVertices) + uiPitch);
    }
    
    // early out on invisible polys
    if (uiEdgeCount < 2) 
    {
      eDBG_STAT_ADD(RejectedPolygons, 1);
      return;
    }
    eDBG_STAT_ADD(PolyEdges, uiEdgeCount);        
    
    // mark end of edgelist
    pkEdgeMin = akEdges;
    pkEdgeMax = pkEdge;
    
    // prepare edgebucket buffer
    akBuckets = eGML_PolyGetBucketBuffer( uiEdgeCount );
    
    // create edgebuckets from edgelist
    pkFirstBucket = NULL;
    
    pkLastEdge = pkEdgeMin;
    eC_Bool bLastLeft = akEdges[0].m_bLeft;
    i=0;		
    do 
    {					
      // wraparound counter
      if ((++i) == uiEdgeCount) i=0;
      pkEdge = &akEdges[i];
      
      eC_Bool bLeft = pkEdge->m_bLeft;
      // detect corner 
      if ((bLastLeft) && (!bLeft))
      {			
        // alloc edgebucket
        eGML_Polygon_EdgeBucket *pkBucket = &akBuckets[uiBucketCount++];
        
        // set first edgepair
        pkBucket->m_pkLeftEdge = (eGML_Polygon_Edge*) pkLastEdge;
        pkBucket->m_pkRightEdge = pkEdge;		
        
        pkBucket->m_iTop = eC_ToInt( pkEdge->m_vY1 );	//top = y or top = y1 ?
        
        // insert bucket (sorted by top-y)		
        eGML_Polygon_EdgeBucket *pkNode = pkFirstBucket;
        eGML_Polygon_EdgeBucket **ppBase = &pkFirstBucket;
        while ((pkNode) && (pkNode->m_iTop < pkBucket->m_iTop))
        {		
          ppBase = &(pkNode->m_pkNext);
          pkNode = *ppBase;
        }
        
        *ppBase = pkBucket;
        pkBucket->m_pkNext = pkNode;
      }
      bLastLeft = bLeft;
      pkLastEdge = pkEdge;
    } while (i>0);
    
    // early out on illegal polys
    if (uiBucketCount < 1) 
    {
      eDBG_STAT_ADD(RejectedPolygons, 1);
      return;
    }
    
    eDBG_STAT_ADD(EdgeBuckets, uiBucketCount);
    eDBG_STAT_ADD(NumberOfPolygons, 1);
    
    // prepare texture [TODO: add support for mipmaps]        
    pkTexMap = pkTexture->GetBitmap();
    // get base offset
    P3 kTextureBase;
    A2::GetOffset( pkTexMap, 0, 0, kTextureBase );        
    // prepare wrapping masks
    eC_UInt uiMaskX = (pkTexture->GetWidthMask() << eC_FP_LBIT) | eC_FP_LOW;
    eC_UInt uiMaskY = (pkTexture->GetHeightMask() << eC_FP_LBIT) | eC_FP_LOW;
    eC_UInt uiShift = pkTexture->GetPitchShift();
    
    // prepare active edge table
    akAEs = eGML_PolyGetAETBuffer( uiEdgeCount );
    
    // topmost vertex
    eC_Int	iYpos = pkFirstBucket->m_iTop;
    eC_Fixed fxYtop = eC_FixFromInt( iYpos); //eC_FixFromValue( iYpos );
    
    if (bPerspective)
    {
      // prepare uv data        
#ifdef eGML_TEXTUREFLOAT
      fUX = kFrame.dat.flt.m_fUX;
      fVX = kFrame.dat.flt.m_fVX;
      fZX = kFrame.dat.flt.m_fZX;
      fZ0 = kFrame.dat.flt.m_fZ0 + kFrame.dat.flt.m_fZY * iYpos;
      fU0 = kFrame.dat.flt.m_fU0 + kFrame.dat.flt.m_fUY * iYpos;
      fV0 = kFrame.dat.flt.m_fV0 + kFrame.dat.flt.m_fVY * iYpos;
#else   
      fxUX = kFrame.dat.fix.m_fxUX;
      fxVX = kFrame.dat.fix.m_fxVX;
      fxZX = kFrame.dat.fix.m_fxZX;
      fxZ0 = kFrame.dat.fix.m_fxZ0 + eC_FixMul( kFrame.dat.fix.m_fxZY, fxYtop );
      fxU0 = kFrame.dat.fix.m_fxU0 + eC_FixMul( kFrame.dat.fix.m_fxUY, fxYtop );
      fxV0 = kFrame.dat.fix.m_fxV0 + eC_FixMul( kFrame.dat.fix.m_fxVY, fxYtop );
#endif
    } else {            
      // prepare uv data        
      fxUX = kFrame.dat.fix.m_fxUX;
      fxVX = kFrame.dat.fix.m_fxVX;
      fxU0 = kFrame.dat.fix.m_fxU0 + eC_FixMul( kFrame.dat.fix.m_fxUY, fxYtop );
      fxV0 = kFrame.dat.fix.m_fxV0 + eC_FixMul( kFrame.dat.fix.m_fxVY, fxYtop );
    }
    
    do
    {		
      // find new active edgebuckets 			
      eGML_Polygon_EdgeBucket *pkBucket;
      // enter only when ytop of currently first bucket is reached
      while ((pkBucket = pkFirstBucket) && (pkBucket->m_iTop <= iYpos))
      {
        // remove bucket
        pkFirstBucket = pkBucket->m_pkNext;		
        // assign bucket startedges
        eGML_Polygon_Edge *pkEdge1, *pkEdge2;
        eC_Value vDelta;
        // compare startpoint (inequal in case of removed flat edges)
        // and slope (in common case of equal startpoint) to decide order
        vDelta = pkBucket->m_pkLeftEdge->m_vX1 - pkBucket->m_pkRightEdge->m_vX1;
        if (( vDelta < 0) ||
          (( vDelta == 0 ) && ( pkBucket->m_pkLeftEdge->m_vIncX <= pkBucket->m_pkRightEdge->m_vIncX )))
        {
          pkEdge1 = pkBucket->m_pkLeftEdge;
          pkEdge2 = pkBucket->m_pkRightEdge;
        } else {
          pkEdge1 = pkBucket->m_pkRightEdge;
          pkEdge2 = pkBucket->m_pkLeftEdge;
        }
        
        // create edgecontainers
        eGML_Polygon_ActiveEdge	*pkActive1 = &akAEs[uiAECount++];
        eGML_Polygon_ActiveEdge	*pkActive2 = &akAEs[uiAECount++];
        pkActive1->m_pkEdge = pkEdge1;
        pkActive1->m_pkNext = pkActive2;
        pkActive1->m_iBottom = eC_ToInt(pkEdge1->m_vY2);  //ceil or floor ?
        pkActive2->m_pkEdge = pkEdge2;
        pkActive2->m_iBottom = eC_ToInt(pkEdge2->m_vY2);  //ceil or floor ?
        
        // find edge insert position (just one search per bucket)
        eGML_Polygon_ActiveEdge *pkNode;
        eGML_Polygon_ActiveEdge **ppAET = &pkAET;
        while ((pkNode = *ppAET) && (pkNode->m_pkEdge->m_vX <= pkEdge1->m_vX))
        {
          // specialcase equal m_vX by slope comparison
          if (pkNode->m_pkEdge->m_vX == pkEdge1->m_vX)
          {
            if (pkNode->m_pkEdge->m_vIncX > pkEdge1->m_vIncX) break;
          }
          ppAET = &(pkNode->m_pkNext);
        }
        
        // insert edges
        pkActive2->m_pkNext = pkNode;
        *ppAET = pkActive1;
      }
      
      // loop all active edges
      eGML_Polygon_ActiveEdge **ppAET = &pkAET;
      eGML_Polygon_ActiveEdge *pkAEdge;
      eC_Value				 vX,vXold=0;
      
      while ((pkAEdge = *ppAET))
      {
        pkEdge = pkAEdge->m_pkEdge;
        
        // check if edge is finished
        eC_Bool	bLeft = pkEdge->m_bLeft;
        if (iYpos >= pkAEdge->m_iBottom)
        {
          // advance to next edge [circular]
          if (bLeft)
          {
            if ( (--pkEdge) < pkEdgeMin) pkEdge = (eGML_Polygon_Edge*) pkEdgeMax;
          } else {
            if ( (++pkEdge) > pkEdgeMax) pkEdge = (eGML_Polygon_Edge*) pkEdgeMin;
          }
          // check for final edge in this bucket
          if (pkEdge->m_bLeft != bLeft)
          {
            // remove edge from aet
            *ppAET = pkAEdge->m_pkNext;
            continue;
          }
          // activate next edge 
          pkAEdge->m_pkEdge = pkEdge;
          pkAEdge->m_iBottom = eC_ToInt(pkEdge->m_vY2); //ceil or floor ?
        }
        
        // clip spanborder to lineendpoint
        vX = pkEdge->m_vX;
        if (pkEdge->m_vDx < 0)
        {					
          if (vX < pkEdge->m_vX2) vX = pkEdge->m_vX2; 
        } else {
          if (vX > pkEdge->m_vX2) vX = pkEdge->m_vX2;
        }
        
        // store spanborder 
        pkEdge->m_vSpan = vX;
        
        // step edge
        pkEdge->m_vX += pkEdge->m_vIncX; 
        
        if (vX < vXold) 
        {
          // out of order span found [happens only in case of crossing(illegal) edges]
          // remove misplaced edge 
          *ppAET = pkAEdge->m_pkNext;
          // find proper place [usually just swap with previous]
          eGML_Polygon_ActiveEdge **ppAET2 = &pkAET;
          eGML_Polygon_ActiveEdge *pkAEdge2;
          while ((pkAEdge2 = *ppAET2) && (pkAEdge2->m_pkEdge->m_vSpan < vX)) ppAET2 = &(pkAEdge2->m_pkNext);					
          // reinsert edge
          pkAEdge->m_pkNext = pkAEdge2;					
          *ppAET2 = pkAEdge;
          // reget last spanposition [warning: assumes edge* is following next* in ActiveEdge struct]
          vXold = (*(eGML_Polygon_Edge**)(ppAET+1))->m_vSpan;
          
          eDBG_STAT_ADD(CrossingEdges, 1);
        } else {
          vXold = vX;
          // next edge
          ppAET = &(pkAEdge->m_pkNext);
        }							
      }
      
      // find and render spans
      if (bPerspective)
      {
        // perspective corrected span loop
#ifdef eGML_TEXTUREFLOAT
        fU = fU0;
        fV = fV0;
        fZ = fZ0;
#else
        // scanline start UV
        fxU = fxU0;
        fxV = fxV0;
        fxZ = fxZ0;
#endif
        
        pkAEdge = pkAET;			
        while (pkAEdge)
        {
          eC_Value vStartX, vStopX;
          
          vStartX = pkAEdge->m_pkEdge->m_vSpan;
          pkAEdge = pkAEdge->m_pkNext;
          if (pkAEdge)
          {
            vStopX = pkAEdge->m_pkEdge->m_vSpan - eC_FromFloat(0.5f);
			if (vStopX > vStartX)
			{
#ifdef eGML_TEXTUREFLOAT
            S1::RenderSpanSubdivision( pBitmap, eC_FixFromInt(iYpos), eC_FixFromValue(vStartX), eC_FixFromValue(vStopX), 
              (P2)EGML_CAST( P2, kTextureBase), fU, fV, fZ, fUX, fVX, fZX, uiShift, uiMaskX, uiMaskY, rData
#ifdef eGML_TEXTURECHECK
              ,pTexturArea
#endif
              );
#else
            S1::RenderSpanSubdivision( pBitmap, eC_FixFromInt(iYpos), eC_FixFromValue(vStartX), eC_FixFromValue(vStopX), 
              (P2)EGML_CAST( P2, kTextureBase), fxU, fxV, fxZ, fxUX, fxVX, fxZX, uiShift, uiMaskX, uiMaskY, rData
#ifdef eGML_TEXTURECHECK
              ,pTexturArea
#endif
              );
#endif
			}
            
            pkAEdge = pkAEdge->m_pkNext;
          }
        }
        
#ifdef eGML_TEXTUREFLOAT
        fZ0 += kFrame.dat.flt.m_fZY;
        fU0 += kFrame.dat.flt.m_fUY;
        fV0 += kFrame.dat.flt.m_fVY;
#else
        fxZ0 += kFrame.dat.fix.m_fxZY;
        fxU0 += kFrame.dat.fix.m_fxUY;
        fxV0 += kFrame.dat.fix.m_fxVY;
#endif
      }
      else
      {
        // linear span loop
        
        // scanline start UV
        fxU = fxU0;
        fxV = fxV0;
        
        pkAEdge = pkAET;			
        while (pkAEdge)
        {
          eC_Value vStartX, vStopX;
          
          vStartX = pkAEdge->m_pkEdge->m_vSpan;
          pkAEdge = pkAEdge->m_pkNext;
          if (pkAEdge)
          {
            vStopX = pkAEdge->m_pkEdge->m_vSpan - eC_FromFloat(0.5f);
            
			if (vStopX > vStartX)
		    {	          
            S1::RenderSpan( pBitmap, eC_FixFromInt(iYpos), eC_FixFromValue(vStartX), eC_FixFromValue(vStopX), 
              (P2)EGML_CAST( P2, kTextureBase), fxU, fxV, fxUX, fxVX, uiShift, uiMaskX, uiMaskY, rData
#ifdef eGML_TEXTURECHECK
              ,pTexturArea
#endif
              );
			}
            
            pkAEdge = pkAEdge->m_pkNext;
          }
        }
        
        fxU0 += kFrame.dat.fix.m_fxUY;
        fxV0 += kFrame.dat.fix.m_fxVY;
      }
      
      // next scanline
      iYpos++;            
      
    } while (pkAET);
  }
};
#endif

//---------------------------------------------------------------
// Polygon draw code template. 
//
template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
class A1,			// Accessmethods for Destination
eC_Bool bMustBlendAA
> 

class eGML_Polydraw
{
public:	
  
  static void DrawPolygon( eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor )
  {
    eC_UByte ubAlpha = 255;
    eC_UInt uiNewCount;
    eC_Bool bTextured = false;
    P1 pxColor;     
    
    eDBG_PERF_BEGIN( Polygons );
    
	#ifndef eGML_POLYGONALPHA_DISABLE
    
    if (ulFlags & eGML_Bitmap::POLY_USE_RENDERSTATE)
    {
      // recieve alpha
      if ( pBitmap->GetState( eGML_RenderState::ALPHAENABLE ).bValue )
      {
        ubAlpha = (eC_UByte) ((pBitmap->GetState( eGML_RenderState::ALPHA )).uiValue);        
      } 
	  #ifndef eGML_POLYGONTEXTURE_DISABLE
      bTextured = pBitmap->GetScreen()->RenderState.GetBool( eGML_RenderState::TEXTUREENABLE );
	  #endif
    }  
	#endif

    pxColor = A1::ColorToPixel( kColor );     
    
    if (ulFlags & eGML_Bitmap::POLY_NOCLIP)
    {	
      if (ulFlags & eGML_Bitmap::POLY_EXTRUDE)
        eGML_Extrude_Polygon( (eC_UByte *)pvValues, uiPitch, uiCount,ulFlags & (eGML_Bitmap::POLY_HINT_CW|eGML_Bitmap::POLY_HINT_CCW) );

      // render direct
	  #ifndef eGML_POLYGONTEXTURE_DISABLE
      if (bTextured) renderTextured( pBitmap, pvValues, uiCount, uiPitch, pxColor ); else 
	  #endif
        if (ulFlags & eGML_Bitmap::POLY_ANTIALIAS) renderPlainAA( pBitmap, pvValues, uiCount, uiPitch, pxColor, ubAlpha );
											else renderPlain( pBitmap, pvValues, uiCount, uiPitch, pxColor, ubAlpha );
    } 
    else 
    {
      if (ulFlags & eGML_Bitmap::POLY_EXTRUDE)
        eGML_Extrude_Polygon( (eC_UByte *)pvValues, uiPitch, uiCount,ulFlags & (eGML_Bitmap::POLY_HINT_CW|eGML_Bitmap::POLY_HINT_CCW) );

      // prepare clipper
      eGML_Clipper::InitRegion( pBitmap->GetClipMinX(), pBitmap->GetClipMinY(), pBitmap->GetClipMaxX()+1, pBitmap->GetClipMaxY()+1 );
      // perform 2dclipping
      eGML_Coord2D *pkClipped = eGML_Clipper::ClipPolygon( pvValues, uiCount, uiPitch, uiNewCount );


      if (uiNewCount > 0)
      {
        // render clipped
		#ifndef eGML_POLYGONTEXTURE_DISABLE
        if (bTextured) renderTextured( pBitmap, pkClipped, uiNewCount, sizeof(eGML_Coord2D), pxColor ); else
		#endif
        
		if (ulFlags & eGML_Bitmap::POLY_ANTIALIAS) renderPlainAA( pBitmap, pkClipped, uiNewCount, sizeof(eGML_Coord2D), pxColor, ubAlpha );
											  else renderPlain( pBitmap, pkClipped, uiNewCount, sizeof(eGML_Coord2D), pxColor, ubAlpha );
      }
    }				
/*
    if (ulFlags & eGML_Bitmap::POLY_ANTIALIAS) 
    {
      eC_UByte *pubData = (eC_UByte *)pvValues;
      eGML_Vector2 kFirst= *((eGML_Vector2 *) (pubData+uiPitch*(uiCount-1)));

      for (eC_UInt uiI=0;uiI<uiCount;uiI++)
      {
        pBitmap->DrawLine(kFirst, *((eGML_Vector2 *)pubData), kColor,(ulFlags&(eGML_Bitmap::LINE_HQAA|eGML_Bitmap::LINE_ANTIALIAS))|eGML_Bitmap::LINE_WIDE, eC_FromFloat(1.5f));
        kFirst= *((eGML_Vector2 *)pubData) ;
        pubData+=uiPitch;
      }
    }
*/    
    eDBG_PERF_END( Polygons );
  }
  
private: 
  
  static inline void renderPlain(eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, P1 pxColor, eC_UByte ubAlpha )
  {
    eGML_Polydraw_NoAA< P1, eGML_HSpanRender<P1,P2,P1,A1> >::
      Polygon( pBitmap, pvValues, uiCount, uiPitch, pxColor, ubAlpha ); 
  }

  static inline void renderPlainAA(eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, P1 pxColor, eC_UByte ubAlpha )
  {
    eGML_Polydraw_AA< P1,P2,A1,bMustBlendAA>::
      Polygon( pBitmap, pvValues, uiCount, uiPitch, pxColor, ubAlpha ); 
  }
  
  #ifndef eGML_POLYGONTEXTURE_DISABLE
  static void renderTextured(eGML_Bitmap *pBitmap, const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, P1 pxColor )
  {        
    eGML_Polygon_TextureFrame *pkFrame = NULL;
	eGML_Polydraw_data pData = NULL;
    eGML_Texture *pkTexture;
    eC_Bool bPerspective;	


    // get active texture
    eGML_Screen *pkBase = pBitmap->GetScreen();
    pkTexture = (eGML_Texture *) pkBase->RenderState.GetPtr( eGML_RenderState::TEXTURE );
    // check for nulltexture
    if (!pkTexture) return;

	// check for alpha using blendmodes
	if (pkBase->RenderState.GetUInt( eGML_RenderState::TEXTUREBLENDING ) == eGML_RenderState::TB_COLOR)
	{
		eDBG_ASSERT(pkTexture->GetType()==eGML::BM_ALPHA8, "texture has to be alpha8 type for color blendmode");
		pData = (eGML_Polydraw_data) pkBase->RenderState.GetUInt( eGML_RenderState::COLOR1 );
	} else {
		if (pkTexture->GetType() != pBitmap->GetType())
		{
			eDBG_ASSERT(false, "texture format does not match destination format");
			return;
		}
	}
    
    // get texture matrix cache
    pkFrame = (eGML_Polygon_TextureFrame *) pkBase->RenderState.GetPtr( eGML_RenderState::I_TEXMATCACHE );
    
    switch (pkBase->RenderState.GetUInt( eGML_RenderState::TEXTUREMODE ))
    {

	case eGML_RenderState::TM_DIRECT:
	{
	  if (!pkBase->RenderState.GetBool( eGML_RenderState::I_TEXMATVALID ))
	  {
			// directly grab mapping parameters
			pkFrame->dat.fix.m_fxU0 = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINX ) );
			pkFrame->dat.fix.m_fxV0 = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINY ) );
			pkFrame->dat.fix.m_fxUX = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISX ) );
			pkFrame->dat.fix.m_fxVX = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISX ) );
			pkFrame->dat.fix.m_fxUY = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISY ) );
			pkFrame->dat.fix.m_fxVY = eC_FixFromValue( pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISY ) );

			// mark data as valid
			pkBase->RenderState.Set( eGML_RenderState::I_TEXMATVALID, true );
        }

		// do not use z interpolation
        bPerspective = false;
        break;
	}

    case eGML_RenderState::TM_PLANE2D:
    {
        if (!pkBase->RenderState.GetBool( eGML_RenderState::I_TEXMATVALID ))
        {
          // precalc 2d mapping parameters
          const eGML_Vector2 kB( pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINY ) );
          const eGML_Vector2 kU( pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISY ) );
          const eGML_Vector2 kV( pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISY ) );
          // common denominator
          
#ifdef eGML_TEXTURESHIFT
          eC_UInt uiShift = eGML_TEXTURESHIFT;

          eGML_Vector2 u;
          eGML_Vector2 v;

          if ( pkTexture->GetPitchShift()<uiShift )
            uiShift = pkTexture->GetPitchShift();
          
          if ( pkTexture->GetHeightShift()<uiShift )
            uiShift = pkTexture->GetHeightShift();

          u.x = eC_RShift(kU.x,uiShift);
          u.y = eC_RShift(kU.y,uiShift);
          
          v.x = eC_RShift(kV.x,uiShift);
          v.y = eC_RShift(kV.y,uiShift);
                    
          eC_Value n = u.Cross(v);
#else
          eC_Value n = kU.Cross(kV);
#endif
          
          
          
          if (n==0)
          {
			  eDBG_DP("ERROR: Illegal 2d textureframe. Check UAXIS and VAXIS renderstates.\n");
          }
          // prescale by texturesize
          eC_Value nu = eC_FromInt(1);
          eC_Value nv = eC_FromInt(1);
          
          if (n)
          {               
#ifdef eGML_TEXTURESHIFT            
            uiShift= uiShift<<1;
            nu = eC_Div( eC_FromInt(pkTexture->GetWidth()>>uiShift ) , n);
            nv = eC_Div( eC_FromInt(pkTexture->GetHeight()>>uiShift), n);
#else
            nu = eC_Div( eC_FromInt(pkTexture->GetWidth()) , n);
            nv = eC_Div( eC_FromInt(pkTexture->GetHeight()), n);
#endif
          }
          
          // U/V steps                
          pkFrame->dat.fix.m_fxUX = eC_FixFromValue( eC_Mul( kV.y, nu ) );
          pkFrame->dat.fix.m_fxVX = eC_FixFromValue( eC_Mul( kU.y, -nv ) );
          pkFrame->dat.fix.m_fxUY = eC_FixFromValue( eC_Mul( kV.x, -nu ) );
          pkFrame->dat.fix.m_fxVY = eC_FixFromValue( eC_Mul( kU.x, nv ) );
          // initial U/V for screenpos 0,0 pixelcenter 0.5,0.5
          
          pkFrame->dat.fix.m_fxU0 = eC_FixMul(eC_FixFromValue(eC_FromFloat(0.5f)-kB.x), pkFrame->dat.fix.m_fxUX) + eC_FixMul(eC_FixFromValue(eC_FromFloat(0.5f)-kB.y), pkFrame->dat.fix.m_fxUY);
          pkFrame->dat.fix.m_fxV0 = eC_FixMul(eC_FixFromValue(eC_FromFloat(0.5f)-kB.x), pkFrame->dat.fix.m_fxVX) + eC_FixMul(eC_FixFromValue(eC_FromFloat(0.5f)-kB.y), pkFrame->dat.fix.m_fxVY);
          // mark data as valid
          pkBase->RenderState.Set( eGML_RenderState::I_TEXMATVALID, true );
        }            
        
        // do not use z interpolation
        bPerspective = false;
        break;
      }
      
    case eGML_RenderState::TM_PLANE3D:
      {
        if (!pkBase->RenderState.GetBool( eGML_RenderState::I_TEXMATVALID ))
        {
          // precalc 3d mapping parameters
          const eGML_Vector3 kB( pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINY ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXORIGINZ ));
          const eGML_Vector3 kU( pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISY ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXUAXISZ ));
          const eGML_Vector3 kV( pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISX ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISY ),
            pkBase->RenderState.GetValue( eGML_RenderState::TEXVAXISZ ));
          
          eGML_Vector2 kCenter = pBitmap->GetViewCenter();
          const eGML_Matrix3 kScale = pBitmap->GetPrescaleMatrix();
          
		  //const eGML_Vector3 kRoot = eGML_Vector3( kScale[0][0], kScale[1][1], kScale[2][2] );
		  //// projected input
		  //eGML_Vector3 kBl = kB * kRoot;
		  //eGML_Vector3 kUl = kU * kRoot;
		  //eGML_Vector3 kVl = kV * kRoot;
          
		  // projected input already with float values
		  eC_Float kBl_x = eC_ToFloat(kB.x) * eC_ToFloat(kScale[0][0]);
		  eC_Float kBl_y = eC_ToFloat(kB.y) * eC_ToFloat(kScale[1][1]);
		  eC_Float kBl_z = eC_ToFloat(kB.z) * eC_ToFloat(kScale[2][2]);

		  eC_Float kUl_x = eC_ToFloat(kU.x) * eC_ToFloat(kScale[0][0]);
		  eC_Float kUl_y = eC_ToFloat(kU.y) * eC_ToFloat(kScale[1][1]);
		  eC_Float kUl_z = eC_ToFloat(kU.z) * eC_ToFloat(kScale[2][2]);

		  eC_Float kVl_x = eC_ToFloat(kV.x) * eC_ToFloat(kScale[0][0]);
		  eC_Float kVl_y = eC_ToFloat(kV.y) * eC_ToFloat(kScale[1][1]);
		  eC_Float kVl_z = eC_ToFloat(kV.z) * eC_ToFloat(kScale[2][2]);

	      // transformation matrix (calculation with float Values)
          eC_Float kSz_x = kUl_y * kVl_z - kUl_z * kVl_y;
          eC_Float kSz_y = kUl_z * kVl_x - kUl_x * kVl_z;
          eC_Float kSz_z = kUl_x * kVl_y - kUl_y * kVl_x;
          
          eC_Float kSu_x = kVl_y * kBl_z - kVl_z * kBl_y;
          eC_Float kSu_y = kVl_z * kBl_x - kVl_x * kBl_z;
          eC_Float kSu_z = kVl_x * kBl_y - kVl_y * kBl_x;
          
          eC_Float kSv_x = kBl_y * kUl_z - kBl_z * kUl_y;
          eC_Float kSv_y = kBl_z * kUl_x - kBl_x * kUl_z;
          eC_Float kSv_z = kBl_x * kUl_y - kBl_y * kUl_x;
          
          eC_Float kCenter_x=eC_ToFloat(kCenter.x);
          eC_Float kCenter_y=eC_ToFloat(kCenter.y);
          
          // apply projection center offset
          kSz_z = kSz_z - kCenter_x*kSz_x - kCenter_y*kSz_y;
          kSu_z = kSu_z - kCenter_x*kSu_x - kCenter_y*kSu_y;
          kSv_z = kSv_z - kCenter_x*kSv_x - kCenter_y*kSv_y;
          // premultiply texturewidth
          eC_UInt xs = pkTexture->GetWidth();
          eC_UInt ys = pkTexture->GetHeight();
          kSu_x *= xs; kSu_y *= xs; kSu_z *= xs;
          kSv_x *= ys; kSv_y *= ys; kSv_z *= ys;
          
#ifdef eGML_TEXTUREFLOAT
          // U/V steps
          pkFrame->dat.flt.m_fUX = kSu_x;
          pkFrame->dat.flt.m_fVX = kSv_x;
          pkFrame->dat.flt.m_fZX = kSz_x;
          pkFrame->dat.flt.m_fUY = kSu_y;
          pkFrame->dat.flt.m_fVY = kSv_y;            
          pkFrame->dat.flt.m_fZY = kSz_y; 
          // initial U/V for screenpos 0,0 pixelcenter 0,0
          pkFrame->dat.flt.m_fU0 = kSu_z;
          pkFrame->dat.flt.m_fV0 = kSv_z;
          pkFrame->dat.flt.m_fZ0 = kSz_z;
#else
          // scale into numerically safe range
          eC_Float s = 1.0f / ( (eC_ToFloat(kScale[0][0]) * eC_ToFloat(kScale[1][1]) * eC_ToFloat(kScale[2][2])) * (eC_ToFloat(kCenter.x) * eC_ToFloat(kCenter.y)) );
          eGML_Vector3 kSu = eGML_Vector3(eC_FromFloat(kSu_x * s ),eC_FromFloat(kSu_y * s),eC_FromFloat(kSu_z * s));
          eGML_Vector3 kSv = eGML_Vector3(eC_FromFloat(kSv_x * s ),eC_FromFloat(kSv_y * s),eC_FromFloat(kSv_z * s));
          eGML_Vector3 kSz = eGML_Vector3(eC_FromFloat(kSz_x * s ),eC_FromFloat(kSz_y * s),eC_FromFloat(kSz_z * s));
          
          // U/V steps
          pkFrame->dat.fix.m_fxUX = eC_FixFromValue( kSu.x );
          pkFrame->dat.fix.m_fxVX = eC_FixFromValue( kSv.x );
          pkFrame->dat.fix.m_fxZX = eC_FixFromValue( kSz.x );
          pkFrame->dat.fix.m_fxUY = eC_FixFromValue( kSu.y );
          pkFrame->dat.fix.m_fxVY = eC_FixFromValue( kSv.y );            
          pkFrame->dat.fix.m_fxZY = eC_FixFromValue( kSz.y );            
          // initial U/V for screenpos 0,0 pixelcenter 0,0
          pkFrame->dat.fix.m_fxU0 = eC_FixFromValue( kSu.z );
          pkFrame->dat.fix.m_fxV0 = eC_FixFromValue( kSv.z );
          pkFrame->dat.fix.m_fxZ0 = eC_FixFromValue( kSz.z );
#endif
          
          // mark data as valid
          pkBase->RenderState.Set( eGML_RenderState::I_TEXMATVALID, true );
        }
        
        // use z interpolation
        bPerspective = true;
        break;
      }
      
    default:
      // unkown texture mode
      return;
        }

#ifdef eGML_TEXTURECHECK
       eGML_Polygon_TextureTargetArea *pTexturArea = NULL;
       pTexturArea = (eGML_Polygon_TextureTargetArea*)pkBase->RenderState.GetPtr( eGML_RenderState::TEXTUREAREA );
#endif

		switch (pkBase->RenderState.GetUInt( eGML_RenderState::TEXTUREBLENDING ))
		{
			case eGML_RenderState::TB_NORMAL:
				{
					#ifndef eGML_POLYGONALPHA_DISABLE
					if ( pkBase->RenderState.GetBool( eGML_RenderState::ALPHAENABLE ) )
					{
						pData = (eGML_Polydraw_data) pkBase->RenderState.GetUInt( eGML_RenderState::ALPHA );
						eGML_Polydraw_Tex_NoAA< P1,P2,P2,A1,A1, eGML_HSpanRenderTex< P1,P2,A1, eGML_WritebackTex_Alpha<P1,P2,A1> > >::
							Polygon( pBitmap, pvValues, uiCount, uiPitch, pkTexture, *pkFrame, bPerspective, pData
#ifdef eGML_TEXTURECHECK
                     ,pTexturArea
#endif                     
                     );  
					} else 
					#endif
					eGML_Polydraw_Tex_NoAA< P1,P2,P2,A1,A1, eGML_HSpanRenderTex< P1,P2,A1, eGML_WritebackTex_Default<P1,P2,A1> > >::
						Polygon( pBitmap, pvValues, uiCount, uiPitch, pkTexture, *pkFrame, bPerspective, pData
#ifdef eGML_TEXTURECHECK
                  ,pTexturArea
#endif                  
                  );  
					break;
				}

			#ifndef eGML_TEXTUREBLEND_COLOR_DISABLE
			case eGML_RenderState::TB_COLOR:
				{
					#ifndef eGML_POLYGONALPHA_DISABLE
					if ( pkBase->RenderState.GetBool( eGML_RenderState::ALPHAENABLE ) )
					{
            pData = (eGML_Polydraw_data) ( ((eC_UInt)pData) | (pkBase->RenderState.GetUInt( eGML_RenderState::ALPHA) << 24 ) );						
						eGML_Polydraw_Tex_NoAA< P1,P2,eGML_OffsetA8,A1,eGML_PixelA8_Access, eGML_HSpanRenderTex< P1,P2,A1, eGML_WritebackTex_ColorAlpha<P1,P2,A1> > >::
							Polygon( pBitmap, pvValues, uiCount, uiPitch, pkTexture, *pkFrame, bPerspective, pData
#ifdef eGML_TEXTURECHECK
                     ,pTexturArea
#endif
                     );  
					} else 
					#endif
					eGML_Polydraw_Tex_NoAA< P1,P2,eGML_OffsetA8,A1,eGML_PixelA8_Access, eGML_HSpanRenderTex< P1,P2,A1, eGML_WritebackTex_Color<P1,P2,A1> > >::
				          Polygon( pBitmap, pvValues, uiCount, uiPitch, pkTexture, *pkFrame, bPerspective, pData
#ifdef eGML_TEXTURECHECK
                      ,pTexturArea
#endif
                      );  
					break;
				}
			#endif

			#ifndef eGML_TEXTUREBLEND_ADD_DISABLE
			case eGML_RenderState::TB_ADD :
				{
					eGML_Polydraw_Tex_NoAA< P1,P2,P2,A1,A1, eGML_HSpanRenderTex< P1,P2,A1, eGML_WritebackTex_Additive<P1,P2,A1> > >::
						Polygon( pBitmap, pvValues, uiCount, uiPitch, pkTexture, *pkFrame, bPerspective, pData
#ifdef eGML_TEXTURECHECK
                  ,pTexturArea
#endif
                  );  
					break;
				}
			#endif

			default:
				// unknown blending mode
				return;
		}
    }
	#endif
    
};


//---------------------------------------------------------------	
#endif


