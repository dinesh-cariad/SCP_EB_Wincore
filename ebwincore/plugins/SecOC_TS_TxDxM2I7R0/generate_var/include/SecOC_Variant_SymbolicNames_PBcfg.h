/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

[!IF "var:defined('postBuildVariant')"!]
  [!/* Current PB variant Sufix
  */!][!VAR "SECOC_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef SECOC_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define SECOC_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
  [!/* Current PB variant Sufix
  */!][!VAR "SECOC_VARIANT_SUFIX"="string("")"!]
#ifndef SECOC_SYMBOLICNAMES_PBCFG_H
#define SECOC_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

/*==================[includes]===============================================*/

/*==================[macros]=================================================*/

/*------------------[symbolic name values]----------------------------------*/

[!SELECT "as:modconf('SecOC')"!][!//

/** \brief Export symbolic name values for Rx Secured Layer PDU IDs */

[!LOOP "node:order(SecOCRxPduProcessing/*)"!][!//
[!IF "SecOCRxSecuredPduLayer = 'SecOCRxSecuredPdu'"!][!//
/** \brief Export symbolic name value for Rx Secured Layer, the Secured PDU ID [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCRxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCRxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCRxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxSecuredLayerPduId)"!]U
[!ELSE!][!//
/** \brief Export symbolic name value for Rx Secured Layer, the Authentic PDU ID [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCRxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCRxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCRxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxAuthenticPdu/SecOCRxAuthenticPduId)"!]U

/** \brief Export symbolic name value for Rx Secured Layer, the Cryptographic PDU ID [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCRxCryptographicPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCRxCryptographicPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCRxCryptographicPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCRxSecuredPduLayer/SecOCRxCryptographicPdu/SecOCRxCryptographicPduId)"!]U
[!ENDIF!][!//
[!ENDLOOP!][!//


/** \brief Export symbolic name values for Tx Secured Layer PDUs */

[!LOOP "node:order(SecOCTxPduProcessing/*, 'SecOCTxAuthenticPduLayer/SecOCTxAuthenticLayerPduId')"!][!//
[!IF "SecOCTxSecuredPduLayer = 'SecOCTxSecuredPdu'"!][!//
/** \brief Export symbolic name value for Tx secured Layer PDU Id [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCTxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCTxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCTxSecuredLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxSecuredLayerPduId)"!]U
[!ELSE!][!//
/** \brief Export symbolic name value for Tx secured Layer authentic PDU Id [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCTxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCTxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCTxAuthenticPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxAuthenticPdu/SecOCTxAuthenticPduId)"!]U

/** \brief Export symbolic name value for Tx cryptographic Layer PDU Id [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCTxCryptographicPduId_[!"node:name(.)"!])
#error SecOCConf_SecOCTxCryptographicPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCTxCryptographicPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCTxSecuredPduLayer/SecOCTxCryptographicPdu/SecOCTxCryptographicPduId)"!]U
[!ENDIF!][!//
/** \brief Export symbolic name value for Tx authentic Layer PDU Id [!"node:value(./SecOCTxAuthenticPduLayer/SecOCTxAuthenticLayerPduId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCTxAuthenticLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCTxAuthenticLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCTxAuthenticLayerPduId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCTxAuthenticPduLayer/SecOCTxAuthenticLayerPduId)"!]U

[!ENDLOOP!][!//


/** \brief Export symbolic name values for Rx Freshness Value IDs */

[!LOOP "node:order(SecOCRxPduProcessing/*, 'SecOCFreshnessValueId')"!][!//
/** \brief Export symbolic name value for freshness value ID [!"node:value(./SecOCFreshnessValueId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!])
#error SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCFreshnessValueId)"!]U

[!ENDLOOP!][!//


/** \brief Export symbolic name values for Tx Freshness Value IDs */

[!LOOP "node:order(SecOCTxPduProcessing/*, 'SecOCFreshnessValueId')"!][!//
/** \brief Export symbolic name value for freshness value ID [!"node:value(./SecOCFreshnessValueId)"!] of [!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!]
 */
#if (defined SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!])
#error SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] already defined
#endif
#define SecOCConf_SecOCFreshnessValueId_[!"node:name(.)"!][!"$SECOC_VARIANT_SUFIX"!] [!"node:value(./SecOCFreshnessValueId)"!]U

[!ENDLOOP!][!//

[!ENDSELECT!][!//

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/
[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef SECOC_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef SECOC_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]============================================*/
[!ENDCODE!]
