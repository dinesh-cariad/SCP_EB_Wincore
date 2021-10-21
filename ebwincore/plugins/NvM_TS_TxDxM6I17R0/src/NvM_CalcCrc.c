/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (advisory)
 * A cast shall not be performed between a pointer to object type and a pointer to a different object type.
 *
 * Reason: The portion of code is necessary and it has no alignment problems.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason: Even though is only used once the object needs a global scope.
 */
/* !LINKSTO NVM076,1 */
/*==================[inclusions]=============================================*/

#if (defined NVM_INTERNAL_USE)
#error NVM_INTERNAL_USE is already defined
#endif
/* prevent redundant declarations of RTE types */
#define NVM_INTERNAL_USE

#include <SchM_NvM.h>             /* module specific header for             */
                                  /* Schedule Manager services              */
#include <NvM_Trace.h>
#include <NvM.h>                  /* Extern NVRAM Manager API definitions   */
#include <NvM_FunctionIntern.h>   /* Contains external function definitions */
                                  /* which are used only inside NvM.        */
#include <NvM_DataIntern.h>       /* NvM data which is used only            */
                                  /* within the NvM module.                 */
#include <NvM_CalcCrc.h>          /* Extern definitions of unit CalcCrc.    */
#include <NvM_StateMachine.h>     /* Contains extern definitions for unit   */
                                  /* 'StateMachine'. It can be included in  */
                                  /* library source code files.             */
/* !LINKSTO NVM555,1 */
#include <Crc.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

/** \brief Function to calculate CRC with interruption.
 **        If CRC shall be calculated with interruption, this function is
 **        called cyclically by NvM_MainFunction() until calculation is
 **        completed.
 **
 ** \pre   NvM_CalculateCrc must be executed prior to this call.
 **/
STATIC FUNC(void, NVM_CODE) NvM_CalcCrc_AsyncState(void);

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[external constants]=====================================*/
/*==================[internal constants]=====================================*/
/*==================[external data]==========================================*/
/*==================[internal data]==========================================*/

#if (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)
#define NVM_START_SEC_VAR_POWER_ON_INIT_8
#include <NvM_MemMap.h>

/**  \brief The RAM buffer for storing the calculated CRC values
 **         for all blocks which are configured to have CRC calculated
 **         for their permanent RAM blocks (with parameter NvMCalcRamBlockCrc).
 **/
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, NVM_VAR_POWER_ON_INIT) NvM_RamBlockCrcValues[NVM_RAMBLOCK_CRC_BUFFER_SIZE] = {0U};

#define NVM_STOP_SEC_VAR_POWER_ON_INIT_8
#include <NvM_MemMap.h>
#endif

#if (NVM_NVBLOCK_CRC_BUFFER_SIZE > 0U)
#define NVM_START_SEC_VAR_POWER_ON_INIT_8
#include <NvM_MemMap.h>

/**  \brief The RAM buffer for storing the calculated CRC values
 **         for all blocks which are configured to have CRC comparison mechanism
 **         for their permanent/synced RAM blocks (with parameter NvMBlockUseCRCCompMechanism).
 **/
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, NVM_VAR_POWER_ON_INIT) NvM_NvBlockCrcValues[NVM_NVBLOCK_CRC_BUFFER_SIZE] = {0U};

#define NVM_STOP_SEC_VAR_POWER_ON_INIT_8
#include <NvM_MemMap.h>
#endif

#define NVM_START_SEC_VAR_INIT_32
#include <NvM_MemMap.h>

/**  \brief The remaining number of bytes for CRC calculation.
 **         If CRC calculation must be interrupted it is used to store the
 **         length of the data block which must still be CRC calculated.
 **/
STATIC VAR(uint32, NVM_VAR) NvM_CalcCrc_RemainingLength = 0U;

#define NVM_STOP_SEC_VAR_INIT_32
#include <NvM_MemMap.h>

#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>

/** \brief Intermediate address for CRC calculation.
 **        If CRC calculation must be interrupted this is used to store data
 **        address at which CRC calculation must continue after interruption.
 **/
STATIC NvM_PtrToApplDataType NvM_CalcCrc_Address  = NULL_PTR;

#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>

/*==================[external function definitions]==========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

FUNC(uint32, NVM_CODE) NvM_CalcCrc_UpdateLength(void)
{
  uint32 NvMCurrentCrcLength = 0U;

  if (NvM_CalcCrc_RemainingLength > NVM_CRC_NUM_OF_BYTES)
  {
    NvM_CalcCrc_RemainingLength -= NVM_CRC_NUM_OF_BYTES;
    NvMCurrentCrcLength = NVM_CRC_NUM_OF_BYTES;
    /* MainFunction must terminate after first call */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }
  else
  {
    NvMCurrentCrcLength = NvM_CalcCrc_RemainingLength;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_StateMachine_TerminateCurrentLevel;

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_OK));
    NvM_GlobalErrorStatus = NVM_REQ_OK;
  }

  /* Update calculation address for this cycle */
  NvM_CalcCrc_Address = &(NvM_CalcCrc_Address[NVM_CRC_NUM_OF_BYTES]);

  return NvMCurrentCrcLength;
}

FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_CalcCrc_GetBlockCrcAddress(void)
{
  NvM_PtrToApplDataType ret = NULL_PTR;
  ret = &(NvM_GetDataAddress()[NvM_CurrentBlockDescriptorPtr->nvBlockLength]);
  return ret;
}

FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetStoredRamCRCAddress(void)
{
  NvM_PtrToApplDataType ret = NULL_PTR;
#if (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U)
  /* return address of stored crc value */
  ret = (NvM_PtrToApplDataType)
   (&(NvM_RamBlockCrcValues[NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].ramBlockCrcIndex]));
#else
  ret = NULL_PTR;
#endif
  return ret;
}

#if (NVM_CRC_COMP_MECH == STD_ON)
FUNC_P2VAR(uint8, AUTOMATIC, NVM_CODE) NvM_GetStoredNvCRCAddress(void)
{
  NvM_PtrToApplDataType ret = NULL_PTR;
#if (NVM_NVBLOCK_CRC_BUFFER_SIZE > 0U)
  /* return address of stored crc value */
  ret = (NvM_PtrToApplDataType)
   (&(NvM_NvBlockCrcValues[NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockCrcIndex]));
#else
  ret = NULL_PTR;
#endif
  return ret;
}
#endif /*NVM_CRC_COMP_MECH == STD_ON*/

FUNC(void, NVM_CODE) NvM_CalculateCrc(NvM_PtrToApplDataType DataAddress)
{
  uint32 Length = NvM_BlockDescriptorTable[NvM_GlobalBlockNumber].nvBlockLength;

  /* Check if Crc should be calculated in parts */
  if (Length > NVM_CRC_NUM_OF_BYTES)
  {
    /* Save the remaining length to continue Crc calculation */
    NvM_CalcCrc_RemainingLength = Length - NVM_CRC_NUM_OF_BYTES;
    /* Use only the configured size for this calculation */
    Length = NVM_CRC_NUM_OF_BYTES;

    /* Save the start address of data block */
    NvM_CalcCrc_Address = DataAddress;

    /* Set state function to continue Crc calculation */

    DBG_NVM_GLOBALCALLLEVEL((NvM_GlobalCallLevel),((NvM_GlobalCallLevel)+1U));
    NvM_GlobalCallLevel++;
    NvM_CurrentFunctionPtr[NvM_GlobalCallLevel] = &NvM_CalcCrc_AsyncState;
    /* MainFunction must terminate after first call */

    DBG_NVM_GLOBALERRORSTATUS((NvM_GlobalErrorStatus),(NVM_REQ_PENDING));
    NvM_GlobalErrorStatus = NVM_REQ_PENDING;
  }

  NvM_CalcCrc_ProcessCrc(DataAddress, (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                         Length, NvM_CurrentBlockDescriptorPtr->blockDesc, TRUE);
}

FUNC(void, NVM_CODE) NvM_CopyCrc
(
  NvM_PtrToApplDataType DataAddress,
  NvM_PtrToApplDataType CrcAddress,
  uint32 BlockDesc
)
{
  /* Deviation MISRAC2012-1 */
  NvM_PtrToApplDataType CrcBufPtr8 = CrcAddress;

  /* Perform byte copy of the CRC data from CRC buffer to the target address */
  DataAddress[0U] = CrcBufPtr8[0U];

#if ((NVM_NUMBER_OF_CALC_CRC16_BLOCKS > 0U) || (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U))
/* If current block use 16/32 bit crc */
  if ((NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC16) ||
      (NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32)
     )
  {
    DataAddress[1U] = CrcBufPtr8[1U];
  }
#endif

#if (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U)
  /* If current block use 32 bit crc */
  if (NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32)
  {
    DataAddress[2U] = CrcBufPtr8[2U];
    DataAddress[3U] = CrcBufPtr8[3U];
  }
#endif
}

FUNC(NvM_RequestResultType, NVM_CODE) NvM_CheckCrc
(
  P2CONST(uint8, AUTOMATIC, NVM_APPL_DATA) DataAddress,
  NvM_PtrToApplDataType CrcCalcBuffer,
  uint32 BlockDesc
)
{
  NvM_RequestResultType Result;

  if ((CrcCalcBuffer[0] != DataAddress[0]))
  {
    Result = NVM_REQ_NOT_OK;
  }
#if ((NVM_NUMBER_OF_CALC_CRC16_BLOCKS > 0U) || (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U))
  /* If current block use 16/32 bit crc */
  else if (
            (
              (NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC16) ||
              (NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32)
            ) &&
            (CrcCalcBuffer[1] != DataAddress[1])
          )
  {
    Result = NVM_REQ_NOT_OK;
  }
#endif /* #if ((NVM_NUMBER_OF_CALC_CRC16_BLOCKS > 0U) || (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U)) */
#if (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U)
  /* If current block use 32 bit crc */
  else if (
            (NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32) &&
            (
              (CrcCalcBuffer[2] != DataAddress[2]) ||
              (CrcCalcBuffer[3] != DataAddress[3])
            )
          )
  {
    Result = NVM_REQ_NOT_OK;
  }
#endif /* #if (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U) */
  else
  {
    Result = NVM_REQ_OK;
  }

  return Result;
}

FUNC(void, NVM_CODE) NvM_CalcCrc_ProcessCrc
(
  NvM_PtrToApplDataType CrcDataAdress,
  NvM_PtrToApplDataType CalcCrcBuffer,
  uint32 Length,
  uint32 BlockDesc,
  boolean FirstCall
)
{
#if (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U)
  P2VAR(uint32, AUTOMATIC, NVM_APPL_DATA) CrcBufPtr32 =
      /* Deviation MISRAC2012-1 */
      (P2VAR(uint32, AUTOMATIC, NVM_APPL_DATA))CalcCrcBuffer;
#endif
#if (NVM_NUMBER_OF_CALC_CRC16_BLOCKS > 0U)
  P2VAR(uint16, AUTOMATIC, NVM_APPL_DATA) CrcBufPtr16 =
      /* Deviation MISRAC2012-1 */
      (P2VAR(uint16, AUTOMATIC, NVM_APPL_DATA))CalcCrcBuffer;
#endif
#if (NVM_NUMBER_OF_CALC_CRC8_BLOCKS > 0U)
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) CrcBufPtr8 =
      /* Deviation MISRAC2012-1 */
      (P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA))CalcCrcBuffer;
#endif
#if (NVM_NUMBER_OF_CALC_CRC32_BLOCKS > 0U)

  /* If current block use 32 bit crc */
  if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC32)
  {
    /* Calculate CRC32 and store the result in the CRC buffer as a 32-bit value.
     */
    *CrcBufPtr32 =
      Crc_CalculateCRC32(CrcDataAdress, Length, *CrcBufPtr32, FirstCall);
  }
#endif
#if (NVM_NUMBER_OF_CALC_CRC16_BLOCKS > 0U)

  /* If current block use 16 bit crc */
  if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC16)
  {
    /* Calculate CRC16 and store the result in the CRC buffer as a 16-bit value.
     */
    *CrcBufPtr16 = Crc_CalculateCRC16(CrcDataAdress, Length, *CrcBufPtr16, FirstCall);
  }
#endif
#if (NVM_NUMBER_OF_CALC_CRC8_BLOCKS > 0U)

  /* If current block use 8 bit crc */
  if(NVM_BD_CRCTYPE(BlockDesc) == NVM_BD_CRCTYPE_CRC8)
  {
    /* Calculate CRC8 and store the result in the CRC buffer as a 8-bit value.
     */
    *CrcBufPtr8 = Crc_CalculateCRC8(CrcDataAdress, Length, *CrcBufPtr8, FirstCall);
  }
#endif
}
#if ((NVM_SET_RAM_BLOCK_STATUS_API == STD_ON) && (NVM_RAMBLOCK_CRC_BUFFER_SIZE > 0U))
/* Add an element to the top */
FUNC(void, NVM_CODE) NvM_CalcCrc_EnqueueElement( uint16 BlockNumber )
{
    NvM_CrcQueue[NvM_RamBlockCrcCalcCount] = BlockNumber;
    NvM_RamBlockCrcCalcCount++;
    NvM_AdminBlockTable[BlockNumber].NvMExtendedStatus |= NVM_ES_RAM_BLOCK_CRC_CALC_MASK;
}

/* Peek the element from the bottom */
FUNC(uint16, NVM_CODE) NvM_CalcCrc_PeekElement( void )
{
  return NvM_CrcQueue[0U];
}
/* Remove NvM_GlobalBlockNumber from queue */
FUNC(void, NVM_CODE) NvM_CalcCrc_RemoveElement( void )
{
#if(NVM_NUMBER_OF_CRC_BLOCKS > 1)
const uint16 MaxIterator = NvM_RamBlockCrcCalcCount;
  uint16 BlockPosition = 0U;
  uint8 BlockFound = 0U;
#endif
    SchM_Enter_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
/* No need to shift blocks if only one with CRC */
#if(NVM_NUMBER_OF_CRC_BLOCKS > 1)
    /* update count */
    for(BlockPosition=0U; BlockPosition < MaxIterator; BlockPosition++ )
    {
        /* Block found update queue */
        if(1U == BlockFound)
        {
            NvM_CrcQueue[BlockPosition-1U] = NvM_CrcQueue[BlockPosition];
        }
        else
        {
            /* Search element */
            if(NvM_CrcQueue[BlockPosition] == NvM_GlobalBlockNumber)
            {
                /* set internal flag */
                BlockFound = 1U;
#endif
                /* Reset the RAM block CRC calculation flag */
                NvM_RamBlockCrcCalcCount--;
                NVM_CLEAR_EXT_STATUS(NVM_ES_RAM_BLOCK_CRC_CALC_MASK);
#if(NVM_NUMBER_OF_CRC_BLOCKS > 1)
            }
        }
    }
#endif
SchM_Exit_NvM_SCHM_NVM_EXCLUSIVE_AREA_0();
}
#endif
/*==================[internal function definitions]==========================*/

STATIC FUNC(void, NVM_CODE) NvM_CalcCrc_AsyncState(void)
{
  /* Update the remaining length */
  uint32 Length = NvM_CalcCrc_UpdateLength();
  NvM_CalcCrc_ProcessCrc(NvM_CalcCrc_Address, (NvM_PtrToApplDataType)&NvM_CalcCrc_CalcBuffer,
                         Length, NvM_CurrentBlockDescriptorPtr->blockDesc, FALSE);
}

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*==================[end of file]============================================*/
