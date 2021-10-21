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
#ifndef XCP_WRAPPER_TRACE_H
#define XCP_WRAPPER_TRACE_H

[!AUTOSPACING!][!//

/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>
[!ENDIF!]

/*==================[macros]================================================*/

#ifndef DBG_XCP_WRAPPER_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Xcp_Wrapper_GetVersionInfo() */
#define DBG_XCP_WRAPPER_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_GETVERSIONINFO_EXIT
/** \brief Exit point of function Xcp_Wrapper_GetVersionInfo() */
#define DBG_XCP_WRAPPER_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_XCP_WRAPPER_INIT_ENTRY
/** \brief Entry point of function Xcp_Wrapper_Init() */
#define DBG_XCP_WRAPPER_INIT_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_INIT_EXIT
/** \brief Exit point of function Xcp_Wrapper_Init() */
#define DBG_XCP_WRAPPER_INIT_EXIT(a)
#endif

#ifndef DBG_XCP_WRAPPER_MAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_MainFunction() */
#define DBG_XCP_WRAPPER_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_WRAPPER_MAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_Wrapper_MainFunction() */
#define DBG_XCP_WRAPPER_MAINFUNCTION_EXIT()
#endif

[!IF "as:modconf('Xcp')[1]/XcpGeneral/XcpEventTriggerMainFunc = 'true'"!]
  [!IF "(node:exists(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel)) and
        (num:i(count(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel/*)) > 0)"!]
    [!LOOP "as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpEventChannel/*"!]
      [!IF "XcpEventChannelTimeCycle > 0"!]

#ifndef DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_ENTRY
/** \brief Entry point of function Xcp_Wrapper_MainFunction_[!"@name"!]() */
#define DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_ENTRY()
#endif

#ifndef DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_EXIT
/** \brief Exit point of function Xcp_Wrapper_MainFunction_[!"@name"!]() */
#define DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_EXIT()
#endif
      [!ENDIF!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDIF!]

[!IF "as:modconf('Xcp')[1]/XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true'"!]
#ifndef DBG_XCP_WRAPPER_CRCMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_CrcMainFunction() */
#define DBG_XCP_WRAPPER_CRCMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_WRAPPER_CRCMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_Wrapper_CrcMainFunction() */
#define DBG_XCP_WRAPPER_CRCMAINFUNCTION_EXIT()
#endif
[!ENDIF!]

#ifndef DBG_XCP_WRAPPER_SOADSOCONMODECHG_ENTRY
/** \brief Entry point of function Xcp_Wrapper_SoAdSoConModeChg() */
#define DBG_XCP_WRAPPER_SOADSOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_SOADSOCONMODECHG_EXIT
/** \brief Exit point of function Xcp_Wrapper_SoAdSoConModeChg() */
#define DBG_XCP_WRAPPER_SOADSOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_CANIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_CanIfRxIndication() */
#define DBG_XCP_WRAPPER_CANIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_CANIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_CanIfRxIndication() */
#define DBG_XCP_WRAPPER_CANIFRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_FrIfRxIndication() */
#define DBG_XCP_WRAPPER_FRIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_FrIfRxIndication() */
#define DBG_XCP_WRAPPER_FRIFRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_SOADIFRXINDICATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_SoAdIfRxIndication() */
#define DBG_XCP_WRAPPER_SOADIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_SOADIFRXINDICATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_SoAdIfRxIndication() */
#define DBG_XCP_WRAPPER_SOADIFRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_CanIfTxConfirmation() */
#define DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_CanIfTxConfirmation() */
#define DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_FrIfTxConfirmation() */
#define DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_FrIfTxConfirmation() */
#define DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_SoAdIfTxConfirmation() */
#define DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_EXIT
/** \brief Exit point of function Xcp_Wrapper_SoAdIfTxConfirmation() */
#define DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function Xcp_Wrapper_FrIfTriggerTransmit() */
#define DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function Xcp_Wrapper_FrIfTriggerTransmit() */
#define DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_XCP_WRAPPER_SETEVENT_ENTRY
/** \brief Entry point of function Xcp_Wrapper_SetEvent() */
#define DBG_XCP_WRAPPER_SETEVENT_ENTRY(a)
#endif

#ifndef DBG_XCP_WRAPPER_SETEVENT_EXIT
/** \brief Exit point of function Xcp_Wrapper_SetEvent() */
#define DBG_XCP_WRAPPER_SETEVENT_EXIT(a,b)
#endif

#ifndef DBG_XCP_WRAPPER_TXMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_TxMainFunction() */
#define DBG_XCP_WRAPPER_TXMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_WRAPPER_TXMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_Wrapper_TxMainFunction() */
#define DBG_XCP_WRAPPER_TXMAINFUNCTION_EXIT()
#endif

#ifndef DBG_XCP_WRAPPER_RXMAINFUNCTION_ENTRY
/** \brief Entry point of function Xcp_Wrapper_RxMainFunction() */
#define DBG_XCP_WRAPPER_RXMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_XCP_WRAPPER_RXMAINFUNCTION_EXIT
/** \brief Exit point of function Xcp_Wrapper_RxMainFunction() */
#define DBG_XCP_WRAPPER_RXMAINFUNCTION_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef XCP_WRAPPER_TRACE_H */
/*==================[end of file]===========================================*/
