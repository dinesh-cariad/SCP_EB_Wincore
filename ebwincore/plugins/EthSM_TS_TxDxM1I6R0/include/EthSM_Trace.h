/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ETHSM_TRACE_H
#define ETHSM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
/*==================[macros]================================================*/

#ifndef DBG_ETHSM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function EthSM_IsValidConfig() */
#define DBG_ETHSM_ISVALIDCONFIG_ENTRY(a)
#endif

#ifndef DBG_ETHSM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function EthSM_IsValidConfig() */
#define DBG_ETHSM_ISVALIDCONFIG_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_INIT_ENTRY
/** \brief Entry point of function EthSM_Init() */
#define DBG_ETHSM_INIT_ENTRY(a)
#endif

#ifndef DBG_ETHSM_INIT_EXIT
/** \brief Exit point of function EthSM_Init() */
#define DBG_ETHSM_INIT_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function EthSM_GetVersionInfo() */
#define DBG_ETHSM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_ETHSM_GETVERSIONINFO_EXIT
/** \brief Exit point of function EthSM_GetVersionInfo() */
#define DBG_ETHSM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_ETHSM_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function EthSM_RequestComMode() */
#define DBG_ETHSM_REQUESTCOMMODE_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_REQUESTCOMMODE_EXIT
/** \brief Exit point of function EthSM_RequestComMode() */
#define DBG_ETHSM_REQUESTCOMMODE_EXIT(a,b,c)
#endif

#ifndef DBG_ETHSM_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function EthSM_GetCurrentComMode() */
#define DBG_ETHSM_GETCURRENTCOMMODE_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function EthSM_GetCurrentComMode() */
#define DBG_ETHSM_GETCURRENTCOMMODE_EXIT(a,b,c)
#endif

#ifndef DBG_ETHSM_TCPIPMODEINDICATION_ENTRY
/** \brief Entry point of function EthSM_TcpIpModeIndication() */
#define DBG_ETHSM_TCPIPMODEINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_TCPIPMODEINDICATION_EXIT
/** \brief Exit point of function EthSM_TcpIpModeIndication() */
#define DBG_ETHSM_TCPIPMODEINDICATION_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_TRCVLINKSTATECHG_ENTRY
/** \brief Entry point of function EthSM_TrcvLinkStateChg() */
#define DBG_ETHSM_TRCVLINKSTATECHG_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_TRCVLINKSTATECHG_EXIT
/** \brief Exit point of function EthSM_TrcvLinkStateChg() */
#define DBG_ETHSM_TRCVLINKSTATECHG_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_MAINFUNCTION_ENTRY
/** \brief Entry point of function EthSM_MainFunction() */
#define DBG_ETHSM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_ETHSM_MAINFUNCTION_EXIT
/** \brief Exit point of function EthSM_MainFunction() */
#define DBG_ETHSM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_ENTRY
/** \brief Entry point of function EthSM_Transition_Offline_To_WaitCtrlModeInd() */
#define DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_EXIT
/** \brief Exit point of function EthSM_Transition_Offline_To_WaitCtrlModeInd() */
#define DBG_ETHSM_TRANSITION_OFFLINE_TO_WAITCTRLMODEIND_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitCtrlModeInd_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_EXIT
/** \brief Exit point of function EthSM_Transition_WaitCtrlModeInd_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_WAITCTRLMODEIND_TO_WAITTRCVLINK_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_ENTRY
/** \brief Entry point of function EthSM_Transition_OnHold_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_EXIT
/** \brief Exit point of function EthSM_Transition_OnHold_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITLINKDOWN_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitLinkDown_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitLinkDown_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAITLINKDOWN_TO_OFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitOffline_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_EXIT
/** \brief Exit point of function EthSM_Transition_WaitOffline_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITLINKDOWN_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitTrcvLink_To_WaitCtrlMIndDown() */
#define DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_EXIT
/** \brief Exit point of function EthSM_Transition_WaitTrcvLink_To_WaitCtrlMIndDown() */
#define DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAIT_CTRLMIND_DOWN_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitCtrlMIndDown_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitCtrlMIndDown_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAIT_CTRLMIND_DOWN_TO_OFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitTrcvLink_To_WaitOnline() */
#define DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitTrcvLink_To_WaitOnline() */
#define DBG_ETHSM_TRANSITION_WAITTRCVLINK_TO_WAITONLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitOnline_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_EXIT
/** \brief Exit point of function EthSM_Transition_WaitOnline_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_WAITTRCVLINK_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitOnline_To_Online() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitOnline_To_Online() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_ONLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitOnline_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitOnline_To_Offline() */
#define DBG_ETHSM_TRANSITION_WAITONLINE_TO_OFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_Online_To_WaitOffline() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_Online_To_WaitOffline() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_WAITOFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_ENTRY
/** \brief Entry point of function EthSM_Transition_Online_To_OnHold() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_EXIT
/** \brief Exit point of function EthSM_Transition_Online_To_OnHold() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_ONHOLD_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_Online_To_WaitOnline() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_EXIT
/** \brief Exit point of function EthSM_Transition_Online_To_WaitOnline() */
#define DBG_ETHSM_TRANSITION_ONLINE_TO_WAITONLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_OnHold_To_Online() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_EXIT
/** \brief Exit point of function EthSM_Transition_OnHold_To_Online() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_ONLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_OFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_OnHold_To_Offline() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_OFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_OFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_OnHold_To_Offline() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_OFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_ENTRY
/** \brief Entry point of function EthSM_Transition_OnHold_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_EXIT
/** \brief Exit point of function EthSM_Transition_OnHold_To_WaitTrcvLink() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITTRCVLINK_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitOffline_To_WaitDevAuthOffline() */
#define DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_WaitOffline_To_WaitDevAuthOffline() */
#define DBG_ETHSM_TRANSITION_WAITOFFLINE_TO_WAITDEVAUTHOFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_ENTRY
/** \brief Entry point of function EthSM_Transition_OnHold_To_WaitDevAuthOffline() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_EXIT
/** \brief Exit point of function EthSM_Transition_OnHold_To_WaitDevAuthOffline() */
#define DBG_ETHSM_TRANSITION_ONHOLD_TO_WAITDEVAUTHOFFLINE_EXIT(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_ENTRY
/** \brief Entry point of function EthSM_Transition_WaitDevAuthOffline_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_ENTRY(a)
#endif

#ifndef DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_EXIT
/** \brief Exit point of function EthSM_Transition_WaitDevAuthOffline_To_WaitLinkDown() */
#define DBG_ETHSM_TRANSITION_WAITDEVAUTHOFFLINE_TO_WAITLINKDOWN_EXIT(a)
#endif

#ifndef DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_ENTRY
/** \brief Entry point of function EthSM_LookUpNetworkIdxFromNetworkHandle() */
#define DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_ENTRY(a)
#endif

#ifndef DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_EXIT
/** \brief Exit point of function EthSM_LookUpNetworkIdxFromNetworkHandle() */
#define DBG_ETHSM_LOOKUPNETWORKIDXFROMNETWORKHANDLE_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_ENTRY
/** \brief Entry point of function EthSM_LookUpNetworkIdxFromCtrlIdx() */
#define DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_ENTRY(a)
#endif

#ifndef DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_EXIT
/** \brief Exit point of function EthSM_LookUpNetworkIdxFromCtrlIdx() */
#define DBG_ETHSM_LOOKUPNETWORKIDXFROMCTRLIDX_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_HANDLETCPIPMODEINDICATION_ENTRY
/** \brief Entry point of function EthSM_HandleTcpIpModeIndication() */
#define DBG_ETHSM_HANDLETCPIPMODEINDICATION_ENTRY(a,d,c)
#endif

#ifndef DBG_ETHSM_HANDLETCPIPMODEINDICATION_EXIT
/** \brief Exit point of function EthSM_HandleTcpIpModeIndication() */
#define DBG_ETHSM_HANDLETCPIPMODEINDICATION_EXIT(a,d,c)
#endif

#ifndef DBG_ETHSM_HANDLECOMMMODEREQUEST_ENTRY
/** \brief Entry point of function EthSM_HandleComMModeRequest() */
#define DBG_ETHSM_HANDLECOMMMODEREQUEST_ENTRY(a,d,c)
#endif

#ifndef DBG_ETHSM_HANDLECOMMMODEREQUEST_EXIT
/** \brief Exit point of function EthSM_HandleComMModeRequest() */
#define DBG_ETHSM_HANDLECOMMMODEREQUEST_EXIT(a,b,c,d)
#endif

#ifndef DBG_ETHSM_CTRLMODEINDICATION_ENTRY
/** \brief Entry point of function EthSM_CtrlModeIndication() */
#define DBG_ETHSM_CTRLMODEINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_CTRLMODEINDICATION_EXIT
/** \brief Exit point of function EthSM_CtrlModeIndication() */
#define DBG_ETHSM_CTRLMODEINDICATION_EXIT(a,b)
#endif

#ifndef DBG_ETHSM_DEVAUTHMODEINDICATION_ENTRY
/** \brief Entry point of function EthSM_DevAuthNoComModeIndication() */
#define DBG_ETHSM_DEVAUTHMODEINDICATION_ENTRY(a)
#endif

#ifndef DBG_ETHSM_DEVAUTHMODEINDICATION_EXIT
/** \brief Exit point of function EthSM_DevAuthNoComModeIndication() */
#define DBG_ETHSM_DEVAUTHMODEINDICATION_EXIT(a)
#endif

#ifndef DBG_ETHSM_HANDLECTRLMODEINDICATION_ENTRY
/** \brief Entry point of function EthSM_HandleCtrlModeStateChg() */
#define DBG_ETHSM_HANDLECTRLMODEINDICATION_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHSM_HANDLECTRLMODEINDICATION_EXIT
/** \brief Entry point of function EthSM_HandleCtrlModeStateChg() */
#define DBG_ETHSM_HANDLECTRLMODEINDICATION_EXIT(a,b,c)
#endif

#ifndef DBG_ETHSM_HANDLETRCVLINKSTATECHG_ENTRY
/** \brief Entry point of function EthSM_HandleTrcvIndication() */
#define DBG_ETHSM_HANDLETRCVLINKSTATECHG_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHSM_HANDLETRCVLINKSTATECHG_EXIT
/** \brief Exit point of function EthSM_HandleTrcvIndication() */
#define DBG_ETHSM_HANDLETRCVLINKSTATECHG_EXIT(a,b,c)
#endif

#ifndef DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_ENTRY
/** \brief Entry point of function EthSM_HandleDevAuthNoComInd() */
#define DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_ENTRY(a,b)
#endif

#ifndef DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_EXIT
/** \brief Exit point of function EthSM_HandleDevAuthNoComInd() */
#define DBG_ETHSM_HANDLEDEVAUTHNOCOMIND_EXIT(a,b)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHSM_TRACE_H */
/*==================[end of file]===========================================*/
