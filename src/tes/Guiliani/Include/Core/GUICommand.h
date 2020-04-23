/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICOMMAND__H_
#define GUICOMMAND__H_

#include "eC_Types.h"
#include "GUIStreamableObject.h"
#include "eC_TList_doubleLinked.h"
#include "RefCounted.h"
#include "SharedPtr.h"

class CGUICommand;
/// Abstract command pointer type.
typedef NSmartPtr::SharedPtr<CGUICommand> CGUICommandPtr;

///Compare operators needed in order to used Sort() on a  list of CGUICommandPtr
bool operator==( const CGUICommandPtr& a, const CGUICommandPtr& b );
bool operator>( const CGUICommandPtr& a, const CGUICommandPtr& b );

/// Type of command list, used in CGUICommand CGUICommandHandler.
typedef eC_TListDoubleLinked<CGUICommandPtr> CommandPtrList;

/// Command object base class.

/**
    Command objects are used within Guiliani to represent a certain behaviour in reaction to user
    input. For example clicking on a button, or moving a slider may execute a command object. You may
    see a command object as a predefined action (or chain of actions) that will be executed if a
    certain condition is fulfilled. The command object's Do() method describes this action.

    A typical use case for command objects is the communication between the user interface and the
    underlying application logic. By deploying commands you will gain three major advantages:
    -# Commands are thread safe. They will be serialized and executed in the GUI's thread context.
    -# They decouple GUI and application development. You can start developing the GUI even while the
    underlying application logic is not available - and vice versa. The typical work flow would be to
    start off with empty command-stubs whose Do() methods only include debug messages and attach the
    actual application-binding commands once they are available.
    -# Commands simplify automated testing. You can test the application-API through the respective
    command-objects without the GUI.

    Commands can be executed by calling CGUICommandHandler::Execute(). By default, a command is
    executed once after that method has been called. To run a command repeatedly (have its Do() method
    executed in each GUI main loop cycle), you may set it to 'not finished' explicitly by calling
    SetToBeFinished() with a value of False.

    Each command can have zero or more additional commands which are executed when the 'parent' command
    is finished. The additional commands are <i>not</i> enqueued in the command handler.
    Instead, their Do() methods are called directly. Only if they are not finished after this first call
    they are enqueued. Therefore, the priorities of additional commands are initially ignored. Only if
    they are enqueued because they must run repeatedly the regular command handler mechanisms apply.

@attention
    Do not use CGUICommand* and do not delete CGUICommand objects manually! Instead use CGUICommandPtr and let the
    smart pointer take care of deleting the object once it is no longer referenced.

    The following example shows a simple command which receives a message-string during construction
    which it will later write into a textfield within the GUI.
@code
class CExampleCommand
    : public CGUICommand
{
public:
    // The constructor receives the string which is to be displayed
    CExampleCommand( const eC_String& kMessageString) : m_kMessageString(kMessageString) {};

    // Every command-object must have a Do()-method.
    // The code herein will be executed threadsafe within the GUI's thread-context.
    // This example code will output a debug message to the logfile (or debug-window in VS2005) and change the text-label
    // of an object within the GUI.
    void Do()
    {
         GUILOG( GUI_TRACE_DEBUG, m_kMessageString);
         CGUIBaseTextField* pTextField = static_cast<CGUIBaseTextField*>(GETGUI.GetObjectByID(OBJ_TEXTFIELD));
         if( pTextField)
         {
             pTextField->GetLabel()->SetTextString( m_kMessageString);
             pTextField->InvalidateArea();
         }
    }
private:
    eC_String m_kMessageString;
};
@endcode

    The next line shows how to create a command object and how to enqueue it into the GUI's command handler
    (accessed through the GETCMDHDL helper macro). This could be done anywhere - even in another thread.
    Don't worry about memory leaks here, as commands are implemented as smart-pointers.
@code
    GETCMDHDL.Execute( new CExampleCommand("Hello world!\n"));
@endcode

    The following example shows how to implement a command which gets executed repeatedly after a given time span.
    This is done via the SetToBeFinished() and SetTimeUntilNextExecution() APIs.
@code
class CRepeatingCommand
    : public CGUICommand
{
public:
    // The constructor receives the string which is to be displayed
    CRepeatingCommand( const eC_String& kMessageString) : m_kMessageString(kMessageString) {};

    // The Do method logs the given string to the debug outputs, stays in the queue and gets
    // executed every 1000ms
    void Do()
    {
         GUILOGMESSAGE( m_kMessageString);
         SetToBeFinished(false);
         SetTimeUntilNextExecution(1000);
    }
private:
    eC_String m_kMessageString;
};
@endcode

    @ingroup GUILIANI_COMMANDS
*/
class CGUICommand
    : public CGUIStreamableObject,
      public NSmartPtr::RefCounted
{
    friend class CGUICommandHandler;
public:
    /** Adds an additional command to this command's list. Commands in the
        list are executed immediately after this command has finished.
        @param pkCmd The command to be added.
        @param bOneTime Whether the command is executed only one time. If true,
               the command will be executed once and is then removed from the
               list of additional commands. If false, it stays in the list and
               is executed the next time after the 'parent' command. */
    void AddAdditionalCmd(CGUICommandPtr pkCmd, eC_Bool bOneTime = false);

    /** Removes an additional command from this command's list.
        @param pkCmd The command to be removed. */
    void RemoveAdditionalCmd(CGUICommandPtr pkCmd);

    /// Removes all additional commands from this command's list.
    void RemoveAllAdditionalCmds();

    /** Returns whether this command object's execution is completely finished.
        @return True if command execution is completed, otherwise False.*/
    inline eC_Bool IsFinished() const { return m_bIsFinished;}

    /** Gets the remaining time until this command should be executed again.
        @return Time in milliseconds until next execution. */
    inline eC_UInt GetTimeUntilNextExecution() const { return m_uiTimeUntilNextExecution; };

    /** Sets the remaining time until this command should be executed (again).
        If the command is executed repeatedly (i.e. its IsFinished-Flag is set to false) you need
        to call SetTimeUntilNextExecution() during each call to Do().
        Note that for the first call to CGUI::Run() or CGUI::RunOnce(), the time since
        creation of the GUI is taken as the time since the last command processing.
        @see IsFinished(), SetToBeFinished()
        @param uiTime Time in milliseconds until next execution. */
    void SetTimeUntilNextExecution( eC_UInt uiTime);

    /** Sets the priority of the command
        @param uiPriority is the priority of the command*/
    void SetPriority(eC_UInt uiPriority);

    /** Gets the priority of the command. The smaller the value of m_uiPriority,
        the higher the priority. Thus, 0 is the highest.
        @return The priority of the command*/
    inline eC_UInt GetPriority() const {return m_uiPriority;}

    /** Returns the number of additional commands.
        @return Number of additional commands.
    */
    eC_UInt GetNumberOfAdditionalCmds() const {return m_kAdditionalCmds.GetQuantity();}

    /** Returns the command object of the wanted additional command.
        @param uiIndex Index of the additional command.
        @return The command object of the wanted additional command,
        or NULL if there is no command at the given index.
    */
    CGUICommandPtr GetAdditionalCmd(const eC_UInt& uiIndex) const;


#if defined GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

#if defined GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing the command class ID into a stream.
    static const eC_Char XMLTAG_COMMANDCLASSID[];

    /// XML tag to be used when writing the additional command count into a stream.
    static const eC_Char XMLTAG_ADDITIONALCMDCOUNT[];

    /// XML tag to be used when writing the remove after execution flag.
    static const eC_Char XMLTAG_REMOVE_AFTER_EXECUTION[];
#endif

protected:
    /** Set the command as 'finished' or not.
        By default, a command is 'finished' which results in a single execution
        of its Do() method after it has been added to the command handler by
        calling CGUICommandHandler::Execute(). To keep a command running (have
        its Do() method executed in each GUI main loop cycle), mark it as 'not
        finished'.
        @param bIsFinished Pass true to mark the command as finished, false to
               mark it as unfinished.
    */
    inline void SetToBeFinished(eC_Bool bIsFinished){m_bIsFinished = bIsFinished;}

    /** CGUICommand Constructor.
        @attention
        Since command-constructors typically get called from another thread they must not call any Guiliani APIs! 

        @param uiPriority The command's priority. 0 is the highest priority. In
               each GUI main loop cycle, all queued commands are executed
               sorted by their priority (see CGUICommandHandler::Execute()).
        @param uiTimeUntilFirstExecution Time in milliseconds until the command's first execution.
        @param bFinished true means that the command will be removed from the queue after execution,
               false means that the command stays in the queue and will be executed again.
      */
    CGUICommand(const eC_UInt uiPriority = 0, const eC_UInt uiTimeUntilFirstExecution = 0, const eC_Bool bFinished = true);

    /** Action of the command object. Must be implemented in the derived
        class.*/
    virtual void Do() = 0;

    /// Destructor.
    virtual ~CGUICommand() {}
private:
    /** Immediately executes the additional commands attached to this command. 
        They will not be queued, but instead their Do() methods will immediately be called. */
    void ExecuteAdditionalCmds();

    /** Boolean value indicating whether command execution is finished.
        Is always true for synchronous commands and has to be set explicitly
        for asynchronous ones.*/
    eC_Bool     m_bIsFinished;

    /// Time span in milliseconds until next execution of this command object is desired.
    eC_UInt     m_uiTimeUntilNextExecution;

    /// The priority of this command. The smaller the value of m_uiPriority, the higher the priority.
    /// Thus, 0 is the highest.
    eC_UInt     m_uiPriority;

    struct AdditionalCmd_t
    {
        AdditionalCmd_t()
            : m_pkCmd(NULL), m_bExecOneTime(false) {}
        AdditionalCmd_t(CGUICommandPtr pkCmd, eC_Bool bExecOneTime)
            : m_pkCmd(pkCmd), m_bExecOneTime(bExecOneTime) {}
        CGUICommandPtr m_pkCmd;
        eC_Bool m_bExecOneTime;
    };

    typedef eC_TListDoubleLinked<AdditionalCmd_t> AdditionalCmdList;
    AdditionalCmdList m_kAdditionalCmds;
};
#endif
