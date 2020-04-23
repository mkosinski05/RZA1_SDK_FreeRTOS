/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIPROPERTIES__H_
#define GUIPROPERTIES__H_

#include "eC_Types.h"
#include "eC_String.h"
#include "GUISubject.h"
#include "GUIPropertyResource.h"

/// Retrieve pointer to property handler instance
#define GETPROPHDL            CGUIProperties::GetInstance()

/**
    It is possible to access the property values by using the Get - methods, or by registering an observer for a specific property.
    @brief This class allows for the global customisation of controls without the need to derive specialised objects.
*/
class CGUIProperties
{
    friend class CGUI;
public:
    /// return pointer to Global property instance
    static inline CGUIProperties& GetInstance() { return ms_kGlobalPropHdl; }

    /// Should only be called by the destructor. As a workaround for memory leak
    /// detection ~CGUI() may call it too, but it is forbidden to be called by
    /// other code.
    void DeInit();

#ifdef GUILIANI_STREAM_GUI
    /**
    Reads all Properties from file. Streamingfile Reader has to initialized.
    @param kPath Path to the SourceFile.
    @see CGUIStreamReader
    */
    void ReadGlobalPropertiesFromFile(const eC_String& kPath);
#endif

#ifdef GUILIANI_WRITE_GUI
     /**
    Writes all Properties to file. Streamingfile Writer has to initialized.
    @param kPath Path to the DestinationFile.
    @see CGUIStreamWriter
    */
    void WriteGlobalPropertiesToFile(const eC_String& kPath) const;
#endif

    /**
        Set an integer property.
        @param ePropertyID The unique id of the property.
        @param iPropertyValue The property value to set for the property id.
        @see GetGlobalIntegerProperty()
    */
    void SetGlobalIntegerProperty(const GlobalProperty_t ePropertyID, const eC_Int iPropertyValue);

        /**
        Set a color property.
        @param ePropertyID The unique id of the property.
        @param uiPropertyValue The property value to set for the property id as ARGB (32 Bit) value.
        @see GetGlobalColorProperty()
    */
    void SetGlobalColorProperty(const GlobalProperty_t ePropertyID, const eC_UInt uiPropertyValue);

    /**
        Set a float property.
        @param ePropertyID The unique id of the property.
        @param fPropertyValue The property value to set for the property id.
        @see GetGlobalFloatProperty()
    */
    void SetGlobalFloatProperty(const GlobalProperty_t ePropertyID, const eC_Float fPropertyValue);

    /**
        Set a text property.
        @param ePropertyID The unique id of the property.
        @param rkStr The property value to set for the property id.
        @see GetGlobalTextProperty()
    */
    void SetGlobalTextProperty(const GlobalProperty_t ePropertyID, const eC_String& rkStr);

    /**
        Set a boolean property.
        @param ePropertyID The unique id of the property.
        @param bPropertyValue The property value to set for the property id.
        @see GetGlobalBoolProperty()
    */
    void SetGlobalBoolProperty(const GlobalProperty_t ePropertyID, const eC_Bool bPropertyValue);

    /**
        Set a GUIValue property.
        @param ePropertyID The unique id of the property.
        @param kPropertyValue The property value to set for the property id.
        @param eType The property type to set for the property id.
        @see GetGlobalProperty()
    */
    void SetGlobalProperty(const GlobalProperty_t ePropertyID, const CGUIValue& kPropertyValue, const CGUIValue::GUIValue_t& eType);

    /**
        Set a GUIValue property.
        @param ePropertyID The unique id of the property.
        @param kPropertyValue The property value to set for the property id.
        @see GetGlobalProperty()
    */
    void SetGlobalProperty(const GlobalProperty_t ePropertyID, const CGUIValue& kPropertyValue);

    /**
        Get the value of an integer property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    eC_Int   GetGlobalIntegerProperty(const GlobalProperty_t ePropertyID) const;

    /**
        Get the value of a color property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    eC_UInt   GetGlobalColorProperty(const GlobalProperty_t ePropertyID) const;

    /**
        Get the value of a float property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    eC_Float GetGlobalFloatProperty(const GlobalProperty_t ePropertyID) const;

    /**
        Get the value of a text property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    const eC_String GetGlobalTextProperty(const GlobalProperty_t ePropertyID) const;

    /**
        Get the value of a boolean property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    eC_Bool GetGlobalBoolProperty(const GlobalProperty_t ePropertyID) const;

    /**
        Get the value of a property.
        @param ePropertyID The property id of the wanted property.
        @return The value stored for the property.
    */
    const CGUIValue& GetGlobalProperty(const GlobalProperty_t ePropertyID) const;

    /** Adds an observer to a property.
        The observer will be notified by calling CGUIObserver::OnNotification()
        whenever the property gets updated.
        @param ePropertyID The property id which is observed.
        @param pObserver The observer to add.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddObserver(const GlobalProperty_t ePropertyID,  CGUIObserver *pObserver );

    /** Removes an registered observer from a property.
        @param ePropertyID The property id which shall no longer be observed.
        @param pObserver The observer to remove.
    */
    void RemoveObserver (const GlobalProperty_t ePropertyID, CGUIObserver *pObserver);

    /**
        Creates a dynamic property id, and sets its value.
        @param kPropertyValue The value that shall be set.
        @return The dynamically created property id.
    */
    GlobalProperty_t AddDynamicProperty(const CGUIValue& kPropertyValue);

    /**
        Removes a dynamically created property.
        @param eID The id of the property to remove.
    */
    void RemoveDynamicProperty(const GlobalProperty_t eID);

    /// Returns the number of properties.
    inline eC_UInt GetNOFProperties() const {return m_uiGUIPropertyTableSize;}



#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    /// Destructor
    ~CGUIProperties();

private:

    /// Data type for global properties.
    typedef struct
    {
        CGUIValue           kPropertyValue;     ///< The property value. GUIValue also holds the property type.
        CGUISubject*        pkPropertySubject;   ///< The subject managing the observers of this property.
    } GUI_GlobalProperties_t;

    /// Constructor.
    CGUIProperties();

    /** Initializes the property handler.
    */
    void Init();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIProperties(const CGUIProperties& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIProperties& operator=(const CGUIProperties& kSource);

    /// static protected member pointer to Properties-Handler object
    static CGUIProperties        ms_kGlobalPropHdl;

    /// The property table stores all global properties.
    GUI_GlobalProperties_t *m_pGUIPropertyTable;
    /// Current size of m_pGUIPropertyTable.
    eC_UInt m_uiGUIPropertyTableSize;
};

#endif
