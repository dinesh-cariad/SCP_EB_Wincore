/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LDCOM_TRACE_INTERNAL_H
#define LDCOM_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function LdCom_DummyCbkStartOfReception() */
#define DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_ENTRY(a,b)
#endif

#ifndef DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_EXIT
/** \brief Exit point of function LdCom_DummyCbkStartOfReception() */
#define DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_EXIT(a,b,c)
#endif

#ifndef DBG_LDCOM_DUMMYCBKCOPYRXDATA_ENTRY
/** \brief Entry point of function LdCom_DummyCbkCopyRxData() */
#define DBG_LDCOM_DUMMYCBKCOPYRXDATA_ENTRY(a,b)
#endif

#ifndef DBG_LDCOM_DUMMYCBKCOPYRXDATA_EXIT
/** \brief Exit point of function LdCom_DummyCbkCopyRxData() */
#define DBG_LDCOM_DUMMYCBKCOPYRXDATA_EXIT(a,b,c)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTPRXINDICATION_ENTRY
/** \brief Entry point of function LdCom_DummyCbkTpRxIndication() */
#define DBG_LDCOM_DUMMYCBKTPRXINDICATION_ENTRY(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTPRXINDICATION_EXIT
/** \brief Exit point of function LdCom_DummyCbkTpRxIndication() */
#define DBG_LDCOM_DUMMYCBKTPRXINDICATION_EXIT(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKCOPYTXDATA_ENTRY
/** \brief Entry point of function LdCom_DummyCbkCopyTxData() */
#define DBG_LDCOM_DUMMYCBKCOPYTXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_LDCOM_DUMMYCBKCOPYTXDATA_EXIT
/** \brief Exit point of function LdCom_DummyCbkCopyTxData() */
#define DBG_LDCOM_DUMMYCBKCOPYTXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_ENTRY
/** \brief Entry point of function LdCom_DummyCbkTpTxConfirmation() */
#define DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_EXIT
/** \brief Exit point of function LdCom_DummyCbkTpTxConfirmation() */
#define DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKRXINDICATION_ENTRY
/** \brief Entry point of function LdCom_DummyCbkRxIndication() */
#define DBG_LDCOM_DUMMYCBKRXINDICATION_ENTRY(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKRXINDICATION_EXIT
/** \brief Exit point of function LdCom_DummyCbkRxIndication() */
#define DBG_LDCOM_DUMMYCBKRXINDICATION_EXIT(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function LdCom_DummyCbkTriggerTransmit() */
#define DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_ENTRY(a)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_EXIT
/** \brief Exit point of function LdCom_DummyCbkTriggerTransmit() */
#define DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_LDCOM_DUMMYCBKTXCONFIRMATION_ENTRY
/** \brief Entry point of function LdCom_DummyCbkTxConfirmation() */
#define DBG_LDCOM_DUMMYCBKTXCONFIRMATION_ENTRY()
#endif

#ifndef DBG_LDCOM_DUMMYCBKTXCONFIRMATION_EXIT
/** \brief Exit point of function LdCom_DummyCbkTxConfirmation() */
#define DBG_LDCOM_DUMMYCBKTXCONFIRMATION_EXIT()
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

#endif /* ifndef LDCOM_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
