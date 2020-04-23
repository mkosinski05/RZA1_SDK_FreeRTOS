/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_START_ANIMATIONS_CMD_H)
#define GUI_START_ANIMATIONS_CMD_H

#include "GUICommand.h"

/// Starts all animations chains and therefore all contained animations.
/** This command by default starts all animation chains. This will result in starting
    of all animations contained within an animation chain.
    If bStartSingleAnimationChain is true it is possible to only start the animation
    chain specified by uiAnimationChainID.

    @ingroup GUILIANI_COMMANDS
  */
class CGUIStartAnimationsCmd : public CGUICommand
{
public:

    /** Constructs an CGUIStartAnimationsCmd.
        @param bStartSingleAnimationChain True if only a single animation chain should be started
               else all chains will be started.
        @param uiAnimationChainID The chain that should be started if bStartSingleAnimationChain=true
    */
    CGUIStartAnimationsCmd(bool bStartSingleAnimationChain, eC_UInt uiAnimationChainID);

    /** Standard constructor
    */
    CGUIStartAnimationsCmd();

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    ~CGUIStartAnimationsCmd();
    virtual void Do();
private:
    bool        m_bStartSingleAnimationChain;
    eC_UInt     m_uiAnimationChainID;
};

#endif // GUI_START_ANIMATIONS_CMD_H
