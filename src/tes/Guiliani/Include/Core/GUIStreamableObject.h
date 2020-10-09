/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUISTREAMABLEOBJECT__H_
#define GUISTREAMABLEOBJECT__H_

#include "eC_Types.h"

#ifdef GUILIANI_WRITE_GUI
#include "eC_String.h"
#endif

/**
    If GUILIANI_WRITE_GUI is not defined, the method's header should not have an eC_String
    even if it's not used inside the method. It would still construct the string.
*/
#ifndef GUILIANI_WRITE_GUI
    #define SetXMLTag(a) SetXMLTag()
#endif

/// @defgroup GUILIANI_STREAMING Streaming
/// @brief Streaming allows the (re)construction of User Interfaces from a file (XML or Binary) and is the basis for Guiliani's Graphical Editor GSE

/// Base class for streamable objects.

/**
    This class declares a generic interface which is used for Guiliani's streaming
    mechanism (see @ref GUILIANI_STREAMING).
    Any object which is to be constructed via streaming has to be
    derived from this class. Derived classes implement the ReadFromStream and
    WriteToStream in such a way that they read/write all of the object's attributes
    from/to the data stream. When implementing the read/write methods take special
    care to the order in which the attributes are read/written, as this has to be
    identical in both methods.

    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamableObject
{
public:
    virtual ~CGUIStreamableObject() {}

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /** XML tag to be used when writing the class version into a stream.
      */
    static const eC_Char XMLTAG_CLASSVERSION[];
#endif

#if defined(GUILIANI_STREAM_GUI)
    /**
        Reads attributes from the streaming file. Only for use with
        GUIFactoryManager.
        @see CGUIObject::ReadFromStream()
    */
    virtual void ReadFromStream() {}
#endif

#if defined(GUILIANI_WRITE_GUI)
    /**
        Writes attributes to the streaming file. A CGUIStreamWriter
        has to be initialized.
        @param bWriteClassID This flag is used to switch writing of the class
               ID, leading and trailing tags. When implementing a new
               streamable object, check this flag. If it is true, first write
               the class ID, then continue with this object's attributes,
               and finally call the base class implementation with this flag
               set to false (this is the default).
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID = false) {}
#endif
    /**
        Sets an XML tag which is used by the WriteToStream() method as leading
        and trailing tag.
        @param kXMLTag XML tag
    */
    void SetXMLTag(const eC_String& kXMLTag)
    {
#if defined(GUILIANI_WRITE_GUI)
        m_kXMLTag = kXMLTag;
#endif
    }

#if defined(GUILIANI_WRITE_GUI)
    /**
        Returns the XML tag which is used by the WriteToStream() method as leading
        and trailing tag.
        @return The XML tag
    */
    const eC_String& GetXMLTag() const {return m_kXMLTag;}
#endif

protected:

#if defined(GUILIANI_STREAM_GUI)
    /** Reads the typical streaming header. This should always be the first thing to call within ReadFromStream()
        @throws CGUIStreamingException In case the read class version is larger than the version of the class for which the library was built.
        @throws CGUIStreamingException in case the read class version is lesser than the minimal version allowed.
        @param uiClassVersion The current version number of the class.
        @param uiClassMinVersion The minimal allowed version number of the class. If not set (== 0), it will not be evaluated.
        @return Class Version number as read from the stream
    */
    eC_UInt ReadStreamingHeader( const eC_UInt& uiClassVersion, const eC_UInt& uiClassMinVersion = 0) const;
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes the typical streaming header including XMLTag, ClassID and ClassVersion .
        This should always be the first thing to call within WriteToStream()
        @param bWriteClassID Hand over the parameter as received within WriteToStream()
        @param pkClassIDTag The ClassIDTag which shall encapsulate the ClassID value. (E.g. XMLTAG_CONTROLCLASSID for controls or XMLTAG_COMMANDCLASSID for commands)
        @param iClassID The unique Class ID which is used by the streaming factory to decide which object to instantiate
        @param uiClassVersion The version number of the class which shall be written to the stream
    */
    void WriteStreamingHeader( const eC_Bool& bWriteClassID, const eC_Char* const pkClassIDTag, const eC_Int& iClassID, const eC_UInt& uiClassVersion) const;

    /** Writes the typical streaming footer, including the closing XMLTag.
        This should always be the last thing to call within WriteToStream() 
        @param bWriteClassID Hand over the parameter as received within WriteToStream()
    */
    void WriteStreamingFooter( const eC_Bool& bWriteClassID) const;

private:
    eC_String   m_kXMLTag;
#endif
};

#endif
