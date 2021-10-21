#ifndef EEP_H
#define EEP_H

/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*==================[inclusions]============================================*/
#include <Std_Types.h>               /* AUTOSAR standard types */
#include <TSAutosar.h>               /* EB specific standard types */

#include <Eep_Version.h>             /* this module's version declaration */
#include <Eep_Cfg.h>                 /* this module's public configuration */

/*==================[macros]================================================*/

/** \brief AUTOSAR module instance identification
 **
 ** */
#define EEP_INSTANCE_ID                            0U

/*------------------[API service IDs]---------------------------------------*/
#if (defined EEP_SID_INIT)
#error EEP_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Init(). */
#define EEP_SID_INIT                               0U

#if (defined EEP_SID_SET_MODE)
#error EEP_SID_SET_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_SetMode(). */
#define EEP_SID_SET_MODE                           1U

#if (defined EEP_SID_READ)
#error EEP_SID_READ already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Read(). */
#define EEP_SID_READ                               2U

#if (defined EEP_SID_WRITE)
#error EEP_SID_WRITE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Write(). */
#define EEP_SID_WRITE                              3U

#if (defined EEP_SID_ERASE)
#error EEP_SID_ERASE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Erase(). */
#define EEP_SID_ERASE                              4U

#if (defined EEP_SID_COMPARE)
#error EEP_SID_COMPARE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Compare(). */
#define EEP_SID_COMPARE                            5U

#if (defined EEP_SID_CANCEL)
#error EEP_SID_CANCEL already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_Cancel(). */
#define EEP_SID_CANCEL                             6U

#if (defined EEP_SID_GET_STATUS)
#error EEP_SID_GET_STATUS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_GetStatus(). */
#define EEP_SID_GET_STATUS                         7U

#if (defined EEP_SID_GET_JOB_RESULT)
#error EEP_SID_GET_JOB_RESULT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_GetJobResult(). */
#define EEP_SID_GET_JOB_RESULT                     8U

#if (defined EEP_SID_GET_VERSION_INFO)
#error EEP_SID_GET_VERSION_INFO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_GetVersionInfo(). */
#define EEP_SID_GET_VERSION_INFO                   10U

#if (defined EEP_SID_MAIN_FUNCTION)
#error EEP_SID_MAIN_FUNCTION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for Eep_MainFunction(). */
#define EEP_SID_MAIN_FUNCTION                      9U

/*------------------[DET error codes]---------------------------------------*/
#if (defined EEP_E_PARAM_CONFIG)
#error EEP_E_PARAM_CONFIG already defined
#endif
/** \brief Development error code used if Eep_Init() is called with
 * null parameter pointer */
#define EEP_E_PARAM_CONFIG                        16U

#if (defined EEP_E_PARAM_ADDRESS)
#error EEP_E_PARAM_ADDRESS already defined
#endif
/** \brief Development error code used if Eep_Read(), Eep_Write(),
 *   Eep_Compare(), Eep_Erase() is called and parameter EepromAddress
 *   is not in within the valid EEPROM address range */
#define EEP_E_PARAM_ADDRESS                       17U

#if (defined EEP_E_PARAM_DATA)
#error EEP_E_PARAM_DATA already defined
#endif
/** \brief Development error code used if Eep_Read(), Eep_Write(),
 *   Eep_Compare(), Eep_Erase() is called and parameter DataBufferPtr
 *   is null */
#define EEP_E_PARAM_DATA                          18U

#if (defined EEP_E_PARAM_LENGTH)
#error EEP_E_PARAM_LENGTH already defined
#endif
/** \brief Development error code used if Eep_Read(), Eep_Write(),
 *   Eep_Compare(), Eep_Erase() is called and parameter Length
 *   is not within the specified minimum and maximum values */
#define EEP_E_PARAM_LENGTH                        19U

#if (defined EEP_E_UNINIT)
#error EEP_E_UNINIT already defined
#endif
/** \brief Development error code used if Eep_Read(), Eep_Write(),
 *   Eep_Compare(), Eep_Erase(), Eep_SetMode() is called
 *   and EEPROM module is uninitialized */
#define EEP_E_UNINIT                              32U

#if (defined EEP_E_BUSY)
#error EEP_E_BUSY already defined
#endif
/** \brief Development error code used if Eep_Read(), Eep_Write(),
 *   Eep_Compare(), Eep_Erase(), Eep_SetMode() is called
 *   and EEPROM module is busy */
#define EEP_E_BUSY                                33U

/*------------------[internal API id definition]----------------------------*/

#if (defined EEP_INTERNAL_API_ID)
#error EEP_INTERNAL_API_ID is already defined
#endif
/** \brief Service id for internal functions */
#define EEP_INTERNAL_API_ID                       0xFFU

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define EEP_START_SEC_CODE
#include <Eep_MemMap.h>

/** \brief Initialize the EEPROM Driver.
 **
 ** This function initializes the EEPROM Driver.
 **
 ** \param[in] ConfigPtr Config Pointer to driver configuration.
 **
 ** \ServiceID{::EEP_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, EEP_CODE) Eep_Init(
   P2CONST(Eep_ConfigType, AUTOMATIC, EEP_APPL_CONST) ConfigPtr);

/** \brief Set the EEPROM Driver operation mode.
 **
 ** This function sets the EEPROM Driver operation mode into
 ** MEMIF_MODE_SLOW or MEMIF_MODE_FAST.
 **
 ** \param[in] Mode Required module mode to be set
 **
 ** \ServiceID{::EEP_SID_SET_MODE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, EEP_CODE) Eep_SetMode(
   MemIf_ModeType Mode);

/** \brief Initiate EEPROM Driver read operation.
 **
 ** This function shall copy given parameters, and initiate read job
 **
 ** \param[in] EepromAddress Start job EEPROM address
 ** \param[out] DataBufferPtr User buffer data pointer
 ** \param[in] Length Job length in bytes
 **
 ** \return Status of the job acceptance
 ** \retval E_OK job accepted
 ** \retval E_NOT_OK job not accepted, failure
 **
 ** \ServiceID{::EEP_SID_READ}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, EEP_CODE) Eep_Read(
   Eep_AddressType EepromAddress,
   P2VAR(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
   Eep_LengthType Length);

/** \brief Initiate EEPROM Driver write operation.
 **
 ** This function shall copy given parameters, and initiate write job
 **
 ** \param[in] EepromAddress Start job EEPROM address
 ** \param[in] DataBufferPtr User buffer data pointer
 ** \param[in] Length Job length in bytes
 **
 ** \return Status of the job acceptance
 ** \retval E_OK job accepted
 ** \retval E_NOT_OK job not accepted, failure
 **
 ** \ServiceID{::EEP_SID_WRITE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, EEP_CODE) Eep_Write(
   Eep_AddressType EepromAddress,
   P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
   Eep_LengthType Length);

/** \brief Initiate EEPROM Driver erase operation.
 **
 ** This function shall copy given parameters, and initiate erase job
 **
 ** \param[in] EepromAddress Start job EEPROM address
 ** \param[in] Length Job length in bytes
 **
 ** \return Status of the job acceptance
 ** \retval E_OK job accepted
 ** \retval E_NOT_OK job not accepted, failure
 **
 ** \ServiceID{::EEP_SID_ERASE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, EEP_CODE) Eep_Erase(
   Eep_AddressType EepromAddress,
   Eep_LengthType Length);

/** \brief Initiate EEPROM Driver compare operation.
 **
 ** This function shall copy given parameters, and initiate compare job
 **
 ** \param[in] EepromAddress Start job EEPROM address
 ** \param[in] DataBufferPtr User buffer data pointer
 ** \param[in] Length Job length in bytes
 **
 ** \return Status of the job acceptance
 ** \retval E_OK job accepted
 ** \retval E_NOT_OK job not accepted, failure
 **
 ** \ServiceID{::EEP_SID_COMPARE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, EEP_CODE) Eep_Compare(
   Eep_AddressType EepromAddress,
   P2CONST(uint8, AUTOMATIC, EEP_APPL_CONST) DataBufferPtr,
   Eep_LengthType Length);

/** \brief Cancel EEPROM Driver ongoing job synchronously.
 **
 ** This function shall abort a running job synchronously
 **
 **
 ** \ServiceID{::EEP_SID_CANCEL}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, EEP_CODE) Eep_Cancel(
   void);

/** \brief Get status of EEPROM Driver
 **
 ** This function shall synchronously return status of EEPROM Driver
 **
 ** \return Module status
 ** \retval MEMIF_UNINIT The module has not been initialized (yet).
 ** \retval MEMIF_BUSY The module is currently busy.
 ** \retval MEMIF_IDLE The module is currently idle.
 **
 ** \ServiceID{::EEP_SID_GET_STATUS}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(MemIf_StatusType, EEP_CODE) Eep_GetStatus(
   void);

/** \brief Get result of last job of EEPROM Driver
 **
 ** This function shall synchronously return result of the last job
 ** accepted by EEPROM Driver
 **
 ** \return Job result
 ** \retval MEMIF_JOB_OK The job has been finished successfully.
 ** \retval MEMIF_JOB_PENDING The job has not yet been finished.
 ** \retval MEMIF_JOB_FAILED The job has not been finished successfully.
 ** \retval MEMIF_BLOCK_INCONSISTENT Compared data areas are not equal.
 ** \retval MEMIF_JOB_CANCELED The job has been canceled.
 **
 ** \ServiceID{::EEP_SID_GET_JOB_RESULT}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(MemIf_JobResultType, EEP_CODE) Eep_GetJobResult(
   void);

#if(EEP_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the EEPROM Driver.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Vendor Id
 **  - Module Id
 **  - Instance Id
 **  - Vendor specific version numbers
 **
 **  Precondition: Function is only available if parameter EepVersionInfoApi
 **  is set to true.
 **
 ** \param[out] VersionInfoPtr Pointer to where to store the version
 ** information of this module.
 **
 ** \ServiceID{::EEP_SID_GET_VERSION_INFO}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, EEP_CODE) Eep_GetVersionInfo(
  CONSTP2VAR(Std_VersionInfoType, AUTOMATIC, EEP_APPL_DATA) VersionInfoPtr);
#endif

/** \brief Service to perform the processing of the EEPROM jobs
 **
 ** This function shall perform the processing of the EEPROM read, write,
 ** erase and compare jobs.
 **
 **
 **
 ** \ServiceID{::EEP_SID_MAIN_FUNCTION}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, EEP_CODE) Eep_MainFunction(
  void);

#define EEP_STOP_SEC_CODE
#include <Eep_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef EEP_H */
