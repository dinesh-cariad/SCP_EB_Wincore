/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.CommunicationControl.Impl,1 */
/* This file contains the implementation of the Diagnostic Service CommunicationControl */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  */
/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */
/*===============================[includes]======================================================*/

#include <Dcm_Dsp_SvcH_CommunicationControl.h>
#include <Dcm_Trace.h>
#include <TSAutosar.h>
#include <TSMem.h>

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)
/*===========================[macro definitions]=================================================*/
#if (defined DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID)  /* To prevent double declaration */
#error DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID already defined
#endif /* if (defined DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID) */

/** \brief Invalid value for the requested communication mode */
#define DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID                  (0xFFU)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X00)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X00 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X00) */

/** \brief Macro for CommunicationControl sub-service Id for EnableRxAndTx */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X00                (0x00U)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X01)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X01 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X01) */

/** \brief Macro for CommunicationControl sub-service Id for EnableRxAndDisableTx */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X01                (0x01U)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X02)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X02 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X02) */

/** \brief Macro for CommunicationControl sub-service Id for DisableRxAndEnableTx */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X02                (0x02U)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X03)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X03 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X03) */

/** \brief Macro for CommunicationControl sub-service Id for DisableRxAndTx */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X03                (0x03U)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X04)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X04 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X04) */

/** \brief Macro for CommunicationControl sub-service Id for
 **        EnableRxAndDisableTxWithEnhancedAddressInformation
 */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X04                (0x04U)

#if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X05)  /* To prevent double declaration */
#error DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X05 already defined
#endif /* if (defined DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X05) */

/** \brief Macro for CommunicationControl sub-service Id for
 **        EnableRxAndTxWithEnhancedAddressInformation
 */
#define DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X05                (0x05U)

/* Macro for CommunicationControl service ID */
#if (defined DCM_DSP_SERVICE_0X28)
  #error "DCM_DSP_SERVICE_0X28 is already defined"
#endif
#define DCM_DSP_SERVICE_0X28                (0x28U)

/*==================[type definitions]===========================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X03_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>
/** \brief Array that hold all communication modes defined by DCM.
 **        This array is used to determine the requested communication mode.
 ** On line is the message type and on the column is the sub-service.
 */
/* Deviation MISRAC2012-1 */
STATIC CONST(Dcm_CommunicationModeType, DCM_CONST) Dcm_AllCommunicationMode[4U][4U] =
  {{DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID, DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID,
             DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID,DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID},
   {DCM_ENABLE_RX_TX_NORM, DCM_ENABLE_RX_DISABLE_TX_NORM,
                                             DCM_DISABLE_RX_ENABLE_TX_NORM, DCM_DISABLE_RX_TX_NORM},
   {DCM_ENABLE_RX_TX_NM, DCM_ENABLE_RX_DISABLE_TX_NM,
                                                 DCM_DISABLE_RX_ENABLE_TX_NM, DCM_DISABLE_RX_TX_NM},
   {DCM_ENABLE_RX_TX_NORM_NM, DCM_ENABLE_RX_DISABLE_TX_NORM_NM,
                                      DCM_DISABLE_RX_ENABLE_TX_NORM_NM, DCM_DISABLE_RX_TX_NORM_NM}};

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
/** \brief Array that hold all communication modes define by RTE and may have different values
 **        than the communication modes define by DCM.
 **        This array is used to determine the requested RTE communication mode.
 ** On line is the message type and on the column is the sub-service.
 */
/* Deviation MISRAC2012-1 */
STATIC CONST(Dcm_CommunicationModeType, DCM_CONST)
                                              Dcm_AllCommunicationControlModeDeclarations[4U][4U] =
  {{DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID, DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID,
    DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID, DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID},
   {RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NORM,
    RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NORM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NORM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NORM},
   {RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NM,
    RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NM},
   {RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NORM_NM,
    RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_DISABLE_TX_NORM_NM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_ENABLE_TX_NORM_NM,
    RTE_MODE_DcmCommunicationControl_DCM_DISABLE_RX_TX_NORM_NM}};
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

#endif /* #if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
              (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
              (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
              (DCM_0X28_0X03_SSVC_ENABLED == STD_ON )) */

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Array holding the current mode of the communication channels.
 **        This array is used to determine if a reset of a channel is needed in case of
 **        communication control mode rule fail or a session change. Always when the communication
 **        mode is changed(0x28 request or communication control mode rule fail or session change)
 **        for a channel from Dcm_CommunicationControlLookUpTable[] the corresponding index from
 **        this array is updated with the changed communication mode.
 ** The index of elements will be equal with the index of Dcm_CommunicationControlLookUpTable[]
 ** elements.
 */
STATIC VAR(Dcm_CommunicationModeType, DCM_VAR)
                   Dcm_CommunicationControl_CommunicationMode[DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief This global variable hold the requested communication mode based on
 **        Dcm_AllCommunicationMode[] to can be used in the call of BswM API on invocation of
 **        the sent confirmation.
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_CommunicationModeType, DCM_VAR) Dcm_CommunicationControl_RequestedModeConfirmation;

/** \brief This global variable hold the requested communication mode based on
 **        Dcm_AllCommunicationControlModeDeclarations[] to can be used in the call of
 **        SchMSwitchCommunicationControl on invocation of the sent confirmation.
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_CommunicationModeType, DCM_VAR)
                                     Dcm_CommunicationControl_RequestedModeDeclarationConfirmation;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This global variable hold if is a positive or negative response to determine if the
 **        ModeSwitch and BswMCurrentState functions need to be called on invocation of the sent
 **        confirmation
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(boolean, DCM_VAR) Dcm_CommunicationControl_PositiveResponse = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_16
#include <Dcm_MemMap.h>

#if((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))
/** \brief This global variable hold the index of requested  SubNode channel from
 **        Dcm_CommunicationControlSubNodeChannel[] to can be used in the call of
 **        SchMSwitchCommunicationControl and setting the communication mode on invocation of the
 **        sent confirmation, to not search again the index in
 **        Dcm_CommunicationControlSubNodeChannel[].
 ** Note: Initialized with 65535, because the DcmDspComControlSubNodeId has the 1 to 65535 range
 **       this means that are 65534 maximum elements configured also maximum number of elements
 **       for Dcm_CommunicationControlSubNodeChannel is 65534, so 65535 is an invalid value.
 */
STATIC VAR(uint16, DCM_VAR) Dcm_CommunicationControl_SubNodeChannelIndex = DCM_UINT16_MAX;
#endif /* #if((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || (DCM_0X28_0X05_SSVC_ENABLED == STD_ON)) */

#define DCM_STOP_SEC_VAR_INIT_16
#include <Dcm_MemMap.h>

#else /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief This global variable hold the communication mode that was confirm by BswM API.
 **        According with that:
 **          - if BswM does not confirm the communication mode change the service
 **            is call again with pending
 **          - if BswM confirm the mode change than is compare with
 **            Dcm_CommunicationControl_RequestedCommunicationMode and determine if is a positive
 **            or negative response
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_CommunicationModeType, DCM_VAR) Dcm_CommunicationControl_ConfirmedCommunicationMode;

/** \brief This global variable hold the requested communication mode based on
 **        Dcm_AllCommunicationMode[] to be compare with the confirmation mode from BswM API, to
 **        can determine if is a positive or a negative response.
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(Dcm_CommunicationModeType, DCM_VAR) Dcm_CommunicationControl_RequestedCommunicationMode;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This global variable hold the type of the requested sub-service of
 **        CommunicationControl(0x28).
 **        This variable is used to determine if on invocation of the sent confirmation, the call
 **        to the internal sub-service is needed.
 **
 ** TRUE  - The requested CommunicationControl(0x28) sub-service is handled internally(by DCM).
 ** FALSE - The requested CommunicationControl(0x28) sub-service is handled externally
 **         (by application).
 */
STATIC VAR(boolean, DCM_VAR) Dcm_CommunicationControl_SubFunctionHandlerIsInternal = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief This global variable hold the sub-service handler, extracted from
 **        CommunicationControl(0x28) request in the DCM_INITIAL state.
 **        This variable is used in each call of the CommunicationControl handler to call the
 **        sub-service handler whether is external or internal.
 */
STATIC VAR(Dcm_GenericServiceHandlerType, DCM_VAR) Dcm_CommunicationControl_SubServiceHandler;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*=======================[internal function declarations]========================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x28 internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal DCM Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x28 internal service handler
 **
 ** \param[in]    OpStatus Operation state for internal DCM Service function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X03_SSVC_ENABLED == STD_ON ))

#if(DCM_MODEDECLARATION_SUPPORT == STD_OFF)
/** \brief Internal handler to get network id from SubnetNumber, to be used for calling the BswM API
 **        and to set the communication mode when mode declaration is disable.
 **
 ** \param[in] SubnetNumber SubnetNumber extracted from request.
 ** \param[in] DcmRxPduId   RxPduId extracted from pMsgContext.
 **
 ** \retval The network identifier for the current protocol
 ** \retval The SubnetNumber parameter from the CommunicationControl request
 */
STATIC FUNC(NetworkHandleType, DCM_CODE)
                                     Dcm_Dsp_CommunicationControl_GetNetworkId_ModeDeclarationFalse(
  uint8 SubnetNumber,
  PduIdType DcmRxPduId);
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

/** \brief This function check if the subnet number and communication type are supported
 **
 ** \param[in] MessageType  CommunicationType parameter from request.
 ** \param[in] SubnetNumber SubnetNumber parameter from request.
 **
 ** \retval TRUE  - The subnet number and communication type are supported
 ** \retval FALSE - The subnet number or communication type are not supported
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_CommunicationControl_CheckSubnetNumberAndCommType(
  uint8 MessageType,
  uint8 SubnetNumber);

/** \brief This function is the commune handler for 0x00, 0x01, 0x02, 0x03 sub-service.
 **
 ** \param[in]    OpStatus    Operation state for internal DCM Service function.
 ** \param[in]    subService  The sub-service ID use to determine the right mode.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE      Operation completed
 ** \retval DCM_E_PENDING   Operation did not complete, it must be called again just if mode
 **                         declaration support is disabled
 */
STATIC FUNC(Std_ReturnType, DCM_CODE)
                                    Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
  Dcm_OpStatusType OpStatus,
  uint8 subService,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
/** \brief This function set the communication mode on invocation of the sent confirmation.
 **
 ** \param[in] SubnetNumber SubnetNumber extracted from request.
 ** \param[in] DcmRxPduId   RxPduId extracted from pMsgContext.
 **
 ** \retval void
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SetComMode_ModeDeclarationTrue(
  uint8 SubnetNumber,
  PduIdType DcmRxPduId);
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#endif /*#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X03_SSVC_ENABLED == STD_ON )) */

#if((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
    ((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
     (DCM_0X28_0X05_SSVC_ENABLED == STD_ON)))

/** \brief This function is the commune handler for 0x04, 0x05 sub-service.
 **
 ** \param[in]    OpStatus    Operation state for internal DCM Service function.
 ** \param[in]    SubService  The sub-service ID use to determine the right mode.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE      Operation completed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_CommonEnhancedAddressInformation(
  Dcm_OpStatusType OpStatus,
  uint8 SubService,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);


/** \brief This function check if the subnode number and communication type are supported
 **
 ** \param[in] MessageType             CommunicationType parameter from request.
 ** \param[in] SubNodeId               Sub-node ID parameter from request.
 ** \param[out] SubNodeChannelIndex    The coresponding index in the
 **                                    Dcm_CommunicationControlSubNodeChannel[] for requested
 **                                    sub-node ID.
 **
 ** \retval TRUE  - The subnode number and communication type are supported
 ** \retval FALSE - The subnode number or communication type are not supported
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_CommunicationControl_CheckSubNodeIdAndCommType(
  uint8 MessageType,
  uint16 SubNodeId,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) SubNodeChannelIndex);

#endif /* #if((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
              ((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
              (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))) */

#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X03_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))

/** \brief This function reset all the internal variables that are used in the CommunicationControl.
 **
 ** \retval void
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ClearInternalVariables(void);

#endif

/** \brief This function performs requested actions upon session change.
 **
 ** \param[in] NewSession     Session level after change.
 **
 ** \retval void
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ResetOnSessionChange(Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

/*=======================[external function definitions]=========================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm511,1 */
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_CommunicationControl_SvcStart(DCM_INITIAL, pMsgContext);
      break;

    /* Consecutive call after the function retuned PENDING before */
    case DCM_PENDING:
      ServiceProcessStatus = Dcm_Dsp_CommunicationControl_SvcCont(DCM_PENDING, pMsgContext);
      break;

    /* cancel any ongoing service processing */
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_CommunicationControl_SvcCont(DCM_CANCEL, pMsgContext);
      break;

    case DCM_CONFIRMED_OK:
    case DCM_CONFIRMED_NOK:
      if(Dcm_CommunicationControl_SubFunctionHandlerIsInternal == TRUE)
      {
        /* Call sub-service handler */
        ServiceProcessStatus = Dcm_Dsp_CommunicationControl_SvcCont(OpStatus, pMsgContext);
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch mode */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      ServiceProcessStatus = DCM_E_DONE;
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_Init(void)
{
  DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_ENTRY();

  /* Initializes the mode of all ComM Channels that DCM uses */
  TS_MemSet(Dcm_CommunicationControl_CommunicationMode,DCM_ENABLE_RX_TX_NORM_NM,DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE);

  /* Initializes the global variable that 0x28 uses */
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Dcm_CommunicationControl_RequestedModeConfirmation = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_RequestedModeDeclarationConfirmation =
                                                         DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_PositiveResponse = FALSE;

#if((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))
 /* Initialized with 65535, because the DcmDspComControlSubNodeId has the 1 to 65535 range
  * this means that are 65534 maximum elements configured also maximum number of elements
  * for Dcm_CommunicationControlSubNodeChannel is 65534, so 65535 is an invalid value.
  */
  Dcm_CommunicationControl_SubNodeChannelIndex = DCM_UINT16_MAX;
#endif

  TS_PARAM_UNUSED(Dcm_CommunicationControl_RequestedModeConfirmation);
  TS_PARAM_UNUSED(Dcm_CommunicationControl_RequestedModeDeclarationConfirmation);
  TS_PARAM_UNUSED(Dcm_CommunicationControl_PositiveResponse);
#else

  Dcm_CommunicationControl_ConfirmedCommunicationMode = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_RequestedCommunicationMode = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;

  TS_PARAM_UNUSED(Dcm_CommunicationControl_ConfirmedCommunicationMode);
  TS_PARAM_UNUSED(Dcm_CommunicationControl_RequestedCommunicationMode);
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

  Dcm_CommunicationControl_SubFunctionHandlerIsInternal = FALSE;
  Dcm_CommunicationControl_SubServiceHandler = NULL_PTR;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_EXIT();
}

#if (DCM_0X28_0X00_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_EnableRxAndTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_ENTRY(OpStatus,pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X00, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_EXIT(ServiceProcessStatus,OpStatus,
                                                            pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X00_SSVC_ENABLED == STD_ON) */

#if (DCM_0X28_0X01_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_EnableRxAndDisableTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_ENTRY(OpStatus,pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X01, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_EXIT(ServiceProcessStatus,OpStatus,
                                                            pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) */

#if (DCM_0X28_0X02_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_DisableRxAndEnableTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_ENTRY(OpStatus,pMsgContext);
  TS_PARAM_UNUSED(OpStatus);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X02, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_EXIT(ServiceProcessStatus,OpStatus,
                                                            pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) */

#if (DCM_0X28_0X03_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_DisableRxAndTx_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_ENTRY(OpStatus,pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X03, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_EXIT(ServiceProcessStatus,OpStatus,
                                                            pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X03_SSVC_ENABLED == STD_ON) */

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)

#if (DCM_0X28_0X04_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE)
 Dcm_Dsp_CommunicationControl_EnableRxAndDisableTxWithEnhancedAddrInfo_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_ENTRY(
                                                                              OpStatus,pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonEnhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X04, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_EXIT(
                                                        ServiceProcessStatus,OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X04_SSVC_ENABLED == STD_ON) */

#if (DCM_0X28_0X05_SSVC_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_EnableRxAndTxWithEnhancedAddrInfo_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_ENTRY(
                                                                              OpStatus,pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_CommonEnhancedAddressInformation(
                               OpStatus, DCM_DSP_COMMUNICATIONCONTROL_SUBSERVICE_0X05, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_EXIT(
                                                        ServiceProcessStatus,OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /* (DCM_0X28_0X05_SSVC_ENABLED == STD_ON) */

#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.ResetCommunicationControl,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ResetCommunicationControl(void)
{
  uint8_least ModeAndLookUpIndex = 0U;

  /* Deviation TASKING-1 <+2> */
  for (ModeAndLookUpIndex = 0U;
       ModeAndLookUpIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE;
       ModeAndLookUpIndex++)
  {
    if (Dcm_CommunicationControl_CommunicationMode[ModeAndLookUpIndex] != DCM_ENABLE_RX_TX_NORM_NM)
    {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
      /* !LINKSTO Dcm.CommunicationControl.ModeRuleFails.ModeSwitch,1 */
      /* !LINKSTO Dcm.CommunicationControl.DefaultSession.ModeSwitch,1 */
      /* !LINKSTO Dcm.CommunicationControl.SessionChange.ModeSwitch,1 */
      (void)Dcm_CommunicationControlLookUpTable[ModeAndLookUpIndex].SchMSwitchCommunicationControl
        (RTE_MODE_DcmCommunicationControl_DCM_ENABLE_RX_TX_NORM_NM);

#if (DCM_BSWM_API_ENABLED == STD_ON)
      /* !LINKSTO Dcm.CommunicationControl.ModeRuleFails.BswMCurrentState,1 */
      /* !LINKSTO Dcm.CommunicationControl.DefaultSession.BswMCurrentState,1 */
      /* !LINKSTO Dcm.CommunicationControl.SessionChange.BswMCurrentState,1 */
      /* Invoke the BswM interface to notify the Communication Mode change */
      BswM_Dcm_CommunicationMode_CurrentState(
                              Dcm_CommunicationControlLookUpTable[ModeAndLookUpIndex].ComMChannelId,
                              DCM_ENABLE_RX_TX_NORM_NM);
#endif

#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#if (DCM_BSWM_API_ENABLED == STD_ON)
      /* !LINKSTO Dcm.EB.CommunicationControl.DefaultSession.DcmModeDeclarationSupportFalse,1 */
      /* !LINKSTO Dcm.EB.CommunicationControl.SessionChange.DcmModeDeclarationSupportFalse,1 */
      /* Invoke the BswM interface to request the Communication Mode change */
      BswM_Dcm_RequestCommunicationMode(
                 Dcm_CommunicationControlLookUpTable[ModeAndLookUpIndex], DCM_ENABLE_RX_TX_NORM_NM);
#endif
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

      Dcm_CommunicationControl_CommunicationMode[ModeAndLookUpIndex] = DCM_ENABLE_RX_TX_NORM_NM;
    }
  }
}

#if (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.ProcessModeRules,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ProcessModeRules(void)
{
  uint8 Nrc = DCM_E_GENERALREJECT;
  DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_ENTRY();

  DCM_PRECONDITION_ASSERT((Dcm_Dsp_CommunicationControl_ModeRuleFunction != NULL_PTR),
            DCM_SVCID_PROCESSMODERULES);

  /* Re-enable communication in case that the referenced mode rule is not fulfilled any more. */
  Dcm_Dsp_CommunicationControl_ModeRuleFunction(&Nrc);

  if (Nrc != DCM_E_OK)
  {
    /* !LINKSTO Dcm.CommunicationControl.ModeRuleFails.ModeSwitch,1 */
    /* !LINKSTO Dcm.CommunicationControl.ModeRuleFails.BswMCurrentState,1 */
    Dcm_Dsp_CommunicationControl_ResetCommunicationControl();
  }
  DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_EXIT();
}
#endif /* #if (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON) */

#if((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SetConfirmedCommunicationMode,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(
  Dcm_CommunicationModeType CommunicationMode)
{
  DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_ENTRY(CommunicationMode);

  Dcm_CommunicationControl_ConfirmedCommunicationMode = CommunicationMode;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_EXIT();
}
#endif /* #if((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON)) */

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SessionChange,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  TS_PARAM_UNUSED(OldSession);

  if (DCM_DEFAULT_SESSION == NewSession)
  {
    /* set communication mode to DCM_ENABLE_RX_TX_NORM_NM */
    /* !LINKSTO Dcm.EB.CommunicationControl.DefaultSession.DcmModeDeclarationSupportFalse,1 */
    /* !LINKSTO Dcm.CommunicationControl.DefaultSession.ModeSwitch,1 */
    /* !LINKSTO Dcm.CommunicationControl.DefaultSession.BswMCurrentState,1 */
    Dcm_Dsp_CommunicationControl_ResetCommunicationControl();
  }
  else
  {
    (void)Dcm_Dsp_CommunicationControl_ResetOnSessionChange(NewSession);
  }

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_EXIT();

}

/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_ENTRY(pMsgContext);

  Dcm_CommunicationControl_SubFunctionHandlerIsInternal = FALSE;

  Dcm_CommunicationControl_SubServiceHandler =
                                            Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

  ServiceProcessStatus = Dcm_Dsp_CommunicationControl_SvcCont(OpStatus, pMsgContext);

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_EXIT(ServiceProcessStatus,pMsgContext);

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SvcCont,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_ENTRY(pMsgContext);

  DCM_PRECONDITION_ASSERT(Dcm_CommunicationControl_SubServiceHandler !=
                                                                    NULL_PTR, DCM_INTERNAL_API_ID);

  /* Call sub-service handler */
  ServiceProcessStatus = Dcm_CommunicationControl_SubServiceHandler(OpStatus, pMsgContext);

  /* Sub-service handlers return E_OK and E_NOT_OK, so must be converted to DCM_E_DONE */
  if((Dcm_CommunicationControl_SubFunctionHandlerIsInternal == FALSE) &&
     ((ServiceProcessStatus == E_OK) || (ServiceProcessStatus == E_NOT_OK)))
  {
    ServiceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_EXIT(ServiceProcessStatus,pMsgContext);

  return ServiceProcessStatus;
}

#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X03_SSVC_ENABLED == STD_ON ))
#if(DCM_MODEDECLARATION_SUPPORT == STD_OFF)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.GetNetworkId.ModeDeclarationFalse,1 */
STATIC FUNC(NetworkHandleType, DCM_CODE)
                                    Dcm_Dsp_CommunicationControl_GetNetworkId_ModeDeclarationFalse(
  uint8 SubnetNumber,
  PduIdType DcmRxPduId)
{
  NetworkHandleType NetworkId;
  /* variable to get the channel id */
  Dcm_RxPduIdInfoType RxPduIdInfo;

  if(SubnetNumber == 0x0FU)
  {
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(DcmRxPduId, &RxPduIdInfo);
    /* Get the network identifier */
    NetworkId = RxPduIdInfo.ComMChannelId;
  }
  else
  {
    /* If the subnet parameter is different from 0x0F :'NetworkId' is the
       copy of the SubnetNumber parameter from the CommunicationControl request */
    NetworkId = SubnetNumber;
  }

  return NetworkId;
}
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.CheckSubnetNumberAndCommType,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_CommunicationControl_CheckSubnetNumberAndCommType(
  uint8 MessageType,
  uint8 SubnetNumber)
{
  boolean ValidSubNetAndCommType = FALSE;
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
#if(DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0)
  uint8_least SpecificChannelIndex = 0;
#endif /* #if(DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0) */
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

#if(DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  TS_PARAM_UNUSED(SubnetNumber);
#endif

  /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTx.WrongCommmunicType.0x31,1 */
  /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTx.WrongCommmunicType.0x31,1 */
  /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndEnableTx.WrongCommmunicType.0x31,1 */
  /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndTx.WrongCommmunicType.0x31,1 */
  /* If bit 0 and bit 1 from communicationType parameter are different than 0 then:
   * - If mode declaration support is enable: continue to check if SubnetNumber is valid and return
   *   ValidSubNetAndCommType
   * - If mode declaration support is disable: set the ValidSubNetAndCommType flag to TRUE.
   */
  if(MessageType != 0U)
  {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    /* !LINKSTO Dcm.CommunicationControl.EnableRxAndTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
    /* !LINKSTO Dcm.CommunicationControl.EnableRxAndDisableTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
    /* !LINKSTO Dcm.CommunicationControl.DisableRxAndEnableTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
    /* !LINKSTO Dcm.CommunicationControl.DisableRxAndTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
    if((SubnetNumber > 0x00U) && (SubnetNumber < 0x0FU))
    {
#if(DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0)
      /* Loop through all CommunicationControl Specific channels */
      for(SpecificChannelIndex = 0U;
          SpecificChannelIndex < DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL;
          SpecificChannelIndex++)
      {
        if(SubnetNumber ==
                         Dcm_CommunicationControlSpecificChannel[SpecificChannelIndex].SubnetNumber)
        {
          ValidSubNetAndCommType = TRUE;
        }
      }
#endif
    }
    else
    {
      /* Subnet number 0x00 is always valid. */
      ValidSubNetAndCommType = TRUE;
    }
#else
    /* Any networkID extracted from request is valid even if does not have any reference
     * in DCM configuration. */
    ValidSubNetAndCommType = TRUE;
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */
  }

  return ValidSubNetAndCommType;
}

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.CommonUnenhancedAddressInformation,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE)
                                    Dcm_Dsp_CommunicationControl_CommonUnenhancedAddressInformation(
  Dcm_OpStatusType OpStatus,
  uint8 subService,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  /* Get the message type from request data */
  const uint8 MessageType = pMsgContext->reqData[0U] & DCM_MESSAGE_TYPE_MASK;
  /* Get the subnet number from request data */
  const uint8 SubnetNumber = (pMsgContext->reqData[0U] & DCM_SUBNET_NUMBER_MASK) >> 4U;
#if(DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  NetworkHandleType NetworkId;
  /* Get the subnet number from request data */
#endif

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      /* Check if the message has 1 bytes, otherwise set a NRC */
      if (pMsgContext->reqDataLen == 1U)
      {
        boolean ValidSubNetAndCommType = FALSE;

        /* Check the SubnetNumber and CommunicationType are valid according with ISO */
        ValidSubNetAndCommType =
                 Dcm_Dsp_CommunicationControl_CheckSubnetNumberAndCommType(
                                                                         MessageType, SubnetNumber);

        /* If SubnetNumber and CommunicationType are valid call the sub-service,
         * otherwise set the NRC */
        if(ValidSubNetAndCommType == TRUE)
        {
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
          /* Set the positive response variable to TRUE in order to be called the
           * Dcm_Dsp_CommunicationControl_SetComMode_ModeDeclarationTrue on invocation of the sent
           * confirmation */
          Dcm_CommunicationControl_PositiveResponse = TRUE;

          /* Set the requested mode variable in order to be used in
          * Dcm_Dsp_CommunicationControl_SetComMode_ModeDeclarationTrue on invocation of the sent
          * confirmation */
          Dcm_CommunicationControl_RequestedModeConfirmation =
                                                  Dcm_AllCommunicationMode[MessageType][subService];
          Dcm_CommunicationControl_RequestedModeDeclarationConfirmation =
                               Dcm_AllCommunicationControlModeDeclarations[MessageType][subService];

          /* Assemble the positive response */
          pMsgContext->resDataLen = 1U;
          pMsgContext->resData[0U] = subService;

          ServiceProcessStatus = DCM_E_DONE;
#else /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

          /* Set the requested communication mode to be compared with
          * Dcm_CommunicationControl_RequestedCommunicationMode */
          Dcm_CommunicationControl_RequestedCommunicationMode =
                                                  Dcm_AllCommunicationMode[MessageType][subService];

#if(DCM_BSWM_API_ENABLED == STD_ON)
          NetworkId = Dcm_Dsp_CommunicationControl_GetNetworkId_ModeDeclarationFalse(
                                                             SubnetNumber, pMsgContext->dcmRxPduId);
     /* !LINKSTO Dcm.EB.BswM.CommunicationControlSpecificChannel.DcmModeDeclarationSupportFalse,1 */
     /* !LINKSTO Dcm.EB.BswM.CommunicationControlRxComMChannel.DcmModeDeclarationSupportFalse,1 */
     /* Invoke the BswM interface to request the Communication Mode change */

          /* !LINKSTO Dcm.EB.UDSServices.BswM.ResetToDefaultSession.DiscardControlType,1 */
          if( FALSE == Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(DCM_SID_COMMUNICATIONCONTROL))
          {
            BswM_Dcm_RequestCommunicationMode(
                                      NetworkId, Dcm_CommunicationControl_RequestedCommunicationMode);
          }
#endif

         /* Return pending in order to compare the requested mode with the confirm mode from BswM */
          ServiceProcessStatus = DCM_E_PENDING;
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */
        }
        else
        {
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
          /* Set the variable that hold if is a positive response to FALSE */
          Dcm_CommunicationControl_PositiveResponse = FALSE;
#endif
      /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTx.WrongCommmunicType.0x31,1 */
      /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTx.WrongCommmunicType.0x31,1 */
      /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndEnableTx.WrongCommmunicType.0x31,1 */
      /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndTx.WrongCommmunicType.0x31,1 */
      /* !LINKSTO Dcm.CommunicationControl.EnableRxAndTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
      /* !LINKSTO Dcm.CommunicationControl.EnableRxAndDisableTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
      /* !LINKSTO Dcm.CommunicationControl.DisableRxAndEnableTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
      /* !LINKSTO Dcm.CommunicationControl.DisableRxAndTx.SubnetMismatch0x01_0x0E.NRC0x31,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          ServiceProcessStatus = DCM_E_DONE;
        }
      }
      else
      {
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
        /* Set the variable that hold if is a positive response to FALSE */
        Dcm_CommunicationControl_PositiveResponse = FALSE;
#endif
        /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTx.RequestLength.0x13,1 */
        /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTx.RequestLength.0x13,1 */
        /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndEnableTx.RequestLength.0x13,1 */
        /* !LINKSTO Dcm.EB.CommunicationControl.DisableRxAndTx.RequestLength.0x13,1 */
        /* Set the negative response if the request length is different from 2 byte */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        ServiceProcessStatus = DCM_E_DONE;
      }
      /* Set SubFunctionHandlerIsInternal to true because is an internal sub-service and need to be
       * called on the invocation of the confirmation */
      Dcm_CommunicationControl_SubFunctionHandlerIsInternal = TRUE;
    }
    break;

#if(DCM_MODEDECLARATION_SUPPORT == STD_OFF)
    case DCM_PENDING:
    {
      uint8_least ModeIndex = 0U;

      /* !LINKSTO Dcm.EB.UDSServices.BswM.ResetToDefaultSession.DiscardControlType,1 */
      if(TRUE == Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(DCM_SID_COMMUNICATIONCONTROL))
      {
        /* Assembly the positive response */
        /* !LINKSTO Dcm.EB.CommunicationModeTransition.Completion,1 */
        pMsgContext->resDataLen = 1U;
        pMsgContext->resData[0U] = subService;

        /* Reset the mode transition parameters */
        Dcm_CommunicationControl_ConfirmedCommunicationMode =
                                                       DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
        Dcm_CommunicationControl_RequestedCommunicationMode =
                                                       DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
        ServiceProcessStatus = DCM_E_DONE;
      }
      /* Check if a mode transition confirmation is received from BswM */
      else if (Dcm_CommunicationControl_ConfirmedCommunicationMode !=
                                                         DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID)
      {
        /* Check if the mode transition request is accepted by BswM. ie requested communication
          mode is same as the communication mode entered */
        if (Dcm_CommunicationControl_ConfirmedCommunicationMode ==
                                                Dcm_CommunicationControl_RequestedCommunicationMode)
        {
          NetworkId = Dcm_Dsp_CommunicationControl_GetNetworkId_ModeDeclarationFalse(
                                                             SubnetNumber, pMsgContext->dcmRxPduId);

          /* Deviation TASKING-1 */
          for (ModeIndex = 0U; ModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE; ModeIndex++)
          {
            if (Dcm_CommunicationControlLookUpTable[ModeIndex] == NetworkId)
            {
              /* !LINKSTO Dcm.EB.CommunicationModeTransition.InternallyMode,1 */
              Dcm_CommunicationControl_CommunicationMode[ModeIndex] =
                                                Dcm_CommunicationControl_RequestedCommunicationMode;
              break;
            }
          }
          /* Assembly the positive response */
          /* !LINKSTO Dcm.EB.CommunicationModeTransition.Completion,1 */
          pMsgContext->resDataLen = 1U;
          pMsgContext->resData[0U] = subService;

          /* Reset the mode transition parameters */
          Dcm_CommunicationControl_ConfirmedCommunicationMode =
                                                         DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
          Dcm_CommunicationControl_RequestedCommunicationMode =
                                                         DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
        }
        else
        {
          /* !LINKSTO Dcm.EB.CommunicationMode.NotAccepted.NRC0x22,1 */
          /* mode transition request not accepted. Set negative response */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        }
        ServiceProcessStatus = DCM_E_DONE;
      }
      else
      {
        ServiceProcessStatus = DCM_E_PENDING;
      }
    }
    break;
#endif
    case DCM_CONFIRMED_OK:
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
      if(Dcm_CommunicationControl_PositiveResponse == TRUE)
      {
        /* !LINKSTO Dcm.EB.UDSServices.ModeSwitch.ResetToDefaultSession.DiscardControlType,1 */
        if(FALSE == Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(DCM_SID_COMMUNICATIONCONTROL))
        {
          /* Set the communication mode on invocation of the sent confirmation
           * in case of a positive response */
          Dcm_Dsp_CommunicationControl_SetComMode_ModeDeclarationTrue(
                                                               SubnetNumber, pMsgContext->dcmRxPduId);
        }
      }
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */
      Dcm_Dsp_CommunicationControl_ClearInternalVariables();
      ServiceProcessStatus = DCM_E_DONE;
      break;

    default:
      /* In case of a CANCEL or a CONFIRMED_NOK clear the local variables and return DCM_E_DONE */
      Dcm_Dsp_CommunicationControl_ClearInternalVariables();
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  return ServiceProcessStatus;
}

#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.SetComMode.ModeDeclarationTrue,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_SetComMode_ModeDeclarationTrue(
  uint8 SubnetNumber,
  PduIdType DcmRxPduId)
{
  NetworkHandleType NetworkId;
  uint8_least LookUpAndModeIndex = 0U;
  boolean EntryFoundInLookUpTable = FALSE;
#if((DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0U) || \
    (DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL > 0U))
  uint8 SpecificAndAllChIndex = 0U;
#endif

  /* Variable to get the channel id */
  Dcm_RxPduIdInfoType RxPduIdInfo;

  if (SubnetNumber == 0x0FU)
  {
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(DcmRxPduId, &RxPduIdInfo);

    /* Get the network identifier */
    NetworkId = RxPduIdInfo.ComMChannelId;

    for (LookUpAndModeIndex = 0U;
          ((LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) && (FALSE == EntryFoundInLookUpTable));
          LookUpAndModeIndex++)
    {
      if (Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].ComMChannelId ==
            NetworkId)
      {
        /* !LINKSTO Dcm.CommunicationControl.Subnet0x0F.ModeSwitch,2 */
        (void)Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].SchMSwitchCommunicationControl(
          Dcm_CommunicationControl_RequestedModeDeclarationConfirmation);

#if (DCM_BSWM_API_ENABLED == STD_ON)
        /* !LINKSTO Dcm.CommunicationControl.Subnet0x0F.BswMCurrentState,2 */
        /* Invoke the BswM interface to notify the Communication Mode change */
        BswM_Dcm_CommunicationMode_CurrentState(
                                         NetworkId, Dcm_CommunicationControl_RequestedModeConfirmation);
#endif  /* #if (DCM_BSWM_API_ENABLED == STD_ON) */

        /* Set the requested communication mode in the Dcm_CommunicationControl_CommunicationMode array
         *  that have the same index as Dcm_CommunicationControlLookUpTable array */
        Dcm_CommunicationControl_CommunicationMode[LookUpAndModeIndex] =
                                                     Dcm_CommunicationControl_RequestedModeConfirmation;

        /* We found the entry in the communication control lookup table and we do not need to
         * continue further.*/
        EntryFoundInLookUpTable = TRUE;
      }
    }
    /* It is expected that the channel is always found in the CommunicationControlLookUpTable.*/
    DCM_INVARIANT_ASSERT(LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE, DCM_INTERNAL_API_ID);
  }
  else if((SubnetNumber > 0x00U) && (SubnetNumber < 0x0FU))
  {
#if (DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0U)
    /* Loop through all CommunicationControl Specific channels */
    for(SpecificAndAllChIndex = 0U;
        SpecificAndAllChIndex < DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL;
        SpecificAndAllChIndex++)
    {
      if (SubnetNumber ==
                        Dcm_CommunicationControlSpecificChannel[SpecificAndAllChIndex].SubnetNumber)
      {
        EntryFoundInLookUpTable = FALSE;
        for (LookUpAndModeIndex = 0U;
              ((LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) && (FALSE == EntryFoundInLookUpTable));
              LookUpAndModeIndex++)
        {
          if (Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].ComMChannelId ==
                Dcm_CommunicationControlSpecificChannel[SpecificAndAllChIndex].ComMChannelId)
          {
            /* !LINKSTO Dcm.CommunicationControl.SubnetMatch0x01_0x0E.ModeSwitch,2 */
            (void)Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].
                                            SchMSwitchCommunicationControl(
                                            Dcm_CommunicationControl_RequestedModeDeclarationConfirmation);
#if (DCM_BSWM_API_ENABLED == STD_ON)
            /* !LINKSTO Dcm.CommunicationControl.SubnetMatch0x01_0x0E.BswMCurrentState,2 */
            BswM_Dcm_CommunicationMode_CurrentState(
                           Dcm_CommunicationControlSpecificChannel[SpecificAndAllChIndex].ComMChannelId,
                           Dcm_CommunicationControl_RequestedModeConfirmation);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */

            /* Set the requested communication mode in the Dcm_CommunicationControl_CommunicationMode
             *  array that have the same index as Dcm_CommunicationControlLookUpTable array */
            Dcm_CommunicationControl_CommunicationMode[LookUpAndModeIndex] =
                                                     Dcm_CommunicationControl_RequestedModeConfirmation;

            /* We found the entry in the communication control lookup table and we do not need to
             * continue further.*/
             EntryFoundInLookUpTable = TRUE;
          }
        }
        /* It is expected that the channel is always found in the CommunicationControlLookUpTable.*/
        DCM_INVARIANT_ASSERT(LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE, DCM_INTERNAL_API_ID);
      }
    }
#endif /* #if (DCM_NUM_COMMUNICATIONCONTROL_SPECIFIC_CHANNEL > 0U) */
  }
  else /* SubnetNumber = 0x00U */
  {
#if (DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL > 0U)
    /* Loop through all CommunicationControl channels */
    /* Deviation TASKING-1 <+2> */
    for(SpecificAndAllChIndex = 0U;
        SpecificAndAllChIndex < DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL;
        SpecificAndAllChIndex++)
    {
      EntryFoundInLookUpTable = FALSE;
      for (LookUpAndModeIndex = 0U;
            ((LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) && (FALSE == EntryFoundInLookUpTable));
            LookUpAndModeIndex++)
      {

       if (Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].ComMChannelId ==
            Dcm_CommunicationControlAllChannel[SpecificAndAllChIndex])
        {
          /* !LINKSTO Dcm.CommunicationControl.Subnet0x00.ModeSwitch,2 */
          (void)Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].SchMSwitchCommunicationControl(
                                         Dcm_CommunicationControl_RequestedModeDeclarationConfirmation);
#if (DCM_BSWM_API_ENABLED == STD_ON)
          /* !LINKSTO Dcm.CommunicationControl.Subnet0x00.BswMCurrentState,2 */
          /* Invoke the BswM interface to request the Communication Mode change */
          BswM_Dcm_CommunicationMode_CurrentState(
                                              Dcm_CommunicationControlAllChannel[SpecificAndAllChIndex],
                                              Dcm_CommunicationControl_RequestedModeConfirmation);
#endif  /* #if (DCM_BSWM_API_ENABLED == STD_ON) */

          /* Set the requested communication mode in the Dcm_CommunicationControl_CommunicationMode
          *  array that have the same index as Dcm_CommunicationControlLookUpTable array */
          Dcm_CommunicationControl_CommunicationMode[LookUpAndModeIndex] =
                                                     Dcm_CommunicationControl_RequestedModeConfirmation;

          /* We found the entry in the communication control lookup table and we do not need to
           * continue further.*/
          EntryFoundInLookUpTable = TRUE;
        }
      }
      /* It is expected that the channel is always found in the CommunicationControlLookUpTable.*/
      DCM_INVARIANT_ASSERT(LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE, DCM_INTERNAL_API_ID);
    }
#endif /* #if (DCM_NUM_COMMUNICATIONCONTROL_ALL_CHANNEL > 0U) */
  }
}
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */
#endif /*#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X03_SSVC_ENABLED == STD_ON ))*/

#if((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
    ((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
     (DCM_0X28_0X05_SSVC_ENABLED == STD_ON)))

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.CheckSubNodeIdAndCommType,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_CommunicationControl_CheckSubNodeIdAndCommType(
  uint8 MessageType,
  uint16 SubNodeId,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) SubNodeChannelIndex)
{
  boolean ValidSubNodeIdAndCommType = FALSE;
  uint16 SubNodeChannelIterator = 0U;

  /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTxWithEnhancedAddInfo.WrongCommmunicType.0x31,1 */
  /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTxWithEnhancedAddInfo.WrongCommmunicType.0x31,1 */
  /* If bit 0 and bit 1 from communicationType parameter are different than 0 then continue to
   * check if the requested SubNodeId is configured.
   */
  if(MessageType != 0U)
  {
    /* !LINKSTO SWS_Dcm_01078,1 */
    /* !LINKSTO SWS_Dcm_01080,1 */
    /* Loop through all CommunicationControl SubNode channels */
    for(SubNodeChannelIterator = 0U;
        SubNodeChannelIterator < DCM_NUM_COMMUNICATIONCONTROL_SUBNODE_CHANNEL;
        SubNodeChannelIterator++)
    {
      if(SubNodeId == Dcm_CommunicationControlSubNodeChannel[SubNodeChannelIterator].SubNodeId)
      {
        ValidSubNodeIdAndCommType = TRUE;
        *SubNodeChannelIndex = SubNodeChannelIterator;
        break;
      }
    }
  }

  return ValidSubNodeIdAndCommType;
}

/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.CommonEnhancedAddressInformation,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_CommonEnhancedAddressInformation(
  Dcm_OpStatusType OpStatus,
  uint8 SubService,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      /* Check if the message has 3 bytes, otherwise set a NRC */
      if (pMsgContext->reqDataLen == 3U)
      {
        boolean ValidSubNodeAndCommType = FALSE;
        /* Get the message type from request data */
        const uint8 MessageType = pMsgContext->reqData[0U] & DCM_MESSAGE_TYPE_MASK;
        /* Calculate the requested SubNode ID */
        const uint8 HighByte = (uint8)pMsgContext->reqData[1U];
        const uint8 LowByte = (uint8)pMsgContext->reqData[2U];
        const uint16 requestedSubNodeId =(uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte,
                                                                                           LowByte);
        /* Initialized with 65535, because the DcmDspComControlSubNodeId has the 1 to 65535 range
         * this means that are 65534 maximum elements configured also maximum number of elements
         * for Dcm_CommunicationControlSubNodeChannel is 65534, so 65535 is an invalid value. */
        uint16 SubNodeChannelIndex = DCM_UINT16_MAX;
        /* Check the SubnetNumber and CommunicationType are valid according with ISO */
        ValidSubNodeAndCommType =
                 Dcm_Dsp_CommunicationControl_CheckSubNodeIdAndCommType(
                                             MessageType, requestedSubNodeId, &SubNodeChannelIndex);

        /* If SubnetNumber and CommunicationType are valid call the sub-service,
         * otherwise set the NRC */
        if(ValidSubNodeAndCommType == TRUE)
        {
          /* Get the index column for Dcm_AllCommunicationMode and
           * Dcm_AllCommunicationControlModeDeclarations according to requested sub-service:
           *  - if sub-service is 0x04(0100) bit 0 is not set => DCM_IS_BIT_SET = 0 =>
           *    ColumnIndex = 1 that means that DCM_ENABLE_RX_DISABLE_TX_NORM/
           *    DCM_ENABLE_RX_DISABLE_TX_NM/DCM_ENABLE_RX_DISABLE_TX_NORM_NM communication modes are
           *    used according with message type.
           *  - if sub-service is 0x05(0101) bit 0 is set => DCM_IS_BIT_SET = 1 =>
           *    ColumnIndex = 0 that means that DCM_ENABLE_RX_TX_NORM/DCM_ENABLE_RX_TX_NM/
           *    DCM_ENABLE_RX_TX_NORM_NM communication modes are used according with message type.
           */
           uint8 ColumnIndex;

          /* The sub node ID exist in Dcm_CommunicationControlSubNodeChannel[] because
           * ValidSubNodeAndCommType is TRUE, that means that SubNodeChannelIndex is different
           * then 65535
           */
          DCM_PRECONDITION_ASSERT(SubNodeChannelIndex != DCM_UINT16_MAX, DCM_INTERNAL_API_ID);

          if (DCM_UINT8_IS_BIT_SET(SubService, 0U))
          {
            ColumnIndex = 0U;
          }
          else
          {
            ColumnIndex = 1U;
          }

          /* Save the sub node index in order to be used on invocation of the sent confirmation to
           * determine the SchMSwitchCommunicationControl function and setting the communication
           * mode
           */
          Dcm_CommunicationControl_SubNodeChannelIndex = SubNodeChannelIndex;

          /* Set the positive response variable to TRUE in order to be changed the communication
           * mode on invocation of the sent confirmation
           */
          Dcm_CommunicationControl_PositiveResponse = TRUE;

          /* Set the requested mode variable in order to be used in communication mode change
           * on invocation of the sent confirmation
           */
          Dcm_CommunicationControl_RequestedModeConfirmation =
                                                 Dcm_AllCommunicationMode[MessageType][ColumnIndex];
          Dcm_CommunicationControl_RequestedModeDeclarationConfirmation =
                              Dcm_AllCommunicationControlModeDeclarations[MessageType][ColumnIndex];

          /* Assemble the positive response */
          pMsgContext->resDataLen = 1U;
          pMsgContext->resData[0U] = SubService;

          ServiceProcessStatus = DCM_E_DONE;
        }
        else
        {
          /* Set the variable that hold if is a positive response to FALSE */
          Dcm_CommunicationControl_PositiveResponse = FALSE;

          /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTxWithEnhancedAddInfo.WrongCommmunicType.0x31,1 */
          /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTxWithEnhancedAddInfo.WrongCommmunicType.0x31,1 */
          /* !LINKSTO SWS_Dcm_01078,1 */
          /* !LINKSTO SWS_Dcm_01080,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          ServiceProcessStatus = DCM_E_DONE;
        }
      }
      else
      {
        /* Set the variable that hold if is a positive response to FALSE */
        Dcm_CommunicationControl_PositiveResponse = FALSE;

        /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndDisableTxWithEnhancedAddInfo.RequestLength.0x13,1 */
        /* !LINKSTO Dcm.EB.CommunicationControl.EnableRxAndTxWithEnhancedAddInfo.RequestLength.0x13,1 */
        /* Set the negative response if the request length is different from 5 byte */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        ServiceProcessStatus = DCM_E_DONE;
      }
      /* Set SubFunctionHandlerIsInternal to true because is an internal sub-service and need to be
       * called on the invocation of the confirmation */
      Dcm_CommunicationControl_SubFunctionHandlerIsInternal = TRUE;
    }
    break;

    case DCM_CONFIRMED_OK:
    {
      uint8_least LookUpAndModeIndex = 0U;
      if(Dcm_CommunicationControl_PositiveResponse == TRUE)
      {
        /* Loop through all CommunicationControl Specific channels */
        LookUpAndModeIndex = 0U;

        while((LookUpAndModeIndex < DCM_COMMUNICATIONCONTROL_LOOKUP_SIZE) &&
              (Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].ComMChannelId !=
               Dcm_CommunicationControlSubNodeChannel[Dcm_CommunicationControl_SubNodeChannelIndex]
                                                                                    .ComMChannelId))
        {
          LookUpAndModeIndex++;
        }
        /* !LINKSTO Dcm.EB.UDSServices.ModeSwitch.ResetToDefaultSession.DiscardControlType,1 */
        if(FALSE == Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(DCM_SID_COMMUNICATIONCONTROL))
        {
          /* !LINKSTO Dcm.CommunicationControl.EnableRxAndDisableTxWithEnhancedAddInfo.ModeSwitch,1 */
          /* !LINKSTO Dcm.CommunicationControl.EnableRxAndTxWithEnhancedAddInfo.ModeSwitch,1 */
          (void)Dcm_CommunicationControlLookUpTable[LookUpAndModeIndex].
                                      SchMSwitchCommunicationControl(
                                      Dcm_CommunicationControl_RequestedModeDeclarationConfirmation);
#if (DCM_BSWM_API_ENABLED == STD_ON)
          /* !LINKSTO Dcm.CommunicationControl.EnableRxAndDisableTxWithEnhancedAddInfo.BswMCurrentState,1 */
          /* !LINKSTO Dcm.CommunicationControl.EnableRxAndTxWithEnhancedAddInfo.BswMCurrentState,1 */
          BswM_Dcm_CommunicationMode_CurrentState(
                        Dcm_CommunicationControlSubNodeChannel
                                         [Dcm_CommunicationControl_SubNodeChannelIndex].ComMChannelId,
                        Dcm_CommunicationControl_RequestedModeConfirmation);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */

          /* Set the requested communication mode in the Dcm_CommunicationControl_CommunicationMode
           *  array that have the same index as Dcm_CommunicationControlLookUpTable array */
          Dcm_CommunicationControl_CommunicationMode[LookUpAndModeIndex] =
                                                   Dcm_CommunicationControl_RequestedModeConfirmation;
        }
      }

      Dcm_Dsp_CommunicationControl_ClearInternalVariables();
      ServiceProcessStatus = DCM_E_DONE;
      break;
    }

    default:
      /* In case of a CANCEL or a CONFIRMED_NOK clear the local variables and return DCM_E_DONE */
      Dcm_Dsp_CommunicationControl_ClearInternalVariables();
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  return ServiceProcessStatus;
}

#endif /* #if((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
              ((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
               (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))) */

#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X03_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
    (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.CommunicationControl.ClearInternalVariables,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ClearInternalVariables(void)
{
#if(DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Dcm_CommunicationControl_RequestedModeConfirmation = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_RequestedModeDeclarationConfirmation =
                                                         DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_PositiveResponse = FALSE;
#if((DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || (DCM_0X28_0X05_SSVC_ENABLED == STD_ON))
 /* Initialized with 65535, because the DcmDspComControlSubNodeId has the 1 to 65535 range
  * this means that are 65534 maximum elements configured also maximum number of elements
  * for Dcm_CommunicationControlSubNodeChannel is 65534, so 65535 is an invalid value.
  */
  Dcm_CommunicationControl_SubNodeChannelIndex = DCM_UINT16_MAX;
#endif
#else
  Dcm_CommunicationControl_ConfirmedCommunicationMode = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
  Dcm_CommunicationControl_RequestedCommunicationMode = DCM_0X28_COMMUNICATIONCONTROL_MODE_INVALID;
#endif /* #if(DCM_MODEDECLARATION_SUPPORT == STD_ON) */

  Dcm_CommunicationControl_SubFunctionHandlerIsInternal = FALSE;
  Dcm_CommunicationControl_SubServiceHandler = NULL_PTR;
}
#endif /*#if((DCM_0X28_0X00_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X01_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X02_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X03_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X04_SSVC_ENABLED == STD_ON) || \
             (DCM_0X28_0X05_SSVC_ENABLED == STD_ON)) */

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_CommunicationControl_ResetOnSessionChange(Dcm_SesCtrlType NewSession)
{
  uint8_least sessionIdx;
  boolean ResetCommunicationControl = TRUE;
  boolean CommunicationControlConfigExist = FALSE;
  /* get configuration entry of 0x28 service Id */
  P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) SidConfig = NULL_PTR;
  Dcm_ProtocolIndexType ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

  DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_ENTRY(ProtocolIdx, NewSession);

  /* This function is called with parameter NewSession which is always different from default session.
   * Variable ProtocolIdx must be one valid protocol index since only UDS service DiagnosticSessionContol
   * can request switch to non-default session */
  DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);

  CommunicationControlConfigExist =
                      Dcm_DspInternal_GetSidConfig(ProtocolIdx, DCM_DSP_SERVICE_0X28, &SidConfig);

  /* If the CommunicationControl(0x28) service is available in the current protocol then
   * check if service request is allowed in current session. Otherwise do not reset the
   * communicationControl state. Is not need to search for all protocols to see if we have
   * CommunicationControl (0x28)service configured or not to reset the communication state,
   * because an preemption will be made and a reset to default session will be executed and the
   * communication state will be reset. */
  if(CommunicationControlConfigExist == TRUE)
  {
    /* If Service CommunicationControl(0x28) is not provided in the new session,
     * the flag ResetCommunicationControl will remain TRUE otherwise FALSE */
    DCM_PRECONDITION_ASSERT(DCM_DEFAULT_SESSION != NewSession, DCM_INTERNAL_API_ID);

    if(SidConfig->NumSesCtrls > 0U)
    {
      /* check if CommunicationControl service 0x28 is supported in the requested session */
      for (sessionIdx = 0U; sessionIdx < SidConfig->NumSesCtrls; ++sessionIdx)
      {
        if ((SidConfig->SesCtrls[sessionIdx] == DCM_ALL_SESSION_LEVEL) ||
            (SidConfig->SesCtrls[sessionIdx] == NewSession))
        {
          ResetCommunicationControl = FALSE;
          break;
        }
      }
    }
    else
    {
      ResetCommunicationControl = FALSE;
    }

    /* set communication mode to DCM_ENABLE_RX_TX_NORM_NM if CommunicationControl service 0x28 is
     * not supported in the new session */
    /* !LINKSTO Dcm.CommunicationControl.SessionChange.BswMCurrentState,1 */
    /* !LINKSTO Dcm.CommunicationControl.SessionChange.ModeSwitch,1 */
    /* !LINKSTO Dcm.EB.CommunicationControl.SessionChange.DcmModeDeclarationSupportFalse,1 */
    if (ResetCommunicationControl == TRUE)
    {
      (void)Dcm_Dsp_CommunicationControl_ResetCommunicationControl();
    }
  }

  DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X28 == STD_ON) */
/*==================[end of file]================================================================*/
