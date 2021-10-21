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
#ifndef DLT_USERCALLOUTS_H
#define DLT_USERCALLOUTS_H

/* !LINKSTO Dlt.swdd.DltConfig.Template.Dlt_UserCallouts_h,1 */
[!AUTOSPACING!]

/*==================[includes]===============================================*/

#include <Dlt.h>

/*==================[macros]=================================================*/

#if (defined DLT_APPL_OK)
#error DLT_APPL_OK already defined
#endif
/** \brief return value of DLT application callout for successful execution. */
#define DLT_APPL_OK                     0x01U

#if (defined DLT_APPL_NOT_OK)
#error DLT_APPL_NOT_OK already defined
#endif
/** \brief return value of DLT application callout for unsuccessful execution. */
#define DLT_APPL_NOT_OK                 0x00U

/*==================[type definitions]=======================================*/

[!IF "((node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')) or (DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true')"!]
/** \brief DLT application callout return type
 **
 ** This type provides the different return values to be used by the application
 ** callouts.
 */
typedef uint8 Dlt_ApplReturnType;
[!ENDIF!][!// "(node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')and (DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true')"
/*==================[external function declarations]=========================*/
#define DLT_START_SEC_APPL_CODE
#include <Dlt_MemMap.h>

[!IF "(node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')"!]
/** \brief Function prototype providing the factory default settings for pairs of
 ** ApplicationID and ContextID
 ** This callout function shall provide the user specific values for desired
 ** pairs of ApplicationID and ContextID
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Dlt_Callouts.c from the templates folder can be used as a template.
 ** \param[in]      data_set_index    Variable to hold the current data_set_index.
 ** \param[in,out]  session_id        Pointer to the RAM area where the session_id will be hold.
 ** \param[in,out]  app_id            Pointer to the RAM area where the app_id will be hold.
 ** \param[in,out]  context_id        Pointer to the RAM area where the context_id will be hold.
 ** \param[in,out]  trace_status      Pointer to the RAM area where the trace_status will be hold.
 ** \return Function result
 ** \retval DLT_APPL_OK function execution ended in success
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
extern FUNC(Dlt_ApplReturnType, DLT_APPL_CODE) Dlt_ApplGetConfigFactoryDefault(
  uint8 data_set_index,
  P2VAR(Dlt_SessionIDType, AUTOMATIC, DLT_APPL_DATA) session_id,
  P2VAR(Dlt_ApplicationIDType, AUTOMATIC, DLT_APPL_DATA) app_id,
  P2VAR(Dlt_ContextIDType, AUTOMATIC, DLT_APPL_DATA) context_id,
  P2VAR(Dlt_MessageTraceStatusType, AUTOMATIC, DLT_APPL_DATA) trace_status);

[!ENDIF!][!// "(node:exists(DltGeneral/DltImplementNVRamStorage) = 'true') and (DltGeneral/DltImplementNVRamStorage = 'true')"

[!IF "DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true'"!]
/** \brief Function prototype providing the ECU software version.
 ** This callout function shall provide the customer specific string representing
 **  the ECU software version info.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Dlt_Callouts.c from the templates folder can be used as a template.
 ** \param[in,out]  softwareVersion        Pointer to the RAM area where the version string will be written.
 ** \param[in]      softwareVersionLength  Length of the buffer that will hold the version string.
 ** \return Function result
 ** \retval DLT_APPL_OK function execution ended in success
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
  /* !LINKSTO EB_Req_ControlMessage_GetSoftwareVersion_Callout, 1 */
extern FUNC(Dlt_ApplReturnType, DLT_APPL_CODE) Dlt_GetSoftwareVersion(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_DATA) softwareVersion,
  CONST(uint32, AUTOMATIC) softwareVersionLength);
[!ENDIF!][!// "DltMultipleConfigurationContainer/DltProtocol/DltGetSoftwareVersion = 'true'"

/* !LINKSTO EB_Req_EcuIdChoice.Callout.Description,1 */
[!IF "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Callout'"!]
/** \brief Function prototype providing the ECU ID.
 ** This callout function shall provide a pointer to the customer specific ECU ID at runtime.
 **
 ** Implementation of this function shall be provided by the user.
 ** The file Dlt_Callouts.c from the templates folder can be used as a template.
 ** \return pointer to the location of the ECU ID.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** */
extern FUNC_P2VAR (uint8, DLT_APPL_DATA, DLT_APPL_CODE) Dlt_AppGetEcuIdAddress(void);
[!ENDIF!][!// "DltMultipleConfigurationContainer/DltProtocol/DltEcuIdChoice = 'Callout'"
#define DLT_STOP_SEC_APPL_CODE
#include <Dlt_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/
/*==================[internal function definitions]==========================*/

#endif /* ifndef DLT_USERCALLOUTS_H */
/*==================[end of file]============================================*/
