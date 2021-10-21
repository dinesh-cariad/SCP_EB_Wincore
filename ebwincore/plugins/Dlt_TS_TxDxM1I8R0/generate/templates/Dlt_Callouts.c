/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!AUTOSPACING!]

/*==================[inclusions]=============================================*/

#include <Dlt_Trace.h>
#include <Dlt_UserCallouts.h>

/* !LINKSTO Dlt.swdd.DltGeneration.Template.Dlt_Callouts_c,1 */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/
#define DLT_START_SEC_APPL_CODE
#include <Dlt_MemMap.h>
[!IF "(node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')"!]
FUNC(Dlt_ApplReturnType, DLT_APPL_CODE) Dlt_ApplGetConfigFactoryDefault(
  uint8 data_set_index,
  P2VAR(Dlt_SessionIDType, AUTOMATIC, DLT_APPL_DATA) session_id,
  P2VAR(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_DATA) app_id,
  P2VAR(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_DATA) context_id,
  P2VAR(Dlt_MessageTraceStatusType, AUTOMATIC, DLT_APPL_DATA) trace_status)
{
  Dlt_ApplReturnType RetValue;

  DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_ENTRY(data_set_index, session_id, app_id, context_id, trace_status);

  /* insert your code here */

  RetValue = DLT_APPL_OK;

  DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_EXIT(data_set_index, session_id, app_id, context_id, trace_status, RetValue);
  return RetValue;
}
[!ENDIF!][!// "(node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')"


[!IF "DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true'"!] [!//
FUNC(Dlt_ApplReturnType, DLT_APPL_CODE) Dlt_GetSoftwareVersion(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) softwareVersion,
  CONST(uint32, AUTOMATIC) softwareVersionLength)
{
  Dlt_ApplReturnType RetValue;

  DBG_DLT_GETSOFTWAREVERSION_ENTRY(softwareVersion, softwareVersionLength);

  /* insert your code here */

  RetValue = DLT_APPL_OK;

  DBG_DLT_GETSOFTWAREVERSION_EXIT(RetValue, softwareVersion, softwareVersionLength);
  return RetValue;
}

[!ENDIF!][!//"DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true'"
[!IF "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Callout'"!]
FUNC_P2VAR (uint8, DLT_APPL_DATA, DLT_APPL_CODE) Dlt_AppGetEcuIdAddress(void)
{
  /* replace NULL_PTR with pointer to actual ECU ID */
  return NULL_PTR;
}
[!ENDIF!][!// "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Callout'"
#define DLT_STOP_SEC_APPL_CODE
#include <Dlt_MemMap.h>

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
