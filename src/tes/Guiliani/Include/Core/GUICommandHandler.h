/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICOMMANDHANDLER__H_
#define GUICOMMANDHANDLER__H_
#include "eC_Types.h"
#include "eC_TList_doubleLinked.h"
#include "eC_Semaphore.h"
#include "GUICommand.h"

/** @defgroup GUILIANI_COMMANDS Commands
    @brief Implementation of the Command Design Pattern within Guiliani

The Command design pattern allows you to encapsulate actions and associated
parameters into small, reusable pieces of source code. In the context of GUI
development, commands prove especially useful when implementing the link
between GUI representation and underlying application logic.

Guiliani implements this design pattern in various classes:
- Commands are implemented by deriving from the base class CGUICommand.
- For executing commands, the CGUICommandHandler must be used.
- One of the most common uses of commands is a reaction to a button click, see
CGUIBaseButton.

<p>
@section sec_commands_questions1 What is a command technically?

Technically a command is a C++ class derived from Guiliani's CGUICommand base-class.
This is necessary so Guiliani can deal with your customized commands through the generalized
interface of this base class. This interface is extremely slim and only requires you implement a
single method. This method is named Do() and includes the specific application logic that you
wish to execute therein.

You may place _any_ code inside of these Do() methods that you wish, and therefore also call any
user-defined APIs. Just be aware that you should not do any time-consuming tasks here, as this will
block the UI. Your API-calls should therefore either return immediately or you should start a
dedicated thread from the Do() method if you know that respective processing might take longer.

<p>
@section sec_commands_questions2 How can I execute Commands from within the GUI?

Some controls already have dedicated attributes for adding commands to them. (e.g. CGUIButton).
For others, you can use Guiliani's predefined Behaviours to execute commands in reaction to an
incoming event (e.g. key-pressed or mouse-clicks).

<p>
@section sec_commands_questions3 How can I execute Commands from within my application?

You application code can use Guiliani's CGUICommandHandler to execute commands whenever desired.
Simply call GETCMDHDL.Execute( new YourCommandClass() ) to do so. This will add your new command
to the queue of command-objects and Guiliani will process it as soon as possible.
Typically, you may want to do this e.g. in order to update a status-icon within the GUI (e.g.
battery-power icon) or e.g., to show a popup-window which shows information to the user.

<p>
@section sec_commands_questions4 When will the code within my command's Do() method be executed?

It will be executed at the next possible occasion. The precise time can not be defined since this
depends on how much time other tasks within the user-interface consume - such as other commands being
processed, or graphical operations while updating the screen.

<p>
@section sec_commands_questions5 Why is using commands thread-safe?

Thread-safety is granted by the fact, that all Guiliani APIs which deal with commands (such as the
GETCMDHDL.Execute() interface) are by design guaranteed to work thread-safe. And Guiliani's
CGUICommandHandler component will execute the code contained in the aforementioned Do() method in the
GUI's thread-context. Therefore, you can safely access GUI components within this method!

<p>
@section sec_commands_questions6 How can I use commands inside the Guiliani HMI Editor?

The Guiliani Editor (GSE) is designed to be extensible and allows you to add custom objects and
commands into it. For a detailed description and examples on how to do this please refer to the
respective documentation of GSE. The general procedure is to implement @ref
GUILIANI_STREAMING for your custom command(s) and add them to GSE, so that they become available for
use within the attribute view.

<p>
@section sec_commands_questions7 Is there a tutorial with example code on how to use commands?

Yes. There is a howto on Application Binding om our <a href=https://www.guiliani.de>website</a>
which includes the usage of commands.

*/

/// Retrieve pointer to command handler instance.
#define GETCMDHDL           CGUICommandHandler::GetInstance()

/// Manages and executes instances of CGUICommand.

/** If a command ought to be executed, the method Execute() must be called.
The command is not executed right away. Instead, it is enqueued according
to its priority. The framework processes queued commands in each GUI main
loop cycle. Processing a command means executing its Do() method.

@ingroup GUILIANI_COMMANDS
*/
class CGUICommandHandler
{
public:
    friend class CGUI;
    friend class CGUICommand;

    /// return pointer to command handler instance
    static inline CGUICommandHandler& GetInstance() { return ms_kCommandHandler; }

    /// Inits all resources. Creates Semaphore.
    void Init();

    /// Frees all resources. Usually this is only called when shutting the application down.
    void DeInit();

    /** Returns the time in milliseconds until the next command in the 'working' or
        'waiting' queue wishes to be processed again.
        @return Idle time or 1000ms if no command is waiting.
    */
    eC_UInt GetIdleTime();

    /** Adds a command to the 'waiting commands' queue according to its waiting
    time and priority.
    The command will be executed (its CGUICommand::Do() method will be called) later.

    Commands with the shortest waiting times are executed first. If any
    commands have the same waiting times, the ones with higher priorities
    are executed first. For commands with the same time and priority the execution
    order may be arbitrary.

    @attention
    Note that any command placed into the queue will be owned by the command
    handler. Do not delete the command yourself as commands are implemented as
    smart-pointers.

    @param pCommand Pointer to the command object to be added.
    @return True if the command was successfully added, False if not.*/
    eC_Bool Execute(CGUICommandPtr pCommand);

    /** Set the maximum number of commands in the internal working queue.
    There are two internal queues, the waiting queue and the working queue.
    The Execute() method enqueues a command in the waiting queue. The
    Process() method moves commands from the waiting into the working queue
    until either the waiting queue is empty or the maximum number of
    commands in the working queue is reached. Only commands in the working
    queue are actually executed.

    Note that if this number is too low and there are a lot of unfinished
    commands in the working queue, some commands in the waiting queue may
    never be executed. The default value is 100 commands.

    @param uiMaxNoOfWorkingCmds The maximum number of commands in the
    working queue.
    */
    inline void SetMaxWorkingCmds(const eC_UInt &uiMaxNoOfWorkingCmds)
    {
        m_uiMaxNoOfWorkingCommands = uiMaxNoOfWorkingCmds;
    }

    /** Outputs debugging information about the number of commands in the queues.
    This is useful for example to check whether when switching to a new dialog and
    back to the former one the number of command remains the same. For most applications
    this invariant applies. */
    void DumpQueues() const;

    /** Outputs debugging information about the commands in the queues.
        Thus, you can monitor the commands of each queue. 
        This is only available if GUI_TRACE_LEVEL is set to GUI_TRACE_DEBUG */
    void DumpCommands();

    /** Removes the given command from the command handler.
    @param pkCommand Pointer to the command object to be removed. 
    */
    void RemoveCommand( CGUICommandPtr pkCommand);

    /** Checks if the given command is currently enqueued in the command handler and waiting to be processed.
    @param pkCommand Pointer to the command object which is to be checked
    @return True if the command is enqueued in the command handler. False otherwise.
    */
    eC_Bool IsCommandInQueue( CGUICommandPtr pkCommand);

#ifndef eC_TARGET_ENV_POCKET
private: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#else
public:
#endif
    /// Destructor
    ~CGUICommandHandler();

private:
    /** Insert a command into the waiting queue at the correct position derived
    from 'time until next execution' and priority.
    @param pkCommand Pointer to the command object to be added.
    @remarks This method does not enter/leave the queue semaphore and must
    therefore be called from within a block of code that properly
    locks the semaphore.
    */
    void InsertCommand(CGUICommandPtr pkCommand);

    /** Used by CGUICommand to set the time until next execution in a
    thread-safe manner.
    @param pCommand A pointer to the command to be modified.
    @param uiTime The new time until next execution.
    */
    void SetTimeUntilNextExecution(CGUICommand* pCommand, eC_UInt uiTime);

    /** Used by CGUICommand to set the priority in a thread-safe manner.
    @param pCommand A pointer to the command to be modified.
    @param uiPriority The new priority.
    */
    void SetPriority(CGUICommand* pCommand, eC_UInt uiPriority);

    /** Helper for Process() that transfers commands between the 'waiting' and
    the 'working' queue as required by the individual commands' waiting times
    and priorities.
    @remarks This method does not enter/leave the queue semaphore and must
    therefore be called from within a block of code that properly
    locks the semaphore.
    */
    void SortQueuesAndMoveCommands();

    /** Start processing of commands. Moves commands from the 'waiting' to the 'working' queue
    and removes completed ones from the 'working' queue.
    Unfinished objects in the working queue will remain in the working queue and will not be
    removed until marked as finished (see CGUICommand::SetToBeFinished()).
    */
    void Process();

    /** Command handler constructor.
    */
    CGUICommandHandler();

    /** Copy-constructor.
    Dummy implementation to avoid unintended use of compiler-generated default */
    CGUICommandHandler(const CGUICommandHandler& kSource);

    /** Operator= method.
    Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUICommandHandler& operator=(const CGUICommandHandler& kSource);

    /// The singleton instance.
    static CGUICommandHandler   ms_kCommandHandler;

    /// Maximum number of working commands
    eC_UInt m_uiMaxNoOfWorkingCommands;
    /// Queue with commands which are waiting to be processed. It has a dynamic size.
    CommandPtrList m_WaitingQueue;
    /// Queue with commands which are currently being processed. It has a fixed maximum of elements.
    CommandPtrList m_WorkingQueue;
    /// Internal timer to calculate the time span between last and current call to Process().
    eC_UInt m_uiTime;
    /// Internal semaphore to sequentialise access to the queues, thus another thread may call any
    /// function of the command handler.
    eC_Semaphore m_kQueueSemaphore;
    /// Whether the command queues may have been modifed so that commands are no longer in order of priority
    eC_Bool m_bQueueIsDirty;
};
#endif
