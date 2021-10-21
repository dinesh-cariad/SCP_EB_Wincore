/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/


#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00864.Enabled,1 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp function declarations */
#include <LinIf_Externals.h>  /* Callouts */
#include <LinIf_Trace.h>
#include <PduR_LinIf.h>         /* PDU Router LIN Interface */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <LinIf_Version.h>
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
#if (LINIF_CDD_REPORTING_ENABLE == STD_OFF)
#include <Mirror.h>
#endif /* LINIF_CDD_REPORTING_ENABLE == STD_OFF */
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_FilterSupplFuncID
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_NC_AssignNad
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

#if (LINIF_NC_SAVECONF_SUPPORTED == STD_ON)
STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_NC_SaveConf
(
  NetworkHandleType Channel
);
#endif

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_NC_AssignFrmIdRange
(
  P2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_APPL_DATA) pIfChConfSlave,
  P2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_NI_ReadById
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_HandleNCReq
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
);

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */
/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_91005_1,1 */
FUNC(void, LINIF_CODE) LinIf_RxIndication
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Lin_SduPtr
)
{
  DBG_LINIF_RXINDICATION_ENTRY(Channel, Lin_SduPtr);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if(LINIF_UNINIT_ACCESS)
  {
    /* !LINKSTO LinIf.EB.UnInit.RxIndication,1,
                LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
     */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_RXIND, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00849,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
              LINIF_API_RXIND, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if (NULL_PTR == Lin_SduPtr)
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00850,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
              LINIF_API_RXIND, LINIF_E_PARAM_POINTER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
      &LinIf_ChannelInfo[Channel];

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00189,1 */
    if(pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
    {
      CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
        LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
      CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
        LINIF_GET_CHANNEL_SLAVE(Channel);

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00754.RxIndication,1 */
      LinIf_StartTimerBI(pIfChConfSlave, pIfChannelSlave);

      switch(pIfChannel->LastHeaderType)
      {
        case LINIF_FRM_RX:
        {
          PduInfoType pdu;

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00734.Length,1 */
          pdu.SduLength = pIfChannel->LastDl;
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00734.DataPtr,1 */
          pdu.SduDataPtr = Lin_SduPtr;

          LinIf_Status_PduRRxInd(Channel, &pdu);

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
          {
            uint16 enabledChannels;

            TS_AtomicAssign16(enabledChannels, LinIf_BusMiroringChannelEnabled);
            if (LINIF_CHANNEL_ENABLED == LINIF_MIRROR_CHANNEL_ENABLED(enabledChannels, Channel))
            {
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00838,1 */
              LINIF_UL_REPORTING(LINIF_GET_COMM_CHANNEL(Channel), pIfChannel->FrameInfo.Pid, &pdu, LIN_RX_OK);
            }
          }
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
          pIfChannel->LastHeaderType = LINIF_FRM_EMPTY;
          break;
        }
        case LINIF_FRM_MRF:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00750,1 */
          if(0U == Lin_SduPtr[LINIF_PDU_BYTE_NAD])
          {
            /* GoToSleep command */
            LinIf_SlaveHandleSleep(Channel);
          }
          else
          {
            LinIf_NCHReturnType ret = LinIf_HandleNCReq(Channel, Lin_SduPtr);
            if(LINIF_NC_HANDLE_OK == ret)
            {
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00785_1,1 */
              LinIf_StartTimerNas(pIfChConfSlave, pIfChannelSlave);
            }
#if (LINIF_TP_SUPPORTED == STD_ON)
            else if(LINIF_NC_HANDLE_FWD_TP == ret)
            {
              PduInfoType pdu;

              pdu.SduLength = 8U;
              pdu.SduDataPtr = Lin_SduPtr;

              LinTp_CancelReceiveHandling(Channel);
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00772,1,
                          LinIf.ASR20-11.SWS_LinIf_00773,1
               */
              LinTp_RxIndication(Channel, &pdu);
            }
#endif /* LINIF_TP_SUPPORTED == STD_ON */
            else if(LINIF_NC_HANDLE_E_NOT_OK == ret)
            {
              /* LINIF_NC_HANDLE_E_NOT_OK */
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00787,1,
                          LinIf.ASR20-11.SWS_LinIf_00774,1
               */
              pIfChannelSlave->RSID = 0U;
            }
            else
            {
              /* LINIF_NC_HANDLE_IGNORE */
              /* nothing to do */
            }
          }
          pIfChannel->LastHeaderType = LINIF_FRM_EMPTY;
          break;
        }
        case LINIF_FRM_EMPTY:
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00848,1 */
          break;
        }
        /* CHECK: NOPARSE */
        default:
        {
          /*should not happen*/
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
          LINIF_UNREACHABLE_CODE_ASSERT(LINIF_API_RXIND);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
          break;
        /* CHECK: PARSE */
        }
      }
    }
  }

  DBG_LINIF_RXINDICATION_EXIT(Channel, Lin_SduPtr);
}

STATIC FUNC(Std_ReturnType, LINIF_CODE) LinIf_FilterSupplFuncID
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
    LINIF_CFG_GET_CHANNEL_SLAVE(Channel);

  Std_ReturnType ret = E_NOT_OK;

  /* Supplier ID wildcard or this node */
  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00778_2,1 */
  if( ((dataPtr[0U] == LINIF_WC_SUPPLID_LSB) && (dataPtr[1U] == LINIF_WC_SUPPLID_MSB))
    ||((dataPtr[0U] == pIfChConfSlave->NCSupIdB0) && (dataPtr[1U] == pIfChConfSlave->NCSupIdB1))
    )
  {
    /* Function ID wildcard or this node */
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00778_1,1 */
    if( ((dataPtr[2U] == LINIF_WC_FUNC_ID_LSB) && (dataPtr[3U] == LINIF_WC_FUNC_ID_MSB))
      ||((dataPtr[2U] == pIfChConfSlave->NCFuncIdB0) && (dataPtr[3U] == pIfChConfSlave->NCFuncIdB1))
      )
    {
      ret = E_OK;
    }
  }

  return ret;
}

STATIC FUNC(LinIf_NCHReturnType, LINIF_CODE) LinIf_NC_AssignNad
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  LinIf_NCHReturnType ret = LINIF_NC_HANDLE_E_NOT_OK;

  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00810_1,1 */
  if(E_OK == LinIf_FilterSupplFuncID(Channel, &dataPtr[3U]))
  {
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00779_1,1 */
    TS_AtomicAssign8(pIfChannelSlave->ConfNad, dataPtr[7U]);

    pIfChannelSlave->RSID = LINIF_ASSIGN_NAD_RSID;

    ret = LINIF_NC_HANDLE_OK;
  }

  return ret;
}

#if (LINIF_NC_SAVECONF_SUPPORTED == STD_ON)
STATIC FUNC(LinIf_NCHReturnType, LINIF_CODE) LinIf_NC_SaveConf
(
  NetworkHandleType Channel
)
{
  LinIf_NCHReturnType ret = LINIF_NC_HANDLE_E_NOT_OK;
  /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00812.SaveConfig.ON,1,
              LinIf.ASR20-11.SWS_LinIf_00782_1,1
   */
  if(FALSE != LINIF_NC_SAVECONF_FUNC(LINIF_GET_COMM_CHANNEL(Channel)))
  {
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

    pIfChannelSlave->RSID = LINIF_SAVECONF_RSID;
    ret = LINIF_NC_HANDLE_OK;
  }
  else
  {
    /* Do nothing, no response shall be transmitted */
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00782_3,1 */
  }
  return ret;
}
#endif

STATIC FUNC(LinIf_NCHReturnType, LINIF_CODE) LinIf_NC_AssignFrmIdRange
(
  P2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_APPL_DATA) pIfChConfSlave,
  P2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  LinIf_NCHReturnType ret = LINIF_NC_HANDLE_E_NOT_OK;

  uint8 flags;

  TS_AtomicAssign8(flags, pIfChannelSlave->Flags);
  TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);

  if(0U == (flags & LINIF_FLG_CONF_PID_ALTER))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00811,1 */
    uint8 startIdx = dataPtr[3U];
    uint8 idx;

    for(idx = 0U; idx < 4U; idx++)
    {
      uint8 upperLimit = pIfChConfSlave->FrameStartInd + pIfChConfSlave->FrameNumAssoc;
      uint8 offset = startIdx + idx;
      uint8 pid = dataPtr[4U + idx];

      if((pid != LINIF_UNUSED_DATA) && (offset >= upperLimit))
      {
        break;
      }

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00809_1,1,
                  LinIf.ASR20-11.SWS_LinIf_00809_2,1
       */
      if((pid != 0x3C) && (pid != 0x3D))
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00781_1,1 */
        LinIf_PidTable[pIfChConfSlave->FrameStartInd + offset] = pid;
      }
    }

    if(4U == idx)
    {
      pIfChannelSlave->RSID = LINIF_ASSIGN_FRMIDRNG_RSID;
      ret = LINIF_NC_HANDLE_OK;
    }

    TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);
  }

  return ret;
}

STATIC FUNC(LinIf_NCHReturnType, LINIF_CODE) LinIf_NI_ReadById
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

  LinIf_NCHReturnType ret = LINIF_NC_HANDLE_E_NOT_OK;

  if(E_OK == LinIf_FilterSupplFuncID(Channel, &dataPtr[4U]))
  {
    /* !LINKSTO LinIf.EB.Slave.LinIfSerialNumber.DISABLED,1 */
    ret = LINIF_NC_HANDLE_FWD_TP;

    switch(dataPtr[3U])
    {
      case LINIF_ID_READ_ID_PI:
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00813,1,
                    LinIf.ASR20-11.SWS_LinIf_00783,1
         */
        TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_READ_BY_ID_1);
        pIfChannelSlave->RSID = LINIF_READ_ID_RSID;
        ret = LINIF_NC_HANDLE_OK;
        break;
      }
      case LINIF_ID_READ_ID_SN:
      {
        /* Check if supported, otherwise forward to TP */
        /* !LINKSTO LinIf.EB.Slave.LinIfSerialNumber.ENABLED,1 */
        if(LINIF_SERNUM_MASK == (LINIF_CFG_GET_CHANNEL[Channel].CfgFlags & LINIF_SERNUM_MASK))
        {
          TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_READ_BY_ID_1);
          pIfChannelSlave->RSID = LINIF_READ_ID_RSID;
          ret = LINIF_NC_HANDLE_OK;
        }
        break;
      }
      case LINIF_ID_READ_ID_BT:
      {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00840,1,
                    LinIf.ASR20-11.SWS_LinIf_00841,1
         */
        pIfChannelSlave->RSID = LINIF_STANDARD_RSID_NEGATIVE_RESPONSE;
        ret = LINIF_NC_HANDLE_OK;
        break;
      }
      default:
        /* Handled by TP if enabled */
        break;
    }
  }

  return ret;
}

STATIC FUNC(LinIf_NCHReturnType, LINIF_CODE) LinIf_HandleNCReq
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataPtr
)
{
  CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
    LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
  CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
    LINIF_GET_CHANNEL_SLAVE(Channel);
  LinIf_NCHReturnType Result = LINIF_NC_HANDLE_FWD_TP;

  if((dataPtr[LINIF_PDU_BYTE_NAD] == LINIF_FUNCTIONAL_NAD) && (0U != pIfChannelSlave->RSID))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00871,1 */
    Result = LINIF_NC_HANDLE_IGNORE;
  }
  else
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00771.NAD,1,
                LinIf.ASR20-11.SWS_LinIf_00771.PCI,1,
                LinIf.ASR20-11.SWS_LinIf_00771.SID,1,
                LinIf.ASR20-11.SWS_LinIf_00778_3,1,
                LinIf.ASR20-11.SWS_LinIf_00789,1
     */
    switch(dataPtr[LINIF_PDU_BYTE_SID])
    {
      case LINIF_SID_ASSIGN_NAD:
      {
        Result = LINIF_NC_HANDLE_E_NOT_OK;

        if((dataPtr[LINIF_PDU_BYTE_NAD] == pIfChConfSlave->NCInitNad) || (dataPtr[LINIF_PDU_BYTE_NAD] == LINIF_WC_NAD))
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00790,1 */
          Result = LINIF_NC_HANDLE_IGNORE;

          if(dataPtr[LINIF_PDU_BYTE_PCI] == 0x06U)
          {
            Result = LinIf_NC_AssignNad(Channel, dataPtr);
          }
        }
        break;
      }
#if(LINIF_NC_SAVECONF_SUPPORTED == STD_ON)
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00812.SaveConfig.OFF,1 */
      case LINIF_SID_SAVECONF:
      {
        Result = LINIF_NC_HANDLE_E_NOT_OK;
        if((dataPtr[LINIF_PDU_BYTE_NAD] == pIfChannelSlave->ConfNad) || (dataPtr[LINIF_PDU_BYTE_NAD] == LINIF_WC_NAD))
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00790,1 */
          Result = LINIF_NC_HANDLE_IGNORE;

          if(dataPtr[LINIF_PDU_BYTE_PCI] == 0x01U)
          {
            Result = LinIf_NC_SaveConf(Channel);
          }
        }
        break;
      }
#endif /* LINIF_NC_SAVECONF_SUPPORTED == STD_ON */
      case LINIF_SID_ASSIGN_FRMIDRNG:
      {
        Result = LINIF_NC_HANDLE_E_NOT_OK;
        if((dataPtr[LINIF_PDU_BYTE_NAD] == pIfChannelSlave->ConfNad) || (dataPtr[LINIF_PDU_BYTE_NAD] == LINIF_WC_NAD))
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00790,1 */
          Result = LINIF_NC_HANDLE_IGNORE;

          if(dataPtr[LINIF_PDU_BYTE_PCI] == 0x06U)
          {
            Result = LinIf_NC_AssignFrmIdRange(pIfChConfSlave, pIfChannelSlave, dataPtr);
          }
        }
        break;
      }
      case LINIF_SID_READ_ID:
      {
        Result = LINIF_NC_HANDLE_E_NOT_OK;
        if((dataPtr[LINIF_PDU_BYTE_NAD] == pIfChannelSlave->ConfNad) || (dataPtr[LINIF_PDU_BYTE_NAD] == LINIF_WC_NAD))
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00790,1 */
          Result = LINIF_NC_HANDLE_IGNORE;

          if(dataPtr[LINIF_PDU_BYTE_PCI] == 0x06U)
          {
            Result = LinIf_NI_ReadById(Channel, dataPtr);
          }
        }
        break;
      }
      default:
      {
        /* Forwarded to TP */
        break;
      }
    }
  }

  return Result;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else
#if (TS_MERGED_COMPILE == STD_OFF)
#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* TS_MERGED_COMPILE */
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00864.Disabled,1 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
