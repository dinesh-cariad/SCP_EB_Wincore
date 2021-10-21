/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DSP_MEMORY_SERVICES_H
#define DCM_DSP_MEMORY_SERVICES_H

/* !LINKSTO Dcm.Dsn.File.MemoryServices.PublicApi,1 */
/* This file contains the public API of the memory read and write services functions. */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

/*==================[macros]=====================================================================*/

/** \brief Size of AddressAndLengthFormatIdentifier in bytes */
#if (defined DCM_ADDRESSANDLENGTHFORMATID_SIZE)
  #error "DCM_ADDRESSANDLENGTHFORMATID_SIZE is already defined"
#endif
#define DCM_ADDRESSANDLENGTHFORMATID_SIZE   (0x01U)


/** \brief States of read operation for Memory */
#if (defined DCM_MEMORY_ADDRESS_READ_INITIAL)
  #error "DCM_MEMORY_ADDRESS_READ_INITIAL is already defined"
#endif
#define DCM_MEMORY_ADDRESS_READ_INITIAL 0U

#if (defined DCM_MEMORY_ADDRESS_READ_SUBSEQUENT)
  #error "DCM_MEMORY_ADDRESS_READ_SUBSEQUENT is already defined"
#endif
#define DCM_MEMORY_ADDRESS_READ_SUBSEQUENT 1U

#if (defined DCM_MEMORY_ADDRESS_READ_CANCEL)
  #error "DCM_MEMORY_ADDRESS_READ_CANCEL is already defined"
#endif
#define DCM_MEMORY_ADDRESS_READ_CANCEL 2U

#if (defined DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP)
  #error "DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP is already defined"
#endif
#define DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP 3U

#if(DCM_DSP_USE_SERVICE_0X3D == STD_ON)

/** \brief States of write operation for Memory */
#if (defined DCM_MEMORY_ADDRESS_WRITE_INITIAL)
  #error "DCM_MEMORY_ADDRESS_WRITE_INITIAL is already defined"
#endif
#define DCM_MEMORY_ADDRESS_WRITE_INITIAL 0U

#if (defined DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT)
  #error "DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT is already defined"
#endif
#define DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT 1U

#if (defined DCM_MEMORY_ADDRESS_WRITE_CANCEL)
  #error "DCM_MEMORY_ADDRESS_WRITE_CANCEL is already defined"
#endif
#define DCM_MEMORY_ADDRESS_WRITE_CANCEL 2U

#if (defined DCM_MEMORY_ADDRESS_WRITE_CONFIRM_FORCED_RCRRP)
  #error "DCM_MEMORY_ADDRESS_WRITE_CONFIRM_FORCED_RCRRP is already defined"
#endif
#define DCM_MEMORY_ADDRESS_WRITE_CONFIRM_FORCED_RCRRP 3U

#endif /* #if(DCM_DSP_USE_SERVICE_0X3D == STD_ON) */

#if((DCM_DSP_USE_SERVICE_0X34 == STD_ON) || (DCM_DSP_USE_SERVICE_0X35 == STD_ON))
/** \brief Values of a memory operation type */
#if (defined DCM_READMEMORY)
  #error "DCM_READMEMORY is already defined"
#endif
#define DCM_READMEMORY   0U

#if (defined DCM_WRITEMEMORY)
  #error "DCM_WRITEMEMORY is already defined"
#endif
#define DCM_WRITEMEMORY 1U
#endif
/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.Type.Dcm_MemoryAddressReadStateType,1 */
/** \brief Type contains State of current read operation for Memory */
typedef uint8 Dcm_MemoryAddressReadStateType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_ReadMemoryContextType,1 */
/* GENERATED SECTION BEGIN EA:{2DB6E64A-7ABC-4ed7-A9F8-23A652189768} */

/** \brief This contains the context information for executing a reading of a memory address
 *  element **/
typedef struct
{
  /** \brief Memory address from which the data will be read **/
  uint32 MemoryAddress;
  /** \brief Amount of bytes to read from the memory address **/
  uint32 MemorySize;
  /** \brief How much of the buffer is already written at the setup of the operation **/
  uint32 OutputBufferOccupied;
  /** \brief Amount of data the read operation is 0allowed to write to the buffer **/
  uint32 OutputBufferLimit;
  /** \brief Pointer to the output buffer where the memory data shall be read **/
  P2VAR(uint8, TYPEDEF, DCM_VAR) OutputBuffer;
  /** \brief Current state of the reading operation **/
  Dcm_MemoryAddressReadStateType MemoryAddressReadProcessingState;
  /** \brief Nrc to be returned in case the reading operation failed **/
  Dcm_NegativeResponseCodeType NrcConditions;
}  Dcm_ReadMemoryContextType;

/* GENERATED SECTION END EA:{2DB6E64A-7ABC-4ed7-A9F8-23A652189768} */

/* !LINKSTO Dcm.Dsn.Type.Dcm_MemoryAddressWriteStateType,1 */
/** \brief This type contains state of current write operation for Memory */
typedef uint8 Dcm_MemoryAddressWriteStateType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_WriteMemoryContextType,1 */
/* GENERATED SECTION BEGIN EA:{1E104D2A-2D67-41fd-95AD-01546FA005E1} */

/** \brief This contains the context information for executing a writing of a memory address
 *  element **/
typedef struct
{
  /** \brief The memory address from which the data will be write **/
  uint32 MemoryAddress;
  /** \brief The amount of bytes to write from the memory address **/
  uint32 MemorySize;
  /** \brief Pointer to the input buffer from where the Memory data shall be written **/
  P2VAR(uint8, TYPEDEF, DCM_VAR) InputBuffer;
  /** \brief The Nrc to be returned in case the writing operation failed **/
  Dcm_NegativeResponseCodeType NrcConditions;
  /** \brief The current state of the writing operation **/
  Dcm_MemoryAddressWriteStateType MemoryAddressWriteProcessingState;
}  Dcm_WriteMemoryContextType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_MemoryRangeInfoType,1 */
/** \brief  This type contains the configuration information of a MemoryRangeInfo **/
typedef struct
{
  /** \brief Low memory address of a range allowed for reading/writing **/
  uint32 MemoryRangeLow;
  /** \brief High memory address of a range allowed for reading/writing **/
  uint32 MemoryRangeHigh;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  /** \brief Pointer to Mode rule function **/
  Dcm_ModeRuleFunctionType ModeRuleMemFunction;
#endif
  /** \brief Pointer to array of security levels needed for read/write access in this
   *  memory **/
  P2CONST(Dcm_SecLevelType, TYPEDEF, DCM_CONST) MemSecLevels;
  /** \brief Number of security levels for for which this memory read/write access is
   *  provided **/
  uint8 NumMemSecLevels;
}
Dcm_MemoryRangeInfoType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_MemoryIdInfoType,1 */
/** \brief  This type contains the configuration information of all memory ranges **/
typedef struct
{
  /** \brief Pointer to the range of memory address allowed for writing **/
  P2CONST(Dcm_MemoryRangeInfoType, TYPEDEF, DCM_CONST) WriteMemoryRangeInfo;
  /** \brief Pointer to the range of memory address allowed for reading **/
  P2CONST(Dcm_MemoryRangeInfoType, TYPEDEF, DCM_CONST) ReadMemoryRangeInfo;
  /** \brief Number of 'WriteMemoryRangeInfos' configured **/
  uint16 numberofWriteRangeInfo;
  /** \brief Number of 'ReadMemoryRangeInfos' configured **/
  uint16 numberofReadRangeInfo;
  /** \brief Value of memory identifier used to select the desired memory device **/
  uint8 MemoryIDValue;
}
Dcm_MemoryIdInfoType;

/* GENERATED SECTION END EA:{1E104D2A-2D67-41fd-95AD-01546FA005E1} */

#if((DCM_DSP_USE_SERVICE_0X34 == STD_ON) || (DCM_DSP_USE_SERVICE_0X35 == STD_ON))
/** \brief Type of a memory operation: read or write */
typedef uint8 Dcm_MemoryOperationType;
#endif

/*==================[external function declarations]=============================================*/

#if (DCM_DSP_USE_MEMORYSERVICES == STD_ON)

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* GENERATED SECTION BEGIN EA:{C7180693-B34E-4856-AE8F-5423DF70A9BE} */

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON))
/** \brief Performs the Memory overlapping checks for 'Read/Write Memory' services
 ** The function is called by Dcm_Init
 **
 ** \param  none
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}. **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_MemoryServices_Init(void);
#endif /* #if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON))
/** \brief Performs the Memory range validity checks for 'ReadMemory' service and return indexes to
 *  memory range and memory id.
 *
 *  \param memRangeIndex [out] Index to memoryRangeInfo.
 *  \param memIdIndex [out] Index to MemoryIdInfo.
 *  \param readMemAddress [in] Starting Memory Address to be read.
 *  \param readMemSize [in] size of memory to be read.
 *  \return uint8 **/
extern FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_GetRange(
  P2VAR(uint16, AUTOMATIC, DCM_VAR) memRangeIndex,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) memIdIndex,
  uint32 readMemAddress,
  uint32 readMemSize
);
#endif /*#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
              (DCM_DSP_USE_SERVICE_0X2C == STD_ON) ||\
              (DCM_DSP_USE_SERVICE_0X35 == STD_ON))*/

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2C == STD_ON))
/** \brief Sets up the read operation for data residing at a memory address (a DcmDspMemoryIdInfo
 *  element).
 *
 *  \param pReadMemoryContext [in] Pointer to execution context of current process
 *  \param MemoryAddress [in] The address from which to read the data
 *  \param MemorySize [in] How much data to read from this memory address
 *  \param OutputBuffer [in] A pointer to the buffer location where the read data shall
 *  be placed
 *  \param BufferOffset [in] The maximum size of the buffer
 *  \param BufferSize [in] The offset from which it the write is into the buffer starts.
 *  This counts as already occupied in the output buffer (and
 *   therefore adds to the length of the occupied buffer)
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Setup(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferOffset,
  uint32 BufferSize
);

/** \brief Executes the reading of data from a given memory address
 *
 *  \retval DCM_E_DONE  Reading of the DID list has finished successfully
 *  \retval DCM_E_NOT_OK  Reading of the DID list has failed
 *  \retval DCM_E_PENDING  In order to continue reading the list of DIDs this function needs to be
 *  called again within the next processing cycle
 *  \retval DCM_E_FORCE_RCRRP  This status is returned by the Dcm_ReadMemory callout in order to
 *  instruct the DSL that an immediate RequestCorrectlyReceivedResponsePending NRC needs to be
 *  transmitted.
 *
 *  \param pReadMemoryContext [in] Pointer to execution context of current process
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Execute(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext
);

/** \brief Retrieves the results of a memory address read operation.
 *
 *  \retval DCM_E_DONE  Reading of the signal data for this DID has finished successfully
 *  \retval DCM_E_NOT_OK  Reading of the signal data for this DID has failed
 *
 *  \param pReadMemoryContext [in] Pointer to execution context of current process
 *  \param pNrc [out] Nrc In case reading failed, this parameter contains the negative
 *  response code to be sent to the tester as a reason for the failure
 *  \param pBufferOccupied [in] Holds the amount of data now present in the buffer given to the
 *  pReadMemoryContext when the related Dcm_Dsp_MemoryServices_ReadMemory_Setupfunction was
 *  called.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_GetResults(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) pBufferOccupied
);

/** \brief Determines if session security and mode rule access for a memory address are correct.
 *
 *  \retval DCM_E_OK  Conditions are ok
 *  \retval DCM_E_NOT_OK  Conditions are not ok
 *
 *  \param memRangeIndex [in] Index to memoryRangeInfo.
 *  \param memIdIndex [in] Index to memoryRangeInfo.
 *  \param nrc [out] The NRC that occurred due to condition checks, if these failed.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks(
  uint16 memRangeIndex,
  uint16 memIdIndex,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc
);

#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)

/** \brief Cancels a memory address reading operation so that the currently running asynchronous
 *  Dcm_ReadMemory() interface receive a final call with an OpStatus of DCM_CANCEL on the next call
 *  of the related Dcm_Dsp_MemoryServices_ReadMemory_Execute() function.
 *
 *  \param pReadMemoryContext [in] Pointer to the current memory address reading context
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Cancel(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext
);

/** \brief This function must be called in order to confirm the sending of an immediate
 *  RequestCorrectlyReceivedResponsePending NRC to the application implementing the Dcm_ReadMemory
 *  callout.
 *
 *  \param pReadMemoryContext [in] Pointer to execution context of current process
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_ConfirmForcedRCRRP(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext
);
#endif /*#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)*/

#endif /*#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2C == STD_ON))*/

#if((DCM_DSP_USE_SERVICE_0X3D == STD_ON)||(DCM_DSP_USE_SERVICE_0X34 == STD_ON))
/** \brief Performs the Memory range validity checks and returns indexes to
 **  memory range and memory id.
 **
 ** \param[out] memRangeIndex Index to memoryRangeInfo.
 ** \param[out] memIdIndex Index to MemoryIdInfo.
 ** \param[in]  writeMemAddress Starting Memory Address to be write.
 ** \param[in]  writeMemSize Size of memory to be write.
 **
 */
extern FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_GetRange(
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) memRangeIndex,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) memIdIndex,
  uint32 writeMemAddress, uint32 writeMemSize);
#endif /*#if((DCM_DSP_USE_SERVICE_0X3D == STD_ON)||(DCM_DSP_USE_SERVICE_0X34 == STD_ON))*/

#if (DCM_DSP_USE_SERVICE_0X3D == STD_ON)
/** \brief Executes the writing of data from a given memory address.
 *
 *  \retval DCM_E_OK  Writing to a memory address has finished successfully.
 *  \retval DCM_E_NOT_OK  Writing to a memory address has failed.
 *  \retval DCM_E_PENDING  In order to continue writing a memory address this function needs to be
 *  called again within the next processing cycle.
 *  \retval DCM_E_FORCE_RCRRP  This status is returned by the Dcm_WriteMemory callout in order to
 *  instruct the DSL that an immediate RequestCorrectlyReceivedResponsePending NRC needs to be
 *  transmitted.
 *
 *  \param pWriteMemoryContext [in] Pointer to the current memory address writing context.
 *  \return uint8 **/
extern FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Execute(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext
);

/** \brief Sets up the write operation for data residing at a memory address
 *
 *  \param pWriteMemoryContext [in] Pointer to the current memory address writing context.
 *  \param MemoryAddress [in] The address from which to write the data.
 *  \param MemorySize [in] How much data to write from this memory address.
 *  \param InputBuffer [in] A pointer to the input buffer.
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Setup(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InputBuffer
);

/** \brief Cancels a memory address writing operation and set the ProcessingState to
 *  DCM_MEMORY_ADDRESS_WRITE_CANCEL so that the currently running asynchronous Dcm_WriteMemory()
 *  interface receive a final call with an OpStatus of DCM_CANCEL on the next call of the related
 *  Dcm_Dsp_WriteMemoryByAddress_Execute() function
 *
 *  \param pWriteMemoryContext [in] Pointer to the current memory address writing context.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Cancel(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext
);

/** \brief Retrieves the results of a memory address write operation.
 *
 *  \retval DCM_E_OK  Writing to a memory address has finished successfully.
 *  \retval DCM_E_NOT_OK  Writing to a memory address has finished has failed.
 *
 *  \param NrcConditions [in] NRC value from write context of current process.
 *  \param pNrc [out] In case write failed, this parameter contains the negative
 *  response code to be sent to the tester as a reason for the failure.
 *  \return uint8 **/
extern FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_GetResults(
  Dcm_NegativeResponseCodeType NrcConditions,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc
);

/** \brief This function must be called in order to confirm the sending of an immediate
 *  RequestCorrectlyReceivedResponsePending NRC to the application implementing the Dcm_WriteMemory
 *  callout
 *
 *  \param pWriteMemoryContext [in] Pointer to the current memory address writing context
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_ConfirmForcedRCRRP(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext
);

#endif /* DCM_DSP_USE_SERVICE_0X3D == STD_ON */

#if ((DCM_DSP_USE_SERVICE_0X34 == STD_ON)||(DCM_DSP_USE_SERVICE_0X35 == STD_ON))

#if (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)
/** \brief Determines if the address and size combination exists and if the
 *  operation is allowed in the current security level.
 *
 *  \param MemoryAddress [in] Requested MemoryAddress
 *  \param MemorySize    [in] Requested MemorySize
 *  \param Operation     [in] The requested operation: read or write
 *  \param Nrc           [out] Stores the returned nrc.
 *
 *  \retval DCM_E_OK        MemoryRange and security checks are successful
 *  \retval DCM_E_NOT_OK    MemoryRange and security checks are not successful
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_RangeAndSecurityCheck(
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MemoryOperationType Operation,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc
);
#endif

#if (DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK == STD_ON)
/** \brief Determines if the requested memory Id is configured
 *
 *  \retval DCM_E_OK        The memory Id is configured
 *  \retval DCM_E_NOT_OK    The memory Id is not configured
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_MemoryIdCheck(
  uint8 MemoryId
);
#endif

#endif /*((DCM_DSP_USE_SERVICE_0X34 == STD_ON)||(DCM_DSP_USE_SERVICE_0X35 == STD_ON))*/

#if (DCM_DSP_USE_GETMEMORYADDRESSANDSIZE == STD_ON)
 /** \brief Extracts the Memory address and the size of memory to be read
   **
   ** \param[in] reqData Pointer to 'addressAndLengthFormatIdentifier'.
   ** \param[in] memAddLength length (In bytes) of the memoryAddress parameter.
   ** \param[in] memSizeLength length (In bytes) of the memorySize parameter.
   ** \param[out] memAddress Starting Memory Address to be read.
   ** \param[out] memSize size of memory to be read.
   **
   ** \retval DCM_E_OK                Conditions are ok
   ** \retval DCM_E_NOT_OK            Conditions are not ok
   */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) reqData,
  uint8 memAddLength, uint8 memSizeLength,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) memAddress,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) memSize );
#endif /* (DCM_DSP_USE_GETMEMORYADDRESSANDSIZE == STD_ON) */

#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)

/** \brief Handles verification of AddressLengthIdentifier in the request.
 **
 ** \param[in]    AddressLengthIdentifier holds number of bytes occupied by memoryAddress[]
 **               and memorySize[] in the request
 **
 ** \retval DCM_E_OK                Operation Success
 ** \retval DCM_E_NOT_OK            Operation Failed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(
  uint8 AddressLengthIdentifier);

#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */

/* GENERATED SECTION END   EA:{C7180693-B34E-4856-AE8F-5423DF70A9BE} */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_NUM_MEMORY_IDINFO > 0U)
/** \brief Global array holding the configuration information for Writing/Reading memory */
extern CONST(Dcm_MemoryIdInfoType, DCM_CONST)
  Dcm_MemoryIdInfoAndIdValueTable[DCM_NUM_MEMORY_IDINFO];
#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if (DCM_DSP_USE_MEMORYSERVICES == STD_ON) */

#endif /* ifndef DCM_DSP_DATATRANSFER_SERVICES_H */
/*==================[end of file]================================================================*/
