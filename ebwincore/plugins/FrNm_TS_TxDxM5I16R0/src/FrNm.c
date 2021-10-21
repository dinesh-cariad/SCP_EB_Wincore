/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO FRNM064_Refine,1 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions or as structure member
 * where the number of parentheses matter.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * In the context of "for(instIdx=0U; instIdx<FRNM_NUMBER_OF_CHANNELS; ++instIdx)"
 * when "FRNM_NUMBER_OF_CHANNELS = 1" the message condition is always true is expected;
 * Expression "instIdx < FRNM_NUMBER_OF_CHANNELS" is always evaluated as true
 * if "FRNM_NUMBER_OF_CHANNELS = 1".
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * In the context of:
 * "for (instIdx = 0; instIdx < FRNM_NUMBER_OF_TX_PDUS; instIdx++)
 *  {
 *    if (FrNm_RootPtr->TxPduMap[instIdx].TxPduId == PduId)
 *    {
 *      break;
 *    }
 *  }"
 * instIdx parameter is never greater or equal to FRNM_NUMBER_OF_TX_PDUS since
 * FrNm_RootPtr->TxPduMap always contains PduId;
 * Expression "instIdx < FRNM_NUMBER_OF_TX_PDUS" is added for defensive programming.
 */

/*==================[includes]===========================================*/

#define FRNM_NO_CFGCLASSMIX_REQUIRED
#ifndef FRNM_NO_PBCFG_REQUIRED
#define FRNM_NO_PBCFG_REQUIRED
#endif

#include <FrNm_Trace.h>
/* !LINKSTO FRNM065,1 */
#include <FrNm_Api.h>              /* FrNm API definitions (own interface) */
#include <FrNm_Int.h>          /* FrNm internal interface */
/* !LINKSTO FRNM065,1 */
#include <FrNm_Cbk.h>          /* FRNM callback API (own interface) */
#include <FrNm_Hsm.h>          /* public API of FrNm_Hsm.c */
#include <FrNm_HsmFrNm.h>      /* public statechart model definitions, */
#include <FrNm_Lcfg.h>
/* !LINKSTO FRNM066_SchM,1 */
#include <SchM_FrNm.h>         /* SchM-header for FrNm */
/* !LINKSTO FRNM066_Nm_Cbk,1 */
#include <Nm_Cbk.h>            /* Nm callback API */
/* !LINKSTO FRNM066_FrIf,1 */
#include <FrIf.h>              /* FrIf API */
#include <TSMem.h>

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (FRNM_DET_REPORT_ERROR() macros) */
/* !LINKSTO FRNM066_Det,1 */
#include <Det.h>
#endif

#if ((FRNM_COM_USER_DATA_SUPPORT == STD_ON) || (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON))
/* !LINKSTO FRNM066_PduR,1 */
#include <PduR_FrNm.h>         /* PduR API for FrNm */
#endif
#include <FrNm_Lcfg_Static.h>

/*==================[macro definitions]==================================*/
/* !LINKSTO FrNm.Version.Check,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRNM_VENDOR_ID /* configuration check */
#error FRNM_VENDOR_ID must be defined
#endif

#if (FRNM_VENDOR_ID != 1U) /* vendor check */
#error FRNM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRNM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRNM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRNM_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRNM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRNM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRNM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRNM_AR_RELEASE_MINOR_VERSION != 0U )
#error FRNM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef FRNM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRNM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRNM_AR_RELEASE_REVISION_VERSION != 3U )
#error FRNM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRNM_SW_MAJOR_VERSION /* configuration check */
#error FRNM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRNM_SW_MAJOR_VERSION != 5U)
#error FRNM_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef FRNM_SW_MINOR_VERSION /* configuration check */
#error FRNM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRNM_SW_MINOR_VERSION < 16U)
#error FRNM_SW_MINOR_VERSION wrong (< 16U)
#endif

#ifndef FRNM_SW_PATCH_VERSION /* configuration check */
#error FRNM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRNM_SW_PATCH_VERSION < 7U)
#error FRNM_SW_PATCH_VERSION wrong (< 7U)
#endif





/*------------------------[Defensive programming]----------------------------*/

#if (defined FRNM_PRECONDITION_ASSERT)
#error FRNM_PRECONDITION_ASSERT is already defined
#endif

#if (defined FRNM_PRECONDITION_ASSERT_NO_EVAL)
#error FRNM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRNM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))
#else
#define FRNM_PRECONDITION_ASSERT(Condition, ApiId)
#define FRNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRNM_POSTCONDITION_ASSERT)
#error FRNM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FRNM_POSTCONDITION_ASSERT_NO_EVAL)
#error FRNM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRNM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))
#else
#define FRNM_POSTCONDITION_ASSERT(Condition, ApiId)
#define FRNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRNM_INVARIANT_ASSERT)
#error FRNM_INVARIANT_ASSERT is already defined
#endif

#if (defined FRNM_INVARIANT_ASSERT_NO_EVAL)
#error FRNM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FRNM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))
#else
#define FRNM_INVARIANT_ASSERT(Condition, ApiId)
#define FRNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRNM_STATIC_ASSERT)
# error FRNM_STATIC_ASSERT is already defined
#endif
#if (FRNM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FRNM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FRNM_STATIC_ASSERT(expr)
#endif

#if (defined FRNM_UNREACHABLE_CODE_ASSERT)
#error FRNM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FRNM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FRNM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FRNM_MODULE_ID, FRNM_INSTANCE_ID, (ApiId))
#else
#define FRNM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FRNM_INTERNAL_API_ID)
#error FRNM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FRNM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[module internal macros]-----------------------------*/

/* Macro for better readability since event emitting is done at many places */
#if (defined FRNM_EMIT)
#error FRNM_EMIT already defined
#endif
#define FRNM_EMIT(a,b)                                 \
        FRNM_HSMEMITINST(&FrNm_HsmScFrNm, a, b)

#if (defined INCREMENT_CYCLE)
#error INCREMENT_CYCLE already defined
#endif
#define INCREMENT_CYCLE(value) \
do\
{ \
  if ((value) == UINT8_C(63)) \
  { \
    (value) = UINT8_C(0); \
  } \
  else \
  { \
    (value) = (value) + 1; \
  } \
}\
while(0)

/*==================[type definitions]===================================*/

/*==================[external function declarations]=====================*/

/*==================[internal function declarations]=====================*/

#define FRNM_START_SEC_CODE
/* !LINKSTO FRNM066_MemMap,1 */
#include <FrNm_MemMap.h>

#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
/** \brief Process Car wakeup functionality
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleCarWakeup(
  FRNM_PDL_SF(PduIdType instIdx));
#endif

STATIC FUNC(void, FRNM_CODE) FrNm_HandleTimers
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
);

#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
STATIC FUNC(void, FRNM_CODE) FrNm_HandleControlBitVector
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
);
#endif

STATIC FUNC(void, FRNM_CODE) FrNm_CheckRepetitionCycle
(
  uint8 instIdx
);

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
STATIC FUNC(void, FRNM_CODE) FrNm_CopyPduData
(
  uint8 instIdx,
  boolean isTxPduPtr,
  P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
);
#endif

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_ON) || \
     (FRNM_DEV_ERROR_DETECT == STD_ON))
STATIC FUNC(uint8, FRNM_CODE) FrNm_GetClusterIndex
(
  NetworkHandleType NetworkHandle
);
#endif


#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
STATIC FUNC(uint8, FRNM_CODE) FrNm_GetPduIndex
(
  PduIdType PduId
);
#endif

/** \brief Handle received vote PDU.
 ** \param instIdx ClusterId
 ** \param SduDataPtr Received Nm message
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxVotePdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
);

/** \brief Handle received data PDU.
 ** \param instIdx ClusterId
 ** \param SduDataPtr Received Nm message
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxDataPdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
);

/** \brief Handle received vote/data PDU.
 ** \param instIdx ClusterId
 ** \param SduDataPtr Received Nm message
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxMixedPdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
);

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON)

/** \brief Aggregate the EIRA/ERA
 **
 ** This function aggregate and filter(based on FrNm_PnFilterMask) the recieved requests
 **
 ** \param[in] PnInfo Pointer to the EIRA/ERA structure member where the unfiltered
 **            requests are stored
 ** \param[in] pPnStatus Pointer to EIRA/ERA structure member where the filtered PN requests
 **            will be aggregated
 */
STATIC FUNC(boolean, FRNM_CODE) FrNm_AggregatePnInfo
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) PnInfo
);

/** \brief Handles the EIRA/ERA accumulation
 **
 ** This function accumulate requests in the EIRA/ERA vector. If a change occur the timer is
 ** reseted and the PduR is notified via PduR_FrNmRxIndication callback.
 **
 ** \param[in] PnStatePtr Pointer to EIRA/ERA structure
 ** \param[in] PduId Id of the PDU used in FrNm_RxIndication callback
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandlePnInfo
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
);

#endif

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[external constants]=================================*/

/*==================[internal constants]=================================*/

/*==================[external data]======================================*/

#define FRNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

VAR(FrNm_ChanStatusType, FRNM_VAR) FrNm_ChanStatus[FRNM_NUMBER_OF_CHANNELS];

P2CONST(FrNm_ConfigType,FRNM_VAR,FRNM_APPL_CONST) FrNm_RootPtr;

/** \brief Partial netoworking info: EiraValue and EiraTimer */
#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
VAR(FrNm_PnStatusType, FRNM_VAR) FrNm_PnEiraStatus;
#endif

/** \brief Function pointer to routine schedualed from channel main function */
VAR(FrNm_DoActionType, FRNM_VAR) FrNm_DoActFkp[FRNM_NUMBER_OF_CHANNELS];

#define FRNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[internal data]======================================*/

#define FRNM_START_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

/** \brief Intialization information of FrNm module
 */
/* !LINKSTO FRNM071,1 */
VAR(uint8, FRNM_VAR) FrNm_InitStatus = FRNM_UNINIT;

#define FRNM_STOP_SEC_VAR_INIT_8
#include <FrNm_MemMap.h>

/*==================[external functions definitions]=====================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/*------------------[FrNm_IsValidConfig]---------------------------------------------*/

/* !LINKSTO FrNm.Impl.IsValid,1 */
FUNC(Std_ReturnType, FRNM_CODE) FrNm_IsValidConfig
(
  P2CONST(void, AUTOMATIC, FRNM_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(FrNm_ConfigType,AUTOMATIC,FRNM_APPL_CONST) ConfigPtr = voidConfigPtr;

  if (ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
    {
      /* Validate the post build configuration against the compile time configuration */
      if (FRNM_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if (FrNm_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
          Published information in compile time configuration*/
          if(FRNM_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  return RetVal;
}

/*------------------[FrNm_Init]------------------------------------------*/

FUNC(void, FRNM_CODE) FrNm_Init
(
  CONSTP2CONST(FrNm_ConfigType, AUTOMATIC, FRNM_APPL_CONST) nmConfigPtr
)
{
  uint8 instIdx;

  /* !LINKSTO FRNM059,1 */

  P2CONST(FrNm_ConfigType,AUTOMATIC,FRNM_APPL_CONST) LocalConfigPtr = nmConfigPtr;

  DBG_FRNM_INIT_ENTRY(nmConfigPtr);

  /* !LINKSTO FrNm.EB.Implicit.Uninit.State,1 */
  /* Deviation TASKING-1 */
  for(instIdx=0U; instIdx<FRNM_NUMBER_OF_CHANNELS; ++instIdx)
  {
      FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_UNINIT;
  }

    /* Save the configuration pointer */
#if (FRNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
    /* If the initialization function is called with a null pointer get the configuration from the
     * post build configuration manager */
  if (LocalConfigPtr == NULL_PTR)
  {
      /* !LINKSTO FrNm.Impl.Init,1 */
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      if (E_OK == PbcfgM_GetConfig(
            FRNM_MODULE_ID,
            FRNM_INSTANCE_ID,
            &ModuleConfig))
      {
          LocalConfigPtr = (P2CONST(FrNm_ConfigType, AUTOMATIC, FRNM_APPL_CONST)) ModuleConfig;
      }
  }
#endif /* FRNM_PBCFGM_SUPPORT_ENABLED == STD_OFF */

/* check if development error detection is enabled */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)

  /* check whether parameter LocalConfigPtr is a NULL_PTR */
  if(LocalConfigPtr == NULL_PTR)
  {
      /* found a NULL_PTR, report to DET */
      /* !LINKSTO FRNM395,1 */
      (void)Det_ReportError(FRNM_MODULE_ID,
                            FRNM_INSTANCE_ID,
                            FRNM_SERVID_INIT,
                            FRNM_E_INVALID_POINTER
                           );
  }
  /* check that configuration pointer is valid */
  else if ( E_OK != FrNm_IsValidConfig(LocalConfigPtr))
  {
      (void)Det_ReportError(FRNM_MODULE_ID,
                            FRNM_INSTANCE_ID,
                            FRNM_SERVID_INIT,
                            FRNM_E_INIT_FAILED
                           );
  }
  else
  if(FRNM_PB_REQUIRED_CFG_RAM_SIZE > FRNM_DATA_MEM_SIZE)
  {
    /* !LINKSTO FrNm.EB.PostBuildRamSize_2,1 */
      (void)Det_ReportError(FRNM_MODULE_ID,
                            FRNM_INSTANCE_ID,
                            FRNM_SERVID_INIT,
                            FRNM_E_INIT_FAILED
                           );
  }
  else
#else
  /* !LINKSTO FrNm.EB.ComM_Init.NULL_PTR,1 */
  if(FrNm_IsValidConfig(LocalConfigPtr) == E_OK)
#endif
  {
    /* Initialize module */

    /* Set the pointer to the post-build configuration */
    FrNm_RootPtr = LocalConfigPtr;

    /* Pre Compile Initializations */
    /* Deviation TASKING-1 */
    for (instIdx = 0U; instIdx < FRNM_NUMBER_OF_CHANNELS; ++instIdx)
    {
      FRNM_CHANNEL_STATUS(instIdx).RxPduPtr =
        &FrNm_Rx_Tx_Buffer[FRNM_CHANNEL_BUFFER_OFFSET(instIdx)];
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
      FRNM_CHANNEL_STATUS(instIdx).TxPduPtr =
        &FrNm_Rx_Tx_Buffer[FRNM_TX_BUFFER_OFFSET + FRNM_CHANNEL_BUFFER_OFFSET(instIdx)];
#else
      FRNM_CHANNEL_STATUS(instIdx).TxPduPtr = NULL_PTR;
#endif
      FRNM_CHANNEL_STATUS(instIdx).CurState = NM_STATE_BUS_SLEEP;
      FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
      FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;
      FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = 0U;
      FRNM_CHANNEL_STATUS(instIdx).ActCycle = 0U;
      FRNM_CHANNEL_STATUS(instIdx).ChanStatus  = 0U;
      FRNM_CHANNEL_STATUS(instIdx).ComEnabled = TRUE;

#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
      /* !LINKSTO FRNM432,1 */
      TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnTimer, 0U, (FRNM_EIRA_MAX_TIMER_SIZE * sizeof(FrNm_TimeType)));
      TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnValue, 0U, FRNM_PN_INFO_LENGTH);
      TS_MemSet(FRNM_CHANNEL_STATUS(instIdx).PnEra.PnInfo, 0U, FRNM_PN_INFO_LENGTH);
#endif
      FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss = FALSE;
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
      FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounterValid = FALSE;
      FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounter = 0U;
#endif
      FrNm_DoActFkp[instIdx] = NULL_PTR;
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = 0U;
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
#endif
    }

    /* FRNM307 */

    /* Init of channel specific data structures: */
    if (FrNm_InitStatus == FRNM_INIT)
    {
      /* Deviation TASKING-1 */
      for (instIdx = 0U; instIdx < FRNM_NUMBER_OF_CHANNELS; instIdx++)
      {
        /* Emit event to indicate reinitialisation and to enter into bus sleep state */
        FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE);
      }
    }
    else
    {
      /* Initialize FrNm state machine, perform initial transitions */
      FrNm_HsmInit(&FrNm_HsmScFrNm);
    }

    /* Init of non channel specific data structures (valid for all scenarios): */
#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
    /* !LINKSTO FRNM421,1 */
    TS_MemSet(FrNm_PnEiraStatus.PnValue, 0x00U, FRNM_PN_INFO_LENGTH);
    TS_MemSet(FrNm_PnEiraStatus.PnInfo, 0x00U, FRNM_PN_INFO_LENGTH);
    TS_MemSet(FrNm_PnEiraStatus.PnTimer, 0x00U, (FRNM_EIRA_MAX_TIMER_SIZE * sizeof(FrNm_TimeType)));
#endif

    /* FRNM073 */
    FrNm_InitStatus = FRNM_INIT;
  }
  DBG_FRNM_INIT_EXIT(nmConfigPtr);
}


FUNC(void, FRNM_CODE) FrNm_MainFunction
(
  uint8 instIdx
)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* Do not execute main function until module is initialized, this may be the
   * case during ECU startup. */

  if (FrNm_InitStatus == FRNM_UNINIT)
  {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                          (FRNM_MAINFUNCTION_ID + FRNM_INST(instIdx)),
                          FRNM_E_UNINIT
                         );
#endif /* FRNM_DEV_ERROR_DETECT == STD_ON */
  }
  else
  {
    uint16 FlexRayTickDummy;
    Std_ReturnType FlexRayStatus;

#if (FRNM_HW_VOTE_ENABLE == STD_ON)
    uint8 NmVector;
    uint8 NmChannelProperty;
#endif

#if (FRNM_HW_VOTE_ENABLE == STD_ON)

    /* Note from SWS: If the NM-Vote is transmitted in the static segment of the
     * FlexRay schedule (alternative 1, 3, 4 and 7), the usage of the HW
     * NM-Vector support is possible.
     * Schedule variant 3 is not supported and hence need not be checked here. */

    NmChannelProperty = FRNM_CHANNEL_PROPERTY(instIdx);

    if (((NmChannelProperty & FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_1) ||
        (((NmChannelProperty & FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_4) ||
        ((NmChannelProperty & FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_7))
       )
    {

      if (FrIf_GetNmVector(FRNM_CHANNEL_CTRLIDX(instIdx), &NmVector) == E_OK)
      {
        /* Handle the vote bit */
        if ((NmVector & UINT8_C(0x80)) != 0U)
        {
          /* Emit event to restart remote sleep timer if module is in ready
           * sleep state. */
          FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_VOTE_RECEIVED);
        }

#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
        if (FRNM_CHANNELID_CONFIG(instIdx).FrNmNodeDetectionEnabled == TRUE)
        {
          if (((NmChannelProperty & FRNM_CONTROL_BIT_VECTOR) == FRNM_CONTROL_BIT_VECTOR) &&
              ((NmChannelProperty & FRNM_REPEAT_MESSAGE_BIT) == FRNM_REPEAT_MESSAGE_BIT)
             )
          {
            /* Handle the repeat message bit since the received vote is for the
             * channel having schedule variant 7. */
            if ((NmVector & FRNM_PDU_REPEAT_MSG_REQUESTED) != 0U)
            {
              /* Set channel status with request info so as to start
               * repeat message transmission in the next repetition cycle. */
              TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REPEAT_MSG_REQUEST_BIT);
            }
          }
        }
#endif
      }
    }
#endif /* FRNM_HW_VOTE_ENABLE */

    /* Handle timers */
    FrNm_HandleTimers(FRNM_PL_SF(instIdx));

    /* Get the module cycle time */
    FlexRayStatus = FrIf_GetGlobalTime(FRNM_CHANNEL_CTRLIDX(instIdx),
                                       &FRNM_CHANNEL_STATUS(instIdx).ActCycle,
                                       &FlexRayTickDummy);

    /* !LINKSTO FrNm.EB.EmulatedCounter_1,1 */
    /* !LINKSTO FrNm.EB.EmulatedCounter_2,1 */
    /* !LINKSTO FrNm.EB.EmulatedCounter_3,1 */
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
    if(FlexRayStatus == E_OK)
    {
      FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounterValid = TRUE;
      FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounter = FRNM_CHANNEL_STATUS(instIdx).ActCycle;
    }
    else
    {
      INCREMENT_CYCLE(FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounter);
      FRNM_CHANNEL_STATUS(instIdx).ActCycle = FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounter;
    }
#endif

#if (FRNM_SYNCH_ERR_EXTENDED == STD_ON)
#if (FRNM_MAIN_ACROSS_FR_CYCLE == STD_OFF)
#if (FRNM_CYCLE_COUNTER_EMULATION == STD_OFF)
    if(FlexRayStatus == E_OK)
#else
    if(FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounterValid == TRUE)
#endif
    {
      /* This FrNm_MainFunction is scheduled before the cycle start.
       * i.e transmission of the first vote shall take place only in the next
       *  cycle. Hence increment cycle count to evaluate FrNm channel timings. */
      INCREMENT_CYCLE(FRNM_CHANNEL_STATUS(instIdx).ActCycle);
    }
#endif
#else /* FRNM_SYNCH_ERR_EXTENDED == STD_OFF */
#if (FRNM_MAIN_ACROSS_FR_CYCLE == STD_OFF)
    if(FlexRayStatus == E_OK)
    {
      /* This FrNm_MainFunction is scheduled before the cycle start.
       * i.e transmission of the first vote shall take place only in the next
       *  cycle. Hence increment cycle count to evaluate FrNm channel timings. */
      INCREMENT_CYCLE(FRNM_CHANNEL_STATUS(instIdx).ActCycle);
    }
#endif
#endif
    /* Handle problems getting the actual cycle */
    if (FlexRayStatus != E_OK)
    {
      /* Obviously the synchronization is lost. */
      FrNm_HandleSynchError(instIdx, FALSE);

      FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss = TRUE;
#if ((FRNM_CYCLE_COUNTER_EMULATION == STD_ON) && (FRNM_SYNCH_ERR_EXTENDED == STD_ON))
      /* !LINKSTO SWS_FrNm_00378,1, SWS_FrNm_00379,1 */
      if (FRNM_CHANNEL_STATUS(instIdx).FrNm_EmulatedCounterValid == TRUE)
      {
        FrNm_CheckRepetitionCycle(instIdx);
      }
#endif
    }
    else
    {
      FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss = FALSE;

      FrNm_CheckRepetitionCycle(instIdx);

      if (0U == (FRNM_CHANNEL_STATUS(instIdx).ActCycle %
                 FRNM_CHANNELTIME_CONFIG(instIdx).VotingCycle
                )
         )
      {
          /* Emit event to repeat transmission of vote  */
          FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_VOTE_CYCLE_TIMESLOT);
      }

      if (0U == (FRNM_CHANNEL_STATUS(instIdx).ActCycle %
                 FRNM_CHANNELTIME_CONFIG(instIdx).DataCycle
                )
         )
      {
        /* Emit event to repeat transmission of data */
        FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_DATA_CYCLE_TIMESLOT);
      }
    }

    /* Perform state transitions and actions according to the events queued */
    while (TRUE == FRNM_HSMMAININST(&FrNm_HsmScFrNm, instIdx))
    {
      /* Execute state machine as long as there is a transition in the last
       * state machine function call. */
    }

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
    FrNm_HandlePnInfo(&FrNm_PnEiraStatus, FRNM_PN_EIRA_PDUID);
#endif

#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
    if (FRNM_CHANNELID_CONFIG(instIdx).EraCalcEnabled == STD_ON)
    {
      /* Process ERA for all channels*/
      FrNm_HandlePnInfo(&FRNM_CHANNEL_STATUS(instIdx).PnEra,
                        FRNM_CHANNELID_CONFIG(instIdx).EraPduId
                       );
      FrNm_HandlePnTimers(&FRNM_CHANNEL_STATUS(instIdx).PnEra,
                          FRNM_CHANNELID_CONFIG(instIdx).EraPduId
                         );
    }
    else
    {
      /* added for C coverage */
    }
#endif
  }
  return;
}

/*------------------[FrNm_PassiveStartUp]--------------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_PassiveStartUp
(
   NetworkHandleType NetworkHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_PASSIVESTARTUP_ENTRY(NetworkHandle);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_PASSIVESTARTUP, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_PASSIVESTARTUP, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {

    if (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_BUS_SLEEP)
    {
      SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      /* Set Passive Start status flag */
      FRNM_CHANNEL_STATUS(instIdx).ChanStatus |= (uint8)FRNM_NETWORK_PASSIVE_START;
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
      if (FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == 0U)
      {
        FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = FRNM_PASSIVE_STARTUP_AWB;
      }
      else
      {
        FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = FRNM_PASSIVE_STARTUP_AWB;
      }
#endif
      SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

      /* FRNM138, FRNM317, Emit event for transition to Synchronise mode */
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_NETWORK_START);
      /* FRNM237 */
      retVal = E_OK;
    }
    else
    {
      /* FRNM260: */
      retVal = E_NOT_OK;
    }
  }

  DBG_FRNM_PASSIVESTARTUP_EXIT(retVal,NetworkHandle);
  return retVal;
}

/*------------------[FrNm_NetworkRequest]--------------------------------*/

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkRequest
(
   NetworkHandleType NetworkHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_NETWORKREQUEST_ENTRY(NetworkHandle);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKREQUEST, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKREQUEST, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    /* Set Network Requested status flag */
    FRNM_CHANNEL_STATUS(instIdx).ChanStatus |= (uint8)FRNM_NETWORK_REQUESTED;
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if (FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == 0U)
    {
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = FRNM_NETWORK_REQUEST_AWB;
    }
#endif
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    if (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_BUS_SLEEP)
    {
      /* FRNM316, Emit event for transition to Synchronise mode */
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_NETWORK_START);
    }

    retVal = E_OK;
  }

  DBG_FRNM_NETWORKREQUEST_EXIT(retVal,NetworkHandle);
  return retVal;
}

/*------------------[FrNm_NetworkGwEraRequest]--------------------------------*/
/* !LINKSTO FrNm.EB.FrNm_NetworkGwEraRequest,1 */
FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkGwEraRequest
(
   NetworkHandleType NetworkHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_NETWORKGWERAREQUEST_ENTRY(NetworkHandle);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKGWERAREQUEST, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKGWERAREQUEST, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    /* Set Network Requested status flag */
    /* !LINKSTO FrNm.EB.FrNm_NetworkGwEraRequest.NetworkRequest,1 */
    FRNM_CHANNEL_STATUS(instIdx).ChanStatus |=  (uint8)FRNM_NETWORK_REQUESTED;
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if (FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == 0U)
    {
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = FRNM_NETWORK_GW_ERA_REQUEST_AWB;
    }
#endif
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    if (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_BUS_SLEEP)
    {
      /* FRNM316, Emit event for transition to Synchronise mode */
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_NETWORK_START);
    }

    retVal = E_OK;
  }

  DBG_FRNM_NETWORKGWERAREQUEST_EXIT(retVal,NetworkHandle);
  return retVal;
}

/*------------------[FrNm_NetworkRelease]--------------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkRelease
(
   NetworkHandleType NetworkHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_NETWORKRELEASE_ENTRY(NetworkHandle);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKRELEASE, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_NETWORKRELEASE, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    /* Clear Network Requested status flag */
    FRNM_CHANNEL_STATUS(instIdx).ChanStatus &= (~FRNM_NETWORK_REQUESTED);
#if (FRNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if ((FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == FRNM_NETWORK_REQUEST_AWB) ||
        (FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 == FRNM_NETWORK_GW_ERA_REQUEST_AWB))
    {
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp1 = FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2;
      FRNM_CHANNEL_STATUS(instIdx).ActiveWakeUp2 = 0U;
    }
#endif
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    retVal = E_OK;
  }


  DBG_FRNM_NETWORKRELEASE_EXIT(retVal,NetworkHandle);
  return retVal;
}
#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */

/*------------------[FrNm_SetUserData]-----------------------------------*/

#if ((FRNM_USER_DATA_ENABLED == STD_ON)   \
  && (FRNM_COM_USER_DATA_SUPPORT == STD_OFF))
FUNC(Std_ReturnType, FRNM_CODE) FrNm_SetUserData
(
   NetworkHandleType NetworkHandle,
   P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) nmUserDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_SETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_SETUSERDATA, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_SETUSERDATA, FRNM_E_INVALID_CHANNEL);
  }
  /* Check if the channel has a pdu configured to send data (FrNmTxPduContainsData = True) */
  else if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_NM_DATA_ENABLED) !=
            FRNM_NM_DATA_ENABLED)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_SETUSERDATA, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmUserDataPtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_SETUSERDATA, FRNM_E_INVALID_POINTER);
  }
  else if (FRNM_CHANNEL_PDULENGTH(instIdx) <= FRNM_RESERVED_BYTES(instIdx))
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_SETUSERDATA, FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    /* Protect data buffer from being read for the transmission */
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    /* Reserved bytes in the data buffer are being filled just before actual data transmission and
     * not at this place.
     */
    TS_MemCpy(
      &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)], nmUserDataPtr,
      (PduLengthType)(FRNM_CHANNEL_PDULENGTH(instIdx) - FRNM_RESERVED_BYTES(instIdx)));
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

  DBG_FRNM_SETUSERDATA_EXIT(retVal,NetworkHandle,nmUserDataPtr);
  return retVal;
}
#endif /* (FRNM_USER_DATA_ENABLED == STD_ON) && .. */

/*------------------[FrNm_GetUserData]-----------------------------------*/
#if (FRNM_USER_DATA_ENABLED == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetUserData
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmUserDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && \
     (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_GETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETUSERDATA, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETUSERDATA, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmUserDataPtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETUSERDATA, FRNM_E_INVALID_POINTER);
  }
  else if (FRNM_CHANNEL_PDULENGTH(instIdx) <= FRNM_RESERVED_BYTES(instIdx))
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETUSERDATA, FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    /* Copy user data */
    TS_MemCpy(
      nmUserDataPtr,
      &FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[FRNM_RESERVED_BYTES(instIdx)],
      (PduLengthType)(FRNM_CHANNEL_PDULENGTH(instIdx) - FRNM_RESERVED_BYTES(instIdx)));

    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    retVal = E_OK;
  }

  DBG_FRNM_GETUSERDATA_EXIT(retVal,NetworkHandle,nmUserDataPtr);
  return retVal;
}

#endif /* (FRNM_USER_DATA_ENABLED == STD_ON) */


/*------------------[FrNm_GetPduData]------------------------------------*/

#if ((FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON)   \
   || (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON) \
   || (FRNM_USER_DATA_ENABLED == STD_ON))
FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetPduData
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmPduData
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_GETPDUDATA_ENTRY(NetworkHandle,nmPduData);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETPDUDATA, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETPDUDATA, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmPduData == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETPDUDATA, FRNM_E_INVALID_POINTER);
  }
#if (FRNM_USER_DATA_ENABLED == STD_OFF)
  /* In case FRNM_CONTROL_BIT_VECTOR_ENABLED is STD_OFF
   * FRNM_SOURCE_NODE_IDENTIFIER_ENABLED cannot be STD_ON
   * In case FRNM_CONTROL_BIT_VECTOR_ENABLED is STD_ON
   * FRNM_SOURCE_NODE_IDENTIFIER_ENABLED can be STD_ON or STD_OFF
   * In case FRNM_USER_DATA_ENABLED is STD_ON
   * parameter FrNmSourceNodeIdentifierEnabled should be not checked
   */
  else if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == FALSE)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETPDUDATA, FRNM_E_INVALID_FUNCTION_ARG);
  }
#endif
  else
#endif
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    /* Copy the whole NM PDU data */
    TS_MemCpy(
      nmPduData,
      FRNM_CHANNEL_STATUS(instIdx).RxPduPtr,
      FRNM_CHANNEL_PDULENGTH(instIdx));

    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    retVal = E_OK;
  }

  DBG_FRNM_GETPDUDATA_EXIT(retVal,NetworkHandle,nmPduData);
  return retVal;
}
#endif /* ((FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON)   \
        *|| (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON) ||   \
        *|| (FRNM_USER_DATA_ENABLED == STD_ON)) */

/*------------------[FrNm_RepeatMessageRequest]--------------------------*/

#if (FRNM_NODE_DETECTION_ENABLED == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_RepeatMessageRequest
(
   NetworkHandleType NetworkHandle
)
{
  /* Implicit requirement : If not in network mode, this function returns E_NOT_OK */
  Std_ReturnType retVal = E_NOT_OK;
  Nm_StateType CurState;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_REPEATMESSAGEREQUEST_ENTRY(NetworkHandle);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_REPEATMESSAGEREQUEST, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_REPEATMESSAGEREQUEST, FRNM_E_INVALID_CHANNEL);
  }
  else if(FRNM_CHANNELID_CONFIG(instIdx).FrNmNodeDetectionEnabled == FALSE)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_REPEATMESSAGEREQUEST, FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    CurState = FRNM_CHANNEL_STATUS(instIdx).CurState;
    /* Repeat message request is accepted only in Network mode */
    if ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP) ||
        (CurState == NM_STATE_REPEAT_MESSAGE)
       )
    {
      /* FRNM111, FRNM226 */
      /* Set Repeat Message Requested flag */
      TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REPEAT_MSG_REQUEST_BIT);

      /* Set repeat message bit in the transmit buffer */
#if (FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON)
      if (((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U) &&
          ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_REPEAT_MESSAGE_BIT) != 0U)
         )
      {
        /* Make sure that transmission not happens while setting bit */
        /* Set repeat message bit */
        TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_PDU_REPEAT_MSG_BIT);
      }
#endif
      retVal = E_OK;
    }
  }

  DBG_FRNM_REPEATMESSAGEREQUEST_EXIT(retVal,NetworkHandle);
  return retVal;
}
#endif /* (FRNM_NODE_DETECTION_ENABLED == STD_ON) */

/*------------------[FrNm_GetNodeIdentifier]-----------------------------*/

#if (FRNM_NODE_DETECTION_ENABLED == STD_ON) && (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetNodeIdentifier
(
  NetworkHandleType NetworkHandle,
  P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmNodeIdPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_GETNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr);

  /* Perform checks, get index */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETNODEIDENTIFIER, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETNODEIDENTIFIER, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmNodeIdPtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETNODEIDENTIFIER, FRNM_E_INVALID_POINTER);
  }
  else if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == FALSE)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETNODEIDENTIFIER, FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    /* Copy Node Identifier */
    *nmNodeIdPtr = FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[FRNM_PDU_BYTE_1];
    retVal = E_OK;
  }

  DBG_FRNM_GETNODEIDENTIFIER_EXIT(retVal,NetworkHandle,nmNodeIdPtr);
  return retVal;
}

/*------------------------[FrNm_GetLocalNodeIdentifier]---------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetLocalNodeIdentifier
(
  NetworkHandleType NetworkHandle,
  P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmNodeIdPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_GETLOCALNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETLOCALNODEIDENTIFIER, FRNM_E_UNINIT);
  }

  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_GETLOCALNODEIDENTIFIER,
      FRNM_E_INVALID_CHANNEL);
  }

  else if (nmNodeIdPtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx,FRNM_SERVID_GETLOCALNODEIDENTIFIER,
      FRNM_E_INVALID_POINTER);
  }
  else if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == FALSE)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETLOCALNODEIDENTIFIER, FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    /* Copy Node Id */
    *nmNodeIdPtr = FRNM_CHANNEL_NODEID(instIdx);
    retVal = E_OK;
  }

  DBG_FRNM_GETLOCALNODEIDENTIFIER_EXIT(retVal,NetworkHandle,nmNodeIdPtr);
  return retVal;
}
#endif /*(FRNM_NODE_DETECTION_ENABLED == STD_ON)&&(FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON) */

/*------------------[FrNm_RequestBusSynchronization]---------------------*/

#if (FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_RequestBusSynchronization
(
   NetworkHandleType NetworkHandle
)
{
  /* FRNM174: The FrNm module shall support the function FrNm_RequestBusSynchronization
   * by returning E_OK without executing any functionality.
   */
  Std_ReturnType retVal = E_NOT_OK;
  /* Though, this function has no functionality, DET checks are performed */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#else
  TS_PARAM_UNUSED(NetworkHandle);
#endif

  DBG_FRNM_REQUESTBUSSYNCHRONIZATION_ENTRY(NetworkHandle);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_REQUESTBUSSYNCHRONIZATION, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_REQUESTBUSSYNCHRONIZATION,
      FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    /* Do nothing, FRNM174 */
    retVal = E_OK;
  }


  DBG_FRNM_REQUESTBUSSYNCHRONIZATION_EXIT(retVal,NetworkHandle);
  return retVal;
}
#endif /* (FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) */


/*------------------[FrNm_CheckRemoteSleepIndication]--------------------*/

#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_CheckRemoteSleepIndication
(
   NetworkHandleType NetworkHandle,
   P2VAR(boolean, AUTOMATIC, FRNM_APPL_DATA) nmRemoteSleepIndPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
  Nm_StateType CurState;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_CHECKREMOTESLEEPINDICATION_ENTRY(NetworkHandle,nmRemoteSleepIndPtr);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_CHECKREMOTESLEEPINDICATION, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_CHECKREMOTESLEEPINDICATION, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmRemoteSleepIndPtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_CHECKREMOTESLEEPINDICATION, FRNM_E_INVALID_POINTER);
  }
  else
#endif
  {
    /* It is always the case that remote sleep indication bit is cleared when FrNm is in Repeat
     * Message State.Though the below check includes Repeat Message State in order to check FRNM186
     */
    CurState = FRNM_CHANNEL_STATUS(instIdx).CurState;

    if ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP) ||
        (CurState == NM_STATE_REPEAT_MESSAGE)
       )
    {
      /* FRNM185 */
      if ((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_REMOTE_SLEEP_INDICATION) != 0U)
      {
        *nmRemoteSleepIndPtr = TRUE;
      }
      else
      {
        *nmRemoteSleepIndPtr = FALSE;
      }
      retVal = E_OK;
    }
    else
    {
      /* FRNM186 */
      retVal = E_NOT_OK;
    }
  }

  DBG_FRNM_CHECKREMOTESLEEPINDICATION_EXIT(retVal,NetworkHandle,nmRemoteSleepIndPtr);
  return retVal;
}
#endif /* (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON) */

/*------------------[FrNm_GetState]--------------------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetState
(
   NetworkHandleType NetworkHandle,
   P2VAR(Nm_StateType, AUTOMATIC, FRNM_APPL_DATA) nmStatePtr,
   P2VAR(Nm_ModeType, AUTOMATIC, FRNM_APPL_DATA) nmModePtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_GETSTATE_ENTRY(NetworkHandle,nmStatePtr,nmModePtr);

  /* Perform checks, get index */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETSTATE, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETSTATE, FRNM_E_INVALID_CHANNEL);
  }
  else if (nmStatePtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETSTATE, FRNM_E_INVALID_POINTER);
  }
  else if (nmModePtr == NULL_PTR)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_GETSTATE, FRNM_E_INVALID_POINTER);
  }
  else
#endif
  {
    /* Note from SWS : Consistency between the provided values and the current values
       of the state and mode should be ensured.*/

    /* Above consistency can not be guaranteed by FrNm since the state changes are driven
     * by external events. Even if the remaining code snippet of this function is protected
     * by a critical section, it could still happen a state transition happens before a
     * calling module evaluates these values. Hence, in order to have a consistency of these values
     * the calling module shall consider this aspect and call this function inside critical section.
     */

    *nmStatePtr = FRNM_CHANNEL_STATUS(instIdx).CurState;

    switch (*nmStatePtr)
    {
      case NM_STATE_NORMAL_OPERATION: /* fall through */
      case NM_STATE_REPEAT_MESSAGE: /* fall through */
      case NM_STATE_READY_SLEEP:
        *nmModePtr = NM_MODE_NETWORK;
        retVal = E_OK;
        break;
      case NM_STATE_BUS_SLEEP:
        *nmModePtr = NM_MODE_BUS_SLEEP;
        retVal = E_OK;
        break;
      case NM_STATE_SYNCHRONIZE:
        *nmModePtr = NM_MODE_SYNCHRONIZE;
        retVal = E_OK;
        break;
      default:
        FRNM_UNREACHABLE_CODE_ASSERT(FRNM_SERVID_GETSTATE);
        /* place for defensive programming */
        break;
    }
  }

  DBG_FRNM_GETSTATE_EXIT(retVal,NetworkHandle,nmStatePtr,nmModePtr);
  return retVal;
}

/*------------------[FrNm_GetVersionInfo]--------------------------------*/

#if (FRNM_VERSION_INFO_API == STD_ON)
FUNC(void, FRNM_CODE) FrNm_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, FRNM_APPL_DATA) NmVerInfoPtr
)
{
  DBG_FRNM_GETVERSIONINFO_ENTRY(NmVerInfoPtr);
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
   if (NULL_PTR == NmVerInfoPtr)
   {
     FRNM_DET_REPORT_ERROR(
     FRNM_INSTANCE_ID, FRNM_SERVID_GETVERSIONINFO, FRNM_E_INVALID_POINTER);
   }
   else
#endif
   {
     NmVerInfoPtr->vendorID         = FRNM_VENDOR_ID;
     NmVerInfoPtr->moduleID         = FRNM_MODULE_ID;
     NmVerInfoPtr->sw_major_version = FRNM_SW_MAJOR_VERSION;
     NmVerInfoPtr->sw_minor_version = FRNM_SW_MINOR_VERSION;
     NmVerInfoPtr->sw_patch_version = FRNM_SW_PATCH_VERSION;
   }

   DBG_FRNM_GETVERSIONINFO_EXIT(NmVerInfoPtr);
}
#endif /* (FRNM_VERSION_INFO_API == STD_ON) */

/*------------------[FrNm_StartupError]-----------------------------------*/

FUNC(void, FRNM_CODE) FrNm_StartupError
(
   NetworkHandleType NetworkHandle
)
{
#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(NetworkHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(NetworkHandle);
#endif

  DBG_FRNM_STARTUPERROR_ENTRY(NetworkHandle);

  /* Perform checks, get index */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_STARTUPERROR, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_STARTUPERROR, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss = TRUE;

    /* Emit event to indicate startup error */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_STARTUP_ERROR);

    /* FRNM336: The FlexRay Network Management must react (execute) synchronously on reception
     * of the indication FrNm_StartupError from the FlexRay State Manager even when the FrNM
     * Mainfunction is no longer executing.
     * But we do not perform state transitions and actions here.
     * Rationale: We strongly assume that the FrNm will also be scheduled if the synchronization is
     * lost because normally FrSM and FrNm are scheduled from the same entity and FrSM needs to be
     * scheduled even if synchronization is lost because it is the task of FrSM to organize the
     * re-synchronization of the FR.
     */
  }

  DBG_FRNM_STARTUPERROR_EXIT(NetworkHandle);
}



/*------------------[FrNm_Transmit]--------------------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_Transmit
(
  PduIdType FrNmTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, FRNM_APPL_CONST) PduInfoPtr
)
{
  TS_PARAM_UNUSED(FrNmTxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);

  DBG_FRNM_TRANSMIT_ENTRY(FrNmTxPduId,PduInfoPtr);

  /* FRNM366 : The FrNm implementation shall provide an API FrNm_Transmit (see FRNM359).
   * This API shall never be called by PduR as the FrNm will always query the data by
   * means of PduR_FrNmTriggerTransmit. FrNm_Transmit is an empty function returning
   * E_NOT_OK at any time. This requirement is relevant to avoid linker errors as PduR
   * expects this API to be provided.
   */

  DBG_FRNM_TRANSMIT_EXIT((E_NOT_OK),FrNmTxPduId,PduInfoPtr);
  return (E_NOT_OK);
}


/*------------------[FrNm_EnableCommunication]---------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_EnableCommunication
(
   NetworkHandleType nmChannelHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ((FRNM_DEV_ERROR_DETECT == STD_ON) \
  || ((FRNM_PASSIVE_MODE_ENABLED == STD_OFF) \
  &&  (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)))
  const uint8 instIdx = FrNm_GetClusterIndex(nmChannelHandle);
#else
  TS_PARAM_UNUSED(nmChannelHandle);
#endif

  DBG_FRNM_ENABLECOMMUNICATION_ENTRY(nmChannelHandle);
  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_ENABLECOMMUNICATION, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_ENABLECOMMUNICATION, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
    Nm_StateType CurState;
    /* Enable communication request is accepted only in Network mode with passive mode
     * disabled, FRNM388.
     */

    CurState = FRNM_CHANNEL_STATUS(instIdx).CurState;

    if ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP) ||
        (CurState == NM_STATE_REPEAT_MESSAGE)
       )
    {
      /* Enable transmission. No event being emitted */
      FRNM_CHANNEL_STATUS(instIdx).ComEnabled = TRUE;
      retVal = E_OK;
    }
    else
#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */
    {
      /* return E_NOT_OK if module is not in network mode, FRNM388.
       * or if module operates with passive mode enabled, FRNM389.
       */
      retVal = E_NOT_OK;
    }
  }

  DBG_FRNM_ENABLECOMMUNICATION_EXIT(retVal,nmChannelHandle);
  return retVal;
}



/*------------------[FrNm_DisableCommunication]--------------------------*/

FUNC(Std_ReturnType, FRNM_CODE) FrNm_DisableCommunication
(
   NetworkHandleType nmChannelHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ((FRNM_DEV_ERROR_DETECT == STD_ON) \
  || ((FRNM_PASSIVE_MODE_ENABLED == STD_OFF) \
  && (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)))
  const uint8 instIdx = FrNm_GetClusterIndex(nmChannelHandle);
#else
  TS_PARAM_UNUSED(nmChannelHandle);
#endif

  DBG_FRNM_DISABLECOMMUNICATION_ENTRY(nmChannelHandle);
  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_DISABLECOMMUNICATION, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(instIdx, FRNM_SERVID_DISABLECOMMUNICATION, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
    Nm_StateType CurState;
    /* Disable communication request is accepted only in Network mode with passive mode
     * disabled, FRNM391.
     */
    CurState = FRNM_CHANNEL_STATUS(instIdx).CurState;

    if ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP) ||
        (CurState == NM_STATE_REPEAT_MESSAGE)
       )
    {
      /* Disable transmission. No event being emitted */
      FRNM_CHANNEL_STATUS(instIdx).ComEnabled = FALSE;
      retVal = E_OK;
    }
    else
#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */
    {
      /* return E_NOT_OK if module is not in network mode, FRNM391
       * or if module operates with passive mode enabled, FRNM392
       */
      retVal = E_NOT_OK;
    }
  }

  DBG_FRNM_DISABLECOMMUNICATION_EXIT(retVal,nmChannelHandle);
  return retVal;
}



/*------------------[FrNm_RxIndication]----------------------------------*/

FUNC(void, FRNM_CODE) FrNm_RxIndication
(
   PduIdType RxPduId,
   P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
)
{
#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
  VAR(PduInfoType, FRNM_APPL_DATA) UserPdu;
#endif /* (FRNM_COM_USER_DATA_SUPPORT == STD_ON) */

  DBG_FRNM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr);

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (RxPduId >= FRNM_NUMBER_OF_RX_PDUS)
  {
    /* In case the incoming PduId is invalid, the associated instance can not be
     * determined, therefore instance 0 is reported to DET.
     */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_RXINDICATION, FRNM_E_PDU_ID_INVALID);
  }
  else if (FrNm_InitStatus == FRNM_UNINIT)
  {
    /* If FrNm_Init is not called, the mapping between Rx PduId and the FrIf cluster
     * is unknown at PostBuild time, so instance 0 is reported to DET.
    */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_RXINDICATION,
                           FRNM_E_UNINIT);
  }
  else if (
           (PduInfoPtr == NULL_PTR) ||
           (PduInfoPtr->SduDataPtr == NULL_PTR)
          )
  {
    /* Report FRNM_E_INVALID_POINTER.*/
    FRNM_DET_REPORT_ERROR(FrNm_RootPtr->RxPduMap[RxPduId].ChannelHandle, FRNM_SERVID_RXINDICATION,
                          FRNM_E_INVALID_POINTER);
  }
  else if (PduInfoPtr->SduLength != FRNM_CHANNEL_PDULENGTH(FrNm_RootPtr->RxPduMap[RxPduId].ChannelHandle))
  {
    /* Det error shall be raised in case any of the remote node send data with size different
     * than expected because this would mix up data between different nodes.
     */
    FRNM_DET_REPORT_ERROR(FrNm_RootPtr->RxPduMap[RxPduId].ChannelHandle, FRNM_SERVID_RXINDICATION,
                          FRNM_E_INVALID_FUNCTION_ARG);
  }
  else
#endif
  {
    const uint8 instIdx = FrNm_RootPtr->RxPduMap[RxPduId].ChannelHandle;
#if (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
    if ((PduInfoPtr->SduDataPtr[0U] & FRNM_CBV_SLEEPREADYBIT_MASK) != 0U)
    {
      /* FRNM396 */
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
      FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION(FRNM_INST(instIdx));
#else
    if (FRNM_CALL_NM_COORDREADYTOSLEEPINDICATION(FRNM_INST(instIdx)) != SCHM_E_OK)
    {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
        FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                              FRNM_SERVID_RXINDICATION,
                              FRNM_E_COORDREADYTOSLEEPINDICATION
                              );
#endif
    }
#endif
    }
#endif /* (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON) */
    if (FrNm_RootPtr->RxPduMap[RxPduId].PduType == FRNM_VOTE)
    {
      FrNm_HandleRxVotePdu(instIdx, PduInfoPtr->SduDataPtr);
      /* Emit event to indicate RxIndication to the upper layer if in network mode */
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_RX_INDICATION);
    }
    else
    {
#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
      /* FRNM407 */
      boolean PnMessage = FALSE;
      /* !LINKSTO FrNm.EB.PnEnabled_FALSE_1,1 */
      if (FRNM_CHANNEL_PNENABLED(instIdx) == TRUE)
      {
        SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
        /* FrNm_RxIndication could be called more than once within the interval of 2 main function
         * calls. Therefore, the external requests to be aggregated.
         */
        /* FrNm_AggregatePnInfo shall be called only when CRI bit is set in the received NM-PDU */
        /* !LINKSTO FRNM404,1 */
        if ((PduInfoPtr->SduDataPtr[0U] & FRNM_CBV_PNINFOBIT_MASK) != 0U)
        {
#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
          PnMessage = FrNm_AggregatePnInfo(&FrNm_PnEiraStatus,
                                           &PduInfoPtr->SduDataPtr[FRNM_PN_INFO_OFFSET]
                                          );
#endif
#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
          if (FRNM_CHANNELID_CONFIG(instIdx).EraCalcEnabled == STD_ON)
          {
            /* !LINKSTO FRNM430,1, FRNM431,1, FRNM434,1 */
            PnMessage = FrNm_AggregatePnInfo(&FRNM_CHANNEL_STATUS(instIdx).PnEra,
                                             &PduInfoPtr->SduDataPtr[FRNM_PN_INFO_OFFSET]
                                             );
          }
          else
          {
            /* added for C coverage */
          }
#endif
        }
        SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      }
#endif

#if (FRNM_PN_SUPPORTED == STD_ON)
#if ((FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON))
    if ((PnMessage == TRUE)||
        /* FRNM409 : Partial networking is not supported */
        (FRNM_CHANNEL_PNENABLED(instIdx) == FALSE) ||
        (FRNM_CHANNEL_CONFIG(instIdx).AllNmMessagesKeepAwake == TRUE)
       )
#else
      if ((FRNM_CHANNEL_PNENABLED(instIdx) == FALSE) ||
          (FRNM_CHANNEL_CONFIG(instIdx).AllNmMessagesKeepAwake == TRUE))
#endif
#endif
      {
        if (FrNm_RootPtr->RxPduMap[RxPduId].PduType == FRNM_DATA)
        {
          FrNm_HandleRxDataPdu(instIdx, PduInfoPtr->SduDataPtr);
        }
        if (FrNm_RootPtr->RxPduMap[RxPduId].PduType == FRNM_MIXED)
        {
          FrNm_HandleRxMixedPdu(instIdx, PduInfoPtr->SduDataPtr);
        }
        /* Emit event to indicate RxIndication to the upper layer if in network mode */
        FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_RX_INDICATION);
      }
      SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

      /* Copy the NM bytes from the second byte position since first byte
       * has already been copied.
       */
      TS_MemCpy(
        &FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[1U],
        &PduInfoPtr->SduDataPtr[1U],
        (PduLengthType)(PduInfoPtr->SduLength - 1U));
      SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
      /* Requirement ECUC_FrNm_00411 is satisfied since
       * the calling of the function Nm_CarWakeUpIndication
       * is done after the population of RxPduPtr buffer
       */
      /* !LINKSTO ECUC_FrNm_00411,1 */
      FrNm_HandleCarWakeup(FRNM_PL_SF(instIdx));
#endif
#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
      if (FrNm_ComUserDataInfo[instIdx].ComUserRxPduEnabled == TRUE)
      {
        UserPdu.SduLength = PduInfoPtr->SduLength - FRNM_RESERVED_BYTES(instIdx);
        UserPdu.SduDataPtr = &FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[FRNM_RESERVED_BYTES(instIdx)];

        PduR_FrNmRxIndication(FRNM_CHANNEL_USERRXPDUID(instIdx), &UserPdu);
      }
#endif /* (FRNM_COM_USER_DATA_SUPPORT == STD_ON) */
    }
  }

  DBG_FRNM_RXINDICATION_EXIT(RxPduId,PduInfoPtr);
}

STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxVotePdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* Received Pdu contains the Vote and may be Control Bit Vector,too.
   * The first byte can not be copied to internal buffer if schedule variant is not 7
   * as it might corrupt the Control Bit Vector bit already stored. Therefore, vote bit
   * need to be extracted and stored in the internal buffer if schedule variant is not 7.
   */

  /* Position of vote is last bit in the first byte.*/
  if ((SduDataPtr[0U] & 0x80U) == 0x80U)
  {
    /* Store this bit info in the internal buffer.*/
    FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] |= 0x80U;
  }
  else
  {
    /* Clear this bit info in the internal buffer.*/
    FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] &= 0x7FU;
  }

  /* !LINKSTO FrNm.EB.ScheduleVariant6,1 */
  if (((SduDataPtr[0U] & 0x80U) == 0x80U) ||
      ((FRNM_CHANNEL_PROPERTY(instIdx) &
        FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_6))
  {
    /* Emit event to indicate a vote received to keep the cluster awake */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_VOTE_RECEIVED);
  }

  if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) ==
       FRNM_PDU_SCHEDULE_VARIANT_7)
  {
    /*Schedule variant is 7, hence first byte can be copied, Vote + CBV */
    FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] = SduDataPtr[0U];
#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
    FrNm_HandleControlBitVector(FRNM_PL_SF(instIdx));
#endif
  }
}

STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxDataPdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* Received Pdu contains the Data and may be Control Bit Vector,too.
   * The first byte can not be copied to internal buffer as it might corrupt
   * the vote bit already stored. Also, if the schedule variant is 7,
   * then control bit vector is invalid with this data pdu. Therefore,
   * control bit vector need to be extracted and stored in the internal buffer
   * if schedule variant is not 7.
   */
   FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] &= 0x80U;
   FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] |= SduDataPtr[0U] & 0x7FU;
#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
   FrNm_HandleControlBitVector(FRNM_PL_SF(instIdx));
#endif
}

STATIC FUNC(void, FRNM_CODE) FrNm_HandleRxMixedPdu
(
  uint8 instIdx,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) SduDataPtr
)
{
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* Received Pdu contains Vote, Data and may be Control Bit Vector,too.*/

  /* Refer SWS 7.9 Schedule details, point 2, when both data and vote is transmitted
   * using same Pdu in dynamic segment (schedule variant 2), the presence (or non-presence)
   * of the PDU corresponds to the NM-Vote.
   * Therfore event to be emitted if the received pdu contains +ve vote OR if the pdu belongs
   * to schedule variant 2.
   */
  /* !LINKSTO FrNm.EB.ScheduleVariant2,2 */
  if (((SduDataPtr[0U] & 0x80U) == 0x80U) ||
      ((FRNM_CHANNEL_PROPERTY(instIdx) &
        FRNM_PDU_SCHEDULE_VARIANT_MASK) == FRNM_PDU_SCHEDULE_VARIANT_2)
     )
  {
    /* Emit event to indicate a vote received to keep the cluster awake */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_VOTE_RECEIVED);
  }
  /* First byte can be copied as it won't harm anything since both data and vote
   * belongs to same pdu.
   */
  FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] = SduDataPtr[0U];

  /* Check if repeat message request feature is supported.*/
#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
  FrNm_HandleControlBitVector(FRNM_PL_SF(instIdx));
#endif
}

/*------------------[FrNm_TriggerTransmit]-------------------------------*/
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_TriggerTransmit
(
   PduIdType TxPduId,
   P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_FRNM_TRIGGERTRANSMIT_ENTRY(TxPduId,PduInfoPtr);

  /* Excerpts from AUTOSAR_TPS_ECUConfiguration.pdf V3.2.0 (R4.0 Rev3)
   * section "4.4.1.2 Definition of Handle IDs":
   * Handle IDs are defined by the module providing the API and used by
   * the module calling the API. Handle IDs that are used in
   * callback functions (e.g. Tx Confirmation functions or Trigger
   * Transmit functions) shall be defined by the upper layer module.
   * Hence, TxPduId for 'FrNm_TriggerTransmit' shall be defined by
   * the parameter 'FrNmTxConfirmationPduId'.
   */

  /* Perform DET checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (TxPduId >= FRNM_NUMBER_OF_TX_PDUS)
  {
    /* In case the incoming PduId is invalid, the associated instance can not be
     * determined, therefore instance 0 is reported to DET.
     */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_TRIGGERTRANSMIT, FRNM_E_PDU_ID_INVALID);
  }
  else if (FrNm_InitStatus == FRNM_UNINIT)
  {
    /* If FrNm_Init is not called, the mapping between Tx PduId and the FrIf cluster
     * is unknown at PostBuild time, so instance 0 is reported to DET.
     */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_TRIGGERTRANSMIT,
                           FRNM_E_UNINIT);
  }
  else if (
           (PduInfoPtr == NULL_PTR) ||
           (PduInfoPtr->SduDataPtr == NULL_PTR)
          )
  {
    /* Report FRNM_E_INVALID_POINTER.*/
    FRNM_DET_REPORT_ERROR(FrNm_RootPtr->TxPduMap[TxPduId].ChannelHandle,
      FRNM_SERVID_TRIGGERTRANSMIT, FRNM_E_INVALID_POINTER);
  }
  else
#endif
  {
     const uint8 instIdx = FrNm_RootPtr->TxPduMap[TxPduId].ChannelHandle;
     PduLengthType PduLength;

    if (FrNm_RootPtr->TxPduMap[TxPduId].PduType == FRNM_VOTE)
    {
      /* The Pdu is of type vote and hence only the first byte
       * need to be copied. But the first byte can be copied
       * only if the schedule variant is 7. If the schedule variant
       * is not 7 then the control bit vector (if supported) need to be send
       * with Pdu of type data or pdu of type 'data and vote'.
       */

      if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) !=
           FRNM_PDU_SCHEDULE_VARIANT_7)
      {
        /* Position of vote is last bit in the first byte.*/
        if ((FRNM_CHANNEL_STATUS(instIdx).ControlBitVector & 0x80U) == 0x80U)
        {
          /* Store this bit info in the transmit buffer.*/
          PduInfoPtr->SduDataPtr[0U] = 0x80U;
        }
        else
        {
          /* Clear this bit info in the transmit buffer.*/
          PduInfoPtr->SduDataPtr[0U] = 0U;
        }
      }
      else
      {
        /*schedule variant is 7, hence the first can be completely copied */
        PduInfoPtr->SduDataPtr[0U] = FRNM_CHANNEL_STATUS(instIdx).ControlBitVector;
      }
      PduInfoPtr->SduLength = 1U;
    }
    else
    {
      /* Store data pointer temporarily */
      P2VAR(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) DataPtr = PduInfoPtr->SduDataPtr;
      /* Increment pointer to data byte location to copy data bytes from PduR or from
       * internal transmit buffer.Here pointer address is changing tempororily to
       * avoid additional buffer and mem copy since PduR interface also has PduInfoType pointer.
       */

#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
      if (FrNm_ComUserDataInfo[instIdx].ComUserTxPduEnabled == TRUE)
      {
        /* PduLength in com user data holds only length corresponds to user data */
        PduLength = FRNM_CHANNEL_USERDATAPDULENGTH(instIdx);
      }
      else
#endif
      {
        /* Com user data is not supported for this channel */
        /* PduLength in FrNm_RootPtr->TxPduMap holds length corresponds to user data and FrNm header bytes
         * (Vote,SID) */
        PduLength = FrNm_RootPtr->TxPduMap[TxPduId].PduLength - FRNM_RESERVED_BYTES(instIdx);
      }

      PduInfoPtr->SduDataPtr = &DataPtr[FRNM_RESERVED_BYTES(instIdx)];
      /* PduLength is ensured to be greater than or equal to FRNM_RESERVED_BYTES through Xpath checks */
      PduInfoPtr->SduLength = PduLength;
      /* Copy Pdu Data bytes */
      FrNm_CopyPduData(instIdx, FALSE, PduInfoPtr);

      /* Relocate SduDataPtr and length to store vote/cbv and source node id */
      PduInfoPtr->SduDataPtr = DataPtr;
      PduInfoPtr->SduLength = PduLength + FRNM_RESERVED_BYTES(instIdx);

      if (FrNm_RootPtr->TxPduMap[TxPduId].PduType == FRNM_DATA)
      {
        /* Pdu corresponds to  the Data and may be Control Bit Vector,too.
         * if schedule variant is not 7, then control bit vector also need to be sent with
         * data pdu. The first byte can not be copied from internal buffer as it might also
         * have the vote bit already stored. Therefore, control bit vector need to be
         * extracted and stored in the transmit buffer.
         */
         /* Position of control bit vector is first bit in the first byte.*/
         PduInfoPtr->SduDataPtr[0U] = FRNM_CHANNEL_STATUS(instIdx).ControlBitVector & 0x7FU;
      }
      if (FrNm_RootPtr->TxPduMap[TxPduId].PduType == FRNM_MIXED)
      {
        /* Pdu correspnds to Vote, Data and may be Control Bit Vector,too.
         */
        /* Copy the first byte as it contains the vote and control bit vector info.
         */
        PduInfoPtr->SduDataPtr[0U] = FRNM_CHANNEL_STATUS(instIdx).ControlBitVector;
      }

#if (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
      if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == TRUE)
      {
        PduInfoPtr->SduDataPtr[FRNM_SOURCE_NODE_IDENTIFIER_POS]=FRNM_CHANNEL_NODEID(instIdx);
      }
#endif
    }
    retVal  = E_OK;
  }

  DBG_FRNM_TRIGGERTRANSMIT_EXIT(retVal,TxPduId,PduInfoPtr);
  return retVal;
}
#else /* (FRNM_PASSIVE_MODE_ENABLED == STD_ON) */

FUNC(Std_ReturnType, FRNM_CODE) FrNm_TriggerTransmit
(
   PduIdType TxPduId,
   P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
)
{
  TS_PARAM_UNUSED(TxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  FRNM_DET_REPORT_ERROR(FRNM_INSTANCE_ID, FRNM_SERVID_TRIGGERTRANSMIT, FRNM_E_PDU_ID_INVALID);
#endif
  return E_NOT_OK;
}

#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */
/*------------------[FrNm_TxConfirmation]--------------------------------*/
#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(void, AUTOMATIC) FrNm_TxConfirmation
(
   PduIdType TxPduId
)
{
#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && \
     (FRNM_DEV_ERROR_DETECT == STD_OFF) && \
     (FRNM_COM_USER_DATA_SUPPORT == STD_OFF))
  TS_PARAM_UNUSED(TxPduId);
#endif

  DBG_FRNM_TXCONFIRMATION_ENTRY(TxPduId);

  /* Perform Det checks */
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (TxPduId >= FRNM_NUMBER_OF_TX_PDUS)
  {
    /* In case the incoming PduId is invalid, the associated instance can not be
     * determined, therefore instance 0 is reported to DET.
     */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_TXCONFIRMATION, FRNM_E_PDU_ID_INVALID);
  }
  else if (FrNm_InitStatus == FRNM_UNINIT)
  {
    /* If FrNm_Init is not called, the mapping between Tx PduId and the FrIf cluster
     * is unknown at PostBuild time, so instance 0 is reported to DET.
     */
    FRNM_DET_REPORT_ERROR(0U, FRNM_SERVID_TXCONFIRMATION,
                           FRNM_E_UNINIT);
  }
  else
#endif
  {
#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
    /* If the PDU belongs to NM-Vote type, it was not triggerd by PduR */
    if (FrNm_RootPtr->TxPduMap[TxPduId].PduType != FRNM_VOTE)
    {
      const uint8 instIdx = FrNm_RootPtr->TxPduMap[TxPduId].ChannelHandle;
      if (FrNm_ComUserDataInfo[instIdx].ComUserTxPduEnabled == TRUE)
      {
        /* Forward Tx confirmation with PDU Id defined by PduR */
        PduR_FrNmTxConfirmation(FRNM_CHANNEL_COMUSERTXCONFPDUID(instIdx));
      }
    }
#endif /* (FRNM_COM_USER_DATA_SUPPORT == STD_ON) */
    /* Emit event to reset transmission error detection timer */
    FRNM_EMIT(FrNm_RootPtr->TxPduMap[TxPduId].ChannelHandle, FRNM_HSM_FRNM_EV_TX_CONFIRMATION);
  }

  DBG_FRNM_TXCONFIRMATION_EXIT(TxPduId);
}
#else /* (FRNM_PASSIVE_MODE_ENABLED == STD_ON) */

FUNC(void, AUTOMATIC) FrNm_TxConfirmation
(
   PduIdType TxPduId
)
{
    TS_PARAM_UNUSED(TxPduId);
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
    FRNM_DET_REPORT_ERROR(FRNM_INSTANCE_ID, FRNM_SERVID_TXCONFIRMATION, FRNM_E_PDU_ID_INVALID);
#endif
}

#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */

/*------------------[FrNm_SetSleepReadyBit]--------------------------------*/
#if (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
FUNC(Std_ReturnType, FRNM_CODE) FrNm_SetSleepReadyBit
(
  NetworkHandleType nmChannelHandle,
  boolean nmSleepReadyBit
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (FRNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(nmChannelHandle);
#else
  const uint8 instIdx = FrNm_GetClusterIndex(nmChannelHandle);
#endif

  DBG_FRNM_SETSLEEPREADYBIT_ENTRY(nmChannelHandle,nmSleepReadyBit);

#if (FRNM_DEV_ERROR_DETECT == STD_ON)
  if (FrNm_InitStatus == FRNM_UNINIT)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_SETSLEEPREADYBIT, FRNM_E_UNINIT);
  }
  else if (instIdx >= FRNM_NUMBER_OF_CHANNELS)
  {
    FRNM_DET_REPORT_ERROR(
      instIdx, FRNM_SERVID_SETSLEEPREADYBIT, FRNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
    if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_CONTROL_BIT_VECTOR) != 0U)
    {
      /* FRNM398 */
      if (nmSleepReadyBit == TRUE)
      {
        TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_CBV_SLEEPREADYBIT);
      }
      else
      {
        TS_AtomicClearBit_8(&FRNM_CHANNEL_STATUS(instIdx).ControlBitVector, FRNM_CBV_SLEEPREADYBIT);
      }

      retVal = E_OK;
    }
  }

  DBG_FRNM_SETSLEEPREADYBIT_EXIT(retVal,nmChannelHandle,nmSleepReadyBit);
  return retVal;
}
#endif /* (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON) */

#if ((FRNM_MULTICORE_ENABLED == STD_ON) &&\
     ((FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON) || (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)))
/* CHECK: NOPARSE */
/* No parse have been added because
 * (FrNm_Dummy_RxIndication function has been added with the support for multi core.)
 * when modelling the cross core function calls (Schm_Call) context is required
 * for modelling the calling context.
 * The RxIndication is called from an interrupt context which cannot be modelled
 * in Rte therefore a dummy context has been added.
 * Rte generates a function declaration, here an empty function definitions
 * is provided but never called.
 */
FUNC(void, FRNM_CODE) FrNm_Dummy_RxIndication (void)
{

}
/* CHECK: PARSE */
#endif
/*==================[internal functions definitions]=====================*/

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Handle transmission of Nm vote with or without Nm data.
 ** \param instIdx Index to the channel clusters.
 */
FUNC(void, FRNM_CODE) FrNm_HandleVoteCycle
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
)
{
  PduLengthType PduLength;
  uint8 PduIndex;
  PduInfoType FrNm_PduInfo;
#if (FRNM_HSM_INST_MULTI_ENABLED != STD_ON)
  uint8 instIdx = 0U;
#endif

  /* Vote: Only positive votes are transmitted in the voting cycle.
   * When in ReadySleepState, the vote are cleared and is transmitted
   * through FrNm_TriggerTransmit. That is, there is no explicit
   * transmission for 'negative' vote.
   */
  /* Data: If the pdu schedule variant is 1 or 2, then both data and vote
   * are transmitted in the same pdu. In this case pdus are sent only
   * on voting cycle and not in data cycle.
   */
   if (((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) ==
         FRNM_PDU_SCHEDULE_VARIANT_1) ||
       ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) ==
         FRNM_PDU_SCHEDULE_VARIANT_2)
      )
  {
    /* Assign the data byte offset position to copy data bytes either from
     * Com using PduR or from the internal transmit buffer being filled with
     * FrNm_SetUserData().
     */

#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
      if (FrNm_ComUserDataInfo[instIdx].ComUserTxPduEnabled == TRUE)
      {
        /* PduLength in com user data holds only length corresponds to user data */
        PduLength = FRNM_CHANNEL_USERDATAPDULENGTH(instIdx);
      }
      else
#endif
      {
        /* Com user data is not supported for this channel*/
        /* If dual channels are configured, length of corresponding PDUs must be equal.
         * Therefore, use first TxPduId to derive the PduLength.
         */
        PduIndex = FrNm_GetPduIndex(FRNM_CHANNEL_TXVOTEPDUID(instIdx)[0U]);
        /* PduLength in FrNm_RootPtr->TxPduMap holds length corresponds to user data and FrNm header bytes
         * (Vote,SID) */
        PduLength = FrNm_RootPtr->TxPduMap[PduIndex].PduLength - FRNM_RESERVED_BYTES(instIdx);
      }

    FrNm_PduInfo.SduDataPtr = &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)];
    FrNm_PduInfo.SduLength = PduLength;

    /* Copy Pdu Data bytes */
    FrNm_CopyPduData(instIdx, TRUE, &FrNm_PduInfo);

#if (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
    if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == TRUE)
    {
      FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_SOURCE_NODE_IDENTIFIER_POS] =
        FRNM_CHANNEL_NODEID(instIdx);
    }
#endif

    /* Copy vote and cbv byte*/
    FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[0U] = FRNM_CHANNEL_STATUS(instIdx).ControlBitVector;
    /* SduDataPtr and length need to be relocated before passing to FrIf*/
    FrNm_PduInfo.SduDataPtr = FRNM_CHANNEL_STATUS(instIdx).TxPduPtr;
    FrNm_PduInfo.SduLength = PduLength + FRNM_RESERVED_BYTES(instIdx);
  }
  /* If Schedule variant 7 copy first byte as it contains the control bit vector,too.*/
  else if ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) ==
            FRNM_PDU_SCHEDULE_VARIANT_7)
  {
    FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[0U] = FRNM_CHANNEL_STATUS(instIdx).ControlBitVector;
    FrNm_PduInfo.SduDataPtr = FRNM_CHANNEL_STATUS(instIdx).TxPduPtr;
    FrNm_PduInfo.SduLength = 1U;
  }
  else
  {
    /* Copy only the vote bit, here vote bit can be directly set to 0x80, too. */
    FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[0U] = (FRNM_CHANNEL_STATUS(instIdx).ControlBitVector & 0x80U);

    FrNm_PduInfo.SduDataPtr = FRNM_CHANNEL_STATUS(instIdx).TxPduPtr;
    FrNm_PduInfo.SduLength = 1U;
  }

  /* If there are double channels configured, then more than one vote pdu might be configured
   * for same channel and in this case transmission to be done for each pdu.
   */
#if (FRNM_DUAL_CHANNEL_PDU_ENABLE == STD_ON)
  for(PduIndex = 0U; PduIndex < FRNM_CHANNEL_NOOFTXVOTEPDUS(instIdx); PduIndex++)
  {
    /* The behaviour if transmit function returns NOT_OK is not described in SWS.
     * Though, transmission error is detected by checking whether confirmation is
     * is received or not.
     */
    (void)FrIf_Transmit(FRNM_CHANNEL_TXVOTEPDUID(instIdx)[PduIndex], &FrNm_PduInfo);
  }
#else
  (void)FrIf_Transmit(FRNM_CHANNEL_TXVOTEPDUID(instIdx)[0U], &FrNm_PduInfo);
#endif
}
#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Handle transmission of Nm data.
 ** \param instIdx Index to the channel clusters.
 */
FUNC(void, FRNM_CODE) FrNm_HandleDataCycle
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
)
{
  PduLengthType PduLength;
  uint8 PduIndex;
  PduInfoType FrNm_PduInfo;
#if (FRNM_HSM_INST_MULTI_ENABLED != STD_ON)
  uint8 instIdx = 0U;
#endif

  /* If the pdu schedule variant is 1 or 2, then both data and vote
   * are transmitted only in the vote cycle.
   */
  if (((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) !=
        FRNM_PDU_SCHEDULE_VARIANT_1) &&
      ((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_PDU_SCHEDULE_VARIANT_MASK) !=
        FRNM_PDU_SCHEDULE_VARIANT_2)
      )
  {
    /* Assign the data byte offset position to copy data bytes either from
     * Com using PduR or from the internal transmit buffer being filled with
     * FrNm_SetUserData().
     */
#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)
      if (FrNm_ComUserDataInfo[instIdx].ComUserTxPduEnabled == TRUE)
      {
        /* PduLength in com user data holds only length corresponds to user data */
        PduLength = FRNM_CHANNEL_USERDATAPDULENGTH(instIdx);
      }
      else
#endif
      {
        /* Com user data is not supported for this channel*/
        /* PduLength in FrNm_RootPtr->TxPduMap holds length corresponds to user data and FrNm header bytes
         * (Vote,SID) */
        /* If dual channels are configured, length of corresponding PDUs must be equal.
         * Therefore, use first TxPduId to derive the PduLength.
         */
        PduIndex = FrNm_GetPduIndex(FRNM_CHANNEL_TXDATAPDUID(instIdx)[0U]);
        PduLength = FrNm_RootPtr->TxPduMap[PduIndex].PduLength - FRNM_RESERVED_BYTES(instIdx);
      }
      FrNm_PduInfo.SduDataPtr = &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)];
      FrNm_PduInfo.SduLength = PduLength;

      /* Copy Pdu Data bytes */
      FrNm_CopyPduData(instIdx, TRUE, &FrNm_PduInfo);

#if (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
      if (FRNM_CHANNELID_CONFIG(instIdx).FrNmSourceNodeIdentifierEnabled == TRUE)
      {
        FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_SOURCE_NODE_IDENTIFIER_POS] =
          FRNM_CHANNEL_NODEID(instIdx);
      }
#endif

      /* If pdu schedule variant is not 1 or 2 , then control bit vector
       * to be copied in to the transmit buffer.
       */
      /* clear vote/cbv byte*/
      FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[0U] = (FRNM_CHANNEL_STATUS(instIdx).ControlBitVector & 0x7FU);

      /* SduDataPtr and length need to be relocated before passing to FrIf*/
      FrNm_PduInfo.SduDataPtr = FRNM_CHANNEL_STATUS(instIdx).TxPduPtr;
      FrNm_PduInfo.SduLength = PduLength + FRNM_RESERVED_BYTES(instIdx);

      /* If there are double channels configured, then more than one vote pdu might be configured
       * for same channel and in this case transmission to be done for each pdu.
       */
#if (FRNM_DUAL_CHANNEL_PDU_ENABLE == STD_ON)
      for(PduIndex = 0U; PduIndex < FRNM_CHANNEL_NOOFTXDATAPDUS(instIdx); PduIndex++)
      {
      /* The behaviour if transmit function returns NOT_OK is not described in SWS.
       * Though, transmission error is detected by checking whether confirmation is
       * is received or not.
       */
      (void)FrIf_Transmit(FRNM_CHANNEL_TXDATAPDUID(instIdx)[PduIndex], &FrNm_PduInfo);
    }
#else
    (void)FrIf_Transmit(FRNM_CHANNEL_TXDATAPDUID(instIdx)[0U], &FrNm_PduInfo);
#endif
  }
}
#endif /*(FRNM_PASSIVE_MODE_ENABLED == STD_OFF)*/

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Copy data to the NM transmit buffer.
 ** \param instIdx Index to the channel clusters.
 ** \param isTxPduPtr - is set to true in case PduInfoPtr refers to TxPduPtr
 **                     (FrNm_HandleVoteCycle, FrNm_HandleDataCycle).
 **                     Is called in context of FrNm_Mainfunction
 **                   - is set to false in case PduInfoPtr does not reffer
 **                     to TxPduPtr (FrNm_TriggerTransmit).
 **                     FrNm_TriggerTransmit provides it's own buffer.
 ** \param PduInfoPtr Address of Pdu which contains the transmit buffer and length information.
 */
STATIC FUNC(void, FRNM_CODE) FrNm_CopyPduData
(
  uint8 instIdx,
  boolean isTxPduPtr,
  P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_DATA) PduInfoPtr
)
{
#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_OFF) || (FRNM_COM_USER_DATA_SUPPORT == STD_ON))
TS_PARAM_UNUSED(instIdx);
#endif

#if (FRNM_COM_USER_DATA_SUPPORT == STD_ON)

  if (FrNm_ComUserDataInfo[instIdx].ComUserTxPduEnabled == TRUE)
  {
    if (E_OK == PduR_FrNmTriggerTransmit(FRNM_CHANNEL_COMUSERTXCONFPDUID(instIdx),
                                         PduInfoPtr
                                        )
       )
    {
      if(isTxPduPtr == FALSE)
      {
        /* call of this function initiated by FrIf with it's own Pdu */
        /* Now, data has been copied to the FrNm_PduBuffer or to the buffer
         * provided by FrIf through FrNm_TriggerTransmit().
         * Here, FrNm_PduBuffer or buffer from FrIf is used to receive data
         * from Com instead of directly using TxPduPtr. This is to avoid call of
         * PduR_FrNmTriggerTransmit() within a critical section as TxPduPtr
         * is accessed at more than one place.
         */
        SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
        /* Copy the NM data bytes received from Com to the internal buffer */
        TS_MemCpy(
              &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)],
              &PduInfoPtr->SduDataPtr[0U],
              PduInfoPtr->SduLength);
        SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      }
    }
    else
    {
      if(isTxPduPtr == FALSE)
      {
        /* PduR_FrNmTriggerTransmit returned E_NOT_OK, copy to FrIf Pdu last successful
         * received data stored in TxPduPtr */
        SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
        /* Copy the NM data bytes received from Com to the internal buffer.*/
        TS_MemCpy(
              &PduInfoPtr->SduDataPtr[0U],
              &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)],
              PduInfoPtr->SduLength);
        SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      }
    }
  }
  else
#endif /* (FRNM_COM_USER_DATA_SUPPORT == STD_ON) */
  {
    if(isTxPduPtr == FALSE)
    {
      SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      /* SWS_FrNm_00450 - use the last transmitted value for NmUserData*/
      TS_MemCpy(
           &PduInfoPtr->SduDataPtr[0U],
            &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_RESERVED_BYTES(instIdx)],
            PduInfoPtr->SduLength);
      SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
    }
    else
    {
       /* data already in TxPduPtr */
    }
  }

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
  /* Eira is accessed in main function and RxIndication too, hence protect the copying */
  /* !LINKSTO FrNm.EB.PnEnabled_FALSE_2,1 */
  if (FRNM_CHANNEL_PNENABLED(instIdx) == TRUE)
  {
    SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

    (void)FrNm_AggregatePnInfo(&FrNm_PnEiraStatus,
                               &FRNM_CHANNEL_STATUS(instIdx).TxPduPtr[FRNM_PN_INFO_OFFSET]
                              );
    SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
  }
#endif
}
#endif

/** \brief Handle start up error and global time error.
 ** \param instIdx Index to the channel clusters.
 ** \param longBussOff FrNm_StartupError has been called.
 */
FUNC(void, FRNM_CODE) FrNm_HandleSynchError
(
  uint8 instIdx,
  boolean longBussOff
)
{
  /* In case FRNM_SYNCH_ERR_EXTENDED is set to STD_ON
   * The behaviour of start up error is implemented as descibed in ticket
   * www.autosar.org/bugzilla/show_bug.cgi?id=52552
   *
   * In case FRNM_SYNCH_ERR_EXTENDED is set to STD_OFF
   * The behaviour of start up error is implemented as descibed in ticket
   * www.autosar.org/bugzilla/show_bug.cgi?id=51436.
   * Following cases are considered as special cases.
   *
   * Case 1. Repeat message state (and when cycle counter emulation is ON):
   * The reaction upon synchronous error in network mode (when cycle counter emulation is OFF) is
   * to enter synchronous mode.
   * The reaction upon synchronous error in Repeat message state (when cycle counter emulation is
   * ON) is to enter bus sleep mode.
   *
   * Case 2. Synchronous state:
   * The reaction upon synchronous error in synchronous state (if network is not in requested
   * state) is to enter bus sleep mode. But, synchronous state is the target state if error occures
   * in network mode.
   *
   * Notes:
   * SWS doesn't say reaction upon error while reading global time when FrNm is in the synchronize
   * state. Therefore, synchronize state must ignore error of global time and consider only
   * FrNm_StartUpError().
   *
   * The reaction upon synchronous error in ready sleep state is not clear, therefore we consider
   * it same as of the behaviour when cycle counter emulation is OFF. i,e state will change to
   * synchronous mode.
   *
   * Because of the above mentioned points it is assumed that synchronous error is a kind of event
   * for which action to be taken immediately and the error status should  not be kept for
   * evaluating at a later point of time.
   *
   * The rationale behind the rules mentioned in SWS are not clear and expect these rules would
   * change in future autosar releases.
   */

#if (FRNM_SYNCH_ERR_EXTENDED == STD_ON)
  boolean leaveNetworkMode = FALSE;

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
#if (FRNM_CYCLE_COUNTER_EMULATION == STD_ON)
  TS_PARAM_UNUSED(longBussOff);
#endif



  switch(FRNM_CHANNEL_STATUS(instIdx).CurState)
  {
    case NM_STATE_SYNCHRONIZE:
      /* On short error no action required */
      /* On long error handling done via statemachine(guard) */
      break;
    /* !LINKSTO SWS_FrNm_00384,1, SWS_FrNm_00342,1 */
    case NM_STATE_REPEAT_MESSAGE:
    case NM_STATE_NORMAL_OPERATION:
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE);
      leaveNetworkMode = TRUE;
      break;
    case NM_STATE_READY_SLEEP:
#if (FRNM_CYCLE_COUNTER_EMULATION == STD_OFF)
      /* Emit event to indicate startup/synch error to enter into bus sleep state */
      if(longBussOff == FALSE)
      {
        /* !LINKSTO SWS_FrNm_00444,1 */
        if((FRNM_CHANNEL_STATUS(instIdx).ChanStatus & FRNM_NETWORK_REQUESTED) != 0U)
        {
          FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE);
          leaveNetworkMode = TRUE;
        } /* else wait for longbussoff */
      }
      else
      {
        /* !LINKSTO SWS_FrNm_00338,1 */
        FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE);
        leaveNetworkMode = TRUE;
      }
#endif
      break;
    default:
      /* nothing to do */
      break;
  }

  /* Stop data or vote transmission.
   * This bit is used by Enable/Disable communication service as well.
   * This bit is cleared upon entry of network mode and doesn't conflict with above services as
   * the enable/disable services are allowed to set/clear this bit only in the network mode.
   */

  if(leaveNetworkMode == TRUE)
  {
    /* Stop all timers irrespective of whether they are runing or not */
    FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
    FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;
    FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = 0U;
  }

#else /* FRNM_SYNCH_ERR_EXTENDED == STD_ON */

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
  TS_PARAM_UNUSED(longBussOff);

#if (FRNM_CYCLE_COUNTER_EMULATION == STD_ON)
  if (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_REPEAT_MESSAGE)
  {
    /* !LINKSTO FRNM385,1, FRNM386_Refine,1 */
    /* Emit event to indicate startup/synch error to enter into bus sleep state */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SLEEP_MODE);
  }
  else
#endif
  {
    /* !LINKSTO FRNM383,1, FRNM384_Refine,1, FRNM338,1 */
    /* !LINKSTO FRNM342_StartupError,1, FRNM342_NoGlobalTime,1 */
    /* Emit event to indicate startup/synch error to enter into synchronize state */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_FAILSAFE_SYNCH_MODE);
  }

  /* Stop data or vote transmission.
   * This bit is used by Enable/Disable communication service as well.
   * This bit is cleared upon entry of network mode and doesn't conflict with above services as
   * the enable/disable services are allowed to set/clear this bit only in the network mode.
   */

  FRNM_CHANNEL_STATUS(instIdx).ComEnabled = FALSE;

  /* Stop all timers irrespective of whether they are runing or not */
  FRNM_CHANNEL_STATUS(instIdx).UniversalTimer = 0U;
  FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer = 0U;
  FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer = 0U;

#endif
}

#if ((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))
/** \brief Handle control bit vector received respective for each clusters.
 ** \param instIdx Index to the channel clusters.
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleControlBitVector
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
)
{
  if (FRNM_CHANNELID_CONFIG(instIdx).FrNmNodeDetectionEnabled == TRUE)
  {
    /* Check if repeat message request feature is supported.*/
    if (((FRNM_CHANNEL_PROPERTY(instIdx) & FRNM_REPEAT_MESSAGE_BIT) ==
          FRNM_REPEAT_MESSAGE_BIT) &&
        ((FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[0U] & 0x01U) == 0x01U)
       )
    {
      /* Set the channel status. No event being emitted */
      TS_AtomicSetBit_8(&FRNM_CHANNEL_STATUS(instIdx).ChanStatus, FRNM_REPEAT_MSG_REQUEST_BIT);
    }
  }
}
#endif /*((FRNM_REPEAT_MSG_BIT_ENABLED == STD_ON) && (FRNM_NODE_DETECTION_ENABLED == STD_ON))*/

/** \brief Handle timers respective for each clusters.
 ** \param instIdx ClusterIndex received from MainFunction
 */
STATIC FUNC(void, FRNM_CODE) FrNm_CheckRepetitionCycle
(
  uint8 instIdx
)
{

#if (FRNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif
  /* Set the Repetition Cycle counter event */
  if (0U == (FRNM_CHANNEL_STATUS(instIdx).ActCycle %
             FRNM_CHANNELTIME_CONFIG(instIdx).RepetitionCycle
            )
      )
  {
    /* Emit event so as to perform transition from the current state if the
     * guard conditions are met. */
    FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_REP_CYCLE_COMPLETED);

    /* Call Nm_SynchronizationPoint if FrNm is in Network Mode, FRNM322 */
#if (FRNM_SYNCHRONIZATIONPOINT_ENABLED == STD_ON)
    if ((FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_NORMAL_OPERATION) ||
        (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_READY_SLEEP) ||
        (FRNM_CHANNEL_STATUS(instIdx).CurState == NM_STATE_REPEAT_MESSAGE)
       )
       {
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
         FRNM_CALL_NM_SYNCHRONIZATIONPOINT(FRNM_INST(instIdx));
#else
       if (FRNM_CALL_NM_SYNCHRONIZATIONPOINT(FRNM_INST(instIdx)) != SCHM_E_OK)
         {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
           /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
           FRNM_DET_REPORT_ERROR(FRNM_INST(instIdx),
                                 (FRNM_SERVID_MAINFUNCTION_X + FRNM_INST(instIdx)),
                                 FRNM_E_SYNCHRONIZATIONPOINT
                                );
#endif
         }
#endif
       }
#endif
  }
}

/** \brief Handle timers respective for each clusters.
 ** \param instIdx ClusterIndex received from MainFunction
 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleTimers
(
  /* Deviation MISRAC2012-1 */
  FRNM_PDL_SF(uint8 instIdx)
)
{
  if (FRNM_CHANNEL_STATUS(instIdx).UniversalTimer > 0U)
  {
    --FRNM_CHANNEL_STATUS(instIdx).UniversalTimer;
    if (FRNM_CHANNEL_STATUS(instIdx).UniversalTimer == 0U)
    {
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_UNI_TIMEOUT);
    }
  }

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
  if (FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer > 0U)
  {
    --FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer;
    if (FRNM_CHANNEL_STATUS(instIdx).TimeoutTimer == 0U)
    {
      FRNM_EMIT(instIdx, FRNM_HSM_FRNM_EV_TX_TIMEOUT);
    }
  }
#endif

  if (0U == (FRNM_CHANNEL_STATUS(instIdx).ActCycle %
             FRNM_CHANNELTIME_CONFIG(instIdx).RepetitionCycle
            )
     )
  {
#if ((FRNM_SYNCH_ERR_EXTENDED == STD_ON) && (FRNM_CYCLE_COUNTER_EMULATION == STD_OFF))
    /* If Synchronization is lost and cycle emulation is off
     * the ActCycle can get stuck on 0 this way ready sleep counter
     * is decremented every cycle and not repetition cycle */
    if (FRNM_CHANNEL_STATUS(instIdx).FrNm_SyncLoss == FALSE)
#endif
    {
      if (FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer > 0U)
      {
        /* Note from SWS:
         * As all transitions regarding the FrNmReadySleepCnt are guarded and the order of evaluation
         * is implicitly defined by the guards - the FrNmReadySleepCnt is only decremented if no Repeat
         * Message Request is active (FrNm_RepeatMessage is set to FALSE), the network has been released
         * (FrNm_NetworkRequested is set to FALSE) and if no Cluster awake vote has been received on
         * the bus.
         */

        /* Note from SWS :
         * A local request to keep the network awake (i.e., Network Request) will be Ignored in the
         * Ready Sleep State when the FrNmReadySleepCnt has already reached 0.
         */

        /* We ignore local network requests, be it NetworkRequest or RepeatMessageRequest
         * at the time when FrNmReadySleepCnt reach 0.
         */
        --FRNM_CHANNEL_STATUS(instIdx).ReadySleepTimer;
      }
    }
  }
  /* FrNmNodeDetectionLock timer shall be considered later while implementing relevant feature. */
}

#if ((FRNM_HSM_INST_MULTI_ENABLED == STD_ON) || \
     (FRNM_DEV_ERROR_DETECT == STD_ON))
/** \brief Get channel/cluster array index.
 ** \param NetworkHandle ChannelId received from ComM/Nm
 */
STATIC FUNC(uint8, FRNM_CODE) FrNm_GetClusterIndex
(
  NetworkHandleType NetworkHandle
)

{
  uint8_least instIdx;

  /* Deviation TASKING-1 */
  for (instIdx = 0U; instIdx < FRNM_NUMBER_OF_CHANNELS; instIdx++)
  {
    if (FRNM_CHANNEL_CONFIG(instIdx).ChannelIdentifiers.ChannelId == NetworkHandle)
    {
      break;
    }
  }
  return (uint8)instIdx;
}
#endif /*((FRNM_HSM_INST_MULTI_ENABLED == STD_ON) || (FRNM_DEV_ERROR_DETECT == STD_ON))*/

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/** \brief Get Tx pdu array index.
 ** \param PduId TxPduId received from FrIf
 */
STATIC FUNC(uint8, FRNM_CODE) FrNm_GetPduIndex
(
  PduIdType PduId
)
{
  uint8_least instIdx = 0U;
  /* No parse have been added because
   * instIdx is never greater or equal to FRNM_NUMBER_OF_TX_PDUS
   * since FrNm_RootPtr->TxPduMap always contains PduId.
   * Expression instIdx < FRNM_NUMBER_OF_TX_PDUS is added for defensive
   * programming.
   */
/* CHECK: NOPARSE */
  /* Deviation TASKING-2 */
  for (instIdx = 0; instIdx < FRNM_NUMBER_OF_TX_PDUS; instIdx++)
  {
    if (FrNm_RootPtr->TxPduMap[instIdx].TxPduId == PduId)
    {
      break;
    }
  }
/* CHECK: PARSE */
  return (uint8)instIdx;
}
#endif

#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON) || (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
STATIC FUNC(void, FRNM_CODE) FrNm_HandlePnInfo
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
)
{
  uint8 PnIndex;
  boolean EiraChanged = FALSE;
  VAR (uint8, AUTOMATIC) PnInfo[FRNM_PN_INFO_LENGTH];

  /* Protect FrNm_PnStatus from concurrent access in different
   * Channel specific main functions.
   */
  SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  /* copy and clear external and internal requests */
  TS_MemCpy(PnInfo, PnStatePtr->PnInfo, FRNM_PN_INFO_LENGTH);
  TS_MemSet(PnStatePtr->PnInfo, 0U, FRNM_PN_INFO_LENGTH);

  SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

  for (PnIndex = 0U; PnIndex < FRNM_PN_INFO_LENGTH; PnIndex++)
  {
    if ((PnInfo[PnIndex] & FrNm_RootPtr->PnFilterMask[PnIndex]) != 0U)
    {
      uint8 PnBitPos;

      for (PnBitPos = 0U; PnBitPos < 8U; PnBitPos++)
      {
        /* load timer value by mapping through FrNm_EiraTimerMap*/
        if ((FrNm_RootPtr->EiraTimerMap[(PnIndex * 8U) + PnBitPos]) != FRNM_EIRA_INVALID_INDEX)
        {
          uint8 PnBitMask = 1U << PnBitPos;

          if ((PnInfo[PnIndex] & PnBitMask) != 0U)
          {
            if ((PnStatePtr->PnValue[PnIndex] & PnBitMask) == 0U)
            {
              SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
              PnStatePtr->PnValue[PnIndex] |= PnBitMask;
              SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
              EiraChanged = TRUE;
            }
            /* !LINKSTO FRNM436,1 */
            PnStatePtr->PnTimer[FrNm_RootPtr->EiraTimerMap[(PnIndex * 8U) + PnBitPos]] =
                                                                            FRNM_PN_RESET_TIME + 1U;
          }
        }
      }
    }
  }

  if (EiraChanged == TRUE)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = PnStatePtr->PnValue;
    pduInfo.SduLength = FRNM_PN_INFO_LENGTH;
    /* !LINKSTO FRNM438,1 */
    PduR_FrNmRxIndication(PduId, &pduInfo);
  }
}

STATIC FUNC(boolean, FRNM_CODE) FrNm_AggregatePnInfo
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) PnInfo
)
{
  uint8 PnIndex;
  uint8 PnInfoEira;
  boolean ValidMessage = FALSE;

  for (PnIndex = 0U; PnIndex < FRNM_PN_INFO_LENGTH; PnIndex++)
  {
    /* Aggregate external and internal requests */
    /* !LINKSTO FRNM433,1 */
    PnInfoEira = PnInfo[PnIndex] & FrNm_RootPtr->PnFilterMask[PnIndex];
    PnStatePtr->PnInfo[PnIndex] |= PnInfoEira;

    if (PnInfoEira != 0U)
    {
      ValidMessage = TRUE;
    }
  }

  return(ValidMessage);
}

FUNC(void, FRNM_CODE) FrNm_HandlePnTimers
(
  P2VAR(FrNm_PnStatusType, AUTOMATIC, FRNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
)
{
  uint8 PnIndex;
  boolean EiraChanged = FALSE;

  for (PnIndex = 0U; PnIndex < FRNM_EIRA_MAX_TIMER_SIZE; PnIndex++)
  {
    uint8 EiraIndex = FrNm_RootPtr->EiraTimerMap[PnIndex];

    if (EiraIndex != FRNM_EIRA_INVALID_INDEX)
    {
      /* Valid index; The Eira Timer can be accessed using this index */
      if (PnStatePtr->PnTimer[EiraIndex] > 0U)
      {
        /* Protect FrNm_PnStatus from concurrent access in different
         * Channel specific main functions.
         */
        SchM_Enter_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();

        PnStatePtr->PnTimer[EiraIndex]--;

        if (PnStatePtr->PnTimer[EiraIndex] == 0U)
        {
          /* Decrement PN timer.
           * Timers in EiraTimer holds values corresponding to each PN (each bit) by
           * mapping through FrNm_RootPtr->EiraTimerMap.
           * For example EiraTimer[0] is the timer for PN0 (Bit 0 of EiraValue[0]) and
           * EiraTimer[9] is the timer for PN9 (Bit 1 of EiraValue[1]).
           */
          /* !LINKSTO FRNM437,1 */
          PnStatePtr->PnValue[PnIndex/8U] &= (uint8)~((uint8)(1U << (PnIndex % 8U)));
          EiraChanged = TRUE;
        }

        SchM_Exit_FrNm_SCHM_FRNM_EXCLUSIVE_AREA_0();
      }
    }
  }
  if (EiraChanged == TRUE)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = PnStatePtr->PnValue;
    pduInfo.SduLength = FRNM_PN_INFO_LENGTH;
    /* !LINKSTO FRNM438,1 */
    PduR_FrNmRxIndication(PduId, &pduInfo);
  }
}

#endif

#if (FRNM_CARWAKEUP_RX_ENABLED == STD_ON)
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, FRNM_CODE) FrNm_HandleCarWakeup(
  FRNM_PDL_SF(PduIdType instIdx))
{
  if (FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpRxEnabled == TRUE)
  {
    /* !LINKSTO ECUC_FrNm_00402,1 */
    uint8 CwuBytePos = FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpBytePosition;
    uint8 CwuMask = 1U << FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpBitPosition;
    uint8 RecNid = 0U;
    boolean CallCarWakeUpIndication = FALSE;
    /* Parameter FrNmCarWakeUpFilterEnabled can be set to true
     * only in case NodeId is configured this is assured by an xdm check.
     * Reading of the NodeId shall be done only if FrNmCarWakeUpFilterEnabled
     * is set to true, this assures that NodeId is configured.
     */
    if(FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpFilterEnabled == TRUE)
    {
      RecNid = FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[FRNM_PDU_BYTE_1];
    }

    if((FRNM_CHANNEL_STATUS(instIdx).RxPduPtr[CwuBytePos] & CwuMask) != 0U)
    {
      /* !LINKSTO ECUC_FrNm_00410,1 */
      if (FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpFilterEnabled == FALSE)
      {
        CallCarWakeUpIndication = TRUE;
      }

      /* !LINKSTO ECUC_FrNm_00412,1 */
      if ((FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpFilterEnabled == TRUE) &&
          (RecNid == FRNM_CHANNELID_CONFIG(instIdx).FrNmCarWakeUpFilterNodeId))
      {
        CallCarWakeUpIndication = TRUE;
      }

      if(CallCarWakeUpIndication == TRUE)
      {
#if (FRNM_MULTICORE_ENABLED == STD_OFF)
        FRNM_CALL_NM_CARWAKEUPINDICATION(FRNM_INST(instIdx));
#else
        if (FRNM_CALL_NM_CARWAKEUPINDICATION(FRNM_INST(instIdx)) != SCHM_E_OK)
        {
#if (FRNM_DEV_ERROR_DETECT == STD_ON)
           uint8 FrNm_Instance_Macro;
#if (FRNM_HSM_INST_MULTI_ENABLED == STD_ON)
           FrNm_Instance_Macro = (uint8)(FRNM_INST(instIdx));
#else
           FrNm_Instance_Macro = 0U;
#endif
          /* !LINKSTO FrNm.EB.SchM_Call_DET,1 */
          FRNM_DET_REPORT_ERROR(FrNm_Instance_Macro,
                                FRNM_SERVID_RXINDICATION,
                                FRNM_E_CARWAKEUPINDICATION
                               );
#endif
        }
#endif
      }
    }
  }
}
#endif

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[end of file]========================================*/
