/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]

[!IF "var:defined('postBuildVariant')"!]
  [!/* Current PB variant Sufix
  */!][!VAR "TCPIP_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef TCPIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define TCPIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
  [!/* Current PB variant Sufix
  */!][!VAR "TCPIP_VARIANT_SUFIX"="string("")"!]
#ifndef TCPIP_SYMBOLICNAMES_PBCFG_H
#define TCPIP_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]


[!INCLUDE "../../generate_macros/TcpIp_Vars.m"!]

/*==================[macros]=================================================*/

/*------------------[symbolic name definitions]------------------------------*/
[!LOOP "TcpIpConfig/TcpIpLocalAddr/*"!][!//
[!CALL "GuardedDefine", "Comment"="concat('SymbolicName value for ',name(.))",
  "Name"="concat('TcpIpConf_TcpIpLocalAddr_',name(.),$TCPIP_VARIANT_SUFIX)"!][!"TcpIpAddrId"!]U

#if (defined TCPIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

[!CALL "GuardedDefine", "Comment"="concat('Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) for ',name(.))",
 "Indent"="'  '","Name"="concat('TcpIp_',name(.),$TCPIP_VARIANT_SUFIX)"!][!"TcpIpAddrId"!]U

#endif /* defined TCPIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!][!//

[!LOOP "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*)"!][!//
[!CALL "GuardedDefine", "Comment"="concat('SymbolicName value for ',name(.))",
  "Name"="concat('TcpIpConfig_TcpIpIpSecConfig_TcpIpIpSecConnections_',name(.),$TCPIP_VARIANT_SUFIX)"!][!"TcpIpIpSecConId"!]U

#if (defined TCPIP_PROVIDE_LEGACY_SYMBOLIC_NAMES)

[!CALL "GuardedDefine", "Comment"="concat('Export symbolic name value with module abbreviation as prefix only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) for ',name(.))",
 "Indent"="'  '","Name"="concat('TcpIp_',name(.),$TCPIP_VARIANT_SUFIX)"!][!"TcpIpIpSecConId"!]U

#endif /* defined TCPIP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!][!//

[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef TCPIP_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef TCPIP_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
[!ENDCODE!][!//
