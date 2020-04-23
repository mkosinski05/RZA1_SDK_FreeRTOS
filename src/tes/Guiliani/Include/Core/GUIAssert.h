/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIASSERT_H__
#define __GUIASSERT_H__

#if defined _DEBUG || defined DEBUG

#include "eC_Types.h"
#include "eC_String.h"
#include "GUITrace.h"
#include <cassert>

/** This is a class which is applied for debugging in the Guiliani library and applications.
    An assertion is simply a statement that a given logical criterion must hold.
    In release situation, the code will be compiled out.
    @file GUIAssert.h
*/


/** If condition is false, it will log in which file on which line the assertion failed.*/
#define GUI_ASSERT(Condition, Rationale) NGUIAssert::Assert(Condition, Rationale, eC_String( __FILE__), eC_String(__LINE__))

/// @brief Namespace for Guiliani assertions
namespace NGUIAssert
{
    /** Assert method that logs where the assertion failed and then calls assert().
        @param bCondition The condition which decides whether the assert failed or not.
        @param kExplanation A text that shall give some explanation about the assertion.
               Best is not just to describe what is assured but rather also say why the
               condition must hold and to describe on a higher level of logic than what
               the condition itself explains (please describe the rationale).
        @param kFileName the file name where the assert happens.
        @param kLineNumber The line number where the assert happens.
    */
    static void Assert(
        eC_Bool bCondition,
        const eC_String& kExplanation,
        const eC_String& kFileName,
        const eC_String& kLineNumber)
    {
        if (!bCondition)
            CGUITrace::GetInstance().Log("ASSERTION FAILED: ", kExplanation + "\n", kFileName, kLineNumber);
        assert(bCondition);
    }
};
#else // release
  #define GUI_ASSERT(Condition, Rationale) {}
#endif //#if defined _DEBUG || defined DEBUG

#endif //__GUIASSERT_H__
