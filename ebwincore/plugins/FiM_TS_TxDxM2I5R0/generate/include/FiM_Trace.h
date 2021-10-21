/**
 * \file
 *
 * \brief AUTOSAR FiM
 *
 * This file contains the implementation of the AUTOSAR
 * module FiM.
 *
 * \version 2.5.3
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FIM_TRACE_H
#define FIM_TRACE_H

[!AUTOSPACING!]
[!//
/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>

[!ENDIF!]
/*==================[macros]================================================*/

#ifndef DBG_FIM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FiM_GetVersionInfo() */
#define DBG_FIM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_FIM_GETVERSIONINFO_EXIT
/** \brief Exit point of function FiM_GetVersionInfo() */
#define DBG_FIM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_FIM_INIT_ENTRY
/** \brief Entry point of function FiM_Init() */
#define DBG_FIM_INIT_ENTRY(a)
#endif

#ifndef DBG_FIM_INIT_EXIT
/** \brief Exit point of function FiM_Init() */
#define DBG_FIM_INIT_EXIT(a)
#endif

#ifndef DBG_FIM_MAINFUNCTION_ENTRY
/** \brief Entry point of function FiM_MainFunction() */
#define DBG_FIM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_FIM_MAINFUNCTION_EXIT
/** \brief Exit point of function FiM_MainFunction() */
#define DBG_FIM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_FIM_GETFUNCTIONPERMISSION_ENTRY
/** \brief Entry point of function FiM_GetFunctionPermission() */
#define DBG_FIM_GETFUNCTIONPERMISSION_ENTRY(a, b)
#endif

#ifndef DBG_FIM_GETFUNCTIONPERMISSION_EXIT
/** \brief Exit point of function FiM_GetFunctionPermission() */
#define DBG_FIM_GETFUNCTIONPERMISSION_EXIT(a, b, c)
#endif

#ifndef DBG_FIM_DEMTRIGGERONEVENTSTATUS_ENTRY
/** \brief Entry point of function FiM_DemTriggerOnEventStatus() */
#define DBG_FIM_DEMTRIGGERONEVENTSTATUS_ENTRY(a, b, c)
#endif

#ifndef DBG_FIM_DEMTRIGGERONEVENTSTATUS_EXIT
/** \brief Exit point of function FiM_DemTriggerOnEventStatus() */
#define DBG_FIM_DEMTRIGGERONEVENTSTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_FIM_DEMTRIGGERONUNCONFIRMED_ENTRY
/** \brief Entry point of function FiM_DemTriggerOnUnconfirmed() */
#define DBG_FIM_DEMTRIGGERONUNCONFIRMED_ENTRY(a)
#endif

#ifndef DBG_FIM_DEMTRIGGERONUNCONFIRMED_EXIT
/** \brief Exit point of function FiM_DemTriggerOnUnconfirmed() */
#define DBG_FIM_DEMTRIGGERONUNCONFIRMED_EXIT(a)
#endif

#ifndef DBG_FIM_GETMESSAGESTATUS_ENTRY
/** \brief Entry point of function FiM_GetMessageStatus() */
#define DBG_FIM_GETMESSAGESTATUS_ENTRY(a, b)
#endif

#ifndef DBG_FIM_GETMESSAGESTATUS_EXIT
/** \brief Exit point of function FiM_GetMessageStatus() */
#define DBG_FIM_GETMESSAGESTATUS_EXIT(a, b, c)
#endif

#ifndef DBG_FIM_DEMINIT_ENTRY
/** \brief Entry point of function FiM_DemInit() */
#define DBG_FIM_DEMINIT_ENTRY()
#endif

#ifndef DBG_FIM_DEMINIT_EXIT
/** \brief Exit point of function FiM_DemInit() */
#define DBG_FIM_DEMINIT_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef FIM_TRACE_H */
/*==================[end of file]===========================================*/
