#include "<*>LAYOUTER_CLASS_NAME<*>.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUILayouterResource.h"

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define <*>LAYOUTER_CLASS_VERSION_NAME<*> <*>LAYOUTER_CLASS_VERSION<*>

<*>LAYOUTER_CLASS_NAME<*>::<*>LAYOUTER_CLASS_NAME<*>()
{
    SetXMLTag("<*>LAYOUTER_CLASS_NAME<*>");
}

eC_Bool <*>LAYOUTER_CLASS_NAME<*>::IsDependentOnParentSize()
{
    return true;
}

void <*>LAYOUTER_CLASS_NAME<*>::DoLayout(eMovedEdges_t eMovedEdges)
{
}

#if defined(GUILIANI_STREAM_GUI)
void <*>LAYOUTER_CLASS_NAME<*>::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader( <*>LAYOUTER_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUILayouter::ReadFromStream();

    switch (cuiVersion)
    {
    case <*>LAYOUTER_CLASS_VERSION<*>:
        // ************************************************************
        // NOTE:    Insert custom attribute read calls here.
        // ************************************************************
        break;
    default:
        break;
    }
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void <*>LAYOUTER_CLASS_NAME<*>::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader( bWriteClassID, XMLTAG_LAYOUTERCLASSID, <*>LAYOUTER_CLASS_ID_NAME<*>, <*>LAYOUTER_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUILayouter::WriteToStream();

    // ************************************************************
    // NOTE:    Insert custom attribute write calls here.
    // ************************************************************

    WriteStreamingFooter( bWriteClassID );
}
#endif
