/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_VSpanRender.h
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


#ifndef EGML_VSPANRENDER__H_
#define EGML_VSPANRENDER__H_

//---------------------------------------------------------------
// Vertical Span Renderer - No AntiAliasing
//
template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
typename P3,      // Style type
class A1			// Accessmethods for Destination
> 

class eGML_VSpanRender
{
public:

	static inline void RenderSpanAlpha(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, P3 kStyle, eC_UByte ubAlpha )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// full pixels
		for (eC_Int y=y1i; y<=y2i; y++)
		{
			A1::BlendPixelGamma( kOffset, pxColor, ubAlpha );
			A1::IncY( pBitmap, kOffset );
		}
	}

	// parameters are: minor axis pos, major axis start, major axis stop, color
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, P3 kStyle )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// full pixels
		for (eC_Int y=y1i; y<=y2i; y++)
		{
			A1::PutPixel( kOffset, pxColor );			
			A1::IncY( pBitmap, kOffset );
		}
	}

	// render span with additional halfplane constraint
	static inline void RenderSpanEdge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, P3 kStyle )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( y1 - eC_FixFromInt(y1i), fxDistInc );

		// full pixels
		for (eC_Int y=y1i; y<=y2i; y++)
		{
			if (fxDist>0) A1::PutPixel( kOffset, pxColor );			
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}
	}

	// render span with two additional halfplane constraints
	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		// reject spans that cover less than one pixel 
		//if ((y2i-y1i) < 1) return;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i))+fxClip, fxDistInc );

		// full pixels
		for (eC_Int y=y1i; y<=y2i; y++)
		{
			if ((fxDist>0)&&(fxDist<=fxDist2)) 
				A1::PutPixel( kOffset, pxColor );
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}
	}

	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major ){
		/*  should never be called in this context, since the calling methode does the clipping already  */
	}

#ifndef eGML_LINEPATTERN_DISABLE
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		const eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxClip += y1 - eC_FixFromInt(y1i);
		fxDist -= eC_FixMul( fxClip, fxDistInc );
		// texture subpixel correction
		//		eC_Fixed fxT = pkStyle->fxTex - eC_FixMul( fxClip, pkStyle->fxTexInc );


		//		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i)) + fxClip, fxDistInc );
		// texture subpixel correction
		eC_Fixed fxT = pkStyle->fxTex + eC_FixMul( fxDist , pkStyle->fxTexMul );


		// full pixels
		for (eC_Int y=y1i; y<=y2i; y++)
		{
			if ((fxDist>0)&&(fxDist<=fxDist2))
			{
				P1 px = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
				A1::PutPixel( kOffset, px );
			} 
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
			// texture update
			fxT += pkStyle->fxTexInc;
		}
	}

	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle,eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* will never be called, but nees to be defined, since the interface supports this methode for dynamic binding */
	}

#endif

	// used when (y2-y1)<=1


	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{	

		/* protect buffer */
		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);
		/* protect buffer */
		BREAK_ON_OVERFLOW(y,max_Minor,0);		
		BREAK_ON_UNDERFLOW(y,min_Minor);

		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));	

		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
		A1::PutPixel( kOffset, pxColor );

	}

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle)
	{	
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));	

		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
		A1::PutPixel( kOffset, pxColor );

	}

	// vector access
	static inline eC_Value GetMajFromVector( const eGML_Vector2 &rkVector )  { return rkVector.x; }
	static inline eC_Value GetMinFromVector( const eGML_Vector2 &rkVector )  { return rkVector.y; }
};

//---------------------------------------------------------------	
#endif

