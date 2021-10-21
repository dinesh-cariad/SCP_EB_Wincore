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

/* !LINKSTO Dcm.Dsn.File.MemoryServices.Impl,1 */
/* This file contains the implementation of memory read and write service functions. */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  */
/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */
/*===============================[includes]======================================================*/

#include <Dcm_Dsp_MemoryServices.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>
#if (DCM_DSP_USE_MEMORYSERVICES == STD_ON)

#include <Dcm_Trace.h>

/*==================[macro definitions]==========================================================*/

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ((((DCM_DSP_USE_SERVICE_0X34 == STD_ON) || (DCM_DSP_USE_SERVICE_0X35 == STD_ON)) && \
     (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)) || \
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON))

/** \brief Determines if the security level is correct for the desired request
 **
 ** \param[in] MemoryAddressTypePtr A pointer to the memory configuration
 **
 ** \retval DCM_E_OK                Conditions are ok
 ** \retval DCM_E_NOT_OK            Conditions are not ok
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_CheckSecurity(
  P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_CONST) MemoryAddressTypePtr);
#endif

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON))
#if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U))
#if (DCM_DEV_ERROR_DETECT == STD_ON)
/** \brief Determines if there is overlap between two ranges
 **
 ** \param[in] LowValueRange1 the lower limit of the first range
 ** \param[in] HighValueRange1 the higher limit of the first range
 ** \param[in] LowValueRange2 the lower limit of the second range
 ** \param[in] HighValueRange2 the higher limit of the second range
 **
 ** \retval FALSE                There is no overlap
 ** \retval TRUE                 There is overlap
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_CompareRanges(uint32 LowValueRange1, uint32 HighValueRange1,
                                              uint32 LowValueRange2, uint32 HighValueRange2);

#if(DCM_MAX_NUM_READ_RANGES != 0U)
/** \brief Performs check of overlapping on read regions
 ** \param none
 **
 ** \retval none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_CheckReadRanges(void);
#endif /* #if(DCM_MAX_NUM_READ_RANGES != 0U)*/

#if(DCM_MAX_NUM_WRITE_RANGES != 0U)
/** \brief Performs check of overlapping on write regions
 ** \param none
 **
 ** \retval none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_CheckWriteRanges(void);
#endif /* #if(DCM_MAX_NUM_WRITE_RANGES != 0U) */

#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#endif /* #if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U)) */
#endif /* #if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[external constants declarations]============================================*/

/*===================[internal constants declarations]===========================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON))
/** \brief Mutex controlling access to memory read */
/* !LINKSTO Dcm.Dsn.MemoryServices.RdLock,1 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_MutexType, DCM_VAR ) Dcm_Dsp_MemoryServices_ReadLock = DCM_MUTEX_UNLOCKED;
#endif /*((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON))*/

#if(DCM_DSP_USE_SERVICE_0X3D == STD_ON)
/** \brief Mutex controlling access to memory write */
/* !LINKSTO Dcm.Dsn.MemoryServices.WrLock,1 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_MutexType, DCM_VAR ) Dcm_Dsp_MemoryServices_WriteLock = DCM_MUTEX_UNLOCKED;
#endif /* #if(DCM_DSP_USE_SERVICE_0X3D == STD_ON) */

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON))
#if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U))
#if (DCM_DEV_ERROR_DETECT == STD_ON)
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsl_CompareRanges(uint32 LowValueRange1, uint32 HighValueRange1,
                                              uint32 LowValueRange2, uint32 HighValueRange2)
{
  boolean Overlap = TRUE;
  /* !LINKSTO Dcm.EB.WrongMemoryRangeConstruction_DET,1 */
  if(LowValueRange1 < HighValueRange1)
  {
    /* Compare the ranges only if the range to compare is in the proper order */
    if(
        (
          /* range 1 is below both boundaries of range 2*/
          (
            (LowValueRange1 < LowValueRange2)&&(LowValueRange1 < HighValueRange2)
          &&(HighValueRange1 < LowValueRange2)&&(HighValueRange1 < HighValueRange2)
          )
          ||
          /* range 1 is adobe both boundaries of range 2*/
          (
              (LowValueRange1 > LowValueRange2)&&(LowValueRange1 > HighValueRange2)
            &&(HighValueRange1 > LowValueRange2)&&(HighValueRange1 > HighValueRange2)
          )
        )
      )
    {
      Overlap = FALSE;
    }
  }
  else
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_RANGE_WITH_WRONG_LIMITS);
  }
  return Overlap;
}
#if(DCM_MAX_NUM_READ_RANGES != 0U)
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_CheckReadRanges(void)
{
  uint8_least CurrentMemId = 0U;
  uint8_least CurrentRange = 0U;
  uint8_least MemIdToCompareWith = 0U;
  uint8_least RangeToCompareWith = 0U;
  boolean SkipRange = FALSE;
  boolean InterruptComparison = FALSE;
  uint32 CurrentReadLowLimit;
  uint32 CurrentReadHighLimit;

  /* Loop on all memory tables */
  /* Deviation TASKING-1 */
  for (CurrentMemId = 0U; (CurrentMemId < DCM_NUM_MEMORY_IDINFO); CurrentMemId++)
  {
    /* Evaluate current Read range against the other Read ranges on the table */
    for (CurrentRange = 0U;
         (CurrentRange < Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].numberofReadRangeInfo);
         CurrentRange++)
    {
      /* Range to perform the comparison on the complete range */
      CurrentReadLowLimit = (Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].ReadMemoryRangeInfo[CurrentRange]).MemoryRangeLow;
      CurrentReadHighLimit = (Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].ReadMemoryRangeInfo[CurrentRange]).MemoryRangeHigh;

      /* Perform comparison with all other ranges of all MemIds */
      for (MemIdToCompareWith = CurrentMemId; (MemIdToCompareWith < DCM_NUM_MEMORY_IDINFO); MemIdToCompareWith++)
      {
        for (RangeToCompareWith = CurrentRange;
             (RangeToCompareWith < Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].numberofReadRangeInfo);
             RangeToCompareWith++)
        {
          if((CurrentMemId == MemIdToCompareWith)&&(CurrentRange == RangeToCompareWith))
          {
            /* Do not compare ranges to themselves */
            SkipRange = TRUE;
          }
          else
          {
            SkipRange = FALSE;
          }
          if(SkipRange == FALSE)
          {
            if(Dcm_Dsl_CompareRanges(
                    CurrentReadLowLimit,
                    CurrentReadHighLimit,
                    (Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].ReadMemoryRangeInfo[RangeToCompareWith]).MemoryRangeLow,
                    (Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].ReadMemoryRangeInfo[RangeToCompareWith]).MemoryRangeHigh)
               )
            {
              /* !LINKSTO SWS_Dcm_01158_DET_condition,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_MEMORY_OVERLAP);
              InterruptComparison = TRUE;
              break;
            }
          }
        }
        if(InterruptComparison == TRUE)
        {
          break;
        }
      }
      if(InterruptComparison == TRUE)
      {
        break;
      }
    }
    if(InterruptComparison == TRUE)
    {
      break;
    }
  }
}
#endif /* #if(DCM_MAX_NUM_READ_RANGES != 0U) */
#if(DCM_MAX_NUM_WRITE_RANGES != 0U)
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_CheckWriteRanges(void)
{
  uint8_least CurrentMemId = 0U;
  uint8_least CurrentRange = 0U;
  uint8_least MemIdToCompareWith = 0U;
  uint8_least RangeToCompareWith = 0U;
  boolean SkipRange = FALSE;
  boolean InterruptComparison =FALSE;
  uint32 CurrentWriteLowLimit;
  uint32 CurrentWriteHighLimit;

  /* Loop on all memory tables */
  /* Deviation TASKING-1 */
  for (CurrentMemId = 0U; (CurrentMemId < DCM_NUM_MEMORY_IDINFO); CurrentMemId++)
  {
    /* Evaluate current Write range against the other Write ranges on the table */
    for (CurrentRange = 0U;
         (CurrentRange < Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].numberofWriteRangeInfo);
         CurrentRange++)
    {
      /* Range to perform the comparison on the complete range */
      CurrentWriteLowLimit = (Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].WriteMemoryRangeInfo[CurrentRange]).MemoryRangeLow;
      CurrentWriteHighLimit = (Dcm_MemoryIdInfoAndIdValueTable[CurrentMemId].WriteMemoryRangeInfo[CurrentRange]).MemoryRangeHigh;

      /* Perform comparison with all other ranges of all MemIds */
      for (MemIdToCompareWith = CurrentMemId; (MemIdToCompareWith < DCM_NUM_MEMORY_IDINFO); MemIdToCompareWith++)
      {
        for (RangeToCompareWith = CurrentRange;
             (RangeToCompareWith < Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].numberofWriteRangeInfo);
             RangeToCompareWith++)
        {
          if((CurrentMemId == MemIdToCompareWith)&&(CurrentRange == RangeToCompareWith))
          {
            /* Do not compare ranges to themselves */
            SkipRange = TRUE;
          }
          else
          {
            SkipRange = FALSE;
          }
          if(SkipRange == FALSE)
          {
            if(Dcm_Dsl_CompareRanges(
                    CurrentWriteLowLimit,
                    CurrentWriteHighLimit,
                    (Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].WriteMemoryRangeInfo[RangeToCompareWith]).MemoryRangeLow,
                    (Dcm_MemoryIdInfoAndIdValueTable[MemIdToCompareWith].WriteMemoryRangeInfo[RangeToCompareWith]).MemoryRangeHigh)
               )
            {
              /* !LINKSTO SWS_Dcm_01056_DET_condition,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_MEMORY_OVERLAP);
              InterruptComparison = TRUE;
              break;
            }
          }
        }
        if(InterruptComparison == TRUE)
        {
          break;
        }
      }
      if(InterruptComparison == TRUE)
      {
        break;
      }
    }
    if(InterruptComparison == TRUE)
    {
      break;
    }
  }
}
#endif /* #if(DCM_MAX_NUM_WRITE_RANGES != 0U) */
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#endif /* #if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U)) */
#if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U))

#if (DCM_DEV_ERROR_DETECT == STD_ON)
FUNC(void, DCM_CODE) Dcm_Dsl_MemoryServices_Init(void)
{
#if(DCM_MAX_NUM_READ_RANGES != 0U)
  Dcm_Dsl_CheckReadRanges();
#endif /* #if(DCM_MAX_NUM_READ_RANGES != 0U) */
#if(DCM_MAX_NUM_WRITE_RANGES != 0U)
  Dcm_Dsl_CheckWriteRanges();
#endif /* #if(DCM_MAX_NUM_WRITE_RANGES != 0U) */
}

#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#endif/* #if((DCM_MAX_NUM_READ_RANGES != 0U)||(DCM_MAX_NUM_WRITE_RANGES != 0U)) */
#endif /* #if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */
#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON))
FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_GetRange(
  P2VAR(uint16, AUTOMATIC, DCM_VAR) memRangeIndex,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) memIdIndex,
  uint32 readMemAddress, uint32 readMemSize)
{
  uint8 RetVal = DCM_E_NOT_OK;
  uint8_least IdIndex = 0U;
  uint8_least RangeIndex = 0U;
  uint32 LocalMemSize = readMemSize;

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_ENTRY(
    memRangeIndex, memIdIndex, readMemAddress, readMemSize);
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
  if(LocalMemSize == 0U)
  {
    /* If the NRC policy is following ISO 2006, an artificial size of 1 byte
     * is forced on the size to obtain a positive response */
    /* !LINKSTO SWS_Dcm_00493.MemRead.ZeroSize.ISO_2006,2 */
    LocalMemSize = 1U;
  }
#endif
  /* !LINKSTO SWS_Dcm_00493.MemRead.ZeroSize.ISO_2013,1 */
  /* !LINKSTO SWS_Dcm_00493.MemRead.NonZeroSize.UpperLimitInclusion,1 */
  /* !LINKSTO SWS_Dcm_00493.MemRead.NonZeroSize.UpperLimitExclusion,1 */
  if(LocalMemSize != 0U)
  {
    /* Check if Read Memory is supported in the requested address range */
    for (IdIndex = 0U; ((IdIndex < DCM_NUM_MEMORY_IDINFO) && (RetVal != DCM_E_OK)); ++IdIndex)
    {
      for (RangeIndex = 0U;
           RangeIndex < Dcm_MemoryIdInfoAndIdValueTable[IdIndex].numberofReadRangeInfo;
           RangeIndex++)
      {
        DCM_PRECONDITION_ASSERT(
          (Dcm_MemoryIdInfoAndIdValueTable[IdIndex].ReadMemoryRangeInfo != NULL_PTR),
           DCM_INTERNAL_API_ID);
        if ((readMemAddress >=
             (Dcm_MemoryIdInfoAndIdValueTable[IdIndex]
              .ReadMemoryRangeInfo[RangeIndex]).MemoryRangeLow) &&
#if (DCM_EXCLUDE_UPPER_LIMITS == STD_ON)
            (readMemAddress <
             (Dcm_MemoryIdInfoAndIdValueTable[IdIndex]
              .ReadMemoryRangeInfo[RangeIndex]).MemoryRangeHigh) &&
#else
            (readMemAddress <=
             (Dcm_MemoryIdInfoAndIdValueTable[IdIndex]
              .ReadMemoryRangeInfo[RangeIndex]).MemoryRangeHigh) &&
#endif
            (((readMemAddress + LocalMemSize) - 1U) <=
             (Dcm_MemoryIdInfoAndIdValueTable[IdIndex]
              .ReadMemoryRangeInfo[RangeIndex]).MemoryRangeHigh))
        {
          *memIdIndex = (uint16)IdIndex;
          *memRangeIndex = (uint16)RangeIndex;
          RetVal = DCM_E_OK;
          break;
        }
      }
    }
  }
  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_EXIT(
    RetVal, memRangeIndex, memIdIndex, readMemAddress, readMemSize);

  return RetVal;
}

#endif /*#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
              (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
              (DCM_DSP_USE_SERVICE_0X35 == STD_ON))*/

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON))
FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Setup(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferOffset,
  uint32 BufferSize)
{
  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_ENTRY(pReadMemoryContext,MemoryAddress,
    MemorySize,OutputBuffer,BufferOffset,BufferSize);

  pReadMemoryContext->MemoryAddress = MemoryAddress;
  pReadMemoryContext->MemorySize = MemorySize;
  pReadMemoryContext->OutputBuffer = OutputBuffer;
  pReadMemoryContext->OutputBufferOccupied = BufferOffset;
  pReadMemoryContext->OutputBufferLimit = BufferSize;

  pReadMemoryContext->NrcConditions = DCM_E_OK;

  pReadMemoryContext->MemoryAddressReadProcessingState
    = DCM_MEMORY_ADDRESS_READ_INITIAL;

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_EXIT(pReadMemoryContext, MemoryAddress,
    MemorySize,OutputBuffer,BufferOffset,BufferSize);
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Execute(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_ReturnReadMemoryType ReadMemoryStatus = DCM_READ_FAILED;
  boolean ReadMemoryCalled = FALSE;

  /* Variable holding the index of MemoryRangeInfo table */
  uint16 memRangeIndex = 0U;
  /* Variable holding the index of MemoryIdInfo table */
  uint16 memIdIndex = 0U;

  Std_ReturnType retVal;
  /* Perform checks to ensure that requested Memory falls within 'configured ranges' */
  retVal = Dcm_Dsp_MemoryServices_ReadMemory_GetRange(&memRangeIndex,
                                                 &memIdIndex,
                                                 pReadMemoryContext->MemoryAddress,
                                                 pReadMemoryContext->MemorySize);

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_ENTRY(pReadMemoryContext);

  switch (pReadMemoryContext->MemoryAddressReadProcessingState)
  {
    case DCM_MEMORY_ADDRESS_READ_INITIAL:
    {
      /* Check if adequate buffer is configured to read the data */
      if ((pReadMemoryContext->OutputBufferOccupied
           + pReadMemoryContext->MemorySize
           )
           <= pReadMemoryContext->OutputBufferLimit
          )
      {
        Dcm_NegativeResponseCodeType localNrc = DCM_E_OK;

        if (retVal != DCM_E_NOT_OK)
        {
            retVal =
              Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks(
                memRangeIndex,
                memIdIndex,
                &localNrc);

          if (DCM_E_OK == retVal)
          {
            /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.ReadDID.Concurrency,1 */
            const Std_ReturnType CurrentRdLockStatus
              = Dcm_Internal_GetLock(&Dcm_Dsp_MemoryServices_ReadLock);

            if (CurrentRdLockStatus != DCM_E_RESOURCE_LOCKED)
            {
            /* Call the Dcm_ReadMemory() callouts with the MemoryIdentifier parameter */
            /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadFailed.AUTOSAR430,1 */
            /* !LINKSTO Dcm.EB.ReadDataById.ReadMemoryRangeSourceForDynamicDIDFailed.AUTOSAR430,1 */
              ReadMemoryStatus =
                Dcm_ReadMemory(DCM_INITIAL,
                             Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
                             pReadMemoryContext->MemoryAddress,
                             pReadMemoryContext->MemorySize,
                             &(pReadMemoryContext->OutputBuffer
                               [pReadMemoryContext->OutputBufferOccupied])
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
                             ,&(pReadMemoryContext->NrcConditions)
#endif
                             );
              ReadMemoryCalled = TRUE;
            }
            else
            {
              ReadMemoryStatus = DCM_E_RESOURCE_LOCKED;
            }
          }
          else
          {
            pReadMemoryContext->NrcConditions = localNrc;
            serviceProcessStatus = DCM_E_DONE;
          }
        }
        else
        {
          /* Memory Range Check failed; Set NRC */
          pReadMemoryContext->NrcConditions = DCM_E_REQUESTOUTOFRANGE;
          serviceProcessStatus = DCM_E_DONE;
        }
      }
      else
      {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_INTERFACE_BUFFER_OVERFLOW);
#endif
        pReadMemoryContext->NrcConditions = DCM_E_REQUESTOUTOFRANGE;
        serviceProcessStatus = DCM_E_DONE;
      }

      /* Only move to next read processing state if the initial one was actually
       * executed */
      if (ReadMemoryStatus != DCM_E_RESOURCE_LOCKED)
      {
        pReadMemoryContext->MemoryAddressReadProcessingState
          = DCM_MEMORY_ADDRESS_READ_SUBSEQUENT;
      }
    }
    break;

    case DCM_MEMORY_ADDRESS_READ_SUBSEQUENT:
    {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_ReadMemory() callouts with all In-parameters set to 0x0 */
      ReadMemoryStatus = Dcm_ReadMemory(DCM_PENDING,
                                        0U,
                                        0U,
                                        0U,
                                        &(pReadMemoryContext->OutputBuffer[
                                          pReadMemoryContext->OutputBufferOccupied]));
#else
      /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.ReadMemoryRangeSourceForDynamicDIDFailed.AUTOSAR430,1 */
      ReadMemoryStatus =
        Dcm_ReadMemory(DCM_PENDING,
                     Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
                     pReadMemoryContext->MemoryAddress,
                     pReadMemoryContext->MemorySize,
                     &(pReadMemoryContext->OutputBuffer
                       [pReadMemoryContext->OutputBufferOccupied]),
                     &(pReadMemoryContext->NrcConditions));
#endif
      ReadMemoryCalled = TRUE;
    }
    break;

    case DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP:
    {

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_ReadMemory() callouts with all In-parameters set to 0x0 */
      ReadMemoryStatus = Dcm_ReadMemory(DCM_FORCE_RCRRP_OK,
                                        0U,
                                        0U,
                                        0U,
                                        &(pReadMemoryContext->OutputBuffer[
                                          pReadMemoryContext->OutputBufferOccupied]));
#else
      /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.ReadMemoryRangeSourceForDynamicDIDFailed.AUTOSAR430,1 */
      ReadMemoryStatus =
        Dcm_ReadMemory(DCM_FORCE_RCRRP_OK,
                     Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
                     pReadMemoryContext->MemoryAddress,
                     pReadMemoryContext->MemorySize,
                     &(pReadMemoryContext->OutputBuffer
                       [pReadMemoryContext->OutputBufferOccupied]),
                     &(pReadMemoryContext->NrcConditions));
#endif

      pReadMemoryContext->MemoryAddressReadProcessingState
        = DCM_MEMORY_ADDRESS_READ_SUBSEQUENT;

      ReadMemoryCalled = TRUE;
    }
    break;

    case DCM_MEMORY_ADDRESS_READ_CANCEL:
    {

#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_ReadMemory() callouts with all In-parameters set to 0x0 */
      ReadMemoryStatus = Dcm_ReadMemory(DCM_CANCEL,
                                        0U,
                                        0U,
                                        0U,
                                        &(pReadMemoryContext->OutputBuffer[
                                          pReadMemoryContext->OutputBufferOccupied]));
#else
      /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.ReadMemoryRangeSourceForDynamicDIDFailed.AUTOSAR430,1 */
      ReadMemoryStatus =
        Dcm_ReadMemory(DCM_CANCEL,
                     Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
                     pReadMemoryContext->MemoryAddress,
                     pReadMemoryContext->MemorySize,
                     &(pReadMemoryContext->OutputBuffer
                       [pReadMemoryContext->OutputBufferOccupied]),
                     &(pReadMemoryContext->NrcConditions));
#endif

      ReadMemoryCalled = TRUE;
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Mandatory default; defensive branch. */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READMEMORY);
    }
    break;
    /* CHECK: PARSE */
  }

  /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.ReadDID.Concurrency,1 */
  if (DCM_CANCEL == pReadMemoryContext->MemoryAddressReadProcessingState)
  {
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    if (serviceProcessStatus != DCM_E_DONE)
    {
      switch (ReadMemoryStatus)
      {
        case DCM_READ_OK:
          /* Read Memory successful; Update the response data length */
          pReadMemoryContext->OutputBufferOccupied +=
            pReadMemoryContext->MemorySize;
          /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReturnOK.NotNRC0x00.ASR430,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReturnOK.NotNRC0x00.ASR430,1 */
          pReadMemoryContext->NrcConditions = DCM_E_OK;
          serviceProcessStatus = DCM_E_DONE;
          break;

        case DCM_READ_FORCE_RCRRP:
          /* !LINKSTO Dcm.EB.ReadMemoryByAddress.DCM_READ_FORCE_RCRRP,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.DCM_READ_FORCE_RCRRP,1 */
          serviceProcessStatus = DCM_E_FORCE_RCRRP;
          break;

        case DCM_READ_PENDING:
          serviceProcessStatus = DCM_E_PENDING;
          break;

        case DCM_READ_FAILED:
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
          /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadFailed.AUTOSAR403,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReadFailed.AUTOSAR403,1 */
          pReadMemoryContext->NrcConditions = DCM_E_GENERALREJECT;
#endif
          /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReturnNOK.NRC0x00.ASR430,1 */
          /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReturnNOK.NRC0x00.ASR430,1 */
          if ((pReadMemoryContext->NrcConditions) == 0x00U)
          {
            pReadMemoryContext->NrcConditions = DCM_E_CONDITIONSNOTCORRECT;
          }

          serviceProcessStatus = DCM_E_NOT_OK;
          break;

        case DCM_E_RESOURCE_LOCKED:
          serviceProcessStatus = DCM_E_RESOURCE_LOCKED;
          break;

        default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_READMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
#endif
          pReadMemoryContext->NrcConditions = DCM_E_CONDITIONSNOTCORRECT;
          serviceProcessStatus = DCM_E_NOT_OK;
          break;
      }
    }
  }

  /* Release the read mutex if the read operation is finished */
  if ((ReadMemoryCalled == TRUE)
      && ((serviceProcessStatus == DCM_E_DONE)
         || (serviceProcessStatus == DCM_E_NOT_OK)
         )
     )
  {
    Dcm_Internal_ReleaseLock(&Dcm_Dsp_MemoryServices_ReadLock);
  }

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_EXIT(serviceProcessStatus,
    pReadMemoryContext);
  return serviceProcessStatus;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_GetResults(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) pBufferOccupied)
{
  Std_ReturnType operationResult;

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_ENTRY(pReadMemoryContext, pNrc,
    pBufferOccupied);

  *pBufferOccupied = pReadMemoryContext->OutputBufferOccupied;

  if (DCM_E_OK != pReadMemoryContext->NrcConditions)
  {
    operationResult = DCM_E_NOT_OK;
    *pNrc = pReadMemoryContext->NrcConditions;
  }
  else
  {
    operationResult = DCM_E_DONE;
  }

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_EXIT(operationResult, pReadMemoryContext,
  pNrc, pBufferOccupied);
  return operationResult;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks(
  uint16 memRangeIndex,
  uint16 memIdIndex,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc)
{
  Dcm_NegativeResponseCodeType localNrc = DCM_E_OK;

  Std_ReturnType ReturnVal = DCM_E_NOT_OK;

  /* Holds the context information For Read memory by address service */
  P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_CONST) pReadMemoryAddressType =
    &Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].ReadMemoryRangeInfo[memRangeIndex];

  DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_ENTRY(memRangeIndex, memIdIndex, nrc);

  /* All calls of Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks are made after a
   * successful check via Dcm_Dsp_MemoryServices_ReadMemory_GetRange which ensures
   * that the ReadMemory pointer is not NULL */
  DCM_PRECONDITION_ASSERT((pReadMemoryAddressType != NULL_PTR), DCM_INTERNAL_API_ID);

  /* Check for security access , If the complete memory range can be read in the
   * current security level
   */
  /* !LINKSTO SWS_Dcm_00494.ReadByAddress.SecLevel,1 */
  /* !LINKSTO SWS_Dcm_00494.ReadByLabel.SecLevel,1 */
  if (pReadMemoryAddressType->NumMemSecLevels > 0U)
  {
    /* get current security level */
    Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    if (curSecLevel != DCM_SEC_LEV_LOCKED)
    {
      uint8_least secLevIdx;
      /* Check for security level */
      for (secLevIdx = 0U;
           ((secLevIdx < pReadMemoryAddressType->NumMemSecLevels)&&(ReturnVal != DCM_E_OK));
           ++secLevIdx)
      {
        if (curSecLevel == pReadMemoryAddressType->MemSecLevels[secLevIdx])
        {
          ReturnVal = DCM_E_OK;
        }
      }
    }
  }
  else
  {
    ReturnVal = DCM_E_OK;
  }

  if (DCM_E_OK != ReturnVal)
  {
    /* Security Access denied; Set NRC */
    localNrc = DCM_E_SECURITYACCESSDENIED;
  }
  else
  {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    /* !LINKSTO SWS_Dcm_00826.ReadByAddress.ModCond,1 */
    /* !LINKSTO SWS_Dcm_00826.ReadByLabel.ModCond,1 */
    if (pReadMemoryAddressType->ModeRuleMemFunction != NULL_PTR)
    {
      /* invoke the mode rule function for the service
       * if the mode rule resulted in failure curNrc will have the calculated nrc,
       * else zero which implies there is no failure */
      pReadMemoryAddressType->ModeRuleMemFunction(&localNrc);

      if (localNrc != DCM_E_OK)
      {
        ReturnVal = DCM_E_NOT_OK;
      }
    }
#endif /*DCM_MODEDECLARATION_SUPPORT */
  }

  *nrc = localNrc;

  DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_EXIT(
    ReturnVal,
    memRangeIndex,
    memIdIndex,
    nrc);

  return ReturnVal;
}

#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_Cancel(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext
)
{
  Std_ReturnType result;

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_ENTRY(pReadMemoryContext);

  /* update the OpStatus to CANCEL */
  pReadMemoryContext->MemoryAddressReadProcessingState = DCM_MEMORY_ADDRESS_READ_CANCEL;

  /* let the current async state cancel the operation */
  result = Dcm_Dsp_MemoryServices_ReadMemory_Execute(pReadMemoryContext);

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_EXIT(result, pReadMemoryContext);

  return result;
}

FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_ReadMemory_ConfirmForcedRCRRP(
  P2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) pReadMemoryContext)
{
  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_ENTRY(pReadMemoryContext);

  pReadMemoryContext->MemoryAddressReadProcessingState =
    DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP;

  DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_EXIT(pReadMemoryContext);
}
#endif /*#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)*/

#endif /*#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) || (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON))*/

#if(DCM_DSP_USE_SERVICE_0X3D == STD_ON)

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Cancel(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext
)
{
  Std_ReturnType result;

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_ENTRY(pWriteMemoryContext);

  /* update the OpStatus to CANCEL */
  pWriteMemoryContext->MemoryAddressWriteProcessingState = DCM_MEMORY_ADDRESS_WRITE_CANCEL;

  /* let the current async state cancel the operation */
  result = Dcm_Dsp_MemoryServices_WriteMemory_Execute(pWriteMemoryContext);

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_EXIT(result, pWriteMemoryContext);

  return result;
}

FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_ConfirmForcedRCRRP(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext
)
{
  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_ENTRY(pWriteMemoryContext);

  pWriteMemoryContext->MemoryAddressWriteProcessingState =
    DCM_MEMORY_ADDRESS_WRITE_CONFIRM_FORCED_RCRRP;

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_EXIT(pWriteMemoryContext);
}

FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Execute(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_ReturnWriteMemoryType writeMemoryStatus = DCM_WRITE_FAILED;
  Std_ReturnType retVal = DCM_E_NOT_OK;
  boolean WriteMemoryCalled = FALSE;

  /* Variable holding the index of MemoryRangeInfo table */
  uint16 memRangeIndex = 0U;
  /* Variable holding the index of MemoryIdInfo table */
  uint16 memIdIndex = 0U;
  /* Perform checks to ensure that requested Memory falls within
    * 'configured ranges' */
  retVal = Dcm_Dsp_MemoryServices_WriteMemory_GetRange(&memRangeIndex, &memIdIndex,
          pWriteMemoryContext->MemoryAddress, pWriteMemoryContext->MemorySize);

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_ENTRY(pWriteMemoryContext);

  switch(pWriteMemoryContext->MemoryAddressWriteProcessingState)
  {
    case DCM_MEMORY_ADDRESS_WRITE_INITIAL:
    {
      if(retVal != DCM_E_NOT_OK)
      {
        /* Holds the context information For write memory by address service */
        P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_CONST)
          pWriteMemoryAddressType =
          &Dcm_MemoryIdInfoAndIdValueTable[memIdIndex]
          .WriteMemoryRangeInfo[memRangeIndex];

        /* pWriteMemoryAddressType is taken only after a successful range check via
         * Dcm_Dsp_MemoryServices_WriteMemory_GetRange which ensured that the
         * write pointer is not NULL */
        DCM_PRECONDITION_ASSERT((pWriteMemoryAddressType != NULL_PTR), DCM_INTERNAL_API_ID);

        retVal = Dcm_Dsp_MemoryServices_CheckSecurity(pWriteMemoryAddressType);

        if(retVal != DCM_E_OK)
        {
          /* Security Access denied; Set NRC */
          pWriteMemoryContext->NrcConditions = DCM_E_SECURITYACCESSDENIED;
          serviceProcessStatus = DCM_E_OK;
        }
        else
        {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
          /* !LINKSTO SWS_Dcm_00825.WriteByAddress.ModCond,1 */
          /* !LINKSTO SWS_Dcm_00825.WriteByLabel.ModCond,1 */
          if (pWriteMemoryAddressType->ModeRuleMemFunction != NULL_PTR)
          {
            /* invoke the mode rule function for the service
            * if the mode rule resulted in failure curNrc will have the calculated nrc,
            * else zero which implies there is no failure */
            pWriteMemoryAddressType->ModeRuleMemFunction(&retVal);
          }

          if(retVal != DCM_E_OK)
          {
            pWriteMemoryContext->NrcConditions = retVal;
            serviceProcessStatus = DCM_E_OK;
          }
          else
#endif /*DCM_MODEDECLARATION_SUPPORT */
          {
            /*! LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.ReadDID.Concurrency,1 */
            const Std_ReturnType CurrentWrLockStatus
              = Dcm_Internal_GetLock(&Dcm_Dsp_MemoryServices_WriteLock);

            if (CurrentWrLockStatus != DCM_E_RESOURCE_LOCKED)
            {

              /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR430,1 */
              /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR422,1 */
              /* Call the Dcm_WriteMemory() callouts with the MemoryIdentifier parameter */
              writeMemoryStatus =
                Dcm_WriteMemory(DCM_INITIAL,
                  Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
                  pWriteMemoryContext->MemoryAddress, pWriteMemoryContext->MemorySize,
                  pWriteMemoryContext->InputBuffer
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_422) || \
    (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_430)
                  ,&(pWriteMemoryContext->NrcConditions)
#endif
                  );

              WriteMemoryCalled = TRUE;
            }
            else
            {
              writeMemoryStatus = DCM_E_RESOURCE_LOCKED;
            }
          }
        }
      }
      else
      {
        /* Memory Range Check failed; Set NRC */
        pWriteMemoryContext->NrcConditions = DCM_E_REQUESTOUTOFRANGE;
        serviceProcessStatus = DCM_E_OK;
      }
    }
    break;

    case DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT:
    {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_WriteMemory() callouts with all In-parameters set to 0x0 */
      writeMemoryStatus = Dcm_WriteMemory(
        DCM_PENDING,
        0U,
        0U,
        0U,
        pWriteMemoryContext->InputBuffer);
#else
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR422,1 */
      writeMemoryStatus =
        Dcm_WriteMemory(DCM_PENDING,
          Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
          pWriteMemoryContext->MemoryAddress, pWriteMemoryContext->MemorySize,
          pWriteMemoryContext->InputBuffer,
          &(pWriteMemoryContext->NrcConditions));
#endif
      WriteMemoryCalled = TRUE;
    }
    break;

    case DCM_MEMORY_ADDRESS_WRITE_CONFIRM_FORCED_RCRRP:
    {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_WriteMemory() callouts with all In-parameters set to 0x0 */
      writeMemoryStatus = Dcm_WriteMemory(
        DCM_FORCE_RCRRP_OK,
        0U,
        0U,
        0U,
        pWriteMemoryContext->InputBuffer);
#else
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR422,1 */
      writeMemoryStatus =
        Dcm_WriteMemory(DCM_FORCE_RCRRP_OK,
          Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
          pWriteMemoryContext->MemoryAddress, pWriteMemoryContext->MemorySize,
          pWriteMemoryContext->InputBuffer,
          &(pWriteMemoryContext->NrcConditions));
#endif

      pWriteMemoryContext->MemoryAddressWriteProcessingState =
        DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT;

      WriteMemoryCalled = TRUE;
    }
    break;

    case DCM_MEMORY_ADDRESS_WRITE_CANCEL:
    {
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
      /* Call the Dcm_WriteMemory() callouts with all In-parameters set to 0x0 */
      writeMemoryStatus = Dcm_WriteMemory(
        DCM_CANCEL,
        0U,
        0U,
        0U,
        pWriteMemoryContext->InputBuffer);
#else
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR430,1 */
      /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR422,1 */
      writeMemoryStatus =
        Dcm_WriteMemory(DCM_CANCEL,
          Dcm_MemoryIdInfoAndIdValueTable[memIdIndex].MemoryIDValue,
          pWriteMemoryContext->MemoryAddress, pWriteMemoryContext->MemorySize,
          pWriteMemoryContext->InputBuffer,
          &(pWriteMemoryContext->NrcConditions));
#endif

      WriteMemoryCalled = TRUE;
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  if (DCM_CANCEL == pWriteMemoryContext->MemoryAddressWriteProcessingState)
  {
    serviceProcessStatus = DCM_E_OK;
  }
  else
  {
    if (serviceProcessStatus != DCM_E_OK)
    {
      switch (writeMemoryStatus)
      {
        case DCM_WRITE_OK:
          serviceProcessStatus = DCM_E_OK;
          /* !LINKSTO Dcm.EB.WriteMemoryByAddress.ReturnOK.NotNRC0x00.ASR430,1 */
          pWriteMemoryContext->NrcConditions = DCM_E_OK;
          break;

        case DCM_WRITE_FORCE_RCRRP:
          /* !LINKSTO Dcm.EB.WriteMemoryByAddress.DCM_WRITE_FORCE_RCRRP,1 */
          serviceProcessStatus = DCM_E_FORCE_RCRRP;
          break;

        case DCM_WRITE_PENDING:
          serviceProcessStatus = DCM_E_PENDING;
          pWriteMemoryContext->MemoryAddressWriteProcessingState =
            DCM_MEMORY_ADDRESS_WRITE_SUBSEQUENT;
          break;

        case DCM_WRITE_FAILED:
#if (DCM_DATATRANSFER_SERVICES_ASR_VERSION == DATA_TRANSFER_SERVICES_AUTOSAR_403)
          /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteFailed.AUTOSAR403,1 */
          pWriteMemoryContext->NrcConditions = DCM_E_GENERALPROGRAMMINGFAILURE;
#endif

          /* !LINKSTO Dcm.EB.WriteMemoryByAddress.ReturnNOK.NRC0x00.ASR430,1 */
          if ((pWriteMemoryContext->NrcConditions) == 0x00U)
          {
            pWriteMemoryContext->NrcConditions = DCM_E_CONDITIONSNOTCORRECT;
          }

          serviceProcessStatus = DCM_E_NOT_OK;
          break;

        case DCM_E_RESOURCE_LOCKED:
          serviceProcessStatus = DCM_E_RESOURCE_LOCKED;
          break;

        default:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
#endif
          pWriteMemoryContext->NrcConditions = DCM_E_CONDITIONSNOTCORRECT;
          serviceProcessStatus = DCM_E_NOT_OK;
          break;
      }
    }
  }

  /* Release the write mutex if the write operation is finished */
  if ((WriteMemoryCalled == TRUE)
     && ((serviceProcessStatus == DCM_E_OK)
         || (serviceProcessStatus == DCM_E_NOT_OK)
        )
     )
  {
    Dcm_Internal_ReleaseLock(&Dcm_Dsp_MemoryServices_WriteLock);
  }

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_EXIT(serviceProcessStatus, pWriteMemoryContext);

  return serviceProcessStatus;
}

FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_GetResults(
  Dcm_NegativeResponseCodeType NrcConditions,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) pNrc)
{
  Std_ReturnType operationResult;

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_ENTRY(pNrc);

  if (DCM_E_OK != NrcConditions)
  {
    operationResult = DCM_E_NOT_OK;
    *pNrc = NrcConditions;
  }
  else
  {
    operationResult = DCM_E_OK;
  }

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_EXIT(operationResult);

  return operationResult;
}

FUNC(void, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_Setup(
  P2VAR(Dcm_WriteMemoryContextType, AUTOMATIC, DCM_VAR) pWriteMemoryContext,
  uint32 MemoryAddress, uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InputBuffer)
{
  DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_ENTRY(MemoryAddress, MemorySize, InputBuffer);

  pWriteMemoryContext->MemoryAddress = MemoryAddress;
  pWriteMemoryContext->MemorySize = MemorySize;
  pWriteMemoryContext->InputBuffer = InputBuffer;
  pWriteMemoryContext->NrcConditions = DCM_E_OK;

  pWriteMemoryContext->MemoryAddressWriteProcessingState =
    DCM_MEMORY_ADDRESS_WRITE_INITIAL

  DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_EXIT();
}
#endif /* #if(DCM_DSP_USE_SERVICE_0X3D == STD_ON) */

#if((DCM_DSP_USE_SERVICE_0X3D == STD_ON)||(DCM_DSP_USE_SERVICE_0X34 == STD_ON))

FUNC(uint8, DCM_CODE) Dcm_Dsp_MemoryServices_WriteMemory_GetRange(
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) memRangeIndex,
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) memIdIndex,
  uint32 writeMemAddress, uint32 writeMemSize)
{
  uint8 RetVal = DCM_E_NOT_OK;
  uint8_least memIdIdx = 0U;
  uint8_least memRangeIdx = 0U;
  uint32 LocalMemSize = writeMemSize;

  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_ENTRY(
    memRangeIndex, memIdIndex, writeMemAddress, writeMemSize);
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
  if(LocalMemSize == 0U)
  {
    /* If the NRC policy is following ISO 2006, an artificial size of 1 byte
     * is forced on the size to obtain a positive response */
    /* !LINKSTO SWS_Dcm_00489.MemWrite.ZeroSize.ISO_2006,2 */
    LocalMemSize = 1U;
  }
#endif
  /* !LINKSTO SWS_Dcm_00489.MemWrite.ZeroSize.ISO_2013,1 */
  /* !LINKSTO SWS_Dcm_00489.MemWrite.NonZeroSize.UpperLimitInclusion,1 */
  /* !LINKSTO SWS_Dcm_00489.MemWrite.NonZeroSize.UpperLimitExclusion,1 */
  if(LocalMemSize != 0U)
  {
    /* Check if Read Memory is supported in the requested address range */
    for (memIdIdx = 0U; ((memIdIdx < DCM_NUM_MEMORY_IDINFO) && (RetVal != DCM_E_OK)) ; ++memIdIdx)
    {
      for (memRangeIdx = 0U;
           memRangeIdx < Dcm_MemoryIdInfoAndIdValueTable[memIdIdx].numberofWriteRangeInfo;
           memRangeIdx++)
      {
        DCM_PRECONDITION_ASSERT(
          (Dcm_MemoryIdInfoAndIdValueTable[memIdIdx].WriteMemoryRangeInfo != NULL_PTR),
          DCM_INTERNAL_API_ID);

        if ((writeMemAddress >=
             (Dcm_MemoryIdInfoAndIdValueTable[memIdIdx]
              .WriteMemoryRangeInfo[memRangeIdx]).MemoryRangeLow) &&
#if (DCM_EXCLUDE_UPPER_LIMITS == STD_ON)
            (writeMemAddress <
             (Dcm_MemoryIdInfoAndIdValueTable[memIdIdx]
              .WriteMemoryRangeInfo[memRangeIdx]).MemoryRangeHigh) &&
#else
            (writeMemAddress <=
             (Dcm_MemoryIdInfoAndIdValueTable[memIdIdx]
              .WriteMemoryRangeInfo[memRangeIdx]).MemoryRangeHigh) &&
#endif
            (((writeMemAddress + LocalMemSize) - 1U) <=
             (Dcm_MemoryIdInfoAndIdValueTable[memIdIdx]
              .WriteMemoryRangeInfo[memRangeIdx]).MemoryRangeHigh))
        {
          *memIdIndex = (uint16)memIdIdx;
          *memRangeIndex = (uint16)memRangeIdx;
          RetVal = DCM_E_OK;
          break;
        }
      }
    }
  }
  DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_EXIT(
    RetVal, memRangeIndex, memIdIndex, writeMemAddress, writeMemSize);

  return RetVal;
}
#endif /* #if((DCM_DSP_USE_SERVICE_0X3D == STD_ON)||(DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */

#if ((DCM_DSP_USE_SERVICE_0X34 == STD_ON)||(DCM_DSP_USE_SERVICE_0X35 == STD_ON))

#if (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_RangeAndSecurityCheck(
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MemoryOperationType Operation,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc
)
{
  Std_ReturnType PermissionsResult = DCM_E_NOT_OK;

  /* Memory Id index */
  uint8_least MemIdIdx = 0U;

  /* Variable holding the index of MemoryRangeInfo table */
  uint16 MemRangeIndex = 0U;
  /* Variable holding the index of MemoryIdInfo table */
  uint16 MemIdCfgIndex = 0U;

  /* Deviation TASKING-1 */
  for (MemIdIdx = 0U; MemIdIdx < DCM_NUM_MEMORY_IDINFO; MemIdIdx++)
  {
#if (DCM_DSP_USE_SERVICE_0X34 == STD_ON)
    if (Operation == DCM_WRITEMEMORY)
    {
      PermissionsResult = Dcm_Dsp_MemoryServices_WriteMemory_GetRange(
        &MemRangeIndex, &MemIdCfgIndex, MemoryAddress, MemorySize);

      /* !LINKSTO Dcm.EB.RequestDownload.MemoryRangeCheck,1 */
      if (PermissionsResult == DCM_E_NOT_OK)
      {
        *Nrc = DCM_E_REQUESTOUTOFRANGE;
      }
      else
      {
        /* Pointer to requested memory in configuration */
        P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_VAR) WriteMemoryAddressPtr =
          &Dcm_MemoryIdInfoAndIdValueTable[MemIdCfgIndex].WriteMemoryRangeInfo[MemRangeIndex];

        PermissionsResult = Dcm_Dsp_MemoryServices_CheckSecurity(WriteMemoryAddressPtr);

        /* !LINKSTO Dcm.EB.RequestDownload.SecurityAccessDenied,1 */
        if (PermissionsResult == DCM_E_NOT_OK)
        {
          *Nrc = DCM_E_SECURITYACCESSDENIED;
        }
      }
    }
    else
#endif
#if (DCM_DSP_USE_SERVICE_0X35 == STD_ON)
    {
      DCM_PRECONDITION_ASSERT((Operation == DCM_READMEMORY), DCM_INTERNAL_API_ID);

      PermissionsResult = Dcm_Dsp_MemoryServices_ReadMemory_GetRange(
        &MemRangeIndex, &MemIdCfgIndex, MemoryAddress, MemorySize);

      /* !LINKSTO Dcm.EB.RequestUpload.MemoryRangeCheck,1 */
      if (PermissionsResult == DCM_E_NOT_OK)
      {
        *Nrc = DCM_E_REQUESTOUTOFRANGE;
      }
      else
      {
        /* Pointer to requested memory in configuration */
        P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_VAR) ReadMemoryAddressPtr =
          &Dcm_MemoryIdInfoAndIdValueTable[MemIdCfgIndex].ReadMemoryRangeInfo[MemRangeIndex];

        PermissionsResult = Dcm_Dsp_MemoryServices_CheckSecurity(ReadMemoryAddressPtr);

        /* !LINKSTO Dcm.EB.RequestUpload.SecurityAccessDenied,1 */
        if (PermissionsResult == DCM_E_NOT_OK)
        {
          *Nrc = DCM_E_SECURITYACCESSDENIED;
        }
      }
    }
#endif
    if (PermissionsResult == DCM_E_NOT_OK)
    {
      break;
    }
  }

  return PermissionsResult;
}
#endif /* (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK))*/

#if (DCM_ENABLE_DATA_TRANSFER_MEM_ID_CHECK == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_MemoryIdCheck(
  uint8 MemoryId
)
{
  Std_ReturnType MemoryIdFound = DCM_E_NOT_OK;

  /* Memory Id index */
  uint8_least MemIdIdx = 0U;

  /* !LINKSTO Dcm.EB.RequestUpload.MemoryIdValueCheck,1 */
  /* !LINKSTO Dcm.EB.RequestDownload.MemoryIdValueCheck,1 */
  for (MemIdIdx = 0U; MemIdIdx < DCM_NUM_MEMORY_IDINFO; MemIdIdx++)
  {
    if (MemoryId == Dcm_MemoryIdInfoAndIdValueTable[MemIdIdx].MemoryIDValue)
    {
      MemoryIdFound = DCM_E_OK;
      break;
    }
  }

  return MemoryIdFound;
}
#endif

#endif /*#if ((DCM_DSP_USE_SERVICE_0X34 == STD_ON)||(DCM_DSP_USE_SERVICE_0X35 == STD_ON))*/

#if (DCM_DSP_USE_GETMEMORYADDRESSANDSIZE == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) reqData,
  uint8 memAddLength, uint8 memSizeLength,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) memAddress,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) memSize )
{
  uint8_least reqDataIdx;
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_ENTRY(
    reqData, memAddLength, memSizeLength, memAddress, memSize);

  /* Verifying if the specified addressAndLengthFormatIdentifier is valid;
     Expected number of bytes in address and memory size field is from 1 to 4*/

  if((memAddLength < 1U) ||(memSizeLength < 1U) ||
     (memAddLength > DCM_MAX_ADDRESS_BYTES) || (memSizeLength > DCM_MAX_MEMSIZE_BYTES))
  {
    retVal = DCM_E_NOT_OK;
  }
  else
  {
    /* Fetch the Memory Address to be read */
    for (reqDataIdx = 1U; reqDataIdx <= memAddLength; ++reqDataIdx)
    {
      *memAddress = (uint32)(*memAddress << DCM_BYTE_BITS) |
                    (uint32)(reqData[reqDataIdx]);
    }
    /* Fetch the Size of Memory to be read */
    for (reqDataIdx = 1U; reqDataIdx <= memSizeLength; ++reqDataIdx)
    {
      *memSize = (uint32)(*memSize << DCM_BYTE_BITS) |
                 (uint32)(reqData[reqDataIdx + memAddLength]);
    }
    retVal = DCM_E_OK;
  }

  DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_EXIT(
    retVal, reqData, memAddLength, memSizeLength, memAddress, memSize);
  return retVal;
}
#endif /* (DCM_DSP_USE_GETMEMORYADDRESSANDSIZE == STD_ON) */

#if (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification(
  uint8 AddressLengthIdentifier)
{
  uint8_least Idx;
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_ENTRY(AddressLengthIdentifier);

  /* Search the list of supported addressAndLengthFormatIdentifiers */
  /* Deviation TASKING-1 */
  for(Idx = 0U; Idx < DCM_NUM_ADDR_LENGTH_FORMAT_IDENTIFIER; Idx++)
  {
    if(Dcm_SupportedAddAndLenFormatIds[Idx] == AddressLengthIdentifier)
    {
      retVal = DCM_E_OK;
      break;
    }
  }

  DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_EXIT(retVal,AddressLengthIdentifier);
  return retVal;
}

#endif /* (DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ((((DCM_DSP_USE_SERVICE_0X34 == STD_ON) || (DCM_DSP_USE_SERVICE_0X35 == STD_ON)) && \
     (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)) || \
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON))

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_MemoryServices_CheckSecurity(
P2CONST(Dcm_MemoryRangeInfoType, AUTOMATIC, DCM_CONST) MemoryAddressTypePtr)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_ENTRY(MemoryAddressTypePtr);

  DCM_PRECONDITION_ASSERT((MemoryAddressTypePtr != NULL_PTR), DCM_INTERNAL_API_ID);

  /* Check for security access , If the complete memory range can be written in the
   * current security level
   */
  /* !LINKSTO SWS_Dcm_00490.WriteByAddress.SecLevel,1 */
  /* !LINKSTO SWS_Dcm_00490.WriteByLabel.SecLevel,1 */
  if(MemoryAddressTypePtr->NumMemSecLevels > 0U)
  {
    /* get current security level */
    Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    if (curSecLevel != DCM_SEC_LEV_LOCKED)
    {
      uint8_least secLevIdx;

      /* Check for security level */
      for (secLevIdx = 0U;
      secLevIdx < MemoryAddressTypePtr->NumMemSecLevels;
      secLevIdx++)
      {
        if (curSecLevel == MemoryAddressTypePtr->MemSecLevels[secLevIdx])
        {
          retVal = DCM_E_OK;
          break;
        }
      }
      if(secLevIdx >= MemoryAddressTypePtr->NumMemSecLevels)
      {
        retVal = DCM_E_NOT_OK;
      }
    }
    else
    {
      retVal = DCM_E_NOT_OK;
    }
  }
  else
  {
    /* If is no reference configured this operation is allowed in any Security Level */
    retVal = DCM_E_OK;
  }

  DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_EXIT(retVal);

  return retVal;
}

#endif /*#if ((((DCM_DSP_USE_SERVICE_0X34 == STD_ON) || (DCM_DSP_USE_SERVICE_0X35 == STD_ON)) && \
     (DCM_ENABLE_DATA_TRANSFER_MEMRANGE_AND_SECURITY_CHECK == STD_ON)) || \
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON)) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_MEMORYSERVICES == STD_ON) */

/*==================[end of file]================================================================*/
