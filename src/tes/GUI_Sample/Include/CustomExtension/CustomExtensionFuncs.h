/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

/** @brief Contains declarations for functions that are called during start-up
           for obtaining custom extensions.
  */
#if !defined(CUSTOM_EXTENSION_FUNCS_H)
#define CUSTOM_EXTENSION_FUNCS_H

#include <vector>
#include "GUIBehaviourResource.h"
#include "GUICommandResource.h"
#include "GUIControlResource.h"
#include "GUILayouterResource.h"
#include "GUICommand.h"

class CGUIFactory;
class CGUIBehaviourDecorator;
class CGUIObject;
class CGUILayouter;

/// Describes a custom behaviour decorator.
struct BehaviourDescriptor
{
    /** Constructs a behaviour descriptor.
        @param eID The unique class ID of the behaviour.
        @param rkClassIDName The behaviour class ID as a string.
        @param pkBehaviour Pointer to an instance of the custom behaviour.
      */
    BehaviourDescriptor(
        BehaviourClassID_t eID,
        const eC_String& rkClassIDName,
        CGUIBehaviourDecorator* pkBehaviour)
        : m_eID(eID), m_kClassIDName(rkClassIDName), m_pkBehaviour(pkBehaviour)
    {}

    /// The behaviour class ID.
    BehaviourClassID_t m_eID;

    /// The behaviour class ID as a string.
    eC_String m_kClassIDName;

    /// Pointer to an instance of the behaviour.
    CGUIBehaviourDecorator* m_pkBehaviour;
};

/// Describes a custom command.
struct CommandDescriptor
{
    /** Constructs a command descriptor.
        @param eID The unique class ID of the command.
        @param rkClassIDName The command class ID as a string.
        @param pkCommand Pointer to an instance of the custom command.
      */
    CommandDescriptor(
        CommandClassID_t eID,
        const eC_String& rkClassIDName,
        CGUICommandPtr pkCommand)
        : m_eID(eID), m_kClassIDName(rkClassIDName), m_pkCommand(pkCommand)
    {}

    /// The command class ID.
    CommandClassID_t m_eID;

    /// The command class ID as a string.
    eC_String m_kClassIDName;

    /// Pointer to an instance of the command.
    CGUICommandPtr m_pkCommand;
};

/// Describes a custom control.
struct ControlDescriptor
{
    /** Constructs a control descriptor.
        @param eID The unique class ID of the control.
        @param rkControlName Human-readable name of the control. For instance,
               for the control class CGUIButton, this could simply be 'button'.
        @param pkControl Pointer to an instance of the custom control.
      */
    ControlDescriptor(
        ControlClassID_t eID,
        const eC_String& rkControlName,
        CGUIObject* pkControl)
        : m_eID(eID), m_kControlName(rkControlName), m_pkControl(pkControl)
    {}

    /// The control class ID.
    ControlClassID_t m_eID;

    /// Human-readable name of the control.
    eC_String m_kControlName;

    /// Pointer to an instance of the control.
    CGUIObject* m_pkControl;
};

/// Describes a custom layouter.
struct LayouterDescriptor
{
    /** Constructs a layouter descriptor.
        @param eID The unique class ID of the layouter.
        @param rkClassIDName The layouter class ID as a string.
        @param pkLayouter Pointer to an instance of the custom layouter.
      */
    LayouterDescriptor(
        LayouterClassID_t eID,
        const eC_String& rkClassIDName,
        CGUILayouter* pkLayouter)
        : m_eID(eID), m_kClassIDName(rkClassIDName), m_pkLayouter(pkLayouter)
    {}

    /// The layouter class ID.
    LayouterClassID_t m_eID;

    /// The layouter class ID as a string.
    eC_String m_kClassIDName;

    /// Pointer to an instance of the layouter.
    CGUILayouter* m_pkLayouter;
};

/** Describes an enumeration value so that it can be mapped to readable text in
    the editor's GUI. */
struct EnumMapping
{
    /** Constructs an enumeration mapping descriptor.
        @param rkTypeTag The XML tag used when reading from or writing the
               enumeration value to a stream. In other words, this must be the
               same string that is used in the respective control's/command's/
               behaviour's/layouter's WriteToStream and ReadFromStream
               implementations.
        @param iEnumValue The enumeration value.
        @param rkValueName The string representation to be used for iEnumValue.
      */
    EnumMapping(
        const eC_String& rkTypeTag,
        eC_Int iEnumValue,
        const eC_String& rkValueName)
        : m_kTypeTag(rkTypeTag), m_iEnumValue(iEnumValue), m_kValueName(rkValueName)
    {}

    /// The XML tag used when reading from or writing the enumeration value to a stream.
    eC_String m_kTypeTag;

    /// The enumeration value.
    eC_Int m_iEnumValue;

    /// The enumeration value as a string.
    eC_String m_kValueName;
};

/// Describes a custom attribute repetition.
struct AttributeRepetitionDescriptor
{
    /// The possible types of attributes.
    enum AttributeType // Copy from class ObjectAttribute::AttributeType
    {
        AT_INT,     ///< Integer value.
        AT_HEX,     ///< Integer value, formatted in hex.
        AT_SHORT,   ///< Short integer value.
        AT_FLOAT,   ///< Float value.
        AT_BOOL,    ///< Boolean value.
        AT_STRING,  ///< String.
        AT_OBJECTID,///< Object handle (ID).
        AT_IMAGEID, ///< Image ID.
        AT_TEXTID,  ///< Text ID.
        AT_FONTID,  ///< Font ID.
        AT_SOUNDID, ///< Sound ID.
        AT_GENRESID, ///< General resource ID.
        AT_DATAPOOLID, ///< DataPool resource ID.
        AT_COMMENT, ///< Comment (for XML comment tags without values).
    };

    /** Constructs a repetition descriptor.
        @param rkRepetitionTitle The title of the repetition.
        @param rkXMLTag The XML tag of the repetition.
        @param rkAttributeValue The attribute value of the repetition as string.
        @param kAttributeType The attribute type of the repetition as enum.
      */
    AttributeRepetitionDescriptor(
        const eC_String &rkRepetitionTitle,
        const eC_String &rkXMLTag,
        const eC_String &rkAttributeValue,
        AttributeRepetitionDescriptor::AttributeType kAttributeType)
        : m_kRepetitionTitle(rkRepetitionTitle), m_kXMLTag(rkXMLTag), m_kAttributeValue(rkAttributeValue), m_kAttributeType(kAttributeType)
    {}

    /// The repetition title as a string (The attribute name of the tag containing the number of repetitions).
    /// Marks the end of a repetition lists (XMLTag and AttributeValue have to be empty strings).
    eC_String m_kRepetitionTitle;

    /// The repetition XML tag as a string.
    eC_String m_kXMLTag;

    /// The repetition attribute value as a string.
    eC_String m_kAttributeValue;

    /// The repetition attribute type enum.
    AttributeType m_kAttributeType;
};

typedef std::vector<CGUIFactory*>                   FactoryVector;
typedef std::vector<BehaviourDescriptor>            BehaviourVector;
typedef std::vector<CommandDescriptor>              CommandVector;
typedef std::vector<ControlDescriptor>              ControlVector;
typedef std::vector<LayouterDescriptor>             LayouterVector;
typedef std::vector<EnumMapping>                    EnumMappingVector;
typedef std::vector<AttributeRepetitionDescriptor>  RepetitionVector;

/** Called on start-up for obtaining custom factories.
    @param rkFactories Receives the custom factories.
  */
void GetCustomFactories(FactoryVector& rkFactories);

/** Called on start-up for obtaining custom behaviours.
    @param rkBehaviours Receives the custom behaviours.
  */
void GetCustomBehaviours(BehaviourVector& rkBehaviours);

/** Called on start-up for obtaining custom commands.
    @param rkCommands Receives the custom commands.
  */
void GetCustomCommands(CommandVector& rkCommands);

/** Called on start-up for obtaining custom controls.
    @param rkControls Receives the custom controls.
  */
void GetCustomControls(ControlVector& rkControls);

/** Called on start-up for obtaining custom layouters.
    @param rkLayouters Receives the custom layouters.
  */
void GetCustomLayouters(LayouterVector& rkLayouters);

/** Called on start-up to allow adding of custom enum mappings.
    @param rkEnumMappings Receives the custom enum mappings.
  */
void GetCustomEnumMappings(EnumMappingVector& rkEnumMappings);

/** Called on start-up to allow adding of custom attribute repetition mappings.
    @param rkRepetitions Receives the custom repetition mappings.
  */
void GetCustomRepetitions(RepetitionVector& rkRepetitions);

#endif // CUSTOM_EXTENSION_FUNCS_H
