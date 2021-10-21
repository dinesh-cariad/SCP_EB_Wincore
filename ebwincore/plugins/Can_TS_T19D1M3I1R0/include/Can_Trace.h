/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CAN_TRACE_H
#define CAN_TRACE_H

/*==[Includes]================================================================*/
#include <Can_Cfg.h>

#if (CAN_TRACE_USAGE == STD_ON)
#include <Dbg.h>
#endif /* (CAN_TRACE_USAGE == STD_ON) */

/*==[Macros]==================================================================*/
#ifndef DBG_CAN_INIT_ENTRY
/** \brief Entry point of function Can_Init() */
#define DBG_CAN_INIT_ENTRY(Config)
#endif

#ifndef DBG_CAN_INIT_EXIT
/** \brief Exit point of function Can_Init() */
#define DBG_CAN_INIT_EXIT(Config)
#endif


#ifndef DBG_CAN_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Can_GetVersionInfo() */
#define DBG_CAN_GETVERSIONINFO_ENTRY(versioninfo)
#endif

#ifndef DBG_CAN_GETVERSIONINFO_EXIT
/** \brief Exit point of function Can_GetVersionInfo() */
#define DBG_CAN_GETVERSIONINFO_EXIT(versioninfo)
#endif


#ifndef DBG_CAN_INITCONTROLLER_ENTRY
/** \brief Entry point of function Can_InitController() */
#define DBG_CAN_INITCONTROLLER_ENTRY(Controller, Config)
#endif

#ifndef DBG_CAN_INITCONTROLLER_EXIT
/** \brief Exit point of function Can_InitController() */
#define DBG_CAN_INITCONTROLLER_EXIT(Controller, Config)
#endif


#ifndef DBG_CAN_SETCONTROLLERMODE_ENTRY
/** \brief Entry point of function Can_SetControllerMode() */
#define DBG_CAN_SETCONTROLLERMODE_ENTRY(Controller, Transition)
#endif

#ifndef DBG_CAN_SETCONTROLLERMODE_EXIT
/** \brief Exit point of function Can_SetControllerMode() */
#define DBG_CAN_SETCONTROLLERMODE_EXIT(RetVal, Controller, Transition)
#endif


#ifndef DBG_CAN_DISABLECONTROLLERINTERRUPTS_ENTRY
/** \brief Entry point of function Can_DisableControllerInterrupts() */
#define DBG_CAN_DISABLECONTROLLERINTERRUPTS_ENTRY(Controller)
#endif

#ifndef DBG_CAN_DISABLECONTROLLERINTERRUPTS_EXIT
/** \brief Exit point of function Can_DisableControllerInterrupts() */
#define DBG_CAN_DISABLECONTROLLERINTERRUPTS_EXIT(Controller)
#endif


#ifndef DBG_CAN_ENABLECONTROLLERINTERRUPTS_ENTRY
/** \brief Entry point of function Can_EnableControllerInterrupts() */
#define DBG_CAN_ENABLECONTROLLERINTERRUPTS_ENTRY(Controller)
#endif

#ifndef DBG_CAN_ENABLECONTROLLERINTERRUPTS_EXIT
/** \brief Exit point of function Can_EnableControllerInterrupts() */
#define DBG_CAN_ENABLECONTROLLERINTERRUPTS_EXIT(Controller)
#endif


#ifndef DBG_CAN_CHECKWAKEUP_ENTRY
/** \brief Entry point of function Can_CheckWakeup() */
#define DBG_CAN_CHECKWAKEUP_ENTRY(Controller)
#endif

#ifndef DBG_CAN_CHECKWAKEUP_EXIT
/** \brief Exit point of function Can_CheckWakeup() */
#define DBG_CAN_CHECKWAKEUP_EXIT(RetVal, Controller)
#endif


#ifndef DBG_CAN_WRITE_ENTRY
/** \brief Entry point of function Can_Write() */
#define DBG_CAN_WRITE_ENTRY(Hth, PduInfo)
#endif

#ifndef DBG_CAN_WRITE_EXIT
/** \brief Exit point of function Can_Write() */
#define DBG_CAN_WRITE_EXIT(RetVal, Hth, PduInfo)
#endif


#ifndef DBG_CAN_MAINFUNCTION_WRITE_ENTRY
/** \brief Entry point of function Can_MainFunction_Write() */
#define DBG_CAN_MAINFUNCTION_WRITE_ENTRY()
#endif

#ifndef DBG_CAN_MAINFUNCTION_WRITE_EXIT
/** \brief Exit point of function Can_MainFunction_Write() */
#define DBG_CAN_MAINFUNCTION_WRITE_EXIT()
#endif


#ifndef DBG_CAN_MAINFUNCTION_READ_ENTRY
/** \brief Entry point of function Can_MainFunction_Read() */
#define DBG_CAN_MAINFUNCTION_READ_ENTRY()
#endif

#ifndef DBG_CAN_MAINFUNCTION_READ_EXIT
/** \brief Exit point of function Can_MainFunction_Read() */
#define DBG_CAN_MAINFUNCTION_READ_EXIT()
#endif


#ifndef DBG_CAN_MAINFUNCTION_BUSOFF_ENTRY
/** \brief Entry point of function Can_MainFunction_BusOff() */
#define DBG_CAN_MAINFUNCTION_BUSOFF_ENTRY()
#endif

#ifndef DBG_CAN_MAINFUNCTION_BUSOFF_EXIT
/** \brief Exit point of function Can_MainFunction_BusOff() */
#define DBG_CAN_MAINFUNCTION_BUSOFF_EXIT()
#endif


#ifndef DBG_CAN_MAINFUNCTION_WAKEUP_ENTRY
/** \brief Entry point of function Can_MainFunction_Wakeup() */
#define DBG_CAN_MAINFUNCTION_WAKEUP_ENTRY()
#endif

#ifndef DBG_CAN_MAINFUNCTION_WAKEUP_EXIT
/** \brief Exit point of function Can_MainFunction_Wakeup() */
#define DBG_CAN_MAINFUNCTION_WAKEUP_EXIT()
#endif

#ifndef DBG_CAN_PROCESSTXFRAME_ENTRY
/** \brief Entry point of function Can_ProcessTxFrame() */
#define DBG_CAN_PROCESSTXFRAME_ENTRY()
#endif

#ifndef DBG_CAN_PROCESSTXFRAME_EXIT
/** \brief Exit point of function Can_ProcessTxFrame() */
#define DBG_CAN_PROCESSTXFRAME_EXIT()
#endif

#ifndef DBG_CAN_PROCESSRXFRAME_ENTRY
/** \brief Entry point of function Can_ProcessRxFrame() */
#define DBG_CAN_PROCESSRXFRAME_ENTRY()
#endif

#ifndef DBG_CAN_PROCESSRXFRAME_EXIT
/** \brief Exit point of function Can_ProcessRxFrame() */
#define DBG_CAN_PROCESSRXFRAME_EXIT()
#endif

#ifndef DBG_CAN_STOREINRXBUFFER_ENTRY
/** \brief Entry point of function Can_StoreInRxBuffer() */
#define DBG_CAN_STOREINRXBUFFER_ENTRY(a,b,c)
#endif

#ifndef DBG_CAN_STOREINRXBUFFER_EXIT
/** \brief Exit point of function Can_StoreInRxBuffer() */
#define DBG_CAN_STOREINRXBUFFER_EXIT(a,b,c,d)
#endif

#ifndef DBG_CAN_STOREINTXBUFFER_ENTRY
/** \brief Entry point of function Can_StoreInTxBuffer() */
#define DBG_CAN_STOREINTXBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_CAN_STOREINTXBUFFER_EXIT
/** \brief Exit point of function Can_StoreInTxBuffer() */
#define DBG_CAN_STOREINTXBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_CAN_READFROMRXBUFFER_ENTRY
/** \brief Entry point of function Can_ReadFromRxBuffer() */
#define DBG_CAN_READFROMRXBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_CAN_READFROMRXBUFFER_EXIT
/** \brief Exit point of function Can_ReadFromRxBuffer() */
#define DBG_CAN_READFROMRXBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_CAN_READFROMTXBUFFER_ENTRY
/** \brief Entry point of function Can_ReadFromTxBuffer() */
#define DBG_CAN_READFROMTXBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_CAN_READFROMTXBUFFER_EXIT
/** \brief Exit point of function Can_ReadFromTxBuffer() */
#define DBG_CAN_READFROMTXBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_CAN_MAINFUNCTION_MODE_ENTRY
/** \brief Entry point of function Can_MainFunction_Mode() */
#define DBG_CAN_MAINFUNCTION_MODE_ENTRY()
#endif

#ifndef DBG_CAN_MAINFUNCTION_MODE_EXIT
/** \brief Exit point of function Can_MainFunction_Mode() */
#define DBG_CAN_MAINFUNCTION_MODE_EXIT()
#endif

#ifndef DBG_CAN_CONTROLLERSTATE
/** \brief Change of Can_ControllerState */
#define DBG_CAN_CONTROLLERSTATE(oldValue,newValue)
#endif

#ifndef DBG_CAN_INTERRUPTENABLED
/** \brief Change of Can_InterruptEnabled */
#define DBG_CAN_INTERRUPTENABLED(oldValue,newValue)
#endif

/*==[Types]===================================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* CAN_TRACE_H */
