/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef IPDUM_API_H
#define IPDUM_API_H

/*==================[inclusions]============================================*/
/* !LINKSTO IPDUM.ASR40.IPDUM151_3,1 */
#include <ComStack_Types.h>
#include <IpduM_Types.h>
#include <IpduM_Int.h>
/* !LINKSTO IPDUM.ASR40.IPDUM148_4,1 */
#include <IpduM_Cfg.h>
#include <IpduM_Version.h>
/* !LINKSTO IPDUM.ASR40.IPDUM151_2,1 */
#include <SchM_IpduM.h>

#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_ON)
#include <IpduM_MultiMF.h>
#endif

#if (IPDUM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM.h>         /* Post build configuration manager */
#endif

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
#include <IpduM_IsValidConfig.h>
#endif
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

/** \brief Initializes all module-related global variables.
 **
 ** Initializes all module-related global variables including default values,
 ** default selector field and state of timeout monitors.
 **
 ** \param[in]  Config  Pointer to post build configuration of the IpduM
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, IPDUM_CODE) IpduM_Init
(
  P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_APPL_CONST) Config
);

/** \brief Transmit an I-PDU
 **
 ** This function transmits the data given through \p PduInfoPtr through the
 ** I-PDU given by \p TxPduId.
 **
 ** \param[in]  TxPduId  ID of I-PDU to be transmitted.
 ** \param[in]  PduInfoPtr   A pointer to a structure with I-PDU related data that shall be
 **                          transmitted: data length and pointer to I-SDU buffer
 **
 **
 ** \return Standard Return Code
 ** \retval E_OK The request was accepted by IpduM.
 ** \retval E_NOT_OK The request was not accepted by IpduM, a detailed error
 **                  condition was sent to DET.
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non Reentrant for the same PDU-ID. Reentrant for different PDU-ID}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(Std_ReturnType, IPDUM_CODE) IpduM_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr
);

#if(IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_OFF)
#ifndef RTE_SCHM_SWCBSW_IPDUM_MAINFUNCTIONRX
/** \brief IpduM RX main function
 **
 ** Processes DEFERRED ContainerRxPdus
 **
 ** This function has to be called periodically by a task controlled by the
 ** BSW scheduler.
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionRx (void);
#endif /* RTE_SCHM_SWCBSW_IPDUM_MAINFUNCTIONRX */

#ifndef RTE_SCHM_SWCBSW_IPDUM_MAINFUNCTIONTX
/** \brief IpduM TX main function
 **
 ** Performs the processes of the activities that are not directly initiated by
 ** the calls from PDU-R. This includes at least the TxConfirmation time observation.
 **
 ** This function has to be called periodically by a task controlled by the
 ** BSW scheduler.
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, IPDUM_CODE) IpduM_MainFunctionTx (void);
#endif /* RTE_SCHM_SWCBSW_IPDUM_MAINFUNCTIONTX */
#endif /* (IPDUM_DEDICATED_IPDU_PROCESSING_SUPPORT == STD_OFF) */

#if (IPDUM_VERSION_INFO_API == STD_ON)
/** \brief Return module version information
 **
 ** This function returns the IpduM version information in the memory
 ** area \p versioninfo references.
 **
 ** Preconditions:
 ** - The parameter \p versioninfo may not be a NULL pointer
 **
 ** \param[out] versioninfo Version information
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, IPDUM_CODE) IpduM_GetVersionInfo
(
  CONSTP2VAR(Std_VersionInfoType, AUTOMATIC, IPDUM_APPL_DATA) versioninfo
);
#else
/*
 * IpduM_GetVersionInfo() API function is disabled -
 * provide macro implementation
 */
#if (defined IpduM_GetVersionInfo)
#error IpduM_GetVersionInfo already defined!
#endif /* #if (defined IpduM_GetVersionInfo)*/
#define IpduM_GetVersionInfo(versioninfo)                          \
   do                                                              \
   {                                                               \
      /* get version info of IpduM module */                       \
      (versioninfo)->vendorID = IPDUM_VENDOR_ID;                   \
      (versioninfo)->moduleID = IPDUM_MODULE_ID;                   \
      (versioninfo)->sw_major_version = IPDUM_SW_MAJOR_VERSION;    \
      (versioninfo)->sw_minor_version = IPDUM_SW_MINOR_VERSION;    \
      (versioninfo)->sw_patch_version = IPDUM_SW_PATCH_VERSION;    \
   } while(0)
#endif

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef IPDUM_API_H */
/*==================[end of file]===========================================*/

