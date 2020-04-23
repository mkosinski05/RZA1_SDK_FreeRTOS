#ifndef CGUIIMAGEDECODERRAW_H
#define CGUIIMAGEDECODERRAW_H

#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"

/** Reads a raw-image
*/
class CGUIImageDecoderRAW : public CGUIImageDecoder
{
public:
    CGUIImageDecoderRAW();

    virtual eC_Bool LoadImg(CGUIImageData& kImageData, eC_File* pkTempImageFile);
};

#endif // CGUIIMAGEDECODERRAW_H
