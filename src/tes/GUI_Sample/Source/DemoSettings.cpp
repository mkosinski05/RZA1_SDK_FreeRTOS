#include "DemoSettings.h"

#include "GUI.h"
#include "GUIGeometryObject.h"
#include "GUIImage.h"
#include "GUIListItem.h"
#include "GUIScrollBar.h"
#include "GUICheckBox.h"
#include "GUIObserver.h"

/*Always include last*/
#include "GUIMemLeakWatcher.h"

const eC_UInt DemoSettings::ms_cuiLightModeColor(0XFFF5F5F4);
const eC_UInt DemoSettings::ms_cuiDarkModeColor(0xFF797979);

void DemoSettings::Init()
{
    m_kEasings[0].Name = "Ease-Out Expo";
    m_kEasings[0].Value = CGUIEasing::EASE_OUT_EXPO;
    m_kEasings[1].Name = "Ease-In Quad";
    m_kEasings[1].Value = CGUIEasing::EASE_IN_QUAD;
    m_kEasings[2].Name = "Ease-In-Out Sine";
    m_kEasings[2].Value = CGUIEasing::EASE_IN_OUT_SINE;
    m_kEasings[3].Name = "Ease-Out Circ";
    m_kEasings[3].Value = CGUIEasing::EASE_OUT_CIRC;

    m_pkCombo1 = dynamic_cast<CGUIComboBox*>(GETGUI.GetObjectByID(EASING_TYPES));

    if (NULL != m_pkCombo1)
    {
        for (eC_Int idx = 0; idx < MAX_EASINGS; ++idx)
        {
            CGUIListItem *pkListItem = new CGUIListItem(
                NULL, eC_FromInt(8), eC_FromInt(0),
                m_pkCombo1->GetWidth() - eC_FromInt(16) - m_pkCombo1->GetListBox()->GetVerticalScrollbar()->GetWidth(),
                eC_FromInt(32), m_kEasings[idx].Name);

            pkListItem->GetLabel()->SetAligned(CGUIText::V_CENTERED);
            pkListItem->GetLabel()->SetFont(FNT_BEBASNEUE_REGULAR);
            pkListItem->GetLabel()->SetRelXPos(eC_FromInt(5));
            m_pkCombo1->AddItem(pkListItem);
        }

        /*Set the header combobox*/
        m_pkCombo1->SetSelection(GetEasingIndex());
        m_pkCombo1->SetHeaderEditable(false);
    }

    m_kBackground[0].Name = "GeometryObject";
    m_kBackground[0].ID = IMG_BACKGROUNDIMAGE;
    m_kBackground[1].Name = "Image (native)";
    m_kBackground[1].ID = IMG_GUILIANI_DEMO_BACKGROUND_NATIVE;
    m_kBackground[2].Name = "Image (RLE)";
    m_kBackground[2].ID = IMG_GUILIANI_DEMO_BACKGROUND_RLE;
    m_kBackground[3].Name = "Image (RAW)";
    m_kBackground[3].ID = IMG_GUILIANI_DEMO_BACKGROUND_RAW;

    //Another combobox with 4 values for the different backgrounds
    m_pkCombo2 = dynamic_cast<CGUIComboBox*>(GETGUI.GetObjectByID(BG_TYPES));

    if (NULL != m_pkCombo2)
    {
        for (eC_Int idx = 0; idx < MAX_BACKGROUNDS; ++idx)
        {
            CGUIListItem *pkListItem = new CGUIListItem(
                NULL, eC_FromInt(8), eC_FromInt(0),
                m_pkCombo2->GetWidth() - eC_FromInt(16) - m_pkCombo2->GetListBox()->GetVerticalScrollbar()->GetWidth(),
                eC_FromInt(32), m_kBackground[idx].Name);

            pkListItem->GetLabel()->SetAligned(CGUIText::V_CENTERED);
            pkListItem->GetLabel()->SetFont(FNT_BEBASNEUE_REGULAR);
            pkListItem->GetLabel()->SetRelXPos(eC_FromInt(5));
            m_pkCombo2->AddItem(pkListItem);
        }

        /*Set the header combobox*/
        m_pkCombo2->SetSelection(GetBackgroundIndex());
        m_pkCombo2->SetHeaderEditable(false);
        m_pkCombo2->AddSelectionObserver(this);
    }

    m_kTransitions[0].Name = "Push From Left";
    m_kTransitions[0].Value = CGUITransitionCmd::PUSH_FROM_LEFT;
    m_kTransitions[1].Name = "Push From Right";
    m_kTransitions[1].Value = CGUITransitionCmd::PUSH_FROM_RIGHT;
    m_kTransitions[2].Name = "Push From Top";
    m_kTransitions[2].Value = CGUITransitionCmd::PUSH_FROM_TOP;
    m_kTransitions[3].Name = "Push From Bottom";
    m_kTransitions[3].Value = CGUITransitionCmd::PUSH_FROM_BOTTOM;
    m_kTransitions[4].Name = "Dissolve";
    m_kTransitions[4].Value = CGUITransitionCmd::DISSOLVE;

    //Another combobox with 5 values for the different transition types, pass the value to the main class
    m_pkCombo3 = dynamic_cast<CGUIComboBox*>(GETGUI.GetObjectByID(TRANSITION_TYPE));

    if (NULL != m_pkCombo3)
    {
        for (eC_Int idx = 0; idx < MAX_TRANSITIONS; ++idx)
        {
            CGUIListItem *pkListItem = new CGUIListItem(
                NULL, eC_FromInt(8), eC_FromInt(0),
                m_pkCombo3->GetWidth() - eC_FromInt(16) - m_pkCombo3->GetListBox()->GetVerticalScrollbar()->GetWidth(),
                eC_FromInt(32), m_kTransitions[idx].Name);

            pkListItem->GetLabel()->SetAligned(CGUIText::V_CENTERED);
            pkListItem->GetLabel()->SetFont(FNT_BEBASNEUE_REGULAR);
            pkListItem->GetLabel()->SetRelXPos(eC_FromInt(5));
            m_pkCombo3->AddItem(pkListItem);
        }

        /*Set the header combobox*/
        m_pkCombo3->SetSelection(GetTransitionIndex());
        m_pkCombo3->SetHeaderEditable(false);
    }
}

DemoSettings::~DemoSettings()
{
    DeInit();
}

void DemoSettings::DeInit()
{
}

void DemoSettings::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX, const eC_UInt uiY)
{
    // selection changed
    if (NULL != m_pkCombo2)
    {
        m_eBackgroundImage = m_kBackground[m_pkCombo2->GetSelection()].ID;
        ChangeBackground(DLG_SETTINGS);
    }
}

/*Handling for leaving the settings dialog, gets the string and stores it*/
void DemoSettings::HandleCallAPI(const eC_String& kAPI, const eC_String kParam)
{
    if (kAPI == "Clean")
    {
        if (NULL != m_pkCombo1)
        {
            m_eGaugeEasing = m_kEasings[m_pkCombo1->GetSelection()].Value;
        }

        if (NULL != m_pkCombo2)
        {
            m_eBackgroundImage = m_kBackground[m_pkCombo2->GetSelection()].ID;
        }

        if (NULL != m_pkCombo3)
        {
            //Grab the selected value of the transition type
            m_eTransitionType = m_kTransitions[m_pkCombo3->GetSelection()].Value;
        }
    }
}

void DemoSettings::SetImageMode(const ImageMode_t& eImageMode)
{
    // Get the object of the button which sets light or dark mode
    CGUIButton* pkFocussedButton = dynamic_cast<CGUIButton*>(GETGUI.GetObjectByID(BTN_CHANGE_SKIN));

    if (NULL != pkFocussedButton)
    {
        m_eImageMode = eImageMode;

        switch (eImageMode)
        {
        case IM_LIGHT_MODE:
            m_uiBackgroundColor = ms_cuiLightModeColor;

            // Change the label of the button that selects Dark/Light mode images
            pkFocussedButton->SetLabel(DARK_IMAGE_SET_TXT);
            break;

        case IM_DARK_MODE:
            m_uiBackgroundColor = ms_cuiDarkModeColor;

            // Change the label of the button that selects Dark/Light mode images
            pkFocussedButton->SetLabel(LIGHT_IMAGE_SET_TXT);
            break;

        default:
            break;
        }
    }
}

void DemoSettings::ChangeBackground(const ObjectHandle_t& eObjectID)
{
    if (NO_HANDLE != eObjectID)
    {
        // Get the dialog of eObjectID, which is a composite object
        CGUICompositeObject *pkDialogCompObj = dynamic_cast<CGUICompositeObject*>(GETGUI.GetObjectByID(eObjectID));
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

eC_Bool DemoSettings::IsCheckBoxSelected(const ObjectHandle_t& eObjectID) const
{
    eC_Bool bRet = false;
    CGUICheckBox* pkCheckBox = dynamic_cast<CGUICheckBox*>(GETGUI.GetObjectByID(eObjectID));

    if (NULL != pkCheckBox)
    {
        bRet = pkCheckBox->IsSelected();
    }

    return bRet;
}

void DemoSettings::SelectCheckBox(const ObjectHandle_t& eObjectID, const eC_Bool& bSelected, const eC_Bool& bVisible) const
{
    CGUICheckBox* pkCheckBox = dynamic_cast<CGUICheckBox*>(GETGUI.GetObjectByID(eObjectID));

    if (NULL != pkCheckBox)
    {
       pkCheckBox->SetSelected(bSelected);
       pkCheckBox->SetInvisible(!bVisible);
    }
}
