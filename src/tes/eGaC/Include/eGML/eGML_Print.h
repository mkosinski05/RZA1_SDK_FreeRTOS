/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Print.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:18:56 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_PRINT__H_
#define EGML_PRINT__H_
//---------------------------------------------------------------	

#include "eGML_Bitmap.h"
#include "eGML_DebugFont.h"

//---------------------------------------------------------------	
eDBG_PERF_DEF( DebugText );

//---------------------------------------------------------------	
// Simple fontblitter used for debugfont only.
//
template <
		  typename P2,		// Pixeltype for Destination
		  typename O2,		// Offsettype for Destination
		  class A2			// Accessmethods for Destination
		 > 

class eGML_DebugFont_Print
{
public:

	static eC_Int Blit_Char(eC_UByte ubChar, eGML_Bitmap *pkDest, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor )
	{
		O2			kDstLine;
		eC_UByte   *ubSrc;
		eC_Int		iWidth, iHeight;
		P2			pxColor;       
		
		if (!DebugFont_GetChar(ubChar, (void **)&ubSrc, iWidth, iHeight)) return 0;		

        // culling
        if (((uiX) < pkDest->GetClipMinX()) ||
            ((uiY) < pkDest->GetClipMinY()) ||
            ((uiX + iWidth) > pkDest->GetClipMaxX()) ||
            ((uiY + iHeight) > pkDest->GetClipMaxY()) ) return iWidth;

        eDBG_PERF_BEGIN( DebugText );

		A2::GetOffset( pkDest, uiX, uiY, kDstLine );		
		pxColor = A2::ColorToPixel( kColor );

		for (eC_Int y=0; y < iHeight; y++)
		{
			O2 kDst = kDstLine;
			for (eC_Int x=0; x < iWidth; x++)
            {				 
				if (*ubSrc++) A2::PutPixel( kDst, pxColor );
				A2::IncX( pkDest, kDst );
			}
			A2::IncY( pkDest, kDstLine );
		}

        eDBG_PERF_END( DebugText );
		return iWidth;
	}
};


//---------------------------------------------------------------	
#endif

