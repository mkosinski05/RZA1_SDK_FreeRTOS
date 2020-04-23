/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Nios_Screen.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Fri Sep 09 18:47:27 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_RZA1HSCREEN__H_
#define EGML_RZA1HSCREEN__H_
//---------------------------------------------------------------	
#include "eGML_RZA1H.h"

extern "C" 
{
    void GrpDrv_SetFrameBuffer(void * ptr);
}

class eGML_RZA1H_Screen: public eGML_Screen {
public:
    virtual ~eGML_RZA1H_Screen();

    //overriding
    virtual void ShowBuffer(eC_UInt uiBuffer);

protected:
    friend class eGML_RZA1H;

    //constructor
    eGML_RZA1H_Screen(eGML * pEgml, eC_Enum eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, void **ppBitmapBuffer, eC_UByte ubBitmapBufferType);
};

#endif /* EGML_RSK2_SH7264_SCREEN_H_ */
