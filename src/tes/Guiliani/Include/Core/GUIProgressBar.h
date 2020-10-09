/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIPROGRESSBAR__H_
#define CGUIPROGRESSBAR__H_

#include "GUIImageResource.h"
#include "GUIAbstractBar.h"
#include "GUINinePatch.h"

/// A bitmap based progress bar.

// @guiliani_doxygen toplevel_control Progress Bar
/**
    <table border="0">
        <tr>
            <td width="200">@image html progress_bar.png</td>
            <td>
                The "progress bar" control is used to give the user an indication of how long an operation is going to take, and show that the running task is still in progress. 
                It can also be used for every kind of bar graph visualization. The "progress bar" control consists of two images, one for the 
                background and one for the bar itself (Class: CGUIProgressBar). The "progress bar" supports nine patch images (CGUINinePatch).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    A progress bar control is used to give the user an indication of how long an operation
    is going to take, and to show that the application has not frozen.

    The progress bar can be oriented vertically or horizontally. It has two types named normal type or activity type.
    A normal type progress bar indicates fractions of a maximum amount.
    It fills the progressing area from left to right or bottom to up normally. This can be reversed by using the SetBase() API.

    The activity mode is used to indicate an indeterminate progress bar, i.e. the duration of an operation
    can not be determined. There are several Loop-Modes which define how the progressbar is animated in this case.

    Normally the progressing area is determined by the given foreground image. It can not exceed the whole
    control's dimension. The range can be changed with SetFullRangeSize().

    If the Foreground image is smaller than the bar itself, it will be scaled to the required size. 
    You can customize the foreground image's scaling behaviour through it's CGUINinePatch. The background image will automatically 
    be scaled by leaving the left and right half untouched and only stretching the middle pixel(s) to reach the desired size.

    If the Foreground image is at least the same size as the bar itself, it will not be scaled/stretched.
    Instead it will be "cut off" to the required size.

    Example:
@code

    // Create a progress bar.
    CGUIProgressBar* pkProgressBar = new CGUIProgressBar(this,
                                            eC_FromInt(0), eC_FromInt(0),
                                            eC_FromInt(100), eC_FromInt(30),
                                            NO_HANDLE);

    // Set images.
    // Before calling this method, please add image IDs to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
    pkProgressBar->SetImages(IMG_FOREGROUND,
                             IMG_BACKGROUND);

    // Set the type of progress bar.
    pkProgressBar->SetTypeOfProgressBar(CGUIProgressBar::PBT_PROGRESSBAR_TYPE_ACTIVITY);

    // Set the type of looping mode to be used when type activity.
    pkProgressBar->SetLoopMode(CGUIProgressBar::LM_REVERTLOOP_MODE);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIProgressBar : public CGUIAbstractBar
{
public:
    /// Progress bar types.
    enum ProgressBarType_t
    {
        /** Traditional progress bar. The bar is static and a level or
            percentage of progress is shown in the control.
        */
        PBT_PROGRESSBAR_TYPE_NORMAL,
        /** The bar inside the control moves from one end to another as specified by the
            looping mode (see LoopMode_t enumerator for information on loop modes). Apart
            from this an animation interval and step size has to be set (see SetAnimationInterval()
            and SetStepSize()) to define animation speed.
            Bar dimensions are set by using SetFullRangeSize().
        */
        PBT_PROGRESSBAR_TYPE_ACTIVITY
    };

    /// Loop modes for PBT_PROGRESSBAR_TYPE_ACTIVITY.
    enum LoopMode_t
    {
        /// The full bar travels from start to end repeatedly.
        LM_LOOP_MODE,
        /// Cycles the full bar from start to end and reverts direction from end to start repeatedly.
        LM_REVERTLOOP_MODE,
        /// Fills up the bar from start to end until full and starts again empty repeatedly.
        LM_FILL_UP
    };

    /** Constructor.
        By default, the progress bar type is set to be PBT_PROGRESSBAR_TYPE_NORMAL.
        The level range is from 0 to 100.
        The step size is set to 1 pixel.
        The dimension of the bar is determined after the
        foreground image is set. Here it is set to be 0 by 0.
        @param pParent Pointer to parent object.
        @param vX X position relative to its parent object.
        @param vY Y position relative to its parent object.
        @param vWidth Width of the control.
        @param vHeight Height of the control.
        @param eID object handle for the control.
    */
    CGUIProgressBar(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);


    /** Constructor.
        By default, the progress bar type is set to be PBT_PROGRESSBAR_TYPE_NORMAL.
        The level range is from 0 to 100.
        The step size is set to 1 pixel.
        The dimension of the bar is determined by the foreground image.
        It can not exceed the dimension of the control.
        @param pParent Pointer to parent object.
        @param vX X position relative to its parent object.
        @param vY Y position relative to its parent object.
        @param vWidth Width of the control.
        @param vHeight Height of the control.
        @param eBackground Background image of the control.
        @param eForeground Foreground image of the control.
        @param eID Object handle for the control.
    */
    CGUIProgressBar(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ImageResource_t &eBackground,
        const ImageResource_t &eForeground,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIProgressBar class.
        @param kSource Source object to be copied.
    */
    CGUIProgressBar(const CGUIProgressBar& kSource);


    /** Operator= method of the CGUIProgressBar class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIProgressBar& operator=(const CGUIProgressBar& kSource);

    /// Standard draw function.
    eC_Bool DoDraw();

    /** Sets the foreground and background images.
        If the bar area dimension is not determined previously, it is set here.
        @param eForeground Foreground image of the control.
        @param eBackground Background image of the control.
    */
    void SetImages(const ImageResource_t eForeground, const ImageResource_t eBackground);

    /// Standard Timer/Animation function.
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** The function SetFullRangeSize is used to set the range of bar inside
        the Progress bar control. If the bar is drawn vertically, the bar's height
        is changed, otherwise, the bar's width is changed.
        @param vSize Bar's range.
    */
    void SetFullRangeSize(const eC_Value &vSize);

    /** @return The bar's full range size.
    */
    inline eC_Value GetFullRangeSize() const
    {
        return m_bDrawVertical ? m_vBarHeight : m_vBarWidth;
    }

    /** This function sets the type of progress bar to be shown in the control.
        @param eType Progress bar type.
    */
    void SetTypeOfProgressBar(ProgressBarType_t eType);

    /** This function gets the type of progress bar to be shown in the control.
        @return Progress bar type
    */
    inline ProgressBarType_t GetTypeOfProgressBar() const
    {
        return m_eProgressBarType;
    }

    /** This function defines the mode in which the ProgressBar will loop, if it is
        of type PBT_PROGRESSBAR_TYPE_ACTIVITY.
        This will not have any effect for ProgressBars of type PBT_PROGRESSBAR_TYPE_NORMAL
        @param eLM Loop mode for the control.
    */
    inline void SetLoopMode(LoopMode_t eLM)
    {
        m_eLoopMode = eLM;
    }

    /** This function return the mode in which the ProgressBar will loop.
        @see SetLoopMode()
        @return Loop mode for the control
    */
    inline LoopMode_t GetLoopMode() const
    {
        return m_eLoopMode;
    }

    /** Increases the level/percentage of the bar by the value that is set with
        SetStepSize().
        @return True if successful, False otherwise
    */
    eC_Bool Increase();

    /** Decreases the level/percentage of the bar by the value that is set with
        SetStepSize().
        @return True if successful, False otherwise
    */
    eC_Bool Decrease();

    /** Change the draw direction of this control. This decides whether the control
        is a vertical progress bar or a horizontal progress bar.
        @param bVert Vertical draw flag.
    */
    void SetVertical(eC_Bool bVert)
    {
        m_bDrawVertical = bVert;
    }

    /// Gets the drawing flag to see whether the progress bar is a vertical one or horizontal one.
    inline eC_Bool IsVertical() const
    {
        return m_bDrawVertical;
    }

    /// Returns the X position of the bar.
    inline eC_Value GetBarXPos() const
    {
        return m_vBarX;
    }

    /// Returns the Y position of the bar.
    inline eC_Value GetBarYPos() const
    {
        return m_vBarY;
    }

    /// Returns the width of the bar.
    inline eC_Value GetBarWidth() const
    {
        return m_vBarWidth;
    }

    /// Returns the height of the bar.
    inline eC_Value GetBarHeight() const
    {
        return m_vBarHeight;
    }

    /** Sets the position of the bar by given rect.
        Note that this does not affect the entire object's size/position, but only the
        "progressing area" of the actual bar inside of it.
        @param kBarRect Rect to set.
    */
    inline void SetBarRect(CGUIRect& kBarRect)
    {
        m_vBarX = kBarRect.GetX1();
        m_vBarY = kBarRect.GetY1();
        m_vBarWidth = kBarRect.GetWidth();
        m_vBarHeight = kBarRect.GetHeight();
        InvalidateArea();
    }

    /** Sets the X position of the bar.
        @see SetBarRect();
        @param vX X position.
    */
    void SetBarXPos(const eC_Value &vX);

    /** Sets the Y position of the bar.
        @see SetBarRect();
        @param vY Y position.
    */
    void SetBarYPos(const eC_Value &vY);

    /** Sets the width of the bar.
        @see SetBarRect();
        @param vWidth width to set.
    */
    void SetBarWidth(const eC_Value &vWidth);

    /** Sets the height of the bar.
        @see SetBarRect();
        @param vHeight height to set.
    */
    void SetBarHeight(const eC_Value &vHeight);

    ///    Destructor
    ~CGUIProgressBar();

    /** Change the width of this control. The progressing area is changed proportionally.
        @param vWidth Width of the control.
    */
    virtual void SetWidth(const eC_Value &vWidth);

    /** Change the height of this control. The progressing area is changed proportionally.
        @param vHeight Height of the control.
    */
    virtual void SetHeight(const eC_Value &vHeight);

    /** Grants access to the nine patch which is used for scaling the foreground image.
        @return Nine patch used to display the object */
    CGUINinePatch& GetNinePatch() { return m_kNinePatch; }

#ifdef GUILIANI_STREAM_GUI
    CGUIProgressBar();

    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:

    /** Internal method called by Increase() and Decrease().
        @param bInc true to increase a step, false to decrease a step.
        @return Always True
    */
    eC_Bool IncrementValue(eC_Bool bInc);

    /** Draw the normal bar
    */
    virtual void DrawTypeNormalBar();

    /** Draw the activity bar.
    */
    virtual void DrawTypeActivityBar();

    /// Help function to initialize the control.
    void Init(const ImageResource_t &eForeground, const ImageResource_t &eBackground);

    /** Copy attributes from source control to this control.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    void CopyAttributes(const CGUIProgressBar& kSource);

    /// Deinitialize the control and free the resources.
    void DeInit();

    ///    Background image ID.
    ImageResource_t m_eImgBackground;

    ///    Foreground image ID.
    ImageResource_t m_eImgForeground;

    ///    The x position of the progressing area relative to the control.
    eC_Value m_vBarX;

    ///    The y position of the progressing area relative to the control.
    eC_Value m_vBarY;

    ///    The width of the progressing area.
    eC_Value m_vBarWidth;

    ///    The height of the progressing area.
    eC_Value m_vBarHeight;

    /// The progress bar type.
    ProgressBarType_t m_eProgressBarType;

    /// Loop mode used in the progress bar with activity type.
    LoopMode_t m_eLoopMode;

    /// Indicates whether the progress bar is a vertical bar or horizontal bar.
    eC_Bool m_bDrawVertical;

    /// Indicates the direction in which the bar will be moving in Activity mode
    eC_Bool m_bDirection;
    
    /// Nine patch borders for scaling the foreground image
    CGUINinePatch m_kNinePatch;
};
#endif
