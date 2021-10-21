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

[!INCLUDE "../../generate_macros/EthIf_Cfg.m"!][!//
#include <EthIf_Int.h> /* Module internal declarations */
#include <EthIf.h>  /* Module declarations */
#include <EthIf_DrvTypes.h>

[!LOOP "EthIfGeneral/EthIfPublicCddHeaderFile/*"!][!//
#include <[!"."!]>   /* CDD header file */
[!ENDLOOP!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

[!IF "$EthIf_CtrlBswRefNr > 1"!][!//
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

/** \brief Configuration of Eth controller(s) function pointers */
CONST( EthIf_EthFuncConfigType, ETHIF_APPL_CONST ) EthIf_EthFuncConfig[[!"num:i($EthIf_CtrlBswRefNr)"!]U] =
{
[!FOR "i"="1" TO "$EthIf_CtrlBswRefNr"!][!//
  [!VAR "driverVendorId" = "asc:getVendorId(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*[num:i($i)]/EthIfEthControllerBswmdImplementationRefs)))"!][!//
  [!VAR "driverApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*[num:i($i)]/EthIfEthControllerBswmdImplementationRefs)))"!][!//
  [!VAR "driverInfo" = "concat($driverVendorId,'_', $driverApiInfix, '_')"!][!//
  {
    &Eth_[!"$driverInfo"!]GetControllerMode,
    &Eth_[!"$driverInfo"!]GetPhysAddr,
    &Eth_[!"$driverInfo"!]ProvideTxBuffer,
[!IF "$EthIf_MiiApiEnable = 'true'"!][!//
    &Eth_[!"$driverInfo"!]ReadMii,
    &Eth_[!"$driverInfo"!]WriteMii,
[!ENDIF!][!//
    &Eth_[!"$driverInfo"!]Receive,
    &Eth_[!"$driverInfo"!]SetControllerMode,
    &Eth_[!"$driverInfo"!]Transmit,
    &Eth_[!"$driverInfo"!]TxConfirmation,
    &Eth_[!"$driverInfo"!]SetPhysAddr,
[!IF "$EthIf_UpdatePhysAddrFilterApiEnable = 'true'"!][!//
    &Eth_[!"$driverInfo"!]UpdatePhysAddrFilter,
[!ENDIF!][!//
[!IF "$EthIf_GlobalTimeApiEnable = 'true'"!][!//
    &Eth_[!"$driverInfo"!]GetCurrentTime,
    &Eth_[!"$driverInfo"!]EnableEgressTimeStamp,
    &Eth_[!"$driverInfo"!]GetEgressTimeStamp,
    &Eth_[!"$driverInfo"!]GetIngressTimeStamp,
    &Eth_[!"$driverInfo"!]SetCorrectionTime,
    &Eth_[!"$driverInfo"!]SetGlobalTime,
[!ENDIF!][!//
[!IF "$EthIf_EthIfRetransmitApiEnable = 'true'"!]
    &Eth_[!"$driverInfo"!]Retransmit
[!ENDIF!][!//
  },
[!ENDFOR!][!//
};
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>
[!ENDIF!][!//

[!IF "$EthIf_TrcvBswRefNr > 1"!][!//
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

  /** \brief Configuration of Eth transceiver(s) function pointers */
  CONST( EthIf_EthTrcvFuncConfigType, ETHIF_APPL_CONST ) EthIf_EthTrcvFuncConfig[[!"num:i($EthIf_TrcvBswRefNr)"!]U] =
  {
[!FOR "i"="1" TO "$EthIf_TrcvBswRefNr"!][!//
  [!VAR "trcvVendorId" = "asc:getVendorId(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*[num:i($i)]/EthIfEthTrcvBswmdImplementationRefs)))"!]
  [!VAR "trcvApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*[num:i($i)]/EthIfEthTrcvBswmdImplementationRefs)))"!]
  [!VAR "trcvInfo" = "concat($trcvVendorId,'_', $trcvApiInfix, '_')"!][!//
  {
    &EthTrcv_[!"$trcvInfo"!]SetPhyLoopbackMode,
    &EthTrcv_[!"$trcvInfo"!]SetPhyTxMode,
    &EthTrcv_[!"$trcvInfo"!]GetPhySignalQuality,
    &EthTrcv_[!"$trcvInfo"!]GetCableDiagnosticsResult,
    &EthTrcv_[!"$trcvInfo"!]GetPhyIdentifier,
    &EthTrcv_[!"$trcvInfo"!]SetPhyTestMode,
[!IF "$EthIf_WakeUpSupportApi = 'true'"!][!//
    &EthTrcv_[!"$trcvInfo"!]SetTransceiverWakeupMode,
    &EthTrcv_[!"$trcvInfo"!]CheckWakeup,
[!IF "$EthIf_GetTransceiverWakeupModeApi = 'true'"!][!//
    &EthTrcv_[!"$trcvInfo"!]GetTransceiverWakeupMode,
[!ENDIF!][!//
[!ENDIF!][!//
    &EthTrcv_[!"$trcvInfo"!]SetTransceiverMode,
    &EthTrcv_[!"$trcvInfo"!]GetTransceiverMode,
    &EthTrcv_[!"$trcvInfo"!]GetLinkState
    },
[!ENDFOR!]
  };
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>
[!ENDIF!][!//


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]
