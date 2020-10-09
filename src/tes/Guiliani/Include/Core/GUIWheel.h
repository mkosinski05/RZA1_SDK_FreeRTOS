#if !defined(GUIWHEEL_H)
#define GUIWHEEL_H

#include "GUIObject.h"
#include "GUIEasing.h"
#include "GUIImageResource.h"
#include "GUIText.h"

/// Displays a wheel in the GUI.
// @guiliani_doxygen toplevel_control Wheel
/**
<table border="0">
<tr>
<td width="200">@image html wheel.png</td>
<td>
The "wheel" control is used to display a wheel. The position and dimension of the image can be set freely. The scaling of the background image is arbitrary in x and y dimension -
so the aspect ratio can be changed. The "wheel" control is typically used to select a value out of a small number of elements e.g., temperatures.
The "wheel" can by endless (means: cyclic).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**
This control displays a wheel containing a range of values.

Example:
@code

// Create a wheel.
CGUIWheel* pkWheel = new CGUIWheel(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(30), eC_FromInt(30),
OBJ_WHEEL_ID,
);
pkWheel->AddEntry("aaaa");

@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUIWheel : public CGUIObject
{
protected:
    struct NodeInfo;
public:
    /// Type used for the nodes.
    typedef eC_TListDoubleLinked<NodeInfo> NodeList;

    /// Direction enumeration used for scrolling, dragging and destination direction.
    enum Direction_t
    {
        WHEEL_HORIZONTALLY,
        WHEEL_VERTICALLY,
    };

    /** Constructor
        @param pParent Pointer to the parent object of this image.
        @param vX The x coordinate of the upper left corner of this image.
        @param vY The y coordinate of the upper left corner of this image.
        @param vWidth The width of this image.
        @param vHeight The height of this image.
        @param eID This wheel's object ID.
    */
    CGUIWheel(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Default constructor
    CGUIWheel();
 
    /// Copy constructor
    CGUIWheel(const CGUIWheel& kToBeCopied);
 
    /// Assignment constructor
    CGUIWheel& operator=(const CGUIWheel& kSource);

    virtual ~CGUIWheel();
 
    /**
        initializes members.
    */
    void Init();
    
    /**
        De initializes members.
    */
    void DeInit();

    /** Set the minimum allowed value of the wheel
        @param iMinValue new minimum value */
    void SetMinValue( eC_Int iMinValue) { m_iMinValue = iMinValue; }

    /** Get the minimum allowed value of the wheel
        @return Minimum value */
    eC_Int GetMinValue() { return m_iMinValue; }

    /** Set the maximum allowed value of the wheel
        @param iMaxValue new maximum value */
    void SetMaxValue( eC_Int iMaxValue) { m_iMaxValue = iMaxValue; }

    /** Get the maximum allowed value of the wheel
        @return Maximum value */
    eC_Int GetMaxValue() { return m_iMaxValue; }

    /** Set the stepsize between two values within the wheel.
        @param iStepSize stepsize/distance of values in the wheel */
    void SetStepSize( eC_Int iStepSize) { m_iStepValue = iStepSize; }

    /** Set the string which will be appended as a postfix to each entry in the wheel-
        @param kPostfixString Postfix which will be appended to each item in the wheel. */
    void SetPostfixString(const eC_String& kPostfixString) { m_kPostfixString = kPostfixString; }

    /** Adapt the wheel to its current parameters. 
        Call this after updating the range of values. */
    virtual void AdaptWheel();

    /** Adds a new entry to the wheel.
        @param kEntry Value of new entry.
        @param bSort True if wheel should be sorted after adding entry.
    */
    virtual void AddEntry(const eC_String &kEntry, eC_Bool bSort = false);

    /** Deletes an entry from the wheel.
        @param kEntry Value of entry which will be deleted.
    */
    virtual void DeleteEntry(const eC_String &kEntry);

    /** Deletes all entries from the wheel.
    */
    virtual void DeleteAllEntries();

    /** Scrolls to the given entry/node index.
        @param uiNodeIndex Index of entry/node.
    */
    virtual void ScrollTo(eC_UInt uiNodeIndex);

    /** Scrolls to the given entry/node value.
        @param kNodeText Value of entry/node.
    */
    virtual void ScrollTo(const eC_String &kNodeText);

    /** Scrolls using animations to the given entry/node value.
        @param uiNodeIndex Index of entry/node.
    */
    virtual void ScrollToAnimated(eC_UInt uiNodeIndex);

    /** Scrolls using animations to the given entry/node value.
        @param kNodeText Value of entry/node.
    */
    virtual void ScrollToAnimated(const eC_String &kNodeText);

    /** GetCurrentIndex returns the current focused entry value.
        @return eC_UInt containing the current focused index.
    */
    virtual eC_UInt GetCurrentIndex() { return GetCurrentNode(); }

    /** GetCurrentValue returns the current focused entry value.
        @return eC_String containing the current value.
    */
    virtual const eC_String& GetCurrentValue() 
    {
        NodeList::Iterator iter = m_kNodes.GetAt(GetCurrentNode());
        if (iter.IsValid())
        {
            return (*iter).m_sNodeText; 
        }
        return eC_String::ms_ckEmptyString;
    };

    /**  SearchEntry returns the index of an existing entry value.
         If the given value is not part of the list -1 is returned.
         @note kNodeText will automatically be adapted if a Minimum length and FillUp-string for the wheel were given!
         @param kNodeText Value of entry/node which is searched for.
         @return -1 if not found else the index containing the searched value.
    */
    virtual eC_Int SearchEntry(const eC_String &kNodeText);

    // Methods derived from CGUIObject 
    virtual eC_Bool DoDraw();
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));
    virtual eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);
    virtual void SetWidth(const eC_Value &vWidth);
    virtual void SetHeight(const eC_Value &vHeight);
    virtual eC_Bool SetValue(const CGUIValue& rkValue);

    /** Returns whether the wheel is in Cyclic mode (Nodes continue from the beginning, when scrolling past the end).
        @return True if wheel is cyclic, False otherwise. */
    eC_Bool IsCyclic() const { return m_bCyclic; }

    /** Specifies whether the wheel is in Cyclic mode (Nodes continue from the beginning, when scrolling past the end).
        @param bCyclic True if wheel shall be cyclic, False otherwise. */
    void SetCyclic(eC_Bool bCyclic) { m_bCyclic = bCyclic; }

    /** Returns whether the wheel is using the given min/max/stepsize values to automatically generate its entries.
        @return True if range values are used to automatically generate content, False otherwise. */
    eC_Bool IsUsingRange() const { return m_bCyclic; }

    /** Specifies whether the wheel is using the given min/max/stepsize values to automatically generate its entries.
        Instead of automatically generating entries from the given range, you can manually add entries via the AddEntry() method.
        @param bUseRange True if range values are used to automatically generate content, False otherwise. */
    void SetUseRange(eC_Bool bUseRange) { m_bUseRange = bUseRange; }

    /** Returns the total number of entries in the wheel.
        @return The total number of entries in the wheel */
    eC_UInt GetNumberOfEntries() { return m_kNodes.GetQuantity(); }

    /** Prefixes the given entry with the contents of the FillupString until it reaches the defined minimum length.
        This also prefixes the entry with m_kPrefixString and appends m_kPostfixString
        @param kEntry Text which will be extended to the minimum length and get pre-/postfixed
    */
    virtual void NormalizeEntry(eC_String& kEntry);

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
        @param bWriteClassID This flag is used to select if writing of control
               class ID, leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif


protected:

    /** If m_bUseRange is true this method creates nodes between m_iMinValue and m_iMaxValue
        using m_iStepValue as step size.
    */
    virtual void CreateNodesFromRange();
 
    /** Adjusts all nodes GUIText to reflect the current position
    */
    virtual void AdjustNodesRelativePositions();
    
    /** Adjusts all nodes GUIText to reflect the current position. Used for cyclic wheels.
    */
    virtual void AdjustNodesRelativePositionsCyclic();
    
    /** Adjusts all nodes GUIText to reflect the current position. Used for non-cyclic wheels.
    */
    virtual void AdjustNodesRelativePositionsNotCyclic();
 
    /** Adjusts all the current node and scrolling position dependent of the DragDelta.
        @param vDragDelta The offset to which the text positions should be adjusted.
    */
    virtual void AdjustCurrentScrollPosAndNode(eC_Value vDragDelta);
    
    /** Adjusts the sizes of remaining space, number of visible nodes and default height.
    */
    virtual void AdjustSize();

    /** Retrieves the node index specified via an offset in pixels.
        @param vOffset Offset in pixels to the current position.
        @param iIndex Resulting index.
        @param iCycle Resulting cycle.
        @param vScrollPos Resulting scrolling position.
    */
    virtual void GetNodeIndexInRelPos(eC_Value vOffset, eC_Int &iIndex, eC_Int &iCycle, eC_Value &vScrollPos);
     
    /** Sets the current node index via index.
        @param uiNodeIndex The current node index to set.
    */
    virtual void SetCurrentNode(eC_UInt uiNodeIndex);
    
    /** Retrieves the current node index.
        @return The current node index.
    */
    virtual eC_UInt GetCurrentNode() { return m_CurrentNodeIndex; }
    
    /** Retrieves an iterator pointing to the current node.
        @return The iterator to the current position
    */
    virtual NodeList::SafeIterator GetCurrentNodeSafeIter() { return m_CurrentNodeSafeIter; }
    
    /** Retrieves the index pointing to the current node.
        @param kNodeIter Iterator pointing to the position to which the index should be returned.
        @return The iterator to the current position
    */
    virtual eC_UInt GetNodeIndexFromIter(const NodeList::Iterator &kNodeIter);
    
    /** Set the current wheel position via Iterator.
        @param kNodeSafeIter Iterator pointing to the new position.
    */
    virtual void SetCurrentNodeSafeIter(const NodeList::SafeIterator &kNodeSafeIter);
     
    /** Retrieves an iterator pointing to the next node.
        @param  kIter Iterator pointing to double linked list.
        @return An iterator to the next node.
    */
    virtual NodeList::Iterator GetNextNodeCyclic(NodeList::Iterator kIter);
     
    /** Retrieves an iterator pointing to the previous node.
        @param  kIter Iterator pointing to double linked list.
        @return An iterator to the previous node.
    */
    virtual NodeList::Iterator GetPreviousNodeCyclic(NodeList::Iterator kIter);

    /** Gets the current scrolling position dependent of direction.
        @return Value of the scrolling position.
    */
    virtual eC_Value GetCurrentScrollPosForCurrentDirection() { if (m_eDirection == WHEEL_HORIZONTALLY) return m_vCurrentScrollPosX; else return m_vCurrentScrollPosY; }

    /** Sets the scrolling position dependent to the direction of the wheel.
        @param vCurrentScrollPos New scrolling position horizontal or vertical.
    */
    virtual void SetCurrentScrollPosForCurrentDirection(eC_Value vCurrentScrollPos) { if (m_eDirection == WHEEL_HORIZONTALLY) m_vCurrentScrollPosX = vCurrentScrollPos; else m_vCurrentScrollPosY = vCurrentScrollPos; }

    /** Retrieves the node size dependent to the direction of the wheel.
        @return Value of the node size for the current direction.
    */
    virtual eC_Value GetDefaultNodeSizeCurrentDirection() { if (m_eDirection == WHEEL_HORIZONTALLY) return m_vNodeDefaultWidth; else return m_vNodeDefaultHeight; }

    /** Sets the node/entry size.
        @param vWidth Value of new node/entry width.
        @param vHeight Value of new node/entry height.
    */
    virtual void SetDefaultNodeSize(eC_Value vWidth, eC_Value vHeight) { if (vWidth <= 0) vWidth = eC_FromInt(1); m_vNodeDefaultWidth = vWidth; if (vHeight <= 0) vHeight = eC_FromInt(1); m_vNodeDefaultHeight = vHeight; }

    /** Start animation.
    */
    virtual void StartScrollingAnimation();

    /** Stop animation.
    */
    virtual void StopScrollingAnimation();

    /** Returns a reference to the internal list of nodes.
        @return List of nodes */
    NodeList& GetNodes() { return m_kNodes; }

    /// Contains information about one node.
    struct NodeInfo
    {
        eC_String m_sNodeText; ///< Value of entry displayed.
        CGUIText *m_pGUIText;  ///< GUIText needed to display value

        /** Copy constructor
            @param kToBeCopied Node from which to copy */
        NodeInfo(const NodeInfo& kToBeCopied):m_sNodeText(""), m_pGUIText(NULL) { operator=(kToBeCopied); }
        /// Default constructor
        NodeInfo() :m_sNodeText(""), m_pGUIText(NULL) { }
        /** Constructor
            @param pParent Object to which the internal text shall be attached
            @param kNodeText Text string to display on the text object
            @param vXPos the text's relative x position
            @param vYPos the text's relative y position
            @param vWidth the text's width
            @param vHeight the text's height */
        NodeInfo(CGUIObject *pParent, const eC_String &kNodeText, eC_Value vXPos, eC_Value vYPos, eC_Value vWidth, eC_Value vHeight) :
            m_sNodeText(kNodeText)
        { 
            m_pGUIText = new CGUIText(pParent, kNodeText, vXPos, vYPos); 
            m_pGUIText->SetAligned(CGUIText::V_CENTERED, CGUIText::H_CENTERED);
            m_pGUIText->SetTextWidth(vWidth);
            m_pGUIText->SetTextHeight(vHeight);
            
            m_pGUIText->SetFont(DUMMY_FONT);
        }
        /** Assignment Operator
            @param kNode Node from which to copy
            @return this node */
        NodeInfo& operator=(const NodeInfo& kNode)
        {
            if (&kNode != this)
            {
                m_sNodeText = kNode.m_sNodeText;
                delete m_pGUIText;
                m_pGUIText = kNode.m_pGUIText->Clone();
                m_pGUIText->SetContainingObject(kNode.m_pGUIText->GetContainingObject());
            }
            return *this;
        };
        /** Compare operators needed in order to used Sort()
            @param kNode Node with which to compare
            @return True When the two node's texts are identical */
        bool operator==(const NodeInfo& kNode)
        {
            return kNode.m_sNodeText == m_sNodeText; 
        };
        /** Compare operators needed in order to used Sort()
            @param kNode Node with which to compare
            @return True if the other node's value is larger. */
        bool operator>(const NodeInfo& kNode)
        {
            eC_Int iValueMe = 0;
            eC_Int iValueOther = 0;
            eC_Bool bIsNumMe = m_sNodeText.ToInt(iValueMe);
            eC_Bool bIsNumOther = kNode.m_sNodeText.ToInt(iValueOther);
            if (bIsNumMe && bIsNumOther)
            {
                return iValueOther > iValueMe;
            }
            else if (!bIsNumMe && !bIsNumOther)
            {
                return kNode.m_sNodeText > m_sNodeText;
            }
            return bIsNumMe;
        };
        ///Destructor
        ~NodeInfo() { if (m_pGUIText) delete m_pGUIText;  m_pGUIText = NULL; }
    };

    NodeList m_kNodes;///< List containing all nodes/entries.

    NodeList::SafeIterator m_CurrentNodeSafeIter;///< Iterator pointing to the current node/entry.
    eC_UInt m_CurrentNodeIndex; ///< Index of current node.
    eC_Value m_vCurrentScrollPosX; ///< Scrolling position of current node (offset in pixels (0-m_vNodeDefaultWidth)).
    eC_Value m_vCurrentScrollPosY;  ///< Scrolling position of current node (offset in pixels (0-m_vNodeDefaultHeight)).

    eC_Value m_vTargetPos;///< Target position in pixels for animation

    // Used for animation
    // Animation start position
    eC_Int m_iAniStartNodeIndex;///< Animation start node index.
    eC_Int m_iAniStartNodeCycle;///< Animation start node cycle.
    eC_Value m_vAniStartScrollPos;///< Animation start scrolling position
    // Animation target position
    eC_Int m_iAniTargetNodeIndex;///< Animation target node.
    eC_Int m_iAniTargetNodeCycle;///< Animation target node cycle.
    eC_Value m_vAniTargetScrollPos;///< Animation target scrolling position.

    /// Calculates the index, cycle and offset (scrolling position) out of a pixel position.
    virtual void PixelToIndexCycleOffset(eC_Value vPixel, eC_Int &iIndex, eC_Int &iCycle, eC_Value &vOffset);

    /// Calculates the position in pixels out of entry index, cycle and offset (scrolling position).
    virtual eC_Value IndexCycleOffsetToPixel(eC_Int iIndex, eC_Int iCycle, eC_Value vOffset);

    eC_Value m_vNodeDefaultWidth;///< Entry width in pixels. Has to be > 0.
    eC_Value m_vNodeDefaultHeight;///< Entry height in pixels. Has to be > 0

    eC_Bool m_bCyclic;///< True if wheel is cyclic. False if non-cyclic. The wheel then stops at begin/end of entries.
    eC_Bool m_bUseRange;///< If true entries will be automatically generated with the use of below values.
    eC_Int m_iMinValue;///< Smallest generated entry.
    eC_Int m_iMaxValue;///< Biggest generated entry
    eC_Int m_iStepValue;///< Step size used for entry generation.
    eC_Int m_iMinDigitNumber; ///< Minimal number of digits
    eC_String m_kFillupString; ///< String will be used to fill up missing digits.
    eC_String m_kPrefixString; ///< String will be place in front of entry.
    eC_String m_kPostfixString; ///< String will be place behind of entry. Can be used for example for units etc.
    eC_Bool m_bFillFromBegin;///< If true entries will be filled in front of entry, if false at end.
    
    eC_UInt     m_uiDragTimeStamp;///< Time at start of animation and Timestamp of last received DoDrag()-Event
    eC_UInt     m_uiDragStarted; ///< Holds the time when the last drag was started. Used to calculate the drag speed.
    CGUIPoint   m_kDragStart; ///< Holds the start point of a drag.

    ImageResource_t  m_eBGImage; ///< Used background image

    Direction_t  m_eDirection; ///< Direction of the wheel. Either horizontal or vertical.

    /// EasingType used for kinetic animation
    CGUIEasing::EasingType_t  m_eEasingTypeCurrent; ///< Current active easing.
    CGUIEasing::EasingType_t  m_eEasingTypeNormal; ///< Easing used for all animations which will not go further then the wheel end.
    CGUIEasing::EasingType_t  m_eEasingTypeBack; ///< Easing for non-cyclic wheel if dragged further then wheel size.

    /// Duration of easing animation for kinetic scrolling
    eC_Value m_vEasingDurationCurrent; ///< Current active easing duration.
    eC_Value m_vEasingDurationNormal; ///< Easing duration used for all animations which will not go further then the wheel end.
    eC_Value m_vEasingDurationBack; ///< Easing duration for non-cyclic wheel if dragged further then wheel size.

    FontResource_t m_eEntriesFont;///< Font of all non focused entries.
    FontResource_t m_eFocussedEntryFont;///< Font of the focused entry.
    eC_UInt m_uiEntriesColor;///< Text color of all non focused entries.
    eC_UInt m_uiFocussedEntryColor;///< Text color of focused entry.
    eC_String m_kFocussedValue;///< Current focused value.
    eC_Value m_vNodeX;///< Node horizontal feed rate for the gui text positions.
    eC_Value m_vNodeY;///< Node vertical feed rate for the gui text positions.
    eC_UInt m_uiNumOfVisibleNodes;///< Number of visible nodes of the wheel.
    eC_Value m_vRemainingSpaceX;///< If the nodes are not fitting exactly this is the rest space.
    eC_Value m_vRemainingSpaceY;///< If the nodes are not fitting exactly this is the rest space.
    eC_Value m_vAdditionalSpace;///< Used for positioning in DoDraw.
};

#endif // GUIWHEEL_H


