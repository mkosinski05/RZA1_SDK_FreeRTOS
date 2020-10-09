/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITRACE__H_
#define GUITRACE__H_

#include "GUIObserver.h"
#include "GUISubject.h"
#include "eC_Semaphore.h"
#include "GUIConfig.h"

class CGUICompositeObject;
class eC_File;
class eC_PlatformFile;

/** Logs a message using CGUITrace::Log.
    @param Level The debug level of the message. For a list of trace levels,
    see GUITraceLevels.h.
    @param Message The message string to be logged.
*/
#define GUILOG(Level, Message) GUILOG_##Level(Message)

// The concatenate defines will be replaced with the correct Log methods
// or with no operation to minimize the code, if only error level is set,
// or if trace is switched off.
//------------------------------------------------------------------------------------
#if (GUI_TRACE_LEVEL == GUI_TRACE_DEBUG)
#define GUILOG_GUI_TRACE_DEBUG(Message) CGUITrace::GetInstance().Log("DEBUG: ", Message)
#define GUILOG_GUI_TRACE_WARNING(Message) CGUITrace::GetInstance().Log("WARNING: ", Message)
#define GUILOG_GUI_TRACE_ERROR(Message) CGUITrace::GetInstance().Log("ERROR: ", Message,eC_String(__FILE__), eC_String(__LINE__))
#define GUILOG_THROW_EXCEPTION(Exception, ErrorMessage) {CGUITrace::GetInstance().Log("ERROR: ", ErrorMessage,eC_String(__FILE__), eC_String(__LINE__)); throw Exception;}

#elif (GUI_TRACE_LEVEL == GUI_TRACE_WARNING)
#define GUILOG_GUI_TRACE_DEBUG(Message) {}
#define GUILOG_GUI_TRACE_WARNING(Message) CGUITrace::GetInstance().Log("WARNING: ", Message)
#define GUILOG_GUI_TRACE_ERROR(Message) CGUITrace::GetInstance().Log("ERROR: ", Message)
#define GUILOG_THROW_EXCEPTION(Exception, ErrorMessage) {CGUITrace::GetInstance().Log("ERROR: ", ErrorMessage); throw Exception;}

#elif (GUI_TRACE_LEVEL == GUI_TRACE_ERROR)
#define GUILOG_GUI_TRACE_DEBUG(Message) {}
#define GUILOG_GUI_TRACE_WARNING(Message) {}
#define GUILOG_GUI_TRACE_ERROR(Message) CGUITrace::GetInstance().Log("ERROR: ", Message)
#define GUILOG_THROW_EXCEPTION(Exception, ErrorMessage) {CGUITrace::GetInstance().Log("ERROR: ", ErrorMessage); throw Exception;}

#else // Trace is switched off
#define GUILOG_GUI_TRACE_DEBUG(Message) {}
#define GUILOG_GUI_TRACE_WARNING(Message) {}
#define GUILOG_GUI_TRACE_ERROR(Message) {}
#define GUILOG_THROW_EXCEPTION(Exception, ErrorMessage) {throw Exception;}
//------------------------------------------------------------------------------------
#endif

// If GUI trace is set off, the GUI_REG_... defines are not needed, so they will be
// replaced with no operation and the whole class definition is not needed.
//------------------------------------------------------------------------------------
#if (GUI_TRACE_LEVEL == GUI_TRACE_OFF)
#define GUI_REG_STDOUT_TRACE() {}
#define GUI_REG_FILE_TRACE(FILE) {}
#define GUI_REG_DEBUGGER_TRACE() {}
#else
#define GUI_REG_STDOUT_TRACE() CGUITrace::GetInstance().CreateStdOutTrace();
#define GUI_REG_FILE_TRACE(FILE) CGUITrace::GetInstance().CreateFileTrace(FILE);
#define GUI_REG_DEBUGGER_TRACE() CGUITrace::GetInstance().CreateDebuggerTrace();
#endif

/**
This macro ought to be called at the beginning of any method.
It logs following information:
- name of the method
- name of file where the method is defined
- line within that file
- timestamp in milliseconds
*/
#define GUILOGMETHODENTRY CGUITrace::GetInstance().Log("GUILOGMETHODENTRY: ", "Method "+eC_String(__FUNCTION__)+" called. Timestamp: "+GETTIMER.GetTime()+"ms \n", eC_String(__FILE__), eC_String(__LINE__) );

/**
This macro logs the given message to the registered log outputs.
Use this as a simple replacement for printf if you quickly wish to log a message from within your application code.
Every output written through this interface way will be prefixed with INFO for easier parsing.
Note that this will not be affected by Guiliani's trace-level settings. I.e., this will also be visible in release builds!
*/
#define GUILOGMESSAGE(Message) CGUITrace::GetInstance().Log("INFO: ", Message)

/** @defgroup GUILIANI_DEBUGGING Debugging
    @brief Tracing and Debugging functionalities offered by Guiliani

CGUITrace enables you to add debug output to your application. The advantage of using
CGUITrace as your debug-method, compared to e.g. using printf-debugging directly, is:
<UL>
<LI> You can redirect your debug output to multiple targets (console, file, IDE-debugger window) </LI>
<LI> You can choose between predefined levels of severity (debug, warning, error, exception) </LI>
<LI> You can completely remove debug messages of given severity levels from your code at compile time </LI>
</UL>
Another advantage is that all logged messages will be prefixed with DEBUG, WARNING, ERROR or INFO allowing
you to quickly sort and parse the logged output.

@section sec_guitrace_init Initializing logging
The following example code shows how to register one (or several) logging targets.
This should be placed right at the beginning of your application's startup code.
@code
    // Enable logging to a file
    GUI_REG_FILE_TRACE("ExampleApp.log");
    // Enable logging to debugger window in VisualStudio
    GUI_REG_DEBUGGER_TRACE();
    // Enable logging to stdout
    GUI_REG_STDOUT_TRACE();
@endcode

@section sec_guitrace_usage Usage of the logging macros
Throughout your code, use the following macros if you wish to log something:
@code
    // Log a debug message
    GUILOG(GUI_TRACE_DEBUG,   "CMyClass:: My debug message.\n");
    // Log a warning
    GUILOG(GUI_TRACE_WARNING, "CMyClass:: My warning message.\n");
    // Log an error
    GUILOG(GUI_TRACE_ERROR,   "CMyClass:: My error message.\n");
    // Log and throw an exception
    GUILOG_THROW_EXCEPTION(CGUIException(), "CMyClass:: My exception message.\n");
    // Log the current file, method, and line number
    GUILOGMETHODENTRY
    // Log a simple informational message
    GUILOGMESSAGE("Hello World.\n");
@endcode

@section sec_guitrace_lavel Setting the trace level
You can choose which messages should be compiled into your binary via the GUI_TRACE_LEVEL define.
All messages of at least the defined severity will remain, while all others will be stripped by the
preprocessor. This define is usually set within the GUIConfig.h file.
@code
    // enable ALL log output
    #define GUI_TRACE_LEVEL GUI_TRACE_DEBUG
    // Do not show debug messages anymore
    #define GUI_TRACE_LEVEL GUI_TRACE_WARNING
    // Show only severe errors
    #define GUI_TRACE_LEVEL GUI_TRACE_ERROR
    // Do not show ANY log output
    #define GUI_TRACE_LEVEL GUI_TRACE_OFF
@endcode

@section sec_guitrace_custom_log Writing custom logging targets
It is possible to implement customized targets for log output by deriving them from CGUIObserver
and adding them as an observer to the CGUITrace subject.
Your customized trace class will then be called whenever a piece of code uses GUILOG(...).

This is an example on how to implement your own custom trace:
@code
    class CCustomTrace : public CGUIObserver
    {
    public:
        CCustomTrace():CGUIObserver(){}
        virtual void OnNotification(const eC_String& rkTraceString) { // Custom code goes here }
        virtual ~CCustomTrace(){}
    };
@endcode

This is how to attach it as an observer to CGUITrace:
@code
    CCustomTrace* pkCustomTrace = new CCustomTrace();
    CGUITrace::GetInstance().AddObserver( pkCustomTrace );
@endcode
*/

/** 
    Please refer to the module "Debugging" for detailed information.
    @see GUILIANI_DEBUGGING

    @ingroup GUILIANI_DEBUGGING
    @brief Guiliani Trace component for platform independent logging of debug/error information to various targets.
*/
class CGUITrace : public CGUISubject
{
public:
    /**
        CGUITrace is a Singleton class, i.e. only one instance of it can be created.
        GetInstance is the method that returns a reference to this instance.
        @return The only instance of CGUITrace.
    */
    static CGUITrace& GetInstance()
    {
        static CGUITrace kTrace;
        return kTrace;
    }

    /**
        Destructor. Needs to be public because else the Pocket PC compiler does
        not like it.
    */
    ~CGUITrace();

    /** Logs a message by notifying all observers.
        The log message consists of two strings and, optionally, the calling
        location (file and line number from which this method was called).
        @param rkLogStr Text before the log message.
        @param rkOutputStr Log message.
        @param rkFileName Name of the file from which this method was called.
        @param rkLineNumber Line number from which this method was called.
    */
    void Log(
        const eC_String& rkLogStr, const eC_String& rkOutputStr,
        const eC_String& rkFileName = "", const eC_String& rkLineNumber = "");

    /** Creates a CStdOutTrace object, which logs all messages to StdOut. */
    void CreateStdOutTrace();

    /** Creates a CFileTrace object, which logs all messages to the given logfile.
        @param TraceFileName Name of the log file to be written.
    */
    void CreateFileTrace(const eC_String &TraceFileName);

    /** Creates a CDebuggerTrace object, which logs all messages to the VisualStudio debugger window. */
    void CreateDebuggerTrace();

    /**
    * Print the hierarchy of the passed object.
    * If the object is not a composite object, it
    * prints the hierarchy of the parent of this object.
    * @param pObj pointer to an object or composite object
    */
    static void PrintObjectTree(CGUIObject* pObj);

    /**
    Definition of enum values for de/activation of logging output at runtime.
    */
    enum GUITraceActivation_t
    {
        GUITRACE_NOT_ACTIVE = 0, /// Messages will not be logged
        GUITRACE_ACTIVE = 1 /// Messages will be logged
    };

    /** De/Activates trace output at runtime.
    If tracing is activated this switch enables the user to dimiss the trace
    messages before being written to the output.
    @param eTraceActive GUITRACE_NOT_ACTIVE or GUITRACE_ACTIVE to deactivate or activate tracing.
    */
    void SetTraceActive(GUITraceActivation_t eTraceActive);

    /** Checks if trace output at runtime is activated.
    @return The current trace activation state.
    */
    GUITraceActivation_t GetTraceActive();

private:
    /** Standard-constructor.
        Not a dummy implementation, but should not be used from outside.
    */
    CGUITrace();

    /** Copy-constructor. Should not be used.
        Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
    */
    CGUITrace(const CGUITrace& kSource);

    /** Operator= method. Should not be used.
        Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
        @return This object.
    */
    CGUITrace& operator=(const CGUITrace& kSource);

    /** Prints all messages into a file.
       @ingroup GUILIANI_DEBUGGING
    */
    class CFileTrace : public CGUIObserver
    {
    public:
        /** Constructs a file trace object.
            @param pcTraceFileName Name of the file into which trace output
            is written.
        */
        CFileTrace(const eC_String& rkTraceFileName);

        virtual void OnNotification(const eC_String& rkTraceString);

        ~CFileTrace()
        {
            delete m_pkTraceFile;
            m_pkTraceFile=NULL;
        }


    private:
        /** Default-constructor. Should not be used.
            Dummy declaration with no implementation, just to hide the function.
        */
        CFileTrace();

        /** Copy-constructor. Should not be used.
            Dummy declaration with no implementation, just to hide the function.
            @param kSource Source object to be copied.
        */
        CFileTrace(const CFileTrace& kSource);

        /** Operator= method. Should not be used.
            Dummy declaration with no implementation, just to hide the function.
            @param kSource Source object to be copied.
            @return This object.
        */
        CFileTrace& operator=(const CFileTrace& kSource);

        eC_File *m_pkTraceFile;
    };

    /** Prints all messages to stdout.
       @ingroup GUILIANI_DEBUGGING
    */
    class CStdOutTrace : public CGUIObserver
    {
    public:
        CStdOutTrace():CGUIObserver(){}
        virtual void OnNotification(const eC_String& rkTraceString);
        ~CStdOutTrace(){}
    };

    /** Prints all messages using the Win32 API debugger interface.
       @ingroup GUILIANI_DEBUGGING
    */
    class CDebuggerTrace : public CGUIObserver
    {
    public:
        CDebuggerTrace():CGUIObserver(){}
        virtual void OnNotification(const eC_String& rkTraceString);
        ~CDebuggerTrace(){}

    private:
        /** Copy-constructor. Should not be used.
            Dummy declaration with no implementation, just to hide the function.
            @param kSource Source object to be copied.
        */
        CDebuggerTrace(const CDebuggerTrace& kSource);

        /** Operator= method. Should not be used.
            Dummy declaration with no implementation, just to hide the function.
            @param kSource Source object to be copied.
            @return This object.
        */
        CDebuggerTrace& operator=(const CDebuggerTrace& kSource);
    };

    /** Helper function used by PrintObjectTree() to
        traverse the composite object
        and write the child hierarchy to a file.
        @param pObj pointer to a composite object
        @param ilvl recursion depth
    */
    static void PrintCompositeDebugTree(const CGUICompositeObject* pObj, eC_Int ilvl);

    CStdOutTrace    *m_pkStdOutTrace;       /**< Pointer to CStdOutTrace instance if it is created. */
    CFileTrace      *m_pkFileTrace;         /**< Pointer to CFileTrace instance if it is created. */
    CDebuggerTrace  *m_pkDebuggerTrace;     /**< Pointer to CDebuggerTrace instance if it is created. */

    /** Protects the trace outputs against simultaneous access from different threads. */
    eC_Semaphore m_kSem;

    GUITraceActivation_t m_eTraceActive;
};

#endif
