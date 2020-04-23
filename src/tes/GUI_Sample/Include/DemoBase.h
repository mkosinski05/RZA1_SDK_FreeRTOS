#if !defined _DEMOBASE_H_
#define _DEMOBASE_H_

#include "eC_String.h"

class DemoBase
{
public:
    virtual ~DemoBase()
    {
    }

    // initialization
    virtual void Init() = 0;

    // de-initialization
    virtual void DeInit() = 0;

    virtual void HandleCallAPI(const eC_String& kAPI, const eC_String kParam)
    {
    }
};

#endif
