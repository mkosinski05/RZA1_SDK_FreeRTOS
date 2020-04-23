/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(EXAMPLE_LAYOUTER_H)
#define EXAMPLE_LAYOUTER_H

#include "GUILayouter.h"

/** An example layouter that keeps its object centered in the object's parent
    horizontally, vertically or both.
  */
class ExampleLayouter: public CGUILayouter
{
public:
    /** Constructs an ExampleLayouter.
        @param bHorizontal Whether the object is centered horizontally in its parent.
        @param bVertical Whether the object is centered vertically in its parent.
        @param pkObject Pointer to object to be associated with this layouter.
      */
    ExampleLayouter(eC_Bool bHorizontal, eC_Bool bVertical, CGUIObject* const pkObject=NULL);

    /// Default constructor to be used by a factory.
    ExampleLayouter()
    {
        SetXMLTag("ExampleLayouter");
    }

    virtual eC_Bool IsDependentOnParentSize() {return true;}

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
        @param bWriteClassID This flag is used to select whether writing of layouter ID,
               leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    void DoLayout(eMovedEdges_t eMovedEdges);

    /// Whether the object is centered horizontally.
    eC_Bool m_bHorizontal;

    /// Whether the object is centered vertically.
    eC_Bool m_bVertical;
};

#endif // EXAMPLE_LAYOUTER_H
