#include "ExampleLayouter.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUILayouterResource.h"
#include "GUICompositeObject.h"
#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define EXAMPLE_LAYOUTER_CLASS_VERSION 2
// The minimal class version allowed.
#define EXAMPLE_LAYOUTER_CLASS_MIN_VERSION 1

ExampleLayouter::ExampleLayouter(eC_Bool bHorizontal, eC_Bool bVertical, CGUIObject* const pkObject) :
    CGUILayouter(pkObject),
    m_bHorizontal(bHorizontal),
    m_bVertical(bVertical)
{
    SetXMLTag("ExampleLayouter");
}

void ExampleLayouter::DoLayout(eMovedEdges_t eMovedEdges)
{
    CGUIObject* pkObj = GetAssociatedObject();
    if (pkObj == NULL)
    {
        return;
    }

    CGUICompositeObject* pkParent = pkObj->GetParent();
    if (pkParent == NULL)
    {
        return;
    }

    if (m_bHorizontal)
    {
        pkObj->SetRelXPos(eC_Div(pkParent->GetWidth() - pkObj->GetWidth(), eC_FromInt(2)));
    }

    if (m_bVertical)
    {
        pkObj->SetRelYPos(eC_Div(pkParent->GetHeight() - pkObj->GetHeight(), eC_FromInt(2)));
    }
}

#if defined(GUILIANI_STREAM_GUI)
void ExampleLayouter::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader(EXAMPLE_LAYOUTER_CLASS_VERSION, EXAMPLE_LAYOUTER_CLASS_MIN_VERSION);

    if (cuiVersion <= 1)
    {
        m_bHorizontal = GETINPUTSTREAM.READ_BOOL("CenterHorizontal");
        m_bVertical = GETINPUTSTREAM.READ_BOOL("CenterVertical");
        CGUILayouter::ReadFromStream();
    }
    else
    {
        // always base-class first
        CGUILayouter::ReadFromStream();

        m_bHorizontal = GETINPUTSTREAM.READ_BOOL("CenterHorizontal");
        m_bVertical = GETINPUTSTREAM.READ_BOOL("CenterVertical");
    }
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void ExampleLayouter::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader(bWriteClassID, XMLTAG_LAYOUTERCLASSID, LYT_EXAMPLE, EXAMPLE_LAYOUTER_CLASS_VERSION);

    CGUILayouter::WriteToStream();

    GETOUTPUTSTREAM.WriteBool(m_bHorizontal, "CenterHorizontal");
    GETOUTPUTSTREAM.WriteBool(m_bVertical, "CenterVertical");

    WriteStreamingFooter(bWriteClassID);
}
#endif
