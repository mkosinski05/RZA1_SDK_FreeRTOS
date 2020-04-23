#if !defined(<*>HEADER_DEFINE<*>)
#define <*>HEADER_DEFINE<*>

#include "GUIFactory.h"

class <*>FACTORY_CLASS_NAME<*> : public CGUIFactory
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
};

#endif // <*>HEADER_DEFINE<*>
