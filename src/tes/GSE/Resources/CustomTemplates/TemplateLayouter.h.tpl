#if !defined(<*>HEADER_DEFINE<*>)
#define <*>HEADER_DEFINE<*>

#include "GUILayouter.h"

class <*>LAYOUTER_CLASS_NAME<*> : public CGUILayouter
{
public:
    <*>LAYOUTER_CLASS_NAME<*>();

    virtual eC_Bool IsDependentOnParentSize();

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
        @param bWriteClassID This flag is used to select if writing of layouter
               class ID, leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Implements the layouter functionality.
    void DoLayout(eMovedEdges_t eMovedEdges);
};

#endif // <*>HEADER_DEFINE<*>
