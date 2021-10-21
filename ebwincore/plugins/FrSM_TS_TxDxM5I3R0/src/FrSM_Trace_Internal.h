/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRSM_TRACE_INTERNAL_H
#define FRSM_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

#include <FrSM_guards.h> /* After External/API/Cfg, before Trace/internal/private */

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_FRSM_PASSIVESTATE
/** \brief Change of FrSM_PassiveState */
#define DBG_FRSM_PASSIVESTATE(a,b)
#endif

#ifndef DBG_FRSM_INITSTATUS
/** \brief Change of FrSM_InitStatus */
#define DBG_FRSM_INITSTATUS(a,b)
#endif

#ifndef DBG_FRSM_REQUESTEDSTATE_GRP
#define DBG_FRSM_REQUESTEDSTATE_GRP(a,b,c)
#endif

#ifndef DBG_FRSM_STATE_GRP
#define DBG_FRSM_STATE_GRP(a,b,c)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef FRSM_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
