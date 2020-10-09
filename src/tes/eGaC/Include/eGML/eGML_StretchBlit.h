/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_StretchBlit.h
* %version:        134.0 %
*
* author:          hh04005
* creation date:   Thu May 04 17:20:18 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_STRETCHBLIT__H_
#define EGML_STRETCHBLIT__H_
//---------------------------------------------------------------

#include "eGML_Blit.h"
#include "eGML_Bitmap.h"
#include "eGML_BitmapA8.h"
#include "eDBG_Statistic.h"
#include "eC_Math.h"


//---------------------------------------------------------------

eDBG_PERF_DEF( StretchBlit_CPY );
eDBG_PERF_DEF( StretchBlit_INT );
eDBG_PERF_DEF( StretchBlit_RGB );
eDBG_PERF_DEF( StretchBlit_IDX );

//---------------------------------------------------------------

/*TODO: everything */

// Generic stretchblit codes. Used by bitmaps to implement various conversions.
//

#ifdef eC_TARGET_CPU_ARM_NEON

#define OPTIMIZED_STRETCHBLIT(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			 if((eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
                 A1::GetOffset( pkSource,           0,           0,          kSrc );\
				 A2::GetOffset( this    ,           uiClipMinX,  uiClipMinY, kDst );\
				 \
				 if((255==ubAlpha) && (!pkSource>HasAlpha()))\
				 {\
                    if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinear;\
                    }\
                    else\
                    {\
                        OptimizedCallNearest;\
                    }\
				 }\
				 else\
				 {\
					if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinearAlpha;\
                    }\
                    else\
                    {\
                        OptimizedCallNearestAlpha;\
                    }\
				 }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_STRETCHBLIT(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY)  \
			    DefaultCall;\

#endif

#ifdef eC_TARGET_CPU_ARM_NEON
#define OPTIMIZED_STRETCHBLITCOLOR(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			 if((eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
                 A1::GetOffset( pkAlpha,          0,          0, kSrc );\
				 A2::GetOffset( this   , uiClipMinX, uiClipMinY, kDst );\
				 \
				 if((255==ubAlpha) && (!pkAlpha>HasAlpha()))\
				 {\
                    if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinear;\
                    }\
                    else\
                    {\
                        OptimizedCallNearest;\
                    }\
				 }\
				 else\
				 {\
					if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinearAlpha;\
                    }\
                    else\
                    {\
                        OptimizedCallNearestAlpha;\
                    }\
				 }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_STRETCHBLITCOLOR(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			    DefaultCall;\

#endif

#ifdef eC_TARGET_CPU_ARM_NEON
#define OPTIMIZED_STRETCHBLITALPHA1(O1,O2,O3,A1,A2,A3,OptimizedCallNearest,OptimizedCallBilinear,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			 if((eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
                 A1::GetOffset( pkSource,          0,          0, kSrc );\
				 A2::GetOffset( this    , uiClipMinX, uiClipMinY, kDst );\
				 \
				if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                {\
                   OptimizedCallBilinear;\
                }\
                else\
                {\
                   OptimizedCallNearest;\
                }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else
#define OPTIMIZED_STRETCHBLITALPHA1(O1,O2,O3,A1,A2,A3,OptimizedCallNearest,OptimizedCallBilinear,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY)  \
			    DefaultCall;\

#endif //eC_TARGET_CPU_ARM_NEON

#ifdef eC_TARGET_CPU_ARM_NEON
#define OPTIMIZED_STRETCHBLITALPHA(OS,OD,OA,AS,AD,AA,OptimizedCallNearest,OptimizedCallBilinear,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			 if((eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				OS kSrc;\
				OA kAlpha;\
				OD kDst;\
				\
				AS::GetOffset( pkSource,          0,          0, kSrc   );\
				AA::GetOffset( pkAlpha ,          0,          0, kAlpha );\
				AD::GetOffset( this    , uiClipMinX, uiClipMinY, kDst   );\
				 \
				if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                {\
                   OptimizedCallBilinear;\
                }\
                else\
                {\
                   OptimizedCallNearest;\
                }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else
#define OPTIMIZED_STRETCHBLITALPHA(OS,OD,OA,AS,AD,AA,OptimizedCallNearest,OptimizedCallBilinear,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY)  \
			    DefaultCall;\

#endif //eC_TARGET_CPU_ARM_NEON

#ifdef eC_TARGET_CPU_ARM_NEON
const eC_UInt MIN_STRETCHBLIT_WIDTH  = 2;
const eC_UInt MIN_STRETCHBLIT_HEIGHT = 2;

#define OPTIMIZED_STRETCHBLIT_FILLRECT(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,FillRect,FillRectAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			 if((eGML_RenderState::OB_NEON == GetState( eGML_RenderState::OPTIMIZED_BLIT ).uiValue)) \
			 {	\
				 O1 kSrc;\
				 O2 kDst;\
				 \
                 A1::GetOffset( pkSource,          0,          0, kSrc );\
				 A2::GetOffset( this    , uiClipMinX, uiClipMinY, kDst );\
				 \
				 eGML_Color clColor = A1::PixelToColor(A1::GetPixel(kSrc));\
				 \
				 if(255==ubAlpha)\
				 {\
                    if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinear;\
                    }\
					else if (((uiSrcWidth < MIN_STRETCHBLIT_WIDTH) && (uiSrcHeight < MIN_STRETCHBLIT_HEIGHT)) && (false == (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))))\
					{\
						FillRect;\
					}\
                    else\
                    {\
                        OptimizedCallNearest;\
                    }\
				 }\
				 else\
				 {\
					if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)\
                    {\
                        OptimizedCallBilinearAlpha;\
                    }\
					else if (((uiSrcWidth < MIN_STRETCHBLIT_WIDTH) && (uiSrcHeight < MIN_STRETCHBLIT_HEIGHT)) && (false == (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))))\
					{\
						FillRectAlpha;\
					}\
                    else\
                    {\
                        OptimizedCallNearestAlpha;\
                    }\
				 }\
			 }\
			 else\
			 {\
                CALL_DEFAULT_FUNCTION(DefaultCall);\
			 }\

#else

#define OPTIMIZED_STRETCHBLIT_FILLRECT(O1,O2,A1,A2,OptimizedCallNearest,OptimizedCallNearestAlpha,OptimizedCallBilinear,OptimizedCallBilinearAlpha,FillRect,FillRectAlpha,DefaultCall,ulFlags,ubAlpha,uiClipMinX,uiClipMinY) \
			    DefaultCall;\

#endif // eC_TARGET_CPU_ARM_NEON

class eGML_StretchBlitParam
{
public:
  static bool CalcParams( eGML_Bitmap *pkSrc, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
    eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags,
    eC_Fixed &src_left, eC_Fixed &src_top, eC_Fixed &src_right, eC_Fixed &src_bottom,
    eC_Fixed &dst_left, eC_Fixed &dst_top , eC_Fixed &dst_right, eC_Fixed &dst_bottom,
    eC_Fixed &dx, eC_Fixed &dy,
    eC_Int &iWidth, eC_Int &iHeight);

};


template <
typename P1,		// Pixeltype for Source
typename O1,		// Offsettype for Source
class A1,			// Accessmethods for Source

typename P2,		// Pixeltype for Destination
typename O2,		// Offsettype for Destination
class A2			// Accessmethods for Destination

>

class eGML_StretchBlit
{
public:

  static inline void StretchBlit_Int(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
    eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKey)
  {
      switch (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))
      {
      default: // usual case: no flags set
         StretchBlit_Int<false>(pkSrc, pkDest,
            uiSrcWidth, uiSrcHeight, iSrcX, iSrcY,
            uiDstWidth, uiDstHeight, iDstX, iDstY, ulFlags, ubAlpha, cColorKey);
         break;
      case eGML_Bitmap::BLIT_COLORKEY:
         StretchBlit_Int<true>(pkSrc, pkDest,
            uiSrcWidth, uiSrcHeight, iSrcX, iSrcY,
            uiDstWidth, uiDstHeight, iDstX, iDstY, ulFlags, ubAlpha, cColorKey);
         break;
      }
  }

  // Generic stretch blitter (with & without bilinear filtering).
  //
  template <bool tpl_bColorKey>
  static inline void StretchBlit_Int(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
     eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKey)
  {
     eC_Int iHeight;
     eC_Int iWidth;

     eC_Fixed dst_left;
     eC_Fixed dst_top;
     eC_Fixed dst_right;
     eC_Fixed dst_bottom;

     eC_Fixed src_left;
     eC_Fixed src_top;
     eC_Fixed src_right;
     eC_Fixed src_bottom;

     eC_Fixed dx;
     eC_Fixed dy;

     O1 kSrcLine;
     O2 kDstLine;

     eC_Bool boolAlpha = ubAlpha!=255;


     eC_Fixed sy;
     eC_Int   oy;

     if (!eGML_StretchBlitParam::CalcParams( pkSrc     , 0           , pkDest,
        uiSrcWidth, uiSrcHeight , iSrcX     , iSrcY,
        uiDstWidth, uiDstHeight , iDstX     , iDstY,      ulFlags,
        src_left  , src_top     , src_right , src_bottom,
        dst_left  , dst_top     , dst_right , dst_bottom,
        dx        , dy          ,
        iWidth    , iHeight))
     {
        return;
     }

     // Compare colorkeys pixel based!
     P1 pxCK = 0;
     if (tpl_bColorKey)
     {
        pxCK = A1::ColorToPixel( cColorKey );
     }

     A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
     A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );

     sy= src_top;
     oy= eC_FixToInt(src_top);


     for (eC_Int y=0; y < iHeight; y++)
     {
        O1 kSrc = kSrcLine;
        O2 kDst = kDstLine;


        eC_Fixed sx= src_left;
        eC_Int   ox= eC_FixToInt(sx);

        if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
        {

           eC_UByte fracy= eC_FixFracToByte(sy);

           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

                 if (!tpl_bColorKey)
                 {
                    A2::BlendPixelSrcAlpha( kDst, A1::Blend4( pkSrc, kSrc, fracx, fracy ), ubAlpha );
                 }
                 else
                 {
                    P1 px1, px2, px3, px4;
                    if(A1::ColorKeyTest4(pkSrc, kSrc, pxCK, px1, px2, px3, px4))
                    {
                       P1 px = A1::MixBilinear(px1, px2, px3, px4, fracx, fracy);
                       A2::BlendPixelSrcAlpha( kDst, px, ubAlpha );
                    }
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

                 if (!tpl_bColorKey)
                 {
                    A2::PutPixel( kDst, A1::Blend4( pkSrc, kSrc, fracx, fracy ) );
                 }
                 else
                 {
                    P1 px1, px2, px3, px4;
                    if(A1::ColorKeyTest4(pkSrc, kSrc, pxCK, px1, px2, px3, px4))
                    {
                       P1 px = A1::MixBilinear(px1, px2, px3, px4, fracx, fracy);
                       A2::BlendPixelSrcAlpha( kDst, px, ubAlpha );
                    }
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }


        }
        else //Non filtered loop
        {
           if (boolAlpha)
           {

              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 P1 px = A1::GetPixel(kSrc);

                 if( (!tpl_bColorKey) || (A1::PixelToColor(px) != pxCK) )
                 {
                    A2::BlendPixelSrcAlpha( kDst, px, ubAlpha );
                 }


                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 if ((!tpl_bColorKey) || (A1::PixelToColor(A1::GetPixel(kSrc)) != cColorKey))
                 {
                    A2::PutPixel( kDst, A1::GetPixel( kSrc) );
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
        }

        sy+= dy;
        eC_Int ty= eC_FixToInt(sy);

        A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
        oy= ty;

        A2::IncY( pkDest, kDstLine );
     }
  }


  static inline void StretchBlit_RGB(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
     eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKey)
  {
     switch (ulFlags & (eGML_Bitmap::BLIT_COLORKEY))
     {
     default: // usual case: no flags set
        StretchBlit_RGB<false>(pkSrc, pkDest,
           uiSrcWidth, uiSrcHeight, iSrcX, iSrcY,
           uiDstWidth, uiDstHeight, iDstX, iDstY, ulFlags, ubAlpha, cColorKey);
        break;
     case eGML_Bitmap::BLIT_COLORKEY:
        StretchBlit_RGB<true>(pkSrc, pkDest,
           uiSrcWidth, uiSrcHeight, iSrcX, iSrcY,
           uiDstWidth, uiDstHeight, iDstX, iDstY, ulFlags, ubAlpha, cColorKey);
        break;
     }
  }


  // Generic stretch blitter (with & without bilinear filtering).
  //
  template <bool tpl_bColorKey>
  static inline void StretchBlit_RGB(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
     eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha, eGML_Color cColorKey)
  {
     eC_Int iHeight;
     eC_Int iWidth;

     eC_Fixed dst_left;
     eC_Fixed dst_top;
     eC_Fixed dst_right;
     eC_Fixed dst_bottom;

     eC_Fixed src_left;
     eC_Fixed src_top;
     eC_Fixed src_right;
     eC_Fixed src_bottom;

     eC_Fixed dx;
     eC_Fixed dy;

     O1 kSrcLine;
     O2 kDstLine;

     eC_Fixed sy;
     eC_Int   oy;

     eC_Bool boolAlpha = ubAlpha!=255;

     if (!eGML_StretchBlitParam::CalcParams( pkSrc     , 0           ,  pkDest,
        uiSrcWidth, uiSrcHeight ,  iSrcX     , iSrcY,
        uiDstWidth, uiDstHeight ,  iDstX     , iDstY,      ulFlags,
        src_left  , src_top     ,  src_right , src_bottom,
        dst_left  , dst_top     ,  dst_right , dst_bottom,
        dx        , dy          ,
        iWidth    , iHeight))
     {
        return;
     }

     P1 pxCK = 0;
     if (tpl_bColorKey)
     {
        pxCK = A1::ColorToPixel( cColorKey );
     }

     A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
     A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );

     sy= src_top;
     oy= eC_FixToInt(src_top);


     for (eC_Int y=0; y < iHeight; y++)
     {
        O1 kSrc = kSrcLine;
        O2 kDst = kDstLine;



        eC_Fixed sx= src_left;
        eC_Int   ox= eC_FixToInt(sx);

        if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
        {

           eC_UByte fracy= eC_FixFracToByte(eC_FixFrac(sy));

           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;
                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

                 //P1 pxPixel   = A1::GetPixel( kSrc );
                 P1 pxPixel;
                 if (!tpl_bColorKey)
                 {
                    pxPixel = A1::Blend4( pkSrc, kSrc, fracx, fracy );
                    eC_UByte r = A1::RedFromPixel   ( pxPixel );
                    eC_UByte g = A1::GreenFromPixel ( pxPixel );
                    eC_UByte b = A1::BlueFromPixel  ( pxPixel );
                    eC_UByte a = A1::AlphaFromPixel ( pxPixel );

                    A2::BlendPixelSrcAlpha( kDst, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a), ubAlpha );
                 }
                 else
                 {
                    P1 px1, px2, px3, px4;
                    if(A1::ColorKeyTest4(pkSrc, kSrc, pxCK, px1, px2, px3, px4))
                    {
                       pxPixel = A1::MixBilinear(px1, px2, px3, px4, fracx, fracy);
                       eC_UByte r = A1::RedFromPixel   ( pxPixel );
                       eC_UByte g = A1::GreenFromPixel ( pxPixel );
                       eC_UByte b = A1::BlueFromPixel  ( pxPixel );
                       eC_UByte a = A1::AlphaFromPixel ( pxPixel );

                       A2::BlendPixel( kDst, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB( r, g, b )), a), ubAlpha );
                    }
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;
                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;
                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

                 P1 pxPixel;
                 if (!tpl_bColorKey)
                 {
                    pxPixel = A1::Blend4( pkSrc, kSrc, fracx, fracy );
                    eC_UByte r = A1::RedFromPixel   ( pxPixel );
                    eC_UByte g = A1::GreenFromPixel ( pxPixel );
                    eC_UByte b = A1::BlueFromPixel  ( pxPixel );
                    eC_UByte a = A1::AlphaFromPixel ( pxPixel );
                    A2::PutPixel( kDst, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB( r, g, b )), a) );
                 }
                 else
                 {
                    P1 px1, px2, px3, px4;
                    if(A1::ColorKeyTest4(pkSrc, kSrc, pxCK, px1, px2, px3, px4))
                    {
                       pxPixel = A1::MixBilinear(px1, px2, px3, px4, fracx, fracy);
                       eC_UByte r = A1::RedFromPixel   ( pxPixel );
                       eC_UByte g = A1::GreenFromPixel ( pxPixel );
                       eC_UByte b = A1::BlueFromPixel  ( pxPixel );
                       eC_UByte a = A1::AlphaFromPixel ( pxPixel );
                       A2::PutPixel( kDst, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB( r, g, b )), a) );
                    }
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;
                 A2::IncX( pkDest, kDst );
              }
           }

        }
        else //Non filtered loop
        {
           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;
                 P1 pxPixel = A1::GetPixel( kSrc );
                 if ((!tpl_bColorKey) || (A1::PixelToColor(pxPixel) != cColorKey))
                 {
                    eC_UByte r = A1::RedFromPixel( pxPixel );
                    eC_UByte g = A1::GreenFromPixel( pxPixel );
                    eC_UByte b = A1::BlueFromPixel( pxPixel );
                    A2::BlendPixel( kDst, A2::PixelFromRGB( r, g, b ), ubAlpha );
                 }
                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;
                 P1 pxPixel  = A1::GetPixel( kSrc );
                 if ((!tpl_bColorKey) || (A1::PixelToColor(pxPixel) != cColorKey))
                 {
                    eC_UByte r = A1::RedFromPixel( pxPixel );
                    eC_UByte g = A1::GreenFromPixel( pxPixel );
                    eC_UByte b = A1::BlueFromPixel( pxPixel );
                    A2::PutPixel( kDst, A2::PixelFromRGB( r, g, b ) );
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
        }

        sy+= dy;
        eC_Int ty= eC_FixToInt(sy);

        A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
        oy= ty;

        A2::IncY( pkDest, kDstLine );
     }
  }





  // Generic stretch color blitter (with & without bilinear filtering).
  //
  static inline void StretchBlit_IntCol(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
     eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha)
  {
     eC_Int iHeight;
     eC_Int iWidth;

     eC_Fixed dst_left;
     eC_Fixed dst_top;
     eC_Fixed dst_right;
     eC_Fixed dst_bottom;

     eC_Fixed src_left;
     eC_Fixed src_top;
     eC_Fixed src_right;
     eC_Fixed src_bottom;

     eC_Fixed dx;
     eC_Fixed dy;

     O1 kSrcLine;
     O2 kDstLine;

     eC_Fixed sy;
     eC_Int   oy;

     eC_Bool boolAlpha = ubAlpha!=255;
     // Get Pixel from Color

     P2 pPixel = A2::ColorToPixel( cCol );

     if (!eGML_StretchBlitParam::CalcParams( pkSrc     , 0           , pkDest,
        uiSrcWidth, uiSrcHeight , iSrcX     , iSrcY,
        uiDstWidth, uiDstHeight , iDstX     , iDstY,      ulFlags,
        src_left  , src_top     , src_right , src_bottom,
        dst_left  , dst_top     , dst_right , dst_bottom,
        dx        , dy          ,
        iWidth    , iHeight))
     {
        return;
     }



     A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
     A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );

     sy= src_top;
     oy= eC_FixToInt(src_top);


     for (eC_Int y=0; y < iHeight; y++)
     {
        O1 kSrc = kSrcLine;
        O2 kDst = kDstLine;


        eC_Fixed sx= src_left;
        eC_Int   ox= eC_FixToInt(sx);

        if ((ulFlags&eGML_Bitmap::BLIT_BILINEAR))
        {

           eC_UByte fracy= eC_FixFracToByte(eC_FixFrac(sy));

           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));
                 eC_UByte   ubBlend = A1::AlphaFromPixel(A1::Blend4( pkSrc, kSrc, fracx, fracy ));
                 ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
                 A2::BlendPixel( kDst, pPixel, ubBlend );

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));
                 eC_UByte ubBlend = A1::AlphaFromPixel(A1::Blend4( pkSrc, kSrc, fracx, fracy ));

                 A2::BlendPixelSrcAlpha( kDst, pPixel, ubBlend );

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }

        }
        else //Non filtered loop
        {
           if (boolAlpha) // ubAlpha!=255;
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte ubBlend = (eC_UByte) A1::AlphaFromPixel(A1::GetPixel(kSrc));

                 ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
                 A2::BlendPixel( kDst, pPixel, ubBlend );

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 A2::BlendPixel( kDst, pPixel, A1::AlphaFromPixel(A1::GetPixel(kSrc)));

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }

        }

        sy+= dy;
        eC_Int ty= eC_FixToInt(sy);

        A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
        oy= ty;

        A2::IncY( pkDest, kDstLine );
     }
  }



  static inline void decodePixel(eC_UByte ubAlphas[4], P2 mixedColors[4], eC_UByte c[4], O1 kOffset, eGML_Bitmap *pkAlpha, P2 pPixelOutline, P2 pPixelInside, eC_UByte & flag){
     c[0] = kOffset[0];
     c[1] = kOffset[1];
     c[2] = kOffset[pkAlpha->m_uiPitch];
     c[3] = kOffset[pkAlpha->m_uiPitch+1];


     if ((c[0] | c[1] | c[2] | c[3])>>1  == 0) {
        flag = 0;
        return;
     }

     //if ((c[0] == c[1]) &&  (c[0] == c[2]) && (c[0] == c[3])){
     if (!(	(c[0] ^ c[1]) |  (c[0] ^ c[2]) | (c[0] ^ c[3]) )>>1) {
        flag = (c[0] & FLAG_BIT_MASK);
        ubAlphas[0] = c[0] | (( c[0] & 224) >> 5);
        return;
     }

     // BlendColors:
     // we cannot just blend the alphas of a 5bit / 2 bit encoded font
     // 1. get the decoded alphas
     // 2. calc the 2color mixtures
     // 3. filter the 4 alpha values and 4 2-Color colors
     // 4. Blend the result with the background color
     // ScreenAt(x,y) = Blend( FilterBilinear(c[0],c[1],c[2],c[3]),FilterBilinear(a[0],a[1],a[2],a[3]))
     // c[x] =  DecodeFontmapAndBlendOutlineColorAndInsideColor(x,y, OutlineColor, InsideColor)
     // a[x] =  DecodeFontmap(x,y)
     // a[x] = real alpha for blitting with destination bitmap
     // The fontmapvalues are used as blend values for the Outline-Color and Inside-Color.
     // The real alpha values are:
     // alpha = FontMapAt(OUTSIDE) = 0 // full transparent
     // alpha = FontMapAt(INSIDE) = 255 // full opaque
     // alpha = FontMapAt(OUTLINE) = [0..255] // fade in
     // alpha = FontMapAt(BLEND) = 255 // Outline * (255 - Blend) + Inside * Blend


     for (int i_ = 0; i_ < 4; i_++) {

        switch (c[i_] & FLAG_BIT_MASK )
        {
           // least probability
        case OUTLINE_BIT_MASK:
           ubAlphas[i_] = c[i_] | (( c[i_] & 224) >> 5);
           mixedColors[i_] = pPixelOutline;// 5Bit to 8Bit conversion
           break;
           // most probabilty
        case INSIDE_BIT_MASK:
           ubAlphas[i_] = eC_BLEND_MAX;
           mixedColors[i_] = pPixelInside; // alpha is MAX, since we are inside
           break;
           // outside
        case BIT_BLEND_MASK:
           ubAlphas[i_] = eC_BLEND_MAX;
           mixedColors[i_] = A2::MixLinear( pPixelOutline, pPixelInside ,c[i_] | (( c[i_] & 224) >> 5)); // alpha is MAX, since both colors are used
           break;
        default:
           ubAlphas[i_] = 0;
           mixedColors[i_] = 0;
           ;
        }
     }

     flag = BIT_BLEND_MASK;
  }

  static inline void decodePixelSimpleUBAlpha(O1 kTexOffset, O2 kDst, P2 pxColorOutline, P2 pxColorInside, eC_UByte ubAlpha) {
     eC_UByte uRawBlend = A1::GetRawPixel( kTexOffset );
     if (uRawBlend>>1 == 0) return;
     switch (uRawBlend & FLAG_BIT_MASK)
     {
        // least probability
     case OUTLINE_BIT_MASK:
        A2::BlendPixel( kDst, pxColorOutline, ((uRawBlend | (( uRawBlend & 224) >> 5)) * ubAlpha) >> 8 );// 5Bit to 8Bit conversion
        break;
        // most probabilty
     case INSIDE_BIT_MASK:
        A2::BlendPixel( kDst, pxColorInside, (eC_BLEND_MAX * ubAlpha) >> 8); // alpha is MAX, since we are inside
        break;
        // outside
     case BIT_BLEND_MASK:
        uRawBlend |= (( uRawBlend & 224) >> 5);
        A2::BlendPixel(kDst, A2::MixLinear(pxColorOutline,pxColorInside,uRawBlend),(eC_BLEND_MAX * ubAlpha) >> 8); // alpha is MAX, since both colors are used
        break;
     default:
        ;
     }
  }

  static inline void decodePixelSimple(O1 kTexOffset, O2 kDst, P2 pxColorOutline, P2 pxColorInside) {
     eC_UByte uRawBlend = A1::GetRawPixel( kTexOffset );
     if (uRawBlend>>1 == 0) return;
     switch (uRawBlend & FLAG_BIT_MASK)
     {
        // least probability
     case OUTLINE_BIT_MASK:
        A2::BlendPixel( kDst, pxColorOutline, (uRawBlend | (( uRawBlend & 224) >> 5)));// 5Bit to 8Bit conversion
        break;
        // most probabilty
     case INSIDE_BIT_MASK:
        A2::BlendPixel( kDst, pxColorInside, eC_BLEND_MAX ); // alpha is MAX, since we are inside
        break;
        // outside
     case BIT_BLEND_MASK:
        uRawBlend |= (( uRawBlend & 224) >> 5);
        A2::BlendPixel(kDst, A2::MixLinear(pxColorOutline,pxColorInside,uRawBlend),eC_BLEND_MAX) ; // alpha is MAX, since both colors are used
        break;
     default:
        ;
     }
  }

  static inline void StretchBlit_Int2Col(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDest,
     eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2,  eC_Flag ulFlags, eC_UByte ubAlpha)
  {
     eC_Int iHeight;
     eC_Int iWidth;

     eC_Fixed dst_left;
     eC_Fixed dst_top;
     eC_Fixed dst_right;
     eC_Fixed dst_bottom;

     eC_Fixed src_left;
     eC_Fixed src_top;
     eC_Fixed src_right;
     eC_Fixed src_bottom;

     eC_Fixed dx;
     eC_Fixed dy;

     O1 kSrcLine;
     O2 kDstLine;

     eC_Fixed sy;
     eC_Int   oy;

     eC_Bool boolAlpha = ubAlpha!=255;
     // Get Pixel from Color

     P2 pPixelOutline = A2::ColorToPixel( cCol1 );
     P2 pPixelInside = A2::ColorToPixel( cCol2 );

     if (!eGML_StretchBlitParam::CalcParams( pkSrc     , 0           , pkDest,
        uiSrcWidth, uiSrcHeight , iSrcX     , iSrcY,
        uiDstWidth, uiDstHeight , iDstX     , iDstY,      ulFlags,
        src_left  , src_top     , src_right , src_bottom,
        dst_left  , dst_top     , dst_right , dst_bottom,
        dx        , dy          ,
        iWidth    , iHeight))
     {
        return;
     }



     A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
     A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );

     sy= src_top;
     oy= eC_FixToInt(src_top);


     for (eC_Int y=0; y < iHeight; y++)
     {
        O1 kSrc = kSrcLine;
        O2 kDst = kDstLine;


        eC_Fixed sx= src_left;
        eC_Int   ox= eC_FixToInt(sx);

        if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
        {

           eC_UByte fracy= eC_FixFracToByte(eC_FixFrac(sy));

           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));


                 eC_UByte c[4];
                 eC_UByte ubAlphas[4];
                 P2 mixedColors[4];
                 eC_UByte flag = 0;

                 decodePixel(ubAlphas, mixedColors, c, kSrc, pkSrc, pPixelOutline, pPixelInside, flag);


                 switch (flag)
                 {
                 case BIT_BLEND_MASK:
                    {
                       // and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
                       A2::BlendPixel( kDst, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracx, fracy), (A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracx, fracy)*ubAlpha)>>8 );
                       break;
                    }
                 case INSIDE_BIT_MASK:
                    {
                       A2::BlendPixel( kDst, pPixelInside, ubAlpha);
                       break;
                    }
                 case OUTLINE_BIT_MASK:
                    {
                       A2::BlendPixel( kDst, pPixelOutline,(ubAlphas[0]*ubAlpha)>>8);
                       break;
                    }
                 }

                 // now use bilinearfilterinig for the alphas
                 //eC_UByte ubBlend = (A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracx, fracy)*ubAlpha)>>8;

                 // and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
                 //A2::BlendPixel( kDst, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracx, fracy), ubBlend );


                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

                 eC_UByte c[4];
                 eC_UByte ubAlphas[4];
                 P2 mixedColors[4];
                 eC_UByte flag = 0;

                 decodePixel(ubAlphas, mixedColors, c, kSrc, pkSrc, pPixelOutline, pPixelInside, flag);


                 switch (flag)
                 {
                 case BIT_BLEND_MASK:
                    {
                       // and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
                       A2::BlendPixel( kDst, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracx, fracy), A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracx, fracy));
                       break;
                    }
                 case INSIDE_BIT_MASK:
                    {
                       A2::BlendPixel( kDst, pPixelInside, ubAlpha);
                       break;
                    }
                 case OUTLINE_BIT_MASK:
                    {
                       A2::BlendPixel( kDst, pPixelOutline,ubAlphas[0]);
                       break;
                    }
                 }

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }

        }
        else //Non filtered loop
        {
           if (boolAlpha)
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;
                 //eC_UByte ubBlend = A1::AlphaFromPixel(A1::GetPixel(kSrc));

                 decodePixelSimpleUBAlpha(kSrc, kDst, pPixelOutline, pPixelInside, ubAlpha);

                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }
           else
           {
              for (eC_Int x=0; x < iWidth; x++)
              {
                 eC_Int   tx;

                 decodePixelSimple(kSrc, kDst, pPixelOutline, pPixelInside);


                 sx+= dx;
                 tx= eC_FixToInt(sx);

                 A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );

                 ox= tx;

                 A2::IncX( pkDest, kDst );
              }
           }

        }

        sy+= dy;
        eC_Int ty= eC_FixToInt(sy);

        A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
        oy= ty;

        A2::IncY( pkDest, kDstLine );
     }
  }

};






template <
typename P1,		// Pixeltype for Source
typename O1,		// Offsettype for Source
class A1,			// Accessmethods for Source

typename P2,		// Pixeltype for Destination
typename O2,		// Offsettype for Destination
class A2,			// Accessmethods for Destination

typename P3,		// Pixeltype for Alpha
typename O3,		// Offsettype for Alpha
class A3			// Accessmethods for Alpha

>

class eGML_StretchBlitAlpha
{

public:

  static inline void StretchBlit_IntAlpha(eGML_Bitmap *pkSrc, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
    eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha)
  {
    eC_Int iHeight;
    eC_Int iWidth;

    eC_Fixed dst_left;
    eC_Fixed dst_top;
    eC_Fixed dst_right;
    eC_Fixed dst_bottom;

    eC_Fixed src_left;
    eC_Fixed src_top;
    eC_Fixed src_right;
    eC_Fixed src_bottom;

    eC_Fixed dx;
    eC_Fixed dy;

    O1 kSrcLine;
    O2 kDstLine;
    O3 kAlphaLine;

    eC_Fixed sy;
    eC_Int   oy;

    eC_Bool boolAlpha = ubAlpha!=255;


    if (!eGML_StretchBlitParam::CalcParams( pkSrc     , pkAlpha     , pkDest,
      uiSrcWidth, uiSrcHeight ,  iSrcX     , iSrcY,
      uiDstWidth, uiDstHeight ,  iDstX     , iDstY,      ulFlags,
      src_left  , src_top     ,  src_right , src_bottom,
      dst_left  , dst_top     ,  dst_right , dst_bottom,
      dx        , dy          ,
      iWidth    , iHeight))
    {
      return;
    }



    A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
    A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );
    A3::GetOffset( pkAlpha, eC_FixToInt(src_left), eC_FixToInt(src_top), kAlphaLine );

    sy= src_top;
    oy= eC_FixToInt(src_top);


    for (eC_Int y=0; y < iHeight; y++)
    {
      O1 kSrc = kSrcLine;
      O2 kDst = kDstLine;
      O3 kAlpha = kAlphaLine;


      eC_Fixed sx= src_left;
      eC_Int   ox= eC_FixToInt(sx);

      if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
      {

        eC_UByte fracy= eC_FixFracToByte(eC_FixFrac(sy));

        if (boolAlpha)
        {


          for (eC_Int x=0; x < iWidth; x++)
          {
            eC_Int   tx;

            eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));
            eC_UByte   ubBlend = A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlpha, fracx, fracy));

            ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);

            A2::BlendPixel( kDst, A1::Blend4( pkSrc, kSrc, fracx, fracy ), ubBlend );

            sx+= dx;
            tx= eC_FixToInt(sx);

            A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
            A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

            ox= tx;

            A2::IncX( pkDest, kDst );
          }
        }
        else
        {
          for (eC_Int x=0; x < iWidth; x++)
          {
            eC_Int   tx;

            eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

            A2::BlendPixel(
               kDst, A1::Blend4( pkSrc, kSrc, fracx, fracy ),
               A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlpha, fracx, fracy))
               );

            sx+= dx;
            tx= eC_FixToInt(sx);

            A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
            A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

            ox= tx;

            A2::IncX( pkDest, kDst );
          }
        }

      }
      else //Non filtered loop
      {
        if (boolAlpha)
        {
          for (eC_Int x=0; x < iWidth; x++)
          {
            eC_Int   tx;
            eC_UByte ubBlend = A3::AlphaFromPixel(A3::GetPixel( kAlpha));
            ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
            A2::BlendPixel( kDst, A1::GetPixel( kSrc),  ubBlend );

            sx+= dx;
            tx= eC_FixToInt(sx);

            A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
            A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

            ox= tx;

            A2::IncX( pkDest, kDst );
          }
        }
        else
        {
          for (eC_Int x=0; x < iWidth; x++)
          {
            eC_Int   tx;

            A2::BlendPixel( kDst, A1::GetPixel( kSrc),  A3::AlphaFromPixel(A3::GetPixel( kAlpha)));


            sx+= dx;
            tx= eC_FixToInt(sx);

            A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
            A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

            ox= tx;

            A2::IncX( pkDest, kDst );
          }
        }



      }

      sy+= dy;
      eC_Int ty= eC_FixToInt(sy);

      A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
      A3::AddOffset( pkAlpha, kAlphaLine, 0, ty-oy, kAlphaLine );
      oy= ty;

      A2::IncY( pkDest, kDstLine );
    }
    }


  static inline void StretchBlit_RGBAlpha(eGML_Bitmap *pkSrc, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDest,
    eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_Int iSrcX, eC_Int iSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha)
  {
      eC_Int iHeight;
      eC_Int iWidth;

      eC_Fixed dst_left;
      eC_Fixed dst_top;
      eC_Fixed dst_right;
      eC_Fixed dst_bottom;

      eC_Fixed src_left;
      eC_Fixed src_top;
      eC_Fixed src_right;
      eC_Fixed src_bottom;

      eC_Fixed dx;
      eC_Fixed dy;

      O1 kSrcLine;
      O2 kDstLine;
      O3 kAlphaLine;

      eC_Fixed sy;
      eC_Int   oy;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_StretchBlitParam::CalcParams( pkSrc     , pkAlpha     , pkDest,
        uiSrcWidth, uiSrcHeight ,  iSrcX     , iSrcY,
        uiDstWidth, uiDstHeight ,  iDstX     , iDstY,      ulFlags,
        src_left  , src_top     ,  src_right , src_bottom,
        dst_left  , dst_top     ,  dst_right , dst_bottom,
        dx        , dy          ,
        iWidth    , iHeight))
      {
        return;
      }



      A1::GetOffset( pkSrc  , eC_FixToInt(src_left), eC_FixToInt(src_top), kSrcLine );
      A2::GetOffset( pkDest , eC_FixToInt(dst_left), eC_FixToInt(dst_top), kDstLine );
      A3::GetOffset( pkAlpha, eC_FixToInt(src_left), eC_FixToInt(src_top), kAlphaLine );

      sy= src_top;
      oy= eC_FixToInt(src_top);


      for (eC_Int y=0; y < iHeight; y++)
      {
        O1 kSrc = kSrcLine;
        O2 kDst = kDstLine;
        O3 kAlpha = kAlphaLine;


        eC_Fixed sx= src_left;
        eC_Int   ox= eC_FixToInt(sx);

        if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
        {

          eC_UByte fracy= eC_FixFracToByte(eC_FixFrac(sy));
          if (boolAlpha)
          {

            for (eC_Int x=0; x < iWidth; x++)
            {
              eC_Int   tx;
              eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

              P1 pxPixel   = A1::Blend4( pkSrc, kSrc, fracx, fracy );
              eC_UByte r   = A1::RedFromPixel( pxPixel );
              eC_UByte g   = A1::GreenFromPixel( pxPixel );
              eC_UByte b   = A1::BlueFromPixel( pxPixel );

              // (todo) check compiler optimizations, maybe add Blend4Alpha() ?
              eC_UByte ubBlend = A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlpha, fracx, fracy));
              ubBlend = (ubBlend*ubAlpha)>>8;
              A2::BlendPixel( kDst, A2::PixelFromRGB( r, g, b ), ubBlend );

              sx+= dx;
              tx= eC_FixToInt(sx);

              A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
              A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

              ox= tx;

              A2::IncX( pkDest, kDst );
            }
          }
          else
          {

            for (eC_Int x=0; x < iWidth; x++)
            {
              eC_Int   tx;
              eC_UByte   fracx= eC_FixFracToByte(eC_FixFrac(sx));

              P1 pxPixel   = A1::Blend4( pkSrc, kSrc, fracx, fracy );
              eC_UByte r   = A1::RedFromPixel( pxPixel );
              eC_UByte g   = A1::GreenFromPixel( pxPixel );
              eC_UByte b   = A1::BlueFromPixel( pxPixel );

              A2::BlendPixel(
                 kDst, A2::PixelFromRGB( r, g, b ),
                 A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlpha, fracx, fracy))
                 );

              sx+= dx;
              tx= eC_FixToInt(sx);

              A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
              A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

              ox= tx;

              A2::IncX( pkDest, kDst );
            }
          }

        }
        else //Non filtered loop
        {
          if (boolAlpha)
          {
            for (eC_Int x=0; x < iWidth; x++)
            {
              eC_Int   tx;

              P1 pxPixel  = A1::GetPixel( kSrc );
              eC_UByte r = A1::RedFromPixel( pxPixel );
              eC_UByte g = A1::GreenFromPixel( pxPixel );
              eC_UByte b = A1::BlueFromPixel( pxPixel );

              eC_UByte ubBlend = A3::AlphaFromPixel(A3::GetPixel( kAlpha));
              ubBlend = (ubBlend*ubAlpha)>>8;

              A2::BlendPixel( kDst, A2::PixelFromRGB( r, g, b ),  ubBlend );

              sx+= dx;
              tx= eC_FixToInt(sx);

              A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
              A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

              ox= tx;

              A2::IncX( pkDest, kDst );
            }
          }
          else
          {
            for (eC_Int x=0; x < iWidth; x++)
            {
               eC_Int   tx;

               P1 pxPixel  = A1::GetPixel( kSrc );
               eC_UByte r = A1::RedFromPixel( pxPixel );
               eC_UByte g = A1::GreenFromPixel( pxPixel );
               eC_UByte b = A1::BlueFromPixel( pxPixel );

               A2::BlendPixel( kDst, A2::PixelFromRGB( r, g, b ),  A3::AlphaFromPixel(A3::GetPixel( kAlpha)));

               sx+= dx;
               tx= eC_FixToInt(sx);

               A1::AddOffset( pkSrc, kSrc, tx-ox, 0, kSrc );
               A3::AddOffset( pkAlpha, kAlpha, tx-ox, 0, kAlpha );

               ox= tx;

               A2::IncX( pkDest, kDst );
            }
          }

        }

        sy+= dy;
        eC_Int ty= eC_FixToInt(sy);

        A1::AddOffset( pkSrc, kSrcLine, 0, ty-oy, kSrcLine );
        A3::AddOffset( pkAlpha, kAlphaLine, 0, ty-oy, kAlphaLine );
        oy= ty;

        A2::IncY( pkDest, kDstLine );
      }
    }

};

//---------------------------------------------------------------
#endif

