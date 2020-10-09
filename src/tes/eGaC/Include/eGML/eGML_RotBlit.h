/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_RotBlit.h
* %version:        134.0 %
* 
* author:          hh04030
* creation date:   Wed Apr 26 11:04:39 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_ROTBLIT__H_
#define EGML_ROTBLIT__H_
//---------------------------------------------------------------	

#include "eGML_Blit.h"
#include "eGML_Bitmap.h"
#include "eGML_BitmapA8.h"
#include "eGML_FontDecoder.h"
#include "eDBG_Statistic.h"
#include "eC_Math.h"


//---------------------------------------------------------------	



typedef
struct
{
  eC_Int   y1;
  eC_Fixed dx;      //dx/dy
  eC_Fixed sx;
  eC_Fixed v;
  eC_Fixed u;
  eC_Fixed dv;
  eC_Fixed du;
}eGML_RotBlitEdgeLeft;

typedef
struct
{
  eC_Int   y1;
  eC_Fixed dx;      //dx/dy
  eC_Fixed sx;
}eGML_RotBlitEdgeRight;

//---------------------------------------------------------------	


class eGML_RotBlitParam
{
  public:
  static bool RotBlitCalc(eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, 
                          eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY,
                          eC_Value vRotPosX, eC_Value vRotPosY, eGML_RotBlitEdgeLeft *pEdgesLeft, eGML_RotBlitEdgeRight *pEdgesRight,
                          eC_Int &iEdgeCntLeft, eC_Int &iEdgeCntRight,  eC_Int &iEy,
                          eC_Fixed &fixAU, eC_Fixed &fixAV);

};






template <
		  typename P1,		// Pixeltype for Source
		  typename O1,		// Offsettype for Source
          class A1,			// Accessmethods for Source

		  typename P2,		// Pixeltype for Destination
		  typename O2,		// Offsettype for Destination
		  class A2			// Accessmethods for Destination

		 > 

class eGML_RotBlit
{
public:


	static inline void RotBlit_Int(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{		
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
     
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {
        O2 kOffset;
        O1 kTexOffset;

        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;
          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;  
          iLeft++;
        }
                
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left        
          if ( sx < iClipLeft)
          {
            u   += au * (iClipLeft - sx);
            v   += av * (iClipLeft - sx);            
            sx= iClipLeft;
          } 
                    
//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          
          A2::GetOffset(pkDst, sx, iY, kOffset ); 

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A2::BlendPixelSrcAlpha( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ), ubAlpha );                
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A2::BlendPixelSrcAlpha( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ), ubAlpha );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A2::PutPixel( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A2::PutPixel( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }      
            }
          }
          else
          {
            if (boolAlpha)
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A2::BlendPixelSrcAlpha( kOffset, A1::GetPixel( kTexOffset ) , ubAlpha);
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A2::BlendPixelSrcAlpha( kOffset, A1::GetPixel( kTexOffset ) , ubAlpha);

                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {                    
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A2::PutPixel( kOffset, A1::GetPixel( kTexOffset ) );
              A2::IncX( pkDst, kOffset );   

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A2::PutPixel( kOffset, A1::GetPixel( kTexOffset ) );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }

          }
        }
        
        rx+= drx;        

        lu+= dlu;
        lv+= dlv;        
        lx+= dlx;
      }
    }


	static inline void RotBlit_RGB(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;

      


      eC_Fixed av;
      eC_Fixed au;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {
        O2 kOffset;
        O1 kTexOffset;

        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;

          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;

          iLeft++;
        }
             
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left
          if ( sx < iClipLeft)
          {
            u += au * (iClipLeft - sx);
            v += av * (iClipLeft - sx);
            sx= iClipLeft;
          }        

//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          A2::GetOffset(pkDst, sx, iY, kOffset );

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
              P1 pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
              eC_UByte r = A1::RedFromPixel   ( pxPixel );
              eC_UByte g = A1::GreenFromPixel ( pxPixel );
              eC_UByte b = A1::BlueFromPixel  ( pxPixel );
              eC_UByte a = A1::AlphaFromPixel ( pxPixel );              
              
              A2::BlendPixelSrcAlpha( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a), ubAlpha );
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                P1 pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
                eC_UByte r = A1::RedFromPixel   ( pxPixel );
                eC_UByte g = A1::GreenFromPixel ( pxPixel );
                eC_UByte b = A1::BlueFromPixel  ( pxPixel );
                eC_UByte a = A1::AlphaFromPixel ( pxPixel );
                                
                A2::BlendPixelSrcAlpha( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a), ubAlpha );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
              P1 pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
              eC_UByte r = A1::RedFromPixel   ( pxPixel );
              eC_UByte g = A1::GreenFromPixel ( pxPixel );
              eC_UByte b = A1::BlueFromPixel  ( pxPixel );
              eC_UByte a = A1::AlphaFromPixel ( pxPixel );
              
              A2::PutPixel( kOffset,  A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a) );
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
                r = A1::RedFromPixel   ( pxPixel );
                g = A1::GreenFromPixel ( pxPixel );
                b = A1::BlueFromPixel  ( pxPixel );
                a = A1::AlphaFromPixel ( pxPixel );
                                
                A2::PutPixel( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a) );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }

          }
          else
          {
            if (boolAlpha)
            {

              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
              P1 pxPixel  = A1::GetPixel( kTexOffset );
              eC_UByte r = A1::RedFromPixel   ( pxPixel );
              eC_UByte g = A1::GreenFromPixel ( pxPixel );
              eC_UByte b = A1::BlueFromPixel  ( pxPixel );
              eC_UByte a = A1::AlphaFromPixel ( pxPixel );
              
              A2::BlendPixelSrcAlpha( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a), ubAlpha );
                                          
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                pxPixel  = A1::GetPixel( kTexOffset );
                r = A1::RedFromPixel   ( pxPixel );
                g = A1::GreenFromPixel ( pxPixel );
                b = A1::BlueFromPixel  ( pxPixel );
                a = A1::AlphaFromPixel ( pxPixel );
                
                A2::BlendPixelSrcAlpha( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a), ubAlpha );
                
                
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
              P1 pxPixel  = A1::GetPixel( kTexOffset );
              eC_UByte r = A1::RedFromPixel   ( pxPixel );
              eC_UByte g = A1::GreenFromPixel ( pxPixel );
              eC_UByte b = A1::BlueFromPixel  ( pxPixel );
              eC_UByte a = A1::AlphaFromPixel ( pxPixel );
              
              A2::PutPixel( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a) );
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                pxPixel  = A1::GetPixel( kTexOffset );
                r = A1::RedFromPixel   ( pxPixel );
                g = A1::GreenFromPixel ( pxPixel );
                b = A1::BlueFromPixel  ( pxPixel );
                a = A1::AlphaFromPixel ( pxPixel );
                
                A2::PutPixel( kOffset, A2::ColorAndAlphaToPixel(A2::PixelToColor(A2::PixelFromRGB(r, g, b)), a) );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
          }

        }
        rx+= drx;
        lx+= dlx;
        
        lu+= dlu;
        lv+= dlv;
      }		
    }



	static inline void RotBlit_IntCol(eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
      
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      P2 pPixel = A2::ColorToPixel( cCol );

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {
        O2 kOffset;
        O1 kTexOffset;

        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;

          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;

          iLeft++;
        }
        
        
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left
          if ( sx < iClipLeft)
          {
            u += au * (iClipLeft - sx);
            v += av * (iClipLeft - sx);
            sx= iClipLeft;
          }        

//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          A2::GetOffset(pkDst, sx, iY, kOffset );

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
              
              A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
                // (todo) check compiler optimization, maybe add Blend4Alpha() ?
              eC_UByte ubBlend = A1::AlphaFromPixel(A1::Blend4( pkAlpha, kTexOffset, fracv, fracu ));

              ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
              
              A2::BlendPixel( kOffset, pPixel, ubBlend );
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));

                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                // (todo) check compiler optimization, maybe add Blend4Alpha() ?
                ubBlend = A1::AlphaFromPixel(A1::Blend4( pkAlpha, kTexOffset, fracv, fracu ));
				
                ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
                if (ubBlend > 0)
                   A2::BlendPixel( kOffset, pPixel, ubBlend );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

              // (todo) check compiler optimization, maybe add Blend4Alpha() ?
              A2::BlendPixel( kOffset, pPixel, A1::AlphaFromPixel(A1::Blend4( pkAlpha, kTexOffset, fracv, fracu ) ));
                
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));

                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

                // (todo) check compiler optimization, maybe add Blend4Alpha() ?
                eC_UByte ubBlend = A1::AlphaFromPixel(A1::Blend4( pkAlpha, kTexOffset, fracv, fracu ));
                if (ubBlend > 0)
                   A2::BlendPixel( kOffset, pPixel, ubBlend);

                A2::IncX( pkDst, kOffset );

                u+= au;
                v+= av;
              }
            }

          }
          else
          {
             if (boolAlpha)
             {
                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                eC_UByte ubBlend = A1::AlphaFromPixel(A1::GetPixel( kTexOffset ));

                if (ubBlend > 0) 
                {
                   ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
                   A2::BlendPixel( kOffset, pPixel, ubBlend );
                }

                A2::IncX( pkDst, kOffset );         

                u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
                v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
                sx++;

                for (int x= sx ;x<ex ;x++)
                {
                   A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                   ubBlend = A1::AlphaFromPixel(A1::GetPixel( kTexOffset ));
                   ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
                   if (ubBlend > 0){					
                      A2::BlendPixel( kOffset, pPixel, ubBlend );
                   }

                   A2::IncX( pkDst, kOffset );         
                   u+= au;
                   v+= av;
                }
             }
             else
             {

                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

                eC_UByte ubBlend= A1::AlphaFromPixel(A1::GetPixel( kTexOffset ));
                if (ubBlend > 0)
                   A2::BlendPixel( kOffset, pPixel,ubBlend);

                A2::IncX( pkDst, kOffset );         

                u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
                v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
                sx++;

                for (int x= sx ;x<ex ;x++)
                {
                   A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

                   eC_UByte ubBlend= A1::AlphaFromPixel(A1::GetPixel( kTexOffset ));

                   if (ubBlend > 0)
                      A2::BlendPixel( kOffset, pPixel, ubBlend);

                   A2::IncX( pkDst, kOffset );         
                   u+= au;
                   v+= av;
                }
             }
          }
        }
        rx+= drx;
        lx+= dlx;

        lu+= dlu;
        lv+= dlv;
      }
    }



	static inline void RotBlit_Int_CK(eGML_Bitmap *pkSrc, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha, eGML_Color cColorKey)
	{		
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
     
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {
        O2 kOffset;
        O1 kTexOffset;

        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;
          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;  
          iLeft++;
        }
                
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left        
          if ( sx < iClipLeft)
          {
            u   += au * (iClipLeft - sx);
            v   += av * (iClipLeft - sx);            
            sx= iClipLeft;
          } 
                    
//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          
          A2::GetOffset(pkDst, sx, iY, kOffset ); 

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              eGML_Color kColor = A1::PixelToColor( A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
              if (kColor != cColorKey)
              {
                A2::BlendPixelSrcAlpha( kOffset, A2::ColorToPixel( kColor ), ubAlpha);
              }

              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                eGML_Color kColor = A1::PixelToColor( A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
                if (kColor != cColorKey)
                {
                  A2::BlendPixelSrcAlpha( kOffset, A2::ColorToPixel( kColor ), ubAlpha);
                }
               
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              eGML_Color kColor = A1::PixelToColor( A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
              if (kColor != cColorKey)
              {
                 A2::PutPixel( kOffset, A2::ColorToPixel( kColor ) );
              }

              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                eGML_Color kColor = A1::PixelToColor( A1::Blend4( pkSrc, kTexOffset, fracv, fracu ) );
                if (kColor != cColorKey)
                {
                   A2::PutPixel( kOffset, A2::ColorToPixel( kColor ) );
                }
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }      
            }
          }
          else
          {
            if (boolAlpha)
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              eGML_Color kColor = A1::PixelToColor( A1::GetPixel( kTexOffset ) );
              if (kColor != cColorKey)
              {
                A2::BlendPixelSrcAlpha( kOffset, A2::ColorToPixel( kColor ), ubAlpha);
              }
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                eGML_Color kColor = A1::PixelToColor( A1::GetPixel( kTexOffset ) );
                if (kColor != cColorKey)
                {
                  A2::BlendPixelSrcAlpha( kOffset, A2::ColorToPixel( kColor ), ubAlpha);
                }

                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {                    
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              eGML_Color kColor = A1::PixelToColor( A1::GetPixel( kTexOffset ) );
              if (kColor != cColorKey)
              {
                 A2::PutPixel( kOffset, A2::ColorToPixel( kColor ) );
              }

              A2::IncX( pkDst, kOffset );   

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                eGML_Color kColor = A1::PixelToColor( A1::GetPixel( kTexOffset ) );
                if (kColor != cColorKey)
                {
                   A2::PutPixel( kOffset, A2::ColorToPixel( kColor ) );
                }
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }

          }
        }
        
        rx+= drx;        

        lu+= dlu;
        lv+= dlv;        
        lx+= dlx;
      }
    }

	static inline void decodePixel(eC_UByte ubAlphas[4], P2 mixedColors[4], eC_UByte c[4], O1 kOffset, eGML_Bitmap *pkAlpha, P2 pPixelOutline, P2 pPixelInside, eC_UByte & flag){
	  c[0] = kOffset[0];
	  c[1] = kOffset[1];
	  c[2] = kOffset[pkAlpha->m_uiPitch];
	  c[3] = kOffset[pkAlpha->m_uiPitch+1];

	  
	  if ((c[0] | c[1] | c[2] | c[3])>>1  == 0)
	  {
			flag = 0;
			return;
	  }	  

	  //if ((c[0] == c[1]) &&  (c[0] == c[2]) && (c[0] == c[3])){
	  if (!(	(c[0] ^ c[1]) |  (c[0] ^ c[2]) | (c[0] ^ c[3]) )>>1)
	  {	  
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


	  for (int i_ = 0; i_ < 4; i_++)
	  {

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


	static inline void RotBlit_Int2Col(eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
      
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      P2 pPixelOutline = A2::ColorToPixel( cCol1 );
	  P2 pPixelInside  = A2::ColorToPixel( cCol2 );

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {
        O2 kOffset;
        O1 kTexOffset;

        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;

          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;

          iLeft++;
        }
        
        
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left
          if ( sx < iClipLeft)
          {
            u += au * (iClipLeft - sx);
            v += av * (iClipLeft - sx);
            sx= iClipLeft;
          }        

//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          A2::GetOffset(pkDst, sx, iY, kOffset );

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {
				eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
				eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));

				A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

				eC_UByte c[4];
				eC_UByte ubAlphas[4];
				P2 mixedColors[4];
				eC_UByte flag = 0;

				decodePixel(ubAlphas, mixedColors, c, kTexOffset, pkAlpha, pPixelOutline, pPixelInside, flag);

				// now use bilinearfilterinig for the alphas

				switch (flag) {
					case BIT_BLEND_MASK:{
						// and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
						A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), (A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu)*ubAlpha)>>8 );
						break;
					   }
					case INSIDE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelInside, ubAlpha);
						break;
					 }
					case OUTLINE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelOutline,(ubAlphas[0]*ubAlpha)>>8);
						break;
					  }
				}


				A2::IncX( pkDst, kOffset );

				u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
				v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
				sx++;

              for (int x= sx ;x<ex ;x++)
              {

                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));

                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

				decodePixel(ubAlphas, mixedColors, c, kTexOffset, pkAlpha, pPixelOutline, pPixelInside, flag);

				switch (flag) {
					case BIT_BLEND_MASK:{
						// and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
						A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), (A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu)*ubAlpha)>>8 );
						break;
					   }
					case INSIDE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelInside, ubAlpha);
						break;
					 }
					case OUTLINE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelOutline,(ubAlphas[0]*ubAlpha)>>8);
						break;
					  }
				}                

                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
                
              A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

				eC_UByte c[4];
				eC_UByte ubAlphas[4];
				P2 mixedColors[4];
				eC_UByte flag = 0;

				decodePixel(ubAlphas, mixedColors, c, kTexOffset, pkAlpha, pPixelOutline, pPixelInside, flag );

				switch (flag) {
					case BIT_BLEND_MASK:{
						eC_UByte ubBlend = A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu);
						// and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
						A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), ubBlend );
						break;
					   }
					case INSIDE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelInside, eC_BLEND_MAX);
						break;
					 }
					case OUTLINE_BIT_MASK:{
						A2::BlendPixel( kOffset, pPixelOutline,ubAlphas[0]);
						break;
					  }
				}

				// and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
				//A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), A2::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu));
				//A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), 255);

               
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);

				decodePixel(ubAlphas, mixedColors, c, kTexOffset, pkAlpha, pPixelOutline, pPixelInside, flag );

					switch (flag) {
						case BIT_BLEND_MASK:{
							eC_UByte ubBlend = A1::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu);
							// and use bilinearfilterinig for the 4 2-Color colors and blend them with the bilinearfilterd alpha
							A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), ubBlend );
							break;
						   }
						case INSIDE_BIT_MASK:{
							A2::BlendPixel( kOffset, pPixelInside, eC_BLEND_MAX);
							break;
						 }
						case OUTLINE_BIT_MASK:{
							A2::BlendPixel( kOffset, pPixelOutline,ubAlphas[0]);
							break;
						  }
					}
				

				//A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), A2::MixBilinear(ubAlphas[0],ubAlphas[1], ubAlphas[2], ubAlphas[3],fracv, fracu));
				//A2::BlendPixel( kOffset, A2::MixBilinear(mixedColors[0],mixedColors[1], mixedColors[2], mixedColors[3],fracv, fracu), 255);
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }

          }
          else
          {
            if (boolAlpha)
            {
              A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
			  
			  decodePixelSimpleUBAlpha(kTexOffset, kOffset, pPixelOutline, pPixelInside, ubAlpha);
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                decodePixelSimpleUBAlpha(kTexOffset, kOffset, pPixelOutline, pPixelInside, ubAlpha);

                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {

              A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              
              decodePixelSimple(kTexOffset, kOffset, pPixelOutline, pPixelInside);
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                
                decodePixelSimple(kTexOffset, kOffset, pPixelOutline, pPixelInside);
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
          }
        }
        rx+= drx;
        lx+= dlx;
        
        lu+= dlu;
        lv+= dlv;
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

class eGML_RotBlitAlpha
{

public:
	static inline void RotBlit_IntAlpha(eGML_Bitmap *pkSrc, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
      
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {

        O3 kAlphaOffset;
        O2 kOffset;
        O1 kTexOffset;


        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;

          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;

          iLeft++;
        }
        
        
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left
          if ( sx < iClipLeft)
          {
            u += au * (iClipLeft - sx);
            v += av * (iClipLeft - sx);
            sx= iClipLeft;
          }        

//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          A2::GetOffset(pkDst, sx, iY, kOffset );

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {

              eC_UByte   fracv= eC_FixFracToByte(v);
              eC_UByte   fracu= eC_FixFracToByte(u);
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              eC_UByte ubBlend = A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu));
              ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);
              
              A2::BlendPixelSrcAlpha( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ), ubBlend );
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(v);
                fracu= eC_FixFracToByte(u);
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                ubBlend = A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu ));
                ubBlend = eGMLBlit_MulABDiv255(ubBlend, ubAlpha);

                A2::BlendPixel( kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ), ubBlend );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              eC_UByte   fracv= eC_FixFracToByte(v);
              eC_UByte   fracu= eC_FixFracToByte(u);
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              A2::BlendPixel(
                 kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ),
                 A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu ))
                 );
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(v);
                fracu= eC_FixFracToByte(u);
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                A2::BlendPixel(
                   kOffset, A1::Blend4( pkSrc, kTexOffset, fracv, fracu ),
                   A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu))
                   );
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
              
          }
          else
          {
            if (boolAlpha)
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              A2::BlendPixel(
                 kOffset, A1::GetPixel( kTexOffset ),
                 eGMLBlit_MulABDiv255(
                    A3::AlphaFromPixel(A3::GetPixel(kAlphaOffset)),
                    ubAlpha
                    )
                 );
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                A2::BlendPixel(
                   kOffset, A1::GetPixel( kTexOffset ),
                   eGMLBlit_MulABDiv255(
                      A3::AlphaFromPixel(A3::GetPixel( kAlphaOffset )),
                      ubAlpha
                      )
                   );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {

              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              A2::BlendPixel(
                 kOffset, A1::GetPixel( kTexOffset ),
                 A3::AlphaFromPixel(A3::GetPixel( kAlphaOffset ))
                 );
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                A2::BlendPixel(
                   kOffset, A1::GetPixel( kTexOffset ),
                   A3::AlphaFromPixel(A3::GetPixel( kAlphaOffset ))
                   );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }              
          }
        }
        rx+= drx;
        lx+= dlx;
        
        lu+= dlu;
        lv+= dlv;
      }

    }
		


	static inline void RotBlit_RGBAlpha(eGML_Bitmap *pkSrc, eGML_Bitmap *pkAlpha, eGML_Bitmap *pkDst, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags, eC_Value vStretchX, eC_Value vStretchY, eC_Value vRotPosX, eC_Value vRotPosY, eC_UByte ubAlpha)
	{	
      eGML_RotBlitEdgeLeft aEdgesLeft[3];
      eGML_RotBlitEdgeRight aEdgesRight[3];

      eC_Int iEy = 0;
      eC_Int iSy = 0;

      eC_Int iClipLeft = pkDst->GetClipMinX();
      eC_Int iClipRight= pkDst->GetClipMaxX()+1;

      eC_Int iLeft = 0;
      eC_Int iRight= 0;

      eC_Int iEdgeCntLeft=0;
      eC_Int iEdgeCntRight=0;

      eC_Fixed lx = 0;
      eC_Fixed dlx = 0;
    
      eC_Fixed lv = 0;
      eC_Fixed dlv = 0;

      eC_Fixed lu = 0;
      eC_Fixed dlu = 0;

      eC_Fixed rx = 0;
      eC_Fixed drx = 0;
     
      eC_Fixed av = 0;
      eC_Fixed au = 0;

      eC_Bool boolAlpha = ubAlpha!=255;

      if (!eGML_RotBlitParam::RotBlitCalc(pkDst, uiWidth, uiHeight, uiSrcX, uiSrcY, 
                                     vDstX,  vDstY,  vAngle,  ulFlags,  vStretchX,  vStretchY,
                                     vRotPosX,  vRotPosY,  aEdgesLeft,  aEdgesRight,
                                     iEdgeCntLeft, iEdgeCntRight, iEy, au, av))
      {
        return;
      }


//Start Y - Value
      iSy = aEdgesRight[0].y1;

      for (eC_Int iY=iSy; iY<iEy; iY++)
      {

        O3 kAlphaOffset;
        O2 kOffset;
        O1 kTexOffset;


        eC_Fixed v;
        eC_Fixed u;

        eC_Int ex;
        eC_Int sx;

//Test for new right edge
        if (iRight<iEdgeCntRight)
        if (iY == aEdgesRight[iRight].y1 )
        {
          drx = aEdgesRight[iRight].dx;
           rx = aEdgesRight[iRight].sx;

          iRight++;
        }

//Test for new left edge
        if (iLeft<iEdgeCntLeft)
        if (iY == aEdgesLeft[iLeft].y1 )
        {
          dlx = aEdgesLeft[iLeft].dx;
           lx = aEdgesLeft[iLeft].sx;

           lu = aEdgesLeft[iLeft].u;
          dlu = aEdgesLeft[iLeft].du;

           lv = aEdgesLeft[iLeft].v;
          dlv = aEdgesLeft[iLeft].dv;

          iLeft++;
        }
        
        
        u= lu;
        v= lv;

        ex= eC_FixToInt(rx);
        sx= eC_FixToInt(lx);

//Check span is inside

        if (sx < iClipRight && ex > iClipLeft )
        {

//Clip Left
          if ( sx < iClipLeft)
          {
            u += au * (iClipLeft - sx);
            v += av * (iClipLeft - sx);
            sx= iClipLeft;
          }        

//Clip Right
          if ( ex > iClipRight)
          {
            ex= iClipRight;
          }        

          A2::GetOffset(pkDst, sx, iY, kOffset );

//Render Span
          if (ulFlags&eGML_Bitmap::BLIT_BILINEAR)
          {
            if (boolAlpha)
            {

              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              P1 pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
              eC_UByte r = A1::RedFromPixel( pxPixel );
              eC_UByte g = A1::GreenFromPixel( pxPixel );
              eC_UByte b = A1::BlueFromPixel( pxPixel );
              
              A2::BlendPixel(
                 kOffset, A2::PixelFromRGB( r, g, b ),
                 eGMLBlit_MulABDiv255(
                    A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu)), 
                    ubAlpha
                    )
                 );
              
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
                r = A1::RedFromPixel( pxPixel );
                g = A1::GreenFromPixel( pxPixel );
                b = A1::BlueFromPixel( pxPixel );
                
                A2::BlendPixel(
                   kOffset, A2::PixelFromRGB( r, g, b ),
                   eGMLBlit_MulABDiv255(
                      A3::AlphaFromPixel(A3::Blend4(pkAlpha, kAlphaOffset, fracv, fracu)),
                      ubAlpha
                      )
                   );

                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
            else
            {
              eC_UByte   fracv= eC_FixFracToByte(eC_FixFrac(v));
              eC_UByte   fracu= eC_FixFracToByte(eC_FixFrac(u));
              
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              P1 pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
              eC_UByte r = A1::RedFromPixel( pxPixel );
              eC_UByte g = A1::GreenFromPixel( pxPixel );
              eC_UByte b = A1::BlueFromPixel( pxPixel );
              
              A2::BlendPixel(
                 kOffset, A2::PixelFromRGB( r, g, b ),  
                 A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu ))
                 );
              
              
              A2::IncX( pkDst, kOffset );

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                fracv= eC_FixFracToByte(eC_FixFrac(v));
                fracu= eC_FixFracToByte(eC_FixFrac(u));
                
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                pxPixel  = A1::Blend4( pkSrc, kTexOffset, fracv, fracu );
                r = A1::RedFromPixel( pxPixel );
                g = A1::GreenFromPixel( pxPixel );
                b = A1::BlueFromPixel( pxPixel );
                
                A2::BlendPixel(
                   kOffset, A2::PixelFromRGB( r, g, b ),
                   A3::AlphaFromPixel(A3::Blend4( pkAlpha, kAlphaOffset, fracv, fracu ))
                   );
                
                
                A2::IncX( pkDst, kOffset );
                
                u+= au;
                v+= av;
              }
            }
          }
          else
          {
            if (boolAlpha)
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              P1 pxPixel  = A1::GetPixel( kTexOffset );
              eC_UByte r = A1::RedFromPixel( pxPixel );
              eC_UByte g = A1::GreenFromPixel( pxPixel );
              eC_UByte b = A1::BlueFromPixel( pxPixel );
              
              A2::BlendPixel(
                 kOffset, A2::PixelFromRGB( r, g, b ),
                 eGMLBlit_MulABDiv255(
                    A3::AlphaFromPixel(A3::GetPixel(kAlphaOffset)), 
                    ubAlpha
                    )
                 );
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                pxPixel  = A1::GetPixel( kTexOffset );
                r = A1::RedFromPixel( pxPixel );
                g = A1::GreenFromPixel( pxPixel );
                b = A1::BlueFromPixel( pxPixel );
                
                A2::BlendPixel(
                   kOffset, A2::PixelFromRGB( r, g, b ),
                   eGMLBlit_MulABDiv255(
                      A3::AlphaFromPixel(A3::GetPixel(kAlphaOffset)), 
                      ubAlpha
                      )
                   );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
            else
            {
              A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
              A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
              
              P1 pxPixel  = A1::GetPixel( kTexOffset );
              eC_UByte r = A1::RedFromPixel( pxPixel );
              eC_UByte g = A1::GreenFromPixel( pxPixel );
              eC_UByte b = A1::BlueFromPixel( pxPixel );
              
              A2::BlendPixel(
                 kOffset, A2::PixelFromRGB( r, g, b ),
                 A3::AlphaFromPixel(A3::GetPixel(kAlphaOffset))
                 );
              
              A2::IncX( pkDst, kOffset );         

              u+= eC_FixMul( au, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              v+= eC_FixMul( av, (eC_FixFromInt(1)-eC_FixFrac(lx)) );
              sx++;

              for (int x= sx ;x<ex ;x++)
              {
                A1::GetOffset(pkSrc, eC_FixToInt(v), eC_FixToInt(u), kTexOffset);
                A3::GetOffset(pkAlpha, eC_FixToInt(v), eC_FixToInt(u), kAlphaOffset);
                
                pxPixel  = A1::GetPixel( kTexOffset );
                r = A1::RedFromPixel( pxPixel );
                g = A1::GreenFromPixel( pxPixel );
                b = A1::BlueFromPixel( pxPixel );
                
                A2::BlendPixel(
                   kOffset, A2::PixelFromRGB( r, g, b ),
                   A3::AlphaFromPixel(A3::GetPixel(kAlphaOffset))
                   );
                
                A2::IncX( pkDst, kOffset );         
                u+= au;
                v+= av;
              }
            }
          }
        }
        rx+= drx;
        lx+= dlx;
        
        lu+= dlu;
        lv+= dlv;
      }	
    }


};


//---------------------------------------------------------------	
#endif

