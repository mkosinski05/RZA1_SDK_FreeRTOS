/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_STREAM_GUI

#ifndef GUISTANDARDFACTORY__H_
#define GUISTANDARDFACTORY__H_

#include "GUIFactory.h"

/// Factory implementation for Guiliani standard classes.

/** This implementation creates Guiliani standard controls, commands, texts,
    layouters, behaviour decorators and animations.
*/
class CGUIStandardFactory : public CGUIFactory
{
public:
    /// Constructor.
    CGUIStandardFactory();

    /// Destructor.
    virtual ~CGUIStandardFactory();

    virtual CGUIObject* CreateControl(ControlClassID_t eControlID);

    virtual CGUILayouter* CreateLayouter(LayouterClassID_t eLayouterID);

    virtual CGUIBehaviourDecorator* CreateBehaviour(BehaviourClassID_t eBehaviourID);

    virtual CGUICommand* CreateCommand(CommandClassID_t eCommandID);

    virtual CGUIText* CreateText(TextTypeID_t eTextType);

    virtual CGUIAnimation* CreateAnimation(AnimationType_t eAnimation);

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIStandardFactory(const CGUIStandardFactory& kSource);

    /** Assignment operator.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIStandardFactory& operator=(const CGUIStandardFactory& kSource);
};
#endif
#endif
