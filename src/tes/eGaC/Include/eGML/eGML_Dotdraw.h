/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       %name%
* %version:        %
* 
* author:          %created_by%
* creation date:   %date_created%
* description:

* %date_modified:  % (%derived_by: %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_DOTDRAW__H_
#define EGML_DOTDRAW__H_

//---------------------------------------------------------------

eDBG_STAT_DEF(NumberOfDots);
eDBG_STAT_DEF(RejectedDot);
eDBG_STAT_DEF(DotRadius);

eDBG_PERF_DEF(Dots);

#ifdef  eGML_GAMMACORRECTAA
#define BlendPixelGamma( p, c, a )      BlendPixel( p, c, g_eGML_ubGammaRamp[ (eC_UByte) (a) ] )
#else
#define BlendPixelGamma( p, c, a )      BlendPixelSrcAlpha( p, c, a )
#endif

//---------------------------------------------------------------
template <typename P1, typename P2, class A1, eC_Bool bMustBlendAA>
void eGML_Linedraw<P1,P2,A1,bMustBlendAA>::DrawDot( eGML_Bitmap *pkDest, eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, 
                                      eGML_Color kColor, eC_Flag uiFlag )
{
   P2	kOffset;
   P1	pxColor;

   eDBG_STAT_ADD(DotRadius, eC_ToFloat(vRadius));

   // convert color to pixelformat 
   pxColor = A1::ColorToPixel( kColor );

   // specialcase 1pixel dots
   if (vRadius <= eC_FromInt(1))
   {        
      eC_Int iPosX = eC_ToInt( vCenterX );
      eC_Int iPosY = eC_ToInt( vCenterY );

      // culling
      if ((iPosX < (eC_Int)pkDest->GetClipMinX()) || (iPosY < (eC_Int)pkDest->GetClipMinY()) ||
         (iPosX > (eC_Int)pkDest->GetClipMaxX()) || (iPosY > (eC_Int)pkDest->GetClipMaxY()))
      {
         eDBG_STAT_ADD(RejectedDot, 1);
         return;
      }
      eDBG_STAT_ADD(NumberOfDots, 1);
      eDBG_PERF_BEGIN( Dots );

      // find address
      A1::GetOffset( pkDest, (eC_UInt) iPosX, (eC_UInt) iPosY, kOffset );

      // check rendering mode
      if (uiFlag & eGML_LF_ANTIALIAS)
      {
         // Antialiased Pixel. (HQ AA support not required here)
         eC_Fixed fxPosX = eC_FixFromValue( vCenterX );
         eC_Fixed fxPosY = eC_FixFromValue( vCenterY );

         // use wupixel writing for AA singlepixel dots
         eC_UInt  uiAlphaX = eC_FixFracToByte( fxPosX );
         eC_UInt  uiAlphaY = eC_FixFracToByte( fxPosY );

         if (uiAlphaY == 0)
         {
            if (uiAlphaX == 0)
            {
               // isolated
               if(bMustBlendAA)
               {
                  A1::BlendPixelSrcAlpha( kOffset, pxColor, 255 );
               }
               else
               {
                  A1::PutPixel( kOffset, pxColor );
               }
            } else {
               // horizontal
               A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[ (eC_UByte) (uiAlphaX ^ 0xff) ] );
               if ((eC_UInt)(iPosX+1) < pkDest->GetClipMaxX())
               {
                  A1::IncX( pkDest, kOffset );
                  A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[ (eC_UByte) uiAlphaX ] );
               }
            }
         } else {                
            if (uiAlphaX == 0)
            {
               // vertical
               A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[ (eC_UByte) (uiAlphaY ^ 0xff) ] );
               if ((eC_UInt)(iPosY+1) < pkDest->GetClipMaxY())
               {
                  A1::IncY( pkDest, kOffset );
                  A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[ (eC_UByte) uiAlphaY ] );
               }
            } else {
               // full pixel
               P2	kOffset2 = kOffset;
               // find intensities [optimised to use only one mul]
               eC_UInt uiAlpha1 = uiAlphaX * uiAlphaY;                 // aX*aY 
               eC_UInt uiAlpha2 = (uiAlphaX << 8) - uiAlphaX;          // aX*255
               eC_UInt uiAlpha3 = (uiAlphaY << 8) - uiAlphaY;          // aY*255
               eC_UInt uiAlpha4 = 65025 - (uiAlpha2 + uiAlpha3);       // (255-(aX+aY))*255
               // pixelintensities are (a4+a1,a2-a1,a3-a1,a1)

               A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[(eC_UByte) ((uiAlpha4+uiAlpha1) >> 8)] );
               if ((eC_UInt)(iPosX+1) < pkDest->GetClipMaxX())
               {
                  A1::IncX( pkDest, kOffset );
                  A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[(eC_UByte) ((uiAlpha2-uiAlpha1) >> 8)] );
                  if ((eC_UInt)(iPosY+1) < pkDest->GetClipMaxY())
                  {
                     A1::IncY( pkDest, kOffset );
                     A1::BlendPixelGamma( kOffset, pxColor, g_eGML_ubGammaRamp[(eC_UByte) (uiAlpha1 >> 8)] );
                     A1::IncY( pkDest, kOffset2 );
                     A1::BlendPixelGamma( kOffset2,pxColor, g_eGML_ubGammaRamp[(eC_UByte) ((uiAlpha3-uiAlpha1) >> 8)] );
                  }
               }
            }                
         }
      } 
      else 
      {
         A1::PutPixel( kOffset, pxColor );
      }

      eDBG_PERF_END( Dots );
      return;
   }   

   eDBG_PERF_BEGIN( Dots );

   // width correction if filtered		
#ifdef eGML_FILTEREDAA
   if (uiFlag & eGML_LF_HQ_AA)
   {
      // disable hqaa for very small dots
      if (vRadius < eC_FromInt(6))
      {
         uiFlag &= ~eGML_LF_HQ_AA;
         vRadius += eC_FromFloat( 0.25f );
      } else {
         vRadius += eC_FromFloat( 0.5f );	
      }
   } else {
#endif				
      if (uiFlag & eGML_LF_ANTIALIAS)
      {
         vRadius += eC_FromFloat( 0.25f );
      } else {		
         vRadius += eC_FromFloat( 0.5f );
         // implement minimum radius for nonAA dots
         if (vRadius < eC_FromFloat( 1.5f )) vRadius = eC_FromFloat( 1.5f );
      }
#ifdef eGML_FILTEREDAA
   }
#endif

   // find (AA conservative) bounding box
   eC_UInt uiRadius = eC_ToInt( eC_Ceil( vRadius ) );
   eC_Int  iMinX = eC_ToInt( vCenterX - vRadius );
   eC_Int  iMinY = eC_ToInt( vCenterY - vRadius );
   eC_Int  iWidth  = uiRadius*2 + 1;
   eC_Int  iHeight = uiRadius*2 + 1;
   eC_Int  iXOffset = 0;
   eC_Int  iYOffset = 0;

   // clipping
   if (iMinX < (eC_Int)pkDest->GetClipMinX())
   {
      iXOffset = pkDest->GetClipMinX() - iMinX;
      iWidth -= iXOffset;       
      iMinX = (eC_Int)pkDest->GetClipMinX();
   }
   if (iMinY < (eC_Int)pkDest->GetClipMinY())
   {
      iYOffset = pkDest->GetClipMinY() - iMinY;
      iHeight -= iYOffset;       
      iMinY = (eC_Int)pkDest->GetClipMinY();
   }

   const eC_Int iCxMax = (eC_Int)pkDest->GetClipMaxX();
   const eC_Int iCyMax = (eC_Int)pkDest->GetClipMaxY();
   if ((iMinX+iWidth)  > iCxMax) iWidth  = iCxMax - iMinX + 1;
   if ((iMinY+iHeight) > iCyMax) iHeight = iCyMax - iMinY + 1;

   // culling
   if ((iWidth<=0)||(iHeight<=0))
   {
      eDBG_STAT_ADD(RejectedDot, 1);
      eDBG_PERF_END( Dots );
      return;
   }

   eDBG_STAT_ADD(NumberOfDots, 1);

   // find topleft adress
   A1::GetOffset( pkDest, (eC_UInt) iMinX, (eC_UInt) iMinY, kOffset );

   // subpixel offset (shifted one bit to avoid mulfrac signoverflow)
#ifdef eC_FIXPOINT
   eC_Value vXf = eC_Frac( vCenterX - vRadius ) / 2;
   eC_Value vYf = eC_Frac( vCenterY - vRadius ) / 2;
#else
   // floatversion of eC_Frac is buggy: eC_Frac(-2.2) = (-0.8) !!! That's why we implement eC_Frac here again, since the modification eC_Frac migh have majoy sideeffects
   eC_Value vXf = vCenterX - vRadius;
   eC_Value vYf = vCenterY - vRadius;

   if (vXf < eC_FromInt(0))
      vXf = eC_Div((vXf - eC_Ceil(vXf)), eC_FromInt(2));
   else
      vXf = eC_Div((vXf - eC_Floor(vXf)), eC_FromInt(2));

   if (vYf < eC_FromInt(0))
      vYf = eC_Div((vYf - eC_Ceil(vYf)), eC_FromInt(2));
   else
      vYf = eC_Div((vYf - eC_Floor(vYf)), eC_FromInt(2));
#endif
   // half inverse radius
   eC_Value vIR   = - eC_Div( eC_FromFloat(1.0f) , vRadius * 2 );  //reci ?

   // init circle distance counter (0..1 inside antialiased circle border)    
   eC_Fixed fxLx1 = eC_FixFromValue( eC_MulQ(eC_MulFrac(vXf, vXf), vIR)*2 - vXf )*2 - (eC_FixFromValue( vRadius ) / 2);
   eC_Fixed fxLy1 = eC_FixFromValue( eC_MulQ(eC_Mul(vYf, vYf), vIR)*2 - vYf )*2;    
   eC_Fixed fxLx2 = eC_FixFromValue( eC_MulQ((eC_FromInt(1) - vXf*4), vIR) + eC_FromInt(1) );
   eC_Fixed fxLy2 = eC_FixFromValue( eC_MulQ((eC_FromInt(1) - vYf*4), vIR) + eC_FromInt(1) );    
   eC_Fixed fxL3  = eC_FixFromValue( vIR ) * 2;

   // clipping offset (integrating with distance init would make MulFrac impossible)
   if (iXOffset>0)
   {
      fxLx1 += iXOffset * fxLx2 + ((((iXOffset - 1) * iXOffset)>>1) * fxL3);
      fxLx2 += iXOffset * fxL3;
   }
   if (iYOffset>0)
   {
      fxLy1 += iYOffset * fxLy2 + ((((iYOffset - 1) * iYOffset)>>1) * fxL3);
      fxLy2 += iYOffset * fxL3;
   }

   // check rendering mode
   if (uiFlag & eGML_LF_ANTIALIAS)
   {    
      // doublestep antialiasing
      if (uiFlag & eGML_LF_HQ_AA)
      {
         fxLx1 >>= 1; fxLx2 >>= 1; fxLy1 >>= 1; fxLy2 >>= 1; fxL3  >>= 1;
      }

      // raster circle scanlines (antialiased)
      for (eC_Int iY=0; iY < iHeight; iY++)
      {
         P2       kDst = kOffset;
         eC_Int   iX = 0;
         eC_Fixed fxLen1 = fxLx1 + fxLy1;
         eC_Fixed fxLen2 = fxLx2;

         // left part
         while (iX < iWidth)
         {           
            // inside circle ?
            if (fxLen1 > 0)
            {
               // inside AA region ? 

               if (fxLen1 < eC_FixFromInt(1))
               {
                  // antialiasing
                  A1::BlendPixelSrcAlpha( kDst, pxColor, eC_FixFracToByte( fxLen1) );

                  // this part is an optimization: All full tone pixels inside the circle can be drawn as a spawn
               } 
               else 
               {
                  // full tone reached -> finish leftside
                  eC_Int iSpan = (iWidth - iX*2 - iXOffset) - 3;  // -2 or more. This is the distance to the circle border. -2 seems to work best. more negative values make it even saver but much slower, since more pixels are blended
                  if (iSpan>1)
                  {
                     // render midpart
                     if(bMustBlendAA)
                     {
                        eC_Int i = iSpan;
                        while(i-- >0)
                        {
                           A1::BlendPixelSrcAlpha(kDst, pxColor, 255);
                           A1::IncX(pkDest, kDst);
                        }
                     }
                     else
                     {
                        A1::PutSpan( kDst, pxColor, iSpan );
                     }

                     iX += iSpan;     
#ifdef eGML_REDUCEMUL
                     for (eC_Int iX2=0; iX2 < iSpan; iX2++) { fxLen1 += fxLen2; fxLen2 += fxL3; } 
#else
                     fxLen1 += iSpan * fxLen2 + ((((iSpan - 1) * iSpan)>>1) * fxL3);
                     fxLen2 += iSpan * fxL3;
#endif
                     break;
                  }
                  else 
                  {
                     // full tone pixel
                     if(bMustBlendAA)
                     {
                        A1::BlendPixelSrcAlpha( kDst, pxColor, 255);
                     }
                     else
                     {
                        A1::PutPixel( kDst, pxColor );
                     }
                  }                        
               }                
            } 

            // increment squared distance to pixel
            fxLen1 += fxLen2;
            fxLen2 += fxL3;            
            A1::IncX( pkDest, kDst );
            iX++;
         }

         // right part
         while (iX < iWidth)
         {           
            // inside circle ?
            if (fxLen1 > 0)
            {
               // inside AA region ?
               if (fxLen1 < eC_FixFromInt(1))
               {
                  A1::BlendPixelGamma( kDst, pxColor, eC_FixFracToByte( fxLen1) );
               }
               else
               {
                  if(bMustBlendAA)
                  {
                     A1::BlendPixelSrcAlpha(kDst, pxColor, 255);
                  }
                  else
                  {
                     A1::PutPixel( kDst, pxColor );
                  }
               }
            } else {
               // leaving circle on right side->abort scanline
               break;
            }

            // increment squared distance to pixel
            fxLen1 += fxLen2;
            fxLen2 += fxL3;            
            A1::IncX( pkDest, kDst );
            iX++;
         }

         // increment yaxis
         fxLy1 += fxLy2;
         fxLy2 += fxL3;  

         A1::IncY( pkDest, kOffset );
      }
   }
   else
   {
      // raster circle scanlines (not antialiased)
      for (eC_Int iY=0; iY < iHeight; iY++)
      {
         P2       kDst = kOffset;
         eC_Fixed fxLen1 = fxLx1 + fxLy1;
         eC_Fixed fxLen2 = fxLx2;

         for (eC_Int iX=0; iX < iWidth; iX++)
         {           
            // inside circle ?
            if (fxLen1 > eC_FixFromFloat(0.5f))
            {
               // HACK HACK HACK 
               // original value was -1
               // -3 is selected by try and error. The span rendere is much faster, than the put pixel, but the ixoffset seems to 
               // have some sort of rounding error, which causes too much pixels to be drawn. -3 is A HACK!!
               eC_Int iSpan = (iWidth - iX*2 - iXOffset) - 3; 
               // HACK HACK HACK 
               if (iSpan>1)
               {
                  // render midpart
                  A1::PutSpan( kDst, pxColor, iSpan );
                  iX += iSpan-1;     

#ifdef eGML_REDUCEMUL
                  for (eC_Int iX2=0; iX2 < iSpan; iX2++) { fxLen1 += fxLen2; fxLen2 += fxL3; } 
#else
                  fxLen1 += iSpan * fxLen2 + ((((iSpan - 1) * iSpan)>>1) * fxL3);
                  fxLen2 += iSpan * fxL3;
#endif
                  continue;
               } else {
                  // singlepixel midpart
                  A1::PutPixel( kDst, pxColor );
               }
            } 

            // increment squared distance to pixel
            fxLen1 += fxLen2;
            fxLen2 += fxL3;            
            A1::IncX( pkDest, kDst );                
         }

         // increment yaxis
         fxLy1 += fxLy2;
         fxLy2 += fxL3;  

         A1::IncY( pkDest, kOffset );
      }

   }
   eDBG_PERF_END( Dots );
}

//---------------------------------------------------------------	
#endif

