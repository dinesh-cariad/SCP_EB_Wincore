/**
 * \file
 *
 * \brief AUTOSAR MemIf
 *
 * This file contains the implementation of the AUTOSAR
 * module MemIf.
 *
 * \version 5.11.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef MEMIF_H
#define MEMIF_H

/*==================[inclusions]============================================*/

/* !LINKSTO MemIf037,1 */
#include <Std_Types.h>          /* AUTOSAR standard types */

#include <MemIf_Version.h>      /* this module's version declaration */
#include <MemIf_Types.h>        /* static public types of MemIf module */

#include <MemIf_Cfg.h>          /* generated module configuration */

/*==================[macros]================================================*/

/*------------------[Parameters for DET calls]------------------------------*/

#if (defined MEMIF_INSTANCE_ID)
#error MEMIF_INSTANCE_ID is already defined
#endif /* if (defined MEMIF_INSTANCE_ID) */
/** \brief MemIf instance ID
 **
 ** The identifier of the index based instance of the Memory Abstraction
 ** Interface, starting from 0. I.e. if the module is a single instance
 ** module, the instance ID is equal to 0. */
#define MEMIF_INSTANCE_ID             0U

#if (defined MEMIF_E_PARAM_DEVICE)
#error MEMIF_E_PARAM_DEVICE is already defined
#endif /* if (defined MEMIF_E_PARAM_DEVICE) */
/* !LINKSTO MemIf030,1 */
/** \brief Development error codes
 **
 ** API service called with wrong device index parameter */
#define MEMIF_E_PARAM_DEVICE          1U

#if (defined MEMIF_E_PARAM_NULL_PTR)
#error MEMIF_E_PARAM_NULL_PTR is already defined
#endif /* if (defined MEMIF_E_PARAM_NULL_PTR) */
/** \brief Development error codes
 **
 ** API service called with invalid null pointer parameter */
#define MEMIF_E_PARAM_NULL_PTR        2U

/*------------------[list of AUTOSAR API Ids]-------------------------------*/

#if (defined MEMIF_SETMODE_ID)
#error MEMIF_SETMODE_ID is already defined
#endif /* if (defined MEMIF_SETMODE_ID) */
/** \brief AUTOSAR API ID of MemIf_SetMode() function */
#define MEMIF_SETMODE_ID              1U

#if (defined MEMIF_READ_ID)
#error MEMIF_READ_ID is already defined
#endif /* if (defined MEMIF_READ_ID) */
/** \brief AUTOSAR API ID of MemIf_Read() function */
#define MEMIF_READ_ID                 2U

#if (defined MEMIF_WRITE_ID)
#error MEMIF_WRITE_ID is already defined
#endif /* if (defined MEMIF_WRITE_ID) */
/** \brief AUTOSAR API ID of MemIf_Write() function */
#define MEMIF_WRITE_ID                3U

#if (defined MEMIF_CANCEL_ID)
#error MEMIF_CANCEL_ID is already defined
#endif /* if (defined MEMIF_CANCEL_ID) */
/** \brief AUTOSAR API ID of MemIf_Cancel() function */
#define MEMIF_CANCEL_ID               4U

#if (defined MEMIF_GETSTATUS_ID)
#error MEMIF_GETSTATUS_ID is already defined
#endif /* if (defined MEMIF_GETSTATUS_ID) */
/** \brief AUTOSAR API ID of MemIf_GetStatus() function */
#define MEMIF_GETSTATUS_ID            5U

#if (defined MEMIF_GETJOBRESULT_ID)
#error MEMIF_GETJOBRESULT_ID is already defined
#endif /* if (defined MEMIF_GETJOBRESULT_ID) */
/** \brief AUTOSAR API ID of MemIf_GetJobResult() function */
#define MEMIF_GETJOBRESULT_ID         6U

#if (defined MEMIF_INVALIDATEBLOCK_ID)
#error MEMIF_INVALIDATEBLOCK_ID is already defined
#endif /* if (defined MEMIF_INVALIDATEBLOCK_ID) */
/** \brief AUTOSAR API ID of MemIf_InvalidateBlock() function */
#define MEMIF_INVALIDATEBLOCK_ID      7U

#if (defined MEMIF_GETVERSIONINFO_ID)
#error MEMIF_GETVERSIONINFO_ID is already defined
#endif /* if (defined MEMIF_GETVERSIONINFO_ID) */
/** \brief AUTOSAR API ID of MemIf_GetVersionInfo() function */
#define MEMIF_GETVERSIONINFO_ID       8U

#if (defined MEMIF_ERASEIMMEDIATEBLOCK_ID)
#error MEMIF_ERASEIMMEDIATEBLOCK_ID is already defined
#endif /* if (defined MEMIF_ERASEIMMEDIATEBLOCK_ID) */
/** \brief AUTOSAR API ID of MemIf_EraseImmediateBlock() function */
#define MEMIF_ERASEIMMEDIATEBLOCK_ID  9U

/*==================[type definitions]======================================*/

#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

/** \brief MemIf_SetMode function pointer type definition */
typedef P2FUNC(void, MEMIF_APPL_CODE, MemIf_SetModeFctPtrType)(
  MemIf_ModeType Mode);

/** \brief MemIf_Read function pointer type definition */
typedef P2FUNC(Std_ReturnType, MEMIF_APPL_CODE, MemIf_ReadFctPtrType)(
  uint16                                   BlockNumber,
  uint16                                   BlockOffset,
  P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
  uint16                                   Length);

/** \brief MemIf_Write function pointer type definition */
typedef P2FUNC(Std_ReturnType, MEMIF_APPL_CODE, MemIf_WriteFctPtrType)(
  uint16                                   BlockNumber,
  P2CONST(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr);

/** \brief MemIf_Cancel function pointer type definition */
typedef P2FUNC(void, MEMIF_APPL_CODE, MemIf_CancelFctPtrType)(void);

/** \brief MemIf_GetStatus function pointer type definition */
typedef P2FUNC(MemIf_StatusType, MEMIF_APPL_CODE, MemIf_GetStatusFctPtrType)(
  void);

/** \brief MemIf_GetJobResult function pointer type definition */
typedef P2FUNC(MemIf_JobResultType, MEMIF_APPL_CODE, MemIf_GetJobResultFctPtrType)(
  void);

/** \brief MemIf_InvalidateBlock function pointer type definition */
typedef P2FUNC(Std_ReturnType, MEMIF_APPL_CODE, MemIf_InvalidateBlockFctPtrType)(
  uint16 BlockNumber);

/** \brief MemIf_EraseImmediateBlock function pointer type definition */
typedef P2FUNC(Std_ReturnType, MEMIF_APPL_CODE, MemIf_EraseImmedBlockFctPtrType)(
  uint16 BlockNumber);

#endif

/*==================[external function declarations]========================*/

#define MEMIF_START_SEC_CODE
#include <MemIf_MemMap.h>

/* function declarartion/definitions only available for Det On and multiple
 * drivers */
#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)
# if (MEMIF_DEV_ERROR_DETECT == STD_ON)

/* declarations of other MemIf_*() functions */

/** \brief Mapped to read service of corresponding EEPROM
 ** Abstraction and Flash EEPROM Emulation modules
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \param[in]   BlockNumber Number of logical block in EA or FEE
 ** \param[in]   BlockOffset Read address offset inside the block
 ** \param[out]  DataBufferPtr Pointer to data buffer
 ** \param[in]   Length Number of bytes to read
 ** \return Std_ReturnType
 ** \retval E_OK      Read finished successfully.
 ** \retval E_NOT_OK  Read failed.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Read(
   uint8                                    DeviceIndex,
   uint16                                   BlockNumber,
   uint16                                   BlockOffset,
   P2VAR(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr,
   uint16                                   Length);

/** \brief Mapped to write service of corresponding EEPROM
 ** Abstraction and Flash EEPROM Emulation modules
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \param[in]   BlockNumber Number of logical block in EA or FEE
 ** \param[in]   DataBufferPtr Pointer to data buffer
 **
 ** \return Std_ReturnType
 ** \retval E_OK       Write finished successfully.
 ** \retval E_NOT_OK   Write failed.
 **
 ** \ServiceID{3}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_Write(
   uint8                                    DeviceIndex,
   uint16                                   BlockNumber,
   P2CONST(uint8, AUTOMATIC, MEMIF_APPL_DATA) DataBufferPtr);

/** \brief Calls the cancel function of the underlying modules: either Flash
 ** EEPROM Emulation or EEPROM Abstraction
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \ServiceID{4}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, MEMIF_CODE) MemIf_Cancel(uint8 DeviceIndex);

/** \brief Mapped to service GetJobResult of corresponding EEPROM
 ** Abstraction and Flash EEPROM Emulation modules
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \return job result
 ** \retval MEMIF_JOB_OK              Job finished successfully.
 ** \retval MEMIF_JOB_FAILED          Job failed.
 ** \retval MEMIF_JOB_PENDING         Job is being performed.
 ** \retval MEMIF_JOB_CANCELLED       Job has been cancelled.
 ** \retval MEMIF_BLOCK_INCONSISTENT  Block inconsistent, can't read data.
 ** \retval MEMIF_BLOCK_INVALID       Block marked invalid, can't read data.
 ** \retval MEMIF_COMPARE_UNEQUAL     Block Comparision Unequal
 **
 ** \ServiceID{6}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(MemIf_JobResultType, MEMIF_CODE) MemIf_GetJobResult(
  uint8 DeviceIndex);

/** \brief Mapped to service InvalidateBlock of corresponding EEPROM
 ** Abstraction and Flash EEPROM Emulation modules
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \param[in]   BlockNumber Number of logical block in EA or FEE
 **
 ** \return Std_ReturnType
 ** \retval E_OK      Invalidated Block successfully.
 ** \retval E_NOT_OK  Block Invalidation Failed
 **
 ** \ServiceID{7}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_InvalidateBlock(
   uint8  DeviceIndex,
   uint16 BlockNumber);

/** \brief Mapped to service EraseImmediateBlock of corresponding EEPROM
 ** Abstraction and Flash EEPROM Emulation modules
 **
 ** \pre development error detection is configured
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** \param[in] BlockNumber Number of logical block in EA or FEE
 **
 ** \return Std_ReturnType
 ** \retval E_OK       Block Erased successfully.
 ** \retval E_NOT_OK   Block Erase Failed
 **
 ** \ServiceID{9}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, MEMIF_CODE) MemIf_EraseImmediateBlock(
   uint8  DeviceIndex,
   uint16 BlockNumber);

# else /* MEMIF_DEV_ERROR_DETECT == STD_OFF */

#define MemIf_Read(DeviceIndex, BlockNumber, BlockOffset, DataBufferPtr, Length) \
  MemIf_ReadFctPtr[(DeviceIndex)]((BlockNumber), (BlockOffset), (DataBufferPtr), (Length))

#define MemIf_Write(DeviceIndex, BlockNumber, DataBufferPtr) \
  MemIf_WriteFctPtr[(DeviceIndex)]((BlockNumber),(DataBufferPtr))

#define MemIf_Cancel(DeviceIndex) \
  MemIf_CancelFctPtr[(DeviceIndex)]()

#define MemIf_GetJobResult(DeviceIndex) \
  MemIf_GetJobResultFctPtr[(DeviceIndex)]()

#define MemIf_InvalidateBlock(DeviceIndex, BlockNumber) \
  MemIf_InvalidateBlockFctPtr[(DeviceIndex)]((BlockNumber))

#define MemIf_EraseImmediateBlock(DeviceIndex, BlockNumber) \
  MemIf_EraseImmediateBlockFctPtr[(DeviceIndex)]((BlockNumber))

# endif  /* MEMIF_DEV_ERROR_DETECT */

/** \brief Mapped to service SetMode of all EEPROM Abstraction and Flash
 ** EEPROM Emulation modules
 **
 ** no error detection, because no DeviceIndex
 **
 ** \param[in] Mode Desired mode for the underlying flash or EEPROM driver
 **
 ** \ServiceID{1}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, MEMIF_CODE) MemIf_SetMode(
  MemIf_ModeType Mode);

/** \brief Mapped to service GetStatus of corresponding EEPROM Abstraction and
 ** Flash EEPROM Emulation modules
 **
 ** This implementation is only used, if more than one memory abstraction
 ** module is configured or development error detection is defined.
 **
 ** If the function MemIf_GetStatus is called with the device index denoting
 ** a broadcast to all configured devices (see MemIf036), this module shall
 ** call the "GetStatus" functions of all underlying devices in turn.
 **
 ** \param[in] DeviceIndex is used for selection of memory abstraction modules
 ** (and thus memory devices). If only one memory abstraction module is
 ** configured, the parameter DeviceIndex is ignored.
 **
 ** The device index value ::MEMIF_BROADCAST_ID is used to identify all
 ** underlying devices within one call. This special "broadcast" device ID is
 ** only allowed in the call to MemIf_GetStatus to determine the status of all
 ** underlying abstraction modules and device drivers.
 **
 ** \return Status of memory device
 **
 ** \retval MEMIF_IDLE if all underlying devices have returned this state
 **
 ** \retval MEMIF_UNINIT if at least one device returned this state, all other
 ** returned states shall be ignored
 **
 ** \retval MEMIF_BUSY if at least one configured device returned this state
 ** and no other device returned MEMIF_UNINIT
 **
 ** \retval MEMIF_BUSY_INTERNAL if at least one configured device returned this
 ** state and no other device returned MEMIF_BUSY or MEMIF_UNINIT
 **
 ** \ServiceID{5}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(MemIf_StatusType, MEMIF_CODE) MemIf_GetStatus(
  uint8 DeviceIndex);

#endif  /* MEMIF_NUMBER_OF_DEVICES > 1U */

#if (MEMIF_VERSION_INFO_API == STD_ON)

/** \brief Get version information
 **
 ** This function provides the information to module vendor ID, module ID and
 ** software version major.minor.patch
 **
 ** \pre ::MEMIF_VERSION_INFO_API = STD_ON
 **
 ** \param[out] versioninfo Pointer to standard version information structure.
 **
 ** \ServiceID{8}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, MEMIF_CODE) MemIf_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, MEMIF_APPL_DATA) VersionInfoPtr);

#endif  /* (MEMIF_VERSION_INFO_API == STD_ON) */

#define MEMIF_STOP_SEC_CODE
#include <MemIf_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if (MEMIF_MAXIMUM_NUMBER_OF_DEVICES > 1U)

#define MEMIF_START_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

/** \brief Function pointer array with *_Read() functions */
extern CONST(MemIf_ReadFctPtrType, MEMIF_CONST)
  MemIf_ReadFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

/** \brief Function pointer array with *_Write() functions */
extern CONST(MemIf_WriteFctPtrType, MEMIF_CONST)
  MemIf_WriteFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

/** \brief Function pointer array with *_Cancel() functions */
extern CONST(MemIf_CancelFctPtrType, MEMIF_CONST)
  MemIf_CancelFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

/** \brief Function pointer array with *_GetJobResult() functions */
extern CONST(MemIf_GetJobResultFctPtrType, MEMIF_CONST)
  MemIf_GetJobResultFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

/** \brief Function pointer array with *_InvalidateBlock() functions */
extern CONST(MemIf_InvalidateBlockFctPtrType, MEMIF_CONST)
  MemIf_InvalidateBlockFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

/** \brief Function pointer array with *_EraseImmediateBlock() functions */
extern CONST(MemIf_EraseImmedBlockFctPtrType, MEMIF_CONST)
  MemIf_EraseImmediateBlockFctPtr[MEMIF_MAXIMUM_NUMBER_OF_DEVICES];

#define MEMIF_STOP_SEC_CONST_UNSPECIFIED
#include <MemIf_MemMap.h>

#endif  /* ((MEMIF_NUMBER_OF_DEVICES > 1U)
           || (MEMIF_DEV_ERROR_DETECT == STD_ON)) */

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef MEMIF_H */
/*==================[end of file]===========================================*/
