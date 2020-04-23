/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_HSpanRender_HQ_AA.h
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


#ifndef EGML_HSPANRENDER_HQ_AA__H_
#define EGML_HSPANRENDER_HQ_AA__H_
//---------------------------------------------------------------	
//---------------------------------------------------------------
// Horizontal Span Renderer - Filtered AntiAliasing
//

template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
typename P3,      // Style type
class A1			// Accessmethods for Destination
> 

class eGML_HSpanRender_HQAA 
{
public:

	// parameters are: minor axis pos, major axis start, major axis stop, color
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, P3 kStyle )
	{				
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		eC_Int iLen;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = x2i - x1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(x1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixeles
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		A1::IncX( pBitmap, kOffset );
		x1i++;
		if (x1i>x2i) return;
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncX( pBitmap, kOffset );
		x1i++;
		if (x1i>x2i) return;

		// full pixels
		A1::PutSpan( kOffset, pxColor, x2i-x1i );

		// samplepoint distance -> index into filter
		ubOffset = (eC_FixFracToByte(x2) ^ 255) >> (8 - eGML_FILTERSHIFT);
		// lower AA pixel
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncX( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
	}

	// render span with additional halfplane constraint
	static inline void RenderSpanEdge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, P3 kStyle )
	{
		eC_UInt uiAlpha, uiAlpha2;
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		eC_Int iLen;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = x2i - x1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( x1 - eC_FixFromInt(x1i), fxDistInc );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(x1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixeles
		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;			
			uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncX( pBitmap, kOffset );
		x1i++;
		// edge distance update
		fxDist += fxDistInc;
		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
			uiAlpha = g_eGML_ubFilter[ ubOffset ];
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncX( pBitmap, kOffset );
		x1i++;
		// edge distance update
		fxDist += fxDistInc;	

		// full pixels
		for (eC_Int x=x1i; x<x2i; x++)
		{
			if (fxDist>0)
			{
				if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha2 );
			}
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}

		// lower AA pixel
		if (x1i<=x2i)
		{
			// samplepoint distance -> index into filter
			ubOffset = (eC_FixFracToByte(x2) ^ 255) >> (8 - eGML_FILTERSHIFT);

			if (fxDist>0)
			{	
				if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
				uiAlpha = g_eGML_ubFilter[ ubOffset ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;	
			if (fxDist>0)
			{
				if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
				uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}
	}

	// render span with two additional halfplane constraints
	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		/* DEBUG */
		eGML_HSpanRender_AA<P1,P2,P3,A1>::RenderSpan2Edge(pBitmap, y, x1, x2, fxDist, fxDistInc, fxDist2, fxClip, kStyle );
	}



	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major ){

#ifdef eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA
		eGML_HSpanRender_AA<P1,P2,P3,A1>::RenderSpan2Edge(pBitmap, y, x1, x2, fxDist, fxDistInc, fxDist2, fxClip, kStyle, max_Minor, min_Minor, max_Major, min_Major);
#else


		BREAK_ON_OVERFLOW(y,max_Major,0);
		BREAK_ON_UNDERFLOW(y,min_Major);


		eC_UInt uiAlpha, uiAlpha2;
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		eC_Fixed x1i_tmp = x1;
		eC_Int iLen;
		eC_Fixed fxD;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = x2i - x1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( x1 - eC_FixFromInt(x1i), fxDistInc );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(x1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixeles
		fxD = fxDist2-fxDist;
		if (((x1i_tmp >= min_Minor) && (x1i_tmp  < max_Minor))) {
			if ((fxDist>0)&&(fxD>=0))
			{
				if (fxDist<eC_FixFromInt(1)) {
					uiAlpha2 = eC_FixFracToByte( fxDist ); 
				} else {
					if (fxD<eC_FixFromInt(1)) {
						uiAlpha2 = eC_FixFracToByte( fxD ); 
					} else {
						uiAlpha2 = 0xff;
					}
				}
				uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}
		x1i_tmp += eC_FixFromInt(1);
		A1::IncX( pBitmap, kOffset );
		x1i++;
		// edge distance update
		fxDist += fxDistInc;
		fxD = fxDist2-fxDist;
		if (((x1i_tmp >= min_Minor) && (x1i_tmp  < max_Minor))) {

			if ((fxDist>0)&&(fxD>=0))
			{
				if (fxDist<eC_FixFromInt(1)) {
					uiAlpha2 = eC_FixFracToByte( fxDist ); 
				} else {
					if (fxD<eC_FixFromInt(1)) {
						uiAlpha2 = eC_FixFracToByte( fxD ); 
					} else {
						uiAlpha2 = 0xff;
					}
				}
				uiAlpha = g_eGML_ubFilter[ ubOffset ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}


		x1i_tmp += eC_FixFromInt(1);
		A1::IncX( pBitmap, kOffset );
		x1i++;
		// edge distance update
		fxDist += fxDistInc;	

		// full pixels
		for (eC_Int x=x1i; x<x2i; x++)
		{
			
			if (((x1i_tmp >= min_Minor) && (x1i_tmp  < max_Minor))) {
				fxD = fxDist2-fxDist;
				if ((fxDist>0)&&(fxD>=0))
				{
					if (fxDist<eC_FixFromInt(1)) {
						uiAlpha2 = eC_FixFracToByte( fxDist ); 
					} else {
						if (fxD<eC_FixFromInt(1)) {
							uiAlpha2 = eC_FixFracToByte( fxD ); 
						} else {
							uiAlpha2 = 0xff;
						}
					}
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha2 );
				}
			}
			x1i_tmp += eC_FixFromInt(1);
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}

		// lower AA pixel
		if (x1i<=x2i)
		{
			// samplepoint distance -> index into filter

			ubOffset = (eC_FixFracToByte(x2) ^ 255) >> (8 - eGML_FILTERSHIFT);

			if (((x1i_tmp >= min_Minor) && (x1i_tmp  < max_Minor))) {
				fxD = fxDist2-fxDist;
				if ((fxDist>0)&&(fxD>=0))
				{
					if (fxDist<eC_FixFromInt(1)) {
						uiAlpha2 = eC_FixFracToByte( fxDist ); 
					} else {
						if (fxD<eC_FixFromInt(1)) {
							uiAlpha2 = eC_FixFracToByte( fxD ); 
						} else {
							uiAlpha2 = 0xff;
						}
					}
					uiAlpha = g_eGML_ubFilter[ ubOffset ];
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
				}
			}
			x1i_tmp += eC_FixFromInt(1);
			A1::IncX( pBitmap, kOffset );
			// edge distance update
			if (((x1i_tmp >= min_Minor) && (x1i_tmp  < max_Minor))) {
				fxDist += fxDistInc;	
				if (fxDist>0)
				{
					if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
					uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
				}
			}
		}
	#endif
	}



	// render span with two additional halfplane constraints
#ifndef eGML_LINEPATTERN_DISABLE
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		/* DEBUG */
		eGML_HSpanRender_AA<P1,P2,P3,A1>::RenderSpanPattern2Edge(pBitmap, y, x1, x2, fxDist, fxDistInc, fxDist2, fxClip, kStyle);
	}

	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* DEBUG */
		eGML_HSpanRender_AA<P1,P2,P3,A1>::RenderSpanPattern2Edge(pBitmap, y, x1, x2, fxDist, fxDistInc, fxDist2, fxClip, kStyle, max_Minor, min_Minor, max_Major, min_Major);
	}


#endif

	// used when (y2-y1)<=2

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* protect buffer */
		BREAK_ON_OVERFLOW(y,max_Major,0);
		BREAK_ON_UNDERFLOW(y,min_Major);
		/* protect buffer */

		P2 kOffset;

		BREAK_ON_OVERFLOW(x,max_Minor,0);
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));
		A1::GetOffset( pBitmap, eC_FixToInt(x)/*-1*/, eC_FixToInt(y), kOffset );
		if (eC_FixToInt(x) >= eC_FixToInt(min_Minor)) {
			// samplepoint distance -> index into filter
			eC_UInt ubOffset = eC_FixFracToByte(x) >> (8 - eGML_FILTERSHIFT);
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		}
		A1::IncX( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(x,max_Minor,1);
		if (eC_FixToInt(x)+1 >= eC_FixToInt(min_Minor)) {
			eC_UInt ubOffset = eC_FixFracToByte(x) >> (8 - eGML_FILTERSHIFT);
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		}
		A1::IncX( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(x,max_Minor,2);
		if (eC_FixToInt(x)+2 >= eC_FixToInt(min_Minor)) {

			// invert
			eC_UInt ubOffset = eC_FixFracToByte(x) >> (8 - eGML_FILTERSHIFT);
			ubOffset = eGML_FILTERTABLE/2 - ubOffset;
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset  ] );
		}
		A1::IncX( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(x,max_Minor,3);
		if (eC_FixToInt(x)+3 >= eC_FixToInt(min_Minor)) {
			eC_UInt ubOffset = eC_FixFracToByte(x) >> (8 - eGML_FILTERSHIFT);
			ubOffset = eGML_FILTERTABLE/2 - ubOffset;
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2)  ] );
		}
	}

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x, P3 kStyle)
	{
		P2 kOffset;
		A1::GetOffset( pBitmap, eC_FixToInt(x)/*-1*/, eC_FixToInt(y), kOffset );
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(x) >> (8 - eGML_FILTERSHIFT);
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		A1::IncX( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncX( pBitmap, kOffset );
		// invert		
		ubOffset = eGML_FILTERTABLE/2 - ubOffset;
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset  ] );
		A1::IncX( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2)  ] );
	}

	// vector access
	static inline eC_Value GetMajFromVector( const eGML_Vector2 &rkVector )  { return rkVector.y; }
	static inline eC_Value GetMinFromVector( const eGML_Vector2 &rkVector )  { return rkVector.x; }
};

#endif
