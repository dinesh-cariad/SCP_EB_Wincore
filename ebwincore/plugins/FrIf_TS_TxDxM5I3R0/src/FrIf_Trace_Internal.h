/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRIF_TRACE_INTERNAL_H
#define FRIF_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

#include <FrIf_guards.h> /* Include define guards */

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_FRIF_MODULEINITIALIZED
/** \brief Change of FrIf_ModuleInitialized */
#define DBG_FRIF_MODULEINITIALIZED(a,b)
#endif

#ifndef DBG_FRIF_STATE_GRP
#define DBG_FRIF_STATE_GRP(a,b,c)
#endif

#ifndef DBG_FRIF_SYNCSTATE_GRP
#define DBG_FRIF_SYNCSTATE_GRP(a,b,c)
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

#endif /* ifndef FRIF_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
