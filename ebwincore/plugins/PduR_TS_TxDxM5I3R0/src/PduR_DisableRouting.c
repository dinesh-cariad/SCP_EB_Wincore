/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 *  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 *  Reason:
 *  The memory routines are optimized for dealing with aligned memory sections.
 *
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <TSMem.h>                /* TS_MemCpy */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */

#if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && \
     (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON))
#include <SchM_PduR.h>            /* Schedule Manager for module PduR */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

#if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF)
#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)
#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
static FUNC(void, PDUR_CODE) PduR_ResetSbTxBuffer
(
  uint16 BufId
);
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */
static FUNC(void, PDUR_CODE) PduR_ResetSbTxBufferDynPyld
(
  uint16 BufId
);

#endif /* ( PDUR_SB_TX_BUFFER_SUPPORT == STD_ON ) */

#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
static FUNC(void, PDUR_CODE) PduR_ResetDfTxBuffer
(
  uint16 BufId
);

static FUNC(void, PDUR_CODE) PduR_ResetTfTxBuffer
(
  uint16 BufId
);
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

static FUNC(void, PDUR_CODE) PduR_ResetDfTxBufferDynPyld
(
  uint16 BufId
);

static FUNC(void, PDUR_CODE) PduR_ResetTfTxBufferDynPyld
(
  uint16 BufId
);

#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */
#endif /* (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) */

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

FUNC(void, PDUR_CODE) PduR_DisableRouting
(
  PduR_RoutingPathGroupIdType GroupId
)
{
  DBG_PDUR_DISABLEROUTING_ENTRY(GroupId);

  /* check if PDU Router is initialized */
  if(PduR_State == PDUR_UNINIT)
  {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
    /* reported to Det in development mode */
    PDUR_DET_REPORT_ERROR(PDUR_SID_DISABLE_ROUTING, PDUR_E_INVALID_REQUEST);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
  }
  /* range check of parameter GroupId,
    if this GroupId doesn't exist function returns with no action carried out */
  else if(GroupId >= PduR_GConfigPtr->PduR_RoutingPathGroupsConfig.RPathGroupsCount)
  {
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
    PDUR_DET_REPORT_ERROR(PDUR_SID_DISABLE_ROUTING, PDUR_E_ROUTING_PATH_GROUP_ID_INVALID);
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* pointer to this routing path group's first destination buffer configuration configuration */
    CONSTP2CONST(PduR_RPathGroupsConfigType, AUTOMATIC, PDUR_APPL_CONST) pRPathGroupsConfig =
        PDUR_GET_CONFIG_ADDR(PduR_RPathGroupsConfigType,
            PduR_GConfigPtr->PduR_RoutingPathGroupsConfig.PduR_RPathGroupsConfigRef);

    CONSTP2CONST(PduR_RPathGroupsDestBufConfigType, AUTOMATIC, PDUR_APPL_CONST) pRPGDestBufConfig =
        PDUR_GET_CONFIG_ADDR(PduR_RPathGroupsDestBufConfigType,
            pRPathGroupsConfig[GroupId].PduR_RPathGroupsDestBufConfigRef);

    /* pointer to single buffer information in RAM for routing path groups bitmap */
    /* Deviation MISRAC2012-1 */
    CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) RoutingPathsGroupBitMapPtr =
        PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_RoutingPathGroupsBitmapRef);

    /* get this routing path group's byte and bit position on the bitmap */
    const PduR_RoutingPathGroupIdType BytePosition = GroupId/8U;
    const uint8 BitPosition = (uint8)(GroupId%8U);

    CONSTP2CONST(PduR_DestPduTypeOfBufferType, AUTOMATIC, PDUR_APPL_CONST) pDestPduTypeOfBuffer =
        PDUR_GET_CONFIG_ADDR(PduR_DestPduTypeOfBufferType, pRPGDestBufConfig->DestPduTypeOfBufferRef);

    uint8 DestIdx = 0U;
    const uint8 DestCnt = pRPGDestBufConfig->DestinationCount;

#if( (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) || (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON) )
    CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDestPduBufferIdRef =
        PDUR_GET_CONFIG_ADDR(uint8, pRPGDestBufConfig->DestPduBufferIdRef);
#endif

    /* ENTER CRITICAL SECTION */
    SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_3();

    TS_AtomicClearBit_8(&RoutingPathsGroupBitMapPtr[BytePosition], BitPosition);

    /* EXIT CRITICAL SECTION */
    SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_3();

    /* Go through all destinations of the groupID */
    do
    {

      switch(pDestPduTypeOfBuffer[DestIdx])
      {
      case PDUR_DEST_NO_BUFFER_REFERENCED:
        break;

#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)
#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
      case PDUR_DEST_SINGLE_BUFFER:
        /* single buffer is set to default value */
        PduR_ResetSbTxBuffer(pDestPduBufferIdRef[DestIdx]);
        break;
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

      case PDUR_DEST_SINGLE_BUFFER_DYNAMIC_PAYLOAD:
        /* single buffer is set to default value */
        PduR_ResetSbTxBufferDynPyld(pDestPduBufferIdRef[DestIdx]);
        break;
#endif /* ( PDUR_SB_TX_BUFFER_SUPPORT == STD_ON ) */

#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
      case PDUR_DEST_DIRECT_FIFO:
        PduR_ResetDfTxBuffer(pDestPduBufferIdRef[DestIdx]);
        break;
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

      case PDUR_DEST_DIRECT_FIFO_DYNAMIC_PAYLOAD:
        PduR_ResetDfTxBufferDynPyld(pDestPduBufferIdRef[DestIdx]);
        break;

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
      case PDUR_DEST_TRIGGERED_FIFO:
        PduR_ResetTfTxBuffer(pDestPduBufferIdRef[DestIdx]);
        break;
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

      case PDUR_DEST_TRIGGERED_FIFO_DYNAMIC_PAYLOAD:
        PduR_ResetTfTxBufferDynPyld(pDestPduBufferIdRef[DestIdx]);
        break;
#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */

        /* CHECK: NOPARSE */
      default:
        /* this default case is intended to be used only in development error detection mode */
#if (PDUR_DEV_ERROR_DETECT == STD_ON)
        PDUR_UNREACHABLE_CODE_ASSERT(PDUR_SID_DISABLE_ROUTING);
#endif /* PDUR_DEV_ERROR_DETECT == STD_ON */
        break;
        /* CHECK: PARSE */
      }

      ++DestIdx;
    }
    while(DestIdx < DestCnt);
  }
  /* else, this routing path group GroupId does not exist, PDU Router module returns with no action */

  DBG_PDUR_DISABLEROUTING_EXIT(GroupId);
}


/*==================[internal function definitions]=========================*/

#if (PDUR_ZERO_COST_OPERATION_IF == STD_OFF)
#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
static FUNC(void, PDUR_CODE) PduR_ResetSbTxBuffer
(
  uint16 BufId
)
{
  /* pointer to configuration of single buffer */
  CONSTP2CONST(PduR_BufSbConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufSbConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufSbConfigType, PduR_GConfigPtr->PduR_BufSbConfigRef);

  /* initialize single buffer whose ID has been received as parameter */
  /* pointer to default values of single buffer */
  CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDefaultValues =
      PDUR_GET_CONFIG_ADDR(uint8, pBufSbConfig[BufId].DefaultRef);

  /* pointer to memory location of single buffer in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
      PDUR_GET_RAM_ADDR(uint8, pBufSbConfig[BufId].MemRef);

  /* Copy default values into single buffer */
  TS_MemCpy(pBufMem, pDefaultValues, pBufSbConfig[BufId].Length);
}
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

static FUNC(void, PDUR_CODE) PduR_ResetSbTxBufferDynPyld
(
  uint16 BufId
)
{
  /* pointer to configuration of single buffer */
  CONSTP2CONST(PduR_BufSbConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufSbConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufSbConfigType, PduR_GConfigPtr->PduR_BufSbDynPyldConfigRef);

  /* pointer to single buffer information in RAM for dynamic payload */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_BufSbDynPyldInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufSbInfo =
      PDUR_GET_RAM_ADDR(PduR_BufSbDynPyldInfoType, PduR_GConfigPtr->PduR_BufSbDynPyldInfoRef);

  /* initialize single buffer whose ID has been received as parameter */
  /* pointer to default values of single buffer */
  CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDefaultValues =
      PDUR_GET_CONFIG_ADDR(uint8, pBufSbConfig[BufId].DefaultRef);

  /* pointer to memory location of single buffer in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
      PDUR_GET_RAM_ADDR(uint8, pBufSbConfig[BufId].MemRef);

  /* Copy default values into single buffer */
  TS_MemCpy(pBufMem, pDefaultValues, pBufSbConfig[BufId].Length);

  /* PduR_LoTriggerTransmit delivers the default values if called without Lo_Transmit */
  pBufSbInfo[BufId].UsedSduLength = pBufSbConfig[BufId].Length;
}
#endif /* ( PDUR_SB_TX_BUFFER_SUPPORT == STD_ON ) */

#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
static FUNC(void, PDUR_CODE) PduR_ResetDfTxBuffer
(
  uint16 BufId
)
{
  /* pointer to D-FIFO buffer information in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
      PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfInfoRef);

  /* ENTER CRITICAL SECTION */
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

  pBufDfInfo[BufId].Count = 0U;
  pBufDfInfo[BufId].HeadPos = 0U;
  pBufDfInfo[BufId].ConfP = 0U;
  pBufDfInfo[BufId].InTransmission = 0U;

  /* LEAVE CRITICAL SECTION */
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
}
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

static FUNC(void, PDUR_CODE) PduR_ResetDfTxBufferDynPyld
(
  uint16 BufId
)
{
  /* pointer to D-FIFO buffer information in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
      PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfDynPyldInfoRef);

  /* initialize D-FIFO buffers */
  /* pointer to configuration of D-FIFO buffer for dynamic payload */
  CONSTP2CONST(PduR_BufDfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufDfConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufDfDynPyldConfigType,
          PduR_GConfigPtr->PduR_BufDfDynPyldConfigRef);

  /* pointer to UsedSduLength information in RAM*/
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
      PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufId].UsedSduLengthPtr);

  uint8 idxBufferDepth;

  /* ENTER CRITICAL SECTION */
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

  pBufDfInfo[BufId].Count = 0U;
  pBufDfInfo[BufId].HeadPos = 0U;
  pBufDfInfo[BufId].ConfP = 0U;
  pBufDfInfo[BufId].InTransmission = 0U;

  /* Reset Used Sdu Length */
  for(idxBufferDepth = 0U; idxBufferDepth < pBufDfConfig[BufId].Depth; ++idxBufferDepth)
  {
    pUsedSduLength[idxBufferDepth] = 0U;
  }
  /* LEAVE CRITICAL SECTION */
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
}

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
static FUNC(void, PDUR_CODE) PduR_ResetTfTxBuffer
(
  uint16 BufId
)
{
  /* pointer to configuration of T-FIFO buffer */
  CONSTP2CONST(PduR_BufTfConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTfConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTfConfigType, PduR_GConfigPtr->PduR_BufTfConfigRef);

  /* pointer to T-FIFO buffer information in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_BufTfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufTfInfo =
      PDUR_GET_RAM_ADDR(PduR_BufTfInfoType, PduR_GConfigPtr->PduR_BufTfInfoRef);

  /* pointer to memory location of T-FIFO buffer in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
      PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufId].MemRef);

  /* pointer to default values of T-FIFO buffer */
  CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDefaultValues =
      PDUR_GET_CONFIG_ADDR(uint8, pBufTfConfig[BufId].DefaultRef);

  /* ENTER CRITICAL SECTION */
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

  pBufTfInfo[BufId].Count = 0U;
  pBufTfInfo[BufId].HeadPos = 0U;
  pBufTfInfo[BufId].ConfP = 0U;

  /* Copy default values into buffer */
  TS_MemCpy(pBufMem, pDefaultValues, pBufTfConfig[BufId].Length);

  /* LEAVE CRITICAL SECTION */
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
}
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

static FUNC(void, PDUR_CODE) PduR_ResetTfTxBufferDynPyld
(
  uint16 BufId
)
{
  /* pointer to configuration of T-FIFO buffer */
  CONSTP2CONST(PduR_BufTfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTfConfig =
      PDUR_GET_CONFIG_ADDR(PduR_BufTfDynPyldConfigType,
          PduR_GConfigPtr->PduR_BufTfDynPyldConfigRef);

  /* pointer to T-FIFO buffer information in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(PduR_BufTfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufTfInfo =
      PDUR_GET_RAM_ADDR(PduR_BufTfInfoType, PduR_GConfigPtr->PduR_BufTfDynPyldInfoRef);

  uint8 idxBufferDepth;

  /* pointer to memory location of T-FIFO buffer in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
      PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufId].MemRef);

  /* pointer to default values of T-FIFO buffer */
  CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDefaultValues =
      PDUR_GET_CONFIG_ADDR(uint8, pBufTfConfig[BufId].DefaultRef);

  /* pointer to UsedSduLength information in RAM */
  /* Deviation MISRAC2012-1 */
  CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
      PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufId].UsedSduLengthPtr);

  /* ENTER CRITICAL SECTION */
  SchM_Enter_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();

  pBufTfInfo[BufId].Count = 0U;
  pBufTfInfo[BufId].HeadPos = 0U;
  pBufTfInfo[BufId].ConfP = 0U;

  /* Copy default values into buffer */
  TS_MemCpy(pBufMem, pDefaultValues, pBufTfConfig[BufId].Length);

  pUsedSduLength[0] = pBufTfConfig[BufId].Length;
  for(idxBufferDepth = 1U; idxBufferDepth < pBufTfConfig[BufId].Depth; ++idxBufferDepth)
  {
    pUsedSduLength[idxBufferDepth] = 0U;
  }
  /* LEAVE CRITICAL SECTION */
  SchM_Exit_PduR_SCHM_PDUR_EXCLUSIVE_AREA_0();
}
#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */
#endif /* (PDUR_ZERO_COST_OPERATION_IF == STD_OFF) */

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */


/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && */
/*==================[end of file]===========================================*/
