/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIFRAMERATE__H_
#define GUIFRAMERATE__H_

#include "GUITrace.h"
#include "eC_TList_doubleLinked.h"
#include "GUIFramerateCounter.h"
#include "GUIAnimation.h"

#define GETFPS CGUIFramerate::GetInstance()

/** 
GUI FPS measarument class.
Class for generating FPS measurements of the GUILIANI components
using standard GfxWrapper calls. There already two default FPS counter:
    - DEFAULT_FPS (0) meassures only the time needed for drawing GUILIANI controls
    - DEFAULT_ABS_FPS (1) meassures the time for a complete loop in CGUI::Run
*/
class CGUIFramerate: public CGUIAnimation
{
    /** The GUI class is the only one which may draw the FPS counter.
        This is done in the Run method*/
    friend class CGfxWrap;

public:
    /** Enumeration for the counter IDs*/
    enum CounterID
    {
        INVALID_COUNTER_ID = -1,
        DEFAULT_FPS,
        DEFAULT_ABS_FPS
    };

    /** Method to access the one and only instance (Singleton)
        @return The instance of this class.
    */
    static inline CGUIFramerate& GetInstance();

    /** Adds a counter.
        @param kFPSTextPrefix Text prefix to display.
        @return id of the new counter
    */
    CounterID AddCounter(eC_String kFPSTextPrefix = "FPS: ");

    /** Sets the position of the FPS counter with the given id on the screen.
        @param eCounterID id of the counter to change
        @param vX x coordinate of the new position
        @param vY y coordinate of the new position
    */
    void SetPosition(CounterID eCounterID, eC_Value vX, eC_Value vY) const;

    /** Sets the color for the text.
        @param eCounterID id of the counter to change
        @param uiTextColor color for the text
    */
    void SetTextColor(CounterID eCounterID, eC_UInt uiTextColor);

    /** Sets the color for the text.
        @param eCounterID id of the counter to change
        @param uiBackColor color for the background rectangle
    */
    void SetBackColor(CounterID eCounterID, eC_UInt uiBackColor) const;

    /** Sets the font for the text.
        @param eCounterID id of the counter to change
        @param eFontID font for the text to set
    */
    void SetFont(CounterID eCounterID, const FontResource_t& eFontID) const;

    /** Set the invisiility of all FPS counter.
        By default all counters are invisible and has to be set visible by the
        application.
        @param bInvisible*/
    void SetInvisible(eC_Bool bInvisible);

    /** Set the invisiility of the FPS counter with the given id.
        By default all counters are invisible and has to be set visible by the
        application.
        @param eCounterID id of the counter to change
        @param bInvisible*/
    void SetInvisible(CounterID eCounterID, eC_Bool bInvisible) const;

    /** Get the invisibility of the FPS counter.
        @param eCounterID id of the counter to change
        @return true if FPS counter is invisible otherwise false
        if the ID does not exist true is returned*/
    eC_Bool GetInvisible(CounterID eCounterID);

    /** Get the frames per second of the default FPS counter.
        @param eCounterID id of the counter
        @return frames per second for the given counter id*/
    eC_Value GetFPS(CounterID eCounterID = DEFAULT_FPS) const;

    /** Inform the FPS counter about a new frame.
        @param eCounterID ID of the counter.
      */
    void CountFrame(CounterID eCounterID) const;

    /** Start the measurement for the FPS.
        @param eCounterID ID of the counter.
    */
    void StartMeasurement(CounterID eCounterID) const;

    /** Stop the measurement for the FPS.
        @param eCounterID ID of the counter.
    */
    void StopMeasurement(CounterID eCounterID) const;

    /** Updates the FPS text and avoids that the GUI goes into sleep mode if there's nothing to do.
        @param uiElapsedTime Time elapsed (in ms) since start of animation.
    */
    void ApplyAnimation(eC_UInt uiElapsedTime);

private:
    /// the one and only framerate instance
    static CGUIFramerate* ms_pkInstance;

    /// internal time
    eC_Time64 m_kInternalTimer;

    /// list of framerate counters
    eC_TListDoubleLinked<CGUIFramerateCounter*> m_kCounterList;

    /// ID for the next counter to add
    CounterID m_eLastCounterID;

    /**stanadrd CGUIFramerate constructor
       @see SetPosition*/
    CGUIFramerate();

    /** CGUIFramerate destructor*/
    virtual ~CGUIFramerate();

    /** The single instance needs to be deleted*/
    static void DeleteInstance();

    /** calculates the current FPS for all counters*/
    void Calculate();

    /** draws all visible FPS counters on the screen*/
    void Draw() const;

    /** Access a counter with given ID.
        @param eCounterID ID of teh the counter to access
        @return Pointer to the counter withe given ID, if there is no counter with
        given ID, a NULL pointer is returned.*/
    inline CGUIFramerateCounter* GetCounter(CounterID eCounterID) const;
};

inline CGUIFramerateCounter* CGUIFramerate::GetCounter(CounterID eCounterID) const
{
    //@todo: rework
    CGUIFramerateCounter* pkCounter = NULL;
    eC_TIterator<CGUIFramerateCounter*> kCurrCounterIter = m_kCounterList.GetBegin();
    for (eC_Int i = 0; (i < eCounterID) && kCurrCounterIter.IsValid(); ++i)
    {
        ++kCurrCounterIter;
    }
    if (kCurrCounterIter.IsValid())
    {
        pkCounter = *kCurrCounterIter;
    }

    return pkCounter;
}

inline CGUIFramerate& CGUIFramerate::GetInstance()
{
    if (NULL == ms_pkInstance)
    {
        ms_pkInstance = new CGUIFramerate();
    }
    return *ms_pkInstance;
}

inline void CGUIFramerate::CountFrame(CounterID eCounterID) const
{
    CGUIFramerateCounter* pkCounter = GetCounter(eCounterID);
    if (NULL != pkCounter)
    {
        pkCounter->CountFrame();
    }
    else
    {
        GUILOG(GUI_TRACE_DEBUG, "CGUIFramerate::CountFrame: There is no Counter with ID: " + eC_String(eCounterID) + eC_String("\n"));
    }
}

inline void CGUIFramerate::StartMeasurement(CounterID eCounterID) const
{
    CGUIFramerateCounter* pkCounter = GetCounter(eCounterID);
    if (NULL != pkCounter)
    {
        pkCounter->StartMeasurement();
    }
    else
    {
        GUILOG(GUI_TRACE_DEBUG, ("CGUIFramerate::StartMeasurement: There is no Counter with ID: ") + eC_String(eCounterID) + eC_String("\n"));
    }
}

inline void CGUIFramerate::StopMeasurement(CounterID eCounterID) const
{
    CGUIFramerateCounter* pkCounter = GetCounter(eCounterID);
    if (NULL != pkCounter)
    {
        pkCounter->StopMeasurement();
    }
    else
    {
        GUILOG(GUI_TRACE_DEBUG, eC_String("CGUIFramerate::StopMeasurement: There is no Counter with ID: ") + eC_String(eCounterID) + eC_String("\n"));
    }
}
#endif //GUIFRAMERATE__H_
