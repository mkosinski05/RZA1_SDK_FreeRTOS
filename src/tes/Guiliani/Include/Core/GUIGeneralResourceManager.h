/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUIGENERALRESOURCEMANAGER_H__)
#define GUIGENERALRESOURCEMANAGER_H__

#include "eC_Types.h"
#include "eC_File.h"
#include "eC_String.h"
#include "GUIRefCntPtr.h"
#include "GUIGeneralResource.h"

/**
* The CGUIGeneralResourceInterpreter interface should be implemented by an
* application developer. It is used to apply a file content interpreter to
* an abstract resource managed by CGUIGeneralResourceManager.
*
* As an example we assume that we have an abstract resource file containing
* PDF data. Then one can create a PDF content interpreter in an application
* and implement this interface.
*
* Then the resource needs to be requested via CGUIResourceManager and
* afterwards the interpreter can be applied. At last the resource should be
* released using Free operation:
*
* \code
*  GETRESMANAGER.RequestGeneralResource(ID_OF_TEST_PDF_FILE);
*  GETGENERALRESMANAGER.ApplyInterpreter(ID_OF_TEST_PDF_FILE, myInterpreter);
*  GETRESMANAGER.FreeGeneralResource(ID_OF_TEST_PDF_FILE);
* \endcode
* @brief Interpreter for general resources
*/
class CGUIGeneralResourceInterpreter
{
public:
    virtual ~CGUIGeneralResourceInterpreter() {}

    /**
    * Start the interpreter to parse given file pointer. The file pointer
    * is provided just in Read-only mode!
    *
    * After Interpret() has finished, pkFile pointer will be destroyed
    * automatically.
    *
    * Take care of not to destroy pkFile.
    *
    * @param pkFile Valid pointer to file containing data to be parsed.
    * @throw CGUIException In case of an error parsing the data file.
    */
    virtual void Interpret(eC_File* pkFile) = 0;
};

/** Macro returning instance of general resource manager. */
#define GETGENERALRESMANAGER CGUIGeneralResourceManager::GetInstance()

/**
* All abstract resources that are
* not handled by specific resource managers and needs to be tracked by GUI
* projects.
*
* Before parsing specific resources, it is necesary to request the
* resource ID via GETRESMANAGER.RequestGeneralResource() to force this object
* to load the resource data. ApplyInterpreter can then be used to apply a
* specific resource parser to loaded data. Afterwards,
* GETRESMANAGER.FreeGeneralResource() destroys the resource's data buffer
* and should be invoked, too.
* @brief Singleton of general resource file manager
*/
class CGUIGeneralResourceManager
{
    friend class CGUIResourceManager;
public:
    /// Deletes the class and clears the internal pointer
    static void DeleteInstance();

    /** Static methot to get a reference to the instance
        @return Reference to the instance
    */
    static CGUIGeneralResourceManager& GetInstance();

    /**
    * Use given resource interpreter to apply data associated to resource ID.
    * Data needs to be loaded before calling this method, use
    * GETRESMANAGER.RequestGeneralResource() to do this. Do not forget to
    * release the resource, if you don't need it anymore.
    *
    * @param eResourceID Requested resource ID contains data to be interpreted.
    * @param rkInterpreter Interpreter used to parse data of given resource ID.
    * @throw CGUIException In case of uninitialized data, resource ID out of
    *  range or the interpreter throws an error.
    */
    void ApplyInterpreter(GeneralResource_t eResourceID,
        CGUIGeneralResourceInterpreter& rkInterpreter);

    /**
    * Get number of known general resources.
    *
    * @return Count of general resources.
    */
    eC_UInt GetNOFResources() const;

private:
    /**
    * Set number of resources this GeneralResourceManager knows.
    *
    * @param uiNOFResources Number of resources this manager should contain.
    * @return True, if resources can be allocated, otherwise False.
    */
    eC_Bool SetNOFResources(eC_UInt uiNOFResources);

    /**
    * First load content of the file given with rkPath to buffer and
    * instruct current active interpreter to parse this data. Buffer
    * is assigned to given resource ID then.
    *
    * Call UnloadResource when the resource data is not needed anymore.
    *
    * @param rkPath Points to file the content of is to be loaded.
    * @param eID General resource ID the content will be mapped to.
    * @throw CGUIException When file can neither be loaded nor interpreted.
    */
    void LoadResource(const eC_String& rkPath,
        GeneralResource_t eID);

    /**
    * Call this to destroy buffer mapped with given general resource ID.
    *
    * @param eID General resource ID the associated content will be removed.
    */
    void UnloadResource(GeneralResource_t eID);

    /**
     * Keep constructor private because of singleton instance.
     */
    CGUIGeneralResourceManager();

    /** Static private member pointer to instance of CGUIGeneralResourceManager.*/
    static CGUIGeneralResourceManager* ms_pGenResMgr;

    /**
    * This nested class is used to store resource file content and provide
    * it as kind of eC_MemoryFile. On destruction of this object, containing
    * data will will be destroyed, too.
    */
    class CGUIGeneralResourceData
    {
    public:
        /** Ref counted pointer to eC_File. */
        typedef NSmartPtr::CGUIRefCntPtr<eC_File> PeC_File;

        /**
        * Constructor takes file given by rkPath, opens it and reads containing
        * data to a memory buffer.
        *
        * @throw CGUIException File could not be loaded to this buffer.
        */
        CGUIGeneralResourceData(const eC_String& rkPath);

        /**
        * Destroy buffer.
        */
        ~CGUIGeneralResourceData();

        /** Get memory file pointer.*/
        PeC_File GetFile();

    private:
        /**
        * Copy-constructor.
        * Dummy declaration with no implementation, just to hide the function.
        * @param kSource Source object to be copied.
        */
        CGUIGeneralResourceData(const CGUIGeneralResourceData& kSource);

    private:
        /** General resource data. */
        eC_UByte* m_pData;
        /** Size of data array. */
        eC_UInt m_uiDataSize;
    };

    /** Ref counted pointer type to CGUIGeneralResourceData. */
    typedef NSmartPtr::CGUIRefCntPtr<CGUIGeneralResourceData> PGUIGeneralResourceData;

    /** Contains data mapped to general resource IDs. */
    eC_TArray<PGUIGeneralResourceData> m_apResources;
};
#endif // GUIGENERALRESOURCEMANAGER_H__
