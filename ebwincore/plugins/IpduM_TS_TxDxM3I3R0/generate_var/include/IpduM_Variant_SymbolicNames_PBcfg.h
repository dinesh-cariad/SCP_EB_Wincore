/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!INCLUDE "../../generate_macros/IpduM_Macros.m"!]

[!IF "var:defined('postBuildVariant')"!]
[!VAR "IPDUM_VARIANT_SUFFIX"="concat('_',$postBuildVariant)"!]
[!VAR "inclGuard" = "concat('IPDUM_SYMBOLICNAMES',$postBuildVariant,'_PBCFG_H')"!]
[!ELSE!]
[!VAR "IPDUM_VARIANT_SUFFIX"="''"!]
[!VAR "inclGuard" = "'IPDUM_SYMBOLICNAMES_PBCFG_H'"!]
[!ENDIF!]
#ifndef [!"$inclGuard"!]
#define [!"$inclGuard"!]

/*==================[inclusions]============================================*/

/*==================[macros]================================================*/

/************************/
/* Symbolic name values */
/************************/
[!INDENT "0"!][!//
[!SELECT "IpduMConfig/*[1]"!][!//
  [!IF "node:exists(IpduMRxPathway/*)"!][!//
    [!LOOP "IpduMRxPathway/*"!]
      /*################################################################################################*/
      /* Rx pathway: [!"name(.)"!] */
      [!CALL "GetSymbolName",
        "ShortNameRef"="'./IpduMRxIndication'",
        "OldName"="name(.)"!][!//
      [!CALL "GenSymbols",
        "SymbolicPrefix"="'IpduMConf_IpduMRxIndication_'",
        "SymbolicName"="$SymbolName",
        "SymbolicIdName"="'IpduMRxHandleId'",
        "SymbolicValue"="node:path(./IpduMRxIndication/IpduMRxHandleId)",
        "PduName"="name(node:ref(./IpduMRxIndication/IpduMRxIndicationPduRef))",
        "VariantSuffix"="$IPDUM_VARIANT_SUFFIX"!][!//
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMTxPathway/*)"!][!//
    [!LOOP "IpduMTxPathway/*"!][!//
      /*################################################################################################*/
      /* Tx pathway: [!"name(.)"!] */
      [!IF "node:exists(./IpduMTxRequest/IpduMTxStaticPart)"!][!//
        [!CALL "GetSymbolName",
          "ShortNameRef"="'./IpduMTxRequest/IpduMTxStaticPart'",
          "OldName"="name(.)"!][!//
        [!CALL "GenSymbols",
          "SymbolicPrefix"="'IpduMConf_IpduMTxStaticPart_'",
          "SymbolicName"="$SymbolName",
          "SymbolicIdName"="'IpduMTxStaticHandleId'",
          "SymbolicValue"="node:path(./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticHandleId)",
          "PduName"="name(node:ref(./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef))",
          "VariantSuffix"="$IPDUM_VARIANT_SUFFIX"!][!//
      [!ENDIF!][!//
      [!LOOP "IpduMTxRequest/IpduMTxDynamicPart/*"!][!//
        [!CALL "GenSymbols",
          "SymbolicPrefix"="'IpduMConf_IpduMTxDynamicPart_'",
          "SymbolicName"="name(.)",
          "SymbolicIdName"="'IpduMTxDynamicHandleId'",
          "SymbolicValue"="node:path(./IpduMTxDynamicHandleId)",
          "PduName"="name(node:ref(./IpduMTxDynamicPduRef))",
          "VariantSuffix"="$IPDUM_VARIANT_SUFFIX"!][!//
      [!ENDLOOP!][!//
      [!CALL "GetSymbolName",
        "ShortNameRef"="'./IpduMTxRequest'",
        "OldName"="name(.)"!][!//
      [!CALL "GenSymbols",
        "SymbolicPrefix"="'IpduMConf_IpduMTxRequest_'",
        "SymbolicName"="$SymbolName",
        "SymbolicIdName"="'IpduMTxConfirmationPduId'",
        "SymbolicValue"="node:path(./IpduMTxRequest/IpduMTxConfirmationPduId)",
        "PduName"="name(node:ref(./IpduMTxRequest/IpduMOutgoingPduRef))",
        "VariantSuffix"="$IPDUM_VARIANT_SUFFIX",
        "Legacy"="false()"!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
  [!IF "node:exists(IpduMContainerRxPdu/*)"!][!//
    /*################################################################################################*/
    [!LOOP "IpduMContainerRxPdu/*"!]
      /* IpduMContainerRxPdu: [!"name(.)"!] */
      [!CALL "GenSymbols",
        "SymbolicPrefix"="'IpduMConf_IpduMContainerRxPdu_'",
        "SymbolicName"="name(.)",
        "SymbolicIdName"="'IpduMContainerRxHandleId'",
        "SymbolicValue"="node:path(./IpduMContainerRxHandleId)",
        "PduName"="name(node:ref(./IpduMContainerRxPduRef))",
        "VariantSuffix"="$IPDUM_VARIANT_SUFFIX",
        "Legacy"="false()"!][!//
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMContainedTxPdu/*)"!][!//
    /*################################################################################################*/
    [!LOOP "IpduMContainedTxPdu/*"!]
      /* IpduMContainedTxPdu: [!"name(.)"!] */
      [!CALL "GenSymbols",
        "SymbolicPrefix"="'IpduMConf_IpduMContainedTxPdu_'",
        "SymbolicName"="name(.)",
        "SymbolicIdName"="'IpduMContainedTxPduHandleId'",
        "SymbolicValue"="node:path(./IpduMContainedTxPduHandleId)",
        "PduName"="name(node:ref(./IpduMContainedTxPduRef))",
        "VariantSuffix"="$IPDUM_VARIANT_SUFFIX",
        "Legacy"="false()"!][!//
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMContainerTxPdu/*)"!][!//
    /*################################################################################################*/
    [!LOOP "IpduMContainerTxPdu/*"!]
      [!IF "node:exists(./IpduMContainerTxHandleId)"!][!//
      /* IpduMContainerTxPdu: [!"name(.)"!] */
      [!CALL "GenSymbols",
        "SymbolicPrefix"="'IpduMConf_IpduMContainerTxPdu_'",
        "SymbolicName"="name(.)",
        "SymbolicIdName"="'IpduMContainerTxHandleId'",
        "SymbolicValue"="node:path(./IpduMContainerTxHandleId)",
        "PduName"="name(node:ref(./IpduMContainerTxPduRef))",
        "VariantSuffix"="$IPDUM_VARIANT_SUFFIX",
        "Legacy"="false()"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDSELECT!][!//
[!ENDINDENT!][!//

/*****************************************************/
/* Vendor specific outgoing and confirmation PDU IDs */
/*****************************************************/
#if (defined IPDUM_PROVIDE_ADJACENT_MODULE_VENDOR_SYMBOLIC_NAMES)
[!INDENT "0"!][!//
[!SELECT "IpduMConfig/*[1]"!][!//
  [!IF "node:exists(IpduMRxPathway/*)"!][!//
    [!LOOP "IpduMRxPathway/*"!]
     /*################################################################################################*/
      /* Rx pathway: [!"name(.)"!] */
      [!IF "node:exists(./IpduMRxIndication/IpduMRxStaticPart)"!][!/*
        */!][!CALL "GetOutputPduId","IpduMOutgoingPduRef"="IpduMRxIndication/IpduMRxStaticPart/IpduMOutgoingStaticPduRef"!]
        /* [!"node:path(IpduMRxIndication/IpduMRxStaticPart/IpduMOutgoingStaticPduRef)"!] */
[!VAR "SymbolName" = "concat('IPDUM_RX_OUTGOING_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
        #define [!"$SymbolName"!] [!"num:i($PduID)"!]U /* [!"name(node:ref(IpduMRxIndication/IpduMRxStaticPart/IpduMOutgoingStaticPduRef))"!] Static Rx */
      [!ENDIF!][!//
      [!LOOP "IpduMRxIndication/IpduMRxDynamicPart/*"!]
        [!CALL "GetOutputPduId","IpduMOutgoingPduRef"="IpduMOutgoingDynamicPduRef"!][!/*
        */!]/* [!"node:path(IpduMOutgoingDynamicPduRef)"!] */
[!VAR "SymbolName" = "concat('IPDUM_RX_OUTGOING_ID_', name(./../../..), '_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
        #define [!"$SymbolName"!] [!"num:i($PduID)"!]U /* [!"name(node:ref(IpduMOutgoingDynamicPduRef))"!] Dynamic Rx */
      [!ENDLOOP!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
  [!IF "node:exists(IpduMContainedRxPdu/*)"!][!//
    /*################################################################################################*/
    [!LOOP "IpduMContainedRxPdu/*"!]
      /* IpduMContainedRxPdu: [!"name(.)"!] */
      [!CALL "GetOutputPduId","IpduMOutgoingPduRef"="./IpduMContainedRxPduRef"!][!/*
      */!]
[!VAR "SymbolName" = "concat('IPDUM_RX_OUTGOING_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
        #define [!"$SymbolName"!] [!"num:i($PduID)"!]U
    [!ENDLOOP!][!//
  [!ENDIF!][!//
  [!IF "node:exists(IpduMContainerTxPdu/*)"!]
    /*################################################################################################*/
    [!LOOP "IpduMContainerTxPdu/*"!]
      /* IpduMContainerTxPdu: [!"name(.)"!] */
      [!CALL "GetOutputPduId", "IpduMOutgoingPduRef"="./IpduMContainerTxPduRef"!][!//
[!VAR "SymbolName" = "concat('IPDUM_TX_OUTGOING_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
      #define [!"$SymbolName"!] [!"num:i($PduID)"!]U  /* [!"name(node:ref(./IpduMContainerTxPduRef))"!] Tx */
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMContainedTxPdu/*)"!]
    /*################################################################################################*/
    [!LOOP "IpduMContainedTxPdu/*"!]
      /* IpduMContainedTxPdu: [!"name(.)"!] */
      [!CALL "GetResponsePduId", "IpduMPduRef"="./IpduMContainedTxPduRef"!][!//
[!VAR "SymbolName" = "concat('IPDUM_TX_CONFIRM_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
      #define [!"$SymbolName"!] [!"num:i($ResponsePduID)"!]U  /* [!"name(node:ref(./IpduMContainedTxPduRef))"!] Tx */
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMContainerTxPdu/*)"!]
    /*################################################################################################*/
    [!LOOP "IpduMContainerTxPdu/*"!]
      /* IpduMContainerTxPdu: [!"name(.)"!] */
      [!CALL "GetResponsePduId", "IpduMPduRef"="./IpduMContainerTxPduRef"!][!//
[!VAR "SymbolName" = "concat('IPDUM_TX_CONFIRM_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
      #define [!"$SymbolName"!] [!"num:i($ResponsePduID)"!]U  /* [!"name(node:ref(./IpduMContainerTxPduRef))"!] Tx */
    [!ENDLOOP!]
  [!ENDIF!]
  [!IF "node:exists(IpduMTxPathway/*)"!][!//
    [!LOOP "IpduMTxPathway/*"!]
     /*################################################################################################*/
      /* Tx pathway: [!"name(.)"!] */
      [!IF "node:exists(./IpduMTxRequest/IpduMTxStaticPart)"!][!/*
        */!][!IF "./IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticConfirmation = 'true'"!][!/*
          */!][!CALL "GetResponsePduId","IpduMPduRef"="IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef"!]
              /* [!"node:path(IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef)"!] */
[!VAR "SymbolName" = "concat('IPDUM_TX_CONFIRM_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
              #define [!"$SymbolName"!] [!"num:i($ResponsePduID)"!]U /* [!"name(node:ref(IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef))"!] Static Tx */
            [!ELSE!][!//
              /* [!"name(node:ref(IpduMTxRequest/IpduMTxStaticPart/IpduMTxStaticPduRef))"!] Disabled static TxConfirmation */
            [!ENDIF!][!//
      [!ENDIF!][!//
      [!LOOP "IpduMTxRequest/IpduMTxDynamicPart/*"!][!/*
        */!][!IF "./IpduMTxDynamicConfirmation = 'true'"!][!/*
          */!][!CALL "GetResponsePduId","IpduMPduRef"="./IpduMTxDynamicPduRef"!]
              /* [!"node:path(IpduMTxDynamicPduRef)"!] */
[!VAR "SymbolName" = "concat('IPDUM_TX_CONFIRM_ID_', name(./../../..), '_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
              #define [!"$SymbolName"!] [!"num:i($ResponsePduID)"!]U /* [!"name(node:ref(IpduMTxDynamicPduRef))"!] Dynamic Tx */
            [!ELSE!][!//
                 /* [!"name(node:ref(IpduMTxDynamicPduRef))"!]  Disabled dynamic TxConfirmation */
            [!ENDIF!][!//
      [!ENDLOOP!][!/*
        */!][!CALL "GetOutputPduId","IpduMOutgoingPduRef"="./IpduMTxRequest/IpduMOutgoingPduRef"!]
        /* [!"node:path(./IpduMTxRequest/IpduMOutgoingPduRef)"!] */
[!VAR "SymbolName" = "concat('IPDUM_TX_OUTGOING_ID_', name(.), $IPDUM_VARIANT_SUFFIX)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] already defined!
#endif /* #if (defined [!"$SymbolName"!])*/
        #define [!"$SymbolName"!] [!"num:i($PduID)"!]U  /* [!"name(node:ref(./IpduMTxRequest/IpduMOutgoingPduRef))"!] Tx */
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDSELECT!][!//
[!ENDINDENT!][!//
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef [!"$inclGuard"!] */
/*==================[end of file]===========================================*/
