/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GFX_ENV_WIN__H__
#define __GFX_ENV_WIN__H__

#include "GUIRect.h"
#include <windows.h>

/// Implements windows GDI specific features.

/**
    This class implements the Windows GDI specific features, which may be
    used in different graphic wrappers within the Windows environment.
*/

class CGfxEnvWin
{
protected:
    /** Get the visual screen from window and create the memory DC.
        @param uiWidth The width of the screen to be created.
        @param uiHeight The height of the screen to be created.
        @param ubBitPerPixel Color depth in Bit per Pixel. 0 will use the platform's default.
     */
    void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight, const eC_UByte &ubBitPerPixel=0);

    /** Internal method used in the CreateMemoryDC method.
        @param nBPP Bit per pixel
        @param dwCompression Compression
        @param uiWidth Width
        @param uiHeight Heihht
        @return Bitmap info header
     */
    LPBITMAPINFO CreateBitmapInfoHeader(USHORT  nBPP, DWORD dwCompression, eC_UInt uiWidth, eC_UInt uiHeight);

    /** Refresh the screen within the given invalidate area.
        @param kInvalidArea This area will be refreshed.
        @return True if OK
     */
    eC_Bool Refresh(const CGUIRect &kInvalidArea);
    /** Set the clip rectangle.
        @param NewClipRect The new clipping rectangle.
     */
    void SetCliprect(const CGUIRect &NewClipRect);
    /** Set the drawing environment including the pen, brush and so on,
        and selecting them in the DC.
        @param vLineWidth The line width used in the drawing environment.
        @param uiColor    The color used in the drawing environment.
     */
    void SetDrawingEnvironment(const eC_Value &vLineWidth, const eC_UInt &uiColor);
    /** Release the appropriate resources which is selected into the DC in the
        SetDrawingEnvironment method.
     */
    void ReleaseDrawingEnviroment();

    /** @return The color bit depth reported by the internally held memory dc.
     */
    inline const USHORT GetCaps(void) const
    {
        return m_usCaps;
    }

    /// Window which will be created by user application
    HWND        m_hWnd;

    /// Graphic Device Context, this is memory dc.
    HDC         m_hMemoryDC;

    /// Graphic Device Context of actual windows' window on which the bitmap will be drawn.
    HDC         m_hVisualDC;

    /// Windows' bitmap which is associated with m_hMemoryDC
    HBITMAP     m_DCBitmap;

    /// Old bitmap
    HBITMAP     m_OldDCBitmap;

    /// Pointer to memory of offscreen buffer
    BYTE        *m_pBits;

    /** Constructor.
        @param hWnd The window used to get the visual DC.
    */
    CGfxEnvWin(const HWND &hWnd = NULL);
    virtual ~CGfxEnvWin();

    HPEN        m_hPen;     ///< Pen
    HPEN        m_hPenOld;  ///< Old pen
    HBRUSH      m_hBrush;   ///< Brush
    HBRUSH      m_hBrushOld;///< Old brush

private:
    /** Create the memory DC which is used to draw the bitmap and text.
        @param uiWidth  The width of the memory DC.
        @param uiHeight The height of the memory DC.
     */
    void CreateMemoryDC(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxEnvWin(const CGfxEnvWin& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxEnvWin& operator=(const CGfxEnvWin& kSource);

    USHORT m_usCaps;

};
#endif
