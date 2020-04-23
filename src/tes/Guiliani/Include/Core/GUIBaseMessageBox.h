/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASEMESSAGEBOX__H_
#define GUIBASEMESSAGEBOX__H_

#include "GUITextResource.h"
#include "GUICompositeObject.h"
#include "GUIGeometryObject.h"
#include "GUIBaseTextField.h"
#include "GUIText.h"

class CGUIBaseButton;

/// This is the Message Box base class.

/**
This class is used to pop up a message on the screen.
It remains on the screen until its OK button is pressed.

Example:
@code

// Create base message box.
CGUIBaseMessageBox *pkBaseMessageBox = new CGUIBaseMessageBox(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(100), eC_FromInt(100),
"Message", OBJ_BASEMESSAGEBOX);
@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUIBaseMessageBox
    : public CGUICompositeObject
{
public:
    /** constructor
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the message box
    @param vHeight Height of the message box
    @param kTitle String to show in the title bar
    @param bModal create modal box
    @param eID Object ID
    */
    CGUIBaseMessageBox(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &kTitle,
        const eC_Bool &bModal = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** constructor
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the message box
    @param vHeight Height of the message box
    @param eTextID TextID to show in the title bar
    @param bModal create modal box
    @param eID Object ID
    */
    CGUIBaseMessageBox(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const TextResource_t &eTextID,
        const eC_Bool &bModal = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** constructor
    @param pParent Pointer to the designated parent object.
    @param kRect CGUIRect defining position and size
    @param kTitle String to show in the title bar
    @param bModal create modal box
    @param eID Object ID
    */
    CGUIBaseMessageBox(
        CGUICompositeObject *const pParent,
        const CGUIRect& kRect,
        const eC_String &kTitle,
        const eC_Bool &bModal = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);
    eC_Bool ClaimFocus();

    /** Sets the background-color
    @param uiColor color to use a background-color
    */
    void SetBackgroundColor(const eC_UInt uiColor)
    {
        if (NULL != m_pkBackground)
        {
            m_pkBackground->SetColor(uiColor);
            m_pkBackground->InvalidateArea();
        }
    }

    /** Sets the color of the text displayed in the messagebox
    @param uiTextColor color to set
    */
    void SetTextColor(const eC_UInt uiTextColor)
    {
        if (NULL != m_pkMessageBoxText)
        {
            m_pkMessageBoxText->GetLabel()->SetTextColor(uiTextColor, uiTextColor, uiTextColor, uiTextColor);
            m_pkMessageBoxText->InvalidateArea();
        }
    }

    /** Sets the font-id of the text displayed in messagebox
    @param eFontID font-id to use
    */
    void SetTextFont(const FontResource_t& eFontID)
    {
        if (NULL != m_pkMessageBoxText)
        {
            m_pkMessageBoxText->GetLabel()->SetFont(eFontID);
            m_pkMessageBoxText->InvalidateArea();
        }
    }

    /** Sets the text messagebox using string
    @param kText String to use as text
    */
    void SetText(const eC_String& kText)
    {
        if (NULL != m_pkMessageBoxText)
        {
            m_pkMessageBoxText->GetLabel()->SetTextString(kText);
        }
    }

    /** Sets the text of messagebox using a TextID
    @param eTextID TextID to use
    */
    void SetText(const TextResource_t& eTextID)
    {
        if (m_pkMessageBoxText)
        {
            m_pkMessageBoxText->GetLabel()->SetTextID(eTextID);
        }
    }

    /**    Standard Constructor.
    @see CGUIObject().
    */
    CGUIBaseMessageBox();

    virtual ~CGUIBaseMessageBox();

    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
    There is no implementation for this method. */
    CGUIBaseMessageBox(const CGUIBaseMessageBox& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
    There is no implementation for this method. */
    CGUIBaseMessageBox& operator=(const CGUIBaseMessageBox& kSource);

    /** Helper function for constructors. */
    void Init(const eC_Value& vX, const eC_Value& vY, const eC_Value &vWidth, const eC_Value &vHeight, const eC_Bool& bModal);

    CGUIGeometryObject* m_pkBackground;
    CGUICompositeObject* m_pkContainer;
    CGUIBaseTextField* m_pkMessageBoxText;
    CGUIBaseButton* m_pOKButton;

    eC_Bool m_bModal;
    eC_Value m_vDragPosX;
    eC_Value m_vDragPosY;
};

#endif
