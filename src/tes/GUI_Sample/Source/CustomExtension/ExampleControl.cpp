#include "ExampleControl.h"
#include "GfxWrap.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUIControlResource.h"
#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define EXAMPLE_CONTROL_CLASS_VERSION 2
// The minimal class version allowed.
#define EXAMPLE_CONTROL_CLASS_MIN_VERSION 1

ExampleControl::ExampleControl(
    CGUICompositeObject *const pParent,
    const eC_Value &vX, const eC_Value &vY,
    const eC_Value &vWidth, const eC_Value &vHeight,
    eC_UInt uiInnerColor, eC_UInt uiBorderColor,
    eC_Value vBorderWidth,
    const ObjectHandle_t &eID) :
    CGUIObject(pParent, vX, vY, vWidth, vHeight, eID),
    m_uiInnerColor(uiInnerColor),
    m_uiBorderColor(uiBorderColor),
    m_vBorderWidth(vBorderWidth)
{
    SetXMLTag("ExampleControl");
}

ExampleControl::ExampleControl() :
    m_uiInnerColor(0),
    m_uiBorderColor(0),
    m_vBorderWidth(eC_FromInt(0))
{
    SetXMLTag("ExampleControl");
}

void ExampleControl::SetCenterColor(eC_UInt uiInnerColor)
{
    if (uiInnerColor != m_uiInnerColor)
    {
        m_uiInnerColor = uiInnerColor;
        InvalidateArea();
    }
}

void ExampleControl::SetBorderColor(eC_UInt uiBorderColor)
{
    if (uiBorderColor != m_uiBorderColor)
    {
        m_uiBorderColor = uiBorderColor;
        InvalidateArea();
    }
}

void ExampleControl::SetBorderWidth(eC_Value vBorderWidth)
{
    if (vBorderWidth != m_vBorderWidth)
    {
        m_vBorderWidth = vBorderWidth;
        InvalidateArea();
    }
}

eC_Bool ExampleControl::DoDraw()
{
    CGUIRect kAbsRect(GetAbsRect());
    if (m_vBorderWidth != eC_FromInt(0))
    {
        GETGFX.SetForegroundColor(m_uiBorderColor);
        GETGFX.FilledRect(kAbsRect);
        kAbsRect.Expand(-m_vBorderWidth);
    }
    GETGFX.SetForegroundColor(m_uiInnerColor);
    GETGFX.FilledRect(kAbsRect);
    return true;
}

#if defined(GUILIANI_STREAM_GUI)
void ExampleControl::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader(EXAMPLE_CONTROL_CLASS_VERSION, EXAMPLE_CONTROL_CLASS_MIN_VERSION);

    if (cuiVersion <= 1)
    {
        m_uiInnerColor = GETINPUTSTREAM.READ_HEX("InnerColor");
        m_uiBorderColor = GETINPUTSTREAM.READ_HEX("BorderColor");
        m_vBorderWidth = eC_FromFloat(GETINPUTSTREAM.READ_FLOAT("BorderWidth"));

        CGUIObject::ReadFromStream();
    }
    else
    {
        // always base-class first
        CGUIObject::ReadFromStream();

        // remove grouping
        GETINPUTSTREAM.DELETE_COMMENT_TAG("ExampleControl");

        m_uiInnerColor = GETINPUTSTREAM.READ_HEX("InnerColor");
        m_uiBorderColor = GETINPUTSTREAM.READ_HEX("BorderColor");
        m_vBorderWidth = eC_FromFloat(GETINPUTSTREAM.READ_FLOAT("BorderWidth"));

        // remove grouping
        GETINPUTSTREAM.DELETE_COMMENT_TAG("/ExampleControl");
    }
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void ExampleControl::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader(bWriteClassID, XMLTAG_CONTROLCLASSID, CTL_EXAMPLE, EXAMPLE_CONTROL_CLASS_VERSION);

    CGUIObject::WriteToStream();
    GETOUTPUTSTREAM.WriteCommentTag("ExampleControl");
    GETOUTPUTSTREAM.WriteHex(m_uiInnerColor, "InnerColor");
    GETOUTPUTSTREAM.WriteHex(m_uiBorderColor, "BorderColor");
    GETOUTPUTSTREAM.WriteFloat(eC_ToFloat(m_vBorderWidth), "BorderWidth");

    // write attribute-group end
    GETOUTPUTSTREAM.WriteCommentTag("/ExampleControl");

    WriteStreamingFooter(bWriteClassID);
}
#endif
