/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_FreeRTOS_Platform.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Fri Sep 09 18:46:36 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

//
// Interface to platform specific functions
//



#ifndef EC_FreeRTOS_PLATFORM__H_
#define EC_FreeRTOS_PLATFORM__H_


//---------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "eC_Types.h"
#include "FreeRTOS.h"
#include "portable.h"
// platform API unicode encoding
#define eC_STRING_UNICODE_UTF16
//#define eC_LEAKDETECTION

//#error Please define eC_STRING_UNICODE_UTF8, eC_STRING_UNICODE_UTF16 or eC_STRING_UNICODE_UTF32 matching your platform's API unicode encoding.

//---------------------------------------------------------------
// basic clib functions

inline void *eC_Malloc(eC_UInt tSize, eC_UByte ubID=0) 
{
  return pvPortMalloc((size_t) tSize); 
}

inline void eC_Free(void *pkData, eC_UByte ubID=0)
{
  vPortFree(pkData);	
}

inline void *eC_Memcpy( void *dest, const void *src, size_t count )
{
    return memcpy(dest, src, count);
}

inline void *eC_Memmove( void *dest, const void *src, size_t count )
{
    return memmove(dest, src, count);
}

inline void *eC_Memset( void *dest, int c, size_t count )
{
    return memset(dest, c, count);
}

//---------------------------------------------------------------
// basic fileio functions

#define eC_SEEK_BEGIN   SEEK_SET
#define eC_SEEK_CURRENT SEEK_CUR
#define eC_SEEK_END     SEEK_END

class eC_PlatformFileIO
{
public:
    typedef struct _FILE_FIND_DATA {
        eC_WChar pwcFileName[256];
    } FILE_FIND_DATA;

    static void *eC_PlatformFOpen( const eC_Char *acName, const char *acMode );
    static void *eC_PlatformFOpen( const eC_WChar *acName, const eC_WChar *acMode );
    static void eC_PlatformFClose( void *pvFile );
    static eC_Bool eC_PlatformFSeek( void *pvFile, eC_Int iOffset, eC_UInt uiFlag );
    static eC_UInt eC_PlatformFTell( void *pvFile );
    static eC_UInt eC_PlatformFWrite( void *pvFile, void *pvData, eC_UInt uiSize );
    static eC_UInt eC_PlatformFRead( void *pvFile, void *pvData, eC_UInt uiSize );
    static void *eC_PlatformFindFirstFile( const eC_WChar *acName, FILE_FIND_DATA& fileData );
    static eC_Bool eC_PlatformFindNextFile( void *pvFile, FILE_FIND_DATA& fileData );
    static eC_Bool eC_PlatformFindClose( void *pvFile );
};

//---------------------------------------------------------------
// debug interface

#define eC_GETTICKS()		eC_GetTicks64()
#define eC_GET_MICROSECONDS(tTime)		eC_GetMicroSec(tTime)

extern void eC_PlatformStringOut(const char *acMessage);

extern eC_Time64 eC_GetTicks64();
extern eC_UInt eC_GetMS(eC_Time64 tTime);
extern eC_UInt eC_GetMicroSec(eC_Time64 tTime);

//---------------------------------------------------------------
// task/thread suspend/switch interface
eC_Bool eC_Wait(eC_UInt ms, eC_Flag bActive = false);


#endif
