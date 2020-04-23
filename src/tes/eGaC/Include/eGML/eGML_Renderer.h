/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Renderer.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:19:12 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_RENDERER__H_
#define EGML_RENDERER__H_
//---------------------------------------------------------------
#include "eGML_Bitmap.h"

//---------------------------------------------------------------
class eGML_Renderer
{
	public:
		// 
		eGML_Renderer( eGML_Bitmap *pkTarget );

        /** Init has to be called once, to register the renderer with eGML. 
        @see Exit */
        static void Init();
    
        /** Exit has to be called once, after the usage any renderer instance.
        @see Init */
        static void Exit();    

		//
		virtual eC_UInt GetID() const;

		// 
		void Release() { delete this; }

		//
		inline eGML_Bitmap * GetTarget() const { return m_pkTarget; }

	protected:
		    
		// private destructor.
		virtual ~eGML_Renderer();	

		// target bitmap (used, not owned by renderer)
		eGML_Bitmap *m_pkTarget;
};


//---------------------------------------------------------------
#endif
