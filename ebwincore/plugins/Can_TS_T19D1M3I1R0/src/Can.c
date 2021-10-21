/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2004 Deviations:
 *
 * MISRA-1) Deviated Rule: 19.1 (advisory)
 *   '#include' statements in a file should only be preceded by other
 *   preprocessor directives or comments.
 *
 *   Reason:
 *   Inclusion of MemMap.h.
 *
 * MISRA-2) Deviated Rule: 11.3 (advisory)
 *   A cast should not be performed between a pointer type and an
 *   integral type.
 *
 *   Reason:
 *   Use of NULL pointer.
 *
 */

/*==[Includes]================================================================*/
#include <TSAutosar.h>
#include <Os.h>
#include <Can.h>
#include <Can_Internal.h>
#include <Can_Trace.h>
#include <Can_WinApi.h>
#include <CanIf_Cbk.h>
#include <CanIf.h>
#include <SchM_Can.h>
#include <VirtualBusInterfaceC.h>
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
#include <Det.h>
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
#include "cif_swig_defs.h"

/*==[Macros]==================================================================*/
/** \brief Dummy value used in virtual bus data structure */
#define CAN_VB_DUMMY_CHANNEL 1U

/** \brief Calculate next index in circular buffer */
#define CAN_NEXT_IDX(idx) (uint8)(((idx)+1U) % CAN_BUFFER_SIZE)

/** \brief Maximum Can_DisableControllerInterrupts() nesting depth */
#define CAN_MAX_DIS_INT_DEPTH 255U

/*==[Types]===================================================================*/
/** \brief States of global state machine */
typedef enum
{
  CAN_UNINIT,
  CAN_WINLAYER_INIT,
  CAN_READY
} Can_InitStateType;

/*==[Declaration of functions with internal linkage]==========================*/
/* Deviation MISRA-1 */
#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

/** \brief Frame reception callback
 **
 ** This callback function is called by the VirtualBus library after
 ** reception of a CAN frame.
 */
STATIC FUNC(void, CAN_CODE) Can_VBusCallback(VirtualBusDataType* Can_RawRxData);

#if (CAN_NUM_HRH > 0)
/** \brief Store received frame
 **
 ** This function stores a received frame in the circular buffer.
 **
 ** \param[in] index Index of the buffer to store the frame into. Note: This
 **                  is *not* the index within the buffer, it is the index
 **                  *of* the buffer (since there is an array of buffers: one
 **                  for each HRH)!
 ** \param[in] frame Frame to store in the buffer
 ** \param[in] sdu   Actual data. For reasons of performance this has been
 **                  decoupled from the 'frame' parameter (some extra copies,
 **                  partly in interrupt context, would be needed otherwise).
 **
 ** \return Result of the store request
 ** \retval CAN_BUF_OK The frame has successfully been stored.
 ** \retval CAN_BUF_FULL The frame has been discarded because the reception
 **                      buffer is full (can only happen if the buffer mode
 **                      is set to CAN_BUFFER_KEEP_OLDEST).
 */
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_StoreInRxBuffer(
          uint8 index,
          P2VAR(Can_HwBufType, AUTOMATIC, CAN_VAR) frame,
          P2VAR(uint8, AUTOMATIC, CAN_VAR) sdu);

/** \brief Retrieve received frame from buffer
 **
 ** This function returns (and removes) the oldest frame stored in the
 ** circular receive buffer.
 **
 ** \param[in] index  Index of the buffer to retrieve the frame from. Note:
 **                   This is *not* the index within the buffer, it is the index
 **                   *of* the buffer (since there is an array of buffers: one
 **                   for each HRH)!
 ** \param[out] frame Frame retrieved from the buffer
 **
 ** \return Result of the read request
 ** \retval CAN_BUF_OK The frame has successfully been read.
 ** \retval CAN_BUF_EMPTY No frame could be read because the buffer is empty.
 */
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_ReadFromRxBuffer(
          uint8 index,
          P2VAR(P2VAR(Can_HwBufType, AUTOMATIC, CAN_VAR), AUTOMATIC, CAN_VAR) frame);
#endif /* (CAN_NUM_HRH > 0) */

#if (CAN_NUM_HTH > 0)
/** \brief Store transmitted PduId
 **
 ** This function stores the PduId of a transmitted frame in the circular
 ** buffer.
 **
 ** \param[in] index Index of the buffer to store the PduId into. Note: This
 **                  is *not* the index within the buffer, it is the index
 **                  *of* the buffer (since there is an array of buffers: one
 **                  for each HTH)!
 ** \param[in] PduId PduId to store in the buffer
 **
 ** \return Result of the store request
 ** \retval CAN_BUF_OK The PduId has successfully been stored.
 ** \retval CAN_BUF_FULL The PduId has been discarded because the transmission
 **                      buffer is full.
 */
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_StoreInTxBuffer(
          uint8 index,
          PduIdType PduId);

/** \brief Retrieve transmitted PduId from buffer
 **
 ** This function returns (and removes) the oldest PduId stored in the
 ** circular transmit buffer.
 **
 ** \param[in] index  Index of the buffer to retrieve the PduId from. Note:
 **                   This is *not* the index within the buffer, it is the index
 **                   *of* the buffer (since there is an array of buffers: one
 **                   for each HTH)!
 ** \param[out] PduId Frame retrieved from the buffer
 **
 ** \return Result of the read request
 ** \retval CAN_BUF_OK The PduId has successfully been read.
 ** \retval CAN_BUF_EMPTY No PduId could be read because the buffer is empty.
 */
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_ReadFromTxBuffer(
          uint8 index,
          P2VAR(PduIdType, AUTOMATIC, CAN_VAR) PduId);
#endif /* (CAN_NUM_HTH > 0) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>

/*==[Constants with internal linkage]=========================================*/
/* Deviation MISRA-1 */
#define CAN_START_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

/** \brief Mapping between Can and CanIf controller states
 **
 ** Note: The order must match the order of the Can_ControllerStateType
 **       typedef!
 */
STATIC CONST(CanIf_ControllerModeType, CAN_CONST) Can_CanIfControllerModeMap[4] =
          {
            CANIF_CS_UNINIT,
            CANIF_CS_STARTED,
            CANIF_CS_STOPPED,
            CANIF_CS_SLEEP
          };

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

/*==[Variables with internal linkage]=========================================*/
/* Avoid unnecessary inclusion of MemMap.h */
#if ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT) || \
     (CAN_NUM_HRH > 0) || (CAN_NUM_HTH >0))

/* Deviation MISRA-1 */
#define CAN_START_SEC_VAR_8BIT
#include <Can_MemMap.h>

#if (CAN_NUM_HRH > 0)
/** \brief Index of element to be read from Rx circular buffer */
STATIC VAR(uint8, CAN_VAR) Can_RxBufReadIdx[CAN_NUM_HRH] = {0U};

/** \brief Index of element to be stored in Rx circular buffer */
STATIC VAR(uint8, CAN_VAR) Can_RxBufStoreIdx[CAN_NUM_HRH] = {0U};
#endif /* (CAN_NUM_HRH > 0) */

#if (CAN_NUM_HTH > 0)
/** \brief Index of element to be read from Tx circular buffer */
STATIC VAR(uint8, CAN_VAR) Can_TxBufReadIdx[CAN_NUM_HTH] = {0U};

/** \brief Index of element to be stored in Tx circular buffer */
STATIC VAR(uint8, CAN_VAR) Can_TxBufStoreIdx[CAN_NUM_HTH] = {0U};
#endif /* (CAN_NUM_HTH > 0) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_VAR_8BIT
#include <Can_MemMap.h>

#endif /* ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT) ||  \
          (CAN_NUM_HRH > 0) || (CAN_NUM_HTH >0)) */

/* Deviation MISRA-1 */
#define CAN_START_SEC_VAR_UNSPECIFIED
#include <Can_MemMap.h>

#if (CAN_DEV_ERROR_DETECTION == STD_ON)
/** \brief Global state machine */
STATIC VAR(Can_InitStateType, CAN_VAR) Can_InitState = CAN_UNINIT;
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */

/** \brief Flag for re-initialization of the connection */
STATIC VAR(boolean, CAN_VAR) Can_CallbackRegistered = FALSE;

#if (CAN_NUM_HRH > 0)
/** \brief Circular buffer for storing incoming frames (hardware emulation) */
STATIC VAR(Can_HwBufType, CAN_VAR) Can_RxCircBuffer[CAN_NUM_HRH][CAN_BUFFER_SIZE];
#endif /* (CAN_NUM_HRH > 0) */

#if (CAN_NUM_HTH > 0)
/** \brief Circular buffer for storing outgoiong frames (hardware emulation) */
STATIC VAR(PduIdType, CAN_VAR) Can_TxCircBuffer[CAN_NUM_HTH][CAN_BUFFER_SIZE];

/** \brief Mutexes for controller state machine */
STATIC VAR(boolean, CAN_VAR) Can_HTHLocked[CAN_NUM_HTH];
#endif /* (CAN_NUM_HTH > 0) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_VAR_UNSPECIFIED
#include <Can_MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/
/* Deviation MISRA-1 */
#define CAN_START_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

CONST(Can_ConfigType, CAN_CONST) CAN_CONFIG_SET = {0U};

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

/*==[Definition of functions with external linkage]===========================*/
/* Deviation MISRA-1 */
#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

/*------------------[Can_Init()]--------------------------------------------*/

FUNC(void, CAN_CODE) Can_Init
(
  P2CONST(Can_ConfigType, AUTOMATIC, CAN_CONST) Config
)
{
  uint8 i;
  DBG_CAN_INIT_ENTRY(Config);

#if (CAN_DEV_ERROR_DETECTION == STD_ON)

  if(Can_InitState != CAN_UNINIT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT, CAN_E_TRANSITION);
  }
  /* Deviation MISRA-2 */
  else if(Config == NULL_PTR)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT, CAN_E_PARAM_POINTER);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
    /* Only switch to CAN_READY if the virtual bus library has been
     * properly initialized */
    if(Sd_Init() == SD_OK)
    {
      Can_InitState = CAN_WINLAYER_INIT;
    }
#else
    /* If DET is off, there is no way to react on an error in the
     * virtual bus library... */
    (void)Sd_Init();
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
    TS_PARAM_UNUSED(Config);

#if (CAN_DEV_ERROR_DETECTION == STD_ON)
    if(Can_InitState == CAN_WINLAYER_INIT)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
    {
      for (i=0; i<CAN_CONTROLLER_COUNT; i++)
      {
        Can_Controllers[i].ControllerState->State = CAN_CS_STOPPED;
      }
      Can_InitializeCriticalSection();
#if (CAN_NUM_HTH > 0)
      for(i=0U; i<CAN_NUM_HTH; i++)
      {
        Can_HTHLocked[i] = FALSE;
      }
#endif /* (CAN_NUM_HTH > 0) */
      for (i=0; i<CAN_CONTROLLER_COUNT; i++)
      {
        Can_InitController(i, (Can_ControllerBaudrateConfigType*) &Can_Controllers[i]);
      }

    }
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
    if(Can_InitState == CAN_WINLAYER_INIT)
    {
      Can_InitState = CAN_READY;
    }
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  }
  DBG_CAN_INIT_EXIT(Config);
}

/*------------------[Can_GetVersionInfo()]----------------------------------*/

#if (CAN_GET_VERSION_INFO_API == STD_ON)
FUNC(void, CAN_CODE) Can_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CAN_APPL_DATA) versioninfo
)
{
  DBG_CAN_GETVERSIONINFO_ENTRY(versioninfo);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  /* Deviation MISRA-2 */
  if(versioninfo == NULL_PTR)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_GET_VERSION_INFO, CAN_E_PARAM_POINTER);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    versioninfo->vendorID         = CAN_VENDOR_ID;
    versioninfo->moduleID         = CAN_MODULE_ID;
    versioninfo->sw_major_version = CAN_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CAN_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CAN_SW_PATCH_VERSION;
  }
  DBG_CAN_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* (CAN_GET_VERSION_INFO_API == STD_ON) */

/*------------------[Can_InitController()]----------------------------------*/

FUNC(void, CAN_CODE) Can_InitController
(
  uint8 Controller,
  P2CONST(Can_ControllerBaudrateConfigType, AUTOMATIC, CAN_CONST) Config
)
{
  boolean ErrFlag = FALSE;

  DBG_CAN_INITCONTROLLER_ENTRY(Controller, Config);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if((Can_InitState != CAN_READY) && (Can_InitState != CAN_WINLAYER_INIT))
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT_CONTROLLER, CAN_E_UNINIT);
  }
  /* Deviation MISRA-2 */
  else if(Config == NULL_PTR)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT_CONTROLLER, CAN_E_PARAM_POINTER);
  }
  else if(Controller >= CAN_CONTROLLER_COUNT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT_CONTROLLER, CAN_E_PARAM_CONTROLLER);
  }
  else if(Can_Controllers[Controller].ControllerState->State != CAN_CS_STOPPED)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_INIT_CONTROLLER, CAN_E_TRANSITION);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    if(Can_CallbackRegistered == TRUE)
    {
      if(Sd_Unregister_ReceiveCallback() == SD_ERROR)
      {
        ErrFlag = TRUE;
      }
    }
    if(ErrFlag == FALSE)
    {
      Can_Controllers[Controller].ControllerState->InterruptEnabled         = TRUE;
      Can_Controllers[Controller].ControllerState->IntDisableNestingCounter = 0;
      Can_Controllers[Controller].ControllerState->TxInterruptPending       = FALSE;
      Can_Controllers[Controller].ControllerState->RxInterruptPending       = FALSE;
      if(Sd_Register_ReceiveCallback(Can_VBusCallback) == SD_OK)
      {
        /* Buffer handling is simulated by the MCAL, thus the SoftDevice may not
         * buffer frames -> queue size 1. */
        if (Sd_SetQueueSize(1U) == SD_OK)
        {
          Can_CallbackRegistered = TRUE;
        }
      }
    }
  }
  DBG_CAN_INITCONTROLLER_EXIT(Controller, Config);
}

/*------------------[Can_SetControllerMode()]-------------------------------*/

FUNC(Can_ReturnType, CAN_CODE) Can_SetControllerMode
(
  uint8 Controller,
  Can_StateTransitionType Transition
)
{
  uint8 i;
  Can_ReturnType  RetVal = CAN_NOT_OK;

  DBG_CAN_SETCONTROLLERMODE_ENTRY(Controller, Transition);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_READY)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_SET_CONTROLLER_MODE, CAN_E_UNINIT);
  }
  else if(Controller >= CAN_CONTROLLER_COUNT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_SET_CONTROLLER_MODE, CAN_E_PARAM_CONTROLLER);
  }
  else if(((Transition == CAN_T_START)  && (Can_Controllers[Controller].ControllerState->State != CAN_CS_STOPPED)) ||
          (((Transition == CAN_T_STOP)   && ((Can_Controllers[Controller].ControllerState->State != CAN_CS_STARTED) &&
              (Can_Controllers[Controller].ControllerState->State != CAN_CS_STOPPED))) ||
          (((Transition == CAN_T_SLEEP)  && ((Can_Controllers[Controller].ControllerState->State != CAN_CS_STOPPED) &&
              (Can_Controllers[Controller].ControllerState->State != CAN_CS_SLEEP))) ||
          (((Transition == CAN_T_WAKEUP) && ((Can_Controllers[Controller].ControllerState->State != CAN_CS_STOPPED) &&
              (Can_Controllers[Controller].ControllerState->State != CAN_CS_SLEEP))) ||
          (Transition > CAN_T_WAKEUP)))))
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_SET_CONTROLLER_MODE, CAN_E_TRANSITION);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    switch(Transition)
    {
      case CAN_T_START:
        if(Sd_Connect(&Can_Controllers[Controller]) == SD_OK)
        {
          Can_Controllers[Controller].ControllerState->State = CAN_CS_STARTED;
          RetVal = CAN_OK;
        }
        break;
      case CAN_T_STOP:
        if(Sd_Disconnect(&Can_Controllers[Controller]) == SD_OK)
        {
          /* Clear Rx and Tx buffers. */
#if (CAN_NUM_HRH > 0)
          for(i=0U; i<CAN_NUM_HRH; i++)
          {
            Can_RxBufStoreIdx[i] = 0U;
            Can_RxBufReadIdx[i] = 0U;
          }
#endif /* (CAN_NUM_HRH > 0) */
#if (CAN_NUM_HTH > 0)
          for(i=0U; i<CAN_NUM_HTH; i++)
          {
            Can_TxBufStoreIdx[i] = 0U;
            Can_TxBufReadIdx[i] = 0U;
          }
#endif /* (CAN_NUM_HTH > 0) */
          Can_Controllers[Controller].ControllerState->State = CAN_CS_STOPPED;
          RetVal = CAN_OK;
        }
        break;
        /* CAN290, CAN267 */
      case CAN_T_SLEEP:
          Can_Controllers[Controller].ControllerState->State = CAN_CS_SLEEP;
          RetVal = CAN_OK;
          break;
      case CAN_T_WAKEUP:
          Can_Controllers[Controller].ControllerState->State = CAN_CS_STOPPED;
          RetVal = CAN_OK;
        break;
      default:
        /* Do nothing but return CAN_NOT_OK. */
        break;
    }

    CanIf_ControllerModeIndication( Controller,
        Can_CanIfControllerModeMap[ Can_Controllers[Controller].ControllerState->State ] );

  }

  DBG_CAN_SETCONTROLLERMODE_EXIT(RetVal, Controller, Transition);
  return RetVal;
}

/*------------------[Can_DisableControllerInterrupts()]---------------------*/

/* !LINKSTO CAN204, 1 */
FUNC(void, CAN_CODE) Can_DisableControllerInterrupts
(
  uint8 Controller
)
{
  DBG_CAN_DISABLECONTROLLERINTERRUPTS_ENTRY(Controller);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_READY)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_DISABLE_CONTROLLER_INTERRUPTS, CAN_E_UNINIT);
  }
  else if(Controller >= CAN_CONTROLLER_COUNT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_DISABLE_CONTROLLER_INTERRUPTS, CAN_E_PARAM_CONTROLLER);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {

#if ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT))
    SchM_Enter_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
    if(Can_Controllers[Controller].ControllerState->IntDisableNestingCounter == CAN_MAX_DIS_INT_DEPTH)
    {
      SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
    }
    else
    {
      /* Interrupts need to be disabled only at the first call to this
       * function (CAN049). */
      if(Can_Controllers[Controller].ControllerState->IntDisableNestingCounter == 0U)
      {
#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
        OS_WINDOWSClearInterrupt(Can_Rx_Interrupt);
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
#if (CAN_TX_MODE == CAN_MODE_INTERRUPT)
        OS_WINDOWSClearInterrupt(Can_Tx_Interrupt);
#endif /* (CAN_TX_MODE == CAN_MODE_INTERRUPT) */
        Can_Controllers[Controller].ControllerState->InterruptEnabled = FALSE;
      }
      Can_Controllers[Controller].ControllerState->IntDisableNestingCounter++;
      SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
    }
#endif /* ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT)) */
  }
  DBG_CAN_DISABLECONTROLLERINTERRUPTS_EXIT(Controller);
}

/*------------------[Can_EnableControllerInterrupts()]----------------------*/

/* !LINKSTO CAN204, 1 */
FUNC(void, CAN_CODE) Can_EnableControllerInterrupts
(
  uint8 Controller
)
{
  DBG_CAN_ENABLECONTROLLERINTERRUPTS_ENTRY(Controller);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_READY)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_ENABLE_CONTROLLER_INTERRUPTS, CAN_E_UNINIT);
  }
  else if(Controller >= CAN_CONTROLLER_COUNT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_ENABLE_CONTROLLER_INTERRUPTS, CAN_E_PARAM_CONTROLLER);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {

#if ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT))
    SchM_Enter_Can_SCHM_CAN_EXCLUSIVE_AREA_0();

    if(Can_Controllers[Controller].ControllerState->IntDisableNestingCounter > 0U)
    {
      Can_Controllers[Controller].ControllerState->IntDisableNestingCounter--;
    }

    if(Can_Controllers[Controller].ControllerState->IntDisableNestingCounter == 0U)
    {
      DBG_CAN_INTERRUPTENABLED(Can_InterruptEnabled, TRUE);
      Can_Controllers[Controller].ControllerState->InterruptEnabled = TRUE;
      /* If an interrupt has been requested while Controller interrupts were
       * disabled, trigger them now (will be executed after SchM_Exit below). */
#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
      if(Can_Controllers[Controller].ControllerState->RxInterruptPending == TRUE)
      {
        OS_WINDOWSTriggerInterrupt(Can_Rx_Interrupt);
      }
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
#if (CAN_TX_MODE == CAN_MODE_INTERRUPT)
      if(Can_Controllers[Controller].ControllerState->TxInterruptPending == TRUE)
      {
        OS_WINDOWSTriggerInterrupt(Can_Tx_Interrupt);
      }
#endif /* (CAN_TX_MODE == CAN_MODE_INTERRUPT) */
    }
    SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
#endif  /* ((CAN_RX_MODE == CAN_MODE_INTERRUPT) || (CAN_TX_MODE == CAN_MODE_INTERRUPT)) */
  }
  DBG_CAN_ENABLECONTROLLERINTERRUPTS_EXIT(Controller);
}

/*------------------[Can_CheckWakeup()]---------------------------------*/

FUNC(Can_ReturnType, CAN_CODE) Can_CheckWakeup
(
  uint8 Controller
)
{
  DBG_CAN_CHECKWAKEUP_ENTRY(Controller);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_READY)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_CBK_CHECK_WAKEUP, CAN_E_UNINIT);
  }
  else if(Controller >= CAN_CONTROLLER_COUNT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_CBK_CHECK_WAKEUP, CAN_E_PARAM_CONTROLLER);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    TS_PARAM_UNUSED(Controller);

    /* Do nothing. */
  }
  DBG_CAN_CHECKWAKEUP_EXIT(CAN_NOT_OK, Controller);
  return CAN_NOT_OK;
}

/*------------------[Can_Write()]-------------------------------------------*/

FUNC(Can_ReturnType, CAN_CODE) Can_Write
(
  uint8 Hth,
  P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_CONST) PduInfo
)
{
  Can_ReturnType RetVal = CAN_NOT_OK;

/* Only process the rest of the function if a transmit handle exists. */
#if (CAN_NUM_HTH > 0)
  VirtualBusDataType Can_VBusData;
  uint8 Can_VBusBuffer[CAN_SDU_LENGTH];
  uint8 Can_OldTxBufStoreIndex;
  uint8 i;

  DBG_CAN_WRITE_ENTRY(Hth, PduInfo);
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_READY)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_WRITE, CAN_E_UNINIT);
  }
  else if((Hth > (CAN_NUM_HRH + CAN_NUM_HTH)) ||
          (Can_HohConfig[Hth].CanObjectType != CAN_OBJECT_TYPE_TRANSMIT))
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_WRITE, CAN_E_PARAM_HANDLE);
  }
  /* Deviation MISRA-2 */
  else if(PduInfo == NULL_PTR)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_WRITE, CAN_E_PARAM_POINTER);
  }
  /* Deviation MISRA-2 */
  else if(PduInfo->sdu == NULL_PTR)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_WRITE, CAN_E_PARAM_POINTER);
  }
  else if(PduInfo->length > CAN_SDU_LENGTH)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
        CAN_API_WRITE, CAN_E_PARAM_DLC);
  }
  else
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    SchM_Enter_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
    if(Can_HTHLocked[Can_HohConfig[Hth].CanBufferIndex] == FALSE)
    {
      Can_HTHLocked[Can_HohConfig[Hth].CanBufferIndex] = TRUE;
      SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();

      Can_VBusData.nBusID = Can_HohConfig[Hth].CanControllerRef->ConrollerId;
      Can_VBusData.nChannel = Can_HohConfig[Hth].CanControllerRef->CanHwChannel;
      Can_VBusData.nID = PduInfo->id;
      Can_VBusData.nLength = PduInfo->length;

      /* Set the extended flag (needed by the SoftDevice). */
      if((PduInfo->id & CAN_EXT_MASK) != 0U)
      {
        Can_VBusData.nCycleExt = 1U;
      }
      else
      {
        Can_VBusData.nCycleExt = 0U;
      }

      for(i=0U; (i<PduInfo->length)&&(i<CAN_SDU_LENGTH); i++)
      {
        Can_VBusBuffer[i] = PduInfo->sdu[i];
      }

      Can_VBusData.pData = Can_VBusBuffer;

      /* Save index to current transmission buffer. */
      Can_OldTxBufStoreIndex = Can_TxBufStoreIdx[Can_HohConfig[Hth].CanBufferIndex];

      if(Can_StoreInTxBuffer(Can_HohConfig[Hth].CanBufferIndex, PduInfo->swPduHandle) ==
          CAN_BUF_OK)
      {
        if(Sd_SendData(&Can_VBusData) == SD_OK)
        {
#if (CAN_TX_MODE == CAN_MODE_INTERRUPT)
          Can_HohConfig[Hth].CanControllerRef->ControllerState->TxInterruptPending = TRUE;
          if(Can_HohConfig[Hth].CanControllerRef->ControllerState->InterruptEnabled == TRUE)
          {
            OS_WINDOWSTriggerInterrupt(Can_Tx_Interrupt);
          }
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
          RetVal = CAN_OK;
        }
        else
        {
          /* The frame could not be sent due to problems with the VirtualBus,
           * thus it has to be removed from the transmission buffer again. */
          Can_TxBufStoreIdx[Can_HohConfig[Hth].CanBufferIndex] = Can_OldTxBufStoreIndex;
        }
      }
      else
      {
        RetVal = CAN_BUSY;
      }
      Can_HTHLocked[Can_HohConfig[Hth].CanBufferIndex] = FALSE;
    }
    else
    {
      SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
      RetVal = CAN_BUSY;
    }
  }

/* If no transmit handle exists, call DET (if enabled) and then exit with CAN_NOT_OK. */
#else /* (CAN_NUM_HTH > 0) */

  DBG_CAN_WRITE_ENTRY(Hth, PduInfo);

  TS_PARAM_UNUSED(Hth);
  TS_PARAM_UNUSED(PduInfo);

#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
      CAN_API_WRITE, CAN_E_PARAM_HANDLE);
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */

#endif /* (CAN_NUM_HTH > 0) */

  DBG_CAN_WRITE_EXIT(RetVal, Hth, PduInfo);
  return RetVal;
}

/*------------------[Can_MainFunction_Write()]------------------------------*/

FUNC(void, CAN_CODE) Can_MainFunction_Write_Internal(void)
{
  DBG_CAN_MAINFUNCTION_WRITE_ENTRY();
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState == CAN_READY)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
#if ((CAN_TX_MODE == CAN_MODE_POLLING) && (CAN_NUM_HTH >0))
    Can_ProcessTxFrame();
#endif /* ((CAN_TX_MODE == CAN_MODE_POLLING) && (CAN_NUM_HTH > 0)) */
  }
  DBG_CAN_MAINFUNCTION_WRITE_EXIT();
}

/*------------------[Can_MainFunction_Read()]-------------------------------*/

FUNC(void, CAN_CODE) Can_MainFunction_Read_Internal(void)
{
  DBG_CAN_MAINFUNCTION_READ_ENTRY();
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState == CAN_READY)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
#if ((CAN_RX_MODE == CAN_MODE_POLLING) && (CAN_NUM_HRH > 0))
    Can_ProcessRxFrame();
#endif /* (CAN_RX_MODE == CAN_MODE_POLLING) */
  }
  DBG_CAN_MAINFUNCTION_READ_EXIT();
}

/*------------------[Can_MainFunction_BusOff()]-----------------------------*/

#if (CAN_MAIN_FUNCTION_BUSOFF == STD_ON)
FUNC(void, CAN_CODE) Can_MainFunction_BusOff(void)
{
  DBG_CAN_MAINFUNCTION_BUSOFF_ENTRY();
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState == CAN_READY)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    /* Do nothing. */
  }
  DBG_CAN_MAINFUNCTION_BUSOFF_EXIT();
}
#endif

/*------------------[Can_MainFunction_Mode()]-----------------------------*/

FUNC(void, CAN_CODE) Can_MainFunction_Mode(void)
{
  DBG_CAN_MAINFUNCTION_MODE_ENTRY();
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState != CAN_UNINIT)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    /* Do nothing */
  }
  DBG_CAN_MAINFUNCTION_MODE_EXIT();
}

/*------------------[Can_MainFunction_Wakeup()]-----------------------------*/

#if (CAN_MAIN_FUNCTION_WAKEUP == STD_ON)
FUNC(void, CAN_CODE) Can_MainFunction_Wakeup(void)
{
  DBG_CAN_MAINFUNCTION_WAKEUP_ENTRY();
#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  if(Can_InitState == CAN_READY)
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
  {
    /* Do nothing. */
  }
  DBG_CAN_MAINFUNCTION_WAKEUP_EXIT();
}
#endif

/*------------------[Can_ProcessTxFrame()]----------------------------------*/

#if (CAN_NUM_HTH > 0)
FUNC(void, CAN_CODE) Can_ProcessTxFrame(void)
{
  uint8     i;
  PduIdType PduId = 0U;
  DBG_CAN_PROCESSTXFRAME_ENTRY();
  for(i=0U; i<CAN_NUM_HTH; i++)
  {
    while(Can_ReadFromTxBuffer(i, &PduId) != CAN_BUF_EMPTY)
    {
      CanIf_TxConfirmation(PduId);
#if (CAN_TX_MODE == CAN_MODE_INTERRUPT)
      Can_HohConfig[i].CanControllerRef->ControllerState->TxInterruptPending = FALSE;
#endif /* (CAN_TX_MODE == CAN_MODE_INTERRUPT) */
    }
  }

  DBG_CAN_PROCESSTXFRAME_EXIT();
}
#endif /* (CAN_NUM_HTH > 0) */

/*------------------[Can_ProcessRxFrame()]----------------------------------*/

#if (CAN_NUM_HRH > 0)
FUNC(void, CAN_CODE) Can_ProcessRxFrame(void)
{
  P2VAR(Can_HwBufType, AUTOMATIC, CAN_APPL_DATA) Can_RxDataPtr;
  Can_IdType  CanId;
  uint8       Dlc;
  uint8       CanSdu[CAN_SDU_LENGTH];
  uint8       i, k;
  DBG_CAN_PROCESSRXFRAME_ENTRY();
  Can_EnterCriticalSection();
  for(k=0U; k<CAN_NUM_HRH; k++)
  {
    while(Can_ReadFromRxBuffer(k, &Can_RxDataPtr) != CAN_BUF_EMPTY)
    {
      CanId = Can_RxDataPtr->CanId;
      Dlc = Can_RxDataPtr->Dlc;
      for(i=0U; (i<Dlc)&&(i<CAN_SDU_LENGTH); i++)
      {
        CanSdu[i] = Can_RxDataPtr->Sdu[i];
      }
      CanIf_RxIndication(Can_HrhLookUpTable[k], CanId, Dlc, CanSdu);
#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
      Can_HohConfig[Can_HrhLookUpTable[k]].CanControllerRef
                      ->ControllerState->RxInterruptPending = FALSE;
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
#ifdef CUSTOM_CANLPDURECEIVECALLOUTFUNCTION
        CUSTOM_CANLPDURECEIVECALLOUTFUNCTION(Can_HrhLookUpTable[k], CanId, Dlc, CanSdu)
#endif
    }
  }

  DBG_CAN_PROCESSRXFRAME_EXIT();
}
#endif /* (CAN_NUM_HRH > 0) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>

/*==[Definition of functions with internal linkage]===========================*/
/* Deviation MISRA-1 */
#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

/*------------------[Can_VBusCallback()]------------------------------------*/

STATIC FUNC(void, CAN_CODE) Can_VBusCallback(VirtualBusDataType* Can_RawRxData)
{
#if (CAN_NUM_HRH > 0)
  uint8 i;
  Can_HwBufType RxData;
  Can_BufReturnType BufRet = CAN_BUF_EMPTY;
#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
  uint32 Controller = CAN_INVALID_CTRL;
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
  if(Can_RawRxData != NULL_PTR)
  {
    for(i=0U; i<CAN_NUM_HRH; i++)
    {
      if(( ( Can_RawRxData->nID & Can_HohConfig[Can_HrhLookUpTable[i]].CanIdMask ) ==
           ( Can_HohConfig[Can_HrhLookUpTable[i]].CanId &
                  Can_HohConfig[Can_HrhLookUpTable[i]].CanIdMask ) )
          &&
         /* Frame is STANDARD and HRH type is STANDARD or MIXED. */
        (((Can_RawRxData->nCycleExt == 0U) &&
          (Can_HohConfig[Can_HrhLookUpTable[i]].CanIdType != CAN_ID_TYPE_EXTENDED))
            ||
         /* Frame is EXTENDED and HRH type is EXTENDED or MIXED. */
         ((Can_RawRxData->nCycleExt != 0U) &&
          (Can_HohConfig[Can_HrhLookUpTable[i]].CanIdType != CAN_ID_TYPE_STANDARD)))
        &&
        (Can_RawRxData->nBusID ==
                Can_HohConfig[Can_HrhLookUpTable[i]].CanControllerRef->ConrollerId)
        )
      {
        RxData.CanId = Can_RawRxData->nID;
        RxData.Dlc = (uint8)Can_RawRxData->nLength;
        if (Can_RawRxData->nCycleExt == TRUE)
        {
          RxData.CanId  |= CAN_EXT_MASK;
        }
        /* CHWI TODO problem with critical section from rx thread
        Can_EnterCriticalSection();*/
        BufRet = Can_StoreInRxBuffer(
                   Can_HohConfig[Can_HrhLookUpTable[i]].CanBufferIndex,
                   &RxData, Can_RawRxData->pData);
        /* CHWI TODO  problem with critical section from rx thread
        Can_LeaveCriticalSection();*/

        if(BufRet == CAN_BUF_OK)
        {
#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
          Controller = Can_HohConfig[Can_HrhLookUpTable[i]].CanControllerRef->ConrollerId;
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
          /* We want to receive the frame only once, even if it matches
           * multiple HRHs. */
          break;
        }
      }
    }
  }

#if (CAN_RX_MODE == CAN_MODE_INTERRUPT)
  if((BufRet != CAN_BUF_EMPTY) && (Controller != CAN_INVALID_CTRL))
  {
    Can_Controllers[Controller].ControllerState->RxInterruptPending = TRUE;
    if(Can_Controllers[Controller].ControllerState->InterruptEnabled == TRUE)
    {
      OS_WINDOWSTriggerInterrupt(Can_Rx_Interrupt);
    }
  }
#endif /* (CAN_RX_MODE == CAN_MODE_INTERRUPT) */
#endif /* (CAN_NUM_HRH > 0) */
}

/*------------------[Can_StoreInRxBuffer()]-----------------------------------*/

#if (CAN_NUM_HRH > 0)
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_StoreInRxBuffer(
          uint8 index,
          P2VAR(Can_HwBufType, AUTOMATIC, CAN_VAR) frame,
          P2VAR(uint8, AUTOMATIC, CAN_VAR) sdu)
{
  uint8 i;
  Can_BufReturnType RetVal = CAN_BUF_FULL;
  DBG_CAN_STOREINRXBUFFER_ENTRY(index,frame,sdu);

#if (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_OLDEST)
  /* If the next index equals the read index, the buffer is full.
   * This will always waste one element of the buffer, but we run
   * under Windows and have plenty of RAM... ;-) */
  if(CAN_NEXT_IDX(Can_RxBufStoreIdx[index]) != Can_RxBufReadIdx[index])
#endif /* (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_OLDEST) */
  {
    Can_RxCircBuffer[index][Can_RxBufStoreIdx[index]].CanId = frame->CanId;
    Can_RxCircBuffer[index][Can_RxBufStoreIdx[index]].Dlc = frame->Dlc;

    for(i=0U; (i<frame->Dlc)&&(i<CAN_SDU_LENGTH) ; i++)
    {
      Can_RxCircBuffer[index][Can_RxBufStoreIdx[index]].Sdu[i] = sdu[i];
    }

    Can_RxBufStoreIdx[index] = CAN_NEXT_IDX(Can_RxBufStoreIdx[index]);

    RetVal = CAN_BUF_OK;
  }

  #if (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_OLDEST && CAN_DEV_ERROR_DETECTION == STD_ON)
  else
  {
    /* CHWI TODO problem with critical section from rx thread
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
    CAN_API_STORE_IN_RX_BUFFER, CAN_E_DATALOST);*/
  }
  #endif /* (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_OLDEST && CAN_DEV_ERROR_DETECTION == STD_ON) */

#if (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_NEWEST)
  if(Can_RxBufStoreIdx[index] == Can_RxBufReadIdx[index])
  {
    Can_RxBufReadIdx[index] = CAN_NEXT_IDX(Can_RxBufReadIdx[index]);
  }
  #if (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_OLDEST && CAN_DEV_ERROR_DETECTION == STD_ON)
  else
  {
    /* CHWI TODO problem with critical section from rx thread
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
    CAN_API_STORE_IN_RX_BUFFER, CAN_E_DATALOST);*/
  }
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */
#endif /* (CAN_BUFFER_MODE == CAN_BUFFER_KEEP_NEWEST) */
  DBG_CAN_STOREINRXBUFFER_EXIT(RetVal,index,frame,sdu);
  return RetVal;
}
#endif /* (CAN_NUM_HRH > 0) */

/*------------------[Can_StoreInTxBuffer()]-----------------------------------*/

#if (CAN_NUM_HTH > 0)
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_StoreInTxBuffer(
          uint8 index,
          PduIdType PduId)
{
  Can_BufReturnType RetVal = CAN_BUF_FULL;
  DBG_CAN_STOREINTXBUFFER_ENTRY(index,PduId);
  SchM_Enter_Can_SCHM_CAN_EXCLUSIVE_AREA_0();

  /* If the next index equals the read index, the buffer is full.
   * This will always waste one element of the buffer, but we run
   * under Windows and have plenty of RAM... ;-) */
  if(CAN_NEXT_IDX(Can_TxBufStoreIdx[index]) != Can_TxBufReadIdx[index])
  {
    Can_TxCircBuffer[index][Can_TxBufStoreIdx[index]] = PduId;

    Can_TxBufStoreIdx[index] = CAN_NEXT_IDX(Can_TxBufStoreIdx[index]);

    RetVal = CAN_BUF_OK;
  }

#if (CAN_DEV_ERROR_DETECTION == STD_ON)
  else
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID,
    CAN_API_STORE_IN_TX_BUFFER, CAN_E_DATALOST);
  }
#endif /* (CAN_DEV_ERROR_DETECTION == STD_ON) */


  SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
  DBG_CAN_STOREINTXBUFFER_EXIT(RetVal,index,PduId);
  return RetVal;
}
#endif /* (CAN_NUM_HTH > 0) */

/*------------------[Can_ReadFromRxBuffer()]-----------------------------------*/

#if (CAN_NUM_HRH > 0)
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_ReadFromRxBuffer(
    uint8 index,
    P2VAR(P2VAR(Can_HwBufType, AUTOMATIC, CAN_VAR), AUTOMATIC, CAN_VAR) frame)
{
  Can_BufReturnType RetVal = CAN_BUF_EMPTY;
  DBG_CAN_READFROMRXBUFFER_ENTRY(index,frame);

  /* If read and store index are equal, the buffer is empty. */
  if(Can_RxBufReadIdx[index] != Can_RxBufStoreIdx[index])
  {
    *frame=&Can_RxCircBuffer[index][Can_RxBufReadIdx[index]];

    Can_RxBufReadIdx[index] = CAN_NEXT_IDX(Can_RxBufReadIdx[index]);

    RetVal = CAN_BUF_OK;
  }
  DBG_CAN_READFROMRXBUFFER_EXIT(RetVal,index,frame);
  return RetVal;
}
#endif /* (CAN_NUM_HRH > 0) */

/*------------------[Can_ReadFromTxBuffer()]---------------------------------*/

#if (CAN_NUM_HTH > 0)
STATIC FUNC(Can_BufReturnType, CAN_CODE) Can_ReadFromTxBuffer(
    uint8 index,
    P2VAR(PduIdType, AUTOMATIC, CAN_VAR) PduId)
{
  Can_BufReturnType RetVal = CAN_BUF_EMPTY;
  DBG_CAN_READFROMTXBUFFER_ENTRY(index,PduId);
  SchM_Enter_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
  /* If read and store index are equal, the buffer is empty. */
  if(Can_TxBufReadIdx[index] != Can_TxBufStoreIdx[index])
  {
    *PduId=Can_TxCircBuffer[index][Can_TxBufReadIdx[index]];

    Can_TxBufReadIdx[index] = CAN_NEXT_IDX(Can_TxBufReadIdx[index]);

    RetVal = CAN_BUF_OK;
  }
  SchM_Exit_Can_SCHM_CAN_EXCLUSIVE_AREA_0();
  DBG_CAN_READFROMTXBUFFER_EXIT(RetVal,index,PduId);
  return RetVal;
}
#endif /* (CAN_NUM_HTH > 0) */

/* Deviation MISRA-1 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>
