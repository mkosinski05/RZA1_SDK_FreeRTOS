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
        /** Constructor */
        CGUIImageDecoderRAW();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);
};

#endif // CGUIIMAGEDECODERRAW_H
