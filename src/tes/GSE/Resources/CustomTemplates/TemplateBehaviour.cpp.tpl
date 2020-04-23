#include "<*>BEHAVIOUR_CLASS_NAME<*>.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUIBehaviourResource.h"

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define <*>BEHAVIOUR_CLASS_VERSION_NAME<*> <*>BEHAVIOUR_CLASS_VERSION<*>

<*>BEHAVIOUR_CLASS_NAME<*>::<*>BEHAVIOUR_CLASS_NAME<*>(CGUIObject* const pObject)
    : CGUIBehaviourDecorator(pObject)
{
    SetXMLTag("<*>BEHAVIOUR_CLASS_NAME<*>");
}

<*>BEHAVIOUR_CLASS_NAME<*>::<*>BEHAVIOUR_CLASS_NAME<*>()
    : CGUIBehaviourDecorator()
{
    SetXMLTag("<*>BEHAVIOUR_CLASS_NAME<*>");
}

#if defined(GUILIANI_STREAM_GUI)
void <*>BEHAVIOUR_CLASS_NAME<*>::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader( <*>BEHAVIOUR_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUIBehaviourDecorator::ReadFromStream();

    switch (cuiVersion)
    {
    case <*>BEHAVIOUR_CLASS_VERSION<*>:
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
void <*>BEHAVIOUR_CLASS_NAME<*>::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader( bWriteClassID, XMLTAG_BEHAVIOURCLASSID, <*>BEHAVIOUR_CLASS_ID_NAME<*>, <*>BEHAVIOUR_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUIBehaviourDecorator::WriteToStream();

    // ************************************************************
    // NOTE:    Insert custom attribute write calls here.
    // ************************************************************

    WriteStreamingFooter( bWriteClassID );
}
#endif
