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
#ifndef CANTP_ENTRYTABLE_H
#define CANTP_ENTRYTABLE_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* typedefs for AUTOSAR com stack */

#include <CanTp_EntryTableTypes.h>  /* CanTp jumptable type definitions */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/
[!IF "CanTpJumpTable/CanTpJumpTableMode = 'SERVER'"!][!//

#define CANTP_START_SEC_CONST_ENTRY_JUMP_TABLE
#include <CanTp_MemMap.h>

/** \brief CanTp entry jumptable */
extern CONST(CanTp_EntryTableType, CANTP_ENTRY_JUMP_TABLE) CanTp_EntryTable;

#define CANTP_STOP_SEC_CONST_ENTRY_JUMP_TABLE
#include <CanTp_MemMap.h>

[!ELSEIF "CanTpJumpTable/CanTpJumpTableMode = 'CLIENT'"!][!//

#define CANTP_START_SEC_CONST_UNSPECIFIED
#include <CanTp_MemMap.h>

/** \brief CanTp entry jumptable pointer */
extern CONSTP2CONST(CanTp_EntryTableType, CANTP_CONST, CANTP_ENTRY_JUMP_TABLE) CanTp_EntryTablePtr;

#define CANTP_STOP_SEC_CONST_UNSPECIFIED
#include <CanTp_MemMap.h>

[!ENDIF!][!//
/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_ENTRYTABLE_H */
/*==================[end of file]============================================*/
