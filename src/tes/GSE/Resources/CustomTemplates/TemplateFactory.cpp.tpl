#include "<*>FACTORY_CLASS_NAME<*>.h"
// All custom extensions headers must be included here.

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

CGUIObject* <*>FACTORY_CLASS_NAME<*>::CreateControl(
    ControlClassID_t eControlID)
{
    return NULL;
}

CGUILayouter* <*>FACTORY_CLASS_NAME<*>::CreateLayouter(
    LayouterClassID_t eLayouterID)
{
    return NULL;
}

CGUIBehaviourDecorator* <*>FACTORY_CLASS_NAME<*>::CreateBehaviour(
    BehaviourClassID_t eBehaviourID)
{
    return NULL;
}

CGUICommand* <*>FACTORY_CLASS_NAME<*>::CreateCommand(
    CommandClassID_t eCommandID)
{
    return NULL;
}
