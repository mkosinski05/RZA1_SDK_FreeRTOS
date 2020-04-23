/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapA5_2BitFlags.h
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


#ifndef EGML_BITMAPA5_2BIT_FLAGS__H_
#define EGML_BITMAPA5_2BIT_FLAGS__H_

//---------------------------------------------------------------	
#include "eGML_Bitmap.h"
#include "eGML_Screen.h"
#include "eGML_FontDecoder.h"

//---------------------------------------------------------------	
// pixel type
typedef eC_UByte		eGML_PixelA5_2;
typedef eGML_PixelA5_2 *	eGML_OffsetA5_2;

//---------------------------------------------------------------	
/* !!! INTERNAL USE ONLY !!!
Bitmap using 5bit ALPHA only layout. */
class eGML_BitmapA5_2 : public eGML_Bitmap
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
    virtual void BlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_Color cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);
	virtual void Blit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eGML_Color cCol1, eGML_Color cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);
    
    //
    virtual void StretchBlit(eGML_Bitmap *pkSource, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
    
    //
    virtual void StretchBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha=255);
    
    //
    virtual void StretchBlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_Color cCol, eC_Flag ulFlags, eC_UByte ubAlpha=255);
	virtual void StretchBlit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_Color cCol1, eGML_Color cCol2, eC_Flag ulFlags, eC_UByte ubAlpha=255);
    
    //
    virtual void RotBlit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255, eGML_Color cColorKey = 0);
    
    //
    virtual void RotBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
    
    //
    virtual void RotBlitColor(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_Color cCol, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
	virtual void RotBlit2Color(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_Color cCol1, eGML_Color cCol2, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX=eC_FromInt(1), eC_Value vStretchY=eC_FromInt(1), eC_Value vRotPosX=eC_FromFloat(0.5f), eC_Value vRotPosY=eC_FromFloat(0.5f), eC_UByte ubAlpha=255);
    
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
	  eGML_BitmapA5_2(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
    //
    virtual ~eGML_BitmapA5_2();
    

  private:
    
    friend class eGML_PixelA5_2_Access;
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
Access functions for eGML_BitmapA5_2.
See eGML_Pixel_Access, eGML_BitmapA5_2 for general info */
class eGML_PixelA5_2_Access
{
  public :
    
    //
    static inline eGML_PixelA5_2 ColorToPixel(eGML_Color kColor)
    {
      return (eGML_PixelA5_2) kColor;
    }

    //
    static inline eGML_PixelA5_2 ColorAndAlphaToPixel(eGML_Color kColor, eC_UByte /*ubAlpha*/)
    {
      // use only kColor 
      return ColorToPixel(kColor);
    }
    
    //
    static inline eGML_Color PixelToColor(eGML_PixelA5_2 pxPixel)
    {
      return (eGML_Color) pxPixel;
    }

    //
    static inline eGML_Color PixelToColorAndAlpha(eGML_PixelA5_2 pxPixel, eC_UByte & rubAlpha)
    {
      rubAlpha = 255;
      return PixelToColor(pxPixel);
    }
    
	//
    static inline eGML_PixelA5_2 PixelFromRGB(eC_UByte ubR, eC_UByte ubG, eC_UByte ubB)
    {
      return (eGML_PixelA5_2) ((((eC_UInt)ubR) + ((eC_UInt)ubG)*2 + ((eC_UInt)ubB)) >> 2);
    }

    static inline eGML_PixelA5_2 PixelFromColorARGB32(eGML_PixelA5_2 colARGB32 )
    {
        return (eGML_PixelA5_2) ( colARGB32 ? 0xFFFFFFFF : 0x00000000);
    }

   static inline eGML_ColorARGB32 ColorARGB32FromPixel(eGML_PixelA5_2 pxPixel )
    {
      eC_UByte ubR = RedFromPixel(pxPixel);
      eC_UByte ubG = GreenFromPixel(pxPixel);
      eC_UByte ubB = BlueFromPixel(pxPixel);
      return (eGML_ColorARGB32)( 0xFF000000 | (ubR << 16) | (ubG << 8) | (ubB) );
    }
    //
    static inline eC_UByte RedFromPixel(eGML_PixelA5_2 pxPixel)
    {
      return (eC_UByte) pxPixel;
    }
    
    //
    static inline eC_UByte GreenFromPixel(eGML_PixelA5_2 pxPixel)
    {
      return (eC_UByte) pxPixel;
    }
    
    //
    static inline eC_UByte BlueFromPixel(eGML_PixelA5_2 pxPixel)
    {
      return (eC_UByte) pxPixel;
	}

    static inline eC_UByte AlphaFromPixel(eGML_PixelA5_2 pxPixel)
    {
	  return (eC_UByte) pxPixel;
    }

    //
    static inline void GetOffset(eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetA5_2 &res)
    {
      eGML_BitmapA5_2 *pInternalBitmap = (eGML_BitmapA5_2 *) pBitmap;
      res = (eGML_OffsetA5_2)( ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiY * pInternalBitmap->m_uiPitch) + uiX );
    }
    
    //
    static inline void TextureOffset(const eGML_OffsetA5_2 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetA5_2 &res)
    {
      res = base + (uiY << uiN) + uiX;
    }
    
    //
    static inline void AddOffset(eGML_Bitmap *pBitmap, eGML_OffsetA5_2 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetA5_2 &res)
    {
      eGML_BitmapA5_2 *pInternalBitmap = (eGML_BitmapA5_2 *) pBitmap;
      res = (eGML_OffsetA5_2)( ((eGML_Pixel*)kOffset) + (uiY * pInternalBitmap->m_uiPitch) + uiX );
    }
    
    //
    static inline void IncX(eGML_Bitmap *pBitmap, eGML_OffsetA5_2 &kOffset)
    {
      kOffset++;
    }
    
    //
    static inline void IncY(eGML_Bitmap *pBitmap, eGML_OffsetA5_2 &kOffset)
    {
      kOffset = (eGML_OffsetA5_2) (((eGML_Pixel*)kOffset) + ((eGML_BitmapA5_2 *)pBitmap)->m_uiPitch);
    }


    static inline eGML_PixelA5_2 GetPixel(eGML_OffsetA5_2 kOffset)
   {
		eC_UByte u = *kOffset;		
		
		switch (u & FLAG_BIT_MASK){
			case BIT_BLEND_MASK:
				return (u | (( u & 224) >> 5));
			case INSIDE_BIT_MASK:
				return 255;
			default:
				return 0;
		}			
    }
    
	static inline eGML_PixelA5_2 GetRawPixel(eGML_OffsetA5_2 kOffset)
    {
      return *kOffset;
    }

    //
    static inline void PutPixel(eGML_OffsetA5_2 kOffset, eGML_PixelA5_2 pxColor)
    {
      *kOffset = pxColor;
    }
    
    //
    static inline void PutSpan(eGML_OffsetA5_2 &kOffset, eGML_PixelA5_2 pxColor, eC_Int iCount)
    {
      while (iCount-- >0) *kOffset++ = pxColor;
    }
   
	//
	static inline eGML_PixelA5_2 MixAverage(eGML_PixelA5_2 pxColor1, eGML_PixelA5_2 pxColor2)
	{
		return (eGML_PixelA5_2) ((((eC_UInt) pxColor1) + ((eC_UInt) pxColor2)) >> 1);
	}

    //
    static inline eGML_PixelA5_2 MixAdditive(eGML_PixelA5_2 pxColor1, eGML_PixelA5_2 pxColor2)
    {
		// color sum
		eC_UInt uiC = (eC_UInt) pxColor1 + (eC_UInt) pxColor2;
		// saturate (only lower 8bit) 
		uiC |= (-(eC_Int)(uiC & 0x100)) >> 8;	
		return (eGML_PixelA5_2) (eC_UByte)uiC;
    }

	//
    static inline eGML_PixelA5_2 MixLinear(eGML_PixelA5_2 pxColor1, eGML_PixelA5_2 pxColor2, eC_UByte ubAlpha)
	{
		eC_Int c1 = (eC_UInt) pxColor1;
		eC_Int c2 = (eC_UInt) pxColor2;
		c1 += ( (c2 - c1) * ubAlpha ) >> 8;
		return (eGML_PixelA5_2) c1;
    }

	//
	static inline eGML_PixelA5_2 MixBilinear(eGML_PixelA5_2 pxPixel1, eGML_PixelA5_2 pxPixel2, eGML_PixelA5_2 pxPixel3, eGML_PixelA5_2 pxPixel4, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
	{
		// mix 1 & 2
		eC_Int c1 = (eC_UInt) pxPixel1;
		eC_Int c2 = (eC_UInt) pxPixel2;		
		c1 += ( (c2 - c1) * ubAlphaX ) >> 8;
		// mix 3 & 4
		eC_Int c3 = (eC_UInt) pxPixel3;
		eC_Int c4 = (eC_UInt) pxPixel4;
		c3 += ( (c4 - c3) * ubAlphaX ) >> 8;			
		// mix 1,2 & 3,4      
		c1 += ( (c3 - c1) * ubAlphaY ) >> 8;	
      
		return (eGML_PixelA5_2) c1;
	}

    //
    static inline void BlendPixel(eGML_OffsetA5_2 kOffset, eGML_PixelA5_2 pxColor, eC_UByte ubAlpha)
    {
      eC_Int p1 = (eC_Int) *kOffset;
      eC_Int p2 = (eC_Int) pxColor;
      
      // blend
      *kOffset = (eC_UByte) (p1 + ( ((p2 - p1) * ubAlpha ) >> 8));
    }

    static inline void BlendPixelSrcAlpha(eGML_OffsetA5_2 kOffset, eGML_PixelA5_2 pxARGB, eC_UByte ubAlpha)
    {
       BlendPixel(kOffset, pxARGB, ubAlpha);
    }   

    // additive (saturation) blending putpixel
    static inline void AddPixel(eGML_OffsetA5_2 kOffset, eGML_PixelA5_2 pxColor)
    {
		eGML_PixelA5_2 pxPixel = MixAdditive( GetPixel(kOffset), pxColor );
		PutPixel( kOffset, pxPixel );
    }        
    
    //
    static inline eGML_PixelA5_2 Blend4(eGML_Bitmap *pBitmap, eGML_OffsetA5_2 kOffset, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
    {
      eC_Int c1 = (eC_Int)GetPixel(kOffset);
      eC_Int c2 = (eC_Int)GetPixel(kOffset+1);
      eC_Int c3 = (eC_Int)GetPixel(kOffset + ((eGML_BitmapA5_2 *)pBitmap)->m_uiPitch);
      eC_Int c4 = (eC_Int)GetPixel(kOffset + ((eGML_BitmapA5_2 *)pBitmap)->m_uiPitch+1);
      

      c1+= (((c2-c1) * ubAlphaX)>>8);
      c3+= (((c4-c3) * ubAlphaX)>>8);
      
      c1+= (((c3-c1) * ubAlphaY)>>8);
      
      return (eGML_PixelA5_2) c1;
    }
	
   static inline eC_Bool ColorKeyTest4(eGML_Bitmap *pBitmap, 
                                       eGML_OffsetA5_2 kOffset, 
                                       const eGML_Color cColorKey,
                                       eGML_PixelA5_2 &pxPixel1,
                                       eGML_PixelA5_2 &pxPixel2,
                                       eGML_PixelA5_2 &pxPixel3,
                                       eGML_PixelA5_2 &pxPixel4
                                       ) 
   {
      eGML_OffsetA5_2 kOffset2 = kOffset;
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

