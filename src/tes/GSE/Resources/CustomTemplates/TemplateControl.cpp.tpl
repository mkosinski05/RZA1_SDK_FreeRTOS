#include "<*>CONTROL_CLASS_NAME<*>.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUIControlResource.h"

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define <*>CONTROL_CLASS_VERSION_NAME<*> <*>CONTROL_CLASS_VERSION<*>

<*>CONTROL_CLASS_NAME<*>::<*>CONTROL_CLASS_NAME<*>(
    CGUICompositeObject *const pParent,
    const eC_Value &vX, const eC_Value &vY,
    const eC_Value &vWidth, const eC_Value &vHeight,
    const ObjectHandle_t &eID)
    : CGUIObject(pParent, vX, vY, vWidth, vHeight, eID)
{
    SetXMLTag("<*>CONTROL_CLASS_NAME<*>");
}

<*>CONTROL_CLASS_NAME<*>::<*>CONTROL_CLASS_NAME<*>()
    : CGUIObject()
{
    SetXMLTag("<*>CONTROL_CLASS_NAME<*>");
}

#if defined(GUILIANI_STREAM_GUI)
void <*>CONTROL_CLASS_NAME<*>::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader( <*>CONTROL_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUIObject::ReadFromStream();

    switch (cuiVersion)
    {
    case <*>CONTROL_CLASS_VERSION<*>:
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
void <*>CONTROL_CLASS_NAME<*>::WriteToStream(const eC_Bool bWriteClassID)
{    
    WriteStreamingHeader( bWriteClassID, XMLTAG_CONTROLCLASSID, <*>CONTROL_CLASS_ID_NAME<*>, <*>CONTROL_CLASS_VERSION_NAME<*>);

    // always stream attributes of base-class first to retain order in attribute-view!
    CGUIObject::WriteToStream();

    // ************************************************************
    // NOTE:    Insert custom attribute write calls here.
    // ************************************************************

    WriteStreamingFooter( bWriteClassID );
}
#endif
