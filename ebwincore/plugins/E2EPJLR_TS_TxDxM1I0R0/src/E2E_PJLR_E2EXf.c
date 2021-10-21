/**
 * \file
 *
 * \brief AUTOSAR E2EPJLR
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPJLR.
 *
 * \version 1.0.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <E2E_PJLR_E2EXf.h>  /* E2EXf APIs for E2E Profile JLR */

#if (E2EXF_IN_USE == STD_ON)

#include <E2E_PXX_E2EXf.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/* Call PJLR dependent check functions */
static FUNC(Std_ReturnType, E2E_CODE) E2EXf_Inv_PJLRCheck
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_PJLRCheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_PJLRCheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

/*------------------- E2EXf_PJLRInPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_PJLRInPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_VAR_CLEARED) ProtectStatePtr,
    P2CONST(E2EXf_PJLRProtectCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint16 BufferLength;

  /* check if partition is initialized */
  if(E2EXf_PartitionInitState == E2EXF_ONLINE)
  {
    /* get address of general transformer config */
    CONSTP2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_CONST) PXXConfigPtr =
        &(ConfigPtr->PXXConfig);

    /* returns E_SAFETY_HARD_RUNTIMEERROR if one of the conditions fails, otherwise okay */
    RetVal = E2EXf_InPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr,
        InputBufferLength, PXXConfigPtr);

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      /* get config addresses */
      CONSTP2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_CONST) ProfileCfgPtr =
          &(ConfigPtr->PJLRConfig);

      Std_ReturnType RetValProt;

      /* prepare in-place buffer */
      E2EXf_InPlaceShiftUpperHeaderToLeft(BufferPtr, PXXConfigPtr);

      /* set buffer length */
      BufferLength = (uint16)((InputBufferLength + ((E2EXF_XFRMBUFFERLENGTH_TYPE)PXXConfigPtr->HeaderLength / 8U)) & 0xFFFFU);
      *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) BufferLength;

      /* call protection function */
      RetValProt = E2E_PJLRProtect(ProfileCfgPtr, ProtectStatePtr, BufferPtr, BufferLength);

      if(RetValProt != E2E_E_OK)
      {
        RetVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_PJLROutOfPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_PJLROutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_VAR_CLEARED) ProtectStatePtr,
    P2CONST(E2EXf_PJLRProtectCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint16 BufferLength;

  /* check if partition is initialized */
  if(E2EXf_PartitionInitState == E2EXF_ONLINE)
  {
    /* get address of general transformer config */
    CONSTP2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_CONST) PXXConfigPtr =
        &(ConfigPtr->PXXConfig);

    /* returns E_SAFETY_HARD_RUNTIMEERROR if one of the conditions fails, otherwise okay */
    RetVal = E2EXf_OutOfPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr,
        InputBufferPtr, InputBufferLength, PXXConfigPtr);

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      /* get config addresses */
      CONSTP2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_CONST) ProfileCfgPtr =
          &(ConfigPtr->PJLRConfig);

      Std_ReturnType RetValProt;

      /* copy buffer out-of-place */
      E2EXf_CopyOutOfPlaceBuffer(BufferPtr, InputBufferPtr, InputBufferLength, PXXConfigPtr);

      /* set buffer length */
      BufferLength = (uint16)((InputBufferLength + ((E2EXF_XFRMBUFFERLENGTH_TYPE)PXXConfigPtr->HeaderLength / 8U)) & 0xFFFFU);
      *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) BufferLength;

      /* call protection function */
      RetValProt = E2E_PJLRProtect(ProfileCfgPtr, ProtectStatePtr, BufferPtr, BufferLength);

      if(RetValProt != E2E_E_OK)
      {
        RetVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_Inv_PJLRInPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_PJLRInPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_PJLRCheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_PJLRCheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;

  /* check if inverted transformer is initialized */
  if(E2EXf_PartitionInitState == E2EXF_ONLINE)
  {
    /* get address of general transformer config */
    CONSTP2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_CONST) PXXConfigPtr =
        &(ConfigPtr->PXXConfig);

    /* returns E_SAFETY_HARD_RUNTIMEERROR if one of the conditions fails, otherwise okay */
    RetVal = E2EXf_Inv_InPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr,
        InputBufferLength, PXXConfigPtr);

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      RetVal = E2EXf_Inv_PJLRCheck(BufferPtr, InputBufferLength, CheckStatesPtr, ConfigPtr);

      /* prepare in-place buffer */
      E2EXf_Inv_InPlaceShiftUpperHeaderToRight(BufferPtr, PXXConfigPtr);

      /* set buffer length */
      if(InputBufferLength == 0U)
      {
        *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) 0U;
      }
      else
        /* InputBufferLength >= HeaderLength / 8 */
      {
        *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) (InputBufferLength - ((E2EXF_XFRMBUFFERLENGTH_TYPE)PXXConfigPtr->HeaderLength / 8U));
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_Inv_PJLROutOfPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_PJLROutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_PJLRCheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_PJLRCheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;

  /* check if inverted transformer is initialized */
  if(E2EXf_PartitionInitState == E2EXF_ONLINE)
  {
    /* get address of general transformer config */
    CONSTP2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_CONST) PXXConfigPtr =
        &(ConfigPtr->PXXConfig);

    /* returns E_SAFETY_HARD_RUNTIMEERROR if one of the conditions fails, otherwise okay */
    RetVal = E2EXf_Inv_OutOfPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr, InputBufferPtr,
        InputBufferLength, PXXConfigPtr);

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      RetVal = E2EXf_Inv_PJLRCheck(InputBufferPtr, InputBufferLength, CheckStatesPtr, ConfigPtr);

      /* copy buffer out-of-place */
      E2EXf_Inv_CopyOutOfPlaceBuffer(BufferPtr, InputBufferPtr, InputBufferLength, PXXConfigPtr);

      /* set buffer length */
      if(InputBufferLength == 0U)
      {
        *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) 0U;
      }
      else
        /* InputBufferLength >= HeaderLength / 8 */
      {
        *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE) (InputBufferLength - ((E2EXF_XFRMBUFFERLENGTH_TYPE)PXXConfigPtr->HeaderLength / 8U));
      }
    }
  }

  return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2EXf_Inv_PJLRCheck
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_PJLRCheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_PJLRCheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  Std_ReturnType RetValCheck;
  E2E_PCheckStatusType RetValMapStatusToSM;
  uint16 BufferLength = (uint16)(InputBufferLength & 0xFFFFU);

  /* get config addresses */
  CONSTP2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_CONST) ProfileConfigPtr =
      &(ConfigPtr->PJLRConfig);
  CONSTP2CONST(E2E_SMConfigType, AUTOMATIC, E2E_CONST) SMConfigPtr =
      &(ConfigPtr->SMConfig);
  /* get RAM addresses */
  CONSTP2VAR(E2E_PJLRCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatePtr =
      &(CheckStatesPtr->PJLRCheckState);
  CONSTP2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr =
      &(CheckStatesPtr->SMCheckState);

  RetValCheck = E2E_PJLRCheck(ProfileConfigPtr, CheckStatePtr, BufferPtr, BufferLength);

  /* get profile independent state status */
  RetValMapStatusToSM = E2E_PJLRMapStatusToSM(RetValCheck, CheckStatePtr->Status);

  RetVal = E2EXf_Inv_SMCheck(RetValMapStatusToSM, SMConfigPtr, SMCheckStatePtr);

  return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#else /* E2EXF_IN_USE == STD_ON */

#include <E2E_Defs.h>           /* prevent empty translation unit */

/* Avoid empty translation unit according to ISO C90 */
E2E_PREVENTEMPTYTRANSLATIONUNIT;
#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[end of file]============================================*/
