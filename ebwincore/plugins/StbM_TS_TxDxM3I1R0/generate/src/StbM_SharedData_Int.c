/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!AUTOSPACING!]
[!INCLUDE "../include/StbM_Vars.m"!]

/** \addtogroup Synchronized Time Base Manager
 ** @{ */

/*==[Includes]================================================================*/

#include <StbM_Int_Cfg.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

[!IF "($StbMShareDataTimeBasesNumber > 0)"!]

#define STBM_START_SEC_VAR_SHARED_TIME_DATA
#include <StbM_MemMap.h>

/** \brief Variable that holds the Memory Shared Data of the Time Bases */
VAR(StbM_SharedEthOffsetType, STBM_VAR) StbM_SharedEthOffsetData;

#define STBM_STOP_SEC_VAR_SHARED_TIME_DATA
#include <StbM_MemMap.h>

[!ENDIF!]

/*==[Definition of functions with internal linkage]===========================*/

/** @} doxygen end group definition */
/*==================[end of file]===========================================*/

