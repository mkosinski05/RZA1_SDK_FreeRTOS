/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_BitmapRGB565.h
* %version:        134.1 %
*
* author:          hh04005
* creation date:   Tue May 02 11:14:19 2006
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BITMAPRGB565__H_
#define EGML_BITMAPRGB565__H_
//---------------------------------------------------------------

#include "eGML_Bitmap.h"
#include "eGML_BitmapA8.h"
#include "eGML_FontDecoder.h"

//#define eGML_BitmapRGB565_SWAP_XY
//#define eGML_BitmapRGB565_UseSWAR      // set to enable SWAR optimization techniques (SIMD Within A Register)

//---------------------------------------------------------------
// pixel type
typedef eC_UShort         eGML_PixelRGB565;
typedef eGML_PixelRGB565 *   eGML_OffsetRGB565;

//---------------------------------------------------------------

class eGML_BitmapRGB565 : public eGML_Bitmap
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

   virtual eGML_Color ColorFromARGB(eC_UByte ubA, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;
   virtual eGML_Color ColorFromRGB( eC_UByte ubR, eC_UByte ubG, eC_UByte ubB ) const;
   virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;

   virtual void DrawLine( const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth=0 );

   virtual void DrawPolygon( const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags );

   virtual void DrawDot( eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0 );

   virtual void Clear(eGML_Color clColor);

   virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_ColorARGB32 clColor, eC_Flag ulFlags = 0);

protected:
   //
   eGML_BitmapRGB565(eGML_Screen *pkScreen, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBBT=eGML::BBT_EGML, void *pBitmapBuffer=0);
   ///
   virtual ~eGML_BitmapRGB565();


private:
   friend class eGML_PixelRGB565_Access;
   friend class eGML_Screen;

   //
   virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor );

   //
   virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor );
};


//---------------------------------------------------------------
// lowlevel access methods for this kind of bitmap
//


class eGML_PixelRGB565_Access
{
public :

   static inline eGML_PixelRGB565 ColorToPixel( eGML_Color kColor )
   {
      return (eGML_PixelRGB565) kColor;
   }

   static inline eGML_PixelRGB565 ColorAndAlphaToPixel( eGML_Color kColor, eC_UByte /*ubAlpha*/ )
   {
      // use only kColor
      return ColorToPixel(kColor);
   }

   static inline eGML_Color PixelToColor( eGML_PixelRGB565 pxPixel )
   {
      return (eGML_Color) pxPixel;
   }

   static inline eGML_Color PixelToColorAndAlpha( eGML_PixelRGB565 pxPixel, eC_UByte & rubAlpha )
   {
      rubAlpha = 255; //OPAQUE;
      return PixelToColor(pxPixel);
   }

   static inline eGML_PixelRGB565 PixelFromRGB( eC_UByte ubR, eC_UByte ubG, eC_UByte ubB )
   {
      return (eGML_PixelRGB565)( ((((eC_UInt)ubR) << 8) & 0xF800) | ((((eC_UInt)ubG) << 3) & 0x07E0) | (((eC_UInt)ubB) >> 3) );
   }

   static inline eGML_PixelRGB565 PixelFromColorARGB32( eGML_ColorARGB32 colARGB32 )
   {
      return (eGML_PixelRGB565)( ((((eC_UInt)colARGB32) >> 8) & 0xF800) | ((((eC_UInt)colARGB32) >> 5) & 0x07E0) | ((((eC_UInt)colARGB32) >> 3) & 0x001F) );
   }
   
   static inline eGML_ColorARGB32 ColorARGB32FromPixel( eGML_PixelRGB565 pxPixel )
   {
      eC_UByte ubR = RedFromPixel(pxPixel);
      eC_UByte ubG = GreenFromPixel(pxPixel);
      eC_UByte ubB = BlueFromPixel(pxPixel);
      if ((ubR == ubB) && ((ubR & 0xF8) == (ubG & 0xF8)))
      {
          ubR = ubG;
          ubB = ubG;
      }
      return (eGML_ColorARGB32)( 0xFF000000 | (ubR << 16) | (ubG << 8) | (ubB) );
   }

   static inline eC_UByte RedFromPixel( eGML_PixelRGB565 pxPixel )
   {
      eC_UByte red = (eC_UByte) ((pxPixel & 0xF800) >> 8);
      return red|(red>>5);
   }

   static inline eC_UByte GreenFromPixel( eGML_PixelRGB565 pxPixel )
   {
      eC_UByte green = (eC_UByte) ((pxPixel & 0x07E0) >> 3);
      return green|(green>>6);
   }

   static inline eC_UByte BlueFromPixel( eGML_PixelRGB565 pxPixel )
   {
      eC_UByte blue = (eC_UByte) ((pxPixel & 0x001f) << 3);
      return blue|(blue>>5);
   }

   static inline eGML_PixelRGB565 RedBlueFromPixel(eGML_PixelRGB565 pxPixel)
   {
      return (pxPixel & 0xF81F);
   }

   static inline eGML_PixelRGB565 PixelFromParts(eGML_PixelRGB565 pxRB, eC_UByte ubG)
   {
      return  (eGML_PixelRGB565) ((pxRB & 0xF81F) | ((((eC_UInt)ubG) << 3) & 0x07E0));
   }

   static inline eC_UByte AlphaFromPixel(eGML_PixelRGB565 pxPixel)
   {
      return (eC_UByte) (0xff);
   }

   static inline void GetOffset( eGML_Bitmap *pBitmap, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB565 &res )
   {
      eGML_BitmapRGB565 *pInternalBitmap = (eGML_BitmapRGB565 *) pBitmap;
#ifdef eGML_BitmapRGB565_SWAP_XY
      res = (eGML_OffsetRGB565)( ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiX * (pInternalBitmap->m_uiHeight * 2)) + (((pInternalBitmap->m_uiHeight-1) - uiY) * 2) );
#else
      res = (eGML_OffsetRGB565)( ((eGML_Pixel*)pInternalBitmap->m_pkData) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 2) );
#endif
   }

   static inline void TextureOffset( const eGML_OffsetRGB565 &base, eC_UInt uiX, eC_UInt uiY, eC_UInt uiN, eGML_OffsetRGB565 &res )
   {
      res = base + (uiY << uiN) + uiX;
   }

   static inline void AddOffset( eGML_Bitmap *pBitmap, eGML_OffsetRGB565 kOffset, eC_UInt uiX, eC_UInt uiY, eGML_OffsetRGB565 &res )
   {
      eGML_BitmapRGB565 *pInternalBitmap = (eGML_BitmapRGB565 *) pBitmap;
#ifdef eGML_BitmapRGB565_SWAP_XY
            eC_Int iY = -uiY;
			res = (eGML_OffsetRGB565)( ((eGML_Pixel*)kOffset) + (uiX * (pInternalBitmap->m_uiHeight * 2)) + (iY * 2) );
#else
      res = (eGML_OffsetRGB565)( ((eGML_Pixel*)kOffset) + (uiY * pInternalBitmap->m_uiPitch) + (uiX * 2) );
#endif
   }

   static inline void IncX( eGML_Bitmap *pBitmap, eGML_OffsetRGB565 &kOffset )
   {
#ifdef eGML_BitmapRGB565_SWAP_XY
			kOffset = (eGML_OffsetRGB565) (((eGML_Pixel*)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiHeight * 2);
#else
      kOffset++;
#endif
   }

   static inline void IncY( eGML_Bitmap *pBitmap, eGML_OffsetRGB565 &kOffset )
   {
#ifdef eGML_BitmapRGB565_SWAP_XY
			kOffset--;
#else
      kOffset = (eGML_OffsetRGB565) (((eGML_Pixel*)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiPitch);
#endif
   }

   static inline void PutPixel( eGML_OffsetRGB565 kOffset, eGML_PixelRGB565 pxColor )
   {
      *kOffset = pxColor;
   }

   static inline void PutSpan( eGML_OffsetRGB565 &kOffset, eGML_PixelRGB565 pxColor, eC_Int iCount )
   {
      while (iCount-- >0) *kOffset++ = pxColor;
   }

   static inline eGML_PixelRGB565 GetRawPixel( eGML_OffsetRGB565 kOffset )
   {
      return *kOffset;
   }

   static inline eGML_PixelRGB565 GetPixel( eGML_OffsetRGB565 kOffset )
   {
      return *kOffset;
   }

   static inline eGML_PixelRGB565 MixAverage(eGML_PixelRGB565 pxColor1, eGML_PixelRGB565 pxColor2)
   {
      return (pxColor1 & pxColor2) + ((pxColor1 ^ pxColor2) >> 1);
   }

   static inline eGML_PixelRGB565 MixAdditive(eGML_PixelRGB565 pxColor1, eGML_PixelRGB565 pxColor2)
   {
#ifdef eGML_BitmapRGB565_UseSWAR
      //TODO test
      eC_Int uiRB = (eC_UInt) (pxColor1 & 0xF81F) + (eC_UInt) (pxColor2 & 0xF81F);
      eC_Int uiG = (eC_UInt) (pxColor1 & 0x07E0) + (eC_UInt) (pxColor2 & 0x07E0);

      uiRB |= (-(uiRB & 0x20020)) >> 5;
      uiG |= (-(uiG & 800)) >> 6;

      return (eGML_PixelRGB565) ((uiRB & 0xF81F) | (uiG & 0x07E0));
#else
      // color split and sum
      eC_UShort uiR = RedFromPixel(pxColor1)   + (eC_UShort) RedFromPixel(pxColor2);
      eC_UShort uiG = GreenFromPixel(pxColor1) + (eC_UShort) GreenFromPixel(pxColor2);
      eC_UShort uiB = BlueFromPixel(pxColor1)  + (eC_UShort) BlueFromPixel(pxColor2);
      // saturate (only lower 8bit)
      uiR |= (-(eC_Short)(uiR & 0x100)) >> 8;
      uiG |= (-(eC_Short)(uiG & 0x100)) >> 8;
      uiB |= (-(eC_Short)(uiB & 0x100)) >> 8;
      // recompose
      return PixelFromRGB( (eC_UByte)uiR, (eC_UByte)uiG, (eC_UByte)uiB );
#endif
   }

   static inline eGML_PixelRGB565 MixLinear(eGML_PixelRGB565 pxColor1, eGML_PixelRGB565 pxColor2, eC_UByte ubAlpha)
   {
#ifdef eGML_BitmapRGB565_UseSWAR
      eC_UShort uiAlpha = ((eC_UShort)ubAlpha + 2) >> 2;

      eC_Int r1b1 = (eC_UInt) (pxColor1 & 0xF81F);
      eC_Int g1 = (eC_UInt) (pxColor1 & 0x07E0);
      eC_Int r2b2 = (eC_UInt) (pxColor2 & 0xF81F);
      eC_Int g2 = (eC_UInt) (pxColor2 & 0x07E0);

      r1b1 += ( (r2b2 - r1b1) * uiAlpha ) >> 6;
      g1 += ( (g2 - g1) * uiAlpha ) >> 6;

      return (eGML_PixelRGB565) ((r1b1 & 0xF81F) |  (g1 & 0x07E0));

#else
      eC_UInt uiAlpha = ((eC_UInt)ubAlpha) + (ubAlpha&1);

      // color split

      eC_Short r1 = (eC_UShort) RedFromPixel(pxColor1);
      eC_Short g1 = (eC_UShort) GreenFromPixel(pxColor1);
      eC_Short b1 = (eC_UShort) BlueFromPixel(pxColor1);
      eC_Short r2 = (eC_UShort) RedFromPixel(pxColor2);
      eC_Short g2 = (eC_UShort) GreenFromPixel(pxColor2);
      eC_Short b2 = (eC_UShort) BlueFromPixel(pxColor2);

      // blend
      r1 += (eC_UShort)(( (r2 - r1) * uiAlpha ) >> 8);
      g1 += (eC_UShort)(( (g2 - g1) * uiAlpha ) >> 8);
      b1 += (eC_UShort)(( (b2 - b1) * uiAlpha ) >> 8);

      // recompose

      return PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 );
#endif

   }



   static inline eGML_PixelRGB565 MixBilinear(eGML_PixelRGB565 pxPixel1, eGML_PixelRGB565 pxPixel2, eGML_PixelRGB565 pxPixel3, eGML_PixelRGB565 pxPixel4, eC_UByte ubAlphaX, eC_UByte ubAlphaY)
   {

#ifdef eGML_BitmapRGB565_UseSWAR

      eC_UShort uiAlphaX = ((eC_UShort)ubAlphaX + 2) >> 2;
      eC_UShort uiAlphaY = ((eC_UShort)ubAlphaY + 2) >> 2;

      eC_Int r1b1 = (eC_UInt) (pxPixel1 & 0xF81F);
      eC_Int g1 = (eC_UInt) (pxPixel1 & 0x07E0);
      eC_Int r2b2 = (eC_UInt) (pxPixel2 & 0xF81F);
      eC_Int g2 = (eC_UInt) (pxPixel2 & 0x07E0);

      r1b1 += ( (r2b2 - r1b1) * uiAlphaX ) >> 6;
      g1 += ( (g2 - g1) * uiAlphaX ) >> 6;

      r1b1 &= 0xF81F;
      g1 &= 0x07E0;

      eC_Int r3b3 = (eC_UInt) (pxPixel3 & 0xF81F);
      eC_Int g3 = (eC_UInt) (pxPixel3 & 0x07E0);
      eC_Int r4b4 = (eC_UInt) (pxPixel4 & 0xF81F);
      eC_Int g4 = (eC_UInt) (pxPixel4 & 0x07E0);

      r3b3 += ( (r4b4 - r3b3) * uiAlphaX ) >> 6;
      g3 += ( (g4 - g3) * uiAlphaX ) >> 6;

      r3b3 &= 0xF81F;
      g3 &= 0x07E0;

      r1b1 += ( (r3b3 - r1b1) * uiAlphaY ) >> 6;
      g1 += ( (g3 - g1) * uiAlphaY ) >> 6;

      return (eGML_PixelRGB565) ((r1b1 & 0xF81F) | (g1 & 0x07E0));
#else
      eC_Short iAlphaX = ((eC_Short)ubAlphaX) + (ubAlphaX&1);
      eC_Short iAlphaY = ((eC_Short)ubAlphaY) + (ubAlphaY&1);

      // mix 1 & 2
      eC_Short r1 = (eC_Short) RedFromPixel(pxPixel1);
      eC_Short g1 = (eC_Short) GreenFromPixel(pxPixel1);
      eC_Short b1 = (eC_Short) BlueFromPixel(pxPixel1);
      eC_Short r2 = (eC_Short) RedFromPixel(pxPixel2);
      eC_Short g2 = (eC_Short) GreenFromPixel(pxPixel2);
      eC_Short b2 = (eC_Short) BlueFromPixel(pxPixel2);
      r1 += ( (r2 - r1) * iAlphaX ) >> 8;
      g1 += ( (g2 - g1) * iAlphaX ) >> 8;
      b1 += ( (b2 - b1) * iAlphaX ) >> 8;

      // mix 3 & 4
      eC_Short r3 = (eC_Short) RedFromPixel(pxPixel3);
      eC_Short g3 = (eC_Short) GreenFromPixel(pxPixel3);
      eC_Short b3 = (eC_Short) BlueFromPixel(pxPixel3);
      eC_Short r4 = (eC_Short) RedFromPixel(pxPixel4);
      eC_Short g4 = (eC_Short) GreenFromPixel(pxPixel4);
      eC_Short b4 = (eC_Short) BlueFromPixel(pxPixel4);
      r3 += ( (r4 - r3) * iAlphaX ) >> 8;
      g3 += ( (g4 - g3) * iAlphaX ) >> 8;
      b3 += ( (b4 - b3) * iAlphaX ) >> 8;

      // mix 1,2 & 3,4
      r1 += ( (r3 - r1) * iAlphaY ) >> 8;
      g1 += ( (g3 - g1) * iAlphaY ) >> 8;
      b1 += ( (b3 - b1) * iAlphaY ) >> 8;

      // recompose
      return PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 );
#endif
   }

   static inline void BlendPixel(eGML_OffsetRGB565 kOffset, eGML_PixelRGB565 pxColor, eC_UByte ubAlpha)
   {
      eGML_PixelRGB565 pxPixel = MixLinear( GetPixel(kOffset), pxColor, ubAlpha );
      PutPixel( kOffset, pxPixel );
   }

   static inline void BlendPixelSrcAlpha(eGML_OffsetRGB565 kOffset, eGML_PixelRGB565 pxARGB, eC_UByte ubAlpha)
   {
      BlendPixel(kOffset, pxARGB, ubAlpha);
   }

   static inline void AddPixel(eGML_OffsetRGB565 kOffset, eGML_PixelRGB565 pxColor)
   {
      eGML_PixelRGB565 pxPixel = MixAdditive( GetPixel(kOffset), pxColor );
      PutPixel( kOffset, pxPixel );
   }

   static inline eGML_PixelRGB565 Blend4(  eGML_Bitmap *pBitmap, eGML_OffsetRGB565 kOffset,
      eC_UByte ubAlphaX, eC_UByte ubAlphaY )
   {
#ifdef eGML_BitmapRGB565_UseSWAR

      eGML_PixelRGB565 pxPixel1= kOffset[0];
      eGML_PixelRGB565 pxPixel2= kOffset[1];
      eGML_PixelRGB565 pxPixel3= ((eGML_PixelRGB565 *)(((eC_UByte *)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiPitch))[0];
      eGML_PixelRGB565 pxPixel4= ((eGML_PixelRGB565 *)(((eC_UByte *)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiPitch))[1];

      eC_UShort uiAlphaX = ((eC_UShort)ubAlphaX + 2) >> 2;
      eC_UShort uiAlphaY = ((eC_UShort)ubAlphaY + 2) >> 2;

      eC_Int r1b1 = (eC_UInt) (pxPixel1 & 0xF81F);
      eC_Int g1 = (eC_UInt) (pxPixel1 & 0x07E0);
      eC_Int r2b2 = (eC_UInt) (pxPixel2 & 0xF81F);
      eC_Int g2 = (eC_UInt) (pxPixel2 & 0x07E0);

      r1b1 += ( (r2b2 - r1b1) * uiAlphaX ) >> 6;
      g1 += ( (g2 - g1) * uiAlphaX ) >> 6;

      r1b1 &= 0xF81F;
      g1 &= 0x07E0;

      eC_Int r3b3 = (eC_UInt) (pxPixel3 & 0xF81F);
      eC_Int g3 = (eC_UInt) (pxPixel3 & 0x07E0);
      eC_Int r4b4 = (eC_UInt) (pxPixel4 & 0xF81F);
      eC_Int g4 = (eC_UInt) (pxPixel4 & 0x07E0);

      r3b3 += ( (r4b4 - r3b3) * uiAlphaX ) >> 6;
      g3 += ( (g4 - g3) * uiAlphaX ) >> 6;

      r3b3 &= 0xF81F;
      g3 &= 0x07E0;

      r1b1 += ( (r3b3 - r1b1) * uiAlphaY ) >> 6;
      g1 += ( (g3 - g1) * uiAlphaY ) >> 6;

      return (eGML_PixelRGB565) ((r1b1 & 0xF81F) | (g1 & 0x07E0));
#else

      eC_Short r1;
      eC_Short g1;
      eC_Short b1;

      eC_Short r2;
      eC_Short g2;
      eC_Short b2;

      eC_Short uiAlphaX = ((eC_Short)ubAlphaX) + (ubAlphaX&1);
      eC_Short uiAlphaY = ((eC_Short)ubAlphaY) + (ubAlphaY&1);

      eGML_PixelRGB565 pxPixel1= kOffset[0];
      eGML_PixelRGB565 pxPixel2= kOffset[1];
      eGML_PixelRGB565 pxPixel3= ((eGML_PixelRGB565 *)(((eC_UByte *)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiPitch))[0];
      eGML_PixelRGB565 pxPixel4= ((eGML_PixelRGB565 *)(((eC_UByte *)kOffset) + ((eGML_BitmapRGB565 *)pBitmap)->m_uiPitch))[1];

      r1= (((((eC_Short)  RedFromPixel(pxPixel2))- ((eC_Short)  RedFromPixel(pxPixel1))) * uiAlphaX)>>8) + ((eC_Short)  RedFromPixel(pxPixel1));
      g1= (((((eC_Short)GreenFromPixel(pxPixel2))- ((eC_Short)GreenFromPixel(pxPixel1))) * uiAlphaX)>>8) + ((eC_Short)GreenFromPixel(pxPixel1));
      b1= (((((eC_Short) BlueFromPixel(pxPixel2))- ((eC_Short) BlueFromPixel(pxPixel1))) * uiAlphaX)>>8) + ((eC_Short) BlueFromPixel(pxPixel1));

      r2= (((((eC_Short)  RedFromPixel(pxPixel4))- ((eC_Short)  RedFromPixel(pxPixel3))) * uiAlphaX)>>8) + ((eC_Short)  RedFromPixel(pxPixel3));
      g2= (((((eC_Short)GreenFromPixel(pxPixel4))- ((eC_Short)GreenFromPixel(pxPixel3))) * uiAlphaX)>>8) + ((eC_Short)GreenFromPixel(pxPixel3));
      b2= (((((eC_Short) BlueFromPixel(pxPixel4))- ((eC_Short) BlueFromPixel(pxPixel3))) * uiAlphaX)>>8) + ((eC_Short) BlueFromPixel(pxPixel3));


      r1+= (((r2-r1) * uiAlphaY)>>8);
      g1+= (((g2-g1) * uiAlphaY)>>8);
      b1+= (((b2-b1) * uiAlphaY)>>8);


      return (eGML_PixelRGB565)( PixelFromRGB( (eC_UByte)r1, (eC_UByte)g1, (eC_UByte)b1 ) );
#endif
   }

   static inline eC_Bool ColorKeyTest4(eGML_Bitmap *pBitmap,
      eGML_OffsetRGB565 kOffset,
      const eGML_Color cColorKey,
      eGML_PixelRGB565 &pxPixel1,
      eGML_PixelRGB565 &pxPixel2,
      eGML_PixelRGB565 &pxPixel3,
      eGML_PixelRGB565 &pxPixel4
      )
   {
      eGML_OffsetRGB565 kOffset2 = kOffset;
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
