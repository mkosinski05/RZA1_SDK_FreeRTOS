/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_SpandrawTex.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:20:02 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_SPANDRAWTEX__H_
#define EGML_SPANDRAWTEX__H_
//---------------------------------------------------------------	
#include "eDBG_Statistic.h"
#include "eGML_Config.h"
#include "eC_Math.h"
#include "eGML_BitmapA8.h"

//---------------------------------------------------------------
typedef void* eGML_Polydraw_data;
//#define eGML_SPANDRAWTEX_AVOIDBRANCH	// set when unpredicted branches hurt performance more than unnecessary readwrites

#ifdef eGML_TEXTURECHECK
struct eGML_Polygon_TextureTargetArea
{
   eC_UInt m_uiMinX;
   eC_UInt m_uiMinY;
   eC_UInt m_uiMaxX;
   eC_UInt m_uiMaxY;
};
#endif




#ifndef eGML_POLYGONTEXTURE_DISABLE
//---------------------------------------------------------------
// Texture Pixel Writeback
//
template <
		  typename P1,		// Pixeltype for Destination/Texture
		  typename P2,		// Offsettype for Destination/Texture
		  class A1			// Accessmethods for Destination/Texture
		 > 

class eGML_WritebackTex_Default
{
public:	
	static inline void BlendTexel(const P2 &rkSrc, const P2 &rkDest, const eC_Fixed u, const eC_Fixed v, 
								  const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eC_UInt uiN,
								  const eGML_Polydraw_data &rData)
	{
		P2 kTex;

	    // get texel offset
        A1::TextureOffset( rkSrc, eC_FixToInt(u & uiMaskX), eC_FixToInt(v & uiMaskY), uiN, kTex );

#ifdef eGML_TEXTURE_USE_ALPHA
        if(A1::AlphaFromPixel(A1::GetPixel( kTex )))
#endif
        {
            // read texel and write pixel
            A1::PutPixel( rkDest,  A1::GetPixel( kTex ) );	
        }
	}	
};

//---------------------------------------------------------------
template <typename P1, typename P2, class A1> 
class eGML_WritebackTex_Alpha
{
public:	
	static inline void BlendTexel(const P2 &rkSrc, const P2 &rkDest, const eC_Fixed u, const eC_Fixed v, 
								  const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eC_UInt uiN,
								  const eGML_Polydraw_data &rData)
	{
		P2 kTex;

	    // get texel offset
        A1::TextureOffset( rkSrc, eC_FixToInt(u & uiMaskX), eC_FixToInt(v & uiMaskY), uiN, kTex );

		// read texel and write pixel
		A1::BlendPixel( rkDest, A1::GetPixel( kTex ), (eC_UByte) ((eC_UInt)rData) );	
	}	
};

//---------------------------------------------------------------
template <typename P1, typename P2, class A1> 
class eGML_WritebackTex_Additive
{
public:	
	static inline void BlendTexel(const P2 &rkSrc, const P2 &rkDest, const eC_Fixed u, const eC_Fixed v,
								  const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eC_UInt uiN, 
								  const eGML_Polydraw_data &rData)
	{
		P2 kTex;

	    // get texel offset
        A1::TextureOffset( rkSrc, eC_FixToInt(u & uiMaskX), eC_FixToInt(v & uiMaskY), uiN, kTex );

		// read texel and mix additive
		A1::AddPixel( rkDest, A1::GetPixel( kTex ) );	
	}	
};

//---------------------------------------------------------------
template <typename P1, typename P2, class A1> 
class eGML_WritebackTex_Color
{
public:	
	static inline void BlendTexel(const P2 &rkSrc, const P2 &rkDest, const eC_Fixed u, const eC_Fixed v,
								  const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eC_UInt uiN, 
								  const eGML_Polydraw_data &rData)
	{
		eGML_OffsetA8 kTex;
		eGML_PixelA8  kPix;

	    // get texel 
        eGML_PixelA8_Access::TextureOffset( (eGML_OffsetA8) EGML_CAST(eGML_OffsetA8,rkSrc), eC_FixToInt(u & uiMaskX), eC_FixToInt(v & uiMaskY), uiN, kTex );
		kPix = eGML_PixelA8_Access::GetPixel( kTex );

		// read texel and use it as alpha weight
		#ifndef eGML_SPANDRAWTEX_AVOIDBRANCH
		if (kPix > 0) 
		#endif
		A1::BlendPixel( rkDest, A1::ColorToPixel((eGML_Color) rData), (eC_UByte) kPix );	
	}	
};

//---------------------------------------------------------------
template <typename P1, typename P2, class A1> 
class eGML_WritebackTex_ColorAlpha
{
public:	
	static inline void BlendTexel(const P2 &rkSrc, const P2 &rkDest, const eC_Fixed u, const eC_Fixed v,
								  const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eC_UInt uiN, 
								  const eGML_Polydraw_data &rData)
	{
		eGML_OffsetA8 kTex;
		eGML_PixelA8  kPix;

	    // get texel 
        eGML_PixelA8_Access::TextureOffset( (eGML_OffsetA8) EGML_CAST(eGML_OffsetA8,rkSrc), eC_FixToInt(u & uiMaskX), eC_FixToInt(v & uiMaskY), uiN, kTex );
		kPix = (eGML_PixelA8)(  (eGML_PixelA8_Access::GetPixel( kTex ) * (((eC_UInt)rData)>>24)  )  >> 8  );

		// read texel and use it as alpha weight
		#ifndef eGML_SPANDRAWTEX_AVOIDBRANCH
		if (kPix > 0) 
		#endif
		A1::BlendPixel( rkDest, A1::ColorToPixel((eGML_Color) ((eC_UInt)rData&0xffffff) ), (eC_UByte) kPix );	
	}	
};


//---------------------------------------------------------------
// Horizontal Span Renderer - Texturemapped, No AntiAliasing
//
template <
		  typename P1,		// Pixeltype for Destination/Texture
		  typename P2,		// Offsettype for Destination/Texture
		  class A1,			// Accessmethods for Destination/Texture
		  class W1			// Pixel Writeback methods
		 > 

class eGML_HSpanRenderTex
{
public:

	// parameters are: minor axis pos, major axis start, major axis stop, texture mipmap, initial uv, uv increment
	static inline void RenderSpan(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, 
                                  const P2 &rTexture, eC_Fixed u, eC_Fixed v, eC_Fixed ux, eC_Fixed vx, 
                                  const eC_UInt uiN, const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
                                  ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif
                                  )
	{
#ifdef eGML_TEXTURECHECK
      eC_UInt uiTargetX = 0;
      eC_UInt uiTargetY = 0;
#endif
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kDst;
		
        // get first pixel of destination
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kDst );

        // subtexel correction and scanstart interpolation
        u += ux*x1i;
        v += vx*x1i;

        // full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
#ifdef eGML_TEXTURECHECK
         //Check Texture area before the texture pixel is blend
         uiTargetX = eC_FixToInt(u & uiMaskX);
         uiTargetY = eC_FixToInt(v & uiMaskY);
         if(   (pTexturArea == NULL) || 
             !((uiTargetX < pTexturArea->m_uiMinX) ||
               (uiTargetX > pTexturArea->m_uiMaxX) ||
               (uiTargetY < pTexturArea->m_uiMinY) ||
               (uiTargetY > pTexturArea->m_uiMaxY)) )
         {
            // fetch texel and write pixel		
			   W1::BlendTexel(rTexture, kDst, u, v, uiMaskX, uiMaskY, uiN, rData);
         }
         else
         {
//            eDBG_DP("Tx:%d Ty:%d\tminx:%d maxx:%d miny:%d maxy: %d",uiTargetX, uiTargetY,pTexturArea->m_uiMinX,pTexturArea->m_uiMaxX,pTexturArea->m_uiMinY,pTexturArea->m_uiMaxY);
         }
#else
            // fetch texel and write pixel		
			   W1::BlendTexel(rTexture, kDst, u, v, uiMaskX, uiMaskY, uiN, rData);
#endif
			// increment dest
			A1::IncX( pBitmap, kDst );
            // increment u/v
            u += ux;
            v += vx;
		}
	}


    #ifdef eGML_TEXTUREFLOAT
    // floatingpoint perspective correct mapping
    static inline void RenderSpanPersp(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, 
                                  const P2 &rTexture, eC_Float uz, eC_Float vz, eC_Float zi, eC_Float ux, eC_Float vx, eC_Float zx,
                                  const eC_UInt uiN, const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
                                  ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif
                                  )
    #else
    // fixedpoint perspective correct mapping
    static inline void RenderSpanPersp(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, 
                                  const P2 &rTexture, eC_Fixed uz, eC_Fixed vz, eC_Fixed zi, eC_Fixed ux, eC_Fixed vx, eC_Fixed zx,
                                  const eC_UInt uiN, const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
                                  ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif                                  
                                  )
    #endif
	{		
#ifdef eGML_TEXTURECHECK
      eC_UInt uiTargetX = 0;
      eC_UInt uiTargetY = 0;
#endif
		eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kDst;
        eC_Fixed u,v;
        #ifdef eGML_TEXTUREFLOAT
        eC_Float z;
        #else
        eC_Fixed z;
        #endif
		
        // get first pixel of destination
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kDst );

        // subtexel correction and scanstart interpolation
        zi += zx*x1i;
        uz += ux*x1i;
        vz += vx*x1i;

        // full pixels
		for (eC_Int x=x1i; x<=x2i; x++)
		{
            // perspective correction
            #ifdef eGML_TEXTUREFLOAT
            z = eC_FltReci( zi );
            u = eC_FixFromFloat(uz * z);
            v = eC_FixFromFloat(vz * z);
            #else            
            z = eC_FixReci( zi );
            u = eC_FixMul(uz, z);
            v = eC_FixMul(vz, z);
            #endif


#ifdef eGML_TEXTURECHECK
         //Check Texture area before the texture pixel is blend
         uiTargetX = eC_FixToInt(u & uiMaskX);
         uiTargetY = eC_FixToInt(v & uiMaskY);
         if(   (pTexturArea == NULL) || 
             !((uiTargetX < pTexturArea->m_uiMinX) ||
               (uiTargetX > pTexturArea->m_uiMaxX) ||
               (uiTargetY < pTexturArea->m_uiMinY) ||
               (uiTargetY > pTexturArea->m_uiMaxY)) )
         {
            // fetch texel and write pixel		
			   W1::BlendTexel(rTexture, kDst, u, v, uiMaskX, uiMaskY, uiN, rData);
         }
         else
         {
//            eDBG_DP("Tx:%d Ty:%d\tminx:%d maxx:%d miny:%d maxy: %d",uiTargetX, uiTargetY,pTexturArea->m_uiMinX,pTexturArea->m_uiMaxX,pTexturArea->m_uiMinY,pTexturArea->m_uiMaxY);
         }
#else
         // fetch texel and write pixel
			W1::BlendTexel(rTexture, kDst, u, v, uiMaskX, uiMaskY, uiN, rData);
#endif

			// increment dest         
			A1::IncX( pBitmap, kDst );
            // increment u/v
            zi += zx;
            uz += ux;
            vz += vx;
		}
	}


    #ifdef eGML_TEXTUREFLOAT
    // floatingpoint perspective correct / affine subdivision mapping
    static inline void RenderSpanSubdivision(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, 
                                  const P2 &rTexture, eC_Float uz, eC_Float vz, eC_Float zi, eC_Float ux, eC_Float vx, eC_Float zx,
                                  const eC_UInt uiN, const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
                                  ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif
                                  )
    #else
    // fixedpoint perspective correct / affine subdivision mapping
    static inline void RenderSpanSubdivision(eGML_Bitmap *pBitmap, eC_Fixed y, eC_Fixed x1, eC_Fixed x2, 
                                  const P2 &rTexture, eC_Fixed uz, eC_Fixed vz, eC_Fixed zi, eC_Fixed ux, eC_Fixed vx, eC_Fixed zx,
                                  const eC_UInt uiN, const eC_UInt uiMaskX, const eC_UInt uiMaskY, const eGML_Polydraw_data &rData
#ifdef eGML_TEXTURECHECK
                                  ,const eGML_Polygon_TextureTargetArea *pTexturArea=NULL
#endif                                  
                                  )
    #endif
	{
#ifdef eGML_TEXTURECHECK
      eC_UInt uiTargetX = 0;
      eC_UInt uiTargetY = 0;
#endif
        eC_Int x1i = eC_FixToInt(x1);
		eC_Int x2i = eC_FixToInt(x2);
		P2   kDst;
        eC_Fixed ul,vl,ur,vr,du,dv;
        #ifdef eGML_TEXTUREFLOAT
        eC_Float zl,zr, uxa,vxa,zxa;
        #else
        eC_Fixed zl,zr, uxa,vxa,zxa;
        #endif
        eC_Int iWidth = (x2i-x1i) + 1;

        // drop illegal spans
        if (iWidth <= 0) return;

        // get first pixel of destination
		A1::GetOffset( pBitmap, x1i, eC_FixToInt(y), kDst );        

        // subtexel correction and scanstart interpolation
        zi += zx*x1i;
        uz += ux*x1i;
        vz += vx*x1i;

        // initial (left) u/v
        #ifdef eGML_TEXTUREFLOAT
        zl = eC_FltReci( zi );
        ul = eC_FixFromFloat(uz * zl);
        vl = eC_FixFromFloat(vz * zl);
        #else            
        zl = eC_FixReci( zi );
        ul = eC_FixMul( uz, zl );
        vl = eC_FixMul( vz, zl );
        #endif

        // prepare subdivision
        int iSubdivisions = iWidth / eGML_TEXTURESTEP;
        int iModLength =  iWidth % eGML_TEXTURESTEP;

        if (iSubdivisions>0)
        {
            uxa = ux * eGML_TEXTURESTEP;
            vxa = vx * eGML_TEXTURESTEP;
            zxa = zx * eGML_TEXTURESTEP;
                        
            do 
            {
                uz += uxa;
                vz += vxa;
                zi += zxa;

                // target (right) u/v
                #ifdef eGML_TEXTUREFLOAT
                zr = eC_FltReci( zi );
                ur = eC_FixFromFloat(uz * zr);
                vr = eC_FixFromFloat(vz * zr);
                #else            
                zr = eC_FixReci( zi );
                ur = eC_FixMul( uz, zr );
                vr = eC_FixMul( vz, zr );
                #endif       
                // deltas
                du = (ur - ul) / eGML_TEXTURESTEP;
                dv = (vr - vl) / eGML_TEXTURESTEP;
                                
                // render affine subspan
                for (eC_Int x=0; x<eGML_TEXTURESTEP; x++)
                {
#ifdef eGML_TEXTURECHECK
                  //Check Texture area before the texture pixel is blend
                  uiTargetX = eC_FixToInt(ul & uiMaskX);
                  uiTargetY = eC_FixToInt(vl & uiMaskY);
                  if(   (pTexturArea == NULL) || 
                     !((uiTargetX < pTexturArea->m_uiMinX) ||
                        (uiTargetX > pTexturArea->m_uiMaxX) ||
                        (uiTargetY < pTexturArea->m_uiMinY) ||
                        (uiTargetY > pTexturArea->m_uiMaxY)) )
                  {
                     // fetch texel and write pixel		
			            W1::BlendTexel(rTexture, kDst, ul, vl, uiMaskX, uiMaskY, uiN, rData);
                  }
                  else
                  {
         //            eDBG_DP("Tx:%d Ty:%d\tminx:%d maxx:%d miny:%d maxy: %d",uiTargetX, uiTargetY,pTexturArea->m_uiMinX,pTexturArea->m_uiMaxX,pTexturArea->m_uiMinY,pTexturArea->m_uiMaxY);
                  }
#else
                   // fetch texel and write pixel
                   W1::BlendTexel(rTexture, kDst, ul, vl, uiMaskX, uiMaskY, uiN, rData);
#endif
					// increment dest			
                    A1::IncX( pBitmap, kDst );
                    // increment u/v
                    ul += du;
                    vl += dv;
                }

                zl = zr;
                ul = ur;
                vl = vr;
            } while (--iSubdivisions > 0);
        }
                
        if (iModLength>0)
        {
            // skip to end of span
            uz += ux * iModLength;
            vz += vx * iModLength;
            zi += zx * iModLength;
            // target (right) u/v
            #ifdef eGML_TEXTUREFLOAT
            zr = eC_FltReci( zi );
            ur = eC_FixFromFloat(uz * zr);
            vr = eC_FixFromFloat(vz * zr);
            #else            
            zr = eC_FixReci( zi );
            ur = eC_FixMul( uz, zr );
            vr = eC_FixMul( vz, zr );
            #endif   
            // deltas
            du = (ur - ul) / iModLength;
            dv = (vr - vl) / iModLength;
            
            // render affine endspan
            for (eC_Int x=0; x<iModLength; x++)
            {
#ifdef eGML_TEXTURECHECK
               //Check Texture area before the texture pixel is blend
               uiTargetX = eC_FixToInt(ul & uiMaskX);
               uiTargetY = eC_FixToInt(vl & uiMaskY);
               if(   (pTexturArea == NULL) || 
                  !((uiTargetX < pTexturArea->m_uiMinX) ||
                     (uiTargetX > pTexturArea->m_uiMaxX) ||
                     (uiTargetY < pTexturArea->m_uiMinY) ||
                     (uiTargetY > pTexturArea->m_uiMaxY)) )
               {
                  // fetch texel and write pixel		
			         W1::BlendTexel(rTexture, kDst, ul, vl, uiMaskX, uiMaskY, uiN, rData);
               }
               else
               {
      //            eDBG_DP("Tx:%d Ty:%d\tminx:%d maxx:%d miny:%d maxy: %d",uiTargetX, uiTargetY,pTexturArea->m_uiMinX,pTexturArea->m_uiMaxX,pTexturArea->m_uiMinY,pTexturArea->m_uiMaxY);
               }
#else
               // fetch texel and write pixel
               W1::BlendTexel(rTexture, kDst, ul, vl, uiMaskX, uiMaskY, uiN, rData);
#endif
				// increment dest           
                A1::IncX( pBitmap, kDst );
                // increment u/v
                ul += du;
                vl += dv;
            }
        }
    }

};
#endif

//---------------------------------------------------------------	
#endif
