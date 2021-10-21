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

#ifndef E2E_E2EXF_STC_H
#define E2E_E2EXF_STC_H

/*==================[inclusions]============================================*/

#include <E2EXf_Cfg.h>

#if (E2EXF_IN_USE == STD_ON)

#include <TransformerTypes.h>    /* Transformer specific standard types */

/*==================[macros]================================================*/

#if (defined E2EXF_UNINIT) /* to prevent double declaration */
#error E2EXF_UNINIT already defined
#endif /* if (defined E2EXF_UNINIT) */

#if (defined E2EXF_ONLINE) /* to prevent double declaration */
#error E2EXF_ONLINE already defined
#endif /* if (defined E2EXF_ONLINE) */

/** \brief E2EXf_StateType */
#define E2EXF_UNINIT  0U
#define E2EXF_ONLINE  1U

#if (defined E2EXF_EB_CONCAT) /* to prevent double declaration */
#error E2EXF_EB_CONCAT already defined
#endif /* if (defined E2EXF_EB_CONCAT) */
/* Deviation MISRAC2012-1 */
#define E2EXF_EB_CONCAT(a,b) a ## b

#if (defined E2EXF_EB_PARTITIONINITSTATEVAR_NAME_HLP) /* to prevent double declaration */
#error E2EXF_EB_PARTITIONINITSTATEVAR_NAME_HLP already defined
#endif /* if (defined E2EXF_EB_PARTITIONINITSTATEVAR_NAME_HLP) */

#define E2EXF_EB_PARTITIONINITSTATEVAR_NAME_HLP(name) E2EXF_EB_CONCAT(name , _InitState)

#if (defined E2EXF_EB_PARTITIONINITSTATEVAR_NAME) /* to prevent double declaration */
#error E2EXF_EB_PARTITIONINITSTATEVAR_NAME already defined
#endif /* if (defined E2EXF_EB_PARTITIONINITSTATEVAR_NAME) */

#define E2EXF_EB_PARTITIONINITSTATEVAR_NAME E2EXF_EB_PARTITIONINITSTATEVAR_NAME_HLP(E2EXF_EB_PARTITION_INITFUNC_NAME)

#if (defined E2EXF_DEF_VAR_PARTITIONINITSTATE) /* to prevent double declaration */
#error E2EXF_DEF_VAR_PARTITIONINITSTATE already defined
#endif /* if (defined E2EXF_DEF_VAR_PARTITIONINITSTATE) */

#define E2EXF_DEF_VAR_PARTITIONINITSTATE \
static VAR(E2EXf_StateType, E2EXF_VAR) E2EXF_EB_PARTITIONINITSTATEVAR_NAME = E2EXF_UNINIT

#if (defined E2EXF_DEF_FUNC_PARTITIONINIT_START) /* to prevent double declaration */
#error E2EXF_DEF_FUNC_PARTITIONINIT_START already defined
#endif /* if (defined E2EXF_DEF_FUNC_PARTITIONINIT_START) */

#define E2EXF_DEF_FUNC_PARTITIONINIT_START \
FUNC(Std_ReturnType, E2EXF_CODE) E2EXF_EB_PARTITION_INITFUNC_NAME(void)\
{\
  Std_ReturnType RetVal = E2E_E_OK; /* Std_ReturnType is defined as uint8 */\
\

#if (defined E2EXF_DEF_FUNC_PARTITIONINIT_STOP) /* to prevent double declaration */
#error E2EXF_DEF_FUNC_PARTITIONINIT_STOP already defined
#endif /* if (defined E2EXF_DEF_FUNC_PARTITIONINIT_STOP) */

#define E2EXF_DEF_FUNC_PARTITIONINIT_STOP \
  if (RetVal == E2E_E_OK)\
  {\
     E2EXF_EB_PARTITIONINITSTATEVAR_NAME = E2EXF_ONLINE;\
  }\
\
  return RetVal;\
}

#if (defined E2EXF_DEF_FUNC_PARTITIONDEINIT) /* to prevent double declaration */
#error E2EXF_DEF_FUNC_PARTITIONDEINIT already defined
#endif /* if (defined E2EXF_DEF_FUNC_PARTITIONDEINIT) */

#define E2EXF_DEF_FUNC_PARTITIONDEINIT \
FUNC(void, E2EXF_CODE) E2EXF_EB_PARTITION_DEINITFUNC_NAME(void)\
{\
  E2EXF_EB_PARTITIONINITSTATEVAR_NAME = E2EXF_UNINIT;\
}

#if (defined E2EXF_DECL_FUNC_XF_INPLACE) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_XF_INPLACE already defined
#endif /* if (defined E2EXF_DECL_FUNC_XF_INPLACE) */

#define E2EXF_DECL_FUNC_XF_INPLACE(XfrmName)                                        \
extern FUNC(uint8, E2EXF_CODE) XfrmName                                             \
(                                                                                   \
    P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) BufferPtr,                             \
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2EXF_APPL_DATA) BufferLengthPtr, \
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength                                   \
)

#if (defined E2EXF_DECL_FUNC_INVXF_INPLACE) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_INVXF_INPLACE already defined
#endif /* if (defined E2EXF_DECL_FUNC_INVXF_INPLACE) */

#define E2EXF_DECL_FUNC_INVXF_INPLACE(XfrmName) \
E2EXF_DECL_FUNC_XF_INPLACE(XfrmName)

#if (defined E2EXF_DECL_FUNC_XF_OUTOFPLACE) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_XF_OUTOFPLACE already defined
#endif /* if (defined E2EXF_DECL_FUNC_XF_OUTOFPLACE) */

#define E2EXF_DECL_FUNC_XF_OUTOFPLACE(XfrmName)                                     \
extern FUNC(uint8, E2EXF_CODE) XfrmName                                             \
(                                                                                   \
    P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) BufferPtr,                             \
    P2VAR(E2EXF_XFRMBUFFERLENGTH_TYPE, AUTOMATIC, E2EXF_APPL_DATA) BufferLengthPtr, \
    P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) InputBufferPtr,                      \
    E2EXF_XFRMBUFFERLENGTH_TYPE InputBufferLength                                   \
)

#if (defined E2EXF_DECL_FUNC_INVXF_OUTOFPLACE) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_INVXF_OUTOFPLACE already defined
#endif /* if (defined E2EXF_DECL_FUNC_INVXF_OUTOFPLACE) */

#define E2EXF_DECL_FUNC_INVXF_OUTOFPLACE(XfrmName) \
E2EXF_DECL_FUNC_XF_OUTOFPLACE(XfrmName)

#if (defined E2EXF_DECL_FUNC_PARTITIONINIT) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_PARTITIONINIT already defined
#endif /* if (defined E2EXF_DECL_FUNC_PARTITIONINIT) */
/* Deviation MISRAC2012-1 */
#define E2EXF_DECL_FUNC_PARTITIONINIT(PartitionInfix) \
extern FUNC(Std_ReturnType, E2EXF_CODE) E2EXf ## PartitionInfix ## PartitionInit(void)

#if (defined E2EXF_DECL_FUNC_PARTITIONDEINIT) /* to prevent double declaration */
#error E2EXF_DECL_FUNC_PARTITIONDEINIT already defined
#endif /* if (defined E2EXF_DECL_FUNC_PARTITIONDEINIT) */
/* Deviation MISRAC2012-1 */
#define E2EXF_DECL_FUNC_PARTITIONDEINIT(PartitionInfix) \
extern FUNC(void, E2EXF_CODE) E2EXf ## PartitionInfix ## PartitionDeInit(void)

#if (defined E2EXF_LOW_NIBBLE_MASK)           /* To prevent double declaration */
#error E2EXF_LOW_NIBBLE_MASK already defined
#endif /* if (defined E2EXF_LOW_NIBBLE_MASK) */

/* Defines mask for the lower nibble of a byte */
#define  E2EXF_LOW_NIBBLE_MASK   0x0FU

#if (defined E2EXF_HIGH_NIBBLE_MASK)          /* To prevent double declaration */
#error E2EXF_HIGH_NIBBLE_MASK already defined
#endif /* if (defined E2EXF_HIGH_NIBBLE_MASK) */

/* Defines mask for the high nibble of a byte */
#define  E2EXF_HIGH_NIBBLE_MASK   0xF0U

#if (defined E2EXF_HEADER_LENGTH_16)          /* To prevent double declaration */
#error E2EXF_HEADER_LENGTH_16 already defined
#endif /* if (defined E2EXF_HEADER_LENGTH_16) */

/* Defines macro for header length of 16 bits */
#define  E2EXF_HEADER_LENGTH_16   16U

#if (defined E2EXF_BIT_OFFSET_12)             /* To prevent double declaration */
#error E2EXF_BIT_OFFSET_12 already defined
#endif /* if (defined E2EXF_BIT_OFFSET_12) */

/* Defines macro for offset of 12 bits to set nibble in profile 01 */
#define  E2EXF_BIT_OFFSET_12   12U

#if (defined E2EXF_INPUT_BUFFER_LENGTH_ZERO)  /* To prevent double declaration */
#error E2EXF_INPUT_BUFFER_LENGTH_ZERO already defined
#endif /* if (defined E2EXF_INPUT_BUFFER_LENGTH_ZERO) */

/* Defines macro for buffer length of 0 bits */
#define  E2EXF_INPUT_BUFFER_LENGTH_ZERO   0U

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/* The E2EXf_Init API has to be called with a NULL pointer instead of a
   configuration struct. To get use of the E2EXf_ConfigType and to be
   conform with the Autosar specification, it is defined as void here. */
/* !LINKSTO E2E.ASR421.SWS_E2EXf_00030.NoCheck,1 */
typedef VAR(void, TYPEDEF) E2EXf_ConfigType;

/* Type to reflect initialization state of a dedicated partition of E2EXf module */
typedef VAR(uint8, TYPEDEF) E2EXf_StateType;

/* Type of configuration part that is common for all transformers */
typedef struct
{
    uint16 UpperHeaderBitsToShift; /* EndToEndTransformationDescription.upperHeaderBitsToShift,
                                      needed by serializer and deserializer */
    uint16 HeaderLength; /* BufferProperties.HeaderLength in bits,
                            needed by serializer and deserializer */
    boolean ProfileBehavior; /* EndToEndTransformationDescription.profileBehavior;
                                TRUE by default if it is not specified,
                                i.e. for other profiles than P01 and P02 */
} E2EXf_PXXConfigType;

#endif /* (E2EXF_IN_USE == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef E2E_E2EXF_STC_H */
/*==================[end of file]===========================================*/
