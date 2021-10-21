/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DSL_SECURITYLEVELHANDLER_H
#define DCM_DSL_SECURITYLEVELHANDLER_H

/* !LINKSTO Dcm.Dsn.File.DslSecurityLevelHandler.PublicApi,1 */
/* This file contains the public API of the Security Level Handler software unit. */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>                           /* Declaration of Dcm internals */

#include <Dcm_Types.h>                         /* DCM type definition */

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* GENERATED SECTION BEGIN EA:{30B91353-75A1-4f01-86C6-E96F4FCF93BE} */

/* Interface SecurityLevelHandler defines all support functions for accessing the
 * security level */

/** \brief Initializes the security level to the value 0x00
 *
 *
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_Init(void);

/** \brief This function sets a new security level value in the DCM module
 *
 *  \param newSecLevel [in] Security level requested
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(
  Dcm_SecLevelType newSecLevel
);

/** \brief This function provides the active security level value.
 *
 *
 *  \return Dcm_SecLevelType **/
extern FUNC(Dcm_SecLevelType, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel(void);

/* GENERATED SECTION END   EA:{30B91353-75A1-4f01-86C6-E96F4FCF93BE} */

/** \brief This function sets security level upon session level change.
 **
 **  \param[in] OldSession          Session level before change
 **  \param[in] NewSession          Session level after change
 **
 **  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_SECURITYLEVELHANDLER_H */
/*==================[end of file]================================================================*/
