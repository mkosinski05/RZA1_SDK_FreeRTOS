#ifndef GUI_STREAM_TYPES_H_
#define GUI_STREAM_TYPES_H_
/// File type for Streams
enum FileType_t
{
    FILE_TYPE_XML = 0,    ///< XML
    FILE_TYPE_BINARY = 1, ///< Binary
    FILE_TYPE_CUSTOM = 2  ///< User defined custom type
};

/// File endianess for streams
enum FileEndianess_t
{
    FILE_ENDIANESS_BIG = 0,    ///< big endian
    FILE_ENDIANESS_LITTLE = 1, ///< little endian
    FILE_ENDIANESS_INVALID = 255   ///< Invalid/not applicable. E.g. for XML
};

/// File alignment for Streams
enum FileAlignment_t
{
    FILE_ALIGNMENT_8BIT = 0,  ///< byte aligned
    FILE_ALIGNMENT_16BIT = 1, ///< 16-bit aligned
    FILE_ALIGNMENT_32BIT = 2, ///< 32-bit aligned
    FILE_ALIGNMENT_64BIT = 3, ///< 64-bit aligned
    FILE_ALIGNMENT_INVALID = 255  ///< Invalid/not applicable. E.g. for XML
};

// File structure is 0xEEAAVVVV (E = Endianess, A = Alignment, V= Version
const eC_UInt cSHIFT_ENDIANESS = 24; ///< shift value for endianess in binary streaming header
const eC_UInt cSHIFT_ALIGNMENT = 16; ///< shift value for alignment in binary streaming header
const eC_UInt cSHIFT_VERSION = 0;    ///< shift value for version in binary streaming header

const eC_UInt cMASK_ENDIANESS = (0x000000FFu) << cSHIFT_ENDIANESS; ///< mask for endianess in binary streaming header
const eC_UInt cMASK_ALIGNMENT = (0x000000FFu) << cSHIFT_ALIGNMENT; ///< mask for alignment in binary streaming header
const eC_UInt cMASK_VERSION = (0x0000FFFFu) << cSHIFT_VERSION;     ///< mask for version in binary streaming header
#endif // GUI_STREAM_TYPES_H_
