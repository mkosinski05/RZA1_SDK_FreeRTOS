/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Texture.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:20:28 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_TEXTURE__H_
#define EGML_TEXTURE__H_
//----------------------------------------------------------------------------
#include "eGML_Bitmap.h"

//----------------------------------------------------------------------------
// maximum number of buffers (2==double buffering)
#define eGML_MAXMIPMAPS 12

//----------------------------------------------------------------------------
/**
@memo
Class for handling with textures.
@doc
This class provides effective handling of textures in graphic objects. The best way is to apply textures is, to use {@link eGML_Primitive primitives}
as graphical objects. To be flexible in working out visible effects there is a seperation between texture parameters and display informations.
Display informations can be set/modified by using \Ref{eGML_RenderState} methods and parameters.
So in any case there is cooperation and interaction between eGML_RenderState and eGML_Texture.
For creating texture objects in <B>eGML</B> you only have to call <TT><B>CreateTexture</B></TT> method of \Ref{eGML_Screen}. <BR>
In contrast to fill surfaces with textures, you are able to draw multi color lines by using {@link eGML_Pattern pattern} objects. <BR>
For getting more information about \Ref{eGML_Texture} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Texture_1.cpp]{examples/example_Texture_1.cpp.html} <BR>
@see eGML_RenderState, eGML_Pattern, eGML_Screen, eGML_Primitive
@see Tutorial_eGML
*/
#ifdef eGML_POLYGONTEXTURE_DISABLE

class eGML_Texture
{
};


#else


class eGML_Texture
{
  public:
    
    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW

    
    /** Locks texture object for modifying the bitmap. <BR>
    Be sure that the texture object is <I>not</I> locked yet, otherwise this operation is not progressed.
    @param uiMipMap Reserved for next releases. Must be zero.
    @param bExclusive Reserved for next releases. Must be <B>true</B>.
    @return Pointer to tetxure bitmap.
    @see Unlock, GetBitmap, eGML_Bitmap */
    virtual eGML_Bitmap *Lock(eC_UInt uiMipMap = 0, eC_Bool bExclusive = true);
    
    /** Unlocks the texture object again after it was {@link Lock locked}. <BR>
    You can simply call Unlock without any parameters for indicating that the hole bitmap has been changed or
    you can specify a rectangle that describes a region of the bitmap that has been modified during the Lock.
    Be sure that the texture object has been locked before using Unlock.
    @param uiX1 Left border of modified region.
    @param uiY1 Top border of modified region.
    @param uiX2 Right border of modified region.
    @param uiY2 Bottom border of modified region.
    @see Lock, eGML_Bitmap */
    virtual void Unlock(eC_UInt uiX1 = 0, eC_UInt uiY1 = 0, eC_UInt uiX2 = 0, eC_UInt uiY2 = 0);
    
    /** Provides pointer to a bitmap.
    @param uiMipMap Reserved for next releases. Must be zero.
    @return Pointer to texture bitmap.
    @see Lock, eGML_Bitmap */
    virtual eGML_Bitmap *GetBitmap(eC_UInt uiMipMap = 0);
    
    /* !!! NOT FINISHED YET !!! For Release 1.3
    Transmits modifications of one bitmap to another.
    @param uiMipMap Number of the bitmap, that has to be modified with datas of the current {@link Lock locked} bitmap.
    @see Lock, Unlock, eGML_Bitmap */
    virtual void CreateMipMaps(eC_UInt uiMipMap = 1);
    
    /** Set a new clipping border for the texture bitmap.
    All rendering operations that are applied to this texture will be clipped to this rectangle.
    By default the clipping rect is set to <TT>[0,0] .. [width,height]</TT>. A clipping rect will not allow access to pixels outside the bitmap.
    @param ulMinX Minimum allowed X-coordinate (<I>must</I> be value between 0...ulMaxX).
    @param ulMinY Minimum allowed Y-coordinate (<I>must</I> be value between 0...ulMaxY).
    @param ulMaxX Maximum allowed X-coordinate (<I>must</I> be value between ulMinX...{@link GetWidth width}).
    @param ulMaxY Maximum allowed Y-coordinate (<I>must</I> be value between ulMinY...{@link GetHeight height}).
    @see GetClipMinX, GetClipMinY, GetClipMaxX, GetClipMaxY, eGML_Bitmap */
    void SetCliprect(eC_UInt ulMinX, eC_UInt ulMinY, eC_UInt ulMaxX, eC_UInt ulMaxY);
    
    /** Sets aspect ratio of texture object. It is defined: <BR>
    <PRE>ratio = width / height</PRE>.
    @param vAspect New aspect ratio.
    @see GetAspect */
    void SetAspect(eC_Value vAspect);
    
    /** Provides left border of clip rectangle.
    @return X-position of left border.
    @see SetCliprect, GetClipMaxX, GetClipMinY, GetClipMaxY */
    inline eC_UInt GetClipMinX() const { return m_apBitmap[0]->GetClipMinX(); }
    
    /** Provides right border of clip rectangle.
    @return X-position of right border.
    @see SetCliprect, GetClipMinX, GetClipMinY, GetClipMaxY */
    inline eC_UInt GetClipMaxX() const { return m_apBitmap[0]->GetClipMaxX(); }
    
    /** Provides top border of clip rectangle.
    @return Y-position of top border.
    @see SetCliprect, GetClipMinX, GetClipMaxX, GetClipMaxY */
    inline eC_UInt GetClipMinY() const { return m_apBitmap[0]->GetClipMinY(); }
    
    /** Provides bottom border of clip rectangle.
    @return Y-position of bottom border.
    @see SetCliprect, GetClipMinX, GetClipMaxX, GetClipMinY */
    inline eC_UInt GetClipMaxY() const { return m_apBitmap[0]->GetClipMaxY(); }
    
    /** Provides texture width in pixels.
    The number of pixels has to be parameter of <TT><B>CreateTexture</B></TT> call by an \Ref{eGML_Screen} object.
    @return Texture width in pixels.
    @see GetHeight, GetWidthMask, eGML_Screen */
    inline eC_UInt GetWidth() const    { return m_apBitmap[0]->GetWidth(); }
    
    /** Provides texture height in pixels.
    The number of pixels has to be parameter of <TT><B>CreateTexture</B></TT> call by an \Ref{eGML_Screen} object.
    @return Texture height in pixels.
    @see GetWidth, GetHeightMask, eGML_Screen */
    inline eC_UInt GetHeight() const   { return m_apBitmap[0]->GetHeight(); }
    
    /** Provides texture type as specified in <TT><B>CreateTexture</B></TT> call of \Ref{eGML_Screen}.
    @return Texture type.
    @see eGML_Screen, BitmapType */
    inline eC_UInt GetType() const     { return m_apBitmap[0]->GetType(); }
    
    /** Makes a check up if the texture is of a basic (native) {@link BitmapType type}.
    @return If texture is of a basic (native) type <B>true</B> will be returned.
    @see BitmapType */
    inline eC_Bool IsNative() const    { return m_apBitmap[0]->IsNative(); }
    
    /** Provides aspect ratio of this texture.
    @return Aspect ratio of this texture.
    @see SetAspect */
    inline eC_Value GetAspect() const  { return m_apBitmap[0]->GetAspect(); }
    
    /* !!! NOT FINISHED YET !!! For Release 1.3
    Provides number of mipmaps. The number of mipmaps has to be parameter of <TT><B>CreateTexture</B></TT> call of \Ref{eGML_Screen}.
    @return Number of mipmaps.
    @see CreateMipMaps, eGML_Screen */
    inline eC_UInt GetMipMapCount() const { return m_uiLevels-1; }
    
    /** Makes a checkup if the texture is {@link Lock locked}.
    @return If texture is locked <B>true</B> will be returned.
    @see Lock, IsValid */
    inline eC_Bool IsLocked() const { return m_bLocked; }
    
    /** Makes a checkup if the texture is valid. The texture is valid only if everything was working fine when creating the object
    (refer to <TT><B>CreateTexture</B></TT> of \Ref{eGML_Screen} class).
    @return If texture is valid <B>true</B> will be returned.
    @see IsLocked */
    inline eC_Bool IsValid() const  { return m_bValid; }
    
    /* !!! NOT FINISHED YET !!! For Release 1.3
    Provides the exponent of 2, of the texture width. <BR>
    e. g.: width = 256 = 2^8 -> <TT>GetPitchShift() = 8</TT>.
    @see GetWidth */
    inline eC_UInt GetPitchShift() const { return m_uiPitchShift; }

    /* !!! NOT FINISHED YET !!! For Release 1.3
    Provides the exponent of 2, of the texture height. <BR>
    e. g.: height = 256 = 2^8 -> <TT>GetHeightShift() = 8</TT>.
    @see GetHeight */
    inline eC_UInt GetHeightShift() const { return m_uiHeightShift; }
    
    /* !!! NOT FINISHED YET !!! For Release 1.3
    Provides bit mask for the texture width. This value depends on the number of pixels in X-direction.
    @return Bit mask of texture width. Pre-condition for reasonable values: the texture width must be multiple of 2, then <TT>mask = width - 1</TT>.
    @see GetHeightMask, GetWidth */
    inline eC_UInt GetWidthMask() const  { return m_uiXMask; }
    
    /* !!! NOT FINISHED YET !!! For Release 1.3
    Provides bit mask for the texture height. This value depends on the number of pixels in Y-direction.
    @return Bit mask of texture height. Pre-condition for reasonable values: the texture height must be multiple of 2, then <TT>mask = height - 1</TT>
    @see GetWidthMask, GetHeight */
    inline eC_UInt GetHeightMask() const { return m_uiYMask; }
    
    /** Deletes texture object by calling the private destructor. Releases the allocated memory for the surface bitmaps again.
    Be sure that the texture is {@link Unlock unlocked} before destructor call.
    @see eGML_Bitmap, Unlock */
    void Release() { delete this; }
    
  private:
    
    friend class eGML_Screen;
    
    // !!! INTERNAL USE ONLY !!!
    // all following private members are for internal use only
    
    // private constructor.
    eGML_Texture(eGML_Screen *pkScreen, eC_UInt eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiMipMaps = 0);
    
    // private destructor.
    virtual ~eGML_Texture();
    
    // latest lock status
    eC_Bool m_bLocked;
    
    // latest lock level
    eC_UInt m_uiLockedLevel;
    
    // indicates if there is an exclusive lock
    eC_Bool m_bExclusiveLock;
    
    // indicates if texture is valid
    eC_Bool m_bValid;
    
    // toplevel shift value 
    eC_UInt m_uiPitchShift;
    eC_UInt m_uiHeightShift;
    
    // toplevel bitmask for x direction
    eC_UInt m_uiXMask;
    
    // toplevel bitmask for x direction
    eC_UInt m_uiYMask;
    
    // parent screen
    eGML_Screen * m_pkScreen;
    
    // texture surfaces: number of bitmaps for this texture surfaces
    eGML_Bitmap * m_apBitmap[eGML_MAXMIPMAPS];
    
    // texture surfaces: level of texture
    eC_UInt m_uiLevels;
};


#endif

//----------------------------------------------------------------------------
#endif
