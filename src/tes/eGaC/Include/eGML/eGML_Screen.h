/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Screen.h
* %version:        134.0 %
* 
* author:          hh04031
* creation date:   Tue Aug 09 13:49:03 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_SCREEN__H_
#define EGML_SCREEN__H_

//---------------------------------------------------------------
#include "eGML_Bitmap.h"
#include "eGML_Primitive.h"
#include "eGML_RenderState.h"
//#include "eGML_Rendering.h"
#include "eGML_Texture.h"
#include "eGML_Pattern.h"

//---------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
#define eGML_MAXBUFFER 8

//---------------------------------------------------------------
class eGML_Pattern;

//---------------------------------------------------------------
eDBG_PERF_DEF(FlipScreen);

//---------------------------------------------------------------
/**
@memo
Framebuffer / Visible Bitmap. 
@doc
A screen represents the visible framebuffer and provides access to the connected rendering device. Each screen can be used like
a {@link eGML_Bitmap bitmap} although internally a single screen contains several bitmaps. The use of multiple bitmaps enables the screen
to do buffering (usually double buffering, but triple buffering and other techniques are possible as well). <BR>
When creating a screen (using \Ref{CreateScreen} by \Ref{eGML} object) the number of buffers can be specified. In case buffering is disabled
by using single buffer screen (creation with buffer parameter = 1) the screen will act exactly like a bitmap. Because clearing and refilling the
screen while it is displayed leads to flickering and other visual artifacts, it is strongly recommended to use 2 buffers (bitmaps) per screen. <BR>
When using a buffered screen there is always one bitmap (called the <I>active</I> bitmap) which is the current render target. All bitmap 
functions of \Ref{eGML_Screen} (like \Ref{Clear} or \Ref{Blit}) work on the active bitmap. While rendering to the active bitmap a different one
(the <I>visible</I> bitmap) is shown. After rendering has finished you can make the active bitmap visible by calling \Ref{Flip}. <BR>
For getting more information about \Ref{eGML_Screen} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Screen_1.cpp]{examples/example_Screen_1.cpp.html} <BR>
@see eGML, eGML_Bitmap, eGML_RenderState
@see Tutorial_eGML
*/
class eGML_Screen : public eGML_Bitmap
{
  public:
    
       
    /* !!! INTERNAL USE ONLY !!!
    current renderstate of screens device. When rendering primitives many additional parameters are available as renderstates.
    Renderstates are global and apply to all bitmaps associated to (created by) the current screen. */
    eGML_RenderState RenderState;
    
    // !!! INHERITED !!! documentation in base class
    virtual void *Lock();
    
    // !!! INHERITED !!! documentation in base class
    virtual void Unlock(eC_UInt uiX1 = 0, eC_UInt uiY1 = 0, eC_UInt uiX2 = 0, eC_UInt uiY2 = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void SetPalette(eC_UInt uiIndex, eC_UInt uiColor);
    
    // !!! INHERITED !!! documentation in base class
    virtual eC_UInt GetPalette(eC_UInt uiIndex);
    
    // !!! INHERITED !!! documentation in base class
    inline void SetPalette(eC_UInt uiIndex, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB)
    { eGML_Bitmap::SetPalette(uiIndex, ubR, ubG, ubB); }
    
    // !!! INHERITED !!! documentation in base class
    inline void GetPalette(eC_UInt uiIndex, eC_UByte &ubR, eC_UByte &ubG, eC_UByte &ubB)
    { eGML_Bitmap::GetPalette(uiIndex, ubR, ubG, ubB); }
    
    // !!! INHERITED !!! documentation in base class
    virtual void Blit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                            eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void BlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight,
                                        eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255);
    
    // !!! INHERITED !!! documentation in base class
    virtual void BlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                       eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol, eC_Flag ulFlags, eC_UByte ubAlpha = 255);

    virtual void Blit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                       eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha = 255);
    // !!! INHERITED !!! documentation in base class
    virtual void StretchBlit(eGML_Bitmap *pkSource, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                          eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY,
                                          eC_Flag ulFlags, eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void StretchBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight,
                                                     eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight,
                                                     eC_Int iDstX, eC_Int iDstY, eC_Flag ulFlags, eC_UByte ubAlpha = 255);
    
    // !!! INHERITED !!! documentation in base class
    virtual void StretchBlitColor(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                                     eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol,
                                                     eC_Flag ulFlags, eC_UByte ubAlpha = 255);

	virtual void StretchBlit2Color(eGML_Bitmap *pkAlpha, eC_UInt uiSrcWidth, eC_UInt uiSrcHeight, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_UInt uiDstWidth, eC_UInt uiDstHeight, eC_Int iDstX, eC_Int iDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Flag ulFlags, eC_UByte ubAlpha = 255);
    // !!! INHERITED !!! documentation in base class
    virtual void RotBlit(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                   eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags,
                                   eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
                                   eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f),
                                   eC_UByte ubAlpha = 255, eGML_Color cColorKey = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void RotBlitAlpha(eGML_Bitmap *pkSource, eGML_Bitmap *pkAlpha, eC_UInt uiWidth, eC_UInt uiHeight,
                                              eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Value vDstX, eC_Value vDstY, eC_Value vAngle, eC_Flag ulFlags,
                                              eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
                                              eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f),
                                              eC_UByte ubAlpha = 255);
    
    // !!! INHERITED !!! documentation in base class
    virtual void RotBlitColor(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                              eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol, eC_Value vAngle, eC_Flag ulFlags,
                                              eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
                                              eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f),
                                              eC_UByte ubAlpha = 255);
    
    virtual void RotBlit2Color(eGML_Bitmap *pkSource, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiSrcX, eC_UInt uiSrcY,
                                          eC_Value vDstX, eC_Value vDstY, eGML_ColorARGB32 cCol1, eGML_ColorARGB32 cCol2, eC_Value vAngle, eC_Flag ulFlags,
                                          eC_Value vStretchX = eC_FromInt(1), eC_Value vStretchY = eC_FromInt(1),
                                          eC_Value vRotPosX = eC_FromFloat(0.5f), eC_Value vRotPosY = eC_FromFloat(0.5f),
                                          eC_UByte ubAlpha = 255);
    // !!! INHERITED !!! documentation in base class
    virtual void Clear(eGML_Color clColor);
    
    // !!! INHERITED !!! documentation in base class    
	virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_ColorARGB32 clColor, eC_Flag ulFlags = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawLine(const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags,
                                         eC_Value vStartWidth, eC_Value vEndWidth = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawPolygon(const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, eC_Flag ulFlags);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, const eGML_Matrix2 &kRotation, const eGML_Vector2 &kTranslation,
                                                 eC_Flag ulFlags);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawPrimitive(eGML_Primitive *pkPrimitive, const eGML_Matrix3 &kRotation, const eGML_Vector3 &kTranslation,
                                                 eC_Flag ulFlags);
    
    // !!! INHERITED !!! documentation in base class
    virtual void DrawDot(eC_Value vCenterX, eC_Value vCenterY, eC_Value vRadius, eGML_Color kColor, eC_Flag uiFlags = 0);
    
    // !!! INHERITED !!! documentation in base class
    virtual void SetCliprect(eC_UInt ulMinX, eC_UInt ulMinY, eC_UInt ulMaxX, eC_UInt ulMaxY);
    
    // !!! INHERITED !!! documentation in base class
    virtual eGML_Color ColorFromRGB(eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;

    // !!! INHERITED !!! documentation in base class
    virtual eGML_Color ColorFromARGB(eC_UByte ubA, eC_UByte ubR, eC_UByte ubG, eC_UByte ubB) const;

    // TODO: Why do we have to add this method here? it's defined in baseclass already?
    inline eGML_Color ColorFromRGB(eC_UInt uiColor) const
    {
       return ColorFromRGB( (eC_UByte)((uiColor >> 16) & 0xFF), (eC_UByte)((uiColor >> 8) & 0xFF), (eC_UByte)(uiColor & 0xFF));
    }
    
    virtual eC_Bool ColorToRGB(eGML_Color uiColor, eC_UByte & rubR, eC_UByte & rubG, eC_UByte & rubB) const;

    // !!! INHERITED !!! documentation in base class
    virtual void DebugText(const eC_Char *acText,  eC_UInt uiX, eC_UInt uiY, eC_UInt uiColor); 
    
    // !!! INHERITED !!! documentation in base class
    virtual eGML_Screen *GetScreen()  const  { return (eGML_Screen *) this; }
    
    // !!! INHERITED !!! documentation in base class
    virtual void SetView(eC_Value vAspect, const eGML_Vector2 &kScale = eGML_Vector2(eC_FromFloat(1.0f), eC_FromFloat(1.0f)),
                                      const eGML_Vector2 &kCenter = eGML_Vector2(eC_FromFloat(0.0f), eC_FromFloat(0.0f)),
                                      eC_Value vNear = eC_FromFloat(1.0f), eC_Value vFar = eC_FromFloat(1000.0f),
                                      eC_Value vFOV = eC_FromFloat(90.0f));
    
    /** Creates a new image container. Additional bitmaps created by CreateBitmap are not directly visible. To display them their content
    has to be transferred to the screen using a \Ref{Blit} operation.
    @param eBitmapType {@link BitmapType Format} specifies the internal organisation of displaymemory.
    @param uiWidth Width of bitmap in pixels.
    @param uiHeight Height of bitmap in pixels.
    @param ubBitmapBufferType optional bitmap buffertype.
    @param pBitmapBuffer optional pointer of extern allocated memory. If this pointer is not zero, the bitmap does not allocate own memory.
    @return Pointer to the new bitmap or NULL if creation failed.
    @see eGML_Bitmap, BitmapType, Blit */
    virtual eGML_Bitmap* CreateBitmap(eC_UInt eBitmapType, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBitmapBufferType=eGML::BBT_EGML, void *pBitmapBuffer=0);
    
    /** Creates a new {@link eGML_Texture texture} by calling the private constructor of the texture object. <BR>
    The main task of the constructor is to allocate memory for the surface (displayed by a {@link eGML_Bitmap bitmap}) containing
    the texture. The height and the width parameters have to be a power of 2 otherwise the creation will fail.
    Only if everything was working fine the texture is valid.
    @param eBitmapType {@link BitmapType Bitmap-type} for creating bitmaps for the texture surfaces.
    @param uiWidth Width of the texture surfaces.
    @param uiHeight Height of the texture surfaces.
    @param uiMipMaps Reserved for next releases. Must be zero.
    @see eGML_Texture, eGML_Bitmap, BitmapType */
    virtual eGML_Texture* CreateTexture(eC_UInt eBitmapType, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiMipMaps = 0);

    /** Creates a new {@link eGML_Pattern pattern} by calling the private constructor of the pattern object. <BR>
    The constructor initializes the array containing all colors listed in the parameter puiRGB32Colors.
    @param pColors Array containing all colors for the pattern object.
    @param uiNbOfColors Quantity of colors stored in the pattern object. This value <I>must</I> be smaller than 256.
    @param pkBitmap Bitmap the pattern object belongs to.
    @see eGML_Pattern, eGML_Bitmap */
    virtual eGML_Pattern* CreatePattern(eGML_ColorARGB32 *pColors, eC_UInt uiNbOfColors, eGML_Bitmap *pkBitmap = 0);
    
    /** Creates a new geometry container. A {@link eGML_Primitive primitive} buffer is a set of vertices and their connectivity. The basic layout
    and initial size must be specified upon construction. The {@link Buffer_Types layout} specifies which data are stored per vertex.
    To access these data it is organised into several buffers which can be locked/modified using the eGML_Primitive interface. <BR>
    The {@link Vertex_Layout_Fragments fragment} flags can be combined to form a primitive layout. There will be no single buffer for each of them,
    but instead several fragments will be combined into one buffer.
    @param uiLayout Combination of fragment flags that indicate vertexdata layout.
    @param usSize Initial size (number of vertices) of primitive.
    @param uiFlags Contains information about fragment buffer layout (see \Ref{Vertex_Layout_Fragments}).
    @return Pointer to the primitive object or NULL if creation failed.
    @see eGML_Primitive, Vertex_Layout_Fragments, Buffer_Types */
		virtual eGML_Primitive* CreatePrimitive(eC_Flag uiLayout, eC_UShort usSize, eC_Flag uiFlags = 0);

    /** Provides the current rendertarget.
    The active bitmap is the one to which all rendering calls are passed on when calling rendering methods from screen.
    @return Pointer to current rendertarget bitmap.
    @see eGML_Bitmap */
    virtual eGML_Bitmap* GetActiveBitmap() const;
    
    /** Provides the currently displayed bitmap.
    @return Pointer to currently displayed bitmap.
    @see eGML_Bitmap */
    virtual eGML_Bitmap* GetDisplayBitmap() const;
    
    /** Provides a specific buffer of this screen. Since each screen contains multiple buffers you can use this function to request the pointer to a
    specific buffer. Usually (when using \Ref{Flip}) it is not necessary to get buffers directly.
    @param uiBuffer Index of buffer (first buffer has index 0).
    @return Pointer to specific bitmap.
    @see ShowBuffer, SetActiveBuffer, Flip */
    virtual eGML_Bitmap* GetBuffer(eC_UInt uiBuffer) const;

    /** Do ARGB32 post-divide. This is a utility function that can be called on platforms
     that have a hardware layer manager / compositor which can only handle non-premultiplied
     layers. In this case the application can render to an offscreen (pre-multiplied)
     framebuffer and use this function to alpha-post-divide the offscreen bitmap and store
     the result in the active buffer. The position parameters can be used to limit the
     post divide operation to a rectangular sub-region of the bitmap.
     @param pkBitmapPremul Pointer to premultiplied ARGB32 bitmap
     @param uiSrcX Left source position
     @param uiSrcY Top source position
     @param iDstX Left destination position
     @param iDstY Right destination position
     @param uiW Width
     @param uiH Height
     @see ShowBuffer, SetActiveBuffer, Flip, Unlock
     */
    virtual void PostDivideARGB32(eGML_Bitmap *pkBitmapPremul, eC_UInt uiSrcX, eC_UInt uiSrcY, eC_Int iDstX, eC_Int iDstY, eC_UInt uiW, eC_UInt uiH);
    
    /** Directly forces a buffer to be visible. In case you do not want to use \Ref{Flip} for buffer management, you can manually set the
    visible buffer using this function.
    @param uiBuffer Index of buffer (first buffer has index 0).
    @see GetBuffer, SetActiveBuffer, Flip */
    virtual void ShowBuffer(eC_UInt uiBuffer);
    
    /** Directly forces a buffer to be active. In case you do not want to use \Ref{Flip} for buffer management, you can manually set the
    active buffer using this function. All rendering methods called on screen will be executed on the active buffer.
    @param uiBuffer Index of buffer (first buffer has index 0).
    @see GetBuffer, ShowBuffer, Flip */
    virtual void SetActiveBuffer(eC_UInt uiBuffer);
    
    /** Shows active buffer. Flip is the standard way to make sure the rendered frame becomes visible. It will exchange active and visible
    buffers (or cycle them if more than 2 buffers are available). You can mix calls to Flip with manual buffermanagement via \Ref{ShowBuffer}
    and \Ref{SetActiveBuffer}, but usually simple double or triple buffering using Flip is recommended
    @param ulFlags Reserved.
    @see GetBuffer, ShowBuffer, SetActiveBuffer, GetActiveBitmap */
    virtual void Flip(eC_Flag ulFlags = 0);
    
    /* !!! INTERNAL USE ONLY !!!
    Checks construction success.
    @return <B>True</B> if the construction succeed without any error, else <B>false</B> will be returned
    @see CreateScreen */
    inline eC_Bool IsValid() const { return m_bIsValid; }

	/** Return eGML instance. A pointer to the \Ref{eGML} baseclass that created the screen is returned.
	@return pointer to eGML baseclass
	*/
	inline eGML *GetOwner() const { return m_pkInstance; }
    
	  
   // NAVIGON add on
   virtual void RecalculatePrescaleMatrix(eC_UInt uiWidth, eC_UInt uiHeight, eC_Value vNear, eC_Value vFar, eC_Value vFOVx, eC_Value vFOVy);


  protected:
    
    friend class eGML;
    
    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    eGML_Screen(eGML *pkInstance, eC_UInt eType, eC_UInt eFormat, eC_UInt uiWidth, eC_UInt uiHeight);

    // private constructor. construction only via <B>eGML</B> baseclass
    eGML_Screen(eGML *pkInstance, eC_UInt eType, eC_UInt eFormat, eC_UInt uiViewDepth, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers);

    bool Init(eC_UInt uiBuffers, void **ppBitmapBuffer, eC_UByte ubBitmapBufferType);

    // destructor. Close all bitmaps created by this screen before closing the screen itself.
    virtual ~eGML_Screen();
    
    // assign active bitmap properties
    virtual void CopyBitmapProperties(eGML_Bitmap *pkSource);

    // alloc/calculate alpha post divide table (ARGB32 screens only)
    void InitPostDivide64kTable(void);
    
    // alloc/calculate alpha post divide table (ARGB32 screens only)
    void FreePostDivide64kTable(void);

    // 8bit x/y LUT
    eC_UByte *m_ubaaDiv64kTable;

    // hardware type (layer/monitor) of this virtual screen
    eC_Enum m_eScreenType;
    
    // physical bitmaps storing screendata
    eGML_Bitmap * m_apBuffer[eGML_MAXBUFFER];
    
    // number of buffers used on this screen
    eC_UInt m_uiBufferCount;
    
    // index of currently displayed buffer
    eC_UInt m_uiDisplayBuffer;
    
    // index of active buffer (render target)
    eC_UInt m_uiActiveBuffer;
    
    // internal flag to signal successful device allocation
    eC_Bool m_bIsValid;	

    // pointer to instance of egml that owns this screen
	  eGML *m_pkInstance;

};

//---------------------------------------------------------------
#endif
