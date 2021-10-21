/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_EXITTABLE_H
#define CANTP_EXITTABLE_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* typedefs for AUTOSAR com stack */

#include <CanTp_ExitTableTypes.h>     /* CanTp jumptable type definitions */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

[!IF "(CanTpJumpTable/CanTpJumpTableMode != 'OFF')"!][!//
#define CANTP_START_SEC_CONST_EXIT_JUMP_TABLE
#include <CanTp_MemMap.h>

/** \brief CanTp exit jumptable */
extern CONST(CanTp_ExitTableType, CANTP_EXIT_JUMP_TABLE) CanTp_ExitTable;

#define CANTP_STOP_SEC_CONST_EXIT_JUMP_TABLE
#include <CanTp_MemMap.h>

[!ENDIF!][!//
/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_EXITTABLE_H */
/*==================[end of file]============================================*/
