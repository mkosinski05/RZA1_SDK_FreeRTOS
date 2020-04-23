/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIOBJECTSTATEBEHAVIOUR__H_
#define GUIOBJECTSTATEBEHAVIOUR__H_

#include "GUIBehaviourDecorator.h"
#include "GUIObjectHandleResource.h"

/// @brief Behaviour for processing object state changes
class CGUIObjectStateBehaviour 
    : public CGUIBehaviourDecorator
{
public:
    /** Constructor
        @param eObjID Object ID
        @param bToggle If true, all states marked as "True" will be toggled. Otherwise, all states will be set to True/False exactly as given.
        @param bFocusable True if focusable
        @param bInvisible True if invisible
        @param bGrayedOut True if grayed out
        @param bDisabled True if disabled
    */
    CGUIObjectStateBehaviour(ObjectHandle_t eObjID, eC_Bool bToggle, eC_Bool bFocusable, eC_Bool bInvisible, eC_Bool bGrayedOut, eC_Bool bDisabled) :
        CGUIBehaviourDecorator(), 
        m_eObjID(eObjID),
        m_bToggle(bToggle),
        m_bFocusable(bFocusable),
        m_bInvisible(bInvisible),
        m_bGrayedOut(bGrayedOut),
        m_bDisabled(bDisabled)
    {
        SetXMLTag("GUIObjectStateBehaviour");   
    }

    ///Default constructor
    CGUIObjectStateBehaviour():
        CGUIBehaviourDecorator(), 
        m_eObjID(NO_HANDLE),
        m_bToggle(false),
        m_bFocusable(false),
        m_bInvisible(false),
        m_bGrayedOut(false),
        m_bDisabled(false)
    {
        SetXMLTag("GUIObjectStateBehaviour");
    }


#if defined(GUILIANI_STREAM_GUI)
    /** Reads all attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of command
               class ID, leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    
    /// Implements the actual command functionality.
    eC_Bool DoClick(const eC_Value &vAbsX, 
                    const eC_Value &vAbsY);
private:

    ObjectHandle_t m_eObjID;

    /// Specifies whether the states shall be toggled on/off, or if they will be set exactly as given.
    eC_Bool m_bToggle;

    // Object states which can be changed through this command
    eC_Bool m_bFocusable; ///< Whether the object is focusable
    eC_Bool m_bInvisible; ///< Whether the object is invisible
    eC_Bool m_bGrayedOut; ///< Whether the object is grayed out
    eC_Bool m_bDisabled; ///< Whether the object is disabled
};
#endif //GUIOBJECTSTATEBEHAVIOUR__H_
