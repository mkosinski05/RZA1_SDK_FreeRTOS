/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_MemLeakDetection.h
* %version:        134.1 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:00:18 2006
* description:

* %date_modified:  % (%derived_by: hh04031 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef eC_MEMLEAKDETECTION_H
#define eC_MEMLEAKDETECTION_H



//-------------------------------------------------------------------------------------------------------
#ifdef DOCXX

/**@name eC_MemLeakdetection
@memo
Application wide memory leakdetection system.
@doc
If the macro \Ref{eC_LEAKDETECTION} is defined, the memory leakdetection is activated.<BR>
If the macro \Ref{eC_MEMORY_CONSUMPTION_OVERVIEW} is defined, the memory consumtion report is activated also.<BR>

The eC_MemLeakdetection overloads all new, malloc, free, and delete operators of the application. The new and delete 
operator signature will extend by the parameter for the calling file and the linenumber. These informations 
together with the allocated bytes are stored in a hashtable. The Hashtable has \Ref{eC_LEAKDETECTION_HASHTABLE_SIZE}
entries. <BR>
If the application allocates memory, the eC_MemLeakdetection component stores the callers data in to the hashtable 
and removes the entries again if the memory is deallocated by the application. At the end of the applications life, the 
\Ref{eC_DebugShutdown} function must be called to finisch the MemLeakdetection. By calling this function the 
eC_MemLeakdetection prints out a final reportlist of all still open allocations. These open allocations 
are Memory-Leaks. Depends of the environment, the reporting list will printed out either in the Visual C++ Studio,
or on the console. If the list is represented in the Visual Studio, it is possible to get to the caller just by 
double click on a line of the reporting list.

<b>Usageasge of the Memleakdetection:</b>
1. Define \Ref{eC_LEAKDETECTION} in an application wide scope (e.g. makefile of header file). The best way is to 
use a eC_UserConfig-File.
2. There is <b>no</b> additional work necessary to inititialize or start the Memleak-System.
3. At the end of the applications life, call \Ref{eC_DebugShutdown}. This prints a list of all memory leaks.


<b>Usageasge of the MemoryConsumption:</b>
1. Define \Ref{eC_MEMORY_CONSUMPTION_OVERVIEW} in an application wide scope (e.g. makefile of header file). The 
best way is to use a eC_UserConfig-File.
2. There is <b>no</b> additional work necessary to inititialize or start the MemConsumtion-System.
3. By calling the \Ref{eC_DebugPrintMemoryConsumtion}-Function is possible to get a insight of the current 
memory usage, during the applications life. 
4. Also it is possible to measure the memory consumption during a period. In this case the consumtion list must
be reseted first. To get the memory consumption list at the end of the measure period, call the 
\Ref{eC_DebugPrintMemoryConsumtion}-Function.


<b>Description of the format and the content of the reporting lists:</b>
The Memoryleakdetekion and the MemoryConsumtion both serves with a reporting list. The construction and content of 
these lists are indentical. The following section describes the informations in these reporting 
lists.

Sample of reporting list
\eGaC\comps\eGML\code\eGML_BitmapRGB32.cpp(15), 2461696 byte, 3 calls, <eMem_Heap> 
\eGaC\comps\eMem\code\eMem_MemPool.cpp(391), 2457640 byte, 2 calls
\comps\eMem\code\eMem_CellPool.cpp(549), 1397808 byte, 86 calls
\eGaC\comps\eMem\code\eMem_CellPool.cpp(431), 1259792 byte, 1204 calls
\eGaC\comps\eGML\code\eGML_Screen.cpp(129), 1058512 byte, 10178 calls

The list is like a table with colums, the items of a row are separated by a ',' sign. A row
has following values:
1. The filename and in parenthesis the linenumber, this combination is the caller
2. All bytes allocated by the caller
3. The number of memory allocations of this caller
4. Optional sign to indicate if the memory was mamaged by the eMem-Component

This List is sorted by the amount of allocated memory by the caller.

At the and of the MemoryConsumtion list the following additional informations will be printed out :
1. The max. useage of memory, in the form like this :"--- Maximun useage 13589237 of Bytes ---"
2. The current useage of memory, in the from like this :" --- Currently are 12265782 of Bytes in use ---"

*/

//@{

/**
Adds a new entry in to the internal used Hashtable. Use this function to register a
new memory allocation occurence.

@param adr Adress of the allocated memory
@param size Number of bytes allocated
@param src Filename of the caller
@param lnr Line number in the callers file
*/
void EC_FNKT(eC_DebugAddItem)(unsigned long adr, unsigned long size, const char *src, int lnr);

/**
Removes an entry in the internal used Hashtable. Use this function if the application
frees a allocated memory.

@param adr Adress of the freed memory
*/
void EC_FNKT(eC_DebugRemItem)(unsigned long adr);


/**
Prints out the report list of the current used memory and theirs caller.

Description of the format and the content of the reporting lists:
The MemoryConsumtion serves with a reporting list. The following section 
describes the informations in this reporting list.

Sample of reporting list
\eGaC\comps\eGML\code\eGML_BitmapRGB32.cpp(15), 2461696 byte, 3 calls, <eMem_Heap> 
\eGaC\comps\eMem\code\eMem_MemPool.cpp(391), 2457640 byte, 2 calls
\comps\eMem\code\eMem_CellPool.cpp(549), 1397808 byte, 86 calls
\eGaC\comps\eMem\code\eMem_CellPool.cpp(431), 1259792 byte, 1204 calls
\eGaC\comps\eGML\code\eGML_Screen.cpp(129), 1058512 byte, 10178 calls

The list is like a table with colums, the items of a row are separated by a ',' sign. A row
has following values:
1. The filename and in parenthesis the linenumber, this combination is the caller
2. All bytes allocated by the caller
3. The number of memory allocations of this caller
4. Optional sign to indicate if the memory was mamaged by the eMem-Component

This List is sorted by the amount of allocated memory by the caller.

At the and of the MemoryConsumtion list the following additional informations will be printed out :
1. The max. useage of memory, in the form like this :"--- Maximun useage 13589237 of Bytes ---"
2. The current useage of memory, in the from like this :" --- Currently are 12265782 of Bytes in use ---"

@param usReportListLength The length of the reporting list. If this parameter is not used, the report list
has the default length defined in eC_MEMORY_CONSUMPTION_REPORTLENGTH
*/
void EC_FNKT(eC_DebugPrintMemoryConsumtion)(unsigned short usReportListLength);

/**
Resets the internal used hastable for recording the memory consumtion data.
Call this function to clear all previous data and start a new measering of memmory consumtion.
*/
void EC_FNKT(eC_DebugResetMemoryConsumtion)();

/**
If the the \Ref{eC_LEAKDETECTION} is activated, call this functin an the very end 
of the aplications life. By calling this function 
1. The MemLeak-Detection system will be shut down
2. A reporting list will be printed out of all memory leaks.

Description of the format and the content of the reporting lists:
The Memory Leakdetection serves with a reporting list. The following section 
describes the informations in this reporting list.

Sample of reporting list
\eGaC\comps\eGML\code\eGML_BitmapRGB32.cpp(15), 2461696 byte, 3 calls, <eMem_Heap> 
\eGaC\comps\eMem\code\eMem_MemPool.cpp(391), 2457640 byte, 2 calls
\comps\eMem\code\eMem_CellPool.cpp(549), 1397808 byte, 86 calls
\eGaC\comps\eMem\code\eMem_CellPool.cpp(431), 1259792 byte, 1204 calls
\eGaC\comps\eGML\code\eGML_Screen.cpp(129), 1058512 byte, 10178 calls

The list is like a table with colums, the items of a row are separated by a ',' sign. A row
has following values:
1. The filename and in parenthesis the linenumber, this combination is the caller
2. All bytes allocated by the caller
3. The number of memory allocations of this caller
4. Optional sign to indicate if the memory was mamaged by the eMem-Component

This List is sorted by the amount of allocated memory by the caller.

*/
void EC_FNKT(eC_DebugShutdown)();

//@}

#endif
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------











//--------------------
//     INCLUDES
//--------------------
#include <stdlib.h>
#include "eC_Platform.h"


//--------------------
//     Defines
//--------------------
//Output of leak data to different channels...
#ifdef eC_TARGET_ENV_WIN32
#define eC_LEAK_LOG eC_WindowOutput
#else
#define eC_LEAK_LOG printf
#endif

//Hashtable defs
#ifdef eC_LEAKDETECTION_HASHTABLE_SIZE
#define eC_HASHSIZE eC_LEAKDETECTION_HASHTABLE_SIZE
#else
#define eC_HASHSIZE 0x1FFFE
#endif

// Debugbuffer sizes
#define eC_MAXSRCFILE	128			// Bytes allocated to store sourcefileinfo in memblocks...
#define eC_MAXLINE 1024


//---------------------------
//      Function prototypes
//---------------------------
extern void eC_WindowOutput(const char *format, ...);
//No Operation; Empty statement
inline void eC_NOP(void){}




//----------------------------------------------------------------
//-------- Begin of the leakdetection code -----------------------
//----------------------------------------------------------------
//---------- Only activated if eC_LEAKDETECTION is defined -------
//----------------------------------------------------------------

/*
	The complete Code for leakdetection can be switched 
	on or off by the eC_LEAKDETECTION definition.
*/
#ifdef eC_LEAKDETECTION

typedef enum 
{
   eC_LEAKDETECTION_SORT_MEMORY_CONSUMTION_NUMBER_OF_BYTES,
   eC_LEAKDETECTION_SORT_MEMORY_CONSUMTION_NUMBER_OF_CALLS
}eC_LeakdetectionMemoryConsumtionListSortValue;


// Bind to memdebug hooks
extern void eC_DebugAddItem(unsigned long adr, unsigned long size, const char *src, int lnr);
extern void eC_DebugRemItem(unsigned long adr);
extern void eC_DebugExit(void);
extern void eC_DebugPrintMemoryConsumtion(unsigned short usReportListLength=eC_MEMORY_CONSUMPTION_REPORTLENGTH, 
               eC_LeakdetectionMemoryConsumtionListSortValue eSortValue=eC_LEAKDETECTION_SORT_MEMORY_CONSUMTION_NUMBER_OF_BYTES);
extern void eC_DebugResetMemoryConsumtion();
extern void eC_DebugStartAllocationWatch();
extern void eC_DebugStopAllocationWatch();
extern void eC_DebugPrintAllocationWatch(unsigned short usReportListLength=eC_MEMORY_CONSUMPTION_REPORTLENGTH, 
               eC_LeakdetectionMemoryConsumtionListSortValue eSortValue=eC_LEAKDETECTION_SORT_MEMORY_CONSUMTION_NUMBER_OF_BYTES);


//-------------------------------------------------------
// Treenode to represent an allocated memoryblock.
typedef struct tag_memItem
{
	unsigned long	adr;						// address of the represented memblock
	unsigned long	size;						// size of the represented memblock in bytes
	char     src[eC_MAXSRCFILE];	// sourcefile
	int		   lnr;						      // linenumber
	char	   heapMemory;					// indicator for using eMem_Heap
} memItem;


//-------------------------------------------------------
// Listnode to represent a memoryleak
typedef struct tag_memleakItem
{
	unsigned long	calls;
	unsigned long	size;
	int		lnr;
	char  src[eC_MAXSRCFILE];
	char	heapMemory;						// indicator for using eMem_Heap
} memleakItem;


//-------------------------------------------------------
// Prototype overloaded operators 
//-------------------------------------------------------
inline  void *  operator new(size_t size, const char *file, int line)
{
	void *ptr = (void *)malloc(size);
	eC_DebugAddItem((unsigned long)ptr, (unsigned long)size, file, line);
  return(ptr);
};

inline void *  operator new[](size_t size, const char *file, int line)
{
  void *ptr = (void *)malloc(size);
	eC_DebugAddItem((unsigned long)ptr, (unsigned long)size, file, line);
  return(ptr);
};

inline void  operator delete(void *p)
{
	eC_DebugRemItem((unsigned long)p);
  free(p);
};

inline void  operator delete(void *p, const char *file, int line)
{
	eC_DebugRemItem((unsigned long)p);
  free(p);
};

inline void  operator delete[](void *p)
{
	eC_DebugRemItem((unsigned long)p);
  free(p);
};

inline void  operator delete[](void *p, const char *file, int line)
{
	eC_DebugRemItem((unsigned long)p);
  free(p);
};

inline void *  eC_malloc(size_t size, const char *file, int line)
{
	void *ptr = (void *)malloc(size);
	eC_DebugAddItem((unsigned long)ptr, (unsigned long)size, file, line);
  return(ptr);
}

inline void *  eC_calloc(size_t num, size_t size, const char *file, int line)
{
	void *ptr = (void *)calloc(num, size);
	eC_DebugAddItem((unsigned long)ptr,(unsigned long) num*size, file, line);
  return(ptr);
}

inline void *  eC_realloc(void *oldptr, size_t size, const char *file, int line)
{
	void *ptr = (void *)realloc(oldptr, size);
	if (oldptr) eC_DebugRemItem((unsigned long)oldptr);
	eC_DebugAddItem((unsigned long)ptr, (unsigned long)size, file, line);
  return(ptr);
}

inline void  eC_free(void *p)
{
	eC_DebugRemItem((unsigned long)p);
  free(p);
};


//-------------------------------------------------------
//-------------------------------------------------------
// Defines; for overloading global new and delete operators
//-------------------------------------------------------

// Using Memdebugging
#define eC_NEW new(__FILE__, __LINE__)
	#define eC_DebugShutdown eC_DebugExit
#else //eC_LEAKDETECTION
// Using NO Memdebugging
	#define eC_NEW new
	#define eC_DebugShutdown eC_NOP
#endif //eC_LEAKDETECTION


// Install memdebug handlers (__eC_DONTHOOKMDEBUG__ is present so that eC_Leakdetect.cpp
// can exclude itself from memorydebugging, don't use it anywhere else!)
#ifdef eC_LEAKDETECTION
	#ifndef __eC_DONTHOOKMDEBUG__ //avoid the use of debug logging for eC_Leakdetect itself
		#undef new
		#define new				eC_NEW
		#define malloc(x)		eC_malloc(x,__FILE__,__LINE__)
		#define calloc(x,y)		eC_calloc(x,y,__FILE__,__LINE__)
		#define realloc(x,y)	eC_realloc(x,y,__FILE__,__LINE__)
		#define free(p)			eC_free(p)
	#else //__eC_DONTHOOKMDEBUG__ 
		#undef new
		#define new				eC_NEW
		#define malloc(s)       malloc(s)
		#define calloc(c, s)    calloc(c, s)
		#define realloc(p, s)   realloc(p, s)
		#define _expand(p, s)   _expand(p, s)
		#define free(p)         free(p)
	#endif //__eC_DONTHOOKMDEBUG__ 
#endif //eC_LEAKDETECTION


#endif


