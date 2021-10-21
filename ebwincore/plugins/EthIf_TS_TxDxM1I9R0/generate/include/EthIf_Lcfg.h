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
#ifndef ETHIF_LCFG_H
#define ETHIF_LCFG_H

/*==================[includes]==============================================*/

#include <TSAutosar.h>          /* EB specific standard types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>
/** \brief Mapping of controller specific main function Rx function to generic functions
 */
[!LOOP "node:order(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfPhysController/*, 'EthIfPhysControllerIdx')"!][!//
  [!LOOP "./EthIfPhysCtrlRxMainFunctionPriorityProcessing/*"!][!//
extern FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx_[!"node:name(.)"!](void);

  [!ENDLOOP!][!//
[!ENDLOOP!][!//

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define ETHIF_START_SEC_CONST_32
#include <EthIf_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
extern CONST(uint32, ETHIF_CONST) EthIf_LcfgSignature;

#define ETHIF_STOP_SEC_CONST_32
#include <EthIf_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHIF_LCFG_H */
/*==================[end of file]===========================================*/
[!ENDCODE!]
