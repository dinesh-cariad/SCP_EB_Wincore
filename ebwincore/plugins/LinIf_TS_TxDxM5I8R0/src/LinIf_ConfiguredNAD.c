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

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00861.Enabled,1,
            LinIf.ASR20-11.SWS_LinIf_00862.Enabled,1
 */
#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinIf_Trace.h>

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

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00829,1 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetConfiguredNAD
(
  NetworkHandleType Channel,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) Nad
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_LINIF_GETCONFIGUREDNAD_ENTRY(Channel, Nad);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    /* !LINKSTO LinIf.EB.UnInit.GetConfiguredNAD,1,
                LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1
     */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_GETCONFNAD, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       ||  (Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels)))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00831,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_GETCONFNAD, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if (NULL_PTR == Nad)
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00832,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_GETCONFNAD, LINIF_E_PARAM_POINTER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
        LINIF_GET_CHANNEL_SLAVE(Channel);

    uint8 flags;

    TS_AtomicAssign8(flags, pIfChannelSlave->Flags);

    if(0U == (flags & LINIF_FLG_CONF_NAD_ALTER))
    {
      TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00830,1 */
      *Nad = pIfChannelSlave->ConfNad;

      TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);

      Result = E_OK;
    }
  }

  DBG_LINIF_GETCONFIGUREDNAD_EXIT(Channel, Result, Nad);

  return Result;
}

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00833,1 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetConfiguredNAD
(
  NetworkHandleType Channel,
  uint8 Nad
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_LINIF_SETCONFIGUREDNAD_ENTRY(Channel, Nad);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    /* !LINKSTO LinIf.EB.UnInit.SetConfiguredNAD,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_SETCONFNAD, LINIF_E_UNINIT);
  }
  else if ((Channel < LinIf_ConfigPtr->NumChannelsMaster)
       || ( Channel >= LinIf_ConfigPtr->NumSlaveChannels))
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00835,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_SETCONFNAD, LINIF_E_NONEXISTENT_CHANNEL);
  }
  else if (0U == Nad)
  {
    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00836,1 */
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_SETCONFNAD, LINIF_E_PARAMETER);
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    CONSTP2VAR(LinIf_SlaveChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelSlave =
      LINIF_GET_CHANNEL_SLAVE(Channel);

    uint8 flags;

    TS_AtomicAssign8(flags, pIfChannelSlave->Flags);

    if(0U == (flags & LINIF_FLG_CONF_NAD_ALTER))
    {
      TS_AtomicSetBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);

      /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00834,1 */
      pIfChannelSlave->ConfNad = Nad;

      TS_AtomicClearBit_8(&pIfChannelSlave->Flags, LINIF_BIT_FLG_CONF_NAD_ALTER);

      Result = E_OK;
    }
  }
  DBG_LINIF_SETCONFIGUREDNAD_EXIT(Result, Channel, Nad);

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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00861.Disabled,1,
            LinIf.ASR20-11.SWS_LinIf_00862.Disabled,1
 */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
