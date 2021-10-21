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

/* !LINKSTO Dcm.Dsn.File.CommunicationControl.ConfigC,1 */

/*==================[inclusions]=================================================================*/
[!AUTOSPACING!]
#include <Dcm_CommunicationControl_Cfg.h>
[!INDENT "0"!]
[!INCLUDE "../include/Dcm_common.m"!]
[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
#include <SchM_Dcm.h>
[!ENDIF!]

[!IF "$DCM_DSP_USE_SERVICE_0X28 = 'STD_ON'"!]
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

[!IF "$DCM_MODEDECLARATION_SUPPORT = 'STD_ON'"!]
#if (DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL > 0U)

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

  /* Array of configuration information for Communication Control All Channels */
  CONST(Dcm_CommunicationControlAllChannelType, DCM_CONST) Dcm_CommunicationControlAllChannel[DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL] =
  {
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlAllChannel/*"!]
      [!WS "2"!]/* Network Id */
      [!WS "2"!][!"as:ref(DcmDspAllComMChannelRef)/ComMChannelId"!]U,
    [!ENDLOOP!]
  };

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>
#endif

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0U)
  /* Array of configuration information for Communication Control All Channels */
  CONST(Dcm_CommunicationControlSpecificChannelType, DCM_CONST) Dcm_CommunicationControlSpecificChannel[DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL] =
  {
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSpecificChannel/*"!][!//
    [!WS "2"!]{
      [!INDENT "4"!]
      /* Network Id */
      [!"as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId"!]U,
      /* Subnet Number which controls the specific ComMChannel */
      [!"DcmDspSubnetNumber"!]U,
    [!ENDINDENT!]
    [!WS "2"!]},
    [!ENDLOOP!][!//
  };
#endif

#if (DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL > 0U)
  /* Array of configuration information for Communication Control SubNode Channels */
  CONST(Dcm_CommunicationControlSubNodeChannelType, DCM_CONST) Dcm_CommunicationControlSubNodeChannel[DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL] =
  {
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSubNode/*"!][!//
    [!WS "2"!]{
      [!INDENT "4"!]
      /* SubNode identification number */
      [!"DcmDspComControlSubNodeId"!]U,
      /* Network identification number */
      [!"as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId"!]U,
    [!ENDINDENT!]
    [!WS "2"!]},
    [!ENDLOOP!][!//
  };
#endif

#if (DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U)
  /* Array of ComMChannels and SchM Switch pointers for CommunicationControl */
  CONST(Dcm_CommunicationControlLookupType, DCM_CONST) Dcm_CommunicationControlLookUpTable[DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE] =
  {
    [!VAR "ChannelList"="'#'"!][!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
      [!IF "not( contains( $ChannelList, concat( '#', ./DcmDslProtocolRxChannelId,'#')))"!]
      [!WS "2"!]{
      [!INDENT "4"!]
        /* Pointer to SchM Switch for DcmCommunicationControl */
        &SchM_Switch_Dcm_DcmCommunicationControl_[!"name(as:ref(DcmDslProtocolRxComMChannelRef))"!],
        /* Network Id */
        [!"DcmDslProtocolRxChannelId"!]U,
      [!ENDINDENT!]
      [!WS "2"!]},
        [!VAR "ChannelList" = "concat( $ChannelList,'#',./DcmDslProtocolRxChannelId,'#')"!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
[!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlAllChannel/*"!]
      [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspAllComMChannelRef)/ComMChannelId, '#')))"!]
      [!WS "2"!]{
      [!INDENT "4"!]
        /* pointer to SchM Switch for DcmCommunicationControl */
        &SchM_Switch_Dcm_DcmCommunicationControl_[!"name(as:ref(./DcmDspAllComMChannelRef))"!],
        /* Network Id */
        [!"as:ref(DcmDspAllComMChannelRef)/ComMChannelId"!]U,
      [!ENDINDENT!]
      [!WS "2"!]},
        [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspAllComMChannelRef)/ComMChannelId, '#')"!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
[!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSpecificChannel/*"!]
      [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId, '#')))"!]
      [!WS "2"!]{
      [!INDENT "4"!]
         /* pointer to SchM Switch for DcmCommunicationControl */
        &SchM_Switch_Dcm_DcmCommunicationControl_[!"name(as:ref(./DcmDspSpecificComMChannelRef))"!],
        /* Network Id */
        [!"as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId"!]U,
       [!ENDINDENT!]
      [!WS "2"!]},
        [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspSpecificComMChannelRef)/ComMChannelId, '#')"!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
[!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSubNode/*"!]
      [!IF "not(contains($ChannelList, concat('#', as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId, '#')))"!]
      [!WS "2"!]{
      [!INDENT "4"!]
        /* pointer to SchM Switch for DcmCommunicationControl */
        &SchM_Switch_Dcm_DcmCommunicationControl_[!"name(as:ref(./DcmDspComControlSubNodeComMChannelRef))"!],
        /* Network Id */
        [!"as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId"!]U,
      [!ENDINDENT!]
      [!WS "2"!]},
        [!VAR "ChannelList" = "concat($ChannelList, as:ref(DcmDspComControlSubNodeComMChannelRef)/ComMChannelId, '#')"!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  };
#endif /* #if (DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U) */

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSetting/DcmDspComControlCommunicationReEnableModeRuleRef)"!][!//
/* Constant holding the pointer to the configured mode rule to be used when resetting Communication Control */
CONST(Dcm_ModeRuleFunctionType, DCM_CONST) Dcm_Dsp_CommunicationControl_ModeRuleFunction =
  [!WS "2"!]&Dcm_ModeRule_[!"name(node:ref(DcmConfigSet/*[1]/DcmDsp/DcmDspComControl/DcmDspComControlSetting/DcmDspComControlCommunicationReEnableModeRuleRef))"!]_Result;
[!ENDIF!][!//

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
[!//
[!ELSE!][!//
#if (DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U)

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>
  /* Array holding the ComM channels ID referenced from RxConnections */
CONST(NetworkHandleType, DCM_CONST) Dcm_CommunicationControlLookUpTable[DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE] =
  {
    [!VAR "ChannelList"="'#'"!][!//
    [!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
      [!IF "not( contains( $ChannelList, concat( '#', ./DcmDslProtocolRxChannelId,'#')))"!]
      [!INDENT "2"!]
        /* Network Id */
        [!"DcmDslProtocolRxChannelId"!]U,
      [!ENDINDENT!]
        [!VAR "ChannelList" = "concat( $ChannelList,'#',./DcmDslProtocolRxChannelId,'#')"!]
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  };

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>
#endif /* #if (DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE > 0U) */

[!ENDIF!][!// DCM_MODEDECLARATION_SUPPORT

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/
[!ENDIF!][!// DCM_DSP_USE_SERVICE_0X28
[!ENDINDENT!]

/*==================[end of file]================================================================*/
