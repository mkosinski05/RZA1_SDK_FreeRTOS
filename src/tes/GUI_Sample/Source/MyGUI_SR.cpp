/*
* Copyright (C) 2004 TES Electronic Solutions GmbH,
* All Rights Reserved.
* This source code and any compilation or derivative thereof is the
* proprietary information of TES Electronic Solutions GmbH
* and is confidential in nature.
* Under no circumstances is this software to be exposed to or placed
* under an Open Source License of any type without the expressed
* written permission of TES Electronic Solutions GmbH
*
*############################################################
*/

/******************************************************************************
*   PROJECT:        Guiliani
*******************************************************************************
*
*    MODULE:        MyGUI_SR.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/GSE/branches/Releases/1.0_Guiliani_2.1/StreamRuntime/src/MyGUI_SR.cpp $
*
*    Date created:  2005
*
*
*
*    Author:        JRE
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: MyGUI_SR.cpp 2159 2014-11-26 15:36:46Z christian.euler $
*
******************************************************************************/
#include "MyGUI_SR.h"

#include "GUI.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUIBaseTextField.h"
#include "GUICarousel.h"
#include "GUITextField.h"
#include "GUILocalisationHandler.h"
#include "GUISlider.h"
#include "GUIScrollingTextField.h"
#include "GUIResourceManager.h"
#include "GUIGeometryObject.h"
#include "GUIAnimationMove.h"
#include "GUIAnimationSize.h"
#include "GUIAnimationObserver.h"
#include "GUIBaseMessageBox.h"

#include "GUIDatapool.h"
#include "GUIEventHandler.h"
#include "GUIPlaybackInput.h"

#include "UserConfig.h"

#ifdef GFX_USE_EGML
#include "GfxWrapeGML.h"
#endif

#include "StreamRuntimeConfig.h"
#include "GUIFramerate.h"

extern "C"
{
#include <stdbool.h>
#include "r_typedefs.h"
#include "r_led_drv_api.h"
#include "r_os_abstraction_api.h"
#include "r_compiler_abstraction_api.h"
#include "iodefine_cfg.h"

#include "r_led_drv_api.h"
#include "FreeRTOS.h"
#include "task.h"

/* Dependencies */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "spibsc_iobitmask.h"
#include "cpg_iobitmask.h"
#include "gpio_iobitmask.h"
}

#if __ICCARM__ == 1
extern "C"
{
    int open(const char *filename, int amode);
    int close(int handle);
}
#endif


// LAST INCLUDE!!
#include "GUIMemLeakWatcher.h"

//---------------------------------------------------------------
//Text Animation
//---------------------------------------------------------------
TextAnimation::TextAnimation(eC_UByte ubStartVal, eC_UByte ubEndVal) :
    m_pkTextField(NULL),
    m_ubStartVal(ubStartVal),
    m_ubEndVal(ubEndVal)
{
    m_pkTextField = static_cast<CGUITextField*>(GETGUI.GetObjectByID(INFO_TEXT));
    GETTIMER.AddAnimationCallback(40, this);
    m_ubCalc = m_ubStartVal;
}

TextAnimation::~TextAnimation()
{
    GETTIMER.RemoveAnimationCallback(this);
    m_pkTextField = NULL;
}

void TextAnimation::DoAnimate(const eC_Value &vTimes)
{
    if (m_ubCalc != m_ubEndVal)
    {
        m_ubCalc = (m_ubCalc + m_ubEndVal) / 2;
    }

    if (NULL != m_pkTextField)
    {
        m_pkTextField->SetAlpha(eC_UByte(m_ubCalc));
        m_pkTextField->InvalidateArea();
    }
}

//---------------------------------------------------------------
//Box Observer
//---------------------------------------------------------------
BoxObserver::BoxObserver(eC_Bool bShow) :
    m_bShow(bShow),
    m_pkTextAnimation(NULL)
{
    m_pkTextAnimation = NULL;
}

BoxObserver::~BoxObserver()
{
    if (m_pkTextAnimation != NULL)
    {
        delete m_pkTextAnimation;
        m_pkTextAnimation = NULL;
    }
}

/*Checks if the animation on the dialog is running*/
void BoxObserver::OnStatusChanged(CGUIAnimation::AnimationStatus_t eStatus, CGUIAnimation* pkAnimation)
{
    if (eStatus == CGUIAnimation::ANIMATION_FINISHED)
    {
        if (m_bShow == true)
        {
            m_pkTextAnimation = new TextAnimation(0, 255);
        }
        else
        {
            m_pkTextAnimation = new TextAnimation(255, 0);
        }
        GETGUI.InvalidateChildren();
    }
}

CMyGUI::CMyGUI(
    eC_Value x, eC_Value y,
    eC_Value width, eC_Value height,
    ObjectHandle_t eID) :
    CStreamRuntimeGUI(x, y, width, height, eID),
    m_pkDialogAnimation(NULL),
    m_pkDialogCarousel(NULL),
    m_pkDialogGauge(NULL),
    m_pkDialogKeyboard(NULL),
    m_pkDialogSettings(NULL),
    m_pkDialogSlider(NULL),
    m_pkDialogText(NULL),
    m_pkBoxObserver(NULL),
    m_eTransitionType(CGUITransitionCmd::PUSH_FROM_TOP),
    m_eBackgroundImage(IMG_GUILIANI_DEMO_BACKGROUND_NATIVE),
    m_eGaugeEasingType(CGUIEasing::EASE_IN_OUT_SINE),
    m_eSelectedMenu(MENU_ANIMATION),
    m_uiBackgroundColor(DemoSettings::ms_cuiLightModeColor),
    m_eImageMode(DemoSettings::IM_LIGHT_MODE),
    m_kExportStreamingMode(""),
    m_pkGlassPane(NULL)
{
    // add callback for polling RTC
    GETTIMER.AddAnimationCallback(100, this);

    /* register callback function */
    CGUIDataPool::Register(DATAPOOL_LED,&pvLedButtonCallback);

    // Add application specific initialisation here if necessary
    GETEVENTHDL.SetDragThreshold(20);

    // init "demo"-mode
    CGUIPlaybackInput::CreateInstance(false, "other_resources/EventRecord.dat");

    CheckExportStreamingMode();

    ShowDialog(DLG_MAIN);
    ChangeBackground(DLG_MAIN);

    m_pkGlassPane = new CGUIGeometryObject(this, 0, 0, this->GetWidth(), this->GetHeight(), 0xff000000, CGUIGeometryObject::PS_RECTANGLE, eC_FromInt(1), true);
    m_pkGlassPane->SetAlpha(127);
    m_pkGlassPane->SetInvisible(true);

    // init text for sub-menus
    MenuTexts[0] = ANIMATION_BUTTON_TXT;
    MenuTexts[1] = BUTTONS_BUTTON_TXT;
    MenuTexts[2] = SLIDER_BUTTON_TEXT;
    MenuTexts[3] = DUMMY_TEXT;
    MenuTexts[4] = GAUGE_N_WHEEL_BUTTON_TEXT;
    MenuTexts[5] = CAROUSEL_BUTTON_TXT;
    MenuTexts[6] = KEYBOARD_TXT;
    MenuTexts[7] = SCRATCHPAD_TXT;
    MenuTexts[8] = SETTINGS_BUTTON_TXT;

    eC_Value vPosX = eC_FromInt(0);
    eC_Value vPosY = eC_FromInt(0);

    if (GetWidth() == eC_FromInt(800))
    {
        vPosX = eC_FromInt(70);
        vPosY = eC_FromInt(5);
    }
    else if (GetWidth() == eC_FromInt(480))
    {
        vPosX = eC_FromInt(40);
        vPosY = eC_FromInt(0);
    }

    GETFPS.SetInvisible(true);
    GETFPS.SetPosition(CGUIFramerate::DEFAULT_ABS_FPS, vPosX, vPosY);
    GETFPS.SetBackColor(CGUIFramerate::DEFAULT_ABS_FPS, 0x00000000);
    GETFPS.SetTextColor(CGUIFramerate::DEFAULT_ABS_FPS, 0xffdd0000);
    GETFPS.SetFont(CGUIFramerate::DEFAULT_ABS_FPS, FNT_BEBASNEUE_REGULAR);

    GETFPS.SetPosition(CGUIFramerate::DEFAULT_FPS, vPosX, vPosY + eC_FromInt(10));
    GETFPS.SetBackColor(CGUIFramerate::DEFAULT_FPS, 0x00000000);
    GETFPS.SetTextColor(CGUIFramerate::DEFAULT_FPS, 0xffdd0000);
    GETFPS.SetFont(CGUIFramerate::DEFAULT_FPS, FNT_BEBASNEUE_REGULAR);

    /* open LED driver */
    int_t led_handle = (-1);
    uint16_t led = LED0;
    led_handle = open( DEVICE_INDENTIFIER "led", O_RDWR);
    /* LED OFF */
    control(led_handle, CTL_SET_LED_OFF, &led);
    /* close LED driver */
    close(led_handle);

}

CMyGUI::~CMyGUI()
{
    // Add application specific de-initialisation here if necessary
    Cleanup();
}

DATE last_date;

void CMyGUI::DoAnimate(const eC_Value &vTimes)
{
    /* polling real time clock */
    {
        char date_str[32];
        DATE date;
        /* open real time clock */
        int_t rtc_handle = open(DEVICE_INDENTIFIER "rtc", O_RDWR);
        if (control(rtc_handle, CTL_GET_DATE, &date) == 0)
        {
            if( date.Field.Second != last_date.Field.Second )
            {
                /* create text for time */
                sprintf(date_str,"%.2d:%.2d:%.2d\0",(int_t) date.Field.Hour, (int_t) date.Field.Minute, (int_t) date.Field.Second);
                /* get the object for AID_TEXTFIELD_2 */
                CGUITextField* pkTextField =  static_cast<CGUITextField*>(GETGUI.GetObjectByID(AID_TEXTFIELD_2));
                /* set the new label for AID_TEXTFIELD_2 */
                pkTextField->SetLabel(date_str);
            }
            last_date = date;
        }
        close(rtc_handle);
    }
}

void CMyGUI::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    if (NULL != pkUpdatedObject)
    {
        if (pkUpdatedObject->GetID() == MAIN_MENU_CAROUSEL)
            ChangeMenuName();
    }
}

void CMyGUI::ShowInfoText(const eC_String& kText)
{
    if (kText == "anim")
    {
        AnimateInfoTxt(INFO_ANIMATION_TXT);
    }
    else if (kText == "buttons")
    {
        AnimateInfoTxt(INFO_BUTTONS_TXT);
    }
    else if (kText == "caro")
    {
        AnimateInfoTxt(INFO_CAROUSEL_TXT);
    }
    else if (kText == "gauge")
    {
        AnimateInfoTxt(INFO_GAUGE_TXT);
    }
    else if (kText == "key")
    {
        AnimateInfoTxt(INFO_KEYBOARD_TXT);
    }
    else if (kText == "scratch")
    {
        AnimateInfoTxt(INFO_SCRATCHPAD_TXT);
    }
    else if (kText == "set")
    {
        AnimateInfoTxt(INFO_SETTINGS_TXT);
    }
    else if (kText == "slide")
    {
        AnimateInfoTxt(INFO_SLIDER_TXT);
    }
    else if (kText == "text")
    {
        AnimateInfoTxt(INFO_TEXT_TXT);
    }
}

void CMyGUI::SetTransition(const eC_String& kText)
{
    // this also sets the origin menu-item
    if (kText == "anim")
    {
        SetTransition(ANIMATION_BUTTON);
        m_eSelectedMenu = MENU_ANIMATION;
    }
    else if (kText == "btn")
    {
        SetTransition(BUTTONS_BUTTON);
        m_eSelectedMenu = MENU_BUTTONS;
    }
    else if (kText == "sld")
    {
        SetTransition(SLIDER_BUTTON);
        m_eSelectedMenu = MENU_SLIDER;
    }
    else if (kText == "txt")
    {
        SetTransition(TEXT_BUTTON);
        m_eSelectedMenu = MENU_TEXT;
    }
    else if (kText == "gauge")
    {
        SetTransition(GAUGE_BUTTON);
        m_eSelectedMenu = MENU_GAUGE;
    }
    else if (kText == "caro")
    {
        SetTransition(CAROUSEL_BUTTON);
        m_eSelectedMenu = MENU_CAROUSEL;
    }
    else if (kText == "key")
    {
        SetTransition(KEYBOARD_BUTTON);
        m_eSelectedMenu = MENU_KEYBOARD;
    }
    else if (kText == "scratch")
    {
        SetTransition(SCRATCHPAD_BUTTON);
        m_eSelectedMenu = MENU_SCRATCHPAD;
    }
    else if (kText == "settings")
    {
        SetTransition(SETTINGS_BUTTON);
        m_eSelectedMenu = MENU_SETTINGS;
    }
}

void CMyGUI::Cleanup()
{
    if (m_pkDialogAnimation != NULL)
    {
        delete m_pkDialogAnimation;
        m_pkDialogAnimation = NULL;
    }

    if (m_pkDialogCarousel != NULL)
    {
        delete m_pkDialogCarousel;
        m_pkDialogCarousel = NULL;
    }

    if (m_pkDialogGauge != NULL)
    {
        delete m_pkDialogGauge;
        m_pkDialogGauge = NULL;
    }

    if (m_pkDialogKeyboard != NULL)
    {
        delete m_pkDialogKeyboard;
        m_pkDialogKeyboard = NULL;
    }

    if (m_pkDialogSettings != NULL)
    {
        delete m_pkDialogSettings;
        m_pkDialogSettings = NULL;
    }

    if (m_pkDialogSlider != NULL)
    {
        delete m_pkDialogSlider;
        m_pkDialogSlider = NULL;
    }

    if (m_pkDialogText != NULL)
    {
        delete m_pkDialogText;
        m_pkDialogText = NULL;
    }

    if (m_pkBoxObserver != NULL)
    {
        delete m_pkBoxObserver;
        m_pkBoxObserver = NULL;
    }
}

void CMyGUI::ShowGlassPane()
{
    if (NULL != m_pkGlassPane)
    {
        m_pkGlassPane->SetInvisible(false);
        GETGUI.DrawOnTop(m_pkGlassPane);
    }
}

void CMyGUI::ShowDialog(const ObjectHandle_t& eDialogID)
{
    ShowGlassPane();
    ChangeBackground(eDialogID);

    if (eDialogID == DLG_MAIN)
    {
        CGUICarousel* pkMainMenu = static_cast<CGUICarousel*>(GETGUI.GetObjectByID(MAIN_MENU_CAROUSEL));
        if (NULL != pkMainMenu)
        {
            pkMainMenu->AddValueObserver(this);
        }
    }
}

void CMyGUI::CheckExportStreamingMode()
{
    eC_String kXmlExtension(".xml");
    eC_String kBinaryExtension(".bin");

    eC_String kImageFileName = GETRUNTIMECONFIG.GetImageFileName();
    if (kImageFileName.IsEmpty())
    {
        return;
    }

    // check if export streaming mode is xml or binary
    if (eC_String::notfound != kImageFileName.Find(kXmlExtension))
    {
        m_kExportStreamingMode = kXmlExtension;
    }
    else if (eC_String::notfound != kImageFileName.Find(kBinaryExtension))
    {
        m_kExportStreamingMode = kBinaryExtension;
    }
}

eC_Bool CMyGUI::CallApplicationAPI(const eC_String& kAPI, const eC_String& kParam)
{
    static eC_Bool bToggleLang = false;
    static eC_Bool bToggleSkin = false;
    static eC_Bool bToggleNeon = false;
    static eC_Bool bToggleFPS = false;
    static eC_Bool bToggleBilinear = false;

    if (kAPI == "EnableRichText")
    {
        /*Toggle between rich-text and normal text on the Text-Dialog*/
        EnableRichText();
    }
    else if (kAPI == "ChangeLang")
    {
        /*Change the language from the settings dialog*/
        ChangeLang(bToggleLang);
        bToggleLang = !bToggleLang;
    }
    else if (kAPI == "ChangeNeon")
    {
        /*Enable / Disable NEON optimization*/
        ChangeNeon(bToggleNeon);
        bToggleNeon = !bToggleNeon;
    }
    else if (kAPI == "ChangeBlitBilinear")
    {
        /*Enable / Disable Bilinear filtering*/
        ChangeBlitBilinear(bToggleBilinear);
        bToggleBilinear = !bToggleBilinear;
    }
    else if (kAPI == "ChangeShowFPS")
    {
        /*Enable / Disable NEON optimization*/
        ShowFPS(bToggleFPS);
        bToggleFPS = !bToggleFPS;
    }
    else if (kAPI == "ToggleFlow")
    {
        /*calling the HandleCallAPI from the DemoCarousel-Class to toggle between flow- and carousel-mode*/
        if (NULL != m_pkDialogCarousel)
        {
            m_pkDialogCarousel->HandleCallAPI(kAPI, kParam);
        }
    }
    else if (kAPI == "ToggleSkin")
    {
        /*Toggle between the dark and light skin on the demo from the settings screen*/
        ChangeSkin(bToggleSkin);
        bToggleSkin = !bToggleSkin;

        // Since the current active dialog is "Settings", the foreground color of its Geometry object is
        // changed immediately. The foreground color for other dialogs will be changed, when transition
        // occurs and they are loaded.
        ChangeBackground(DLG_SETTINGS);
    }
    else if (kAPI == "StartAnimation")
    {
        /*Calling the HandleCallAPI from the DemoAnimation-Class to set the index of the animation that needs to be called*/
        if (NULL != m_pkDialogAnimation)
        {
            m_pkDialogAnimation->HandleCallAPI(kAPI, kParam);
        }
    }
    else if (kAPI == "Animation")
    {
        /*API Call to create an instance of the DemoAnimation. It is needed, because the object requires Controlls that are not created*/
        if (NULL == m_pkDialogAnimation)
        {
            m_pkDialogAnimation = new DemoAnimation();
        }
        m_pkDialogAnimation->Init();
        ShowDialog(DLG_ANIMATION);
    }
    else if (kAPI == "Carousel")
    {
        /*API Call to create an instance of the DemoCarousel. It is needed, because the object requires Controlls that are not created*/
        if (NULL == m_pkDialogCarousel)
        {
            m_pkDialogCarousel = new DemoCarousel();
        }
        m_pkDialogCarousel->Init();
        ShowDialog(DLG_CAROUSEL);
    }
    else if (kAPI == "Slider")
    {
        /*API Call to create an instance of the DemoSlider. It is needed, because the object requires Controlls that are not created*/
        if (NULL == m_pkDialogSlider)
        {
            m_pkDialogSlider = new DemoSlider();
        }
        m_pkDialogSlider->Init();
        ShowDialog(DLG_SLIDER);
    }
    else if (kAPI == "Text")
    {
        if (NULL == m_pkDialogText)
        {
            m_pkDialogText = new DemoText();
        }
        m_pkDialogText->Init();
        ShowDialog(DLG_TEXT);
    }
    else if (kAPI == "Gauge")
    {
        /*API Call to create an instance of the DemoGauge. Depending on the value of the combobox in the settings a different instange will be created.
        It is needed, because the object requires Controlls that are not created*/
        if (NULL == m_pkDialogGauge)
        {
            m_pkDialogGauge = new DemoGauge(m_eGaugeEasingType);
        }
        m_pkDialogGauge->Init();
        m_pkDialogGauge->SetEasing(m_eGaugeEasingType);
        ShowDialog(DLG_GAUGEANDWHEEL);
    }
    else if (kAPI == "Settings")
    {
        /*API Call to create an instance of the DemoSettings. It is needed, because the object requires Controlls that are not created*/
        if (NULL == m_pkDialogSettings)
        {
            m_pkDialogSettings = new DemoSettings();
        }

        ShowDialog(DLG_SETTINGS);
        m_pkDialogSettings->SetEasing(m_eGaugeEasingType);
        m_pkDialogSettings->SetBackground(m_eBackgroundImage);
        m_pkDialogSettings->SetTransition(m_eTransitionType);
        m_pkDialogSettings->SetImageMode(m_eImageMode);

        m_pkDialogSettings->Init();
    }
    else if (kAPI == "Buttons")
    {
        ShowDialog(DLG_BUTTONS);
    }
    else if (kAPI == "Keyboard")
    {
        if (NULL == m_pkDialogKeyboard)
        {
            m_pkDialogKeyboard = new DemoKeyboard();
        }

        ShowDialog(DLG_KEYBOARD);
        m_pkDialogKeyboard->Init();
    }
    else if (kAPI == "Scratchpad")
    {
        ShowDialog(DLG_SCRATCHPAD);
    }
    else if (kAPI == "MessageBox")
    {
        GETPROPHDL.SetGlobalColorProperty(GUI_PROP_DEFAULT_COLOR, 0xFF989898);
        GETPROPHDL.SetGlobalColorProperty(GUI_PROP_DEFAULT_HIGHLIGHT_COLOR, 0xFFA5A5A5);
        GETPROPHDL.SetGlobalColorProperty(GUI_PROP_DEFAULT_PRESSED_COLOR, 0xFF656565);
        CGUICompositeObject* pkParent = dynamic_cast<CGUICompositeObject*>(GETGUI.GetObjectByID(DLG_BUTTONS));

        if (NULL != pkParent)
        {
            eC_Value vMessageBoxWidth = eC_FromInt(300);
            eC_Value vMessageBoxHeight = eC_FromInt(250);
            if (vMessageBoxWidth > GetWidth())
                vMessageBoxWidth = GetWidth() - eC_FromInt(20);
            if (vMessageBoxHeight > GetHeight())
                vMessageBoxHeight = GetHeight() - eC_FromInt(20);
            eC_Value vMessageBoxXPos = eC_Div((pkParent->GetWidth() - vMessageBoxWidth), 2);
            eC_Value vMessageBoxYPos = eC_Div((pkParent->GetHeight() - vMessageBoxHeight), 2);
            CGUIBaseMessageBox* pkMessageBox = new CGUIBaseMessageBox(pkParent, vMessageBoxXPos, vMessageBoxYPos, vMessageBoxWidth, vMessageBoxHeight, "", true);
            if (NULL != pkMessageBox)
            {
                pkMessageBox->SetBackgroundColor(0xFFEFEFEF);
                pkMessageBox->SetTextFont(HEADER_FNT);
                pkMessageBox->SetText("This is a simple message!");
            }
        }
    }
    else if (kAPI == "Clean")
    {
        // remove box-observer when dialog is removed
        if (NULL != m_pkBoxObserver)
        {
            delete m_pkBoxObserver;
            m_pkBoxObserver = NULL;
        }

        // save settings from dialog and navigate to correct menu-item in main-menu
        if (MENU_UNKNOWN != m_eSelectedMenu)
        {
            CGUIObject* pkObjectToFocus = NULL;
            if (m_eSelectedMenu == MENU_ANIMATION)
            {
                if (NULL != m_pkDialogAnimation)
                {
                    delete m_pkDialogAnimation;
                    m_pkDialogAnimation = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(ANIMATION_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_BUTTONS)
            {
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(BUTTONS_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_SLIDER)
            {
                if (m_pkDialogSlider)
                {
                    delete m_pkDialogSlider;
                    m_pkDialogSlider = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(SLIDER_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_TEXT)
            {
                if (NULL != m_pkDialogText)
                {
                    delete m_pkDialogText;
                    m_pkDialogText = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(TEXT_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_GAUGE)
            {
                if (NULL != m_pkDialogGauge)
                {
                    delete m_pkDialogGauge;
                    m_pkDialogGauge = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(GAUGE_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_CAROUSEL)
            {
                if (NULL != m_pkDialogCarousel)
                {
                    delete m_pkDialogCarousel;
                    m_pkDialogCarousel = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(CAROUSEL_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_KEYBOARD)
            {
                if (NULL != m_pkDialogKeyboard)
                {
                    delete m_pkDialogKeyboard;
                    m_pkDialogKeyboard = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(KEYBOARD_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_SCRATCHPAD)
            {
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(SCRATCHPAD_BUTTON));
            }
            else if (m_eSelectedMenu == MENU_SETTINGS)
            {
                if (NULL != m_pkDialogSettings)
                {
                    // save settings
                    m_pkDialogSettings->HandleCallAPI(kAPI, kParam);

                    m_eTransitionType = m_pkDialogSettings->GetTransition();
                    m_eBackgroundImage = m_pkDialogSettings->GetBackgroundImage();
                    m_eGaugeEasingType = m_pkDialogSettings->GetEasing();
                    m_uiBackgroundColor = m_pkDialogSettings->GetBackgroundColor();
                    m_eImageMode = m_pkDialogSettings->GetImageMode();

                    delete m_pkDialogSettings;
                    m_pkDialogSettings = NULL;
                }
                pkObjectToFocus = static_cast<CGUIButton*>(GETGUI.GetObjectByID(SETTINGS_BUTTON));
            }

            CGUICarousel* pkMainMenu = static_cast<CGUICarousel*>(GETGUI.GetObjectByID(MAIN_MENU_CAROUSEL));
            if ((NULL != pkMainMenu) && (NULL != pkObjectToFocus))
            {
                pkMainMenu->JumpToObject(pkObjectToFocus);
                ChangeMenuName();
                ShowDialog(DLG_MAIN);
            }
        }
    }
    else if (kAPI == "SetTransition")
    {
        SetTransition(kParam);
    }
    else if (kAPI == "InfoText")
    {
        ShowInfoText(kParam);
    }
    else if (kAPI == "StartPlayback")
    {
        static_cast<CGUIPlaybackInput&>(GETINPUTMEDIA).StartPlayback();
    }
    else
    {
        /*HandleCallAPI from the gauge dialog. This will change the value of the wheel depending on the button that is pressed*/
        if (NULL != m_pkDialogGauge)
        {
            m_pkDialogGauge->HandleCallAPI(kAPI, kParam);
        }
        return false;
    }
    return true;
}

void CMyGUI::EnableRichText()
{
    CGUIBaseTextField* pkNormalText = static_cast<CGUIBaseTextField*>(GETGUI.GetObjectByID(NORMAL_TEXT_FIELD));
    CGUIBaseTextField* pkRichText = static_cast<CGUIBaseTextField*>(GETGUI.GetObjectByID(RICH_TEXT_FIELD));
    if (pkNormalText != NULL && pkRichText != NULL)
    {
        pkNormalText->SetInvisible(!(pkNormalText->IsInvisible()));
        pkRichText->SetInvisible(!(pkRichText->IsInvisible()));
    }
}

void CMyGUI::ChangeLang(const eC_Bool& bToggle)
{
    if (true == bToggle)
    {
        GETLOCALEHDL.LoadLocalisationFile("English.lng");
    }
    else
    {
        GETLOCALEHDL.LoadLocalisationFile("German.lng");
    }
    GETGUI.InvalidateArea();
}

void CMyGUI::ChangeNeon(const eC_Bool& bUseNeon)
{
#ifdef GFX_USE_EGML
    CGfxWrapeGML* pkGfxWrap = dynamic_cast<CGfxWrapeGML*>(&GETGFX);
    if (NULL != pkGfxWrap)
        pkGfxWrap->SetNEONOptimization(bUseNeon);
#endif
    GETGUI.InvalidateArea();
}

void CMyGUI::ShowFPS(const eC_Bool& bToggle)
{
    GETFPS.SetInvisible(bToggle);
}

void CMyGUI::ChangeBlitBilinear(const eC_Bool& bToggle)
{
#ifdef GFX_USE_EGML
    CGfxWrapeGML* pkGfxWrap = dynamic_cast<CGfxWrapeGML*>(&GETGFX);
    if (NULL != pkGfxWrap)
        pkGfxWrap->SetBilinearBlit(bToggle);
#endif
}

void CMyGUI::ChangeSkin(const eC_Bool& bToggle)
{
    if (m_kExportStreamingMode.IsEmpty())
    {
        return;
    }

    if (true == bToggle)
    {
        GETRESMANAGER.RegisterImageResourcesFromFile(eC_String("images_Light") + m_kExportStreamingMode);
        m_eImageMode = DemoSettings::IM_LIGHT_MODE;
    }
    else
    {
        GETRESMANAGER.RegisterImageResourcesFromFile(eC_String("images_Dark") + m_kExportStreamingMode);
        m_eImageMode = DemoSettings::IM_DARK_MODE;
    }

    if (NULL != m_pkDialogSettings)
    {
        // Set the image mode.
        m_pkDialogSettings->SetImageMode(m_eImageMode);

        m_eBackgroundImage = m_pkDialogSettings->GetBackgroundImage();
        m_uiBackgroundColor = m_pkDialogSettings->GetBackgroundColor();
    }

    GETGUI.InvalidateArea();
}

void CMyGUI::ChangeMenuName()
{
    CGUICarousel* pkMainMenu = static_cast<CGUICarousel*>(GETGUI.GetObjectByID(MAIN_MENU_CAROUSEL));
    CGUITextField* pkMenuName = static_cast<CGUITextField*>(GETGUI.GetObjectByID(MENU_NAME));

    /* Get the Button that is in focus and get its textfield. Then set the Textfield for the menu-name to that of the button */
    if (pkMainMenu != NULL && pkMenuName != NULL)
    {
        eC_Int iIndex = pkMainMenu->GetSelectedIndex();

        if ((iIndex >= 0) && (iIndex < 9))
        {
            TextResource_t eTextID = MenuTexts[iIndex];

            // only 30 texts in evaluation
            if (eTextID == DUMMY_TEXT)
            {
                pkMenuName->SetLabel("Text");
            }
            else
            {
                pkMenuName->SetLabel(eTextID);
            }
        }
    }
}

void CMyGUI::ChangeBackground(const ObjectHandle_t& eObjectID)
{
    if (NO_HANDLE != eObjectID)
    {
        // Get the dialog of eObjectID, which is a composite object
        CGUICompositeObject *pkDialogCompObj =  dynamic_cast<CGUICompositeObject*>(GETGUI.GetObjectByID(eObjectID));
        if (NULL != pkDialogCompObj)
        {
            CGUIGeometryObject* pkBackgroundRect = dynamic_cast<CGUIGeometryObject*>(pkDialogCompObj->GetObjectByID(OBJ_BACKGROUND_GEO));
            CGUIImage* pkImage = dynamic_cast<CGUIImage*>(pkDialogCompObj->GetObjectByID(OBJ_BACKGROUND_IMAGE));

            // depending on background-image
            if (
                (m_eBackgroundImage == IMG_GUILIANI_DEMO_BACKGROUND_NATIVE) ||
                (m_eBackgroundImage == IMG_GUILIANI_DEMO_BACKGROUND_RLE) ||
                (m_eBackgroundImage == IMG_GUILIANI_DEMO_BACKGROUND_RAW)
                )
            {
                if (NULL != pkImage)
                {
                    pkImage->SetImage(m_eBackgroundImage, false);
                    pkImage->SetStretchBlit(false);
                    pkImage->SetInvisible(false);
                    pkImage->InvalidateArea();
                }

                if (NULL != pkBackgroundRect)
                    pkBackgroundRect->SetInvisible(true);
            }
            else
            {
                if (NULL != pkImage)
                    pkImage->SetInvisible(true);

                if (NULL != pkBackgroundRect)
                {
                    pkBackgroundRect->SetColor(m_uiBackgroundColor);
                    pkBackgroundRect->SetInvisible(false);
                    pkBackgroundRect->InvalidateArea();
                }
            }
        }
    }
}

void CMyGUI::AnimateInfoTxt(const TextResource_t& eID)
{
    CGUIBaseTextField* pkTextField = static_cast<CGUITextField*>(GETGUI.GetObjectByID(INFO_TEXT));
    CGUIGeometryObject* pkBackground = static_cast<CGUIGeometryObject*>(GETGUI.GetObjectByID(TEXT_BACKDROP));
    CGUICompositeObject* pkContainer = static_cast<CGUICompositeObject*>(GETGUI.GetObjectByID(INFO_CONTAINER));

    if (m_pkBoxObserver != NULL)
    {
        delete m_pkBoxObserver;
        m_pkBoxObserver = NULL;
    }

    if (NULL != pkTextField)
    {
        eC_Value vTargetXPos = pkTextField->GetRelXPos();
        eC_Value vTargetYPos = pkTextField->GetRelYPos();
        eC_Value vTargetWidth = pkTextField->GetWidth();
        eC_Value vTargetHeight = pkTextField->GetHeight();

        if (pkTextField->IsInvisible())
        {
            if (NULL != pkBackground)
                pkBackground->SetInvisible(false);

            pkTextField->SetInvisible(false);
            pkTextField->SetAlpha(0);
            CGUIAnimationSize* pkSizeAnimation = new CGUIAnimationSize(pkBackground, CGUIEasing::EASE_IN_CUBIC, CGUIEasing::EASE_OUT_CUBIC, 0, 0, vTargetWidth, vTargetHeight, false, 250, 20);
            CGUIAnimationMove* pkMoveAnimation = new CGUIAnimationMove(pkBackground, CGUIEasing::EASE_IN_CUBIC, CGUIEasing::EASE_OUT_CUBIC, vTargetWidth, vTargetHeight, vTargetXPos, vTargetYPos, 250, false, 20);

            pkTextField->SetLabel(eID);
            if (m_pkBoxObserver == NULL)
            {
                m_pkBoxObserver = new BoxObserver(true);
            }
            pkSizeAnimation->SetAnimationObserver(m_pkBoxObserver);

            pkMoveAnimation->SetDeletedAfterFinish(true);
            pkMoveAnimation->StartAnimation();

            pkSizeAnimation->SetDeletedAfterFinish(true);
            pkSizeAnimation->StartAnimation();
        }
        else
        {
            pkTextField->SetInvisible(true);
            pkTextField->SetAlpha(255);
            CGUIAnimationSize* pkSizeAnimation = new CGUIAnimationSize(pkBackground, CGUIEasing::EASE_IN_CUBIC, CGUIEasing::EASE_OUT_CUBIC, vTargetWidth, vTargetHeight, 0, 0, false, 250, 20);
            CGUIAnimationMove* pkMoveAnimation = new CGUIAnimationMove(pkBackground, CGUIEasing::EASE_IN_CUBIC, CGUIEasing::EASE_OUT_CUBIC, vTargetXPos, vTargetYPos, vTargetWidth, vTargetHeight, 250, false, 20);

            pkTextField->SetLabel(eID);
            if (m_pkBoxObserver == NULL)
            {
                m_pkBoxObserver = new BoxObserver(false);
            }
            pkSizeAnimation->SetAnimationObserver(m_pkBoxObserver);

            pkMoveAnimation->SetDeletedAfterFinish(true);
            pkMoveAnimation->StartAnimation();

            pkSizeAnimation->SetDeletedAfterFinish(true);
            pkSizeAnimation->StartAnimation();
        }
    }
}

// set dialog-transition within command to appropriate type
void CMyGUI::SetTransition(const ObjectHandle_t& button)
{
    CGUIButton* pkBackButton = dynamic_cast<CGUIButton*>(GETGUI.GetObjectByID(button));
    if (NULL != pkBackButton)
    {
        CGUICommandPtr CmdPtr = pkBackButton->GetCommand();
        if (CmdPtr)
        {
            CGUICommand* pkCommand = CmdPtr->GetAdditionalCmd(0).RawPtr();
            CGUITransitionCmd* pkTransitionCmd = dynamic_cast<CGUITransitionCmd*>(pkCommand);
            if (NULL != pkTransitionCmd)
            {
                pkTransitionCmd->SetTransitionType(m_eTransitionType);
            }
        }
    }
}

void CMyGUI::NotifyOfDestruction(const CGUIObject* pDestructedObject)
{
    if (NULL != m_pkGlassPane)
    {
        m_pkGlassPane->SetInvisible(true);
    }
}

void CMyGUI::pvLedButtonCallback(CDataPoolEntry& data)
{
    CGUIValue value;
    uint16_t led = LED0;
    int_t led_handle = (-1);

    /* get the value of datapool for LED checkbox */
    CGUIDataPool::Get(DATAPOOL_LED,value);

    /* open LED driver */
    led_handle = open( DEVICE_INDENTIFIER "led", O_RDWR);

    /* check the value of datapool for LED checkbox */
    if (value.ToInt() == 0)
    {
        /* LED OFF */
        control(led_handle, CTL_SET_LED_OFF, &led);
    }
    else
    {
        /* LED ON */
        control(led_handle, CTL_SET_LED_ON, &led);
    }
    close(led_handle);
}
