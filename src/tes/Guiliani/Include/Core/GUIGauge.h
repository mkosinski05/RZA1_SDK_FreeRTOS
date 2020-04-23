/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIGAUGE__H_
#define GUIGAUGE__H_

#include "GUIAbstractBar.h"
#include "GUIImageResource.h"

/// Basic gauge control, which visualizes values on a meter using a needle.
// @guiliani_doxygen toplevel_control Gauge
/**
    <table border="0">
        <tr>
            <td width="200">@image html gauge.png</td>
            <td>
                The "gauge" control is used to visualize values on a meter using a needle. The visualization can be customized by either supplying images, for both the background and the needle, or by drawing the needle with a line of customizable color and length. The usage of the "gauge" control is very easy as only minimal value, maximal value and maximal rotation have to be defined. The current position of the needle is calculated automatically through the given value. The "gauge" control can be used in a wide range of use-cases e.g. to present an analog clock or a speedometer (Class: CGUIGauge).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The gauge is derived from the CGUIAbstractBar baseclass. It defines a range of valid values along with
    a minimum- and maximum value for the contained needle. It visualizes any value within the valid range by
    rotating the needle to the respective angle.

    The visualization can be customized by either supplying images, for both the background and the needle,
    or by drawing the needle with a line of customizable color and length. The simple line-based visualization is
    useful as a fallback solution if you do not have access to the final needle graphics yet.

    Another application of the gauge control is as an interactive object, which enables the user to select a
    value from a given range. This can be done by using the CGUIAbstractBar::SetStepSize and CGUIAbstractBar::SetMinValue /
    CGUIAbstractBar::SetMaxValue APIs. The control is then operated via the Increase() and Decrease() APIs of
    CGUIAbstractBar, which are by default mapped onto GK_PAGEDOWN / GK_PAGEUP.

    @note Since the needle is only allowed to reside at valid positions (=integers between given Minimum and Maximum)
    its movement may appear jerky e.g., if the range of values is 0-100 and the range of angles is 0-180. If you wish
    to achieve a smooth rotation, increase the range of values.

    Example:
@code
    // Create gauge object (visualized using bitmaps)
    CGUIGauge* pGauge = new CGUIGauge( pkPage4,
                                       eC_FromInt(300), eC_FromInt(300), // Position
                                       eC_FromInt(400), eC_FromInt(400), // Size
                                       0, 2000,                          // Value range
                                       eC_FromInt(-20), eC_FromInt(200), // Min- and Max angle of needle
                                       eGaugeImage, eNeedleImage);       // Bitmaps for background and needle

    // Sets the gauge to the given value (The needle will reflect the value automatically)
    pGauge->SetValue((eC_Int)50);

    // Increases the gauge to the next valid value
    pGauge->Increase();

    // Reconfigures the gauge to only allow (integer!) values from 0 through 9
    pGauge->SetMinValue(0);
    pGauge->SetMaxValue(9);

    // Simulates the needle movement by animating the needle with dummy values
    pGauge->SetSimulate( true);
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIGauge
    : public CGUIAbstractBar
{
public:
    /** Constructs a gauge with a vector needle.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the gauge.
        @param vHeight Height of the gauge.
        @param iMin Minimum allowed value
        @param iMax Maximum allowed value
        @param vMinAngle Minimum allowed needle angle
        @param vMaxAngle Maximum allowed needle angle
        @param eGaugeImage Image used to represent the gauge itself (without needle)
        @param vNeedleLength Length of the needle in pixels (From center of gauge to its tip)
        @param eID ObjectID of the gauge
    */
    CGUIGauge(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Int iMin, const eC_Int iMax, const eC_Value vMinAngle, const eC_Value vMaxAngle,
        const ImageResource_t eGaugeImage, const eC_Value &vNeedleLength, const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a gauge with a bitmap needle
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the gauge.
        @param vHeight Height of the gauge.
        @param iMin Minimum allowed value
        @param iMax Maximum allowed value
        @param vMinAngle Minimum allowed needle angle
        @param vMaxAngle Maximum allowed needle angle
        @param eGaugeImage Image used to represent the gauge itself (without needle)
        @param eNeedleImage Image used to represent the needle (Will be rotated to corresponding angle)
        @param eID ObjectID of the gauge
    */
    CGUIGauge(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Int iMin, const eC_Int iMax, const eC_Value vMinAngle, const eC_Value vMaxAngle,
        const ImageResource_t eGaugeImage, const ImageResource_t eNeedleImage, const ObjectHandle_t &eID = NO_HANDLE);

    virtual ~CGUIGauge();

    virtual eC_Bool DoDraw();

    /** Sets the minimum allowed rotation angle (in degrees) for the needle.
        0 = Needle points to the left
        90 = Needle points upwards
        180 = Needle points right
        270 = Needle points downwards
        @param vMinAngle New minimum angle
    */
    void SetMinAngle( const eC_Value vMinAngle);

    /** Sets the maximum allowed rotation angle (in degrees) for the needle.
        @param vMaxAngle New maximum angle
        @see SetMinAngle
    */
    void SetMaxAngle( const eC_Value vMaxAngle);

    /** Simulates needle movement by feeding the gauge with dummy values.
        This is especially useful during a project's prototyping phase
        @param bSimulate TRUE starts simulation mode, FALSE stops simulation */
    void SetSimulate( const eC_Bool bSimulate);

    /// Animation is used within simulation mode
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Sets length for vector needle.
        <b>This has no effect when using a bitmap for the needle</b>
        @param vLength New needle length in pixels
    */
    void SetNeedleLength( const eC_Value vLength);

    /** Sets the image, which will be used for visualizing the needle.
        The needle image will be centered within the gauge and will be rotated around its own center.
        It will always be displayed in its original size, regardless of the surrounding gauge's size.
        @param eNeedleImageID The image ID
    */
    void SetNeedleImage( const ImageResource_t eNeedleImageID);

    /// Sets the image, which will be used for visualizing the gauge
    void SetGaugeImage( const ImageResource_t eGaugeImageID);

    /// Streaming Implementation
#ifdef GUILIANI_WRITE_GUI
    void WriteToStream(const eC_Bool bWriteClassID);
#endif
#ifdef GUILIANI_STREAM_GUI
    CGUIGauge();
    void ReadFromStream();
#endif

protected:

    /// Internal helper method for initialization
    void Init(const eC_Int iMin, const eC_Int iMax, const eC_Value vMinAngle, const eC_Value vMaxAngle,const ImageResource_t eGaugeImage);

    /** Called by the framework whenever the GUIAbstractBar's (and therefore the gauge's value) changes.
        This causes an update of the needle's angle.
        @return Always True
    */
    virtual eC_Bool OnValueChange();

    /// Helper method for drawing the gauge
    virtual void DrawGauge();

    /// Helper method for drawing the needle
    virtual void DrawNeedle();

    /// Improved invalidation method, which does not simply invalidate the entire object, but instead only the area covered by the needle.
    void SmartInvalidation();

    /// Calculates the endpoint of the needle's tip after rotation
    void CalculateEndpoint();

    /// Updates the needle's rotationangle with respect to Min/Max Value and Min/Max Angle
    void UpdateNeedleAngle();

    /// Lower limit of rotation angles
    eC_Value    m_vMinAngle;
    /// Upper limit of rotation angles
    eC_Value    m_vMaxAngle;
    /// Current rotation angle
    eC_Value    m_vAngle;
    /// X coordinate of the needle endpoint, relative to the center of the gauge
    eC_Value    m_vEndX;
    /// Y coordinate of the needle endpoint, relative to the center of the gauge
    eC_Value    m_vEndY;
    /// Length of vector needle
    eC_Value    m_vLength;
    /// Color of the needle
    eC_UInt     m_uiNeedleColor;
    /// Image used for gauge
    ImageResource_t m_eGaugeImage;
    /// Image used for needle
    ImageResource_t m_eNeedleImage;
    /// Indicates if simulation mode is active or not
    eC_Bool     m_bSimulate;
};

#endif
