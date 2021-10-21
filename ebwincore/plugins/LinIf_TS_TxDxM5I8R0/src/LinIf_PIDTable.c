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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00859.Enabled,1,
            LinIf.ASR20-11.SWS_LinIf_00860.Enabled,1
 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinIf_Trace.h>
#include <SchM_LinIf.h>       /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <LinIf_Version.h>
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

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

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_91002,1 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetPIDTable
(
  NetworkHandleType Channel,
  P2VAR(Lin_FramePidType, AUTOMATIC, AUTOMATIC) PidBuffer,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) PidBufferLength
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_LINIF_GETPIDTABLE_ENTRY(Channel, PidBuffer, PidBufferLength);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
      /* !LINKSTO LinIf.EB.UnInit.GetPIDTable,1,
                  LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
       */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_GETPIDTABLE, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00818,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_GETPIDTABLE, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if ((NULL_PTR == PidBufferLength)
       || ((NULL_PTR == PidBuffer) && (0U != *PidBufferLength)))
  {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00819,1,
                  LinIf.ASR20-11.SWS_LinIf_00820,1
       */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_GETPIDTABLE, LINIF_E_PARAM_POINTER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
      LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

    if(0U == *PidBufferLength)
    {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00828,1 */
      *PidBufferLength = pIfChConfSlave->FrameNumAssoc;
      Result = E_OK;
    }
    else if(pIfChConfSlave->FrameNumAssoc <= *PidBufferLength)
    {
      uint8 flags;
      uint8 i;

      TS_AtomicAssign8(flags, pIfChannelSlave->Flags);

      if(0U == (flags & LINIF_FLG_CONF_PID_ALTER))
      {
        TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);

        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00816.PidBufferLength,1 */
        *PidBufferLength = pIfChConfSlave->FrameNumAssoc;
        for(i = 0; i < pIfChConfSlave->FrameNumAssoc; i++)
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00816.PidBuffer,1 */
          PidBuffer[i] = LinIf_PidTable[i + pIfChConfSlave->FrameStartInd];
        }

        TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);

        Result = E_OK;
      }
    }
    else
    {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                LINIF_API_GETPIDTABLE, LINIF_E_PARAMETER);
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    }
  }

  DBG_LINIF_GETPIDTABLE_EXIT(Result, Channel, PidBuffer, PidBufferLength);

  return Result;
}

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_91003,1 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetPIDTable
(
  NetworkHandleType Channel,
  P2VAR(Lin_FramePidType, AUTOMATIC, AUTOMATIC) PidBuffer,
  uint8 PidBufferLength
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_LINIF_SETPIDTABLE_ENTRY(Channel, PidBuffer, PidBufferLength);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
      /* !LINKSTO LinIf.EB.UnInit.SetPIDTable,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_SETPIDTABLE, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       || ( Channel >= LinIf_ConfigPtr->NumSlaveChannels))
  {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00825,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_SETPIDTABLE, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if (NULL_PTR == PidBuffer)
  {
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00826,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                            LINIF_API_SETPIDTABLE, LINIF_E_PARAM_POINTER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2CONST(LinIfSlaveChannelType, AUTOMATIC, LINIF_VAR) pIfChConfSlave =
      LINIF_CFG_GET_CHANNEL_SLAVE(Channel);
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

    if(PidBufferLength == pIfChConfSlave->FrameNumAssoc)
    {
      uint8 flags;
      uint8 i;

      TS_AtomicAssign8(flags, pIfChannelSlave->Flags);

      if(0U == (flags & LINIF_FLG_CONF_PID_ALTER))
      {
        TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);

        for(i = 0; i < pIfChConfSlave->FrameNumAssoc; i++)
        {
          if(LINIF_UNUSED_DATA != PidBuffer[i])
          {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00823,1 */
            LinIf_PidTable[pIfChConfSlave->FrameStartInd + i] = PidBuffer[i];
          }
        }

        TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_PID_ALTER);

        Result = E_OK;
      }
    }
    else
    {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00827,1 */
      (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                LINIF_API_SETPIDTABLE, LINIF_E_PARAMETER);
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    }
  }

  DBG_LINIF_SETPIDTABLE_EXIT(Result, Channel, PidBuffer, PidBufferLength);

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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00859.Disabled,1,
            LinIf.ASR20-11.SWS_LinIf_00860.Disabled,1
 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
