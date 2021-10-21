/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (defined TS_MERGED_COMPILE)
#error TS_MERGED_COMPILE is already defined
#endif
/* Set macro that enables merged compilation macros. */
#define TS_MERGED_COMPILE STD_ON

/* First of all include all header files, the C-files depend on.
 * This prevents overlapping memory sections because all included files are protected for
 * further inclusion. */

#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h>
#include <SoAd_Trace.h>
#include <SoAd.h> /* Module public header file. */
#include <SoAd_Int.h> /* Module internal header file. */
#include <SoAd_Cbk.h> /* Module callbacks */
#include <SoAd_Version.h> /* Module Version Info */
#include <SchM_SoAd.h>
#if(SOAD_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
#if(SOAD_META_DATA_HANDLING == STD_ON)
#include <EcuC.h> /* Module public API */
#endif
#if(SOAD_TLS_ENABLED == STD_ON)
#include <Tls.h>
#endif /* SOAD_TLS_ENABLED == STD_ON */

/* manual handling of special cases (conditional includes) */
#include <SoAd_Cfg.h>          /* defines SOAD_PBCFGM_SUPPORT_ENABLED */
#if (SOAD_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
  #include <PbcfgM.h>           /* Post build configuration manager API */
#endif /* SOAD_PBCFGM_SUPPORT_ENABLED */

#include <SoAd_01_UdpSM_Int.h> /* UDP state machine unit. */
#include <SoAd_01_UdpSM_User.h> /* Unit public header file. */
#include <SoAd_02_SocketCon_Int.h> /* Common socket handling relevant APIs. */
#include <SoAd_04_TcpSM_Int.h> /* UDP state machine unit. */
#include <SoAd_04_TcpSM_User.h> /* Unit public header file. */

/* All source files which contains sections other than *_SEC_CODE (or no sections) have to be
 * included before the memory mapping in this file to protect them for overlapping memory
 * sections. */
#include <SoAd_Int_08_MainFunction.c>

/* List of files that include only memory abstraction CODE segments. */
/* Start code section declaration. */
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

#include <SoAd_01_UdpSM.c>
#include <SoAd_01_UdpSM_User.c>
#include <SoAd_02_SocketCon.c>
#include <SoAd_03_Lib.c>
#include <SoAd_04_TcpSM.c>
#include <SoAd_04_TcpSM_User.c>
#include <SoAd_Int_05_TxTp.c>
#include <SoAd_Int_06_TxIf.c>
#include <SoAd_Int_07_Rx.c>



/* Stop code section declaration. */
#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>
