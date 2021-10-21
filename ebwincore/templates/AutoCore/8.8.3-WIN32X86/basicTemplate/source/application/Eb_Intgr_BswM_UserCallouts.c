/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>                  /* AUTOSAR standard types        */
#include <TSAutosar.h>                  /* EB specific standard types    */
#include <Eb_Intgr_BswM_UserCallouts.h> /* Callout function declarations */


#include <Platforms_Modules.h>          /* platform-specific config pointer
                                           names */
#include <Base_Modules.h>               /* Module enable defines and
                                           standard config pointer names */
#include <Eb_Intgr_BswM_Modules.h>      /* Init function call defines    */
#include <Eb_Intgr_Det.h>               /* Det definitions */

#if defined(BASE_NVM_ENABLED)
#if (BASE_NVM_ENABLED==STD_ON)
#ifndef NVM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define NVM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <NvM_DataIntern.h>             /* contains defines for service ids */
#include <Eb_Intgr_NvM_Include.h>       /* prototype for callback */
#endif
#endif

#if defined(BASE_BSWM_ENABLED)
#if (BASE_BSWM_ENABLED==STD_ON)
#ifndef BSWM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define BSWM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <BswM.h>
#endif
#endif

#if defined(BASE_CAN_ENABLED)
#if (BASE_CAN_ENABLED==STD_ON)
#ifndef CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CAN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_CAN_HEADER
#endif
#endif

#if defined(BASE_CANIF_ENABLED)
#if (BASE_CANIF_ENABLED==STD_ON)
#ifndef CANIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CANIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <CanIf.h>
#endif
#endif

#if defined(BASE_CANNM_ENABLED)
#if (BASE_CANNM_ENABLED==STD_ON)
#ifndef CANNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CANNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <CanNm.h>
#endif
#endif

#if defined(BASE_CANSM_ENABLED)
#if (BASE_CANSM_ENABLED==STD_ON)
#ifndef CANSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CANSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <CanSM.h>
#endif
#endif

#if defined(BASE_CANTP_ENABLED)
#if (BASE_CANTP_ENABLED==STD_ON)
#ifndef CANTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CANTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <CanTp.h>
#endif
#endif

#if defined(BASE_CANTRCV_ENABLED)
#if (BASE_CANTRCV_ENABLED==STD_ON)
#ifndef CANTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define CANTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <CanTrcv.h>
#endif
#endif

#if defined(BASE_COM_ENABLED)
#if (BASE_COM_ENABLED==STD_ON)
#ifndef COM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define COM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Com.h>
#endif
#endif

#if defined(BASE_COMM_ENABLED)
#if (BASE_COMM_ENABLED==STD_ON)
#ifndef COMM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define COMM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <ComM.h>
#include <ComM_EcuMBswM.h>
#endif
#endif

#if defined(BASE_DCM_ENABLED)
#if (BASE_DCM_ENABLED==STD_ON)
#ifndef DCM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define DCM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Dcm.h>
#endif
#endif

#if defined(BASE_DEM_ENABLED)
#if (BASE_DEM_ENABLED==STD_ON)
#ifndef DEM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define DEM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Dem.h>
#endif
#endif

#if defined(BASE_DET_ENABLED)
#if (BASE_DET_ENABLED==STD_ON)
#ifndef DET_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define DET_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Det.h>
#endif
#endif

#if defined(BASE_EA_ENABLED)
#if (BASE_EA_ENABLED==STD_ON)
#ifndef EA_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define EA_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Ea.h>
#endif
#endif

#if defined(BASE_ECUM_ENABLED)
#if (BASE_ECUM_ENABLED==STD_ON)
#ifndef ECUM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ECUM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <EcuM.h>
#endif
#endif

#if defined(BASE_EEP_ENABLED)
#if (BASE_EEP_ENABLED==STD_ON)
#ifndef EEP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define EEP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Eep.h>
#endif
#endif

#if defined(BASE_ETH_ENABLED)
#if (BASE_ETH_ENABLED==STD_ON)
#ifndef ETH_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ETH_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_ETH_HEADER
#endif
#endif

#if defined(BASE_ETHRTP_ENABLED)
#if (BASE_ETHRTP_ENABLED==STD_ON)
#ifndef ETHRTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ETHRTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <EthRtp.h>
#endif
#endif

#if defined(BASE_ETHIF_ENABLED)
#if (BASE_ETHIF_ENABLED==STD_ON)
#ifndef ETHIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ETHIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <EthIf.h>
#endif
#endif

#if defined(BASE_ETHSM_ENABLED)
#if (BASE_ETHSM_ENABLED==STD_ON)
#ifndef ETHSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ETHSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <EthSM.h>
#endif
#endif

#if defined(BASE_ETHTRCV_ENABLED)
#if (BASE_ETHTRCV_ENABLED==STD_ON)
#ifndef ETHTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define ETHTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <EthTrcv.h>
#endif
#endif

#if defined(BASE_FEE_ENABLED)
#if (BASE_FEE_ENABLED==STD_ON)
#ifndef FEE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FEE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_FEE_HEADER
#endif
#endif

#if defined(BASE_FIM_ENABLED)
#if (BASE_FIM_ENABLED==STD_ON)
#ifndef FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FiM.h>
#endif
#endif

#if defined(BASE_FLS_ENABLED)
#if (BASE_FLS_ENABLED==STD_ON)
#ifndef FLS_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FLS_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_FLS_HEADER
#endif
#endif

#if defined(BASE_FR_ENABLED)
#if (BASE_FR_ENABLED==STD_ON)
#ifndef FR_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FR_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_FR_HEADER
#endif
#endif

#if defined(BASE_FRIF_ENABLED)
#if (BASE_FRIF_ENABLED==STD_ON)
#ifndef FRIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FRIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FrIf.h>
#endif
#endif

#if defined(BASE_FRNM_ENABLED)
#if (BASE_FRNM_ENABLED==STD_ON)
#ifndef FRNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FRNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FrNm.h>
#endif
#endif

#if defined(BASE_FRSM_ENABLED)
#if (BASE_FRSM_ENABLED==STD_ON)
#ifndef FRSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FRSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FrSM.h>
#endif
#endif

#if defined(BASE_FRTP_ENABLED)
#if (BASE_FRTP_ENABLED==STD_ON)
#ifndef FRTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FRTP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FrTp.h>
#endif
#endif

#if defined(BASE_FRTRCV_ENABLED)
#if (BASE_FRTRCV_ENABLED==STD_ON)
#ifndef FRTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define FRTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <FrTrcv.h>
#endif
#endif

#if defined(BASE_IPDUM_ENABLED)
#if (BASE_IPDUM_ENABLED==STD_ON)
#ifndef IPDUM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define IPDUM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <IpduM.h>
#endif
#endif

#if defined(BASE_LIN_ENABLED)
#if (BASE_LIN_ENABLED==STD_ON)
#ifndef LIN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define LIN_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_LIN_HEADER
#endif
#endif

#if defined(BASE_LINIF_ENABLED)
#if (BASE_LINIF_ENABLED==STD_ON)
#ifndef LINIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define LINIF_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <LinIf.h>
#endif
#endif

#if defined(BASE_LINSM_ENABLED)
#if (BASE_LINSM_ENABLED==STD_ON)
#ifndef LINSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define LINSM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <LinSM.h>
#endif
#endif

#if defined(BASE_LINTRCV_ENABLED)
#if (BASE_LINTRCV_ENABLED==STD_ON)
#ifndef LINTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define LINTRCV_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <LinTrcv.h>
#endif
#endif

#if defined(BASE_NM_ENABLED)
#if (BASE_NM_ENABLED==STD_ON)
#ifndef NM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define NM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Nm.h>
#endif
#endif

#if defined(BASE_NVM_ENABLED)
#if (BASE_NVM_ENABLED==STD_ON)
#ifndef NVM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define NVM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <NvM.h>
#endif
#else
/* define macro here for access without check if defined */
#define BASE_NVM_ENABLED    STD_OFF
#endif

#if defined(BASE_PDUR_ENABLED)
#if (BASE_PDUR_ENABLED==STD_ON)
#ifndef PDUR_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define PDUR_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <PduR.h>
#endif
#endif

#if defined(BASE_RAMTST_ENABLED)
#if (BASE_RAMTST_ENABLED==STD_ON)
#ifndef RAMTST_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define RAMTST_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <RamTst.h>
#endif
#endif

#if defined(BASE_RTE_ENABLED)
#if (BASE_RTE_ENABLED==STD_ON)
#ifndef RTE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define RTE_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Rte_Main.h>
#endif
#endif

#if defined(BASE_SOAD_ENABLED)
#if (BASE_SOAD_ENABLED==STD_ON)
#ifndef SOAD_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define SOAD_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <SoAd.h>
#endif
#endif

#if defined(BASE_SD_ENABLED)
#if (BASE_SD_ENABLED==STD_ON)
#ifndef SD_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define SD_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Sd.h>
#endif
#endif

#if defined(BASE_SPI_ENABLED)
#if (BASE_SPI_ENABLED==STD_ON)
#ifndef SPI_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define SPI_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include BASE_SPI_HEADER
#endif
#endif

#if defined(BASE_TCPIP_ENABLED)
#if (BASE_TCPIP_ENABLED==STD_ON)
#ifndef TCPIP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define TCPIP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <TcpIp.h>
#endif
#endif

#if defined(BASE_UDPNM_ENABLED)
#if (BASE_UDPNM_ENABLED==STD_ON)
#ifndef UDPNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define UDPNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <UdpNm.h>
#endif
#endif

#if defined(BASE_XCP_ENABLED)
#if (BASE_XCP_ENABLED==STD_ON)
#ifndef XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#define XCP_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
#endif
#include <Xcp.h>
#endif
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define BSWM_START_SEC_CODE
#include <BswM_MemMap.h>

/*------------------[User callouts]------------------------------------------*/

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnStartupTwoA(void)
{
  Eb_Intgr_BswM_DriverInitTwo();

#if (BASE_NVM_ENABLED==STD_ON)
  /* when the NvM is used, call NvM_ReadAll() here. The job finished
   * callback EcuM_CB_NfyNvMJobEnd will then request BSWM_STARTUP_TWO_B. */
  NvM_ReadAll();
#else
  /* directly request BSWM_STARTUP_TWO_B */
  BswM_RequestMode(EB_INTGR_BSWM_BSWM_SM, EB_INTGR_BSWM_STARTUP_TWO_B);
#endif

}

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnStartupTwoB(void)
{
  Eb_Intgr_BswM_DriverInitThree();
}

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnPrpShutdown(void)
{
  /* Do all activities here which shall be done right before the shutdown */

  EcuM_StateType shutdownTarget;
  uint8 sleepMode;

#if defined(BASE_DEM_ENABLED)
#if (BASE_DEM_ENABLED==STD_ON)
  /* end the Dem operation cycle and shutdown the Dem */
  (void) Dem_SetOperationCycleState(DEM_OPCYC_POWER, DEM_CYCLE_STATE_END);
  Dem_Shutdown();
#endif
#endif

  EcuM_GetShutdownTarget(&shutdownTarget, &sleepMode);

  if (ECUM_STATE_OFF == shutdownTarget)
  {
    BswM_RequestMode(EB_INTGR_BSWM_BSWM_SM, EB_INTGR_BSWM_GO_OFF_ONE_A);
  }
  else if (ECUM_STATE_SLEEP == shutdownTarget)
  {
    EcuM_GoPoll();
  }
  else if (ECUM_STATE_RESET == shutdownTarget)
  {
    /* Insert your code here. */
  }
  else
  {
    /* Insert your code here. */
  }
}

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffOneA(void)
{
#if 0
/* When compile the final demo application, remove enclosing "#if 0", "#endif"
 * preprocessor statements.
 *
 * Reason:
 * Within the basicTemplate itself the RteGeneratorOutput is set to BSW_SCHEDULER_ONLY
 * and hence no Rte_Start(), Rte_Stop() are generated/provided.
 *
 * But during demo application configuration the RteGeneratorOutput is set to FULL mode
 * and Rte_Start(), Rte_Stop() are generated/provided and have to be called by the application.
*/
#if defined(BASE_RTE_ENABLED)
#if (BASE_RTE_ENABLED==STD_ON)
  if ( Rte_Stop() != E_OK )
  {
    /* failed to stop the Rte */
  }
#endif
#endif
#endif /* 0 */

#if defined(BASE_COMM_ENABLED)
#if (BASE_COMM_ENABLED==STD_ON)
  ComM_DeInit();
#endif
#endif

#if (BASE_NVM_ENABLED==STD_ON)
  /* when the NvM is used, call NvM_WriteAll() here. The job finished
   * callback EcuM_CB_NfyNvMJobEnd will then request BSWM_GO_OFF_ONE_B. */
  NvM_WriteAll();
#else
  /* directly request BSWM_GO_OFF_ONE_B */
  BswM_RequestMode(EB_INTGR_BSWM_BSWM_SM, EB_INTGR_BSWM_GO_OFF_ONE_B);
#endif
}


FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffOneB(void)
{
  (void) EcuM_GoDown(BSWM_MODULE_ID);
}


FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnGoOffTwo(void)
{
  /* Insert your code here. */
}

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnWakeupTwo(void)
{
  /* Insert your code here. */
}

FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_DriverInitTwo(void)
{
  /* Adapt names of initialization functions or configuration structure
   * parameters in header file "Platforms_Modules.h" for architecture specific
   * modifications.
   * The following Module initialization sequence has been implemented on basis
   * of table 2 "Driver Initialization Details, Sample Configuration" you will
   * find in SWS "Specification of ECU State Manager with fixed state machine"
   * (document version  1.1.0, AUTOSAR 4.0.2). */

#if defined(BASE_SPI_ENABLED)
#if (BASE_SPI_ENABLED==STD_ON)
  SPI_INIT_FUNC(BASE_SPI_CONFIG_PTR);
#endif
#endif
#if defined(BASE_EEP_ENABLED)
#if (BASE_EEP_ENABLED==STD_ON)
  EEP_INIT_FUNC(BASE_EEP_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FLS_ENABLED)
#if (BASE_FLS_ENABLED==STD_ON)
  FLS_INIT_FUNC(BASE_FLS_CONFIG_PTR);
#endif
#endif
#if defined(BASE_EA_ENABLED)
#if (BASE_EA_ENABLED==STD_ON)
  EA_INIT_FUNC();
#endif
#endif
#if defined(BASE_FEE_ENABLED)
#if (BASE_FEE_ENABLED==STD_ON)
  FEE_INIT_FUNC();
#endif
#endif
#if defined(BASE_NVM_ENABLED)
#if (BASE_NVM_ENABLED==STD_ON)
  NVM_INIT_FUNC();
#endif
#endif
#if defined(BASE_CANTRCV_ENABLED)
#if (BASE_CANTRCV_ENABLED==STD_ON)
  CANTRCV_INIT_FUNC();
#endif
#endif
#if defined(BASE_CAN_ENABLED)
#if (BASE_CAN_ENABLED==STD_ON)
  CAN_INIT_FUNC(BASE_CAN_CONFIG_PTR);
#endif
#endif
#if defined(BASE_CANIF_ENABLED)
#if (BASE_CANIF_ENABLED==STD_ON)
  CANIF_INIT_FUNC(BASE_CANIF_CONFIG_PTR);
#endif
#endif
#if defined(BASE_CANSM_ENABLED)
#if (BASE_CANSM_ENABLED==STD_ON)
  CANSM_INIT_FUNC(BASE_CANSM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_CANTP_ENABLED)
#if (BASE_CANTP_ENABLED==STD_ON)
  CANTP_INIT_FUNC(BASE_CANTP_CONFIG_PTR);
#endif
#endif
#if defined(BASE_LINTRCV_ENABLED)
#if (BASE_LINTRCV_ENABLED==STD_ON)
  LINTRCV_INIT_FUNC();
#endif
#endif
#if defined(BASE_LIN_ENABLED)
#if (BASE_LIN_ENABLED==STD_ON)
  LIN_INIT_FUNC(BASE_LIN_CONFIG_PTR);
#endif
#endif
#if defined(BASE_LINIF_ENABLED)
#if (BASE_LINIF_ENABLED==STD_ON)
  LINIF_INIT_FUNC(BASE_LINIF_CONFIG_PTR);
#endif
#endif
#if defined(BASE_LINSM_ENABLED)
#if (BASE_LINSM_ENABLED==STD_ON)
  LINSM_INIT_FUNC(BASE_LINSM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_LINTP_ENABLED)
#if (BASE_LINTP_ENABLED==STD_ON)
  LINTP_INIT_FUNC(BASE_LINTP_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FRTRCV_ENABLED)
#if (BASE_FRTRCV_ENABLED==STD_ON)
  FRTRCV_INIT_FUNC();
#endif
#endif
#if defined(BASE_FR_ENABLED)
#if (BASE_FR_ENABLED==STD_ON)
  FR_INIT_FUNC(BASE_FR_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FRIF_ENABLED)
#if (BASE_FRIF_ENABLED==STD_ON)
  FRIF_INIT_FUNC(BASE_FRIF_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FRSM_ENABLED)
#if (BASE_FRSM_ENABLED==STD_ON)
  FRSM_INIT_FUNC(BASE_FRSM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FRTP_ENABLED)
#if (BASE_FRTP_ENABLED==STD_ON)
  FRTP_INIT_FUNC(BASE_FRTP_CONFIG_PTR);
#endif
#endif
#if defined(BASE_PDUR_ENABLED)
#if (BASE_PDUR_ENABLED==STD_ON)
  PDUR_INIT_FUNC(BASE_PDUR_CONFIG_PTR);
#endif
#endif
#if defined(BASE_NM_ENABLED)
#if (BASE_NM_ENABLED==STD_ON)
  NM_INIT_FUNC();
#endif
#endif
#if defined(BASE_CANNM_ENABLED)
#if (BASE_CANNM_ENABLED==STD_ON)
  CANNM_INIT_FUNC(BASE_CANNM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_FRNM_ENABLED)
#if (BASE_FRNM_ENABLED==STD_ON)
  FRNM_INIT_FUNC(BASE_FRNM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_COM_ENABLED)
#if (BASE_COM_ENABLED==STD_ON)
  COM_INIT_FUNC(BASE_COM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_IPDUM_ENABLED)
#if (BASE_IPDUM_ENABLED==STD_ON)
  IPDUM_INIT_FUNC(BASE_IPDUM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_DCM_ENABLED)
#if (BASE_DCM_ENABLED==STD_ON)
  DCM_INIT_FUNC(BASE_DCM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_RAMTST_ENABLED)
#if (BASE_RAMTST_ENABLED==STD_ON)
  RAMTST_INIT_FUNC();
#endif
#endif
#if defined(BASE_ETHTRCV_ENABLED)
#if (BASE_ETHTRCV_ENABLED==STD_ON)
  ETHTRCV_INIT_FUNC(BASE_ETHTRCV_CONFIG_PTR);
#endif
#endif
#if defined(BASE_ETH_ENABLED)
#if (BASE_ETH_ENABLED==STD_ON)
  ETH_INIT_FUNC(BASE_ETH_CONFIG_PTR);
#endif
#endif
#if defined(BASE_ETHIF_ENABLED)
#if (BASE_ETHIF_ENABLED==STD_ON)
  ETHIF_INIT_FUNC(BASE_ETHIF_CONFIG_PTR);
#endif
#endif
#if defined(BASE_ETHSM_ENABLED)
#if (BASE_ETHSM_ENABLED==STD_ON)
  ETHSM_INIT_FUNC(BASE_ETHSM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_TCPIP_ENABLED)
#if (BASE_TCPIP_ENABLED==STD_ON)
  TCPIP_INIT_FUNC(BASE_TCPIP_CONFIG_PTR);
#endif
#endif
#if defined(BASE_SOAD_ENABLED)
#if (BASE_SOAD_ENABLED==STD_ON)
  SOAD_INIT_FUNC(BASE_SOAD_CONFIG_PTR);
#endif
#endif
#if defined(BASE_SD_ENABLED)
#if (BASE_SD_ENABLED==STD_ON)
  SD_INIT_FUNC();
#endif
#endif
#if defined(BASE_UDPNM_ENABLED)
#if (BASE_UDPNM_ENABLED==STD_ON)
  UDPNM_INIT_FUNC();
#endif
#endif
}


FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_DriverInitThree(void)
{
  /* Adapt names of initialization functions or configuration structure
   * parameters in header file "Platforms_Modules.h" for architecture specific
   * modifications.
   * The following Module initialization sequence has been implemented on basis
   * of table 2 "Driver Initialization Details, Sample Configuration" you will
   * find in SWS "Specification of ECU State Manager with fixed state machine"
   * (document version  1.1.0, AUTOSAR 4.0.2). */

#if defined(BASE_COMM_ENABLED)
#if (BASE_COMM_ENABLED==STD_ON)
  COMM_INIT_FUNC(BASE_COMM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_DEM_ENABLED)
#if (BASE_DEM_ENABLED==STD_ON)
  DEM_INIT_FUNC();
  /* start the standard operation cycle */
  (void) Dem_SetOperationCycleState(DEM_OPCYC_POWER, DEM_CYCLE_STATE_START);
#endif
#endif
#if defined(BASE_FIM_ENABLED)
#if (BASE_FIM_ENABLED==STD_ON)
  FIM_INIT_FUNC(BASE_FIM_CONFIG_PTR);
#endif
#endif
#if defined(BASE_XCP_ENABLED)
#if (BASE_XCP_ENABLED==STD_ON)
  /* Xcp uses NvM RAM block, hence must be located here and not in
  BswM_DriverInitTwo */
  XCP_INIT_FUNC(BASE_XCP_CONFIG_PTR);
#endif
#endif
}


FUNC(void, BSWM_APPL_CODE) Eb_Intgr_BswM_OnRunTwo(void)
{
#if defined(BASE_COMM_ENABLED)
#if (BASE_COMM_ENABLED==STD_ON)
  ComM_CommunicationAllowed(ComMConf_ComMChannel_CanNetwork, TRUE);
#endif
#endif

#if defined(BASE_RTE_ENABLED)
#if (BASE_RTE_ENABLED==STD_ON)
#if 0
/* When compile the final demo application, remove enclosing "#if 0", "#endif"
 * preprocessor statements.
 *
 * Reason:
 * Within the basicTemplate itself the RteGeneratorOutput is set to BSW_SCHEDULER_ONLY
 * and hence no Rte_Start(), Rte_Stop() are generated/provided.
 *
 * But during demo application configuration the RteGeneratorOutput is set to FULL mode
 * and Rte_Start(), Rte_Stop() are generated/provided and have to be called by the application.
*/
  if ( Rte_Start() != E_OK )
  {
    /* Rte start failed */
  }
#endif /* 0 */
#endif
#endif
}

#if (BASE_NVM_ENABLED==STD_ON)
/* define NvM job finished callback only if NvM is part of the project */

FUNC(void, ECUM_APPL_CODE) EcuM_CB_NfyNvMJobEnd
(
  uint8                 ServiceId,
  NvM_RequestResultType JobResult
)
{
#if (NVM_DYNAMIC_CONFIGURATION == STD_ON)
  TS_PARAM_UNUSED(JobResult);
#endif

  if (ServiceId == NVM_READ_ALL_API_ID)
  {
#if (NVM_DYNAMIC_CONFIGURATION == STD_OFF)

    if (JobResult != NVM_REQ_OK)
    {
      NvM_RequestResultType BlockResult;
      Std_ReturnType Error = NvM_GetErrorStatus(NvMConf_NvMBlockDescriptor_NvMBlock_ConfigID, &BlockResult);

      if (   (Error == E_OK)
          && (BlockResult != NVM_REQ_OK) )
      {
        /* Write ConfigId */
        (void) NvM_WriteBlock(NvMConf_NvMBlockDescriptor_NvMBlock_ConfigID, NULL_PTR);
      }
    }

#endif

    BswM_RequestMode(EB_INTGR_BSWM_BSWM_SM, EB_INTGR_BSWM_STARTUP_TWO_B);
  }
  else
  {
#if (EB_INTGR_DEV_ERROR_DETECT == STD_ON)

    (void) Det_ReportError(EB_INTGR_MODULE_ID,
                           EB_INTGR_INSTANCE_ID,
                           EB_INTGR_API_ECUM_CB_NFYNVMJOBEND,
                           EB_INTGR_E_WRONG_SERVICEID);

#endif
  }
}
#endif

#define BSWM_STOP_SEC_CODE
#include <BswM_MemMap.h>


/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
