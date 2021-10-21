/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!][!//
[!AUTOSPACING!][!//

[!IF "var:defined('postBuildVariant')"!][!//
[!/* Current PB variant Sufix
*/!][!VAR "DOIP_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!][!//

#ifndef DOIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define DOIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!][!//
[!/* Current PB variant Sufix
*/!][!VAR "DOIP_VARIANT_SUFIX"="string("")"!][!//
#ifndef DOIP_SYMBOLICNAMES_PBCFG_H
#define DOIP_SYMBOLICNAMES_PBCFG_H
[!ENDIF!][!//

/*==================[includes]==============================================*/

/*==================[macros]================================================*/
[!/* === Array for holding Shortnames === */!]
[!VAR "ShortNameList"="''"!]

[!/* === MACRO to get the string for the symbolic name value. ===
 * "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
 * "OldName" must hold an unique string for the case that the specification */!]
[!MACRO "GetSymbolName","ShortNameRef","OldName"!]
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!]
  [!IF "$SymbolName = ''"!]
    [!VAR "SymbolName" = "concat(name(../.),'_',$OldName)"!]
  [!ELSE!]
    [!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!]
    [!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!]
      [!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!]
    [!ENDIF!]
    [!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!]
  [!ENDIF!]
  [!VAR "SymbolName" = "concat($SymbolName, $DOIP_VARIANT_SUFIX)"!]
[!ENDMACRO!]

/** \brief Symbolic name value for UDP connections */
[!LOOP "DoIPConfigSet/*[1]/DoIPConnections/DoIPUdpConnection/*"!]
[!SELECT "DoIPSoAdRxPdu"!]

/* === UDP Rx ID === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPSoAdRxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPSoAdRxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]

[!SELECT "DoIPSoAdTxPdu"!]

/* === UDP Tx ID === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPSoAdTxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPSoAdTxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]
[!ENDLOOP!]

/** \brief Symbolic name value for UDP vehicle announcement connections */
[!LOOP "DoIPConfigSet/*[1]/DoIPConnections/DoIPUdpVehicleAnnouncement/*"!]
[!SELECT "DoIPSoAdTxPdu"!]

/* === UDP Tx ID === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPSoAdTxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPSoAdTxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]
[!ENDLOOP!]

/* ============================================================================================== */

/** \brief Symbolic name value for TCP connections */
[!LOOP "DoIPConfigSet/*[1]/DoIPConnections/DoIPTcpConnection/*"!]
[!SELECT "DoIPSoAdRxPdu"!]

/* === TCP Rx ID === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPSoAdRxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPSoAdRxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPSoAdRxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]

[!SELECT "DoIPSoAdTxPdu"!]

/* === TCP Tx ID === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPSoAdTxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPSoAdTxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPSoAdTxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]
[!ENDLOOP!]

/* ============================================================================================== */

/** \brief Symbolic name value for channel handle ID for PduR */
[!LOOP "DoIPConfigSet/*[1]/DoIPChannel/*"!]
[!SELECT "DoIPPduRRxPdu"!]

/* === Rx ID for PduR === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPPduRRxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPPduRRxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPPduRRxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPPduRRxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPPduRRxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]

[!IF "node:exists(DoIPPduRTxPdu)"!]
[!SELECT "DoIPPduRTxPdu"!]

/* === Tx ID for PduR === */
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(.)"!]
#if (defined DoIPConf_[!"$SymbolName"!])
#error DoIPConf_[!"$SymbolName"!] is already defined
#endif
#define DoIPConf_[!"$SymbolName"!] [!"./DoIPPduRTxPduId"!]U

#if(defined DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined DoIP_[!"../@name"!]_DoIPPduRTxPdu[!"$DOIP_VARIANT_SUFIX"!])
#error DoIP_[!"../@name"!]_DoIPPduRTxPdu[!"$DOIP_VARIANT_SUFIX"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 *         (AUTOSAR 4.0 rev2 <= AUTOSAR version )
 */
#define DoIP_[!"../@name"!]_DoIPPduRTxPdu[!"$DOIP_VARIANT_SUFIX"!] [!"./DoIPPduRTxPduId"!]U
#endif /* DOIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDSELECT!]
[!ENDIF!]
[!ENDLOOP!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

[!IF "var:defined('postBuildVariant')"!][!//
#endif /* ifndef DOIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!][!//
#endif /* ifndef DOIP_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!][!//

/*==================[end of file]===========================================*/
[!ENDCODE!]
