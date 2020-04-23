/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_GDI_Screen.h
* %version:        134.0 %
* 
* author:          hh04030
* creation date:   Tue Feb 14 08:42:08 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_GDISCREEN__H_
#define EGML_GDISCREEN__H_
//---------------------------------------------------------------   
#include <windows.h> 
#include "../eGML_Screen.h"

//---------------------------------------------------------------   

///
class eGML_GDI_Screen : public eGML_Screen
{
public:

    ///
    virtual ~eGML_GDI_Screen();

    ///
    virtual void ShowBuffer(eC_UInt uiBuffer);

    ///
    inline HWND GetHWND() { return (m_HWND_Screen); }

    // Arrange all Screenwindows to on a grid
    static void RearrangeWindows();

    ///
    eC_Flag ScaleMouseCoord2Screen(eC_UInt &ruiX, eC_UInt &ruiY);

protected:
    friend class eGML_GDI;

    /// private constructor. construction only via eGML_GDI baseclass
    eGML_GDI_Screen(eGML_GDI *pkInstance, eC_Enum eType, eC_Enum eFormat, eC_UInt uiViewDepth, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, HWND hWnd=0);

    // GDI Screen window handle (WIN32 - HWND)
    HWND     m_HWND_Screen;

    // for windows to application mouse coord conversion
    eGML_Vector2  m_vMouseOffset;
    eGML_Vector2  m_vMouseScale;

public:
    bool   m_bFullScreen, isFull;
    // eGML instance is a global singleton, so pointer could be static for all Windows
    static eGML_GDI * m_pkParentEGML;
};

//---------------------------------------------------------------   
#endif

