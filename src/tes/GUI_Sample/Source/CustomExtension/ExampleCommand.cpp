#include "ExampleCommand.h"
#include "GUI.h"
#include "GUIStreamReader.h"
#include "GUIStreamWriter.h"
#include "GUIStreamingException.h"
#include "GUICommandResource.h"

#include "GUIMemLeakWatcher.h" // <-- has to be the last include

#define EXAMPLE_COMMAND_CLASS_VERSION 1
// The minimal class version allowed.
#define EXAMPLE_COMMAND_CLASS_MIN_VERSION 1

ExampleCommand::ExampleCommand(ObjectHandle_t eTargetObject, eC_Value vStepSize) :
    m_eTargetObject(eTargetObject),
    m_vStepSize(vStepSize)
{
    SetXMLTag("ExampleCommand");
}

ExampleCommand::ExampleCommand() :
    m_eTargetObject(NO_HANDLE),
    m_vStepSize(eC_FromInt(0))
{
    SetXMLTag("ExampleCommand");
}

void ExampleCommand::Do()
{
    // when using GetObjectByID this define must be checked
#if defined(STREAMRUNTIME_APPLICATION)
    if (m_eTargetObject == NO_HANDLE || m_vStepSize == eC_FromInt(0))
    {
        return;
    }
    CGUIObject* pkObj = GETGUI.GetObjectByID(m_eTargetObject);
    if (pkObj == NULL)
    {
        return;
    }
    // Do not allow setting width or height less than zero.
    if (pkObj->GetWidth() + eC_Mul(m_vStepSize, eC_FromInt(2)) < eC_FromInt(0) ||
        pkObj->GetHeight() + eC_Mul(m_vStepSize, eC_FromInt(2)) < eC_FromInt(0))
    {
        return;
    }
    pkObj->InvalidateArea();
    CGUIRect kRect = pkObj->GetRelRect();
    kRect.Expand(m_vStepSize);
    pkObj->SetRelRect(kRect);
    pkObj->InvalidateArea();
#endif
}

#if defined(GUILIANI_STREAM_GUI)
void ExampleCommand::ReadFromStream()
{
    ReadStreamingHeader(EXAMPLE_COMMAND_CLASS_VERSION, EXAMPLE_COMMAND_CLASS_MIN_VERSION);

    m_eTargetObject = GETINPUTSTREAM.READ_OBJECTID("TargetObjectID");
    m_vStepSize = eC_FromFloat(GETINPUTSTREAM.READ_FLOAT("StepSize"));

    // base-class of CGUICommand is read at the end due to additional commands
    CGUICommand::ReadFromStream();
}
#endif

#if defined(GUILIANI_WRITE_GUI)
void ExampleCommand::WriteToStream(const eC_Bool bWriteClassID)
{
    WriteStreamingHeader(bWriteClassID, XMLTAG_COMMANDCLASSID, CMD_EXAMPLE, EXAMPLE_COMMAND_CLASS_VERSION);

    GETOUTPUTSTREAM.WriteObjectID(m_eTargetObject, "TargetObjectID");
    GETOUTPUTSTREAM.WriteFloat(eC_ToFloat(m_vStepSize), "StepSize");

    // base-class of CGUICommand is written at the end due to additional commands
    CGUICommand::WriteToStream();

    WriteStreamingFooter(bWriteClassID);
}
#endif
