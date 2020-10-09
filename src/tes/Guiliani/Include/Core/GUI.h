/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI__H_
#define GUI__H_

#include "GUICompositeObject.h"

// Helper macros
/// Retrieve pointer to GUI instance.
#define GETGUI CGUI::GetInstance()

// PocketPC 2002 platform does not support exception handling, therefore
// as a dirty workaround it is replaced.
#ifdef PPC_2002
#define try           if(true)
#define catch()       if(false)
#define throw         if(true)
#endif

/// GUI base class for any Guiliani application.

/**
    The CGUI class is the application's interface for communicating with Guiliani.
    An application derives a class from this one and does its own special
    initialization in its constructor.

    CGUI contains the application main loop which is started by executing
    the Run() method. The loop handles input events, processes pending
    animations and commands and initiates redrawing of the controls.
    If an application wants to end, it calls the Quit() method which makes
    Run() return after the next cycle.

    @ingroup GUILIANI_CONTROLS
*/
class CGUI
    : public CGUICompositeObject
{
    /** CGUI declares variables used by the 4-way focusing algorithm which CGUIObject and
        CGUICompositeObject access, but which no other classes should be able to access. */
    friend class CGUIObject;

public:
    /** Returns the singleton GUI instance.
        @return the Instance of the GUI
    */
    static inline CGUI& GetInstance() { assert(NULL != ms_pInstance); return *ms_pInstance; }

    /**
       Check whether the instance of the GUI is constructed.
       This method does neither indicate whether the GUI is initialized nor whether it is running.
       @return True if an instance was created, False else.
     */
    static inline eC_Bool IsCreated() {return (ms_pInstance != NULL);}

    /**
       Check whether the instance of the GUI is running. 'Running' means that
       Run() has been called and Quit() has not been called.
       @return True if the GUI is running, otherwise False.
     */
    static inline eC_Bool IsRunning() {return ms_bRunning;}

    /// Quit the GUI's main loop after the current cycle is complete.
    void Quit();

    /**
        Special version of FocusAny which adds functionality that is needed
        by the framework. From the user's perspective the function does what is
        documented in the base class.
        @return eC_Bool True if focus was obtained, False otherwise
        @see CGUICompositeObject::FocusAny
     */
    eC_Bool FocusAny();

    /** Implements the loop that the application runs in.
        Call this method from the application's main function. It calls RunOnce()
        repeatedly and returns only after Quit() has been called.
        @return Currently always False.
      */
    eC_Bool Run();

    /** The 'body' of the main loop. Processes input events, commands and triggers
        redraws when necessary.

        Call this method directly on platforms that have their own loop management
        and don't allow applications to run in a self-contained loop. You will need
        to determine the point at which to quit the application manually.
        @return True before Quit() has been called, false after it has been called.
        @remark This method always sets the internal 'running' state to true first.
                If one of the events or commands leads to a call to Quit() during
                application processing, the 'running' state is set to False. This
                allows for determining 'user quits' by examining the return value.
      */
    eC_Bool RunOnce();

    /** Called when the focus should be set to the next control that is in the
        given direction of the currently focused object. This function needs
        the event handler instance to already be created.
        @param eFocusDirection The focus direction
    */
    void FourWayFocusNext(FourWayFocus_t eFocusDirection);

    /** This method slot will be called whenever a CGUIObject is destroyed.
        You may override this method in derived classes to plug-in any code which needs to
        be informed when a CGUIObject is destroyed.
        @param pDestroyedObject Pointer to object which has been destroyed.  */
    virtual void NotifyOfDestruction( const CGUIObject* pDestroyedObject) {}

    /** Returns the time that Guiliani may idle until the next animation or command
        timer expires.
        @return The idle time in milliseconds.
      */
    eC_UInt GetIdleTime() const;

    /** Returns the version number of Guiliani that was used during build.
        @return Version number string
    */
    static const eC_String& GetVersionString();

    /// Prints information about the Guiliani revision and Configuration settings used while building this library.
    static void PrintLibraryInformation();

    /** Sets a Guiliani License key to remove limitations of a Lite or Eval-Version.
        Note that this needs to be done BEFORE constructing the root CGUI object!
        @param pcLicenseKey your license key.
    */
    static void SetGuilianiLicenseKey(const eC_Char* pcLicenseKey);

    /** Loads a Guiliani License key file and calls SetGuilianiLicenseKey() with the loaded key.
        Note that this needs to be done BEFORE constructing the root CGUI object!
        @param kFilename Name of the license file
        @return True if the key file could be loaded, False otherwise.
    */
    static eC_Bool LoadGuilianiLicenseKey(const eC_String& kFilename);

    /** This slot is implemented for drawing an overlay, like the fake mouse cursor (CGUIFakeMouseCursor).
    Unlike DoDraw, which is being called BEFORE any children are being drawn, this method will be automatically
    called by the framework AFTER all children have been drawn.
    @return True if anything was drawn, False otherwise
    */
    virtual eC_Bool DoPostDraw();

protected:

    /// constructor
    CGUI(const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /// constructor
    CGUI(const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /// Destructor
    virtual ~CGUI(void);
    
    /// This boolean value specifies if the GUI is currently running
    static eC_Bool ms_bRunning;

    static const eC_Int GUILIANI_VERSION_MAJOR;
    static const eC_Int GUILIANI_VERSION_MINOR;

private:
    /** Finds a 4-way focus neighbor ID for an object in a specific direction.
        @param pkObject The object whose neighbor ID is to be found.
        @param eDirection The direction.
        @return The object handle that is set for pkObject in eDirection.
        @see CGUIObject::SetNeighborIDs()
      */
    ObjectHandle_t GetNeighborID(const CGUIObject* pkObject, FourWayFocus_t eDirection) const;

    /// static member pointer to GUI instance
    static CGUI                 *ms_pInstance;

    /// Initialization of arbitrary members
    void Init();

    /** CGUI standard constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUI();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
        @param kSource source object
    */
    CGUI(const CGUI& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
        @param kSource source object
    */
    CGUI& operator=(const CGUI& kSource);

    /// Used in 4-way navigation algorithm. The shortest distance exp 2 found so far.
    eC_UInt m_uiDistance;

    /// Used in 4-way navigation algorithm. The object in the shortest distance found so far.
    CGUIObject *m_kObjectToFocus;

    /// Used in 4-way navigation algorithm. Only objects of at least this distance exp 2 will be
    /// considered to be a match.
    eC_UInt m_uiMinDistance;

    /// Used in 4-way navigation algorithm. List of pointers to objects that are in the
    /// current distance and which denied to get the focus.
    ObjectPtrList m_pSameDistanceObjects;

    /// Maximum idle time until next GUI loop. Calculated from last frame's duration and next expected
    /// execution dates for waiting commands and registered animations
    eC_UInt m_uiIdleTime;

    /// Evaluation expired
    eC_Bool m_bEvalExpired;

};
#endif
