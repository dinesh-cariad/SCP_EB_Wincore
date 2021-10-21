/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * The memory API defines the memory identifier handle as type of void* in order to
 * be generic from the possible implementations. The current implementation internally
 * decided to use the handle as index into an array, thus an integral data type is needed.
 * Thus the handle pointer is converted to a handle integer.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 * Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 */

/*==================[inclusions]============================================*/
#include <TSMem.h>              /* TS_MemCpy(), TS_MemSet() */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Memory_Int.h>   /* own SW-unit interface file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
#include <NvM.h>
#include <TcpIp_IpV4_Int.h>
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

/* Limits:
 * number of dynamic memories to allocate: 65534
 * maximum memory block size: 65535
 */
/*==================[macros]================================================*/
#if defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON)

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS

#define TCPIP_MEMORY_BLOCK_UNUSED 0xFFFFU

/* Deviation MISRAC2012-1 */
#define TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr) (uint16)(((uint16_least)(idPtr)))

/* Deviation MISRAC2012-1 */
#define TCPIP_MEMORY_CREATE_IDPTR_FROM_IDX(idx) ((P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA))((uint16_least)(idx)))

/*==================[type definitions]======================================*/

/* \brief This type contains controls information for a memory block. */
typedef struct
{
  uint32 dataStart;
  uint16 dataPosition;
  uint16 blockSize;
}TcpIp_Memory_BlockDescriptorType;

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS

/* \brief Function looks for a free memory block in memory pool specified by
 * memoryPoolIdx */
STATIC FUNC_P2VAR(void, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_getMemoryBlock
(
  uint16 memoryPoolIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) sizePtr
);

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

/* \brief Function for entering the IDLE state */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_entry_IDLE(void);

/** \brief Transition function from state IDLE to IP_AVAILABLE or IP_UNAVAILABLE
 *
 * This function transfers the state machine state from IDLE to IP_AVAILABLE or IP_UNAVAILABLE
 * depending on the result of NvM_GetErrorStatus
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_tr_IDLE_ACTIVE(void);

/** \brief Transition function from states IP_AVAILABLE or IP_UNAVAILABLE to IDLE
 *
 * This function transfers the state machine from states IP_AVAILABLE or IP_UNAVAILABLE to IDLE
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_tr_ACTIVE_IDLE(void);

/** \brief Internal function for calling NvM_SetRamBlockStatus
 *
 * This function calls NvM_SetRamBlockStatus with the parameter status
 * to inform NvM if writing to permanent storage from the given
 * Memory NvM local cache is possible (it isn't used) or not (it is used)
 *
 * \param[in] status         If writing by NvM to permanent storage is possible (TRUE) or not (FALSE)
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_Set_Status
(
  boolean status
);
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

#define TCPIP_START_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

VAR(uint32,TCPIP_VAR_CLEARED) TcpIp_Memory_NvM_Ip_Memory[TCPIP_NVM_BLOCK_SIZE];

#define TCPIP_STOP_SEC_VAR_CLEARED_32
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

STATIC VAR(uint8,TCPIP_VAR_CLEARED) TcpIp_Memory_NvM_State;

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS

#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/* Deviation MISRAC2012-2 */
STATIC VAR(uint8,TCPIP_VAR_CLEARED) TcpIp_Memory_AllocatedMemory[TCPIP_MEMORY_TOTAL_SIZE];

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC VAR(TcpIp_Memory_BlockDescriptorType,TCPIP_VAR_CLEARED)
  TcpIp_Memory_BlockDescriptor[TCPIP_MEMORY_NUM_TOTAL_BLOCKS];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Memory_init
(
  void
)
{
  DBG_TCPIP_MEMORY_INIT_ENTRY();

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)
  TcpIp_Memory_NvM_entry_IDLE();
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS
  {
    uint8_least i = 0U;
    for(i = 0U; i < (uint8_least)TcpIp_PBcfgPtr->numOfMemoryPools; i++)
    {
      CONSTP2CONST(TcpIp_memoryConfigType,AUTOMATIC,TCPIP_APPL_CONST) memoryPoolPtr =
          &((TCPIP_CFG(memory,i)));

      const uint16 descriptorOffset = memoryPoolPtr->memoryBlockDescriptorOffset;
      const uint16 numMemoryBlocks = memoryPoolPtr->numOfBlocks;
      const uint16 memoryBlockSize = memoryPoolPtr->memoryBlockSize;
      uint32 memoryPoolOffset = memoryPoolPtr->memoryPoolOffset;
      uint16 j;

      TCPIP_PRECONDITION_ASSERT((descriptorOffset + numMemoryBlocks) <= TCPIP_MEMORY_NUM_TOTAL_BLOCKS, TCPIP_INTERNAL_API_ID);
      for(j = descriptorOffset; j < (descriptorOffset + numMemoryBlocks); j++)
      {
        P2VAR(TcpIp_Memory_BlockDescriptorType,AUTOMATIC,TCPIP_APPL_DATA) memoryDescriptorPtr =
          &TcpIp_Memory_BlockDescriptor[j];

        memoryDescriptorPtr->dataStart = memoryPoolOffset;
        memoryDescriptorPtr->blockSize = memoryBlockSize;
        memoryDescriptorPtr->dataPosition = TCPIP_MEMORY_BLOCK_UNUSED;
        memoryPoolOffset += memoryBlockSize;
      }
    }
  }

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

  DBG_TCPIP_MEMORY_INIT_EXIT();

}

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS

TS_MOD_PRIV_DEFN FUNC_P2VAR(void, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_new
(
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) sizePtr
)
{
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) memHandle = TCPIP_MEMORY_IDPTR_INVALID;
  uint8 i = 0U;
  uint8 startBufferIdx = 0U;
  uint8 endBufferIdx = TcpIp_PBcfgPtr->numOfMemoryPools - 1U;

  DBG_TCPIP_MEMORY_NEW_ENTRY(sizePtr);

  /* look for the best fitting memory block */
  while((endBufferIdx - startBufferIdx) > 1)
  {
    const uint8 fitIndex = startBufferIdx + ((endBufferIdx - startBufferIdx)/2U);
    CONSTP2CONST(TcpIp_memoryConfigType,AUTOMATIC,TCPIP_APPL_CONST) memoryPoolPtr =
        &((TCPIP_CFG(memory,fitIndex)));

    TCPIP_PRECONDITION_ASSERT(fitIndex < TcpIp_PBcfgPtr->numOfMemoryPools, DET_INTERNAL_API_ID);
    TCPIP_PRECONDITION_ASSERT(endBufferIdx >= startBufferIdx, DET_INTERNAL_API_ID);

    if((*sizePtr) >= memoryPoolPtr->memoryBlockSize)
    {
      startBufferIdx = fitIndex;
    }
    else
    {
      endBufferIdx = fitIndex;
    }
  }

  if((*sizePtr) > (TCPIP_CFG(memory,startBufferIdx)).memoryBlockSize)
  {
    startBufferIdx = endBufferIdx;
  }

  /* Start from smallest buffer that could fit. If none is free, look for larger one */
  for(i = startBufferIdx;
      (i < TcpIp_PBcfgPtr->numOfMemoryPools) && (memHandle == TCPIP_MEMORY_IDPTR_INVALID);
      i++
     )
  {
    memHandle = TcpIp_Memory_getMemoryBlock(i, sizePtr);
  }

  /* if no buffer is left which fits, look for a smaller one */
  if((memHandle == TCPIP_MEMORY_IDPTR_INVALID) && (startBufferIdx != 0U))
  {
    for(i = startBufferIdx - 1U;
        (i != 0) && (memHandle == TCPIP_MEMORY_IDPTR_INVALID);
        i--
       )
    {
      memHandle = TcpIp_Memory_getMemoryBlock(i, sizePtr);
    }
  }


  DBG_TCPIP_MEMORY_NEW_EXIT(memHandle,sizePtr);

  return memHandle;
}

TS_MOD_PRIV_DEFN FUNC_P2VAR(uint8, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_getLinearMemoryAddress
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 index,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) size
)
{
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) memPtr;
  uint32 linearSize;

  DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_ENTRY(idPtr,index,size);

  TCPIP_PRECONDITION_ASSERT(idPtr != TCPIP_MEMORY_IDPTR_INVALID,DET_INTERNAL_API_ID);

  {
    const uint16 blockIdx = TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr);
    P2VAR(TcpIp_Memory_BlockDescriptorType,AUTOMATIC,TCPIP_APPL_DATA) memoryDescriptorPtr =
      &TcpIp_Memory_BlockDescriptor[blockIdx];
    const uint32 data_start = memoryDescriptorPtr->dataStart;
    const uint32 data_postion = memoryDescriptorPtr->dataPosition;
    const uint32 blockSize = memoryDescriptorPtr->blockSize;
    const uint32 offset = data_postion + index;

    TCPIP_PRECONDITION_ASSERT(index <= blockSize,DET_INTERNAL_API_ID);

    if(offset < blockSize)
    {
      memPtr = &TcpIp_Memory_AllocatedMemory[data_start + offset];
      linearSize = blockSize - offset;
    }
    else
    {
      const uint32 wrapped_offset = offset - blockSize;
      memPtr = &TcpIp_Memory_AllocatedMemory[data_start + wrapped_offset];
      linearSize = (data_postion - wrapped_offset);
    }

  }
  *size = linearSize;

  DBG_TCPIP_MEMORY_GETLINEARMEMORYADDRESS_EXIT(memPtr,idPtr,index,size);

  return memPtr;
}

#if ((TCPIP_TCP_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Memory_get
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) destPtr,
  uint32 index,
  uint32 size
)
{
  DBG_TCPIP_MEMORY_GET_ENTRY(idPtr,destPtr,index,size);

  TCPIP_PRECONDITION_ASSERT(size > 0U,DET_INTERNAL_API_ID);

  TCPIP_PRECONDITION_ASSERT(idPtr != TCPIP_MEMORY_IDPTR_INVALID,DET_INTERNAL_API_ID);

  /* Deviation MISRAC2012-1 */
  TCPIP_PRECONDITION_ASSERT
  (
    TcpIp_Memory_BlockDescriptor[TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr)].blockSize >= index,
    DET_INTERNAL_API_ID
  );
  /* Deviation MISRAC2012-1 */
  TCPIP_PRECONDITION_ASSERT
  (
    size <= (TcpIp_Memory_BlockDescriptor[TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr)].blockSize - index),
    DET_INTERNAL_API_ID
  );


  {
    uint32 firstSize;
    CONSTP2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) firstMemPtr =
      TcpIp_Memory_getLinearMemoryAddress(idPtr,index,&firstSize);

    if(firstSize < size)
    {
      const uint32 secondSize = size - firstSize;
      uint32 remainingSize;
      CONSTP2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) secondMemPtr =
         TcpIp_Memory_getLinearMemoryAddress(idPtr,index + firstSize,&remainingSize);

      TCPIP_PRECONDITION_ASSERT(secondSize <= remainingSize,DET_INTERNAL_API_ID);

      TS_MemCpy(&destPtr[firstSize], secondMemPtr, secondSize);
    }
    else
    {
      firstSize = size;
    }
    TS_MemCpy(&destPtr[0U], firstMemPtr, firstSize);
  }

  DBG_TCPIP_MEMORY_GET_EXIT(idPtr,destPtr,index,size);
}
#endif /* ((TCPIP_TCP_ENABLED == STD_ON)) */

#if (TCPIP_TCP_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_Memory_increase
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 size
)
{
  const uint16 blockIdx = TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr);
  uint16 blockSize;
  DBG_TCPIP_MEMORY_INCREASE_ENTRY(idPtr,size);
  TCPIP_PRECONDITION_ASSERT(blockIdx < TCPIP_MEMORY_NUM_TOTAL_BLOCKS, TCPIP_INTERNAL_API_ID);
  {
    blockSize = TcpIp_Memory_BlockDescriptor[blockIdx].blockSize;


    TS_PARAM_UNUSED(idPtr);
    TS_PARAM_UNUSED(size);
  }
  DBG_TCPIP_MEMORY_INCREASE_EXIT(blockSize,idPtr,size);

  return blockSize;
}
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */


TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_Memory_decrease
(
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) idPtr,
  uint32 size
)
{

  uint32 newSize;

  DBG_TCPIP_MEMORY_DECREASE_ENTRY(idPtr,size);

  TCPIP_PRECONDITION_ASSERT(idPtr != TCPIP_MEMORY_IDPTR_INVALID,DET_INTERNAL_API_ID);
  {
    const uint16 blockIdx = TCPIP_MEMORY_CREATE_IDX_FROM_IDPTR(idPtr);
    P2VAR(TcpIp_Memory_BlockDescriptorType,AUTOMATIC,TCPIP_APPL_DATA) TcpIp_Memory_BlockDescriptorPtr =
      &TcpIp_Memory_BlockDescriptor[blockIdx];
    const uint16 blockSize = TcpIp_Memory_BlockDescriptorPtr->blockSize;

    TCPIP_PRECONDITION_ASSERT(size <= blockSize,DET_INTERNAL_API_ID);

    if(size < blockSize)
    {
      const uint32 data_start_new = (TcpIp_Memory_BlockDescriptorPtr->dataPosition + size);

      if(data_start_new >= blockSize)
      {
        TcpIp_Memory_BlockDescriptorPtr->dataPosition = (uint16)(data_start_new - blockSize);
      }
      else
      {
        TcpIp_Memory_BlockDescriptorPtr->dataPosition = (uint16)data_start_new;
      }
      newSize = blockSize;
    }
    else
    {
      TcpIp_Memory_BlockDescriptorPtr->dataPosition = TCPIP_MEMORY_BLOCK_UNUSED;
      newSize = 0U;
    }
  }

  DBG_TCPIP_MEMORY_DECREASE_EXIT(newSize,idPtr,size);

  return  newSize;
}

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_start(void)
{
  DBG_TCPIP_MEMORY_NVM_START_ENTRY();

  switch(TcpIp_Memory_NvM_State)
  {
    case TCPIP_MEMORY_NVM_STATE_IDLE:
      TcpIp_Memory_NvM_tr_IDLE_ACTIVE();
      break;
    /* CHECK: NOPARSE */
    case TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE:
    case TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE:
    default:
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }

  DBG_TCPIP_MEMORY_NVM_START_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_stop(void)
{
  DBG_TCPIP_MEMORY_NVM_STOP_ENTRY();

  switch(TcpIp_Memory_NvM_State)
  {
    case TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE:
    case TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE:
      TcpIp_Memory_NvM_tr_ACTIVE_IDLE();
      break;
    /* CHECK: NOPARSE */
    case TCPIP_MEMORY_NVM_STATE_IDLE:
    default:
      TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
      break; /* should not happen */
    /* CHECK: PARSE */
  }

  DBG_TCPIP_MEMORY_NVM_STOP_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_setIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  uint32 ipAddr,
  uint8 method
)
{
  const uint16 memNvMIdx = TcpIp_Memory_NvM_getCacheIdx(localAddrId,method);

  DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_ENTRY(localAddrId,ipAddr,method);

  if(memNvMIdx < TCPIP_NVM_BLOCK_NUM_OF_ENTRIES)
  {
    switch(TcpIp_Memory_NvM_State)
    {
      case TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE:
      case TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE:
          TcpIp_Memory_NvM_Ip_Memory[memNvMIdx] = ipAddr;
        break;
      /* CHECK: NOPARSE */
      case TCPIP_MEMORY_NVM_STATE_IDLE:
      default:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_MEMORY_NVM_SETIPADDRESS_EXIT(localAddrId,ipAddr,method);

}
TS_MOD_PRIV_DEFN FUNC(boolean, TCPIP_CODE) TcpIp_Memory_NvM_getIpAddress
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) ipAddrPtr,
  uint8 method
)
{
  boolean ipAddrSet = FALSE;
  const uint16 memNvMIdx = TcpIp_Memory_NvM_getCacheIdx(localAddrId,method);

  DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_ENTRY(localAddrId,ipAddrPtr,method);

  if(memNvMIdx < TCPIP_NVM_BLOCK_NUM_OF_ENTRIES)
  {
    switch(TcpIp_Memory_NvM_State)
    {
      case TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE:
        if(TcpIp_Memory_NvM_Ip_Memory[memNvMIdx] != TCPIP_IPV4_ADDR_INVALID_VALUE)
        {
          *ipAddrPtr = TcpIp_Memory_NvM_Ip_Memory[memNvMIdx];
          ipAddrSet = TRUE;
        }
        break;
      case TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE:
        /* CtrlSM should be informed that the IP generation is needed */
        break;
      case TCPIP_MEMORY_NVM_STATE_IDLE:
        /* Special use case, this is called when conflict is received on a single linkLocal
         * assigned address, ctrl is transferred to offline and LinkLocal state machine reenteres
         * state PROBING */
        break;
      /* CHECK: NOPARSE */
      default:
        TCPIP_UNREACHABLE_CODE_ASSERT(DET_INTERNAL_API_ID);
        break; /* should not happen */
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_MEMORY_NVM_GETIPADDRESS_EXIT(ipAddrSet,localAddrId,ipAddrPtr,method);

  return ipAddrSet;
}
#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

/*==================[internal function definitions]=========================*/

#ifdef TCPIP_MEMORY_NUM_TOTAL_BLOCKS

STATIC FUNC_P2VAR(void, TCPIP_APPL_DATA, TCPIP_CODE) TcpIp_Memory_getMemoryBlock
(
  uint16 memoryPoolIdx,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) sizePtr
)
{
  P2VAR(void, AUTOMATIC, TCPIP_APPL_DATA) memHandle = TCPIP_MEMORY_IDPTR_INVALID;

  CONSTP2CONST(TcpIp_memoryConfigType,AUTOMATIC,TCPIP_APPL_CONST) memoryPoolPtr =
      &((TCPIP_CFG(memory,memoryPoolIdx)));
  const uint16 descriptorOffset = memoryPoolPtr->memoryBlockDescriptorOffset;
  const uint16 buffSize = memoryPoolPtr->memoryBlockSize;
  const uint16 buffCount = memoryPoolPtr->numOfBlocks;
  uint16 j;
  TCPIP_PRECONDITION_ASSERT((descriptorOffset + buffCount) <= TCPIP_MEMORY_NUM_TOTAL_BLOCKS, TCPIP_INTERNAL_API_ID);
  for(j = descriptorOffset; j < (descriptorOffset + buffCount); j++)
  {
    if(TcpIp_Memory_BlockDescriptor[j].dataPosition == TCPIP_MEMORY_BLOCK_UNUSED)
    {
      TcpIp_Memory_BlockDescriptor[j].dataPosition = 0U;
      memHandle = TCPIP_MEMORY_CREATE_IDPTR_FROM_IDX(j);
      *sizePtr = buffSize;
      break;
    }
  }

  return memHandle;
}

#endif /* TCPIP_MEMORY_NUM_TOTAL_BLOCKS */

#if (TCPIP_NVM_STORING_ENABLED == STD_ON)

STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_entry_IDLE(void)
{
  TcpIp_Memory_NvM_State = TCPIP_MEMORY_NVM_STATE_IDLE;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_tr_IDLE_ACTIVE(void)
{
  uint8 state = TCPIP_MEMORY_NVM_STATE_IP_UNAVAILABLE;
  VAR(NvM_RequestResultType, EBTEST_VAR) RequestResult = NVM_REQ_NOT_OK;

  /* transition code */
  (void)NvM_GetErrorStatus(TCPIP_NVM_BLOCK_ID,&RequestResult);
  if(RequestResult == NVM_REQ_OK)
  {
    state = TCPIP_MEMORY_NVM_STATE_IP_AVAILABLE;
  }

  /* entry code */
  TcpIp_Memory_NvM_Set_Status(FALSE);
  TcpIp_Memory_NvM_State = state;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_tr_ACTIVE_IDLE(void)
{
  TcpIp_Memory_NvM_Set_Status(TRUE);
  TcpIp_Memory_NvM_entry_IDLE();
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Memory_NvM_Set_Status
(
  boolean status
)
{
#if (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON)
  Std_ReturnType retVal = E_NOT_OK;
  /* NvM should be in a correct state */
  retVal =
#else /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
  (void)
#endif /* (TCPIP_PRECONDITION_ASSERT_ENABLED == STD_ON) */
  NvM_SetRamBlockStatus(TCPIP_NVM_BLOCK_ID,status);
  TCPIP_PRECONDITION_ASSERT((retVal == E_OK),TCPIP_INTERNAL_API_ID);
}

#endif /* (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
