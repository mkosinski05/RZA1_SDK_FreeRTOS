/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_NULL__H__
#define __FNT_WRAP_NULL__H__

#include "FntWrap.h"

/// The dummy font wrapper class.

/**
    This is the dummy font wrapper class.

    @ingroup GUILIANI_GRAPHICS
*/

class CFntWrapNull :
    public CFntWrap
{
public:
    /// Create the insta    nce
    inline static void CreateInstance()
    {
        if(NULL == ms_pFntWrap)
        {
            ms_pFntWrap = new CFntWrapNull();
        }
    }

protected:

    virtual void Text(
        const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString) {}

    virtual void RequiredSpace(
        const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight) {}

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts)
    {
        m_uiNOFFonts = uiNOFFonts;
        return true;
    }


 private:
    /** Copy-constructor. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapNull(const CFntWrapNull& kSource);

    /** Operator= method. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapNull& operator=(const CFntWrapNull& kSource);


    CFntWrapNull()
    {
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
        GUILOG(GUI_TRACE_WARNING, "|  FntWrapNull chosen  |\n");
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
    }
};
#endif
