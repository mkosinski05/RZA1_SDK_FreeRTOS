#include "CustomExtensionFactory.h"
// All custom extensions headers must be included here.
#include "CustomControlResource.h"
#include "CustomLayouterResource.h"
#include "CustomCommandResource.h"
#include "CustomBehaviourResource.h"

#include "ExampleControl.h"
#include "ExampleBehaviour.h"
#include "ExampleLayouter.h"
#include "ExampleCommand.h"
#include "CallApplicationAPICmd.h"
// Do not edit the following tags as they are used by the custom
// extension wizard.
/*<*CUSTOM_HEADERS_BLK_BEGIN*/
/*<*CUSTOM_HEADERS_BLK_END*/

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

CGUIObject* CustomExtensionFactory::CreateControl(ControlClassID_t eControlID)
{
    switch(eControlID)
    {
#define ENTRY(a, b) case a: return new b();
    CUSTOM_CONTROL_TABLE
#undef ENTRY
    default:
        break;
    }
    return NULL;
}

CGUILayouter* CustomExtensionFactory::CreateLayouter(LayouterClassID_t eLayouterID)
{
    switch(eLayouterID)
    {
#define ENTRY(a, b) case a: return new b();
    CUSTOM_LAYOUTER_TABLE
#undef ENTRY
    default:
        break;
    }
    return NULL;
}

CGUIBehaviourDecorator* CustomExtensionFactory::CreateBehaviour(BehaviourClassID_t eBehaviourID)
{
    switch(eBehaviourID)
    {
#define ENTRY(a, b) case a: return new b();
    CUSTOM_BEHAVIOUR_TABLE
#undef ENTRY
    default:
        break;
    }
    return NULL;
}

CGUICommand* CustomExtensionFactory::CreateCommand(CommandClassID_t eCommandID)
{
    switch(eCommandID)
    {
#define ENTRY(a, b) case a: return new b();
    CUSTOM_COMMAND_TABLE
#undef ENTRY
    default:
        break;
    }

    return NULL;
}

CGUIText* CustomExtensionFactory::CreateText(TextTypeID_t eTextType)
{
    return NULL;
}
