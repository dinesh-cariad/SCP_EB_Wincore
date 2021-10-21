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

#include <LinIf_Trace.h>

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>      /* Module library API */
#include <LinIf_Int.h>  /* LinIf internal data */
#include <SchM_LinIf.h> /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
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

#if (LINIF_TRCV_SUPPORTED == STD_ON)

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE  */

/** \brief Set the given LIN transceiver to the given mode
 ** \param[in] Channel Identification of the LinIf channel
 ** \param[in] TransceiverMode Requested mode transition
 **/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
  NetworkHandleType Channel,
  LinTrcv_TrcvModeType TransceiverMode
)
{
  Std_ReturnType retVal;

  DBG_LINIF_SETTRCVMODE_ENTRY(Channel, TransceiverMode);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVMODE,
                          LINIF_E_UNINIT
                         );
    retVal = E_NOT_OK;
  }
  else if(Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (LINIF_TRCV_INVALIDCHANNELID == LINIF_GET_TRCV_CHID(Channel))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (((uint8)((uint8)LINTRCV_TRCV_MODE_NORMAL | (uint8)LINTRCV_TRCV_MODE_STANDBY) | (uint8)LINTRCV_TRCV_MODE_SLEEP) < (uint8)TransceiverMode)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVMODE,
                          LINIF_E_TRCV_INV_MODE
                         );
    retVal = E_NOT_OK;
  }
  else if (LINTRCV_TRCV_MODE_SLEEP == TransceiverMode)
  {
    LinTrcv_TrcvModeType currentMode = LINTRCV_TRCV_MODE_SLEEP;
    if ((E_OK != LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].GetOpMode(LINIF_GET_TRCV_CHID(Channel), &currentMode))
        || (LINTRCV_TRCV_MODE_NORMAL != currentMode))
    {
      (void)Det_ReportError(LINIF_MODULE_ID,
                            LINIF_INSTANCE_ID,
                            LINIF_API_SETTRCVMODE,
                            LINIF_E_TRCV_NOT_NORMAL
                           );
      retVal = E_NOT_OK;
    }
    else
    {
      retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].SetOpMode(LINIF_GET_TRCV_CHID(Channel), TransceiverMode);
    }
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].SetOpMode(LINIF_GET_TRCV_CHID(Channel), TransceiverMode);
  }

  DBG_LINIF_SETTRCVMODE_EXIT(retVal, Channel, TransceiverMode);

  return retVal;
}

/** \brief Queries the actual state of the LinTrcv Driver
 ** \param[in] Channel Identification of the LinIf channel
 ** \param[out] TransceiverModePtr Pointer to a memory location
 **                                where the mode will be stored
 **/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
  NetworkHandleType Channel,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, AUTOMATIC) TransceiverModePtr
)
{
  Std_ReturnType retVal;

  DBG_LINIF_GETTRCVMODE_ENTRY(Channel, TransceiverModePtr);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVMODE,
                          LINIF_E_UNINIT
                         );
    retVal = E_NOT_OK;
  }
  else if(Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (LINIF_TRCV_INVALIDCHANNELID == LINIF_GET_TRCV_CHID(Channel))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (NULL_PTR == TransceiverModePtr)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVMODE,
                          LINIF_E_PARAMETER_POINTER
                         );
    retVal = E_NOT_OK;
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].GetOpMode(LINIF_GET_TRCV_CHID(Channel), TransceiverModePtr);
  }

  DBG_LINIF_GETTRCVMODE_EXIT(retVal, Channel, TransceiverModePtr);

  return retVal;
}

/** \brief Queries the reason for the wake up that has been
 *         detected by the LinTrcv Driver
 ** \param[in] Channel Identification of the LinIf channel
 ** \param[out] TrcvWuReasonPtr Pointer to a memory location
 **                                where the reason will be stored
 **/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
  NetworkHandleType Channel,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, AUTOMATIC) TrcvWuReasonPtr
)
{
  Std_ReturnType retVal;

  DBG_LINIF_GETTRCVWAKEUPREASON_ENTRY(Channel, TrcvWuReasonPtr);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVWUREASON,
                          LINIF_E_UNINIT
                         );
    retVal = E_NOT_OK;
  }
  else if(Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVWUREASON,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (LINIF_TRCV_INVALIDCHANNELID == LINIF_GET_TRCV_CHID(Channel))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVWUREASON,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (NULL_PTR == TrcvWuReasonPtr)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_GETTRCVWUREASON,
                          LINIF_E_PARAMETER_POINTER
                         );
    retVal = E_NOT_OK;
  }
  else
  {
    LinTrcv_TrcvModeType currentMode = LINTRCV_TRCV_MODE_SLEEP;
    if ((E_OK != LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].GetOpMode(LINIF_GET_TRCV_CHID(Channel), &currentMode))
        || (LINTRCV_TRCV_MODE_NORMAL != currentMode))
    {
      (void)Det_ReportError(LINIF_MODULE_ID,
                            LINIF_INSTANCE_ID,
                            LINIF_API_GETTRCVWUREASON,
                            LINIF_E_TRCV_NOT_NORMAL
                           );
      retVal = E_NOT_OK;
    }
    else
    {
      retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].GetBusWuReason(LINIF_GET_TRCV_CHID(Channel), TrcvWuReasonPtr);
    }
  }
#else
  retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].GetBusWuReason(LINIF_GET_TRCV_CHID(Channel), TrcvWuReasonPtr);
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

  DBG_LINIF_GETTRCVWAKEUPREASON_EXIT(retVal, Channel, TrcvWuReasonPtr);

  return retVal;
}

/** \brief Enables, disables and clears the notification
 *         for wakeup events on the addressed network
 ** \param[in] Channel Identification of the LIN channel
 ** \param[in] LinTrcvWakeupMode Requested transceiver wake up reason.
 **/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
  NetworkHandleType Channel,
  LinTrcv_TrcvWakeupModeType LinTrcvWakeupMode
)
{
  Std_ReturnType retVal;

  DBG_LINIF_SETTRCVWAKEUPMODE_ENTRY(Channel, LinTrcvWakeupMode);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
  /* check DET errors first */
  if (LINIF_UNINIT_ACCESS)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVWUMODE,
                          LINIF_E_UNINIT
                         );
    retVal = E_NOT_OK;
  }
  else if(Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVWUMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (LINIF_TRCV_INVALIDCHANNELID == LINIF_GET_TRCV_CHID(Channel))
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVWUMODE,
                          LINIF_E_TRCV_INV_CHANNEL
                         );
    retVal = E_NOT_OK;
  }
  else if (((uint8)((uint8)LINTRCV_WUMODE_ENABLE | (uint8)LINTRCV_WUMODE_DISABLE) | (uint8)LINTRCV_WUMODE_CLEAR) < (uint8)LinTrcvWakeupMode)
  {
    (void)Det_ReportError(LINIF_MODULE_ID,
                          LINIF_INSTANCE_ID,
                          LINIF_API_SETTRCVWUMODE,
                          LINIF_E_PARAMETER
                         );
    retVal = E_NOT_OK;
  }
  else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
  {
    retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(Channel)].SetWakeupMode(LINIF_GET_TRCV_CHID(Channel), LinTrcvWakeupMode);
  }

  DBG_LINIF_SETTRCVWAKEUPMODE_EXIT(retVal, Channel, LinTrcvWakeupMode);

  return retVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#endif /* LINIF_TRCV_SUPPORTED == STD_ON */
/*==================[end of file]===========================================*/
