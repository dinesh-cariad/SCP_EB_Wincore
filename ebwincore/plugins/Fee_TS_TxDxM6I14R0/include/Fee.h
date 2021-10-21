#ifndef FEE_H
#define FEE_H
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

/*==================[inclusions]============================================*/
/* !LINKSTO FEE084,1, FEE002,1 */
#include <Std_Types.h>        /* AUTOSAR Standard type header*/
/* !LINKSTO FEE002,1 */
#include <Fee_Cfg.h>          /* FEE configuration header */

/*==================[macros]================================================*/

/*------------------------[AUTOSAR API service IDs]------------------------------*/
#if (defined FEE_INIT_API_ID)
#error FEE_INIT_API_ID already defined
#endif
/** \brief Service Id of Fee_Init */
#define FEE_INIT_API_ID                 0x00U

#if (defined FEE_SETMODE_API_ID)
#error FEE_SETMODE_API_ID already defined
#endif
/** \brief Service Id of Fee_SetMode */
#define FEE_SETMODE_API_ID              0x01U

#if (defined FEE_READ_API_ID)
#error FEE_READ_API_ID already defined
#endif
/** \brief Service Id of Fee_Read */
#define FEE_READ_API_ID                 0x02U

#if (defined FEE_WRITE_API_ID)
#error FEE_WRITE_API_ID already defined
#endif
/** \brief Service Id of Fee_Write */
#define FEE_WRITE_API_ID                0x03U

#if (defined FEE_CANCEL_API_ID)
#error FEE_CANCEL_API_ID already defined
#endif
/** \brief Service Id of Fee_Cancel */
#define FEE_CANCEL_API_ID               0x04U

#if (defined FEE_GETSTATUS_API_ID)
#error FEE_GETSTATUS_API_ID already defined
#endif
/** \brief Service Id of Fee_GetStatus */
#define FEE_GETSTATUS_API_ID            0x05U

#if (defined FEE_GETJOBRESULT_API_ID)
#error FEE_GETJOBRESULT_API_ID already defined
#endif
/** \brief Service Id of Fee_GetJobResult */
#define FEE_GETJOBRESULT_API_ID         0x06U

#if (defined FEE_INVALIDATEBLOCK_API_ID)
#error FEE_INVALIDATEBLOCK_API_ID already defined
#endif
/** \brief Service Id of Fee_InvalidateBlock */
#define FEE_INVALIDATEBLOCK_API_ID      0x07U

#if (defined FEE_GET_VERSION_INFO_API_ID)
#error FEE_GET_VERSION_INFO_API_ID already defined
#endif
/** \brief Service Id of Fee_GetVersionInfo */
#define FEE_GET_VERSION_INFO_API_ID     0x08U

#if (defined FEE_ERASEIMMEDIATEBLOCK_API_ID)
#error FEE_ERASEIMMEDIATEBLOCK_API_ID already defined
#endif
/** \brief Service Id of Fee_EraseImmediateBlock */
#define FEE_ERASEIMMEDIATEBLOCK_API_ID  0x09U

#if (defined FEE_JOBENDNOTIFICATION_API_ID)
#error FEE_JOBENDNOTIFICATION_API_ID already defined
#endif
/** \brief Service Id of Fee_JobEndNotification */
#define FEE_JOBENDNOTIFICATION_API_ID   0x10U

#if (defined FEE_JOBERRORNOTIFICATION_API_ID)
#error FEE_JOBERRORNOTIFICATION_API_ID already defined
#endif
/** \brief Service Id of Fee_JobErrorNotification */
#define FEE_JOBERRORNOTIFICATION_API_ID 0x11U

#if (defined FEE_MAINFUNCTION_API_ID)
#error FEE_MAINFUNCTION_API_ID already defined
#endif
/** \brief Service Id of Fee_MainFunction */
#define FEE_MAINFUNCTION_API_ID         0x12U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define FEE_START_SEC_CODE
#include <MemMap.h>

/** \brief Service for initialization of the Fee module
 **
 ** This function initializes the FEE module.
 ** Normally, the ECU Manager invokes this API.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, FEE_CODE) Fee_Init(void);

/** \brief Service for setting the operation mode of the underlying flash driver
 **
 ** This function sets the operation mode of the underlying flash driver.
 **
 ** \param[in]  Mode The desired mode of the flash driver
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
 #if (FEE_SET_MODE_SUPPORTED == STD_ON)
extern FUNC(void, FEE_CODE) Fee_SetMode(MemIf_ModeType Mode);
 #endif

/** \brief Service for reading the contents of a logical block
 **
 ** This API starts a read operation.
 ** The read operation involves reading the contents of a logical block
 ** stored in flash into a user buffer. The actual read job is
 ** executed asynchronously within Fee_MainFunction.
 **
 ** \param[in]  BlockNumber Number of logical block to be read
 ** \param[in]  BlockOffset Read address offset inside the block
 ** \param[in]  DataBufferPtr Pointer to data buffer
 ** \param[in]  Length  Number of bytes to read
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the read job
 ** \retval E_NOT_OK Fee rejected the read job
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_Read
(
  uint16 BlockNumber,
  uint16 BlockOffset,
  P2VAR(uint8, AUTOMATIC, FEE_APPL_DATA)DataBufferPtr,
  uint16 Length
);

/** \brief Service for writing a logical block
 **
 ** This API starts a write operation.
 ** This operation writes the contents of the user buffer to a
 ** logical block in flash. The actual write job is
 ** executed asynchronously within Fee_MainFunction.
 **
 ** \param[in]      BlockNumber Number of logical block
 ** \param[out]     DataBufferPtr Pointer to data buffer
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the write job
 ** \retval E_NOT_OK Fee rejected the write job
 **
 ** \ServiceID{3}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_Write
(
  uint16 BlockNumber,
  P2CONST(uint8, AUTOMATIC, FEE_APPL_DATA)DataBufferPtr
);

/** \brief Service for cancelling an ongoing job
 **
 ** This API cancels a previous job request. It also resets
 ** the module internal state machine so that a new job request can be accepted.
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, FEE_CODE) Fee_Cancel(void);

/** \brief Service for getting the status of the module
 **
 ** This API returns the status of the Fee module.
 **
 ** \return MemIf_StatusType
 ** \retval MEMIF_UNINIT        Fee has not been initialized.
 ** \retval MEMIF_IDLE          Fee is Idle doing no job
 ** \retval MEMIF_BUSY          Fee is busy processing previous request or request in queue
 ** \retval MEMIF_BUSY_INTERNAL Fee internal operation is in progress
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(MemIf_StatusType, FEE_CODE) Fee_GetStatus(void);

/** \brief Service for getting the result of the last job
 **
 ** This API returns the result of the last processed job.
 **
 ** \return MemIf_JobResultType
 ** \retval MEMIF_JOB_PENDING        Requested job is in queue or being processed
 ** \retval MEMIF_JOB_OK             Requested job finished successfully
 ** \retval MEMIF_JOB_FAILED         Requested job failed with error
 ** \retval MEMIF_JOB_CANCELED       Requested job got cancelled
 ** \retval MEMIF_BLOCK_INCONSISTENT Requested block is inconsistent in flash
 ** \retval MEMIF_BLOCK_INVALID      Requested block is invalidated in flash or not found in flash
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(MemIf_JobResultType, FEE_CODE) Fee_GetJobResult(void);

/** \brief Service for invalidating a logical block
 **
 ** This API marks the contents of a logical block invalid.
 ** This is done by writing the block management info with an invalid block size.
 **
 ** \param[in]      BlockNumber Number of logical block to be invalidated
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the invalidate job
 ** \retval E_NOT_OK Fee rejected the invalidate job
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_InvalidateBlock(uint16 BlockNumber);

/** \brief Service for getting the version information
 **
 ** This API returns version information of Fee module.
 ** Normally, the application will invoke this API.
 **
 ** \param[out]     VersionInfoPtr Pointer to standard version information structure
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
#if (FEE_VERSION_INFO_API == STD_ON)
extern FUNC(void, FEE_CODE) Fee_GetVersionInfo
(P2VAR(Std_VersionInfoType, AUTOMATIC, FEE_APPL_DATA) VersionInfoPtr);
#endif

/** \brief Service to ensure that immediate data can be written
 **
 ** This function doesn't erase or invalidate the block.
 ** FEE makes sure that there is always space available for
 ** immediate block write and it is not sure that this API is
 ** called just before the write of an immediate block.
 **
 ** \param[in]      BlockNumber Number of logical block to be written
 **
 ** \return Std_ReturnType
 ** \retval E_OK     Fee accepted the erase immediate job
 ** \retval E_NOT_OK Fee rejected the erase immediate job
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, FEE_CODE) Fee_EraseImmediateBlock(uint16 BlockNumber);

/** \brief Scheduled service to process all Fee jobs asynchronously
 **
 ** This function asynchronously handles the requested read/write/erase jobs
 ** and performs all internal management operations.
 **
 ** \ProductionError{FEE_E_FLASH_ACCESSIBLE, thrown\, during initialization
 ** or during section switch if the section status cannot be read or written
 ** or a section cannot be erased.}
 **
 ** \ProductionError{FEE_E_DATA_RECOVERED, thrown\, during startup if the
 ** user data from active section cannot be used or active section cannot
 ** be determined because of an unexpected section status combination and
 ** user data is recovered from the inactive section. Outdated data could
 ** be recovered in this case. }
 **
 ** \ServiceID{12}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Asynchronous} */
extern FUNC(void, FEE_CODE) Fee_MainFunction(void);

#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef FEE_H */

