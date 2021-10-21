/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */



/*==================[inclusions]============================================*/

#include <IpduM_IsValidConfig.h>

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

#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

FUNC(Std_ReturnType, IPDUM_CODE) IpduM_IsValidConfig
(
   P2CONST(void, AUTOMATIC, IPDUM_APPL_CONST) ConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;

  if (ConfigPtr != NULL_PTR)
  {
    CONSTP2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST) PBConfigPtr =
        (P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST)) ConfigPtr;

    /* Check if the configuration fits to the platform */
    if (TS_PlatformSigIsValid(PBConfigPtr->PlatformSignature) == TRUE)
    {
      /* Validate the post build configuration against the compile time configuration */
      if (IPDUM_CFG_SIGNATURE == PBConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if (IpduM_LcfgSignature == PBConfigPtr->LcfgSignature)
        {
          /* Validate the post build configuration against the published information
                                configuration */
          if (IPDUM_PUBLIC_INFO_SIGNATURE == PBConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  return RetVal;
}

/* AUTOSAR Memory Mapping - end section for code */
#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[internal function definitions]=========================*/
/*==================[end of file]===========================================*/
