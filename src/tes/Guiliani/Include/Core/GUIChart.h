/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICHART_H_
#define GUICHART_H_

#include "eC_Types.h"
#include "eC_Types.h"
#include "GUIObject.h"
#include "GUIPoint.h"
#include "GUIFontResource.h"


typedef    eC_TArray<CGUIPoint> GraphValues;

const int  ciTickStepSizes[] = { 2, 5, 10 }; /// Contains all available step sizes. Usually 2,5,10.

/// Simple control for displaying geometrical primitives.

// @guiliani_doxygen toplevel_control Geometry Object Control
/**
<table border="0">
<tr>
<td width="200">@image html geometry_object.png</td>
<td>
The "geometry control" control offers a simple way to add geometrical primitives, such as lines or rectangles, to your user-interface
    without writing customized drawing code (Class: CGUIChart).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen

/** This class offers a simple way to add geometrical primitives, such as lines or rectangles, to your user-interface
    without writing customized drawing code.
*/
class CGUIChart : public CGUIObject
{
private:
    class CGUIGraph
    {
    public:
        CGUIGraph() : m_uiColor(0xff000000), m_vLineWidth(eC_FromInt(1)) {CalculateValueRange();};
        CGUIGraph(GraphValues &kValues, eC_UInt uiColor, eC_Value vLineWidth) : m_uiColor(uiColor), m_vLineWidth(vLineWidth) 
        {
             m_kValues.SetQuantity(kValues.GetQuantity(), false);
             memcpy( m_kValues.GetArray(), kValues.GetArray(), kValues.GetQuantity() * sizeof(GraphValues) );
             CalculateValueRange();
        };
        CGUIGraph(const CGUIGraph& kSource)
        {
            operator = (kSource);
        };
        CGUIGraph& operator=(const CGUIGraph& kSource) 
        {
            m_uiColor = kSource.m_uiColor;
            m_vLineWidth = kSource.m_vLineWidth;
            m_vXMin = kSource.m_vXMin;
            m_vXMax = kSource.m_vXMax;
            m_vYMin = kSource.m_vYMin;
            m_vYMax = kSource.m_vYMax;
            m_kValues.SetQuantity(kSource.m_kValues.GetQuantity(), false);
            memcpy( m_kValues.GetArray(), kSource.m_kValues.GetArray(), kSource.m_kValues.GetQuantity() * sizeof(GraphValues) );
            return *this;
        };
        void Draw(eC_Value vXPos, eC_Value vYPos, eC_Value vXScaleFactor, eC_Value vYScaleFactor);
        eC_UInt GetColor() const { return m_uiColor;};
        eC_Value GetLineWidth() const { return m_vLineWidth;};
        const GraphValues& GetValues() const { return m_kValues;}; 
        void CalculateValueRange()
        {
            m_vXMin = eC_MAXVALUE, m_vXMax = -eC_MAXVALUE, m_vYMin = eC_MAXVALUE, m_vYMax = -eC_MAXVALUE;
            
            for(eC_UInt i=0 ; i < m_kValues.GetQuantity(); ++i)
            {
                m_vXMin = eC_Min(m_kValues[i].m_vX, m_vXMin);
                m_vXMax = eC_Max(m_kValues[i].m_vX, m_vXMax);
                m_vYMin = eC_Min(m_kValues[i].m_vY, m_vYMin);
                m_vYMax = eC_Max(m_kValues[i].m_vY, m_vYMax);
            }
        }; 
        void GetValueRange(eC_Value &vXMin, eC_Value & vXMax, eC_Value &vYMin, eC_Value &vYMax) 
        {
            CalculateValueRange();
            vXMin = m_vXMin, vXMax = m_vXMax, vYMin = m_vYMin, vYMax = m_vYMax;
        }; 
    private:
        GraphValues m_kValues;
        eC_UInt     m_uiColor;
        eC_Value    m_vLineWidth;
        eC_Value    m_vXMin;
        eC_Value    m_vXMax;
        eC_Value    m_vYMin;
        eC_Value    m_vYMax;
    };

    class CGUIAxis
    {
        enum LabelAlignment_t
        {
            LA_ORIGIN,  ///< Axis which goes through origin in x direction.
            LA_XAXIS,  ///< Axis which goes through origin in y direction.
            LA_YAXIS,  ///< Axis will be from left to right at m_vUser1AxisXPosY
        };
    public:
        CGUIAxis() : m_uiColor(0xff000000), m_uiBGColor(0xffffffff), m_uiLabelColor(0xff000000), m_uiTicksColor(0xffbbbbbb), m_uiGridColor(0xffbbbbbb), m_vLineWidth(eC_FromInt(1)), 
            m_ptOrigin(CGUIPoint()), m_vLabelDistFromAxis(eC_FromInt(6)) {};

        CGUIAxis(GraphValues &kValues, eC_UInt uiColor, eC_UInt uiBGColor, eC_UInt uiLabelColor, eC_UInt uiTicksColor, eC_UInt uiGridColor, eC_Value vLineWidth, CGUIPoint ptOrigin) : 
            m_uiColor(uiColor), m_uiBGColor(uiBGColor), m_uiLabelColor(uiLabelColor), m_uiTicksColor(uiTicksColor), m_uiGridColor(uiGridColor),
            m_vLineWidth(vLineWidth), m_ptOrigin(ptOrigin), m_vLabelDistFromAxis(eC_FromInt(6)) 
        {
            m_kValues.SetQuantity(kValues.GetQuantity(), false);
             memcpy( m_kValues.GetArray(), kValues.GetArray(), kValues.GetQuantity() * sizeof(GraphValues) );
        };

        CGUIAxis(const CGUIAxis& kSource)
        {
            operator = (kSource);
        }

        void Draw(eC_Value vXPos, eC_Value vYPos, CGUIRect rcValueRange, eC_Value vXScaleFactor, eC_Value vYScaleFactor, eC_Bool bShowTicks = false,
            eC_String kOriginLabel = eC_String(""), eC_String kXAxisLabel = eC_String(""), eC_String kYAxisLabel = eC_String(""));

        void DrawGrid(eC_Value vXPos, eC_Value vYPos, CGUIRect rcValueRange, eC_Value vXScaleFactor, eC_Value vYScaleFactor);
        void DrawTickLabel(eC_Value vXPos, eC_Value vYPos, eC_Value vValue, eC_Bool bIsXAxis);
        void DrawAxisLabels(eC_Value vXPos, eC_Value vYPos, eC_String &kLabel, LabelAlignment_t eAlignment);

        eC_UInt CalCNumOfDigits(eC_Value vValue);

        CGUIAxis& operator=(const CGUIAxis& kSource) 
        {
            m_uiColor = kSource.m_uiColor;
            m_uiBGColor = kSource.m_uiBGColor;
            m_uiLabelColor = kSource.m_uiLabelColor;
            m_uiTicksColor = kSource.m_uiTicksColor;
            m_uiGridColor = kSource.m_uiGridColor;
            m_vLineWidth = kSource.m_vLineWidth;
            m_ptOrigin = kSource.m_ptOrigin;
            m_vLabelDistFromAxis = kSource.m_vLabelDistFromAxis;
            m_kValues.SetQuantity(kSource.m_kValues.GetQuantity(), false);
            memcpy( m_kValues.GetArray(), kSource.m_kValues.GetArray(), kSource.m_kValues.GetQuantity() * sizeof(GraphValues) );
            return *this;
        }

        eC_Bool IsXAxis()
        {
            if (m_kValues.GetQuantity() > 1)
            {
                if(m_kValues[0].m_vX == m_kValues[1].m_vX)
                    return false;
            }
            return true;
        }

        /**
        SelectTickStep calculates "sensible" step for representing labels, drawing ticks etc. on graphs
        vMmin and vMax define range of values to be represented it is assumed that desired number of "ticks"
        (BaseStep is a difference between two neighbour ticks) should be between 8 and 20.
        @param vMin Minimum value of the range where ticks should be drawn.
        @param vMax Minimum value of the range where ticks should be drawn.
        @return Step value which should match for the given range (In units of the graph not pixels).
        */    
        eC_Value SelectTickStep(eC_Value vMin, eC_Value vMax) 
        {
            double dRange = eC_ToFloat(vMax - vMin);
            double dLogRange = log10(dRange);
            dLogRange = ceil(dLogRange) - 1;
            double dStep = pow(10.0, dLogRange);

            // selecting BaseStep as step divided by 1, 2 or 5
            double dBaseStep = dStep;
            double dTicksNum = dRange / dBaseStep;
            int i=0;
            while( dTicksNum < 8 )
            {
                dBaseStep = dStep / ciTickStepSizes[i++];
                dTicksNum = dRange / dBaseStep;
            }
            return eC_FromFloat(dBaseStep);
        }

        eC_Value CalcPixelStep(eC_Value vTickStep, eC_Value vMinPixelStep, eC_Value vMaxPixelStep) 
        {
            if (vTickStep == eC_FromInt(0)) return eC_FromInt(0);
            if ((vTickStep >= vMinPixelStep && vTickStep <= vMaxPixelStep) || (vMinPixelStep == vMaxPixelStep))
                return vTickStep;
            else 
            {
                // TickStep has a form M * pow(10, P), where P is power of
                double dPower = log10(eC_ToFloat(vTickStep));
                double dP = ceil(dPower) -1;
                double dPow10P = pow(10.0, dP);
                double dM = eC_ToFloat(vTickStep) / dPow10P;
                if (dM >= 10)
                {
                    dPow10P *= 10;
                }

                eC_Int iOldK = 1, iK = 1, i = 0;
                if(vTickStep < vMinPixelStep) // increasing TickStep to fit into the given pixel range 
                {
                    double minK = eC_ToFloat(vMinPixelStep) / dPow10P;
                    while (iK < minK)
                    {
                        if(i <2)
                            iK = iOldK * ciTickStepSizes[i++];
                        else 
                        {
                            iK = iOldK = iOldK * 10;
                            i = 0;
                        }
                    }
                    vTickStep = eC_FromFloat(dPow10P * iK);
                }
                else if (vTickStep > vMaxPixelStep) 
                { // decreasing TickStep to make it less then maxPix
                    dPow10P *= 10;
                    double maxK = dPow10P / eC_ToFloat(vMaxPixelStep);
                    while (iK < maxK)
                    {
                        if(i <2)
                            iK = iOldK * ciTickStepSizes[i++];
                        else 
                        {
                            iK = iOldK = iOldK * 10;
                            i = 0;
                        }
                    }
                    vTickStep = eC_FromFloat(dPow10P / iK);
                }
                return vTickStep;
            }
        }

    private:
        GraphValues m_kValues;
        eC_UInt     m_uiColor;
        eC_UInt     m_uiBGColor;
        eC_UInt     m_uiLabelColor;
        eC_UInt     m_uiTicksColor;
        eC_UInt     m_uiGridColor;
        eC_Value    m_vLineWidth;
        CGUIPoint   m_ptOrigin;
        eC_Value    m_vLabelDistFromAxis;
    };

public:
    /// Enumeration for object shapes
    enum GraphType_t
    {
        GT_LINE,  ///< Diagonal line, from top/left to bottom/right corner of the object
        GT_BAR,  ///< Diagonal line, from top/right to bottom/left corner of the object
        GT_PIE, ///< Vertical line, centered within the object
        //CT_AREA, ///< Horizontal line, centered within the object
        //CT_BUBBLE ///< Ellipse, centered within the object
    };

    /** Enumeration of the various axis which can be displayed
    */
    enum AxisIndex_t
    {
        AI_ORIGIN_X,  ///< Axis which goes through origin in x direction.
        AI_ORIGIN_Y,  ///< Axis which goes through origin in y direction.
        AI_USER1_X,  ///< Axis will be from left to right at m_vUser1AxisXPosY
        AI_USER1_Y,  ///< Axis will be from top to bottom at m_vUser1AxisYPosX
        AI_USER2_X,  ///< Axis will be from left to right at m_vUser2AxisXPosY
        AI_USER2_Y,  ///< Axis will be from top to bottom at m_vUser2AxisYPosX
    };

    /** Constructor
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the geometry object
        @param vHeight Height of the geometry object
        @param uiColor Color of drawing
        @param eGraphType Shape descriptor. Valid values are defined in CGUIChart::GraphType_t
        @param bFilled Fill-out object (Currently only supported for rectangles)
        @param eID Object identifier of this object (choose NO_HANDLE if none is required)
    */
    CGUIChart(
        CGUICompositeObject* const pParent,
        const eC_Value& vX,
        const eC_Value& vY,
        const eC_Value& vWidth,
        const eC_Value& vHeight,
        const eC_UInt&  uiColor,
        const GraphType_t eGraphType,
        const eC_Bool&  bFilled = false,
        const ObjectHandle_t& eID = NO_HANDLE);

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

    /** Standard constructor. */
    CGUIChart();

    /** Destructor */
    virtual ~CGUIChart();

    virtual eC_Bool DoDraw();

    /**
    Sets the color of the primitive as a 32 Bit hexadecimal value (0xAARRGGBB).
    @param uiColor Color of drawing
    */
    void SetColor(const eC_UInt& uiColor);

    /**
    Sets the shape of the primitive as an enum.
    @param eChartType Shape descriptor. Valid values are defined in CGUIChart::GraphType_t
    */
    void SetChartType(const GraphType_t eChartType);

    /**
    Sets the drawing thickness of the primitive.
    @param vThickness Thickness (in pixels) of pen
    */
    void SetThickness(const eC_Value& vThickness);

    /**
    Determines whether the object should be filled.
    @param bFilled Fill-out object. (Currently only supported for rectangles).
    */
    void SetFilled(const eC_Bool& bFilled);

    /**
    Convert coordinates in graph-context to coordinates relative to the object
    @param kGraphPt point in graph-coordinates
    @return the corresponding coordinates relative to the object
    */
    CGUIPoint TranslateGraphToObjectCoord(const CGUIPoint& kGraphPt);

    /**
    Convert coordinates relative to the object to graph-coordinates
    @param kObjectPt point in object-coordinates
    @return the corresponding graph-coordinates
    */
    CGUIPoint TranslateObjectToGraphCoord(const CGUIPoint& kObjectPt);

    /**
    Scale a given point along the axis via an x- and y-factor
    @param kPt the point to scale
    @param vXFactor scaling-factor along the x-axis
    @param vYFactor scaling-factor along the y-axis
    @return the scaled point
    */
    CGUIPoint Scale(const CGUIPoint& kPt, eC_Value vXFactor, eC_Value vYFactor);

    virtual void SetWidth(const eC_Value &vX);

    virtual void SetHeight(const eC_Value &vY);

private:
    void Init();
    void CalculateChartValueRange();

    /**
    Recalculate objects after resizing
    */
    void RecalcAreasAndAxis();

    /**
    Simulate various input values and display
    */
    void Simulate();

    eC_Bool             m_bFilled;
    eC_UInt             m_uiBGColor;

    eC_Bool             m_bLabels;
    eC_UInt             m_uiLabelColor;
    eC_String           m_kTitle;
    eC_String           m_kLabelOrigin;
    eC_String           m_kLabelX;
    eC_String           m_kLabelY;
    FontResource_t      m_eFont;

    CGUIRect            m_rcScaledArea;
    CGUIRect            m_rcGraphArea;
    CGUIPoint           m_ptOrigin;

    eC_Value            m_ValueRangeMinX;
    eC_Value            m_ValueRangeMaxX;
    eC_Value            m_ValueRangeMinY;
    eC_Value            m_ValueRangeMaxY;

    eC_Value            m_vTicksEveryX;
    eC_Value            m_vTicksEveryY;

    eC_Bool             m_bAutoScale;
    eC_Value            m_vScalefactorX;
    eC_Value            m_vScalefactorY;

    eC_Bool             m_bShowXAxis;
    eC_UInt             m_uiXAxisColor;
    eC_Value            m_vXAxisLineWidth;
    eC_Bool             m_bShowYAxis;
    eC_UInt             m_uiYAxisColor;
    eC_Value            m_vYAxisLineWidth;

    eC_Bool             m_bShowUser1XAxis;
    eC_Value            m_vUser1XAxisPos;
    eC_UInt             m_uiUser1XAxisColor;
    eC_Value            m_vUser1XAxisLineWidth;

    eC_Bool             m_bShowUser1YAxis;
    eC_Value            m_vUser1YAxisPos;
    eC_UInt             m_uiUser1YAxisColor;
    eC_Value            m_vUser1YAxisLineWidth;

    eC_Bool             m_bShowUser2XAxis;
    eC_Value            m_vUser2XAxisPos;
    eC_UInt             m_uiUser2XAxisColor;
    eC_Value            m_vUser2XAxisLineWidth;

    eC_Bool             m_bShowUser2YAxis;
    eC_Value            m_vUser2YAxisPos;
    eC_UInt             m_uiUser2YAxisColor;
    eC_Value            m_vUser2YAxisLineWidth;

    eC_Bool             m_bShowGrid;
    eC_UInt             m_uiGridColor;

    eC_Bool             m_bShowTicks;
    eC_UInt             m_uiTicksColor;

    eC_Bool             m_bSimulateGraphs;

    CGUIRect            m_rcValueRange;

    typedef eC_TListDoubleLinked<CGUIGraph> GraphList;
    typedef eC_TListDoubleLinked<CGUIAxis> AxesList;

    GraphList m_kGraphList;
    AxesList m_kAxesList;

    eC_TArray<GraphType_t> m_kGraphType;
};

#endif // GEOMETRYOBJECT_H_
