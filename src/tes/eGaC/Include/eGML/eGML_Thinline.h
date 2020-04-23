/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Thinline.h
* %version:        134.0 %
*
* author:          hh04031
* creation date:   Thu Aug 11 10:12:49 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_THINLINE__H_
#define EGML_THINLINE__H_

//---------------------------------------------------------------
// entrypoint for clipped thin lines. does the octant splitting
// and antialias codepath selection (3x)
//
template <typename P1, typename P2, class A1, eC_Bool bMustBlendAA>
void eGML_Linedraw<P1,P2,A1,bMustBlendAA>::ThinLineClipped( eGML_Bitmap *pBitmap, const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd,
                                                           eGML_Color kColor, eC_Flag ulFlags, void *pvStyle, eC_Value vXmax, eC_Value vYmax, eC_Value vXmin, eC_Value vYmin, eC_Flag uiCode)
{
	eC_Value x, y, dx, dy;

	eDBG_PERF_BEGIN( ThinLines );

	// cast color
	P1 pxColor = A1::ColorToPixel( kColor );

	// calc deltas
	dx = kEnd.x - kStart.x;
	dy = kEnd.y - kStart.y;



	// check for 90° line

	if ((((eC_UInt)dx) & ((eC_UInt)dy)) == 0)
	{
		if (dx == 0)
		{
			//ThinLineV( pBitmap, kStart, dy, kColor, ulFlags );
			vXmax += eC_FromFloat(0.5f);
		} else {
			//ThinLineH( pBitmap, kStart, dy, kColor, ulFlags );
			vYmax += eC_FromFloat(0.5f);
		}
	}


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
					    ThinLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, ulFlags, vXmax, vYmax, vXmin, vYmin, uiCode);
			    }
			    else
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    ThinLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, ulFlags, vXmax, vYmax, vXmin, vYmin,  uiCode );
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_VSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    ThinLineOctant( pBitmap, x, y, dx, dy, (eGML_Line_patternStyle*)pvStyle, ulFlags, vXmax, vYmax, vXmin, vYmin, uiCode );
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
					    ThinLineOctant( pBitmap, x, y, dx, dy, pxColor, ulFlags, vXmax, vYmax, vXmin, vYmin, uiCode );
			    }
			    else
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_VSpanRender_AA<P1,P2,P1,A1> >::
					    ThinLineOctant( pBitmap, x, y, dx, dy, pxColor, ulFlags, vXmax, vYmax, vXmin, vYmin, uiCode );
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_VSpanRender<P1,P2,P1,A1> >::
				    ThinLineOctant( pBitmap, x, y, dx, dy, pxColor, ulFlags, vXmax, vYmax, vXmin, vYmin, uiCode );
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
					    ThinLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
			    }
			    else
			    #endif
			    {
				    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender_AA<P1,P2,eGML_Line_patternStyle*,A1> >::
					    ThinLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
			    }
		    } else {
			    eGML_Linedraw_Octant<eGML_Line_patternStyle*, eGML_HSpanRender<P1,P2,eGML_Line_patternStyle*,A1> >::
				    ThinLineOctant( pBitmap, y, x, dy, dx, (eGML_Line_patternStyle*)pvStyle, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
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
					    ThinLineOctant( pBitmap, y, x, dy, dx, pxColor, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
			    }
			    else
			    #endif
			    {
				    eGML_Linedraw_Octant<P1, eGML_HSpanRender_AA<P1,P2,P1,A1> >::
					    ThinLineOctant( pBitmap, y, x, dy, dx, pxColor, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
			    }
		    } else {
			    eGML_Linedraw_Octant<P1, eGML_HSpanRender<P1,P2,P1,A1> >::
				    ThinLineOctant( pBitmap, y, x, dy, dx, pxColor, ulFlags, vYmax, vXmax, vYmin, vXmin, uiCode );
		    }
        }
	}

	eDBG_PERF_END( ThinLines );
}

//---------------------------------------------------------------
// thin line octant rendering with standart parameters
//
template <typename L1, class S1>
inline void eGML_Linedraw_Octant<L1,S1>::ThinLineOctant( eGML_Bitmap *pBitmap,
	eC_Value vStartMaj, eC_Value vStartMin,
	eC_Value vDeltaMaj, eC_Value vDeltaMin,
	const L1 &kStyle, eC_Flag ulFlags,
	eC_Value vMajmax, eC_Value vMinmax, eC_Value vMajMin, eC_Value vMinMin, eC_Flag uiCode)
{
	eC_Value vIncMin;

	// minor axis - increment (result is -1..+1)
	// (major axis increment is always 1)
	if (vDeltaMaj > eC_FromFloat(0.001f))
	{
		vIncMin = eC_Div( vDeltaMin, vDeltaMaj );
	} else {
		vIncMin = 0;
	}



	// call inner code
	ThinLineInner( pBitmap,
					eC_FixFromValue(vStartMaj), eC_FixFromValue(vStartMin),
					eC_FixFromValue(vIncMin), eC_FixFromValue(vDeltaMaj), eC_FixFromValue(vDeltaMin),
					kStyle, ulFlags, eC_FixFromValue(vMajmax), eC_FixFromValue(vMinmax), eC_FixFromValue(vMajMin), eC_FixFromValue(vMinMin),uiCode );
}




//---------------------------------------------------------------
// thin line octant rendering with preprocessed parameters
// preprocessed:
//   incMin = deltaMin / deltaMaj
//
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1, S1>::ThinLineInner( eGML_Bitmap *pBitmap, eC_Fixed fxStartMaj, eC_Fixed fxStartMin,
                                                  eC_Fixed fxIncMin, eC_Fixed fxDeltaMaj, eC_Fixed fxDeltaMin,
                                                  const L1 &kStyle, eC_Flag ulFlags, eC_Fixed fxMaxMaj, eC_Fixed fxMaxMin, eC_Fixed fxMinMaj, eC_Fixed fxMinMin, eC_Flag uiCode )
{
	eC_Fixed fxPosMaj, fxPosMin, fxOffsetMaj, fxOffsetMin, fxEndMaj;

	eDBG_STAT_ADD(LineLength, eC_FixToFloat(fxDeltaMaj));

	fxPosMaj = eC_FixCeil( fxStartMaj - eC_FixFromFloat(0.5f)) ;  // round to subpixelmovment to nearest pixeledge


	// recalculate the starposition in x,y. (moves the startpos backwards along the directionvector)
	fxOffsetMaj = fxPosMaj - fxStartMaj;
	fxOffsetMin = eC_FixMulQ( fxIncMin, fxOffsetMaj);

	// +0.5 to get round(y) instead of floor(y) but compensated by -0.5 shift for AA lines
	if (!(ulFlags & eGML_LF_ANTIALIAS)) {
		fxOffsetMin += eC_FixFromFloat(0.5f);
	}

	fxPosMin = fxStartMin + fxOffsetMin;

	// the HQ_AA uses 4 pixel-spans, which needs to be centered along the line
	if (ulFlags & eGML_LF_HQ_AA) {
		fxPosMin -= eC_FixFromFloat(1.45f); // optical center, which seems to look best at 1.45 (instead of 2 or 3, from 4 possible discreet positions in a 4-Pixel-Span)
		if (fxPosMin < fxMinMin) // check, if the correction runs out of the BMP-boundarys
			uiCode |= eGML_Clipper::CB_OUTOFBOUND ;
	}

	// line endpoint (rounded)
	fxEndMaj = fxStartMaj + fxDeltaMaj;

	//fxMaxMaj += eC_FixFromFloat(0.5f); // fxMaxMay is too small for (<) comparison: BMP-Width = 256, MaxMay = 255, fxPosMay will never reach 255 at the loop-condition (fxPosMay < 255) -- fixed by <= replacement

	if (uiCode & (eGML_Clipper::CB_OUTOFBOUND | eGML_Clipper::CF_CLIPPED | eGML_Clipper::CF_OVERFLOW))
	{
		// make sure, we really don't miss the last pixel!
		fxEndMaj += eC_FixFromFloat(0.5f);

		if (fxEndMaj > fxMaxMaj) fxEndMaj = fxMaxMaj;
		//if ((fxEndMaj - fxPosMaj) < eC_FixFromInt(1)) return; // no pixels
		while ( fxPosMaj <= fxEndMaj )
		{
			S1::RenderPixel( pBitmap, fxPosMaj, fxPosMin, kStyle,	fxMaxMin, fxMinMin, fxMaxMaj, fxMinMaj );
			fxPosMaj += eC_FixFromInt(1);
			fxPosMin += fxIncMin;
		}
	} else {
		if (fxEndMaj > fxMaxMaj) fxEndMaj = fxMaxMaj;
		//if (fxPosMaj < eC_FixFromInt(0)) fxPosMaj= eC_FixFromInt(0);
		//if (fxPosMin < eC_FixFromInt(0)) fxPosMin = eC_FixFromInt(0);
		//if ((fxEndMaj - fxPosMaj) < eC_FixFromInt(1)) return; // no pixels

		while ( fxPosMaj <= fxEndMaj )
		{
			S1::RenderPixel( pBitmap, fxPosMaj, fxPosMin, kStyle);
			fxPosMaj += eC_FixFromInt(1);
			fxPosMin += fxIncMin;
		}
	}
}


//---------------------------------------------------------------
#endif


