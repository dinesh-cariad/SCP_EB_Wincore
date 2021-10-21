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
#ifndef XCP_CBK_H
#define XCP_CBK_H

/*==================[includes]===============================================*/
#include <Xcp_Cfg.h>

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
#include <XcpOnCan_Cbk.h>     /* AUTOSAR XCP on CAN callback header */
#endif

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
#include <XcpOnFr_Cbk.h>      /* AUTOSAR XCP on Flexray callback header */
#endif

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
#include <XcpOnEth_Cbk.h>     /* AUTOSAR XCP on Ethernet callback header */
#endif

#if (XCP_ON_CDD_ENABLED == STD_ON)
#include <XcpOnCdd_Cbk.h>     /* AUTOSAR XCP on CDD callback header */
#endif

#if ( (XCP_STORE_DAQ_SUPPORTED == STD_ON) || \
      (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) )
#include <NvM.h>
#endif
/*==================[macros]=================================================*/

/*------------------[error codes]--------------------------------------------*/

/*------------------[macros for service IDs]---------------------------------*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

#if ( XCP_CAL_PAG_STORE_SUPPORTED == STD_ON )
/** \brief Callback for STORE_CAL_REQ in SET_REQUEST command
 **
 ** This callback function shall be called by the application after issuing
 ** the SET_REQUEST command (STORE_CAL_REQ bit set) and after the NvM writing
 ** is completed (successful or not).
 **
 ** Scenario:
 ** SET_REQUEST command (STORE_CAL_REQ bit is set) is received by the XCP module.
 ** The XCP module calls the application callout function
 ** Xcp_ApplSetReqStoreCalReq() that stores the calibration data to NV memory.
 ** After the data is stored to NV memory, the application must call the
 ** Xcp_SetReqStoreCalReqCbk() function.
 **
 ** This callback function resets the STORE_CAL_REQ bit
 ** of the Current Session Status parameter in GET_STATUS only if it is called with
 ** the ServiceId input parameter as the ID for NvM_WriteBlock() API, otherwise it does nothing.
 ** If the callback function is called with JobResult input parameter NVM_REQ_OK,
 ** it also sends the EV_STORE_CAL event, otherwise no event is sent.
 **
 ** \param[in]    ServiceId - Unique Service ID of NVRAM manager service
 ** \param[in]    JobResult - Covers the job result of the previous processed single block job
 **
 ** \return Result of the operation.
 ** \retval E_OK - Callback function has been processed successfully
 **
 ** \ProductionError{XCP_E_RESP_CTO_QUEUE_FULL, thrown\, if command response
 ** or event package cannot be sent to the master because internal queue is full.}
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_SetReqStoreCalReqCbk
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
);
#endif /* (XCP_CAL_PAG_STORE_SUPPORTED == STD_ON) */

#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
/** \brief Callback for a single NvM block request
 **
 ** This callback function shall be called by NvM upon the end of a single
 ** block request. The XCP slave will initially call NvM to store the DAQ lists
 ** configuration. Upon the storage end this callback will be requested and XCP
 ** will send a event to the master - EV_STORE_DAQ or EV_CLEAR_DAQ.
 **
 ** \param[in]  ServiceId   Unique Service ID of NVRAM manager service
 ** \param[in]  JobResult   Covers the job result of the previous processed single block job
 ** \return Result of the operation
 ** \retval E_OK     Callback function has been processed successfully
 ** \retval E_NOT_OK Callback function has not been processed successfully
 **
 ** \ProductionError{XCP_E_RESP_CTO_QUEUE_FULL, thrown\, if a command response
 ** or event package cannot be sent to the master because the internal queue is full.}
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_NvmStoreDaqSingleCbk
(
  uint8 ServiceId,
  NvM_RequestResultType JobResult
);
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
/** \brief Callback for reading the associated NvM block
 **
 ** This callback function corresponds to configuration parameter NvMReadRamBlockFromNvCallback
 ** and is triggered by NvM.
 **
 ** */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_ReadRamBlockFromNvMNativeCbk(
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer
);
/** \brief Callback for writing the associated NvM block
 **
 ** This callback function corresponds to configuration parameter NvMWriteRamBlockToNvCallback
 ** and is triggered by NvM.
 **
 ** */
extern FUNC(Std_ReturnType, XCP_CODE) Xcp_WriteRamBlockToNvMNativeCbk(
  P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer
);
#endif

#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCP_CBK_H */
/*==================[end of file]============================================*/
