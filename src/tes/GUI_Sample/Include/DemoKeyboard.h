#if !defined _DEMOKEYBOARD_H_
#define _DEMOKEYBOARD_H_

#include "DemoBase.h"

#include "GUIBaseCheckBox.h"
#include "GUIEdit.h"
#include "GUIBaseSlider.h"

/*Observer-Class for the slider that handles the Tilt Angle
Alternative method, in contrast to DataPool*/
class ObsvShowPassword : public CGUIObserver
{
public:
    ObsvShowPassword(CGUIEdit* pkPasswordField, CGUIBaseCheckBox* pkCheckbox);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUIBaseCheckBox* m_pkCheckbox;
    CGUIEdit* m_pkPasswordField;
};

/*Class for the Carousel-Dialog*/
class DemoKeyboard : public DemoBase
{
public:
    DemoKeyboard();

    virtual ~DemoKeyboard();

    virtual void Init();

    virtual void DeInit();

private:
    ObsvShowPassword* m_pkShowPassword;
};

#endif
