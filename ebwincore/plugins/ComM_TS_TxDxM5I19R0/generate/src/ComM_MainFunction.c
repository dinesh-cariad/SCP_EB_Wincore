/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
[!AUTOSPACING!]
[!INCLUDE "../../generate_macros/ComM_Functions.m"!][!//

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!]
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!]
[!CALL "GetMasterCoreId"!][!//
[!CALL "GetUsedCores"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
/* prevent RTE symbols defined in ComM.h to be visible for ComM's .c files */
#if (defined COMM_INTERNAL_USAGE)
#error COMM_INTERNAL_USAGE already defined
#endif
#define COMM_INTERNAL_USAGE

#include <ComM_Int.h>               /* Module public API */
#include <ComM_BSW_Cfg.h>           /* Module Internal identifiers */
#include <ComM.h>
#include <ComM_Int.h>
#if (COMM_INCLUDE_RTE == STD_ON)
/* Symbols defined by RTE: Rte_ModeType_ComMode
 * Functions declared by RTE: ComM_RequestComMode(), ComM_MainFunction_X() */
 /* !LINKSTO ComM506_Refine_Rte,1 */
#include <Rte_ComM.h>
#endif
#include <SchM_ComM.h>
/* for the case COMM_INCLUDE_RTE == STD_OFF these symbols are provided via
 * ComM.h. */
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!//
    [!FOR "Idx" = "1" TO "count(text:split($UsedCores,'_'))"!][!//
      [!VAR "CurrentCore" = "num:i(string(text:split($UsedCores,'_')[position() = $Idx]))"!][!//
      [!IF "num:i($CurrentCore) != num:i($MasterCoreId)"!][!//
#include <SchM_ComM_1_Slave_[!"num:i($CurrentCore)"!].h>
      [!ENDIF!][!//
    [!ENDFOR!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

[!VAR "LeastMainFnPeriodChId" = "0"!][!//
[!LOOP "node:order(ComMConfigSet/*[1]/ComMChannel/*, 'ComMMainFunctionPeriod')"!][!//
[!VAR "LeastMainFnPeriodChId" = "ComMChannelId"!]
[!BREAK!]
[!ENDLOOP!][!//

[!LOOP "ComMConfigSet/*[1]/ComMChannel/*"!][!//
/** \brief Main function for channelid [!"ComMChannelId"!] **/
/* !LINKSTO ComM429,1, ComM472,1 */
FUNC(void, COMM_CODE) ComM_MainFunction_[!"ComMChannelId"!]
(
  void
)
{
  if (ComM_InitStatus != COMM_UNINIT)
  {
/* !LINKSTO ComM979,1 */
#if (COMM_PNC_SUPPORT == STD_ON)
    if( COMM_PNC_ENABLED )
    {
      [!IF "ComMChannelId = $LeastMainFnPeriodChId"!]
       ComM_PncProcessTimers();
      [!ENDIF!]
      ComM_PncMainFunction();
    }
#endif

    ComM_ProcessTimers([!"ComMChannelId"!]U);

    if(COMM_CH_HSM_EVENT_PENDING([!"ComMChannelId"!]U))
    {
      /* ComM channels are independent of each other and do not send events to
       * each other. Hence the return value is ignored */
      (void) COMM_HSMMAININST(
        &ComM_HsmScComM, [!"ComMChannelId"!]U);
    }

    if(ComM_RteNotificationDoActFkp != NULL_PTR)
    {
      ComM_RteNotificationDoActFkp([!"ComMChannelId"!]U);
    }
  }
}

[!ENDLOOP!][!//
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

[!IF "ComMGeneral/ComMPncSupport = 'true'"!]

[!LOOP "node:order(node:refs(ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='EIRA']
        [ComMPncComSignalDirection='RX']/ComMPncComSignalRef), 'ComHandleId')"!]
[!VAR "Generated" = "'false'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  uint8 ReceivedData[COMM_PN_INFO_LENGTH] = {0};
  /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_1,1 */
  if(Com_ReceiveSignal([!"ComHandleId"!], ReceivedData) == E_OK)
  {
    /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_3,1 */
    if (SCHM_E_OK != SchM_Send_ComM_1_Slave_[!"num:i($FoundCore)"!]_Slave_[!"num:i($FoundCore)"!]_DataSet_[!"node:name(.)"!](ReceivedData))
    {
#if (COMM_DEV_ERROR_DETECT == STD_ON)
       /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
       COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_SLAVE_DATA_SET, COMM_E_SLAVE_DATA_SET);
#endif
    }
  }
}
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
      [!VAR "Generated" = "'true'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$Generated = 'false'"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
  ComM_RxEiraSignalStatus = TRUE;
  ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
[!ENDIF!][!//


[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/** \brief Master core receive function for an RX signal */
FUNC(void, COMM_CODE) ComM_Master_ReceiveFunction_[!"node:name(.)"!] (void)
{
  boolean DataReceived = FALSE;
  [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
  while(SCHM_E_OK == SchM_Receive_ComM_Master_DataGet_[!"node:name(.)"!](&ComM_MulticoreRxBuffer[[!"num:i($RxSignalIndex)"!]][0]))
  {
    DataReceived = TRUE;
  }
  if(DataReceived == TRUE)
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    ComM_RxEiraSignalStatus = TRUE;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
  }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
    COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_MASTER_DATA_GET, COMM_E_MASTER_DATA_GET);
  }
#endif
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//



[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncGatewayEnabled = 'true'"!]
[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA']
        [ComMPncComSignalDirection='RX'][(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType='COMM_GATEWAY_TYPE_ACTIVE')]/ComMPncComSignalRef), 'ComHandleId')"!]
[!VAR "Generated" = "'false'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  uint8 ReceivedData[COMM_PN_INFO_LENGTH] = {0};
  /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_1,1 */
  if(Com_ReceiveSignal([!"ComHandleId"!], ReceivedData) == E_OK)
  {
    if (SCHM_E_OK != SchM_Send_ComM_1_Slave_[!"num:i($FoundCore)"!]_Slave_[!"num:i($FoundCore)"!]_DataSet_[!"node:name(.)"!](ReceivedData))
    {
#if (COMM_DEV_ERROR_DETECT == STD_ON)
       /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
       COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_SLAVE_DATA_SET, COMM_E_SLAVE_DATA_SET);
#endif
    }
  }
}
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
      [!VAR "Generated" = "'true'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$Generated = 'false'"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
  ComM_RxActiveEraSignalStatus = TRUE;
  ComM_RxUnfilteredEraSignalStatus = TRUE;
  ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
[!ENDIF!][!//


[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/** \brief Master core receive function for an RX signal */
FUNC(void, COMM_CODE) ComM_Master_ReceiveFunction_[!"node:name(.)"!] (void)
{
  boolean DataReceived = FALSE;
  [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
  while(SCHM_E_OK == SchM_Receive_ComM_Master_DataGet_[!"node:name(.)"!](&ComM_MulticoreRxBuffer[[!"num:i($RxSignalIndex)"!]][0]))
  {
    DataReceived = TRUE;
  }
  if(DataReceived == TRUE)
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    ComM_RxActiveEraSignalStatus = TRUE;
    ComM_RxUnfilteredEraSignalStatus = TRUE;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
  }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
    COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_MASTER_DATA_GET, COMM_E_MASTER_DATA_GET);
  }
#endif
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalKind='ERA']
        [ComMPncComSignalDirection='RX'][(node:exists(node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType)) and (node:ref(ComMPncComSignalChannelRef)/ComMPncGatewayType='COMM_GATEWAY_TYPE_PASSIVE')]/ComMPncComSignalRef), 'ComHandleId')"!]
[!VAR "Generated" = "'false'"!][!//
[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  uint8 ReceivedData[COMM_PN_INFO_LENGTH] = {0};
  /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_1,1 */
  if(Com_ReceiveSignal([!"ComHandleId"!], ReceivedData) == E_OK)
  {
    if (SCHM_E_OK != SchM_Send_ComM_1_Slave_[!"num:i($FoundCore)"!]_Slave_[!"num:i($FoundCore)"!]_DataSet_[!"node:name(.)"!](ReceivedData))
    {
#if (COMM_DEV_ERROR_DETECT == STD_ON)
       /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
       COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_SLAVE_DATA_SET, COMM_E_SLAVE_DATA_SET);
#endif
    }
  }
}
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
      [!VAR "Generated" = "'true'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$Generated = 'false'"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/* !LINKSTO ComM975,1, ComM986,1, ComM971,1 */
FUNC(void, COMM_CODE) ComM_COMCbk_[!"node:name(.)"!](void)
{
  /* There is one bit indicating the status of each unique Com signal.
   * For easy mapping, the bits are assigned in the ascending order of
   * the signal handle ids.
   */
  ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
  ComM_RxUnfilteredEraSignalStatus = TRUE;
  ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
[!ENDIF!][!//


[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
    [!VAR "FoundCore" = "''"!][!//
    [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'RX'"!][!//
    [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>
/** \brief Master core receive function for an RX signal */
FUNC(void, COMM_CODE) ComM_Master_ReceiveFunction_[!"node:name(.)"!] (void)
{
  boolean DataReceived = FALSE;
  [!CALL "GetRxSignalIndex","SignalHandleId"="ComHandleId"!][!//
  while(SCHM_E_OK == SchM_Receive_ComM_Master_DataGet_[!"node:name(.)"!](&ComM_MulticoreRxBuffer[[!"num:i($RxSignalIndex)"!]][0]))
  {
    DataReceived = TRUE;
  }
  if(DataReceived == TRUE)
  {
    ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0();
    ComM_RxUnfilteredEraSignalStatus = TRUE;
    ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0();
  }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
    COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_MASTER_DATA_GET, COMM_E_MASTER_DATA_GET);
  }
#endif
}
#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!]
[!ENDIF!]

[!IF "as:modconf('ComM')[1]/ComMGeneral/ComMMultiCoreSupport = 'true'"!][!//
  [!IF "as:modconf('ComM')[1]/ComMGeneral/ComMPncSupport = 'true'"!][!//
    [!IF "count(text:split($UsedCores,'_')) > 1 "!][!//
      [!VAR "TxIdx" = "0"!][!//
      [!LOOP "node:order(node:refs(as:modconf('ComM')[1]/ComMConfigSet/*[1]/ComMPnc/*/ComMPncComSignal/*[ComMPncComSignalDirection='TX']/ComMPncComSignalRef), 'ComHandleId')"!][!//
        [!VAR "FoundCore" = "''"!][!//
        [!CALL "GetCore","SignalHandle"="ComHandleId","Direction"="'TX'"!][!//
        [!IF "num:i($FoundCore) != num:i($MasterCoreId)"!][!//
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_START_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>
/** \brief Slave core receive function for a TX signal */
FUNC(void, COMM_1_Slave_1_CODE) ComM_1_Slave_[!"num:i($FoundCore)"!]_Slave_[!"num:i($FoundCore)"!]_ReceiveFunction_[!"node:name(.)"!](void)
{
   uint8 ReceivedData[COMM_PN_INFO_LENGTH] = {0};
   boolean DataReceived = FALSE;
   while(SCHM_E_OK == SchM_Receive_ComM_1_Slave_[!"num:i($FoundCore)"!]_Slave_[!"num:i($FoundCore)"!]_DataGet_[!"node:name(.)"!](ReceivedData))
   {
     DataReceived = TRUE;
   }
   if (DataReceived == TRUE)
   {
     /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_1,1 */
     (void)Com_SendSignal(ComM_TxComSignalCfg[[!"num:i($TxIdx)"!]].ComHandleId, ReceivedData);
   }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
   else
   {
     /* !LINKSTO ComM.EB.SchM_Call_Sender_Receiver_DET,1 */
     COMM_DET_REPORT_ERROR([!"num:i($FoundCore)"!], COMM_SID_SLAVE_DATA_GET, COMM_E_SLAVE_DATA_GET);
   }
#endif
}
#define COMM_1_Slave_[!"num:i($FoundCore)"!]_STOP_SEC_CODE
#include <ComM_1_Slave_[!"num:i($FoundCore)"!]_MemMap.h>

        [!ENDIF!][!//
        [!VAR "TxIdx" = "num:i($TxIdx) + 1"!][!//
      [!ENDLOOP!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
