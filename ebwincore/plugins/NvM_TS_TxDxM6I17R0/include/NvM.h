/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef NVM_H
#define NVM_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>        /* EB specific standard types                 */

/* Generated configuration data. Defines NVM_INCLUDE_RTE. */
/* !LINKSTO NVM553,1 */
#include <NvM_Cfg.h>
#include <NvM_Api_Stc.h>
#include <NvM_BSW.h>                /* NvM's common API */
#include <NvM_Wrapper.h>

#if ( NVM_INCLUDE_RTE == STD_ON )
# include <Rte_NvM_Type.h>         /* Rte type definitions. */
#endif

/*==================[macros]================================================*/

#if ((NVM_INCLUDE_RTE == STD_OFF) || (NVM_SERVICE_PORT_USED == STD_OFF))
/* These macros are also defined in Rte_NvM_Type.h */
/* !LINKSTO SWS_NvM_00470,2 */
#ifndef NVM_REQ_OK
/** \brief value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the last API request has finished without error. */
#define NVM_REQ_OK                  0U
#endif

#ifndef NVM_REQ_NOT_OK
/** \brief value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the last API request has finished with an error. */
#define NVM_REQ_NOT_OK              1U
#endif

#ifndef NVM_REQ_PENDING
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the last API request has still not finished. */
#define NVM_REQ_PENDING             2U
#endif

#ifndef NVM_REQ_INTEGRITY_FAILED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if for e.g. the CRC check of the last API request has finished. */
#define NVM_REQ_INTEGRITY_FAILED    3U
#endif

#ifndef NVM_REQ_BLOCK_SKIPPED
/** \brief Value of type NvM_RequestResultType returned by
 ** NvM_GetErrorStatus. */
#define NVM_REQ_BLOCK_SKIPPED       4U
#endif

#ifndef NVM_REQ_NV_INVALIDATED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the requested NvM block is invalid. */
#define NVM_REQ_NV_INVALIDATED      5U
#endif

#ifndef NVM_REQ_CANCELED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the request has been cancelled. */
#define NVM_REQ_CANCELED            6U
#endif

#ifndef NVM_REQ_REDUNDANCY_FAILED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the required redundancy of the referenced NV block is lost. */
#define NVM_REQ_REDUNDANCY_FAILED   7U
#endif

#ifndef NVM_REQ_RESTORED_DEFAULTS
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the referenced NV block has been restored with default values. */
#define NVM_REQ_RESTORED_DEFAULTS   8U
#endif

#ifndef NVM_REQ_RESTORED_FROM_ROM
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the referenced NV block has been restored from ROM.defined needed for backwards compatibility 
    with BSW modules that use it. */
#define NVM_REQ_RESTORED_FROM_ROM   NVM_REQ_RESTORED_DEFAULTS
#endif

#endif  /* ((NVM_INCLUDE_RTE == STD_OFF) || (NVM_SERVICE_PORT_USED == STD_OFF)) */

#if(defined(NVM_VARIABLE_BLOCK_LENGTH_EN))
#if (NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON)

#if ( defined(NVM_REQ_OK_BLK_INCREASED) )
#error NVM_REQ_OK_BLK_INCREASED is already defined
#endif
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the Read API request has finished without error but the Block Size was changed , increased. */
#define NVM_REQ_OK_BLK_INCREASED 9u

#if (defined NVM_REQ_OK_BLK_DECREASED)
#error NVM_REQ_OK_BLK_DECREASED is already defined
#endif
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the Read API request has finished without error but the Block Size was changed , decreased. */
#define NVM_REQ_OK_BLK_DECREASED 10u

#endif /* NVM_VARIABLE_BLOCK_LENGTH_EN == STD_ON */
#endif /*defined(NVM_VARIABLE_BLOCK_LENGTH_EN)*/

#if (defined NvM_GetErrorStatus)
#error NvM_GetErrorStatus is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#define NvM_GetErrorStatus NvM_ASR40_GetErrorStatus

#if (defined NvM_SetDataIndex)
#error NvM_SetDataIndex is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#define NvM_SetDataIndex NvM_ASR40_SetDataIndex

#if (defined NvM_GetDataIndex)
#error NvM_GetDataIndex is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#define NvM_GetDataIndex NvM_ASR40_GetDataIndex

#if (defined NvM_SetRamBlockStatus)
#error NvM_SetRamBlockStatus is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#define NvM_SetRamBlockStatus NvM_ASR40_SetRamBlockStatus

#if (defined NvM_ReadBlock)
#error NvM_ReadBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#if (defined NVM_READBLOCK_CALLOUT)
#define NvM_ReadBlock NvM_Wrapper_ReadBlock
#else
#define NvM_ReadBlock NvM_ASR40_ReadBlock
#endif

#if (defined NvM_ReadPRAMBlock)
#error NvM_ReadPRAMBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.2 API as default to other
 ** BSW modules */
#if (defined NVM_READPRAMBLOCK_CALLOUT)
#define NvM_ReadPRAMBlock NvM_Wrapper_ReadPRAMBlock
#else
#define NvM_ReadPRAMBlock NvM_ASR42_ReadPRAMBlock
#endif

#if (defined NvM_WriteBlock)
#error NvM_WriteBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#if (defined NVM_WRITEBLOCK_CALLOUT)
#define NvM_WriteBlock NvM_Wrapper_WriteBlock
#else
#define NvM_WriteBlock NvM_ASR40_WriteBlock
#endif

#if (defined NvM_WritePRAMBlock)
#error NvM_WritePRAMBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.2 API as default to other
 ** BSW modules */
#if (defined NVM_WRITEPRAMBLOCK_CALLOUT)
#define NvM_WritePRAMBlock NvM_Wrapper_WritePRAMBlock
#else
#define NvM_WritePRAMBlock NvM_ASR42_WritePRAMBlock
#endif

#if (defined NvM_RestoreBlockDefaults)
#error NvM_RestoreBlockDefaults is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
#define NvM_RestoreBlockDefaults NvM_Wrapper_RestoreBlockDefaults
#else
#define NvM_RestoreBlockDefaults NvM_ASR40_RestoreBlockDefaults
#endif

#if (defined NvM_RestorePRAMBlockDefaults)
#error NvM_RestorePRAMBlockDefaults is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.2 API as default to other
 ** BSW modules */
#if (defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT)
#define NvM_RestorePRAMBlockDefaults NvM_Wrapper_RestorePRAMBlockDefaults
#else
#define NvM_RestorePRAMBlockDefaults NvM_ASR42_RestorePRAMBlockDefaults
#endif

#if (defined NvM_EraseNvBlock)
#error NvM_EraseNvBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#if (defined NVM_ERASENVBLOCK_CALLOUT)
#define NvM_EraseNvBlock NvM_Wrapper_EraseNvBlock
#else
#define NvM_EraseNvBlock NvM_ASR40_EraseNvBlock
#endif

#if (defined NvM_InvalidateNvBlock)
#error NvM_InvalidateNvBlock is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
#define NvM_InvalidateNvBlock NvM_Wrapper_InvalidateNvBlock
#else
#define NvM_InvalidateNvBlock NvM_ASR40_InvalidateNvBlock
#endif


#if (defined NvM_CancelJobs)
#error NvM_CancelJobs is already defined
#endif
/** \brief Wrapping macro for multicore function to other BSW modules
 **/
#if (defined NVM_CANCELJOBS_CALLOUT)
#define NvM_CancelJobs NvM_Wrapper_CancelJobs
#else
#define NvM_CancelJobs NvM_ASR40_CancelJobs
#endif

#if (defined NvM_SetBlockProtection)
#error NvM_SetBlockProtection is already defined
#endif
/** \brief Wrapping macro to provide AUTOSAR 4.0 API as default to other
 ** BSW modules */
#define NvM_SetBlockProtection NvM_ASR40_SetBlockProtection


/** \brief ****************** API IDs ***********************************************/

#if (defined NVM_SET_DATA_INDEX_API_ID)
#error NVM_SET_DATA_INDEX_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_SetDataIndex().
 **/
#define NVM_SET_DATA_INDEX_API_ID             0x01U

#if (defined NVM_GET_DATA_INDEX_API_ID)
#error NVM_GET_DATA_INDEX_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_GetDataIndex().
 **/
#define NVM_GET_DATA_INDEX_API_ID             0x02U

#if (defined NVM_SET_BLOCK_PROTECTION_API_ID)
#error NVM_SET_BLOCK_PROTECTION_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_SetBlockProtection().
 **/
#define NVM_SET_BLOCK_PROTECTION_API_ID       0x03U

#if (defined NVM_GET_ERROR_STATUS_API_ID)
#error NVM_GET_ERROR_STATUS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_GetErrorStatus().
 **/
#define NVM_GET_ERROR_STATUS_API_ID           0x04U

#if (defined NVM_SET_RAM_BLOCK_STATUS_API_ID)
#error NVM_SET_RAM_BLOCK_STATUS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_SetRamBlockStatus().
 **/
#define NVM_SET_RAM_BLOCK_STATUS_API_ID       0x05U

#if (defined NVM_READ_BLOCK_API_ID)
#error NVM_READ_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_ReadBlock().
 **/
#define NVM_READ_BLOCK_API_ID                 0x06U

#if (defined NVM_WRITE_BLOCK_API_ID)
#error NVM_WRITE_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_WriteBlock().
 **/
#define NVM_WRITE_BLOCK_API_ID                0x07U

#if (defined NVM_RESTORE_BLOCK_DEFAULTS_API_ID)
#error NVM_RESTORE_BLOCK_DEFAULTS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_RestoreBlockDefaults().
 **/
#define NVM_RESTORE_BLOCK_DEFAULTS_API_ID     0x08U

#if (defined NVM_READ_PRAM_BLOCK_API_ID)
#error NVM_READ_PRAM_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_ReadPRAMBlock().
 **/
#define NVM_READ_PRAM_BLOCK_API_ID              0x16U

#if (defined NVM_WRITE_PRAM_BLOCK_API_ID)
#error NVM_WRITE_PRAM_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_WritePRAMBlock().
 **/
#define NVM_WRITE_PRAM_BLOCK_API_ID             0x17U

#if (defined NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID)
#error NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_RestorePRAMBlockDefaults().
 **/
#define NVM_RESTORE_PRAM_BLOCK_DEFAULTS_API_ID  0x18U

#if (defined NVM_ERASE_NV_BLOCK_API_ID)
#error NVM_ERASE_NV_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_EraseNvBlock().
 **/
#define NVM_ERASE_NV_BLOCK_API_ID             0x09U

#if (defined NVM_CANCEL_WRITE_ALL_API_ID)
#error NVM_CANCEL_WRITE_ALL_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_CancelWriteAll().
 **/
#define NVM_CANCEL_WRITE_ALL_API_ID           0x0AU

#if (defined NVM_INVALIDATE_NV_BLOCK_API_ID)
#error NVM_INVALIDATE_NV_BLOCK_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_InvalidateNvBlock().
 **/
#define NVM_INVALIDATE_NV_BLOCK_API_ID        0x0BU

#if (defined NVM_READ_ALL_API_ID)
#error NVM_READ_ALL_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_ReadAll().
 **/
#define NVM_READ_ALL_API_ID                   0x0CU

#if (defined NVM_WRITE_ALL_API_ID)
#error NVM_WRITE_ALL_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_WriteAll().
 **/
#define NVM_WRITE_ALL_API_ID                  0x0DU

#if (defined NVM_VALIDATE_ALL_API_ID)
#error NVM_VALIDATE_ALL_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_ValidateAll().
 **/
#define NVM_VALIDATE_ALL_API_ID               0x19U

#if (defined NVM_GET_VERSION_INFO_API_ID)
#error NVM_GET_VERSION_INFO_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_GetVersionInfo().
 **/
#define NVM_GET_VERSION_INFO_API_ID           0x0FU

#if (defined NVM_CANCEL_JOBS_API_ID)
#error NVM_CANCEL_JOBS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_CancelJobs().
 **/
#define NVM_CANCEL_JOBS_API_ID                0x10U

#if (defined NVM_SET_BLOCK_LOCK_STATUS_API_ID)
#error NVM_SET_BLOCK_LOCK_STATUS_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_SetBlockLockStatus().
 **/
#define NVM_SET_BLOCK_LOCK_STATUS_API_ID      0x13U

#if (defined NVM_ENABLE_BC_API_ID)
#error NVM_ENABLE_BC_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_EnableBc().
 **/
#define NVM_ENABLE_BC_API_ID      0x14U

#if (defined NVM_FIRST_INIT_ALL_API_ID)
#error NVM_FIRST_INIT_ALL_API_ID is already defined
#endif
/** \brief Defines API ID of function NvM_FirstInitAll().
 **/
#define NVM_FIRST_INIT_ALL_API_ID      0x15U

#if (defined NVM_CRYPTO_HOOKS_API_ID)
#error NVM_CRYPTO_HOOKS_API_ID is already defined
#endif
/** \brief Defines API ID of crypto hooks operation...used for DET.
 **/
#define NVM_CRYPTO_HOOKS_API_ID      0x16U

/*==================[external function declarations]========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if ( NVM_VERSION_INFO_API == STD_ON )
/** \brief Service to get the version information of the NvM module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 ** - Module ID
 ** - Vendor ID
 ** - Vendor specific version numbers
 **
 ** Remarks:
 ** - This function can be invoked by the user application although
 **   NvM_Init was not invoked yet.
 **
 ** \param[out] versionInfoPtr Pointer to where to store the version
 **                  information of this module.
 **
 ** \ServiceID{15}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC( void, NVM_CODE ) NvM_GetVersionInfo
(
  P2VAR( Std_VersionInfoType, AUTOMATIC, NVM_APPL_DATA ) versionInfoPtr
);
#endif

/** \brief Service for setting the lock status of the NV block
 **        of an NVRAM block.
 **
 ** If the API is called with parameter Locked as TRUE, the NV contents
 ** associated to the NVRAM block identified by BlockId cannot be modified
 ** by any other requests. The Block is skipped during NvM_WriteAll and
 ** other requests such as NvM_WriteBlock, NvM_InvalidateNvBlock and
 ** NvM_EraseNvBlock are rejected. At next start-up, during processing
 ** of NvM_ReadAll, this NVRAM block will be loaded from NV memory.
 **
 ** If the API is called with parameter Locked as FALSE, this NVRAM block
 ** will be processed normally.
 **
 ** Note 1:  The setting made using this service cannot be changed by
 ** NvM_SetRamBlockStatus or NvM_SetBlockProtection.
 **
 ** Note 2: This service can only be used by BSW Components.
 ** It cannot be accessed via RTE.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] BlockLocked TRUE: Mark the NV block as locked.
 **            FALSE: Mark the NV block as unlocked.
 **
 ** \ServiceID{19}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, NVM_CODE) NvM_SetBlockLockStatus
(
  NvM_ASR40_BlockIdType   BlockId,
  boolean                 BlockLocked
);

#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
/** \brief Enable/disable the Block Check mechanism for a given block.
 **
 ** \retval :  void
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] Enable/Disable value.
 **
 ** \retval :  Std_ReturnType
 **/
extern FUNC(Std_ReturnType,NVM_CODE) NvM_EnableBlockCheck
(
  NvM_BlockIdType BlockId,
  boolean BcEnable
);
/** \brief  Initialize the BlockCheck's internal variables
 **
 ** \retval :  void
 **
 ** \param[in] BcDisable : If TRUE the Block Check mechanism is disabled, otherwise enabled.
 **/
extern FUNC(void,NVM_CODE) NvM_DisableBlockCheckMechanism(boolean BcDisable);
#endif
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef NVM_H */

/*==================[end of file NvM.h]=====================================*/
