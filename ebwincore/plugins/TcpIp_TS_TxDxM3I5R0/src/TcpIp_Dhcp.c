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
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in
 * parentheses.
 *
 * Reason:
 * Parentheses defeat the purpose of a compile-time-if macro: e.g. it could not be used
 * within a parameter list, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/
#include <TSMem.h>              /* memory functions */
#include <SchM_TcpIp.h>         /* SchM API declarations */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Dhcp_Priv.h>    /* own SW-unit private header file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */

/*==================[macros]================================================*/

/* \brief decimal value of the ASCII '.' */
#define TCPIP_DOT_CHARACTER                                            46U


#if ((TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON))
/** \brief Dispatch macro for function \a reachConfirmation for IpV4, IpV6 and \a TCPIP_IP_DUALSTACK */
#define TCPIP_DHCP_GET_STATUS(page)                 \
  TCPIP_DHCP_IF_V4_V6_BOTH(TcpIp_DhcpV4_getStatus,  \
                          TcpIp_DhcpV6_getStatus,   \
                          TcpIp_Dhcp_FctDispatchArray[(page)].TcpIp_Dhcp_GetStatusFctPtr )
#endif /* ((TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) || (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)) */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))

/** \brief Write Dhcp option
 *
 * This functions writes the configurable option.
 *
 * \param[in]  DataLength        length of the option
 * \param[in]  DataPtr           pointer to buffer where data should be copied from
 * \param[in]  optionDataPtr     pointer to the option data
 * \param[in]  optionLengthPtr   pointer to the option length
 * \param[in]  dhcpOptionCfgPtr  pointer to the dhcp option configuration
 * \param[in]  optionIdx         index of configured option
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOptionInternal
(
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionIdx
);

#endif /* ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if  (TCPIP_DHCPGETSTATUS_API == STD_ON)
#if (TCPIP_IP_DUALSTACK == STD_ON)
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC CONST(TcpIp_Dhcp_FctDispatchType,TCPIP_CONST) TcpIp_Dhcp_FctDispatchArray[TCPIP_IP_NUMDOMAINS] =
{
  {  /* BEGIN IPv4 table */
    &TcpIp_DhcpV4_getStatus,
  }, /* END IPv4 table */
  {  /* BEGIN IPv6 table */
    &TcpIp_DhcpV6_getStatus,
  }  /* END IPv6 table */
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>
#endif /* TCPIP_IP_DUALSTACK==STD_ON */
#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>


#if ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON))

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Dhcp_writeOptionFQDN
(
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name_len
)
{
  uint8 currentLen = 0U;
  uint8 labelLen = 0;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) LabelLengthPtr =
    &domain_name[0U];
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DomainPtr =
    &domain_name[1U];

  /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpWriteOption,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  if(DataLength != 0U)
  {
    /* save domain name in canonical wire format, this format represents the
       domain name in a sequence of label length + label name, e.g. eb.com ->
       2,'e','b',3,'c','o','m',0 for more information refer to rfc1035 */
    for(currentLen = 0U; currentLen < DataLength; currentLen++)
    {
      if(DataPtr[currentLen] == TCPIP_DOT_CHARACTER)
      {
        /* replace dot by label length */
        *LabelLengthPtr = labelLen;
        LabelLengthPtr = &DomainPtr[currentLen];
        labelLen = 0U;
      }
      else
      {
        /* copy byte of domain name */
        DomainPtr[currentLen] = DataPtr[currentLen];
        labelLen++;
      }
    }

    /* set length of the last label */
    *LabelLengthPtr = labelLen;
    /* Add the length of the terminator*/
    *domain_name_len = currentLen;
    (*domain_name_len)++;
  }
  else
  {
    (*domain_name_len) = 0U;
  }

  /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpWriteOption,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Dhcp_readOptionFQDN
(
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) domain_name,
  uint16 domain_name_len
)
{
  uint16 copiedlen = 0U;

  /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpReadOption,1 */
  SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  /* if domain name exist copy it to buffer */
  if(domain_name_len != 0U)
  {
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DomainPtr =
      &domain_name[1U];
    uint8 labelLen = domain_name[0U];
    uint8 currentLen;
    uint8 currentLabelLen = 0U;

    copiedlen = domain_name_len - 1U;

    for(currentLen = 0U; currentLen < copiedlen; currentLen++)
    {
      if(currentLabelLen < labelLen)
      {
        DataPtr[currentLen] = DomainPtr[currentLen];
        currentLabelLen++;
      }
      else
      {
        DataPtr[currentLen] = TCPIP_DOT_CHARACTER;
        labelLen = DomainPtr[currentLen];
        currentLabelLen = 0U;
      }
    }
  }

  /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpReadOption,1 */
  SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

  /* set length of the domain name */
  *DataLength = copiedlen;
}


#endif /* ((TCPIP_DHCPV4_FQDN_ENABLED == STD_ON) || (TCPIP_DHCPV6_FQDN_ENABLED == STD_ON)) */

#if ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))


TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_readOption
(
 uint16 optionId,
 P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) DataLength,
 P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
 P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
 P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
 P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
 uint8 optionNum
)
{
 Std_ReturnType result = E_NOT_OK;
 uint8 i;

 for (i=0U; i < optionNum; ++i)
 {
   if(dhcpOptionCfgPtr[i].optionCode == optionId)
   {
     if((optionLengthPtr[i] != 0U) && (*DataLength >= (optionLengthPtr[i] )))
     {
       /* !LINKSTO TcpIp.Design.DhcpV6.EA2.DhcpV6ReadOption,1 */
       /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpReadOption,1 */
       SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

       TS_MemCpy( DataPtr,
                  &optionDataPtr[dhcpOptionCfgPtr[i].optionDataOffset],
                  optionLengthPtr[i]
                );

       *DataLength = optionLengthPtr[i];

       /* !LINKSTO TcpIp.Design.DhcpV6.EA2.DhcpV6ReadOption,1 */
       /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpReadOption,1 */
       SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

       result = E_OK;
     }
     break;
   }
 }

 return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOption
(
  uint16 optionId,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionNum
)
{
  Std_ReturnType result = E_NOT_OK;
  uint8 i;

  for (i=0U; i < optionNum; ++i)
  {
    if(dhcpOptionCfgPtr[i].optionCode == optionId)
    {
      result = TcpIp_Dhcp_writeOptionInternal
                 (DataLength,DataPtr,optionDataPtr,optionLengthPtr,dhcpOptionCfgPtr,i);
      break;
    }
  }

  return result;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOptionRxIndication
(
  uint16 optionId,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionNum
)
{
  Std_ReturnType result = E_NOT_OK;
  uint8 i;

  for (i=0U; i < optionNum; ++i)
  {
    if((dhcpOptionCfgPtr[i].optionCode == optionId) && (!dhcpOptionCfgPtr[i].optionTransmit))
    {
      result = TcpIp_Dhcp_writeOptionInternal
                 (DataLength,DataPtr,optionDataPtr,optionLengthPtr,dhcpOptionCfgPtr,i);
      break;
    }
  }

  return result;
}

#endif /* ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */

#if  (TCPIP_DHCPGETSTATUS_API == STD_ON)

FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpGetStatus
(
  TcpIp_LocalAddrIdType LocalAddressId,
  P2VAR(uint8,AUTOMATIC, TCPIP_APPL_DATA) StatusPtr
)
{
  TcpIp_ReturnType retval = TCPIP_E_NOT_OK;

  DBG_TCPIP_DHCPGETSTATUS_ENTRY(LocalAddressId, StatusPtr);

#if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    /* report Det if TcpIp is not initialized */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPGETSTATUS, TCPIP_E_NOTINIT);
  }
  else if(LocalAddressId >= TcpIp_PBcfgPtr->configLocalAddrNum)
  {
    /* report Det if Local Address exceeds number of configured addresses */
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPGETSTATUS, TCPIP_E_INV_ARG);
  }
  else if(StatusPtr == NULL_PTR)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_DHCPGETSTATUS, TCPIP_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Deviation MISRAC2012-1 */
    retval = TCPIP_DHCP_GET_STATUS(TCPIP_IPADDRM_ADDRDOMAIN_GET(TcpIp_IpAddrM_getLocalAddrInfo(LocalAddressId)))
             (
               LocalAddressId,
               StatusPtr
             );
  }

  DBG_TCPIP_DHCPGETSTATUS_EXIT(retval, LocalAddressId, StatusPtr);

  return retval;
}

#endif /* (TCPIP_DHCPGETSTATUS_API == STD_ON) */


/*==================[internal function definitions]=========================*/

#if ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON))

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Dhcp_writeOptionInternal
(
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  P2VAR (uint8, AUTOMATIC, TCPIP_APPL_DATA) optionDataPtr,
  P2VAR (uint16, AUTOMATIC, TCPIP_APPL_DATA) optionLengthPtr,
  P2CONST(TcpIp_DhcpOptionConfigType,AUTOMATIC,TCPIP_APPL_CONST) dhcpOptionCfgPtr,
  uint8 optionIdx
)
{
  Std_ReturnType result = E_NOT_OK;

  if(DataLength <= dhcpOptionCfgPtr[optionIdx].optionMaxLength)
  {
    /* !LINKSTO TcpIp.Design.DhcpV6.EA2.DhcpV6WriteOption,1 */
    /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpWriteOption,1 */
    /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

    TS_MemCpy(  &optionDataPtr[dhcpOptionCfgPtr[optionIdx].optionDataOffset],
                DataPtr,
                DataLength
              );

    optionLengthPtr[optionIdx] = DataLength;

    /* !LINKSTO TcpIp.Design.DhcpV6.EA2.DhcpV6WriteOption,1 */
    /* !LINKSTO TcpIp.Design.DhcpV4.EA2.DhcpWriteOption,1 */
    /* !LINKSTO TcpIp.Design.DhcpV6.EA2.rxIndication,1 */
    /* !LINKSTO TcpIp.Design.DhcpV4.EA2.rxIndication,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_2();

    result = E_OK;
  }
  return result;
}
#endif /* ((TCPIP_DHCPV6_OPTIONS_ENABLED == STD_ON) || (TCPIP_DHCPV4_OPTIONS_ENABLED == STD_ON)) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>
/*==================[end of file]===========================================*/
