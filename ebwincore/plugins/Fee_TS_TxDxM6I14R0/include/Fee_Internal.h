/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2017 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* This file contains the prototypes of internal functions used by Fee APIs from AUTOSAR_SWS_FlashEEPROMEmulation.pdf
 * and internal definitions and declarations */

#ifndef FEE_INTERNAL_H
#define FEE_INTERNAL_H

/*==================[inclusions]============================================*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <Fee_Version.h>
#include <Fee_Internal_Types.h>        /* Flash driver header, includes also Std_Types and Fls types */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif
/*==================[macros]================================================*/
/*------------------------[AUTOSAR DET error codes]------------------------------*/
/* !LINKSTO FEE048,1 */
#if (defined FEE_E_UNINIT)
#error FEE_E_UNINIT already defined
#endif
/** \brief Development error indicating that the module has not been initialized */
#define FEE_E_UNINIT                    1U

#if (defined FEE_E_INVALID_BLOCK_NO)
#error FEE_E_INVALID_BLOCK_NO already defined
#endif
/** \brief Development error indicating that the addressed logical block is not
 ** configured or not configured as immediate data */
#define FEE_E_INVALID_BLOCK_NO          2U

#if (defined FEE_E_INVALID_BLOCK_OFS)
#error FEE_E_INVALID_BLOCK_OFS already defined
#endif
/** \brief Development error indicating that the block offset is invalid */
#define FEE_E_INVALID_BLOCK_OFS         3U

#if (defined FEE_E_INVALID_DATA_PTR)
#error FEE_E_INVALID_DATA_PTR already defined
#endif
/** \brief Development error indicating that a null pointer is passed as a parameter */
#define FEE_E_INVALID_DATA_PTR          4U

#if (defined FEE_E_INVALID_BLOCK_LEN)
#error FEE_E_INVALID_BLOCK_LEN already defined
#endif
/** \brief Development error indicating that the Block length is invalid */
#define FEE_E_INVALID_BLOCK_LEN         5U

#if (defined FEE_E_BUSY)
#error FEE_E_BUSY already defined
#endif
/** \brief Development error indicating that Fee is busy */
#define FEE_E_BUSY         6U

#if (defined FEE_E_BUSY_INTERNAL)
#error FEE_E_BUSY_INTERNAL already defined
#endif
/** \brief Development error indicating that the requested mode is invalid */
#define FEE_E_BUSY_INTERNAL              7U

#if (defined FEE_E_INVALID_CANCEL)
#error FEE_E_INVALID_CANCEL already defined
#endif
/** \brief Development error indicating that the requested cancel pending job is invalid */
#define FEE_E_INVALID_CANCEL              8U

#if (defined FEE_E_INVALID_MODE)
#error FEE_E_INVALID_MODE already defined
#endif
/** \brief Development error indicating that the requested mode is invalid */
#define FEE_E_INVALID_MODE              9U

#if (defined FEE_E_FLS_REQUEST_REFUSED)
#error FEE_E_FLS_REQUEST_REFUSED already defined
#endif
/** \brief Development error indicating that the requested job was refused by the driver */
#define FEE_E_FLS_REQUEST_REFUSED              10U

#if (defined FEE_E_NOT_CFG_BLK_LOST)
#error FEE_E_NOT_CFG_BLK_LOST already defined
#endif
/** \brief Development error indicating that the requested job was refused by the driver */
#define FEE_E_NOT_CFG_BLK_LOST              11U

#if (defined FEE_E_PREEMPTION_OCCURED)
#error FEE_E_PREEMPTION_OCCURED already defined
#endif
/** \brief Development error indicating a asynchronous API preemption occured */
#define FEE_E_PREEMPTION_OCCURED              12U





/*------------------------[Defensive programming]----------------------------*/

#if (defined FEE_PRECONDITION_ASSERT)
#error FEE_PRECONDITION_ASSERT is already defined
#endif

#if (defined FEE_PRECONDITION_ASSERT_NO_EVAL)
#error FEE_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FEE_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))
#else
#define FEE_PRECONDITION_ASSERT(Condition, ApiId)
#define FEE_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FEE_POSTCONDITION_ASSERT)
#error FEE_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FEE_POSTCONDITION_ASSERT_NO_EVAL)
#error FEE_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FEE_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))
#else
#define FEE_POSTCONDITION_ASSERT(Condition, ApiId)
#define FEE_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FEE_INVARIANT_ASSERT)
#error FEE_INVARIANT_ASSERT is already defined
#endif

#if (defined FEE_INVARIANT_ASSERT_NO_EVAL)
#error FEE_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FEE_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FEE_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))
#else
#define FEE_INVARIANT_ASSERT(Condition, ApiId)
#define FEE_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FEE_STATIC_ASSERT)
# error FEE_STATIC_ASSERT is already defined
#endif
#if (FEE_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FEE_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FEE_STATIC_ASSERT(expr)
#endif

#if (defined FEE_UNREACHABLE_CODE_ASSERT)
#error FEE_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FEE_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FEE_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId))
#else
#define FEE_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FEE_INTERNAL_API_ID)
#error FEE_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FEE_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------------[Development error reporting]----------------------*/

#if (defined FEE_DET_REPORT_ERROR)
#error FEE_DET_REPORT_ERROR already defined
#endif

/* define various macros used for development error reporting,
 * if development error detection is enabled */
#if (FEE_DEV_ERROR_DETECT == STD_ON)
/** \brief Report to the Det
 **
 ** \param[in] ApiId the service ID of the API function
 ** \param[in] ErrorId the ErrorID to be reported */
#define FEE_DET_REPORT_ERROR(ApiId, ErrorId)\
  ((void)Det_ReportError(FEE_MODULE_ID, FEE_INSTANCE_ID, (ApiId), (ErrorId)))
#else
#define FEE_DET_REPORT_ERROR(ApiId, ErrorId)
#endif

/*------------------------[Design and implementation macros]----------------------*/
#if (defined FEE_SECTION_STATUS_ACTIVE)
#error FEE_SECTION_STATUS_ACTIVE already defined
#endif
/** \brief Value to indicate Active section */
#define FEE_SECTION_STATUS_ACTIVE             0xAAU

#if (defined FEE_SECTION_STATUS_ERASABLE)
#error FEE_SECTION_STATUS_ERASABLE already defined
#endif
/** \brief Value to indicate To Be Erased section */
#define FEE_SECTION_STATUS_ERASABLE              0xDDU

#if (defined FEE_WRITTEN_MARKER_PATTERN)
#error FEE_WRITTEN_MARKER_PATTERN already defined
#endif
/** \brief Value to indicate that a block was successfully written */
#define FEE_WRITTEN_MARKER_PATTERN                      0x96U

#if (defined FEE_CONSISTENCY_MARKER_PATTERN)
#error FEE_CONSISTENCY_MARKER_PATTERN already defined
#endif
/** \brief Value to indicate that a block was successfully written */
#define FEE_CONSISTENCY_MARKER_PATTERN                      0x77U

#if (defined FEE_ZERO_BLOCKADDRESS)
#error FEE_ZERO_BLOCKADDRESS already defined
#endif
/** \brief Value to indicate NULL address. Used for initialize cache. No Block can have this address*/
#define FEE_ZERO_BLOCKADDRESS         0x00000000U

#if (defined FEE_INCONSISTENT_BLOCKADDRESS)
#error FEE_INCONSISTENT_BLOCKADDRESS already defined
#endif
/** \brief Value to indicate address corresponding to an inconsistent block in cache.
 * This should be flash address type, as the macro is used to update the cache */
#define FEE_INCONSISTENT_BLOCKADDRESS(Section)         (FEE_SECTION_START_ADDRESS(Section) + 1U)

#if (defined FEE_INVALID_BLOCKADDRESS)
#error FEE_INVALID_BLOCKADDRESS already defined
#endif
/** \brief Value to indicate address corresponding to an invalid block in cache.
 * This value is used to update the cache if a block was invalidated in the active section,
 * while a section switch was not pending. The value signals Fee that the block management
 * information shall be copied during the next section switch.
 * This should be flash address type, as the macro is used to update the cache */
#define FEE_INVALID_BLOCKADDRESS(Section)              FEE_FOOTER_ADDRESS(Section)

#if (defined FEE_WRITTEN_MARKER_SIZE)
#error FEE_WRITTEN_MARKER_SIZE already defined
#endif
/** \brief The size of written marker */
#define FEE_WRITTEN_MARKER_SIZE              FEE_VIRTUAL_PAGE_SIZE

#if (defined FEE_BLOCKINFO_PLUS_WM_SIZE_AL)
#error FEE_BLOCKINFO_PLUS_WM_SIZE_AL already defined
#endif
/** \brief The size of written marker */
#define FEE_BLOCKINFO_PLUS_WM_SIZE_AL              (FEE_BLOCKINFO_ALIGNED_TOTAL_SIZE + FEE_WRITTEN_MARKER_SIZE )

/** \brief Macro to call Fls_GetJobResult API if available and
 ** update internal flash job result variable.
 **
 ** This macro is dummy in case polling mode is not supported,
 ** but in that case internal flash job result variable gets updated
 ** via call back functions. */
#if (FEE_POLLING_MODE == STD_ON)
#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/* !LINKSTO Fee.Dsn.Critical.ResumeFailed,1 */
#define FEE_GET_FLASH_JOB_RESULT()  Fee_Get_Critical_Result()
#else
#define FEE_GET_FLASH_JOB_RESULT()  (Fee_Gv.FeeFlashJobResult = Fls_GetJobResult())
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */
#else
#define FEE_GET_FLASH_JOB_RESULT()
#endif /* (FEE_POLLING_MODE == STD_ON) */

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
#define FEE_FOOTER_BUFFER_IDX_FOR_CRASH            ((Fee_State == FEE_INIT_READ_FOOTER) ? FEE_BLOCKINFO_PLUS_WM_SIZE_AL : 0U)
#else
#define FEE_FOOTER_BUFFER_IDX_FOR_CRASH            0U
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

#if (defined FEE_DELAY)
#error FEE_DELAY already defined
#endif
/** \brief Value of startup or switch delay */
#define FEE_DELAY                             10U

#if (defined FEE_INFINITE_DELAY)
#error FEE_INFINITE_DELAY already defined
#endif
/** \brief Value of startup infinite delay */
#define FEE_INFINITE_DELAY                    255U

#if (defined FEE_WRITE_COPY_STATUS_LENGTH)
#error FEE_WRITE_COPY_STATUS_LENGTH already defined
#endif
/** \brief Number of bytes used for block info copy/write field */
#define FEE_WRITE_COPY_STATUS_LENGTH          1U

#if (defined FEE_CHECKSUM_SIZE)
#error FEE_CHECKSUM_SIZE already defined
#endif
/** \brief Number of bytes used for block info checksum */
#define FEE_CHECKSUM_SIZE                     1U

#if (defined FEE_BLOCKDATAADDRESS_SIZE)
#error FEE_BLOCKDATAADDRESS_SIZE already defined
#endif
/** \brief Number of bytes used to store the address of the block data */
#define FEE_BLOCKDATAADDRESS_SIZE             4U

#if (defined FEE_CHECKSUM_NUMBER_OF_BYTES)
#error FEE_CHECKSUM_NUMBER_OF_BYTES already defined
#endif
/** \brief Number of bytes used for calculating the checksum */
#define FEE_CHECKSUM_NUMBER_OF_BYTES          (Fls_LengthType)((FEE_BLOCKINFO_SIZE) - (FEE_CHECKSUM_SIZE))

#if (defined FEE_BLOCKNUMBER_INDEX)
#error FEE_BLOCKNUMBER_INDEX already defined
#endif
/** \brief Index of block number information in blockinfo */
#define FEE_BLOCKNUMBER_INDEX                 0U

#if (defined FEE_BLOCKLENGTH_INDEX)
#error FEE_BLOCKLENGTH_INDEX already defined
#endif
/** \brief Index of block length information in blockinfo */
#define FEE_BLOCKLENGTH_INDEX                 2U

#if (defined FEE_BLOCKDATAADDRESS_INDEX)
#error FEE_BLOCKDATAADDRESS_INDEX already defined
#endif
/** \brief Index of the start address of the block data */
#define FEE_BLOCKDATAADDRESS_INDEX            4U

#if (defined FEE_WRITE_STATUS_INDEX)
#error FEE_WRITE_STATUS_INDEX already defined
#endif
/** \brief Index of copy/write status information in blockinfo */
#define FEE_WRITE_STATUS_INDEX                8U

#if (defined FEE_CHECKSUM_INDEX)
#error FEE_CHECKSUM_INDEX already defined
#endif
/** \brief Index of checksum information in blockinfo */
#define FEE_CHECKSUM_INDEX                    9U

#if (defined FEE_ECOUNTER_CRC_COMPUTE_SIZE)
#error FEE_ECOUNTER_CRC_COMPUTE_SIZE already defined
#endif
/** \brief Counter size CRC */
#define FEE_ECOUNTER_CRC_COMPUTE_SIZE           4U

#if (defined FEE_CID_CRC_COMPUTE_SIZE)
#error FEE_CID_CRC_COMPUTE_SIZE already defined
#endif
/** \brief ConfigId size + Section counter size */
#define FEE_CID_CRC_COMPUTE_SIZE           2U

#if (defined FEE_USER_JOB)
#error FEE_USER_JOB already defined
#endif
/** \brief Value to indicate that the block is present in
* flash as a result of write request */
#define FEE_USER_JOB                       0xBBU

#if (defined FEE_SWITCH_JOB)
#error FEE_SWITCH_JOB already defined
#endif
/** \brief Value to indicate that the block is copied
* from the inactive section */
#define FEE_SWITCH_JOB                      0xCCU

#if (defined FEE_INVALID_SIZE)
#error FEE_INVALID_SIZE already defined
#endif
/** \brief Value to indicate invalid block size */
#define FEE_INVALID_SIZE                      0U

#if (defined FEE_BLOCKINFO_SIZE)
#error FEE_BLOCKINFO_SIZE already defined
#endif
/** \brief Number of bytes in block info */
#define FEE_BLOCKINFO_SIZE                    10U

#if (defined FEE_INDEX_ZERO)
#error FEE_INDEX_ZERO already defined
#endif
/** \brief value 0x00 */
#define FEE_INDEX_ZERO                        0U

#if (defined FEE_CRC_START_VALUE)
#error FEE_CRC_START_VALUE already defined
#endif
/** \brief value for Crc to start calculation */
#define FEE_CRC_START_VALUE                   0U

#if (defined FEE_DATA_INFO_MARGIN)
#error FEE_DATA_INFO_MARGIN already defined
#endif
/** \brief Delimitation between block data and block info */
#define FEE_DATA_INFO_MARGIN       FEE_MARGIN

#if (defined FEE_STATE_MAX_RETRIES)
#error FEE_STATE_MAX_RETRIES already defined
#endif
/** \brief Number of retries for a state */
#define FEE_STATE_MAX_RETRIES                 1U

#if (defined FEE_FLASH_ERASE_COUNTER_MAX_VALUE)
#error FEE_FLASH_ERASE_COUNTER_MAX_VALUE already defined
#endif
/** \brief Maximum value of the flash erase counter.
 * The value is used to signal an invalid value of the counter */
#define FEE_FLASH_ERASE_COUNTER_MAX_VALUE   0xFFFFFFFFU

#if (defined FEE_READ_BYTE)
#error FEE_READ_BYTE already defined
#endif
/** \brief Macro to read the byte from the uint8 buffer */
/* Deviation MISRA-1 */
#define FEE_READ_BYTE(Buffer, Index)                  \
  ((uint8)(Buffer)[(Index)])

#if (defined FEE_READ_WORD)
#error FEE_READ_WORD already defined
#endif
/** \brief Macro to read the word from the uint8 buffer */
/* Deviation MISRA-1 */
#define FEE_READ_WORD(Buffer, Index)                  \
  ((uint16)((uint16)((Buffer)[(Index)+1U])<<8U)|(uint16)((Buffer)[(Index)]))

#if (defined FEE_READ_DWORD)
#error FEE_READ_DWORD already defined
#endif
/** \brief Macro to read the double word from the uint8 buffer */
/* Deviation MISRA-1 */
#define FEE_READ_DWORD(Buffer, Index)                                               \
  ((uint32) ((uint32)((Buffer)[(Index)+3U])<<24U)|((uint32)((Buffer)[(Index)+2U])<<16U)| \
            ((uint32)((Buffer)[(Index)+1U])<<8U)|((uint32)((Buffer)[(Index)])))

#if (defined FEE_WRITE_BYTE)
#error FEE_WRITE_BYTE already defined
#endif
/** \brief Macro to write the byte to the uint8 buffer */
/* Deviation MISRA-1 <START>*/
#define FEE_WRITE_BYTE(Buffer, Index, Byte)                      \
  do {                                                           \
    (Buffer)[(Index)] = (uint8)(Byte);                             \
  } while(0)
/* Deviation MISRA-1 <STOP>*/

#if (defined FEE_WRITE_WORD)
#error FEE_WRITE_WORD already defined
#endif
/** \brief Macro to write the word to the uint8 buffer */
/* Deviation MISRA-1 <START>*/
#define FEE_WRITE_WORD(Buffer, Index, Word)                      \
  do {                                                           \
    (Buffer)[(Index)] =    (uint8)((uint16)(Word) & 0xFFU);        \
    (Buffer)[(Index)+1U] = (uint8)((uint16)(Word) >> 8U);          \
  } while(0)
/* Deviation MISRA-1 <STOP>*/

#if (defined FEE_WRITE_DWORD)
#error FEE_WRITE_DWORD already defined
#endif
/** \brief Macro to write the double word to the uint8 buffer */
/* Deviation MISRA-1 <START>*/
#define FEE_WRITE_DWORD(Buffer, Index, Word)                      \
  do {                                                            \
    (Buffer)[(Index)] =    (uint8)((uint32)(Word) & 0xFFU);         \
    (Buffer)[(Index)+1U] = (uint8)((uint32)(Word) >> 8U);           \
    (Buffer)[(Index)+2U] = (uint8)((uint32)(Word) >> 16U);          \
    (Buffer)[(Index)+3U] = (uint8)((uint32)(Word) >> 24U);          \
  } while(0)
/* Deviation MISRA-1 <STOP>*/

#if (defined FEE_HAS_ATLEAST_ONEBYTE_OF)
#error FEE_HAS_ATLEAST_ONEBYTE_OF already defined
#endif
/** \brief Check if the header field has at least one byte equal to given marker pattern */
#define FEE_HAS_ATLEAST_ONEBYTE_OF(MarkerField, Pattern) \
    (((uint8)(MarkerField)[0U] == (uint8)(Pattern)) || \
        ((uint8)(MarkerField)[(FEE_VIRTUAL_PAGE_SIZE - 1U)] == (uint8)(Pattern)))

#if (defined FEE_TRIGGER_TRANS)
#error FEE_TRIGGER_TRANS already defined
#endif
/** \brief Helper macro to trigger a state change and execute the entry function */
#define FEE_TRIGGER_TRANS(targetState)    \
  (Fee_NextState = (targetState))

#if (defined FEE_SECTION_START_ADDRESS)
#error FEE_SECTION_START_ADDRESS already defined
#endif
/** \brief Macro to find the section start address given the section number */
#define FEE_SECTION_START_ADDRESS(SecNo)     Fee_SectionCfg[(SecNo)].FeeSectionStartAddress

#if (defined FEE_SECTION_SIZE)
#error FEE_SECTION_SIZE already defined
#endif
/** \brief Macro to find the section size given the section number */
#define FEE_SECTION_SIZE(SecNo)     Fee_SectionCfg[(SecNo)].FeeSectionSize

#if (defined FEE_SECTION_END_ADDRESS)
#error FEE_SECTION_END_ADDRESS already defined
#endif
/** \brief Macro to find the section end address given the section number */
#define FEE_SECTION_END_ADDRESS(SecNo)        (Fee_SectionCfg[(SecNo)].FeeSectionStartAddress + Fee_SectionCfg[(SecNo)].FeeSectionSize)

#if (defined FEE_FOOTER_ADDRESS)
#error FEE_FOOTER_ADDRESS already defined
#endif
/** \brief Macro to find the address at which the footer is written given the section number */
#define FEE_FOOTER_ADDRESS(Section)  (FEE_SECTION_END_ADDRESS(Section) - FEE_HEADER_SIZE - FEE_CONSISTENCY_PATTERN_SIZE)

#if (defined FEE_DATA_ADDRESS)
#error FEE_DATA_ADDRESS already defined
#endif
/** \brief The address of the section at which the first data block is written */
#define FEE_DATA_ADDRESS(Section)             \
  (FEE_SECTION_START_ADDRESS(Section) + FEE_HEADER_SIZE + FEE_CONSISTENCY_PATTERN_SIZE)

#if (defined FEE_FIRST_BLOCK_INFO_ADDRESS)
#error FEE_FIRST_BLOCK_INFO_ADDRESS already defined
#endif
/** \brief The address at which the first block info is written */
#define FEE_FIRST_BLOCK_INFO_ADDRESS(Section) \
  (FEE_FOOTER_ADDRESS(Section) - FEE_BLOCKINFO_PLUS_WM_SIZE_AL)

#if (defined FEE_DECREMENT)
#error FEE_DECREMENT already defined
#endif
/** \brief Macro function to circularly decrement by 1 on one Byte */
#define FEE_DECREMENT(x) (((x) == (uint8)0) ? (uint8)255 : ((x) - (uint8)1))

#if (defined FEE_INCREMENT)
#error FEE_INCREMENT already defined
#endif
/** \brief Macro function to circularly increment by 1 on one Byte */
#define FEE_INCREMENT(x) (((x) == (uint8)255) ? (uint8)0 : ((x) + (uint8)1))

#if (defined FEE_INCREMENT_BY)
#error FEE_INCREMENT_BY already defined
#endif
/** \brief Macro function to circularly increment by 1 on one Byte */
#define FEE_INCREMENT_BY(x,i) (((x) == (uint8)255) ? ((i) - (uint8)1) : ((x) + (i)))

#if (defined FEE_BYTESUM)
#error FEE_BYTESUM already defined
#endif
/** \brief Macro function to circularly add two given numbers one Byte */
#define FEE_BYTESUM(x,y) ((((uint8)255 - (x)) < (y)) ? ((y) - ((uint8)255 - (x)) - (uint8)1) : ((x) + (y)))

#if (defined FEE_SECTIONDIF)
#error FEE_SECTIONDIF already defined
#endif
/** \brief Macro function to circularly subtract two given numbers one Byte */
#define FEE_SECTIONDIF(x,y) (uint8)(((x) < (y)) ? ((uint8)FEE_NUMBER_OF_SECTIONS - ((y) - (x))) : ((x) - (y)))

#if (defined FEE_SECTIONINCREMENT)
#error FEE_SECTIONINCREMENT already defined
#endif
/** \brief Macro function to circularly increment by 1 the section index */
#define FEE_SECTIONINCREMENT(x) (((x) == ((uint8)FEE_NUMBER_OF_SECTIONS - (uint8)1)) ? (uint8)0 : ((x) + 1))

#if (defined FEE_SECTIONDECREMENT)
#error FEE_SECTIONDECREMENT already defined
#endif
/** \brief Macro function to circularly decrement by 1 the section index */
#define FEE_SECTIONDECREMENT(x) (((x) == (uint8)0) ? ((uint8)FEE_NUMBER_OF_SECTIONS - (uint8)1) : ((x) - (uint8)1))

#if (defined FEE_SECTIONSUM)
#error FEE_SECTIONSUM already defined
#endif
/** \brief Macro function to circularly add two given numbers one Byte */
#define FEE_SECTIONSUM(x,y) (((((uint8)FEE_NUMBER_OF_SECTIONS -(uint8)1) - (x)) < (y)) ? ((y) - (((uint8)FEE_NUMBER_OF_SECTIONS - (uint8)1) - (x)) - (uint8)1) : ((x) + (y)))


#if (defined FEE_ALLIGNED_TO_VIRTUAL_PAGE)
#error FEE_ALLIGNED_TO_VIRTUAL_PAGE already defined
#endif
/* Get the given size aligned to the virtual page size */
#if (FEE_VIRTUAL_PAGE_SIZE == 1U)
#define FEE_ALLIGNED_TO_VIRTUAL_PAGE(Size)     (Fls_AddressType)(Size)
#else
#define FEE_ALLIGNED_TO_VIRTUAL_PAGE(Size)  \
    (Fls_AddressType)((Fls_AddressType)((FEE_VIRTUAL_PAGE_SIZE - (Fls_AddressType)((Fls_AddressType)(Size) % FEE_VIRTUAL_PAGE_SIZE)) % FEE_VIRTUAL_PAGE_SIZE) + (Fls_AddressType)(Size))
#endif

/* !LINKSTO Fee.Dsn.DynamicBlockLength.Switch,1 */
#if (FEE_DYNAMIC_BLOCK_LENGTH == STD_ON)
#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
#define FEE_COPY_BLOCK_SIZE()  (((Fee_Gv.FeeCopyBlockIndex) >= FEE_NO_OF_CONFIGURED_BLOCKS) ?  \
                                             (Fee_BlockCfgPtr->FeeBlockSize) : \
                                             (Fee_Gv.FeeOldSizes[Fee_Gv.FeeCopyBlockIndex]))
#define FEE_COPY_BLOCK_SIZE_AL() (((Fee_Gv.FeeCopyBlockIndex) >= FEE_NO_OF_CONFIGURED_BLOCKS) ?  \
                                             (Fee_BlockCfgPtr->FeeBlockAlignedSize) : \
                                             (FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeOldSizes[Fee_Gv.FeeCopyBlockIndex])))
#else
#define FEE_COPY_BLOCK_SIZE()  (Fee_Gv.FeeOldSizes[Fee_Gv.FeeCopyBlockIndex])
#define FEE_COPY_BLOCK_SIZE_AL()  (FEE_ALLIGNED_TO_VIRTUAL_PAGE(Fee_Gv.FeeOldSizes[Fee_Gv.FeeCopyBlockIndex]))
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */
#else
#define FEE_COPY_BLOCK_SIZE()  (Fee_BlockCfgPtr->FeeBlockSize)
#define FEE_COPY_BLOCK_SIZE_AL()  (Fee_BlockCfgPtr->FeeBlockAlignedSize)
#endif /* #if (FEE_DYNAMIC_BLOCK_LENGTH != STD_OFF) */

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
#define FEE_BLOCK_CFG(InternalBlockIndex)  (((InternalBlockIndex) >= FEE_NO_OF_CONFIGURED_BLOCKS) ? (&Fee_NotCfgBlocks[((InternalBlockIndex) - FEE_NO_OF_CONFIGURED_BLOCKS)]) : \
                                                                                                  (&Fee_BlockCfg[(InternalBlockIndex)]))
#else
#define FEE_BLOCK_CFG(InternalBlockIndex)  (&Fee_BlockCfg[(InternalBlockIndex)])
#endif /* #if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U) */

#if (defined FEE_USER_JOB_CHANGES_SECTION_MASK)
#error FEE_USER_JOB_CHANGES_SECTION_MASK already defined
#endif
/** \brief Macro used to extract the FeeUserJobChangesSection flag
 **        from bit 1 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: No user job changed section in the last cycle;
 **        Value 1: An user job had changed the section and, after preparing the next active section,
 **                 the job shall be performed immediately because the new active section has plenty of
 **                 empty space, and there is no point in computing the sizes again.
 **/
#define FEE_USER_JOB_CHANGES_SECTION_MASK                   0x0001U

#if (defined FEE_HIGHEST_INTERNAL_PRIORITY_MASK)
#error FEE_HIGHEST_INTERNAL_PRIORITY_MASK already defined
#endif
/** \brief Macro used to extract the FeeHighestInternalPriority flag
 **        from bit 2 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: Fee can process user jobs.
 **        Value 1: The fee can't process any user job because of internal activities that have a higher priority.
 **/
#define FEE_HIGHEST_INTERNAL_PRIORITY_MASK                  0x0002U

#if (defined FEE_SWITCH_RESTARTED_MASK)
#error FEE_SWITCH_RESTARTED_MASK already defined
#endif
/** \brief Macro used to extract the FeeSwitchRestarted flag
 **        from bit 3 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: No section switch restarted
 **        Value 1: Section switch was restarted.
 **/
#define FEE_SWITCH_RESTARTED_MASK                           0x0004U

#if (defined FEE_FLASH_FINAL_REFUSAL_MASK)
#error FEE_FLASH_FINAL_REFUSAL_MASK already defined
#endif
/** \brief Macro used to extract the FeeFlsRefusal flag
 **        from bit 4 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: Not yet set.
 **        Value 1: When all retries fail it will be set.
 **/
#define FEE_FLASH_FINAL_REFUSAL_MASK                        0x0008U

#if (defined FEE_SS_READ_FAIL_RETRY_MASK)
#error FEE_SS_READ_FAIL_RETRY_MASK already defined
#endif
/** \brief Macro used to extract the FeeReadFaiLRetry flag
 **        from bit 5 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: No read fail.
 **        Value 1: Read failed. A retry shall be done.
 **/
#define FEE_SS_READ_FAIL_RETRY_MASK                         0x0010U

#if (defined FEE_NOT_CFG_BLK_LOST_MASK)
#error FEE_NOT_CFG_BLK_LOST_MASK already defined
#endif
/** \brief Macro used to extract the FeeNotCfgBlkLost flag
 **        from bit 6 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: Not lost.
 **        Value 1: The current block can not be switched and it will be lost.
 **/
#define FEE_NOT_CFG_BLK_LOST_MASK                           0x0020U

#if (defined FEE_MAIN_FUNCTION_CALL_MASK)
#error FEE_MAIN_FUNCTION_CALL_MASK already defined
#endif
/** \brief Macro used to extract the FeeMainFunctionCall flag
 **        from bit 7 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: Fee main function run is finished;
 **        Value 1: Fee main function is currently running.
 **/
#define FEE_MAIN_FUNCTION_CALL_MASK                        0x0040U

#if (defined FEE_PREEMPTION_MASK)
#error FEE_PREEMPTION_MASK already defined
#endif
/** \brief Macro used to extract the preemption flag
 **        from bit 8 of Fee_Gv.FeeInternalFlags
 **        If the "&" operation with this mask gives the following values it means:
 **        Value 0: No Fee asynchronous API is interrupted by the current call;
 **        Value 1: Fee asynchronous API is interrupted by the current call.
 **/
#define FEE_PREEMPTION_MASK                                0x0080U

#if (defined FEE_CHECK_FLAG)
#error FEE_CHECK_FLAG already defined
#endif
/** \brief Macro used to get the value of the flag passed
 **        as parameter from Fee_Gv.FeeInternalFlags
 **/
#define FEE_CHECK_FLAG(Flag)                (((Fee_Gv.FeeInternalFlags) & ((uint16)(Flag))) == ((uint16)(Flag)))

#if (defined FEE_SET_FLAG)
#error FEE_SET_FLAG already defined
#endif
/** \brief Macro used to set the value of the flag passed
 **        as parameter in Fee_Gv.FeeInternalFlags
 **/
#define FEE_SET_FLAG(Flag)                  (Fee_Gv.FeeInternalFlags |= ((uint16)(Flag)))

#if (defined FEE_RESET_FLAG)
#error FEE_RESET_FLAG already defined
#endif
/** \brief Macro used to reset the value of the flag passed
 **        as parameter in Fee_Gv.FeeInternalFlags
 **/
#define FEE_RESET_FLAG(Flag)                (Fee_Gv.FeeInternalFlags &= (((uint16)(~((uint16)(Flag))))))

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
/** \brief Macro used to reset the preemption flag in case the preemption protection is used
 **/
#define FEE_RESET_PREEMPTION_PROTECTION()              FEE_RESET_FLAG(FEE_PREEMPTION_MASK)
#else
#define FEE_RESET_PREEMPTION_PROTECTION()
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief  Function to start a user job or, if there isn't any, to start
 *          the proper internal operation. */
extern FUNC(void, FEE_CODE) Fee_SfTriggerJobOrInternal(void);

/** \brief State function to start jobs when Fee is in idle state */
extern FUNC(void, FEE_CODE) Fee_SfIdleState(void);

/** \brief State function to retry a certain state or trigger the desired function,
 *         but always on the next main function */
extern FUNC(void, FEE_CODE) Fee_SfWaitACycle(void);

#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/** \brief State function that does not do anything, waiting for a job for the critical block.
 */
extern FUNC(void, FEE_CODE) Fee_SfFrozenCritical(void);
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

/** \brief Function to initiate the marking of the section as active */
extern FUNC(void, FEE_CODE) Fee_MarkSectionActive
(
  uint8 ActiveSectionIndex
);

/** \brief Function to repair damaged sections */
extern FUNC(void, FEE_CODE) Fee_StartUp2(void);

/** \brief Function to process section switch operation.
 *         It gets the next block to be copied and check whether space is available
 *         to copy the block. If space is available the block is copied.
 *         Otherwise, the restart switch process takes over.
 */
extern FUNC(void, FEE_CODE) Fee_SectionSwitching(void);

/** \brief Function to calculate the needed space for switch in the last section available.
 */
extern FUNC(Fls_LengthType, FEE_CODE) Fee_CalculateSSRequiredSpace(void);

/** \brief  Function to check whether all the bytes of the block info are in
 *          erased state
 *
 * \param[in] Index block info index in the buffer
 * \revtal TRUE erased byte is present.
 * \retval FALSE erased byte is not present.
 */
extern FUNC(boolean, FEE_CODE) Fee_CheckBlockInfoErased
(
  const uint16 BufferIndex
);

/** \brief Function to handle flash operations refusals. */
extern FUNC(void, FEE_CODE) Fee_FlsJobRefused(void);

/** \brief Function to check if the job really failed or it was refused all the retries.
 *
 * \revtal TRUE it was a failure.
 * \retval FALSE it was a refusal.
 */
extern FUNC(boolean, FEE_CODE) Fee_FlsJobFailed(void);

/** \brief Handle activities after a successful ending job.
 **/
extern FUNC(void, FEE_CODE) Fee_JobEnd(void);

/** \brief Handle activities after a failed ending job.
 **/
extern FUNC(void, FEE_CODE) Fee_JobError(void);

/** \brief Clears the requested job variables and cancels ongoing job operation.
 **/
extern FUNC(void, FEE_CODE) Fee_CancelJob(void);

/** \brief Function to initiate Fee job */
extern FUNC(void, FEE_CODE) Fee_InitiateJob(void);

/** \brief Function to initiate Fee store request if possible */
extern FUNC(void, FEE_CODE) Fee_InitiateStoreRequest(void);

/** \brief Function to change the section when there is no more space for the current job */
extern FUNC(void, FEE_CODE) Fee_JobTriggersSectionChange(void);

/** \brief Function to get block index from configuration table
 *
 *  \param[in] BlockNumber requested block number
 *  \param[out] The index in configuration table where the block id was found.
 */
extern FUNC(uint16, FEE_CODE) Fee_SearchConfigTable
(
    const uint16 BlockId
);

/** \brief Function to calculate the checksum of the given data in the buffer
 *
 * \param[in] Buffer the buffer to calculate the checksum
 * \param[in] DataLength length of the buffer
 */
extern FUNC(uint8, FEE_CODE) Fee_CalculateChecksum
(
  P2CONST(uint8, FEE_CONST, FEE_APPL_DATA)Buffer,
  Fls_LengthType Length
);

/** \brief Function to fill the Fee buffer with block info
 *         including block number, block size, copy/write status and checksum
 *
 * \param[in] BlockNumber block number
 * \param[in] BlockSize block length
 * \param[in] BlockAddress the address where the block is stored in flash
 * \param[in] CopyWriteStatus copy/write status (ex: from user request, or from switch)
 */
extern FUNC(void, FEE_CODE) Fee_FillBlockInfo
(
  const uint16 BlockNumber,
  const uint16 BlockSize,
  const Fls_AddressType BlockAddress,
  const uint8  CopyWriteStatus
);


#if (FEE_CRITICAL_BLOCK_NUMBER != 0U)
/** \brief Handle activities regarding critical block request.
 **/
extern FUNC(void, FEE_CODE) Fee_CriticalBlockHandle(void);
/** \brief Simulate flash failure for returning to critical state. Otherwise returns the Fls job result.
 **/
extern FUNC(void, FEE_CODE) Fee_Get_Critical_Result(void);
/** \brief Returns TRUE if the critical block has been erased. Otherwise returns FALSE
 **/
extern FUNC(boolean, FEE_CODE) Fee_IsCriticalBlockErased
(
  uint16 CriticalBlockIndex
);
/** \brief Cancels any job in order to prepare the critical block writing.
 **/
extern FUNC(void, FEE_CODE) Fee_CriticalCancel(void);
#endif /* (FEE_CRITICAL_BLOCK_NUMBER != 0U) */

#if  ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON))
/** \brief Sets the preemption flag in case it is not set yet.
 **/
extern FUNC(boolean, FEE_CODE) Fee_CheckAndSetPreemptionProtection(void);
#endif /* ((FEE_WRITE_CUSTOM_API == STD_ON) || (FEE_READ_CUSTOM_API == STD_ON)) */

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
/** \brief Returns the total size used by the found not configured blocks.
 **/
extern FUNC(uint32, FEE_CODE) Fee_GetNotConfigFoundBlocksUsedSize(void);
#endif

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

/** \brief Structure which contains all the state related parameters */
extern VAR(Fee_Gv_t, FEE_VAR_CLEARED) Fee_Gv;

#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <MemMap.h>

#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>

/** \brief Fee Block Configuration
 **
 ** Contains the block specific parameters configured by the user of the
 ** FEE module */
extern CONST(Fee_BlockConfiguration_t, FEE_CONST)
  Fee_BlockCfg[FEE_NO_OF_CONFIGURED_BLOCKS];

/** \brief Fee Section Configuration
 **
 ** Contains the section specific parameters configured by the user of the
 ** FEE module */
extern CONST(Fee_SectionConfiguration_t, FEE_CONST)
  Fee_SectionCfg[FEE_NUMBER_OF_SECTIONS];

#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <MemMap.h>

#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include <MemMap.h>

/** \brief Variable which shows the internal state of the module */
extern VAR(Fee_State_t, FEE_VAR) Fee_State;

/** \brief Variable which shows the next internal state of the module */
extern VAR(Fee_State_t, FEE_VAR) Fee_NextState;

/** \brief Variable which holds the address of the block configuration array */
extern P2CONST(Fee_BlockConfiguration_t, AUTOMATIC, FEE_VAR) Fee_BlockCfgPtr;

#if (FEE_NUMBER_NOT_CONFIGURED_BLOCKS > 0U)
extern VAR(Fee_BlockConfiguration_t, FEE_VAR) Fee_NotCfgBlocks[FEE_NUMBER_NOT_CONFIGURED_BLOCKS];
#endif

#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <MemMap.h>

#if FEE_FLS_ALIGNED_32 == STD_OFF
#define FEE_START_SEC_CONFIG_DATA_UNSPECIFIED
#else
#define FEE_START_SEC_CONFIG_DATA_32
#endif
#include <MemMap.h>

#if (FEE_CONSISTENCY_PATTERN_ENABLED == STD_ON)
extern CONST(uint8, FEE_CONST)
  Fee_ConsistencyPattern[FEE_CONSISTENCY_PATTERN_SIZE];
#endif
extern CONST(uint8, FEE_CONST)
  Fee_WrittenMarkerPattern[FEE_WRITTEN_MARKER_SIZE];

#if FEE_FLS_ALIGNED_32 == STD_OFF
#define FEE_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#else
#define FEE_STOP_SEC_CONFIG_DATA_32
#endif
#include <MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef FEE_INTERNAL_H */
