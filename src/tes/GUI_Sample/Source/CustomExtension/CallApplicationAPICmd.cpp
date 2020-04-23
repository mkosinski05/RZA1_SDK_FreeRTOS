#include "CallApplicationAPICmd.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUICommandResource.h"
#if defined(STREAMRUNTIME_APPLICATION)
#include "MyGUI_SR.h"
#endif

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define CALLAPPLICATIONAPICMD_VERSION 1

CallApplicationAPICmd::CallApplicationAPICmd()
{
    SetXMLTag("CallApplicationAPICmd");
}

void CallApplicationAPICmd::Do()
{
#if defined(STREAMRUNTIME_APPLICATION)
    // This the CallApplicationAPI method in MyGUI.
    // Feel free to extend CallApplicationAPI as required.
    GETMYGUI.CallApplicationAPI(m_kAPI, m_kParam);
#endif
}

#if defined(GUILIANI_STREAM_GUI)
void CallApplicationAPICmd::ReadFromStream()
{
    const eC_UInt cuiVersion = ReadStreamingHeader(CALLAPPLICATIONAPICMD_VERSION);

    if (cuiVersion <= 1)
    {
        m_kAPI = GETINPUTSTREAM.READ_STRING("ApplicationAPI");
        m_kParam = GETINPUTSTREAM.READ_STRING("Parameter");
        CGUICommand::ReadFromStream();
    }
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void CallApplicationAPICmd::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader(bWriteClassID, XMLTAG_COMMANDCLASSID, CMD_CALLAPPLICATIONAPI, CALLAPPLICATIONAPICMD_VERSION);

    GETOUTPUTSTREAM.WriteString(m_kAPI, "ApplicationAPI");
    GETOUTPUTSTREAM.WriteString(m_kParam, "Parameter");

    // base-class is read after attributes due to additional commands
    CGUICommand::WriteToStream();

    WriteStreamingFooter(bWriteClassID);
}
#endif
