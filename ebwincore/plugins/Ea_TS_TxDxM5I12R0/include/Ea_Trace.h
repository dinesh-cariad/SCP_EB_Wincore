/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef EA_TRACE_H
#define EA_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Ea_Trace_Internal.h>

/*==================[macros]================================================*/

#ifndef DBG_EA_INIT_ENTRY
/** \brief Entry point of function Ea_Init() */
#define DBG_EA_INIT_ENTRY()
#endif
/* !LINKSTO EA155,1 */
#ifndef DBG_EA_MAINSTATE
/** \brief Change of Ea_MainState */
#define DBG_EA_MAINSTATE(a,b)
#endif

#ifndef DBG_EA_JOBRESULT
/** \brief Change of Ea_JobResult */
#define DBG_EA_JOBRESULT(a,b)
#endif

#ifndef DBG_EA_STATUS
/** \brief Change of Ea_Status */
#define DBG_EA_STATUS(a,b)
#endif

#ifndef DBG_EA_INIT_EXIT
/** \brief Exit point of function Ea_Init() */
#define DBG_EA_INIT_EXIT()
#endif

#ifndef DBG_EA_SETMODE_ENTRY
/** \brief Entry point of function Ea_SetMode() */
#define DBG_EA_SETMODE_ENTRY(a)
#endif

#ifndef DBG_EA_SETMODE_EXIT
/** \brief Exit point of function Ea_SetMode() */
#define DBG_EA_SETMODE_EXIT(a)
#endif

#ifndef DBG_EA_READ_ENTRY
/** \brief Entry point of function Ea_Read() */
#define DBG_EA_READ_ENTRY(a,b,c,d)
#endif

#ifndef DBG_EA_READ_EXIT
/** \brief Exit point of function Ea_Read() */
#define DBG_EA_READ_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_EA_WRITE_ENTRY
/** \brief Entry point of function Ea_Write() */
#define DBG_EA_WRITE_ENTRY(a,b)
#endif

#ifndef DBG_EA_WRITE_EXIT
/** \brief Exit point of function Ea_Write() */
#define DBG_EA_WRITE_EXIT(a,b,c)
#endif

#ifndef DBG_EA_CANCEL_ENTRY
/** \brief Entry point of function Ea_Cancel() */
#define DBG_EA_CANCEL_ENTRY()
#endif

#ifndef DBG_EA_CANCEL_EXIT
/** \brief Exit point of function Ea_Cancel() */
#define DBG_EA_CANCEL_EXIT()
#endif

#ifndef DBG_EA_GETSTATUS_ENTRY
/** \brief Entry point of function Ea_GetStatus() */
#define DBG_EA_GETSTATUS_ENTRY()
#endif

#ifndef DBG_EA_GETSTATUS_EXIT
/** \brief Exit point of function Ea_GetStatus() */
#define DBG_EA_GETSTATUS_EXIT(a)
#endif

#ifndef DBG_EA_GETJOBRESULT_ENTRY
/** \brief Entry point of function Ea_GetJobResult() */
#define DBG_EA_GETJOBRESULT_ENTRY()
#endif

#ifndef DBG_EA_GETJOBRESULT_EXIT
/** \brief Exit point of function Ea_GetJobResult() */
#define DBG_EA_GETJOBRESULT_EXIT(a)
#endif

#ifndef DBG_EA_INVALIDATEBLOCK_ENTRY
/** \brief Entry point of function Ea_InvalidateBlock() */
#define DBG_EA_INVALIDATEBLOCK_ENTRY(a)
#endif

#ifndef DBG_EA_INVALIDATEBLOCK_EXIT
/** \brief Exit point of function Ea_InvalidateBlock() */
#define DBG_EA_INVALIDATEBLOCK_EXIT(a,b)
#endif

#ifndef DBG_EA_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Ea_GetVersionInfo() */
#define DBG_EA_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_EA_GETVERSIONINFO_EXIT
/** \brief Exit point of function Ea_GetVersionInfo() */
#define DBG_EA_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_EA_ERASEIMMEDIATEBLOCK_ENTRY
/** \brief Entry point of function Ea_EraseImmediateBlock() */
#define DBG_EA_ERASEIMMEDIATEBLOCK_ENTRY(a)
#endif

#ifndef DBG_EA_ERASEIMMEDIATEBLOCK_EXIT
/** \brief Exit point of function Ea_EraseImmediateBlock() */
#define DBG_EA_ERASEIMMEDIATEBLOCK_EXIT(a,b)
#endif

#ifndef DBG_EA_JOBENDNOTIFICATION_ENTRY
/** \brief Entry point of function Ea_JobEndNotification() */
#define DBG_EA_JOBENDNOTIFICATION_ENTRY()
#endif

#ifndef DBG_EA_JOBENDNOTIFICATION_EXIT
/** \brief Exit point of function Ea_JobEndNotification() */
#define DBG_EA_JOBENDNOTIFICATION_EXIT()
#endif

#ifndef DBG_EA_JOBERRORNOTIFICATION_ENTRY
/** \brief Entry point of function Ea_JobErrorNotification() */
#define DBG_EA_JOBERRORNOTIFICATION_ENTRY()
#endif

#ifndef DBG_EA_JOBERRORNOTIFICATION_EXIT
/** \brief Exit point of function Ea_JobErrorNotification() */
#define DBG_EA_JOBERRORNOTIFICATION_EXIT()
#endif

#ifndef DBG_EA_MAINFUNCTION_ENTRY
/** \brief Entry point of function Ea_MainFunction() */
#define DBG_EA_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_EA_MAINFUNCTION_EXIT
/** \brief Exit point of function Ea_MainFunction() */
#define DBG_EA_MAINFUNCTION_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef EA_TRACE_H */
/*==================[end of file]===========================================*/
