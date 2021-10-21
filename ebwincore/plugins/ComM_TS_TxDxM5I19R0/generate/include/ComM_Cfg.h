/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO ComM456,1, ComM463_Refine_ComM_Cfg,1 */
#ifndef COMM_CFG_H
#define COMM_CFG_H

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.5 (required)
 * An external object or function shall be declared in one and only one file.
 *
 * Reason:
 * The files SchM_ComM.h and ComM_Cfg.h contain declarations of ComM_MainFunction_x. This
 * is due to the issue reported in http://www.autosar.org/bugzilla/show_bug.cgi?id=53853
 * and should be fixed in a future version of the Rte.
 */

/*==================[includes]==============================================*/

[!INCLUDE "../../generate_macros/ComM_Functions.m"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!]
[!CALL "GetMasterCoreId"!][!//
[!CALL "GetUsedCores"!][!//
[!ENDIF!][!//
[!ENDIF!][!//

[!IF "ComMGeneral/ComMPncSupport = 'true'"!]
#include <Com.h>
[!ENDIF!]


/*==================[macros]================================================*/
/* *** General feature flags *** */

/* !LINKSTO ComM.PB.Impl.Hash,1 */
#if (defined COMM_CFG_SIGNATURE)
#error COMM_CFG_SIGNATURE already defined
#endif
#define COMM_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('ComM')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

#if (defined COMM_PUBLIC_INFO_SIGNATURE)
#error COMM_PUBLIC_INFO_SIGNATURE already defined
#endif
#define COMM_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('ComM')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('ComM')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

#if (defined COMM_PBCFGM_SUPPORT_ENABLED)
#error COMM_PBCFGM_SUPPORT_ENABLED already defined
#endif
#define COMM_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('ComM')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* ----- Pre-processor switch to enable/diable relocateable postbuild config ----- */
/* !LINKSTO ComM.PB.Impl.Relocatable,1 */
#if (defined COMM_RELOCATABLE_CFG_ENABLE)
#error COMM_RELOCATABLE_CFG_ENABLE already defined
#endif
#if (COMM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define COMM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
#define COMM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

#if (defined COMM_SERVICE_API_NONE)
#error COMM_SERVICE_API_NONE already defined
#endif
/** \brief Definition that no default service API is set **/
#define COMM_SERVICE_API_NONE   0U

#if (defined COMM_SERVICE_API_ASR32)
#error COMM_SERVICE_API_ASR32 already defined
#endif
/** \brief Definition that AUTOSAR 3.2 service API is set as default **/
#define COMM_SERVICE_API_ASR32 32U

#if (defined COMM_SERVICE_API_ASR40)
#error COMM_SERVICE_API_ASR40 already defined
#endif
/** \brief Definition that AUTOSAR 4.0 service API is set as default **/
#define COMM_SERVICE_API_ASR40 40U

#if (defined COMM_ENABLE_ASR32_SERVICE_API)
#error COMM_ENABLE_ASR32_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 3.2 service API for ComM is enabled **/
#define COMM_ENABLE_ASR32_SERVICE_API                   [!//
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_ENABLE_ASR40_SERVICE_API)
#error COMM_ENABLE_ASR40_SERVICE_API already defined
#endif
/** \brief Definition whether AUTOSAR 4.0 service API for ComM is enabled **/
#define COMM_ENABLE_ASR40_SERVICE_API                   [!//
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_DEFAULT_ASR_SERVICE_API)
#error COMM_DEFAULT_ASR_SERVICE_API already defined
#endif
/** \brief Definition of default service API **/
#define COMM_DEFAULT_ASR_SERVICE_API                    [!//
[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI = 'AUTOSAR_32'"!]COMM_SERVICE_API_ASR32
[!ELSEIF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI = 'AUTOSAR_40'"!]COMM_SERVICE_API_ASR40
[!ELSE!]COMM_SERVICE_API_NONE
[!ENDIF!]

[!SELECT "ComMConfigSet/*[1]"!][!//
[!LOOP "ComMChannel/*"!]
#if (defined ComMConf_ComMChannel_[!"name(.)"!])
#error ComMConf_ComMChannel_[!"name(.)"!] already defined
#endif
/** \brief Symbolic name for the ComM channel "[!"name(.)"!]" */
#define ComMConf_ComMChannel_[!"name(.)"!]   [!"ComMChannelId"!]U

#if (defined COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"ComMChannelId"!] */
#define [!"name(.)"!]                     [!"ComMChannelId"!]U

#if (defined ComM_[!"name(.)"!])
#error ComM_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define ComM_[!"name(.)"!]   [!"ComMChannelId"!]U
#endif /* COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]


[!LOOP "ComMUser/*"!]
#if (defined ComMConf_ComMUser_[!"name(.)"!])
#error ComMConf_ComMUser_[!"name(.)"!] already defined
#endif

/** \brief Symbolic name for the ComM user "[!"name(.)"!]" */
#define ComMConf_ComMUser_[!"name(.)"!]   [!"ComMUserIdentifier"!]U

#if (defined COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"ComMUserIdentifier"!] */
#define [!"name(.)"!]                     [!"ComMUserIdentifier"!]U

#if (defined ComM_[!"name(.)"!])
#error ComM_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define ComM_[!"name(.)"!]   [!"ComMUserIdentifier"!]U
#endif /* COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]
[!ENDSELECT!][!//

/** \brief Enable/disable development error detection */
#if (defined COMM_DEV_ERROR_DETECT)
#error COMM_DEV_ERROR_DETECT already defined
#endif
#define COMM_DEV_ERROR_DETECT             [!//
[!IF "ComMGeneral/ComMDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Enable/disable the version info API */
#if (defined COMM_VERSION_INFO_API)
#error COMM_VERSION_INFO_API already defined
#endif
#define COMM_VERSION_INFO_API             [!//
[!IF "ComMGeneral/ComMVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Minimum time duration in ms, spent in the Full Communication
 * Mode */
#if (defined COMM_T_MIN_FULL_COM_MODE_DURATION)
#error COMM_T_MIN_FULL_COM_MODE_DURATION already defined
#endif
#define COMM_T_MIN_FULL_COM_MODE_DURATION [!//
[!"num:i(ComMGeneral/ComMTMinFullComModeDuration * 1000)"!]U

/** \brief Check if Rte is used in ComM */
#if (defined COMM_INCLUDE_RTE)
#error COMM_INCLUDE_RTE already defined
#endif
#define COMM_INCLUDE_RTE                  [!//
[!IF "ComMGeneral/ComMRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Check if Dcm usage is enabled in ComM */
#if (defined COMM_DCM_ENABLED)
#error COMM_DCM_ENABLED already defined
#endif
#define COMM_DCM_ENABLED                  [!//
[!IF "ComMGeneral/ComMDcmUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO ComM.Config.ComMMultiCoreSupport,1 */
/** \brief Enable multicore support*/
#if (defined COMM_MULTICORE_ENABLED)
#error COMM_MULTICORE_ENABLED already defined
#endif
#define COMM_MULTICORE_ENABLED           [!//
[!IF "ComMGeneral/ComMMultiCoreSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Wake up of one channel shall lead to a wake up of all channels if
 * switch is enabled. */
#if (defined COMM_SYNCHRONOUS_WAKE_UP)
#error COMM_SYNCHRONOUS_WAKE_UP already defined
#endif
#define COMM_SYNCHRONOUS_WAKE_UP          [!//
[!IF "ComMGeneral/ComMSynchronousWakeUp = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief mode limitation functionality for channel shall lead to limit mode changing if
 * switch is enabled. **/
#if (defined COMM_MODE_LIMITATION_ENABLED)
#error COMM_MODE_LIMITATION_ENABLED already defined
#endif
#define COMM_MODE_LIMITATION_ENABLED      [!//
[!IF "ComMGeneral/ComMModeLimitationEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief mode inhibition affects the ECU or not. **/
#if (defined COMM_ECU_GROUP_CLASSIFICATION)
#error COMM_ECU_GROUP_CLASSIFICATION already defined
#endif
#define COMM_ECU_GROUP_CLASSIFICATION     [!//
[!"num:i(ComMGeneral/ComMEcuGroupClassification)"!]U

/** \brief wakeup inhibition functionality for channel shall limit wakeup if
 * switch is enabled. **/
#if (defined COMM_WAKEUP_INHIBITION_ENABLED)
#error COMM_WAKEUP_INHIBITION_ENABLED already defined
#endif
#define COMM_WAKEUP_INHIBITION_ENABLED    [!//
[!IF "ComMGeneral/ComMWakeupInhibitionEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief signal if the port interface ComM_CurrentChannelRequest is used */
#if (defined COMM_CURRENTCHANNELREQUEST_ENABLED)
#error COMM_CURRENTCHANNELREQUEST_ENABLED already defined
#endif
#define COMM_CURRENTCHANNELREQUEST_ENABLED  [!//
[!IF "(count(ComMConfigSet/*[1]/ComMChannel/*[ComMFullCommRequestNotificationEnabled = 'true']) > 0) and (ComMGeneral/ComMRteUsage = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "ComMGeneral/ComMWakeupInhibitionEnabled = 'true' or ComMGeneral/ComMModeLimitationEnabled = 'true'"!][!//
/** \brief Number of bytes in Nvm for storing ComM channel status */
#if (defined COMM_NUM_BYTES_NVM)
#error COMM_NUM_BYTES_NVM already defined
#endif
#define COMM_NUM_BYTES_NVM                [!//
[!"num:i((((count(ComMConfigSet/*[1]/ComMChannel/*))- 1 )div 8)+1)"!]U
[!ENDIF!][!//

[!//
[!VAR "NvMEnabled"!][!IF "node:exists(ComMGeneral/ComMGlobalNvMBlockDescriptor)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//

/** \brief Enable/Disable NvM support */
#if (defined COMM_NVM_ENABLED)
#error COMM_NVM_ENABLED already defined
#endif
#define COMM_NVM_ENABLED                  [!"$NvMEnabled"!]

[!IF "($NvMEnabled = 'STD_ON') and ((ComMGeneral/ComMWakeupInhibitionEnabled = 'true') or (ComMGeneral/ComMModeLimitationEnabled = 'true'))"!][!//
/** \brief NvM block Id for ComM block Id */
#if (defined COMM_NVM_BLOCK_DESCRIPTOR)
#error COMM_NVM_BLOCK_DESCRIPTOR already defined
#endif
#define COMM_NVM_BLOCK_DESCRIPTOR         [!//
[!"num:i(as:ref(ComMGeneral/ComMGlobalNvMBlockDescriptor)/NvMNvramBlockIdentifier)"!]U
[!ENDIF!][!//

[!IF "ComMGeneral/ComMPncSupport = 'true'"!]
[!CALL "GetPnInfoLength"!]

/** \brief Number of bytes to store the EIRA or ERA */
#if (defined COMM_PN_INFO_LENGTH)
#error COMM_PN_INFO_LENGTH already defined
#endif
#define COMM_PN_INFO_LENGTH [!"num:i($PnInfoLength)"!]U

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport='true'"!]
[!CALL "GetNoOfTxBuffers"!]
/** \brief Number of Tx buffers (for each signal which is not on the Master Core a buffer is needed) */
#if (defined COMM_NO_OF_TX_BUFFERS)
#error COMM_NO_OF_TX_BUFFERS already defined
#endif
#define COMM_NO_OF_TX_BUFFERS [!"num:i($NoOfTxBuffers)"!]U

[!CALL "GetNoOfRxBuffers"!]
/** \brief Number of Rx buffers (for each signal which is not on the Master Core a buffer is needed) */
#if (defined COMM_NO_OF_RX_BUFFERS)
#error COMM_NO_OF_RX_BUFFERS already defined
#endif
#define COMM_NO_OF_RX_BUFFERS [!"num:i($NoOfRxBuffers)"!]U

[!ENDIF!]

[!ENDIF!][!//

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport='true'"!]
[!CALL "GetMasterCoreId"!]
/** \brief Core on which the Master application is present */
#if (defined COMM_MASTER_CORE_ID)
#error COMM_MASTER_CORE_ID already defined
#endif
#define COMM_MASTER_CORE_ID [!"num:i($MasterCoreId)"!]U
[!ENDIF!]

[!SELECT "ComMConfigSet/*[1]"!][!//
/** \brief Number of ComM communication channels */
#if (defined COMM_NUM_CHANNELS)
#error COMM_NUM_CHANNELS already defined
#endif
#define COMM_NUM_CHANNELS                 [!//
[!"num:i(count(ComMChannel/*))"!]U

/** \brief Number of ComM users */
#if (defined COMM_NUM_USERS)
#error COMM_NUM_USERS already defined
#endif
#define COMM_NUM_USERS                    [!//
[!"num:i(count(ComMUser/*))"!]U

#if (defined USER_ARRAY_SIZE)
#error USER_ARRAY_SIZE already defined
#endif
#if (COMM_NUM_USERS == 0U)
#define USER_ARRAY_SIZE 1U
#else
#define USER_ARRAY_SIZE COMM_NUM_USERS
#endif

/** \brief Flag indicating if Nm access is needed at all.
 *
 * ComM does not need to call Nm if no ComM channel is configured to used
 * either Nm variant PASSIVE or FULL. */
#if (defined COMM_NM_ACCESS_NEEDED)
#error COMM_NM_ACCESS_NEEDED already defined
#endif
#define COMM_NM_ACCESS_NEEDED             [!//
[!IF "count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'PASSIVE') or (ComMNetworkManagement/ComMNmVariant = 'FULL')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if CanSm access is needed at all. */
#if (defined COMM_CANSM_ACCESS_NEEDED)
#error COMM_CANSM_ACCESS_NEEDED already defined
#endif
#define COMM_CANSM_ACCESS_NEEDED          [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_CAN']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if FrSm access is needed at all. */
#if (defined COMM_FRSM_ACCESS_NEEDED)
#error COMM_FRSM_ACCESS_NEEDED already defined
#endif
#define COMM_FRSM_ACCESS_NEEDED           [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_FR']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if LinSm access is needed at all. */
#if (defined COMM_LINSM_ACCESS_NEEDED)
#error COMM_LINSM_ACCESS_NEEDED already defined
#endif
#define COMM_LINSM_ACCESS_NEEDED          [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_LIN']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if EthSm access is needed at all. */
#if (defined COMM_ETHSM_ACCESS_NEEDED)
#error COMM_ETHSM_ACCESS_NEEDED already defined
#endif
#define COMM_ETHSM_ACCESS_NEEDED          [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_ETH']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if there is at least one channel of variant
 * "full" */
#if (defined COMM_NM_VARIANT_FULL_NEEDED)
#error COMM_NM_VARIANT_FULL_NEEDED already defined
#endif
#define COMM_NM_VARIANT_FULL_NEEDED       [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'FULL']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if there is at least one channel of variant
 * "passive" */
#if (defined COMM_NM_VARIANT_PASSIVE_NEEDED)
#error COMM_NM_VARIANT_PASSIVE_NEEDED already defined
#endif
#define COMM_NM_VARIANT_PASSIVE_NEEDED    [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'PASSIVE']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if there are only passive channels
 * "passive" */
#if (defined COMM_NM_VARIANT_PASSIVE_ONLY)
#error COMM_NM_VARIANT_PASSIVE_ONLY already defined
#endif
#define COMM_NM_VARIANT_PASSIVE_ONLY    [!//
[!IF "num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'PASSIVE'])) =
      num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Flag indicating if there is at least one channel of variant
 * "light" */
#if (defined COMM_NM_VARIANT_LIGHT_NEEDED)
#error COMM_NM_VARIANT_LIGHT_NEEDED already defined
#endif
#define COMM_NM_VARIANT_LIGHT_NEEDED      [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'LIGHT']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if there is at least one channel of variant
 * "none" */
#if (defined COMM_NM_VARIANT_NONE_NEEDED)
#error COMM_NM_VARIANT_NONE_NEEDED already defined
#endif
#define COMM_NM_VARIANT_NONE_NEEDED       [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'NONE']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO ComM.EB.Dsn.Information.COMM_NM_VARIANT_SLAVE_ACTIVE_NEEDED,1 */
/** \brief Flag indicating if there is at least one channel of variant
 * "SLAVE_ACTIVE" */
#if (defined COMM_NM_VARIANT_SLAVE_ACTIVE_NEEDED)
#error COMM_NM_VARIANT_SLAVE_ACTIVE_NEEDED already defined
#endif
#define COMM_NM_VARIANT_SLAVE_ACTIVE_NEEDED       [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'SLAVE_ACTIVE']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO ComM.EB.Dsn.Information.COMM_NM_VARIANT_SLAVE_PASSIVE_NEEDED,1 */
/** \brief Flag indicating if there is at least one channel of variant
 * "SLAVE_PASSIVE" */
#if (defined COMM_NM_VARIANT_SLAVE_PASSIVE_NEEDED)
#error COMM_NM_VARIANT_SLAVE_PASSIVE_NEEDED already defined
#endif
#define COMM_NM_VARIANT_SLAVE_PASSIVE_NEEDED       [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'SLAVE_PASSIVE']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if only channels of variant "full" are
 * configured */
#if (defined COMM_NM_VARIANT_FULL_ONLY)
#error COMM_NM_VARIANT_FULL_ONLY already defined
#endif
#define COMM_NM_VARIANT_FULL_ONLY         [!//
[!IF "count(ComMChannel/*[ComMNetworkManagement/ComMNmVariant = 'FULL']) = count(ComMChannel/*)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if only channels of variant "light" ore "none" are
 * configured */
#if (defined COMM_NM_VARIANT_LIGHT_NONE_ONLY)
#error COMM_NM_VARIANT_LIGHT_NONE_ONLY already defined
#endif
#define COMM_NM_VARIANT_LIGHT_NONE_ONLY   [!//
[!IF "count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'LIGHT') or (ComMNetworkManagement/ComMNmVariant = 'NONE')]) = count(ComMChannel/*)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if there is at least one channel of bustype
 * "internal" */
#if (defined COMM_BUSTYPE_INTERNAL_NEEDED)
#error COMM_BUSTYPE_INTERNAL_NEEDED already defined
#endif
#define COMM_BUSTYPE_INTERNAL_NEEDED      [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_INTERNAL']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Flag indicating if only channels of bustype "internal" are configured */
#if (defined COMM_BUSTYPE_INTERNAL_ONLY)
#error COMM_BUSTYPE_INTERNAL_ONLY already defined
#endif
#define COMM_BUSTYPE_INTERNAL_ONLY   [!//
[!IF "count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_INTERNAL']) = count(ComMChannel/*)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief NM channel connected to the ComM channel */
#if (defined COMM_NM_CHANNEL_OF_CHANNEL)
#error COMM_NM_CHANNEL_OF_CHANNEL already defined
#endif
#define COMM_NM_CHANNEL_OF_CHANNEL(idx)   [!//
[!IF "count(ComMChannel/*) = 1"!][!//
[!SELECT "ComMChannel/*[1]/ComMNetworkManagement"!][!//
[!IF "(ComMNmVariant = 'FULL') or (ComMNmVariant = 'PASSIVE')"!][!//
[!"num:i(as:modconf('Nm')[1]/NmChannelConfig/*[as:ref(NmComMChannelRef)/ComMChannelId = 0]/NmChannelId)"!]U [!//
/* Fixed mapping of channel specific attributes to attributes of channel #0 */
[!ELSE!][!//
0xFFU /* no NM channel associated */
[!ENDIF!][!//
[!ENDSELECT!][!//
[!ELSE!][!//
(ComM_NmChannelOfChannel[(idx)])
/* Dynamic mapping of channel specific attributes */
[!ENDIF!][!//

[!IF "(count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_CAN']) = count(ComMChannel/*))
or (count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_FR']) = count(ComMChannel/*))
or (count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_LIN']) = count(ComMChannel/*))
or (count(ComMChannel/*[ComMBusType = 'COMM_BUS_TYPE_ETH']) = count(ComMChannel/*))"!][!//
[!/* this also covers the case count(ComMChannel/*) = 1 */!][!//
/* !LINKSTO HisComM0001_Refine1,1, HisComM0002_Refine1,1, HisComM0003_Refine1,1 */
/** \brief Bus type of ComM channel */
#if (defined COMM_BUSTYPE_OF_CHANNEL)
#error COMM_BUSTYPE_OF_CHANNEL already defined
#endif
#define COMM_BUSTYPE_OF_CHANNEL(idx)      [!"ComMChannel/*[1]/ComMBusType"!]

#if (defined COMM_BUSTYPE_OF_CHANNEL_DYNAMIC)
#error COMM_BUSTYPE_OF_CHANNEL_DYNAMIC already defined
#endif
#define COMM_BUSTYPE_OF_CHANNEL_DYNAMIC   STD_OFF
[!ELSE!][!//
/** \brief Bus type of ComM channel */
/* !LINKSTO HisComM0001_Refine1,1, HisComM0002_Refine1,1, HisComM0003_Refine1,1 */
#if (defined COMM_BUSTYPE_OF_CHANNEL)
#error COMM_BUSTYPE_OF_CHANNEL already defined
#endif
#define COMM_BUSTYPE_OF_CHANNEL(idx)      (ComM_BusTypeOfChannel[(idx)])

#if (defined COMM_BUSTYPE_OF_CHANNEL_DYNAMIC)
#error COMM_BUSTYPE_OF_CHANNEL_DYNAMIC already defined
#endif
#define COMM_BUSTYPE_OF_CHANNEL_DYNAMIC   STD_ON

#if (defined COMM_BUSTYPE_OF_CHANNEL_INIT)
#error COMM_BUSTYPE_OF_CHANNEL_INIT already defined
#endif
#define COMM_BUSTYPE_OF_CHANNEL_INIT \
{ \
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  [!"ComMBusType"!], /* for channel [!"name(.)"!] */ \
[!ENDLOOP!][!//
}
[!ENDIF!][!//

[!IF "(count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'FULL')]) = count(ComMChannel/*))
or (count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'PASSIVE')]) = count(ComMChannel/*))
or (count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'LIGHT')]) = count(ComMChannel/*))
or (count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'NONE')]) = count(ComMChannel/*))
or (count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'SLAVE_ACTIVE')]) = count(ComMChannel/*))
or (count(ComMChannel/*[(ComMNetworkManagement/ComMNmVariant = 'SLAVE_PASSIVE')]) = count(ComMChannel/*))"!][!//
[!/* this also covers the case count(ComMChannel/*) = 1 */!][!//
/** \brief Nm variant of ComM channel */
/* !LINKSTO HisComM0001_Refine1,1, HisComM0002_Refine1,1, HisComM0003_Refine1,1 */
#if (defined COMM_NM_VARIANT_OF_CHANNEL)
#error COMM_NM_VARIANT_OF_CHANNEL already defined
#endif
#define COMM_NM_VARIANT_OF_CHANNEL(idx)    [!//
COMM_NM_[!"ComMChannel/*[1]/ComMNetworkManagement/ComMNmVariant"!]_VARIANT
#if (defined COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC)
#error COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC already defined
#endif
#define COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC STD_OFF
[!ELSE!][!//
#if (defined COMM_NM_VARIANT_OF_CHANNEL)
#error COMM_NM_VARIANT_OF_CHANNEL already defined
#endif
#define COMM_NM_VARIANT_OF_CHANNEL(idx)    (ComM_NmVariantOfChannel[(idx)])

#if (defined COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC)
#error COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC already defined
#endif
#define COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC STD_ON

#if (defined COMM_NM_VARIANT_OF_CHANNEL_INIT)
#error COMM_NM_VARIANT_OF_CHANNEL_INIT already defined
#endif
#define COMM_NM_VARIANT_OF_CHANNEL_INIT \
{ \
[!LOOP "node:order(ComMChannel/*, 'ComMChannelId')"!][!//
  COMM_NM_[!"ComMNetworkManagement/ComMNmVariant"!]_VARIANT, /* for channel [!"name(.)"!] */ \
[!ENDLOOP!][!//
}
[!ENDIF!][!//

[!ENDSELECT!][!//

#if (defined COMM_PNC_RELEASE_CHANNEL)
#error COMM_PNC_RELEASE_CHANNEL already defined
#endif
#define COMM_PNC_RELEASE_CHANNEL    [!//
[!IF "ComMGeneral/ComMPncReleaseChannel = 'COMM_NO_INTERNAL_REQUESTS'"!][!//
COMM_NO_INTERNAL_REQUESTS
[!ELSE!][!//
COMM_NO_INTERNAL_AND_EXTERNAL_REQUESTS
[!ENDIF!]

#if (defined COMM_RELEASE_NONE_CHANNELS)
#error COMM_RELEASE_NONE_CHANNELS already defined
#endif
#define COMM_RELEASE_NONE_CHANNELS [!//
[!IF "ComMGeneral/ComMReleaseNoneChannels = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO ECUC_ComM_00558,1 */
#if (defined COMM_RESET_AFTER_FORCING_NOCOMM)
#error COMM_RESET_AFTER_FORCING_NOCOMM already defined
#endif
#define COMM_RESET_AFTER_FORCING_NOCOMM [!//
[!IF "ComMGeneral/ComMResetAfterForcingNoComm = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*------------------[Pnc configuration]-------------------------------------*/
/** \brief General Support for Partial network cluster (Pnc) */
#if (defined COMM_PNC_SUPPORT)
#error COMM_PNC_SUPPORT already defined
#endif
#define COMM_PNC_SUPPORT                  [!//
[!IF "ComMGeneral/ComMPncSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined COMM_PNC_GW_ENABLED)
#error COMM_PNC_GW_ENABLED already defined
#endif
[!IF "node:exists(ComMGeneral/ComMPncGatewayEnabled)"!][!//
/** \brief Support for Partial network cluster gateway */
#define COMM_PNC_GW_ENABLED               [!//
[!IF "ComMGeneral/ComMPncGatewayEnabled = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#define COMM_PNC_GW_ENABLED    STD_OFF
[!ENDIF!]

#if (defined COMM_0_PNC_VECTOR_AVOIDANCE)
#error COMM_0_PNC_VECTOR_AVOIDANCE already defined
#endif
[!IF "ComMGeneral/ComMPncGatewayEnabled = 'true'"!][!//
/** \brief Support for ComM 0 Partial Network Cluster Vector Avoidance */
#define COMM_0_PNC_VECTOR_AVOIDANCE       [!//
[!IF "node:exists(ComMGeneral/ComM0PncVectorAvoidance) and (ComMGeneral/ComM0PncVectorAvoidance = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ELSE!]
#define COMM_0_PNC_VECTOR_AVOIDANCE       STD_OFF
[!ENDIF!]

#if (defined COMM_TX_ERA_PASSIVE)
#error COMM_TX_ERA_PASSIVE already defined
#endif
#define COMM_TX_ERA_PASSIVE          0U

#if (defined COMM_TX_ERA_ACTIVE_OR_EIRA)
#error COMM_TX_ERA_ACTIVE_OR_EIRA already defined
#endif
#define COMM_TX_ERA_ACTIVE_OR_EIRA   1U

/** \brief Number of Partial network cluster (Pnc) */
#if (defined COMM_NUM_PNC)
#error COMM_NUM_PNC already defined
#endif
#define COMM_NUM_PNC                      [!//
[!IF "ComMGeneral/ComMPncSupport = 'true'"!][!"num:i(count(ComMConfigSet/*[1]/ComMPnc/*))"!]U[!ELSE!]0U[!ENDIF!]

[!IF "ComMGeneral/ComMPncSupport = 'true'"!]

[!VAR "LeastMainFnPeriod" = "num:min(ComMConfigSet/*[1]/ComMChannel/*/ComMMainFunctionPeriod)"!][!//

/** \brief Prepare Sleep timeout time in ms, spent in the
 *  PNC_PREPARE_SLEEP state */
#if (defined COMM_T_PNC_PREPARE_SLEEP)
#error COMM_T_PNC_PREPARE_SLEEP already defined
#endif
#define COMM_T_PNC_PREPARE_SLEEP [!"num:i((ComMGeneral/ComMPncPrepareSleepTimer * 1000) div ($LeastMainFnPeriod * 1000))"!]U
[!ENDIF!][!//

[!SELECT "ComMConfigSet/*[1]"!][!//
[!LOOP "ComMPnc/*"!]
#if (defined ComMConf_ComMPnc_[!"name(.)"!])
#error ComMConf_ComMPnc_[!"name(.)"!] already defined
#endif
/** \brief Symbolic name for the ComM Pnc "[!"name(.)"!]" */
#define ComMConf_ComMPnc_[!"name(.)"!]   [!"ComMPncId"!]U

#if (defined COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) [!"ComMPncId"!] */
#define [!"name(.)"!]                     [!"ComMPncId"!]U

#if (defined ComM_[!"name(.)"!])
#error ComM_[!"name(.)"!] already defined
#endif

/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define ComM_[!"name(.)"!]   [!"ComMPncId"!]U
#endif /* COMM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

/** \brief Number of unique Rx EIRA Signals */
#if (defined COMM_NUM_RX_EIRA_SIGNALS)
#error COMM_NUM_RX_EIRA_SIGNALS already defined
#endif
#define COMM_NUM_RX_EIRA_SIGNALS      [!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!"num:i(count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA'][ComMPncComSignalDirection='RX']/ComMPncComSignalRef)))"!]U[!ELSE!]0U[!ENDIF!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
/** \brief Number of unique Rx Active ERA Signals */
#if (defined COMM_NUM_RX_ACTIVE_ERA_SIGNALS)
#error COMM_NUM_RX_ACTIVE_ERA_SIGNALS already defined
#endif
#define COMM_NUM_RX_ACTIVE_ERA_SIGNALS   [!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!"num:i(count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA'][ComMPncComSignalDirection='RX'][node:exists(ComMPncComSignalChannelRef) and (node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType='COMM_GATEWAY_TYPE_ACTIVE')]/ComMPncComSignalRef)))"!]U[!ELSE!]0U[!ENDIF!]

/** \brief Number of unique Rx Active ERA Signals */
/* Number represents all the ERA signals (with channel type ACTIVE and channel type PASSIVE) */
#if (defined COMM_NUM_RX_UNFILTERED_ERA_SIGNALS)
#error COMM_NUM_RX_UNFILTERED_ERA_SIGNALS already defined
#endif
#define COMM_NUM_RX_UNFILTERED_ERA_SIGNALS   [!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!"num:i(count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA'][ComMPncComSignalDirection='RX']/ComMPncComSignalRef)))"!]U[!ELSE!]0U[!ENDIF!]
[!ENDIF!]

/** \brief Number of unique Tx EIRA Signals */
#if (defined COMM_NUM_TX_SIGNALS)
#error COMM_NUM_TX_SIGNALS already defined
#endif
#define COMM_NUM_TX_SIGNALS      [!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!"num:i(count(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef)))"!]U[!ELSE!]0U[!ENDIF!]

[!ENDSELECT!][!//

[!IF "node:exists(ComMGeneral/ComMDirectUserMapping) and (ComMGeneral/ComMDirectUserMapping = 'true')"!][!//
/** \brief Short Name of ComM users used by BswM when direct user
 *  mapping is enabled */
[!LOOP "ComMConfigSet/*[1]/ComMPnc/*"!][!//
#if (defined PNCUser_[!"ComMPncId"!])
#error PNCUser_[!"ComMPncId"!] already defined
#endif
#define  PNCUser_[!"ComMPncId"!]  [!"ComMPncId"!]U
[!ENDLOOP!][!//

/** \brief Short Name of ComM channels used by BswM when direct user
 *  mapping is enabled */
[!LOOP "ComMConfigSet/*[1]/ComMChannel/*"!][!//
#if (defined ChannelUser_[!"ComMChannelId"!])
#error ChannelUser_[!"ComMChannelId"!] already defined
#endif
#define  ChannelUser_[!"ComMChannelId"!]  [!"ComMChannelId"!]U
[!ENDLOOP!][!//
[!ENDIF!][!//



/** \brief VLAN support  */
#if (defined COMM_VLAN_SUPPORT)
#error COMM_VLAN_SUPPORT already defined
#endif
#define COMM_VLAN_SUPPORT               [!//
[!IF "(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*/ComMManageReference/*[1]) >0) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/** \brief Number of managed channels */
#if (defined COMM_VLAN_MANAGED_CH_NB)
#error COMM_VLAN_MANAGED_CH_NB already defined
#endif
#define COMM_VLAN_MANAGED_CH_NB               [!//
[!"num:i(count(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMChannel/*/ComMManageReference/*[1]))"!]U




/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* hide declaration in own header if included by ComM itself and RTE is
 * enabled */
#if ((!defined(COMM_INTERNAL_USAGE)) || (COMM_INCLUDE_RTE == STD_OFF))

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/* If Rte usage is enabled in the ComM configuration the declaration of these
 * functions are provided by Rte_ComM.h. */

[!LOOP "ComMConfigSet/*[1]/ComMChannel/*"!][!//
#ifndef RTE_SCHM_SWCBSW_COMM_MAINFUNCTION_[!"ComMChannelId"!]
#define RTE_SCHM_SWCBSW_COMM_MAINFUNCTION_[!"ComMChannelId"!]
/** \brief Main function for channel [!"ComMChannelId"!] */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE) ComM_MainFunction_[!"ComMChannelId"!](void);

#endif /* RTE_SCHM_SWCBSW_COMM_MAINFUNCTION */
[!ENDLOOP!][!//

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#endif

#if (COMM_PNC_SUPPORT == STD_ON)


#if (defined COMM_PN_OFFSET)
#error COMM_PN_OFFSET already defined
#endif
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!CALL "macroGetPnOffset"!][!//
#define COMM_PN_OFFSET [!"num:i($macroRetValGetPnOffset)"!]U
[!ELSE!]
#define COMM_PN_OFFSET
[!ENDIF!]

/** \brief COM Callback function indicates reception of an RX-EIRA
 **
 ** This function is called from the COM module in case the PDU including the
 ** eira signal is received.
 **
 ** \note called in interrupt context */
[!LOOP "node:refs(ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA'][ComMPncComSignalDirection='RX']/ComMPncComSignalRef)"!]

[!VAR "Generated" = "'false'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/** \brief Com callback function for the signal "[!"node:name(.)"!]" */
extern FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!]
(void);
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
      [!VAR "Generated" = "'true'"!][!//
    [!ENDIF!]
  [!ENDIF!]
[!ENDIF!]
[!IF "$Generated = 'false'"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/** \brief Com callback function for the signal "[!"node:name(.)"!]" */
extern FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!]
(void);
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
[!ENDIF!]

[!ENDLOOP!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
/** \brief COM Callback function indicates reception of an RX-ERA
 **
 ** This function is called from the COM module in case the PDU including the
 ** eira signal is received.
 **
 ** \note called in interrupt context */
[!LOOP "node:refs(ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA'][ComMPncComSignalDirection='RX']/ComMPncComSignalRef)"!]
[!VAR "Generated" = "'false'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/** \brief Com callback function for the signal "[!"node:name(.)"!]" */
extern FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!]
(void);
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
      [!VAR "Generated" = "'true'"!][!//
    [!ENDIF!]
  [!ENDIF!]
[!ENDIF!]
[!IF "$Generated = 'false'"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/** \brief Com callback function for the signal "[!"node:name(.)"!]" */
extern FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!]
(void);
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
[!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]


#endif

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef COMM_CFG_H */
