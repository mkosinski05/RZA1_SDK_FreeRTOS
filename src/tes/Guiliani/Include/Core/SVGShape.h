#ifndef CSVGSHAPE_H
#define CSVGSHAPE_H

#include "eC_Types.h"

struct GradientStop_t
{
    eC_UInt m_uiColor;
    float m_vOffset;
};

/** A SVG-gradient */
class CGradient
{
public:
    CGradient() :
        m_eGradientType(LINEAR),
        m_eSpread(PAD),
        m_bIsStroke(false),
        m_avXform(),
        m_vFx(1.f),
        m_vFy(1.f)
    {
    };

    typedef enum
    {
        LINEAR,
        RADIAL
    } GradientType_t;

    typedef enum
    {
        PAD,
        REFLECT,
        REPEAT
    } Spread_t;

    GradientType_t m_eGradientType;
    Spread_t m_eSpread;

    eC_Bool m_bIsStroke;

    float m_avXform[6];

    float m_vFx, m_vFy;//focalpoint
    std::vector<GradientStop_t> m_akGradientStops;
};

struct Path_t
{
    //a sequence of cubic bezier-curves
    eC_Bool m_bClosed;
    std::vector<float> m_avCoords;//x,y,x,y,x,y...
};

/** This class represents a SVG-form.
A SVG-image is just a row of objects from this type.
*/
struct CSVGShape
{
    CSVGShape() :
        m_eType(JUST_STROKE),
        m_eCapstyle(SQUARE_CAP),
        m_eJoinstyle(MITER_JOIN),
        m_eFillrule(NON_ZERO),
        m_vStrokeDashOffset(0.0f),
        m_ubOpacity(),
        m_uiFillColor(0),
        m_uiStrokeColor(0),
        m_vStrokeWidth(0),
        m_avBounds()
    {
    };

    typedef enum
    {
        JUST_STROKE,
        JUST_FILL,
        STROKE_AND_FILL
    } Form_Type_t;

    typedef enum
    {
        BUTT_CAP,
        ROUND_CAP,
        SQUARE_CAP
    } Capstyle_t;

    typedef enum
    {
        BEVEL_JOIN,
        ROUND_JOIN,
        MITER_JOIN
    } Joinstyle_t;

    typedef enum
    {
        NON_ZERO,
        EVEN_ODD
    } Fillrule_t;

    Form_Type_t m_eType;
    Capstyle_t  m_eCapstyle;
    Joinstyle_t m_eJoinstyle;
    Fillrule_t  m_eFillrule;

    //Dashpattern
    float m_vStrokeDashOffset;
    std::vector<float> m_avStrokeDashArray;//max size from NanoSVG = 8

    eC_UByte    m_ubOpacity; //0 = fully transparent; 255 = fully opaque
    eC_UInt     m_uiFillColor;
    eC_UInt     m_uiStrokeColor;
    eC_Value    m_vStrokeWidth;

    float m_avBounds[4];    //tight bounding box of the shape [minx,miny,maxx,maxy].

    std::vector<CGradient> m_akGradients;
    std::vector<Path_t> m_akPaths;
};

#endif // CSVGSHAPE_H
