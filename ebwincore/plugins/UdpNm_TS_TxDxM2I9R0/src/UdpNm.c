/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
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
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * It is used in function parameter declarations and definitions or as structure member where
 * the number of parentheses matter.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * In the context of "for(index = 0U; (index < UDPNM_NUMBER_OF_CHANNELS); ++index)"
 * when "UDPNM_NUMBER_OF_CHANNELS = 1" the message condition is always true is expected;
 * Expression "index < UDPNM_NUMBER_OF_CHANNELS" is always evaluated as true
 * if "UDPNM_NUMBER_OF_CHANNELS = 1".
 */

/*===============================[includes]=================================*/
#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#ifndef UDPNM_NO_PBCFG_REQUIRED
#define UDPNM_NO_PBCFG_REQUIRED
#endif

#include <UdpNm_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <TSAtomic_Assign.h>    /* Atomic assignment macros */
#include <TSMem.h>
#include <UdpNm_Api.h>          /* UdpNm API definitions (own interface) */
#include <UdpNm_Int.h>          /* UdpNm internal interface */
#include <UdpNm_Cbk.h>          /* UdpNm callback API (own interface) */
#include <UdpNm_Hsm.h>          /* public API of UdpNm_Hsm.c */
#include <UdpNm_HsmUdpNm.h>     /* public statechart model definitions, */
#include <UdpNm_Lcfg.h>
#include <UdpNm_Lcfg_Static.h>

/* !LINKSTO EB_SWS_UdpNm_00082_4,1 */
#include <Nm_Cbk.h>             /* Nm API for Nm_StateChangeNotification() */
/* !LINKSTO EB_SWS_UdpNm_00082_7,1 */
#include <SchM_UdpNm.h>         /* SchM-header for UdpNm */
/* !LINKSTO EB_SWS_UdpNm_00083_1,1 */
#include <SoAd.h>

#if ((UDPNM_COM_USER_DATA_SUPPORT ==STD_ON) || (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON))
/* !LINKSTO SWS_UdpNm_00311,1 */
#include <PduR_UdpNm.h>         /* PduR API for UdpNm */
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (UDPNM_DET_REPORT_ERROR() macros) */
/* !LINKSTO EB_SWS_UdpNm_00082_5,1 */
#include <Det.h>
#endif

#include <UdpNm_Adaptive.h>
#if (UDPNM_ADAPTIVE == STD_ON)
#include <AdaptiveUdpNm_Cfg.h>
#endif
/*===========================[macro definitions]============================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef UDPNM_VENDOR_ID /* configuration check */
#error UDPNM_VENDOR_ID must be defined
#endif

#if (UDPNM_VENDOR_ID != 1U) /* vendor check */
#error UDPNM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef UDPNM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error UDPNM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (UDPNM_AR_RELEASE_MAJOR_VERSION != 4U)
#error UDPNM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef UDPNM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error UDPNM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (UDPNM_AR_RELEASE_MINOR_VERSION != 1U )
#error UDPNM_AR_RELEASE_MINOR_VERSION wrong (!= 1U)
#endif

#ifndef UDPNM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error UDPNM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (UDPNM_AR_RELEASE_REVISION_VERSION != 3U )
#error UDPNM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef UDPNM_SW_MAJOR_VERSION /* configuration check */
#error UDPNM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (UDPNM_SW_MAJOR_VERSION != 2U)
#error UDPNM_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef UDPNM_SW_MINOR_VERSION /* configuration check */
#error UDPNM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (UDPNM_SW_MINOR_VERSION < 9U)
#error UDPNM_SW_MINOR_VERSION wrong (< 9U)
#endif

#ifndef UDPNM_SW_PATCH_VERSION /* configuration check */
#error UDPNM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (UDPNM_SW_PATCH_VERSION < 7U)
#error UDPNM_SW_PATCH_VERSION wrong (< 7U)
#endif





/*------------------------[Defensive programming]----------------------------*/

#if (defined UDPNM_PRECONDITION_ASSERT)
#error UDPNM_PRECONDITION_ASSERT is already defined
#endif

#if (defined UDPNM_PRECONDITION_ASSERT_NO_EVAL)
#error UDPNM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (UDPNM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))
#else
#define UDPNM_PRECONDITION_ASSERT(Condition, ApiId)
#define UDPNM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined UDPNM_POSTCONDITION_ASSERT)
#error UDPNM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined UDPNM_POSTCONDITION_ASSERT_NO_EVAL)
#error UDPNM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (UDPNM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))
#else
#define UDPNM_POSTCONDITION_ASSERT(Condition, ApiId)
#define UDPNM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined UDPNM_INVARIANT_ASSERT)
#error UDPNM_INVARIANT_ASSERT is already defined
#endif

#if (defined UDPNM_INVARIANT_ASSERT_NO_EVAL)
#error UDPNM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (UDPNM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define UDPNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))
#else
#define UDPNM_INVARIANT_ASSERT(Condition, ApiId)
#define UDPNM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined UDPNM_STATIC_ASSERT)
# error UDPNM_STATIC_ASSERT is already defined
#endif
#if (UDPNM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define UDPNM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define UDPNM_STATIC_ASSERT(expr)
#endif

#if (defined UDPNM_UNREACHABLE_CODE_ASSERT)
#error UDPNM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (UDPNM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define UDPNM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, (ApiId))
#else
#define UDPNM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined UDPNM_INTERNAL_API_ID)
#error UDPNM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define UDPNM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[module internal macros]--------------------------------*/

#if (defined UDPNM_EMIT)
#error UDPNM_EMIT already defined
#endif
/* just an abbreviating macro */
#define UDPNM_EMIT(a,b)                     UDPNM_HSMEMITINST(&UdpNm_HsmScUdpNm, (a),( b))

#if (defined UDPNM_HANDLE_TICK)
#error UDPNM_HANDLE_TICK already defined
#endif

#define UDPNM_HANDLE_TICK(instIdx, timer, event)         \
  do                                                     \
  {                                                      \
    if (UDPNM_CHANNEL_STATUS(instIdx).timer > 0U)        \
   {                                                     \
     --UDPNM_CHANNEL_STATUS(instIdx).timer;              \
     if (UDPNM_CHANNEL_STATUS(instIdx).timer == 0U)      \
     {                                                   \
       (void)UDPNM_EMIT(instIdx, event);                 \
     }                                                   \
   }                                                     \
  }                                                      \
  while (0)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define UDPNM_START_SEC_CODE
/* !LINKSTO EB_SWS_UdpNm_00082_8,1 */
#include <UdpNm_MemMap.h>

#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
/** \brief Process Car wakeup functionality
 ** \param instIdx index of state machine instance to work on
 ** \return To call CarWakeupRxIndication or not */
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, UDPNM_CODE) UdpNm_HandleCarWakeup(
  UDPNM_PDL_SF(PduIdType instIdx));

/** \brief Process Car wakeup callouts
 ** \param instIdx index of state machine instance to work on
 */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleCarWakeupCallout(
  UDPNM_PDL_SF(PduIdType instIdx));
#endif

/** \brief Process all timers.
 ** \param instIdx index of state machine instance to work on */
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleTimerTick(UDPNM_PDL_SF(uint8 instIdx));

#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON) || (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)

/** \brief Handles the EIRA/ERA accumulation
 **
 ** This function accumulate requests in the EIRA/ERA vector. If a change occur the timer is
 ** reseted and the PduR is notified via PduR_UdpNmRxIndication callback.
 **
 ** \param[in] PnStatePtr Pointer to EIRA/ERA structure
 ** \param[in] PduId Id of the PDU used in UdpNm_SoAdIfRxIndication callback
 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandlePnInfo
(
  P2VAR(UdpNm_PnStatusType, AUTOMATIC, UDPNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
);

/** \brief Process the EIRA/ERA timers
 **
 ** This function decrement the timer for EIRA/ERA
 ** If the timer expires the change is communicate to PduR via the PduR_UdpNmRxIndication callback
 **
 ** \param[in] pPnStatus Pointer to EIRA/ERA structure
 ** \param[in] PduId Id of the PDU used in PduR_UdpNmRxIndication callback
 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandlePnTimers
(
  P2VAR(UdpNm_PnStatusType, AUTOMATIC, UDPNM_APPL_DATA) pPnStatus,
  PduIdType PduId
);

/** \brief Aggregate the EIRA/ERA
 **
 ** This function aggregate and filter(based on UdpNm_PnFilterMask) the recieved requests
 **
 ** \param[in] PnInfo Pointer to the EIRA/ERA structure member where the unfiltered
 **            requests are stored
 ** \param[in] pPnStatus Pointer to EIRA/ERA structure member where the filtered PN requests
 **            will be aggregated
 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_AggregatePnInfo
(
  P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) PnInfo,
  P2VAR(UdpNm_PnStatusType,AUTOMATIC, UDPNM_APPL_DATA) pPnStatus
);

/** \brief checks whether the received message have valid PN information
 **
 ** This function checks whether the received PDU have the PN Info Bit set
 ** and if the Pn Info pass the PnFilterMask
 **
 ** \param[in] PduId Pdu Id of the received PDU.
 **
 ** \return Received message is valid or not.
 */
STATIC FUNC(boolean, UDPNM_CODE) UdpNm_IsValidPnMessage
(
  PduIdType PduId
);

#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*=====================[external constants]=================================*/

/*=====================[internal constants]=================================*/

/*=======================[external data]====================================*/

#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

#if (UDPNM_ADAPTIVE == STD_OFF)
VAR(UdpNm_ChanStatusType, UDPNM_VAR) UdpNm_ChanStatus[UDPNM_NUMBER_OF_CHANNELS];
#else
P2VAR(UdpNm_ChanStatusType, UDPNM_VAR, UDPNM_APPL_DATA) UdpNm_ChanStatus;
#endif

P2CONST(UdpNm_ConfigType, UDPNM_VAR, UDPNM_APPL_CONST) UdpNm_RootPtr;

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/*=======================[internal data]====================================*/

#define UDPNM_START_SEC_VAR_INIT_8
#include <UdpNm_MemMap.h>

/** \brief Intialization information of UdpNm module */
STATIC VAR(boolean, UDPNM_VAR) UdpNm_Initialized = FALSE;

#define UDPNM_STOP_SEC_VAR_INIT_8
#include <UdpNm_MemMap.h>

#define UDPNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/** \brief Partial netoworking info: EiraValue and EiraTimer */
#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
VAR(UdpNm_PnStatusType, UDPNM_VAR) UdpNm_EiraStatus;
#endif

#define UDPNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <UdpNm_MemMap.h>

/*=====================[external functions definitions]=====================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/*------------------[UdpNm_IsValidConfig]---------------------------------------------*/
/* !LINKSTO UdpNm.Impl.IsValid,1, UdpNm.EB.PBCFGM101,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_IsValidConfig
(
  P2CONST(void, AUTOMATIC, UDPNM_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(UdpNm_ConfigType,AUTOMATIC,UDPNM_APPL_CONST) ConfigPtr = voidConfigPtr;

  if (ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    /* !LINKSTO UdpNm.EB.PBCFGM102,1 */
    if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
    {
      /* Validate the post build configuration against the compile time configuration */
      if (UDPNM_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if (UdpNm_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
          Published information in compile time configuration*/
          if(UDPNM_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
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

/*------------------[UdpNm_Init]---------------------------------------------*/
/* !LINKSTO SWS_UdpNm_00208,1 */
FUNC(void, UDPNM_CODE) UdpNm_Init
(
  P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_APPL_CONST) udpnmConfigPtr
)
{
  NetworkHandleType ChIdx;
  /* No protection against races required, as there is no concurrent
   * calling of functions expected at initialization time of the ComStack */

  /* !LINKSTO SWS_UdpNm_00060,1 */
  P2CONST(UdpNm_ConfigType,AUTOMATIC,UDPNM_APPL_CONST) LocalConfigPtr = udpnmConfigPtr;

  DBG_UDPNM_INIT_ENTRY(udpnmConfigPtr);

  /* Save the configuration pointer */
  /* !LINKSTO UdpNm.EB.PBCFGM110,1 */
#if (UDPNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager */
  if (LocalConfigPtr == NULL_PTR)
  {
    /* !LINKSTO UdpNm.Impl.Init,1 */
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if (E_OK == PbcfgM_GetConfig(UDPNM_MODULE_ID, UDPNM_INSTANCE_ID, &ModuleConfig))
    {
      LocalConfigPtr = (P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* UDPNM_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  if ( E_OK != UdpNm_IsValidConfig(LocalConfigPtr))
  {
    /* !LINKSTO SWS_UdpNm_00018,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INSTANCE_ID, UDPNM_SERVID_INIT, UDPNM_E_INIT_FAILED);
  }
  else
  if(UDPNM_PBRAM_SIZE > UDPNM_DATA_MEM_SIZE)
  {
    /* !LINKSTO UdpNm.EB.PostBuildRamSize_3,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INSTANCE_ID, UDPNM_SERVID_INIT, UDPNM_E_INIT_FAILED);
  }
  else
#else
  /* !LINKSTO UdpNm.EB.ComM_Init_NULL_PTR,1 */
  if ( E_OK == UdpNm_IsValidConfig(LocalConfigPtr))
#endif
  {
    UdpNm_RootPtr = LocalConfigPtr;

    /* Deviation TASKING-1 */
    for(ChIdx = 0U; ChIdx < UDPNM_NUMBER_OF_CHANNELS; ChIdx++)
    {
      UDPNM_CHANNEL_STATUS(ChIdx).RxPduPtr = &UdpNm_Rx_Tx_Buffer[UDPNM_CHANNEL_BUFFER_OFFSET(ChIdx)];
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
      UDPNM_CHANNEL_STATUS(ChIdx).TxPduPtr = &UdpNm_Rx_Tx_Buffer[UDPNM_TX_BUFFER_OFFSET + \
                                                                 UDPNM_CHANNEL_BUFFER_OFFSET(ChIdx)];
#endif
      UDPNM_CHANNEL_STATUS(ChIdx).MessageFlags = 0U;
      /* !LINKSTO SWS_UdpNm_00141,2 */
      UDPNM_CHANNEL_STATUS(ChIdx).CurState = NM_STATE_BUS_SLEEP;
      UDPNM_CHANNEL_STATUS(ChIdx).UdpNmTimeoutTimer = 0U;
      UDPNM_CHANNEL_STATUS(ChIdx).UniversalTimer = 0U;
      UDPNM_CHANNEL_STATUS(ChIdx).RmsTimer = 0U;
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
      /* !LINKSTO SWS_UdpNm_00061,2, SWS_UdpNm_00033,1 */
      UDPNM_CHANNEL_STATUS(ChIdx).MsgCycleTimer = 0U;
      UDPNM_CHANNEL_STATUS(ChIdx).ImmediateNmTransmissionCounter = 0;
#endif
      /* !LINKSTO SWS_UdpNm_00143,1 */
      UDPNM_CHANNEL_STATUS(ChIdx).ChanStatus = 0U;
#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
      /* !LINKSTO SWS_UdpNm_00355,1, UdpNm.ASR431.SWS_UdpNm_00355,1 */
      TS_MemSet(UDPNM_CHANNEL_STATUS(ChIdx).Era.PnValue, 0U, UDPNM_PN_INFO_LENGTH);
      TS_MemSet(UDPNM_CHANNEL_STATUS(ChIdx).Era.PnInfo, 0U, UDPNM_PN_INFO_LENGTH);
      TS_MemSet(UDPNM_CHANNEL_STATUS(ChIdx).Era.PnTimer,
                0x00U,
                (UDPNM_EIRA_MAX_TIMER_SIZE * sizeof(UdpNm_TimeType))
               );
#endif
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
      UDPNM_CHANNEL_STATUS(ChIdx).ActiveWakeUp1 = 0U;
      UDPNM_CHANNEL_STATUS(ChIdx).ActiveWakeUp2 = 0U;
#endif
    }

    UdpNm_HsmInit(&UdpNm_HsmScUdpNm);

#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
  /* !LINKSTO SWS_UdpNm_00342,1, SWS_UdpNm_00343,1, SWS_UdpNm_00344,1 */
  /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00344,1 */
  TS_MemSet(UdpNm_EiraStatus.PnValue, 0x00U, UDPNM_PN_INFO_LENGTH);
  TS_MemSet(UdpNm_EiraStatus.PnInfo,  0x00U, UDPNM_PN_INFO_LENGTH);
  TS_MemSet(UdpNm_EiraStatus.PnTimer, 0x00U, (UDPNM_EIRA_MAX_TIMER_SIZE * sizeof(UdpNm_TimeType)));
#endif

    UdpNm_Initialized = TRUE;
  }

  DBG_UDPNM_INIT_EXIT(udpnmConfigPtr);
}

/*------------------[UdpNm_PassiveStartUp]-----------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00211,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_PASSIVESTARTUP_ENTRY(nmChannelHandle);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00039,1, SWS_UdpNm_00191,1, SWS_UdpNm_00018,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_PASSIVESTARTUP, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_PASSIVESTARTUP, UDPNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    Nm_StateType CurState = UDPNM_CHANNEL_STATUS(index).CurState;

    if ((CurState == NM_STATE_BUS_SLEEP) ||
        (CurState == NM_STATE_PREPARE_BUS_SLEEP)
       )
    {
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
      SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
      if (UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 == 0U)
      {
        UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 = UDPNM_PASSIVE_STARTUP_AWB;
      }
      else
      {
        UDPNM_CHANNEL_STATUS(index).ActiveWakeUp2 = UDPNM_PASSIVE_STARTUP_AWB;
      }
      SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
#endif

      /* !LINKSTO EB_SWS_UdpNm_00128_1,1, EB_SWS_UdpNm_00128_2,1 */
      UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_NETWORK_START);
      retVal = E_OK;
    }
    else
    {
      /* !LINKSTO EB_SWS_UdpNm_00212,1, UdpNm.ASR431.SWS_UdpNm_00147,1 */
      retVal = E_NOT_OK;
    }
  }

  DBG_UDPNM_PASSIVESTARTUP_EXIT(retVal, nmChannelHandle);

  return retVal;
}

/*------------------[UdpNm_NetworkRequest]-----------------------------------*/
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00213,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_NETWORKREQUEST_ENTRY(nmChannelHandle);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_NETWORKREQUEST, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_NETWORKREQUEST, UDPNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    /* Set the network requested flag */
    /* !LINKSTO SWS_UdpNm_00104,1 */
    UDPNM_CHANNEL_STATUS(index).ChanStatus |= (uint8)UDPNM_NETWORK_REQUESTED;
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if (UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 == 0U)
    {
      UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 = UDPNM_NETWORK_REQUEST_AWB;
    }
#endif
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED);

    retVal = E_OK;
  }

  DBG_UDPNM_NETWORKREQUEST_EXIT(retVal, nmChannelHandle);

  return retVal;
}

/*------------------[UdpNm_NetworkGwEraRequest]-----------------------------------*/
/* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkGwEraRequest
(
  NetworkHandleType nmChannelHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_NETWORKGWERAREQUEST_ENTRY(nmChannelHandle);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(
      nmChannelHandle, UDPNM_SERVID_NETWORKGWERAREQUEST, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
    || (UdpNm_IndexFromNmChannelHandle[nmChannelHandle]
      >= UDPNM_NUMBER_OF_CHANNELS))
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(
      nmChannelHandle, UDPNM_SERVID_NETWORKGWERAREQUEST, UDPNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index
       = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#else
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    /* Set the network requested flag */
    /* !LINKSTO UdpNm.EB.UdpNm_NetworkGwEraRequest.NetworkRequest,1 */
    UDPNM_CHANNEL_STATUS(index).ChanStatus |= (uint8)UDPNM_NETWORK_REQUESTED;
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if (UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 == 0U)
    {
      UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 = UDPNM_NETWORK_GW_ERA_REQUEST_AWB;
    }
#endif
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    /* Emit the event to trigger the transition */
    UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED);

    retVal = E_OK;
  }


  DBG_UDPNM_NETWORKGWERAREQUEST_EXIT(retVal,nmChannelHandle);

  return retVal;
}

/*------------------[UdpNm_NetworkRelease]-----------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00214,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_NETWORKRELEASE_ENTRY(nmChannelHandle);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_NETWORKRELEASE, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_NETWORKRELEASE, UDPNM_E_INVALID_CHANNEL);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif

    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    /* Clear the Network requested flag */
    /* !LINKSTO SWS_UdpNm_00105,1 */
    UDPNM_CHANNEL_STATUS(index).ChanStatus &= (uint8)(~UDPNM_NETWORK_REQUESTED);
#if (UDPNM_ACTIVE_WAKEUP_BIT_ENABLED == STD_ON)
    if ((UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 == UDPNM_NETWORK_REQUEST_AWB) ||
        (UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 == UDPNM_NETWORK_GW_ERA_REQUEST_AWB))
    {
      UDPNM_CHANNEL_STATUS(index).ActiveWakeUp1 = UDPNM_CHANNEL_STATUS(index).ActiveWakeUp2;
      UDPNM_CHANNEL_STATUS(index).ActiveWakeUp2 = 0U;
    }
#endif
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_NET_REQ_STATUS_CHANGED);

    retVal = E_OK;
  }

  DBG_UDPNM_NETWORKRELEASE_EXIT(retVal, nmChannelHandle);

  return retVal;
}

#endif /* (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) */

/*------------------[UdpNm_DisableCommunication]-----------------------------*/
#if (UDPNM_COM_CONTROL_ENABLED == STD_ON)
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00215,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_DisableCommunication(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_DISABLECOMMUNICATION_ENTRY(nmChannelHandle);

#if ((UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF) || (UDPNM_PASSIVE_MODE_ENABLED == STD_ON))
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_DISABLECOMMUNICATION, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_DISABLECOMMUNICATION,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else
#endif
  {
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
    boolean emitComControlEv = FALSE;
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    if ((UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_REPEAT_MESSAGE) ||
        (UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_NORMAL_OPERATION) ||
        (UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_READY_SLEEP)
       )
    {
      if ((UDPNM_CHANNEL_STATUS(index).ChanStatus & UDPNM_COM_DISABLED) == 0U)
      {
        UDPNM_CHANNEL_STATUS(index).ChanStatus |= (uint8)UDPNM_COM_DISABLED;

        emitComControlEv = TRUE;

        retVal = E_OK;
      }
      else
      {
        /* !LINKSTO SWS_UdpNm_00177_Implicit,1 */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00172,1 */
      retVal = E_NOT_OK;
    }
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    if(emitComControlEv == TRUE)
    {
      UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_COM_CONTROL);
    }
#else
    /* !LINKSTO SWS_UdpNm_00307,1 */
    retVal = E_NOT_OK;
#endif
  }

  DBG_UDPNM_DISABLECOMMUNICATION_EXIT(retVal, nmChannelHandle);

  return retVal;
}

/*------------------[UdpNm_EnableCommunication]------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00216,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_EnableCommunication(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_ENABLECOMMUNICATION_ENTRY(nmChannelHandle);

#if ((UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF) || (UDPNM_PASSIVE_MODE_ENABLED == STD_ON))
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_ENABLECOMMUNICATION, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_ENABLECOMMUNICATION,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else
#endif
  {

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
     boolean emitComControlEv = FALSE;
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    if ((UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_REPEAT_MESSAGE) ||
        (UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_NORMAL_OPERATION) ||
        (UDPNM_CHANNEL_STATUS(index).CurState == NM_STATE_READY_SLEEP)
       )
    {
      if ((UDPNM_CHANNEL_STATUS(index).ChanStatus & UDPNM_COM_DISABLED) != 0U)
      {
        /* !LINKSTO SWS_UdpNm_00176,1 */
        UDPNM_CHANNEL_STATUS(index).ChanStatus &= (uint8)(~UDPNM_COM_DISABLED);

        emitComControlEv = TRUE;

        retVal = E_OK;
      }
      else
      {
        /* !LINKSTO SWS_UdpNm_00177,1 */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* !LINKSTO SWS_UdpNm_00305,1 */
      retVal = E_NOT_OK;
    }
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    if(emitComControlEv == TRUE)
    {
      UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_COM_CONTROL);
    }
#else
    /* !LINKSTO SWS_UdpNm_00306,1 */
    retVal = E_NOT_OK;
#endif
  }

  DBG_UDPNM_ENABLECOMMUNICATION_EXIT(retVal, nmChannelHandle);

  return retVal;
}
#endif /* (UDPNM_COM_CONTROL_ENABLED == STD_ON) */

/*------------------------[UdpNm_SetUserData]--------------------------------*/
/* !LINKSTO SWS_UdpNm_00312,1 */
#if ((UDPNM_USER_DATA_ENABLED == STD_ON)        \
  && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)    \
  && (UDPNM_COM_USER_DATA_SUPPORT == STD_OFF))
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00217,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetUserData
(
  NetworkHandleType nmChannelHandle,
  P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmUserDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_SETUSERDATA_ENTRY(nmChannelHandle, nmUserDataPtr);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_SETUSERDATA, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_SETUSERDATA, UDPNM_E_INVALID_CHANNEL);
  }
  else if (nmUserDataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_SETUSERDATA, UDPNM_E_NULL_POINTER);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    uint32 UserDataLength = (uint32)UDPNM_CHANNEL_PDULENGTH(index) - \
                            (uint32)UDPNM_CHANNEL_CONFIG(index).UDFBPos;
    if (UserDataLength != 0U)
    {
      /* !LINKSTO SWS_UdpNm_00159,1 */
      SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
      TS_MemCpy(&UDPNM_CHANNEL_STATUS(index).TxPduPtr[UDPNM_CHANNEL_CONFIG(index).UDFBPos],
                nmUserDataPtr,
                UserDataLength
               );
      SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

      retVal = E_OK;
    }
  }

  DBG_UDPNM_SETUSERDATA_EXIT(retVal, nmChannelHandle, nmUserDataPtr);

  return retVal;
}

#endif /* ((UDPNM_USER_DATA_ENABLED == STD_ON)
        * && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
        * && (UDPNM_COM_USER_DATA_SUPPORT == STD_OFF)) */

/*------------------------[UdpNm_GetUserData]--------------------------------*/
#if (UDPNM_USER_DATA_ENABLED == STD_ON)
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00218,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetUserData
(
  NetworkHandleType                         nmChannelHandle,
  P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA)  nmUserDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_GETUSERDATA_ENTRY(nmChannelHandle, nmUserDataPtr);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETUSERDATA, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETUSERDATA, UDPNM_E_INVALID_CHANNEL);
  }
  else if (nmUserDataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETUSERDATA, UDPNM_E_NULL_POINTER);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    uint32 UserDataLength = (uint32)UDPNM_CHANNEL_PDULENGTH(index) - \
                            (uint32)UDPNM_CHANNEL_CONFIG(index).UDFBPos;
    /* !LINKSTO UdpNm.ComUserDataSupport.SetGetUserDataNoUserData,2 */
    if (UserDataLength != 0U)
    {
      SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

      /* !LINKSTO SWS_UdpNm_00160,2 */
      TS_MemCpy(nmUserDataPtr,
                &UDPNM_CHANNEL_STATUS(index).RxPduPtr[UDPNM_CHANNEL_CONFIG(index).UDFBPos],
                UserDataLength
               );

      SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

      retVal = E_OK;
    }
  }

  DBG_UDPNM_GETUSERDATA_EXIT(retVal, nmChannelHandle, nmUserDataPtr);

  return retVal;
}
#endif /* (UDPNM_USER_DATA_ENABLED == STD_ON) */

/*------------------------[UdpNm_GetNodeIdentifier]--------------------------*/
#if (UDPNM_NODE_ID_ENABLED == STD_ON)
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00219,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetNodeIdentifier
(
  NetworkHandleType                         nmChannelHandle,
  P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA)  nmNodeIdPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ((UDPNM_DEV_ERROR_DETECT == STD_ON) || (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON))
  NetworkHandleType index = 0U;

  if(nmChannelHandle < UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
  {
    index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
  }
#endif
#if (UDPNM_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(nmChannelHandle);
#endif

  DBG_UDPNM_GETNODEIDENTIFIER_ENTRY(nmChannelHandle, nmNodeIdPtr);

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETNODEIDENTIFIER, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (index >= UDPNM_NUMBER_OF_CHANNELS) ||
           (UDPNM_CHANNEL_CONFIG(index).UdpNmNodeIdEnabled == FALSE)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(
      nmChannelHandle, UDPNM_SERVID_GETNODEIDENTIFIER, UDPNM_E_INVALID_CHANNEL);
  }
  else if (nmNodeIdPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETNODEIDENTIFIER, UDPNM_E_NULL_POINTER);
  }
  else
#endif
  {
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00132,1 */
    if(UDPNM_CHANNEL_CONFIG(index).UdpNmNodeIdEnabled == TRUE)
    {
      *nmNodeIdPtr = UDPNM_CHANNEL_STATUS(index).RxPduPtr[UDPNM_CHANNEL_CONFIG(index).NidPos];
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  DBG_UDPNM_GETNODEIDENTIFIER_EXIT(retVal, nmChannelHandle, nmNodeIdPtr);

  return retVal;
}

/*------------------------[UdpNm_GetLocalNodeIdentifier]---------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00220,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetLocalNodeIdentifier
(
  NetworkHandleType                         nmChannelHandle,
  P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA)  nmNodeIdPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if ((UDPNM_DEV_ERROR_DETECT == STD_ON) || (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON))
  NetworkHandleType index = 0U;

  if(nmChannelHandle < UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
  {
    index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
  }
#endif
  DBG_UDPNM_GETLOCALNODEIDENTIFIER_ENTRY(nmChannelHandle, nmNodeIdPtr);

#if (UDPNM_DEV_ERROR_DETECT == STD_OFF) && (UDPNM_NODEID_CALLBACK_ENABLED == STD_OFF) && (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETLOCALNODEIDENTIFIER, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (index >= UDPNM_NUMBER_OF_CHANNELS) ||
           (UDPNM_CHANNEL_CONFIG(index).UdpNmNodeIdEnabled == FALSE)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_GETLOCALNODEIDENTIFIER,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else if (nmNodeIdPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_GETLOCALNODEIDENTIFIER,
                           UDPNM_E_NULL_POINTER
                          );
  }
  else
#endif
  {
#if (UDPNM_NODEID_CALLBACK_ENABLED == STD_ON)
    /* !LINKSTO UdpNm.EB.UdpNmNodeIdCallback_1,1 */
    /* Store the value return by Node Id callback function */
    *nmNodeIdPtr = UDPNM_NODEID_CALLBACK_NAME(nmChannelHandle);
#else
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00133,1 */
    if(UDPNM_CHANNEL_CONFIG(index).UdpNmNodeIdEnabled == TRUE)
    {
      *nmNodeIdPtr = UDPNM_CHANNEL_NODEID(UdpNm_IndexFromNmChannelHandle[nmChannelHandle]);
      retVal = E_OK;
     }
     else
     {
       retVal = E_NOT_OK;
     }
#endif
  }

  DBG_UDPNM_GETLOCALNODEIDENTIFIER_EXIT(retVal, nmChannelHandle, nmNodeIdPtr);

  return retVal;
}
#endif /* (UDPNM_NODE_ID_ENABLED == STD_ON) */

/*------------------------[UdpNm_RepeatMessageRequest]---------------------*/
/* !LINKSTO EB_SWS_UdpNm_00163_3,1 */
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00221,1, SWS_UdpNm_00135,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
  Std_ReturnType retVal = E_NOT_OK;
  Nm_StateType CurState = NM_STATE_UNINIT;

#if ((UDPNM_DEV_ERROR_DETECT == STD_ON) || (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON))
  NetworkHandleType index = 0U;
  if(nmChannelHandle < UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
  {
    index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
  }
#endif
  DBG_UDPNM_REPEATMESSAGEREQUEST_ENTRY(nmChannelHandle);

#if (UDPNM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_REPEATMESSAGEREQUEST, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (index >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_REPEATMESSAGEREQUEST,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else
#endif
  {
    CurState = UDPNM_CHANNEL_STATUS(index).CurState;
    if ((UDPNM_CHANNEL_CONFIG(index).UdpNmNodeDetectionEnabled == TRUE) &&
        ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP))
       )
    {
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00113,1, UdpNm.ASR431.SWS_UdpNm_00121,1, UdpNm.ASR431.SWS_UdpNm_00111,1 */
      TS_AtomicSetBit_8(&UDPNM_CHANNEL_STATUS(index).TxPduPtr[UDPNM_CHANNEL_CONFIG(index).CbvPos],
                        UDPNM_CBV_REPEATMESSAGEBIT
                       );

      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00120,1, UdpNm.ASR431.SWS_UdpNm_00112,1 */
      UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON);

      retVal = E_OK;
    }
    else
    {
      /* !LINKSTO EB_SWS_UdpNm_00137_1,1, EB_SWS_UdpNm_00137_2,1, EB_SWS_UdpNm_00137_3,1 */
      retVal = E_NOT_OK;
    }
  }

  DBG_UDPNM_REPEATMESSAGEREQUEST_EXIT(retVal, nmChannelHandle);

  return retVal;
}
#endif /* ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)) */

/*------------------------[UdpNm_GetPduData]---------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00138,1 */
#if ((UDPNM_USER_DATA_ENABLED == STD_ON)        \
  || (UDPNM_NODE_ID_ENABLED == STD_ON)          \
  || (UDPNM_NODE_DETECTION_ENABLED == STD_ON))
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00309,1, EB_SWS_UdpNm_00139_1,1, EB_SWS_UdpNm_00139_2,1, EB_SWS_UdpNm_00139_3,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetPduData
(
  NetworkHandleType                        nmChannelHandle,
  P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmPduDataPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

#if ((UDPNM_DEV_ERROR_DETECT == STD_ON) || (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON))
  NetworkHandleType index = 0U;

  if(nmChannelHandle < UDPNM_INDEXFROMNMCHANNELHANDLE_NUM)
  {
    index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
  }
#endif
  DBG_UDPNM_GETPDUDATA_ENTRY(nmChannelHandle, nmPduDataPtr);

#if (UDPNM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETPDUDATA, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (index >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETPDUDATA, UDPNM_E_INVALID_CHANNEL);
  }
  else if (nmPduDataPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETPDUDATA, UDPNM_E_NULL_POINTER);
  }
  else
#endif
 {
#if(UDPNM_USER_DATA_ENABLED == STD_OFF)
  /* !LINKSTO EB_SWS_UdpNm_00139_1,1, EB_SWS_UdpNm_00139_2,1, EB_SWS_UdpNm_00139_3,1 */
  if((UDPNM_CHANNEL_CONFIG(index).UdpNmNodeDetectionEnabled == TRUE) ||
     (UDPNM_CHANNEL_CONFIG(index).UdpNmNodeIdEnabled == TRUE)
    )
#endif
  {
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    /* !LINKSTO SWS_UdpNm_00375,1, UdpNm.ASR431.SWS_UdpNm_00375,1 */
    TS_MemCpy(nmPduDataPtr, UDPNM_CHANNEL_STATUS(index).RxPduPtr, UDPNM_CHANNEL_PDULENGTH(index));

    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

  retVal = E_OK;
  }
 }

  DBG_UDPNM_GETPDUDATA_EXIT(retVal, nmChannelHandle, nmPduDataPtr);

  return retVal;
}
#endif /* if ((UDPNM_USER_DATA_ENABLED == STD_ON)
        * || (UDPNM_NODE_ID_ENABLED == STD_ON)
        * || (UDPNM_NODE_DETECTION_ENABLED == STD_ON)) */

/*------------------------[UdpNm_GetState]-----------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00310,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetState
(
  NetworkHandleType                               nmChannelHandle,
  P2VAR(Nm_StateType, AUTOMATIC, UDPNM_APPL_DATA) nmStatePtr,
  P2VAR(Nm_ModeType, AUTOMATIC, UDPNM_APPL_DATA)  nmModePtr
)
{
  Nm_StateType CurState;
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_GETSTATE_ENTRY(nmChannelHandle, nmStatePtr, nmModePtr);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETSTATE, UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETSTATE, UDPNM_E_INVALID_CHANNEL);
  }
  else if (nmStatePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETSTATE, UDPNM_E_NULL_POINTER);
  }
  else if (nmModePtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_GETSTATE, UDPNM_E_NULL_POINTER);
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif

    TS_AtomicAssign8(CurState, UDPNM_CHANNEL_STATUS(index).CurState);

    /* !LINKSTO SWS_UdpNm_00092,1, SWS_UdpNm_00094,1 */
    switch (CurState)
    {
      case NM_STATE_NORMAL_OPERATION: /* fall through */
      case NM_STATE_REPEAT_MESSAGE: /* fall through */
      case NM_STATE_READY_SLEEP:
        *nmModePtr = NM_MODE_NETWORK;
        break;
      case NM_STATE_PREPARE_BUS_SLEEP:
        *nmModePtr = NM_MODE_PREPARE_BUS_SLEEP;
        break;
      default:
        /* By default, Bus sleep state is the current state
           after initialization */
        *nmModePtr = NM_MODE_BUS_SLEEP;
        break;
    }
    *nmStatePtr = CurState;
    retVal = E_OK;
  }

  DBG_UDPNM_GETSTATE_EXIT(retVal, nmChannelHandle, nmStatePtr, nmModePtr);

  return retVal;
}

/*------------------------[UdpNm_GetVersionInfo]-----------------------------*/
#if (UDPNM_VERSION_INFO_API == STD_ON)
/* !LINKSTO SWS_UdpNm_00224,1 */
FUNC(void, UDPNM_CODE) UdpNm_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, UDPNM_APPL_DATA) versioninfo
)
{
  DBG_UDPNM_GETVERSIONINFO_ENTRY(versioninfo);

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1, ECUC_UdpNm_00003,1 */
  if (NULL_PTR == versioninfo)
  {
    /* !LINKSTO SWS_UdpNm_00318,1, SWS_UdpNm_00292,1, SWS_UdpNm_00018,1 */
    UDPNM_DET_REPORT_ERROR(UDPNM_INSTANCE_ID, UDPNM_SERVID_GETVERSIONINFO, UDPNM_E_NULL_POINTER);
  }
  else
#endif
  {
    versioninfo->vendorID         = UDPNM_VENDOR_ID;
    versioninfo->moduleID         = UDPNM_MODULE_ID;
    versioninfo->sw_major_version = UDPNM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = UDPNM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = UDPNM_SW_PATCH_VERSION;
  }

  DBG_UDPNM_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* (UDPNM_VERSION_INFO_API == STD_ON) */

/*------------------------[UdpNm_RequestBusSynchronization]------------------*/
/* !LINKSTO EB_SWS_UdpNm_00163_1,1, SWS_UdpNm_00130,2, UdpNm.ASR431.SWS_UdpNm_00226,1 */
#if ((UDPNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RequestBusSynchronization
(
 NetworkHandleType nmChannelHandle
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_ENTRY(nmChannelHandle);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle, UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION,UDPNM_E_NO_INIT);
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif

#if (UDPNM_COM_CONTROL_ENABLED == STD_ON)
    if (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(index).ChanStatus, UDPNM_COM_DISABLED_BIT, uint8))
    {
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00181,1 */
      retVal = E_NOT_OK;
    }
    else
#endif
    {
      Nm_StateType CurState = UDPNM_CHANNEL_STATUS(index).CurState;
      if ((CurState == NM_STATE_BUS_SLEEP) ||
          (CurState == NM_STATE_PREPARE_BUS_SLEEP)
         )
      {
        /* !LINKSTO EB_SWS_UdpNm_00187_1,1, EB_SWS_UdpNm_00187_2,1 */
        retVal = E_NOT_OK;
      }
      else
      {
        PduInfoType pduInfo;

        pduInfo.SduDataPtr = UDPNM_CHANNEL_STATUS(index).TxPduPtr;
        pduInfo.SduLength  = UDPNM_CHANNEL_PDULENGTH(index);

#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
        /* !LINKSTO SWS_UdpNm_00317,1 */
        UdpNm_GetPduUserData(UDPNM_INST(index), &pduInfo);
#endif
        if (E_OK == SoAd_IfTransmit(UDPNM_CHANNEL_TXPDUID(index), &pduInfo))
        {
          retVal = E_OK;
        }
      }
    }
  }

  DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_EXIT(retVal, nmChannelHandle);

  return retVal;
}
#endif /* if ((UDPNM_BUS_SYNCHRONIZATION == STD_ON)
          && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))*/

/*------------------------[UdpNm_CheckRemoteSleepIndication]-----------------*/
#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/* !LINKSTO SWS_UdpNm_00149,1, UdpNm.ASR431.SWS_UdpNm_00227,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_CheckRemoteSleepIndication
(
  NetworkHandleType                         nmChannelHandle,
  P2VAR(boolean, AUTOMATIC, UDPNM_APPL_DATA) nmRemoteSleepIndPtr
)
{
  /* It makes no sense to protect this function against interruption, as
   * if the result shall be reliable for the caller, it must protect the
   * whole call against interruption anyway. */

  Std_ReturnType retVal = E_NOT_OK;

  DBG_UDPNM_CHECKREMOTESLEEPINDICATION_ENTRY(nmChannelHandle, nmRemoteSleepIndPtr);

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
    TS_PARAM_UNUSED(nmChannelHandle);
#endif

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00196,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_CHECKREMOTESLEEPINDICATION,
                           UDPNM_E_NO_INIT
                          );
  }
  else if ((nmChannelHandle >= UDPNM_INDEXFROMNMCHANNELHANDLE_NUM) ||
           (UdpNm_IndexFromNmChannelHandle[nmChannelHandle] >= UDPNM_NUMBER_OF_CHANNELS)
        )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_CHECKREMOTESLEEPINDICATION,
                           UDPNM_E_INVALID_CHANNEL
                          );
  }
  else if (nmRemoteSleepIndPtr == NULL_PTR)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1 */
    UDPNM_DET_REPORT_ERROR(nmChannelHandle,
                           UDPNM_SERVID_CHECKREMOTESLEEPINDICATION,
                           UDPNM_E_NULL_POINTER
                          );
  }
  else
#endif
  {
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    const NetworkHandleType index = UdpNm_IndexFromNmChannelHandle[nmChannelHandle];
#endif
    Nm_StateType CurState = UDPNM_CHANNEL_STATUS(index).CurState;
    if ((CurState == NM_STATE_NORMAL_OPERATION) ||
        (CurState == NM_STATE_READY_SLEEP)
       )
    {
      /* !LINKSTO SWS_UdpNm_00153,1 */
      if (TS_IsBitSet(&UDPNM_CHANNEL_STATUS(index).ChanStatus, UDPNM_RSI_CALLED_BIT, uint8))
      {
        *nmRemoteSleepIndPtr = TRUE;
        retVal = E_OK;
      }
      else
      {
        *nmRemoteSleepIndPtr = FALSE;
        retVal = E_OK;
      }
    }
    else
    {
      /* !LINKSTO EB_SWS_UdpNm_00154_1,1, EB_SWS_UdpNm_00154_2,1, EB_SWS_UdpNm_00154_3,1 */
      retVal = E_NOT_OK;
    }
  }

  DBG_UDPNM_CHECKREMOTESLEEPINDICATION_EXIT(retVal, nmChannelHandle, nmRemoteSleepIndPtr);

  return retVal;
}
#endif /* (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON) */

/*------------------------[UdpNm_MainFunction]-------------------------------*/
FUNC(void, UDPNM_CODE) UdpNm_MainFunction(uint8 instIdx)
{
  /* Always perform initialization check w/o Det report. See EB
   * req. UdpNm.MainFunction.InitCheck. Related SWS requirements are not
   * fulfilled. */
  /* !LINKSTO UdpNm.MainFunction.InitCheck,1 */

#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  if (TRUE == UdpNm_Initialized)
  {

    UDPNM_CHANNEL_STATUS(instIdx).MessageFlags &= (uint8)(~(uint8)(UDPNM_MESSAGE_SENT_ON_CYCLE));
    UdpNm_HandleTimerTick(UDPNM_PL_SF(instIdx));

    /* Main processing function of HSM */
    (void)UDPNM_HSMMAININST(&UdpNm_HsmScUdpNm, instIdx);

#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
    /* Process ERA for all channels*/
    if(UDPNM_CHANNEL_CONFIG(instIdx).EraCalcEnabled == STD_ON)
    {
      /* !LINKSTO SWS_UdpNm_00354,1, SWS_UdpNm_00353,1 */
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00372_1,1 */
      UdpNm_HandlePnInfo(&UDPNM_CHANNEL_STATUS(instIdx).Era,
                         UDPNM_CHANNEL_CONFIG(instIdx).EraPduId
                        );
      UdpNm_HandlePnTimers(&UDPNM_CHANNEL_STATUS(instIdx).Era,
                           UDPNM_CHANNEL_CONFIG(instIdx).EraPduId
                          );
    }
    else
    {
      /* added for C coverage */
    }
#endif

#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
    /* call UdpNm_HandlePnTimers and UdpNm_HandlePnInfo once per all channels(last channel) */
    if(instIdx == (UDPNM_NUMBER_OF_CHANNELS - 1U))
    {
      /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00372_2,1 */
      UdpNm_HandlePnInfo(&UdpNm_EiraStatus, UDPNM_PN_EIRA_PDUID);
      UdpNm_HandlePnTimers(&UdpNm_EiraStatus, UDPNM_PN_EIRA_PDUID);
    }
#endif
  }

}

/*------------------------[UdpNm_SoAdIfTxConfirmation]-----------------------------*/
/* !LINKSTO SWS_UdpNm_00228,2 */
#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfTxConfirmation(PduIdType TxPduId)
{
  NetworkHandleType index = 0U;

  DBG_UDPNM_SOADIFTXCONFIRMATION_ENTRY(TxPduId);

  /* lookup udpnm channel related given txpduid */
  /* Deviation TASKING-1 */
  for (index = 0U; (index < UDPNM_NUMBER_OF_CHANNELS); ++index)
  {
    if(UDPNM_CHANNEL_CONFIG(index).TxConfPduId == TxPduId)
    {
      break;
    }
  }

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO SWS_UdpNm_00196,1, SWS_UdpNm_00018,1 */
  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00039,1, EB_SWS_UdpNm_00229_2,1, SWS_UdpNm_00191,1, SWS_UdpNm_00230,1 */
    UDPNM_DET_REPORT_ERROR((uint8)TxPduId, UDPNM_SERVID_TXCONFIRMATION, UDPNM_E_NO_INIT);
  }
  else if (index > (UDPNM_NUMBER_OF_CHANNELS - 1U))
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2, UdpNm.ASR431.SWS_UdpNm_00463,1 */
    UDPNM_DET_REPORT_ERROR((uint8)TxPduId, UDPNM_SERVID_TXCONFIRMATION, UDPNM_E_INVALID_PDUID);
  }
  else
#endif
  {
#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
    uint32 UserDataLength = (uint32)UDPNM_CHANNEL_PDULENGTH(index) - \
                            (uint32)UDPNM_CHANNEL_CONFIG(index).UDFBPos;
    /* !LINKSTO UdpNm.ComUserDataSupport.PduRCallsNoUserData,2 */
    if (0U != UserDataLength)
    {
      /* !LINKSTO SWS_UdpNm_00316,1 */
      PduR_UdpNmTxConfirmation(UDPNM_CHANNEL_USERTXCONFPDUID(index));
    }
#endif

    UDPNM_EMIT(UDPNM_INST(index), UDPNM_HSM_UDPNM_EV_TX_CONFIRMATION);

  }
#if ((UDPNM_HSM_INST_MULTI_ENABLED == STD_OFF) && (UDPNM_DEV_ERROR_DETECT == STD_OFF))
  TS_PARAM_UNUSED(TxPduId);
#endif

  DBG_UDPNM_SOADIFTXCONFIRMATION_EXIT(TxPduId);
}
#endif /* (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) */

/*------------------------[UdpNm_SoAdIfRxIndication]-------------------------------*/
/* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00231,1 */

#if(UDPNM_SOADIFRXINDICATION_ASR412 == STD_OFF)
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication
(
 PduIdType                                        RxPduId,
 P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
)
#else
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication
(
 PduIdType                                        RxPduId,
 P2VAR(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
)
#endif
{
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
  NetworkHandleType index = 0U;
#endif
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
  boolean CallCarWakeUpIndication = FALSE;
#endif
#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
  PduInfoType UserPdu;
#endif
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
  boolean RepeatMsgBitSet = FALSE;
#endif

#if (UDPNM_HSM_INST_MULTI_ENABLED != STD_ON)
  TS_PARAM_UNUSED(RxPduId);
#endif

  DBG_UDPNM_SOADIFRXINDICATION_ENTRY(RxPduId, PduInfoPtr);

#if (UDPNM_DEV_ERROR_DETECT == STD_ON)

  /* !LINKSTO SWS_UdpNm_00233,1, EB_SWS_UdpNm_00232_1,1 */
  /* lookup UdpNm channel related given RxPduId */
  /* Deviation TASKING-1 */
  for (index = 0U; index < UDPNM_NUMBER_OF_CHANNELS; ++index)
  {
    if(UDPNM_CHANNEL_CONFIG(index).RxPduId == RxPduId)
    {
      break;
    }
  }

  if (UdpNm_Initialized == FALSE)
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00191,1, UdpNm.ASR431.SWS_UdpNm_00039,1, EB_SWS_UdpNm_00232_2,1 */
    UDPNM_DET_REPORT_ERROR((uint8)RxPduId, UDPNM_SERVID_RXINDICATION, UDPNM_E_NO_INIT);
  }
  /* !LINKSTO SWS_UdpNm_00196,1 */
  else if (index > (UDPNM_NUMBER_OF_CHANNELS - 1U))
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00192,2, EB_SWS_UdpNm_00232_4,1, UdpNm.ASR431.SWS_UdpNm_00463,1 */
    UDPNM_DET_REPORT_ERROR((uint8)RxPduId, UDPNM_SERVID_RXINDICATION, UDPNM_E_INVALID_PDUID);
  }
  else if (((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR)) ||
           (PduInfoPtr->SduLength > UDPNM_CHANNEL_PDULENGTH(RxPduId))
          )
  {
    /* !LINKSTO SWS_UdpNm_00018,1, SWS_UdpNm_00292,1, EB_SWS_UdpNm_00232_3,1 */
    UDPNM_DET_REPORT_ERROR((uint8)RxPduId,
                           UDPNM_SERVID_RXINDICATION,
                           UDPNM_E_NULL_POINTER
                          );
  }
  else
#endif
  {
#if (UDPNM_PN_SUPPORTED == STD_ON)
    boolean ValidPnMessage = FALSE;
#endif

    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

    /* !LINKSTO SWS_UdpNm_00035,1 */
    /* Copy Pdu Data to internal buffer */
    TS_MemCpy(UDPNM_CHANNEL_STATUS(RxPduId).RxPduPtr,
              PduInfoPtr->SduDataPtr,
              PduInfoPtr->SduLength
             );

#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON) || (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
    /* !LINKSTO UdpNm.EB.PnEnabled_FALSE_1,1 */
    if (UDPNM_CHANNEL_PNENABLED(RxPduId) == TRUE)
    {
      /* Check whether the received message have valid PN information */
      ValidPnMessage = UdpNm_IsValidPnMessage(RxPduId);

      if (ValidPnMessage == TRUE)
      {
        /* UdpNm_SoAdIfRxIndication could be called more than once within the interval of 2 main
         * function calls. Therefore, the external requests to be aggregated.
         */
#if (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
        if (UDPNM_CHANNEL_CONFIG(RxPduId).EraCalcEnabled == STD_ON)
        {
          /* !LINKSTO SWS_UdpNm_00353,1 */
          UdpNm_AggregatePnInfo(&UDPNM_CHANNEL_STATUS(RxPduId).RxPduPtr[UDPNM_PN_INFO_OFFSET],
                                &UDPNM_CHANNEL_STATUS(RxPduId).Era
                               );
        }
        else
        {
          /* added for C coverage */
        }
#endif
#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
        /* !LINKSTO SWS_UdpNm_00342,1 */
        UdpNm_AggregatePnInfo(&UDPNM_CHANNEL_STATUS(RxPduId).RxPduPtr[UDPNM_PN_INFO_OFFSET],
                              &UdpNm_EiraStatus
                             );
#endif
      }
    }
#endif

#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
    /* Requirement SWS_UdpNm_00375 is satisfied since
     * the calling of the function Nm_CarWakeUpIndication
     * is done after the population of RxPduPtr buffer
     */
    /* !LINKSTO SWS_UdpNm_00375,1 */
    CallCarWakeUpIndication = UdpNm_HandleCarWakeup(UDPNM_PL_SF(RxPduId));
#endif
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
    if (UDPNM_CHANNEL_CONFIG(RxPduId).UdpNmNodeDetectionEnabled == TRUE)
    {
      if (TS_IsBitSet(&(UDPNM_CHANNEL_STATUS(RxPduId).RxPduPtr[UDPNM_CHANNEL_CONFIG(RxPduId).CbvPos]),
          UDPNM_CBV_REPEATMESSAGEBIT, uint8))
      {
        RepeatMsgBitSet = TRUE;
      }
    }
#endif
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
    if (CallCarWakeUpIndication == TRUE)
    {
      UdpNm_HandleCarWakeupCallout(UDPNM_PL_SF(RxPduId));
    }
#endif
#if (UDPNM_PN_SUPPORTED == STD_ON)
    /* !LINKSTO SWS_UdpNm_00328,1 */
    /* !LINKSTO SWS_UdpNm_00329,2 */
    /* !LINKSTO SWS_UdpNm_00462,1 */
    /* !LINKSTO SWS_UdpNm_00460,1 */
    /* !LINKSTO SWS_UdpNm_00461,1 */
    if ((ValidPnMessage == TRUE) ||
        (UDPNM_CHANNEL_PNENABLED(RxPduId) == FALSE) ||
        (UDPNM_CHANNEL_CONFIG(RxPduId).AllNmMessagesKeepAwake == TRUE)
       )
#endif
    {
#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
      /* !LINKSTO UdpNm.EB.ComUserDataSupport.UdpNmUserDataRxIndication,1 */
      /* !LINKSTO UdpNm.ComUserDataSupport.PduRCallsNoUserData,2 */
      /* If UserRxPduEnabled is TRUE user data length is greater than zero
       * this is assured by a check in UdpNm_Checks.m
       */
      if(UDPNM_CHANNEL_USERRXPDUENABLED(RxPduId) == TRUE)
      {
        UserPdu.SduLength = (PduLengthType)UDPNM_CHANNEL_PDULENGTH(RxPduId) - \
                            (PduLengthType)UDPNM_CHANNEL_CONFIG(RxPduId).UDFBPos;
        UserPdu.SduDataPtr =
          &(UDPNM_CHANNEL_STATUS(RxPduId).RxPduPtr[UDPNM_CHANNEL_CONFIG(RxPduId).UDFBPos]);
        PduR_UdpNmRxIndication(UDPNM_CHANNEL_USERRXPDUID(RxPduId), &UserPdu);
      }
#endif

/* !LINKSTO EB_SWS_UdpNm_00163_3,1, UdpNm.ASR431.SWS_UdpNm_00119,1 */
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
      /* Notify again if the RepeatMessageBit is set */
      if (RepeatMsgBitSet == TRUE)
      {
#if (UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON)
        uint8 Index;

        /* Nm_RepeatMessageIndication has to be called for all the channels
           which are configured with this Rx Pdu */
        /* Deviation TASKING-1 */
        for (Index = 0U; Index < UDPNM_NUMBER_OF_CHANNELS; Index++)
        {
          if ((UDPNM_CHANNEL_CONFIG(Index).RxPduId == RxPduId) &&
              (UDPNM_CHANNEL_CONFIG(Index).UdpNmRepeatMsgIndEnabled == TRUE)
             )
          {
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
            /* !LINKSTO SWS_UdpNm_00014,1, UdpNm.ASR431.SWS_UdpNm_00014,1 */
            UDPNM_CALL_NM_REPEATMESSAGEINDICATION(UDPNM_INST(Index));
#else
            if (UDPNM_CALL_NM_REPEATMESSAGEINDICATION(UDPNM_INST(Index)) != SCHM_E_OK)
            {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
              /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
              UDPNM_DET_REPORT_ERROR(UDPNM_INST(Index), UDPNM_SERVID_RXINDICATION, UDPNM_E_REPEATMESSAGEINDICATION);
#endif
            }
#endif
          }
        }
#endif
        UDPNM_EMIT((uint8)RxPduId, UDPNM_HSM_UDPNM_EV_REPEAT_MESSAGE_REASON);
      }
#endif
      UDPNM_EMIT((uint8)RxPduId, UDPNM_HSM_UDPNM_EV_RX_INDICATION);
    }
  }

  DBG_UDPNM_SOADIFRXINDICATION_EXIT(RxPduId, PduInfoPtr);
}

/*------------------------[UdpNm_Transmit]-----------------------------------*/
#if ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON )
/* !LINKSTO SWS_UdpNm_00313,1 */
FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_Transmit
(
    PduIdType UdpNmTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
)
{
  TS_PARAM_UNUSED(UdpNmTxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);

  DBG_UDPNM_TRANSMIT_ENTRY(UdpNmTxPduId, PduInfoPtr);

  /* !LINKSTO SWS_UdpNm_00315,1 */

  DBG_UDPNM_TRANSMIT_EXIT(E_OK, UdpNmTxPduId, PduInfoPtr);

  return E_OK;
}
#endif

/*------------------------[UdpNm_GetPduUserData]-----------------------------*/
#if ((UDPNM_COM_USER_DATA_SUPPORT == STD_ON) || (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON))
FUNC(void, UDPNM_CODE) UdpNm_GetPduUserData
(
  uint8 instIdx,
  P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) pduInfo
)
{
#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)
  uint32 UserDataLength = (uint32)UDPNM_CHANNEL_PDULENGTH(instIdx) - \
                          (uint32)UDPNM_CHANNEL_CONFIG(instIdx).UDFBPos;
  /* !LINKSTO UdpNm.ComUserDataSupport.PduRCallsNoUserData,2 */
  if(0U != UserDataLength)
  {
    uint8 UDFBPos = UDPNM_CHANNEL_CONFIG(instIdx).UDFBPos;
    PduInfoType PduUserInfo;

    PduUserInfo.SduDataPtr = &(pduInfo->SduDataPtr[UDFBPos]);
    PduUserInfo.SduLength  = pduInfo->SduLength - UDFBPos;

    /* !LINKSTO SWS_UdpNm_00317,1, UdpNm.ASR431.SWS_UdpNm_00317,1 */
    (void)PduR_UdpNmTriggerTransmit(UDPNM_CHANNEL_USERTXCONFPDUID(instIdx), &PduUserInfo);
  }
#endif
#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON)
  /* !LINKSTO UdpNm.EB.PnEnabled_FALSE_2,1 */
  if (UDPNM_CHANNEL_PNENABLED(instIdx) == TRUE)
  {
    SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
    UdpNm_AggregatePnInfo(&pduInfo->SduDataPtr[UDPNM_PN_INFO_OFFSET],
                          &UdpNm_EiraStatus
                         );
    SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();
  }
#endif
#if (!((UDPNM_HSM_INST_MULTI_ENABLED == STD_ON) && \
       (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)))
  TS_PARAM_UNUSED(instIdx);
#endif
}
#endif /* ( UDPNM_COM_USER_DATA_SUPPORT == STD_ON ) */

/*=====================[external Non API function definitions]=====================*/
/*------------------------[UdpNm_IsValidPnMessage]-----------------------------*/
#if (UDPNM_PN_EIRA_CALC_ENABLED == STD_ON) || (UDPNM_PN_ERA_CALC_ENABLED == STD_ON)
STATIC FUNC(boolean, UDPNM_CODE) UdpNm_IsValidPnMessage(PduIdType PduId)
{
  boolean ValidMessage = FALSE;
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PnInfo =
          &UdpNm_ChanStatus[PduId].RxPduPtr[UDPNM_PN_INFO_OFFSET];

  /* !LINKSTO SWS_UdpNm_00045,2, UdpNm.ASR431.SWS_UdpNm_00045,1, SWS_UdpNm_00331,1 */
  if (TS_IsBitSet(&(UdpNm_ChanStatus[PduId].RxPduPtr[UdpNm_ChanConfig[PduId].CbvPos]),
                  UDPNM_CBV_PNINFOBIT,
                  uint8
                 )
     )
  {
    uint8 PnPduId;
    for (PnPduId = 0U; ((PnPduId < UDPNM_PN_INFO_LENGTH) && (ValidMessage == FALSE)); PnPduId++)
    {
      /* !LINKSTO SWS_UdpNm_00338,1 */
      if ((PnInfo[PnPduId] & UdpNm_RootPtr->PnFilterMask[PnPduId]) != 0U)
      {
        ValidMessage = TRUE;
      }
    }
  }
  return (ValidMessage);
}

#endif

/* CHECK: NOPARSE */
/*
 * No parse have been added because:
 * (UdpNm_Dummy_RxIndication function has been added with the support for multi core.)
 * When modelling the cross core function calls (Schm_Call) context is required
 * for modelling the calling context.
 * The RxIndication is called from an interrupt context which cannot be modelled
 * in Rte therefore a dummy context has been added.
 * Rte generates a function declaration, here an empty function definitions
 * is provided but never called.
 */
#if ((UDPNM_MULTICORE_SUPPORT == STD_ON) &&\
     ((UDPNM_REPEAT_MSG_IND_ENABLED == STD_ON) || (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)))
FUNC(void, UDPNM_CODE) UdpNm_Dummy_RxIndication (void)
{

}
#endif
/* CHECK: PARSE */

/*=====================[internal functions definitions]=====================*/
#if (UDPNM_CARWAKEUP_RX_ENABLED == STD_ON)
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, UDPNM_CODE) UdpNm_HandleCarWakeup(
  UDPNM_PDL_SF(PduIdType instIdx))
{
  boolean CallCarWakeUpIndication = FALSE;
  if (UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpRxEnabled == TRUE)
  {
    /* !LINKSTO SWS_UdpNm_00373,1 */
    uint32 CwuBytePos = UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpBytePosition;
    uint8  CwuMask = 1U << UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpBitPosition;
    uint8  RecNid = 0U;
    /* Parameter UdpNmCarWakeUpFilterEnabled can be set to true
     * only in case NodeId is configured this is assured by an xdm check.
     * Reading of the NodeId shall be done only if UdpNmCarWakeUpFilterEnabled
     * is set to true, this assures that NodeId is configured.
     */
    if(UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpFilterEnabled == TRUE)
    {
      RecNid = UDPNM_CHANNEL_STATUS(instIdx).RxPduPtr[UDPNM_CHANNEL_CONFIG(instIdx).NidPos];
    }

    if((UDPNM_CHANNEL_STATUS(instIdx).RxPduPtr[CwuBytePos] & CwuMask) != 0U)
    {
      /* !LINKSTO SWS_UdpNm_00374,1 */
      if (UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpFilterEnabled == FALSE)
      {
        CallCarWakeUpIndication = TRUE;
      }

      /* !LINKSTO SWS_UdpNm_00376,1 */
      if ((UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpFilterEnabled == TRUE) &&
          (RecNid == UDPNM_CHANNEL_CONFIG(instIdx).UdpNmCarWakeUpFilterNodeId))
      {
        CallCarWakeUpIndication = TRUE;
      }
    }
  }

  return CallCarWakeUpIndication;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleCarWakeupCallout(
  UDPNM_PDL_SF(PduIdType instIdx))
{
#if (UDPNM_MULTICORE_SUPPORT == STD_OFF)
  UDPNM_CALL_NM_CARWAKEUPINDICATION(UDPNM_INST(instIdx));
#else
  if (UDPNM_CALL_NM_CARWAKEUPINDICATION(UDPNM_INST(instIdx)) != SCHM_E_OK)
  {
#if (UDPNM_DEV_ERROR_DETECT == STD_ON)
    uint8 UdpNm_Instance_Macro;
#if (UDPNM_HSM_INST_MULTI_ENABLED == STD_ON)
    UdpNm_Instance_Macro = (uint8)(UDPNM_INST(instIdx));
#else
    UdpNm_Instance_Macro = 0U;
#endif
    /* !LINKSTO UdpNm.EB.SchM_Call_DET,1 */
    UDPNM_DET_REPORT_ERROR(UdpNm_Instance_Macro, UDPNM_SERVID_RXINDICATION, UDPNM_E_CARWAKEUPINDICATION);
#endif
  }
#endif
}
#endif

/*------------------------[UdpNm_HandleTimerTick]-----------------------------*/
/* Deviation MISRAC2012-1 */
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandleTimerTick(UDPNM_PDL_SF(uint8 instIdx))
{
  UDPNM_HANDLE_TICK(instIdx, UniversalTimer, UDPNM_HSM_UDPNM_EV_UNI_TIMEOUT);
  UDPNM_HANDLE_TICK(instIdx, RmsTimer, UDPNM_HSM_UDPNM_EV_RMS_TIMEOUT);

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
  UDPNM_HANDLE_TICK(instIdx, MsgCycleTimer, UDPNM_HSM_UDPNM_EV_MSG_CYCLE_TIMEOUT);
  if (UDPNM_CHANNEL_STATUS(instIdx).UdpNmTimeoutTimer == 1U)
  {
    UDPNM_CHANNEL_STATUS(instIdx).NmTimerExpired = TRUE;
  }
#endif

  UDPNM_HANDLE_TICK(instIdx, UdpNmTimeoutTimer, UDPNM_HSM_UDPNM_EV_NM_TIMEOUT);
}

/*------------------------[UdpNm_HandlePnTimer]-----------------------------*/
#if ((UDPNM_PN_EIRA_CALC_ENABLED == STD_ON) || (UDPNM_PN_ERA_CALC_ENABLED == STD_ON))
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandlePnTimers
(
  P2VAR(UdpNm_PnStatusType,AUTOMATIC, UDPNM_APPL_DATA) pPnStatus,
  PduIdType PduId
)
{
  uint8 PnIndex;
  boolean EiraChanged = FALSE;

  for (PnIndex = 0U; PnIndex < UDPNM_EIRA_MAX_TIMER_SIZE; PnIndex++)
  {
    /* Get EiraTimer index from mapped EIRA/ERA timer array.
     *
     * Timers in EiraTimer holds values corresponding to each PN (each bit)
     * by mapping through UdpNm_EiraTimerMap.
     * For example EiraTimer[0] is the timer for PN0 (Bit 0 of EiraValue[0]) and
     * EiraTimer[9] is the timer for PN9 (Bit 1 of EiraValue[1]). */
    uint8 EiraIndex = UdpNm_RootPtr->EiraTimerMap[PnIndex];

    if (UDPNM_EIRA_TIMER_INVALID != EiraIndex)
    {
      if (0U < pPnStatus->PnTimer[EiraIndex])
      {
        pPnStatus->PnTimer[EiraIndex]--;

        if (0U == pPnStatus->PnTimer[EiraIndex])
        {
          /* If PN timer get expired, then clear the corresponding PN status bit
           * and set the flag to indicate a change in EIRA status */
          /* !LINKSTO SWS_UdpNm_00351,1, SWS_UdpNm_00360,1, UdpNm.ASR431.SWS_UdpNm_00351,1 */
          /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00360,1 */
          pPnStatus->PnValue[PnIndex/8U] &= (uint8)~((uint8)(1U << (PnIndex%8U)));
          EiraChanged = TRUE;
        }
      }
    }
  }

  if (EiraChanged == TRUE)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = pPnStatus->PnValue;
    pduInfo.SduLength = UDPNM_PN_INFO_LENGTH;
    /* !LINKSTO SWS_UdpNm_00352,1, SWS_UdpNm_00361,1, UdpNm.ASR431.SWS_UdpNm_00352,1 */
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00361,1 */
    PduR_UdpNmRxIndication(PduId, &pduInfo);
  }
}

/*------------------------[UdpNm_HandlePnInfo]-----------------------------*/
STATIC FUNC(void, UDPNM_CODE) UdpNm_HandlePnInfo
(
  P2VAR(UdpNm_PnStatusType, AUTOMATIC, UDPNM_APPL_DATA) PnStatePtr,
  PduIdType PduId
)
{
  uint8 PnIndex;
  boolean EiraChanged = FALSE;

  SchM_Enter_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

  /* !LINKSTO SWS_UdpNm_00335,2 */
  for (PnIndex = 0U; PnIndex < UDPNM_PN_INFO_LENGTH; PnIndex++)
  {
    if ((PnStatePtr->PnInfo[PnIndex] & UdpNm_RootPtr->PnFilterMask[PnIndex]) != 0U)
    {
      uint8 PnBitPos;
      for (PnBitPos = 0U; PnBitPos < 8U; PnBitPos++)
      {
        if (UdpNm_RootPtr->EiraTimerMap[(PnIndex * 8U) + PnBitPos] != UDPNM_EIRA_TIMER_INVALID)
        {
          uint8 PnBitMask = 1U << PnBitPos;
          if ((PnStatePtr->PnInfo[PnIndex] & PnBitMask) != 0U)
          {
            if ((PnStatePtr->PnValue[PnIndex] & PnBitMask) == 0U)
            {
              /* !LINKSTO SWS_UdpNm_00347,1, SWS_UdpNm_00357,1, SWS_UdpNm_00348,1 */
              /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00347,1, UdpNm.ASR431.SWS_UdpNm_00348,1 */
              /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00357,1 */
              PnStatePtr->PnValue[PnIndex] |= PnBitMask;
              EiraChanged = TRUE;
            }
            /* load timer value */
            /* !LINKSTO SWS_UdpNm_00359,2, SWS_UdpNm_00349,1, SWS_UdpNm_00350,2 */
            /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00349,1, UdpNm.ASR431.SWS_UdpNm_00359,1 */
            PnStatePtr->PnTimer[UdpNm_RootPtr->EiraTimerMap[(PnIndex * 8U) + PnBitPos]]
                  = UDPNM_PN_RESET_TIME + 1U;
          }
        }
      }
    }
  }
  TS_MemSet(PnStatePtr->PnInfo, 0U, UDPNM_PN_INFO_LENGTH);
  SchM_Exit_UdpNm_SCHM_UDPNM_EXCLUSIVE_AREA_0();

  if (EiraChanged == TRUE)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = PnStatePtr->PnValue;
    pduInfo.SduLength = UDPNM_PN_INFO_LENGTH;
    /* !LINKSTO SWS_UdpNm_00352,1, SWS_UdpNm_00361,1, UdpNm.ASR431.SWS_UdpNm_00352,1 */
    /* !LINKSTO UdpNm.ASR431.SWS_UdpNm_00361,1 */
    PduR_UdpNmRxIndication(PduId, &pduInfo);
  }
}

/*------------------------[UdpNm_AggregatePnInfo]-----------------------------*/
STATIC FUNC(void, UDPNM_CODE) UdpNm_AggregatePnInfo
(
  P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) PnInfo,
  P2VAR(UdpNm_PnStatusType,AUTOMATIC, UDPNM_APPL_DATA) pPnStatus
)
{
  uint8 PnIndex;
  /* !LINKSTO SWS_UdpNm_00335,2 */
  for (PnIndex = 0U; PnIndex < UDPNM_PN_INFO_LENGTH; PnIndex++)
  {
      /* Aggregate external requests */
      /* !LINKSTO SWS_UdpNm_00356,1, SWS_UdpNm_00346,1 */
      pPnStatus->PnInfo[PnIndex] |= (uint8)(PnInfo[PnIndex] & UdpNm_RootPtr->PnFilterMask[PnIndex]);
  }
}

#endif

#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==============================[end of file]===============================*/
