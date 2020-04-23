/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_GDI.h
* %version:        134.0 %
* 
* author:          hh04030
* creation date:   Mon Apr 24 12:55:57 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_GDI__H_
#define EGML_GDI__H_
//---------------------------------------------------------------	
#include "../eGML.h"
#include "eGML_GDI_Screen.h"

#include "eC_TList.h"

#define Platform_eGML eGML_GDI

//---------------------------------------------------------------	
///
class eGML_GDI : public eGML
{
	public:
        ///
        virtual eGML_Screen* CreateScreen(eC_Enum eType, eC_Enum eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, eC_UInt uiViewDepth=0);
        
        /// overloaded additional method to Create Screen into a given window hwnd
        eGML_Screen* CreateScreen(eC_Enum eType, eC_Enum eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, eC_UInt uiViewDepth=0, HWND hwnd = 0);

        ///
        virtual bool CheckAbortFlag();

        ///
        virtual eC_UInt GetKey();

        ///
        virtual eC_Flag GetMouse(eC_UInt &ruiX, eC_UInt &ruiY);

		//
		virtual void GetMousePos(eC_UInt &ruiX, eC_UInt &ruiY);

        // static method for global message loop for all screens
        static LRESULT CALLBACK eGML_GDI::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
        // GDI Screen List
        static eC_TList<eGML_GDI_Screen *> m_GDIScreenList;

        // The first Window (Main) is the first in the list
        // We ensure this when adding new windos to the list (see CreateGDIScreen)
        eGML_GDI_Screen * GetMainGDIScreen()
        {
            eGML_GDI_Screen * pkScreenRet = NULL;
            m_GDIScreenList.GetFirst(pkScreenRet);
            return pkScreenRet;
        }

	private:
        virtual void InitInstance();		
        virtual void ExitInstance();

        void ProcessEvents();

        static eC_UInt GetBitDepth(eC_Flag eFormat);

    protected:
        friend class eGML;
        eGML_GDI();
        virtual ~eGML_GDI();

        // Mouse Position is Target global 
        static eC_UInt m_uiKeyCode;
        static eC_UInt m_uiMx;
        static eC_UInt m_uiMy;
        static eC_Flag m_bMP;

};

//---------------------------------------------------------------	
#endif

