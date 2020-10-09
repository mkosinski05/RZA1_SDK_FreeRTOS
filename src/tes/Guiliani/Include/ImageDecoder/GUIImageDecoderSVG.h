#ifndef CGUIIMAGEDECODERSVG_H
#define CGUIIMAGEDECODERSVG_H

#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"
#include "GUIImageData.h"

#include "nanosvg.h"
#include "nanosvgrast.h"

#include <vector>

#include "GUIException.h"
#include "SVGShape.h"

/** Reads a SVG-file with NanoSVG and transforms it into a std::vector<CSVGShape>
    thas is also the representation as CGUIImageData.
    @todo size != width*height*bits
*/
class CGUIImageDecoderSVG : public CGUIImageDecoder
{
    public:
        /** Constructor */
        CGUIImageDecoderSVG();
        /** Destructor */
        ~CGUIImageDecoderSVG();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);

private:
    ///parse SVG-file with NanoSVG
    NSVGimage* Parse(eC_File* pkFile, const eC_Char* units, eC_Value dpi);

    ///these helper-functions translate from NanoSVG to Guiliani data
    void ParsePaths(NSVGshape const* pkParserShape, CSVGShape& kShape);
    void ParseFill(NSVGshape const* pkParserShape, CSVGShape& kShape);
    void ParseStroke(NSVGshape const* pkParserShape, CSVGShape& kShape);
    void ParseGradients(const NSVGgradient& kParserGradient, CGradient& kGradient);

    ///convert the swapped RGBA values
    eC_UInt TranslateColor(eC_UInt c, eC_UByte opacity);

    /** Translate gradient-coords to user space.
        There are a few bugs, the radial gradient is sometimes off.
    */
    void XTransform(std::vector<CSVGShape>& m_akSvgShapes);

    ///Rasterizer
    NSVGrasterizer* m_pkRaster;

    ///calculates the real size of SVG-data, not height*widht*bytesPerPixel
    void CalculateSizeOfSVG(std::vector<CSVGShape>* pakSvgShapes);
};

#endif // CGUIIMAGEDECODERSVG_H
