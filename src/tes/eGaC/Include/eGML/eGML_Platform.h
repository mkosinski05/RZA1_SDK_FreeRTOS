/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Platform.h
* %version:        134.1 %
*
* author:          hh04030
* creation date:   Wed Apr 26 10:57:40 2006
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/



// The eGML_DEFAULT Platform setting for eGML Screen and Benchmark Bitmap Includes
// Also the platformspecific includes should be done in this file

//-----------------------------------------------------------------------------
#ifdef DOCXX

// The following defines should be set for platformspecific adaptation:

/** Default screen aspect ratio. The result of width/height of the screen is <I>not</I> the same as the aspect ratio and
depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_SCREEN_ASPECT

/** Default screen width. Depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_SCREEN_WIDTH

/** Default screen height. Depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_SCREEN_HEIGHT

/** Default bitmap type of the platform. Depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_BITMAP_TYPE

/** Default number of buffers allocated when creating a screen object. Depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_BUFFERS

/** Default alpha bitmap type of the platform. Depends on the current platform.
@see Global_Platform_Defines */
#define eGML_DEFAULT_ALPHABITMAP_TYPE

#endif // DOCXX
//-----------------------------------------------------------------------------

#ifdef eC_TARGET_ENV_WIN32
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480

#ifdef eC_TARGET_GFX_DIRECTX
#include "eGML_DX/eGML_DX.h"
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_DX::BM_NATIVE_RGB32
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#endif

#ifdef eC_TARGET_GFX_SDL
#include "eGML_SDL/eGML_SDL.h"
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML_SDL::BM_NATIVE_RGB32
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#endif

#ifdef eC_TARGET_GFX_GDI
#include "windows.h"
#include "eGML_GDI/eGML_GDI.h"
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML::BM_ARGB32
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#endif

#ifdef eC_TARGET_GFX_FUJITSUGDC
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1/1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#include "eGML_FUJITSUGDC/eGML_FGDC.h"
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML_FGDC::BM_NATIVE_RGB555
#define eGML_DEFAULT_ALPHABITMAP_TYPE eGML_FGDC::BM_NATIVE_ALPHA8
#endif
#endif //eC_TARGET_ENV_WIN32


#ifdef eC_TARGET_ENV_RR
#include "eGML_RR/eGML_RR.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(416.0f/240.0f)
#define eGML_DEFAULT_SCREEN_WIDTH  416
#define eGML_DEFAULT_SCREEN_HEIGHT 240
#define eGML_DEFAULT_BUFFERS 3
#define eGML_DEFAULT_BITMAP_TYPE   eGML_RR::BM_NATIVE_555
#endif

#ifdef eC_TARGET_ENV_CRUSADER
#include "eGML_SGSU/eGML_SGSU.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(416.0f/240.0f)
#define eGML_DEFAULT_SCREEN_WIDTH  416
#define eGML_DEFAULT_SCREEN_HEIGHT 240
#define eGML_DEFAULT_BUFFERS 3
#define eGML_DEFAULT_BITMAP_TYPE   eGML_SGSU::BM_NATIVE_555
#endif


#ifdef eC_TARGET_ENV_NAVIO2
#include "eGML_TM/eGML_TM.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(480.0f/234.0f)
#define eGML_DEFAULT_SCREEN_WIDTH  480
#define eGML_DEFAULT_SCREEN_HEIGHT 234
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML_TM::BM_NATIVE_YUV
#endif

#ifdef eC_TARGET_ENV_POCKET
#include "eGML_Pocket/eGML_Pocket.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  240
#define eGML_DEFAULT_SCREEN_HEIGHT 320
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Pocket::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_UTST
#include "eGML_UTST/eGML_UTST.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_UTST::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_LINUX
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#ifdef eC_TARGET_GFX_SDL
#include "eGML_SDL/eGML_SDL.h"
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML_SDL::BM_NATIVE_RGB32
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#elif eC_TARGET_GFX_XLIB
#include "eGML_Xlib/eGML_Xlib.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Linux::BM_NATIVE_RGB32
#else
#include "eGML_LINUX/eGML_Linux.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Linux::BM_NATIVE_RGB32
#endif
#endif

#ifdef eC_TARGET_ENV_ELINUX
#include "eGML_ELINUX/eGML_eLinux.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  320
#define eGML_DEFAULT_SCREEN_HEIGHT 240
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_eLinux::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_OSE
#include "eGML_OSE/eGML_OSE.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_OSE::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_NIOS
#include "eGML_NIOS/eGML_NIOS.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  160
#define eGML_DEFAULT_SCREEN_HEIGHT 120
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML::BM_RGB32
#endif

#ifdef eC_TARGET_ENV_RZA1H
#include "eGML_RZA1H/eGML_RZA1H.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.66f)
#define eGML_DEFAULT_SCREEN_WIDTH  800
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 2
#define eGML_DEFAULT_BITMAP_TYPE   eGML::BM_RGB565
#endif

#ifdef eC_TARGET_ENV_CAPI
#include "eGML_Capi/eGML_CAPI.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.0f)
#define eGML_DEFAULT_SCREEN_WIDTH  320
#define eGML_DEFAULT_SCREEN_HEIGHT 240
#define eGML_DEFAULT_BUFFERS 3
#define eGML_DEFAULT_BITMAP_TYPE   eGML_CAPI::BM_NATIVE_RGB556
#endif

#ifdef eC_TARGET_ENV_OMAP331
#include "eGML_OMAP331/eGML_OMAP331.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  240
#define eGML_DEFAULT_SCREEN_HEIGHT 320
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_OMAP331::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_EMBOS
#include "eGML_embOS/eGML_embOS.h"
#endif

#ifdef eC_TARGET_ENV_RSK2_SH7264
#include "eGML_rsk2_sh7264/eGML_rsk2_sh7264.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH 240
#define eGML_DEFAULT_SCREEN_HEIGHT 320
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE eGML_rsk2_sh7264::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_DCX
#include "eGML_DCX/eGML_DCX.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  240
#define eGML_DEFAULT_SCREEN_HEIGHT 320
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_DCX::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_MAGIKMI
#include "eGML_MAGiKMI/eGML_MAGiKMI.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  800
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_MAGiKMI::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_SC14450
#include "eGML_LINUX/eGML_Linux.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  240
#define eGML_DEFAULT_SCREEN_HEIGHT 320
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Linux::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_HICO7723
#include "eGML_LINUX/eGML_Linux.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.33f)
#define eGML_DEFAULT_SCREEN_WIDTH  640
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Linux::BM_NATIVE_RGB565
#endif

#ifdef eC_TARGET_ENV_MS7724
#include "eGML_LINUX/eGML_Linux.h"
#define eGML_DEFAULT_SCREEN_ASPECT eC_FromFloat(1.66f)
#define eGML_DEFAULT_SCREEN_WIDTH  800
#define eGML_DEFAULT_SCREEN_HEIGHT 480
#define eGML_DEFAULT_BUFFERS 1
#define eGML_DEFAULT_BITMAP_TYPE   eGML_Linux::BM_NATIVE_RGB565
#endif

#ifndef eGML_DEFAULT_ALPHABITMAP_TYPE
#define eGML_DEFAULT_ALPHABITMAP_TYPE eGML::BM_ALPHA8
#endif


//-----------------------------------------------------------------------------------------------
#ifdef eGML_USE_LITTLE_HELPERS
//
// little helper macros
//

//-----------------------------------------------------------------------------------------------
//#ifdef DOCXX

/**
@name eGML_Little_Helpers
@memo
Macros to simplify and to accelerate working with <B>eGML</B>.
@doc
This additional vocabulary of macros will assist the conversant user to simplify and accelerate handling of the <B>eGML</B>. <BR>
Some nerving work is done by these macros, e. g. conversions to eC_Value format or you just have to write the short forms of the macro names.
But also the sensitivity of errors can be reduced, e. g. by <TT>xxx<B>Prim</B>xxx</TT> macros which executes conversions, cast operations and
buffer accesses. <BR>
But please be carful: Do not use <I>Little Helpers</I> if you are not experienced in handling with the basic code behind. <BR>
At least you should study the \Ref{Tutorial_eGML} first.
@see eC_Math, eGML_Vector2, eGML_Vector3, eGML_Matrix2, eGML_Matrix3, eGML_Primitive
@see Tutorial_eGML
*/

//@{

//-----------------------------------------------------------------------------------------------
// integer input

/** Short form of the macro eC_FromInt.
@param a Integer value to convert.
@return Conversion result.
@see eC_FromInt */
#define _I(a)                    eC_FromInt(a)

/** Short form of eGML_Vector2 constructor. The integer parameters are automatically converted to \Ref{eC_Value} format.
@param a Integer X-component value of new vector.
@param b Integer Y-component value of new vector.
@see eGML_Vector2 */
#define _I2(a,b)                 eGML_Vector2(eC_FromInt(a), eC_FromInt(b))

/** Short form of eGML_Vector3 constructor. The integer parameters are automatically converted to \Ref{eC_Value} format.
@param a Integer X-component value of new vector.
@param b Integer Y-component value of new vector.
@param c Integer Z-component value of new vector.
@see eGML_Vector3 */
#define _I3(a,b,c)               eGML_Vector3(eC_FromInt(a), eC_FromInt(b), eC_FromInt(c))

/** Short form of eGML_Matrix2 constructor. The integer parameters are automatically converted to \Ref{eC_Value} format.
@param a Integer coefficient <TT>[0][0]</TT> of new matrix.
@param b Integer coefficient <TT>[0][1]</TT> of new matrix.
@param c Integer coefficient <TT>[1][0]</TT> of new matrix.
@param d Integer coefficient <TT>[1][1]</TT> of new matrix.
@see eGML_Matrix2 */
#define _I4(a,b,c,d)             eGML_Matrix2(eC_FromInt(a), eC_FromInt(b), eC_FromInt(c), eC_FromInt(d))

/** Short form of eGML_Matrix3 constructor. The integer parameters are automatically converted to \Ref{eC_Value} format.
@param a Integer coefficient <TT>[0][0]</TT> of new matrix.
@param b Integer coefficient <TT>[0][1]</TT> of new matrix.
@param c Integer coefficient <TT>[0][2]</TT> of new matrix.
@param d Integer coefficient <TT>[1][0]</TT> of new matrix.
@param e Integer coefficient <TT>[1][1]</TT> of new matrix.
@param f Integer coefficient <TT>[1][2]</TT> of new matrix.
@param g Integer coefficient <TT>[2][0]</TT> of new matrix.
@param h Integer coefficient <TT>[2][1]</TT> of new matrix.
@param i Integer coefficient <TT>[2][2]</TT> of new matrix.
@see eGML_Matrix3 */
#define _I9(a,b,c,d,e,f,g,h,i)   eGML_Matrix3(eC_FromInt(a), eC_FromInt(b), eC_FromInt(c),\
                                              eC_FromInt(d), eC_FromInt(e), eC_FromInt(f),\
                                              eC_FromInt(g), eC_FromInt(h), eC_FromInt(i))

//-----------------------------------------------------------------------------------------------
// float input

/** Short form of the macro eC_FromFloat.
@param a Floatingpoint value to convert.
@return Conversion result.
@see eC_FromFloat */
#define _F(a)                    eC_FromFloat(a)

/** Short form of eGML_Vector2 constructor. The floatingpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Floatingpoint X-component value of new vector.
@param b Floatingpoint Y-component value of new vector.
@see eGML_Vector2 */
#define _F2(a,b)                 eGML_Vector2(eC_FromFloat(a), eC_FromFloat(b))

/** Short form of eGML_Vector3 constructor. The floatingpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Floatingpoint X-component value of new vector.
@param b Floatingpoint Y-component value of new vector.
@param c Floatingpoint Z-component value of new vector.
@see eGML_Vector3 */
#define _F3(a,b,c)               eGML_Vector3(eC_FromFloat(a), eC_FromFloat(b), eC_FromFloat(c))

/** Short form of eGML_Matrix2 constructor. The floatingpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Floatingpoint coefficient <TT>[0][0]</TT> of new matrix.
@param b Floatingpoint coefficient <TT>[0][1]</TT> of new matrix.
@param c Floatingpoint coefficient <TT>[1][0]</TT> of new matrix.
@param d Floatingpoint coefficient <TT>[1][1]</TT> of new matrix.
@see eGML_Matrix2 */
#define _F4(a,b,c,d)             eGML_Matrix2(eC_FromFloat(a), eC_FromFloat(b), eC_FromFloat(c), eC_FromFloat(d))

/** Short form of eGML_Matrix3 constructor. The floatingpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Floatingpoint coefficient <TT>[0][0]</TT> of new matrix.
@param b Floatingpoint coefficient <TT>[0][1]</TT> of new matrix.
@param c Floatingpoint coefficient <TT>[0][2]</TT> of new matrix.
@param d Floatingpoint coefficient <TT>[1][0]</TT> of new matrix.
@param e Floatingpoint coefficient <TT>[1][1]</TT> of new matrix.
@param f Floatingpoint coefficient <TT>[1][2]</TT> of new matrix.
@param g Floatingpoint coefficient <TT>[2][0]</TT> of new matrix.
@param h Floatingpoint coefficient <TT>[2][1]</TT> of new matrix.
@param i Floatingpoint coefficient <TT>[2][2]</TT> of new matrix.
@see eGML_Matrix3 */
#define _F9(a,b,c,d,e,f,g,h,i)   eGML_Matrix3(eC_FromFloat(a), eC_FromFloat(b), eC_FromFloat(c),\
                                              eC_FromFloat(d), eC_FromFloat(e), eC_FromFloat(f),\
                                              eC_FromFloat(g), eC_FromFloat(h), eC_FromFloat(i))

//-----------------------------------------------------------------------------------------------
// fixedpoint input

/** Short form of the macro eC_FixToValue.
@param a Fixpoint value to convert.
@return Conversion result.
@see eC_FixToValue */
#define _FX(a)                   eC_FixToValue(a)

/** Short form of eGML_Vector2 constructor. The fixpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Fixpoint X-component value of new vector.
@param b Fixpoint Y-component value of new vector.
@see eGML_Vector2 */
#define _FX2(a,b)                eGML_Vector2(eC_FixToValue(a), eC_FixToValue(b))

/** Short form of eGML_Vector3 constructor. The fixpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Fixpoint X-component value of new vector.
@param b Fixpoint Y-component value of new vector.
@param c Fixpoint Z-component value of new vector.
@see eGML_Vector3 */
#define _FX3(a,b,c)              eGML_Vector3(eC_FixToValue(a), eC_FixToValue(b), eC_FixToValue(c))

/** Short form of eGML_Matrix2 constructor. The fixpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Fixpoint coefficient <TT>[0][0]</TT> of new matrix.
@param b Fixpoint coefficient <TT>[0][1]</TT> of new matrix.
@param c Fixpoint coefficient <TT>[1][0]</TT> of new matrix.
@param d Fixpoint coefficient <TT>[1][1]</TT> of new matrix.
@see eGML_Matrix2 */
#define _FX4(a,b,c,d)            eGML_Matrix2(eC_FixToValue(a), eC_FixToValue(b), eC_FixToValue(c), eC_FixToValue(d))

/** Short form of eGML_Matrix3 constructor. The fixpoint parameters are automatically converted to \Ref{eC_Value} format.
@param a Fixpoint coefficient <TT>[0][0]</TT> of new matrix.
@param b Fixpoint coefficient <TT>[0][1]</TT> of new matrix.
@param c Fixpoint coefficient <TT>[0][2]</TT> of new matrix.
@param d Fixpoint coefficient <TT>[1][0]</TT> of new matrix.
@param e Fixpoint coefficient <TT>[1][1]</TT> of new matrix.
@param f Fixpoint coefficient <TT>[1][2]</TT> of new matrix.
@param g Fixpoint coefficient <TT>[2][0]</TT> of new matrix.
@param h Fixpoint coefficient <TT>[2][1]</TT> of new matrix.
@param i Fixpoint coefficient <TT>[2][2]</TT> of new matrix.
@see eGML_Matrix3 */
#define _FX9(a,b,c,d,e,f,g,h,i)  eGML_Matrix3(eC_FixToValue(a), eC_FixToValue(b), eC_FixToValue(c),\
                                              eC_FixToValue(d), eC_FixToValue(e), eC_FixToValue(f),\
                                              eC_FixToValue(g), eC_FixToValue(h), eC_FixToValue(i))

//-----------------------------------------------------------------------------------------------
// primitive, type, variable

/** Short form of eGML_Primitive buffer modification block. This macro contains everything that is necessary for primitive buffer access.
<TT><B>Lock</B></TT>ing and <TT><B>Unlock</B></TT>ing for <I>read-only</I>-access to a buffer containing 2D- or 3D-positional coordinates [x, y (,z)]
are part of the macro. So an \Ref{eGML_Primitive} object has to be defined <I>before</I>. To realise buffer access a variable with a dedicated
type is required. You just need to assign the names of the type (T) and the variable (v) as parameters to the macro. The returned pointer of the
Lock operation inside of the macro will be casted to the type '<TT>T *</TT>'. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error the code
behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
Please do not use this <I>Little Helper</I> if you are not experienced in handling with primitives. At least you should study the
{@link Tutorial_eGML tutorial} first.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@see ReadPrim_T, ReadPrimLoop, eGML_Primitive */
#define ReadPrim(p,T,v)           for (T *v = (T*) p->Lock(eGML_Primitive::BT_COORD, eGML_Primitive::LF_READONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_COORD), v = NULL)

/** Short form of eGML_Primitive buffer modification block. This macro contains everything that is necessary for primitive buffer access.
<TT><B>Lock</B></TT>ing and <TT><B>Unlock</B></TT>ing for <I>write-only</I>-access to a buffer containing 2D- or 3D-positional coordinates [x, y (,z)]
are part of the macro. So an \Ref{eGML_Primitive} object has to be defined <I>before</I>. To realise buffer access a variable with a dedicated
type is required. You just need to assign the names of the type (T) and the variable (v) as parameters to the macro. The returned pointer of the
Lock operation inside of the macro will be casted to the type '<TT>T *</TT>'. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error the code
behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
Please do not use this <I>Little Helper</I> if you are not experienced in handling with primitives. At least you should study the
{@link Tutorial_eGML tutorial} first.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@see WritePrim_T, WritePrimLoop, eGML_Primitive */
#define WritePrim(p,T,v)          for (T *v = (T*) p->Lock(eGML_Primitive::BT_COORD, eGML_Primitive::LF_WRITEONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_COORD), v = NULL)

/** Short form of transformed eGML_Primitive buffer modification block. This macro contains everything that is necessary for primitive buffer access.
<TT><B>Lock</B></TT>ing and <TT><B>Unlock</B></TT>ing for <I>read-only</I>-access to a buffer containing transformed 3D-positional data [x, y ,z]
are part of the macro. So an \Ref{eGML_Primitive} object has to be defined <I>before</I>. To realise buffer access a variable with a dedicated
type is required. You just need to assign the names of the type (T) and the variable (v) as parameters to the macro. The returned pointer of the
Lock operation inside of the macro will be casted to the type '<TT>T *</TT>'. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error the code
behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
Please do not use this <I>Little Helper</I> if you are not experienced in handling with primitives. At least you should study the
{@link Tutorial_eGML tutorial} first.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@see ReadPrim, ReadPrimLoop_T, eGML_Primitive */
#define ReadPrim_T(p,T,v)         for (T *v = (T*) p->Lock(eGML_Primitive::BT_TRANSFORMED, eGML_Primitive::LF_READONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_TRANSFORMED), v = NULL)

/** Short form of transformed eGML_Primitive buffer modification block. This macro contains everything that is necessary for primitive buffer access.
<TT><B>Lock</B></TT>ing and <TT><B>Unlock</B></TT>ing for <I>write-only</I>-access to a buffer containing transformed 3D-positional data [x, y ,z]
are part of the macro. So an \Ref{eGML_Primitive} object has to be defined <I>before</I>. To realise buffer access a variable with a dedicated
type is required. You just need to assign the names of the type (T) and the variable (v) as parameters to the macro. The returned pointer of the
Lock operation inside of the macro will be casted to the type '<TT>T *</TT>'. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error the code
behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
Please do not use this <I>Little Helper</I> if you are not experienced in handling with primitives. At least you should study the
{@link Tutorial_eGML tutorial} first.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@see WritePrim, WritePrimLoop_T, eGML_Primitive */
#define WritePrim_T(p,T,v)        for (T *v = (T*) p->Lock(eGML_Primitive::BT_TRANSFORMED, eGML_Primitive::LF_WRITEONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_TRANSFORMED), v = NULL)


/** Short form of repeated eGML_Primitive buffer modification block. This macro expands the \Ref{ReadPrim} macro. Buffer access that would be done
in <TT><B>ReadPrim</B></TT> just once will be repeated in <TT><B>ReadPrimLoop</B></TT> a number of times defined by buffer size and storage type. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error
the code behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
To get detailed information about this <I>Little Helper</I>, refer to \Ref{ReadPrim} or to the dedicated {@link Tutorial_eGML tutorial}.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@param i Reserved.
@see ReadPrim, ReadPrimLoop_T, eGML_Primitive */
#define ReadPrimLoop(p,T,v,i)     for (T *v = (T*) p->Lock(eGML_Primitive::BT_COORD, eGML_Primitive::LF_READONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_COORD), v = NULL) for (eC_UInt i = 0; i < p->GetBasesize(); i++)

/** Short form of repeated eGML_Primitive buffer modification block. This macro expands the \Ref{WritePrim} macro. Buffer access that would be done
in <TT><B>WritePrim</B></TT> just once will be repeated in <TT><B>WritePrimLoop</B></TT> a number of times defined by buffer size and storage type. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error
the code behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
To get detailed information about this <I>Little Helper</I>, refer to \Ref{WritePrim} or to the dedicated {@link Tutorial_eGML tutorial}.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@param i Reserved.
@see WritePrim, WritePrimLoop_T, eGML_Primitive */
#define WritePrimLoop(p,T,v,i)    for (T *v = (T*) p->Lock(eGML_Primitive::BT_COORD, eGML_Primitive::LF_WRITEONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_COORD), v = NULL) for (eC_UInt i = 0; i < p->GetBasesize(); i++)

/** Short form of repeated transformed eGML_Primitive buffer modification block. This macro expands the \Ref{ReadPrim_T} macro.
Buffer access that would be done in <TT><B>ReadPrim_T</B></TT> just once will be repeated in <TT><B>ReadPrimLoop_T</B></TT> a number of times
defined by buffer size and storage type. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error
the code behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
To get detailed information about this <I>Little Helper</I>, refer to \Ref{ReadPrim_T} or to the dedicated {@link Tutorial_eGML tutorial}.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@param i Reserved.
@see ReadPrim_T, ReadPrimLoop, eGML_Primitive */
#define ReadPrimLoop_T(p,T,v,i)   for (T *v = (T*) p->Lock(eGML_Primitive::BT_TRANSFORMED, eGML_Primitive::LF_READONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_TRANSFORMED), v = NULL) for (eC_UInt i = 0; i < p->GetBasesize(); i++)

/** Short form of repeated transformed eGML_Primitive buffer modification block. This macro expands the \Ref{WritePrim_T} macro.
Buffer access that would be done in <TT><B>WritePrim_T</B></TT> just once will be repeated in <TT><B>WritePrimLoop_T</B></TT> a number of times
defined by buffer size and storage type. <BR>
Note: The parameter 'v' is defined within local scope only. The Lock operation at the beginning is checked for returning NULL so on error
the code behind the macro will not be executed. Unlock is called automatically at end of block but you are able to exit prematurely by using 'break'. <BR>
To get detailed information about this <I>Little Helper</I>, refer to \Ref{WritePrim_T} or to the dedicated {@link Tutorial_eGML tutorial}.
@param p Pointer to primitive object.
@param T Type of the varaible.
@param v Varaible to modify.
@param i Reserved.
@see WritePrim_T, WritePrimLoop, eGML_Primitive */
#define WritePrimLoop_T(p,T,v,i)  for (T *v = (T*) p->Lock(eGML_Primitive::BT_TRANSFORMED, eGML_Primitive::LF_WRITEONLY); (v != NULL); p->Unlock(eGML_Primitive::BT_TRANSFORMED), v = NULL) for (eC_UInt i = 0; i < p->GetBasesize(); i++)

//@}

// end of eGML_Little_Helpers

//#endif // DOCXX
//-----------------------------------------------------------------------------------------------


// notes for primitive macros:
//
// v is defined within local scope only. lock is checked for 0 return, block is not executed when
// lock failed. unlock is called automatically at end of block. premature exit by using 'break' is possible.
// lock does use optimised flags (read/write only). casting is done automatically.

#endif //eGML_USE_LITTLE_HELPERS


#ifdef eGML_USE_GLOBAL_FLAGS

namespace eGML_Flag
{
  using eGML::BM_RGB32;
  using eGML::BM_ALPHA8;
  using eGML::SL_DEFAULT;

  using eGML_Bitmap::BLIT_NOCLIP;
  using eGML_Bitmap::BLIT_BILINEAR;
  using eGML_Bitmap::BLIT_ASPECT;

  using eGML_Bitmap::PRIM_POINTS;
  using eGML_Bitmap::PRIM_LINES;
  using eGML_Bitmap::PRIM_FILLED;
  using eGML_Bitmap::PRIM_CLOSED;
  using eGML_Bitmap::PRIM_ROUND;
  using eGML_Bitmap::PRIM_INDEX;
  using eGML_Bitmap::PRIM_INDEXCOLOR;
  using eGML_Bitmap::PRIM_RHWPRESENT;
  using eGML_Bitmap::PRIM_3DWIDTH;
  using eGML_Bitmap::PRIM_NOCLIP;
  using eGML_Bitmap::PRIM_ANTIALIAS;
  using eGML_Bitmap::PRIM_HQAA;
  using eGML_Bitmap::PRIM_NOCULLING;

  using eGML_Bitmap::POLY_NONE;
  using eGML_Bitmap::POLY_ANTIALIAS;
  using eGML_Bitmap::POLY_HQAA;
  using eGML_Bitmap::POLY_NOCLIP;

  using eGML_Bitmap::LINE_NONE;
  using eGML_Bitmap::LINE_WIDE_S;
  using eGML_Bitmap::LINE_WIDE_T;
  using eGML_Bitmap::LINE_WIDE;
  using eGML_Bitmap::LINE_WIDE_2;
  using eGML_Bitmap::LINE_ANTIALIAS;
  using eGML_Bitmap::LINE_HQAA;
  using eGML_Bitmap::LINE_ROUND_START;
  using eGML_Bitmap::LINE_ROUND_END;
  using eGML_Bitmap::LINE_NOCLIP;

  using eGML_Bitmap::DOT_NONE;
  using eGML_Bitmap::DOT_ANTIALIAS;
  using eGML_Bitmap::DOT_HQAA;
  using eGML_Bitmap::DOT_NOCLIP;

  using eGML_Primitive::CLF_XY;
  using eGML_Primitive::CLF_Z;
  using eGML_Primitive::CLF_XYZ;
  using eGML_Primitive::ALF_COLOR;
  using eGML_Primitive::ALF_UV1;
  using eGML_Primitive::ALF_UV2;
  using eGML_Primitive::NLF_NORMAL;

  using eGML_Primitive::BT_COORD;
  using eGML_Primitive::BT_ATTRIB;
  using eGML_Primitive::BT_NORMAL;
  using eGML_Primitive::BT_TRANSFORMED;
  using eGML_Primitive::BT_INDEX;
  using eGML_Primitive::BT_INDEXATTRIB;
  using eGML_Primitive::BT_INDEXNORMAL;
  using eGML_Primitive::BT_QUANTITY;

  using eGML_Primitive::TF_NO_TRANSLATION;
  using eGML_Primitive::TF_TRANSLATION_ONLY;
  using eGML_Primitive::TF_SCALE_ONLY;
  using eGML_Primitive::TF_ASSIGN_DEPTH;

  using eGML_Primitive::LF_READWRITE;
  using eGML_Primitive::LF_READONLY;
  using eGML_Primitive::LF_WRITEONLY;

  using eGML_Primitive::SF_EMPTY;
  using eGML_Primitive::SF_FILLED;
  using eGML_Primitive::SF_LOCKED;
  using eGML_Primitive::SF_TRANSFORMED2D;
  using eGML_Primitive::SF_TRANSFORMED3D;
  using eGML_Primitive::SF_TRANSFORMED;
  using eGML_Primitive::SF_PROJECTED;

  using eGML_Primitive::IBLT_TRILIST;
  using eGML_Primitive::IBLT_LINELIST;
  using eGML_Primitive::IBLT_STRIPE;
  using eGML_Primitive::IBLT_FAN;

  using eGML_Primitive::IALF_COLOR;

  using eGML_RenderState::COLOR1;
  using eGML_RenderState::COLOR2;
  using eGML_RenderState::ALPHA;
  using eGML_RenderState::ALPHAENABLE;
  using eGML_RenderState::LINEWIDTH;
  using eGML_RenderState::POINTSIZE;
  using eGML_RenderState::BASEDEPTH;
  using eGML_RenderState::CULLING;
  using eGML_RenderState::TEXTUREENABLE;
  using eGML_RenderState::TEXTUREMODE;
  using eGML_RenderState::TEXORIGINX;
  using eGML_RenderState::TEXORIGINY;
  using eGML_RenderState::TEXORIGINZ;
  using eGML_RenderState::TEXUAXISX;
  using eGML_RenderState::TEXUAXISY;
  using eGML_RenderState::TEXUAXISZ;
  using eGML_RenderState::TEXVAXISX;
  using eGML_RenderState::TEXVAXISY;
  using eGML_RenderState::TEXVAXISZ;
  using eGML_RenderState::TEXTURE;
  using eGML_RenderState::LINEPATTERN;
  using eGML_RenderState::LINEPATTERNENABLE;
  using eGML_RenderState::LINEPATTERNSCALE;
  using eGML_RenderState::VERTEXCOLORENABLE;

  using eGML_RenderState::CULL_NONE;
  using eGML_RenderState::CULL_CW;
  using eGML_RenderState::CULL_CCW;

  using eGML_RenderState::TM_PLANE2D;
  using eGML_RenderState::TM_PLANE3D;
  using eGML_RenderState::TM_UV;
};


using namespace eGML_Flag;

#endif //eGML_USE_GLOBAL_FLAGS
//-----------------------------------------------------------------------------------------------
