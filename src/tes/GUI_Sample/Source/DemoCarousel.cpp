#include "DemoCarousel.h"

#include "GUI.h"
#include "GUIObserver.h"
#include "GUIImage.h"
#include "GUIText.h"
#include "GUITextField.h"
#include "GUIButton.h"
/*Always include last*/
#include "GUIMemLeakWatcher.h"


ObsvSliderTilt::ObsvSliderTilt(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider) :
    m_pkTargetObject(pkCarousel)
{
    if (pkSlider)
    {
        pkSlider->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("TILT_OBSERVER: Slider is missing\n"));
    }
}

void ObsvSliderTilt::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (m_pkTargetObject)
    {
        /*Get the value of the left slider and set it for the tilt-angle*/
        eC_Value newVal = kObservedValue.ToValue();
        m_pkTargetObject->SetTiltAngle(newVal);

        m_pkTargetObject->InvalidateArea();
    }
}

ObsvSliderRadius::ObsvSliderRadius(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider) :
    m_pkTargetObject(pkCarousel)
{
    if (pkSlider)
    {
        pkSlider->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("RADIUS_OBSERVER: Slider is missing\n"));
    }
}

void ObsvSliderRadius::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (m_pkTargetObject)
    {
        /*get the value of the right slider and set it for the radius*/
        eC_Value newVal = kObservedValue.ToValue();
        m_pkTargetObject->SetRadius(newVal);

        m_pkTargetObject->InvalidateArea();
    }
}

ObsvSliderCount::ObsvSliderCount(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider) :
    m_pkTargetObject(pkCarousel),
    m_uiCount(5)
{
    if (pkSlider != NULL)
    {
        pkSlider->AddValueObserver(this);
    }
    else
    {
        GUILOG(GUI_TRACE_ERROR, eC_String("COUNT_OBSERVER: Slider is missing\n"));
    }

    m_eImages[0] = IMG_CAROUSELIMG;
    m_eImages[1] = IMG_CAROUSELIMG2;
    m_eImages[2] = IMG_CAROUSELIMG3;
    m_eImages[3] = IMG_CAROUSELIMG4;
    m_eImages[4] = IMG_CAROUSELIMG5;
}

void ObsvSliderCount::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (m_pkTargetObject != NULL)
    {
        eC_Value newVal = kObservedValue.ToValue();
        eC_UInt numOfChild = m_pkTargetObject->GetNumberOfChildren();
        if (newVal < numOfChild)
        {
            for (; m_uiCount >= newVal; m_uiCount--)
            {
                // Remove the Object from the carousel
                CGUIObject* pkObject = m_pkTargetObject->GetChild(m_uiCount);
                if (NULL != pkObject)
                {
                    m_pkTargetObject->RemoveObject(pkObject);
                    delete pkObject;
                }
                m_pkTargetObject->InvalidateArea();
            }
        }
        else
        {
            eC_UInt tmp = m_uiCount;
            for (; m_uiCount < tmp + newVal - numOfChild; m_uiCount++)
            {
                // Add another object to the carousel get first child as template
                eC_Value vTargetWidth, vTargetHeight;
                m_pkTargetObject->GetOriginalSize(0, vTargetWidth, vTargetHeight);
                CGUIButton* pkButton = new CGUIButton(m_pkTargetObject, 0, 0, vTargetWidth, vTargetHeight, eC_String(m_uiCount + 2), NULL);
                if (NULL != pkButton)
                {
                    pkButton->GetLabel()->SetFont(HEADER_FNT);
                    pkButton->GetNinePatch().Assign(5, 5, 5, 5);

                    ImageResource_t eImage = m_eImages[(m_uiCount + 1) % 5];
                    pkButton->SetImages(eImage, eImage, eImage, eImage, eImage);
                    pkButton->SetAlpha(192);
                    m_pkTargetObject->AddObject(pkButton);
                }
                m_pkTargetObject->InvalidateArea();
            }
        }
    }
}

/*Get every needed element of the dialog and assign it to the observers*/
void DemoCarousel::Init()
{
    // reset flowmode on init
    m_bFlowModeActive = false;

    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_CAROUSEL:Fetching the Carousel Object\n"));
    m_pkCarousel = static_cast<CGUICarousel*>(GETGUI.GetObjectByID(CAROUSEL_TEST));
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_CAROUSEL:Fetching the tiltSlider Object\n"));
    CGUIBaseSlider* pkSliderTilt = static_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_TILT));
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_CAROUSEL:Fetching the radiSlider Object\n"));
    CGUIBaseSlider* pkSliderRadius = static_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(SLIDER_RADIUS));
    GUILOG(GUI_TRACE_DEBUG, eC_String("DEMO_CAROUSEL:Fetching the numSlider Object\n"));
    CGUIBaseSlider* pkSliderNumber = static_cast<CGUIBaseSlider*>(GETGUI.GetObjectByID(NUMBER_OF_ELEMENTS));

    if (NULL != m_pkCarousel)
    {
        m_pkObserverTilt = new ObsvSliderTilt(m_pkCarousel, pkSliderTilt);
        m_pkObserverRadius = new ObsvSliderRadius(m_pkCarousel, pkSliderRadius);
        m_pkObserverNumber = new ObsvSliderCount(m_pkCarousel, pkSliderNumber);
    }
}

void DemoCarousel::DeInit()
{
    delete m_pkObserverTilt;
    m_pkObserverTilt = NULL;
    delete m_pkObserverRadius;
    m_pkObserverRadius = NULL;
    delete m_pkObserverNumber;
    m_pkObserverNumber = NULL;
}

DemoCarousel::~DemoCarousel()
{
    DeInit();
}

/*Toggle modes between flow and carousel mode*/
void DemoCarousel::HandleCallAPI(const eC_String& kAPI, const eC_String kParam)
{
    if (kAPI == "ToggleFlow")
    {
        ChangeCarouselMode();
    }
}

void DemoCarousel::ChangeCarouselMode()
{
    if (NULL != m_pkCarousel)
    {
        m_bFlowModeActive = !m_bFlowModeActive;
        CGUIObject* pSelectedObj = m_pkCarousel->GetSelectedObject();
        m_pkCarousel->EnableFlowMode(m_bFlowModeActive);

        if (m_bFlowModeActive)
        {
            m_pkCarousel->SetPerspectiveFactor(eC_FromFloat(1.2));
        }
        else
        {
            m_pkCarousel->SetPerspectiveFactor(eC_FromFloat(0.5));
        }

        if (pSelectedObj)
        {
            m_pkCarousel->JumpToObject(pSelectedObj);
        }
        m_pkCarousel->InvalidateArea();
    }
}
