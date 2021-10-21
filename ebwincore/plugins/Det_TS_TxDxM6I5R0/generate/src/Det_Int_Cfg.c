/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains all definitions of the configuration of the AUTOSAR
 * module Det that are target independent. */

/*==================[inclusions]============================================*/

#include <Std_Types.h>
#include <Det_Int.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if (DET_REPORT_ERROR_STOP_EXECUTION == TRUE)
#define DET_START_SEC_CODE
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>

FUNC(void, DET_CODE) Det_StopExecution(uint8 ErrorType)
{
  if (ErrorType == DET_DEVELOPMENT_ERROR_TYPE)
  {
    while (1)
    {
      ;
    }
  }
}

#define DET_STOP_SEC_CODE
/* !LINKSTO Det.EB.req.bsw_include,1 */
#include <Det_MemMap.h>
#endif

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
