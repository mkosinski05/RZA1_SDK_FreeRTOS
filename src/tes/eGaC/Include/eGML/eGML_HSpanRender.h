/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_HSpanRender.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:19:56 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
******************************************************************************/

#ifndef EGML_HSPANRENDER__H_
#define EGML_HSPANRENDER__H_


//---------------------------------------------------------------
// Horizontal Span Renderer - No AntiAliasing
//
template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
typename P3,      // Style type
class A1			// Accessmethods for Destination
> 

class eGML_HSpanRender
{
public:

	// supported for horizontal spans only 
	static inline void RenderSpanAlpha(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, P3 kStyle, eC_UByte ubAlpha )
	{		
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );
		// full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
			A1::BlendPixelGamma( kOffset, pxColor, ubAlpha );
			A1::IncX( pBitmap, kOffset );
		}
	}

	// parameters are: minor axis pos, major axis start, major axis stop, color
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, P3 kStyle )
	{		
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// full pixels
		A1::PutSpan( kOffset, pxColor, (x2i-x1i)+1 );
	}

	// render span with additional halfplane constraint
	static inline void RenderSpanEdge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, P3 kStyle )
	{
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( x1 - eC_FixFromInt(x1i), fxDistInc );

		// full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
			if (fxDist>0) A1::PutPixel( kOffset, pxColor );
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}
	}

	// render span with two additional halfplane constraints
#ifndef eGML_LINEPATTERN_DISABLE
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kOffset;
		const eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );

		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// distance term subpixel correction
		fxClip += x1 - eC_FixFromInt(x1i);
		fxDist -= eC_FixMul( fxClip, fxDistInc );
		// texture subpixel correction
		//eC_Fixed fxT = pkStyle->fxTex - eC_FixMul( fxClip, pkStyle->fxTexInc );
		//fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i)) + fxClip, fxDistInc );
		// texture subpixel correction
		eC_Fixed fxT = pkStyle->fxTex + eC_FixMul( fxDist , pkStyle->fxTexMul );

		// full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
			if ((fxDist>0)&&(fxDist<=fxDist2))
			{
				P1 px = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
				A1::PutPixel( kOffset, px );
			} 
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
			// texture update
			fxT += pkStyle->fxTexInc;
		}
	}

	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle,eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* will never be called, since we clip in rasterline already */
	}

#endif

	// render span with two additional halfplane constraints
	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (x1 - eC_FixFromInt(x1i)) + fxClip, fxDistInc );

		// full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
			if ((fxDist>0)&&(fxDist<=fxDist2))
				A1::PutPixel( kOffset, pxColor );
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}
	}


	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major ){
		/*  should never be called in this context, since the calling methode does the clipping already  */
	}

	// used when (y2-y1)<=1
	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* protect buffer */
		BREAK_ON_OVERFLOW(y,max_Major,0);
		BREAK_ON_UNDERFLOW(y,min_Major);
		/* protect buffer */

		BREAK_ON_OVERFLOW(x,max_Minor,0);
		BREAK_ON_UNDERFLOW(x,min_Minor);

		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));
		
		if (eC_FixToInt(x) >= eC_FixToInt(min_Minor)) {
			A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
			A1::PutPixel( kOffset, pxColor );
		}
	}	

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x, P3 kStyle)
	{
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
		A1::PutPixel( kOffset, pxColor );
	}	

	// vector access
	static inline eC_Value GetMajFromVector( const eGML_Vector2 &rkVector )  { return rkVector.y; }
	static inline eC_Value GetMinFromVector( const eGML_Vector2 &rkVector )  { return rkVector.x; }
};

#endif
