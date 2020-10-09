#if !defined EC_ATOMIC_SEM_H
#define EC_ATOMIC_SEM_H

#include "eC_Types.h"
#include "eC_Semaphore.h"

class eC_Atomic_sem
{
public:
    static void increment(volatile eC_Int* piAddend);
    static eC_Int decrement(volatile eC_Int* piAddend);

private:
    eC_Atomic_sem();

    eC_Semaphore m_kSem;
    static eC_Atomic_sem& getInstance();
};

typedef eC_Atomic_sem eC_Atomic;

#endif // EC_ATOMIC_SEM_H
