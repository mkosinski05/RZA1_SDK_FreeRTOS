/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Config.h
* %version:        134.1 %
*
* author:          hh04031
* creation date:   Thu Aug 11 10:31:40 2005
* description:

* %date_modified:  % (%derived_by: hh04019 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_CONFIG__H_
#define EGML_CONFIG__H_

//-----------------------------------------------------------------------------------------------------------------
#ifdef DOCXX

/**@name eGML_Global_Defines
@memo
Macros for <B>eGML</B> specific settings.
@doc
These 'define macros' adjust general settings and the operation mode of the <B>eGML</B> at <I>compiletime</I>.
If you just want to use predefined dafault settings you need not to do anything - all macros are already set in the headerfiles
<I>eGML_Config.h</I> and <I>eGML_Platform.h</I> ("<I>$(eGML_ROOT)/comps/eGML/code</I>" folder).
But if you want to use special/project-specific settings, you have to use the \Ref{eGML_USE_OWN_CONFIG} precompiler option and
you have define your own settings inside of <I>eGML_User_Config.h</I> file ("<I>$(eGML_ROOT)/comps/eGML/code</I>" folder).
@see
Global_Platform_Defines
*/
//@{

/** Indicator for using an user defined configuration file. This file must have got the name <I>eGML_User_Config.h</I> and should
contain settings for all macros listed below. */
//#define eGML_USE_OWN_CONFIG

/* !!! INTERNAL USE ONLY !!!
Default gamma factor for target display. Used in antialiasing calculation only.
@see <A HREF=antialiasing/Antialiasing.html>Antialiasing</A> */
#define eGML_TARGETGAMMA

/** Default subdivision step for perspective correction when drawing textures.
@see eGML_Texture */
#define eGML_TEXTURESTEP

/** Filtered (high quality) antialiasing. If defined support for filtered (high quality) antialiasing is enabled.
@see <A HREF=antialiasing/Antialiasing.html>Antialiasing</A> */
#define eGML_FILTEREDAA

/** Width variation along wide lines. If defined support for width variation along wide lines is enabled.
@see eGML_Pattern */
#define eGML_VARIWIDTH

/** Rasterisastion modification to prefer loops instead of multiplication.
If defined rasterisastion modification is enabled to prefer loops over multiplication.
@see eC_Math */
#define eGML_REDUCEMUL

/** Gamma correction when antialiasing. If defined gammacorrection will be applied when antialiasing.
@see <A HREF=antialiasing/Antialiasing.html>Antialiasing</A> */
#define eGML_GAMMACORRECTAA

/** Floating point for perspective correct texture loop. If defined floating point values will be used for perspective correct texture loop.
@see eGML_Texture */
#define eGML_TEXTUREFLOAT

/** Support basic bitmap RGB32 format. If defined support for basic bitmap RGB32 format is enabled.
@see eGML_Bitmap */
#define eGML_USEBITMAPRGB32

/** Support basic bitmap ARGB32 format. If defined support for basic bitmap ARGB32 format is enabled.
@see eGML_Bitmap */
#define eGML_USEBITMAPARGB32

/** Support basic bitmap Alpha8 format. If defined support for basic bitmap Alpha8 format is enabled.
@see eGML_Bitmap */
#define eGML_USEBITMAPA8

/** Activates eMem component. If defined the eMem component is enabled, so that eMem will manage all memory allocations by <B>eGML</B>.
@see Component_eMem */
#define eGML_USEEMEM

/** Enables little helpers. If defined additional macros will simplify mathematic operations under <B>eGML</B>.
@see eGML_Little_Helpers, eC_Math */
#define eGML_USE_LITTLE_HELPERS

/** Enables the used of semaphores in eGML::Lock/Unlock methods to solve the mutual exclusion problem.
*/
#define eGML_THREADSAFE

/** Defines the line width threshold value, for drawing round line ends. Below this threshold
no dots will be drawn.
*/
#define eGML_ROUND_LINEEND_THRESHOLD

/** divides thicklines into seperate lineparts, to accelerate lineclippings by using no clipping-test for the majorpart of
a thickline and preciese clipping for the clipped-line part on a clip-border
*/
#define eGML_THICKLINE_ACCELEARTION

/** Use the thicklines methodes instead of 4px wide thinlines
*/
#define eGML_USE_HQ_THICKLINES


/** at clipped thicklines, the original code redirected HQ_AA-SpanRender2Edge calls to AA-SpanRender2Edge calls for performance reasons.
* the intentions was, that these visual errors (in an avg. 10px margin) won't be seen anyway
*/
#define eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA

//@Include: ../../comps/eGML/code/eGML_Platform.h

//@}

#endif

//-----------------------------------------------------------------------------------------------------------------
#define eGML_TARGETGAMMA    2.2
#define eGML_TEXTURESTEP    8

#include "eC.h"

//-----------------------------------------------------------------------------------------------------------------
#define eGML_VERSION    160
// Versionhistory
// V1.60: +ARM NEON Blit optimization
// V1.50: generic Version used up to 2017

#ifdef eGML_USE_OWN_CONFIG

#ifdef eGML_CONFIG_FILE
#include "eGML_User_Config.h"
#endif //eGML_CONFIG_FILE

#else

#define eGML_TOGGLE_NEON_SUPPORT

#define eGML_FILTEREDAA             // support for filtered (hq) antialiasing
#define eGML_VARIWIDTH              // support width variation along wide lines.
//#define eGML_REDUCEMUL              // enable rasterisastion modification to prefer loops instead of multiplication
//#define eGML_GAMMACORRECTAA         // enable gammacorrection when antialiasing is applied
#define eGML_TEXTUREFLOAT           // enable floating point values to perspective correct textures
#define eGML_USEBITMAPRGB32RLE16    // enable support for basic bitmap rgb32 RLE16
#define eGML_USEBITMAPRGB32RLE8     // enable support for basic bitmap rgb32 RLE8
#define eGML_USEBITMAPARGB32        // enable support for basic bitmap argb32
#define eGML_USEBITMAPA8            // enable support for basic bitmap alpha8
#define eGML_USEBITMAPRGB565        // enable support for basic bitmap rgb565
#if !(defined eC_TARGET_ENV_FREERTOS) && !(defined eC_TARGET_ENV_MBED)
#define eGML_USEBITMAPRGB32         // enable support for basic bitmap rgb32
#define eGML_USEBITMAPRGB555        // enable support for basic bitmap rgb555
#define eGML_USEBITMAPA1            // enable support for basic bitmap alpha1 (1bpp monochrome / alpha)
#define eGML_USEBITMAPA5_2          // enable support for 5 bit alpha, 2 bits for flags
#endif //  !(defined eC_TARGET_ENV_FREERTOS) && !(defined eC_TARGET_ENV_MBED)

#ifdef eC_TARGET_ENV_POCKET
#define eGML_USEBITMAP_POCKET565    // enable support for native pocket bitmap RGB565
//#define eGML_USEBITMAP_POCKET555    // enable support for native pocket bitmap RGB555
#endif
//#define eGML_USEEMEM                // activates eMem component
//#define eGML_USE_LITTLE_HELPERS     // additional macros will simplify mathematic operations
//#define eGML_TEXTURE_USE_ALPHA      // enables support for textures with alpha channel

// Special options to adapt pocket binary code size
#ifdef eC_TARGET_ENV_POCKET

//Enable/Disable polygons
//#define eGML_POLYGONTEXTURE_DISABLE
//#define eGML_POLYGONALPHA_DISABLE

#define eGML_ALPHA8_POLYGON_DISABLE
#define eGML_ALPHA8_LINE_DISABLE
#define eGML_ALPHA8_DOT_DISABLE

//#define eGML_ALPHA8_BLIT_DISABLE
#define eGML_ALPHA8_BLITALPHA_DISABLE
#define eGML_ALPHA8_BLITCOLOR_DISABLE

//#define eGML_ALPHA8_STRETCHBLIT_DISABLE
#define eGML_ALPHA8_STRETCHBLITALPHA_DISABLE
#define eGML_ALPHA8_STRETCHBLITCOLOR_DISABLE

#define eGML_ALPHA8_ROTBLIT_DISABLE
#define eGML_ALPHA8_ROTBLITALPHA_DISABLE
#define eGML_ALPHA8_ROTBLITCOLOR_DISABLE


//#define eGML_RGB32_POLYGON_DISABLE
//#define eGML_RGB32_LINE_DISABLE
//#define eGML_RGB32_DOT_DISABLE

//#define eGML_RGB32_BLIT_DISABLE
//#define eGML_RGB32_BLITALPHA_DISABLE
//#define eGML_RGB32_BLITCOLOR_DISABLE

//#define eGML_RGB32_STRETCHBLIT_DISABLE
//#define eGML_RGB32_STRETCHBLITALPHA_DISABLE
//#define eGML_RGB32_STRETCHBLITCOLOR_DISABLE

//#define eGML_RGB32_ROTBLIT_DISABLE
//#define eGML_RGB32_ROTBLITALPHA_DISABLE
//#define eGML_RGB32_ROTBLITCOLOR_DISABLE
#endif // eC_TARGET_ENV_POCKET

#endif //eGML_USE_OWN_CONFIG

//Be sure default is set to valid value
#ifndef eGML_TARGETGAMMA
#define eGML_TARGETGAMMA    2.2
#endif

//Be sure default is set to valid value
#ifndef eGML_TEXTURESTEP
#define eGML_TEXTURESTEP    8
#endif

//Be sure default is set to valid value
#ifndef eGML_ROUND_LINEEND_THRESHOLD
#define eGML_ROUND_LINEEND_THRESHOLD    3.0
#endif

// shoud be used, if HQ_AA was used before this bugfix and the visual appearance of the application changes dramaticly
// but HQ_AA 1px wide lines are much slower than HQ_AA 1px thin lines, that is why we used 1px thinslines instead.
// -> please uncomment the next lines, if you want to use 1px widelines (like before) instead of 1px thinlines
//#ifndef eGML_USE_HQ_THICKLINES
//#define eGML_USE_HQ_THICKLINES
//#endif

// use fast (but ugly) thicklines at the screen-border. (about 5%-10% average speed increase for those lines)
// (at the screenedge, thicklines were divided in multiple sections: an unclipped section and a clipped section to improve performance by avoiding clipcalculations for the main part.
//  Unfortunatly, the transition between those two parts is visible and produces visual artifacts, so it looks better to uses one linedraw-methode for the whole line)
// -> please uncomment the next lines, if you want to use fast (but ugly) clipped lines
//#ifndef eGML_THICKLINE_ACCELEARTION
//#define eGML_THICKLINE_ACCELEARTION
//#endif

// redirects HQ_AA SpanEdges to AA_Spanedges for perfomance optimizations for clipped lines, which
// are seperated in an unclipped main segement and an edge segement
// since there is always an overlapping region between the two segements, it actually doesn't matter, if we
// use the same AA-modes - simple AA is much faster. It looks ugly anyways, because the AA-pixels at the transition
// are blended together, which produces a little bump at the overlappregion of the two linesegements
// -> please comment the next lines, if you want to use 4px for the unclipped segements AND clipped lines segements
#ifndef eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA
#define eGML_REDIRECT_HQ_AA_TO_SIMPLE_AA
#endif

//---------------------------------------------------------------
#include "eC_Math.h"
#include "eC_Memory.h"
#include "eDBG.h"
#include "eGML_Statistic.h"

#ifdef eGML_USEEMEM
#include "eMem.h"
#endif

//---------------------------------------------------------------

class eGML_Screen;
class eGML_Renderer;
class eGML_Bitmap;

//---------------------------------------------------------------

/** Abstract color type.
Internal structure known to <B>eGML</B> only, to create values of this type you use <TT><B>ColorFromRGB</B></TT> functions of \Ref{eGML_Bitmap}.
Please note that eGML_Color values are bitmap specific (the internal representation might be YUV instead of RGB if this
is what the underlying hardware uses - to avoid colorspace conversions at every function call <B>eGML</B> uses this abstraction).
Within the <B>eGML</B> you will also find eC_UInt or eGML_RGB32 typed values to be used as color parameters. <BR>
In this case the structure does <I>not</I> depend on the bitmap but is {@link eGML_RGB32 RGB32}.
@see eGML_Bitmap, eGML_RGB32 */
typedef eC_UInt eGML_Color;

/** RGB32 color type. Bits [31-24] must zero , [23-16] Red, [15-8] Green, [7-0] Blue, e. g.: <BR>
black-<TT>0x000000</TT>, white-<TT>0xffffff</TT>, red-<TT>0xff0000</TT>, green-<TT>0x00ff00</TT>, blue-<TT>0x0000ff</TT>).
@see eGML_Color */
typedef eC_UInt eGML_RGB32;

/** RGB32 color type. Bits [31-24] must zero , [23-16] Red, [15-8] Green, [7-0] Blue, e. g.: <BR>
black-<TT>0x000000</TT>, white-<TT>0xffffff</TT>, red-<TT>0xff0000</TT>, green-<TT>0x00ff00</TT>, blue-<TT>0x0000ff</TT>).
@see eGML_Color */
typedef eC_UInt eGML_ColorARGB32;
//---------------------------------------------------------------
/**
@memo
Singleton <B>eGML</B> baseclass.
@doc
During runtime there is always exactly one instance of <B>eGML</B> or one of its child classes. Derivations are used in case specialisation for
a certain platform is required. Static methods provide access to global Initialisation and Shutdown. The <B>eGML</B> object is obtained by
initialising the system and provides access to the factory methods necessary to create any other <B>eGML</B> object. So it is recommended
to start an application with the <B>eGML</B> initialisation (see \Ref{Init}) and to end with a \Ref{Exit} call. <BR>
To get an idea how simple a basic application could look like, observe the {@link Tutorial_eGML tutorial} of <B>eGML</B>. <BR>
If you would like to learn something about components used by <B>eGML</B> just have look {@link Components here}. <BR>
@see Components
@see Tutorial_eGML */
class eGML
{
public:

	/** Bitmap Buffer Types
	These enum specified the types of bitmap buffer types
	*/
	enum BitmapBufferType
	{
		/// Internal used only
		BBT_EGML         = 0,
		/// External system-ram-data
		BBT_RAM,
		/// External rom-data
		BBT_ROM,
		// !!! INTERNAL USE ONLY !!! just for dimensioning array
		BBT_QUANTITY
	};

	/** Basic bitmap types. <BR>
	These formats are supported on all platforms, conversion will happen if one is not a native format on target hardware. */
	enum BitmapType
	{
		/* !!! INTERNAL USE ONLY !!!
		bitmap type is unknown. Bitmaps can not be created with type BM_UNKNWON */
		BM_UNKNWON,
		/// Bitmap is 32bit per pixel, RGB (highest 8bit unsused).
		BM_RGB32,
      /// Bitmap is 32bit per pixel, 8 Bit RLE encoded (Readonly!)
		BM_RGB32RLE8,
      /// Bitmap is 32bit per pixel, 16 Bit RLE encoded (Readonly!)
        BM_RGB32RLE16,
		/// Bitmap is 32bit per pixel, ARGB.
		BM_ARGB32,
		/// Bitmap is 8bit per pixel alpha/greyscale only.
		BM_ALPHA8,
		/// Bitmap is 16bit per pixel.
		BM_RGB565,
		/// Bitmap is 16bit per pixel.
		BM_RGB555,
		/// Bitmap is 5bit alpha (highest), 2Bit Flag, 1 (lowest) dummy Bit for 7Bit compression
		BM_ALPHA5_2,
		/// Bitmap is 1bit per pixel alpha or black/white image.
		BM_ALPHA1,
    // !!! INTERNAL USE ONLY !!! just for dimensioning array
		BM_QUANTITY
	};

	/** Display/device to open screen on. If the hardware supports rendering to multiple outputs
	(overlays or physical devices) you can select which one to use for this screen. */
	enum ScreenLayer
	{
		/** Open on the default display. This setting is guaranteed to work on all platforms.
		On Windows for example 'default' would open a screen using the monitor/device the active desktop is on. */
		SL_DEFAULT,

		SL_VIRTUAL,
		// !!! INTERNAL USE !!! just for defining array size
		SL_QUANTITY
	};

	// types for flexible plugin renderer management
	typedef eGML_Renderer * (*eGML_RenderFactory)(eGML_Bitmap *);

	struct RenderEntry
	{
		eC_UInt             uiRenderClassID;
		eGML_RenderFactory  pkRenderFactory;
		RenderEntry        *pkNext;
	};


#ifdef eGML_USEEMEM
	/** Does the global initialisation of <B>eGML</B>. Heap management will alloc its initial working chunk from systemmemory, constant
	lookuptables will be precalculated and the debugsystem is initialised if necessary. You should consider this function to be quite
	slow as it might have to do a whole lot more (platform specific).
	@param rkInitParameter Initial {@link Component_eMem eMem} configuration.
	@return Pointer to a system dependent instance of <B>eGML</B>.
	@see Exit, Component_eMem */
	static eGML *Init(eMem_InitParameter *rkInitParameter=0);
#else
	// !!! INTERNAL USE ONLY !!!
	static eGML *Init();
#endif


	enum eGML_LockFlags
	{
		LF_DRAW   = 1,
		LF_CREATE = 2,
		LF_ALL    = 3
	};

#ifdef eGML_USEEMEM
	/** Lock eGML for special activity. To call lock is only necessary in multiprocess enviroments.
	@param ulLockFlag (see \Ref(eGML_LockFlags))
	@param pkHeap optional heap used for object creation
	*/
	static void Lock( eC_Flag ulLockFlag, eMem_Heap *pkHeap=0);

	/** Provides an instance of a heap. This <TT><B>eMem_Heap</B></TT> performs advanced memory management functionality for the <B>eGML</B>
	if enabled (see \Ref{eGML_USEEMEM} for more information).
	@return Pointer to special heap.
	@see Component_eMem */
	static eMem_Heap *GetHeap();

	/** Provides an instance of the heap used for all eGML internal allocation. This <TT><B>eMem_Heap</B></TT> performs advanced memory management functionality for the <B>eGML</B>
	if enabled (see \Ref{eGML_USEEMEM} for more information).
	@return Pointer to special heap.
	@see Component_eMem */
	static eMem_Heap *GetInternalHeap();
#else
	static void Lock( eC_Flag ulLockFlag );
#endif

	/** Unlock eGML.
	@param ulLockFlag (see \Ref(eGML_LockFlags))
	*/
	static void Unlock( eC_Flag ulLockFlag );


	/** Shuts down <B>eGML</B> and frees global resources. Bitmaps and buffers created and managed by <B>eGML</B> should be deleted first
	since they are unusable after calling Exit.
	@see Init */
	static void Exit();

	/** Initialises the rendering device. Therefor this method chooses a videomode and creates a screen object used to access the display.
	Please note that the videomode (uiWidth, uiHeight and uiViewDepth) is specified independently from the buffer organisation.
	You can e.g. use a 32 bit RGB buffer in any videomode, <B>eGML</B> will automatically apply the necessary conversions if the hardware does not have
	native support for this combination. If on the other hand the platform is known to the application it can directly request a native
	buffer organisation like 16 bit RGB or 24 bit YUV. This will make direct pixel manipulation more difficult but guarantees the best performance.
	The basic set of possible buffer formats you can find in \Ref{BitmapType}, additional (native) formats are defined by their specialised
	bitmap classes. \Ref{eGML_Screen}s serve as factory to create additional \Ref{eGML_Bitmap} objects.
	Maximum performance while blitting bitmaps can be guaranteed only if all involved bitmaps are created from the same screen.
	@param eScreenLayer The \Ref{ScreenLayer} that specifies which physical screen or layer to use (if hardware has more than one).
	@param eBitmapType The \Ref{BitmapType} that specifies internal organisation of display memory. If the hardware can not handle the requested format, conversion will take place during display.
	@param uiWidth Visible screen width in pixels.
	@param uiHeight Visible screen height in pixels.
	@param uiBuffers Number of display buffers to allocate, 2 equals to double buffering.
	@param uiViewDepth Optional display bit depth, pass 0 to use eBitmapType bitdepth. In case of a converted format an override can be usefull.
	@return Pointer to screen object or NULL if creation failed.
	@see eGML_Screen, eGML_Bitmap */
	virtual eGML_Screen* CreateScreen(eC_Enum eScreenLayer, eC_Enum eBitmapType, eC_UInt uiWidth, eC_UInt uiHeight,
		eC_UInt uiBuffers   , eC_UInt uiViewDepth = 0);

	/** Initialises a virtual render device.
	@param eBitmapType The \Ref{BitmapType} that specifies internal organisation of display memory. If the hardware can not handle the requested format, conversion will take place during display.
	@param uiWidth Visible screen width in pixels.
	@param uiHeight Visible screen height in pixels.
	@param uiBuffers Number of display buffers to allocate, 2 equals to double buffering.
	@param ppBitmapBuffer Pointer to an array which stores bitmap buffer pointer for each internal buffer.
	@param ubBitmapBufferType \Ref{BitmapBufferType}
	@return Pointer to screen object or NULL if creation failed.
	@see eGML_Screen, eGML_Bitmap */
	virtual eGML_Screen* CreateScreen(eC_Enum eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UInt uiBuffers, void **ppBitmapBuffer, eC_UByte ubBitmapBufferType);

	/* !!! INTERNAL USE ONLY !!!
	@return Error code. */
	eC_UInt GetErrorCode();

	/** Polls the underlying operating system for user requested termination of the running application. Naturally this function is used
	only in testbeds and only if the rendering system recieves user input (typically the case in windowed rendering on PC).
	@return Only if you terminate the application <B>true</B> will be returned.
	@see RequestTermination */
	virtual bool CheckAbortFlag();

	/** Signals an exit condition. If the host system does <I>not</I> recieve user input and \Ref{CheckAbortFlag} is used for termination
	checkup in your testcode, you can force CheckAbortFlag to return <B>true</B> next time, if you call this function before.
	@see CheckAbortFlag */
	void RequestTermination();

	/** Creates a rendering interface for the target Bitmap.
	@param eRenderClass
	@param pkTarget
	@return Pointer to renderer or NULL if creation failed.
	*/
	virtual eGML_Renderer *CreateRenderer(eC_UInt uiRenderClassID, eGML_Bitmap *pkTarget);

	//
	virtual void *AllocBitmapBuffer( eC_UInt uiBitmapType, eC_UByte ubBitmapBufferType, eC_UInt uiWidth, eC_UInt uiHeight );

	//
	virtual void FreeBitmapBuffer( void *pBitmapBuffer, eC_UByte ubBitmapBufferType);


	/** Provides the last keycode recieved from the host system. Usually implemented for test environments only.
	The returncode is splitted into two parts: <BR>
	1. The lower byte (bits 0 -  7) contains an ASCII code and is used for alphanumeric inputs. <BR>
	2. The upper byte (bits 8 - 31) represents the current state of several buttons and special keys. <BR>
	Each button and special key has its own distinct bit, this way multiple concurrent keypresses can be detected.
	The alphanumeric input (ASCII code part of the return value) delivers the most recent keycode since the last call of GetKey.
	@return Only if no key was pressed '0' will be returned, else ASCII code of key <I>and</I> state of virtual buttons or special keys.
	@see eGML_KeyCodes, GetMouse */
	virtual eC_UInt GetKey();

	/** Virtual keys. Since most embedded platforms do not have a full alphanumeric keyboard a fixed set of virtual buttons and direction
	keys are provided. The mapping to real keys is platform dependent.
	@see GetKey */
	enum eGML_KeyCodes
	{
		/// Direction key: up.
		KC_UP      = 0x100,
		/// Direction key: down.
		KC_DOWN    = 0x200,
		/// Direction key: left.
		KC_LEFT    = 0x400,
		/// Direction key: right.
		KC_RIGHT   = 0x800,
		/// Select button associated with direction keys.
		KC_SELECT  = 0x1000,
		/// Select button associated with 2nd pair of direction keys.
		KC_SELECT2 = 0x2000,
		/// Direction key (2nd pair): plus.
		KC_PLUS    = 0x4000,
		/// Direction key (2nd pair): minus.
		KC_MINUS   = 0x8000,
		/// Virtual button 1. Belongs to 1st set of virtual buttons.
		KC_BUTTON1 = 0x010000,
		/// Virtual button 2. Belongs to 1st set of virtual buttons.
		KC_BUTTON2 = 0x020000,
		/// Virtual button 3. Belongs to 1st set of virtual buttons.
		KC_BUTTON3 = 0x040000,
		/// Virtual button 4. Belongs to 1st set of virtual buttons.
		KC_BUTTON4 = 0x080000,
		/// Virtual button 5. Belongs to 2nd set of virtual buttons.
		KC_BUTTON5 = 0x100000,
		/// Virtual button 6. Belongs to 2nd set of virtual buttons.
		KC_BUTTON6 = 0x200000,
		/// Virtual button 7. Belongs to 2nd set of virtual buttons.
		KC_BUTTON7 = 0x400000,
		/// Virtual button 8. Belongs to 2nd set of virtual buttons.
		KC_BUTTON8 = 0x800000
	};

	/** Provides the position of the last mouseclick recieved from the host system. Usually implemented for test environments only.
	@param ruiX Recieves X-coordinate of latest click (return value).
	@param ruiY Recieves Y-coordinate of latest click (return value).
	@return Only if no click was recieved '0' is returned.
	@see GetKey */
	virtual eC_Flag GetMouse(eC_UInt &ruiX, eC_UInt &ruiY);

	//
	virtual void GetMousePos(eC_UInt &ruiX, eC_UInt &ruiY);

	//
	static bool RegisterRenderer(eC_UInt uiID, eGML_RenderFactory pkFactory);

protected:

	// !!! INTERNAL USE ONLY !!!
	// all following protected members are for internal use only

	// protected constructor to forbid direct construction via new. \Ref{Init} is <I>not</I> automatically called when constructing.
	eGML();

	// protected destructor. In case \Ref{Exit} has not been called yet, the destructor will automatically call Exit.
	virtual ~eGML();


	//
	RenderEntry *FindRenderer(eC_UInt uiID);

	//
	virtual bool InternalRegisterRenderer(eC_UInt uiID, eGML_RenderFactory pkFactory);

	//
	static eGML *InternalInit();

	//
	static eC_UInt GetBitDepth(eC_Flag eFormat);

	/* Initialisation callback. Since \Ref{Init} has to be static it can't be virtual. To give child classes of <B>eGML</B> the possibility for
	overloading Init this function is prepared to be replaced with platformspecific initialisation code. */
	virtual void InitInstance();

	// Shutdown callback. See \Ref{InitInstance}
	virtual void ExitInstance();

	// flag to store terminationrequests until they are polled
	bool m_bAbortFlag;

private:

	// !!! INTERNAL USE ONLY !!!
	// all following privat members are for internal use only

	// Stores number of eGML_Init calls
	static eC_UInt m_uiRefCounter;
	// flag that specifies if this instance has been initialised yet. internal use only
	bool m_bIsInitialized;

	// last reported error code
	eC_UInt m_uiLastError;

	// list of known plugin renderers
	RenderEntry *m_pkRenderer;

	// pointer to global <B>eGML</B> instance. internal use only
	static eGML *ms_pkEGML;

#ifdef eGML_THREADSAFE
	static eC_Semaphore ms_akSem[2]; //Semaphores for eGML_THREADSAFE
#endif

#ifdef eGML_USEEMEM
	// pointer to global eMem_Heap instance. internal use only
	static eMem_Heap *ms_pkHeap;
	static eMem_Heap *ms_pkInternalHeap;
#endif
};


#ifdef eC_LEAKDETECTION
#define __ExtParamDec__ ,const char *file, int line
#define __ExtEmemCall__ _Internal_Alloc(uiSize,file,line)
#define __ExtmemCall__ eMem_malloc(sizeof(int)*uiSize, file, line)
#define __ExtmemFree__ eMem_free(p)
#else
#define __ExtParamDec__
#define __ExtEmemCall__ Alloc(uiSize)
#define __ExtmemCall__ eC_Malloc(uiSize)
#define __ExtmemFree__ eC_Free(p)
#endif


//---------------------------------------------------------------
#ifdef eGML_USEEMEM
//---------------------------------------------------------------

inline void *eGML_GetMem (eC_UInt uiSize __ExtParamDec__)
{
	return eGML::GetHeap()->__ExtEmemCall__;
}

inline void eGML_FreeMem (void *p)
{
	eGML::GetHeap()->Free(p);
}

inline void *eGML_InternalGetMem (eC_UInt uiSize __ExtParamDec__)
{
	return eGML::GetInternalHeap()->__ExtEmemCall__;
}

inline void eGML_InternalFreeMem (void *p)
{
	eGML::GetInternalHeap()->Free(p);
}

//---------------------------------------------------------------
#else
//
#define eGML_InternalGetMem eGML_GetMem
#define eGML_InternalFreeMem eGML_FreeMem

inline void *eGML_GetMem (eC_UInt uiSize __ExtParamDec__)
{
	return __ExtmemCall__;
}

inline void eGML_FreeMem (void *p)
{
	__ExtmemFree__;
}
//
#endif

#ifdef eC_LEAKDETECTION
#define eGML_GetMem(s) eGML_GetMem(s, __FILE__, __LINE__)
#define eGML_InternalGetMem(s) eGML_InternalGetMem(s, __FILE__, __LINE__)
#endif


//---------------------------------------------------------------
//Define Macro for including new and delete overwriting in all classes
#ifdef eGML_USEEMEM
//---------------------------------------------------------------
#ifdef eC_LEAKDETECTION
#define NO_OVERLOAD
#else
#define OVERLOAD
#endif
//---------------------------------------------------------------
#endif

#ifdef eC_LEAKDETECTION
#define __ParamDec__ ,const char *file, int line
#define __EmemCall__ _Internal_Alloc((eC_UInt)size,file,line)
#else
#define __ParamDec__
#define __EmemCall__ Alloc((eC_UInt)size)
#endif
//---------------------------------------------------------------
#ifdef OVERLOAD
#define EGML_NEW_DELETE_OPERATOR_OVERWRITING \
	inline void* operator new( size_t size __ParamDec__)\
{\
	eMem_Heap *pHeap = eGML::GetHeap(); \
	if( pHeap == 0 ) return pHeap;\
	return pHeap->__EmemCall__;  \
}\
	inline void* operator new[]( size_t size __ParamDec__) \
{ \
	eMem_Heap *pHeap = eGML::GetHeap(); \
	if( pHeap == 0 ) return pHeap;\
	return pHeap->__EmemCall__;\
}\
	inline void operator delete( void *p )\
{\
	eMem_Heap *pHeap = eGML::GetHeap(); \
	if( pHeap != 0 ) pHeap->Free(p);\
}\
	inline void operator delete[]( void *p )\
{\
	eMem_Heap *pHeap = eGML::GetHeap(); \
	if( pHeap != 0 ) pHeap->Free(p);\
}
#else
#define EGML_NEW_DELETE_OPERATOR_OVERWRITING
#endif


//---------------------------------------------------------------

#define eGML_ARRAY_CREATE(name, type) \
	static type * name = NULL; \
	static eC_UInt name##_count = 0;

#define eGML_ARRAY_CHECK( name, nb, type, clear ) \
	if ( name##_count < (eC_UInt)(nb) ) \
{\
	if ( name != NULL )\
	  eGML_InternalFreeMem( name );\
	\
  name = (type *)eGML_InternalGetMem( sizeof(type) * nb );\
  if ( clear ) \
  {\
    eC_Memset(name, 0, sizeof(type *) * nb);\
  }\
	name##_count = nb;\
}

#define eGML_ARRAY_FREE( name ) \
	if ( name != NULL ) \
	eGML_InternalFreeMem( name ); \
	name = NULL; \
	name##_count = 0;

//---------------------------------------------------------------
#endif
