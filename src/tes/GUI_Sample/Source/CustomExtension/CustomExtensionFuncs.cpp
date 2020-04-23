#include "CustomExtensionFuncs.h"
// Include any headers for custom extensions here.
#include "CustomExtensionFactory.h"
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

void GetCustomFactories(FactoryVector& rkFactories)
{
    /* Add custom factories here. Example:
       rkFactories.push_back(new MyFactory()); */

    rkFactories.push_back(new CustomExtensionFactory());
/*<*CUSTOM_FACTORIES_BLK_BEGIN*/
/*<*CUSTOM_FACTORIES_BLK_END*/
}

void GetCustomBehaviours(BehaviourVector& rkBehaviours)
{
    /* Add custom behaviours here. Example:
    rkBehaviours.push_back(
         BehaviourDescriptor(BHV_MY_BEHAVIOUR, "BHV_MY_BEHAVIOUR", new MyBehaviour(NULL)));
    */

    rkBehaviours.push_back(BehaviourDescriptor(
        BHV_EXAMPLE, "BHV_EXAMPLE", new ExampleBehaviour(ExampleBehaviour::DA_MOVE, NULL)));
/*<*CUSTOM_BEHAVIOURS_BLK_BEGIN*/
/*<*CUSTOM_BEHAVIOURS_BLK_END*/
}

void GetCustomCommands(CommandVector& rkCommands)
{
    /* Add custom commands here. Example:
    rkCommands.push_back(
         CommandDescriptor(CMD_MY_COMMAND, "CMD_MY_COMMAND", new MyCommand()));
    */

    rkCommands.push_back(CommandDescriptor(
        CMD_EXAMPLE, "CMD_EXAMPLE", new ExampleCommand(NO_HANDLE, eC_FromInt(0))));
    rkCommands.push_back(CommandDescriptor(
        CMD_CALLAPPLICATIONAPI, "CMD_CALLAPPLICATIONAPI", new CallApplicationAPICmd("Enter API", "Enter Parameter")));
/*<*CUSTOM_COMMANDS_BLK_BEGIN*/
/*<*CUSTOM_COMMANDS_BLK_END*/
}

void GetCustomControls(ControlVector& rkControls)
{
    /* Add custom controls here. Example:
    rkControls.push_back(
         ControlDescriptor(CNT_MY_CONTROL, "my control",
             new MyControl(NULL, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100))));
    */

    rkControls.push_back(ControlDescriptor(CTL_EXAMPLE, "example control",
        new ExampleControl(NULL, eC_FromInt(0), eC_FromInt(0), eC_FromInt(100), eC_FromInt(100),
            0xFFFFFFFF, 0xFF000000, eC_FromInt(2))));
/*<*CUSTOM_CONTROLS_BLK_BEGIN*/
/*<*CUSTOM_CONTROLS_BLK_END*/
}

void GetCustomLayouters(LayouterVector& rkLayouters)
{
    /* Add custom layouters here. Example:
    rkLayouters.push_back(
         LayouterDescriptor(LYT_MY_LAYOUTER, "LYT_MY_LAYOUTER", new MyLayouter()));
    */

    rkLayouters.push_back(LayouterDescriptor(LYT_EXAMPLE, "LYT_EXAMPLE", new ExampleLayouter(false, false)));
/*<*CUSTOM_LAYOUTERS_BLK_BEGIN*/
/*<*CUSTOM_LAYOUTERS_BLK_END*/
}

void GetCustomEnumMappings(EnumMappingVector& rkEnumMappings)
{
    /* Add enum mappings for custom behaviours, controls, commands and layouters
       here. Example: your control 'MyControl' has an enum type 'MyEnum',
       values of which are written into streams with the XML tag 'MyEnumValue'.
       The enum values defined in the control are 'MY_VALUE_1' and 'MY_VALUE_2'.
       You would then add mappings as follows:
    rkEnumMappings.push_back(EnumMapping("MyEnumValue", MyControl::MY_VALUE_1, "MY_VALUE_1"));
    rkEnumMappings.push_back(EnumMapping("MyEnumValue", MyControl::MY_VALUE_2, "MY_VALUE_2"));
    */

    // Add the values for the DragAction enum in the ExampleBehaviour.
    rkEnumMappings.push_back(EnumMapping("DragAction", ExampleBehaviour::DA_MOVE, "DA_MOVE"));
    rkEnumMappings.push_back(EnumMapping("DragAction", ExampleBehaviour::DA_SIZE, "DA_SIZE"));
}

void GetCustomRepetitions(RepetitionVector& rkRepetitions)
{
    /* Add attribute repetition mappings for custom attribute repetitions here. 
       Example: your repetition 'NumberOfImages' has one containing attribute value with XMLTag "ImageID"
       of attribute type AT_IMAGEID.
       Example:
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "KeyID", "", AttributeRepetitionDescriptor::AT_INT));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "KeyModifiers", "0", AttributeRepetitionDescriptor::AT_INT));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "CheckForModifiers", "false", AttributeRepetitionDescriptor::AT_BOOL));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "IsActivated", "true", AttributeRepetitionDescriptor::AT_BOOL));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "MappedObjectID", "NO_HANDLE", AttributeRepetitionDescriptor::AT_OBJECTID));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("", "ObjectIsAbstract", "false", AttributeRepetitionDescriptor::AT_BOOL));
    rkRepetitions.push_back(AttributeRepetitionDescriptor("NumberOfKeyMappings", "", "", AttributeRepetitionDescriptor::AT_COMMENT));
    */
}

