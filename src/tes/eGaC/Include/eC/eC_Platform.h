/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Platform.h
* %version:        134.1 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:00:29 2006
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_PLATFORM__H_
#define EC_PLATFORM__H_
//
// Interface to platform specific functions
//
//-------------------------------------------------------------------------------------------
#ifdef DOCXX

/**@name Global_Platform_Defines
@memo
Compiler options for platform specific settings.
@doc
In this chapter you will find fundamental compiler options for correct application development with the <B>eGML</B> and components (<B>eGaC</B>).
It is necessary to set at least <I>two</I> compiler options to fit to the target system, further settings are optional:
<P>
<LI><TT><B>eC_TARGET_CPU_xxx</B></TT> - target CPU <I>obligatory!</I>
<LI><TT><B>eC_TARGET_ENV_xxx</B></TT> - target environment <I>obligatory!</I>
<LI><TT><B>eC_TARGET_GFX_xxx</B></TT> - target graphics interface <I>optional</I>
<LI><TT><B>eC_TARGET_COMPILER_xxx</B></TT> - used compiler <I>optional</I>
</P>
'<TT><B>xxx</B></TT>' has to be replaced by the appendix of your desired device/component (look at the chapters below). <BR>
To get an introduction how to start developing and how to build a project with <B>eGML</B>, please reference to the <B>eGML</B> {@link Tutorial_eGML tutorial}. <BR>
If you do not find the compiler option (or combinations with it) for your desired CPU, environment, graphics interface or compiler in this chapter,
please do not hesitate to send us an <A HREF=mailto:eGML@tesbv.com>email</A>. We will give you detailed information about possibilities to use the <B>eGML</B> and components on your target.
@see eC_Global_Defines, eGML_Global_Defines
@see Tutorial_eGML
*/

//@{

//-------------------------------------------------------------------------------------------
/** @name CPU
@memo
Platform CPU.
@doc
This compiler option <TT><B>eC_TARGET_CPU_xxx</B></TT> defines the target platform CPU. <BR>
To be sure that your source code will be compiled correctly for your target, you <I>must</I> set in any case this and the intended compiler option
for platform-{@link Environment environment} in your project environment first.
The macros for {@link Graphics graphics-interfaces} and {@link Compiler compilers} are optional and only have to be used if there is an <B>eGML</B>
version for them, so reference to their documentation. <BR>
In the following list you will find the CPU types the <B>eGaC</B> project is prepared for. Please choose exactly one of these.
@see Environment, Graphics, Compiler
*/

//@{

/** Platform CPU: <BR>
x86 based. */
#define eC_TARGET_CPU_X86

/** Platform CPU: <BR>
TriMedia. */
#define eC_TARGET_CPU_TM

/** Platform CPU: <BR>
MIPS architecture. */
#define eC_TARGET_CPU_MIPS

/** Platform CPU: <BR>
ARM-7 family. */
#define eC_TARGET_CPU_ARM7

/** Platform CPU: <BR>
SH-4 family. */
#define eC_TARGET_CPU_SH4

/** Platform CPU: <BR>
XScale low power architecture. */
#define eC_TARGET_CPU_XSCALE

/** Platform CPU: <BR>
SPARC architecture. */
#define eC_TARGET_CPU_SPARC

/** Platform CPU: <BR>
68K architecture. */
#define eC_TARGET_CPU_m68k

//@}


//-------------------------------------------------------------------------------------------
/** @name Render control
@memo
Defines to disable explicit functionality.
@doc
To get a lower footprint; unused functionality can be disabled.<BR><BR>
<B>General defines </B><BR><BR> 
<LI>eGML_PRIMITIVE_ALL_DISABLE
<LI>eGML_LINEPATTERN_DISABLE
<LI>eGML_POLYGONTEXTURE_DISABLE
<LI>eGML_POLYGONALPHA_DISABLE
<BR><BR> 
<B>eGML_BitmapA8 specific defines </B><BR><BR> 
<LI>eGML_ALPHA8_POLYGON_DISABLE
<LI>eGML_ALPHA8_LINE_DISABLE
<LI>eGML_ALPHA8_DOT_DISABLE
<LI>eGML_ALPHA8_BLIT_DISABLE
<LI>eGML_ALPHA8_BLITALPHA_DISABLE
<LI>eGML_ALPHA8_BLITCOLOR_DISABLE
<LI>eGML_ALPHA8_STRETCHBLIT_DISABLE
<LI>eGML_ALPHA8_STRETCHBLITALPHA_DISABLE
<LI>eGML_ALPHA8_STRETCHBLITCOLOR_DISABLE
<LI>eGML_ALPHA8_ROTBLIT_DISABLE
<LI>eGML_ALPHA8_ROTBLITALPHA_DISABLE
<LI>eGML_ALPHA8_ROTBLITCOLOR_DISABLE
<BR><BR> 
<B>eGML_BitmapRGB32 specific defines </B><BR><BR> 
<LI>eGML_RGB32_POLYGON_DISABLE
<LI>eGML_RGB32_LINE_DISABLE
<LI>eGML_RGB32_DOT_DISABLE
<LI>eGML_RGB32_BLIT_DISABLE
<LI>eGML_RGB32_BLITALPHA_DISABLE
<LI>eGML_RGB32_BLITCOLOR_DISABLE
<LI>eGML_RGB32_STRETCHBLIT_DISABLE
<LI>eGML_RGB32_STRETCHBLITALPHA_DISABLE
<LI>eGML_RGB32_STRETCHBLITCOLOR_DISABLE
<LI>eGML_RGB32_ROTBLIT_DISABLE
<LI>eGML_RGB32_ROTBLITALPHA_DISABLE
<LI>eGML_RGB32_ROTBLITCOLOR_DISABLE
*/


//-------------------------------------------------------------------------------------------
/** @name Environment
@memo
Platform environment.
@doc
This compiler option <TT><B>eC_TARGET_ENV_xxx</B></TT> defines the target platform environment (operating system and/or complex hardware constellation). <BR>
To be sure that your source code will be compiled correctly for your target, you <I>must</I> set in any case this and the intended compiler option
for platform-\Ref{CPU} in your project environment first.
The macros for {@link Graphics graphics-interfaces} and {@link Compiler compilers} are optional and only have to be used if there is an <B>eGML</B>
version for them, so reference to their documentation. <BR>
In the following list you will find the environment types the <B>eGaC</B> project is prepared for. Please choose exactly one of these.
@see CPU, Graphics, Compiler
*/

//@{

/** Platform environment: <BR>
Windows 32 bit operating system. Must be defined for:
<LI>Windows 95
<LI>Windows 2000
<LI>Windows XP
*/
#define eC_TARGET_ENV_WIN32

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
TriMedia Navi02 system by VDO. */
#define eC_TARGET_ENV_NAVIO2

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
Roadrunner system by VDO. */
#define eC_TARGET_ENV_RR

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
Crusader system by Siemens VDO. */
#define eC_TARGET_ENV_CRUSADER

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
Game Boy Advanced (GBA). */
#define eC_TARGET_ENV_GBA

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
SH-4/TriMedia multiprocessor system. */
#define eC_TARGET_ENV_SH4TM

/** Platform environment: <BR>
Pocket PC. Must be defined for the most Windows CE versions. */
#define eC_TARGET_ENV_POCKET

/** Platform environment: <BR>
Solaris platform / operating system. */
#define eC_TARGET_ENV_SOLARIS

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
Amiga with CBM-bus. */
#define eC_TARGET_ENV_CBM_AMIGA

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
eLinux/Mips based  MMI-Starterkit from Ultratronik*/
#define eC_TARGET_ENV_UTST 

/* !!! INTERNAL USE ONLY !!!
Platform environment: <BR>
Toshiba/CapricornII*/
#define eC_TARGET_ENV_CAPI

//@}

//-------------------------------------------------------------------------------------------
/** @name Graphics
@memo
Platform graphics interface.
@doc
This compiler option <TT><B>eC_TARGET_GFX_xxx</B></TT> defines the target platform graphics interface (graphics library and/or -card). <BR>
To be sure that your source code will be compiled correctly for your target, you <I>must</I> set in any case the intended compiler options for
platform-\Ref{CPU} and -{@link Environment environment} in your project environment first.
If you want to use one of the following <I>documented</I> graphics interfaces or one of the <I>documented</I> {@link Compiler compilers},
you also <I>must</I> set the intended precompiler options, but if you want to use other graphics interfaces or compilers you <I>need not</I> to
set a precompiler option. <BR>
In the following list you will find the graphic interface types the <B>eGaC</B> project is prepared for. Please choose exactly one of these.
@see CPU, Environment, Compiler
*/

//@{

/** Platform graphics interface: <BR>
Simple DirectMedia Layer (SDL) multimedia library. */
#define eC_TARGET_GFX_SDL

/** Platform graphics interface: <BR>
Windows Graphics Device Interface (GDI). */
#define eC_TARGET_GFX_GDI

/** Platform graphics interface: <BR>
Fujitsu 'Coral P' Graphic Display Controller (GDC). */
#define eC_TARGET_GFX_FUJITSUGDC

/** Platform graphics interface: <BR>
Siemens Graphics Support Unit (GSU). */
#define eC_TARGET_GFX_SIEMENSGSU

/* !!! INTERNAL USE ONLY !!!
Platform graphics interface: <BR>
Amiga Enhanced Chip Set (ECS). */
#define eC_TARGET_GFX_ECS

/* !!! INTERNAL USE ONLY !!!
Platform graphics interface: <BR>
Amiga Enhanced Graphics System (EGS) GUI. */
#define eC_TARGET_GFX_EGS_RII

//@}

//-------------------------------------------------------------------------------------------
/** @name Compiler
@memo
Platform compiler.
@doc
This compiler option <TT><B>eC_TARGET_COMPILER_xxx</B></TT> defines the target platform compiler (family). <BR>
To be sure that your source code will be compiled correctly for your target, you <I>must</I> set in any case the intended compiler options for
platform-\Ref{CPU}, -{@link Environment environment} in your project environment first.
If you want to use one of the following <I>documented</I> compilers or one of the <I>documented</I> {@link Graphics graphics-interfaces},
you also <I>must</I> set the intended precompiler options, but if you want to use other graphics interfaces or compilers you <I>need not</I> to
set a precompiler option. <BR>
In the following list you will find the compiler (families) the <B>eGaC</B> project is prepared for. Please choose exactly one of these.
@see CPU, Environment, Graphics
*/

//@{

/** Platform compiler: <BR>
GNU Compiler Collection (GCC) family. */
#define eC_TARGET_COMPILER_GCC

/** Platform compiler: <BR>
General cross compiler (XCC). */
#define eC_TARGET_COMPILER_XCC

/** Platform compiler: <BR>
TriMedia Compiler (TMC) family. */
#define eC_TARGET_COMPILER_TMC

/** Platform compiler: <BR>
Microsoft Visual C/C++ (MSVC). */
#define eC_TARGET_COMPILER_MSVC

//@}

//-------------------------------------------------------------------------------------------

//@}
// end of Global_Platform_Defines.

#endif // DOCXX

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_CPU_X86
#ifdef eC_TARGET_ENV_WIN32
#include "x86_win32/eC_x86_win32_Platform.h"
#endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_CPU_TM
#ifdef eC_TARGET_ENV_NAVIO2
#include "tm_navi/eC_tm_navi_Platform.h"
#endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_CPU_MIPS
#ifdef eC_TARGET_ENV_RR
#include "mips_rr/eC_mips_rr_Platform.h"
#endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_CPU_MIPS
#ifdef eC_TARGET_ENV_CRUSADER
#include "mips_crusader/eC_mips_crusader_Platform.h"
#endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_CPU_XSCALE
#ifdef eC_TARGET_ENV_POCKET
#include "pocket/eC_pocket_Platform.h"
#endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_UTST
#include "utst/eC_utst_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_LINUX
#include "linux/eC_linux_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_ELINUX
#include "elinux/eC_elinux_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_OSE
#include "ose/eC_ose_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_NIOS
#include "nios/eC_nios_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_CAPI
#include "capi/eC_capi_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_OMAP331
#include "omap331/eC_omap331_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_VXWORKS
#include "vxWorks/eC_vXWorks_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_EMBOS
#include "embOS/eC_embOS_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_RSK2_SH7264
#include "rsk2_sh7264/eC_rsk2_sh7264_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_SAM4S_EK2
#include "SAM4S-EK2/eC_SAM4S-EK2_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_RX63N_NO_OS
#include "rx63n/no_os/eC_RX63N_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_RX63N_FREERTOS
#include "rx63n/freertos/eC_RX63N_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_DCX
#include "dcx/eC_dcx_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_MAGIKMI
#include "imx27_elinux/eC_imx27_elinux_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_SC14450
#include "linux/eC_linux_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_HICO7723
#include "linux/eC_linux_Platform.h"
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_MS7724
#include "linux/eC_linux_Platform.h"
#endif


//-------------------------------------------------------------------------------------------
#if defined(eC_TARGET_ENV_RZA1H)
  #if  defined(eC_TARGET_ENV_EMBOS)
    #include "RZA1H_Embos/eC_RZA1H_Platform_embOS.h"
  #elif defined(eC_TARGET_ENV_FREERTOS)
    #include "FreeRTOS/eC_FreeRTOS_Platform.h"
  #elif defined(eC_TARGET_ENV_MBED)
    #include "mbed/eC_mbed_Platform.h"
  #else
    #include "RZA1H/eC_RZA1H_Platform.h"
  #endif
#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_STM32F429
#include "stm32f429/eC_stm32f429_Platform.h"
#endif
#endif
