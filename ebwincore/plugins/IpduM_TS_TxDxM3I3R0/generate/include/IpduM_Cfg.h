/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef IPDUM_CFG_H
#define IPDUM_CFG_H

[!INCLUDE "../../generate_macros/IpduM_GenParams.m"!]

/* This file contains all target independent public configuration declarations
 * for the AUTOSAR module IpduM. */

[!AUTOSPACING!]

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>

/*==================[macros]================================================*/
/*------------------[General]-----------------------------------------------*/

[!INDENT "0"!]
#if (defined IPDUM_AUTOSAR_VERSION)
#error IPDUM_AUTOSAR_VERSION already defined
#endif

/** \brief IpduM Header Byte Order */
#if (defined IPDUM_HEADER_BYTE_ORDER)
#error IPDUM_HEADER_BYTE_ORDER already defined!
#endif /* #if (defined IPDUM_HEADER_BYTE_ORDER)*/
#define IPDUM_HEADER_BYTE_ORDER[!WS "3"!][!IF "not(node:empty(IpduMGeneral/IpduMHeaderByteOrder))"!][!"node:value(IpduMGeneral/IpduMHeaderByteOrder)"!][!ELSE!]IPDUM_LITTLE_ENDIAN[!ENDIF!][!//

/** \brief The respective AUTOSAR version */
#define IPDUM_AUTOSAR_VERSION         30

#if (defined IPDUM_DEV_ERROR_DETECT)
#error IPDUM_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating if development error detection is activated for
 ** IPDUM */
#define IPDUM_DEV_ERROR_DETECT         STD_[!"node:when($IpduMDevErrorDetect = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_VERSION_INFO_API)
#error IPDUM_VERSION_INFO_API already defined
#endif
/** \brief Switch, indicating if the version information API is enabled or
 ** disabled for IPDUM */
#define IPDUM_VERSION_INFO_API         STD_[!"node:when($IpduMVersionInfoApi = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_STATIC_PART_EXISTS)
#error IPDUM_STATIC_PART_EXISTS already defined
#endif
/** \brief Switch, indicating if a static part exists or not in an IPDU.
 ** This is to allow optimizations in case the IpduM will never
 ** be used with a static part.
 **
 ** \note This is a pre-compile option. If this is set to False then it
 ** will not be possible to add static parts after compilation.
 **/
#define IPDUM_STATIC_PART_EXISTS         STD_[!"node:when($IpduMStaticPartExists = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_ZERO_COPY)
#error IPDUM_ZERO_COPY already defined
#endif
/** \brief This indicates do not allocate memory for data in the IpduM.
**  Only pointers for static and dynamic parts will be passed.
**
** \note Zero copy is possible only in some circumstances.
 **/
#define IPDUM_ZERO_COPY         STD_[!"node:when($IpduMZeroCopy = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_BYTE_COPY)
#error IPDUM_BYTE_COPY already defined
#endif
/** \brief Switch, indicating use of byte-wise copy routines. Only possible if static and
**  dynamic parts are already byte-aligned.
**/
#define IPDUM_BYTE_COPY         STD_[!"node:when($IpduMByteCopy = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_DYNAMIC_PART_QUEUE)
#error IPDUM_DYNAMIC_PART_QUEUE already defined
#endif
/** \brief This specifies if queuing is enabled for dynamic PDUs.
**/
#define IPDUM_DYNAMIC_PART_QUEUE         STD_[!"node:when($IpduMDynamicPartQueue = 'true', 'ON', 'OFF')"!]

/** \brief This specifies the number of bytes on stack used for the buffer
**  in RxIndication
**/
#if (defined IPDUM_RECEIVE_STACK_SIZE)
#error IPDUM_RECEIVE_STACK_SIZE already defined!
#endif /* #if (defined IPDUM_RECEIVE_STACK_SIZE)*/
#define IPDUM_RECEIVE_STACK_SIZE 254U

#if (defined IPDUM_TX_SDU_SIZE)
#error IPDUM_TX_SDU_SIZE already defined!
#endif /* #if (defined IPDUM_TX_SDU_SIZE)*/

/** \brief This specifies the number of bytes for the SDU buffer in the
**  transmission queue
**/
#define IPDUM_TX_SDU_SIZE [!"$IpduMTxRequestMaxSduLength"!]U

#if (defined IPDUM_AUTOMATIC_SELECTOR)
#error IPDUM_AUTOMATIC_SELECTOR already defined
#endif
/** \brief This specifies if automatic selector setting is enabled for transmit
**  PDUs.
**/
#define IPDUM_AUTOMATIC_SELECTOR         STD_[!"node:when($IpduMTxAutomaticSelector = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_INITIALIZATION_BY_SIGNAL_VALUE)
#error IPDUM_INITIALIZATION_BY_SIGNAL_VALUE already defined
#endif
/** \brief This macro defines if the static and dynamic parts are initialized in retrieving
**  signal values from an upper layer module by IpduM_Init. If the functionality is enabled
**  it is set to STD_ON, otherwise to STD_OFF.
**/
#define IPDUM_INITIALIZATION_BY_SIGNAL_VALUE         STD_[!"node:when($IpduMInitializationBySignalValue = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_ENABLE_JIT_UPDATE)
#error IPDUM_ENABLE_JIT_UPDATE already defined
#endif
/** \brief This macro defines if the Just-In-Time update funtionality of the static and dynamic
 ** parts is enabled. If Just-In-Time update is enabled it is set to STD_ON, otherwise to STD_OFF.
 **/
#define IPDUM_ENABLE_JIT_UPDATE         STD_[!"node:when($IpduMEnableJitUpdate = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_CONTAINER_PDU_HANDLING_ENABLE)
#error IPDUM_CONTAINER_PDU_HANDLING_ENABLE already defined
#endif
/** \brief This macro defines if the Multiple-PDU-to-Container-Mapping functionality
 ** of the module is enabled. If IpduMContainerPduHandlingEnable is enabled then
 ** it is set to STD_ON, otherwise to STD_OFF.
 **/
#define IPDUM_CONTAINER_PDU_HANDLING_ENABLE         STD_[!"node:when($IpduMContainerPduHandlingEnable = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_CNTND_COLLECT_QUEUED_TX)
#error IPDUM_CNTND_COLLECT_QUEUED_TX already defined
#endif
/** \brief This macro defines whether IpduMContainedTxPdus have been configured with
 ** IPDUM_COLLECT_QUEUED as collection semantics.
 **/
#define IPDUM_CNTND_COLLECT_QUEUED_TX             STD_[!"node:when($IpduMContainedCollectQueuedTxEnable = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_CONTAINER_QUEUING_RX)
#error IPDUM_CONTAINER_QUEUING_RX already defined
#endif
/** \brief This macro defines whether IpduMContainerRxPdus have been configured with
 ** IpduMContainerQueueSize set.
 **/
#define IPDUM_CONTAINER_QUEUING_RX             STD_[!"node:when($IpduMContainerQueuingRxEnable = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_DEQUEUE_IN_TX_CONF)
#error IPDUM_DEQUEUE_IN_TX_CONF already defined
#endif
/** \brief This macro defines whether IpduMContainedTxPdus have been configured with
 ** IPDUM_COLLECT_QUEUED as collection semantics.
 **/
#define IPDUM_DEQUEUE_IN_TX_CONF             STD_[!"node:when(./IpduMGeneral/IpduMDequeueInTxConf = 'true', 'ON', 'OFF')"!]

#if (defined IPDUM_MAX_CONTAINER_RX_LENGTH_U32)
#error IPDUM_MAX_CONTAINER_RX_LENGTH_U32 already defined
#endif
/** \brief Macro used for on-stack buffer allocation. Size in uint32
 **/
#define IPDUM_MAX_CONTAINER_RX_LENGTH_U32        [!"num:i($IpduMMaxContainerRxLength div asc:getDataTypeSize('uint32') + 1)"!]U

#if (defined IPDUM_HDR_ID_OFFST)
#error IPDUM_HDR_ID_OFFST already defined
#endif
[!IF "not(node:empty(IpduMGeneral/IpduMHeaderByteOrder)) and node:value(IpduMGeneral/IpduMHeaderByteOrder) = 'IPDUM_LITTLE_ENDIAN'"!]
#define IPDUM_HDR_ID_OFFST 0
[!ELSE!]
#define IPDUM_HDR_ID_OFFST 1
[!ENDIF!]

#if (defined IPDUM_CONTAINEDTXPDU_PRIORITY_HANDLING_ENABLE)
#error IPDUM_CONTAINEDTXPDU_PRIORITY_HANDLING_ENABLE already defined
#endif
#define IPDUM_CONTAINEDTXPDU_PRIORITY_HANDLING_ENABLE STD_[!"node:when($IpduMContainedTxPduPriorityHandlingEnable = 'true', 'ON', 'OFF')"!]


#if (defined IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT)
#error IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT already defined
#endif
#define IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT STD_[!"node:when($IpduMMultiMF = 'true', 'ON', 'OFF')"!]


/* ----- Pre-processor switch to enable/disable relocateable postbuild config ----- */
[!VAR "relocatable" = "'STD_OFF'"!]
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('IpduM')) = 'true'"!]
  [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]
    [!VAR "relocatable" = "'STD_ON'"!]
  [!ENDIF!]
[!ELSE!]
  [!IF "IpduMGeneral/IpduMRelocatablePbcfgEnable = 'true'"!]
    [!VAR "relocatable" = "'STD_ON'"!]
  [!ENDIF!]
[!ENDIF!]
#if (defined IPDUM_RELOCATABLE_CFG_ENABLE)
#error IPDUM_RELOCATABLE_CFG_ENABLE already defined
#endif
#define IPDUM_RELOCATABLE_CFG_ENABLE [!"$relocatable"!]


#if (defined IPDUM_PBCFGM_SUPPORT_ENABLED)
#error IPDUM_PBCFGM_SUPPORT_ENABLED already defined
#endif
#define IPDUM_PBCFGM_SUPPORT_ENABLED STD_[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('IpduM')) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined IPDUM_CFG_SIGNATURE) /* to prevent double declaration */
#error IPDUM_CFG_SIGNATURE already defined
#endif /* if (defined IPDUM_CFG_SIGNATURE) */

/** \brief This macro specifies the compile time signature. */
#define IPDUM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('IpduM')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U


#if (defined IPDUM_PUBLIC_INFO_SIGNATURE) /* to prevent double declaration */
#error IPDUM_PUBLIC_INFO_SIGNATURE already defined
#endif /* if (defined IPDUM_PUBLIC_INFO_SIGNATURE) */

/** \brief This macro specifies the published information signature. */
#define IPDUM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(as:modconf('IpduM')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ])"!]U


#if (defined IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE)
#error IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE already defined
#endif
#define IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE STD_[!"node:when($IpduMStaticContainerPduHandlingEnable = 'true', 'ON', 'OFF')"!]

[!ENDINDENT!][!//



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "IpduMDefensiveProgramming"!][!//

#if (defined IPDUM_DEFENSIVE_PROGRAMMING_ENABLED)
#error IPDUM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define IPDUM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined IPDUM_PRECONDITION_ASSERT_ENABLED)
#error IPDUM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define IPDUM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true') and (IpduMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined IPDUM_POSTCONDITION_ASSERT_ENABLED)
#error IPDUM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define IPDUM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true') and (IpduMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined IPDUM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error IPDUM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define IPDUM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true') and (IpduMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined IPDUM_INVARIANT_ASSERT_ENABLED)
#error IPDUM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define IPDUM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true') and (IpduMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined IPDUM_STATIC_ASSERT_ENABLED)
#error IPDUM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define IPDUM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../IpduMGeneral/IpduMDevErrorDetect  = 'true') and (IpduMDefProgEnabled = 'true') and (IpduMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef IPDUM_CFG_H */
/*==================[end of file]===========================================*/
