/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef WDGIF_TRACE_H
#define WDGIF_TRACE_H
/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>
[!ENDIF!]

/*==================[macros]================================================*/

#ifndef DBG_WDGIF_SETTRIGGERCONDITION_ENTRY
/** \brief Entry point of function WdgIf_SetTriggerCondition() */
#define DBG_WDGIF_SETTRIGGERCONDITION_ENTRY(a,b)
#endif

#ifndef DBG_WDGIF_SETTRIGGERCONDITION_EXIT
/** \brief Exit point of function WdgIf_SetTriggerCondition() */
#define DBG_WDGIF_SETTRIGGERCONDITION_EXIT(a,b)
#endif

#ifndef DBG_WDGIF_SETMODE_ENTRY
/** \brief Entry point of function WdgIf_SetMode() */
#define DBG_WDGIF_SETMODE_ENTRY(a,b)
#endif

#ifndef DBG_WDGIF_SETMODE_EXIT
/** \brief Exit point of function WdgIf_SetMode() */
#define DBG_WDGIF_SETMODE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGIF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function WdgIf_GetVersionInfo() */
#define DBG_WDGIF_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_WDGIF_GETVERSIONINFO_EXIT
/** \brief Exit point of function WdgIf_GetVersionInfo() */
#define DBG_WDGIF_GETVERSIONINFO_EXIT(a)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef WDGIF_TRACE_H */
/*==================[end of file]===========================================*/
