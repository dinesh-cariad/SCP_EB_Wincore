[!/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('CANIF_MACROS_M'))"!][!//
[!VAR "CANIF_MACROS_M"="'true'"!][!/*

--------------------------------------------------------------------------
    Set the values of useful variables
--------------------------------------------------------------------------
*/!][!VAR "txBuffersNum" = "num:i(0)"!][!/*

*/!][!VAR "txPduNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfTxPduCfg/*))"!][!/*
*/!][!IF "CanIfPublicCfg/CanIfPublicTxBuffering = 'true'"!][!/*
   */!][!VAR "txBuffersNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfTxPduCfg/*[as:ref(as:ref(as:ref(CanIfTxPduBufferRef)/CanIfBufferHthRef/*[1])/CanIfHthIdSymRef)/CanHandleType='BASIC']))"!][!/*
*/!][!ENDIF!][!/*
*** The number of CAN drivers present in the configuration ***
*/!][!VAR "canDriverNum" = "num:i(count(CanIfCtrlDrvCfg/*))"!][!/*
*/!][!VAR "rxPduNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfRxPduCfg/*))"!][!/*
*/!][!VAR "pduNum" = "num:i($txPduNum + $rxPduNum)"!][!/*
*/!][!VAR "staticTxPduNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfTxPduCfg/*[CanIfTxPduType = 'STATIC']))"!][!/*
*/!][!VAR "dynamicTxPduNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfTxPduCfg/*[CanIfTxPduType = 'DYNAMIC']))"!][!/*
*/!][!VAR "controllerNum" = "num:i(count(CanIfCtrlDrvCfg/*/CanIfCtrlCfg/*))"!][!/*
*/!][!VAR "rxPduRangeConfigNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfRxPduCfg/*[node:exists(CanIfRxPduCanIdRange)]))"!][!/*
*/!][!VAR "hrhNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfInitHohCfg/*/CanIfHrhCfg/*))"!][!/*
*/!][!VAR "hthNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfInitHohCfg/*/CanIfHthCfg/*))"!][!/*
*/!][!VAR "userDlcErrorNotifNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfRxPduCfg/*[CanIfUserDlcErrorNotification != '']))"!][!/*
*/!][!VAR "userDlcPassedNotifNum" = "num:i(count(CanIfInitCfg/*[1]/CanIfRxPduCfg/*[CanIfUserDlcPassedNotification != '']))"!][!/*

  *** global maximum HOH ID ***
  */!][!VAR "maxCanHohId" = "0"!][!/*
  
  */!][!VAR "i" = "0"!][!/*
  *** String with the following info for each driver: "VendorId_ApiInfix|drvMaxCanHohId#"***
  */!][!VAR "driverInfo" = "''"!][!/*
  
  */!][!FOR "i" = "1" TO "$canDriverNum"!][!/*
    *** Vendor ID ***
    */!][!VAR "driverVendorId" = "node:refs(CanIfCtrlDrvCfg/*[num:i($i)]/CanIfCtrlDrvNameRef)/../CommonPublishedInformation/VendorId"!][!/*
  
    *** Api Infix ***
    */!][!VAR "driverApiInfix" = "node:refs(CanIfCtrlDrvCfg/*[num:i($i)]/CanIfCtrlDrvNameRef)/../CommonPublishedInformation/VendorApiInfix"!][!/*
    
    *** Max HW object Id per driver ***
    */!][!VAR "maxCanHthId" = "0"!][!/*
    */!][!IF "$hthNum > 0"!][!/*
       */!][!VAR "maxCanHthId" = "num:max(node:refs(node:refs(CanIfCtrlDrvCfg/*[num:i($i)]/CanIfCtrlDrvInitHohConfigRef)/CanIfHthCfg/*/CanIfHthIdSymRef)/CanObjectId)"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "maxCanHrhId" = "0"!][!/*
    */!][!IF "$hrhNum > 0"!][!/*
       */!][!VAR "maxCanHrhId" = "num:max(node:refs(node:refs(CanIfCtrlDrvCfg/*[num:i($i)]/CanIfCtrlDrvInitHohConfigRef)/CanIfHrhCfg/*/CanIfHrhIdSymRef)/CanObjectId)"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "drvMaxCanHohId" = "num:max(text:split(concat($maxCanHthId,' ',$maxCanHrhId)))"!][!/*
    
  *** Put together the VendorId, ApiInfix and drvMaxCanHohId ***
    */!][!VAR "driverInfo" = "concat($driverInfo, $driverVendorId,'_',$driverApiInfix,'|',num:i($drvMaxCanHohId),'|',num:i($maxCanHohId),'#')"!][!/*
    
    *** increment the global maximum HOH ***
    */!][!VAR "maxCanHohId" = "$maxCanHohId + $drvMaxCanHohId + 1"!][!/*
    
  */!][!ENDFOR!][!/*

*** HOH ID translation optimization usable? ***
*/!][!VAR "hohTranslationOptimization" = "'false'"!][!/*

*/!][!IF "$canIfPublicMultipleDrvSupport = 'false'"!][!/*
  */!][!IF "node:isconsecutive(node:refs(CanIfInitCfg/*[1]/CanIfInitHohCfg/*[1]/CanIfHrhCfg/*/CanIfHrhIdSymRef)/CanObjectId, 0)"!][!/*
     */!][!IF "node:isconsecutive(node:refs(CanIfInitCfg/*[1]/CanIfInitHohCfg/*[1]/CanIfHthCfg/*/CanIfHthIdSymRef)/CanObjectId, $hrhNum)"!][!/*
        */!][!VAR "hohTranslationOptimization" = "'true'"!][!/*
     */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "($hohTranslationOptimization = 'false') and ($canifHOHTransOpt = 'true')"!][!/*
  */!][!ERROR!]If CanIfPublicHohTranslationOpt is enabled, then all HRH CanObjectIds must be smaller than HTH CanObjectIds and all ObjectIds must be 0-based and dense.[!ENDERROR!][!/*
*/!][!ENDIF!][!/*


*** transceiver configuration ***
*/!][!VAR "trcvChannelsNum" = "num:i(count(CanIfTrcvDrvCfg/*/CanIfTrcvCfg/*))"!][!/*


*** determine rxBufferSize ***
*/!][!VAR "rxBufferSize" = "0"!][!/*
*/!][!IF "$canifDLCCheck"!][!/*
   *** If DLC check is enabled, we need CanIfRxPduDlc + 1 bytes per buffer
       if CanIfRxPduDlc is greater than 0 and 9 byte if DLC is set to 0 ***
   */!][!LOOP "CanIfInitCfg/*[1]/CanIfRxPduCfg/*"!][!/*
      */!][!IF "CanIfRxPduReadData"!][!/*
          */!][!IF "CanIfRxPduDlc > 0"!][!/*
             */!][!VAR "rxBufferSize" = "$rxBufferSize + CanIfRxPduDlc + 1"!][!/*
          */!][!ELSE!][!/*
             */!][!VAR "rxBufferSize" = "$rxBufferSize + 9"!][!/*
          */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
   */!][!ENDLOOP!][!/*
*/!][!ELSE!][!/*
   *** If DLC check is not enabled, we need 8 + 1 bytes per buffer ***
   */!][!LOOP "CanIfInitCfg/*[1]/CanIfRxPduCfg/*"!][!/*
      */!][!IF "CanIfRxPduReadData"!][!/*
         */!][!VAR "rxBufferSize" = "$rxBufferSize + 9"!][!/*
      */!][!ENDIF!][!/*
   */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*





*** variable to get the CAN controller sorted according to their ID ***
Value:
Paths to CanIfCtrlCfg containers seperated with "," and sorted
according to CanIfCtrlId (ascending).
*/!][!VAR "sortedCtrlCfgs"="text:join(node:paths(node:order(CanIfCtrlDrvCfg/*/CanIfCtrlCfg/*,'CanIfCtrlId')),',')"!][!/*

*** variable which gives the maximum CAN controller ID ***
*/!][!VAR "maxCanControllerId"="num:max(node:refs(CanIfCtrlDrvCfg/*/CanIfCtrlCfg/*/CanIfCtrlCanCtrlRef)/CanControllerId)"!][!/*


--------------------------------------------------------------------------
    Define macros
--------------------------------------------------------------------------

*** GetControllerIndex: Retrieves the CanIf internal index of the controller ***
*/!][!MACRO "GetControllerIndex", "controllerCfgPath"!][!/*
   */!][!VAR "controllerIdx"="'CANIF_INVALID_CONTROLLER_ID'"!][!/*
   */!][!FOR "i"="0" TO "$controllerNum - 1"!][!/*
      */!][!IF "$controllerCfgPath = text:split($sortedCtrlCfgs,',')[position()-1 = $i]"!][!/*
         */!][!VAR "controllerIdx"!][!"num:i($i)"!][!ENDVAR!][!/*
      */!][!ENDIF!][!/*
   */!][!ENDFOR!][!/*
   */!][!"$controllerIdx"!][!/*
*/!][!ENDMACRO!][!/*

*** GetCanDriverIndex: Retrieves the internal driver Index based on the VendorId and ApiInfix of the driver ***
*/!][!MACRO "GetCanDriverIndex", "VendorId", "ApiInfix"!][!/*
   */!][!VAR "driverIdx"="'CANIF_INVALID_DRIVER_ID'"!][!/*
   */!][!FOR "i"="1" TO "count(text:split($driverInfo, '#'))"!][!/*
   */!][!VAR "tempPos" = "text:split($driverInfo,'#')[position() = $i]"!][!/*
      */!][!IF "text:match(text:split($tempPos, '|')[1], concat($VendorId, '_', $ApiInfix))"!][!/*
         */!][!VAR "driverIdx"!][!"num:i($i - 1)"!][!ENDVAR!][!/*
      */!][!ENDIF!][!/*
   */!][!ENDFOR!][!/*
   */!][!"$driverIdx"!][!/*
*/!][!ENDMACRO!][!/*

*** CreateCanDriverInfixByIdx: Creates Infix for the CAN driver API functions based
    on the CommonPublishedInformation of the CAN driver selected by idx ***
*/!][!MACRO "CreateCanDriverInfixByIdx", "idx"!][!/*
    */!][!VAR "tempPos" = "text:split($driverInfo, '#')[position() = ($idx + 1)]"!][!/*
    */!][!"text:split($tempPos, '|')[1]"!][!/*
*/!][!ENDMACRO!][!/*

*** Determine if a specific driver supports wake up or not ***
*/!][!MACRO "CanDriverWakeupSupport", "idx"!][!/*
    */!][!VAR "tempRet" = "'false'"!][!/*
    */!][!IF "$canWakeupSupport = 'true'"!][!/*
      */!][!IF "$canIfPublicMultipleDrvSupport = 'false'"!][!/*
        */!][!LOOP "CanIfCtrlDrvCfg/*[1]/CanIfCtrlCfg/*"!][!/*
          */!][!IF "node:ref(CanIfCtrlCanCtrlRef)/CanWakeupSupport = 'true'"!][!/*
            */!][!VAR "tempRet" = "'true'"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDLOOP!][!/*
      */!][!ELSE!][!/*
        */!][!IF "CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR422' or CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR431' or CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR440'"!][!/*
          */!][!LOOP "node:refs(CanIfCtrlDrvCfg/*[num:i($idx + 1)]/CanIfCtrlDrvNameRef)/../CanConfigSet/CanController/*"!][!/*
            */!][!IF "./CanWakeupSupport = 'true'"!][!/*
              */!][!VAR "tempRet" = "'true'"!][!/*
            */!][!ENDIF!][!/*
          */!][!ENDLOOP!][!/*
        */!][!ELSE!][!/*
          */!][!LOOP "node:refs(CanIfCtrlDrvCfg/*[num:i($idx + 1)]/CanIfCtrlDrvNameRef)/../CanConfigSet/*/CanController/*"!][!/*
            */!][!IF "./CanWakeupSupport = 'true'"!][!/*
              */!][!VAR "tempRet" = "'true'"!][!/*
            */!][!ENDIF!][!/*
          */!][!ENDLOOP!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!"$tempRet"!][!/*
*/!][!ENDMACRO!][!/*

*** GetDriverHohOffset: Returns the HOH Id offset based on the driver Id ***
*/!][!MACRO "GetDriverHohOffset", "idx"!][!/*
    */!][!VAR "tempPos" = "text:split($driverInfo, '#')[position() = ($idx + 1)]"!][!/*
    */!][!"text:split($tempPos, '|')[3]"!][!/*
*/!][!ENDMACRO!][!/*


*** AddBitwiseOred : Add the second paramter to the list given in the first
    parameter by attaching it with "|" at the end or replacing the 0 at the
    start of the C expression and return the expression.
    This can be used to calculate e.g. the C string to define the wakeup
    validation sources of a CAN controller
    Please note that before adding a new element it is checked, that the
    element is not yet present. ***
*/!][!MACRO "AddBitwiseOred", "List", "NewElement"!][!/*
   */!][!IF "($List = '0U') or ($List = '0') or ($List = '')"!][!/* first list element
      */!][!"$NewElement"!][!/*
   */!][!ELSE!][!/* new list element
      */!][!IF "not( text:match( $List, concat( '\b', $NewElement, '\b' ) ) )"!][!/*
         */!][!"concat( $List, " | ", $NewElement )"!][!/*
      */!][!ELSE!][!/* element already in the list
         */!][!"$List"!][!/*
      */!][!ENDIF!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*


*** End of file ***
*/!][!ENDIF!][!//
