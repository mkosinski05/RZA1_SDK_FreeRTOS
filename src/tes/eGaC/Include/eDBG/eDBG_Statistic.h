/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eDBG
* file name:       eDBG_Statistic.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:09:26 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef eDBG_STATISTIC__H_
#define eDBG_STATISTIC__H_

//---------------------------------------------------------------
#include "eDBG_Config.h"

//-----------------------------------------------------------------------------------------------
/**@name eDBG_Statistics
@memo
Statistic counters.
@doc
When enabled within <I>eDBG_Config.h</I> file ("<I>$(eGML_ROOT)/comps/eDBG/code</I>" folder), additional statistic gathering macros
are available. The necessary code will be removed at compiletime when \Ref{eDBG_STATISTIC} is undefined. Statistic counters are very similar
to global variables, each one must be 'created' in exactly one sourcefile and can be 'defined' (used) in any number of header and source files.
When using a compiler (or linker) that does not support global construction the \Ref{eDBG_AUTOREGISTERSTAT} define has to be removed
from <I>eDBG_Config.h</I> and an additional step is necessary to make use of statistic counters. <BR>
Namely 'registering' them. This is very rare though. Each statistic counter has a 'priority', a 'name' and a type. When outputting counters to the
debug out or tracking them in realtime the priority is used to select the counters of interest. <BR>
To get more information about {@link eDBG_Statistics statistics}, look at the \Ref{Tutorial_eDBG}.
@see eDBG_STATISTIC, eDBG_AUTOREGISTERSTAT
@see Tutorial_eDBG
*/

//@{

//-----------------------------------------------------------------------------------------------
#ifndef eDBG_STATISTIC

// Dummy definition to avoid warnings at the MIPS_rr compiler
#define eDBG__NOP_ class hannibal


/** Creates a simple counter.
Every statistic counter has to be created within one sourcefile (analog to a global identifier). The extension '<B>_S</B>' is short for 'simple' and
specifies the counter type to be created. Simple counters store an accumulated value (modified by \Ref{eDBG_STAT_ADD} and
\Ref{eDBG_STAT_SET}) and it's average over all frames.
@param pri Priority of the counter (\Ref{eC_UInt} format, 0 for most basic counters, higher for more specific ones).
@param name Case sensitive name of counter (<I>must</I> be unique, is <I>not</I> a string - just like varaible name).
@see eDBG_STAT_CREATE_R, eDBG_STAT_DEF */
#define eDBG_STAT_CREATE_S(pri, name) eDBG__NOP_

/** Creates a range counter.
Every statistic counter has to be created within one sourcefile (analog to a global identifier). The extension '<B>_R</B>' is short for 'range' and
specifies the counter type to be created. Range counters recieve a series of values via \Ref{eDBG_STAT_ADD} and store the average,
minimum and maximum values of them.
@param pri Priority of the counter (\Ref{eC_UInt} format, 0 for most basic counters, higher for more specific ones).
@param name Case sensitive name of counter (<I>must</I> be unique, is <I>not</I> a string - just like varaible name).
@see eDBG_STAT_CREATE_S, eDBG_STAT_DEF */
#define eDBG_STAT_CREATE_R(pri, name) eDBG__NOP_

/** Increments a counter.
Depending on the counter type <I>add</I> is used to increment the current counter value or add a new value to the series. You also can
decrement your counter, if you set a negative value for the parameter.
Use \Ref{eDBG_STAT_SET} to reset a counter.
@param name Case sensitive name of counter to modify.
@param value Value to add (\Ref{eC_Float} format).
@see eDBG_STAT_SET */
#define eDBG_STAT_ADD(name, value)

/** Sets a counter.
Depending on the counter type <I>set</I> is used to reset the current counter value or remove all values from the current statisitc series.
Use \Ref{eDBG_STAT_ADD} to modify a counter.
@param name Case sensitive name of counter to modify.
@param value Value to set (\Ref{eC_Float} format).
@see eDBG_STAT_ADD */
#define eDBG_STAT_SET(name, value)

/** Defines statistic counter.
To use a counter created within another source or header file you have to 'define' it. This is very similar to an <B>extern</B> definition.
<TT>eDBG_STAT_DEF</TT> must be placed outside of function bodies (analog to extern declarations).
@param name Case sensitive name of counter to define (it is <I>not</I> a string - just like varaible name).
@see eDBG_STAT_CREATE_S, eDBG_STAT_CREATE_R */
#define eDBG_STAT_DEF(name) eDBG__NOP_

/* !!! NOT FINISHED !!! for Release 1.3
Initialises statistic counter. */
#define eDBG_STAT_INIT()

/* !!! NOT FINISHED !!! for Release 1.3
Shuts statistic counter down. */
#define eDBG_STAT_SHUTDOWN()

/** Indicates next frame.
Since many counters gather information not only absolute but also on a per frame basis it is necessary to call the
<TT>eDBG_STAT_NEXTFRAME</TT> macro somewhere within your mainloop - usually right before or after calling <TT><B>Flip</B></TT>
method of \Ref{eGML_Screen} class. Using statistics without providing information about frames is possible but will yield meaningless results
for all 'per frame' averages. */
#define eDBG_STAT_NEXTFRAME()

/** Outputs statistics to LOG target.
All counters with a priority level lower or equal to <TT>maxpri</TT> parameter will be printed out using \Ref{eDBG_DP} as well as
all registered variables to watch for (refer to \Ref{eDBG_STAT_GROUP_SHOW}).
This function is designed to be used before terminating the application, but outputting on a regular basis is of course possible as well.
To watch counters in realtime use \Ref{eDBG_STAT_ONSCREEN} or \Ref{eDBG_STAT_ONSCREEN2}.
@param maxpri Highest priority to include into the output (\Ref{eC_Short} format, passing 0 will output only level 0 counters).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_ONSCREEN, eDBG_STAT_ONSCREEN2 */
#define eDBG_STAT_SHOW(maxpri)

/** Outputs statistics of a group to LOG target.
All variables registered by <TT><B>eDBG_WATCH_xxx</B></TT> macros will be printed out using \Ref{eDBG_DP}.
This function is designed to be used before terminating the application, but outputting on a regular basis is of course possible as well.
To watch the variables in realtime use \Ref{eDBG_STAT_GROUP_ONSCREEN} or \Ref{eDBG_STAT_GROUP_ONSCREEN2}.
@param group Debug group, choose <TT><B>eGML_STATGROUP_WATCH</B></TT> as value (refer to <TT>eDBG_WATCH_xxx</TT> macros).
@see eDBG_STAT_SHOW, eDBG_STAT_GROUP_ONSCREEN, eDBG_STAT_GROUP_ONSCREEN2 */
#define eDBG_STAT_GROUP_SHOW(group)

/** Outputs statistics to a bitmap or screen.
To track statistic counters <I>and</I> variables you want to watch (refer to \Ref{eDBG_STAT_GROUP_ONSCREEN}) for in realtime,
you can print their current values into an \Ref{eGML_Bitmap} or \Ref{eGML_Screen}. Using this function onto the screen right before
the <TT><B>Flip</B></TT> method of eGML_Screen class is called, will ensure the output is not overdrawn by anything else.
To improve the contrast you can use the \Ref{eDBG_STAT_ONSCREEN2} macro.
@param bitmap Pointer to screen or bitmap to render into.
@param minpri Lowest priority level to include into the output (\Ref{eC_Short} format).
@param maxpri Highest priority level to include into the output (\Ref{eC_Short} format).
@param origin Specifies one screen corner to print out. Possible values are:
<UL>
<LI><TT>eDBG_Statistic::O_UL</TT> (upper left)</LI>
<LI><TT>eDBG_Statistic::O_LL</TT> (lower left)</LI>
<LI><TT>eDBG_Statistic::O_UR</TT> (upper right)</LI>
<LI><TT>eDBG_Statistic::O_LR</TT> (lower right)</LI>
</UL>
Note: <TT>eDBG_Statistic</TT> is an internal class and is <I>not</I> documented.
@param offsetX X-axis offset in pixels (\Ref{eC_UInt} format).
@param offsetY Y-axis offset in pixels (\Ref{eC_UInt} format).
@param color Direct RGB32 text color (\Ref{eC_UInt} format, no need of conversion).
@see eDBG_STAT_SHOW, eDBG_STAT_ONSCREEN2, eDBG_STAT_GROUP_ONSCREEN */
#define eDBG_STAT_ONSCREEN(bitmap, minpri, maxpri, origin, offsetX, offsetY, color)

/** Outputs statistics of a group to bitmap or screen.
To track variables you want to watch for in realtime, you can print their current values into an \Ref{eGML_Bitmap} or \Ref{eGML_Screen}.
Using this function onto the screen right before the <TT><B>Flip</B></TT> method of eGML_Screen class is called,
will ensure the output is not overdrawn by anything else. To improve the contrast you can use the \Ref{eDBG_STAT_ONSCREEN2} macro.
When using this macro you will see a list of all variables registered by <TT><B>eDBG_WATCH_xxx</B></TT> macros and their current values.
@param bitmap Pointer to screen or bitmap to render into.
@param group Debug group, choose <TT><B>eGML_STATGROUP_WATCH</B></TT> as value (refer to <TT>eDBG_WATCH_xxx</TT> macros).
@param origin Specifies one screen corner to print out. Possible values are:
<UL>
<LI><TT>eDBG_Statistic::O_UL</TT> (upper left)</LI>
<LI><TT>eDBG_Statistic::O_LL</TT> (lower left)</LI>
<LI><TT>eDBG_Statistic::O_UR</TT> (upper right)</LI>
<LI><TT>eDBG_Statistic::O_LR</TT> (lower right)</LI>
</UL>
Note: <TT>eDBG_Statistic</TT> is an internal class and is <I>not</I> documented.
@param offsetX X-axis offset in pixels (\Ref{eC_UInt} format).
@param offsetY Y-axis offset in pixels (\Ref{eC_UInt} format).
@param color Direct RGB32 text color (\Ref{eC_UInt} format, no need of conversion).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN2, eDBG_STAT_ONSCREEN */
#define eDBG_STAT_GROUP_ONSCREEN(bitmap, group, origin, offsetX, offsetY, color)

/** Outputs statistics to bitmap or screen with contrast effect.
To track statistic counters <I>and</I> variables you want to watch (refer to \Ref{eDBG_STAT_GROUP_ONSCREEN}) for in realtime,
you can print their current values into an \Ref{eGML_Bitmap} or \Ref{eGML_Screen}. Using this function onto the screen right before
the <TT><B>Flip</B></TT> method of eGML_Screen class is called, will ensure the output is not overdrawn by anything else.
To improve the contrast you can use the \Ref{eDBG_STAT_ONSCREEN2} macro. Additionally this macro performs contrast effect by the second
color parameter, when displaying the data. This 'shadow' color improves readability on certain combinations of font/background colors.
@param bitmap Pointer to screen or bitmap to render into.
@param minpri Lowest priority level to include into the output (\Ref{eC_Short} format).
@param maxpri Highest priority level to include into the output (\Ref{eC_Short} format).
@param origin Specifies one screen corner to print out. Possible values are:
<UL>
<LI><TT>eDBG_Statistic::O_UL</TT> (upper left)</LI>
<LI><TT>eDBG_Statistic::O_LL</TT> (lower left)</LI>
<LI><TT>eDBG_Statistic::O_UR</TT> (upper right)</LI>
<LI><TT>eDBG_Statistic::O_LR</TT> (lower right)</LI>
</UL>
Note: <TT>eDBG_Statistic</TT> is an internal class and is <I>not</I> documented.
@param offsetX X-axis offset in pixels (\Ref{eC_UInt} format).
@param offsetY Y-axis offset in pixels (\Ref{eC_UInt} format).
@param color1 Direct RGB32 text color (\Ref{eC_UInt} format, no need of conversion).
@param color2 Direct RGB32 shadow color (\Ref{eC_UInt} format, no need of conversion).
@see eDBG_STAT_SHOW, eDBG_STAT_ONSCREEN, eDBG_STAT_GROUP_ONSCREEN2 */
#define eDBG_STAT_ONSCREEN2(bitmap, minpri, maxpri, origin, offsetX, offsetY, color, color2)

/** Outputs statistics of a group to bitmap or screen with contrast effect.
To track variables you want to watch for in realtime, you can print their current values into an \Ref{eGML_Bitmap} or \Ref{eGML_Screen}.
Using this function onto the screen right before the <TT><B>Flip</B></TT> method of eGML_Screen class is called,
will ensure the output is not overdrawn by anything else. To improve the contrast you can use the \Ref{eDBG_STAT_ONSCREEN2} macro.
When using this macro you will see a list of all variables registered by <TT><B>eDBG_WATCH_xxx</B></TT> macros and their current values.
Additionally this macro performs contrast effect by the second color parameter, when displaying the data.
This 'shadow' color improves readability on certain combinations of font/background colors.
@param bitmap Pointer to screen or bitmap to render into.
@param group Debug group, choose <TT><B>eGML_STATGROUP_WATCH</B></TT> as value (refer to <TT>eDBG_WATCH_xxx</TT> macros).
@param origin Specifies one screen corner to print out. Possible values are:
<UL>
<LI><TT>eDBG_Statistic::O_UL</TT> (upper left)</LI>
<LI><TT>eDBG_Statistic::O_LL</TT> (lower left)</LI>
<LI><TT>eDBG_Statistic::O_UR</TT> (upper right)</LI>
<LI><TT>eDBG_Statistic::O_LR</TT> (lower right)</LI>
</UL>
Note: <TT>eDBG_Statistic</TT> is an internal class and is <I>not</I> documented.
@param offsetX X-axis offset in pixels (\Ref{eC_UInt} format).
@param offsetY Y-axis offset in pixels (\Ref{eC_UInt} format).
@param color1 Direct RGB32 text color (\Ref{eC_UInt} format, no need of conversion).
@param color2 Direct RGB32 shadow color (\Ref{eC_UInt} format, no need of conversion).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN, eDBG_STAT_ONSCREEN2 */
#define eDBG_STAT_GROUP_ONSCREEN2(bitmap, group, origin, offsetX, offsetY, color1, color2)

/** Registers a simple counter.
When using a compiler (or linker) that does not support global construction and \Ref{eDBG_AUTOREGISTERSTAT} is undefined,
you have to manually register each counter (somewhere within your startup code, counters will not work until registered).
@param pri Priority of the counter (\Ref{eC_UInt} format, 0 for most basic counters, higher for more specific ones).
@param name Case sensitive name of counter.
@see eDBG_STAT_DEF, eDBG_STAT_CREATE_S, eDBG_STAT_REGISTER_R */
#define eDBG_STAT_REGISTER_S(pri, name)

/** Registers a range counter.
When using a compiler (or linker) that does not support global construction and \Ref{eDBG_AUTOREGISTERSTAT} is undefined,
you have to manually register each counter (somewhere within your startup code, counters will not work until registered).
@param pri Priority of the counter (\Ref{eC_UInt} format, 0 for most basic counters, higher for more specific ones).
@param name Case sensitive name of counter.
@see eDBG_STAT_DEF, eDBG_STAT_CREATE_R, eDBG_STAT_REGISTER_S */
#define eDBG_STAT_REGISTER_R(pri, name)

/* !!! NOT FINISHED !!! for Release 1.3
Define performance counter.
@param name */
#define eDBG_PERF_DEF(name) eDBG__NOP_

/* !!! NOT FINISHED !!! for Release 1.3
Create a performance counter.
@param pri
@param name */
#define eDBG_PERF_CREATE(pri, name)  eDBG__NOP_

/** Creates a performance counter (timer).
Every timer has to be created within one sourcefile (analog to a global identifier). The extension '<B>_H</B>' is short for 'history' and
specifies the timer type to be created. History timers store their instantaneous values (like a history) for continuing time measurements
after interruption (e. g. you want to use the timer inside of loops). After creating a timer, you have to register it
(refer to \Ref{eDBG_PERF_REGISTER_H}).
@param pri Number of the timer (\Ref{eC_UInt} format, choose '<TT><B>eGML_STATLEVEL_WATCH</B></TT> + number' as parameter, starting with 0).
@param name Case sensitive name of timer (<I>must</I> be unique, is <I>not</I> a string - just like varaible name).
@see eDBG_PERF_REGISTER_H */
#define eDBG_PERF_CREATE_H(pri, name)  eDBG__NOP_

/* !!! NOT FINISHED !!! for Release 1.3
Create a performance counter.
@param pri
@param name */
#define eDBG_PERF_CREATE_REAL(pri, name)  eDBG__NOP_

/** Starts performance counter (timer).
For gathering performance of your code or some code passages, you have to mark start and end of measurements.
With this macro you start the timer or you start the timer again after interruption. All historical data are stored, so the timer will continue
with the same status on last call of \Ref{eDBG_PERF_END}.
@param name Case sensitive name of timer.
@see eDBG_PERF_END */
#define eDBG_PERF_BEGIN(name) 

/** stops performance counter (timer).
For gathering performance of your code or some code passages, you have to mark start and end of measurements.
With this macro you stop the timer or you just interrupt the current measurement. All historical data are stored, so the timer will continue
with its current status, when using \Ref{eDBG_PERF_BEGIN} again.
@param name Case sensitive name of timer.
@see eDBG_PERF_BEGIN */
#define eDBG_PERF_END(name)

/* !!! NOT FINISHED !!! for Release 1.3
Registers a performance counter.
@param pri
@param name */
#define eDBG_PERF_REGISTER(pri, name)

/** Registers a performance counter (timer).
You have to register each performance counter (somewhere within your startup code, timers will not work until registered), after creating it.
@param pri Number of the timer (\Ref{eC_UInt} format, choose '<TT><B>eGML_STATLEVEL_WATCH</B></TT> + number' as parameter, starting with 0).
@param name Case sensitive name of timer.
@see eDBG_PERF_CREATE_H */
#define eDBG_PERF_REGISTER_H(pri, name)

/** Watches at an eC_Value.
This macro registers a variable with <TT><B>eC_Value</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_Value} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_VALUE(value)

/** Watches at an eC_Float value.
This macro registers a variable with <TT><B>eC_Float</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_Float} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_FLOAT(value)

/** Watches at an eC_Fixed value.
This macro registers a variable with <TT><B>eC_Fixed</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_Fixed} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_FIXED(value)

/** Watches at an eC_UShort value.
This macro registers a variable with <TT><B>eC_UShort</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_UShort} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_SHORT(value)

/** Watches at an eC_UByte value.
This macro registers a variable with <TT><B>eC_UByte</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_UByte} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_BYTE(value)

/** Watches at an eC_Bool value.
This macro registers a variable with <TT><B>eC_Bool</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_Bool} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_BOOL(value)

/** Watches at a double value.
This macro registers a variable with <TT><B>double</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (double format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_DOUBLE(value)

/** Watches at an eC_UInt value.
This macro registers a variable with <TT><B>eC_UInt</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_UInt} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_UINT(value)

/** Watches at an eC_Int value.
This macro registers a variable with <TT><B>eC_Int</B></TT> format, for output on a LOG target and/or on a bitmap or screen.
Every variable registered by <TT>eDBG_WATCH_VALUE</TT> is automatically member of the <TT><B>eGML_STATGROUP_WATCH</B></TT>
group. This group administrates internally the collection of all registered variables. It is not accessable by the user, but the macros has to be
called when displaying datas on the screen or LOG target.
@param value Value to register for watching (\Ref{eC_Int} format).
@see eDBG_STAT_GROUP_SHOW, eDBG_STAT_GROUP_ONSCREEN*/
#define eDBG_WATCH_INT(value)

/** Outputs statistics of a group to bitmap or screen as a graphical bar.
To track performance measurements in realtime, you can show their current results on an \Ref{eGML_Bitmap} or \Ref{eGML_Screen}.
Using this function onto the screen right before the <TT><B>Flip</B></TT> method of eGML_Screen class is called,
will ensure the output is not overdrawn by anything else. When calling this macro you will see a bar showing time slices of each performance
counter (timer) thats created and registered. So you can see performance qualities relative to each other. If you want to display overhead
or unknown time slices you have to set the parameter <TT>full</TT> to value <B>true</B>. <BR>
Every timer created by \Ref{eDBG_PERF_CREATE_H} and  registered by \Ref{eDBG_PERF_REGISTER_H} is automatically member
of the <TT><B>eGML_STATGROUP_WATCH_ALL</B></TT> group. This group administrates internally the collection of all registered timers.
It is not accessable by the user, but the macros has to be called when displaying a runtime bar on the screen.
@param bitmap Pointer to screen or bitmap to render into.
@param group Debug group, choose <TT><B>eGML_STATGROUP_WATCH_ALL</B></TT> as value.
@param origin Specifies alignment of the visible bar on the screen. Possible values are:
<UL>
<LI><TT>eDBG_Statistic::A_LEFT</TT> (left)</LI>
<LI><TT>eDBG_Statistic::A_RIGHT</TT> (right)</LI>
</UL>
Note: <TT>eDBG_Statistic</TT> is an internal class and is <I>not</I> documented.
@param offsetX X-axis offset of the complete screen in pixels (\Ref{eC_UInt} format).
@param offsetY Y-axis offset of the complete screen in pixels (\Ref{eC_UInt} format).
@param width Width of the graphical bar.
@param height Height of the graphical bar.
@param full Must be set to <B>true</B> if complete time (including overhead and unknown time) should be displayed.
@param color Direct RGB32 text color (\Ref{eC_UInt} format, no need of conversion).
@see eDBG_STAT_GROUP_ONSCREEN */
#define eDBG_PERF_ONSCREEN(bitmap, group, origin, offsetX, offsetY, width, height, full, color)

//@}

//---------------------------------------------------------------
#else
#include "eC_Platform.h"
#include "eC_TList.h"
#include "eC_TArray.h"
#include "eC_String.h"

//---------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!

#define eDBG_STAT_DEF(name)               extern eDBG_StatCounter *g_kStat_##name
#ifdef eDBG_AUTOREGISTERSTAT
#define eDBG_STAT_CREATE_S(pri, name)     eDBG_StatCounter * g_kStat_##name = g_kStatistic.AddCounter( new eDBG_StatCounterS(pri, #name ) )
#define eDBG_STAT_CREATE_R(pri, name)     eDBG_StatCounter * g_kStat_##name = g_kStatistic.AddCounter( new eDBG_StatCounterR(pri, #name ) )
#define eDBG_STAT_REGISTER_S(pri, name)
#define eDBG_STAT_REGISTER_R(pri, name)
#define eDBG_STAT_ADD(name, value)        if (g_kStat_##name) (g_kStat_##name)->Add( (eC_Float)(value) )
#define eDBG_STAT_SET(name, value)        if (g_kStat_##name) (g_kStat_##name)->Set( (eC_Float)(value) )
#else
#define eDBG_STAT_CREATE_S(pri, name)     eDBG_StatCounter * g_kStat_##name = NULL
#define eDBG_STAT_CREATE_R(pri, name)     eDBG_StatCounter * g_kStat_##name = NULL
#define eDBG_STAT_REGISTER_S(pri, name)   g_kStat_##name = g_pkStatistic->AddCounter( new eDBG_StatCounterS(pri, #name ) )
#define eDBG_STAT_REGISTER_R(pri, name)   g_kStat_##name = g_pkStatistic->AddCounter( new eDBG_StatCounterR(pri, #name ) )
#define eDBG_STAT_ADD(name, value)        if (g_kStat_##name) (g_kStat_##name)->Add( (eC_Float)(value) ); else 
#define eDBG_STAT_SET(name, value)        if (g_kStat_##name) (g_kStat_##name)->Set( (eC_Float)(value) ); else
#endif

#define eDBG_PERF_DEF(name)               extern eDBG_PerfCounter *g_kPerf_##name
#ifdef eDBG_AUTOREGISTERSTAT
#define eDBG_PERF_CREATE(pri, name)       eDBG_PerfCounter * g_kPerf_##name = g_kStatistic.AddTimer( new eDBG_PerfCounter(pri, #name ) )
#define eDBG_PERF_CREATE_H(pri, name)     eDBG_PerfCounter * g_kPerf_##name = g_kStatistic.AddTimer( new eDBG_PerfCounterH(pri, #name ) )
#define eDBG_PERF_CREATE_REAL(pri, name)  eDBG_PerfCounter * g_kPerf_##name = g_kStatistic.AddTimer( new eDBG_PerfCounterReal(pri, #name ) )
#define eDBG_PERF_REGISTER(pri, name)
#define eDBG_PERF_REGISTER_H(pri, name)
#define eDBG_PERF_BEGIN(name)             if (g_kPerf_##name) (g_kPerf_##name)->Set( 0.0f )
#define eDBG_PERF_END(name)               if (g_kPerf_##name) (g_kPerf_##name)->Add( 1.0f )
#else
#define eDBG_PERF_CREATE(pri, name)       eDBG_PerfCounter * g_kPerf_##name = NULL
#define eDBG_PERF_CREATE_H(pri, name)     eDBG_PerfCounter * g_kPerf_##name = NULL
#define eDBG_PERF_CREATE_REAL(pri, name)  eDBG_PerfCounter * g_kPerf_##name = NULL
#define eDBG_PERF_REGISTER(pri, name)     g_kPerf_##name = g_pkStatistic->AddTimer( new eDBG_PerfCounter(pri, #name ) )
#define eDBG_PERF_REGISTER_H(pri, name)   g_kPerf_##name = g_pkStatistic->AddTimer( new eDBG_PerfCounterH(pri, #name ) )
#define eDBG_PERF_BEGIN(name)             if (g_kPerf_##name) (g_kPerf_##name)->Set( 0.0f ); else
#define eDBG_PERF_END(name)               if (g_kPerf_##name) (g_kPerf_##name)->Add( 1.0f ); else
#endif

#ifdef eDBG_AUTOREGISTERSTAT
#define eDBG_STAT_INIT()
#define eDBG_STAT_NEXTFRAME()             g_kStatistic.NextFrame()
#define eDBG_STAT_SHOW(maxpri)            g_kStatistic.Show( eDBG_Group(eDBG_Range(0,maxpri)) )
#define eDBG_STAT_GROUP_SHOW(group)       g_kStatistic.Show( group )
#define eDBG_STAT_SHUTDOWN()
#define eDBG_WATCH_VALUE(value)           g_kStatistic.AddCounter( new eDBG_StatWatchV(0, #value, &value) )
#define eDBG_WATCH_FLOAT(value)           g_kStatistic.AddCounter( new eDBG_StatWatchF(0, #value, &value) )
#define eDBG_WATCH_FIXED(value)           g_kStatistic.AddCounter( new eDBG_StatWatchFX(0, #value, &value) )
#define eDBG_WATCH_BYTE(value)            g_kStatistic.AddCounter( new eDBG_StatWatchUB(0, #value, &value) )
#define eDBG_WATCH_SHORT(value)           g_kStatistic.AddCounter( new eDBG_StatWatchUS(0, #value, &value) )
#define eDBG_WATCH_BOOL(value)            g_kStatistic.AddCounter( new eDBG_StatWatchB(0, #value, &value) )
#define eDBG_WATCH_INT(value)             g_kStatistic.AddCounter( new eDBG_StatWatchI(0, #value, &value) )
#define eDBG_WATCH_UINT(value)            g_kStatistic.AddCounter( new eDBG_StatWatchUI(0, #value, &value) )
#define eDBG_WATCH_DOUBLE(value)          g_kStatistic.AddCounter( new eDBG_StatWatchDouble(0, #value, &value) )
#ifdef eDBG_EGMLOUTPUT
#define eDBG_STAT_ONSCREEN(bitmap, minpri, maxpri, origin, offsetX, offsetY, color)              g_kStatistic.ShowOnScreen(bitmap,eDBG_Group(eDBG_Range(minpri,maxpri)),origin,offsetX,offsetY,color)
#define eDBG_STAT_ONSCREEN2(bitmap, minpri, maxpri, origin, offsetX, offsetY, color1, color2)    g_kStatistic.ShowOnScreen(bitmap,eDBG_Group(eDBG_Range(minpri,maxpri)),origin,offsetX,offsetY,color1,color2)
#define eDBG_STAT_GROUP_ONSCREEN(bitmap, group, origin, offsetX, offsetY, color)                 g_kStatistic.ShowOnScreen(bitmap,group,origin,offsetX,offsetY,color)
#define eDBG_STAT_GROUP_ONSCREEN2(bitmap, group, origin, offsetX, offsetY, color1, color2)       g_kStatistic.ShowOnScreen(bitmap,group,origin,offsetX,offsetY,color1,color2)
#define eDBG_PERF_ONSCREEN(bitmap, group, origin, offsetX, offsetY, width, height, full, color)  g_kStatistic.BarOnScreen(bitmap, group, origin, offsetX, offsetY, width, height, full, color)
#else
#define eDBG_STAT_ONSCREEN(bitmap, minpri, maxpri, origin, offsetX, offsetY, color)
#define eDBG_STAT_ONSCREEN2(bitmap, minpri, maxpri, origin, offsetX, offsetY, color1, color2)
#define eDBG_STAT_GROUP_ONSCREEN(bitmap, group, origin, offsetX, offsetY, color)
#define eDBG_STAT_GROUP_ONSCREEN2(bitmap, group, origin, offsetX, offsetY, color1, color2)
#define eDBG_PERF_ONSCREEN(bitmap, group, origin, offsetX, offsetY, width, height, full, color)
#endif
#else
#define eDBG_STAT_INIT()                  g_pkStatistic = new eDBG_Statistic
#define eDBG_STAT_NEXTFRAME()             g_pkStatistic->NextFrame()
#define eDBG_STAT_SHOW(maxpri)            g_pkStatistic->Show( eDBG_Group(eDBG_Range(0,maxpri)) )
#define eDBG_STAT_GROUP_SHOW(group)       g_pkStatistic->Show( group )
#define eDBG_STAT_SHUTDOWN()              delete g_pkStatistic
#define eDBG_WATCH_VALUE(value)           g_pkStatistic->AddCounter( new eDBG_StatWatchV(0, #value, &value) )
#define eDBG_WATCH_FLOAT(value)           g_pkStatistic->AddCounter( new eDBG_StatWatchF(0, #value, &value) )
#define eDBG_WATCH_FIXED(value)           g_pkStatistic->AddCounter( new eDBG_StatWatchFX(0, #value, &value) )
#define eDBG_WATCH_BYTE(value)            g_pkStatistic->AddCounter( new eDBG_StatWatchUB(0, #value, &value) )
#define eDBG_WATCH_SHORT(value)           g_pkStatistic->AddCounter( new eDBG_StatWatchUS(0, #value, &value) )
#define eDBG_WATCH_BOOL(value)            g_pkStatistic->AddCounter( new eDBG_StatWatchB(0, #value, &value) )
#define eDBG_WATCH_INT(value)             g_pkStatistic->AddCounter( new eDBG_StatWatchI(0, #value, &value) )
#define eDBG_WATCH_UINT(value)            g_pkStatistic->AddCounter( new eDBG_StatWatchUI(0, #value, &value) )
#define eDBG_WATCH_DOUBLE(value)          g_pkStatistic->AddCounter( new eDBG_StatWatchDouble(0, #value, &value) )
#ifdef eDBG_EGMLOUTPUT
#define eDBG_STAT_ONSCREEN(bitmap, minpri, maxpri, origin, offsetX, offsetY, color)              g_pkStatistic->ShowOnScreen(bitmap,eDBG_Group(eDBG_Range(minpri,maxpri)),origin,offsetX,offsetY,color)
#define eDBG_STAT_ONSCREEN2(bitmap, minpri, maxpri, origin, offsetX, offsetY, color1, color2)    g_pkStatistic->ShowOnScreen(bitmap,eDBG_Group(eDBG_Range(minpri,maxpri)),origin,offsetX,offsetY,color1,color2)
#define eDBG_STAT_GROUP_ONSCREEN(bitmap, group, origin, offsetX, offsetY, color)                 g_pkStatistic->ShowOnScreen(bitmap,group,origin,offsetX,offsetY,color)
#define eDBG_STAT_GROUP_ONSCREEN2(bitmap, group, origin, offsetX, offsetY, color1, color2)       g_pkStatistic->ShowOnScreen(bitmap,group,origin,offsetX,offsetY,color1,color2)
#define eDBG_PERF_ONSCREEN(bitmap, group, origin, offsetX, offsetY, width, height, full, color)  g_pkStatistic->BarOnScreen(bitmap, group, origin, offsetX, offsetY, width, height, full, color)
#else
#define eDBG_STAT_ONSCREEN(bitmap, minpri, maxpri, origin, offsetX, offsetY, color)
#define eDBG_STAT_ONSCREEN2(bitmap, minpri, maxpri, origin, offsetX, offsetY, color1, color2)
#define eDBG_STAT_GROUP_ONSCREEN(bitmap, group, origin, offsetX, offsetY, color)
#define eDBG_STAT_GROUP_ONSCREEN2(bitmap, group, origin, offsetX, offsetY, color1, color2)
#define eDBG_PERF_ONSCREEN(bitmap, group, origin, offsetX, offsetY, width, height, full, color)
#endif
#endif

//---------------------------------------------------------------
// counter baseclass
//
#define eDBG_MAXSTATNAME	64
#define eDBG_TIMESLICE      10
#define eDBG_MAXGROUPCOUNT  32
#define eDBG_MAXGRAPHCOUNT  128

class eDBG_StatCounter
{
public:
  eDBG_StatCounter( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_StatCounter();

  virtual void Add( eC_Float fValue ) = 0;
  virtual void Set( eC_Float fValue ) = 0;
  virtual void Show( eC_UInt ulFrames ) = 0;
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames ) = 0;
  virtual void Nextframe( );

  inline const char *GetName() const { return m_acName; }
  inline eC_UInt GetPriority() const { return m_uiPriority; } 

protected:		
  char		m_acName[eDBG_MAXSTATNAME+1];
  eC_UInt	m_uiPriority;
};

//---------------------------------------------------------------
// simple counter, singlevalue [display value and value/frame]
//
class eDBG_StatCounterS : public eDBG_StatCounter
{
public:
  eDBG_StatCounterS( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_StatCounterS();

  virtual void Add( eC_Float fValue );
  virtual void Set( eC_Float fValue );
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );
  virtual void Nextframe();

private:
  eC_Float	m_fValue;
  eC_Float	m_fFrameValue;
};

//---------------------------------------------------------------
// range counter, singlevalue [display min value, max value and avg value]
//
class eDBG_StatCounterR : public eDBG_StatCounter
{
public:
  eDBG_StatCounterR( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_StatCounterR();

  virtual void Add( eC_Float fValue );
  virtual void Set( eC_Float fValue );
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );
  virtual void Nextframe();

private:
  eC_Float	m_fMin;
  eC_Float	m_fMax;
  eC_Float	m_fAccu;
  eC_Float	m_fFrameValue;
  eC_UInt		m_uiCount;
};

//---------------------------------------------------------------
// watch counter, type eC_Value
//
class eDBG_StatWatchV : public eDBG_StatCounter
{
public:
  eDBG_StatWatchV( eC_UInt uiPriority, const char *acName, eC_Value *pvWatch );
  virtual ~eDBG_StatWatchV();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_Value	*m_pvWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_Float
//
class eDBG_StatWatchF : public eDBG_StatCounter
{
public:
  eDBG_StatWatchF( eC_UInt uiPriority, const char *acName, eC_Float *pfWatch );
  virtual ~eDBG_StatWatchF();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_Float	*m_pfWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_Fixed
//
class eDBG_StatWatchFX : public eDBG_StatCounter
{
public:
  eDBG_StatWatchFX( eC_UInt uiPriority, const char *acName, eC_Fixed *pfxWatch );
  virtual ~eDBG_StatWatchFX();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_Fixed	*m_pfxWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_UByte
//
class eDBG_StatWatchUB : public eDBG_StatCounter
{
public:
  eDBG_StatWatchUB( eC_UInt uiPriority, const char *acName, eC_UByte *pubWatch );
  virtual ~eDBG_StatWatchUB();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_UByte	*m_pubWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_UShort
//
class eDBG_StatWatchUS : public eDBG_StatCounter
{
public:
  eDBG_StatWatchUS( eC_UInt uiPriority, const char *acName, eC_UShort *pusWatch );
  virtual ~eDBG_StatWatchUS();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_UShort	*m_pusWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_Bool
//
class eDBG_StatWatchB : public eDBG_StatCounter
{
public:
  eDBG_StatWatchB( eC_UInt uiPriority, const char *acName, eC_Bool *pbWatch );
  virtual ~eDBG_StatWatchB();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_Bool	*m_pbWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_Int
//
class eDBG_StatWatchI : public eDBG_StatCounter
{
public:
  eDBG_StatWatchI( eC_UInt uiPriority, const char *acName, eC_Int *piWatch );
  virtual ~eDBG_StatWatchI();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_Int	*m_piWatch;
};

//---------------------------------------------------------------
// watch counter, type eC_UInt
//
class eDBG_StatWatchUI : public eDBG_StatCounter
{
public:
  eDBG_StatWatchUI( eC_UInt uiPriority, const char *acName, eC_UInt *puiWatch );
  virtual ~eDBG_StatWatchUI();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  eC_UInt	*m_puiWatch;
};

//---------------------------------------------------------------
// watch counter, type double
//
class eDBG_StatWatchDouble : public eDBG_StatCounter
{
public:
  eDBG_StatWatchDouble( eC_UInt uiPriority, const char *acName, double *pdWatch );
  virtual ~eDBG_StatWatchDouble();

  virtual void Add( eC_Float fValue ) {}
  virtual void Set( eC_Float fValue ) {}
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

private:
  double	*m_pdWatch;
};

//---------------------------------------------------------------
// performance counter [display value and value/frame]
//
class eDBG_PerfCounter : public eDBG_StatCounter
{
public:
  eDBG_PerfCounter( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_PerfCounter();

  virtual void Add( eC_Float fValue );
  virtual void Set( eC_Float fValue );
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

  virtual eC_Float GetMS(eC_UInt ulFrames) const;        
  inline eC_UInt GetHash() const { return m_uiHash; }

protected:
  void Hash(const eC_Char *acName);

  eC_Time64	m_uiTimestamp;
  eC_Time64	m_uiElapsedTime;
  eC_UInt		m_uiRuns;
  eC_UInt     m_uiHash;
};


//---------------------------------------------------------------
// history performance counter [display value and value/frame]
//
class eDBG_PerfCounterH : public eDBG_PerfCounter
{
public:
  eDBG_PerfCounterH( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_PerfCounterH();

  virtual void Add( eC_Float fValue );
  virtual void Set( eC_Float fValue );
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );
  virtual void Nextframe();

  virtual eC_Float GetMS(eC_UInt ulFrames) const;

protected:
  friend class eDBG_Statistic;

  eC_Time64	m_auiHistory[ eDBG_TIMESLICE ];
  eC_Time64	m_uiAvgElapsedTime;
  eC_Time64	m_uiAllElapsedTime;
  eC_UInt     m_uiFrameIndex;        
  eC_Bool     m_bFReady;
};

//---------------------------------------------------------------
// performance counter real time (not average value) [display value]
//
class eDBG_PerfCounterReal : public eDBG_PerfCounter
{
public:
  eDBG_PerfCounterReal( eC_UInt uiPriority, const char *acName );
  virtual ~eDBG_PerfCounterReal();

  virtual void Add( eC_Float fValue );
  virtual void Set( eC_Float fValue );
  virtual void Show( eC_UInt ulFrames );
  virtual const eC_Char *Get( eC_Char *acBuffer, eC_UInt ulFrames );

  virtual eC_Float GetMS(eC_UInt ulFrames) const;        

protected:

};



//---------------------------------------------------------------
// some target compilers had difficulties to recognise eDBG_Range
// as an POD (plain old data) Type. So we had to use macros and
// mappingto integer.
//

#define eDBG_Range(rmin, rmax)  (((eC_UShort)rmin)<<16) | ((eC_UShort)rmax)
#define eDBG_Level(r)           (((eC_UShort)r)<<16) | ((eC_UShort)r)
#define eDBG_RangeMin(r)        ((eC_UInt)(((r)>>16) & 0xffff))
#define eDBG_RangeMax(r)        ((eC_UInt)((r) & 0xffff))


class eDBG_RangeClass
{
public:
  inline eDBG_RangeClass() {}

  inline eDBG_RangeClass( eC_UInt uiRange )
  {
    m_usMin = (eC_UShort) eDBG_RangeMin( uiRange );
    m_usMax = (eC_UShort) eDBG_RangeMax( uiRange );
  }

  inline eDBG_RangeClass( eC_UInt uiStart, eC_UInt uiEnd )
  {
    if (uiEnd > uiStart)
    {
      m_usMin = (eC_UShort) uiStart;
      m_usMax = (eC_UShort) uiEnd;
    } else {
      m_usMin = (eC_UShort) uiEnd;
      m_usMax = (eC_UShort) uiStart;
    }
  }

  inline eC_UInt Min() const { return (eC_UInt) m_usMin; }
  inline eC_UInt Max() const { return (eC_UInt) m_usMax; }

private:

  eC_UShort m_usMin;
  eC_UShort m_usMax;
};


//---------------------------------------------------------------
class eDBG_Group
{
public:

  eDBG_Group( eC_UInt uiFirst );
  eDBG_Group( eC_UInt uiCount, eC_UInt uiFirst, ... );
  ~eDBG_Group();

  eC_Bool isInGroup(eC_UInt uiValue) const;

  eDBG_Group operator| (const eDBG_Group& rkGroup) const;

private:

  inline eDBG_Group( ) {}

  eC_UInt             m_uiQuantity;
  eDBG_RangeClass     m_akStatic[ eDBG_MAXGROUPCOUNT ];
};


//---------------------------------------------------------------
#ifdef eDBG_AUTOREGISTERSTAT
#define eDBG_STATMAGIC 0x351948af
#endif

#ifdef eDBG_EGMLOUTPUT
class eGML_Bitmap;
#endif

typedef eC_TList<eDBG_StatCounter *> eDBG_StatList;

class eDBG_Statistic
{
public:

  enum eDBG_Origin    { O_UL, O_LL, O_UR, O_LR };
  enum eDBG_Alignment { A_LEFT, A_RIGHT };

  eDBG_Statistic();
  ~eDBG_Statistic(); 
	void Shutdown();
  eDBG_StatCounter *AddCounter( eDBG_StatCounter *kCount );
  eDBG_PerfCounter *AddTimer( eDBG_PerfCounter *kCount );

  void NextFrame();

  void Show( const eDBG_Group &rkGroup );

#ifdef eDBG_EGMLOUTPUT
  void ShowOnScreen(eGML_Bitmap *pkBitmap, const eDBG_Group &rkGroup, eC_Flag fOrigin, eC_UInt uiX, eC_UInt uiY, eC_UInt uiColor1, eC_UInt uiColor2 = 0xff000000);
  void BarOnScreen(eGML_Bitmap *pkBitmap, const eDBG_Group &rkGroup, eC_Flag fAlignment, eC_UInt uiX, eC_UInt uiY, eC_UInt uiWidth, eC_UInt uiHeight, eC_Bool bFullFrame = false, eC_UInt uiColor = 0x000000 );

#endif

protected:
#ifdef eDBG_EGMLOUTPUT
  void ClearEntries();
  void AddEntry(const eC_Char *acName, const eC_Char *acValue);
#endif

private:

  eC_UInt              m_uiFrames;
  eDBG_StatList       *m_pkCounters;
  eDBG_StatList       *m_pkTimers;
#ifdef eDBG_FPSCOUNT
  eDBG_PerfCounterH   *m_pkFPS;
  eC_Bool              m_bFPSready;
  eC_Float             m_fAllFPS;
#endif
#ifdef eDBG_AUTOREGISTERSTAT
  eC_Flag              m_fIsReady;
#endif
#ifdef eDBG_EGMLOUTPUT
  eC_String            m_kNames;
  eC_String            m_kValues;

  static eC_UInt       ms_auiColors[];
#endif
};

//---------------------------------------------------------------
#ifdef eDBG_AUTOREGISTERSTAT
extern eDBG_Statistic g_kStatistic;
#else
extern eDBG_Statistic *g_pkStatistic;
#endif

#endif
//---------------------------------------------------------------
#endif
