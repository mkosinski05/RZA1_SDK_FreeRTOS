#ifndef CGUIIMAGEDECODERBLU_H
#define CGUIIMAGEDECODERBLU_H

#include "eC_Types.h"
#include "eC_File.h"

#include "GUIImageDecoder.h"

/** Reads a raw-image
*/
class CGUIImageDecoderBLU : public CGUIImageDecoder
{
    public:
        /** Constructor */
        CGUIImageDecoderBLU();

    virtual eC_Bool LoadImg(CGUIImageData &pImageData, eC_File *pTempImageFile);
};

#endif // CGUIIMAGEDECODERRAW_H
