/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SD_CFG_H
#define SD_CFG_H

/*  MISRA-C:2012 Deviation List
 *
 *  MISRA-1) Deviated Rule: 20.5 (advisory)
 *   #undef shall not be used.
 *
 *   Reason:
 *   Macro SD_NO_PBCFG_REQUIRED and SD_NO_CFG_REQUIRED may be defined in
 *   more than one instance which will cause compile
 *   warning.
 */

[!SELECT "SdGeneral"!][!//

/** \brief Pre-processor switch for enabling production error detection.
 * this has to be done before including Dem.h to check if its necessary.
 */
#define SD_DEM_DETECT [!IF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_MALFORMED_MSG = 'DEM' or
                            VendorSpecific/ProductionErrorHandling/Enable_SD_E_OUT_OF_RES = 'DEM' or
                            VendorSpecific/ProductionErrorHandling/Enable_SD_E_SUBSCR_NACK_RECV = 'DEM'
                           "!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//

/*==================[includes]==============================================*/

[!LOOP "SdHeaderFileInclusion/*"!][!//
#include <[!"."!]>   /* Sd header file */
[!ENDLOOP!][!//

#if(SD_DEM_DETECT == STD_ON)
/* !LINKSTO Sd.ASR40.SWS_SD_00003,1 */
/* !LINKSTO Sd.ASR40.SWS_SD_00106,1 */
#include <Dem.h>
#endif /* SD_DEM_DETECT == STD_ON */

#include <SoAd.h>

/*==================[macros]================================================*/
[!IF "var:defined('postBuildVariant')"!]
#define SD_POSTBUILD_VARIANT_ENABLED
[!ELSE!]
#define SD_POSTBUILD_VARIANT_DISABLED
[!ENDIF!]

/** \brief Pre-processor switch for enabling development error detection.
 */
#define SD_DEV_ERROR_DETECT [!IF "SdDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!// /* !LINKSTO Sd.ASR40.SWS_SD_00108,1 */

/** \brief Pre-processor switch to enable or disable retry functionality to subscribe to Event groups of ServerServices with TTL set to 0xFFFFFF. */
#define SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED [!IF "SdSubscribeEventgroupRetryEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//

/** \brief Pre-processor switch for enabling production error detection of SD_E_MALFORMED_MSG.
 */
#define SD_DEM_DETECT_SD_E_MALFORMED_MSG [!IF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_MALFORMED_MSG = 'DEM'"!]TS_PROD_ERR_REP_TO_DEM[!//
[!ELSEIF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_MALFORMED_MSG = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET

/** \brief DET error Id of SD_E_MALFORMED_MSG.
 */
#define SD_E_MALFORMED_MSG_DEMTODET [!"VendorSpecific/ProductionErrorHandling/SD_E_MALFORMED_MSG_ReportToDemDetErrorId"!]U[!//
[!ELSE!]TS_PROD_ERR_DISABLE[!ENDIF!]

/** \brief Pre-processor switch for enabling production error detection of SD_E_OUT_OF_RES.
 */
#define SD_DEM_DETECT_SD_E_OUT_OF_RES [!IF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_OUT_OF_RES = 'DEM'"!]TS_PROD_ERR_REP_TO_DEM[!//
[!ELSEIF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_OUT_OF_RES = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET

/** \brief DET error Id of SD_E_OUT_OF_RES.
 */
#define SD_E_OUT_OF_RES_DEMTODET [!"VendorSpecific/ProductionErrorHandling/SD_E_OUT_OF_RES_ReportToDemDetErrorId"!]U[!//
[!ELSE!]TS_PROD_ERR_DISABLE[!ENDIF!]

/** \brief Pre-processor switch for enabling production error detection of SD_E_SUBSCR_NACK_RECV.
 */
#define SD_DEM_DETECT_SD_E_SUBSCR_NACK_RECV [!IF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_SUBSCR_NACK_RECV = 'DEM'"!]TS_PROD_ERR_REP_TO_DEM[!//
[!ELSEIF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_SUBSCR_NACK_RECV = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET

/** \brief DET error Id of SD_E_SUBSCR_NACK_RECV.
 */
#define SD_E_SUBSCR_NACK_RECV_DEMTODET [!"VendorSpecific/ProductionErrorHandling/SD_E_SUBSCR_NACK_RECV_ReportToDemDetErrorId"!]U[!//
[!ELSE!]TS_PROD_ERR_DISABLE[!ENDIF!]

/** \brief Pre-processor switch for enabling production error detection of SD_E_SERVER_NOT_AVAILABLE.
 */
#define SD_DEM_DETECT_SD_E_SERVER_NOT_AVAILABLE [!IF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_SERVER_NOT_AVAILABLE = 'DEM'"!]TS_PROD_ERR_REP_TO_DEM[!//
[!ELSEIF "VendorSpecific/ProductionErrorHandling/Enable_SD_E_SERVER_NOT_AVAILABLE = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET

/** \brief DET error Id of SD_E_SERVER_NOT_AVAILABLE.
 */
#define SD_E_SERVER_NOT_AVAILABLE_DEMTODET [!"VendorSpecific/ProductionErrorHandling/SD_E_SERVER_NOT_AVAILABLE_ReportToDemDetErrorId"!]U[!//
[!ELSE!]TS_PROD_ERR_DISABLE[!ENDIF!]

/** \brief Pre-processor switch for enabling version info API support.
 */
#define SD_VERSION_INFO_API [!IF "SdVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!// /* !LINKSTO Sd.ASR40.SWS_SD_00126,1 */

/** \brief Defines the maximum number of options transmitted within a Sd control messages. */
#define SD_SEND_MAXNUM_OPTIONS [!"VendorSpecific/SdTxOptionsBufferSize"!]U

/** \brief Defines the maximum SOME-IP/SD PDU size. */
#define SD_SEND_MAXPAYLOAD [!"VendorSpecific/SdTxFrameBufferSize"!]U

/** \brief Defines the maximum number of options being processes in a received Sd control messages. */
#define SD_RECEIVE_MAXNUM_OPTIONS [!"VendorSpecific/SdRxOptionsBufferSize"!]U

/** \brief 0U is not a valid Dem_EventIdType value */
#define SD_INVALID_DEM_EVENTID 0U

#if (defined SD_SERVER_NOT_AVAILABLE_CALLBACK)
#error SD_SERVER_NOT_AVAILABLE_CALLBACK is already defined
#endif

/** \brief Pre-processor switch for enabling user defined callback function to a server service no longer available. */
#define SD_SERVER_NOT_AVAILABLE_CALLBACK [!IF "node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackServerNotAvailable/Sd_ServerNotAvailableCallBack)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined SD_MALFORMED_MSG_CALLBACK)
#error SD_MALFORMED_MSG_CALLBACK is already defined
#endif
/** \brief Pre-processor switch for enabling user defined callback function to a received malformed UDP Sd frame. */
#define SD_MALFORMED_MSG_CALLBACK [!IF "node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackMalformedMsg/Sd_MalformedMsgCallBack)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined SD_SUBSCR_NACK_RECV_CALLBACK)
#error SD_SUBSCR_NACK_RECV_CALLBACK is already defined
#endif
/** \brief Pre-processor switch for enabling user defined callback function to a received malformed UDP Sd frame. */
#define SD_SUBSCR_NACK_RECV_CALLBACK [!IF "node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackSubscribeNackReceived/Sd_SubscribeNackReceivedCallBack)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

[!ENDSELECT!][!//

#define SD_MAX_INSTANCE_COUNT  [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxInstanceCount)"!]U

#define SD_MAX_CLIENT_SERVICE_COUNT [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxClientServiceCount)"!]U

#define SD_MAX_CLIENT_SERVICE_EVENTGROUP_COUNT [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxClientServiceEventGroupCount)"!]U

#define SD_MAX_CLIENT_CONSUMED_GROUPS_MULTICAST_EVENTREF_COUNT  [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxConsumedEventGroupMulticastReferences)"!]U

#define SD_MAX_SERVER_SERVICE_COUNT [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxServerServiceCount)"!]U

#define SD_MAX_SERVER_SERVICE_EVENT_COUNT [!"num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxServerServiceEventCount)"!]U

#define SD_LOOPS_PER_SECOND [!"num:i((1) div (as:modconf('Sd')[1]/SdGeneral/SdMainFunctionCycleTime))"!]U

[!VAR "MaxRemoteNodeCount"!][!"num:i(0)"!][!ENDVAR!][!//
[!VAR "MaxInstanceRemoteNodeCount"!][!"num:i(0)"!][!ENDVAR!][!//
[!LOOP "as:modconf('Sd')/SdConfig/*[1]/SdInstance/*"!][!//
[!VAR "MaxRemoteNodeCount"!][!"num:i($MaxRemoteNodeCount + SdMaximumRemoteNodes)"!][!ENDVAR!][!//
[!IF "num:i(SdMaximumRemoteNodes) > $MaxInstanceRemoteNodeCount"!][!//
[!VAR "MaxInstanceRemoteNodeCount"!][!"num:i(SdMaximumRemoteNodes)"!][!ENDVAR!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

#define SD_MAXNUM_REMOTENODES [!"num:i($MaxRemoteNodeCount)"!]U

#define SD_MAXNUM_INSTANCE_REMOTENODES [!"num:i($MaxInstanceRemoteNodeCount)"!]U

#define SD_MAXNUM_SUBSCRIPTIONS (SD_MAX_SERVER_SERVICE_EVENT_COUNT * SD_MAXNUM_INSTANCE_REMOTENODES)

#define MAX_SERVER_PRO_INSTANCE ((SD_MAX_SERVER_SERVICE_COUNT / SD_MAX_INSTANCE_COUNT) + 1)

#define MIN_NUM_BYTES_PRO_INSTANCE (((MAX_SERVER_PRO_INSTANCE / 8) + 1) * SD_MAXNUM_INSTANCE_REMOTENODES)

/* size in bytes of the global (includes all configured instances) bitmap to indicate which server has received
  a FIND message and from which remote node it has been received */
#define SD_SERVER_FIND_RECEIVED_GLOBAL_MAX_BITMAP_SIZE (MIN_NUM_BYTES_PRO_INSTANCE * SD_MAX_INSTANCE_COUNT)


/** \brief Compile time verification value */
#if (defined SD_CFG_SIGNATURE)
#error SD_CFG_SIGNATURE already defined!
#endif
#define SD_CFG_SIGNATURE  [!"asc:getConfigSignature(as:modconf('Sd')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Compile time Published information verification value */
#if (defined SD_PUBLIC_INFO_SIGNATURE)
#error SD_PUBLIC_INFO_SIGNATURE already defined!
#endif
#define SD_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('Sd')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('Sd')[1]/CommonPublishedInformation/Release))"!]U

/** \brief Pre-processor switch indicates if PbcfgM uses Sd */
#if (defined SD_PBCFGM_SUPPORT_ENABLED)
#error SD_PBCFGM_SUPPORT_ENABLED already defined!
#endif
#define SD_PBCFGM_SUPPORT_ENABLED  [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('Sd')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_RELOCATABLE_CFG_ENABLE)
#error SD_RELOCATABLE_CFG_ENABLE is already defined
#endif
/** \brief Pre-processor switch to enable/diable relocateable postbuild config
 */
#define SD_RELOCATABLE_CFG_ENABLE  [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('Sd')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!][!IF "SdGeneral/SdRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ENDIF!]

#if (SD_DEV_ERROR_DETECT == STD_ON)



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "SdDefensiveProgramming"!][!//

#if (defined SD_DEFENSIVE_PROGRAMMING_ENABLED)
#error SD_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define SD_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_PRECONDITION_ASSERT_ENABLED)
#error SD_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define SD_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true') and (SdPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_POSTCONDITION_ASSERT_ENABLED)
#error SD_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define SD_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true') and (SdPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_UNREACHABLE_CODE_ASSERT_ENABLED)
#error SD_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define SD_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true') and (SdUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_INVARIANT_ASSERT_ENABLED)
#error SD_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define SD_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true') and (SdInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined SD_STATIC_ASSERT_ENABLED)
#error SD_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define SD_STATIC_ASSERT_ENABLED           [!//
[!IF "(../SdGeneral/SdDevErrorDetect  = 'true') and (SdDefProgEnabled = 'true') and (SdStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

#endif /* SD_DEV_ERROR_DETECT == STD_ON */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[inclusions]==============================================*/
[!LOOP "variant:all()"!][!//
#include <Sd_[!"."!]_PBcfg.h>
[!ENDLOOP!][!//

/* Exclude post-build-time config include file if requested to do so */
#if (!defined SD_NO_PBCFG_REQUIRED) && (!defined SD_NO_CFG_REQUIRED)
#if (defined SD_POSTBUILD_VARIANT_DISABLED)
#include <Sd_PBcfg.h>
#endif /* SD_POSTBUILD_VARIANT_DISABLED */
#endif /* SD_NO_PBCFG_REQUIRED, SD_NO_CFG_REQUIRED */

#endif /* ifndef SD_CFG_H */
/*==================[end of file]===========================================*/

