/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) A conversion from a type of the Crypto interface to the IP unit is necessary.
 * 2) The types are identical besides from the names of the members in the struct.
 *
 */

/*==================[inclusions]============================================*/
#include <TSMem.h>                /* memory functions */
#include <SchM_TcpIp.h>           /* SchM API declarations */

#include <TcpIp_guards.h>         /* after foreign/API/Cfg; before internal/private */
#include <TcpIp_IpSec_Int.h>      /* own SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>    /* used SW-unit interface file */
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */


#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)

/*==================[macros]================================================*/

/** \brief TcpIp_RequestIpSecMode() service ID */
#define TCPIP_SID_REQIPSECMOD                                   0xF3U

/** \brief AH fixed fields length */
#define TCPIP_IPSEC_AH_FIXEDHEADER_LEN                            12U

/** \brief Indicates the length of the extended sequence number */
#define TCPIP_IPSEC_ESN_LEN                                        4U

/** \brief salt size for gmac */
#define TCPIP_IPSEC_SALT_SIZE                                      4U

/** \brief initialization vector size for gmac */
#define TCPIP_IPSEC_IV_GMAC_SIZE                                   8U

/** \brief macro returns SPI from AH header */
#define TCPIP_IPSEC_SPI_GET(headerPtr)                              \
  (TCPIP_GET32(headerPtr, 4))

/** \brief macro returns sequence number (lower 32 bits) from AH header */
#define TCPIP_IPSEC_SEQ_GET(headerPtr)                              \
  (TCPIP_GET32(headerPtr, 8))

/** \brief macro returns the length of the AH header */
#define TCPIP_IPSEC_LEN_BYTES_GET(headerPtr)                        \
  (((uint16)headerPtr[1U] + 2U) * 4U)

/** \brief macro returns the next header of the AH header */
#define TCPIP_IPSEC_NEXTHDR_GET(headerPtr)                          \
  (headerPtr[0U])

/** \brief macro sets the AH header */
#define TCPIP_IPSEC_CREATE_AH(pdata, nxtHdr, HdrLen, Spi, SeqNum, icvLen)         \
  do                                                                              \
  {                                                                               \
    (pdata)[0U]= (nxtHdr);                                                        \
    (pdata)[1U]= (HdrLen);                                                        \
    TS_MemBZero(&(pdata)[2U], 2U);                                                \
    TCPIP_SET32((pdata), 4, (Spi));                                               \
    TCPIP_SET32((pdata), 8, (SeqNum));                                            \
    TS_MemBZero(&(pdata)[12U], (icvLen));                                         \
  } while (0U)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief generates MAC for outgoing AH
 *
 * \param[in] AhOffset          offset of authentication header
 * \param[in] ethTxHandlePtr    Structure containing data-link-layer buffer information
 * \param[in] saDataPtr         pointer to SA data
 * \param[in] numberOfFragments number of fragments
 *
 * \retval E_OK             MAC generated correctly
 * \retval E_NOT_OK         MAC generation failed
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpSec_GenerateMac
(
  uint8 AhOffset,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  uint16 numberOfFragments
);

/** \brief Extract ICV and IV from AH header and verifies MAC from incoming AH
 *
 * \param[in] DataPtrIn         data pointer
 * \param[in] DataInLen         length of the data
 * \param[in] saDataPtr         pointer to SA description
 * \param[in] ahOffset          offset of the AH
 * \param[in] ahHeaderLen       length of the AH
 *
 * \retval E_OK             MAC verification passed
 * \retval E_NOT_OK         MAC verification failed
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_MacVerify
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtrIn,
  uint16 DataInLen,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  uint16 ahOffset,
  uint16 ahHeaderLen
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

/** \brief Latest requested state as requested with API call TcpIp_RequestIpSecMode() for all IpSec connections */
VAR(TcpIp_StateType, TCPIP_VAR_CLEARED) TcpIp_IpSec_requestedState[TCPIP_IPSEC_MAX_NUM_CONNECTIONS];
/** \brief Current state of all IpSec conneections */
VAR(TcpIp_StateType, TCPIP_VAR_CLEARED) TcpIp_IpSec_currentState[TCPIP_IPSEC_MAX_NUM_CONNECTIONS];

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

/*==================[external function definitions]=========================*/
#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpSecMode
(
  uint8 conId,
  TcpIp_IPsecStateType reqState
)
{
  Std_ReturnType returnValue = E_NOT_OK;

  DBG_TCPIP_REQUESTIPSECMODE_ENTRY(conId,reqState);

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )

  if(TCPIP_UNINIT)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQIPSECMOD, TCPIP_E_NOTINIT);
  }
  else if(conId >= TCPIP_IPSEC_MAX_NUM_CONNECTIONS)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQIPSECMOD, TCPIP_E_INV_ARG);
  }
  else if((reqState != TCPIP_IPSEC_ACTIVE) && (reqState != TCPIP_IPSEC_INACTIVE))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_REQIPSECMOD, TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    if(conId < TCPIP_IPSEC_NUM_CONNECTIONS)
    {
      /* the external connection index need to be mapped to the internal order
         (internal connection index is order by local address id) */
      uint8 internConId = (TCPIP_CFG_TYPE(uint8,ipSecConnIndexExt2Int,conId));

      /* !LINKSTO TcpIp.Design.IpSec.Atomic.requestedState,1 */
      TS_AtomicAssign8(TcpIp_IpSec_requestedState[internConId], reqState);
      returnValue = E_OK;
    }
  }
#else /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  TS_PARAM_UNUSED(conId);
  TS_PARAM_UNUSED(reqState);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

  DBG_TCPIP_REQUESTIPSECMODE_EXIT(returnValue,conId,reqState);
  return returnValue;
}

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSec_init
(
  void
)
{
  uint8_least i = 0U;

  DBG_TCPIP_IPSEC_INIT_ENTRY();

  for(i=0U; i < TCPIP_IPSEC_NUM_CONNECTIONS; i++)
  {
    TcpIp_IpSec_requestedState[i] = TCPIP_IPSEC_INACTIVE;
    TcpIp_IpSec_currentState[i] = TCPIP_IPSEC_INACTIVE;
  }
  DBG_TCPIP_IPSEC_INIT_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_IpSec_mainFunction
(
  void
)
{
  uint8_least conId = 0U;

  DBG_TCPIP_IPSEC_MAINFUNCTION_ENTRY();

  for(conId = 0U; conId < TCPIP_IPSEC_NUM_CONNECTIONS; conId++)
  {
    TcpIp_IPsecStateType requestedState;

    /* !LINKSTO TcpIp.Design.IpSec.Atomic.requestedState,1 */
    TS_AtomicAssign8(requestedState,TcpIp_IpSec_requestedState[conId]);

    switch(requestedState)
    {
      case TCPIP_IPSEC_ACTIVE:
        if(TcpIp_IpSec_currentState[conId] == TCPIP_IPSEC_INACTIVE)
        {
          TcpIp_IpSecDb_start((uint8) conId);
          TcpIp_IpSec_currentState[conId] = TCPIP_IPSEC_ACTIVE;
        }
        break;
      case TCPIP_IPSEC_INACTIVE:
        if(TcpIp_IpSec_currentState[conId] == TCPIP_IPSEC_ACTIVE)
        {
          TcpIp_IpSecDb_stop((uint8) conId);
          TcpIp_IpSec_currentState[conId] = TCPIP_IPSEC_INACTIVE;
        }
        break;
      /* CHECK: NOPARSE */
      default:
        /* should not happen */
        TCPIP_UNREACHABLE_CODE_ASSERT(TCPIP_INTERNAL_API_ID);
        break;
      /* CHECK: PARSE */
    }
  }

  DBG_TCPIP_IPSEC_MAINFUNCTION_EXIT();
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_ahRxIndication
(
  P2VAR(TcpIp_Uint8PtrType, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) dataLenPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) protocolPtr,
  uint16 ahOffset,
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) ipsecSaIdxPtr
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  const uint16 remainLength = *dataLenPtr - ahOffset;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) ahHeaderPtr = &((*dataPtr)[ahOffset]);

  DBG_TCPIP_IPSEC_AHRXINDICATION_ENTRY(dataPtr,dataLenPtr,protocolPtr,ahOffset,localAddrId,remoteAddrPtr,ipsecSaIdxPtr);

  /* do some length checks */
  if((remainLength >= TCPIP_IPSEC_AH_FIXEDHEADER_LEN) &&
     (TCPIP_IPSEC_LEN_BYTES_GET(ahHeaderPtr) >= TCPIP_IPSEC_AH_FIXEDHEADER_LEN) &&
     (remainLength >= TCPIP_IPSEC_LEN_BYTES_GET(ahHeaderPtr))
    )
  {
    TcpIp_IpSecSaDataType saData;
    uint32 Spi = TCPIP_IPSEC_SPI_GET(ahHeaderPtr);
    uint32 SeqNum = TCPIP_IPSEC_SEQ_GET(ahHeaderPtr);
    uint16 ahHeaderLength = TCPIP_IPSEC_LEN_BYTES_GET(ahHeaderPtr);

    /* retrieve the security association to verify the protection */
    returnValue =
      TcpIp_IpSecDb_getSecurityRxAssociation
        (Spi, SeqNum, localAddrId, remoteAddrPtr, ipsecSaIdxPtr, &saData);

    if(returnValue == E_OK)
    {
      /* check if the mac in the message is correct */
      returnValue =
        TcpIp_IpSec_MacVerify
        (
          *dataPtr,
          *dataLenPtr,
          &saData,
          ahOffset,
          ahHeaderLength
        );

      if(returnValue == E_OK)
      {
        /* return the pointer to the next header */
        *dataPtr = &((*dataPtr)[ahOffset + ahHeaderLength]);
        *dataLenPtr = remainLength - ahHeaderLength;
        *protocolPtr = TCPIP_IPSEC_NEXTHDR_GET(ahHeaderPtr);

#if (TCPIP_SECURITY_MODE == TCPIP_IKEV2)
         /* update seq num */
        returnValue = TcpIp_IpSecDb_UpdateSeqNum(*ipsecSaIdxPtr, saData.SeqNum, SeqCheck);
#endif /*(TCPIP_SECURITY_MODE == TCPIP_IKEV2) */
      }
#if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON)
      else
      {
        TcpIp_IpSecDb_LogError(localAddrId, remoteAddrPtr, TCPIP_IPSEC_INVALID_ICV);
      }
#endif /* #if (TCPIP_IPSECDB_GETANDRESETMEASUREMENTDATA_ENABLE == STD_ON) */
    }
    else
    {
      /* no matching security association found or sequence number invalid */
      returnValue = E_NOT_OK;
    }
  }

  DBG_TCPIP_IPSEC_AHRXINDICATION_EXIT(returnValue,dataPtr,dataLenPtr,protocolPtr,ahOffset,localAddrId,remoteAddrPtr,ipsecSaIdxPtr);

  return returnValue;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_buildAh
(
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  uint16 frgCnt,
  uint8 AhOffset
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  TcpIp_IpSecSaDataType saData;

  DBG_TCPIP_IPSEC_BUILDAH_ENTRY(ipParameterPtr, ethTxHandlePtr, frgCnt, AhOffset);

  returnValue = TcpIp_IpSecDb_getSecurityTxAssociation(ipParameterPtr->ipSecSaCommonIdx, &saData, TRUE);

  if (returnValue == E_OK)
  {
    /* add aH header */
    /* rfc4302: length of AH in 32-bit words (4-byte units), minus "2"  */
    const uint8 headerSize = (uint8)((saData.securityHeadersize)/4U)-2U;
    const uint8 IcvLength = saData.securityHeadersize - TCPIP_IPSEC_AH_FIXEDHEADER_LEN;

    TCPIP_IPSEC_CREATE_AH( ethTxHandlePtr[0U].bufferPtr,
                           ipParameterPtr->protocol,
                           headerSize,
                           saData.spi,
                           saData.SeqNumLow,
                           IcvLength
                         );

    /* change protocol to AH */
    ipParameterPtr->protocol = TCPIP_IP_EXT_HDR_AUTHENTICATION;

    /* csm MAC generate */
    returnValue = TcpIp_IpSec_GenerateMac( AhOffset,
                                           ethTxHandlePtr,
                                           &saData,
                                           frgCnt
                                         );
  }

  DBG_TCPIP_IPSEC_BUILDAH_EXIT(returnValue, ipParameterPtr, ethTxHandlePtr, frgCnt, AhOffset);

  return returnValue;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(Std_ReturnType, TCPIP_CODE ) TcpIp_IpSec_GenerateMac
(
  uint8 AhOffset,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  uint16 numberOfFragments
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  /* offset and generate function if AH is !gmac */
  uint8 macOffset = 0U;
  TcpIp_Crypto_MacGenerateFctPtrType macGenerateFctPtr = &TcpIp_Crypto_Common_MacGenerate;

#if (TCPIP_IPSEC_GMAC_ENABLED == STD_ON)

  uint8 initializationVector[TCPIP_IPSEC_IV_GMAC_SIZE] = {0U};

  if(saDataPtr->useGmac)
  {
    TS_STATIC_ASSERT(TCPIP_IPSEC_IV_GMAC_SIZE >= 8U, GMAC_IV_SIZE_too_small);

    macOffset = TCPIP_IPSEC_IV_GMAC_SIZE;
    macGenerateFctPtr = &TcpIp_Crypto_Gmac_MacGenerate;

    /* Set initialization vector to sequence number */
    COMSTACK_SET32(initializationVector, TCPIP_IPSEC_IV_GMAC_SIZE - 4U, saDataPtr->SeqNumLow);
    COMSTACK_SET32(initializationVector, TCPIP_IPSEC_IV_GMAC_SIZE - 8U, saDataPtr->SeqNumHigh);

    returnValue =
      TcpIp_Crypto_setNonce
       (saDataPtr->keyId, initializationVector, TCPIP_IPSEC_IV_GMAC_SIZE, TCPIP_IPSEC_SALT_SIZE);
  }
  else
  {
    /* nothing to be initialized when its !gmac */
    returnValue = E_OK;
  }

  if(returnValue == E_OK)
#endif  /* (TCPIP_IPSEC_GMAC_ENABLED == STD_ON) */
  {
    /* HMAC or CMAC */
    /* max number of frag TcpIp_PBcfgPtr->ipFragConfig.txFragmentSegmentCount  + 1 for ESN */
    /* Deviation MISRAC2012-1 <+2> */
    P2VAR(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA) dataFragmentPtr =
       (P2VAR(TcpIp_Crypto_FragmentType, AUTOMATIC, TCPIP_APPL_DATA)) ethTxHandlePtr;
    uint8 tempMac[TCPIP_IPSEC_MAX_ICV_LEN] = {0U};
    uint32 macLen = saDataPtr->MacLen;

    /* first fragment includes the header */
    dataFragmentPtr[0U].fragDataPtr = &dataFragmentPtr[0U].fragDataPtr[-1*((sint8)AhOffset)];
    dataFragmentPtr[0U].fragDataLength += AhOffset;

    returnValue = macGenerateFctPtr( saDataPtr->jobId,
                                     dataFragmentPtr,
                                     tempMac,
                                     &macLen,
                                     (uint8) numberOfFragments
                                   );

    /* reset buffer values */
    dataFragmentPtr[0U].fragDataPtr = &dataFragmentPtr[0U].fragDataPtr[AhOffset];
    dataFragmentPtr[0U].fragDataLength -= AhOffset;

    if(returnValue == E_OK)
    {
      /* copy mac */
      /* set ICV */
      TS_MemCpy(&ethTxHandlePtr[0U].bufferPtr[macOffset + TCPIP_IPSEC_AH_FIXEDHEADER_LEN], tempMac, macLen);

#if (TCPIP_IPSEC_GMAC_ENABLED == STD_ON)
      if(saDataPtr->useGmac)
      {
        /* copy IV to transmit buffer */
        TS_MemCpy(&ethTxHandlePtr[0U].bufferPtr[TCPIP_IPSEC_AH_FIXEDHEADER_LEN],
                  initializationVector,
                  TCPIP_IPSEC_IV_GMAC_SIZE);
      }
#endif /* (TCPIP_IPSEC_GMAC_ENABLED == STD_ON) */
    }
  }

  return returnValue;
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_MacVerify
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtrIn,
  uint16 DataInLen,
  P2VAR(TcpIp_IpSecSaDataType, AUTOMATIC, TCPIP_APPL_DATA) saDataPtr,
  uint16 ahOffset,
  uint16 ahHeaderLen
)
{
  Std_ReturnType returnValue = E_NOT_OK;
  TcpIp_Crypto_MacVerifyFctPtrType macVerifyFctPtr;
  const uint8 macLength = saDataPtr->MacLen;
  uint8 authFieldLen;
  uint16 macOffset;

#if (TCPIP_IPSEC_GMAC_ENABLED == STD_ON)
  if(saDataPtr->useGmac)
  {
    authFieldLen = TCPIP_IPSEC_IV_GMAC_SIZE + macLength;
    macOffset = ahOffset + TCPIP_IPSEC_AH_FIXEDHEADER_LEN + TCPIP_IPSEC_IV_GMAC_SIZE;
    macVerifyFctPtr = &TcpIp_Crypto_Gmac_MacVerify;
  }
  else
#endif /* (TCPIP_IPSEC_GMAC_ENABLED == STD_ON) */
  {
    macOffset = ahOffset + TCPIP_IPSEC_AH_FIXEDHEADER_LEN;
    authFieldLen = macLength;
    macVerifyFctPtr = &TcpIp_Crypto_Common_MacVerify;
  }

  /* first check if ah header fits */
  if((DataInLen >= macOffset + macLength) &&
     (ahHeaderLen >= (uint16)macLength + TCPIP_IPSEC_AH_FIXEDHEADER_LEN)
    )
  {
#if (TCPIP_IPSEC_GMAC_ENABLED == STD_ON)
    if(saDataPtr->useGmac)
    {
      P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) initializationVectorPtr =
         &DataPtrIn[ahOffset + TCPIP_IPSEC_AH_FIXEDHEADER_LEN];
      const uint16 initializationVectorSize = TCPIP_IPSEC_IV_GMAC_SIZE;

      /* for gmac the initialization vector must be combined with the salt */
      returnValue =
        TcpIp_Crypto_setNonce
          (saDataPtr->keyId, initializationVectorPtr, initializationVectorSize, TCPIP_IPSEC_SALT_SIZE);
    }
    else
    {
      returnValue = E_OK;
    }

    if(returnValue == E_OK)
#endif /* (TCPIP_IPSEC_GMAC_ENABLED == STD_ON) */
    {
      uint8 TempICV[TCPIP_IPSEC_MAX_ICV_LEN] = {0U};
      const uint16 authFieldOffset = ahOffset + TCPIP_IPSEC_AH_FIXEDHEADER_LEN;
      TcpIp_Crypto_FragmentType dataFragment;
      uint8 numOfDataFrags = 1U;

      /* only copy mac length that will be compared, rest of it must be padding */
      TS_MemCpy(TempICV, &DataPtrIn[macOffset], macLength);

      /* zero-out authentication field */
      TS_MemBZero(&DataPtrIn[authFieldOffset], authFieldLen);

      /* incoming data + ESN need to be verified */
      dataFragment.fragDataPtr = DataPtrIn;
      dataFragment.fragDataLength = DataInLen;

      returnValue = macVerifyFctPtr
      (
        saDataPtr->jobId,
        &dataFragment,
        numOfDataFrags,
        TempICV,
        macLength
      );
    }
  }

  return returnValue;
}

#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)  */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
