/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.DslCommunicationServices.Impl,1 */
/* This file contains the implementation of the Public APIs needed for the communication modules. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 16.7 (advisory)
 *     "A pointer parameter in a function prototype should be declared as pointer to const if
 *      the pointer is not used to modify the addressed object."
 *
 *     Reason:
 *     AUTOSAR SWS 4.0.3 specifies the parameter as non-const. Since the function defined here
 *     is a dummy one, the pointer parameter is unused.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.13 (advisory)
  *     "A pointer should point to a const-qualifed type whenever possible."
  *
  *     Reason:
  *     AUTOSAR SWS 4.0.3 specifies the parameter as non-const. Since the function defined here
  *     is a dummy one, the pointer parameter is unused.
  *
  *  MISRAC2012-2) Deviated Rule: 16.1 (required)
  *     "All switch statements shall be well-formed"
  *
  *     Reason:
  *     This violation is present because rule 16.4 is violated and justified inside the affected
  *     switch statement, as noted under rule 16.1:
  *     some of the restrictions imposed on switch statements by this rule are expounded in the
  *     15.3, 16.2, 16.3, 16.4, 16.5, 16.6 rules, which are violated inside the current switch statement.
  *
  *  MISRAC2012-3) Deviated Rule: 16.4 (required)
  *     "Every switch statement shall have a default label"
  *
  *     Reason:
  *     On the default case no action is established for the connections that are in Idle state
  *     or an unknown value which could have corrupted the connection state variable.
  */

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                               /* AUTOSAR Communication Stack types */
#include <TSAutosar.h>
#include <TSMem.h>

#include <Dcm_Dsl_CommunicationServices.h>                /* Dcm Dsl Communication Services header
                                                             file */
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>  /* Dcm Dsl RxConnections header file */
#include <Dcm_Dsl_CommunicationServices_TxConnections.h>  /* Dcm Dsl TxConnections header file */
#include <Dcm_Dsl_CommunicationServices_BufferManager.h>  /* Dcm Dsl BufferManager header file */

#include <Dcm_Dsl_ComMChannelStateManager.h>              /* Dcm Dsl CComMChannelStateManager header
                                                             file */
#include <Dcm_Dsl_Supervisor.h>                           /* Dcm Dsl Supervisor header file */

#include <Dcm.h>                                          /* Definition of DCM_MODULE_ID */
#include <Dcm_Int.h>                                      /* Module intenal definitions */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <TSMem.h>                                        /* EB specific memory functions */
#include <SchM_Dcm.h>                                     /* SchM interface for Dcm */
#include <PduR_Dcm.h>                                     /* PduR interface for Dcm */
#include <ComM_Dcm.h>                                     /* ComM interface for Dcm */
#include <Dcm_Trace.h>
#include <Dcm_Cbk.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

FUNC(void, DCM_CODE) Dcm_Dsl_Init(void)
{
  DBG_DCM_DSL_INIT_ENTRY();

  Dcm_Dsl_BufferInit();

  Dcm_Dsl_RxConnectionInit();

  Dcm_Dsl_TxConnectionInit();

  Dcm_ProcessTxConnectionsOnMainFunction = FALSE;

  DBG_DCM_DSL_INIT_EXIT();
}

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
FUNC(Dcm_DemClientIdType, DCM_CODE) Dcm_Dsl_GetDemClientId(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  /* !LINKSTO Dcm.DcmDslProtocolRow.DcmDemClientRef,1 */
  Dcm_DemClientIdType ClientId = 0;

  DBG_DCM_DSL_GETDEMCLIENTID_ENTRY(ProtocolIndex);

  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIndex, DCM_INTERNAL_API_ID);

  ClientId = Dcm_ProtocolConfig[ProtocolIndex].DemClientId;

  DBG_DCM_DSL_GETDEMCLIENTID_EXIT(ClientId, ProtocolIndex);

  return ClientId;
}
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

/*==================[end of file]================================================================*/
