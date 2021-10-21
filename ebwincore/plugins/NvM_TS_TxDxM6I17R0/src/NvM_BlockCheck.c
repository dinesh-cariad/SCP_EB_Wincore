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
/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *    "A conversion should not be performed from pointer to void into pointer to object."
 *
 *    Reason: The portion of code is necessary and it has no alignment problems.
 *
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: false positive, see tasking issue TCVX-41885
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason: false positive, see tasking issue TCVX-41885
 *
 */
/*==================[inclusions]============================================*/
#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
/* prevent redundant declarations of RTE types */
#define NVM_INTERNAL_USE

#include <Std_Types.h>            /* AUTOSAR standard types */
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSMem.h>                /* Autosar Module  Base */
#include <MemIf.h>                /* Contains extern definition of function */
#include <SchM_NvM.h>             /* module specific header for             */
                                  /* Schedule Manager services              */
#include <NvM_CalcCrc.h>          /* Extern definitions of unit CalcCrc.    */
#include <NvM.h>                  /* configuration header*/
#include <NvM_BlockCheck.h>       /* own header */
#include <NvM_FunctionIntern.h>   /* module internal header needed for function declaration */
#include <NvM_DataIntern.h>       /* User Header Needed for Callout declaration*/

#if ( NVM_DEV_ERROR_DETECT == STD_ON )
/* !LINKSTO NVM556,1 */
#include <Det.h>                  /* API of module Det.*/
#endif

/* if any DEM event is switched on, include Dem.h */
#if (NVM_PROD_ERR_HANDLING_BLOCK_CHECK == TS_PROD_ERR_REP_TO_DEM)
  /* !LINKSTO SWS_NvM_00554,2 */
#include <Dem.h>
#endif

#include <NvM_UsrHdr.h>          /* User Header Needed for Callout declaration*/

#if (NVM_BLOCK_CHECK_ENABLED == STD_ON)
/*====================================[macros]=================================================*/
#if (defined NVM_BC_INVALID_QUEUE_EL)
#error NVM_BC_INVALID_QUEUE_EL is already defined
#endif
/** \brief Macro used inside the BC queue. */
#if (NVM_BC_NR_OF_BLOCKS > 255 )
#define NVM_BC_INVALID_QUEUE_EL 0xFFFFu
#else
#define NVM_BC_INVALID_QUEUE_EL 0xFFu
#endif

#if (defined NVM_BC_DISABLED_EXEC_COUNTER)
#error NVM_BC_DISABLED_EXEC_COUNTER is already defined
#endif
/** \brief Macro to check if BcDelay counter is configured to 0. */
#define NVM_BC_DISABLED_EXEC_COUNTER 0u


#if (defined NVM_BC_AUTOSTART_BIT)
#error NVM_BC_AUTOSTART_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_AUTOSTART_BIT 0x01U

#if (defined NVM_BC_AUTOSTART_EN)
#error NVM_BC_AUTOSTART_EN is already defined
#endif
/** \brief NVM_BC_AUTOSTART_EN: Check whether the BC CRC
 **        Compare feature is present
 **
 ** \param[in]  value : bit pattern from which the BC CRC Compare is extracted.
 **
 **/
#define NVM_BC_AUTOSTART_EN(value) (((value) & NVM_BC_AUTOSTART_BIT) != 0U)

#if (defined NVM_BC_CRCCMP_BIT)
#error NVM_BC_CRCCMP_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_CRCCMP_BIT 0x02U

#if (defined NVM_BC_CRCCMP_EN)
#error NVM_BC_CRCCMP_EN is already defined
#endif
/** \brief NVM_BC_CRCCMP_EN: Check whether the BC CRC
 **        Compare feature is present
 **
 ** \param[in]  value : bit pattern from which the BC CRC Compare is extracted.
 **
 **/
#define NVM_BC_CRCCMP_EN(value) (((value) & NVM_BC_CRCCMP_BIT) != 0U)

#if (defined NVM_BC_RAMCMP_BIT)
#error NVM_BC_RAMCMP_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_RAMCMP_BIT 0x04U

#if (defined NVM_BC_RAMCMP_EN)
#error NVM_BC_RAMCMP_EN is already defined
#endif
/** \brief NVM_BC_RAMCMP_EN: Check whether the BC RAM
 **        Compare feature is present
 **
 ** \param[in]  value : bit pattern from which the BC RAM Compare is extracted.
 **
 **/
#define NVM_BC_RAMCMP_EN(value) (((value) & NVM_BC_RAMCMP_BIT) != 0U)

#if (defined NVM_BC_REDCOPIESCMP_BIT)
#error NVM_BC_REDCOPIESCMP_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_REDCOPIESCMP_BIT 0x08U

#if (defined NVM_BC_REDCOPIESCMP_EN)
#error NVM_BC_REDCOPIESCMP_EN is already defined
#endif
/** \brief NVM_BC_REDCOPIESCMP_EN: Check whether the BC Redundant Copies
 **        Compare feature is present
 **
 ** \param[in]  value : bit pattern from which the BC Redd Copies Compare is extracted.
 **
 **/
#define NVM_BC_REDCOPIESCMP_EN(value) (((value) & NVM_BC_REDCOPIESCMP_BIT) != 0U)

#if (defined NVM_BC_AUTOREPAIR_BIT)
#error NVM_BC_AUTOREPAIR_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_AUTOREPAIR_BIT 0x10U

#if (defined NVM_BC_AUTOREPAIR_EN)
#error NVM_BC_AUTOREPAIR_EN is already defined
#endif
/** \brief NVM_BC_AUTOREPAIR_EN: Check whether the BC AutoRepair feature is present
 **
 ** \param[in]  value : bit pattern from which the BC Autorepair is extracted.
 **
 **/
#define NVM_BC_AUTOREPAIR_EN(value) (((value) & NVM_BC_AUTOREPAIR_BIT) != 0U)


#if (defined NVM_BC_ENABLE_API_BIT)
#error NVM_BC_ENABLE_API_BIT is already defined
#endif
/** \brief mask for BC AutoRepair bit */
#define NVM_BC_ENABLE_API_BIT 0x20U

#if (defined NVM_BC_ENABLE_API_EN)
#error NVM_BC_ENABLE_API_EN is already defined
#endif
/** \brief NVM_BC_ENABLE_API_EN: Check whether the BC API Enable/Disable is present
 **
 ** \param[in]  value : bit pattern from which the BC Enable API is extracted.
 **
 **/
#define NVM_BC_ENABLE_API_EN(value) (((value) & NVM_BC_ENABLE_API_BIT) != 0U)

#if (defined NVM_BC_RED_COPIES_FAILED_BIT)
#error NVM_BC_RED_COPIES_FAILED_BIT is already defined
#endif
/** \brief mask for BC Red Copies Compare Operation, 1- failed, 0 ok/ bit */
#define NVM_BC_RED_COPIES_FAILED_BIT 0x02U

#if (defined NVM_BC_RED_FIRST_COPY_FAILED_BIT)
#error NVM_BC_RED_FIRST_COPY_FAILED_BIT is already defined
#endif
/** \brief mask for BC Red Block Index0 BC Result, 1- failed, 0 ok/ bit */
#define NVM_BC_RED_FIRST_COPY_FAILED_BIT 0x04U

#if (defined NVM_BC_RED_SEC_COPY_FAILED_BIT)
#error NVM_BC_RED_SEC_COPY_FAILED_BIT is already defined
#endif
/** \brief mask for BC Red Block Index1 BC Result, 1- failed, 0 ok/ bit */
#define NVM_BC_RED_SEC_COPY_FAILED_BIT 0x08U

#if (defined NVM_BC_AUTOREPAIR_ONGOING_BIT)
#error NVM_BC_AUTOREPAIR_ONGOING_BIT is already defined
#endif
/** \brief mask for BC AutoRepair Ongoing bit */
#define NVM_BC_AUTOREPAIR_ONGOING_BIT 0x10U

#if (defined NVM_BC_FAILED_BIT)
#error NVM_BC_FAILED_BIT is already defined
#endif
/** \brief mask for BC mechanism check failed bit */
#define NVM_BC_FAILED_BIT 0x20U

#if (defined NVM_BC_CHECK_STATUS)
#error NVM_BC_CHECK_STATUS is already defined
#endif
/** \brief NVM_BC_CHECK_STATUS: Checks the status of the BC Element
 **
 ** \param[in]  status : bit pattern from which the status is extracted.
 **
 ** \param[in]  mask : bit pattern from which the status is extracted.
 **/
#define NVM_BC_CHECK_STATUS(status, mask) (((status) & mask) != 0U)

#if (defined NVM_BC_SET_STATUS)
#error NVM_BC_SET_STATUS is already defined
#endif
/** \brief NVM_BC_SET_STATUS: Set the mask corresponding bit status of the BC Element
 **
 ** \param[in]  status : bit pattern for which the status is set.
 **
 ** \param[in]  mask : bit pattern fir which the status is set.
 **/
#define NVM_BC_SET_STATUS(status, mask) ((status) |= mask)

#if (defined NVM_BC_CLEAR_STATUS)
#error NVM_BC_CLEAR_STATUS is already defined
#endif
/** \brief NVM_BC_CLEAR_STATUS: Set the mask corresponding bit status of the BC Element
 **
 ** \param[in]  status : bit pattern for which the status is cleared.
 **
 ** \param[in]  mask : bit pattern fir which the status is cleared.
 **/
#define NVM_BC_CLEAR_STATUS(status, mask) ((status) &= (~mask))

#if (defined NVM_BC_STATUS_EL_OFF)
#error NVM_BC_STATUS_EL_OFF is already defined
#endif
/** \brief Value of BcElStatus when the Element is OFF(needs Enable to start again BC).
 **/
#define NVM_BC_STATUS_EL_OFF             0x00U

#if (defined NVM_BC_STATUS_EL_IN_QUEUE)
#error NVM_BC_STATUS_EL_IN_QUEUE is already defined
#endif
/** \brief Value of BcElStatus when the Element is inserted in queue.
 **/
#define NVM_BC_STATUS_EL_IN_QUEUE             0x01U

#if (defined NVM_BC_STATUS_EL_DISABLED)
#error NVM_BC_STATUS_EL_DISABLED is already defined
#endif
/** \brief Value of BcElStatus when the Element was disabled from the API
 **        (status will transition in OFF ).
 **/
#define NVM_BC_STATUS_EL_DISABLED             0x02U

#if (defined NVM_BC_STATUS_EL_NOT_IN_QUEUE)
#error NVM_BC_STATUS_EL_NOT_IN_QUEUE is already defined
#endif
/** \brief Value of BcElStatus when the Element is not yet inserted in queue.
 **/
#define NVM_BC_STATUS_EL_NOT_IN_QUEUE             0x03U

#if (defined NVM_BC_STATUS_EL_JOB_FINISHED)
#error NVM_BC_STATUS_EL_JOB_FINISHED is already defined
#endif
/** \brief Value of BcElStatus when the Element processing is finished.
 **/
#define NVM_BC_STATUS_EL_JOB_FINISHED             0x04U

#if (defined NVM_BC_STATUS_EL_ENABLED)
#error NVM_BC_STATUS_EL_ENABLED is already defined
#endif
/** \brief Value of BcElStatus when the Element was enabled from the API
 **        (status will transition either in EL_NOT_IN_QUEUE or EL_IN_QUEUE
 **        depending on configuration of delay timer).
 **/
#define NVM_BC_STATUS_EL_ENABLED             0x05U

#if (defined NVM_BC_OFF)
#error NVM_BC_OFF is already defined
#endif
/** \brief Value of BcElState when element is disabled
 **/
#define NVM_BC_OFF             0x00U

#if (defined NVM_BC_ON)
#error NVM_BC_ON is already defined
#endif
/** \brief Value of BcElState when element is enabled
 **/
#define NVM_BC_ON             0x01U

#if (defined NVM_BC_OFF_EDGE)
#error NVM_BC_OFF_EDGE is already defined
#endif
/** \brief Value of BcElState was disabled from NvM_EnableBlockCheck API but the function
 **        NvM_BcProcessQueue() did not process it yet.
 **/
#define NVM_BC_OFF_EDGE             0x02U

#if (defined NVM_BC_ON_EDGE)
#error NVM_BC_ON_EDGE is already defined
#endif
/** \brief Value of BcElState was enabled from NvM_EnableBlockCheck API but the function
 **        NvM_BcProcessQueue() did not process it yet.
 **/
#define NVM_BC_ON_EDGE             0x03U

/*==================[type definitions]=======================================*/
/**  \brief Defines the elements of the Block Check Queue.
 **/
typedef struct {
  /** \brief Index of the Previous Queue Element **/
  uintBC BcQueuePrvEl;
  /** \brief Index of the Next Queue Element **/
  uintBC BcQueueNxtEl;
}NvM_BcQueueElType;

/**  \brief Defines the structure of the BC element that is currently in process.
 **/
typedef struct {
  /**  \brief Defines the type for state functions of
   **         asynchronous state machine.
   **/
  NvM_CurrentFunctionPtrType BcCurrentFctPtr;
  /** \brief Elements hold start address where the BC data is hold**/
  NvM_PtrToApplDataType PointerDataPrimary;
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
  /** \brief Elements hold start address where the BC data is hold
   **        for second mirror of a redundant block**/
  NvM_PtrToApplDataType PointerDataRedCopy;
#endif
  /** \brief Element Points to the source that is to be written**/
  const uint8 *WriteSource;
  /** \brief Start address for the data used for CRC calculation**/
  NvM_PtrToApplDataType CalcCrcAdress;
  /** \brief Element holding the calculated CRC**/
  uint32 CalculatedCrc;
  /** \brief Number of bytes that remains to be processed in the next CRC call**/
  uint32 CrcRemainingLenght;
  /** \brief Index of the BC element**/
  uintBC BcIdx;
  /** \brief Global Status of the BC element that is currently in process.**/
  uint8 BcGlobalStatus;
  /** \brief Dynamic Status of the BC element that is currently in process.**/
  uint8 DynStatus;
  /** \brief Data Index for the block that is to be processed.**/
  uint8 DataIndex;
  /** \brief Holds the number of Read Retries for the current block.**/
  uint8 ReadRetryCnt;
  /** \brief Holds the number of Write Retries for the current block.**/
  uint8 WriteRetryCnt;
}NvM_BcRunningElType;

/**  \brief Defines the elements of the Block Check AdminData.
 **/
typedef struct {
  /** \brief Execution Counter until the BC element will be introduced into the queue.**/
  uint8 BcExecCounter;
  /** \brief Status of the BC element.**/
  uint8 BcElStatus;
  /** \brief State of the BC element.
  **         Possible values : NVM_BC_ON, NVM_BC_ON_EDGE, NVM_BC_OFF, NVM_BC_OFF_EDGE
  **/
  uint8 BcElState;
}NvM_BcAdminDataType;
/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/
#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>
/** \brief Function to cancel the ongoing lower layer processing for the current
 **        block that is processed by BC.
 **
 **        Call MemIf_Cancel() if the device of the running block is not MEMIF_IDLE.
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcCancelActiveRun(void);

/** \brief Function to remove a BC element from queue.
 **
 **        Calls NvM_BcCancelActiveRun() if the element that has to be removed
 **        from queue is the one currently running.
 **        Remove element from queue.
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcRemoveElFromQueue(uintBC BcIdx);

/** \brief Function to insert a BC element in queue.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcInsertElInQueue(uintBC BcIdx);

/** \brief Gets the address where the block data starts.
 **
 ** \param[in] Address of the block RAM.
 ** \param[in] Block Descriptor
 ** \retval :  Data Address
 **
 **
 **/
STATIC FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_BcGetDataAddress
(
  NvM_PtrToApplDataType Buffer,
  uint32 BlockDesc
);

/** \brief Gets the address where the block data starts.
 **
 ** \param[in] Address of the destination block RAM.
 ** \param[in] Address of the Source block RAM/ROM.
 ** \param[in] Block Id
 ** \param[in] Block Descriptor
 ** \param[in] Block length
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcFillBufferForWrite
(
  NvM_PtrToApplDataType DestBuffer,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) SrcBuffer,
  uint16 BlockId,
  uint32 BlockDesc,
  uint16 BlockLenght
);

/** \brief Gets the Lower layer device block number.
 **
 ** \param[in] NvM block descriptor table index.
 ** \retval :  Lower Device Block Number
 **
 **
 **/
STATIC FUNC(uint16,NVM_CODE) NvM_BcGetLowerDeviceBlockNumber(uint16 BlockIndex);

/** \brief First state in the BC state machine.Initializes variables needed for BC process.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_StartBlkCheck(void);


/** \brief Triggers the MemIf_Read. If the Read is accepted the next function in the SM will be
 **     NvM_BcSm_ReadBlockFromNv_Result, otherwise it will call NvM_BcSm_ReadBlockFromNv_Retry().
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv(void);

/** \brief If the Read retry is not reached the next function in the SM will be
 **       NvM_BcSm_ReadBlockFromNv, otherwise it will call NvM_BcSm_SetErrorResult();
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv_Retry(void);

/** \brief Will check the Job status of the underlying device. If it's pending then
 **       the function will exit without doing anything and willl be called again in
 **       the NvM main cycle. If the job was ok then, depending on the configuration
 **       the next comparison SM function is called. If the job returns invalidated
 **       then the BC will finish for this Block without result. If the result is any
 **       other..e.g. error,inconsistent, then the function NvM_BcSm_SetErrorResult()
 **       will be called.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv_Result(void);

/** \brief Will check if the CRC calculated on the read data is the same as the one
 **        stored in NV. If Yes, then the next comparison function will be introduced
 **        in the SM, otherwise NvM_BcSm_SetErrorResult will be called.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_Check_Crc(void);

/** \brief Will start the CRC calculation. If more cycles are needed for the complete
 **       CRC calculation to be done then NvM_BcSm_CalculateCrc_Async will be set in
 **       the SM and crc calculation will continue in the next NvM main cycle.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CalculateCrc(void);

/** \brief Will continue CRC calculation and if needed the same function will be called
 **        in the next NvM main cycle.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CalculateCrc_Async(void);

/** \brief Will compare the read data with the data stored in user RAM. If different
 **        function NvM_BcSm_SetErrorResult will be called.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CompareRam(void);

#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
/** \brief If both copies of a redundant block are read a comparison will be done between
 **       the data. If different NvM_BcSm_SetErrorResult will be called.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CompareReddCopies(void);
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */

/** \brief Will log a production error for the failed block and then call the function NvM_BcSm_CheckResults()
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_SetErrorResult(void);

/** \brief If all the block copies for the current block have been read and checked and
 **      if autorepair is enabled for this block then NvM_BcSm_StartAutoRepair is called
 **      otherwise the Block check will finish for this block. If not all the copies of
 **      the block have been read/checked then the function NvM_BcSm_ReadBlockFromNv will
 **      be introduced in the SM and block check will continue in the next NvM cycle.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CheckResults(void);

/** \brief Starts the autorepair SM.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_StartAutoRepair(void);

/** \brief Calls the write function.
 **
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv(void);

/** \brief If the Write retry is not reached the next function in the SM will be
 **       NvM_BcSm_WriteBlockToNv, otherwise the autorepair will abort without completing
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv_Retry(void);

/** \brief Will check the Job status of the underlying device. If it's pending then
 **       the function will exit without doing anything and willl be called again in
 **       the NvM main cycle. If the job was ok then if no more blocks are needed to be
 **       written it will finish the Block Check process, otherwise it will continue
 **       with the next block.. If the job returns anything other result then the
 **       Block Check will finish for this Block without auto repair.
 **/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv_Results(void);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>
/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/
/*==================[internal data]==========================================*/

#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

VAR( uint8,NVM_APPL_DATA ) NvM_BcGlobalDisableFlag = (uint8)STD_OFF;

#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>

#if (NVM_BC_NR_OF_BLOCKS > 255)
#define NVM_START_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>
#else
#define NVM_START_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>
#endif
/**  \brief Variable holds the index of the last element in the BC queue.
 **/
STATIC VAR( uintBC, NVM_VAR_CLEARED ) NvM_BcQueueBackEl;
#if (NVM_BC_NR_OF_BLOCKS > 255)
#define NVM_STOP_SEC_VAR_CLEARED_16
#include <NvM_MemMap.h>
#else
#define NVM_STOP_SEC_VAR_CLEARED_8
#include <NvM_MemMap.h>
#endif

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>
/**  \brief Variable holds the first BC element in queue, also
 **         the one that is currently in process.
 **/
STATIC VAR( NvM_BcRunningElType, NVM_VAR_CLEARED ) NvM_BcQueueFrontEl;
/**  \brief Array holds all the queue elements of the BC. The ones that
 **         are inserted in the queue and the ones that are not yet.
 **/
STATIC VAR( NvM_BcQueueElType, NVM_VAR_CLEARED ) NvM_BcQueueData[NVM_BC_NR_OF_BLOCKS];
/**  \brief Array holds all BC elements and their information, status
 **         and execution counter.
 **/
STATIC VAR( NvM_BcAdminDataType, NVM_VAR_CLEARED ) NvM_BcAdminData[NVM_BC_NR_OF_BLOCKS];
#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <NvM_MemMap.h>
/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/*==============================NvM_BcGetDataAddress============================*/
STATIC FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_BcGetDataAddress
(
  NvM_PtrToApplDataType Buffer,
  uint32 BlockDesc
)
{
  NvM_PtrToApplDataType DataBufPtr = Buffer;

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  /* If Static Block Id is enabled, exclude Block Header. */
  if (NVM_BD_USESTATICBLOCKID(BlockDesc))
  {
    DataBufPtr = &(Buffer[NVM_BLOCKID_LENGTH]);
  }
#else
  TS_PARAM_UNUSED(BlockDesc);
#endif
  return DataBufPtr;
}

/*==============================NvM_BcFillBufferForWrite============================*/
STATIC FUNC(void,NVM_CODE) NvM_BcFillBufferForWrite
(
  NvM_PtrToApplDataType DestBuffer,
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) SrcBuffer,
  uint16 BlockId,
  uint32 BlockDesc,
  uint16 BlockLenght
)
{

#if (NVM_STATIC_BLOCKID_CHECK == STD_ON)
  uint16 LocalBlockId = BlockId;
  /* If Static Block Id is enabled copy first the block id. */
  if (NVM_BD_USESTATICBLOCKID(BlockDesc))
  {
    TS_MemCpy( DestBuffer,
              (const uint8*)&LocalBlockId,
               NVM_BLOCKID_LENGTH
              );
    TS_MemCpy( &(DestBuffer[NVM_BLOCKID_LENGTH]),
               SrcBuffer,
               BlockLenght
             );
  }
  else
#else
  TS_PARAM_UNUSED(BlockDesc);
  TS_PARAM_UNUSED(BlockId);
#endif
  {
    TS_MemCpy( DestBuffer,
               SrcBuffer,
               BlockLenght
             );
  }
}
/*===================================NvM_BcSm_StartBlkCheck================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_StartBlkCheck(void)
{
  /*BC data index is set to the last copy so the Read starts from last to first */
  NvM_BcQueueFrontEl.DataIndex = NvM_BlockDescriptorTable[NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex].nvBlockNum - 1;
  /*Store addresses that will be used for Read/Write*/
  NvM_BcQueueFrontEl.PointerDataPrimary = &NvM_InternalBuffer[0U];
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
  NvM_BcQueueFrontEl.PointerDataRedCopy = &NvM_InternalBuffer[0U];
#endif
  NvM_BcQueueFrontEl.WriteSource = NULL_PTR;
  /*Reset BC dynamic status flags*/
  NvM_BcQueueFrontEl.DynStatus = 0U;
  NvM_BcQueueFrontEl.ReadRetryCnt = 1U; /*One Read retry for each block*/
  NvM_BcQueueFrontEl.WriteRetryCnt = 1U;/*One Write retry for each block*/
  NvM_BcQueueFrontEl.CalculatedCrc = 0U;
  NvM_BcQueueFrontEl.CrcRemainingLenght = 0U;
  /*trigger Read*/
  NvM_BcSm_ReadBlockFromNv();
}

/*===================================NvM_BcSm_ReadBlockFromNv================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  uint16 BlkLenght = NvM_GetBlockLength(NvM_BlockDescriptorTable[BlockIndex].blockDesc,
                                        NvM_BlockDescriptorTable[BlockIndex].nvBlockLength) +
                     NvM_GetBlockCRCLength(NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  const MemIf_StatusType Status = MemIf_GetStatus(NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc));
  if ( Status != MEMIF_BUSY)
  {
    /*Triger Read Block*/
    if ( MemIf_Read(
          NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc),
          NvM_BcGetLowerDeviceBlockNumber(BlockIndex), 0U,
          NvM_BcQueueFrontEl.PointerDataPrimary, BlkLenght ) == E_OK )
    {
      /*Prepare state machine to check read result in next cycle*/
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_ReadBlockFromNv_Result;
    }
    else
    {
      /* Perform the Read Retry. */
      NvM_BcSm_ReadBlockFromNv_Retry();
    }
  }
  else
  {
    /* If the lower Try again next NvM main cycle*/
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_ReadBlockFromNv;
  }
}

/*===================================NvM_BcSm_ReadBlockFromNv_Retry================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv_Retry(void)
{
  /*Check if Read retry reached the limit*/
  if ( NvM_BcQueueFrontEl.ReadRetryCnt !=0U )
  {
    /* try to read the same block again */
    NvM_BcQueueFrontEl.ReadRetryCnt--;
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_ReadBlockFromNv;
  }
  else

  {
    /*Retry limit reach, go into error state for this block copy*/
    NvM_BcSm_SetErrorResult();
  }
}

/*===================================NvM_BcSm_ReadBlockFromNv_Result================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_ReadBlockFromNv_Result(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  const MemIf_JobResultType Result = MemIf_GetJobResult( NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc) );
  switch( Result )
  {
    case MEMIF_JOB_PENDING:
    {
      /*wait for next NvM Main function*/
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
      break;
    }
    case MEMIF_JOB_OK:
    {
      /*If CRC Compare mechanism is configured trigger the CRC Calculation*/
      if (NVM_BC_CRCCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor))
      {
        NvM_BcSm_CalculateCrc();
      }
      else
      {
      /*If RAM Compare mechanism is configured trigger the RAM Comparison*/
        if (NVM_BC_RAMCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor))
        {
         NvM_BcSm_CompareRam();
        }
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
        else /*NVM_BC_REDCOPIESCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor)*/
        {
          /*Trigger Redundant Copies Compare*/
          NvM_BcSm_CompareReddCopies();
        }
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */
      }
      break;
    }
    /* !LINKSTO NvM.BlockCheck.PreConditions.BlocksErased,1 */
    /* !LINKSTO NvM.BlockCheck.PreConditions.BlocksInvalidated,1 */
    case MEMIF_BLOCK_INVALID:
    {
      /*If the Read returns a block as INVALID proceed further considering this block as ok*/
      NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
      /*Exit Bc SM*/
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
      break;
    }
    default:
    {
      /* !LINKSTO NvM.VariableBlockLength.BlockCheck,1 */
      /* In case MemIf delivered an other error e.g. MEMIF_JOB_FAILED
       * Block Check for this block shall be considered as failed.
       */
      NvM_BcSm_SetErrorResult();
      break;
    }
  }/* end of switch( Result ) */
}

/*===================================NvM_BcSm_Check_Crc================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_Check_Crc(void)
{
  NvM_RequestResultType CrcCompResult = NVM_REQ_NOT_OK;
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  uint32 Length = NvM_BlockDescriptorTable[BlockIndex].nvBlockLength;
  /* Get the data address of the Read Block*/
  NvM_PtrToApplDataType pDataAddress = NvM_BcGetDataAddress(NvM_BcQueueFrontEl.PointerDataPrimary,
                                                      NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  /* Compare the calculated CRC with the ONE stored in NV*/
  CrcCompResult = NvM_CheckCrc(&pDataAddress[Length],
                            (NvM_PtrToApplDataType)(&NvM_BcQueueFrontEl.CalculatedCrc),
                            NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  /* !LINKSTO NvM.BlockCheck.Processing.CrcCheck,1 */
  if(CrcCompResult == NVM_REQ_OK)
  {
    /* If CRC is ok proceed to do the RAM comparison, if configured*/
    if (NVM_BC_RAMCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor))
    {
     NvM_BcSm_CompareRam();
    }
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
    /* If CRC is ok proceed to do the Redundant Copies comparison, if configured*/
    else if (NVM_BC_REDCOPIESCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor))
    {
      /*if index is 0 call compare only if index1 mirror was ok*/
      if(NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_SEC_COPY_FAILED_BIT) == TRUE)
      {
        NvM_BcSm_CheckResults();
      }
      else
      {
        NvM_BcSm_CompareReddCopies();
      }
    }
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */
    else
    {
      /*If no other Comparison is configured move to next block index if any*/
      NvM_BcSm_CheckResults();
    }
  }
  else
  {
    /* If CRC check failed then Block Check for this block shall be considered as failed.*/
    NvM_BcSm_SetErrorResult();
  }
}

/*==============================NvM_BcCalculateCrc============================*/
STATIC FUNC(void, NVM_CODE) NvM_BcSm_CalculateCrc(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  uint32 Length = NvM_BlockDescriptorTable[BlockIndex].nvBlockLength;
  NvM_PtrToApplDataType DataAddress = NvM_BcGetDataAddress(NvM_BcQueueFrontEl.PointerDataPrimary,
                                                     NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  boolean CopyCrc = FALSE;
  /* Check if Crc should be calculated in parts */
  if (Length > NVM_CRC_NUM_OF_BYTES)
  {
    /* Save the remaining length to continue Crc calculation */
    NvM_BcQueueFrontEl.CrcRemainingLenght = Length - NVM_CRC_NUM_OF_BYTES;
    /* Use only the configured size for this calculation */
    Length = NVM_CRC_NUM_OF_BYTES;

    /* Save the start address of data block */
    NvM_BcQueueFrontEl.CalcCrcAdress = DataAddress;

    /* Set state function to continue Crc calculation */

    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_CalculateCrc_Async;
    /* MainFunction must terminate after first call */
  }
  else
  {
    /*check if crc is triggered from a read process or from a autorepair write*/
    if(NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_AUTOREPAIR_ONGOING_BIT))
    {
      CopyCrc = TRUE;
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
    }
    else
    {
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_Check_Crc;
    }
  }
  NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
  /*Call the function that does the actual CRC calculation*/
  NvM_CalcCrc_ProcessCrc(DataAddress,
                        (NvM_PtrToApplDataType)&NvM_BcQueueFrontEl.CalculatedCrc,
                        Length, NvM_BlockDescriptorTable[BlockIndex].blockDesc,
                        TRUE);
  /* If the CRC calculation is done during an Auto repair process the CRC has to be copied into
     internal buffer to be written in NV*/
  if(CopyCrc == TRUE)
  {
    NvM_CopyCrc( &DataAddress[Length],
                 (NvM_PtrToApplDataType)&NvM_BcQueueFrontEl.CalculatedCrc,
                 NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  }
}

/*===================================NvM_BcSm_CalculateCrc_Async================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CalculateCrc_Async(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  uint32 CurrentCrcLength = 0U;
  uint32 Length = NvM_BlockDescriptorTable[BlockIndex].nvBlockLength;
  NvM_PtrToApplDataType DataAddress = NvM_BcGetDataAddress(NvM_BcQueueFrontEl.PointerDataPrimary,
                                                     NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  boolean CopyCrc = FALSE;

  if (NvM_BcQueueFrontEl.CrcRemainingLenght > NVM_CRC_NUM_OF_BYTES)
  {
    NvM_BcQueueFrontEl.CrcRemainingLenght -= NVM_CRC_NUM_OF_BYTES;
    CurrentCrcLength = NVM_CRC_NUM_OF_BYTES;
    /* MainFunction must terminate after first call */
  }
  else
  {
    CurrentCrcLength = NvM_BcQueueFrontEl.CrcRemainingLenght;
    /*check if crc is triggered from a read process or from a autorepair write*/
    if(NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_AUTOREPAIR_ONGOING_BIT))
    {
      CopyCrc = TRUE;
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
    }
    else
    {
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_Check_Crc;
    }

  }
  NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;

  /* Update calculation address for this cycle */
  NvM_BcQueueFrontEl.CalcCrcAdress = &(NvM_BcQueueFrontEl.CalcCrcAdress[NVM_CRC_NUM_OF_BYTES]);
  /*Call the function that does the actual CRC calculation*/
  NvM_CalcCrc_ProcessCrc(NvM_BcQueueFrontEl.CalcCrcAdress,
                        (NvM_PtrToApplDataType)&NvM_BcQueueFrontEl.CalculatedCrc,
                         CurrentCrcLength,
                         NvM_BlockDescriptorTable[BlockIndex].blockDesc,
                         FALSE);
  /* If the CRC calculation is done during an Auto repair process the CRC has to be copied into
     internal buffer to be written in NV*/
  if(CopyCrc == TRUE)
  {
    NvM_CopyCrc( &DataAddress[Length],
                 (NvM_PtrToApplDataType)&NvM_BcQueueFrontEl.CalculatedCrc,
                 NvM_BlockDescriptorTable[BlockIndex].blockDesc);
  }
}

/*===================================NvM_BcSm_CompareRam================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CompareRam(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  NvM_PtrToApplDataType pDataAddress;
  /* Proceed with RAM comparison only if the permananet RAM is valid*/
  if (NvM_AdminBlockTable[BlockIndex].NvMValid == NVM_BLOCK_VALID)
  {
    pDataAddress = NvM_BcGetDataAddress(NvM_BcQueueFrontEl.PointerDataPrimary,
                                        NvM_BlockDescriptorTable[BlockIndex].blockDesc);
    /* Compare the Read NV data with the permanent RAM*/
    /* !LINKSTO NvM.BlockCheck.Processing.RamCompare,1 */
    if (TS_MemCmp(pDataAddress, NvM_BlockDescriptorTable[BlockIndex].ramBlockDataAddress,
                  NvM_BlockDescriptorTable[BlockIndex].nvBlockLength) != E_OK)
    {
      NvM_BcSm_SetErrorResult();
    }
    else
    {
      /*Proceed to next Block Copy if any*/
      NvM_BcSm_CheckResults();
    }
  }
  else
  {
    /*Proceed to next Block Copy if any*/
    NvM_BcSm_CheckResults();
  }
}
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
/*===================================NvM_BcSm_CompareReddCopies================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CompareReddCopies(void)
{
  uint16 length;
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  /*Check if both redundant copies have been read*/
  if(NvM_BcQueueFrontEl.DataIndex == 1U)
  {
    /*Adjust data index for first mirror*/
    NvM_BcQueueFrontEl.DataIndex--;
    /*Store the address of the data for the second redundant copy*/
    NvM_BcQueueFrontEl.PointerDataRedCopy = NvM_BcQueueFrontEl.PointerDataPrimary;
    /*Get a new address to copy the first copy*/
    NvM_BcQueueFrontEl.PointerDataPrimary = &NvM_IntermediateReadBuffer[0U];
    /*Proceed again to read the first copy*/
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_ReadBlockFromNv;
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
  }
  else
  {
    length = NvM_GetBlockLength(NvM_BlockDescriptorTable[BlockIndex].blockDesc,
        NvM_BlockDescriptorTable[BlockIndex].nvBlockLength);
    /*Compare the data of the 2 redundant copies of a block*/
    /* !LINKSTO NvM.BlockCheck.Processing.RedCopiesCompare,1 */
    if (TS_MemCmp(NvM_BcQueueFrontEl.PointerDataPrimary,
                  NvM_BcQueueFrontEl.PointerDataRedCopy,
                  length) != E_OK)
    {
      /*Set failed bit in case the comparison fails*/
      NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_FAILED_BIT);
      NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_COPIES_FAILED_BIT);
    }
    NvM_BcSm_CheckResults();
  }
}
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */

/*===================================NvM_BcSm_SetErrorResult================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_SetErrorResult(void)
{
  /*Set Failed BIT */
  NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_FAILED_BIT);
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
  if(NvM_BcQueueFrontEl.DataIndex == 0U)
  {
    /**/
    NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_FIRST_COPY_FAILED_BIT);
  }
  if(NvM_BcQueueFrontEl.DataIndex == 1U)
  {
    NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_SEC_COPY_FAILED_BIT);
  }
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)*/
  NvM_BcSm_CheckResults();

}

/*===================================NvM_BcSm_CheckResults================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_CheckResults(void)
{
  /*Check if BC is finished for all Block Copies*/
  if(NvM_BcQueueFrontEl.DataIndex == 0U)
  {
    /*Check if Block check failed, if Yes trigger a production error for the block*/
    if(NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_FAILED_BIT))
    {
#if ( NVM_PROD_ERR_HANDLING_BLOCK_CHECK == TS_PROD_ERR_REP_TO_DET )
      NVM_DET_REPORT_ERROR( NVM_ENABLE_BC_API_ID, NVM_E_BLOCK_CHECK );
#elif ( NVM_PROD_ERR_HANDLING_BLOCK_CHECK == TS_PROD_ERR_REP_TO_USER )
      NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex,
                                       NVM_ENABLE_BC_API_ID, NVM_E_USER_CALLOUT_BLOCK_CHECK);
#elif ( NVM_PROD_ERR_HANDLING_BLOCK_CHECK == TS_PROD_ERR_REP_TO_DEM )
#if (NVM_BLOCK_CHECK_DEM_EVENT_ID != NVM_INVALID_DEM_EVENTID)
      NVM_DEM_REPORT_ERROR(NVM_BLOCK_CHECK_DEM_EVENT_ID, DEM_EVENT_STATUS_FAILED);
#endif
#endif
    }
    /* !LINKSTO NvM.BlockCheck.Processing.AutoRepair,1 */
    /*If Block Check failed and autorepair is enabled start autorepair*/
    if((NVM_BC_AUTOREPAIR_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor)) &&
        (NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_FAILED_BIT)))
    {
      /*Start AutoRepair process in the next NvM Main cycle*/
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_StartAutoRepair;
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    }
    else
    {
      /*BlockCheckIsFinished and no error*/
      NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
      /*Exit Bc SM*/
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    }
  }
  else
  {
    /*Adjust data index for the next Block copy to be Read and start Read again*/
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    NvM_BcQueueFrontEl.DataIndex--;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_ReadBlockFromNv;
  }
}

/*===================================NvM_BcSm_StartAutoRepair================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_StartAutoRepair(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  boolean AbortAutoRepair = FALSE;
  NVM_BC_SET_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_AUTOREPAIR_ONGOING_BIT);
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
  if(NVM_BC_REDCOPIESCMP_EN(NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BcBlkDescriptor))
  {
    /* !LINKSTO NvM.BlockCheck.Processing.AutoRepair_RedCopiesCompFail,1 */
    if(NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_COPIES_FAILED_BIT))
    {
      /*If Redundant Copies configured and the compare between the copies failed the data of
       the first copy will be used to write the second one.*/
      NvM_BcQueueFrontEl.WriteSource = (const uint8*)NvM_BcQueueFrontEl.PointerDataPrimary;
      NvM_BcQueueFrontEl.DataIndex = 1U;
    }
    else if ((NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_FIRST_COPY_FAILED_BIT)) &&
             (NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_SEC_COPY_FAILED_BIT)))
    {
      /*If Redundant Copies configured and the CRC Compare failed for both copies the autorepair will
       abort as there is not valid data to be used for repair.*/
      AbortAutoRepair = TRUE;
    }
    /* !LINKSTO NvM.BlockCheck.Processing.AutoRepair_RedCopiesCrcFail,1 */
    else if (NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_SEC_COPY_FAILED_BIT))
    {
      /*If Redundant Copies configured and the CRC Compare failed only for the second copy, use
        the data from first copy to repair the second one.*/
      NvM_BcQueueFrontEl.WriteSource = (const uint8*)NvM_BcQueueFrontEl.PointerDataPrimary;
      NvM_BcQueueFrontEl.DataIndex = 1U;
    }
    else
    {
      /*If Redundant Copies configured and the CRC Compare failed only for the first copy, use
        the data from the second copy to repair the first one.*/
      NvM_BcQueueFrontEl.WriteSource = (const uint8*)NvM_BcQueueFrontEl.PointerDataRedCopy;
      /*index is already 0 but to articulate that we shall write the first copy*/
      NvM_BcQueueFrontEl.DataIndex = 0U;
    }
  }
  else
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)*/
  {
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
    if (NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc) == NVM_BD_BLOCKMNGTTYPE_REDUNDANT)
    {
      if(!NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_FIRST_COPY_FAILED_BIT))
      {
        /*If we have a redundant block for which the first copy did not failed during BlockCheck then
        it means the second one will have to be written*/
        NvM_BcQueueFrontEl.DataIndex = 1U;
      }
    }
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */
    /* !LINKSTO NvM.BlockCheck.Processing.AutoRepair_ConditionRam,1 */
    if((NvM_BlockDescriptorTable[BlockIndex].ramBlockDataAddress != NULL_PTR) &&
    (NvM_AdminBlockTable[BlockIndex].NvMValid == NVM_BLOCK_VALID))
    {
      /*If we have permanent RAM configured and is valid we shall copy it's data to internal
        buffer to be used for autorepair.*/
      /* Deviation MISRAC2012-1 */
      NvM_BcFillBufferForWrite(NvM_BcQueueFrontEl.PointerDataPrimary,
                               (const uint8*)NvM_BlockDescriptorTable[BlockIndex].ramBlockDataAddress,
                               NvM_BlockDescriptorTable[BlockIndex].nvBlockIdentifier,
                               NvM_BlockDescriptorTable[BlockIndex].blockDesc,
                               NvM_BlockDescriptorTable[BlockIndex].nvBlockLength);
      NvM_BcQueueFrontEl.WriteSource = (const uint8*)NvM_BcQueueFrontEl.PointerDataPrimary;
    }
    /* !LINKSTO NvM.BlockCheck.Processing.AutoRepair_ConditionRom,1 */
    else if (NvM_BlockDescriptorTable[BlockIndex].romBlockDataAddress != NULL_PTR)
    {
      /*If ROM defaults are configured we shall copy it's data to internal
        buffer to be used for autorepair.*/
      NvM_BcFillBufferForWrite(NvM_BcQueueFrontEl.PointerDataPrimary,
                         (const uint8*)NvM_BlockDescriptorTable[BlockIndex].romBlockDataAddress,
                         NvM_BlockDescriptorTable[BlockIndex].nvBlockIdentifier,
                         NvM_BlockDescriptorTable[BlockIndex].blockDesc,
                         NvM_BlockDescriptorTable[BlockIndex].nvBlockLength);
      NvM_BcQueueFrontEl.WriteSource = (const NvM_PtrToApplDataType)NvM_BcQueueFrontEl.PointerDataPrimary;
    }
    else
    {
      /*If there is not valid data to be use for autorepair then the process shall be aborted*/
      AbortAutoRepair = TRUE;
    }
  }
  if (AbortAutoRepair == FALSE)
  {
    if(NVM_BD_CRCTYPE(NvM_BlockDescriptorTable[NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex].blockDesc) != NVM_BD_CRCTYPE_NONE)
    {
      /*If Block has CRC configured proceed to calculate the CRC on data first*/
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_CalculateCrc;
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    }
    else
    {
      /*Proceed to writing the data*/
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    }
  }
  else
  {
    /*Force abort AutoRepair - BlockCheckIsFinished*/
    NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
    /*Exit Bc SM*/
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
  }

}
/*===================================NvM_BcSm_WriteBlockToNv================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  const MemIf_StatusType Status = MemIf_GetStatus(NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc));
  if ( Status != MEMIF_BUSY)
  {
    /*Triger Write Block*/
    if ( MemIf_Write(
          NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc),
          NvM_BcGetLowerDeviceBlockNumber(BlockIndex),
          NvM_BcQueueFrontEl.WriteSource) == E_OK
       )
    {
      /*Proceed to check Write Result in the next NvM Main cycle*/
      NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv_Results;
      NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    }
    else
    {
      /* Perform the Write Retry. */
      NvM_BcSm_WriteBlockToNv_Retry();
    }
  }
  else
  {
    /*If lower layer is BUSY try again next NvM Main cycle*/
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
  }
}
/*===================================NvM_BcSm_WriteBlockToNv_Retry================================*/
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv_Retry(void)
{
  if ( NvM_BcQueueFrontEl.WriteRetryCnt !=0U )
  {
    /* try to Write the same block again */
    NvM_BcQueueFrontEl.WriteRetryCnt--;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
  }
  else
  {
    /*Retry limit reach, abort autorepair*/
    NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
    /*Exit Bc SM*/
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
  }
}
STATIC FUNC(void,NVM_CODE) NvM_BcSm_WriteBlockToNv_Results(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  const MemIf_JobResultType Result =
    MemIf_GetJobResult(NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc));

  /* evaluate Result */
  if (Result == MEMIF_JOB_PENDING)
  {
    /* Do nothing. The lower layer has not finished the job yet. */
  }
  else /* MEMIF_JOB_OK or MEMIF_JOB_FAILED */
  {
    if (Result == MEMIF_JOB_OK)
    {
#if (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0)
      if((NvM_BcQueueFrontEl.DataIndex == 0U) &&
         (NVM_BD_BLOCKMNGTTYPE(NvM_BlockDescriptorTable[BlockIndex].blockDesc) == NVM_BD_BLOCKMNGTTYPE_REDUNDANT) &&
         (NVM_BC_CHECK_STATUS(NvM_BcQueueFrontEl.DynStatus, NVM_BC_RED_SEC_COPY_FAILED_BIT)))
      {
        /*If Block is of type Redundant and the second copy failed proceed to write it*/
        NvM_BcQueueFrontEl.DataIndex = 1U;
        NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_WriteBlockToNv;
      }
      else
#endif /* (NVM_BC_NR_OF_BLOCKS_REDUNDANT > 0) */
      {
        /* Finish Block Check process for this block*/
        NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
      }
    }
    else
    {
      /* Finish Block Check process for this block*/
      NvM_BcAdminData[NvM_BcQueueFrontEl.BcIdx].BcElStatus = NVM_BC_STATUS_EL_JOB_FINISHED;
    }
  }
  /*Exit Bc SM*/
  NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_PENDING;
}

/*==============================NvM_BcGetLowerDeviceBlockNumber============================*/
STATIC FUNC(uint16,NVM_CODE) NvM_BcGetLowerDeviceBlockNumber(uint16 BlockIndex)
{
  uint16 ret;

  ret = ( uint16 )( ( uint16 )
     ( NvM_BlockDescriptorTable[BlockIndex].nvBlockBaseNum )
       |
    ( ( uint16 )NvM_BcQueueFrontEl.DataIndex ) );
  return ret;
}

/*==============================NvM_BcCancelActiveRun============================*/
STATIC FUNC(void,NVM_CODE) NvM_BcCancelActiveRun(void)
{
  uint16 BlockIndex = NvM_BcCfgData[NvM_BcQueueFrontEl.BcIdx].BlockIndex;
  MemIf_StatusType Status;
  Status = MemIf_GetStatus( NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc) );
  if ( Status != MEMIF_IDLE )
  {
    /*Cancel lower layer activities when an abort occurs during a Block Check process*/
    MemIf_Cancel(NVM_BD_DEVICEID(NvM_BlockDescriptorTable[BlockIndex].blockDesc) );
  }
}

/*==============================NvM_BcRemoveElFromQueue============================*/
STATIC FUNC(void,NVM_CODE) NvM_BcRemoveElFromQueue(uintBC BcIdx)
{
  /*Check if the element that has to be removed is the one in the front queue*/
  if (BcIdx == NvM_BcQueueFrontEl.BcIdx)
  {
   /*When removing from the queue the block that is currently processed first cancel
     the lower layer underlying operations*/
   NvM_BcCancelActiveRun();
   /*Check if any other elements are in queue and if yes put the next one
     in the FrontQueue element*/
   if(NvM_BcQueueData[BcIdx].BcQueueNxtEl == NVM_BC_INVALID_QUEUE_EL)
   {
     NvM_BcQueueFrontEl.BcIdx = NVM_BC_INVALID_QUEUE_EL;
     NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_NOT_OK;
     NvM_BcQueueFrontEl.BcCurrentFctPtr = NULL_PTR;
     NvM_BcQueueBackEl = NVM_BC_INVALID_QUEUE_EL;
   }
   else
   {
     NvM_BcQueueFrontEl.BcIdx = NvM_BcQueueData[BcIdx].BcQueueNxtEl;
     NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_StartBlkCheck;
     NvM_BcQueueData[NvM_BcQueueData[BcIdx].BcQueueNxtEl].BcQueuePrvEl = NVM_BC_INVALID_QUEUE_EL;
     NvM_BcQueueData[BcIdx].BcQueueNxtEl = NVM_BC_INVALID_QUEUE_EL;
   }
  }
  /*Check if the element that has to be removed from the queue is the last one*/
  else if (BcIdx == NvM_BcQueueBackEl)
  {
    NvM_BcQueueBackEl = NvM_BcQueueData[BcIdx].BcQueuePrvEl;
    NvM_BcQueueData[BcIdx].BcQueuePrvEl = NVM_BC_INVALID_QUEUE_EL;
    NvM_BcQueueData[NvM_BcQueueBackEl].BcQueueNxtEl = NVM_BC_INVALID_QUEUE_EL;
  }
  else if (NvM_BcQueueData[BcIdx].BcQueueNxtEl != NVM_BC_INVALID_QUEUE_EL)
  {
    NvM_BcQueueData[NvM_BcQueueData[BcIdx].BcQueuePrvEl].BcQueueNxtEl = NvM_BcQueueData[BcIdx].BcQueueNxtEl;
    NvM_BcQueueData[NvM_BcQueueData[BcIdx].BcQueueNxtEl].BcQueuePrvEl = NvM_BcQueueData[BcIdx].BcQueuePrvEl;
    NvM_BcQueueData[BcIdx].BcQueueNxtEl = NVM_BC_INVALID_QUEUE_EL;
    NvM_BcQueueData[BcIdx].BcQueuePrvEl = NVM_BC_INVALID_QUEUE_EL;
  }
  else
  {
    /*Nothing to do, element not in queue*/
  }
}

/*===================================NvM_BcInsertElInQueue=========================*/
STATIC FUNC(void,NVM_CODE) NvM_BcInsertElInQueue(uintBC BcIdx)
{
  NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_IN_QUEUE;
  /*Check if there are no elements in queue and if Yes insert the element in the Front Queue*/
  if(NvM_BcQueueFrontEl.BcIdx == NVM_BC_INVALID_QUEUE_EL)
  {
    NvM_BcQueueFrontEl.BcIdx = BcIdx;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_StartBlkCheck;
    NvM_BcQueueBackEl = BcIdx;
  }
  else
  {
    if(BcIdx != NvM_BcQueueFrontEl.BcIdx)
    {
      NvM_BcQueueData[NvM_BcQueueBackEl].BcQueueNxtEl = BcIdx;
      NvM_BcQueueData[BcIdx].BcQueuePrvEl = NvM_BcQueueBackEl;
      NvM_BcQueueBackEl = BcIdx;
    }
    else
    {
      /*Cannot Push, already in queue*/
    }
  }
}

/*==============================EXTERNAL FUNCTIONS============================*/
/*==============================NvM_BcProcessQueue============================*/
FUNC(void,NVM_CODE) NvM_BcProcessQueue(void)
{
  uintBC BcIdx;
  /*Process each BC element configured*/
  /* Deviation TASKING-1 */
  for(BcIdx =0; BcIdx < NVM_BC_NR_OF_BLOCKS; BcIdx++)
  {
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    /*Check if the element state has changed*/
    if(NvM_BcAdminData[BcIdx].BcElState == NVM_BC_ON_EDGE)
    {
      NvM_BcAdminData[BcIdx].BcElState = NVM_BC_ON;
      NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_ENABLED;
    }
    else if (NvM_BcAdminData[BcIdx].BcElState == NVM_BC_OFF_EDGE)
    {
      NvM_BcAdminData[BcIdx].BcElState = NVM_BC_OFF;
      NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_DISABLED;
    }
    else
    {
      /*empty else, nothing changed in the state of the BC element*/
    }
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    /* Check the BC element status */
    switch(NvM_BcAdminData[BcIdx].BcElStatus)
    {
      case NVM_BC_STATUS_EL_NOT_IN_QUEUE:
      {
        /* If element is not yet in queue process the delay counters*/
        NvM_BcAdminData[BcIdx].BcExecCounter--;
        if(NvM_BcAdminData[BcIdx].BcExecCounter == NVM_BC_DISABLED_EXEC_COUNTER)
        {
          /*when delay counter expires insert element in queue*/
          NvM_BcInsertElInQueue(BcIdx);
        }
        break;
      }
      case NVM_BC_STATUS_EL_IN_QUEUE:
      {
        /*nothing to do, empty case*/
        break;
      }
      case NVM_BC_STATUS_EL_ENABLED:
      {
        /*After a BC element is enabled check if it needs to be inserted in queue immediately*/
        if(NvM_BcCfgData[BcIdx].BcDelayCounter == NVM_BC_DISABLED_EXEC_COUNTER)
        {
          NvM_BcInsertElInQueue(BcIdx);
        }
        else
        {
          NvM_BcAdminData[BcIdx].BcExecCounter = NvM_BcCfgData[BcIdx].BcDelayCounter;
          NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_NOT_IN_QUEUE;
        }
        break;
      }
      case NVM_BC_STATUS_EL_DISABLED:
      {
        /*After an element is disabled remove it from the queue.*/
        NvM_BcRemoveElFromQueue(BcIdx);
        NvM_BcAdminData[BcIdx].BcExecCounter = NVM_BC_DISABLED_EXEC_COUNTER;
        NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_OFF;
        break;
      }
      case NVM_BC_STATUS_EL_JOB_FINISHED:
      {
        /*After a BC element finishes the BC process remove it from queue*/
        NvM_BcRemoveElFromQueue(BcIdx);
        /*If delay counter is configured for this block it shall be restarted otherwise turn off*/
        if(NvM_BcCfgData[BcIdx].BcDelayCounter == NVM_BC_DISABLED_EXEC_COUNTER)
        {
          NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_OFF;
        }
        else
        {
          NvM_BcAdminData[BcIdx].BcExecCounter = NvM_BcCfgData[BcIdx].BcDelayCounter;
          NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_NOT_IN_QUEUE;
        }
        break;
      }
      default :
      {
        break;
      }
    }
  }
}

/*==============================NvM_BcResetCurrentProcess============================*/
FUNC(void,NVM_CODE) NvM_BcResetCurrentProcess(void)
{
  /* !LINKSTO NvM.BlockCheck.Processing.BlockCheck,1 */
  /*If any BC is in process cancel the underlying lower layer operations and reset the BC SM*/
  if((NvM_BcQueueFrontEl.BcCurrentFctPtr != NvM_BcSm_StartBlkCheck) &&
     (NvM_BcQueueFrontEl.BcCurrentFctPtr != NULL_PTR))
  {
    NvM_BcCancelActiveRun();
    NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_NOT_OK;
    NvM_BcQueueFrontEl.BcCurrentFctPtr = &NvM_BcSm_StartBlkCheck;
  }
}

/*==============================NvM_BcMainFuncProcess============================*/
FUNC(void,NVM_CODE) NvM_BcMainFuncProcess(void)
{
  NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_NOT_OK;
  /*Call the SM function until the BcGlobalStatus is NVM_REQ_PENDING*/
  while( ( NvM_BcQueueFrontEl.BcCurrentFctPtr != NULL_PTR) &&
         ( NvM_BcQueueFrontEl.BcGlobalStatus != NVM_REQ_PENDING ) )
  {
    NvM_BcQueueFrontEl.BcCurrentFctPtr();
  }
}

/*===================================NvM_BcInit================================*/
FUNC(void,NVM_CODE) NvM_BcInit(void)
{
  uintBC BcIdx;
  NvM_BcQueueBackEl = NVM_BC_INVALID_QUEUE_EL;
  NvM_BcQueueFrontEl.BcIdx = NVM_BC_INVALID_QUEUE_EL;
  NvM_BcQueueFrontEl.BcGlobalStatus = NVM_REQ_NOT_OK;
  NvM_BcQueueFrontEl.DynStatus = 0U;
  NvM_BcQueueFrontEl.BcCurrentFctPtr = NULL_PTR;
  /*Initialize all variables with the configured BC elements configuration*/
  /* Deviation TASKING-2 */
  for(BcIdx = 0U; BcIdx < NVM_BC_NR_OF_BLOCKS; BcIdx++)
  {
    NvM_BcQueueData[BcIdx].BcQueuePrvEl = NVM_BC_INVALID_QUEUE_EL;
    NvM_BcQueueData[BcIdx].BcQueueNxtEl = NVM_BC_INVALID_QUEUE_EL;
    if(NVM_BC_AUTOSTART_EN(NvM_BcCfgData[BcIdx].BcBlkDescriptor) == TRUE)
    {
      if(NvM_BcCfgData[BcIdx].BcDelayCounter == NVM_BC_DISABLED_EXEC_COUNTER)
      {
        NvM_BcInsertElInQueue(BcIdx);
      }
      else
      {
        NvM_BcAdminData[BcIdx].BcExecCounter = NvM_BcCfgData[BcIdx].BcDelayCounter;
        NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_NOT_IN_QUEUE;
      }
      NvM_BcAdminData[BcIdx].BcElState = NVM_BC_ON;
    }
    else
    {
      NvM_BcAdminData[BcIdx].BcExecCounter = NVM_BC_DISABLED_EXEC_COUNTER;
      NvM_BcAdminData[BcIdx].BcElStatus = NVM_BC_STATUS_EL_OFF;
      NvM_BcAdminData[BcIdx].BcElState = NVM_BC_OFF;
    }

  }
}
/* !LINKSTO NvM.BlockCheck.Config.NvM_BlockCheck_Enable,1 */
/*===================================NvM_EnableBlockCheck================================*/
FUNC(Std_ReturnType,NVM_CODE) NvM_EnableBlockCheck
(
  NvM_BlockIdType BlockId,
  boolean BcEnable
)
{
  uint16 BlockIndex = NVM_GET_BLOCK_INDEX(BlockId);
  uintBC BcIdx;
  Std_ReturnType Result = E_NOT_OK; /* Result of this function */
#if ( NVM_DEV_ERROR_DETECT == STD_ON )
  /* Check if Block Id is within valid range */
  if ( BlockIndex == NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS )
  {
    /* !LINKSTO NvM.BlockCheck.DET.BlkId,1 */
    NVM_DET_REPORT_ERROR( NVM_ENABLE_BC_API_ID, NVM_E_PARAM_BLOCK_ID );
  }
  else if (NvM_BlockDescriptorTable[BlockIndex].bcBlockIdx == NVM_BC_INVALID_QUEUE_EL)
  {
    /* !LINKSTO NvM.BlockCheck.DET.WrongConfig,1 */
    NVM_DET_REPORT_ERROR( NVM_ENABLE_BC_API_ID, NVM_E_BLOCK_CONFIG );
  }
  else
#endif /* NVM_DEV_ERROR_DETECT == STD_ON */
  {
    BcIdx = NvM_BlockDescriptorTable[BlockIndex].bcBlockIdx;
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    if(BcEnable == TRUE)
    {
      /*Detect a rising edge in the current BC element state*/
      if((NvM_BcAdminData[BcIdx].BcElState == NVM_BC_OFF) ||
         (NvM_BcAdminData[BcIdx].BcElState == NVM_BC_OFF_EDGE))
      {
        NvM_BcAdminData[BcIdx].BcElState = NVM_BC_ON_EDGE;
      }
    }
    else
    {
      /*Detect a low edge in the current BC element state*/
      if(NvM_BcAdminData[BcIdx].BcElState == NVM_BC_ON)
      {
        NvM_BcAdminData[BcIdx].BcElState = NVM_BC_OFF_EDGE;
      }
    }
    SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
    Result = E_OK;
  }
  return Result;
}
/* !LINKSTO NvM.BlockCheck.EnableMechanism_Api,1 */
/*===================================NvM_DisableBlockCheckMechanism================================*/
FUNC(void,NVM_CODE) NvM_DisableBlockCheckMechanism
(
  boolean BcDisable
)
{
  /*Save the disable Flag*/
  if(BcDisable == TRUE)
  {
    NvM_BcGlobalDisableFlag = STD_ON;
  }
  else
  {
    NvM_BcGlobalDisableFlag = STD_OFF;
  }
}

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

#endif /*NVM_BLOCK_CHECK_ENABLED == STD_ON*/

/*==================[end of file]============================================*/
