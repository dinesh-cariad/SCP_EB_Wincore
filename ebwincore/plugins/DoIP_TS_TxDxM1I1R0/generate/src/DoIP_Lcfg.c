/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

/*==================[inclusions]============================================*/

#include <DoIP_Int.h>           /* Module internal declarations */

#if (DOIP_RTE_USED == STD_ON)
#include <Rte_DoIP.h>
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true'"!][!//
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

/* DoIP wrapper function declarations for RTE generated SVC port interface
   RoutingActivationAuthentication() and RoutingActivationConfirmation(). */
[!LOOP "as:modconf('DoIP')[1]/DoIPConfigSet/*[1]/DoIPRoutingActivation/*"!][!//
[!IF "node:exists(./DoIPRoutingActivationAuthenticationCallback) and node:empty(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationFunc)"!][!//
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationAuthentication_Wrapper
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Authentified,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationResData
);
[!ENDIF!][!//
[!IF "node:exists(./DoIPRoutingActivationConfirmationCallback) and node:empty(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationFunc)"!][!//
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationConfirmation_Wrapper
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Confirmed,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationResData
);
[!ENDIF!][!//
[!ENDLOOP!][!//

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>
[!ENDIF!][!//

/*==================[external constants]====================================*/

#define DOIP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <DoIP_MemMap.h>

CONST(DoIP_User_GetVin_FpType, DOIP_APPL_CONST) DoIP_User_GetVin_Fp =
  [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetVIN/DoIPGetVin)"!] [!//
  &DoIP_User_GetVinDummy; /* Dummy User_GetVinFp */
  [!ELSE!] [!//
  &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetVIN/DoIPGetVin)"!]; /* User_GetVinFp */
  [!ENDIF!] [!//


#if(DOIP_POWER_MODE == STD_ON)
CONST(DoIP_PowerMode_FpType, DOIP_APPL_CONST) DoIP_PowerMode_Fp =
  [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPPowerModeCallback/DoIPPowerMode)"!][!//
  &Rte_Call_DoIP_CBGetPowerMode_GetPowerMode; /* service port interface */
  [!ELSE!] [!//
  &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPPowerModeCallback/DoIPPowerMode)"!]; /* user provided callback function */
  [!ENDIF!] [!//
#endif /* (DOIP_POWER_MODE == STD_ON) */

[!VAR "DoIP_MaxRoutingActivations" = "num:i(count(as:modconf('DoIP')[1]/DoIPConfigSet/*/DoIPRoutingActivation/*))"!][!//
CONST(DoIP_RoutingActivationAuth_FpType, DOIP_APPL_CONST) DoIP_RoutingActivationAuth_FpList[[!"num:i($DoIP_MaxRoutingActivations)"!]U] =
{
[!LOOP "as:modconf('DoIP')[1]/DoIPConfigSet/*[1]/DoIPRoutingActivation/*"!][!//
[!IF "node:exists(./DoIPRoutingActivationAuthenticationCallback)"!][!//
[!IF "node:empty(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationFunc)"!][!//
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true'"!][!//
  &DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationAuthentication_Wrapper, /* SVC interface callback function */
[!ELSE!][!//
  &DoIP_RoutingActivationAuthDummy, /* Dummy DoIP_RoutingActivationAuthFp */
[!ENDIF!][!//
[!ELSE!][!//
  &[!"./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationFunc"!], /* User callback function */
[!ENDIF!][!//
[!ELSE!][!//
  &DoIP_RoutingActivationAuthDummy, /* Dummy DoIP_RoutingActivationAuthFp */
[!ENDIF!][!//
[!ENDLOOP!][!//
};

CONST(DoIP_RoutingActivationConf_FpType, DOIP_APPL_CONST) DoIP_RoutingActivationConf_FpList[[!"num:i($DoIP_MaxRoutingActivations)"!]U] =
{
[!LOOP "as:modconf('DoIP')[1]/DoIPConfigSet/*[1]/DoIPRoutingActivation/*"!][!//
[!IF "node:exists(./DoIPRoutingActivationConfirmationCallback)"!][!//
[!IF "node:empty(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationFunc)"!][!//
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true'"!][!//
  &DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationConfirmation_Wrapper, /* SVC interface callback function */
[!ELSE!][!//
  &DoIP_RoutingActivationConfDummy, /* Dummy DoIP_RoutingActivationConfFp */
[!ENDIF!][!//
[!ELSE!][!//
  &[!"./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationFunc"!], /* User callback function */
[!ENDIF!][!//
[!ELSE!][!//
  &DoIP_RoutingActivationConfDummy, /* Dummy DoIP_RoutingActivationConfFp */
[!ENDIF!][!//
[!ENDLOOP!][!//
};

#if(DOIP_DYN_GID_SYNC_ENABLED == STD_ON)
CONST(DoIP_SyncGid_FpType, DOIP_APPL_CONST) DoIP_User_SyncGid_Fp =
    &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPDynamicGIDMasterSelectionCallback/DoIPDynamicGIDMasterSelection)"!];

CONST(DoIP_GetGid_FpType, DOIP_APPL_CONST) DoIP_User_GetGid_Fp =
    &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetGid/DoIPGetGID)"!];
#endif /* DOIP_DYN_GID_SYNC_ENABLED == STD_ON */

#if(DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON)
CONST(DoIP_VIDReqReceived_FpType, DOIP_APPL_CONST) DoIP_User_VIDRequestReceived_Fp =
    &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPVIDRequestReceivedCallback/DoIPVIDRequestReceived)"!];
#endif /* DOIP_USER_VID_REQ_RECEIVED_CALLBACK == STD_ON */

#if(DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON)
CONST(DoIP_TcpSoConModeChg_FpType, DOIP_APPL_CONST) DoIP_User_TcpSoConModeChg_Fp =
    &[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPTcpSoConModeChgCallback/DoIPTcpSoConModeChg)"!];
#endif /* DOIP_USER_TCP_SOCON_MODECHG_CALLBACK == STD_ON */

[!VAR "DoIP_NumRoutingActivationCallbacks" = "num:i(count(as:modconf('DoIP')[1]/DoIPGeneral/DoIPRoutingActivationCallbackList/*))"!][!//
[!IF "$DoIP_NumRoutingActivationCallbacks > 0"!][!//
CONST(DoIP_RoutingActivationCallback_FpType, DOIP_APPL_CONST)
  DoIP_RoutingActivationCallback_FpList[DOIP_ROUTING_ACTIVATION_CALLBACK_NUM] =
{
[!LOOP "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRoutingActivationCallbackList/*"!][!//
  &[!"."!],
[!ENDLOOP!][!//
};
[!ENDIF!][!//

#define DOIP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <DoIP_MemMap.h>

#define DOIP_START_SEC_CONST_32
#include <DoIP_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
CONST(uint32, DOIP_CONST) DoIP_LcfgSignature = [!"asc:getConfigSignature(as:modconf('DoIP')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

#define DOIP_STOP_SEC_CONST_32
#include <DoIP_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true'"!][!//
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

/* DoIP wrapper functions for RTE generated SVC port interface
   RoutingActivationAuthentication() and RoutingActivationConfirmation().*/
[!LOOP "as:modconf('DoIP')[1]/DoIPConfigSet/*[1]/DoIPRoutingActivation/*"!][!//
[!IF "node:exists(./DoIPRoutingActivationAuthenticationCallback) and node:empty(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationFunc)"!][!//
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationAuthentication_Wrapper
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Authentified,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) AuthenticationResData
)
{
[!IF "(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationReqLength > 0) and (./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationResLength > 0)"!][!//
  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationAuthentication(Authentified, AuthenticationReqData, AuthenticationResData);
[!ELSEIF "(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationReqLength > 0) and (./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationResLength = 0)"!][!//
  TS_PARAM_UNUSED(AuthenticationResData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationAuthentication(Authentified, AuthenticationReqData);
[!ELSEIF "(./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationReqLength = 0) and (./DoIPRoutingActivationAuthenticationCallback/DoIPRoutingActivationAuthenticationResLength > 0)"!][!//
  TS_PARAM_UNUSED(AuthenticationReqData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationAuthentication(Authentified, AuthenticationResData);
[!ELSE!][!//
  TS_PARAM_UNUSED(AuthenticationReqData);
  TS_PARAM_UNUSED(AuthenticationResData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationAuthentication(Authentified);
[!ENDIF!][!//
}
[!ENDIF!][!//

[!IF "node:exists(./DoIPRoutingActivationConfirmationCallback) and node:empty(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationFunc)"!][!//
FUNC(Std_ReturnType, DOIP_CODE) DoIP_Rte_Call_CB[!"node:name(.)"!]_RoutingActivationConfirmation_Wrapper
(
  P2VAR(boolean, AUTOMATIC, DOIP_APPL_DATA) Confirmed,
  P2CONST(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationReqData,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) ConfirmationResData
)
{
[!IF "(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationReqLength > 0) and (./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationResLength > 0)"!][!//
  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationConfirmation(Confirmed, ConfirmationReqData, ConfirmationResData);
[!ELSEIF "(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationReqLength > 0) and (./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationResLength = 0)"!][!//
  TS_PARAM_UNUSED(ConfirmationResData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationConfirmation(Confirmed, ConfirmationReqData);
[!ELSEIF "(./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationReqLength = 0) and (./DoIPRoutingActivationConfirmationCallback/DoIPRoutingActivationConfirmationResLength > 0)"!][!//
  TS_PARAM_UNUSED(ConfirmationReqData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationConfirmation(Confirmed, ConfirmationResData);
[!ELSE!][!//
  TS_PARAM_UNUSED(ConfirmationReqData);
  TS_PARAM_UNUSED(ConfirmationResData);

  /* Call RTE generated function. */
  return Rte_Call_DoIP_CB[!"node:name(.)"!]RoutingActivation_RoutingActivationConfirmation(Confirmed);
[!ENDIF!][!//
}
[!ENDIF!][!//
[!ENDLOOP!][!//
#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>
[!ENDIF!][!//


/*==================[end of file]===========================================*/
[!ENDCODE!]
