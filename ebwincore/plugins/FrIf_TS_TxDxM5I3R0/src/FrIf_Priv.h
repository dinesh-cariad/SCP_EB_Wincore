/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
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
* MISRAC2012-1) Deviated Rule: 20.10 (required)
* The # and ## preprocessor operators should not be used.
*
* Reason:
* ## operator is used to keep the code simple.
*
*/

#ifndef FRIF_PRIV_H
#define FRIF_PRIV_H

/*======================[Include Section]=====================================*/

#include <TSAutosar.h>      /* EB specific standard types */
#include <FrIf_Api.h>       /* get FrIf types and function prototypes          */
#include <FrIf_LocalCfg.h>  /* get local (porting specific) compiler switches */

#define TS_RELOCATABLE_CFG_ENABLE FRIF_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>

#include <FrIf_guards.h> /* Include define guards */

/* test for valid macro definition of FRIF_DECOUPLED_RX_ENABLE */
#ifndef FRIF_DECOUPLED_RX_ENABLE
#error FRIF_DECOUPLED_RX_ENABLE not defined
#endif  /* FRIF_DECOUPLED_RX_ENABLE */

/* test for valid value of FRIF_DECOUPLED_RX_ENABLE */
#if (FRIF_DECOUPLED_RX_ENABLE != STD_ON) && \
    (FRIF_DECOUPLED_RX_ENABLE != STD_OFF)
#error macro FRIF_DECOUPLED_RX_ENABLE has an invalid value
#endif /* FRIF_DECOUPLED_RX_ENABLE */


/* test for valid macro definition of FRIF_IMMEDIATE_RX_ENABLE */
#ifndef FRIF_IMMEDIATE_RX_ENABLE
#error FRIF_IMMEDIATE_RX_ENABLE not defined
#endif  /* FRIF_IMMEDIATE_RX_ENABLE */

/* test for valid value of FRIF_IMMEDIATE_RX_ENABLE */
#if (FRIF_IMMEDIATE_RX_ENABLE != STD_ON) && \
    (FRIF_IMMEDIATE_RX_ENABLE != STD_OFF)
#error macro FRIF_IMMEDIATE_RX_ENABLE has an invalid value
#endif /* FRIF_IMMEDIATE_RX_ENABLE */


/* test for valid macro definition of FRIF_DECOUPLED_TX_ENABLE */
#ifndef FRIF_DECOUPLED_TX_ENABLE
#error FRIF_DECOUPLED_TX_ENABLE not defined
#endif  /* FRIF_DECOUPLED_TX_ENABLE */

/* test for valid value of FRIF_DECOUPLED_TX_ENABLE */
#if (FRIF_DECOUPLED_TX_ENABLE != STD_ON) && \
    (FRIF_DECOUPLED_TX_ENABLE != STD_OFF)
#error macro FRIF_DECOUPLED_TX_ENABLE has an invalid value
#endif /* FRIF_DECOUPLED_TX_ENABLE */


/* test for valid macro definition of FRIF_IMMEDIATE_TX_ENABLE */
#ifndef FRIF_IMMEDIATE_TX_ENABLE
#error FRIF_IMMEDIATE_TX_ENABLE not defined
#endif  /* FRIF_IMMEDIATE_TX_ENABLE */

/* test for valid value of FRIF_IMMEDIATE_TX_ENABLE */
#if (FRIF_IMMEDIATE_TX_ENABLE != STD_ON) && \
    (FRIF_IMMEDIATE_TX_ENABLE != STD_OFF)
#error macro FRIF_IMMEDIATE_TX_ENABLE has an invalid value
#endif /* FRIF_IMMEDIATE_TX_ENABLE */


/* test for valid macro definition of FRIF_TXCONFIRMATION_ENABLE */
#ifndef FRIF_TXCONFIRMATION_ENABLE
#error FRIF_TXCONFIRMATION_ENABLE not defined
#endif  /* FRIF_TXCONFIRMATION_ENABLE */

/* test for valid value of FRIF_TXCONFIRMATION_ENABLE */
#if (FRIF_TXCONFIRMATION_ENABLE != STD_ON) && \
    (FRIF_TXCONFIRMATION_ENABLE != STD_OFF)
#error macro FRIF_TXCONFIRMATION_ENABLE has an invalid value
#endif /* FRIF_TXCONFIRMATION_ENABLE */

/*======================[Global Macros]=======================================*/

#if (defined FRIF_GET_CONFIG_ADDR) /* to prevent double declaration */
#error FRIF_GET_CONFIG_ADDR already defined
#endif /* FRIF_GET_CONFIG_ADDR */

/** \brief Define FRIF_GET_CONFIG_ADDR macro for getting a reference to a config element, */
#define FRIF_GET_CONFIG_ADDR(type, offset) \
TS_UNCHECKEDGETCFG(FrIf_RootConfigPtr, type, FRIF, offset)

#if (defined FRIF_GET_CHECKED_CONFIG_ADDR) /* to prevent double declaration */
#error FRIF_GET_CHECKED_CONFIG_ADDR already defined
#endif /* FRIF_GET_CHECKED_CONFIG_ADDR */

/** \brief Define FRIF_GET_CHECKED_CONFIG_ADDR macro for getting a reference to a config element, */
#define FRIF_GET_CHECKED_CONFIG_ADDR(type, offset) \
TS_CHECKEDGETCFG(FrIf_RootConfigPtr, type, FRIF, offset)

/*===================== Module Initialization Check macro ====================*/

/* start data section declaration */
#define FRIF_START_SEC_VAR_INIT_8
#include <FrIf_MemMap.h> /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/* variable definition used for module initialization status checking */
extern VAR(uint8,FRIF_VAR) FrIf_ModuleInitialized;

/* stop data section declaration */
#define FRIF_STOP_SEC_VAR_INIT_8
#include <FrIf_MemMap.h> /* !LINKSTO FrIf.ASR40.FrIf05088,1 */


/*================== DEM error reporting and BusMirroring macros =============*/

/* check if at least one channel status information shall be reported to DEM */
#if (   (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_PROD_ERR_HANDLING_SW_CH_A  == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_PROD_ERR_HANDLING_SW_CH_B  == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM) )
#define FRIF_CHANNEL_STATUS_DEM_REPORTING STD_ON
#else
#define FRIF_CHANNEL_STATUS_DEM_REPORTING STD_OFF
#endif

/* if at least one DEM reporting is switched on, include Dem.h */
#if (   (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DEM) \
     || (FRIF_CHANNEL_STATUS_DEM_REPORTING == STD_ON) )
#include <Dem.h>
#endif

/* !LINKSTO FrIf.ASR44.SWS_FrIf_05095,1 */
#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
#include <Mirror.h>
#endif /* FRIF_BUSMIRRORINGSUPPORT_ENABLE */

/* check if at least one channel status information shall be reported at all */
#if (   (FRIF_PROD_ERR_HANDLING_NIT_CH_A != TS_PROD_ERR_DISABLE) \
     || (FRIF_PROD_ERR_HANDLING_NIT_CH_B != TS_PROD_ERR_DISABLE) \
     || (FRIF_PROD_ERR_HANDLING_SW_CH_A  != TS_PROD_ERR_DISABLE) \
     || (FRIF_PROD_ERR_HANDLING_SW_CH_B  != TS_PROD_ERR_DISABLE) \
     || (FRIF_PROD_ERR_HANDLING_ACS_CH_A != TS_PROD_ERR_DISABLE) \
     || (FRIF_PROD_ERR_HANDLING_ACS_CH_B != TS_PROD_ERR_DISABLE) )
#define FRIF_CHANNEL_STATUS_REPORTING STD_ON
#else
#define FRIF_CHANNEL_STATUS_REPORTING STD_OFF
#endif


/*========================= DET error reporting macros =======================*/

/* checking for development error tracer (DET) configuration */
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)


#include <Det.h>               /* get development error tracer interface */

/* function-like macro for simplified DET usage - map to Det - API function call */
#define FrIf_Det_ReportError(nServiceID, nErrorID) \
            (void)Det_ReportError(        \
                ((uint16)FRIF_MODULE_ID), \
                ((uint8)0U),              \
                ((uint8)(nServiceID)),     \
                ((uint8)(nErrorID)))

#else  /* FRIF_REPORT_TO_DET_ENABLE */

/* function-like macro for simplified DET usage maps to empty statement */
#define FrIf_Det_ReportError(nServiceID, nErrorID)

#endif   /* FRIF_REPORT_TO_DET_ENABLE */

/*========================= constant config address macros ===================*/

/* for future implementation - currently not configurable */
#define FRIF_CONST_CFG_ADDRESS_ENABLE STD_OFF
#if (FRIF_CONST_CFG_ADDRESS_ENABLE == STD_OFF)    /* configuration address is non-constant */

/* start data section declaration */
#define FRIF_START_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */


extern P2CONST(FrIf_ConfigType,FRIF_VAR_FAST,FRIF_APPL_CONST) FrIf_RootConfigPtr;

/* start data section declaration */
#define FRIF_STOP_SEC_VAR_FAST_INIT_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */


#elif (FRIF_CONST_CFG_ADDRESS_ENABLE == STD_ON) /* configuration address is constant */

/*
 * root pointer is replaced by absolute address that is given via
 * pre-compile-time switch FRIF_CONST_CONFIG_ADDRESS
 */
#define FrIf_RootConfigPtr                                                   \
    ((P2CONST(FrIf_ConfigType,FRIF_VAR_FAST,FRIF_APPL_CONST))    \
    FRIF_CONST_CONFIG_ADDRESS)

#endif /* FRIF_CONST_CFG_ADDRESS_ENABLE */

/*================= Single controller optimization macros ====================*/
/* only the macros here must be used for addressing the drivers hardware ressources */

/* single controller optimization is enabled */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

/* replace all indexing macros by a constant index - indexing parameters are not used */
#define FRIF_CTRLIDX 0U
#define FRIF_NUM_CTRL 1U
#define FRIF_FR_CTRLIDX 0U

#elif (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)

/* replace all indexing macros by function parameter names */
#define FRIF_CTRLIDX (FrIf_CtrlIdx)
#define FRIF_NUM_CTRL (FrIf_NumCtrl)
#define FRIF_FR_CTRLIDX \
  (FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_CTRLIDX].FrDeviceIdx)

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */


/* If only a single Cluster is enabled */
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

#define FRIF_CLSTIDX 0U

/* obtains number of Tx-Pdus assigned to the cluster */
#define FRIF_GET_CLSTPDUNUM(clstptr) (FrIf_RootConfigPtr->FrIf_TxPduNum)

#else /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF */

#define FRIF_CLSTIDX (FrIf_ClstIdx)

/* obtains number of Tx-Pdus assigned to the cluster */
#define FRIF_GET_CLSTPDUNUM(clstptr) ((clstptr)->ClstTxPduNum)

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */


#define FRIF_FRTRCV_INSTANCE_IDX \
  (FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_CTRLIDX].Trcv[FrIf_ChnlIdx].TrcvDriverIdx)

#define FRIF_FR_INSTANCE_IDX \
  (FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_CTRLIDX].FrDriverIdx)


#define FrIf_Call_Fr_Arg0(DrvIdx,service_name)  \
          FrIf_Call_Fr(DrvIdx,service_name)()

#define FrIf_Call_Fr_Arg1(DrvIdx,service_name,a)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a))

#define FrIf_Call_Fr_Arg2(DrvIdx,service_name,a,b)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b))

#define FrIf_Call_Fr_Arg3(DrvIdx,service_name,a,b,c)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b),(c))

#define FrIf_Call_Fr_Arg4(DrvIdx,service_name,a,b,c,d)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b),(c),(d))

#define FrIf_Call_Fr_Arg5(DrvIdx,service_name,a,b,c,d,e)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b),(c),(d),(e))

#define FrIf_Call_Fr_Arg6(DrvIdx,service_name,a,b,c,d,e,f)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b),(c),(d),(e),(f))

#define FrIf_Call_Fr_Arg8(DrvIdx,service_name,a,b,c,d,e,f,g,h)  \
          FrIf_Call_Fr(DrvIdx,service_name)((a),(b),(c),(d),(e),(f),(g),(h))

#if (FRIF_SINGLE_FR_OPT_ENABLE == STD_ON)


#define FrIf_Call_Fr(DrvIdx,service_name)  \
            FrIf_Call_Fr_Tmp(FRIF_SINGLE_FR_PREFIX,service_name)

#define FrIf_Call_Fr_Tmp(Prefix,service_name)  \
             FrIf_Call_Fr_Tmp1(Prefix,service_name)

/* Deviation MISRAC2012-1 */
#define FrIf_Call_Fr_Tmp1(Prefix,service_name)  \
            Prefix##_##service_name

#else

/* Deviation MISRAC2012-1 */
#define FrIf_Call_Fr(DrvIdx,service_name)  \
            FrIf_FrFuncPtr[(DrvIdx)].Fr_##service_name


#endif /* FRIF_SINGLE_FR_OPT_ENABLE */



#if (FRIF_SINGLE_FRTRCV_OPT_ENABLE == STD_ON)

#define FrIf_Call_FrTrcv(DrvIdx,service_name)  \
            FrIf_Call_FrTrcv_Tmp1(FRIF_SINGLE_FRTRCV_PREFIX,service_name)

#define FrIf_Call_FrTrcv_Tmp1(DrvIdx,service_name)  \
            FrIf_Call_FrTrcv_Tmp(DrvIdx,service_name)

/* Deviation MISRAC2012-1 */
#define FrIf_Call_FrTrcv_Tmp(Prefix,service_name)  \
            Prefix##_##service_name

#else /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */

/* Deviation MISRAC2012-1 */
#define FrIf_Get_FrTrcvFunctionPtr(DrvIdx,service_name) \
            FrIf_TrcvFuncPtr[(DrvIdx)].FrTrcv_##service_name

/* Deviation MISRAC2012-1 */
#define FrIf_Call_FrTrcv(DrvIdx,service_name)  \
            FrIf_TrcvFuncPtr[(DrvIdx)].FrTrcv_##service_name

#endif /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */

/* internal state machine states */
#define FRIF_SYNCSTATE_ASYNC 0U
#define FRIF_SYNCSTATE_SYNC 1U

#if (FRIF_IMMEDIATE_TX_ENABLE == STD_ON)

/* bit index of tx-pdu type bit */
#define FRIF_TXTYPE_SHIFT 7
/* bit mask (for byte masking) from bit index for tx-pdu type bit */
#define FRIF_TXTYPE_MASK ((uint8)(0x01U<<(FRIF_TXTYPE_SHIFT)))
/* tx-pdu type is decoupled transmission */
#define FRIF_TXTYPE_DECOUPLED 0U

/* bit mask for getting controller idx of TxType_TxCntLimit_CtrlIdx attribute of FrIf_TxPduType */
#define FRIF_CTRLIDX_MASK 0x7FU

#endif /* FRIF_IMMEDIATE_TX_ENABLE */


/* bit mask for getting transmit counter for queued transmit requests in case of decoupled tx-pdu */
#define FRIF_TXCNT_MASK 0x7FU

#if (FRIF_TRANSMIT_QUEUE_ENABLE == STD_ON)

/* obtains transmit counter limit for queued transmit requests from var TxType_TxCntLimit_CtrlIdx */
#define FRIF_GET_CNTLIMIT(x) ((uint8)((uint8)(x)&(uint8)(FRIF_TXCNT_MASK)))

#else /* (FRIF_TRANSMIT_QUEUE_ENABLE == STD_OFF) */

/* transmit counter limit if queued transmit requests are disabled */
#define FRIF_GET_CNTLIMIT(x) 1U

#endif /* FRIF_TRANSMIT_QUEUE_ENABLE */


/* status value for uninitialized module */
#define FRIF_UNINIT 0U
/* status value for initialized module */
#define FRIF_INIT 1U


/* tx pending bit not used /not set */
#define FRIF_PENDINGBIT_NONE 0x00U
/* bit index of pending bit */
#define FRIF_PENDINGBIT_SHIFT 7
/* bit mask (for byte masking) from bit index for tx pending bit */
#define FRIF_PENDINGBIT_MASK ((uint8)(0x01U<<(FRIF_PENDINGBIT_SHIFT)))
/* Transform bitmask into a clear bit mask */
#define FRIF_PENDINGBIT_CLEARBITMASK ((uint8)(((uint8)(0xFFU))^((uint8)(FRIF_PENDINGBIT_MASK))))
/* Transform bitmask into a set bit mask */
#define FRIF_PENDINGBIT_SETBITMASK (FRIF_PENDINGBIT_MASK)


/* tx update bit not used /not set */
#define FRIF_UPDATEBIT_NONE 8U
/* bit mask (for byte masking) for bit position of update bit */
#define FRIF_UB_BITPOS_MASK 0x0FU
/* obtains bit position of UB from var UB_BitMask_OwnerIdx of type FrIf_PduPositionType */
#define FRIF_UB_GETBITPOS(x) \
    ((uint8)(((uint8)(x))&((uint8)(FRIF_UB_BITPOS_MASK))))
/* obtain bitmask (for byte masking) from bit index for update bit */
#define FRIF_UB_GETBITMASK(bitindex) \
    ((uint8)(0x01U<<(bitindex)))
/* Transform bitmask into a clear bit mask */
#define FRIF_UB_CLEARBITMASK(updatebitmask) \
    ((uint8)(((uint8)(0xFFU))^((uint8)(updatebitmask))))
/* Transform bitmask into a set bit mask */
#define FRIF_UB_SETBITMASK(updatebitmask) (updatebitmask)

/* bit shift value for bitmask of owner index value */
#define FRIF_OWNERIDX_SHIFT 4
/* bit mask (for byte masking) for owner index value */
#define FRIF_OWNERIDX_MASK 0x0FU
/* obtains owner index from var UB_BitMask_OwnerIdx of type FrIf_PduPositionType */
#define FRIF_GET_OWNERIDX(x) \
    ((uint8)(((uint8)(((uint8)(x))>>(FRIF_OWNERIDX_SHIFT)))&((uint8)(FRIF_OWNERIDX_MASK))))

/* Fr_GetChannelStatus bit pattern */
#define FRIF_CHNL_STAT_ACS_VF    0x0001U
#define FRIF_CHNL_STAT_ACS_SE    0x0002U
#define FRIF_CHNL_STAT_ACS_CE    0x0004U
#define FRIF_CHNL_STAT_ACS_AC    0x0008U
#define FRIF_CHNL_STAT_ACS_BV    0x0010U
#define FRIF_CHNL_STAT_ACS_TC    0x0020U
#define FRIF_CHNL_STAT_SW_VM     0x0100U
#define FRIF_CHNL_STAT_SW_SE     0x0200U
#define FRIF_CHNL_STAT_SW_BV     0x0400U
#define FRIF_CHNL_STAT_SW_TC     0x0800U
#define FRIF_CHNL_STAT_NIT_SE    0x1000U
#define FRIF_CHNL_STAT_NIT_BV    0x2000U

/* NIT status data bitmask vSS!SyntaxError, vSS!Bviolation */
#define FRIF_ERRBITMASK_NIT_CH    ((uint16)(FRIF_CHNL_STAT_NIT_SE | FRIF_CHNL_STAT_NIT_BV))

/* symbol window status data bitmask vSS!SyntaxError, vSS!Bviolation, vSS!TxConflict) */
#define FRIF_ERRBITMASK_SW_CH \
    ((uint16)((uint16)(FRIF_CHNL_STAT_SW_SE | FRIF_CHNL_STAT_SW_BV) | FRIF_CHNL_STAT_SW_TC))

/* aggregated channel status bitmask vSS!SyntaxError, vSS!ContentError, vSS!Bviolation */
/* and vSS!TxConflict */
#define FRIF_ERRBITMASK_ACS_CH    \
    ((uint16)((uint16)(FRIF_CHNL_STAT_ACS_SE | FRIF_CHNL_STAT_ACS_CE) \
            | (uint16)(FRIF_CHNL_STAT_ACS_BV | FRIF_CHNL_STAT_ACS_TC)))


#undef TS_RELOCATABLE_CFG_ENABLE

/*======================[Global Data Types]===================================*/

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

#endif  /* FRIF_PRIV_H */

