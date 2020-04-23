#if !defined EC_X86_WIN32_ATOMIC_H
#define EC_X86_WIN32_ATOMIC_H

#include "eC_Types.h"

// Win32 implementation of the atomic operations.
// Uses the InterlockedX Win32 API calls.
class eC_Atomic_win32
{
public:
    static void increment(volatile eC_Int* piAddend);
    static eC_Int decrement(volatile eC_Int* piAddend);
};

typedef eC_Atomic_win32 eC_Atomic;

#endif // EC_X86_WIN32_ATOMIC_H
