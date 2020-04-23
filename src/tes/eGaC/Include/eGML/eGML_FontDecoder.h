/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_FontDecoder.h
* %version:        134.0 %
*
* author:          hh04058
* creation date:   Thu May 04 17:16:36 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
******************************************************************************/


#ifndef EGML_FONTDECODER__H_
#define EGML_FONTDECODER__H_
//---------------------------------------------------------------	

#include "eGML.h"


#ifndef eC_QUICKDIV_255
#define eC_QUICKDIV_255(x) ((eC_UByte)((((x) + ((x) >> 8) + 1) >> 8)))
#endif

#ifndef INSIDE_BIT_MASK
#define INSIDE_BIT_MASK 6 // 0000011X
#endif


#ifndef FLAG_BIT_MASK
#define FLAG_BIT_MASK 6 // 0000011X
#endif


#ifndef OUTLINE_BIT_MASK
#define OUTLINE_BIT_MASK 4 // 0000010X
#endif

#ifndef BIT_BLEND_MASK
#define BIT_BLEND_MASK  2 // 0000001X
#endif

#ifndef OUTSIDE_BIT_MASK
#define OUTSIDE_BIT_MASK 0 // 00000000
#endif


#ifndef A_
#define A_(x) (((x)& 0xff000000) >> 24)
#endif

#ifndef R_
#define R_(x) (((x)& 0x00ff0000) >> 16)
#endif

#ifndef G_
#define G_(x) (((x)& 0x0000ff00) >> 8)
#endif

#ifndef B_
#define B_(x) (((x)& 0x000000ff))
#endif




class eGML_FontDecoder {
public:
	static eGML_Color decodeColorFast(eC_UByte alpha, eGML_Color blendcolorOutside, eGML_Color blendcolorInside)
	{

		eC_UByte factor = 0;
		eGML_Color realAlpha;
		
		switch (alpha & FLAG_BIT_MASK)
		{
		case INSIDE_BIT_MASK:
			{
				// 5 Bit Alpha
				// 2. & 3. Bit Flag
				// 1. Bit Dummy 7 Bit Compression
				// x -> highest bits are copied to lowest bits for 5Bit to 8Bit conversion
				// f -> flags
				// D -> Dummybit, which might be killed by 7 Bit compression in the fontgenerator
				// (xxx)00(ff)D
				// (xxx)00(xxx)			
				// 224 = 8.7.6. Bitmask
				// >> 5 copy 3 Bits to lower bits
				factor = (alpha | (( alpha & 224) >> 5)) ;
				// just for testing
				realAlpha = 255UL << 24;
				return
					( realAlpha|
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(R_(blendcolorInside)* factor))<<16 |
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(G_(blendcolorInside)* factor))<<8 |
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(B_(blendcolorInside)* factor))  );
			}

		case OUTLINE_BIT_MASK:
			{
				factor = (alpha | (( alpha & 224UL) >> 5)) ;
				realAlpha = (alpha | (( alpha & 224UL) >> 5)) << 24; // copy the 3 top-bits to lowest bits and put them in A of ARGB
				return
					(realAlpha|
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(R_(blendcolorOutside)* factor))<<16 |
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(G_(blendcolorOutside)* factor))<<8 |
					 EC_STATIC_CAST(eC_UInt,eC_QUICKDIV_255(B_(blendcolorOutside)* factor))  );
			}

		case BIT_BLEND_MASK:
			{
				eC_UByte insideCL = (alpha | (( alpha & 224) >> 5));
				eC_UByte outside = (eC_UByte)(255 - insideCL);
				realAlpha = 255UL << 24;
				return
					realAlpha |
					(EC_STATIC_CAST(eC_UInt,(eC_QUICKDIV_255(R_(blendcolorOutside) * outside) + eC_QUICKDIV_255(R_(blendcolorInside) * insideCL))))<< 16 |
					(EC_STATIC_CAST(eC_UInt,(eC_QUICKDIV_255(G_(blendcolorOutside) * outside) + eC_QUICKDIV_255(G_(blendcolorInside) * insideCL))))<< 8 |
					(EC_STATIC_CAST(eC_UInt,(eC_QUICKDIV_255(B_(blendcolorOutside) * outside) + eC_QUICKDIV_255(B_(blendcolorInside) * insideCL))));
			}
		default:
			return 0;
		}
	}


	// 5Bit -> 8Bit alpha conversion
	static eC_UByte decodeBlendValue(eC_UByte alpha, eC_Flag & regionCode)
	{
		regionCode = alpha & FLAG_BIT_MASK;
		switch (regionCode)
		{
		case INSIDE_BIT_MASK:
			{
				return 255;
			}

		case OUTLINE_BIT_MASK:
			{
				return (alpha | (( alpha & 224) >> 5)) ; // 5bit -> 8 Bit
			}

		case BIT_BLEND_MASK:
			{
				return (alpha | (( alpha & 224) >> 5)) ;
			}
		default:
			return 0;
		}
	}

};

#endif

