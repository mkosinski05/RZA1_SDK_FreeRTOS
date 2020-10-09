/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIDATAPOOL__H_
#define GUIDATAPOOL__H_

#include "eC_Types.h"
#include "eC_TList_doubleLinked.h"
#include "eC_Semaphore.h"
#include "GUISubject.h"
#include "GUIObserver.h"
#include "eC_String.h"
#include "GUIValue.h"
#include "GUIObjectHandleResource.h"
#include "GUIDataPoolResource.h"


class CDataPoolEntry;

/// Type for datapool callback-functions
typedef void (*DataPoolCallback)(CDataPoolEntry&);


// @guiliani_doxygen toplevel_module DataPool
/**
    <table border="0">
    <tr>
        <td width="200">@image html DataPool.png</td>
            <td>
                The "data pool" module is providing all means for on the fly data updates of GUI controls. It supports automatic notification 
                on data updates and enables easy data binding. The "data pool" offers connectors to attach single variables, arrays or even complex
                data structure including databases. Furthermore, the "data pool" ensures that all data access is performed Thread Save (Class: CGUIDataPool).

            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen



/** @brief Connects a GUIObject to the DataPool.
    DataPoolConnectors are used to observe values within the DataPool.
    GUIObjects, which are observing entries in the datapool must have a DataPoolConnector.
    A DataPoolConnector will be notified of changes to the DataPool via the OnNotification(DataPoolEntry&) Interface,
    and will notify its observers via the NotifyObservers (const CGUIValue&, const CGUIObject* const, const eC_UInt, const eC_UInt)
    interface if the value of its GUIObject changes. 

    @ingroup GUILIANI_DATAPOOL
*/
class CDataPoolConnector : public CGUIObserver, public CGUISubject
{
public:
    /** Constructs a DataPoolConnector and attaches it to the given Object
        @param pParentObject Pointer to object to which the DataPoolConnector shall be attached 
    */
    CDataPoolConnector(CGUIObject* pParentObject = NULL) :
        CGUIObserver(),
        m_pParentObject(pParentObject)
    {}

    /** Sets the pointer to the parent CGUIObject to which this DataPoolConnector is attached
        @param pParentObject Pointer to object to which the DataPoolConnector shall be attached
    */
    void SetParentObject( CGUIObject *pParentObject)
    {
        m_pParentObject = pParentObject;
    }

#ifndef GUILIANI_NO_DATAPOOL
    /** Called by the DataPool if an entry has changed
        @param kObservedValue DataPoolEntry contain the updated value(s)
    */
    virtual void OnNotification(CDataPoolEntry& kObservedValue);
#endif

private:
    CGUIObject* m_pParentObject;

    // Unused OnNotification slots. Declared only to prevent compiler warnings.
    virtual void OnNotification() {}
    virtual void OnNotification(const eC_String& kMessage) {}
    virtual void OnNotification(const CGUIObject* const pkUpdatedObject) {}
    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0) {}
};

#ifndef GUILIANI_NO_DATAPOOL

/** @brief A single entry in the DataPool.
    The DataPoolEntry class represents a single entry (unique id + a value/array of values) within the DataPool. 
    DataPoolEntries are observed by DataPoolConnectors and will be notified of changes to the DataPoolConnector via the 
    OnNotification(value, object, x, y) interface. In the opposite direction, a DataPooolEntry notifies its observers by calling
    their OnNotification(DataPoolEntry) interfaces.

    @attention  
    Instances of CDataPoolEntry may only be accessed from the GUI-Thread and all operations must be guarded by locking the DataPool via 
    CGUIDataPool::Lock() and CGUIDataPool::Unlock().

    @ingroup GUILIANI_DATAPOOL
*/ 
class CDataPoolEntry : public CGUIObserver, public CGUISubject
{
    /// Declare CGUIDataPool as friend, so only it may set DataPoolResource_t and delete DataPool entries
    friend class CGUIDataPool; 

public:
    /** Gets the value from the internal value-array at the given indices
        @param uiX X - Index for which to set the value
        @param uiY Y - Index for which to set the value
        return the requested value
        */
    virtual const CGUIValue& Get(const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Gets the maximum quantity of the internal array of CGUIValues
        @param[out] uiX X - Quantity
        @param[out] uiY Y - Quantity
        */
    void GetQuantity(eC_UInt& uiX, eC_UInt& uiY) { uiX = m_uiMaxX; uiY=m_uiMaxY; } 
  
    /** Returns the associated DataPool resource ID
        @return associated DataPool resource ID
    */
    inline DataPoolResource_t GetID() const { return m_eDataID; }

protected:
    /// Default constructor
    CDataPoolEntry();

    /** Constructor
        @param kValue The value of the entry
    */
    CDataPoolEntry(const CGUIValue& kValue);

    /** Sets the maximum quantity of the internal array of CGUIValues.
        The quantity is the number of values, which is stored in the entry. It is stored as a two-dimensional array.
        @note The entries former content will be deleted by this operation.
        @param uiX New X Quantity
        @param uiY New Y Quantity
    */
    void SetQuantity(const eC_UInt uiX, const eC_UInt uiY);

    /** Sets the value in the internal value-array at the given indices.
        When overriding this method, make sure to notify the DataPool about changed values via CGUIDataPool::AddToNotificationQueue(*this)
        Also make sure to update values only if they have actually changed either in value or type.
        @param kValue The new Value to set
        @param uiX X-Index for which to set the value
        @param uiY Y-Index for which to set the value */
    virtual void Set( const CGUIValue& kValue, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Copy constructor. Note that only the list of Subjects/Observers will be copied, but
        NOT the internal array of CGUIValues.
        @param kDataPoolEntry Where to copy from
        @return Where to copy to
    */
    virtual CDataPoolEntry& operator= ( const CDataPoolEntry& kDataPoolEntry);

    /** Overloaded AddObserver Implementation, so only DataPoolConnectors may be added/removed to this subject
        @param pObserver DataPoolconnector, which shall observe this DataPoolEntry
    */
    void AddObserver( CDataPoolConnector *pObserver )
    {
        CGUISubject::AddObserver( pObserver );
    }

    /** Register a callback function with this DataPoolEntry, which will be notified
        along with all observers.
        @param pCallback Pointer to callback function of type DataPoolCallback */
    void SetCallback( DataPoolCallback pCallback)
    {
        m_pDataPoolCallback = pCallback;
    }

    /// Call OnNotification( CDataPoolEntry&) on observing DataPoolConnectors and call registered callback function
    void NotifyDataPoolConnectors();

    /** Notification slot, which is called by associated Subjects (e.g. objects within the GUI) when their value changes.
        The default implementation locks the DataPool, calls Set(kValue, uiX, uiY). and unlocks the DataPool again.
        @param kValue New value, which shall be set in the  DataPoolEntry
        @param pkUpdatedObject Object from which notification was triggered. (Can be NULL)
        @param uiX X-Index for which to set the value
        @param uiY Y-Index for which to set the value
    */
    virtual void OnNotification(const CGUIValue& kValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

protected:

    /// Maximum X-Size of internal array 
    eC_UInt                 m_uiMaxX;
    /// Maximum Y-Size of internal array 
    eC_UInt                 m_uiMaxY;

    /// Internal array of GUIValues
    eC_TArray<CGUIValue>    m_kValue;

    /// DataPool resource ID associated with this entry.
    DataPoolResource_t      m_eDataID;

    /// Callback function, which will be called within NotifyDataPoolConnectors
    DataPoolCallback        m_pDataPoolCallback;

    /// protected Destructor: only friend class CGUIDataPool is allowed to delete DataPoolEntries
    virtual ~CDataPoolEntry(){}

private:
    /// Sets the associated DataPool resource ID. Only allowed to invoke by friend class CDataPool.
    inline void SetID(const DataPoolResource_t &eID)
    {
        m_eDataID = eID;
    }

    /// Only DataPoolConnectors may be added/removed to this subject, therefore the standard API is hidden
    void AddObserver( CGUIObserver *pObserver );
};



/** @defgroup GUILIANI_DATAPOOL DataPool
    @brief The Guiliani DataPool Component for generic binding of UI Elements to arbitrary data sources.

    The DataPool stores application specific data and allows objects within the user interface to get notified when data changes, 
    for which they have registered themselves as observers.
    <p>
    The DataPool is meant to work as an interface between the GUI and the underlying application and is thus designed to be threadsafe.
    The CGUIDataPool::Set() and CGUIDataPool::Get() methods may therefore be accessed from both the UI-Thread, and application threads.
    @attention
    Note that the ONLY threadsafe APIs are:
    - Set( const DataPoolResource_t eDataID, CGUIValue kValue, const eC_UInt uiX, const eC_UInt uiY )
    - Get( const DataPoolResource_t eDataID, CGUIValue& kValue, const eC_UInt uiX, const eC_UInt uiY )
    Do NOT call any other APIs from outside the Guiliani thread!
    <p>


    The following example shows how an object gets registered to an entry within the DataPool.
    In this example a DataPool entry named ID_OF_DATAPOOL_RESOURCE (which has been defined in UserDataPoolResource.h) gets connected to the object pkSlider.
    Note that this will always result in a two-way dependency, which means that the object within the GUI will get notified when the DataPool
    changes, but changing the GUI object's value (e.g. by dragging the slider) will also update the value within the DataPool.
@code
    // Create an object
    CGUISlider* pkSlider = new CGUISlider(this, eC_FromInt(0), eC_FromInt(0), 
                                        eC_FromInt(300), eC_FromInt(20),
                                        0, 0, 100, 10,
                                        CGUIRange::BASE_AT_MINIMUM,
                                        CGUIBaseSlider::SLD_HORIZONTAL,
                                        OBJ_MY_SLIDER);
    // Register to entry within the DataPool                                    
    CGUIDataPool::Register( ID_OF_DATAPOOL_RESOURCE, pkSlider );
@endcode


    <p>
    Using pointers is not possible when streaming. In this case, use the CGUIDataPool::AutoRegister() method instead.
    It registers an Object Handle with an entry of the DataPool. If an object with this ID is constructed (or streamed) it will
    automatically be registered within the DataPool.
@code
    CGUIDataPool::AutoRegister( ID_OF_DATAPOOL_RESOURCE, OBJ_MY_SLIDER );
@endcode


    <p>
    Manually changing a value within the DataPool (e.g. from application code) can be done via the CGUIDataPool::Set() interface.
@code
    CGUIDataPool::Set( ID_OF_DATAPOOL_RESOURCE, "New Value" );
@endcode

    <p>
@section sec_guidatapool_custom_datapoolconnectors Implementing a custom DataPoolConnector
    The following example code demonstrates how to implement a custom DataPoolConnector, which observes a value within the DataPool.
    Its OnNotification() method will be called whenever the associated value within the DataPool gets updated.
@code
// A basic customized DataPoolConnector example
class MyDataPoolConnector : public CDataPoolConnector
{
public:
    MyDataPoolConnector(): CDataPoolConnector() {};

    // Called by the DataPool if an entry has changed
    virtual void OnNotification( CDataPoolEntry& kObservedValue)
    {
        GUILOGMESSAGE("Received an update from the DataPool. New value: " + kObservedValue.Get().ToString() + "\n");
    }
};

// Usage example:
MyDataPoolConnector* pMyDataPoolConnector = new MyDataPoolConnector();
CGUIDataPool::Register( ID_OF_DATAPOOL_RESOURCE, pMyDataPoolConnector);
@endcode

    <p>
@section sec_guidatapool_callbacks Implementing a simple callback function
It is possible to register callback functions to entries within the DataPool, which will automatically be called when 
their corresponding value gets changed. To work as a callback your function must accept a CDataPoolEntry& as a paramter 
and have void as return value. The DataPoolEntry parameter will contain the observed DataPoolEntry from which you can
extract its current value.

@code
// A simple callback function, which gets called when an entry within the DataPool changes
void MyDataPoolCallback( CDataPoolEntry& kDataPoolEntry)
{
    GUILOGMESSAGE(" MyDataPoolCallback called.  New value: " + kDataPoolEntry.Get().ToString() + "\n");
}

// Usage example:
CGUIDataPool::Register( DATA_ENTRY_1, &MyDataPoolCallback);
@endcode

    <p>
@section sec_guidatapool_custom_controls Implementing controls with custom DataPool-bindings

    The following is an example implementation of a customized Image-control, which will display one of three
    defineable images with regard of the value received from the DataPool.

    It inherits from the standard CGUIImage base class, but extends it by a custom SetValue() method,
    which implements the communication with the DataPool.

    The SetValue() implementation will interpret the value received from the DataPool as an integer.
    If the value lies in the range of 0-49 it will display the "GreenImage" supplied during construction.
    For values in the range of 50-74 it will display the "YellowImage", and for all others it will display
    the "RedImage".

@code
#include "GUIImage.h"
class CustomImage : public CGUIImage
{
public:
    CustomImage ( 
        CGUICompositeObject* const pParent, 
        const eC_Value& vX, 
        const eC_Value& vY, 
        const ImageResource_t& eGreenImageID, 
        const ImageResource_t& eYellowImageID, 
        const ImageResource_t& eRedImageID, 
        const ObjectHandle_t& eID = NO_HANDLE)
        : CGUIImage(  pParent, vX, vY, eGreenImageID, eID), 
        m_eGreenImage(eGreenImageID), m_eYellowImage(eYellowImageID), m_eRedImage(eRedImageID)
    {}

    /// This method gets called by the DataPool 
    virtual eC_Bool SetValue( CDataPoolEntry& rkValue)
    {
        // If the numerical value received by the DataPool is smaller than 50, show the "green image"
        if( rkValue.Get().ToInt() < 50)
        {
            SetImage( m_eGreenImage );
        }
        // If the numerical value received by the DataPool is between 50 and 75 , show the "yellow image"
        else if( rkValue.Get().ToInt() < 75)
        {
            SetImage( m_eYellowImage );
        }
        // in all other cases show the "red image"
        else
        {
            SetImage( m_eRedImage );
        }
        InvalidateArea();

        CGUIObject::SetValue( rkValue);
        return true; 
    }
protected:
    ImageResource_t m_eGreenImage;
    ImageResource_t m_eYellowImage;
    ImageResource_t m_eRedImage;
};
@endcode

    <p>
@section sec_guidatapool_custom_datapoolentries Implementing custom DataPoolEntries

    In some ocassions it will be necessary to replace the standard DataPoolEntries within the DataPool with customized
    code, which does for instance access an external database to retrieve the requested data. For this purpose it is
    possible to..

    -# inherit from DataPoolEntry, 
    -# add the specific code to the Get() method which is called whenever an observer of the DataPool wishes to retrieve the current value of this entry within the DataPool
    -# and assign the customized DataPoolEntry to the DataPool via CGUIDataPool::SetDataPoolEntry

    The following example code gives a skeleton implementation of a customized DataPoolEntry.
@code
// Customized DataPoolEntry type.
// This could e.g. access a database in Get(...) to retrieve the requested data
class MyDataPoolEntry : public CDataPoolEntry
{
public:
    MyDataPoolEntry() 
    { 
        // Choose initial dimension for data storage of 4x1 values
        SetQuantity(4,1); 
    }

    void Set( const CGUIValue& kValue, const eC_UInt uiX, const eC_UInt uiY) 
    {
        // Check if value has really changed
        const CGUIValue& kOldValue = Get(uiX, uiY);

        // Do only trigger notifications and update values if the value has actually changed.
        // Note: Even an identical value may trigger a notification when the datatypes are different!
        if( kOldValue.GetType() != kValue.GetType() ||
            kOldValue != kValue)
        {
            // TODO: Custom Code for setting the new value goes here...
            // or just call CDataPoolEntry::Set( kValue, uiX, uiY);
            // Notify the DataPool about the changed value
            CGUIDataPool::AddToNotificationQueue( *this);
        }
    }

    const CGUIValue& Get( const eC_UInt uiX = 0, const eC_UInt uiY = 0) 
    { 
        // Custom Data retrieval code goes here....
        // or just return CDataPoolEntry::Get(uiX, uiY);
    }
};

// Assign a custom DataPoolEntry to the entry named ID_OF_DATAPOOL_RESOURCE within the DataPool 
CGUIDataPool::SetDataPoolEntry( ID_OF_DATAPOOL_RESOURCE, new MyDataPoolEntry() );
@endcode


    <p>
@section sec_guidatapool_arrays Using arrays within the DataPool

    A single DataPool entry can store more than a single CGUIValue. If you wish to store multiple values you can access these via the
    x,y parameters of the CGUIDataPool::Set() and CGUIDataPool::Get() interfaces.
    Most standard GUI elements will only make use of the first value (=which is the one at indices 0,0). But some - such as ListBoxes - will 
    automatically know how to deal with arrays.

    The following example shows how to set / get arrays of data in the DataPool:
@code
    // Set maximum number of values for ID_OF_DATAPOOL_RESOURCE
    CDataPoolEntry* pkEntry;
    CGUIDataPool::Get( ID_OF_DATAPOOL_RESOURCE, &pkEntry);
    if( pkEntry) pkEntry->SetQuantity(3,1);

    // This sets the string "Last Mouse Position" into the first value (=index 0,0) of ID_OF_DATAPOOL_RESOURCE
    // and the mouse X/Y positions at indices (1,0) and (2,0)
    CGUIDataPool::Set( ID_OF_DATAPOOL_RESOURCE, "Last Mouse Position", 0,0);
    CGUIDataPool::Set( ID_OF_DATAPOOL_RESOURCE, uiMouseX, 1,0);
    CGUIDataPool::Set( ID_OF_DATAPOOL_RESOURCE, uiMouseY, 2,0);
    
    // This shows these values in a Listbox 
    CGUIListBox* pListBox = new CGUIListBox( this, eC_FromInt(10), eC_FromInt(0), eC_FromInt(200), eC_FromInt(60) );
    CGUIDataPool::Register(ID_OF_DATAPOOL_RESOURCE, pListBox);
@endcode
*/

#define DATAPOOL_FILE_VERSION   2

/** DataPool implementation for Guiliani applications.
    @see GUILIANI_DATAPOOL
    @ingroup GUILIANI_DATAPOOL
*/
class CGUIDataPool
{
public:
    /// Convenience typedef for a list of DataPoolResource_t 
    typedef eC_TListDoubleLinked<DataPoolResource_t> DataIDList;
    /** @name Data handling */
    ///@{
    /** Sets the value of a DataPool entry. Setting a value in the Datapool will trigger a notification 
        to all its observers during the next call to ProcessNotifications.
        This API is threadsafe.
        @param eDataID ID of entry within the DataPool, which shall be set
        @param kValue New value to of eDataID
        @param uiX X-Index of value to be set within eDataID (if it is a multidimensional DataPoolEntry)
        @param uiY Y-Index of value to be set within eDataID (if it is a multidimensional DataPoolEntry)
    */
    static void Set (const DataPoolResource_t eDataID, CGUIValue kValue, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Gets the current value of the given DataPool entry.
        This API is threadsafe.
        @param eDataID ID of entry within the DataPool, which shall be returned
        @param kValue Reference to GUIValue, which will receive the current value of eDataID
        @param uiX X-Index of value to get within eDataID (if it is a multidimensional DataPoolEntry)
        @param uiY Y-Index of value to get within eDataID (if it is a multidimensional DataPoolEntry)
    */
    static void Get(const DataPoolResource_t eDataID, CGUIValue& kValue, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Sets a specific DataPoolEntry for the given DataPoolResource. 
        This enables users to write custom DataPoolEntries, which e.g., read data from external data sources such as databases, 
	or application specific data structures.
        @param eDataID ID of entry within the DataPool, which shall be set  
        @param pkEntry Custom DataPoolEntry to be assigned to eDataID
    */
    static void SetDataPoolEntry( const DataPoolResource_t eDataID, CDataPoolEntry* pkEntry);

    /** Sets the Quantity of the given DataPoolEntry.
        The quantity is the number of values, which is stored in the entry. It is stored as a two-dimensional array.
        @param eDataID ID of entry whose quantity shall be set
        @param uiX New X Quantity
        @param uiY New Y Quantity
    */
    static void SetQuantity( const DataPoolResource_t eDataID, const eC_UInt uiX, const eC_UInt uiY );

    /** Gets the DataPoolEntry for the given DataID,
        @param eDataID ID of entry within the DataPool, which shall be returned
        @param ppkEntry Address of a pointer to a DataPoolEntry, which will be set to the found entry or to NULL if not found
    */
    static void Get( const DataPoolResource_t eDataID, CDataPoolEntry **ppkEntry);

    /** Requests an immediate update for the given DataPoolConnector.
        All DataPoolEntries to which the DataPoolConnector is registered will trigger an OnNotification( CDataPoolEntry& ) on pkObserver.
        @param pkObserver DataPoolConnectorwhich shall receive an update with the current values within the DataPool.
    */
    static void GetUpdate( CDataPoolConnector* pkObserver);
    ///@}
    /** @name Locking mechanisms */
    ///@{
    /** Locks the DataPool for thread-critical operations.
        @return True if the DataPool was locked successfully, False otherwise. */
    static eC_Bool Lock() { return ms_kDataPoolSema.Enter(); }

    /** Unlocks the DataPool after thread-critical operations.
        @return True if the DataPool was unlocked successfully, False otherwise. */
    static eC_Bool Unlock() { return ms_kDataPoolSema.Leave(); }
    ///@}
    /** @name Management of DataPool entries */
    ///@{
    /** Registers an object to the data entry with the given ID.
        The object's SetValue() method will then automatically be called by the object's CGUIDataPoolConnector if the data entry changes.
        @param eDataID ID of entry within the DataPool to which the given object shall be registered.
        @param pkObserver Pointer to GUIObject, which shall be registered to eDataID
    */
    static void Register( const DataPoolResource_t eDataID, CGUIObject* pkObserver);
    
    /** Registers a DataPoolConnector to the data entry with the given ID.
        The CDataPoolConnector's OnNotification() method will be called if the data entry changes.
        @param eDataID ID of entry within the DataPool to which the given DataPoolConnector shall be registered.
        @param pkObserver Pointer to DataPoolConnector, which shall be registered to eDataID
    */
    static void Register( const DataPoolResource_t eDataID, CDataPoolConnector* pkObserver);

    /** Registers a Callback function within the DataPoolEntry for the given ID
        Note that only ONE callback function can be registered to any DataPool ID. Registering another callback will replace the previous one.
        @param eDataID ID of entry within the DataPool to which the given callback function shall be registered.
        @param pCallback function pointer to the callback, which shall be registered to eDataID
    */
    static void Register( const DataPoolResource_t eDataID, DataPoolCallback pCallback);

    /** Unregisters the Callback function for the DataPoolEntry for the given ID.
        @param eDataID ID of entry within the DataPool for which the callback shall be unregistered.
    */
    static void UnregisterCallback( const DataPoolResource_t eDataID);

    /** Unregisters an object to/from the data entry with the given ID.
        @param eDataID ID of entry within the DataPool from which the given object shall be unregistered.
        @param pkObserver Pointer to GUIObject, which shall be unregistered from eDataID
    */
    static void Unregister( const DataPoolResource_t eDataID, CGUIObject* pkObserver);
    
    /** Unregisters a DataPoolConnector to/from the data entry with the given ID.
        @param eDataID ID of entry within the DataPool from which the given DataPoolConnector shall be unregistered.
        @param pkObserver Pointer to DataPoolConnector, which shall be unregistered from eDataID
    */
    static void Unregister( const DataPoolResource_t eDataID, CDataPoolConnector* pkObserver);

    /** Unregisters an object from ALL Data entries.
        @param pkObserver Pointer to GUIObject, which shall be unregistered from all DataPoolEntries
    */
    static void Unregister( CGUIObject* pkObserver);

    /** Unregisters a DataPoolConnector from ALL Data entries.
        @param pkObserver Pointer to DataPoolConnector, which shall be unregistered from all DataPoolEntries
    */
    static void Unregister( CDataPoolConnector* pkObserver);

    /** Automatically registers an object ID to an entry within the DataPool.
        Whenever an object with this ID gets constructed, it will automatically be registered to the given DataPool entry. 
        @note This will automatically perform a GETGUI.GetObjectByID( eObjectID) and thus register the first found object with this ID
        to eDataID.
        @param eDataID ID of entry within DataPool to which this ObjectID shall be automatically registered
        @param eObjectID ID of object, which shall automatically be registered to eDataID upon construction.
    */
    static void AutoRegister( const DataPoolResource_t eDataID, const ObjectHandle_t eObjectID);

    /** Removes a DataID from the Auto-Registry of the given ObjectID.
        Objects with this ObjectID will no longer be automatically registered to this DataID upon their construction.
        @param eDataID ID of entry within DataPool to which this ObjectID shall no longer be automatically registered
        @param eObjectID ObjectID, which shall no longer be automatically registered to eDataID
    */
    static void RemoveAutoRegister( const DataPoolResource_t eDataID, const ObjectHandle_t eObjectID);
    
    /** Creates a dynamic DataPool Entry and returns its ID.
        @return ID of newly created entry within DataPool
    */
    static DataPoolResource_t RegisterDynamicDataPoolResource();
    ///@}
#ifdef GUILIANI_STREAM_GUI
    /** Reads the DataPool from a file. 
        The DataPool file stores Object handles, which will be autoregistered to a given DataPool entry. 
        @param kPath Full filename (including path) of the file containing the data pool */
    static void ReadDataPoolFromFile(const eC_String& kPath);
#endif
    
#ifdef GUILIANI_WRITE_GUI
    /** Writes the DataPool to a file. 
        The DataPool file will contain the current Auto-Registry. All Object handles, which were registered to a DataPool entry through
        an earlier call to AutoRegister will be written to the DataPool file.
        @param kPath Full filename (including path) of the file containing the data pool */
    static void WriteDataPoolToFile(const eC_String& kPath);
#endif

    /** @return A list of all Object IDs, which will be automatically registered to the given DataPool-entry upon their construction.
        @param eDataID The DataPool ID to query
    */
    static const eC_TListDoubleLinked<ObjectHandle_t> GetAutoRegisteredObservers(const DataPoolResource_t eDataID );

    /** Adds the datapool entry to the list of pending notifications.
        Note that each DataPoolEntry gets notified only ONCE per GUI-cycle! This is required to avoid circular dependencies.
        @param kEntry DataPoolEntry for which a notification shall be triggered.
    */
    static void AddToNotificationQueue( CDataPoolEntry& kEntry);

    /** @return Total number of DataPoolResources */
    static eC_UInt GetNOFDataPoolResources() { return ms_kDataPool.GetQuantity(); }

    /** Removes all registered DataPool observers and all entries within the list of auto-registered observers. */
    static void Clear();

    /** @return The number of currently pending notifications, which will be processed during the next frame */
    static eC_UInt GetNumberOfPendingNotifications() { return ms_kPendingNotifications.GetQuantity(); }

    /** Dumps debugging information about the DataPool to the registered trace sinks. 
        This is only available within debug builds. */
    static void DumpDataPool();

private:
    // Declare CGUIObject as friend, so only it may access NotifyOfCreation and NotifyOfDestruction
    friend class CGUIObject;
    // Declare CGUI as friend, so only it may access ProcessNotifications
    friend class CGUI;

    // Called by CGUIObject base class upon construction of an object, or after an object got sucessfully streamed.
    static void NotifyOfCreation( CGUIObject* pObject);

    // Once per cycle, the GUI checks for changes in the DataPool and Notifies all registered observers
    static void ProcessNotifications();
    // Called by CGUI during initialization
    static void Init();
    // Called by CGUI during DeInit
    static void DeInit();

    // Never used - declared just to avoid accidental usage
    CGUIDataPool();
    ~CGUIDataPool();

    // Resizes the internal array of data pool entries to the given size
    static eC_Bool SetNOFDataPoolResources(const eC_UInt uiNumberOfDataPoolResources);

    // Array of all known DataPoolEntries (Index within the Array = DataPoolResource_t of the respective DataPoolEntry)
    static eC_TArray<CDataPoolEntry*> ms_kDataPool;

    // Convenience typedef for a list of DataPoolEntries 
    typedef eC_TListDoubleLinked<CDataPoolEntry*> DataPoolEntryList;
    // List of data pool entries, which shall be processed (And thus their observers be notified) during the next call to "ProcessNotifications"
    static DataPoolEntryList ms_kPendingNotifications;

    // The Data registry stores all ObjectIDs, which shall automatically be observing an entry in the datapool if the respective object is created
    class DataRegistryElement
    {   
    public:
        DataRegistryElement( const ObjectHandle_t eObjectHandle) : m_eObjectHandle(eObjectHandle) {}
        DataRegistryElement() : m_eObjectHandle(NO_HANDLE) {}
        ObjectHandle_t  m_eObjectHandle;
        DataIDList      m_DataIDList;
    
        bool operator==(const DataRegistryElement& Value) { return Value.m_eObjectHandle  == m_eObjectHandle; }
    };
    typedef eC_TListDoubleLinked<DataRegistryElement> DataRegistryList;
    static DataRegistryList ms_kDataRegistry;

    // Semaphores guarding the DataPool
    static eC_Semaphore ms_kDataPoolSema;
    static eC_Semaphore ms_kNotificationQueueSema;
};

#else

/// Stub implementation for Guiliani builds without DataPool
class CGUIDataPool
{
    friend class CGUIObject;
    friend class CGUI;
public:
    static void Set( const DataPoolResource_t eDataID, CGUIValue kValue, const eC_UInt uiX=0, const eC_UInt uiY=0 ){}
    static void SetDataPoolEntry( const DataPoolResource_t eDataID, CDataPoolEntry* pkEntry){}
    static void Get( const DataPoolResource_t eDataID, CGUIValue& kValue, const eC_UInt uiX=0, const eC_UInt uiY=0 ){}
    static void Get( const DataPoolResource_t eDataID, CDataPoolEntry* pkEntry){}
    static void GetUpdate( CDataPoolConnector* pkObserver){}
    static void Register( const DataPoolResource_t eDataID, CGUIObject* pkObserver){}
    static void Register( const DataPoolResource_t eDataID, CDataPoolConnector* pkObserver){}
    static void Register( const DataPoolResource_t eDataID, DataPoolCallback pCallback){}
    static void Unregister( const DataPoolResource_t eDataID, CGUIObject* pkObserver){}
    static void Unregister( const DataPoolResource_t eDataID, CDataPoolConnector* pkObserver){}
    static void Unregister( CGUIObject* pkObserver){}
    static void Unregister( CDataPoolConnector* pkObserver){}
    static void AutoRegister( const DataPoolResource_t eDataID, const ObjectHandle_t eObjectID){}
    static void RemoveAutoRegister( const DataPoolResource_t eDataID, const ObjectHandle_t eObjectID){}
    static void ReadDataPoolFromFile(const eC_String& kPath){}
    static void WriteDataPoolToFile(const eC_String& kPath){}
    static const eC_TListDoubleLinked<ObjectHandle_t> GetAutoRegisteredObservers(const DataPoolResource_t eDataID ){ static eC_TListDoubleLinked <ObjectHandle_t> kDummyList; return kDummyList; }
    static DataPoolResource_t RegisterDynamicDataPoolResource(){ return DUMMY_DATAPOOLRESOURCE;}
    static void AddToNotificationQueue( CDataPoolEntry* pkEntry){}
    static eC_UInt GetNOFDataPoolResources() { return 0; }
    static void Clear(){}
    static eC_UInt GetNumberOfPendingNotifications() { return 0; }
    static void DumpDataPool(){}
private:
    
    static void NotifyOfCreation( CGUIObject* pObject) {}
    static void ProcessNotifications() {}
    static void Init() {}
    static void DeInit() {}
};

#endif




#endif
