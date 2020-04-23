/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPEGMLWIN__H_
#define GFXWRAPEGMLWIN__H_

#include "GfxWrapeGML.h"
#include "GfxEnvWin.h"
#include "GUIRect.h"


/// This is the eGML implementation of the graphics wrapper in windows version.

class CGfxWrapeGMLwin : public CGfxWrapeGML, protected CGfxEnvWin
{
public:
    /**
    Creates an instance of CGfxWrapeGMLwin object and sets it as the active graphics wrapper
    @param hWnd The window instance for this wrapper.
    @param uiScreenWidth The screen width.
    @param uiScreenHeight The screen height.
    @param ubBitPerPixel Desired color depth in Bit per Pixel. 0 will use the platform's default.
    @return Indicate whether by this call, an instance is created.
            The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd,
                                  const eC_UInt &uiScreenWidth,
                                  const eC_UInt &uiScreenHeight,
                                  const eC_UByte &ubBitPerPixel=0);

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    void SetForegroundColorImpl(const eC_UByte &ubRed, const eC_UByte &ubGreen, const eC_UByte &ubBlue, const eC_UByte& ubAlpha=255);
    virtual void SetCliprect(const CGUIRect &NewClipRect);

    ///@return Windows device context
    inline HDC GetScreen() const {return m_hMemoryDC;}

    /** This wrapper can be used in combination with windows ClearType fonts (see CFntWrapWin). But for this to work calls to several potentially
        time consuming GDI interface need to be done. You can avoid these GDI calls if you do not wish to use Windows ClearType fonts.
        By default, calls to GDI are enabled.
        @param bDisableGDI TRUE if calls to GDI interfaces shall be disabled where possible. False otherwise.
    */
    void SetDisableGDI( const eC_Bool bDisableGDI) { m_bDisableGDI = bDisableGDI; }

    /** By default Guiliani will update the screen by copying all invalidated areas from the back to the front buffer.
        If you wish to update the entire screen at once, you may switch on FullscreenRefresh, which will then copy the entire
        area of the GUI from the back to the front buffer once per frame.
        @param bFullscreenRefresh TRUE will result in one copy operation from back to front buffer, FALSE (the default) will do partial refreshs of the screen
    */
    void SetFullscreenRefresh( const eC_Bool bFullscreenRefresh) { m_bFullscreenRefresh = bFullscreenRefresh; }

protected:
    /**
    @param hWnd The window used to create the screen.
    */
    CGfxWrapeGMLwin(const HWND &hWnd);
    virtual ~CGfxWrapeGMLwin(void);

    /** Creates a Screen with the given parameters.
    @param uiWidth The screen width.
    @param uiHeight The screen height.
    @param ubBitPerPixel Desired color depth in Bit per Pixel.
    */
    virtual void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight,const eC_UByte &ubBitPerPixel);

    /// Setting m_bDisableGDI to false avoids calls to potentially time consuming GDI interfaces
    eC_Bool m_bDisableGDI;

    /// Setting m_bFullscreenRefresh to true will copy the entire back to the front buffer instead of performing partial refreshs of all invalidated rectangles
    eC_Bool m_bFullscreenRefresh;

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapeGMLwin(const CGfxWrapeGMLwin& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapeGMLwin& operator=(const CGfxWrapeGMLwin& kSource);
};
#endif
