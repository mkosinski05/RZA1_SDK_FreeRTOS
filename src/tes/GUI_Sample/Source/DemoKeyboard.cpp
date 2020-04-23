#include "DemoKeyboard.h"

#include "GUI.h"
#include "GUIEditableText.h"
#include "GUIObserver.h"
#include "GUIImage.h"
/*Always include last*/
#include "GUIMemLeakWatcher.h"

ObsvShowPassword::ObsvShowPassword(CGUIEdit* pkPasswordField, CGUIBaseCheckBox* pkCheckbox) :
    m_pkCheckbox(pkCheckbox),
    m_pkPasswordField(pkPasswordField)
{
    if (m_pkCheckbox != NULL)
    {
        m_pkCheckbox->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("TILT_OBSERVER: Slider is missing\n"));
    }
}

void ObsvShowPassword::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if ((NULL != m_pkPasswordField) && (NULL != m_pkCheckbox))
    {
        eC_Bool bSelected = m_pkCheckbox->IsSelected();
        m_pkPasswordField->GetLabel()->SetPasswordMode(!bSelected);

        m_pkPasswordField->InvalidateArea();
    }
}

DemoKeyboard::DemoKeyboard() :
    m_pkShowPassword(NULL)
{
}

DemoKeyboard::~DemoKeyboard()
{
    DeInit();
}

void DemoKeyboard::Init()
{
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_KEYBOARD: get Password-field\n"));
    CGUIEdit* pkPasswordField = static_cast<CGUIEdit*>(GETGUI.GetObjectByID(EDIT_PASSWORD));
    CGUIBaseCheckBox* pkCheckbox = static_cast<CGUIBaseCheckBox*>(GETGUI.GetObjectByID(CHK_SHOWPASS));

    m_pkShowPassword = new ObsvShowPassword(pkPasswordField, pkCheckbox);
}

void DemoKeyboard::DeInit()
{
    delete m_pkShowPassword;
    m_pkShowPassword = NULL;
}
