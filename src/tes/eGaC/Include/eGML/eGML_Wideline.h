/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Wideline.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:21:00 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
  28-08-08     | hh04058       | - added an 0.5f offset for HQ_AA clipped-thick-endlines,
                                 because the endparts of a clipped thickline are drawn
								 as simple AA lines for performance reasons
								 - since all line segements overlap slightly
								 the AA-Pixels are drawn multiple times. This leads
								 to optlical buckls at the overlapping intersections (no fix planned)
								 To get better optical results, undefine eGML_THICKLINE_ACCELEARTION
								 in the eGML_Config.h. This prevents the linesegmentation, but is about 20% slower.
								 (This will lead to an average 10% performance-loss, since the more expensive
								 methode will only be called for clipped lines)

******************************************************************************/

#ifndef EGML_WIDELINE__H_
#define EGML_WIDELINE__H_

//---------------------------------------------------------------
// entrypoint for clipped (and entierly inside guardband) wide
// lines. does the octant splitting and antialias codepath selection
//
template <typename P1, typename P2, class A1, eC_Bool bMustBlendAA>
void eGML_Linedraw<P1,P2,A1,bMustBlendAA>::WideLineClipped( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, 
                                                           eGML_Color kColor, eC_Value vStartWidth, eC_Value vEndWidth, eC_Flag ulFlags, void *pvStyle )
{		
	eC_Value x, y, dx, dy;
	eC_Value dw = 0;

	eDBG_PERF_BEGIN( WideLines );

	// cast color
	P1 pxColor = A1::ColorToPixel( kColor );

	// calc deltas
	dx = kEnd.x - kStart.x;
	dy = kEnd.y - kStart.y;
	#ifdef eGML_VARIWIDTH
	dw = vEndWidth - vStartWidth;
	#endif

	// find major axis
	if (eC_Abs(dx) > eC_Abs(dy))
	{
		// major X, orient left->right
		if (dx < eC_FromInt(0))	
		{
			x = kEnd.x;
			y = kEnd.y;
            dx = -dx;
			dy = -dy;
			#ifdef eGML_VARIWIDTH
			dw = -dw;
			vStartWidth = vEndWidth;
			#endif	
			#ifndef eGML_LINEPATTERN_DISABLE
            if (ulFlags & eGML_LF_PATTERN)
            {
                eGML_Line_patternStyle *pkStyle = (eGML_Line_patternStyle*) pvStyle;
                pkStyle->bFlip = true;
            }
			#endif
		} else {
			x = kStart.x;
			y = kStart.y;				
		}

		#ifndef eGML_LINEPATTERN_DISABLE
        if (ulFlags & eGML_LF_PATTERN)
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender_HQAA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    WideLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
		    }
        }
        else
		#endif
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<P1, eGML_VSpanRender_HQAA<P1,P2,P1,A1> >::
					    WideLineOctant( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_VSpanRender_AA<P1,P2,P1,A1> >::
					    WideLineOctant( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_VSpanRender<P1,P2,P1,A1> >::
				    WideLineOctant( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags ); 
		    }
        }
	}
	else 
	{
		// major Y, orient top->bottom
		if (dy < eC_FromInt(0))
		{
			x = kEnd.x;
			y = kEnd.y;
            dx = -dx;
			dy = -dy;
			#ifdef eGML_VARIWIDTH
			dw = -dw;
			vStartWidth = vEndWidth;
			#endif				
			#ifndef eGML_LINEPATTERN_DISABLE
            if (ulFlags & eGML_LF_PATTERN)
            {
                eGML_Line_patternStyle *pkStyle = (eGML_Line_patternStyle*) pvStyle;
                pkStyle->bFlip = true;
            }			
			#endif
		} else {
			x = kStart.x;
			y = kStart.y;
		}			

		#ifndef eGML_LINEPATTERN_DISABLE
        if (ulFlags & eGML_LF_PATTERN)
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender_HQAA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    WideLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags ); 
		    }
        }
        else
		#endif
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<P1, eGML_HSpanRender_HQAA<P1,P2,P1,A1> >::
					    WideLineOctant( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_HSpanRender_AA<P1,P2,P1,A1> >::
					    WideLineOctant( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_HSpanRender<P1,P2,P1,A1> >::
				    WideLineOctant( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags ); 
		    }
        }
	}

	eDBG_PERF_END( WideLines );
}


//---------------------------------------------------------------
// entrypoint for clipped, but 'on edge' wide lines. does the 
// octant splitting and antialias codepath selection (3x)
//
template <typename P1, typename P2, class A1, eC_Bool bMustBlendAA>
void eGML_Linedraw<P1,P2,A1,bMustBlendAA>::WideLineOnEdge( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, 
                                                          eGML_Color kColor, eC_Value vStartWidth, eC_Value vEndWidth, eC_Flag ulFlags, void *pvStyle)
{		
	eC_Value x, y, dx, dy;
	eC_Value dw = 0;
    eGML_Line_clipRange kXclip, kYclip;


	// cast color
	P1 pxColor = A1::ColorToPixel( kColor );

   // (note) when trying to draw a 2 pixel line, the setup code
   //         changes the line width to 1.5, resulting in a 3 pixel line.
   //        setting the width to 1.0 at this point results in a 2 pixel line!
   //
   // (note) the solution for now is to subtract 0.5 which results in correct
   //         1,2,3,4,5 pixel wide h and vlines

   vStartWidth -= eC_FromFloat(0.5f);
   vEndWidth -= eC_FromFloat(0.5f);

	// calc deltas
	dx = kEnd.x - kStart.x;
	dy = kEnd.y - kStart.y;
	#ifdef eGML_VARIWIDTH
	dw = vEndWidth - vStartWidth;
	#endif

	kXclip.fxMin = eC_FixFromInt( pBitmap->GetClipMinX() );
	kXclip.fxMax = eC_FixFromInt( pBitmap->GetClipMaxX() );
	kYclip.fxMin = eC_FixFromInt( pBitmap->GetClipMinY() );
	kYclip.fxMax = eC_FixFromInt( pBitmap->GetClipMaxY() );


	// find major axis
	if (eC_Abs(dx) > eC_Abs(dy))
	{
		// major X, orient left->right
		if (dx < eC_FromInt(0))	
		{
			x = kEnd.x;
			y = kEnd.y;
			dx = -dx;
			dy = -dy;
			#ifdef eGML_VARIWIDTH
			dw = -dw;
			vStartWidth = vEndWidth;
			#endif	
			#ifndef eGML_LINEPATTERN_DISABLE
            if (ulFlags & eGML_LF_PATTERN)
            {
                eGML_Line_patternStyle *pkStyle = (eGML_Line_patternStyle*) pvStyle;
                pkStyle->bFlip = true;
            }
			#endif
		} else {
			x = kStart.x;
			y = kStart.y;				
		}

		#ifndef eGML_LINEPATTERN_DISABLE
        if (ulFlags & eGML_LF_PATTERN)
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
				kYclip.fxMax += eC_FixFromInt(1);
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender_HQAA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctantClip( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctantClip( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    WideLineOctantClip( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
		    }
        }
        else
		#endif
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
				kYclip.fxMax += eC_FixFromInt(1);
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<P1, eGML_VSpanRender_HQAA<P1,P2,P1,A1> >::
					    WideLineOctantClip( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_VSpanRender_AA<P1,P2,P1,A1> >::
					    WideLineOctantClip( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_VSpanRender<P1,P2,P1,A1> >::
				    WideLineOctantClip( pBitmap, x, y, dx, dy, pxColor, vStartWidth, dw, ulFlags, kXclip, kYclip ); 
		    }
        }
	}
	else 
	{
		// major Y, orient top->bottom
		if (dy < eC_FromInt(0))
		{
			x = kEnd.x;
			y = kEnd.y;
			dx = -dx;
			dy = -dy;
			#ifdef eGML_VARIWIDTH
			dw = -dw;
			vStartWidth = vEndWidth;
			#endif	
			#ifndef eGML_LINEPATTERN_DISABLE
            if (ulFlags & eGML_LF_PATTERN)
            {
                eGML_Line_patternStyle *pkStyle = (eGML_Line_patternStyle*) pvStyle;
                pkStyle->bFlip = true;
            }
			#endif
		} else {
			x = kStart.x;
			y = kStart.y;
		}			

		#ifndef eGML_LINEPATTERN_DISABLE
        if (ulFlags & eGML_LF_PATTERN)
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
				kXclip.fxMax += eC_FixFromInt(1);
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender_HQAA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctantClip( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kYclip, kXclip ); // kYclip and kXclip swaped because y-Axis is major
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    WideLineOctantClip( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kYclip, kXclip );  // kYclip and kXclip swaped because y-Axis is major
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    WideLineOctantClip( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, vStartWidth, dw, ulFlags, kYclip, kXclip );  // kYclip and kXclip swaped because y-Axis is major
		    }
        }
        else
		#endif
        {
		    if (ulFlags & (eGML_LF_ANTIALIAS | eGML_LF_HQ_AA))
		    {
				kXclip.fxMax += eC_FixFromInt(1);
			    #ifdef eGML_FILTEREDAA
			    if (ulFlags & eGML_LF_HQ_AA)
			    {
				    eGML_Linedraw_Octant<P1, eGML_HSpanRender_HQAA<P1,P2,P1,A1> >::
					    WideLineOctantClip( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags, kYclip, kXclip ); 
			    }
			    else 
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_HSpanRender_AA<P1,P2,P1,A1> >::
					    WideLineOctantClip( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags, kYclip, kXclip ); 
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_HSpanRender<P1,P2,P1,A1> >::
				    WideLineOctantClip( pBitmap, y, x, dy, dx, pxColor, vStartWidth, dw, ulFlags, kYclip, kXclip ); 
		    }
        }
	}

}

//---------------------------------------------------------------	
// wide line octant rendering with standart parameters
//
template <typename L1, class S1> 
inline void eGML_Linedraw_Octant<L1,S1>::WideLineOctant( eGML_Bitmap *pBitmap, 
	eC_Value vStartMaj, eC_Value vStartMin, eC_Value vDeltaMaj, eC_Value vDeltaMin, 
	const L1 &kStyle, eC_Value vStartWidth, eC_Value vDeltaWidth, eC_Flag ulFlags )
{	
	eC_Value vNr, vIncMin, vWidthFactor = 0;         
	eC_Value vNormalMaj, vNormalMin, vLen;
    octantData kPos;
    rasterLoop pRaster;

	// ensure legal startwidth
	if (vStartWidth < eC_FromInt(1))
	{
		vStartWidth	= eC_FromInt(1);
	}

	// get width variation 
	#ifdef eGML_VARIWIDTH
	if (vDeltaWidth != eC_FromInt(0))
		vWidthFactor = eC_Div(vDeltaWidth , vStartWidth );
	else
		vWidthFactor = 0;
	#endif

	// catch divbyzero
	vLen = eC_Len2(vDeltaMaj, vDeltaMin);
//      vLen = (eC_Abs( vDeltaMaj ) + eC_Abs( vDeltaMin )) / 2;
	if (vLen <= eC_FromFloat(0.0001f)) return;

    // choose rasterisation loop
    #ifndef eGML_LINEPATTERN_DISABLE
    if (ulFlags&eGML_LF_PATTERN)
    {
        pRaster = rasterPatternV;            
    }
    else
	#endif
    {
        if (vStartWidth>vLen)
        {
        // specialcase 'wider than long' lines    
        pRaster = rasterShort;
        } 
        else 
        {
        // default 3part code  
        #ifdef eGML_VARIWIDTH
        pRaster = rasterLongV;
        #else
        pRaster = rasterLong;
        #endif
        }
    }

	// normalize deltas
	vNr = eC_Reci( vLen );		
	vNormalMaj = eC_Mul( vDeltaMaj, vNr );
	vNormalMin = eC_Mul( vDeltaMin, vNr );

	// minor axis - increment (result is -1..+1)			
	// (major axis increment is always 1)		
	if (vDeltaMaj > eC_FromFloat(0.1f))
	{
		vIncMin = eC_Div(vDeltaMin, vDeltaMaj);
	} else {
		vIncMin = 0;
	}

    // convert to fixedpoint
    kPos.fxStartMaj = eC_FixFromValue(vStartMaj);
    kPos.fxStartMin = eC_FixFromValue(vStartMin);
    kPos.fxDeltaMaj = eC_FixFromValue(vDeltaMaj);
    kPos.fxDeltaMin = eC_FixFromValue(vDeltaMin);
    kPos.fxNormalMaj = eC_FixFromValue(vNormalMaj);
    kPos.fxNormalMin = eC_FixFromValue(vNormalMin);
    kPos.fxLen = eC_FixFromValue(vLen);

	// call inner code
	WideLineInner( pBitmap, kPos, eC_FixFromValue(vIncMin), kStyle, eC_FixFromValue(vStartWidth), 
                    eC_FixFromValue(vWidthFactor), ulFlags, pRaster );
}


//---------------------------------------------------------------
// wide line octant rendering with standart parameters for
// clipped lines
//
template < typename L1,	class S1 > 
void eGML_Linedraw_Octant<L1,S1>::WideLineOctantClip( eGML_Bitmap *pBitmap, eC_Value vStartMaj, eC_Value vStartMin, 
						                              eC_Value vDeltaMaj, eC_Value vDeltaMin, const L1 &kStyle, 
                                                      eC_Value vStartWidth, eC_Value vDeltaWidth, eC_Flag ulFlags,
                                                      const clipRange &kMajClip, const clipRange &kMinClip )
{
    eC_Value vNr, vIncMin, vWidthFactor = 0;         
	eC_Value vNormalMaj, vNormalMin, vLen;
    octantData kPos;
    rasterLoop pRaster;

	// ensure legal startwidth
	if (vStartWidth < eC_FromInt(1))
	{
		vStartWidth	= eC_FromInt(1);
	}
	
	// get width variation 
	#ifdef eGML_VARIWIDTH
	if (vDeltaWidth != eC_FromInt(0))
		vWidthFactor = eC_Div(vDeltaWidth , vStartWidth );
	else
		vWidthFactor = 0;
	#endif

	// catch divbyzero
	vLen = eC_Len2(vDeltaMaj, vDeltaMin);
	if (vLen <= eC_FromFloat(0.0001f)) return;

    // choose rasterisation loop        
	#ifndef eGML_LINEPATTERN_DISABLE
    if (ulFlags&eGML_LF_PATTERN)
    {
        pRaster = rasterPatternClip;            
    }
    else
	#endif
    {
        pRaster = rasterShortClip;
    }
    
	// normalize deltas
	vNr = eC_Reci( vLen );		
	vNormalMaj = eC_Mul( vDeltaMaj, vNr );
	vNormalMin = eC_Mul( vDeltaMin, vNr );

	// minor axis - increment (result is -1..+1)			
	// (major axis increment is always 1)		
	if (vDeltaMaj > eC_FromFloat(0.1f))
	{
		vIncMin = eC_Div(vDeltaMin, vDeltaMaj);
	} else {
		vIncMin = 0;
	}

    // convert to fixedpoint
	

    kPos.fxStartMaj = eC_FixFromValue(vStartMaj);
    kPos.fxStartMin = eC_FixFromValue(vStartMin);
    kPos.fxDeltaMaj = eC_FixFromValue(vDeltaMaj);
    kPos.fxDeltaMin = eC_FixFromValue(vDeltaMin);
    kPos.fxNormalMaj = eC_FixFromValue(vNormalMaj);
    kPos.fxNormalMin = eC_FixFromValue(vNormalMin);
    kPos.fxLen = eC_FixFromValue(vLen);

    // pass clipping information
    kPos.kMajClip = kMajClip;
    kPos.kMinClip = kMinClip;

	// call inner code
	WideLineInner( pBitmap, kPos, eC_FixFromValue(vIncMin), kStyle, eC_FixFromValue(vStartWidth), 
                   eC_FixFromValue(vWidthFactor), ulFlags, pRaster );
}


//---------------------------------------------------------------
// wide line octant rendering with preprocessed parameters
// preprocessed:
//   widthFactor = (endWidth - startWidth) / startWidth
//   incMin = deltaMin / deltaMaj
//   normalMaj = deltaMaj / sqrt( deltaMaj*deltaMaj + deltaMin*deltaMin )
//   normalMin = deltaMin / sqrt( deltaMaj*deltaMaj + deltaMin*deltaMin )
//
template <typename L1, class S1> 
void eGML_Linedraw_Octant<L1, S1>::WideLineInner( eGML_Bitmap *pBitmap, octantData &kPos, 
                                                  eC_Fixed fxIncMin, const L1 &kStyle, 
                                                  eC_Fixed fxLineWidth, eC_Fixed fxWidthFactor, 
                                                  eC_Flag ulFlags, rasterLoop pRaster)
{		
	eC_Fixed	fxOffsetMaj, fxAbsMin;
	eC_Fixed	fxOutMaj= 0, fxOutMin = 0;
	eC_Fixed	fxEdgeMaj, fxEdgeMin;
    rasterData  kData;

	eDBG_STAT_ADD(LineLength, eC_FixToFloat(kPos.fxDeltaMaj));
	
	// calc edge offset
	fxEdgeMaj = eC_FixMul( fxLineWidth/2, kPos.fxNormalMaj);
	fxEdgeMin = eC_FixMul( fxLineWidth/2, kPos.fxNormalMin );
	fxAbsMin  = eC_FixAbs( fxEdgeMin );

    //---debug
    #ifdef eGML_VARIWIDTH
//    if (fxWidthFactor != eC_FixFromInt(0)) ulFlags &= ~eGML_LF_WIDE_S;
    #endif
    //---debug

	// extend line endpoints by linewidth/2
	if (ulFlags & eGML_LF_WIDE_S)
	{

        if (ulFlags & eGML_LF_WIDE_2)
        {
            // quarter width extrusion
		    kPos.fxStartMaj -= fxEdgeMaj>>2;
		    kPos.fxStartMin -= fxEdgeMin>>2;
		    kPos.fxDeltaMaj += fxEdgeMaj>>1;
		    kPos.fxDeltaMin += fxEdgeMin>>1;
			kPos.fxLen += fxLineWidth>>2;
#ifdef eGML_VARIWIDTH
            if (fxWidthFactor != eC_FixFromInt(0))
            {
                kPos.fxDeltaMaj += eC_FixMulQ(fxEdgeMaj >> 2, fxWidthFactor);
                kPos.fxLen += eC_FixMulQ(fxLineWidth >> 3, fxWidthFactor);
            }
#endif
        } else {
            // half width extrusion
            kPos.fxStartMaj -= fxEdgeMaj>>1;
            kPos.fxStartMin -= fxEdgeMin>>1;
            kPos.fxDeltaMaj += fxEdgeMaj;
            kPos.fxDeltaMin += fxEdgeMin;
            kPos.fxLen += fxLineWidth>>1;  
#ifdef eGML_VARIWIDTH
            if (fxWidthFactor != eC_FixFromInt(0))
            {
                kPos.fxDeltaMaj += eC_FixMulQ(fxEdgeMaj >> 1, fxWidthFactor);
                kPos.fxLen += eC_FixMulQ(fxLineWidth >> 2, fxWidthFactor);
            }
#endif
        }
	} else {
		// include endpoint (adjust ep halfplane)
		kPos.fxDeltaMaj += kPos.fxNormalMaj;
		kPos.fxDeltaMin += kPos.fxNormalMin;
	}

	// line endpoint (exact)
	kData.fxEndMaj = kPos.fxStartMaj + kPos.fxDeltaMaj;

	// vSpan height
	kData.fxSpan = eC_FixMulQ(fxIncMin, fxEdgeMin) + fxEdgeMaj;		    // need 1/nx to avoid division calc: (ny/nx)*ny+nx

	#ifdef eGML_VARIWIDTH
	if (fxWidthFactor != eC_FixFromInt(0))
	{
		kData.fxSpanInc = eC_FixMul(fxWidthFactor, kData.fxSpan);
		kData.fxSpanInc = eC_FixDiv(kData.fxSpanInc, kPos.fxDeltaMaj);	//note use : deltamaj+2*absmin ? and reduce startwidth by f(absmin) ?
	} else {
		kData.fxSpanInc = 0;
	}
	#endif

	// get point of line with major-axis = first span		
	// rotate 90°? 
	fxOutMaj = kPos.fxStartMaj - fxAbsMin;	
	fxOutMin = kPos.fxStartMin - eC_FixMulQ(fxIncMin, fxAbsMin);

	kData.fxPosMaj = eC_FixCeil( fxOutMaj - eC_FixFromFloat(0.5f) );

	kData.fxPosMin = fxOutMin;

	fxOffsetMaj = kData.fxPosMaj - fxOutMaj;
	fxOutMaj = kPos.fxStartMaj + fxAbsMin;

	#ifdef eGML_VARIWIDTH
	// variwidth-line correction
	if (fxWidthFactor != eC_FixFromInt(0))
	{
		// get abs normal min for endwidth
		fxAbsMin += eC_FixMul( fxWidthFactor, fxAbsMin );
	}
	#endif

    // prepare datablock
    kData.fxIncMin = fxIncMin;

	// find midpart (using quicker loop without endpoint handling)	
	kData.fxMid1Maj = eC_FixFloor(fxOutMaj) + eC_FixFromInt(1);
	kData.fxMid2Maj = eC_FixFloor(kData.fxEndMaj - fxAbsMin);            
    // get point of line with major-axis = last span
	kData.fxEndMaj += fxAbsMin ;	

	// subpixel correction
	kData.fxPosMin += eC_FixMul( fxIncMin, fxOffsetMaj );	

    // hqAA offset correction and round(y)
    if (!(ulFlags & eGML_LF_HQ_AA)) kData.fxPosMin += eC_FixFromFloat(0.5f);

	// currently, we uses simple AA-Span-methodes for edgelines -> 2px width missing!! +0.5f makes is visually even
#ifdef eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA
	if ((ulFlags & eGML_LF_AA_CLIP_END_PART_CORRECTION) && (ulFlags & eGML_LF_HQ_AA)) {
		kData.fxPosMin += eC_FixFromFloat(0.5f);		
	}
#endif
	
    // call rasterisation code
    pRaster( pBitmap, kStyle, kData, kPos, ulFlags);
}

//---------------------------------------------------------------	
#endif
