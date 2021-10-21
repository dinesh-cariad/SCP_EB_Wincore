/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Mirror.h> /* Module public API */
#include <Rte_Mirror.h> /* RTE APIs supported */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[internal function declarations]=======================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define MIRROR_START_SEC_CODE
#include <Mirror_MemMap.h>

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.GetDestNetwork,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationGetDestNetwork (P2VAR(NetworkHandleType, AUTOMATIC, RTE_APPL_DATA) network)
{
  *network = Mirror_GetDestNetwork();
  return E_OK;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.GetNetworkHandle,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationGetNetworkHandle (Mirror_NetworkType networkType, uint8 networkId, P2VAR(NetworkHandleType, AUTOMATIC, RTE_APPL_DATA) network)
{
  Std_ReturnType retVal = E_NOT_OK;
  *network = Mirror_GetNetworkHandle(networkType, networkId);
  if(MIRROR_INVALID_NETWORK != *network)
  {
      retVal = E_OK;
  }
  else
  {
      /* Do nothing */
  }
  return retVal;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.GetNetworkId,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationGetNetworkId (NetworkHandleType network, P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) networkId)
{
  Std_ReturnType retVal = E_NOT_OK;
  *networkId = Mirror_GetNetworkId(network);
  if(MIRROR_INVALID_MIRROR_ID != *networkId)
  {
      retVal = E_OK;
  }
  else
  {
      /* Do nothing */
  }
  return retVal;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.GetNetworkType,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationGetNetworkType (NetworkHandleType network, P2VAR(Mirror_NetworkType, AUTOMATIC, RTE_APPL_DATA) networkType)
{
  Std_ReturnType retVal = E_NOT_OK;
  *networkType = Mirror_GetNetworkType(network);
  if((NetworkHandleType)MIRROR_NT_INVALID != *networkType)
  {
      retVal = E_OK;
  }
  else
  {
      /* Do nothing */
  }
  return retVal;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.IsMirrorActive,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationIsMirrorActive (P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) mirrorActive)
{
  *mirrorActive = Mirror_IsMirrorActive();
  return E_OK;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.IsSourceNetworkStarted,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationIsSourceNetworkStarted (NetworkHandleType network, P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) sourceNetworkStarted)
{
  *sourceNetworkStarted = Mirror_IsSourceNetworkStarted(network);
  return E_OK;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.Offline,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationOffline (void)
{
  Mirror_Offline();
  return E_OK;
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.StartSourceNetwork,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationStartSourceNetwork (NetworkHandleType network)
{
  return Mirror_StartSourceNetwork(network);
}

/* !LINKSTO Mirror.ASR44.SWS_Mirror_01033.StopSourceNetwork,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationStopSourceNetwork (NetworkHandleType network)
{
  return Mirror_StopSourceNetwork(network);
}

/* !LINKSTO Mirror.EB.OperationStartAllSourceNetworks,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationStartAllSourceNetworks (void)
{
  return Mirror_StartAllSourceNetworks();
}

/* !LINKSTO Mirror.EB.OperationStopAllSourceNetworks,1 */
FUNC(Std_ReturnType, RTE_CODE) Mirror_OperationStopAllSourceNetworks (void)
{
  return Mirror_StopAllSourceNetworks();
}

#define MIRROR_STOP_SEC_CODE
#include <Mirror_MemMap.h>

/*==================[internal function definitions]=========================*/
