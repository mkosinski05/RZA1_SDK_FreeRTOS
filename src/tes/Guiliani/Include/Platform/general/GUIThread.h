/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _GUITHREAD__H_
#define _GUITHREAD__H_

#include "eC_Types.h"


/** These are the 16 supported priority states for the CGUIThread. */
enum ThreadPriority_t
{
    GUITHREAD_PRIORITY_IDLE,
    GUITHREAD_PRIORITY_ABOVE_IDLE,
    GUITHREAD_PRIORITY_BELOW_LOW,
    GUITHREAD_PRIORITY_LOW,
    GUITHREAD_PRIORITY_ABOVE_LOW,
    GUITHREAD_PRIORITY_BELOW_NORM,
    GUITHREAD_PRIORITY_NORM,
    GUITHREAD_PRIORITY_ABOVE_NORM,
    GUITHREAD_PRIORITY_BELOW_HIGH,
    GUITHREAD_PRIORITY_HIGH,
    GUITHREAD_PRIORITY_ABOVE_HIGH,
    GUITHREAD_PRIORITY_BELOW_CRITICAL,
    GUITHREAD_PRIORITY_CRITICAL,
    GUITHREAD_PRIORITY_ABOVE_CRITICAL,
    GUITHREAD_PRIORITY_BELOW_REALTIME,
    GUITHREAD_PRIORITY_REALTIME,
    GUITHREAD_PRIORITY_NOT_SET
};

/// This is the base class for Guiliani threads.

/** Derive from this class if you want to create user threads and implement at
    least the Execute() method. Additionally, you may implement Setup() and
    Cleanup() to suit your needs.

    @par Notes for porting Guiliani
    The implementation of this base class in the Guiliani core only provides
    the platform-independent methods. When porting Guiliani to a new platform,
    the following methods of CGUIThread have to be implemented:
    - Start()
    - Terminate()
    - SetPriority() and GetPriority()
    - MapPriority(const ThreadPriority_t&) and MapPriority(const eC_Int&)
    - WaitForShutdown()

    Thread class example:
@code
    class CExampleThread:
        public CGUIThread
    {
    public:
        CExampleThread():CGUIThread(1000) {}

    private:
        void Setup() {}
        void Execute(void *pArg)
        {
            GUILOG(GUI_TRACE_DEBUG, "Parameter: "+eC_String(*(static_cast<eC_UInt*>(pArg)))+".\n");
            for (eC_UInt i=0; i < *(static_cast<eC_UInt*>(pArg)); i++)
            {
                GUILOG(GUI_TRACE_DEBUG, "ExampleThread: "+eC_String(i)+".\n");
                eC_Wait(5000);
            }
        }
        void Cleanup() {}
    };
@endcode

    Create and execute thread example:
@code
    CExampleThread *pThread = new CExampleThread();
    eC_UInt uiParam = 10;
    pThread->Start((void*)&uiParam);
@endcode
*/

class CGUIThread
{
public:
    /** GetArg method gets the arguments.
        @return void pointer to Arguments.
    */
    inline void* GetArg() const {return m_pArg;}

    /** SetArg method sets the arguments.
        @param pArg void pointer to arguments.
    */
    inline void SetArg(void* pArg) {m_pArg = pArg;}

    /// @return True if this thread is currently running, otherwise False.
    inline eC_Bool IsRunning() const {return m_bRunning;}

    /// @return True if this thread has been completely shut down, otherwise False.
    inline eC_Bool IsShutdownCompleted() const {return m_bShutdownFinishedFlag;}

    /// @return The ID of this thread.
    inline eC_UInt GetThreadId() const {return m_uiThreadId;}

    /** Creates and starts the thread.
        Platform-specific implementations should make sure to set this thread's
        ID (@ref m_uiThreadId).
        @param pArg void pointer to the arguments.
        @return Platform-specific return value, usually negative when an error occurred and 0 or larger when OK
    */
    eC_Int Start(void *pArg);

    /** Terminates the thread.
        @note This method may have platform-dependant side effects and should never be used in order to ensure a reliable platform-agnostic application. For example, destructors of objects created in the thread context will not be called when a thread is terminated.

        On Windows (where TerminateThread() is called):
        Note that the MSDN documentation states "TerminateThread is a dangerous function that should only be used in the most extreme cases".
        The value 0xdeadbeef will be passed to TerminateThread() as dwExitCode.

        On Linux (where pthread_cancel() is called):
        This will interfere with certain ways of exception handling since a abi::__forced_unwind exception will be thrown and must not be intercepted.
        If a catch-all is implemented in a user-derived thread class, this exception will need to be rethrown:
        @code
        try
        {
            // code that may throw an exception
        }
        catch (abi::__forced_unwind&)
        {
            throw;
        }
        catch (...)
        {
            // expected exception handling
        }
        @endcode

        On other platforms this may be empty (as in the dummy thread) or execute arbitrary platform-specific code.

        Again, when writing a Guiliani application this method should never be used. Guiliani will only use Terminate() in case a still running thread is destroyed, which will not happen during normal operation.
    */
    void Terminate();

    /** Sets the m_bRunning flag to false to initiate the shutdown process.
    */
    inline void InitShutdown() {m_bRunning = false;}

    /**
        @deprecated
            Due to namespace collision with winreg.h, this method has been replaced by InitShutdown() and will be removed in future releases.
    */
    inline void InitiateShutdown() {InitShutdown();}

    /** This method doesn't return until the thread has cleanly shut down. */
    void WaitForShutdown();

    /** Sets the new priority to the thread.
        @param ePriority The new priority
    */
    void SetPriority(const ThreadPriority_t& ePriority);

    /** Returns the priority.
        @return thread priority.
    */
    ThreadPriority_t GetPriority();

protected:
    /** Protected contructor, because it should only be called by derived classes.
        @param uiStackSize The stack size for the new thread in bytes
    */
    CGUIThread(const eC_UInt& uiStackSize);

    /** Virtual protected destructor, because it should only be called by derived classes. */
    virtual ~CGUIThread();

    /** Virtual method Setup is called before Execute and can be re-implemented in the user's derived class. */
    virtual void Setup() {}

    /** Virtual method Execute must be implemented in the user's derived class.
        It contains the thread's working code.
        @param pArg Implementation-specific void pointer to the thread's working data
    */
    virtual void Execute(void *pArg) =0;

    /** Virtual method Cleanup is called after Execute and can be re-implemented in the user's derived class to
        clean up any allocated resources.
    */
    virtual void Cleanup() {}

    /** Maps Guiliani thread priorities to platform-specific priorities.
        @param ePriority Guiliani thread priority.
        @return mapped platform-specific priority.
    */
    eC_Int MapPriority(const ThreadPriority_t& ePriority);

    /** Maps platform-specific priorities to Guiliani thread priorities.
        @param iPriority thread priority value from platform specific priority.
        @return mapped Guiliani priority.
    */
    ThreadPriority_t MapPriority(const eC_Int& iPriority);

    eC_UInt m_uiStackSize;          ///< Size for this thread's stack.
    eC_UInt m_uiThreadId;           ///< This thread's unique ID.
    void *m_pArg;                   ///< Pointer to arguments to be passed to the thread.
    volatile eC_Bool m_bRunning;    ///< Whether this thread is currently running.

private:
    /** Private default constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.*/
    CGUIThread();

    /* Private copy-constructor. Should not be used.
    *  Dummy declaration with no implementation, just to hide the function.
       @param kSource Source object.
    */
    CGUIThread(const CGUIThread& kSource);

    /* Private operator= method. Should not be used.
    *  Dummy declaration with no implementation, just to hide the function.
       @param kSource Source object.
       @return this object.
    */
    CGUIThread& operator=(const CGUIThread& kSource);

    /** Controls the thread's lifecycle by calling Setup(), Execute() and Cleanup(). */
    void Run();

    /** This is the static method from where the new thread will be started.
        @param pThis this pointer.
    */
    static void* EntryPoint(void *pThis);

    /** Initializes this class. */
    void Init();

    /** Deinitalizes this class. */
    void DeInit();

    /// This handle can be used for the class which implements this wrapper, if needed.
    void *m_pHandle;

    /// Will be set to true after calling the Cleanup() function.
    eC_Bool m_bShutdownFinishedFlag;
};
#endif
