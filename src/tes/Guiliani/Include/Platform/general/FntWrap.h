/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP__H__
#define __FNT_WRAP__H__

#include "eC_Types.h"
#include "GUIFontResource.h"
#include "GUIText.h"
#include "GUITrace.h"

class CGUIRenderTarget;
class eC_String;

/// The font wrapper base class.

/**
    This is the font wrapper base class.
    Guiliani is designed independently of a certain font engine. This class declares interfaces
    for handling all font functions such as loading fonts and printing text on the screen.

    <b>Please be aware that due to the cross-platform character of the framework, not all functions declared
    within the CFntWrap base class need to be available in all platform-specific implementations.</b>
    @ingroup GUILIANI_GRAPHICS
*/

class CFntWrap
{
public:

    /** The graphics wrapper has the possibilities to access member functions
        of the font wrapper.
    */
    friend class CGfxWrap;

    /**
       Definition of font styles.
    */
    enum GUIFont_t
    {
        FNT_NORMAL = 0,
        FNT_ITALIC =  1,
        FNT_UNDERLINED = 2,
        FNT_BOLD = 4
    };

    /**
       Delete the instance if it exists. Set the pointer NULL.
    */
    static void DeleteInstance();

protected:

    /// Constructor
    CFntWrap();

    /// Destructor
    virtual ~CFntWrap();

    /** Print the text to screen.
        @param vX1 Absolute x position of the top-left corner of the bounding rectangle.
        @param vY1 Absolute y position of the top-left corner of the bounding rectangle.
        @param lpString The string to be printed.
    */
    virtual void Text(
        const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString) = 0;

    // ---------------------------------------------------------------
    // Font-access
    // ---------------------------------------------------------------

    /** Sets a new font for the application.
        @param eID id (and hence index) of the font.
    */
    virtual void SetFont (const FontResource_t &eID);

    /** Sets a new font spacing, i.e the space between letters.
        @param fSpacing New font spacing.
    */
    virtual void SetFontSpacing (const eC_Float &fSpacing);

    /** Calculates the required space in x- and y-direction of a given text with the currently active font.
        @param pkText The text.
        @param vWidth Reference to width of the font in pixels.
        @param vHeight Reference to height of the font in pixels.
    */
    virtual void RequiredSpace(
        const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight) = 0;

    /** Calculates number of characters that fit in a given width with the currently active font.
        @param pkText The text.
        @param vWidthMax Maximum width for the text in pixels.
        @param uiNumChars Reference value returning the number of characters that fit into that width.
    */
    virtual void FittingNumChars(const eC_String * const pkText, eC_Value vWidthMax, eC_UInt &uiNumChars);

    /** Returns the currently set font's internal leading.
        If the internal leading is not supported by a given Font-Wrapper, this returns 0.
        @return internal leading in pixels
    */
    virtual eC_Int GetInternalLeading() const
    {
        return 0;
    }

    /** Change the font at position eFontID with the font identified by pcPath.
        If the font already exists, it is unloaded first.
        @param eFontID id (and hence index) of the font.
        @param pcPath Path or name of the font.
        @param uiFontSize Size of the font.
        @param eFontStyle Style of the font (italic, underline, bold, ... see GUIFont.h).
    */
    void ChangeFont(const FontResource_t &eFontID,
        const eC_String& pcPath,
        const eC_UInt uiFontSize,
        const GUIFont_t &eFontStyle);

    /** Loads a font identified by eFontID into an array of fonts at position eFontID.
        If the font already exists, it's not loaded again. Should only be called by a FntWrapper.
        @param eFontID id (and hence index) of the font.
        @return True Font loades successfully.<BR>
        False else.
    */
    virtual void LoadFont(const FontResource_t &eFontID) {}

    /** Removes the font from the font list. Should only be called by a FntWrapper.
        @param eFontID Id of font to be removed.
    */
    virtual void UnloadFont(const FontResource_t &eFontID) {}

    /** Sets the total number of fonts. This is called internally by the ResourceManager when dynamically
        allocated font resources require the total number of fonts to be raised. Derived classes will
        have to override this method and resize their internal font-storage structures according to
        the new number of fonts which will have to be stored.
        @param uiNOFFonts The number of fonts
        @return True if internal storage has been successfully adapted to support the requested
                number of fonts. False otherwise.
    */
    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

    /** Gets the number of fonts (array size).
        @return Number of fonts.
    */
    inline virtual eC_UInt GetNOFFonts() {return m_uiNOFFonts;}

    /**
       Get the x position adjustment because of platform specific font structure.
       @param eHorAlignment The horizontal alignment of GUIText object.
       @return The x position adjustment. The GUItext object should add this adjustment to
       get the correct position.
    */
    virtual eC_Value GetTextXPosAdjustment(const CGUIText::HorAligned_t eHorAlignment){return 0;}

    /**
       Get the Y position adjustment because of platform specific font structure.
       @param eVerAlignment The vertical alignment of GUIText object.
       @return The y position adjustment. The GUItext object should add this adjustment to
       get the correct position.
    */
    virtual eC_Value GetTextYPosAdjustment(const CGUIText::VerAligned_t eVerAlignment){return 0;}

    /**
       Sets the RenderTarget the Fnt Wrapper should use. The inherited FntWrapper has to react on this change
       @param rRenderTarget reference to RenderTarget object containing the needed information, this is gfxwrapper dependent
    */
    virtual void SetRenderTarget(const CGUIRenderTarget& rRenderTarget)
    {
        GUILOG( GUI_TRACE_DEBUG, "This Font-Wrapper does only support rendering directly to the screen. Text may thus not be visible on BitmapPlanes.\n");
    };

    /// Current font id.
    FontResource_t m_eCurFont;

    /// Font spacing, i.e the space between letters.
    eC_Value m_vSpacing;

    /// static protected member pointer to Font-Wrapper instance
    static CFntWrap *ms_pFntWrap;

    /// Total number of fonts
    eC_UInt m_uiNOFFonts;

 private:
    /** Copy-constructor. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrap(const CFntWrap& kSource);

    /** Operator= method. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrap& operator=(const CFntWrap& kSource);
};
#endif
