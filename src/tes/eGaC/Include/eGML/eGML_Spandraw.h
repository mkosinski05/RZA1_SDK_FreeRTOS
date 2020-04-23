/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Spandraw.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:19:56 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags
25-08-08     | hh04031       | - seperated the span-classes in seperate files, to prevent code confusion
                               - added pixel by pixel clipping
******************************************************************************/


#ifndef EGML_SPANDRAW__H_
#define EGML_SPANDRAW__H_
//---------------------------------------------------------------	

#include "eDBG_Statistic.h"
#include "eGML_Config.h"
#include "eC_Math.h"
#include "eGML_Pattern.h"




// required for compiling pure Trimedia
#ifndef eGML_TARGETGAMMA
#define eGML_TARGETGAMMA 2.2
#endif

// overflowcheck. This is a mocro, because of frequent changes of this check-methode
#ifndef BREAK_ON_OVERFLOW
#define BREAK_ON_OVERFLOW(MINOR_POS,MAX_MINOR,OFFSET)  if ((eC_FixToInt(MINOR_POS)+OFFSET) > eC_FixToInt(MAX_MINOR)) return
#endif
#ifndef BREAK_ON_UNDERFLOW 
#define BREAK_ON_UNDERFLOW(MINOR_POS,MIN_MINOR)  if (eC_FixToInt(MINOR_POS) < eC_FixToInt(MIN_MINOR)) return
#endif 

#ifndef eGML_FILTERTABLE
#define eGML_FILTERTABLE	64		// size of filtertable in bytes (size of one cacheline)
#endif
#ifndef eGML_FILTERSHIFT	
#define eGML_FILTERSHIFT	5		// (1 << filtershift) = filtertable / 2
#endif

//---------------------------------------------------------------
#ifdef eGML_FILTEREDAA
extern eC_UByte	g_eGML_ubFilter[ eGML_FILTERTABLE + 1 ];
extern void eGML_InitLineFilter();
#endif

//---------------------------------------------------------------
#ifndef eGML_GAMMA___
#define eGML_GAMMA___
extern eC_UByte g_eGML_ubGammaRamp[ 256 ];
extern void eGML_InitGammaRamp(eC_Value vGamma = eC_FromFloat( (float) eGML_TARGETGAMMA ) );
#endif

//---------------------------------------------------------------
// how to combine diffrent alphavalues when antialiasing corners
#ifndef eGML_DISTCOMBINE
#define eGML_DISTCOMBINE(d1, d2)		((d1<d2)?d1:d2)
#endif
//#define eGML_DISTCOMBINE(d1, d2)		((d1 * d2) >>8)

//---------------------------------------------------------------
// use gamma lookup before blending if required

#ifndef eGML_GAMMACORRECTAA______
#define eGML_GAMMACORRECTAA______
#ifdef  eGML_GAMMACORRECTAA
#define BlendPixelGamma( p, c, a )      BlendPixel( p, c, g_eGML_ubGammaRamp[ (eC_UByte) (a) ] )
#else
#define BlendPixelGamma( p, c, a )      BlendPixelSrcAlpha( p, c, a )
#endif
#endif

//---------------------------------------------------------------
#ifndef eGML_LINEPATTERN_DISABLE
#ifndef eGML_Line_patternStyle_DEFINE_ONCE
#define eGML_Line_patternStyle_DEFINE_ONCE
struct eGML_Line_patternStyle
{    
	// Pattern data
	eGML_Pattern    *pkPattern;
	const eGML_Color  *aclColorBase;
	const eGML_Color  *aclColors;
	// Additional interpolator
	eC_Fixed         fxTex;
	eC_Fixed         fxTexInc;
	eC_Fixed         fxTexMul;
	eC_Fixed		 fxTexOffset;
	eC_Bool          bFlip;   
	// References to original startpoint
	eGML_Line_patternStyle(const eGML_Vector2 &kA) : kStart(kA)  {;}
	const eGML_Vector2 &kStart;
};
#endif
#endif

	#include "eGML_HSpanRender_AA.h"
	#ifdef eGML_FILTEREDAA
	#include "eGML_HSpanRender_HQ_AA.h"
	#endif
	#include "eGML_HSpanRender.h"

	#include "eGML_VSpanRender_AA.h"
	#ifdef eGML_FILTEREDAA
	#include "eGML_VSpanRender_HQ_AA.h"
	#endif
	#include "eGML_VSpanRender.h"

#endif
