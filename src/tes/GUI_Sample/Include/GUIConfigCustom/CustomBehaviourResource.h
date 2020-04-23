#if !defined(CUSTOM_BEHAVIOUR_RESOURCE_H)
#define CUSTOM_BEHAVIOUR_RESOURCE_H

/**
Custom behavior IDs.
Each of these IDs maps to a behavior implementation.
You may freely add and remove entries between the CUSTOM_XXX_BLK_BEGIN comments. Please use the following syntax for every entry:
ENTRY(CLASS_ID, Classname).
Please add backslash as continuation character at the end if further ENTRIES follow.
Use ENTRY_ENUM(CLASS_ID) adding entries into the enum without a corresponding class (e.g. custom extensions which are not in use anymore).
*/
#define CUSTOM_BEHAVIOUR_TABLE            \
ENTRY(BHV_EXAMPLE, ExampleBehaviour) /* Class ID of the example behaviour declared in ExampleBehaviour.h */\
/*<*CUSTOM_BEHAVIOURS_BLK_BEGIN*/\
/*<*CUSTOM_BEHAVIOURS_BLK_END*/


#endif // CUSTOM_BEHAVIOUR_RESOURCE_H
