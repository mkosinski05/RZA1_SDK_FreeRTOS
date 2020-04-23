/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:15:39 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML__H_
#define EGML__H_

//---------------------------------------------------------------------------------------
/** @name Component_eGML
@memo
The <B>e</B>mbedded <B>G</B>raphic <B>M</B>ulti-Platform <B>L</B>ibrary.
@doc
This chapter performs the complete description of the whole <B>eGML</B> API for developing graphic applications.
The following overview represents the API segmentation. <BR>
For a structured introduction into the {@link Component_eGML eGML} please use the \Ref{Tutorial_eGML}.
@see Tutorial_eGML
*/

//@{

//@Include: ../../comps/eGML/code/eGML_Config.h
//@Include: ../../comps/eGML/code/eGML_Bitmap.h
//@Include: ../../comps/eGML/code/eGML_Screen.h
//@Include: ../../comps/eGML/code/eGML_Linedraw.h
//@Include: ../../comps/eGML/code/eGML_RenderState.h
//@Include: ../../comps/eGML/code/eGML_Primitive.h
//@Include: ../../comps/eGML/code/eGML_Grobi.h
//@Include: ../../comps/eGML/code/eGML_Pattern.h
//@Include: ../../comps/eGML/code/eGML_Texture.h
//@Include: ../../comps/eGML/code/eGML_Vector2.h
//@Include: ../../comps/eGML/code/eGML_Vector3.h
//@Include: ../../comps/eGML/code/eGML_Matrix3.h
//@Include: ../../comps/eGML/code/eGML_Matrix2.h
//@Include: ../../comps/eGML/code/eGML_Renderer.h

//@}

//---------------------------------------------------------------------------------------
#include "eC.h"

#include "eGML_Config.h"
#include "eGML_Bitmap.h"
#include "eGML_Screen.h"
#include "eGML_Vector2.h"
#include "eGML_Vector3.h"
#include "eGML_Matrix2.h"
#include "eGML_Matrix3.h"
#include "eGML_Renderer.h"

//---------------------------------------------------------------------------------------
//now include Platform depending stuff
#include "eGML_Platform.h"

//---------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
class eGML_Base
{
public:
  inline eGML_Base() {;}
  virtual ~eGML_Base() {;}

#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
};

//---------------------------------------------------------------------------------------
#endif
