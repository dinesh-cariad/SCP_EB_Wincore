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

#ifndef LINIF_TRCVTYPES_H
#define LINIF_TRCVTYPES_H

#include <LinIf_CfgTypes.h>
[!NOCODE!][!INCLUDE "LinIf_Macros.m"!][!ENDNOCODE!]
[!CALL "GenerateTrcvIncludes"!]

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/
[!IF "$numTrcvs > 0"!]
/** \brief Pointer to function LinTrcv_SetOpMode()
 **
 ** Function pointer type for the Lin Transceiver function
 ** LinTrcv_SetOpMode().
 */
typedef P2FUNC(Std_ReturnType, LINTRCV_CODE, LinIf_TrcvSetOpModeFuncPtrType)
(
  uint8 LinNetwork,
  LinTrcv_TrcvModeType OpMode
);

/** \brief Pointer to function LinTrcv_GetOpMode()
 **
 ** Function pointer type for the LIN Transceiver function
 ** LinTrcv_GetOpMode().
 */
typedef P2FUNC(Std_ReturnType, LINTRCV_CODE, LinIf_TrcvGetOpModeFuncPtrType)
(
  uint8 LinNetwork,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_DATA) OpMode
);

/** \brief Pointer to function LinTrcv_GetBusWuReason()
 **
 ** Function pointer type for the LIN Transceiver function
 ** LinTrcv_GetBusWuReason().
 */
typedef P2FUNC(Std_ReturnType, LINTRCV_CODE, LinIf_TrcvGetBusWuReasonFuncPtrType)
(
  uint8 LinNetwork,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_DATA) Reason
);

/** \brief Pointer to function LinTrcv_SetWakeupMode()
 **
 ** Function pointer type for the LIN Transceiver function
 ** LinTrcv_SetWakeupMode().
 */
typedef P2FUNC(Std_ReturnType, LINTRCV_CODE, LinIf_TrcvSetWakeupModeFuncPtrType)
(
  uint8 LINNetwork,
  LinTrcv_TrcvWakeupModeType TrcvWakupMode
);

[!IF "$cwuNeeded = 'true'"!][!//
/** \brief Pointer to function LinTrcv_CheckWakeup()
 **
 ** Function pointer type for the LIN Transceiver function
 ** LinTrcv_CheckWakeup().
 */
typedef P2FUNC(Std_ReturnType, LINTRCV_CODE, LinIf_TrcvCheckWakeupFuncPtrType)
(
  uint8 LinNetwork
);
[!ENDIF!][!//

/** \brief LinTrcv function pointers
 **/
typedef struct
{
  LinIf_TrcvSetOpModeFuncPtrType SetOpMode;
  LinIf_TrcvGetOpModeFuncPtrType GetOpMode;
  LinIf_TrcvGetBusWuReasonFuncPtrType GetBusWuReason;
  LinIf_TrcvSetWakeupModeFuncPtrType SetWakeupMode;
[!IF "$cwuNeeded = 'true'"!][!//
  LinIf_TrcvCheckWakeupFuncPtrType CheckWakeup;
[!ENDIF!][!//
} LinIf_LinTrcvFuncPtrType;

[!NOCODE!][!IF "$numTrcvs = 1 and $trcvDoApiInfix = 'true'"!]
/* Macros for backwards compatibility */
[!CALL "EnumerateTrcvs"!]
[!VAR "apiPrefix" = "concat('LinTrcv', text:split($trcvLst,',')[4], substring-before(text:split($trcvLst,',')[5], ';'))"!]
[!VAR "apis" = "'SetOpMode GetOpMode GetBusWuReason SetWakeupMode'"!]
[!IF "$cwuNeeded = 'true'"!]
[!VAR "apis" = "concat($apis, ' CheckWakeup')"!]
[!ENDIF!]
[!LOOP "text:split($apis)"!]
[!VAR "defApi" = "concat('LinTrcv_', .)"!]
[!CODE!]
#if (defined [!"$defApi"!])
#error [!"$defApi"!] already defined!
#endif /* #if( defined [!"$defApi"!])*/
#define [!"$defApi"!] [!"concat($apiPrefix, '_', .)"!]
[!ENDCODE!][!//
[!ENDLOOP!]
[!ENDIF!][!ENDNOCODE!][!//

[!ENDIF!][!//
#endif /* ifndef LINIF_TRCVTYPES_H */
/*==================[end of file]============================================*/
