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

#ifndef DCM_DSP_SVC_RC_H
#define DCM_DSP_SVC_RC_H

/* !LINKSTO Dcm.Dsn.File.RoutineControl.PublicApi,1 */
/* This file contains the public API of the Diagnostic Service RoutineControl */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>
#include <Dcm_RoutineControl_Cfg.h>                             /*  RoutineControl Configuration */

#if(DCM_DSP_USE_SERVICE_0X31 == STD_ON)

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/** \brief Type to indicate the Routine Identifier*/
/* !LINKSTO Dcm.EB.Dcm_RoutineIDType, 1 */
typedef uint16 Dcm_RoutineIDType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief The generic service handler for UDS service RoutineControl 0x31
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Internal sub-service handler StartRoutine(0x01) for UDS service 0x31
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_StartRoutine_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Internal sub-service handler StopRoutine(0x02) for UDS service 0x31
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_StopRoutine_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Internal sub-service handler RequestResults(0x03) for UDS service 0x31
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_RequestResults_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief This function initializes the Routine Control Service Handler Unit
 *
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_Init(void);

#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
/** \brief This function is required to be present in the Application software when
 **        DcmDspRoutineEnableDefaultInterfaces configuration parameter is set to TRUE and it is
 **        called when an unsupported routine is requested with Start (0x01) sub-service.
 **
 ** \param[in]  OpStatus Operation mode of the function.
 ** \param[in]  RoutineID Unsupported routine identifier
 ** \param[in]  routineControlOptionRecord Pointer to the location in the input buffer of the first
 **                                        byte of the routineControlOptionRecord from the
 **                                        request message
 ** \param[in]  routineControlOptionRecordLength The length of the routineControlOptionRecord from
 **                                        the request message.
 ** \param[out] routineInfoAndStatusRecord Pointer to the location in the output buffer of the first
 **                                        byte of the routineInfo and routineStatusRecord data
 ** \param[out] routineInfoAndStatusRecordLength Pointer to which the application shall write the
 **                                              amount of data, in bytes, which was written to
 **                                              the routineInfoAndStatusRecord pointer,
 **                                              comprising the routineInfo and routineStatusRecord
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether
 **                 the RID is supported or not. Valid only if the return value is E_NOT_OK.
 **
 ** \retval E_OK               Request was successful.
 ** \retval E_NOT_OK           Request was not successful.
 ** \retval DCM_E_PENDING      Request is not yet finished.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultRoutineStart(
  Dcm_OpStatusType OpStatus,
  Dcm_RoutineIDType RoutineID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineControlOptionRecord,
  uint32 routineControlOptionRecordLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecord,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecordLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief This function is required to be present in the Application software when
 **        DcmDspRoutineEnableDefaultInterfaces configuration parameter is set to TRUE and it is
 **        called when an unsupported routine is requested with Stop (0x02) sub-service.
 **
 ** \param[in]  OpStatus Operation mode of the function.
 ** \param[in]  RoutineID Unsupported routine identifier
 ** \param[in]  routineControlOptionRecord Pointer to the location in the input buffer of the first
 **                                        byte of the routineControlOptionRecord from the
 **                                        request message
 ** \param[in]  routineControlOptionRecordLength The length of the routineControlOptionRecord from
 **                                        the request message.
 ** \param[out] routineInfoAndStatusRecord Pointer to the location in the output buffer of the first
 **                                        byte of the routineInfo and routineStatusRecord data
 ** \param[out] routineInfoAndStatusRecordLength Pointer to which the application shall write the
 **                                              amount of data, in bytes, which was written to
 **                                              the routineInfoAndStatusRecord pointer,
 **                                              comprising the routineInfo and routineStatusRecord
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether
 **                 the RID is supported or not. Valid only if the return value is E_NOT_OK.
 **
 ** \retval E_OK               Request was successful.
 ** \retval E_NOT_OK           Request was not successful.
 ** \retval DCM_E_PENDING      Request is not yet finished.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultRoutineStop(
  Dcm_OpStatusType OpStatus,
  Dcm_RoutineIDType RoutineID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineControlOptionRecord,
  uint32 routineControlOptionRecordLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecord,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecordLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief This function is required to be present in the Application software when
 **        DcmDspRoutineEnableDefaultInterfaces configuration parameter is set to TRUE and it is
 **        called when an unsupported routine is requested with RequestResults (0x03) sub-service.
 **
 ** \param[in]  OpStatus Operation mode of the function.
 ** \param[in]  RoutineID Unsupported routine identifier
 ** \param[in]  routineControlOptionRecord Pointer to the location in the input buffer of the first
 **                                        byte of the routineControlOptionRecord from the
 **                                        request message
 ** \param[in]  routineControlOptionRecordLength The length of the routineControlOptionRecord from
 **                                        the request message.
 ** \param[out] routineInfoAndStatusRecord Pointer to the location in the output buffer of the first
 **                                        byte of the routineInfo and routineStatusRecord data
 ** \param[out] routineInfoAndStatusRecordLength Pointer to which the application shall write the
 **                                              amount of data, in bytes, which was written to
 **                                              the routineInfoAndStatusRecord pointer,
 **                                              comprising the routineInfo and routineStatusRecord
 ** \param[out] Nrc Pointer to which the application shall write the NRC to be returned or whether
 **                 the RID is supported or not. Valid only if the return value is E_NOT_OK.
 **
 ** \retval E_OK               Request was successful.
 ** \retval E_NOT_OK           Request was not successful.
 ** \retval DCM_E_PENDING      Request is not yet finished.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DefaultRoutineRequestRoutineResults(
  Dcm_OpStatusType OpStatus,
  Dcm_RoutineIDType RoutineID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineControlOptionRecord,
  uint32 routineControlOptionRecordLength,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecord,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) routineInfoAndStatusRecordLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
#endif /*(DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)*/

#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
/** \brief This function is required to be present in the Application software when
 **        DcmDspRoutineEnableRoutineInfoByte configuration parameter is set to TRUE and it is
 **        called when a requested routine returns E_OK.
 **
 ** \param[in] RoutineID              Routine Identifier
 ** \param[out] RoutineInfoByte       Pointer where the application may write a byte of data which
 **                                   shall be inserted as the first byte in the response buffer
 **                                   after the Routine Identifier
 **
 ** \retval TRUE               Byte was written in the pointer.
 ** \retval FALSE              Byte was not written in the pointer.
 */
extern FUNC(boolean, DCM_CODE) Dcm_AddRoutineInfoByte(
  Dcm_RoutineIDType RoutineID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) RoutineInfoByte);
#endif /*(DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)*/
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if (DCM_DSP_USE_SERVICE_0X31 == STD_ON) */

#endif /* ifndef DCM_DSP_SVC_RC_H */
/*==================[end of file]================================================================*/
