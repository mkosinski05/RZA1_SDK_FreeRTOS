/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Blit.h
* %version:        134.0 %
*
* author:          hh04005
* creation date:   Thu May 04 17:16:36 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BLIT__H_
#define EGML_BLIT__H_
//---------------------------------------------------------------

#include "eGML_Bitmap.h"
#include "eDBG_Statistic.h"
#include "eGML_FontDecoder.h"

#include "eGML_NEON/neonloops.h"

#ifdef eC_TARGET_CPU_ARM_NEON
#ifdef eGML_TOGGLE_NEON_SUPPORT
#define CALL_DEFAULT_FUNCTION(DefaultCall)\
        DefaultCall;\

#else //eGML_TOGGLE_NEON_SUPPORT
#define CALL_DEFAULT_FUNCTION(DefaultCall)\
        eDBG_ASSERT(false, "blit not supported");\

#endif //eGML_TOGGLE_NEON_SUPPORT
#endif //eC_TARGET_CPU_ARM_NEON

#ifdef eC_TARGET_CPU_ARM_NEON

#define OPTIMIZED_BLIT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha)\
			 if((eGML_Bitmap::BLIT_COLORKEY != (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))) && (eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
				 A1::GetOffset( pkSource, uiSrcX, uiSrcY, kSrc );\
				 A2::GetOffset( this    ,  iDstX,  iDstY, kDst );\
				 \
    			 if((255==ubAlpha) && (!pkSource->HasAlpha()))\
				 {\
					OptimizedCall;\
				 }\
				 else\
				 {\
					OptimizedCallAlpha;\
				 }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_BLIT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha)\
			    DefaultCall;\

#endif //#ifdef eC_TARGET_CPU_ARM_NEON

#ifdef eC_TARGET_CPU_ARM_NEON

#define OPTIMIZED_COLOR_BLIT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha)\
			 if((eGML_Bitmap::BLIT_COLORKEY != (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))) && (eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
				 A1::GetOffset( pkAlpha , uiSrcX, uiSrcY, kSrc );\
				 A2::GetOffset( this    ,  iDstX,  iDstY, kDst );\
				 \
				 if((255==ubAlpha) && (!pkAlpha->HasAlpha()))\
				 {\
					OptimizedCall;\
				 }\
				 else\
				 {\
					OptimizedCallAlpha;\
				 }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_COLOR_BLIT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha)\
			    DefaultCall;\

#endif //#ifdef eC_TARGET_CPU_ARM_NEON

#ifdef eC_TARGET_CPU_ARM_NEON

#define OPTIMIZED_FILLRECT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha,iX1,iY1)\
             \
			 if((eGML_Bitmap::BLIT_COLORKEY != (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))) && (eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue))\
			 {\
                      O2 kDst;\
                      A2::GetOffset( this, iX1, iY1, kDst);\
                      \
                      if(0xff==ubAlpha)\
                      {\
                        OptimizedCall;\
                      }\
                      else\
                      {\
                        OptimizedCallAlpha;\
                      }\
             }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_FILLRECT(O1,O2,A1,A2,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha,iX1,iY1)\
			    DefaultCall;\

#endif //#ifdef eC_TARGET_CPU_ARM_NEON

//---------------------------------------------------------------

#ifndef eC_BLEND_MAX
#define eC_BLEND_MAX 255
#endif

eDBG_PERF_DEF( Blit_CPY );
eDBG_PERF_DEF( Blit_INT );
eDBG_PERF_DEF( Blit_RGB );
eDBG_PERF_DEF( Blit_IDX );

eDBG_PERF_DEF( Blit_ALPHAINT );
eDBG_PERF_DEF( Blit_ALPHARGB );

eDBG_PERF_DEF( Blit_COLORINT );

eDBG_PERF_DEF( Blit_FILLRECT);


#define eGMLBlit_MulABDiv255(a,b)  eC_UByte( (                                    \
                                               (eC_UShort(a) * eC_UShort(b))      \
                                               +                                  \
                                               (                                  \
                                                 (eC_UShort(a) * eC_UShort(b))    \
                                                 >>8                              \
                                               )                                  \
                                               +                                  \
                                               1                                  \
                                             )                                    \
                                             >>8                                  \
                                           )

static inline eGML_ColorARGB32 eGMLBlit_MixLinear(const eGML_ColorARGB32 pxColor1, const eGML_ColorARGB32 pxColor2, eC_UByte ubAlpha)
{
    eGML_ColorARGB32 Result;
    if (ubAlpha == 0xFF)
    {
        Result = pxColor2;
    }
    else
    {
        eC_UByte ad = (eC_UByte)(pxColor1 >> 24);
        ad = eGMLBlit_MulABDiv255(ad, 255 - ubAlpha) + ubAlpha;
        eC_UInt rb1 = (eC_UInt) (pxColor1 & 0xff00ff);
        eC_UInt g1  = (eC_UInt)((pxColor1 >> 8) & 0xff);
        eC_UInt rb2 = (eC_UInt) (pxColor2 & 0xff00ff);
        eC_UInt g2  = (eC_UInt)((pxColor2 >> 8) & 0xff);
        rb1 += ( (rb2 - rb1) * ubAlpha ) >> 8;
        g1  += ( (g2 - g1) * ubAlpha ) >> 8;
        Result = ((rb1 & 0xff00ff) | (((eC_UInt)g1) << 8) | (ad<<24));
    }
    return Result;
}
//---------------------------------------------------------------

// Generic blit codes. Used by bitmaps to implement various conversions.
//
template <
typename P1,		// Pixeltype for Source
typename O1,		// Offsettype for Destination
class A1,			// Accessmethods for Source

typename P2,		// Pixeltype for Destination
typename O2,		// Offsettype for Destination
class A2			// Accessmethods for Destination
>

class eGML_Blit
{
public:


   //---------------------------------------------------------------
   // Direct memcpy blitter. Source and destination format must be equal.
   // Offset type must be valid pointer.
   //
   static inline void Blit_Cpy(eGML_Bitmap *pkSource, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_UInt uiBytesPerPixel)
   {
#ifdef eGML_DEBUGPIXELACCESS
      Blit_Int(pkSource, pkDest, pvBase,
         uiWidth, uiHeight,
         uiX1, uiY1, uiX2, uiY2, 255);

#else
      eDBG_PERF_BEGIN( Blit_CPY );

      O1 kSrcLine;
      O2 kDstLine;

      A1::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      A2::GetOffset( pkDest, uiX2, uiY2, kDstLine );

      eC_UInt uiPitch = uiWidth * uiBytesPerPixel;

      for (eC_UInt y=0; y < uiHeight; y++)
      {

         eC_Memcpy( (void *)kDstLine, (const void *)kSrcLine, uiPitch );
         A1::IncY( pkSource, kSrcLine );
         A2::IncY( pkDest, kDstLine );
      }

      eDBG_PERF_END( Blit_CPY );
#endif

   }


   //---------------------------------------------------------------
   // Generic fast blitter. Using pixel->color and color->pixel casting.
   //
   static inline void Blit_Int(eGML_Bitmap *pkSource, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKey=0)
   {
      // decide four possibilities
      switch (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))
      {
      default: // usual case: no flags set
         Blit_Int<false>(pkSource, pkDest, pvBase, uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, ubAlpha, cColorKey);
         break;
      case eGML_Bitmap::BLIT_COLORKEY:
         Blit_Int<true>(pkSource, pkDest, pvBase, uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, ubAlpha, cColorKey);
         break;
      }
   }

   template<bool tpl_bEnableColorKey>
   static inline void Blit_Int(eGML_Bitmap *pkSource, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_UByte ubAlpha, eGML_Color cColorKey)
   {
      eDBG_PERF_BEGIN( Blit_INT );

      O1 kSrcLine;
      O2 kDstLine;

      // Compare colorkeys pixel based!
      P1 pxCK = 0;
      if (tpl_bEnableColorKey)
      {
         pxCK = A1::ColorToPixel( cColorKey );
      }

      A1::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      A2::GetOffset( pkDest, uiX2, uiY2, kDstLine );

      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kSrc = kSrcLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               P1 px = A1::GetPixel(kSrc);
               if ((!tpl_bEnableColorKey) || (A1::PixelToColor(px) != pxCK))
               {
                  A2::PutPixel(kDst, px);
               }
               A1::IncX( pkSource, kSrc );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkSource, kSrcLine );
            A2::IncY( pkDest, kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kSrc = kSrcLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               P1 px = A1::GetPixel( kSrc );
               if ((!tpl_bEnableColorKey) || (A1::PixelToColor(px) != pxCK))
               {
                  A2::BlendPixelSrcAlpha(kDst, px, ubAlpha);
               }
               A1::IncX( pkSource, kSrc );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkSource, kSrcLine );
            A2::IncY( pkDest, kDstLine );
         }
      }

      eDBG_PERF_END( Blit_INT );
   }


   //---------------------------------------------------------------
   // Generic fast blitter. Using color splitting and color repacking.
   // Necessary for certian combinations of source/dest types. usually generates slower code.
   // HINT: cColorKey has to be converted to Source Native Color by caller!!!
   //
   static void Blit_RGB(eGML_Bitmap *pkSource, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKeySrcFormat=0)
   {
      // decide four possibilities
      switch (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))
      {
      default: // usual case: no flags set
         Blit_RGB<false>(pkSource, pkDest, pvBase, uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, ubAlpha, cColorKeySrcFormat);
         break;
      case eGML_Bitmap::BLIT_COLORKEY:
         Blit_RGB<true>(pkSource, pkDest, pvBase, uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, ubAlpha, cColorKeySrcFormat);
         break;
      }
   }

   template<bool tpl_bEnableColorKey>
   static inline void Blit_RGB(eGML_Bitmap *pkSource, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_UByte ubAlpha, eGML_Color cColorKeySrcFormat)
   {
      eDBG_PERF_BEGIN( Blit_RGB );

      O1 kSrcLine;
      O2 kDstLine;

      A1::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      A2::GetOffset( pkDest,   uiX2, uiY2, kDstLine );

      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kSrcBlit = kSrcLine;
            O2 kDstBlit = kDstLine;
 
            for (eC_UInt x=0; x < uiWidth; x++)
            {
               P1 pxPixel   =  A1::GetPixel(kSrcBlit);
               if ((!tpl_bEnableColorKey) || (A1::PixelToColor( pxPixel ) != cColorKeySrcFormat))
               {
                  eC_UByte r = A1::RedFromPixel( pxPixel );
                  eC_UByte g = A1::GreenFromPixel( pxPixel );
                  eC_UByte b = A1::BlueFromPixel( pxPixel );
                  A2::PutPixel(kDstBlit, A2::PixelFromRGB(r, g, b));
               }
               A1::IncX( pkSource, kSrcBlit );
               A2::IncX( pkDest, kDstBlit );
            }

            A1::IncY( pkSource, kSrcLine );
            A2::IncY( pkDest,   kDstLine );
         }
      }
      else
      { 
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kSrcBlit = kSrcLine;
            O2 kDstBlit = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               P1 pxPixel   = A1::GetPixel(kSrcBlit);
               if ((!tpl_bEnableColorKey) || (A1::PixelToColor( pxPixel ) != cColorKeySrcFormat))
               {
                  eC_UByte r = A1::RedFromPixel(   pxPixel );
                  eC_UByte g = A1::GreenFromPixel( pxPixel );
                  eC_UByte b = A1::BlueFromPixel(  pxPixel );
                  eC_UByte a = A1::AlphaFromPixel( pxPixel );

                  // modulate color by global alpha and calculate correct destination alpha
                  A2::BlendPixelSrcAlpha(
                     kDstBlit,
                     A2::ColorAndAlphaToPixel(A2::PixelFromRGB( r, g, b ), a),
                     ubAlpha
                     );

               } // Colorkey
               A1::IncX( pkSource, kSrcBlit);
               A2::IncX( pkDest,   kDstBlit);
            }

            A1::IncY( pkSource, kSrcLine );
            A2::IncY( pkDest,   kDstLine );
         }
      }
      eDBG_PERF_END( Blit_RGB );
   }




   //---------------------------------------------------------------
   // Generic color blitter. Writing solid color weighted by alphamap.
   //
   static inline void Blit_ColorInt(eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2,
      eGML_ColorARGB32 cCol, eC_UByte ubAlpha)
   {
      eDBG_PERF_BEGIN( Blit_COLORINT );

      O1 kAlphaLine;
      O2 kDstLine;

      A1::GetOffset( pkAlpha, uiX1, uiY1, kAlphaLine );
      A2::GetOffset( pkDest, uiX2, uiY2, kDstLine );


      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kAlpha = kAlphaLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               //A2::BlendPixel( kDst, pxColor, (eC_UByte) A1::AlphaFromPixel(A1::GetPixel(kAlpha)));
               eGML_ColorARGB32 colDstPixel = A2::ColorARGB32FromPixel(A2::GetPixel(kDst));
               A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol, (eC_UByte) A1::AlphaFromPixel(A1::GetPixel(kAlpha)))));

               A1::IncX( pkAlpha, kAlpha );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkAlpha, kAlphaLine );
            A2::IncY( pkDest, kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kAlpha = kAlphaLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubBlend = (eC_UByte) A1::AlphaFromPixel(A1::GetPixel( kAlpha ));

               ubBlend = eGMLBlit_MulABDiv255( ubBlend , ubAlpha );
               //A2::BlendPixel( kDst, pxColor, ubBlend );
               eGML_ColorARGB32 colDstPixel = A2::ColorARGB32FromPixel(A2::GetPixel(kDst));
               A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol, ubBlend)));

               A1::IncX( pkAlpha, kAlpha );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkAlpha, kAlphaLine );
            A2::IncY( pkDest, kDstLine );
         }
      }

      eDBG_PERF_END( Blit_COLORINT );
   }

   static inline void Blit_2ColorInt(eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest, void *pvBase,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2,
      eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_UByte ubAlpha)
   {
      eDBG_PERF_BEGIN( Blit_COLORINT );

      /*if (pkAlpha->m_pkScreen->RenderState.FNT_OUTLINE) {
      Blit_ColorInt_2_Col(pkAlpha, pkDest, pvBase,uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, kCol, ubAlpha);
      return;
      }*/

      O1 kAlphaLine;
      O2 kDstLine;

      A1::GetOffset( pkAlpha, uiX1, uiY1, kAlphaLine );
      A2::GetOffset( pkDest, uiX2, uiY2, kDstLine );


      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kAlpha = kAlphaLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {

               eC_UByte uRawBlend = (eC_UByte) A1::GetRawPixel( kAlpha );
               eGML_ColorARGB32 colDstPixel = A2::ColorARGB32FromPixel(A2::GetPixel(kDst));

               // two blends -> two pixelaccess
               /*switch (uRawBlend & FLAG_BIT_MASK)
               {
               // least probability
               case OUTLINE_BIT_MASK:
               {
               if (!BIT_BLEND_MASK) {
               A2::BlendPixel( kDst, pxColorOutline, (uRawBlend | (( uRawBlend & 224) >> 5)) );
               break; // skip two color blending
               } else {
               A2::BlendPixel( kDst, pxColorOutline, 255-(uRawBlend | (( uRawBlend & 224) >> 5)) ); // inv_alph for outline to inline blending
               }
               }

               // most probabilty
               case INSIDE_BIT_MASK:
               {
               A2::BlendPixel( kDst, pxColorInside, (uRawBlend | (( uRawBlend & 224) >> 5)) );
               break;
               }
               // outside
               default:
               ;
               }*/

               switch (uRawBlend & FLAG_BIT_MASK)
               {
                  // least probability
               case OUTLINE_BIT_MASK:
                  //A2::BlendPixel( kDst, pxColorOutline, (uRawBlend | (( uRawBlend & 224) >> 5)));// 5Bit to 8Bit conversion
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol1, (uRawBlend | (( uRawBlend & 224) >> 5)))));
                  break;
                  // most probabilty
               case INSIDE_BIT_MASK:
                  //A2::BlendPixel( kDst, pxColorInside, eC_BLEND_MAX); // alpha is MAX, since we are inside
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol2, eC_BLEND_MAX)));
                  break;
                  // outside
               case BIT_BLEND_MASK:
                  uRawBlend |= (( uRawBlend & 224) >> 5);
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(cCol1, cCol2, uRawBlend)));
                  break;
               default:
                  ;
               }

               A1::IncX( pkAlpha, kAlpha );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkAlpha, kAlphaLine );
            A2::IncY( pkDest, kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O1 kAlpha = kAlphaLine;
            O2 kDst = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte uRawBlend = (eC_UByte) A1::GetPixel( kAlpha );
               eGML_ColorARGB32 colDstPixel = A2::ColorARGB32FromPixel(A2::GetPixel(kDst));

               switch (uRawBlend & FLAG_BIT_MASK)
               {
                  // least probability
               case OUTLINE_BIT_MASK:
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol1, ((uRawBlend | (( uRawBlend & 224) >> 5)) * ubAlpha) >> 8 )));
                  break;
                  // most probabilty
               case INSIDE_BIT_MASK:
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol2, (eC_BLEND_MAX * ubAlpha) >> 8)));
                  break;
                  // outside
               case BIT_BLEND_MASK:
                  uRawBlend |= (( uRawBlend & 224) >> 5);
                  A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(cCol1, cCol2, uRawBlend)));
                  break;
               default:
                  ;
               }

               A1::IncX( pkAlpha, kAlpha );
               A2::IncX( pkDest, kDst );
            }

            A1::IncY( pkAlpha, kAlphaLine );
            A2::IncY( pkDest, kDstLine );
         }
      }

      eDBG_PERF_END( Blit_COLORINT );
   }


   // Generic color fill
   //
   static inline void Fill_Color(eGML_Bitmap *pkDest, eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eGML_ColorARGB32 cCol)
   {
      O2 kDstLine;
      eDBG_PERF_BEGIN( Blit_FILLRECT );
      eC_UByte ubAlpha = (eC_UByte)(cCol >> 24);
      if (ubAlpha == 0xFF)
      {
          A2::GetOffset(pkDest, uiX1, uiY1, kDstLine);
          P2 pxColor = A2::PixelFromColorARGB32(cCol);

          for (eC_UInt y = 0; y < uiHeight; y++)
          {
              O2 kDst = kDstLine;

              A2::PutSpan(kDst, pxColor, uiWidth);
              A2::IncY(pkDest, kDstLine);
          }
      }
      else
      {
         A2::GetOffset( pkDest, uiX1, uiY1, kDstLine );
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            O2 kDst = kDstLine;
            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eGML_ColorARGB32 colDstPixel = A2::ColorARGB32FromPixel(A2::GetPixel(kDst));
               A2::PutPixel( kDst, A2::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, cCol, ubAlpha)) );
               A2::IncX( pkDest, kDst );
            }
            A2::IncY( pkDest, kDstLine );
         }
      }
      eDBG_PERF_END( Blit_FILLRECT);

   }

};

//---------------------------------------------------------------

#ifdef eC_TARGET_CPU_ARM_NEON

#define OPTIMIZED_BLIT_ALPHA(OS,OA,OD,AS,AA,AD,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha) \
			 if((eGML_Bitmap::BLIT_COLORKEY != (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))) && (eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				OS kSrc;\
				OA kAlpha;\
				OD kDst;\
				\
				AS::GetOffset( pkSource, uiSrcX, uiSrcY, kSrc   );\
				AA::GetOffset( pkAlpha , uiSrcX, uiSrcY, kAlpha );\
				AD::GetOffset( this    , iDstX,   iDstY, kDst   );\
				 \
				 {\
				 if((255==ubAlpha) && (!pkSource->HasAlpha()))\
				 {\
					OptimizedCall;\
				 }\
				 else\
				 {\
					OptimizedCallAlpha;\
				 }\
			   }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_BLIT_ALPHA(OS,OA,OD,AS,AA,AD,OptimizedCall,OptimizedCallAlpha,DefaultCall,ulFlags,ubAlpha) \
			    DefaultCall;\

#endif


template <
typename PS,		// Pixeltype for Color Source
typename OS,		// Offsettype for Color Source
class AS,			// Accessmethods for Color Source

   typename PA,		// Pixeltype for Alpha Source
   typename OA,		// Offsettype for Alpha Source
class AA,			// Accessmethods for Alpha Source

   typename PD,		// Pixeltype for Destination
   typename OD,		// Offsettype for Destination
class AD			// Accessmethods for Destination
>

class eGML_AlphaBlit
{
public:
   //---------------------------------------------------------------
   //
   static inline void Blit_Int(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_Flag ulFlags, eC_UByte ubAlpha)
   {
      (void)ulFlags;
      Blit_Int(pkSource, pkAlpha, pkDest, uiWidth, uiHeight, uiX1, uiY1, uiX2, uiY2, ubAlpha);
   }

   static inline void Blit_Int(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_UByte ubAlpha)
   {
      eDBG_PERF_BEGIN( Blit_ALPHAINT );

      OS kSrcLine;
      OA kAlphaLine;
      OD kDstLine;

      AS::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      AA::GetOffset( pkAlpha , uiX1, uiY1, kAlphaLine );
      AD::GetOffset( pkDest  , uiX2, uiY2, kDstLine );

      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubBlend = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel(kAlpha));

               AD::BlendPixel( kDst, AS::GetPixel( kSrc ), ubBlend );

               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );

            }

            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubBlend = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel(kAlpha));
               ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
               AD::BlendPixel( kDst, AS::GetPixel( kSrc ), ubBlend );

               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );

            }

            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }

      }

      eDBG_PERF_END( Blit_ALPHAINT );
   }

   static inline void Blit_RGB(eGML_Bitmap *pkSource,eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_Flag ulFlags, eC_UByte ubAlpha)
   {
      eDBG_PERF_BEGIN( Blit_ALPHARGB );

      OS kSrcLine;
      OA kAlphaLine;
      OD kDstLine;

      AS::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      AA::GetOffset( pkAlpha , uiX1, uiY1, kAlphaLine );
      AD::GetOffset( pkDest  , uiX2, uiY2, kDstLine );

      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubAa = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel( kAlpha ));

               if (0 != ubAa)
               {
               //PS pxPixel   = AS::GetPixel( kSrc);
               //eC_UByte r = AS::RedFromPixel( pxPixel );
               //eC_UByte g = AS::GreenFromPixel( pxPixel );
               //eC_UByte b = AS::BlueFromPixel( pxPixel );
//                   AD::BlendPixel( kDst, AD::PixelFromRGB( r, g, b ), ubAa );
                   eGML_ColorARGB32 colSrcPixel = AS::ColorARGB32FromPixel(AS::GetPixel(kSrc));
                   eGML_ColorARGB32 colDstPixel = AD::ColorARGB32FromPixel(AD::GetPixel(kDst));
                   AD::PutPixel( kDst, AD::PixelFromColorARGB32(eGMLBlit_MixLinear(colDstPixel, colSrcPixel, ubAa)));
               }
               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );
            }
            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubBlend = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel(kAlpha));
               ubBlend = (ubBlend*ubAlpha)>>8;

               if (0 != ubBlend)
               {
               PS pxPixel = AS::GetPixel( kSrc);
               eC_UByte r = AS::RedFromPixel( pxPixel );
               eC_UByte g = AS::GreenFromPixel( pxPixel );
               eC_UByte b = AS::BlueFromPixel( pxPixel );
               AD::BlendPixel( kDst, AD::PixelFromRGB( r, g, b ), ubBlend );
               }

               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );
            }

            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }

      }

      eDBG_PERF_END( Blit_ALPHARGB);


   }

   static inline void Blit_RGB_Cpy_Alpha(eGML_Bitmap *pkSource,eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
      eC_UInt uiWidth, eC_UInt uiHeight,
      eC_UInt uiX1, eC_UInt uiY1, eC_UInt uiX2, eC_UInt uiY2, eC_Flag ulFlags, eC_UByte ubAlpha)
   {
      eDBG_PERF_BEGIN( Blit_RGB );

      OS kSrcLine;
      OA kAlphaLine;
      OD kDstLine;

      AS::GetOffset( pkSource, uiX1, uiY1, kSrcLine );
      AA::GetOffset( pkAlpha , uiX1, uiY1, kAlphaLine );
      AD::GetOffset( pkDest  , uiX2, uiY2, kDstLine );

      if (ubAlpha==255)
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubAa = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel( kAlpha ));

               if (0 != ubAa)
               {
                   PS pxPixel   = AS::GetPixel( kSrc);
                   eC_UByte r = AS::RedFromPixel( pxPixel );
                   eC_UByte g = AS::GreenFromPixel( pxPixel );
                   eC_UByte b = AS::BlueFromPixel( pxPixel );
                   AD::PutPixel( kDst, AD::PixelFromColorARGB32( pkSource->ColorFromARGB(ubAa, r, g, b)  ));
               }
               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );
            }
            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }
      }
      else
      {
         for (eC_UInt y=0; y < uiHeight; y++)
         {
            OS kSrc   = kSrcLine;
            OA kAlpha = kAlphaLine;
            OD kDst   = kDstLine;

            for (eC_UInt x=0; x < uiWidth; x++)
            {
               eC_UByte ubBlend = (eC_UByte) AA::AlphaFromPixel(AA::GetPixel(kAlpha));
               ubBlend = (ubBlend*ubAlpha)>>8;

               if (0 != ubBlend)
               {
                   PS pxPixel = AS::GetPixel( kSrc);
                   eC_UByte r = AS::RedFromPixel( pxPixel );
                   eC_UByte g = AS::GreenFromPixel( pxPixel );
                   eC_UByte b = AS::BlueFromPixel( pxPixel );
                   AD::PutPixel( kDst, AD::PixelFromColorARGB32( pkSource->ColorFromARGB( ubBlend, r, g, b ) ));
               }

               AS::IncX( pkSource, kSrc );
               AA::IncX( pkAlpha, kAlpha );
               AD::IncX( pkDest , kDst );
            }

            AS::IncY( pkSource, kSrcLine );
            AA::IncY( pkAlpha , kAlphaLine );
            AD::IncY( pkDest  , kDstLine );
         }

      }

      eDBG_PERF_END( Blit_RGB);


   }

};
//---------------------------------------------------------------
#endif

