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
[!AUTOSPACING!][!//

/* !LINKSTO ComM503_Refine,1 */

/*==================[inclusions]============================================*/

/* prevent RTE symbols defined in ComM.h to be visible for ComM's .c files */
#if (defined COMM_INTERNAL_USAGE)
#error COMM_INTERNAL_USAGE already defined
#endif
#define COMM_INTERNAL_USAGE

#include <ComM_BSW.h>         /* public API symbols */
#include <ComM_Rte_Cfg.h>    /* internal Rte config dependent header */

#if (COMM_INCLUDE_RTE == STD_ON)
/* Functions defined by RTE: Rte_Mode_UM_*_currentMode() */
/* !LINKSTO ComM506_Refine_Rte,1 */
#include <Rte_ComM.h>
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[external constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#if (COMM_INCLUDE_RTE == STD_ON)
[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI != 'NONE'"!]
#if (COMM_NUM_USERS != 0U)
CONST(ComM_RteSwitchCbkType, COMM_CONST) ComM_RteSwitchCbk[USER_ARRAY_SIZE] =
{
[!LOOP "node:order(ComMConfigSet/*[1]/ComMUser/*, 'ComMUserIdentifier')"!][!//
  /* user [!"name(.)"!], ID=[!"ComMUserIdentifier"!] */
  &Rte_Switch_UM_[!"name(.)"!]_currentMode,
[!ENDLOOP!][!//
};
#endif


[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI = 'AUTOSAR_40'"!]
#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
CONST(ComM_RteWriteCbkType, COMM_CONST) ComM_RteWriteCbk[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMConfigSet/*[1]/ComMChannel/*, 'ComMChannelId')"!][!//
  /* channel [!"name(.)"!], ID=[!"ComMChannelId"!] */
[!IF "ComMFullCommRequestNotificationEnabled = 'true'"!][!//
  &Rte_Write_ComM_CR_[!"name(.)"!]_fullComRequestors,
[!ELSE!][!//
  NULL_PTR, /* ComM_CurrentChannelRequest not used for this channel */
[!ENDIF!][!//
[!ENDLOOP!][!//
};
#endif /* COMM_CURRENTCHANNELREQUEST_ENABLED */
[!ENDIF!]
[!ENDIF!]
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]

#if (COMM_NUM_USERS != 0U)
CONST(ComM_ASR32_RteSwitchCbkType, COMM_CONST) ComM_ASR32_RteSwitchCbk[USER_ARRAY_SIZE] =
{
[!LOOP "node:order(ComMConfigSet/*[1]/ComMUser/*, 'ComMUserIdentifier')"!][!//
  /* user [!"name(.)"!], ID=[!"ComMUserIdentifier"!] */
  &Rte_Switch_UM_ASR32_[!"name(.)"!]_currentMode,
[!ENDLOOP!][!//
};
#endif
[!ENDIF!]

[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]

#if (COMM_NUM_USERS != 0U)
CONST(ComM_ASR40_RteSwitchCbkType, COMM_CONST) ComM_ASR40_RteSwitchCbk[USER_ARRAY_SIZE] =
{
[!LOOP "node:order(ComMConfigSet/*[1]/ComMUser/*, 'ComMUserIdentifier')"!][!//
  /* user [!"name(.)"!], ID=[!"ComMUserIdentifier"!] */
  &Rte_Switch_UM_ASR40_[!"name(.)"!]_currentMode,
[!ENDLOOP!][!//
};
#endif

#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
CONST(ComM_ASR40_RteWriteCbkType, COMM_CONST) ComM_ASR40_RteWriteCbk[COMM_NUM_CHANNELS] =
{
[!LOOP "node:order(ComMConfigSet/*[1]/ComMChannel/*, 'ComMChannelId')"!][!//
  /* channel [!"name(.)"!], ID=[!"ComMChannelId"!] */
[!IF "ComMFullCommRequestNotificationEnabled = 'true'"!][!//
  &Rte_Write_ComM_CR_ASR40_[!"name(.)"!]_fullComRequestors,
[!ELSE!][!//
  NULL_PTR, /* ComM_CurrentChannelRequest not used for this channel */
[!ENDIF!][!//
[!ENDLOOP!][!//
};
#endif /* COMM_CURRENTCHANNELREQUEST_ENABLED */
[!ENDIF!]
#endif /* COMM_INCLUDE_RTE */

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external data]=========================================*/

#if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))

#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]
VAR(ComM_ASR32_ModeType, COMM_VAR) ComM_ASR32_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]
VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_ASR40_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI != 'NONE'"!]
VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

#endif /* #if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
