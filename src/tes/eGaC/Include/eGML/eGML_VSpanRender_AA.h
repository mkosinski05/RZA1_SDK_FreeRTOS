/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_VSpanRender_AA.h
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


#ifndef EGML_VSPANRENDER_AA__H_
#define EGML_VSPANRENDER_AA__H_
//---------------------------------------------------------------	


//---------------------------------------------------------------
// Vertical Span Renderer - Simple AntiAliasing
//
template <
typename P1,		// Pixeltype for Destination
typename P2,		// Offsettype for Destination
typename P3,      // Style type
class A1			// Accessmethods for Destination
> 

class eGML_VSpanRender_AA 
{
public:

	// parameters are: minor axis pos, major axis start, major axis stop, color
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, P3 kStyle )
	{
		eC_UInt uiAlpha;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// first pixel 
		uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
		A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha );
		A1::IncY( pBitmap, kOffset );
		y1i++;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{
			A1::PutPixel( kOffset, pxColor );
			A1::IncY( pBitmap, kOffset );
		}

		// last pixel
		uiAlpha =  eC_FixFracToByte(y2);
		A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha );
	}

	// spans at the endpointsof widelines require special handling
	static inline void RenderSpanEdge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2, 
		eC_Fixed fxDist, eC_Fixed fxDistInc, P3 kStyle )
	{
		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		P2   kOffset;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( y1 - eC_FixFromInt(y1i), fxDistInc );

		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
			// first pixel 
			uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
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

		// last pixel
		uiAlpha =  eC_FixFracToByte(y2);

		if (fxDist>0)
		{
			if (fxDist<eC_FixFromInt(1)) uiAlpha2 = eC_FixFracToByte( fxDist ); else uiAlpha2 = 0xff;
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
	}

	// render span with two additional halfplane constraints
	// IS CALLES BY HQ_AA_RenderSpan2Edge
	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Fixed fxD;
		P2   kOffset;        
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i))+fxClip, fxDistInc );

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
			// first pixel 
			uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		// edge distance update
		fxDist += fxDistInc;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{	
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
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
		}

		// last pixel
		uiAlpha =  eC_FixFracToByte(y2);

		fxD = fxDist2-fxDist;
		//BMZ (overwrite fix)
		if ((fxDist>0)&&(fxD>=0) && uiAlpha!=0)
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
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
	}

	static inline void RenderSpan2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		// do nothing, if we are out of bound
		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);

		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);

		eC_Fixed y1i_tmp = y1;

		eC_Fixed fxD;
		P2   kOffset;        
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;
		
		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i))+fxClip, fxDistInc );

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
			// first pixel 
			uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor)))
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		y1i_tmp += eC_FixFromInt(1);
		// edge distance update
		fxDist += fxDistInc;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{	
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
				if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor)))
					A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha2 );
			}

			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
			y1i_tmp += eC_FixFromInt(1);
		}

		// last pixel
		uiAlpha =  eC_FixFracToByte(y2);

		fxD = fxDist2-fxDist;
		//BMZ (overwrite fix)
		if ((fxDist>0)&&(fxD>=0) && uiAlpha!=0)
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
			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor)))
				A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
	}


	// render span with two additional halfplane constraints
#ifndef eGML_LINEPATTERN_DISABLE
	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle )
	{
		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Fixed fxD;
		P2   kOffset;        
		P1   pxC;
		const eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i)) + fxClip, fxDistInc );
		// texture subpixel correction
		eC_Fixed fxT = pkStyle->fxTex + eC_FixMul( fxDist , pkStyle->fxTexMul );

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
			// first pixel 
			uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
			pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
			A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
		A1::IncY( pBitmap, kOffset );
		y1i++;
		// edge distance update
		fxDist += fxDistInc;
		// texture update
		fxT += pkStyle->fxTexInc;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{	
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
				pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
				A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) uiAlpha2 );
			}
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
			// texture update
			fxT += pkStyle->fxTexInc;
		}

		// last pixel
		uiAlpha =  eC_FixFracToByte(y2);

		fxD = fxDist2-fxDist;
		//BMZ (overwrite fix)
		if ((fxDist>0)&&(fxD>=0) && uiAlpha!=0)
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
			pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
			A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
		}
	}


	static inline void RenderSpanPattern2Edge(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y1, eC_Fixed y2,
		eC_Fixed fxDist, eC_Fixed fxDistInc, eC_Fixed fxDist2, eC_Fixed fxClip, P3 kStyle,eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);

		eC_UInt uiAlpha, uiAlpha2;
		eC_Int y1i = eC_FixToInt(y1);
		eC_Int y2i = eC_FixToInt(y2);
		eC_Fixed y1i_tmp = y1;
		eC_Fixed fxD;
		P2   kOffset;        
		P1   pxC;
		const eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );

		// reject spans that cover less than one pixel 
		if ((y2i-y1i) < 1) return;

		A1::GetOffset( pBitmap, eC_FixToInt(x), y1i, kOffset );

		// distance term subpixel correction
		fxDist -= eC_FixMul( (y1 - eC_FixFromInt(y1i)) + fxClip, fxDistInc );
		// texture subpixel correction
		eC_Fixed fxT = pkStyle->fxTex + eC_FixMul( fxDist , pkStyle->fxTexMul );

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
				// first pixel 
				uiAlpha = eC_FixFracToByte(y1) ^ 0xff;
				pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
				A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}
		y1i_tmp += eC_FixFromInt(1);
		A1::IncY( pBitmap, kOffset );
		y1i++;
		// edge distance update
		fxDist += fxDistInc;
		// texture update
		fxT += pkStyle->fxTexInc;

		// full pixels
		for (eC_Int y=y1i; y<y2i; y++)
		{	
			if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))){
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
					pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
					A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) uiAlpha2 );
				}
			}
			y1i_tmp += eC_FixFromInt(1);
			A1::IncY( pBitmap, kOffset );
			// edge distance update
			fxDist += fxDistInc;
			// texture update
			fxT += pkStyle->fxTexInc;
		}

		// last pixel
		if (((y1i_tmp >= min_Minor) && (y1i_tmp  < max_Minor))) {
			uiAlpha =  eC_FixFracToByte(y2);

			fxD = fxDist2-fxDist;
			//BMZ (overwrite fix)
			if ((fxDist>0)&&(fxD>=0) && uiAlpha!=0)
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
				pxC = (P1) pkStyle->aclColors[ eC_FixFracToByte(fxT) ];
				A1::BlendPixelGamma( kOffset, pxC, (eC_UByte) eGML_DISTCOMBINE(uiAlpha, uiAlpha2) );
			}
		}
	}


#endif

	// used when (y2-y1)<=1

	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle, eC_Fixed max_Minor, eC_Fixed min_Minor, eC_Fixed max_Major, eC_Fixed min_Major)
	{
		/* protect buffer */
		BREAK_ON_OVERFLOW(x,max_Major,0);
		BREAK_ON_UNDERFLOW(x,min_Major);
		/* protect buffer */

		BREAK_ON_OVERFLOW(x,max_Major,0);

		P2   kOffset;
		eC_UInt uiAlpha;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		BREAK_ON_OVERFLOW(y,max_Minor,0);

		uiAlpha = eC_FixFracToByte(y);

		if (eC_FixToInt(y) >= eC_FixToInt(min_Minor)) {
			A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) (uiAlpha ^ 0xff) );
			BREAK_ON_OVERFLOW(y,max_Minor,1);
			A1::IncY( pBitmap, kOffset );
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha );
		} else if (eC_FixToInt(y)+1 >= eC_FixToInt(min_Minor)) {
			BREAK_ON_OVERFLOW(y,max_Minor,1);
			A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y + eC_FixFromInt(1)), kOffset );
			A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha );
		}
	}	


	static inline void RenderPixel(eGML_Bitmap *pBitmap, eC_Fixed x, eC_Fixed y, P3 kStyle)
	{
		P2   kOffset;
		eC_UInt uiAlpha;
		P1   pxColor = *(static_cast<P1*>((void*)&kStyle));

		A1::GetOffset( pBitmap, eC_FixToInt(x), eC_FixToInt(y), kOffset );

		uiAlpha = eC_FixFracToByte(y);
		A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) (uiAlpha ^ 0xff) );
		A1::IncY( pBitmap, kOffset );
		A1::BlendPixelGamma( kOffset, pxColor, (eC_UByte) uiAlpha );
	}	

	// vector access
	static inline eC_Value GetMajFromVector( const eGML_Vector2 &rkVector )  { return rkVector.x; }
	static inline eC_Value GetMinFromVector( const eGML_Vector2 &rkVector )  { return rkVector.y; }
};


#endif
