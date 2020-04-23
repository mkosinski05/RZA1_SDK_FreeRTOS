/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_RLECOmpressedBlits.h
* %version:        150.0 %
*
* author:          hh04030
* creation date:   2014, Mar 6th
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------

******************************************************************************/


#if defined( eGML_USEBITMAPRGB32RLE8 ) || defined ( eGML_USEBITMAPRGB32RLE16 )


#include "eGML_PrecompiledImageInfo.h"


#define eGMLRLEComprBlits_MulABDiv255(a,b)  eC_UByte( (                                    \
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

class eGML_RLECompressedBlits
{
public:

   //---------------------------------------------------------------
   // Blit directly fom a compressed Bitmap to RGB32 Bitmap
   //
   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void Blit_ARGB32RLE16(eGML_Bitmap *pkSourceRGB32RLE16, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, false, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       else
       {
           Blit_ARGB32RLE<true, false, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       pkSourceRGB32RLE16->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void Blit_ARGB32RLE8(eGML_Bitmap *pkSourceRGB32RLE8, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, false, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       else
       {
           Blit_ARGB32RLE<true, false, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       pkSourceRGB32RLE8->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitAlpha_ARGB32RLE16(eGML_Bitmap *pkSourceRGB32RLE16, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, true, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       else
       {
           Blit_ARGB32RLE<true, true, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       pkSourceRGB32RLE16->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitAlpha_ARGB32RLE8(eGML_Bitmap *pkSourceRGB32RLE8, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, true, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       else
       {
           Blit_ARGB32RLE<true, true, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, 1, 1);
       }
       pkSourceRGB32RLE8->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitMag_ARGB32RLE16(eGML_Bitmap *pkSourceRGB32RLE16, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, false, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       else
       {
           Blit_ARGB32RLE<true, false, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       pkSourceRGB32RLE16->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitMag_ARGB32RLE8(eGML_Bitmap *pkSourceRGB32RLE8, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, false, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       else
       {
           Blit_ARGB32RLE<true, false, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       pkSourceRGB32RLE8->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitMagAlpha_ARGB32RLE16(eGML_Bitmap *pkSourceRGB32RLE16, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, true, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       else
       {
           Blit_ARGB32RLE<true, true, RLE16CurDecompressInfo, eC_UShort, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE16->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       pkSourceRGB32RLE16->Unlock();
   }

   template
   <
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void BlitMagAlpha_ARGB32RLE8(eGML_Bitmap *pkSourceRGB32RLE8, eGML_Bitmap *pkDestRGB32,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
       if(255 == ubAlpha)
       {
           Blit_ARGB32RLE<false, true, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       else
       {
           Blit_ARGB32RLE<true, true, RLE8CurDecompressInfo, eC_UByte, P2, O2, A2>((eGML_PrecompiledImageInfo *)pkSourceRGB32RLE8->Lock(), pkDestRGB32, uiWidth, uiHeight, uiSrcX, uiSrcY, iDstX, iDstY, ubAlpha, usXMag, usYMag);
       }
       pkSourceRGB32RLE8->Unlock();
   }

   template
   <
      bool tpl_bApplyGlobalAlpha, // Apply Global alpha on output Pixelcolor
      bool tpl_bBlendPixels,      // Use AlphaByte of ARGB32 Pixel to Blend output pixelcolor
      class RLECurDecompressInfo, class RLEDataType,
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void Blit_ARGB32RLE(eGML_PrecompiledImageInfo * pstRLECompressedInfo, eGML_Bitmap *pkDest,
      eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                         eC_Int   iDstX, eC_Int   iDstY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
      // begin at the beginning of the stream
      RLECurDecompressInfo myRLECurDecompressInfo;
      myRLECurDecompressInfo.pubyCurRLEDataByte = (RLEDataType *)pstRLECompressedInfo->GetRLEDataBytes();
      myRLECurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_GETNEXT;
      myRLECurDecompressInfo.CurrentTokenDataposition = 0;
      myRLECurDecompressInfo.CurrentTokenDatacount = 0;

      // Get src & dst rectangle koords
      eC_Int src_left  = (uiSrcX);
      eC_Int src_right = (uiSrcX + uiWidth - 1);
      eC_Int src_top   = (uiSrcY);
      eC_Int src_bottom= (uiSrcY + uiHeight - 1);

      eC_Int dst_left  = (iDstX);
      eC_Int dst_right = (iDstX + (usXMag * uiWidth) - 1);
      eC_Int dst_top   = (iDstY);
      eC_Int dst_bottom= (iDstY + (usYMag * uiHeight) - 1);

      // Get src & dst clipping boundaries
      eC_Int src_minx= 0;
      eC_Int src_miny= 0;
      eC_Int src_maxx= pstRLECompressedInfo->uiWidth - 1;
      eC_Int src_maxy= pstRLECompressedInfo->uiHeight - 1;

      eC_Int dst_minx= pkDest->GetClipMinX();
      eC_Int dst_miny= pkDest->GetClipMinY();
      eC_Int dst_maxx= pkDest->GetClipMaxX();
      eC_Int dst_maxy= pkDest->GetClipMaxY();


      // Trivial Reject
      if (src_left  < src_minx || src_right >src_maxx ||
          src_top   < src_miny || src_bottom>src_maxy ||
          dst_right < dst_minx || dst_left  >dst_maxx ||
          dst_bottom< dst_miny || dst_top   >dst_maxy)
      {
          return;
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // X
      eC_UShort usFirstXWidth = 0;
      eC_UShort usLastXWidth = 0;
      eC_UInt uiClippedPixelsX = 0;

      // Clip Dst Left
      if (dst_left<dst_minx)
      {
          eC_Int    iCompletePixelsOutSideLeft = (dst_minx - dst_left) / usXMag;
          eC_UShort usClippedPixelsOutsideLeft = (eC_UShort)(((dst_minx - dst_left) - iCompletePixelsOutSideLeft * usXMag));
          if (usClippedPixelsOutsideLeft > 0)
          {
              usFirstXWidth = usXMag - usClippedPixelsOutsideLeft;
              ++uiClippedPixelsX;
          }
          // Calculate new src_startx
          src_left += iCompletePixelsOutSideLeft;
          dst_left = dst_minx;

          // Cut ScrWidth as well
          uiWidth  -= iCompletePixelsOutSideLeft;
      }

      // Clip Dst Right
      if (dst_right>dst_maxx)
      {
          eC_Int    iCompletePixelsOutSideRight = (dst_right - dst_maxx) / usXMag;
          // two possibilities
          // 1.) there is only ONE pixelpart, which is covered by FirstPixel
          if ((dst_left + usFirstXWidth) > dst_maxx)
          {
              // The right clippingborder has to be used
              usFirstXWidth = (eC_UShort)(1 + dst_maxx - dst_left);
          }
          else
          {
              // 2.) clip a second pixel
              eC_UShort usClippedPixelsOutsideRight = (eC_UShort)((dst_right - dst_maxx - (iCompletePixelsOutSideRight * usXMag)));
              if (usClippedPixelsOutsideRight > 0)
              {
                  usLastXWidth = usXMag - usClippedPixelsOutsideRight;
                  ++uiClippedPixelsX;
              }
          }
          // Calculate new src_startx
          src_right -= iCompletePixelsOutSideRight;
          dst_right = dst_maxx;
          // Set remaining complete Pixels first
          uiWidth  -= iCompletePixelsOutSideRight;
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // Y
      eC_UShort usFirstYHeight = 0;
      eC_UShort usLastYHeight = 0;
      eC_UInt uiClippedPixelsY = 0;
      // Clip Dst Top
      if (dst_top<dst_miny)
      {
          eC_Int    iCompletePixelsOutSideTop = (dst_miny - dst_top) / usYMag;
          eC_UShort usClippedPixelsOutsideTop = (eC_UShort)(((dst_miny - dst_top) - iCompletePixelsOutSideTop * usYMag));
          if (usClippedPixelsOutsideTop > 0)
          {
              usFirstYHeight = usYMag - usClippedPixelsOutsideTop;
              ++uiClippedPixelsY;
          }

          // Calculate new src_startx
          src_top += iCompletePixelsOutSideTop;
          dst_top = dst_miny;

          // Cut ScrHeight as well
          uiHeight  -= iCompletePixelsOutSideTop;
      }

      // Clip Dst Bottom
      if (dst_bottom>dst_maxy)
      {
          eC_Int    iCompletePixelsOutSideBottom = (dst_bottom - dst_maxy) / usYMag;
          // two possibilities
          // 1.) there is only ONE pixelpart, which is covered by FirstPixel
          if ((dst_top + usFirstYHeight) > dst_maxy)
          {
              // The bottom clippingborder has to be used
              usFirstYHeight = (eC_UShort)(1 + dst_maxy - dst_top);
          }
          else
          {
              // 2.) clip a second pixel
              eC_UShort usClippedPixelsOutsideBottom = (eC_UShort)(dst_bottom - dst_maxy - (iCompletePixelsOutSideBottom * usYMag));
              if (usClippedPixelsOutsideBottom > 0)
              {
                  usLastYHeight = usYMag - usClippedPixelsOutsideBottom;
                  ++uiClippedPixelsY;
              }
          }

          // Calculate new src_startx
          src_bottom -= iCompletePixelsOutSideBottom;
          dst_bottom = dst_maxy;

          // Cut ScrHeight as well
          uiHeight  -= iCompletePixelsOutSideBottom;
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // iHeight / iWidth is destination pixel base
      eC_Int iWidth = ((dst_right  - dst_left)+1);
      eC_Int iHeight= ((dst_bottom - dst_top )+1);

      // Check parameters for safety.
      if (iHeight<=0 || iWidth<=0)
      {
          return;
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // use calculated values
      uiSrcX = src_left;
      uiSrcY = src_top;
       iDstX = dst_left;
       iDstY = dst_top;

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // skip leading pixel before blitting first row
      SkipPixels(uiSrcY * pstRLECompressedInfo->uiWidth + uiSrcX, myRLECurDecompressInfo);

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // First Row, Y-clipped
      // now write pixels of last line, probably y-clipped
      if (usFirstYHeight > 0)
      {
          if (usFirstXWidth > 0)
          {
              // write the first Pixel probably clipped
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX, iDstY, ubAlpha, usFirstXWidth, usFirstYHeight);
          }
          WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(uiWidth - uiClippedPixelsX, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth, iDstY, ubAlpha, usXMag, usFirstYHeight);
          // write the last Pixel probably clipped
          if (usLastXWidth > 0)
          {
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth + (uiWidth - uiClippedPixelsX) * usXMag, iDstY, ubAlpha, usLastXWidth, usFirstYHeight);
          }
          // skip to next line
          SkipPixels(pstRLECompressedInfo->uiWidth - uiWidth, myRLECurDecompressInfo);
      }
      // ------------------------------------------------------------------------------------------------------------------------------------------
      // Complete Y Size, work on n-(yclipped) rows
      eC_Int y = iDstY + usFirstYHeight;
      for (eC_UInt rw = 0; rw < (uiHeight - uiClippedPixelsY); rw++)
      {
          // write the first Pixel probably clipped
          if (usFirstXWidth > 0)
          {
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX, y, ubAlpha, usFirstXWidth, usYMag);
          }
          // decompress and write line pixels
          WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(uiWidth - uiClippedPixelsX, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth, y, ubAlpha, usXMag, usYMag);
          // write the last Pixel probably clipped
          if (usLastXWidth > 0)
          {
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth + (uiWidth - uiClippedPixelsX) * usXMag, y, ubAlpha, usLastXWidth, usYMag);
          }
          // skip to next line
          SkipPixels(pstRLECompressedInfo->uiWidth - uiWidth, myRLECurDecompressInfo);
          y += usYMag;
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------
      // now write pixels of last line, probably y-clipped
      if (usLastYHeight > 0)
      {
          if (usFirstXWidth > 0)
          {
              // write the first Pixel probably clipped
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX, y, ubAlpha, usFirstXWidth, usLastYHeight);
          }
          WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(uiWidth - uiClippedPixelsX, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth, y, ubAlpha, usXMag, usLastYHeight);
          // write the last Pixel probably clipped
          if (usLastXWidth > 0)
          {
              WritePixels<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, RLECurDecompressInfo, P2, O2, A2>(1, myRLECurDecompressInfo, pstRLECompressedInfo, pkDest, iDstX + usFirstXWidth + (uiWidth - uiClippedPixelsX) * usXMag, y, ubAlpha, usLastXWidth, usLastYHeight);
          }
      }
   }

   template<class RLECurDecompressInfo>
   static inline void SkipPixels(eC_UInt uiCount, RLECurDecompressInfo & CurDecompressInfo)
   {
      if (RLECURDECOMPRESSTOKEN_END != CurDecompressInfo.eCurrentToken)
      {
         while (uiCount)
         {
            // if the last tag is done
            if (RLECURDECOMPRESSTOKEN_GETNEXT == CurDecompressInfo.eCurrentToken)
            {
               // get a new tag
               if (0 == *CurDecompressInfo.pubyCurRLEDataByte)
               {
                  ++CurDecompressInfo.pubyCurRLEDataByte;
                  // did we found the end
                  if (0 == *CurDecompressInfo.pubyCurRLEDataByte)
                  {
                     CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_END;
                     CurDecompressInfo.CurrentTokenDataposition = 0;
                     CurDecompressInfo.CurrentTokenDatacount = 0;
                     // abort loop
                     uiCount = 0;
                  }
                  else
                  {
                     // ABS
                     CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_ABS;
                     CurDecompressInfo.CurrentTokenDataposition = 0;
                     CurDecompressInfo.CurrentTokenDatacount = *CurDecompressInfo.pubyCurRLEDataByte;
                     ++CurDecompressInfo.pubyCurRLEDataByte;
                  }
               }
               else
               {
                  // RLE
                  CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_RLE;
                  CurDecompressInfo.CurrentTokenDataposition = 0;
                  CurDecompressInfo.CurrentTokenDatacount = *CurDecompressInfo.pubyCurRLEDataByte;
                  ++CurDecompressInfo.pubyCurRLEDataByte;
               }
            }
            if (RLECURDECOMPRESSTOKEN_END != CurDecompressInfo.eCurrentToken)
            {
               // now reduce count for the amount of bytes in this tag
               eC_UInt uiTagLength = (CurDecompressInfo.CurrentTokenDatacount - CurDecompressInfo.CurrentTokenDataposition);
               if (uiCount >= uiTagLength)
               {
                  uiCount -= uiTagLength;
                  // skip to next token
                  switch (CurDecompressInfo.eCurrentToken)
                  {
                  case RLECURDECOMPRESSTOKEN_ABS:
                     // skip color array
                     CurDecompressInfo.pubyCurRLEDataByte += CurDecompressInfo.CurrentTokenDatacount;
                     break;
                  case RLECURDECOMPRESSTOKEN_RLE:
                     // skip color
                     ++CurDecompressInfo.pubyCurRLEDataByte;
                     break;
                  default:
                     // should not happen
                     break;
                  }
                  CurDecompressInfo.CurrentTokenDataposition = 0;
                  CurDecompressInfo.CurrentTokenDatacount = 0;
                  CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_GETNEXT;
               }
               else
               {
                  CurDecompressInfo.CurrentTokenDataposition += uiCount;
                  uiCount = 0;
               }
            } // ! END
         } // while uiCount
      } // ! END
   }

   template
   <
      bool tpl_bApplyGlobalAlpha, // Apply Global alpha on output Pixelcolor
      bool tpl_bBlendPixels,      // Use AlphaByte of ARGB32 Pixel to Blend output pixelcolor
      class RLECurDecompressInfo,
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline void WritePixels(eC_UInt uiCount, RLECurDecompressInfo & CurDecompressInfo, const eGML_PrecompiledImageInfo * pstRLECompressedInfo, eGML_Bitmap *pkDest, eC_UInt uiX, eC_UInt uiY, eC_UByte ubAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
      O2 kDstLine;
      A2::GetOffset( pkDest, uiX, uiY, kDstLine );

      if (RLECURDECOMPRESSTOKEN_END != CurDecompressInfo.eCurrentToken)
      {
         while (uiCount)
         {
            // if the last tag is done
            if (RLECURDECOMPRESSTOKEN_GETNEXT == CurDecompressInfo.eCurrentToken)
            {
               // get a new tag
               if (0 == *CurDecompressInfo.pubyCurRLEDataByte)
               {
                  ++CurDecompressInfo.pubyCurRLEDataByte;
                  // did we found the end
                  if (0 == *CurDecompressInfo.pubyCurRLEDataByte)
                  {
                     CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_END;
                     CurDecompressInfo.CurrentTokenDataposition = 0;
                     CurDecompressInfo.CurrentTokenDatacount = 0;
                     // abort loop
                     uiCount = 0;
                  }
                  else
                  {
                     // ABS
                     CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_ABS;
                     CurDecompressInfo.CurrentTokenDataposition = 0;
                     CurDecompressInfo.CurrentTokenDatacount = *CurDecompressInfo.pubyCurRLEDataByte;
                     ++CurDecompressInfo.pubyCurRLEDataByte;
                  }
               }
               else
               {
                  // RLE
                  CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_RLE;
                  CurDecompressInfo.CurrentTokenDataposition = 0;
                  CurDecompressInfo.CurrentTokenDatacount = *CurDecompressInfo.pubyCurRLEDataByte;
                  ++CurDecompressInfo.pubyCurRLEDataByte;
               }
            }
            if (RLECURDECOMPRESSTOKEN_END != CurDecompressInfo.eCurrentToken)
            {
               // now reduce count for the amount of bytes in this tag
               eC_UInt uiTagLength = (CurDecompressInfo.CurrentTokenDatacount - CurDecompressInfo.CurrentTokenDataposition);
               if (uiCount >= uiTagLength)
               {
                  uiCount -= uiTagLength;
                  // skip to next token
                  switch (CurDecompressInfo.eCurrentToken)
                  {
                  case RLECURDECOMPRESSTOKEN_ABS:
                     // copy color array
                     for (eC_UInt i = CurDecompressInfo.CurrentTokenDataposition; i < CurDecompressInfo.CurrentTokenDatacount; i++)
                     {
                        eGML_Color cPixelColor = pstRLECompressedInfo->acolColorPalette[CurDecompressInfo.pubyCurRLEDataByte[i]];
                        eC_UByte   ubPixelAlpha = (eC_UByte)(cPixelColor >> 24);
                        P2 pxColor = A2::PixelFromRGB((eC_UByte)((cPixelColor >> 16) & 0xFF), (eC_UByte)((cPixelColor >> 8) & 0xFF), (eC_UByte)(cPixelColor & 0xFF));
                        kDstLine = WritePixel<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, P2, O2, A2>(pkDest, kDstLine, pxColor, ubPixelAlpha, ubAlpha, usXMag, usYMag);
                     }
                     CurDecompressInfo.pubyCurRLEDataByte += CurDecompressInfo.CurrentTokenDatacount;
                     break;
                  case RLECURDECOMPRESSTOKEN_RLE:
                     {
                        // copy color array
                        eGML_Color cPixelColor = pstRLECompressedInfo->acolColorPalette[CurDecompressInfo.pubyCurRLEDataByte[0]];
                        eC_UByte   ubPixelAlpha = (eC_UByte)(cPixelColor >> 24);
                        P2 pxColor = A2::PixelFromRGB((eC_UByte)((cPixelColor >> 16) & 0xFF), (eC_UByte)((cPixelColor >> 8) & 0xFF), (eC_UByte)(cPixelColor & 0xFF));
                        for (eC_UInt i = CurDecompressInfo.CurrentTokenDataposition; i < CurDecompressInfo.CurrentTokenDatacount; i++)
                        {
                            kDstLine = WritePixel<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, P2, O2, A2>(pkDest, kDstLine, pxColor, ubPixelAlpha, ubAlpha, usXMag, usYMag);
                        }
                        // goto next token
                        ++CurDecompressInfo.pubyCurRLEDataByte;
                     }
                     break;
                  default:
                     // should not happen
                     break;
                  }
                  CurDecompressInfo.CurrentTokenDataposition = 0;
                  CurDecompressInfo.CurrentTokenDatacount = 0;
                  CurDecompressInfo.eCurrentToken = RLECURDECOMPRESSTOKEN_GETNEXT;
               }
               else
               {
                  switch (CurDecompressInfo.eCurrentToken)
                  {
                  case RLECURDECOMPRESSTOKEN_ABS:
                     // copy color array
                     for (eC_UInt i = CurDecompressInfo.CurrentTokenDataposition; i < CurDecompressInfo.CurrentTokenDataposition + uiCount; i++)
                     {
                        eGML_Color cPixelColor = pstRLECompressedInfo->acolColorPalette[CurDecompressInfo.pubyCurRLEDataByte[i]];
                        eC_UByte   ubPixelAlpha = (eC_UByte)(cPixelColor >> 24);
                        P2 pxColor = A2::PixelFromRGB((eC_UByte)((cPixelColor >> 16) & 0xFF), (eC_UByte)((cPixelColor >> 8) & 0xFF), (eC_UByte)(cPixelColor & 0xFF));
                        kDstLine = WritePixel<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, P2, O2, A2>(pkDest, kDstLine, pxColor, ubPixelAlpha, ubAlpha, usXMag, usYMag);
                     }
                     break;
                  case RLECURDECOMPRESSTOKEN_RLE:
                     {
                        // copy color array
                        eGML_Color cPixelColor = pstRLECompressedInfo->acolColorPalette[CurDecompressInfo.pubyCurRLEDataByte[0]];
                        eC_UByte   ubPixelAlpha = (eC_UByte)(cPixelColor >> 24);
                        P2 pxColor = A2::PixelFromRGB((eC_UByte)((cPixelColor >> 16) & 0xFF), (eC_UByte)((cPixelColor >> 8) & 0xFF), (eC_UByte)(cPixelColor & 0xFF));
                        for (eC_UInt i = CurDecompressInfo.CurrentTokenDataposition; i < CurDecompressInfo.CurrentTokenDataposition + uiCount; i++)
                        {
                            kDstLine = WritePixel<tpl_bApplyGlobalAlpha, tpl_bBlendPixels, P2, O2, A2>(pkDest, kDstLine, pxColor, ubPixelAlpha, ubAlpha, usXMag, usYMag);
                        }
                     }
                     break;
                  default:
                     // should not happen
                     break;
                  }
                  CurDecompressInfo.CurrentTokenDataposition += uiCount;
                  uiCount = 0;
               }
            }
         }
      }
   }

   template
   <
      bool tpl_bApplyGlobalAlpha, // Apply Global alpha on output Pixelcolor
      bool tpl_bBlendPixels,      // Use AlphaByte of ARGB32 Pixel to Blend output pixelcolor
      typename P2, typename O2, class A2             // Pixeltype, Offsettype, Accessmethods for Destination
   >
   static inline O2 WritePixel(eGML_Bitmap *pkDest, O2 kDstLine, P2 BlendColor, eC_UByte ubPixelAlpha, eC_UByte ubGlobalAlpha, eC_UShort usXMag, eC_UShort usYMag)
   {
       // Premultiply (Blend) or put pixel nonmultiplied
       if ((true == tpl_bBlendPixels) || (true == tpl_bApplyGlobalAlpha))
       {
           // Apply Global Alpha, if we have to
           if (true == tpl_bApplyGlobalAlpha)
           {
               ubPixelAlpha = eGMLRLEComprBlits_MulABDiv255(ubPixelAlpha, ubGlobalAlpha);
           }
           // do not blend opaque pixels
           switch (ubPixelAlpha)
           {
           case 0xFF:
               // we draw additional lines using a copy of given offset first
                if (usYMag > 1)
                {
                    usYMag--;
                    O2 kDstLineY = kDstLine;
                    while (usYMag--)
                    {
                        A2::IncY( pkDest, kDstLineY );
                        O2 kDstLineX = kDstLineY;
                        for (eC_Short usX1=0; usX1<usXMag; usX1++)
                        {
                            A2::PutPixel(kDstLineX, BlendColor);
                            A2::IncX( pkDest, kDstLineX );
                        }
                    }
                }
                // last is the usual case, we advance the offset
                for (eC_Short usX1=0; usX1<usXMag; usX1++)
                {
                   A2::PutPixel(kDstLine, BlendColor);
                   A2::IncX( pkDest, kDstLine );
                }
               break;
           case 0:
               // Nothing to do : RetCol = SrcColor; Avoid Putpixel, but advance the offset
               for (eC_Short usX1=0; usX1<usXMag; usX1++)
               {
                   A2::IncX( pkDest, kDstLine );
               }
               break;
           default:
               // we draw additional lines using a copy of given offset first
                if (usYMag > 1)
                {
                    usYMag--;
                    O2 kDstLineY = kDstLine;
                    while (usYMag--)
                    {
                        A2::IncY( pkDest, kDstLineY );
                        O2 kDstLineX = kDstLineY;
                        for (eC_Short usX1=0; usX1<usXMag; usX1++)
                        {
                            A2::BlendPixel(kDstLineX, BlendColor, ubPixelAlpha);
                            A2::IncX( pkDest, kDstLineX );
                        }
                    }
                }
                // last is the usual case, we advance the offset
               for (eC_Short usX1=0; usX1<usXMag; usX1++)
               {
                   A2::BlendPixel(kDstLine, BlendColor, ubPixelAlpha);
                   A2::IncX( pkDest, kDstLine );
               }
               break;
           }
       }
       else
       {
           // just put non premultiplied pixels to destination
           switch (ubPixelAlpha)
           {
           case 0xFF:
               // we draw additional lines using a copy of given offset first
                if (usYMag > 1)
                {
                    usYMag--;
                    O2 kDstLineY = kDstLine;
                    while (usYMag--)
                    {
                        A2::IncY( pkDest, kDstLineY );
                        O2 kDstLineX = kDstLineY;
                        for (eC_Short usX1=0; usX1<usXMag; usX1++)
                        {
                            A2::PutPixel(kDstLineX, BlendColor);
                            A2::IncX( pkDest, kDstLineX );
                        }
                    }
                }
                // last is the usual case, we advance the offset
               for (eC_Short usX1=0; usX1<usXMag; usX1++)
               {
                   A2::PutPixel(kDstLine, BlendColor);
                   A2::IncX( pkDest, kDstLine );
               }
               break;
           case 0:
               // Nothing to do : RetCol = SrcColor; Avoid Putpixel, but advance the offset
               for (eC_Short usX1=0; usX1<usXMag; usX1++)
               {
                   A2::IncX( pkDest, kDstLine );
               }
               break;
           default:
               // we draw additional lines using a copy of given offset first
                if (usYMag > 1)
                {
                    usYMag--;
                    O2 kDstLineY = kDstLine;
                    while (usYMag--)
                    {
                        A2::IncY( pkDest, kDstLineY );
                        O2 kDstLineX = kDstLineY;
                        for (eC_Short usX1=0; usX1<usXMag; usX1++)
                        {
                            A2::BlendPixelSrcAlpha(kDstLineX, BlendColor, ubGlobalAlpha);
                            A2::IncX( pkDest, kDstLineX );
                        }
                    }
                }
                // last is the usual case, we advance the offset
               for (eC_Short usX1=0; usX1<usXMag; usX1++)
               {
                   A2::BlendPixelSrcAlpha(kDstLine, BlendColor, ubGlobalAlpha);
                   A2::IncX( pkDest, kDstLine );
               }
               break;
           }
       }
       return kDstLine;
   }
};

#endif // defined( eGML_USEBITMAPRGB32RLE8 ) || defined ( eGML_USEBITMAPRGB32RLE16 )

