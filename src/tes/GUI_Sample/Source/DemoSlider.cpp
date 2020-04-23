#include "DemoSlider.h"

#include "GUI.h"

/*Always include last*/
#include "GUIMemLeakWatcher.h"

ObsvProgressImg::ObsvProgressImg(CGUIProgressBar* pkProgressBar, CGUIImage* pkImage, CGUIBaseSlider* pkSlider) :
    m_pkTargetObject(pkProgressBar),
    m_pkTargetImageObject(pkImage)
{
    if (pkSlider)
    {
        pkSlider->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("SLIDER_OBSERVER: Slider is missing\n"));
    }
}

void ObsvProgressImg::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (m_pkTargetObject)
    {
        eC_Value newVal = kObservedValue.ToValue();

        /*Change the value for the progress bar*/
        if (m_pkTargetObject)
        {
            m_pkTargetObject->SetValue(newVal);
            m_pkTargetObject->InvalidateArea();
        }

        /*Depending on the value of the slider, the image will have a different image*/
        if (m_pkTargetImageObject)
        {
            if (newVal < 75)
            {
                m_pkTargetImageObject->SetImage(IMG_ROUND_BUTTON_STD_GRAY);
            }
            else if (newVal > 225)
            {
                m_pkTargetImageObject->SetImage(IMG_ROUND_BUTTON_HOVER_BLUE);
            }
            else
            {
                m_pkTargetImageObject->SetImage(IMG_ROUND_BUTTON_PRESSED_GREEN);
            }
            m_pkTargetImageObject->InvalidateArea();
        }
        else
        {
            GUILOG(GUI_TRACE_ERROR, eC_String("SLIDER_OBSERVER: IMAGE COULD NOT BE FOUND\n"));
        }
    }
}

ObsvGeometry::ObsvGeometry(CGUIBaseSlider* pkSliderR, CGUIBaseSlider* pkSliderG, CGUIBaseSlider* pkSliderB, CGUIGeometryObject* pkGeometry) :
    m_pkGeometry(pkGeometry),
    m_pkSliderRed(pkSliderR),
    m_pkSliderGreen(pkSliderG),
    m_pkSliderBlue(pkSliderB)
{
    if (NULL != m_pkSliderRed)
    {
        m_pkSliderRed->AddValueObserver(this);
    }

    if (NULL != m_pkSliderGreen)
    {
        m_pkSliderGreen->AddValueObserver(this);
    }

    if (NULL != m_pkSliderBlue)
    {
        m_pkSliderBlue->AddValueObserver(this);
    }

    OnNotification(0, m_pkSliderRed, 0, 0);
    OnNotification(0, m_pkSliderGreen, 0, 0);
    OnNotification(0, m_pkSliderBlue, 0, 0);
}

void ObsvGeometry::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (NULL != m_pkGeometry)
    {
        eC_UInt uiColor = m_pkGeometry->GetColor();
        if (pkUpdatedObject == m_pkSliderRed)
        {
            eC_UByte ubSlider = static_cast<eC_UByte>(eC_ToInt(m_pkSliderRed->GetRange().GetValue()));
            uiColor = (uiColor & 0xff00ffff) + (ubSlider << 16);
            m_pkGeometry->SetColor(uiColor);
        }
        else if (pkUpdatedObject == m_pkSliderGreen)
        {
            eC_UByte ubSlider = static_cast<eC_UByte>(eC_ToInt(m_pkSliderGreen->GetRange().GetValue()));
            uiColor = (uiColor & 0xffff00ff) + (ubSlider << 8);
            m_pkGeometry->SetColor(uiColor);
        }
        else if (pkUpdatedObject == m_pkSliderBlue)
        {
            eC_UByte ubSlider = static_cast<eC_UByte>(eC_ToInt(m_pkSliderBlue->GetRange().GetValue()));
            uiColor = (uiColor & 0xffffff00) + (ubSlider);
            m_pkGeometry->SetColor(uiColor);
        }
        m_pkGeometry->InvalidateArea();
    }
}


DemoSlider::~DemoSlider()
{
    DeInit();
}

void DemoSlider::Init()
{
    CGUIProgressBar* pkProgressBar = dynamic_cast<CGUIProgressBar*>(GETGUI.GetObjectByID(PROGRESS_HORIZONTAL));
    CGUIImage* pkImage = dynamic_cast<CGUIImage*>(GETGUI.GetObjectByID(IMG_CHANGE));
    CGUIBaseSlider* pkSlider = dynamic_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_HORIZONTAL));

    m_pkProgressImg = new ObsvProgressImg(pkProgressBar, pkImage, pkSlider);

    CGUIGeometryObject* pkGeometry = dynamic_cast<CGUIGeometryObject*>(GETGUI.GetObjectByID(GEO_COLOR));
    CGUIBaseSlider* pkSliderRed = dynamic_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_R));
    CGUIBaseSlider* pkSliderGreen = dynamic_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_G));
    CGUIBaseSlider* pkSliderBlue = dynamic_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_B));

    m_pkGeometryObserver = new ObsvGeometry(pkSliderRed, pkSliderGreen, pkSliderBlue, pkGeometry);
}

void DemoSlider::DeInit()
{
    delete m_pkProgressImg;
    m_pkProgressImg = NULL;

    delete m_pkGeometryObserver;
    m_pkGeometryObserver = NULL;
}
