/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_INTERNAL_H
#define CANTP_INTERNAL_H


/*
 *  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Directive: Dir 4.7 (required)
 *    If a function returns error information, then that error information shall
 *    be tested.
 *
 *    Reason:
 *    No reaction possible on Det return value. Furthermore it shall be E_OK
 *    all the time so it is not really error information that is returned.
 */

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>       /* typedefs for AUTOSAR com stack */
#include <CanTp_InternalLib.h>    /* internal library definitions */
#include <CanTp_InternalCfg.h>    /* CanTp internal configuration */
#include <CanTp_Cfg.h>            /* CanTp configuration */
#include <CanTp_Types_Int.h>      /* generated CanTp postbuild types */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_ExitTable.h>      /* CanTp jumptable definitions */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

#if (defined PDUR_NO_PBCFG_REQUIRED) /* to prevent double declaration */
#error PDUR_NO_PBCFG_REQUIRED is already defined
#endif /* if (defined PDUR_NO_PBCFG_REQUIRED) */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED

#include <PduR_CanTp.h>           /* PduR module API */

#undef PDUR_NO_PBCFG_REQUIRED

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
#include <EcuC.h>
#endif

/* !LINKSTO CanTp.ASR40.CANTP165,1 */
#include <SchM_CanTp.h>           /* Schedule manager public API */

#if (defined TS_RELOCATABLE_CFG_ENABLE) /* to prevent double declaration */
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif /* if (defined TS_RELOCATABLE_CFG_ENABLE) */

#define TS_RELOCATABLE_CFG_ENABLE CANTP_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>

/*==================[macros]=================================================*/

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
#define MAX_NUMBER_OF_TX_CHANNELS CANTP_MAX_TX_CHANNELS
#else /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF */
#define MAX_NUMBER_OF_TX_CHANNELS CanTp_CfgPtr->NumberOfTxChannels
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */


#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
#define MAX_NUMBER_OF_CHANNELS CANTP_MAX_PARALLEL_CHANNELS
#else /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF */
#define MAX_NUMBER_OF_CHANNELS CanTp_CfgPtr->NumberOfChannels
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

#if(CANTP_MAX_TX_CHANNELS > 255U)
#define INVALID_TX_CHANNEL_ID    0xFFFFU
#else
#define INVALID_TX_CHANNEL_ID    0xFFU
#endif

#if((CANTP_MAX_RX_CHANNELS + CANTP_MAX_TX_CHANNELS) > 255U)
#define INVALID_RX_CHANNEL_ID    0xFFFFU
#else
#define INVALID_RX_CHANNEL_ID    0xFFU
#endif

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)

/* macro to get locked channels */
#define CANTP_GET_LOCKED_TX_CH_INDEX(Channel, CanTpTxSduId, retVal)        \
  do                                                                       \
  {                                                                        \
    retVal = CanTp_GetLockedTxChannelIndex(&Channel, CanTpTxSduId);        \
  }while(0U)

/* macro to get locked channel or the first released one */
#define CANTP_GET_TX_CH_INDEX(Channel, CanTpTxSduId, retVal)               \
  do                                                                       \
  {                                                                        \
    retVal = CanTp_GetTxChannelIndex(&Channel, CanTpTxSduId);              \
  }while(0U)

#define CANTP_GET_LOCKED_RX_CH_INDEX(Channel, CanTpRxSduId, retVal)        \
  do                                                                       \
  {                                                                        \
    retVal = CanTp_FindLockedRxChannelIndex(&Channel, CanTpRxSduId);       \
  }while(0U)

/* macro to get locked channel or the first released one  */
#define CANTP_GET_RX_CH_INDEX(Channel, CanTpRxSduId, retVal)               \
  do                                                                       \
  {                                                                        \
    retVal = CanTp_GetRxChannelIndex(&Channel, CanTpRxSduId);              \
  }while(0U)

#else /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF */

/* macro to get locked channels */
#define CANTP_GET_LOCKED_TX_CH_INDEX(ChannelId, CanTpTxSduId, retVal)        \
  do                                                                         \
  {                                                                          \
    ChannelId = CANTP_CFG_GET_TXNSDU_CONFIG(CanTpTxSduId).Channel;           \
    TS_PARAM_UNUSED(retVal);                                                 \
  }while(0U)

/* macro to get locked channel or the first released one */
#define CANTP_GET_TX_CH_INDEX(ChannelId, CanTpTxSduId, retVal)               \
  do                                                                         \
  {                                                                          \
    ChannelId = CANTP_CFG_GET_TXNSDU_CONFIG(CanTpTxSduId).Channel;           \
    TS_PARAM_UNUSED(retVal);                                                 \
  }while(0U)

#define CANTP_GET_LOCKED_RX_CH_INDEX(ChannelId, CanTpRxSduId, retVal)        \
  do                                                                         \
  {                                                                          \
    ChannelId = CANTP_CFG_GET_RXNSDU_CONFIG(CanTpRxSduId).Channel;           \
    TS_PARAM_UNUSED(retVal);                                                 \
  }while(0U)

#define CANTP_GET_RX_CH_INDEX(ChannelId, CanTpRxSduId, retVal)               \
  do                                                                         \
  {                                                                          \
    ChannelId = CANTP_CFG_GET_RXNSDU_CONFIG(CanTpRxSduId).Channel;           \
    TS_PARAM_UNUSED(retVal);                                                 \
  }while(0U)

#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */


/*------------------[the Inc/Dec counter for NonIdleChannels]----------------*/

#if((CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS) > 255U)
  #define CANTP_INC_NON_IDLE_CHANNELS()                                     \
    TS_AtomicAssign16(CanTp_NonIdleChannelCounter, CanTp_NonIdleChannelCounter+1U)
#else /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS <= 255U)*/
  #define CANTP_INC_NON_IDLE_CHANNELS()                                     \
    TS_AtomicAssign8(CanTp_NonIdleChannelCounter, CanTp_NonIdleChannelCounter+1U)
#endif /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS > 255U) */

#if((CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS) > 255U)
  #define CANTP_DEC_NON_IDLE_CHANNELS()                                     \
    TS_AtomicAssign16(CanTp_NonIdleChannelCounter, CanTp_NonIdleChannelCounter-1U)
#else /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS <= 255U)*/
  #define CANTP_DEC_NON_IDLE_CHANNELS()                                     \
    TS_AtomicAssign8(CanTp_NonIdleChannelCounter, CanTp_NonIdleChannelCounter-1U)
#endif /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS > 255U) */

#if((CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS) > 255U)
  #define CANTP_GET_NON_IDLE_CHANNELS(RetValue)                                     \
    TS_AtomicAssign16(RetValue, CanTp_NonIdleChannelCounter)
#else /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS <= 255U)*/
  #define CANTP_GET_NON_IDLE_CHANNELS(RetValue)                                     \
    TS_AtomicAssign8(RetValue, CanTp_NonIdleChannelCounter)
#endif /* (CANTP_MAX_TX_CHANNELS + CANTP_MAX_RX_CHANNELS > 255U) */


/*------------------[the CAN Tp Instance Id]---------------------------------*/

#if (defined CANTP_INSTANCE_ID)
#error CANTP_INSTANCE_ID is already defined
#endif /* if (defined CANTP_INSTANCE_ID) */

/** \brief instance id, used for DET */
#define CANTP_INSTANCE_ID     0U





/*------------------------[Defensive programming]----------------------------*/

#if (defined CANTP_PRECONDITION_ASSERT)
#error CANTP_PRECONDITION_ASSERT is already defined
#endif

#if (defined CANTP_PRECONDITION_ASSERT_NO_EVAL)
#error CANTP_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANTP_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))
#else
#define CANTP_PRECONDITION_ASSERT(Condition, ApiId)
#define CANTP_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANTP_POSTCONDITION_ASSERT)
#error CANTP_POSTCONDITION_ASSERT is already defined
#endif

#if (defined CANTP_POSTCONDITION_ASSERT_NO_EVAL)
#error CANTP_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANTP_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))
#else
#define CANTP_POSTCONDITION_ASSERT(Condition, ApiId)
#define CANTP_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANTP_INVARIANT_ASSERT)
#error CANTP_INVARIANT_ASSERT is already defined
#endif

#if (defined CANTP_INVARIANT_ASSERT_NO_EVAL)
#error CANTP_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (CANTP_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))
#else
#define CANTP_INVARIANT_ASSERT(Condition, ApiId)
#define CANTP_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANTP_STATIC_ASSERT)
# error CANTP_STATIC_ASSERT is already defined
#endif
#if (CANTP_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define CANTP_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define CANTP_STATIC_ASSERT(expr)
#endif

#if (defined CANTP_UNREACHABLE_CODE_ASSERT)
#error CANTP_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (CANTP_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define CANTP_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(CANTP_MODULE_ID, CANTP_INSTANCE_ID, (ApiId))
#else
#define CANTP_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined CANTP_INTERNAL_API_ID)
#error CANTP_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define CANTP_INTERNAL_API_ID DET_INTERNAL_API_ID




/*------------------[critical section protection]----------------------------*/

#if (defined CANTP_ENTER_CRITICAL_SECTION) /* to prevent double declaration */
#error CANTP_ENTER_CRITICAL_SECTION is already defined
#endif /* if (defined CANTP_ENTER_CRITICAL_SECTION) */

/** \brief macro CANTP_ENTER_CRITICAL_SECTION
 **
 ** Calls the SchM to enter a critical section.
 */
#define CANTP_ENTER_CRITICAL_SECTION()  do                                  \
  {                                                                         \
    CANTP_SCHM_ENTER_CANTP(); \
  } while(0U)


#if (defined CANTP_LEAVE_CRITICAL_SECTION) /* to prevent double declaration */
#error CANTP_LEAVE_CRITICAL_SECTION is already defined
#endif /* if (defined CANTP_LEAVE_CRITICAL_SECTION) */

/** \brief macro CANTP_LEAVE_CRITICAL_SECTION
 **
 ** Calls the SchM to leave a critical section.
 */
#define CANTP_LEAVE_CRITICAL_SECTION()  do                                 \
  {                                                                        \
    CANTP_SCHM_EXIT_CANTP(); \
  } while(0U)



/*--------------[access macros for external functions]-----------------------*/

#if (defined CANTP_DET_REPORT_ERROR) /* to prevent double declaration */
#error CANTP_DET_REPORT_ERROR is already defined
#endif /* if (defined CANTP_DET_REPORT_ERROR) */

/** \brief Macro to call external function Det_ReportError() */
/* Deviation MISRAC2012-1 <START> */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_DET_REPORT_ERROR          (void)(CanTp_InitVars.ExitTablePtr->DetReportError)
#else
#define CANTP_DET_REPORT_ERROR          (void)Det_ReportError
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */
/* Deviation MISRAC2012-1 <STOP> */


#if (defined CANTP_CANIF_TRANSMIT) /* to prevent double declaration */
#error CANTP_CANIF_TRANSMIT is already defined
#endif /* if (defined CANTP_CANIF_TRANSMIT) */

/** \brief Macro to call external function CanIf_Transmit() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_CANIF_TRANSMIT            (CanTp_InitVars.ExitTablePtr->CanIfTransmit)
#else
#define CANTP_CANIF_TRANSMIT            CanIf_Transmit
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_PDUR_RX_INDICATION) /* to prevent double declaration */
#error CANTP_PDUR_RX_INDICATION is already defined
#endif /* if (defined CANTP_PDUR_RX_INDICATION) */

/** \brief Macro to call external function PduR_CanTpRxIndication() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_PDUR_RX_INDICATION        (CanTp_InitVars.ExitTablePtr->PduRCanTpRxIndication)
#else
#define CANTP_PDUR_RX_INDICATION        PduR_CanTpRxIndication
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_PDUR_START_OF_RECEPTION) /* to prevent double declaration */
#error CANTP_PDUR_START_OF_RECEPTION is already defined
#endif /* if (defined CANTP_PDUR_START_OF_RECEPTION) */

/** \brief Macro to call external function PduR_CanTpStartOfReception() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_PDUR_START_OF_RECEPTION    (CanTp_InitVars.ExitTablePtr->PduRCanTpStartOfReception)
#else
#define CANTP_PDUR_START_OF_RECEPTION    PduR_CanTpStartOfReception
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_PDUR_COPY_RX_DATA) /* to prevent double declaration */
#error CANTP_PDUR_COPY_RX_DATA is already defined
#endif /* if (defined CANTP_PDUR_COPY_RX_DATA) */

/** \brief Macro to call external function PduR_CanTpCopyRxData() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_PDUR_COPY_RX_DATA    (CanTp_InitVars.ExitTablePtr->PduRCanTpCopyRxData)
#else
#define CANTP_PDUR_COPY_RX_DATA    PduR_CanTpCopyRxData
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_PDUR_COPY_TX_DATA) /* to prevent double declaration */
#error CANTP_PDUR_COPY_TX_DATA is already defined
#endif /* if (defined CANTP_PDUR_COPY_TX_DATA) */

/** \brief Macro to call external function PduR_CanTpCopyTxData() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_PDUR_COPY_TX_DATA    (CanTp_InitVars.ExitTablePtr->PduRCanTpCopyTxData)
#else
#define CANTP_PDUR_COPY_TX_DATA    PduR_CanTpCopyTxData
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_PDUR_TX_CONFIRMATION) /* to prevent double declaration */
#error CANTP_PDUR_TX_CONFIRMATION is already defined
#endif /* if (defined CANTP_PDUR_TX_CONFIRMATION) */

/** \brief Macro to call external function PduR_CanTpTxConfirmation() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_PDUR_TX_CONFIRMATION      (CanTp_InitVars.ExitTablePtr->PduRCanTpTxConfirmation)
#else
#define CANTP_PDUR_TX_CONFIRMATION      PduR_CanTpTxConfirmation
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_SCHM_ENTER_CANTP) /* to prevent double declaration */
#error CANTP_SCHM_ENTER_CANTP is already defined
#endif /* if (defined CANTP_SCHM_ENTER_CANTP) */

/** \brief Macro to call external function SchM_Enter_CanTp() */
#if(CANTP_EXIT_JUMPTABLE_WITH_SCHM == STD_ON)
#define CANTP_SCHM_ENTER_CANTP          (CanTp_InitVars.ExitTablePtr->SchMEnterCanTp)
#else
#define CANTP_SCHM_ENTER_CANTP          SchM_Enter_CanTp_SCHM_CANTP_EXCLUSIVE_AREA_0
#endif /* CANTP_EXIT_JUMPTABLE_WITH_SCHM == STD_ON */


#if (defined CANTP_SCHM_EXIT_CANTP) /* to prevent double declaration */
#error CANTP_SCHM_EXIT_CANTP is already defined
#endif /* if (defined CANTP_SCHM_EXIT_CANTP) */

/** \brief Macro to call external function SchM_Exit_CanTp() */
#if(CANTP_EXIT_JUMPTABLE_WITH_SCHM == STD_ON)
#define CANTP_SCHM_EXIT_CANTP           (CanTp_InitVars.ExitTablePtr->SchMExitCanTp)
#else
#define CANTP_SCHM_EXIT_CANTP           SchM_Exit_CanTp_SCHM_CANTP_EXCLUSIVE_AREA_0
#endif /* CANTP_EXIT_JUMPTABLE_WITH_SCHM == STD_ON */



#if(CANTP_USE_GPT == STD_ON)

#if (defined CANTP_GPT_ENABLE_NOTIFICATION) /* to prevent double declaration */
#error CANTP_GPT_ENABLE_NOTIFICATION is already defined
#endif /* if (defined CANTP_GPT_ENABLE_NOTIFICATION) */

/** \brief Macro to call external function Gpt_EnableNotification() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_GPT_ENABLE_NOTIFICATION   (CanTp_InitVars.ExitTablePtr->GptEnableNotification)
#else
#define CANTP_GPT_ENABLE_NOTIFICATION   Gpt_EnableNotification
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_GPT_START_TIMER) /* to prevent double declaration */
#error CANTP_GPT_START_TIMER is already defined
#endif /* if (defined CANTP_GPT_START_TIMER) */

/** \brief Macro to call external function Gpt_StartTimer() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_GPT_START_TIMER           (CanTp_InitVars.ExitTablePtr->GptStartTimer)
#else
#define CANTP_GPT_START_TIMER           Gpt_StartTimer
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */


#if (defined CANTP_GPT_STOP_TIMER) /* to prevent double declaration */
#error CANTP_GPT_STOP_TIMER is already defined
#endif /* if (defined CANTP_GPT_STOP_TIMER) */

/** \brief Macro to call external function Gpt_StopTimer() */
#if(CANTP_PROVIDE_JUMPTABLE == STD_ON)
#define CANTP_GPT_STOP_TIMER            (CanTp_InitVars.ExitTablePtr->GptStopTimer)
#else
#define CANTP_GPT_STOP_TIMER            Gpt_StopTimer
#endif /* CANTP_PROVIDE_JUMPTABLE == STD_ON */

#endif /* CANTP_USE_GPT == STD_ON */


/*--------------[optimization related macros ]-------------------------------*/


#if (defined CANTP_GET_FIRST_N_PCI_BYTE) /* to prevent double declaration */
#error CANTP_GET_FIRST_N_PCI_BYTE is already defined
#endif /* if (defined CANTP_GET_FIRST_N_PCI_BYTE) */

/** \brief Byte index in CAN frame for first N_PCI byte
 **
 ** This macro delivers the index of the first N_PCI byte of the N-PDU given
 ** as a pointer as parameter. It is used for length calculations throughout
 ** the module.
 **
 ** \param[in] PduPtr Pointer to element of type CanTp_RxNPduTableType,
 **            CanTp_RxNSduConfigType or CanTp_TxNSduConfigType
 **
 ** \return Index of first N_PCI byte.
 ** \retval 0U if the N-PDU uses standard or normal fixed addressing format
 ** \retval 1U if the N-PDU uses mixed or extended addressing format
 */
#define CANTP_GET_FIRST_N_PCI_BYTE( PduPtr )   \
  ( ((CANTP_GET_ADDR_FORMAT((PduPtr)) == CANTP_STANDARD) || \
    (CANTP_GET_ADDR_FORMAT((PduPtr)) == CANTP_NORMALFIXED)) ? 0U : 1U )

/*--------------[relocatable postbuild related macros]-----------------------*/

#if (defined CANTP_PBCFG_ACCESS) /* to prevent double declaration */
#error CANTP_PBCFG_ACCESS is already defined
#endif /* if (defined CANTP_PBCFG_ACCESS) */

#define CANTP_PBCFG_ACCESS(type, element) \
    (TS_UNCHECKEDGETCFG(CanTp_CfgPtr, type, CANTP, (element)))


#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

#if (defined CANTP_CFG_GET_RXNPDU_LOOKUPTABLE) /* to prevent double declaration */
#error CANTP_CFG_GET_RXNPDU_LOOKUPTABLE is already defined
#endif /* if (defined CANTP_CFG_GET_RXNPDU_LOOKUPTABLE) */

#define CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_RxNPduLookupEntryType, CanTp_CfgPtr->RxNPduLookupTablePtr)[(Index)])


#if (defined CANTP_CFG_GET_RXNPDU_TABLE) /* to prevent double declaration */
#error CANTP_CFG_GET_RXNPDU_TABLE is already defined
#endif /* if (defined CANTP_CFG_GET_RXNPDU_TABLE) */

#define CANTP_CFG_GET_RXNPDU_TABLE(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_RxNPduTableType, CanTp_CfgPtr->RxNPduTablePtr)[(Index)])


#if (defined CANTP_CFG_GET_RXNSDU_CONFIG) /* to prevent double declaration */
#error CANTP_CFG_GET_RXNSDU_CONFIG is already defined
#endif /* if (defined CANTP_CFG_GET_RXNSDU_CONFIG) */

#define CANTP_CFG_GET_RXNSDU_CONFIG(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_RxNSduConfigType, CanTp_CfgPtr->RxNSduConfigPtr)[(Index)])


#if (defined CANTP_CFG_GET_TXNSDU_CONFIG) /* to prevent double declaration */
#error CANTP_CFG_GET_TXNSDU_CONFIG is already defined
#endif /* if (defined CANTP_CFG_GET_TXNSDU_CONFIG) */

#define CANTP_CFG_GET_TXNSDU_CONFIG(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_TxNSduConfigType, CanTp_CfgPtr->TxNSduConfigPtr)[(Index)])

#if(CANTP_USE_GPT == STD_ON)

#if (defined CANTP_CFG_GET_GPT_CHANNEL) /* to prevent double declaration */
#error CANTP_CFG_GET_GPT_CHANNEL is already defined
#endif /* if (defined CANTP_CFG_GET_GPT_CHANNEL) */

#define CANTP_CFG_GET_GPT_CHANNEL(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_GptChannelType, CanTp_CfgPtr->GptChannelPtr)[(Index)])

#endif /* CANTP_USE_GPT == STD_ON */


#if (defined CANTP_CFG_GET_MFCTR_CONFIG) /* to prevent double declaration */
#error CANTP_CFG_GET_MFCTR_CONFIG is already defined
#endif /* if (defined CANTP_CFG_GET_MFCTR_CONFIG) */

#define CANTP_CFG_GET_MFCTR_CONFIG(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_MfCtrConfigType, CanTp_CfgPtr->MfCtrConfigPtr)[(Index)])


#if (defined CANTP_CFG_GET_TXNPDU_TPCHANNEL) /* to prevent double declaration */
#error CANTP_CFG_GET_TXNPDU_TPCHANNEL is already defined
#endif /* if (defined CANTP_CFG_GET_TXNPDU_TPCHANNEL) */

#define CANTP_CFG_GET_TXNPDU_TPCHANNEL(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_TxNPduToTpChannelType, CanTp_CfgPtr->TxNPduToTpChannelPtr)[(Index)].Tx_Chan)

#if (defined CANTP_CFG_GET_TXFCNPDU_TPCHANNEL) /* to prevent double declaration */
#error CANTP_CFG_GET_TXFCNPDU_TPCHANNEL is already defined
#endif /* if (defined CANTP_CFG_GET_TXFCNPDU_TPCHANNEL) */

#define CANTP_CFG_GET_TXFCNPDU_TPCHANNEL(Index)   \
    (CANTP_PBCFG_ACCESS(CanTp_TxNPduToTpChannelType, CanTp_CfgPtr->TxNPduToTpChannelPtr)[(Index)].Rx_Chan)

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

#define CANTP_START_SEC_VAR_INIT_UNSPECIFIED
#include <CanTp_MemMap.h>

/** \brief CanTp global configuration */
extern P2CONST(CanTp_ConfigType, CANTP_VAR, CANTP_APPL_CONST) CanTp_CfgPtr;

#define CANTP_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <CanTp_MemMap.h>

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

/** \brief Initialization of channel pointer
 **
 ** This function prepares the channel pointer for the next transmission
 ** and makes a clean up from the last transmission.
 **
 ** \param[in] ChannelPtr  pointer to channel data structure which will be cleaned
 */
extern FUNC(void, CANTP_CODE) CanTp_InitChannelPtr
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);


/** \brief Request buffer for an I-PDU to transmit and continue transmission
 **
 ** This function handles the internal TX states. A transition from one state
 ** to another shall be performed by calling this state periodically. In
 ** addition CanTp_Transmit, CanTp_Rxindication(), CanTp_TxConfirmation() and
 ** timeouts may also initiate a transition.
 **
 ** Preconditions:
 ** - the given channel mode is CANTP_CH_MODE_TX.
 **
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 */
FUNC(void, CANTP_CODE) CanTp_TxStateHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  boolean CFBufferRequested
);


/** \brief Start of reception
 **
 ** This function indicates the upper layer about a new incoming reception. And
 ** handles the return value of the upper layer.
 **
 ** Preconditions:
 ** - the given channel is in state CANTP_CH_RX_BUFREQ_SF or
 **   CANTP_CH_RX_BUFREQ_FF
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 **
 ** \return Result of last buffer request
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_ProcessStartOfReception
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);


/** \brief Copying received N-SDU data and requesting block size
 **
 ** This function requests buffer from upper layer to copy the received data
 ** of a SF or FF. In case that all data are already copied this function requests
 ** buffer from the upper layer to be used for block size calculation.
 **
 ** \pre The given channel is in state CANTP_CH_RX_BUFREQ_SF or
 **      CANTP_CH_RX_BUFREQ_FF. It is allowed to request a bigger buffer
 **      using NSduDataLength = 0U.
 **
 ** \param[in] ChannelPtr  Pointer to channel data structure to ease lookups
 ** \param[in] NPduDataPtr Pointer to the received data for the upper layer.
 ** \param[in] NSduDataLength Length of the received data indicated by NPduDataPtr
 **
 ** \return The function returns the status of the data copy request.
 **
 ** \retval BUFREQ_OK The function will return BUFREQ_OK, all data are copied to
 **                   upper layer.
 */
extern FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_ProcessStartOfReceptionData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(uint8, AUTOMATIC, CANTP_VAR) NPduDataPtr,
  uint8 NSduDataLength
);

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
/** \brief Rx state handling
 **
 ** This function handles the internal RX states. A transition from one state
 ** to another shall be performed by calling this state periodically. In
 ** addition CanTp_Rxindication(), CanTp_TxConfirmation() and timeouts may also
 ** initiate a transition.
 **
 **  ** Preconditions:
 ** - the given channel mode is CANTP_CH_MODE_RX.
 **
 ** \param[in] ChannelPtr  Pointer to channel data structure to ease lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_RxStateHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);
#endif

/** \brief Prepares a SF, FF or a CF
 **
 ** This function prepares a SF, FF or a CF to transmit via CanIf. It fills
 ** the header according to the frame type and copies the payload from the
 ** temporary buffer.
 **
 ** Preconditions:
 ** - The temporary buffer must hold the payload suitable for the next frame.
 ** - note: the channel state is not checked to be one of the above states!
 **   if the function is called while the channel is in a wrong state,
 **   random/corrupt data may be sent!
 **
 ** \param[in] ChannelPtr  pointer to channel that should prepare the xF
 **
 ** Note that PayloadSize shall already be calculated by the caller.
 */
extern FUNC(void, CANTP_CODE) CanTp_PreparexF
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief Prepares a FC frame
 **
 ** This function transmits a flow control frame.
 ** The block size is calculated automatically.
 **
 ** \param[in] ChannelPtr - Pointer to Rx channel that requires a FC.
 ** \param[in] FcNPCI_Header - Parameter defines the FC NPCI header byte.
 */
extern FUNC(void, CANTP_CODE) CanTp_PrepareFC
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 FcNPCI_Header
);

/** \brief Transmits a CAN frame via CanIf
 **
 ** This function transmits a frame via CanIf
 ** - on busy the transmit will be repeated later
 ** - on error the whole transmission will be stopped
 **
 ** Preconditions:
 ** - data must be prepared for transmission
 ** - Channel must be in a state with action TRANSMIT.
 **
 ** \param[in] ChannelPtr    pointer to channel data structure in RAM
 ** \param[in] FCBufferPtr   pointer to FC data structure in stack
 */
extern FUNC(void, CANTP_CODE) CanTp_TransmitFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
);


/** \brief Indirect callback function for STmin timeout
 **
 ** This callback function is called indirectly either by the General Purpose
 ** Timer or by the CanTp_MainFunction (depending on configuration).
 ** The function is called when STmin for the next CF is over. The CF is sent
 ** immediately.
 **
 ** \param[in] Channel  Channel to send next consecutive frame
 */
FUNC(void, CANTP_CODE) CanTp_STminCallback
(
  CanTp_ChType Channel
);


/** \brief Stops a connection channel
 **
 ** This function brings the given channel to idle mode
 ** If ULIndication is CANTP_ULIND_STATUS, the upper layer will be informed of
 ** the stopped transmission or reception.
 **
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 ** \param[in] ULIndication  info if upper layer shall be informed or not
 ** \param[in] Result  result of the connection
 */
extern FUNC(void, CANTP_CODE) CanTp_StopChannel
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  CanTp_ULIndicationType ULIndication,
  NotifResultType Result
);

/** \brief Unlock a channel
 **
 ** This function releases a connection channel and takes care of any
 ** postticks.
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to simplify lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_HandlePostponedFlags
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief Handles Timeouts
 **
 ** This function handles different timeout conditions and notifies PduR with
 ** corresponding timeout errors
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to simplify lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_TimeoutHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);


/** \brief Calculate the block size
 **
 ** This function calculates the block size based on the available buffer.
 **
 ** \pre The block size shall only be calculated before sending FC(CTS) of a
 **      received first frame.
 **
 ** \param[in] ChannelPtr Pointer to Rx channel which needs block size calculation.
 ** \param[out] BlockSizePtr Parameter pointer returns the result of the block size calculation.
 **
 ** \return The function returns the result of the calculation.
 **
 ** \retval E_OK The calculation was successful. The block size is provided via
 **              output parameter.
 ** \retval E_NOT_OK Provided buffer is too small. Output parameter shall not be used.
 **
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CalculateBlockSize
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(uint8, AUTOMATIC, CANTP_VAR) BlockSizePtr
);


/** \brief Verify the block size
 **
 ** This function compares the block size of the channel with the provided available buffer
 ** of the upper layer.
 **
 **
 ** \param[in] ChannelPtr  Pointer to Rx channel which needs block size verification.
 **
 ** \return The function returns the result of the comparison
 **
 ** \retval TRUE The function will return TRUE, if the the available buffer suits
 **         the actual block size requirement.
 ** \retval FALSE The function will return FALSE, if the available buffer is smaller
 **               than the actual block size.
 */
extern FUNC(boolean, CANTP_CODE) CanTp_VerifyBlockSize
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);


/** \brief Copy N-Sdu data to upper layer.
 **
 ** This function tries to copy the provided data to the upper layer. Depending on
 ** the return value of the copy process the available upper layer buffer will be
 ** provided through the channel structure.
 **
 ** \pre The data to copy shall fit into the available buffer. It is allowed to
 **      request a bigger buffer using NPduDataPtr = NULL_PTR and NSduDataLength = 0U.
 **
 ** \param[in] ChannelPtr  Pointer to the channel structure.
 ** \param[in] NPduDataPtr Pointer to the received data for the upper layer.
 ** \param[in] NSduDataLength Length of the received data indicated by NPduDataPtr
 **
 ** \return The function returns the status of the data copy request.
 **
 */
extern FUNC(BufReq_ReturnType, CANTP_CODE) CanTp_RxProcessData
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(uint8, AUTOMATIC, CANTP_VAR) NPduDataPtr,
  uint8 NSduDataLength
);


/** \brief Check and handle wait frame timeout.
 **
 ** This function checks the timeout N_Br and starts the sending
 ** process of the wait frame if timeout has elapsed.
 **
 ** \pre Shall be used if timeout N_Br is set and wait frame sending is required.
 **
 ** \param[in] ChannelPtr  Pointer to the channel structure.
 **
 */
extern FUNC(void, CANTP_CODE) CanTp_CheckAndHandleWaitFrameTimeOut
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
/** \brief Processes postponed Rx indication
 **
 ** This function processes postponed Rx indication which occurs during
 ** channel lock.
 **
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_PostRxIndication
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);
#endif

/** \brief Processes postponed Tx confirmation
 **
 ** This function processes postponed Tx confirmation which occurs during
 ** channel lock.
 **
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_PostTxConfirmation
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);


/** \brief Processes postponed STmin timeout
 **
 ** This function processes postponed STmin timeout which occurs during
 ** channel lock.
 **
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to ease lookups
 */
extern FUNC(void, CANTP_CODE) CanTp_PostSTminCallback
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief Prepare padding
 **
 ** This function fills a frame with padding bytes. For 8 byte frames this is only performed if
 ** padding is enabled. For FD frames up to 64 bytes this will always be performed to the next valid
 ** size.
 **
 **
 ** \param[in] ChannelPtr - Pointer to channel data structure to ease lookups
 ** \param[in] Padding - Parameter defines if padding shall be performed or not.
 ** \param[in] CanIdType - (Standard or extended) ID for (CAN2.0 or CANFD) or
 **                        (CANTP_TX_NO_FD_CANID if CANFD is not enabled).
 ** \param[in] FCBufferPtr - pointer to FC data structure in stack
 */
extern FUNC(void, CANTP_CODE) CanTp_PreparePadding
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 Padding,
  uint8 CanIdType,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
);

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

#if(CANTP_DYNAMIC_NSA_API == STD_ON)

/** \brief Get source address index
 **
 ** This function retrieves the source address index for the provided N-SDU ID and direction.
 ** This source address index shall be used to access the dynamic source address array to get the
 ** dynamic source address which corresponds to the N-SDU given.
 **
 ** \param[in] NSduId - Id for a provided NSdu either (TxNSdu or RxNSdu)
 ** \param[in] Direction - defining the direction either send or receive
 ** \param[out] NSaIndexPtr - to fill with the found source address index
 **
 ** \return The function returns the status of the returned index.
 **
 ** \retval E_OK Source address index found and returned.
 ** \retval E_NOT_OK Provided parameters are not correct. Output parameter shall not be used.
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetNSaIndex
(
  PduIdType NSduId,
  uint8 Direction,
  P2VAR(CanTp_RxNPduLookupEntryType, AUTOMATIC, CANTP_VAR) NSaIndexPtr
);

#endif /* CANTP_DYNAMIC_NSA_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

/** \brief Transmit the postponed frame
 **
 ** This function checks if there is a postponed frame and sends it.
 **
 ** \param[in]  TxChannelID     Index of the tx channel.
 **
 */
extern FUNC(void, CANTP_CODE) CanTp_TransmitPostponedFrame
(
  CanTp_TxChannelType TxChannelID
);

/** \brief Transmit the postponed flow control
 **
 ** This function checks if there is a postponed FC and sends it.
 **
 ** \param[in]  RxChannelID     Index of the rx channel.
 **
 */
extern FUNC(void, CANTP_CODE) CanTp_TransmitPostponedFlowControl
(
  CanTp_ChType RxChannelID
);

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)

/** \brief Get the locked channel or the first released channel
 **
 ** This function returns the index of the channel.
 **
 ** \param[in]  TxNSduId      TxNSduId for the current TxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The first released channel or the locked one, was found and written to the address
 ** \retval E_NOT_OK No free channel was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetTxChannelIndex
(
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType TxNSduId
);

/** \brief Get the locked Tx channel for the corresponding N-SDU
 **
 ** This function returns the index of the locked channel.
 **
 ** \param[in]  TxNSduId      TxNSduId for the current TxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The locked channel of the N-SDU was found and written to the address
 ** \retval E_NOT_OK No channel involved in transmission with the TxNSduId was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetLockedTxChannelIndex
(
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType TxNSduId
);


/** \brief Get the locked channel or the first released one
 **
 ** This function returns the index of the channel.
 **
 ** \param[in]  RxNSduId      Currently received RxNSduId
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The first released channel or the locked one corresponding to NSDUId,
 **               was found and written to the address
 ** \retval E_NOT_OK No Rx channel was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetRxChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
);

/** \brief Get the locked Rx channel for the corresponding N-SDU
 **
 ** This function returns the index of the free channel.
 **
 ** \param[in]  RxNSduId          RxNSduId for the current RxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr     Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK     The locked channel of the N-SDU was found and written to the address
 ** \retval E_NOT_OK No channel involved in a reception with the RxNSduId was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_FindRxNsduChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
);

/** \brief Get the Rx channel for the corresponding N-SDU
 **
 ** This function returns the index of the locked channel.
 **
 ** \param[in]  RxNSduId          RxNSduId for the current RxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr     Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK     The locked channel of the N-SDU was found and written to the address
 ** \retval E_NOT_OK No channel involved in a reception with the RxNSduId was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_FindLockedRxChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
);

/** \brief Get the channel index for the given Tx PDU ID
 **
 ** This function returns the channel index for the Tx PDU ID CanTpTxPduId.
 **
 ** \param[in] CanTpTxPduId, N-PDU Id of transmitted N-PDU
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The channel was found and written to the address
 ** \retval E_NOT_OK No channel was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetChannelOfPostponedTxPdu
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_TxChannelType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
);

/** \brief Get the channel index for the given TxFc PDU ID
 **
 ** This function returns the channel index for the TxFc PDU ID CanTpTxPduId.
 **
 ** \param[in] CanTpTxPduId, N-PDU Id of transmitted N-PDU
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The channel was found and written to the address
 ** \retval E_NOT_OK No channel was found
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetChannelOfPostponedTxFcPdu
(
  PduIdType CanTpTxPduId,
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr
);
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

/** \brief Try to send FC if corresponding PDU is not involved in a transmission.
 **
 ** \param[in] ChannelPtr  Channel pointer of the FC that should be transmitted
 ** \param[in] Type        Type of FC that should be transmitted
 **
 */
extern FUNC(void, CANTP_CODE) CanTp_TryToSendFC
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 Type
);

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef CANTP_INTERNAL_H */
/*==================[end of file]============================================*/
