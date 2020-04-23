/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIMEMLEAKFUNCTIONS__H_
#define GUIMEMLEAKFUNCTIONS__H_

#include "GUIMemLeakDetection.h"
#include "GUIConfigDebug.h"

// EVC4 uses the operator new from MemLeakDetection, although the GUIMemLeakWatcher.h header is not included. Thus, the compiler is broken.
#ifndef eC_TARGET_ENV_POCKET
#ifdef GUILIANI_LEAK_DETECTION

// Needed for workaround due to that it's not possible to define the delete operator with file, line and function information
const char* g_pcFile;
int g_iLine;
const char* g_pcFunction;

void Guiliani_delete(const char* pFile, int iLine, const char* pFunction)
{
  g_pcFile = pFile;
  g_iLine = iLine;
  g_pcFunction = pFunction;
}

void* Guiliani_malloc(size_t uiSize, const char* pFile, int iLine, const char* pFunction)
{
    return CGUIMemLeakDetection::GetInstance().MemAlloc(uiSize, pFile, iLine, pFunction, "GUILIANI_malloc", false);
}

void Guiliani_free(void* pMemory, const char* pFile, int iLine, const char* pFunction)
{
    CGUIMemLeakDetection::GetInstance().FreeMem(pMemory, pFile, iLine, pFunction, "GUILIANI_free", false);
}

void* Guiliani_realloc(void* pMemory, size_t uiSize, const char* pFile, int iLine, const char* pFunction)
{
    return CGUIMemLeakDetection::GetInstance().MemRealloc(pMemory, uiSize, pFile, iLine, pFunction);
}

void* operator new(size_t uiSize, const char* pFile, int iLine, const char* pFunction)
{
    return CGUIMemLeakDetection::GetInstance().MemAlloc(uiSize, pFile, iLine, pFunction, "operator new", false);
}

// Vector new routine
void* operator new[](size_t uiSize, const char* pFile, int iLine, const char* pFunction)
{
    return CGUIMemLeakDetection::GetInstance().MemAlloc(uiSize, pFile, iLine, pFunction, "operator new[]", true);
}

void operator delete(void* Memory) throw()
{
    CGUIMemLeakDetection::GetInstance().FreeMem(Memory, g_pcFile, g_iLine, g_pcFunction, "operator delete", false);
}

// Vector delete routine
void operator delete[](void* Memory) throw()
{
    CGUIMemLeakDetection::GetInstance().FreeMem(Memory, g_pcFile, g_iLine, g_pcFunction, "operator delete[]", true);
}

#endif //#ifdef GUILIANI_LEAK_DETECTION
#endif //#ifndef eC_TARGET_POCKETPC

#endif
