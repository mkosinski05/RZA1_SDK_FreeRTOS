/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Linedraw.h
* %version:        134.0 %
* 
* author:          hh04027
* creation date:   Mon Nov 28 10:02:39 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
28-08-08     | hh04058       | Changed clipping calls for antialised linedrawing between adjacent neighbor bmps
******************************************************************************/


#ifndef EGML_LINEDRAW__H_
#define EGML_LINEDRAW__H_

//---------------------------------------------------------------
#include "eDBG_Statistic.h"
#include "eC_Math.h"
#include "eGML_Vector2.h"
#include "eGML_Bitmap.h"
#include "eGML_Spandraw.h"
#include "eGML_Clipping.h"
#include "eGML_RenderState.h"

//---------------------------------------------------------------
void eGML_InitLineFilter();

//---------------------------------------------------------------
eDBG_STAT_DEF(NumberOfLines);
eDBG_STAT_DEF(RejectedLines);
eDBG_STAT_DEF(LineWidth);
eDBG_STAT_DEF(LineLength);

eDBG_PERF_DEF(WideLines);
eDBG_PERF_DEF(ThinLines);
eDBG_PERF_DEF(LineClipping);

//---------------------------------------------------------------
/**
@memo
Line attributes.
@doc
Attributs of lines to be drawn, e. g. by <TT><B>DrawLine</B></TT> method of \Ref{eGML_Bitmap} class.
@see eGML_Bitmap
*/
enum eGML_Lineflags
{
	/// No special attributes are set.
	eGML_LF_NONE   = 0,
	/// Apply linewidth in direction of line.
	eGML_LF_WIDE_S = eGML_FLAG_STYLE * 1, 
	/// Apply linewidth perpendicular to line.
	eGML_LF_WIDE_T = eGML_FLAG_STYLE * 2, 
	/// Combination of both kinds of linewidth. Result of bitwise <B>OR</B>-operation with \Ref{eGML_LF_WIDE_S} and \Ref{eGML_LF_WIDE_T}.
	eGML_LF_WIDE   = eGML_FLAG_STYLE * 3, 
	/// Apply half linewidth in direction of line.
	eGML_LF_WIDE_2 = eGML_FLAG_STYLE * 4, 
	/// Use antialiasing.
	eGML_LF_ANTIALIAS   = eGML_FLAG_FILTER * 1, 
	/// Use filtered (gaussian) antialiasing (AA). But be careful because there might be a performance impact.
	eGML_LF_HQ_AA       = eGML_FLAG_FILTER * 2, 
	/// Use rounded corner for startpoint. Do not combine with \Ref{eGML_LF_WIDE_S}.
	eGML_LF_ROUND_START = eGML_FLAG_LOCAL * 1, 
	/// Use rounded corner for endpoint. Do not combine with \Ref{eGML_LF_WIDE_S}.
	eGML_LF_ROUND_END   = eGML_FLAG_LOCAL * 2, 
	/// Skip clipping, line is guaranteed to be on screen.
	eGML_LF_DO_NOT_CLIP = eGML_FLAG_TRANSFORM * 1, 
	/// Pattern on line.
	eGML_LF_PATTERN = eGML_FLAG_STYLE * 8, 
	/// Automatically advance linepattern offset
	eGML_LF_ADVANCEPATTERN = eGML_FLAG_STYLE * 16, 
	/// Adjust pattern scale with respect to reference line length (used for 3d pattern display)
	eGML_LF_RESCALEPATTERN = eGML_FLAG_STYLE * 32, 
	// Thicklines are are clipped in two part, on inside the rect and one crossing the rect.
	// the crossing thickline part has an 0.5 offset (unknown, where it actually comes from!! The major problem is, that everything has to work in subpixel accuracy )
	eGML_LF_AA_CLIP_END_PART_CORRECTION = eGML_FLAG_STYLE * 64 
};

//---------------------------------------------------------------
// 1d axis clipping intervall
struct eGML_Line_clipRange
{
	eC_Fixed    fxMin, fxMax;
};

#ifndef eGML_LINEPATTERN_DISABLE
//---------------------------------------------------------------
template <typename T>
class eGML_PixelTypeTrait
{
public:
	enum is_pixel_type { value = 1 };
};

//---------------------------------------------------------------
template <>
class eGML_PixelTypeTrait<eGML_Line_patternStyle *>
{
public:
	enum is_pixel_type { value = 0 };
};
#endif

//---------------------------------------------------------------
// Linedraw code template. 
//
template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
class A1,			// Accessmethods for Destinatio
eC_Bool bMustBlendAA // true=always blend when AA is enabled (no putpixel/putspan)
> 

class eGML_Linedraw
{
public:

	// general entrypoint
	static void DrawLine( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth );

	// special function for endpoint rendering (make private ?)
	static void DrawDot( eGML_Bitmap *pkDest, eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlag );

private:

	// instance octant templates for thin lines that are already clipped
	static void ThinLineClipped( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, void *pvStyle, eC_Value vXmax, eC_Value vYmax, eC_Value vXmin = 0, eC_Value vYmin = 0, eC_Flag uiCode = 0);

	// instance octant templates for wide lines that are already clipped
	static void WideLineClipped( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor,
		eC_Value vStartWidth, eC_Value vEndWidth, eC_Flag ulFlags, void *pvStyle );	

	// instance octant templates for wide lines that cross one or more clipping borders
	static void WideLineOnEdge( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor,
		eC_Value vStartWidth, eC_Value vEndWidth, eC_Flag ulFlags, void *pvStyle);	
};


#include "eGML_Dotdraw.h"


typedef eGML_Line_clipRange clipRange;

// used to carry all interpolation parameters needed in inner rasterloop
struct rasterData
{
	eC_Fixed    fxPosMaj, fxMid1Maj, fxMid2Maj, fxEndMaj;
	eC_Fixed    fxPosMin, fxIncMin;
	eC_Fixed    fxSpan, fxSpanInc;
};

// used to carry all positional parameters needed in setup
struct octantData
{
	eC_Fixed    fxStartMaj, fxStartMin;
	eC_Fixed    fxDeltaMaj, fxDeltaMin;
	eC_Fixed    fxNormalMaj, fxNormalMin;
	eC_Fixed    fxLen;
	// used in clipping case only
	clipRange   kMajClip;
	clipRange   kMinClip;
};



//---------------------------------------------------------------
// Linedraw code for single octant. 
// Assumes MajorAxis Delta > MinorAxis Delta
//         MajorAxis Delta > 0
//
template <
typename L1,		// Linestyle type
class S1			// Spanrender methods
> 

class eGML_Linedraw_Octant
{

public:
	// function pointer to inner raster loop
	typedef void (*rasterLoop)( eGML_Bitmap *, const L1 &, const rasterData &, const octantData &, const eC_Flag &); 

protected:



	// available raster loops (each loop renders an entire line)
	static void rasterLong( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0);
	static void rasterLongV( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0);
	static void rasterShort( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0);
	static void rasterShortClip( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0);
#ifndef eGML_LINEPATTERN_DISABLE
	static void rasterPatternV( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0 );
	static void rasterPatternClip( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags = 0 );
#endif

public:
	// standard interface Thinline octant
	static inline void ThinLineOctant( eGML_Bitmap *pBitmap, eC_Value vStartMaj, eC_Value vStartMin, 
		eC_Value vDeltaMaj, eC_Value vDeltaMin, const L1 &kStyle, eC_Flag ulFlags, eC_Value vMajmax, eC_Value vMinmax, eC_Value vMajMin, eC_Value vMinMin, eC_Flag uiCode = 0);	

	// internal version, using preprocessed inputs :
	static void ThinLineInner( eGML_Bitmap *pBitmap, eC_Fixed fxStartMaj, eC_Fixed fxStartMin, eC_Fixed fxIncMin, 
		eC_Fixed fxDeltaMaj, eC_Fixed fxDeltaMin, const L1 &kStyle, eC_Flag ulFlags, eC_Fixed fxMaxMaj, eC_Fixed fxMaxMin, eC_Fixed fxMajMin, eC_Fixed fxMinMin, eC_Flag uiCode = 0);


	// standard interface Wideline octant
	static inline void WideLineOctant( eGML_Bitmap *pBitmap, eC_Value vStartMaj, eC_Value vStartMin, 
		eC_Value vDeltaMaj, eC_Value vDeltaMin, 
		const L1 &kStyle, eC_Value vStartWidth, eC_Value vDeltaWidth, eC_Flag ulFlags );

	// standard interface clipped wideline octant
	static void WideLineOctantClip( eGML_Bitmap *pBitmap, eC_Value vStartMaj, eC_Value vStartMin, 
		eC_Value vDeltaMaj, eC_Value vDeltaMin, const L1 &kStyle, 
		eC_Value vStartWidth, eC_Value vDeltaWidth, eC_Flag ulFlags,
		const clipRange &kMajClip, const clipRange &kMinClip);

	// internal version, using preprocessed inputs :
	static inline void WideLineInner( eGML_Bitmap *pBitmap, octantData &kPos, eC_Fixed fxIncMin, const L1 &kStyle, 
		eC_Fixed fxLineWidth, eC_Fixed fxWidthFactor, eC_Flag ulFlags, rasterLoop pRaster);	
};

//---------------------------------------------------------------
template <typename P1, typename P2, class A1, eC_Bool bMustBlendAA> 
void eGML_Linedraw<P1,P2,A1,bMustBlendAA>::DrawLine( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, 
									   eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth )
{
	eC_Value vXmin, vXmax, vYmin, vYmax, vLineWidth, vBorder;
	eGML_Vector2 kA, kB;
	eC_Flag uiCode;
	eC_Bool bThin;
#ifndef eGML_LINEPATTERN_DISABLE
	eGML_Line_patternStyle kPattern( kStart );
#define PATTERN_REF &kPattern
#else
	ulFlags &= ~eGML_LF_PATTERN;
#define PATTERN_REF NULL
#endif

	eDBG_STAT_ADD(NumberOfLines, 1);


	// width correction if filtered		

#ifdef eGML_USE_HQ_THICKLINES
#ifdef eGML_FILTEREDAA
	if (ulFlags & eGML_LF_HQ_AA)
	{
		vStartWidth += eC_FromFloat( 0.25f );
#ifdef eGML_VARIWIDTH
		vEndWidth += eC_FromFloat( 0.25f );
#endif
	} else {
#endif
		if (!(ulFlags & eGML_LF_ANTIALIAS))
		{
			vStartWidth -= eC_FromFloat( 0.5f );
#ifdef eGML_VARIWIDTH
			vEndWidth -= eC_FromFloat( 0.5f );
#endif
		}
#ifdef eGML_FILTEREDAA
	}
#endif

#else
		
		/* why Ceiling?
			the problem is, that linewidths + [0.0 ...0.999999] e.G. 2.45 make no sense in NO_AA modes
			the linepositions of a NO_AA line is corrected by -0.5 to even out the visual appearance of
			NO_AA, AA and HQ_AA lines. This causes the a 2.5 line to move to about 3.0. The spanhight
			is calculated by the normalvector of the linewidth. The normalvector is calculated by
			
			v = {(end.x - start.x),(end.y - start.y)} 
			n = normalize(v) 
			legthOf(no) of a horizontal or vertical line is about 1.0 +/- 0.0001
			
			Finally we get varying minorpositions in the span rendere, since it 
			round (minPos) e.G. round(3.001) = 3 or round(2.9999) = 2
			For Linewidths 1,2,3,4,5 ... we get something like this round(2.5111) and round (2.49999) which are both 2
			
			The clean solution would be to round the starpositions correctly like this
			SpanMinPos = ceil(minPos - spanlength - 0.5 + frac(spanlength)/2)
			
			But, since we should use integer widths anyways in NO_AA mode, we can just round the linewidth here
		*/
		if (!(ulFlags & eGML_LF_ANTIALIAS))
		{
			vStartWidth = eC_Ceil(vStartWidth - eC_FromFloat(0.5f)); // rounding correction for 2 +/- [0.0 ... 0.9999] lines
			vStartWidth -= eC_FromFloat( 0.5f);	// visual correction, since the human receiption thinks, that AA_LINES look a little bit smaller than NO_AA Lines
#ifdef eGML_VARIWIDTH
			vEndWidth = eC_Ceil(vEndWidth - eC_FromFloat(0.5f));
			vEndWidth -= eC_FromFloat( 0.5f );			
#endif
			}
			
#endif


	// find maximum width
	vLineWidth = vStartWidth;
#ifdef eGML_VARIWIDTH
	if (vEndWidth > vLineWidth) vLineWidth = vEndWidth;		
#endif
	// guarantee minimum width
	if (vLineWidth < eC_FromFloat(1.0f)) 
	{
		vStartWidth = vEndWidth = vLineWidth = eC_FromInt(1);
	}

	if (vLineWidth < eC_FromFloat(3.0f)) // use eGML_ROUND_LINEEND_THRESHOLD ?
	{
		ulFlags &= ~(eGML_LF_ROUND_START | eGML_LF_ROUND_END);
		ulFlags |= eGML_LF_WIDE_S; //extrude instead of rounding for small lines
	}

	bThin = (vLineWidth <= eC_FromFloat(1.0)) && ((ulFlags & eGML_LF_PATTERN)==0);		/*DEBUG*/

	eDBG_STAT_ADD(LineWidth, eC_ToFloat(vLineWidth));    

#ifndef eGML_LINEPATTERN_DISABLE
	// prepare pattern style
	if (ulFlags & eGML_LF_PATTERN)
	{
		eGML_Screen *pkBase = pBitmap->GetScreen();
		eC_Value vScale,vOffset,vLen;

		kPattern.pkPattern = (eGML_Pattern *) pkBase->RenderState.GetPtr( eGML_RenderState::LINEPATTERN );                
		vScale = pkBase->RenderState.GetValue( eGML_RenderState::I_LINPATFACTOR );
		vOffset = pkBase->RenderState.GetValue( eGML_RenderState::LINEPATTERNOFFSET );		

		if (ulFlags & (eGML_LF_RESCALEPATTERN | eGML_LF_ADVANCEPATTERN))
		{
			vLen = eC_Len2(kEnd.x-kStart.x, kEnd.y-kStart.y);

			if (vLen>0)
			{
				if (ulFlags & eGML_LF_RESCALEPATTERN)
				{
					// scale pattern based on length/reflength quotient
					eC_Value vRefLen = pkBase->RenderState.GetValue( eGML_RenderState::I_LINPATREFLEN );
					if (ulFlags & eGML_LF_ADVANCEPATTERN)
					{
						// this could perhaps be done quicker.
						pkBase->RenderState.Set( eGML_RenderState::LINEPATTERNOFFSET, vOffset + eC_Mul( vScale, vRefLen ) );
					}
					if (vRefLen != vLen)
					{
						vScale = eC_Mul(vScale, eC_Div(vRefLen, vLen) );
					}
				} 
				else 
				{
					if (ulFlags & eGML_LF_ADVANCEPATTERN)
					{
						// this could perhaps be done quicker.
						pkBase->RenderState.Set( eGML_RenderState::LINEPATTERNOFFSET, vOffset + eC_Mul( vScale, vLen ) );
					}
				}
			}
		}

		// store pattern parameters
		kPattern.fxTexMul = eC_FixFromValue( vScale );
		kPattern.fxTexOffset = eC_FixFrac( eC_FixFromValue( vOffset ) );
		kPattern.aclColorBase = kPattern.pkPattern->GetColors();
		kPattern.bFlip = false;
	}
#endif

	// check for clip hint
	if (ulFlags & eGML_LF_DO_NOT_CLIP)
	{
		// force unclipped renderung
		if (bThin)
		{
			ThinLineClipped( pBitmap, kStart, kEnd, kColor, ulFlags, PATTERN_REF, 2048, 2048 );		
		} else {
			WideLineClipped( pBitmap, kStart, kEnd, kColor, vStartWidth, vEndWidth, ulFlags, PATTERN_REF );

			// check for rounded endpoints
			if (ulFlags & eGML_LF_ROUND_START) DrawDot( pBitmap, kStart.x, kStart.y, vStartWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK );               
			if (ulFlags & eGML_LF_ROUND_END) DrawDot( pBitmap, kEnd.x, kEnd.y, vEndWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK );
		}
		return;
	} else {
		// calc clipping rect for this linewidth 
		vXmin = eC_FromInt(pBitmap->GetClipMinX());
		vYmin = eC_FromInt(pBitmap->GetClipMinY());
		vXmax = eC_FromInt(pBitmap->GetClipMaxX());
		vYmax = eC_FromInt(pBitmap->GetClipMaxY());
		// find guardband (border = lw / sqrt(2) )
		if (bThin)
		{
			vBorder = 0; 			
		} else  {
			vBorder = eC_Ceil( eC_MulQ( vLineWidth, eC_FromFloat(0.708f) ) );
			vXmin -= vBorder;
			vYmin -= vBorder;
			vXmax += vBorder - eC_FromInt(1);
			vYmax += vBorder - eC_FromInt(1);
		}

		// calc clipped endpoints
		eDBG_PERF_BEGIN(LineClipping);
		eGML_Clipper::InitRegionV(vXmin, vYmin, vXmax, vYmax);

		kA = kStart;
		kB = kEnd;        

#ifdef eGML_VARIWIDTH
		if (vStartWidth != vEndWidth) 
		{
			// parametric clip
			eC_Value vTa, vTb;

			uiCode = eGML_Clipper::ClipLinePara( kA, kB, vTa, vTb );
			if (uiCode & eGML_Clipper::CF_CLIPPED)
			{
				eC_Value vDeltaWidth = vEndWidth - vStartWidth;

				// width correction
				if (vTb < eC_FromInt( 1 )) vEndWidth = vStartWidth + eC_Mul( vTb, vDeltaWidth );
				if (vTa > eC_FromInt( 0 )) vStartWidth += eC_Mul( vTa, vDeltaWidth );
			}

		} else {
#endif
			/* Bugfix:
			we need to define an "overdraw"-region, to reconstruct AA-Pixels on clippingborders
			-4 for 4 pixel HQ_AA
			-2 for 2 pixel AA
			-1 for NO_AA (since somehow the line is 1px too short on small clipborder-to-line-angles 
			*/
			if (ulFlags & eGML_LF_HQ_AA){
				eGML_Clipper::SetOverFlowMargin(eC_FromFloat(5.0f)); // 4.0f + 1.0f (safty reason, to compensate possible rounding errors)
				uiCode = eGML_Clipper::ClipLine( kA, kB, true, eC_FromFloat(-4.0f),eC_FromFloat(-4.0f),eC_FromFloat(4.0f),eC_FromFloat(4.0f)); // saftymargin needs to be at least the size of the span, which is used for antialiasing
			} else {
				if (ulFlags & eGML_LF_ANTIALIAS){
					eGML_Clipper::SetOverFlowMargin(eC_FromFloat(3.0f));
					uiCode = eGML_Clipper::ClipLine( kA, kB, true, eC_FromFloat(-2.0f),eC_FromFloat(-2.0),eC_FromFloat(2.0),eC_FromFloat(2.0));
				} else {
					eGML_Clipper::SetOverFlowMargin(eC_FromFloat(1.0f)); // safty first! prevents possible bufferoverlows during rasterization
					uiCode = eGML_Clipper::ClipLine( kA, kB,true, eC_FromFloat(-1.0),eC_FromFloat(-1.0),eC_FromFloat(1.0),eC_FromFloat(1.0));
				}
			}

#ifdef eGML_VARIWIDTH
		}
#endif

		// exact endpoint clipping

#ifdef eGML_THICKLINE_ACCELEARTION
		if ((vBorder > 0) && (uiCode & eGML_Clipper::CF_VIS))
		{
			eC_Value vTa, vTb;

			// line is inside guardband, find inner clip region
			vBorder *= 2;
			vXmin += vBorder;
			vYmin += vBorder;
			vXmax -= vBorder;
			vYmax -= vBorder;
            eGML_Clipper::InitRegionV(vXmin + eC_FromInt(4), vYmin + eC_FromInt(4), vXmax - eC_FromInt(4), vYmax - eC_FromInt(4));

			eGML_Vector2 kA2 = kA;
			eGML_Vector2 kB2 = kB;
			eC_Value     vStartWidth2 = vStartWidth;
			eC_Value     vEndWidth2 = vEndWidth;
			uiCode = eGML_Clipper::ClipLinePara( kA, kB, vTa, vTb );
#ifdef eGML_VARIWIDTH

			// inner clip width correction
			if ((vStartWidth != vEndWidth) && (uiCode != eGML_Clipper::CF_NOTVIS))
			{
				eC_Value vDeltaWidth = vEndWidth - vStartWidth;            

				if (vTb < eC_FromInt( 1 )) vEndWidth = vStartWidth + eC_Mul( vTb, vDeltaWidth );
				if (vTa > eC_FromInt( 0 )) vStartWidth += eC_Mul( vTa, vDeltaWidth );
			}

#endif
			if (uiCode == eGML_Clipper::CF_NOTVIS)
			{
				// mainpart not visible				
				WideLineOnEdge( pBitmap, kA2, kB2, kColor, vStartWidth2, vEndWidth2, ulFlags | eGML_LF_WIDE_S|eGML_LF_WIDE_2|eGML_LF_AA_CLIP_END_PART_CORRECTION, PATTERN_REF);
				// check for rounded endpoints
				if (ulFlags & eGML_LF_ROUND_START) DrawDot( pBitmap, kStart.x, kStart.y, vStartWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK);
				if (ulFlags & eGML_LF_ROUND_END)   DrawDot( pBitmap, kEnd.x, kEnd.y, vEndWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK);
			} else {
				// mainpart and clippart visible
				if (kA2 != kA) {					
					WideLineOnEdge( pBitmap, kA2, kA, kColor, vStartWidth2, vStartWidth, ulFlags | eGML_LF_WIDE_S|eGML_LF_WIDE_2|eGML_LF_AA_CLIP_END_PART_CORRECTION, PATTERN_REF);                
				}
				if (kB2 != kB) {					
					WideLineOnEdge( pBitmap, kB, kB2, kColor, vEndWidth, vEndWidth2, ulFlags | eGML_LF_WIDE_S|eGML_LF_WIDE_2|eGML_LF_AA_CLIP_END_PART_CORRECTION, PATTERN_REF);
				}
			}
		}
#endif

		if (uiCode & eGML_Clipper::CF_VIS)
		{
			// render clipped line
			if (bThin) {
				ThinLineClipped( pBitmap, kA, kB, kColor, ulFlags, PATTERN_REF, vXmax, vYmax, vXmin, vYmin, uiCode );
			} else {			

				
#ifdef eGML_THICKLINE_ACCELEARTION
				WideLineClipped( pBitmap, kA, kB, kColor, vStartWidth, vEndWidth, ulFlags, PATTERN_REF ); // fast, but has visual artifacts on overlapping lineregions
#else
				WideLineOnEdge( pBitmap, kA, kB, kColor, vStartWidth, vEndWidth, ulFlags | eGML_LF_WIDE_S|eGML_LF_WIDE_2|eGML_LF_AA_CLIP_END_PART_CORRECTION, PATTERN_REF ); // pixel-wiseclipping (slow, but prevents visual artifacts
#endif
				// check for rounded endpoints
				if (ulFlags & eGML_LF_ROUND_START) DrawDot( pBitmap, kStart.x, kStart.y, vStartWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK);
				if (ulFlags & eGML_LF_ROUND_END) DrawDot( pBitmap, kEnd.x, kEnd.y, vEndWidth/2, kColor, ulFlags & eGML_FLAG_FILTER_MASK);
			}
		} else {	
			eDBG_STAT_ADD(RejectedLines, 1);		        
		}
	}
}

#include "eGML_Thinline.h"
#include "eGML_Wideline.h"
#include "eGML_Rasterline.h"

//---------------------------------------------------------------	
#endif

