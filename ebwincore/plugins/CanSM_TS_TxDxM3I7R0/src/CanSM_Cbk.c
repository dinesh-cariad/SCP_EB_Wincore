/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <CanSM_Trace.h>
#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* AUTOSAR communication stack types */
#include <TSAtomic_Assign.h>        /* Atomic assignment macros */

#include <CanSM_Cfg.h>              /* Module callback API */
#include <CanSM_Cbk.h>              /* Module callback API */
#include <CanSM_Internal.h>         /* Module private API */

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                    /* Development Error Tracer */
#endif

#if (CANSM_PNSUPPORT == STD_ON)
#include <CanNm.h>                  /* CanNm_ConfirmPnAvailability() */
#endif

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(void, CANSM_CODE) CanSM_ControllerBusOff
(
  uint8 ControllerId
)
{
  DBG_CANSM_CONTROLLERBUSOFF_ENTRY(ControllerId);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  /* CANSM190 */
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_CONTROLLERBUSOFF, CANSM_E_UNINIT);
  }
  else
  {

    if ( CANSM_NW_IDX_FROM_CONTROLLER(ControllerId) == CANSM_NETWORKS_NUM )
    {
    /* no network found */
    CANSM_DET(CANSM_SID_CONTROLLERBUSOFF, CANSM_E_PARAM_CONTROLLER);
    }
    else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
    {

      const uint8 sigBusOff = CANSM_BUS_OFF;
      const uint8 sigCtrlIndication = CANSM_CTRLMODETYPE_STOPPED;

      /* !LINKSTO dsn.CanSM.CallContext.CanSM_ControllerBusOff,1 */
      TS_AtomicAssign8( CANSM_NW_INFO_BUSOFF( CANSM_NW_IDX_FROM_CONTROLLER(ControllerId) ), sigBusOff );

      /* Consider multiple controllers for a single network:
       * Since a bus-off triggers a state change in the CAN driver, we reproduce
       * the state change in CanSM.
       * This ensure that we can distinguish which controller needs to be restarted
       * during bus-off recovery.  */
      /* !LINKSTO CanSM.EB.BusOffRecovery.RestartCtrl,1 */
      TS_AtomicAssign8( CanSM_CtrlModeIndication[CanSM_CtrlIdxFromCtrlId(ControllerId)], sigCtrlIndication );

    }

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  }
#endif

  DBG_CANSM_CONTROLLERBUSOFF_EXIT(ControllerId);
}

FUNC(void, CANSM_CODE) CanSM_ControllerModeIndication (
  uint8 ControllerId,
  CanIf_ControllerModeType ControllerMode
)
{

  DBG_CANSM_CONTROLLERMODEINDICATION_ENTRY(ControllerId,ControllerMode);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_CONTROLLERMODEINDICATION, CANSM_E_UNINIT);
  }
  else if ( CANSM_NW_IDX_FROM_CONTROLLER(ControllerId) == CANSM_NETWORKS_NUM )
  {
    /* no network found */
    CANSM_DET(CANSM_SID_CONTROLLERMODEINDICATION, CANSM_E_PARAM_CONTROLLER);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */
  {
    uint8 buf;
    switch (ControllerMode)
    {
      case CANIF_CS_STOPPED:
        buf = CANSM_CTRLMODETYPE_STOPPED;
        break;
      case CANIF_CS_SLEEP:
        buf = CANSM_CTRLMODETYPE_SLEEP;
        break;
      case CANIF_CS_STARTED:
        buf = CANSM_CTRLMODETYPE_STARTED;
        break;
      default:
        buf = CANSM_CTRLMODETYPE_RESET;
        break;
    }

    TS_AtomicAssign8(
        CanSM_CtrlModeIndication[CanSM_CtrlIdxFromCtrlId(ControllerId)],
        buf
        );
  }

  DBG_CANSM_CONTROLLERMODEINDICATION_EXIT(ControllerId,ControllerMode);
}

#if (CANSM_USE_TRANSCEIVER == STD_ON)

FUNC(void, CANSM_CODE) CanSM_TransceiverModeIndication (
  uint8 TransceiverId,
  CanTrcv_TrcvModeType TransceiverMode
)
{

  DBG_CANSM_TRANSCEIVERMODEINDICATION_ENTRY(TransceiverId,TransceiverMode);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_TRANSCEIVERMODEINDICATION, CANSM_E_UNINIT);
  }
  else if ( CANSM_NW_IDX_FROM_TRCV(TransceiverId) == CANSM_NETWORKS_NUM )
  {
    /* no network found */
    CANSM_DET(CANSM_SID_TRANSCEIVERMODEINDICATION, CANSM_E_PARAM_TRANSCEIVER);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

  {
    uint8 buf;
    switch (TransceiverMode)
    {
      case CANTRCV_TRCVMODE_NORMAL:
        buf = CANSM_TRCVMODETYPE_NORMAL;
        break;
      case CANTRCV_TRCVMODE_STANDBY:
        buf = CANSM_TRCVMODETYPE_STANDBY;
        break;
      default:
        buf = CANSM_TRCVMODETYPE_RESET;
        break;
    }

    TS_AtomicAssign8(
        CANSM_NW_INFO_TRCVMODEINDICATION(CANSM_NW_IDX_FROM_TRCV(TransceiverId)),
        buf
        );
  }

  DBG_CANSM_TRANSCEIVERMODEINDICATION_EXIT(TransceiverId,TransceiverMode);
}

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */


#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )

FUNC(void, CANSM_CODE) CanSM_ClearTrcvWufFlagIndication (
  uint8 Transceiver
)
{

  DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_ENTRY(Transceiver);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_CLEARTRCVWUFFLAGINDICATION, CANSM_E_UNINIT);
  }
  else if ( CANSM_NW_IDX_FROM_TRCV(Transceiver) == CANSM_NETWORKS_NUM )
  {
    /* no network found */
    CANSM_DET(CANSM_SID_CLEARTRCVWUFFLAGINDICATION, CANSM_E_PARAM_TRANSCEIVER);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

  {

    uint8 nw = CANSM_NW_IDX_FROM_TRCV(Transceiver);
    TS_AtomicAssign8(CANSM_NW_INFO_TRCVCLEARWUFINDICATION(nw), TRUE);

  }

  DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_EXIT(Transceiver);
}

FUNC(void, CANSM_CODE) CanSM_CheckTransceiverWakeFlagIndication (
  uint8 Transceiver
)
{

  DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_ENTRY(Transceiver);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION, CANSM_E_UNINIT);
  }
  else if ( CANSM_NW_IDX_FROM_TRCV(Transceiver) == CANSM_NETWORKS_NUM )
  {
    /* no network found */
    CANSM_DET(CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION, CANSM_E_PARAM_TRANSCEIVER);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

  {

    uint8 nw = CANSM_NW_IDX_FROM_TRCV(Transceiver);
    TS_AtomicAssign8(CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw), TRUE);

  }

  DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_EXIT(Transceiver);
}

FUNC(void, CANSM_CODE) CanSM_ConfirmPnAvailability (
  uint8 TransceiverId
)
{
  DBG_CANSM_CONFIRMPNAVAILABILITY_ENTRY(TransceiverId);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
    CANSM_DET(CANSM_SID_CONFIRMPNAVAILABILITY, CANSM_E_UNINIT);
  }
  else if ( CANSM_NW_IDX_FROM_TRCV(TransceiverId) == CANSM_NETWORKS_NUM )
  {
    /* no network found */
    CANSM_DET(CANSM_SID_CONFIRMPNAVAILABILITY, CANSM_E_PARAM_TRANSCEIVER);
  }
  else
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

  {
    NetworkHandleType NwHandle = CANSM_NW_HANDLE_FROM_IDX(CANSM_NW_IDX_FROM_TRCV(TransceiverId));

    /* confirm the PN availability to the CanNm module */
    CanNm_ConfirmPnAvailability(NwHandle);
  }

  DBG_CANSM_CONFIRMPNAVAILABILITY_EXIT(TransceiverId);
}
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

