#if !defined EC_ATOMIC_H
#define EC_ATOMIC_H

// Abstraction for atomic operations.

#ifdef DOCXX

/** @name eC_Atomic
    @doc Abstractions for atomic operations that modify integers. Can be used
         for modifying values that are used in more than one thread.
  */

#endif // DOCXX

// ----------------------------------------------------------------------------
#if defined(eC_TARGET_CPU_X86) && defined(eC_TARGET_ENV_WIN32)

#include "x86_win32/eC_x86_win32_Atomic.h"

// ----------------------------------------------------------------------------
#elif defined(eC_TARGET_ENV_POCKET)

#include "pocket/eC_pocket_Atomic.h"

// ----------------------------------------------------------------------------
#elif defined(eC_TARGET_ENV_LINUX)

#include "linux/eC_linux_Atomic.h"

// ----------------------------------------------------------------------------
#elif defined(eC_TARGET_ENV_EMBOS)

#include "embOS/eC_embOS_Atomic.h"

#else

#include "eC_Atomic_sem.h"

#endif

#endif // EC_ATOMIC_H

