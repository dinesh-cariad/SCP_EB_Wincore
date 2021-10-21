/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef EEP_TRACE_H
#define EEP_TRACE_H
/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>

[!ENDIF!]
/*==================[macros]================================================*/

#ifndef DBG_EEP_PROCESSJOBENDNOTIFICATION_ENTRY
/** \brief Entry point of function Eep_ProcessJobEndNotification */
#define DBG_EEP_PROCESSJOBENDNOTIFICATION_ENTRY()
#endif

#ifndef DBG_EEP_PROCESSJOBENDNOTIFICATION_EXIT
/** \brief Exit point of function Eep_ProcessJobEndNotification */
#define DBG_EEP_PROCESSJOBENDNOTIFICATION_EXIT()
#endif

#ifndef DBG_EEP_PROCESSJOBERRORNOTIFICATION_ENTRY
/** \brief Entry point of function Eep_ProcessJobErrorNotification */
#define DBG_EEP_PROCESSJOBERRORNOTIFICATION_ENTRY()
#endif

#ifndef DBG_EEP_PROCESSJOBERRORNOTIFICATION_EXIT
/** \brief Exit point of function Eep_ProcessJobErrorNotification */
#define DBG_EEP_PROCESSJOBERRORNOTIFICATION_EXIT()
#endif

#ifndef DBG_EEP_COPYJOBVALUES_ENTRY
/** \brief Entry point of function Eep_CopyJobValues */
#define DBG_EEP_COPYJOBVALUES_ENTRY(a,b,c,d)
#endif

#ifndef DBG_EEP_COPYJOBVALUES_EXIT
/** \brief Exit point of function Eep_CopyJobValues */
#define DBG_EEP_COPYJOBVALUES_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_EEP_JOBWRITEPROCESS_ENTRY
/** \brief Entry point of function Eep_JobWriteProcess */
#define DBG_EEP_JOBWRITEPROCESS_ENTRY()
#endif

#ifndef DBG_EEP_JOBWRITEPROCESS_EXIT
/** \brief Exit point of function Eep_JobWriteProcess */
#define DBG_EEP_JOBWRITEPROCESS_EXIT()
#endif

#ifndef DBG_EEP_JOBREADPROCESS_ENTRY
/** \brief Entry point of function Eep_JobReadProcess */
#define DBG_EEP_JOBREADPROCESS_ENTRY()
#endif

#ifndef DBG_EEP_JOBREADPROCESS_EXIT
/** \brief Exit point of function Eep_JobReadProcess */
#define DBG_EEP_JOBREADPROCESS_EXIT()
#endif

#ifndef DBG_EEP_JOBERASEPROCESS_ENTRY
/** \brief Entry point of function Eep_JobEraseProcess */
#define DBG_EEP_JOBERASEPROCESS_ENTRY()
#endif

#ifndef DBG_EEP_JOBERASEPROCESS_EXIT
/** \brief Exit point of function Eep_JobEraseProcess */
#define DBG_EEP_JOBERASEPROCESS_EXIT()
#endif

#ifndef DBG_EEP_JOBCOMPAREPROCESS_ENTRY
/** \brief Entry point of function Eep_JobCompareProcess */
#define DBG_EEP_JOBCOMPAREPROCESS_ENTRY()
#endif

#ifndef DBG_EEP_JOBCOMPAREPROCESS_EXIT
/** \brief Exit point of function Eep_JobCompareProcess */
#define DBG_EEP_JOBCOMPAREPROCESS_EXIT()
#endif

#ifndef DBG_EEP_INIT_ENTRY
/** \brief Entry point of function Eep_Init */
#define DBG_EEP_INIT_ENTRY(a)
#endif

#ifndef DBG_EEP_INIT_EXIT
/** \brief Exit point of function Eep_Init */
#define DBG_EEP_INIT_EXIT(a)
#endif

#ifndef DBG_EEP_SETMODE_ENTRY
/** \brief Entry point of function Eep_SetMode */
#define DBG_EEP_SETMODE_ENTRY(a)
#endif

#ifndef DBG_EEP_SETMODE_EXIT
/** \brief Exit point of function Eep_SetMode */
#define DBG_EEP_SETMODE_EXIT(a)
#endif

#ifndef DBG_EEP_READ_ENTRY
/** \brief Entry point of function Eep_Read */
#define DBG_EEP_READ_ENTRY(a,b,c)
#endif

#ifndef DBG_EEP_READ_EXIT
/** \brief Exit point of function Eep_Read */
#define DBG_EEP_READ_EXIT(a,b,c,d)
#endif

#ifndef DBG_EEP_WRITE_ENTRY
/** \brief Entry point of function Eep_Write */
#define DBG_EEP_WRITE_ENTRY(a,b,c)
#endif

#ifndef DBG_EEP_WRITE_EXIT
/** \brief Exit point of function Eep_Write */
#define DBG_EEP_WRITE_EXIT(a,b,c,d)
#endif

#ifndef DBG_EEP_ERASE_ENTRY
/** \brief Entry point of function Eep_Erase */
#define DBG_EEP_ERASE_ENTRY(a,b)
#endif

#ifndef DBG_EEP_ERASE_EXIT
/** \brief Exit point of function Eep_Erase */
#define DBG_EEP_ERASE_EXIT(a,b,c)
#endif

#ifndef DBG_EEP_COMPARE_ENTRY
/** \brief Entry point of function Eep_Compare */
#define DBG_EEP_COMPARE_ENTRY(a,b,c)
#endif

#ifndef DBG_EEP_COMPARE_EXIT
/** \brief Exit point of function Eep_Compare */
#define DBG_EEP_COMPARE_EXIT(a,b,c,d)
#endif

#ifndef DBG_EEP_CANCEL_ENTRY
/** \brief Entry point of function Eep_Cancel */
#define DBG_EEP_CANCEL_ENTRY()
#endif

#ifndef DBG_EEP_CANCEL_EXIT
/** \brief Exit point of function Eep_Cancel */
#define DBG_EEP_CANCEL_EXIT()
#endif

#ifndef DBG_EEP_GETSTATUS_ENTRY
/** \brief Entry point of function Eep_GetStatus */
#define DBG_EEP_GETSTATUS_ENTRY()
#endif

#ifndef DBG_EEP_GETSTATUS_EXIT
/** \brief Exit point of function Eep_GetStatus */
#define DBG_EEP_GETSTATUS_EXIT(a)
#endif

#ifndef DBG_EEP_GETJOBRESULT_ENTRY
/** \brief Entry point of function Eep_GetJobResult */
#define DBG_EEP_GETJOBRESULT_ENTRY()
#endif

#ifndef DBG_EEP_GETJOBRESULT_EXIT
/** \brief Exit point of function Eep_GetJobResult */
#define DBG_EEP_GETJOBRESULT_EXIT(a)
#endif

#ifndef DBG_EEP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Eep_GetVersionInfo */
#define DBG_EEP_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_EEP_GETVERSIONINFO_EXIT
/** \brief Exit point of function Eep_GetVersionInfo */
#define DBG_EEP_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_EEP_MAINFUNCTION_ENTRY
/** \brief Entry point of function Eep_MainFunction */
#define DBG_EEP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_EEP_MAINFUNCTION_EXIT
/** \brief Exit point of function Eep_MainFunction */
#define DBG_EEP_MAINFUNCTION_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef EEP_TRACE_H */
/*==================[end of file]===========================================*/
