/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
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

#include <EthIf_Lcfg.h>        /* Corresponding header file */
[!INCLUDE "../../generate_macros/EthIf_LinkTime.m"!][!//
#include <EthIf_Int.h> /* Module internal declarations */
#include <EthIf.h>  /* Module declarations */

#include <EthSM_Cbk.h>         /* EthSM callback API */
#include <TcpIp_Cbk.h>         /* TcpIp EthIf callback API */
[!LOOP "EthIfGeneral/EthIfPublicCddHeaderFile/*"!][!//
#include <[!"."!]>   /* CDD header file */
[!ENDLOOP!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

CONST(EthIf_Up_RxIndication_FpType, ETHIF_APPL_CONST) EthIf_Up_RxIndication_FpList[[!"num:i($EthIf_MaxFrameTypeOwner + 1)"!]U] =
{
  [!FOR "I"="1" TO "$EthIf_MaxFrameTypeOwner + 1"!] [!// Caution: Counts till <=
  [!// +1 entry - Last entry points to dummy functions and can be used as invalid value.
    [!IF "$I <= $EthIf_MaxFrameTypeOwner"!] [!//
    &[!"string(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfRxIndicationConfig/*[num:i($I)]/EthIfRxIndicationFunction)"!], /* Up_RxIndicationFp */
    [!ELSE!] [!//
    &EthIf_Up_RxIndicationDummy /* Up_RxIndicationFp: Last entry */
    [!ENDIF!] [!// $I < EthIf_MaxUpRxIndication
  [!ENDFOR!] [!//
};

CONST(EthIf_Up_TxConfirmation_FpType, ETHIF_APPL_CONST) EthIf_Up_TxConfirmation_FpList[[!"num:i($EthIf_MaxUpTxConfirmation + 1)"!]U] =
{
  [!FOR "I"="1" TO "$EthIf_MaxUpTxConfirmation + 1"!][!// Caution: Counts till <=
  [!// +1 entry - Last entry points to dummy functions and can be used as invalid value.
    [!IF "$I <= $EthIf_MaxUpTxConfirmation"!][!//
    &[!"string(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfTxConfirmationConfig/*[num:i($I)]/EthIfTxConfirmationFunction)"!], /* Up_TxConfirmationFp */
    [!ELSE!][!//
    &EthIf_Up_TxConfirmationDummy /* Up_TxConfirmationFp: Last entry */
    [!ENDIF!][!// $I < EthIf_MaxUpTxConfirmation
  [!ENDFOR!][!//
};

CONST(EthIf_Up_TrcvLinkStateChg_FpType, ETHIF_APPL_CONST) EthIf_Up_TrcvLinkStateChg_FpList[[!"num:i($EthIf_MaxUpTrcvLinkStateChg + 1)"!]U] =
{
  [!FOR "I"="1" TO "$EthIf_MaxUpTrcvLinkStateChg + 1"!][!// Caution: Counts till <=
  [!// +1 entry - Last entry points to dummy functions and can be used as invalid value.
   [!IF "$I <= $EthIf_MaxUpTrcvLinkStateChg"!][!//
    &[!"string(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfTrcvLinkStateChgConfig/*[num:i($I)]/EthIfTrcvLinkStateChgFunction)"!], /* Up_TrcvLinkStateChgFp */
    [!ELSE!][!//
    &EthIf_Up_TrcvLinkStateChgDummy, /* Up_TrcvLinkStateChgFp */
    [!ENDIF!][!// $I < $EthIf_MaxUpTrcvLinkStateChg
  [!ENDFOR!][!//
};

#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CONFIG_DATA_8
#include <EthIf_MemMap.h>

CONST(uint8, ETHIF_APPL_CONST) EthIf_Up_TxConfirmation_MaxNum = [!"num:i($EthIf_MaxUpTxConfirmation)"!]U;

CONST(uint8, ETHIF_APPL_CONST) EthIf_Up_TrcvLinkStateChg_MaxNum = [!"num:i($EthIf_MaxUpTrcvLinkStateChg)"!]U;

CONST(uint8, ETHIF_APPL_CONST) EthIf_FrameTypeOwner_MaxNum = [!"num:i($EthIf_MaxFrameTypeOwner)"!]U;

#define ETHIF_STOP_SEC_CONFIG_DATA_8
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CONFIG_DATA_16
#include <EthIf_MemMap.h>

CONST(uint16, ETHIF_APPL_CONST) EthIf_LOT_FrameTypeOwnerToFrameType[[!"num:i($EthIf_MaxFrameTypeOwner)"!]U] =
{
  [!LOOP "node:order(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfFrameOwnerConfig/*,'node:value(EthIfOwner)')"!][!//
    [!"node:value(EthIfFrameType)"!]U, /* FrameType */
  [!ENDLOOP!][!//
};

#define ETHIF_STOP_SEC_CONFIG_DATA_16
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CONST_32
#include <EthIf_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
CONST(uint32, ETHIF_CONST) EthIf_LcfgSignature = [!"asc:getConfigSignature(as:modconf('EthIf')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

#define ETHIF_STOP_SEC_CONST_32
#include <EthIf_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>

/** \brief Mapping of controller specific main function Rx function to generic functions
 */
[!VAR "count" = "0"!]
[!LOOP "node:order(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfPhysController/*, 'EthIfPhysControllerIdx')"!][!//
  [!LOOP "./EthIfPhysCtrlRxMainFunctionPriorityProcessing/*"!][!//
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx_[!"node:name(.)"!](void)
{
  EthIf_MainFunctionRxPrio( [!"num:i($count)"!]U);
[!VAR "count" = "$count + 1"!]
}
  [!ENDLOOP!][!//
[!ENDLOOP!][!//

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]
