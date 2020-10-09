#ifndef __WAVELOADER_H__
#define __WAVELOADER_H__

#include "eC_Types.h"
#include "GUITrace.h"

// some magic bytes within wavefiles
#define WAVE_FILETYPE_RIFF "RIFF"
#define WAVE_DATATYPE_WAVE "WAVE"
#define WAVE_CHUNKID_FMT "fmt "
#define WAVE_CHUNKID_DATA "data"

// namespace for fileloaders
namespace FileLoader
{
    /**
    This class is internally used within sound wrappers to load wave-files
    */
    class WaveLoader
    {
    public:
        /// Structure of format chunk of wave-file
        struct FormatChunk_t
        {
            eC_Short    sFormatTag;         ///< Format of data (1 is non-compressed data)
            eC_UShort   usChannels;         ///< Number of channels
            eC_UInt     uiSamplesPerSec;    ///< Samplerate of file
            eC_UInt     uiAvgBytesPerSec;   ///< Number of bytes per second
            eC_UShort   usBlockAlign;       ///< Size of sample in bytes
            eC_UShort   usBitsPerSample;    ///< Number of bits per sample
        };

        /// Structure of data chunk of wave-file
        struct DataChunk_t
        {
            eC_UInt uiDataSize; ///< Size of sound data
            eC_UByte *pubData;  ///< Allocated memory block for sound data
        };

    private:
        /// structure for header of wavefile
        struct WaveHeader_t
        {
            char    ubIdRiff[4];    ///< Always RIFF (Resource Interchange File Format)
            eC_UInt uiFileSize;     ///< Size of complete file
            char    ubIdWave[4];    ///< Always WAVE for wavefiles
        };

        /// structure of a chunk
        struct Chunk_t
        {
            char    ubChunkID[4];   ///< Four bytes for identification of chunk
            eC_UInt uiChunkSize;    ///< Size of chunk in bytes
        };

    public:
        /// Default constructor
        WaveLoader() { memset(&m_kWaveFormat, 0, sizeof(m_kWaveFormat)); memset(&m_kWaveData, 0, sizeof(m_kWaveData)); }

        /// Default destructor
        ~WaveLoader() {}

        /**
        Loads a wavefile and prepares the sound data for further use
        @param pkPath String with path to file to load
        @return true, if file was successfully loaded, otherwise false
        */
        eC_Bool Load(const eC_String &pkPath);

        /**
        Getter for internal format of loaded wave-file
        @return Format of loaded wave-file
        */
        FormatChunk_t GetWaveFormat() const { return m_kWaveFormat; }

        /**
        Getter for internal sound buffer
        @return Data of loaded wave-file
        */
        DataChunk_t GetWaveData() const { return m_kWaveData; }

        /**
        Unload currently loaded wave-file and clear memory buffer
        */
        void Unload()
        {
            if (m_kWaveData.pubData)
            {
                delete[] m_kWaveData.pubData;
                m_kWaveData.pubData = NULL;
            }
        }

    private:
        /// Buffer-size for reading the file
        enum { BUFFER_SIZE = 1024 };

    private:
        FormatChunk_t m_kWaveFormat;    ///< Format of currently loaded wave-file
        DataChunk_t m_kWaveData;        ///< Data of currently loaded wave-file
    };
}

#endif
