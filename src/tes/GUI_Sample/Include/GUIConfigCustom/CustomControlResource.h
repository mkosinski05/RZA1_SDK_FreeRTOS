#if !defined(CUSTOM_CONTROL_RESOURCE_H)
#define CUSTOM_CONTROL_RESOURCE_H

/**
Custom control IDs.
Each of these IDs maps to a control implementation.
You may freely add and remove entries between the CUSTOM_XXX_BLK_BEGIN comments. Please use the following syntax for every entry:
ENTRY(CLASS_ID, Classname).
Please add backslash as continuation character at the end if further ENTRIES follow.
Use ENTRY_ENUM(CLASS_ID) adding entries into the enum without a corresponding class (e.g. custom extensions which are not in use anymore).
*/
#define CUSTOM_CONTROL_TABLE            \
ENTRY(CTL_EXAMPLE, ExampleControl) \
/*<*CUSTOM_CONTROLS_BLK_BEGIN*/\
/*<*CUSTOM_CONTROLS_BLK_END*/

#endif // CUSTOM_CONTROL_RESOURCE_H
