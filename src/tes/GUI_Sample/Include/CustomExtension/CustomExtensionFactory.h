/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(CUSTOM_EXTENSION_FACTORY_H)
#define CUSTOM_EXTENSION_FACTORY_H

#include "GUIFactory.h"

/** An example for a custom factory.

    This factory must be able to create all custom extensions that are
    registered in the editor.
  */
class CustomExtensionFactory : public CGUIFactory
{
protected:
    /** Creates a new GUI object ('control' or 'widget').
        @param eControlID Class ID of the control.
        @return The object that has been created or NULL if the ID was unknown.
    */
    virtual CGUIObject* CreateControl(ControlClassID_t eControlID);

    /** Creates a new layouter.
        @param eLayouterID Class ID of the layouter.
        @return The layouter that has been created or NULL if the ID was unknown.
    */
    virtual CGUILayouter* CreateLayouter(LayouterClassID_t eLayouterID);

    /** Creates a new behaviour decorator.
        @param eBehaviourID Class ID of the behaviour.
        @return The behaviour that has been created or NULL if the ID was unknown.
    */
    virtual CGUIBehaviourDecorator* CreateBehaviour(BehaviourClassID_t eBehaviourID);

    /** Creates a new command.
        @param eCommandID Class ID of the command.
        @return The command that has been created or NULL if the ID was unknown.
    */
    virtual CGUICommand* CreateCommand(CommandClassID_t eCommandID);

    /** Creates a new text object.
        @param eTextType The text type ID.
        @return The text that has been created or NULL if the ID was unknown.
      */
    virtual CGUIText* CreateText(TextTypeID_t eTextType);
};

#endif // EXAMPLE_FACTORY_H
