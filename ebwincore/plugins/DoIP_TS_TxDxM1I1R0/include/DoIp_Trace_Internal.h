/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_TRACE_INTERNAL_H
#define DOIP_TRACE_INTERNAL_H

/* give test files a chance to disable the internal trace macros completely */
#ifndef DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING
/*==================[inclusions]============================================*/

#include <Std_Types.h> /* AUTOSAR standard types */


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==================[macros]================================================*/
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_ENTRY
/** \brief Entry point of function DoIP_ProcessAuthConfPendingRoutingActivations()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_EXIT
/** \brief Exit point of function DoIP_ProcessAuthConfPendingRoutingActivations()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PROCESSAUTHCONFPENDINGROUTINGACTIVATIONS_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PROCESSALIVECHECK_ENTRY
/** \brief Entry point of function DoIP_ProcessAliveCheck()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PROCESSALIVECHECK_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PROCESSALIVECHECK_EXIT
/** \brief Exit point of function DoIP_ProcessAliveCheck()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PROCESSALIVECHECK_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CONTINUEROUTINGACTIVATION_ENTRY
/** \brief Entry point of function DoIP_ContinueRoutingActivation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CONTINUEROUTINGACTIVATION_ENTRY(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CONTINUEROUTINGACTIVATION_EXIT
/** \brief Exit point of function DoIP_ContinueRoutingActivation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CONTINUEROUTINGACTIVATION_EXIT(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEALIVECHECKCONRESET_ENTRY
/** \brief Entry point of function DoIP_HandleAliveCheckConReset()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLEALIVECHECKCONRESET_ENTRY(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEALIVECHECKCONRESET_EXIT
/** \brief Exit point of function DoIP_HandleAliveCheckConReset()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLEALIVECHECKCONRESET_EXIT(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_ENTRY
/** \brief Entry point of function DoIP_HandleTxDiagMsgConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_ENTRY(TcpConIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_EXIT
/** \brief Exit point of function DoIP_HandleTxDiagMsgConfirmation()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLETXDIAGMSGCONFIRMATION_EXIT(Retval,TcpConIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PREPARETCPCON_ENTRY
/** \brief Entry point of function DoIP_PrepareTcpCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PREPARETCPCON_ENTRY(TcpConIdx,DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PREPARETCPCON_EXIT
/** \brief Exit point of function DoIP_PrepareTcpCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PREPARETCPCON_EXIT(TcpConIdx,DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFY_VIN_ENTRY
/** \brief Entry point of function DoIP_Verify_VIN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_VERIFY_VIN_ENTRY(ReceivedVinPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFY_VIN_EXIT
/** \brief Exit point of function DoIP_Verify_VIN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_VERIFY_VIN_EXIT(Retval,ReceivedVinPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_VIN_ENTRY
/** \brief Entry point of function DoIP_Fill_VIN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_VIN_ENTRY(VinPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_VIN_EXIT
/** \brief Exit point of function DoIP_Fill_VIN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FILL_VIN_EXIT(Retval,VinPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETAVAILPAYLOADLENGTH_ENTRY
/** \brief Entry point of function DoIP_GetAvailPayloadLength()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETAVAILPAYLOADLENGTH_ENTRY(TcpConIdx,FillDiagMsgPldFp,AvailablePldDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETAVAILPAYLOADLENGTH_EXIT
/** \brief Exit point of function DoIP_GetAvailPayloadLength()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_GETAVAILPAYLOADLENGTH_EXIT(Retval,TcpConIdx,FillDiagMsgPldFp,AvailablePldDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLMSG_ENTRY
/** \brief Entry point of function DoIP_FillMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLMSG_ENTRY(TcpConIdx,HdrLength,PduInfoPtr,AvailableDataPtr,FillMsgHdrFp,FillDiagMsgPldFp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLMSG_EXIT
/** \brief Exit point of function DoIP_FillMsg()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FILLMSG_EXIT(Retval,TcpConIdx,HdrLength,PduInfoPtr,AvailableDataPtr,FillMsgHdrFp,FillDiagMsgPldFp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLGENHEADERNACK_ENTRY
/** \brief Entry point of function DoIP_FillGenHeaderNack()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLGENHEADERNACK_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLGENHEADERNACK_EXIT
/** \brief Exit point of function DoIP_FillGenHeaderNack()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLGENHEADERNACK_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLALIVECHECKREQ_ENTRY
/** \brief Entry point of function DoIP_FillAliveCheckReq()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLALIVECHECKREQ_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLALIVECHECKREQ_EXIT
/** \brief Exit point of function DoIP_FillAliveCheckReq()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLALIVECHECKREQ_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATION_ENTRY
/** \brief Entry point of function DoIP_RoutingActivation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ROUTINGACTIVATION_ENTRY(TcpConIdx,PduPtr,PayloadLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATION_EXIT
/** \brief Exit point of function DoIP_RoutingActivation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ROUTINGACTIVATION_EXIT(TcpConIdx,PduPtr,PayloadLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_ENTRY
/** \brief Entry point of function DoIP_RoutingActivationStandardChannels()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_ENTRY(TcpConIdx,PayloadLength,PduPtr,TxState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_EXIT
/** \brief Exit point of function DoIP_RoutingActivationStandardChannels()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_ROUTINGACTIVATIONSTANDARDCHANNELS_EXIT(Retval,TcpConIdx,PayloadLength,PduPtr,TxState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATIONCHANNEL_ENTRY
/** \brief Entry point of function DoIP_RoutingActivationChannel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ROUTINGACTIVATIONCHANNEL_ENTRY(TcpConIdx,DoIPPduRPduId,PayloadLength,RoutingActIdx,ChannelProcessedPtr,PduPtr,TxStatePtr,RespCodePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ROUTINGACTIVATIONCHANNEL_EXIT
/** \brief Exit point of function DoIP_RoutingActivationChannel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ROUTINGACTIVATIONCHANNEL_EXIT(TcpConIdx,DoIPPduRPduId,PayloadLength,RoutingActIdx,ChannelProcessedPtr,PduPtr,TxStatePtr,RespCodePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_ENTRY
/** \brief Entry point of function DoIP_VerifyAuthenticationConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_ENTRY(TcpConIdx,ChannelPtr,TxStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_EXIT
/** \brief Exit point of function DoIP_VerifyAuthenticationConfirmation()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_VERIFYAUTHENTICATIONCONFIRMATION_EXIT(Retval,TcpConIdx,ChannelPtr,TxStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYAUTHENTICATION_ENTRY
/** \brief Entry point of function DoIP_VerifyAuthentication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_VERIFYAUTHENTICATION_ENTRY(TcpConIdx,ChannelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYAUTHENTICATION_EXIT
/** \brief Exit point of function DoIP_VerifyAuthentication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_VERIFYAUTHENTICATION_EXIT(Retval,TcpConIdx,ChannelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYCONFIRMATION_ENTRY
/** \brief Entry point of function DoIP_VerifyConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_VERIFYCONFIRMATION_ENTRY(TcpConIdx,ChannelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_VERIFYCONFIRMATION_EXIT
/** \brief Exit point of function DoIP_VerifyConfirmation()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_VERIFYCONFIRMATION_EXIT(Retval,TcpConIdx,ChannelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_ENTRY
/** \brief Entry point of function DoIP_CallRoutingActivationCallbacks()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_ENTRY(TcpConPtr, ActivationType, OemSpecificPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_EXIT
/** \brief Exit point of function DoIP_CallRoutingActivationCallbacks()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CALLROUTINGACTIVATIONCALLBACKS_EXIT(TcpConPtr, ActivationType, OemSpecificPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLESEGMENTEDRX_ENTRY
/** \brief Entry point of function DoIP_HandleSegmentedRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLESEGMENTEDRX_ENTRY(TcpConIdx,PduInfoPtr,RxMsgPtr,MsgLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLESEGMENTEDRX_EXIT
/** \brief Exit point of function DoIP_HandleSegmentedRx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLESEGMENTEDRX_EXIT(Retval,TcpConIdx,PduInfoPtr,RxMsgPtr,MsgLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEROUTINGACTIVATION_ENTRY
/** \brief Entry point of function DoIP_HandleRoutingActivation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLEROUTINGACTIVATION_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEROUTINGACTIVATION_EXIT
/** \brief Exit point of function DoIP_HandleRoutingActivation()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLEROUTINGACTIVATION_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SENDALIVECHECKREQUEST_ENTRY
/** \brief Entry point of function DoIP_SendAliveCheckRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SENDALIVECHECKREQUEST_ENTRY(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SENDALIVECHECKREQUEST_EXIT
/** \brief Exit point of function DoIP_SendAliveCheckRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SENDALIVECHECKREQUEST_EXIT(Retval,TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLERXSTARTREADY_ENTRY
/** \brief Entry point of function DoIP_HandleRxStartReady()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLERXSTARTREADY_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLERXSTARTREADY_EXIT
/** \brief Exit point of function DoIP_HandleRxStartReady()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLERXSTARTREADY_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEDIAGHEADER_ENTRY
/** \brief Entry point of function DoIP_HandleDiagHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLEDIAGHEADER_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEDIAGHEADER_EXIT
/** \brief Exit point of function DoIP_HandleDiagHeader()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLEDIAGHEADER_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_STARTDIAGRECEPTION_ENTRY
/** \brief Entry point of function DoIP_StartDiagReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_STARTDIAGRECEPTION_ENTRY(TcpConIdx,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_STARTDIAGRECEPTION_EXIT
/** \brief Exit point of function DoIP_StartDiagReception()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_STARTDIAGRECEPTION_EXIT(Retval,TcpConIdx,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_COPYDIAGMSG_ENTRY
/** \brief Entry point of function DoIP_CopyDiagMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_COPYDIAGMSG_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_COPYDIAGMSG_EXIT
/** \brief Exit point of function DoIP_CopyDiagMsg()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_COPYDIAGMSG_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_DISCARDDIAGMSG_ENTRY
/** \brief Entry point of function DoIP_DiscardDiagMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_DISCARDDIAGMSG_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_DISCARDDIAGMSG_EXIT
/** \brief Exit point of function DoIP_DiscardDiagMsg()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_DISCARDDIAGMSG_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_DISCARDMSG_ENTRY
/** \brief Entry point of function DoIP_DiscardMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_DISCARDMSG_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_DISCARDMSG_EXIT
/** \brief Exit point of function DoIP_DiscardMsg()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_DISCARDMSG_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEALIVECHECKRESP_ENTRY
/** \brief Entry point of function DoIP_HandleAliveCheckResp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLEALIVECHECKRESP_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLEALIVECHECKRESP_EXIT
/** \brief Exit point of function DoIP_HandleAliveCheckResp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_HANDLEALIVECHECKRESP_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SENDTCPNONDIAGMSG_ENTRY
/** \brief Entry point of function DoIP_SendTcpNonDiagMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SENDTCPNONDIAGMSG_ENTRY(TcpConIdx,TxState,RespCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SENDTCPNONDIAGMSG_EXIT
/** \brief Exit point of function DoIP_SendTcpNonDiagMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SENDTCPNONDIAGMSG_EXIT(TcpConIdx,TxState,RespCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETTXSDULENGTH_ENTRY
/** \brief Entry point of function DoIP_GetTxSduLength()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETTXSDULENGTH_ENTRY(TcpConIdx,TxState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETTXSDULENGTH_EXIT
/** \brief Exit point of function DoIP_GetTxSduLength()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_GETTXSDULENGTH_EXIT(Retval,TcpConIdx,TxState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGMSGHDR_ENTRY
/** \brief Entry point of function DoIP_FillDiagMsgHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGMSGHDR_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGMSGHDR_EXIT
/** \brief Exit point of function DoIP_FillDiagMsgHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGMSGHDR_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLCUSTOMMSGHDR_ENTRY
/** \brief Entry point of function DoIP_FillCustomMsgHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLCUSTOMMSGHDR_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLCUSTOMMSGHDR_EXIT
/** \brief Exit point of function DoIP_FillCustomMsgHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLCUSTOMMSGHDR_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGACKNACKHDR_ENTRY
/** \brief Entry point of function DoIP_FillDiagAckNackHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGACKNACKHDR_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGACKNACKHDR_EXIT
/** \brief Exit point of function DoIP_FillDiagAckNackHdr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGACKNACKHDR_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGMSGPLD_ENTRY
/** \brief Entry point of function DoIP_FillDiagMsgPld()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGMSGPLD_ENTRY(TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGMSGPLD_EXIT
/** \brief Exit point of function DoIP_FillDiagMsgPld()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FILLDIAGMSGPLD_EXIT(Retval,TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLCUSTOMMSGPLD_ENTRY
/** \brief Entry point of function DoIP_FillCustomMsgPld()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLCUSTOMMSGPLD_ENTRY(TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLCUSTOMMSGPLD_EXIT
/** \brief Exit point of function DoIP_FillCustomMsgPld()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FILLCUSTOMMSGPLD_EXIT(Retval,TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGACKNACKPLD_ENTRY
/** \brief Entry point of function DoIP_FillDiagAckNackPld()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLDIAGACKNACKPLD_ENTRY(TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLDIAGACKNACKPLD_EXIT
/** \brief Exit point of function DoIP_FillDiagAckNackPld()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FILLDIAGACKNACKPLD_EXIT(Retval,TcpConIdx,PduInfoPtr,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADREQUESTRELEASEIPADDR_ENTRY
/** \brief Entry point of function DoIP_SoAdRequestReleaseIpAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADREQUESTRELEASEIPADDR_ENTRY(SoConId,Operation)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADREQUESTRELEASEIPADDR_EXIT
/** \brief Exit point of function DoIP_SoAdRequestReleaseIpAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADREQUESTRELEASEIPADDR_EXIT(SoConId,Operation)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_ENTRY
/** \brief Entry point of function DoIP_RequestReleaseIpAddrAllConn()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_ENTRY(Operation)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_EXIT
/** \brief Exit point of function DoIP_RequestReleaseIpAddrAllConn()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_REQUESTRELEASEIPADDRALLCONN_EXIT(Operation)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETUPDATEHOSTNAMEOPTION_ENTRY
/** \brief Entry point of function DoIP_GetUpdateHostnameOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETUPDATEHOSTNAMEOPTION_ENTRY(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETUPDATEHOSTNAMEOPTION_EXIT
/** \brief Exit point of function DoIP_GetUpdateHostnameOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETUPDATEHOSTNAMEOPTION_EXIT(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CLOSEALLCON_ENTRY
/** \brief Entry point of function DoIP_CloseAllCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CLOSEALLCON_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CLOSEALLCON_EXIT
/** \brief Exit point of function DoIP_CloseAllCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CLOSEALLCON_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_OPENALLCON_ENTRY
/** \brief Entry point of function DoIP_OpenAllCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_OPENALLCON_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_OPENALLCON_EXIT
/** \brief Exit point of function DoIP_OpenAllCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_OPENALLCON_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_UDP_CHECK_GENHEADER_ENTRY
/** \brief Entry point of function DoIP_Udp_Check_GenHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_UDP_CHECK_GENHEADER_ENTRY(DoIP_SoAdRxPduId,PduInfoPtr,PayloadTypePtr,PayloadLengthPtr,Ret_IfTransmitPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_UDP_CHECK_GENHEADER_EXIT
/** \brief Exit point of function DoIP_Udp_Check_GenHeader()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_UDP_CHECK_GENHEADER_EXIT(Retval,DoIP_SoAdRxPduId,PduInfoPtr,PayloadTypePtr,PayloadLengthPtr,Ret_IfTransmitPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_TCP_CHECK_GENHEADER_ENTRY
/** \brief Entry point of function DoIP_Tcp_Check_GenHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_TCP_CHECK_GENHEADER_ENTRY(TcpConIdx,GenHdrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_TCP_CHECK_GENHEADER_EXIT
/** \brief Exit point of function DoIP_Tcp_Check_GenHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_TCP_CHECK_GENHEADER_EXIT(TcpConIdx,GenHdrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_GENHEADER_ENTRY
/** \brief Entry point of function DoIP_Fill_GenHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_GENHEADER_ENTRY(PduPtr,PayloadLength,PayloadType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_GENHEADER_EXIT
/** \brief Exit point of function DoIP_Fill_GenHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_GENHEADER_EXIT(PduPtr,PayloadLength,PayloadType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLROUTINGACTRESPONSE_ENTRY
/** \brief Entry point of function DoIP_FillRoutingActResponse()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLROUTINGACTRESPONSE_ENTRY(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILLROUTINGACTRESPONSE_EXIT
/** \brief Exit point of function DoIP_FillRoutingActResponse()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILLROUTINGACTRESPONSE_EXIT(TcpConIdx,SduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_DIAGHEADER_ENTRY
/** \brief Entry point of function DoIP_Fill_DiagHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_DIAGHEADER_ENTRY(PduPtr,TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_DIAGHEADER_EXIT
/** \brief Exit point of function DoIP_Fill_DiagHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_DIAGHEADER_EXIT(PduPtr,TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_DIAGACKNACKHEADER_ENTRY
/** \brief Entry point of function DoIP_Fill_DiagAckNackHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_DIAGACKNACKHEADER_ENTRY(TcpConIdx,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FILL_DIAGACKNACKHEADER_EXIT
/** \brief Exit point of function DoIP_Fill_DiagAckNackHeader()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FILL_DIAGACKNACKHEADER_EXIT(TcpConIdx,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_VID_RESP_ANN_ENTRY
/** \brief Entry point of function DoIP_Send_VID_Resp_Ann()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SEND_VID_RESP_ANN_ENTRY(SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_VID_RESP_ANN_EXIT
/** \brief Exit point of function DoIP_Send_VID_Resp_Ann()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SEND_VID_RESP_ANN_EXIT(Retval,SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_ENTITY_STATUS_RESP_ENTRY
/** \brief Entry point of function DoIP_Send_Entity_Status_Resp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SEND_ENTITY_STATUS_RESP_ENTRY(SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_ENTITY_STATUS_RESP_EXIT
/** \brief Exit point of function DoIP_Send_Entity_Status_Resp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SEND_ENTITY_STATUS_RESP_EXIT(Retval,SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_POWERMODE_RESP_ENTRY
/** \brief Entry point of function DoIP_Send_PowerMode_Resp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SEND_POWERMODE_RESP_ENTRY(SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SEND_POWERMODE_RESP_EXIT
/** \brief Exit point of function DoIP_Send_PowerMode_Resp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SEND_POWERMODE_RESP_EXIT(Retval,SoAdTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FINDTCPCON_ENTRY
/** \brief Entry point of function DoIP_FindTcpCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_FINDTCPCON_ENTRY(DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_FINDTCPCON_EXIT
/** \brief Exit point of function DoIP_FindTcpCon()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_FINDTCPCON_EXIT(Retval,DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_RESETTCPCON_ENTRY
/** \brief Entry point of function DoIP_ResetTcpCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_RESETTCPCON_ENTRY(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_RESETTCPCON_EXIT
/** \brief Exit point of function DoIP_ResetTcpCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_RESETTCPCON_EXIT(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAKE_UINT16_ENTRY
/** \brief Entry point of function DoIP_Make_uint16()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAKE_UINT16_ENTRY(Byte1_Msb,Byte0_Lsb)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAKE_UINT16_EXIT
/** \brief Exit point of function DoIP_Make_uint16()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_MAKE_UINT16_EXIT(Retval,Byte1_Msb,Byte0_Lsb)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAKE_UINT32_ENTRY
/** \brief Entry point of function DoIP_Make_uint32()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAKE_UINT32_ENTRY(Byte3_Msb,Byte2,Byte1,Byte0_Lsb)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAKE_UINT32_EXIT
/** \brief Exit point of function DoIP_Make_uint32()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_MAKE_UINT32_EXIT(Retval,Byte3_Msb,Byte2,Byte1,Byte0_Lsb)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PREPARETCPCONCUSTOM_ENTRY
/** \brief Entry point of function DoIP_PrepareTcpConCustom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PREPARETCPCONCUSTOM_ENTRY(TcpConIdx,DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_PREPARETCPCONCUSTOM_EXIT
/** \brief Exit point of function DoIP_PrepareTcpConCustom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_PREPARETCPCONCUSTOM_EXIT(TcpConIdx,DoIPSoAdPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_RESETTCPCONCUSTOM_ENTRY
/** \brief Entry point of function DoIP_ResetTcpConCustom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_RESETTCPCONCUSTOM_ENTRY(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_RESETTCPCONCUSTOM_EXIT
/** \brief Exit point of function DoIP_ResetTcpConCustom()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_RESETTCPCONCUSTOM_EXIT(TcpConIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_COPYCUSTOMMSGPAYLOAD_ENTRY
/** \brief Entry point of function DoIP_CopyCustomMsgPayload()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_COPYCUSTOMMSGPAYLOAD_ENTRY(TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_COPYCUSTOMMSGPAYLOAD_EXIT
/** \brief Exit point of function DoIP_CopyCustomMsgPayload()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_COPYCUSTOMMSGPAYLOAD_EXIT(Retval,TcpConIdx,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLECUSTOMMSG_ENTRY
/** \brief Entry point of function DoIP_HandleCustomMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLECUSTOMMSG_ENTRY(TcpConIdx,PayloadType,PayloadLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_HANDLECUSTOMMSG_EXIT
/** \brief Exit point of function DoIP_HandleCustomMsg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_HANDLECUSTOMMSG_EXIT(TcpConIdx,PayloadType,PayloadLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/
/*==================[internal function definitions]=========================*/
#else /* DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#define DOIP_TRACE_INTERNAL_H_DISABLED
#endif /* DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#endif /* ifndef DOIP_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
