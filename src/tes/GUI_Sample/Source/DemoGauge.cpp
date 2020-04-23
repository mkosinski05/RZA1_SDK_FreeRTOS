#include "DemoGauge.h"

#include "GUI.h"

#include "GUITimer.h"
#include "GUIDataPool.h"
/*Always include last*/
#include "GUIMemLeakWatcher.h"

DemoGauge::~DemoGauge()
{
    DeInit();
}

void DemoGauge::Init()
{
    /*Get the gauge that has to be animated*/
    m_pkMainGauge = dynamic_cast<CGUIGauge*>(GETGUI.GetObjectByID(GAUGE_ANIMATED));
    m_pkWheelControl = dynamic_cast<CGUIWheel*>(GETGUI.GetObjectByID(VERTICAL_WHEEL));
}

void DemoGauge::DeInit()
{
    GETTIMER.RemoveAnimationCallback(this);
}

/*Sets the wheel to certain values, depending on the button press*/
void DemoGauge::HandleCallAPI(const eC_String& kAPI, const eC_String kParam)
{
    if (kAPI == "HighWheel")
    {
        if (m_pkWheelControl)
        {
            m_pkWheelControl->ScrollToAnimated(eC_String("300"));
            m_pkWheelControl->InvalidateArea();
        }
    }
    else if (kAPI == "MiddleWheel")
    {
        if (m_pkWheelControl)
        {
            m_pkWheelControl->ScrollToAnimated(eC_String("150"));
            m_pkWheelControl->InvalidateArea();
        }
    }
    else if (kAPI == "LowWheel")
    {
        if (m_pkWheelControl)
        {
            m_pkWheelControl->ScrollToAnimated(eC_String("0"));
            m_pkWheelControl->InvalidateArea();
        }
    }
}
