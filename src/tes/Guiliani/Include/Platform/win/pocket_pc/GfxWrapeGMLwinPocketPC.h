/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

// GfxWrapeGMLwinPocketPC.h: interface for the GfxWrapeGMLwinPocketPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GFXWRAPEGMLWINPOCKETPC__H_)
#define _GFXWRAPEGMLWINPOCKETPC__H_

#include "GfxWrapeGMLwin.h"
#include "gx.h"
#include "eGML_Pocket_Bitmap565.h"


/// This is the eGML implementation of the graphics wrapper in PocketPC Version.

class CGfxWrapeGMLwinPocketPC : public CGfxWrapeGMLwin
{
public:
    /** Constructor
        @param hWnd Window handle
    */
    CGfxWrapeGMLwinPocketPC(HWND hWnd);

    virtual ~CGfxWrapeGMLwinPocketPC();

    void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    /** Copy an eGML bitmp buffer
        @param pkBMP The bitmap in RGB565 format
        @param pusDst Destination
    */
    void CopyBuffer(eGML_Pocket_Bitmap565 *pkBMP, eC_UShort *pusDst);

    ///Update the display porperties
    void UpdateDisplayProperties();

private:
    void *m_pFrameBuffer;
    GXDisplayProperties m_DisProp;
};

#endif // #if !defined(_GFXWRAPEGMLWINPOCKETPC__H_)
