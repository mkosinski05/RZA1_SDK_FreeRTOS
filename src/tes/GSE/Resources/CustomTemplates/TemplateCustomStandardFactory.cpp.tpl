#ifdef GUILIANI_STREAM_GUI

#include "CustomStandardFactory.h"

#include "GUI.h"
#include "GUITrace.h"
#include "GUIMultiCmdBehaviour.h"
#include "GUISingleCmdBehaviour.h"
#include "GUIHotkeysBehaviour.h"
#include "GUICompositeBehaviour.h"
#include "GUIObjectStateBehaviour.h"
#include "GUIStreamingException.h"
#include "GUIStreamReader.h"
#include "GUIResourceFileHandler.h"
#include "GUILayouter.h"
#include "GUILayouterAnchor.h"
#include "GUILayouterReposition.h"
#include "GUILayouterGrid.h"
#include "GUILayouterList.h"
#include "GUILayouterAlignToParent.h"
#include "GUILayouterPercentage.h"
#include "GUIBehaviourDecorator.h"
#include "GUIAnimationMove.h"
#include "GUIAnimationSize.h"
#include "GUIAnimationStdGUIObject.h"
#include "GUIAnimationMoveInOut.h"
#include "GUIAnimationBlinking.h"
#include "GUICommand.h"
#include "GUIBaseButton.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUIScrollingText.h"
#include "GUIEditableText.h"
#include "GUIImage.h"
#include "GUIBaseSlider.h"
#include "GUISlider.h"
#include "GUIBaseTextField.h"
#include "GUIBaseCheckBox.h"
#include "GUIRadioButtonGroup.h"
#include "GUIElementsButton.h"
#include "GUIRadioButton.h"
#include "GUIBaseInputField.h"
#include "GUIBaseMessageBox.h"
#include "GUIBaseRadioButton.h"
#include "GUIScrollBar.h"
#include "GUIMultiToggleButton.h"
#include "GUICheckBox.h"
#include "GUIRepositionCompositeObject.h"
#include "GUIIconButton.h"
#include "GUIProgressBar.h"
#include "GUIScrollView.h"
#include "GUICenterFocusContainer.h"
#include "GUITableView.h"
#include "GUIListBox.h"
#include "GUIListItem.h"
#include "GUIComboBoxHeader.h"
#include "GUIComboBox.h"
#include "GUIQuitCmd.h"
#include "GUILoadDialogCmd.h"
#include "GUIPlaybackSoundCmd.h"
#include "GUITransitionCmd.h"
#include "GUISetObjectStateCmd.h"
#include "GUIInputField.h"
#include "GUIMenuBar.h"
#include "GUIMenuItem.h"
#include "GUIMenuItemSeparator.h"
#include "GUIMenu.h"
#include "GUIAnimatedImage.h"
#include "GUICarousel.h"
#include "GUITextField.h"
#include "GUIMultiLineEdit.h"
#include "GUIImageStack.h"
#include "GUIScrollingTextField.h"
#include "GUITouchScrollView.h"
#include "GUIGauge.h"
#include "GUIGeometryObject.h"
#include "GUIBlendButton.h"
#include "GUIRichText.h"
#include "GUIProperties.h"
#include "GUILoadAnimationsCmd.h"
#include "GUIStartAnimationsCmd.h"
#include "GUIKeyboard.h"
#include "GUIWheel.h"

#include "GUIMemLeakWatcher.h"
#include "WindowsLeakWatcher.h"

CustomStandardFactory::CustomStandardFactory(void)
{
}

CustomStandardFactory::~CustomStandardFactory(void)
{
}

CGUIObject* CustomStandardFactory::CreateControl(ControlClassID_t eControlID)
{
    CGUIObject* pkNewObject=NULL;

    switch (eControlID)
    {
#define ENTRY(a, b) case a: pkNewObject = new b();           break;
    STRIPPED_CONTROL_TABLE
#undef ENTRY
    case DUMMY_CONTROL:
    default:
        break;
    }

    return pkNewObject;
}

CGUILayouter* CustomStandardFactory::CreateLayouter(LayouterClassID_t eLayouterID)
{
    CGUILayouter* pkNewLayouter=NULL;

    switch (eLayouterID)
    {
#define ENTRY(a, b) case a: pkNewLayouter = new b();           break;
    STRIPPED_LAYOUTER_TABLE
#undef ENTRY
    case DUMMY_LAYOUTER:
    default:
        break;
    }

    return pkNewLayouter;
}

CGUIBehaviourDecorator* CustomStandardFactory::CreateBehaviour(BehaviourClassID_t eBehaviourID)
{
    CGUIBehaviourDecorator* pkNewBehaviour=NULL;

    switch (eBehaviourID)
    {
#define ENTRY(a, b) case a: pkNewBehaviour = new b();           break;
    STRIPPED_BEHAVIOUR_TABLE
#undef ENTRY
    case DUMMY_BEHAVIOUR:
    default:
        break;
    }

    return pkNewBehaviour;
}

CGUICommand* CustomStandardFactory::CreateCommand(CommandClassID_t eCommandID)
{
    CGUICommand* pkNewCommand=NULL;

    switch (eCommandID)
    {
#define ENTRY(a, b) case a: pkNewCommand = new b();           break;
    STRIPPED_COMMAND_TABLE
#undef ENTRY
    case DUMMY_COMMAND:
    default:
        break;
    }

    return pkNewCommand;
}

CGUIText* CustomStandardFactory::CreateText(TextTypeID_t eTextType)
{
    CGUIText* pkNewText = NULL;

    switch (eTextType)
    {
#define ENTRY(a, b) case a: pkNewText = new b(NULL, DUMMY_TEXT);           break;
    STRIPPED_TEXTTYPE_TABLE
#undef ENTRY
    case TT_DUMMY_TEXT:
    default:
        break;
    }

    return pkNewText;
}

CGUIAnimation* CustomStandardFactory::CreateAnimation(AnimationType_t eAnimation)
{
    CGUIAnimation* pkAnimation = NULL;

    switch (eAnimation)
    {
#define ENTRY(a, b) case a: pkAnimation = new b();           break;
    STRIPPED_ANIMATION_TABLE
#undef ENTRY
    case DUMMY_ANIMATION:
    default:
        break;
    }

    return pkAnimation;
}

#endif
