/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIPERFMON__H_
#define GUIPERFMON__H_
#include "eC_Types.h"
#include "eC_String.h"

// Exclude this class from the doxygen documentation
#ifndef DOXYGEN

/// GUIPERFMON Is for internal use only!!!

// Uncomment the following define to activate the performance monitor
//#define GUIPERFMON

/// Please use the following macros to monitor the performance
/// In case of deactivated monitoring no code will be executed
#ifndef GUIPERFMON
#define CONTROLPOINTSTART(cp);
#define CONTROLPOINTEND(cp);
#define CONTROLPOINTPERFMEASURE();
#else

/// Retrieve reference to Timer instance
#define GETPERFMON        CGUIPerfMon::GetInstance()

/** Stores the start time for a specified control point (column).
    @param cp The enum specifying the control point. */
#define CONTROLPOINTSTART(cp) GETPERFMON.ControlPointStart(cp);

/** Stores the end time for a specified control point (column).
    @param cp The enum specifying the control point. */
#define CONTROLPOINTEND(cp) GETPERFMON.ControlPointEnd(cp);

/** Calculates and writes (appends) one line of PerfMon report into the "PerfMon.log"-file. */
#define CONTROLPOINTPERFMEASURE() GETPERFMON.CalcPerfMeasures();


/** @defgroup GUILIANI_PERFORMANCE Performance
    @brief Performance monitoring offered by Guiliani

CGUIPerfMon enables you to collect performance measurements and log it into a file which can
later be used to evaluate performance bottlenecks. For every transit through the main event loop
CGUIPerfMon is able to collect timing data for specific parts of the main event loop. At the end
of every main event loop the collected and calculated data can be written into a log file. Each measured
area is called control point.

The monitoring is done for a predefined set of control points. In addition to the standard control points
already defined by Guiliani a user can set up to CGUIPerfMon::kNumOfAvailableUserEntries user entries.
For every loop through CGUI::RunOnce() the measurements are collected and written out into the perf mon
log file.

@section sec_guiperformance_init Activation of performance monitoring
The following example code shows how to activate the performance logging feature.
The preprocessor define should be configured inside the compiler settings and applied for
all files that want to use CGUIPerfMon.

@code
// Used inside a global include .h file
#define GUIPERFMON
// or defined inside the compiler settings.
@endcode

@section sec_guiperformance_controlpoints Control Points

The predefined control points are:
<UL>
<LI> eFrameDuration: Total time for one frame (excluding time within eInputWrapper, which is typically idle time). </LI>
<LI> eInputWrapper: Time spent within the input wrapper (GETINPUTMEDIA.GetEvent()). </LI>
<LI> eGuiInit: Time spent within CGUI::Init(). </LI>
<LI> eHandleEvent: Time spent within GETEVENTHDL.HandleEvent(). </LI>
<LI> eCmdHdlProcess: Time spent within GETCMDHDL.Process(). </LI>
<LI> eGetTimerProcess: Time spent within GETTIMER.Process(). </LI>
<LI> eRedrawGUI: Time spent within GETGFX.RedrawGUI(). </LI>
<LI> eText: Time spent within CGfxWrap::Text(). </LI>
<LI> eLoadImg: Accumulated time spent within CGfxWrap::LoadImg(). </LI>
<LI> eLoadFont: Accumulated time spent within CGfxWrap::LoadFont(). </LI>
<LI> eRequiredSpace: Accumulated time spent within CGfxWrap::RequiredSpace(). </LI>
</UL>

The user control points which can be freely defined are:
<UL>
<LI> eUser00: Your definition. </LI>
<LI> ...: </LI>
<LI> eUser09: Your definition. </LI>
</UL>

@section sec_guiperformance_controlpoints Available helper macros

@code
// Retrieve reference to performance monitor instance
#define GETPERFMON        CGUIPerfMon::GetInstance()

// Stores the start time for a specified control point (column).
//    @param eCP The enum specifying the control point.
#define CONTROLPOINTSTART(cp) GETPERFMON.ControlPointStart(cp);
// Stores the end time for a specified control point (column).
//    @param cp The enum specifying the control point.
#define CONTROLPOINTEND(cp) GETPERFMON.ControlPointEnd(cp);

//Calculates and writes (appends) one line of PerfMon report into the "PerfMon.log"-file.
#define CONTROLPOINTPERFMEASURE() GETPERFMON.CalcPerfMeasures();
@endcode


This is an example on how to measure time for a control point:
@code
    CONTROLPOINTSTART(CGUIPerfMon::eInputWrapper)
    CGUIAutoPtr<CGUIEvent> pEvent(GETINPUTMEDIA.GetEvent( suiIdleTime ), false);
    CONTROLPOINTEND(CGUIPerfMon::eInputWrapper)
@endcode

To write a new entry into the log file you can use the following code. Beside generating
the monitoring output it also resets all stored measures done before.
Normally it is not necessary because the output will be done automatically at the
end of CGUI::RunOnce():
@code
    CONTROLPOINTPERFMEASURE();
@endcode

*/

/** Guiliani performance monitoring component for platform independent monitoring of
    consumed time for predefined and user defined control points.
    Please refer to the module "Performance" for detailed information.
    @see GUILIANI_PERFORMANCE

    @ingroup GUILIANI_PERFORMANCE
*/

/// Guiliani performance monitoring component
class CGUIPerfMon
{
public:
    /// @return pointer to the singleton timer instance.
    static inline CGUIPerfMon& GetInstance() { return ms_kPerfMon; }

    /// Number of allowed user entries.
    static const eC_UInt kNumOfAvailableUserEntries = 10;

    /// Available control points (columns) of the perf mon log file.
    enum PerfMonControlPoint_t
    {
        eFrameDuration,
        eInputWrapper,
        eGuiInit,
        eHandleEvent,
        eCmdHdlProcess,
        eGetTimerProcess,
        eRedrawGUI,
        eText,
        eLoadImg,
        eLoadFont,
        eRequiredSpace,
        eUser00,
        eUser01,
        eUser02,
        eUser03,
        eUser04,
        eUser05,
        eUser06,
        eUser07,
        eUser08,
        eUser09,
        eNumOfElements
    };

    /// Saves the start time for a specified control point (column).
    /** The saved start time will be used to calculate the time difference
        between ControlPointEnd and ControlPointStart which is written into the log.
        @param eCP The enum specifying the control point.
    */
    void        ControlPointStart(PerfMonControlPoint_t eCP) {m_StartCP[eCP] = eC_GetTicks64();};

    /// Saves the end time for a specified control point (column).
    /** The saved end time will be used to calculate the time difference
        between ControlPointEnd and ControlPointStart which is written into the log.
        @param eCP The enum specifying the control point.
    */
    void        ControlPointEnd(PerfMonControlPoint_t eCP) {m_SumCP[eCP] = m_SumCP[eCP] + (eC_GetTicks64() - m_StartCP[eCP]); ++m_CountCP[eCP];};

    /** Calculates and writes (appends) one line of PerfMon report into the "PerfMon.log"-file.
    */
    void        CalcPerfMeasures();

    /// Sets the number of used user entries (user control points).
    /** This number of user entries are then part of reports if
        CalcPerfMeasures() is called.
        @param uiNumOfUserEntries The number of user entries (0-kNumOfAvailableUserEntries).
    */
    void        SetNumOfUserEntries(eC_UInt uiNumOfUserEntries) {m_NumOfUsedUserEntries = uiNumOfUserEntries;};

    /** Gets the number of used user entries (user control points).
        @return Number of used user entries.
    */
    eC_UInt     GetNumOfUserEntries()                           {return m_NumOfUsedUserEntries;};

    /// Pauses the output of performance monitoring. No new control points in the log file will be created anymore.
    void        PauseMonitoring(){m_PerfMonPaused = true;};

    /// Continues the output of performance monitoring.
    void        ContinueMonitoring(){m_PerfMonPaused = false;};

    /// Sets if a counter will be written per control points.
    /** For every control point the number of calls used to measure the data will be
        collected and with this setting it is possible to generate output into the
        log file containing all counter values (Default false).
        @param bCountLog True for logging the counter per control point.
    */
    void        SetCountLog(eC_Bool bCountLog) {m_bCountLog = bCountLog;};

    /** When enabled, the logged values will be written to a log file.
        @param bWriteLogFile True enables logging to file, False disables it */
    void        SetWriteLogFile(eC_Bool bWriteLogFile) {m_bWriteLogFile = bWriteLogFile;};

    /** Sets the number of cycles after which a new log entry shall be written.
        The summarized measurements will be divided by the number of cycles, thus resulting in an average value.
        Increasing the cycles per log reduces the performance overhead for logging.
        @param uiCyclesPerLog Number of cycles per log. By default, this equals 1.
    */
    void        SetCyclesPerLog(eC_UInt uiCyclesPerLog) {m_uiCyclesPerLog = uiCyclesPerLog;};

    /// Reset all monitored values and counters to 0.
    void        Reset();

    virtual ~CGUIPerfMon();

private:
    /// Hidden constructor.
    CGUIPerfMon();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIPerfMon(const CGUIPerfMon& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIPerfMon& operator=(const CGUIPerfMon& kSource);

    /// Calcs the current number of used control points (columns).
    eC_UInt     GetNumOfEntries(){return (eNumOfElements - kNumOfAvailableUserEntries + m_NumOfUsedUserEntries);};

    /// Writes one line of the perfmon log file to the end of the file (append).
    void        WriteToFile(const eC_String& rkTraceString);

    eC_Time64 m_StartCP[eNumOfElements]; /// Used to store the start time of a monitored control point.
    eC_Time64 m_EndCP[eNumOfElements]; /// Used to store the end time of a monitored control point.
    eC_Time64 m_SumCP[eNumOfElements]; /// Used to accumulate the total time of a monitored control point.
    eC_UInt   m_CountCP[eNumOfElements]; /// Number of measurements on this control point since it was last logged.

    eC_UInt m_NumOfUsedUserEntries; /// Number of user entries which are visible withing the PerfMon log file.
    eC_Bool m_PerfMonPaused; /// If true the log file output is paused.

    eC_Bool m_bCountLog; /// If enabled the counter will be written into the log file per control point.
    eC_Bool m_bWriteLogFile; /// If enabled, the logged values will be written to the log file. Otherwise, they will only be written to GUITrace.
    
    eC_UInt m_uiCyclesPerLog; /// Number of cycles to summarize before writing a log entry
    eC_UInt m_uiCycleCount; /// Total number of measured cycles

    /// Singleton instance.
    static CGUIPerfMon ms_kPerfMon;
};
#endif //GUIPERFMON
#endif /* DOXYGEN */
#endif
