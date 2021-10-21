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

/*==================[inclusions]=============================================*/

#include <E2E_PXX_E2EXf.h>
#include <E2E_No_Check_E2EXf.h>  /* Inverse transformer with disabled end-to-end check */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

#if (E2EXF_IN_USE == STD_ON)

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/*------------------- E2EXf_Inv_NoCheckInPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_NoCheckInPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_NoCheck_CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
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
          *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE)
              ((E2EXF_XFRMBUFFERLENGTH_TYPE) InputBufferLength -
                    (E2EXF_XFRMBUFFERLENGTH_TYPE) ( ((E2EXF_XFRMBUFFERLENGTH_TYPE) PXXConfigPtr->HeaderLength) / 8U) );
      }
    }
  }

  return RetVal;
}


/*------------------- E2EXf_Inv_NoCheckOutOfPlace_Transformer -------------------------*/
FUNC(uint8, E2E_CODE) E2EXf_Inv_NoCheckOutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_NoCheck_CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
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
        *BufferLengthPtr = (E2EXF_XFRMBUFFERLENGTH_TYPE)
            ((E2EXF_XFRMBUFFERLENGTH_TYPE) InputBufferLength -
                    (E2EXF_XFRMBUFFERLENGTH_TYPE) ( ((E2EXF_XFRMBUFFERLENGTH_TYPE) PXXConfigPtr->HeaderLength) / 8U) );
      }
    }
  }

  return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#else

/* Avoid empty translation unit according to ISO C90 */
typedef void E2EPreventEmptyTranslationUnit_0_Type;

#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[internal function definition]===========================*/

/*==================[end of file]============================================*/
