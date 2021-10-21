/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef SOAD_TRACE_INTERNAL_H
#define SOAD_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_SOAD_IFTRANSMITPDUROUTEDEST_ENTRY
/** \brief Entry point of function SoAd_IfTransmitPduRouteDest() */
#define DBG_SOAD_IFTRANSMITPDUROUTEDEST_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_IFTRANSMITPDUROUTEDEST_EXIT
/** \brief Exit point of function SoAd_IfTransmitPduRouteDest() */
#define DBG_SOAD_IFTRANSMITPDUROUTEDEST_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_IFTRANSMITPDUROUTEDESTUDP_ENTRY
/** \brief Entry point of function SoAd_IfTransmitPduRouteDestUdp() */
#define DBG_SOAD_IFTRANSMITPDUROUTEDESTUDP_ENTRY(a,b,c,e,f,g,h,i)
#endif

#ifndef DBG_SOAD_IFTRANSMITPDUROUTEDESTUDP_EXIT
/** \brief Exit point of function SoAd_IfTransmitPduRouteDestUdp() */
#define DBG_SOAD_IFTRANSMITPDUROUTEDESTUDP_EXIT(a,b,c,d,f,g,h,i,j)
#endif

#ifndef DBG_SOAD_RXUDP_ENTRY
/** \brief Entry point of function SoAd_RxUdp() */
#define DBG_SOAD_RXUDP_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RXUDP_EXIT
/** \brief Exit point of function SoAd_RxUdp() */
#define DBG_SOAD_RXUDP_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RXPROCESSPDUHEADERS_ENTRY
/** \brief Entry point of function SoAd_RxProcessPduHeaders() */
#define DBG_SOAD_RXPROCESSPDUHEADERS_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_RXPROCESSPDUHEADERS_EXIT
/** \brief Exit point of function SoAd_RxProcessPduHeaders() */
#define DBG_SOAD_RXPROCESSPDUHEADERS_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_RXPROCESSTCPPDUSEGMENT_ENTRY
/** \brief Entry point of function SoAd_RxProcessTcpPduSegment() */
#define DBG_SOAD_RXPROCESSTCPPDUSEGMENT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_RXPROCESSTCPPDUSEGMENT_EXIT
/** \brief Exit point of function SoAd_RxProcessTcpPduSegment() */
#define DBG_SOAD_RXPROCESSTCPPDUSEGMENT_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_ULIFRXINDICATION_ENTRY
/** \brief Entry point of function SoAd_ULIfRxIndication() */
#define DBG_SOAD_ULIFRXINDICATION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_ULIFRXINDICATION_EXIT
/** \brief Exit point of function SoAd_ULIfRxIndication() */
#define DBG_SOAD_ULIFRXINDICATION_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_SOCKETTOSOCONID_ENTRY
/** \brief Entry point of function SoAd_SocketToSoConId() */
#define DBG_SOAD_SOCKETTOSOCONID_ENTRY(a)
#endif

#ifndef DBG_SOAD_SOCKETTOSOCONID_EXIT
/** \brief Exit point of function SoAd_SocketToSoConId() */
#define DBG_SOAD_SOCKETTOSOCONID_EXIT(a,b)
#endif

#ifndef DBG_SOAD_LISTENSOCKETTOMAPID_ENTRY
/** \brief Entry point of function SoAd_ListenSocketToMapId() */
#define DBG_SOAD_LISTENSOCKETTOMAPID_ENTRY(a)
#endif

#ifndef DBG_SOAD_LISTENSOCKETTOMAPID_EXIT
/** \brief Exit point of function SoAd_ListenSocketToMapId() */
#define DBG_SOAD_LISTENSOCKETTOMAPID_EXIT(a,b)
#endif

#ifndef DBG_SOAD_LISTENSOCKETTOSOCONID_ENTRY
/** \brief Entry point of function SoAd_ListenSocketToSoConId() */
#define DBG_SOAD_LISTENSOCKETTOSOCONID_ENTRY(a)
#endif

#ifndef DBG_SOAD_LISTENSOCKETTOSOCONID_EXIT
/** \brief Exit point of function SoAd_ListenSocketToSoConId() */
#define DBG_SOAD_LISTENSOCKETTOSOCONID_EXIT(a,b)
#endif

#ifndef DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_ENTRY
/** \brief Entry point of function SoAd_GetActivePduRouteDestVirtualId() */
#define DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_ENTRY(a)
#endif

#ifndef DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_EXIT
/** \brief Exit point of function SoAd_GetActivePduRouteDestVirtualId() */
#define DBG_SOAD_GETACTIVEPDUROUTEVIRTUALDESTID_EXIT(a,b)
#endif

#ifndef DBG_SOAD_RESETWILDCARDSOCON_ENTRY
/** \brief Entry point of function SoAd_ResetWildCardSoCon() */
#define DBG_SOAD_RESETWILDCARDSOCON_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_RESETWILDCARDSOCON_EXIT
/** \brief Exit point of function SoAd_ResetWildCardSoCon() */
#define DBG_SOAD_RESETWILDCARDSOCON_EXIT(a,b)
#endif

#ifndef DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_ENTRY
/** \brief Entry point of function SoAd_ResetWildCardSoConInPduRoute() */
#define DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_ENTRY(a)
#endif

#ifndef DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_EXIT
/** \brief Exit point of function SoAd_ResetWildCardSoConInPduRoute() */
#define DBG_SOAD_RESETWILDCARDSOCONINPDUROUTE_EXIT(a)
#endif

#ifndef DBG_SOAD_ULIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function SoAd_ULIfTxConfirmation() */
#define DBG_SOAD_ULIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_SOAD_ULIFTXCONFIRMATION_EXIT
/** \brief Exit point of function SoAd_ULIfTxConfirmation() */
#define DBG_SOAD_ULIFTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_SOAD_ULIFTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_ULIfTriggerTransmit() */
#define DBG_SOAD_ULIFTRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_ULIFTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function SoAd_ULIfTriggerTransmit() */
#define DBG_SOAD_ULIFTRIGGERTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_ULTPTXCONFIRMATION_ENTRY
/** \brief Entry point of function SoAd_ULTpTxConfirmation() */
#define DBG_SOAD_ULTPTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_ULTPTXCONFIRMATION_EXIT
/** \brief Exit point of function SoAd_ULTpTxConfirmation() */
#define DBG_SOAD_ULTPTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INITDATASTRUCTURES_ENTRY
/** \brief Entry point of function SoAd_InitDataStructures() */
#define DBG_SOAD_INITDATASTRUCTURES_ENTRY()
#endif

#ifndef DBG_SOAD_INITDATASTRUCTURES_EXIT
/** \brief Exit point of function SoAd_InitDataStructures() */
#define DBG_SOAD_INITDATASTRUCTURES_EXIT()
#endif

#ifndef DBG_SOAD_INITSOCONTXDATASTRUCTURES_ENTRY
/** \brief Entry point of function SoAd_InitSoConTxDataStructures() */
#define DBG_SOAD_INITSOCONTXDATASTRUCTURES_ENTRY(a)
#endif

#ifndef DBG_SOAD_INITSOCONTXDATASTRUCTURES_EXIT
/** \brief Exit point of function SoAd_InitSoConTxDataStructures() */
#define DBG_SOAD_INITSOCONTXDATASTRUCTURES_EXIT(a)
#endif

#ifndef DBG_SOAD_REQUESTTCPIPSOCKET_ENTRY
/** \brief Entry point of function SoAd_RequestTcpIpSocket() */
#define DBG_SOAD_REQUESTTCPIPSOCKET_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_REQUESTTCPIPSOCKET_EXIT
/** \brief Exit point of function SoAd_RequestTcpIpSocket() */
#define DBG_SOAD_REQUESTTCPIPSOCKET_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_INT_04_SETUPSOCON_ENTRY
/** \brief Entry point of function SoAd_Int_04_SetupSoCon() */
#define DBG_SOAD_INT_04_SETUPSOCON_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_INT_04_SETUPSOCON_EXIT
/** \brief Exit point of function SoAd_Int_04_SetupSoCon() */
#define DBG_SOAD_INT_04_SETUPSOCON_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_INT_04_SETUPGROUPSOCON_ENTRY
/** \brief Entry point of function SoAd_Int_04_SetupGroupSoCon() */
#define DBG_SOAD_INT_04_SETUPGROUPSOCON_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_SETUPGROUPSOCON_EXIT
/** \brief Exit point of function SoAd_Int_04_SetupGroupSoCon() */
#define DBG_SOAD_INT_04_SETUPGROUPSOCON_EXIT(a,b)
#endif

#ifndef DBG_SOAD_ROUTINGGROUPACTION_ENTRY
/** \brief Entry point of function SoAd_RoutingGroupAction() */
#define DBG_SOAD_ROUTINGGROUPACTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_ROUTINGGROUPACTION_EXIT
/** \brief Exit point of function SoAd_routingGroupAction() */
#define DBG_SOAD_ROUTINGGROUPACTION_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_ISPDUROUTEDESTENABLED_ENTRY
/** \brief Entry point of function SoAd_IsPduRouteDestEnabled() */
#define DBG_SOAD_ISPDUROUTEDESTENABLED_ENTRY(a)
#endif

#ifndef DBG_SOAD_ISPDUROUTEDESTENABLED_EXIT
/** \brief Exit point of function SoAd_IsPduRouteDestEnabled() */
#define DBG_SOAD_ISPDUROUTEDESTENABLED_EXIT(a,b)
#endif

#ifndef DBG_SOAD_ISSOCKETROUTEENABLED_ENTRY
/** \brief Entry point of function SoAd_IsSocketRouteEnabled() */
#define DBG_SOAD_ISSOCKETROUTEENABLED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_ISSOCKETROUTEENABLED_EXIT
/** \brief Exit point of function SoAd_IsSocketRouteEnabled() */
#define DBG_SOAD_ISSOCKETROUTEENABLED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_STARTUPSOCON_ENTRY
/** \brief Entry point of function SoAd_StartUpSoCon() */
#define DBG_SOAD_STARTUPSOCON_ENTRY(a)
#endif

#ifndef DBG_SOAD_STARTUPSOCON_EXIT
/** \brief Exit point of function SoAd_StartUpSoCon() */
#define DBG_SOAD_STARTUPSOCON_EXIT(a)
#endif

#ifndef DBG_SOAD_INT_04_SHUTDOWNSOCKET_ENTRY
/** \brief Entry point of function SoAd_Int_04_ShutdownSocket() */
#define DBG_SOAD_INT_04_SHUTDOWNSOCKET_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_04_SHUTDOWNSOCKET_EXIT
/** \brief Exit point of function SoAd_Int_04_ShutdownSocket() */
#define DBG_SOAD_INT_04_SHUTDOWNSOCKET_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_02_CLEANUPSOCON_ENTRY
/** \brief Entry point of function SoAd_Int_02_CleanUpSoCon() */
#define DBG_SOAD_INT_02_CLEANUPSOCON_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_02_CLEANUPSOCON_EXIT
/** \brief Exit point of function SoAd_Int_02_CleanUpSoCon() */
#define DBG_SOAD_INT_02_CLEANUPSOCON_EXIT(a,b)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPRX_ENTRY
/** \brief Entry point of function SoAd_MainFunctionTpRx() */
#define DBG_SOAD_MAINFUNCTIONTPRX_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPRX_EXIT
/** \brief Exit point of function SoAd_MainFunctionTpRx() */
#define DBG_SOAD_MAINFUNCTIONTPRX_EXIT(a,b)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPTXTCP_ENTRY
/** \brief Entry point of function SoAd_MainFunctionTpTxTcp() */
#define DBG_SOAD_MAINFUNCTIONTPTXTCP_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPTXTCP_EXIT
/** \brief Exit point of function SoAd_MainFunctionTpTxTcp() */
#define DBG_SOAD_MAINFUNCTIONTPTXTCP_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPTXUDP_ENTRY
/** \brief Entry point of function SoAd_MainFunctionTpTxUdp() */
#define DBG_SOAD_MAINFUNCTIONTPTXUDP_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONTPTXUDP_EXIT
/** \brief Exit point of function SoAd_MainFunctionTpTxUdp() */
#define DBG_SOAD_MAINFUNCTIONTPTXUDP_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONIFTXUDP_ENTRY
/** \brief Entry point of function SoAd_MainFunctionIfTxUdp() */
#define DBG_SOAD_MAINFUNCTIONIFTXUDP_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONIFTXUDP_EXIT
/** \brief Exit point of function SoAd_MainFunctionIfTxUdp() */
#define DBG_SOAD_MAINFUNCTIONIFTXUDP_EXIT(a,b)
#endif

#ifndef DBG_SOAD_CLEANUPUDPTXBUFFER_ENTRY
/** \brief Entry point of function SoAd_CleanUpUdpTxBuffer() */
#define DBG_SOAD_CLEANUPUDPTXBUFFER_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_CLEANUPUDPTXBUFFER_EXIT
/** \brief Exit point of function SoAd_CleanUpUdpTxBuffer() */
#define DBG_SOAD_CLEANUPUDPTXBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONUDPTXCONF_ENTRY
/** \brief Entry point of function SoAd_MainFunctionUdpTxConf() */
#define DBG_SOAD_MAINFUNCTIONUDPTXCONF_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_MAINFUNCTIONUDPTXCONF_EXIT
/** \brief Exit point of function SoAd_MainFunctionUdpTxConf() */
#define DBG_SOAD_MAINFUNCTIONUDPTXCONF_EXIT(a,b)
#endif

#ifndef DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_ENTRY
/** \brief Entry point of function SoAd_HandleTriggeredRoutingGroups() */
#define DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_ENTRY()
#endif

#ifndef DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_EXIT
/** \brief Exit point of function SoAd_HandleTriggeredRoutingGroups() */
#define DBG_SOAD_HANDLETRIGGEREDROUTINGGROUPS_EXIT()
#endif

#ifndef DBG_SOAD_INT_02_ULSOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_02_ULSoConModeChg() */
#define DBG_SOAD_INT_02_ULSOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_02_ULSOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_02_ULSoConModeChg() */
#define DBG_SOAD_INT_02_ULSOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function SoAd_ULLocalIpAddrAssignmentChg() */
#define DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function SoAd_ULLocalIpAddrAssignmentChg() */
#define DBG_SOAD_ULLOCALIPADDRASSIGNMENTCHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_REMOTEADDRESSMATCH_ENTRY
/** \brief Entry point of function SoAd_RemoteAddressMatch() */
#define DBG_SOAD_REMOTEADDRESSMATCH_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_REMOTEADDRESSMATCH_EXIT
/** \brief Exit point of function SoAd_RemoteAddressMatch() */
#define DBG_SOAD_REMOTEADDRESSMATCH_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_MESSAGEACCEPTANCEFILTER_ENTRY
/** \brief Entry point of function SoAd_MessageAcceptanceFilter() */
#define DBG_SOAD_MESSAGEACCEPTANCEFILTER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_MESSAGEACCEPTANCEFILTER_EXIT
/** \brief Exit point of function SoAd_MessageAcceptanceFilter() */
#define DBG_SOAD_MESSAGEACCEPTANCEFILTER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_ULTPSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function SoAd_ULTpStartofReception() */
#define DBG_SOAD_ULTPSTARTOFRECEPTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_ULTPSTARTOFRECEPTION_EXIT
/** \brief Exit point of function SoAd_ULTpStartofReception() */
#define DBG_SOAD_ULTPSTARTOFRECEPTION_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_ULTPCOPYRXDATA_ENTRY
/** \brief Entry point of function SoAd_ULTpCopyRxData() */
#define DBG_SOAD_ULTPCOPYRXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_ULTPCOPYRXDATA_EXIT
/** \brief Exit point of function SoAd_ULTpCopyRxData() */
#define DBG_SOAD_ULTPCOPYRXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_ULTPRXINDICATION_ENTRY
/** \brief Entry point of function SoAd_ULTpRxIndication() */
#define DBG_SOAD_ULTPRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_ULTPRXINDICATION_EXIT
/** \brief Exit point of function SoAd_ULTpRxIndication() */
#define DBG_SOAD_ULTPRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_SOAD_ULTPCOPYTXDATA_ENTRY
/** \brief Entry point of function SoAd_ULTpCopyTxData() */
#define DBG_SOAD_ULTPCOPYTXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_ULTPCOPYTXDATA_EXIT
/** \brief Exit point of function SoAd_ULTpCopyTxData() */
#define DBG_SOAD_ULTPCOPYTXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RECEIVETPPDU_ENTRY
/** \brief Entry point of function SoAd_ReceiveTpPdu() */
#define DBG_SOAD_RECEIVETPPDU_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_SOAD_RECEIVETPPDU_EXIT
/** \brief Exit point of function SoAd_ReceiveTpPdu() */
#define DBG_SOAD_RECEIVETPPDU_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_SOAD_PROCESSTPPDU_HEADERMODE_ENTRY
/** \brief Entry point of function SoAd_ProcessTpPdu_HeaderMode() */
#define DBG_SOAD_PROCESSTPPDU_HEADERMODE_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_PROCESSTPPDU_HEADERMODE_EXIT
/** \brief Exit point of function SoAd_ProcessTpPdu_HeaderMode() */
#define DBG_SOAD_PROCESSTPPDU_HEADERMODE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_ENTRY
/** \brief Entry point of function SoAd_ProcessTpPdu_NonHeaderMode() */
#define DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_EXIT
/** \brief Exit point of function SoAd_ProcessTpPdu_NonHeaderMode() */
#define DBG_SOAD_PROCESSTPPDU_NONHEADERMODE_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_RECEIVETPPDUUDPNOBUF_ENTRY
/** \brief Entry point of function SoAd_ReceiveTpPduUdpNoBuf() */
#define DBG_SOAD_RECEIVETPPDUUDPNOBUF_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RECEIVETPPDUUDPNOBUF_EXIT
/** \brief Exit point of function SoAd_ReceiveTpPduUdpNoBuf() */
#define DBG_SOAD_RECEIVETPPDUUDPNOBUF_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RINGBUFFERSTORE_ENTRY
/** \brief Entry point of function SoAd_RingBufferStore() */
#define DBG_SOAD_RINGBUFFERSTORE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_SOAD_RINGBUFFERSTORE_EXIT
/** \brief Exit point of function SoAd_RingBufferStore() */
#define DBG_SOAD_RINGBUFFERSTORE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RINGBUFFERDELIVERTP_ENTRY
/** \brief Entry point of function SoAd_RingBufferDeliverTp() */
#define DBG_SOAD_RINGBUFFERDELIVERTP_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_RINGBUFFERDELIVERTP_EXIT
/** \brief Exit point of function SoAd_RingBufferDeliverTp() */
#define DBG_SOAD_RINGBUFFERDELIVERTP_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_RINGBUFFERDISCARD_ENTRY
/** \brief Entry point of function SoAd_RingBufferDiscard() */
#define DBG_SOAD_RINGBUFFERDISCARD_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_RINGBUFFERDISCARD_EXIT
/** \brief Exit point of function SoAd_RingBufferDiscard() */
#define DBG_SOAD_RINGBUFFERDISCARD_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_RINGBUFFERLOADACCESS_ENTRY
/** \brief Entry point of function SoAd_RingBufferLoadAccess() */
#define DBG_SOAD_RINGBUFFERLOADACCESS_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_RINGBUFFERLOADACCESS_EXIT
/** \brief Exit point of function SoAd_RingBufferLoadAccess() */
#define DBG_SOAD_RINGBUFFERLOADACCESS_EXIT(a,b,c,d)
#endif

#ifndef DBG_SOAD_RINGBUFFERSAVEACCESS_ENTRY
/** \brief Entry point of function SoAd_RingBufferSaveAccess() */
#define DBG_SOAD_RINGBUFFERSAVEACCESS_ENTRY(a,b,c)
#endif

#ifndef DBG_SOAD_RINGBUFFERSAVEACCESS_EXIT
/** \brief Exit point of function SoAd_RingBufferSaveAccess() */
#define DBG_SOAD_RINGBUFFERSAVEACCESS_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_EXTRACTPDUHEADER_ENTRY
/** \brief Entry point of function SoAd_ExtractPduHeader() */
#define DBG_SOAD_EXTRACTPDUHEADER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_EXTRACTPDUHEADER_EXIT
/** \brief Exit point of function SoAd_ExtractPduHeader() */
#define DBG_SOAD_EXTRACTPDUHEADER_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_SOAD_WRITEPDUHEADER_ENTRY
/** \brief Entry point of function SoAd_WritePduHeader() */
#define DBG_SOAD_WRITEPDUHEADER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_WRITEPDUHEADER_EXIT
/** \brief Exit point of function SoAd_WritePduHeader() */
#define DBG_SOAD_WRITEPDUHEADER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_SOAD_RESETREMOTEADDRESS_ENTRY
/** \brief Entry point of function SoAd_ResetRemoteAddress() */
#define DBG_SOAD_RESETREMOTEADDRESS_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_RESETREMOTEADDRESS_EXIT
/** \brief Exit point of function SoAd_ResetRemoteAddress() */
#define DBG_SOAD_RESETREMOTEADDRESS_EXIT(a,b)
#endif

#ifndef DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_ENTRY
/** \brief Entry point of function SoAd_PduFanOutPduTxConfPending() */
#define DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_ENTRY(a)
#endif

#ifndef DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_EXIT
/** \brief Exit point of function SoAd_PduFanOutPduTxConfPending() */
#define DBG_SOAD_PDUFANOUTPDUTXCONFPENDING_EXIT(a,b)
#endif

#ifndef DBG_SOAD_PREPARETRANSMIT_ENTRY
/** \brief Entry point of function SoAd_PrepareTransmit() */
#define DBG_SOAD_PREPARETRANSMIT_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_SOAD_PREPARETRANSMIT_EXIT
/** \brief Exit point of function SoAd_PrepareTransmit() */
#define DBG_SOAD_PREPARETRANSMIT_EXIT(a,b,c,d,e,f,g)
#endif

/* === === [UDP state machine APIs] === === */

#ifndef DBG_SOAD_INT_01_UDPSM_CONNECTING_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_Connecting() */
#define DBG_SOAD_INT_01_UDPSM_CONNECTING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_CONNECTING_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_Connecting() */
#define DBG_SOAD_INT_01_UDPSM_CONNECTING_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_CLOSED_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_Closed() */
#define DBG_SOAD_INT_01_UDPSM_CLOSED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_CLOSED_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_Closed() */
#define DBG_SOAD_INT_01_UDPSM_CLOSED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_DISCONNECTED_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_Disconnected() */
#define DBG_SOAD_INT_01_UDPSM_DISCONNECTED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_DISCONNECTED_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_Disconnected() */
#define DBG_SOAD_INT_01_UDPSM_DISCONNECTED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_ERROR_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_Error() */
#define DBG_SOAD_INT_01_UDPSM_ERROR_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_ERROR_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_Error() */
#define DBG_SOAD_INT_01_UDPSM_ERROR_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_UPCLOSING_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_UpClosing() */
#define DBG_SOAD_INT_01_UDPSM_UPCLOSING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_UPCLOSING_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_UpClosing() */
#define DBG_SOAD_INT_01_UDPSM_UPCLOSING_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_CONNECTED_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_Connected() */
#define DBG_SOAD_INT_01_UDPSM_CONNECTED_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_CONNECTED_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_Connected() */
#define DBG_SOAD_INT_01_UDPSM_CONNECTED_EXIT(a,b,c)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_CLOSED_Entry_Cleanup() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_CLOSED_Entry_Cleanup() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CLOSED_ENTRY_CLEANUP_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_CLOSING_Entry_Closing() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_CLOSING_Entry_Closing() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CLOSING_ENTRY_CLOSING_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_OFFLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_OFFLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_OFFLINE_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_ONLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_ONLINE_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRYONLINE_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_ONLINE_Entry_Online() */
#define DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRY_ONLINE_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRYONLINE_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_ONLINE_Entry_Online() */
#define DBG_SOAD_01_UDPSM_STATE_ONLINE_ENTRY_ONLINE_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_CONNECTING_Entry_Connecting() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_CONNECTING_Entry_Connecting() */
#define DBG_SOAD_INT_01_UDPSM_STATE_CONNECTING_ENTRY_CONNECTING_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_RESTART_Entry_Restart() */
#define DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_RESTART_Entry_Restart() */
#define DBG_SOAD_INT_01_UDPSM_STATE_RESTART_ENTRY_RESTART_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_SHUTDOWN_Entry_ShutDown() */
#define DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_SHUTDOWN_Entry_ShutDown() */
#define DBG_SOAD_INT_01_UDPSM_STATE_SHUTDOWN_ENTRY_SHUTDOWN_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_RECONNECT_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_RECONNECT_Entry_SoConModeChg() */
#define DBG_SOAD_INT_01_UDPSM_STATE_RECONNECT_ENTRY_SOCONMODECHG_EXIT(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY
/** \brief Entry point of function SoAd_Int_01_UdpSM_State_ONLINE_Exit_CloseRxTx() */
#define DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_ENTRY(a,b)
#endif

#ifndef DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT
/** \brief Exit point of function SoAd_Int_01_UdpSM_State_ONLINE_Exit_CloseRxTx() */
#define DBG_SOAD_INT_01_UDPSM_STATE_ONLINE_EXIT_CLOSERXTX_EXIT(a,b)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOAD_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
