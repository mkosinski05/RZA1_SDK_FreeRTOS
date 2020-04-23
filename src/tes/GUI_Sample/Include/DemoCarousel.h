#if !defined _DEMOCAROUSEL_H_
#define _DEMOCAROUSEL_H_

#include "DemoBase.h"

#include "GUIBaseSlider.h"
#include "GUICarousel.h"

#include "GUIImageResource.h"

/*Observer-Class for the slider that handles the Tilt Angle
Alternative method, in contrast to DataPool*/
class ObsvSliderTilt : public CGUIObserver
{
public:
    ObsvSliderTilt(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUICarousel* m_pkTargetObject;
};

/*Observer-Class for the slider that handles the Radius
Alternative method, in contrast to DataPool*/
class ObsvSliderRadius : public CGUIObserver
{
public:
    ObsvSliderRadius(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUICarousel* m_pkTargetObject;
};

/*Observer-Class for the slider that handles the Radius
Alternative method, in contrast to DataPool*/
class ObsvSliderCount : public CGUIObserver
{
public:
    ObsvSliderCount(CGUICarousel* pkCarousel, CGUIBaseSlider* pkSlider);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUICarousel* m_pkTargetObject;

    ImageResource_t m_eImages[5];
    eC_UInt m_uiCount;
};

/*Class for the Carousel-Dialog*/
class DemoCarousel : public DemoBase
{
public:
    DemoCarousel() :
        m_pkObserverTilt(NULL),
        m_pkObserverRadius(NULL),
        m_pkObserverNumber(NULL),
        m_pkCarousel(NULL),
        m_bFlowModeActive(false)
    {
    }

    virtual ~DemoCarousel();

    virtual void Init();

    virtual void DeInit();

    virtual void HandleCallAPI(const eC_String& kAPI, const eC_String kParam);

private:
    /*method for toggleing between carousel and flow mode*/
    void ChangeCarouselMode();

private:
    ObsvSliderTilt* m_pkObserverTilt;
    ObsvSliderRadius* m_pkObserverRadius;
    ObsvSliderCount* m_pkObserverNumber;
    CGUICarousel* m_pkCarousel;

    eC_Bool m_bFlowModeActive;
};

#endif
