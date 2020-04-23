/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITABCONTAINER__H_
#define GUITABCONTAINER__H_

#include "GUICompositeObject.h"
#include "GUIImageResource.h"

#include "GUIEventHandler.h"
#include "GUIButton.h"

/**
this Class defines a sub-object with is used within the CGUITabContainer
*/
class CGUITabItem
    : public CGUICompositeObject
{
public:
    /** CGUICompositeObject standard constructor.
    @see CGUIObject().
    */
    CGUITabItem();

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the composite object
    @param vHeight Height of the composite object
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUITabItem(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect Bounding rectangle relative to parent.
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUITabItem(CGUICompositeObject *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUITabItem()
    {
    }

    /**
    return text of the TabItem
    @return the currently used text
    */
    eC_String GetText() const
    {
        return m_kText;
    }

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

private:
    eC_String m_kText;
};

/**
The Tab-Container is a control which is used to group objects within a dialog and to switch between them.
Every child-object is a TabItem which holds the objects which should be displayed as a group.
These TabItems have a button displaying the name of the group and set the corresponding TabItem as the active child.
*/
class CGUITabContainer :
    public CGUICompositeObject
{
public:
    /**
    Position of the Buttonbox
    */
    enum TabButtonPosition_t
    {
        POS_TOP, ///< Top
        POS_LEFT, ///< Left
        POS_BOTTOM, ///< Bottom
        POS_RIGHT //< Right
    };

    /** CGUICompositeObject standard constructor.
    @see CGUIObject().
    */
    CGUITabContainer();

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the composite object
    @param vHeight Height of the composite object
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUITabContainer(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect Bounding rectangle relative to parent.
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUITabContainer(CGUICompositeObject *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUITabContainer() {};

    /**
    add a new TabItem to the Container
    @param pkTabItem TabItem to insert
    @return true if successful, else false
    */
    virtual eC_Bool AddObject(CGUITabItem *pkTabItem);

    /**
    add a new object to the Container
    @param pkObject object to insert
    @return true if successful, else false
    */
    virtual eC_Bool AddObject(CGUIObject *pkObject);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    virtual eC_Bool DoClick(const eC_Value& vAbsX, const eC_Value& vAbsY);

    virtual void SetWidth(const eC_Value &vX);
    virtual void SetHeight(const eC_Value &vY);

    /**
    sets the number of tabs which are displayed
    @param uiNumberOfTabs number of tabs
    */
    void SetNumberOfTabs(const eC_UInt uiNumberOfTabs)
    {
        if (m_uiNumberOfTabs != uiNumberOfTabs)
        {
            m_uiNumberOfTabs = uiNumberOfTabs;
            ReArrangeButtons();
        }
    }

    /**
    Sets position of the buttonbox
    @param eTabButtonPosition position of buttonbox
    */
    void SetButtonPosition(const TabButtonPosition_t eTabButtonPosition)
    {
        if (m_eTabButtonPosition != eTabButtonPosition)
        {
            m_eTabButtonPosition = eTabButtonPosition;
            ReArrangeButtons();
        }
    }

    /**
    set images for buttons in the buttonbox
    @param eBtnStandard image for standard
    @param eBtnPressed image for pressed
    @param eBtnHighlighted image for highlighted
    @param eBtnGrayedOut image for grayed out
    @param eBtnFocused image for focused
    */
    void SetButtonImages(
        const ImageResource_t &eBtnStandard,
        const ImageResource_t &eBtnPressed,
        const ImageResource_t &eBtnHighlighted,
        const ImageResource_t &eBtnGrayedOut,
        const ImageResource_t &eBtnFocused)
    {
        m_eBtnStandard = eBtnStandard;
        m_eBtnPressed = eBtnPressed;
        m_eBtnHighlighted= eBtnHighlighted;
        m_eBtnGrayedOut = eBtnGrayedOut;
        m_eBtnFocused = eBtnFocused;

        ReArrangeButtons();
    }

    /**
    Returns a pointer to the client-area of the TabContainer
    @return pointer to client-area
    */
    CGUICompositeObject* GetClientArea()
    {
        return m_pkClientArea;
    }

private:
    /** initialize the container and buttons
    */
    void Init();

    /** re-arrange buttons after layout or sizes have been changed
    */
    void ReArrangeButtons();

    /** adjust container sizes and positions according to tab-position
    */
    void AdjustContainers();

    /** switch the child-element with the given index to visible and all others invisible
    */
    void SwitchChildren(const eC_UInt uiIndex);

private:
    TabButtonPosition_t m_eTabButtonPosition;
    eC_UInt m_uiNumberOfTabs;
    eC_UInt m_uiActiveTabIndex;
    eC_Value m_vButtonWidth;
    eC_Value m_vButtonHeight;
    eC_Value m_vMaxButtonWidth;
    eC_Value m_vMaxButtonHeight;
    eC_Bool m_bAutoSizeButtons;

    ImageResource_t m_eBtnPressed; ///< the pressed image
    ImageResource_t m_eBtnHighlighted; ///< the highlighted image
    ImageResource_t m_eBtnStandard; ///< the standard image
    ImageResource_t m_eBtnGrayedOut; ///< the grayed out image
    ImageResource_t m_eBtnFocused; ///< the focussed image

    CGUIButton* m_pkButtons;

    CGUICompositeObject* m_pkButtonBox;
    CGUITabItem* m_pkClientArea;
};

#endif
