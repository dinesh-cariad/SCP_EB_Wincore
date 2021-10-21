/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PDUR_TRACE_H
#define PDUR_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>   /* custom implementations of the *_ENTRY and *_EXIT macros */

/*==================[macros]================================================*/

#ifndef DBG_PDUR_GATEIFRXINDICATIONDF_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationDf() */
#define DBG_PDUR_GATEIFRXINDICATIONDF_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONDF_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationDf() */
#define DBG_PDUR_GATEIFRXINDICATIONDF_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationDfDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationDfDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONDFDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONNB_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationNb() */
#define DBG_PDUR_GATEIFRXINDICATIONNB_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONNB_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationNb() */
#define DBG_PDUR_GATEIFRXINDICATIONNB_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSB_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationSb() */
#define DBG_PDUR_GATEIFRXINDICATIONSB_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSB_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationSb() */
#define DBG_PDUR_GATEIFRXINDICATIONSB_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSBDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationSbDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONSBDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSBDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationSbDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONSBDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSBMCOREDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationSbMCoreDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONSBMCOREDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONSBMCOREDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationSbMCoreDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONSBMCOREDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONTF_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationTf() */
#define DBG_PDUR_GATEIFRXINDICATIONTF_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONTF_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationTf() */
#define DBG_PDUR_GATEIFRXINDICATIONTF_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationTfDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationTfDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONTFDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationDfMCoreDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationDfMCoreDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONDFMCOREDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITSB_ENTRY
/** \brief Entry point of function PduR_GateIfTriggerTransmitSb() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITSB_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITSB_EXIT
/** \brief Exit point of function PduR_GateIfTriggerTransmitSb() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITSB_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITSBDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfTriggerTransmitSbDynPyld() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITSBDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITSBDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfTriggerTransmitSbDynPyld() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITSBDYNPYLD_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITTF_ENTRY
/** \brief Entry point of function PduR_GateIfTriggerTransmitTf() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITTF_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITTF_EXIT
/** \brief Exit point of function PduR_GateIfTriggerTransmitTf() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITTF_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITTFDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfTriggerTransmitTfDynPyld() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITTFDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFTRIGGERTRANSMITTFDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfTriggerTransmitTfDynPyld() */
#define DBG_PDUR_GATEIFTRIGGERTRANSMITTFDYNPYLD_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDF_ENTRY
/** \brief Entry point of function PduR_GateIfTxConfirmationDf() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDF_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDF_EXIT
/** \brief Exit point of function PduR_GateIfTxConfirmationDf() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDF_EXIT(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDFDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfTxConfirmationDfDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDFDYNPYLD_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDFDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfTxConfirmationDfDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDFDYNPYLD_EXIT(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONTF_ENTRY
/** \brief Entry point of function PduR_GateIfTxConfirmationTf() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONTF_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONTF_EXIT
/** \brief Exit point of function PduR_GateIfTxConfirmationTf() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONTF_EXIT(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONTFDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfTxConfirmationTfDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONTFDYNPYLD_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONTFDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfTxConfirmationTfDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONTFDYNPYLD_EXIT(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDFMCOREDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfTxConfirmationDfMCoreDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDFMCOREDYNPYLD_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATEIFTXCONFIRMATIONDFMCOREDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfTxConfirmationDfMCoreDynPyld() */
#define DBG_PDUR_GATEIFTXCONFIRMATIONDFMCOREDYNPYLD_EXIT(a)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONMCUPSBDYNPYLD_ENTRY
/** \brief Entry point of function PduR_GateIfRxIndicationMcUpSbDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONMCUPSBDYNPYLD_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATEIFRXINDICATIONMCUPSBDYNPYLD_EXIT
/** \brief Exit point of function PduR_GateIfRxIndicationMcUpSbDynPyld() */
#define DBG_PDUR_GATEIFRXINDICATIONMCUPSBDYNPYLD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_RELEASEGATETPSESSION_ENTRY
/** \brief Entry point of function PduR_ReleaseGateTpSession() */
#define DBG_PDUR_RELEASEGATETPSESSION_ENTRY(a)
#endif

#ifndef DBG_PDUR_RELEASEGATETPSESSION_EXIT
/** \brief Exit point of function PduR_ReleaseGateTpSession() */
#define DBG_PDUR_RELEASEGATETPSESSION_EXIT(a)
#endif

#ifndef DBG_PDUR_RELEASEGATETPSESSIONQUEUE_ENTRY
/** \brief Entry point of function PduR_ReleaseGateTpSessionQueue() */
#define DBG_PDUR_RELEASEGATETPSESSIONQUEUE_ENTRY(a)
#endif

#ifndef DBG_PDUR_RELEASEGATETPSESSIONQUEUE_EXIT
/** \brief Exit point of function PduR_ReleaseGateTpSessionQueue() */
#define DBG_PDUR_RELEASEGATETPSESSIONQUEUE_EXIT(a)
#endif

#ifndef DBG_PDUR_RELEASEGATETPQUEUEELEMENT_ENTRY
/** \brief Entry point of function PduR_ReleaseGateTpQueueElement() */
#define DBG_PDUR_RELEASEGATETPQUEUEELEMENT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_RELEASEGATETPQUEUEELEMENT_EXIT
/** \brief Exit point of function PduR_ReleaseGateTpQueueElement() */
#define DBG_PDUR_RELEASEGATETPQUEUEELEMENT_EXIT(a,b)
#endif

#ifndef DBG_PDUR_RESETGATETPQUEUEHEAD_ENTRY
/** \brief Entry point of function PduR_ResetGateTpQueueHead() */
#define DBG_PDUR_RESETGATETPQUEUEHEAD_ENTRY(a)
#endif

#ifndef DBG_PDUR_RESETGATETPQUEUEHEAD_EXIT
/** \brief Exit point of function PduR_ResetGateTpQueueHead() */
#define DBG_PDUR_RESETGATETPQUEUEHEAD_EXIT(a)
#endif

#ifndef DBG_PDUR_GETGATETPSESSIONINUSE_ENTRY
/** \brief Entry point of function PduR_GetGateTpSessionInUse() */
#define DBG_PDUR_GETGATETPSESSIONINUSE_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GETGATETPSESSIONINUSE_EXIT
/** \brief Exit point of function PduR_GetGateTpSessionInUse() */
#define DBG_PDUR_GETGATETPSESSIONINUSE_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_CALCAVAILABLEDATASIZE_ENTRY
/** \brief Entry point of function PduR_CalcAvailableDataSize() */
#define DBG_PDUR_CALCAVAILABLEDATASIZE_ENTRY(a)
#endif

#ifndef DBG_PDUR_CALCAVAILABLEDATASIZE_EXIT
/** \brief Exit point of function PduR_CalcAvailableDataSize() */
#define DBG_PDUR_CALCAVAILABLEDATASIZE_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPNOSESSIONQUEUED_ENTRY
/** \brief Entry point of function PduR_GateTpNoSessionQueued() */
#define DBG_PDUR_GATETPNOSESSIONQUEUED_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATETPNOSESSIONQUEUED_EXIT
/** \brief Exit point of function PduR_GateTpNoSessionQueued() */
#define DBG_PDUR_GATETPNOSESSIONQUEUED_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPGETSESSIONQUEUEHEADINDEX_ENTRY
/** \brief Entry point of function PduR_GateTpGetSessionQueueHeadIndex() */
#define DBG_PDUR_GATETPGETSESSIONQUEUEHEADINDEX_ENTRY(a)
#endif

#ifndef DBG_PDUR_GATETPGETSESSIONQUEUEHEADINDEX_EXIT
/** \brief Exit point of function PduR_GateTpGetSessionQueueHeadIndex() */
#define DBG_PDUR_GATETPGETSESSIONQUEUEHEADINDEX_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPSETSESSIONQUEUEHEADINDEX_ENTRY
/** \brief Entry point of function PduR_GateTpSetSessionQueueHeadIndex() */
#define DBG_PDUR_GATETPSETSESSIONQUEUEHEADINDEX_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATETPSETSESSIONQUEUEHEADINDEX_EXIT
/** \brief Exit point of function PduR_GateTpSetSessionQueueHeadIndex() */
#define DBG_PDUR_GATETPSETSESSIONQUEUEHEADINDEX_EXIT(a,b)
#endif

#ifndef DBG_PDUR_ISGATETPSESSIONINUSE_ENTRY
/** \brief Entry point of function PduR_IsGateTpSessionInUse() */
#define DBG_PDUR_ISGATETPSESSIONINUSE_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_ISGATETPSESSIONINUSE_EXIT
/** \brief Exit point of function PduR_IsGateTpSessionInUse() */
#define DBG_PDUR_ISGATETPSESSIONINUSE_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATETPCOPYRXDATA_ENTRY
/** \brief Entry point of function PduR_GateTpCopyRxData() */
#define DBG_PDUR_GATETPCOPYRXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GATETPCOPYRXDATA_EXIT
/** \brief Exit point of function PduR_GateTpCopyRxData() */
#define DBG_PDUR_GATETPCOPYRXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_NTO1GATETPCOPYRXDATA_ENTRY
/** \brief Entry point of function PduR_Nto1GateTpCopyRxData() */
#define DBG_PDUR_NTO1GATETPCOPYRXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_NTO1GATETPCOPYRXDATA_EXIT
/** \brief Exit point of function PduR_Nto1GateTpCopyRxData() */
#define DBG_PDUR_NTO1GATETPCOPYRXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_COPYDATATOTPBUFFER_ENTRY
/** \brief Entry point of function PduR_CopyDataToTpBuffer() */
#define DBG_PDUR_COPYDATATOTPBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_COPYDATATOTPBUFFER_EXIT
/** \brief Exit point of function PduR_CopyDataToTpBuffer() */
#define DBG_PDUR_COPYDATATOTPBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GATETPCOPYTXDATA_ENTRY
/** \brief Entry point of function PduR_GateTpCopyTxData() */
#define DBG_PDUR_GATETPCOPYTXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_GATETPCOPYTXDATA_EXIT
/** \brief Exit point of function PduR_GateTpCopyTxData() */
#define DBG_PDUR_GATETPCOPYTXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_NTO1GATETPCOPYTXDATA_ENTRY
/** \brief Entry point of function PduR_Nto1GateTpCopyTxData() */
#define DBG_PDUR_NTO1GATETPCOPYTXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_NTO1GATETPCOPYTXDATA_EXIT
/** \brief Exit point of function PduR_Nto1GateTpCopyTxData() */
#define DBG_PDUR_NTO1GATETPCOPYTXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_ENTRY
/** \brief Entry point of function PduR_CopyDataFromCircularBuffer() */
#define DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_EXIT
/** \brief Exit point of function PduR_CopyDataFromCircularBuffer() */
#define DBG_PDUR_COPYDATAFROMCIRCULARBUFFER_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_COPYDATAFROMLINEARBUFFER_ENTRY
/** \brief Entry point of function PduR_CopyDataFromLinearBuffer() */
#define DBG_PDUR_COPYDATAFROMLINEARBUFFER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_COPYDATAFROMLINEARBUFFER_EXIT
/** \brief Exit point of function PduR_CopyDataFromLinearBuffer() */
#define DBG_PDUR_COPYDATAFROMLINEARBUFFER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_GATETPRXINDICATION_ENTRY
/** \brief Entry point of function PduR_GateTpRxIndication() */
#define DBG_PDUR_GATETPRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATETPRXINDICATION_EXIT
/** \brief Exit point of function PduR_GateTpRxIndication() */
#define DBG_PDUR_GATETPRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_NTO1GATETPRXINDICATION_ENTRY
/** \brief Entry point of function PduR_Nto1GateTpRxIndication() */
#define DBG_PDUR_NTO1GATETPRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_NTO1GATETPRXINDICATION_EXIT
/** \brief Exit point of function PduR_Nto1GateTpRxIndication() */
#define DBG_PDUR_NTO1GATETPRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function PduR_GateTpStartOfReception() */
#define DBG_PDUR_GATETPSTARTOFRECEPTION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GATETPSTARTOFRECEPTION_EXIT
/** \brief Exit point of function PduR_GateTpStartOfReception() */
#define DBG_PDUR_GATETPSTARTOFRECEPTION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function PduR_Nto1GateTpStartOfReception() */
#define DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_EXIT
/** \brief Exit point of function PduR_Nto1GateTpStartOfReception() */
#define DBG_PDUR_NTO1GATETPSTARTOFRECEPTION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_ASSIGNGATETPSESSION_ENTRY
/** \brief Entry point of function PduR_AssignGateTpSession() */
#define DBG_PDUR_ASSIGNGATETPSESSION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_ASSIGNGATETPSESSION_EXIT
/** \brief Exit point of function PduR_AssignGateTpSession() */
#define DBG_PDUR_ASSIGNGATETPSESSION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_ASSIGNTPBUFFERROTF_ENTRY
/** \brief Entry point of function PduR_AssignTpBufferRotf() */
#define DBG_PDUR_ASSIGNTPBUFFERROTF_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_ASSIGNTPBUFFERROTF_EXIT
/** \brief Exit point of function PduR_AssignTpBufferRotf() */
#define DBG_PDUR_ASSIGNTPBUFFERROTF_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_ENTRY
/** \brief Entry point of function PduR_AssignNextBiggerTpBuffer() */
#define DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_ENTRY(a)
#endif

#ifndef DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_EXIT
/** \brief Exit point of function PduR_AssignNextBiggerTpBuffer() */
#define DBG_PDUR_ASSIGNNEXTBIGGERTPBUFFER_EXIT(a,b)
#endif

#ifndef DBG_PDUR_LARGESTTPBUFFERABOVE_ENTRY
/** \brief Entry point of function PduR_LargestTpBufferAbove() */
#define DBG_PDUR_LARGESTTPBUFFERABOVE_ENTRY(a)
#endif

#ifndef DBG_PDUR_LARGESTTPBUFFERABOVE_EXIT
/** \brief Exit point of function PduR_LargestTpBufferAbove() */
#define DBG_PDUR_LARGESTTPBUFFERABOVE_EXIT(a,b)
#endif

#ifndef DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_ENTRY
/** \brief Entry point of function PduR_AssignBiggestTpBufferInRange() */
#define DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_EXIT
/** \brief Exit point of function PduR_AssignBiggestTpBufferInRange() */
#define DBG_PDUR_ASSIGNBIGGESTTPBUFFERINRANGE_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GETGATETPQUEUEINDEXHEAD_ENTRY
/** \brief Entry point of function PduR_GetGateTpQueueIndexHead() */
#define DBG_PDUR_GETGATETPQUEUEINDEXHEAD_ENTRY(a)
#endif

#ifndef DBG_PDUR_GETGATETPQUEUEINDEXHEAD_EXIT
/** \brief Exit point of function PduR_GetGateTpQueueIndexHead() */
#define DBG_PDUR_GETGATETPQUEUEINDEXHEAD_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_GateTpTxConfirmation() */
#define DBG_PDUR_GATETPTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GATETPTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_GateTpTxConfirmation() */
#define DBG_PDUR_GATETPTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_NTO1GATETPTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_Nto1GateTpTxConfirmation() */
#define DBG_PDUR_NTO1GATETPTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_NTO1GATETPTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_Nto1GateTpTxConfirmation() */
#define DBG_PDUR_NTO1GATETPTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GATETPUPRECEPTION_ENTRY
/** \brief Entry point of function PduR_GateTpUpReception() */
#define DBG_PDUR_GATETPUPRECEPTION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GATETPUPRECEPTION_EXIT
/** \brief Exit point of function PduR_GateTpUpReception() */
#define DBG_PDUR_GATETPUPRECEPTION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICIFRXINDICATION_ENTRY
/** \brief Entry point of function PduR_GenericIfRxIndication() */
#define DBG_PDUR_GENERICIFRXINDICATION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICIFRXINDICATION_EXIT
/** \brief Exit point of function PduR_GenericIfRxIndication() */
#define DBG_PDUR_GENERICIFRXINDICATION_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICIFTRANSMIT_ENTRY
/** \brief Entry point of function PduR_GenericIfTransmit() */
#define DBG_PDUR_GENERICIFTRANSMIT_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICIFTRANSMIT_EXIT
/** \brief Exit point of function PduR_GenericIfTransmit() */
#define DBG_PDUR_GENERICIFTRANSMIT_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICIFTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function PduR_GenericIfTriggerTransmit() */
#define DBG_PDUR_GENERICIFTRIGGERTRANSMIT_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICIFTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function PduR_GenericIfTriggerTransmit() */
#define DBG_PDUR_GENERICIFTRIGGERTRANSMIT_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_GenericIfTxConfirmation() */
#define DBG_PDUR_GENERICIFTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GENERICIFTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_GenericIfTxConfirmation() */
#define DBG_PDUR_GENERICIFTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GENERICTPCANCELRECEIVE_ENTRY
/** \brief Entry point of function PduR_GenericTpCancelReceive() */
#define DBG_PDUR_GENERICTPCANCELRECEIVE_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GENERICTPCANCELRECEIVE_EXIT
/** \brief Exit point of function PduR_GenericTpCancelReceive() */
#define DBG_PDUR_GENERICTPCANCELRECEIVE_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPCANCELTRANSMIT_ENTRY
/** \brief Entry point of function PduR_GenericTpCancelTransmit() */
#define DBG_PDUR_GENERICTPCANCELTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_GENERICTPCANCELTRANSMIT_EXIT
/** \brief Exit point of function PduR_GenericTpCancelTransmit() */
#define DBG_PDUR_GENERICTPCANCELTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPCHANGEPARAMETER_ENTRY
/** \brief Entry point of function PduR_GenericTpChangeParameter() */
#define DBG_PDUR_GENERICTPCHANGEPARAMETER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICTPCHANGEPARAMETER_EXIT
/** \brief Exit point of function PduR_GenericTpChangeParameter() */
#define DBG_PDUR_GENERICTPCHANGEPARAMETER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_GENERICTPCOPYRXDATA_ENTRY
/** \brief Entry point of function PduR_GenericTpCopyRxData() */
#define DBG_PDUR_GENERICTPCOPYRXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICTPCOPYRXDATA_EXIT
/** \brief Exit point of function PduR_GenericTpCopyRxData() */
#define DBG_PDUR_GENERICTPCOPYRXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_GENERICTPCOPYTXDATA_ENTRY
/** \brief Entry point of function PduR_GenericTpCopyTxData() */
#define DBG_PDUR_GENERICTPCOPYTXDATA_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_GENERICTPCOPYTXDATA_EXIT
/** \brief Exit point of function PduR_GenericTpCopyTxData() */
#define DBG_PDUR_GENERICTPCOPYTXDATA_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_PDUR_GENERICTPRXINDICATION_ENTRY
/** \brief Entry point of function PduR_GenericTpRxIndication() */
#define DBG_PDUR_GENERICTPRXINDICATION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPRXINDICATION_EXIT
/** \brief Exit point of function PduR_GenericTpRxIndication() */
#define DBG_PDUR_GENERICTPRXINDICATION_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function PduR_GenericTpStartOfReception() */
#define DBG_PDUR_GENERICTPSTARTOFRECEPTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICTPSTARTOFRECEPTION_EXIT
/** \brief Exit point of function PduR_GenericTpStartOfReception() */
#define DBG_PDUR_GENERICTPSTARTOFRECEPTION_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_GENERICTPTRANSMIT_ENTRY
/** \brief Entry point of function PduR_GenericTpTransmit() */
#define DBG_PDUR_GENERICTPTRANSMIT_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPTRANSMIT_EXIT
/** \brief Exit point of function PduR_GenericTpTransmit() */
#define DBG_PDUR_GENERICTPTRANSMIT_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_GENERICTPTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_GenericTpTxConfirmation() */
#define DBG_PDUR_GENERICTPTXCONFIRMATION_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_GENERICTPTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_GenericTpTxConfirmation() */
#define DBG_PDUR_GENERICTPTXCONFIRMATION_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_GETCONFIGURATIONID_ENTRY
/** \brief Entry point of function PduR_GetConfigurationId() */
#define DBG_PDUR_GETCONFIGURATIONID_ENTRY()
#endif

#ifndef DBG_PDUR_GETCONFIGURATIONID_EXIT
/** \brief Exit point of function PduR_GetConfigurationId() */
#define DBG_PDUR_GETCONFIGURATIONID_EXIT(a)
#endif

#ifndef DBG_PDUR_GETVERSIONINFO_ENTRY
/** \brief Entry point of function PduR_GetVersionInfo() */
#define DBG_PDUR_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_PDUR_GETVERSIONINFO_EXIT
/** \brief Exit point of function PduR_GetVersionInfo() */
#define DBG_PDUR_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_PDUR_INIT_ENTRY
/** \brief Entry point of function PduR_Init() */
#define DBG_PDUR_INIT_ENTRY(a)
#endif

#ifndef DBG_PDUR_INIT_EXIT
/** \brief Exit point of function PduR_Init() */
#define DBG_PDUR_INIT_EXIT(a)
#endif

#ifndef DBG_PDUR_INITBUFFER_ENTRY
/** \brief Entry point of function PduR_InitBuffer() */
#define DBG_PDUR_INITBUFFER_ENTRY()
#endif

#ifndef DBG_PDUR_INITBUFFER_EXIT
/** \brief Exit point of function PduR_InitBuffer() */
#define DBG_PDUR_INITBUFFER_EXIT()
#endif

#ifndef DBG_PDUR_INITTPGATEWAY_ENTRY
/** \brief Entry point of function PduR_InitTpGateway() */
#define DBG_PDUR_INITTPGATEWAY_ENTRY()
#endif

#ifndef DBG_PDUR_INITTPGATEWAY_EXIT
/** \brief Exit point of function PduR_InitTpGateway() */
#define DBG_PDUR_INITTPGATEWAY_EXIT()
#endif

#ifndef DBG_PDUR_INITMULTITPTX_ENTRY
/** \brief Entry point of function PduR_InitMultiTpTx() */
#define DBG_PDUR_INITMULTITPTX_ENTRY()
#endif

#ifndef DBG_PDUR_INITMULTITPTX_EXIT
/** \brief Exit point of function PduR_InitMultiTpTx() */
#define DBG_PDUR_INITMULTITPTX_EXIT()
#endif

#ifndef DBG_PDUR_INITMULTIIFTX_ENTRY
/** \brief Entry point of function PduR_InitMultiIfTx() */
#define DBG_PDUR_INITMULTIIFTX_ENTRY()
#endif

#ifndef DBG_PDUR_INITMULTIIFTX_EXIT
/** \brief Exit point of function PduR_InitMultiIfTx() */
#define DBG_PDUR_INITMULTIIFTX_EXIT()
#endif

#ifndef DBG_PDUR_INITSBTXBUFFER_ENTRY
/** \brief Entry point of function PduR_InitSbTxBuffer() */
#define DBG_PDUR_INITSBTXBUFFER_ENTRY()
#endif

#ifndef DBG_PDUR_INITSBTXBUFFER_EXIT
/** \brief Exit point of function PduR_InitSbTxBuffer() */
#define DBG_PDUR_INITSBTXBUFFER_EXIT()
#endif

#ifndef DBG_PDUR_INITDFTXBUFFER_ENTRY
/** \brief Entry point of function PduR_InitDfTxBuffer() */
#define DBG_PDUR_INITDFTXBUFFER_ENTRY()
#endif

#ifndef DBG_PDUR_INITDFTXBUFFER_EXIT
/** \brief Exit point of function PduR_InitDfTxBuffer() */
#define DBG_PDUR_INITDFTXBUFFER_EXIT()
#endif

#ifndef DBG_PDUR_INITTFTXBUFFER_ENTRY
/** \brief Entry point of function PduR_InitTfTxBuffer() */
#define DBG_PDUR_INITTFTXBUFFER_ENTRY()
#endif

#ifndef DBG_PDUR_INITTFTXBUFFER_EXIT
/** \brief Exit point of function PduR_InitTfTxBuffer() */
#define DBG_PDUR_INITTFTXBUFFER_EXIT()
#endif

#ifndef DBG_PDUR_INITNTO1TABTP_ENTRY
/** \brief Entry point of function PduR_InitNto1TabTp() */
#define DBG_PDUR_INITNTO1TABTP_ENTRY()
#endif

#ifndef DBG_PDUR_INITNTO1TABTP_EXIT
/** \brief Exit point of function PduR_InitNto1TabTp() */
#define DBG_PDUR_INITNTO1TABTP_EXIT()
#endif

#ifndef DBG_PDUR_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function PduR_IsValidConfig() */
#define DBG_PDUR_ISVALIDCONFIG_ENTRY(a)
#endif

#ifndef DBG_PDUR_ISVALIDCONFIG_EXIT
/** \brief Exit point of function PduR_IsValidConfig() */
#define DBG_PDUR_ISVALIDCONFIG_EXIT(a,b)
#endif

#ifndef DBG_PDUR_MULTIIFRXINDICATION_ENTRY
/** \brief Entry point of function PduR_MultiIfRxIndication() */
#define DBG_PDUR_MULTIIFRXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTIIFRXINDICATION_EXIT
/** \brief Exit point of function PduR_MultiIfRxIndication() */
#define DBG_PDUR_MULTIIFRXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_ENTRY
/** \brief Entry point of function PduR_SearchMultiTpTxSessionIndex() */
#define DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_ENTRY(a)
#endif

#ifndef DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_EXIT
/** \brief Exit point of function PduR_SearchMultiTpTxSessionIndex() */
#define DBG_PDUR_SEARCHMULTITPTXSESSIONINDEX_EXIT(a,b)
#endif

#ifndef DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_ENTRY
/** \brief Entry point of function PduR_GetFirstMTabUpTpTxEntry() */
#define DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_ENTRY(a)
#endif

#ifndef DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_EXIT
/** \brief Exit point of function PduR_GetFirstMTabUpTpTxEntry() */
#define DBG_PDUR_GETFIRSTMTABUPTPTXENTRY_EXIT(a,b)
#endif

#ifndef DBG_PDUR_RELEASEMULTITPTXSESSION_ENTRY
/** \brief Entry point of function PduR_ReleaseMultiTpTxSession() */
#define DBG_PDUR_RELEASEMULTITPTXSESSION_ENTRY(a)
#endif

#ifndef DBG_PDUR_RELEASEMULTITPTXSESSION_EXIT
/** \brief Exit point of function PduR_ReleaseMultiTpTxSession() */
#define DBG_PDUR_RELEASEMULTITPTXSESSION_EXIT(a)
#endif

#ifndef DBG_PDUR_MULTITPCOPYTXDATA_ENTRY
/** \brief Entry point of function PduR_MultiTpCopyTxData() */
#define DBG_PDUR_MULTITPCOPYTXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_MULTITPCOPYTXDATA_EXIT
/** \brief Exit point of function PduR_MultiTpCopyTxData() */
#define DBG_PDUR_MULTITPCOPYTXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_MULTITPTRANSMIT_ENTRY
/** \brief Entry point of function PduR_MultiTpTransmit() */
#define DBG_PDUR_MULTITPTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTITPTRANSMIT_EXIT
/** \brief Exit point of function PduR_MultiTpTransmit() */
#define DBG_PDUR_MULTITPTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_ASSIGNMULTITPTXSESSION_ENTRY
/** \brief Entry point of function PduR_AssignMultiTpTxSession() */
#define DBG_PDUR_ASSIGNMULTITPTXSESSION_ENTRY(a)
#endif

#ifndef DBG_PDUR_ASSIGNMULTITPTXSESSION_EXIT
/** \brief Exit point of function PduR_AssignMultiTpTxSession() */
#define DBG_PDUR_ASSIGNMULTITPTXSESSION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_MULTITPTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_MultiTpTxConfirmation() */
#define DBG_PDUR_MULTITPTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTITPTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_MultiTpTxConfirmation() */
#define DBG_PDUR_MULTITPTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_PDUR_MULTIUPIFTRANSMIT_ENTRY
/** \brief Entry point of function PduR_MultiUpIfTransmit() */
#define DBG_PDUR_MULTIUPIFTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTIUPIFTRANSMIT_EXIT
/** \brief Exit point of function PduR_MultiUpIfTransmit() */
#define DBG_PDUR_MULTIUPIFTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_MULTIUPMCOREIFTRANSMIT_ENTRY
/** \brief Entry point of function PduR_MultiUpMCoreIfTransmit() */
#define DBG_PDUR_MULTIUPMCOREIFTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTIUPMCOREIFTRANSMIT_EXIT
/** \brief Exit point of function PduR_MultiUpMCoreIfTransmit() */
#define DBG_PDUR_MULTIUPMCOREIFTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_MULTIIFTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function PduR_MultiIfTriggerTransmit() */
#define DBG_PDUR_MULTIIFTRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_PDUR_MULTIIFTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function PduR_MultiIfTriggerTransmit() */
#define DBG_PDUR_MULTIIFTRIGGERTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_PDUR_MULTIIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_MultiIfTxConfirmation() */
#define DBG_PDUR_MULTIIFTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_PDUR_MULTIIFTXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_MultiIfTxConfirmation() */
#define DBG_PDUR_MULTIIFTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_PDUR_MULTIIFTXGETBYTEINDEX_ENTRY
/** \brief Entry point of function PduR_MultiIfTxGetByteIndex() */
#define DBG_PDUR_MULTIIFTXGETBYTEINDEX_ENTRY(a,b,c)
#endif

#ifndef DBG_PDUR_MULTIIFTXGETBYTEINDEX_EXIT
/** \brief Exit point of function PduR_MultiIfTxGetByteIndex() */
#define DBG_PDUR_MULTIIFTXGETBYTEINDEX_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_WRAPASR32STARTOFRECEPTION_ENTRY
/** \brief Entry point of function PduR_WrapASR32StartOfReception() */
#define DBG_PDUR_WRAPASR32STARTOFRECEPTION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_PDUR_WRAPASR32STARTOFRECEPTION_EXIT
/** \brief Exit point of function PduR_WrapASR32StartOfReception() */
#define DBG_PDUR_WRAPASR32STARTOFRECEPTION_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_PDUR_WRAPASR32COPYRXDATA_ENTRY
/** \brief Entry point of function PduR_WrapASR32CopyRxData() */
#define DBG_PDUR_WRAPASR32COPYRXDATA_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_PDUR_WRAPASR32COPYRXDATA_EXIT
/** \brief Exit point of function PduR_WrapASR32CopyRxData() */
#define DBG_PDUR_WRAPASR32COPYRXDATA_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_PDUR_WRAPASR32RXINDICATION_ENTRY
/** \brief Entry point of function PduR_WrapASR32RxIndication() */
#define DBG_PDUR_WRAPASR32RXINDICATION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_WRAPASR32RXINDICATION_EXIT
/** \brief Exit point of function PduR_WrapASR32RxIndication() */
#define DBG_PDUR_WRAPASR32RXINDICATION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_WRAPASR32COPYTXDATA_ENTRY
/** \brief Entry point of function PduR_WrapASR32CopyTxData() */
#define DBG_PDUR_WRAPASR32COPYTXDATA_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_PDUR_WRAPASR32COPYTXDATA_EXIT
/** \brief Exit point of function PduR_WrapASR32CopyTxData() */
#define DBG_PDUR_WRAPASR32COPYTXDATA_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_PDUR_WRAPASR32TXCONFIRMATION_ENTRY
/** \brief Entry point of function PduR_WrapASR32TxConfirmation() */
#define DBG_PDUR_WRAPASR32TXCONFIRMATION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_PDUR_WRAPASR32TXCONFIRMATION_EXIT
/** \brief Exit point of function PduR_WrapASR32TxConfirmation() */
#define DBG_PDUR_WRAPASR32TXCONFIRMATION_EXIT(a,b,c,d)
#endif

#ifndef DBG_PDUR_ENABLEROUTING_ENTRY
/** \brief Entry point of function PduR_EnableRouting() */
#define DBG_PDUR_ENABLEROUTING_ENTRY(a)
#endif

#ifndef DBG_PDUR_ENABLEROUTING_EXIT
/** \brief Exit point of function PduR_EnableRouting() */
#define DBG_PDUR_ENABLEROUTING_EXIT(a)
#endif

#ifndef DBG_PDUR_DISABLEROUTING_ENTRY
/** \brief Entry point of function PduR_DisableRouting() */
#define DBG_PDUR_DISABLEROUTING_ENTRY(a)
#endif

#ifndef DBG_PDUR_DISABLEROUTING_EXIT
/** \brief Exit point of function PduR_DisableRouting() */
#define DBG_PDUR_DISABLEROUTING_EXIT(a)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef PDUR_TRACE_H */
/*==================[end of file]===========================================*/
