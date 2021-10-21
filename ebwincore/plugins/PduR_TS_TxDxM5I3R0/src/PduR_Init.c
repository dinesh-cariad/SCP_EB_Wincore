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
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (depend part) */
#include <TSMem.h>                /* TS_MemCpy */

#if (PDUR_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>               /* Post build configuration manager */
#endif /* PDUR_PBCFGM_SUPPORT_ENABLED */

#if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF)
#include <PduR_Lcfg.h>            /* Link-time configurable data */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>

/** \brief Initialize buffer used for If gateway operations
 **
 ** If support for If gateway operations is enabled the status information of
 ** FIFO buffers (TT-FIFO and D-FIFO) is reset and the default
 ** values for single buffers and TT-FIFO buffers are copied into
 ** the Tx buffer memory.
 ** **/
#if ( (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) || \
      (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON) )
STATIC FUNC(void, PDUR_CODE) PduR_InitBuffer(void);
#endif /* check if buffers are used */


/** \brief Initialize sessions and queuing for TP gateway operations
 **
 ** If support for Tp gateway operations is enabled
 ** the Tp gateway sessions, including a possible upper layer destination module
 ** and one ore more lower layer Tp module(s), and the respective Tp gateway queue
 ** is initialized.
 **/
#if (PDUR_TPGATEWAY_SUPPORT == STD_ON )
STATIC FUNC(void, PDUR_CODE) PduR_InitTpGateway(void);
#endif /* (PDUR_TPGATEWAY_SUPPORT == STD_ON) && .. */


/** \brief Initialize sessions for Tp multicast transmit operations
 **
 ** If support for Tp multicast to multiple lower layer Tp modules is enabled,
 ** the multicast transmit sessions are initialized.
 **/
#if (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitMultiTpTx(void);
#endif /* (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON) */


/** \brief Initialize info for If multicast transmit operations
 **
 ** If support for If multicast to multiple lower layer If modules is enabled and
 ** PduRMulticastTxConfirmation is enabled, the multicast transmit info is initialized.
 **/
#if ((PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
     (PDUR_MULTICAST_TX_CONFIRMATION == STD_ON))
STATIC FUNC(void, PDUR_CODE) PduR_InitMultiIfTx(void);
#endif /* (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON) */


/** \brief Initialize single buffers
 **
 ** If support for single buffers is enabled, the configured Tx buffers are initialized
 ** for dynamic and static payload.
 **/
#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitSbTxBuffer(void);
#endif /* (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) */


/** \brief Initialize direct FIFO buffers
 **
 ** If support for FIFO buffers is enabled, the configured direct FIFO buffers are initialized
 ** for dynamic and static payload.
 **/
#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitDfTxBuffer(void);
#endif /* (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON) */


/** \brief Initialize triggered FIFO buffers
 **
 ** If support for FIFO buffers is enabled, the configured triggered FIFO buffers are initialized
 ** for dynamic and static payload.
 **/
#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitTfTxBuffer(void);
#endif /* (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON) */

/** \brief Initialize Nto1 table
 **
 ** If Tp gateway queue feature and N:1 routing support is enabled, the Nto1 table is initialized.
 **/
#if (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitNto1TabTp(void);
#endif /* (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON) */

/** \brief Initialize routing path group bit map
 **
 ** If routing path group feature is enabled, the routing path group bit map is initialized.
 **/
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitRoutingPathGroups(void);
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
/* AUTOSAR Memory Mapping - start section for code */
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>

FUNC(void, PDUR_CODE) PduR_Init
(
   P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_APPL_CONST) ConfigPtr
)
{
   DBG_PDUR_INIT_ENTRY(ConfigPtr);
#if (PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF)
#if (PDUR_PBCFGM_SUPPORT_ENABLED == STD_ON)
   /* If the initialization function is called with an null pointer get the configuration from the
    * post build configuration manager */

   if (ConfigPtr == NULL_PTR)
   {
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      (void) PbcfgM_GetConfig(
             PDUR_MODULE_ID,
             0U, /* dummy instance ID */
             &ModuleConfig);

      PduR_GConfigPtr = (P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_APPL_CONST)) ModuleConfig;
   }
   else
#endif /* PDUR_PBCFGM_SUPPORT_ENABLED == STD_OFF */
   {
      PduR_GConfigPtr = ConfigPtr;
   }

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check that configuration pointer is valid */
   if ( E_OK != PduR_IsValidConfig(PduR_GConfigPtr))
   {
      PDUR_DET_REPORT_ERROR(PDUR_SID_INIT, PDUR_E_CONFIG_PTR_INVALID);
   }
   else
#endif /* PDUR_DEV_ERROR_DETECT */
   {
#else /* PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF */
      TS_PARAM_UNUSED(ConfigPtr);
      PduR_GConfigPtr = &PduR_ConfigLayout.PduR_RootConfig;
#endif /* PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF */

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
      if (PduR_State == PDUR_ONLINE)
      {
         PDUR_DET_REPORT_ERROR(PDUR_SID_INIT, PDUR_E_INVALID_REQUEST);
      }
      else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
      {
         /* initialize Tx buffers for If gateway operation */
#if ((PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) || \
     (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON))
         PduR_InitBuffer();
#endif

         /* initialize info for If multicast operation */
#if ((PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
     (PDUR_MULTICAST_TX_CONFIRMATION == STD_ON))
         PduR_InitMultiIfTx();
#endif

         /* initialize TP gateway operation, including possible reception to upper layer module
          * and/or multiple lower layer TP module(s) */
#if (PDUR_TPGATEWAY_SUPPORT == STD_ON)
         PduR_InitTpGateway();
#endif

         /* initialize multicast transmit from upper layer TP module */
#if (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON)
         PduR_InitMultiTpTx();
#endif

         /* initialize Nto1 table */
#if (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON)
         PduR_InitNto1TabTp();
#endif
         /* initialize routing path group bit map */
#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
         PduR_InitRoutingPathGroups();
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

         PduR_State = PDUR_ONLINE;
      }
#if (PDUR_CONST_CFG_ADDRESS_ENABLE == STD_OFF)
   }
#endif

   DBG_PDUR_INIT_EXIT(ConfigPtr);
}

/*==================[internal function definitions]=========================*/

#if ((PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) || \
     (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON))
STATIC FUNC(void,PDUR_CODE) PduR_InitBuffer(void)
{
   DBG_PDUR_INITBUFFER_ENTRY();

#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)
   PduR_InitSbTxBuffer();
#endif /* ( PDUR_SB_TX_BUFFER_SUPPORT == STD_ON ) */

#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
   PduR_InitDfTxBuffer();
   PduR_InitTfTxBuffer();
#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */

   DBG_PDUR_INITBUFFER_EXIT();
}
#endif /* check if buffers are used */


#if (PDUR_TPGATEWAY_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitTpGateway(void)
{
   PduR_GateTpSessionIndexType SessionIndex;
   PduR_PduIdType PduIdIndex;

   uint8 MoreEntries = PDUR_LAST_MULTICAST_ENTRY_OF_PDUID;

   /* pointer to routing table */
   CONSTP2CONST(PduR_GTabTpRxType, AUTOMATIC, PDUR_APPL_CONST) pGTabTpRx =
      PDUR_GET_CONFIG_ADDR(PduR_GTabTpRxType, PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxRef);

   /* pointer to Tp gateway sessions */
   /* Deviation MISRAC2012-1 */
   CONSTP2VAR(PduR_GateTpSessionInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pSessionInfos =
      PDUR_GET_RAM_ADDR(PduR_GateTpSessionInfoType, PduR_GConfigPtr->PduR_GateTpSessionInfoRef);

   DBG_PDUR_INITTPGATEWAY_ENTRY();

   /* initialize Tp gateway sessions */
   for(SessionIndex = 0U; SessionIndex < PduR_GConfigPtr->PduR_BufTpCount; SessionIndex++)
   {
      PduR_ReleaseGateTpSession(&pSessionInfos[SessionIndex]);
   }

   /* initialize Tp gateway queue for each entry */
   for(PduIdIndex = 0U; PduIdIndex < PduR_GConfigPtr->PduR_GTabTpConfig.GTabRxCount; PduIdIndex++)
   {
      /* initialize every first entry of a gateway multicast PDU */
      if(MoreEntries == PDUR_LAST_MULTICAST_ENTRY_OF_PDUID)
      {
         PduR_ReleaseGateTpSessionQueue(PduIdIndex);
      }

      MoreEntries = pGTabTpRx[PduIdIndex].MoreEntries;
   }

   DBG_PDUR_INITTPGATEWAY_EXIT();
}
#endif /* (PDUR_TPGATEWAY_SUPPORT == STD_ON) && .. */


#if (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitMultiTpTx(void)
{
   uint8_least LoopIdx;

   DBG_PDUR_INITMULTITPTX_ENTRY();

   /* initialize mutlicast transmit sessions */
   for(LoopIdx = 0U; LoopIdx < PduR_GConfigPtr->PduR_MultiTpTxRPathsMax; LoopIdx++)
   {
      /* release multicast transmit session */
      PduR_ReleaseMultiTpTxSession((uint8)LoopIdx);
   }

   DBG_PDUR_INITMULTITPTX_EXIT();
}
#endif /* (PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON) */


#if ((PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) && \
     (PDUR_MULTICAST_TX_CONFIRMATION == STD_ON))
STATIC FUNC(void, PDUR_CODE) PduR_InitMultiIfTx(void)
{
   PduR_PduIdType ByteIdx;

   /* pointer to configuration */
   CONSTP2CONST(PduR_MTabUpIfConfigType, AUTOMATIC, PDUR_APPL_CONST) pMTabUpIfConfig =
      &PduR_GConfigPtr->PduR_MTabUpIfConfig;

   const PduR_PduIdType ByteIdxMax = pMTabUpIfConfig->MTabTxCount / 4U; /* 2*MTabTxCount/8 */

   /* pointer to multicast transmission flags */
   /* Deviation MISRAC2012-1 <+2> */
   CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pMultiIfTxInfo =
      PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_MultiIfTxInfoRef);

   DBG_PDUR_INITMULTIIFTX_ENTRY();

   /* initialize mutlicast transmit info */
   for(ByteIdx = 0U; ByteIdx < ByteIdxMax; ByteIdx++)
   {
      /* reset multicast transmit info */
      pMultiIfTxInfo[ByteIdx] = 0U;
   }

   DBG_PDUR_INITMULTIIFTX_EXIT();
}
#endif /* (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON) */


#if (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitSbTxBuffer(void)
{
   /* iterator for buffer IDs */
   uint16_least BufId;

   DBG_PDUR_INITSBTXBUFFER_ENTRY();

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufSbCount > 0U)
   {
      /* pointer to configuration of single buffer */
      CONSTP2CONST(PduR_BufSbConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufSbConfig =
         PDUR_GET_CONFIG_ADDR(PduR_BufSbConfigType, PduR_GConfigPtr->PduR_BufSbConfigRef);

      /* initialize single buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufSbCount; ++BufId)
      {
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
   }
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufSbDynPyldCount > 0U)
   {
      /* pointer to configuration of single buffer */
      CONSTP2CONST(PduR_BufSbConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufSbConfig =
         PDUR_GET_CONFIG_ADDR(PduR_BufSbConfigType, PduR_GConfigPtr->PduR_BufSbDynPyldConfigRef);

      /* pointer to single buffer information in RAM for dynamic payload */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufSbDynPyldInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufSbInfo =
         PDUR_GET_RAM_ADDR(PduR_BufSbDynPyldInfoType, PduR_GConfigPtr->PduR_BufSbDynPyldInfoRef);

      /* initialize single buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufSbDynPyldCount; ++BufId)
      {
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
   }

   DBG_PDUR_INITSBTXBUFFER_EXIT();
}
#endif /* ( PDUR_SB_TX_BUFFER_SUPPORT == STD_ON ) */


#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitDfTxBuffer(void)
{
   /* iterator for buffer IDs */
   uint16_least BufId;

   DBG_PDUR_INITDFTXBUFFER_ENTRY();

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufDfCount > 0U)
   {
      /* pointer to D-FIFO buffer information in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfInfoRef);

      /* initialize D-FIFO buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufDfCount; ++BufId)
      {
         pBufDfInfo[BufId].Count = 0U;
         pBufDfInfo[BufId].HeadPos = 0U;
         pBufDfInfo[BufId].ConfP = 0U;
         pBufDfInfo[BufId].InTransmission = 0U;
      }
   }
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufDfDynPyldCount > 0U)
   {
      /* pointer to D-FIFO buffer information in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufDfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufDfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufDfInfoType, PduR_GConfigPtr->PduR_BufDfDynPyldInfoRef);

      /* initialize D-FIFO buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufDfDynPyldCount; ++BufId)
      {
         uint8 idxBufferDepth;

         /* pointer to configuration of D-FIFO buffer for dynamic payload */
         CONSTP2CONST(PduR_BufDfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufDfConfig =
            PDUR_GET_CONFIG_ADDR(PduR_BufDfDynPyldConfigType,
                                 PduR_GConfigPtr->PduR_BufDfDynPyldConfigRef);

         /* pointer to UsedSduLength information in RAM*/
         /* Deviation MISRAC2012-1 */
         CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pUsedSduLength =
            PDUR_GET_RAM_ADDR(uint8, pBufDfConfig[BufId].UsedSduLengthPtr);

         pBufDfInfo[BufId].Count = 0U;
         pBufDfInfo[BufId].HeadPos = 0U;
         pBufDfInfo[BufId].ConfP = 0U;
         pBufDfInfo[BufId].InTransmission = 0U;

         /* Reset Used Sdu Length */
         for(idxBufferDepth=0U; idxBufferDepth < pBufDfConfig[BufId].Depth; ++idxBufferDepth)
         {
            pUsedSduLength[idxBufferDepth] = 0U;
         }
      }
   }

   DBG_PDUR_INITDFTXBUFFER_EXIT();
}
#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */


#if (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitTfTxBuffer(void)
{
   /* iterator for buffer IDs */
   uint16_least BufId;

   DBG_PDUR_INITTFTXBUFFER_ENTRY();

#if (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON)
   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufTfCount > 0U)
   {
      /* pointer to configuration of T-FIFO buffer */
      CONSTP2CONST(PduR_BufTfConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTfConfig =
          PDUR_GET_CONFIG_ADDR(PduR_BufTfConfigType, PduR_GConfigPtr->PduR_BufTfConfigRef);

      /* pointer to T-FIFO buffer information in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufTfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufTfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufTfInfoType, PduR_GConfigPtr->PduR_BufTfInfoRef);

      /* initialize T-FIFO buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufTfCount; ++BufId)
      {
         /* pointer to memory location of T-FIFO buffer in RAM */
         /* Deviation MISRAC2012-1 */
         CONSTP2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) pBufMem =
            PDUR_GET_RAM_ADDR(uint8, pBufTfConfig[BufId].MemRef);

         /* pointer to default values of T-FIFO buffer */
         CONSTP2CONST(uint8, AUTOMATIC, PDUR_APPL_CONST) pDefaultValues =
            PDUR_GET_CONFIG_ADDR(uint8, pBufTfConfig[BufId].DefaultRef);

         pBufTfInfo[BufId].Count = 0U;
         pBufTfInfo[BufId].HeadPos = 0U;
         pBufTfInfo[BufId].ConfP = 0U;

         /* Copy default values into buffer */
         TS_MemCpy(pBufMem, pDefaultValues, pBufTfConfig[BufId].Length);
      }
   }
#endif  /* (PDUR_LOIF_STATPAYLOAD_ENABLED == STD_ON) */

   /* Check if at least 1 buffer exists to avoid bounds checks warnings from the buffer ref */
   if(PduR_GConfigPtr->PduR_BufTfDynPyldCount > 0U)
   {
      /* pointer to configuration of T-FIFO buffer */
      CONSTP2CONST(PduR_BufTfDynPyldConfigType, AUTOMATIC, PDUR_APPL_CONST) pBufTfConfig =
          PDUR_GET_CONFIG_ADDR(PduR_BufTfDynPyldConfigType,
                               PduR_GConfigPtr->PduR_BufTfDynPyldConfigRef);

      /* pointer to T-FIFO buffer information in RAM */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_BufTfInfoType, AUTOMATIC, PDUR_VAR_CLEARED) pBufTfInfo =
         PDUR_GET_RAM_ADDR(PduR_BufTfInfoType, PduR_GConfigPtr->PduR_BufTfDynPyldInfoRef);

      /* initialize T-FIFO buffers */
      for(BufId = 0U; BufId < PduR_GConfigPtr->PduR_BufTfDynPyldCount; ++BufId)
      {
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

         pBufTfInfo[BufId].Count = 0U;
         pBufTfInfo[BufId].HeadPos = 0U;
         pBufTfInfo[BufId].ConfP = 0U;

         /* Copy default values into buffer */
         TS_MemCpy(pBufMem, pDefaultValues, pBufTfConfig[BufId].Length);

         /* PduR_LoTriggerTransmit delivers the default values if called without Lo_Transmit */
         pUsedSduLength[0] = pBufTfConfig[BufId].Length;
         for(idxBufferDepth = 1U; idxBufferDepth < pBufTfConfig[BufId].Depth; ++idxBufferDepth)
         {
            pUsedSduLength[idxBufferDepth] = 0U;
         }
      }
   }

   DBG_PDUR_INITTFTXBUFFER_EXIT();
}
#endif /* ( PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON ) */

#if (PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitNto1TabTp(void)
{
   uint16 tableIdx;

   /* pointer to Nto1 table */
   CONSTP2CONST(PduR_Nto1TabTpType, AUTOMATIC, PDUR_APPL_CONST) pNto1TabTp =
      PDUR_GET_CONFIG_ADDR(PduR_Nto1TabTpType, PduR_GConfigPtr->PduR_Nto1TabTpConfig.Nto1TabTpRef);

   DBG_PDUR_INITNTO1TABTP_ENTRY();

   for(tableIdx = 0U; tableIdx < PduR_GConfigPtr->PduR_Nto1TabTpConfig.Nto1TabTpCount; ++tableIdx)
   {
      /* pointer to memory location of active GMPduId */
      /* Deviation MISRAC2012-1 */
      CONSTP2VAR(PduR_PduIdType, AUTOMATIC, PDUR_VAR_CLEARED) pGMPduIdMem =
         PDUR_GET_RAM_ADDR(PduR_PduIdType, pNto1TabTp[tableIdx].GMPduIdMemRef);

      *pGMPduIdMem = PDUR_INVALID_PDUID;
   }

   DBG_PDUR_INITNTO1TABTP_EXIT();
}
#endif /* ( PDUR_TPGWQUEUE_AND_NTO1ROUTING_ENABLED == STD_ON ) */

#if (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON)
STATIC FUNC(void, PDUR_CODE) PduR_InitRoutingPathGroups(void)
{
   const uint16 NumberOfRoutingPathGroups = PduR_GConfigPtr->PduR_RoutingPathGroupsConfig.RPathGroupsCount;

   if(NumberOfRoutingPathGroups > 0U)
   {
      /* there is at least one routing path group */
      /* pointer to single buffer information in RAM for routing path groups bitmap */
      /* Deviation MISRAC2012-1 */
      P2VAR(uint8, AUTOMATIC, PDUR_VAR_CLEARED) RoutingPathsGroupBitMapPtr =
          PDUR_GET_RAM_ADDR(uint8, PduR_GConfigPtr->PduR_RoutingPathGroupsBitmapRef);

      P2CONST(PduR_RPathGroupsConfigType, AUTOMATIC, PDUR_APPL_CONST) pRPGConfig =
          PDUR_GET_CONFIG_ADDR(PduR_RPathGroupsConfigType, PduR_GConfigPtr->PduR_RoutingPathGroupsConfig.PduR_RPathGroupsConfigRef);
      uint16_least RPGroupIdx = 0U;

      /* set all bytes of the routing path groups bitmap to 0 */
      TS_MemBZero( RoutingPathsGroupBitMapPtr, ((NumberOfRoutingPathGroups + 7U) / 8U) );

      /* go through all routing path groups to check which one of them have been configured to be enabled at initialization phase */
      do
      {
         if(pRPGConfig->PduR_IsEnabledAtInit != 0U)
         { /* enabled at initialization phase */
            /* get this routing path group bit position within the calculated byte position and set it to enabled */
            *RoutingPathsGroupBitMapPtr |= (1U << (RPGroupIdx%8U));
         }
         /* else, this routing path group is not activated on initializing */

         ++pRPGConfig;
         ++RPGroupIdx;
         /* each eight times that the index is incremented, the pointer goes to the next byte on the bitmap */
         if( (RPGroupIdx % 8U) == 0U )
         {
            ++RoutingPathsGroupBitMapPtr;
         }
      }while(RPGroupIdx < NumberOfRoutingPathGroups);
   }
}
#endif /* (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) */

/* AUTOSAR Memory Mapping - end section for code */
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>


#else /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && .. */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) */
/*==================[end of file]===========================================*/
