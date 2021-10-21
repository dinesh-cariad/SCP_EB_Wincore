/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef WDGIF_CFG_H
#define WDGIF_CFG_H
/*==================[inclusions]=============================================*/
[!NOCODE!]
[!AUTOSPACING!]
[!VAR "WDGIF_DEVCOUNT"="count(WdgIfDevice/*)"!][!//
*** the existence of at least one driver is enforced in the xdm file
*** list elements have a multipicity of 1..255

*** An XDM check ensures that at most one Wdg w/ empty VendorApiInfix is
*** configured
[!CODE!][!//

/*==================[macros]=================================================*/

#ifdef WDGIF_DEV_ERROR_DETECT
#error WDGIF_DEV_ERROR_DETECT already defined!
#endif
/** \brief Macro for DET usage */
#define WDGIF_DEV_ERROR_DETECT  [!//
[!IF "WdgIfGeneral/WdgIfDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#ifdef WDGIF_VERSION_INFO_API
#error WDGIF_VERSION_INFO_API already defined!
#endif
/** \brief En-/disable API version information */
#define WDGIF_VERSION_INFO_API  [!//
[!IF "WdgIfGeneral/WdgIfVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/* !LINKSTO WDGIF.EB.DesignDecision.PreCompileValue1,1 */
#ifdef WDGIF_EB_NUMBER_OF_DEVICES
#error WDGIF_EB_NUMBER_OF_DEVICES already defined!
#endif
/** \brief Number of configured watchdog devices */
#define WDGIF_EB_NUMBER_OF_DEVICES [!"num:integer($WDGIF_DEVCOUNT)"!]U
[!ENDCODE!]

************************************************************************************
*** Provide symbolic name values for WdgIfDeviceIndex.
************************************************************************************

[!CODE!][!//
[!LOOP "WdgIfDevice/*"!]

#if (defined WdgIfConf_WdgIfDevice_[!"name(.)"!])
#error WdgIfConf_WdgIfDevice_[!"name(.)"!] is already defined
#endif
/** \brief Export symbolic name value */
#define WdgIfConf_WdgIfDevice_[!"name(.)"!]  [!"WdgIfDeviceIndex"!]U
[!ENDLOOP!]
[!ENDCODE!]

*** Symbolic name values are provided with AUTOSAR 4.0 rev3 first.
*** So, there is no need to provide compatible definitions for preceding releases.


************************************************************
*** initialize setmode and trigger base variables.
************************************************************

*** Init the variables
[!VAR "setmodefuncs" = "''"!]
[!VAR "triggerfuncs" = "''"!]
[!VAR "connector" = "''"!]        ** Contains a comma after the first loop
[!VAR "infix" = "''"!]

************************************************************************
*** loop over watchdog references and generate driver specific Wdg_SetMode
*** and Wdg_SetTriggerCondition functions
************************************************************************
[!IF "WdgIfGeneral/WdgIfDriverAPIInfixEnable = 'true'"!][!//
[!LOOP "WdgIfDevice/*"!][!//
[!IF "node:exists(WdgIfDrvBswImplementationRef)"!][!//
[!VAR "WDGIF_VENDORAPIINFIX"!][!//
[!"asc:getVendorApiInfix(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
[!ENDVAR!][!//
[!VAR "WDGIF_VENDORID"!][!//
[!"asc:getVendorId(as:path(node:ref(WdgIfDrvBswImplementationRef)))"!][!//
[!ENDVAR!][!//
[!ELSE!][!//
[!VAR "WDGIF_VENDORAPIINFIX"!][!//
[!"as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorApiInfix"!][!//
[!ENDVAR!][!//
[!VAR "WDGIF_VENDORID"!][!//
[!"as:ref(WdgIfDriverRef)/../CommonPublishedInformation/VendorId"!][!//
[!ENDVAR!][!//
[!ENDIF!][!//
*** non-empty VendoApiInfix, infix corresponds to _VendorId_VendorApiInfix
[!IF "$WDGIF_VENDORAPIINFIX != ''"!]
[!VAR "infix"!][!//
_[!"$WDGIF_VENDORID"!]_[!"$WDGIF_VENDORAPIINFIX"!][!//
[!ENDVAR!]
[!ENDIF!]
*** concatenate
*** - setmodefuncs, connector, Modulname Wdg, infix and function name SetMode
*** - settriggerfuncs, connector, Modulname Wdg, infix and function name SetTriggerCondition
[!VAR "setmodefuncs"!][!"$setmodefuncs"!][!"$connector"!] &Wdg[!"$infix"!]_SetMode[!ENDVAR!]
[!VAR "triggerfuncs"!][!"$triggerfuncs"!][!"$connector"!] &Wdg[!"$infix"!]_SetTriggerCondition[!ENDVAR!]
[!VAR "connector"="','"!] *** after first loop cycle, we need a comma as seperator
[!ENDLOOP!]
[!ELSE!][!//
[!VAR "setmodefuncs"!][!"$setmodefuncs"!] &Wdg_SetMode[!ENDVAR!]
[!VAR "triggerfuncs"!][!"$triggerfuncs"!] &Wdg_SetTriggerCondition[!ENDVAR!]
[!ENDIF!]

[!IF "$WDGIF_DEVCOUNT = 1"!]
[!CODE!]

#if (defined WdgIf_IntSetMode)
#error WdgIf_IntSetMode is already defined
#endif
/* Internal macro for mapping Wdg_SetMode() to the underlying watchdog device */
#define WdgIf_IntSetMode(DeviceIndex,WdgMode)     Wdg[!"$infix"!]_SetMode(WdgMode)

#if (defined WdgIf_IntTrigger)
#error WdgIf_IntTrigger is already defined
#endif
/* Internal macro for mapping Wdg_SetTriggerCondition() to the underlying watchdog device */
#define WdgIf_IntTrigger(DeviceIndex,Timeout)     Wdg[!"$infix"!]_SetTriggerCondition(Timeout)
[!ENDCODE!]
[!ELSE!]
** multiple devices
[!CODE!]

#if (defined WdgIf_IntSetMode)
#error WdgIf_IntSetMode is already defined
#endif
/* Internal macro for mapping Wdg_SetMode() to the underlying watchdog
 * devices */
#define WdgIf_IntSetMode(DeviceIndex,WdgMode) \
  (WdgIf_SetModeFpArray[(DeviceIndex)](WdgMode))

#if (defined WdgIf_IntTrigger)
#error WdgIf_IntTrigger is already defined
#endif
/* Internal macro for mapping Wdg_SetTriggerCondition() to the underlying watchdog
 * devices */
#define WdgIf_IntTrigger(DeviceIndex,Timeout) \
  (WdgIf_TriggerFpArray[(DeviceIndex)](Timeout))

#if (defined WDGIF_SETMODEPTRS)
#error WDGIF_SETMODEPTRS is already defined
#endif
/** \brief Function pointer macro for mapping Wdg_SetMode to the underlying
 ** watchdog devices
 **
 ** WDGIF_SETMODEPTRS contains the driver specific Wdg_SetMode functions for
 ** all possible drivers
 **/
#define WDGIF_SETMODEPTRS {[!"$setmodefuncs"!]}

#if (defined WDGIF_TRIGGERPTRS)
#error WDGIF_TRIGGERPTRS is already defined
#endif
/** \brief Function pointer macro for mapping Wdg_SetTriggerCondition to the underlying
 ** watchdog devices
 **
 ** WDGIF_TRIGGERPTRS contains the driver specific Wdg_SetTriggerCondition functions for
 ** all possible drivers
 **/
#define WDGIF_TRIGGERPTRS {[!"$triggerfuncs"!]}
[!ENDCODE!]
[!ENDIF!]

[!ENDNOCODE!]

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef WDGIF_CFG_H */
/*==================[end of file]============================================*/
