/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapARGB32.h
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


#ifndef EGML_BITMAPARGB32__H_
#define EGML_BITMAPARGB32__H_
//---------------------------------------------------------------	

#include "eGML_Bitmap.h"
#include "eGML_Screen.h"
#include "eGML_BitmapA8.h"
#include "eGML_FontDecoder.h"
#include "eGML_Blit.h"

//---------------------------------------------------------------	
// pixel type

#ifdef eGML_DEBUGPIXELACCESS

typedef eC_UInt			eGML_PixelARGB32;

struct eGML_OffsetARGB32
{
  eC_Int           iX;
  eC_Int           iY;
  eC_Int           iMinX;
  eC_Int           iMinY;
  eC_Int           iMaxX;
  eC_Int           iMaxY;
  eC_Int           iWidth;
  eC_Int           iHeight;
  eGML_PixelARGB32 *pD;  
};


#else

typedef eC_UInt			eGML_PixelARGB32;
typedef eGML_PixelARGB32 *	eGML_OffsetARGB32;

#endif

#define eGML_BitmapARGB32_IgnoreMSB		// set if it is safe to consider upper byte to be 0x00  (0x00rrggbb)
#define eGML_BitmapARGB32_UseSWAR		// set to enable SWAR optimization techniques (SIMD Within A Register)

//---------------------------------------------------------------	
/* !!! INTERNAL USE ONLY !!!
Bitmap using 32bit RGB layout. */
class eGML_BitmapARGB32 : public eGML_Bitmap
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

    //
    virtual eGML_Color ColorFromARGB( eC_UByte ubA, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB ) const;
    
    virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;
    
    virtual eC_Bool HasAlpha() const;

    //
    virtual void DrawLine( const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth=0 );
    
    //
    virtual void DrawPolygon( const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags );
    
    //
    virtual void DrawDot( eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0 );

    // utility method to premultiply a color
    static eGML_Color Premultiply(eGML_Color clColorAndAlpha);

  protected:
    //
    eGML_BitmapARGB32(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
    //
    virtual ~eGML_BitmapARGB32();

  private:
    
    friend class eGML_PixelARGB32_Access;
    friend class eGML_Screen;		
    
    eC_Bool m_bHasAlpha;
    
    //
    virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor );
    
    //
    virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor );		
};

//---------------------------------------------------------------	
// lowlevel access methods for this kind of bitmap

/* !!! INTERNAL USE ONLY !!!
Access functions for eGML_BitmapARGB32.
See eGML_Pixel_Access, eGML_BitmapARGB32 for general info */
class eGML_PixelARGB32_Access
{
  public :
    
    //
    static inline eGML_PixelARGB32 ColorToPixel(eGML_Color kColor)
    {
      return (eGML_PixelARGB32) kColor;
    }

    //
    static inline eGML_PixelARGB32 ColorAndAlphaToPixel(eGML_Color kColor, eC_UByte ubAlpha)
    {
      return (eGML_PixelARGB32) ((kColor&0xffffff) | (ubAlpha << 24));
    }
    
    //
    static inline eGML_Color PixelToColor(eGML_PixelARGB32 pxPixel)
    {
      return (eGML_Color) (pxPixel&0xffffff);
    }

    //
    static inline eGML_Color PixelToColorAndAlpha(eGML_PixelARGB32 pxPixel, eC_UByte & rubAlpha)
    {
       rubAlpha = eC_UByte(pxPixel >> 24);
       return PixelToColor(pxPixel);
    }
    
	//
    static inline eGML_PixelARGB32 PixelFromRGB(eC_UByte ubR, eC_UByte ubG, eC_UByte ubB)
    {
      // PixelFromRGB assumes an opaque pixel
      return ((0xFF000000) | ( ((eC_UInt)ubR) << 16) | (((eC_UInt)ubG) << 8) | (eC_UInt)ubB );
    }

	//
    static inline eGML_PixelARGB32 PixelFromColorARGB32( eGML_ColorARGB32 colARGB32 )
    {
      return (eGML_PixelARGB32)(colARGB32);
    }
	
   static inline eGML_ColorARGB32 ColorARGB32FromPixel( eGML_PixelARGB32 pxPixel )
   {
      return (eGML_ColorARGB32)( pxPixel );
   }

    //
    static inline eC_UByte RedFromPixel(eGML_PixelARGB32 pxPixel)
    {
	  #ifdef eGML_BitmapARGB32_IgnoreMSB
	  return (eC_UByte) (pxPixel >> 16);
	  #else
      return (eC_UByte) ((pxPixel >> 16) & 0xff);
	  #endif
    }
    
    //
    static inline eC_UByte GreenFromPixel(eGML_PixelARGB32 pxPixel)
    {
      return (eC_UByte) ((pxPixel >> 8) & 0xff);
    }
    
    //
    static inline eC_UByte BlueFromPixel(eGML_PixelARGB32 pxPixel)
    {
      return (eC_UByte) (pxPixel & 0xff);
    }	

    static inline eC_UByte AlphaFromPixel(eGML_PixelARGB32 pxPixel)
    {
	  #ifdef eGML_BitmapARGB32_IgnoreMSB
	  return (eC_UByte) (pxPixel >> 24);
	  #else
      return (eC_UByte) ((pxPixel >> 24) & 0xff);
	  #endif
    }

	//
    static inline eGML_PixelARGB32 RedBlueFromPixel(eGML_PixelARGB32 pxPixel)
    {
      return (pxPixel & 0xff00ff);
    }	

	//
	static inline eGML_PixelARGB32 PixelFromParts(eGML_PixelARGB32 pxRB, eC_UByte ubG)
    {
      return  (pxRB & 0xff00ff) | (((eC_UInt)ubG) << 8);
    }

#ifdef eGML_DEBUGPIXELACCESS
    static inline void InitOffset(eGML_BitmapARGB32 *pkBitmap, eC_Int x, eC_Int y, eGML_OffsetARGB32 &res)
    {
      res.iX     = x;
      res.iY     = y;
      res.iMinX  = pkBitmap->GetClipMinX();
      res.iMinY  = pkBitmap->GetClipMinY();
      res.iMaxX  = pkBitmap->GetClipMaxX();
      res.iMaxY  = pkBitmap->GetClipMaxY();
      res.iWidth = pkBitmap->GetWidth()-1;
      res.iHeight= pkBitmap->GetHeight()-1;
      res.pD    = (eGML_PixelARGB32 *)( ((eGML_Pixel*)pkBitmap->m_pkData) + (y * pkBitmap->m_uiPitch) + (x * 4) );
    }

    static inline bool CheckOffset( eGML_OffsetARGB32 &res , bool cliprange=true)
    {
      if (cliprange)
      {
        if ( (res.iX < res.iMinX) || (res.iX > res.iMaxX) || (res.iY < res.iMinY) || (res.iY > res.iMaxY) )       
        {
          eDBG_DP(" ILLEGAL OFFSET (CLIPRANGE) %d %d \n ", res.iX, res.iY );
          return false;
        }
      }
      else
      {
        if ( (res.iX < 0) || (res.iX > res.iWidth) || (res.iY < 0) || (res.iY > res.iHeight) )       
        {
          eDBG_DP(" ILLEGAL OFFSET (SIZE) %d %d \n ", res.iX, res.iY );
          return false;
        }
      }
      
      return true;
    }

    //
    static inline void GetOffset(eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetARGB32 &res)
    {
      eGML_BitmapARGB32 *pInternalBitmap = (eGML_BitmapARGB32 *) pBitmap;
      InitOffset(pInternalBitmap, (eC_Int)uiX, (eC_Int)uiY, res);      
    }
    
    //
    static inline void AddOffset(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetARGB32 &res)
    {
      eGML_BitmapARGB32 *pInternalBitmap = (eGML_BitmapARGB32 *) pBitmap;
      InitOffset(pInternalBitmap, ((eC_Int)uiX)+res.iX, ((eC_Int)uiY)+res.iY, res);
    }
    
    //
    static inline void IncX(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 &kOffset)
    {
      kOffset.pD++;
      kOffset.iX++;
    }
    
    //
    static inline void IncY(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 &kOffset)
    {
      kOffset.pD= (eGML_PixelARGB32 *) (((eGML_Pixel*)kOffset.pD) + ((eGML_BitmapARGB32 *)pBitmap)->m_uiPitch);
      kOffset.iY++;
    }
    
    //
    static inline eGML_PixelARGB32 GetPixel(eGML_OffsetARGB32 kOffset)
    {
      if (CheckOffset(kOffset, false))
      {
        return *kOffset.pD;
      }
      
      return 0;
    }

    //
    static inline void PutPixel(eGML_OffsetARGB32 kOffset, eGML_PixelARGB32 pxColor)
    {
      if (CheckOffset(kOffset))
      { 
        *kOffset.pD = pxColor;
      }      
    }
    
    //
    static inline void PutSpan(eGML_OffsetARGB32 &kOffset, eGML_PixelARGB32 pxColor, eC_Int iCount)
    {
      while (iCount-- >0)
      {
        if (CheckOffset(kOffset))
        {
          PutPixel(kOffset, pxColor);
        }
        kOffset.iX++;
        kOffset.pD++;
      }
    }
    
    //
    static inline void TextureOffset(const eGML_OffsetARGB32 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetARGB32 &res)
    {
      res.iX      = (eC_Int)uiX;
      res.iY      = (eC_Int)uiY;
      res.iMinX   = 0;
      res.iMinY   = 0;
      res.iMaxX   = 1023;
      res.iMaxY   = 1023;
      res.iHeight = 1023;
      res.iWidth  = 1023;
      res.pD      = base.pD + (uiY << uiN) + uiX;
    }
   

#else
    //
    static inline void GetOffset(eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetARGB32 &res)
    {
      eGML_BitmapARGB32 *pInternalBitmap = (eGML_BitmapARGB32 *) pBitmap;
      res = (eGML_OffsetARGB32)( ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 4) );
    }
    
    //
    static inline void AddOffset(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetARGB32 &res)
    {
      eGML_BitmapARGB32 *pInternalBitmap = (eGML_BitmapARGB32 *) pBitmap;
      res = (eGML_OffsetARGB32)( ((eGML_Pixel*)kOffset) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 4) );
    }
    
    //
    static inline void IncX(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 &kOffset)
    {
      kOffset++;
    }
    
    //
    static inline void IncY(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 &kOffset)
    {
      kOffset = (eGML_OffsetARGB32) (((eGML_Pixel*)kOffset) + ((eGML_BitmapARGB32 *)pBitmap)->m_uiPitch);
    }
    
    //
    static inline eGML_PixelARGB32 GetPixel(eGML_OffsetARGB32 kOffset)
    {
      return *kOffset;
    }

    //
    static inline void PutPixel(eGML_OffsetARGB32 kOffset, eGML_PixelARGB32 pxColor)
    {
      *kOffset = pxColor;
    }
    
    //
    static inline void PutSpan(eGML_OffsetARGB32 &kOffset, eGML_PixelARGB32 pxColor, eC_Int iCount)
    {     
         while (iCount-- >0) *kOffset++ = pxColor;      
    }
    
    //
    static inline void TextureOffset(const eGML_OffsetARGB32 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetARGB32 &res)
    {
      res = base + (uiY << uiN) + uiX;
    }
   
#endif //eGML_DEBUGPIXELACCESS   

    //
    static inline void BlendPixel(eGML_OffsetARGB32 kOffset, eGML_PixelARGB32 pxColor, eC_UByte ubAlpha)
    {
       // Ignore alpha component of pxColor
       //  multiply src color only by global alpha

       eGML_PixelARGB32 pxPixel = MixLinear( GetPixel(kOffset), pxColor, ubAlpha );
       PutPixel( kOffset, pxPixel );
    } 

    static inline void BlendPixelSrcAlpha(eGML_OffsetARGB32 kOffset, eGML_PixelARGB32 pxARGB, eC_UByte ubAlpha)
    {
       // Multiply src color by global alpha
       //  Consider src alpha when calculating destination alpha
       //  (required to make simple 'copy' blits work correctly)
       eGML_PixelARGB32 pxPixel = MixLinearSrcAlpha( GetPixel(kOffset), pxARGB, ubAlpha );
       PutPixel( kOffset, pxPixel);
    }   

    //
    static inline void AddPixel(eGML_OffsetARGB32 kOffset, eGML_PixelARGB32 pxColor)
    {
       eGML_PixelARGB32 pxPixel = MixAdditive( GetPixel(kOffset), pxColor );
       PutPixel( kOffset, pxPixel );
    }

	//
	static inline eGML_PixelARGB32 MixAverage(eGML_PixelARGB32 pxColor1, eGML_PixelARGB32 pxColor2)
	{
		#ifdef eGML_BitmapARGB32_UseSWAR
		// build avg of all components at once
		return (pxColor1 & pxColor2) + ((pxColor1 ^ pxColor2) >> 1);
		#else
		// TODO
		return (pxColor1 & pxColor2) + ((pxColor1 ^ pxColor2) >> 1);
		#endif
	}

    //
    static inline eGML_PixelARGB32 MixAdditive(eGML_PixelARGB32 pxColor1, eGML_PixelARGB32 pxColor2)
    {
		// color split and sum
		eC_UInt uiR = RedFromPixel(pxColor1)   + (eC_UInt) RedFromPixel(pxColor2);
		eC_UInt uiG = GreenFromPixel(pxColor1) + (eC_UInt) GreenFromPixel(pxColor2);
		eC_UInt uiB = BlueFromPixel(pxColor1)  + (eC_UInt) BlueFromPixel(pxColor2);
		// saturate (only lower 8bit) 
		uiR |= (-(eC_Int)(uiR & 0x100)) >> 8;
		uiG |= (-(eC_Int)(uiG & 0x100)) >> 8;
		uiB |= (-(eC_Int)(uiB & 0x100)) >> 8;
		// recompose 
		return PixelFromRGB( (eC_UByte)uiR, (eC_UByte)uiG, (eC_UByte)uiB );
    }

	//
    static inline eGML_PixelARGB32 MixLinear(const eGML_PixelARGB32 pxColor1, const eGML_PixelARGB32 pxColor2, eC_UByte ubAlpha)
    {
       // Cd = Cd*(1-Ag) + Cs*Ag
       // Ad = Ad*(1-Ag) + Ag
       //
       //
       //  pxColor1=Cd/Ad
       //  pxColor2=Cs/As

       // color split
       eC_UInt rb1 = (eC_UInt) RedBlueFromPixel(pxColor1);
       eC_UInt g1  = (eC_UInt) GreenFromPixel(pxColor1);
       eC_UInt rb2 = (eC_UInt) RedBlueFromPixel(pxColor2);
       eC_UInt g2  = (eC_UInt) GreenFromPixel(pxColor2);
       
       // alpha split
       eC_UByte ad = AlphaFromPixel(pxColor1);

       // Ad = Ad*(1-Ag) + Ag
       ad = eGMLBlit_MulABDiv255(ad, 255 - ubAlpha) + ubAlpha;

       ubAlpha >>= 1;
       rb1 += ( (rb2 - rb1) * ubAlpha ) >> 7;
       g1  += ( (g2 - g1) * ubAlpha ) >> 7;

       // recompose 
       return PixelFromParts( (eGML_PixelARGB32)rb1, (eC_UByte)g1 ) | (ad<<24);
    }

    static inline eGML_PixelARGB32 MixLinearSrcAlpha(const eGML_PixelARGB32 pxARGB1, const eGML_PixelARGB32 pxARGB2, eC_UByte ubAlpha)
    {
       // Cd = Cd*(1-Ag) + Cs*Ag
       // Ad = Ad*(1-As*Ag) + As*Ag
       //
       //  pxColor1=Cd/Ad
       //  pxColor2=Cs/As

       // color split
       eC_UInt rb1 = (eC_UInt) RedBlueFromPixel (pxARGB1);
       eC_UInt g1  = (eC_UInt) GreenFromPixel   (pxARGB1);
       eC_UInt rb2 = (eC_UInt) RedBlueFromPixel (pxARGB2);
       eC_UInt g2  = (eC_UInt) GreenFromPixel   (pxARGB2);
       
       // alpha split
       eC_UByte ad = AlphaFromPixel(pxARGB1);
       eC_UByte as = AlphaFromPixel(pxARGB2);

       eC_UByte asg = eGMLBlit_MulABDiv255(as, ubAlpha);

       // Ad = Ad*(1-As*Ag) + As*Ag
       ad = eGMLBlit_MulABDiv255(ad, 255 - asg) + asg;
       
       // for opaque destianation pixels
       if (ad == 255)
       {
           // we just add sourcecolor multiplied using it's alpha
           // Cd = Cd*(1-asg)+Cs*asg
           asg >>= 1;
           rb1 += ( (rb2 - rb1) * asg ) >> 7;
           g1  += ( (g2 - g1) * asg ) >> 7;
       }
       else
       {
           // TODO: For all pixels ad != 255 and ad != 0 may not be correct
           ubAlpha >>= 1;
           rb1 += ( (rb2 - rb1) * ubAlpha ) >> 7;
           g1  += ( (g2 - g1) * ubAlpha ) >> 7;
       }
       // recompose 
       return PixelFromParts( (eGML_PixelARGB32)rb1, (eC_UByte)g1 ) | (ad<<24);
    }

	//
	static inline eGML_PixelARGB32 MixBilinear(eGML_PixelARGB32 pxPixel1, eGML_PixelARGB32 pxPixel2, eGML_PixelARGB32 pxPixel3, eGML_PixelARGB32 pxPixel4, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
	{
		#ifdef eGML_BitmapARGB32_UseSWAR

		ubAlphaX >>= 1;
		ubAlphaY >>= 1;

		// mix 1 & 2
		eC_Int rb1 = (eC_UInt) RedBlueFromPixel(pxPixel1);
		eC_Int g1  = (eC_UInt) GreenFromPixel(pxPixel1);		
		eC_Short a1 = (eC_Short) AlphaFromPixel(pxPixel1);
		eC_Int rb2 = (eC_UInt) RedBlueFromPixel(pxPixel2);
		eC_Int g2  = (eC_UInt) GreenFromPixel(pxPixel2);
		eC_Short a2 = (eC_Short) AlphaFromPixel(pxPixel2);
		rb1 += ( (rb2 - rb1) * ubAlphaX ) >> 7;
		g1  += ( (g2 - g1) * ubAlphaX ) >> 7;
		a1  += ( (a2 - a1) * ubAlphaX ) >> 7;

		// mix 3 & 4
		eC_Int rb3 = (eC_UInt) RedBlueFromPixel(pxPixel3);
		eC_Int g3  = (eC_UInt) GreenFromPixel(pxPixel3);	
		eC_Short a3 = (eC_Short) AlphaFromPixel(pxPixel3);
		eC_Int rb4 = (eC_UInt) RedBlueFromPixel(pxPixel4);
		eC_Int g4  = (eC_UInt) GreenFromPixel(pxPixel4);		
		eC_Short a4 = (eC_Short) AlphaFromPixel(pxPixel4);
		rb3 += ( (rb4 - rb3) * ubAlphaX ) >> 7;
		g3  += ( (g4 - g3) * ubAlphaX ) >> 7;
		a3  += ( (a4 - a3) * ubAlphaX ) >> 7;
		
		// mix 1,2 & 3,4    
		rb1 = RedBlueFromPixel(rb1);
		rb3 = RedBlueFromPixel(rb3);

		rb1 += ( (rb3 - rb1) * ubAlphaY ) >> 7;
		g1  += ( (g3 - g1) * ubAlphaY ) >> 7;
		a1  += ( (a3 - a1) * ubAlphaY ) >> 7;
      
		// recompose 
		return ColorAndAlphaToPixel(PixelFromParts( (eGML_PixelARGB32)rb1, (eC_UByte)g1 ), (eC_UByte)a1);

		#else

    eC_UInt uiAlphaX = ((eC_UInt)ubAlphaX)+(ubAlphaX&1);
    eC_UInt uiAlphaY = ((eC_UInt)ubAlphaY)+(ubAlphaY&1);

		// mix 1 & 2
		eC_Int r1 = (eC_UInt) RedFromPixel(pxPixel1);
		eC_Int g1 = (eC_UInt) GreenFromPixel(pxPixel1);
		eC_Int b1 = (eC_UInt) BlueFromPixel(pxPixel1);
		eC_Int a1 = (eC_UInt) AlphaFromPixel(pxPixel1);
		eC_Int r2 = (eC_UInt) RedFromPixel(pxPixel2);
		eC_Int g2 = (eC_UInt) GreenFromPixel(pxPixel2);
		eC_Int b2 = (eC_UInt) BlueFromPixel(pxPixel2);
		eC_Int a2 = (eC_UInt) AlphaFromPixel(pxPixel2);
		r1 += ( (r2 - r1) * uiAlphaX ) >> 8;
		g1 += ( (g2 - g1) * uiAlphaX ) >> 8;
		b1 += ( (b2 - b1) * uiAlphaX ) >> 8;

		// mix 3 & 4
		eC_Int r3 = (eC_UInt) RedFromPixel(pxPixel3);
		eC_Int g3 = (eC_UInt) GreenFromPixel(pxPixel3);
		eC_Int b3 = (eC_UInt) BlueFromPixel(pxPixel3);
		eC_Int a3 = (eC_UInt) AlphaFromPixel(pxPixel3);
		eC_Int r4 = (eC_UInt) RedFromPixel(pxPixel4);
		eC_Int g4 = (eC_UInt) GreenFromPixel(pxPixel4);
		eC_Int b4 = (eC_UInt) BlueFromPixel(pxPixel4);
		eC_Int a4 = (eC_UInt) AlphaFromPixel(pxPixel4);

		r3 += ( (r4 - r3) * uiAlphaX ) >> 8;
		g3 += ( (g4 - g3) * uiAlphaX ) >> 8;
		b3 += ( (b4 - b3) * uiAlphaX ) >> 8;
		a3 += ( (a4 - a3) * uiAlphaX ) >> 8;
		
		// mix 1,2 & 3,4      
		r1 += ( (r3 - r1) * uiAlphaY ) >> 8;
		g1 += ( (g3 - g1) * uiAlphaY ) >> 8;
		b1 += ( (b3 - b1) * uiAlphaY ) >> 8;
		a1 += ( (a3 - a1) * uiAlphaY ) >> 8;
      
		// recompose 
		return ColorAndAlphaToPixel( PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 ), a1);
		#endif
	}
    
   static inline eGML_PixelARGB32 Blend4(eGML_Bitmap *pBitmap, eGML_OffsetARGB32 kOffset, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
   {
      eGML_OffsetARGB32 kOffset2 = kOffset;
      IncY(pBitmap, kOffset2);

      // fetch
      eGML_PixelARGB32 pxPixel1 = GetPixel(kOffset);
      eGML_PixelARGB32 pxPixel3 = GetPixel(kOffset2);
      IncX(pBitmap, kOffset);
      IncX(pBitmap, kOffset2);
      eGML_PixelARGB32 pxPixel2 = GetPixel(kOffset);
      eGML_PixelARGB32 pxPixel4 = GetPixel(kOffset2);

      // mix colors
      return MixBilinear(pxPixel1, pxPixel2, pxPixel3, pxPixel4, ubAlphaX, ubAlphaY);
   }

   static inline eC_Bool ColorKeyTest4(eGML_Bitmap *pBitmap, 
                                       eGML_OffsetARGB32 kOffset, 
                                       const eGML_Color cColorKey,
                                       eGML_PixelARGB32 &pxPixel1,
                                       eGML_PixelARGB32 &pxPixel2,
                                       eGML_PixelARGB32 &pxPixel3,
                                       eGML_PixelARGB32 &pxPixel4
                                       ) 
   {
      eGML_OffsetARGB32 kOffset2 = kOffset;
      IncY(pBitmap, kOffset2);

      pxPixel1 = GetPixel(kOffset);
      if(PixelToColor(pxPixel1) != cColorKey)
      {
         pxPixel3 = GetPixel(kOffset2);
         if(PixelToColor(pxPixel3) != cColorKey) 
         {
            IncX(pBitmap, kOffset);
            pxPixel2 = GetPixel(kOffset);
            if(PixelToColor(pxPixel2) != cColorKey)
            {
               IncX(pBitmap, kOffset2);
               pxPixel4 = GetPixel(kOffset);
               return (PixelToColor(pxPixel4) != cColorKey);
            }
         }
      }
      return false;
   }

};

//---------------------------------------------------------------	
#endif
