/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

/** This header should never be included directly by user code. */

#ifndef GUIMEMLEAKDETECTION__H_
#define GUIMEMLEAKDETECTION__H_

#include "GUIConfigDebug.h"

// EVC4 uses the operator new from MemLeakDetection, although the GUIMemLeakWatcher.h header is not included. Thus, the compiler is broken.
#ifndef eC_TARGET_ENV_POCKET
#ifdef GUILIANI_LEAK_DETECTION

#include "eC_String.h"
#include "GUITrace.h"
#include "eC_TList_doubleLinked.h"

#define LEAK_DETECTION CGUIMemLeakDetection::GetInstance()

/// Class CGUIMemLeakDetection for analyzing the use of memory and detecting the memory leaks.

/**
    Include GUIMemLeakFuntions.h in the same file where the main() function is implemented.
    To show the results of leak detection call ShowResults() at the end.
    Don't use the new operator in this class, otherwise there will be a recursive new operator call
    until you'll have a stack overflow and the application crashes!

    @attention  CGUIMemLeakDetection IS NOT THREAD-SAFE! USE IT CAREFULLY, ESPECIALLY WHILE USING IT IN COMMANDS.<br>
                Be aware then memleakdetection will only work for .cpp files in which GUIMemLeakWatcher.h was included
                as the last include statement!

    @code
    /// main.cpp
    #include "GUIMemLeakFunctions.h"
    #include "GUIMemLeakWatcher.h"

    int main()
    {
      //some code here
      ...

      // Call this at the end to show the leak detection results on
      // currently registered output device (console, etc.).
      LEAK_DETECTION.ShowResults();
      return 0;
    }
    @endcode

    @code
    // Example of leak detection results.
    // The output shows the file, line number and function
    // where the not freed allocation happened.

    DEBUG: ---------- Guiliani memoryleak detection information ----------
    DEBUG: ---------- Leak detected ----------
    DEBUG: File: ..\MainWindow.cpp
    DEBUG: Line: 530
    DEBUG: Function: CMainWindow::PopulateTable
    DEBUG: Adress: 0xd8c900
    @endcode
*/
class CGUIMemLeakDetection
{
public:
    /* The memory allocation functions need to access the private memory management methods
        in this class, thus they are friend to this class.
    */

    friend void Guiliani_delete(const char* pFile, int iLine, const char* pFunction);
    friend void* Guiliani_malloc(size_t uiSize, const char* pFile, int iLine, const char* pFunction);
    friend void Guiliani_free(void* pMemory, const char* pFile, int iLine, const char* pFunction);
    friend void* Guiliani_realloc(void* pMemory, size_t uiSize, const char* pFile, int iLine, const char* pFunction);
    friend void* operator new(size_t uiSize, const char* pFile, int iLine, const char* pFunction);
    friend void* operator new[](size_t uiSize, const char* pFile, int iLine, const char* pFunction);
    friend void operator delete(void* Memory) throw();
    friend void operator delete[](void* Memory) throw();

    /** Method which writes the results of the analysis to a file.
        @param sFilename Filename with path for the results
    */
    void WriteResultsToFile(const eC_String& sFilename = "MemoryAnalysisResults.log");

    /** Method which shows results on screen uses GUILOG. */
    void ShowResults();

    /** Method which returns the instance of this class.
        @return instance of this class
    */
    static CGUIMemLeakDetection& GetInstance()
    {
#ifdef GUILIANI_MEMORY_ALLOCATION_LOGGING
        static CGUIMemLeakDetection s_MemLeakDetection(true);
#else
        static CGUIMemLeakDetection s_MemLeakDetection(false);
#endif
        return s_MemLeakDetection;
    }

private:
    /** Enables Guiliani leak detection file logging, which outputs every memory
        allocation and deallocation to the file "MemoryAllocation.log".
    */
    void EnableLogging();

    /** Own method for allocating memory */
    void* MemAlloc(size_t Size, const eC_Char* pFile, eC_Int iLine, const eC_Char* pFunction, const eC_Char* pNewTypeName, eC_Bool bIsArray);

    /** Own method for freeing memory */
    void FreeMem(void* pMemory, const eC_Char* pFile, eC_Int iLine, const eC_Char* pFunction, const eC_Char* pDeleteTypeName, eC_Bool bIsArray);

    /** Own method for reallocating memory */
    void* MemRealloc(void* pMemory, size_t uiSize, const eC_Char* pFile, eC_Int iLine, const eC_Char* pFunction);

    /** Struct BlockHead_t: This struct will be added to every created Object,
        after the CreateInstance method of CGUIMemLeakDetection was called.
        These information are needed, for the evaluation of the memory usage.
    */
    struct BlockHead_t
    {
    /** For the magic number, to check whether the object was really
        created with the overloaded new operator.
    */
        eC_UInt m_uiMagicNumber;

    /** The information about the object size.
    */
        size_t m_uiObjectSize;
    };

    /** Struct BlockInfo_t includes the information about the newed object.
        This information will be added to list
        There is only pointer assignment, because it must not use the new
        operator.
        If you use the new operator, there will be a recursive new operator call
        until you'll have a stack overflow and the application crashes!
    */
    struct BlockInfo_t
    {
        /** BlockInfo_t constructor */
        BlockInfo_t()
        {
            m_pFilename = NULL;
            m_uiLine = 0;
            m_pFunction = NULL;
            m_pMemoryAddress = NULL;
            m_bIsArrayAlloc = false;
        }

        /** Overloaded equal-operator. It compares only the MemoryAddress.
            @param kValue right value
        */
        bool operator==(const BlockInfo_t& kValue) const
        {
            return m_pMemoryAddress==kValue.m_pMemoryAddress;
        }

        /** Char pointer to the filename string */
        const eC_Char* m_pFilename;

        /** Integer value with the line where the object is created */
        eC_UInt m_uiLine;

        /** Char pointer which includes the function name where the object is created */
        const eC_Char* m_pFunction;

        /** Memory address value which includes start address of the pointer */
        void* m_pMemoryAddress;

        /** Whether the allocation is a C++ array allocation (new[]). */
        eC_Bool m_bIsArrayAlloc;
    };

    /** Because of singleton the constructor is private */
    CGUIMemLeakDetection(const eC_Bool& rkLogging);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIMemLeakDetection(const CGUIMemLeakDetection& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIMemLeakDetection& operator=(const CGUIMemLeakDetection& kSource);

    /** Because of singleton the destructor is private */
    ~CGUIMemLeakDetection();

    /** File pointer for the memory allocation file. */
    FILE *m_fpMemoryAllocation;

    /** Boolean for the state of writing memory allocation. */
    eC_Bool m_bLogState;

    /** Attribute for information about after how many steps information will be written into memory
    allocation file */
    eC_UInt m_uiMemoryAllocSteps;

    /** Attribute for counting the steps (delete operator) */
    eC_UInt m_uiMemoryAllocStepsCounterDelete;

    /** Attribute for counting the steps (new operator) */
    eC_UInt m_uiMemoryAllocStepsCounterNew;

    /** List for the object information. */
    eC_TListDoubleLinked<BlockInfo_t> m_List;

    /** Attribute which contains the information about the current memory usage (byte). */
    eC_Int m_iUsedMemory;

    /** Attribute with the information about the maximum memory allocation (byte). */
    eC_Int m_iMaxMem;

    /** Attribute of the allocated memory value (byte) of the largest object. */
    size_t m_uiLargestObject;

    /** Counter for the number of not deleted objects. */
    eC_Int m_iNumNotDeletedObjects;

    /** Static constant definition of the magic number */
    static const eC_UInt ms_cuiMagicNr = 395743214;
};

#endif //#ifdef GUILIANI_LEAK_DETECTION
#endif //#ifndef eC_TARGET_POCKETPC

#endif
