/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Memory.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:00:23 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_MEMORY__H_
#define EC_MEMORY__H_

//------------------------------------------------------------------------------------------------------
#include "eC_Types.h"
#include "eC_Platform.h"

//------------------------------------------------------------------------------------------------------
/* !!! INTERNAL USE ONLY !!!
@name eC_Memory
@memo
Memory manager interface.
@doc
Interface to define/replace <B>eGML</B>'s lowlevel memory manager.
All allocations within <B>eGML</B> are mapped to theses functions (including new,new[],delete and delete[]).
*/

//{



#ifdef eC_LEAKDETECTION
extern void eC_DebugAddItem(unsigned long adr, unsigned long size, const char *src, int lnr);

inline void* eC_DbgGetMem(eC_UInt size, const char *file, int line)
{
  void *p = eC_Malloc((eC_UInt)(size));
  eC_DebugAddItem((int)p, size, file, line );
  return p;
}
#endif



#ifndef eC_INITMEM
// Prepares heap. Redefine this macro to call any function necessary to initialize memory management.
#define eC_INITMEM()
#endif

#ifndef eC_EXITMEM
// Cleans up heap. Redefine this macro to call any function necessary to close memory management.
#define eC_EXITMEM()
#endif

#ifndef eC_GETMEM
/* Allocates memory. Redefine this macro to call your memory allocation. Is used by <B>new</B> and <B>new[]</B> as well.
@param s Size in bytes to allocate (as eC_UInt). */
#ifdef eC_LEAKDETECTION
#define eC_GETMEM(s) eC_DbgGetMem(s, __FILE__, __LINE__)   
#else
#define eC_GETMEM(s) eC_Malloc((eC_UInt)(s))
#endif
#endif

#ifndef eC_GETMEMEXT
/* Allocates memory with a platform depending ID.
Redefine this macro to call your memory allocation. Is used by <B>new</B> and <B>new[]</B> as well.
@param s Size in bytes to allocate (as eC_UInt). */
#define eC_GETMEMEXT(s,i) eC_Malloc( (eC_UInt)(s), (eC_UByte)(i) )
#endif

#ifndef eC_FREEMEM
/* Frees memory. Redefine this macro to call your memory release. Is used by <B>delete</B> and <B>delete[]</B> as well.
@param p Pointer to the memoryblock (as void *). */ 
#ifdef eC_LEAKDETECTION
#define eC_FREEMEM(p) {eC_DebugRemItem((unsigned long)p); eC_Free(p);}
#else
#define eC_FREEMEM(p) eC_Free(p)
#endif
#endif

#ifndef eC_FREEMEMEXT
/* Frees memory with a platform depending ID. Redefine this macro to call your memory release. Is used by <B>delete</B> and <B>delete[]</B> as well.
@param p Pointer to the memoryblock (as void *). */ 
#define eC_FREEMEMEXT(p,i) eC_Free(p, (eC_UByte)(i))
#endif

//}


//------------------------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// map new/delete to getmem/freemem 

#ifdef eC_OVERLOAD_NEW_DELETE_OPERATOR
#ifdef __ICCARM__
  #include <new>
  inline void* operator new( size_t size ) throw(_XSTD bad_alloc)
  { 
    return eC_GETMEM( size ); 	
  }
  
  inline void* operator new[]( size_t size ) throw(_XSTD bad_alloc)
  { 
    return eC_GETMEM( size );
  }
  
  inline void operator delete( void *p ) throw()
  {
    eC_FREEMEM( p );
  }
  
  inline void operator delete[]( void *p ) throw()
  {
    eC_FREEMEM( p );
  }
#else //__ICCARM__
  inline void* operator new(size_t size, void* __p)  
  { 
    return __p; 
  }

  #ifdef eC_TARGET_COMPILER_MSVC
  inline void  operator delete  (void*, void*)
  { 
  }
  #endif
  
  inline void* operator new( size_t size ) 
  { 
    return eC_GETMEM( size ); 	
  }
  
  inline void* operator new[]( size_t size ) 
  { 
    return eC_GETMEM( size );
  }
  
  inline void operator delete( void *p )
  {
    eC_FREEMEM( p );
  }
  
  inline void operator delete[]( void *p )
  {
    eC_FREEMEM( p );
  }
  #endif
#endif

//---------------------------------------------------------------
#endif
