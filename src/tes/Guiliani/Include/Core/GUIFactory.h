/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_STREAM_GUI

#ifndef GUIFACTORY__H_
#define GUIFACTORY__H_

#include "GUIBehaviourResource.h"
#include "GUICommandResource.h"
#include "GUIControlResource.h"
#include "GUILayouterResource.h"
#include "GUITextTypeResource.h"
#include "GUIAnimationResource.h"
#include "RefCounted.h"
#include "SharedPtr.h"

class CGUIText;
class CGUIObject;
class CGUILayouter;
class CGUIBehaviourDecorator;
class CGUICommand;
class CGUIAnimation;

class CGUIFactory;
/// Abstract factory pointer type.
typedef NSmartPtr::SharedPtr<CGUIFactory> CGUIFactoryPtr;

/// Interface for streaming factories.

/** Factories are used when loading streaming files.
    Each factory has to be derived from this base class.

@section sec_guifactory_overview Overview

    A factory creates GUI objects (controls), commands, layouters, behaviours
    and texts based on their IDs. The CGUIFactoryManager calls a method of
    this class, then calls ReadFromStream on the returned object. <p>
    For creating your own customized factory, you must derive it from CGUIFactory,
    implement the corresponding CreateXXX-interfaces and register it with the
    CGUIFactoryManager.

@section sec_guifactory_example Example usage

    Guiliani's standard objects are instantiated within the CGUIStandardFactory which comes with
    the framework. If you wish to add customized objects, so that they can be read from a streaming-file,
    you must implement a corresponding factory and register it with the framework. The factory's sole
    purpose is to instantiate your customized objects. This is done by mapping an ID read from a stream onto
    the matching <i>new CSomeCustomizedClass()</i> call.

    The following example code snippets demonstrate how this is done in practice.
    You must derive your customized factory from the CGUIFactory base class and re-implement those
    interfaces, for which you have created customized objects (In this example Controls and Commands).

@code
class ExampleFactory : public CGUIFactory
{
protected:
    virtual CGUIObject* CreateControl(ControlClassID_t eControlID);
    virtual CGUICommand* CreateCommand(CommandClassID_t eCommandID);
};
@endcode

    There exist dedicated methods for each of the different object types (controls, commands, layouters, behaviours and texts.)
    The factory method gets called by the framework and receives an ID, which it then uses internally within
    a switch/case statement to construct the matching object.<p>

    The following example implements the <i>CreateControl</i> interface of a customized factory named <i>ExampleFactory</i>.
    If Guiliani itself reads an ID from a stream which it did not recognize, it calls the registered UserFactory's methods,
    thus requesting them to instantiate the object identified by <i>eControlID</i>. These IDs are typically declared as
    enumerations within the UserControlResource.h / UserCommandResource.h / UserLayouterResource.h / UserBehaviourResource.h
    and UserTextTypeResource.h headers. All the factory needs to do, is to check for the ID in a switch/case, create the
    corresponding object if possible, or return NULL otherwise.<p>

@code
CGUIObject* ExampleFactory::CreateControl(ControlClassID_t eControlID)
{
    switch(eControlID)
    {
    case CTL_EXAMPLE:
        return new ExampleControl();
        break;
    default:
        break;
    }
    return NULL;
}
@endcode

    The same applies to all other types of objects, such as e.g. commands. As you can see from the following example:
@code
CGUICommand* ExampleFactory::CreateCommand(CommandClassID_t eCommandID)
{
    switch(eCommandID)
    {
    case CMD_EXAMPLE:
        return new ExampleCommand();
        break;
    default:
        break;
    }

    return NULL;
}
@endcode

    Once you have finished implementing your customized factory, use the CGUIFactoryManagers AddUserFactory interface
    to make your factory known within the framework, and you are done. The framework will now be able to use your factory to
    load your custom objects from a stream.

@code
    GETFACTORY.AddUserFactory( new ExampleFactory() );
@endcode

*/
class CGUIFactory : public NSmartPtr::RefCounted
{
protected:
    /// The factory manager needs to call the load methods.
    friend class CGUIFactoryManager;

    /// Destructor.
    virtual ~CGUIFactory() {}

    /** Creates a new GUI object.
        @param eControlID ID of the control.
        @return The object that has been created or NULL if the ID was unknown.
    */
    virtual CGUIObject* CreateControl(ControlClassID_t eControlID) { return NULL; }

    /** Creates a new layouter.
        @param eLayouterID ID of the layouter.
        @return The layouter that has been created or NULL if the ID was unknown.
    */
    virtual CGUILayouter* CreateLayouter(LayouterClassID_t eLayouterID) { return NULL; }

    /** Creates a new behaviour decorator.
        @param eBehaviourID ID of the behaviour.
        @return The behaviour that has been created or NULL if the ID was unknown.
    */
    virtual CGUIBehaviourDecorator* CreateBehaviour(BehaviourClassID_t eBehaviourID) { return NULL; }

    /** Creates a new command.
        @param eCommandID ID of the command.
        @return The command that has been created or NULL if the ID was unknown.
    */
    virtual CGUICommand* CreateCommand(CommandClassID_t eCommandID) { return NULL; }

    /** Creates a new text object.
        @param eTextType The text type ID.
        @return The text that has been created or NULL if the ID was unknown.
      */
    virtual CGUIText* CreateText(TextTypeID_t eTextType) { return NULL; }

    /** Creates a new animation object.
        @param eAnimation The animation ID.
        @return The animation that has been created or NULL if the ID was unknown.
      */
    virtual CGUIAnimation* CreateAnimation(AnimationType_t eAnimation) { return NULL; }
};
#endif
#endif
