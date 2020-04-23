#ifndef CGUIIMAGEDECODERRLE_H
#define CGUIIMAGEDECODERRLE_H

#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"
#include "GUIImageData.h"

/** Reads a SVG-file with NanoSVG and transforms it into a std::vector<CSVGShape>
thas is also the representation as CGUIImageData.
@todo size != width*height*bits
*/
class CGUIImageDecoderRLE : public CGUIImageDecoder
{
public:
    /** Constructor */
    CGUIImageDecoderRLE();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);
};

#endif // CGUIIMAGEDECODERRLE_H
