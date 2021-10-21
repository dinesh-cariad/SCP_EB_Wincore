/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Can.h>
#include <Can_Internal.h>
#include <SchM_Can.h>
#include <cif_swig_defs.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#if (CAN_NUM_HRH > 0)

#define CAN_START_SEC_CONST_8BIT
#include <Can_MemMap.h>

CONST(uint8, CAN_CONST)Can_HrhLookUpTable[CAN_NUM_HRH] =
{
[!LOOP "node:order(CanConfigSet/*[1]/CanHardwareObject/*, 'node:value(./CanObjectId)')"!][!/*
*/!][!IF "node:value(./CanObjectType) = 'RECEIVE'"!]  [!"node:value(./CanObjectId)"!]U, /* [!"node:name(.)"!] */
[!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!]};

#define CAN_STOP_SEC_CONST_8BIT
#include <Can_MemMap.h>

#endif /* (CAN_NUM_HRH > 0) */

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/



[!VAR "PeriodIndex" = "0"!]
[!LOOP "CanGeneral/CanMainFunctionRWPeriods/CanMainFunctionReadPeriod/*"!]
/** \brief Declare multiple Can_MainFunction_Read's for different periods */
  extern FUNC(void, CAN_CODE) Can_MainFunction_Read_[!"num:i($PeriodIndex)"!](void)
  {
    Can_MainFunction_Read_Internal();
  }
  [!VAR "PeriodIndex" ="num:i($PeriodIndex + '1')"!][!//
[!ENDLOOP!][!//

[!VAR "PeriodIndex" = "0"!][!//
[!LOOP "CanGeneral/CanMainFunctionRWPeriods/CanMainFunctionWritePeriod/*"!]
/** \brief Declare multiple Can_MainFunction_Write's for different periods */
  extern FUNC(void, CAN_CODE) Can_MainFunction_Write_[!"num:i($PeriodIndex)"!](void)
  {
    Can_MainFunction_Write_Internal();
  }
  [!VAR "PeriodIndex" ="num:i($PeriodIndex + '1')"!][!//
[!ENDLOOP!][!//
[!VAR "MaxConfigCount" = "num:i(count(./CanControllerBaudrateConfig/*))"!][!//
[!VAR "ConfigCount" = "num:i(count(CanConfigSet/*)) - 1"!][!//
[!FOR "Instance" = "0" TO "$ConfigCount"!][!//
[!VAR "SelectConfig" = "concat('CanConfigSet/*[',$Instance + 1,']')"!][!//
[!SELECT "node:ref($SelectConfig)"!][!//
/* instance [!"num:inttohex($Instance)"!] */
[!VAR "ControllerCount" = "num:i(count(CanController/*)) "!][!//
/** \brief runtime data for controllers */
Can_CtrlStateTableType Can_ControllerStateTable[[!"$ControllerCount"!]U];

[!//
[!VAR "CtrlIdx" = "0"!][!//
[!LOOP "CanController/*"!][!//
Can_BaudRateType Can_Baudrate_[!"node:name(.)"!] = {
  [!"node:value(./CanControllerBaudrateConfig/*[1]/CanControllerBaudRate)"!], /* CanControllerBaudRate */
  [!"node:value(./CanControllerBaudrateConfig/*[1]/CanControllerPropSeg)"!], /* CanControllerPropSeg */
  [!"node:value(./CanControllerBaudrateConfig/*[1]/CanControllerSeg1)"!], /* CanControllerSeg1 */
  [!"node:value(./CanControllerBaudrateConfig/*[1]/CanControllerSeg2  )"!], /* CanControllerSeg2 */
  [!"node:value(./CanControllerBaudrateConfig/*[1]/CanControllerSyncJumpWidth)"!], /* CanControllerSyncJumpWidth */
};

[!IF "node:exists(CanControllerFdBaudrateConfig/*[1])"!][!//
Can_BaudRateFDType Can_BaudrateFD_[!"node:name(.)"!] = {
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/CanControllerFdBaudRate )"!], /* CanControllerFdBaudRate */
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/CanControllerPropSeg)"!], /* CanControllerPropSeg */
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/CanControllerSeg1)"!], /* CanControllerSeg1 */
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/CanControllerSeg2  )"!], /* CanControllerSeg2 */
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/CanControllerSyncJumpWidth)"!], /* CanControllerSyncJumpWidth */
[!IF "./CanControllerFdBaudrateConfig/*[1]/CanControllerTxBitRateSwitch = 'true'"!][!//
  TRUE,/* CanControllerTxBitRateSwitch */
[!ELSE!][!//
  FALSE,/* CanControllerTxBitRateSwitch */
[!ENDIF!][!//
[!IF "node:value(../*[$CtrlIdx+1]/CanHardwareEmulation/CanHardware) = 'CIF_PCAN_CT_USB'"!][!//
  [!"node:value(./CanControllerFdBaudrateConfig/*[1]/PCAN_frequency)"!], /* PCAN_frequency */
[!ELSE!][!//
  0, /* pcan_f not needed */
[!ENDIF!][!//
};
[!ENDIF!][!//
[!VAR "CtrlIdx" = "num:i($CtrlIdx)+1"!]
[!ENDLOOP!][!//
[!ENDSELECT!][!//
[!ENDFOR!][!//
[!VAR "ConfigCount" = "num:i(count(CanConfigSet/*)) - 1"!][!//
[!FOR "Instance" = "0" TO "$ConfigCount"!][!//
[!VAR "SelectConfig" = "concat('CanConfigSet/*[',$Instance + 1,']')"!][!//
[!SELECT "node:ref($SelectConfig)"!][!//
/* instance [!"num:inttohex($Instance)"!] */
[!VAR "ControllerCount" = "num:i(count(CanController/*)) "!][!//
Can_ControllerType Can_Controllers[CAN_CONTROLLER_COUNT] = {
[!VAR "CtrlIdx" = "0"!][!//
[!LOOP "CanController/*"!][!//
  {
    /* [[!"num:i($CtrlIdx)"!]] [!"node:name(.)"!]  */
    &Can_ControllerStateTable[[!"num:i($CtrlIdx)"!]], /* ControllerState */
[!IF "CanFDSupport = 'false'"!][!//
    NULL_PTR, /* BaudrateConfig */
    NULL_PTR, /* FdBaudrateConfig */
[!ELSE!][!//
[!IF "node:exists(CanControllerBaudrateConfig/*[1])"!][!//
    &Can_Baudrate_[!"node:name(.)"!], /* BaudConfig */
[!ELSE!][!//
    NULL_PTR, /* BaudConfig */
[!ENDIF!][!//
[!IF "node:exists(CanControllerFdBaudrateConfig/*[1])"!][!//
    &Can_BaudrateFD_[!"node:name(.)"!], /* BaudFDConfig */
[!ELSE!][!//
    NULL_PTR, /* BaudFDConfig */
[!ENDIF!][!//
[!ENDIF!][!//
    CAN_MODE_[!"node:value(./CanBusoffProcessing)"!], /* CanBusoffProcessing     */
[!IF "CanControllerActivation = 'true'"!][!//
    TRUE, /* CanControllerActivation */
[!ELSE!][!//
    FALSE, /* CanControllerActivation */
[!ENDIF!][!//
    [!"num:i($CtrlIdx)"!], /* CanControllerId */
    CAN_MODE_[!"node:value(./CanRxProcessing)"!], /* CanRxProcessing */
    CAN_MODE_[!"node:value(./CanTxProcessing)"!], /* CanTxProcessing */
    CAN_MODE_[!"node:value(./CanWakeupProcessing)"!], /* CanWakeupProcessing */
[!IF "CanWakeupSupport = 'true'"!][!//
    TRUE, /* CanWakeupSupport */
[!ELSE!][!//
    FALSE, /* CanWakeupSupport */
[!ENDIF!][!//
[!IF "CanFDSupport = 'true'"!][!//
    TRUE, /* CanFDSupport */
[!ELSE!][!//
    FALSE, /* CanFDSupport */
[!ENDIF!][!//
    [!"node:value(./CanHardwareEmulation/CanHardware)"!], /* CanHardware */
    [!"node:value(./CanHardwareEmulation/CanHardwareChannel)"!], /* CanHwChannel */
[!IF "CanFDSupport = 'false'"!][!//
    [!"node:value(./CanHardwareEmulation/BitRate)"!], /* CanBitRate */
[!ELSE!][!//
    CIF_BITRATE_500K, /* CanBitRate */
[!ENDIF!][!//
    [!"node:value(./CanHardwareEmulation/CanBufferSize)"!]U +1U, /* CanBufferSize */
[!VAR "CtrlIdx" = "num:i($CtrlIdx)+1"!][!//
  },
[!ENDLOOP!][!//
[!ENDSELECT!][!//
};
[!ENDFOR!][!//

#if (CAN_NUM_HRH + CAN_NUM_HTH > 0)

#define CAN_START_SEC_VAR_UNSPECIFIED
#include <Can_MemMap.h>

[!VAR "HrhIdx" = "0"!][!//
[!VAR "HthIdx" = "0"!][!//
Can_HohConfigType Can_HohConfig[CAN_NUM_HRH + CAN_NUM_HTH] =
{
[!LOOP "node:order(CanConfigSet/*[1]/CanHardwareObject/*, 'node:value(./CanObjectId)')"!][!//
  {
    /* [!"node:name(.)"!] */
    [!IF "node:value(./CanObjectType) = 'RECEIVE'"!][!"node:value(./CanIdValue)"!]U[!ELSE!]CAN_ID_UNUSED[!ENDIF!],
    [!IF "node:value(./CanObjectType) = 'RECEIVE'"!]CAN_ID_TYPE_[!"node:value(./CanIdType)"!][!ELSE!]CAN_ID_TYPE_UNUSED[!ENDIF!],
    CAN_OBJECT_TYPE_[!"node:value(./CanObjectType)"!],
    [!IF "(node:value(./CanObjectType) = 'RECEIVE') and (node:exists(./CanFilterMaskRef))"!][!"node:value(node:ref(./CanFilterMaskRef)/CanFilterMaskValue)"!]U[!ELSE!]CAN_MASK_UNUSED[!ENDIF!],
    [!IF "node:value(./CanObjectType) = 'RECEIVE'"!][!"num:i($HrhIdx)"!][!VAR "HrhIdx" ="num:i($HrhIdx + '1')"!][!ELSE!][!"num:i($HthIdx)"!][!VAR "HthIdx" ="num:i($HthIdx + '1')"!][!ENDIF!]U,
    &Can_Controllers[[!VAR "CanControllerRefPath" = "node:path(node:ref(CanControllerRef))"!]CAN_CONTROLLER_[!"text:toupper(node:name(node:ref($CanControllerRefPath)))"!]]
  },
[!ENDLOOP!][!//
};

#define CAN_STOP_SEC_VAR_UNSPECIFIED
#include <Can_MemMap.h>

#endif /* (CAN_NUM_HRH + CAN_NUM_HTH > 0) */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
