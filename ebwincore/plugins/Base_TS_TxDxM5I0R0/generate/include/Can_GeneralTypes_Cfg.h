#ifndef CAN_GENERALTYPES_CFG_H
#define CAN_GENERALTYPES_CFG_H
[!AUTOSPACING!][!//
/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <Std_Types.h>          /* AUTOSAR standard type definitions */

/*==================[macros]=================================================*/

#if (defined TS_GENERALTYPES_CAN_CONTROLLERSTATETYPE)
#error TS_GENERALTYPES_CAN_CONTROLLERSTATETYPE already defined
#endif

/** \brief Macro for enabling generation of type Can_ControllerStateType
 *
 * Range:
 *  - STD_ON (if configuration parameter 'Base/GeneralTypes/GeneralTypes_Can_ControllerStateType' enabled)
 *  - STD_OFF (if configuration parameter 'Base/GeneralTypes/GeneralTypes_Can_ControllerStateType' disabled)
 */
/* !LINKSTO Base.GeneralTypes.Can_ControllerStateType,1 */
#define TS_GENERALTYPES_CAN_CONTROLLERSTATETYPE [!WS!][!IF "as:modconf('Base')[1]/GeneralTypes/GeneralTypes_Can_ControllerStateType='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==================[type definitions]=======================================*/

/** \brief Represents the hardware object handles of a CAN hardware unit. For CAN
 *         hardware units with more than 255 HW objects use extended range (generated).
 *
 * Range:
 *  - uint16 (if configuration parameter 'CanIf/CanIfPublicCfg/CanIfPublicHandleTypeEnum = UINT16')
 *  - uint8  (otherwise)
 */
/* !LINKSTO Base.ASR403.CAN429,1 */
[!IF "node:exists(as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfPublicHandleTypeEnum) and
      (as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfPublicHandleTypeEnum = 'UINT16')"!]
typedef uint16 Can_HwHandleType;
[!ELSE!][!//
typedef uint8 Can_HwHandleType;
[!ENDIF!][!//

/** \brief Represents the Identifier of an L-PDU. For extended IDs the most
 *        significant bit is set (generated).
 *
 * Range:
 *  - uint16 (if configuration parameter 'CanIf/CanIfPublicCfg/CanIfPublicCanIdTypeEnum = UINT16')
 *  - uint32 (otherwise)
 */
/* !LINKSTO Base.ASR403.CAN416,1 */
[!IF "node:exists(as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfPublicCanIdTypeEnum) and
      (as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfPublicCanIdTypeEnum = 'UINT16')"!]
typedef uint16 Can_IdType;
[!ELSE!][!//
typedef uint32 Can_IdType;
[!ENDIF!][!//

#endif /* ifndef CAN_GENERALTYPES_CFG_H */
/*==================[end of file]============================================*/
