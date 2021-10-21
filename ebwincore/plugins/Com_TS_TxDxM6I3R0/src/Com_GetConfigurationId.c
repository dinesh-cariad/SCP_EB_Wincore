/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */
#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Development Error Detection */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif
/** \brief Com_GetConfiguratinoId - get post-build-time configuration ID
 * \param[in] None
 * \return
 * \retval post-build-time configuration ID
 */
FUNC(uint32, COM_CODE) Com_GetConfigurationId
(
   void
)
{
    uint32 ConfigId;

    DBG_COM_GETCONFIGURATIONID_ENTRY();

#if (COM_DEV_ERROR_DETECT == STD_ON)
    /* check whether COM is initialized */
    if (Com_InitStatus == COM_EB_UNINIT)
    {
        COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_GetConfigurationId);
        ConfigId = (uint32) UINT32_C(0xFFFFFFFF);
    }
    else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
    {
        ConfigId = Com_gConfigPtr->ComConfigurationId;
    }


    DBG_COM_GETCONFIGURATIONID_EXIT(ConfigId);
    return ConfigId;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
