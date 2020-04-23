/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Semaphore.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:00:34 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC__SEMAPHORE_H_
#define EC__SEMAPHORE_H_

//-------------------------------------------------------------------------------------------------------------
#include "eC.h"

//-------------------------------------------------------------------------------------------------------------
/**
@memo
Multitasking interface for threads synchronisation.
@doc
This class provides an interface of a binary (<B>mut</B>ually-<B>ex</B>clusive) semaphore used as a platform independent
interthread synchronisation technique. <BR>
To get more information about \Ref{eC_Semaphore} and its handling, look at the \Ref{Tutorial_eC}.
@see Tutorial_eC
*/
class eC_Semaphore
{
  public:
    
    /** Standard Constructor. */
    eC_Semaphore();
    
    /** Destructor. Cleaning up again. */
    ~eC_Semaphore();
    
    /** Creates a semaphore object.
    @param pcName is an optional parameter to give the new semaphore an appealing name
    @return <B>true</B> if the semaphore object was successfully created.
    @see Enter, Leave */
    eC_Bool Create(const eC_Char * pcName = "");
    
    /** Waits for getting semaphore to enter synchronised code.
    @param uiMs maximum time (timeout value in milliseconds) to wait until entering the code sequence, if no value is set this function will wait for ever
    @return <B>false</B> if an error occured while waiting or there was a timeout, or <B>true</B> if timeout was not overstepped.
    @see Create, Leave */
    eC_Bool Enter(eC_UInt uiMs = (eC_UInt) (-1));
    
    /** Leaves the synchronised code sequence and releases the semaphore again.
    @return <B>true</B> if the semaphore was left without any error 
    @see Create, Enter */
    eC_Bool Leave();
    
  private:
    
    // !!! INTERNAL USE ONLY !!!
    // all following private members are for internal use only
    
    //
    void * pHandle;
};



//-------------------------------------------------------------------------------------------------------------
#endif
