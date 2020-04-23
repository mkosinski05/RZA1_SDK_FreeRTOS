/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

/**
@warning Always include this file as the LAST included header in your .cpp files!
*/
#ifndef __GUI_MEM_LEAK_WATCHER_H__
#define __GUI_MEM_LEAK_WATCHER_H__

#include  <stddef.h>
#include "GUIConfigDebug.h"

// EVC4 uses the operator new from MemLeakDetection, although this header is not included. Thus, the compiler is broken.
#ifndef eC_TARGET_ENV_POCKET

#ifdef GUILIANI_LEAK_DETECTION

// Disable warning "4291:no matching operator delete found; memory will not be freed
// if initialization throws an exception" on Visual C++ Compilers.
#ifdef _WIN32
    #pragma warning( disable : 4291 )
#endif

#ifdef new
    #undef new
#endif

#ifdef delete
    #undef delete
#endif

#ifdef malloc
    #undef malloc
#endif

#ifdef free
    #undef free
#endif

#ifdef realloc
    #undef realloc
#endif

/** memory-tracing function used with delete()
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of callin gfunction
*/
void Guiliani_delete(const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with malloc()
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of calling function
    @param uiSize Size
*/
void* Guiliani_malloc(size_t uiSize, const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with free()
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of calling function
    @param pMemory Pointer to allocated memory
*/
void Guiliani_free(void* pMemory, const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with realloc()
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of calling function
    @param pMemory Pointer to allocated memory
    @param uiSize Size
*/
void* Guiliani_realloc(void* pMemory, size_t uiSize, const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with new() operator
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of calling function
    @param uiSize Size
*/
void* operator new(size_t uiSize, const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with new[] operator
    @param pFile File name
    @param iLine Line in file
    @param pFunction Name of calling function
    @param uiSize Size
*/
void* operator new[](size_t uiSize, const char* pFile, int iLine, const char* pFunction);

/** memory-tracing function used with delete() operator
    @param Memory Pointer to allocated memory
*/
void operator delete(void* Memory) throw();

/** memory-tracing function used with delete[] operator
    @param Memory Pointer to allocated memory
*/
void operator delete[](void* Memory) throw();

#define new new(__FILE__,__LINE__,__FUNCTION__)
#define delete Guiliani_delete(__FILE__,__LINE__,__FUNCTION__),delete
#define malloc(x) Guiliani_malloc(x,__FILE__,__LINE__,__FUNCTION__)
#define free(x) Guiliani_free(x,__FILE__,__LINE__,__FUNCTION__)
#define realloc(x,y) Guiliani_realloc(x,y,__FILE__,__LINE__,__FUNCTION__)

#endif //#ifndef eC_TARGET_POCKETPC
#endif //#ifdef GUILIANI_LEAK_DETECTION
#endif //#ifndef __GUI_MEM_LEAK_WATCHER_H__
