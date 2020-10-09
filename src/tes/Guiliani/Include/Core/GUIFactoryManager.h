/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if defined( GUILIANI_STREAM_GUI )

#if !defined( GUIFACTORY_MANAGER_H )
#define GUIFACTORY_MANAGER_H

#include "GUIStandardFactory.h"
#include "GUICommand.h"

/// Retrieve pointer to factory manager instance.
#define GETFACTORY CGUIFactoryManager::GetInstance()

/// Manages a CGUIStandardFactory and user factories.
/** When reading GUI definitions from streaming files, an instance of this
    class has to be created (CreateInstance()). It can then be used for reading
    streaming files, usually by calling LoadDialogFromFile().

    This class uses implementations of CGUIFactory for creating the objects as
    required. It keeps an object of CGUIStandardFactory for creating standard
    Guiliani objects. If user-defined objects (like derived controls, commands
    etc.) have to be created, appropriate user factories must be implemented in
    the application. They can be added here by calling AddUserFactory().

    @ingroup GUILIANI_STREAMING
  */
class CGUIFactoryManager
{
public:
    /// Returns the singleton instance of this class.
    static CGUIFactoryManager& GetInstance() { return *ms_pInstance; }

    /// Creates the singleton instance of this class.
    static void CreateInstance();

    /// Deletes the singleton instance of this class.
    static void DeleteInstance();

    /** Adds a user factory. All user factories are deleted when the factory
        manager is destroyed.
        @param pkFactory Pointer to the factory to be added.
      */
    inline void AddUserFactory(CGUIFactoryPtr pkFactory) { m_kFactories.AddAtEnd(pkFactory); }

    /** Reads a GUI definition from streaming file.
        This method uses the CGUIResourceFileHandler to open the file. This
        means that the file name is prefixed with the resource path prefix.
        @param rkPath path to the streaming file.
        @return Pointer to the created GUIObject, NULL in case of error.
    */
    CGUIObject* LoadDialogFromFile(const eC_String& rkPath);

    /** Reads a GUI definition from streaming file.
        Use this method only if you really need to load files that are located
        outside of the path that is used as the resource prefix (see
        CGUIResourceFileHandler). Normally you should use
        LoadDialogFromFile(const eC_String&).
        @param pkFile Pointer to an already opened file. This method does not
               close or delete the file. It is up to the caller of this method
               to clean up.
        @return Pointer to the created GUIObject.
    */
    CGUIObject* LoadDialogFromFile(eC_File* pkFile);

    /** Loads a CGUIText from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the text type ID.
        Calls CGUIFactory::CreateText() to create a text object, then calls
        ReadFromStream on the new object.
        @return Pointer to the created and initialized CGUIText object.
    */
    CGUIText* LoadTextFromStream();

    /** Loads a CGUICommand from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the command class
        ID. Calls CGUIFactory::CreateCommand() to create a command object, then calls
        ReadFromStream on the new object.
        @return Pointer to the created and initialized CGUICommand object.
    */
    CGUICommandPtr LoadCommandFromStream();

    /** Loads a CGUILayouter from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the layouter class
        ID. Calls CGUIFactory::CreateLayouter() to create a layouter object, then
        calls ReadFromStream on the new object.
        @return Pointer to the created and initialized CGUILayouter object.
    */
    CGUILayouter* LoadLayouterFromStream();

    /** Loads a CGUIBehaviour from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the behaviour
        class ID. Calls CGUIFactory::CreateBehaviour() to create a
        behaviour object, then calls ReadFromStream on the new object.
        @return Pointer to the created and initialized CGUIBehaviour object.
    */
    CGUIBehaviourDecorator* LoadBehaviourFromStream();

    /** Loads a CGUIAnimation from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the animation
        class ID. Calls CGUIFactory::CreateAnimation() to create an
        animation object, then calls ReadFromStream on the new object.
        @return Pointer to the created and initialized CGUIAnimation object.
    */
    CGUIAnimation* LoadAnimationFromStream();

    /**
        Returns the pointer to the object that is currently reading from the stream.
        @return The object or NULL when currently no object is reading from stream.
    */
    const CGUIObject* GetCurrentlyStreamingObject() const {return m_pkCurrentlyStreamingObject;}

    /** Returns the name of the file which is currently being streamed.
        @return The name of the file from which the factory is currently streaming
    */
    const eC_String& GetCurrentFileName() const { return m_kCurrentFileName; }

    /** Returns the error state of the last LoadDialog operation.
        @return True, in case of an error loading the dialog, False otherwise.
     */
    eC_Bool DialogLoadingError() const { return m_bDialogLoadingError; }

private:
    /// CGUICompositeObject must be able to call LoadDialogFromStream.
    friend class CGUICompositeObject;

    /// Private constructor.
    CGUIFactoryManager();

    /// Private destructor.
    ~CGUIFactoryManager() {}

    /// Private copy constructor.
    CGUIFactoryManager(const CGUIFactoryManager& kSource);

    /// Private assignment operator.
    CGUIFactoryManager& operator=(const CGUIFactoryManager& kSource);

    /** Loads a CGUIObject from the current streaming file.
        Uses the current instance of CGUIStreamReader to read the object class
        ID. Calls CGUIFactory::CreateControl() to create a GUI object, then calls
        ReadFromStream on the new object.
        @return Pointer to the created and initialized GUI object.
        @throws CGUIStreamingControlException When catching a
                CGUIStreamingException, it is not re-thrown. Instead, a
                CGUIStreamingControlException containing the pointer to the new
                object is thrown.
    */
    CGUIObject* LoadDialogFromStream();

    /** Checks whether factory list is empty and throws an exception in that case
        @throws CGUIException
    */
    void CheckNofFactoryEntries(void);

    /// Singleton instance.
    static CGUIFactoryManager* ms_pInstance;

    /// Name of the file that is currently being processed.
    eC_String m_kCurrentFileName;

    /// Factory list type.
    typedef eC_TListDoubleLinked<CGUIFactoryPtr> FactoryList;

    /// List of user factories.
    FactoryList m_kFactories;

    /** Holds the pointer to the currently streaming object.
    This pointer is set in LoadDialogFromStream before ReadFromStream is called on the object and reset to NULL afterwards.
    The pointer enables the stream reader to access object which is currently reading itself from the stream.
    */
    CGUIObject* m_pkCurrentlyStreamingObject;

    /** Dialog loading error flag. */
    eC_Bool m_bDialogLoadingError;
};

#endif // GUILIANI_STREAM_GUI
#endif // GUIFACTORY_MANAGER_H
