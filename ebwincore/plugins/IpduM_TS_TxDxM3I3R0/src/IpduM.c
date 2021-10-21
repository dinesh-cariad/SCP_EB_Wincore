
/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3
 * A cast shall not be performed between a pointer to object type and
 * a pointer to a different object type.
 *
 * Reason:
 * For post build configuration this type cast is needed.
 *
 * MISRAC2012-2) Deviated Rule: 20.7
 * Expressions resulting from the expansion of macro parameters shall be
 * enclosed in parentheses.
 *
 * Reason:
 * The AUTOSAR compiler abstraction requires these definitions in this way
 * and the arguments cannot be enclosed in parentheses due to C syntax.
 *
 * MISRAC2012-3) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only
 * appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in
 * SEC_CODE memory sections. Objects at block scope require a different
 * memory mapping, e.g. to a SEC_VAR section, which leads to nested memory
 * sections, which is not supported by some compilers.
 *
 * MISRAC2012-4) Deviated Rule: 18.4
 * The +, -, += and -= operators should not be applied to an expression
 * of pointer type.
 *
 * Reason:
 * When assembling the Container PDU it's more efficient using pointer
 * arithmetic than the adress-of operator, indexing and helper variables.
 * (No negative index could be used)
 *
 * MISRAC2012-5) Deviated Rule: 13.2 (required)
 * The value of an expression and its persistent side effects shall be the same under
 * all permitted evaluation orders.
 *
 * Reason:
 * Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 * The side effect was checked and has no negative impact.
 *
 * MISRAC2012-6) Deviated Rule: 13.4 (required)
 * The result of an assignment operator should not be used.
 *
 * Reason:
 * The result of the assignment has to be used, the counter value has to be updated
 * externally. Expression is safe.
 *
 * MISRAC2012-7) Deviated Rule: 14.2
 * A for loop shall be well-formed.
 *
 * Reason:
 * 1)The comparison is done with the incremented loop counter to protect the last for loop entry.
 * 2)Macro COMSTACK_BF_ITERATE is an optimized pattern to iterate over bit fields.
 *   It allows the compiler to optimize the code footprint reducing number of instruction
 *   cache miss.
 *
 */


/*==================[inclusions]============================================*/
#include <IpduM_Trace.h>

#include <ComStack_Types.h>
#include <IpduM_Lcfg_Static.h>
/* !LINKSTO IPDUM.ASR40.IPDUM148_3,1 */
#include <PduR_IpduM.h>
/* !LINKSTO IPDUM.ASR40.IPDUM148_2,1 */
#include <IpduM_Cbk.h>
/* !LINKSTO IPDUM.EB.IPDUM148,1 */
#include <IpduM_Api.h>
#include <TSAtomic_Assign.h>
#include <TSMem.h>
#include <IpduM_Int.h>
#include <ComStack_Helpers.h>

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
/* !LINKSTO IPDUM.ASR40.IPDUM148_5,1 */
#include <Det.h>
#endif

/*==================[macros]================================================*/

/* Deviation MISRAC2012-5 */
/* Deviation MISRAC2012-6 */
/* Deviation MISRAC2012-7 */
#define IPDUM_COMSTACK_BF_ITERATE(Next, BitFieldPtr, Start, End) \
  (Next) = (Start); (End) > ((Next) = ComStack_FindNextOne((BitFieldPtr), (Next)));
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef IPDUM_VENDOR_ID /* configuration check */
#error IPDUM_VENDOR_ID must be defined
#endif

#if (IPDUM_VENDOR_ID != 1U) /* vendor check */
#error IPDUM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef IPDUM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error IPDUM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (IPDUM_AR_RELEASE_MAJOR_VERSION != 4U)
#error IPDUM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef IPDUM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error IPDUM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (IPDUM_AR_RELEASE_MINOR_VERSION != 0U )
#error IPDUM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef IPDUM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error IPDUM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (IPDUM_AR_RELEASE_REVISION_VERSION != 3U )
#error IPDUM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef IPDUM_SW_MAJOR_VERSION /* configuration check */
#error IPDUM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (IPDUM_SW_MAJOR_VERSION != 3U)
#error IPDUM_SW_MAJOR_VERSION wrong (!= 3U)
#endif

#ifndef IPDUM_SW_MINOR_VERSION /* configuration check */
#error IPDUM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (IPDUM_SW_MINOR_VERSION < 3U)
#error IPDUM_SW_MINOR_VERSION wrong (< 3U)
#endif

#ifndef IPDUM_SW_PATCH_VERSION /* configuration check */
#error IPDUM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (IPDUM_SW_PATCH_VERSION < 40U)
#error IPDUM_SW_PATCH_VERSION wrong (< 40U)
#endif

/*==================[type definitions]======================================*/

/* typedef to resolve MISRA Rule 16.7 violation */
#if (IPDUM_ZERO_COPY == STD_ON)
typedef P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduTxInfoType;
#else
typedef P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduTxInfoType;
#endif /* (IPDUM_ZERO_COPY == STD_ON) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(PduLengthType, IPDUM_CODE) IpduM_BitCopy
(
  P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) Dest,
  P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) Src,
  P2CONST(IpduM_CopyBitFieldType, AUTOMATIC, IPDUM_APPL_CONST) CopyBitField,
  IpduM_BitfieldArraySizeType Size
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_PreparePdu
(
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay
);
#endif

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_InsertQueue
(
  PduIdType TxPduId,
  PduIdType PduRTxConfirmationPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  uint16 DynamicPriority
);

/* Deviation MISRAC2012-2 */
IPDUM_STATIC FUNC(FUNC_P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED), IPDUM_CODE) IpduM_RemoveQueue
(
    PduIdType TxConfirmationPduId
);

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_HandleQueueFilling
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayT,
  P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartT,
  PduTxInfoType PduTxInfoPtr,
  P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) IpduMDynamicTxPduIdPtr,
  P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) PduRTxConfirmationPduIdPtr,
  P2VAR(boolean, AUTOMATIC, IPDUM_APPL_DATA) ImmediatelySendPtr
);

#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(void,IPDUM_CODE) IpduM_SetDynamicPduReadyToSend(PduIdType TxPduId);
#endif

IPDUM_STATIC FUNC(uint16, IPDUM_CODE) IpduM_GetMaxPriorityQueue(PduIdType TxPduId);
#endif /* IPDUM_DYNAMIC_PART_QUEUE == STD_ON */

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON) || \
    (IPDUM_INITIALIZATION_BY_SIGNAL_VALUE == STD_ON)
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_UpdatePdu
(
  PduIdType PduRTriggerTxPduId,
  PduIdType TxPartIndex,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay
);
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) || .. */

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetJitDataTransmit
(
  PduIdType TxPduId,
  P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPart,
  P2CONST(IpduM_TxDataType, AUTOMATIC, IPDUM_APPL_DATA) TxData,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
);

#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetStaticJitData
(
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
);
#endif

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetDynamicJitData
(
  PduIdType TxConfPduId,
  P2CONST(IpduM_TxDataType, AUTOMATIC, IPDUM_APPL_DATA) TxData,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
);
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleRxPathWay
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_HandleTxPathWay
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr
);

#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_RxCntd_BinSearch
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) outGoingPduId,
  uint32 PduHeaderIdValue
);

#if (IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleStaticRxPdu
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);
#endif /* IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleDynamicRxPdu
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleRxContainerPdu
(
  PduIdType ContainerRxIndex,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddtoDeferredList
(
  PduIdType ContainerRxIndex,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_EvalTxCntrTrgCond
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_VAR_CLEARED) TxCntrPtr,
  P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainedPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  PduLengthType Length,
  PduIdType ContainerIdx
);

IPDUM_STATIC FUNC(uint8, IPDUM_CODE) IpduM_TxCntrNewInst
(
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) QPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  PduIdType ContainerIdx,
  uint8 NumInstances
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_IDLE
(
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRamData,
  PduIdType ContainerIdx,
  uint16 SendTimeout
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_TRIGGERED_LB
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) QFull,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduLengthType ContainedLength,
  PduIdType ContainerIdx,
  PduIdType IdxContained
);

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_CntrNextTriggered
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRAMPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) InstPtr,
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) QPtr,
  PduIdType ContainerIdx,
  uint8 InstIdx
);

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_TRIGGERED_CQ
(
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) QFull,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
);

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_CntrTransmitCQ
(
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
);
#endif /* IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON */

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_CntrTransmitLB
(
  PduIdType ContainerIdx,
  PduIdType IdxContained
);

#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(Std_ReturnType , IPDUM_CODE) IpduM_CntrTT
(
  PduIdType ContainerIdx,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);
#endif

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_CntrTxConf
(
  PduIdType ContainerIdx
);

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)
/** \brief Adds a COLLECT_QUEUED Contained PDU to the Container
 *
 * Copies the associated header, length and payload of a COLLECT_QUEUED
 * Contained PDU to the container buffer.
 * Updates instance variables.
 * Must be ensured that the payload fits before calling, the function
 * does not check.
 *
 * \param[in]   PduInfoPtr     Source PDU to be stored
 * \param[in]   InstPtr        Instance pointer
 * \param[in]   HeaderId       Contained PDU header ID
 * \param[in]   BuffOffset     Current length of Container
 * \param[in]   ContainerIdx   Index of the Container (config)
 * \param[in]   IdxContained   Index of the Contained (config)
 * \param[in]   HeaderSize     Size of the Contained PDU header in bytes
 * \return void
 */
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddCntdCQ
(
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  uint32 HeaderId,
  PduLengthType BuffOffset,
  PduIdType ContainerIdx,
  PduIdType IdxContained,
  uint8 HeaderSize
);
#endif /* IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON */

/** \brief Assembles the Container consisting of Contained PDUs
 * with LAST_IS_BEST IpduMContainedTxPduCollectionSemantics
 *
 * \param[out]  pduInfoPtr      Destination PDU with SduDataPtr
 *                              pointing to buffer
 * \param[in]   ContainerTxIdx  Index of the Container (config)
 * \param[in]   NumCntd         Number of Contained PDUs in the
 *                              instance. Locked while processing.
 * \param[in]   InstIdx         Index of the Container Instance
 *
 * \return Std_ReturnType
 * \retval E_OK      At least one Contained PDU could be retrieved
 *                   by PduR_IpduMTriggerTransmit()
 * \retval E_NOT_OK  None of the Contained PDUs could be retrieved
 */

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_PackLBtoBuff
(
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr,
  PduIdType ContainerTxIdx,
  IpduM_NumCntdType NumCntd,
  uint8 InstIdx
);

/** \brief Adds the LastIsBest Contained PDU to the Container list
 *
 *  Adds the ID to the list and increments the number of
 *  Contained PDUs associated with the instance
 *
 *  No checks are performed, PDU must fit. Called from
 *  protected context.
 *
 * \param[out] AddedOutPtr   Pointer to store the boolean value
 *                           whether IdxContained was added to the list
 * \param[inout] InstPtr     Instance pointer
 * \param[in]  ContainerIdx  Idx of the Container
 * \param[in]  IdxContained  Idx of the LastIsBest ContainedPdu
 *
 *  \return void
 */
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddContainedLBToList
(
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) AddedOutPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
);

/** \brief Transmits the next pending instance of a Container
 *
 * Used when transmission is triggered from the MainFunction
 * or TxConfirmation.
 *
 * \param[inout]  CntrRAMPtr    Pointer to Container RAM Data
 * \param[in]     ContainerIdx  Idx of the Container
 * \param[in]     Event         Cause of the transmission
 *
 * \return void
 */
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_SendNextCntr
(
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRAMPtr,
  PduIdType ContainerIdx,
  uint8 Event
);

#endif /* IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#define IPDUM_START_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

#if(IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
/* Deviation MISRAC2012-3 */
IPDUM_STATIC CONST(IpduM_RxIndHandlerType, IPDUM_APPL_CONST) IpduM_RxHandlers[3U] =
{
  IpduM_HandleRxPathWay,
  IpduM_HandleRxContainerPdu,
  IpduM_AddtoDeferredList
};
#else
/* Deviation MISRAC2012-3 */
IPDUM_STATIC CONST(IpduM_RxIndHandlerType, IPDUM_APPL_CONST) IpduM_RxHandlers[1U] =
{
  IpduM_HandleRxPathWay
};
#endif


#define IPDUM_STOP_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define IPDUM_START_SEC_VAR_INIT_8
#include <IpduM_MemMap.h>

/** \brief IpduM_InitStatus
 * global variable for the actual initialization status of the IpduM */
/* !LINKSTO IPDUM.ASR40.IPDUM075,1 */
IPDUM_STATIC VAR(IpduM_StatusType, IPDUM_VAR_FAST) IpduM_InitStatus = IPDUM_UNINIT;

#define IPDUM_STOP_SEC_VAR_INIT_8
#include <IpduM_MemMap.h>


#define IPDUM_START_SEC_VAR_INIT_UNSPECIFIED
#include <IpduM_MemMap.h>

/** \brief IpduM_ConfigPtr
 * global variable for the config ptr of the IpduM */
/* !LINKSTO IPDUM.ASR40.IPDUM075,1 */
P2CONST(IpduM_ConfigType, IPDUM_VAR, IPDUM_APPL_CONST) IpduM_ConfigPtr = NULL_PTR;

#define IPDUM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <IpduM_MemMap.h>

/*==================[external function definitions]=========================*/

#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

/*--------------------------------------------------------------------------*/
FUNC(void, IPDUM_CODE) IpduM_Init
(
  P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST) Config
)
{
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
#if (IPDUM_ZERO_COPY == STD_OFF)
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayI;
#endif
  P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST) InternalConfig = Config;
  uint16 i;

  DBG_IPDUM_INIT_ENTRY(Config);

#if (IPDUM_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with an null pointer get the configuration from the
   * post build configuration manager */

  if (InternalConfig == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    (void) PbcfgM_GetConfig(
            IPDUM_MODULE_ID,
            0U, /* dummy instance ID */
            &ModuleConfig);

    InternalConfig = (P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST)) ModuleConfig;
  }
#endif

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == InternalConfig)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_INIT, IPDUM_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  else if (E_OK != IpduM_IsValidConfig(InternalConfig))
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_INIT, IPDUM_E_CONFIG_PTR_INVALID);
  }
  else
#endif /* IPDUM_DEV_ERROR_DETECT == STD_ON */
  {
    /* Config is valid, init */
    IpduM_ConfigPtr = InternalConfig;

    /* Set everything to 0 before initializing with non-0 values */
    /* !LINKSTO IpduM.dsn.Transition.Instance.Init,1 */
    TS_MemBZero(IpduM_GDataMemPtr, IpduM_GDataMemSize);

    if (FALSE == IPDUM_NULL_PTR_CHECK(IpduM_ConfigPtr->TxPathWay))
    {
      /* Deviation MISRAC2012-1 */
      TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

      for (i=0; i<IPDUM_TX_PATHWAY_LEN; i++)
      {
#if (IPDUM_ZERO_COPY == STD_OFF)
        TxPathWayI = IPDUM_CFG_GET_TX_PATHWAY(i);

        /* Initialize selector and initial values */
        /* !LINKSTO IPDUM.EB.IpduMInitializationBySignalValue.Failure,1 */
        IpduM_PreparePdu(TxPathWayI);

#if (IPDUM_INITIALIZATION_BY_SIGNAL_VALUE == STD_ON)
        /* Since the return value of the Com_TriggerTransmit is E_NOT_OK due to stopped
           signals at initialization, but initial data copied, the return value is not
           considered here */
        (void)IpduM_UpdatePdu(
                              TxPathWayI->InitialDynamicPartTrigTxHandleId,
                              TxPathWayI->InitialDynamicPartIndex,
                              TxPathWayI
                             );


#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
        /* Since the return value of the Com_TriggerTransmit is E_NOT_OK due to stopped
           signals at initialization, but initial data copied, the return value is not
           considered here */
        (void)IpduM_UpdatePdu(
                              TxPathWayI->StaticPartTrigTxHandleId,
                              TxPathWayI->StaticPartIndex,
                              TxPathWayI
                             );
#endif /* (IPDUM_STATIC_PART_EXISTS == STD_ON) */
#endif /* (IPDUM_INITIALIZATION_BY_SIGNAL_VALUE == STD_ON) */
#endif /* (IPDUM_ZERO_COPY == STD_OFF) */

        /* Initialize timeout monitor */
        TxData[i].ConfirmationTimeoutCounter = 0U;

        /* Set TxConfirmation PDU-ID to invalid value */
        TxData[i].PduRTxConfirmationPduId = IPDUM_RESERVED_PDUID;

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
        /* Set associated dynamic Tx part of confirmation ID to invalid value */
        TxData[i].DynamicPartIndex = IPDUM_RESERVED_PDUID;

        /* Set TriggerTx PDU-ID to invalid value */
        TxData[i].PduRTriggerTxPduId = IPDUM_RESERVED_PDUID;
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
        /* Initialize queue position */
        TxData[i].QueuePos = 0U;
#endif
      }
    }

#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)

    /* Set terminator bit for the bitfield, RX Containers */
    for(i = 0U; i < IPDUM_RX_CONTAINER_PROCESSING_DEFERRED_NO; i++)
    {
      P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) deferredListPtr = IPDUM_GET_DEFERRED_LIST(IPDUM_GET_RX_MF_IDX(i));

      deferredListPtr[IPDUM_RX_CONTAINER_PROCESSING_DEFERRED_NO / 32U] |= ((uint32)1U << ((IPDUM_RX_CONTAINER_PROCESSING_DEFERRED_NO) % 32U));
    }

    /* Set terminator bit for the bitfield, TX Containers */
    for(i = 0U; i < IPDUM_TX_CONTAINER_LEN; i++)
    {
      P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) cntrTxBFPtr = IPDUM_GET_TX_CNTR_CH_BF(IPDUM_GET_TX_MF_IDX(i));

      cntrTxBFPtr[IPDUM_TX_CONTAINER_LEN / 32U] |= ((uint32)1U << ((IPDUM_TX_CONTAINER_LEN) % 32U));
    }
#endif

    /* tag module as initialized */
    IpduM_InitStatus = IPDUM_INIT;

  } /* Config valid */

  DBG_IPDUM_INIT_EXIT(Config);
}
/*--------------------------------------------------------------------------*/
FUNC(void, IPDUM_CODE) IpduM_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  DBG_IPDUM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr);

  if(IPDUM_UNINIT != IpduM_InitStatus)
  {
    if(RxPduId < (IPDUM_RX_PATHWAY_LEN + IPDUM_RX_CONTAINER_LEN))
    {
      if((NULL_PTR != PduInfoPtr) && (NULL_PTR != PduInfoPtr->SduDataPtr))
      {
        IPDUM_RXIND_HANDLER(RxPduId, PduInfoPtr);
      }
#if (IPDUM_DEV_ERROR_DETECT == STD_OFF)
    }
  }
#else
      else
      {
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_PARAM_POINTER);
      }
    }
    else
    {
      IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_PARAM);
    }
  }
  else
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_UNINIT);
  }
#endif

  DBG_IPDUM_RXINDICATION_EXIT(RxPduId,PduInfoPtr);
}
/*--------------------------------------------------------------------------*/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetValue = E_NOT_OK;

  DBG_IPDUM_TRANSMIT_ENTRY(TxPduId,PduInfoPtr);

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if (IPDUM_INIT != IpduM_InitStatus)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_UNINIT);
  }
  else if (TxPduId >= (IpduM_ConfigPtr->TxPartLen + IPDUM_CONTAINED_TX_LEN))
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_PARAM);
  }
  else if (NULL_PTR == PduInfoPtr)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_PARAM_POINTER);
  }
  else
#endif /* IPDUM_DEV_ERROR_DETECT == STD_ON */
  {
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    if(TxPduId < IpduM_ConfigPtr->TxPartLen)
    {
#endif
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
      if (NULL_PTR == PduInfoPtr->SduDataPtr)
      {
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_PARAM_POINTER);
      }
#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
      else if (PduInfoPtr->SduLength > IPDUM_TX_SDU_SIZE)
      {
        /* if queuing is enabled, check that data fits into the queue */
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_PARAM);
      }
#endif /* IPDUM_DYNAMIC_PART_QUEUE == STD_ON */
      else
#endif /* IPDUM_DEV_ERROR_DETECT == STD_ON */
      {
        RetValue = IpduM_HandleTxPathWay(TxPduId, PduInfoPtr);
      }
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    }
    else
    {
      if(0U < PduInfoPtr->SduLength)
      {
        const PduIdType ContainedPduIndex = TxPduId - (PduIdType)IpduM_ConfigPtr->TxPartLen;
        const PduIdType ParentContainerIndex = IPDUM_CFG_GET_CNTD_TX(ContainedPduIndex)->IpduMContainerTxIndex;
#if(IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)
        const boolean isLastIsBest = IPDUM_CNTRTX_IS_LAST_IS_BEST(IPDUM_CFG_GET_CNTR_TX(ParentContainerIndex)->CfgFlags);

        if(!isLastIsBest)
        {
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
          if (NULL_PTR == PduInfoPtr->SduDataPtr)
          {
            IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_PARAM_POINTER);
          }
          else
#endif /* IPDUM_DEV_ERROR_DETECT == STD_ON */
          {
            RetValue = IpduM_CntrTransmitCQ(PduInfoPtr, ParentContainerIndex, ContainedPduIndex);
          }
        }
        else
#endif /* IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON */
        {
          RetValue = IpduM_CntrTransmitLB(ParentContainerIndex, ContainedPduIndex);
        }
      }
      else
      {
        RetValue = E_OK;
      }
    }
#endif /* IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
  }

  DBG_IPDUM_TRANSMIT_EXIT(RetValue,TxPduId,PduInfoPtr);
  return RetValue;
}

/*--------------------------------------------------------------------------*/
#if (IPDUM_ZERO_COPY == STD_OFF)
FUNC(Std_ReturnType , IPDUM_CODE) IpduM_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay;
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) Buffer;
  Std_ReturnType RetValue = E_NOT_OK;

  DBG_IPDUM_TRIGGERTRANSMIT_ENTRY(TxPduId,PduInfoPtr);

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if (IPDUM_INIT != IpduM_InitStatus)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_UNINIT);
  }
  else if (NULL_PTR==PduInfoPtr)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_PARAM_POINTER);
  }
  else if (NULL_PTR==PduInfoPtr->SduDataPtr)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_PARAM_POINTER);
  }
  else if (TxPduId >= (IpduM_ConfigPtr->TxPathWayLen + IPDUM_TX_CONTAINER_LEN))
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_PARAM);
  }
  else
#endif

  {
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    if(TxPduId < IpduM_ConfigPtr->TxPathWayLen)
    {
#endif
      TxPathWay = IPDUM_CFG_GET_TX_PATHWAY(TxPduId);
      /* Ignore if the SduLength is insufficient */
      if (PduInfoPtr->SduLength >= IPDUM_GET_BYTE_SIZE(TxPathWay->Size))
      {

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
        {
          P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
          IpduM_JitDataType JitDataDyn = {IPDUM_RESERVED_PDUID, IPDUM_RESERVED_PDUID};
          Std_ReturnType RetValUpdDyn;
#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
          /* static JIT update */
          IpduM_JitDataType JitDataStat = {IPDUM_RESERVED_PDUID, IPDUM_RESERVED_PDUID};

          /* get static JIT data */
          IpduM_GetStaticJitData(TxPathWay, &JitDataStat);

          /* JIT update of static part of multiplexed I-PDU, overrules initialized return value */
          RetValue =
              IpduM_UpdatePdu(JitDataStat.TxPduId, JitDataStat.TxPartIndex, TxPathWay);
#endif /* (IPDUM_STATIC_PART_EXISTS == STD_ON) */

          /* dynamic JIT update */
          /* Deviation MISRAC2012-1 */
          TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

          /* get dynamic JIT data */
          IpduM_GetDynamicJitData(TxPduId, TxData, TxPathWay, &JitDataDyn);

          /* JIT update of dynamic part of multiplexed I-PDU */
          RetValUpdDyn =
              IpduM_UpdatePdu(JitDataDyn.TxPduId, JitDataDyn.TxPartIndex, TxPathWay);

          /* Error is returned if Up_TriggerTransmit is called erroneous either by static or dynamic
           part */
          if (RetValUpdDyn != E_OK)
          {
            RetValue = E_NOT_OK;
          }
        }

        if(RetValue == E_OK)
#endif /* IPDUM_ENABLE_JIT_UPDATE == STD_ON */
        {
          /* Get pointer to internal buffer */
          Buffer = &IpduM_GDataMemPtr[TxPathWay->BufferOffset];

          SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

          /* Copy data to output buffer */
          PduInfoPtr->SduLength = IPDUM_GET_BYTE_SIZE(TxPathWay->Size);
          IPDUM_COPY(PduInfoPtr->SduDataPtr, Buffer, PduInfoPtr->SduLength);

          SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

          RetValue = E_OK;
        }
      }
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_PARAM);
      }
#endif
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    }
    else
    {
      RetValue = IpduM_CntrTT(TxPduId - (PduIdType)IPDUM_TX_PATHWAY_LEN, PduInfoPtr);
    }
#endif /* IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
  }

  DBG_IPDUM_TRIGGERTRANSMIT_EXIT(RetValue,TxPduId,PduInfoPtr);

  return RetValue;
}
#endif /* IPDUM_ZERO_COPY == STD_OFF */
/*--------------------------------------------------------------------------*/
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation
(
  PduIdType TxPduId
)
{
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayC;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
  PduIdType TxDynConfirmHandleId = IPDUM_RESERVED_PDUID;

  DBG_IPDUM_TXCONFIRMATION_ENTRY(TxPduId);

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if (IPDUM_INIT != IpduM_InitStatus)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TX_CONFIRMATION, IPDUM_E_UNINIT);
  }
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
  else if (TxPduId >= (IpduM_ConfigPtr->TxPathWayLen + IpduM_ConfigPtr->TxContainerPduNum))
#else
  else if (TxPduId >= IpduM_ConfigPtr->TxPathWayLen)
#endif
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_TX_CONFIRMATION, IPDUM_E_PARAM);
  }
  else
#endif

  {
#if(IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    if(TxPduId < IpduM_ConfigPtr->TxPathWayLen)
    {
#endif
      boolean TxConfirmFlag = FALSE;

      /* Deviation MISRAC2012-1 */
      TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

      TxPathWayC = IPDUM_CFG_GET_TX_PATHWAY(TxPduId);

      SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

      /* Check if we are waiting for a confirmation */
      if ((TxData[TxPduId].ConfirmationTimeoutCounter > 0U) || (TxPathWayC->ConfirmationTimeout == 0U))
      {
        if(TxData[TxPduId].ConfirmationTimeoutCounter == IPDUM_TRANSMIT_ONGOING)
        {
          /* Signal not to start the time out counter, because the PDU is confirmed before the function
           * IpduM_Transmit() has finished */
          TxData[TxPduId].ConfirmationTimeoutCounter = IPDUM_TRANSMIT_ONGOING_CONFIRMED;
        }
        else
        {
          /* Stop timeout counter */
          TxData[TxPduId].ConfirmationTimeoutCounter = 0U;
        }

        /* In the case there is a dynamic part get the dynamic confirmation ID to call the PduR with */
        if((IPDUM_RESERVED_PDUID != TxData[TxPduId].PduRTxConfirmationPduId) &&
            (TxPathWayC->DynamicConfirmArraySize != 0U))
        {
          /* Store handle ID for TxConfirmation of dynamic part for later use */
          TxDynConfirmHandleId = TxData[TxPduId].PduRTxConfirmationPduId;

          /* Reset stored ID for confirmation of dynamic part */
          TxData[TxPduId].PduRTxConfirmationPduId = IPDUM_RESERVED_PDUID;

          /* TxData[TxPduId].PduRTriggerTxPduId and TxData[TxPduId].DynamicPartIndex are not reset,
           they refer always to the last part transmitted relevant for JIT update */
        }

        /* Call the confirmation outside the exclusive area */
        TxConfirmFlag = TRUE;
      }
      SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

      if (TxConfirmFlag == TRUE)
      {

#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
        /* Call PduR_IpduM_TxConfirmation for the static part */
        if (IPDUM_RESERVED_PDUID != TxPathWayC->StaticPartTxConfHandleId)
        {
          PduR_IpduMTxConfirmation(TxPathWayC->StaticPartTxConfHandleId);
        }
#endif

        /* do PduR_IpduMTxConfirmation() if we have found a matching dynamic part */
        if (IPDUM_RESERVED_PDUID != TxDynConfirmHandleId)
        {
          /* Call PduR_IpduM_TxConfirmation for the dynamic part */
          PduR_IpduMTxConfirmation(TxDynConfirmHandleId);
        }
      }
#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    }
    else
    {
      IpduM_CntrTxConf(TxPduId - (PduIdType)IPDUM_TX_PATHWAY_LEN);
    }
#endif /* IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
  }

  DBG_IPDUM_TXCONFIRMATION_EXIT(TxPduId);
}

#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_OFF)
FUNC(void, IPDUM_CODE) IpduM_MainFunctionRx(void)
#else
FUNC(void, IPDUM_CODE) IpduM_MainFunctionRx_Generic
(
  IpduM_MainFuncIdxType MFIdx
)
#endif
{

  DBG_IPDUM_MAINFUNCTIONRX_ENTRY();

#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
  if (IPDUM_INIT == IpduM_InitStatus)
  {
    if(0U != IPDUM_RX_CONTAINER_PROCESSING_DEFERRED_NO)
    {
      /* Deviation MISRAC2012-1 */
      P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) deferredListPtr = IPDUM_GET_DEFERRED_LIST(MFIdx);
      P2VAR(uint8, AUTOMATIC, AUTOMATIC) deferredDataPtr;
      /* Used for Containers without queuing */
      uint32 localDataBuffer[IPDUM_MAX_CONTAINER_RX_LENGTH_U32];
      PduInfoType stPduInfo;
      uint32 ContainerRxIndex;

      /* Deviation MISRAC2012-6 */
      /* Deviation MISRAC2012-7 */
      for(IPDUM_COMSTACK_BF_ITERATE(ContainerRxIndex, deferredListPtr, COMSTACK_BF_START, IPDUM_RX_CONTAINER_PROCESSING_DEFERRED_NO))
      {
#if (IPDUM_CONTAINER_QUEUING_RX == STD_ON)
        /* Deviation MISRAC2012-1 */
        P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) cntrPtr = IPDUM_CFG_GET_CNTR_RX(ContainerRxIndex);
#endif
        volatile P2VAR(IpduM_RxCntrInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile rxInstPtr;
        const PduIdType cntrByteIdx = (PduIdType)(ContainerRxIndex / 32U);
        const uint8 cntrBitIdx = ContainerRxIndex % 32U;

        /* Remove mark from container for RX MF processing */
        TS_AtomicClearBit_32(&deferredListPtr[cntrByteIdx], cntrBitIdx);

#if (IPDUM_CONTAINER_QUEUING_RX == STD_ON)
        if(cntrPtr->NumInstances > 1U)
        {
          /* volatile ptr to volatile data */
          P2VAR(IpduM_CntrQueueType, AUTOMATIC, AUTOMATIC) rxQueue = IPDUM_GET_RX_QUEUE(ContainerRxIndex);
          uint8 readIdx = rxQueue->ReadIdx;
          rxInstPtr = IPDUM_GET_RX_INST(ContainerRxIndex, readIdx);

          if(IPDUM_RX_STATE_STORED != rxInstPtr->State)
          {
            /* This happens only when overflow occurs during processing, otherwise
             * the container wouldn't be marked
             */
            readIdx = (readIdx + 1U) % cntrPtr->NumInstances;
            rxInstPtr = IPDUM_GET_RX_INST(ContainerRxIndex, readIdx);
          }

          while(IPDUM_RX_STATE_STORED == rxInstPtr->State)
          {
            rxInstPtr->State = IPDUM_RX_STATE_READING;
            /* Deviation MISRAC2012-1 */
            stPduInfo.SduDataPtr = IPDUM_GET_DEFERRED_DATA_INST(ContainerRxIndex, readIdx);
            stPduInfo.SduLength = rxInstPtr->PduLength;

            readIdx = (readIdx + 1U) % cntrPtr->NumInstances;

            IpduM_HandleRxContainerPdu((PduIdType)ContainerRxIndex, &stPduInfo);

            rxInstPtr->State = IPDUM_RX_STATE_IDLE;

            rxInstPtr = IPDUM_GET_RX_INST(ContainerRxIndex, readIdx);
          }

          rxQueue->ReadIdx = readIdx;
        }
        else
#endif
        {
          deferredDataPtr = IPDUM_GET_DEFERRED_DATA_INST(ContainerRxIndex, 0U);
          rxInstPtr = IPDUM_GET_RX_INST(ContainerRxIndex, 0U);
          stPduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, AUTOMATIC))localDataBuffer;

          SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

          stPduInfo.SduLength = rxInstPtr->PduLength;
          TS_MemCpy(localDataBuffer, deferredDataPtr, stPduInfo.SduLength);
          rxInstPtr->State = IPDUM_RX_STATE_IDLE;

          SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

          IpduM_HandleRxContainerPdu((PduIdType)ContainerRxIndex, &stPduInfo);
        }
      } /* end for */
    } /* deferred */
  }
#endif

  DBG_IPDUM_MAINFUNCTIONRX_EXIT();
}
#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_OFF)
FUNC(void, IPDUM_CODE) IpduM_MainFunctionTx(void)
#else
FUNC(void, IPDUM_CODE) IpduM_MainFunctionTx_Generic
(
  IpduM_MainFuncIdxType MFIdx
)
#endif
{
  uint16 index;
#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_ON)
  IpduM_MainFuncIdxType i;
#endif
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
#if (IPDUM_ZERO_COPY == STD_OFF)
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) TxBuffer;
#endif
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayMain;
  P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartMain;
  PduInfoType PduInfo;
  boolean TxFlag;
  PduIdType TxPduId;
#endif /* IPDUM_DYNAMIC_PART_QUEUE == STD_ON */

  DBG_IPDUM_MAINFUNCTIONTX_ENTRY();

  if (IPDUM_INIT == IpduM_InitStatus)
  {
    if(FALSE == IPDUM_NULL_PTR_CHECK(IpduM_ConfigPtr->TxPathWay))
    {
#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_OFF)
      for(index=0U; index<IPDUM_TX_PATHWAY_LEN; index++)
      {
#else
      IpduM_MainFuncIdxType mfIdxOfs = IPDUM_GET_TX_PATHWAY_MF_IDX(MFIdx);
      for(i=1U; i<=IPDUM_GET_TX_PATHWAY_MF_IDX(mfIdxOfs); i++)
      {
        index = IPDUM_GET_TX_PATHWAY_MF_IDX(mfIdxOfs + i);
#endif
#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
        TxFlag = FALSE;
        TxPduId = 0U;
#endif

        /* Deviation MISRAC2012-1 */
        TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

        SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

        /* Check for pending requests */
        if (TxData[index].ConfirmationTimeoutCounter>=IPDUM_TRANSMIT_ONGOING_CONFIRMED)
        {
          /* the main function just preempted a call to PduR_IpduMTransmit() =>
           * do nothing */
        }
        else if (TxData[index].ConfirmationTimeoutCounter>0U)
        {
          /* a call to PduR_IpduMTransmit() has completed and we're waiting
           * for the corresponding confirmation */

          /* decrease timer */
          TxData[index].ConfirmationTimeoutCounter--;
        }
#if (IPDUM_DYNAMIC_PART_QUEUE != STD_ON)
        else
        {
        }
#else /* (IPDUM_DYNAMIC_PART_QUEUE != STD_ON) */
        else
        {
          P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) QueueEntry;

          /* Get entry with highest priority */
          QueueEntry = IpduM_RemoveQueue(index);

          if (NULL_PTR != QueueEntry)
          {
#if (IPDUM_ZERO_COPY == STD_ON)
            uint8 SduDataBuffer[IPDUM_TX_SDU_SIZE];
#endif
            TxPduId = QueueEntry->TxPduId;

            /* Get pointer of TxPathWay */
            TxPathWayMain = IPDUM_CFG_GET_TX_PATHWAY(index);

            /* set timeout counter to special value for ongoing calls to
             * PduR_IpduMTransmit() - will be set to correct timeout value after
             * PduR_IpduMTransmit() has returned */
            TxData[index].ConfirmationTimeoutCounter = IPDUM_TRANSMIT_ONGOING;

            /* Set PDU Info */
            PduInfo.SduLength = IPDUM_GET_BYTE_SIZE(TxPathWayMain->Size);

            /* TxPathWay is the same, but TxPartMain may have been changed */
            TxPartMain = IPDUM_CFG_GET_TX_PART(TxPduId);

#if (IPDUM_ZERO_COPY == STD_OFF)
            /* Get pointer to static buffer */
            TxBuffer = &IpduM_GDataMemPtr[TxPathWayMain->BufferOffset];

            /* Assemble PDU */
            (void)IpduM_BitCopy(TxBuffer,
                QueueEntry->SduData,
                IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartMain),
                TxPartMain->CopyBitFieldArraySize);

            /* Set PDU Info */
            PduInfo.SduDataPtr = &TxBuffer[0];

            /* Automatically set selector value */
#if (IPDUM_AUTOMATIC_SELECTOR == STD_ON)
            IPDUM_SET_SELECTOR(PduInfo.SduDataPtr,
                TxPartMain->SelectorValue,
                TxPathWayMain->StartBit,
                TxPathWayMain->EndBit);
#endif
#else /* (IPDUM_ZERO_COPY == STD_OFF) */

            /* copy data from queue element to local buffer, since after we've
             * left the critical section another IpduM_Transmit() or the
             * IpduM_MainFunctionTx() might preempt us and modify the queue
             * adjust the source data pointer according to the start bit
             * (in the zero copy case only a single CopyBitField is allowed) */
            IPDUM_COPY(SduDataBuffer, &QueueEntry->SduData[IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartMain)->StartBit)], PduInfo.SduLength);

            /* Set PDU Info */
            PduInfo.SduDataPtr = SduDataBuffer;
#endif /* (IPDUM_ZERO_COPY == STD_OFF) */

            TxData[TxPartMain->TxConfirmationPduId].PduRTxConfirmationPduId =
                QueueEntry->PduRTxConfirmationPduId;

            TxFlag = TRUE;
          }
        }
#endif /* (IPDUM_DYNAMIC_PART_QUEUE != STD_ON) */

        SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
        if (TxFlag == TRUE)
        {
          Std_ReturnType RetValue;

          /* Get pointer of TxPathWay */
          TxPathWayMain = IPDUM_CFG_GET_TX_PATHWAY(index);

          /* Get TxPartMain */
          TxPartMain = IPDUM_CFG_GET_TX_PART(TxPduId);

          /* Transmit frame */
          RetValue = PduR_IpduMTransmit(
              TxPartMain->TxOutgoingPduId,
              &PduInfo
          );

          if (E_NOT_OK == RetValue)
          {
            /* The transmit failed, reset confirmation timer so that other frames
             * can be transmitted */
            TS_AtomicAssign16(TxData[index].ConfirmationTimeoutCounter, 0);
          }
          else
          {
            /* Start timer for timeout surveillance */
            TS_AtomicAssign16(TxData[index].ConfirmationTimeoutCounter, TxPathWayMain->ConfirmationTimeout);
          }
        }
#endif /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */
      }
    } /* no TxPathways */

#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    if(FALSE == IPDUM_NULL_PTR_CHECK(IpduM_ConfigPtr->ContainerTxPdu))
    {
      uint32 ContainerTxIdx;

      P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) txBFPtr = IPDUM_GET_TX_CNTR_CH_BF(MFIdx);
      /* Deviation MISRAC2012-6 */
      /* Deviation MISRAC2012-7 */
      for(IPDUM_COMSTACK_BF_ITERATE(ContainerTxIdx, txBFPtr, COMSTACK_BF_START, IPDUM_TX_CONTAINER_LEN))
      {
        P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRAMPtr = IPDUM_GET_TX_CNTR_RAMDATA(ContainerTxIdx);
        uint8 event = IPDUM_TXCNTR_EV_NONE;

        SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();


        if(0U < cntrRAMPtr->SendTimeout)
        {
          cntrRAMPtr->SendTimeout--;

          if(0U == cntrRAMPtr->SendTimeout)
          {
            event = IPDUM_TXCNTR_EV_SENDTOUT;
          }
        }

        if(0U < cntrRAMPtr->TxConfTimeout)
        {
          cntrRAMPtr->TxConfTimeout--;

          if(0U == cntrRAMPtr->TxConfTimeout)
          {
            event |= IPDUM_TXCNTR_EV_TXCONF;
          }
        }

        if(0U == cntrRAMPtr->TxConfTimeout)
        {
          event |= IPDUM_TXCNTR_EV_DEQUEUE;
        }

        SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

        IpduM_SendNextCntr(cntrRAMPtr, (PduIdType)ContainerTxIdx, event);

      } /* End Tx Cntr loop */

    } /* no ContainerTxPdus */
#endif /* IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
  }

  DBG_IPDUM_MAINFUNCTIONTX_EXIT();
}



/*--------------------------------------------------------------------------*/
#if (IPDUM_VERSION_INFO_API == STD_ON)
FUNC(void, IPDUM_CODE) IpduM_GetVersionInfo
(
  CONSTP2VAR(Std_VersionInfoType, AUTOMATIC, IPDUM_APPL_DATA) versioninfo
)
{
  DBG_IPDUM_GETVERSIONINFO_ENTRY(versioninfo);
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)

  if (NULL_PTR==versioninfo)
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_GET_VERSION_INFO, IPDUM_E_PARAM_POINTER);
  }
  else
#endif

  {
    versioninfo->vendorID         = IPDUM_VENDOR_ID;
    versioninfo->moduleID         = IPDUM_MODULE_ID;
    versioninfo->sw_major_version = IPDUM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = IPDUM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = IPDUM_SW_PATCH_VERSION;
  }

  DBG_IPDUM_GETVERSIONINFO_EXIT(versioninfo);
}
#endif

/*--------------------------------------------------------------------------*/

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[internal function definitions]=========================*/

#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(PduLengthType, IPDUM_CODE) IpduM_BitCopy
(
  P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) Dest,
  P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) Src,
  P2CONST(IpduM_CopyBitFieldType, AUTOMATIC, IPDUM_APPL_CONST) CopyBitField,
  IpduM_BitfieldArraySizeType Size
)
{
  IpduM_BitfieldArraySizeType i;
  uint8 SrcOffset;
  uint8 DstOffset = 0U;
  PduLengthType LastByte = 0U;

  for (i=0U; i<Size; i++)
  {
#if (IPDUM_BYTE_COPY == STD_ON)
    uint8 Length = (uint8) IPDUM_GET_BYTE_SIZE(CopyBitField[i].EndBit-CopyBitField[i].StartBit);
    SrcOffset = (uint8) IPDUM_GET_BYTE_OFFSET(CopyBitField[i].StartBit);
    DstOffset = (uint8) IPDUM_GET_BYTE_OFFSET(CopyBitField[i].DestinationBit);

    /* Copy data bytewise */
    IPDUM_COPY(&Dest[DstOffset], &Src[SrcOffset], Length);

    if (((PduLengthType) DstOffset + (PduLengthType) Length) > LastByte)
    {
        LastByte = ((PduLengthType) DstOffset + (PduLengthType) Length);
    }
#else
    uint16 j;
    uint16 Length = CopyBitField[i].EndBit-CopyBitField[i].StartBit;

    /* Copy data bitwise */
    for (j=0U; j<=Length; j++)
    {
      /* Src and Dst offset in bytes */
      SrcOffset = (uint8)((uint16)(CopyBitField[i].StartBit+j)>>3U);
      DstOffset = (uint8)((uint16)(CopyBitField[i].DestinationBit+j)>>3U);

      /* First clear bit, because destination can be initialized with any value */
      Dest[DstOffset] &= (uint8)(~(uint8)(1U<<
                          (IPDUM_MODULO_8((uint16)(CopyBitField[i].DestinationBit+j)))));

      /* Set bit */
      Dest[DstOffset] |= (uint8)(((uint8)(Src[SrcOffset]>>
                          (IPDUM_MODULO_8((uint16)(CopyBitField[i].StartBit+j))))&1U) <<
                          (IPDUM_MODULO_8((uint16)(CopyBitField[i].DestinationBit+j))));

    }
    if ( DstOffset > LastByte)
    {
        LastByte = DstOffset;
    }
#endif
  }

  return LastByte + 1U;
}
#endif

/*--------------------------------------------------------------------------*/
#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_PreparePdu
(
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay
)
{
  uint8 i;
  uint8 Size;
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) Buffer;

  Buffer = &IpduM_GDataMemPtr[TxPathWay->BufferOffset];

  /* Convert from bits to bytes */
  Size = (uint8) IPDUM_GET_BYTE_SIZE(TxPathWay->Size);

  /* set default values */
  for (i=0U; i<Size; i++)
  {
    Buffer[i] = TxPathWay->UnusedAreasDefault;
  }
}
#endif

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
/*--------------------------------------------------------------------------*/
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_InsertQueue
(
  PduIdType TxPduId,
  PduIdType PduRTxConfirmationPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  uint16 DynamicPriority
)
{
  uint8 i;
  IpduM_QueueEntryType key;
  P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) Queue;
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) QueuePosition;
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayIQ;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
  Std_ReturnType RetValue = E_NOT_OK;

  /* Get TxPathWay */
  PduIdType TxConfPduId = IPDUM_CFG_GET_TX_PART(TxPduId)->TxConfirmationPduId;
  TxPathWayIQ = IPDUM_CFG_GET_TX_PATHWAY(TxConfPduId);

  /* Insert entry if there is a queue, otherwise return E_NOT_OK to indicate that the PDU can not
   * be transmitted while a transmission is ongoing */
  if(TxPathWayIQ->QueueSize > 0U)
  {
    RetValue = E_OK;

    /* Get TxData */
    /* Deviation MISRAC2012-1 */
    TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

    /* Get start address of queue for this PDU */
    /* Deviation MISRAC2012-1 */
    Queue = (P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[TxPathWayIQ->QueueOffset]);

    /* Get Queue position */
    QueuePosition = &TxData[TxConfPduId].QueuePos;

    /* First check if PDU is already queued */
    for (i=0U; i<(*QueuePosition); i++)
    {
      if (Queue[i].TxPduId == TxPduId)
      {
        RetValue = E_NOT_OK;
      }
    }

    if (E_OK == RetValue)
    {
      if ((*QueuePosition) >= TxPathWayIQ->QueueSize)
      {
        /* Queue full, check if priority is higher than job with lowest priority */
        if ( DynamicPriority < Queue[0].DynamicPriority )
        {
          /* Priority is higher, overwrite lowest priority job */
          Queue[0].TxPduId = TxPduId;
          Queue[0].PduRTxConfirmationPduId = PduRTxConfirmationPduId;
          IPDUM_COPY(Queue[0].SduData, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
          Queue[0].DynamicPriority = DynamicPriority;

          /* Sort queue */
          i=0U;
          while(((i+1U)<TxPathWayIQ->QueueSize) && (Queue[i].DynamicPriority<Queue[i+1U].DynamicPriority))
          {
            /* Swap entries */
            key = Queue[i];
            Queue[i] = Queue[i+1U];
            Queue[i+1U] = key;
            i++;
          }
        }
        else
        {
          /* Priority is lower, job cannot be queued */
          RetValue = E_NOT_OK;
        }
      }
      else
      {
        /* Queue not full, insert entry */
        Queue[(*QueuePosition)].TxPduId = TxPduId;
        Queue[(*QueuePosition)].PduRTxConfirmationPduId = PduRTxConfirmationPduId;
        IPDUM_COPY(Queue[(*QueuePosition)].SduData, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
        Queue[(*QueuePosition)].DynamicPriority = DynamicPriority;

        /* Sort queue according to priority */
        i=(*QueuePosition);
        while((i>0U) && (Queue[i].DynamicPriority>=Queue[i-1U].DynamicPriority))
        {
          /* Swap entries */
          key = Queue[i];
          Queue[i] = Queue[i-1U];
          Queue[i-1U] = key;
          i--;
        }
        /* Set position to next free element */
        (*QueuePosition)++;
      }
    }
  }

  return RetValue;
}

/*--------------------------------------------------------------------------*/
/* Deviation MISRAC2012-2 */
IPDUM_STATIC FUNC(FUNC_P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED), IPDUM_CODE) IpduM_RemoveQueue
(
    PduIdType TxConfirmationPduId
)
{
  P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) Queue;
  P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) NextEntry;
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) QueuePosition;
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayRQ;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;

  /* Get TxPathWay */
  TxPathWayRQ = IPDUM_CFG_GET_TX_PATHWAY(TxConfirmationPduId);

  /* Get TxData */
  /* Deviation MISRAC2012-1 */
  TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

  /* Get start address of queue for this PDU */
  /* Deviation MISRAC2012-1 */
  Queue = (P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[TxPathWayRQ->QueueOffset]);

  /* Get start address of queue for this PDU */
  QueuePosition = &TxData[TxConfirmationPduId].QueuePos;

  if ( (*(QueuePosition) > 0U) &&
       ((Queue[(*QueuePosition)-1U].DynamicPriority&IPDUM_PRIO_NOT_READY_TO_SEND)!=IPDUM_PRIO_NOT_READY_TO_SEND)
     )
  {
    /* Return job with highest priority */
    NextEntry = &Queue[(*QueuePosition)-1U];

    /* Remove element */
    (*QueuePosition)--;
  }
  else
  {
    NextEntry = NULL_PTR;
  }

  return NextEntry;
}

/*--------------------------------------------------------------------------*/
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_HandleQueueFilling
(
    PduIdType TxPduId,
    P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
    P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayT,
    P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartT,
    PduTxInfoType PduTxInfoPtr,
    P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) IpduMDynamicTxPduIdPtr,
    P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) PduRTxConfirmationPduIdPtr,
    P2VAR(boolean, AUTOMATIC, IPDUM_APPL_DATA) ImmediatelySendPtr
)
{
  Std_ReturnType RetValue;
  uint16 DynPriority;

#if (IPDUM_ZERO_COPY == STD_ON)
  TS_PARAM_UNUSED(PduTxInfoPtr);
#else /* (IPDUM_ZERO_COPY == STD_ON) */
  /* Static or dynamic part */
  if (TxPduId < IpduM_ConfigPtr->TxDynamicPartOffset)
  {
    RetValue = E_OK;

    /* Static */
    if ((TxPathWayT->TriggerMode & (uint8)IPDUM_TRIGGER_STATIC) == (uint8)IPDUM_TRIGGER_STATIC)
    {
      P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) QueueEntry;

      /* Trigger condition fulfilled, increase priority for pending dynamic PDUs */
      IpduM_SetDynamicPduReadyToSend(TxPduId);

      /* Remove entry with highest priority */
      QueueEntry = IpduM_RemoveQueue(TxPartT->TxConfirmationPduId);

      /* Copy only if a dynamic part exists */
      if (NULL_PTR != QueueEntry)
      {
        /* Get pointer to static buffer */
        PduTxInfoPtr->SduDataPtr = &IpduM_GDataMemPtr[TxPathWayT->BufferOffset];

        /* Assemble dynamic PDU */
        PduTxInfoPtr->SduLength = IpduM_BitCopy(PduTxInfoPtr->SduDataPtr,
        QueueEntry->SduData,
        IPDUM_CFG_GET_TX_COPY_BITFIELD(IPDUM_CFG_GET_TX_PART(QueueEntry->TxPduId)),
        IPDUM_CFG_GET_TX_PART(QueueEntry->TxPduId)->CopyBitFieldArraySize);

        /* Store PDU IDs for dynamic part (currently retrieved from queue) */
        *IpduMDynamicTxPduIdPtr = QueueEntry->TxPduId;
        *PduRTxConfirmationPduIdPtr = QueueEntry->PduRTxConfirmationPduId;
      }

      /* And mark for immediately sending */
      *ImmediatelySendPtr = TRUE;
    }
  }
  else
#endif /* (IPDUM_ZERO_COPY == STD_ON) */
  {
    RetValue = E_OK;

    /* dynamic part (when IPDUM_ZERO_COPY == STD_ON the must not be a static part) */

    /* Set priority */
    DynPriority = TxPartT->DynamicPriority;

    /* Dynamic triggering condition fulfilled? */
    if ((TxPathWayT->TriggerMode & (uint8)IPDUM_TRIGGER_DYNAMIC) != (uint8)IPDUM_TRIGGER_DYNAMIC)
    {
      /* Trigger condition not fulfilled, mark as not to send */
      DynPriority |= IPDUM_PRIO_NOT_READY_TO_SEND;
    }

    /* Check if priority is higher than highest in queue and triggered */
    if ((DynPriority < IpduM_GetMaxPriorityQueue(TxPduId)) &&
        (DynPriority < IPDUM_PRIO_NOT_READY_TO_SEND))
    {
      /* Priority is higher than in queue, send immediately */
      *ImmediatelySendPtr = TRUE;

      /* Store PDU IDs for dynamic part (currently passed via API) */
      *IpduMDynamicTxPduIdPtr = TxPduId;
      *PduRTxConfirmationPduIdPtr = TxPartT->PduRTxConfirmationPduId;
    }
    else
    {
      /* Priority is lesser than in queue, put into queue */
      RetValue = IpduM_InsertQueue(TxPduId,
              TxPartT->PduRTxConfirmationPduId,
              PduInfoPtr,
              DynPriority);
    }
  }

  return RetValue;
}

/*--------------------------------------------------------------------------*/
#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_SetDynamicPduReadyToSend(PduIdType TxPduId)
{
  uint8 i;
  P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) Queue;
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) QueuePos;
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;

  /* Get TxPathWay */
  PduIdType TxConfPduId = IPDUM_CFG_GET_TX_PART(TxPduId)->TxConfirmationPduId;
  TxPathWay = IPDUM_CFG_GET_TX_PATHWAY(TxConfPduId);

  /* Get TxData */
  /* Deviation MISRAC2012-1 */
  TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

  /* Get start address of queue for this PDU */
  /* Deviation MISRAC2012-1 */
  Queue = (P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[TxPathWay->QueueOffset]);

  /* Get Queue position */
  QueuePos = &TxData[IPDUM_CFG_GET_TX_PART(TxPduId)->TxConfirmationPduId].QueuePos;

  /* Set PDU to ready for send */
  for(i=0; i<(*QueuePos);i++)
  {
    Queue[i].DynamicPriority &= (uint16)(~IPDUM_PRIO_NOT_READY_TO_SEND);
  }

}
#endif

/*--------------------------------------------------------------------------*/
IPDUM_STATIC FUNC(uint16, IPDUM_CODE) IpduM_GetMaxPriorityQueue(PduIdType TxPduId)
{
  uint8 QueuePosition;
  P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED) Queue;
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayGM;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
  uint16 Priority=IPDUM_PRIO_LOWEST_POSSIBLE;

  /* Get TxPathWay */
  PduIdType TxConfPduId = IPDUM_CFG_GET_TX_PART(TxPduId)->TxConfirmationPduId;
  TxPathWayGM = IPDUM_CFG_GET_TX_PATHWAY(TxConfPduId);

  /* Get TxData */
  /* Deviation MISRAC2012-1 */
  TxData = (P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

  /* Get start address of queue for this PDU */
  /* Deviation MISRAC2012-1 */
  Queue = (P2VAR(IpduM_QueueEntryType, AUTOMATIC, IPDUM_VAR_CLEARED))(&IpduM_GDataMemPtr[TxPathWayGM->QueueOffset]);

  /* Get Queue position */
  QueuePosition = TxData[TxConfPduId].QueuePos;

  if (QueuePosition > 0U)
  {
      Priority = Queue[QueuePosition - 1U].DynamicPriority;
  }

  return Priority;
}
#endif


/*--------------------------------------------------------------------------*/
#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON) || \
    (IPDUM_INITIALIZATION_BY_SIGNAL_VALUE == STD_ON)
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_UpdatePdu
(
  PduIdType PduRTriggerTxPduId,
  PduIdType TxPartIndex,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay
)
{
  Std_ReturnType RetVal;

  /* JIT update of multiplexed I-PDU */
  if (PduRTriggerTxPduId != IPDUM_RESERVED_PDUID)
  {
    CONSTP2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartU =
      IPDUM_CFG_GET_TX_PART(TxPartIndex);
    uint8 TrigTxBuffer[IPDUM_TX_SDU_SIZE];
    PduInfoType PduInfoTrigTx;

    /* prepare PDU for TriggerTransmit */
    PduInfoTrigTx.SduDataPtr = &TrigTxBuffer[0];
    PduInfoTrigTx.SduLength = IPDUM_TX_SDU_SIZE;

    /* get update of PDU */
    RetVal = PduR_IpduMTriggerTransmit(PduRTriggerTxPduId, &PduInfoTrigTx);

    /* segment copied also in case of an erroneous call of the PduR within IpduM_Init,
       necessary for initialization where the Com_TriggerTransmit returns E_NOT_OK  */
    /* !LINKSTO IPDUM.EB.IpduMInitializationBySignalValue.Success,2 */
    if(E_OK == RetVal)
    {
      SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

      /* unhandled return value of PduLengthType */
      (void)IpduM_BitCopy(&IpduM_GDataMemPtr[TxPathWay->BufferOffset],
                          PduInfoTrigTx.SduDataPtr,
                          IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartU),
                          TxPartU->CopyBitFieldArraySize);

      SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();
    }
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) || .. */


#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetJitDataTransmit
(
  PduIdType TxPduId,
  P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPart,
  P2CONST(IpduM_TxDataType, AUTOMATIC, IPDUM_APPL_DATA) TxData,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
)
{

  if(TxPduId < IpduM_ConfigPtr->TxDynamicPartOffset)
  /* static trigger condition fulfilled */
  {
    IpduM_GetDynamicJitData(TxPart->TxConfirmationPduId, TxData, TxPathWay, JitData);
  }
#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
  else
  /* dynamic trigger condition fulfilled */
  {
    IpduM_GetStaticJitData(TxPathWay, JitData);
  }
#endif /* (IPDUM_STATIC_PART_EXISTS == STD_ON) */

}


#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetStaticJitData
(
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
)
{
  /* initial data for JIT update */
  JitData->TxPduId = IPDUM_RESERVED_PDUID;
  JitData->TxPartIndex = IPDUM_RESERVED_PDUID;

  if(TxPathWay->StaticPartIndex != IPDUM_RESERVED_PDUID)
  {
    const boolean isJitUpdate = (boolean)IPDUM_CFG_GET_TX_PART(TxPathWay->StaticPartIndex)->JitUpdate;

    /* get PDU-ID of static part for enabled JIT */
    if(isJitUpdate != (boolean)FALSE)
    {
      JitData->TxPduId = TxPathWay->StaticPartTrigTxHandleId;
      JitData->TxPartIndex = TxPathWay->StaticPartIndex;
    }
  }
}
#endif /* (IPDUM_STATIC_PART_EXISTS == STD_ON) */


IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_GetDynamicJitData
(
  PduIdType TxConfPduId,
  P2CONST(IpduM_TxDataType, AUTOMATIC, IPDUM_APPL_DATA) TxData,
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWay,
  P2VAR(IpduM_JitDataType, AUTOMATIC, IPDUM_APPL_DATA) JitData
)
{
  PduIdType DynamicPartIndexLocal;
  TS_AtomicAssign16(DynamicPartIndexLocal, TxData[TxConfPduId].DynamicPartIndex);

  /* initial data for JIT update */
  JitData->TxPduId = IPDUM_RESERVED_PDUID;
  JitData->TxPartIndex = IPDUM_RESERVED_PDUID;

  /* did we get a dynamic PDU already? */
  if (DynamicPartIndexLocal != IPDUM_RESERVED_PDUID)
  {
    CONSTP2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartJitDyn =
        IPDUM_CFG_GET_TX_PART(DynamicPartIndexLocal);

    /* get PDU-ID of already received dynamic part for enabled JIT */
    if(TxPartJitDyn->JitUpdate)
    {
      TS_AtomicAssign16(JitData->TxPduId, TxData[TxConfPduId].PduRTriggerTxPduId);
      JitData->TxPartIndex = DynamicPartIndexLocal;
    }
  }
  else
  {
    CONSTP2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartJitDyn =
        IPDUM_CFG_GET_TX_PART(TxPathWay->InitialDynamicPartIndex);

    /* get PDU-ID of initial dynamic part for enabled JIT */
    if(TxPartJitDyn->JitUpdate)
    {
      JitData->TxPduId = TxPathWay->InitialDynamicPartTrigTxHandleId;
      JitData->TxPartIndex = TxPathWay->InitialDynamicPartIndex;
    }
  }
}
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */


IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleRxPathWay
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  uint16 i;
  IpduM_SelectorType Selector;
#if (IPDUM_ZERO_COPY == STD_OFF)
  uint8 Buffer[IPDUM_RECEIVE_STACK_SIZE];
  PduInfoType PduBufferInfo = {NULL_PTR, 0U};
#else
  PduInfoType LocalPduInfo;
  IpduM_BitfieldType Offset;
  IpduM_BitfieldType EndByte;
#endif
  P2CONST(IpduM_RxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) RxPathWayRI;
  P2CONST(IpduM_RxPartType, AUTOMATIC, IPDUM_APPL_CONST) RxDynamicPart = NULL_PTR;

  /* Get RxPathWay container */
  RxPathWayRI = IPDUM_CFG_GET_RX_PATHWAY(RxPduId);

  if(PduInfoPtr->SduLength >= IPDUM_GET_MIN_SDU_LEN((PduLengthType)RxPathWayRI->EndBit))
  {
    /* Extract selector */
    Selector = IPDUM_GET_SELECTOR(PduInfoPtr->SduDataPtr, RxPathWayRI->StartBit, RxPathWayRI->EndBit);

    /* Search for dynamic part */
    i = 0U;
    while(i < RxPathWayRI->DynamicPduArraySize)
    {
      IpduM_SelectorType SelectorValue = IPDUM_CFG_GET_RX_DYNAMIC_PART(RxPathWayRI, i)->SelectorValue;
      if (SelectorValue == Selector)
      {
        /* Found, set RxDynamicPart */
        RxDynamicPart = IPDUM_CFG_GET_RX_DYNAMIC_PART(RxPathWayRI, i);
        break;
      }
      i++;
    }

#if (IPDUM_ZERO_COPY == STD_ON)

    /* Static part exists? */
#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
    {
      if (FALSE == IPDUM_NULL_PTR_CHECK(RxPathWayRI->StaticPart))
      {
        P2CONST(IpduM_RxPartType, AUTOMATIC, IPDUM_APPL_CONST) RxStaticPart = IPDUM_CFG_GET_RX_STATIC_PART(RxPathWayRI);
        /* Calculate offset */
        Offset = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_RX_COPY_BITFIELD(RxStaticPart)->StartBit);
        EndByte = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_RX_COPY_BITFIELD(RxStaticPart)->EndBit);

        /* !LINKSTO IpduM.EB.RxPathWay.SduLength.Min.StaticPart,1 */
        if(IPDUM_GET_MIN_SDU_LEN((PduLengthType)IPDUM_CFG_GET_RX_COPY_BITFIELD(RxStaticPart)->EndBit) <= PduInfoPtr->SduLength)
        {
          /* Call rx indication for static part */
          LocalPduInfo.SduLength = (EndByte - Offset) + 1U;
          LocalPduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[Offset];
          PduR_IpduMRxIndication(RxStaticPart->OutgoingHandleId, &LocalPduInfo);
        }
      }
    }
#endif

    /* Always ensure that the selector is valid */
    if ( NULL_PTR != RxDynamicPart )
    {
      /* Calculate offset */
      Offset = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_RX_COPY_BITFIELD(RxDynamicPart)->StartBit);
      EndByte = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_RX_COPY_BITFIELD(RxDynamicPart)->EndBit);

      /* !LINKSTO IpduM.EB.RxPathWay.SduLength.Min.DynamicPart,1 */
      if(IPDUM_GET_MIN_SDU_LEN((PduLengthType)IPDUM_CFG_GET_RX_COPY_BITFIELD(RxDynamicPart)->EndBit) <= PduInfoPtr->SduLength)
      {
        /* Call rx indication for dynamic part */
        LocalPduInfo.SduLength = (EndByte - Offset) + 1U;
        LocalPduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[Offset];
        PduR_IpduMRxIndication(RxDynamicPart->OutgoingHandleId, &LocalPduInfo);
      }
    }
#else /* IPDUM_ZERO_COPY == STD_ON) */
#if (IPDUM_STATIC_PART_EXISTS == STD_ON)
    {
      if (FALSE == IPDUM_NULL_PTR_CHECK(RxPathWayRI->StaticPart))
      {
        P2CONST(IpduM_RxPartType, AUTOMATIC, IPDUM_APPL_CONST) RxStaticPart = IPDUM_CFG_GET_RX_STATIC_PART(RxPathWayRI);
        P2CONST(IpduM_CopyBitFieldType, AUTOMATIC, IPDUM_APPL_CONST) CopyBitField = IPDUM_CFG_GET_RX_COPY_BITFIELD(RxStaticPart);

        /* !LINKSTO IpduM.EB.RxPathWay.SduLength.Min.StaticPart,1 */
        if(IPDUM_GET_MIN_SDU_LEN((PduLengthType)CopyBitField[RxStaticPart->CopyBitFieldArraySize - 1U].EndBit) <= PduInfoPtr->SduLength)
        {
          PduBufferInfo.SduDataPtr = &Buffer[0];

          /* Assemble fragmented static part into temporary buffer */
          PduBufferInfo.SduLength = IpduM_BitCopy(&Buffer[0],
              PduInfoPtr->SduDataPtr,
              CopyBitField,
              RxStaticPart->CopyBitFieldArraySize);

          /* Call rx indication for static part */
          PduR_IpduMRxIndication(RxStaticPart->OutgoingHandleId, &PduBufferInfo);
        }
      }
    }
#endif

    /* Always ensure that the selector is valid */
    if ( NULL_PTR != RxDynamicPart )
    {
      P2CONST(IpduM_CopyBitFieldType, AUTOMATIC, IPDUM_APPL_CONST) CopyBitField = IPDUM_CFG_GET_RX_COPY_BITFIELD(RxDynamicPart);

      /* !LINKSTO IpduM.EB.RxPathWay.SduLength.Min.DynamicPart,1 */
      if(IPDUM_GET_MIN_SDU_LEN((PduLengthType)CopyBitField[RxDynamicPart->CopyBitFieldArraySize - 1U].EndBit) <= PduInfoPtr->SduLength)
      {
        PduBufferInfo.SduDataPtr = &Buffer[0];

        /* Assemble fragmented dynamic part into temporary buffer */
        PduBufferInfo.SduLength = IpduM_BitCopy(&Buffer[0],
            PduInfoPtr->SduDataPtr,
            CopyBitField,
            RxDynamicPart->CopyBitFieldArraySize);

        /* Call rx indication for dynamic part */
        PduR_IpduMRxIndication(RxDynamicPart->OutgoingHandleId, &PduBufferInfo);
      }
    }
#endif
  }
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_PARAM);
  }
#endif
}

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_HandleTxPathWay
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr
)
{
  P2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxPartT;
  P2CONST(IpduM_TxPathWayType, AUTOMATIC, IPDUM_APPL_CONST) TxPathWayT;
  P2VAR(IpduM_TxDataType, AUTOMATIC, IPDUM_VAR_CLEARED) TxData;
  PduIdType IpduMDynamicTxPduId = IPDUM_RESERVED_PDUID; /* local PDU-ID of dynamic part */
  PduIdType PduRTxConfirmationPduId = IPDUM_RESERVED_PDUID; /* TxConf PDU-ID of dynamic part */
#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
  uint16 DynPriority;
  boolean ImmediatelySend = FALSE;
#endif /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */
#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
  PduIdType PduRTriggerTxPduId = IPDUM_RESERVED_PDUID; /* TriggerTx PDU-ID of dynamic part */
  IpduM_JitDataType JitData = {IPDUM_RESERVED_PDUID, IPDUM_RESERVED_PDUID}; /* JIT update data */
#endif

  PduInfoType PduTxInfo = { NULL_PTR, 0U };

  Std_ReturnType RetValue = E_NOT_OK;

  /* Get TxPartT */
  TxPartT = IPDUM_CFG_GET_TX_PART(TxPduId);

  /* Deviation MISRAC2012-1 */
  TxData = (P2VAR(IpduM_TxDataType, IPDUM_VAR, IPDUM_VAR))(&IpduM_GDataMemPtr[IPDUM_TX_DATA_OFFSET]);

  TxPathWayT = IPDUM_CFG_GET_TX_PATHWAY(TxPartT->TxConfirmationPduId);

#if (IPDUM_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON)
  /* CHECK: NOPARSE */
  if (TxPathWayT->TriggerMode > (uint8)IPDUM_TRIGGER_MODE_MAX_VAL)
  {
    /* unexpected error condition - should never be reached */
    IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_SID_TRANSMIT);
  }
  else
  /* CHECK: PARSE */
#endif /* (IPDUM_DEFENSIVE_PROGRAMMING_ENABLED == STD_ON) */
  {
    boolean TxFlag = FALSE;

    SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

    /* Check if a transmission is ongoing */
    if (0 == TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter)
    {
      /* No request pending */

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
      RetValue = IpduM_HandleQueueFilling(TxPduId,
          PduInfoPtr,
          TxPathWayT,
          TxPartT,
          &PduTxInfo,
          &IpduMDynamicTxPduId,
          &PduRTxConfirmationPduId,
          &ImmediatelySend
      );
#else /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */
#if (IPDUM_ZERO_COPY == STD_OFF)
      /* dynamic part ? */
      if (TxPduId >= IpduM_ConfigPtr->TxDynamicPartOffset)
#endif /* (IPDUM_ZERO_COPY == STD_OFF) */
       /* dynamic part (when IPDUM_ZERO_COPY == STD_ON the must not be a static part) */
      {
        /* Store PDU IDs for dynamic part */
        IpduMDynamicTxPduId = TxPduId;
        PduRTxConfirmationPduId = TxPartT->PduRTxConfirmationPduId;
#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
        PduRTriggerTxPduId = TxPartT->PduRTriggerTxPduId;
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */
      }
#endif /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */

#if (IPDUM_ZERO_COPY == STD_OFF)
      /* Get pointer to static buffer */
      PduTxInfo.SduDataPtr = &IpduM_GDataMemPtr[TxPathWayT->BufferOffset];

      /* Assemble PDU */
      PduTxInfo.SduLength = IpduM_BitCopy(PduTxInfo.SduDataPtr,
          PduInfoPtr->SduDataPtr,
          IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartT),
          TxPartT->CopyBitFieldArraySize);
#endif /* (IPDUM_ZERO_COPY == STD_OFF) */

      /* In case we have updated the dynamic part, store the PDU-IDs for the dynamic part */
      if (IpduMDynamicTxPduId != IPDUM_RESERVED_PDUID)
      {
        TxData[TxPartT->TxConfirmationPduId].PduRTxConfirmationPduId = PduRTxConfirmationPduId;
#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
        TxData[TxPartT->TxConfirmationPduId].PduRTriggerTxPduId = PduRTriggerTxPduId;
        TxData[TxPartT->TxConfirmationPduId].DynamicPartIndex = TxPduId;
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */
      }

#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
      if (TRUE == ImmediatelySend)
#else /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */
        /* Trigger condition fulfilled? */
        if ( ((TxPduId < IpduM_ConfigPtr->TxDynamicPartOffset) &&
            ((TxPathWayT->TriggerMode & (uint8)IPDUM_TRIGGER_STATIC) == (uint8)IPDUM_TRIGGER_STATIC) ) ||
            ((TxPduId >= IpduM_ConfigPtr->TxDynamicPartOffset) &&
                ((TxPathWayT->TriggerMode & (uint8)IPDUM_TRIGGER_DYNAMIC) == (uint8)IPDUM_TRIGGER_DYNAMIC) )
        )
#endif /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */
        {
#if (IPDUM_ZERO_COPY == STD_ON)
          /* In case of zero copy the buffer passed as parameter (PduInfoPtr)
           * is used for further transmission. This is done by setting the SduDataPtr
           * of the outgoing PDU to the byte position containing the start bit
           * of the TX segment within the buffer provided by PduInfoPtr function parameter.
           * This requires to adapt the SduLength to the byte size of the TX segment
           * to avoid a bound violation further on when PduR_IpduMTransmit is called.
           * The generator ensures that there is only one single dynamic TX segment
           * configured. */

          /* Calculate byte offsets of the dynamic TX segment */
          const IpduM_BitfieldType StartByteOffset = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartT)->StartBit);
          const IpduM_BitfieldType EndByteOffset = IPDUM_GET_BYTE_OFFSET(IPDUM_CFG_GET_TX_COPY_BITFIELD(TxPartT)->EndBit);

          /* Calculate required SduLength in byte */
          PduTxInfo.SduLength = (EndByteOffset - StartByteOffset) + 1U;
#else /* (IPDUM_ZERO_COPY == STD_ON) */

          /* If zero copy is deactivated get the SduLength form the configured Tx pathway */
          PduTxInfo.SduLength = IPDUM_GET_BYTE_SIZE(TxPathWayT->Size);

#endif /* (IPDUM_ZERO_COPY == STD_ON) */

          /* set timeout counter to special value for ongoing calls to
           * PduR_IpduMTransmit() - will be set to correct timeout value after
           * PduR_IpduMTransmit() has returned */
          TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter = IPDUM_TRANSMIT_ONGOING;

          /* Set PDU Info */
#if (IPDUM_ZERO_COPY == STD_OFF)

          /* Automatically set selector value */
#if (IPDUM_AUTOMATIC_SELECTOR == STD_ON)
          /* do we have an updated dynamic part? */
          if (IpduMDynamicTxPduId != IPDUM_RESERVED_PDUID)
          {
            CONSTP2CONST(IpduM_TxPartType, AUTOMATIC, IPDUM_APPL_CONST) TxDynamicPart = IPDUM_CFG_GET_TX_PART(IpduMDynamicTxPduId);

            /* set the selector value based on the TxPart configuration (which is derived based on the TxPduId */
            IPDUM_SET_SELECTOR(PduTxInfo.SduDataPtr,
                TxDynamicPart->SelectorValue,
                TxPathWayT->StartBit,
                TxPathWayT->EndBit);
          }
#endif /* (IPDUM_AUTOMATIC_SELECTOR == STD_ON) */
#else /* (IPDUM_ZERO_COPY == STD_OFF) */

          /* zero copy - just adjust the data pointer according to the start bit
           * (in the zero copy case only a single CopyBitField is allowed) */
          PduTxInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[StartByteOffset];
#endif /* (IPDUM_ZERO_COPY == STD_OFF) */

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
          /* get data for JIT update */
          IpduM_GetJitDataTransmit(TxPduId, TxPartT, TxData, TxPathWayT, &JitData);
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */

          TxFlag = TRUE;
        }
#if (IPDUM_DYNAMIC_PART_QUEUE == STD_OFF)
        else
        {
          /* IpduMTxTriggerMode == NONE, it's retrieved by IpduM_TriggerTransmit(), return E_OK */
          RetValue = E_OK;
        }
#endif /* IPDUM_DYNAMIC_PART_QUEUE == STD_OFF */
    }
#if (IPDUM_DYNAMIC_PART_QUEUE == STD_ON)
    else /* Transmission ongoing */
    {
      if (TxPduId >= IpduM_ConfigPtr->TxDynamicPartOffset)
      {
        /* Set priority */
        DynPriority = TxPartT->DynamicPriority;

        /* Queue dynamic part */
        if ((TxPathWayT->TriggerMode & (uint8)IPDUM_TRIGGER_DYNAMIC) != (uint8)IPDUM_TRIGGER_DYNAMIC)
        {
          /* Trigger condition not fulfilled, mark as not to send */
          DynPriority |= IPDUM_PRIO_NOT_READY_TO_SEND;
        }

        RetValue = IpduM_InsertQueue(TxPduId,
            TxPartT->PduRTxConfirmationPduId,
            PduInfoPtr,
            DynPriority);
      }
    }
#endif /* (IPDUM_DYNAMIC_PART_QUEUE == STD_ON) */

    SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

#if (IPDUM_ENABLE_JIT_UPDATE == STD_ON)
    /* JIT update of multiplexed I-PDU if trigger condition fulfilled */
    if (TxFlag == TRUE)
    {
      Std_ReturnType RetValUpdatePdu;

      RetValUpdatePdu =
          IpduM_UpdatePdu(JitData.TxPduId, JitData.TxPartIndex, TxPathWayT);

      /* No Transmit to be called if Up_TriggerTransmit is called erroneous */
      if (RetValUpdatePdu != E_OK)
      {
        /* Reset the timeout counter, IpduM_TxTimeoutType is uint16 */
        TS_AtomicAssign16(TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter, 0U);
        /* !LINKSTO IpduM.ASR19-11.IpduM_00171.NoTransmission,1 */
        TxFlag = FALSE;
      }
    }
#endif /* (IPDUM_ENABLE_JIT_UPDATE == STD_ON) */

    /* Check whether the frame is to be transmitted */
    if (TxFlag == TRUE)
    {
      Std_ReturnType TransmitRetVal = E_NOT_OK;

      /* Transmit frame */
      TransmitRetVal = PduR_IpduMTransmit(TxPartT->TxOutgoingPduId, &PduTxInfo);

      SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

      if (E_NOT_OK == TransmitRetVal)
      {
        /* Reset confirmation timer so that a new frame can be transmitted */
        TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter = 0U;
      }
      else
      {
        switch (TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter)
        {
        case IPDUM_TRANSMIT_ONGOING:
          /* Start timer for timeout monitoring */
          TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter = TxPathWayT->ConfirmationTimeout;
          break;

        case IPDUM_TRANSMIT_ONGOING_CONFIRMED:
          /* Do not start the time out counting when the transmit of the PDU
           * was confirmed before PduR_IpduMTransmit() has completely finished */
          TxData[TxPartT->TxConfirmationPduId].ConfirmationTimeoutCounter = 0U;
          break;

          /* all possible cases have been explicitly covered in 'case's above, the 'default'
           * cannot be reached */

          /* CHECK: NOPARSE */
        default:
          IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_SID_TRANSMIT);
          break;
          /* CHECK: PARSE */
        }
      }
      RetValue = TransmitRetVal;

      SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0() ;
    }
    /* !LINKSTO IpduM.ASR19-11.IpduM_00171.RetVal,1 */
    /* RetVal remains set to E_NOT_OK */
  }
  /* TxPathWayT->TriggerMode > IPDUM_TRIGGER_MODE_MAX_VAL */

  return RetValue;
}

#if (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_RxCntd_BinSearch
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) outGoingPduId,
  uint32 PduHeaderIdValue
)
{
  P2CONST(IpduM_ContainedRxPduType, AUTOMATIC, IPDUM_APPL_CONST) cntdRxPtr = IPDUM_CFG_GET_CNTD_RX(0);
  sint32 left = ContainerPdu->ContainedRxPduFirst;
  sint32 right = (left + ContainerPdu->ContainedRxPduNum) - 1U;
  Std_ReturnType RetVal = E_NOT_OK;

  while (left <= right)
  {
    sint32 middle = (left + right) / 2U;

    if (cntdRxPtr[middle].ContainedPduHeaderId < PduHeaderIdValue)
    {
      /* If PduHeaderId greater, ignore left half */
      left = middle + 1U;
    }
    else if (cntdRxPtr[middle].ContainedPduHeaderId > PduHeaderIdValue)
    {
      /* If PduHeaderId is less, ignore right half */
      right = middle - 1U;
    }
    else
    {
      *outGoingPduId = cntdRxPtr[middle].OutgoingPduId;
      RetVal = E_OK;
      break;
    }
  }

  return RetVal;
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleDynamicRxPdu
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  uint32 ContainedDLCLength = 0U;
  PduLengthType Idx;
  PduLengthType IdxOld = 0;
  boolean Stop = FALSE;
  const PduLengthType HeaderSize = ContainerPdu->HeaderSize;
  for (Idx = 0U; ((Idx < PduInfoPtr->SduLength) && (Idx >= IdxOld)); Idx += HeaderSize + (PduLengthType)ContainedDLCLength)
  {
    const PduLengthType RemainLen = PduInfoPtr->SduLength - Idx;

    /* only continue if there are enough bytes left to provide a complete header */
    if(RemainLen >= HeaderSize)
    {
      /* initialize PduHeaderId with 0U to handle the 4th byte for short header and the next 4 bytes for long header*/
      uint32 PduHeaderId = 0U;
      /* extract PDU Header Id: first 3 bytes for short header, first 4 bytes for long header */
      /* extract DLC: 4th byte for short header, 5th-7th bytes for long header */
      /* get header type: short(0), long(1) */
      if(HeaderSize == IPDUM_HEADERTYPE_SHORT)
      {
        /* Deviation MISRAC2012-4 */
        TS_MemCpy(((uint8*)&PduHeaderId + IPDUM_HDR_ID_OFFST), &PduInfoPtr->SduDataPtr[Idx], IPDUM_SHORT_HDR_DLC_OFFST);
        ContainedDLCLength = PduInfoPtr->SduDataPtr[Idx + IPDUM_SHORT_HDR_DLC_OFFST];
      }
      else
      {
        uint32 swapValue;
        TS_MemCpy(&PduHeaderId, &PduInfoPtr->SduDataPtr[Idx], 4U);
        TS_MemCpy(&ContainedDLCLength, &PduInfoPtr->SduDataPtr[Idx + 4U], 4U);
        swapValue = IPDUM_SWAPU32_IF_MUST(ContainedDLCLength);
        ContainedDLCLength = swapValue;
      }

      /* stop when a HeaderId = 0 is encountered */
      if(PduHeaderId != 0U)
      {
        if ((ContainedDLCLength + HeaderSize) > ContainedDLCLength)
        {
          /* only continue if there are enough bytes left to cover the PDU size read from DLC */
          if(RemainLen >= (ContainedDLCLength + HeaderSize))
          {
            PduIdType outGoingPduId;

            if(E_OK == IpduM_RxCntd_BinSearch(ContainerPdu, &outGoingPduId, IPDUM_SWAPU32_IF_MUST(PduHeaderId)))
            {
              PduInfoType LocalPdu;

              LocalPdu.SduLength = (PduLengthType)ContainedDLCLength;
              LocalPdu.SduDataPtr =
                  &PduInfoPtr->SduDataPtr[Idx + HeaderSize];

              /* forward matching received PDUs to PduR */
              PduR_IpduMRxIndication(
                  outGoingPduId,
                  &LocalPdu
              );
              /* Misra allows one break statement per each for/while loop */
            }
          }
          else
          {
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
            IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_HEADER);
#endif
            Stop = TRUE;
          }
        }
        else
        {
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
          IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_HEADER);
#endif
          Stop = TRUE;
        }
      }
      else
      {
        /* stop processing if a PDU header ID equal to 0 is encountered */
        Stop = TRUE;
      }
    }

    if(Stop == TRUE)
    {
      /* Misra allows one break statement per each for/while loop */
      break;
    }
  }
}

#if (IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleStaticRxPdu
(
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  uint32 idx;
  uint32 startIdx = ContainerPdu->ContainedRxPduFirst;
  uint32 stopIdx = startIdx + ContainerPdu->ContainedRxPduNum;

  for(idx = startIdx; idx < stopIdx; idx++)
  {
    P2CONST(IpduM_StaticContainedRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainedPdu = IPDUM_CFG_GET_STATIC_CNTD_RX(idx);

    if(ContainedPdu->IpduMContainedPduOffset >= PduInfoPtr->SduLength)
    {
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
      IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_CONTAINER);
#endif
      break;
    }

    /* If it has UpdateBits it has to be set */
    if((FALSE == ContainedPdu->HasIpduMUpdateBitPositionSet) ||
       (0U < (PduInfoPtr->SduDataPtr[ContainedPdu->IpduMUpdateBitBytePosition] & ContainedPdu->IpduMUpdateBitMask)))
    {
      PduInfoType LocalPdu;

      /* only that last contained PDU
       * can have variable size
       */
      if(idx ==  (stopIdx - 1U))
      {
        PduLengthType RemainingLength = PduInfoPtr->SduLength - (PduLengthType)ContainedPdu->IpduMContainedPduOffset;

        if(RemainingLength < ContainedPdu->PduLength)
        {
          LocalPdu.SduLength = RemainingLength;
        }
        else
        {
          LocalPdu.SduLength = ContainedPdu->PduLength;
        }
      }
      else
      {
        LocalPdu.SduLength = ContainedPdu->PduLength;
      }

      LocalPdu.SduDataPtr = &PduInfoPtr->SduDataPtr[ContainedPdu->IpduMContainedPduOffset];

      PduR_IpduMRxIndication(ContainedPdu->OutgoingPduId, &LocalPdu);
    }
  }
}
#endif /* IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_HandleRxContainerPdu
(
  PduIdType ContainerRxIndex,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainerPdu =
      IPDUM_CFG_GET_CNTR_RX(ContainerRxIndex);

#if (IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
  if(ContainerPdu->HeaderSize == IPDUM_HEADERTYPE_NONE)
  {
    IpduM_HandleStaticRxPdu(ContainerPdu, PduInfoPtr);
  }
  else
#endif
  {
    IpduM_HandleDynamicRxPdu(ContainerPdu, PduInfoPtr);
  }
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddtoDeferredList
(
  PduIdType ContainerRxIndex,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  if(PduInfoPtr->SduLength <= IPDUM_CFG_GET_CNTR_RX(ContainerRxIndex)->PduLength)
  {
#if (IPDUM_CONTAINER_QUEUING_RX == STD_ON)
    /* Deviation MISRAC2012-1 */
    P2CONST(IpduM_ContainerRxPduType, AUTOMATIC, IPDUM_APPL_CONST) cntrPtr = IPDUM_CFG_GET_CNTR_RX(ContainerRxIndex);
#endif
    P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) deferredListPtr = IPDUM_GET_DEFERRED_LIST(IPDUM_GET_RX_MF_IDX(ContainerRxIndex));
    volatile P2VAR(IpduM_RxCntrInstType, AUTOMATIC, AUTOMATIC) volatile rxInst;
    P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) deferredDataPtr;
#if (IPDUM_CONTAINER_QUEUING_RX == STD_ON)
    P2VAR(IpduM_CntrQueueType,  AUTOMATIC, AUTOMATIC) rxQueue = IPDUM_GET_RX_QUEUE(ContainerRxIndex);
    uint8 writeIdx = (cntrPtr->NumInstances > 1) ? rxQueue->WriteIdx : 0U;
#else
    const uint8 writeIdx = 0U;
#endif
    const uint16 cntrByteIdx = ContainerRxIndex / (uint16)32U;
    const uint8 cntrBitIdx = ContainerRxIndex % 32U;


    rxInst = IPDUM_GET_RX_INST(ContainerRxIndex, writeIdx);

#if (IPDUM_CONTAINER_QUEUING_RX == STD_ON)
    if(cntrPtr->NumInstances > 1U)
    {
      /* !LINKSTO IpduM.EB.CntrRx.DequeueOrder.NoOverFlow,1 */
      rxQueue->WriteIdx = (writeIdx + 1U) % cntrPtr->NumInstances;

      if(IPDUM_RX_STATE_IDLE != rxInst->State)
      {
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_QUEUEOVFL);
#endif
        if(IPDUM_RX_STATE_READING == rxInst->State)
        {
          /* !LINKSTO IpduM.EB.CntrRx.DequeueOrder.OverFlowWhileReading,1 */
          /* Jump over the current one */
          writeIdx = (writeIdx + 1U) % cntrPtr->NumInstances;
          /* rxQueue->WriteIdx already incremented, not storing it */
          rxInst = IPDUM_GET_RX_INST(ContainerRxIndex, writeIdx);
        }
        else
        {
          /* !LINKSTO IpduM.EB.CntrRx.DequeueOrder.OverFlowNotRead,1 */
          /* Overflow, advance the index to dequeue the oldest */
          uint8 ReadIndex = (rxQueue->ReadIdx + 1U) % cntrPtr->NumInstances;
          rxQueue->ReadIdx = ReadIndex;
        }
      }
    }
#endif

    rxInst->State = IPDUM_RX_STATE_WRITING;

    deferredDataPtr = IPDUM_GET_DEFERRED_DATA_INST(ContainerRxIndex, writeIdx);

    rxInst->PduLength = PduInfoPtr->SduLength;
    TS_MemCpy(deferredDataPtr, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

    rxInst->State = IPDUM_RX_STATE_STORED;

    /* Mark container */
    TS_AtomicSetBit_32((volatile uint32*)&deferredListPtr[cntrByteIdx], cntrBitIdx);
  }
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    IPDUM_DET_REPORT_ERROR(IPDUM_SID_RX_INDICATION, IPDUM_E_CONTAINER);
  }
#endif /* IPDUM_DEV_ERROR_DETECT == STD_ON */
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_EvalTxCntrTrgCond
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_VAR_CLEARED) TxCntrPtr,
  P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) ContainedPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  PduLengthType Length,
  PduIdType ContainerIdx
)
{
  /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.TRG_ALWAYS,1 */
  /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.TRG_FIRST,1 */
  /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.THRESHOLD,1 */
  /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.ALL_STATIC,1 */
#if(IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_OFF)
  if ((Length + TxCntrPtr->HeaderSize) >= TxCntrPtr->PduLength)
#else
  if (((Length + TxCntrPtr->HeaderSize) >= TxCntrPtr->PduLength)
  || ((IPDUM_HEADERTYPE_NONE == TxCntrPtr->HeaderSize) && (TxCntrPtr->NumCntdMax == InstPtr->NumCntd)))
#endif
  {
    P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
    TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
    IPDUM_TX_ADVANCE_WRITE(qPtr, TxCntrPtr);

    InstPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
    *Event = (uint8)(IPDUM_TXCNTR_EV_DEQUEUE | IPDUM_TXCNTR_EV_NEW_INST_EMPTY);
  }
  else if (IPDUM_CNTND_IS_TRGGR_ALWAYS(ContainedPtr->CfgFlags)
       || (IPDUM_CNTRTX_IS_TRGGR_FIRST(TxCntrPtr->CfgFlags)
       || (Length >= TxCntrPtr->IpduMContainerTxSizeThreshold)))
  {
    InstPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
    *Event = IPDUM_TXCNTR_EV_DEQUEUE;
  }
  else
  {
    *Event = IPDUM_TXCNTR_EV_NONE;
    *RetVal = E_OK;
  }
}

IPDUM_STATIC FUNC(uint8, IPDUM_CODE) IpduM_TxCntrNewInst
(
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) QPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  PduIdType ContainerIdx,
  uint8 NumInstances
)
{
  uint8 nextWIdx;

  TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);

  if(NumInstances > 1U)
  {
    /* Queuing enabled, store PDU in the next instance */
    P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) nextInst;
    nextWIdx = (IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr) + 1U) % NumInstances;
    TS_AtomicClearBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);
    nextInst = IPDUM_GET_TX_INST(ContainerIdx, nextWIdx);
    TS_AtomicSetBit_8(&nextInst->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);
    QPtr->WriteIdx = nextWIdx;

    *Event = IPDUM_TXCNTR_EV_TRANSMIT;
  }
  else
  {
    uint8 state = InstPtr->State;
    /* No queuing, single instance */
    nextWIdx = 0U;

    switch(state)
    {
      case IPDUM_TXCNTR_ST_IDLE:
      {
        *Event = IPDUM_TXCNTR_EV_TRANSMIT;
        break;
      }
      case IPDUM_TXCNTR_ST_WAIT_CONF:
      {
        /* TriggerTransmit or TxConfirmation pending, deny transmission */
        *Event = IPDUM_TXCNTR_EV_NONE;
        break;
      }
      case IPDUM_TXCNTR_ST_WAIT_TX:     /* intended fallthrough */
      case IPDUM_TXCNTR_ST_TRIGGERED:
      {
        /* Instance has overflown without being transmitted, drop it and store PDU */
        InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
        *Event = IPDUM_TXCNTR_EV_TRANSMIT;
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
        IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_QUEUEOVFL);
#endif
        break;
      }
      /* CHECK: NOPARSE */
      default:
        IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  return nextWIdx;
}

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_CntrTransmitLB
(
  PduIdType ContainerIdx,
  PduIdType IdxContained
)
{
  P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) containedPtr = IPDUM_CFG_GET_CNTD_TX(IdxContained);
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile instPtr = IPDUM_GET_TX_INST(ContainerIdx, qPtr->WriteIdx);

  boolean qFull = FALSE;
  uint8 event = IPDUM_TXCNTR_EV_TRANSMIT;
  Std_ReturnType RetVal = E_NOT_OK;

  if(0U == (instPtr->InstLock & IPDUM_TXCNTR_CNTD_STR_MSK))
  {
    P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) txCntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);
    P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRamData = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);

    TS_AtomicSetBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);

    while(IPDUM_TXCNTR_EV_NONE != event)
    {
      uint8 state = instPtr->State;

      switch(state)
      {
        case IPDUM_TXCNTR_ST_IDLE:
        {
          IpduM_TX_ST_IDLE(instPtr, cntrRamData, ContainerIdx, containedPtr->IpduMContainedTxPduSendTimeout);

          break;
        }
        case IPDUM_TXCNTR_ST_WAIT_TX:
        {
          PduLengthType buffOffset;
          boolean sizeExceeded = FALSE;
          boolean added = FALSE;

          IPDUM_AA_PDULEN(buffOffset, instPtr->PduLength);

          /* Does it fit ? */
          if((containedPtr->PduLength + (PduLengthType)txCntrPtr->HeaderSize) <= (txCntrPtr->PduLength - buffOffset))
          {
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Transmit,1 */
            IpduM_AddContainedLBToList(&added, instPtr, ContainerIdx, IdxContained);

            if(added && (containedPtr->IpduMContainedTxPduSendTimeout < cntrRamData->SendTimeout))
            {
              TS_AtomicAssign16(cntrRamData->SendTimeout, containedPtr->IpduMContainedTxPduSendTimeout);
            }
          }
          else
          {
            sizeExceeded = TRUE;
          }

          /* Check trigger conditions */
          if(added)
          {
            PduLengthType currentLength = (buffOffset + (PduLengthType)txCntrPtr->HeaderSize) + containedPtr->PduLength;

            IpduM_EvalTxCntrTrgCond(txCntrPtr, containedPtr, instPtr, &event, &RetVal, currentLength, ContainerIdx);
          }
          else if(sizeExceeded)
          {
            /* !LINKSTO IpduM.dsn.Transition.Container.Process.QueueSizeNotReached,1 */
            instPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
            event = (uint8)(IPDUM_TXCNTR_EV_DEQUEUE | IPDUM_TXCNTR_EV_NEW_INST);
          }
          else
          {
            event = IPDUM_TXCNTR_EV_NONE;
            RetVal = E_OK;
          }

          break;
        }

        case IPDUM_TXCNTR_ST_TRIGGERED:
        {
          IpduM_TX_ST_TRIGGERED_LB(txCntrPtr, &event, &qFull, &RetVal, instPtr, containedPtr->PduLength, ContainerIdx, IdxContained);

          break;
        }
        case IPDUM_TXCNTR_ST_WAIT_CONF:
        {
          PduLengthType instLen;
          /* Can still append if TT and the instance is the head */

          /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.Transmit,1 */
          IPDUM_AA_PDULEN(instLen, instPtr->PduLength);

          if((0U == (instPtr->InstLock & IPDUM_TXCNTR_SEALED_MSK))
            &&
              ((containedPtr->PduLength + (PduLengthType)txCntrPtr->HeaderSize) <= (txCntrPtr->PduLength - instLen)))
          {
            boolean added;

            SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

            IpduM_AddContainedLBToList(&added, instPtr, ContainerIdx, IdxContained);

            SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

            event = IPDUM_TXCNTR_EV_NONE;
            RetVal = E_OK;
          }
          else
          {
            event = IPDUM_TXCNTR_EV_NEW_INST;

            qFull = TRUE;
          }

          TS_AtomicClearBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);

          break;
        }
        /* CHECK: NOPARSE */
        default:
          IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_INTERNAL_API_ID);
          break;
        /* CHECK: PARSE */
      } /* end switch */

      if(IPDUM_TXCNTR_EV_NEW_INST == event)
      {
        instPtr = IPDUM_GET_TX_INST(ContainerIdx, IpduM_TxCntrNewInst(instPtr, qPtr, &event, ContainerIdx, txCntrPtr->NumInstances));
      }
    }

    TS_AtomicClearBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);
  }

  return RetVal;
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_IDLE
(
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRamData,
  PduIdType ContainerIdx,
  uint16 SendTimeout
)
{
  P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) txBFPtr = IPDUM_GET_TX_CNTR_CH_BF(IPDUM_GET_TX_MF_IDX(ContainerIdx));

  InstPtr->NumCntd = 0U;
  InstPtr->PduLength = 0U;
  InstPtr->InstLock = IPDUM_TXCNTR_CNTD_STR_MSK;
  TS_AtomicAssign16(CntrRamData->SendTimeout, SendTimeout);
  /* !LINKSTO IpduM.dsn.Transition.Instance.Idle.WaitTransmit,1 */
  InstPtr->State = IPDUM_TXCNTR_ST_WAIT_TX;

  /* Mark the container for the MF */
  TS_AtomicSetBit_32(&txBFPtr[(PduIdType)(ContainerIdx / 32U)], (PduIdType)(ContainerIdx % 32U));
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_TRIGGERED_LB
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) QFull,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduLengthType ContainedLength,
  PduIdType ContainerIdx,
  PduIdType IdxContained
)
{
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  /* Transmit handles only the head instance, not the tail from ReadIdx */
  uint8 instIdx = IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr);

  if(IPDUM_TXCNTR_EV_DEQUEUE == (*Event & IPDUM_TXCNTR_EV_DEQUEUE))
  {
    P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRamData = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);
    uint16 timeoutVal;
    TS_AtomicAssign16(cntrRamData->SendTimeout, 0U);
    TS_AtomicAssign16(timeoutVal, cntrRamData->TxConfTimeout);

    /* !LINKSTO IpduM.dsn.Transition.Instance.Triggered.TxConfTimer,1 */
    if(((0U == timeoutVal) && (FALSE == *QFull)) && (instIdx == qPtr->ReadIdx))
    {
      PduInfoType pduInfo;
      Std_ReturnType canTransmit;
      /* Value used during Transmit - TriggerTransmit */
      timeoutVal = TxCntrPtr->IpduMContainerTxConfirmationTimeout;

      pduInfo.SduLength = TxCntrPtr->PduLength;
      pduInfo.SduDataPtr = NULL_PTR;

      /* !LINKSTO IPDUM.ASR42.00193.LB,1 */
      if(IPDUM_CNTRTX_IS_TXMODE_DIRECT(TxCntrPtr->CfgFlags))
      {
        if(instIdx == qPtr->WriteIdx)
        {
          IPDUM_TX_ADVANCE_WRITE(qPtr, TxCntrPtr);
        }

        pduInfo.SduDataPtr = IPDUM_GET_CNTR_TXDATA(ContainerIdx, 0U);
        /* !LINKSTO IPDUM.ASR42.00220,1 */
        canTransmit = IpduM_PackLBtoBuff(&pduInfo, ContainerIdx, InstPtr->NumCntd, instIdx);

        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_ASSEMBLED_BTIDX);

        IPDUM_AA_PDULEN(InstPtr->PduLength, pduInfo.SduLength);
      }
      else
      {
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_EXP_TT_BITIDX);
        canTransmit = E_OK;
      }

      if(E_OK == canTransmit)
      {
        /* !LINKSTO IpduM.dsn.Transition.Instance.Triggered.WaitConfirmation,1 */
        InstPtr->State = IPDUM_TXCNTR_ST_WAIT_CONF;
        TS_AtomicAssign16(cntrRamData->TxConfTimeout, timeoutVal);

        if(E_OK == PduR_IpduMTransmit(TxCntrPtr->IpduMContainerTxHandleId, &pduInfo))
        {
          if(0U == timeoutVal)
          {
            /* DIRECT, TxConf timeout not configured, return to IDLE */
            /* TRIGGERTRANSMIT always has a non-zero generated value */
            InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
            if(0U != ((IPDUM_TXCNTR_EV_NEW_INST | IPDUM_TXCNTR_EV_NEW_INST_EMPTY) & *Event))
            {
              IPDUM_TX_ADVANCE_READ(qPtr, TxCntrPtr);
            }
          }
          else
          {
            /* Nothing to do */
          }
        }
        else
        {
          /* Head instance, already assembled, try again in the MF */
          InstPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
          TS_AtomicAssign16(cntrRamData->TxConfTimeout, 0U);
        }
      }
      else
      {
        /* None could be retrieved, return to IDLE */
        InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
      }

    }

    *Event &= (uint8)~(IPDUM_TXCNTR_EV_DEQUEUE | IPDUM_TXCNTR_EV_NEW_INST_EMPTY);

    if(IPDUM_TXCNTR_EV_NONE == *Event)
    {
      TS_AtomicClearBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);
      *RetVal = E_OK;
    }
  }
  else
  {
    IPDUM_PRECONDITION_ASSERT(IPDUM_TXCNTR_EV_TRANSMIT == *Event, IPDUM_SID_TRANSMIT);

    /* !LINKSTO IpduM.dsn.Transition.Instance.Triggered.Transmit,1 */
    if(0U == (InstPtr->InstLock & IPDUM_TXCNTR_SEALED_MSK))
    {
      PduLengthType instLen;
      IPDUM_AA_PDULEN(instLen, InstPtr->PduLength);

      if((ContainedLength + (PduLengthType)TxCntrPtr->HeaderSize) <= (TxCntrPtr->PduLength - instLen))
      {
        boolean added;

        IpduM_AddContainedLBToList(&added, InstPtr, ContainerIdx, IdxContained);

        *Event = IPDUM_TXCNTR_EV_NONE;
        *RetVal = E_OK;
      }
      else
      {
        /* Current PDU doesn't fit, store it a new instance */
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
        *Event = IPDUM_TXCNTR_EV_NEW_INST;
      }
    }
    else
    {
      /* !LINKSTO IpduM.dsn.Transition.Container.Process.QueueSizeReached,1 */
      /* Overwrite */
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
      IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_QUEUEOVFL);
#endif
      if(instIdx == qPtr->ReadIdx)
      {
        IPDUM_TX_ADVANCE_READ(qPtr, TxCntrPtr);
      }

      /* Either no transmission is pending or for the previous instance,
       * this is the oldest stored one, drop it */
      InstPtr->State = IPDUM_TXCNTR_ST_IDLE;

      *QFull = TRUE;
    }
  }
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_CntrNextTriggered
(
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRAMPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) InstPtr,
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) QPtr,
  PduIdType ContainerIdx,
  uint8 InstIdx
)
{
  PduInfoType pduInfo;
  Std_ReturnType canTransmit = E_NOT_OK;
  boolean isDirect = IPDUM_CNTRTX_IS_TXMODE_DIRECT(TxCntrPtr->CfgFlags);

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)
  if(!IPDUM_CNTRTX_IS_LAST_IS_BEST(TxCntrPtr->CfgFlags))
  {
    if(isDirect)
    {
      pduInfo.SduLength = InstPtr->PduLength;
    }
    else
    {
      pduInfo.SduLength = TxCntrPtr->PduLength;
      TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_EXP_TT_BITIDX);
    }

    pduInfo.SduDataPtr = IPDUM_GET_CNTR_TXDATA(ContainerIdx, InstIdx);

    canTransmit = E_OK;
  }
  else
#endif
  {
    if(isDirect)
    {
      TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
      pduInfo.SduDataPtr = IPDUM_GET_CNTR_TXDATA(ContainerIdx, 0U);

      if(0U == (InstPtr->InstLock & IPDUM_TXCNTR_ASSEMBLED_MSK))
      {
        /* !LINKSTO IPDUM.ASR42.00220,1 */
        canTransmit = IpduM_PackLBtoBuff(&pduInfo, ContainerIdx, InstPtr->NumCntd, InstIdx);
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_ASSEMBLED_BTIDX);
        IPDUM_AA_PDULEN(InstPtr->PduLength, pduInfo.SduLength);
      }
      else
      {
        IPDUM_AA_PDULEN(pduInfo.SduLength, InstPtr->PduLength);
        canTransmit = E_OK;
      }
    }
    else
    {
      TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_EXP_TT_BITIDX);
      pduInfo.SduLength = TxCntrPtr->PduLength;
      pduInfo.SduDataPtr = NULL_PTR;
      canTransmit = E_OK;
    }
  }

  /* Check again for the state just in case TxConfirmation had interrupted */
  if((E_OK == canTransmit) && (InstPtr->State == IPDUM_TXCNTR_ST_TRIGGERED))
  {
    if(isDirect)
    {
      TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);

      if(InstIdx == QPtr->WriteIdx)
      {
        IPDUM_TX_ADVANCE_WRITE(QPtr, TxCntrPtr);
      }
    }

    InstPtr->State = IPDUM_TXCNTR_ST_WAIT_CONF;
    TS_AtomicAssign16(CntrRAMPtr->TxConfTimeout, TxCntrPtr->IpduMContainerTxConfirmationTimeout);

    if(E_OK == PduR_IpduMTransmit(TxCntrPtr->IpduMContainerTxHandleId, &pduInfo))
    {
      if(0U == TxCntrPtr->IpduMContainerTxConfirmationTimeout)
      {
        /* DIRECT TriggerMode, no TxConf timeout configured, return to IDLE */
        InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
        IPDUM_TX_ADVANCE_READ(QPtr, TxCntrPtr);
      }
      else
      {
         /* Nothing to do */
      }
    }
    else
    {
      /* Transmission failed, remain TRIGGERED */
      TS_AtomicClearBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
      TS_AtomicAssign16(CntrRAMPtr->TxConfTimeout, 0U);
      InstPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
    }
  }
  else
  {
    /* None could be retrieved, return to IDLE */
    InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
    IPDUM_TX_ADVANCE_READ(QPtr, TxCntrPtr);
  }
}

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_TX_ST_TRIGGERED_CQ
(
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) TxCntrPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Event,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) QFull,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) RetVal,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
)
{
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  uint8 instIdx = IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr);

  if(IPDUM_TXCNTR_EV_DEQUEUE == (IPDUM_TXCNTR_EV_DEQUEUE & *Event))
  {
    P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRamData = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);
    uint16 timeoutVal;
    boolean isDirect = IPDUM_CNTRTX_IS_TXMODE_DIRECT(TxCntrPtr->CfgFlags);
    TS_AtomicAssign16(cntrRamData->SendTimeout, 0U);
    TS_AtomicAssign16(timeoutVal, cntrRamData->TxConfTimeout);

    if(((0U == timeoutVal) && (FALSE == *QFull)) && (instIdx == qPtr->ReadIdx))
    {
      PduInfoType pduInfo;
      /* Value used during Transmit - TriggerTransmit */
      timeoutVal = TxCntrPtr->IpduMContainerTxConfirmationTimeout;

      if(isDirect)
      {
        pduInfo.SduLength = InstPtr->PduLength;
      }
      else
      {
        pduInfo.SduLength = TxCntrPtr->PduLength;
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_EXP_TT_BITIDX);
      }

      pduInfo.SduDataPtr = IPDUM_GET_CNTR_TXDATA(ContainerIdx, IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr));

      /* Setting it prematurely to handle if Transmit is interrupted by TriggerTransmit or TxConfirmation */
      /* !LINKSTO IpduM.dsn.Transition.Instance.Triggered.WaitConfirmation,1 */
      InstPtr->State = IPDUM_TXCNTR_ST_WAIT_CONF;
      TS_AtomicAssign16(cntrRamData->TxConfTimeout, timeoutVal);

      if(E_OK == PduR_IpduMTransmit(TxCntrPtr->IpduMContainerTxHandleId, &pduInfo))
      {
        if(0U == timeoutVal)
        {
          /* !LINKSTO IpduM.dsn.Transition.Container.Process.Idle,1 */
          InstPtr->State = IPDUM_TXCNTR_ST_IDLE;
          if(0U != ((IPDUM_TXCNTR_EV_NEW_INST | IPDUM_TXCNTR_EV_NEW_INST_EMPTY) & *Event))
          {
            IPDUM_TX_ADVANCE_READ(qPtr, TxCntrPtr);
          }
        }
        else
        {
          if(isDirect)
          {
            TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
          }
        }
      }
      else
      {
        InstPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
        TS_AtomicAssign16(cntrRamData->TxConfTimeout, 0U);
      }
    }

    *Event &= (uint8)~(IPDUM_TXCNTR_EV_DEQUEUE | IPDUM_TXCNTR_EV_NEW_INST_EMPTY);

    if(IPDUM_TXCNTR_EV_NONE == *Event)
    {
      *RetVal = E_OK;
    }
  }
  else
  {
    IPDUM_PRECONDITION_ASSERT(IPDUM_TXCNTR_EV_TRANSMIT == *Event, IPDUM_SID_TRANSMIT);

    /* Check if tail of the queue or other element */
    if(0U == (InstPtr->InstLock & IPDUM_TXCNTR_SEALED_MSK))
    {
      PduLengthType buffOffset;
      IPDUM_AA_PDULEN(buffOffset, InstPtr->PduLength);

      if((PduInfoPtr->SduLength + (PduLengthType)TxCntrPtr->HeaderSize) <= (TxCntrPtr->PduLength - buffOffset))
      {
        IpduM_AddCntdCQ(PduInfoPtr,
                        InstPtr,
                        IPDUM_CFG_GET_CNTD_TX(IdxContained)->IpduMContainedPduHeaderId,
                        buffOffset,
                        ContainerIdx,
                        IdxContained,
                        TxCntrPtr->HeaderSize);

        *Event = IPDUM_TXCNTR_EV_NONE;
        *RetVal = E_OK;
      }
      else
      {
        /* Current PDU doesn't fit, create new instance and store it */
        TS_AtomicSetBit_8(&InstPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);
        *Event = IPDUM_TXCNTR_EV_NEW_INST;
      }
    }
    else
    {
      /* !LINKSTO IpduM.dsn.Transition.Container.Process.QueueSizeReached,1 */
      /* Overwrite */
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
      IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRANSMIT, IPDUM_E_QUEUEOVFL);
#endif
      if(instIdx == qPtr->ReadIdx)
      {
        IPDUM_TX_ADVANCE_READ(qPtr, TxCntrPtr);
      }

      /* Either no TxConfirmation is pending or for the previous instance,
       * this is the oldest stored one, drop it */
      InstPtr->State = IPDUM_TXCNTR_ST_IDLE;

      *QFull = TRUE;
    }
  }
}

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_CntrTransmitCQ
(
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
)
{
  P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) containedPtr = IPDUM_CFG_GET_CNTD_TX(IdxContained);
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile instPtr = IPDUM_GET_TX_INST(ContainerIdx, qPtr->WriteIdx);

  boolean qFull = FALSE;
  uint8 event = IPDUM_TXCNTR_EV_TRANSMIT;
  Std_ReturnType RetVal = E_NOT_OK;

  if(0U == (instPtr->InstLock & IPDUM_TXCNTR_CNTD_STR_MSK))
  {
    P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_VAR_CLEARED) txCntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);
    P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRamData = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);

    TS_AtomicSetBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);

    while(IPDUM_TXCNTR_EV_NONE != event)
    {
      uint8 state = instPtr->State;

      switch(state)
      {
        case IPDUM_TXCNTR_ST_IDLE:
        {
          IpduM_TX_ST_IDLE(instPtr, cntrRamData, ContainerIdx, containedPtr->IpduMContainedTxPduSendTimeout);

          break;
        }
        case IPDUM_TXCNTR_ST_WAIT_TX:
        {
          PduLengthType buffOffset;
          boolean sizeExceeded = FALSE;

          IPDUM_AA_PDULEN(buffOffset, instPtr->PduLength);

          if((PduInfoPtr->SduLength + (PduLengthType)txCntrPtr->HeaderSize) <= (txCntrPtr->PduLength - buffOffset))
          {
            uint16 sendTimeout;
            TS_AtomicAssign16(sendTimeout, cntrRamData->SendTimeout);

            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Transmit,1 */
            IpduM_AddCntdCQ(PduInfoPtr,
                            instPtr,
                            containedPtr->IpduMContainedPduHeaderId,
                            buffOffset,
                            ContainerIdx,
                            IdxContained,
                            txCntrPtr->HeaderSize);

            if(containedPtr->IpduMContainedTxPduSendTimeout < sendTimeout)
            {
              TS_AtomicAssign16(cntrRamData->SendTimeout, containedPtr->IpduMContainedTxPduSendTimeout);
            }
          }
          else
          {
            sizeExceeded = TRUE;
          }

          /* Check trigger conditions */
          {
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.TRG_ALWAYS,1 */
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.TRG_FIRST,1 */
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.THRESHOLD,1 */
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.ALL_STATIC,1 */
            if(sizeExceeded)
            {
              /* !LINKSTO IpduM.dsn.Transition.Container.Process.QueueSizeNotReached,1 */
              instPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
              event = (uint8)(IPDUM_TXCNTR_EV_DEQUEUE | IPDUM_TXCNTR_EV_NEW_INST);
            }
            else
            {
              PduLengthType currentLength = (buffOffset + (PduLengthType)txCntrPtr->HeaderSize) + PduInfoPtr->SduLength;

              IpduM_EvalTxCntrTrgCond(txCntrPtr,containedPtr, instPtr, &event, &RetVal, currentLength, ContainerIdx);
            }
          }

          break;
        }
        case IPDUM_TXCNTR_ST_TRIGGERED:
        {
          IpduM_TX_ST_TRIGGERED_CQ(PduInfoPtr, txCntrPtr, &event, &qFull, &RetVal, instPtr, ContainerIdx, IdxContained);
          break;
        }
        case IPDUM_TXCNTR_ST_WAIT_CONF:
        {
          /* Can still append if TT and the instance is the head */
          PduLengthType instLen;
          IPDUM_AA_PDULEN(instLen, instPtr->PduLength);

          if((0U == (instPtr->InstLock & IPDUM_TXCNTR_SEALED_MSK))
            &&((PduInfoPtr->SduLength + (PduLengthType)txCntrPtr->HeaderSize) <= (txCntrPtr->PduLength - instLen)))
          {
            IpduM_AddCntdCQ(PduInfoPtr,
                            instPtr,
                            containedPtr->IpduMContainedPduHeaderId,
                            instLen,
                            ContainerIdx,
                            IdxContained,
                            txCntrPtr->HeaderSize);

            event = IPDUM_TXCNTR_EV_NONE;
            RetVal = E_OK;
          }
          else
          {
            /* !LINKSTO IpduM.EB.CntrTx.DequeueOrder.OverFlow,1 */
            /* Current PDU doesn't fit, store it in a new instance */
            event = IPDUM_TXCNTR_EV_NEW_INST;

            qFull = TRUE;
          }

          break;
        }
        /* CHECK: NOPARSE */
        default:
          IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_SID_TRANSMIT);
          break;
        /* CHECK: PARSE */
      } /* end switch */

      if(IPDUM_TXCNTR_EV_NEW_INST == event)
      {
        instPtr = IPDUM_GET_TX_INST(ContainerIdx, IpduM_TxCntrNewInst(instPtr, qPtr, &event, ContainerIdx, txCntrPtr->NumInstances));
      }
    }

    TS_AtomicClearBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);
  }

  return RetVal;
}
#endif /* IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON */

#if (IPDUM_ZERO_COPY == STD_OFF)
IPDUM_STATIC FUNC(Std_ReturnType , IPDUM_CODE) IpduM_CntrTT
(
  PduIdType ContainerIdx,
  P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
)
{
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) txCntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) instPtr;

  uint8 state;
  Std_ReturnType RetVal = E_NOT_OK;

  if(!IPDUM_CNTRTX_IS_TXMODE_DIRECT(txCntrPtr->CfgFlags))
  {
    uint8 instIdx = qPtr->ReadIdx;
    instPtr = IPDUM_GET_TX_INST(ContainerIdx, instIdx);
    state = instPtr->State;

    switch(state)
    {
      case IPDUM_TXCNTR_ST_WAIT_CONF:
      {
        P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRamData = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);

        if(PduInfoPtr->SduLength >= txCntrPtr->PduLength)
        {
          TS_AtomicSetBit_8(&instPtr->InstLock, IPDUM_TXCNTR_SEALED_BTIDX);

          if(instIdx == qPtr->WriteIdx)
          {
            IPDUM_TX_ADVANCE_WRITE(qPtr, txCntrPtr);
          }

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)
          if(!IPDUM_CNTRTX_IS_LAST_IS_BEST(txCntrPtr->CfgFlags))
          {
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.TriggerTransmit.CQ,1 */
            PduInfoPtr->SduLength = instPtr->PduLength;
            /* !LINKSTO IPDUM.ASR42.00193.CQ,1 */
            TS_MemCpy(PduInfoPtr->SduDataPtr, IPDUM_GET_CNTR_TXDATA(ContainerIdx, instIdx), PduInfoPtr->SduLength);

            RetVal = E_OK;
          }
          else
#endif
          {
            /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.TriggerTransmit.LB,1 */
            /* !LINKSTO IPDUM.ASR42.00220,1 */
            /* !LINKSTO IPDUM.ASR42.00193.LB,1 */
            RetVal = IpduM_PackLBtoBuff(PduInfoPtr, ContainerIdx, instPtr->NumCntd, instIdx);
          }

          if(IPDUM_CNTRTX_IS_TT_NOWAITCONF(txCntrPtr->CfgFlags))
          {
            /* No TxConf timeout configured, return to IDLE */
            IPDUM_TX_ADVANCE_READ(qPtr, txCntrPtr);
            /* !LINKSTO IpduM.dsn.Transition.Container.Process.Idle,1 */
            instPtr->State = IPDUM_TXCNTR_ST_IDLE;
            TS_AtomicAssign16(cntrRamData->TxConfTimeout, 0U);
          }
          else
          {
            /* Reload timer and wait for confirmation */
            TS_AtomicAssign16(cntrRamData->TxConfTimeout, txCntrPtr->IpduMContainerTxConfirmationTimeout);
            TS_AtomicClearBit_8(&instPtr->InstLock, IPDUM_TXCNTR_EXP_TT_BITIDX);
          }
        }
        else
        {
          /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.TriggerTransmit.MinSduLength,1 */
          instPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
          TS_AtomicAssign16(cntrRamData->TxConfTimeout, 0U);
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
          IPDUM_DET_REPORT_ERROR(IPDUM_SID_TRIGGER_TRANSMIT, IPDUM_E_PARAM);
#endif
        }

        break;
      }
      case IPDUM_TXCNTR_ST_IDLE:        /* intended fallthrough */
      case IPDUM_TXCNTR_ST_WAIT_TX:     /* intended fallthrough */
      case IPDUM_TXCNTR_ST_TRIGGERED:
      {
        RetVal = E_NOT_OK;
        break;
      }
      /* CHECK: NOPARSE */
      default:
        IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  return RetVal;
}
#endif /* IPDUM_ZERO_COPY == STD_OFF */

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_CntrTxConf
(
  PduIdType ContainerIdx
)
{
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) cntrRAMPtr = IPDUM_GET_TX_CNTR_RAMDATA(ContainerIdx);
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) instPtr;
  PduIdType idx;
  uint8 instIdx = qPtr->ReadIdx;
  uint8 state;

  instPtr = IPDUM_GET_TX_INST(ContainerIdx, instIdx);
  state = instPtr->State;

  switch(state)
  {
    case IPDUM_TXCNTR_ST_WAIT_CONF:
    {
      P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) txCntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);

      if((0U < txCntrPtr->IpduMContainerTxConfirmationTimeout)
      && (0U == (instPtr->InstLock & IPDUM_TXCNTR_EXP_TT_MSK)))
      {
        P2VAR(PduIdType, AUTOMATIC, IPDUM_VAR_CLEARED) txConfList = IPDUM_GET_TX_CONFLIST(ContainerIdx, instIdx);
        /* Safe to iterate over, not altered at this point */
        for(idx = 0U; idx < instPtr->NumCntd; idx++)
        {
          PduIdType ContainedId = txConfList[idx];
          if(IPDUM_RESERVED_PDUID != ContainedId)
          {
            P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) containedPtr = IPDUM_CFG_GET_CNTD_TX(ContainedId);
            if(IPDUM_CNTND_IS_TXCONF(containedPtr->CfgFlags))
            {
              PduR_IpduMTxConfirmation(containedPtr->IpduMContainedTxPduHandleId);
            }
          }
        }

        /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.Idle.TxConfirmation,1 */
        /* !LINKSTO IpduM.dsn.Transition.Container.Process.Idle,1  */
        instPtr->InstLock = 0U;
        instPtr->State = IPDUM_TXCNTR_ST_IDLE;
        TS_AtomicAssign16(cntrRAMPtr->TxConfTimeout, 0U);

        IPDUM_TX_ADVANCE_READ(qPtr, txCntrPtr);

#if(IPDUM_DEQUEUE_IN_TX_CONF == STD_OFF)
        if(!IPDUM_CNTRTX_IS_TXMODE_DIRECT(txCntrPtr->CfgFlags))
#endif
        {
          IpduM_SendNextCntr(cntrRAMPtr, ContainerIdx, IPDUM_TXCNTR_EV_DEQUEUE);
        }
      }

      break;
    }
    case IPDUM_TXCNTR_ST_IDLE:          /* intended fallthrough */
    case IPDUM_TXCNTR_ST_WAIT_TX:       /* intended fallthrough */
    case IPDUM_TXCNTR_ST_TRIGGERED:     /* intended fallthrough */
    {
      /* Ignore unexpected TxConfirmation */
      break;
    }
    /* CHECK: NOPARSE */
    default:
      IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_SID_TX_CONFIRMATION);
      break;
    /* CHECK: PARSE */
  }



}

#if (IPDUM_CNTND_COLLECT_QUEUED_TX == STD_ON)
IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddCntdCQ
(
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  uint32 HeaderId,
  PduLengthType BuffOffset,
  PduIdType ContainerIdx,
  PduIdType IdxContained,
  uint8 HeaderSize
)
{
  IpduM_NumCntdType nContained = InstPtr->NumCntd;
  uint8 instIdx = IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr);
  P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED) dataBuff = (P2VAR(uint8, AUTOMATIC, IPDUM_VAR_CLEARED))(&(IPDUM_GET_CNTR_TXDATA(ContainerIdx, instIdx))[BuffOffset]);
  P2VAR(PduIdType, AUTOMATIC, IPDUM_VAR_CLEARED) txConfList = IPDUM_GET_TX_CONFLIST(ContainerIdx, instIdx);

  InstPtr->NumCntd++;
  txConfList[nContained] = IdxContained;
  InstPtr->PduLength = BuffOffset + (HeaderSize + PduInfoPtr->SduLength);

  if(HeaderSize == IPDUM_HEADERTYPE_SHORT)
  {
    /* Deviation MISRAC2012-4 */
    TS_MemCpy(dataBuff, ((uint8*)&HeaderId + IPDUM_HDR_ID_OFFST), IPDUM_SHORT_HDR_DLC_OFFST);
    dataBuff[IPDUM_SHORT_HDR_DLC_OFFST] = (uint8)PduInfoPtr->SduLength;
  }
  else
  {
    const uint32 len = IPDUM_SWAPU32_IF_MUST(PduInfoPtr->SduLength);

    TS_MemCpy(dataBuff, &HeaderId, 4U);
    TS_MemCpy(&dataBuff[4U], &len, 4U);
  }

  TS_MemCpy(&dataBuff[HeaderSize], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
}
#endif /* IPDUM_CNTND_COLLECT_QUEUED_TX */

IPDUM_STATIC FUNC(Std_ReturnType, IPDUM_CODE) IpduM_PackLBtoBuff
(
  P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr,
  PduIdType ContainerTxIdx,
  IpduM_NumCntdType NumCntd,
  uint8 InstIdx
)
{
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) cntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerTxIdx);
  P2CONST(IpduM_ContainedTxPduType, AUTOMATIC, IPDUM_APPL_CONST) containedPtr;
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) origDataPTr = pduInfoPtr->SduDataPtr;
  P2VAR(PduIdType, AUTOMATIC, IPDUM_VAR_CLEARED) txConfList = IPDUM_GET_TX_CONFLIST(ContainerTxIdx, InstIdx);
  PduLengthType finalLength;
  uint16 cntdIdx;
  uint8 numSucceeded = 0U;
  Std_ReturnType retVal;

#if (IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
  if(IPDUM_HEADERTYPE_NONE ==  cntrPtr->HeaderSize)
  {
    TS_MemSet(pduInfoPtr->SduDataPtr, cntrPtr->IpduMUnusedAreasDefault, cntrPtr->PduLength);
    finalLength = 0U;

    for(cntdIdx = 0U; cntdIdx < NumCntd; cntdIdx++)
    {
      containedPtr = IPDUM_CFG_GET_CNTD_TX(txConfList[cntdIdx]);

      /* Deviation MISRAC2012-4 */
      pduInfoPtr->SduDataPtr = origDataPTr + containedPtr->IpduMContainedPduOffset;
      pduInfoPtr->SduLength = containedPtr->PduLength;

      if(E_OK == PduR_IpduMTriggerTransmit(containedPtr->IpduMContainedTxPduHandleId, pduInfoPtr))
      {
        finalLength = (((cntrPtr->NumCntdMax - 1U) == cntdIdx) ? pduInfoPtr->SduLength : containedPtr->PduLength) + containedPtr->IpduMContainedPduOffset;

        if(IPDUM_CNTRTX_HAS_UPDATE_BITS(cntrPtr->CfgFlags))
        {
          origDataPTr[containedPtr->IpduMUpdateBitBytePosition] |= containedPtr->IpduMUpdateBitMask;

          if(containedPtr->IpduMUpdateBitBytePosition >= finalLength)
          {
            finalLength = (containedPtr->IpduMUpdateBitBytePosition + 1U);
          }
        }
        numSucceeded++;
      }
      else
      {
        /* Dropped */
        txConfList[cntdIdx] = IPDUM_RESERVED_PDUID;
      }

    } /* end for */
  }
  else
#endif /* IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
  {
    Std_ReturnType ttRet;
    /* Value set for the very first iteration */
    pduInfoPtr->SduLength = 0U;
    for(cntdIdx = 0U; cntdIdx < NumCntd; cntdIdx++)
    {
      const uint8 headerSize = cntrPtr->HeaderSize;
      containedPtr = IPDUM_CFG_GET_CNTD_TX(txConfList[cntdIdx]);

      /* Deviation MISRAC2012-4 */
      pduInfoPtr->SduDataPtr += headerSize;
      pduInfoPtr->SduLength = containedPtr->PduLength;
      ttRet = PduR_IpduMTriggerTransmit(containedPtr->IpduMContainedTxPduHandleId, pduInfoPtr);

      /* Deviation MISRAC2012-4 */
      pduInfoPtr->SduDataPtr -= headerSize;

      if(E_OK == ttRet)
      {
        const uint32 headerId = containedPtr->IpduMContainedPduHeaderId;
        if(IPDUM_HEADERTYPE_SHORT == headerSize)
        {
          TS_MemCpy(pduInfoPtr->SduDataPtr, ((const uint8*)&headerId + IPDUM_HDR_ID_OFFST), IPDUM_SHORT_HDR_DLC_OFFST);
          pduInfoPtr->SduDataPtr[IPDUM_SHORT_HDR_DLC_OFFST] = (uint8)pduInfoPtr->SduLength;
        }
        else
        {
          const uint32 len = IPDUM_SWAPU32_IF_MUST(pduInfoPtr->SduLength);

          TS_MemCpy(pduInfoPtr->SduDataPtr, &headerId, 4U);
          /* Deviation MISRAC2012-4 */
          TS_MemCpy(pduInfoPtr->SduDataPtr + 4U, &len, 4U);
        }
        /* Deviation MISRAC2012-4 */
        pduInfoPtr->SduDataPtr += headerSize + pduInfoPtr->SduLength;
        numSucceeded++;
      }
      else
      {
        /* Dropped */
        txConfList[cntdIdx] = IPDUM_RESERVED_PDUID;
      }
    } /* end for */
    finalLength = pduInfoPtr->SduDataPtr - origDataPTr;
  } /* end dynamic header */

  if(0U != numSucceeded)
  {
    pduInfoPtr->SduDataPtr = origDataPTr;
    pduInfoPtr->SduLength = finalLength;
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_AddContainedLBToList
(
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) AddedOutPtr,
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) volatile InstPtr,
  PduIdType ContainerIdx,
  PduIdType IdxContained
)
{
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) cntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);
  P2VAR(PduIdType, AUTOMATIC, IPDUM_VAR_CLEARED) txConfList;
  IpduM_NumCntdType numCntd = InstPtr->NumCntd;
  IpduM_NumCntdType insPos = numCntd;
  boolean needsToBeProcessed = TRUE;
  IpduM_NumCntdType idx;
  uint8 instIdx = IPDUM_GET_IDX_TX_INST(ContainerIdx, InstPtr);

  txConfList = IPDUM_GET_TX_CONFLIST(ContainerIdx, instIdx);

  for(idx = 0U; idx < insPos; idx++)
  {
    if(IdxContained == txConfList[idx])
    {
      needsToBeProcessed = FALSE;
      break;
    }
  }

  if(needsToBeProcessed)
  {
    PduLengthType newLength;
    /* First two branches could be merged with offsetof */
#if (IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON)
    if(IPDUM_HEADERTYPE_NONE == cntrPtr->HeaderSize)
    {
      PduLengthType pduOffset = IPDUM_CFG_GET_CNTD_TX(IdxContained)->IpduMContainedPduOffset;

      for(idx = numCntd; idx > 0U; idx--)
      {
        if(IPDUM_CFG_GET_CNTD_TX(txConfList[idx - 1U])->IpduMContainedPduOffset > pduOffset)
        {
          txConfList[idx] = txConfList[idx - 1U];
        }
        else
        {
          break;
        }
      }

      insPos = idx;
    }
    else
#endif /* IPDUM_STATIC_CONTAINER_PDU_HANDLING_ENABLE == STD_ON */
#if (IPDUM_CONTAINEDTXPDU_PRIORITY_HANDLING_ENABLE == STD_ON)
    if(IPDUM_CNTRTX_HAS_PRIORITY(cntrPtr->CfgFlags))
    {
      uint8 newPrio = IPDUM_CFG_GET_CNTD_TX(IdxContained)->IpduMContainedTxPduPriority;

      for(idx = numCntd; idx > 0U; idx--)
      {
        /* !LINKSTO IpduM.ASR44.SWS_IpduM_00249,1 */
        if(IPDUM_CFG_GET_CNTD_TX(txConfList[idx - 1U])->IpduMContainedTxPduPriority > newPrio)
        {
          txConfList[idx] = txConfList[idx - 1U];
        }
        else
        {
          break;
        }
      }

      insPos = idx;
    }
    else
#endif /* IPDUM_CONTAINEDTXPDU_PRIORITY_HANDLING_ENABLE == STD_ON  */
    {
      /* Normal LB, no special handling */
    }

    InstPtr->NumCntd++;
    IPDUM_AA_PDULEN(newLength, InstPtr->PduLength);
    newLength += (cntrPtr->HeaderSize + IPDUM_CFG_GET_CNTD_TX(IdxContained)->PduLength);
    IPDUM_AA_PDULEN(InstPtr->PduLength, newLength);
    txConfList[insPos] = IdxContained;
    *AddedOutPtr = TRUE;
  }
  else
  {
    *AddedOutPtr = FALSE;
  }
}

IPDUM_STATIC FUNC(void, IPDUM_CODE) IpduM_SendNextCntr
(
  P2VAR(IpduM_TxCntrRAMDataType, AUTOMATIC, IPDUM_VAR_CLEARED) CntrRAMPtr,
  PduIdType ContainerIdx,
  uint8 Event
)
{
  P2CONST(IpduM_ContainerTxPduType, AUTOMATIC, IPDUM_APPL_CONST) txCntrPtr = IPDUM_CFG_GET_CNTR_TX(ContainerIdx);
  P2VAR(IpduM_CntrQueueType, AUTOMATIC, IPDUM_VAR_CLEARED) qPtr = IPDUM_GET_TX_QUEUE(ContainerIdx);
  volatile P2VAR(IpduM_CntrTxInstType, AUTOMATIC, IPDUM_VAR_CLEARED) instPtr;
  uint8 event = Event;
  uint8 state;

  /* Check if the tail has timed out */
  if(IPDUM_TXCNTR_EV_SENDTOUT == (IPDUM_TXCNTR_EV_SENDTOUT & event))
  {
    /* Tail timed out */
    uint8 writeIdx = qPtr->WriteIdx;
    instPtr = IPDUM_GET_TX_INST(ContainerIdx, writeIdx);

    IPDUM_PRECONDITION_ASSERT(IPDUM_TXCNTR_ST_WAIT_TX == instPtr->State, IPDUM_INTERNAL_API_ID);

    /* !LINKSTO IpduM.dsn.Transition.Instance.WaitTransmit.Triggered.SENDTOUT,1 */
    instPtr->State = IPDUM_TXCNTR_ST_TRIGGERED;
    event &= (uint8)~IPDUM_TXCNTR_EV_SENDTOUT;
  }

  while(IPDUM_TXCNTR_EV_NONE != event)
  {
    uint8 readIdx = qPtr->ReadIdx;
    instPtr = IPDUM_GET_TX_INST(ContainerIdx, readIdx);
    state = instPtr->State;

    switch(state)
    {
      case IPDUM_TXCNTR_ST_IDLE:
      {
        P2VAR(uint32, AUTOMATIC, IPDUM_VAR_CLEARED) txBFPtr = IPDUM_GET_TX_CNTR_CH_BF(IPDUM_GET_TX_MF_IDX(ContainerIdx));

        TS_AtomicClearBit_32(&txBFPtr[(PduIdType)(ContainerIdx / 32U)], (PduIdType)(ContainerIdx % 32U));

        event = IPDUM_TXCNTR_EV_NONE;

        break;
      }
      case IPDUM_TXCNTR_ST_WAIT_TX:
      {
        IPDUM_PRECONDITION_ASSERT(IPDUM_TXCNTR_EV_SENDTOUT != (IPDUM_TXCNTR_EV_SENDTOUT & event), IPDUM_INTERNAL_API_ID);

        event = IPDUM_TXCNTR_EV_NONE;

        break;
      }
      case IPDUM_TXCNTR_ST_TRIGGERED:
      {
        TS_AtomicSetBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);

        IpduM_CntrNextTriggered(txCntrPtr, CntrRAMPtr, instPtr, qPtr, ContainerIdx, readIdx);

        TS_AtomicClearBit_8(&instPtr->InstLock, IPDUM_TXCNTR_CNTD_STR_BTIDX);

        event &= (uint8)~IPDUM_TXCNTR_EV_DEQUEUE;

        break;
      }
      case IPDUM_TXCNTR_ST_WAIT_CONF:
      {
        SchM_Enter_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

        /* !LINKSTO IpduM.dsn.Transition.Instance.WaitConfirmation.Idle.TxConfTimer,1 */
        /* !LINKSTO IpduM.EB.CntrTx.DequeueOrder.NoOverFlow,1 */
        if(readIdx != qPtr->WriteIdx)
        {
          IPDUM_TX_ADVANCE_READ(qPtr, txCntrPtr);
        }

        instPtr->InstLock = 0U;
        instPtr->State = IPDUM_TXCNTR_ST_IDLE;

        SchM_Exit_IpduM_SCHM_IPDUM_EXCLUSIVE_AREA_0();

        event &= (uint8)~IPDUM_TXCNTR_EV_TXCONF;
        break;
      }
      /* CHECK: NOPARSE */
      default:
        IPDUM_UNREACHABLE_CODE_ASSERT(IPDUM_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }
}

#endif /* (IPDUM_CONTAINER_PDU_HANDLING_ENABLE == STD_ON) */

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[end of file]===========================================*/
