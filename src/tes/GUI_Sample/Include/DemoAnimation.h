#if !defined _DEMOANIMATION_H_
#define _DEMOANIMATION_H_

#include "DemoBase.h"

#include "GUIAnimationObserver.h"
#include "GUIButton.h"
#include "GUIImage.h"
#include "GUIAnimationMove.h"

class AnimationObserver : public CGUIAnimationObserver
{
public:
    AnimationObserver(CGUICompositeObject* pkButtonParent) :
        m_pkButtonParent(pkButtonParent)
    {
    }

    /*Greyes out every button on the touch scroll view*/
    void GreyOutButtons(const eC_Bool& bToggle);

    /*Checks if the animation on the dialog is running*/
    virtual void OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus, CGUIAnimation* pkAnimation);

private:
    CGUICompositeObject* m_pkButtonParent;
};

/*Class the inherits the demo base, this class handles everything that is on the Animation-Dialog*/
class DemoAnimation : public DemoBase
{
public:
    DemoAnimation();

    virtual ~DemoAnimation();

    virtual void Init();

    virtual void DeInit();

    virtual void HandleCallAPI(const eC_String& kAPI, const eC_String kParam);

private:
    CGUIImage* m_pkAnimatedImage;
    AnimationObserver* m_pkAnimObserver;
    eC_Value m_vStartX, m_vStartY, m_vEndX, m_vEndY;
    eC_UInt m_uiDuration;
};

#endif
