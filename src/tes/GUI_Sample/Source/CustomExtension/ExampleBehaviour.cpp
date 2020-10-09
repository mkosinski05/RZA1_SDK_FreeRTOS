#include "ExampleBehaviour.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUIBehaviourResource.h"
#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define EXAMPLE_BEHAVIOUR_CLASS_VERSION 2
// The minimal class version allowed.
#define EXAMPLE_BEHAVIOUR_CLASS_MIN_VERSION 1

ExampleBehaviour::ExampleBehaviour(DragAction eAction, CGUIObject* const pObject) :
    CGUIBehaviourDecorator(pObject),
    m_eDragAction(eAction)
{
    SetXMLTag("ExampleBehaviour");
}

/// Default constructor to be used by the factory.
ExampleBehaviour::ExampleBehaviour()
{
    SetXMLTag("ExampleBehaviour");
}

eC_Bool ExampleBehaviour::DoDrag(
    const eC_Value &vDeltaX,
    const eC_Value &vDeltaY,
    const eC_Value &vAbsX,
    const eC_Value &vAbsY)
{
    SetXMLTag("ExampleBehaviour");

    CGUIObject* pkObj = GetAssociatedObject();
    if (pkObj == NULL)
        return false;

    // First invalidate the 'old' area (before moving or resizing the object).
    pkObj->InvalidateArea();

    switch (m_eDragAction)
    {
        case DA_MOVE:
            pkObj->SetRelXPos(pkObj->GetRelXPos() + vDeltaX);
            pkObj->SetRelYPos(pkObj->GetRelYPos() + vDeltaY);
            break;

        case DA_SIZE:
        {
            eC_Value vNewWidth = pkObj->GetWidth() + vDeltaX;
            eC_Value vNewHeight = pkObj->GetHeight() + vDeltaY;
            if (vNewWidth > eC_FromInt(0))
            {
                pkObj->SetWidth(vNewWidth);
            }
            if (vNewHeight > eC_FromInt(0))
            {
                pkObj->SetHeight(vNewHeight);
            }
        }
        break;

        default:
            // give some other object up the tree the chance to handle the event
            return false;
    }

    // Finally invalidate the 'new' area (after moving or resizing the object).
    pkObj->InvalidateArea();

    return false;
}

#if defined(GUILIANI_STREAM_GUI)
void ExampleBehaviour::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader(EXAMPLE_BEHAVIOUR_CLASS_VERSION, EXAMPLE_BEHAVIOUR_CLASS_MIN_VERSION);

    if (cuiVersion <= 1)
    {
        m_eDragAction = static_cast<DragAction>(GETINPUTSTREAM.READ_INT("DragAction"));
        CGUIBehaviourDecorator::ReadFromStream();
    }
    else
    {
        // always base-class first
        CGUIBehaviourDecorator::ReadFromStream();

        m_eDragAction = static_cast<DragAction>(GETINPUTSTREAM.READ_INT("DragAction"));
    }
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void ExampleBehaviour::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader(bWriteClassID, XMLTAG_BEHAVIOURCLASSID, BHV_EXAMPLE, EXAMPLE_BEHAVIOUR_CLASS_VERSION);

    // always base-class first
    CGUIBehaviourDecorator::WriteToStream();

    GETOUTPUTSTREAM.WriteInt(m_eDragAction, "DragAction");

    WriteStreamingFooter(bWriteClassID);
}
#endif
