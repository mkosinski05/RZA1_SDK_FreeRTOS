/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_FileHandler.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 16:59:55 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_FILE_HANDLER__H_
#define EC_FILE_HANDLER__H_

//-----------------------------------------------------------------------------------------
#include "eC_Config.h"
#include "eC_Platform.h"
#include "eC_File.h"
#include "eC_TList.h"
#include "eC_String.h"

//---------------------------------------------------------------
class eC_FileHandler
{
  public:
  
  inline eC_FileHandler() {};
  virtual ~eC_FileHandler() {};
 
  virtual eC_File *OpenFile( eC_Char *pcName, eC_Flag uiFlag)=0;
};


//---------------------------------------------------------------
class eC_FileHandlerMemory : public eC_FileHandler
{
  public:
  struct eC_FHM_Data
  {
    char          *m_pcFileName;
    unsigned char *m_pubData;
    unsigned long *m_pulSize;    
  };

  eC_FileHandlerMemory(eC_FHM_Data *ptFileData=0, eC_UInt uiDataCnt=0);

  virtual eC_File *OpenFile( eC_Char *pcName, eC_Flag uiFlag);
  
  void Append(eC_FHM_Data *ptFileData, eC_UInt uiDataCnt);

  private:
    eC_TList<eC_FHM_Data> m_kList;
};

//---------------------------------------------------------------
class eC_FileHandlerPlatform : public eC_FileHandler
{
  public:
  virtual eC_File *OpenFile( eC_Char *pcName, eC_Flag uiFlag);
};

//---------------------------------------------------------------
class eC_FileHandlerBuilder : public eC_FileHandlerPlatform
{
  public:
  
  enum eC_FileHandlerBuilder_Flags
  {
    FHB_CREATE_C   = 1,
    FHB_CREATE_H   = 2,
    FHB_MULTIFILES = 4
  };

  virtual ~eC_FileHandlerBuilder();
  virtual eC_File *OpenFile( eC_Char *pcName, eC_Flag uiFlag);

  eC_Bool CreateFiles(eC_Char *pcName, eC_Flag uiFlag);

  private:
  void WriteString( eC_File *pkFile, eC_Char *pcString, eC_Bool bCRLF );

  struct eC_FHB_Data
  {
    eC_String m_kFileName;
    eC_String m_kCodeName;
    eC_String m_kSizeName;
    eC_UInt   m_uiSize;
  };

  eC_TList<eC_FHB_Data *> m_kList;


};


//---------------------------------------------------------------
#endif

