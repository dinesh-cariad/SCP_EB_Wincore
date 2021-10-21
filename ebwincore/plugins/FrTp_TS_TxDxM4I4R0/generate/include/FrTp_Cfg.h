/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!INCLUDE "FrTp_Variables.m"!]
[!CODE!]

#ifndef FRTP_CFG_H
#define FRTP_CFG_H

/******************************************************************************
**                      Include Section                                      **
******************************************************************************/

#include <ComStack_Types.h> /* Communication stack types from AUTOSAR */

/******************************************************************************
**                      Global Macros                                        **
******************************************************************************/

/* standard SWS pre-compile time configuration parameters */
[!CALL "GuardedDefine", "Comment"="'Development error detection support'",
  "Name"="'FRTP_DEV_ERROR_DETECT'"!] [!IF "FrTpGeneral/FrTpDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Version info API support'",
  "Name"="'FRTP_VERSION_INFO_API'"!] [!IF "FrTpGeneral/FrTpVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'PBCFGM support'",
  "Name"="'FRTP_PBCFGM_SUPPORT_ENABLED'"!] [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('FrTp')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Acknowledgment and retry mechanisms support'",
  "Name"="'FRTP_HAVE_ACKRT'"!] [!IF "FrTpGeneral/FrTpAckRt = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Supported number of concurrent channels'",
  "Name"="'FRTP_CHAN_NUM'"!] [!"num:integer(FrTpGeneral/FrTpChanNum)"!]U
[!CALL "GuardedDefine", "Comment"="'Transmit cancellation support'",
  "Name"="'FRTP_HAVE_TC'"!] [!IF "FrTpGeneral/FrTpTransmitCancellation = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!CALL "GuardedDefine", "Comment"="'Support for data transfer with unknown message length'",
  "Name"="'FRTP_UNKNOWN_MSG_LENGTH'"!] [!IF "FrTpGeneral/FrTpUnknownMsgLength = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* vendor specific pre-compile time configuration parameters */
[!CALL "GuardedDefine", "Comment"="'Support for full duplex'",
  "Name"="'FRTP_FULLDUPLEX_ENABLE'"!] [!IF "FrTpGeneral/FrTpFullDuplexEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!CALL "GuardedDefine", "Comment"="'Number of configured connections'",
  "Name"="'FRTP_CON_NUM'"!] [!"$NumberOfConnections"!]U
[!CALL "GuardedDefine", "Comment"="'Number of configured TX PDUs'",
  "Name"="'FRTP_TXPDU_NUM'"!] [!"FrTpGeneral/FrTpTxPduNum"!]U
[!CALL "GuardedDefine", "Comment"="'Number of FrTp_MainFunction() invocations per FlexRay communication cycle'",
  "Name"="'FRTP_MAINFUNCTIONS_PER_FLEXRAYCYCLE'"!] [!"FrTpGeneral/VendorSpecific/FrTpMainfunctionsPerCommunicationCycle"!]U
[!CALL "GuardedDefine", "Comment"="'Compile time verification value'",
  "Name"="'FRTP_CFG_SIGNATURE'"!] [!"asc:getConfigSignature(as:modconf('FrTp')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U
[!CALL "GuardedDefine", "Comment"="'Public info signature'",
  "Name"="'FRTP_PUBLIC_INFO_SIGNATURE'"!] [!"asc:getConfigSignature(node:difference(as:modconf('FrTp')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('FrTp')[1]/CommonPublishedInformation/Release))"!]U

#if (FRTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
  [!CALL "GuardedDefine", "Comment"="'Support for relocatable postbuild configuration'",
    "Name"="'FRTP_RELOCATABLE_CFG_ENABLE'"!] [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#else
  [!CALL "GuardedDefine", "Comment"="'Support for relocatable postbuild configuration'",
    "Name"="'FRTP_RELOCATABLE_CFG_ENABLE'"!] [!IF "FrTpGeneral/VendorSpecific/FrTpRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
#endif

[!CALL "GuardedDefine", "Comment"="'Support copy of STF to local buffer'",
  "Name"="'FRTP_COPY_STF_LOCALBUFFER'"!] [!IF "FrTpGeneral/VendorSpecific/FrTpCopyToLocalBuffer = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (FRTP_COPY_STF_LOCALBUFFER == STD_ON)
  [!CALL "GuardedDefine", "Comment"="'Maximum size of local buffer'",
    "Name"="'FRTP_MAXSIZE_LOCALBUFFER'"!]  [!"FrTpGeneral/VendorSpecific/FrTpLocalBufferSize"!]U
#endif

[!CALL "GuardedDefine", "Comment"="'Support of connection buffer'",
    "Name"="'FRTP_LIMIT_NUMBER_CONNECTIONS'"!] [!IF "$FrTpLimitNumberOfConnections = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (FRTP_LIMIT_NUMBER_CONNECTIONS == STD_ON)
  [!CALL "GuardedDefine", "Comment"="'Maximum number of buffered connections'",
    "Name"="'FRTP_SUM_BUFFER_SIZE'"!] [!"$SumBufferSize"!]U

  [!CALL "GuardedDefine", "Comment"="'Number of configured connection limits'",
    "Name"="'FRTP_NUMBER_CONNECTION_LIMITS'"!] [!"$NumberOfConnectionLimits"!]U
    
  [!CALL "GuardedDefine", "Comment"="'Number of connection buffers'",
    "Name"="'FRTP_NUMBER_CONNECTION_BUFFERS'"!] [!"$NumberOfConnectionBuffers"!]U

  [!CALL "GuardedDefine", "Comment"="'The data type of a connection limit config index'",
    "Name"="'FRTP_CONNECTION_LIMIT_CONFIG_IDX_TYPE'"!] [!"$ConnectionLimitConfigIdxType"!]
#endif

/******************************** defensive programming ************************************/




/*------------------[Defensive programming]---------------------------------*/
[!SELECT "FrTpDefensiveProgramming"!][!//

#if (defined FRTP_DEFENSIVE_PROGRAMMING_ENABLED)
#error FRTP_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define FRTP_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRTP_PRECONDITION_ASSERT_ENABLED)
#error FRTP_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define FRTP_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true') and (FrTpPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRTP_POSTCONDITION_ASSERT_ENABLED)
#error FRTP_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define FRTP_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true') and (FrTpPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRTP_UNREACHABLE_CODE_ASSERT_ENABLED)
#error FRTP_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define FRTP_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true') and (FrTpUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRTP_INVARIANT_ASSERT_ENABLED)
#error FRTP_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define FRTP_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true') and (FrTpInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined FRTP_STATIC_ASSERT_ENABLED)
#error FRTP_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define FRTP_STATIC_ASSERT_ENABLED           [!//
[!IF "(../FrTpGeneral/FrTpDevErrorDetect  = 'true') and (FrTpDefProgEnabled = 'true') and (FrTpStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


#endif /* FRTP_CFG_H */
[!ENDCODE!][!//
