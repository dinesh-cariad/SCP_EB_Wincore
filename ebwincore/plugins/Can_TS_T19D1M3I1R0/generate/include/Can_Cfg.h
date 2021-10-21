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
#ifndef CAN_CFG_H
#define CAN_CFG_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/*==================[macros]================================================*/

/** \brief CAN event processing via interrupts */
#define CAN_MODE_INTERRUPT        0U

/** \brief CAN event processing via polling (MainFunctions) */
#define CAN_MODE_POLLING          1U

/** \brief CAN Buffer mode "Keep oldest" */
#define CAN_BUFFER_KEEP_OLDEST    0U

/** \brief CAN Buffer mode "Keep newest" */
#define CAN_BUFFER_KEEP_NEWEST    1U

/** \brief Debug and Trace support */
#define CAN_TRACE_USAGE                   [!IF "node:exists(as:modconf('Dbg')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Controller Indices */
[!VAR "MaxConfigCount" = "num:i(count(./CanControllerBaudrateConfig/*))"!][!//
[!VAR "ConfigCount" = "num:i(count(CanConfigSet/*)) - 1"!][!//
[!FOR "Instance" = "0" TO "$ConfigCount"!][!//
[!VAR "SelectConfig" = "concat('CanConfigSet/*[',$Instance + 1,']')"!][!//
[!SELECT "node:ref($SelectConfig)"!][!//
[!VAR "ControllerCount" = "num:i(count(CanController/*)) "!][!//
[!//
[!VAR "CtrlIdx" = "0"!][!//
[!LOOP "CanController/*"!][!//
#define CAN_CONTROLLER_[!"text:toupper(node:name(.))"!] [!"num:i($CtrlIdx)"!]U
[!VAR "CtrlIdx" = "num:i($CtrlIdx)+1"!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//
[!ENDFOR!][!//

#define CAN_CONTROLLER_COUNT [!"num:i($CtrlIdx)"!]U
/*---------------[generated code begins here]-------------------------------*/

#if (defined CanConf_CanController_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!])
#error CanConf_CanController_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!] is already defined
#endif
/** \brief Configuration parameter CanControllerId - mapping parameter name <-> id */
#define CanConf_CanController_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!] [!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanControllerId)"!]U

#ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"node:name(CanConfigSet/*[1]/CanController/*[1])"!])
#error [!"node:name(CanConfigSet/*[1]/CanController/*[1])"!] is already defined
#endif
/** \brief Configuration parameter CanControllerId - mapping parameter name <-> id (AUTOSAR version <= 3.1 rev4) */
#define [!"node:name(CanConfigSet/*[1]/CanController/*[1])"!]                [!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanControllerId)"!]U

#if (defined Can_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!])
#error Can_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!] is already defined
#endif
/** \brief Configuration parameter CanControllerId - mapping parameter name <-> id (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Can_[!"node:name(CanConfigSet/*[1]/CanController/*[1])"!]                [!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanControllerId)"!]U
#endif /* ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */

/** \brief Name of CanConfigSet */
#define CAN_CONFIG_SET                    [!"node:name(CanConfigSet/*[1])"!]

/** \brief Configuration parameter CanControllerId - fixed macro name for internal use */
#define CAN_CONTROLLER_ID                 [!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanControllerId)"!]U

/** \brief Configuration parameter CanRxProcessing */
#define CAN_RX_MODE                       CAN_MODE_[!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanRxProcessing)"!]

/** \brief Configuration parameter CanTxProcessing */
#define CAN_TX_MODE                       CAN_MODE_[!"node:value(CanConfigSet/*[1]/CanController/*[1]/CanTxProcessing)"!]

/** \brief Configuration parameter CanIndex */
#define CAN_INDEX                         [!"node:value(CanGeneral/CanIndex)"!]U

/** \brief Configuration parameter CanTimeoutDuration */
#define CAN_TIMEOUT_DURATION              [!"node:value(CanGeneral/CanTimeoutDuration)"!]

/** \brief Configuration parameter CanIdenticalIdCancellation */
#define CAN_IDENTICAL_ID_CANCELLATION     [!"node:value(CanGeneral/CanIdenticalIdCancellation)"!]U

/** \brief Configuration parameter CanMainFunctionBusoffPeriod */
#define CAN_MAIN_FUNCTION_BUSOFF          [!IF "node:exists('CanGeneral/CanMainFunctionBusoffPeriod')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration parameter CanMainFunctionWakeupPeriod */
#define CAN_MAIN_FUNCTION_WAKEUP          [!IF "node:exists('CanGeneral/CanMainFunctionWakeupPeriod')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration parameter CanMainFunctionModePeriod */
#define CAN_MAIN_FUNCTION_MODE_PERIOD     [!"node:value(CanGeneral/CanMainFunctionModePeriod)"!]

/** \brief Configuration parameter CanDevErrorDetection */
#define CAN_DEV_ERROR_DETECTION           [!IF "CanGeneral/CanDevErrorDetection = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration parameter CanVersionInfoApi */
#define CAN_GET_VERSION_INFO_API          [!IF "CanGeneral/CanVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Configuration parameter CanBufferMode */
#define CAN_BUFFER_MODE                   CAN_BUFFER_[!"node:value(CanGeneral/CanBufferMode)"!]

/** \brief Number of receive objects */
#define CAN_NUM_HRH                       [!"num:i(count(CanConfigSet/*[1]/CanHardwareObject/*[CanObjectType='RECEIVE']))"!]U

/** \brief Number of transmit objects */
#define CAN_NUM_HTH                       [!"num:i(count(CanConfigSet/*[1]/CanHardwareObject/*[CanObjectType='TRANSMIT']))"!]U

#define CAN_CONTROLLER_COUNT              [!"num:i(count(CanConfigSet/*[1]/CanController/*))"!]U

[!LOOP "CanConfigSet/*[1]/CanHardwareObject/*[CanObjectType='RECEIVE']"!]

#if (defined CanConf_CanHardwareObject_[!"node:name(.)"!])
#error CanConf_CanHardwareObject_[!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of receive object [!"node:name(.)"!] */
#define CanConf_CanHardwareObject_[!"node:name(.)"!] [!"node:value(./CanObjectId)"!]U

#ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"node:name(.)"!])
#error [!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of receive object [!"node:name(.)"!] (AUTOSAR version <= 3.1 rev4) */
#define [!"node:name(.)"!]                [!"node:value(./CanObjectId)"!]U

#if (defined Can_[!"node:name(.)"!])
#error Can_[!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of receive object [!"node:name(.)"!] (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Can_[!"node:name(.)"!]                [!"node:value(./CanObjectId)"!]U
#endif /* ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]


[!LOOP "CanConfigSet/*[1]/CanHardwareObject/*[CanObjectType='TRANSMIT']"!]

#if (defined CanConf_CanHardwareObject_[!"node:name(.)"!])
#error CanConf_CanHardwareObject_[!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of transmit object [!"node:name(.)"!] */
#define CanConf_CanHardwareObject_[!"node:name(.)"!] [!"node:value(./CanObjectId)"!]U

#ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#if (defined [!"node:name(.)"!])
#error [!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of transmit object [!"node:name(.)"!] (AUTOSAR version <= 3.1 rev4) */
#define [!"node:name(.)"!]                [!"node:value(./CanObjectId)"!]U

#if (defined Can_[!"node:name(.)"!])
#error Can_[!"node:name(.)"!] is already defined
#endif
/** \brief CanObjectId of transmit object [!"node:name(.)"!] (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Can_[!"node:name(.)"!]                [!"node:value(./CanObjectId)"!]U
#endif /* ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

[!IF "node:exists(CanGeneral/CanLPduReceiveCalloutFunction)"!]
/** \brief Declare the custom-named PDU receive callout function */
  extern FUNC(boolean, COM_APPL_CODE) [!"node:value(CanGeneral/CanLPduReceiveCalloutFunction)"!]
  (
      uint8        Hrh,
      Can_IdType   CanId,
      uint8        CanDlc,
      const uint8  *CanSduPtr
  );

  #define CUSTOM_CANLPDURECEIVECALLOUTFUNCTION(HRH, CANID,CANDLC,CANSDUPTR) [!"node:value(CanGeneral/CanLPduReceiveCalloutFunction)"!](HRH, CANID,CANDLC,CANSDUPTR);

[!ENDIF!]

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef CAN_CFG_H */
/*==================[end of file]===========================================*/
