#ifndef FEE_CFG_H
#define FEE_CFG_H
/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \version 6.14.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!AUTOSPACING!]
[!VAR "VendorApiInfix" = "'EB'"!]
[!VAR "VendorId" = "1"!]
/*==================[inclusions]============================================*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <MemIf_Types.h>
/* !LINKSTO FEE084,1, FEE002,1 */
[!IF "(node:exists(as:modconf('Fls')[1]/CommonPublishedInformation/VendorApiInfix)) and (not(as:modconf('Fls')[1]/CommonPublishedInformation/VendorApiInfix = ''))"!][!//
[!VAR "VendorApiInfix" = "as:modconf('Fls')[1]/CommonPublishedInformation/VendorApiInfix"!][!//
[!VAR "VendorId" = "as:modconf('Fls')[1]/CommonPublishedInformation/VendorId"!][!//
#include <Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!].h>
[!ELSE!]
#include <Fls.h>
[!ENDIF!]
/* !LINKSTO FEE002,1 */
#include <NvM_Cbk.h>  /* NvM callback interface */
[!IF "((node:exists(FeeGeneral/FeeNvmJobEndNotification)) and
(FeeGeneral/FeeNvmJobEndNotification != '') and (FeeGeneral/FeeNvmJobEndNotification != 'NULL_PTR'))
or ((node:exists(FeeGeneral/FeeNvmJobErrorNotification)) and
(FeeGeneral/FeeNvmJobErrorNotification != '') and (FeeGeneral/FeeNvmJobErrorNotification != 'NULL_PTR'))"!]
#include <NvM_Cbk.h>  /* NvM callback interface */
[!ENDIF!]
[!INCLUDE "Fee_Checks.m"!]

/*==================[macros]================================================*/

/** \brief The total flash size
 **
 ** This parameter defines the total flash size. */
#if (defined FEE_TOTAL_FLASH_SIZE)
#error FEE_TOTAL_FLASH_SIZE is already defined
#endif
#define FEE_TOTAL_FLASH_SIZE                       [!"num:inttohex($TotalFlashSize)"!]U
 
/** \brief Represents the number of blocks configured by the user */
#if (defined FEE_NO_OF_CONFIGURED_BLOCKS)
#error FEE_NO_OF_CONFIGURED_BLOCKS is already defined
#endif
#define FEE_NO_OF_CONFIGURED_BLOCKS                [!"num:integer(count(FeeBlockConfiguration/*))"!]U

/** \brief Represents the size of the biggest block */
#if (defined FEE_BIGGEST_BLOCK_SIZE)
#error FEE_BIGGEST_BLOCK_SIZE is already defined
#endif
#define FEE_BIGGEST_BLOCK_SIZE                     [!"num:inttohex($LargestBlockSizeInSection)"!]U

#if (defined FEE_NUMBER_NOT_CONFIGURED_BLOCKS)
#error FEE_NUMBER_NOT_CONFIGURED_BLOCKS is already defined
#endif
/** \brief Represents the number of not-configured blocks that Fee can handle */
#define FEE_NUMBER_NOT_CONFIGURED_BLOCKS           [!"$NumberOfNotConfigBlocks"!]U

#if (defined FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE)
#error FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE is already defined
#endif
/** \brief Represents the upper layer data size for not-configured blocks that Fee can handle */
#define FEE_NOT_CONFIGURED_BLOCKS_DATA_SIZE        [!"$NotConfigBlocksDataSize"!]U

#if (defined FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE)
#error FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE is already defined
#endif
/** \brief Represents the total size reserved for the not-configured blocks that Fee can handle */
#define FEE_NOT_CONFIGURED_BLOCKS_TOTAL_SIZE       [!"$NotConfigBlocksTotalSizeAl"!]U

#if (defined FEE_TOTAL_NUMBER_OF_BLOCKS)
#error FEE_TOTAL_NUMBER_OF_BLOCKS is already defined
#endif
/** \brief Total number of blocks (configured and not configured) to be used by Fee */
#define FEE_TOTAL_NUMBER_OF_BLOCKS                 [!"num:integer(count(FeeBlockConfiguration/*) + $NumberOfNotConfigBlocks)"!]U

[!SELECT "FeeGeneral"!]

/** \brief Number of sections
 **
 ** This parameter defines the total number of sections. */
#if (defined FEE_NUMBER_OF_SECTIONS)
#error FEE_NUMBER_OF_SECTIONS is already defined
#endif
#define FEE_NUMBER_OF_SECTIONS                     [!"FeeNumberOfSections"!]U


/** \brief Represents the size of internal buffer for writing block data and
 ** reading block infos */
#if (defined FEE_BUFFER_SIZE)
#error FEE_BUFFER_SIZE is already defined
#endif
#define FEE_BUFFER_SIZE                            [!"$BufferSize"!]U

/** \brief Represents the size of internal buffer used at init to cache blocks */
#if (defined FEE_INIT_BUFFER_SIZE)
#error FEE_INIT_BUFFER_SIZE is already defined
#endif
#define FEE_INIT_BUFFER_SIZE                       [!"$InitBufferSize"!]U

/** \brief Represents the size of internal buffer in 32bit elements */
#if (defined FEE_BUFFER_SIZE_32)
#error FEE_BUFFER_SIZE_32 is already defined
#endif
#define FEE_BUFFER_SIZE_32                         [!"$BufferSize32"!]U

/** \brief Virtual page size */
#if (defined FEE_VIRTUAL_PAGE_SIZE)
#error FEE_VIRTUAL_PAGE_SIZE is already defined
#endif
#define FEE_VIRTUAL_PAGE_SIZE                      [!"FeeVirtualPageSize"!]U

/** \brief ConfigId computed over Virtual Page, Number of sections, Sections addresses and sizes  */
#if (defined FEE_CONFIG_ID)
#error FEE_CONFIG_ID is already defined
#endif
#define FEE_CONFIG_ID                              [!"$ConfigId"!]U

/** \brief LegacyConfigId computed over Virtual Page, Number of sections, Total flash data size */
#if (defined FEE_LEGACY_CONFIG_ID)
#error FEE_LEGACY_CONFIG_ID is already defined
#endif
#define FEE_LEGACY_CONFIG_ID                       [!"$ConfigId_Legacy"!]U

/** \brief The size of consistency pattern markers */
#define FEE_CONSISTENCY_PATTERN_SIZE               [!"$EndPatternSize"!]U

/** \brief Size of virtual page aligned block info */
#if (defined FEE_BLOCKINFO_ALIGNED_SIZE)
#error FEE_BLOCKINFO_ALIGNED_SIZE is already defined
#endif
#define FEE_BLOCKINFO_ALIGNED_SIZE                 [!"$AlignedBlockInfoSize"!]U


/** \brief Number of bytes used by header fields when aligned to virtual page */
#if (defined FEE_SECTION_STATUS_ALIGNED_SIZE)
#error FEE_SECTION_STATUS_ALIGNED_SIZE is already defined
#endif
#define FEE_SECTION_STATUS_ALIGNED_SIZE            [!"$HeaderStatusFieldSize"!]U

/** \brief Number of bytes used by the Erase Counter field */
#if (defined FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL)
#error FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL is already defined
#endif
#define FEE_SECTION_MANAGEMENT_OBJ_SIZE_AL         [!"$SectionManagementObjectSizeAl"!]U

/** \brief Size of the 5 fields of the section header (Erase Counter Field, Active Field,
 *         Erasable Field, Crc, ConfigId, SectionCounter and its inverted)
 */
#if (defined FEE_HEADER_SIZE)
#error FEE_HEADER_SIZE is already defined
#endif
#define FEE_HEADER_SIZE                            [!"$HeaderSize"!]U

/** \brief Offset of erasable field from header start address */
#if (defined FEE_HEADER_ERASABLE_MARKER_OFFSET)
#error FEE_HEADER_ERASABLE_MARKER_OFFSET is already defined
#endif
#define FEE_HEADER_ERASABLE_MARKER_OFFSET          0U

/** \brief Offset of erase counter field from header start address */
#if (defined FEE_SECTION_MANAGEMENT_OFFSET)
#error FEE_SECTION_MANAGEMENT_OFFSET is already defined
#endif
#define FEE_SECTION_MANAGEMENT_OFFSET              [!"num:integer($HeaderStatusFieldSize)"!]U

/** \brief Offset of header Crc */
#if (defined FEE_ECRCFIELD_OFFSET)
#error FEE_ECRCFIELD_OFFSET is already defined
#endif
#define FEE_ECRCFIELD_OFFSET                       [!"num:integer($HeaderStatusFieldSize + $HeaderEraseCounter)"!]U

/** \brief Offset of configId and section counter Crc */
#if (defined FEE_CRCFIELD_OFFSET)
#error FEE_CRCFIELD_OFFSET is already defined
#endif
#define FEE_CRCFIELD_OFFSET                        [!"num:integer($HeaderStatusFieldSize + $HeaderEraseCounter + $ECounterCrc)"!]U

/** \brief Offset of header Config Id */
#if (defined FEE_CIDFIELD_OFFSET)
#error FEE_CIDFIELD_OFFSET is already defined
#endif
#define FEE_CIDFIELD_OFFSET                        [!"num:integer($HeaderStatusFieldSize + $HeaderEraseCounter + $ECounterCrc + $CidCrc)"!]U

 /** \brief Offset of SectionCounter */
#if (defined FEE_SECTIONCOUNTERFIELD_OFFSET)
#error FEE_SECTIONCOUNTERFIELD_OFFSET is already defined
#endif
#define FEE_SECTIONCOUNTERFIELD_OFFSET             [!"num:integer($HeaderStatusFieldSize + $HeaderEraseCounter + $ECounterCrc + $CidCrc + $HeaderConfigId)"!]U

/** \brief Offset of active field from header start address */
#if (defined FEE_HEADER_ACTIVE_MARKER_OFFSET)
#error FEE_HEADER_ACTIVE_MARKER_OFFSET is already defined
#endif
#define FEE_HEADER_ACTIVE_MARKER_OFFSET            [!"num:integer($SectionManagementObjectSizeAl + $HeaderStatusFieldSize + $EndPatternSize)"!]U

[!IF "(not(node:exists(FeeConsistencyPattern)) or ((node:exists(FeeConsistencyPattern)) and (FeeConsistencyPattern = 'false')))"!][!//
 /** \brief Offset of section management end pattern field from header start address */
#if (defined FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET)
#error FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET is already defined
#endif
#define FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET    0U
[!ELSE!]
/** \brief Offset of section management end pattern field from header start address */
#if (defined FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET)
#error FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET is already defined
#endif
#define FEE_SECTION_MANAGEMENT_END_PATTERN_OFFSET    [!"num:integer($SectionManagementObjectSizeAl + $HeaderStatusFieldSize)"!]U
[!ENDIF!]

/** \brief Erase counter API on\off compiler switch */
#if (defined FEE_ERASE_COUNTER_API)
#error FEE_ERASE_COUNTER_API is already defined
#endif
#define FEE_ERASE_COUNTER_API                        [!//
[!IF "FeeEraseCounterApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief FreezeActivities API on\off compiler switch */
#if (defined FEE_FREEZE_ACTIVITIES_API)
#error FEE_FREEZE_ACTIVITIES_API is already defined
#endif
[!IF "(node:exists(FeeFreezeActivitiesApi) and (FeeFreezeActivitiesApi = 'true'))"!][!//
#define FEE_FREEZE_ACTIVITIES_API                    STD_ON
[!ELSE!]
#define FEE_FREEZE_ACTIVITIES_API                    STD_OFF
[!ENDIF!]

/** \brief DynamicBlockLength on\off compiler switch */
#if (defined FEE_DYNAMIC_BLOCK_LENGTH)
#error FEE_DYNAMIC_BLOCK_LENGTH is already defined
#endif
[!IF "(node:exists(FeeDynamicBlockLength) and (FeeDynamicBlockLength = 'true'))"!][!//
#define FEE_DYNAMIC_BLOCK_LENGTH                      STD_ON
[!ELSE!]
#define FEE_DYNAMIC_BLOCK_LENGTH                      STD_OFF
[!ENDIF!]

/** \brief FeeEnableSmallSectionSize on\off compiler switch */
#if (defined FEE_SMALL_SECTION_ENABLED)
#error FEE_SMALL_SECTION_ENABLED is already defined
#endif
[!IF "FeeEnableSmallSectionSize = 'true'"!][!//
#define FEE_SMALL_SECTION_ENABLED                      STD_ON
[!ELSE!]
#define FEE_SMALL_SECTION_ENABLED                      STD_OFF
[!ENDIF!]

/** \brief WriteCustom API on\off compiler switch */
#if (defined FEE_WRITE_CUSTOM_API)
#error FEE_WRITE_CUSTOM_API is already defined
#endif
[!IF "(node:exists(FeeWriteCustomApi) and (FeeWriteCustomApi = 'true'))"!][!//
#define FEE_WRITE_CUSTOM_API                         STD_ON
[!ELSE!]
#define FEE_WRITE_CUSTOM_API                         STD_OFF
[!ENDIF!]

/** \brief Fee_ReadCustom API on\off compiler switch */
#if (defined FEE_READ_CUSTOM_API)
#error FEE_READ_CUSTOM_API is already defined
#endif
[!IF "(node:exists(FeeReadCustomApi) and (FeeReadCustomApi = 'true'))"!][!//
#define FEE_READ_CUSTOM_API                          STD_ON
[!ELSE!]
#define FEE_READ_CUSTOM_API                          STD_OFF
[!ENDIF!]

#if (defined FEE_CANCEL_SECTION_ERASE)
#error FEE_CANCEL_SECTION_ERASE is already defined
#endif
/** \brief Pattern for data consistency on\off compiler switch */
#define FEE_CANCEL_SECTION_ERASE                     [!//
[!IF "((node:exists(FeeCancelSectionEraseApi)) and (FeeCancelSectionEraseApi = 'true'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_CRITICAL_BLOCK_NUMBER)
#error FEE_CRITICAL_BLOCK_NUMBER is already defined
#endif
/** \brief Critical block number */
#define FEE_CRITICAL_BLOCK_NUMBER                    [!"$CriticalBlockNumber"!]U

[!IF "(node:exists(FeeCriticalBlock))"!][!//
/** \brief Critical block size. */
#define FEE_CRITICAL_BLOCK_SIZE                      [!"$CriticalBlockSizeAl"!]U
[!ENDIF!]

/** \brief Pattern for data consistency on\off compiler switch */
#if (defined FEE_CONSISTENCY_PATTERN_ENABLED)
#error FEE_CONSISTENCY_PATTERN_ENABLED is already defined
#endif
#define FEE_CONSISTENCY_PATTERN_ENABLED              [!//
[!IF "((node:exists(FeeConsistencyPattern)) and (FeeConsistencyPattern = 'true'))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Start pattern for data consistency on\off compiler switch */
#if (defined FEE_CONSISTENCY_START_PATTERN_ENABLED)
#error FEE_CONSISTENCY_START_PATTERN_ENABLED is already defined
#endif
#define FEE_CONSISTENCY_START_PATTERN_ENABLED        [!//
[!IF "((node:exists(FeeConsistencyStartPattern)) and (FeeConsistencyStartPattern = 'true')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief The size of combined consistency patterns */
#if (defined FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE)
#error FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE is already defined
#endif
[!IF "(node:exists(FeeConsistencyPattern)) and (FeeConsistencyPattern = 'true') "!]
[!IF "(FeeConsistencyStartPattern = 'true') and (node:exists(FeeConsistencyStartPattern))"!]
#define FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE      (2U * FEE_CONSISTENCY_PATTERN_SIZE)
[!ELSE!]
#define FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE      FEE_CONSISTENCY_PATTERN_SIZE
[!ENDIF!]
[!ELSE!]
#define FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE      0U
[!ENDIF!]

/** \brief Size of virtual page aligned block info and consistency patterns */
#if (defined FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE)
#error FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE is already defined
#endif
#define FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE                (FEE_BLOCKINFO_ALIGNED_SIZE + FEE_CONSISTENCY_PATTERN_ALIGNED_TOTAL_SIZE)

/** \brief Size of management overhead info for a block*/
#if (defined FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE)
#error FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE is already defined
#endif
#define FEE_BLOCK_MANAGEMENT_ALIGNED_TOTAL_SIZE         [!"num:integer($BlockInfoTotalSizeAl + $StartPatternSize + $WrittenMarkerSize)"!]U

/** \brief Number of reserved block info + written marker */
#if (defined FEE_MARGIN)
#error FEE_MARGIN is already defined
#endif
#define FEE_MARGIN                                      [!"num:integer($Margin)"!]U

/** \brief Nvm Job end notification function */
#if (defined FEE_NVM_JOB_END_NOTIFICATION)
#error FEE_NVM_JOB_END_NOTIFICATION is already defined
#endif
#define FEE_NVM_JOB_END_NOTIFICATION() [!//
[!IF "node:exists(FeeNvmJobEndNotification) and (FeeNvmJobEndNotification != '') and (FeeNvmJobEndNotification != 'NULL_PTR')"!]
  [!"FeeNvmJobEndNotification"!]()[!ELSE!] do {} while (0)
[!ENDIF!]

/** \brief Nvm Job error notification function */
#if (defined FEE_NVM_JOB_ERROR_NOTIFICATION)
#error FEE_NVM_JOB_ERROR_NOTIFICATION is already defined
#endif
#define FEE_NVM_JOB_ERROR_NOTIFICATION() [!//
[!IF "node:exists(FeeNvmJobErrorNotification) and (FeeNvmJobErrorNotification != '') and (FeeNvmJobErrorNotification != 'NULL_PTR')"!]
  [!"FeeNvmJobErrorNotification"!]()[!ELSE!] do {} while (0)
[!ENDIF!]

/** \brief Version info on\off compiler switch */
#if (defined FEE_VERSION_INFO_API)
#error FEE_VERSION_INFO_API is already defined
#endif
#define FEE_VERSION_INFO_API               [!//
[!IF "FeeVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Polling Mode on\off compiler switch */
#if (defined FEE_POLLING_MODE)
#error FEE_POLLING_MODE is already defined
#endif
#define FEE_POLLING_MODE                   [!//
[!IF "FeePollingMode = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Development error detect on\off compiler switch */
#if (defined FEE_DEV_ERROR_DETECT)
#error FEE_DEV_ERROR_DETECT is already defined
#endif
#define FEE_DEV_ERROR_DETECT               [!//
[!IF "FeeDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Initialization production error detect on\off compiler switch */
#if (defined FEE_PROD_ERROR_DETECT)
#error FEE_PROD_ERROR_DETECT is already defined
#endif
#define FEE_PROD_ERROR_DETECT              [!//
[!IF "node:exists(FeeProdErrorDetect) and (FeeProdErrorDetect = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Set mode on\off compiler switch */
#if (defined FEE_SET_MODE_SUPPORTED)
#error FEE_SET_MODE_SUPPORTED is already defined
#endif
#define FEE_SET_MODE_SUPPORTED             [!//
[!IF "FeeSetModeSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief InstanceId of this module instance */
#if (defined FEE_INSTANCE_ID)
#error FEE_INSTANCE_ID is already defined
#endif
#define FEE_INSTANCE_ID                    [!"FeeIndex"!]U

/** \brief Compatibility with Fls aligned to 4 bytes */
#if (defined FEE_FLS_ALIGNED_32)
#error FEE_FLS_ALIGNED_32 is already defined
#endif
#define FEE_FLS_ALIGNED_32 [!//
[!IF "(node:exists(FeeUseBufferForJobs)) and (FeeUseBufferForJobs = 'true') "!]
  STD_ON[!ELSE!]STD_OFF
[!ENDIF!]

/** \brief Macro to show if Enable Abort Erase functionality is available */
#if (defined FEE_ENABLE_ABORT_ERASE)
#error FEE_ENABLE_ABORT_ERASE is already defined
#endif
#define FEE_ENABLE_ABORT_ERASE             [!//
[!IF "FeeEnableAbortErase = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!]

[!SELECT "as:modconf('Fls')[1]/FlsGeneral"!]
/** \brief Macro to show if Fls_Cancel API is available */
#if (defined FEE_FLS_CANCEL_API)
#error FEE_FLS_CANCEL_API is already defined
#endif
#define FEE_FLS_CANCEL_API                 [!//
[!IF "FlsCancelApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro to call Fls_Cancel API, if available */
#if (defined FEE_CANCEL_FLASHJOB)
#error FEE_CANCEL_FLASHJOB is already defined
#endif
#define FEE_CANCEL_FLASHJOB() \
  do\
  {\
[!IF "FlsCancelApi = 'true'"!]
    Fee_Gv.FeeFlashJobResult = MEMIF_JOB_CANCELED;\
    Fls_Cancel();\
[!ENDIF!]
  }while(0)

/** \brief Macro to call Fls_SetMode API, if available */
#if (defined FEE_FLS_SETMODE)
#error FEE_FLS_SETMODE is already defined
#endif
#define FEE_FLS_SETMODE(Mode) \
[!IF "FlsSetModeApi = 'true'"!]
  (Fls_SetMode((Mode)))
[!ELSE!][!//
  TS_PARAM_UNUSED(Mode)
[!ENDIF!]
[!ENDSELECT!]

[!IF "node:exists(as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_FLASH_ACCESSIBLE) and (as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_FLASH_ACCESSIBLE != '') and (as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_FLASH_ACCESSIBLE != 'NULL_PTR')"!][!//
/** \brief Flash accessible Dem event Id */
#if (defined FEE_E_FLASH_ACCESSIBLE_DEM_EVENT_ID)
#error FEE_E_FLASH_ACCESSIBLE_DEM_EVENT_ID is already defined
#endif
#define FEE_E_FLASH_ACCESSIBLE_DEM_EVENT_ID    [!//
[!"num:i(node:ref(as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_FLASH_ACCESSIBLE)/DemEventId)"!]U
[!ENDIF!]

[!IF "node:exists(as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_DATA_RECOVERED) and (as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_DATA_RECOVERED != '') and (as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_DATA_RECOVERED != 'NULL_PTR')"!][!//
/** \brief Flash Data Recovered Dem event Id */
#if (defined FEE_E_DATA_RECOVERED_DEM_EVENT_ID)
#error FEE_E_DATA_RECOVERED_DEM_EVENT_ID is already defined
#endif
#define FEE_E_DATA_RECOVERED_DEM_EVENT_ID      [!//
[!"num:i(node:ref(as:modconf('Fee')[1]/FeeDemEventParameterRefs/FEE_E_DATA_RECOVERED)/DemEventId)"!]U
[!ENDIF!]

[!SELECT "as:modconf('Fls')[1]/FlsPublishedInformation"!]
/** \brief Erase value of flash */
#if (defined FEE_FLASH_ERASE_VALUE)
#error FEE_FLASH_ERASE_VALUE is already defined
#endif
#define FEE_FLASH_ERASE_VALUE                  [!"num:inttohex(FlsErasedValue, 2)"!]U

/** \brief Value to fill unused bytes */
#if (defined FEE_PAD_BYTE)
#error FEE_PAD_BYTE is already defined
#endif
#define FEE_PAD_BYTE                           (uint8)(~FEE_FLASH_ERASE_VALUE)

/** \brief Value to indicate Erased section */
#if (defined FEE_SECTION_STATUS_ERASED)
#error FEE_SECTION_STATUS_ERASED is already defined
#endif
#define FEE_SECTION_STATUS_ERASED              [!"num:inttohex(FlsErasedValue, 2)"!][!"substring-after(num:inttohex(FlsErasedValue, 2),'0x')"!]U

/** \brief Value to indicate Erased section */
#if (defined FEE_SECTION_STATUS_ERASED_COUNTER)
#error FEE_SECTION_STATUS_ERASED_COUNTER is already defined
#endif
#define FEE_SECTION_STATUS_ERASED_COUNTER      [!"num:inttohex(FlsErasedValue, 2)"!][!"substring-after(num:inttohex(FlsErasedValue, 2),'0x')"!][!/*
*/!][!"substring-after(num:inttohex(FlsErasedValue, 2),'0x')"!][!"substring-after(num:inttohex(FlsErasedValue, 2),'0x')"!]U
[!ENDSELECT!]
    


/* -----------[Symbolic names for the Fee blocks]--------------------------- */

[!LOOP "FeeBlockConfiguration/*"!][!//
/** \brief Symbolic name for the FEE Block number [!"FeeBlockNumber"!]" */
#define FeeConf_FeeBlockConfiguration_[!"name(.)"!]   [!"FeeBlockNumber"!]U

#ifndef FEE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"name(.)"!]   [!"FeeBlockNumber"!]U
/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Fee_[!"@name"!] [!"FeeBlockNumber"!]U
#endif /* ifndef FEE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/* !LINKSTO Fee.VendorInfixDriver.Support,1 */
[!IF "(node:exists(as:modconf('Fls')[1]/CommonPublishedInformation/VendorApiInfix)) and (not(as:modconf('Fls')[1]/CommonPublishedInformation/VendorApiInfix = ''))"!][!//
/*------------------------[Driver Vendor APIs]------------------------------*/
#define Fls_Write Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_Write
#define Fls_Read Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_Read
[!IF "as:modconf('Fls')[1]/FlsGeneral/FlsCancelApi = 'true'"!]
#define Fls_Cancel Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_Cancel
[!ENDIF!]
#define Fls_GetJobResult Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_GetJobResult
#define Fls_GetStatus Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_GetStatus
[!IF "as:modconf('Fls')[1]/FlsGeneral/FlsSetModeApi = 'true'"!]
#define Fls_SetMode Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_SetMode
[!ENDIF!]
#define Fls_Erase Fls_[!"$VendorId"!]_[!"$VendorApiInfix"!]_Erase

[!ENDIF!][!//



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FeeDefensiveProgramming"!][!//

#if (defined FEE_DEFENSIVE_PROGRAMMING_ENABLED)
#error FEE_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FEE_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_PRECONDITION_ASSERT_ENABLED)
#error FEE_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FEE_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true') and (FeePrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_POSTCONDITION_ASSERT_ENABLED)
#error FEE_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FEE_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true') and (FeePostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FEE_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FEE_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true') and (FeeUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_INVARIANT_ASSERT_ENABLED)
#error FEE_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FEE_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true') and (FeeInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FEE_STATIC_ASSERT_ENABLED)
#error FEE_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FEE_STATIC_ASSERT_ENABLED           [!//
[!IF "(../FeeGeneral/FeeDevErrorDetect  = 'true') and (FeeDefProgEnabled = 'true') and (FeeStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

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

/*==================[end of file]===========================================*/
#endif /* #ifndef FEE_CFG_H */
