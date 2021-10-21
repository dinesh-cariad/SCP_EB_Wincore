/**
 * \file
 *
 * \brief AUTOSAR SomeIpXf
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpXf.
 *
 * \version 1.0.47
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
 *
 */

/*==================[inclusions]============================================*/

#include <SomeIpXf_Int.h>          /* internal functions and types */
#include <TransformerTypes.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define SOMEIPXF_START_SEC_CODE
#include <SomeIpXf_MemMap.h>

/*-------------------- SomeIpXf_Int_GenericTransformer ----------------------*/
/* CHECK: NOPARSE */
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Int_GenericTransformer (
  P2VAR(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  P2CONST(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
)
{
  uint8 RetVal = E_SER_GENERIC_ERROR;

  /* Deviation MISRAC2012-1 <+4> */
  SOMEIPXF_PARAM_UNUSED(BufPtr);
  SOMEIPXF_PARAM_UNUSED(DataPtr);
  SOMEIPXF_PARAM_UNUSED(ConfigPtr);
  SOMEIPXF_PARAM_UNUSED(AdminData);

  /* No additional QM Types supported */
  /* If QM Type is added then remove NOPARSE here and in SomeIpXf.c.m4 */

  return RetVal;
}
/* CHECK: PARSE */
/*------------------- SomeIpXf_Int_Inv_GenericTransformer --------------------*/
/* CHECK: NOPARSE */
FUNC(uint8, SOMEIPXF_CODE) SomeIpXf_Int_Inv_GenericTransformer (
  P2CONST(uint8, AUTOMATIC, SOMEIPXF_APPL_DATA) BufPtr,
  SOMEIPXF_XFRMBUFFERLENGTH_TYPE BufferLength,
  P2VAR(void, AUTOMATIC, SOMEIPXF_APPL_DATA) DataPtr,
  P2VAR(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_DATA) BytesCopiedPtr,
  P2CONST(SOMEIPXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, SOMEIPXF_APPL_CONST) ConfigPtr,
  P2VAR(SomeIpXf_AdminDataType, AUTOMATIC, SOMEIPXF_APPL_DATA) AdminData
)
{
  uint8 RetVal = E_SER_GENERIC_ERROR;

  /* Deviation MISRAC2012-1 <+6> */
  SOMEIPXF_PARAM_UNUSED(BufPtr);
  SOMEIPXF_PARAM_UNUSED(BufferLength);
  SOMEIPXF_PARAM_UNUSED(DataPtr);
  SOMEIPXF_PARAM_UNUSED(BytesCopiedPtr);
  SOMEIPXF_PARAM_UNUSED(ConfigPtr);
  SOMEIPXF_PARAM_UNUSED(AdminData);

  /* No additional QM Types supported */
  /* If QM Type is added then remove NOPARSE here and in SomeIpXf.c.m4 */

  return RetVal;
}
/* CHECK: PARSE */

#define SOMEIPXF_STOP_SEC_CODE
#include <SomeIpXf_MemMap.h>

  /*==================[end of file]===========================================*/
