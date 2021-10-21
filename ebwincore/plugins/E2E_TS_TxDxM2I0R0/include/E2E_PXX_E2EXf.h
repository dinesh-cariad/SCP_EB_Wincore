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

#ifndef E2E_PXX_E2EXF_H
#define E2E_PXX_E2EXF_H

/*==================[inclusions]============================================*/

#include <E2E_E2EXf_Stc.h>      /* static declarations of non AUTOSAR APIs */

#if (E2EXF_IN_USE == STD_ON)

#include <E2E.h>
#include <E2E_SM.h>

/*==================[macros]================================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/* Checks preconditions for in-place protect transformer */
extern FUNC(uint8, E2E_CODE) E2EXf_InPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Checks preconditions for out-of-place protect transformer */
extern FUNC(uint8, E2E_CODE) E2EXf_OutOfPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Prepare in-place buffer of protect transformer */
extern FUNC(void, E2E_CODE) E2EXf_InPlaceShiftUpperHeaderToLeft
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Prepare out-of-place buffer of protect transformer by copying buffer and
   shifting upper header to left */
extern FUNC(void, E2E_CODE) E2EXf_CopyOutOfPlaceBuffer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Checks preconditions for in-place check transformer */
extern FUNC(uint8, E2E_CODE) E2EXf_Inv_InPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Checks preconditions for out-of-place protect transformer */
extern FUNC(uint8, E2E_CODE) E2EXf_Inv_OutOfPlaceTransPreCondCheck
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Prepare in-place buffer of check transformer */
extern FUNC(void, E2E_CODE) E2EXf_Inv_InPlaceShiftUpperHeaderToRight
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Copy buffer for out-of-place check transformer */
extern FUNC(void, E2E_CODE) E2EXf_Inv_CopyOutOfPlaceBuffer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_PXXConfigType, AUTOMATIC, E2E_APPL_CONST) PXXConfigPtr
);

/* Calls E2E_SMCheck and sets dependent return value */
extern FUNC(uint8, E2E_CODE) E2EXf_Inv_SMCheck
(
    E2E_PCheckStatusType RetValMapStatusToSM,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) SMConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_VAR_CLEARED) SMCheckStatePtr
);

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef E2E_PXX_E2EXF_H */
/*==================[end of file]===========================================*/
