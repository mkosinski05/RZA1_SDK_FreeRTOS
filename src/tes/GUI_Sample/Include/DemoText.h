#if !defined _DEMOTEXT_H_
#define _DEMOTEXT_H_

#include "DemoBase.h"

#include "GUITextField.h"
#include "GUIBaseSlider.h"

/*Observer-Class for the slider that handles the Tilt Angle
Alternative method, in contrast to DataPool*/
class ObsvTextWidth : public CGUIObserver
{
public:
    ObsvTextWidth(CGUITextField* pkField, CGUITextField* pkFieldRich, CGUIBaseSlider* pkSlider);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUITextField* m_pkTargetObject;
    CGUITextField* m_pkTargetObjectRich;
};

/*Class for the Carousel-Dialog*/
class DemoText : public DemoBase
{
public:
    DemoText();

    virtual ~DemoText();

    virtual void Init();

    virtual void DeInit();

private:
    ObsvTextWidth* m_pkWidthSlider;
};

#endif
