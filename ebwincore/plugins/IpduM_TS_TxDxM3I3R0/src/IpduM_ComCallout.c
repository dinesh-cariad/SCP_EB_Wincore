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
/*
 * MISRA-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only
 * appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in
 * SEC_CODE memory sections. Objects at block scope require a different
 * memory mapping, e.g. to a SEC_VAR section, which leads to nested memory
 * sections, which is not supported by some compilers.
 */

/*==================[inclusions]============================================*/
#include <IpduM_Trace.h>
#include <ComStack_Types.h>
#include <TSCompiler.h>

#include <IpduM_Int.h>
#include <IpduM_ComCallout.h>

#if (IPDUM_DEV_ERROR_DETECT==STD_ON)
#include <Det.h>
#endif

#if (IPDUM_SERVICE_MESSAGES==STD_ON)
/* !LINKSTO IPDUM.ASR40.IPDUM148_6,1 */
#include <Com.h>
#endif

#if (IPDUM_SERVICE_MESSAGES==STD_ON)
/*==================[macros]================================================*/
#if (IPDUM_NETWORK_BIG_ENDIAN==STD_ON)

#if (IPDUM_RX_CALLOUT_LENGTH==1U)
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  ((src)[(pos)])
#elif (IPDUM_RX_CALLOUT_LENGTH==2U)
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  ((IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)])) << 8U) | \
                                (src)[(pos)+1U])
#else
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  ((IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)]))    << 24U) | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+1U])) << 16U) | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+2U])) << 8U)  | \
   (src)[(pos)+3U])
#endif

#else /* (IPDUM_NETWORK_BIG_ENDIAN==STD_OFF) */

#if (IPDUM_RX_CALLOUT_LENGTH==1U)
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  ((src)[(pos)])
#elif (IPDUM_RX_CALLOUT_LENGTH==2U)
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  (((src)[(pos)]) | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+1U])) << 8U))
#else
#if (defined IPDUM_GET_MESSAGEID)
#error IPDUM_GET_MESSAGEID already defined!
#endif /* #if (defined IPDUM_GET_MESSAGEID)*/
#define IPDUM_GET_MESSAGEID(src,pos) \
  ((src)[(pos)] | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+1U])) << 8U)  | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+2U])) << 16U) | \
   (IpduM_ServiceMessageIdType)(((IpduM_ServiceMessageIdType)((src)[(pos)+3U])) << 24U))
#endif

#endif

/*==================[type definitions]======================================*/

#if (IPDUM_RX_CALLOUT_LENGTH==1)
typedef uint8 IpduM_ServiceMessageIdType;
#elif (IPDUM_RX_CALLOUT_LENGTH==2)
typedef uint16 IpduM_ServiceMessageIdType;
#else
typedef uint32 IpduM_ServiceMessageIdType;
#endif

typedef struct
{
#if ((IPDUM_RX_CALLOUT_LENGTH==1) || (IPDUM_RX_CALLOUT_LENGTH==2))
  PduIdType ComTxPduId;
  IpduM_ServiceMessageIdType ServiceMessageId;
#else
  IpduM_ServiceMessageIdType ServiceMessageId;
  PduIdType ComTxPduId;
#endif
} IpduM_MapInfoType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define IPDUM_START_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

/* Deviation MISRAC2012-1 */
IPDUM_STATIC CONST(IpduM_MapInfoType, IPDUM_CONST) IpduM_MapInfo[IPDUM_MAP_INFO_LENGTH] = IPDUM_MAP_INFO_CFG;

#define IPDUM_STOP_SEC_CONST_UNSPECIFIED
#include <IpduM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define IPDUM_START_SEC_CODE
#include <IpduM_MemMap.h>

FUNC(boolean, IPDUM_CODE) IpduM_ProcessRequestPdu
(
  PduIdType PdumRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) RxRequestPduInfoPtr
)
{
  IpduM_ServiceMessageIdType ServiceMessageId;
  uint16 i;

  DBG_IPDUM_PROCESSREQUESTPDU_ENTRY(PdumRxPduId,RxRequestPduInfoPtr);

  TS_PARAM_UNUSED(PdumRxPduId);

  /* Extract service message */
  ServiceMessageId = (IpduM_ServiceMessageIdType)(IPDUM_GET_MESSAGEID(RxRequestPduInfoPtr->SduDataPtr,
                               IPDUM_RX_CALLOUT_BYTEPOS));

  /* Search for Com PduId */
  i=0;
  while (i<(uint16)(IPDUM_MAP_INFO_LENGTH))
  {
    if (IpduM_MapInfo[i].ServiceMessageId==ServiceMessageId)
  {
    /* Call Com */
      Com_TriggerIPDUSend(IpduM_MapInfo[i].ComTxPduId);

      break;
  }
    i++;
}

  DBG_IPDUM_PROCESSREQUESTPDU_EXIT(FALSE,PdumRxPduId,RxRequestPduInfoPtr);

  /* Return always FALSE to avoid further processing by Com */
  return FALSE;
}

#define IPDUM_STOP_SEC_CODE
#include <IpduM_MemMap.h>

#endif /* #if (IPDUM_SERVICE_MESSAGES==STD_OFF) */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

/*==================[end of file]===========================================*/

