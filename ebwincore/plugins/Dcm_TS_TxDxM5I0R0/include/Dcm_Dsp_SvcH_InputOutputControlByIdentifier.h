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

/* !LINKSTO Dcm.Dsn.File.IOCBI.PublicApi,1 */
/* This file contains the public API of the Diagnostic Service InputOutputControlByIdentifier */

#ifndef DCM_DSP_SVCH_INPUTOUTPUTCONTROLBYIDENTIFIER_H
#define DCM_DSP_SVCH_INPUTOUTPUTCONTROLBYIDENTIFIER_H

/*==================[inclusions]=================================================================*/

#include <Dcm.h>
#include <Dcm_Int.h>
#include <Dcm_Cfg.h>                                            /* Dcm configuration header file */

#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)

#include <Dcm_Dsp_DidServices.h>

/*==================[macros]=====================================================================*/

#if (defined DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER)
#error DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER already defined
#endif /* if (defined DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER) */

/** \brief Internal API service ID for Dcm_Dsp_InputOutputControlByIdentifier_SvcH.
 **
 ** Definition of DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER.
 */
#define DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER 0xA5U

#if (defined DCM_SVCID_RETURNCONTROLTOECU)
#error DCM_SVCID_RETURNCONTROLTOECU already defined
#endif /* if (defined DCM_SVCID_RETURNCONTROLTOECU) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_RETURNCONTROLTOECU.
 */
#define DCM_SVCID_RETURNCONTROLTOECU 0x39U

#if (defined DCM_SVCID_RESETTODEFAULT)
#error DCM_SVCID_RESETTODEFAULT already defined
#endif /* if (defined DCM_SVCID_RESETTODEFAULT) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_RESETTODEFAULT.
 */
#define DCM_SVCID_RESETTODEFAULT 0x3CU

#if (defined DCM_SVCID_FREEZECURRENTSTATE)
#error DCM_SVCID_FREEZECURRENTSTATE already defined
#endif /* if (defined DCM_SVCID_FREEZECURRENTSTATE) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_FREEZECURRENTSTATE.
 */
#define DCM_SVCID_FREEZECURRENTSTATE 0x3AU

#if (defined DCM_SVCID_SHORTTERMADJUSTMENT)
#error DCM_SVCID_SHORTTERMADJUSTMENT already defined
#endif /* if (defined DCM_SVCID_SHORTTERMADJUSTMENT) */

/** \brief Dsp interface API service ID.
 **
 ** Definition of DCM_SVCID_SHORTTERMADJUSTMENT.
 */
#define DCM_SVCID_SHORTTERMADJUSTMENT 0x3DU

/*==================[type definitions]===========================================================*/

/** \brief  This type contains the different stages of returning control of signal to Ecu
 *  */
typedef uint8 Dcm_DidReturnControlToEcuStatusType;

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief  This type contains the different stages of returning control of signal to Ecu
 *          Possible values: DCM_RET_CTRL_DEFAULT_INACTIVE
 *                           DCM_RET_CTRL_DEFAULT_FINISHED
 *                           DCM_RET_CTRL_DEFAULT_PENDING */
typedef uint8 Dcm_DidReturnControlToEcuStatusDefaultType;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSync) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IO Control
      (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateAsync) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSync) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSync) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Async operation)
    and fixed length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeAsync) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Async operation)
    and variable length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeAsync) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSync) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to reset an IOControl to default value
     (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultAsync) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSync) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
        (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuAsync) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief  This type holds the context information of an IOControl request
 */
typedef struct
{
  /** \brief Contains the DcmDspData configuration */
  Dcm_DidDataType dataCfg;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  /** \brief Contains the DcmDspDidControlMask mask value for MaskSize 3 and 4 */
  uint32 DcmDspDidControlMaskUint32;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  /** \brief Index of DID Signal currently being processed */
  uint16 SignalInd;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  /** \brief Contains the DcmDspDidControlMask mask value for MaskSize 2 */
  uint16 DcmDspDidControlMaskUint16;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  /** \brief The DID index currently being processed */
  Dcm_DidConfigIdxType DidConfigIdx;
  /** \brief Specifies if the signal is being processed for the first time */
  boolean InitSignalProcessing;
    /** \brief Describes how the server shall control its inputs or outputs */
  uint8 IOControlParameter;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  /** \brief Contains the DcmDspDidControlMask mask value for MaskSize 1 */
  uint8 DcmDspDidControlMaskUint8;
  /** \brief Contains the DcmDspDidControlMaskType (DCM_CONTROLMASK_EXTERNAL or DCM_CONTROLMASK_NOT_DEFINED) */
  uint8 DcmDspDidControlMask;
 /** \brief Contains the DcmDspDidControlMaskSize (1,2,3,4) */
  uint8 DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  /** \brief Status of the currently processed IOControlByIdentifier request */
  Dcm_DidRequestProcessingStType RequestProcessingSt;
}
Dcm_IOControlDIDContextType;

/** \brief  This type holds the context information of an IOControl ReturnControlToEcu subfunction
 */
typedef struct
{
  /** \brief Contains the DcmDspData configuration */
  Dcm_DidDataType dataCfg;

  /** \brief Index of DID Signal currently being returning control to ECU */
  uint16 returnToEcuSigInd;

  /** \brief The Did index in the table Dcm_DidConfig */
  uint16 configIdx;

  /** \brief Processing time allowed for returncontroltoecu completion */
  uint16 timeoutCount;

  /** \brief Status of return control of signal to ECU */
  Dcm_DidReturnControlToEcuStatusType returnCtrlToECUStatus;

  /** \brief Indicates if a call is the first or a consecutive attempt to return control to ECU
   *         for the current signal */
  boolean InitSignalProcessing;
}
Dcm_IOReturnControlToEcuStatusType;

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief  This type holds the status information of an IOControl ReturnControlToEcu subfunction.
 *          DataType active when the DcmDspDidEnableDefaultInterfaces configuration parameter is set to TRUE.
 */
typedef struct
{
  /** \brief Indicates if a call is the first or a consecutive attempt to return control to ECU
   *         for the unconfigured DIDs. */
  boolean IsDefaultDidReadReturnPending;

  /** \brief Status of the currently processed Did. Set to TRUE if the Did is unconfigured. Set to FALSE otherwise. */
  boolean IsDidUnconfigured;

  /** \brief Status of ReturnControlToEcuForAllSignals for the unconfigured DIDs.
   *         This global variable holds the status of the ReturnControlToECUForAllSignals for unconfigured DIDs.
   *         When the function Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals() is called,
   *         the first step is to handle the configured DIDs. After all of the configured DIDs are handled, the control for
   *         unconfigured DIDs is returned to ECU.
   *         When no action is needed, the status is set to DCM_RET_CTRL_DEFAULT_INACTIVE.
   *         When the control for unconfigured DIDs is not returned to ECU in a single cycle, the status is
   *         set to DCM_RET_CTRL_DEFAULT_PENDING and the Dcm is calling the function to ReturnControlToECUForAllSignals for
   *         unconfigured DIDs again at later point in time as long as DCM_PENDING is returned. The callout for configured DIDs
   *         is not called when the unconfigured DIDs are returning DCM_PENDING.
   *         When the control to ECU for all unconfigured DIDs is performed, the status is set to DCM_RET_CTRL_DEFAULT_FINISHED
   *         (in this moment the timer "DcmDspReturnControlToEcuTimeout" is reseted by calling the initialization function
   *         Dcm_Dsp_InputOutputControlByIdentifier_Init()). */
  Dcm_DidReturnControlToEcuStatusDefaultType ReturnControlToECUForAllSignalsStatus;
}
Dcm_DefaultInterfacesStatusType;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service 0x2F
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in]    opState Operation state of the function.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
  );

/** \brief Initialize the internal data of the InputOutputControlByIdentifier service
 **
 ** \param[in]    NONE
 **
 ** \return       NONE
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_Init(void);

/** \brief Get the timeout counter value for ReturnControlToEcu
 **
 ** \param[in]    NONE
 **
 ** \return       Value of the timeout counter
 */
extern FUNC(uint16, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuTimeoutCount(void);

/** \brief Get the status of ReturnControlToEcu
 **
 ** \param[in]    NONE
 **
 ** \return       Status of return control to ECU funtionality
 */
extern FUNC(Dcm_DidReturnControlToEcuStatusType, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus(void);

/** \brief Set the status of ReturnControlToEcu
 **
 ** \param[in]    ReturnCtrlToECUStatus   The status of return control to ECU
 **
 ** \return       NONE
 */
extern FUNC(void, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus(
    Dcm_DidReturnControlToEcuStatusType ReturnCtrlToECUStatus
    );

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief Set the status of ReturnControlToEcuForAllSignals for the unconfigured DIDs.
 **
 ** \param[in]    ReturnCtrlToECUStatus The status of ReturnControlToEcuForAllSignals for the unconfigured DIDs.
 **
 ** \return       NONE
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus(
  uint8 ReturnCtrlToECUStatus);

/** \brief Get the status of ReturnControlToEcuForAllSignals for the unconfigured DIDs.
 **
 ** \param[in]    NONE
 **
 ** \return       The status of ReturnControlToEcuForAllSignals for the unconfigured DIDs.
 */
extern FUNC(uint8, DCM_CODE)
 Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus(void);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
/** \brief This function locks the DidServices on session transition to default session and
 **        triggers Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals()
 **        to be called at the next Dcm_MainFunction() cycle.
 **
 ** \param[in]    NONE
 ** \return       NONE
 */
extern FUNC(void, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignalsTryLock(void);

/** \brief This function locks the DidServices on session transition to default session and
 **        stops all the control in progress.
 **
 ** \param[inout] NONE
 ** \param[in]    NONE
 */
extern FUNC(void, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals(void);

/** \brief This function performs required actions upon session change.
 **
 **  \param[in] OldSession          Session level before change
 **  \param[in] NewSession          Session level after change
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

#endif /* (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if(DCM_DSP_USE_SERVICE_0X2F == STD_ON) */

#endif /* ifndef DCM_DSP_SVCH_INPUTOUTPUTCONTROLBYIDENTIFIER_H */
/*==================[end of file]================================================================*/
