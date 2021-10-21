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

#ifndef DOIP_CFG_H
#define DOIP_CFG_H

[!CODE!]
/*==================[includes]==============================================*/

#include <Std_Types.h> /* AUTOSAR standard types */

[!LOOP "DoIPGeneral/DoIPHeaderFileInclusion/*"!][!//
#include <[!"."!]>   /* DoIP header file */
[!ENDLOOP!][!//

/*==================[macros]================================================*/

/* When timer is set to 0, the closest we can get is the next MainFunction tick. */
[!// Macro to calculate mainfunction ticks from time (float).
[!MACRO "CalcTimeout", "Value", "Name"!][!//
[!IF "($Value) = 0"!][!"num:i(1)"!][!ELSE!][!//
[!SELECT "as:modconf('DoIP')[1]/DoIPGeneral"!][!//
[!VAR "mValue" = "num:i(($Value div ./DoIPMainFunctionPeriod) - 0.00001) + 1"!][!//
[!IF "$Value = 0"!][!VAR "mValue" = "0"!][!ENDIF!][!//
[!VAR "dValue" = "$Value div ./DoIPMainFunctionPeriod"!][!//
[!IF "(($dValue - $mValue) > 0.00001) or (($dValue - $mValue) < -0.00001)"!][!//
[!WARNING!] The value ([!"$Value"!]s) of [!"$Name"!] is not divisible by DoIPMainFunctionPeriod. It will be rounded to: [!"$mValue * ./DoIPMainFunctionPeriod"!]s.[!ENDWARNING!][!//
[!ENDIF!][!//
[!"num:i($mValue)"!][!//
[!ENDSELECT!][!ENDIF!][!//
[!ENDMACRO!][!//


#if (defined DOIP_DEV_ERROR_DETECT)
#error DOIP_DEV_ERROR_DETECT is already defined
#endif
/** \brief Pre-processor switch for enabling development error detection. */
#define DOIP_DEV_ERROR_DETECT [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPDevelopmentErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_ENABLE_MAIN_FUNCTION_TX)
#error DOIP_ENABLE_MAIN_FUNCTION_TX is already defined
#endif

/** \brief Pre-processor switch for enabling calling the tx part of the main function separately. */
#define DOIP_ENABLE_MAIN_FUNCTION_TX [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPEnableMainFunctionTx = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_VERSION_INFO_API)
#error DOIP_VERSION_INFO_API is already defined
#endif
/** \brief Pre-processor switch for enabling version info API support. */
#define DOIP_VERSION_INFO_API [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_MAX_PAYLOADSIZE)
#error DOIP_MAX_PAYLOADSIZE is already defined
#endif
/** \brief Maximum allowed diagnostic payload size. */
#define DOIP_MAX_PAYLOADSIZE ((uint32)[!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxRequestBytes)"!]U)

#if (defined DOIP_MAX_NUMBYTE_DIAGACKNACK)
#error DOIP_MAX_NUMBYTE_DIAGACKNACK is already defined
#endif
/** \brief Maximum number of bytes of the original Diagnostic message that will be copied into the ACK/NACK response message. */
#define DOIP_MAX_NUMBYTE_DIAGACKNACK [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxNumByteDiagAckNack)"!]U

#if (defined DOIP_VIN_INVALIDITY_PATTERN)
#error DOIP_VIN_INVALIDITY_PATTERN is already defined
#endif
/** \brief Vehicle identification number invalidity pattern. */
#define DOIP_VIN_INVALIDITY_PATTERN [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPVinInvalidityPattern)"!]U

#if (defined DOIP_VIN_STATUS_ENABLE)
#error DOIP_VIN_STATUS_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling vehicle identification number status. */
#define DOIP_VIN_STATUS_ENABLE [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPUseVehicleIdentificationSyncStatus = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_DHCP_VIN_USE_ENABLE)
#error DOIP_DHCP_VIN_USE_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling the use of VIN for DHCP host naming. */
#define DOIP_DHCP_VIN_USE_ENABLE [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPDhcpOptionVinUse = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_USER_GET_VIN)
#error DOIP_USER_GET_VIN is already defined
#endif
/** \brief Pre-processor switch for enabling user defined callback function to retrieve VIN. */
#define DOIP_USER_GET_VIN [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetVIN/DoIPGetVin)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined DOIP_USER_VID_REQ_RECEIVED_CALLBACK)
#error DOIP_USER_VID_REQ_RECEIVED_CALLBACK is already defined
#endif
/** \brief Pre-processor switch for enabling user defined callback function to indicate a received VID request. */
#define DOIP_USER_VID_REQ_RECEIVED_CALLBACK [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPVIDRequestReceivedCallback/DoIPVIDRequestReceived)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined DOIP_POWER_MODE)
#error DOIP_POWER_MODE is already defined
#endif
/** \brief Pre-processor switch for enabling service port/user callback function to retrieve Power Mode. */
#define DOIP_POWER_MODE [!IF "not(node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPPowerModeCallback/DoIPPowerMode)) or (as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_RTE_USED)
#error DOIP_RTE_USED is already defined.
#endif
/** \brief Pre-processor switch for enabling access to Rte. */
#define DOIP_RTE_USED [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_VEHICLE_ANN_REPETITION)
#error DOIP_VEHICLE_ANN_REPETITION is already defined
#endif
/** \brief Number of repetitons of the vehicle announcements. */
#define DOIP_VEHICLE_ANN_REPETITION [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPVehicleAnnouncementRepetition)"!]U

#if (defined DOIP_VEHICLE_ANN_TIMEOUT)
#error DOIP_VEHICLE_ANN_TIMEOUT is already defined
#endif
/** \brief Timeout until next vehicle announcements shall be sent. */
#define DOIP_VEHICLE_ANN_TIMEOUT [!CALL "CalcTimeout", "Value"="as:modconf('DoIP')[1]/DoIPGeneral/DoIPVehicleAnnouncementInterval", "Name"="'DoIPVehicleAnnouncementInterval'"!]U

#if (defined DOIP_VEHICLE_ANN_RESP_MAX_INIT_TIMEOUT)
#error DOIP_VEHICLE_ANN_RESP_MAX_INIT_TIMEOUT is already defined
#endif
/** \brief Maximum timeout for initial vehicle announcement and vehicle identification response. */
#define DOIP_VEHICLE_ANN_RESP_MAX_INIT_TIMEOUT [!CALL "CalcTimeout", "Value"="as:modconf('DoIP')[1]/DoIPGeneral/DoIPInitialVehicleAnnouncementTime", "Name"="'DoIPInitialVehicleAnnouncementTime'"!]U

#if (defined DOIP_MAX_PARALLEL_TESTER)
#error DOIP_MAX_PARALLEL_TESTER is already defined
#endif
/** \brief Maximum number of parallel active tester (TCP connections). */
#define DOIP_MAX_PARALLEL_TESTER [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxTesterConnections)"!]U

/** \brief Maximum number of UDP socket connections. */
#define DOIP_MAX_UDP_SOCKET_CONNECTIONS [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxUDPSocketConnections)"!]U

#if (defined DOIP_MAX_CHANNELS)
#error DOIP_MAX_CHANNELS is already defined
#endif
/** \brief Maximum number configured channels. */
#define DOIP_MAX_CHANNELS [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxChannels)"!]U

#if (defined DOIP_TCP_INITIAL_INACTIVITY_TIMEOUT)
#error DOIP_TCP_INITIAL_INACTIVITY_TIMEOUT is already defined
#endif
/** \brief Initial timeout of a TCP connection waiting for a routing activation request. */
#define DOIP_TCP_INITIAL_INACTIVITY_TIMEOUT [!CALL "CalcTimeout", "Value"="as:modconf('DoIP')[1]/DoIPGeneral/DoIPInitialInactivityTime", "Name"="'DoIPInitialInactivityTime'"!]U

#if (defined DOIP_TCP_GENERAL_INACTIVITY_TIMEOUT)
#error DOIP_TCP_GENERAL_INACTIVITY_TIMEOUT is already defined
#endif
/** \brief General timeout of a TCP connection waiting for a routing activation request. */
#define DOIP_TCP_GENERAL_INACTIVITY_TIMEOUT [!CALL "CalcTimeout", "Value"="as:modconf('DoIP')[1]/DoIPGeneral/DoIPGeneralInactivityTime", "Name"="'DoIPGeneralInactivityTime'"!]U

#if (defined DOIP_DIAG_ENT_STATUS_MAXBYTE_USE)
#error DOIP_DIAG_ENT_STATUS_MAXBYTE_USE is already defined
#endif
/** \brief Pre-processor switch for using the Max data size element of a diagnostic entity status response. */
#define DOIP_DIAG_ENT_STATUS_MAXBYTE_USE [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPEntityStatusMaxByteFieldUse = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_NODE_TYPE)
#error DOIP_NODE_TYPE is already defined
#endif
/** \brief Switch for DoIPNodeType DOIP_GATEWAY or DOIP_NODE */
#define DOIP_NODE_TYPE   [!//
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPNodeType = 'DOIP_GATEWAY'"!][!//
DOIP_GATEWAY
[!ELSE!][!//
DOIP_NODE
[!ENDIF!][!//

#if (defined DOIP_RESPONSE_BEFORE_ROUTACT)
#error DOIP_RESPONSE_BEFORE_ROUTACT is already defined
#endif
/** \brief Switch for DoIPResponseBeforeRoutingActivation DOIP_DISABLE_DIAG_NACK_0x06 or DOIP_ENABLE_ALL_DIAG_NACK */
#define DOIP_RESPONSE_BEFORE_ROUTACT   [!//
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPResponseBeforeRoutingActivation = 'DOIP_ENABLE_ALL_DIAG_NACK'"!][!//
DOIP_ENABLE_ALL_DIAG_NACK
[!ELSE!][!//
DOIP_DISABLE_DIAG_NACK_0x06
[!ENDIF!][!//

#if (defined DOIP_PL_LENGTH_ENTITY_STATUS_RESP)
#error DOIP_PL_LENGTH_ENTITY_STATUS_RESP is already defined
#endif
/** \brief  Pre-processor switch to determine the size of Diagnostic entity status response message */
#define DOIP_PL_LENGTH_ENTITY_STATUS_RESP   [!//
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPEntityStatusMaxByteFieldUse = 'true'"!][!//
7U
[!ELSE!][!//
3U
[!ENDIF!][!//

#if (defined DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED)
#error DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED is already defined
#endif
/** \brief Pre-processor switch for enabling custom payload type diagnostic messages. */
#define DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPCustomPayloadTypeEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_GETANDRESETMEASUREMENTDATA_API)
#error DOIP_GETANDRESETMEASUREMENTDATA_API is already defined
#endif
/** \brief Pre-processor switch for enabling Get and Reset measurement data API. */
#define DOIP_GETANDRESETMEASUREMENTDATA_API [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetAndResetMeasurementDataApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_USER_TCP_SOCON_MODECHG_CALLBACK)
#error DOIP_USER_TCP_SOCON_MODECHG_CALLBACK is already defined
#endif
/** \brief Pre-processor switch for enabling user defined callback function to provide information about the TCP connection status. */
#define DOIP_USER_TCP_SOCON_MODECHG_CALLBACK [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPTcpSoConModeChgCallback/DoIPTcpSoConModeChg)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined DOIP_TCP_CONN_BUFF_SIZE)
#error DOIP_TCP_CONN_BUFF_SIZE is already defined
#endif
/** \brief Tcp connection buffer size used for segmented header reception and storing a part of diagnostic message payload used for NACK. */
[!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxNumByteDiagAckNack > 11"!][!//
#define DOIP_TCP_CONN_BUFF_SIZE ([!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxNumByteDiagAckNack)"!]U)
[!ELSE!][!//
#define DOIP_TCP_CONN_BUFF_SIZE   11U
[!ENDIF!][!//

#define DOIP_MAX_STANDARD_CHANNELS (DOIP_MAX_CHANNELS - DOIP_MAX_CUSTOM_CHANNELS)

#if (defined DOIP_MAX_VEHICLE_ANNOUNCEMENT_CON)
#error DOIP_MAX_VEHICLE_ANNOUNCEMENT_CON is already defined
#endif
/** \brief Maximum number of Udp announcement connections. */
#define DOIP_MAX_VEHICLE_ANNOUNCEMENT_CON [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPMaxVehicleAnnouncementCon)"!]U

#if (defined DOIP_PDULENGTHTYPE_MAX)
#error DOIP_PDULENGTHTYPE_MAX is already defined
#endif
/** \brief Maximum DoIP message payload size */
[!IF "as:modconf('EcuC')[1]/EcucPduCollection/PduLengthTypeEnum = 'UINT16'"!][!//
#define DOIP_PDULENGTHTYPE_MAX 0xFFFFU
[!ELSE!][!//
#define DOIP_PDULENGTHTYPE_MAX 0xFFFFFFFFU
[!ENDIF!][!//

#if (defined DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT)
#error DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT is already defined
#endif
/** \brief Timeout for waiting for a response to an Alive check request. */
#define DOIP_ALIVE_CHECK_RESPONSE_TIMEOUT [!CALL "CalcTimeout", "Value"="as:modconf('DoIP')[1]/DoIPGeneral/DoIPAliveCheckResponseTimeout", "Name"="'DoIPAliveCheckResponseTimeout'"!]U

#if (defined DOIP_USE_MAC_ADDR_ENABLE)
#error DOIP_USE_MAC_ADDR_ENABLE is already defined
#endif
/** \brief Pre-processor switch for enabling the use of MAC Address instead of EID for Vehicle Identification/Announcement. */
#define DOIP_USE_MAC_ADDR_ENABLE [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPUseMacAddressForIdentification = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_USE_EID_AS_GID_ENABLED)
#error DOIP_USE_EID_AS_GID_ENABLED is already defined
#endif

/** \brief Pre-processor switch for enabling setting GID to EID. */
#define DOIP_USE_EID_AS_GID_ENABLED [!IF "as:modconf('DoIP')[1]/DoIPGeneral/DoIPUseEIDasGID = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_DYN_GID_SYNC_ENABLED)
#error DOIP_DYN_GID_SYNC_ENABLED is already defined
#endif
/** \brief Pre-processor switch for enabling Dynamic GID synchronization. */
#define DOIP_DYN_GID_SYNC_ENABLED [!IF "node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPDynamicGIDMasterSelectionCallback/DoIPDynamicGIDMasterSelection) or node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPGetGid/DoIPGetGID)"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

/** \brief Compile time verification value */
#if (defined DOIP_CFG_SIGNATURE)
#error DOIP_CFG_SIGNATURE already defined!
#endif
#define DOIP_CFG_SIGNATURE  [!"asc:getConfigSignature(as:modconf('DoIP')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U

/** \brief Compile time Published information verification value */
#if (defined DOIP_PUBLIC_INFO_SIGNATURE)
#error DOIP_PUBLIC_INFO_SIGNATURE already defined!
#endif
#define DOIP_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('DoIP')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('DoIP')[1]/CommonPublishedInformation/Release))"!]U

/** \brief Pre-processor switch indicates if PbcfgM uses DoIP */
#if (defined DOIP_PBCFGM_SUPPORT_ENABLED)
#error DOIP_PBCFGM_SUPPORT_ENABLED already defined!
#endif
#define DOIP_PBCFGM_SUPPORT_ENABLED  [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('DoIP')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch to enable/disable relocatable postbuild config
 */
#if (defined DOIP_RELOCATABLE_CFG_ENABLE)
#error DOIP_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define DOIP_RELOCATABLE_CFG_ENABLE [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('DoIP')) = 'true'"!][!//
[!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!][!IF "DoIPGeneral/DoIPRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ENDIF!]

/** \brief Pre-processor switch indicating that DoIP_IsValidConfig function shall be mapped to memory section DOIP_CODE_CC_BLOCK
*/
#if (defined DOIP_ISVALIDCONFIG_MMAP_ENABLED)
#error DOIP_ISVALIDCONFIG_MMAP_ENABLED already defined!
#endif
#define DOIP_ISVALIDCONFIG_MMAP_ENABLED [!//
[!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('DoIP')) = 'true'"!][!//
[!IF "node:exists(as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection) and as:modconf('PbcfgM')[1]/PbcfgMGeneral/PbcfgMMapIsValidFunctionToMemSection = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//
[!ELSE!]STD_OFF[!//
[!ENDIF!]

#if (defined DOIP_HOSTNAME_SIZE_MAX)
#error DOIP_HOSTNAME_SIZE_MAX is already defined
#endif
/** \brief Maximum size of DHCP HostName. */
#define DOIP_HOSTNAME_SIZE_MAX [!"num:i(as:modconf('DoIP')[1]/DoIPGeneral/DoIPHostNameSizeMax)"!]U

#if (defined DOIP_DHCP_HOSTNAME_PREFIX)
#error DOIP_DHCP_HOSTNAME_PREFIX is already defined
#endif
/** \brief DHCP HostName string. */
#define DOIP_DHCP_HOSTNAME_PREFIX [!//
[!IF "not(node:empty(as:modconf('DoIP')[1]/DoIPGeneral/DoIPDhcpHostNamePrefix))"!][!//
"[!"string(as:modconf('DoIP')[1]/DoIPGeneral/DoIPDhcpHostNamePrefix)"!]" /* user provided Dhcp host name prefix */
[!ELSE!]""[!//
[!ENDIF!][!//

#if (defined DOIP_DHCP_HOSTNAME_PREFIX_SIZE)
#error DOIP_DHCP_HOSTNAME_PREFIX_SIZE is already defined
#endif
/** \brief Length of DHCP HostName string. */
#define DOIP_DHCP_HOSTNAME_PREFIX_SIZE [!"num:i(string-length(as:modconf('DoIP')[1]/DoIPGeneral/DoIPDhcpHostNamePrefix))"!]U

/** \brief Number of configured routing activation user callback functions. */
#if (defined DOIP_ROUTING_ACTIVATION_CALLBACK_NUM)
#error DOIP_ROUTING_ACTIVATION_CALLBACK_NUM is already defined
#endif
#define DOIP_ROUTING_ACTIVATION_CALLBACK_NUM [!"num:i(count(as:modconf('DoIP')[1]/DoIPGeneral/DoIPRoutingActivationCallbackList/*))"!]U

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "DoIPDefensiveProgramming"!][!//

#if (defined DOIP_DEFENSIVE_PROGRAMMING_ENABLED)
#error DOIP_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define DOIP_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_PRECONDITION_ASSERT_ENABLED)
#error DOIP_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define DOIP_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true') and (DoIPPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_POSTCONDITION_ASSERT_ENABLED)
#error DOIP_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define DOIP_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true') and (DoIPPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_UNREACHABLE_CODE_ASSERT_ENABLED)
#error DOIP_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define DOIP_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true') and (DoIPUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_INVARIANT_ASSERT_ENABLED)
#error DOIP_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define DOIP_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true') and (DoIPInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined DOIP_STATIC_ASSERT_ENABLED)
#error DOIP_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define DOIP_STATIC_ASSERT_ENABLED           [!//
[!IF "(../DoIPGeneral/DoIPDevelopmentErrorDetect  = 'true') and (DoIPDefProgEnabled = 'true') and (DoIPStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*------------------------[Defensive programming]----------------------------*/

#if (defined DOIP_PRECONDITION_ASSERT)
#error DOIP_PRECONDITION_ASSERT is already defined
#endif

#if (defined DOIP_PRECONDITION_ASSERT_NO_EVAL)
#error DOIP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (DOIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))
#else
#define DOIP_PRECONDITION_ASSERT(Condition, ApiId)
#define DOIP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DOIP_POSTCONDITION_ASSERT)
#error DOIP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined DOIP_POSTCONDITION_ASSERT_NO_EVAL)
#error DOIP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (DOIP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))
#else
#define DOIP_POSTCONDITION_ASSERT(Condition, ApiId)
#define DOIP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DOIP_INVARIANT_ASSERT)
#error DOIP_INVARIANT_ASSERT is already defined
#endif

#if (defined DOIP_INVARIANT_ASSERT_NO_EVAL)
#error DOIP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (DOIP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))
#else
#define DOIP_INVARIANT_ASSERT(Condition, ApiId)
#define DOIP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined DOIP_STATIC_ASSERT)
# error DOIP_STATIC_ASSERT is already defined
#endif
#if (DOIP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define DOIP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define DOIP_STATIC_ASSERT(expr)
#endif

#if (defined DOIP_UNREACHABLE_CODE_ASSERT)
#error DOIP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (DOIP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **
 ** \param[in] ApiId Service ID of the API function */
#define DOIP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(DOIP_MODULE_ID, DOIP_INSTANCE_ID, (ApiId))
#else
#define DOIP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined DOIP_INTERNAL_API_ID)
#error DOIP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define DOIP_INTERNAL_API_ID DET_INTERNAL_API_ID

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/
[!ENDCODE!]
#endif /* DOIP_CFG_H */
/*==================[end of file]===========================================*/
