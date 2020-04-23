/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_RenderState.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:19:28 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_RENDERSTATE__H_
#define EGML_RENDERSTATE__H_
//---------------------------------------------------------------

#include "eGML_Bitmap.h"


//---------------------------------------------------------------
/**
@memo
Global state of the rendering device.
@doc
The renderstate is member of each \Ref{eGML_Screen} and stores attributes which are constant for the entire {@link eGML_Primitive primitive}.
Renderstates are relevant only when drawing primitives, all kinds of <TT><B>Blit</B></TT> operations or direct calls to <TT><B>DrawLine</B></TT>
or <TT><B>DrawPolygon</B></TT> (each is method of \Ref{eGML_Bitmap}/\Ref{eGML_Screen}) recieve their attributes via function parameters.
Since states are persistent over multiple calls to <TT><B>DrawPrimitive</B></TT> (is also member of bitmap/screen), you do not have to set
all states before calling the method but only those which changed considering the last call. Thereby the amount of data sent to the driver/rendering
hardware will be reduced. You do not need to construct a renderstate object explicitly, because it is a member of the active eGML_Screen object. <BR>
Applications can try to minimize the overhead even further by sorting primitives for similar states where possible. <BR>
One powerful feature of the eGML_Renderstate class is the possibility to set e. g. 3D specific modes for culling and textures. To see which modes
you can evaluate refer to the \Ref{eGML_Renderstate_Modes}. <BR>
To get more information about \Ref{eGML_RenderState} and its handling, look at the \Ref{Tutorial_eGML}.
@see eGML_Renderstate_Modes, eGML_Screen, eGML_Bitmap, eGML_Primitive
@see Tutorial_eGML
*/
class eGML_RenderState
{
public:

	/* !!! INTERNAL USE ONLY !!!
	if macro eGML_USEEMEM is defined (use of eMem) the new, new[], delete and delete[] operators are overloaded */
#undef new
	EGML_NEW_DELETE_OPERATOR_OVERWRITING
#define new eC_NEW


		/** List of all possible renderstates. Using the <TT><B>Set</B></TT> method requires a renderstate as first parameter and a
		value that type fits to the specified state as second and last parameter. So the description of each state provides an
		indication of which kind of type the second parameter has to be.
		@see Set */
		enum RenderStates 
	{
		/** Solid color for rendering. Requires <TT><B>eGML_Color</B></TT> value (use <TT><B>ColorFromRGB</B></TT> (\Ref{eGML_Screen}/
		\Ref{eGML_Bitmap}) to specify). */
		COLOR1,
		// !!! INTERNAL USE ONLY !!! Secondary color. Used for special primitives only.
		COLOR2,
		/// Transparency of alphablending. Requires integer value 0..255, reasonable only if \Ref{ALPHAENABLE} is set <B>true</B> before.
		ALPHA,
		/// Activates alphablending if set <B>true</B>. Requires boolean state.
		ALPHAENABLE,
		/// Constant width for line rendering. Requires <TT><B>eC_Value</B></TT>.
		LINEWIDTH,
		/// Constant width for point rendering. Requires <TT><B>eC_Value</B></TT>.
		POINTSIZE,
		/// Z-distance at which dot radius is equal to 2D radius. Requires <TT><B>eC_Value</B></TT>.
		BASEDEPTH,
		/// Backface culling mode. Possible values see \Ref{Culling_Modes}.
		CULLING,
		/// Enables textures if set <B>true</B>. Requires boolean state. Refer to \Ref{eGML_Texture} to get more information.
		TEXTUREENABLE,
		/// Texture mode. Possible values see \Ref{Texture_Modes}, reasonable only if \Ref{TEXTUREENABLE} is set <B>true</B> before.
		TEXTUREMODE,
		/** X-coordinate of point in 2D/3D that is mapped to upper left corner of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXORIGINX,
		/** Y-coordinate of point in 2D/3D that is mapped to upper left corner of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXORIGINY,
		/** Z-coordinate of point in 3D that is mapped to upper left corner of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXORIGINZ,
		/** X-component of vector in 2D/3D space that is mapped to top edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXUAXISX,
		/** Y-component of vector in 2D/3D space that is mapped to top edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXUAXISY,
		/** Z-component of vector in 3D space that is mapped to top edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXUAXISZ,
		/** X-component of vector in 2D/3D space that is mapped to left edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXVAXISX,
		/** Y-component of vector in 2D/3D space that is mapped to left edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXVAXISY,
		/** Z-component of vector in 3D space that is mapped to left edge of texture.
		Requires <TT><B>eC_Value</B></TT> and depends on \Ref{TEXTUREMODE} setting. */
		TEXVAXISZ,
		/** Texture object. Requires pointer to texture object or NULL if there is no object.
		Refer to \Ref{eGML_Texture} to get more information. */
		TEXTURE,
		/** Texture blending. When texturemapping is enabled this mode controls how 
		*/
		TEXTUREBLENDING,

		//NEW
		/// Line pattern object. Requires pointer to pattern object. Refer to \Ref{eGML_Pattern} to get more information.
		LINEPATTERN,
		/// Enables patterns if set <B>true</B>. Requires boolean state. Refer to \Ref{eGML_Pattern} to get more information.
		LINEPATTERNENABLE,
		/// Number of pixels for entire pattern. Requires <TT><B>eC_Value</B></TT>.
		LINEPATTERNSCALE,
		/// Patternoffset at start of line. <TT><B>eC_Value</B></TT>
		LINEPATTERNOFFSET,
		/// Enables vertex colors if set <B>true</B>. Requires boolean state. Refer to \Ref{eGML_Primitive} to get more information.
		VERTEXCOLORENABLE,

		//
		TEXTUREWRAPU,
		TEXTUREWRAPV,
		TEXTUREFILTER,

#ifdef eGML_TEXTURECHECK
      TEXTUREAREA,
#endif

	  	//--------------------------------------
		OPTIMIZED_BLIT,

		//--------------------------------------
		// !!! INTERNAL USE ONLY !!!
		// internal renderstates. not to be documented or used outside of <B>eGML</B>
		//
		I_TEXMATVALID,
		//
		I_TEXMATCACHE,
		//
		I_LINPATFACTOR,
		//
		I_LINPATREFLEN,

		// just for dimensioning state arrays
		eGML_RS_QUANTITY
	};

	//---------------------------------------------------------------
	/**
	List of all possible culling modes used by the <TT><B>Set</B></TT> method together with the
	<TT><B>CULLING GML_State</B></TT> of \Ref{eGML_RenderState} class.
	@see eGML_RenderState
	*/
	enum Culling_Modes
	{
		/// No culling.
		CULL_NONE = 0,
		/// Culling clockwise.
		CULL_CW   = 1,
		/// Culling counterclockwise.
		CULL_CCW  = 2
	};

	//---------------------------------------------------------------
	/**
	@memo
	List of texture modes.
	@doc
	List of all possible {@link eGML_Texture texture} modes used by the <TT><B>Set</B></TT> method together with the
	<TT><B>TEXTUREMODE GML_State</B></TT> of \Ref{eGML_RenderState} class.
	@see eGML_RenderState, eGML_Texture
	*/
	enum Texture_Modes
	{
		/// Plane 2D textures. Possible use of 2D-orientation.
		TM_PLANE2D = 1,
		/// Plane 3D textures. Possible use of 3D-orientation.
		TM_PLANE3D = 2,
		/// UV-parameters for textures.
		TM_UV      = 4,
		// experimental 
		TM_DIRECT  = 5
	};

	//
	enum Texture_Wrapping
	{
		TW_REPEAT = 1,
		TW_CLAMP  = 2,
		TW_BORDER = 3,
		TW_MIRROR = 4
	};

	//
	enum Texture_Filtering
	{
		TF_NONE = 0,
		TF_BILINEAR = 1,
		TF_TRILINEAR = 2,
		TF_ANISOTROPIC = 3
	};

	//
	enum Texture_Blending
	{
		TB_NORMAL = 0,
		TB_MULTIPLY = 1,
		TB_ADD = 2,
		TB_COLOR = 3
	};

	//
	enum Optimized_Blit
	{
		OB_NONE    = 0,
		OB_NEON	   = 1
	};

	/** Standard constructor. Prepares renderstate object (e. g. invalidates all states that have to be enabled explicitly).
	@see Set */
	eGML_RenderState();

#ifdef DOCXX
	/** Sets renderstate. The expected type of value depends on the state you wish to modify.
	@param eState {@link RenderStates State} to modify.
	@param Value New value for this state (may need explicit cast to eC_Int for integer constants on some compilers).
	@see RenderStates */
	void Set(enum RenderStates eState, ... Value);
#endif

	// !!! INTERNAL USE ONLY !!!
	// all following public members are for internal use only

	//
	typedef void (* eGML_StateCallback) (eGML_RenderState * pkBase, eGML_RenderState::RenderStates eState,
		const eC_MixedType &kValue);

	//
	void Set(enum RenderStates eState, bool iValue);

	//
	void Set(enum RenderStates eState, eC_Int iValue);

	//
	void Set(enum RenderStates eState, void *pvValue);

#ifndef eC_FIXPOINT
	//
	void Set(enum RenderStates eState, const eC_Value &vValue);
#endif

	// catch ambigous calls.
	inline void Set(enum RenderStates eState, const eC_UInt &uiValue) { Set(eState, static_cast<eC_Int>(uiValue)); }

	//
	inline const eC_MixedType & Get(eC_UInt uiState) const  { return m_akState[uiState]; }

	//
	inline eC_Int GetInt(enum RenderStates eState) const     { return m_akState[eState].iValue; }

	//
	inline eC_UInt GetUInt(enum RenderStates eState) const   { return m_akState[eState].uiValue; }

	//
	inline eC_Value GetValue(enum RenderStates eState) const { return m_akState[eState].vValue; }

	//
	inline eC_Fixed GetFixed(enum RenderStates eState) const { return m_akState[eState].fxValue; }

	//
	inline eGML_Color GetColor(enum RenderStates eState) const { return m_akState[eState].kValue; }

	//
	inline eC_Bool GetBool(enum RenderStates eState) const
	{  
		/* if ( m_akState[eState].iValue == 1)  return true; else  return false; */
		return m_akState[eState].bValue;
	}

	//
	inline void *GetPtr(enum RenderStates eState) const { return m_akState[eState].pvValue; }

	//
	void RegisterCallback(enum RenderStates eState, eGML_StateCallback pfCode);

private:

	friend class eGML_Screen;

	// !!! INTERNAL USE ONLY !!!
	// all following private members are for internal use only

	//
	static void InvalidateTextureMatrix(eGML_RenderState *pkBase, eGML_RenderState::RenderStates eState, const eC_MixedType &kValue);

	//
	static void SetNewPatternScale(eGML_RenderState *pkBase, eGML_RenderState::RenderStates eState, const eC_MixedType &kValue);

	//
	eGML_Screen * m_kParent;

	//
	eC_MixedType m_akState[eGML_RS_QUANTITY];

	//
	eGML_StateCallback m_apCallback[eGML_RS_QUANTITY];
};

//---------------------------------------------------------------
#endif

