/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DCM_TYPES_CFG_H
#define DCM_TYPES_CFG_H

/*==================[includes]===================================================================*/
/*==================[macros]=====================================================================*/

/* !LINKSTO Dcm.EB.SesCtrlType.RteUsageFalse,1 */
#if (DCM_INCLUDE_RTE == STD_OFF)
/*------------------[definitions of session levels]----------------------------------------------*/
[!LOOP "DcmConfigSet/*[1]/DcmDsp/DcmDspSession/*/DcmDspSessionRow/*"!][!//
#if (defined DcmConf_[!"name(../../.)"!]_[!"./@name"!])
#error DcmConf_[!"name(../../.)"!]_[!"./@name"!] already defined
#endif
/** \brief Export symbolic name value */
#define DcmConf_[!"name(../../.)"!]_[!"@name"!] [!"num:i(./DcmDspSessionLevel)"!]U

#if (defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined Dcm_[!"name(.)"!])
#error Dcm_[!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only
 **        (AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define Dcm_[!"@name"!] [!"num:i(./DcmDspSessionLevel)"!]U
#endif /* defined DCM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!][!//
#endif /* #if (DCM_INCLUDE_RTE == STD_OFF) */

/*==================[type definitions]===========================================================*/
/*==================[external function declarations]=============================================*/
/*==================[internal function declarations]=============================================*/
/*==================[external constants]=========================================================*/
/*==================[internal data]==============================================================*/
/*==================[external function definitions]==============================================*/
/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_TYPES_CFG_H */
/*==================[end of file]================================================================*/
