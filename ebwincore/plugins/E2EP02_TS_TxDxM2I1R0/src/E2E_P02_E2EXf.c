/**
 * \file
 *
 * \brief AUTOSAR E2EP02
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP02.
 *
 * \version 2.1.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 2.2 (required)
 * There shall be no dead code.
 *
 * Reason:
 * The macro E2E_PARAM_UNUSED causes a False Positive Misra warning.
 * The macro is needed to avoid a compiler warning and can't
 * be written differently.
 * It expands to a void cast which is an exception of the rule 2.2
 * according to the MISRA-C:2012 Guidelines March 2013.
*/

/*==================[inclusions]=============================================*/

#include <E2E_P02_E2EXf.h>   /* E2EXf APIs for E2E Profile P02 */
#include <E2E_Defs.h>        /* E2E_PARAM_UNUSED / E2E_PREVENTEMPTYTRANSLATIONUNIT */

#if (E2EXF_IN_USE == STD_ON)

#include <E2E_Mem.h>         /* E2E_MemCpy */
#include <E2E_PXX_E2EXf.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/* Call P02 dependend check functions */
static FUNC(Std_ReturnType, E2E_CODE) E2EXf_Inv_P02Check
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatePtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr,
    P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ProfileConfigPtr,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) SMConfigPtr,
    boolean ProfileBehavior
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

/*------------------- E2EXf_P02InPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_P02InPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_VAR_CLEARED) ProtectStatePtr,
    P2CONST(E2EXf_P02ProtectCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
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
      CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_CONST) ProfileCfgPtr =
          &(ConfigPtr->P02Config);

      /* get header length of general transformer config */
      uint16 HeaderLength = PXXConfigPtr->HeaderLength;

      Std_ReturnType RetValProt;

      /* prepare in-place buffer */
      E2EXf_InPlaceShiftUpperHeaderToLeft(BufferPtr, PXXConfigPtr);

      /* set buffer length */
       BufferLength = (uint16)((InputBufferLength + ((E2EXF_XFRMBUFFERLENGTH_TYPE)HeaderLength / 8U)) & 0xFFFFU);
      *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE)BufferLength;

      /* return immediately if check of buffer length fails */
      if(*BufferLengthPtr != ((E2EXF_XFRMBUFFERLENGTH_TYPE)ProfileCfgPtr->DataLength / 8U))
      {
        RetVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* call protection function */
        RetValProt = E2E_P02Protect(ProfileCfgPtr, ProtectStatePtr, BufferPtr);

        if(RetValProt != E2E_E_OK)
        {
          RetVal = E_SAFETY_HARD_RUNTIMEERROR;
        }
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_P02OutOfPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_P02OutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_VAR_CLEARED) ProtectStatePtr,
    P2CONST(E2EXf_P02ProtectCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
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
    RetVal = E2EXf_OutOfPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr, InputBufferPtr,
        InputBufferLength, PXXConfigPtr);

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      /* get config addresses */
      CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_CONST) ProfileCfgPtr =
          &(ConfigPtr->P02Config);

      /*  get header length of general transformer config */
      uint16 HeaderLength = PXXConfigPtr->HeaderLength;
      Std_ReturnType RetValProt;

      /* copy buffer out-of-place */
      E2EXf_CopyOutOfPlaceBuffer(BufferPtr, InputBufferPtr, InputBufferLength, PXXConfigPtr);

      /* set buffer length */
      BufferLength = (uint16)((InputBufferLength + ((E2EXF_XFRMBUFFERLENGTH_TYPE)HeaderLength / 8U)) & 0xFFFFU );
      *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE)BufferLength;

      /* return immediately if check of buffer length fails */
      if(*BufferLengthPtr != ((E2EXF_XFRMBUFFERLENGTH_TYPE)ProfileCfgPtr->DataLength / 8U))
      {
        RetVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* call protection function */
        RetValProt = E2E_P02Protect(ProfileCfgPtr, ProtectStatePtr, BufferPtr);

        if(RetValProt != E2E_E_OK)
        {
          RetVal = E_SAFETY_HARD_RUNTIMEERROR;
        }
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_Inv_P02InPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_P02InPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_P02CheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_P02CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;

  /* check if inverted transformer is initialized */
  if(E2EXf_PartitionInitState == E2EXF_ONLINE)
  {
    /* get config address */
    CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_CONST) ProfileConfigPtr =
        &(ConfigPtr->P02Config);
    CONSTP2CONST(E2E_SMConfigType, AUTOMATIC, E2E_CONST) SMConfigPtr =
        &(ConfigPtr->SMConfig);
    CONSTP2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_CONST) PXXConfigPtr =
        &(ConfigPtr->PXXConfig);

    /* get RAM addresses */
    CONSTP2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatePtr =
        &(CheckStatesPtr->P02CheckState);
    CONSTP2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr =
        &(CheckStatesPtr->SMCheckState);

    /* returns E_SAFETY_HARD_RUNTIMEERROR if one of the conditions fails, otherwise okay */
    RetVal = E2EXf_Inv_InPlaceTransPreCondCheck(BufferPtr, BufferLengthPtr,
        InputBufferLength, PXXConfigPtr);

    /* set NewDataAvailable */
    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      /* BufferPtr is null is only possible with InputBufferLength 0. For InputBufferLength greater
         0, the precondition check fails already and this part of code is not reached */
      if(BufferPtr == NULL_PTR)
      {
        CheckStatePtr->NewDataAvailable = FALSE;
      }
      /* BufferPtr is not null and InputBufferLength holds configured data length */
      else if(InputBufferLength == ((E2EXF_XFRMBUFFERLENGTH_TYPE)ProfileConfigPtr->DataLength / 8U))
      {
        CheckStatePtr->NewDataAvailable = TRUE;
      }
      else
      {
        RetVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
    }

    /* Precondition checks successful */
    if(E_OK == RetVal)
    {
      RetVal = E2EXf_Inv_P02Check(BufferPtr, InputBufferLength, CheckStatePtr, SMCheckStatePtr,
          ProfileConfigPtr, SMConfigPtr, PXXConfigPtr->ProfileBehavior);

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


/*------------------- E2EXf_Inv_P02OutOfPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_P02OutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2EXf_P02CheckStatesType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatesPtr,
    P2CONST(E2EXf_P02CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
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
      /* Copy InputBuffer to Buffer to re-use E2EXf_Inv_P02InPlace_Transformer function */
      if(InputBufferPtr == NULL_PTR)
      {
        BufferPtr = NULL_PTR;
      }
      else
      {
        E2E_MemCpy(BufferPtr, InputBufferPtr, InputBufferLength);
      }

      /* Re-use in-place inv transformer function to avoid duplicated code. Note that
         precondition checks are performed again. */
      RetVal = E2EXf_Inv_P02InPlace_Transformer(BufferPtr, BufferLengthPtr, InputBufferLength,
          CheckStatesPtr, ConfigPtr, E2EXf_PartitionInitState);
    }
  }

  return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2EXf_Inv_P02Check
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_VAR_CLEARED) CheckStatePtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr,
    P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ProfileConfigPtr,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) SMConfigPtr,
    boolean ProfileBehavior
)
{
  Std_ReturnType RetVal = E_SAFETY_HARD_RUNTIMEERROR;
  Std_ReturnType RetValCheck;
  E2E_PCheckStatusType RetValMapStatusToSM;

  /* data byte for call of E2E_P02Check if no new data are available */
  const uint8 SingleByte = 0U;
  /* initialize data pointer for call of E2E_P02Check */
  P2CONST(uint8, AUTOMATIC, E2E_CONST) DataPtr = BufferPtr;

  /* The pre condition check of the inverted transformer function excludes buffer pointer is null
   * and input buffer length greater 0. With this pre condition check performed before, the check
   * of no new data with (BufferPtr == NULL_PTR) && (InputBufferLength == 0U)) can be reduced. */
  /* Deviation MISRAC2012-1 */
  E2E_PARAM_UNUSED(InputBufferLength);
  if(BufferPtr == NULL_PTR)
  {
    DataPtr = &SingleByte;
  }

  RetValCheck = E2E_P02Check(ProfileConfigPtr, CheckStatePtr, DataPtr);

  /* profileBehavior is not R4_2 (true), its PRE_R4_2 (false) */
  if(ProfileBehavior != TRUE)
  {
    CheckStatePtr->MaxDeltaCounter = ProfileConfigPtr->MaxDeltaCounterInit;
  }

  /* get profile independent state status */
  RetValMapStatusToSM = E2E_P02MapStatusToSM(RetValCheck, CheckStatePtr->Status, ProfileBehavior);

  RetVal = E2EXf_Inv_SMCheck(RetValMapStatusToSM, SMConfigPtr, SMCheckStatePtr);

  return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#else /* E2EXF_IN_USE == STD_ON */

/* Avoid empty translation unit according to ISO C90 */
E2E_PREVENTEMPTYTRANSLATIONUNIT;
#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[end of file]============================================*/
