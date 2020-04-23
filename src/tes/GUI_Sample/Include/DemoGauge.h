#if !defined _DEMOGAUGE_H_
#define _DEMOGAUGER_H_

#include "DemoBase.h"

#include "GUIGauge.h"
#include "GUIEasing.h"
#include "GUIWheel.h"
#include "GUIValue.h"

/*Duration of the animation*/
#define ANIMATION_TIME 400


/*Class for the GaugeAndWheel-Dialog*/
class DemoGauge : public DemoBase, public CGUIAnimatable
{
public:
    DemoGauge(CGUIEasing::EasingType_t eEasingUp = CGUIEasing::EASE_LINEAR, CGUIEasing::EasingType_t eEasingDown = CGUIEasing::EASE_LINEAR) :
        m_eEasingUp(eEasingUp),
        m_eEasingDown(eEasingDown)
    {
    }

    virtual ~DemoGauge();

    virtual void Init();

    virtual void DeInit();

    virtual void HandleCallAPI(const eC_String& kAPI, const eC_String kParam);

    void SetEasing(CGUIEasing::EasingType_t eEasing)
    {
        m_eEasingUp = eEasing;
    }

private:
    CGUIValue m_vWheelValueFromDatapool;
    CGUIValue m_vValueBuffer;
    CGUIGauge* m_pkMainGauge;
    CGUIWheel* m_pkWheelControl;

    /*Easing types for the directions of scrolling*/
    CGUIEasing::EasingType_t m_eEasingUp;
    CGUIEasing::EasingType_t m_eEasingDown;
};

#endif
