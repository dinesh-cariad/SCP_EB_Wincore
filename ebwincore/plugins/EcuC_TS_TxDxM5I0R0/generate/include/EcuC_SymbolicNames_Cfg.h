/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
#ifndef ECUC_SYMBOLICNAMES_CFG_H
#define ECUC_SYMBOLICNAMES_CFG_H

[!INCLUDE "../include/EcuC_Vars.m"!][!//
[!INCLUDE "CommonMacros.m"!]

/*==================[macros]=================================================*/

/*------------------[symbolic name definitions]------------------------------*/
[!LOOP "(as:modconf("EcuC")[1]/EcucPduCollection/Pdu/*)"!][!//
[!IF "node:exists(PduId)"!][!//

[!CALL "GuardedDefine", "Comment"="concat('SymbolicName value for ',name(.))",
  "Name"="concat('EcuCConf_PduId_',name(.))"!][!"PduId"!]U

[!ENDIF!][!//
[!ENDLOOP!][!//

#endif /* ECUC_SYMBOLICNAMES_CFG_H */
[!ENDCODE!][!//
