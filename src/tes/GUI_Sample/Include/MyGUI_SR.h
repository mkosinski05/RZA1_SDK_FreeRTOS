/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _MYGUI_SR_H_
#define _MYGUI_SR_H_

#include "DemoAnimation.h"
#include "DemoCarousel.h"
#include "DemoGauge.h"
#include "DemoKeyboard.h"
#include "DemoSettings.h"
#include "DemoSlider.h"
#include "DemoText.h"

#include "GUITextField.h"
#include "GUIAnimatable.h"
#include "GUIAnimationObserver.h"
#include "GUIGeometryObject.h"

#include "StreamRuntimeGUI.h"
#include "GUIObjectHandleResource.h"

#define GETMYGUI    static_cast<CMyGUI&>(GETGUI)

class TextAnimation
    :public CGUIAnimatable
{
public:
    TextAnimation(eC_UByte ubStartVal, eC_UByte ubEndVal);
    virtual ~TextAnimation();

    virtual void DoAnimate(const eC_Value &vTimes);

private:
    CGUITextField* m_pkTextField;
    eC_UByte m_ubStartVal;
    eC_UByte m_ubEndVal;
    eC_UByte m_ubCalc;
};

class BoxObserver
    :public CGUIAnimationObserver
{
public:

    BoxObserver(eC_Bool bShow);
    virtual ~BoxObserver();

    virtual void OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus, CGUIAnimation* pkAnimation);

private:
    eC_Bool m_bShow;
    TextAnimation* m_pkTextAnimation;
};

// Application specific CGUI instance. Implemented by customer.
class CMyGUI : public NStreamRuntime::CStreamRuntimeGUI, public CGUIObserver
{
public:
    enum SelectedMenu_t
    {
        MENU_UNKNOWN,
        MENU_ANIMATION,
        MENU_BUTTONS,
        MENU_SLIDER,
        MENU_TEXT,
        MENU_GAUGE,
        MENU_CAROUSEL,
        MENU_KEYBOARD,
        MENU_SCRATCHPAD,
        MENU_SETTINGS,
        MENU_COUNT
    };

public:
    CMyGUI(eC_Value x, eC_Value y, eC_Value width, eC_Value height, ObjectHandle_t eID);
    ~CMyGUI();

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

    /// Example implementation for simple Application <-> GUI communication
    eC_Bool CallApplicationAPI(const eC_String& kAPI, const eC_String& kParam);

    virtual void DoAnimate(const eC_Value &vTimes);

    static void pvLedButtonCallback(CDataPoolEntry& data);

private:
    /* Method for toggleing between richtext and normal text, just flips the invisibility */
    void EnableRichText();

    /* Call the different language set*/
    void ChangeLang(const eC_Bool& toggle);

    /* Enable / Disable NEON optimization*/
    void ChangeNeon(const eC_Bool& toggle);

    /* Enable / Disable FPS display*/
    void ShowFPS(const eC_Bool& toggle);

    /* Enable / Disable Bilinear Blit*/
    void ChangeBlitBilinear(const eC_Bool& bToggle);

    /* Changes the name of a textfiel depending on the focused element of the carousel */
    void ChangeMenuName();

    /* Changes the image set between dark and light */
    void ChangeSkin(const eC_Bool& toggle);

    /* Change background color of a dialog */
    void ChangeBackground(const ObjectHandle_t& eObjectID);

    /* Create the field that displays the Info text for the diffrent dialogs */
    void AnimateInfoTxt(const TextResource_t& eID);

    void SetTransition(const ObjectHandle_t& button);

    void Cleanup();

    void NotifyOfDestruction(const CGUIObject* pDestructedObject);

    void ShowDialog(const ObjectHandle_t& eDialogID);

    void ShowInfoText(const eC_String& kText);

    void SetTransition(const eC_String& kText);

    void CheckExportStreamingMode();

    void ShowGlassPane();

private:
    DemoAnimation* m_pkDialogAnimation;
    DemoCarousel* m_pkDialogCarousel;
    DemoGauge* m_pkDialogGauge;
    DemoSettings* m_pkDialogSettings;
    DemoSlider* m_pkDialogSlider;
    DemoText* m_pkDialogText;
    DemoKeyboard* m_pkDialogKeyboard;

    BoxObserver* m_pkBoxObserver;
    CGUITransitionCmd::TransitionType_t m_eTransitionType;
    ImageResource_t m_eBackgroundImage;
    CGUIEasing::EasingType_t m_eGaugeEasingType;

    SelectedMenu_t m_eSelectedMenu;
    eC_UInt m_uiBackgroundColor;
    DemoSettings::ImageMode_t m_eImageMode;
    eC_String m_kExportStreamingMode;

    CGUIGeometryObject* m_pkGlassPane;
    TextResource_t MenuTexts[9];
};

#endif //#ifndef _MYGUI_SR_H_
