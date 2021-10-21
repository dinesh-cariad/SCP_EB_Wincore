[!/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHIF_LINKTIME_M'))"!]
[!VAR "ETHIF_LINKTIME_M"="'true'"!][!/*

=== Link time macros ===

*/!][!VAR "EthIf_MaxFrameTypeOwner" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfFrameOwnerConfig/*))"!][!//
[!VAR "EthIf_MaxUpTxConfirmation" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfTxConfirmationConfig/*))"!][!//
[!VAR "EthIf_MaxUpTrcvLinkStateChg" = "num:i(count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfTrcvLinkStateChgConfig/*))"!][!//
[!ENDIF!][!// ETHIF_LINKTIME_M
