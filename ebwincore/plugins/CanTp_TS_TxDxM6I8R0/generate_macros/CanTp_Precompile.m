[!/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('CANTP_PRECOMPILE_M'))"!][!/*
*/!][!VAR "CANTP_PRECOMPILE_M"="'true'"!][!/*


=============================================================================
Variables that use macros for their calculation
=============================================================================

*** General variables ***

*** API functions in jump table mode ***
*/!][!VAR "CanTpProvideApiFunctions" = "(CanTpJumpTable/CanTpJumpTableMode != 'CLIENT') or (CanTpJumpTable/CanTpUseWrapperMacros != 'true')"!][!/*



*** TS_CANTP_22062 ***
*/!][!IF "CanTpGeneral/CanTpTxPduOptimization = 'false'"!][!/*
    */!][!INFO "Tx PDU ID optimization is disabled. If you have many N-SDUs configured, please adapt the CanIf configuration in the way, that all Tx PDUs that are assigned to the CanTp (parameter CanIfTxUserType is set to CAN_TP) have consecutive IDs (defined in parameter CanIfTxPduId) in order to improve performance and enable the parameter CanTpTxPduOptimization."!][!/*
*/!][!ENDIF!][!/*

*** Calculation macros ***

*** macro for calculating the STmin integer representation from the given floating point value ***
*/!][!MACRO "calcSTmin", "value"!][!/*
   */!][!IF "$value*1000 > 0.9"!][!/*
      */!][!"num:i($value*1000 + 0.99999)"!][!/*
   */!][!ELSEIF "$value*10000 >= 0.00001"!][!/*
      */!][!"num:i($value*10000 + 0.99999 + 240)"!][!/*
   */!][!ELSE!][!/*
      */!]0[!/*
   */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*


*** Miscellaneous ***



*** End of file ***
*/!][!ENDIF!][!/*
*/!][!//
