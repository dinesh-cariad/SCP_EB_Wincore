/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/

#include <Std_Types.h>              /* AUTOSAR standard types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define LINIF_START_SEC_CONST_32
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, LINIF_CONST) LinIf_LcfgSignature;

/* stop data section declaration */
#define LINIF_STOP_SEC_CONST_32
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
