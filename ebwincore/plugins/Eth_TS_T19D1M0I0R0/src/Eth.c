/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 *   Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 *   Reason:
 *   The AUTOSAR compiler abstraction requires these definitions in this way. Declarations,
 *   function arguments and function return value cannot be enclosed in parentheses due to C
 *   syntax.
 *
 * MISRAC2012-2) Deviated Rule: 2.7 (advisory)
 *   There should be no unused parameters in functions
 *
 *   Reason:
 *   This functions are mostly controller type specific and are usually called from the common layer
 *   via function pointers. It is expected that they contain the defined parameter footprint.
 *   Depending on the type of the controller some parameters could be left unused.
 *
 */

/*==================[inclusions]============================================*/
#define ETH_NO_CFGCLASSMIX_REQUIRED   /* no PBcfg symbolic ids required */

#include <TSAutosar.h>         /* EB specific standard types */
#include <Eth_Api.h>           /* Module public API */
#include <Eth_Int.h>           /* internal module API */
#include <Eth_Lcfg.h>          /* for signature */

#include <Eth_Trace.h>        /* debug and trace macros; must be last */
/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef ETH_VENDOR_ID /* configuration check */
#error ETH_VENDOR_ID must be defined
#endif

#if (ETH_VENDOR_ID != 1U) /* vendor check */
#error ETH_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef ETH_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error ETH_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETH_AR_RELEASE_MAJOR_VERSION != 4U)
#error ETH_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef ETH_AR_RELEASE_MINOR_VERSION /* configuration check */
#error ETH_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETH_AR_RELEASE_MINOR_VERSION != 1U )
#error ETH_AR_RELEASE_MINOR_VERSION wrong (!= 1U)
#endif

#ifndef ETH_AR_RELEASE_REVISION_VERSION /* configuration check */
#error ETH_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (ETH_AR_RELEASE_REVISION_VERSION != 3U )
#error ETH_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef ETH_SW_MAJOR_VERSION /* configuration check */
#error ETH_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETH_SW_MAJOR_VERSION != 0U)
#error ETH_SW_MAJOR_VERSION wrong (!= 0U)
#endif

#ifndef ETH_SW_MINOR_VERSION /* configuration check */
#error ETH_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETH_SW_MINOR_VERSION < 0U)
#error ETH_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef ETH_SW_PATCH_VERSION /* configuration check */
#error ETH_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (ETH_SW_PATCH_VERSION < 8U)
#error ETH_SW_PATCH_VERSION wrong (< 8U)
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/** \brief Provide tx buffer internal
*
* This function is the internal implementation of the different provide tx buffer API functions.
*
* \param[in] CtrlIdx Controller index.
* \param[in] BufTxIdxInt Tx buffer index internal
* \param[in] Priority Priority of the requested tx buffer (for best effort it is 0U)
* \param[in] BufIdxPtr Pointer to retured Tx buffer index
* \param[in] BufPtr Pinter to the returned Tx buffer
* \param[in] LenBytePtr Pointer to the requested and returned tx buffer length
* \retval BUFREQ_OK: success
* \retval BUFREQ_E_NOT_OK: development error detected
* \retval BUFREQ_E_BUSY: all buffers in use
* \retval BUFREQ_E_OVFL: requested buffer too large
*/
/* Deviation MISRAC2012-1 <START> */
static FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBufferInt
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
);
/* Deviation MISRAC2012-1 <STOP> */

/** \brief Receive processing internal
*
* This function is the internal implementation of the different receive API functions.
*
* \param[in] CtrlIdx Controller index.
* \param[in] FifoIdx Fifo index (is equal to queue rx index
* \param[in] RxStatus Pointer to the returned Rx status
*/
static FUNC(void, ETH_CODE) Eth_ReceiveInt
(
  uint8 CtrlIdx,
  uint8 FifoIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define ETH_START_SEC_VAR_CLEARED_8
#include <Eth_MemMap.h>

/** \brief Stores the state of each Eth controller. */
VAR(Eth_CtrlStateType, ETH_VAR) Eth_CtrlState[ETH_CTRL_NO];

/** \brief Stores the mode of each Eth controller. */
VAR(Eth_ModeType, ETH_VAR) Eth_CtrlMode[ETH_CTRL_NO];

#define ETH_STOP_SEC_VAR_CLEARED_8
#include <Eth_MemMap.h>

#define ETH_START_SEC_VAR_INIT_8
#include <Eth_MemMap.h>

VAR(Eth_StateType, ETH_VAR) Eth_Initialized = ETH_STATE_UNINIT;

#define ETH_STOP_SEC_VAR_INIT_8
#include <Eth_MemMap.h>

#define ETH_START_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>

/* pointer to post-build configuration */
P2CONST(Eth_ConfigType, ETH_VAR, ETH_APPL_CONST) Eth_RootPtr = NULL_PTR;

#define ETH_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <Eth_MemMap.h>
/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

FUNC( Std_ReturnType, ETH_CODE) Eth_IsValidConfig
(
    P2CONST(void,AUTOMATIC,ETH_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(Eth_ConfigType,AUTOMATIC,ETH_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_ETH_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if (ConfigPtr != NULL_PTR)
  {
      /* Check if the configuration fits to the platform */
      if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature))
      {
          /* Validate the post build configuration against the compile time configuration */
          if (ETH_CFG_SIGNATURE == ConfigPtr->CfgSignature)
          {
              /* Validate the post build configuration against the link time configuration */
              if (Eth_LcfgSignature == ConfigPtr->LcfgSignature)
              {
                  /* Validate the Published information in post build configuration against the
                     Published information in compile time configuration*/
                  if(ETH_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
                  {
                      /* Indicate that the configuration is valid */
                      RetVal = E_OK;
                  }
              }
          }
      }
  }

  DBG_ETH_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);

  return RetVal;
}

FUNC(void, ETH_CODE) Eth_Init
(
  P2CONST(Eth_ConfigType, AUTOMATIC, ETH_APPL_CONST) PBCfgPtr
)
{
  P2CONST(Eth_ConfigType,AUTOMATIC,ETH_APPL_CONST) LocalConfigPtr = PBCfgPtr;

  DBG_ETH_INIT_ENTRY(PBCfgPtr);

  /* Save the configuration pointer */
#if (ETH_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager */
  if (LocalConfigPtr == NULL_PTR)
  {
      PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
      if (E_OK == PbcfgM_GetConfig(
            ETH_MODULE_ID,
            ETH_INSTANCE_ID,
            &ModuleConfig))
      {
          LocalConfigPtr = (P2CONST(Eth_ConfigType, AUTOMATIC, ETH_APPL_CONST)) ModuleConfig;
      }
  }
#endif /* ETH_PBCFGM_SUPPORT_ENABLED == STD_OFF */


#if (ETH_DEV_ERROR_DETECT == STD_ON)
  /* check whether parameter Eth_RootPtr is a NULL_PTR */
  if (LocalConfigPtr == NULL_PTR)
  {
    ETH_DET_REPORT_ERROR(ETH_INIT_SVCID, ETH_E_INV_POINTER);
  }
  /* check that configuration pointer is valid */
  else if ( E_OK != Eth_IsValidConfig(LocalConfigPtr))
  {
    ETH_DET_REPORT_ERROR(ETH_INIT_SVCID, ETH_E_INV_POINTER);
  }
  else
#endif
  {
    /* !LINKSTO Eth.ASR41.SWS_Eth_00003, 1 */
    uint8_least CtrlIdx = 0U;
    /* !LINKSTO Eth.ASR41.SWS_Eth_00028, 1 */
    Eth_RootPtr = LocalConfigPtr;
    /* Since there is only one eth controller, no loop is necessary */
    {
      TS_AtomicAssign8( Eth_CtrlMode[CtrlIdx], ETH_MODE_DOWN);
      TS_AtomicAssign8( Eth_CtrlState[CtrlIdx], ETH_CTRL_STATE_UNINIT);
    }
    /* !LINKSTO Eth.ASR41.SWS_Eth_00014, 1 */
    Eth_Arch_Init ( LocalConfigPtr);
    Eth_Initialized = ETH_STATE_INIT;
  }

  DBG_ETH_INIT_EXIT(ConfigPtr);

  return;
}

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422) || (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)
/* Deviation MISRAC2012-2 <+4> */
FUNC(Std_ReturnType, ETH_CODE) Eth_ControllerInit
(
  uint8 CtrlIdx,
  uint8 CfgIdx
)
{
  Std_ReturnType Result =   E_NOT_OK;

  DBG_ETH_CONTROLLERINITASR422_ENTRY(CtrlIdx, CfgIdx);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (Eth_Initialized != ETH_STATE_INIT)
  {
    ETH_DET_REPORT_ERROR(ETH_CONTROLLERINIT_SVCID, ETH_E_NOT_INITIALIZED);
  }
  else if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_CONTROLLERINIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else if (CfgIdx > ETH_MAX_CFGIDX)
  {
    ETH_DET_REPORT_ERROR(ETH_CONTROLLERINIT_SVCID, ETH_E_INV_CONFIG);
  }
  else
#else
  TS_PARAM_UNUSED( CfgIdx );
#endif
  {

      if ( Eth_Arch_ControllerInit ( CtrlIdx) == E_OK )
      {
          TS_AtomicAssign8( Eth_CtrlState[CtrlIdx], ETH_CTRL_STATE_INIT);
          Result = E_OK;
      }
  }

  DBG_ETH_CONTROLLERINITASR422_EXIT( Result, CtrlIdx, CfgIdx);

  return Result;
}
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)||(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)*/
FUNC(Std_ReturnType, ETH_CODE) Eth_ControllerInit
(
  uint8 CtrlIdx
)
{
  Std_ReturnType Result =   E_NOT_OK;

  DBG_ETH_CONTROLLERINITASR430B_ENTRY(CtrlIdx);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (Eth_Initialized != ETH_STATE_INIT)
  {
    ETH_DET_REPORT_ERROR(ETH_CONTROLLERINIT_SVCID, ETH_E_NOT_INITIALIZED);
  }
  else if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_CONTROLLERINIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
#endif
  {
      if ( Eth_Arch_ControllerInit ( CtrlIdx ) == E_OK )
      {
          TS_AtomicAssign8( Eth_CtrlState[CtrlIdx], ETH_CTRL_STATE_INIT);
          Result = E_OK;
      }
  }

  DBG_ETH_CONTROLLERINITASR430B_EXIT(Result,CtrlIdx);

  return Result;
}
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)||(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)*/

FUNC(Std_ReturnType, ETH_CODE) Eth_SetControllerMode
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_SETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlMode);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_SETCONTROLLERMODE_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;
    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_SETCONTROLLERMODE_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
      if ( Eth_Arch_SetControllerMode ( CtrlIdx, CtrlMode ) == E_OK )
      {
        TS_AtomicAssign8( Eth_CtrlMode[CtrlIdx], CtrlMode);
        Result = E_OK;
      }
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_SETCONTROLLERMODE_EXIT(Result,CtrlIdx,CtrlMode);

  return Result;
}

FUNC(Std_ReturnType, ETH_CODE) Eth_GetControllerMode
(
  uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_APPL_DATA) CtrlModePtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_GETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlModePtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_GETCONTROLLERMODE_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_GETCONTROLLERMODE_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (CtrlModePtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_GETCONTROLLERMODE_SVCID, ETH_E_INV_POINTER);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
      TS_AtomicAssign8(*CtrlModePtr, Eth_CtrlMode[CtrlIdx]);
      Result = E_OK;
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_GETCONTROLLERMODE_EXIT(Result,CtrlIdx,CtrlModePtr);

  return Result;
}

FUNC(void, ETH_CODE) Eth_GetPhysAddr
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{
  DBG_ETH_GETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_GETPHYSADDR_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_GETPHYSADDR_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (PhysAddrPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_GETPHYSADDR_SVCID, ETH_E_INV_POINTER);
    }
    else
#endif
    {
        Eth_Arch_GetPhysAddr ( CtrlIdx, PhysAddrPtr );
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_GETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr);

  return;
}

FUNC(void, ETH_CODE) Eth_SetPhysAddr
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{

  DBG_ETH_SETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_SETPHYSADDR_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_SETPHYSADDR_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (PhysAddrPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_SETPHYSADDR_SVCID, ETH_E_INV_POINTER);
    }
    else
#endif
    {
        Eth_Arch_SetPhysAddr ( CtrlIdx, PhysAddrPtr );
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_SETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr);

  return;
}

#if (ETH_UPDATE_PHYS_ADDR_FILTER_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ETH_CODE) Eth_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_UPDATEPHYSADDRFILTER_ENTRY(CtrlIdx,PhysAddrPtr,Action);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_UPDATEPHYSADDRFILTER_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_UPDATEPHYSADDRFILTER_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (PhysAddrPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_UPDATEPHYSADDRFILTER_SVCID, ETH_E_INV_POINTER);
    }
    else if( !(( Action ==  ETH_ADD_TO_FILTER) || ( Action == ETH_REMOVE_FROM_FILTER)) )
    {
      ETH_DET_REPORT_ERROR(ETH_UPDATEPHYSADDRFILTER_SVCID, ETH_E_INV_PARAM);
    }
    else
#endif
    {
      Result = Eth_Arch_UpdatePhysAddrFilter( CtrlIdx, PhysAddrPtr, Action);
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_UPDATEPHYSADDRFILTER_EXIT(Result,CtrlIdx,PhysAddrPtr,Action);

  return Result;
}
#endif /* ETH_UPDATE_PHYS_ADDR_FILTER_SUPPORT == STD_ON */

#if (ETH_ENABLE_MII_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ETH_CODE) Eth_MiiInit
(
  uint8 CtrlIdx
)
{
  Std_ReturnType Result =   E_NOT_OK;

  DBG_ETH_MIIINITASR430B_ENTRY(CtrlIdx);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (Eth_Initialized != ETH_STATE_INIT)
  {
    ETH_DET_REPORT_ERROR(ETH_MIIINIT_SVCID, ETH_E_NOT_INITIALIZED);
  }
  else if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_MIIINIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
#endif
  {
      if ( Eth_Arch_MiiInit ( CtrlIdx ) == E_OK )
      {
          TS_AtomicAssign8( Eth_CtrlState[CtrlIdx], ETH_CTRL_STATE_MII_INIT);
          Result = E_OK;
      }
  }

  DBG_ETH_MIIINITASR430B_EXIT(Result, CtrlIdx);

  return Result;
}

FUNC(Eth_ReturnType, ETH_CODE) Eth_WriteMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  uint16 RegVal
)
{
  Eth_ReturnType Result = ETH_E_NOT_OK;

  DBG_ETH_WRITEMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegVal);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
#if (ETH_VIRT_ROLE != ETH_VIRT_ROLE_ETH_VSWT)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_WRITEMII_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
#endif /* (ETH_VIRT_ROLE != ETH_VIRT_ROLE_ETH_VSWT) */
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if ( (CtrlState != ETH_CTRL_STATE_MII_INIT) && (CtrlState != ETH_CTRL_STATE_INIT) )
    {
      ETH_DET_REPORT_ERROR(ETH_WRITEMII_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else
    {
      CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl = ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);

      if ( (pCtrl->CtrlEnableMii) == FALSE)
      {
        ETH_DET_REPORT_ERROR(ETH_WRITEMII_SVCID, ETH_E_INV_CONFIG);
      }
      else
#endif
      {
        Result = Eth_Arch_WriteMii ( CtrlIdx, TrcvIdx, RegIdx, RegVal );
      }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
    }
  }
#endif /* ETH_DEV_ERROR_DETECT  */

  DBG_ETH_WRITEMII_EXIT(Result,CtrlIdx,TrcvIdx,RegIdx,RegVal);

  return Result;
}

FUNC(Eth_ReturnType, ETH_CODE) Eth_ReadMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) RegValPtr
)
{
  Eth_ReturnType Result = ETH_E_NOT_OK;

  DBG_ETH_READMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegVal);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
#if (ETH_VIRT_ROLE != ETH_VIRT_ROLE_ETH_VSWT)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_READMII_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
#endif /* (ETH_VIRT_ROLE != ETH_VIRT_ROLE_ETH_VSWT) */
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if ( (CtrlState != ETH_CTRL_STATE_MII_INIT) && (CtrlState != ETH_CTRL_STATE_INIT) )
    {
      ETH_DET_REPORT_ERROR(ETH_READMII_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (RegValPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_READMII_SVCID, ETH_E_INV_POINTER);
    }
    else
    {
      CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl = ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);

      if ( (pCtrl->CtrlEnableMii) == FALSE)
      {
        ETH_DET_REPORT_ERROR(ETH_READMII_SVCID, ETH_E_INV_CONFIG);
      }
      else
#endif
      {
        Result = Eth_Arch_ReadMii ( CtrlIdx, TrcvIdx, RegIdx, RegValPtr );
      }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
    }
  }
#endif /* ETH_DEV_ERROR_DETECT  */

  DBG_ETH_READMII_EXIT(Result,CtrlIdx,TrcvIdx,RegIdx,RegVal);

  return Result;
}
#endif /* ETH_ENABLE_MII_SUPPORT == STD_ON */

FUNC(void, ETH_CODE) Eth_GetCounterState
(
  uint8 CtrlIdx,
  uint16 CtrlOffs,
  P2VAR(uint32, AUTOMATIC, ETH_APPL_DATA) CtrlValPtr
)
{
  DBG_ETH_GETCOUNTERSTATE_ENTRY(CtrlIdx,CtrlOffs,CtrlValPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_GETCOUNTERSTATE_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_GETCOUNTERSTATE_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (CtrlValPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_GETCOUNTERSTATE_SVCID, ETH_E_INV_POINTER);
    }
    else
#endif
    {
      Eth_Arch_GetCounterState ( CtrlIdx, CtrlOffs, CtrlValPtr );
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_GETCOUNTERSTATE_EXIT(CtrlIdx,CtrlOffs,CtrlValPtr);

  return;
}
/* Deviation MISRAC2012-2 <+4> */
FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetCounterValues
(
  uint8 CtrlIdx,
  P2VAR(Eth_CounterType, AUTOMATIC, EBSTUBS_APPL_DATA) CounterPtr
)
{
  Eth_ReturnType Result = E_NOT_OK;

  DBG_ETH_GETCOUNTERVALUES_ENTRY(CtrlIdx,CounterPtr);

  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(CounterPtr);

  /* This API function is not supported. */
  ETH_DET_REPORT_ERROR( ETH_GETCOUNTERVALUES_SVCID, ETH_E_API_NOT_SUPPORTED);

  DBG_ETH_GETCOUNTERVALUES_EXIT(Result, CtrlIdx,CounterPtr);

  return Result;
}
/* Deviation MISRAC2012-2 <+4> */
FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetRxStats
(
  uint8 CtrlIdx,
  P2VAR(Eth_RxStatsType, AUTOMATIC, EBSTUBS_APPL_DATA) RxStats
)
{
  Eth_ReturnType Result = E_NOT_OK;

  DBG_ETH_GETRXSTATS_ENTRY(CtrlIdx,RxStats);

  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(RxStats);

  /* This API function is not supported. */
  ETH_DET_REPORT_ERROR( ETH_GETRXSTATS_SVCID, ETH_E_API_NOT_SUPPORTED);

  DBG_ETH_GETRXSTATS_EXIT(Result,CtrlIdx,RxStats);

  return Result;
}
/* Deviation MISRAC2012-2 <+4> */
FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetTxStats
(
  uint8 CtrlIdx,
  P2VAR(Eth_TxStatsType, AUTOMATIC, EBSTUBS_APPL_DATA) TxStats
)
{
  Eth_ReturnType Result = E_NOT_OK;

  DBG_ETH_GETTXSTATS_ENTRY(CtrlIdx,TxStats);

  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(TxStats);

  /* This API function is not supported. */
  ETH_DET_REPORT_ERROR( ETH_GETTXSTATS_SVCID, ETH_E_API_NOT_SUPPORTED);

  DBG_ETH_GETTXSTATS_EXIT(Result,CtrlIdx,TxStats);

  return Result;
}
/* Deviation MISRAC2012-2 <+4> */
FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetTxErrorCounterValues
(
  uint8 CtrlIdx,
  P2VAR(Eth_TxErrorCounterValuesType, AUTOMATIC, EBSTUBS_APPL_DATA) TxErrorCounterValues
)
{
  Eth_ReturnType Result = E_NOT_OK;

  DBG_ETH_GETTXERRORCOUNTERVALUES_ENTRY(CtrlIdx,TxErrorCounterValues);

  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(TxErrorCounterValues);

  /* This API function is not supported. */
  ETH_DET_REPORT_ERROR( ETH_GETTXERRORCOUNTERVALUES_SVCID, ETH_E_API_NOT_SUPPORTED);

  DBG_ETH_GETTXERRORCOUNTERVALUES_EXIT(Result,CtrlIdx,TxErrorCounterValues);

  return Result;
}


#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)
/* Deviation MISRAC2012-1 <START> */
FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBuffer
(
  uint8 CtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
)
/* Deviation MISRAC2012-1 <STOP> */
{
  BufReq_ReturnType BufReq;

  DBG_ETH_PROVIDETXBUFFERASR422_ENTRY(CtrlIdx,BufIdxPtr,BufPtr,LenBytePtr);

  BufReq = Eth_ProvideTxBufferInt( CtrlIdx, 0U, BufIdxPtr, BufPtr, LenBytePtr );

  DBG_ETH_PROVIDETXBUFFERASR422_EXIT(BufReq,CtrlIdx,BufIdxPtr,BufPtr,LenBytePtr);

  return BufReq;
}
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/
/* Deviation MISRAC2012-1 <START> */
FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBuffer
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
)
/* Deviation MISRAC2012-1 <STOP> */
{
  BufReq_ReturnType BufReq;

  DBG_ETH_PROVIDETXBUFFER_ENTRY(CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr);

  BufReq = Eth_ProvideTxBufferInt( CtrlIdx, Priority, BufIdxPtr, BufPtr, LenBytePtr );

  DBG_ETH_PROVIDETXBUFFER_EXIT(BufReq,CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr);

  return BufReq;
}
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/

FUNC(Std_ReturnType, ETH_CODE) Eth_Transmit
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_TRANSMIT_ENTRY(CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_TRANSMIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_TRANSMIT_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else
    {
      CONSTP2CONST(Eth_CtrlType, AUTOMATIC, ETH_APPL_CONST) pCtrl = ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);
      Eth_ModeType CtrlMode;
      TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

      if (BufIdx >= pCtrl->CtrlTxBufTotal)
      {
        ETH_DET_REPORT_ERROR(ETH_TRANSMIT_SVCID, ETH_E_INV_PARAM);
      }
      else if (PhysAddrPtr == NULL_PTR)
      {
        ETH_DET_REPORT_ERROR(ETH_TRANSMIT_SVCID, ETH_E_INV_POINTER);
      }
      else if (CtrlMode != ETH_MODE_ACTIVE)
      {
        ETH_DET_REPORT_ERROR(ETH_TRANSMIT_SVCID, ETH_E_INV_MODE);
      }
      else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
      {
          Result = Eth_Arch_Transmit
              (
                  CtrlIdx,
                  BufIdx,
                  FrameType,
                  TxConfirmation,
                  LenByte,
                  PhysAddrPtr
              );
      }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
    }
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_TRANSMIT_EXIT(Result,CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr);

  return Result;
}
/* !LINKSTO Eth.EB.EthRetransmissionSupport, 1 */
#if (ETH_SUPPORT_RETRANSMIT == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) Eth_Retransmit
(
  uint8 CtrlIdx,
  uint8 OrigCtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  Eth_FrameType FrameType,
  uint16 LenByte,
  CONSTP2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETH_APPL_DATA) RetransmitInfoPtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_RETRANSMIT_ENTRY
                       (CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if( CtrlIdx >= ETH_CTRL_NO)
  {
    /* !LINKSTO Eth.EB.Eth_Retransmit_DET_0x01, 1 */
    ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else if( OrigCtrlIdx >= ETH_CTRL_NO)
  {
    /* !LINKSTO Eth.EB.Eth_Retransmit_DET_0x01, 1 */
    ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if( CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else
    {
      Eth_ModeType CtrlMode;
      Eth_ModeType OrigCtrlMode;

      TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);
      TS_AtomicAssign8( OrigCtrlMode, Eth_CtrlMode[OrigCtrlIdx]);

      if( NULL_PTR == BufIdxPtr)
      {
        ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_PARAM);
      }
      else if( NULL_PTR == RetransmitInfoPtr)
      {
        ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_POINTER);
      }
      else if( ( CtrlMode != ETH_MODE_ACTIVE) || ( OrigCtrlMode != ETH_MODE_ACTIVE))
      {
        ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_MODE);
      }
      else
      {
        boolean ContRetrans = FALSE;

        if( (*BufIdxPtr) != ETH_BUFIDXTYPE_INVALID )
        {
          /* API is called inside EthIf_TxConformation. */
          CONSTP2CONST(Eth_CtrlType, AUTOMATIC, ETH_APPL_CONST) pCtrl =
          ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);

          if ( (*BufIdxPtr) >= pCtrl->CtrlTxBufTotal)
          {
            ETH_DET_REPORT_ERROR( ETH_RETRANSMIT_SVCID, ETH_E_INV_PARAM);
          }
          else
          {
            ContRetrans = TRUE;
          }
        }
        else
        {
          ContRetrans = TRUE;
        }
        if( ContRetrans)
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
        {
          Result = Eth_Arch_Retransmit
               ( CtrlIdx, OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, LenByte, RetransmitInfoPtr);
        }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
      }
    }
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_RETRANSMIT_EXIT
                (Result,CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr);
  return Result;
}
#endif /* (ETH_SUPPORT_RETRANSMIT == STD_ON) */
/* !LINKSTO Eth.EB.EthBufferListSupport, 1 */
#if (ETH_BUFFER_LIST_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ETH_CODE) Eth_TransmitBufList
(
  const uint8 CtrlIdx,
  const uint8 Priority,
  CONSTP2CONST(Eth_BufListType, TYPEDEF, ETH_APPL_DATA) BufListPtr,
  const uint8 BufListNo,
  CONSTP2CONST(uint8, TYPEDEF, ETH_APPL_DATA) UserPtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETH_TRANSMITBUFLIST_ENTRY(CtrlIdx,Priority,BufListPtr,BufListNo,UserPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if( CtrlIdx >= ETH_CTRL_NO)
  {
    /* !LINKSTO Eth.EB.Eth_TransmitBufList_DET_0x01, 1 */
    ETH_DET_REPORT_ERROR( ETH_TRANSMITBUFLIST_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;
    Eth_ModeType CtrlMode;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

    if( CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR( ETH_TRANSMITBUFLIST_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if( NULL_PTR == BufListPtr)
    {
      ETH_DET_REPORT_ERROR( ETH_TRANSMITBUFLIST_SVCID, ETH_E_INV_PARAM);
    }
    else if( 0U == BufListNo)
    {
      ETH_DET_REPORT_ERROR( ETH_TRANSMITBUFLIST_SVCID, ETH_E_INV_PARAM);
    }
    else if( CtrlMode != ETH_MODE_ACTIVE)
    {
      ETH_DET_REPORT_ERROR( ETH_TRANSMITBUFLIST_SVCID, ETH_E_INV_MODE);
    }
#if (ETH_QOS_PRIORITY_NO < 256U)
    else if (Priority >= ETH_QOS_PRIORITY_NO)
    {
      ETH_DET_REPORT_ERROR(ETH_TRANSMITBUFLIST_SVCID, ETH_E_INV_PARAM);
    }
#endif /* (ETH_QOS_PRIORITY_NO < 256U) */
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
      Result = Eth_Arch_TransmitBufList( CtrlIdx, Priority, BufListPtr, BufListNo, UserPtr);
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_TRANSMITBUFLIST_EXIT(Result,CtrlIdx,Priority,BufListPtr,BufListNo,UserPtr);
  return Result;
}
#endif /* (ETH_BUFFER_LIST_SUPPORT == STD_ON) */

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)
FUNC(void, ETH_CODE) Eth_Receive
(
  uint8 CtrlIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
)
{
  DBG_ETH_RECEIVEASR422_ENTRY(CtrlIdx,RxStatus);

  Eth_ReceiveInt( CtrlIdx, 0U, RxStatus);

  DBG_ETH_RECEIVEASR422_EXIT(CtrlIdx,RxStatus);

  return;
}
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/
extern FUNC(void, ETH_CODE) Eth_Receive
(
  uint8 CtrlIdx,
  uint8 FifoIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
)
{
  DBG_ETH_RECEIVEASR430_ENTRY(CtrlIdx,FifoIdx,RxStatus);

  Eth_ReceiveInt( CtrlIdx, FifoIdx, RxStatus);

  DBG_ETH_RECEIVEASR430_EXIT(CtrlIdx,FifoIdx,RxStatus);

  return;
}
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/

FUNC(void, ETH_CODE) Eth_TxConfirmation
(
  uint8 CtrlIdx
)
{
  DBG_ETH_TXCONFIRMATION_ENTRY(CtrlIdx);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_TXCONFIRMATION_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;
    Eth_ModeType CtrlMode;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_TXCONFIRMATION_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (CtrlMode != ETH_MODE_ACTIVE)
    {
      ETH_DET_REPORT_ERROR(ETH_TXCONFIRMATION_SVCID, ETH_E_INV_MODE);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
#if (ETH_ENABLE_TX_INTERRUPT_SUPPORT == STD_ON)
      /* get pointer to associated eth controller object */
      CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl = ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);

      if( (pCtrl->CtrlEnableTxInterrupt) == FALSE)
#endif /* (ETH_ENABLE_TX_INTERRUPT_SUPPORT == STD_ON) */
      {
        /* Only in polling mode TxConfirmation processing is done */
        Eth_Arch_TxConfirmation(CtrlIdx, FALSE);
      }
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */

  DBG_ETH_TXCONFIRMATION_EXIT(CtrlIdx);

  return;
}


#if (ETH_GET_VERSION_INFO == STD_ON)
FUNC(void, ETH_CODE) Eth_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETH_APPL_DATA) VersionInfoPtr
)
{
  DBG_ETH_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfoPtr == NULL_PTR)
  {
    ETH_DET_REPORT_ERROR(ETH_GETVERSIONINFO_SVCID, ETH_E_INV_POINTER);
  }
  else
#endif
  {

      VersionInfoPtr->vendorID         = ETH_VENDOR_ID;
      VersionInfoPtr->moduleID         = ETH_MODULE_ID;
      VersionInfoPtr->sw_major_version = ETH_SW_MAJOR_VERSION;
      VersionInfoPtr->sw_minor_version = ETH_SW_MINOR_VERSION;
      VersionInfoPtr->sw_patch_version = ETH_SW_PATCH_VERSION;

  }

  DBG_ETH_GETVERSIONINFO_EXIT(VersionInfoPtr);

  return;
}
#endif


#if (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON)
FUNC(void, ETH_CODE) Eth_SetRxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 FifoIdx,
  const Eth_ModeType Mode
)
{
  DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,FifoIdx,Mode);

#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;
    Eth_ModeType CtrlMode;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if(CtrlMode != ETH_MODE_ACTIVE)
    {
      ETH_DET_REPORT_ERROR(ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_MODE);
    }
    else if( (Mode != ETH_MODE_DOWN) && (Mode != ETH_MODE_ACTIVE))
    {
      ETH_DET_REPORT_ERROR(ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_MODE);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
      CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl=ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);
#if (ETH_DEV_ERROR_DETECT == STD_ON)
      if(FifoIdx >= pCtrl->QueRxNo)
      {
        ETH_DET_REPORT_ERROR(ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_PARAM);
      }
      else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
      {
        const uint8 QueRxIdx = pCtrl->OffsetQueRxIdx + FifoIdx;

        Eth_Arch_SetRxInterruptUsercalloutMode( CtrlIdx, QueRxIdx, Mode);
      }
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
  DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,FifoIdx,Mode);
}
#endif /* (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON) */

#if (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON)
FUNC(void, ETH_CODE) Eth_SetTxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 FifoIdx,
  const Eth_ModeType Mode
)
{
  DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,FifoIdx,Mode);
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;
    Eth_ModeType CtrlMode;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if(CtrlMode != ETH_MODE_ACTIVE)
    {
      ETH_DET_REPORT_ERROR(ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_MODE);
    }
    else if( (Mode != ETH_MODE_DOWN) && (Mode != ETH_MODE_ACTIVE))
    {
      ETH_DET_REPORT_ERROR(ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_MODE);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
      CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl=ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);
#if (ETH_DEV_ERROR_DETECT == STD_ON)
      if(FifoIdx >= pCtrl->QueTxBufNo)
      {
        ETH_DET_REPORT_ERROR(ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID, ETH_E_INV_PARAM);
      }
      else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
      {
        const uint8 QueTxBufIdx = pCtrl->OffsetQueTxBufIdx + FifoIdx;

        Eth_Arch_SetTxInterruptUsercalloutMode( CtrlIdx, QueTxBufIdx, Mode);
      }
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
  DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,FifoIdx,Mode);
}
#endif /* (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON) */

#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>

/*==================[internal function definitions]=========================*/

/* Deviation MISRAC2012-1 <START> */
static FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBufferInt
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
)
/* Deviation MISRAC2012-1 <STOP> */
{
  BufReq_ReturnType Result = BUFREQ_E_NOT_OK;
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (CtrlIdx >= ETH_CTRL_NO)
  {
    ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_INV_CTRL_IDX);
  }
  else
  {
    Eth_CtrlStateType CtrlState;

    TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
    if (CtrlState != ETH_CTRL_STATE_INIT)
    {
      ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_NOT_INITIALIZED);
    }
    else if (BufIdxPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_INV_POINTER);
    }
    else if (BufPtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_INV_POINTER);
    }
    else if (LenBytePtr == NULL_PTR)
    {
      ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_INV_POINTER);
    }
    else if (*LenBytePtr == 0U)
    {
      ETH_DET_REPORT_ERROR(ETH_PROVIDETXBUFFER_SVCID, ETH_E_INV_PARAM);
    }
    else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
    {
        Result = Eth_Arch_ProvideTxBuffer ( CtrlIdx, Priority, BufIdxPtr, BufPtr, LenBytePtr );
    }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
  return Result;
}
/* Deviation MISRAC2012-2 <+4> */
static FUNC(void, ETH_CODE) Eth_ReceiveInt
(
  uint8 CtrlIdx,
  uint8 FifoIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
)
{
#if (ETH_DEV_ERROR_DETECT == STD_ON)
  if (RxStatus == NULL_PTR)
  {
    ETH_DET_REPORT_ERROR(ETH_RECEIVE_SVCID, ETH_E_INV_POINTER);
  }
  else
#endif
  {
    *RxStatus = ETH_NOT_RECEIVED;  /* default */
#if (ETH_DEV_ERROR_DETECT == STD_ON)
    if (CtrlIdx >= ETH_CTRL_NO)
    {
      ETH_DET_REPORT_ERROR(ETH_RECEIVE_SVCID, ETH_E_INV_CTRL_IDX);
    }
    else
    {
      Eth_CtrlStateType CtrlState;
      Eth_ModeType CtrlMode;

      TS_AtomicAssign8( CtrlState, Eth_CtrlState[CtrlIdx]);
      TS_AtomicAssign8( CtrlMode, Eth_CtrlMode[CtrlIdx]);

      if (CtrlState != ETH_CTRL_STATE_INIT)
      {
        ETH_DET_REPORT_ERROR(ETH_RECEIVE_SVCID, ETH_E_NOT_INITIALIZED);
      }
      else if (CtrlMode != ETH_MODE_ACTIVE)
      {
        ETH_DET_REPORT_ERROR(ETH_RECEIVE_SVCID, ETH_E_INV_MODE);
      }
      else
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
      {
        CONSTP2CONST(Eth_CtrlType,AUTOMATIC,ETH_APPL_CONST) pCtrl=ETH_CFG_GET_PTR_TO_CTRL(CtrlIdx);
#if (ETH_DEV_ERROR_DETECT == STD_ON)
#if (ETH_SUPPORT_API != ETH_SUPPORT_API_ASR422)
        if (FifoIdx >= pCtrl->QueRxNo)
        {
          ETH_DET_REPORT_ERROR(ETH_RECEIVE_SVCID, ETH_E_INV_PARAM);
        }
        else
#endif /*(ETH_SUPPORT_API != ETH_SUPPORT_API_ASR422)*/
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
        {
#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)
          /* Retreive the best effort queue Rx buffer index */
          const uint8 QueRxIdx = pCtrl->QueRxBeIdx;
          TS_PARAM_UNUSED(FifoIdx);
#else  /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/
          const uint8 QueRxIdx = pCtrl->OffsetQueRxIdx + FifoIdx;
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/

          Eth_Arch_Receive( CtrlIdx, QueRxIdx, RxStatus, FALSE );
        }
      }
#if (ETH_DEV_ERROR_DETECT == STD_ON)
    }
#endif /* (ETH_DEV_ERROR_DETECT == STD_ON) */
  }
}

/*==================[end of file]===========================================*/


