/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GFXWRAPEAGL__H_
#define GFXWRAPEAGL__H_

#include "EAGLView.h"
#include "GfxWrapGL.h"



/// The EAGL graphic wrapper implementation.
class CGfxWrapEAGL: public CGfxWrapGL
{
public:
    // ---------------------------------------------------------------
    // Construction / destruction
    // ---------------------------------------------------------------

    /** Creates the singleton instance of this wrapper.
        @param pWindow The application's UIWindow.
        @param eOrientation The orientation in which to display the GUI.
                            If necessary, all contents are rotated when drawing. The same
                            orientation should be used for the input device.
        @return Returns True if an instance was created by this call, otherwise False.
        @remark In landscape mode, the bitmap planes do not work correctly.
    */
    static eC_Bool CreateInstance(UIWindow* pWindow, IPhoneOrientation_t eOrientation);

    // ---------------------------------------------------------------
    // General functionality
    // ---------------------------------------------------------------

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    /** Creates the internal screen. Always has full screen size.
        @param uiWidth Screen width
        @param uiHeight Screen height
    */
    void CreateScreen(const eC_UInt &uiWidth = 0, const eC_UInt &uiHeight = 0);

    CGUIBitmapPlane* CreateGfxDependentBitmap(GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth, const eC_Value &vHeight);

    /** If necessary, this re-implementation rotates the clipping
        rectangle and the invalidated rectangle to fit the rotated
        GUI before calling the base class implementation.
        @param rkNewClipRect The new clipping rectangle
    */
    virtual void SetCliprect(const CGUIRect& rkNewClipRect);

    virtual eC_Bool DrawToBitmap(GUIBitmapPlaneID_t uiBitmapID);

    virtual eC_Bool DrawToScreen();

protected:
    /// Singleton constructor.
    CGfxWrapEAGL();

    /**
        Destructor.
    */
    virtual ~CGfxWrapEAGL();

    /**
        Initializes this wrapper.
        @param pWindow Pointer to the iPhone window.
        @param eOrientation The orientation used for displaying the GUI.
    */
    void Init(UIWindow* pWindow, IPhoneOrientation_t eOrientation);

    /// Pointer to EAGLView object.
    EAGLView* m_pGLView;
};
#endif //GFXWRAPEAGL__H_
