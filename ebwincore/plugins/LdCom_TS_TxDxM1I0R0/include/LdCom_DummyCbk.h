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
#ifndef LDCOM_DUMMYCBK_H
#define LDCOM_DUMMYCBK_H

/*==================[inclusions]============================================*/
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>

/*==========================================================================*/
#include <LdCom_Cfg.h>           /* Module configurable parameters */

#define LDCOM_START_SEC_CODE
#include <LdCom_MemMap.h>

/* These functions are used as default dummy callback functions if the LdCom module assumes Rte callbacks for
   all PDUs even if they are configured within the system description but not used. As a consequence,
   the Rte will not generate the corresponding callbacks and the LdCom module calls unimplemented callbacks. 
   In this case the dummy callbacks shall be used by default. */

/*==================[macros]================================================*/

/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* !LINKSTO LdCom.EB.SWS_LDCOM_00038_DummyCbk,1 */
/**
 ** \brief This function is the dummy callback function of LdCom_StartOfReception.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  SduLength             Is unused and will be ignored.
 ** \param[out] RxBufferSizePtr       Is unused and will be ignored.
 **
 ** \retval BUFREQ_E_NOT_OK         Default return value for dummy callback function
 **
 **/
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_DummyCbkStartOfReception
(
  PduLengthType SduLength,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
);
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00039_DummyCbk,1 */
/**
 ** \brief This function is the dummy callback function of LdCom_CopyRxData
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  SduInfoPtr             Is unused and will be ignored.
 ** \param[out] RxBufferSizePtr        Is unused and will be ignored.
 **
 ** \retval BUFREQ_E_NOT_OK         Default return value for dummy callback function
 **
 **/
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_DummyCbkCopyRxData
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
);
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00040_DummyCbk,1 */
/**
 ** \brief This function is the dummy callback function of LdCom_TpRxIndication.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  Result      Is unused and will be ignored.
 **
 **/
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
extern FUNC(void, LDCOM_CODE) LdCom_DummyCbkTpRxIndication
(
  NotifResultType Result
);
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00036_DummyCbk,1 */
/**
 ** \brief This function is the dummy callback function of LdCom_CopyTxData.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[out] SduInfoPtr      Is unused and will be ignored.
 ** \param[in]  RetryInfoPtr    Is unused and will be ignored.
 ** \param[out] TxDataCntPtr    Is unused and will be ignored.
 **
 ** \retval BUFREQ_E_NOT_OK     Default return value for dummy callback function
 **
 **/
#if (LDCOM_TPTXAPIS_AVAILABLE == STD_ON)
extern FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_DummyCbkCopyTxData
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) TxDataCntPtr
);
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00037_DummyCbk,1 */
/**
 ** \brief This function is the dummy callback function of LdCom_TpTxConfirmation.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  Result      Is unused and will be ignored.
 **
 **/
#if (LDCOM_TPTXAPIS_AVAILABLE == STD_ON)
extern FUNC(void, LDCOM_CODE) LdCom_DummyCbkTpTxConfirmation
(
  NotifResultType Result
);
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00041_DummyCbk,1 */
/**
 ** \brief  This function is the dummy callback function of LdCom_RxIndication.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  PduInfoPtr     Is unused and will be ignored.
 **
 **/
#if (LDCOM_IFRXAPIS_AVAILABLE == STD_ON)
extern FUNC(void, LDCOM_CODE) LdCom_DummyCbkRxIndication
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);
#endif /* (LDCOM_IFRXAPIS_AVAILABLE == STD_ON) */

/* !LINKSTO LdCom.EB.SWS_LDCOM_00042_DummyCbk,1 */
/**
 ** \brief  This function is the dummy callback function of LdCom_TriggerTransmit.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 ** \param[in]  PduInfoPtr      Is unused and will be ignored.
 **
 ** \retval E_NOT_OK            Default return value for dummy callback function
 **
 **/
extern FUNC(Std_ReturnType, LDCOM_CODE) LdCom_DummyCbkTriggerTransmit
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
);
/* !LINKSTO LdCom.EB.SWS_LDCOM_00053_DummyCbk,1 */
/**
 ** \brief  This function is the dummy callback function of LdCom_TxConfirmation.
 ** This function is not a LdCom AUTOSAR SWS public API.
 **
 **/
extern FUNC(void, LDCOM_CODE) LdCom_DummyCbkTxConfirmation
(
  void
);

#define LDCOM_STOP_SEC_CODE
#include <LdCom_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_DUMMYCBK_H */
/*==================[end of file]===========================================*/
