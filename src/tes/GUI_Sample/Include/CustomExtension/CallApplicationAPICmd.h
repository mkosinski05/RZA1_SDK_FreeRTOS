#if !defined(CALLAPPLICATIONAPICMD_H)
#define CALLAPPLICATIONAPICMD_H

#include "GUICommand.h"

class CallApplicationAPICmd: public CGUICommand
{
public:
    /// Default constructor used by streaming factory
    CallApplicationAPICmd();
    /** Constructor.
        @param kAPI String which will be forwarded to CMyGUI::CallApplicationAPI to identify which application API to call
        @param kParam Optional parameter which will be handed over to CMyGUI::CallApplicationAPI as well */
    CallApplicationAPICmd( const eC_String& kAPI, const eC_String& kParam) : m_kAPI(kAPI), m_kParam( kParam) {};

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
        @param bWriteClassID This flag is used to select if writing of command
               class ID, leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Implements the actual command functionality.
    void Do();

    eC_String   m_kAPI;
    eC_String   m_kParam;
};

#endif // CALLAPPLICATIONAPICMD_H
