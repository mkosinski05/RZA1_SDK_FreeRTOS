/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(STREAMRUNTIMESTARTUP_RZG_H)
#define STREAMRUNTIMESTARTUP_RZG_H

/** The NStreamRuntime Namespace contains generic helper classes for initializing a default StreamRuntime application.
    The code herein typically remains widely unchanged across most StreamRuntimes.
    Feel free to remove these classes from your own application and replace them with custom code, if you wish.*/
namespace NStreamRuntime
{
    void ConstructResourceClasses();
    void DestructResourceClasses();
    eC_Bool LoadConfiguration(int argc, char** argv);
}

#endif

