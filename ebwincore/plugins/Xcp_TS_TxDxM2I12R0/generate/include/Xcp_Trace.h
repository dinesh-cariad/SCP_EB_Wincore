/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef XCP_TRACE_H
#define XCP_TRACE_H

[!AUTOSPACING!][!//

/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>
[!ENDIF!]


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==================[macros]================================================*/

#ifndef DBG_XCP_SETTRANSMISSIONMODE_ENTRY
#define DBG_XCP_SETTRANSMISSIONMODE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SETTRANSMISSIONMODE_EXIT
#define DBG_XCP_SETTRANSMISSIONMODE_EXIT()
#endif

#ifndef DBG_XCP_APPLGETADDRESS_ENTRY
/** \brief Entry point of function Xcp_ApplGetAddress() */
#define DBG_XCP_APPLGETADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLGETADDRESS_EXIT
/** \brief Exit point of function Xcp_ApplGetAddress() */
#define DBG_XCP_APPLGETADDRESS_EXIT(a,b,c)
#endif

[!IF "XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"!]
#ifndef DBG_XCP_APPLISMEMORYAREAACCESSIBLE_ENTRY
/** \brief Entry point of function Xcp_ApplIsMemoryAreaAccessible() */
#define DBG_XCP_APPLISMEMORYAREAACCESSIBLE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLISMEMORYAREAACCESSIBLE_EXIT
/** \brief Exit point of function Xcp_ApplIsMemoryAreaAccessible() */
#define DBG_XCP_APPLISMEMORYAREAACCESSIBLE_EXIT(a,b,c,d,e)
#endif
[!ENDIF!][!// "XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessAreasCallout = 'true'"

#ifndef DBG_XCP_APPLCALPAGINIT_ENTRY
/** \brief Entry point of function Xcp_ApplCalPagInit() */
#define DBG_XCP_APPLCALPAGINIT_ENTRY()
#endif

#ifndef DBG_XCP_APPLCALPAGINIT_EXIT
/** \brief Exit point of function Xcp_ApplCalPagInit() */
#define DBG_XCP_APPLCALPAGINIT_EXIT()
#endif

#ifndef DBG_XCP_APPLGETTIMESTAMP_ENTRY
/** \brief Entry point of function Xcp_ApplGetTimestamp() */
#define DBG_XCP_APPLGETTIMESTAMP_ENTRY()
#endif

#ifndef DBG_XCP_APPLGETTIMESTAMP_EXIT
/** \brief Exit point of function Xcp_ApplGetTimestamp() */
#define DBG_XCP_APPLGETTIMESTAMP_EXIT(a)
#endif

#ifndef DBG_XCP_APPLBUILDCHECKSUM_ENTRY
/** \brief Entry point of function Xcp_ApplBuildChecksum() */
#define DBG_XCP_APPLBUILDCHECKSUM_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLBUILDCHECKSUM_EXIT
/** \brief Exit point of function Xcp_ApplBuildChecksum() */
#define DBG_XCP_APPLBUILDCHECKSUM_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_APPLCOMPAREKEY_ENTRY
/** \brief Entry point of function Xcp_ApplCompareKey() */
#define DBG_XCP_APPLCOMPAREKEY_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLCOMPAREKEY_EXIT
/** \brief Exit point of function Xcp_ApplCompareKey() */
#define DBG_XCP_APPLCOMPAREKEY_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLGETSEED_ENTRY
/** \brief Entry point of function Xcp_ApplGetSeed() */
#define DBG_XCP_APPLGETSEED_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLGETSEED_EXIT
/** \brief Exit point of function Xcp_ApplGetSeed() */
#define DBG_XCP_APPLGETSEED_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLCOPYCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ApplCopyCalPage() */
#define DBG_XCP_APPLCOPYCALPAGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLCOPYCALPAGE_EXIT
/** \brief Exit point of function Xcp_ApplCopyCalPage() */
#define DBG_XCP_APPLCOPYCALPAGE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_APPLGETPAGPROCESSORINFO_ENTRY
/** \brief Entry point of function Xcp_ApplGetPagProcessorInfo() */
#define DBG_XCP_APPLGETPAGPROCESSORINFO_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLGETPAGPROCESSORINFO_EXIT
/** \brief Exit point of function Xcp_ApplGetPagProcessorInfo() */
#define DBG_XCP_APPLGETPAGPROCESSORINFO_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_APPLGETCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ApplGetCalPage() */
#define DBG_XCP_APPLGETCALPAGE_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLGETCALPAGE_EXIT
/** \brief Exit point of function Xcp_ApplGetCalPage() */
#define DBG_XCP_APPLGETCALPAGE_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLSETCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ApplSetCalPage() */
#define DBG_XCP_APPLSETCALPAGE_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLSETCALPAGE_EXIT
/** \brief Exit point of function Xcp_ApplSetCalPage() */
#define DBG_XCP_APPLSETCALPAGE_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLGETSEGMENTMODE_ENTRY
/** \brief Entry point of function Xcp_ApplGetSegmentMode() */
#define DBG_XCP_APPLGETSEGMENTMODE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLGETSEGMENTMODE_EXIT
/** \brief Exit point of function Xcp_ApplGetSegmentMode() */
#define DBG_XCP_APPLGETSEGMENTMODE_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_APPLSETSEGMENTMODE_ENTRY
/** \brief Entry point of function Xcp_ApplSetSegmentMode() */
#define DBG_XCP_APPLSETSEGMENTMODE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLSETSEGMENTMODE_EXIT
/** \brief Exit point of function Xcp_ApplSetSegmentMode() */
#define DBG_XCP_APPLSETSEGMENTMODE_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_APPLGETSEGMENTINFO_ENTRY
/** \brief Entry point of function Xcp_ApplGetSegmentInfo() */
#define DBG_XCP_APPLGETSEGMENTINFO_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_XCP_APPLGETSEGMENTINFO_EXIT
/** \brief Exit point of function Xcp_ApplGetSegmentInfo() */
#define DBG_XCP_APPLGETSEGMENTINFO_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_XCP_APPLGETPAGEINFO_ENTRY
/** \brief Entry point of function Xcp_ApplGetPageInfo() */
#define DBG_XCP_APPLGETPAGEINFO_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLGETPAGEINFO_EXIT
/** \brief Exit point of function Xcp_ApplGetPageInfo() */
#define DBG_XCP_APPLGETPAGEINFO_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_APPLSETREQSTORECALREQ_ENTRY
/** \brief Entry point of function Xcp_ApplSetReqStoreCalReq() */
#define DBG_XCP_APPLSETREQSTORECALREQ_ENTRY()
#endif

#ifndef DBG_XCP_APPLSETREQSTORECALREQ_EXIT
/** \brief Exit point of function Xcp_ApplSetReqStoreCalReq() */
#define DBG_XCP_APPLSETREQSTORECALREQ_EXIT(a)
#endif

#ifndef DBG_XCP_APPLGETPGMPROCESSORINFO_ENTRY
/** \brief Entry point of function Xcp_ApplGetPgmProcessorInfo() */
#define DBG_XCP_APPLGETPGMPROCESSORINFO_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLGETPGMPROCESSORINFO_EXIT
/** \brief Exit point of function Xcp_ApplGetPgmProcessorInfo() */
#define DBG_XCP_APPLGETPGMPROCESSORINFO_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_APPLGETSECTORINFO_ENTRY
/** \brief Entry point of function Xcp_ApplGetSectorInfo() */
#define DBG_XCP_APPLGETSECTORINFO_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLGETSECTORINFO_EXIT
/** \brief Exit point of function Xcp_ApplGetSectorInfo() */
#define DBG_XCP_APPLGETSECTORINFO_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_APPLPROGRAMSTART_ENTRY
/** \brief Entry point of function Xcp_ApplProgramStart() */
#define DBG_XCP_APPLPROGRAMSTART_ENTRY(a)
#endif

#ifndef DBG_XCP_APPLPROGRAMSTART_EXIT
/** \brief Exit point of function Xcp_ApplProgramStart() */
#define DBG_XCP_APPLPROGRAMSTART_EXIT(a)
#endif

#ifndef DBG_XCP_APPLPROGRAMCLEAR_ENTRY
/** \brief Entry point of function Xcp_ApplProgramClear() */
#define DBG_XCP_APPLPROGRAMCLEAR_ENTRY(a,b)
#endif

#ifndef DBG_XCP_APPLPROGRAMCLEAR_EXIT
/** \brief Exit point of function Xcp_ApplProgramClear() */
#define DBG_XCP_APPLPROGRAMCLEAR_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_APPLPROGRAM_ENTRY
/** \brief Entry point of function Xcp_ApplProgram() */
#define DBG_XCP_APPLPROGRAM_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLPROGRAM_EXIT
/** \brief Exit point of function Xcp_ApplProgram() */
#define DBG_XCP_APPLPROGRAM_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_APPLPROGRAMRESET_ENTRY
/** \brief Entry point of function Xcp_ApplProgramReset() */
#define DBG_XCP_APPLPROGRAMRESET_ENTRY()
#endif

#ifndef DBG_XCP_APPLPROGRAMRESET_EXIT
/** \brief Exit point of function Xcp_ApplProgramReset() */
#define DBG_XCP_APPLPROGRAMRESET_EXIT()
#endif

#ifndef DBG_XCP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Xcp_GetVersionInfo() */
#define DBG_XCP_GETVERSIONINFO_ENTRY()
#endif

#ifndef DBG_XCP_GETVERSIONINFO_EXIT
/** \brief Exit point of function Xcp_GetVersionInfo() */
#define DBG_XCP_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_XCP_INIT_ENTRY
/** \brief Entry point of function Xcp_Init() */
#define DBG_XCP_INIT_ENTRY(a)
#endif

#ifndef DBG_XCP_INIT_EXIT
/** \brief Exit point of function Xcp_Init() */
#define DBG_XCP_INIT_EXIT()
#endif

#ifndef DBG_XCP_MAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_MainFunction() */
#define DBG_XCP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_MAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_MainFunction() */
#define DBG_XCP_MAINFUNCTION_EXIT()
#endif

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpWriteMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpWriteMemoryRAMCallback))"!]
#ifndef DBG_XCP_APPLWRITEDATATORAM_ENTRY
/** \brief Entry point of function Xcp_ApplWriteDataToRAM() */
#define DBG_XCP_APPLWRITEDATATORAM_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLWRITEDATATORAM_EXIT
/** \brief Exit point of function Xcp_ApplWriteDataToRAM() */
#define DBG_XCP_APPLWRITEDATATORAM_EXIT(a,b,c,d)
#endif
[!ENDIF!][!// "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))

[!IF "(XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true') and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))"!]
#ifndef DBG_XCP_APPLREADDATAFROMRAM_ENTRY
/** \brief Entry point of function Xcp_ApplReadDataFromRAM() */
#define DBG_XCP_APPLREADDATAFROMRAM_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_APPLREADDATAFROMRAM_EXIT
/** \brief Exit point of function Xcp_ApplReadDataFromRAM() */
#define DBG_XCP_APPLREADDATAFROMRAM_EXIT(a,b,c,d)
#endif
[!ENDIF!][!// "XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism = 'true' and (node:exists(XcpGeneral/XcpReadMemoryRAMCallback)) and not(node:empty(XcpGeneral/XcpReadMemoryRAMCallback))

[!IF "as:modconf('Xcp')[1]/XcpGeneral/XcpEventTriggerMainFunc = 'true'"!]
  [!IF "(node:exists(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel)) and
        (num:i(count(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel/*)) > 0)"!]
    [!LOOP "as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel/*"!]
      [!IF "XcpEventChannelTimeCycle > 0"!]

#ifndef DBG_XCP_MAINFUNCTION_[!"text:toupper(@name)"!]_ENTRY
/** \brief Entry point of function Xcp_MainFunction_[!"@name"!]() */
#define DBG_XCP_MAINFUNCTION_[!"text:toupper(@name)"!]_ENTRY()
#endif

#ifndef DBG_XCP_MAINFUNCTION_[!"text:toupper(@name)"!]_EXIT
/** \brief Exit point of function Xcp_MainFunction_[!"@name"!]() */
#define DBG_XCP_MAINFUNCTION_[!"text:toupper(@name)"!]_EXIT()
#endif
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDIF!]

[!IF "as:modconf('Xcp')[1]/XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true'"!]
#ifndef DBG_XCP_CRCMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_CrcMainFunction() */
#define DBG_XCP_CRCMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_CRCMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_CrcMainFunction() */
#define DBG_XCP_CRCMAINFUNCTION_EXIT()
#endif
[!ENDIF!]

#ifndef DBG_XCP_CALCULATECRC_ENTRY
/** \brief Entry point of function Xcp_CalculateCRCTask() */
#define DBG_XCP_CALCULATECRC_ENTRY()
#endif

#ifndef DBG_XCP_CALCULATECRC_EXIT
/** \brief Exit point of function Xcp_CalculateCRCTask() */
#define DBG_XCP_CALCULATECRC_EXIT()
#endif


#ifndef DBG_XCP_SOADSOCONMODECHG_ENTRY
/** \brief Entry point of function Xcp_SoAdSoConModeChg() */
#define DBG_XCP_SOADSOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SOADSOCONMODECHG_EXIT
/** \brief Exit point of function Xcp_SoAdSoConModeChg() */
#define DBG_XCP_SOADSOCONMODECHG_EXIT()
#endif

#ifndef DBG_XCP_CANIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_CanIfRxIndication() */
#define DBG_XCP_CANIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_CANIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_CanIfRxIndication() */
#define DBG_XCP_CANIFRXINDICATION_EXIT()
#endif

#ifndef DBG_XCP_FRIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_FrIfRxIndication() */
#define DBG_XCP_FRIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_FRIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_FrIfRxIndication() */
#define DBG_XCP_FRIFRXINDICATION_EXIT()
#endif

#ifndef DBG_XCP_SOADIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_SoAdIfRxIndication() */
#define DBG_XCP_SOADIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SOADIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_SoAdIfRxIndication() */
#define DBG_XCP_SOADIFRXINDICATION_EXIT()
#endif

#ifndef DBG_XCP_COMMONRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_CommonRxIndication() */
#define DBG_XCP_COMMONRXINDICATION_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_COMMONRXINDICATION_EXIT
/** \brief Exit point of function Xcp_CommonRxIndication() */
#define DBG_XCP_COMMONRXINDICATION_EXIT()
#endif

#ifndef DBG_XCP_GETPDUDATALENGTH_ENTRY
/** \brief Entry point of function Xcp_GetPduDataLength() */
#define DBG_XCP_GETPDUDATALENGTH_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_GETPDUDATALENGTH_EXIT
/** \brief Exit point of function Xcp_GetPduDataLength() */
#define DBG_XCP_GETPDUDATALENGTH_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_VALIDATEPDULENGTH_ENTRY
/** \brief Entry point of function Xcp_ValidatePduLength() */
#define DBG_XCP_VALIDATEPDULENGTH_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_VALIDATEPDULENGTH_EXIT
/** \brief Exit point of function Xcp_ValidatePduLength() */
#define DBG_XCP_VALIDATEPDULENGTH_EXIT(a,b,c)
#endif


#ifndef DBG_XCP_GETPDUINFRAMESIZE_ENTRY
/** \brief Entry point of function Xcp_GetPduInFrameSize() */
#define DBG_XCP_GETPDUINFRAMESIZE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETPDUINFRAMESIZE_EXIT
/** \brief Exit point of function Xcp_GetPduInFrameSize() */
#define DBG_XCP_GETPDUINFRAMESIZE_EXIT(a)
#endif

#ifndef DBG_XCP_CANIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_CanIfTxConfirmation() */
#define DBG_XCP_CANIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_CANIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_CanIfTxConfirmation() */
#define DBG_XCP_CANIFTXCONFIRMATION_EXIT()
#endif

#ifndef DBG_XCP_FRIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_FrIfTxConfirmation() */
#define DBG_XCP_FRIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_FRIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_FrIfTxConfirmation() */
#define DBG_XCP_FRIFTXCONFIRMATION_EXIT()
#endif

#ifndef DBG_XCP_SOADIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_SoAdIfTxConfirmation() */
#define DBG_XCP_SOADIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_SOADIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_SoAdIfTxConfirmation() */
#define DBG_XCP_SOADIFTXCONFIRMATION_EXIT()
#endif

#ifndef DBG_XCP_COMMONTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_CommonTxConfirmation() */
#define DBG_XCP_COMMONTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_COMMONTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_CommonTxConfirmation() */
#define DBG_XCP_COMMONTXCONFIRMATION_EXIT()
#endif

#ifndef DBG_XCP_FRIFTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function Xcp_FrIfTriggerTransmit() */
#define DBG_XCP_FRIFTRIGGERTRANSMIT_ENTRY(a)
#endif

#ifndef DBG_XCP_FRIFTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function Xcp_FrIfTriggerTransmit() */
#define DBG_XCP_FRIFTRIGGERTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_XCP_COMMONTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function Xcp_CommonTriggerTransmit() */
#define DBG_XCP_COMMONTRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_COMMONTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function Xcp_CommonTriggerTransmit() */
#define DBG_XCP_COMMONTRIGGERTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_XCP_GETPDUINTERNALROUTEID_ENTRY
/** \brief Entry point of function Xcp_GetPduInternalRouteId() */
#define DBG_XCP_GETPDUINTERNALROUTEID_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_GETPDUINTERNALROUTEID_EXIT
/** \brief Exit point of function Xcp_GetPduInternalRouteId() */
#define DBG_XCP_GETPDUINTERNALROUTEID_EXIT(a,b)
#endif

#ifndef DBG_XCP_PROCESSRESOURCESUNLOCK_ENTRY
/** \brief Entry point of function Xcp_ProcessResourcesUnlock() */
#define DBG_XCP_PROCESSRESOURCESUNLOCK_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSRESOURCESUNLOCK_EXIT
/** \brief Exit point of function Xcp_ProcessResourcesUnlock() */
#define DBG_XCP_PROCESSRESOURCESUNLOCK_EXIT()
#endif

#ifndef DBG_XCP_INITIALIZEDAQLISTS_ENTRY
/** \brief Entry point of function Xcp_InitializeDaqLists() */
#define DBG_XCP_INITIALIZEDAQLISTS_ENTRY()
#endif

#ifndef DBG_XCP_INITIALIZEDAQLISTS_EXIT
/** \brief Exit point of function Xcp_InitializeDaqLists() */
#define DBG_XCP_INITIALIZEDAQLISTS_EXIT(a)
#endif

#ifndef DBG_XCP_INITCOMMANDPROCESSOR_ENTRY
/** \brief Entry point of function Xcp_InitCommandProcessor() */
#define DBG_XCP_INITCOMMANDPROCESSOR_ENTRY()
#endif

#ifndef DBG_XCP_INITCOMMANDPROCESSOR_EXIT
/** \brief Exit point of function Xcp_InitCommandProcessor() */
#define DBG_XCP_INITCOMMANDPROCESSOR_EXIT()
#endif

#ifndef DBG_XCP_SETABORTPROGRAMMINGSESSION_ENTER
/** \brief Entry point of function Xcp_SetAbortProgrammingSession() */
#define DBG_XCP_SETABORTPROGRAMMINGSESSION_ENTER()
#endif

#ifndef DBG_XCP_SETABORTPROGRAMMINGSESSION_EXIT
/** \brief Exit point of function Xcp_SetAbortProgrammingSession() */
#define DBG_XCP_SETABORTPROGRAMMINGSESSION_EXIT()
#endif

#ifndef DBG_XCP_HANDLECMDREQEXTSTATUS_ENTER
/** \brief Entry point of function Xcp_HandleCmdReqExtStatus() */
#define DBG_XCP_HANDLECMDREQEXTSTATUS_ENTER()
#endif

#ifndef DBG_XCP_HANDLECMDREQEXTSTATUS_EXIT
/** \brief Exit point of function Xcp_HandleCmdReqExtStatus() */
#define DBG_XCP_HANDLECMDREQEXTSTATUS_EXIT()
#endif

#ifndef DBG_XCP_PROCESSCOMMANDQUEUE_ENTRY
/** \brief Entry point of function Xcp_ProcessCommandQueue() */
#define DBG_XCP_PROCESSCOMMANDQUEUE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCOMMANDQUEUE_EXIT
/** \brief Exit point of function Xcp_ProcessCommandQueue() */
#define DBG_XCP_PROCESSCOMMANDQUEUE_EXIT()
#endif

#ifndef DBG_XCP_GETCMDLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetCmdLength() */
#define DBG_XCP_GETCMDLENGTH_ENTRY(a)
#endif

#ifndef DBG_XCP_GETCMDLENGTH_EXIT
/** \brief Exit point of function Xcp_GetCmdLength() */
#define DBG_XCP_GETCMDLENGTH_EXIT(a,b)
#endif

#ifndef DBG_XCP_SENDCMDERRORPACKET_ENTRY
/** \brief Entry point of function Xcp_SendCmdErrorPacket() */
#define DBG_XCP_SENDCMDERRORPACKET_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SENDCMDERRORPACKET_EXIT
/** \brief Exit point of function Xcp_SendCmdErrorPacket() */
#define DBG_XCP_SENDCMDERRORPACKET_EXIT()
#endif

#ifndef DBG_XCP_ISCMDIMPLEMENTED_ENTRY
/** \brief Entry point of function Xcp_IsCmdImplemented() */
#define DBG_XCP_ISCMDIMPLEMENTED_ENTRY(a)
#endif

#ifndef DBG_XCP_ISCMDIMPLEMENTED_EXIT
/** \brief Exit point of function Xcp_IsCmdImplemented() */
#define DBG_XCP_ISCMDIMPLEMENTED_EXIT(a)
#endif

#ifndef DBG_XCP_INSERTCOMMAND_ENTRY
/** \brief Entry point of function Xcp_InsertCommand() */
#define DBG_XCP_INSERTCOMMAND_ENTRY(a,b)
#endif

#ifndef DBG_XCP_INSERTCOMMAND_EXIT
/** \brief Exit point of function Xcp_InsertCommand() */
#define DBG_XCP_INSERTCOMMAND_EXIT(a,b)
#endif

#ifndef DBG_XCP_SETSYNCHCMDPENDING_ENTRY
/** \brief Entry point of function Xcp_SetSynchCmdPending() */
#define DBG_XCP_SETSYNCHCMDPENDING_ENTRY()
#endif

#ifndef DBG_XCP_SETSYNCHCMDPENDING_EXIT
/** \brief Exit point of function Xcp_SetSynchCmdPending() */
#define DBG_XCP_SETSYNCHCMDPENDING_EXIT()
#endif

#ifndef DBG_XCP_SETDISCONNECTCMDPENDING_ENTRY
/** \brief Entry point of function Xcp_SetDisconnectCmdPending() */
#define DBG_XCP_SETDISCONNECTCMDPENDING_ENTRY()
#endif

#ifndef DBG_XCP_SETDISCONNECTCMDPENDING_EXIT
/** \brief Exit point of function Xcp_SetDisconnectCmdPending() */
#define DBG_XCP_SETDISCONNECTCMDPENDING_EXIT()
#endif

#ifndef DBG_XCP_ISDOWNLOADINBLOCKMODE_ENTRY
/** \brief Entry point of function Xcp_IsDownloadInBlockMode() */
#define DBG_XCP_ISDOWNLOADINBLOCKMODE_ENTRY()
#endif

#ifndef DBG_XCP_ISDOWNLOADINBLOCKMODE_EXIT
/** \brief Exit point of function Xcp_IsDownloadInBlockMode() */
#define DBG_XCP_ISDOWNLOADINBLOCKMODE_EXIT(a)
#endif

#ifndef DBG_XCP_SETDOWNLOADINBLOCKMODE_ENTRY
/** \brief Entry point of function Xcp_SetDownloadInBlockMode() */
#define DBG_XCP_SETDOWNLOADINBLOCKMODE_ENTRY()
#endif

#ifndef DBG_XCP_SETDOWNLOADINBLOCKMODE_EXIT
/** \brief Exit point of function Xcp_SetDownloadInBlockMode() */
#define DBG_XCP_SETDOWNLOADINBLOCKMODE_EXIT()
#endif

#ifndef DBG_XCP_RESETMEMORYWRITEINFO_ENTRY
/** \brief Entry point of function Xcp_ResetMemoryWriteInfo() */
#define DBG_XCP_RESETMEMORYWRITEINFO_ENTRY()
#endif

#ifndef DBG_XCP_RESETMEMORYWRITEINFO_EXIT
/** \brief Exit point of function Xcp_ResetMemoryWriteInfo() */
#define DBG_XCP_RESETMEMORYWRITEINFO_EXIT()
#endif

#ifndef DBG_XCP_COMMANDQUEUEISEMPTY_ENTRY
/** \brief Entry point of function Xcp_CommandQueueIsEmpty() */
#define DBG_XCP_COMMANDQUEUEISEMPTY_ENTRY()
#endif

#ifndef DBG_XCP_COMMANDQUEUEISEMPTY_EXIT
/** \brief Exit point of function Xcp_CommandQueueIsEmpty() */
#define DBG_XCP_COMMANDQUEUEISEMPTY_EXIT(a)
#endif

#ifndef DBG_XCP_COMMANDQUEUEISFULL_ENTRY
/** \brief Entry point of function Xcp_CommandQueueIsFull() */
#define DBG_XCP_COMMANDQUEUEISFULL_ENTRY()
#endif

#ifndef DBG_XCP_COMMANDQUEUEISFULL_EXIT
/** \brief Exit point of function Xcp_CommandQueueIsFull() */
#define DBG_XCP_COMMANDQUEUEISFULL_EXIT(a)
#endif

#ifndef DBG_XCP_PACKTXBUFFERDATA_ENTRY
/** \brief Entry point of function Xcp_PackTxBufferData() */
#define DBG_XCP_PACKTXBUFFERDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_PACKTXBUFFERDATA_EXIT
/** \brief Exit point of function Xcp_PackTxBufferData() */
#define DBG_XCP_PACKTXBUFFERDATA_EXIT(a)
#endif

#ifndef DBG_XCP_FLUSHCMDQUEUE_ENTRY
/** \brief Entry point of function Xcp_FlushCMDQueue() */
#define DBG_XCP_FLUSHCMDQUEUE_ENTRY()
#endif

#ifndef DBG_XCP_FLUSHCMDQUEUE_EXIT
/** \brief Exit point of function Xcp_FlushCMDQueue() */
#define DBG_XCP_FLUSHCMDQUEUE_EXIT()
#endif

#ifndef DBG_XCP_ISEVENTCTOQUEUEEMPTY_ENTRY
/** \brief Entry point of function Xcp_IsEventCtoQueueEmpty() */
#define DBG_XCP_ISEVENTCTOQUEUEEMPTY_ENTRY()
#endif

#ifndef DBG_XCP_ISEVENTCTOQUEUEEMPTY_EXIT
/** \brief Exit point of function Xcp_IsEventCtoQueueEmpty() */
#define DBG_XCP_ISEVENTCTOQUEUEEMPTY_EXIT(a)
#endif

#ifndef DBG_XCP_GETEVENTCTOLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetEventCtoLength() */
#define DBG_XCP_GETEVENTCTOLENGTH_ENTRY()
#endif

#ifndef DBG_XCP_GETEVENTCTOLENGTH_EXIT
/** \brief Exit point of function Xcp_GetEventCtoLength() */
#define DBG_XCP_GETEVENTCTOLENGTH_EXIT(a)
#endif

#ifndef DBG_XCP_DEQUEUEEVENTCTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_DeQueueEventCtoQueue() */
#define DBG_XCP_DEQUEUEEVENTCTOQUEUE_ENTRY(a, b)
#endif

#ifndef DBG_XCP_DEQUEUEEVENTCTOQUEUE_EXIT
/** \brief Exit point of function Xcp_DeQueueEventCtoQueue() */
#define DBG_XCP_DEQUEUEEVENTCTOQUEUE_EXIT(a)
#endif

#ifndef DBG_XCP_ENQUEUECTOEVENT_ENTRY
/** \brief Entry point of function Xcp_EnqueueCtoEvent() */
#define DBG_XCP_ENQUEUECTOEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_ENQUEUECTOEVENT_EXIT
/** \brief Exit point of function Xcp_EnqueueCtoEvent() */
#define DBG_XCP_ENQUEUECTOEVENT_EXIT()
#endif

#ifndef DBG_XCP_FLUSHCTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_FlushCTOQueue() */
#define DBG_XCP_FLUSHCTOQUEUE_ENTRY()
#endif

#ifndef DBG_XCP_FLUSHCTOQUEUE_EXIT
/** \brief Exit point of function Xcp_FlushCTOQueue() */
#define DBG_XCP_FLUSHCTOQUEUE_EXIT()
#endif

#ifndef DBG_XCP_FLUSHEVENTCTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_FlushEventCTOQueue() */
#define DBG_XCP_FLUSHEVENTCTOQUEUE_ENTRY()
#endif

#ifndef DBG_XCP_FLUSHEVENTCTOQUEUE_EXIT
/** \brief Exit point of function Xcp_FlushEventCTOQueue() */
#define DBG_XCP_FLUSHEVENTCTOQUEUE_EXIT()
#endif

#ifndef DBG_XCP_WRITEDWORDTOBYTEARRAY_ENTRY
/** \brief Entry point of function Xcp_WriteDwordToByteArray() */
#define DBG_XCP_WRITEDWORDTOBYTEARRAY_ENTRY(a)
#endif

#ifndef DBG_XCP_WRITEDWORDTOBYTEARRAY_EXIT
/** \brief Exit point of function Xcp_WriteDwordToByteArray() */
#define DBG_XCP_WRITEDWORDTOBYTEARRAY_EXIT(a)
#endif

#ifndef DBG_XCP_WRITEWORDTOBYTEARRAY_ENTRY
/** \brief Entry point of function Xcp_WriteWordToByteArray() */
#define DBG_XCP_WRITEWORDTOBYTEARRAY_ENTRY(a)
#endif

#ifndef DBG_XCP_WRITEWORDTOBYTEARRAY_EXIT
/** \brief Exit point of function Xcp_WriteWordToByteArray() */
#define DBG_XCP_WRITEWORDTOBYTEARRAY_EXIT(a)
#endif

#ifndef DBG_XCP_GETCOMMANDPROCESSORSTATE_ENTRY
/** \brief Entry point of function Xcp_GetCommandProcessorState() */
#define DBG_XCP_GETCOMMANDPROCESSORSTATE_ENTRY()
#endif

#ifndef DBG_XCP_GETCOMMANDPROCESSORSTATE_EXIT
/** \brief Exit point of function Xcp_GetCommandProcessorState() */
#define DBG_XCP_GETCOMMANDPROCESSORSTATE_EXIT(a)
#endif

#ifndef DBG_XCP_ABORTBUSYCOMMAND_ENTRY
/** \brief Entry point of function Xcp_AbortBusyCommand() */
#define DBG_XCP_ABORTBUSYCOMMAND_ENTRY()
#endif

#ifndef DBG_XCP_ABORTBUSYCOMMAND_EXIT
/** \brief Exit point of function Xcp_AbortBusyCommand() */
#define DBG_XCP_ABORTBUSYCOMMAND_EXIT()
#endif

#ifndef DBG_XCP_SETREQSTORECALREQCBK_ENTRY
/** \brief Entry point of function Xcp_SetReqStoreCalReqCbk() */
#define DBG_XCP_SETREQSTORECALREQCBK_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SETREQSTORECALREQCBK_EXIT
/** \brief Exit point of function Xcp_SetReqStoreCalReqCbk() */
#define DBG_XCP_SETREQSTORECALREQCBK_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_NVMSTOREDAQSINGLECBK_ENTRY
/** \brief Entry point of function Xcp_NvmStoreDaqSingleCbk() */
#define DBG_XCP_NVMSTOREDAQSINGLECBK_ENTRY(a,b)
#endif

#ifndef DBG_XCP_NVMSTOREDAQSINGLECBK_EXIT
/** \brief Exit point of function Xcp_NvmStoreDaqSingleCbk() */
#define DBG_XCP_NVMSTOREDAQSINGLECBK_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_PREPARESTOREDDAQLISTS_ENTRY
/** \brief Entry point of function Xcp_PrepareStoredDaqLists() */
#define DBG_XCP_PREPARESTOREDDAQLISTS_ENTRY()
#endif

#ifndef DBG_XCP_PREPARESTOREDDAQLISTS_EXIT
/** \brief Exit point of function Xcp_PrepareStoredDaqLists() */
#define DBG_XCP_PREPARESTOREDDAQLISTS_EXIT()
#endif

#ifndef DBG_XCP_PREPARESTOREDDAQLISTSRESUME_ENTRY
/** \brief Entry point of function Xcp_PrepareStoredDaqListsResume() */
#define DBG_XCP_PREPARESTOREDDAQLISTSRESUME_ENTRY()
#endif

#ifndef DBG_XCP_PREPARESTOREDDAQLISTSRESUME_EXIT
/** \brief Exit point of function Xcp_PrepareStoredDaqListsResume() */
#define DBG_XCP_PREPARESTOREDDAQLISTSRESUME_EXIT()
#endif

#ifndef DBG_XCP_GETDAQLISTPTR_ENTRY
/** \brief Entry point of function Xcp_GetDaqListPtr() */
#define DBG_XCP_GETDAQLISTPTR_ENTRY(a)
#endif

#ifndef DBG_XCP_GETDAQLISTPTR_EXIT
/** \brief Exit point of function Xcp_GetDaqListPtr() */
#define DBG_XCP_GETDAQLISTPTR_EXIT(a,b)
#endif

#ifndef DBG_XCP_GETBUILDCHECKSUMREQSTATUS_ENTRY
/** \brief Entry point of function Xcp_GetBuildChecksumReqStatus() */
#define DBG_XCP_GETBUILDCHECKSUMREQSTATUS_ENTRY()
#endif

#ifndef DBG_XCP_GETBUILDCHECKSUMREQSTATUS_EXIT
/** \brief Exit point of function Xcp_GetBuildChecksumReqStatus() */
#define DBG_XCP_GETBUILDCHECKSUMREQSTATUS_EXIT(a)
#endif

#ifndef DBG_XCP_GETMIRRORTPTR_ENTRY
/** \brief Entry point of function Xcp_GetMirrorPtr() */
#define DBG_XCP_GETMIRRORTPTR_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETMIRRORTPTR_EXIT
/** \brief Exit point of function Xcp_GetMirrorPtr() */
#define DBG_XCP_GETMIRRORTPTR_EXIT(a)
#endif

#ifndef DBG_XCP_GETNOOFDYNAMICDAQLISTS_ENTRY
/** \brief Entry point of function Xcp_GetNoOfDynamicDaqLists() */
#define DBG_XCP_GETNOOFDYNAMICDAQLISTS_ENTRY()
#endif

#ifndef DBG_XCP_GETNOOFDYNAMICDAQLISTS_EXIT
/** \brief Exit point of function Xcp_GetNoOfDynamicDaqLists() */
#define DBG_XCP_GETNOOFDYNAMICDAQLISTS_EXIT(a)
#endif

#ifndef DBG_XCP_GETLENGTHTOBERESERVED_ENTRY
/** \brief Entry point of function Xcp_GetLengthToBeReserved() */
#define DBG_XCP_GETLENGTHTOBERESERVED_ENTRY(a)
#endif

#ifndef DBG_XCP_GETLENGTHTOBERESERVED_EXIT
/** \brief Exit point of function Xcp_GetLengthToBeReserved() */
#define DBG_XCP_GETLENGTHTOBERESERVED_EXIT(a,b)
#endif

#ifndef DBG_XCP_DISCONNECT_ENTRY
/** \brief Entry point of function Xcp_Disconnect() */
#define DBG_XCP_DISCONNECT_ENTRY()
#endif

#ifndef DBG_XCP_DISCONNECT_EXIT
/** \brief Exit point of function Xcp_Disconnect() */
#define DBG_XCP_DISCONNECT_EXIT()
#endif

#ifndef DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_ReserveBytesToRespCTOQueue() */
#define DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_ENTRY(a)
#endif

#ifndef DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_EXIT
/** \brief Exit point of function Xcp_ReserveBytesToRespCTOQueue() */
#define DBG_XCP_RESERVEBYTESTORESPCTOQUEUE_EXIT(a,b)
#endif

#ifndef DBG_XCP_CLEARODTENTRIES_ENTRY
/** \brief Entry point of function Xcp_ClearODTEntries() */
#define DBG_XCP_CLEARODTENTRIES_ENTRY(a,b)
#endif

#ifndef DBG_XCP_CLEARODTENTRIES_EXIT
/** \brief Exit point of function Xcp_ClearODTEntries() */
#define DBG_XCP_CLEARODTENTRIES_EXIT(a,b)
#endif

#ifndef DBG_XCP_CLEARODTS_ENTRY
/** \brief Entry point of function Xcp_ClearODTs() */
#define DBG_XCP_CLEARODTS_ENTRY(a,b)
#endif

#ifndef DBG_XCP_CLEARODTS_EXIT
/** \brief Exit point of function Xcp_ClearODTs() */
#define DBG_XCP_CLEARODTS_EXIT(a,b)
#endif

#ifndef DBG_XCP_INSERTRESPONSECTO_ENTRY
/** \brief Entry point of function Xcp_InsertResponseCTO() */
#define DBG_XCP_INSERTRESPONSECTO_ENTRY(a)
#endif

#ifndef DBG_XCP_INSERTRESPONSECTO_EXIT
/** \brief Exit point of function Xcp_InsertResponseCTO() */
#define DBG_XCP_INSERTRESPONSECTO_EXIT(a)
#endif

#ifndef DBG_XCP_INSERTMULTICASTRESPONSECTO_ENTRY
/** \brief Entry point of function Xcp_InsertMulticastResponseCTO() */
#define DBG_XCP_INSERTMULTICASTRESPONSECTO_ENTRY(a)
#endif

#ifndef DBG_XCP_INSERTMULTICASTRESPONSECTO_EXIT
/** \brief Exit point of function Xcp_InsertMulticastResponseCTO() */
#define DBG_XCP_INSERTMULTICASTRESPONSECTO_EXIT(a)
#endif

#ifndef DBG_XCP_TERMINATESESSION_ENTRY
/** \brief Entry point of function Xcp_TerminateSession() */
#define DBG_XCP_TERMINATESESSION_ENTRY()
#endif

#ifndef DBG_XCP_TERMINATESESSION_EXIT
/** \brief Exit point of function Xcp_TerminateSession() */
#define DBG_XCP_TERMINATESESSION_EXIT()
#endif

#ifndef DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_ENTRY
/** \brief Entry point of function Xcp_RemoveDAQListFromEventChannel() */
#define DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_ENTRY(a,b)
#endif

#ifndef DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_EXIT
/** \brief Exit point of function Xcp_RemoveDAQListFromEventChannel() */
#define DBG_XCP_REMOVEDAQLISTFROMEVENTCHANNEL_EXIT(a,b)
#endif

#ifndef DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_ENTRY
/** \brief Entry point of function Xcp_AssociateDAQListToEventChannel() */
#define DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_ENTRY(a,b)
#endif

#ifndef DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_EXIT
/** \brief Exit point of function Xcp_AssociateDAQListToEventChannel() */
#define DBG_XCP_ASSOCIATEDAQLISTTOEVENTCHANNEL_EXIT(a,b)
#endif

#ifndef DBG_XCP_ENQUEUEBYTEINCTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_EnqueueByteInCtoQueue() */
#define DBG_XCP_ENQUEUEBYTEINCTOQUEUE_ENTRY(a)
#endif

#ifndef DBG_XCP_ENQUEUEBYTEINCTOQUEUE_EXIT
/** \brief Exit point of function Xcp_EnqueueByteInCtoQueue() */
#define DBG_XCP_ENQUEUEBYTEINCTOQUEUE_EXIT(a)
#endif

#ifndef DBG_XCP_DEQUEUECMDQUEUE_ENTRY
/** \brief Entry point of function Xcp_DeQueueCmdQueue() */
#define DBG_XCP_DEQUEUECMDQUEUE_ENTRY()
#endif

#ifndef DBG_XCP_DEQUEUECMDQUEUE_EXIT
/** \brief Exit point of function Xcp_DeQueueCmdQueue() */
#define DBG_XCP_DEQUEUECMDQUEUE_EXIT()
#endif

#ifndef DBG_XCP_ISCTOQUEUEEMPTY_ENTRY
/** \brief Entry point of function Xcp_IsCtoQueueEmpty() */
#define DBG_XCP_ISCTOQUEUEEMPTY_ENTRY()
#endif

#ifndef DBG_XCP_ISCTOQUEUEEMPTY_EXIT
/** \brief Exit point of function Xcp_IsCtoQueueEmpty() */
#define DBG_XCP_ISCTOQUEUEEMPTY_EXIT(a)
#endif

#ifndef DBG_XCP_GETCTOLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetCtoLength() */
#define DBG_XCP_GETCTOLENGTH_ENTRY()
#endif

#ifndef DBG_XCP_GETCTOLENGTH_EXIT
/** \brief Exit point of function Xcp_GetCtoLength() */
#define DBG_XCP_GETCTOLENGTH_EXIT(a)
#endif

#ifndef DBG_XCP_DEQUEUECTOQUEUE_ENTRY
/** \brief Entry point of function Xcp_DeQueueCtoQueue() */
#define DBG_XCP_DEQUEUECTOQUEUE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_DEQUEUECTOQUEUE_EXIT
/** \brief Exit point of function Xcp_DeQueueCtoQueue() */
#define DBG_XCP_DEQUEUECTOQUEUE_EXIT(a,b)
#endif

#ifndef DBG_XCP_ISMULTICASTRESPONSE_ENTRY
/** \brief Entry point of function Xcp_IsMulticastResponse() */
#define DBG_XCP_ISMULTICASTRESPONSE_ENTRY()
#endif

#ifndef DBG_XCP_ISMULTICASTRESPONSE_EXIT
/** \brief Exit point of function Xcp_IsMulticastResponse() */
#define DBG_XCP_ISMULTICASTRESPONSE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMD_ENTRY
/** \brief Entry point of function Xcp_ProcessCmd() */
#define DBG_XCP_PROCESSCMD_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMD_EXIT
/** \brief Exit point of function Xcp_ProcessCmd() */
#define DBG_XCP_PROCESSCMD_EXIT()
#endif

#ifndef DBG_XCP_EXECUTECOMMAND_ENTRY
/** \brief Entry point of function Xcp_ExecuteCommand() */
#define DBG_XCP_EXECUTECOMMAND_ENTRY(a)
#endif

#ifndef DBG_XCP_EXECUTECOMMAND_EXIT
/** \brief Exit point of function Xcp_ExecuteCommand() */
#define DBG_XCP_EXECUTECOMMAND_EXIT(a,b)
#endif

#ifndef DBG_XCP_PROCESSCMDCONNECT_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdConnect() */
#define DBG_XCP_PROCESSCMDCONNECT_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDCONNECT_EXIT
/** \brief Exit point of function Xcp_ProcessCmdConnect() */
#define DBG_XCP_PROCESSCMDCONNECT_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDDISCONNECT_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdDisConnect() */
#define DBG_XCP_PROCESSCMDDISCONNECT_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDDISCONNECT_EXIT
/** \brief Exit point of function Xcp_ProcessCmdDisConnect() */
#define DBG_XCP_PROCESSCMDDISCONNECT_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETID_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetId() */
#define DBG_XCP_PROCESSCMDGETID_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETID_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetId() */
#define DBG_XCP_PROCESSCMDGETID_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETCOMMMODEINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetCommModeInfo() */
#define DBG_XCP_PROCESSCMDGETCOMMMODEINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETCOMMMODEINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetCommModeInfo() */
#define DBG_XCP_PROCESSCMDGETCOMMMODEINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETSTATUS_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetStatus() */
#define DBG_XCP_PROCESSCMDGETSTATUS_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSTATUS_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetStatus() */
#define DBG_XCP_PROCESSCMDGETSTATUS_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdStartStopDAQList() */
#define DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_EXIT
/** \brief Exit point of function Xcp_ProcessCmdStartStopDAQList() */
#define DBG_XCP_PROCESSCMDSTARTSTOPDAQLIST_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdStartStopSynch() */
#define DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_EXIT
/** \brief Exit point of function Xcp_ProcessCmdStartStopSynch() */
#define DBG_XCP_PROCESSCMDSTARTSTOPSYNCH_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSSTARTSTOPSYNCH_ENTRY
/** \brief Entry point of function Xcp_ProcessStartStopSynch() */
#define DBG_XCP_PROCESSSTARTSTOPSYNCH_ENTRY(a)
#endif

#ifndef DBG_XCP_PROCESSSTARTSTOPSYNCH_EXIT
/** \brief Exit point of function Xcp_ProcessStartStopSynch() */
#define DBG_XCP_PROCESSSTARTSTOPSYNCH_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQProcessorInfo() */
#define DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQProcessorInfo() */
#define DBG_XCP_PROCESSCMDGETDAQPROCESSORINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQLISTMODE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQListMode() */
#define DBG_XCP_PROCESSCMDGETDAQLISTMODE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQLISTMODE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQListMode() */
#define DBG_XCP_PROCESSCMDGETDAQLISTMODE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQResolutionInfo() */
#define DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQResolutionInfo() */
#define DBG_XCP_PROCESSCMDGETDAQRESOLUTIONINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQEVENTINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQEventInfo() */
#define DBG_XCP_PROCESSCMDGETDAQEVENTINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQEVENTINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQEventInfo() */
#define DBG_XCP_PROCESSCMDGETDAQEVENTINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQLISTINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQListInfo() */
#define DBG_XCP_PROCESSCMDGETDAQLISTINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQLISTINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQListInfo() */
#define DBG_XCP_PROCESSCMDGETDAQLISTINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDCLEARDAQLIST_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdClearDAQList() */
#define DBG_XCP_PROCESSCMDCLEARDAQLIST_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDCLEARDAQLIST_EXIT
/** \brief Exit point of function Xcp_ProcessCmdClearDAQList() */
#define DBG_XCP_PROCESSCMDCLEARDAQLIST_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSETDAQLISTMODE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetDAQListMode() */
#define DBG_XCP_PROCESSCMDSETDAQLISTMODE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETDAQLISTMODE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetDAQListMode() */
#define DBG_XCP_PROCESSCMDSETDAQLISTMODE_EXIT(a)
#endif

#ifndef DBG_XCP_SUBSETDAQLISTMODE_ENTRY
/** \brief Entry point of function Xcp_SubSetDAQListMode() */
#define DBG_XCP_SUBSETDAQLISTMODE_ENTRY(a)
#endif

#ifndef DBG_XCP_SUBSETDAQLISTMODE_EXIT
/** \brief Exit point of function Xcp_SubSetDAQListMode() */
#define DBG_XCP_SUBSETDAQLISTMODE_EXIT(a)
#endif

#ifndef DBG_XCP_SUBSETDAQMODE_DAQPROCESS_ENTRY
/** \brief Entry point of function Xcp_SubSetDaqMode_DaqProcess() */
#define DBG_XCP_SUBSETDAQMODE_DAQPROCESS_ENTRY()
#endif

#ifndef DBG_XCP_SUBSETDAQMODE_DAQPROCESS_EXIT
/** \brief Exit point of function Xcp_SubSetDaqMode_DaqProcess() */
#define DBG_XCP_SUBSETDAQMODE_DAQPROCESS_EXIT()
#endif

#ifndef DBG_XCP_PROCESSSETDAQLISTMODE_ENTRY
/** \brief Entry point of function Xcp_ProcessSetDAQListMode() */
#define DBG_XCP_PROCESSSETDAQLISTMODE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSSETDAQLISTMODE_EXIT
/** \brief Exit point of function Xcp_ProcessSetDAQListMode() */
#define DBG_XCP_PROCESSSETDAQLISTMODE_EXIT()
#endif

#ifndef DBG_XCP_PROCESSCMDSETMTA_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetMTA() */
#define DBG_XCP_PROCESSCMDSETMTA_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETMTA_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetMTA() */
#define DBG_XCP_PROCESSCMDSETMTA_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSHORTUPLOAD_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdShortUpload() */
#define DBG_XCP_PROCESSCMDSHORTUPLOAD_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSHORTUPLOAD_EXIT
/** \brief Exit point of function Xcp_ProcessCmdShortUpload() */
#define DBG_XCP_PROCESSCMDSHORTUPLOAD_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDUPLOAD_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdUpload() */
#define DBG_XCP_PROCESSCMDUPLOAD_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDUPLOAD_EXIT
/** \brief Exit point of function Xcp_ProcessCmdUpload() */
#define DBG_XCP_PROCESSCMDUPLOAD_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDDOWNLOAD_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdDownload() */
#define DBG_XCP_PROCESSCMDDOWNLOAD_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDDOWNLOAD_EXIT
/** \brief Exit point of function Xcp_ProcessCmdDownload() */
#define DBG_XCP_PROCESSCMDDOWNLOAD_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDDOWNLOADNEXT_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdDownloadNext() */
#define DBG_XCP_PROCESSCMDDOWNLOADNEXT_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDDOWNLOADNEXT_EXIT
/** \brief Exit point of function Xcp_ProcessCmdDownloadNext() */
#define DBG_XCP_PROCESSCMDDOWNLOADNEXT_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSONEDOWNLOAD_ENTRY
/** \brief Entry point of function Xcp_ProcessOneDownload() */
#define DBG_XCP_PROCESSONEDOWNLOAD_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_PROCESSONEDOWNLOAD_EXIT
/** \brief Exit point of function Xcp_ProcessOneDownload() */
#define DBG_XCP_PROCESSONEDOWNLOAD_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDTLSPECIFIC_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdTLSpecific() */
#define DBG_XCP_PROCESSCMDTLSPECIFIC_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDTLSPECIFIC_EXIT
/** \brief Exit point of function Xcp_ProcessCmdTLSpecific() */
#define DBG_XCP_PROCESSCMDTLSPECIFIC_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_ENTRY
/** \brief Entry point of function Xcp_ProcessSubCmdGetSlaveIdOnCAN() */
#define DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_EXIT
/** \brief Exit point of function Xcp_ProcessSubCmdGetSlaveIdOnCAN() */
#define DBG_XCP_PROCESSSUBCMDGETSLAVEIDONCAN_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_ENTRY
/** \brief Entry point of function Xcp_ProcessSubCmdGetSlaveIdOnEthernet() */
#define DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_EXIT
/** \brief Exit point of function Xcp_ProcessSubCmdGetSlaveIdOnEthernet() */
#define DBG_XCP_PROCESSSUBCMDGETSLAVEIDONETHERNET_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETDAQID_ENTRY
/** \brief Entry point of function Xcp_ProcessSubCmdGetDaqID() */
#define DBG_XCP_PROCESSSUBCMDGETDAQID_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSSUBCMDGETDAQID_EXIT
/** \brief Exit point of function Xcp_ProcessSubCmdGetDaqID() */
#define DBG_XCP_PROCESSSUBCMDGETDAQID_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDFREEDAQ_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdFreeDAQ() */
#define DBG_XCP_PROCESSCMDFREEDAQ_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDFREEDAQ_EXIT
/** \brief Exit point of function Xcp_ProcessCmdFreeDAQ() */
#define DBG_XCP_PROCESSCMDFREEDAQ_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCDAQ_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdAllocDAQ() */
#define DBG_XCP_PROCESSCMDALLOCDAQ_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCDAQ_EXIT
/** \brief Exit point of function Xcp_ProcessCmdAllocDAQ() */
#define DBG_XCP_PROCESSCMDALLOCDAQ_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCODT_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdAllocODT() */
#define DBG_XCP_PROCESSCMDALLOCODT_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCODT_EXIT
/** \brief Exit point of function Xcp_ProcessCmdAllocODT() */
#define DBG_XCP_PROCESSCMDALLOCODT_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCODTENTRY_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdAllocODTEntry() */
#define DBG_XCP_PROCESSCMDALLOCODTENTRY_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDALLOCODTENTRY_EXIT
/** \brief Exit point of function Xcp_ProcessCmdAllocODTEntry() */
#define DBG_XCP_PROCESSCMDALLOCODTENTRY_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSETDAQPTR_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetDAQPtr() */
#define DBG_XCP_PROCESSCMDSETDAQPTR_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETDAQPTR_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetDAQPtr() */
#define DBG_XCP_PROCESSCMDSETDAQPTR_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDWRITEDAQ_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdWriteDAQ() */
#define DBG_XCP_PROCESSCMDWRITEDAQ_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDWRITEDAQ_EXIT
/** \brief Exit point of function Xcp_ProcessCmdWriteDAQ() */
#define DBG_XCP_PROCESSCMDWRITEDAQ_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEED_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetSeed() */
#define DBG_XCP_PROCESSCMDGETSEED_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEED_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetSeed() */
#define DBG_XCP_PROCESSCMDGETSEED_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDUNLOCK_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdUnlock() */
#define DBG_XCP_PROCESSCMDUNLOCK_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDUNLOCK_EXIT
/** \brief Exit point of function Xcp_ProcessCmdUnlock() */
#define DBG_XCP_PROCESSCMDUNLOCK_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQCLOCK_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetDAQClock() */
#define DBG_XCP_PROCESSCMDGETDAQCLOCK_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETDAQCLOCK_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetDAQClock() */
#define DBG_XCP_PROCESSCMDGETDAQCLOCK_EXIT(a)
#endif

#ifndef DBG_XCP_UINT16FROMCMD_ENTRY
/** \brief Entry point of function Xcp_Uint16FromCmd() */
#define DBG_XCP_UINT16FROMCMD_ENTRY(a)
#endif

#ifndef DBG_XCP_UINT16FROMCMD_EXIT
/** \brief Exit point of function Xcp_Uint16FromCmd() */
#define DBG_XCP_UINT16FROMCMD_EXIT(a,b)
#endif

#ifndef DBG_XCP_UINT32FROMCMD_ENTRY
/** \brief Entry point of function Xcp_Uint32FromCmd() */
#define DBG_XCP_UINT32FROMCMD_ENTRY(a)
#endif

#ifndef DBG_XCP_UINT32FROMCMD_EXIT
/** \brief Exit point of function Xcp_Uint32FromCmd() */
#define DBG_XCP_UINT32FROMCMD_EXIT(a,b)
#endif

#ifndef DBG_XCP_PROCESSCMDCOPYCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdCopyCalPage() */
#define DBG_XCP_PROCESSCMDCOPYCALPAGE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDCOPYCALPAGE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdCopyCalPage() */
#define DBG_XCP_PROCESSCMDCOPYCALPAGE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETPAGPROCINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetPagProcInfo() */
#define DBG_XCP_PROCESSCMDGETPAGPROCINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETPAGPROCINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetPagProcInfo() */
#define DBG_XCP_PROCESSCMDGETPAGPROCINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetCalPage() */
#define DBG_XCP_PROCESSCMDGETCALPAGE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETCALPAGE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetCalPage() */
#define DBG_XCP_PROCESSCMDGETCALPAGE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSETCALPAGE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetCalPage() */
#define DBG_XCP_PROCESSCMDSETCALPAGE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETCALPAGE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetCalPage() */
#define DBG_XCP_PROCESSCMDSETCALPAGE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEGMENTMODE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetSegmentMode() */
#define DBG_XCP_PROCESSCMDGETSEGMENTMODE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEGMENTMODE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetSegmentMode() */
#define DBG_XCP_PROCESSCMDGETSEGMENTMODE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSETSEGMENTMODE_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetSegmentMode() */
#define DBG_XCP_PROCESSCMDSETSEGMENTMODE_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETSEGMENTMODE_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetSegmentMode() */
#define DBG_XCP_PROCESSCMDSETSEGMENTMODE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEGMENTINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetSegmentInfo() */
#define DBG_XCP_PROCESSCMDGETSEGMENTINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSEGMENTINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetSegmentInfo() */
#define DBG_XCP_PROCESSCMDGETSEGMENTINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETPAGEINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetPageInfo() */
#define DBG_XCP_PROCESSCMDGETPAGEINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETPAGEINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetPageInfo() */
#define DBG_XCP_PROCESSCMDGETPAGEINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDBUILDCHECKSUM_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdBuildChecksum() */
#define DBG_XCP_PROCESSCMDBUILDCHECKSUM_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDBUILDCHECKSUM_EXIT
/** \brief Exit point of function Xcp_ProcessCmdBuildChecksum() */
#define DBG_XCP_PROCESSCMDBUILDCHECKSUM_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDMODIFYBITS_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdModifyBits() */
#define DBG_XCP_PROCESSCMDMODIFYBITS_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDMODIFYBITS_EXIT
/** \brief Exit point of function Xcp_ProcessCmdModifyBits() */
#define DBG_XCP_PROCESSCMDMODIFYBITS_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMMAX_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgramMax() */
#define DBG_XCP_PROCESSCMDPROGRAMMAX_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMMAX_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgramMax() */
#define DBG_XCP_PROCESSCMDPROGRAMMAX_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMNEXT_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgramNext() */
#define DBG_XCP_PROCESSCMDPROGRAMNEXT_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMNEXT_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgramNext() */
#define DBG_XCP_PROCESSCMDPROGRAMNEXT_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAM_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgram() */
#define DBG_XCP_PROCESSCMDPROGRAM_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAM_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgram() */
#define DBG_XCP_PROCESSCMDPROGRAM_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMRESET_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgramReset() */
#define DBG_XCP_PROCESSCMDPROGRAMRESET_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMRESET_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgramReset() */
#define DBG_XCP_PROCESSCMDPROGRAMRESET_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMCLEAR_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgramClear() */
#define DBG_XCP_PROCESSCMDPROGRAMCLEAR_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMCLEAR_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgramClear() */
#define DBG_XCP_PROCESSCMDPROGRAMCLEAR_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMSTART_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdProgramStart() */
#define DBG_XCP_PROCESSCMDPROGRAMSTART_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDPROGRAMSTART_EXIT
/** \brief Exit point of function Xcp_ProcessCmdProgramStart() */
#define DBG_XCP_PROCESSCMDPROGRAMSTART_EXIT(a)
#endif

#ifndef DBG_XCP_ABORTCOMMANDWITHERROR_ENTRY
/** \brief Entry point of function Xcp_AbortCommandWithError() */
#define DBG_XCP_ABORTCOMMANDWITHERROR_ENTRY(a)
#endif

#ifndef DBG_XCP_ABORTCOMMANDWITHERROR_EXIT
/** \brief Exit point of function Xcp_AbortCommandWithError() */
#define DBG_XCP_ABORTCOMMANDWITHERROR_EXIT()
#endif

#ifndef DBG_XCP_COMMANDCANBEEXECUTED_ENTRY
/** \brief Entry point of function Xcp_CommandCanBeExecuted(a,b) */
#define DBG_XCP_COMMANDCANBEEXECUTED_ENTRY(a)
#endif

#ifndef DBG_XCP_COMMANDCANBEEXECUTED_EXIT
/** \brief Exit point of function Xcp_CommandCanBeExecuted(a,b,c) */
#define DBG_XCP_COMMANDCANBEEXECUTED_EXIT(a,b)
#endif

#ifndef DBG_XCP_PROGRAMFLASH_ENTRY
/** \brief Entry point of function Xcp_ProgramFlash() */
#define DBG_XCP_PROGRAMFLASH_ENTRY(a,b)
#endif

#ifndef DBG_XCP_PROGRAMFLASH_EXIT
/** \brief Exit point of function Xcp_ProgramFlash() */
#define DBG_XCP_PROGRAMFLASH_EXIT()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSECTORINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetSectorInfo() */
#define DBG_XCP_PROCESSCMDGETSECTORINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETSECTORINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetSectorInfo() */
#define DBG_XCP_PROCESSCMDGETSECTORINFO_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDGETPGMPROCINFO_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdGetPgmProcInfo() */
#define DBG_XCP_PROCESSCMDGETPGMPROCINFO_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDGETPGMPROCINFO_EXIT
/** \brief Exit point of function Xcp_ProcessCmdGetPgmProcInfo() */
#define DBG_XCP_PROCESSCMDGETPGMPROCINFO_EXIT(a)
#endif

#ifndef DBG_XCP_ISPROGRAMMINGBLOCKMODE_ENTRY
/** \brief Entry point of function Xcp_IsProgrammingBlockMode() */
#define DBG_XCP_ISPROGRAMMINGBLOCKMODE_ENTRY()
#endif

#ifndef DBG_XCP_ISPROGRAMMINGBLOCKMODE_EXIT
/** \brief Exit point of function Xcp_IsProgrammingBlockMode() */
#define DBG_XCP_ISPROGRAMMINGBLOCKMODE_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDSETREQUEST_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdSetRequest() */
#define DBG_XCP_PROCESSCMDSETREQUEST_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDSETREQUEST_EXIT
/** \brief Exit point of function Xcp_ProcessCmdSetRequest() */
#define DBG_XCP_PROCESSCMDSETREQUEST_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSCMDUSERDEFINED_ENTRY
/** \brief Entry point of function Xcp_ProcessCmdUserDefined() */
#define DBG_XCP_PROCESSCMDUSERDEFINED_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSCMDUSERDEFINED_EXIT
/** \brief Exit point of function Xcp_ProcessCmdUserDefined() */
#define DBG_XCP_PROCESSCMDUSERDEFINED_EXIT(a)
#endif

#ifndef DBG_XCP_STARTSTOREDAQ_ENTRY
/** \brief Entry point of function Xcp_StartStoreDaq() */
#define DBG_XCP_STARTSTOREDAQ_ENTRY(a,b)
#endif

#ifndef DBG_XCP_STARTSTOREDAQ_EXIT
/** \brief Exit point of function Xcp_StartStoreDaq() */
#define DBG_XCP_STARTSTOREDAQ_EXIT(a,b)
#endif

#ifndef DBG_XCP_STARTCLEARDAQ_ENTRY
/** \brief Entry point of function Xcp_StartClearDaq() */
#define DBG_XCP_STARTCLEARDAQ_ENTRY(a)
#endif

#ifndef DBG_XCP_STARTCLEARDAQ_EXIT
/** \brief Exit point of function Xcp_StartClearDaq() */
#define DBG_XCP_STARTCLEARDAQ_EXIT(a)
#endif

#ifndef DBG_XCP_STARTSTORECAL_ENTRY
/** \brief Entry point of function Xcp_StartStoreCal() */
#define DBG_XCP_STARTSTORECAL_ENTRY(a)
#endif

#ifndef DBG_XCP_STARTSTORECAL_EXIT
/** \brief Exit point of function Xcp_StartStoreCal() */
#define DBG_XCP_STARTSTORECAL_EXIT(a)
#endif

#ifndef DBG_XCP_SETEVENT_ENTRY
/** \brief Entry point of function Xcp_SetEvent() */
#define DBG_XCP_SETEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_SETEVENT_EXIT
/** \brief Exit point of function Xcp_SetEvent() */
#define DBG_XCP_SETEVENT_EXIT(a)
#endif

#ifndef DBG_XCP_SETCYCLICEVENTS_ENTRY
/** \brief Entry point of function Xcp_SetCyclicEvents() */
#define DBG_XCP_SETCYCLICEVENTS_ENTRY()
#endif

#ifndef DBG_XCP_SETCYCLICEVENTS_EXIT
/** \brief Exit point of function Xcp_SetCyclicEvents() */
#define DBG_XCP_SETCYCLICEVENTS_EXIT()
#endif

#ifndef DBG_XCP_SETONECYCLICEVENT_ENTRY
/** \brief Entry point of function Xcp_SetOneCyclicEvent() */
#define DBG_XCP_SETONECYCLICEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_SETONECYCLICEVENT_EXIT
/** \brief Exit point of function Xcp_SetOneCyclicEvent() */
#define DBG_XCP_SETONECYCLICEVENT_EXIT()
#endif

#ifndef DBG_XCP_MARKEVENTASSET_ENTRY
/** \brief Entry point of function Xcp_MarkEventAsSet() */
#define DBG_XCP_MARKEVENTASSET_ENTRY(a)
#endif

#ifndef DBG_XCP_MARKEVENTASSET_EXIT
/** \brief Exit point of function Xcp_MarkEventAsSet() */
#define DBG_XCP_MARKEVENTASSET_EXIT(a)
#endif

#ifndef DBG_XCP_CHECKDAQLISTACTIVE_ENTRY
/** \brief Entry point of function Xcp_CheckDaqListActive() */
#define DBG_XCP_CHECKDAQLISTACTIVE_ENTRY(a)
#endif

#ifndef DBG_XCP_CHECKDAQLISTACTIVE_EXIT
/** \brief Exit point of function Xcp_CheckDaqListActive() */
#define DBG_XCP_CHECKDAQLISTACTIVE_EXIT(a)
#endif

#ifndef DBG_XCP_INITRECEIVEPROCESSOR_ENTRY
/** \brief Entry point of function Xcp_InitReceiveProcessor() */
#define DBG_XCP_INITRECEIVEPROCESSOR_ENTRY()
#endif

#ifndef DBG_XCP_INITRECEIVEPROCESSOR_EXIT
/** \brief Exit point of function Xcp_InitReceiveProcessor() */
#define DBG_XCP_INITRECEIVEPROCESSOR_EXIT()
#endif

#ifndef DBG_XCP_RECEIVERXDATA_ENTRY
/** \brief Entry point of function Xcp_ReceiveRxData() */
#define DBG_XCP_RECEIVERXDATA_ENTRY(a,b)
#endif

#ifndef DBG_XCP_RECEIVERXDATA_EXIT
/** \brief Exit point of function Xcp_ReceiveRxData() */
#define DBG_XCP_RECEIVERXDATA_EXIT()
#endif

#ifndef DBG_XCP_PROCESSRECEPTION_ENTRY
/** \brief Entry point of function Xcp_ProcessReception() */
#define DBG_XCP_PROCESSRECEPTION_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSRECEPTION_EXIT
/** \brief Exit point of function Xcp_ProcessReception() */
#define DBG_XCP_PROCESSRECEPTION_EXIT()
#endif

#ifndef DBG_XCP_PROCESSRXPDU_ENTRY
/** \brief Entry point of function Xcp_ProcessRxPdu() */
#define DBG_XCP_PROCESSRXPDU_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_PROCESSRXPDU_EXIT
/** \brief Exit point of function Xcp_ProcessRxPdu() */
#define DBG_XCP_PROCESSRXPDU_EXIT(a)
#endif

#ifndef DBG_XCP_UNPACKRXBUFFERDATA_ENTRY
/** \brief Entry point of function Xcp_UnpackRxBufferData() */
#define DBG_XCP_UNPACKRXBUFFERDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_UNPACKRXBUFFERDATA_EXIT
/** \brief Exit point of function Xcp_UnpackRxBufferData() */
#define DBG_XCP_UNPACKRXBUFFERDATA_EXIT()
#endif

#ifndef DBG_XCP_GETRCVACTION_ENTRY
/** \brief Entry point of function Xcp_GetRcvAction() */
#define DBG_XCP_GETRCVACTION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETRCVACTION_EXIT
/** \brief Exit point of function Xcp_GetRcvAction() */
#define DBG_XCP_GETRCVACTION_EXIT(a,b)
#endif

#ifndef DBG_XCP_PROCESSRCVACTION_ENTRY
/** \brief Entry point of function Xcp_ProcessRcvAction() */
#define DBG_XCP_PROCESSRCVACTION_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_PROCESSRCVACTION_EXIT
/** \brief Exit point of function Xcp_ProcessRcvAction() */
#define DBG_XCP_PROCESSRCVACTION_EXIT()
#endif

#ifndef DBG_XCP_COPYSOADREMOTEADDRESS_ENTRY
/** \brief Entry point of function Xcp_CopySoAdRemoteAddress() */
#define DBG_XCP_COPYSOADREMOTEADDRESS_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_COPYSOADREMOTEADDRESS_EXIT
/** \brief Exit point of function Xcp_CopySoAdRemoteAddress() */
#define DBG_XCP_COPYSOADREMOTEADDRESS_EXIT()
#endif

#ifndef DBG_XCP_SOADCHANGETXTOMULTICAST_ENTRY
/** \brief Entry point of function Xcp_SoAdChangeAddressToMulticast() */
#define DBG_XCP_SOADCHANGETXTOMULTICAST_ENTRY();
#endif

#ifndef DBG_XCP_SOADCHANGETXTOMULTICAST_EXIT
/** \brief Exit point of function Xcp_SoAdChangeTxToMulticast() */
#define DBG_XCP_SOADCHANGETXTOMULTICAST_EXIT(a)
#endif

#ifndef DBG_XCP_SETMULTICASTADDRESS_ENTRY
/** \brief Entry point of function Xcp_SetMulicastAddress() */
#define DBG_XCP_SETMULTICASTADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SETMULTICASTADDRESS_EXIT
/** \brief Exit point of function Xcp_SetMulicastAddress() */
#define DBG_XCP_SETMULTICASTADDRESS_EXIT(a,b)
#endif

#ifndef DBG_XCP_SOADCHANGETXTOUNICAST_ENTRY
/** \brief Entry point of function Xcp_SoAdChangeTxToUnicast() */
#define DBG_XCP_SOADCHANGETXTOUNICAST_ENTRY()
#endif

#ifndef DBG_XCP_SOADCHANGETXTOUNICAST_EXIT
/** \brief Exit point of function Xcp_SoAdChangeTxToUnicast() */
#define DBG_XCP_SOADCHANGETXTOUNICAST_EXIT(a)
#endif

#ifndef DBG_XCP_SOADGETLOCALADDRESS_ENTRY
/** \brief Entry point of function Xcp_GetLocalAddress() */
#define DBG_XCP_SOADGETLOCALADDRESS_ENTRY(b)
#endif

#ifndef DBG_XCP_SOADGETLOCALADDRESS_EXIT
/** \brief Exit point of function Xcp_GetLocalAddress() */
#define DBG_XCP_SOADGETLOCALADDRESS_EXIT(a,b)
#endif

#ifndef DBG_XCP_ISCMDALLOWED_ENTRY
/** \brief Entry point of function Xcp_IsCmdAllowed() */
#define DBG_XCP_ISCMDALLOWED_ENTRY(a,b)
#endif

#ifndef DBG_XCP_ISCMDALLOWED_EXIT
/** \brief Exit point of function Xcp_IsCmdAllowed() */
#define DBG_XCP_ISCMDALLOWED_EXIT(a)
#endif

#ifndef DBG_XCP_ISCTOPDUACCEPTED_ENTRY
/** \brief Entry point of function Xcp_IsCtoPduAccepted() */
#define DBG_XCP_ISCTOPDUACCEPTED_ENTRY(a,b)
#endif

#ifndef DBG_XCP_ISCTOPDUACCEPTED_EXIT
/** \brief Exit point of function Xcp_IsCtoPduAccepted() */
#define DBG_XCP_ISCTOPDUACCEPTED_EXIT(a)
#endif

#ifndef DBG_XCP_INSERTSTIM_ENTRY
/** \brief Entry point of function Xcp_InsertSTIM() */
#define DBG_XCP_INSERTSTIM_ENTRY(a)
#endif

#ifndef DBG_XCP_INSERTSTIM_EXIT
/** \brief Exit point of function Xcp_InsertSTIM() */
#define DBG_XCP_INSERTSTIM_EXIT()
#endif

#ifndef DBG_XCP_GETSTIMLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetStimLength() */
#define DBG_XCP_GETSTIMLENGTH_ENTRY(a)
#endif

#ifndef DBG_XCP_GETSTIMLENGTH_EXIT
/** \brief Exit point of function Xcp_GetStimLength() */
#define DBG_XCP_GETSTIMLENGTH_EXIT(a,b)
#endif

#ifndef DBG_XCP_STIMULATEDATA_ENTRY
/** \brief Entry point of function Xcp_StimulateData() */
#define DBG_XCP_STIMULATEDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_STIMULATEDATA_EXIT
/** \brief Exit point of function Xcp_StimulateData() */
#define DBG_XCP_STIMULATEDATA_EXIT()
#endif

#ifndef DBG_XCP_INSERTODTTOSTIMBUFFER_ENTRY
/** \brief Entry point of function Xcp_InsertOdtToStimBuffer() */
#define DBG_XCP_INSERTODTTOSTIMBUFFER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_INSERTODTTOSTIMBUFFER_EXIT
/** \brief Exit point of function Xcp_InsertOdtToStimBuffer() */
#define DBG_XCP_INSERTODTTOSTIMBUFFER_EXIT()
#endif

#ifndef DBG_XCP_GETSTIMDTOLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetStimDtoLength() */
#define DBG_XCP_GETSTIMDTOLENGTH_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETSTIMDTOLENGTH_EXIT
/** \brief Exit point of function Xcp_GetStimDtoLength() */
#define DBG_XCP_GETSTIMDTOLENGTH_EXIT(a,b)
#endif

#ifndef DBG_XCP_STIMULATEODT_ENTRY
/** \brief Entry point of function Xcp_StimulateOdt() */
#define DBG_XCP_STIMULATEODT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_STIMULATEODT_EXIT
/** \brief Exit point of function Xcp_StimulateOdt() */
#define DBG_XCP_STIMULATEODT_EXIT()
#endif

#ifndef DBG_XCP_STIMULATEDAQ_ENTRY
/** \brief Entry point of function Xcp_StimulateDaq() */
#define DBG_XCP_STIMULATEDAQ_ENTRY(a)
#endif

#ifndef DBG_XCP_STIMULATEDAQ_EXIT
/** \brief Exit point of function Xcp_StimulateDaq() */
#define DBG_XCP_STIMULATEDAQ_EXIT()
#endif

#ifndef DBG_XCP_STIMULATEEVENT_ENTRY
/** \brief Entry point of function Xcp_StimulateEvent() */
#define DBG_XCP_STIMULATEEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_STIMULATEEVENT_EXIT
/** \brief Exit point of function Xcp_StimulateEvent() */
#define DBG_XCP_STIMULATEEVENT_EXIT()
#endif

#ifndef DBG_XCP_INITFRAMECTR_ENTRY
/** \brief Entry point of function Xcp_InitFrameCtr() */
#define DBG_XCP_INITFRAMECTR_ENTRY()
#endif

#ifndef DBG_XCP_INITFRAMECTR_EXIT
/** \brief Exit point of function Xcp_InitFrameCtr() */
#define DBG_XCP_INITFRAMECTR_EXIT()
#endif

#ifndef DBG_XCP_PROCESSTRANSMIT_ENTRY
/** \brief Entry point of function Xcp_ProcessTransmit() */
#define DBG_XCP_PROCESSTRANSMIT_ENTRY(a)
#endif

#ifndef DBG_XCP_PROCESSTRANSMIT_EXIT
/** \brief Exit point of function Xcp_ProcessTransmit() */
#define DBG_XCP_PROCESSTRANSMIT_EXIT()
#endif

#ifndef DBG_XCP_INITTRANSMITPROCESSOR_ENTRY
/** \brief Entry point of function Xcp_InitTransmitProcessor() */
#define DBG_XCP_INITTRANSMITPROCESSOR_ENTRY()
#endif

#ifndef DBG_XCP_INITTRANSMITPROCESSOR_EXIT
/** \brief Exit point of function Xcp_InitTransmitProcessor() */
#define DBG_XCP_INITTRANSMITPROCESSOR_EXIT()
#endif

#ifndef DBG_XCP_PROVIDETXDATA_ENTRY
/** \brief Entry point of function Xcp_ProvideTxData() */
#define DBG_XCP_PROVIDETXDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_PROVIDETXDATA_EXIT
/** \brief Exit point of function Xcp_ProvideTxData() */
#define DBG_XCP_PROVIDETXDATA_EXIT(a,b)
#endif

#ifndef DBG_XCP_INDICATETXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_IndicateTxConfirmation() */
#define DBG_XCP_INDICATETXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_INDICATETXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_IndicateTxConfirmation() */
#define DBG_XCP_INDICATETXCONFIRMATION_EXIT()
#endif

#ifndef DBG_XCP_TRANSMITAVAILABLETXPDUS_ENTRY
/** \brief Entry point of function Xcp_TransmitAvailableTxPdus() */
#define DBG_XCP_TRANSMITAVAILABLETXPDUS_ENTRY(a, b)
#endif

#ifndef DBG_XCP_TRANSMITAVAILABLETXPDUS_EXIT
/** \brief Exit point of function Xcp_TransmitAvailableTxPdus() */
#define DBG_XCP_TRANSMITAVAILABLETXPDUS_EXIT()
#endif

#ifndef DBG_XCP_UPDATEAVAILABLETXBUFFERS_ENTRY
/** \brief Entry point of function Xcp_UpdateAvailableTxBuffers() */
#define DBG_XCP_UPDATEAVAILABLETXBUFFERS_ENTRY(a)
#endif

#ifndef DBG_XCP_UPDATEAVAILABLETXBUFFERS_EXIT
/** \brief Exit point of function Xcp_UpdateAvailableTxBuffers() */
#define DBG_XCP_UPDATEAVAILABLETXBUFFERS_EXIT()
#endif

#ifndef DBG_XCP_PROCESSRETRY_ENTRY
/** \brief Entry point of function Xcp_ProcessRetry() */
#define DBG_XCP_PROCESSRETRY_ENTRY(a,b)
#endif

#ifndef DBG_XCP_PROCESSRETRY_EXIT
/** \brief Exit point of function Xcp_ProcessRetry() */
#define DBG_XCP_PROCESSRETRY_EXIT(a)
#endif

#ifndef DBG_XCP_TRANSMIT_ENTRY
/** \brief Entry point of function Xcp_Transmit() */
#define DBG_XCP_TRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_TRANSMIT_EXIT
/** \brief Exit point of function Xcp_Transmit() */
#define DBG_XCP_TRANSMIT_EXIT(a)
#endif

#ifndef DBG_XCP_SETPDUTRANSMITSTATUS_ENTRY
/** \brief Entry point of function Xcp_SetPduTransmitStatus() */
#define DBG_XCP_SETPDUTRANSMITSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SETPDUTRANSMITSTATUS_EXIT
/** \brief Exit point of function Xcp_SetPduTransmitStatus() */
#define DBG_XCP_SETPDUTRANSMITSTATUS_EXIT()
#endif

#ifndef DBG_XCP_ADDFRAMEHEADER_ENTRY
/** \brief Entry point of function Xcp_AddFrameHeader() */
#define DBG_XCP_ADDFRAMEHEADER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_XCP_ADDFRAMEHEADER_EXIT
/** \brief Exit point of function Xcp_AddFrameHeader() */
#define DBG_XCP_ADDFRAMEHEADER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_INITFLEXRAYBUFFER_ENTRY
/** \brief Entry point of function Xcp_InitFlexRayBuffer() */
#define DBG_XCP_INITFLEXRAYBUFFER_ENTRY()
#endif

#ifndef DBG_XCP_INITFLEXRAYBUFFER_EXIT
/** \brief Exit point of function Xcp_InitFlexRayBuffer() */
#define DBG_XCP_INITFLEXRAYBUFFER_EXIT(a)
#endif

#ifndef DBG_XCP_CLEARDYNAMICINFOBLOCK_ENTRY
/** \brief Entry point of function Xcp_ClearDynamicInfoBlock() */
#define DBG_XCP_CLEARDYNAMICINFOBLOCK_ENTRY()
#endif

#ifndef DBG_XCP_CLEARDYNAMICINFOBLOCK_EXIT
/** \brief Exit point of function Xcp_ClearDynamicInfoBlock() */
#define DBG_XCP_CLEARDYNAMICINFOBLOCK_EXIT()
#endif

#ifndef DBG_XCP_RESETEVENTPROCESSOR_ENTRY
/** \brief Entry point of function Xcp_ResetEventProcessor() */
#define DBG_XCP_RESETEVENTPROCESSOR_ENTRY()
#endif

#ifndef DBG_XCP_RESETEVENTPROCESSOR_EXIT
/** \brief Exit point of function Xcp_ResetEventProcessor() */
#define DBG_XCP_RESETEVENTPROCESSOR_EXIT()
#endif

#ifndef DBG_XCP_RESETDAQLISTDATA_ENTRY
/** \brief Entry point of function Xcp_ResetDaqListData() */
#define DBG_XCP_RESETDAQLISTDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_RESETDAQLISTDATA_EXIT
/** \brief Exit point of function Xcp_ResetDaqListData() */
#define DBG_XCP_RESETDAQLISTDATA_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSEVENTS_ENTRY
/** \brief Entry point of function Xcp_ProcessEvents() */
#define DBG_XCP_PROCESSEVENTS_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSEVENTS_EXIT
/** \brief Exit point of function Xcp_ProcessEvents() */
#define DBG_XCP_PROCESSEVENTS_EXIT()
#endif

#ifndef DBG_XCP_FLUSHDTORUNTIMEDATA_ENTRY
/** \brief Entry point of function Xcp_FlushDTORunTimeData() */
#define DBG_XCP_FLUSHDTORUNTIMEDATA_ENTRY()
#endif

#ifndef DBG_XCP_FLUSHDTORUNTIMEDATA_EXIT
/** \brief Exit point of function Xcp_FlushDTORunTimeData() */
#define DBG_XCP_FLUSHDTORUNTIMEDATA_EXIT()
#endif

#ifndef DBG_XCP_CLEARSTOPPEDDTOS_ENTRY
/** \brief Entry point of function Xcp_ClearStoppedDtos() */
#define DBG_XCP_CLEARSTOPPEDDTOS_ENTRY()
#endif

#ifndef DBG_XCP_CLEARSTOPPEDDTOS_EXIT
/** \brief Exit point of function Xcp_ClearStoppedDtos() */
#define DBG_XCP_CLEARSTOPPEDDTOS_EXIT()
#endif

#ifndef DBG_XCP_RESETEVENTCHANNELS_ENTRY
/** \brief Entry point of function Xcp_ResetEventChannels() */
#define DBG_XCP_RESETEVENTCHANNELS_ENTRY()
#endif

#ifndef DBG_XCP_RESETEVENTCHANNELS_EXIT
/** \brief Exit point of function Xcp_ResetEventChannels() */
#define DBG_XCP_RESETEVENTCHANNELS_EXIT()
#endif

#ifndef DBG_XCP_GETDAQIDFROMDTO_ENTRY
/** \brief Entry point of function Xcp_GetDaqIDFromDto() */
#define DBG_XCP_GETDAQIDFROMDTO_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_GETDAQIDFROMDTO_EXIT
/** \brief Exit point of function Xcp_GetDaqIDFromDto() */
#define DBG_XCP_GETDAQIDFROMDTO_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_ISDAQREADYTOBESAMPLED_ENTRY
/** \brief Entry point of function Xcp_IsDAQReadyToBeSampled() */
#define DBG_XCP_ISDAQREADYTOBESAMPLED_ENTRY(a,b)
#endif

#ifndef DBG_XCP_ISDAQREADYTOBESAMPLED_EXIT
/** \brief Exit point of function Xcp_IsDAQReadyToBeSampled() */
#define DBG_XCP_ISDAQREADYTOBESAMPLED_EXIT(a,b)
#endif

#ifndef DBG_XCP_ISDAQPRIOQUEUEEMPTY_ENTRY
/** \brief Entry point of function Xcp_IsDaqPrioQueueEmpty() */
#define DBG_XCP_ISDAQPRIOQUEUEEMPTY_ENTRY()
#endif

#ifndef DBG_XCP_ISDAQPRIOQUEUEEMPTY_EXIT
/** \brief Exit point of function Xcp_IsDaqPrioQueueEmpty() */
#define DBG_XCP_ISDAQPRIOQUEUEEMPTY_EXIT(a)
#endif

#ifndef DBG_XCP_GETDTOLENGTH_ENTRY
/** \brief Entry point of function Xcp_GetDtoLength() */
#define DBG_XCP_GETDTOLENGTH_ENTRY()
#endif

#ifndef DBG_XCP_GETDTOLENGTH_EXIT
/** \brief Exit point of function Xcp_GetDtoLength() */
#define DBG_XCP_GETDTOLENGTH_EXIT(a)
#endif

#ifndef DBG_XCP_GETDTODATA_ENTRY
/** \brief Entry point of function Xcp_GetDtoData() */
#define DBG_XCP_GETDTODATA_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETDTODATA_EXIT
/** \brief Exit point of function Xcp_GetDtoData() */
#define DBG_XCP_GETDTODATA_EXIT(a)
#endif

#ifndef DBG_XCP_UPDATEEVENT_ENTRY
/** \brief Entry point of function Xcp_UpdateEvent() */
#define DBG_XCP_UPDATEEVENT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_UPDATEEVENT_EXIT
/** \brief Exit point of function Xcp_UpdateEvent() */
#define DBG_XCP_UPDATEEVENT_EXIT()
#endif

#ifndef DBG_XCP_UPDATEDAQ_ENTRY
/** \brief Entry point of function Xcp_UpdateDaq() */
#define DBG_XCP_UPDATEDAQ_ENTRY(a,b)
#endif

#ifndef DBG_XCP_UPDATEDAQ_EXIT
/** \brief Exit point of function Xcp_UpdateDaq() */
#define DBG_XCP_UPDATEDAQ_EXIT(a,b)
#endif

#ifndef DBG_XCP_UPDATEODT_ENTRY
/** \brief Entry point of function Xcp_UpdateOdt() */
#define DBG_XCP_UPDATEODT_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_UPDATEODT_EXIT
/** \brief Exit point of function Xcp_UpdateOdt() */
#define DBG_XCP_UPDATEODT_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_SAMPLEDATA_ENTRY
/** \brief Entry point of function Xcp_SampleData() */
#define DBG_XCP_SAMPLEDATA_ENTRY(a)
#endif

#ifndef DBG_XCP_SAMPLEDATA_EXIT
/** \brief Exit point of function Xcp_SampleData() */
#define DBG_XCP_SAMPLEDATA_EXIT()
#endif

#ifndef DBG_XCP_SAMPLEDTODATA_ENTRY
/** \brief Entry point of function Xcp_SampleDtoData() */
#define DBG_XCP_SAMPLEDTODATA_ENTRY(a,b)
#endif

#ifndef DBG_XCP_SAMPLEDTODATA_EXIT
/** \brief Exit point of function Xcp_SampleDtoData() */
#define DBG_XCP_SAMPLEDTODATA_EXIT()
#endif

#ifndef DBG_XCP_SAMPLEDAQ_ENTRY
/** \brief Entry point of function Xcp_SampleDaq() */
#define DBG_XCP_SAMPLEDAQ_ENTRY(a)
#endif

#ifndef DBG_XCP_SAMPLEDAQ_EXIT
/** \brief Exit point of function Xcp_SampleDaq() */
#define DBG_XCP_SAMPLEDAQ_EXIT()
#endif

#ifndef DBG_XCP_SAMPLEEVENT_ENTRY
/** \brief Entry point of function Xcp_SampleEvent() */
#define DBG_XCP_SAMPLEEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_SAMPLEEVENT_EXIT
/** \brief Exit point of function Xcp_SampleEvent() */
#define DBG_XCP_SAMPLEEVENT_EXIT()
#endif

#ifndef DBG_XCP_CLEARDAQLISTDTOS_ENTRY
/** \brief Entry point of function Xcp_ClearStoppedDtos() */
#define DBG_XCP_CLEARDAQLISTDTOS_ENTRY(a)
#endif

#ifndef DBG_XCP_CLEARDAQLISTDTOS_EXIT
/** \brief Exit point of function Xcp_ClearStoppedDtos() */
#define DBG_XCP_CLEARDAQLISTDTOS_EXIT()
#endif


#ifndef DBG_XCP_GETNEVENTPROCREADYSTATUS_ENTRY
/** \brief Entry point of function Xcp_GetEventProcReadyStatus() */
#define DBG_XCP_GETNEVENTPROCREADYSTATUS_ENTRY(a)
#endif

#ifndef DBG_XCP_GETNEVENTPROCREADYSTATUS_EXIT
/** \brief Exit point of function Xcp_GetEventProcReadyStatus() */
#define DBG_XCP_GETNEVENTPROCREADYSTATUS_EXIT(a)
#endif

#ifndef DBG_XCP_GETNEXTDAQODT_ENTRY
/** \brief Entry point of function Xcp_GetNextDaqOdt() */
#define DBG_XCP_GETNEXTDAQODT_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_GETNEXTDAQODT_EXIT
/** \brief Exit point of function Xcp_GetNextDaqOdt() */
#define DBG_XCP_GETNEXTDAQODT_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_GETNEXTSTIMODT_ENTRY
/** \brief Entry point of function Xcp_GetNextStimOdt() */
#define DBG_XCP_GETNEXTSTIMODT_ENTRY(a,b,c)
#endif

#ifndef DBG_XCP_GETNEXTSTIMODT_EXIT
/** \brief Exit point of function Xcp_GetNextStimOdt() */
#define DBG_XCP_GETNEXTSTIMODT_EXIT(a,b,c,d)
#endif

#ifndef DBG_XCP_GETEVENTPROCSTATUS_ENTRY
/** \brief Entry point of function Xcp_GetEventProcStatus() */
#define DBG_XCP_GETEVENTPROCSTATUS_ENTRY(a)
#endif

#ifndef DBG_XCP_GETEVENTPROCSTATUS_EXIT
/** \brief Exit point of function Xcp_GetEventProcStatus() */
#define DBG_XCP_GETEVENTPROCSTATUS_EXIT(a)
#endif

#ifndef DBG_XCP_GETNEXTDAQLISTPROCSTATUS_ENTRY
/** \brief Entry point of function Xcp_GetNextDaqListProcStatus() */
#define DBG_XCP_GETNEXTDAQLISTPROCSTATUS_ENTRY(a)
#endif

#ifndef DBG_XCP_GETNEXTDAQLISTPROCSTATUS_EXIT
/** \brief Exit point of function Xcp_GetNextDaqListProcStatus() */
#define DBG_XCP_GETNEXTDAQLISTPROCSTATUS_EXIT(a)
#endif

#ifndef DBG_XCP_GETDTOHEADERSIZE_ENTRY
/** \brief Entry point of function Xcp_GetDtoHeaderSize() */
#define DBG_XCP_GETDTOHEADERSIZE_ENTRY(a,b)
#endif

#ifndef DBG_XCP_GETDTOHEADERSIZE_EXIT
/** \brief Exit point of function Xcp_GetDtoHeaderSize() */
#define DBG_XCP_GETDTOHEADERSIZE_EXIT(a)
#endif

#ifndef DBG_XCP_EMITEVENTSTIMTIMEOUT_ENTRY
/** \brief Entry point of function Xcp_EmitEventSTIMTimeout() */
#define DBG_XCP_EMITEVENTSTIMTIMEOUT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_EMITEVENTSTIMTIMEOUT_EXIT
/** \brief Exit point of function Xcp_EmitEventSTIMTimeout() */
#define DBG_XCP_EMITEVENTSTIMTIMEOUT_EXIT()
#endif

#ifndef DBG_XCP_EMITDAQOVERLOADEVENT_ENTRY
/** \brief Entry point of function Xcp_EmitDAQOverloadEvent() */
#define DBG_XCP_EMITDAQOVERLOADEVENT_ENTRY()
#endif

#ifndef DBG_XCP_EMITDAQOVERLOADEVENT_EXIT
/** \brief Exit point of function Xcp_EmitDAQOverloadEvent() */
#define DBG_XCP_EMITDAQOVERLOADEVENT_EXIT()
#endif

#ifndef DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_ENTRY
/** \brief Entry point of function Xcp_IsAllowedToTransmitMulticast() */
#define DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_ENTRY(a)
#endif

#ifndef DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_EXIT
/** \brief Exit point of function Xcp_IsAllowedToTransmitMulticast() */
#define DBG_XCP_ISALLOWEDTOTRANSMITMULTICAST_EXIT(a,b)
#endif

#ifndef DBG_XCP_ISMULTICASTRESPONSETOBESKIPPED_ENTRY
/** \brief Entry point of function Xcp_IsMulticastResponseToBeSkipped() */
#define DBG_XCP_ISMULTICASTRESPONSETOBESKIPPED_ENTRY()
#endif

#ifndef DBG_XCP_ISMULTICASTRESPONSETOBESKIPPED_EXIT
/** \brief Exit point of function Xcp_IsMulticastResponseToBeSkipped() */
#define DBG_XCP_ISMULTICASTRESPONSETOBESKIPPED_EXIT(a)
#endif

#ifndef DBG_XCP_ISSTIMPROCREADY_ENTRY
/** \brief Entry point of function Xcp_IsStimProcReady() */
#define DBG_XCP_ISSTIMPROCREADY_ENTRY(a,b)
#endif

#ifndef DBG_XCP_ISSTIMPROCREADY_EXIT
/** \brief Exit point of function Xcp_IsStimProcReady() */
#define DBG_XCP_ISSTIMPROCREADY_EXIT(a)
#endif

#ifndef DBG_XCP_PROCESSBUSMONITOR_ENTRY
/** \brief Entry point of function Xcp_ProcessBusMonitor() */
#define DBG_XCP_PROCESSBUSMONITOR_ENTRY()
#endif

#ifndef DBG_XCP_PROCESSBUSMONITOR_EXIT
/** \brief Exit point of function Xcp_ProcessBusMonitor() */
#define DBG_XCP_PROCESSBUSMONITOR_EXIT()
#endif

#ifndef DBG_XCP_CONTROL_ENTRY
/** \brief Entry point of function Xcp_Control() */
#define DBG_XCP_CONTROL_ENTRY(a)
#endif

#ifndef DBG_XCP_CONTROL_EXIT
/** \brief Exit point of function Xcp_Control() */
#define DBG_XCP_CONTROL_EXIT(a)
#endif

#ifndef DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_ENTRY
/** \brief Entry point of function Xcp_EnableCommunicationChannel() */
#define DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_ENTRY(a,b,c,d)
#endif

#ifndef DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_EXIT
/** \brief Exit point of function Xcp_EnableCommunicationChannel() */
#define DBG_XCP_ENABLE_COMMUNICATION_CHANNEL_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_ENTRY
/** \brief Entry point of function Xcp_EnableTxCommunicationChannel() */
#define DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_ENTRY(a)
#endif

#ifndef DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_EXIT
/** \brief Exit point of function Xcp_EnableTxCommunicationChannel() */
#define DBG_XCP_ENABLE_TX_COMMUNICATION_CHANNEL_EXIT(a,b)
#endif

#ifndef DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_ENTRY
/** \brief Entry point of function Xcp_EnableRxCommunicationChannel() */
#define DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_ENTRY(a)
#endif

#ifndef DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_EXIT
/** \brief Exit point of function Xcp_EnableRxCommunicationChannel() */
#define DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_EXIT(a,b)
#endif

#ifndef DBG_XCP_TXMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_TxMainFunction() */
#define DBG_XCP_TXMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_TXMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_TxMainFunction() */
#define DBG_XCP_TXMAINFUNCTION_EXIT()
#endif

#ifndef DBG_XCP_RXMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_RxMainFunction() */
#define DBG_XCP_RXMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_RXMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_RxMainFunction() */
#define DBG_XCP_RXMAINFUNCTION_EXIT()
#endif


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef XCP_TRACE_H */
/*==================[end of file]===========================================*/
