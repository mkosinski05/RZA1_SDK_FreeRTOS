/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIFRAMERATECOUNTER__H_
#define GUIFRAMERATECOUNTER__H_

#include "eC_Types.h"
#include "eC_String.h"
#include "GUIFontResource.h"
#include "GUIRect.h"

/**
Class for generating counters which are administrated by the GUIFramerate class.
*/
class CGUIFramerateCounter
{
public:
    // only the CGUIFramerate class may draw and recalculate the counter
    //@TODO Check why CGUIFramerate has to be friend
    friend class CGUIFramerate;

    /** Standard CGUIFramerateCounter constructor*/
    CGUIFramerateCounter();

    /** CGUIFramerateCounter constructor with given FPS text
        the given text is added to FPS number
        i.e. given text is "overall FPS: " the output on the screen
        would be "overall FPS: 78.5"
        @param kFPSTextPrefix Text prefix to display.
    */
    CGUIFramerateCounter(eC_String kFPSTextPrefix);

    /** CGUIFramerateCounter destructor*/
    virtual ~CGUIFramerateCounter();

    /** Sets the position of the FPS counter on the screen
        @param vX x coordinate of the new position
        @param vY y coordinate of the new position
    */
    void SetPosition(eC_Value vX, eC_Value vY);

    /** Sets the color for the text
        @param uiTextColor color for the text
    */
    void SetTextColor(eC_UInt uiTextColor);

    /** Sets the color for the text
        @param uiBackColor color for the background rectangle
    */
    void SetBackColor(eC_UInt uiBackColor);

    /** Sets the font for the text
        @param eFontID font for the text to set.
    */
    void SetFont(const FontResource_t& eFontID);

    /** Set the invisibility of the FPS counter
        @param bInvisible
        */
    void SetInvisible(eC_Bool bInvisible);

    /** Get the invisibility of the FPS counter
        @return true if FPS counter is invisible otherwise false
        */
    eC_Bool GetInvisible() const;

    /** Get the last calculated FPS rate
        @return last calculated FPS rate
        */
    eC_Value GetFPS();

    /** Inform the FPS counter about a new frame */
    void CountFrame();

    /** Start the measurement for this FPS */
    void StartMeasurement();

    /** Stop the measurement for this FPS */
    void StopMeasurement();

protected:
    /// number of frames
    eC_UInt m_uiFrames;

    /// start time of measurement
    eC_Time64 m_kStartTime;

    /// measured time since the last recalculation
    eC_Time64 m_kDeltaTime;

    /// frames per second (FPS)
    eC_Value m_vFPS;

    /// x position of this FPS on the screen
    eC_Value m_vPosX;

    /// y position of this FPS on the screen
    eC_Value m_vPosY;

    /// text color of the FPS
    eC_UInt m_uiTextColor;

    /// background rectangle
    CGUIRect m_kBackRect;

    /// color of the background rectangle
    eC_UInt m_uiBackColor;

    /// current FPS text
    eC_String m_kFPSText;

    /// FPS text prefix
    eC_String m_kFPSTextPrefix;

    /// visibility of the FPS counter
    eC_Bool m_bInvisible;

    /// text font
    FontResource_t m_eFontID;

    /** draws the FPS text on the screen*/
    void Draw() const;

    /** Recalculate the FPS*/
    void Recalculate();

    /** Calculate the back rectangle*/
    void CalcBackRectangle();
};

inline void CGUIFramerateCounter::CountFrame()
{
    m_uiFrames++;
}

inline void CGUIFramerateCounter::StartMeasurement()
{
    m_kStartTime = eC_GetTicks64();
}

inline void CGUIFramerateCounter::StopMeasurement()
{
    m_kDeltaTime = m_kDeltaTime + (eC_GetTicks64() - m_kStartTime);
}

#endif //GUIFRAMERATECOUNTER__H_
