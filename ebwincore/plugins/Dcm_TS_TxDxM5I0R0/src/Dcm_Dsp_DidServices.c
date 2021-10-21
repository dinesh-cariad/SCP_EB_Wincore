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

/* !LINKSTO Dcm.Dsn.File.DidServices.Impl,1 */
/* This file contains the implementation of the implementation of the DidServices software unit. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
 *  MISRA-2) Deviated Rule: 16.7 (required)
 *     "A pointer parameter in a function prototype should be declared as
 *     pointer to const if the pointer is not used to modify the addressed
 *     object."
 *
 *     Reason:
 *     a) Service and sub-service handlers shares a common signature. It may not be possible to use
 *     constants since the functional use of these pointer parameters is different in different
 *     service and sub-service handlers
 *     b) The pointer can be modified in a routine that is called from the function
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.13 (advisory)
  *     "A pointer should point to a const-qualifed type whenever possible."
  *
  *     Reason:
  *     a) Service and sub-service handlers share a common signature. It may not be possible to use
  *     constants since the functional use of these pointer parameters is different in various
  *     service and sub-service handlers.
  *     b) The pointer can be modified in a routine that is called from the function.
  *
  *  MISRAC2012-2) Deviated Rule: 11.5 (advisory)
  *     "A conversion should not be performed from pointer to void into pointer to object."
  *
  *     Reason:
  *     It is a pointer to void casted to a specific pointer necessary for a specific DID operation.
  *     In this way we have a single context type for all DID operations instead of different
  *     context types for each operation.
  *
  *  MISRAC2012-3) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *  MISRAC2012-4) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  *
  */
 /* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * Condition depends on preprocessor settings that is configuration specific. under certain configurations
 * the conditions can be either always true or always false. This does not pose any functional risk
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_DidServices.h>
#include <Dcm_Dsp_VinServices.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>
#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */

/* The functions provided and implemented by this unit are enabled only if internal service
 * handlers which need DID services are configured. */
#if (DCM_DSP_USE_DIDSERVICES == STD_ON)

#include <TSMem.h>                                               /* EB specific memory functions */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif

#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )
#include <Dcm_Dsp_PidServices.h>
#endif
#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
#include <Dcm_Dsp_MidServices.h>
#include <Dcm_Dsp_DemFacade.h>                                  /* Declaration of DemFacade APIs */
#endif
#endif /* DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON */
#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))
#include <Dcm_Dsp_EndiannessInterpretation.h>
#endif
/*===========================[macro definitions]=================================================*/

#if (defined DCM_DID_OBD_READ_LOW_LIMIT)
#error DCM_DID_OBD_READ_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_READ_LOW_LIMIT) */
/** \brief OBD Did lower limit */
#define DCM_DID_OBD_READ_LOW_LIMIT (0xF400U)

#if (defined DCM_DID_OBD_READ_UP_LIMIT)
#error DCM_DID_OBD_READ_UP_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_READ_UP_LIMIT) */
/** \brief OBD Did upper limit */
#define DCM_DID_OBD_READ_UP_LIMIT (0xF8FFU)

#if (defined DCM_DID_STD_RW_LOW_LIMIT)
#error DCM_DID_STD_RW_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_STD_RW_LOW_LIMIT) */
/** \brief Standard Read/Write Did lower limit */
#define DCM_DID_STD_RW_LOW_LIMIT (0x0000U)

#if (defined DCM_DID_STD_RW_UP_LIMIT)
#error DCM_DID_STD_RW_UP_LIMIT already defined
#endif /* if (defined DCM_DID_STD_RW_UP_LIMIT) */
/** \brief Standard Read/Write Did upper limit */
#define DCM_DID_STD_RW_UP_LIMIT (0xF1FFU)

#if (defined DCM_DID_FF00)
#error DCM_DID_FF00 already defined
#endif /* if (defined DCM_DID_FF00) */
/** \brief Standard Read/Write Did */
#define DCM_DID_FF00 (0xFF00U)

#if (defined DCM_DID_STD_READ_LOW_LIMIT)
#error DCM_DID_STD_READ_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_STD_READ_LOW_LIMIT) */
/** \brief Standard read only Did lower limit */
#define DCM_DID_STD_READ_LOW_LIMIT (0xF900U)

#if (defined DCM_PDID_STD_READ_LOW_LIMIT)
#error DCM_PDID_STD_READ_LOW_LIMIT already defined
#endif /* if (defined DCM_PDID_STD_READ_LOW_LIMIT) */
/** \brief Standard read only periodic Did lower limit */
#define DCM_PDID_STD_READ_LOW_LIMIT (0xF200U)

#if (defined DCM_DID_STD_READ_UP_LIMIT)
#error DCM_DID_STD_READ_UP_LIMIT already defined
#endif /* if (defined DCM_DID_STD_READ_UP_LIMIT) */
/** \brief Standard read only Did upper limit */
#define DCM_DID_STD_READ_UP_LIMIT (0xFEFFU)

#if (defined DCM_PDID_STD_READ_UP_LIMIT)
#error DCM_PDID_STD_READ_UP_LIMIT already defined
#endif /* if (defined DCM_PDID_STD_READ_UP_LIMIT) */
/** \brief Standard read only Did upper limit */
#define DCM_PDID_STD_READ_UP_LIMIT (0xF2FFU)

#if (defined DCM_DID_OBD_PID_LOW_LIMIT)
#error DCM_DID_OBD_PID_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_PID_LOW_LIMIT) */
/** \brief OBD Pid lower limit */
#define DCM_DID_OBD_PID_LOW_LIMIT (0xF400U)

#if (defined DCM_DID_OBD_PID_UP_LIMIT)
#error DCM_DID_OBD_PID_UP_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_PID_UP_LIMIT) */
/** \brief OBD Pid upper limit */
#define DCM_DID_OBD_PID_UP_LIMIT (0xF4FFU)

#if (defined DCM_DID_OBD_MID_LOW_LIMIT)
#error DCM_DID_OBD_MID_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_MID_LOW_LIMIT) */
/** \brief OBD Mid lower limit */
#define DCM_DID_OBD_MID_LOW_LIMIT (0xF600U)

#if (defined DCM_DID_OBD_MID_UP_LIMIT)
#error DCM_DID_OBD_MID_UP_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_MID_UP_LIMIT) */
/** \brief OBD Mid upper limit */
#define DCM_DID_OBD_MID_UP_LIMIT (0xF6FFU)

#if (defined DCM_DID_OBD_INFO_LOW_LIMIT)
#error DCM_DID_OBD_INFO_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_INFO_LOW_LIMIT) */
/** \brief OBD InfoType lower limit */
#define DCM_DID_OBD_INFO_LOW_LIMIT (0xF800U)

#if (defined DCM_DID_OBD_INFO_UP_LIMIT)
#error DCM_DID_OBD_INFO_UP_LIMIT already defined
#endif /* if (defined DCM_DID_OBD_INFO_UP_LIMIT) */
/** \brief OBD InfoType upper limit */
#define DCM_DID_OBD_INFO_UP_LIMIT (0xF8FFU)

#if (defined DCM_DID_SUP_LOW_LIMIT)
#error DCM_DID_SUP_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_SUP_LOW_LIMIT) */
/** \brief System Supplier Specific Did lower limit */
#define DCM_DID_SUP_LOW_LIMIT (0xFD00U)

#if (defined DCM_DID_SUP_UP_LIMIT)
#error DCM_DID_SUP_UP_LIMIT already defined
#endif /* if (defined DCM_DID_SUP_UP_LIMIT) */
/** \brief System Supplier Specific Did upper limit */
#define DCM_DID_SUP_UP_LIMIT (0xFEFFU)

/* !LINKSTO Dcm.Dsn.DidServices.SM.GetDidInfo,1 */
/* Processing step identifiers for function: GetDidInfo  */
#if (defined DCM_DIDSERVICES_GETDIDINFO_SELECTTYPE)
  #error "DCM_DIDSERVICES_GETDIDINFO_SELECTTYPE is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_SELECTTYPE     0U

#if (defined DCM_DIDSERVICES_GETDIDINFO_CHECKSESSION)
  #error "DCM_DIDSERVICES_GETDIDINFO_CHECKSESSION is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_CHECKSESSION   1U

#if (defined DCM_DIDSERVICES_GETDIDINFO_CHECKLENGTH)
  #error "DCM_DIDSERVICES_GETDIDINFO_CHECKLENGTH is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_CHECKLENGTH    2U

#if (defined DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY)
  #error "DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY  3U

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
#if (defined DCM_DIDSERVICES_GETDIDINFO_CHECKTYPERANGE)
  #error "DCM_DIDSERVICES_GETDIDINFO_CHECKTYPERANGE is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_CHECKTYPERANGE 4U

#if (defined DCM_DIDSERVICES_GETDIDINFO_RANGECHECKLEN)
  #error "DCM_DIDSERVICES_GETDIDINFO_RANGECHECKLEN is already defined"
#endif
#define DCM_DIDSERVICES_GETDIDINFO_RANGECHECKLEN  5U
#endif
/* Mandatory access macros for context of function: IsDidAvailable */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_GETDIDINFO_CONTEXT_PTR)
  #error "DCM_DSP_GETDIDINFO_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_DSP_GETDIDINFO_CONTEXT_PTR(context)  \
  (((Dcm_DidInfoContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_GETDIDINFO_CONTEXT)
  #error "DCM_DSP_GETDIDINFO_CONTEXT is already defined"
#endif
#define DCM_DSP_GETDIDINFO_CONTEXT(context)      \
  (DCM_DSP_GETDIDINFO_CONTEXT_PTR(context)->DidInfoContext)

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadDid,3 */
/* Processing step identifiers for function: ReadDid  */
#if (defined DCM_DIDSERVICES_READDID_GETDIDINFO)
  #error "DCM_DIDSERVICES_READDID_GETDIDINFO is already defined"
#endif
#define DCM_DIDSERVICES_READDID_GETDIDINFO    0U

#if (defined DCM_DIDSERVICES_READDID_STATICSELECT)
  #error "DCM_DIDSERVICES_READDID_STATICSELECT is already defined"
#endif
#define DCM_DIDSERVICES_READDID_STATICSELECT  1U

#if (defined DCM_DIDSERVICES_READDID_STATICREADDID)
  #error "DCM_DIDSERVICES_READDID_STATICREADDID is already defined"
#endif
#define DCM_DIDSERVICES_READDID_STATICREADDID 2U

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
#if (defined DCM_DIDSERVICES_READDID_DYNSTOREDIDID)
  #error "DCM_DIDSERVICES_READDID_DYNSTOREDIDID is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNSTOREDIDID 3U

#if (defined DCM_DIDSERVICES_READDID_DYNSELECTSRC)
  #error "DCM_DIDSERVICES_READDID_DYNSELECTSRC is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNSELECTSRC  4U

#if (defined DCM_DIDSERVICES_READDID_DYNGETDIDINFO)
  #error "DCM_DIDSERVICES_READDID_DYNGETDIDINFO is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNGETDIDINFO 5U

#if (defined DCM_DIDSERVICES_READDID_DYNREADDID)
  #error "DCM_DIDSERVICES_READDID_DYNREADDID is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNREADDID    6U

#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
#if (defined DCM_DIDSERVICES_READDID_DYNSELECTMEM)
  #error "DCM_DIDSERVICES_READDID_DYNSELECTMEM is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNSELECTMEM  7U

#if (defined DCM_DIDSERVICES_READDID_DYNREADMEM)
  #error "DCM_DIDSERVICES_READDID_DYNREADMEM is already defined"
#endif
#define DCM_DIDSERVICES_READDID_DYNREADMEM    8U
#endif /* DCM_0X2C_0X02_SSVC_ENABLED == STD_ON */
#endif /* DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
#if (defined DCM_DIDSERVICES_READDID_DEF_READ)
  #error "DCM_DIDSERVICES_READDID_DEF_READ is already defined"
#endif /* (defined DCM_DIDSERVICES_READDID_DEF_READ) */
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
#define DCM_DIDSERVICES_READDID_DEF_READ    9U
#else
#define DCM_DIDSERVICES_READDID_DEF_READ    7U
#endif /* DCM_0X2C_0X02_SSVC_ENABLED == STD_ON */
#else
#define DCM_DIDSERVICES_READDID_DEF_READ    3U
#endif /* DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT */
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */

/* Mandatory access macros for context of function: ReadDid */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_READDID_CONTEXT_PTR)
  #error "DCM_DSP_READDID_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_DSP_READDID_CONTEXT_PTR(context) \
  (((Dcm_ReadDidContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_READDID_CONTEXT)
  #error "DCM_DSP_READDID_CONTEXT is already defined"
#endif
#define DCM_DSP_READDID_CONTEXT(context) \
  (DCM_DSP_READDID_CONTEXT_PTR(context)->ReadDidContext)

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadStaticDid,1 */
/* Processing step identifiers for subfunction: ReadStaticDid  */
#if (defined DCM_DIDSERVICES_READSTATICDID_PRIMARYSELECT)
  #error "DCM_DIDSERVICES_READSTATICDID_PRIMARYSELECT is already defined"
#endif
#define DCM_DIDSERVICES_READSTATICDID_PRIMARYSELECT 0U

#if (defined DCM_DIDSERVICES_READSTATICDID_PRIMARYREAD)
  #error "DCM_DIDSERVICES_READSTATICDID_PRIMARYREAD is already defined"
#endif
#define DCM_DIDSERVICES_READSTATICDID_PRIMARYREAD   1U

#if (DCM_DID_REF_COUNT > 0)
#if (defined DCM_DIDSERVICES_READSTATICDID_REFSELECT)
  #error "DCM_DIDSERVICES_READSTATICDID_REFSELECT is already defined"
#endif
#define DCM_DIDSERVICES_READSTATICDID_REFSELECT     2U

#if (defined DCM_DIDSERVICES_READSTATICDID_REFGETDIDINFO)
  #error "DCM_DIDSERVICES_READSTATICDID_REFGETDIDINFO is already defined"
#endif
#define DCM_DIDSERVICES_READSTATICDID_REFGETDIDINFO 3U

#if (defined DCM_DIDSERVICES_READSTATICDID_REFREAD)
  #error "DCM_DIDSERVICES_READSTATICDID_REFREAD is already defined"
#endif
#define DCM_DIDSERVICES_READSTATICDID_REFREAD       4U
#endif

/* Mandatory access macros for context of subfunction: ReadStaticDid */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_READSTATICDID_CONTEXT_PTR)
  #error "DCM_DSP_READSTATICDID_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_DSP_READSTATICDID_CONTEXT_PTR(context) \
  (((Dcm_ReadStaticDidContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_READSTATICDID_CONTEXT)
  #error "DCM_DSP_READSTATICDID_CONTEXT is already defined"
#endif
#define DCM_DSP_READSTATICDID_CONTEXT(context) \
  (DCM_DSP_READSTATICDID_CONTEXT_PTR(context)->ReadStaticDidContext)

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadSingleStaticDid,2 */
/* Processing step identifiers for subfunction: ReadSingleStaticDid  */
#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_STOREDIDID)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_STOREDIDID is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_STOREDIDID  0U

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_SELECT_DATA)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_SELECT_DATA is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_SELECT_DATA 1U

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_LOCAL_DATA)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_LOCAL_DATA is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_LOCAL_DATA  2U

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET is already defined"
#endif

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA is already defined"
#endif
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
#define DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA  3U
#else
#define DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA  2U
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */

/* With DID ranges enabled, OBD Did state ids start after DID range state ids */
#define DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET \
  (DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA + 1U)
#else
/* With DID ranges disabled, OBD Did state ids start after common state ids */
#define DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET \
  (DCM_DIDSERVICES_READSINGLESTATICDID_LOCAL_DATA + 1U)
#endif

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_PID_SELECT)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_PID_SELECT is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_PID_SELECT \
  DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_PID_READ)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_PID_READ is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_PID_READ \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 1U)

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_MID_SELECT)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_MID_SELECT is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_MID_SELECT \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 2U)

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_MID_READ)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_MID_READ is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_MID_READ \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 3U)

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_INFO_SELECT)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_INFO_SELECT is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_INFO_SELECT \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 4U)

#if (defined DCM_DIDSERVICES_READSINGLESTATICDID_INFO_READ)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_INFO_READ is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_INFO_READ \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 5U)

  #if (defined DCM_DIDSERVICES_READSINGLESTATICDID_OBDINFOSELECT_TRYLOCK)
  #error "DCM_DIDSERVICES_READSINGLESTATICDID_OBDINFOSELECT_TRYLOCK is already defined"
#endif
#define DCM_DIDSERVICES_READSINGLESTATICDID_OBDINFOSELECT_TRYLOCK \
  (DCM_DIDSERVICES_READSINGLESTATICDID_OBD_OFFSET + 6U)

    /* Mandatory access macros for context of subfunction: ReadSingleStaticDid */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_READSINGLESTATICDID_CONTEXT_PTR)
  #error "DCM_DSP_READSINGLESTATICDID_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_DSP_READSINGLESTATICDID_CONTEXT_PTR(context) \
  (((Dcm_ReadSingleStaticDidContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_READSINGLESTATICDID_CONTEXT)
  #error "DCM_DSP_READSINGLESTATICDID_CONTEXT is already defined"
#endif
#define DCM_DSP_READSINGLESTATICDID_CONTEXT(context) \
  (DCM_DSP_READSINGLESTATICDID_CONTEXT_PTR(context)->ReadSingleStaticDidContext)

/* !LINKSTO Dcm.Dsn.DidServices.SM.WriteDid,1 */
/* Processing step identifiers for function: WriteDid  */
#if (defined DCM_DIDSERVICES_WRITEDID_GETDIDINFO)
  #error "DCM_DIDSERVICES_WRITEDID_GETDIDINFO is already defined"
#endif
#define DCM_DIDSERVICES_WRITEDID_GETDIDINFO    0U

#if (defined DCM_DIDSERVICES_WRITEDID_SIGSELECT)
  #error "DCM_DIDSERVICES_WRITEDID_SIGSELECT is already defined"
#endif
#define DCM_DIDSERVICES_WRITEDID_SIGSELECT     1U

#if (defined DCM_DIDSERVICES_WRITEDID_SIGWRITE)
  #error "DCM_DIDSERVICES_WRITEDID_SIGWRITE is already defined"
#endif
#define DCM_DIDSERVICES_WRITEDID_SIGWRITE      2U

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
#if (defined DCM_DIDSERVICES_WRITEDID_RANGEWRITE)
  #error "DCM_DIDSERVICES_WRITEDID_RANGEWRITE is already defined"
#endif
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
  #define DCM_DIDSERVICES_WRITEDID_RANGEWRITE    3U
#else
  #define DCM_DIDSERVICES_WRITEDID_RANGEWRITE    1U
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
#endif /* #if (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
#if (defined DCM_DIDSERVICES_WRITEDID_DEFAULT)
  #error "DCM_DIDSERVICES_WRITEDID_DEFAULT is already defined"
#endif /* (defined DCM_DIDSERVICES_WRITEDID_DEFAULT) */
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
#define DCM_DIDSERVICES_WRITEDID_DEFAULT    4U
#else
#define DCM_DIDSERVICES_WRITEDID_DEFAULT    3U
#endif /* #if (DCM_DSP_USE_DIDRANGES == STD_ON) */
#else
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
#define DCM_DIDSERVICES_WRITEDID_DEFAULT    2U
#else
#define DCM_DIDSERVICES_WRITEDID_DEFAULT    1U
#endif /* #if (DCM_DSP_USE_DIDRANGES == STD_ON) */
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */

/* Mandatory access macros for context of function: ReadDid */
/** \brief Dynamically casts the base context to the specialized context */
#if (defined DCM_DSP_WRITEDID_CONTEXT_PTR)
  #error "DCM_DSP_WRITEDID_CONTEXT_PTR is already defined"
#endif
/* Deviation MISRAC2012-2 */
#define DCM_DSP_WRITEDID_CONTEXT_PTR(context) \
  (((Dcm_WriteDidContextType*)(context)->SpecialPtr))

/** \brief Accesses the specialized part of the context */
#if (defined DCM_DSP_WRITEDID_CONTEXT)
  #error "DCM_DSP_WRITEDID_CONTEXT is already defined"
#endif
#define DCM_DSP_WRITEDID_CONTEXT(context) \
  (DCM_DSP_WRITEDID_CONTEXT_PTR(context)->WriteDidContext)

/*===========================[type definitions]==================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.DidServices.SM.GetDidInfo,1 */
/* Processing step functions prototypes for function: GetDidInfo  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_SelectType(Dcm_Dsp_ContextPtr context);
#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckSession(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_DidCheckLen(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckSecurity(
  Dcm_Dsp_ContextPtr context);
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckType_Range(
  Dcm_Dsp_ContextPtr context);
STATIC FUNC(void,DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_RangeCheckLen(Dcm_Dsp_ContextPtr context);
#endif
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON)) */

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadDid,3 */
/* Processing step functions prototypes for function: ReadDid  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_GetDidInfo(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_StaticSelect(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_StaticRead(Dcm_Dsp_ContextPtr context);
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynStoreDidId(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynSelectSrc(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynGetDidInfo(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynReadDid(Dcm_Dsp_ContextPtr context);
#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynSelectMem(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynReadMem(Dcm_Dsp_ContextPtr context);
#endif /* #if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */
#endif
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DefaultRead(Dcm_Dsp_ContextPtr context);
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadStaticDid,1 */
/* Processing step functions prototypes for subfunction: ReadDidStaticDid  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_PrimarySelect(
  Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_PrimaryRead(
  Dcm_Dsp_ContextPtr context);
#if (DCM_DID_REF_COUNT > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefSelect(
  Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefGetDidInfo(
  Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefRead(
  Dcm_Dsp_ContextPtr context);
#endif

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadSingleStaticDid,2 */
/* Processing step functions prototypes for subfunction: ReadDidSingleStaticDid  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_StoreDidId(
  Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_SelectDidDataSrc(
  Dcm_Dsp_ContextPtr context);
#if (DCM_DID_SIGNALS_COUNT > 0U)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadLocalData(
  Dcm_Dsp_ContextPtr context);
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadRangeData(
  Dcm_Dsp_ContextPtr context);
#endif
#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0) )
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidSelect(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidRead(Dcm_Dsp_ContextPtr context);
#endif
#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidSelect(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidRead(Dcm_Dsp_ContextPtr context);
#endif
#if ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0))
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoRead(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE)
Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect_TryLock (Dcm_Dsp_ContextPtr context);
#endif /* ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0)) */
#endif /* DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON */

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)
/* !LINKSTO Dcm.Dsn.DidServices.SM.WriteDid,1 */
/* Processing step functions prototypes for function: WriteDid  */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_GetDidInfo(Dcm_Dsp_ContextPtr context);
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_SigSelect(Dcm_Dsp_ContextPtr context);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_SigWrite(Dcm_Dsp_ContextPtr context);
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_RangeWrite(Dcm_Dsp_ContextPtr context);
#endif
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_DefaultWrite(Dcm_Dsp_ContextPtr context);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#endif /* (DCM_DSP_USE_SERVICE_0X2E == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.DidServices.SM.GetDidInfo,1 */
/** \brief Processing step defintions for function: GetDidInfo */
/* Deviation MISRAC2012-4 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_GetDidInfoStateConfig[] =
{
  /* SELECT TYPE    */ &Dcm_Dsp_DidServices_GetDidInfo_SelectType,
#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
  /* CHECK SESSION  */ &Dcm_Dsp_DidServices_GetDidInfo_CheckSession,
  /* DID CHECK LEN  */ &Dcm_Dsp_DidServices_GetDidInfo_DidCheckLen,
  /* CHECK SECURITY */ &Dcm_Dsp_DidServices_GetDidInfo_CheckSecurity,
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
  /* CHECK RANGE     */ &Dcm_Dsp_DidServices_GetDidInfo_CheckType_Range,
  /* RANGE CHECK LEN */ &Dcm_Dsp_DidServices_GetDidInfo_RangeCheckLen,
#endif
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON)) */
};

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadDid,3 */
/** \brief Processing step defintions for function: ReadDid */
/* Deviation MISRAC2012-4 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_ReadDidStateConfig[] =
{
  /* GET DIDINFO     */ &Dcm_Dsp_DidServices_ReadDid_GetDidInfo,
  /* STATIC SELECT   */ &Dcm_Dsp_DidServices_ReadDid_StaticSelect,
  /* STATIC READ DID */ &Dcm_Dsp_DidServices_ReadDid_StaticRead,
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  /* DYN STORE DIDID */ &Dcm_Dsp_DidServices_ReadDid_DynStoreDidId,
  /* DYN SELECT SRC  */ &Dcm_Dsp_DidServices_ReadDid_DynSelectSrc,
  /* DYN GET DIDINFO */ &Dcm_Dsp_DidServices_ReadDid_DynGetDidInfo,
  /* DYN READ   DID  */ &Dcm_Dsp_DidServices_ReadDid_DynReadDid,
#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
  /* DYN SELECT MEM  */ &Dcm_Dsp_DidServices_ReadDid_DynSelectMem,
  /* DYN READ   MEM  */ &Dcm_Dsp_DidServices_ReadDid_DynReadMem,
#endif /* #if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */
#endif
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  /* DEFAULT READ    */ &Dcm_Dsp_DidServices_ReadDid_DefaultRead,
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */
};

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadStaticDid,1 */
/** \brief Processing step defintions for subfunction: ReadStaticDid */
/* Deviation MISRAC2012-4 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_ReadStaticDidStateConfig[] =
{
  /* STATICSELECT  */ &Dcm_Dsp_DidServices_ReadStaticDid_PrimarySelect,
  /* STATICREAD    */ &Dcm_Dsp_DidServices_ReadStaticDid_PrimaryRead,
#if (DCM_DID_REF_COUNT > 0)
  /* REFSELECT     */ &Dcm_Dsp_DidServices_ReadStaticDid_RefSelect,
  /* REFGETDIDINFO */ &Dcm_Dsp_DidServices_ReadStaticDid_RefGetDidInfo,
  /* REFREAD       */ &Dcm_Dsp_DidServices_ReadStaticDid_RefRead,
#endif
};

/* !LINKSTO Dcm.Dsn.DidServices.SM.ReadSingleStaticDid,2 */
/** \brief Processing step defintions for subfunction: ReadSingleStaticDid */
/* Deviation MISRAC2012-4 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_ReadSingleStaticDidStateConfig[] =
{
  /* STOREDIDID  */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_StoreDidId,
  /* SELECT DATA */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_SelectDidDataSrc,
#if (DCM_DID_SIGNALS_COUNT > 0U)
  /* LOCAL DATA  */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadLocalData,
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
  /* RANGE DATA  */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadRangeData,
#endif
#if (DCM_DSP_USE_OBDSERVICES == STD_ON)
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0) )
  /* OBD PID SELECT  */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidSelect,
  /* OBD PID READ    */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidRead,
#else
  /* OBD PID SELECT  */ NULL_PTR,
  /* OBD PID READ    */ NULL_PTR,
#endif
#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
  /* OBD MID SELECT  */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidSelect,
  /* OBD MID READ    */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidRead,
#else
  /* OBD MID SELECT  */ NULL_PTR,
  /* OBD MID READ    */ NULL_PTR,
#endif
#if ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0))
  /* OBD INFO SELECT */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect,
  /* OBD INFO READ   */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoRead,
  /* OBD INFO TRY LOCK */ &Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect_TryLock,
#else
  /* OBD INFO SELECT */ NULL_PTR,
  /* OBD INFO READ   */ NULL_PTR,
  /* OBD INFO TRY LOCK */ NULL_PTR,
#endif /* ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0)) */
#endif
};

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)
/* !LINKSTO Dcm.Dsn.DidServices.SM.WriteDid,1 */
/** \brief Processing step defintions for subfunction: WriteDid */
/* Deviation MISRAC2012-4 */
STATIC CONST(Dcm_Dsp_ContextFunctionType, DCM_CONST) Dcm_WriteDidStateConfig[] =
{
  /* GET DIDINFO    */ &Dcm_Dsp_DidServices_WriteDid_GetDidInfo,
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
  /* SIGSELECT      */ &Dcm_Dsp_DidServices_WriteDid_SigSelect,
  /* SIGWRITE       */ &Dcm_Dsp_DidServices_WriteDid_SigWrite,
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
  /* WRITE RANGE     */ &Dcm_Dsp_DidServices_WriteDid_RangeWrite,
#endif
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  /* DEFAULT WRITE   */ &Dcm_Dsp_DidServices_WriteDid_DefaultWrite,
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
};
#endif /* (DCM_DSP_USE_SERVICE_0X2E == STD_ON) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Determines whether a DID is in the correct range for an operation
 **
 ** \param[in] reqDid Requestd Did
 ** \param[in] DidOp The operation requested is either Reading (in the context of service
 **            ReadDataByIdentifier or ReadDataByPeriodicIdentifier), writing in the
 **            context of service WriteDataByIdentifier, controlling in the context of
 **            service InputOutputControlByIdentifier or defining a dynamic DID in the
 **            context of service DynamicallyDefineDataIdentifier.
 **
 ** \retval TRUE                The DID is available
 ** \retval FALSE               The DID is not available
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInValidRange(
  uint16        reqDid,
  Dcm_DidOpType DidOp);

/** \brief Determines whether a DID is in the range of OBD DIDs
 **
 ** \param[in] Did Requested Did
 **
 ** \retval TRUE                The DID is in the range of OBD DIDs
 ** \retval FALSE               The DID is not in this range
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInObdRange(uint16 Did);

#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
/** \brief Determines if an OBD Did is available and returns its type (PID/MID/InfoType)
 **        If the OBD Did is not available, the type is reported as not available
 **
 ** \param[in] Did Requested Did
 ** \param[out] ConfigType Type of OBD Did (PID/MID/InfoType or not available)
 **
 ** \return void
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_IsObdDIDAvailable(
  uint16 Did,
  P2VAR(Dcm_DidType, AUTOMATIC, DCM_VAR) ConfigType);
#endif

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
/** \brief Determines whether a DID is in the range of dynamically defined DIDs
 **
 ** \param[in] Did Requestd Did
 **
 ** \retval TRUE                The DID is in the range of dynamically defined DIDs
 ** \retval FALSE               The DID is not in this range
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInDynamicRange(uint16 Did);

/** \brief Determines whether a dynamically defined DID is configured
 **
 ** \pre This function must only be called for Dids which can be dynamically defined.
 **
 ** \param[in] DidConfigIdx  Index of the DidConfig entry of the requested Did
 **
 ** \retval TRUE                The DID has configuration items
 ** \retval FALSE               The DID has not been configured yet
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidDynamicallyConfigured(
  Dcm_DidConfigIdxType DidConfigIdx);
#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */

#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
/** \brief Determines whether a DID can be read in a certain session
 **
 ** \param[in] AccessInfo Pointer to an AccessInfoBase structure
 **
 ** \retval TRUE                The DID is available
 ** \retval FALSE               The DID is not available
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidAvailableInSession(
  Dcm_DidAccessInfoBaseTypePtr AccessInfo);

/** \brief Determines whether a DID can be read in a security level
 **
 ** \param[in] AccessInfo Pointer to an AccessInfoBase structure
 **
 ** \retval TRUE                The DID is available
 ** \retval FALSE               The DID is not available
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidAvailableInSecLevel(
  Dcm_DidAccessInfoBaseTypePtr AccessInfo);
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON)) */

/** \brief Internal function to set the status of Did signals.  This function is used for legacy
 **        support and called together with Dcm_Dsp_DidServices_ReadSingleStaticDid_Init
 **
 ** \param[in] DidInfoPtr Pointer to the DidInfo of the Did to be read
 ** \param[in] pReadSignalContext Pointer to the signal reading context to be initialized
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DidSetStatus(
  P2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext);

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
/** \brief Searches a DidRange configuration if it provides the requested Did.
 **
 ** \param[in]  Did  Requested Did.
 ** \param[out] DidRangeIdx Index of the range providing the Did.
 **
 ** \retval TRUE   A matching DidRange has been found.
 ** \retval FALSE  No matching DidRange has been found.
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_GetDidRangeIndex(
  uint16 Did,
  P2VAR(Dcm_DidConfigIdxType, AUTOMATIC, DCM_VAR) DidRangeIdx);
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

STATIC FUNC (void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Init(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr,
  P2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  boolean WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr);

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Execute(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr);

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Cancel(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr);

STATIC FUNC (void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Init(
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr,
  P2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  boolean WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr);

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Execute(
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr);

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Cancel(
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr);

#if (DCM_DID_REF_COUNT > 0U)
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsp_DidServices_GetNumRefDids(uint16 Did);
#endif

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_InitInternal(boolean NvMRestoreFailed);
#endif

/** \brief Transfer a Did into a target buffer, with length check
 **
 ** \param BufferCfgPtr Pointer to the confi structure of the target buffer
 ** \param Did Did value to be stored
 **
 ** \return boolean
 ** \retval TRUE  Did has been stored and buffer config updated
 ** \retval FALSE Did has not been stored, buffer too small
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_StoreDidId(
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr,
  uint16 Did);

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && \
     (DCM_DID_ENDIANNESS_CONVERSION == STD_ON) && \
     (DCM_DID_SIGNALS_COUNT > 0U))
/** \brief Calculates a DID signal data position if Endianness Conversion is enabled for
 ** the service ReadDataByIdentifier: 0x22 and IOControlByIdentifier: 0x2F
 **
 ** \param[in] DataType     Data type of the current signal
 ** \param[in] DataLength   Length of the current signal in bits
 ** \param[in] DataPosition Position of the current signal in bits
 **
 */
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_DidServices_EndianessConversion_CalculateDataPosition (
  Dcm_DidDataByteType DataType,
  uint16              DataLength,
  uint16              DataPosition);
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && \
           (DCM_DID_ENDIANNESS_CONVERSION == STD_ON) && \
           (DCM_DID_SIGNALS_COUNT > 0U)) */

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Calculate the read scaling info size for a specific DID
 **  and check if the size fits into the transmission buffer
 **
 ** \param[in] pReadSignalContext Pointer to the signal reading context
 **
 ** \retval TRUE                The read scaling info size fits into the buffer
 ** \retval FALSE               The read scaling info size doesn't fit into the buffer
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_ScalingInfoSizeFitsIntoBuffer(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext);
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=======================[external data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
VAR(uint8, DCM_VAR)  Dcm_PeriodicRequest[2];
#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
/** \brief Context for defining dynamic Dids (as received by service 0x2C) */
VAR(Dcm_DDDIDContextType, DCM_VAR) Dcm_DDDIDContext;
#endif /* DCM_0X2C_0X01_SSVC_ENABLED == STD_ON */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
#if (DCM_DDDID_DO_NOT_INIT == STD_OFF)

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* If configuration item DcmDDDIDStorage is set to 'true' the Dcm saves this
 * table to the NvM block indicated by configruation item DcmDspDDDIDBlockIdRef */
VAR(Dcm_DDDidSrcTblEntryType, DCM_VAR) Dcm_DDDidSrcTable[DCM_NUM_DDDID_SRCTAB_ELEMENTS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#else

#define DCM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* If configuration item DcmDDDIDStorage is set to 'true' the Dcm saves this
 * table to the NvM block indicated by configruation item DcmDspDDDIDBlockIdRef */
VAR(Dcm_DDDidSrcTblEntryType, DCM_VAR) Dcm_DDDidSrcTable[DCM_NUM_DDDID_SRCTAB_ELEMENTS];

#define DCM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
#endif /* #if (DCM_DDDID_DO_NOT_INIT == STD_OFF) */
#endif /* #if (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */


/*=======================[internal data declarations]============================================*/


#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
/** \brief Indicate the status of DynamicallyDefinedDID initialization */
STATIC VAR(boolean, DCM_VAR) Dcm_DynamicDidInitialized = FALSE;
#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
STATIC VAR(Dcm_MutexType, DCM_VAR) Dcm_Dsp_DidServices_ServiceBusy = DCM_MUTEX_UNLOCKED;
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Temporary buffer to read data for a dynamically defined Did into */
/* Deviation MISRAC2012-4 */
STATIC VAR(uint8, DCM_VAR) DynamicDidBuffer[DCM_DID_MAX_SIZE];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ((((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON)) && \
      (DCM_DID_SIGNALS_COUNT > 0) ) || (DCM_DSP_USE_DIDRANGES == STD_ON))
  /** \brief Buffer that Dcm uses whenever reading and writing DID signals */
  /* Deviation MISRAC2012-4 */
  STATIC VAR(Dcm_Union_BufferType, DCM_VAR) Dcm_DIDGeneralBuffer;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.DidServices.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_Init(void)
{
  DBG_DCM_DSP_DIDSERVICES_INIT_ENTRY();

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  Dcm_Dsp_DidServices_ServiceBusy = DCM_MUTEX_UNLOCKED;
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_DIDSERVICES_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.DidServices.IB.GetDidIndex,2 */
FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_GetDidIndex(
  uint16 Did,
  P2VAR(Dcm_DidConfigIdxType, AUTOMATIC, DCM_VAR) ConfigIdx)
{
  boolean DIDFound = FALSE;

#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
  Dcm_DidConfigIdxType LowDidIdx = 0U;
  Dcm_DidConfigIdxType HighDidIdx = DCM_DSP_DIDSERVICES_NUM_DIDS - 1U;
#else
  TS_PARAM_UNUSED(Did);
  TS_PARAM_UNUSED(ConfigIdx);
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */

  DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_ENTRY(Did, ConfigIdx);

#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.DID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.PID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnusedDid.DID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnusedDid.PID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnusedDid.OBDMID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnusedDid.InfoType,1 */
  /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnusedDid.DID,1 */
  /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnconfiguredDid.DID,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnusedDid.ResetToDefault,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnusedDid.ReturnControlToEcu,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnusedDid.FreezeCurrentState,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnusedDid.ShortTermAdjustment,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnconfiguredDid.ResetToDefault,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnconfiguredDid.ReturnControlToEcu,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnconfiguredDid.FreezeCurrentState,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnconfiguredDid.ShortTermAdjustment,1 */
  /* Check if the search DID is between lowest and biggest DID */
  if((Did >= Dcm_DidConfig[LowDidIdx].Did) && (Did <= Dcm_DidConfig[HighDidIdx].Did))
  {
    while ((HighDidIdx >= LowDidIdx) && (DIDFound != TRUE))
    {
      /* divide the set */
      const Dcm_DidConfigIdxType MidDidIdx = LowDidIdx + ((HighDidIdx - LowDidIdx) >> 1U);

      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.InfoType,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.OBDMID,1 */
      if (Did < Dcm_DidConfig[MidDidIdx].Did)
      {
        /* continue searching in the lower half of the set */
        HighDidIdx = MidDidIdx - 1U;
      }
      else if (Did > Dcm_DidConfig[MidDidIdx].Did)
      {
        /* continue searching in the upper half of the set */
        LowDidIdx = MidDidIdx + 1U;
      }
      else
      {
        /* the did has been found in the list of statically defined dids */
        DIDFound = TRUE;
        /* store the index of the Did in the ConfigIdx */
        *ConfigIdx = MidDidIdx;
      }
    }
  }
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
  DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_EXIT(result, Did, ConfigIdx);
  return DIDFound;
}

/* !LINKSTO Dcm.Dsn.DidServices.IB.GetMaximumDidLength,1 */
FUNC(uint32, DCM_CODE) Dcm_Dsp_DidServices_GetMaximumDidLength(
  P2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo)
{
  /* Size of DID */
  uint32 DidDataLength = 0U;

  DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_ENTRY(DidInfo);

  if((DidInfo->ConfigType == DCM_DID_TYPE_OBD_PID) ||
     (DidInfo->ConfigType == DCM_DID_TYPE_OBD_MID ) ||
     (DidInfo->ConfigType == DCM_DID_TYPE_OBD_INFO))
  {
    /* Set DID data length equal with maximum buffer size, to skip the length check
     * in case of OBD DIDs */
    DidDataLength = DCM_MAX_BUFFER_SIZE;
  }
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
  else if ((DCM_DID_TYPE_RANGE_NO_GAPS == DidInfo->ConfigType) ||
      (DCM_DID_TYPE_RANGE_GAPS    == DidInfo->ConfigType))
  {
    /* Data length for Range (DcmDspDidRangeMaxDataLength) is stored in bytes */
    DidDataLength = Dcm_DidRangeConfig[DidInfo->ConfigIdx].MaxDataLength;
  }
  else
#endif
  {
#if (DCM_DID_SIGNALS_COUNT > 0U)
    /* Total number of signals configured for the DID */
    const uint16 numSignals = Dcm_DidConfig[DidInfo->ConfigIdx].NumSignals;
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

#if (DCM_DID_REF_COUNT > 0U)
    /* Total number of referenced DIDs by this DID */
    const uint16 numReferencedDids = Dcm_DidConfig[DidInfo->ConfigIdx].NumRefDids;
#endif /* #if (DCM_DID_REF_COUNT > 0U) */

    /* shall not be called for unconfigured DIDs */
    DCM_PRECONDITION_ASSERT(DCM_DID_UNCONFIGURED != DidInfo->ConfigType, DCM_INTERNAL_API_ID);

#if (DCM_DID_SIGNALS_COUNT > 0U)
    if (numSignals > 0)
    {
      /* Pointer to the last signal in the signal table for the DID */
      P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
        Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[DidInfo->ConfigIdx].DidSignalsIdx + (numSignals - 1U));

      Dcm_DidDataType CurrentDataCfg;

      /* Get DcmDspData configuration */
      Dcm_Dsp_DidSignals_GetDidDataConfig(&CurrentDataCfg, SignalCfg->DspDataIndex);

      /* Calculate the total number of data bytes needed for all the signals configured in the
      * DID. The signals corresponding to a DID are stored in the order of starting position
      * of signal data in response buffer. So the total number of data bytes needed for all
      * the signals in a DID can be calculated by adding the position and size of the last
      * signal configured for the DID.
      */
      /* Data length and position for signals are stored in bits */
      DidDataLength =
       (uint32)DCM_GET_BYTES((uint32)SignalCfg->DataPos + (uint32)(&CurrentDataCfg)->DataLength);
    }
    else
    {
      /* do nothing */
    }
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

#if (DCM_DID_REF_COUNT > 0U)
    /* Size of referenced DID */
    if (0U != numReferencedDids)
    {
      uint16_least referencedDidIdx;

      for (referencedDidIdx = 0U; referencedDidIdx < numReferencedDids; referencedDidIdx++)
      {
        const uint16 refDidID =
          Dcm_DidRefers[Dcm_DidConfig[DidInfo->ConfigIdx].DidRefersIdx + referencedDidIdx];
        Dcm_DidConfigIdxType refDidIndex = 0U;
        uint16 refNumSignals;

        /* The referenced Did is checked and hard coded in the configuration, so it is
         * always present */
        (void)Dcm_Dsp_DidServices_GetDidIndex(refDidID, &refDidIndex);

        /* Pointer to the last signal in the signal table for the referenced DID */
        refNumSignals = Dcm_DidConfig[refDidIndex].NumSignals;
        if (refNumSignals > 0)
        {
          P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
              Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[refDidIndex].DidSignalsIdx + (refNumSignals - 1U));
          Dcm_DidDataType CurrentDataCfg;

          /* Get DcmDspData configuration */
          Dcm_Dsp_DidSignals_GetDidDataConfig(&CurrentDataCfg, SignalCfg->DspDataIndex);

          /* Calculate the total number of data bytes needed for all the signals configured in the
           * referenced DIDs.*/
          DidDataLength += (uint32)DCM_GET_BYTES(
            (uint32)SignalCfg->DataPos + (uint32)(&CurrentDataCfg)->DataLength) + DCM_DID_ID_SIZE;
        }
      }
    }
#endif /* #if (DCM_DID_REF_COUNT > 0U) */
  }
  DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_EXIT(DidDataLength, DidInfo);
  return DidDataLength;
}

/*
 * DidServices Function: GetDidInfo
 *
 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Init(
  Dcm_DidInfoContextTypePtr DidInfoContext,
  uint16                    Did,
  Dcm_DidOpType             DidOp,
  uint8                     ProtocolIdx)
{
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &DidInfoContext->DidInfoContext;

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_ENTRY(DidInfoContext, Did, DidOp, ProtocolIdx);

  /* intialize base context */
  DCM_DSP_CONTEXT_INIT(DidInfoContext, Dcm_GetDidInfoStateConfig);

  /* initialize DidInfo */
  DidInfo->Did         = Did;
  DidInfo->DidOp       = DidOp;
  DidInfo->ProtocolIdx = ProtocolIdx;

  /* initialize Did to 'UNCONFIGURED' */
  DidInfo->ConfigType = DCM_DID_UNCONFIGURED;

  /* initialize Nrc to 'REQUESTOUTOFRANGE' matching unconfigured Did */
  DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(DidInfoContext)) = DCM_E_REQUESTOUTOFRANGE;

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_EXIT(DidInfoContext, Did, DidOp, ProtocolIdx);
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Execute(
  Dcm_DidInfoContextTypePtr DidInfoContext )
{
  Std_ReturnType stateOfCtxExe = E_NOT_OK;

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_ENTRY(DidInfoContext);

  stateOfCtxExe = Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(DidInfoContext));

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_EXIT(stateOfCtxExe,DidInfoContext);

  return stateOfCtxExe;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Cancel(
  Dcm_DidInfoContextTypePtr DidInfoContext )
{
  Std_ReturnType stateOfCtxCancel = E_NOT_OK;

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_ENTRY(DidInfoContext);

  stateOfCtxCancel = Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(DidInfoContext));

  DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_EXIT(stateOfCtxCancel,DidInfoContext);

  return stateOfCtxCancel;
}

FUNC_P2VAR(Dcm_Union_BufferType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetDidGeneralBuffer(void)
{
  P2VAR(Dcm_Union_BufferType, AUTOMATIC, DCM_VAR) pDIDGeneralBuffer = NULL_PTR;

#if ((((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON)) && \
      (DCM_DID_SIGNALS_COUNT > 0) ) || (DCM_DSP_USE_DIDRANGES == STD_ON))
  pDIDGeneralBuffer = &Dcm_DIDGeneralBuffer;
#endif

  DBG_DCM_DSP_DIDSERVICES_GETDIDGENERALBUFFER_EXIT(pDIDGeneralBuffer);

  return pDIDGeneralBuffer;

}

/*
 * DidServices Function: ReadDid
 *
 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Init(
  Dcm_ReadDidContextTypePtr ReadDidContext,
  uint16 Did,
  Dcm_DidOpType DidOp,
  boolean WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferConfig,
  boolean EnableRuleChecks,
  PduIdType RxPduId)
{
  /* HSM instance id, present regardless of DCM_HSM_MULTI_ENABLED */
  const uint8 ProtocolIdx = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduId]
    .MainConnectionIndex].ProtocolIndex;

  /* parameter is always TRUE */
  TS_PARAM_UNUSED(EnableRuleChecks);

  DBG_DCM_DSP_DIDSERVICES_READDID_INIT_ENTRY(ReadDidContext, Did, DidOp, WriteDidIds,
    DidBufferConfig, EnableRuleChecks, ProtocolIdx);

  /* intialize base context */
  DCM_DSP_CONTEXT_INIT(ReadDidContext, Dcm_ReadDidStateConfig);

  /* initialize context for function */
  /* - output buffer */
  ReadDidContext->ReadDidContext.DidBufferCfgPtr = DidBufferConfig;
  /* - Flag if Did ids shall be added to output */
  ReadDidContext->ReadDidContext.WriteDidIds = WriteDidIds;

  /* initialize context for subfunction: DidInfo */
  Dcm_Dsp_DidServices_GetDidInfo_Init(
    &ReadDidContext->ReadDidContext.DidInfoContext, Did, DidOp, ProtocolIdx);

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  ReadDidContext->ReadDidContext.DynDidIdx = 0U;
#endif
  DBG_DCM_DSP_DIDSERVICES_READDID_INIT_EXIT(ReadDidContext, Did, DidOp, WriteDidIds,
    DidBufferConfig, EnableRuleChecks, ProtocolIdx);
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Execute(
  Dcm_ReadDidContextTypePtr ReadDidContext)
{
  Std_ReturnType statusOfReadDid;
  /* process the function */
  DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_ENTRY(ReadDidContext);
  statusOfReadDid = Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(ReadDidContext));
  DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_EXIT(statusOfReadDid, ReadDidContext);
  return statusOfReadDid;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Cancel(
  Dcm_ReadDidContextTypePtr ReadDidContext)
{
  Std_ReturnType stsOfReadDidCancel;

  DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_ENTRY(ReadDidContext);
  /* cancel the function */
  stsOfReadDidCancel = Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(ReadDidContext));
  DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_EXIT(stsOfReadDidCancel,ReadDidContext);
  return stsOfReadDidCancel;
}

#if (DCM_DID_SIGNALS_COUNT > 0U)
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext
  )
{
  /* Total number of signals configured for the DID*/
  uint16 numSignals;
  /* The value DCM_E_OK of the local variable nrc shall not be interpreted as a positive response.
   * Only if some error occurs the negative response code will be stored there. Otherwise it is
   * not valid. */
  Dcm_NegativeResponseCodeType nrc = DCM_E_OK;
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Std_ReturnType SignalReadRetVal = DCM_E_NOT_OK;
  boolean breakConditionsNotFulfilled = TRUE;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  boolean scalingInfoSizeConditionFulfilled = TRUE;
#endif

  DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_ENTRY(pReadSignalContext);

  if (pReadSignalContext->CurrentOpStatus == DCM_CANCEL)
  {
    Dcm_Dsp_DidSignals_Read_Cancel(&(pReadSignalContext->SignalReadContext));
    RetVal = DCM_E_DONE;
  }
  else
  {
    /* Total number of signals configured for the DID */
    numSignals = Dcm_DidConfig[pReadSignalContext->CurrentDidCfgIndex].NumSignals;

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
    if((pReadSignalContext->DidOp == DCM_DID_OP_READSCALING) && (pReadSignalContext->NewSignal == TRUE))
    {
      scalingInfoSizeConditionFulfilled = Dcm_Dsp_DidServices_ScalingInfoSizeFitsIntoBuffer(pReadSignalContext);
    }

    if(scalingInfoSizeConditionFulfilled == TRUE)
#endif
    {
      /* If there are any signals in this DID to process, begin processing, otherwise
         return OK */
      if (numSignals != 0U)
      {
        /* Loops over the signals in a DID while negative response code is not set and
         * return value is not pending */
        while ((pReadSignalContext->SignalInd < numSignals) && breakConditionsNotFulfilled )
        {
          if (pReadSignalContext->NewSignal == TRUE)
          {
            /* holds the configuration table of signals in the DID */
            CONSTP2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
              Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[pReadSignalContext->CurrentDidCfgIndex].DidSignalsIdx);
            /* index of the current signal in signal configuration table */
            const uint16 DidDataIndex = SignalCfg[pReadSignalContext->SignalInd].DspDataIndex;
            /* position of the current signal in bits */
            uint16 didDataPosn;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
            if(pReadSignalContext->DidOp == DCM_DID_OP_READSCALING)
            {
              if(pReadSignalContext->SignalInd == 0)
              {
                didDataPosn = 0;
              }
              else
              {
                /* pReadSignalContext->SignalReadContext.BufferOffset is updated after a signal is
                 * successfully read. It will contain the length of the last signals.*/
                didDataPosn = (uint16)(DCM_GET_BITS(pReadSignalContext->SignalReadContext.BufferOffset));
              }
            }
            else
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
            {
              didDataPosn = SignalCfg[pReadSignalContext->SignalInd].DataPos;

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
              if ((pReadSignalContext->SignalInd >= 1U) &&
                (TRUE == pReadSignalContext->EndiannessConversion))
              {
                Dcm_DidDataType LocalDataCfg;

                uint16_least PreviousSignalIndex;

                for (PreviousSignalIndex = 0;
                   PreviousSignalIndex < pReadSignalContext->SignalInd;
                   PreviousSignalIndex++)
                {
                  uint16 DataIndexPrev = SignalCfg[PreviousSignalIndex].DspDataIndex;

                  /* Get DcmDspData configuration */
                  Dcm_Dsp_DidSignals_GetDidDataConfig(&LocalDataCfg, DataIndexPrev);

                  /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalLength.DataTypeLimit,1 */
                  /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalLength.NoDataTypeLimit,1 */
                  didDataPosn = Dcm_Dsp_DidServices_EndianessConversion_CalculateDataPosition(
                  LocalDataCfg.DataType,
                  LocalDataCfg.DataLength,
                  didDataPosn);
                }
              }
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */
            }
            /* Setup read operation for current signal */
            Dcm_Dsp_DidSignals_Read_Init(
                                      &(pReadSignalContext->SignalReadContext), /* context */
                                      pReadSignalContext->outputBuffer,
                                      pReadSignalContext->BufferLimit,
                                      DidDataIndex, /* DidDataIndex */
                                      didDataPosn, /* SignalOffset */
                                      pReadSignalContext->AmountOfBufferOccupied, /* BufferOffset */
                                      pReadSignalContext->DidOp,
                                      pReadSignalContext->EndiannessConversion);
            pReadSignalContext->NewSignal = FALSE;
          }

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
          /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.NoScalingInfo.TransmitNRC0x31,1 */
          if((pReadSignalContext->DidOp == DCM_DID_OP_READSCALING) &&
             (pReadSignalContext->SignalReadContext.ReadScalingSignalLength == 0U))
          {
            nrc = DCM_E_REQUESTOUTOFRANGE;
          }
          else
#endif
          {
            SignalReadRetVal = Dcm_Dsp_DidSignals_Read_Execute(&(pReadSignalContext->SignalReadContext));

            nrc = pReadSignalContext->SignalReadContext.Nrc;
          }

          if (DCM_E_OK == SignalReadRetVal)
          {
            /* Simply advance to the next signal */
            pReadSignalContext->SignalInd++;
            pReadSignalContext->NewSignal = TRUE;

            if (pReadSignalContext->SignalInd == numSignals)
            {
              /* If this was the final signal, Response buffer is now filled with DID(2bytes)
                 and Signals(BufferOffset)*/
              pReadSignalContext->AmountOfBufferOccupied +=
                pReadSignalContext->SignalReadContext.BufferOffset;
              pReadSignalContext->NrcConditions = DCM_E_OK;
              RetVal = DCM_E_DONE;
              breakConditionsNotFulfilled = FALSE;
            }
          }
          else if (DCM_E_NOT_OK == SignalReadRetVal)
          {
            /* Some error occurred, set the nrc and exit the loop */
            pReadSignalContext->NrcConditions = nrc;
            RetVal = DCM_E_NOT_OK;
            breakConditionsNotFulfilled = FALSE;
          }
          else
          {
            DCM_PRECONDITION_ASSERT((DCM_E_PENDING == SignalReadRetVal), DCM_INTERNAL_API_ID);
            /* The operation needs to be called again. Exit the loop */
            RetVal = DCM_E_PENDING;
            breakConditionsNotFulfilled = FALSE;
          }
        }
      }
      else
      {
        /* This DID contains no signals of its own, therefore just return. */
        RetVal = DCM_E_DONE;
      }
    }
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
    else
    {
      /* Scaling size info doesn't fit into the transmission buffer */
      /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.TxBufferShort.TransmitNRC0x22,1 */
      pReadSignalContext->NrcConditions = DCM_E_CONDITIONSNOTCORRECT;
      RetVal = DCM_E_NOT_OK;
    }
#endif /* #if (DCM_DSP_USE_SERVICE_0x24 == STD_ON) */
  }

  DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_EXIT(RetVal, pReadSignalContext);
  return RetVal;
}
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/


FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSignal_GetResults(
  P2CONST(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR)  pReadSignalContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint32, AUTOMATIC, DCM_VAR)                       AmountOfBufferOccupied )
{
  Std_ReturnType operationResult;

  DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_ENTRY(pReadSignalContext,
    Nrc, AmountOfBufferOccupied);

  if (DCM_E_OK == pReadSignalContext->NrcConditions)
  {
    operationResult = DCM_E_DONE;
  }
  else
  {
    operationResult = DCM_E_NOT_OK;
  }

  *AmountOfBufferOccupied = pReadSignalContext->AmountOfBufferOccupied;
  *Nrc = pReadSignalContext->NrcConditions;

  DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_EXIT(operationResult, pReadSignalContext,
    Nrc, AmountOfBufferOccupied);
  return operationResult;
}

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)

/*
 * DidServices Function: WriteDid
 *
 */

FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Init(
  Dcm_WriteDidContextTypePtr       WriteDidContext,
  uint16                           Did,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataPtr,
  uint32                           DataLen,
  uint8                            ProtocolIdx)
{
#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
  /* get configuration for current HSM instance */
  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig =
    &DCM_PROTOCOL_CONFIG(ProtocolIdx);
  const boolean EndiannessConversion = curProtocolConfig->EndiannessConversion;
#else
  const boolean EndiannessConversion = FALSE;
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

  /* intialize base context */
  DCM_DSP_CONTEXT_INIT(WriteDidContext, Dcm_WriteDidStateConfig);

  DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_ENTRY(WriteDidContext, Did, DataPtr, DataLen, ProtocolIdx);

  /* initialize context for function */
  /* - data buffer */
  WriteDidContext->WriteDidContext.BufferCfg.BufferPtr = DataPtr;
  WriteDidContext->WriteDidContext.BufferCfg.Occupied  = DataLen;

  WriteDidContext->WriteDidContext.EndiannessConversion = EndiannessConversion;

  /* - legacy function data */
  /* initialize the signal to be processed next */
  WriteDidContext->WriteDidContext.SignalInd = 0U;
  /* Legth of the request is needed to validate the request message in case of
   * WriteDataByIdentifier */
  WriteDidContext->WriteDidContext.DidInfoContext.DidInfoContext.ReqLen = DataLen;

  /* intialize context for subfunction: DidInfo */
  Dcm_Dsp_DidServices_GetDidInfo_Init(
    &WriteDidContext->WriteDidContext.DidInfoContext, Did, DCM_DID_OP_WRITE, ProtocolIdx);

  DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_EXIT(WriteDidContext, Did, DataPtr, DataLen, ProtocolIdx);
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Execute(
  Dcm_WriteDidContextTypePtr WriteDidContext)
{
  Std_ReturnType statusOfWriteDIDExecute = E_NOT_OK;

  DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_ENTRY(WriteDidContext);
  /* process the function */
  statusOfWriteDIDExecute = Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(WriteDidContext));
  DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_EXIT(statusOfWriteDIDExecute, WriteDidContext);

  return statusOfWriteDIDExecute;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Cancel(
  Dcm_WriteDidContextTypePtr WriteDidContext)
{
  Std_ReturnType statusOfWriteDidCancel = E_NOT_OK;

  DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_ENTRY(WriteDidContext);
  /* process the function */
  statusOfWriteDidCancel = Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(WriteDidContext));
  DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_EXIT(statusOfWriteDidCancel, WriteDidContext);

  return statusOfWriteDidCancel;
}

/* !LINKSTO Dcm.Dsn.DidServices.IB.WriteDid_GetDidInfo,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_GetDidInfo(Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) DidInfoContextPtr =
    &DCM_DSP_WRITEDID_CONTEXT(context).DidInfoContext;

  Std_ReturnType result;

  /* Check if the Did can be read in the current coditions as per the configuration
   * and get the neccessary Did info for reading (not passed by caller yet) */

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_GetDidInfo_Cancel(DidInfoContextPtr);
  }
  else
  {
    /* execute subroutine: get the Did info */
    result = Dcm_Dsp_DidServices_GetDidInfo_Execute(DidInfoContextPtr);
  }

  if (DCM_E_OK == result)
  {
    /* Did can be read, select the next step */
    /* !LINKSTO Dcm.Dsn.DidServices.SM.WriteDid,1 */
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
    const Dcm_ConfigType ConfigType =
      DidInfoContextPtr->DidInfoContext.ConfigType;

    if ((DCM_DID_TYPE_RANGE_NO_GAPS == ConfigType) ||
        (DCM_DID_TYPE_RANGE_GAPS    == ConfigType))
    {
      /* next step: write a DidRange Did */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_WRITEDID_RANGEWRITE;
    }
    else
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */
    {
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
      /* if the Did is available, the config type cannot be DCM_DID_UNCONFIGURED */
      DCM_PRECONDITION_ASSERT(
        (DCM_DID_TYPE_ID_STATIC == DidInfoContextPtr->DidInfoContext.ConfigType),
        DCM_INTERNAL_API_ID);

      /* next step: write a single Did */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_WRITEDID_SIGSELECT;
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
    }
  }
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  /* Check if the Did is unconfigured and default interface is active. */
  else if((DCM_E_NOT_OK == result) && (DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE == (DidInfoContextPtr->DidInfoContext.ConfigType)))
  {
    /* Next step: Default write for an unconfigured Did. */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_WRITEDID_DEFAULT;
  }
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
  else
  {
    /* inherit the result and error code of the subroutine */
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(DidInfoContextPtr));
  }
}

#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_SigSelect(Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_WriteDidContextExtType, AUTOMATIC, DCM_VAR) WriteDidContext =
    &DCM_DSP_WRITEDID_CONTEXT(context);

  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &WriteDidContext->DidInfoContext.DidInfoContext;

  /* holds the number of signals in the DID */
  const uint16 NumSignals = Dcm_DidConfig[DidInfo->ConfigIdx].NumSignals;

  if (WriteDidContext->SignalInd < NumSignals)
  {

    /* holds the configuration table of signals in the DID */
    CONSTP2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
      Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[DidInfo->ConfigIdx].DidSignalsIdx);
    /* bit-position of signal data in request buffer */
    const uint16 DataPos = SignalCfg[WriteDidContext->SignalInd].DataPos;
    /* index of the current signal in signal configuration table */
    const uint16 DspDataIndex = SignalCfg[WriteDidContext->SignalInd].DspDataIndex;

    Std_ReturnType InitRetVal = Dcm_Dsp_DidSignals_Write_Init(
      &(WriteDidContext->SignalWriteContext),
      WriteDidContext->BufferCfg.BufferPtr,
      WriteDidContext->BufferCfg.Occupied,
      DspDataIndex,
      DataPos,
      WriteDidContext->EndiannessConversion);

    if (InitRetVal != DCM_E_OK)
    {
      DCM_DSP_CONTEXT_NRC(context)    = WriteDidContext->SignalWriteContext.Nrc;
      DCM_DSP_CONTEXT_RESULT(context) = InitRetVal;
    }
    else
    {
      /* if the initialization succeded, switch to next state: execute writing the signal */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_WRITEDID_SIGWRITE;
    }
  }
  else
  {
    /* Dids without signals should not be requested (filtered by SvcH) */
    DCM_PRECONDITION_ASSERT(NumSignals > 0U, DCM_INTERNAL_API_ID);

    /* processing finished */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_SigWrite(Dcm_Dsp_ContextPtr context)
{
  Std_ReturnType result;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_WriteDidContextExtType, AUTOMATIC, DCM_VAR) WriteDidContext =
    &DCM_DSP_WRITEDID_CONTEXT(context);


  if (DCM_DSP_CONTEXT_OPSTATUS(context) == DCM_CANCEL)
  {
    Dcm_Dsp_DidSignals_Write_Cancel(&(WriteDidContext->SignalWriteContext));
    /* stop writing signals */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_DONE;
  }
  else
  {
    result = Dcm_Dsp_DidSignals_Write_Execute(&(WriteDidContext->SignalWriteContext));
    if (DCM_E_OK == result)
    {
      /* currently processed signal was written successfully, increment the signal index */
      WriteDidContext->SignalInd++;
      /* select the next signal to be written */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_WRITEDID_SIGSELECT;
    }
    else
    {
      DCM_DSP_CONTEXT_NRC(context) = WriteDidContext->SignalWriteContext.Nrc;
      /* signal processing ongoing, nothing to do */
    }

    DCM_DSP_CONTEXT_RESULT(context) = result;
  }
}

#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
/* !LINKSTO Dcm.Dsn.DidServices.IB.WriteDid_RangeWrite,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_RangeWrite(Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    &DCM_DSP_WRITEDID_CONTEXT(context).DidInfoContext.DidInfoContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    &DCM_DSP_WRITEDID_CONTEXT(context).BufferCfg;

  Dcm_NegativeResponseCodeType Nrc;
  Std_ReturnType result;

  /* The RangeCheck already verified that the occupied buffer is < uint16 */
  DCM_PRECONDITION_ASSERT(BufferCfgPtr->Occupied < DCM_UINT16_MAX, DCM_INTERNAL_API_ID);

  /* call the configured data write function */
  result = Dcm_DidRangeConfig[DidInfoPtr->ConfigIdx]
              .WriteFnc(DidInfoPtr->Did,
                        BufferCfgPtr->BufferPtr,
                        DCM_DSP_CONTEXT_OPSTATUS(context),
                        (uint16)BufferCfgPtr->Occupied,
                        &Nrc);

  /* inherit the result of the subfunction */
  DCM_DSP_CONTEXT_RESULT(context) = result;

  if (DCM_E_NOT_OK == result)
  {
    /* inherit the Nrc of the subfunction */
    DCM_DSP_CONTEXT_NRC(context) = Nrc;
  }
  else if ((DCM_E_OK == result) || (DCM_E_PENDING == result))
  {
    /* Do nothing, result has already been copied into context */
  }
  else
  {
    /* unexpected interface return value */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO Dcm.DET.InterfaceReturnValue, 2 */
    DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDIDDATA, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
  }
}
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_DefaultWrite(Dcm_Dsp_ContextPtr context)
{
  /* Result of the user's Default Write function. */
  Std_ReturnType Result = DCM_E_NOT_OK;
  /* Negative Response Code of the user's Default Write function. */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    &DCM_DSP_WRITEDID_CONTEXT(context).BufferCfg;

  /* Call the user's Default Write custom implementation. */
  /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.Dcm_DefaultDIDWrite,1 */
  /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.Timeout.Cancellation,1 */
  /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.ProtocolPreemption.Cancellation,1 */
  Result = Dcm_DefaultDIDWrite(
            DCM_DSP_CONTEXT_OPSTATUS(context),
            (Dcm_DIDIDType)(DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext.Did),
            (BufferCfgPtr->BufferPtr),
            (BufferCfgPtr->Occupied),
            &Nrc);

  if (DCM_E_OK == Result)
  {
    /* Processing finished. */
    /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.OK,1 */
    DCM_DSP_CONTEXT_RESULT(context) = Result;
    /* Inherit the result and error code of the Dcm_DefaultDIDWrite() function. */
    DCM_DSP_CONTEXT_NRC(context) = Nrc;
  }
  else if(DCM_E_NOT_OK == Result)
  {
    if(DCM_E_POSITIVERESPONSE == Nrc)
    {
      /* Return generalReject as a NegativeResponseCode. */
      /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.0x00,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
      /* Processing finished. */
      DCM_DSP_CONTEXT_RESULT(context) = Result;
    }
    else
    {
      /* Processing finished. */
      /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.NRC,1 */
      DCM_DSP_CONTEXT_RESULT(context) = Result;
      /* Inherit the result and error code of the Dcm_DefaultDIDWrite() function. */
      DCM_DSP_CONTEXT_NRC(context) = Nrc;
    }
  }
  else if(DCM_E_PENDING == Result)
  {
    /* Result remains DCM_E_PENDING. */
    /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.Pending,1 */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
    /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.DET,1 */
    DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDWRITE_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

    /* Return generalReject as a NegativeResponseCode. */
    /* !LINKSTO Dcm.EB.WriteDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.NRC,1 */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
    /* Processing finished. */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#endif /* #if (DCM_DSP_USE_SERVICE_0X2E == STD_ON) */

/*==================[internal function definitions]==============================================*/

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
/* Dcm.Dsn.IB.DidServices.GetDidRangeIndex */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_GetDidRangeIndex(
  uint16                                                Did,
  P2VAR(Dcm_DidConfigIdxType, AUTOMATIC, DCM_VAR) DidRangeIdx)
{
  boolean result = FALSE;

  Dcm_DidConfigIdxType DidIdx;

  /* Deviation TASKING-1 */
  for (DidIdx = 0U; DidIdx < DCM_DSP_DIDSERVICES_NUM_DID_RANGES; ++DidIdx)
  {
    /* check if the range ends with or behind the did */
    if (Dcm_DidRangeConfig[DidIdx].UpperLimit >= Did)
    {
      /* check if the found range also start before or with the did */
      if (Dcm_DidRangeConfig[DidIdx].LowerLimit <= Did)
      {
        /* range covers this did */
        result = TRUE;
        /* store the index of the DidRange in the ConfigIdx */
        *DidRangeIdx = DidIdx;
      }
      /* else: range starts after the did; no range covers this did, result remains FALSE */

      /* DidRanges do not overlap; there can only be one range for a Did - stop searching */
      break;
    }
  }

  return result;
}
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if (DCM_DID_REF_COUNT > 0U)
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsp_DidServices_GetNumRefDids(uint16 Did)
{
  uint8 result = 0U;

  Dcm_DidConfigIdxType DidConfigIdx = 0U;

  if (TRUE == Dcm_Dsp_DidServices_GetDidIndex(Did, &DidConfigIdx))
  {
    result = Dcm_DidConfig[DidConfigIdx].NumRefDids;
  }
  /* else: non-statically defined Dids do not have references */

  return result;
}
#endif

#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidAvailableInSession (
  Dcm_DidAccessInfoBaseTypePtr AccessInfo)
{
  boolean didIsAvailable = FALSE;
  uint8_least sesLevIdx;

  /* get the current session type */
  Dcm_SesCtrlType curSesCtrl = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

  /* check if access info exists and has entries */
  if ((NULL_PTR != AccessInfo) && (AccessInfo->NumSesCtrls > 0U))
  {
    for (sesLevIdx = 0U; sesLevIdx < AccessInfo->NumSesCtrls; ++sesLevIdx)
    {
      if ( (curSesCtrl == AccessInfo->SesCtrls[sesLevIdx]) ||
           (AccessInfo->SesCtrls[sesLevIdx] == DCM_ALL_SESSION_LEVEL))
      {
        didIsAvailable = TRUE;
        break;
      }
    }
  }
  else
  {
    /* Dcm615_Conf:  If there is no reference, no check of session level shall be done. */
    didIsAvailable = TRUE;
  }

  return didIsAvailable;
}
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))) */

#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DidServices.GetDidInfo_SelectType.ObdRange,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_IsObdDIDAvailable(
  uint16 Did,
  P2VAR(Dcm_DidType, AUTOMATIC, DCM_VAR) ConfigType)
{
#if ( (DCM_NUM_PID_CFG > 0) || \
      (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X09 == STD_ON) )
  const uint8 Range = ((uint8)Did / DCM_OBD_AVAILABILITY_RANGE) * DCM_OBD_AVAILABILITY_RANGE;
#endif

  *ConfigType = DCM_DID_UNCONFIGURED;

  if ((DCM_DID_OBD_PID_LOW_LIMIT <= Did) && (Did <= DCM_DID_OBD_PID_UP_LIMIT))
  {
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_NUM_PID_CFG > 0) )
    const Dcm_DspPidIdentifierType PidId = (Dcm_DspPidIdentifierType)Did;

    if (PidId == 0x00U)
    {
      /* !LINKSTO Dcm.ReadDataById.DcmDspPid.FirstAvailabilityPID, 1 */
      *ConfigType = DCM_DID_TYPE_OBD_PID;
    }
    else
#endif
    {
#if (DCM_NUM_PID_CFG > 0)
      uint8_least Index = 0U;

      while ((Index<DCM_NUM_PID_CFG) && (*ConfigType == DCM_DID_UNCONFIGURED))
      {
        /* Check if the PID is available and enabled */
        if (DCM_UINT8_IS_BIT_SET(Dcm_DspPidUsed[Index/8U], Index%8U)
            &&
            (
              (((Dcm_DspPidBitFieldType)(Dcm_DspPidConfig[Index].BitField & DCM_SERVICE_01))!=0U)
             ||
              (((Dcm_DspPidBitFieldType)(Dcm_DspPidConfig[Index].BitField & DCM_SERVICE_02))!=0U)
            )
        )
        {
          if ((Dcm_DspPidConfig[Index].PidIdentifier == PidId) ||
              (((PidId % DCM_OBD_AVAILABILITY_RANGE) == 0x00U) &&
               (Dcm_DspPidConfig[Index].PidIdentifier > Range)))
          {
            *ConfigType = DCM_DID_TYPE_OBD_PID;
          }
        }

        Index++;
      }
#endif
    }
  }

#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
#if (DCM_OBDMID_SUPPORT_SWC == STD_ON)
  if ((DCM_DID_OBD_MID_LOW_LIMIT <= Did) && (Did <= DCM_DID_OBD_MID_UP_LIMIT))
  {
    const uint8 Obdmid = (uint8)Did;
    uint8 iteratorOdbMid = 0U;

    while ((iteratorOdbMid < DCM_DSP_NR_OBDMID) && (*ConfigType == DCM_DID_UNCONFIGURED))
    {
      /* Check if the MID is available and enabled */
      if (DCM_IS_BYTE_FLAG_SET(iteratorOdbMid, Dcm_DspOBDMIDUsed) == TRUE)
      {
        if ((Dcm_DspOBDMIDConfig[iteratorOdbMid].OBDMIDIdentifier == Obdmid) ||
            (((Obdmid % DCM_OBD_AVAILABILITY_RANGE) == 0x00U) &&
            (Dcm_DspOBDMIDConfig[iteratorOdbMid].OBDMIDIdentifier > Range)))
        {
          *ConfigType = DCM_DID_TYPE_OBD_MID;
        }
      }

      iteratorOdbMid++;
    }
  }
#else
  /* Extract the DID low byte. */
  if ((DCM_DID_OBD_MID_LOW_LIMIT <= Did) && (Did <= DCM_DID_OBD_MID_UP_LIMIT))
  {
    /* Extract the DID Low Byte. */
    const uint8 RequestedOBDMID = DCM_LOBYTE(Did);
    /* The ID of the "availability OBDMID" corresponding to Requested OBDMID. */
    uint8 AvailabilityOBDMID = 0xFFU;
    /* Value of the "availability OBDMID". */
    uint32 ObdMidValue = 0xFFFFFFFFU;

    TS_PARAM_UNUSED(Range);

    if(0x00U == RequestedOBDMID)
    {
      /* !LINKSTO Dcm.ISO-15031-5_8.6.1_7,1 */
      /* On-Board Diagnostic Monitor ID 0x00 is required to be configured. */
      *ConfigType = DCM_DID_TYPE_OBD_MID;
    }
    else
    {
      /* Obtain the corresponding "availability OBDMID" for the Requested OBDMID. */
      AvailabilityOBDMID = ((RequestedOBDMID - 1U) / DCM_OBD_AVAILABILITY_RANGE) * DCM_OBD_AVAILABILITY_RANGE;
      /* Get the value of the requested On-Board Diagnostic Monitor ID (OBDMID). */
      (void)Dcm_Dsp_DemFacade_GetAvailableOBDMIDs(AvailabilityOBDMID, &ObdMidValue);

      /* !LINKSTO SAE_J1979-DA_TABLE_A1.06,1 */
      /* Check DATA A/B/C/D to see if requested OBDMID is configured.
       * According to J1979DA_201406.pdf - "Table A1 - Supported Pid/ObdMid/Tid/InfoType Definition". */
      if(DCM_UINT32_IS_BIT_SET(ObdMidValue, (0x20 - (RequestedOBDMID - AvailabilityOBDMID ))))
      {
        /* The requested OBDMID is configured. */
        *ConfigType = DCM_DID_TYPE_OBD_MID;
      }
      else
      {
        /* Do nothing. ConfigType remains DCM_DID_UNCONFIGURED. */
      }
    }
  }
#endif /* (DCM_OBDMID_SUPPORT_SWC == STD_ON) */
#endif /* (DCM_DSP_USE_SERVICE_0X06 == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && \
     (DCM_DSP_VEHINFO_NUM > 0) && \
     (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0))
  if ((DCM_DID_OBD_INFO_LOW_LIMIT <= Did) && (Did <= DCM_DID_OBD_INFO_UP_LIMIT))
  {
    const Dcm_DspVehInfoInfoTypeType VehInfoInfoType = (Dcm_DspVehInfoInfoTypeType)Did;
    uint8_least vehInfoIndex = 0U;

    while ((vehInfoIndex < DCM_DSP_VEHINFO_NUM) && (*ConfigType == DCM_DID_UNCONFIGURED))
    {
      /* Check if the VehicleInfo is available and enabled */
      if (DCM_IS_VEHINFOUSED_IDX_SET(vehInfoIndex))
      {
        if ((Dcm_DspVehInfo[vehInfoIndex].VehInfoInfoType == VehInfoInfoType) ||
            (((VehInfoInfoType % DCM_OBD_AVAILABILITY_RANGE) == 0x00U) &&
             (Dcm_DspVehInfo[vehInfoIndex].VehInfoInfoType > Range)))
        {
          *ConfigType = DCM_DID_TYPE_OBD_INFO;
        }
      }

      vehInfoIndex++;
    }
  }
#endif /* ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && \
           (DCM_DSP_VEHINFO_NUM > 0) && \
           (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0)) */
}
#endif /* #if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON) */

/*
 * Processing steps for function:  GetDidInfo
 */

/* !LINKSTO Dcm.Dsn.DidServices.IB.GetDidInfo_SelectType,2 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_SelectType (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &DCM_DSP_GETDIDINFO_CONTEXT(context);

  /* Check if Did is generally in a valid range for the requested operation */

  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.PID,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.DID,1 */
  if (Dcm_Dsp_DidServices_IsDidInValidRange(DidInfo->Did, DidInfo->DidOp) != FALSE)
  {
    /* Check if and how Did is configured */
    if (Dcm_Dsp_DidServices_GetDidIndex(DidInfo->Did, &DidInfo->ConfigIdx) != FALSE)
    {
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
      /* Did is configured by value */

      /* set the index to the DidInfo structure */
      DidInfo->InfoIdx = Dcm_DidConfig[DidInfo->ConfigIdx].DidInfoIdx;

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
      /* check if the did is a dynamically defined did */
      if (Dcm_Dsp_DidServices_IsDidInDynamicRange(DidInfo->Did))
      {
        /* config access: no existing dynamic onfiguration required */
        if (DidInfo->DidOp == DCM_DID_OP_DYNAMIC)
        {
          /* set Did configuration type to 'dynamic by id' */
          DidInfo->ConfigType = DCM_DID_TYPE_ID_DYNAMIC;
        }
        else if (Dcm_DidInfo[DidInfo->InfoIdx].DDDidMaxNumSrcElements == 0)
        {
          /* set Did configuration type to 'static by id' */
          DidInfo->ConfigType = DCM_DID_TYPE_ID_STATIC;
        }
        /* read access: check if the dynamic configuration is available */
        else if (Dcm_Dsp_DidServices_IsDidDynamicallyConfigured(DidInfo->ConfigIdx))
        {
          /* set Did configuration type to 'dynamic by id' */
          DidInfo->ConfigType = DCM_DID_TYPE_ID_DYNAMIC;
        }
        else
        {
          /* set Did configuration type to 'unconfigured' */
          DidInfo->ConfigType = DCM_DID_UNCONFIGURED;
        }
      }
      else
#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */
      if (Dcm_Dsp_DidServices_IsDidInObdRange(DidInfo->Did))
      {
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReadDIDFromOBDRange,1 */

        /* set Did configuration type to 'static by id' */
        DidInfo->ConfigType = DCM_DID_TYPE_ID_STATIC;
      }
      else
      {
        /* set Did configuration type to 'static by id' */
        DidInfo->ConfigType = DCM_DID_TYPE_ID_STATIC;
      }
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
    }
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
    else if (Dcm_Dsp_DidServices_GetDidRangeIndex(DidInfo->Did, &DidInfo->ConfigIdx) != FALSE)
    { /* Did is configured by range */

      /* set the index to the DidRangeInfo structure */
      DidInfo->InfoIdx = Dcm_DidRangeConfig[DidInfo->ConfigIdx].DidInfoIdx;

      /* Check if the range has gaps */
      if (Dcm_DidRangeConfig[DidInfo->ConfigIdx].HasGaps != FALSE)
      {
        /* set Did configuration type to 'by range with gaps' */
        DidInfo->ConfigType = DCM_DID_TYPE_RANGE_GAPS;
      }
      else
      {
        /* set Did configuration type to 'by range without gaps' */
        DidInfo->ConfigType = DCM_DID_TYPE_RANGE_NO_GAPS;
      }
    }
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */
#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.OBDMID,1 */
    else if ((Dcm_Dsp_DidServices_IsDidInObdRange(DidInfo->Did) != FALSE) && (DCM_DID_OP_READ == DidInfo->DidOp))
    { /* Did is configured by presence of OBD services (implicit range) */
      /* Determine the type of the requested OBD Did (PID/MID/InfoType)
         If the OBD Did is not available, the type remains DCM_DID_UNCONFIGURED */
       Dcm_Dsp_DidServices_GetDidInfo_IsObdDIDAvailable(DidInfo->Did, &DidInfo->ConfigType);
    }
#endif
    else
    {
      /* DidInfo.DidConfigType remains DCM_DID_UNCONFIGURED */
    }
  }

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  if((DidInfo->DidOp == DCM_DID_OP_READSCALING) && (DidInfo->ConfigType!= DCM_DID_TYPE_ID_STATIC))
  {
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.DynamicallyDefinedDID.TransmitNRC0x31,1 */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.RangeDID.TransmitNRC0x31,1 */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.DIDUsageDisabled.TransmitNRC0x31,1 */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_REQUESTOUTOFRANGE;
    /* Did is not supported for read scaling, stop processing */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineById.DynamicallyDefined.NotConfigured,1 */
  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineById.SourceDataIdentifier.NotConfigured,1 */
  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineByMemoryAddress.NotConfigured,1 */
  else
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
  if(DidInfo->ConfigType == DCM_DID_UNCONFIGURED)
  {
#if(DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    /* Did has not been found. Default read option is active. */
    DidInfo->ConfigType = DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE;
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */
    /* Did has not been found, stop processing */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
  else if (DidInfo->ConfigType == DCM_DID_TYPE_RANGE_GAPS)
  {
    /* next state: check the range provider if the Did is available */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKTYPERANGE;
    /* availability check for DidRanges can be asynchronous */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
  }
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */
#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
  else if ( (DidInfo->ConfigType == DCM_DID_TYPE_OBD_PID) ||
            (DidInfo->ConfigType == DCM_DID_TYPE_OBD_MID) ||
            (DidInfo->ConfigType == DCM_DID_TYPE_OBD_INFO) )
  {
    /* Did has been found and is OBD-related, stop processing */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
  }
#endif
  else
  {
    /* 'check conditions' is the next step by default */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKSESSION;
  }
}

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckType_Range (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &DCM_DSP_GETDIDINFO_CONTEXT(context);

  Dcm_DidSupportedType didIsSupported = DCM_DID_NOT_SUPPORTED;

  /* DidRange has gaps -> call the Did provider; result is always E_OK */
  Std_ReturnType result =
    Dcm_DidRangeConfig[DidInfo->ConfigIdx].IsDidAvailableFnc(
      DidInfo->Did,
      DCM_DSP_CONTEXT_OPSTATUS(context),
      &didIsSupported);

  /* SWS_Dcm_00769, SWS_Dcm_00803: this function must not return E_NOT_OK */
  DCM_PRECONDITION_ASSERT(result != DCM_E_NOT_OK, DCM_INTERNAL_API_ID);

  if (DCM_E_OK == result)
  {
    if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
    {
      /* operation cancelled, just return DCM_E_OK */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
    }
    else if (DCM_DID_SUPPORTED == didIsSupported)
    {
      /* set Did configuration type to 'by range with gaps' */
      DidInfo->ConfigType = DCM_DID_TYPE_RANGE_GAPS;
      /* next state: check conditions */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKSESSION;
    }
    else
    {
      /* set Did configuration type to 'unconfigured' */
      DidInfo->ConfigType = DCM_DID_UNCONFIGURED;
      /* DidInfo.DidConfigType remains DCM_DID_UNCONFIGURED */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  } /* else: remain in PENDING state */
}
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckSession (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &DCM_DSP_GETDIDINFO_CONTEXT(context);

  /* get the configured access info according to the requested did operation */
  Dcm_DidAccessInfoBaseTypePtr AccessInfo =
    Dcm_Dsp_DidServices_GetAccessInfo(DidInfo->InfoIdx, DidInfo->DidOp);

  /* this state can only be reached for configured Dids */
  DCM_PRECONDITION_ASSERT(DidInfo->ConfigType != DCM_DID_UNCONFIGURED, DCM_INTERNAL_API_ID);

  /* set default return value to 'NOT OK' */
  DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;

  if (NULL_PTR != AccessInfo)
  {
    /* Check for Session access, whether the DID can be read in the current session level */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.DIDNotSupportedInActiveSession.TransmitNRC0x31,1 */
    if (Dcm_Dsp_DidServices_IsDidAvailableInSession(AccessInfo))
    {
      if (DidInfo->DidOp == DCM_DID_OP_WRITE)
      {
#if (DCM_DSP_USE_DIDRANGES == STD_ON)
        if (  (DCM_DID_TYPE_RANGE_NO_GAPS == DidInfo->ConfigType)
            ||(DCM_DID_TYPE_RANGE_GAPS    == DidInfo->ConfigType)
           )
        {
          /* next step: write a DidRange Did */
          DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_RANGECHECKLEN;
        }
        else
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */
        {
          DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKLENGTH;
        }
      }
      else
      {
        DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY;
      }
    }
    /* else: result remains DCM_E_NOT_OK */
  }
  else
  {
    /* !LINKSTO Dcm433,1, Dcm468,1, Dcm565,1 */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.RequestOutOfRange.DcmDspDidReadScalingDataDisabled.TransmitNRC0x31,1 */
    /* access is disabled for this Did, set the NRC */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_REQUESTOUTOFRANGE;
    /* result remains DCM_E_NOT_OK */
  }

  /* processing done */
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_DidCheckLen(Dcm_Dsp_ContextPtr context)
{

#if (DCM_DID_SIGNALS_COUNT > 0U)
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo
    = &DCM_DSP_GETDIDINFO_CONTEXT(context);

  /* Result of the check if the requested data length matches the signals length, considering both fixed and variable length signals */
  Std_ReturnType DidLengthCheckResult = DCM_E_NOT_OK;
  /* Index for browsing through DidSignals configuration */
  uint16_least SignalsIndex;
  /* Get the number of signals belonging to the requested Did */
  const uint16 NumSignals = Dcm_DidConfig[DidInfo->ConfigIdx].NumSignals;

  if (NumSignals > 0U)
  {
    /* End position for the current signal */
    uint16 CurrentSignalEndingPosition;
    /* Maximum end position between all signals of the Did, including a possible variable length signal (for which its max configured value is considered) */
    uint16 MaxSignalEndingPosition = 0U;
    /* Maximum end position between all signals of the Did, without considering a variable length signal (size 0 is considered for it) */
    uint16 MaxSignalEndingPositionWithoutVarLen = 0U;
    Dcm_DidDataType dataCfg;

    /* Calculate the total number of data bytes needed for all the signals configured in the DID
     * depending on the start position and endianness
     */
    for (SignalsIndex=0U;SignalsIndex<NumSignals;SignalsIndex++)
    {
      P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) CurrentSignalCfg =
        Dcm_Dsp_DidSignals_GetDidSignalConfig((uint16)(Dcm_DidConfig[DidInfo->ConfigIdx].DidSignalsIdx + SignalsIndex));

      /* Get DcmDspData configuration */
      Dcm_Dsp_DidSignals_GetDidDataConfig(&dataCfg, CurrentSignalCfg->DspDataIndex);

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
      if ((dataCfg.AccessInterface == DCM_USE_DATA_SENDER_RECEIVER) &&
          (dataCfg.DataEndianness == DCM_BIG_ENDIAN))
      {
        /* For big endian signals, the data position byte is the last byte the signal will occupy */
        CurrentSignalEndingPosition = (uint16)DCM_GET_BYTES(CurrentSignalCfg->DataPos + 1U);

        /* If the signal is the one that stretches maximum to the buffer end, update the end position */
        if (CurrentSignalEndingPosition > MaxSignalEndingPosition)
        {
          MaxSignalEndingPosition = CurrentSignalEndingPosition;
        }
      }
      else
#endif
      {
        if (dataCfg.FixedLength == FALSE)
        {
          /* Considering that only the last Did signal can have variable length, if the current signal has variable length
           * store the biggest end position up to now, without considering the current one
           * This is the smallest data length that can be requested */
          MaxSignalEndingPositionWithoutVarLen = MaxSignalEndingPosition;
        }

        /* Check if there is another signal that stretches more to the buffer end, including variable length signals */
        CurrentSignalEndingPosition =
          (uint16)DCM_GET_BYTES(CurrentSignalCfg->DataPos + dataCfg.DataLength);

        if (CurrentSignalEndingPosition > MaxSignalEndingPosition)
        {
          MaxSignalEndingPosition = CurrentSignalEndingPosition;
        }
      }
    }

    /* dataCfg now contains the configuration of the last signal of the did
     * Considering that only the last Did signal can have variable length, check if the did has variable length signals
     * and check the request length
     */
    /* If the did contains only fixed length signals, the total length configured for the DID should match
     * the length of the request data (minus the SID and DID identifier size) */
    if (dataCfg.FixedLength == TRUE)
    {
      /* !LINKSTO Dcm473.RequestLength.OnlyFixedLengthSignals,1 */
      if (MaxSignalEndingPosition == DidInfo->ReqLen)
      {
        DidLengthCheckResult = DCM_E_OK;
      }
    }
    /* If the did contains variable length signals, the length of the request data (minus the SID and DID identifier size)
     * should contain enough data when considering the minimum and maximum value of the variable length signal */
    else
    {
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.VariableLengthSignals.MinimumLength,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.VariableLengthSignals.MaximumLength,1 */
      if ((MaxSignalEndingPosition >= DidInfo->ReqLen) &&
          (MaxSignalEndingPositionWithoutVarLen <= DidInfo->ReqLen))
      {
        DidLengthCheckResult = DCM_E_OK;
      }
    }
  }

  /* the service handler has to verify that the request also contains data (ISO requirement) */
  DCM_INVARIANT_ASSERT((DidInfo->ReqLen != 0U), DCM_INTERNAL_API_ID);

  if (DidLengthCheckResult == DCM_E_OK)
  {
    /* next state: select signal to read */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY;
  }
  else
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)  */
  {
    /* set NRC and exit processing */
    DCM_DSP_CONTEXT_NRC(context)    = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
}
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))) */

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_RangeCheckLen(Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo
    = &DCM_DSP_GETDIDINFO_CONTEXT(context);

  if (DidInfo->ReqLen > Dcm_DidRangeConfig[DidInfo->ConfigIdx].MaxDataLength)
  {
    /* set NRC and exit processing */
    DCM_DSP_CONTEXT_NRC(context)    = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
  else
  {
    /* next state: write ranged Did's data */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_GETDIDINFO_CHECKSECURITY;
  }
}
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_CheckSecurity(
  Dcm_Dsp_ContextPtr context)
{
  /* !LINKSTO Dcm.Dsn.DidServices.IB.GetDidInfo_CheckSecurity,1 */
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo =
    &DCM_DSP_GETDIDINFO_CONTEXT(context);

  /* get the configured access info according to the requested did operation */
  Dcm_DidAccessInfoBaseTypePtr AccessInfo =
    Dcm_Dsp_DidServices_GetAccessInfo(DidInfo->InfoIdx, DidInfo->DidOp);

  /* this state is only entered if a Did is configured and has a valid AccessInfo */
  DCM_PRECONDITION_ASSERT(DidInfo->ConfigType != DCM_DID_UNCONFIGURED, DCM_INTERNAL_API_ID);
  DCM_PRECONDITION_ASSERT(NULL_PTR != AccessInfo, DCM_INTERNAL_API_ID);

  /* Check for Session access, whether the DID can be read in the current security level */
  if (TRUE == Dcm_Dsp_DidServices_IsDidAvailableInSecLevel(AccessInfo))
  {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    /* !LINKSTO Dcm819,1, Dcm822,1, Dcm823,1 */
    if (AccessInfo->ModeRuleDidFunction != NULL_PTR)
    {
      /* invoke the mode rule function for the service */
      const boolean DidIsAvailable =
        AccessInfo->ModeRuleDidFunction(&DCM_DSP_CONTEXT_NRC(context));

      if (TRUE == DidIsAvailable)
      {
        /* Did is available */
        DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
      }
      else
      {
        /* Set the ModeRuleFailed flag when ModeRule check returned E_NOT_OK */
        DidInfo->ModeRuleFailed = TRUE;
        /* result remains DCM_E_NOT_OK */
      }
    }
    else
#endif
    {
      /* all static and dynamic condition checks passed -> did is available */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
    }
  }
  else
  {
    /* set the NRC */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.SecurityAccessDenied.TransmitNRC0x33,1 */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_SECURITYACCESSDENIED;
  }
}
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON)) */
/*
 * Processing steps for function:  ReadDid
 */

/* !LINKSTO Dcm.Dsn.DidServices.IB.ReadDid_GetDidInfo,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_GetDidInfo (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) DidInfoContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext;

  Std_ReturnType result;

  /* Check if the Did can be read in the current coditions as per the configuration
   * and get the neccessary Did info for reading (not passed by caller yet) */
  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_GetDidInfo_Cancel(DidInfoContextPtr);
  }
  else
  {
    /* execute subroutine: get the Did info */
    result = Dcm_Dsp_DidServices_GetDidInfo_Execute(DidInfoContextPtr);
  }

  if (DCM_E_OK == result)
  { /* Did can be read, select the next step */
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
    if (DidInfoContextPtr->DidInfoContext.ConfigType == DCM_DID_TYPE_ID_DYNAMIC)
    {
      /* next step: select and read the dynamic Did */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSTOREDIDID;
    }
    else
#endif
    {
      /* if the Did is available, the config type cannot be DCM_DID_UNCONFIGURED */
      DCM_PRECONDITION_ASSERT(
        DidInfoContextPtr->DidInfoContext.ConfigType != DCM_DID_UNCONFIGURED,
        DCM_INTERNAL_API_ID);

      /* next step: select and read the static did */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_STATICSELECT;
    }
  }
  else
  {
#if(DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    /* Check if the Did is unconfigured and default interface is active. */
    if(DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE == DidInfoContextPtr->DidInfoContext.ConfigType)
    {
       /* Next step: Default read for an unconfigured Did. */
       DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DEF_READ;
    }
    else
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */
    {
      /* inherit the result and error code of the subroutine */
      DCM_DSP_CONTEXT_RESULT(context) = result;
      DCM_DSP_CONTEXT_NRC(context)    =
        DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(DidInfoContextPtr));
    }
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_StaticSelect (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).ReadStaticDidContext;

  /* initialize the subfunction ReadStaticDid */
  Dcm_Dsp_DidServices_ReadStaticDid_Init(
    ReadStaticDidContextPtr,
    /* Deviation MISRAC2012-2 <+3> */
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext,
    DCM_DSP_READDID_CONTEXT(context).WriteDidIds,
    DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr);

  DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_STATICREADDID;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_StaticRead (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).ReadStaticDidContext;

  Std_ReturnType result;

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadStaticDid_Cancel(ReadStaticDidContextPtr);
  }
  else
  {
    /* execute subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadStaticDid_Execute(ReadStaticDidContextPtr);
  }

  /* inherit the result and error code of the subroutine */
  DCM_DSP_CONTEXT_RESULT(context) = result;
  DCM_DSP_CONTEXT_NRC(context)    =
    DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadStaticDidContextPtr));

  /* execution path finishes here */
}

#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0) )
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidSelect (Dcm_Dsp_ContextPtr context)
{
  /* OBD PidServices does not follow the Context concept, no Init called here */

  /* next state: read the Pid */
  DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_PID_READ;
}

STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdPidRead (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadSingleStaticDidContextExtType, AUTOMATIC, DCM_VAR)
    ReadSingleStaticDidContextPtr = &DCM_DSP_READSINGLESTATICDID_CONTEXT(context);

  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    ReadSingleStaticDidContextPtr->BufferCfgPtr;

  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    ReadSingleStaticDidContextPtr->DidInfoPtr;

  const uint8 Pid = (uint8)(DidInfoPtr->Did & DCM_UINT8_MAX);

  const Std_ReturnType result = Dcm_Dsp_PidServices_Execute(
    Pid, &DCM_DSP_CONTEXT_NRC(context), BufferCfgPtr);

  /* revert buffer update if function returned an error */
  if (DCM_E_NOT_OK == result)
  {
    /* OBD services do not provide an availability check. If service was requested to write the
     * Did id it must be removed from buffer again */
     /* Deviation MISRAC2012-2 */
    if (DCM_DSP_READSINGLESTATICDID_CONTEXT(context).WriteDidIds == DCM_WRITE_DID_IDS)
    {
      /* if a Did id has been added the data size must be at least DID_ID_SIZE (2 bytes) */
      DCM_PRECONDITION_ASSERT(BufferCfgPtr->Occupied >= DCM_DID_ID_SIZE, DCM_INTERNAL_API_ID);

      /* remove Did id from buffer */
      BufferCfgPtr->Occupied -= DCM_DID_ID_SIZE;
    } /* else: no Did id has been written, nothing to do */
  }

  /* inherit the result of the subroutine; error code has been written directly */
  DCM_DSP_CONTEXT_RESULT(context) = result;

  /* execution path finishes here */
}
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0) ) */

#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidSelect (Dcm_Dsp_ContextPtr context)
{
  /* OBD MidServices does not follow the Context concept, no Init called here */

  /* next state: read the Pid */
  DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_MID_READ;
}

STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdMidRead (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadSingleStaticDidContextExtType, AUTOMATIC, DCM_VAR)
    ReadSingleStaticDidContextPtr = &DCM_DSP_READSINGLESTATICDID_CONTEXT(context);

  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    ReadSingleStaticDidContextPtr->BufferCfgPtr;

  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    ReadSingleStaticDidContextPtr->DidInfoPtr;

  const uint8 Mid = (uint8)(DidInfoPtr->Did & DCM_UINT8_MAX);

  /* OBD MidServices does neither follow the Context concept nor use the standard buffer type
     and data handling -> local adaptation required */
  /* target buffer */
  CONSTP2VAR(uint8, AUTOMATIC, DCM_VAR) BufferPtr =
    &BufferCfgPtr->BufferPtr[BufferCfgPtr->Occupied];
  /* remaining buffer space */
  const uint16 Size = (uint16)((BufferCfgPtr->Size - BufferCfgPtr->Occupied) & DCM_UINT16_MAX);
  /* data length (output) */
  Dcm_MsgLenType DataLength = 0U;

  Std_ReturnType Result = E_NOT_OK;

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
  /* !LINKSTO Dcm.EB.ReadDataByIdentifier.MidServices.Locking,1 */
  Std_ReturnType LockStatus = Dcm_Dsp_MidServices_LockServices();

 /* Keep track of the Locked MidServices so that they can be unlocked at the end of this
  * function if needed. Since the reading of MIDs is purely synchronous, there can be no
  * DCM_E_PEDNING response and therefore the unlocking can happen during the same call/cycle */
  boolean Dcm_Dsp_DidServices_HaveMidServicesLock = TRUE;

  /* If the MidServices could NOT be locked because they are already locked by something else */
  if (LockStatus == DCM_E_RESOURCE_LOCKED)
  {
    /* Lock could not be acquired so there is nothing to unlock at the end of this function */
    Dcm_Dsp_DidServices_HaveMidServicesLock = FALSE;

    /* If the MidServices could not be locked the service handler returns
     * NRC ConditionsNotCorrect (0x22)  */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_CONDITIONSNOTCORRECT;

    /* finish processing */
    Result = E_NOT_OK;
  }
  else
#endif /* #if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON) */
  {
    /* Get availability or non-availability data? */
    if (DCM_IS_AVAILABILITY_OBDSUBID(Mid) == TRUE)
    {
      Result = Dcm_Dsp_MidServices_GetAvailabilityMid(Mid, BufferPtr, Size, &DataLength
                                                      ,&DCM_DSP_CONTEXT_NRC(context));
    }
    else
    {
      Result = Dcm_Dsp_MidServices_GetNonAvailabilityMid(Mid, BufferPtr, Size, &DataLength
                                                        ,&DCM_DSP_CONTEXT_NRC(context)
                                                        ,DCM_WRITEMIDBEFORETID_ENABLED);
    }

    /* update buffer usage only if function executed successfully */
    if (DCM_E_OK == Result)
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.OBD_DID_F6xx.DTRDataObtained.PosResponse,1 */
      BufferCfgPtr->Occupied += DataLength;
    }
    else
    {
      /* reading OBD is synchron; if Result is not OK it must be NOT_OK */
       DCM_PRECONDITION_ASSERT(E_NOT_OK == Result, DCM_INTERNAL_API_ID);

      /* OBD services do not provide an availability check. If service was requested to write the
       * Did id it must be removed from buffer again */
      /* Deviation MISRAC2012-2 */
      if (DCM_DSP_READSINGLESTATICDID_CONTEXT(context).WriteDidIds == DCM_WRITE_DID_IDS)
      {
        /* if a Did id has been added the data size must be at least DID_ID_SIZE (2 bytes) */
        DCM_PRECONDITION_ASSERT(BufferCfgPtr->Occupied >= DCM_DID_ID_SIZE, DCM_INTERNAL_API_ID);

        /* remove Did id from buffer */
        BufferCfgPtr->Occupied -= DCM_DID_ID_SIZE;
      } /* else: no Did id has been written, nothing to do */
    }
  }

#if (DCM_DSP_MIDSERVICES_USE_LOCKING == STD_ON)
  /* If the MidServices were locked, unlock them now. */
  if (Dcm_Dsp_DidServices_HaveMidServicesLock == TRUE)
  {
    Dcm_Dsp_MidServices_UnlockServices();
  }
#endif

  /* inherit the Result of the subroutine; error code has been written directly */
  DCM_DSP_CONTEXT_RESULT(context) = Result;

  /* execution path finishes here */
}
#endif /* DCM_DSP_USE_SERVICE_0X06 == STD_ON */

#if ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0))
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadSingleStaticDidContextExtType, AUTOMATIC, DCM_VAR)
    ReadSingleStaticDidContextPtr = &DCM_DSP_READSINGLESTATICDID_CONTEXT(context);

  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    ReadSingleStaticDidContextPtr->BufferCfgPtr;

  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    ReadSingleStaticDidContextPtr->DidInfoPtr;

  const uint8 InfoId = (uint8)(DidInfoPtr->Did & DCM_UINT8_MAX);

  Std_ReturnType result = Dcm_Dsp_VinServices_HelperInit(
    InfoId,
    &ReadSingleStaticDidContextPtr->ReadInfoContext,
    BufferCfgPtr);

  if (DCM_E_OK == result)
  {
    /* next state: read the OBD info type */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_INFO_READ;
  }
  else
  {
    /* inherit the result and error code of the subroutine */
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    = ReadSingleStaticDidContextPtr->ReadInfoContext.nrc;
  }
}

STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoRead (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadSingleStaticDidContextExtType, AUTOMATIC, DCM_VAR)
    ReadDidSingleStaticDidContextPtr = &DCM_DSP_READSINGLESTATICDID_CONTEXT(context);

  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    ReadDidSingleStaticDidContextPtr->BufferCfgPtr;

  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    ReadDidSingleStaticDidContextPtr->DidInfoPtr;

  /* OBD VehInfo services does not fully follow the Context concept and does
     not store the requested Id inside its context (yet) */
  const uint8 InfoId = (uint8)(DidInfoPtr->Did & DCM_UINT8_MAX);

  Std_ReturnType result;

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    Dcm_Dsp_VinServices_HelperCancel(&ReadDidSingleStaticDidContextPtr->ReadInfoContext);
    Dcm_Dsp_VinServices_UnlockService();
    result = DCM_E_DONE;
  }
  else
  {
    /* OBD VehInfo services does not fully follow the Context concept and does
       not store its own state inside its context (yet) */
    if (DCM_PENDING == DCM_DSP_CONTEXT_OPSTATUS(context))
    {
      result = Dcm_Dsp_VinServices_HelperExecute(
                 DCM_PENDING, InfoId, &ReadDidSingleStaticDidContextPtr->ReadInfoContext);
    }
    else
    {
      result = Dcm_Dsp_VinServices_HelperExecute(
                 DCM_INITIAL, InfoId, &ReadDidSingleStaticDidContextPtr->ReadInfoContext);
    }
  }

/* revert buffer update if function returned an error */
  if (DCM_E_NOT_OK == result)
  {
    /* OBD services do not provide an availability check. If service was requested to write the
     * Did id it must be removed from buffer again */
    /* Deviation MISRAC2012-2 */
    if (DCM_DSP_READSINGLESTATICDID_CONTEXT(context).WriteDidIds == DCM_WRITE_DID_IDS)
    {
      /* if a Did id has been added the data size must be at least DID_ID_SIZE (2 bytes) */
      DCM_PRECONDITION_ASSERT(BufferCfgPtr->Occupied >= DCM_DID_ID_SIZE, DCM_INTERNAL_API_ID);

      /* remove Did id from buffer */
      BufferCfgPtr->Occupied -= DCM_DID_ID_SIZE;
    } /* else: no Did id has been written, nothing to do */

    Dcm_Dsp_VinServices_UnlockService();
  }
  else if(DCM_E_PENDING == result)
  {
    DCM_DSP_CONTEXT_OPSTATUS(context) = DCM_PENDING;
  }
  else
  {
     Dcm_Dsp_VinServices_UnlockService();
  }

  /* inherit the result and error code of the subroutine */
  DCM_DSP_CONTEXT_RESULT(context) = result;
  DCM_DSP_CONTEXT_NRC(context)    = ReadDidSingleStaticDidContextPtr->ReadInfoContext.nrc;

  /* execution path finishes here */
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadSingleStaticDid.ObdInfoSelect.TryLock,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_ObdInfoSelect_TryLock(Dcm_Dsp_ContextPtr context)
{
  const Std_ReturnType  VinLockStatus = Dcm_Dsp_VinServices_LockService();

  if (VinLockStatus == DCM_E_RESOURCE_LOCKED)
  {
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
  }
  else
  {
    DCM_POSTCONDITION_ASSERT(VinLockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);

    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_INFO_SELECT;
  }
}

#endif /* ((DCM_DSP_USE_SERVICE_0X09 == STD_ON) && (DCM_DSP_VEHINFO_NUM > 0)) */
#endif /* DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON */

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynStoreDidId(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferCfgPtr =
    DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr;

  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext;

  /* Check if the dynamic Did id shall be added to the response */
  /* Deviation MISRAC2012-2 */
  if (DCM_DSP_READDID_CONTEXT(context).WriteDidIds == DCM_WRITE_DID_IDS)
  {
    if (FALSE != Dcm_Dsp_DidServices_StoreDidId(DidBufferCfgPtr, DidInfoPtr->Did))
    {
      /* next step: read the data */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTSRC;
    }
    else
    {
      /* Buffer is too small for the response data; Set the Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_RESPONSETOOLONG;
      /* finish processing */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  }
  else
  {
    /* next step: read the data */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTSRC;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynSelectSrc (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext;

  /* Deviation MISRAC2012-2 */
  const uint8 DynDidIdx = DCM_DSP_READDID_CONTEXT(context).DynDidIdx;

  /* checked if the end of the list of referenced Dids is reached */
  if (DynDidIdx < Dcm_DidInfo[DidInfoPtr->InfoIdx].DDDidMaxNumSrcElements)
  {
    const Dcm_DDDidSrcTableIdxType SrcTableIdx =
      Dcm_DidConfig[DidInfoPtr->ConfigIdx].DDDidSrcTableIdx + DynDidIdx;

    CONSTP2CONST(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) SrcTableEntry =
      &Dcm_DDDidSrcTable[SrcTableIdx];

    if ((SrcTableEntry->srcType & DCM_DDDID_SRC_TYPE_DID) == DCM_DDDID_SRC_TYPE_DID)
    {
      /* Deviation MISRAC2012-2 */
      CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) DynDidInfoContextPtr =
        &DCM_DSP_READDID_CONTEXT(context).DynDidInfoContext;

      /* initialize DidInfo for the Did to be read */
      Dcm_Dsp_DidServices_GetDidInfo_Init(
        DynDidInfoContextPtr,
        DynDidIdx,
        DidInfoPtr->DidOp,
        DidInfoPtr->ProtocolIdx);

      /* next state: select / prepare a did to be read */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNGETDIDINFO;
    }
#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
    else if ((SrcTableEntry->srcType & DCM_DDDID_SRC_TYPE_MEM) == DCM_DDDID_SRC_TYPE_MEM)
    {
      /* next state: select / prepare a memory to be read */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTMEM;
    }
#endif /* DCM_0X2C_0X02_SSVC_ENABLED == STD_ON */
    else /* DCM_DDDID_SRC_TYPE_NONE */
    {
      /* reading of dynamically defined Did is finished */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
    }
  }
  else
  {
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynGetDidInfo (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).ReadStaticDidContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) DynDidInfoContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).DynDidInfoContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DynDidBufferCfgPtr =
    &DCM_DSP_READDID_CONTEXT(context).DynDidBufferCfg;

  /* Deviation MISRAC2012-2 */
  const Dcm_DidInfoIdxType DynDidIdx = DCM_DSP_READDID_CONTEXT(context).DynDidIdx;

  /* the DDDid config index is the base source table index + the DDDid index */
  const Dcm_DDDidSrcTableIdxType SrcTableIdx =
    Dcm_DidConfig[DidInfoPtr->ConfigIdx].DDDidSrcTableIdx + DynDidIdx;

  CONSTP2CONST(Dcm_DDDidDefByDataIdentifierType, AUTOMATIC, DCM_VAR) DDDidDefPtr =
    &Dcm_DDDidSrcTable[SrcTableIdx].dataIdDDDid;

  Std_ReturnType result;

  /* store the Did to be read */
  DynDidInfoContextPtr->DidInfoContext.Did         = DDDidDefPtr->srcDid;
  /* Deviation MISRAC2012-2 */
  DynDidInfoContextPtr->DidInfoContext.ProtocolIdx =
    DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext.ProtocolIdx;

  /* get the Did info */
  result = Dcm_Dsp_DidServices_GetDidInfo_Execute(DynDidInfoContextPtr);

  if (DCM_E_OK == result)
  {
    /* if the DID is dynamic, it is composed of other elements that shall be
     * written to the dynamic DID buffer first */
    DynDidBufferCfgPtr->BufferPtr = &DynamicDidBuffer[0U];
    DynDidBufferCfgPtr->Size      = DCM_DID_MAX_SIZE;
    DynDidBufferCfgPtr->Occupied  = 0U;

    Dcm_Dsp_DidServices_ReadStaticDid_Init(
      ReadStaticDidContextPtr,
      &DynDidInfoContextPtr->DidInfoContext,
      DCM_DO_NOT_WRITE_DID_IDS /* no Did ids for data elements of dynamic Dids */,
      DynDidBufferCfgPtr);

    /* next state: read a did */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNREADDID;
  }
  else if (DCM_E_NOT_OK == result)
  {
    /* inherit the Nrc from the subfunction */
    DCM_DSP_CONTEXT_NRC(context) =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(DynDidInfoContextPtr));

    /* Did is not available, decide how to continue depending on result */
    if (DynDidInfoContextPtr->DidInfoContext.ConfigType == DCM_DID_UNCONFIGURED)
    {
      /* Did is not configured, continue with the next source */
      /* Deviation MISRAC2012-2 */
      DCM_DSP_READDID_CONTEXT(context).DynDidIdx += 1U;
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTSRC;
    }
    else
    {
      /* Did is configured but cannot be accessed, stop reading */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  }
  else /* result == DCM_E_PENDING */
  {
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynReadDid (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).ReadStaticDidContext;

  /* execute subroutine: read the selected Did */
  Std_ReturnType result =
    Dcm_Dsp_DidServices_ReadStaticDid_Execute(ReadStaticDidContextPtr);

  if (result == DCM_E_OK)
  {
    /* The DID was successfully read, therefore copy data from the dynamic did
       buffer to the response buffer and advance to the next data element */
    /* Deviation MISRAC2012-2 */
    CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferCfgPtr =
      DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr;

    /* Deviation MISRAC2012-2 */
    CONSTP2CONST(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DynDidBufferCfgPtr =
      &DCM_DSP_READDID_CONTEXT(context).DynDidBufferCfg;

    /* Deviation MISRAC2012-2 */
    const Dcm_DidInfoIdxType DynDidIdx =
      DCM_DSP_READDID_CONTEXT(context).DynDidIdx;

    /* Deviation MISRAC2012-2 */
    CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
      &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext;

    /* the DDDid config index is the base source table index + the DDDid index */
    const Dcm_DDDidSrcTableIdxType SrcTableIdx =
      Dcm_DidConfig[DidInfoPtr->ConfigIdx].DDDidSrcTableIdx + DynDidIdx;

    CONSTP2CONST(Dcm_DDDidDefByDataIdentifierType, AUTOMATIC, DCM_VAR) DDDidDefPtr =
      &Dcm_DDDidSrcTable[SrcTableIdx].dataIdDDDid;

    if ((DidBufferCfgPtr->Occupied + DDDidDefPtr->srcDidSize) <= DidBufferCfgPtr->Size)
    {
      TS_MemCpy(
        &DidBufferCfgPtr->BufferPtr[DidBufferCfgPtr->Occupied],
        &DynDidBufferCfgPtr->BufferPtr[DDDidDefPtr->srcDidPos],
        DDDidDefPtr->srcDidSize);

      DidBufferCfgPtr->Occupied += DDDidDefPtr->srcDidSize;

      /* Deviation MISRAC2012-2 */
      DCM_DSP_READDID_CONTEXT(context).DynDidIdx += 1U;

      /* processing of this dynamic Did src finished, select the next source */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTSRC;
    }
    else
    {
      /* Buffer is too small for the response data; Set the Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_RESPONSETOOLONG;
    }
  }
  else
  {
    /* inherit the result of the subfunction */
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadStaticDidContextPtr));
  }
}

#if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynSelectMem (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    &DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext;

  /* Deviation MISRAC2012-2 */
  const Dcm_DidInfoIdxType DynDidIdx =
    DCM_DSP_READDID_CONTEXT(context).DynDidIdx;

  /* the DDDid config index is the base source table index + the DDDid index */
  const Dcm_DDDidSrcTableIdxType SrcTableIdx =
    Dcm_DidConfig[DidInfoPtr->ConfigIdx].DDDidSrcTableIdx + DynDidIdx;

  CONSTP2VAR(Dcm_DDDidDefByMemAddressType, AUTOMATIC, DCM_VAR) DDDidDefPtr =
    &Dcm_DDDidSrcTable[SrcTableIdx].memAddrDDDid;

  /* memory is directly written to the output buffer */
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferCfgPtr =
    DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr;

  Dcm_Dsp_MemoryServices_ReadMemory_Setup(
    /* Deviation MISRAC2012-2 */
    &DCM_DSP_READDID_CONTEXT(context).DynDidReadMemContext,
    DDDidDefPtr->srcMemAddr,
    DDDidDefPtr->srcMemSize,
    DidBufferCfgPtr->BufferPtr,
    DidBufferCfgPtr->Occupied,
    DidBufferCfgPtr->Size);

  DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNREADMEM;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DynReadMem (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadMemoryContextType, AUTOMATIC, DCM_VAR) DynDidReadMemContextPtr =
    &DCM_DSP_READDID_CONTEXT(context).DynDidReadMemContext;

  Std_ReturnType result =
    Dcm_Dsp_MemoryServices_ReadMemory_Execute(DynDidReadMemContextPtr);

  switch (result)
  {
    case DCM_E_DONE:
    case DCM_E_NOT_OK:
    {
      uint8 nrc;
      uint32 AmountOfListBufferOccupied;

      Std_ReturnType ReadMemResult =
        Dcm_Dsp_MemoryServices_ReadMemory_GetResults(DynDidReadMemContextPtr,
                                                    &nrc,
                                                    &AmountOfListBufferOccupied);

      if (DCM_E_DONE == result)
      {
        if (ReadMemResult != DCM_E_NOT_OK)
        {
          /* update the buffer pointer */
          /* Deviation MISRAC2012-2 */
          CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferCfgPtr =
            DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr;

          DidBufferCfgPtr->Occupied = AmountOfListBufferOccupied;

          /* The memory was successfully read, therefore advance to the next data element*/
          /* Deviation MISRAC2012-2 */
          DCM_DSP_READDID_CONTEXT(context).DynDidIdx += 1U;

          /* processing of this dynamic Did src finished, select the next source */
          DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READDID_DYNSELECTSRC;
        }
        else
        {
          DCM_DSP_CONTEXT_NRC(context)    = DCM_E_GENERALREJECT;
          DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
        }
      }
      else /* result == DCM_E_NOT_OK */
      {
        DCM_PRECONDITION_ASSERT(ReadMemResult == DCM_E_NOT_OK, DCM_INTERNAL_API_ID);
        DCM_DSP_CONTEXT_NRC(context) = nrc;

        /* stop processing of all dynamic dids */
        DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
      }
    }
    break;

    case DCM_E_PENDING:
    {
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
    }
    break;

    case DCM_E_FORCE_RCRRP:
    {
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_FORCE_RCRRP;
      DynDidReadMemContextPtr->MemoryAddressReadProcessingState =
                                                      DCM_MEMORY_ADDRESS_READ_CONFIRM_FORCED_RCRRP;
    }
    break;

    case DCM_E_RESOURCE_LOCKED:
    {
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_RESOURCE_LOCKED;
    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}
#endif /* #if (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */

/* Dcm.Dsn.IB.DidServices.IsDidInDynamicRange,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInDynamicRange(uint16 Did)
{
  boolean result = FALSE;

  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineById.DynamicallyDefined.InvalidRange,1 */
  /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.DefineByMemoryAddress.InvalidRange,1 */
  if ((Did >= DCM_DID_DMC_LOW_LIMIT) && (Did <= DCM_DID_DMC_UP_LIMIT))
  {
    result = TRUE;
  }

  return result;
}

/* Dcm.Dsn.IB.DidServices.IsDidDynamicallyConfigured,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidDynamicallyConfigured(
  Dcm_DidConfigIdxType DidConfigIdx)
{
  boolean result = FALSE;

  const Dcm_DDDidSrcTableIdxType SrcTableIdx = Dcm_DidConfig[DidConfigIdx].DDDidSrcTableIdx;
  CONSTP2CONST(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) SrcTablePtr =
    &Dcm_DDDidSrcTable[SrcTableIdx];

  /* the caller has to ensure that the Did can be configured dynamically */
  DCM_PRECONDITION_ASSERT(SrcTableIdx != DCM_DDDIDSRCTABLEIDX_INVALID, DCM_INTERNAL_API_ID);

  /* the dynamic configuration can NOT have gaps, only look at the first entry */
  if (SrcTablePtr->srcType != DCM_DDDID_SRC_TYPE_NONE)
  {
    /* at least one source element is configured */
    result = TRUE;
  }

  return result;
}
#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_DefaultRead (Dcm_Dsp_ContextPtr context)
{
  /* Result of the user's Default Read function. */
  Std_ReturnType Result = DCM_E_NOT_OK;
  /* Negative Response Code returned from Dcm_DefaultDIDRead() function. */
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Data Identifier Length returned from Dcm_DefaultDIDRead() function. */
  uint32 Did_Length = 0U;
  /* Position in the buffer of the current Did. */
  uint32 DidPosition = DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr->Occupied;

  /* Call the user's Default Read custom implementation. */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.Dcm_DefaultDIDRead,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.Timeout.Cancellation,1 */
  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.ProtocolPreemption.Cancellation,1 */
  Result = Dcm_DefaultDIDRead(
             DCM_DSP_CONTEXT_OPSTATUS(context),
             (Dcm_DIDIDType)(DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext.Did),
             &(DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr->BufferPtr[DidPosition + DCM_DID_ID_SIZE]),
             &Did_Length,
             &Nrc);

  if(DCM_E_OK == Result)
  {
    /* Check if the Did is stored and buffer configuration is updated.
     * Check also if the Did Data Length returned from Dcm_DefaultDIDRead() function togheter
     * with the Did ID Size does not cause an overflow in the Buffer. */
    if((FALSE != Dcm_Dsp_DidServices_StoreDidId(
                  (DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr),
                  DCM_DSP_READDID_CONTEXT(context).DidInfoContext.DidInfoContext.Did)) &&
    ((DidPosition + Did_Length + DCM_DID_ID_SIZE) <= DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr->Size))
    {
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.OK,1 */
      /* Increase the Length of the buffer with the Did's user data length. */
      (DCM_DSP_READDID_CONTEXT(context).DidBufferCfgPtr->Occupied) += Did_Length;
      /* Inherit the result and error code of the Dcm_DefaultDIDRead() function. */
      DCM_DSP_CONTEXT_NRC(context) = Nrc;
      /* Processing finished. */
      DCM_DSP_CONTEXT_RESULT(context) = Result;
    }
    else
    {
      /* Buffer is too small for the response data; Set the Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_RESPONSETOOLONG;
      /* Processing finished. */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  }
  else if(DCM_E_NOT_OK == Result)
  {
    if(DCM_E_POSITIVERESPONSE == Nrc)
    {
      /* Return generalReject as a Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.0x00,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
      /* Processing finished. */
      DCM_DSP_CONTEXT_RESULT(context) = Result;
    }
    else
    {
      /* Processing finished. */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.RequestOutOfRange,1 */
      DCM_DSP_CONTEXT_RESULT(context) = Result;
      /* Inherit the result and error code of the Dcm_DefaultDIDRead() function. */
      DCM_DSP_CONTEXT_NRC(context) = Nrc;
    }
  }
  else if(DCM_E_PENDING == Result)
  {
    /* Result remains DCM_E_PENDING. */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.Pending,1 */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_PENDING;
    DCM_DSP_CONTEXT_OPSTATUS(context) = DCM_PENDING;
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.DET,1 */
    DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

    /* Return generalReject as a Negative Response Code. */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.NRC,1 */
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
    /* Processing finished. */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
  }
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

/*
 * DidServices function: ReadStaticDid
 *
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Init(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr,
  P2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  boolean WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr)
{
  /* initialize base context */
  DCM_DSP_CONTEXT_INIT(ReadStaticDidContextPtr, Dcm_ReadStaticDidStateConfig);

  /* initialize context for ReadSingleStaticDid */
  ReadStaticDidContextPtr->ReadStaticDidContext.DidInfoPtr   = DidInfoPtr;
  ReadStaticDidContextPtr->ReadStaticDidContext.WriteDidIds  = WriteDidIds;
  ReadStaticDidContextPtr->ReadStaticDidContext.BufferCfgPtr = BufferCfgPtr;
#if (DCM_DID_REF_COUNT > 0U)
  ReadStaticDidContextPtr->ReadStaticDidContext.RefDidIdx = 0U;
#endif
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Execute(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr)
{
  /* process the function */
  return Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(ReadStaticDidContextPtr));
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_Cancel(
  Dcm_ReadStaticDidContextTypePtr ReadStaticDidContextPtr)
{
  /* cancel the function */
  return Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(ReadStaticDidContextPtr));
}

/*
 * Processing steps: ReadStaticDid
 *
 */
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadStaticDid.PrimarySelect,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_PrimarySelect (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    DCM_DSP_READSTATICDID_CONTEXT(context).DidInfoPtr;

  /* initialize context for ReadSingleStaticDid */
  Dcm_Dsp_DidServices_ReadSingleStaticDid_Init(
    /* Deviation MISRAC2012-2 <+4> */
    /* Context */ &DCM_DSP_READSTATICDID_CONTEXT(context).ReadSingleStaticDidContext,
    /* DidInfoPtr */ DidInfoPtr,
    /* WriteDidIds */ DCM_DSP_READSTATICDID_CONTEXT(context).WriteDidIds,
    /* BufferConfig */ DCM_DSP_READSTATICDID_CONTEXT(context).BufferCfgPtr);

  DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_PRIMARYREAD;
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadStaticDid.PrimaryRead,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_PrimaryRead (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr =
    &DCM_DSP_READSTATICDID_CONTEXT(context).ReadSingleStaticDidContext;

  Std_ReturnType result;

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadSingleStaticDid_Cancel(ReadSingleStaticDidContextPtr);
  }
  else
  {
    /* execute subroutine: read the selected Did */
    result = Dcm_Dsp_DidServices_ReadSingleStaticDid_Execute(ReadSingleStaticDidContextPtr);
  }

#if (DCM_DID_REF_COUNT > 0U)
  /* if referenced Dids are in configured, check if this particular Did references another */
  if (result == DCM_E_OK)
  {
    /* Deviation MISRAC2012-2 */
    const uint16 Did =
      DCM_DSP_READSTATICDID_CONTEXT(context).DidInfoPtr->Did;

    /* Deviation MISRAC2012-2 */
    DCM_DSP_READSTATICDID_CONTEXT(context).NumRefDids =
      Dcm_Dsp_DidServices_GetNumRefDids(Did);

    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_REFSELECT;
  }
  else
#endif
  {
    /* inherit the result of the subfunction */
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadSingleStaticDidContextPtr));
  }
}

#if (DCM_DID_REF_COUNT > 0U)
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadStaticDid.RefSelect,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefSelect
  (Dcm_Dsp_ContextPtr context)
{
  /* checked if the end of the list of referenced Dids is reached */
  /* Deviation MISRAC2012-2 <+2> */
  if (DCM_DSP_READSTATICDID_CONTEXT(context).RefDidIdx <
      DCM_DSP_READSTATICDID_CONTEXT(context).NumRefDids)
  {
    /* Deviation MISRAC2012-2 */
    CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
      DCM_DSP_READSTATICDID_CONTEXT(context).DidInfoPtr;

    /* Deviation MISRAC2012-2 */
    CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) RefDidInfoPtr =
      &DCM_DSP_READSTATICDID_CONTEXT(context).RefDidInfoContext;

    /* select the referenced Did to be read */
    /* Deviation MISRAC2012-2 */
    const uint16 RefDidIdx = DCM_DSP_READSTATICDID_CONTEXT(context).RefDidIdx;

    /* initialize DidInfo for the Did to be read */
    Dcm_Dsp_DidServices_GetDidInfo_Init(
      RefDidInfoPtr,
      Dcm_DidRefers[Dcm_DidConfig[DidInfoPtr->ConfigIdx].DidRefersIdx + RefDidIdx],
      DCM_DID_OP_READ,
      DidInfoPtr->ProtocolIdx);

    /* next state: check if the Did is available */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_REFGETDIDINFO;
  }
  else
  {
    /* reading of referenced Dids finished */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
  } /* else: result remains DCM_E_PENDING */
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadStaticDid.RefGetDidInfo,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefGetDidInfo (
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_DidInfoContextTypePtr const RefDidInfoContext =
    &DCM_DSP_READSTATICDID_CONTEXT(context).RefDidInfoContext;

  Std_ReturnType result;

  /* only static Dids can have references, this state is synchronous and cannot be cancelled */
  DCM_PRECONDITION_ASSERT(DCM_CANCEL != DCM_DSP_CONTEXT_OPSTATUS(context), DCM_INTERNAL_API_ID);

  result = Dcm_Dsp_DidServices_GetDidInfo_Execute(RefDidInfoContext);

  if (result == DCM_E_OK)
  {
    /* only static Dids defined by id can have references */
    DCM_PRECONDITION_ASSERT(
      RefDidInfoContext->DidInfoContext.ConfigType == DCM_DID_TYPE_ID_STATIC, DCM_INTERNAL_API_ID);

    /* initialize context for ReadSingleStaticDid */
    Dcm_Dsp_DidServices_ReadSingleStaticDid_Init(
      /* Deviation MISRAC2012-2 <+4> */
      /* Context */ &DCM_DSP_READSTATICDID_CONTEXT(context).ReadSingleStaticDidContext,
      /* DidInfoPtr */ &RefDidInfoContext->DidInfoContext,
      /* WriteDidIds */ DCM_WRITE_DID_IDS,
      /* BufferConfig */ DCM_DSP_READSTATICDID_CONTEXT(context).BufferCfgPtr);

    /* next step: read the Did */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_REFREAD;
  }
  else /* (result == DCM_E_NOT_OK) */
  {
    /* only static Dids can have references, GetDidInfo is synchronous and cannot return PENDING */
    DCM_PRECONDITION_ASSERT(
      DCM_CANCEL != DCM_DSP_CONTEXT_OPSTATUS(context), DCM_INTERNAL_API_ID);

    /* referenced Dids cannot be unconfigured (checked at config time) */
    DCM_PRECONDITION_ASSERT(
      RefDidInfoContext->DidInfoContext.ConfigType != DCM_DID_UNCONFIGURED, DCM_INTERNAL_API_ID);

    /* inherit the Nrc from the subfunction */
    DCM_DSP_CONTEXT_NRC(context) =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(RefDidInfoContext));

    /* Did is not available, select the next step depending on result */
    if (DCM_DSP_CONTEXT_NRC(context) == DCM_E_REQUESTOUTOFRANGE)
    {
      /* Did configured but cannot be accessed (range or session error), continue with
         the next Did */
      /* Deviation MISRAC2012-2 */
      DCM_DSP_READSTATICDID_CONTEXT(context).RefDidIdx += 1U;
      /* go back to setup the next Did read */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_REFSELECT;
    }
    else
    {
      /* Did configured but cannot be accessed (security or other error), stop reading */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadStaticDid.RefRead,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadStaticDid_RefRead (Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  Dcm_ReadSingleStaticDidContextTypePtr const ReadSingleStaticDidContextPtr =
    &DCM_DSP_READSTATICDID_CONTEXT(context).ReadSingleStaticDidContext;

  Std_ReturnType result;

  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    /* cancel reading the referenced Did */
    result = Dcm_Dsp_DidServices_ReadSingleStaticDid_Cancel(ReadSingleStaticDidContextPtr);
  }
  else
  {  /* execute reading the referenced Did */
    result = Dcm_Dsp_DidServices_ReadSingleStaticDid_Execute(ReadSingleStaticDidContextPtr);
  }

  if (DCM_E_OK == result)
  {
    /* reading cycle is completed, move on to the next referenced Did */
    /* Deviation MISRAC2012-2 */
    DCM_DSP_READSTATICDID_CONTEXT(context).RefDidIdx += 1U;
    /* go back to setup the next Did read */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSTATICDID_REFSELECT;
  }
  else
  {
    /* inherit the result of the subfunction */
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    =
      DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(ReadSingleStaticDidContextPtr));
  }
}
#endif /* #if (DCM_DID_REF_COUNT > 0) */

/*
 * DidServices function: ReadSingleStaticDid
 *
 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Init(
  Dcm_ReadSingleStaticDidContextTypePtr                     ReadSingleStaticDidContextPtr,
  P2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  boolean                                                   WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR)   BufferCfgPtr)
{
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) ReadSignalContextPtr =
    &ReadSingleStaticDidContextPtr->ReadSingleStaticDidContext.ReadSignalContext;

  /* initialize base context */
  DCM_DSP_CONTEXT_INIT(ReadSingleStaticDidContextPtr, Dcm_ReadSingleStaticDidStateConfig);

  /* initialize context for ReadSingleStaticDid */
  ReadSingleStaticDidContextPtr->ReadSingleStaticDidContext.WriteDidIds  = WriteDidIds;
  ReadSingleStaticDidContextPtr->ReadSingleStaticDidContext.BufferCfgPtr = BufferCfgPtr;
  ReadSingleStaticDidContextPtr->ReadSingleStaticDidContext.DidInfoPtr   = DidInfoPtr;

  /* subfunctions are accessing did configuration items based on this index */
  ReadSignalContextPtr->CurrentDidCfgIndex = DidInfoPtr->ConfigIdx;

  /* legacy support: initialize local signal data reading */
  Dcm_Dsp_DidServices_DidSetStatus(DidInfoPtr, ReadSignalContextPtr);
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Execute(
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr)
{
  /* process the function */
  return Dcm_Dsp_Context_Execute(DCM_DSP_CONTEXT(ReadSingleStaticDidContextPtr));
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_Cancel(
  Dcm_ReadSingleStaticDidContextTypePtr ReadSingleStaticDidContextPtr)
{
  /* cancel the function */
  return Dcm_Dsp_Context_Cancel(DCM_DSP_CONTEXT(ReadSingleStaticDidContextPtr));
}

/*
 * Processing steps: ReadSingleStaticDid
 *
 */
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadSingleStaticDid.StoreDidId,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_StoreDidId(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) StoreBufferCfg =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).BufferCfgPtr;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).DidInfoPtr;

  /* Check if the Did id shall be added to the response */
  /* Deviation MISRAC2012-2 */
  if (DCM_DSP_READSINGLESTATICDID_CONTEXT(context).WriteDidIds == DCM_WRITE_DID_IDS)
  {
    if (FALSE != Dcm_Dsp_DidServices_StoreDidId(StoreBufferCfg, DidInfoPtr->Did))
    {
      /* next step: read the data */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_SELECT_DATA;
    }
    else
    {
      /* Buffer is too small for the response data; Set NRC */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.NRC0x14,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_RESPONSETOOLONG;
      /* finish processing */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
  }
  else
  {
    /* next step: read the data */
    DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_SELECT_DATA;
  }
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadSingleStaticDid.SelectDidDataSrc,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_SelectDidDataSrc(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).DidInfoPtr;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) ReadSignalContextPtr =
    &DCM_DSP_READSINGLESTATICDID_CONTEXT(context).ReadSignalContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).BufferCfgPtr;

  /* legacy support: copy the current buffer config to the legacy context */
  ReadSignalContextPtr->outputBuffer = BufferCfgPtr->BufferPtr;
  ReadSignalContextPtr->BufferLimit  = BufferCfgPtr->Size;
  ReadSignalContextPtr->AmountOfBufferOccupied = BufferCfgPtr->Occupied;

  /* depending on the Did type (local, or range) selected the next step */
  switch (DidInfoPtr->ConfigType)
  {
    /* !LINKSTO Dcm.ReadDataById.DcmDspPid.DIDType,1 */
    /* !LINKSTO Dcm.ReadDataById.DcmDspTestResultObdmidTid.DIDType,1 */
    /* !LINKSTO Dcm.ReadDataById.DcmDspVehInfo.DIDType,1 */
    case DCM_DID_TYPE_ID_STATIC:
    {
#if (DCM_DID_SIGNALS_COUNT > 0U)
      /* read the data of a Did provided locally */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_LOCAL_DATA;

      /* legacy support: initialize local signal data reading */
      Dcm_Dsp_DidServices_DidSetStatus(DidInfoPtr, ReadSignalContextPtr);
#else
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
#endif /* (DCM_DID_SIGNALS_COUNT > 0U) */
    }
    break;

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
    case DCM_DID_TYPE_RANGE_NO_GAPS:
    case DCM_DID_TYPE_RANGE_GAPS:
    {
      /* read the data of a Did provided by a Did range */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_RANGE_DATA;
    }
    break;
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
    /* !LINKSTO Dcm.ReadDataById.DcmDspPid.PID,1 */
    case DCM_DID_TYPE_OBD_PID:
    {
      /* read the data of a Did provided by OBD PidServices */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_PID_SELECT;
    }
    break;

    /* !LINKSTO Dcm.ReadDataById.DcmDspTestResultObdmidTid.OBDMID,1 */
    case DCM_DID_TYPE_OBD_MID:
    {
      /* read the data of a Did provided by OBD PidServices */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_MID_SELECT;
    }
    break;

    /* !LINKSTO Dcm.ReadDataById.DcmDspVehInfo.InfoType,1 */
    case DCM_DID_TYPE_OBD_INFO:
    {
      /* read the data of a Did provided by OBD PidServices */
      DCM_DSP_CONTEXT_NEXT(context) = DCM_DIDSERVICES_READSINGLESTATICDID_OBDINFOSELECT_TRYLOCK;
    }
    break;
#endif /* (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON) */

    /* CHECK: NOPARSE */
    default:
    {
      /* this subfunction cannot directly be used to read dynamic Dids */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
      /* finish processing */
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;

      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }
}

#if (DCM_DID_SIGNALS_COUNT > 0U)
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadSingleStaticDid.ReadLocalData,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadLocalData(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) ReadSignalContextPtr =
    &DCM_DSP_READSINGLESTATICDID_CONTEXT(context).ReadSignalContext;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).BufferCfgPtr;

  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).DidInfoPtr;

  Std_ReturnType result;

  /* legacy support: copy the OpStatus into the legacy context */
  ReadSignalContextPtr->CurrentOpStatus = DCM_DSP_CONTEXT_OPSTATUS(context);

  ReadSignalContextPtr->DidOp = DidInfoPtr->DidOp;

  /* execute the subfunction */
  result = Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute(ReadSignalContextPtr);

  if (DCM_E_DONE == result)
  {
    /* legacy support: copy the new buffer level from the legacy context */
    BufferCfgPtr->Occupied = ReadSignalContextPtr->AmountOfBufferOccupied;

    /* this DID has finished processing */
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_OK;
  }
  else
  {
    DCM_DSP_CONTEXT_RESULT(context) = result;
    DCM_DSP_CONTEXT_NRC(context)    = ReadSignalContextPtr->NrcConditions;

    if (ReadSignalContextPtr->NrcConditions == DCM_E_REQUESTOUTOFRANGE)
    {
      BufferCfgPtr->Occupied -= DCM_DID_ID_SIZE;
    }
  }
}
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DidServices.ReadSingleStaticDid.ReadRangeData,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadSingleStaticDid_ReadRangeData(
  Dcm_Dsp_ContextPtr context)
{
  /* Deviation MISRAC2012-2 */
  CONSTP2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).DidInfoPtr;

  /* Deviation MISRAC2012-2 */
  CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr =
    DCM_DSP_READSINGLESTATICDID_CONTEXT(context).BufferCfgPtr;

  /* Deviation MISRAC2012-2 */
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) ReadSignalContextPtr =
    &DCM_DSP_READSINGLESTATICDID_CONTEXT(context).ReadSignalContext;

  P2VAR(Dcm_Union_BufferType, AUTOMATIC, DCM_VAR) pDIDGeneralBuffer = Dcm_Dsp_DidServices_GetDidGeneralBuffer();

  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  Dcm_OpStatusType OpStatus;
  uint16 DataLength = 0U;
  Std_ReturnType result;

  /* cancellation is handled via the context */
  if (DCM_CANCEL == DCM_DSP_CONTEXT_OPSTATUS(context))
  {
    OpStatus = DCM_CANCEL;
  }
  else
  {
    /* INITIAL/PENDING state is handled via NewSignal state variable.
       The variable is initialized from function ReadSingleStaticDid_Init in DidSetStatus */
    if (ReadSignalContextPtr->NewSignal == TRUE)
    {
      OpStatus = DCM_INITIAL;
    }
    else
    {
      OpStatus = DCM_PENDING;
    }
  }

  result = Dcm_DidRangeConfig[DidInfoPtr->ConfigIdx].ReadFnc(
             DidInfoPtr->Did,
             pDIDGeneralBuffer->BufferUint8,
             OpStatus,
             &DataLength,
             &Nrc);

  DCM_DSP_CONTEXT_RESULT(context) = result;

  if (DCM_E_OK == result)
  {
    if (DataLength > (BufferCfgPtr->Size - BufferCfgPtr->Occupied))
    {
      /* response message buffer is too small */
      /* !LINKSTO Dcm.EB.ReadDataById.ReadDidData.NRC0x14,1 */
      DCM_DSP_CONTEXT_NRC(context) = DCM_E_RESPONSETOOLONG;
      DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    }
    else
    {
      /* copy data into the response message buffer */
      TS_MemCpy(&BufferCfgPtr->BufferPtr[BufferCfgPtr->Occupied],
                pDIDGeneralBuffer->BufferUint8,
                DataLength);
      /* update the number of bytes used in the buffer */
      BufferCfgPtr->Occupied += DataLength;
    }
    ReadSignalContextPtr->NewSignal = TRUE;
  }
  else if (DCM_E_NOT_OK == result)
  {
    /* inherit the Nrc of the callout function */
    DCM_DSP_CONTEXT_NRC(context) = Nrc;
    ReadSignalContextPtr->NewSignal = TRUE;
  }
  else if (DCM_E_PENDING == result)
  {
    ReadSignalContextPtr->NewSignal = FALSE;
  }
  else
  {
    /* unexpected interface return value */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* !LINKSTO Dcm.DET.InterfaceReturnValue, 2 */
    DCM_DET_REPORT_ERROR(DCM_SVCID_READDIDDATA, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    ReadSignalContextPtr->NewSignal = TRUE;
    DCM_DSP_CONTEXT_RESULT(context) = DCM_E_NOT_OK;
    DCM_DSP_CONTEXT_NRC(context) = DCM_E_GENERALREJECT;
  }
}
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

/* Dcm.Dsn.IB.DidServices.IsDidInObdRange,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInObdRange(uint16 Did)
{
  boolean result = FALSE;

  if ((Did >= DCM_DID_OBD_READ_LOW_LIMIT) && (Did <= DCM_DID_OBD_READ_UP_LIMIT))
  {
    result = TRUE;
  }

  return result;
}

/* !LINKSTO Dcm.Dsn.DidServices.IB.IsDidInValidRange,2 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidInValidRange(
  uint16        reqDid,
  Dcm_DidOpType DidOp )
{
  boolean result = FALSE;
  const boolean inObdRange = Dcm_Dsp_DidServices_IsDidInObdRange(reqDid);

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  const boolean inDynamicRange = Dcm_Dsp_DidServices_IsDidInDynamicRange(reqDid);

  if ((reqDid <= DCM_DID_STD_RW_UP_LIMIT)
      ||
      /* Special case for FF00U DID*/
      (reqDid == DCM_DID_FF00)
      ||
      /* system supplier specific record data identifiers */
      ((reqDid >= DCM_DID_SUP_LOW_LIMIT) && (reqDid <= DCM_DID_SUP_UP_LIMIT))
      ||
      /* Periodic Did range */
      ((reqDid >= DCM_PDID_STD_READ_LOW_LIMIT) && (reqDid <= DCM_PDID_STD_READ_UP_LIMIT))
      ||
      /* second range for reading (and Ioctl) Dids */
      ( ((DCM_DID_OP_READ == DidOp) || (DCM_DID_OP_IOCONTROL == DidOp)) &&
        (reqDid >= DCM_DID_STD_READ_LOW_LIMIT) && (reqDid <= DCM_DID_STD_READ_UP_LIMIT))
      ||
      /* OBD range */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.WriteDIDFromOBDRange,1 */
      (((DCM_DID_OP_READ == DidOp) || (DCM_DID_OP_WRITE == DidOp)) && (inObdRange))
      ||
      /* Dynamic Did range */
      (((DCM_DID_OP_DYNAMIC == DidOp) || (DCM_DID_OP_READ == DidOp)) && (inDynamicRange)))
#else
  if ((reqDid <= DCM_DID_STD_RW_UP_LIMIT)
      ||
      /* Special case for FF00U DID*/
      (reqDid == DCM_DID_FF00)
      ||
      /* system supplier specific record data identifiers */
      ((reqDid >= DCM_DID_SUP_LOW_LIMIT) && (reqDid <= DCM_DID_SUP_UP_LIMIT))
      ||
      /* Periodic Did range */
      ((reqDid >= DCM_PDID_STD_READ_LOW_LIMIT) && (reqDid <= DCM_PDID_STD_READ_UP_LIMIT))
      ||
      /* second range for reading (and Ioctl) Dids */
      ( ((DCM_DID_OP_READ == DidOp) || (DCM_DID_OP_IOCONTROL == DidOp)) &&
        (reqDid >= DCM_DID_STD_READ_LOW_LIMIT) && (reqDid <= DCM_DID_STD_READ_UP_LIMIT))
      ||
      /* OBD range */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.WriteDIDFromOBDRange,1 */
      (((DCM_DID_OP_READ == DidOp) || (DCM_DID_OP_WRITE == DidOp)) && (inObdRange)))
#endif
  {
    result = TRUE;
  }

  return result;
}

#if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON))
/* Check for security access , If the DID can be read in the current security level*/
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_IsDidAvailableInSecLevel(
  Dcm_DidAccessInfoBaseTypePtr AccessInfo)
{
  boolean didIsAvailable = FALSE;

  /* check if access info exists and has entries */
  if ((NULL_PTR != AccessInfo) && (AccessInfo->NumSecLevels > 0U))
  {
    /* get current security level */
    Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    if (curSecLevel != DCM_SEC_LEV_LOCKED)
    {
      uint8_least secLevIdx;

      /* Check for security level */
      for (secLevIdx = 0U; secLevIdx < AccessInfo->NumSecLevels; ++secLevIdx)
      {
        if (curSecLevel == AccessInfo->SecLevels[secLevIdx])
        {
          didIsAvailable = TRUE;
          break;
        }
      }
    }
  }
  else
  {
    /* Dcm614_Conf:  If there is no reference, no check of security level shall be done. */
    didIsAvailable = TRUE;
  }

  return didIsAvailable;
}
#endif /* #if ((DCM_DSP_DIDSERVICES_NUM_DIDS > 0) || (DCM_DSP_USE_DIDRANGES == STD_ON)) */

FUNC_P2CONST(Dcm_DidAccessInfoBaseType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetAccessInfo(
  Dcm_DidInfoIdxType didInfoIdx,
  Dcm_DidOpType      didOpType )
{
  Dcm_DidAccessInfoBaseTypePtr AccessInfo = NULL_PTR;

  DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_ENTRY(didInfoIdx, didOpType);
  /* get the configured access info according to the requested did operation */
#if (DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO > 0U)
  if ( (DCM_DID_OP_READ == didOpType) ||
       (DCM_DID_OP_PERIODIC == didOpType) ||
       (DCM_DID_OP_DYNAMIC == didOpType) )
  {/* bugzilla#52874 */
    const Dcm_DidReadAccessInfoIdxType AccessInfoIdx =
      Dcm_DidInfo[didInfoIdx].ReadAccessInfoIdx;

    /* check if access has been configured */
    if (AccessInfoIdx != DCM_DIDREADACCESSINFOIDX_INVALID)
    {
      AccessInfo = &DCM_DID_READACCESS_INFO(AccessInfoIdx);
    }
  }
  else
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO > 0U) */
  {
#if (DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO > 0U)
    if (DCM_DID_OP_WRITE == didOpType)
    {
      const Dcm_DidWriteAccessInfoIdxType AccessInfoIdx =
        Dcm_DidInfo[didInfoIdx].WriteAccessInfoIdx;

      /* check if access has been configured */
      if (AccessInfoIdx != DCM_DIDWRITEACCESSINFOIDX_INVALID)
      {
        AccessInfo = &DCM_DID_WRITEACCESS_INFO(AccessInfoIdx);
      }
    }
    else
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO > 0U) */
    {
#if (DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO > 0U)
      if (DCM_DID_OP_IOCONTROL == didOpType)
      {
        const Dcm_DidIoctlAccessInfoIdxType AccessInfoIdx =
          Dcm_DidInfo[didInfoIdx].IoctlAccessInfoIdx;

        /* check if access has been configured */
        if (AccessInfoIdx != DCM_DIDIOCTLACCESSINFOIDX_INVALID)
        {
          AccessInfo = &DCM_DID_IOCTLACCESS_INFO(AccessInfoIdx);
        }
      }
      else
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO > 0U) */
#if ((DCM_DSP_USE_SERVICE_0X24 == STD_ON) && \
     (DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO > 0U))
      {
        if(DCM_DID_OP_READSCALING == didOpType)
        {
          const Dcm_DidReadScalingAccessInfoIdxType AccessInfoIdx =
            Dcm_DidInfo[didInfoIdx].ReadScalingAccessInfoIdx;

          /* check if access has been configured */
          if (AccessInfoIdx != DCM_DIDREADSCALINGACCESSINFOIDX_INVALID)
          {
            AccessInfo = &DCM_DID_READSCALINGACCESS_INFO(AccessInfoIdx);
          }
        }
      }
#endif /* #if ((DCM_DSP_USE_SERVICE_0X24 == STD_ON) && \
               (DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO > 0U)) */
      {
        /* function called with unsupported DidOp, AccessInfo remains NULL_PTR */
      }
    }
  }

#if ((DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO == 0U) &&      \
     (DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO == 0U) &&     \
     (DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO == 0U) &&     \
     (DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO == 0))
  TS_PARAM_UNUSED(didInfoIdx);
  TS_PARAM_UNUSED(didOpType);
#endif

  DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_EXIT(AccessInfo, didInfoIdx, didOpType);
  return AccessInfo;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DidSetStatus(
  P2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfoPtr,
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR)   pReadSignalContext)
{
  /* Initializing signal index to start from from reading of first signal*/
  pReadSignalContext->SignalInd = 0U;
  /* Initializing the status of asynchronous signal state to process the next asynchronous signal*/
  pReadSignalContext->NrcConditions         = DCM_E_OK;
  /* Initialize endianess conversion */
  pReadSignalContext->EndiannessConversion =
    Dcm_ProtocolConfig[DidInfoPtr->ProtocolIdx].EndiannessConversion;
  /* Set signal processing state to initial */
  pReadSignalContext->NewSignal = TRUE;
}

STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_StoreDidId(
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfgPtr,
  uint16 Did)
{
  boolean result = FALSE;

  if ((BufferCfgPtr->Occupied + DCM_DID_ID_SIZE) <= BufferCfgPtr->Size)
  {
    /* Pointer for storing the DID and signals */
    Dcm_MsgType dataPtr = &BufferCfgPtr->BufferPtr[BufferCfgPtr->Occupied];

    dataPtr[0U] = (uint8)(Did >> 8U);
    dataPtr[1U] = (uint8)(Did);

    BufferCfgPtr->Occupied += DCM_DID_ID_SIZE;

    result = TRUE;
  }
  /* else: result remains FALSE */

  return result;
}


#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)

#if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON))
/* !LINKSTO SWS_Dcm_01097_PeriodicDDDIDSourcesCheckSession,1 */
/* !LINKSTO SWS_Dcm_01097_PeriodicDDDIDSourcesCheckSecurity,1 */
/* !LINKSTO SWS_Dcm_01097_PeriodicDDDIDSourcesCheckModeDependencies,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ValidateDDDIDSources(
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  P2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab = NULL_PTR;
  Dcm_NegativeResponseCodeType LocalNrc = *Nrc;
  Std_ReturnType ReturnVal = E_NOT_OK;
  Dcm_DidConfigIdxType DidCfgIdx = 0U;
  uint16 IndexSourceTable = 0U;
  uint16 MaxSrcElements = 0U;
  DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_ENTRY(RequestedPDID, Nrc);

  /* Check to determine wheter the requested PDID is a dynamically defined one and if so get
   * its index in the DidCfg table */
  for (DidCfgIdx = 0U; DidCfgIdx < DCM_DSP_DIDSERVICES_NUM_DIDS; DidCfgIdx++)
  {
    /* Check if the currently requested PDID is a dynamically defined one */
    if ((Dcm_DidConfig[DidCfgIdx].Did == RequestedPDID)
        && (Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx != DCM_DDDIDSRCTABLEIDX_INVALID)
       )
    {
      break;
    }
  }

  /* If the requested PDID is a dynamically defined one */
  if (DidCfgIdx < DCM_DSP_DIDSERVICES_NUM_DIDS)
  {
    DDDidSrcTab = &Dcm_DDDidSrcTable[Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx];

    /* The the maximal number of source elements that the requested dynamically
     * defined DID can have */
    MaxSrcElements = Dcm_DidInfo[Dcm_DidConfig[DidCfgIdx].DidInfoIdx].DDDidMaxNumSrcElements;

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    for (IndexSourceTable = 0;
         IndexSourceTable < MaxSrcElements;
         IndexSourceTable++
        )
    {
      if ( (DDDidSrcTab[IndexSourceTable].srcType & DCM_DDDID_SRC_TYPE_DID)
            ==
           DCM_DDDID_SRC_TYPE_DID
         )
      {
        CONSTP2VAR(Dcm_DDDIDContextType, AUTOMATIC, DCM_VAR) DDDIDContext = &Dcm_DDDIDContext;

        CONSTP2VAR(Dcm_DidInfoContextType, AUTOMATIC, DCM_VAR) SrcDidInfoContextPtr =
          &(DDDIDContext->srcDidInfoContext);

        /* Check if the source Did can be read in the current conditions as per the configuration */
        Dcm_Dsp_DidServices_GetDidInfo_Init(SrcDidInfoContextPtr,
                                            DDDidSrcTab[IndexSourceTable].dataIdDDDid.srcDid,
                                            DCM_DID_OP_READ, DDDIDContext->ProtocolId);

        ReturnVal = Dcm_Dsp_DidServices_GetDidInfo_Execute(SrcDidInfoContextPtr);

        /* Get the NRC from DID info context */
        if (ReturnVal != E_OK)
        {
          LocalNrc = DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(SrcDidInfoContextPtr));
        }
      }
#if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON)
      else if ( (DDDidSrcTab[IndexSourceTable].srcType & DCM_DDDID_SRC_TYPE_MEM)
                ==
                DCM_DDDID_SRC_TYPE_MEM
              )
      {
        /* Variable holding the index of MemoryRangeInfo table */
        uint16 memRangeIndex = 0U;

        /* Variable holding the index of MemoryIdInfo table */
        uint16 memIdIndex = 0U;

        /* Get the mem range and id info index. No need to check the result as the check was already
         * successfully performed when adding the source to the DDDID */
        (void)Dcm_Dsp_MemoryServices_ReadMemory_GetRange(
          &memRangeIndex,
          &memIdIndex,
          DDDidSrcTab[IndexSourceTable].memAddrDDDid.srcMemAddr,
          DDDidSrcTab[IndexSourceTable].memAddrDDDid.srcMemSize);

        ReturnVal = Dcm_Dsp_MemoryServices_ReadMemory_ConditionChecks(memRangeIndex, memIdIndex,
                                                                 &LocalNrc);
      }
#endif /* #if(DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) */
      else
      {
        /* Do nothing. The connection is busy */
      }

      /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.FailedDDDIDSourceVerification,1 */
      if ((DDDidSrcTab[IndexSourceTable].srcType == DCM_DDDID_SRC_TYPE_NONE)
          || (ReturnVal != E_OK)
         )
      {
        break;
      }
    }

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* This is not a DDDID so nothing could have failed */
    ReturnVal = E_OK;
  }

  /* Set the determined NRC */
  *Nrc = LocalNrc;

  DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_EXIT(ReturnVal, RequestedPDID, Nrc);
  return ReturnVal;
}

#endif /* #if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON)) */

/* !LINKSTO Dcm.Dsn.DidServices.IB.CheckPeriodicDid,1 */
FUNC(Dcm_CheckPeriodicDIDType, DCM_CODE) Dcm_Dsp_DidServices_CheckPeriodicDID(
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Dcm_NegativeResponseCodeType LocalNrc = DCM_E_REQUESTOUTOFRANGE;
  Std_ReturnType ReturnVal = DCM_CHECK_PDID_NOT_OK;
  Dcm_DidAccessInfoBaseTypePtr AccessInfo;
  Dcm_DidConfigIdxType ConfigIdx = 0U;

  /* Get the index of the PDID in the Dcm_DidConfig table  */
  if (Dcm_Dsp_DidServices_GetDidIndex(RequestedPDID, &ConfigIdx) == TRUE)
  {
#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
    boolean DidIsDnamicallyDefined = FALSE;
    boolean DidHasBeenConfigured = FALSE;

    /* Check if the currently requested PDID is a dynamically defined one */
    if (Dcm_DidConfig[ConfigIdx].DDDidSrcTableIdx != DCM_DDDIDSRCTABLEIDX_INVALID)
    {
      DidIsDnamicallyDefined = TRUE;

      /* Check if this dynamically defined DID is also configured i.e. it has as least one
       * source lement of DID or MEM type. */
      if (Dcm_DDDidSrcTable[Dcm_DidConfig[ConfigIdx].DDDidSrcTableIdx].srcType
          != DCM_DDDID_SRC_TYPE_NONE
         )
      {
        DidHasBeenConfigured = TRUE;
      }
    }

    /* Proceed only if the requested periodic DID exists and is statically defined, or dynamically
     * defined AND configrued */
    if (((DidIsDnamicallyDefined == TRUE) && (DidHasBeenConfigured == TRUE))
        || (DidIsDnamicallyDefined == FALSE)
       )
    {
#endif
      /* get the configured access info according to the requested did operation */
      AccessInfo = Dcm_Dsp_DidServices_GetAccessInfo(Dcm_DidConfig[ConfigIdx].DidInfoIdx,
                                                     DCM_DID_OP_PERIODIC);
      LocalNrc = DCM_E_SECURITYACCESSDENIED;
      ReturnVal = DCM_CHECK_PDID_NOT_OK;

      /* check if access info exists and has entries */
      if ((AccessInfo != NULL_PTR) && (AccessInfo->NumSecLevels > 0U))
      {
        /* get current security level */
        Dcm_SecLevelType curSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

        if (curSecLevel != DCM_SEC_LEV_LOCKED)
        {
          uint8_least secLevIdx;

          /* Check for security level */
          for (secLevIdx = 0U; secLevIdx < AccessInfo->NumSecLevels; ++secLevIdx)
          {
            if (curSecLevel == AccessInfo->SecLevels[secLevIdx])
            {
              LocalNrc = DCM_E_POSITIVERESPONSE;
              ReturnVal = DCM_CHECK_PDID_OK;
              break;
            }
          }
        }
      }
      else
      {
        /* Dcm614_Conf:  If there is no reference, no check of security level shall be done. */
        LocalNrc = DCM_E_POSITIVERESPONSE;
        ReturnVal = DCM_CHECK_PDID_OK;
      }

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
      /* !LINKSTO Dcm819,1, Dcm822,1, Dcm823,1 */
      if ((LocalNrc == DCM_E_POSITIVERESPONSE)
          && (AccessInfo != NULL_PTR)
          && (AccessInfo->ModeRuleDidFunction != NULL_PTR)
         )
      {
        /* invoke the mode rule function for the DID */
        const boolean DidIsAvailable =
          AccessInfo->ModeRuleDidFunction(&LocalNrc);

        if (TRUE != DidIsAvailable)
        {
          /* Did is not available */
          ReturnVal = DCM_CHECK_PDID_NOT_OK;
        }
      }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

      /* check if session info exists and has entries */
      if (LocalNrc == DCM_E_POSITIVERESPONSE)
      {
        if ((AccessInfo != NULL_PTR) && (AccessInfo->NumSesCtrls > 0U))
        {
          /* get the current session type */
          Dcm_SesCtrlType curSesCtrl = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();

          uint8_least sesLevIdx;

          /* The periodic DID will be ignored in case it is unsupported in current
           * diagnostic session */
          ReturnVal = DCM_CHECK_PDID_UNSUPPORTED_SESSION;

          for (sesLevIdx = 0U; sesLevIdx < AccessInfo->NumSesCtrls; ++sesLevIdx)
          {
            if ( (curSesCtrl == AccessInfo->SesCtrls[sesLevIdx]) ||
                (AccessInfo->SesCtrls[sesLevIdx] == DCM_ALL_SESSION_LEVEL))
            {
              ReturnVal = DCM_CHECK_PDID_OK;
              break;
            }
          }
        }
        else
        {
          /* Dcm615_Conf:  If there is no reference, no check of session level shall be done. */
          LocalNrc = DCM_E_POSITIVERESPONSE;
          ReturnVal = DCM_CHECK_PDID_OK;
        }
      }
#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
    }
    else
    {
      /* DID may be configured but if it's not defined, we treat it as if it wouldn't be there */
      ReturnVal = DCM_CHECK_PDID_DOES_NOT_EXIST;
    }
#endif
  }
  else
  {
    /* DID isn't even configured */
    ReturnVal = DCM_CHECK_PDID_DOES_NOT_EXIST;
  }

  *Nrc = LocalNrc;

  return ReturnVal;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */


#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DidServices.LockService,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_LockService(void)
{
  Std_ReturnType RetVal = DCM_E_RESOURCE_LOCKED;

  DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_ENTRY();

  RetVal = Dcm_Internal_GetLock(&Dcm_Dsp_DidServices_ServiceBusy);

  DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_EXIT(RetVal);

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.UnlockService,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_UnlockService(void)
{
  DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_ENTRY();

  Dcm_Internal_ReleaseLock(&Dcm_Dsp_DidServices_ServiceBusy);

  DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_EXIT();
}
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
#if (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON)
/* Clear DDDID i.e. remove all its source table entries */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Clear(uint16 Did)
{
  Dcm_DidConfigIdxType DidCfgIdx = 0U;

  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_ENTRY(Did);

  /* Deviation TASKING-1 */
  for (DidCfgIdx = 0U; DidCfgIdx < DCM_DSP_DIDSERVICES_NUM_DIDS; ++DidCfgIdx)
  {
    if ((Dcm_DidConfig[DidCfgIdx].Did == Did) || (Did == DCM_DDDID_CLEAR_ALL))
    {
      if (Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx != DCM_DDDIDSRCTABLEIDX_INVALID)
      {
        uint16 PDidIdx;

        CONSTP2VAR(Dcm_DDDidSrcTblEntryType, AUTOMATIC, DCM_VAR) DDDidSrcTab =
          &Dcm_DDDidSrcTable[Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx];

        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* Valid only if service 0x2A is in use. */
#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
        {
          /* In case clear all DDDIDs, it is needed to check all periodic ones. */
          uint16 clearDid;

          if (Did == DCM_DDDID_CLEAR_ALL)
          {
            clearDid = Dcm_DidConfig[DidCfgIdx].Did;
          }
          else
          {
            clearDid = Did;
          }

          /* Check if the DDDID is used in periodic transmissions and if so first stop
           * transmission for the DDDID and then remove it */
          /* Only check for DIDs that are in range 0xF200-0xF2FF. */
          if ((clearDid >= DCM_PDID_STD_READ_LOW_LIMIT) && (clearDid <= DCM_PDID_STD_READ_UP_LIMIT))
          {
            /* Remove the periodic DID as it can no longer be read under current conditions */
            uint8 Buffer[1];
            Buffer[0] = (uint8)clearDid;

            /* !LINKSTO Dcm.EB.DynamicallyDefineDataIdentifier.StopPeriodicDidBeforeClear,1 */
            Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(Buffer, 1U);
          }
        }
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

        PDidIdx = 0U;

        while ((PDidIdx < Dcm_DidInfo[Dcm_DidConfig[DidCfgIdx].DidInfoIdx].DDDidMaxNumSrcElements)&&
               (DDDidSrcTab[PDidIdx].srcType != DCM_DDDID_SRC_TYPE_NONE))
        {
          DDDidSrcTab[PDidIdx].srcType = DCM_DDDID_SRC_TYPE_NONE;
          PDidIdx++;
        }

        /* LEAVE critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      }
    }

    if (Dcm_DidConfig[DidCfgIdx].Did == Did)
    {
      /* If there is DDDID specified, stop loop since there are no other with same value. */
      break;
    }
  }

  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_EXIT(Did);
}
#endif /* (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON) */

FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Initialized(void)
{
  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_ENTRY();

  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_EXIT(Dcm_DynamicDidInitialized);

  return Dcm_DynamicDidInitialized;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_InitInternal(boolean NvMRestoreFailed)
{
  Dcm_DidConfigIdxType DidCfgIdx;
  Dcm_DDDidSrcTableIdxType SrcTblIdx;
  uint8 MaxSrc;
  uint8 Idx;
  boolean ClearPeriodic = FALSE;
  boolean ClearNonPeriodic = FALSE;

#if (DCM_DDDID_PERSISTENCE == STD_OFF)
  TS_PARAM_UNUSED(NvMRestoreFailed);
#endif

/* !LINKSTO Dcm.DynamicallyDefineDataIdentifier.DcmDDDIDStorage.FALSE,1 */
#if ((DCM_DDDID_POLICY_PERIODIC == DCM_PRDI_PERIODIC_CLEAR_ALWAYS) || \
     ((DCM_DDDID_POLICY_PERIODIC == STD_OFF) && (DCM_DDDID_PERSISTENCE == STD_OFF)))
  ClearPeriodic = TRUE;
#endif
/* !LINKSTO Dcm.DynamicallyDefineDataIdentifier.DcmDDDIDStorage.FALSE,1 */
#if ((DCM_DDDID_POLICY_NONPERIODIC == DCM_DDDID_CLEAR_ALWAYS) || \
     ((DCM_DDDID_POLICY_NONPERIODIC == STD_OFF) && (DCM_DDDID_PERSISTENCE == STD_OFF)))
  ClearNonPeriodic = TRUE;
#endif

#if ((DCM_DDDID_PERSISTENCE == STD_ON) && (DCM_DDDID_DO_NOT_INIT == STD_OFF))
  /* If an NvM problem occurres, clean the DDDID source table, except when both policy settings
     (DcmDynamicallyDefinedDIDInitPolicy, DcmDynamicallyDefinedPeriodicDIDInitPolicy ) are set to
     CLEAR_ONLY_POWERON */
  /* !LINKSTO Dcm.DynamicallyDefineDataIdentifier.DcmDDDIDStorage.TRUE,1 */
  if (NvMRestoreFailed == TRUE)
  {
    ClearPeriodic = TRUE;
    ClearNonPeriodic = TRUE;
  }
#endif /* (DCM_DDDID_PERSISTENCE == STD_ON) && DCM_DDDID_DO_NOT_INIT == STD_OFF) */

  /* Deviation TASKING-1 */
  for (DidCfgIdx = DCM_DDDID_INIT_START_INDEX; DidCfgIdx <= DCM_DDDID_INIT_STOP_INDEX; DidCfgIdx++)
  {
    /* If dynamically defined */
    if (Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx != DCM_DDDIDSRCTABLEIDX_INVALID)
    {
      /* Reset it if it is a periodic one or ALL DDDIDs need to be reset */
      /* !LINKSTO Dcm.EB.Init.Policy.Periodic.DDDID,1 */
      /* !LINKSTO Dcm.EB.Init.Policy.NonPeriodic.DDDID,1 */
      /* Deviation TASKING-2 <+7> */
      if (((TRUE == ClearPeriodic) &&
           (Dcm_DidConfig[DidCfgIdx].Did >= (uint16)0xF200U) &&
           (Dcm_DidConfig[DidCfgIdx].Did <= (uint16)0xF2FFU))
           ||
          ((TRUE == ClearNonPeriodic) &&
           (Dcm_DidConfig[DidCfgIdx].Did >= (uint16)0xF300U) &&
           (Dcm_DidConfig[DidCfgIdx].Did <= (uint16)0xF3FFU)))
      {
        /* Reset/initialize the configured source items of this DDDID */
        MaxSrc = Dcm_DidInfo[Dcm_DidConfig[DidCfgIdx].DidInfoIdx].DDDidMaxNumSrcElements;
        SrcTblIdx = Dcm_DidConfig[DidCfgIdx].DDDidSrcTableIdx;

        for (Idx = 0; Idx < MaxSrc; ++Idx)
        {
          Dcm_DDDidSrcTable[SrcTblIdx + Idx].srcType = DCM_DDDID_SRC_TYPE_NONE;
        }
      }
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.DidServices.DynamicDid.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Init(void)
{
  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_ENTRY();

#if (DCM_DDDID_PERSISTENCE == STD_OFF)
  Dcm_Dsp_DidServices_DynamicDid_InitInternal(FALSE);
  Dcm_DynamicDidInitialized = TRUE;
#else /* DCM_DDDID_PERSISTENCE == STD_OFF */
  /* The initialization will be done by the Dcm_Dsp_DidServices_DynamicDid_InitPersistent call */
  Dcm_DynamicDidInitialized = FALSE;
  Dcm_Dsp_DidServices_DynamicDid_InitPersistent();
#endif /* DCM_DDDID_PERSISTENCE == STD_OFF */

  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_EXIT();
}

#if (DCM_DDDID_PERSISTENCE == STD_ON)
/* DDDID Init if persistency is configured */
/* !LINKSTO Dcm.EB.Config.DynamicallyDefineDataIdentifer.ReinitializeMemory,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_InitPersistent(void)
{
  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_ENTRY();

  if (FALSE == Dcm_DynamicDidInitialized)
  {
    NvM_RequestResultType RestoredNvmData = NVM_REQ_NOT_OK;
    Std_ReturnType NvMRetVal;

    /* get status from NvM of Dcm DDDID persistent block */
    NvMRetVal = NvM_GetErrorStatus(DCM_DDDID_NVM_BLOCK_ID, &RestoredNvmData);

    if (E_OK == NvMRetVal)
    {
      if ((NVM_REQ_OK == RestoredNvmData) || (NVM_REQ_REDUNDANCY_FAILED == RestoredNvmData))
      {
        Dcm_Dsp_DidServices_DynamicDid_InitInternal(FALSE);
        Dcm_DynamicDidInitialized = TRUE;
      }
      else if (NVM_REQ_PENDING == RestoredNvmData)
      {
        /* Nvm returned PENDING, wait for next cycle */
      }
      else
      {
        /* Some problem occured, report via DET */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        if (RestoredNvmData > NVM_REQ_RESTORED_FROM_ROM)
        {
          /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer.NvMRead.InvalidValue,1 */
          DCM_DET_REPORT_ERROR(DCM_SERVID_INIT, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
        /* Reset the sourcetable if needed */
        Dcm_Dsp_DidServices_DynamicDid_InitInternal(TRUE);
        (void)NvM_SetRamBlockStatus(DCM_DDDID_NVM_BLOCK_ID, /* BlockChanged */ TRUE);
        Dcm_DynamicDidInitialized = TRUE;
      }
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if (E_NOT_OK != NvMRetVal)
      {
        /* !LINKSTO Dcm.DET.InterfaceReturnValue, 2 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMGETERRORSTATUS, DCM_E_INTERFACE_RETURN_VALUE);
      }

      /* !LINKSTO Dcm.EB.Init.DDDIDConfigurationLoadFailed,1 */
      DCM_DET_REPORT_ERROR(DCM_SERVID_INIT, DCM_E_NVM_READ_FAIL);
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */

      Dcm_Dsp_DidServices_DynamicDid_InitInternal(TRUE);
      Dcm_DynamicDidInitialized = TRUE;
      (void)NvM_SetRamBlockStatus(DCM_DDDID_NVM_BLOCK_ID, /* BlockChanged */ TRUE);
    }
  } /* (FALSE == Dcm_DynamicDidInitialized) */

  DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_EXIT();
}

#endif /* #if (DCM_DDDID_PERSISTENCE == STD_ON) */
#endif /* #if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON) */

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && \
     (DCM_DID_ENDIANNESS_CONVERSION == STD_ON) && \
     (DCM_DID_SIGNALS_COUNT > 0U))
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_DidServices_EndianessConversion_CalculateDataPosition (
  Dcm_DidDataByteType DataType,
  uint16              DataLength,
  uint16              DataPosition)
{
  uint16 SignalLength = DataPosition;

  if ((DataType == DCM_TYPE_SINT16) || (DataType == DCM_TYPE_UINT16))
  {
    if ((DataLength % 16U) != 0U)
    {
      SignalLength += (uint16)((((DataLength / 16U) + 1U) * 16U) - DataLength);
    }
  }
  else if ((DataType == DCM_TYPE_SINT32) || (DataType == DCM_TYPE_UINT32))
  {
    if ((DataLength % 32U) != 0U)
    {
      SignalLength += (uint16)((((DataLength / 32U) + 1U) * 32U) - DataLength);
    }
  }
  else
  {
    /* no transformation required */
  }

  return SignalLength;
}
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && \
           (DCM_DID_ENDIANNESS_CONVERSION == STD_ON) && \
           (DCM_DID_SIGNALS_COUNT > 0U)) */

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_ScalingInfoSizeFitsIntoBuffer(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext)
{
  Dcm_ReadScalingSignalSizeType totalScalingInfoSize = 0;
  /* Total number of signals configured for the DID*/
  uint16 numSignals = Dcm_DidConfig[pReadSignalContext->CurrentDidCfgIndex].NumSignals;
  uint16 signalIndex = pReadSignalContext->SignalInd;
  boolean result = TRUE;

  while(signalIndex < numSignals)
  {
    /* holds the configuration table of signals in the DID */
    CONSTP2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
      Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[pReadSignalContext->CurrentDidCfgIndex].DidSignalsIdx);

    totalScalingInfoSize += Dcm_Dsp_DidSignals_GetScalingInfoSize(SignalCfg[pReadSignalContext->SignalInd].DspDataIndex);
    signalIndex++;
  }

  if(totalScalingInfoSize + pReadSignalContext->AmountOfBufferOccupied >= pReadSignalContext->BufferLimit)
  {
    result = FALSE;
  }

  return result;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_DIDSERVICES == STD_ON)) */
/*==================[end of file]================================================================*/
