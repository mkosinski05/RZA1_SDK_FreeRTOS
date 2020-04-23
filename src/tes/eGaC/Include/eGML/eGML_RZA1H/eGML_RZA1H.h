/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Nios.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Fri Sep 09 18:47:18 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_RZA1H__H_
#define EGML_RZA1H__H_
//---------------------------------------------------------------	
#include "eGML.h"


#define Platform_eGML eGML_RZA1H

//---------------------------------------------------------------	
///
class eGML_RZA1H : public eGML
{
	public:
		enum RZA1H_BitmapType 
    {	
      BM_NATIVE_RGB32  = eGML::BM_RGB32,
      BM_NATIVE_RGB565 = eGML::BM_RGB565,
      BM_NATIVE_UNKNOWN = eGML::BM_QUANTITY,              
      BM_QUANTITY
    };

  protected:
    friend class eGML;
		eGML_RZA1H();
		virtual ~eGML_RZA1H();    

    //overriding
    virtual eGML_Screen* CreateScreen(eC_Enum eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, void **ppBitmapBuffer, eC_UByte ubBitmapBufferType);

    private:
};

//---------------------------------------------------------------	
#endif
