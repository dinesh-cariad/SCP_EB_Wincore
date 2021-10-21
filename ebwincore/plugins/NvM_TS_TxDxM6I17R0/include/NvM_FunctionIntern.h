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
#ifndef NVM_FUNCTION_INTERN_H
#define NVM_FUNCTION_INTERN_H
/*==================[inclusions]============================================*/
#include <TSAutosar.h>            /* EB specific standard types             */
                                  /* standard types                         */
#include <NvM_Types.h>            /* NVRAM Manager specific type            */
                                  /* definitions.                           */
#include <NvM_Int_Types.h>        /* NvM specific internal types            */
#include <NvM_UsrHdr.h>           /* indirect inclusion of user supplied
                                     header with callout declaration        */
/*==================[macros]================================================*/

#if (defined NVM_CHECK_DYN_STATUS)
#error NVM_CHECK_DYN_STATUS is already defined
#endif
/** \brief Checks one or more status bits in NvMDynStatus field of NvM_AdminBlockTable.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be checked.
 **
 **/
#define NVM_CHECK_DYN_STATUS(blockIdx, bitmask) \
  (((NvM_AdminBlockTable[(blockIdx)].NvMDynStatus) & ((uint16)(bitmask))) == ((uint16)(bitmask)))

#if (defined NVM_SET_DYN_STATUS)
#error NVM_SET_DYN_STATUS is already defined
#endif
/** \brief Sets one or more status bits in NvMDynStatus field of NvM_AdminBlockTable.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be set.
 **
 **/
#define NVM_SET_DYN_STATUS(blockIdx, bitmask) \
  ((NvM_AdminBlockTable[(blockIdx)].NvMDynStatus) |= ((uint16)(bitmask)))

#if (defined NVM_CLEAR_DYN_STATUS)
#error NVM_CLEAR_DYN_STATUS is already defined
#endif
/** \brief Clears one or more status bits in NvMDynStatus field of NvM_AdminBlockTable.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be cleared.
 **
 **/
#define NVM_CLEAR_DYN_STATUS(blockIdx, bitmask) \
  ((NvM_AdminBlockTable[(blockIdx)].NvMDynStatus) &= ((uint16)((uint16)~(bitmask) & 0xFFFFU)))

#if (defined NVM_CHECK_WORKING_STATUS)
#error NVM_CHECK_WORKING_STATUS is already defined
#endif
/** \brief Checks temporary stored status information
 **        in the global NvM_GlobalWorkingStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be checked.
 **
 **/
#define NVM_CHECK_WORKING_STATUS(bitmask) \
        ((NvM_DynamicStatusType)((NvM_GlobalWorkingStatus) & (bitmask)) == (bitmask))

#if (defined NVM_SET_WORKING_STATUS)
#error NVM_SET_WORKING_STATUS is already defined
#endif
/** \brief Sets temporary status information
 **        in the global NvM_GlobalWorkingStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be set.
 **
 **/
#define NVM_SET_WORKING_STATUS(bitmask) \
        (NvM_GlobalWorkingStatus |= (bitmask))

#if (defined NVM_CLEAR_WORKING_STATUS)
#error NVM_CLEAR_WORKING_STATUS is already defined
#endif
/** \brief Clears temporary set status information
 **        in the global NvM_GlobalWorkingStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be cleared.
 **
 **/
#define NVM_CLEAR_WORKING_STATUS(bitmask) \
        (NvM_GlobalWorkingStatus &= (NvM_DynamicStatusType)(~((uint16)(bitmask))))

#if (defined NVM_CHECK_GLOBAL_GENERIC_STATUS)
#error NVM_CHECK_GLOBAL_GENERIC_STATUS is already defined
#endif
/** \brief Checks temporary stored status
 **        information in the global NvM_GlobalGenericStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be checked.
 **
 **/
#define NVM_CHECK_GLOBAL_GENERIC_STATUS(bitmask) \
        ((uint16)((NvM_GlobalGenericStatus) & (bitmask)) == (bitmask))

#if (defined NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE)
#error NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE is already defined
#endif
/** \brief Checks whether the bits defined by the bitmask
 **        are NOT set in the NvM_GlobalGenericStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be checked.
 **
 **/
#define NVM_CHECK_GLOBAL_GENERIC_STATUS_FALSE(bitmask) \
        ((uint16)((NvM_GlobalGenericStatus) & (bitmask)) == (0U))

#if (defined NVM_SET_GLOBAL_GENERIC_STATUS)
#error NVM_SET_GLOBAL_GENERIC_STATUS is already defined
#endif
/** \brief Sets temporary status information
 **        in the global NvM_GlobalGenericStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be set.
 **
 **/
#define NVM_SET_GLOBAL_GENERIC_STATUS(bitmask) \
        (NvM_GlobalGenericStatus |= (bitmask))

#if (defined NVM_CLEAR_GLOBAL_GENERIC_STATUS)
#error NVM_CLEAR_GLOBAL_GENERIC_STATUS is already defined
#endif
/** \brief Clears status flags in the global NvM_GlobalWorkingStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be cleared.
 **
 **/
#define NVM_CLEAR_GLOBAL_GENERIC_STATUS(bitmask) \
        (NvM_GlobalGenericStatus &= (uint16)((uint16)(~(bitmask)) & 0xFFFFU))

#if (defined NVM_CHECK_EXT_STATUS)
#error NVM_CHECK_EXT_STATUS is already defined
#endif
/** \brief Checks status information
 **        in the global NvMExtendedStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be checked.
 **
 **/
#define NVM_CHECK_EXT_STATUS(bitmask) \
  ((uint8)(NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus & (bitmask)) == (bitmask))

#if (defined NVM_SET_EXT_STATUS)
#error NVM_SET_EXT_STATUS is already defined
#endif
/** \brief Sets status information
 **        in the global NvMExtendedStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be set.
 **
 **/
#define NVM_SET_EXT_STATUS(bitmask) \
        (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus |= (uint8)(bitmask))

#if (defined NVM_CLEAR_EXT_STATUS)
#error NVM_CLEAR_EXT_STATUS is already defined
#endif
/** \brief Clears status information
 **        in the global NvMExtendedStatus field.
 **
 ** \param[in]  bitmask: bit mask of the status bits which shall be cleared.
 **
 **/
#define NVM_CLEAR_EXT_STATUS(bitmask) \
        (NvM_AdminBlockTable[NvM_GlobalBlockNumber].NvMExtendedStatus &= (uint8)(~(bitmask)))


#if (defined NVM_CHECK_DATASET_ROM_INDEX_SELECTED)
#error NVM_CHECK_DATASET_ROM_INDEX_SELECTED is already defined
#endif
/** \brief Logical value for a dataset block having it's index
 **        set to a ROM block
 ** \param[in]  BlockIndex: Index of block being checked
 **
 **/
#define NVM_CHECK_DATASET_ROM_INDEX_SELECTED(BlockIndex)    \
(                                                           \
 (                                                          \
   NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex     \
   >=                                                       \
   NvM_BlockDescriptorTable[BlockIndex].nvBlockNum          \
  )                                                         \
  &&                                                        \
  (                                                         \
    NvM_AdminBlockTable[BlockIndex].NvMDatasetBlockIndex    \
    <                                                       \
    (                                                       \
      NvM_BlockDescriptorTable[BlockIndex].romBlockNum      \
      +                                                     \
      NvM_BlockDescriptorTable[BlockIndex].nvBlockNum       \
    )                                                       \
  )                                                         \
)
/*==================[external function declarations]========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief **********************  StateMachine functions ***************************/

/** \brief Checks if a permanent RAM Block must be used
 **        by the current service and if so the RAM Block
 **        is set invalid.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_SetPermanentRamBlockInvalid( void );

/** \brief Checks if a permanent RAM Block must be used
 **        by the current service and if so the RAM Block
 **        is set valid.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_SetPermanentRamBlockValid( void );

/** \brief Checks if a permanent RAM Block must be used
 **        by the current service and if so the RAM Block
 **        is set to unchanged.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_SetPermanentRamBlockUnchanged( void );

/** \brief Checks if a permanent RAM Block must be used
 **        by the current service and if so the RAM Block
 **        is set to changed.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_SetPermanentRamBlockChanged( void );

/** \brief returns the start address of the user
 **        specified RAM data buffer.
 **
 ** returns either the permanent RAM block or temporary RAM block.
 **
 ** \revtal start address of user specified RAM data buffer.
 **         range: 0 .. size of NVM_APPL_DATA
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE ) NvM_GetUserRamBlockAddress( void );

/** \brief returns the length  of the currently processed block
 **        including the block header and excluding the CRC.
 **
 ** \param[in] Block descriptor used to check if static ID is enabled.
 ** \param[in] Block Lenght
 **
 ** \revtal length of data+CRC of current block. range 0..2^16-1
 **
 ** \remarks if CRC is not configured for this block the CRC length is zero. */
extern FUNC(uint16,NVM_CODE) NvM_GetBlockLength( uint32 BlockDesc, uint16 BlkLength);

/** \brief Returns the start address of the buffer to read into or to be written from.
 **        If Internal Buffer is used, it returns the address of internal buffer
 **        If Mirror is used, it returns the address of the Mirror.
 **        Otherwise, it returns the address of the User RAM block.
 **
 ** \revtal Returns the start address of the buffer (including static block id).
 **
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetBufferAddress(void);

/** \brief Returns the start address of data which was read or is to be written
 **        If Internal Buffer is used, it returns the address in internal buffer
 **        where data is stored (after static block id.)
 **        If Mirror is used, it returns the address in Mirror
 **        where data is stored (after static block id.)
 **        Otherwise, it returns the address of the User RAM block.
 **
 ** \revtal Returns the start address of data.
 **
 **/
extern FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetDataAddress(void);

/** \brief Merges NVRAM block number with current
 **        DatasetIndex of this block to get the block number for the
 **        underlying drivers.
 **
 ** \revtal : Block number calculated for the underlying drivers.
 **
 **/
extern FUNC( uint16,NVM_CODE ) NvM_GetMergedBlockNumber( void );

/** \brief Resets DatasetBlockIndex to zero when the currently processed
 **        NVRAM Block is a Redundant Block.
 **/
extern FUNC( void,NVM_CODE ) NvM_ResetRedundantBlockDataIndex( void );

/** \brief Sets the block result and internal management data.
 **        This function is called after the multi block requests
 **        NvM_ReadAll() and NvM_WriteAll finish the processing of a single block.
 **/
extern FUNC( void,NVM_CODE ) NvM_MultiRequestFinishedSingleBlock
(
  NvM_CurrentFunctionPtrType NextState
);

/** \brief Initializes some data structures of the NvM state machine.
 **/
extern FUNC( void,NVM_CODE ) NvM_PreService_Function( void );

/** \brief Sets the block status data after a block service request
 **        is finished.
 **/
extern FUNC( void,NVM_CODE ) NvM_PostService_Function( void );

#if ( NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK > 0U )

/** \brief Invokes the single block callback function after a single block
 **        request has been finished or a block has been completed
 **        as part of a multi-block request.
 ** \param[in] BlockId: Identifier of the block for which the callback must be called.
 **                     range: 0..65535
 ** \param[in] ServiceId: Service ID of the currently finished or cancelled request.
 ** \param[in] ErrorStatus: Status of the currently finished or cancelled request.
 **/
extern FUNC( void,NVM_CODE ) NvM_InvokeSingleBlockCallback
(
  uint16 BlockIndex,
  uint8 ServiceId,
  uint8 ErrorStatus
);

#endif

/** \brief Terminates a multi block service request after it has finished.
 **        Invokes the multiblock callback function.
 **/
extern FUNC( void,NVM_CODE ) NvM_MultiPostService_Function( void );

/** \brief Returns TRUE if the write protection bit or
 **        the permanent write protection bit is of a NVRAM block is set.
 **
 ** \param[in] BlockId: Identifier of the block which data must be read.
 **                     range: 0..65535
 **
 ** \retval TRUE:  block is write protected
 ** \retval FALSE: block is not write protected
 **/
extern FUNC( boolean,NVM_CODE ) NvM_CheckWriteProtection
(
  uint16 BlockIndex
);

/** \brief Checks whether the specified NVRAM Block is a Dataset Block
 **        and the Dataset Index specifies a ROM Data Block.
 **
 ** \param[in] BlockId: Identifier of the block which data must be read.
 **                     range: 0..65535
 **
 ** \retval TRUE:  The current Dataset Block is a ROM Block.
 ** \retval FALSE: The current Block is no Dataset Block or it is a
 **                    Dataset Block and the Dataset Index specifies an
 **                    NV Block.
 **/
extern FUNC( boolean,NVM_CODE ) NvM_CheckDataSetRomBlock
(
  uint16 BlockIndex
);

/** \brief Returns the CRC length in byte of the current block.
 **
 ** \param[in] BlockDesc: BlockDesc to get CRC type
 ** \revtal CRC length of current block. range 0..2^8-1
 **
 **/
extern FUNC( uint8,NVM_CODE ) NvM_GetBlockCRCLength( uint32 BlockDesc );

#if (NVM_SYNC_MECHANISM == STD_ON)

/** \brief Invokes the callback function to copy data between the RAM block
 **        and NvM module's mirror. If the callback returns E_NOT_OK for
 **        the configured number of times, the current job is stopped
 **        and the next job in the queue is processed.
 **
 ** \param[in] ReadCallback: Read Callback function to be invoked.
 ** \param[in] WriteCallback: Write Callback function to be invoked.
 ** \param[in] DataBufPtr: The buffer pointer which shall be passed to the callback.
 **
 ** \retval E_OK:  The callback was successful
 ** \retval E_NOT_OK: The callback was not successful
 **/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_MirrorCopy
(
  NvM_NvToRamCopyCallbackType ReadCallback,
  NvM_RamToNvCopyCallbackType WriteCallback,
  NvM_PtrToApplDataType DataBufPtr
);

#endif

/** \brief Checks if another job is pending or cancelled for the processing
 **        block ID and sets the Multi Request Keep Job Pending flag and
 **        Block request Canceled Flag accordingly during NvM_ReadAll/NvM_WriteAll.
 **/
extern FUNC(void, NVM_CODE) NvM_CheckBlockStatus(void);

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
/** \brief Checks is any single block request queued while processing
 **        NvM_SetRamBlockStatus request. Ongoing NvM_SetRamBlockStatus job is
 **        interrupted and an standard priority job is started.
 **/
extern FUNC(void, NVM_CODE) NvM_ProcessStandardPriorityJob(void);
#endif /* ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)) */

/** \brief ******************** RestoreBlockDefaults functions **********************/

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

/** \brief NvM_RestoreBlockDefaults_Asc: first state of the asynchronous
 **        state machine of API function NvM_RestoreBlockDefaults. Only used
 **        to set the current service ID.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_Asc( void );

/** \brief NvM_RestorePRAMBlockDefaults_Asc: first state of the asynchronous
 **        state machine of API function NvM_RestorePRAMBlockDefaults. Only used
 **        to set the current service ID.
 **
 **/
extern FUNC( void,NVM_CODE ) NvM_RestorePRAMBlockDefaults_Asc( void );

/** \brief NvM_InsertRestoreBlockJob: function to handle insertion of
 **       restore block defaults or PRAM block defaults job into queue
 **
 **/
extern FUNC(Std_ReturnType, NVM_CODE) NvM_InsertRestoreBlockJob(
    boolean                               PRAM,
    uint16                                BlockId,
    uint16                                BlockIndex,
    P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DestPtr);

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

/** \brief State 0 of the asynchronous state machine of
 **        API function NvM_RestoreBlockDefaults.
 **        It resets retry counter and clears relevant flags.
 **/
extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_AscSt0( void );

/** \brief State 1 of the asynchronous state machine of
 **        API function NvM_RestoreBlockDefaults.
 **        Copies default data from default ROM block to user RAM block
 **        or calls the user init.
 **/

extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_AscSt1( void );

/** \brief State 2 of the asynchronous state machine of
 **        API function NvM_RestoreBlockDefaults.
 **        It calculates the CRC of the copied data.
 **/
extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_AscSt2( void );

/** \brief State 3 of the asynchronous state machine of
 **        API function NvM_RestoreBlockDefaults.
 **        It copies the calculated CRC to the user CRC buffer.
 **/
extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_AscSt3( void );

/** \brief State 4 of the asynchronous state machine of
 **        API function NvM_RestoreBlockDefaults.
 **        It performes mirror copy operation and sets the result of the request.
 **/
extern FUNC( void,NVM_CODE ) NvM_RestoreBlockDefaults_AscSt4( void );

/** \brief ******************** ReadBlock functions *********************************/
extern FUNC(void,NVM_CODE) NvM_InitStoredCrcBuf(void);
#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

/** \brief First state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It sets the service id.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async( void );

/** \brief First state of the asynchronous state machine of
 **        API function NvM_ReadPRAMBlock.
 **        It sets the service id.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadPRAMBlock_Async( void );

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

/** \brief Second state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It initializes the dataset index if necessary.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State0( void );

/** \brief Second state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It checks if a second pass is necessary to read
 **        the complete block data and triggers the second pass
 **        if check results positive.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State2( void );

/** \brief Fourth state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It calculates CRC of the block if configured.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State4( void );

/** \brief Fifth state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It checks if calcultated CRC is equal to copied CRC.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State5( void );

/** \brief Sixth state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It copies block data from internal buffer to user buffer
 **        if this is necessary.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State6( void );

/** \brief Tenth state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It only calls NvM_MemIfRead.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State10( void );

/** \brief State number 12 of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It calls MemIf_GetJobResult and evaluates the returned result.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_State12( void );

/** \brief Error handling function for service NvM_ReadBlock().
 **        Handles the Read Retry Counter.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_Retry( void );

/** \brief First error state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It  checks if a redundant block is processed and if so
 **        the copy process for the redundant data is triggered.
 **        Otherwise, It calls NvM_ReadBlock_Async_StateError2.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_StateError1( void );

/** \brief Second error state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It calls NvM_RestoreBlockDefaults() if configured for this block.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_StateError2( void );

/** \brief Third error state of the asynchronous state machine of
 **        API function NvM_ReadBlock.
 **        It evaluates the error status of this NvM_ReadBlock() request.
 **        In all cases other than an invalidated block,
 **        It reports the error to DEM.
 **/
extern FUNC( void,NVM_CODE ) NvM_ReadBlock_Async_StateError3( void );

/** \brief ******************** WriteBlock functions ********************************/

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

/** \brief First state of the asynchronous state machine of
 **        API function NvM_WriteBlock().
 **        It only sets the global service id.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async( void );

/** \brief First state of the asynchronous state machine of
 **        API function NvM_WritePRAMBlock().
 **        It only sets the global service id.
 **/
extern FUNC( void,NVM_CODE ) NvM_WritePRAMBlock_Async( void );

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */

/** \brief Prepares the state machine function pointers for processing
 **        redundant blocks. Used by NvM_WriteBlock().
 **/
extern FUNC(void,NVM_CODE) NvM_WriteBlock_ProcessRedundantBlock
(
  NvM_CurrentFunctionPtrType NextState
);

/** \brief Error handling function for service NvM_WriteBlock().
 **        Handle the Write Retry Counter.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_Error( void );

/** \brief Function to start CRC copy for service NvM_WriteBlock().
 **        If required, it copies the calculated CRC to the internal buffer.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_CopyCrc( void );

/** \brief Function to check if CRC of current Data is different then the CRC
 **        saved during last successful Read/Write. If different continue
 **        with normal handling of the Write process, if not then terminate the
 **        write process and report JOB_OK, like the copying would have taken place.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_CheckAndCopyCrc( void );

/** \brief This state of the NvM_WriteBlock() state machine
 **        initializes management data and starts Crc
 **        calculation if necessary.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_State1( void );

/** \brief Second state of the asynchronous state machine of
 **        API function NvM_WriteBlock().
 **        It handles the copying of data from the RAM block to
 **        NvM's internal buffer and starting the Crc calculation.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_State2( void );

/** \brief Third state of the asynchronous state machine of
 **        API function NvM_WriteBlock().
 **        It Merges the block block number with the dataset index
 **        and calls MemIf_Write().
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Async_State3( void );

/** \brief Evaluates the job result reported by the underlying drivers.
 **        It calls MemIf_Read() if write verification is enabled.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_ProcessJobRes_Async_State( void );

/** \brief Evaluates the job result reported by the underlying drivers.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_ProcessJobRes( void );

#if (NVM_WRITE_VERIFICATION == STD_ON)

/** \brief Evaluates the job result reported by the underlying drivers
 **        and does write verification.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_Verify_Async_State( void );

#endif

#if (NVM_PRE_WRITE_DATA_COMP == STD_ON)

/** \brief Evaluates the job result reported by the underlying drivers
 **        and does pre write data comparison.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_PreWriteDataComp_Async_State( void );

/** \brief Funcion called in case data comparison shall be skipped due to read failure.
**   It prepares the state machine for next state, NvM_WriteBlock_Async_State3.
 **/
extern FUNC( void,NVM_CODE ) NvM_WriteBlock_PreWriteDataComp_ReadError_State( void );
#endif

/** \brief ***************** SetRamBlockStatus functions *************************/

#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
/** \brief first state of the asynchronous state machine of API function
 ** NvM_SetRamBlockStatus_Async_CalcCrc_Start(). It finishes the asynchronous CRC
 ** calculation started by NvM_SetRamBlockStatus().
 ** Start CRC calculation for the changed RAM block.
 **/
extern FUNC(void, NVM_CODE) NvM_SetRamBlockStatus_Async_CalcCrc_Start(void);
#endif

/** \brief First state of the asynchronous state machine of API function
 **        NvM_ReadAll. It only initializes the block number.
 **/
extern FUNC(void,NVM_CODE) NvM_ReadAll_Async(void);

/** \brief Sets EEPROM driver mode and starts reading the Configuration Id.
 **/
extern FUNC(void,NVM_CODE) NvM_ReadAll_Async_State0(void);

/** \brief Iterates over all blocks and copy data from Non Volatile Memory to RAM.
 **/
extern FUNC(void,NVM_CODE) NvM_ReadAll_Async_State5(void);

/** \brief First state of the asynchronous state machine of API function
 **        NvM_ValidateAll. It only initializes the block number.
 **/
extern FUNC(void,NVM_CODE) NvM_ValidateAll_Async(void);

/** \brief Iterates over all blocks and validates them.
**/
extern FUNC(void,NVM_CODE) NvM_ValidateAll_Async_State0(void);

/** \brief Finishes single operation for NvM_ValidateAll multiblock operation.
**/
extern FUNC(void,NVM_CODE) NvM_ValidateAll_Async_State1(void);

/** \brief First state of the asynchronous state machine of API function
 **        NvM_WriteAll().
 **
 **        This state only sets the global service id and NvM_GlobalBlockNumber
 **/
extern FUNC(void,NVM_CODE) NvM_WriteAll_Async(void);

/** \brief Second state of the asynchronous state machine of API function
 **        NvM_WriteAll().
 **
 **        Iterate over all blocks and copy the RAMDataBlock to Non Volatile Memory
 **        if the block has assigned a permanent RAM block.
 **/
extern FUNC(void,NVM_CODE) NvM_WriteAll_Async_State1(void);
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#if ( defined(NVM_MULTICORE_ENABLED) )

#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>

/** \brief Checks if multicore flag is set and if not it will set it.
 **
 **        Functionality is executed in exclusive area to protect read/write on the flag.
 **
 ** \retval TRUE:  MultiCore status flag is successfully set.
 ** \retval FALSE: MultiCore status flag was already set.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(boolean,NVM_CODE) NvM_CheckSetMcFlag
(
  uint16 AdminBlockIndex
);

#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#endif

#if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)

#if (defined(NVM_READBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_ReadBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DstPtr Pointer to a temporary RAM block to where the
 **            NvM data shall be copied.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondReadBlock
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
);
#if (defined(NVM_READBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READBLOCK_CALLOUT */

#if (defined(NVM_WRITEBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_WriteBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_SrcPtr Pointer to the RAM data block which shall be
 **            copied to NVRAM.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondWriteBlock
(
  NvM_BlockIdType                         BlockId,
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr
);
#if (defined(NVM_WRITEBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEBLOCK_CALLOUT */

#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_RestoreBlockDefaults() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 ** \param[in] NvM_DestPtr Pointer to the RAM Data Block
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondRestoreBlockDefaults
(
  NvM_BlockIdType BlockId,
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr
);
#if (defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT */

#if (defined(NVM_READPRAMBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_ReadPRAMBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondReadPRAMBlock
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_READPRAMBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_READPRAMBLOCK_CALLOUT */

#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_WritePRAMBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondWritePRAMBlock
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_WRITEPRAMBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_WRITEPRAMBLOCK_CALLOUT */

#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_RestorePRAMBlockDefaults() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondRestorePRAMBlockDefaults
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT */


#if (defined(NVM_CANCELJOBS_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_CANCELJOBS_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_CancelJobs() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondCancelJobs
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_CANCELJOBS_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_CANCELJOBS_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1) */
#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
extern FUNC(void,NVM_CODE) NvM_InitFirstInitAll(void);
#endif
#if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)

#if (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0)
/** \brief First state of the asynchronous state machine of API function
 **        NvM_FirstInitAll. It only initializes the block number.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async(void);

/** \brief Second function of the state machine. It decrements the Global block
           number and if a block is to be processed by FirstInitAll the corresponding
           function shall be invoked.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_State1(void);

/** \brief Calls the lower layer write function to copy default data to NV.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_CopyToNv(void);

/** \brief Function processes the job result from the NV write.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_ProcessJobRestore_Async(void);

/** \brief Function that invalidates a block during FirstInitAll.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_Invalidate(void);

/** \brief Function processes the job result from the NV block invalidate.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_ProcessJobInvalidate_Async(void);

/** \brief The last function in the FirstInit state machine. If all lower layer
           have finished the job it will call the MultiPostService function.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_End(void);

/** \brief Intermediary function in the state machine. In this function the
           default data is copied to internal buffer.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_PrepareDefData(void);

/** \brief Intermediary function in the state machine. After the data is
           copied into internal buffer this function will calculate the CRC on
           that data and stored it also in the internal buffer to be written in NV memory.
 **/
extern FUNC(void,NVM_CODE) NvM_FirstInitAll_Async_CopyCrc(void);

#endif /* (NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS > 0) */
#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_InvalidateNvBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondInvalidateNvBlock
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_INVALIDATENVBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_INVALIDATENVBLOCK_CALLOUT */

#if (defined(NVM_ERASENVBLOCK_CALLOUT))
#define NVM_START_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */
/** \brief Checks if all the conditions to execute NvM_EraseNvBlock() API are met.
 **
 **        Performs DET checks according to the ASR specification.
 **
 ** \retval returns the block index (in admin/config array) if all conditions are met,
 **         API functionality can be executed. Otherwise retruns NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS.
 **
 ** \param[in] BlockId The block identifier. Selects the block whose
 **            configuration parameter NvMNvBlockIdentifier is equal to
 **            BlockId.
 **/
extern FUNC(uint16,NVM_CODE) NvM_CheckCondEraseNvBlock
(
  NvM_BlockIdType BlockId
);
#if (defined(NVM_ERASENVBLOCK_CALLOUT))
#define NVM_STOP_SEC_MC_SHARED_CODE
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* defined NVM_ERASENVBLOCK_CALLOUT */

#endif /* #if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3) */

#if (NVM_CRYPTO_HOOKS == STD_ON)
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
/** \brief State of the Crypto operation asyncronous state mashine. NvM will enter this job rigth after
           reading a block or before writing a block.
 **/

extern FUNC( void,NVM_CODE ) NvM_CryptoOperations_Async_State( void );

/** \brief State of the Error handling in case of crypto operation error.
 **/

extern FUNC( void,NVM_CODE ) NvM_CryptoOperations_Async_Error( void );
#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
#endif /* #if (NVM_CRYPTO_HOOKS == STD_ON) */
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef NVM_FUNCTION_INTERN_H */

/*=================[end of file NvM_FunctionIntern.h]=======================*/
