/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
#ifndef ETHSM_CFG_H
#define ETHSM_CFG_H

/*==================[includes]==============================================*/

#include <TSAutosar.h>              /* Global configuration */
#include <EthSM_DefProg_Cfg.h>

/*==================[macros]================================================*/

#if (defined ETHSM_SINGLE_NETWORK_OPT_ENABLE)
#error ETHSM_SINGLE_NETWORK_OPT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling single network optimization.
 */
#define ETHSM_SINGLE_NETWORK_OPT_ENABLE     [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMSingleNetworkOptEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_DEV_ERROR_DETECT)
#error ETHSM_DEV_ERROR_DETECT is already defined
#endif
/** \brief Pre-processor switch for enabling development error detection.
 */
#define ETHSM_DEV_ERROR_DETECT              [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_VERSION_INFO_API)
#error ETHSM_VERSION_INFO_API is already defined
#endif
/** \brief Pre-processor switch for enabling version info API support.
 */
#define ETHSM_VERSION_INFO_API              [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_NETWORKS_ARRAYSIZE)
#error ETHSM_NETWORKS_ARRAYSIZE is already defined
#endif
/** \brief Maximum value of handle of destinated communication network for request
 */
#define ETHSM_NETWORKS_ARRAYSIZE            [!"num:i(as:modconf('EthSM')[1]/EthSMGeneral/EthSMMaxNetworks)"!]U

#if (defined ETHSM_DEVAUTH_SUPPORT_ENABLE)
#error ETHSM_DEVAUTH_SUPPORT_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling Device Authentication Support.
 */
#define ETHSM_DEVAUTH_SUPPORT_ENABLE     [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMDevAuthSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_MULTI_CORE)
#error ETHSM_MULTI_CORE is already defined
#endif
/** \brief Pre-processor switch for enabling Multi core support.
 */
#define ETHSM_MULTI_CORE              [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMMultiCoreSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHSM_CALL_COMM_BUSSM_MODEINDICATION)
#error ETHSM_CALL_COMM_BUSSM_MODEINDICATION is already defined
#endif
/** \brief Pre-processor switch for selecting ComM or Rte generated function.
 */
#define ETHSM_CALL_COMM_BUSSM_MODEINDICATION(NetworkIdx, Channel, ComMode)    [!//
[!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMMultiCoreSupport = 'true'"!][!//
EthSM_ComM_BusSM_ModeIndicationFpList[NetworkIdx](Channel, ComMode)
[!ELSE!][!//
ComM_BusSM_ModeIndication(Channel, ComMode)
[!ENDIF!][!//

#if (defined ETHSM_PROD_ERR_HANDLING_CTRL_TEST)
#error ETHSM_PROD_ERR_HANDLING_CTRL_TEST is already defined
#endif
/** \brief Switch for DEM to DET reporting for production error ETHSM_E_LINK_DOWN */
#define ETHSM_PROD_ERR_HANDLING_CTRL_TEST   [!//
[!IF "ReportToDem/EthSMDemCtrlTestResultReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/EthSMDemCtrlTestResultReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined ETHSM_E_DEMTODET_CTRL_TEST)
#error ETHSM_E_DEMTODET_CTRL_TEST is already defined
#endif
[!IF "ReportToDem/EthSMDemCtrlTestResultReportToDem = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled for ETHSM_E_LINK_DOWN */
#define ETHSM_E_DEMTODET_CTRL_TEST [!"ReportToDem/EthSMDemCtrlTestResultReportToDemDetErrorId"!]U
[!ENDIF!][!//

/** \brief Compile time verification value */
#if (defined ETHSM_CFG_SIGNATURE)
#error ETHSM_CFG_SIGNATURE already defined!
#endif
#define ETHSM_CFG_SIGNATURE  [!"asc:getConfigSignature(as:modconf('EthSM')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Compile time Published information verification value */
#if (defined ETHSM_PUBLIC_INFO_SIGNATURE)
#error ETHSM_PUBLIC_INFO_SIGNATURE already defined!
#endif
#define ETHSM_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('EthSM')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('EthSM')[1]/CommonPublishedInformation/Release))"!]U

/** \brief Pre-processor switch indicates if PbcfgM uses EthSM */
#if (defined ETHSM_PBCFGM_SUPPORT_ENABLED)
#error ETHSM_PBCFGM_SUPPORT_ENABLED already defined!
#endif
#define ETHSM_PBCFGM_SUPPORT_ENABLED  [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthSM')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch to enable/disable relocatable postbuild config
 */
#if (defined ETHSM_RELOCATABLE_CFG_ENABLE)
#error ETHSM_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define ETHSM_RELOCATABLE_CFG_ENABLE [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthSM')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!][!IF "EthSMGeneral/EthSMRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ENDIF!]

/** \brief Pre-processor switch indicating that EthSM_IsValidConfig function shall be mapped to memory section ETHSM_CODE_CC_BLOCK
*/
#if (defined ETHSM_ISVALIDCONFIG_MMAP_ENABLED)
#error ETHSM_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif
#define ETHSM_ISVALIDCONFIG_MMAP_ENABLED [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthSM')) = 'true'"!][!//
[!IF "node:exists(as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection) and as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[includes]==============================================*/

#endif /* ifndef ETHSM_CFG_H */

/*==================[end of file]===========================================*/
[!ENDCODE!]
