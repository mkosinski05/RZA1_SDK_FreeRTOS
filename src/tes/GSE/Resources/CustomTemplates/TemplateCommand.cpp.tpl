#include "<*>COMMAND_CLASS_NAME<*>.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUICommandResource.h"

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define <*>COMMAND_CLASS_VERSION_NAME<*> <*>COMMAND_CLASS_VERSION<*>

<*>COMMAND_CLASS_NAME<*>::<*>COMMAND_CLASS_NAME<*>()
{
    SetXMLTag("<*>COMMAND_CLASS_NAME<*>");
}

void <*>COMMAND_CLASS_NAME<*>::Do()
{
#if defined(STREAMRUNTIME_APPLICATION)
    // ************************************************************
    // NOTE:    Implement your specific code here (between  
    //          the defined 'STREAMRUNTIME_APPLICATION'!)
    // ************************************************************
#endif
}

#if defined(GUILIANI_STREAM_GUI)
void <*>COMMAND_CLASS_NAME<*>::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader( <*>COMMAND_CLASS_VERSION_NAME<*>);

    switch (cuiVersion)
    {
    case <*>COMMAND_CLASS_VERSION<*>:
        // ************************************************************
        // NOTE:    Insert custom attribute read calls here.
        // ************************************************************
        break;
    default:
        break;
    }
    
    // base-class of CGUICommand is read at the end due to additional commands
    CGUICommand::ReadFromStream();
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void <*>COMMAND_CLASS_NAME<*>::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader( bWriteClassID, XMLTAG_COMMANDCLASSID, <*>COMMAND_CLASS_ID_NAME<*>, <*>COMMAND_CLASS_VERSION_NAME<*>);

    // ************************************************************
    // NOTE:    Insert custom attribute write calls here.
    // ************************************************************    

    // base-class of CGUICommand is written at the end due to additional commands
    CGUICommand::WriteToStream();

    WriteStreamingFooter( bWriteClassID );
}
#endif
