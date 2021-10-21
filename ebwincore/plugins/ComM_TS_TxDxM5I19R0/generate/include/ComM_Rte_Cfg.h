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
#ifndef COMM_RTE_CFG_H
#define COMM_RTE_CFG_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>  /* AUTOSAR Com Stack standard types */
#include <TSAutosar.h>       /* TS_PROD_* symbols */

[!IF "ComMGeneral/ComMRteUsage = 'true'"!]
#include <ComM_Cfg.h>       /* included precompile time configuration */
#include <Rte_ComM.h>       /* Include Symbols defined by RTE */
[!ENDIF!]

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

#if (COMM_INCLUDE_RTE == STD_ON)
[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI != 'NONE'"!]
#if (COMM_NUM_USERS != 0U)
/** \brief Type for Rte mode switch callback functions */
typedef P2FUNC(Std_ReturnType, RTE_CODE, ComM_RteSwitchCbkType)(
  Rte_ModeType_ComMMode mode);
#endif

[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI = 'AUTOSAR_40'"!]

/** \brief Type for Rte data write callback functions */
typedef P2FUNC(Std_ReturnType, RTE_CODE, ComM_RteWriteCbkType)(
  P2CONST(ComM_UserHandleArrayType, AUTOMATIC, RTE_APPL_CONST) data);
[!ENDIF!]
[!ENDIF!]
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]

#if (COMM_NUM_USERS != 0U)
/** \brief Type for ASR32 Rte mode switch callback functions */
typedef P2FUNC(Std_ReturnType, RTE_CODE, ComM_ASR32_RteSwitchCbkType)(
  Rte_ModeType_ComM_ASR32_Mode mode);
#endif
[!ENDIF!]
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]
#if (COMM_NUM_USERS != 0U)
/** \brief Type for ASR40 Rte mode switch callback functions */
typedef P2FUNC(Std_ReturnType, RTE_CODE, ComM_ASR40_RteSwitchCbkType)(
  Rte_ModeType_ComM_ASR40_Mode mode);
#endif
/** \brief Type for ASR40 Rte data write callback functions */
typedef P2FUNC(Std_ReturnType, RTE_CODE, ComM_ASR40_RteWriteCbkType)(
  P2CONST(ComM_ASR40_UserHandleArrayType, AUTOMATIC, RTE_APPL_CONST) data);
[!ENDIF!]
#endif
/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#if (COMM_INCLUDE_RTE == STD_ON)
[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI != 'NONE'"!]
#if (COMM_NUM_USERS != 0U)
/** \brief Rte switch mode callbacks for each user */
extern CONST(ComM_RteSwitchCbkType, COMM_CONST)
  ComM_RteSwitchCbk[USER_ARRAY_SIZE];
#endif
[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI = 'AUTOSAR_40'"!]

/** \brief Rte data write callbacks for each user */
extern CONST(ComM_RteWriteCbkType, COMM_CONST)
  ComM_RteWriteCbk[COMM_NUM_CHANNELS];
[!ENDIF!]
[!ENDIF!]
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]
#if (COMM_NUM_USERS != 0U)
/** \brief Rte switch mode callbacks for each user */
extern CONST(ComM_ASR32_RteSwitchCbkType, COMM_CONST)
  ComM_ASR32_RteSwitchCbk[USER_ARRAY_SIZE];
#endif
[!ENDIF!]
[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]
#if (COMM_NUM_USERS != 0U)
/** \brief Rte switch mode callbacks for each user */
extern CONST(ComM_ASR40_RteSwitchCbkType, COMM_CONST)
  ComM_ASR40_RteSwitchCbk[USER_ARRAY_SIZE];
#endif
/** \brief Rte data write callbacks for each user */
extern CONST(ComM_ASR40_RteWriteCbkType, COMM_CONST)
  ComM_ASR40_RteWriteCbk[COMM_NUM_CHANNELS];
[!ENDIF!]
#endif

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))

#define COMM_START_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR32ServiceAPI = 'true'"!]
/**
 * \brief Communication mode, which was last succeessuflly reported to each user via the ASR32
 *        interface of the Rte.
 */
extern VAR(ComM_ASR32_ModeType, COMM_VAR) ComM_ASR32_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

[!IF "ComMGeneral/ComMServiceAPI/ComMEnableASR40ServiceAPI = 'true'"!]
/**
 * \brief Communication mode, which was last succeessuflly reported to each user via the ASR40
 *        interface of the Rte.
 */
extern VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_ASR40_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

[!IF "ComMGeneral/ComMServiceAPI/ComMDefaultASRServiceAPI != 'NONE'"!]
/**
 * \brief Communication mode, which was last succeessuflly reported to each user via the default
 *        interface of the Rte.
 */
extern VAR(ComM_ASR40_ModeType, COMM_VAR) ComM_RteModeUser[USER_ARRAY_SIZE];
[!ENDIF!]

#define COMM_STOP_SEC_VAR_CLEARED_8
#include <ComM_MemMap.h>

#endif /* #if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_RTE_CFG_H */
/*==================[end of file]===========================================*/
