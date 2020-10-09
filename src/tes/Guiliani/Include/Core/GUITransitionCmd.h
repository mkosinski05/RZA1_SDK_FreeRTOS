/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_TRANSITION_COMMAND_H
#define GUI_TRANSITION_COMMAND_H

#include "GUICommand.h"
#include "GUIObjectHandleResource.h"
#include "GUITransitionBase.h"
#include "GUIEasing.h"

/// Transits from a source dialog to a destination dialog.
/** This command loads a GUI definition which results in a new CGUIObject.
    The new object will be added to the parent of the source object.
    A transition is computed from source CGUIObject to the new one.
    The source object will be deleted when the transition has finished.

    If something goes wrong while streaming (loading the GUI definition), this
    command does not delete any GUI objects.

    @ingroup GUILIANI_COMMANDS
    */

class CGUITransitionCmd :
    public CGUICommand
{
public:
#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /** XML tag used to indicate a target dialog filename to load. */
    static const eC_Char XMLTAG_DESTDIALOGFILENAME[];
#endif

    ///Transition types
    enum TransitionType_t
    {
        BLEND_ONLY,
        BLEND_FADE,
        BLEND_AND_SHRINK,
        BLEND_AND_ZOOM,
        PUSH_FROM_LEFT,
        PUSH_FROM_RIGHT,
        PUSH_FROM_TOP,
        PUSH_FROM_BOTTOM,
        ROTATE_X_AXIS_CW,
        ROTATE_X_AXIS_CCW,
        ROTATE_Y_AXIS_CW,
        ROTATE_Y_AXIS_CCW,
        DISSOLVE
    };

    /** Constructs a TransitionCmd.
        @param pTransition Transition to be used for dialog change
        @param eSource ID of an object to transit from. To be deleted when this transition is finished.
        @param rkDestFileName File name of the dialog to be loaded.
    */
    CGUITransitionCmd(CGUITransitionBase *pTransition,
                      ObjectHandle_t eSource,
                      const eC_String& rkDestFileName);

    /** Constructs a TransitionCmd.
        @param eTransType Transition type to be used for dialog change
        @param eSource ID of an object to transit from. To be deleted when this transition is finished.
        @param rkDestFileName File name of the dialog to be loaded.
    */
    CGUITransitionCmd(TransitionType_t eTransType,
                      ObjectHandle_t eSource,
                      const eC_String& rkDestFileName);

    /** Constructs a TransitionCmd.
        @param eTransType Transition type to be used for dialog change
        @param eSource ID of an object to transit from. To be deleted when this transition is finished.
        @param rkDestFileName File name of the dialog to be loaded.
        @param uiTransitionTime The transition time
    */
    CGUITransitionCmd(TransitionType_t eTransType,
                      ObjectHandle_t eSource,
                      const eC_String& rkDestFileName,
                      eC_UInt uiTransitionTime);

    /** Constructs a TransitionCmd.
        @param eTransType Transition type to be used for dialog change
        @param eSource ID of an object to transit from. To be deleted when this transition is finished.
        @param eDest Dialog to be transitioned to.
    */
    CGUITransitionCmd(TransitionType_t eTransType,
                      ObjectHandle_t eSource,
                      ObjectHandle_t eDest);

    /** Constructs a TransitionCmd.
        @param eTransType Transition type to be used for dialog change
        @param eSource ID of an object to transit from. To be deleted when this transition is finished.
        @param eDest Dialog to be transitioned to.
        @param uiTransitionTime The transition time
    */
    CGUITransitionCmd(TransitionType_t eTransType,
                      ObjectHandle_t eSource,
                      ObjectHandle_t eDest,
                      eC_UInt uiTransitionTime);

    /// Constructor for factory. Do not use this constructor in user code.
    CGUITransitionCmd();

    /** Sets the Time a transition takes.
        @param uiTransitionTime Time transition will need to switch from source
        to destination.
    */
    void SetTransitionTime(eC_UInt uiTransitionTime);

    /** Gets the time a transition takes.
        @return the transition time.
    */
    eC_UInt GetTransitionTime() const;

    /** Sets the ObjectID of the transition source.
        @param eSource ObjectID of transition source.
    */
    void SetSourceObjectID(ObjectHandle_t eSource);

    /** Gets the ObjectID of the transition source.
        @return ObjectID of transition source.
    */
    ObjectHandle_t GetSourceObjectID() const;

    /** Sets the ObjectID of the transition destination.
        @param eDest ObjectID of transition destination.
    */
    void SetDestinationObjectID(ObjectHandle_t eDest);

    /** Gets the ObjectID of the transition destination.
        @return ObjectID of transition destination.
    */
    ObjectHandle_t GetDestinationObjectID() const;

    /** Sets the transition type of the transition.
        @param eTransitionType type of transition.
    */
    void SetTransitionType(CGUITransitionCmd::TransitionType_t eTransitionType);

    /** Gets the transition type of the transition.
        @return type of transition.
    */
    CGUITransitionCmd::TransitionType_t GetTransitionType() const;

    /** Sets the destination filename.
        @param kDestFileName The path to the file containing the destination dialog.
    */
    void SetDestinationFileName(eC_String &kDestFileName);

    /** Gets tthe destination filename.
        @return type of transition.
    */
    const eC_String& GetDestinationFileName() const;

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Helper for initialization
    void Init();

    virtual ~CGUITransitionCmd();

    void Do();

    /// Transition to be used for dialog change
    CGUITransitionBase* m_pTransition;
    /// Transition Type to be used for dialog change
    TransitionType_t    m_eTransitionType;
    /// Easing Type to be used for dialog change
    CGUIEasing::EasingType_t m_eEasingType;
    /// Total duration of transition in ms
    eC_UInt             m_uiTransitionTime;
    /// ID of object to be deleted.
    ObjectHandle_t m_eSource;
    /// ID of object to be deleted.
    ObjectHandle_t m_eDest;
    /// File name of the dialog to be loaded.
    eC_String m_kDestFileName;
};

#endif
