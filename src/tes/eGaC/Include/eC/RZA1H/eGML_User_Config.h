/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_User_Config.h
* %version:        134.0 %
*
* author:          hh04019
* creation date:   Fri Sep 09 18:46:45 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef __EGML_USER_CONFIG_H__
#define __EGML_USER_CONFIG_H__

// eGML - Config ------------------------------------------
#define eGML_TARGETGAMMA    2.2     // default gamma factor for target display (used in AA calc only)
#define eGML_TEXTURESTEP    8       // default subdivision step for perspective correction
//#define eGML_TEXTUREFLOAT           // use floating point values for perspective correct texture loop


#define eGML_FILTEREDAA							// support for filtered (hq) antialiasing
#define eGML_VARIWIDTH							// support width variation along wide lines.

#define eGML_USEBITMAPRGB565
//#define eGML_USEBITMAPRGB32					// support for basic bitmap rgb32
#define eGML_USEBITMAPA8						// for basic bitmap alpha8
//#define eGML_USEEMEM                // support of eMem, all memory allocation of eGML will be explicit done over eMEM
#define eGML_BitmapRGB565_UseSWAR      // set to enable SWAR optimization techniques (SIMD Within A Register)


//Enable/Disable lines
#define eGML_LINEPATTERN_DISABLE

//Enable/Disable polygons
#define eGML_POLYGONTEXTURE_DISABLE
#define eGML_POLYGONALPHA_DISABLE


#define eGML_ALPHA8_POLYGON_DISABLE
#define eGML_ALPHA8_LINE_DISABLE
#define eGML_ALPHA8_DOT_DISABLE

//#define eGML_ALPHA8_BLIT_DISABLE
#define eGML_ALPHA8_BLITALPHA_DISABLE
#define eGML_ALPHA8_BLITCOLOR_DISABLE

//#define eGML_ALPHA8_STRETCHBLIT_DISABLE
#define eGML_ALPHA8_STRETCHBLITALPHA_DISABLE
#define eGML_ALPHA8_STRETCHBLITCOLOR_DISABLE

#define eGML_ALPHA8_ROTBLIT_DISABLE
#define eGML_ALPHA8_ROTBLITALPHA_DISABLE
#define eGML_ALPHA8_ROTBLITCOLOR_DISABLE


//#define eGML_RGB32_POLYGON_DISABLE
//#define eGML_RGB32_LINE_DISABLE
//#define eGML_RGB32_DOT_DISABLE

//#define eGML_RGB32_BLIT_DISABLE
#define eGML_RGB32_BLITALPHA_DISABLE
#define eGML_RGB32_BLITCOLOR_DISABLE

//#define eGML_RGB32_STRETCHBLIT_DISABLE
#define eGML_RGB32_STRETCHBLITALPHA_DISABLE
#define eGML_RGB32_STRETCHBLITCOLOR_DISABLE

#define eGML_RGB32_ROTBLIT_DISABLE
#define eGML_RGB32_ROTBLITALPHA_DISABLE
#define eGML_RGB32_ROTBLITCOLOR_DISABLE
#endif

