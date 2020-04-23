/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapA1.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Wed May 03 08:48:58 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BITMAPA1__H_
#define EGML_BITMAPA1__H_

//---------------------------------------------------------------	
#include "eGML_Bitmap.h"
#include "eGML_Screen.h"
#include "eGML_FontDecoder.h"

#ifdef eGML_USEBITMAPA1

//---------------------------------------------------------------	
// pixel type
typedef eC_UByte        eGML_PixelA1;
typedef 
struct
{
  eC_UByte *pixelbyte;
  eC_UByte  pixelmask; // current bit 000b0000
}eGML_OffsetA1;


//---------------------------------------------------------------	
/* !!! INTERNAL USE ONLY !!!
Bitmap using 8bit ALPHA only layout. */
class eGML_BitmapA1 : public eGML_Bitmap
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

    //
    virtual void DrawLine( const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth=0 );
    
    //
    virtual void DrawPolygon( const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags );
    
    //
    virtual void DrawDot( eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0 );

  protected:
    //
    eGML_BitmapA1(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
    //
    virtual ~eGML_BitmapA1();
    

  private:
    
    friend class eGML_PixelA1_Access;
    friend class eGML_Screen;
    
    
    //
    virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor );
    
    //
    virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor );		
};

//---------------------------------------------------------------	
// lowlevel access methods for this kind of bitmap
// on reading Alpha is replicated to R, G and B
// on writing input RGB is reduced to B before assigning to Alpha

/* !!! INTERNAL USE ONLY !!!
Access functions for eGML_BitmapA1.
See eGML_Pixel_Access, eGML_BitmapA1 for general info */
class eGML_PixelA1_Access
{
  public :
    
    //
    static inline eGML_PixelA1 ColorToPixel(eGML_Color kColor)
    {
        return (eGML_PixelA1) ( kColor ? 1 : 0 );
    }

    //
    static inline eGML_PixelA1 ColorAndAlphaToPixel(eGML_Color kColor, eC_UByte /*ubAlpha*/)
    {
        // use only kColor 
        return ColorToPixel(kColor);
    }
    
    //
    static inline eGML_Color PixelToColor(eGML_PixelA1 pxPixel)
    {
        return (eGML_Color) ( pxPixel ? 0xFFFFFF : 0x000000);
    }

    //
    static inline eGML_Color PixelToColorAndAlpha(eGML_PixelA1 pxPixel, eC_UByte & rubAlpha)
    {
        rubAlpha = 255;
        return PixelToColor(pxPixel);
    }
    
	//
    static inline eGML_PixelA1 PixelFromRGB(eC_UByte ubR, eC_UByte ubG, eC_UByte ubB)
    {
        return (eGML_PixelA1) (((((((eC_UInt)ubR) + ((eC_UInt)ubG)*2 + ((eC_UInt)ubB)) >> 2) > 0x7f)) ? 1 : 0);
    }
	
    static inline eGML_PixelA1 PixelFromColorARGB32( eGML_PixelA1 colARGB32 )
    {
        return (eGML_PixelA1) ( colARGB32 ? 0xFFFFFFFF : 0x00000000);
    }

    static inline eGML_PixelA1 PixelFromColorARGB32(eGML_ColorARGB32 colARGB32)
    {
        return (eGML_PixelA1)(colARGB32 ? 0xFFFFFFFF : 0x00000000);
    }

   static inline eGML_ColorARGB32 ColorARGB32FromPixel(eGML_PixelA1 pxPixel )
    {
      eC_UByte ubR = RedFromPixel(pxPixel);
      eC_UByte ubG = GreenFromPixel(pxPixel);
      eC_UByte ubB = BlueFromPixel(pxPixel);
      return (eGML_ColorARGB32)( 0xFF000000 | (ubR << 16) | (ubG << 8) | (ubB) );
    }

    //
    static inline eC_UByte RedFromPixel(eGML_PixelA1 pxPixel)
    {
        return (eC_UByte) ( pxPixel ? 0xFF : 0x00 );
    }
    
    //
    static inline eC_UByte GreenFromPixel(eGML_PixelA1 pxPixel)
    {
      return (eC_UByte) ( pxPixel ? 0xFF : 0x00 );
    }
    
    //
    static inline eC_UByte BlueFromPixel(eGML_PixelA1 pxPixel)
    {
      return (eC_UByte) ( pxPixel ? 0xFF : 0x00 );
    }	

    static inline eC_UByte AlphaFromPixel(eGML_PixelA1 pxPixel)
    {
	  return (eC_UByte) ( pxPixel ? 0xFF : 0x00 );
    }
    //
    static inline void GetOffset(eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetA1 &res)
    {
      eGML_BitmapA1 *pInternalBitmap = (eGML_BitmapA1 *) pBitmap;
      res.pixelbyte = ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiY * pInternalBitmap->m_uiPitch) + (uiX >> 3);
      res.pixelmask = 1 << (7-(uiX & 7));
    }
    
    //
    static inline void TextureOffset(const eGML_OffsetA1 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetA1 &res)
    {
      res.pixelbyte = 0;// TODO base + (uiY << uiN) + uiX;
    }
    
    //
    static inline void AddOffset(eGML_Bitmap *pBitmap, eGML_OffsetA1 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetA1 &res)
    {
      eGML_BitmapA1 *pInternalBitmap = (eGML_BitmapA1 *) pBitmap;
      res.pixelbyte = ((eGML_Pixel*)kOffset.pixelbyte) + (uiY * pInternalBitmap->m_uiPitch) + (uiX >> 3);
      res.pixelmask >>= (uiX & 7);
      if (kOffset.pixelmask == 0)
      {
          kOffset.pixelmask = 0x80;
          kOffset.pixelbyte++;
      }
    }
    
    //
    static inline void IncX(eGML_Bitmap *pBitmap, eGML_OffsetA1 &kOffset)
    {
      // shift right to next pixel
      kOffset.pixelmask >>= 1;
      if (kOffset.pixelmask == 0)
      {
          kOffset.pixelmask = 0x80;
          kOffset.pixelbyte++;
      }
    }
    
    //
    static inline void IncY(eGML_Bitmap *pBitmap, eGML_OffsetA1 &kOffset)
    {
      // pitch is lenght of scanline in bytes!!
      kOffset.pixelbyte += ((eGML_BitmapA1 *)pBitmap)->m_uiPitch;
    }

	////
    static inline eGML_PixelA1 GetPixel(eGML_OffsetA1 kOffset)
    {
        return ((*kOffset.pixelbyte & kOffset.pixelmask) > 0 )? 1 : 0;
    }

    static inline eGML_PixelA1 GetRawPixel(eGML_OffsetA1 kOffset)
    {
      return ((*kOffset.pixelbyte & kOffset.pixelmask) > 0 )? 1 : 0;
    }
    
    //
    static inline void PutPixel(eGML_OffsetA1 kOffset, eGML_PixelA1 pxColor)
    {
      if (pxColor == 0)
      {
        *kOffset.pixelbyte &= ~kOffset.pixelmask;
      }
      else
      {
        *kOffset.pixelbyte |= kOffset.pixelmask;
      }
    }
    
    //
    static inline void PutSpan(eGML_OffsetA1 &kOffset, eGML_PixelA1 pxColor, eC_Int iCount)
    {
        while (iCount-- > 0) 
        {
            PutPixel(kOffset, pxColor);
            IncX(0,kOffset);	
        }
    }
   
	//
	static inline eGML_PixelA1 MixAverage(eGML_PixelA1 pxColor1, eGML_PixelA1 pxColor2)
	{
        // if at least one bit is set, return both
		return (eGML_PixelA1) pxColor1 | pxColor2;
	}

    //
    static inline eGML_PixelA1 MixAdditive(eGML_PixelA1 pxColor1, eGML_PixelA1 pxColor2)
    {
        // if at least one bit is set, return both
		return (eGML_PixelA1) pxColor1 | pxColor2;
    }

	//
    static inline eGML_PixelA1 MixLinear(eGML_PixelA1 pxColor1, eGML_PixelA1 pxColor2, eC_UByte ubAlpha)
	{
        // if at least one bit is set, return both
		return (eGML_PixelA1) pxColor1 | pxColor2;
    }

	//
	static inline eGML_PixelA1 MixBilinear(eGML_PixelA1 pxPixel1, eGML_PixelA1 pxPixel2, eGML_PixelA1 pxPixel3, eGML_PixelA1 pxPixel4, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
	{
        // if more than two are set, result is set
        return (eGML_PixelA1) (( pxPixel1 +  pxPixel2 + pxPixel3 + pxPixel4) > 2) ? 1:0;
	}

    //
    static inline void BlendPixel(eGML_OffsetA1 kOffset, eGML_PixelA1 pxColor, eC_UByte ubAlpha)
    {
      //eC_Int p1 = (eC_Int) *kOffset;
      //eC_Int p2 = (eC_Int) pxColor;
      //
      //// blend
      //*kOffset = (eC_UByte) (p1 + ( ((p2 - p1) * ubAlpha ) >> 8));
    }

    static inline void BlendPixelSrcAlpha(eGML_OffsetA1 kOffset, eGML_PixelA1 pxARGB, eC_UByte ubAlpha)
    {
       BlendPixel(kOffset, pxARGB, ubAlpha);
    }   

    // additive (saturation) blending putpixel
    static inline void AddPixel(eGML_OffsetA1 kOffset, eGML_PixelA1 pxColor)
    {
		eGML_PixelA1 pxPixel = MixAdditive( GetPixel(kOffset), pxColor );
		PutPixel( kOffset, pxPixel );
    }        
    
    //
    static inline eGML_PixelA1 Blend4(eGML_Bitmap *pBitmap, eGML_OffsetA1 kOffset, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
    {
      //eC_Int c1 = (eC_Int)kOffset[0];
      //eC_Int c2 = (eC_Int)kOffset[1];
      //eC_Int c3 = (eC_Int)kOffset[((eGML_BitmapA1 *)pBitmap)->m_uiPitch];
      //eC_Int c4 = (eC_Int)kOffset[((eGML_BitmapA1 *)pBitmap)->m_uiPitch+1];
      //
      //c1+= (((c2-c1) * ubAlphaX)>>8);
      //c3+= (((c4-c3) * ubAlphaX)>>8);
      //
      //c1+= (((c3-c1) * ubAlphaY)>>8);
      
      return (eGML_PixelA1) 1; //c1;
    }

	static inline eC_UInt Blend4_2Col(eGML_Bitmap *pBitmap, eGML_OffsetA1 kOffset, eC_UByte ubAlphaX, eC_UByte ubAlphaY, eGML_Color kCol1, eGML_Color kCol2, eC_Flag & regionCode)
    {	
		
		//eC_UInt pxPixel1 = eGML_FontDecoder::decodeColorFast( kOffset[0], kCol1, kCol2);
		//eC_UInt pxPixel2 = eGML_FontDecoder::decodeColorFast( kOffset[1], kCol1, kCol2);
		//eC_UInt pxPixel3 = eGML_FontDecoder::decodeColorFast( kOffset[((eGML_BitmapA1 *)pBitmap)->m_uiPitch] , kCol1, kCol2);
		//eC_UInt pxPixel4 = eGML_FontDecoder::decodeColorFast( kOffset[((eGML_BitmapA1 *)pBitmap)->m_uiPitch+1], kCol1, kCol2);
		//
		//eC_UInt uiAlphaX = ((eC_UInt)ubAlphaX);
		//eC_UInt uiAlphaY = ((eC_UInt)ubAlphaY);

		//// mix 1 & 2
		//eC_Int a1 = (eC_UInt) A_(pxPixel1);
		//eC_Int a2 = (eC_UInt) A_(pxPixel2);
		//eC_Int r1 = (eC_UInt) R_(pxPixel1);
		//eC_Int g1 = (eC_UInt) G_(pxPixel1);
		//eC_Int b1 = (eC_UInt) B_(pxPixel1);
		//eC_Int r2 = (eC_UInt) R_(pxPixel2);
		//eC_Int g2 = (eC_UInt) G_(pxPixel2);
		//eC_Int b2 = (eC_UInt) B_(pxPixel2);

		//a1 += ( (a2 - a1) * uiAlphaX ) >> 8;
		//r1 += ( (r2 - r1) * uiAlphaX ) >> 8;
		//g1 += ( (g2 - g1) * uiAlphaX ) >> 8;
		//b1 += ( (b2 - b1) * uiAlphaX ) >> 8;

		//// mix 3 & 4
		//eC_Int a3 = (eC_UInt) A_(pxPixel3);
		//eC_Int a4 = (eC_UInt) A_(pxPixel4);
		//eC_Int r3 = (eC_UInt) R_(pxPixel3);
		//eC_Int g3 = (eC_UInt) G_(pxPixel3);
		//eC_Int b3 = (eC_UInt) B_(pxPixel3);
		//eC_Int r4 = (eC_UInt) R_(pxPixel4);
		//eC_Int g4 = (eC_UInt) G_(pxPixel4);
		//eC_Int b4 = (eC_UInt) B_(pxPixel4);

		//a3 += ( (a4 - a3) * uiAlphaX ) >> 8;
		//r3 += ( (r4 - r3) * uiAlphaX ) >> 8;
		//g3 += ( (g4 - g3) * uiAlphaX ) >> 8;
		//b3 += ( (b4 - b3) * uiAlphaX ) >> 8;
		//
		//// mix 1,2 & 3,4      
		//a1 += ( (a3 - a1) * uiAlphaY ) >> 8;
		//r1 += ( (r3 - r1) * uiAlphaY ) >> 8;
		//g1 += ( (g3 - g1) * uiAlphaY ) >> 8;
		//b1 += ( (b3 - b1) * uiAlphaY ) >> 8;

		return 0;//(a1 << 24)&0xFF000000 |(r1 << 16)&0x00FF0000 |(g1 << 8)&0x0000FF00 | b1&0x000000FF  ;
    }

   static inline eC_Bool ColorKeyTest4(eGML_Bitmap *pBitmap, 
                                       eGML_OffsetA1 kOffset, 
                                       const eGML_Color cColorKey,
                                       eGML_PixelA1 &pxPixel1,
                                       eGML_PixelA1 &pxPixel2,
                                       eGML_PixelA1 &pxPixel3,
                                       eGML_PixelA1 &pxPixel4
                                       ) 
   {
      eGML_OffsetA1 kOffset2 = kOffset;
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
#endif // eGML_USEBITMAPA1
