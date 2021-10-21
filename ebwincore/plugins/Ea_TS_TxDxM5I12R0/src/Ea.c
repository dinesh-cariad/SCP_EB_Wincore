/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <Ea_Trace.h>
/* !LINKSTO EA113,1 */
#include <TSMem.h>                                        /* Memory Function*/
#include <Ea.h>                        /* public module API (own interface) */
#if (EA_POLLING_MODE == STD_OFF)
  #include <Ea_Cbk.h>           /* public module callback API (own interface) */
#endif
#include <Ea_Int.h>                                  /* internal module API */

#if (EA_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                                                 /* Det API */
#endif

/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef EA_VENDOR_ID /* configuration check */
#error EA_VENDOR_ID must be defined
#endif

#if (EA_VENDOR_ID != 1U) /* vendor check */
#error EA_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef EA_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error EA_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (EA_AR_RELEASE_MAJOR_VERSION != 4U)
#error EA_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef EA_AR_RELEASE_MINOR_VERSION /* configuration check */
#error EA_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (EA_AR_RELEASE_MINOR_VERSION != 0U )
#error EA_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef EA_AR_RELEASE_REVISION_VERSION /* configuration check */
#error EA_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (EA_AR_RELEASE_REVISION_VERSION != 3U )
#error EA_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef EA_SW_MAJOR_VERSION /* configuration check */
#error EA_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (EA_SW_MAJOR_VERSION != 5U)
#error EA_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef EA_SW_MINOR_VERSION /* configuration check */
#error EA_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (EA_SW_MINOR_VERSION < 12U)
#error EA_SW_MINOR_VERSION wrong (< 12U)
#endif

#ifndef EA_SW_PATCH_VERSION /* configuration check */
#error EA_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (EA_SW_PATCH_VERSION < 14U)
#error EA_SW_PATCH_VERSION wrong (< 14U)
#endif

/*--------------------------------------------------------------------------*/

#if (defined EA_IF_LENGTH)
#error EA_IF_LENGTH is already defined
#endif
/** \brief length of invalidation flag
 **
 ** Algorithm depends on a EA_IF_LENGTH of 1, the dependency lies in the
 ** resetting of the invalidation flag */
#define EA_IF_LENGTH 1U

#if (defined EA_STARTMARKER_POS)
#error EA_STARTMARKER_POS is already defined
#endif
/** \brief position of start marker **/
#define EA_STARTMARKER_POS 0U

#if (defined EA_NVM_JOB_ERROR_NOTIFICATION)
#error EA_NVM_JOB_ERROR_NOTIFICATION is already defined
#endif
/** \brief Job error notification
 **
 ** \param[in] jobresult the jobresult to be passed to the NvM */
#define EA_NVM_JOB_ERROR_NOTIFICATION(jobresult)        \
  do                                                    \
  {                                                     \
    /* error handling */                                \
    Ea_JobResult = (jobresult);                         \
    /* notify upper layer */                            \
    EA_JOB_ERROR_NTY_CBK();                             \
    /* set state back to idle */                        \
    Ea_MainState = EA_S_IDLE;                           \
    /* Set the module status back to IDLE */            \
    Ea_Status = MEMIF_IDLE;                             \
  } while (0)

#if (defined EA_NVM_JOB_END_NOTIFICATION)
#error EA_NVM_JOB_END_NOTIFICATION is already defined
#endif
/** \brief Job end notification */
#define EA_NVM_JOB_END_NOTIFICATION()           \
  do                                            \
  {                                             \
    /* error handling */                        \
    Ea_JobResult = MEMIF_JOB_OK;                \
    /* notify upper layer */                    \
    EA_JOB_END_NFY_CBK();                       \
    /* set state back to idle */                \
    Ea_MainState = EA_S_IDLE;                   \
    /* Set the module status back to IDLE */    \
    Ea_Status = MEMIF_IDLE;                     \
  } while (0)

#if (defined EA_SEARCH_INIT)
#error EA_SEARCH_INIT is already defined
#endif
/** \brief Initialise vle counter search */
#define EA_SEARCH_INIT() \
  do{\
    Ea_SearchCurrentCopy  = 1U; \
    Ea_SearchPreviousCopy = 0U; \
    Ea_SearchPreviousVLEC = 0U; \
  } while(0)

#if (defined EA_VLEC_LENGTH)
#error EA_VLEC_LENGTH is already defined
#endif
/** \brief length of vle counter
 **
 ** Algorithm depends on a VLEC_LENGTH of 1, the dependency lies in the
 ** calculation of the total number of virtual pages. If the VLEC length is
 ** more than one byte then the end of the user data will be overwritten! */
#define EA_VLEC_LENGTH 1U

#if (defined EA_DATA_POS)
#error EA_DATA_POS is already defined
#endif
/** \brief position of user data **/
#define EA_DATA_POS EA_VLEC_LENGTH


/* define various macros used for development error reporting,
 * if development error detection is enabled
 */
#if (EA_DEV_ERROR_DETECT == STD_ON)

#if (defined EA_DET_REPORT_ERROR)
#error EA_DET_REPORT_ERROR is already defined
#endif
/** \brief Macro for reporting an error to Det
 **
 ** \param[in] EA_SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module
 **/
#define EA_DET_REPORT_ERROR(EA_SID,ERROR_CODE) \
  (void) Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID, (EA_SID), (ERROR_CODE))


#endif /* if (EA_DEV_ERROR_DETECT == STD_ON) */

#if (defined EA_CALC_ADDRESS)
#error EA_CALC_ADDRESS is already defined
#endif
/** \brief calculate block start address
 **
 ** \param[in] offset block offset to derive the physical EEPROM address
 **                   needed for the underlying EEPROM driver
 ** \param[in] current_copy current copy of data in a block
 **
 ** \return address within a block */
/* !LINKSTO EA007,1 */
#define EA_CALC_ADDRESS(block_number, offset, current_copy) \
   (Eep_AddressType)(Ea_BlockConfig[(block_number)].StartAddress + \
   (((Eep_LengthType)(current_copy) - 1U) * Ea_BlockConfig[(block_number)].BlockPageSize) + \
   (offset))

#if (defined EA_ENDMARKER_POS)
#error EA_ENDMARKER_POS is already defined
#endif
/** \brief position of end marker **/
#define EA_ENDMARKER_POS(block_number) \
  (EA_TOTAL_BLOCK_SIZE(block_number) - EA_VIRTUAL_PAGE_SIZE-EA_VLEC_LENGTH)

#if (defined EA_INVALID_FLAG_POS)
#error EA_INVALID_FLAG_POS is already defined
#endif
/** \brief position of invalidation flag **/
#define EA_INVALID_FLAG_POS(block_number) \
  (EA_TOTAL_BLOCK_SIZE(block_number) - EA_VIRTUAL_PAGE_SIZE)

#if (defined EA_USER_BLOCK_SIZE)
#error EA_USER_BLOCK_SIZE is already defined
#endif
/** \brief user data size */
#define EA_USER_BLOCK_SIZE(block_number) \
  Ea_BlockConfig[(block_number)].UserDataSize

  #if (defined EA_TOTAL_BLOCK_SIZE)
#error EA_TOTAL_BLOCK_SIZE is already defined
#endif
/** \brief including invalidation flag */
#define EA_TOTAL_BLOCK_SIZE(block_number) \
  Ea_BlockConfig[(block_number)].BlockPageSize

#if (defined EA_GET_BLOCK_SIZE)
#error EA_GET_BLOCK_SIZE is already defined
#endif
/** \brief Calculates the real block size from the block page size */
#define EA_GET_BLOCK_SIZE(block_number) \
  ((Eep_LengthType)(Ea_BlockConfig[(block_number)].BlockPageSize \
    - EA_VIRTUAL_PAGE_SIZE))

#if (defined EA_USER_FIRST_BLOCK_SIZE)
#error EA_USER_FIRST_BLOCK_SIZE is already defined
#endif
/** \brief size of user data in first block */
#define EA_USER_FIRST_BLOCK_SIZE \
   (EA_VIRTUAL_PAGE_SIZE - EA_VLEC_LENGTH)

#if (defined EA_VLEC_NUM_OF_COPIES)
#error EA_VLEC_NUM_OF_COPIES is already defined
#endif
/** \brief current VLEC value */
#define EA_VLEC_NUM_OF_COPIES(block_number) \
  Ea_BlockConfig[(block_number)].VLECopiesNum

#if (defined EA_BLOCK_CONSISTENT_VALUE)
#error EA_BLOCK_CONSISTENT_VALUE is already defined
#endif
/** \brief Block consistency value */
#define EA_BLOCK_CONSISTENT_VALUE(block_number) \
  Ea_VLEBlockInfo[(block_number)].BlockConsistent

#if (defined EA_NEXT_VLEC_VALUE)
#error EA_NEXT_VLEC_VALUE is already defined
#endif
/** \brief next VLEC value
 **
 ** \return value of the next VLEC */
#define EA_NEXT_VLEC_VALUE(block_number) \
   ((Ea_VLEBlockInfo[(block_number)].CurrentVLECounterValue + 1U) !=     \
     EA_EEP_ERASE_VALUE ?                                                             \
  (uint8)(Ea_VLEBlockInfo[(block_number)].CurrentVLECounterValue + 1U) : \
    (uint8)(Ea_VLEBlockInfo[(block_number)].CurrentVLECounterValue + 2U))

#if (defined EA_NEXT_VLEC)
#error EA_NEXT_VLEC is already defined
#endif
/** \brief next VLEC
 **
 ** \param[in] value value of previous VLEC
 **
 ** \retval (value + 1U) value of next VLEC
 ** \retval (value + 2U) value of next VLEC */
#define EA_NEXT_VLEC(value) \
   ((((value) + 1U) != EA_EEP_ERASE_VALUE) ? ((value) + 1U) : ((value) + 2U))

#if (defined EA_CURRENT_COPY)
#error EA_CURRENT_COPY is already defined
#endif
/** \brief current copy */
#define EA_CURRENT_COPY(block_number) \
  Ea_VLEBlockInfo[(block_number)].CurrentCopy

#if (defined EA_NEXT_COPY)
#error EA_NEXT_COPY is already defined
#endif
/** \brief next copy
 **
 ** \returns next copy of data in a block */
#define EA_NEXT_COPY(block_number) \
   (uint8)((Ea_VLEBlockInfo[(block_number)].CurrentCopy ==     \
     Ea_BlockConfig[(block_number)].VLECopiesNum) ?     \
     1U :                                                        \
    (Ea_VLEBlockInfo[(block_number)].CurrentCopy + 1U))

#if (defined EA_BLOCK_NOT_IN_CACHE)
#error EA_BLOCK_NOT_IN_CACHE is already defined
#endif
/** \brief checks if block can be found in internal cache
 **
 ** \retval 0 if the current block is in the cache
 ** \retval 1 if the current block is not in the cache */
#define EA_BLOCK_NOT_IN_CACHE(block_number)                                         \
  (Ea_VLEBlockInfo[(block_number)].CurrentCopy == EA_VLE_CURRENT_COPY_UNKNOWN)

#if (defined EA_SET_CACHE)
#error EA_SET_CACHE is already defined
#endif
/** \brief store vlec value in internal cache
 **
 ** \param[in] vlec value of VLEC
 ** \param[in] copy value of current copy */
#define EA_SET_CACHE(block_number, vlec, copy) \
  do{\
    /* save actual vle counter value */ \
    Ea_VLEBlockInfo[(block_number)].CurrentVLECounterValue = (vlec); \
    /* save actual copy counter */ \
    Ea_VLEBlockInfo[(block_number)].CurrentCopy = (copy); \
  }while(0)

#if (defined EA_SET_BLOCK_CONSISTENCY)
#error EA_SET_BLOCK_CONSISTENCY is already defined
#endif
/** \brief store the block consistency to internal cache
 **
 ** \param[in] value value of Block Consistency */
#define EA_SET_BLOCK_CONSISTENCY(block_number, value) \
  do{\
    /* save the block consistency value */ \
    Ea_VLEBlockInfo[(block_number)].BlockConsistent = (value); \
  }while(0)

/*==================[type definitions]======================================*/

/** \brief prototype of state function pointer */
typedef P2FUNC(void, EA_CODE, Ea_StateFpType)(void);

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define EA_START_SEC_CODE
#include <Ea_MemMap.h>


/** \brief If the internal buffer is big enough, set state-machine
 **        to EA_S_WRITE_FINAL, else set state to EA_S_WRITE_DATA.
 **
 ** Write complete block to EEP or only first block depending on
 ** size of internal buffer.
 */
STATIC FUNC(void, EA_CODE) Ea_SfWriteStartMarker(void);

/** \brief Set state-machine to EA_S_WRITE_END_MARKER
 **
 ** Write data to EEP excluding last block with end marker
 */
STATIC FUNC(void, EA_CODE) Ea_SfWriteData(void);

/** \brief Set state-machine to EA_S_WRITE_FINAL
 **
 ** Write remaining data and end marker
 */
STATIC FUNC(void, EA_CODE) Ea_SfWriteEndMarker(void);

/** \brief Set state-machine to EA_S_IDLE
 **
 ** Update internal block cache.
 */
STATIC FUNC(void, EA_CODE) Ea_SfWriteFinal(void);

/** \brief If the internal buffer is big enough, set state-machine to
 **        EA_S_READ_CHECK_MARKER, else set state to EA_S_READ_END_MARKER
 **
 ** Read complete data or first block depending on size of internal buffer
 */
STATIC FUNC(void, EA_CODE) Ea_SfReadStartMarker(void);

/** \brief Set state-machine to EA_S_READ_DATA
 **
 ** Read Virtual Lifetime Extension Counter (VLEC) and Invalidation Flag
 */
STATIC FUNC(void, EA_CODE) Ea_SfReadEndMarker(void);

/** \brief Set state-machine to EA_S_FINAL
 **
 ** Check data consistency.if data is consistent, read user data.
 ** If data is inconsistent or incorrect call EA_JOB_ERROR_NOTIFICATION.
 */
STATIC FUNC(void, EA_CODE) Ea_SfReadData(void);

/** \brief Set state-machine to EA_S_IDLE
 **
 ** Check data consistency.If data is consistent, copy data from
 ** temporary buffer to user data buffer.
 */
STATIC FUNC(void, EA_CODE) Ea_SfReadCheckMarker(void);

/** \brief Set state-machine to EA_S_FINAL
 **
 ** Erase Immediate block
 */
STATIC FUNC(void, EA_CODE) Ea_SfEraseImmediate(void);

/** \brief Set state-machine to EA_S_ERASE_IMMEDIATE
 **
 ** Invalidate the block which is to be erased
 */
STATIC FUNC(void, EA_CODE) Ea_SfInvalidateErase(void);

/** \brief Set state-machine to EA_S_FINAL
 **
 ** Write invalid pattern to EEP
 */
STATIC FUNC(void, EA_CODE) Ea_SfInvalidate(void);

/** \brief Set Ea_State to Ea_State_Idle */
STATIC FUNC(void, EA_CODE) Ea_SfFinal(void);

/** \brief Set state-machine to EA_S_VLE_END_MARKER
 **
 ** Read first VLE Counter.
 */
STATIC FUNC(void, EA_CODE) Ea_SfVleStartMarker(void);

/** \brief Set state-machine to EA_S_VLE_FINAL
 **
 ** Read second VLE Counter
 */
STATIC FUNC(void, EA_CODE) Ea_SfVleEndMarker(void);

/** \brief Resume execution of the main state machine, if the
 **        current copy is found else look for the next one.
 **
 ** If the two VLE Counter values read previously are not continuous:
 ** the first is the current one.If the VLE Counter values are
 ** continuous over all copies: the last copy is the current one.
 ** When the current copy is found, resume execution of the main state machine.
 ** If the copy is not found, look for the next one (read VLEC).
 ** If read request accepted, no state change.
 */
STATIC FUNC(void, EA_CODE) Ea_SfVleFinal(void);

/** \brief Find internal block number
 **
 ** \param[in]  BlockNumber           Identifier of the block (from the configuration)
 ** \param[out] InternalBlockNumber   Internal block number for a block
 **
 ** \return Standard Return Code
 ** \return     E_OK                  BlockNumber found
 ** \return     E_NOT_OK              BlockNumber not found
 **
 */
STATIC FUNC(Std_ReturnType, EA_CODE) Ea_FindInternalBlockNumber
(
  uint16 BlockNumber,
  P2VAR(uint16, AUTOMATIC, EA_VAR) InternalBlockNumber
);

/** \brief Search available copy
 ** If block is inconsistent or erased then search available copy.
 ** Search ends if a valid block was already found or if there are no more
 ** copies of block. If current block is the first block to be read and if there
 ** are more copies of the block then search continue to find valid copy.
 **
 */
STATIC FUNC(void, EA_CODE) Ea_SearchAvailableCopy(void);

/** \brief Get the next copy in allowed range, if multiple copies available.
 **
 */
STATIC FUNC(void, EA_CODE) Ea_GetNextCopy(void);

#define EA_STOP_SEC_CODE
#include <Ea_MemMap.h>

/*==================[external constants]====================================*/

/*------------------[version constants definition]--------------------------*/

/*==================[internal constants]====================================*/

#define EA_START_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/** \brief Function pointer of main state machine
 **
 ** All state IDs EA_S_* besides EA_S_IDLE and EA_S_UNINIT shall be used as
 ** index into this array.
 **/

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * Identifier is used by Ea_MainFunction.
 *
 */

/* Deviation MISRAC2012-1 */
STATIC CONST(Ea_StateFpType, EA_CONST) Ea_MainStateFPList[EA_NUM_STATE_FUNCTIONS] =
{
  &Ea_SfWriteStartMarker, /**< \brief corresponds to EA_S_WRITE_START_MARKER */
  &Ea_SfWriteData,        /**< \brief corresponds to EA_S_WRITE_DATA */
  &Ea_SfWriteEndMarker,   /**< \brief corresponds to EA_S_WRITE_END_MARKER */
  &Ea_SfWriteFinal,       /**< \brief corresponds to EA_S_WRITE_FINAL */
  &Ea_SfReadStartMarker,  /**< \brief corresponds to EA_S_READ_START_MARKER */
  &Ea_SfReadEndMarker,    /**< \brief corresponds to EA_S_READ_END_MARKER */
  &Ea_SfReadData,         /**< \brief corresponds to EA_S_READ_DATA */
  &Ea_SfReadCheckMarker,  /**< \brief corresponds to EA_S_READ_CHECK_MARKER */
  &Ea_SfEraseImmediate,   /**< \brief corresponds to EA_S_ERASE_IMMEDIATE */
  &Ea_SfInvalidateErase,  /**< \brief corresponds to EA_S_INVALIDATE_ERASE */
  &Ea_SfInvalidate,       /**< \brief corresponds to EA_S_INVALIDATE */
  &Ea_SfFinal,            /**< \brief corresponds to EA_S_FINAL */
  &Ea_SfVleStartMarker,   /**< \brief corresponds to EA_S_VLE_START_MARKER */
  &Ea_SfVleEndMarker,     /**< \brief corresponds to EA_S_VLE_END_MARKER */
  &Ea_SfVleFinal          /**< \brief corresponds to EA_S_VLE_FINAL */
};

#define EA_STOP_SEC_CONST_UNSPECIFIED
#include <Ea_MemMap.h>

/*==================[external data]=========================================*/

#define EA_START_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

VAR(Ea_MainStateType, EA_VAR) Ea_MainState = EA_S_UNINIT;

VAR(MemIf_StatusType, EA_VAR) Ea_Status = MEMIF_UNINIT;

VAR(MemIf_JobResultType, EA_VAR) Ea_JobResult = MEMIF_JOB_OK;

VAR(Ea_VLEBlockInfoType, EA_VAR) Ea_VLEBlockInfo[EA_NUMBEROFBLOCKS] = {{0U, 0U, 0U}};

/** \brief Information for current job: pointer to user buffer during Read. */
STATIC P2VAR(uint8, EA_VAR, EA_APPL_DATA) Ea_UserDataReadBufferPtr = NULL_PTR;

/** \brief Information for current job: pointer to user buffer during Write. */
STATIC P2CONST(uint8, EA_VAR, EA_APPL_DATA) Ea_UserDataWriteBufferPtr = NULL_PTR;

#define EA_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

#define EA_START_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

VAR(uint8, EA_VAR) Ea_TempBuffer[EA_TEMPBUFFER_LENGTH] = {0U};

#define EA_STOP_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

/*==================[internal data]=========================================*/

#define EA_START_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

/** \brief Information for VLE copy search: previous copy. */
STATIC VAR(uint8, EA_VAR) Ea_SearchPreviousCopy = 0U;
/** \brief Information for VLE copy search: current copy. */
STATIC VAR(uint8, EA_VAR) Ea_SearchCurrentCopy = 0U;
/** \brief Information for VLE copy search: previous VLE Counter. */
STATIC VAR(uint8, EA_VAR) Ea_SearchPreviousVLEC = 0U;
/** \brief Information for VLE copy search: current VLE Counter. */
STATIC VAR(uint8, EA_VAR) Ea_SearchCurrentVLEC = 0U;

#define EA_STOP_SEC_VAR_INIT_8
#include <Ea_MemMap.h>

#define EA_START_SEC_VAR_INIT_16
#include <Ea_MemMap.h>

/** \brief Information for current job: block number. */
STATIC VAR(uint16, EA_VAR) Ea_InternalBlockNumber = 0U;

/** \brief Information for current job: offset in the block. */
STATIC VAR(uint16, EA_VAR) Ea_BlockOffset = 0U;

/** \brief Information for current job: length to read. */
STATIC VAR(uint16, EA_VAR) Ea_Length = 0U;

#define EA_STOP_SEC_VAR_INIT_16
#include <Ea_MemMap.h>

#define EA_START_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

STATIC VAR(Ea_MainStateType, EA_VAR) Ea_VleNextState = EA_S_IDLE;

#define EA_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Ea_MemMap.h>

/*==================[external function definitions]=========================*/

#define EA_START_SEC_CODE
#include <Ea_MemMap.h>

/*------------------[EA API functions]--------------------------------------*/
/*------------------[Ea_Init]-----------------------------------------------*/
FUNC(void, EA_CODE) Ea_Init(void)
{
  uint8_least i;

  DBG_EA_INIT_ENTRY();

  DBG_EA_JOBRESULT((Ea_JobResult),(MEMIF_JOB_OK));
  Ea_JobResult = MEMIF_JOB_OK;

  for (i=0U; i<EA_NUMBEROFBLOCKS; i++)
  {
    Ea_VLEBlockInfo[i].BlockConsistent = TRUE;
    Ea_VLEBlockInfo[i].CurrentCopy = EA_VLE_CURRENT_COPY_UNKNOWN;
    Ea_VLEBlockInfo[i].CurrentVLECounterValue = 0U;
  }

  DBG_EA_STATUS((Ea_Status),(MEMIF_IDLE));
  Ea_Status = MEMIF_IDLE;

  DBG_EA_MAINSTATE((Ea_MainState),(EA_S_IDLE));
  Ea_MainState = EA_S_IDLE;

  DBG_EA_INIT_EXIT();
}

/*------------------[Ea_SetMode]--------------------------------------------*/
#if (EA_SET_MODE_SUPPORTED == STD_ON)

FUNC(void, EA_CODE) Ea_SetMode
(
  MemIf_ModeType Mode
)
{
  DBG_EA_SETMODE_ENTRY(Mode);
#if (EA_DEV_ERROR_DETECT == STD_ON)

  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_SETMODE, EA_E_UNINIT);
  }
  else if ((Mode != MEMIF_MODE_SLOW) && (Mode != MEMIF_MODE_FAST))
  {
    EA_DET_REPORT_ERROR(EA_API_SETMODE, EA_E_INVALID_MODE);
  }
  else if(MEMIF_BUSY == Ea_Status)
  {
    /* Development error notification */
    EA_DET_REPORT_ERROR(EA_API_SETMODE, EA_E_BUSY);
  }
  else
#endif
  {
    if(MEMIF_IDLE == Ea_Status)
    {
      Eep_SetMode(Mode);
    }
  }

  DBG_EA_SETMODE_EXIT(Mode);
}
#endif

/*------------------[Ea_Read]-----------------------------------------------*/
FUNC(Std_ReturnType, EA_CODE) Ea_Read
(
  uint16 BlockNumber,
  uint16 BlockOffset,
  P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) DataBufferPtr,
  uint16 Length
)
{
  Std_ReturnType retValue = E_NOT_OK;

  DBG_EA_READ_ENTRY(BlockNumber,BlockOffset,DataBufferPtr,Length);

#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_READ, EA_E_UNINIT);
    retValue = E_NOT_OK;
  }
  else if(NULL_PTR == DataBufferPtr)
  {
    EA_DET_REPORT_ERROR(EA_API_READ, EA_E_INVALID_DATA_PTR);
    retValue = E_NOT_OK;
  }
  else if (MEMIF_BUSY == Ea_Status)
  {
    EA_DET_REPORT_ERROR(EA_API_READ, EA_E_BUSY);
    retValue = E_NOT_OK;
  }
  else
#endif
  {
    /* Refuse new job if another one is ongoing */
    if (EA_S_IDLE == Ea_MainState)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
      if (E_OK != Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber))
      {
        EA_DET_REPORT_ERROR(EA_API_READ, EA_E_INVALID_BLOCK_NO);
        retValue = E_NOT_OK;
      }
      else
#else
      (void) Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber);
#endif
      {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        if (BlockOffset >= Ea_BlockConfig[Ea_InternalBlockNumber].UserDataSize)
        {
          EA_DET_REPORT_ERROR(EA_API_READ, EA_E_INVALID_BLOCK_OFS);
          retValue = E_NOT_OK;
        }
        else if (Length > (Ea_BlockConfig[Ea_InternalBlockNumber].UserDataSize - BlockOffset))
        {
          EA_DET_REPORT_ERROR(EA_API_READ, EA_E_INVALID_BLOCK_LEN);
          retValue = E_NOT_OK;
        }
        else
#endif
        {

          DBG_EA_JOBRESULT((Ea_JobResult),(MEMIF_JOB_PENDING));
          Ea_JobResult = MEMIF_JOB_PENDING;

          DBG_EA_STATUS((Ea_Status),(MEMIF_BUSY));
          Ea_Status = MEMIF_BUSY;
          Ea_BlockOffset       = BlockOffset;
          Ea_UserDataReadBufferPtr = DataBufferPtr;
          Ea_Length            = Length;

          if ( EA_BLOCK_NOT_IN_CACHE(Ea_InternalBlockNumber) )
          {
            /* Block not found in cache, read last VLE counter from EEP */
            EA_SEARCH_INIT();

            /* Continue after vle search on next state */
            Ea_VleNextState = EA_S_READ_START_MARKER;

            /* Next state: search for vle counter */

            DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
            Ea_MainState = EA_S_VLE_START_MARKER;
          }
          else
          {
            /* Next state: read the start marker */

            DBG_EA_MAINSTATE((Ea_MainState),(EA_S_READ_START_MARKER));
            Ea_MainState = EA_S_READ_START_MARKER;
          }
          retValue = E_OK;
        }
      }
    }
    else
    {
      retValue = E_NOT_OK;
    }
  }

  DBG_EA_READ_EXIT(retValue,BlockNumber,BlockOffset,DataBufferPtr,Length);
  return retValue;
}

/*------------------[Ea_Write]----------------------------------------------*/
FUNC(Std_ReturnType, EA_CODE) Ea_Write
(
  uint16 BlockNumber,
  P2CONST(uint8, AUTOMATIC, EA_APPL_DATA) DataBufferPtr
)
{
  Std_ReturnType retValue = E_NOT_OK;

  DBG_EA_WRITE_ENTRY(BlockNumber,DataBufferPtr);

#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_WRITE, EA_E_UNINIT);
    retValue = E_NOT_OK;
  }
  else if(NULL_PTR == DataBufferPtr)
  {
    EA_DET_REPORT_ERROR(EA_API_WRITE, EA_E_INVALID_DATA_PTR);
    retValue = E_NOT_OK;
  }
  else if (MEMIF_BUSY == Ea_Status)
  {
    EA_DET_REPORT_ERROR(EA_API_WRITE, EA_E_BUSY);
    retValue = E_NOT_OK;
  }
  else
#endif
  {
    /* Refuse new job if another one is ongoing */
    if (EA_S_IDLE == Ea_MainState)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
      if (E_OK != Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber))
      {
        EA_DET_REPORT_ERROR(EA_API_WRITE, EA_E_INVALID_BLOCK_NO);
        retValue = E_NOT_OK;
      }
      else
#else

      (void) Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber);
#endif
      {

        DBG_EA_JOBRESULT((Ea_JobResult),(MEMIF_JOB_PENDING));
        Ea_JobResult = MEMIF_JOB_PENDING;

        DBG_EA_STATUS((Ea_Status),(MEMIF_BUSY));
        Ea_Status = MEMIF_BUSY;
        Ea_UserDataWriteBufferPtr = DataBufferPtr;

        if ( EA_BLOCK_NOT_IN_CACHE(Ea_InternalBlockNumber) )
        {
          /* Block not found in Cache, read last VLE counter from EEP */
          EA_SEARCH_INIT();

          /* Continue after vle search on next state */
          Ea_VleNextState = EA_S_WRITE_START_MARKER;

          /* Search for vle counter */

          DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
          Ea_MainState = EA_S_VLE_START_MARKER;
        }
        else
        {
          /* Next state: write the start marker */

          DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_START_MARKER));
          Ea_MainState = EA_S_WRITE_START_MARKER;
        }
        retValue = E_OK;
      }
    }
    else
    {
      retValue = E_NOT_OK;
    }
  }

  DBG_EA_WRITE_EXIT(retValue,BlockNumber,DataBufferPtr);
  return retValue;
}

/*------------------[Ea_Cancel]---------------------------------------------*/
FUNC(void, EA_CODE) Ea_Cancel
(
  void
)
{
  DBG_EA_CANCEL_ENTRY();
#if (EA_DEV_ERROR_DETECT == STD_ON)

  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_CANCEL, EA_E_UNINIT);
  }
  /* Check the current status of the module is not MEMIF_BUSY */
  else if (MEMIF_BUSY != Ea_Status)
  {
    EA_DET_REPORT_ERROR(EA_API_CANCEL, EA_E_INVALID_CANCEL);
  }
  else
  {
#else
    if(MEMIF_BUSY == Ea_Status)
    {
#endif
      /* Cancel Eeprom driver */
      Eep_Cancel();
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_CANCELED);
    } /* The closing bracket for both "if(MEMIF_BUSY == Ea_Status)" and the above "else" */

  DBG_EA_CANCEL_EXIT();
}

/*------------------[Ea_GetStatus]------------------------------------------*/
FUNC(MemIf_StatusType, EA_CODE) Ea_GetStatus
(
  void
)
{
  MemIf_StatusType retValue = MEMIF_IDLE;

  DBG_EA_GETSTATUS_ENTRY();
  if (EA_S_UNINIT == Ea_MainState)
  {
#if (EA_DEV_ERROR_DETECT == STD_ON)
    EA_DET_REPORT_ERROR(EA_API_GETSTATUS, EA_E_UNINIT);
#endif
    retValue = MEMIF_UNINIT;
  }
  else if ( EA_S_IDLE != Ea_MainState )
  {
    retValue = MEMIF_BUSY;
  }
  else
  {
    /* do nothing */
  }
  /* Eep_GetStatus() is not called, Ea internal status is returned. */

  DBG_EA_GETSTATUS_EXIT(retValue);
  return retValue;
}

/*------------------[Ea_GetJobResult]---------------------------------------*/
FUNC(MemIf_JobResultType, EA_CODE) Ea_GetJobResult(void)
{
  MemIf_JobResultType retValue = MEMIF_JOB_FAILED;

  DBG_EA_GETJOBRESULT_ENTRY();

#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_GETJOBRESULT, EA_E_UNINIT);
  }
  else
#endif
  {
    /* Eep_GetJobResult() is not called, Ea internal job result is returned */
    retValue = Ea_JobResult;
  }


  DBG_EA_GETJOBRESULT_EXIT(retValue);
  return retValue;
}

/*------------------[Ea_InvalidateBlock]------------------------------------*/
FUNC(Std_ReturnType, EA_CODE) Ea_InvalidateBlock
(
  uint16 BlockNumber
)
{
  Std_ReturnType retValue = E_NOT_OK;

  DBG_EA_INVALIDATEBLOCK_ENTRY(BlockNumber);

#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_INVALIDATEBLOCK, EA_E_UNINIT);
    retValue = E_NOT_OK;
  }
  else if (MEMIF_BUSY == Ea_Status)
  {
    EA_DET_REPORT_ERROR(EA_API_INVALIDATEBLOCK, EA_E_BUSY);
    retValue = E_NOT_OK;
  }
  else
#endif
  {
    /* Refuse new job if another one is ongoing */
    if (EA_S_IDLE == Ea_MainState)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
      if (E_OK != Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber))
      {
        EA_DET_REPORT_ERROR(EA_API_INVALIDATEBLOCK, EA_E_INVALID_BLOCK_NO);
        retValue = E_NOT_OK;
      }
      else
#else
      (void) Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber);
#endif
      {

        DBG_EA_JOBRESULT((Ea_JobResult),(MEMIF_JOB_PENDING));
        Ea_JobResult = MEMIF_JOB_PENDING;
        /* Set the Status of the module as busy*/

        DBG_EA_STATUS((Ea_Status),(MEMIF_BUSY));
        Ea_Status = MEMIF_BUSY;
        if ( EA_BLOCK_NOT_IN_CACHE(Ea_InternalBlockNumber) )
        {
          /* Block not found in Cache, read last VLE counter from EEP */
          EA_SEARCH_INIT();

          /* Continue after vle search on next state */
          Ea_VleNextState = EA_S_INVALIDATE;

          /* Search for vle counter */

          DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
          Ea_MainState = EA_S_VLE_START_MARKER;
        }
        else
        {
          /* Next state: invalidate the block */

          DBG_EA_MAINSTATE((Ea_MainState),(EA_S_INVALIDATE));
          Ea_MainState = EA_S_INVALIDATE;
        }
        retValue = E_OK;
      }
    }
    else
    {
      retValue = E_NOT_OK;
    }
  }

  DBG_EA_INVALIDATEBLOCK_EXIT(retValue,BlockNumber);
  return retValue;
}

/*------------------[Ea_GetVersionInfo]-------------------------------------*/
#if (EA_VERSION_INFO_API == STD_ON)

FUNC(void, EA_CODE) Ea_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr
)
{
  DBG_EA_GETVERSIONINFO_ENTRY(VersionInfoPtr);
#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    EA_DET_REPORT_ERROR(EA_API_GETVERSIONINFO, EA_E_INVALID_DATA_PTR);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = EA_VENDOR_ID;
    VersionInfoPtr->moduleID         = EA_MODULE_ID;
    VersionInfoPtr->sw_major_version = EA_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = EA_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = EA_SW_PATCH_VERSION;
  }

  DBG_EA_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

/*------------------[Ea_EraseImmediateBlock]--------------------------------*/
FUNC(Std_ReturnType, EA_CODE) Ea_EraseImmediateBlock
(
  uint16 BlockNumber
)
{
  Std_ReturnType retValue = E_NOT_OK;

  DBG_EA_ERASEIMMEDIATEBLOCK_ENTRY(BlockNumber);

#if (EA_DEV_ERROR_DETECT == STD_ON)
  if (EA_S_UNINIT == Ea_MainState)
  {
    EA_DET_REPORT_ERROR(EA_API_ERASEIMMEDIATEBLOCK, EA_E_UNINIT);
    retValue = E_NOT_OK;
  }
  else if (MEMIF_BUSY == Ea_Status)
  {
    EA_DET_REPORT_ERROR(EA_API_ERASEIMMEDIATEBLOCK, EA_E_BUSY);
    retValue = E_NOT_OK;
  }
  else
#endif
  {
    /* Refuse new job if another one is ongoing */
    if (EA_S_IDLE == Ea_MainState)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
      if (E_OK != Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber))
      {
        EA_DET_REPORT_ERROR(EA_API_ERASEIMMEDIATEBLOCK, EA_E_INVALID_BLOCK_NO);
        retValue = E_NOT_OK;
      }
      else
#else
      (void) Ea_FindInternalBlockNumber(BlockNumber, &Ea_InternalBlockNumber);
#endif
      {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        if (!Ea_BlockConfig[Ea_InternalBlockNumber].ImmediateBlock)
        {
          EA_DET_REPORT_ERROR(EA_API_ERASEIMMEDIATEBLOCK, EA_E_INVALID_BLOCK_NO);
          retValue = E_NOT_OK;
        }
        else
#endif
        {

          DBG_EA_JOBRESULT((Ea_JobResult),(MEMIF_JOB_PENDING));
          Ea_JobResult = MEMIF_JOB_PENDING;

          DBG_EA_STATUS((Ea_Status),(MEMIF_BUSY));
          Ea_Status = MEMIF_BUSY;

          if ( EA_BLOCK_NOT_IN_CACHE(Ea_InternalBlockNumber) )
          {
            /* Block not found in Cache, read last VLE counter from EEP */
            EA_SEARCH_INIT();

            /* Continue after vle search on next state */
            Ea_VleNextState = EA_S_INVALIDATE_ERASE;

            /* Search for vle counter */

            DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
            Ea_MainState = EA_S_VLE_START_MARKER;
          }
          else
          {
            /* Next state: internal erase immediate */

            DBG_EA_MAINSTATE((Ea_MainState),(EA_S_INVALIDATE_ERASE));
            Ea_MainState = EA_S_INVALIDATE_ERASE;
          }
          retValue = E_OK;
        }
      }
    }
    else
    {
      retValue = E_NOT_OK;
    }
  }

  DBG_EA_ERASEIMMEDIATEBLOCK_EXIT(retValue,BlockNumber);
  return retValue;
}

/*------------------[EA callback API functions]-----------------------------*/
#if (EA_POLLING_MODE == STD_OFF)
/*------------------[Ea_JobEndNotification]---------------------------------*/
FUNC(void, EA_CODE) Ea_JobEndNotification(void)
{
  DBG_EA_JOBENDNOTIFICATION_ENTRY();

  DBG_EA_JOBENDNOTIFICATION_EXIT();
}

/*------------------[Ea_JobErrorNotification]-------------------------------*/
FUNC(void, EA_CODE) Ea_JobErrorNotification(void)
{
  DBG_EA_JOBERRORNOTIFICATION_ENTRY();

  DBG_EA_JOBERRORNOTIFICATION_EXIT();
}
#endif /* #if EA_POLLING_MODE == STD_OFF */

/*------------------[EA scheduled API functions]----------------------------*/
/*------------------[Ea_MainFunction]---------------------------------------*/
FUNC(void, EA_CODE) Ea_MainFunction
(
  void
)
{

  DBG_EA_MAINFUNCTION_ENTRY();
  if ((Ea_MainState != EA_S_UNINIT) && (Ea_MainState != EA_S_IDLE))
  {
    Ea_MainStateFPList[Ea_MainState](); /* call state processing function */
  }

  DBG_EA_MAINFUNCTION_EXIT();
}

/*==================[internal function definitions]=========================*/
/*------------------- State Functions --------------------------------------*/
/*------------------[Ea_SfWriteStartMarker]---------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfWriteStartMarker
(
  void
)
{
  Std_ReturnType retValue = E_NOT_OK;
  uint8 nextVLEValue = EA_NEXT_VLEC_VALUE(Ea_InternalBlockNumber);

  /* Clear internal buffer */
  TS_MemSet(Ea_TempBuffer, EA_EEP_ERASE_VALUE, EA_TEMPBUFFER_LENGTH);

  /* Internal cache big enough? */
  if ( EA_TOTAL_BLOCK_SIZE(Ea_InternalBlockNumber) <= EA_TEMPBUFFER_LENGTH )
  {
    /* Internal temporary buffer is big enough, copy data and write it at once */
    TS_MemCpy(&Ea_TempBuffer[EA_STARTMARKER_POS], &nextVLEValue, EA_VLEC_LENGTH);
    TS_MemCpy(&Ea_TempBuffer[EA_DATA_POS], Ea_UserDataWriteBufferPtr, EA_USER_BLOCK_SIZE(Ea_InternalBlockNumber));
    TS_MemCpy(&Ea_TempBuffer[EA_ENDMARKER_POS(Ea_InternalBlockNumber)], &nextVLEValue, EA_VLEC_LENGTH);

    /* Reset invalidation flag */
    Ea_TempBuffer[EA_INVALID_FLAG_POS(Ea_InternalBlockNumber)] = EA_EEP_ERASE_VALUE;

    /* Call EEP to write block */
    retValue = Eep_Write(
       EA_CALC_ADDRESS(Ea_InternalBlockNumber, 0U, EA_NEXT_COPY(Ea_InternalBlockNumber)),
      Ea_TempBuffer,
      EA_GET_BLOCK_SIZE(Ea_InternalBlockNumber) + EA_IF_LENGTH);

    /* Switch to last state */

    DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_FINAL));
    Ea_MainState = EA_S_WRITE_FINAL;
  }
  else
  {
    /* Internal temporary buffer is to small for writing in one cycle, split writing */

    /* First write VLEC and some data to align it according virtual page size  */
    TS_MemCpy(&Ea_TempBuffer[EA_STARTMARKER_POS], &nextVLEValue, EA_VLEC_LENGTH);
    TS_MemCpy(&Ea_TempBuffer[EA_DATA_POS], Ea_UserDataWriteBufferPtr, (uint16)EA_USER_FIRST_BLOCK_SIZE);

    /* Call EEP to write block */
    retValue = Eep_Write(
       EA_CALC_ADDRESS(Ea_InternalBlockNumber, 0U, EA_NEXT_COPY(Ea_InternalBlockNumber)),
      Ea_TempBuffer,
      EA_VIRTUAL_PAGE_SIZE);

    /* If the remaining size for the second block is smaller than one virtual page,
     * skip to last state and write vle counter and invalidation flag.
     */
    if ((EA_USER_BLOCK_SIZE(Ea_InternalBlockNumber)-EA_USER_FIRST_BLOCK_SIZE) < EA_VIRTUAL_PAGE_SIZE)
    {
      /* Switch to last state */

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_END_MARKER));
      Ea_MainState = EA_S_WRITE_END_MARKER;
    }
    else
    {
      /* Switch to next state */

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_DATA));
      Ea_MainState = EA_S_WRITE_DATA;
    }
  }

  if (E_OK != retValue)
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
  }
  else
  {
    EA_SET_BLOCK_CONSISTENCY(Ea_InternalBlockNumber, FALSE);
  }
}

/*------------------[Ea_SfWriteData]----------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfWriteData
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if ( MEMIF_JOB_PENDING == jobResult )
  {
    /* wait */
  }
  else if ( MEMIF_JOB_OK == jobResult )
  {
    /* size of remaining data = user data - user data in first block, number
     * of data bytes to write is a multiple of whole virtual page size. */
    uint16 length = (EA_USER_BLOCK_SIZE(Ea_InternalBlockNumber) - EA_USER_FIRST_BLOCK_SIZE);
#if (EA_VIRTUAL_PAGE_SIZE > 1U)
    length = (length / EA_VIRTUAL_PAGE_SIZE) * EA_VIRTUAL_PAGE_SIZE;
#endif

    /* Call EEP to write block */
    /* The remaining data/VLEC to be written is handled in the next state */
    if (E_OK == Eep_Write(
           EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_VIRTUAL_PAGE_SIZE, EA_NEXT_COPY(Ea_InternalBlockNumber)),
        &Ea_UserDataWriteBufferPtr[EA_USER_FIRST_BLOCK_SIZE],
        (Eep_LengthType)length))
    {
      /* Switch to last state */

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_END_MARKER));
      Ea_MainState = EA_S_WRITE_END_MARKER;
    }
    else
    {
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfWriteEndMarker]-----------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfWriteEndMarker
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();
  uint8 nextVLEValue = EA_NEXT_VLEC_VALUE(Ea_InternalBlockNumber);

  if ( MEMIF_JOB_PENDING == jobResult )
  {
    /* wait */
  }
  else if ( MEMIF_JOB_OK == jobResult )
  {
    /* Clear internal buffer */
    TS_MemSet(Ea_TempBuffer, EA_EEP_ERASE_VALUE, EA_TEMPBUFFER_LENGTH);

#if (EA_VIRTUAL_PAGE_SIZE > 1U)
    {
      /* Calculate remaining user data */
      const uint16 Remaining
        = (uint16)(EA_USER_BLOCK_SIZE(Ea_InternalBlockNumber) - EA_USER_FIRST_BLOCK_SIZE)
        % EA_VIRTUAL_PAGE_SIZE;

      if (Remaining > 0U)
      {
        /* copy remaining user data */
        TS_MemCpy(
          &Ea_TempBuffer[0],
          &Ea_UserDataWriteBufferPtr[EA_USER_BLOCK_SIZE(Ea_InternalBlockNumber)-Remaining],
          Remaining);
      }
      /* else: no user data to copy */
    }
#endif

    /* Copy VLEC */
    TS_MemCpy(&Ea_TempBuffer[EA_VIRTUAL_PAGE_SIZE-EA_VLEC_LENGTH], &nextVLEValue, EA_VLEC_LENGTH);

    /* Call EEP to write end marker */
    if (E_OK == Eep_Write(
           EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_TOTAL_BLOCK_SIZE(Ea_InternalBlockNumber)-(2U*EA_VIRTUAL_PAGE_SIZE), EA_NEXT_COPY(Ea_InternalBlockNumber)),
        Ea_TempBuffer,
        EA_VIRTUAL_PAGE_SIZE + EA_IF_LENGTH))
    {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_WRITE_FINAL));
      Ea_MainState = EA_S_WRITE_FINAL;
    }
    else
    {
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfWriteFinal]---------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfWriteFinal
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    EA_SET_BLOCK_CONSISTENCY(Ea_InternalBlockNumber, TRUE);
    /* Update VLEC value */
    EA_SET_CACHE(Ea_InternalBlockNumber, EA_NEXT_VLEC_VALUE(Ea_InternalBlockNumber),EA_NEXT_COPY(Ea_InternalBlockNumber));
    /* job finished */
    EA_NVM_JOB_END_NOTIFICATION();
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfReadStartMarker]----------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfReadStartMarker
(
  void
)
{
  Std_ReturnType retValue = E_OK;

  if ((EA_VLE_CURRENT_COPY_UNKNOWN == EA_CURRENT_COPY(Ea_InternalBlockNumber)) ||
      (FALSE == EA_BLOCK_CONSISTENT_VALUE(Ea_InternalBlockNumber)))
  {
    /* Block is inconsistent */
    EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INCONSISTENT);
  }
  else if (EA_TOTAL_BLOCK_SIZE(Ea_InternalBlockNumber) <= EA_TEMPBUFFER_LENGTH)
  {
    /* Internal temporary buffer is big enough, read data at once */

    /* Call EEP to read block */
    retValue = Eep_Read(
      EA_CALC_ADDRESS(Ea_InternalBlockNumber, 0U, EA_CURRENT_COPY(Ea_InternalBlockNumber)),
      Ea_TempBuffer,
      EA_TOTAL_BLOCK_SIZE(Ea_InternalBlockNumber));

    /* Switch to last state */
    DBG_EA_MAINSTATE((Ea_MainState),(EA_S_READ_CHECK_MARKER));
    Ea_MainState = EA_S_READ_CHECK_MARKER;
  }
  else
  {
    /* Internal temporary buffer is to small for reading in one cycle, split reading */

    /* Call EEP to read VLE counter */
    retValue = Eep_Read(
      EA_CALC_ADDRESS(Ea_InternalBlockNumber, 0U, EA_CURRENT_COPY(Ea_InternalBlockNumber)),
      Ea_TempBuffer,
      EA_VLEC_LENGTH);

    /* Switch to next state */
    DBG_EA_MAINSTATE((Ea_MainState),(EA_S_READ_END_MARKER));
    Ea_MainState = EA_S_READ_END_MARKER;
  }

  if (E_OK != retValue)
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
  }
}

/*------------------[Ea_SfReadEndMarker]------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfReadEndMarker
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* Call EEP to read end marker and invalidation flag */
    if (E_OK == Eep_Read(
        EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_ENDMARKER_POS(Ea_InternalBlockNumber), EA_CURRENT_COPY(Ea_InternalBlockNumber)),
        &Ea_TempBuffer[EA_VLEC_LENGTH],
        EA_VLEC_LENGTH + EA_IF_LENGTH))
    {
      /* Next state: read data */

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_READ_DATA));
      Ea_MainState = EA_S_READ_DATA;
    }
    else
    {
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfReadData]-----------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfReadData
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* Note: at this point, start marker and end marker are always equal.
     * Otherwise, it will be found during the VLE search (Ea_SfVleFinal)
     * and will be marked as inconsistent in the cache. Therefore,
     * it will be reported as inconsistent in Ea_SfReadStartMarker itself
     * and here it is not required to check whether the markers are the same.
     */

    /* Check if block is not erased */
    if (Ea_TempBuffer[0] != EA_EEP_ERASE_VALUE)
    {
      /* Check if block is valid (was not invalidated or erased) */
      if (Ea_TempBuffer[2U*EA_VLEC_LENGTH] == EA_EEP_ERASE_VALUE)
      {
        /* Call EEP to read data */
        if (E_OK == Eep_Read(
            EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_DATA_POS + (uint32)Ea_BlockOffset, EA_CURRENT_COPY(Ea_InternalBlockNumber)),
            Ea_UserDataReadBufferPtr,
            (Eep_LengthType)Ea_Length))
        {
          /* Next state: finish operation */

          DBG_EA_MAINSTATE((Ea_MainState),(EA_S_FINAL));
          Ea_MainState = EA_S_FINAL;
        }
        else
        {
          EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
        }
      }
      /* Check if the block was invalidated */
      else if (Ea_TempBuffer[2U*EA_VLEC_LENGTH] == EA_INVALID_PATTERN)
      {
        EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INVALID);
      }
      /* The block is INCONSISTENT */
      else
      {
        EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INCONSISTENT);
      }
    }
    else
    {
      /* Block is erased therefore INCONSISTENT */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INCONSISTENT);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfReadCheckMarker]----------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfReadCheckMarker
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* Note: at this point, start marker and end marker are always equal.
     * Otherwise, it will be found during the VLE search (Ea_SfVleFinal)
     * and will be marked as inconsistent in the cache. Therefore,
     * it will be reported as inconsistent in Ea_SfReadStartMarker itself
     * and here it is not required to check whether the markers are the same.
     */

    /* Check if Block is not erased */
    if (Ea_TempBuffer[EA_STARTMARKER_POS] != EA_EEP_ERASE_VALUE)
    {
      if (Ea_TempBuffer[EA_INVALID_FLAG_POS(Ea_InternalBlockNumber)] == EA_EEP_ERASE_VALUE)
      {
        /* Block valid, copy data */
        TS_MemCpy(&Ea_UserDataReadBufferPtr[0], &Ea_TempBuffer[EA_DATA_POS+Ea_BlockOffset], Ea_Length);

        /* job finished */
        EA_NVM_JOB_END_NOTIFICATION();
      }
      else if (Ea_TempBuffer[EA_INVALID_FLAG_POS(Ea_InternalBlockNumber)] == EA_INVALID_PATTERN)
      {
        /* Block is INVALID */
        EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INVALID);
      }
      else
      {
        /* Block is INCONSISTENT */
        EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INCONSISTENT);
      }
    }
    else
    {
      /* Block is erased therefore INCONSISTENT */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_BLOCK_INCONSISTENT);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfEraseImmediate]-----------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfEraseImmediate
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* !LINKSTO Ea.EraseImmediate.Behaviour,1 */
    /* Erase user data, VLEC and marker */
    if (E_OK == Eep_Erase(EA_CALC_ADDRESS(Ea_InternalBlockNumber, 0U, EA_NEXT_COPY(Ea_InternalBlockNumber)), EA_TOTAL_BLOCK_SIZE(Ea_InternalBlockNumber)))
    {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_FINAL));
      Ea_MainState = EA_S_FINAL;
    }
    else
    {
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfInvalidateErase]---------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfInvalidateErase
(
  void
)
{
  uint8 pattern = EA_INCONSISTENT_PATTERN;

  if (EA_VLE_CURRENT_COPY_UNKNOWN == EA_CURRENT_COPY(Ea_InternalBlockNumber))
  {
    /* no old data to be invalidated so go direct to erase */

    DBG_EA_MAINSTATE((Ea_MainState),(EA_S_ERASE_IMMEDIATE));
    Ea_MainState = EA_S_ERASE_IMMEDIATE;
  }
  else
  {
    /* Copy invalid pattern to temporary buffer */
    TS_MemCpy(Ea_TempBuffer, &pattern, EA_IF_LENGTH);

    /* !LINKSTO Ea.EraseImmediate.Behaviour,1 */
    /* Write inconsistent pattern to EEP */
    if (E_OK == Eep_Write(
        EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_INVALID_FLAG_POS(Ea_InternalBlockNumber), EA_CURRENT_COPY(Ea_InternalBlockNumber)),
        Ea_TempBuffer,
        EA_IF_LENGTH))
    {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_ERASE_IMMEDIATE));
      Ea_MainState = EA_S_ERASE_IMMEDIATE;
    }
    else
    {
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
}

/*------------------[Ea_SfInvalidate]---------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfInvalidate
(
  void
)
{
  uint8 pattern = EA_INVALID_PATTERN;

  if (EA_VLE_CURRENT_COPY_UNKNOWN == EA_CURRENT_COPY(Ea_InternalBlockNumber))
  {
    /* No block was found in vle search */
    EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
  }
  else
  {
    /* Copy invalid pattern to temporary buffer */
    TS_MemCpy(Ea_TempBuffer, &pattern, EA_IF_LENGTH);

    /* Write invalid pattern to EEP */
    if (E_OK == Eep_Write(
        EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_INVALID_FLAG_POS(Ea_InternalBlockNumber), EA_CURRENT_COPY(Ea_InternalBlockNumber)),
        Ea_TempBuffer,
        EA_IF_LENGTH))
    {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_FINAL));
      Ea_MainState = EA_S_FINAL;
    }
    else
    {
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
}

/*------------------[Ea_SfFinal]--------------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfFinal
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* job finished */
    EA_NVM_JOB_END_NOTIFICATION();
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfVleStartMarker]-----------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfVleStartMarker
(
  void
)
{
   /* Read next possible vle counter value */
   if (E_OK == Eep_Read(
       EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_STARTMARKER_POS, Ea_SearchCurrentCopy),
       &Ea_SearchCurrentVLEC,
       EA_VLEC_LENGTH))
   {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_END_MARKER));
      Ea_MainState = EA_S_VLE_END_MARKER;
   }
   else
   {
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
   }
}

/*------------------[Ea_SfVleEndMarker]-------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfVleEndMarker
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if ( MEMIF_JOB_PENDING == jobResult )
  {
    /* wait */
  }
  else if ( MEMIF_JOB_OK == jobResult )
  {
    /* Read only vlec counter */
    if ( E_OK == Eep_Read(
        EA_CALC_ADDRESS(Ea_InternalBlockNumber, EA_ENDMARKER_POS(Ea_InternalBlockNumber), Ea_SearchCurrentCopy),
        Ea_TempBuffer,
        EA_VLEC_LENGTH))
    {

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_FINAL));
      Ea_MainState = EA_S_VLE_FINAL;
    }
    else
    {
      /* call JobErrorNotification callback and set job result */
      EA_NVM_JOB_ERROR_NOTIFICATION(MEMIF_JOB_FAILED);
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*------------------[Ea_SfVleFinal]-----------------------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SfVleFinal
(
  void
)
{
  MemIf_JobResultType jobResult = Eep_GetJobResult();

  if (MEMIF_JOB_PENDING == jobResult)
  {
    /* wait */
  }
  else if (MEMIF_JOB_OK == jobResult)
  {
    /* Start Marker and End marker are same */
    if (Ea_SearchCurrentVLEC == Ea_TempBuffer[0])
    {
      /* block copy is not erased? */
      if (Ea_SearchCurrentVLEC != EA_EEP_ERASE_VALUE)
      {
        /* have we read the next following block with consecutive VLEC? */
        if (EA_NEXT_VLEC(Ea_SearchPreviousVLEC) == Ea_SearchCurrentVLEC)
        {
          Ea_GetNextCopy();
        }
        else /* jump in VLEC of consecutive blocks detected OR
                VLEC in first copy > 1 due to single copy block or wraparound in writing multiple copies */
        {
          /* Valid block found before? */
          if (Ea_SearchPreviousCopy > 0U)
          {
            /* yes: we have read at least 2 blocks and saw the VLEC jump.  end
             * reached, return previous copy and vlec */
            EA_SET_CACHE(Ea_InternalBlockNumber, Ea_SearchPreviousVLEC, Ea_SearchPreviousCopy);

            DBG_EA_MAINSTATE((Ea_MainState),(Ea_VleNextState));
            Ea_MainState = Ea_VleNextState;
          }
          else  /* VLEC in first copy > 1 */
          {
            Ea_GetNextCopy();
          }
        }
      }
      else /* erased copy has been read */
      {
        /* search a valid copy */
        Ea_SearchAvailableCopy();
      }
    }
    else
    /* Start Marker and End Marker not same, block inconsistent */
    {
      EA_SET_BLOCK_CONSISTENCY(Ea_InternalBlockNumber, FALSE);

      /* search a valid copy */
      Ea_SearchAvailableCopy();
    }
  }
  else
  {
    /* call JobErrorNotification callback and set job result */
    EA_NVM_JOB_ERROR_NOTIFICATION(jobResult);
  }
}

/*--------------------------- Helper Functions -----------------------------*/
/*------------------[Ea_FindIndernalBlockNumber]----------------------------*/
STATIC FUNC(Std_ReturnType, EA_CODE) Ea_FindInternalBlockNumber
(
  uint16 BlockNumber,
  P2VAR(uint16, AUTOMATIC, EA_VAR) InternalBlockNumber
)
{
  Std_ReturnType retValue = E_NOT_OK;
  boolean exit_loop  = FALSE;

  /* indices in the Ea_BlockNumberList array */
  uint16 current;
  uint16 min = 0U;
  uint16 max = (uint16)(EA_NUMBEROFBLOCKS - 1U);

  while ( (min <= max) && (!exit_loop) )
  {
    /* We use min + ((max - min) / 2) instead of (min + max) / 2
     * to get the same result without the risk of overflow. */
    current = min + (uint16)((max - min) / 2U);

    if ( Ea_BlockNumberList[current] == BlockNumber )
    {
      /* found */
      *InternalBlockNumber = current;
      retValue                  = E_OK;
      exit_loop            = TRUE;
    }
    else if ( Ea_BlockNumberList[current] > BlockNumber )
    {
      if ( 0U == current )
      {
        exit_loop = TRUE;       /* not found */
      }
      else
      {
        max = (uint16)(current - 1U); /* continue w/ new max */
      }
    }
    else
    {
      if ( current == (uint16)(EA_NUMBEROFBLOCKS - 1U) )
      {
        exit_loop = TRUE;       /* not found */
      }
      else
      {
        min = current + 1U;     /* continue w/ new min */
      }
    }
  }

  return retValue;
}

/*------------------[Ea_SearchAvailableCopy]----------------------------*/
STATIC FUNC(void, EA_CODE) Ea_SearchAvailableCopy
(
  void
)
{
  /* Valid block found before? */
  if (Ea_SearchPreviousCopy > 0U)
  {
    /* End reached, return previous copy and vlec */
    EA_SET_CACHE(Ea_InternalBlockNumber, Ea_SearchPreviousVLEC,Ea_SearchPreviousCopy);

    DBG_EA_MAINSTATE((Ea_MainState),(Ea_VleNextState));
    Ea_MainState = Ea_VleNextState;
  }
  else
  {
    /* If there are multiple copies or block and current block is
     * the first block to be read */
    if ((EA_VLEC_NUM_OF_COPIES(Ea_InternalBlockNumber) > 1U) && (Ea_SearchCurrentCopy == 1U))
    {
      /* check NUM_OF_COPIES for possible data i.e. possible CurrentCopy*/
      Ea_SearchCurrentCopy = EA_VLEC_NUM_OF_COPIES(Ea_InternalBlockNumber);

      /* Trigger next read */

      DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
      Ea_MainState = EA_S_VLE_START_MARKER;
    }
    else
    {

      DBG_EA_MAINSTATE((Ea_MainState),(Ea_VleNextState));
      Ea_MainState = Ea_VleNextState;
    }
  }
}

/*------------------[Ea_GetNextCopy]----------------------------*/
STATIC FUNC(void, EA_CODE) Ea_GetNextCopy
(
  void
)
{
  /* check if we can try the next block without reading beyond the
   * allowed range i.e. we have multiple copies */
  if (Ea_SearchCurrentCopy < EA_VLEC_NUM_OF_COPIES(Ea_InternalBlockNumber))
  {
    /* yes: try next block */
    Ea_SearchPreviousVLEC = Ea_SearchCurrentVLEC;
    Ea_SearchPreviousCopy = Ea_SearchCurrentCopy;

    Ea_SearchCurrentCopy++;

    /* Trigger next read */

    DBG_EA_MAINSTATE((Ea_MainState),(EA_S_VLE_START_MARKER));
    Ea_MainState = EA_S_VLE_START_MARKER;
  }
  else /* no: single copy or first copy is inconsistent,
        * return current copy and VLEC */
  {
    EA_SET_CACHE(Ea_InternalBlockNumber, Ea_SearchCurrentVLEC,Ea_SearchCurrentCopy);

    DBG_EA_MAINSTATE((Ea_MainState),(Ea_VleNextState));
    Ea_MainState = Ea_VleNextState;
  }
}

#define EA_STOP_SEC_CODE
#include <Ea_MemMap.h>
/*==================[end of file]===========================================*/
