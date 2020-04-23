/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_PrecompiledImageInfo.h
* %version:        140.0 %
*
* author:          hh04030
* creation date:   Mrz 14 2014
* description:

* %date_modified:  % (%derived_by: hh04030 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  03-14-14     | hh04030       | Initial version

******************************************************************************/

#ifndef _EGML_PRECOMPILEDIMAGEINFO_H_
#define _EGML_PRECOMPILEDIMAGEINFO_H_

#include "eGML.h"

   typedef enum
   {
      RLECURDECOMPRESSTOKEN_GETNEXT = 0,
      RLECURDECOMPRESSTOKEN_ABS = 1,
      RLECURDECOMPRESSTOKEN_RLE = 2,
      RLECURDECOMPRESSTOKEN_END = 3,
   }  RLECurDecompresstoken;

   typedef struct
   {
      eC_UByte * pubyCurRLEDataByte;
      RLECurDecompresstoken eCurrentToken;
      eC_UInt CurrentTokenDataposition;
      eC_UInt CurrentTokenDatacount;
   }  RLE8CurDecompressInfo;

   typedef struct
   {
      eC_UShort * pubyCurRLEDataByte;
      RLECurDecompresstoken eCurrentToken;
      eC_UInt CurrentTokenDataposition;
      eC_UInt CurrentTokenDatacount;
   }  RLE16CurDecompressInfo;


// Structure to store information about precompiled Bitmaps
class eGML_PrecompiledImageInfo
{
public:
  eC_UInt uieGML;
  eC_UInt ui_RLE;

  eC_UByte ubRLEBits;

  eC_UByte ubVersion;

  eC_UByte bOpaqueColorsOnly;
  eC_UByte ubReserved;

  eC_UByte ubColorBitsA;
  eC_UByte ubColorBitsR;
  eC_UByte ubColorBitsG;
  eC_UByte ubColorBitsB;

  eC_UInt uiWidth;
  eC_UInt uiHeight;
  eC_UInt uiPaletteColorCount;

  eC_UInt uiReserved1;
  eC_UInt uiReserved2;

  eC_UInt uiStartIndexOfColorIndices;

  eGML_ColorARGB32 acolColorPalette[1];

  eC_UByte * GetRLEDataBytes()
  {
      return &(((eC_UByte *)this)[uiStartIndexOfColorIndices]);
  }

};

#endif // _EGML_PRECOMPILEDIMAGEINFO_H_


/*************************** End of file ****************************/
