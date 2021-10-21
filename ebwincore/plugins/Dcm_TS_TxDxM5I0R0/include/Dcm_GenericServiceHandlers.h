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
/* Here is a collection of all implemented/supported AUTOSAR DCM Generic Service Handler         */

#ifndef DCM_GENERIC_SERVICE_HANDLERS_H
#define DCM_GENERIC_SERVICE_HANDLERS_H


/*==================[inclusions]=================================================================*/
#include <Dcm_Cfg.h>

/** \brief The generic service handler shall support the following parameters and return values code.
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X02 == STD_ON) )
#include <Dcm_Dsp_PidServices.h>

/** \brief The generic service handler for OBD service RequestCurrentPowertrainDiagnosticData (0x01)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
  */

/** \brief The generic service handler for OBD service RequestPowertrainFreezeFrameData (0x02)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X10 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X11 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X14 == STD_ON) )
#include <Dcm_Int.h>

/** \brief The generic service handler for OBD service RequestConfirmedDTC (0x03)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestConfirmedDTC_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief The generic service handler for OBD service ClearEmissionDiagInfo (0x04)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearEmissionDiagInfo_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief The generic service handler for OBD service RequestPendingDTC (0x07)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPendingDTC_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief The generic service handler for OBD service RequestPermanentDTC (0x0A)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPermanentDTC_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief Generic subservice handler for UDS service DiagnosticSessionControl (0x10)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief Generic subservice handler for UDS service ECUReset (0x11)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief Generic subservice handler for UDS service ClearDiagnosticInformation (0x14)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearDiagnosticInformation_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

#endif

#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestOnboardMonitoringTestResults.h>

/** \brief The generic service handler for OBD service RequestOnBoardMonitoringTestResults (0x06)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestOnBoardMonitoringTestResults_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X08 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestControlOfOnBoardSysTstComp.h>

/** \brief The generic service handler for OBD service RequestControlOfOnBoardSysTstComp (0x08)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestControlOfOnBoardSysTstComp_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestVehicleInformation.h>

/** \brief The generic service handler for OBD service RequestVehicleInformation (0x09)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestVehicleInformation_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if ( (DCM_DSP_USE_SERVICE_0X19 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0XAF == STD_ON) )
#include <Dcm_Dsp_SvcH_ReadDTCInformation.h>

/** \brief Generic subservice handler for UDS service ReadDTCInformation (0x19)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */

/** \brief Generic subservice handler for UDS service ReadGenericInformation (0xAF)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadGenericInformation_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByIdentifier.h>

/** \brief Generic subservice handler for UDS service ReadDataByIdentifier (0x22)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X23 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadMemoryByAddress.h>

/** \brief Generic subservice handler for UDS service ReadMemoryByAddress (0x23)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadMemoryByAddress_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadScalingDataByIdentifier.h>

/** \brief Generic subservice handler for UDS service ReadScalingDataByIdentifier (0x24)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadScalingDataByIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#include <Dcm_Dsp_SvcH_SecurityAccess.h>

/** \brief Generic subservice handler for UDS service SecurityAccess (0x27)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_SecurityAccess_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) MsgContextPtr);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X28 == STD_ON)
#include <Dcm_Dsp_SvcH_CommunicationControl.h>

/** \brief Generic subservice handler for UDS service CommunicationControl (0x28)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_CommunicationControl_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>

/** \brief Generic subservice handler for UDS service ReadDataByPeriodicIdentifier (0x2A)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
#include <Dcm_Dsp_SvcH_DynamicallyDefineDataIdentifier.h>

/** \brief Generic subservice handler for UDS service DynamicallyDefineDataIdentifier (0x2C)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)
#include <Dcm_Dsp_SvcH_WriteDataByIdentifier.h>

/** \brief Generic subservice handler for UDS service WriteDataByIdentifier (0x2E)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteDataByIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>

/** \brief Generic subservice handler for UDS service InputOutputControlByIdentifier (0x2F)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
#include <Dcm_Dsp_SvcH_RoutineControl.h>

/** \brief Generic subservice handler for UDS service RoutineControl (0x31)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X34 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestDownload.h>

/** \brief Generic subservice handler for UDS service RequestDownload (0x34)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestDownload_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X35 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestUpload.h>

/** \brief Generic subservice handler for UDS service RequestUpload (0x35)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestUpload_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X36 == STD_ON)
#include <Dcm_Dsp_SvcH_TransferData.h>

/** \brief Generic subservice handler for UDS service TransferData (0x36)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TransferData_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X37 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestTransferExit.h>

/** \brief Generic subservice handler for UDS service RequestTransferExit (0x37)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestTransferExit_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X3D == STD_ON)
#include <Dcm_Dsp_SvcH_WriteMemoryByAddress.h>

/** \brief Generic subservice handler for UDS service WriteMemoryByAddress (0x3D)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_WriteMemoryByAddress_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
#include <Dcm_Dsp_SvcH_ControlDTCSetting.h>

/** \brief Generic subservice handler for UDS service ControlDTCSetting (0x85)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X86 == STD_ON)
#include <Dcm_Dsp_SvcH_ResponseOnEvent.h>

/** \brief Generic subservice handler for UDS service ResponseOnEvent (0x86)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ResponseOnEvent_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X87 == STD_ON)
#include <Dcm_Dsp_SvcH_LinkControl.h>

/** \brief Generic subservice handler for UDS service LinkControl (0x87)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_LinkControl_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#if (DCM_DSP_USE_SERVICE_0X3E == STD_ON)
#include <Dcm_Dsp_SvcH_TesterPresent.h>

/** \brief Generic subservice handler for UDS service TesterPresent (0x3E)
 **
 ** \fn extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_TesterPresent_SvcH(Dcm_OpStatusType OpStatus, P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
 */
#endif

#endif /* ifndef DCM_GENERIC_SERVICE_HANDLERS_H */
/*===================[End of file]==============================================================*/
