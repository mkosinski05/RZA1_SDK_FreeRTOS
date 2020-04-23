#if !defined(CUSTOM_LAYOUTER_RESOURCE_H)
#define CUSTOM_LAYOUTER_RESOURCE_H

/**
Custom layouter IDs.
Each of these IDs maps to a layouter implementation.
You may freely add and remove entries between the CUSTOM_XXX_BLK_BEGIN comments. Please use the following syntax for every entry:
ENTRY(CLASS_ID, Classname).
Please add backslash as continuation character at the end if further ENTRIES follow.
Use ENTRY_ENUM(CLASS_ID) adding entries into the enum without a corresponding class (e.g. custom extensions which are not in use anymore).
*/
#define CUSTOM_LAYOUTER_TABLE            \
ENTRY(LYT_EXAMPLE, ExampleLayouter) /* Class ID of the example behaviour declared in ExampleBehaviour.h */\
/*<*CUSTOM_LAYOUTERS_BLK_BEGIN*/\
/*<*CUSTOM_LAYOUTERS_BLK_END*/

#endif // CUSTOM_LAYOUTER_RESOURCE_H
