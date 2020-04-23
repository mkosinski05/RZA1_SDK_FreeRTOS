/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef FNTWRAPFREETYPERGA_H_
#define FNTWRAPFREETYPERGA_H_

#include "FntWrapFreeType.h"
#include "RGA_Platform.h"
#include "RGA.h"

// Macros defined as per RGA documentation
#define RAW_IMAGE_ALIGNMENT                 0x20 // 32 byte alignment for raw image
#define RAW_IMAGE_OFFSET_TO_DATA            0x20 // Actual start of data of raw image is at 32nd byte
#define RAW_IMAGE_WIDTH_BYTE_POS            0xC
#define RAW_IMAGE_HEIGHT_BYTE_POS           0xE
#define RAW_IMAGE_PIXEL_FORMAT_BYTE_POS     0x10
#define RAW_IMAGE_PIXEL_FORMAT              0x14 // A8 pixel format for glyph/text
#define RAW_IMAGE_PREMULTIPLIED_ALPHA       0x0

/** @brief FreeType font wrapper specialization using RGA.
<h3>Platform Specific Limitations </h3>
*/

/**
 When drawing characters of raster font, specify the image of A8, A4, A1 (Alpha Only) format to image argument.
 But, the width of the image must be same as width of drawing target frame buffer for RZ/A1H.
 (Class CFntWrapFreeTypeRGA)
 See @ref subsec_limitations_CGfxWrapRGA "CGfxWrapRGA Platform Specific Limitations"
*/

class CFntWrapFreeTypeRGA:
    public CFntWrapFreeType
{
public:
    /** Create a FntWrap-FreeType object. It should be created
    in implementation after creation of GfxWrap.
    @param uiCacheSize The max size of the freetype cache in bytes.
    @param pkCanvasContext Graphics drawing context of RGA.
    @param pkBuffer Frame buffer of drawing target.
    @param uiMaxNumFaces Maximum number of opened font face objects managed by the FreeType cache.
    @param uiMaxNumSizeObjects Maximum number of opened size objects managed by the FreeType cache.
    @return True, if it is successfully created, False otherwise.
    */
    static eC_Bool CreateInstance(const eC_UInt& uiCacheSize,
                                  Canvas2D_ContextClass *pkCanvasContext,
                                  frame_buffer_t *pkBuffer,
                                  const eC_UInt& uiMaxNumFaces = 10,
                                  const eC_UInt& uiMaxNumSizeObjects = 10);

protected:
    /** Renders a glyph to the screen.
    This function takes the 8 bit alpha image provided by freetype, adds color and blits it to the screen.
    @param vX1 The x coordinate where the glyph image is rendered.
    @param vY1 The y coordinate where the glyph image is rendered.
    @param kGlyphImage The glyph image containing a 8 bit alpha image of the glyph.
    */
    virtual void RenderGlyphToScreen(const eC_Value &vX1, const eC_Value &vY1, CGUIImageData& kGlyphImage);

private:
    /*
        Constructor
        @param uiCacheSize The maximum size of the FreeType cache in byte.
        @param pkCanvasContext Graphics drawing context of RGA.
        @param pkBuffer Frame buffer of drawing target.
        @param uiMaxNumFaces Maximum number of opened font face objects managed by the FreeType cache.
        @param uiMaxNumSizeObjects Maximum number of opened size objects managed by the FreeType cache.
    */
    CFntWrapFreeTypeRGA(const eC_UInt& uiCacheSize,
                        Canvas2D_ContextClass *pkCanvasContext,
                        frame_buffer_t *pkBuffer,
                        const eC_UInt& uiMaxNumFaces,
                        const eC_UInt& uiMaxNumSizeObjects);

    /** Destructor
    */
    virtual ~CFntWrapFreeTypeRGA(void);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeRGA(const CFntWrapFreeTypeRGA& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeRGA& operator=(const CFntWrapFreeTypeRGA& kSource);

    Canvas2D_ContextClass *m_pkCanvas2d; ///< Graphics drawing context of RGA
    frame_buffer_t *m_pkBuffer; ///< Drawing target frame buffer.

    #ifdef RGA_MEASURE_PERFORMANCE
        r_ospl_ftimer_spec_t  kTimer; // for measuring performance
    #endif
};

#endif /* FNTWRAPFREETYPERGA_H_ */
