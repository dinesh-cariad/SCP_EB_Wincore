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
#ifndef EA_H
#define EA_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>           /* EB specific standard types */
/* !LINKSTO EA083,1, EA113,1 */
#include <Std_Types.h>           /* AUTOSAR standard types */

#include <Ea_Version.h>          /* this module's version declaration */
/* !LINKSTO EA113,1 */
#include <Ea_Cfg.h>              /* Configuration Generated */

/*==================[macros]================================================*/

#if (defined EA_API_INIT)
#error EA_API_INIT already defined
#endif
/** \brief Service ID for Ea_Init. */
#define EA_API_INIT                 0x00U

#if (defined EA_API_SETMODE)
#error EA_API_SETMODE already defined
#endif
/** \brief Service ID for Ea_SetMode. */
#define EA_API_SETMODE              0x01U

#if (defined EA_API_READ)
#error EA_API_READ already defined
#endif
/** \brief Service ID for Ea_Read. */
#define EA_API_READ                 0x02U

#if (defined EA_API_WRITE)
#error EA_API_WRITE already defined
#endif
/** \brief Service ID for Ea_Write. */
#define EA_API_WRITE                0x03U

#if (defined EA_API_CANCEL)
#error EA_API_CANCEL already defined
#endif
/** \brief Service ID for Ea_Cancel. */
#define EA_API_CANCEL               0x04U

#if (defined EA_API_GETSTATUS)
#error EA_API_GETSTATUS already defined
#endif
/** \brief Service ID for Ea_GetStatus. */
#define EA_API_GETSTATUS            0x05U

#if (defined EA_API_GETJOBRESULT)
#error EA_API_GETJOBRESULT already defined
#endif
/** \brief Service ID for Ea_GetJobResult. */
#define EA_API_GETJOBRESULT         0x06U

#if (defined EA_API_INVALIDATEBLOCK)
#error EA_API_INVALIDATEBLOCK already defined
#endif
/** \brief Service ID for Ea_InvalidateBlock. */
#define EA_API_INVALIDATEBLOCK      0x07U

#if (defined EA_API_GETVERSIONINFO)
#error EA_API_GETVERSIONINFO already defined
#endif
/** \brief Service ID for Ea_GetVersionInfo. */
#define EA_API_GETVERSIONINFO       0x08U

#if (defined EA_API_ERASEIMMEDIATEBLOCK)
#error EA_API_ERASEIMMEDIATEBLOCK already defined
#endif
/** \brief Service ID for Ea_EraseImmediateBlock. */
#define EA_API_ERASEIMMEDIATEBLOCK  0x09U

#if (defined EA_API_JOBENDNOTIFICATION)
#error EA_API_JOBENDNOTIFICATION already defined
#endif
/** \brief Service ID for Ea_JobEndNotification. */
#define EA_API_JOBENDNOTIFICATION   0x10U

#if (defined EA_API_JOBERRORNOTIFICATION)
#error EA_API_JOBERRORNOTIFICATION already defined
#endif
/** \brief Service ID for Ea_JobErrorNotification. */
#define EA_API_JOBERRORNOTIFICATION 0x11U

#if (defined EA_API_MAINFUNCTION)
#error EA_API_MAINFUNCTION already defined
#endif
/** \brief Service ID for Ea_MainFunction. */
#define EA_API_MAINFUNCTION         0x12U
/* !LINKSTO EA049,1 */
#if (defined EA_E_UNINIT)
#error EA_E_UNINIT already defined
#endif
/** \brief DET Error: module not initialized. */
#define EA_E_UNINIT        1U

#if (defined EA_E_INVALID_BLOCK_NO)
#error EA_E_INVALID_BLOCK_NO already defined
#endif
/** \brief DET Error: invalid block number. */
#define EA_E_INVALID_BLOCK_NO       2U

#if (defined EA_E_INVALID_BLOCK_OFS) /* to prevent double declaration */
#error EA_E_INVALID_BLOCK_OFS already defined
#endif /* if (defined EA_E_INVALID_BLOCK_OFS) */
/** \brief Development error indicating that the block offset is invalid */
#define EA_E_INVALID_BLOCK_OFS         (uint8)0x03U

#if (defined EA_E_INVALID_DATA_PTR) /* to prevent double declaration */
#error EA_E_INVALID_DATA_PTR already defined
#endif /* if (defined EA_E_INVALID_DATA_PTR) */
/** \brief Development error indicating that a null pointer is passed as a parameter */
#define EA_E_INVALID_DATA_PTR          (uint8)0x04U

#if (defined EA_E_INVALID_BLOCK_LEN) /* to prevent double declaration */
#error EA_E_INVALID_BLOCK_LEN already defined
#endif /* if (defined EA_E_INVALID_BLOCK_LEN) */
/** \brief Development error indicating that the Block length is invalid */
#define EA_E_INVALID_BLOCK_LEN         (uint8)0x05U

#if (defined EA_E_BUSY) /* to prevent double declaration */
#error EA_E_BUSY already defined
#endif /* if (defined EA_E_BUSY) */
/** \brief Development error indicating that the status is BUSY */
#define EA_E_BUSY         (uint8)0x06U

#if (defined EA_E_BUSY_INTERNAL) /* to prevent double declaration */
#error EA_E_BUSY_INTERNAL already defined
#endif /* if (defined EA_E_BUSY_INTERNAL) */
/** \brief Development error indicating that the status is BUSY due to internal operation */
#define EA_E_BUSY_INTERNAL         (uint8)0x07U

#if (defined EA_E_INVALID_CANCEL) /* to prevent double declaration */
#error EA_E_INVALID_CANCEL already defined
#endif /* if (defined EA_E_INVALID_CANCEL) */
/** \brief Development error indicating that the cancel request is invalid */
#define EA_E_INVALID_CANCEL         (uint8)0x08U

#if (defined EA_E_INVALID_MODE) /* to prevent double declaration */
#error EA_E_INVALID_MODE already defined
#endif /* if (defined EA_E_INVALID_MODE) */
/** \brief Development error indicating that the requested mode is invalid */
#define EA_E_INVALID_MODE         (uint8)0x09U

#if (defined EA_INSTANCE_ID)
#error EA_INSTANCE_ID already defined
#endif
/** \brief Module's instance ID. */
#define EA_INSTANCE_ID              0U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define EA_START_SEC_CODE
#include <Ea_MemMap.h>

/** \brief Initializes the Ea module.
 **
 ** Initialize the Ea module.
 **
 ** \ServiceID{0}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
*/
extern FUNC(void, EA_CODE) Ea_Init(void);

/** \brief Sets EEPROM driver to FAST or SLOW mode.
 **
 ** Calls the EEPROM driver service Eep_SetMode to switch the EEPROM driver
 ** to fast or slow mode
 **
 ** \param[in] Mode Desired mode for the underlying EEPROM driver
 **
 ** \ServiceID{1}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, EA_CODE) Ea_SetMode(MemIf_ModeType Mode);

/** \brief Reads data from EEPROM.
 **
 ** Read Length bytes of data in the given block starting at
 ** BlockOffset and write them in DataBufferPtr.
 **
 ** \param[in]  BlockNumber   Identifier of the block (from the configuration)
 ** \param[in]  BlockOffset   Read address offset inside the block
 ** \param[in]  Length        Number of bytes to read
 ** \param[out] DataBufferPtr Pointer to data buffer
 **
 ** \return Standard Return Code
 ** \return     E_OK          Job accepted
 ** \return     E_NOT_OK      BlockNumber not found
 **
 ** \ServiceID{2}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{asynchronous}
 **
 */
extern FUNC(Std_ReturnType, EA_CODE) Ea_Read
(
  uint16 BlockNumber,
  uint16 BlockOffset,
  P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) DataBufferPtr,
  uint16 Length
);

/** \brief Writes data to EEPROM.
 **
 ** Write the data from DataBufferPtr to the EEPROM in the block
 ** identified by BlockNumber. The length is always the configured
 ** length for the given block.
 **
 ** \param[in] BlockNumber   Identifier of the block (from the configuration).
 ** \param[in] DataBufferPtr Pointer to data buffer
 **
 ** \return Standard Return Code
 ** \retval    E_OK          Job accepted.
 ** \retval    E_NOT_OK      BlockNumber not found.
 **
 ** \ServiceID{3}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{asynchronous}
 **
 */
extern FUNC(Std_ReturnType, EA_CODE) Ea_Write
(
  uint16 BlockNumber,
  P2CONST(uint8, AUTOMATIC, EA_APPL_DATA) DataBufferPtr
);

/** \brief Calls the function Eep_Cancel.
 **
 ** This function calls the Eep_Cancel() to cancel ongoing jobs.
 **
 ** \ServiceID{4}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{asynchronous}
 **
 */
extern FUNC(void, EA_CODE) Ea_Cancel(void);

/** \brief Returns the status of the Ea.
 **
 ** Calls the Eep_GetStatus function of the underlying EEPROM Driver
 ** and passes the returned value back to the caller. If the underlying driver
 ** returned the status MEMIF_IDLE and an internal operation is currently
 ** ongoing in the EEPROM Abstraction module, this function returns
 ** MEMIF_BUSY_INTERNAL.
 **
 ** \return MemIf_StatusType
 ** \retval MEMIF_UNINIT        The underlying driver has not been initialized.
 ** \retval MEMIF_IDLE          The underlying driver and Ea module are idle.
 ** \retval MEMIF_BUSY          The underlying driver is busy.
 **
 ** \ServiceID{5}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(MemIf_StatusType, EA_CODE) Ea_GetStatus(void);

/** \brief Returns the current job result.
 **
 ** This function returns the status of the current job or the last finished
 ** one if none is ongoing. Calls the function Eep_GetJobResult of the
 ** EEPROM Driver. If it returns MEMIF_JOB_OK, the result from the EEPROM
 ** Abstraction module is returned, otherwise, the result from the
 ** EEPROM Driver is returned.
 **
 ** \return MemIf_JobResultType
 ** \retval MEMIF_JOB_OK             Job finished successfully.
 ** \retval MEMIF_JOB_PENDING        Job is being performed.
 ** \retval MEMIF_JOB_CANCELED       Job has been cancelled.
 ** \retval MEMIF_JOB_FAILED         Job failed.
 ** \retval MEMIF_BLOCK_INCONSISTENT Block inconsistent, can't read data.
 ** \retval MEMIF_BLOCK_INVALID      Block marked invalid, can't read data.
 **
 ** \ServiceID{6}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(MemIf_JobResultType, EA_CODE) Ea_GetJobResult(void);

/** \brief Marks the given block as invalidated.
 **
 ** This function marks the selected block as invalid. Uses the EEPROM
 ** Driver function Eep_Write.
 **
 ** \param[in]  BlockNumber  Identifier of the block (from the configuration).
 **
 ** \return Standard Return Code
 ** \retval E_OK         Job accepted.
 ** \retval E_NOT_OK     BlockNumber not found.
 **
 ** \ServiceID{7}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{asynchronous}
 **
 */
extern FUNC(Std_ReturnType, EA_CODE) Ea_InvalidateBlock(uint16 BlockNumber);


#if (EA_VERSION_INFO_API == STD_ON)

/** \brief Returns Ea's version information.
 **
 ** This service returns the version information of this module. The version
 ** information includes Module ID, Vendor ID and vendor specific version
 ** numbers. NOTE: This function cannot be disabled by configuration
 ** parameter EaVersionInfoApi but the function is only linked to the user
 ** application when it is invoked.
 **
 ** \param[out] VersionInfoPtr Pointer to standard version information
 ** structure.
 **
 ** \ServiceID{8}
 ** \Reentrancy{reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, EA_CODE) Ea_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, EA_APPL_DATA) VersionInfoPtr
);
#endif

/** \brief Pre-erase the data in the given block
 **
 ** This function erases the selected block so that data can be immediately
 ** written during the next write operation. Uses the EEPROM Driver function
 ** Eep_Erase.
 ** When multiple copies are present for a block, the job of the function
 ** first invalidates the latest copy for the block in order to ensure
 ** that old data will not be returned by a subsequent Ea_Read.
 ** It then erases the location where the next copy is to be written.
 **
 ** \param[in] BlockNumber Identifier of the block (from the configuration).
 **
 ** \return Standard Return Code
 ** \retval    E_OK        Job accepted.
 ** \retval    E_NOT_OK    BlockNumber not found.
 **
 ** \ServiceID{9}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{asynchronous} */
extern FUNC(Std_ReturnType, EA_CODE)
  Ea_EraseImmediateBlock(uint16 BlockNumber);

/** \brief Handles the requested jobs asynchronously.
 **
 ** This function needs to be called periodicall by the BSW scheduler.
 **
 ** \ServiceID{18}
 **
 */
extern FUNC(void, EA_CODE) Ea_MainFunction(void);

#define EA_STOP_SEC_CODE
#include <Ea_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef EA_H */
/*==================[end of file]===========================================*/
