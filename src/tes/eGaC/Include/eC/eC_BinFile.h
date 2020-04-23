/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_BinFile.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Tue May 02 11:14:03 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EC_BINFILE__H_
#define EC_BINFILE__H_
//---------------------------------------------------------------

#include "eC_File.h"
#include "eC_TArray.h"

//---------------------------------------------------------------
class eC_BinFile
{
	public:
		eC_BinFile( eC_File *pkFile, eC_Bool bOwnFile = false ) : 
		  m_pkFile(pkFile), m_bOwnFile(bOwnFile)			{ m_bIsReady = Begin(); }
		~eC_BinFile()										{ if (m_bOwnFile) delete m_pkFile; }

		eC_Bool HasChunk( eC_Int iChunkID );
		eC_Int  NextChunk();

		inline eC_Bool IsOpen() const						{ return m_pkFile->IsOpen(); }
		inline eC_Bool IsValid() const						{ return (m_bIsReady) && (IsOpen()); }
		inline const eC_Char *GetFileIDString() const		{ return (m_bIsReady)?((const eC_Char*) m_acFileID):NULL; }
		inline eC_UInt GetFileID() const					{ return (m_bIsReady)?(m_uiFileID):0; }
		inline eC_Int  GetChunkID() const					{ return m_iChunkID; }
		inline eC_UInt GetChunkSize() const					{ return m_uiChunkSize; }
		inline eC_UInt GetChunkCount() const                { return (m_bIsReady)?(m_uiChunkCount):0; }
		inline eC_UInt GetPosition()                        { return m_pkFile->GetPosition(); }
		inline eC_UInt GetChunkRemaining() const			{ return m_uiSkipSize; }

		inline eC_UByte Read8i(eC_UByte ubDef = 0)			{ if (m_uiSkipSize>=1) { m_uiSkipSize-=1; return m_pkFile->Read8i();  } else return ubDef; }
		inline eC_UShort Read16i(eC_UShort usDef = 0)		{ if (m_uiSkipSize>=2) { m_uiSkipSize-=2; return m_pkFile->Read16i(); } else return usDef; }
		inline eC_UInt Read32i(eC_UInt uiDef = 0)			{ if (m_uiSkipSize>=4) { m_uiSkipSize-=4; return m_pkFile->Read32i(); } else return uiDef; }

		eC_UInt Read8(eC_UByte *pubData, eC_UInt uiCount = 1);  
		eC_UInt Read16(eC_UShort *pusData, eC_UInt uiCount = 1);
		eC_UInt Read32(eC_UInt *puiData, eC_UInt uiCount = 1);

	private:
		eC_Bool Begin();

	protected:
		eC_UInt             m_uiFileID;
		eC_Int				m_iChunkID;
		eC_UInt				m_uiChunkSize;
		eC_UInt				m_uiSkipSize;
		eC_UInt				m_uiChunkCount;
		eC_UByte			m_acFileID[5];
		eC_Bool             m_bIsReady;
        eC_File            *m_pkFile;
		eC_Bool				m_bOwnFile;
		eC_TArray<eC_UInt>	m_auiChunklist;
};

//---------------------------------------------------------------
#endif

