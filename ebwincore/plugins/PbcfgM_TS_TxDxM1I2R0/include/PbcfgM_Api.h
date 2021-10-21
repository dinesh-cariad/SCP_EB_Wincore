/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PBCFGM_API_H
#define PBCFGM_API_H

/*==================[inclusions]============================================*/
#include <Std_Types.h>
#include <PbcfgM_Types.h>
#include <PbcfgM_Cfg.h>
#include <PbcfgM_Version.h>
#include <PbcfgM_CfgTypes.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define PBCFGM_START_SEC_CODE
#include <PbcfgM_MemMap.h>

/** \brief Initializes all PbcfgM global variables.
 **
 ** Initializes all PbcfgM global variables including default values,
 ** default selector field and state of timeout monitors.
 **
 ** \param[in] PbcfgM_ConfigPtr  Configuration pointer to post build configuration of the PbcfgM
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, PBCFGM_CODE) PbcfgM_Init
(
  P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
);

/** \brief Get a module configuration from the PbcfgM
 **
 ** Used to get a module configuration from the post build configuration manager.
 **
 ** \param[in]  PbcfgM_ModuleId  Module ID
 ** \param[in]  PbcfgM_InstanceId  Instance ID, if a module has just one instance zero shall be
 **             used as default value.
 ** \param[out] PbcfgM_ModuleConfigPtr  Pointer to the requested module configuration. The pointer is
 **             not changed if the configuration is not found.
 **
 ** \return Function execution success status
 ** \retval E_OK  In the case a configuration was found.
 ** \retval E_NOT_OK  In the case no configuration was found.
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{reentrant}
 ** \Synchronicity{synchronous}
 */
extern FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_GetConfig
(
    uint16 PbcfgM_ModuleId,
    uint16 PbcfgM_InstanceId,
    P2VAR(PbcfgM_ModuleConfigPtrType, AUTOMATIC, PBCFGM_APPL_DATA) PbcfgM_ModuleConfigPtr
);

/** \brief Checks if the PbcfgM configuration is valid.
 **
 ** Checks if the PbcfgM configuration and all referenced configurations are valid.
 **
 ** \param[in] PbcfgM_ConfigPtr  Pointer to the PbcfgM configuration.
 **
 ** \return Function execution status.
 ** \retval E_OK  In the case the PbcfgM and all referenced module configurations are valid.
 ** \retval E_NOT_OK  In the case the PbcfgM or one of the referenced module configuration is
 **                   invalid.
 **
 ** \Reentrancy{reentrant}
 ** \Synchronicity{synchronous}
 */
extern FUNC(Std_ReturnType, PBCFGM_CODE) PbcfgM_IsValidConfig
(
    P2CONST(PbcfgM_ConfigType, AUTOMATIC, PBCFGM_APPL_CONST) PbcfgM_ConfigPtr
);

#define PBCFGM_STOP_SEC_CODE
#include <PbcfgM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PBCFGM_API_H */
/*==================[end of file]===========================================*/
