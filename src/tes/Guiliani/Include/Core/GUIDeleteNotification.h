/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __DELETENOTIFICATION__H_
#define __DELETENOTIFICATION__H_

#include "eC_Types.h"

/**
    @brief Class that sets a local variable to True if the object has
    been deleted.

    When using this helper class, create a local eC_Bool variable bDeleted
    and call InitDeletedFlag() with this variable as the parameter.
    If the instance of this class gets deleted (the dtor is called),
    the local variable will be set to true; if the local variable has
    been set to true the method should return immediately, especially without
    accessing any variables that are not allocated on its local stack.

    Only if the object has not been deleted, call ResetDeletedFlag().

    Example:
@code
class MyClass : protected CDeleteNotification
{
    void DoSomething()
    {
        eC_Bool bDeleted;
        InitDeletedFlag(bDeleted);

        foo(); // this call may delete "this" object

        if(!bDeleted) // the flag has been set to true only if the object was deleted
        {
            ResetDeletedFlag();
            DoSomethingElse();
        }
    }

    void DoSomethingElse();
}
@endcode
*/
class CDeleteNotification
{
public:
    /// CDeleteNotification constructor.
    CDeleteNotification() :
        m_pbDeletedFlag(NULL)
    {
    }

    /** Destructor. If a flag has been set via InitDeletedFlag(), its value
        is set to <i>true</i> at this moment.
    */
    virtual ~CDeleteNotification()
    {
        if(NULL != m_pbDeletedFlag)
        {
            *m_pbDeletedFlag = true;
        }
    }

protected:
    /** Initializes the given flag to false and stores a pointer to it.
        @param bDeleted Flag indicating deletion.
    */
    void InitDeletedFlag(eC_Bool& bDeleted)
    {
        bDeleted = false;
        m_pbDeletedFlag = &bDeleted;
    }

    /// Resets the internal flag pointer to NULL.
    void ResetDeletedFlag()
    {
        m_pbDeletedFlag = NULL;
    }

private:
    eC_Bool* m_pbDeletedFlag;
};

#endif  /*__DELETENOTIFICATION__H_ */
