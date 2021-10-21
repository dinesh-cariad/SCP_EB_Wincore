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
 *
 * MISRAC2012-2) Deviated Rule: 8.9 (Advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that functions are mapped in SEC_CODE memory sections.
 * Objects at block scope require a different memory mapping, e.g. to a SEC_VAR section,
 * which leads to nested memory sections, which is not supported by some compilers.
 *
 * MISRAC2012-8) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object and an integer type.
 *
 * Reason:
 * Alignment checks can only be perfomed by casting the pointer to integer.
 * Losing upper bits is not a problem, so different sizes won't hurt.
 * We never convert the integers back to pointers.
 *
 * MISRAC2012-9) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer
 * to a different object type.
 *
 * Reason:
 * Aligning for better memory subsystem performance can only be done by casting
 * the pointer to different alignment after the alignment checks.
 *
 */



/*==================[inclusions]============================================*/
#ifndef TCPIP_NO_CFGCLASSMIX_REQUIRED
#define TCPIP_NO_CFGCLASSMIX_REQUIRED
#endif

#include <EthIf.h>              /* EthIf API declarations */

#include <TcpIp_Cfg.h>          /* defines TCPIP_PBCFGM_SUPPORT_ENABLED */
#if (TCPIP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>           /* Post build configuration manager API */
#endif /* TCPIP_PBCFGM_SUPPORT_ENABLED */

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Int.h>          /* module internal headerfile */
#include <TcpIp_Socket_Int.h>   /* used SW-unit interface file */
#include <TcpIp_IpV4_Int.h>     /* used SW-unit interface file */
#include <TcpIp_IpV6_Int.h>     /* used SW-unit interface file */
#include <TcpIp_IpAddrM_Int.h>  /* used SW-unit interface file */
#include <TcpIp_Tcp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_Udp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_Memory_Int.h>   /* used SW-unit interface file */
#include <TcpIp_DhcpV4_Int.h>   /* used SW-unit interface file */
#include <TcpIp_DhcpV6_Int.h>   /* used SW-unit interface file */
#include <TcpIp_Ndp_Int.h>      /* used SW-unit interface file */
#include <TcpIp_IpSecDb_Int.h>  /* used SW-unit interface file */
#include <TcpIp_IpSec_Int.h>    /* used SW-unit interface file */

#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */
#include <SchM_TcpIp.h>         /* SchM API declarations */


/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef TCPIP_VENDOR_ID /* configuration check */
#error TCPIP_VENDOR_ID must be defined
#endif

#if (TCPIP_VENDOR_ID != 1U) /* vendor check */
#error TCPIP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef TCPIP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error TCPIP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (TCPIP_AR_RELEASE_MAJOR_VERSION != 4U)
#error TCPIP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef TCPIP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error TCPIP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (TCPIP_AR_RELEASE_MINOR_VERSION != 3U )
#error TCPIP_AR_RELEASE_MINOR_VERSION wrong (!= 3U)
#endif

#ifndef TCPIP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error TCPIP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (TCPIP_AR_RELEASE_REVISION_VERSION != 0U )
#error TCPIP_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef TCPIP_SW_MAJOR_VERSION /* configuration check */
#error TCPIP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (TCPIP_SW_MAJOR_VERSION != 3U)
#error TCPIP_SW_MAJOR_VERSION wrong (!= 3U)
#endif

#ifndef TCPIP_SW_MINOR_VERSION /* configuration check */
#error TCPIP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (TCPIP_SW_MINOR_VERSION < 5U)
#error TCPIP_SW_MINOR_VERSION wrong (< 5U)
#endif

#ifndef TCPIP_SW_PATCH_VERSION /* configuration check */
#error TCPIP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (TCPIP_SW_PATCH_VERSION < 13U)
#error TCPIP_SW_PATCH_VERSION wrong (< 13U)
#endif

/*==================[macros]================================================*/

/** \brief SID of service TcpIp_GetVersionInfo() */
#define TCPIP_SID_GETVERSIONINFO 0x02U

/** \brief SID of service TcpIp_GetAndResetMeasurementData() */
#define TCPIP_SID_GETANDRESETMEASUREMENTDATA 0x45U

/** \brief Helper macro for efficient 64bit checksum calculation */
/* Deviation MISRAC2012-8 */
#define TCPIP_SHIFTCHECKSUMBYTE(data) \
  ( ((uint32) (data)) <<              \
    (uint8)( ((uint8)1u & (uint8) (TS_IF_BE_LE((uint8) ~(uint8), (uint8)) ((uint32)(&(data))))) << 3u) )


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#if (TCPIP_VERSION_INFO_API == STD_ON)

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC CONST(Std_VersionInfoType, TCPIP_CONST) TcpIp_VersionInfo =
{
   TCPIP_VENDOR_ID,
   TCPIP_MODULE_ID,
   TCPIP_SW_MAJOR_VERSION,
   TCPIP_SW_MINOR_VERSION,
   TCPIP_SW_PATCH_VERSION
};

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* TCPIP_VERSION_INFO_API == STD_ON */

/*==================[external data]=========================================*/

#define TCPIP_START_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

/** \brief TcpIp init state */
VAR(uint8, TCPIP_VAR) TcpIp_InitStatus = STD_OFF;

#define TCPIP_STOP_SEC_VAR_INIT_8
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Post-Build pointer */
P2CONST(TcpIp_ConfigType, TCPIP_VAR_CLEARED, TCPIP_APPL_CONST) TcpIp_PBcfgPtr;


#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

/** \brief Prescaler value to implement different timer periods. */
STATIC VAR(uint16, TCPIP_VAR_CLEARED) TcpIp_TimerTick;

#define TCPIP_STOP_SEC_VAR_CLEARED_16
#include <TcpIp_MemMap.h>

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/** \brief Variable to generate random numbers  */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint32, TCPIP_VAR) TcpIp_RndSeed = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON) */

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

FUNC( Std_ReturnType, TCPIP_CODE) TcpIp_IsValidConfig
(
  P2CONST(void,AUTOMATIC,TCPIP_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(TcpIp_ConfigType,AUTOMATIC,TCPIP_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_TCPIP_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
    {
      /* Validate the post build configuration against the compile time configuration */
      if(TCPIP_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if(TcpIp_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the Published information in post build configuration against the
             Published information in compile time configuration*/
          if(TCPIP_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
              /* Indicate that the configuration is valid */
              RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_TCPIP_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);

  return RetVal;
}


FUNC(void, TCPIP_CODE) TcpIp_Init
(
  P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST) ConfigPtr
)
{
  P2CONST(TcpIp_ConfigType,AUTOMATIC,TCPIP_APPL_CONST) LocalConfigPtr = ConfigPtr;

  DBG_TCPIP_INIT_ENTRY(ConfigPtr);

  /* Save the configuration pointer */
#if (TCPIP_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager */
  if(LocalConfigPtr == NULL_PTR)
  {
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      if(E_OK == PbcfgM_GetConfig(
            TCPIP_MODULE_ID,
            TCPIP_INSTANCE_ID,
            &ModuleConfig))
      {
          LocalConfigPtr = (P2CONST(TcpIp_ConfigType, AUTOMATIC, TCPIP_APPL_CONST)) ModuleConfig;
      }
  }
#endif /* TCPIP_PBCFGM_SUPPORT_ENABLED == STD_OFF */

/* check if development error detection is enabled */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)

  /* check whether parameter FrTp_RootPtr is a NULL_PTR */
  if(LocalConfigPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    TCPIP_REPORT_ERROR(TCPIP_SID_INIT,TCPIP_EB_E_PARAM_POINTER);
  }
  /* check that configuration pointer is valid */
  else if( E_OK != TcpIp_IsValidConfig(LocalConfigPtr))
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_INIT,TCPIP_E_INV_ARG);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT */
  {

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)
    /* re-initialize pseudo-random number generator */
    TcpIp_RndSeed = 0U;
#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON) */

    /* store PostBuild config pointer */
    TcpIp_PBcfgPtr = LocalConfigPtr;

    /* Reset MainFunction prescaler */
    TcpIp_TimerTick = 0U;

    /* initialize IpAddrM data */
    TcpIp_IpAddrM_init();

    /* initialize Socket data */
    TcpIp_Socket_init();

#if (TCPIP_IPV4_ENABLED == STD_ON)
    /* initialize Ipv4 data */
    TcpIp_IpV4_init();
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

#if (TCPIP_IPV6_ENABLED == STD_ON)
    /* initialize Ndp data */
    TcpIp_Ndp_init();
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#if (TCPIP_TCP_ENABLED == STD_ON)
    /* initialize Tcp data */
    TcpIp_Tcp_init();
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
#if defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON)
    /* initialize Memory data */
    TcpIp_Memory_init();
#endif /* defined(TCPIP_MEMORY_NUM_TOTAL_BLOCKS) || (TCPIP_NVM_STORING_ENABLED == STD_ON) */

#if (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON)
    TcpIp_Ip_init();
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON) */

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
    TcpIp_IpSecDb_init();
    TcpIp_IpSec_init();
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

    /* TcpIp stack finished initialization */
    TS_AtomicAssign8(TcpIp_InitStatus,STD_ON);
  }

  DBG_TCPIP_INIT_EXIT(ConfigPtr);

} /* eof - TcpIp_Init */


FUNC(void, TCPIP_CODE) TcpIp_MainFunction(void)
{
  DBG_TCPIP_MAINFUNCTION_ENTRY();
  if( !TCPIP_UNINIT )
  {
#if (TCPIP_IPV4_ENABLED == STD_ON) || (TCPIP_IPV6_ENABLED == STD_ON)
#if (TCPIP_IPV4_ENABLED == STD_ON)
    /* call TcpIp_ArpTimer() so that old cache entries can timeout */
    TcpIp_IpV4_mainFunction(TcpIp_TimerTick);
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) */

#if (TCPIP_IPV6_ENABLED == STD_ON)
    TcpIp_Ndp_mainFunction(TcpIp_TimerTick);
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

    TcpIp_Ip_mainFunction();
#endif /* (TCPIP_IPV4_ENABLED == STD_ON) || (TCPIP_IPV6_ENABLED == STD_ON) */

    TcpIp_IpAddrM_mainFunction();

    TcpIp_Socket_mainFunction();

    TcpIp_Socket_mainFunctionTx();

#if (TCPIP_TCP_ENABLED == STD_ON)
    TcpIp_Tcp_mainFunction();
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */

    /* increment counter */
    TcpIp_TimerTick++;
    if(TcpIp_TimerTick >= TCPIP_MAINFUNCTION_PRESCALER_1S)
    {
      TcpIp_TimerTick = 0U;
    }

#if (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON)
    TcpIp_DhcpV4_mainFunction(TcpIp_TimerTick);
#endif /* (TCPIP_DHCPV4_CLIENT_ENABLED == STD_ON) */

#if (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON)
    TcpIp_DhcpV6_mainFunction(TcpIp_TimerTick);
#endif /* (TCPIP_DHCPV6_CLIENT_ENABLED == STD_ON) */
  }

#if (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  TcpIp_IpSec_mainFunction();
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

  DBG_TCPIP_MAINFUNCTION_EXIT();
} /* eof - TcpIp_MainFunction */

#if (TCPIP_MAINFUNCTIONTX_ENABLED == STD_ON)
FUNC(void, TCPIP_CODE) TcpIp_MainFunctionTx(void)
{
  DBG_TCPIP_MAINFUNCTIONTX_ENTRY();
  if( !TCPIP_UNINIT )
  {
    TcpIp_Socket_mainFunctionTx();
  }
  DBG_TCPIP_MAINFUNCTIONTX_EXIT();
} /* eof - TcpIp_MainFunctionTx */
#endif /* (TCPIP_MAINFUNCTIONTX_ENABLED == STD_ON) */


#if (TCPIP_VERSION_INFO_API == STD_ON)
FUNC(void, TCPIP_CODE) TcpIp_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIP_APPL_DATA) versioninfo
)
{
  DBG_TCPIP_GETVERSIONINFO_ENTRY(versioninfo);

  #if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  if( versioninfo == NULL_PTR )
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETVERSIONINFO, TCPIP_EB_E_PARAM_POINTER);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT == STD_ON */
  {

    *versioninfo = TcpIp_VersionInfo;

  }
  DBG_TCPIP_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* TCPIP_VERSION_INFO_API == STD_ON */

#if (TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON)
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetAndResetMeasurementData
(
  TcpIp_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, TCPIP_APPL_DATA) MeasurementDataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_TCPIP_GETANDRESETMEASUREMENTDATA_ENTRY(
                                              MeasurementIdx,
                                              MeasurementResetNeeded,
                                              MeasurementDataPtr
                                            );

#if(TCPIP_DEV_ERROR_DETECT == STD_ON)
  if(TCPIP_UNINIT)
  {
    TCPIP_REPORT_ERROR(TCPIP_SID_GETANDRESETMEASUREMENTDATA, TCPIP_E_NOTINIT);
  }
  else
#endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* !LINKSTO TcpIp.Design.None.EA6.GetAndResetMeasurementData,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();

    switch(MeasurementIdx)
    {
#if (TCPIP_TCP_ENABLED == STD_ON)
      case TCPIP_MEAS_DROP_TCP:
      {
        TcpIp_Tcp_GetAndResetInvalidSocketCount(MeasurementResetNeeded, MeasurementDataPtr);
        RetVal = E_OK;
        break;
      }
#endif /* TCPIP_TCP_ENABLED == STD_ON */
#if (TCPIP_UDP_ENABLED == STD_ON)
      case TCPIP_MEAS_DROP_UDP:
      {
        TcpIp_Udp_GetAndResetInvalidSocketCount(MeasurementResetNeeded, MeasurementDataPtr);
        RetVal = E_OK;
        break;
      }
#endif /* TCPIP_UDP_ENABLED == STD_ON */
#if (TCPIP_IPV4_ENABLED == STD_ON)
      case TCPIP_MEAS_DROP_IPV4:
      case TCPIP_MEAS_REPLACED_ARP:
      case TCPIP_MEAS_DROP_ARP:
      {
        TcpIp_IpV4_GetAndResetCounter(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
        RetVal = E_OK;
        break;
      }
#endif /* TCPIP_IPV4_ENABLED == STD_ON */
#if (TCPIP_IPV6_ENABLED == STD_ON)
      case TCPIP_MEAS_DROP_IPV6:
      case TCPIP_MEAS_DROP_ICMPV6:
        TcpIp_IpV6_GetAndResetCounter(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
        RetVal = E_OK;
        break;
#endif /* TCPIP_IPV6_ENABLED == STD_ON */
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
      case TCPIP_MEAS_DROP_INVALID_POLICY:
      case TCPIP_MEAS_PASS_VALID_POLICY:
      {
        TcpIp_IpSecDb_GetAndResetPolicyCount(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
        RetVal = E_OK;
        break;
      }
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
      case TCPIP_MEAS_ALL:
      {
        if(MeasurementResetNeeded)
        {
#if (TCPIP_TCP_ENABLED == STD_ON)
          TcpIp_Tcp_GetAndResetInvalidSocketCount(TRUE, NULL_PTR);
#endif /* TCPIP_TCP_ENABLED == STD_ON */
#if (TCPIP_UDP_ENABLED == STD_ON)
          TcpIp_Udp_GetAndResetInvalidSocketCount(TRUE, NULL_PTR);
#endif /* TCPIP_UDP_ENABLED == STD_ON */
#if (TCPIP_IPV4_ENABLED == STD_ON)
          TcpIp_IpV4_GetAndResetCounter(MeasurementIdx, TRUE, NULL_PTR);
#endif /* TCPIP_IPV4_ENABLED == STD_ON */
#if (TCPIP_IPV6_ENABLED == STD_ON)
          TcpIp_IpV6_GetAndResetCounter(MeasurementIdx, TRUE, NULL_PTR);
#endif /* TCPIP_IPV6_ENABLED == STD_ON */
#if (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL)
          TcpIp_IpSecDb_GetAndResetPolicyCount(MeasurementIdx, TRUE, NULL_PTR);
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_FIREWALL) */
        }
        RetVal = E_OK;
        break;
      }
      default:
      {
        /* measurement index not supported */
        break;
      }
    }

    /* !LINKSTO TcpIp.Design.None.EA6.GetAndResetMeasurementData,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_6();
  }

  DBG_TCPIP_GETANDRESETMEASUREMENTDATA_EXIT(
                                             RetVal,
                                             MeasurementIdx,
                                             MeasurementResetNeeded,
                                             MeasurementDataPtr
                                           );

  return RetVal;
}
#endif /* TCPIP_GETANDRESETMEASUREMENTDATA_API == STD_ON */

/*==================[internal function definitions]=========================*/

#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_GetRandomValue (void)
{

  TcpIp_RndSeed = (1103515245U * TcpIp_RndSeed) + 12345U;

  return TcpIp_RndSeed;
}
#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON)*/

#if (TCPIP_GET_RANDOM_RANGE_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_GetRandomRange
(
  uint16 range
)
{
  uint16 ret = 0U;

  ret = (uint16)((((TcpIp_GetRandomValue() & 0xFFFFU) + 1U) * range) >> 16U);

  return ret;
}
#endif /* (TCPIP_GET_RANDOM_RANGE_ENABLED == STD_ON)*/

TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_CalcSum16Bit
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 initValue,
  uint16 length
)
{
#if (defined HAS_64BIT_TYPES)  /* 64 bit architecture / types / memory bus */
  typedef uint64 A_Type;
  typedef uint32 A2_Type; /* half an accumulator */
  uint64 TcpIp_accu= 0u;  /* 48 bit accumulator, so we retain the overflow of the 32bit adds - high part is only carry bits so < 16bits */
#else                          /* 32 bit architecture / types / memory bus */
  typedef uint32 A_Type;
  typedef uint16 A2_Type; /* half an accumulator */
                          /* local alias to w32, using the TcpIp namespace to trigger define guard,... */
  #define TcpIp_accu w32  /* w32 is already used for the peeling iterations and initialized with initValue */
#endif /* (defined HAS_64BIT_TYPES) */
  const uint16 maxAlignment= sizeof(A_Type);
  const uint8 halfBits= sizeof(A2_Type)*8u;

  /* do we need to swap result bytes for LE ^ odd start address? */
  /* Deviation MISRAC2012-8 */
  uint8 shift= (((uint8)1u) & (uint8)(TS_IF_BE_LE((uint8),(uint8) ~(uint8)) ((uint32)(&dataPtr[0u])))) << 3u;
  /* 32 bit accumulator for 16-bit values - 64kB -> 32k carries max, so it can't overflow */
  uint32 w32= ((uint32)initValue) << shift; /* put the initValue in the right place for its alignment! */
  uint16_least index= 0u;                   /* MISRA -> must rely on compiler to do strength reduction */

  /* peel till it's aligned */
  if (!TCPIP_ISALIGNED(&dataPtr[index],maxAlignment) && (length > 0u))
  {
    /* Deviation MISRAC2012-8 */
    uint16 len1= maxAlignment - (((uint16)(usize)&dataPtr[index]) & (maxAlignment-1u)); /* len1 > 0 */
    uint16 fatbool= TS_FATBOOL(uint16, length < len1);
    uint16 len2= TS_SELECT(uint16, fatbool, length,len1);

    /* now peel len2 iterations - len2 > 0 since length & len2 were both > 0 */
    length-= len2;
    do {
      /* shift by 8 or 0 bits depending on alignment vs LE/BE */
      w32 += TCPIP_SHIFTCHECKSUMBYTE(dataPtr[index]);
      ++index;
      --len2;
    } while (len2 > 0);
  }
  /* if (length!=0u)  -- skip remaining loops! */

  /* only aligned starts or length==0 comes here (but length=0 should've been jumping to the end of the loop (jump2jump opt) */

  /* if 64bit is possible: read uint64, sum 32bit halves - LE/BE is don't care (associativity) & cyclical carry */
  /*            otherwise: read uint32, sum 16bit halves - LE/BE is don't care (associativity) & cyclical carry */
  if (length >= maxAlignment)  /* use the guarding-if from the compiler's loop-rotation to speed up short cases */
  {
    do {
      /* Deviation MISRAC2012-9 */
      const A_Type read= *((P2CONST(A_Type, AUTOMATIC, TCPIP_APPL_DATA))(&dataPtr[index]));
      TcpIp_accu += ((A_Type)((A2_Type)(read >> halfBits))) + ((A2_Type)read);
      index += maxAlignment;
      length -= maxAlignment;
    } while(length >= maxAlignment);

    #if (defined HAS_64BIT_TYPES)  /* if we used a 64bit accu, fold the 2 16bit SIMD accumulators + 16bit carry parts into 32bit */
    {
      /* mixed strategy for 32bit architectures with costly 64 bit ops:
         reduce to 32bit registers and avoid all 32bit overflows */
      uint32 hi32= (uint32)(TcpIp_accu >> 32u); /* usually just takes the hi register (half) */
      uint32 lo32= (uint32) TcpIp_accu;      /* usually just takes the lo register (half) */
      /* the rest of the computation is in 32 bit registers, without 32bit overflows:
         w32 is 24 bits max (initValue16 << 8)
         hi32 only contains the carry bits of 16k uint32 adds, so < 14 bits, so the high 18 bits are zero -> no narrowing
         Only the 2 lo values are full 16 bits and may produce a carry into the high part which holds the upper byte of initValue at most */
      w32+= hi32 + (lo32 >> 16u)  + ((uint16)lo32);
    }
    #else  /* (defined HAS_64BIT_TYPES) */
      #undef TcpIp_accu
    #endif /* (defined HAS_64BIT_TYPES) */
  }
  /* now peel till we're done */
  if (length > 0u)
  {
    do {
      /* shift by 8 or 0 bits depending on alignment vs LE/BE */
      w32 += TCPIP_SHIFTCHECKSUMBYTE(dataPtr[index]);
      ++index;
      --length;
    } while(length > 0u);
  }
  {
    uint16 w16;
    w32=          (w32 >> 16u)  + ((uint16)w32);
    w16= ((uint16)(w32 >> 16u)) + ((uint16)w32);
    /* now swap the bytes for LE (HTON/NTOH), leave them for BE (& the other way round, if the address was odd) */
    if (shift != 0u)
    {
      w16= COMSTACK_BYTE_MIRROR16(w16); /* __ROR(w16, shift), if there was a portable primitive */
    }
    return w16;
  }
}


TS_MOD_PRIV_DEFN FUNC(uint16, TCPIP_CODE) TcpIp_CalcChecksum16Bit_withInitValue
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 initValue,
  uint16 length
)
{
  const uint16 sumWord = TcpIp_CalcSum16Bit(dataPtr,initValue,length);
  return (uint16)(((uint16)(~sumWord))&0xFFFFU);
}

TS_MOD_PRIV_DEFN FUNC( void, TCPIP_CODE ) TcpIp_EthAddrCpy
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) addrDestPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addrSrcPtr
)
{
  (addrDestPtr)[0U] = (addrSrcPtr)[0U];
  (addrDestPtr)[1U] = (addrSrcPtr)[1U];
  (addrDestPtr)[2U] = (addrSrcPtr)[2U];
  (addrDestPtr)[3U] = (addrSrcPtr)[3U];
  (addrDestPtr)[4U] = (addrSrcPtr)[4U];
  (addrDestPtr)[5U] = (addrSrcPtr)[5U];
}

TS_MOD_PRIV_DEFN FUNC( boolean, TCPIP_CODE ) TcpIp_EthAddrCmp
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr1Ptr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) addr2Ptr
)
{
  boolean result; /* avaluation result */

  /* compare each byte - order assumes the fastest difference detection
   * check low bytes of company unique identifier and OUI first */
  if((((addr1Ptr)[5U]) != ((addr2Ptr)[5U])) ||
    ((((addr1Ptr)[2U]) != ((addr2Ptr)[2U])) ||
    ((((addr1Ptr)[4U]) != ((addr2Ptr)[4U])) ||
    ((((addr1Ptr)[3U]) != ((addr2Ptr)[3U])) ||
    ((((addr1Ptr)[1U]) != ((addr2Ptr)[1U])) ||
    ((((addr1Ptr)[0U]) != ((addr2Ptr)[0U]))))))))
  {
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }
  return result;
}
#if (TCPIP_GET_RANDOM_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_InitRandomGenerator
(
  uint8 ctrlIdx
)
{
  if(TcpIp_RndSeed == 0U)
  {
    uint8 ethAddr[TCPIP_ETH_ADDR_LENGTH];
    EthIf_GetPhysAddr((TCPIP_CFG(ctrl,ctrlIdx)).ethIfCtrlIdx, ethAddr);

    TcpIp_RndSeed = TCPIP_GET32(ethAddr, 2);
  }
}
#endif /* (TCPIP_GET_RANDOM_ENABLED == STD_ON)  */

#if (TCPIP_IPV6_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckMacNotMulticast
(
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) LinkAddr
)
{
  Std_ReturnType retVal;

  if(TCPIP_MAC_IS_MULTICAST(LinkAddr))
  {
    retVal = E_NOT_OK;
  }
  else if(TCPIP_MAC_IS_IPV6_MULTICAST(LinkAddr))
  {
    retVal = E_NOT_OK;
  }
  /* check if link layer address is broadcast */
  else if(TCPIP_MAC_IS_BROADCAST(LinkAddr))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
  }
  return retVal;
}
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */


#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[end of file]===========================================*/
