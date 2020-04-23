#if !defined _DEMOSLIDER_H_
#define _DEMOSLIDER_H_

#include "DemoBase.h"

#include "GUIGeometryObject.h"
#include "GUIProgressBar.h"
#include "GUIImage.h"
#include "GUIBaseSlider.h"

/*Observer-Class for the slider that handles the Tilt Angle
Alternative method, in contrast to DataPool*/
class ObsvProgressImg : public CGUIObserver
{
public:
    ObsvProgressImg(CGUIProgressBar* pkCarousel, CGUIImage* pkImage, CGUIBaseSlider* pkSlider);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUIProgressBar* m_pkTargetObject;
    CGUIImage* m_pkTargetImageObject;
};


class ObsvGeometry : public CGUIObserver
{
public:
    ObsvGeometry(CGUIBaseSlider* pkSliderR, CGUIBaseSlider* pkSliderG, CGUIBaseSlider* pkSliderB, CGUIGeometryObject* pkGeometry);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

private:
    CGUIGeometryObject* m_pkGeometry;
    CGUIBaseSlider* m_pkSliderRed;
    CGUIBaseSlider* m_pkSliderGreen;
    CGUIBaseSlider* m_pkSliderBlue;
};


/*Class for the Slider-Dialog*/
class DemoSlider : public DemoBase
{
public:
    virtual ~DemoSlider();

    virtual void Init();

    virtual void DeInit();

private:
    ObsvProgressImg* m_pkProgressImg;
    ObsvGeometry* m_pkGeometryObserver;
};

#endif
