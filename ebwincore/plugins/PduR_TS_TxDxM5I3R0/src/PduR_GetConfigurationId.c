/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <PduR_Trace.h>
#include <TSAutosar.h>            /* EB specific standard types */
#include <ComStack_Types.h>       /* AUTOSAR Communication Stack types */
#include <PduR_Internal_Static.h> /* Internal API (static part for service ID) */
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */

#if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF)
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/* AUTOSAR Memory Mapping - start section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>
#endif

FUNC(PduR_PBConfigIdType, PDUR_CODE) PduR_GetConfigurationId
(
   void
)
{
   PduR_PBConfigIdType RetVal;

   DBG_PDUR_GETCONFIGURATIONID_ENTRY();

#if (PDUR_DEV_ERROR_DETECT == STD_ON)
   /* check if PDU Router is initialized */
   if(PduR_State == PDUR_UNINIT)
   {
      /* reported to Det in development mode */
      PDUR_DET_REPORT_ERROR(PDUR_SID_GET_CONF_ID, PDUR_E_INVALID_REQUEST);

      /* return value that is most likely unused */
      RetVal = (PduR_PBConfigIdType)PDUR_INVALID_CONFIGURATION_ID;
   }
   else
#endif /* #if (PDUR_DEV_ERROR_DETECT == STD_ON) */
   {
      /* passes the unique identifier of the post-build time configuration of the PduR module */
       RetVal = PduR_GConfigPtr->PduR_ConfigurationId;
   }


   DBG_PDUR_GETCONFIGURATIONID_EXIT(RetVal);
   return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#if (TS_MERGED_COMPILE == STD_OFF)
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
#endif

#else /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) */

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT


/*==================[internal function definitions]=========================*/

#endif /* if (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) */
/*==================[end of file]===========================================*/
