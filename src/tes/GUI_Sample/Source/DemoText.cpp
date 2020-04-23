#include "DemoText.h"

#include "GUI.h"
#include "GUIObserver.h"
#include "GUIImage.h"
/*Always include last*/
#include "GUIMemLeakWatcher.h"

ObsvTextWidth::ObsvTextWidth(CGUITextField* pkField, CGUITextField* pkFieldRich, CGUIBaseSlider* pkSlider) :
    m_pkTargetObject(pkField),
    m_pkTargetObjectRich(pkFieldRich)
{
    if (pkSlider != NULL)
    {
        pkSlider->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("TILT_OBSERVER: Slider is missing\n"));
    }
}

void ObsvTextWidth::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (m_pkTargetObject != NULL && m_pkTargetObjectRich != NULL)
    {
        /*Get the value of the left slider and set it for the tilt-angle*/
        eC_Value newVal = kObservedValue.ToValue();

        m_pkTargetObject->SetWidth(newVal);
        m_pkTargetObjectRich->SetWidth(newVal);
        m_pkTargetObject->InvalidateArea();
        m_pkTargetObjectRich->InvalidateArea();
    }
}

DemoText::DemoText() :
    m_pkWidthSlider(NULL)
{
}

DemoText::~DemoText()
{
    DeInit();
}

void DemoText::Init()
{
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_TEXT:Fetching the normal textfield Object\n"));
    CGUITextField* pkTextField = static_cast<CGUITextField*>(GETGUI.GetObjectByID(NORMAL_TEXT_FIELD));
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_TEXT:Fetching the rich textfield Object\n"));
    CGUITextField* pkTextFieldRich = static_cast<CGUITextField*>(GETGUI.GetObjectByID(RICH_TEXT_FIELD));
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_TEXT:Fetching the width slider Object\n"));
    CGUIBaseSlider* pkWidthSlider = static_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(TEXT_WIDTH));

    m_pkWidthSlider = new ObsvTextWidth(pkTextField, pkTextFieldRich, pkWidthSlider);
}

void DemoText::DeInit()
{
    delete m_pkWidthSlider;
    m_pkWidthSlider = NULL;
}
