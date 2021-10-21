/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
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
 * The macro is used in function parameter declarations and definitions of structure member
 * where the number of parentheses matter.
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The object has internal linkage for readability improvement.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * In the context of expression "for (index = 0U; index < "max_element_number" ; ++index)"
 * when "max_element_number = 1" the message condition is always true is expected;
 * Expression "index < max_element_number" is always evaluated as true if ""max_element_number" = 1".
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * If number of ComMPnc = 0 and ComMModeLimitationEnabled = false and ComMWakeupInhibitionEnabled = false,
 * the message condition is always true is expected for TestSequences: ComM_ComTest_6, ComM_ComTest_2,
 * ComM_ComTest_8, ComM_ComTest_ResourceConsumption, ComM_ComTest_3;
 */

 /*==================[inclusions]============================================*/

#include <ComM_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <TSMem.h>

#include <ComM.h>               /* ComM API definitions (own interface) */
#include <ComM_BSW.h>           /* Module specific public APIs */

#if (COMM_NM_ACCESS_NEEDED == STD_ON)
#include <ComM_Nm.h>            /* NM callback APIs */
#endif

#if (COMM_DCM_ENABLED == STD_ON)
#include <ComM_Dcm.h>           /* DCM callback APIs */
#endif

#include <ComM_EcuMBswM.h>      /* ECUM and BSWM callback APIs */
#include <ComM_BusSm.h>         /* Bus State manager callback APIs */

#include <ComM_BSW_Cfg.h>      /* Module internal BSW specific interface */
#include <ComM_Int.h>           /* Internal Rte proxy function declarations */

#if (COMM_NVM_ENABLED == STD_ON)
/* !LINKSTO ComM506_Refine_NvM,1 */
#include <NvM.h>                /* Memory Access APIs */
#endif

/* !LINKSTO ComM506_Refine_CanSM,1 */
#if (COMM_CANSM_ACCESS_NEEDED == STD_ON)
#include <CanSM_ComM.h>        /* Can state manager */
#endif

/* !LINKSTO ComM506_Refine_FrSM,1 */
#if (COMM_FRSM_ACCESS_NEEDED == STD_ON)
#include <FrSM.h>              /* Flexray state manager */
#endif

/* !LINKSTO ComM506_Refine_LinSM,1 */
#if (COMM_LINSM_ACCESS_NEEDED == STD_ON)
#include <LinSM.h>             /* Lin state manager */
#endif
/* !LINKSTO ComM506_Refine_EthSM,1 */
#if (COMM_ETHSM_ACCESS_NEEDED == STD_ON)
#include <EthSM.h>             /* Eth state manager */
#endif

#if (COMM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO ComM506_Refine_Det,1 */
#include <Det.h>                /* Det API */
#endif

#if (COMM_INCLUDE_RTE == STD_ON)
#include <ComM_Rte_Cfg.h>  /* internal Rte config dependent header */
#endif

#include <SchM_ComM.h>

/* for the case COMM_INCLUDE_RTE == STD_OFF these symbols are provided via
 * ComM.h. */

/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef COMM_VENDOR_ID /* configuration check */
#error COMM_VENDOR_ID must be defined
#endif

#if (COMM_VENDOR_ID != 1U) /* vendor check */
#error COMM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef COMM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error COMM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (COMM_AR_RELEASE_MAJOR_VERSION != 4U)
#error COMM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef COMM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error COMM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (COMM_AR_RELEASE_MINOR_VERSION != 1U )
#error COMM_AR_RELEASE_MINOR_VERSION wrong (!= 1U)
#endif

#ifndef COMM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error COMM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (COMM_AR_RELEASE_REVISION_VERSION != 3U )
#error COMM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef COMM_SW_MAJOR_VERSION /* configuration check */
#error COMM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (COMM_SW_MAJOR_VERSION != 5U)
#error COMM_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef COMM_SW_MINOR_VERSION /* configuration check */
#error COMM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (COMM_SW_MINOR_VERSION < 19U)
#error COMM_SW_MINOR_VERSION wrong (< 19U)
#endif

#ifndef COMM_SW_PATCH_VERSION /* configuration check */
#error COMM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (COMM_SW_PATCH_VERSION < 7U)
#error COMM_SW_PATCH_VERSION wrong (< 7U)
#endif

/*--------------------------------------------------------------------------*/
/* !LINKSTO ComM103_Refine,1, ComM157,1, ComM.ComM140_Implicit,1 */
#if (defined COMM_PERS_DATA_MODIFIED)
#error COMM_PERS_DATA_MODIFIED already defined
#endif
#if ((COMM_NVM_ENABLED == STD_ON) &&                \
     ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)))
#define COMM_PERS_DATA_MODIFIED()                                      \
        ((void) NvM_SetRamBlockStatus(COMM_NVM_BLOCK_DESCRIPTOR, TRUE))
#else
#define COMM_PERS_DATA_MODIFIED()
#endif

/** \brief ID for the invalid event */
#if (defined COMM_EVENT_INVALID)
#error COMM_EVENT_INVALID already defined
#endif
#define COMM_EVENT_INVALID                  255U

/** \brief Set an event for a specific ComM channel */
#if (defined COMM_EMIT)
#error COMM_EMIT already defined
#endif
#define COMM_EMIT(a,b) (void)COMM_HSMEMITINST(&ComM_HsmScComM, a, b)

/** \brief Set an event for a specific ComM PN Channel */
#if (defined COMM_PNC_EMIT)
#error COMM_PNC_EMIT already defined
#endif
#define COMM_PNC_EMIT(a,b) (void)COMM_HSMEMITINST(&ComM_HsmScComMPnc, a, b)

/*--------------------------------------------------------------------------*/
#if (COMM_MULTICORE_ENABLED == STD_OFF)
#if (defined COMM_BUSSM_GET_CURRENT_COM_MODE)
#error COMM_BUSSM_GET_CURRENT_COM_MODE already defined
#endif
#if (COMM_NUM_USERS != 0U)
/** \brief Call wrapper for bus state manager function
 * <Bus>SM_GetCurrentComMode() */
#define COMM_BUSSM_GET_CURRENT_COM_MODE(channel, mode)                  \
  (ComM_BusSMGetCurrentComModeFpList[COMM_BUSTYPE_OF_CHANNEL(channel)]((channel), (mode)))
#endif
#endif
/*--------------------------------------------------------------------------*/

/* Macro for Det checks of channel identifier as function argument */
#if (defined COMM_IS_CHANNEL_INVALID)
#error COMM_IS_CHANNEL_INVALID already defined
#endif

#if (defined STEP_1_CHANNEL_INVALID)
#error STEP_1_CHANNEL_INVALID already defined
#endif

#if (defined STEP_2_CHANNEL_INVALID)
#error STEP_2_CHANNEL_INVALID already defined
#endif

#if (defined STEP_3_CHANNEL_INVALID)
#error STEP_3_CHANNEL_INVALID already defined
#endif

#if (defined STEP_4_CHANNEL_INVALID)
#error STEP_4_CHANNEL_INVALID already defined
#endif

#define STEP_1_CHANNEL_INVALID(Channel) ((Channel) >= COMM_NUM_CHANNELS)

#if (COMM_NM_VARIANT_NONE_NEEDED == STD_ON)
#define STEP_2_CHANNEL_INVALID(Channel) \
            (STEP_1_CHANNEL_INVALID(Channel) || (COMM_NM_VARIANT_OF_CHANNEL(Channel) == COMM_NM_NONE_VARIANT))
#else
#define STEP_2_CHANNEL_INVALID(Channel) STEP_1_CHANNEL_INVALID(Channel)
#endif

#if (COMM_NM_VARIANT_LIGHT_NEEDED == STD_ON)
#define STEP_3_CHANNEL_INVALID(Channel) \
            (STEP_2_CHANNEL_INVALID(Channel) || (COMM_NM_VARIANT_OF_CHANNEL(Channel) == COMM_NM_LIGHT_VARIANT))
#else
#define STEP_3_CHANNEL_INVALID(Channel) STEP_2_CHANNEL_INVALID(Channel)
#endif

#if (COMM_NM_VARIANT_SLAVE_ACTIVE_NEEDED == STD_ON)
#define STEP_4_CHANNEL_INVALID(Channel) \
            (STEP_3_CHANNEL_INVALID(Channel) || (COMM_NM_VARIANT_OF_CHANNEL(Channel) == COMM_NM_SLAVE_ACTIVE_VARIANT))
#else
#define STEP_4_CHANNEL_INVALID(Channel) STEP_3_CHANNEL_INVALID(Channel)
#endif

#if (COMM_NM_VARIANT_SLAVE_PASSIVE_NEEDED == STD_ON)
/* !LINKSTO SWS_ComM_00667.Refine1,1 */
/* !LINKSTO SWS_ComM_00667.Refine2,1 */
/* !LINKSTO SWS_ComM_00602.Refine1,1 */
/* !LINKSTO SWS_ComM_00602.Refine2,1 */
#define COMM_IS_CHANNEL_INVALID(Channel) \
            (STEP_4_CHANNEL_INVALID(Channel) || (COMM_NM_VARIANT_OF_CHANNEL(Channel) == COMM_NM_SLAVE_PASSIVE_VARIANT))
#else
/* !LINKSTO SWS_ComM_00667.Refine1,1 */
/* !LINKSTO SWS_ComM_00667.Refine2,1 */
/* !LINKSTO SWS_ComM_00602.Refine1,1 */
/* !LINKSTO SWS_ComM_00602.Refine2,1 */
#define COMM_IS_CHANNEL_INVALID(Channel) STEP_4_CHANNEL_INVALID(Channel)
#endif

#if (defined NOT_SLAVE_CHANNEL)
#error NOT_SLAVE_CHANNEL already defined
#endif
#define NOT_SLAVE_CHANNEL(Channel) ((Channel >= COMM_NUM_CHANNELS) || \
           ((COMM_NM_VARIANT_OF_CHANNEL(Channel) != COMM_NM_SLAVE_ACTIVE_VARIANT) && \
            (COMM_NM_VARIANT_OF_CHANNEL(Channel) != COMM_NM_SLAVE_PASSIVE_VARIANT) \
           ) \
          )

#if (defined COMM_IS_RX_EIRA)
#error COMM_IS_RX_EIRA already defined
#endif
#define COMM_IS_RX_EIRA 0U

#if (defined COMM_IS_RX_ACTIVE_ERA)
#error COMM_IS_RX_ACTIVE_ERA already defined
#endif
#define COMM_IS_RX_ACTIVE_ERA 1U

#if (defined COMM_IS_RX_UNFILTERED_ERA)
#error COMM_IS_RX_UNFILTERED_ERA already defined
#endif
#define COMM_IS_RX_UNFILTERED_ERA 2U
/*==================[type definitions]======================================*/

/** \brief Function pointer type for the bus state manager functions to
 ** query the current communication mode. */
typedef P2FUNC(Std_ReturnType, COMM_APPL_CODE, ComM_BusSMGetCurrentComModeType)
(
  NetworkHandleType                       Channel,
  P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) ComMode
);


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if (COMM_PNC_SUPPORT == STD_ON)
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/** \brief Updates Rx(EIRA/ERA) arrays
 **
 ** This functions reads the RX signal values from Com and updates RxEira or RxEra arrays
 **
 ** \param[in,out] SignalStatusPtr notification if EIRA or ERA RX signals where updated
 ** \param[in]     ComM_RxSignal_Struct_Type List of EIRA or ERA RX signals
 ** \param[in,out] SignalMergedPtr received EIRA or ERA merged (RxEira, RxEra)
 ** \param[in]     SignaleKind Is EIRA or ERA active or ERA passive signal set is processed (EIRA if isEira is set to true)
 */
STATIC FUNC(void, COMM_CODE) ComM_ProcessPncComSignals
(
  P2VAR(boolean, AUTOMATIC, COMM_APPL_DATA) SignalStatusPtr,
  P2CONST(ComM_RxSignal_Struct_Type, AUTOMATIC, COMM_APPL_CONST) SignalConfigurationPtr,
  P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) SignalMergedPtr,
  uint8 SignalKind
);
/** \brief Handle EcuM PNC wakeup indication
 **
 ** This functions handle PNC wakeup indication following a EcuM PNC wakeup request.
 **
 ** \param[in] Index of the PNC for which wakeup indication was received.
 */
STATIC FUNC(void,COMM_CODE) ComM_PNCWakeUpIndication_PncHandle(uint8 pncIdx);

/** \brief Handle EcuM PNC wakeup indication.
 **
 ** This functions handle channels passive wakeup, following a EcuM PNC wakeup request.
 **
 ** \param[in] Index of the PNC for which wakeup indication was received.
 */
STATIC FUNC(void,COMM_CODE) ComM_PNCWakeUpIndication_ChannelHandle(uint8 pncIdx);

#if (COMM_MULTICORE_ENABLED == STD_ON)
#if (COMM_NO_OF_TX_BUFFERS != 0)
/** \brief Call Schm functions for sending PNC information from Mater to Slave core */
STATIC FUNC(void, COMM_CODE) ComM_ProcessTxSignals(void);
#endif
#endif
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
#endif
/*==================[external constants]====================================*/

#define COMM_START_SEC_CONST_8
#include <ComM_MemMap.h>

#if (COMM_BUSTYPE_OF_CHANNEL_DYNAMIC == STD_ON)
CONST(uint8, COMM_CONST) ComM_BusTypeOfChannel[COMM_NUM_CHANNELS]
  = COMM_BUSTYPE_OF_CHANNEL_INIT;
#endif

#if (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_ON)
CONST(uint8, COMM_CONST) ComM_NmVariantOfChannel[COMM_NUM_CHANNELS]
  = COMM_NM_VARIANT_OF_CHANNEL_INIT;
#endif

#define COMM_STOP_SEC_CONST_8
#include <ComM_MemMap.h>

/*==================[internal constants]====================================*/
#if (COMM_MULTICORE_ENABLED == STD_OFF)
#if (COMM_NUM_USERS != 0U)

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/** \brief Stores function pointer for bus specific state managers **/
/* Deviation MISRAC2012-2 */
STATIC CONST(ComM_BusSMGetCurrentComModeType, COMM_CONST)
  ComM_BusSMGetCurrentComModeFpList[5] =
{
#if (COMM_CANSM_ACCESS_NEEDED == STD_ON)
  /* !LINKSTO ComM855,1 */
  &CanSM_GetCurrentComMode,      /* COMM_BUS_TYPE_CAN = 0 */
#else
  NULL_PTR,                /* never accessed if no CAN channel is available */
#endif
#if (COMM_ETHSM_ACCESS_NEEDED == STD_ON)
  /* !LINKSTO ComM860,1 */
  &EthSM_GetCurrentComMode,      /* COMM_BUS_TYPE_ETH = 1 */
#else
  NULL_PTR,                /* never accessed if no CAN channel is available */
#endif
#if (COMM_FRSM_ACCESS_NEEDED == STD_ON)
  /* !LINKSTO ComM853,1 */
  &FrSM_GetCurrentComMode,       /* COMM_BUS_TYPE_FR = 2 */
#else
  NULL_PTR,                 /* never accessed if no FR channel is available */
#endif
  NULL_PTR,                   /* COMM_BUS_TYPE_INTERNAL = 3 */
#if (COMM_LINSM_ACCESS_NEEDED == STD_ON)
  /* !LINKSTO ComM857,1 */
  &LinSM_GetCurrentComMode      /* COMM_BUS_TYPE_LIN = 4 */
#else
  NULL_PTR                /* never accessed if no LIN channel is available */
#endif
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#endif
#endif

/*==================[external data]=========================================*/

#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

#if (COMM_DCM_ENABLED == STD_ON)
VAR(uint8, COMM_VAR)         ComM_DcmActiveStatus[COMM_NUM_CHANNELS_BYTES];
#endif /* if (COMM_DCM_ENABLED == STD_ON) */

VAR(uint8, COMM_VAR) ComM_Communication_Allowed[COMM_NUM_CHANNELS_BYTES];

VAR(ComM_ASR40_ModeType, COMM_VAR)   ComM_RequestedComMode[COMM_NUM_CHANNELS];

#if (COMM_PNC_GW_ENABLED == STD_ON)
VAR(ComM_ASR40_ModeType, COMM_VAR)   ComM_RequestedComModeWithoutPnc[COMM_NUM_CHANNELS];
#endif /* COMM_PNC_GW_ENABLED == STD_ON) */

VAR(uint8, COMM_VAR) ComM_PassiveStartupStatus[COMM_NUM_CHANNELS_BYTES];

VAR(ComM_ASR40_ModeType, COMM_VAR)   ComM_ChannelMode[COMM_NUM_CHANNELS];

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
VAR(ComM_ASR40_ModeType, COMM_VAR)   ComM_ChannelModePrevious[COMM_NUM_CHANNELS];
#endif

VAR(ComM_StateType, COMM_VAR)  ComM_ChanState[COMM_NUM_CHANNELS];

VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_UserRequestedComMode[USER_ARRAY_SIZE];


#if (COMM_PNC_SUPPORT == STD_ON)
VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_PNCRequestedComMode[COMM_NUM_PNC];

VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_PNCMode[COMM_NUM_PNC];

#if (COMM_0_PNC_VECTOR_AVOIDANCE == STD_ON)
VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_PNCRequestedComMode0PncVectorAvoidance[COMM_NUM_PNC];
#endif

/* !LINKSTO ComM982,1 */
VAR(uint8, COMM_VAR) ComM_TxEiraEraActive[COMM_PN_INFO_LENGTH];
VAR(uint8, COMM_VAR) ComM_RxEira[COMM_PN_INFO_LENGTH];
#if(COMM_PNC_GW_ENABLED == STD_ON)
VAR(uint8, COMM_VAR) ComM_RxActiveEra[COMM_PN_INFO_LENGTH];
VAR(uint8, COMM_VAR) ComM_RxUnfilteredEra[COMM_PN_INFO_LENGTH];
VAR(uint8, COMM_VAR) ComM_TxEraPassive[COMM_PN_INFO_LENGTH];
#endif

VAR(boolean, COMM_VAR) ComM_RxEiraSignalStatus;
#if(COMM_PNC_GW_ENABLED == STD_ON)
VAR(boolean, COMM_VAR) ComM_RxActiveEraSignalStatus;
VAR(boolean, COMM_VAR) ComM_RxUnfilteredEraSignalStatus;
VAR(ComM_PncModeType, COMM_VAR)   ComM_PncStateGWE[COMM_PN_INFO_LENGTH];
#endif

#if (COMM_MULTICORE_ENABLED == STD_ON)
#if (COMM_NO_OF_TX_BUFFERS != 0)
VAR(uint8, COMM_VAR) ComM_MulticoreTxBuffer[COMM_NO_OF_TX_BUFFERS][COMM_PN_INFO_LENGTH];
VAR(boolean, COMM_VAR) ComM_MulticoreTxBufferStatus[COMM_NO_OF_TX_BUFFERS];
#endif /* #if (COMM_NO_OF_TX_BUFFERS != 0) */
#if (COMM_NO_OF_RX_BUFFERS != 0)
VAR(uint8, COMM_VAR) ComM_MulticoreRxBuffer[COMM_NO_OF_RX_BUFFERS][COMM_PN_INFO_LENGTH];
#endif /* #if (COMM_NO_OF_TX_BUFFERS != 0) */
#endif /* #if (COMM_MULTICORE_ENABLED == STD_ON) */
#endif /* if (COMM_PNC_SUPPORT == STD_ON) */

#if (COMM_INCLUDE_RTE == STD_ON)
VAR(boolean, COMM_VAR) ComM_RteNotificationSuccessful[USER_ARRAY_SIZE];

#endif /* #if (COMM_INCLUDE_RTE == STD_ON) */

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

#define COMM_START_SEC_VAR_INIT_8
#include <ComM_MemMap.h>

VAR(ComM_InitStatusType, COMM_VAR) ComM_InitStatus = COMM_UNINIT;

#define COMM_STOP_SEC_VAR_INIT_8
#include <ComM_MemMap.h>


#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))

#define COMM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

VAR(ComM_NvDataType, COMM_VAR_POWER_ON_CLEARED) ComM_EB_NvRAM;

#define COMM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

#endif

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

P2CONST(ComM_ConfigType,AUTOMATIC,COMM_APPL_CONST) ComM_ConfigPtr;

#if ((COMM_MULTICORE_ENABLED == STD_ON) && (COMM_NM_ACCESS_NEEDED == STD_ON))
VAR(Nm_ModeType, COMM_VAR) ComM_LastNmMode[COMM_NUM_CHANNELS];
#endif

VAR(ComM_DoActionType, COMM_VAR) ComM_RteNotificationDoActFkp;

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal data]=========================================*/

#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief Array to store NoCom Inhibition status of ComM Channels. */
STATIC VAR(uint8, COMM_VAR) ComM_ChannelNoComInhibition[COMM_NUM_BYTES_NVM];
#endif

#if(COMM_PNC_SUPPORT == STD_ON)
/* Deviation MISRAC2012-2 */
STATIC volatile VAR(boolean,COMM_VAR) ComMPncMainfunctionRunning;
#endif


#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/** \brief Array to store WakeUp Inhibition status of ComM Channels. */
STATIC VAR(uint8, COMM_VAR) ComM_ChannelWakeUpInhibition[COMM_NUM_BYTES_NVM];
#endif

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

/*==================[external function definitions]=========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/*------------------[ComM_Init]----------------------------------------------*/
/* !LINKSTO ComM146,1, ComM927,1 */
FUNC(void, COMM_CODE) ComM_Init
(
  P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) ConfigPtr
)
{
  uint8_least i;
  P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) LocalConfigPtr = ConfigPtr;

  DBG_COMM_INIT_ENTRY(ConfigPtr);

  /* Post Build Initializations */
/* !LINKSTO ComM.PB.Impl.PbCfgM,1 */
#if ( COMM_PBCFGM_SUPPORT_ENABLED == STD_ON )
  if (LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if (E_OK == PbcfgM_GetConfig( COMM_MODULE_ID, COMM_INSTANCE_ID, &ModuleConfig))
    {
      LocalConfigPtr = (P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST)) ModuleConfig;
    }
  }
#endif

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* Check whether parameter LocalConfigPtr is a NULL_PTR or invalid */
  if (LocalConfigPtr == NULL_PTR)
  {
    /* Found a NULL_PTR, report to DET */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_INIT, COMM_E_WRONG_PARAMETERS);
  }
  else if (E_OK != ComM_IsValidConfig(LocalConfigPtr))
  {
    /* Check that configuration pointer is valid. */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_INIT, COMM_E_WRONG_PARAMETERS);
  }
  else
#else
  /* !LINKSTO ComM.EB.ComM_Init_NULL_PTR,1 */
  if(ComM_IsValidConfig(LocalConfigPtr) == E_OK)
#endif /* COMM_DEV_ERROR_DETECT */
  {
    /* Initialize module */

    /* Set the pointer to the post-build configuration */
    ComM_ConfigPtr = LocalConfigPtr;

    /* Pre Compile Initializations */
    /* Deviation TASKING-1 */
    for (i = 0U; i < COMM_NUM_CHANNELS; ++i)
    {
      /* Requested communication mode for each channel */
      /* !LINKSTO ComM485,1 */
      ComM_RequestedComMode[i]    = COMM_NO_COMMUNICATION;
      /* Initialize ComM Channel State */
      ComM_ChanState[i]          = COMM_NO_COM_NO_PENDING_REQUEST;
      /* Initialize ComM Channel Mode */
      ComM_ChannelMode[i]         = COMM_NO_COMMUNICATION;
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
      ComM_ChannelModePrevious[i] = COMM_NO_COMMUNICATION;
#endif
#if (COMM_PNC_GW_ENABLED == STD_ON)
      ComM_RequestedComModeWithoutPnc[i] = COMM_NO_COMMUNICATION;
#endif /* COMM_PNC_GW_ENABLED == STD_ON) */
      /* The persistent storage is supported we don't
       * to react on a NvM callback after the initialization as the
       * the Read All and Write All will called in EcuM_Init(). */
    }
    /* !LINKSTO ComM313,1 */
    /* Requested communication mode of each user */
    TS_MemSet(ComM_UserRequestedComMode, COMM_NO_COMMUNICATION, USER_ARRAY_SIZE);

    /* Initialize ComM state machine, perform initial transitions */
    ComM_HsmInit(&ComM_HsmScComM);

    /* Initializing the Channel communication status and Dcm request status */
    /* Deviation TASKING-1 */
    for (i = 0U; i < COMM_NUM_CHANNELS_BYTES; ++i)
    {
      ComM_Communication_Allowed[i] = 0U;

#if (COMM_DCM_ENABLED == STD_ON)
      ComM_DcmActiveStatus[i] = 0U;
#endif
      ComM_PassiveStartupStatus[i] = 0U;
    }

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))

  /* The persistent storage is supported we don't need
   * to react on a NvM callback after the initialization as the
   * the Read All and Write All will be called in EcuM_Init().
   */

/* !LINKSTO ComM864,1 */
#if (COMM_NVM_ENABLED == STD_OFF)
    /* NvM storage disabled, re-initialize RAM data */
    ComM_EB_NvRAM = ComM_EB_NvROM;
#endif

    /* Initializing the Channel Inhibition status */
    /* Deviation TASKING-1 */
    for (i = 0U; i < COMM_NUM_BYTES_NVM; ++i)
    {

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
      ComM_ChannelNoComInhibition[i] = ComM_ChannelInhibitionStatusInit[i];
#endif

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
      ComM_ChannelWakeUpInhibition[i] = ComM_EB_NvRAM.ChannelWakeUpInhibition[i];
#endif
    }
#endif
#if (COMM_PNC_SUPPORT == STD_ON)
    if( COMM_PNC_ENABLED )
    {
      /* Initializing the pnc */
      ComM_HsmInit(&ComM_HsmScComMPnc);

      /* Deviation TASKING-1 */
      for (i = 0U; i < COMM_PN_INFO_LENGTH; i++)
      {
#if(COMM_PNC_GW_ENABLED == STD_ON)
        ComM_RxActiveEra[i] = 0U;
        ComM_RxUnfilteredEra[i] = 0U;
        ComM_TxEraPassive[i] = 0U;
#endif
        ComM_RxEira[i] = 0U;
        ComM_TxEiraEraActive[i] = 0U;
      }

      ComM_RxEiraSignalStatus = FALSE;
#if(COMM_PNC_GW_ENABLED == STD_ON)
      ComM_RxActiveEraSignalStatus = FALSE;
      ComM_RxUnfilteredEraSignalStatus = FALSE;
      /* Deviation TASKING-1 */
      for (i = 0U; i < COMM_PN_INFO_LENGTH; ++i)
      {
        ComM_PncStateGWE[i] = 0U;
      }
#endif

      TS_MemSet(ComM_PNCRequestedComMode, COMM_NO_COMMUNICATION, COMM_NUM_PNC * sizeof(ComM_ASR40_ModeType));
      TS_MemSet(ComM_PNCMode, COMM_NO_COMMUNICATION, COMM_NUM_PNC * sizeof(ComM_ASR40_ModeType));

#if(COMM_0_PNC_VECTOR_AVOIDANCE == STD_ON)
      TS_MemSet(ComM_PNCRequestedComMode0PncVectorAvoidance, COMM_NO_COMMUNICATION, COMM_NUM_PNC * sizeof(ComM_ASR40_ModeType));
#endif

#if (COMM_MULTICORE_ENABLED == STD_ON)
#if (COMM_NO_OF_TX_BUFFERS != 0)
      for(i = 0U; i < COMM_NO_OF_TX_BUFFERS; i++)
      {
        uint8 j = 0U;
        for(j = 0U; j < COMM_PN_INFO_LENGTH; j++)
        {
          ComM_MulticoreTxBuffer[i][j] = 0U;
        }
        ComM_MulticoreTxBufferStatus[i] = FALSE;
      }
#endif /* #if (COMM_NO_OF_TX_BUFFERS != 0) */
#if (COMM_NO_OF_RX_BUFFERS != 0)
      for(i = 0U; i < COMM_NO_OF_RX_BUFFERS; i++)
      {
        uint8 j = 0U;
        for(j = 0U; j < COMM_PN_INFO_LENGTH; j++)
        {
          ComM_MulticoreRxBuffer[i][j] = 0U;
        }
      }
#endif
#if (COMM_NM_ACCESS_NEEDED == STD_ON)
      for(i=0U;i<COMM_NUM_CHANNELS;i++)
      {
        ComM_LastNmMode[i] = NM_MODE_BUS_SLEEP;
      }
#endif
#endif /* #if (COMM_MULTICORE_ENABLED == STD_ON) */
    }
    ComMPncMainfunctionRunning = FALSE;
#endif

#if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))
    /* Deviation TASKING-1 */
    for (i = 0U; i < USER_ARRAY_SIZE; ++i)
    {
      ComM_RteNotificationSuccessful[i] = TRUE;
#if (COMM_ENABLE_ASR32_SERVICE_API == STD_ON)
      ComM_ASR32_RteModeUser[i] = COMM_NO_COMMUNICATION;
#endif
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
      ComM_ASR40_RteModeUser[i] = COMM_NO_COMMUNICATION;
#endif
#if (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE)
      ComM_RteModeUser[i] = COMM_NO_COMMUNICATION;
#endif
    }
#endif /* #if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))*/

    ComM_RteNotificationDoActFkp = ComM_RteNotificationDoAction;

    /* Set initialization status in global variable */
    ComM_InitStatus      = COMM_INIT;
  }

  DBG_COMM_INIT_EXIT(ConfigPtr);
}

/*------------------[ComM_DeInit]--------------------------------------------*/
/* !LINKSTO ComM147,1, ComM794,1 */
FUNC(void, COMM_CODE) ComM_DeInit(void)
{
  uint8_least i;

  DBG_COMM_DEINIT_ENTRY();

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_DEINIT, COMM_E_NOT_INITED);
  }
  else
#endif
  {
    /* Check whether all the channels are in the 'NoCommunication' state */
    /* Deviation TASKING-1 */
    for (i = 0U; i < COMM_NUM_CHANNELS; i++)
    {
      /* get locally cached com mode, the BusSm of channels of BusType=Internal
       * cannot be queried */
      if (ComM_ChanState[i] != COMM_NO_COM_NO_PENDING_REQUEST)
      {
        break;
      }
    }

    /* De-init module only if all channels are in no communication state */
    if (COMM_NUM_CHANNELS == i)
    {
      /* deinit module global init status variable */
      ComM_InitStatus = COMM_UNINIT;
    }
  }

  DBG_COMM_DEINIT_EXIT();
}

/*------------------[ComM_GetState]-----------------------------------------*/
/* !LINKSTO ComM872,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetState
(
  NetworkHandleType                                Channel,
  P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_DATA) State
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_GETSTATE_ENTRY(Channel,State);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_STATE, COMM_E_NOT_INITED);
    /* !LINKSTO ComM649_Refine_COMM_E_UNINIT,1 */
    RetVal = COMM_E_UNINIT;
  }
  else if ((NULL_PTR == State) || (Channel >= COMM_NUM_CHANNELS))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_STATE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_PRECONDITION_ASSERT_ENABLED == STD_ON)
    COMM_PRECONDITION_ASSERT((ComM_ChanState[Channel] <= COMM_SILENT_COM),COMM_SID_GET_STATE);
#endif

    *State = ComM_ChanState[Channel];
    RetVal = E_OK;
  }

  DBG_COMM_GETSTATE_EXIT(RetVal,Channel,State);

  return RetVal;
}

/*------------------[ComM_GetStatus]-----------------------------------------*/
/* !LINKSTO ComM242,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus
(
  P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_GETSTATUS_ENTRY(Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == Status)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_STATUS, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {

#if (COMM_PRECONDITION_ASSERT_ENABLED == STD_ON)
    COMM_PRECONDITION_ASSERT((ComM_InitStatus <= COMM_INIT),COMM_SID_GET_STATUS);
#endif
    *Status = ComM_InitStatus;
    RetVal = E_OK;
  }

  DBG_COMM_GETSTATUS_EXIT(RetVal,Status);

  return RetVal;
}

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_GetInhibitionStatus]-------------------------*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM107,1, ComM488,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetInhibitionStatus
(
  NetworkHandleType                                           Channel,
  P2VAR(ComM_ASR32_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_GETINHIBITIONSTATUS_ENTRY(Channel,Status);

  RetVal = ComM_ASR40_GetInhibitionStatus(Channel, Status);

  DBG_COMM_ASR32_GETINHIBITIONSTATUS_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_GetInhibitionStatus]-------------------------------*/
/* !LINKSTO ComM619,1, ComM488,1, ComM182,1 */
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetInhibitionStatus
(
  NetworkHandleType                                           Channel,
  P2VAR(ComM_ASR40_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  ComM_ASR40_InhibitionStatusType ChannelInhibitionStatus = 0U;

  DBG_COMM_ASR40_GETINHIBITIONSTATUS_ENTRY(Channel,Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_INHIBITION_STATUS, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if ((NULL_PTR == Status) || (Channel >= COMM_NUM_CHANNELS))
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_GET_INHIBITION_STATUS, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    /* assemble bit encoded channel inhibition status */
    if (TRUE == ComM_ChannelWakeUpInhibitionStatus(Channel))
    {
      ChannelInhibitionStatus |= COMM_PREVENT_WAKEUP;
    }
#endif
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
    if (TRUE == ComM_ChannelNoComInhibitionStatus(Channel))
    {
      ChannelInhibitionStatus |= COMM_LIMIT_NOCOMM;
    }
#endif

    *Status = ChannelInhibitionStatus;
    RetVal  = E_OK;
  }

  DBG_COMM_ASR40_GETINHIBITIONSTATUS_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_RequestComMode]------------------------------*/

/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_RequestComMode
(
  ComM_ASR32_UserHandleType User,
  ComM_ASR32_ModeType       ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_REQUESTCOMMODE_ENTRY(User,ComMode);

  RetVal = ComM_ASR40_RequestComMode(User, ComMode);

  DBG_COMM_ASR32_REQUESTCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}
#endif

/*------------------[ComM_RequestComMode]------------------------------------*/
/* !LINKSTO ComM110,1, ComM.ASR32.ComM110,1, ComM978,1, ComM972,1, ComM218_Refine_NOCOM,1 */
/* !LINKSTO ComM218_Refine_SILENTCOM,1, ComM848,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_RequestComMode
(
  ComM_ASR40_UserHandleType User,
  ComM_ASR40_ModeType       ComMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (COMM_NUM_USERS != 0U)
  uint8_least ChannelPos;
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
  uint16 InhibitionCounter;
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
  uint8_least PncPos;
#endif
#endif

  DBG_COMM_ASR40_REQUESTCOMMODE_ENTRY(User,ComMode);
#if (COMM_NUM_USERS == 0U)
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(ComMode);
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_REQUEST_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_REQUEST_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
#endif
#else /* #if (COMM_NUM_USERS == 0U) */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_REQUEST_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if (User >= COMM_NUM_USERS)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_REQUEST_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else if ((ComMode != COMM_NO_COMMUNICATION) && (ComMode != COMM_FULL_COMMUNICATION))
  {
    /* !LINKSTO ComM.DetReport.WrongComModeReq,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_REQUEST_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    RetVal = E_OK;
    /* The SchM functions can't be called direct because it's not possible
       to include the Rte files here. Thus the SchM wrapper functions from
       the Rte specific compilation unit must be called. */
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    /* store user requested mode */
    ComM_UserRequestedComMode[User] = ComMode;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

#if (COMM_PNC_SUPPORT == STD_ON)
    if( COMM_PNC_ENABLED )
    {
      /* Get all pnc for a specific user and set ComMode  */
      for (PncPos = 0U; PncPos < COMM_NUM_PNCS_OF_USER(User); PncPos++)
      {
        const uint8 Pnc =COMM_PNC_OF_USER(User,PncPos);
       /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_RequestComMode_NO_COM_REQ_BY_ALL_COMM_USER,1 */
        ComM_EvaluatePnc(Pnc, ComMode);
      }
    }
#endif

    /* Get all channels for a specific user and set ComMode  */
    for (ChannelPos = 0U; ChannelPos < COMM_NUM_CHANNELS_OF_USER(User); ChannelPos++)
    {
      const uint8 Channel = COMM_CHANNEL_OF_USER(User,ChannelPos);
      boolean EmitEventReqStatusChanged = TRUE;
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)  || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
      boolean InhibitionStatus = FALSE;
#endif
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
      ComM_StateType ComM_ChanState_local = ComM_ChanState[Channel];
#endif
      if (COMM_FULL_COMMUNICATION == ComM_UserRequestedComMode[User])
      {
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        if (TRUE == ComM_ChannelNoComInhibitionStatus(Channel))
        {
          InhibitionStatus = TRUE;
        }
#endif
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        if ((TRUE == ComM_ChannelWakeUpInhibitionStatus(Channel)) &&
            /* !LINKSTO ComM842,1 */
            ((COMM_FULL_COM_NETWORK_REQUESTED >  ComM_ChanState_local) ||
             (COMM_SILENT_COM                 == ComM_ChanState_local)
            )
           )
        {
          InhibitionStatus = TRUE;
        }
#endif
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
        /* Inhibit user request if "Limit to No Comm Mode" is on OR "Wake up
         * inhibition" is on, AND current user state is No Comm */
        if (TRUE == InhibitionStatus)
        {
          /* Return COMM_E_MODE_LIMITATION when inhibition is enabled for
           * any channel of a user */
          /* !LINKSTO ComM649_Refine_COMM_E_MODE_LIMITATION,1 */
          RetVal = COMM_E_MODE_LIMITATION;

          ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
          InhibitionCounter = ComM_EB_NvRAM.InhibitionCounter;
          ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

          if(InhibitionCounter < 0xFFFFU)
          {
            /* Count the number of FULL Communication requests ignored */
            ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
            ComM_EB_NvRAM.InhibitionCounter++;
            ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
            /* Set NvRAM modified */
            COMM_PERS_DATA_MODIFIED();
          }
          /* Do not emit event 'REQ_STATUS_CHANGED' if inhibition is active */
          EmitEventReqStatusChanged = FALSE;
        }
        else
#endif
        {
           /* Do Nothing */
        }
      }
      else
      {
        /* Do Nothing */
      }
      ComM_EvaluateChannel(Channel);
      /* Emit event 'REQ_STATUS_CHANGED' if request is accepted
       * and the user is directly mapped to
       * the channel. If the user is indirectly mapped to this channel via a PNC, it is up to the
       * PNC to request the appropriate mode.
       * The channels in the array ComM_ChannelsOfUser are ordered, so that directly mapped channels
       * are located before indirectly (via PNC) mapped channels.
       */
      /* Deviation TASKING-2 */
      if ((TRUE == EmitEventReqStatusChanged) && (COMM_DIRECT_NUM_CHANNELS_OF_USER(User) > ChannelPos))
      {
        /* Set Event REQ_STATUS_CHANGED */
        /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_RequestComMode_REQ_STATUS_CHANGED,1 */
        COMM_EMIT(Channel, COMM_HSM_COMM_EV_REQ_STATUS_CHANGED);
      }
    }
  }
#endif /* #if (COMM_NUM_USERS == 0U) */
  DBG_COMM_ASR40_REQUESTCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_GetMaxComMode]-------------------------------*/

/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetMaxComMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_GETMAXCOMMODE_ENTRY(User,ComMode);

  RetVal = ComM_ASR40_GetMaxComMode(User, ComMode);

  DBG_COMM_ASR32_GETMAXCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}
#endif

/*------------------[ComM_GetMaxComMode]-------------------------------------*/
/* !LINKSTO ComM686,1, ComM085,1, ComM374,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetMaxComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (COMM_NUM_USERS != 0U)
  uint8_least i;
  uint8 Channel;
  boolean comAllowed = FALSE;
#endif

  DBG_COMM_ASR40_GETMAXCOMMODE_ENTRY(User,ComMode);
#if (COMM_NUM_USERS == 0U)
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(ComMode);
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    /* !LINKSTO ComM858,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_MAX_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_MAX_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
#endif
#else /* #if (COMM_NUM_USERS == 0U) */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    /* !LINKSTO ComM858,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_MAX_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if ((NULL_PTR == ComMode) || (User >= COMM_NUM_USERS))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_MAX_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    ComM_StateType ComM_ChanState_local;
    /* Check for limitations, see extended functionality */
    *ComMode = COMM_FULL_COMMUNICATION;

    /* Get all channels for a specific user and check for
     * limitations/inhibitions */
    for (i = 0U; i < COMM_NUM_CHANNELS_OF_USER(User); i++)
    {
      boolean NoComInhibitionStatus = FALSE;
      boolean WkUpInhibitionStatus = FALSE;

      Channel = COMM_CHANNEL_OF_USER(User,i);
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
        if (TRUE == ComM_ChannelNoComInhibitionStatus(Channel))
        {
          NoComInhibitionStatus = TRUE;
        }
#endif
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
        /* !LINKSTO ComM219,1 */
        if (TRUE == ComM_ChannelWakeUpInhibitionStatus(Channel))
        {
          WkUpInhibitionStatus = TRUE;
        }
#endif
      ComM_ChanState_local = ComM_ChanState[Channel];
      comAllowed = ComM_BitArrayIsBitSet(ComM_Communication_Allowed, Channel);
      /* If any one channel has inhibition on and currently is in No Com
       * mode, set the lowest possible mode */
      if (((COMM_NO_COM_NO_PENDING_REQUEST == ComM_ChanState_local) ||
           (COMM_NO_COM_REQUEST_PENDING    == ComM_ChanState_local)
          ) &&
          ((TRUE  == NoComInhibitionStatus                         ) ||
           (TRUE  == WkUpInhibitionStatus                          ) ||
           (FALSE == comAllowed                                    )
          )
         )
      {
        *ComMode = COMM_NO_COMMUNICATION;
        break;
      }
      /* If current state of channel is Silent Com and inhibition is active,
       * the max available mode for user is Silent Communication */
      else if ((COMM_SILENT_COM == ComM_ChanState[Channel]                       ) &&
               ((TRUE == NoComInhibitionStatus) || (TRUE == WkUpInhibitionStatus))
              )
      {
        *ComMode = COMM_SILENT_COMMUNICATION;
      }
      else
      {
        /* do nothing */
      }
    }

    RetVal = E_OK;
  }
#endif /* #if (COMM_NUM_USERS == 0U) */
  DBG_COMM_ASR40_GETMAXCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_GetRequestedComMode]-------------------------*/

/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetRequestedMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_GETREQUESTEDMODE_ENTRY(User,ComMode);

  RetVal = ComM_ASR40_GetRequestedComMode(User, ComMode);

  DBG_COMM_ASR32_GETREQUESTEDMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}
#endif

/*------------------[ComM_GetRequestedComMode]-------------------------------*/
/* !LINKSTO ComM500,1, ComM079,1, ComM625,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetRequestedComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_GETREQUESTEDCOMMODE_ENTRY(User,ComMode);
#if (COMM_NUM_USERS == 0U)
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(ComMode);
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_REQUESTED_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_GET_REQUESTED_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
#endif
#else /* #if (COMM_NUM_USERS == 0U) */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_REQUESTED_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if ((NULL_PTR == ComMode) || (User >= COMM_NUM_USERS))
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_GET_REQUESTED_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_PRECONDITION_ASSERT_ENABLED == STD_ON)
  {
    uint8 max_mode = COMM_MAX(COMM_MAX(COMM_NO_COMMUNICATION,COMM_SILENT_COMMUNICATION),COMM_FULL_COMMUNICATION);
    COMM_PRECONDITION_ASSERT((ComM_UserRequestedComMode[User] <= max_mode ),COMM_SID_GET_REQUESTED_COM_MODE);
  }
#endif
    *ComMode = ComM_UserRequestedComMode[User];
    RetVal = E_OK;
  }
#endif /* #if (COMM_NUM_USERS == 0U) */
  DBG_COMM_ASR40_GETREQUESTEDCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_GetCurrentComMode]---------------------------*/

/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetCurrentComMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_GETCURRENTCOMMODE_ENTRY(User,ComMode);

  RetVal = ComM_ASR40_GetCurrentComMode(User, ComMode);

  DBG_COMM_ASR32_GETCURRENTCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

#endif

/*------------------[ComM_GetCurrentComMode]---------------------------------*/
/* !LINKSTO ComM083,1, ComM084,1, ComM176,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetCurrentComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if (COMM_NUM_USERS != 0U)
  uint8_least    i;
#endif

  DBG_COMM_ASR40_GETCURRENTCOMMODE_ENTRY(User,ComMode);
#if (COMM_NUM_USERS == 0U)
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(ComMode);
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    /* !LINKSTO ComM234_Refine_WRONG_PARAMETERS,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
  {
    /* !LINKSTO ComM234_Refine_WRONG_PARAMETERS,1 */
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
#endif
#else /* #if (COMM_NUM_USERS == 0U) */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    /* !LINKSTO ComM234_Refine_WRONG_PARAMETERS,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if ((NULL_PTR == ComMode) || (User >= COMM_NUM_USERS))
  {
    /* !LINKSTO ComM234_Refine_WRONG_PARAMETERS,1 */
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {

    ComM_ASR40_ModeType localComMode;
    ComM_ASR40_ModeType ResultComMode = COMM_FULL_COMMUNICATION;

    /* Get all channels for a specific user and set ComMode  */
    for (i = 0U; i < COMM_NUM_CHANNELS_OF_USER(User); i++)
    {
      const uint8 channel = COMM_CHANNEL_OF_USER(User,i);
      /* get locally cached com mode, the BusSm of channels of BusType=Internal
       * cannot be queried */
#if (COMM_MULTICORE_ENABLED == STD_OFF)
#if (COMM_BUSTYPE_INTERNAL_NEEDED == STD_ON)
      if (COMM_BUSTYPE_OF_CHANNEL(channel) == COMM_BUS_TYPE_INTERNAL)
      {

        localComMode = ComM_ChannelMode[channel];
        RetVal = E_OK;
      }
      else
#endif
      {
        RetVal = COMM_BUSSM_GET_CURRENT_COM_MODE(channel, &localComMode);
#if (COMM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
        COMM_POSTCONDITION_ASSERT((E_OK == RetVal),COMM_SID_GET_CURRENT_COM_MODE);
#endif
        if( RetVal != E_OK)
        {
          break;
        }
      }
#else /* (COMM_MULTICORE_ENABLED == STD_OFF) */
      localComMode = ComM_ChannelMode[channel];
      RetVal = E_OK;
#endif
      /* get the lowest communication mode possible */
      if (localComMode < ResultComMode)
      {
        ResultComMode = localComMode;
      }
    }
    if (E_OK == RetVal)
    {
      *ComMode = ResultComMode;
    }
  }
#endif /* #if (COMM_NUM_USERS == 0U) */
  DBG_COMM_ASR40_GETCURRENTCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

/* !LINKSTO ComM.EB.ComM_GetCurrentPNCComMode,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetCurrentPNCComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_ENTRY(User,ComMode);

#if (COMM_NUM_USERS == 0U)
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(ComMode);
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_PNC_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_PNC_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
#endif /* #if (COMM_DEV_ERROR_DETECT == STD_ON) */
#else /* #if (COMM_NUM_USERS == 0U) */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_PNC_COM_MODE, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if (User >= COMM_NUM_USERS)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_PNC_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else if (ComMode == NULL_PTR)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_CURRENT_PNC_COM_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif /* (COMM_DEV_ERROR_DETECT == STD_ON) */
  {
#if (COMM_PNC_SUPPORT == STD_ON)
    uint8 numPncs = 0;
    numPncs = COMM_NUM_PNCS_OF_USER(User);
    if (numPncs == 0)
    {
      /* !LINKSTO ComM.EB.ComM_GetCurrentPNCComMode.COMM_E_NO_PNC_ASSIGNED,1 */
      RetVal = COMM_E_NO_PNC_ASSIGNED;
    }
    else if(numPncs > 1)
    {
      /* !LINKSTO ComM.EB.ComM_GetCurrentPNCComMode.COMM_E_MULTIPLE_PNC_ASSIGNED,1 */
      RetVal = COMM_E_MULTIPLE_PNC_ASSIGNED;
    }
    else
    {
      /* !LINKSTO ComM.EB.ComM_GetCurrentPNCComMode.ReturnedComMode.COMM_FULL_COMMUNICATION,1 */
      /* !LINKSTO ComM.EB.ComM_GetCurrentPNCComMode.ReturnedComMode.COMM_NO_COMMUNICATION,1 */
      (*ComMode) = ComM_PNCMode[COMM_PNC_OF_USER(User,0U)];
      RetVal = E_OK;
    }
#else /* (COMM_PNC_SUPPORT == STD_ON) */
#if (COMM_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(User);
    TS_PARAM_UNUSED(ComMode);
#endif /* (COMM_DEV_ERROR_DETECT == STD_OFF) */
    RetVal = COMM_E_NO_PNC_ASSIGNED;
#endif /* (COMM_PNC_SUPPORT == STD_ON) */
  }
#endif /* #if (COMM_NUM_USERS == 0U) */

  DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_EXIT(RetVal,User,ComMode);

  return RetVal;
}

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_PreventWakeUp]---------------------------*/

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_PreventWakeUp
(
  NetworkHandleType Channel,
  boolean           Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_PREVENTWAKEUP_ENTRY(Channel,Status);

  RetVal = ComM_ASR40_PreventWakeUp(Channel, Status);

  DBG_COMM_ASR32_PREVENTWAKEUP_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_PreventWakeUp]-------------------------------------*/
/* !LINKSTO ComM156,1, ComM.ASR32.ComM156,1 */
/* !LINKSTO ComM799,1, ComM470,1 */
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_PreventWakeUp
(
  NetworkHandleType Channel,
  boolean           Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_PREVENTWAKEUP_ENTRY(Channel,Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_PREVENT_WAKE_UP, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_PREVENT_WAKE_UP, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    /* If ECU group classification has bit 0 set, allow Prevent wake up */
    if (COMM_PREVENT_WAKEUP == (ComM_EB_NvRAM.ECUGroupClassification & COMM_PREVENT_WAKEUP))
    {
      if (TRUE == Status)
      {
        if (TRUE != ComM_ChannelWakeUpInhibitionStatus(Channel))
        {
          COMM_EMIT(Channel, COMM_HSM_COMM_EV_WAKEUP_INHIBITION);
          /* Set inhibition status of channel as Wake Up Inhibition */
          ComM_SetChannelComMInhibitionStatus(Channel, COMM_PREVENT_WAKEUP, TRUE);
        }
      }
      else
      {
        /* Reset Bit 0 Request of Wake Up Inhibition */
        /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_ComM_PreventWakeUp_REQ_STATUS_CHANGED,1 */
        ComM_SetChannelComMInhibitionStatus(Channel, COMM_PREVENT_WAKEUP, FALSE);
      }
      /* Set return value to E_OK only if ECU group classification allowed
       * access */
      RetVal = E_OK;
    }
  }

  DBG_COMM_ASR40_PREVENTWAKEUP_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_LimitChannelToNoComMode]---------------------*/

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_LimitChannelToNoComMode
(
  NetworkHandleType Channel,
  boolean           Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_ENTRY(Channel,Status);

  RetVal = ComM_ASR40_LimitChannelToNoComMode(Channel, Status);

  DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_LimitChannelToNoComMode]---------------------------*/
/* !LINKSTO ComM163,1, ComM.ASR32.ComM163,1, ComM470,1 */
/* !LINKSTO ComM800_Refine,1, ComM303,1, ComM841,1, ComM215,1 */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_LimitChannelToNoComMode
(
  NetworkHandleType Channel,
  boolean           Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_ENTRY(Channel,Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_LIMIT_CHANNEL_TO_NO_COM, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_LIMIT_CHANNEL_TO_NO_COM, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    /* If ECU group classification has bit 1 set, allow Limit to No Comm */
    if (COMM_LIMIT_NOCOMM == (ComM_EB_NvRAM.ECUGroupClassification & COMM_LIMIT_NOCOMM))
    {
      if (TRUE == Status)
      {
        /* Set Event for limit to no com */

        COMM_EMIT(Channel, COMM_HSM_COMM_EV_LIMIT_TO_NO_COM);
        /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitChannelToNoComMode_LIMIT_TO_NO_COM,1 */
        ComM_SetChannelComMInhibitionStatus(Channel, COMM_LIMIT_NOCOMM, TRUE);
      }
      else
      {
        /* Reset Bit 1 Request of Limit to No Com */
        /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitChannelToNoCom_REQ_STATUS_CHANGED,1 */
        ComM_SetChannelComMInhibitionStatus(Channel, COMM_LIMIT_NOCOMM, FALSE);
      }
      /* Set return value to E_OK only if ECU group classification allowed
       * access */
      RetVal = E_OK;
    }
  }

  DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_EXIT(RetVal,Channel,Status);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_LimitECUToNoComMode]-------------------------*/

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_LimitECUToNoComMode
(
  boolean Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_LIMITECUTONOCOMMODE_ENTRY(Status);

  RetVal = ComM_ASR40_LimitECUToNoComMode(Status);

  DBG_COMM_ASR32_LIMITECUTONOCOMMODE_EXIT(RetVal,Status);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_LimitECUToNoComMode]-------------------------------*/
/* !LINKSTO ComM124,1, ComM.ASR32.ComM124,1, ComM470,1 */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_LimitECUToNoComMode
(
  boolean Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 Channel;

  DBG_COMM_ASR40_LIMITECUTONOCOMMODE_ENTRY(Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_LIMIT_ECU_TO_NO_COM, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
#endif
  {
    /* If ECU group classification has bit 1 set, allow Limit to No Comm */
    if (COMM_LIMIT_NOCOMM == (ComM_EB_NvRAM.ECUGroupClassification & COMM_LIMIT_NOCOMM))
    {
      /* Deviation TASKING-1 */
      for (Channel = 0U; Channel < COMM_NUM_CHANNELS; Channel++)
      {
        if (TRUE == Status)
        {
          /* Set Event for limit to no com */
          /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitECUToNoComMode_LIMIT_TO_NO_COM,1 */
          COMM_EMIT(Channel, COMM_HSM_COMM_EV_LIMIT_TO_NO_COM);
          ComM_SetChannelComMInhibitionStatus(Channel, COMM_LIMIT_NOCOMM, TRUE);
        }
        else
        {
          /* Reset Bit 1 Request of Limit to No Com */
          /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitECUToNoCom_REQ_STATUS_CHANGED,1 */
          ComM_SetChannelComMInhibitionStatus(Channel, COMM_LIMIT_NOCOMM, FALSE);
        }
      }
      /* Set return value to E_OK only if ECU group classification allowed
       * access */
      RetVal = E_OK;
    }
  }

  DBG_COMM_ASR40_LIMITECUTONOCOMMODE_EXIT(RetVal,Status);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_ReadInhibitCounter]--------------------------*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_READINHIBITCOUNTER_ENTRY(CounterValue);

  RetVal = ComM_ASR40_ReadInhibitCounter(CounterValue);

  DBG_COMM_ASR32_READINHIBITCOUNTER_EXIT(RetVal,CounterValue);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_ReadInhibitCounter]--------------------------------*/
/* !LINKSTO ComM224,1, ComM.ASR32.ComM224,1, ComM302,1, ComM802,1, ComM141,1, ComM138,1, ComM142,1 */
/* !LINKSTO ComM143,1 */
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_READINHIBITCOUNTER_ENTRY(CounterValue);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_READ_INHIBIT_COUNTER, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else if (NULL_PTR == CounterValue)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_READ_INHIBIT_COUNTER, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    *CounterValue = ComM_EB_NvRAM.InhibitionCounter;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    RetVal = E_OK;
  }

  DBG_COMM_ASR40_READINHIBITCOUNTER_EXIT(RetVal,CounterValue);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_ResetInhibitCounter]-------------------------*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM107,1, ComM301,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_ResetInhibitCounter
(
  void
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_RESETINHIBITCOUNTER_ENTRY();

  RetVal = ComM_ASR40_ResetInhibitCounter();

  DBG_COMM_ASR32_RESETINHIBITCOUNTER_EXIT(RetVal);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_ResetInhibitCounter]-------------------------------*/
/* !LINKSTO ComM108,1, ComM803,1, ComM301,1 */
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_ResetInhibitCounter
(
  void
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_RESETINHIBITCOUNTER_ENTRY();

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_RESET_INHIBIT_COUNTER, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
#endif
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    ComM_EB_NvRAM.InhibitionCounter = 0U;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    /* Set NvRAM modified */
    COMM_PERS_DATA_MODIFIED();
    RetVal = E_OK;
  }

  DBG_COMM_ASR40_RESETINHIBITCOUNTER_EXIT(RetVal);

  return RetVal;
}
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))
/*------------------[ComM_ASR32_SetECUGroupClassification]-------------------*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM107,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_SetECUGroupClassification
(
  ComM_ASR32_InhibitionStatusType Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_ENTRY(Status);

  RetVal = ComM_ASR40_SetECUGroupClassification(Status);

  DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_EXIT(RetVal,Status);

  return RetVal;
}
#endif
#endif

/*------------------[ComM_SetECUGroupClassification]-------------------------*/
/* !LINKSTO ComM552,1 */
#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_SetECUGroupClassification
(
  ComM_ASR40_InhibitionStatusType Status
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_ENTRY(Status);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_SET_ECU_GROUP_CLASS, COMM_E_NOT_INITED);
    RetVal = COMM_E_UNINIT;
  }
  else
#endif
  {
    /* Change variable (in NvRAM mirror) */
    ComM_EB_NvRAM.ECUGroupClassification = Status;
    /* Set NvRAM modified */
    COMM_PERS_DATA_MODIFIED();
    RetVal = E_OK;
  }

  DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_EXIT(RetVal,Status);

  return RetVal;
}
#endif

/*------------------[ComM_GetVersionInfo]------------------------------------*/
/* !LINKSTO ComM370,1 */
#if (COMM_VERSION_INFO_API == STD_ON)
FUNC(void, COMM_CODE) ComM_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, COMM_APPL_DATA) VersionInfoPtr)
{
  DBG_COMM_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_GET_VERSION_INFO, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = COMM_VENDOR_ID;
    VersionInfoPtr->moduleID         = COMM_MODULE_ID;
    VersionInfoPtr->sw_major_version = COMM_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = COMM_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = COMM_SW_PATCH_VERSION;
  }

  DBG_COMM_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

/*------------------[ComM_ChannelNoComInhibitionStatus]----------------------*/

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
FUNC(boolean, COMM_CODE) ComM_ChannelNoComInhibitionStatus(
  uint8 instIdx)
{
  boolean RetVal;

  RetVal = ComM_BitArrayIsBitSet(ComM_ChannelNoComInhibition, instIdx);

  return RetVal;
}
#endif

/*------------------[ComM_ChannelWakeUpInhibitionStatus]---------------------*/

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
FUNC(boolean, COMM_CODE) ComM_ChannelWakeUpInhibitionStatus(
  uint8 instIdx)
{
  boolean RetVal;

  RetVal = ComM_BitArrayIsBitSet(ComM_ChannelWakeUpInhibition, instIdx);

  return RetVal;
}
#endif

/*------------------[ComM_Nm_NetworkStartIndication]-------------------------*/
/* !LINKSTO ComM383,1, ComM583,1 */
#if (COMM_NM_ACCESS_NEEDED == STD_ON)
FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication
(
  NetworkHandleType Channel
)
{
#if (COMM_DCM_ENABLED == STD_ON)
  boolean PassiveStartupStatus = FALSE;
#endif
  DBG_COMM_NM_NETWORKSTARTINDICATION_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_NETWORK_START_IND, COMM_E_NOT_INITED);
  }
  else if (COMM_IS_CHANNEL_INVALID(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_NETWORK_START_IND, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_DCM_ENABLED == STD_ON)
    PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, Channel);
    if((ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION) &&
       (PassiveStartupStatus == FALSE))
#else
    if(ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION)
#endif
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(Channel));
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    /* Set Event for network start indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_Nm_NetworkStartIndication_PASSIVE_WAKEUP_IND,1 */
    /* !LINKSTO ComM.EB.ComM_PASSIVE_WAKEUP_IND_NoComNoPendingReq,1, ComM903,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_NM_NETWORKSTARTINDICATION_EXIT(Channel);
}

/*------------------[ComM_Nm_NetworkMode]------------------------------------*/
/* !LINKSTO ComM390,1 */
FUNC(void, COMM_CODE) ComM_Nm_NetworkMode
(
  NetworkHandleType Channel
)
{
  DBG_COMM_NM_NETWORKMODE_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_NETWORK_MODE, COMM_E_NOT_INITED);
  }
  else if (COMM_IS_CHANNEL_INVALID(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_NETWORK_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_MULTICORE_ENABLED == STD_ON)
     /* !LINKSTO ComM.EB.BusNm_GetState,1 */
     ComM_LastNmMode[Channel] = NM_MODE_NETWORK;
#endif
    /* Set Event for network mode indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_Nm_NetworkMode_NM_MODE_SWITCHED,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_NM_MODE_SWITCHED);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_NM_NETWORKMODE_EXIT(Channel);
}

/*------------------[ComM_Nm_PrepareBusSleepMode]----------------------------*/
/* !LINKSTO ComM391,1 */
FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode
(
  NetworkHandleType Channel
)
{
  DBG_COMM_NM_PREPAREBUSSLEEPMODE_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_PREPARE_BUS_SLEEP, COMM_E_NOT_INITED);
  }
  else if (COMM_IS_CHANNEL_INVALID(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_PREPARE_BUS_SLEEP, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_MULTICORE_ENABLED == STD_ON)
     /* !LINKSTO ComM.EB.BusNm_GetState,1 */
     ComM_LastNmMode[Channel] = NM_MODE_PREPARE_BUS_SLEEP;
#endif
    /* Set Event for prepare bus sleep mode indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_Nm_PrepareBusSleepMode_NM_MODE_SWITCHED,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_NM_MODE_SWITCHED);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_NM_PREPAREBUSSLEEPMODE_EXIT(Channel);
}

/*------------------[ComM_Nm_BusSleepMode]-----------------------------------*/
/* !LINKSTO ComM392,1 */
FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode
(
  NetworkHandleType Channel
)
{
  DBG_COMM_NM_BUSSLEEPMODE_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_BUS_SLEEP_MODE, COMM_E_NOT_INITED);
  }
  else if (COMM_IS_CHANNEL_INVALID(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_BUS_SLEEP_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_MULTICORE_ENABLED == STD_ON)
     /* !LINKSTO ComM.EB.BusNm_GetState,1 */
     ComM_LastNmMode[Channel] = NM_MODE_BUS_SLEEP;
#endif
    /* Set Event for bus sleep mode indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_Nm_BusSleepMode_NM_MODE_SWITCHED,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_NM_MODE_SWITCHED);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_NM_BUSSLEEPMODE_EXIT(Channel);
}

/*------------------[ComM_Nm_RestartIndication]------------------------------*/
/* !LINKSTO ComM792,1 */
FUNC(void, COMM_CODE) ComM_Nm_RestartIndication
(
  NetworkHandleType Channel
)
{
#if (COMM_DCM_ENABLED == STD_ON)
  boolean PassiveStartupStatus = FALSE;
#endif
  DBG_COMM_NM_RESTARTINDICATION_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_RESTART_IND, COMM_E_NOT_INITED);
  }
  else if (COMM_IS_CHANNEL_INVALID(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_NM_RESTART_IND, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_DCM_ENABLED == STD_ON)
    PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, Channel);
    if((ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION) &&
       (PassiveStartupStatus == FALSE))
#else
    if(ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION)
#endif
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(Channel));
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    /* Set Event for Nm restart indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_Nm_RestartIndication_PASSIVE_WAKEUP_IND,1, ComM902,1 */
    /* !LINKSTO ComM894,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_NM_RESTARTINDICATION_EXIT(Channel);
}

#endif /* (COMM_NM_ACCESS_NEEDED == STD_ON) */

/*------------------[ComM_BusSM_BusSleepMode]-----------------------------------*/
/* !LINKSTO SWS_ComM_91000,1 */
FUNC(void, COMM_CODE) ComM_BusSM_BusSleepMode
(
  NetworkHandleType Channel
)
{
  DBG_COMM_BUSSM_BUSSLEEPMODE_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_BUS_SM_BUS_SLEEP_MODE, COMM_E_NOT_INITED);
  }
  else if (NOT_SLAVE_CHANNEL(Channel))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_BUS_SM_BUS_SLEEP_MODE, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    /* Set Event for bus sleep mode indication */
    /* !LINKSTO SWS_ComM_01018.Refine1,1 */
    /* !LINKSTO SWS_ComM_01018.Refine2,1 */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.BUS_SM_SLEEP_INDICATION,1 */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.BUS_SM_SLEEP_INDICATION_NO_COMMUNICATION,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_BUS_SM_SLEEP_INDICATION);
  }

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif

  DBG_COMM_BUSSM_BUSSLEEPMODE_EXIT(Channel);
}

#if (COMM_DCM_ENABLED == STD_ON)
/*------------------[ComM_DCM_ActiveDiagnostic]------------------------------*/
/* !LINKSTO ComM866,1, ComM873,1 */
FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic
(
  NetworkHandleType Channel
)
{
  DBG_COMM_DCM_ACTIVEDIAGNOSTIC_ENTRY(Channel);

#if (((COMM_DEV_ERROR_DETECT              == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_OFF)  && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_OFF)) || \
                                                          \
      ((COMM_DEV_ERROR_DETECT             == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_OFF)  && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_ON))  || \
                                                          \
      ((COMM_DEV_ERROR_DETECT             == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_ON)   && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_ON)))
  TS_PARAM_UNUSED(Channel);
#endif
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_DCM_ACTIVE_DIAGNOSTIC, COMM_E_NOT_INITED);
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_DCM_ACTIVE_DIAGNOSTIC, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    /* Diagnostic session on a Passive channel is not possible. */
    /* !LINKSTO ComM883,1, ComM889,1*/
    /* !LINKSTO ComM889_Refine_1,1 */
    /* !LINKSTO ComM889_Refine_2,1 */
    /* !LINKSTO SWS_ComM_00866_Refine_1,1 */
    /* !LINKSTO SWS_ComM_00866_Refine_2,1, SWS_ComM_00866_Refine_3,1 */
    /* !LINKSTO SWS_ComM_00876_Refine_1,1, SWS_ComM_00876_Refine_2,1, SWS_ComM_00876_Refine_3,1 */
    /* !LINKSTO SWS_ComM_00878_Refine_1,1, SWS_ComM_00878_Refine_2,1, SWS_ComM_00878_Refine_3,1 */
    /* !LINKSTO SWS_ComM_00883_Refine_1,1, SWS_ComM_00883_Refine_2,1, SWS_ComM_00883_Refine_3,1 */
#if (COMM_NM_VARIANT_PASSIVE_ONLY == STD_OFF)
    if (COMM_NM_VARIANT_OF_CHANNEL(Channel) != COMM_NM_PASSIVE_VARIANT)
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* Set the bit (active diagnostic request) for the corresponding
       * channel in the bit array */
      ComM_BitArraySet(ComM_DcmActiveStatus, Channel);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* notify state machine about changed diagnostics communication state */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_DCM_ActiveDiagnostic_REQ_STATUS_CHANGED,1 */
      COMM_EMIT(Channel, COMM_HSM_COMM_EV_REQ_STATUS_CHANGED);
    }
    else
    {
      /* Call has no effect */
    }
#endif
  }
  DBG_COMM_DCM_ACTIVEDIAGNOSTIC_EXIT(Channel);
}

/*------------------[ComM_DCM_InactiveDiagnostic]----------------------------*/
/* !LINKSTO ComM874,1 */
FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic
(
  NetworkHandleType Channel
)
{
  DBG_COMM_DCM_INACTIVEDIAGNOSTIC_ENTRY(Channel);
#if (((COMM_DEV_ERROR_DETECT              == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_OFF)  && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_OFF)) || \
                                                          \
      ((COMM_DEV_ERROR_DETECT             == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_OFF)  && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_ON))  || \
                                                          \
      ((COMM_DEV_ERROR_DETECT             == STD_OFF)  && \
      (COMM_NM_VARIANT_OF_CHANNEL_DYNAMIC == STD_ON)   && \
      (COMM_NM_VARIANT_PASSIVE_ONLY       == STD_ON)))
  TS_PARAM_UNUSED(Channel);
#endif
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_DCM_INACTIVE_DIAGNOSTIC, COMM_E_NOT_INITED);
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_DCM_INACTIVE_DIAGNOSTIC, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
#if (COMM_NM_VARIANT_PASSIVE_ONLY == STD_OFF)
    /* Diagnostic session on a Passive channel is not possible.
     * See RFC #56661 */
    if (COMM_NM_VARIANT_OF_CHANNEL(Channel) != COMM_NM_PASSIVE_VARIANT)
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* Clear the bit (active diagnostic request) for the corresponding
       * channel in the bit array */
      ComM_BitArrayClear(ComM_DcmActiveStatus, Channel);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* notify state machine about changed diagnostics communication state */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_DCM_InactiveDiagnostic_REQ_STATUS_CHANGED,1 */
      COMM_EMIT(Channel, COMM_HSM_COMM_EV_REQ_STATUS_CHANGED);
    }
    else
    {
      /* Call has no effect */
    }
#endif
  }

  DBG_COMM_DCM_INACTIVEDIAGNOSTIC_EXIT(Channel);
}
#endif  /* (COMM_DCM_ENABLED == STD_ON) */

/*------------------[ComM_EcuM_WakeUpIndication]-----------------------------*/
/* !LINKSTO ComM275,1, ComM066,1 */
FUNC(void, COMM_CODE) ComM_EcuM_WakeUpIndication
(
  NetworkHandleType Channel
)
{
#if (COMM_DCM_ENABLED == STD_ON)
  boolean PassiveStartupStatus = FALSE;
#endif
#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  uint8 chIdx = 0U;
#endif /* (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) */
  DBG_COMM_ECUM_WAKEUPINDICATION_ENTRY(Channel);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_ECUM_WAKE_UP_INDICATION, COMM_E_NOT_INITED);
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_ECUM_WAKE_UP_INDICATION, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    /* If COMM_SYNCHRONOUS_WAKE_UP is enabled, indicate all channels
     * a passive wakeup */
#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
    /* Set Event for EcuM wakeup indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_EcuM_WakeUpIndication_syncWU_PASSIVE_WAKEUP_IND,1, ComM694,1 */
    /* Deviation TASKING-1 */
    for (chIdx = 0; chIdx < COMM_NUM_CHANNELS; chIdx++)
    {
#if (COMM_DCM_ENABLED == STD_ON)
      PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, chIdx);
      if((ComM_RequestedComMode[chIdx] == COMM_NO_COMMUNICATION) &&
         (PassiveStartupStatus == FALSE))
#else
      if(ComM_RequestedComMode[chIdx] == COMM_NO_COMMUNICATION)
#endif
      {
        ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
        ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(chIdx));
        ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
      }
    }
    (void)ComM_HsmEmit(&ComM_HsmScComM, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
    TS_PARAM_UNUSED(Channel);
#if (COMM_PNC_SUPPORT == STD_ON)
    if( COMM_PNC_ENABLED )
    {
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ECUM_WAKEUP_IND_PNC_NO_COMMUNICATION,1 */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ECUM_WAKEUP_IND,1 */
      (void)ComM_HsmEmit(&ComM_HsmScComMPnc, COMM_HSM_COMMPNC_EV_ECUM_WAKEUP_IND);
    }
#endif
#else /*COMM_SYNCHRONOUS_WAKE_UP*/

#if (COMM_DCM_ENABLED == STD_ON)
    PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, Channel);
    if((ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION) &&
       (PassiveStartupStatus == FALSE))
#else
    if(ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION)
#endif
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(Channel));
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    /* Set Event for EcuM wakeup indication */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_EcuM_WakeUpIndication_PASSIVE_WAKEUP_IND,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);

#if(COMM_VLAN_SUPPORT)
    {
        uint32 index;
        /* Deviation TASKING-1 */
        for(index=0;index<COMM_VLAN_MANAGED_CH_NB;index++)
        {
          if(ComM_VlanConfigTable[index].managedCh == Channel)
          {
#if (COMM_DCM_ENABLED == STD_ON)
            PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, ComM_VlanConfigTable[index].managingCh);
            if((ComM_RequestedComMode[ComM_VlanConfigTable[index].managingCh] == COMM_NO_COMMUNICATION) &&
               (PassiveStartupStatus == FALSE))
#else
            if(ComM_RequestedComMode[ComM_VlanConfigTable[index].managingCh] == COMM_NO_COMMUNICATION)
#endif
            {
              ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
              ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(ComM_VlanConfigTable[index].managingCh));
              ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
            }

            COMM_EMIT(ComM_VlanConfigTable[index].managingCh, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
            break;
          }
        }
    }
#endif

#if ((COMM_DEV_ERROR_DETECT == STD_OFF) && (COMM_HSM_INST_MULTI_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(Channel);
#endif
#endif
  }
  DBG_COMM_ECUM_WAKEUPINDICATION_EXIT(Channel);
}

/* !LINKSTO SWS_ComM_91001,1 */
FUNC(void, COMM_CODE) ComM_EcuM_PNCWakeUpIndication
(
  PNCHandleType PNCid
)
{
#if (COMM_PNC_SUPPORT == STD_ON)
  uint8 PncIDIdx = 0U;
  DBG_COMM_ECUM_PNCWAKEUPINDICATION_ENTRY(PNCid);

  /* Find index from PNCIDs, since PNCIDs are generated in
     sorted order */
  /* Deviation TASKING-1 */
  for (PncIDIdx = 0U; PncIDIdx < COMM_NUM_PNC; PncIDIdx++)
  {
    if(PNCid == ComM_PncID[PncIDIdx])
    {
      break;
    }
  }

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    /* !LINKSTO SWS_ComM_00965,1 */
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_ECUM_PNC_WAKE_UP_INDICATION, COMM_E_NOT_INITED);
  }
  else if (PncIDIdx >= COMM_NUM_PNC)
  {
    COMM_DET_REPORT_ERROR(
      COMM_INSTANCE_ID, COMM_SID_ECUM_PNC_WAKE_UP_INDICATION, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {

    /* !LINKSTO ComM.EB.ComM_EcuM_PNCWakeUpIndication_ComMPncEnable_FALSE,1 */
    if(COMM_PNC_ENABLED)
    {
      ComM_PNCWakeUpIndication_PncHandle(PncIDIdx);
      ComM_PNCWakeUpIndication_ChannelHandle(PncIDIdx);
    }
  }
  DBG_COMM_ECUM_PNCWAKEUPINDICATION_EXIT(PNCid);
#else
  /* !LINKSTO ComM.EB.ComM_EcuM_PNCWakeUpIndication_No_PNCs,1 */
  TS_PARAM_UNUSED(PNCid);
#endif
}

/*------------------[ComM_CommunicationAllowed]-----------------------------*/
/* !LINKSTO ComM871,1, ComM884,1, ComM885,1 */
FUNC(void, COMM_CODE) ComM_CommunicationAllowed
(
  NetworkHandleType Channel,
  boolean           Allowed
)
{
  DBG_COMM_COMMUNICATIONALLOWED_ENTRY(Channel,Allowed);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_COMMUNICATIONALLOWED, COMM_E_NOT_INITED);
  }
  else if (Channel >= COMM_NUM_CHANNELS)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_COMMUNICATIONALLOWED, COMM_E_WRONG_PARAMETERS);
  }
  else
#endif
  {
    if(TRUE == Allowed)
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* Set the bit for the corresponding channel */
      ComM_BitArraySet(ComM_Communication_Allowed, Channel);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    else
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      /* Clear the bit for the corresponding channel */
      ComM_BitArrayClear(ComM_Communication_Allowed, Channel);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    /* Set Event for communication allowed */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_CommunicationAllowed_COMALLOWED_CHANGED,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_COMALLOWED_CHANGED);
  }

  DBG_COMM_COMMUNICATIONALLOWED_EXIT(Channel,Allowed);
}

/*------------------[ComM_BusSM_ModeIndication]------------------------------*/
/* !LINKSTO ComM675,1 */
FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication
(
  NetworkHandleType                                     Channel,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  DBG_COMM_BUSSM_MODEINDICATION_ENTRY(Channel,ComMode);

#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if (COMM_INIT != ComM_InitStatus)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_BUS_SM_INDICATION, COMM_E_NOT_INITED);
  }
  else if ((NULL_PTR == ComMode) || (Channel >= COMM_NUM_CHANNELS))
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_BUS_SM_INDICATION, COMM_E_WRONG_PARAMETERS);
  }
#if (COMM_BUSTYPE_INTERNAL_NEEDED == STD_ON)
  else if (COMM_BUSTYPE_OF_CHANNEL(Channel) == COMM_BUS_TYPE_INTERNAL)
  {
    COMM_DET_REPORT_ERROR(COMM_INSTANCE_ID, COMM_SID_BUS_SM_INDICATION, COMM_E_WRONG_PARAMETERS);
  }
#endif
  else
#endif
  {
    {
#if (COMM_PRECONDITION_ASSERT_ENABLED == STD_ON)
      uint8 max_mode = COMM_MAX(COMM_MAX(COMM_NO_COMMUNICATION,COMM_SILENT_COMMUNICATION),COMM_FULL_COMMUNICATION);
      COMM_PRECONDITION_ASSERT((*ComMode <=max_mode),COMM_SID_BUS_SM_INDICATION);
#endif
    }
    /* !LINKSTO ComM.EB.Dsn.Algorithm.BusSMModeIndication,1 */
    ComM_ChannelMode[Channel] = *ComMode;

    /* Set Bus Mode indication for corresponding channel */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.BUSSM_MODE_IND,1 */
    COMM_EMIT(Channel, COMM_HSM_COMM_EV_BUSSM_MODE_IND);
  }

  DBG_COMM_BUSSM_MODEINDICATION_EXIT(Channel,ComMode);
}

/*==================[internal function definitions]=========================*/

/*------------------[ComM_EB_GetUserRequestedComMode]----------------------------------*/
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetUserRequestedComMode
(
  uint8 User
)
{
  ComM_ASR40_ModeType ret;
  ret = ComM_UserRequestedComMode[User];
  return ret;
}

/*------------------[ComM_EB_SetRequestedComMode]----------------------------------*/
#if (COMM_PNC_GW_ENABLED == STD_ON)
FUNC(void, COMM_CODE) ComM_EB_SetRequestedComMode
(
  uint8 Channel,
  ComM_ASR40_ModeType ComMode,
  ComM_ASR40_ModeType ComModeWithoutPncRequest
)
{
  ComM_RequestedComMode[Channel] = ComMode;
  ComM_RequestedComModeWithoutPnc[Channel] = ComModeWithoutPncRequest;
}
#else /* (COMM_PNC_GW_ENABLED == STD_ON) */
FUNC(void, COMM_CODE) ComM_EB_SetRequestedComMode
(
  uint8 Channel,
  ComM_ASR40_ModeType ComMode
)
{
  ComM_RequestedComMode[Channel] = ComMode;
}
#endif /* (COMM_PNC_GW_ENABLED == STD_ON) */

/*------------------[ComM_EvaluatePnc]----------------------------------*/

#if (COMM_PNC_SUPPORT == STD_ON)
FUNC(void, COMM_CODE) ComM_EvaluatePnc
(
  uint8 Pnc,
  ComM_ASR40_ModeType ComMode
)
{
  const uint8_least NumUser = COMM_NUM_USERS_OF_PNC(Pnc);
  uint8_least       UserPos;

  ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();

  /* Search highest mode requested by users of a PNC */
  for (UserPos = 0U; UserPos < NumUser; UserPos++)
  {
    const uint8 UserOfPnc = COMM_USER_OF_PNC(Pnc,UserPos);

    if (ComMode < ComM_UserRequestedComMode[UserOfPnc])
    {
      ComMode = ComM_UserRequestedComMode[UserOfPnc];
    }
  }

  ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

  /* if users request full communication mode or diagnostics request full
  * com */
  if (COMM_FULL_COMMUNICATION == ComMode)
  {
#if(COMM_PNC_GW_ENABLED == STD_ON)
    const uint8 Index = ((uint8)(ComM_PncID[COMM_INST(Pnc)] >> SHIFT_3_BITS)) - COMM_PN_OFFSET;
    TS_AtomicSetBit_8(&ComM_PncStateGWE[Index], (uint8)(ComM_PncID[COMM_INST(Pnc)] & MASK_3_BITS));
    COMM_PNC_EMIT(Pnc, COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE);
#endif
    /* Set Event for full communication mode */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER,1 */
    /* !LINKSTO ComM.EB.SWS_ComM_00165_1,1 */
    COMM_PNC_EMIT(Pnc, COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER);
  }
  else /* no user needs full com */
  {
#if(COMM_PNC_GW_ENABLED == STD_ON)
    const uint8 Index = ((uint8)(ComM_PncID[COMM_INST(Pnc)] >> SHIFT_3_BITS)) - COMM_PN_OFFSET;
    TS_AtomicClearBit_8(&ComM_PncStateGWE[Index], (uint8)(ComM_PncID[COMM_INST(Pnc)] & MASK_3_BITS));
    COMM_PNC_EMIT(Pnc, COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE);
#endif
    /* Set Event for ready sleep mode */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_RequestComMode_NO_COM_REQ_BY_ALL_COMM_USER,1 */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ModeLimitation_NO_COM_REQ_BY_ALL_COMM_USER,1 */
    COMM_PNC_EMIT(Pnc, COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER);
  }
}
#endif

/*------------------[ComM_SetChannelComMInhibitionStatus]--------------------*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
FUNC(void, COMM_CODE) ComM_SetChannelComMInhibitionStatus
(
  uint8                           instIdx,
  ComM_ASR40_InhibitionStatusType Inhibitiontype,
  boolean                         Status
)
{

#if (COMM_MODE_LIMITATION_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(Inhibitiontype);
#endif

/* !LINKSTO ComM690,1 */
#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
  if (COMM_LIMIT_NOCOMM == Inhibitiontype)
  {
    if (TRUE == Status)
    {
      /* Set inhibition status of channel as Limit to No Com */
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_ChannelNoComInhibition, instIdx);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    else
    {
      /* Reset Bit Request of Limit to No Com */
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArrayClear(ComM_ChannelNoComInhibition, instIdx);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

      /* emit event 'REQ_STATUS_CHANGED'. This triggers the required
       * transitions if there are user requests when inhibition was active */
      /* !LINKSTO ComM839,1, ComM840,1 */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitChannelToNoCom_REQ_STATUS_CHANGED,1 */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_LimitECUToNoCom_REQ_STATUS_CHANGED,1 */
      COMM_EMIT(instIdx, COMM_HSM_COMM_EV_REQ_STATUS_CHANGED);
    }
  }
  else
#endif
  {
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
    /* Find out if storing in NvRAM is allowed */
    const boolean changeNvRamAllowed = ComM_BitArrayIsBitSet(ComM_EB_NoWakeupNvStorage, instIdx);

    /* Modify Wakeup inhibition */
    if (TRUE == Status)
    {
      /* Enable inhibition */

      /* Set runtime variable */
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_ChannelWakeUpInhibition, instIdx);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

      /* Check setting NvRAM */
      if (TRUE == changeNvRamAllowed)
      {
        /* Set value in NvRAM */
        ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
        ComM_BitArraySet(ComM_EB_NvRAM.ChannelWakeUpInhibition, instIdx);
        ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
        /* Set NvRAM modified */
        COMM_PERS_DATA_MODIFIED();
      }
    }
    else
    {
      /* Take back inhibition */

      /* Clear runtime value */
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArrayClear(ComM_ChannelWakeUpInhibition, instIdx);
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();

      /* Check setting NvRAM */
      if (TRUE == changeNvRamAllowed)
      {
        /* Clear value in NvRAM */
        ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
        ComM_BitArrayClear(ComM_EB_NvRAM.ChannelWakeUpInhibition, instIdx);
        ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
        /* Set NvRAM modified */
        COMM_PERS_DATA_MODIFIED();
      }

      /* emit event 'REQ_STATUS_CHANGED'. This triggers the required
       * transitions if there are user requests when inhibition was active */
      /* !LINKSTO ComM839,1, ComM840,1 */
      /* !LINKSTO ComM.EB.Dsn.StateDynamics.ComM_ComM_PreventWakeUp_REQ_STATUS_CHANGED,1 */
      COMM_EMIT(instIdx, COMM_HSM_COMM_EV_REQ_STATUS_CHANGED);
    }
#endif
  }
}
#endif

FUNC(void, COMM_CODE) ComM_BitArraySet
(
  P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) arr,
  uint16                                  idx
)
{
  arr[idx / 8U] |= (uint8)(1U << (idx % 8U));
}

FUNC(void, COMM_CODE) ComM_BitArrayClear
(
  P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) arr,
  uint16                                 idx
)
{
  arr[idx / 8U] &= (uint8)(~(uint8)(1U << (idx % 8U)));
}

FUNC(boolean, COMM_CODE) ComM_BitArrayIsBitSet
(
  P2CONST(uint8, AUTOMATIC, COMM_APPL_DATA) arr,
  uint16                                    idx
)
{
  boolean RetVal;

  RetVal = ((((uint8)(arr[idx / 8U] >> (idx % 8U))) & 1U) != 0U) ? TRUE : FALSE;

  return RetVal;
}

#if (COMM_PNC_SUPPORT == STD_ON)
#if (COMM_NUM_PNC > 0)
/*------------------[ComM_EB_GetPncRequestedComMode]----------------------------------*/
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetPncRequestedComMode
(
  uint8 Channel,
  uint8 PncPos
)
{
  ComM_ASR40_ModeType reqMode;
  uint8 Idx;
  uint8 PncIDIdx = 0U;
  /* Find index from PNCIDs, since PNCIDs are generated in
     sorted order */
  /* Deviation TASKING-1 */
  for (Idx = 0U; Idx < COMM_NUM_PNC; ++Idx)
  {
    if( COMM_PNC_OF_CHANNEL(Channel,PncPos) == ComM_PncID[Idx] )
    {
      PncIDIdx = Idx;
    }
  }
  reqMode = ComM_PNCRequestedComMode[PncIDIdx];
  return reqMode;
}

/*-----------------[ComM_EB_GetPncRequestedComMode0PncVectorAvoidance]-----------------*/
#if(COMM_0_PNC_VECTOR_AVOIDANCE == STD_ON)
FUNC(ComM_ASR40_ModeType, COMM_CODE) ComM_EB_GetPncRequestedComMode0PncVectorAvoidance
(
  uint8 Channel,
  uint8 PncPos
)
{
  ComM_ASR40_ModeType reqMode;
  uint8 Idx;
  uint8 PncIDIdx = 0U;
  /* Find index from PNCIDs, since PNCIDs are generated in
     sorted order */
  for (Idx = 0U; Idx < COMM_NUM_PNC; ++Idx)
  {
    if(COMM_PNC_OF_CHANNEL(Channel,PncPos) == ComM_PncID[Idx])
    {
      PncIDIdx = Idx;
    }
  }
  reqMode = ComM_PNCRequestedComMode0PncVectorAvoidance[PncIDIdx];
  return reqMode;
}
#endif
#endif /* (COMM_NUM_PNC > 0) */

STATIC FUNC(void, COMM_CODE) ComM_ProcessPncComSignals
(
  P2VAR(boolean, AUTOMATIC, COMM_APPL_DATA) SignalStatusPtr,
  P2CONST(ComM_RxSignal_Struct_Type, AUTOMATIC, COMM_APPL_CONST) SignalConfigurationPtr,
  P2VAR(uint8, AUTOMATIC, COMM_APPL_DATA) SignalMergedPtr,
  uint8 SignalKind
)
{
  /* No *_least type for a loop variable as the variable is also used as
     an argument whose underlying type is uint8  */
  uint8 PncIdx;
  uint8 Index;
  uint8 i;
  uint8 RxEiraMerged[COMM_PN_INFO_LENGTH];
  /* Flag to indicate if at least one signal is received successfully */
  boolean EiraStatus = FALSE;

  uint8 signalCount = 0U;
  /* Initialise RxEiraMerged */
  TS_MemSet(RxEiraMerged, 0U, COMM_PN_INFO_LENGTH * sizeof(uint8));

#if(COMM_PNC_GW_ENABLED == STD_ON)
  if(SignalKind == COMM_IS_RX_ACTIVE_ERA)
  {
    signalCount = COMM_NUM_RX_ACTIVE_ERA_SIGNALS;
  }
  else if(SignalKind == COMM_IS_RX_UNFILTERED_ERA)
  {
    signalCount = COMM_NUM_RX_UNFILTERED_ERA_SIGNALS;
  }
  else
#endif
  {
    signalCount = COMM_NUM_RX_EIRA_SIGNALS;
  }

  if (*SignalStatusPtr == TRUE)
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    /* EIRA change has been notified by Com, reset flag */
    *SignalStatusPtr = FALSE;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    for (Index = 0U; Index < signalCount; Index++)
    {
      uint8 RxEiraReceived[COMM_PN_INFO_LENGTH];
      Std_ReturnType retVal;
      /* Read EIRA signal into a temporary buffer */
      /* !LINKSTO ComM984,1 */
#if (COMM_MULTICORE_ENABLED == STD_OFF)
      retVal = Com_ReceiveSignal(SignalConfigurationPtr[Index].ComHandleId, RxEiraReceived);
#else
#if (COMM_NO_OF_RX_BUFFERS == 0)
      retVal = Com_ReceiveSignal(SignalConfigurationPtr[Index].ComHandleId, RxEiraReceived);
#else
      if (SignalConfigurationPtr[Index].isLocalCore == TRUE)
      {
        retVal = Com_ReceiveSignal(SignalConfigurationPtr[Index].ComHandleId, RxEiraReceived);
      }
      else
      {
        uint16 index = SignalConfigurationPtr[Index].MulticoreRxBufferIndex;
        retVal = E_OK;
        ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
        TS_MemCpy(RxEiraReceived, &ComM_MulticoreRxBuffer[index][0], COMM_PN_INFO_LENGTH);
        ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
      }
#endif
#endif
      if (E_OK == retVal)
      {
        /* Update the flag if at least one signal is received */
        EiraStatus = TRUE;
        /* Data from multiple signals may be available at the same time;
         * So, merge the contents of the newly received signal into
         * another buffer.
         */
        /* Deviation TASKING-1 */
        for (i = 0U; i < COMM_PN_INFO_LENGTH; i++)
        {
          /* Some PNCs may not reference this RX EIRA Com signal.
           * Therefore, the bits corresponding to PNCs which do not
           * reference this Com signal have to be masked out before
           * copying into the global EIRA bit vector.
           */
          /* !LINKSTO ComM944,1, ComM.EB.SWS_ComM_00945,1 */
#if(COMM_PNC_GW_ENABLED == STD_ON)
          if(SignalKind == COMM_IS_RX_ACTIVE_ERA)
          {
            RxEiraMerged[i] |= (uint8)(RxEiraReceived[i] & ComM_PncRxActiveEraMask[Index][i]);
          }
          else if(SignalKind == COMM_IS_RX_UNFILTERED_ERA)
          {
            RxEiraMerged[i] |= (uint8)(RxEiraReceived[i] & ComM_PncRxUnfilteredEraMask[Index][i]);
          }
          else
#endif /* #if(COMM_PNC_GW_ENABLED == STD_ON) */
          {
            RxEiraMerged[i] |= (uint8)(RxEiraReceived[i] & ComM_PncRxEiraMask[Index][i]);
          }
        }
      }
      else
      {
#if (COMM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
        /*use main function SID*/
        COMM_POSTCONDITION_ASSERT((retVal != E_OK),COMM_SID_MAIN_FUNCTION);
#endif
      }
    }
  }
  /* If at least one signal is received */
  if (TRUE == EiraStatus)
  {
    /* Deviation TASKING-1 */
    for (PncIdx = 0U; PncIdx < COMM_NUM_PNC; PncIdx++)
    {
      uint8 EiraMask;

      /* Each bit of EIRA stands for one PNC.
      * For example bit 0 of Byte 0 stands for PNC_0 and bit 0 of Byte 1
      * stands for PNC_8
      */
      /* Calculate the byte index from PncID */
      Index = ((uint8)(ComM_PncID[PncIdx] >> SHIFT_3_BITS)) - COMM_PN_OFFSET;
      /* Calculate the byte mask based on bit position of this Pnc */
      EiraMask = (uint8)(1U << (ComM_PncID[PncIdx] & MASK_3_BITS));
      if((SignalMergedPtr[Index] & EiraMask) != (RxEiraMerged[Index] & EiraMask))
      {
        if (SignalKind == COMM_IS_RX_EIRA)
        {
          /* There is a change in EIRA, append the EIRA event to the queue */
          /* !LINKSTO ComM.EB.Dsn.StateDynamics.COMCBK,1, ComM950,1 */
          COMM_PNC_EMIT(PncIdx, COMM_HSM_COMMPNC_EV_COMCBK);
        }
        else if (SignalKind == COMM_IS_RX_ACTIVE_ERA)
        {
          COMM_PNC_EMIT(PncIdx, COMM_HSM_COMMPNC_EV_PNC_UPDATE_TX_PASSIVE);
        }
        else
        {
         /* A Tx output signal which has linked to it a passive channel
          * shall be updated only in case
          * Pn info is received on an Rx signal which has linked to it an active channel
          * or a user request full communication
          */
         /* empty else needed for MISRA */
        }
        if ((SignalKind == COMM_IS_RX_UNFILTERED_ERA) || (SignalKind == COMM_IS_RX_ACTIVE_ERA))
        {
          if ((RxEiraMerged[Index] & EiraMask) != 0U)
          {
            /* There is a change in ERA to 1 */
            /* !LINKSTO ComM.EB.SWS_ComM_00934,1, ComM.EB.SWS_ComM_00951,1 */
            /* !LINKSTO ComM.EB.SWS_ComM_00165_2,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_PREPARE_SLEEP_1,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_PREPARE_SLEEP_2,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_READY_SLEEP_0,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_READY_SLEEP_1,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_READY_SLEEP_2,1 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.FULL_COM_REQ_BY_COMM_USER_PNC_NO_COMMUNICATION_1,1 */
            /* !LINKSTO ComM.EB.ComM_PncReadySleep_PncRequested,1 */
            COMM_PNC_EMIT(PncIdx, COMM_HSM_COMMPNC_EV_FULL_COM_REQ_BY_COMM_USER);
          }
          else
          {
            /* There is a change in ERA to 0 */
            /* !LINKSTO ComM.EB.Dsn.StateDynamics.NO_COM_REQ_BY_ALL_COMM_USER_PNC_REQUESTED_0,1 */
            COMM_PNC_EMIT(PncIdx, COMM_HSM_COMMPNC_EV_NO_COM_REQ_BY_ALL_COMM_USER);
          }
        }
      }
    }
    /* Copy new EIRA to old EIRA for next evaluation */
    /* Deviation TASKING-1 */
    for (Index = 0U; Index < COMM_PN_INFO_LENGTH; Index++)
    {
      SignalMergedPtr[Index] = RxEiraMerged[Index];
    }
  }
}

#if (COMM_MULTICORE_ENABLED == STD_ON)
#if (COMM_NO_OF_TX_BUFFERS != 0)
STATIC FUNC(void, COMM_CODE) ComM_ProcessTxSignals(void)
{
   uint16 i = 0;
   for (i = 0; i < COMM_NUM_TX_SIGNALS; i++)
   {
      if (ComM_TxComSignalCfg[i].SchMSendFctPtr != NULL_PTR)
      {
        uint16 index = ComM_TxComSignalCfg[i].MulticoreTxBufferIndex;
        uint8* Dataptr = (uint8*)(&ComM_MulticoreTxBuffer[index][0]);
        if(ComM_MulticoreTxBufferStatus[index] == TRUE)
        {
          /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_2,1 */
          if (ComM_TxComSignalCfg[i].SchMSendFctPtr(Dataptr) == SCHM_E_OK)
          {
            ComM_MulticoreTxBufferStatus[index] = FALSE;
          }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
          else
          {
            /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
            COMM_DET_REPORT_ERROR(ComM_TxComSignalCfg[i].CoreId, COMM_SID_MASTER_DATA_SET, COMM_E_MASTER_DATA_SET);
          }
#endif
       }
    }
  }
}
#endif
#endif

FUNC(void, COMM_CODE) ComM_PncMainFunction
(
void
)
{
  /* No *_least type for a loop variable as the variable is also used as
     an argument whose underlying type is uint8  */
  uint8 PncIdx;
  boolean ComMExecutePncMainFunction = FALSE;
  ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
  /* CHECK: NOPARSE */
  /* No parse have been added because
   * current test strategy does not permit
   * that a mainfunction to interrupt another mainfunction.
   */
  if (ComMPncMainfunctionRunning == FALSE)
  {
    ComMPncMainfunctionRunning = TRUE;
    ComMExecutePncMainFunction = TRUE;
  }
  ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
  if (ComMExecutePncMainFunction == TRUE)
  {
#if(COMM_PNC_GW_ENABLED == STD_ON)
    ComM_ProcessPncComSignals(&ComM_RxActiveEraSignalStatus,
                              ComM_RxComSignalActiveEraCfg,
                              ComM_RxActiveEra,
                              COMM_IS_RX_ACTIVE_ERA
                              );
    ComM_ProcessPncComSignals(&ComM_RxUnfilteredEraSignalStatus,
                              ComM_RxComSignalUnfilteredEraCfg,
                              ComM_RxUnfilteredEra,
                              COMM_IS_RX_UNFILTERED_ERA
                              );
#endif
    ComM_ProcessPncComSignals(&ComM_RxEiraSignalStatus,
                              ComM_RxComSignalEiraCfg,
                              ComM_RxEira,
                              COMM_IS_RX_EIRA
                              );
    /* Deviation TASKING-1 */
    for (PncIdx = 0U; PncIdx < COMM_NUM_PNC; PncIdx++)
    {
      if(COMM_PNC_HSM_EVENT_PENDING(PncIdx))
      {
        /* Schedule the pnc state machine */
        (void) COMM_HSMMAININST(&ComM_HsmScComMPnc, PncIdx);
      }
    }
    ComMPncMainfunctionRunning = FALSE;
#if (COMM_MULTICORE_ENABLED == STD_ON)
#if (COMM_NO_OF_TX_BUFFERS != 0)
    ComM_ProcessTxSignals();
#endif
#endif
  }
  /* CHECK: PARSE */
}
#endif

FUNC(Std_ReturnType, COMM_CODE) ComM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, COMM_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType result = E_NOT_OK;
  P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) ConfigPtr = voidConfigPtr;

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE )
    {
      /* Validate the post build configuration against the compile time configuration */
      if(COMM_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if(ComM_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
           * Published information in compile time configuration
           */
          if(COMM_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid */
            result = E_OK;
          }
        }
      }
    }
  }
  return result;
}
#if(COMM_PNC_SUPPORT == STD_ON)
STATIC FUNC(void,COMM_CODE) ComM_PNCWakeUpIndication_PncHandle(uint8 pncIdx)
{
  /* !LINKSTO SWS_ComM_00964,1 */
  COMM_PNC_EMIT(pncIdx, COMM_HSM_COMMPNC_EV_ECUM_PNCWAKEUP_IND);
  TS_PARAM_UNUSED(pncIdx);
}

STATIC FUNC(void,COMM_CODE) ComM_PNCWakeUpIndication_ChannelHandle(uint8 pncIdx)
{
#if (COMM_DCM_ENABLED == STD_ON)
  boolean PassiveStartupStatus = FALSE;
#endif
  uint8 chIdx = 0U;
#if(COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  /* Deviation TASKING-1 */
  for (chIdx = 0; chIdx < COMM_NUM_CHANNELS; chIdx++)
  {
#if (COMM_DCM_ENABLED == STD_ON)
    PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, chIdx);
    if((ComM_RequestedComMode[chIdx] == COMM_NO_COMMUNICATION) &&
       (PassiveStartupStatus == FALSE))
#else
    if(ComM_RequestedComMode[chIdx] == COMM_NO_COMMUNICATION)
#endif
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(chIdx));
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
  }
  /* !LINKSTO SWS_ComM_Channel_ComMSynchronousWakeUp_true,1 */
  (void)ComM_HsmEmit(&ComM_HsmScComM, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
  TS_PARAM_UNUSED(pncIdx);
#else
  for (chIdx = 0U; chIdx < COMM_NUM_CHANNELS_OF_PNC(pncIdx); chIdx++)
  {
    const uint8 Channel = COMM_CHANNEL_OF_PNC(pncIdx,chIdx);
#if (COMM_DCM_ENABLED == STD_ON)
    PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, Channel);
    if((ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION) &&
       (PassiveStartupStatus == FALSE))
#else
    if(ComM_RequestedComMode[Channel] == COMM_NO_COMMUNICATION)
#endif
    {
      ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
      ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(Channel));
      ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
    }
    /* !LINKSTO SWS_ComM_Channel_ComMSynchronousWakeUp_false,1 */
    /* !LINKSTO ComM.EB.Dsn.StateDynamics.ECUM_PNCWAKEUP_IND_PNC_NO_COMMUNICATION,1 */
    (void)COMM_HSMEMITINST(&ComM_HsmScComM, Channel, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
#if(COMM_VLAN_SUPPORT)
    {
      uint32 index;
      /* Deviation TASKING-1 */
      for(index=0;index<COMM_VLAN_MANAGED_CH_NB;index++)
      {
        if(ComM_VlanConfigTable[index].managedCh == Channel)
        {
#if (COMM_DCM_ENABLED == STD_ON)
          PassiveStartupStatus = ComM_BitArrayIsBitSet(ComM_DcmActiveStatus, ComM_VlanConfigTable[index].managingCh);
          if((ComM_RequestedComMode[ComM_VlanConfigTable[index].managingCh] == COMM_NO_COMMUNICATION) &&
             (PassiveStartupStatus == FALSE))
#else
          if(ComM_RequestedComMode[ComM_VlanConfigTable[index].managingCh] == COMM_NO_COMMUNICATION)
#endif
          {
            ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
            ComM_BitArraySet(ComM_PassiveStartupStatus, COMM_INST(ComM_VlanConfigTable[index].managingCh));
            ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
          }
          COMM_EMIT(ComM_VlanConfigTable[index].managingCh, COMM_HSM_COMM_EV_PASSIVE_WAKEUP_IND);
        }
      }
    }
#endif
  }
#endif
}
#endif /*(COMM_PNC_SUPPORT == STD_ON)*/




#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[end of file]===========================================*/
