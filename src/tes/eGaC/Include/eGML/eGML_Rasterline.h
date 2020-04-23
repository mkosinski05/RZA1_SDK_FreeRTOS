/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Rasterline.h
* %version:        134.1 %
* 
* author:          hh04027
* creation date:   Mon Nov 28 13:14:56 2005
* description:

* %date_modified:  % (%derived_by: hh04027 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
  28-08-08     | hh04058       | fixed wrong span clipping, of spans, which are "drawn" outside the cliprect
                                 (the span-begin and have been clamped to the clip-limit, which results in a wrong thin line
								  along the clip-limit)
                                 
******************************************************************************/


#ifndef EGML_RASTERLINE__H_
#define EGML_RASTERLINE__H_

//---------------------------------------------------------------	
// rasterization for widelines inside guardband
//
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterLong( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    register eC_Fixed fxPosMaj = kData.fxPosMaj;
    register eC_Fixed fxPosMin = kData.fxPosMin;
    register eC_Fixed fxSpan = kData.fxSpan;
    eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj = 0;

	#ifndef eGML_LINEPATTERN_DISABLE
    // function is valid for pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value == 0) 
    	fxDistIncMaj = 1;
	#endif

	if ( fxDistIncMaj == 0)
	{
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);
	   
#ifdef eGML_VARIWIDTH
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
	    // shift minpos to upper edge
	    fxPosMin -= fxSpan;
	    fxSpan += fxSpan;
	
		// start triangle
		while (fxPosMaj <= kData.fxMid1Maj) 
		{			
			S1::RenderSpanEdge( pBitmap, fxPosMaj, fxPosMin, fxPosMin+fxSpan, fxDist, fxDistIncMaj, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
		}
	
		// parallel midpart
		while (fxPosMaj < kData.fxMid2Maj)
		{
			S1::RenderSpan( pBitmap, fxPosMaj, fxPosMin, fxPosMin+fxSpan, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;		
		}
	
	    // switch halfplane
	    fxDist = eC_FixMul(( eC_FixFloor(fxPosMaj) - (kPos.fxStartMaj+kPos.fxDeltaMaj)), -kPos.fxNormalMaj) + 
				 eC_FixMul(( fxPosMin - (kPos.fxStartMin+kPos.fxDeltaMin)), -kPos.fxNormalMin);
	
		// end triangle    
		while (fxPosMaj < kData.fxEndMaj)
		{
			S1::RenderSpanEdge( pBitmap, fxPosMaj, fxPosMin, fxPosMin+fxSpan, fxDist, -fxDistIncMaj, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	 
			fxDist -= fxDistIncMin;
		}
	}
}

#ifdef eGML_VARIWIDTH
//---------------------------------------------------------------	
// rasterization for variwidth widelines inside guardband
//
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterLongV( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    register eC_Fixed fxPosMaj = kData.fxPosMaj;
    register eC_Fixed fxPosMin = kData.fxPosMin;
    register eC_Fixed fxSpan = kData.fxSpan;
    eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj = 0;

	#ifndef eGML_LINEPATTERN_DISABLE
    // function is valid for pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value == 0) 
    	fxDistIncMaj = 1; //break reason
	#endif


	if( fxDistIncMaj == 0)
	{
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);
	   
#ifdef eGML_VARIWIDTH
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
		// start triangle
		while (fxPosMaj <= kData.fxMid1Maj) 
		{			
			S1::RenderSpanEdge( pBitmap, fxPosMaj, fxPosMin-fxSpan, fxPosMin+fxSpan, fxDist, fxDistIncMaj, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
			fxSpan += kData.fxSpanInc;
		}
	
		// parallel midpart
		while (fxPosMaj < kData.fxMid2Maj)
		{
			S1::RenderSpan( pBitmap, fxPosMaj, fxPosMin-fxSpan, fxPosMin+fxSpan, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;		
			fxSpan += kData.fxSpanInc;
		}
	
	    // switch halfplane
	    fxDist = eC_FixMul(( eC_FixFloor(fxPosMaj) - (kPos.fxStartMaj+kPos.fxDeltaMaj)), -kPos.fxNormalMaj) + 
				 eC_FixMul(( (fxPosMin-fxSpan) - (kPos.fxStartMin+kPos.fxDeltaMin)), -kPos.fxNormalMin);
	
		// end triangle    
		while (fxPosMaj < kData.fxEndMaj)
		{
			S1::RenderSpanEdge( pBitmap, fxPosMaj, fxPosMin-fxSpan, fxPosMin+fxSpan, fxDist, -fxDistIncMaj, kStyle );
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	
			fxDist -= fxDistIncMin;
			fxSpan += kData.fxSpanInc;
		}
	}
}
#endif

//---------------------------------------------------------------	
// rasterization for widelines inside guardband that are shorter
// than wide (inverted lines)
//
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterShort( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    register eC_Fixed fxPosMaj = kData.fxPosMaj;
    register eC_Fixed fxPosMin = kData.fxPosMin;
    register eC_Fixed fxSpan = kData.fxSpan;
    eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj;
    eC_Fixed fxDist2 = 0;

	#ifndef eGML_LINEPATTERN_DISABLE
    // function is valid for pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value == 0) 
    	fxDist2 = 1; //break reason
	#endif

	if( fxDist2 == 0)
	{
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);
	   
	    //fxDist2 = kPos.fxLen + eC_FixFromInt(1);
		fxDist2 = kPos.fxLen + eC_FixFromInt(1);
	
#ifdef eGML_VARIWIDTH
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
		// render as parallelogram
		while (fxPosMaj <= kData.fxEndMaj) 
		{	
			S1::RenderSpan2Edge( pBitmap, fxPosMaj, fxPosMin-fxSpan, fxPosMin+fxSpan, fxDist, fxDistIncMaj, fxDist2, 0, kStyle );        
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
			#ifdef eGML_VARIWIDTH
			fxSpan += kData.fxSpanInc;
			#endif
		}
	}
}

//---------------------------------------------------------------	
// rasterization for widelines 'onedge' that are shorter than wide
//
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterShortClip( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    register eC_Fixed fxPosMaj = kData.fxPosMaj;
    register eC_Fixed fxPosMin = kData.fxPosMin;
    register eC_Fixed fxSpan = kData.fxSpan;
    eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj;
    eC_Fixed fxDist2, fxEndMaj = 0;

	#ifndef eGML_LINEPATTERN_DISABLE
    // function is valid for pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value == 0) 
    	fxEndMaj = 1;
	#endif

	if ( fxEndMaj == 0)
	{
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);

		// some one put this here, but this makes the line-end-distnace to large
		// the result is an strict X-axis / Y-axis parallel edge 

		fxDist2 = kPos.fxLen + eC_FixFromInt(1); // Dist full intensity, original code
#ifdef eGML_VARIWIDTH
	    fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
	    // clipping
	    fxEndMaj = eC_FixMin( kData.fxEndMaj, kPos.kMajClip.fxMax );
		eC_Fixed fxBottom = kPos.kMinClip.fxMax;
	
		// render as parallelogram 
		
		while (fxPosMaj <= fxEndMaj) 
		{	
	        eC_Fixed fxClip = 0, fxMinBottom, fxMaxTop;
	
			/* what happens here?
				1) the lines needs to be drawn over the cliprect
				2) we need to clip pixelwise the overdrawn area
				3) we need to reduce span-length to the visible area
				    that means, we need to move the start and endpoint 
					of the spandraw-methode into the cliprect 
			*/

			fxMinBottom = fxPosMin + fxSpan ; 
			fxMaxTop =  fxPosMin - fxSpan; 
			
			if (!(flags & eGML_LF_ANTIALIAS) && !(flags & eGML_LF_HQ_AA)) {

				if (!(fxMinBottom < kPos.kMinClip.fxMin) && !(fxMaxTop > (kPos.kMinClip.fxMax+ eC_FixFromInt(1))) && (fxPosMaj >= kPos.kMajClip.fxMin )){
					// now we need to move the start and endpoints 
					fxMinBottom = eC_FixMin( fxMinBottom, fxBottom );					
					fxClip      = fxMaxTop - kPos.kMinClip.fxMin;
					if (fxClip > 0) fxClip = 0; // if the startpoint is below the min-clip-margin, we move it into the cliprect
					fxMaxTop   -= fxClip ;
								
					if (fxMaxTop > fxMinBottom) 
					{
						if (fxMinBottom < kPos.kMinClip.fxMin) {
							
							fxMinBottom = kPos.kMinClip.fxMin;					
						} 
						fxMaxTop = fxMinBottom; 				
					}
					
					// no pixelwise clipcheck, since we clip already here
					S1::RenderSpan2Edge( pBitmap, fxPosMaj, fxMaxTop, fxMinBottom, fxDist, fxDistIncMaj, fxDist2, fxClip, kStyle );        
				}
				
			} else {
				// no pixelwise clipcheck, since we need to overdraw the region, to get missing AA-Pixels on bitmapedges
				S1::RenderSpan2Edge( pBitmap, fxPosMaj, fxMaxTop, fxMinBottom, fxDist, fxDistIncMaj, fxDist2, 0, kStyle, kPos.kMinClip.fxMax, kPos.kMinClip.fxMin, kPos.kMajClip.fxMax, kPos.kMajClip.fxMin);        
			}
			
			fxPosMaj += eC_FixFromInt(1);
	
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
			#ifdef eGML_VARIWIDTH
 			fxSpan += kData.fxSpanInc;
			#endif
		}
	}
}


//---------------------------------------------------------------	
// rasterization for pattern lines inside guardband
//
#ifndef eGML_LINEPATTERN_DISABLE
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterPatternV( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    // function is valid for NON pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value != 1)     
	{
	    register eC_Fixed fxPosMaj = kData.fxPosMaj;
		register eC_Fixed fxPosMin = kData.fxPosMin;
		register eC_Fixed fxSpan = kData.fxSpan;
		eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj;
		eC_Fixed fxDist2, fxTexIncMin;

	    eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );
	
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);
	   
	    fxDist2 = kPos.fxLen + eC_FixFromInt(1);
	
#ifdef eGML_VARIWIDTH
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
	    // texture index interpolation    
	    pkStyle->fxTexInc = eC_FixMul( fxDistIncMaj , pkStyle->fxTexMul );
	    fxTexIncMin = eC_FixMul( fxDistIncMin , pkStyle->fxTexMul );
	
	    // flip texture for lines drawn end->start
		eC_Fixed fxXO = kPos.fxStartMaj - eC_FixFromValue( S1::GetMajFromVector( pkStyle->kStart ) );
		if (pkStyle->bFlip) 
		{
			pkStyle->aclColors = pkStyle->aclColorBase + 256;
			pkStyle->fxTex = eC_FixMul(eC_FixFloor(fxXO), fxTexIncMin) + (eC_FixFromInt(256) - pkStyle->fxTexOffset);
		} else {
			pkStyle->aclColors = pkStyle->aclColorBase;
			pkStyle->fxTex = eC_FixMul(eC_FixFloor(fxXO), fxTexIncMin) + pkStyle->fxTexOffset;
		}		

		// render as parallelogram
		while (fxPosMaj <= kData.fxEndMaj) 
		{	
			S1::RenderSpanPattern2Edge( pBitmap, fxPosMaj, fxPosMin-fxSpan, fxPosMin+fxSpan, fxDist, fxDistIncMaj, fxDist2, 0, kStyle );
			fxPosMaj += eC_FixFromInt(1);
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
			#ifdef eGML_VARIWIDTH
			fxSpan += kData.fxSpanInc;
			#endif
		}
	}
}
#endif

//---------------------------------------------------------------	
// rasterization for pattern lines 'onEdge' or 'inverted'
//
#ifndef eGML_LINEPATTERN_DISABLE
template <typename L1, class S1>
void eGML_Linedraw_Octant<L1,S1>::rasterPatternClip( eGML_Bitmap *pBitmap, const L1 &kStyle, const rasterData &kData, const octantData &kPos, const eC_Flag &flags)
{
    // function is valid for NON pixeltype styles only)
    if (eGML_PixelTypeTrait<L1>::value != 1)    
	{
		register eC_Fixed fxPosMaj = kData.fxPosMaj;
		register eC_Fixed fxPosMin = kData.fxPosMin;
		register eC_Fixed fxSpan = kData.fxSpan;
		eC_Fixed fxDist, fxDistIncMin, fxDistIncMaj;
		eC_Fixed fxDist2, fxEndMaj, fxTexIncMin;

	    eGML_Line_patternStyle *pkStyle = static_cast<eGML_Line_patternStyle*>( (void*)((eC_UInt)kStyle) );
	
		// edge half-plane distance initialisation
		fxDist  = eC_FixMul((eC_FixFloor(fxPosMaj) - kPos.fxStartMaj), kPos.fxNormalMaj) + 
				  eC_FixMul(( (fxPosMin-fxSpan) - kPos.fxStartMin), kPos.fxNormalMin);
	   
	    fxDist2 = kPos.fxLen + eC_FixFromInt(1);
	
#ifdef eGML_VARIWIDTH
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin - kData.fxSpanInc, kPos.fxNormalMin);
#else
        fxDistIncMin = kPos.fxNormalMaj + eC_FixMul(kData.fxIncMin, kPos.fxNormalMin);
#endif
	    fxDistIncMaj = kPos.fxNormalMin;
	
	    // clipping
	    fxEndMaj = eC_FixMin( kData.fxEndMaj, kPos.kMajClip.fxMax );
	
	    // texture index interpolation    
	    pkStyle->fxTexInc = eC_FixMul( fxDistIncMaj , pkStyle->fxTexMul );
	    fxTexIncMin = eC_FixMul( fxDistIncMin , pkStyle->fxTexMul );
		
		eC_Fixed fxXO = kPos.fxStartMaj - eC_FixFromValue( S1::GetMajFromVector( pkStyle->kStart ) );

		// flip texture for lines drawn end->start
		if (pkStyle->bFlip) 
		{
			pkStyle->aclColors = pkStyle->aclColorBase + 256;
			pkStyle->fxTex = eC_FixMul(eC_FixFloor(fxXO), fxTexIncMin) + (eC_FixFromInt(256) - pkStyle->fxTexOffset);
		} else {
			pkStyle->aclColors = pkStyle->aclColorBase;
			pkStyle->fxTex = eC_FixMul(eC_FixFloor(fxXO), fxTexIncMin) + pkStyle->fxTexOffset;
		}	

	    // correct clipping
		eC_Fixed fxBottom = kPos.kMinClip.fxMax;
	
		while (fxPosMaj <= fxEndMaj) 
		{	
	        eC_Fixed fxClip = 0, fxMinBottom, fxMaxTop;
	
			/* what happens here?
				1) the lines needs to be drawn over the cliprect
				2) we need to clip pixelwise the overdrawn area
				3) we need to reduce span-length to the visible area
				    that means, we need to move the start and endpoint 
					of the spandraw-methode into the cliprect 
			*/

			fxMinBottom = fxPosMin + fxSpan; 
			fxMaxTop =  fxPosMin - fxSpan;	
				
			if (!(flags & eGML_LF_ANTIALIAS) && !(flags & eGML_LF_HQ_AA)) {

				if (!(fxMinBottom < kPos.kMinClip.fxMin) && !(fxMaxTop > (kPos.kMinClip.fxMax+ eC_FixFromInt(1))) && (fxPosMaj >= kPos.kMajClip.fxMin )){
					// now we need to move the star and endpoints 
					fxMinBottom = eC_FixMin( fxPosMin + fxSpan, fxBottom );				
					fxClip      = fxMaxTop - kPos.kMinClip.fxMin;
					if (fxClip > 0) fxClip = 0; // if the startpoint is below the min-clip-margin, we move it into the cliprect
					fxMaxTop   -= fxClip ;
								
					if (fxMaxTop > fxMinBottom) 
					{
						if (fxMinBottom < kPos.kMinClip.fxMin) {
							fxMinBottom = kPos.kMinClip.fxMin;					
						} 
						fxMaxTop = fxMinBottom; 				
					}
					// no pixelwise clipcheck, since we clip already here					
					S1::RenderSpanPattern2Edge( pBitmap, fxPosMaj, fxMaxTop, fxMinBottom, fxDist, fxDistIncMaj, fxDist2, fxClip, kStyle );        			
				}
				
			} else 
				// no pixelwise clipcheck, since we need to overdraw the region, to get missing AA-Pixels on bitmapedges
				S1::RenderSpanPattern2Edge( pBitmap, fxPosMaj, fxMaxTop, fxMinBottom, fxDist, fxDistIncMaj, fxDist2, fxClip, kStyle,kPos.kMinClip.fxMax, kPos.kMinClip.fxMin, kPos.kMajClip.fxMax, kPos.kMajClip.fxMin );        			

	        
			fxPosMaj += eC_FixFromInt(1);
			fxPosMin += kData.fxIncMin;	
			fxDist += fxDistIncMin;
			#ifdef eGML_VARIWIDTH
			fxSpan += kData.fxSpanInc;
			#endif
		}
	}
}
#endif

//---------------------------------------------------------------	
#endif

