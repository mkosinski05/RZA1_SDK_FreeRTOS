/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Pattern.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:18:06 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_PATTERN__H_
#define EGML_PATTERN__H_
//---------------------------------------------------------------
#include "eGML_Screen.h"

//---------------------------------------------------------------
/**
@memo
Class for handling with patterns.
@doc
This class provides effective handling with patterns in line objects. Pattern objects themselves are passive, so you just have to create before
and to release after using them. This means there is a seperation between pattern parameters themselves and display informations to be
more flexible in working out visible effects . Display informations can be set/modified by using \Ref{eGML_RenderState} methods and parameters.
So in any case there is cooperation and interaction between eGML_RenderState and eGML_Pattern.
For creating pattern objects in <B>eGML</B> you only have to call <TT><B>CreatePattern</B></TT> method of \Ref{eGML_Screen}. <BR>
In contrast to decorate lines with patterns, you are able to fill surfaces with pictures by using {@link eGML_Texture texture} objects. <BR>
For getting more information about \Ref{eGML_Pattern} or its handling, look at the \Ref{Tutorial_eGML} or the following example(s): <BR>
- \URL[example_Pattern_1.cpp]{examples/example_Pattern_1.cpp.html} <BR>
@see eGML_Screen, eGML_RenderState, eGML_Texture
@see Tutorial_eGML
*/
class eGML_Pattern
{
  public:
    
    /* !!! INTERNAL USE ONLY !!!
    if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
    EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW
    
    // !!! INTERNAL USE ONLY !!!
    inline const eGML_Color *GetColors() { return aclColors; }
    
    /** Deletes pattern object. Calls private destructor.
    @see CreatePattern */
    void Release() {delete this; }
    
  protected:
    
    friend class eGML_Screen;
    
    // !!! INTERNAL USE ONLY !!!
    // all following protected members are for internal use only
    
    // protected constructor.
    eGML_Pattern(eGML_ColorARGB32 *pRGB32Colors, eC_UInt uiNbOfColors, eGML_Bitmap *pkBitmap);

	#ifndef eGML_LINEPATTERN_DISABLE
    // protected destructor.
    virtual ~eGML_Pattern();
	#endif
    
    // pattern scaled & filtered (wrapping) to 2^8 entries. converted to native color
    eGML_Color aclColors[256];
	eGML_Color aclColors2[256];
};

//---------------------------------------------------------------
#endif
