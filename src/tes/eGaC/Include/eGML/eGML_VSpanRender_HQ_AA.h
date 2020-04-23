/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_VSpanRender_HQ_AA.h
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


#ifndef EGML_VSPANRENDER_HQ_AA__H_
#define EGML_VSPANRENDER_HQ_AA__H_

//---------------------------------------------------------------	

//---------------------------------------------------------------
// Vertical Span Renderer - Filtered AntiAliasing
//

template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
typename P3,      // Style type
class A1			// Accessmethods for Destination
> 

class eGML_VSpanRender_HQAA 
{
public:

	// parameters are: minor axis pos, major axis start, major axis stop, color
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, P3 kStyle )
	{
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Int iLen;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = y2i - y1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(y1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixeles
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		A1::IncY( pBitmap, kOffset );
		y1i++;
		if (y1i>y2i) return;
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncY( pBitmap, kOffset );
		y1i++;
		if (y1i>y2i) return;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{
			A1::PutPixel( kOffset, pxColor );
			A1::IncY( pBitmap, kOffset );
		}

		// samplepoint distance -> index into filter
		ubOffset = (eC_FixFracToByte(y2) ^ 255) >> (8 - eGML_FILTERSHIFT);
		// lower AA pixel
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncY( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
	}

	// render span with additional halfplane constraint
	// HQ_AA_V
	static inline void RenderSpanEdge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, P3 kStyle )
	{
		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Int iLen;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = y2i - y1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( y1 - eC_FixFromInt(y1i), fxDistInc );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(y1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixels
		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;			
			uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		// edge distance update
		fxDist += fxDistInc;
		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
			uiAlpha = g_eGML_ubFilter[ ubOffset ];
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		// edge distance update
		fxDist += fxDistInc;		

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{
			if (fxDist>0)
			{
				if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha2 );
			}			
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}

		// lower AA pixel
		if (y1i<=y2i)
		{
			// samplepoint distance -> index into filter
			ubOffset = (eC_FixFracToByte(y2) ^ 255) >> (8 - eGML_FILTERSHIFT);

			if (fxDist>0)
			{				
				if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
				uiAlpha = g_eGML_ubFilter[ ubOffset ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
			A1::IncY( pBitmap, kOffset );
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
	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		/*DEBUG*/
		eGML_VSpanRender_AA<P1,P2,P3,A1>::RenderSpan2Edge(pBitmap, x, y1, y2, fxDist, fxDistInc, fxDist2, fxClip, kStyle );
	}

	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major ){
#ifdef eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA
		eGML_VSpanRender_AA<P1,P2,P3,A1>::RenderSpan2Edge(pBitmap, x, y1, y2, fxDist, fxDistInc, fxDist2, fxClip, kStyle, max_Minor, min_Minor, max_Major,min_Major);
#else

		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);

		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Int iLen;
		eC_Fixed fxD;
		eC_Fixed y1i_tmp = y1;
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		iLen = y2i - y1i;
		if (iLen < 0) return;		
		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( y1 - eC_FixFromInt(y1i), fxDistInc );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(y1) >> (8 - eGML_FILTERSHIFT);
		// upper AA pixels
	
		if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
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
				uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		y1i_tmp += eC_FixFromInt(1);
		// edge distance update
		fxDist += fxDistInc;
		if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
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
		A1::IncY( pBitmap, kOffset );
		y1i++;
		y1i_tmp += eC_FixFromInt(1);
		// edge distance update
		fxDist += fxDistInc;		

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{
			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
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
			y1i_tmp += eC_FixFromInt(1);
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}

		// lower AA pixel
		if (y1i<=y2i)
		{
			// samplepoint distance -> index into filter
			ubOffset = (eC_FixFracToByte(y2) ^ 255) >> (8 - eGML_FILTERSHIFT);

			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
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
					}				uiAlpha = g_eGML_ubFilter[ ubOffset ];
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
				}
			}
			y1i_tmp += eC_FixFromInt(1);
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;	
			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
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
					uiAlpha = g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ];
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
				}
			}
		}
#endif
	}


	// render span with two additional halfplane constraints
#ifndef eGML_LINEPATTERN_DISABLE
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		/*DEBUG*/
		eGML_VSpanRender_AA<P1,P2,P3,A1>::RenderSpanPattern2Edge(pBitmap, x, y1, y2, fxDist, fxDistInc, fxDist2, fxClip, kStyle);
	}
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle,eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major){
			eGML_VSpanRender_AA<P1,P2,P3,A1>::RenderSpanPattern2Edge(pBitmap, x, y1, y2, fxDist, fxDistInc, fxDist2, fxClip, kStyle, max_Minor, min_Minor, max_Major, min_Major);
	}
#endif

	// used when (y2-y1)<=2
	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* protect buffer */
		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);
		/* protect buffer */

		P2 kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		BREAK_ON_OVERFLOW(y,max_Minor,0);
		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
		if (eC_FixToInt(y) >= eC_FixToInt(min_Minor)) {
			// samplepoint distance -> index into filter
			eC_UInt ubOffset = eC_FixFracToByte(y) >> (8 - eGML_FILTERSHIFT);
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		}

		A1::IncY( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(y,max_Minor,1);
		if (eC_FixToInt(y)+1 >= eC_FixToInt(min_Minor)) {			
			eC_UInt ubOffset = eC_FixFracToByte(y) >> (8 - eGML_FILTERSHIFT);
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		}

		A1::IncY( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(y,max_Minor,2);
		if (eC_FixToInt(y)+2 >= eC_FixToInt(min_Minor)) {
			// invert
			eC_UInt ubOffset = eC_FixFracToByte(y) >> (8 - eGML_FILTERSHIFT);
			ubOffset = eGML_FILTERTABLE/2 - ubOffset;
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset  ] );
		}

		A1::IncY( pBitmap, kOffset );
		BREAK_ON_OVERFLOW(y,max_Minor,3);
		if (eC_FixToInt(y)+3 >= eC_FixToInt(min_Minor)) {
			eC_UInt ubOffset = eC_FixFracToByte(y) >> (8 - eGML_FILTERSHIFT);
			ubOffset = eGML_FILTERTABLE/2 - ubOffset;
			A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2)  ] );
		}
	}

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle)
	{
		P2 kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));
		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );

		// samplepoint distance -> index into filter
		eC_UInt ubOffset = eC_FixFracToByte(y) >> (8 - eGML_FILTERSHIFT);
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2) ] );
		A1::IncY( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset ] );
		A1::IncY( pBitmap, kOffset );
		// invert
		ubOffset = eGML_FILTERTABLE/2 - ubOffset;
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset  ] );
		A1::IncY( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubFilter[ ubOffset + (eGML_FILTERTABLE/2)  ] );
	}

	// vector access
	static inline eC_Value GetMajFromVector( const eGML_Vector2 &rkVector )  { return rkVector.x; }
	static inline eC_Value GetMinFromVector( const eGML_Vector2 &rkVector )  { return rkVector.y; }
};

#endif
