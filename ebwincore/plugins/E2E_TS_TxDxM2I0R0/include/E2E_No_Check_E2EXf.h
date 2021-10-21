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
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 */

#ifndef E2E_NO_CHECK_E2EXF_H
#define E2E_NO_CHECK_E2EXF_H

/*==================[inclusions]============================================*/

#include <E2E_E2EXf_Stc.h>      /* Static definitions of constants and types */

/*==================[macros]================================================*/

/* Deviation MISRAC2012-1 <START> */

#if (E2EXF_IN_USE == STD_ON)

#if (defined E2EXF_DEF_CFG_INVXF_NO_CHECK_INPLACE) /* to prevent double declaration */
#error E2EXF_DEF_CFG_INVXF_NO_CHECK_INPLACE already defined
#endif /* if (defined E2EXF_DEF_CFG_INVXF_NO_CHECK_INPLACE) */
/** \brief Define configuration data of E2E in-place check-transformer with disabled end-to-end
     check */
#define E2EXF_DEF_CFG_INVXF_NO_CHECK_INPLACE(XfrmName) \
    static CONST(E2EXf_NoCheck_CheckCfgType, E2EXF_VAR_CLEARED) E2EXfNoCheck_CheckCfgType_##XfrmName

#if (defined E2EXF_DEF_CFG_INVXF_NO_CHECK_OUTOFPLACE) /* to prevent double declaration */
#error E2EXF_DEF_CFG_INVXF_NO_CHECK_OUTOFPLACE already defined
#endif /* if (defined E2EXF_DEF_CFG_INVXF_NO_CHECK_OUTOFPLACE) */
/** \brief Define configuration data of E2E check-transformer with disabled end-to-end check */
#define E2EXF_DEF_CFG_INVXF_NO_CHECK_OUTOFPLACE(XfrmName) \
    E2EXF_DEF_CFG_INVXF_NO_CHECK_INPLACE(XfrmName)

#if (defined E2EXF_DEF_FUNC_INVXF_NO_CHECK_INPLACE) /* to prevent double declaration */
#error E2EXF_DEF_FUNC_INVXF_NO_CHECK_INPLACE already defined
#endif /* if (defined E2EXF_DEF_FUNC_INVXF_NO_CHECK_INPLACE) */
/** \brief Define E2E in-place check-transformer function with disabled end-to-end check */
#define E2EXF_DEF_FUNC_INVXF_NO_CHECK_INPLACE(XfrmName) \
    FUNC(uint8, E2EXF_CODE) XfrmName \
    ( \
        P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) BufferPtr, \
        P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2EXF_APPL_DATA) BufferLengthPtr, \
        E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength \
    ) \
    { \
        return E2EXf_Inv_NoCheckInPlace_Transformer(BufferPtr, \
                 BufferLengthPtr, \
                 InputBufferLength, \
                 &(E2EXfNoCheck_CheckCfgType_##XfrmName), \
                 E2EXF_EB_PARTITIONINITSTATEVAR_NAME); \
    }

#if (defined E2EXF_DEF_FUNC_INVXF_NO_CHECK_OUTOFPLACE) /* to prevent double declaration */
#error E2EXF_DEF_FUNC_INVXF_NO_CHECK_OUTOFPLACE already defined
#endif /* if (defined E2EXF_DEF_FUNC_INVXF_NO_CHECK_OUTOFPLACE) */
/** \brief Define E2E out-of-place check-transformer function with disabled end-to-end check */
#define E2EXF_DEF_FUNC_INVXF_NO_CHECK_OUTOFPLACE(XfrmName) \
    FUNC(uint8, E2EXF_CODE) XfrmName \
    ( \
        P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) BufferPtr, \
        P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2EXF_APPL_DATA) BufferLengthPtr, \
        P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) InputBufferPtr, \
        E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength \
    ) \
    { \
        return E2EXf_Inv_NoCheckOutOfPlace_Transformer(BufferPtr, \
                 BufferLengthPtr, \
                 InputBufferPtr, \
                 InputBufferLength, \
                 &(E2EXfNoCheck_CheckCfgType_##XfrmName), \
                 E2EXF_EB_PARTITIONINITSTATEVAR_NAME); \
    }

/* Deviation MISRAC2012-1 <STOP> */

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/* E2E check-transformer configuration data for transformer with disabled end-to-end check */
typedef struct
{
    E2EXf_PXXConfigType PXXConfig;
} E2EXf_NoCheck_CheckCfgType;

/*==================[external function declarations]========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/** \brief Declare E2E in-place inverted (check-)transformer with disabled end-to-end check */
extern FUNC(uint8, E2E_CODE) E2EXf_Inv_NoCheckInPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_NoCheck_CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
);

/** \brief Declare E2E out-of-place inverted (check-)transformer with disabled end-to-end check */
extern FUNC(uint8, E2E_CODE) E2EXf_Inv_NoCheckOutOfPlace_Transformer
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) BufferPtr,
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2E_APPL_DATA) BufferLengthPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) InputBufferPtr,
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength,
    P2CONST(E2EXf_NoCheck_CheckCfgType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    E2EXf_StateType E2EXf_PartitionInitState
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

#endif /* ifndef E2E_NO_CHECK_E2EXF_H */
/*==================[end of file]===========================================*/
