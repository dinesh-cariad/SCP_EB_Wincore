/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \version 6.14.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FEE_TRACE_H
#define FEE_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==================[macros]================================================*/

#ifndef DBG_FEE_INIT_ENTRY
/** \brief Entry point of function Fee_Init() */
#define DBG_FEE_INIT_ENTRY()
#endif

#ifndef DBG_FEE_STATE
/** \brief Change of Fee_State */
#define DBG_FEE_STATE(a,b)
#endif

#ifndef DBG_FEE_INIT_EXIT
/** \brief Exit point of function Fee_Init() */
#define DBG_FEE_INIT_EXIT()
#endif

#ifndef DBG_FEE_SETMODE_ENTRY
/** \brief Entry point of function Fee_SetMode() */
#define DBG_FEE_SETMODE_ENTRY(a)
#endif

#ifndef DBG_FEE_SETMODE_EXIT
/** \brief Exit point of function Fee_SetMode() */
#define DBG_FEE_SETMODE_EXIT(a)
#endif

#ifndef DBG_FEE_READ_ENTRY
/** \brief Entry point of function Fee_Read() */
#define DBG_FEE_READ_ENTRY(a,b,c,d)
#endif

#ifndef DBG_FEE_READ_EXIT
/** \brief Exit point of function Fee_Read() */
#define DBG_FEE_READ_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_FEE_WRITE_ENTRY
/** \brief Entry point of function Fee_Write() */
#define DBG_FEE_WRITE_ENTRY(a,b)
#endif

#ifndef DBG_FEE_WRITE_EXIT
/** \brief Exit point of function Fee_Write() */
#define DBG_FEE_WRITE_EXIT(a,b,c)
#endif

#ifndef DBG_FEE_WRITE_CUSTOM_ENTRY
/** \brief Entry point of function Fee_WriteCustom() */
#define DBG_FEE_WRITE_CUSTOM_ENTRY(a,b,c)
#endif

#ifndef DBG_FEE_WRITE_CUSTOM_EXIT
/** \brief Exit point of function Fee_WriteCustom() */
#define DBG_FEE_WRITE_CUSTOM_EXIT(a,b,c,d)
#endif

#ifndef DBG_FEE_READ_CUSTOM_ENTRY
/** \brief Entry point of function Fee_ReadCustom() */
#define DBG_FEE_READ_CUSTOM_ENTRY(a,b,c,d)
#endif

#ifndef DBG_FEE_READ_CUSTOM_EXIT
/** \brief Exit point of function Fee_ReadCustom() */
#define DBG_FEE_READ_CUSTOM_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_FEE_CANCEL_ENTRY
/** \brief Entry point of function Fee_Cancel() */
#define DBG_FEE_CANCEL_ENTRY()
#endif

#ifndef DBG_FEE_CANCEL_EXIT
/** \brief Exit point of function Fee_Cancel() */
#define DBG_FEE_CANCEL_EXIT()
#endif

#ifndef DBG_FEE_GETSTATUS_ENTRY
/** \brief Entry point of function Fee_GetStatus() */
#define DBG_FEE_GETSTATUS_ENTRY()
#endif

#ifndef DBG_FEE_GETSTATUS_EXIT
/** \brief Exit point of function Fee_GetStatus() */
#define DBG_FEE_GETSTATUS_EXIT(a)
#endif

#ifndef DBG_FEE_GETJOBRESULT_ENTRY
/** \brief Entry point of function Fee_GetJobResult() */
#define DBG_FEE_GETJOBRESULT_ENTRY()
#endif

#ifndef DBG_FEE_GETJOBRESULT_EXIT
/** \brief Exit point of function Fee_GetJobResult() */
#define DBG_FEE_GETJOBRESULT_EXIT(a)
#endif

#ifndef DBG_FEE_INVALIDATEBLOCK_ENTRY
/** \brief Entry point of function Fee_InvalidateBlock() */
#define DBG_FEE_INVALIDATEBLOCK_ENTRY(a)
#endif

#ifndef DBG_FEE_INVALIDATEBLOCK_EXIT
/** \brief Exit point of function Fee_InvalidateBlock() */
#define DBG_FEE_INVALIDATEBLOCK_EXIT(a,b)
#endif

#ifndef DBG_FEE_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Fee_GetVersionInfo() */
#define DBG_FEE_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_FEE_GETVERSIONINFO_EXIT
/** \brief Exit point of function Fee_GetVersionInfo() */
#define DBG_FEE_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_FEE_ERASEIMMEDIATEBLOCK_ENTRY
/** \brief Entry point of function Fee_EraseImmediateBlock() */
#define DBG_FEE_ERASEIMMEDIATEBLOCK_ENTRY(a)
#endif

#ifndef DBG_FEE_ERASEIMMEDIATEBLOCK_EXIT
/** \brief Exit point of function Fee_EraseImmediateBlock() */
#define DBG_FEE_ERASEIMMEDIATEBLOCK_EXIT(a,b)
#endif

#ifndef DBG_FEE_JOBENDNOTIFICATION_ENTRY
/** \brief Entry point of function Fee_JobEndNotification() */
#define DBG_FEE_JOBENDNOTIFICATION_ENTRY()
#endif

#ifndef DBG_FEE_JOBENDNOTIFICATION_EXIT
/** \brief Exit point of function Fee_JobEndNotification() */
#define DBG_FEE_JOBENDNOTIFICATION_EXIT()
#endif

#ifndef DBG_FEE_JOBERRORNOTIFICATION_ENTRY
/** \brief Entry point of function Fee_JobErrorNotification() */
#define DBG_FEE_JOBERRORNOTIFICATION_ENTRY()
#endif

#ifndef DBG_FEE_JOBERRORNOTIFICATION_EXIT
/** \brief Exit point of function Fee_JobErrorNotification() */
#define DBG_FEE_JOBERRORNOTIFICATION_EXIT()
#endif

#ifndef DBG_FEE_MAINFUNCTION_ENTRY
/** \brief Entry point of function Fee_MainFunction() */
#define DBG_FEE_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_FEE_NEXTSTATE
/** \brief Change of Fee_NextState */
#define DBG_FEE_NEXTSTATE(a,b)
#endif

#ifndef DBG_FEE_MAINFUNCTION_EXIT
/** \brief Exit point of function Fee_MainFunction() */
#define DBG_FEE_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_FEE_GETERASECOUNTERVALUE_ENTRY
/** \brief Entry point of function Fee_GetEraseCounterValue() */
#define DBG_FEE_GETERASECOUNTERVALUE_ENTRY()
#endif

#ifndef DBG_FEE_GETERASECOUNTERVALUE_EXIT
/** \brief Exit point of function Fee_GetEraseCounterValue() */
#define DBG_FEE_GETERASECOUNTERVALUE_EXIT()
#endif

#ifndef DBG_FEE_FREEZEACTIVITIES_ENTRY
/** \brief Entry point of function Fee_FreezeActivities() */
#define DBG_FEE_FREEZEACTIVITIES_ENTRY()
#endif

#ifndef DBG_FEE_FREEZEACTIVITIES_EXIT
/** \brief Exit point of function Fee_FreezeActivities() */
#define DBG_FEE_FREEZEACTIVITIES_EXIT()
#endif

#ifndef DBG_FEE_CANCELSECTIONERASE_ENTRY
/** \brief Entry point of function Fee_CancelSectionErase() */
#define DBG_FEE_CANCELSECTIONERASE_ENTRY()
#endif

#ifndef DBG_FEE_CANCELSECTIONERASE_EXIT
/** \brief Exit point of function Fee_CancelSectionErase() */
#define DBG_FEE_CANCELSECTIONERASE_EXIT(a)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef FEE_TRACE_H */
/*==================[end of file]===========================================*/
