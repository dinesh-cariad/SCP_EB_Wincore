/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef MIRROR_TRACE_H
#define MIRROR_TRACE_H

/*==================[macros]================================================*/
/* ====== MIRROR public APIs ====== */

#ifndef DBG_MIRROR_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Mirror_GetVersionInfo() */
#define DBG_MIRROR_GETVERSIONINFO_ENTRY(VersionInfo)
#endif /* #ifndef DBG_MIRROR_GETVERSIONINFO_ENTRY */

#ifndef DBG_MIRROR_GETVERSIONINFO_EXIT
/** \brief Exit point of function Mirror_GetVersionInfo() */
#define DBG_MIRROR_GETVERSIONINFO_EXIT(VersionInfo)
#endif /* #ifndef DBG_MIRROR_GETVERSIONINFO_EXIT */

#ifndef DBG_MIRROR_INIT_ENTRY
/** \brief Entry point of function Mirror_Init() */
#define DBG_MIRROR_INIT_ENTRY(config)
#endif /* #ifndef DBG_MIRROR_INIT_ENTRY */

#ifndef DBG_MIRROR_INIT_EXIT
/** \brief Exit point of function Mirror_Init() */
#define DBG_MIRROR_INIT_EXIT(config)
#endif /* #ifndef DBG_MIRROR_INIT_EXIT */

#ifndef DBG_MIRROR_DEINIT_ENTRY
/** \brief Entry point of function Mirror_DeInit() */
#define DBG_MIRROR_DEINIT_ENTRY()
#endif /* #ifndef DBG_MIRROR_DEINIT_ENTRY */

#ifndef DBG_MIRROR_DEINIT_EXIT
/** \brief Exit point of function Mirror_DeInit() */
#define DBG_MIRROR_DEINIT_EXIT()
#endif /* #ifndef DBG_MIRROR_DEINIT_EXIT */

#ifndef DBG_MIRROR_STARTALLSOURCENETWORKS_ENTRY
/** \brief Exit point of function Mirror_StartAllSourceNetworks() */
#define DBG_MIRROR_STARTALLSOURCENETWORKS_ENTRY()
#endif /* #ifndef DBG_MIRROR_STARTALLSOURCENETWORKS_ENTRY */

#ifndef DBG_MIRROR_STARTALLSOURCENETWORKS_EXIT
/** \brief Exit point of function Mirror_StartAllSourceNetworks() */
#define DBG_MIRROR_STARTALLSOURCENETWORKS_EXIT()
#endif /* #ifndef DBG_MIRROR_STARTALLSOURCENETWORKS_EXIT */

#ifndef DBG_MIRROR_STOPALLSOURCENETWORKS_ENTRY
/** \brief Exit point of function Mirror_StopAllSourceNetworks() */
#define DBG_MIRROR_STOPALLSOURCENETWORKS_ENTRY()
#endif /* #ifndef DBG_MIRROR_STOPALLSOURCENETWORKS_ENTRY */

#ifndef DBG_MIRROR_STOPALLSOURCENETWORKS_EXIT
/** \brief Exit point of function Mirror_StopAllSourceNetworks() */
#define DBG_MIRROR_STOPALLSOURCENETWORKS_EXIT()
#endif /* #ifndef DBG_MIRROR_STOPALLSOURCENETWORKS_EXIT */

#ifndef DBG_MIRROR_STARTSOURCENETWORK_ENTRY
/** \brief Exit point of function Mirror_StartSourceNetwork() */
#define DBG_MIRROR_STARTSOURCENETWORK_ENTRY(network)
#endif /* #ifndef DBG_MIRROR_STARTSOURCENETWORK_ENTRY */

#ifndef DBG_MIRROR_STARTSOURCENETWORK_EXIT
/** \brief Exit point of function Mirror_StartSourceNetwork() */
#define DBG_MIRROR_STARTSOURCENETWORK_EXIT(network)
#endif /* #ifndef DBG_MIRROR_STARTSOURCENETWORK_EXIT */

#ifndef DBG_MIRROR_STOPSOURCENETWORK_ENTRY
/** \brief Exit point of function Mirror_StopSourceNetwork() */
#define DBG_MIRROR_STOPSOURCENETWORK_ENTRY(network)
#endif /* #ifndef DBG_MIRROR_STOPSOURCENETWORK_ENTRY */

#ifndef DBG_MIRROR_STOPSOURCENETWORK_EXIT
/** \brief Exit point of function Mirror_StopSourceNetwork() */
#define DBG_MIRROR_STOPSOURCENETWORK_EXIT(network)
#endif /* #ifndef DBG_MIRROR_STOPSOURCENETWORK_EXIT */

#ifndef DBG_MIRROR_ISMIRRORACTIVE_ENTRY
/** \brief Exit point of function Mirror_IsMirrorActive() */
#define DBG_MIRROR_ISMIRRORACTIVE_ENTRY()
#endif /* #ifndef DBG_MIRROR_ISMIRRORACTIVE_ENTRY */

#ifndef DBG_MIRROR_ISMIRRORACTIVE_EXIT
/** \brief Exit point of function Mirror_IsMirrorActive() */
#define DBG_MIRROR_ISMIRRORACTIVE_EXIT()
#endif /* #ifndef DBG_MIRROR_ISMIRRORACTIVE_EXIT */

#ifndef DBG_MIRROR_OFFLINE_ENTRY
/** \brief Exit point of function Mirror_Offline() */
#define DBG_MIRROR_OFFLINE_ENTRY()
#endif /* #ifndef DBG_MIRROR_OFFLINE_ENTRY */

#ifndef DBG_MIRROR_OFFLINE_EXIT
/** \brief Exit point of function Mirror_Offline() */
#define DBG_MIRROR_OFFLINE_EXIT()
#endif /* #ifndef DBG_MIRROR_OFFLINE_EXIT */

#ifndef DBG_MIRROR_ISSOURCENETWORKSTARTED_ENTRY
/** \brief Exit point of function Mirror_IsSourceNetworkStarted() */
#define DBG_MIRROR_ISSOURCENETWORKSTARTED_ENTRY()
#endif /* #ifndef DBG_MIRROR_ISSOURCENETWORKSTARTED_ENTRY */

#ifndef DBG_MIRROR_ISSOURCENETWORKSTARTED_EXIT
/** \brief Exit point of function Mirror_IsSourceNetworkStarted() */
#define DBG_MIRROR_ISSOURCENETWORKSTARTED_EXIT()
#endif /* #ifndef DBG_MIRROR_ISSOURCENETWORKSTARTED_EXIT */

#ifndef DBG_MIRROR_GETDESTNETWORK_ENTRY
/** \brief Exit point of function Mirror_GetDestNetwork() */
#define DBG_MIRROR_GETDESTNETWORK_ENTRY()
#endif /* #ifndef DBG_MIRROR_GETDESTNETWORK_ENTRY */

#ifndef DBG_MIRROR_GETDESTNETWORK_EXIT
/** \brief Exit point of function Mirror_GetDestNetwork() */
#define DBG_MIRROR_GETDESTNETWORK_EXIT()
#endif /* #ifndef DBG_MIRROR_GETDESTNETWORK_EXIT */

#ifndef DBG_MIRROR_GETNETWORKTYPE_ENTRY
/** \brief Exit point of function Mirror_GetNetworkType() */
#define DBG_MIRROR_GETNETWORKTYPE_ENTRY(network)
#endif /* #ifndef DBG_MIRROR_GETNETWORKTYPE_ENTRY */

#ifndef DBG_MIRROR_GETNETWORKTYPE_EXIT
/** \brief Exit point of function Mirror_GetNetworkType() */
#define DBG_MIRROR_GETNETWORKTYPE_EXIT(network)
#endif /* #ifndef DBG_MIRROR_GETNETWORKTYPE_EXIT */

#ifndef DBG_MIRROR_GETNETWORKID_ENTRY
/** \brief Exit point of function Mirror_GetNetworkId() */
#define DBG_MIRROR_GETNETWORKID_ENTRY(network)
#endif /* #ifndef DBG_MIRROR_GETNETWORKID_ENTRY */

#ifndef DBG_MIRROR_GETNETWORKID_EXIT
/** \brief Exit point of function Mirror_GetNetworkId() */
#define DBG_MIRROR_GETNETWORKID_EXIT(network)
#endif /* #ifndef DBG_MIRROR_GETNETWORKID_EXIT */

#ifndef DBG_MIRROR_GETNETWORKHANDLE_ENTRY
/** \brief Exit point of function Mirror_GetNetworkHandle() */
#define DBG_MIRROR_GETNETWORKHANDLE_ENTRY(networkType, networkId)
#endif /* #ifndef DBG_MIRROR_GETNETWORKHANDLE_ENTRY */

#ifndef DBG_MIRROR_GETNETWORKHANDLE_EXIT
/** \brief Exit point of function Mirror_GetNetworkHandle() */
#define DBG_MIRROR_GETNETWORKHANDLE_EXIT(networkType, networkId)
#endif /* #ifndef DBG_MIRROR_GETNETWORKHANDLE_EXIT */

#ifndef DBG_MIRROR_MAINFUNCTION_ENTRY
/** \brief Exit point of function Mirror_MainFunction() */
#define DBG_MIRROR_MAINFUNCTION_ENTRY()
#endif /* #ifndef DBG_MIRROR_MAINFUNCTION_ENTRY */

#ifndef DBG_MIRROR_MAINFUNCTION_EXIT
/** \brief Exit point of function Mirror_MainFunction() */
#define DBG_MIRROR_MAINFUNCTION_EXIT()
#endif /* #ifndef DBG_MIRROR_MAINFUNCTION_EXIT */

#ifndef DBG_MIRROR_REPORTCANFRAME_ENTRY
/** \brief Exit point of function Mirror_ReportCanFrame() */
#define DBG_MIRROR_REPORTCANFRAME_ENTRY(controllerId, canId, length, payload)
#endif /* #ifndef DBG_MIRROR_REPORTCANFRAME_ENTRY */

#ifndef DBG_MIRROR_REPORTCANFRAME_EXIT
/** \brief Exit point of function Mirror_ReportCanFrame() */
#define DBG_MIRROR_REPORTCANFRAME_EXIT(controllerId, canId, length, payload)
#endif /* #ifndef DBG_MIRROR_REPORTCANFRAME_EXIT */

#ifndef DBG_MIRROR_REPORTLINFRAME_ENTRY
/** \brief Exit point of function Mirror_ReportLinFrame() */
#define DBG_MIRROR_REPORTLINFRAME_ENTRY(network, pid, pdu, status)
#endif /* #ifndef DBG_MIRROR_REPORTLINFRAME_ENTRY */

#ifndef DBG_MIRROR_REPORTLINFRAME_EXIT
/** \brief Exit point of function Mirror_ReportLinFrame() */
#define DBG_MIRROR_REPORTLINFRAME_EXIT(network, pid, pdu, status)
#endif /* #ifndef DBG_MIRROR_REPORTLINFRAME_EXIT */

#ifndef DBG_MIRROR_REPORTFLEXRAYFRAME_ENTRY
/** \brief Exit point of function Mirror_ReportFlexRayFrame() */
#define DBG_MIRROR_REPORTFLEXRAYFRAME_ENTRY(controllerId, slotId, cycle, frChannel, frame, txConflict)
#endif /* #ifndef DBG_MIRROR_REPORTFLEXRAYFRAME_ENTRY */

#ifndef DBG_MIRROR_REPORTFLEXRAYFRAME_EXIT
/** \brief Exit point of function Mirror_ReportFlexRayFrame() */
#define DBG_MIRROR_REPORTFLEXRAYFRAME_EXIT(controllerId, slotId, cycle, frChannel, frame, txConflict)
#endif /* #ifndef DBG_MIRROR_REPORTFLEXRAYFRAME_EXIT */

#ifndef DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_ENTRY
/** \brief Exit point of function Mirror_ReportFlexRayChannelStatus() */
#define DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_ENTRY(clusterId, channelAStatus, channelBStatus)
#endif /* #ifndef DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_ENTRY */

#ifndef DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_EXIT
/** \brief Exit point of function Mirror_ReportFlexRayChannelStatus() */
#define DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_EXIT(clusterId, channelAStatus, channelBStatus)
#endif /* #ifndef DBG_MIRROR_REPORTFLEXRAYCHANNELSTATUS_EXIT */

#ifndef DBG_MIRROR_TXCONFIRMATION_ENTRY
/** \brief Exit point of function Mirror_TxConfirmation() */
#define DBG_MIRROR_TXCONFIRMATION_ENTRY(TxPduId)
#endif /* #ifndef DBG_MIRROR_TXCONFIRMATION_ENTRY */

#ifndef DBG_MIRROR_TXCONFIRMATION_EXIT
/** \brief Exit point of function Mirror_TxConfirmation() */
#define DBG_MIRROR_TXCONFIRMATION_EXIT(TxPduId)
#endif /* #ifndef DBG_MIRROR_TXCONFIRMATION_EXIT */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef MIRROR_TRACE_H */
