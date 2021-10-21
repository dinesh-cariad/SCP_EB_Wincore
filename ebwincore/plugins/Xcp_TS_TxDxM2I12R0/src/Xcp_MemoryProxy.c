/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to void into pointer to object
 *
 * Reason:
 * The MTA address for write purposes is needed.
 *
 * MISRAC2012-2) Deviated Rule: 17.2 (required)
 * Functions shall not call themselves, either directly or indirectly.
 *
 * Reason:
 * Memory proxy is designed in order to access user memory. Code base generic for accessing the memory
 * in the same partition or cross partition. Events are processed sequentially and each event may use
 * one or multiple memory proxy in order to access user data. In order to avoid delays in processing the events,
 * the next pending event is triggered as soon as the processing of the current event is complete.
 * Call graph recursion may only occur in the case when memory proxy is accessing local data on the same partition.
 * The number of possible call graph recursions is limited by the number of events to be processed.
 *
 */

/*==[Includes]================================================================*/

#include <Xcp_MemoryProxy.h>
#include <Xcp.h>
#include <Xcp_Int.h>
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
#include <Xcp_UserCallouts.h>
#endif

#if(XCP_MEMORY_ACCESS_AREAS == STD_ON)

#include <TSMem.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
STATIC FUNC(void, XCP_CODE) Xcp_ProcessModifyBitsFromUserCallout(uint8 Shift_Value, uint8 MemAreaIdx);
STATIC FUNC(uint32, XCP_CODE) Xcp_GetValue(uint8 Shift_Value);
#else
STATIC FUNC(void, XCP_CODE) Xcp_ProcessModifyBits(uint8* address, uint8 Shift_Value, uint16 And_Mask, uint16 Xor_Mask);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

VAR(uint8, XCP_VAR)    Xcp_MemoryProxyBuffer[XCP_MEMORY_PROXY_BUFFER_SIZE];

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
VAR(uint8, XCP_VAR) Xcp_ComandType;
VAR(uint8, XCP_VAR) Xcp_MemoryAreaIdx;
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
#define XCP_START_SEC_VAR_16
#include <Xcp_MemMap.h>

VAR(uint16, XCP_VAR) Xcp_AndMask;
VAR(uint16, XCP_VAR) Xcp_XorMask;

#define XCP_STOP_SEC_VAR_16
#include <Xcp_MemMap.h>
#endif

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

VAR(Atomic_t, XCP_VAR) Xcp_MemoryProxyState = ATOMICS_OBJECT_INITIALIZER(XCP_MEMORY_PROXY_AVAILABLE);
VAR(Atomic_t, XCP_VAR) Xcp_MemoryProxyInterrupted = ATOMICS_OBJECT_INITIALIZER(FALSE);
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
P2VAR(uint8, AUTOMATIC, XCP_VAR) Xcp_DataAddress;
#endif

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==[Definition of functions with external linkage]===========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/* Deviation MISRAC2012-2 */
FUNC(void, XCP_CODE) Xcp_MemoryProxyHandler(Xcp_MemoryProxyDataType* data)
{
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
  Atomic_value_t MemoryProxy_Interrupted_Local;
#endif
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
  /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  boolean MemoryProxy_ResetState = TRUE;
  uint16 EventId = XCP_E_INVALID_EVENT;
  uint8 CurrentApplicationId = (uint8)GetApplicationID();
#endif
  switch(data->type)
  {
    case XCP_MEMORYPROXYDATA_UPLOAD:
    {
      /* copy data from memory to proxy buffer */
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
      /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.MemProxy,1 */
      Xcp_ComandType = XCP_MEMORYPROXYDATA_UPLOAD;
      /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
      Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_USERCALLOUT_REQUEST_IN_PROGRESS);
      Xcp_MemoryAreas[data->MemoryAreaIdx].Callback.readFromRAMCallback(
                    &Xcp_MemoryProxyBuffer,
                    data->address,
                    data->size);
#else
      /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
      TS_MemCpy(&Xcp_MemoryProxyBuffer, data->address, data->size);
#endif
      break;
    }
    case XCP_MEMORYPROXYDATA_MODIFY_BITS:
    {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
      /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.MemProxy,1 */
      Xcp_AndMask = ((uint16)Xcp_MemoryProxyBuffer[1] << 8) | (Xcp_MemoryProxyBuffer[2]);
      Xcp_XorMask = ((uint16)Xcp_MemoryProxyBuffer[3] << 8) | (Xcp_MemoryProxyBuffer[4]);
      Xcp_ComandType = XCP_MEMORYPROXYDATA_MODIFY_BITS;
      Xcp_MemoryAreaIdx = data->MemoryAreaIdx;
      Xcp_DataAddress = data->address;
      Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_USERCALLOUT_REQUEST_IN_PROGRESS);
      Xcp_MemoryAreas[data->MemoryAreaIdx].Callback.readFromRAMCallback(
                    &Xcp_MemoryProxyBuffer[1],
                    data->address,
                    4U);
#else
      uint16 And_Mask, Xor_Mask;
      uint8 Shift_Value;
      Shift_Value = Xcp_MemoryProxyBuffer[0];
      And_Mask = ((uint16)Xcp_MemoryProxyBuffer[1] << 8) | (Xcp_MemoryProxyBuffer[2]);
      Xor_Mask = ((uint16)Xcp_MemoryProxyBuffer[3] << 8) | (Xcp_MemoryProxyBuffer[4]);
      Xcp_ProcessModifyBits(data->address, Shift_Value, And_Mask, Xor_Mask);
#endif
      break;
    }
    case XCP_MEMORYPROXYDATA_DOWNLOAD:
    {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
      /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.MemProxy,1 */
      Xcp_ComandType = XCP_MEMORYPROXYDATA_DOWNLOAD;
      Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_USERCALLOUT_REQUEST_IN_PROGRESS);
      Xcp_MemoryAreas[data->MemoryAreaIdx].Callback.writeToRAMCallback(
                    data->address,
                    &Xcp_MemoryProxyBuffer[0U],
                    data->size);
#else
      TS_MemCpy(data->address, &Xcp_MemoryProxyBuffer, data->size);
#endif
      break;
    }
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
    case XCP_MEMORYPROXYDATA_EVENT_PROCESSOR:
    {
      uint16 WriteIdx = 0U;
      uint16 Idx = 0U;
      uint16 ReadIdx = data->IdxRequestBuffer;

      MemoryProxy_ResetState = FALSE;
      EventId = data->EventId;

      for (Idx = 0U; Idx < data->size; Idx++)
      {
        if (data->RequestBuffer[ReadIdx].appId == CurrentApplicationId)
        {
          if (data->RequestBuffer[ReadIdx].type == XCP_MEMORYPROXYDATA_STIMULATION)
          {
            TS_MemCpy(data->RequestBuffer[ReadIdx].address1, data->RequestBuffer[ReadIdx].address2, data->RequestBuffer[ReadIdx].size);
          }
          else if (data->RequestBuffer[ReadIdx].type == XCP_MEMORYPROXYDATA_SAMPLE_0)
          {
            *(uint8 *)data->RequestBuffer[ReadIdx].address1 = 0x0U;
            /* copy data from memory to proxy buffer */
            TS_MemCpy(&Xcp_MemoryProxyBuffer[WriteIdx], data->RequestBuffer[ReadIdx].address1, data->RequestBuffer[ReadIdx].size);
          }
          else if (data->RequestBuffer[ReadIdx].type == XCP_MEMORYPROXYDATA_SAMPLE_1)
          {
            *(uint8 *)data->RequestBuffer[ReadIdx].address1 = 0x1U;/* copy data from memory to proxy buffer */
            TS_MemCpy(&Xcp_MemoryProxyBuffer[WriteIdx], data->RequestBuffer[ReadIdx].address1, data->RequestBuffer[ReadIdx].size);
          }
          else if (data->RequestBuffer[ReadIdx].type == XCP_MEMORYPROXYDATA_STIMULATION_0)
          {
            /* Deviation MISRAC2012-1 */
            *(uint8 *)data->RequestBuffer[ReadIdx].address2 =
                 *(uint8 *)data->RequestBuffer[ReadIdx].address1 | (uint8)(data->RequestBuffer[ReadIdx].bitMask);
            TS_MemCpy(data->RequestBuffer[ReadIdx].address1,
                 data->RequestBuffer[ReadIdx].address2, data->RequestBuffer[ReadIdx].size);
          }
          else if (data->RequestBuffer[ReadIdx].type == XCP_MEMORYPROXYDATA_STIMULATION_1)
          {
            /* Deviation MISRAC2012-1 */
            *(uint8 *)data->RequestBuffer[ReadIdx].address2 =
               *(uint8 *)data->RequestBuffer[ReadIdx].address1 & (uint8)(~data->RequestBuffer[ReadIdx].bitMask);
            TS_MemCpy(data->RequestBuffer[ReadIdx].address1,
                 data->RequestBuffer[ReadIdx].address2, data->RequestBuffer[ReadIdx].size);
          }
          else
          {
            /* copy data from memory to proxy buffer */
            TS_MemCpy(&Xcp_MemoryProxyBuffer[WriteIdx], data->RequestBuffer[ReadIdx].address1, data->RequestBuffer[ReadIdx].size);
          }
        }
        WriteIdx += data->RequestBuffer[ReadIdx].size;
        ReadIdx++;
        if (ReadIdx == XCP_MAX_SIZE_OF_REQUEST_BUFFER)
        {
          ReadIdx = 0U;
        }
      }

      break;
    }
#endif
    /* CHECK: NOPARSE */
    default:
      /* all possible cases have been explicitly covered in 'case's above, the 'default'
       * cannot be reached */
       XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
      break;
    /* CHECK: PARSE */
  }
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
  /* if user callouts are used, the memoryProxy response will be triggered only for daq/stim data */
  if (EventId != XCP_E_INVALID_EVENT)
#endif
  {
    if (CurrentApplicationId != partition)
    {
      Xcp_SendMemoryProxyNotification[CurrentApplicationId][partition](EventId);
    }
    else
    {
      Xcp_MemoryProxyResponseHandler(&EventId);
    }
  }

  if (MemoryProxy_ResetState == TRUE)
#endif
  {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_OFF)
    /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
    MemoryProxy_Interrupted_Local = Atomics_Load(&Xcp_MemoryProxyInterrupted);
    if (MemoryProxy_Interrupted_Local == TRUE)
    {
      Atomics_Store(&Xcp_MemoryProxyInterrupted, FALSE);
      Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
    }
    else
    {
      Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_READY);
    }
#endif
  }
}

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
FUNC(void, XCP_CODE) Xcp_ApplReadDataFromRAMFinished(uint8 result,
  P2VAR( void, AUTOMATIC, XCP_VAR ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_CONST ) DataPtr,
  uint8 DataLength)
{
  uint8 ComandType = Xcp_ComandType;
  uint8 MemAreaIdx = Xcp_MemoryAreaIdx;
  Atomic_value_t localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);

  /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.MemProxy,1 */
  if ((result == XCP_APPL_OK) && (localRAMCalloutProxyState == XCP_USERCALLOUT_REQUEST_IN_PROGRESS))
  {
    switch(ComandType)
    {
      case XCP_MEMORYPROXYDATA_MODIFY_BITS:
      {
        uint8 Shift_Value;
        Shift_Value = Xcp_MemoryProxyBuffer[0];
        /* The reading of the RAM values is finished, prepare the new values that need to be writen */
        Xcp_ProcessModifyBitsFromUserCallout(Shift_Value, MemAreaIdx);
        break;
      }
      case XCP_MEMORYPROXYDATA_DOWNLOAD:
      case XCP_MEMORYPROXYDATA_UPLOAD:
      default:
      {
        /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
        Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
        uint16 EventId = XCP_E_INVALID_EVENT;
        uint8 CurrentApplicationId = (uint8)GetApplicationID();

        /* Send notification to the active  that Memory Proxy has finished the task */
        if (CurrentApplicationId != partition)
        {
          Xcp_SendMemoryProxyNotification[CurrentApplicationId][partition](EventId);
        }
        else
        {
          Xcp_MemoryProxyResponseHandler(&EventId);
        }
        Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_READY);
        break;
      }
    }
  }
  else
  {
    Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
  }
  TS_PARAM_UNUSED(AddressPtr);
  TS_PARAM_UNUSED(DataPtr);
  TS_PARAM_UNUSED(DataLength);
}

FUNC(void, XCP_CODE) Xcp_ApplWriteDataToRAMFinished(uint8 result,
  P2VAR( void, AUTOMATIC, XCP_VAR ) AddressPtr,
  P2CONST( uint8, AUTOMATIC, XCP_CONST ) DataPtr,
  uint8 DataLength)
{
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  uint16 EventId = XCP_E_INVALID_EVENT;
  uint8 CurrentApplicationId = (uint8)GetApplicationID();
  Atomic_value_t localRAMCalloutProxyState = Atomics_Load(&Xcp_RAMCalloutProxyState);

  /* !LINKSTO Xcp.BswDistribution.CalibrationCommands.UserDefinedCallouts.MemProxy,1 */
  if ((result == XCP_APPL_OK) && (localRAMCalloutProxyState == XCP_USERCALLOUT_REQUEST_IN_PROGRESS))
  {
    /* Send notification to the active  that Memory Proxy has finished the task */
    if (CurrentApplicationId != partition)
    {
      /* !LINKSTO Xcp.dsn.BSWDistribution.MemProxy.Atomics,1 */
      Xcp_SendMemoryProxyNotification[CurrentApplicationId][partition](EventId);
    }
    else
    {
      Xcp_MemoryProxyResponseHandler(&EventId);
    }
  }
  else
  {
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_NO_USER_CALLOUT_REQUESTED);
  }
  Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_READY);
  TS_PARAM_UNUSED(AddressPtr);
  TS_PARAM_UNUSED(DataPtr);
  TS_PARAM_UNUSED(DataLength);
}
#endif

/*==[Definition of functions with internal linkage]===========================*/

#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
STATIC FUNC(void, XCP_CODE) Xcp_ProcessModifyBitsFromUserCallout(uint8 Shift_Value, uint8 MemAreaIdx)
{
  uint32 Value = Xcp_GetValue(Shift_Value);
  TS_MemCpy(&Xcp_MemoryProxyBuffer, (uint8*)&Value, 4);
  /* Bits have been read, we can write the value now */
  Xcp_MemoryAreas[MemAreaIdx].Callback.writeToRAMCallback(Xcp_DataAddress, &Xcp_MemoryProxyBuffer[0U], 4);
}
STATIC FUNC(uint32, XCP_CODE) Xcp_GetValue(uint8 Shift_Value)
{
  uint32 Temp_Value_1, Temp_Value_2, RetValue;
  TS_MemCpy(&RetValue, &Xcp_MemoryProxyBuffer[1], 4);
  Temp_Value_1 = (uint32)( (uint16)(~Xcp_AndMask) ) << Shift_Value;
  Temp_Value_2 = (uint32)Xcp_XorMask << Shift_Value;
  RetValue = ( RetValue & (uint32)( ~Temp_Value_1 ) ) ^ Temp_Value_2;
  return RetValue;
}
#else
STATIC FUNC(void, XCP_CODE) Xcp_ProcessModifyBits(uint8* address, uint8 Shift_Value, uint16 And_Mask, uint16 Xor_Mask)
{
  uint32 Temp_Value_1, Temp_Value_2, A_Value;
  /* XCP_MODIFY_VALUE_SIZE = 4 */
  TS_MemCpy(&A_Value, address, 4);
  Temp_Value_1 = (uint32)( (uint16)(~And_Mask) ) << Shift_Value;
  Temp_Value_2 = (uint32)Xor_Mask << Shift_Value;
  A_Value = ( A_Value & (uint32)( ~Temp_Value_1 ) ) ^ Temp_Value_2;
  TS_MemCpy(address, &A_Value, 4);
}
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif /* XCP_MEMORY_ACCESS_AREAS == STD_ON */