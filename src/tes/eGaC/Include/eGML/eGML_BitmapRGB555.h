/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapRGB555.h
* %version:        134.1 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:16:20 2006
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BITMAPRGB555__H_
#define EGML_BITMAPRGB555__H_
//---------------------------------------------------------------	

#include "eGML_Bitmap.h"
#include "eGML_BitmapA8.h"

//---------------------------------------------------------------	
// pixel type
typedef eC_UShort			eGML_PixelRGB555;

#ifdef eGML_DEBUGPIXELACCESS

struct eGML_OffsetRGB555
{
  eC_Int           iX;
  eC_Int           iY;
  eC_Int           iMinX;
  eC_Int           iMinY;
  eC_Int           iMaxX;
  eC_Int           iMaxY;
  eC_Int           iWidth;
  eC_Int           iHeight;
  eGML_PixelRGB555 *pD;  
};

#else

typedef eGML_PixelRGB555 *	eGML_OffsetRGB555;

#endif


//#define eGML_BitmapPocket555_UseSWAR		// set to enable SWAR optimization techniques (SIMD Within A Register)

//---------------------------------------------------------------	

class eGML_BitmapRGB555 : public eGML_Bitmap
{
	public:
	virtual void *Lock();
	
   virtual void Unlock(eC_UInt uiX1=0, eC_UInt uiY1=0, eC_UInt uiX2=0, eC_UInt uiY2=0);

   virtual void Blit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0);
   virtual void BlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255);

   virtual void BlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);

   virtual void Blit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);

   virtual void StretchBlit(eGML_Bitmap *pkSource, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
   virtual void StretchBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255);

   virtual void StretchBlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);
   virtual void StretchBlit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);

   virtual void RotBlit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1), eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0);
   virtual void RotBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1), eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255);

   virtual void RotBlitColor(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
   virtual void RotBlit2Color(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);

   virtual eGML_Color ColorFromRGB( eC_UByte ubR, eC_UByte ubG, eC_UByte ubB ) const;
   virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;

   virtual void DrawLine( const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth=0 );

   virtual void DrawPolygon( const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags );

   virtual void DrawDot( eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0 );

   virtual void Clear(eGML_Color clColor);

   virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_ColorARGB32 clColor, eC_Flag ulFlags = 0);

protected:
  //
  eGML_BitmapRGB555(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
  ///
  virtual ~eGML_BitmapRGB555();	


	private:
		friend class eGML_PixelRGB555_Access;
		friend class eGML_Screen;

   //
		virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor );

		//
		virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor );		
};


//---------------------------------------------------------------	
// lowlevel access methods for this kind of bitmap
//

class eGML_PixelRGB555_Access
{
public :
  
	static inline eGML_PixelRGB555 ColorToPixel( eGML_Color kColor )
    {
      return (eGML_PixelRGB555) kColor;
    }
	
   static inline eGML_PixelRGB555 ColorAndAlphaToPixel( eGML_Color kColor, eC_UByte /*ubAlpha*/ )
    {
      return ColorToPixel(kColor);
    }
    
    static inline eGML_Color PixelToColor( eGML_PixelRGB555 pxPixel )
    {
      return (eGML_Color) pxPixel;
    }

    static inline eGML_Color PixelToColorAndAlpha( eGML_PixelRGB555 pxPixel, eC_UByte & rubAlpha )
    {
      rubAlpha = 255; //OPAQUE;
      return PixelToColor(pxPixel);
    }
    
    static inline eGML_PixelRGB555 PixelFromRGB( eC_UByte ubR, eC_UByte ubG, eC_UByte ubB )
    {
      return (eGML_PixelRGB555)( ((((eC_UInt)ubR) << 7) & 0x7C00) | ((((eC_UInt)ubG) << 2) & 0x03E0) | (((eC_UInt)ubB) >> 3) );
    }
	
    static inline eGML_PixelRGB555 PixelFromColorARGB32( eGML_ColorARGB32 colARGB32 )
    {
      return (eGML_PixelRGB555)( ((((eC_UInt)colARGB32) >> 9) & 0x7C00) | ((((eC_UInt)colARGB32) >> 5) & 0x03E0) | ((((eC_UInt)colARGB32) >> 3) & 0x001F) );
    }

   static inline eGML_ColorARGB32 ColorARGB32FromPixel( eGML_PixelRGB555 pxPixel )
    {
      eC_UByte ubR = RedFromPixel(pxPixel);
      eC_UByte ubG = GreenFromPixel(pxPixel);
      eC_UByte ubB = BlueFromPixel(pxPixel);
      return (eGML_ColorARGB32)( 0xFF000000 | (ubR << 16) | (ubG << 8) | (ubB) );
    }

    static inline eC_UByte RedFromPixel( eGML_PixelRGB555 pxPixel )
    {
      return (eC_UByte) ((pxPixel & 0x7C00) >> 7);
    }
    
    static inline eC_UByte GreenFromPixel( eGML_PixelRGB555 pxPixel )
    {
      return (eC_UByte) ((pxPixel & 0x03E0) >> 2);
    }
    
    static inline eC_UByte BlueFromPixel( eGML_PixelRGB555 pxPixel )
    {
      return (eC_UByte) ((pxPixel & 0x001f) << 3);
    }	

	static inline eGML_PixelRGB555 RedBlueFromPixel(eGML_PixelRGB555 pxPixel)
	{
		return (pxPixel & 0x7c1f);
	}	

	static inline eGML_PixelRGB555 PixelFromParts(eGML_PixelRGB555 pxRB, eC_UByte ubG)
	{
		return  (eGML_PixelRGB555) ((pxRB & 0x7c1f) | ((((eC_UInt)ubG) << 2) & 0x03E0));
	}

	static inline eC_UByte AlphaFromPixel(eGML_PixelRGB555 pxPixel)
	{
		return 255;
	}	



#ifdef eGML_DEBUGPIXELACCESS
    static inline void InitOffset(eGML_BitmapRGB555 *pkBitmap, eC_Int x, eC_Int y, eGML_OffsetRGB555 &res)
    {
      res.iX     = x;
      res.iY     = y;
      res.iMinX  = pkBitmap->GetClipMinX();
      res.iMinY  = pkBitmap->GetClipMinY();
      res.iMaxX  = pkBitmap->GetClipMaxX();
      res.iMaxY  = pkBitmap->GetClipMaxY();
      res.iWidth = pkBitmap->GetWidth()-1;
      res.iHeight= pkBitmap->GetHeight()-1;
      res.pD    = (eGML_PixelRGB555 *)( ((eGML_Pixel*)pkBitmap->m_pkData) + (y * pkBitmap->m_uiPitch) + (x * 4) );
    }

    static inline bool CheckOffset( eGML_OffsetRGB555 &res , bool cliprange=true)
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

    static inline void GetOffset(eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB555 &res)
    {
      eGML_BitmapRGB555 *pInternalBitmap = (eGML_BitmapRGB555 *) pBitmap;
      InitOffset(pInternalBitmap, (eC_Int)uiX, (eC_Int)uiY, res);      
    }
    
    //
    static inline void AddOffset(eGML_Bitmap *pBitmap, eGML_OffsetRGB555 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB555 &res)
    {
      eGML_BitmapRGB555 *pInternalBitmap = (eGML_BitmapRGB555 *) pBitmap;
      InitOffset(pInternalBitmap, ((eC_Int)uiX)+res.iX, ((eC_Int)uiY)+res.iY, res);
    }
    
    //
    static inline void IncX(eGML_Bitmap *pBitmap, eGML_OffsetRGB555 &kOffset)
    {
      kOffset.pD++;
      kOffset.iX++;
    }
    
    //
    static inline void IncY(eGML_Bitmap *pBitmap, eGML_OffsetRGB555 &kOffset)
    {
      kOffset.pD= (eGML_PixelRGB555 *) (((eGML_Pixel*)kOffset.pD) + ((eGML_BitmapRGB555 *)pBitmap)->m_uiPitch);
      kOffset.iY++;
    }
    
    //
    static inline eGML_PixelRGB555 GetPixel(eGML_OffsetRGB555 kOffset)
    {
      if (CheckOffset(kOffset, false))
      {
        return *kOffset.pD;
      }
      
      return 0;
    }

    //
    static inline void PutPixel(eGML_OffsetRGB555 kOffset, eGML_PixelRGB555 pxColor)
    {
      if (CheckOffset(kOffset))
      { 
        *kOffset.pD = pxColor;
      }      
    }
    
    //
    static inline void PutSpan(eGML_OffsetRGB555 &kOffset, eGML_PixelRGB555 pxColor, eC_Int iCount)
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
    static inline void TextureOffset(const eGML_OffsetRGB555 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetRGB555 &res)
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


    static inline void GetOffset( eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB555 &res )
    {
      eGML_BitmapRGB555 *pInternalBitmap = (eGML_BitmapRGB555 *) pBitmap;
      res = (eGML_OffsetRGB555)( ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 2) );
    }
    
    static inline void TextureOffset( const eGML_OffsetRGB555 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetRGB555 &res )
    {
      res = base + (uiY << uiN) + uiX;
    }
    
    static inline void AddOffset( eGML_Bitmap *pBitmap, eGML_OffsetRGB555 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB555 &res )
    {
      eGML_BitmapRGB555 *pInternalBitmap = (eGML_BitmapRGB555 *) pBitmap;
      res = (eGML_OffsetRGB555)( ((eGML_Pixel*)kOffset) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 2) );
    }
    
    static inline void IncX( eGML_Bitmap *pBitmap, eGML_OffsetRGB555 &kOffset )
    {
      kOffset++;
    }
    
    static inline void IncY( eGML_Bitmap *pBitmap, eGML_OffsetRGB555 &kOffset )
    {
      kOffset = (eGML_OffsetRGB555) (((eGML_Pixel*)kOffset) + ((eGML_BitmapRGB555 *)pBitmap)->m_uiPitch);
    }
    
	static inline eGML_PixelRGB555 GetRawPixel(eGML_OffsetRGB555 kOffset)
    {
      return *kOffset;
    }

    static inline eGML_PixelRGB555 GetPixel( eGML_OffsetRGB555 kOffset )
    {
      return *kOffset;
    }

    static inline void PutPixel( eGML_OffsetRGB555 kOffset, eGML_PixelRGB555 pxColor )
    {
      *kOffset = pxColor;
    }
    
    static inline void PutSpan( eGML_OffsetRGB555 &kOffset, eGML_PixelRGB555 pxColor, eC_Int iCount )
    {
      while (iCount-- > 0) *kOffset++ = pxColor;
    }
#endif
    
	static inline eGML_PixelRGB555 MixAverage(eGML_PixelRGB555 pxColor1, eGML_PixelRGB555 pxColor2)
	{
		#ifdef eGML_BitmapRGB555_UseSWAR
		// build avg of all components at once
		return (pxColor1 & pxColor2) + ((pxColor1 ^ pxColor2) >> 1);
		#else
		// TODO
		return (pxColor1 & pxColor2) + ((pxColor1 ^ pxColor2) >> 1);
		#endif
	}

	static inline eGML_PixelRGB555 MixAdditive(eGML_PixelRGB555 pxColor1, eGML_PixelRGB555 pxColor2)
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

	static inline eGML_PixelRGB555 MixLinear(eGML_PixelRGB555 pxColor1, eGML_PixelRGB555 pxColor2, eC_UByte ubAlpha)
	{
		#ifdef eGML_BitmapRGB555_UseSWAR

		// color splir
		eC_Int rb1 = (eC_UInt) RedBlueFromPixel(pxColor1);
		eC_Int g1  = (eC_UInt) GreenFromPixel(pxColor1);
		eC_Int rb2 = (eC_UInt) RedBlueFromPixel(pxColor2);
		eC_Int g2  = (eC_UInt) GreenFromPixel(pxColor2);

		// blend
		ubAlpha >>= 1;
		rb1 += (eC_UInt) (eGML_PixelRGB555) (( (rb2 - rb1) * (ubAlpha>>1) ) >> 6);
		g1  += ( (g2 - g1) * ubAlpha ) >> 7;

		// recompose 
		return PixelFromParts( (eGML_PixelRGB555)rb1, (eC_UByte)g1 );

		#else

		// color split 
		eC_Int r1 = (eC_UInt) RedFromPixel(pxColor1);
		eC_Int g1 = (eC_UInt) GreenFromPixel(pxColor1);
		eC_Int b1 = (eC_UInt) BlueFromPixel(pxColor1);
		eC_Int r2 = (eC_UInt) RedFromPixel(pxColor2);
		eC_Int g2 = (eC_UInt) GreenFromPixel(pxColor2);
		eC_Int b2 = (eC_UInt) BlueFromPixel(pxColor2);

		// blend
		r1 += ( (r2 - r1) * ubAlpha ) >> 8;
		g1 += ( (g2 - g1) * ubAlpha ) >> 8;
		b1 += ( (b2 - b1) * ubAlpha ) >> 8;

		// recompose 
		return PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 );
		#endif
	}

	static inline eGML_PixelRGB555 MixBilinear(eGML_PixelRGB555 pxPixel1, eGML_PixelRGB555 pxPixel2, eGML_PixelRGB555 pxPixel3, eGML_PixelRGB555 pxPixel4, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
	{
		#ifdef eGML_BitmapRGB555_UseSWAR

		ubAlphaX >>= 1;
		ubAlphaY >>= 1;

		// mix 1 & 2
		eC_Int rb1 = (eC_UInt) RedBlueFromPixel(pxPixel1);
		eC_Int g1  = (eC_UInt) GreenFromPixel(pxPixel1);		
		eC_Int rb2 = (eC_UInt) RedBlueFromPixel(pxPixel2);
		eC_Int g2  = (eC_UInt) GreenFromPixel(pxPixel2);
		rb1 += (eC_UInt) (eGML_PixelRGB555)(( (rb2 - rb1) * ubAlphaX ) >> 7);
		g1  += ( (g2 - g1) * ubAlphaX ) >> 7;

		// mix 3 & 4
		eC_Int rb3 = (eC_UInt) RedBlueFromPixel(pxPixel3);
		eC_Int g3  = (eC_UInt) GreenFromPixel(pxPixel3);	
		eC_Int rb4 = (eC_UInt) RedBlueFromPixel(pxPixel4);
		eC_Int g4  = (eC_UInt) GreenFromPixel(pxPixel4);		
		rb3 += (eC_UInt) (eGML_PixelRGB555)(( (rb4 - rb3) * ubAlphaX ) >> 7);
		g3  += ( (g4 - g3) * ubAlphaX ) >> 7;
		
		// mix 1,2 & 3,4    
		rb1 = (eC_UInt) RedBlueFromPixel((eGML_PixelRGB555)rb1);
		rb3 = (eC_UInt) RedBlueFromPixel((eGML_PixelRGB555)rb3);

		rb1 += (eC_UInt) (eGML_PixelRGB555)(( (rb3 - rb1) * ubAlphaY ) >> 7);
		g1  += ( (g3 - g1) * ubAlphaY ) >> 7;
	    
		// recompose 
		return PixelFromParts( (eGML_PixelRGB555)rb1, (eC_UByte)g1 );

		#else

		// mix 1 & 2
		eC_Int r1 = (eC_UInt) RedFromPixel(pxPixel1);
		eC_Int g1 = (eC_UInt) GreenFromPixel(pxPixel1);
		eC_Int b1 = (eC_UInt) BlueFromPixel(pxPixel1);
		eC_Int r2 = (eC_UInt) RedFromPixel(pxPixel2);
		eC_Int g2 = (eC_UInt) GreenFromPixel(pxPixel2);
		eC_Int b2 = (eC_UInt) BlueFromPixel(pxPixel2);
		r1 += ( (r2 - r1) * ubAlphaX ) >> 8;
		g1 += ( (g2 - g1) * ubAlphaX ) >> 8;
		b1 += ( (b2 - b1) * ubAlphaX ) >> 8;

		// mix 3 & 4
		eC_Int r3 = (eC_UInt) RedFromPixel(pxPixel3);
		eC_Int g3 = (eC_UInt) GreenFromPixel(pxPixel3);
		eC_Int b3 = (eC_UInt) BlueFromPixel(pxPixel3);
		eC_Int r4 = (eC_UInt) RedFromPixel(pxPixel4);
		eC_Int g4 = (eC_UInt) GreenFromPixel(pxPixel4);
		eC_Int b4 = (eC_UInt) BlueFromPixel(pxPixel4);
		r3 += ( (r4 - r3) * ubAlphaX ) >> 8;
		g3 += ( (g4 - g3) * ubAlphaX ) >> 8;
		b3 += ( (b4 - b3) * ubAlphaX ) >> 8;
		
		// mix 1,2 & 3,4      
		r1 += ( (r3 - r1) * ubAlphaY ) >> 8;
		g1 += ( (g3 - g1) * ubAlphaY ) >> 8;
		b1 += ( (b3 - b1) * ubAlphaY ) >> 8;
	    
		// recompose 
		return PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 );
		#endif
	}
    
	static inline void BlendPixel(eGML_OffsetRGB555 kOffset, eGML_PixelRGB555 pxColor, eC_UByte ubAlpha)
	{      
		eGML_PixelRGB555 pxPixel = MixLinear( GetPixel(kOffset), pxColor, ubAlpha );
		PutPixel( kOffset, pxPixel );
	}   

   static inline void BlendPixelSrcAlpha(eGML_OffsetRGB555 kOffset, eGML_PixelRGB555 pxARGB, eC_UByte ubAlpha)
   {
      BlendPixel(kOffset, pxARGB, ubAlpha);
   }   

	static inline void AddPixel(eGML_OffsetRGB555 kOffset, eGML_PixelRGB555 pxColor)
	{
      	eGML_PixelRGB555 pxPixel = MixAdditive( GetPixel(kOffset), pxColor );
		PutPixel( kOffset, pxPixel );
	}


    static inline eGML_PixelRGB555 Blend4(  eGML_Bitmap *pBitmap, eGML_OffsetRGB555 kOffset, 
      eC_UByte ubAlphaX, eC_UByte ubAlphaY )
    {
  		eGML_OffsetRGB555 kOffset2 = kOffset;
		IncY(pBitmap, kOffset2);

		// fetch
		eGML_PixelRGB555 pxPixel1 = GetPixel(kOffset);
		eGML_PixelRGB555 pxPixel3 = GetPixel(kOffset2);
		IncX(pBitmap, kOffset);
		IncX(pBitmap, kOffset2);
		eGML_PixelRGB555 pxPixel2 = GetPixel(kOffset);
		eGML_PixelRGB555 pxPixel4 = GetPixel(kOffset2);
		
		// mix colors
		return MixBilinear(pxPixel1, pxPixel2, pxPixel3, pxPixel4, ubAlphaX, ubAlphaY);
    }

   static inline eC_Bool ColorKeyTest4(eGML_Bitmap *pBitmap, 
                                       eGML_OffsetRGB555 kOffset, 
                                       const eGML_Color cColorKey,
                                       eGML_PixelRGB555 &pxPixel1,
                                       eGML_PixelRGB555 &pxPixel2,
                                       eGML_PixelRGB555 &pxPixel3,
                                       eGML_PixelRGB555 &pxPixel4
                                       ) 
   {
      eGML_OffsetRGB555 kOffset2 = kOffset;
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
