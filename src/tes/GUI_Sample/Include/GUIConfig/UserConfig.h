#if !defined(USER_CONFIG_H)
#define USER_CONFIG_H

//--------------------------------------------------------------------------------------------
// User specific defines
//--------------------------------------------------------------------------------------------

/// Select the graphics wrapper to use. This way you will select which graphics
/// library is used. You must choose exactly ONE graphics wrapper.

//#define GFX_USE_GDI
#define FNT_USE_FREETYPE
//#define GFX_USE_GL
//#define GFX_USE_EGL
#define GFX_USE_EGML
//#define GFX_USE_DAVE
//#define GFX_USE_DIRECTFB

#endif // USER_CONFIG_H

