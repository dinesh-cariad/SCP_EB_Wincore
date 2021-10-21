/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_TRACE_INTERNAL_H
#define CANTP_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

/* This macro is used to compute the channel index in functions
 * where there is only a pointer available to a structure from
 * the CanTp_Channel variable.
 * According to rule MISRA rule 17.2, this is perfectly valid, as
 * both used pointers point to locations from the same array.
 */
#ifndef DBG_CANTP_GET_CHANNEL_INDEX
#define DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr) ((ChannelPtr) - (CanTp_Channel))
#endif

#ifndef DBG_CANTP_INITVARS_STATE
/** \brief State tracing macro for CanTp_InitVars.State */
#define DBG_CANTP_INITVARS_STATE(a,b)
#endif

#ifndef DBG_CANTP_CHANNEL_STATE_GRP
/** \brief State tracing macro for CanTp_Channel[].State */
#define DBG_CANTP_CHANNEL_STATE_GRP(a,b,c)
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

#endif /* ifndef CANTP_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
