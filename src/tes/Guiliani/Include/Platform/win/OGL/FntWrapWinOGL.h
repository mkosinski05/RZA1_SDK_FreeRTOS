/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_WIN_OGL__H__
#define __FNT_WRAP_WIN_OGL__H__

#include "FntWrapWin.h"

/// Specialization of CFntWrapWin for OpenGL.

/**
    This is the implementation of the windows font wrapper with additional
    functionalities of OpenGL font blitting.
*/
// @guiliani_doxygen limitations CFntWrapWinOGL
/**
 FntWrapWinOGL is limited to ASCII characters (Class CFntWrapWinOGL)
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CFntWrapWinOGL "CFntWrapWinOGL Platform Specific Limitations"
*/
class CFntWrapWinOGL :
    public CFntWrapWin
{
public:
    /** Create an OpenGL Windows font wrapper object. It should be created
        in implementation after creation of GfxWrap.
        @param hDC The windows DC in which the text will be shown.
        @return True, if it is successfully created, False otherwise.
     */
    static eC_Bool CreateInstance(const HDC hDC);

private:
    /** Constructor
        @param The DC where the text will be printed.
    */
    CFntWrapWinOGL(const HDC hDC);
    virtual ~CFntWrapWinOGL(void);

    void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);
    void LoadFont(const FontResource_t &eFontID);
    void UnloadFont (const FontResource_t &eFontID);
    eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapWinOGL(const CFntWrapWinOGL& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapWinOGL& operator=(const CFntWrapWinOGL& kSource);

    // All OpenGL font list IDs infact this are simply IDs
    // of displaylists with bitmap calls inside
    eC_TArray<eC_Int> m_pOglFontListIDs;
};
#endif
