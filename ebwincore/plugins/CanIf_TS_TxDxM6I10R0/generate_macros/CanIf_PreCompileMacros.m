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
*/!][!IF "not(var:defined('CANIF_PRECOMPILEMACROS_M'))"!][!//
[!VAR "CANIF_PRECOMPILEMACROS_M"="'true'"!][!/*


*** transceiver configuration ***
*/!][!VAR "trcvDriversNum" = "num:i(count(CanIfTrcvDrvCfg/*))"!][!/*

*** ASR for the CAN driver usage ***
*/!][!VAR "canDriverCompatibility" = "CanIfPublicCfg/CanIfCanDriverCompatibility"!][!/*

*** CAN driver wakeup support configured ***
*/!][!VAR "canWakeupSupport" = "(CanIfPublicCfg/CanIfPublicCtrlWakeupSupport = 'true')"!][!/*

*** CAN transceiver support configured ***
*/!][!VAR "canTrcvSupport" = "(CanIfPublicCfg/CanIfPublicTrcvSupport = 'true')"!][!/*

*** CAN transceiver wakeup support configured ***
*/!][!VAR "canTrcvWakeupSupport" = "(CanIfPublicCfg/CanIfPublicTrcvWakeupSupport = 'true')"!][!/*

*** Global wakeup support enabled ***
*/!][!VAR "wakeupNotificationCalled" = "($canWakeupSupport = 'true') or ($canTrcvWakeupSupport = 'true')"!][!/*

*** Wakeup validation support enabled ***
*/!][!VAR "wakeupValidationCalled" = "($wakeupNotificationCalled = 'true') and (CanIfPublicCfg/CanIfPublicWakeupCheckValidSupport = 'true')"!][!/*

*** DLC error notifcation enabled ***
*/!][!VAR "dlcErrorNotifSupport" = "(num:i(count(CanIfUpperLayerConfig/*[node:exists(CanIfUserDlcErrorNotification)])) > 0)"!][!/*

*** DLC passed notifcation enabled ***
*/!][!VAR "dlcPassedNotifSupport" = "(num:i(count(CanIfUpperLayerConfig/*[node:exists(CanIfUserDlcPassedNotification)])) > 0)"!][!/*

*** <UL>_RxIndication global enable ***
*/!][!VAR "ulRxIndicationSupport" = "(num:i(count(CanIfUpperLayerConfig/*[(node:exists(CanIfRxPduUserRxIndicationName)) and (CanIfUpperLayerUseCanId = 'false')])) > 0)"!][!/*

*** <Cdd>_RxIndication global enable ***
*/!][!VAR "cddRxIndicationSupport" = "(num:i(count(CanIfUpperLayerConfig/*[(node:exists(CanIfRxPduUserRxIndicationName)) and (CanIfUpperLayerUseCanId = 'true')])) > 0)"!][!/*

*** <UL>_TxConfirmation global enable ***
*/!][!VAR "ulTxConfirmationSupport" = "(num:i(count(CanIfUpperLayerConfig/*[node:exists(CanIfTxPduUserTxConfirmationName)])) > 0)"!][!/*

*** <UL>_TxConfirmation global enable ***
*/!][!VAR "ulTxConfirmationResutSupported" = "(num:i(count(CanIfUpperLayerConfig/*[CanIfPublicTxConfResultSupport = 'true'])) > 0)"!][!/*

*** Transceiver infix used ***
*/!][!VAR "useTransceiverInfix" = "(CanIfPublicCfg/CanIfSingleCanTrcvAPIInfixEnable = 'true') or ($trcvDriversNum > 1)"!][!/*

*** Public Tx Buffering supported ***
*/!][!VAR "publicTxBufferingSupport" = "CanIfPublicCfg/CanIfPublicTxBuffering"!][!/*

*** CanIf_ReadRxPduData API Supported ***
*/!][!VAR "canifReadRxPduDataApi" = "(CanIfPublicCfg/CanIfPublicReadRxPduDataApi = 'true')"!][!/*

*** CanIf_ReadRxNotifStatus API Supported ***
*/!][!VAR "canifReadRxNotifStatusApi" = "(CanIfPublicCfg/CanIfPublicReadRxPduNotifyStatusApi = 'true')"!][!/*

*** CanIf_ReadTxNotifStatus API Supported ***
*/!][!VAR "canifReadTxNotifStatusApi" = "(CanIfPublicCfg/CanIfPublicReadTxPduNotifyStatusApi = 'true')"!][!/*

*** CanIf_CancelTxConfirmation API Supported ***
*/!][!VAR "canifCancelTxConfSupported" = "node:containsValue(CanIfCtrlDrvCfg/*/CanIfCtrlDrvTxCancellation, 'true')"!][!/*

*** Range reception support ***
*/!][!VAR "rangeReceptionUsed" = "(CanIfPublicCfg/CanIfPublicRangeReceptionSupport = 'true')"!][!/*

*** Partial Networking support ***
*/!][!VAR "canifPNSupport" = "CanIfPublicCfg/CanIfPublicPnSupport = 'true'"!][!/*

*** DLC Check Used ***
*/!][!VAR "canifDLCCheck" = "CanIfPrivateCfg/CanIfPrivateDlcCheck = 'true'"!][!/*

*** Single CAN Controller Optimization ***
*/!][!VAR "canifSingleCtrlOpt" = "CanIfPublicCfg/CanIfPublicSingleCtrlOpt = 'true'"!][!/*

*** HOH Translation Optimization ***
*/!][!VAR "canifHOHTransOpt" = "CanIfPublicCfg/CanIfPublicHohTranslationOpt = 'true'"!][!/*

*** Det support ***
*/!][!VAR "canifDetEnabled" = "CanIfPublicCfg/CanIfPublicDevErrorDetect = 'true'"!][!/*

*** MainFunction support ***
*/!][!VAR "canifMFSupport" = "CanIfPublicCfg/CanIfDecoupledProcessingSupport = 'true'"!][!/*

*** Rx decoupled measurement support ***
*/!][!VAR "canifRxDecMeasureSupport" = "CanIfDecoupledMeasurementSupport/CanIfRxDecoupledMeasurementSupport = 'true'"!][!/*

*** Tx decoupled measurement support ***
*/!][!VAR "canifTxDecMeasureSupport" = "CanIfDecoupledMeasurementSupport/CanIfTxDecoupledMeasurementSupport = 'true'"!][!/*

*** Wakeup validation on controller started support ***
*/!][!VAR "canifValidateWakeupOnStartedCtrlSupport" = "CanIfPublicCfg/CanIfValidateWakeupOnStartedCtrlOnly = 'true'"!][!/*

*** CanIf_SetDynamicTxId() supported ***
*/!][!VAR "canifSetDynTxIdSupported" = "CanIfPublicCfg/CanIfPublicSetDynamicTxIdApi"!][!/*

*** Multiple driver support ***
*/!][!VAR "canIfPublicMultipleDrvSupport" = "CanIfPublicCfg/CanIfPublicMultipleDrvSupport"!][!/*

*** Mirroring support ***
*/!][!VAR "canIfBusMirrorSupport" = "CanIfPublicCfg/CanIfBusMirroringSupport"!][!/*
*/!][!IF "$canIfBusMirrorSupport = 'true'"!][!/*
*** Can_GetControllerTxErrorCounter supported by driver ***
*/!][!VAR "canIfCanTxErrCntSupported" = "CanIfMirroringSupport/CanIfCanTxErrorCounterSupported"!][!/*
*/!][!VAR "canIfCanDummyTxErrorCount" = "num:i(node:when($canIfCanTxErrCntSupported = 'false', CanIfMirroringSupport/CanIfTxErrorCounterValue, 0))"!][!/*
*** Can_GetControllerErrorState supported by driver ***
*/!][!VAR "canIfCanGetCntrlStateSupported" = "CanIfMirroringSupport/CanIfCanControllerErrorStateSupported"!][!/*
*/!][!VAR "canIfCanDummyErrorState" = "node:when($canIfCanGetCntrlStateSupported = 'false', CanIfMirroringSupport/CanIfErrorStateValue, num:i(0))"!][!/*
*** Size of Tx Mirror Buffer ***
*/!][!VAR "canIfTxMirrorBufSize" = "num:i(CanIfMirroringSupport/TxMirrorBufferSize)"!][!/*
*/!][!VAR "canIfTxMirrorSize" = "num:i(CanIfMirroringSupport/TxMirrorNumTxPdus)"!][!/*
*/!][!ENDIF!][!/* $canIfBusMirrorSupport = 'true' */!][!/*

*** Hook on reception support ***
*/!][!VAR "canIfHookOnReceptionSupport" = "CanIfPublicCfg/CanIfHookOnReceptionSupport"!][!/*

*** Metadata support ***
*/!][!VAR "CanIfMetaDataSupport" = "CanIfPublicCfg/CanIfMetaDataSupport"!][!/*

*** SW filtering enable ***
*/!][!VAR "CanIfSWFilter" = "CanIfPublicCfg/CanIfSoftwareFilteringSupport"!][!/*

*** create upper layer callback lookup table ***
Notes:
- The upper layer callback function lookup table has the following format:
  <upper layer name>:<internal index of upper layer>:<RxIndication callback name>:<Cdd RxIndication callback name>:<TxConfirmation callback name>:<DLC fail function name>:<DLC pass function name>
*/!][!VAR "ulCbFuncLUT"="''"!][!/*
*/!][!VAR "rxCbTypeIndex"="0"!][!/* Index for different Rx upper layer types
*/!][!LOOP "CanIfUpperLayerConfig/*"!][!/*
   */!][!VAR "upperLayer" = "node:name(.)"!][!/* variable for the current upper layer

    * get name of RxIndication callback function *
    */!][!VAR "tmpRxCbFunc" = "'NULL_PTR'"!][!/* set default value
    */!][!VAR "tmpCddRxCbFunc" = "'NULL_PTR'"!][!/* set default value
    */!][!IF "node:exists(CanIfRxPduUserRxIndicationName)"!][!/*
      */!][!IF "CanIfUpperLayerUseCanId = 'false'"!][!/*
        */!][!VAR "tmpRxCbFunc"="concat('&', CanIfRxPduUserRxIndicationName)"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "tmpCddRxCbFunc"="concat('&', CanIfRxPduUserRxIndicationName)"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*

    * get name of TxConfirmation callback function *
    */!][!VAR "tmpTxCbFunc" = "'NULL_PTR'"!][!/* set default value
    */!][!VAR "tmpTxCbResultFunc" = "'NULL_PTR'"!][!/* set default value

    */!][!IF "node:exists(CanIfTxPduUserTxConfirmationName)"!][!/*
      */!][!IF "node:exists(CanIfPublicTxConfResultSupport) and CanIfPublicTxConfResultSupport = 'true'"!][!/*
        */!][!VAR "tmpTxCbResultFunc"="concat('&', CanIfTxPduUserTxConfirmationName)"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "tmpTxCbFunc"="concat('&', CanIfTxPduUserTxConfirmationName)"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*






    * get name of DLC check failed callback function *
    */!][!VAR "tmpDlcFailedCbFunc" = "'NULL_PTR'"!][!/*
    */!][!IF "node:exists(CanIfUserDlcErrorNotification)"!][!/*
       */!][!VAR "tmpDlcFailedCbFunc" = "concat('&', CanIfUserDlcErrorNotification)"!][!/*
    */!][!ENDIF!][!/*

    * get name of DLC check passed callback function *
    */!][!VAR "tmpDlcPassedCbFunc" = "'NULL_PTR'"!][!/*
    */!][!IF "node:exists(CanIfUserDlcPassedNotification)"!][!/*
       */!][!VAR "tmpDlcPassedCbFunc" = "concat('&', CanIfUserDlcPassedNotification)"!][!/*
    */!][!ENDIF!][!/*

    * concatenate string *
    */!][!VAR "ulCbFuncLUT"="concat($ulCbFuncLUT, ' ', $upperLayer, ':', num:i($rxCbTypeIndex), ':', $tmpRxCbFunc, ':', $tmpCddRxCbFunc, ':', $tmpTxCbFunc, ':', $tmpTxCbResultFunc, ':', $tmpDlcFailedCbFunc, ':', $tmpDlcPassedCbFunc)"!][!/*
    */!][!VAR "rxCbTypeIndex"="$rxCbTypeIndex + 1"!][!/*
*/!][!ENDLOOP!][!/*


*** create transceiver lookup table ***
Note:
- Infixes for usage in the API functions are stored as ";" separated list with
  the drivers being indexed as given in the CanIf Configuration.
- Infixes are stored only for those drivers, which have a channel assigned in
  the CanIf configuration. Other transceiver drivers are omitted.
*/!][!IF "$canTrcvSupport = 'true'"!][!/*
*/!][!VAR "trcvInfixList"="''"!][!/*
*/!][!/* *** include transceiver drivers if necessary *** */!][!/*
    Please note that in the loop not $trcvDriversNum is used, because there might be drivers configured without channels!
*/!][!FOR "i"="1" TO "num:i(count(CanIfTrcvDrvCfg/*))"!][!/*
    */!][!IF "node:exists(CanIfTrcvDrvCfg/*[position() = $i]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)"!][!/*

      */!][!IF "node:exists(as:ref(as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*[position() = $i]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../../../CanTrcvConfigSet)"!][!/*
          */!][!SELECT "as:ref(CanIfTrcvDrvCfg/*[position() = $i]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../../../CommonPublishedInformation"!][!/*
            */!][!VAR "trcvInfixList"!][!"$trcvInfixList"!];[!"VendorId"!]_[!IF "node:exists(VendorApiInfix)"!][!"VendorApiInfix"!][!ENDIF!][!ENDVAR!][!/*
          */!][!ENDSELECT!][!/*
      */!][!ELSE!][!/*
          */!][!SELECT "as:ref(CanIfTrcvDrvCfg/*[position() = $i]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../../CommonPublishedInformation"!][!/*
            */!][!VAR "trcvInfixList"!][!"$trcvInfixList"!];[!"VendorId"!]_[!IF "node:exists(VendorApiInfix)"!][!"VendorApiInfix"!][!ENDIF!][!ENDVAR!][!/*
          */!][!ENDSELECT!][!/*
      */!][!ENDIF!][!/*

    */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDIF!][!/* canTrcvSupport */!][!/*


*** CreateTrcvInfixByIdx: Creates Infix for transceiver API functions based
    on the CommonPublishedInformation of the transceiver selected by idx ***
*/!][!MACRO "CreateTrcvInfixByIdx", "idx"!][!/*
    */!][!"text:split($trcvInfixList, ';')[position() = ($idx + 1)]"!][!/*
*/!][!ENDMACRO!][!/*


*** GetPnSupportByIdx: check if partial network is enabled for the transceiver driver
    pointed by idx.
    partial network is enabled if one of the follwing criterias is true:
    1.) EB specific parameter CanTrcvPnSupport is set to true. This vendor specific parameter is
    used for all EB transceiver drivers with partial network capability (e.g. T03)
    2.) EB specific parameter CanTrcvHwPNSupportApiEnable is set to true. This vendor specific parameter is
    used for all EB transceiver drivers without partial network capability (e.g. T01, T02)
    3.) At least one CanTrcvChannel has the parameter CanTrcvHwPnSupport set to true.
    This is the AUTOSAR SWS parameter (maybe) used from 3rd party MCAL drivers. ***

*/!][!MACRO "GetPnSupportByIdx", "idx"!][!/*

  */!][!VAR "trcvPnSupport"="'false'"!][!/*

  */!][!IF "node:exists(as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*[position() = ($idx + 1)]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)"!][!/*

    */!][!IF "node:exists(as:ref(as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*[position() = ($idx + 1)]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../../../CanTrcvConfigSet)"!][!/*

  4.0.3 CanTrcv is used
      */!][!SELECT "as:ref(as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*[position() = ($idx + 1)]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../../.."!][!/*

        */!][!IF "node:exists(VendorSpecific/CanTrcvPnSupport)"!][!/*
          */!][!IF "VendorSpecific/CanTrcvPnSupport = 'true'"!][!/*
            */!][!VAR "trcvPnSupport"="'true'"!][!/*
          */!][!ENDIF!][!/*

        */!][!ELSEIF "node:exists(CanTrcvGeneral/VendorSpecific/CanTrcvHwPNSupportApiEnable)"!][!/*
          */!][!IF "CanTrcvGeneral/VendorSpecific/CanTrcvHwPNSupportApiEnable = 'true'"!][!/*
            */!][!VAR "trcvPnSupport"="'true'"!][!/*
          */!][!ENDIF!][!/*

        */!][!ELSE!][!/*
          */!][!IF "count(CanTrcvConfigSet/*[1]/CanTrcvChannel/*[CanTrcvHwPnSupport = 'true']) > 0"!][!/*
            */!][!VAR "trcvPnSupport"="'true'"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*

      */!][!ENDSELECT!][!/*

    */!][!ELSE!][!/*

  4.2.2 CanTrcv is used
    */!][!SELECT "as:ref(as:modconf('CanIf')[1]/CanIfTrcvDrvCfg/*[position() = ($idx + 1)]/CanIfTrcvCfg/*/CanIfTrcvCanTrcvRef)/../../.."!][!/*

       */!][!IF "node:exists(VendorSpecific/CanTrcvPnSupport)"!][!/*
         */!][!IF "VendorSpecific/CanTrcvPnSupport = 'true'"!][!/*
           */!][!VAR "trcvPnSupport"="'true'"!][!/*
         */!][!ENDIF!][!/*

       */!][!ELSEIF "node:exists(CanTrcvGeneral/VendorSpecific/CanTrcvHwPNSupportApiEnable)"!][!/*
         */!][!IF "CanTrcvGeneral/VendorSpecific/CanTrcvHwPNSupportApiEnable = 'true'"!][!/*
           */!][!VAR "trcvPnSupport"="'true'"!][!/*
         */!][!ENDIF!][!/*

       */!][!ELSE!][!/*
         */!][!IF "count(CanTrcvConfigSet/CanTrcvChannel/*[CanTrcvHwPnSupport = 'true']) > 0"!][!/*
           */!][!VAR "trcvPnSupport"="'true'"!][!/*
         */!][!ENDIF!][!/*
       */!][!ENDIF!][!/*

      */!][!ENDSELECT!][!/*

    */!][!ENDIF!][!/*

  */!][!ENDIF!][!/*

  */!][!"$trcvPnSupport"!][!/*

*/!][!ENDMACRO!][!/*

*** End of file ***
*/!][!ENDIF!][!//
