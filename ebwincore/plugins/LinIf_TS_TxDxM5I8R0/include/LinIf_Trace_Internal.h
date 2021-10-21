/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINIF_TRACE_INTERNAL_H
#define LINIF_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_LINIF_INITSTATE
/** \brief Change of LinIf_InitState */
#define DBG_LINIF_INITSTATE(a,b)
#endif

#ifndef DBG_LINTP_INITSTATE
/** \brief Change of LinTp_InitState */
#define DBG_LINTP_INITSTATE(a,b)
#endif

#ifndef DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP
#define DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_STATE_GRP(a,b,c)
#endif

#ifndef DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP
#define DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_SID_GRP(a,b,c)
#endif

#ifndef DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP
#define DBG_LINTP_CHANNELINFO_PHYSCHANNELINFO_NAD_GRP(a,b,c)
#endif

#ifndef DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP
#define DBG_LINTP_CHANNELINFO_FUNCCHANNELINFO_STATE_GRP(a,b,c)
#endif

#ifndef DBG_LINIF_SCHCURRENTTABLE_GRP
#define DBG_LINIF_SCHCURRENTTABLE_GRP(a,b,c)
#endif

#ifndef DBG_LINIF_CFSM_GRP
#define DBG_LINIF_CFSM_GRP(a,b,c)
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

#endif /* ifndef LINIF_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
