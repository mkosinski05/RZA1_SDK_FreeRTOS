/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapRGB32RLE16.h
* %version:        134.1 %
* 
* author:          hh04019
* creation date:   Tue Feb 07 13:25:07 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BITMAPRGB32RLE16__H_
#define EGML_BITMAPRGB32RLE16__H_
//---------------------------------------------------------------	

#include "eGML_Bitmap.h"
#include "eGML_Screen.h"

//---------------------------------------------------------------	
// pixel type

#ifdef eGML_DEBUGPIXELACCESS

typedef eC_UInt			eGML_PixelRGB32;

struct eGML_OffsetRGB32
{
  eC_Int           iX;
  eC_Int           iY;
  eC_Int           iMinX;
  eC_Int           iMinY;
  eC_Int           iMaxX;
  eC_Int           iMaxY;
  eC_Int           iWidth;
  eC_Int           iHeight;
  eGML_PixelRGB32 *pD;  
};


#else

typedef eC_UInt			eGML_PixelRGB32;
typedef eGML_PixelRGB32 *	eGML_OffsetRGB32;

#endif

#define eGML_BitmapRGB32RLE16_IgnoreMSB		// set if it is safe to consider upper byte to be 0x00  (0x00rrggbb)
#define eGML_BitmapRGB32RLE16_UseSWAR		// set to enable SWAR optimization techniques (SIMD Within A Register)

//---------------------------------------------------------------	
/* !!! INTERNAL USE ONLY !!!
Bitmap using 32bit RGB layout. */
class eGML_BitmapRGB32RLE16 : public eGML_Bitmap
{
  public:
    
    //
    virtual void *Lock();
    
    //
    virtual void Unlock(eC_UInt uiX1=0, eC_UInt uiY1=0, eC_UInt uiX2=0, eC_UInt uiY2=0);
    
    //
    virtual void Blit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
    
    //
    virtual void BlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255);
    
    //
    virtual void BlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);
	virtual void Blit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);

    //
    virtual void StretchBlit(eGML_Bitmap *pkSource, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
    
    //    
    virtual void StretchBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255);
    
    //
    virtual void StretchBlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);
	virtual void StretchBlit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);

    //
    virtual void RotBlit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
    
    //
    virtual void RotBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
	//virtual void RotBlitAlpha2Col(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255, eGML_Color cCol1 = 0, eGML_Color cCol2 = 0);
    
    //
    virtual void RotBlitColor(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
	virtual void RotBlit2Color(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);

    //
    virtual void Clear(eGML_Color clColor);
    
    //
    virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_ColorARGB32 clColor, eC_Flag ulFlags = 0);

    //
    virtual eGML_Color ColorFromRGB( eC_UByte ubR, eC_UByte ubG, eC_UByte ubB ) const;
    
    virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;
    
    virtual eC_Bool HasAlpha() const;

    //
    virtual void DrawLine( const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth=0 );
    
    //
    virtual void DrawPolygon( const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags );
    
    //
    virtual void DrawDot( eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0 );

  protected:
    //
    eGML_BitmapRGB32RLE16(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
    //
    virtual ~eGML_BitmapRGB32RLE16();

  private:
    
    friend class eGML_PixelRGB32_Access;
    friend class eGML_Screen;		
    
    
    //
    virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor );
    
    //
    virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor );		
};


//---------------------------------------------------------------	
#endif
