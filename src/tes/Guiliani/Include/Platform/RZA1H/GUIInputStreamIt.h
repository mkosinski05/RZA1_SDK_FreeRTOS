/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIINPUTSTREAMIT_H_
#define GUIINPUTSTREAMIT_H_

extern "C"
{
    
}
// touchscreen
#include "r_typedefs.h"
#include "tp_if.h"

#include "eC_Semaphore.h"
#include "eC_Types.h"
#include "eC_TList_doubleLinked.h"
#include "GUIInputMedia.h"

class CGUIEvent;

class CGUIInputStreamIt : public CGUIInputMedia
{
public:
    /**
        Singleton for creating an input object and change the pointer in
        base class to input window.
     */
    static void CreateInstance(
        const eC_Int& iTouchScreenX,
        const eC_Int& iTouchScreenY,
        const eC_Int& iTouchScreenWidth,
        const eC_Int& iTouchScreenHeight,
        const eC_Int& iTouchFilterDelta);

    CGUIEvent* GetEvent(eC_UInt uiIdleTime);
    
    void AddEvent(TP_TouchFinger_st kTouchEvent);

protected:
    CGUIInputStreamIt(
    const eC_Int& iTouchScreenX,
    const eC_Int& iTouchScreenY,
    const eC_Int& iTouchScreenWidth,
    const eC_Int& iTouchScreenHeight,
    const eC_Int& iTouchFilterDelta);

    ~CGUIInputStreamIt();

    void StopIdle();

    eC_Bool m_bStopIdle;				///< Attribute to store if idle mode is stoped
    
private:
    /**
        Copy-constructor.
        @param kSource Source object to be copied.
     */
    CGUIInputStreamIt(const CGUIInputStreamIt& kSource) {}

    /**
        Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
     */
    CGUIInputStreamIt& operator=(const CGUIInputStreamIt& kSource) {return *this;}
    
private:
    eC_Semaphore m_kSemaphore;
    eC_TListDoubleLinked<TP_TouchFinger_st> m_kEventQueue;
    
    TP_TouchFinger_st m_kLastEvent;
    eC_Bool m_bEventReady;

    eC_Int m_iTouchScreenX;
    eC_Int m_iTouchScreenY;
    eC_Int m_iTouchScreenWidth;
    eC_Int m_iTouchScreenHeight;
    eC_Int m_iTouchFilterDelta;
    
    TpEvt_EntryType m_eLastTouchEvent;

    eC_Int m_iLastPositionX;
    eC_Int m_iLastPositionY;

    eC_Int m_iLastDraggedX;
    eC_Int m_iLastDraggedY;
};
#endif // GUIINPUTSTREAMIT_H_
