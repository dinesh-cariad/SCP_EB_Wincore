/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!AUTOSPACING!]

[!/* count the events with assigned DTC and immediate Nv storage enabled */!][!//
[!VAR "NumEventsWithDemDTCClassRefAndImmNvStorage" = "count(DemConfigSet/*/DemEventParameter/*[node:refexists(DemDTCClassRef) and node:value(as:ref(DemDTCClassRef)/DemImmediateNvStorage) = 'true'])"!][!//

/*==================[inclusions]============================================*/

#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>     /* EB specific standard types, TS_PARAM_UNUSED() */

[!IF "($NumEventsWithDemDTCClassRefAndImmNvStorage > 0) or (DemGeneral/DemImmediateStorageInternalEvents = 'true') or (DemGeneral/DemClearDTCBehavior != 'DEM_CLRRESP_VOLATILE') or node:exists(DemGeneral/DemCalloutDynamicDTCFnc)"!]
#include <Dem_Cfg.h>                             /* declaration of callouts */
[!ENDIF!]

[!IF "($NumEventsWithDemDTCClassRefAndImmNvStorage > 0) or (DemGeneral/DemImmediateStorageInternalEvents = 'true') or (DemGeneral/DemClearDTCBehavior != 'DEM_CLRRESP_VOLATILE')"!]
/* Implementation hint:
 * include necessary Memory-Stack header-files: NvM.h, Ea/Fee.h, Eep/Fls.h */
[!ENDIF!]

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*------------------[Main callouts]-----------------------------------------*/

[!IF "($NumEventsWithDemDTCClassRefAndImmNvStorage > 0) or (DemGeneral/DemImmediateStorageInternalEvents = 'true') or (DemGeneral/DemClearDTCBehavior != 'DEM_CLRRESP_VOLATILE')"!]

#define DEM_START_SEC_[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCallbackMemStackMainFuncTrigger)"!]_CODE
#include <Dem_MemMap.h>

FUNC(void, DEM_APPL_CODE) [!"DemGeneral/DemCallbackMemStackMainFuncTrigger"!](NvM_BlockIdType BlockId)
{
  TS_PARAM_UNUSED(BlockId);

  /* Implement the mechanism to synchronously trigger memory stack main functions
   * within this callout function.
   *
   * There are multiple possibilities for integration of ECU Startup/Shutdown and memory stack main functions.
   * The project-specific integration requirements usually limit the solution space.
   *
   * EB recommends the following integration approach which serves as basis:
   * + This approach MUST be extended according to all additional integration requirements!
   *   Consider the "Integration Note" in the configuration parameter description
   *   of DemCallbackMemStackMainFuncTrigger.
   * + Provide one single memory stack task, e.g. SchMMemTask_<NN>ms, which shall:
   *   - have HIGHER PRIORITY than the task which triggers BswM_MainFunction(), to avoid preemption
   *   - have the number of activation set to 2, to allow activation from BswM_MainFunction()
   *   - contain only the memory stack main functions
   * + Allow triggering of the memory stack task from the following contexts:
   *   - NvM ReadAll/WriteAll and this callout during ECU Startup/Shutdown via BswM_MainFunction():
   *     -- Set BswMModeRequestPort's for NvM_ReadAll() and NvM_WriteAll() to 'BSWM_DEFERRED'
   *     -- Set BswMModeRequestPort's for Dem_Init() and Dem_Shutdown() to 'BSWM_DEFERRED'
   *     -- Use ActivateTask()
   *   - Cyclic activation as required by NvM/Fee/Fls/Ea/Eep via Rte (SchM)
   *
   * Related EB Integration requirements:
   * - lim.NvM.EB_INTREQ_NvM_0001 / lim.Fee.EB_INTREQ_Fee_0001:
   *   call NvM/Ea/Fee main functions only from same task context
   *
   * Code EXAMPLE for indirectly triggering the memory stack from the Dem:
   *
   *  FUNC(void, DEM_APPL_CODE) Appl_TriggerMemoryStack(NvM_BlockIdType BlockId)
   *  {
   *    NvM_RequestResultType Result;
   *
   *    do
   *    {
   *      ActivateTask(SchMMemTask_<NN>ms);
   *      NvM_GetErrorStatus(BlockId, &Result);
   *    }
   *    while (Result == NVM_REQ_PENDING);
   *
   *    // In case an error occurs, the Dem module will handle it internally.
   *    // Optionally, inform the application to let it perform some generic
   *    // error handling:
   *    if (Result != NVM_REQ_OK)
   *    {
   *      // perform customized error handling
   *    }
   *  }
   *
   */
}

#define DEM_STOP_SEC_[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCallbackMemStackMainFuncTrigger)"!]_CODE
#include <Dem_MemMap.h>

[!ENDIF!]

[!IF "node:exists(DemGeneral/DemCalloutDynamicDTCFnc)"!]

#define DEM_START_SEC_[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCalloutDynamicDTCFnc)"!]_CODE
#include <Dem_MemMap.h>

/* With Side Allocation feature enabled, implement this function to fetch a dynamic DTC value.
 *
 *FUNC(uint32, DEM_APPL_CODE) [!"DemGeneral/DemCalloutDynamicDTCFnc"!](Dem_EventIdType EventId, uint32 DTC)
 *{
 *  TS_PARAM_UNUSED(EventId);
 *
 *   * Implementation hint:
 *   *
 *   * - calculate or look up a new DTC, based on the EventId and/or the original DTC value.
 *   *
 *   *   Example:
 *   *
 *   *     EventId        DTC     NEW_DTC_RIGHT  NEW_DTC_LEFT
 *   *        0x80     0x2000            0x2000        0x4000
 *   *        0x81     0x2001            0x2001        0x4001
 *   *        0x82     0x2002            0x2002        0x4002
 *   *        0x83     0x2003            0x2003        0x4003
 *   *
 *   *     If the input EventId is in range of the side specific events and the left side is
 *   *     selected, add a constant offset 0x2000 to the input DTC value. In all other cases
 *   *     return the original i.e. the input DTC value.
 *   *
 *   *   The new DTC value
 *   *   - shall not be equal to 0x000000
 *   *   - shall be in the same group as the original DTC value
 *   *   - shall be unique
 *   *   - shall not vary during the lifetime of the flashed SW.
 *   *
 *  return DTC;
 *}
 */
#define DEM_STOP_SEC_[!"text:toupper(as:modconf('Dem')[1]/DemGeneral/DemCalloutDynamicDTCFnc)"!]_CODE
#include <Dem_MemMap.h>

[!ENDIF!]
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
