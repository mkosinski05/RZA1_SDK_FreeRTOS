#if !defined _DEMOSETTINGS_H_
#define _DEMOSETTINGS_H_

#include "DemoBase.h"

#include "GUIEasing.h"
#include "GUIComboBox.h"
#include "GUIButton.h"
#include "GUICommand.h"
#include "GUIObserver.h"
#include "GUITransitionCmd.h"

#define MAX_EASINGS     4
#define MAX_BACKGROUNDS 4
#define MAX_TRANSITIONS 5

/*Class for the Settings-Dialog, needed for the combobox*/
class DemoSettings : public DemoBase, public CGUIObserver
{
public:
    ///Image Mode
    enum ImageMode_t
    {
        IM_DARK_MODE,
        IM_LIGHT_MODE
    };

    static const eC_UInt ms_cuiLightModeColor; // Background color for light mode
    static const eC_UInt ms_cuiDarkModeColor; // Background color for dark mode

    DemoSettings()
    {
    }

    virtual ~DemoSettings();

    virtual void Init();

    virtual void DeInit();

    virtual void HandleCallAPI(const eC_String& kAPI, const eC_String kParam);

    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX = 0, const eC_UInt uiY = 0);

    void ChangeBackground(const ObjectHandle_t& eObjectID);

    void SetImageMode(const ImageMode_t& eImageMode);

    ImageMode_t GetImageMode()
    {
        return m_eImageMode;
    }

    CGUIEasing::EasingType_t GetEasing() const
    {
        return m_eGaugeEasing;
    }

    ImageResource_t GetBackgroundImage() const
    {
        return m_eBackgroundImage;
    }

    CGUITransitionCmd::TransitionType_t GetTransition() const
    {
        return m_eTransitionType;
    }

    eC_UInt GetBackgroundColor()
    {
        return m_uiBackgroundColor;
    }

    void SetEasing(CGUIEasing::EasingType_t eEasing)
    {
        m_eGaugeEasing = eEasing;
    }

    void SetBackground(ImageResource_t eID)
    {
        m_eBackgroundImage = eID;
    }

    void SetTransition(CGUITransitionCmd::TransitionType_t eTransition)
    {
        m_eTransitionType = eTransition;
    }

    eC_Int GetEasingIndex() const
    {
        for (eC_Int idx = 0; idx < MAX_EASINGS; ++idx)
        {
            if (m_eGaugeEasing == m_kEasings[idx].Value)
            {
                return idx;
            }
        }
        return 0;
    }

    eC_Int GetBackgroundIndex() const
    {
        for (eC_Int idx = 0; idx < MAX_BACKGROUNDS; ++idx)
        {
            if (m_eBackgroundImage == m_kBackground[idx].ID)
            {
                return idx;
            }
        }
        return 0;
    }

    eC_Int GetTransitionIndex() const
    {
        for (eC_Int idx = 0; idx < MAX_TRANSITIONS; ++idx)
        {
            if (m_eTransitionType == m_kTransitions[idx].Value)
            {
                return idx;
            }
        }
        return 0;
    }

    eC_Bool IsCheckBoxSelected(const ObjectHandle_t& eObjectID) const;
    void SelectCheckBox(const ObjectHandle_t& eObjectID, const eC_Bool& bSelected, const eC_Bool& bVisible = true) const;

private:
    struct tEasings
    {
        eC_String Name;
        CGUIEasing::EasingType_t Value;
    };

    struct tBackground
    {
        eC_String Name;
        ImageResource_t ID;
    };

    struct tTransitions
    {
        eC_String Name;
        CGUITransitionCmd::TransitionType_t Value;
    };

private:
    tEasings m_kEasings[MAX_EASINGS];
    tBackground m_kBackground[MAX_BACKGROUNDS];
    tTransitions m_kTransitions[MAX_TRANSITIONS];
    CGUIComboBox* m_pkCombo1;
    CGUIComboBox* m_pkCombo2;
    CGUIComboBox* m_pkCombo3;
    CGUIEasing::EasingType_t m_eGaugeEasing;
    ImageResource_t m_eBackgroundImage;
    CGUITransitionCmd::TransitionType_t m_eTransitionType;
    eC_UInt m_uiBackgroundColor;
    ImageMode_t m_eImageMode;
};

#endif
