/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_WIN__H__
#define __FNT_WRAP_WIN__H__

#include "FntWrap.h"
#include <windows.h>

/// Font wrapper for GDI.
class CFntWrapWin :
    public CFntWrap
{
public:
    /** Create a windows font wrapper object. It should be created
        in implementation after creation of GfxWrap.
        @param hDC The windows DC in which the text will be shown.
        @return True, if it is successfully created, False otherwise.
     */
    static eC_Bool CreateInstance(const HDC hDC);

protected:
    ///@return The Windows device context
    inline HDC GetDC(){return m_DC;}
    /** Perpare the context for printing the text.
        @param pkString The string to be printed.
        @return The result string which will be
                printed after this preparation.
     */
    const TCHAR* PreparePrintText(const eC_String * const pkString);
    /** Constructor
        @param hDC The DC where the text will be printed.
    */
    CFntWrapWin(const HDC hDC);
    virtual ~CFntWrapWin(void);

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);
    void LoadFont(const FontResource_t &eFontID);
    void UnloadFont(const FontResource_t &eFontID);

    /// The DC where the text will be printed.
    HDC         m_DC;

protected:
    void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);
    void SetFont(const FontResource_t &eFontID);
    void RequiredSpace(const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);

    /** Get the y position adjustment because of the internal leading in the windows font.
        @param eVerAlignment Vertical alignment of the text.
        @return Y adjustment in pixels
    */
    eC_Value GetTextYPosAdjustment(const CGUIText::VerAligned_t eVerAlignment);

    /** This attribute is for the resizing of allocated memory if necessary to avoid fragmentation
        of the memory.
        The method PrepareToPrint is called all the time. So the memory to write into would be
        allocated and deleted all the time. This causes memory fragmentation. To avoid this,
        the memory will be reallocated if required memory is larger. This attribute
        holds information about the amount of current allocated memory.
    */
    eC_UInt         m_uiAmount;

    /** Holds the current allocated memory for the fragmentation avoidance. */
    eC_Char*    m_pcText;

    /// Window font.
    eC_TArray<HFONT> m_phWinFont;

    /**
       Old font handle which should be restored after destroy the graphics wrapper.
    */
    HFONT       m_hWinFontOld;

    /// Storage for text metrics of the currently active font
    TEXTMETRIC  m_sTextMetric;

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapWin(const CFntWrapWin& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapWin& operator=(const CFntWrapWin& kSource);
};
#endif
