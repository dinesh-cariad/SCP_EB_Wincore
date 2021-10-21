/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 2.4 (advisory)
  *     "A project should not contain unused tag declarations."
  *
  *     Reason:
  *     The tag structure shall be used to define a pointer to a function which is
  *     defined as a member inside an incomplete structure definition.
  */

#ifndef DCM_DSP_CONTEXTS_H
#define DCM_DSP_CONTEXTS_H

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */


/*==================[macros]=====================================================================*/

/** \brief Dynamically casts a specialized Dsp context into a base context */
#define DCM_DSP_CONTEXT(context) (&((context)->BaseContext))

/** \brief Wrapper to initialize the Dsp context more easily */
#if (defined DCM_DSP_CONTEXT_INIT)
  #error "DCM_DSP_CONTEXT_INIT is already defined"
#endif
#define DCM_DSP_CONTEXT_INIT(context, config)                                   \
  do                                                                            \
  {                                                                             \
    Dcm_Dsp_Context_Init(DCM_DSP_CONTEXT(context), (void*)(context), (config)); \
  } while (0)

/** \brief Accesses the base member 'next_state' of a specialized Dsp context */
#if (defined DCM_DSP_CONTEXT_NEXT)
  #error "DCM_DSP_CONTEXT_NEXT is already defined"
#endif
#define DCM_DSP_CONTEXT_NEXT(context) ((context)->next_state)

/** \brief Accesses the base member 'opstatus' of a specialized Dsp context */
#if (defined DCM_DSP_CONTEXT_OPSTATUS)
  #error "DCM_DSP_CONTEXT_OPSTATUS is already defined"
#endif
#define DCM_DSP_CONTEXT_OPSTATUS(context) ((context)->opstatus)

/** \brief Accesses the base member 'result' of a specialized Dsp context */
#if (defined DCM_DSP_CONTEXT_RESULT)
  #error "DCM_DSP_CONTEXT_RESULT is already defined"
#endif
#define DCM_DSP_CONTEXT_RESULT(context) ((context)->result)

/** \brief Accesses the base member 'nrc' of a specialized Dsp context */
#if (defined DCM_DSP_CONTEXT_NRC)
  #error "DCM_DSP_CONTEXT_NRC is already defined"
#endif
#define DCM_DSP_CONTEXT_NRC(context) ((context)->nrc)

/*==================[type definitions]===========================================================*/

/* struct Dcm_Dsp_ContextBase; */
typedef struct P2VAR(Dcm_Dsp_ContextBase, TYPEDEF, DCM_VAR) Dcm_Dsp_ContextBasePtrType;

typedef P2FUNC(void, DCM_CODE, Dcm_Dsp_ContextFunctionType)
    (Dcm_Dsp_ContextBasePtrType BaseContextPtr);

/** \brief Generic part of a Dsp context; used for switching through states */
/* Deviation MISRAC2012-1 */
typedef struct Dcm_Dsp_ContextBase
{
  /** \brief Table with functions to be executed when processing through states */
  P2CONST(Dcm_Dsp_ContextFunctionType, TYPEDEF, DCM_CODE) FunctionConfig;
  /** \brief Pointer to the specialized context (to self, to 'cast dynamically') */
  P2VAR(void, TYPEDEF, DCM_VAR) SpecialPtr;
  /** \brief Current state of the related function */
  uint8 current_state;
  /** \brief Programmed next state of the related function */
  uint8 next_state;
  /** \brief Requested service operation status */
  Dcm_OpStatusType opstatus;
  /** \brief Current result of related function */
  Std_ReturnType result;
  /** \brief Negative response code  */
  Dcm_NegativeResponseCodeType nrc;
} Dcm_Dsp_ContextBaseType;

typedef P2VAR(Dcm_Dsp_ContextBaseType, TYPEDEF, DCM_VAR) Dcm_Dsp_ContextPtr;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initialize the context of an asynchronous function */
extern FUNC(void, DCM_CODE) Dcm_Dsp_Context_Init(
  Dcm_Dsp_ContextPtr BaseContext,
  P2VAR(void, AUTOMATIC, DCM_VAR) SpecialPtr,
  P2CONST(Dcm_Dsp_ContextFunctionType, AUTOMATIC, DCM_CONST) FunctionConfig);

/** \brief Process an asynchronous function through its configured states */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_Context_Execute(Dcm_Dsp_ContextPtr BaseContext);

/** \brief Cancel a running asynchronous function */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_Context_Cancel(Dcm_Dsp_ContextPtr BaseContext);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/


#endif /* ifndef DCM_DSP_CONTEXTS_H */
/*==================[end of file]================================================================*/
