/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Bitmap.h
* %version:        134.1 %
* 
* author:          hh04031
* creation date:   Tue Aug 09 13:49:13 2005
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_BITMAP__H_
#define EGML_BITMAP__H_

//---------------------------------------------------------------
#include "eC_Types.h"
#include "eC_Memory.h"
#include "eGML_Config.h"
#include "eGML_Vector2.h"
#include "eGML_Vector3.h"
#include "eGML_Matrix2.h"
#include "eGML_Matrix3.h"

class eGML_Primitive;
class eGML_Screen;
struct eGML_Coord2D;

#ifdef eC_TARGET_GFX_FUJITSUGDC
class eGML_FGDC_Bitmap;
class eGML_FGDC_BitmapRGB555;
class eGML_FGDC_BitmapRGB32;
class eGML_FGDC_BitmapA8;
#endif

//---------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// flags are structured internally to simplify propagating flags
// this structure is not relevant to the application
#define eGML_FLAG_GENERAL_MASK   0x7fff0000
#define eGML_FLAG_LOCAL_MASK     0x0000ffff
#define eGML_FLAG_STYLE_MASK     0x7f000000
#define eGML_FLAG_FILTER_MASK    0x000f0000
#define eGML_FLAG_TRANSFORM_MASK 0x00f00000

#define eGML_FLAG_GENERAL        0x00010000
#define eGML_FLAG_LOCAL          0x00000001
#define eGML_FLAG_STYLE          0x01000000
#define eGML_FLAG_FILTER         0x00010000
#define eGML_FLAG_TRANSFORM      0x00100000

//---------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
eDBG_PERF_DEF( BitmapClear );

//---------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// generic pixel type
typedef eC_UByte eGML_Pixel;

//---------------------------------------------------------------
/**
@memo
Abstract bitmap baseclass.
@doc
A bitmap represents a region of memory that can be used as rendering target. In order to render into a bitmap you have to \Ref{Lock} it
first and \Ref{Unlock} it afterwards. A bitmap that can be displayed is called {@link eGML_Screen screen}, additional bitmaps are usually
created to hold icons or other re-used graphic elements which are {@link Blit blitted} into the screen to make them visible. <BR>
There is one childclass for each \Ref{BitmapType}. The type of a bitmap is defined by its internal layout.
Some bitmaps encode RGB data with one double word (32bit) per pixel, others use 16bit (5bit red, 6bit green, 5bit blue) or
8bit and a LUT (indexed modes). The bitmap types defined in \Ref{BitmapType} are called <I>basic</I> types.
Specialised implementations of <B>eGML</B> provide additional <I>native</I> modes that are supported only on their concrete hardware
platform. Basic modes in contrast are guaranteed to work on all platforms.
When blitting bitmaps <B>eGML</B> will automatically convert formats (refer to \Ref{Blit} for details). <BR>
For getting more information about \Ref{eGML_Bitmap} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Bitmap_1.cpp]{examples/example_Bitmap_1.cpp.html} <BR>
@see eGML_Screen, eGML
@see Tutorial_eGML
*/
class eGML_Bitmap
{
public:
  
/** Flags used for Blit function.
  @see Blit */
  enum DrawBlit_Flags
  {
    /// Skip source and destination clip tests when blitting. Use this flags to speed up blits when you are 100% sure that the blit will be unclipped.
    BLIT_NOCLIP   = eGML_FLAG_TRANSFORM * 1,
      /// Use bilinear filtering during blit operations. (4x memory bandwidth).
      BLIT_BILINEAR = eGML_FLAG_FILTER * 4,
      /// Do correct the aspect ratio when blitting.
      BLIT_ASPECT   = eGML_FLAG_TRANSFORM * 2,
      /// Use the provided colorkey during blitting
      BLIT_COLORKEY = eGML_FLAG_LOCAL * 1,
      /// Copy alpha values during blit
      BLIT_COPY_ONLY_ALPHA = eGML_FLAG_FILTER * 32
  };
  
  /** Flags used for DrawPrimitive function.
  @see DrawPrimitive */
  enum DrawPrimitive_Flags
  {
      /// Render points.
      PRIM_POINTS       = eGML_FLAG_LOCAL *  1,
      /// Render lines/polylines.
      PRIM_LINES        = eGML_FLAG_LOCAL *  2,
      /// Render a filled polygon.
      PRIM_FILLED       = eGML_FLAG_LOCAL *  4,
      /// Close the polyline (polygons are always closed).
      PRIM_CLOSED       = eGML_FLAG_LOCAL *  8,
      /// Round Endpoint
      PRIM_ROUND_START = eGML_FLAG_LOCAL * 2048,
      /// Round Endpoint
      PRIM_ROUND_END  = eGML_FLAG_LOCAL * 4096,
      /// Round Line Connection
      PRIM_ROUND_CONNECT  = eGML_FLAG_LOCAL * 16,
      /// Round line start, end, connect (PRIM_ROUND_START|PRIM_ROUND_END|PRIM_ROUND_CONNECT).
      PRIM_ROUND        = PRIM_ROUND_START|PRIM_ROUND_END|PRIM_ROUND_CONNECT,
      /// Use index buffer to render. this flag can be combined with \Ref{PRIM_LINES}, \Ref{PRIM_FILLED}.
      PRIM_INDEX        = eGML_FLAG_LOCAL * 32,
      /// Use color from the index attribute buffer to render.
      PRIM_INDEXCOLOR   = eGML_FLAG_LOCAL * 64,
      //
      PRIM_FILLEDEXTRUDE = eGML_FLAG_LOCAL * 128,
      // polygon winding is known to be clockwise
      PRIM_HINT_CW      = eGML_FLAG_LOCAL * 256,
      // polygon winding is known to be counter clockwise
      PRIM_HINT_CCW     = eGML_FLAG_LOCAL * 512,
      // !!! INTERNAL USE ONLY !!! internal flag for rhw data
      PRIM_RHWPRESENT   = eGML_FLAG_LOCAL * 1024,
      /// Modify primitive width based on depth.
      PRIM_3DWIDTH      = eGML_FLAG_TRANSFORM * 8,
      /// Primitive is entirely visible, no need to apply clipping.
      PRIM_NOCLIP       = eGML_FLAG_TRANSFORM * 1,
      /// Use antialiasing when rendering.
      PRIM_ANTIALIAS    = eGML_FLAG_FILTER * 1,
      /// Use high quality (HQ) antialiasing (AA).
      PRIM_HQAA         = eGML_FLAG_FILTER * 2,
      /// Do not apply the culling rules.
      PRIM_NOCULLING    = eGML_FLAG_TRANSFORM * 4,
      /// Automatically advance linepattern offset
      PRIM_ADVANCEPATTERN = eGML_FLAG_STYLE * 16 
  };
  
  /** Flags used for DrawPolygon function.
  @see DrawPolygon */
	enum DrawPolygon_Flags 
	{ 
		POLY_NONE = 0,
		POLY_ANTIALIAS       = eGML_FLAG_FILTER * 1,		 // use antialiasing
		POLY_HQAA            = eGML_FLAG_FILTER * 2,		 // use filtered (gaussian) AA (performance impact!)
		POLY_NOCLIP          = eGML_FLAG_TRANSFORM * 1,  // skip clipping, entire polygon is guaranteed to be onscreen
		POLY_USE_RENDERSTATE = eGML_FLAG_LOCAL * 1,      // use renderstate for context data
		POLY_EXTRUDE         = eGML_FLAG_LOCAL * 128,    // Extrude aa-polygon
		POLY_HINT_CW		     = eGML_FLAG_LOCAL * 256,	   // polygon winding is known to be clockwise
		POLY_HINT_CCW		     = eGML_FLAG_LOCAL * 512	   // polygon winding is known to be counter clockwise
	};
  
  /** Flags used for DrawLine function.
  @see DrawLine */
  enum DrawLine_Flags
  {
    /// No special attributes are set.
    LINE_NONE   = 0,
      /// Apply linewidth in direction of line.
      LINE_WIDE_S = eGML_FLAG_STYLE * 1, 
      /// Apply linewidth perpendicular to line.
      LINE_WIDE_T = eGML_FLAG_STYLE * 2, 
      /// Combination of both kinds of linewidth. Result of bitwise <B>OR</B>-operation with \Ref{eGML_LF_WIDE_S} and \Ref{eGML_LF_WIDE_T}.
      LINE_WIDE   = eGML_FLAG_STYLE * 3, 
      /// Apply linewidth in direction of line twice.
      LINE_WIDE_2 = eGML_FLAG_STYLE * 4, 
      /// Use antialiasing.
      LINE_ANTIALIAS   = eGML_FLAG_FILTER * 1, 
      /// Use filtered (gaussian) antialiasing (AA). But be careful because there might be a performance impact.
      LINE_HQAA        = eGML_FLAG_FILTER * 2, 
      /// Use rounded corner for startpoint. Do not combine with \Ref{eGML_LF_WIDE_S}.
      LINE_ROUND_START = eGML_FLAG_LOCAL * 1, 
      /// Use rounded corner for endpoint. Do not combine with \Ref{eGML_LF_WIDE_S}.
      LINE_ROUND_END   = eGML_FLAG_LOCAL * 2, 
      /// Skip clipping, line is guaranteed to be on screen.
      LINE_NOCLIP     = eGML_FLAG_TRANSFORM * 1, 
      // Pattern on line.
      LINE_PATTERN    = eGML_FLAG_STYLE * 8, 
	  // Automatically advance linepattern offset
	  LINE_ADVANCEPATTERN = eGML_FLAG_STYLE * 16 
  };
  
  /** Flags used for DrawDot function.
  @see DrawDot */
  enum DrawDot_Flags
  {
    /// No special attributes are set.
    DOT_NONE       = 0,
      /// Use antialiasing.
      DOT_ANTIALIAS  = eGML_FLAG_FILTER * 1, 
      /// Use filtered (gaussian) antialiasing (AA). But be careful because there might be a performance impact.
      DOT_HQAA       = eGML_FLAG_FILTER * 2, 
      /// Skip clipping, line is guaranteed to be on screen.
      DOT_NOCLIP     = eGML_FLAG_TRANSFORM * 1 
  };
  
  /* !!! INTERNAL USE ONLY !!!
  if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
  EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
    
  /** Deletes bitmap object, calls private destructor.
  @see CreateBitmap */
  virtual void Release() { delete this; }
  
  /** Gains access to bitmap. To obtain write access to the bitmap you have to lock it. Locks are exclusive and can not be nested.
  Locking and {@link Unlock unlocking} can be an expensive operation, therefore you should try to minimise the number of
  Lock/Unlock operations per frame. On the other hand since the lock is exclusive the bitmap can not be displayed or be source of
  a {@link Blit blit} while beeing locked (this would require the <B>eGML</B> itself to lock it). Ideally you would encapsulate all drawing operations
  to a bitmap within one Lock/Unlock block and keep all timeconsuming calculations out of this block to minimise the lock duration.
  Do not use the pointer returned by Lock for write access after the bitmap has been unlocked. This could lead to an access violation at worst
  or your changes beeing partitially ignored at best. <BR>
  Note: a lock/unlock is not necessary for readonly access. (although if you have not locked the bitmap at least once, you will not have the pointer
  to it. This is ok since the bitmap will not contain meaningfull data anyway if it has not been locked/written to at least once).
  If the bitmap could not be locked for some reason NULL is returned and an error code is set.
  Do not access the pixel data directly if you are not absolutly sure about its organisation.
  @return Pointer to the bitmap data in memory. You can ignore the return value if you will draw using <B>eGML</B> functions only.
  @see Unlock, Blit */
  virtual void * Lock() = 0;
  
  /** Signals end of bitmap manipulation. To release a lock obtained with \Ref{Lock} you call this function.
  It can be an expensive operation because on some hardware your changes might have to be transfered into another memory region
  or converted into a different bitmap type (i.e. in case the bitmap is a {@link eGML_Screen screen} but no hardware exists to directly display
  the choosen \Ref{BitmapType}).<BR>
  You can simply call Unlock without any parameters or you can specify a rectangle to describe which region of the bitmap has been modified
  during the Lock. This information is used as a hint to optimise conversion/copy operations if they are necessary.
  @param uiX1 Left border of dirty region.
  @param uiY1 Top border of dirty region.
  @param uiX2 Right border of dirty region.
  @param uiY2 Bottom border of dirty region.
  @see Lock, Blit */
  virtual void Unlock(eC_UInt uiX1 = 0, eC_UInt uiY1 = 0, eC_UInt uiX2 = 0, eC_UInt uiY2 = 0) = 0;
  
  /** Copys part of a bitmap into another bitmap. The destination rectangle will be clipped to both source and destination before the actual blit
  begins. Conversion of different bitmap types will happen if both source and destination are basic \Ref{BitmapType} or the destination is a
  native type. Conversion from native types to basic types and conversion from true color to indexed modes is <I>not</I> supported. <BR>
  The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The sourcebitmap does not have to be locked/unlocked since it is accessed read only.
  @param pkSource Pointer to the sourcebitmap.
  @param uiWidth Width of the region to copy pixels in.
  @param uiHeight Height of the region to copy pixels in.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param cColorKey color that will be used as transparent. Enable colorkey using \Ref{DrawBlit_Flags} BLIT_COLORKEY.
  @see Lock, Unlock, DrawBlit_Flags, StretchBlit, RotBlit, BlitAlpha, BlitColor */
  virtual void Blit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight,
    eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0)
  { 
      (void)pkSource; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)iDstX; (void)iDstY; (void)ulFlags;
      (void)ubAlpha; (void)cColorKey;
      eDBG_LOG( " Blit not implemented");
  }
  
  /** Copys part of a bitmap weigthed by an alphabitmap. The dest rectangle will be clipped (refer to \Ref{Blit} for more information).
  @param pkSource Pointer to the sourcebitmap.
  @param pkAlpha Pointer to the alphabitmap.
  @param uiWidth Width of the region to copy pixels in.
  @param uiHeight Height of the region to copy pixels in.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, Blit, BlitColor */
  virtual void BlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight,
    eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkSource; (void)pkAlpha; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)iDstX; (void)iDstY;
      (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " BlitAlpha not implemented");
  }
  
  /** Fills part of a bitmap with a color weighted by an alphabitmap. This function can be used for text blitting (refer to \Ref{Blit} for more information).
  @param pkAlpha Pointer to alpha bitmap.
  @param uiWidth Width of the region to copy pixels in.
  @param uiHeight Height of the region to copy pixels in.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param cCol Color to blend with destination bitmap.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, Blit, BlitAlpha */
  virtual void BlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkAlpha; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)iDstX; (void)iDstY; (void)cCol;
      (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " BlitColor not implemented");
  }
  /** Fills part of a bitmap with a color weighted by an alphabitmap. This function can be used for text blitting (refer to \Ref{Blit} for more information). Used for 2-color blitting.
  @param pkAlpha Pointer to alpha bitmap.
  @param uiWidth Width of the region to copy pixels in.
  @param uiHeight Height of the region to copy pixels in.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param cCol Color to blend with destination bitmap.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param col1 first color
  @param col2 second color
  @see Lock, Unlock, DrawBlit_Flags, Blit, BlitAlpha */
  virtual void Blit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 col2, eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkAlpha; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)iDstX; (void)iDstY; (void)cCol1; (void)col2; 
      (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " Blit2Color not implemented"); 
  }


  /** Copys and stretches part of a bitmap into another bitmap. The destination rectangle will be clipped before the actual blit begins. Conversion of
  different bitmap types will happen if both source and destination are basic {@link BitmapType BitmapTypes} or the destination is a native
  type. Conversion from native types to basic types and conversion from true color to indexed modes is <I>not</I> supported.
  The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The source bitmap does not have to be locked/unlocked since it is accessed read only.
  @param pkSource Pointer to the sourcebitmap.
  @param uiSrcWidth Width of region to copy from in pixels.
  @param uiSrcHeight Height of region to copy from in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left osition to copy from.
  @param uiDstWidth Width of region to copy to in pixels.
  @param uiDstHeight Height of region to copy to in pixels.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param cColorKey color that will be used as transparent. Enable colorkey using \Ref{DrawBlit_Flags} BLIT_COLORKEY.
  @see Lock, Unlock, DrawBlit_Flags, Blit, RotBlit, StretchBlitAlpha, StretchBlitColor */
  virtual void StretchBlit(eGML_Bitmap *pkSource, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0)
  {
      (void)pkSource; (void)uiSrcWidth; (void)uiSrcHeight; (void)uiSrcX; (void)uiSrcY;
      (void)uiDstWidth; (void)uiDstHeight; (void)iDstX; (void)iDstY;
      (void)ulFlags; (void)ubAlpha; (void)cColorKey;
      eDBG_LOG( " Stretchblit not implemented");
  }
  
  /** Copys and stretches part of a bitmap into another bitmap by using an alpha bitmap. The destination rectangle will be clipped before the actual blit
  begins. Conversion of different bitmap types will happen if both source and destination are basic types ({@link BitmapType BitmapTypes})
  or the destination is a native type. Conversion from native types to basic types and conversion from true color to indexed modes is
  <I>not</I> supported. The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The source- and alphabitmaps do not have to be locked/unlocked since they are accessed read only.
  @param pkSource Pointer to the sourcebitmap.
  @param pkAlpha Pointer to the alphabitmap.
  @param uiSrcWidth Width of region to copy from in pixels.
  @param uiSrcHeight Height of region to copy from in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param uiDstWidth Width of region to copy to in pixels.
  @param uiDstHeight Height of region to copy to in pixels.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, StretchBlit, StretchBlitColor */
  virtual void StretchBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight,
    eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY,
    eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkSource; (void)pkAlpha; (void)uiSrcWidth; (void)uiSrcHeight; (void)uiSrcX; (void)uiSrcY;
      (void)uiDstWidth; (void)uiDstHeight; (void)iDstX; (void)iDstY;
      (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " StretchblitAlpha not implemented");
  }
  
  /** Fills and stretches part of a bitmap with a color weighted by an alphabitmap. The destination rectangle will be clipped before the actual blit begins.
  The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The alphabitmap does not have to be locked/unlocked since it is accessed read only.
  @param pkAlpha Pointer to the sourcebitmap.
  @param uiSrcWidth Width of region to copy from in pixels.
  @param uiSrcHeight Height of region to copy from in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param uiDstWidth Width of region to copy to in pixels.
  @param uiDstHeight Height of region to copy to in pixels.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param cCol Color to fill with.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, StretchBlit, StretchBlitAlpha */
  virtual void StretchBlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY,
    eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkAlpha; (void)uiSrcWidth; (void)uiSrcHeight; (void)uiSrcX; (void)uiSrcY;
      (void)uiDstWidth; (void)uiDstHeight; (void)iDstX; (void)iDstY;
      (void)cCol; (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " StretchblitColor not implemented");
  }

  /** Fills and stretches part of a bitmap with a color weighted by an alphabitmap. The destination rectangle will be clipped before the actual blit begins.
  The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The alphabitmap does not have to be locked/unlocked since it is accessed read only. Used for 2-color blitting.
  @param pkAlpha Pointer to the sourcebitmap.
  @param uiSrcWidth Width of region to copy from in pixels.
  @param uiSrcHeight Height of region to copy from in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param uiDstWidth Width of region to copy to in pixels.
  @param uiDstHeight Height of region to copy to in pixels.
  @param iDstX Top position to copy to.
  @param iDstY Left position to copy to.
  @param cCol Color to fill with.
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise <I>OR</I> operation.
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param col1 first color
  @param col2 second color
  @see Lock, Unlock, DrawBlit_Flags, StretchBlit, StretchBlitAlpha */
  virtual void StretchBlit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY,
    eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha = 255)
  {
      (void)pkAlpha; (void)uiSrcWidth; (void)uiSrcHeight; (void)uiSrcX; (void)uiSrcY;
      (void)uiDstWidth; (void)uiDstHeight; (void)iDstX; (void)iDstY;
      (void)cCol1; (void)cCol2; (void)ulFlags; (void)ubAlpha;
      eDBG_LOG( " StretchBlit2Color not implemented"); 
  }

  /** Blit operation with rotation and scaling. Similar to \Ref{StretchBlit} but rotation can be applied to the source bitmap while blitting.
  Scaling and the center of rotation are optional parameters. In contrast to the other blit functions RotBlit accepts non integer parameters
  for destination position and scaling to allow subpixel positioning (note that subpixel accuracy is achieved only when filtering is enabled
  e.g. \Ref{BLIT_BILINEAR}). The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The source bitmap does not have to be locked/unlocked since it is accessed read only.
  @param pkSource Pointer to the sourcebitmap.
  @param uiWidth Width of region to copy in pixels.
  @param uiHeight Height of region to copy in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param vDstX Top position to copy to (non integer value).
  @param vDstY Left position to copy to (non integer value).
  @param vAngle Rotation in rad (use \Ref{eC_Mul}(x, \Ref{eC_RAD}) to pass parameters in degree).
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise or.
  @param vStretchX Additional scaling in X direction (applied before rotation).
  @param vStretchY Additional scaling in Y direction (applied before rotation).
  @param vRotPosX Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param vRotPosY Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param cColorKey color that will be used as transparent. Enable colorkey using \Ref{DrawBlit_Flags} BLIT_COLORKEY.
  @see Lock, Unlock, DrawBlit_Flags, Blit, StretchBlit */
  virtual void RotBlit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags,
    eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1), eC_Value vRotPosX = eC_FromFloat(0.5f),
    eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0)
  {
      (void)pkSource; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)vDstX; (void)vDstY; (void)vAngle; (void)ulFlags;
      (void)vStretchX; (void)vStretchY; (void)vRotPosX; (void)vRotPosY; (void)ubAlpha; (void)cColorKey;
      eDBG_LOG( " Rotblit not implemented");
  }
  
  /** Blit operation with rotation and scaling using an alpha bitmap. Similar to \Ref{RotBlit} but an additional alphabitmap can be passed
  to specify which pixels of the source are opaque and which are transparent. The blit method is called from the destination bitmap, which
  must have been {@link Lock locked} before.
  The source and alphabitmaps does not have to be locked/unlocked since they are accessed read only.
  @param pkSource Pointer to the sourcebitmap.
  @param pkAlpha Pointer to the alphabitmap.
  @param uiWidth Width of region to copy in pixels.
  @param uiHeight Height of region to copy in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param vDstX Top position to copy to (non integer value).
  @param vDstY Left position to copy to (non integer value).
  @param vAngle Rotation in rad (use \Ref{eC_Mul}(x, \Ref{eC_RAD}) to pass parameters in degree).
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise or.
  @param vStretchX Additional scaling in X direction (applied before rotation).
  @param vStretchY Additional scaling in Y direction (applied before rotation).
  @param vRotPosX Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param vRotPosY Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, RotBlit, RotBlitColor */
  virtual void RotBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight,
    eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags,
    eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
    eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255)
  {
      (void)pkSource; (void)pkAlpha; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)vDstX; (void)vDstY; (void)vAngle; (void)ulFlags;
      (void)vStretchX; (void)vStretchY; (void)vRotPosX; (void)vRotPosY; (void)ubAlpha;
      eDBG_LOG( " RotblitAlpha not implemented"); 
  }
  
  /** Fills and rotates part of a bitmap with a color weighted by an alpha bitmap. Similar to \Ref{RotBlit} but the source is an alphabitmap which is rotated
  and stretched. The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The source and alphabitmaps does not have to be locked/unlocked since they are accessed read only.
  @param pkSource Pointer to the alpha bitmap.
  @param uiWidth Width of region to copy in pixels.
  @param uiHeight Height of region to copy in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param vDstX Top position to copy to (non integer value).
  @param vDstY Left position to copy to (non integer value).
  @param cCol Solid color (used where alpha is 100%).
  @param vAngle Rotation in rad (use \Ref{eC_Mul}(x, \Ref{eC_RAD}) to pass parameters in degree).
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise or.
  @param vStretchX Additional scaling in X direction (applied before rotation).
  @param vStretchY Additional scaling in Y direction (applied before rotation).
  @param vRotPosX Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param vRotPosY Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @see Lock, Unlock, DrawBlit_Flags, RotBlit, RotBlitAlpha */
  virtual void RotBlitColor(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol, eC_Value vAngle, eC_Flag ulFlags,
    eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
    eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255)
  {
    (void)pkSource; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
    (void)vDstX; (void)vDstY; (void)cCol; (void)vAngle; (void)ulFlags;
    (void)vStretchX; (void)vStretchY; (void)vRotPosX; (void)vRotPosY; (void)ubAlpha;
    eDBG_LOG( " RotblitColor not implemented");
  }
 /** Fills and rotates part of a bitmap with a color weighted by an alpha bitmap. Similar to \Ref{RotBlit} but the source is an alphabitmap which is rotated
  and stretched. The blit method is called from the destination bitmap, which must have been {@link Lock locked} before.
  The source and alphabitmaps does not have to be locked/unlocked since they are accessed read only.
  User for 2-color blitting
  @param pkSource Pointer to the alpha bitmap.
  @param uiWidth Width of region to copy in pixels.
  @param uiHeight Height of region to copy in pixels.
  @param uiSrcX Top position to copy from.
  @param uiSrcY Left position to copy from.
  @param vDstX Top position to copy to (non integer value).
  @param vDstY Left position to copy to (non integer value).
  @param cCol Solid color (used where alpha is 100%).
  @param vAngle Rotation in rad (use \Ref{eC_Mul}(x, \Ref{eC_RAD}) to pass parameters in degree).
  @param ulFlags Additional Flags from \Ref{DrawBlit_Flags}. To specify multiple flags combine them with bitwise or.
  @param vStretchX Additional scaling in X direction (applied before rotation).
  @param vStretchY Additional scaling in Y direction (applied before rotation).
  @param vRotPosX Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param vRotPosY Center of rotation in the range of 0.0 to 1.0 (similar to uv-coordinates).
  @param ubAlpha Alpha blending value for the whole area of the blit.
  @param col1 first color
  @param col2 second color
  @see Lock, Unlock, DrawBlit_Flags, RotBlit, RotBlitAlpha */
  virtual void RotBlit2Color(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
    eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Value vAngle, eC_Flag ulFlags,
    eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
    eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f), eC_UByte ubAlpha = 255)
  { 
      (void)pkSource; (void)uiWidth; (void)uiHeight; (void)uiSrcX; (void)uiSrcY;
      (void)vDstX; (void)vDstY; (void)cCol1; (void)cCol2; (void)vAngle; (void)ulFlags;
      (void)vStretchX; (void)vStretchY; (void)vRotPosX; (void)vRotPosY; (void)ubAlpha;
      eDBG_LOG( " RotBlit2Color not implemented"); 
  }

  
  /** Renders a line. Lines can be drawn using any width, antialiasing and subpixel accuracy. 
  Direct calls to DrawLine always work with direct pixel coordinates, to use transformation and view parameters \Ref{DrawPrimitive} has
  to be used. The renderingstyle for the line can be determined by line flags when calling DrawLine.
  @param kStart Line startpoint.
  @param kEnd Line endpoint.
  @param kColor Line color (abstract color, created by \Ref{ColorFromRGB}).
  @param ulFlags Additional flags to set attribut of a line (see \Ref{DrawLine_Flags}).
  @param vStartWidth Line width in pixels.
  @param vEndWidth Optional parameter - if omitted the line is rendered using a constant width (faster).
  @see ColorFromRGB, DrawLine_Flags */
  virtual void DrawLine(const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags,
    eC_Value vStartWidth, eC_Value vEndWidth = 0) = 0;
  
  inline void DrawLine(eC_Value vStartX, eC_Value vStartY, eC_Value vEndX, eC_Value vEndY, 
    eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth = 0)
  {
    DrawLine(eGML_Vector2(vStartX, vStartY), eGML_Vector2(vEndX, vEndY), kColor, ulFlags, vStartWidth, vEndWidth);
  }
  
  
  virtual void DrawPolygon(const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags) = 0;
  
  /** Renders a single polygon. The polygon can be convex, concave or self intersecting. Both clockwise and counterclockwise windings
  are supported. Note that direct calls to DrawPolygon will always work with direct 2D pixel coordinates. To use transformation,
  view parameters, {@link Culling_Modes culling} or advanced material properties ({@link eGML_Texture textures} etc.)
  \Ref{DrawPrimitive} has to be used.
  @param pkVertices Pointer to an array to vertices (stored as x,y pairs of eC_Value or \Ref{eGML_Vector2}).
  @param uiCount Number of vertices (>2).
  @param kColor Solid color to fill polygon with (abstract color, created by \Ref{eGML_ColorFromRGB}).
  @param ulFlags Additional flags to set attribut of a polygon (see \Ref{DrawPolygon_Flags}).
  @see DrawPolygon_Flags, ColorFromRGB */
  inline void DrawPolygon(const eGML_Vector2 *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags)
  {
    DrawPolygon( (eGML_Coord2D *)pkVertices, uiCount, kColor, ulFlags); 
  }
  
  /** Renders a geometry buffer. There is not explicit 2D- or 3D-translation or -rotation of the object. Generally: <BR>
  Both polygons and (poly)lines can be represented by \Ref{eGML_Primitive}. When rendering from a primitive buffer transformation and projection
  of the input data will be applied when necessary. View parameters (see \Ref{SetView}) and {@link eGML_RenderStates renderstates} are used.
  Primitives can be manually transformed before rendering them using methods of eGML_Primitive, or transformed
  directly during rendering by passing additional parameters to DrawPrimitive.
  @param pkPrimitive Pointer to the primitive object to be rendered.
  @param ulFlags Specifys how to interpret the vertex data stored in the primitive object (see \Ref{DrawPrimitive_Flags}).
  @see DrawPrimitive_Flags, eGML_RenderState, SetView */
  virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, eC_Flag ulFlags);
  
  /** Renders a geometry buffer with explicit 2D-transformation. Passing a {@link eGML_Matrix2 2D-matrix} and {@link eGML_Vector2 2D-vector}
  is equivalent to calling transform(matrix,vector) on the primitive before rendering it, but might result in a better performance if many primitives
  are transformed using the same parameters. Generally: <BR>
  Both polygons and (poly)lines can be represented by \Ref{eGML_Primitive}. When rendering from a primitive buffer transformation and projection
  of the input data will be applied when necessary. Note that {@link eGML_RenderStates Renderstates} are used, but \Ref{SetView} is <I>not</I>
  reasonable in this case because view parameters are only necessary for 3D-perspectives (see 3D-version of DrawPrimitive).
  @param pkPrimitive Pointer to the primitiveobject to be rendered.
  @param kRotation 2x2-matrix that specifies rotation and scaling.
  @param kTranslation 2D-vector that specfies translation.
  @param ulFlags Specifys how to interpret the vertex data stored in the primitive object (see \Ref{DrawPrimitive_Flags}).
  @see DrawPrimitive_Flags, eGML_RenderState */
  virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, const eGML_Matrix2 &kRotation, const eGML_Vector2 &kTranslation, eC_Flag ulFlags);
  
  /** Renders a geometry buffer with explicit 3D-transformation. Passing a {@link eGML_Matrix3 3D-matrix} and {@link eGML_Vector3 3D-vector}
  is equivalent to calling transform(matrix,vector) on the primitive before rendering it. But might result in a better performance if many primitives
  are transformed using the same parameters. Generally: <BR>
  Both polygons and (poly)lines can be represented by \Ref{eGML_Primitive}. When rendering from a primitive buffer transformation and projection
  of the input data will be applied when necessary. View parameters (see \Ref{SetView}) and {@link eGML_RenderStates renderstates} are used.
  @param pkPrimitive Pointer to the primitiveobject to be rendered.
  @param kRotation 3x3-matrix that specifies rotation and scaling.
  @param kTranslation 3D-vector that specfies translation.
  @param ulFlags Specifys how to interpret the vertex data stored in the primitive object (see \Ref{DrawPrimitive_Flags}).
  @see DrawPrimitive_Flags, eGML_RenderState, SetView */
  virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, const eGML_Matrix3 &kRotation, const eGML_Vector3 &kTranslation, eC_Flag ulFlags);
  
  
  virtual void DrawDot(eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0);
  /** Renders a simple dot with a determined size (radius) and color.
  @param kCenter Center of dot.
  @param vRadius Radius of the dot in pixels.
  @param kColor Color of dot (abstract color, created by \Ref{ColorFromRGB}).
  @param ulFlags Additional flags to set attribut of a dot (see \Ref{DrawDot_Flags}).
  @see DrawDot_Flags, ColorFromRGB */
  inline void DrawDot(eGML_Vector2 &kCenter, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0)
  {
    DrawDot(kCenter.x, kCenter.y, vRadius, kColor, uiFlags);
  }
  
  
  /** Fills the entire bitmap with a solid color. 
  Clear will ignore the current {@link SetCliprect cliprect}. Use \Ref{FillRect} to regard the cliprect.
  @param clColor Color to fill with (abstract color, created by \Ref{ColorFromRGB}).
  @see FillRect, SetCliprect, ColorFromRGB */
  virtual void Clear(eGML_Color clColor) = 0;
  
  /** Fills rectangular part of bitmap. Similar to \Ref{Clear} but with the additional option to partially fill the bitmap.
  @param uiWidth Width of rectangle in pixels.
  @param uiHeight Height of rectangle in pixels.
  @param iX1 Left edge of rectangle.
  @param iY1 Top edge of rectangle.
  @param clColor Color to fill with (abstract color, created by \Ref{ColorFromRGB}).
  @param ulFlags can be set to BLIT_NOCLIP to disable clipping. 
  @see Clear, SetCliprect, ColorFromRGB */
  virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_ColorARGB32 clColor, eC_Flag ulFlags = 0) = 0;

  /** Sets a new clipping border for this bitmap. All rendering operations that are applied to this bitmap will be clipped to this rectangle.
  By default the clipping rect is set to [0,0] .. [width,height]. You can not set a clipping rect that would allow access to pixels outside the bitmap.
  @param ulMinX Minimum allowed X-coordinate - <I>must</I> be value between 0...ulMaxX.
  @param ulMinY Minimum allowed Y-coordinate - <I>must</I> be value between 0...ulMaxY.
  @param ulMaxX Maximum allowed X-coordinate - <I>must</I> be value between ulMinX...({@link GetWidth width}-1).
  @param ulMaxY Maximum allowed Y-coordinate - <I>must</I> be value between ulMinY...({@link GetHeight height}-1).
  @see Clear, FillRect */
  virtual void SetCliprect(eC_UInt ulMinX, eC_UInt ulMinY, eC_UInt ulMaxX, eC_UInt ulMaxY);
  
  /** Sets entry in color LUT. This function will not have any effect on non indexed bitmaps.
  @param uiIndex Index of color to replace (0..255) 
  @param ubR Red component (0..255).
  @param ubG Green component (0..255).
  @param ubB Blue component (0..255).
  @see GetPalette, ColorFromRGB */
  void SetPalette(eC_UInt uiIndex, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB);
  
  /** Sets entry in color LUT. This function will not have any effect on non indexed bitmaps.
  @param uiIndex Index of color to replace (0..255).
  @param uiColor 32 bit RGB color value (0xffffff is white).
  @see GetPalette, ColorFromRGB */
  virtual void SetPalette(eC_UInt uiIndex, eC_UInt uiColor);
  
  /** Gets entry from color LUT. This function will return 0,0,0 (black) for all indices on non indexed bitmaps.
  @param uiIndex Index of color to get (0..255) 
  @param ubR Return value of red component.
  @param ubG Return value of green component.
  @param ubB Return value of blue component.
  @see SetPalette, ColorFromRGB */
  void GetPalette(eC_UInt uiIndex, eC_UByte &ubR, eC_UByte &ubG, eC_UByte &ubB);
  
  /** Gets entry from color LUT. This function will return 0x000000 (black) for all indices on non indexed bitmaps.
  @param uiIndex Index of color to get (0..255).
  @return 32 bit RGB color value.
  @see SetPalette, ColorFromRGB */
  virtual eC_UInt GetPalette(eC_UInt uiIndex);
  
  /** Converts a RGB Color to <B>eGML</B>s internal format. This conversion might involve a table lookup or some shifting.
  Even tough it is not considered an expensive operation you should try to minimise the number of conversions if several
  operations are performed using the same color.
  @param ubR Red component (0..255).
  @param ubG Green component (0..255).
  @param ubB Blue component (0..255).
  @return Bitmap specific color. All rendering functions use these colors as input instead of plain RGB values.
  @see SetPalette, GetPalette */
  virtual eGML_Color ColorFromRGB(eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;
  
  /** Converts an ARGB Color to <B>eGML</B>s internal format. This conversion might involve a table lookup or some shifting.
  Even tough it is not considered an expensive operation you should try to minimise the number of conversions if several
  operations are performed using the same color.
  @param ubA Alpha component (0..255).
  @param ubR Red component (0..255).
  @param ubG Green component (0..255).
  @param ubB Blue component (0..255).
  @return Bitmap specific color. All rendering functions use these colors as input instead of plain RGB values.
  @see SetPalette, GetPalette */
  virtual eGML_Color ColorFromARGB(eC_UByte ubA, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;

  /** Converts an ARGB Color to <B>eGML</B>s premultiplied internal format. This conversion might involve a table lookup or some shifting.
  Even tough it is not considered an expensive operation you should try to minimise the number of conversions if several
  operations are performed using the same color.
  @param ubA Alpha component (0..255).
  @param ubR Red component (0..255).
  @param ubG Green component (0..255).
  @param ubB Blue component (0..255).
  @return Bitmap specific color. All rendering functions use these colors as input instead of plain RGB values.
  @see SetPalette, GetPalette */
  virtual eGML_Color PremulColorFromARGB(eC_UByte ubA, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;

  /** Converts a RGB Color to <B>eGML</B>s internal format. This conversion might involve a table lookup or some shifting. Even tough it is not
  considered an expensive operation you should try to minimise the number of conversions if several operations are performed using the
  same color. In contrast to the call above, only one parameter containing the complete color information is used.
  @param uiColor 32 bit RGB color value (e. g. 0xffffff is white).
  @return Bitmap specific color. All rendering functions use these colors as input instead of plain RGB values.
  @see SetPalette, GetPalette */
  inline eGML_Color ColorFromRGB(eC_UInt uiColor) const
  {
     return ColorFromRGB( (eC_UByte)((uiColor >> 16) & 0xFF), (eC_UByte)((uiColor >> 8) & 0xFF), (eC_UByte)(uiColor & 0xFF));
  }
  
  /** Converts color from <B>eGML</B>s internal format to separate red, green and blue component.
  @param uiColor color in <B>eGML</B>s internal format.
  @param rubR Return value of red component.
  @param rubG Return value of green component.
  @param rubB Return value of blue component.
  @return Bitmap specific color. All rendering functions use these colors as input instead of plain RGB values.
  @see ColorFromRGB */
  virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;

  /** Converts color from <B>eGML</B>s internal format to a device specific color. 
  @param uiColor color in <B>eGML</B>s internal format.
  @param rubR Return value of red component.
  @param rubG Return value of green component.
  @param rubB Return value of blue component.
  @return Bitmap/device specific color. 
  @see ColorFromRGB */
  inline eC_UInt ColorToRGB(eGML_Color uiColor) const
  {
     eC_UByte r, g, b;
     eC_UInt  uiRetColor = 0;
     if (ColorToRGB(uiColor, r, g, b))
     {
        uiRetColor = (((eC_UInt)r)<<16) + (((eC_UInt)g)<<8) + b;
     }
     return uiRetColor;
  }

  /** Gets information, if Bitmap formats supports (and for RLE actually contains) alpha information
  @return true: an alphachannel is availble, false if not */
  virtual eC_Bool HasAlpha() const
  {
    return false;
  }

  /** Sets view parameters. The aspect ratio will affect {@link Blit blit} and {@link ProjectPoint transform} operations rendering to this bitmap.
  The other view parameters affect transformation/projection only. <BR>
  Near, Far and FOV have no effect when using 2D rendering, scale and offset on the other hand are applied when drawing 2D
  {@link eGML_Primitive primitives} as well.
  @param vAspect Pixel aspect ratio (width of pixel divided by height of pixel).
  @param kScale Optional scaling of X/Y components during projection / primitive rendering.
  @param kOffset Optional translation of X/Y components during projection / primitive rendering.
  @param vNear Optional 3D nearplane distance (has to be >0).
  @param vFar Optional 3D Farplane distance (has to be >vNear).
  @param vFOV Optional <I>F</I>ield <I>O</I>f <I>V</I>iew (before scaling & aspect correction) in degrees.
  @see eGML_Primitive, Blit, ProjectPoint, GetAspect, GetViewFOV, GetViewOffset, GetViewScaling, GetViewNear, GetViewFar, GetViewFrustum */
  virtual void SetView(eC_Value vAspect, 
    const eGML_Vector2 &kScale = eGML_Vector2(eC_FromFloat(1.0f), eC_FromFloat(1.0f)),
    const eGML_Vector2 &kOffset = eGML_Vector2(eC_FromFloat(0.0f), eC_FromFloat(0.0f)),
    eC_Value vNear = eC_FromFloat(1.0f), eC_Value vFar = eC_FromFloat(1000.0f),
    eC_Value vFOV = eC_FromFloat(90.0f));
  
    /** Sets view parameters in dependency of a specified rectangle. Scale and Offset will be modified to fit the view like showing
    through the specified rectangle.
    @param vMinX Left margin of the rectangle.
    @param vMinY Top margin of the rectangle.
    @param vMaxX Right margin of the rectangle.
    @param vMaxY Bottom margin of the rectangle.
  @see SetView, GetAspect, GetViewFOV, GetViewOffset, GetViewScaling, GetViewNear, GetViewFar, GetViewFrustum */
  void SetView(eC_Value vMinX, eC_Value vMinY, eC_Value vMaxX, eC_Value vMaxY);
  
  /** Finds the pixel position of a 3D point. The parameters for this 3D->2D mapping can be set by calling \Ref{SetView}.
  @param kPos3d Position in 3D Cameraspace.
  @param rkPos2d Returns the 2D position within this bitmap.
  @param vRHW Optional parameter for returning reziproc of z-value.
  @return <B>True</B> if returned pixel position is valid (points behind/on nearplane can not be projected and thus return <B>false</B>).
  @see SetView, BackprojectPoint */
  eC_Bool ProjectPoint(const eGML_Vector3 &kPos3d, eGML_Vector2 &rkPos2d, eC_Value *vRHW = 0);
  
  /** Finds the 3D position of a 2D pixel. A single pixel maps to a whole region in space, this function will return a linesegment describing this
  region as if the pixel were a point with no area. To find the {@link eGML_Primitive primitive} visible at this pixel test the returned
  linesegment for intersection with the scene and use the intersection closest to the viewer.
  @param kPos2d 2D position to backproject.
  @param rkStart3d Returns the corresponding 3D position on the nearplane.
  @param rkEnd3d Returns the corresponding 3D position on the farplane.
  @see SetView, ProjectPoint, eGML_Primitive */
  void BackprojectPoint(const eGML_Vector2 &kPos2d, eGML_Vector3 &rkStart3d, eGML_Vector3 &rkEnd3d) const;
  
  /** Debug Textoutput. Prints a string to the {@link eGML_Screen screen}. The font to be used is integrated with <B>eGML</B> (~760 byte)
  and supports the following characters:
  <PRE>!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ 0-9 a-z A-Z ‰ˆ¸ƒ÷‹</PRE>
  @param acText String to output (can contain '\n', '\r' and '\t' as special characters like used in standard-C strings).
  @param uiX Pixel position of left edge.
  @param uiY Pixel position of upper edge.
  @param uiColor RGB32 color (no need of \Ref{ColorFromRGB}) to print text in (unused spaces are left transparent).
  @see DebugTextMeasure */
  virtual void DebugText(const eC_Char *acText, eC_UInt uiX, eC_UInt uiY, eC_UInt uiColor);
  
  /** Debug Textoutput. Finds the bounding rectangle a given text would occupy when printed with \Ref{DebugText}.
  Nothing is rendered when calling DebugTextMeasure.
  @param acText String to output (can contain '\n', '\r' and '\t' as special characters like used in standard-C strings).
  @param uiX Pixel position of left edge (can pass 0 if text does not contain tabs).
  @param uiY Pixel position of upper edge (can pass 0 if text does not contain newlines).
  @param ruiWidth Variable that will recieve the boundingbox width.
  @param ruiHeight Variable that will recieve the boundingbox height.
  @see DebugText */
  virtual void DebugTextMeasure(const eC_Char *acText, eC_UInt uiX, eC_UInt uiY, eC_UInt &ruiWidth, eC_UInt &ruiHeight);
  
  /** Finds parent Screen. Every bitmap is linked to the screen it was created from.
  @return Pointer to \Ref{eGML_Screen} instance.
  @see eGML_Screen */
  virtual eGML_Screen *GetScreen() const { return m_pkScreen; }
  
  /** Provides left border of clip rectangle.
  @return Left border of clip rectangle.
  @see SetCliprect, GetClipMaxX, GetClipMinY, GetClipMaxY */
  inline eC_UInt GetClipMinX() const { return m_uiClipMinX; }
  
  /** Provides right border of clip rectangle.
  @return Right border of clip rectangle.
  @see SetCliprect, GetClipMinX, GetClipMinY, GetClipMaxY */
  inline eC_UInt GetClipMaxX() const { return m_uiClipMaxX; }
  
  /** Provides top border of clip rectangle.
  @return Top border of clip rectangle.
  @see SetCliprect, GetClipMinX, GetClipMaxX, GetClipMaxY */
  inline eC_UInt GetClipMinY() const { return m_uiClipMinY; }
  
  /** Provides bottom border of clip rectangle.
  @return Bottom border of clip rectangle.
  @see SetCliprect, GetClipMinX, GetClipMaxX, GetClipMinY */
  inline eC_UInt GetClipMaxY() const { return m_uiClipMaxY; }
  
  /** Provides bitmap width in pixels. The number of pixels has to be parameter of the {@link CreateBitmap constructor}.
  @return Bitmap width in pixels.
  @see GetHeight, CreateBitmap */
  inline eC_UInt GetWidth() const    { return m_uiWidth; }
  
  /** Provides bitmap height in pixels. The number of pixels has to be parameter of the {@link CreateBitmap constructor}.
  @return Bitmap height in pixels.
  @see GetWidth, CreateBitmap */
  inline eC_UInt GetHeight() const   { return m_uiHeight; }
  
  /** Provides bitmap type as specified in the {@link CreateBitmap constructor}.
  @return Bitmap type.
  @see CreateBitmap, BitmapType */
  inline eC_UInt GetType() const     { return m_uiType; }
  
  //
  inline eC_UByte GetBufferType() const { return m_ubBBT; }

  /** Provides the number of bytes in one scanline. The value depends on the width of the bitmap in pixels.
  @return Number of bytes in one scanline.
  @see GetWidth */
  inline eC_UInt GetPitch() const    { return m_uiPitch; }
  
  /** Makes a check up if the bitmap is of a basic (native) {@link BitmapType type}.
  @return <B>True</B> if bitmap is of a basic (native) type, else <B>false</B>.
  @see CreateBitmap, BitmapType */
  inline eC_Bool IsNative() const    { return (m_uiType >= eGML::BM_QUANTITY); }
  
  /** Provides aspect ratio of this bitmap. It is equivalent to the aspect ratio used in the \Ref{SetView} method; reasonable in 3D mode only.
  @return Aspect ratio of this bitmap.
  @see SetView, GetViewFOV, GetViewOffset, GetViewScaling, GetViewNear, GetViewFar */
  inline eC_Value GetAspect() const  { return m_vAspect; }
  
  /** Provides viewfrustum field of view angle. This value can be modified by a \Ref{SetView} call.
  @return Viewfrustum field of view angle.
  @see SetView, GetAspect, GetViewOffset, GetViewScaling, GetViewNear, GetViewFar, GetViewFrustum */
  inline eC_Value GetViewFOV() const { return m_vFOV; }
  
  /** Provides the viewfrustum center of projection. This value can be modified by a \Ref{SetView} call.
  @return Viewfrustum center of projection.
  @see SetView, GetAspect, GetViewFOV, GetViewScaling, GetViewNear, GetViewFar, GetViewFrustum */
  inline eGML_Vector2 GetViewOffset() const  { return eGML_Vector2(m_vOffsetX, m_vOffsetY); }
  
  /** Provides the viewfrustum scaling. This value can be modified by a \Ref{SetView} call.
  @return Viewfrustum scaling.
  @see SetView, GetAspect, GetViewFOV, GetViewOffset, GetViewNear, GetViewFar, GetViewFrustum */
  inline eGML_Vector2 GetViewScaling() const { return eGML_Vector2(m_vScaleX, m_vScaleY); }
  
  /** Provides the viewfrustum near plane. This value can be modified by a \Ref{SetView} call.
  @return Viewfrustum near plane.
  @see SetView, GetAspect, GetViewFOV, GetViewOffset, GetViewScaling, GetViewFar, GetViewFrustum */
  inline eC_Value GetViewNear() const { return m_vNear; }
  
  /** Provides the viewfrustum far plane. This value can be modified by a \Ref{SetView} call.
  @return Viewfrustum far plane.
  @see SetView, GetAspect, GetViewFOV, GetViewOffset, GetViewScaling, GetViewNear, GetViewFrustum */
  inline eC_Value GetViewFar() const  { return m_vFar; }
  
  /** Provides Z-distance where 3D is the same like 2D. <BR>
  (e. g. 3D-dot (10, 10, Get2dPlaneDistance()) will be rendered at the same position like a 2D-dot (10,10).
  @return Z-distance.
  @see GetViewFrustum */
  inline eC_Value Get2dPlaneDistance() const { return eC_Mul(m_kPerspective[0][0], m_vNear); }
  
  /** Provides the view frustum as a buffer of 8 vertices.
  @param akVertices Pointer to the buffer to get the 8 vertices (this is return value).
  @param vTolerance Scale value for frustum dimension.
  @see SetView, BackprojectPoint, Get2dPlaneDistance */
  void GetViewFrustum(eGML_Vector3 *akVertices, eC_Value vTolerance = eC_FromInt(1)) const;
  
  // !!! INTERNAL USE ONLY !!!
  // all following public members are for internal use only
  
  //
  const eC_MixedType & GetState(eC_UInt uiState) const;
  
  //
  inline const eGML_Matrix3 & GetPrescaleMatrix() const { return m_kPerspective; }
  
  //
  inline eGML_Vector2 GetViewCenter() const { return eGML_Vector2(m_vCenterX, m_vCenterY); }
  

  // NAVIGON add on
  virtual void RecalculatePrescaleMatrix(eC_UInt uiWidth, eC_UInt uiHeight, eC_Value vNear, eC_Value vFar, eC_Value vFOVx, eC_Value vFOVy);

  
  protected:
    enum
    {
      LINELIST =  0x1,
      TRILIST  =  0x2,
      STRIPE   =  0x3,
      FAN      =  0x4,
      LINE     = 0x10,
      POLY     = 0x20,
      CLOSED   = 0x40,
      POINTS   = 0x80,
        
      LINE_CLOSED   = LINE|CLOSED,
      LINE_LINELIST = LINE|LINELIST,
      LINE_TRILIST  = LINE|TRILIST,
      LINE_STRIPE   = LINE|STRIPE,
      LINE_FAN      = LINE|FAN,
        
      POLY_TRILIST  = POLY|TRILIST,
      POLY_STRIPE   = POLY|STRIPE,
      POLY_FAN      = POLY|FAN
    };
    
    enum PRIM_FGDC_FLAGS
    {
      PFI_USE_2D_COORD = 0x1,
        PFI_USE_2D_TRANS = 0x2,
        PFI_USE_3D_COORD = 0x4,
        PFI_USE_3D_TRANS = 0x8,
        PFI_ORTHO        = 0x100,
        PFI_TRANSFORM_3D = 0x200,
        PFI_TRANSFORM_2D = 0x400
    };
    
#ifdef eC_TARGET_GFX_FUJITSUGDC
    friend class eGML_FGDC_Bitmap;
    friend class eGML_FGDC_BitmapRGB555;
    friend class eGML_FGDC_BitmapRGB32;
    friend class eGML_FGDC_BitmapA8;
#endif
    
    friend class eGML_Screen;

	template <typename P1, typename O1, class A1, typename P2, typename O2, class A2> friend class eGML_RotBlit;
	template <typename P1, typename O1, class A1, typename P2, typename O2, class A2> friend class eGML_StretchBlit;
	
    
    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    // Abstract baseclass, no construction
    eGML_Bitmap(eC_UInt uiWidth = 0, eC_UInt uiHeight = 0, eC_UByte ubBBT = eGML::BBT_EGML);
    
    // Destructor. Memory associated with the bitmap is freed. Destruct a bitmap before the screen - it was constructed from - is destructed.
    virtual ~eGML_Bitmap();
    
    // used by blitters. clips blirect to source and dest bitmap
    bool ClipBlitrect(eGML_Bitmap *pkSource, eC_UInt &uiWidth, eC_UInt &uiHeight,
      eC_UInt &uiSrcX, eC_UInt &uiSrcY, eC_Int &iDstX, eC_Int &iDstY);
    
    // used by blitters. clips rect to dest bitmap
    bool ClipFillrect(eC_UInt &uiWidth, eC_UInt &uiHeight, eC_Int &iDstX, eC_Int &iDstY);
    
    // used for debugfont output
    virtual eC_Int DebugFontChar(eC_UByte ubChar, eC_UInt uiX, eC_UInt uiY, eGML_Color kColor);
    
    // used by draw primitive
    void DrawPrimitive(eGML_Primitive *pkPrim, const void *kRot, const void *kTrans, eC_Flag ulFlags, eC_Flag ulHow);
    
    //
    virtual void RenderPrimitivePoly(const void *pvValues, eC_UInt uiCount, eC_UInt uiPitch, eC_Flag ulFlags, eGML_Color kColor);
    
    
    
    // is int instead of enum BitmapType to keep
    eC_UInt m_uiType;
    
    // Width of Bitmap in pixels
    eC_UInt m_uiWidth;
    
    // Height of Bitmap in pixels
    eC_UInt m_uiHeight;
    
    // Screen this bitmap belongs to. (every bitmap is linked to a screen)
    eGML_Screen *  m_pkScreen;
    
    // internal pointer to bitmapdata
    void * m_pkData;
    
    // Number of bytes per scanline
    eC_UInt m_uiPitch;
    
    // State
    eC_Bool m_bLocked;
    
    //Bitmap Buffer Type
    eC_UByte m_ubBBT;
    
    // perspective scaling/shearing
    eGML_Matrix3 m_kPerspective;
    
    // Cliprect --------------------------------------
    
    //
    eC_UInt m_uiClipMinX;
    
    //
    eC_UInt m_uiClipMinY;
    
    //
    eC_UInt m_uiClipMaxX;
    
    //
    eC_UInt m_uiClipMaxY;
    
    // View parameters ---------------------------
    
    // width:height
    eC_Value m_vAspect;
    
    // view frustum
    eC_Value m_vNear;
    
    //
    eC_Value m_vFar;
    
    //
    eC_Value m_vFOV;
    
    //
    eC_Value m_vOffsetX;
    
    //
    eC_Value m_vOffsetY;
    
    //
    eC_Value m_vScaleX;
    
    //
    eC_Value m_vScaleY;
    
    //
    eC_Value m_vCenterX;
    
    //
    eC_Value m_vCenterY;
    
    
#ifdef eC_TARGET_GFX_FUJITSUGDC
    eC_UByte  m_ubBytesPerPixel;
    
    eC_UByte m_ubGDCPixelFormat;
    
    // Real width of Bitmap in pixels
    eC_UInt m_uiRealWidth; 
    
    //
    void *m_pFGDCMem;
    
    // Projection matrix
    float fMatP[16];
#endif
    
};

//---------------------------------------------------------------
#endif
