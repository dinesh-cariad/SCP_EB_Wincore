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
#ifndef NVM_BSW_H
#define NVM_BSW_H

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 8.5 (required)
 *    An external object or function shall be declared in one
 *    and only one file.
 *
 *  Reason:
 *    The files SchM_NvM.h and NvM_BSW.h contain declarations of
 *    NvM_MainFunction. This is due to the issue reported in
 *    https://jira.autosar.org/browse/AR-16331
 *    and should be fixed in a future version of the Rte.
 *
 */
/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration of 'NvM_MainFunction' [-Wredundant-decls]
 *
 * Reason: The files SchM_NvM.h and NvM_BSW.h contain declarations of
 * NvM_MainFunction. This is due to the issue reported in
 * https://jira.autosar.org/browse/AR-16331
 * and should be fixed in a future version of the Rte.
 *
 */

/*==================[inclusions]============================================*/

/* Generated configuration data. Defines NVM_INCLUDE_RTE. */
#include <NvM_Cfg.h>
#include <NvM_BSW_Types.h>
/*==================[type definitions]======================================*/

/*==================[macros]================================================*/
#if ((NVM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
     (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR32))

/** \cond documentation
 ** \brief Exclude this part from the documentation because
 ** NVM_REQ_REDUNDANCY_FAILED, NVM_REQ_RESTORED_DEFAULTS and NVM_REQ_CANCELED
 ** are already defined in NvM.h */

#ifndef NVM_REQ_REDUNDANCY_FAILED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the the required redundancy of the referenced NV block is lost. */
#define NVM_REQ_REDUNDANCY_FAILED   7U
#endif

#ifndef NVM_REQ_RESTORED_DEFAULTS
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the referenced NV block has been restored from ROM. */
#define NVM_REQ_RESTORED_DEFAULTS   8U
#endif

#ifndef NVM_REQ_CANCELED
/** \brief Value of type NvM_RequestResultType returned by NvM_GetErrorStatus
 ** if the referenced NV block has been restored from ROM. */
#define NVM_REQ_CANCELED  NVM_REQ_CANCELLED
#endif

/** \endcond
 ** \brief */
#endif
/*==================[external function declarations]========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

#if (                                                     \
      (                                                   \
        (!defined(NVM_INTERNAL_USE))                      \
        &&                                                \
        defined(NVM_DOXY_FLAG)                            \
      )                                                   \
      ||                                                  \
      (                                                   \
        (!defined(NVM_INTERNAL_USE))                      \
        &&                                                \
        (!defined(RTE_SCHM_SWCBSW_NVM_MAINFUNCTION))      \
      )                                                   \
      ||                                                  \
      (                                                   \
        (defined(NVM_INTERNAL_USE))                       \
        &&                                                \
        (NVM_INCLUDE_RTE == STD_OFF)                      \
        &&                                                \
        (!defined(RTE_SCHM_SWCBSW_NVM_MAINFUNCTION))      \
      )                                                   \
    )                                                     \


/** \brief Service for performing the processing of the NvM jobs.
 **
 ** The NvM_MainFunction() processes the asynchronous requests and
 ** must be called cyclically. First NvM_MainFunction() checks if a
 ** asynchronous request was inserted in the queue. If the queue is
 ** empty it returns immediately else the first state of the state machine
 ** of the asynchonous function is called. If the asynchronous request
 ** calls a function of an underlying module which cannot be processed
 ** synchronously then this asynchronous request is interrupted until
 ** the NvM_MainFunction() is notified that the underlying function is
 ** finished. If a CRC is calculated and the configuration parameter
 ** NvMCrcNumOfBytes is less than the size of the current NVRAM
 ** Block then the CRC calculation is also interrupted each time NvM-
 ** CrcNumOfBytes bytes of the data are processed. If the asynchronous
 ** request is finished the NvM_MainFunction() removes this request
 ** from the queue and calls the NvM Single Block Job End Notification
 ** callback if it is configured for the current NVRAM Block.
 **
 ** \ServiceID{14}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* Deviation MISRAC2012-1, MGCC62-1 */
extern FUNC( void,NVM_CODE ) NvM_MainFunction( void );

#endif

/** \brief Service to read the block dependent error/status information
 **
 ** The request reads the block dependent error/status information in
 ** the administrative part of a RAM block. The status was set by a
 ** former or current synchronous request.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId. Range: 0..65535.
 ** \param[out] RequestResultPtr Pointer to where to store the request result.
 **             The buffer contains the error status of block 'BlockId'.
 **             Range 0..255.
 ** \return     Std_ReturnType
 ** \retval     E_OK The block dependent error/status information was read
                successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{4}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_GetErrorStatus
(
  NvM_ASR40_BlockIdType                                        BlockId,
  P2VAR( NvM_ASR40_RequestResultType,AUTOMATIC,NVM_APPL_DATA ) RequestResultPtr
);

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service for setting the DataIndex of a dataset NVRAM block
 **
 ** The request sets the specified index to associate a dataset NV Block
 ** (with/without ROM Blocks) with its corresponding RAM Block. The
 ** usage in conjunction with all other block management types is possible,
 ** but without any effect.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId. Range: 0..65535.
 ** \param[in] DataIndex Index position of a NVM Block or ROM Data Block
 **            within a Dataset NVRAM Block. Range: 0..255
 ** \return    Std_ReturnType
 ** \retval    E_OK The index position was set successfully.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetDataIndex
(
  NvM_ASR40_BlockIdType BlockId, uint8 DataIndex
);

/** \brief Service for getting the currently set DataIndex of a dataset NVRAM
 ** block
 **
 ** The service gets the current index (association) of a Dataset Block
 ** (with/without ROM blocks) with its corresponding RAM block. The
 ** usage in conjunction with all other block management types is possible
 ** but without any effect.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId.  Range: 0..65535
 ** \param[out] DataIndex Pointer to where to store the current dataset index
 **             Range 0 .. size of NVM_VAR.
 ** \return     Std_ReturnType
 ** \retval     E_OK The index position has been retrieved successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_GetDataIndex
(
  NvM_ASR40_BlockIdType                        BlockId,
  P2VAR( uint8,AUTOMATIC,NVM_APPL_DATA ) DataIndex
);

/** \brief Service to cancel all pending job requests for an NVRAM Block
 **
 ** This service cancels all queued single block requests(Read, Write,
 ** Erase, Invalidate or Restore) for the NVRAM block referenced by BlockId.
 ** It does not cancel an ongoing job processing.
 **
 ** \param[in] BlockId The Block indentifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId
 ** \return    Std_returnType
 ** \retval    E_OK Request has been accepted
 ** \retval    E_OK Request has not been accepted
 **
 ** ServiceID{16}
 ** Reentrancy{Reentrant}
 ** Synchronicity(Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_CancelJobs
(
  NvM_BlockIdType BlockId
);

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service to erase a NV block
 **
 ** This service initiates erasing the data of this block in NV memory. If
 ** this block has immediate priority the underlying services Ea_EraseIm-
 ** mediateBlock() resp. Fee_EraseImmediateBlock() are used for
 ** erasing. If this block has not immediate priority then this block is only
 ** invalidated via the function NvM_InvalidateBlock(). In both cases a
 ** subsequent NvM_ReadBlock() request for this block results in setting
 ** the error status to NVM_REQ_NV_INVALIDATED.Erasing a standard
 ** priority block is normally faster than erasing an immediate priority
 ** block because only few management data needs to be manipulated
 ** in the NV memory by the underlying Ea/Fee module. However, a
 ** subsequent NvM_WriteBlock() request may take more time because
 ** the NVM data also must be erased by NvM_WriteBlock() before it
 ** can be written. In case of an immediate priority block a
 ** NvM_WriteBlock() request may be faster than for a standard priority
 ** block because the block is already pre erased. If the used NVRAM
 ** hardware does not support separate erase and write commands and
 ** automatically erases upon a write command then there is no significant
 ** time difference in writing the data of an immediate priority block
 ** and a standard priority block. Please see the User's Guide of the
 ** EEPROM driver to get more details.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{9}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_EraseNvBlock
(
  NvM_ASR40_BlockIdType BlockId
);

/** \brief Service to invalidate a NV block
 **
 ** This service initiates invalidating the data of this block in NV memory
 ** permanently. For this purpose the underlying function Ea_InvalidateBlock()
 ** resp. Fee_InvalidateBlock() is used. A subsequent
 ** NvM_ReadBlock() request for this block results in setting the error
 ** status to NVM_REQ_NV_INVALIDATED.The RAM Block is not affected
 ** by this service. If the RAM Block is valid it is not invalidated.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_InvalidateNvBlock
(
  NvM_ASR40_BlockIdType BlockId
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
/** \brief Service for setting the RAM block status of an NVRAM block
 **
 ** NvM_SetRamBlockStatus() prepares the NVM Block BlockId for the next
 ** execution of NvM_WriteAll().  If the parameter BlockChanged is Description
 ** set to TRUE and if a Permanent RAM Data Block is defined for block BlockId
 ** then the Permanent RAM Data Block is marked to be valid and changed and
 ** therefore the data will be written by the next call of NvM_WriteAll().  If
 ** CRC calculation is enabled for this block then the CRC will be
 ** recalculated in background.  The RAM Block is synchronously marked as
 ** valid and changed previously to the CRC calculation.  If BlockChanged is
 ** set to FALSE and if a Permanenent Data RAM Block is defined for block
 ** BlockId then the RAM Block is marked as invalid and unchanged and
 ** therefore the data will not be written by the next call of NvM_WriteAll().
 ** If the block BlockId has no Permanent RAM Data Block assigned it is not
 ** marked as valid and changed and therefore no data will be written into the
 ** corresponding NVMBlock by the next call of NvM_WriteAll().
 ** NvM_SetRamBlockStatus() has no side effects to function NvM_WriteBlock().
 ** NvM_WriteBlock() always writes the data independently of the changed and
 ** valid marks.
 **
 ** Note: This function can be synchronous or asynchronous, depending
 ** on CRC disabled or enabled for the block. The CRC calculation is
 ** only triggered when a Permanent RAM CRC Block is assigned to
 ** the requested NVRAM Block. This means the configuration parameter
 ** NvMBlockUseCrc must be enabled for the requested NVRAM Block.
 ** The CRC calculation in background is performed by the NvM_MainFunction()
 ** which must be called cyclically. If CRC calculation finished the
 ** ErrorStatus is set and the Single Block Job End Notification callback
 ** function for the requested NVRAM Block is called if configured.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK : CRC calculation was triggered and finished successfully.
 **
 ** Note: The ErrorStatus remains unmodified if CRC calculation was
 ** not triggered.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The value TRUE is passed to parameter BlockChanged.
 ** - INVALID / UNCHANGED: The value FALSE is passed to parameter BlockChanged.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] BlockChanged TRUE: block will be written by NvM_WriteAll(),
 **            FALSE: block will not be written by NvM_WriteAll().
 ** \return    Std_ReturnType
 ** \retval    E_OK The status of the RAM-Block was changed as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous/Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetRamBlockStatus
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         BlockChanged
);

#endif  /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */


#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service for setting/resetting the write protection for a NV block
 **
 ** This function is used to set the protection flag of the given block. If
 ** ProtectionEnabled is set to TRUE then all subsequent
 ** NvM_WriteBlock() requests for this block will be rejected and
 ** NvM_WriteBlock() returns E_NOT_OK. If ProtectionEnabled is set
 ** to FALSE then all subsequent NvM_WriteBlock() requests for this
 ** block will be accepted and NvM_WriteBlock() returns E_OK.
 ** NvM_WriteAll() skips all blocks who are write protected and sets the
 ** error status for these blocks to NVM_REQ_BLOCK_SKIPPED. If the
 ** configuration parameter NvMWriteBlockOnce is enabled for this block
 ** and DET error detection is enabled then write protection can be
 ** neiter enabled nor disabled for this block and the DET error
 ** NVM_E_BLOCK_CONFIG is reported. If NvMWriteBlockOnce is
 ** enabled then the write protection flag is completely managed internally
 ** by the NVRAM Manager.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] ProtectionEnabled TRUE: Write protection shall be enabled,
 **            FALSE: Write protection shall be disabled
 ** \return    Std_ReturnType
 ** \retval    E_OK The block was enabled/disabled as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR40_SetBlockProtection
(
  NvM_ASR40_BlockIdType BlockId,
  boolean         ProtectionEnabled
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service to copy the data of the NV block to its corresponding RAM
 ** block
 **
 ** Service for copying the data of the NVM Block to its corresponding
 ** RAM Block. This call takes over the given parameters, queues the
 ** read request in the read request list and returns.
 **
 ** ErrorStatus after service finished
 ** - NVM_REQ_OK : The NVM Block was copied successfully or
 **   the ROM Block was copied successfully if a Dataset Block was
 **   requested and the DataIndex selects a ROM Block.
 ** - NVM_REQ_INVALIDATED : The job result of an underlying
 **   abstraction module is MEMIF_BLOCK_INVALID.
 ** - NVM_REQ_INTEGRITY_FAILED : If a CRC mismatch occurs
 **   or if the job result of the underlying memory abstraction module
 **   is MEMIF_BLOCK_INCONSISTENT.
 ** - NVM_REQ_CANCELLED: An underlying memory abstraction
 **   module reports MEMIF_JOB_CANCELED.
 ** - NVM_REQ_NOT_OK: The job result of the underlying memory
 **   abstraction module is MEMIF_JOB_FAILED or the ROM Block
 **   was not copied successfully if a Dataset Block was requested
 **   and the DataIndex selects a ROM Block.
 **
 ** If an error is detected during processing NvM_ReadBlock() and the
 ** ROM default data must be copied by NvM_RestoreBlockDefaults()
 ** the ErrorStatus set by NvM_RestoreBlockDefaults() is ignored and
 ** the above ErrorStatus applies nevertheless.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / UNCHANGED: A NVM Block was copied successfully to the RAM Block.
 ** - VALID / CHANGED: A ROM Block was copied successfully to the RAM Block.
 ** - INVALID / UNCHANGED: An error was detected during copying the NVM Block
 **   to the RAM Block and no ROM Block is configured.
 ** - INVALID / CHANGED: Can not occur.
 **
 ** A ROM Block is copied if a Dataset Block is requested and the
 ** DataIndex selects a ROM Block, or an error is detected during
 ** copying the NVM Block and the default ROM Block data is copied.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK read list overflow, request has not been accepted
 **
 ** \ProductionError{NVM_E_INTEGRITY_FAILED, thrown\, if a CRC mismatch occurs
 ** for a block configured with CRC or MEMIF_BLOCK_INCONSISTENT is reported by
 ** the MemIf module during an attempt to read a NV block.}
 **
 ** \ProductionError{NVM_E_LOSS_OF_REDUNDANCY, thrown\, if a redundant NV block is
 ** detected as damaged (the second copy is different than the first one).}
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRONG_BLOCK_ID, thrown\, if the static block ID stored
 ** in the NV block header is different than the requested block ID.}
 **
 ** \ServiceID{6}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_ReadBlock
(
  NvM_ASR40_BlockIdType                        BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA)  NvM_DstPtr
);


/** \brief Service to copy the data of the RAM block to its corresponding NV
 ** block
 **
 ** Service for copying the data of the RAM Block to its corresponding
 ** NVM Block. First the write protection attribute of the NVRAM Block
 ** shall be tested in the administrative part of the corresponding RAM
 ** Block. In case of disabled write protection, the request shall be
 ** queued in the appropriate write list. The acceptance result of the request
 ** is returned synchronously. In case the requested NVRAM
 ** Block has immediate priority zero the request is inserted into the
 ** Immediate Queue. A currently processed standard priority job is
 ** terminated and resumed after all immediate priority jobs are finished.
 ** Multiple concurrent requests are enqueued.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_SrcPtr Pointer to the RAM data block which shall be
 **            copied to NVRAM.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_VERIFY_FAILED, thrown\, if the content of the RAM
 ** block is not the same as the read back data.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{7}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_WriteBlock
(
  NvM_ASR40_BlockIdType                         BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
);


/** \brief Service to restore the default data to its corresponding RAM block
 **
 ** The services copies the ROM Block default data to its corresponding RAM
 ** Block. The function queues the request and returns. If a Dataset Block is
 ** requested the user application is responsible to set the DataIndex to a
 ** ROM Data Block previously to calling NvM_RestoreBlockDefaults(). The
 ** DataIndex must be set by NvM_SetDataIndex().  If the DataIndex selects a
 ** NVM Block E_NOT_OK is returned and no default data is copied.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK: The default data was copied successfully from
 **   the ROM Block to the RAM Block.
 ** - NVM_REQ_NOT_OK: The default data could not be copied
 **   successfully from the ROM Block to the RAM Block.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The ROM Block data is copied successfully
 **   to the RAM Block.
 ** - INVALID / UNCHANGED: The ROM Block data is not copied
 **   successfully to the RAM Block.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DestPtr Pointer to the RAM Data Block
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK control list overflow, request has not been accepted.
 **            If a Dataset Block is requested which has at least one ROM
 **            Block assigned but the DataIndex selects a NVM Block
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request
 ** cannot be processed because the NVM queue is full.}
 **
 ** \ServiceID{8}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR40_RestoreBlockDefaults
(
  NvM_ASR40_BlockIdType                       BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
);


/** \brief Service to copy the data of the NV block to its corresponding permanent RAM
 ** block
 **
 ** Service to copy the data of the NV block to its corresponding permanent
 ** RAM Block. This call takes over the given parameters, queues the
 ** read request in the read request list and returns.
 **
 ** ErrorStatus after service finished
 ** - NVM_REQ_OK : The NVM Block was copied successfully or
 **   the ROM Block was copied successfully if a Dataset Block was
 **   requested and the DataIndex selects a ROM Block.
 ** - NVM_REQ_INVALIDATED : The job result of an underlying
 **   abstraction module is MEMIF_BLOCK_INVALID.
 ** - NVM_REQ_INTEGRITY_FAILED : If a CRC mismatch occurs
 **   or if the job result of the underlying memory abstraction module
 **   is MEMIF_BLOCK_INCONSISTENT.
 ** - NVM_REQ_CANCELLED: An underlying memory abstraction
 **   module reports MEMIF_JOB_CANCELED.
 ** - NVM_REQ_NOT_OK: The job result of the underlying memory
 **   abstraction module is MEMIF_JOB_FAILED or the ROM Block
 **   was not copied successfully if a Dataset Block was requested
 **   and the DataIndex selects a ROM Block.
 **
 ** If an error is detected during processing NvM_ReadPRAMBlock() and the
 ** ROM default data must be copied by NvM_RestorePRAMBlockDefaults()
 ** the ErrorStatus set by NvM_RestorePRAMBlockDefaults() is ignored and
 ** the above ErrorStatus applies nevertheless.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / UNCHANGED: A NVM Block was copied successfully to the RAM Block.
 ** - VALID / CHANGED: A ROM Block was copied successfully to the RAM Block.
 ** - INVALID / UNCHANGED: An error was detected during copying the NVM Block
 **   to the RAM Block and no ROM Block is configured.
 ** - INVALID / CHANGED: Can not occur.
 **
 ** A ROM Block is copied if a Dataset Block is requested and the
 ** DataIndex selects a ROM Block, or an error is detected during
 ** copying the NVM Block and the default ROM Block data is copied.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK read list overflow, request has not been accepted
 **
 ** \ProductionError{NVM_E_INTEGRITY_FAILED, thrown\, if a CRC mismatch occurs
 ** for a block configured with CRC or MEMIF_BLOCK_INCONSISTENT is reported by
 ** the MemIf module during an attempt to read a NV block.}
 **
 ** \ProductionError{NVM_E_LOSS_OF_REDUNDANCY, thrown\, if a redundant NV block is
 ** detected as damaged (the second copy is different than the first one).}
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRONG_BLOCK_ID, thrown\, if the static block ID stored
 ** in the NV block header is different than the requested block ID.}
 **
 ** \ServiceID{22}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR42_ReadPRAMBlock
(
  NvM_ASR42_BlockIdType   BlockId
);

/** \brief Service to copy the data of the NV block to its corresponding permanent RAM
 ** block
 **
 ** Service for copying the data of the RAM Block to its corresponding permanent
 ** RAM Block. First the write protection attribute of the NVRAM Block
 ** shall be tested in the administrative part of the corresponding RAM
 ** Block. In case of disabled write protection, the request shall be
 ** queued in the appropriate write list. The acceptance result of the request
 ** is returned synchronously. In case the requested NVRAM
 ** Block has immediate priority zero the request is inserted into the
 ** Immediate Queue. A currently processed standard priority job is
 ** terminated and resumed after all immediate priority jobs are finished.
 ** Multiple concurrent requests are enqueued.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_VERIFY_FAILED, thrown\, if the content of the RAM
 ** block is not the same as the read back data.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{23}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR42_WritePRAMBlock
(
  NvM_ASR42_BlockIdType   BlockId
);

/** \brief Service to restore the default data to its corresponding permanent RAM block
 **
 ** The services copies the ROM Block default data to its corresponding permanent RAM
 ** Block. The function queues the request and returns. If a Dataset Block is
 ** requested the user application is responsible to set the DataIndex to a
 ** ROM Data Block previously to calling NvM_RestoreBlockDefaults(). The
 ** DataIndex must be set by NvM_SetDataIndex().  If the DataIndex selects a
 ** NVM Block E_NOT_OK is returned and no default data is copied.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK: The default data was copied successfully from
 **   the ROM Block to the RAM Block.
 ** - NVM_REQ_NOT_OK: The default data could not be copied
 **   successfully from the ROM Block to the RAM Block.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The ROM Block data is copied successfully
 **   to the RAM Block.
 ** - INVALID / UNCHANGED: The ROM Block data is not copied
 **   successfully to the RAM Block.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK control list overflow, request has not been accepted.
 **            If a Dataset Block is requested which has at least one ROM
 **            Block assigned but the DataIndex selects a NVM Block
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request
 ** cannot be processed because the NVM queue is full.}
 **
 ** \ServiceID{24}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR42_RestorePRAMBlockDefaults
(
  NvM_ASR42_BlockIdType   BlockId
);


#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if ((NVM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
	(NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR32))

/** \brief Service to read the block dependent error/status information
 **
 ** The request reads the block dependent error/status information in
 ** the administrative part of a RAM block. The status was set by a
 ** former or current synchronous request.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId. Range: 0..65535.
 ** \param[out] RequestResultPtr Pointer to where to store the request result.
 **             The buffer contains the error status of block 'BlockId'.
 **             Range 0..255.
 ** \return     Std_ReturnType
 ** \retval     E_OK The block dependent error/status information was read
                successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{4}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ASR32_GetErrorStatus
(
  NvM_ASR32_BlockIdType                                       BlockId,
  P2VAR( NvM_ASR32_RequestResultType,AUTOMATIC,NVM_APPL_DATA ) RequestResultPtr
);

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service for setting the DataIndex of a dataset NVRAM block
 **
 ** The request sets the specified index to associate a dataset NV Block
 ** (with/without ROM Blocks) with its corresponding RAM Block. The
 ** usage in conjunction with all other block management types is possible,
 ** but without any effect.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId. Range: 0..65535.
 ** \param[in] DataIndex Index position of a NVM Block or ROM Data Block
 **            within a Dataset NVRAM Block. Range: 0..255
 ** \return    Std_ReturnType
 ** \retval    E_OK The index position was set successfully.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ASR32_SetDataIndex
(
  NvM_ASR32_BlockIdType BlockId, uint8 DataIndex
);

/** \brief Service for getting the currently set DataIndex of a dataset NVRAM
 ** block
 **
 ** The service gets the current index (association) of a Dataset Block
 ** (with/without ROM blocks) with its corresponding RAM block. The
 ** usage in conjunction with all other block management types is possible
 ** but without any effect.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId.  Range: 0..65535
 ** \param[out] DataIndex Pointer to where to store the current dataset index
 **             Range 0 .. size of NVM_VAR.
 ** \return     Std_ReturnType
 ** \retval     E_OK The index position has been retrieved successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ASR32_GetDataIndex
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2VAR( uint8,AUTOMATIC,NVM_APPL_DATA ) DataIndex
);

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service to erase a NV block
 **
 ** This service initiates erasing the data of this block in NV memory. If
 ** this block has immediate priority the underlying services Ea_EraseIm-
 ** mediateBlock() resp. Fee_EraseImmediateBlock() are used for
 ** erasing. If this block has not immediate priority then this service
 ** shall return E_NOT_OK. In case this block has immediate priority a
 ** subsequent NvM_ReadBlock() request for this block results in setting
 ** the error status to NVM_REQ_NV_INVALIDATED. In case of an immediate
 ** priority block a NvM_WriteBlock() request may be faster than for a
 ** standard priority block because the block is already pre erased. If
 ** the used NVRAM hardware does not support separate erase and write
 ** commands and automatically erases upon a write command then there is
 ** no significant time difference in writing the data of an immediate
 ** priority block and a standard priority block. Please see the User's
 ** Guide of the EEPROM driver to get more details.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{9}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_EraseNvBlock
(
  NvM_ASR32_BlockIdType BlockId
);

/** \brief Service to invalidate a NV block
 **
 ** This service initiates invalidating the data of this block in NV memory
 ** permanently. For this purpose the underlying function Ea_InvalidateBlock()
 ** resp. Fee_InvalidateBlock() is used. A subsequent
 ** NvM_ReadBlock() request for this block results in setting the error
 ** status to NVM_REQ_NV_INVALIDATED.The RAM Block is not affected
 ** by this service. If the RAM Block is valid it is not invalidated.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_InvalidateNvBlock
(
  NvM_ASR32_BlockIdType BlockId
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
/** \brief Service for setting the RAM block status of an NVRAM block
 **
 ** NvM_SetRamBlockStatus() prepares the NVM Block BlockId for the next
 ** execution of NvM_WriteAll().  If the parameter BlockChanged is Description
 ** set to TRUE and if a Permanent RAM Data Block is defined for block BlockId
 ** then the Permanent RAM Data Block is marked to be valid and changed and
 ** therefore the data will be written by the next call of NvM_WriteAll().  If
 ** CRC calculation is enabled for this block then the CRC will be
 ** recalculated in background.  The RAM Block is synchronously marked as
 ** valid and changed previously to the CRC calculation.  If BlockChanged is
 ** set to FALSE and if a Permanenent Data RAM Block is defined for block
 ** BlockId then the RAM Block is marked as invalid and unchanged and
 ** therefore the data will not be written by the next call of NvM_WriteAll().
 ** If the block BlockId has no Permanent RAM Data Block assigned it is not
 ** marked as valid and changed and therefore no data will be written into the
 ** corresponding NVMBlock by the next call of NvM_WriteAll().
 ** NvM_SetRamBlockStatus() has no side effects to function NvM_WriteBlock().
 ** NvM_WriteBlock() always writes the data independently of the changed and
 ** valid marks.
 **
 ** Note: This function can be synchronous or asynchronous, depending
 ** on CRC disabled or enabled for the block. The CRC calculation is
 ** only triggered when a Permanent RAM CRC Block is assigned to
 ** the requested NVRAM Block. This means the configuration parameter
 ** NvMBlockUseCrc must be enabled for the requested NVRAM Block.
 ** The CRC calculation in background is performed by the NvM_MainFunction()
 ** which must be called cyclically. If CRC calculation finished the
 ** ErrorStatus is set and the Single Block Job End Notification callback
 ** function for the requested NVRAM Block is called if configured.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK : CRC calculation was triggered and finished successfully.
 **
 ** Note: The ErrorStatus remains unmodified if CRC calculation was
 ** not triggered.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The value TRUE is passed to parameter BlockChanged.
 ** - INVALID / UNCHANGED: The value FALSE is passed to parameter BlockChanged.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] BlockChanged TRUE: block will be written by NvM_WriteAll(),
 **            FALSE: block will not be written by NvM_WriteAll().
 ** \return    Std_ReturnType
 ** \retval    E_OK The status of the RAM-Block was changed as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous/Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ASR32_SetRamBlockStatus
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         BlockChanged
);

#endif  /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service for setting/resetting the write protection for a NV block
 **
 ** This function is used to set the protection flag of the given block. If
 ** ProtectionEnabled is set to TRUE then all subsequent
 ** NvM_WriteBlock() requests for this block will be rejected and
 ** NvM_WriteBlock() returns E_NOT_OK. If ProtectionEnabled is set
 ** to FALSE then all subsequent NvM_WriteBlock() requests for this
 ** block will be accepted and NvM_WriteBlock() returns E_OK.
 ** NvM_WriteAll() skips all blocks who are write protected and sets the
 ** error status for these blocks to NVM_REQ_BLOCK_SKIPPED. If the
 ** configuration parameter NvMWriteBlockOnce is enabled for this block
 ** and DET error detection is enabled then write protection can be
 ** neiter enabled nor disabled for this block and the DET error
 ** NVM_E_BLOCK_CONFIG is reported. If NvMWriteBlockOnce is
 ** enabled then the write protection flag is completely managed internally
 ** by the NVRAM Manager.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] ProtectionEnabled TRUE: Write protection shall be enabled,
 **            FALSE: Write protection shall be disabled
 ** \return    Std_ReturnType
 ** \retval    E_OK The block was enabled/disabled as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ASR32_SetBlockProtection
(
  NvM_ASR32_BlockIdType BlockId,
  boolean         ProtectionEnabled
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service to copy the data of the NV block to its corresponding RAM
 ** block
 **
 ** Service for copying the data of the NVM Block to its corresponding
 ** RAM Block. This call takes over the given parameters, queues the
 ** read request in the read request list and returns.
 **
 ** ErrorStatus after service finished
 ** - NVM_REQ_OK : The NVM Block was copied successfully or
 **   the ROM Block was copied successfully if a Dataset Block was
 **   requested and the DataIndex selects a ROM Block.
 ** - NVM_REQ_INVALIDATED : The job result of an underlying
 **   abstraction module is MEMIF_BLOCK_INVALID.
 ** - NVM_REQ_INTEGRITY_FAILED : If a CRC mismatch occurs
 **   or if the job result of the underlying memory abstraction module
 **   is MEMIF_BLOCK_INCONSISTENT.
 ** - NVM_REQ_CANCELLED: An underlying memory abstraction
 **   module reports MEMIF_JOB_CANCELED.
 ** - NVM_REQ_NOT_OK: The job result of the underlying memory
 **   abstraction module is MEMIF_JOB_FAILED or the ROM Block
 **   was not copied successfully if a Dataset Block was requested
 **   and the DataIndex selects a ROM Block.
 **
 ** If an error is detected during processing NvM_ReadBlock() and the
 ** ROM default data must be copied by NvM_RestoreBlockDefaults()
 ** the ErrorStatus set by NvM_RestoreBlockDefaults() is ignored and
 ** the above ErrorStatus applies nevertheless.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / UNCHANGED: A NVM Block was copied successfully to the RAM Block.
 ** - VALID / CHANGED: A ROM Block was copied successfully to the RAM Block.
 ** - INVALID / UNCHANGED: An error was detected during copying the NVM Block
 **   to the RAM Block and no ROM Block is configured.
 ** - INVALID / CHANGED: Can not occur.
 **
 ** A ROM Block is copied if a Dataset Block is requested and the
 ** DataIndex selects a ROM Block, or an error is detected during
 ** copying the NVM Block and the default ROM Block data is copied.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK read list overflow, request has not been accepted
 **
 ** \ProductionError{NVM_E_INTEGRITY_FAILED, thrown\, if a CRC mismatch occurs
 ** for a block configured with CRC or MEMIF_BLOCK_INCONSISTENT is reported by the
 ** MemIf module during an attempt to read a NV block.}
 **
 ** \ProductionError{NVM_E_LOSS_OF_REDUNDANCY, thrown\, if a redundant NV block is
 ** detected as damaged (the second copy is different than the first one).}
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRONG_BLOCK_ID, thrown\, if the static block ID stored
 ** in the NV block header is different than the requested block ID.}
 **
 ** \ServiceID{6}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_ReadBlock
(
  NvM_ASR32_BlockIdType                        BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA)  NvM_DstPtr
);

/** \brief Service to copy the data of the RAM block to its corresponding NV
 ** block
 **
 ** Service for copying the data of the RAM Block to its corresponding
 ** NVM Block. First the write protection attribute of the NVRAM Block
 ** shall be tested in the administrative part of the corresponding RAM
 ** Block. In case of disabled write protection, the request shall be
 ** queued in the appropriate write list. The acceptance result of the request
 ** is returned synchronously. In case the requested NVRAM
 ** Block has immediate priority zero the request is inserted into the
 ** Immediate Queue. A currently processed standard priority job is
 ** terminated and resumed after all immediate priority jobs are finished.
 ** Multiple concurrent requests are enqueued.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_SrcPtr Pointer to the RAM data block which shall be
 **            copied to NVRAM.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_VERIFY_FAILED, thrown\, if the content of the RAM
 ** block is not the same as the read back data.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{7}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_WriteBlock
(
  NvM_ASR32_BlockIdType                         BlockId,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
);

/** \brief Service to restore the default data to its corresponding RAM block
 **
 ** The services copies the ROM Block default data to its corresponding RAM
 ** Block. The function queues the request and returns. If a Dataset Block is
 ** requested the user application is responsible to set the DataIndex to a
 ** ROM Data Block previously to calling NvM_RestoreBlockDefaults(). The
 ** DataIndex must be set by NvM_SetDataIndex().  If the DataIndex selects a
 ** NVM Block E_NOT_OK is returned and no default data is copied.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK: The default data was copied successfully from
 **   the ROM Block to the RAM Block.
 ** - NVM_REQ_NOT_OK: The default data could not be copied
 **   successfully from the ROM Block to the RAM Block.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The ROM Block data is copied successfully
 **   to the RAM Block.
 ** - INVALID / UNCHANGED: The ROM Block data is not copied
 **   successfully to the RAM Block.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DestPtr Pointer to the RAM Data Block
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK control list overflow, request has not been accepted.
 **            If a Dataset Block is requested which has at least one ROM
 **            Block assigned but the DataIndex selects a NVM Block
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ServiceID{8}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC( Std_ReturnType,NVM_CODE ) NvM_ASR32_RestoreBlockDefaults
(
 NvM_ASR32_BlockIdType                       BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
);

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#endif

#if ((NVM_ENABLE_ASR42_SERVICE_API == STD_ON) || \
  (NVM_DEFAULT_ASR_SERVICE_API == NVM_SERVICE_API_ASR42))

/** \brief Service to read the block dependent error/status information
 **
 ** The request reads the block dependent error/status information in
 ** the administrative part of a RAM block. The status was set by a
 ** former or current synchronous request.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId. Range: 0..65535.
 ** \param[out] RequestResultPtr Pointer to where to store the request result.
 **             The buffer contains the error status of block 'BlockId'.
 **             Range 0..255.
 ** \return     Std_ReturnType
 ** \retval     E_OK The block dependent error/status information was read
                successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{4}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_GetErrorStatus
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(NvM_ASR42_RequestResultType, AUTOMATIC, NVM_APPL_DATA) RequestResultPtr
);

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service for setting the DataIndex of a dataset NVRAM block
 **
 ** The request sets the specified index to associate a dataset NV Block
 ** (with/without ROM Blocks) with its corresponding RAM Block. The
 ** usage in conjunction with all other block management types is possible,
 ** but without any effect.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId. Range: 0..65535.
 ** \param[in] DataIndex Index position of a NVM Block or ROM Data Block
 **            within a Dataset NVRAM Block. Range: 0..255
 ** \return    Std_ReturnType
 ** \retval    E_OK The index position was set successfully.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetDataIndex
(
  NvM_ASR42_BlockIdType BlockId,
  uint8 DataIndex
);

/** \brief Service for getting the currently set DataIndex of a dataset NVRAM
 ** block
 **
 ** The service gets the current index (association) of a Dataset Block
 ** (with/without ROM blocks) with its corresponding RAM block. The
 ** usage in conjunction with all other block management types is possible
 ** but without any effect.
 **
 ** \param[in]  BlockId The block identifier. Selects the block whose
 **             configuration parameter NvMNvBlockIdentifier is equal to
 **             BlockId.  Range: 0..65535
 ** \param[out] DataIndex Pointer to where to store the current dataset index
 **             Range 0 .. size of NVM_VAR.
 ** \return     Std_ReturnType
 ** \retval     E_OK The index position has been retrieved successfully.
 ** \retval     E_NOT_OK An error occurred.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_GetDataIndex
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataIndex
);

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service to erase a NV block
 **
 ** This service initiates erasing the data of this block in NV memory. If
 ** this block has immediate priority the underlying services Ea_EraseIm-
 ** mediateBlock() resp. Fee_EraseImmediateBlock() are used for
 ** erasing. If this block has not immediate priority then this block is only
 ** invalidated via the function NvM_InvalidateBlock(). In both cases a
 ** subsequent NvM_ReadBlock() request for this block results in setting
 ** the error status to NVM_REQ_NV_INVALIDATED.Erasing a standard
 ** priority block is normally faster than erasing an immediate priority
 ** block because only few management data needs to be manipulated
 ** in the NV memory by the underlying Ea/Fee module. However, a
 ** subsequent NvM_WriteBlock() request may take more time because
 ** the NVM data also must be erased by NvM_WriteBlock() before it
 ** can be written. In case of an immediate priority block a
 ** NvM_WriteBlock() request may be faster than for a standard priority
 ** block because the block is already pre erased. If the used NVRAM
 ** hardware does not support separate erase and write commands and
 ** automatically erases upon a write command then there is no significant
 ** time difference in writing the data of an immediate priority block
 ** and a standard priority block. Please see the User's Guide of the
 ** EEPROM driver to get more details.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{9}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_EraseNvBlock
(
  NvM_ASR42_BlockIdType BlockId
);

/** \brief Service to invalidate a NV block
 **
 ** This service initiates invalidating the data of this block in NV memory
 ** permanently. For this purpose the underlying function Ea_InvalidateBlock()
 ** resp. Fee_InvalidateBlock() is used. A subsequent
 ** NvM_ReadBlock() request for this block results in setting the error
 ** status to NVM_REQ_NV_INVALIDATED.The RAM Block is not affected
 ** by this service. If the RAM Block is valid it is not invalidated.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_InvalidateNvBlock
(
  NvM_ASR42_BlockIdType BlockId
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON)
/** \brief Service for setting the RAM block status of an NVRAM block
 **
 ** NvM_SetRamBlockStatus() prepares the NVM Block BlockId for the next
 ** execution of NvM_WriteAll().  If the parameter BlockChanged is Description
 ** set to TRUE and if a Permanent RAM Data Block is defined for block BlockId
 ** then the Permanent RAM Data Block is marked to be valid and changed and
 ** therefore the data will be written by the next call of NvM_WriteAll().  If
 ** CRC calculation is enabled for this block then the CRC will be
 ** recalculated in background.  The RAM Block is synchronously marked as
 ** valid and changed previously to the CRC calculation.  If BlockChanged is
 ** set to FALSE and if a Permanenent Data RAM Block is defined for block
 ** BlockId then the RAM Block is marked as invalid and unchanged and
 ** therefore the data will not be written by the next call of NvM_WriteAll().
 ** If the block BlockId has no Permanent RAM Data Block assigned it is not
 ** marked as valid and changed and therefore no data will be written into the
 ** corresponding NVMBlock by the next call of NvM_WriteAll().
 ** NvM_SetRamBlockStatus() has no side effects to function NvM_WriteBlock().
 ** NvM_WriteBlock() always writes the data independently of the changed and
 ** valid marks.
 **
 ** Note: This function can be synchronous or asynchronous, depending
 ** on CRC disabled or enabled for the block. The CRC calculation is
 ** only triggered when a Permanent RAM CRC Block is assigned to
 ** the requested NVRAM Block. This means the configuration parameter
 ** NvMBlockUseCrc must be enabled for the requested NVRAM Block.
 ** The CRC calculation in background is performed by the NvM_MainFunction()
 ** which must be called cyclically. If CRC calculation finished the
 ** ErrorStatus is set and the Single Block Job End Notification callback
 ** function for the requested NVRAM Block is called if configured.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK : CRC calculation was triggered and finished successfully.
 **
 ** Note: The ErrorStatus remains unmodified if CRC calculation was
 ** not triggered.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The value TRUE is passed to parameter BlockChanged.
 ** - INVALID / UNCHANGED: The value FALSE is passed to parameter BlockChanged.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] BlockChanged TRUE: block will be written by NvM_WriteAll(),
 **            FALSE: block will not be written by NvM_WriteAll().
 ** \return    Std_ReturnType
 ** \retval    E_OK The status of the RAM-Block was changed as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous/Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetRamBlockStatus
(
  NvM_ASR42_BlockIdType BlockId,
  boolean BlockChanged
);

#endif /* (NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) */

#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

/** \brief Service for setting/resetting the write protection for a NV block
 **
 ** This function is used to set the protection flag of the given block. If
 ** ProtectionEnabled is set to TRUE then all subsequent
 ** NvM_WriteBlock() requests for this block will be rejected and
 ** NvM_WriteBlock() returns E_NOT_OK. If ProtectionEnabled is set
 ** to FALSE then all subsequent NvM_WriteBlock() requests for this
 ** block will be accepted and NvM_WriteBlock() returns E_OK.
 ** NvM_WriteAll() skips all blocks who are write protected and sets the
 ** error status for these blocks to NVM_REQ_BLOCK_SKIPPED. If the
 ** configuration parameter NvMWriteBlockOnce is enabled for this block
 ** and DET error detection is enabled then write protection can be
 ** neiter enabled nor disabled for this block and the DET error
 ** NVM_E_BLOCK_CONFIG is reported. If NvMWriteBlockOnce is
 ** enabled then the write protection flag is completely managed internally
 ** by the NVRAM Manager.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] ProtectionEnabled TRUE: Write protection shall be enabled,
 **            FALSE: Write protection shall be disabled
 ** \return    Std_ReturnType
 ** \retval    E_OK The block was enabled/disabled as requested.
 ** \retval    E_NOT_OK An error occurred.
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_SetBlockProtection
(
  NvM_ASR42_BlockIdType BlockId,
  boolean ProtectionEnabled
);

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1))

/** \brief Service to copy the data of the NV block to its corresponding RAM
 ** block
 **
 ** Service for copying the data of the NVM Block to its corresponding
 ** RAM Block. This call takes over the given parameters, queues the
 ** read request in the read request list and returns.
 **
 ** ErrorStatus after service finished
 ** - NVM_REQ_OK : The NVM Block was copied successfully or
 **   the ROM Block was copied successfully if a Dataset Block was
 **   requested and the DataIndex selects a ROM Block.
 ** - NVM_REQ_INVALIDATED : The job result of an underlying
 **   abstraction module is MEMIF_BLOCK_INVALID.
 ** - NVM_REQ_INTEGRITY_FAILED : If a CRC mismatch occurs
 **   or if the job result of the underlying memory abstraction module
 **   is MEMIF_BLOCK_INCONSISTENT.
 ** - NVM_REQ_CANCELLED: An underlying memory abstraction
 **   module reports MEMIF_JOB_CANCELED.
 ** - NVM_REQ_NOT_OK: The job result of the underlying memory
 **   abstraction module is MEMIF_JOB_FAILED or the ROM Block
 **   was not copied successfully if a Dataset Block was requested
 **   and the DataIndex selects a ROM Block.
 **
 ** If an error is detected during processing NvM_ReadBlock() and the
 ** ROM default data must be copied by NvM_RestoreBlockDefaults()
 ** the ErrorStatus set by NvM_RestoreBlockDefaults() is ignored and
 ** the above ErrorStatus applies nevertheless.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / UNCHANGED: A NVM Block was copied successfully to the RAM Block.
 ** - VALID / CHANGED: A ROM Block was copied successfully to the RAM Block.
 ** - INVALID / UNCHANGED: An error was detected during copying the NVM Block
 **   to the RAM Block and no ROM Block is configured.
 ** - INVALID / CHANGED: Can not occur.
 **
 ** A ROM Block is copied if a Dataset Block is requested and the
 ** DataIndex selects a ROM Block, or an error is detected during
 ** copying the NVM Block and the default ROM Block data is copied.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK read list overflow, request has not been accepted
 **
 ** \ProductionError{NVM_E_INTEGRITY_FAILED, thrown\, if a CRC mismatch occurs
 ** for a block configured with CRC or MEMIF_BLOCK_INCONSISTENT is reported by
 ** the MemIf module during an attempt to read a NV block.}
 **
 ** \ProductionError{NVM_E_LOSS_OF_REDUNDANCY, thrown\, if a redundant NV block is
 ** detected as damaged (the second copy is different than the first one).}
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_WRONG_BLOCK_ID, thrown\, if the static block ID stored
 ** in the NV block header is different than the requested block ID.}
 **
 ** \ServiceID{6}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_ReadBlock
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
);

/** \brief Service to copy the data of the RAM block to its corresponding NV
 ** block
 **
 ** Service for copying the data of the RAM Block to its corresponding
 ** NVM Block. First the write protection attribute of the NVRAM Block
 ** shall be tested in the administrative part of the corresponding RAM
 ** Block. In case of disabled write protection, the request shall be
 ** queued in the appropriate write list. The acceptance result of the request
 ** is returned synchronously. In case the requested NVRAM
 ** Block has immediate priority zero the request is inserted into the
 ** Immediate Queue. A currently processed standard priority job is
 ** terminated and resumed after all immediate priority jobs are finished.
 ** Multiple concurrent requests are enqueued.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_SrcPtr Pointer to the RAM data block which shall be
 **            copied to NVRAM.
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK request has not been accepted
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request cannot
 ** be processed because the NVM queue is full.}
 **
 ** \ProductionError{NVM_E_REQ_FAILED, thrown\, if a single block request fails
 ** or lower layer module reports failure. NVM_E_REQ_FAILED shall be reported only
 ** after a maximum number of retries is exceeded for a requests with a retry
 ** counter configured.}
 **
 ** \ProductionError{NVM_E_VERIFY_FAILED, thrown\, if the content of the RAM
 ** block is not the same as the read back data.}
 **
 ** \ProductionError{NVM_E_WRITE_PROTECTED, thrown\, if a write erase or
 ** invalidate operation is requested for a write protected block.}
 **
 ** \ServiceID{7}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_WriteBlock
(
  NvM_ASR42_BlockIdType BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
);

/** \brief Service to restore the default data to its corresponding RAM block
 **
 ** The services copies the ROM Block default data to its corresponding RAM
 ** Block. The function queues the request and returns. If a Dataset Block is
 ** requested the user application is responsible to set the DataIndex to a
 ** ROM Data Block previously to calling NvM_RestoreBlockDefaults(). The
 ** DataIndex must be set by NvM_SetDataIndex().  If the DataIndex selects a
 ** NVM Block E_NOT_OK is returned and no default data is copied.
 **
 ** ErrorStatusafter service finished
 ** - NVM_REQ_OK: The default data was copied successfully from
 **   the ROM Block to the RAM Block.
 ** - NVM_REQ_NOT_OK: The default data could not be copied
 **   successfully from the ROM Block to the RAM Block.
 **
 ** RAMBlockStatus after service finished (applies only for permanent RAM
 ** Blocks)
 ** - VALID / CHANGED : The ROM Block data is copied successfully
 **   to the RAM Block.
 ** - INVALID / UNCHANGED: The ROM Block data is not copied
 **   successfully to the RAM Block.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DestPtr Pointer to the RAM Data Block
 ** \return    Std_ReturnType
 ** \retval    E_OK request has been accepted
 ** \retval    E_NOT_OK control list overflow, request has not been accepted.
 **            If a Dataset Block is requested which has at least one ROM
 **            Block assigned but the DataIndex selects a NVM Block
 **
 ** \ProductionError{NVM_E_QUEUE_OVERFLOW, thrown\, if a new NVM request
 ** cannot be processed because the NVM queue is full.}
 **
 ** \ServiceID{8}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NVM_CODE) NvM_ASR42_RestoreBlockDefaults
(
  NvM_ASR42_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr
);

#endif /* #if (defined(NVM_DOXY_FLAG) || (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)) */

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

#endif /* ifndef NVM_BSW_H */

/*==================[end of file NvM.h]=====================================*/
