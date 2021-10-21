/**
 * \file
 *
 * \brief AUTOSAR E2E
 *
 * This file contains the implementation of the AUTOSAR
 * module E2E.
 *
 * \version 2.0.18
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
 * A conversion should not be performed from pointer
 * to void into pointer to object.
 *
 * Reason:
 * The uint8 pointer cast is used for byte wise
 * initialization of the complete VersionInfo
 * struct with zeros in order to ensure
 * compatibility with ASR 3.2.
 *
 */

/*==================[inclusions]=============================================*/

#include <E2E_PXX_E2EXf.h>

#if (E2EXF_IN_USE == STD_ON)

#include <E2E_Mem.h>               /* E2E_MemCpy */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

FUNC(uint8, E2E_CODE) E2EXf_InPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  Std_ReturnType RetVal;
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;

  /* Following check expressions are split up to avoid problems with coverage instrumentation */
  if(BufferPtr == NULL_PTR)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  /* valid buffer pointer BufferPtr, but invalid header configuration  */
  else if(InputBufferLength < UpperHeaderBytesToShift)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if(BufferLengthPtr == NULL_PTR)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

FUNC(uint8, E2E_CODE) E2EXf_OutOfPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  Std_ReturnType RetVal;
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;

  /* Following check expressions are split up to avoid problems with coverage instrumentation */
  if(InputBufferPtr == NULL_PTR)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  /* valid input buffer pointer InputBufferPtr, but invalid header configuration  */
  else if(InputBufferLength < UpperHeaderBytesToShift)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if((BufferLengthPtr == NULL_PTR) || (BufferPtr == NULL_PTR))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

FUNC(void, E2E_CODE) E2EXf_InPlaceShiftUpperHeaderToLeft
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;
  /* MCG checks for a multiple of 8 */
  const uint16 E2EHeaderLengthBytes = PXXConfigPtr->HeaderLength / 8U;

  if(UpperHeaderBytesToShift > 0U)
  {
    /* copy header to front */
    E2E_MemCpy(BufferPtr, &BufferPtr[E2EHeaderLengthBytes], UpperHeaderBytesToShift);
  }
}

FUNC(void, E2E_CODE) E2EXf_CopyOutOfPlaceBuffer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;
  /* MCG checks for a multiple of 8 */
  const uint16 E2EHeaderLengthBytes = PXXConfigPtr->HeaderLength / 8U;

  /* copy upper header - holds also for UpperHeaderBytesToShift 0 */
  E2E_MemCpy(BufferPtr, InputBufferPtr, UpperHeaderBytesToShift);

  /* copy payload - the input parameter checks ensure that InputBufferLength is greater or equal
     to UpperHeaderBytesToShift */
  E2E_MemCpy(&BufferPtr[UpperHeaderBytesToShift + E2EHeaderLengthBytes],
      &InputBufferPtr[UpperHeaderBytesToShift],
      (E2EXF_XFRMBUFFERLENGTH_TYPE)(InputBufferLength - UpperHeaderBytesToShift));
}

FUNC(void, E2E_CODE) E2EXf_Inv_InPlaceShiftUpperHeaderToRight
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;
  /* MCG checks for a multiple of 8 */
  const uint16 E2EHeaderLengthBytes = PXXConfigPtr->HeaderLength / 8U;

  if((BufferPtr != NULL_PTR) && (UpperHeaderBytesToShift > 0U))
  {
    uint16 Idx;
    uint16 Offset;

    /* E2E_MemCpy is not used, since it starts copying at the beginning.
       Here, reverse copying should be applied to avoid inconsistencies when
       UpperHeaderBytesToShift is greater than E2EHeaderLengthBytes */
    for (Idx = 0U; Idx < UpperHeaderBytesToShift; Idx++)
    {
      Offset = (uint16)((UpperHeaderBytesToShift - Idx - (uint16)1U) & 0xFFFFU);
      BufferPtr[(uint16)(Offset + E2EHeaderLengthBytes)] = (uint8)(BufferPtr[Offset]);
    }
  }
}

FUNC(void, E2E_CODE) E2EXf_Inv_CopyOutOfPlaceBuffer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  /* MCG checks for a multiple of 8 */
  const uint16 UpperHeaderBytesToShift = PXXConfigPtr->UpperHeaderBitsToShift / 8U;
  /* MCG checks for a multiple of 8 */
  const uint16 E2EHeaderLengthBytes = PXXConfigPtr->HeaderLength / 8U;

  if(InputBufferPtr != NULL_PTR)
  {
    if(UpperHeaderBytesToShift > 0U)
    {
      /* copy header */
      E2E_MemCpy(BufferPtr, InputBufferPtr, UpperHeaderBytesToShift);
    }

    /* copy payload (for UpperHeaderBitsToShift >= 0) */
    E2E_MemCpy(&BufferPtr[UpperHeaderBytesToShift],
        &InputBufferPtr[UpperHeaderBytesToShift + E2EHeaderLengthBytes],
        (E2EXF_XFRMBUFFERLENGTH_TYPE)(InputBufferLength - UpperHeaderBytesToShift - E2EHeaderLengthBytes));
  }
}

FUNC(uint8, E2E_CODE) E2EXf_Inv_InPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  Std_ReturnType RetVal;
  /* header length of previous transformer and E2E transformer in bytes,
     MCG checks for a multiple of 8 for both parameters */
  const uint16 OverallHeaderLengthBytes = (PXXConfigPtr->UpperHeaderBitsToShift + PXXConfigPtr->HeaderLength) / 8U;

  /* Following check expressions are split up to avoid problems with coverage instrumentation */
  if((BufferPtr == NULL_PTR) && (InputBufferLength != E2EXF_INPUT_BUFFER_LENGTH_ZERO))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if((BufferPtr != NULL_PTR) && (InputBufferLength < OverallHeaderLengthBytes))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if(BufferLengthPtr == NULL_PTR)
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

FUNC(uint8, E2E_CODE) E2EXf_Inv_OutOfPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
)
{
  Std_ReturnType RetVal;
  /* header length of previous transformer and E2E transformer in bytes,
     MCG checks for a multiple of 8 for both parameters */
  const uint16 OverallHeaderLengthBytes = (PXXConfigPtr->UpperHeaderBitsToShift + PXXConfigPtr->HeaderLength) / 8U;

  /* Following check expressions are split up to avoid problems with coverage instrumentation */
  if((InputBufferPtr == NULL_PTR) && (InputBufferLength != E2EXF_INPUT_BUFFER_LENGTH_ZERO))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if((InputBufferPtr != NULL_PTR) && (InputBufferLength < OverallHeaderLengthBytes))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if((BufferLengthPtr == NULL_PTR) || (BufferPtr == NULL_PTR))
  {
    RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

FUNC(uint8, E2E_CODE) E2EXf_Inv_SMCheck
(
    E2E_PCheckStatusType RetValMapStatusToSM,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) SMConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  Std_ReturnType RetValSMCheck;

  RetValSMCheck = E2E_SMCheck(RetValMapStatusToSM, SMConfigPtr, SMCheckStatePtr);

  if(RetValSMCheck == E2E_E_OK)
  {
    /* low nibble of SMState (E2E_SMStateType) of SMCheckStatePtr shifted to high nibble */
    uint8 NibbleSMState = (SMCheckStatePtr->SMState & (uint8)E2EXF_LOW_NIBBLE_MASK) << (uint8)4U;

    /* low nibble of profile-independent check status (E2E_PCheckStatusType) -ProfileStatus */
    uint8 NibbleProfileStatus = (RetValMapStatusToSM & (uint8)E2EXF_LOW_NIBBLE_MASK);

    /* set both nibbles of RetVal (NibbleSMState, NibbleProfileStatus) */
    RetVal = NibbleSMState | NibbleProfileStatus;
  }
  else
  {
    RetVal = E_SAFETY_SOFT_RUNTIMEERROR;
  }

  return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#else

/* Avoid empty translation unit according to ISO C90 */
typedef void E2EPreventEmptyTranslationUnit_1_Type;

#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[internal function definition]===========================*/

/*==================[end of file]============================================*/
