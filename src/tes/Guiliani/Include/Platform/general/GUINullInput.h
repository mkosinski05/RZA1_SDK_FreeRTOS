/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUINULLINPUT__H_
#define GUINULLINPUT__H_

#include "GUIInputMedia.h"
#include "GUITrace.h"

/// This is the Null Input-Wrapper class.

/**
    It implements a null Input-Wrapper for Guiliani testing.
*/
class CGUINullInput:
    public CGUIInputMedia
{
public:
    /**
        Singleton for creating a null Input-Wrapper.
    */
    inline static void CreateInstance()
    {
        if(NULL == ms_pInputMedia)
        {
            ms_pInputMedia = new CGUINullInput();
        }
    }

    inline CGUIEvent* GetEvent(eC_UInt uiIdleTime) {return NULL;}

    virtual void StopIdle() {}

private:
    /** Constructor. */
    CGUINullInput()
    {
        GUILOG(GUI_TRACE_WARNING, "----------------------\n");
        GUILOG(GUI_TRACE_WARNING, "|  NullInput chosen  |\n");
        GUILOG(GUI_TRACE_WARNING, "----------------------\n");
    }

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUINullInput(const CGUINullInput& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUINullInput& operator=(const CGUINullInput& kSource);
};

#endif
