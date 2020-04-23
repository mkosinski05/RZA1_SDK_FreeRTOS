#include "DemoAnimation.h"

#include "GUI.h"
#include "GUIEasing.h"
#include "GUICompositeObject.h"

#include "GUIAnimationObserver.h"

/*Always include last*/
#include "GUIMemLeakWatcher.h"

/*Greyes out every button on the touch scroll view*/
void AnimationObserver::GreyOutButtons(const eC_Bool& bToggle)
{
    if (NULL != m_pkButtonParent)
    {
        m_pkButtonParent->SetGrayedOut(bToggle, bToggle);
    }
}

/*Checks if the animation on the dialog is running*/
void AnimationObserver::OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus, CGUIAnimation* pAnimation)
{
    if (eStatus == CGUIAnimation::ANIMATION_RUNNING)
    {
        GreyOutButtons(true);
    }
    else    // ANIMATION_STOPPED,ANIMATION_FINISHED,ANIMATION_DELETED,
    {
        GreyOutButtons(false);
    }
}

DemoAnimation::DemoAnimation() :
    m_pkAnimatedImage(NULL),
    m_pkAnimObserver(NULL),
    m_vStartX(-1),
    m_vStartY(-1),
    m_vEndX(-1),
    m_vEndY(-1),
    m_uiDuration(0)
{
}

void DemoAnimation::Init()
{
    /*Get every button of the touch scroll*/
    CGUICompositeObject* pkButtonParent = dynamic_cast<CGUICompositeObject*>(GETGUI.GetObjectByID(ANIMATION_CONTAINER));
    m_pkAnimObserver = new AnimationObserver(pkButtonParent);

    /*Set the animation values for the animatable image*/
    m_vEndX = eC_FromInt(0);

    m_pkAnimatedImage = dynamic_cast<CGUIImage*>(GETGUI.GetObjectByID(ANIMATION_IMG));
    if (NULL != m_pkAnimatedImage)
        m_vEndX = m_pkAnimatedImage->GetRelXPos();

    m_vStartX = m_vEndX;
    m_vStartY = 0;
    m_uiDuration = 1000;
}

DemoAnimation::~DemoAnimation()
{
    DeInit();
}

void DemoAnimation::DeInit()
{
    delete m_pkAnimObserver;
    m_pkAnimObserver = NULL;
}

/*Sets the index for each animation, depending what button was pressed*/
void DemoAnimation::HandleCallAPI(const eC_String& kAPI, const eC_String kParam)
{
    CGUIEasing::EasingType_t eEasingX = CGUIEasing::EASE_NONE;
    CGUIEasing::EasingType_t eEasingY = CGUIEasing::EASE_NONE;

    if (m_vEndY == -1)
    {
        CGUIImage* pkBottomBar = dynamic_cast<CGUIImage*>(GETGUI.GetObjectByID(IMG_BOTTOMBAR));
        if (NULL != pkBottomBar)
        {
            m_vEndY = pkBottomBar->GetRelYPos() - m_pkAnimatedImage->GetHeight();
        }
        else
        {
            m_vEndY = GETGUI.GetHeight() - eC_FromInt(38) - m_pkAnimatedImage->GetHeight();
        }
    }

    if (kAPI == "StartAnimation")
    {
        if (kParam == "back")
        {
            eEasingY = CGUIEasing::EASE_IN_BACK;
        }
        else if (kParam == "bounce")
        {
            eEasingY = CGUIEasing::EASE_OUT_BOUNCE;
        }
        else if (kParam == "circ")
        {
            eEasingY = CGUIEasing::EASE_IN_CIRC;
        }
        else if (kParam == "cube")
        {
            eEasingY = CGUIEasing::EASE_IN_OUT_CUBIC;
        }
        else if (kParam == "elastic")
        {
            eEasingY = CGUIEasing::EASE_OUT_ELASTIC;
        }
        else if (kParam == "expo")
        {
            eEasingY = CGUIEasing::EASE_IN_EXPO;
        }
        else if (kParam == "quad")
        {
            eEasingY = CGUIEasing::EASE_IN_QUAD;
        }
        else if (kParam == "quart")
        {
            eEasingY = CGUIEasing::EASE_OUT_QUART;
        }
        else if (kParam == "sine")
        {
            eEasingY = CGUIEasing::EASE_IN_OUT_SINE;
        }
        else if (kParam == "linear")
        {
            eEasingY = CGUIEasing::EASE_LINEAR;
        }

        CGUIAnimationMove* pkanimation = new CGUIAnimationMove(m_pkAnimatedImage, eEasingX, eEasingY, m_vStartX, m_vStartY, m_vEndX, m_vEndY, m_uiDuration);
        if (NULL != pkanimation)
        {
            pkanimation->SetAnimationObserver(m_pkAnimObserver);
            pkanimation->SetDeletedAfterFinish(true);
            pkanimation->StartAnimation();
        }
    }
}
