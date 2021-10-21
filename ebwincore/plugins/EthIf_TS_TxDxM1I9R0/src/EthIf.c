/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */



/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Usage of concatenation operator improves readability of code.
 *
 */
/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */

/* don't include PB config information here (at compile time) */
#if (!defined ETHIF_NO_PBCFG_REQUIRED)
#define ETHIF_NO_PBCFG_REQUIRED
#endif

#include <EthIf.h>             /* Module public API */
#include <EthIf_Int.h> /* Module internal declarations */
#include <SchM_EthIf.h> /* Exclusive area */
#include <EthIf_Cbk.h>  /* Module callback */
#include <EthIf_Lcfg.h> /* Module Prio Rx API */
#include <EthSM_Cbk.h>  /* EthSM callbacks */
#include <Eth_GeneralTypes.h>

#include <TSPBConfig_Signature.h> /* relocatable post-build macros */

#if (ETHIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
#define PBCFGM_NO_PBCFG_REQUIRED
#include <PbcfgM.h>     /* Post build configuration manager API */
#endif /* ETHIF_PBCFGM_SUPPORT_ENABLED */

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)
#include <BswM_EthIf.h>
#endif /* ETHIF_PORTGROUPSUPPORT == STD_ON */

#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>               /* Det API */
#endif

#ifdef ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS
#include <EthIf_PBcfg.h>      /* PBcfg structure, to allow pre-compile time like  access */
#endif /* ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS */

#include <EthIf_Trace.h>

#include <TSMem.h>

#define TS_RELOCATABLE_CFG_ENABLE    ETHIF_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>     /* Access to postbuild config */

/*==================[postbuild check]=======================================*/

#ifndef ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS
#if (defined ETHIF_PBCFG_H)
#error Postbuild configuration must not be included from module source!
#endif
#endif /* ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS */

/*==================[macros]================================================*/

/*===[postbuild access]===*/

/** \brief Access Post-Build ROM config
 **
 ** This macro accesses an element (struct) in the Post-build config
 ** by using the given references.
 **
 ** \param[in] type Type of the element (member) to access
 ** \param[in] element Element (member) in the config to access
 */
#ifdef ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS

#if (defined ETHIF_PBCFG_ACCESS)
#error ETHIF_PBCFG_ACCESS is already defined
#endif
#define ETHIF_PBCFG_ACCESS(type, name, element) \
    (EthIf_ConfigLayout.name[element])

#else

#if (defined ETHIF_PBCFG_ACCESS)
#error ETHIF_PBCFG_ACCESS is already defined
#endif
/* Deviation MISRAC2012-1 */
#define ETHIF_PBCFG_ACCESS(type, name, element) \
    (TS_UNCHECKEDGETCFG(EthIf_RootPtr, type, ETHIF, (EthIf_RootPtr->name ## Ref))[(element)])

#endif /* ETHIF_PRECOMPILE_TIME_PBCFG_ACCESS */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/** \brief Access specific SwtPort Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_SwtPortType
 ** with index swtPortIdx in the Post-build config
 **
 ** \param[in] swtPortIdx Index of EthIf switch port
 */
#if (defined ETHIF_CFG_GET_SWTPORT_PTR)
#error ETHIF_CFG_GET_SWTPORT_PTR is already defined
#endif
#define ETHIF_CFG_GET_SWTPORT_PTR(swtPortIdx) \
    (&ETHIF_PBCFG_ACCESS(EthIf_SwtPortType, EthIf_SwtPort, swtPortIdx))

#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)

#if (defined ETHIF_ETHIFPORTIDX_ETHSWTIDX)
#error ETHIF_ETHIFPORTIDX_ETHSWTIDX is already defined
#endif
/* macro providing the switch index (in the context of EthSwt driver), out of EthIf port index. */
#define ETHIF_ETHIFPORTIDX_ETHSWTIDX(EthIfPortIdx) \
    (ETHIF_CFG_GET_SWTPORT_PTR(EthIfPortIdx)->EthSwtIdx)

#if (defined ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX)
#error ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX is already defined
#endif
/* macro providing the switch port index (in the context of EthSwt driver), out of EthIf port index. */
#define ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(EthIfPortIdx) \
    (ETHIF_CFG_GET_SWTPORT_PTR(EthIfPortIdx)->EthSwtPortIdx)

#if (defined ETHIF_ETHIFPORTIDX_PORTROLE)
#error ETHIF_ETHIFPORTIDX_PORTROLE is already defined
#endif
/* macro providing the port role of the port specified with port index. */
#define ETHIF_ETHIFPORTIDX_PORTROLE(EthIfPortIdx) \
    (ETHIF_CFG_GET_SWTPORT_PTR(EthIfPortIdx)->EthSwtPortRole)

#if (defined ETHIF_ETHIFPORTIDX_ETHTRCVIDX)
#error ETHIF_ETHIFPORTIDX_ETHTRCVIDX is already defined
#endif
/* macro providing the EthTrcvIdx of the port specified with port index. */
#define ETHIF_ETHIFPORTIDX_ETHTRCVIDX(EthIfPortIdx) \
    (ETHIF_CFG_GET_SWTPORT_PTR(EthIfPortIdx)->EthTrcvIdx)

/** \brief Access specific SwtPortGroup Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_SwtPortGroupType
 ** with index swtPortGroupIdx in the Post-build config
 **
 ** \param[in] swtPortGroupIdx Index of EthIf switch port group
 */
#if (defined ETHIF_CFG_GET_SWTPORTGROUP_PTR)
#error ETHIF_CFG_GET_SWTPORTGROUP_PTR is already defined
#endif
#define ETHIF_CFG_GET_SWTPORTGROUP_PTR(swtPortGroupIdx) \
    (&ETHIF_PBCFG_ACCESS(EthIf_SwtPortGroupType, EthIf_SwtPortGroup, swtPortGroupIdx))

#if (defined ETHIF_GET_PORTGORUP_REFTYPE)
#error ETHIF_GET_PORTGORUP_REFTYPE is already defined
#endif
/* macro providing the reference semantics of a specified switch port group */
#define ETHIF_GET_PORTGORUP_REFTYPE(EthIfPortGroupIdx) \
    (ETHIF_CFG_GET_SWTPORTGROUP_PTR(EthIfPortGroupIdx)->RefType)

#if (defined ETHIF_GET_PORTGROUP_BEGINIDX)
#error ETHIF_GET_PORTGROUP_BEGINIDX is already defined
#endif
/* macro providing the begin index in a PortInPortGroup list for a specified switch port group */
#define ETHIF_GET_PORTGROUP_BEGINIDX(EthIfPortGroupIdx) \
    (ETHIF_CFG_GET_SWTPORTGROUP_PTR(EthIfPortGroupIdx)->BeginIdx)

#if (defined ETHIF_GET_PORTGORUP_NUMPORTS)
#error ETHIF_GET_PORTGORUP_NUMPORTS is already defined
#endif
/* macro providing the number of ports in a specified switch port group */
#define ETHIF_GET_PORTGORUP_NUMPORTS(EthIfPortGroupIdx) \
    (ETHIF_CFG_GET_SWTPORTGROUP_PTR(EthIfPortGroupIdx)->NumPorts)

/** \brief Access specific PortsInPortGroup Post-Build ROM config
 **
 ** This macro resolves to an element of EthIf_PortsInPortGroup
 ** with index portsInPortGroupIdx in the Post-build config
 **
 ** \param[in] portsInPortGroupIdx Index in PortsInPortGroup list
 */
#if (defined ETHIF_CFG_ETHIFPORTIDX)
#error ETHIF_CFG_ETHIFPORTIDX is already defined
#endif
#define ETHIF_CFG_ETHIFPORTIDX(portsInPortGroupIdx)  \
    (ETHIF_PBCFG_ACCESS(uint8, EthIf_PortsInPortGroup, portsInPortGroupIdx))

#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

/** \brief Access specific EthIf controller Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_CtrlType
 ** with index ctrlIdx in the Post-build config
 **
 ** \param[in] ctrlIdx Index of EthIf controller
 */
#if (defined ETHIF_CFG_GET_CTRL_PTR)
#error ETHIF_CFG_GET_CTRL_PTR is already defined
#endif
#define ETHIF_CFG_GET_CTRL_PTR(ctrlIdx) \
    (&ETHIF_PBCFG_ACCESS(EthIf_CtrlType, EthIf_EthIfCtrl, ctrlIdx))

/** \brief Access specific EthIf PhysController Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_PhysCtrlType
 **
 ** \param[in] PhysControllerIdx Index of EthIf PhysController
 */
#if (defined ETHIF_CFG_GET_PHYSCTRL_PTR)
#error ETHIF_CFG_GET_PHYSCTRL_PTR is already defined
#endif
#define ETHIF_CFG_GET_PHYSCTRL_PTR(PhysControllerIdx) \
    (&ETHIF_PBCFG_ACCESS(EthIf_PhysCtrlType, EthIf_PhysCtrl, PhysControllerIdx))

#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
    (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
/** \brief Access specific EthIf physical controller ingress fifo Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_PhysCtrlIngrFifoType
 ** with index EthIfPhysCtrlIngrFifoIdx in the Post-build config
 **
 ** \param[in] EthIfPhysCtrlIngrFifoIdx Index of EthIf physical controller ingress FIFO
 */
#if (defined ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR)
#error ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR is already defined
#endif
#define ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(EthIfPhysCtrlIngrFifoIdx)                            \
  (&ETHIF_PBCFG_ACCESS(EthIf_PhysCtrlIngrFifoType,                                                \
                       EthIf_EthIfPhysCtrlIngrFifo, EthIfPhysCtrlIngrFifoIdx))

/** \brief Access specific EthIf physical controller map data
 **
 ** This macro resolves to a pointer directing to the element of EthIf_PhysCtrlEthIfCtrlMapDataType
 ** with index PhysCtrlIdx in the Post-build config
 **
 ** \param[in] PhysCtrlIdx Index of EthIf physical controller
 */
#if (defined ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR)
#error ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR is already defined
#endif
#define ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(PhysCtrlIdx)                                          \
  (&ETHIF_PBCFG_ACCESS(EthIf_PhysCtrlEthIfCtrlMapDataType,                                        \
                       EthIf_PhysCtrlEthIfCtrlMapData, PhysCtrlIdx))

/** \brief Access specific EthIf EthIfCtrlIdx mapped to physical controller
 **
 ** This macro resolves to a pointer directing to the element of uint8
 ** with index EthIfCtrlIdx in the Post-build config
 **
 ** \param[in] EthIfCtrlIdx Index of EthIf controller
 */
#if (defined ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP)
#error ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP is already defined
#endif
#define ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP(EthIfCtrlIdx)                                     \
  (ETHIF_PBCFG_ACCESS(uint8, EthIf_MapPhysCtrlEthIfCtrlIdx, EthIfCtrlIdx))

#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
          (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */
/*===[other macros]===*/

/** \brief Translate Ethernet Controller Index to Ethernet Transceiver Index
 **
 ** This macro translates the given Ethernet Controller Index to the related
 ** Ethernet Transceiver Index according to the Post-build configuration
 **
 ** \param[in] ctrlIdx Index of EthIf controller
 */
#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
#if (defined ETHIF_CFG_GET_ETHIFTRCV_IDX)
#error ETHIF_CFG_GET_ETHIFTRCV_IDX is already defined
#endif
#define ETHIF_CFG_GET_ETHIFTRCV_IDX(ctrlIdx) \
    (ETHIF_PBCFG_ACCESS(uint8, EthIf_EthIfTrcvIdx, ctrlIdx))

/** \brief Access specific EthIf Transceiver Post-Build ROM config
 **
 ** This macro resolves to a pointer directing to the element of EthIf_TrcvType
 **
 ** \param[in] EthIfTransciverIdx Index of EthIf Transceiver
 */
#if (defined ETHIF_CFG_GET_TRANSCEIVER_PTR)
#error ETHIF_CFG_GET_TRANSCEIVER_PTR is already defined
#endif
#define ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTransciverIdx) \
    (&ETHIF_PBCFG_ACCESS(EthIf_TrcvType, EthIf_EthIfTrcvEthTrcvIdxMap, EthIfTransciverIdx))

#endif /* (ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) */

#if (defined ETHIF_CFG_GET_SWT_IDX)
#error ETHIF_CFG_GET_SWT_IDX is already defined
#endif
#define ETHIF_CFG_GET_SWT_IDX(swtIdx) \
    (ETHIF_PBCFG_ACCESS(uint8, EthIf_EthIfSwt, swtIdx))

/* macros for ethernet header field offsets */
#if (defined ETHIF_ETH_DESTMAC_OFFSET)
#error ETHIF_ETH_DESTMAC_OFFSET is already defined
#endif
#define ETHIF_ETH_DESTMAC_OFFSET 0U                       /* ethernet header offset containing destination MAC address */

#if (defined ETHIF_ETH_SRCMAC_OFFSET)
#error ETHIF_ETH_SRCMAC_OFFSET is already defined
#endif
#define ETHIF_ETH_SRCMAC_OFFSET 6U                        /* ethernet header offset containing source MAC address */

#if (defined ETHIF_ETH_TYPE_OFFSET)
#error ETHIF_ETH_TYPE_OFFSET is already defined
#endif
#define ETHIF_ETH_TYPE_OFFSET 12U                         /* ethernet header offset containing Frame Type */

#if (defined ETHIF_ETH_HEADER_LENGTH)
#error ETHIF_ETH_HEADER_LENGTH is already defined
#endif
#define ETHIF_ETH_HEADER_LENGTH 14U                       /* total ethernet header length */

#if (defined ETHIF_ETH_PAYLOAD_OFFSET)
#error ETHIF_ETH_PAYLOAD_OFFSET is already defined
#endif
#define ETHIF_ETH_PAYLOAD_OFFSET ETHIF_ETH_HEADER_LENGTH  /* ethernet payload offset */

#if (defined ETHIF_VLAN_TPID)
#error ETHIF_VLAN_TPID is already defined
#endif
/* frame type for VLAN */
#define ETHIF_VLAN_TPID 0x8100U

#if ETHIF_VLAN_SUPPORT_ENABLE == STD_ON
/* macros for ethernet header field offsets if VLAN tagging is used */

#if (defined ETHIF_ETH_VLAN_TCI_OFFSET)
#error ETHIF_ETH_VLAN_TCI_OFFSET is already defined
#endif
#define ETHIF_ETH_VLAN_TCI_OFFSET 0U                     /* ethernet header offset containing VLAN TCI field */

#if (defined ETHIF_ETH_VLAN_TYPE_OFFSET)
#error ETHIF_ETH_VLAN_TYPE_OFFSET is already defined
#endif
#define ETHIF_ETH_VLAN_TYPE_OFFSET 2U                    /* ethernet header offset containing Frame Type if VLAN tagging is inserted */

#if (defined ETHIF_ETH_VLAN_HEADER_LENGTH)
#error ETHIF_ETH_VLAN_HEADER_LENGTH is already defined
#endif
#define ETHIF_ETH_VLAN_HEADER_LENGTH 4U                  /* total ethernet header length if VLAN tagging is inserted */

#if (defined ETHIF_ETH_VLAN_PAYLOAD_OFFSET)
#error ETHIF_ETH_VLAN_PAYLOAD_OFFSET is already defined
#endif
#define ETHIF_ETH_VLAN_PAYLOAD_OFFSET ETHIF_ETH_VLAN_HEADER_LENGTH  /* ethernet payload offset if VLAN tagging is inserted*/

#if (defined ETHIF_VLAN_TPID_CHECK)
#error ETHIF_VLAN_TPID_CHECK is already defined
#endif
/* macro checks if a valid TPID is located at the given address */
#define ETHIF_VLAN_TPID_CHECK(value) ((value) == 0x8100U)

#if (defined ETHIF_VLAN_TCI_SET)
#error ETHIF_VLAN_TCI_SET is already defined
#endif
/* macro sets a valid TCI to the given address */
#define ETHIF_VLAN_TCI_SET(addr,UP,VID)                                                 \
        do {                                                                            \
          const uint16 TCI = (uint16)((uint16)(((uint16)(UP))<<13U)|((uint16)(VID)));   \
          (((uint8*)(addr))[0U]) = (uint8)(TCI>>8U);                                    \
          (((uint8*)(addr))[1U]) = (uint8)TCI;                                          \
        }while(0U)

#if (defined ETHIF_VLAN_VID_GET)
#error ETHIF_VLAN_VID_GET is already defined
#endif
/* macro returns a VID (12 bit) from a given address */
#define ETHIF_VLAN_VID_GET(addr)                                                        \
          ((uint16)((uint16)(((uint16)(((uint16)(((uint8*)(addr))[0U]))&0x0FU))<<8U)|   \
           (uint16)(((uint16)(((uint16)(((uint8*)(addr))[1U]))&0xFFU))<<0U)))           \

#if (defined ETHIF_VLAN_TPID_TCI_LENGTH)
#error ETHIF_VLAN_TPID_TCI_LENGTH is already defined
#endif
/* length of fields TPID and TCI in unit of bytes */
#define ETHIF_VLAN_TPID_TCI_LENGTH 4U

#endif /* ETHIF_VLAN_SUPPORT_ENABLE */

#if (defined ETHIF_ETH_TYPE_SET)
#error ETHIF_ETH_TYPE_SET is already defined
#endif
/* macro sets a valid frame type to the given address */
#define ETHIF_ETH_TYPE_SET(addr,type)                                                   \
        do {                                                                            \
          (((uint8*)(addr))[0U]) = (uint8)(((uint16)(type))>>8U);                       \
          (((uint8*)(addr))[1U]) = (uint8)(type);                                       \
        }while(0U)

#if (defined ETHIF_ETH_TYPE_GET)
#error ETHIF_ETH_TYPE_GET is already defined
#endif
/* macro returns a frame type from a given address */
#define ETHIF_ETH_TYPE_GET(addr)                                                        \
          ((uint16)((uint16)(((uint16)(((uint16)(((uint8*)(addr))[0U]))&0xFFU))<<8U)|   \
           (uint16)(((uint16)(((uint16)(((uint8*)(addr))[1U]))&0xFFU))<<0U)))           \

/* define function like macro for development error reporting,
 * if development error detection is enabled */
#if(ETHIF_DEV_ERROR_DETECT == STD_ON)

#if (defined ETHIF_DET_REPORT_ERROR)
#error ETHIF_DET_REPORT_ERROR already defined!
#endif
#define ETHIF_DET_REPORT_ERROR(ApiId,ErrorId)    \
    (void) Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID, (ApiId), (ErrorId))
#else
#define ETHIF_DET_REPORT_ERROR(ApiId,ErrorId)
#endif /* if (ETHIF_DEV_ERROR_DETECT == STD_ON) */

#if (defined ETHIF_PHYS_CTRLIDX)
#error ETHIF_PHYS_CTRLIDX is already defined
#endif
/* macro providing PhysCtrlIdx index for the specified Ethernet interface controller */
#define ETHIF_PHYS_CTRLIDX(CtrlIdx) (ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->PhysCtrlIdx)

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/* macro providing the EthIf port group index for the specified Ethernet interface controller */
#define ETHIF_PORTGROUPIDX(CtrlIdx) (ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->EthIfPortGroupIdx)
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
#if (defined ETHIF_CTRLIDX_ETHIFTRCVIDX)
#error ETHIF_CTRLIDX_ETHIFTRCVIDX is already defined
#endif
/* macro providing the current EthIfTrcv transceiver index */
#define ETHIF_CTRLIDX_ETHIFTRCVIDX(CtrlIdx) (ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->EthIfTrcvIdx)
#endif /* ETHIF_TRCV_SUPPORT_ENABLE */

#if (defined ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX)
#error ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX is already defined
#endif
/* macro providing Eth controller index from EthCtrl config */
#define ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX(PhysControllerIdx) \
(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysControllerIdx)->EthCtrlIdx)

#if (defined ETHIF_ETHTRCV_TRCVMIIIDX)
#error ETHIF_ETHTRCV_TRCVMIIIDX is already defined
#endif
/* macro providing the current EthTrcv transceiver Mii index */
#define ETHIF_ETHTRCV_TRCVMIIIDX(CtrlIdx) (ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->EthTrcvMiiIdx)

#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
    (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))

#if (defined ETHIF_NUM_PRIO_INGR_FIFO)
#error ETHIF_NUM_PRIO_INGR_FIFO is already defined
#endif
#define ETHIF_NUM_PRIO_INGR_FIFO (EthIf_RootPtr->NumMappedPrioIngrFifo)

#if (defined ETHIF_NUM_TOTAL_INGR_FIFO)
#error ETHIF_NUM_TOTAL_INGR_FIFO is already defined
#endif
#define ETHIF_NUM_TOTAL_INGR_FIFO (EthIf_RootPtr->NumIngrFifoTotal)
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
          (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */

/* Timer macros */
#define SET_TIMER_REPEAT_SETMODE(Timer) \
            (TS_AtomicAssign16(Timer, ETHIF_MAX(ETHIF_SET_MODE_TIMEOUT, 1U)))

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
#define SET_SWITCHOFF_TIMER(EthIfPortIdx, Timeout) \
            (TS_AtomicAssign16(EthIf_PortState[(EthIfPortIdx)].SwitchOffTimer, (Timeout)))
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#define ELAPSE_TIMER(Timer) (TS_AtomicAssign16(Timer, 0U))

/* Returns TRUE/FALSE weather a timer elapsed or not */
#define CHECK_TIMER(Timer) ((Timer) == 0U)

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/* Checks if SWITCHOFF timer elapsed for a specified EthIfPortIdx. Precondition is that the port
 * is in state ACTIVE. */
#define CHECK_SWITCHOFF_TIMER(EthIfPortIdx)                    \
  ((EthIf_PortState[(EthIfPortIdx)].SwitchOffTimer == 0U) &&     \
   (EthIf_PortState[(EthIfPortIdx)].Mode == ETH_MODE_DOWN))
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if (defined ETHIF_MAC_ADDR_LEN)
#error ETHIF_MAC_ADDR_LEN is already defined
#endif

/* macro providing Ethernet frame MAC address size */
#define ETHIF_MAC_ADDR_LEN  6U

#define ETH2TRCV_MODE(EthMode) (((EthMode) == ETH_MODE_ACTIVE) ? \
                                 ETHTRCV_MODE_ACTIVE : ETHTRCV_MODE_DOWN)

#define TRCV2ETH_MODE(TrcvMode) (((TrcvMode) == ETHTRCV_MODE_ACTIVE) ? \
                                 ETH_MODE_ACTIVE : ETH_MODE_DOWN)

#define ETHIF_MAX(a, b) (((a)>(b))?(a):(b))

#define ETHIF_MIN(a, b) (((a)<(b))?(a):(b))


/*==================[type definitions]======================================*/

/** \brief Defined type for pointer to pointer type definition without violating Misra rule 20.7
 **/
typedef P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA) EthIf_EthDataTypePtr;

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
typedef boolean EthIf_TrcvLockListType[ETHIF_TRCV_ARRAYSIZE];
#else
typedef boolean EthIf_TrcvLockListType[1U];
#endif /* (ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) */

#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
typedef boolean EthIf_EthSwtPortLockListType[ETHIF_PORTS_ARRAYSIZE];
#else
typedef boolean EthIf_EthSwtPortLockListType[1U];
#endif /* ETHIF_SWT_SUPPORT_ENABLE == STD_ON */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>

/**
 * \brief Function to initialize all configured physical Ethernet controllers
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_InitControllers(void);

/**
 * \brief Function sets the mode for physical Ethernet controller object
 *
 * If a call to Ethernet driver was not successful it will be repeated in a MainFunction().
 *
 * \param[in] PhysCtrlIdx - Physical Controller index of this EthIfController
 * \param[in] ReqMode - Required mode to be set
 * \param[out] NewState - ModeState that is returned to a calling object (EthIfCtrl)
 * \return Std_ReturnType
 */
STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyCtrlMode
(
  uint8 PhysCtrlIdx,
  Eth_ModeType ReqMode,
  P2VAR(EthIf_ModeStateType, AUTOMATIC, ETHIF_APPL_DATA) NewState
);

/**
 * \brief Function updates ModeState for all PhyEthCtrls that are in transitional state
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdatePhyCtrlState(void);

#if(ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_OFF)
/**
 * \brief Function to update mode state and transceiver link state of EthIf objects
 *
 * Function tries to bring each hardware object that is in transitional (REQUESTED) state into
 * a stable state by calling respective set mode functions.
 * For active objects it reads-out / calculates transceiver link state and reports it
 * to upper layers (State Manager or BswM).
*/
STATIC FUNC(void, ETHIF_CODE) EthIf_MainFunctionState(void);
#endif /* #if(ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_OFF) */

/**
 * \brief For each EthIfController retrieves and reports link state of an underlying object
 *
 * For each EthIfController the function retrieves link state of an underlying object,
 * and report the change to SM and BswM.
 * The function requires referenced ports to be locked prior calling.
 * \param[in] EthSwtPortLockList - List of locked ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateEthIfCtrlLinkState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function sets the mode for a switch port group
 *
 * The function forwards mode set operation to each of ports within a group. If some of port
 * set operations were not successful they will be repeated within next MainFunctionState().
 * The function will lock port transceivers prior set operation.

 * \param[in] EthIfCtrlIdx   Ethernet controller index in the context of EthIf
 * \param[in] CtrlMode       Mode to be set
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_SetEthIfCtrlMode
(
  uint8 EthIfCtrlIdx,
  Eth_ModeType CtrlMode
);

/**
 * \brief EthIf_GetFrameTypeOwner() to get the configured frame type owner for requested frame type.
 *
 * \param[in] FrameType - Frame type which shall be assigned to a frame type owner.
 *
 * \return Frame type owner. This index shall be used call the corresponding generic upper layer
 *         callback function (UL_RxIndication() or UL_TxConfirmation()).
 */
STATIC FUNC(uint8, ETHIF_CODE) EthIf_GetFrameTypeOwner
(
  Eth_FrameType FrameType
);

/**
 * \brief Function to update reference counters for various EthIf objects
 *
 * \param[inout] RefCnt - A reference counter that needs to be updated
 * \param[out] NewMode - New mode calculated upon updated counter value
 * \param[in] ReqMode - A requested mode in set operation

 * \return TRUE/FALSE - Underlying hardware needs/needs not to be updated.
 */
#if(ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(boolean, ETHIF_CODE) UpdateRefCnt
(
  P2VAR(EthIf_RefCntType, AUTOMATIC, ETHIF_APPL_DATA) RefCnt,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode,
  Eth_ModeType ReqMode
);
#else
STATIC FUNC(boolean, ETHIF_CODE) UpdateRefCnt
(
  P2CONST(EthIf_RefCntType, AUTOMATIC, ETHIF_APPL_DATA) RefCnt,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode,
  Eth_ModeType ReqMode
);
#endif /* ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
/**
 * \brief Function sets the mode of Ethernet transceiver object
 *
 * If a call to transceiver driver was not successful it will be repeated in a MainFunction().
 *
 * \param[in] EthIfTrcvIdx - Transceiver index in the context of Ethernet interface
 * \param[in] ReqMode - Required mode to be set
 * \param[in] LockTrcv - Determines if a transceiver needs to be lock prior set operation
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_SetTrcvMode
(
  uint8 EthIfTrcvIdx,
  Eth_ModeType ReqMode,
  boolean LockTrcv
);

/**
 * \brief The function obtains transceiver mode.
 *
 * Transceiver locking (MII protection) needs to be handled prior calling this function.
 *
 * \param[in] EthIfTrcvIdx - Index of the Ethernet transceiver in the context of EthIf
 * \param[out] Mode - New transceiver link state
 *
 * \return E_OK - Returned transceiver mode is valid.
 *         E_NOT_OK - Returned transceiver mode is not valid.
 */
STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTrcvMode
(
  uint8 EthIfTrcvIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode
);

/**
 * \brief EthIf_GetLinkState() to get the link state of the transceiver.
 *
 * This function shall be used to obtain the link state of the indexed transceiver.
 * Transceiver locking (MII protection) needs to be handled prior calling this function.
 *
 * \param[in] EthIfTrcvIdx - Index of the Ethernet transceiver in the context of EthIf
 * \param[out] TrcvLinkStatePtr - new transceiver link state
 *
 * \return E_OK - Returned link state was retrieved and shall be processed.
 *         E_NOT_OK - Returned link state shall not be processed.
 */
STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetLinkState
(
  uint8 EthIfTrcvIdx,
  P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHIF_APPL_DATA) TrcvLinkStatePtr
);

/**
 * \brief Function finds out which transceuvers out of EthIfTransceivers container need to be locked
 * in the MainFunctionState()
 *
 * \param[out] TrcvLockList - List of transceivers that need to be locked
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_FindEthIfTrcvAccess
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
);

/**
 * \brief Function to initialize all configured Ethernet interface transceivers
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_InitTransceivers(void);

/**
 * \brief Function updates transceiver mode and link state
 *
 * Function updates mode state for Ethernet interface transceivers that are in REQUESTED state
 * and link state for active transceivers.
 * The function requires transceivers to be locked prior calling.
 *
 * \param[in] TrcvLockList - List of locked transceivers
 */
STATIC FUNC(void, ETHIF_CODE)  EthIf_UpdateEthIfTrcvState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
);

#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */



#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/**
 * \brief Function sets the mode for a switch port group
 *
 * The function forwards mode set operation to each of ports within a group. If some of port
 * set operations were not successful they will be repeated within next MainFunctionState().
 * The function will lock port transceivers prior set operation.

 * \param[in] PortGroupIdx   Port group index within a context of the EthIf
 * \param[in] ReqMode        Mode to set the port group to
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_SetPortGroupMode
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  Eth_ModeType ReqMode
);

/**
 * \brief Function checks if all ports of a port group are DOWN
 *
 * \param[in] PortGroupIdx       Port group index within a context of the EthIf
 * \returns TRUE  - all ports of a port group are DOWN
 *          FALSE - at least one port of a port group is not DOWN
 */
STATIC FUNC(boolean, ETHIF_CODE) EthIf_CheckAllPortsDown
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx
);

/**
 * \brief Function sets the mode for a switch port
 *
 * The function forwards mode set operation to Ethernet switch driver. If driver operation is
 * not successful it will be repeated in the following MainFunctionState() call. DOWN transition
 * will be delayed for the value of ShutdownTimeout.
 * Function requires port to be locked prior calling, and EthSwtPortLockList is a list of
 * successfully locked Switch ports.
 *
 * \param[in] PortIdx             Port index within a context of the EthIf
 * \param[in] ReqMode             Mode to set the port to
 * \param[in] ShutdownTimeout     A time delay of set mode DOWN operation
 * \param[in] EthSwtPortLockList  List of Switch ports successfully locked
 * \returns Std_ReturnType
 */
STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPortMode
(
  EthIf_SwtPortIdxType PortIdx,
  Eth_ModeType ReqMode,
  uint16 ShutdownTimeout,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function updates ModeState for all EthIfControllers that are in transitional state
 *
 * For EthIfControllers that reference switch (port group), that are in REQUESTED state of DOWN
 * transition, function checks if all ports are down, prior proceeding with shutting down
 * physical controller.
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateEthIfCtrlState(void);

/**
 * \brief Function updates ModeState for all ports that are in transitional (REQUESTED) state
 *
 * Function performs delayed port switch-off operation (set with EthIf_SwitchPortGroupRequestMode())
 * or repeat set mode operation in case previous call was unsuccessful.
 *
 * \param[in] EthSwtPortLockList - List of locked Switch ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdatePortState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)
/**
 * \brief Function updates switch-off timer of port groups that are under control of BswM
 *
 * The function takes care of port locking, i.e. it is not necessary to lock ports prior the call.
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateBswMPortGroupSwitchOffTimer(void);

/**
 * \brief Function calculates and reports link state of port groups not referenced by EthIfCtrl
 *
 * The function requires port group ports to be locked prior calling.
 * \param[in] EthSwtPortLockList - List of locked ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateBswMPortGroupLinkState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);
#endif /* ETHIF_PORTGROUPSUPPORT == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/**
 * \brief Function retrieves port group link state
 *
 * In case link state is already calculated for the requested port group in a current MainFunction()
 * cycle - the function only returns the value. In case link state is not known, the function reads
 * out link state of underlying ports and calculates accumulated link state.
 * The function requires ports to be locked prior calling
 *
 * \param[in] PortGroupIdx - Port group index in the context of EthIf.
 * \param[in] EthSwtPortLockList - List of locked ports
 * \returns EthTrcv_LinkStateType - Link state of the requested port group
 */
STATIC FUNC(EthTrcv_LinkStateType, ETHIF_CODE) EthIf_GetPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function updates link state of a specified logical port group (switch)
 *
 * The function reads out link state of a host port and calculates accumulated link state
 * of a port group.
 * The function requires ports to be locked prior calling
 *
 * \param[in] PortGroupIdx - Port group index in the context of EthIf.
 * \param[in] EthSwtPortLockList - List of locked ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateLogicalPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function updates link state of a specified user defined port group
 *
 * The function reads out link state of underlying ports and calculates accumulated link state.
 * The function requires ports to be locked prior calling
 *
 * \param[in] PortGroupIdx - Port group index in the context of EthIf.
 * \param[in] EthSwtPortLockList - List of locked ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateUserPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function retrieves port link state
 *
 * In case link state was already retrieved for the requested port group in a current MainFunction()
 * cycle - the function only returns the value. In case link state is not known, the function reads
 * out link state from EthSwt driver by calling EthSwt_GetLinkState().
 * A protection mechanism is implemented to repeat port set mode operation if link state is DOWN
 * for user configurable time period.
 *
 * \param[in] PortIdx - Port index in the context of EthIf.
 * \param[in] EthSwtPortLockList - List of locked ports
 *
 * \returns EthTrcv_LinkStateType - Link state of the requested port group
 */
STATIC FUNC(EthTrcv_LinkStateType, ETHIF_CODE) EthIf_GetPortLinkState
(
  EthIf_SwtPortIdxType PortIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/**
 * \brief Finds out transceivers referenced by port group ports
 *
 * \param[out] EthSwtPortLockList - [in] List of locked ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_FindTrcvPortGroup
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Function finds out which Switch ports need to be locked in
 * MainFunctionState()
 *
 * \param[out] EthSwtPortLockList - List of ports that need to be locked
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_FindPortTrcvAccess
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
/**
 * \brief Locks listed transceivers (MII protection) that are not already locked
 *
 * \param[inout] TrcvLockList - [in] List of transceivers to be locked
 *                              [out] List of successfully locked transceivers
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_LockTransceivers
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
);

/**
 * \brief Unlocks listed transceivers
 *
 * \param[in] TrcvLockList - List of transceivers to be unlocked
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UnlockTransceivers
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
);
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
/**
 * \brief Locks listed Switch ports that are not already locked
 *
 * \param[inout] EthSwtPortLockList - [in] List of Ports to be locked
 *                                   [out] List of successfully locked Ports
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_LockSwitchPorts
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);

/**
 * \brief Unlocks listed Switch ports
 *
 * \param[in] EthSwtPortLockList - List of Ports to be unlocked
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_UnlockSwitchPorts
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
);
#endif /*ETHIF_SWT_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) || (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/**
 * \brief Function ticks down the SETMODE timer if EthIfSetModeTimeout is set
 *
 * \param[in] Timer - timer value
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_TickTimer
(
  P2VAR(EthIf_TimerType, AUTOMATIC, ETHIF_APPL_DATA) Timer
);
#endif /* (ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) || (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON) */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/**
 * \brief Function ticks down port SWITCHOFF timer
 *
 * \param[in] EthIfPortIdx - Port index
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_Tick_SwitchoffTimer
(
  EthIf_SwtPortIdxType EthIfPortIdx
);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */


/**
 * \brief Internal Rx indication function
 *
 * This function takes care for optional VLAN handling and tries to find suitable upper layer to
 * forward the received data.
 *
 * \param[in] CtrlIdx - Index of the controller within the context of the EthIf.
 * \param[in] FrameType - Ethernet Frame type.
 * \param[in] isBroadcast - Indicates whether Ethernet destination address is a broadcast
 *                          address or not.
 * \param[in] PhysAddrPtr - Ethernet source address.
 * \param[in] DataPtr - Address of the received payload.
 * \param[in] LenByte - Length of the payload contained in the received Rx buffer.
 * \returns boolean
 * \retval TRUE - Upper layer found
 * \retval FALSE - Upper layer not found
 *
 */
STATIC FUNC(boolean, ETHIF_CODE) EthIf_RxIndicationInternal
(
  uint8 EthIfCtrlIdx,
  Eth_FrameType FrameType,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
);

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/**
 * \brief Function increases Measurement data counter
 */
STATIC FUNC(void, ETHIF_CODE) EthIf_IncMeasurementCnt
(
  void
);
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define ETHIF_START_SEC_VAR_INIT_8
#include <EthIf_MemMap.h>

/** \brief Variable storing initialization status of EthIf. */
STATIC VAR(boolean, ETHIF_VAR) EthIf_Initialized = FALSE;

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/** \brief Variable storing current MainFunction cycle counter
 *   Used to avoid multiple objects update */
STATIC VAR(uint8, ETHIF_VAR) EthIf_MainFntCycleCnt = 0U;
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */


#define ETHIF_STOP_SEC_VAR_INIT_8
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_VAR_CLEARED_8
#include <EthIf_MemMap.h>

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
STATIC VAR(boolean, ETHIF_VAR) EthIf_IsMiiActive[ETHIF_TRCV_ARRAYSIZE];
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
STATIC VAR(boolean, ETHIF_VAR) EthIf_IsPortActive[ETHIF_PORTS_ARRAYSIZE];
#endif /* ETHIF_SWT_SUPPORT_ENABLE == STD_ON */

#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthIf_MemMap.h>

/** \brief Variable to manage state of Ethernet interface controllers */
STATIC VAR(EthIf_CtrlStateType, ETHIF_VAR) EthIf_CtrlState[ETHIF_ETHIFCTRL_ARRAYSIZE];

/** \brief Variable to manage state of physical Ethernet controllers */
STATIC VAR(EthIf_PhyCtrlStateType, ETHIF_VAR) EthIf_PhyCtrlState[ETHIF_PHYCTRL_ARRAYSIZE];

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
/** \brief Variable to manage state of Ethernet switch ports */
STATIC VAR(EthIf_PortStateType, ETHIF_VAR) EthIf_PortState[ETHIF_PORTS_ARRAYSIZE];

/** \brief Variable to manage state of Ethernet switch ports groups*/
STATIC VAR(EthIf_PortGroupStateType, ETHIF_VAR) EthIf_PortGroupState[ETHIF_PORTGROUPS_ARRAYSIZE];

/** \brief Variable to manage signal quality of Ethernet switch ports */
STATIC VAR (EthIf_SignalQualityResultType, ETHIF_VAR) EthIf_PortSignalQual[ETHIF_PORTS_ARRAYSIZE];
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
/** \brief Variable to manage state of Ethernet transceivers */
STATIC VAR(EthIf_TrcvStateType, ETHIF_VAR) EthIf_TrcvState[ETHIF_TRCV_ARRAYSIZE];

/** \brief Variable to manage state of Transceiver signal quality */
STATIC VAR (EthIf_SignalQualityResultType, ETHIF_VAR) EthIf_TrcvSignalQual[ETHIF_TRCV_ARRAYSIZE];
#endif

#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_VAR_INIT_UNSPECIFIED
#include <EthIf_MemMap.h>

/* variable storing pointer to post-build config structure */
STATIC P2CONST(EthIf_ConfigType, ETHIF_VAR, ETHIF_APPL_CONST) EthIf_RootPtr = NULL_PTR;

#define ETHIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_VAR_INIT_32
#include <EthIf_MemMap.h>

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
VAR(uint32, ETHIF_VAR) EthIf_MeasurementDropData = 0U;
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define ETHIF_STOP_SEC_VAR_INIT_32
#include <EthIf_MemMap.h>

/*==================[external function definitions]=========================*/
#if ETHIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHIF_START_SEC_CODE_CC_BLOCK
#else
#define ETHIF_START_SEC_CODE
#endif
#include <EthIf_MemMap.h>

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ETHIF_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration. */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_ETHIF_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform. */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
    {
      /* Validate the post build configuration against the compile time configuration. */
      if(ETHIF_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration. */
        if(EthIf_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the post build configuration against the compile time published information
             configuration. */
          if(ETHIF_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid. */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_ETHIF_ISVALIDCONFIG_EXIT(RetVal, voidConfigPtr);
  return RetVal;
}

#if ETHIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define ETHIF_STOP_SEC_CODE_CC_BLOCK
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>
#endif

FUNC(void, ETHIF_CODE) EthIf_Init
(
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_APPL_CONST) CfgPtr
)
{
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHSM_APPL_CONST) LocalConfigPtr = CfgPtr;

  DBG_ETHIF_INIT_ENTRY(CfgPtr);

#if (ETHIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager. */
  if(LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if(E_OK == PbcfgM_GetConfig(
                                 ETHIF_MODULE_ID,
                                 ETHIF_INSTANCE_ID,
                                 &ModuleConfig)
                               )
    {
      LocalConfigPtr = (P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* ETHIF_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(LocalConfigPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_INIT_SVCID, ETHIF_E_INIT_FAILED);
  }
  /* Check that configuration pointer is valid. */
  else if(E_OK != EthIf_IsValidConfig(LocalConfigPtr))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_INIT_SVCID, ETHIF_E_INIT_FAILED);
  }
  else
#endif
  {
    uint8 Idx;

    EthIf_RootPtr = LocalConfigPtr;

    /* Initialize the complete array of Ethernet interface controllers state variable */
    for(Idx = 0U; Idx < ETHIF_ETHIFCTRL_ARRAYSIZE; Idx++)
    {
      CONSTP2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState =
        &EthIf_CtrlState[Idx];

      pEthIf_CtrlState->Mode = ETH_MODE_DOWN;
      pEthIf_CtrlState->ModeState = ETHIF_MODESTATE_SET;
      pEthIf_CtrlState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
#if (ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
      pEthIf_CtrlState->Disabled = FALSE;
#endif
    }

    /* Initialize the complete array of physical Ethernet controllers state variable */
    for(Idx = 0U; Idx < ETHIF_PHYCTRL_ARRAYSIZE; Idx++)
    {
      CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
        &EthIf_PhyCtrlState[Idx];

      pEthIf_PhyCtrlState->IsInitialized = FALSE;
      pEthIf_PhyCtrlState->Mode = ETH_MODE_DOWN;
      pEthIf_PhyCtrlState->ModeState = ETHIF_MODESTATE_SET;
#if(ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON)
      pEthIf_PhyCtrlState->RefCnt = 0U;
#endif
      SET_TIMER_REPEAT_SETMODE(pEthIf_PhyCtrlState->SetRepeatTimer);
    }

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    /* Initialize the complete array of transceivers state variable */
    for(Idx = 0U; Idx < ETHIF_TRCV_ARRAYSIZE; Idx++)
    {
      CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
        &EthIf_TrcvState[Idx];
      CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvSignalQual =
        &EthIf_TrcvSignalQual[Idx];

      pEthIf_TrcvState->IsInitialized = FALSE;
      pEthIf_TrcvState->Mode = ETH_MODE_DOWN;
      pEthIf_TrcvState->ModeState = ETHIF_MODESTATE_SET;
      pEthIf_TrcvState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
      pEthIf_TrcvSignalQual->HighestSignalQuality = 0U;
      pEthIf_TrcvSignalQual->LowestSignalQuality = 0xFFFFFFFFU;
      pEthIf_TrcvSignalQual->ActualSignalQuality = 0U;
      SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
      pEthIf_TrcvState->TrcvWakeupMode = ETHTRCV_WUM_DISABLE;
#endif
    }
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    /* Initialize the complete array of switch port groups state variable */
    for(Idx = 0U; Idx < ETHIF_PORTGROUPS_ARRAYSIZE; Idx++)
    {
      CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortGroupState =
        &EthIf_PortGroupState[Idx];

      pEthIf_PortGroupState->SwitchOffTimer = 0U;
      pEthIf_PortGroupState->CycleCnt = 0xFFU;
      pEthIf_PortGroupState->Mode = ETH_MODE_DOWN;
      pEthIf_PortGroupState->ModeState = ETHIF_MODESTATE_SET;
      pEthIf_PortGroupState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
    }

    /* Initialize the complete array of switch port state variable */
    for(Idx = 0U; Idx < ETHIF_PORTS_ARRAYSIZE; Idx++)
    {
      CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
        &EthIf_PortState[Idx];
      CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortSignalQual =
        &EthIf_PortSignalQual[Idx];

      pEthIf_PortState->CycleCnt = 0xFFU;
      pEthIf_PortState->Mode = ETH_MODE_DOWN;
      pEthIf_PortState->ModeState = ETHIF_MODESTATE_SET;
      pEthIf_PortState->RefCnt = 0U;
      SET_TIMER_REPEAT_SETMODE(pEthIf_PortState->SetRepeatTimer);
      pEthIf_PortState->SwitchOffTimer = 0U;
      pEthIf_PortState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
      pEthIf_PortSignalQual->HighestSignalQuality = 0U;
      pEthIf_PortSignalQual->LowestSignalQuality = 0xFFFFFFFFU;
      pEthIf_PortSignalQual->ActualSignalQuality = 0U;
    }
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

    /* Initialize the complete array of MII busy variable */
#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    for(Idx = 0U; Idx < ETHIF_TRCV_ARRAYSIZE; Idx++)
    {
      EthIf_IsMiiActive[Idx] = FALSE;
    }
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

    /* Initialize all configured physical Ethernet controllers. */
    EthIf_InitControllers();

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    /* Initialize all configured Ethernet transceivers. */
    EthIf_InitTransceivers();
#endif

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    EthIf_MainFntCycleCnt = 0U;
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

    /* Initialization of EthIf successful */
    TS_AtomicAssign8(EthIf_Initialized, TRUE);

    /* avoid warning "variable  was set but never used" on some toolchains */
    TS_PARAM_UNUSED(EthIf_RootPtr);
  }

  DBG_ETHIF_INIT_EXIT(CfgPtr);
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetControllerMode
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETCONTROLLERMODE_ENTRY(CtrlIdx, CtrlMode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCONTROLLERMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCONTROLLERMODE_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if((ETH_MODE_DOWN != CtrlMode) && (ETH_MODE_ACTIVE != CtrlMode))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCONTROLLERMODE_SVCID, ETHIF_E_INV_PARAM);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    CONSTP2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState =
                &EthIf_CtrlState[CtrlIdx];
    RetVal = E_OK;

    /* For EthIfCtrl set request is ignored if the mode does not change. */
    if(pEthIf_CtrlState->Mode != CtrlMode)
    {
      EthIf_ModeStateType PhyCtrlModeState;
#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
      boolean ContinueSetPhyCtrlMode = TRUE;
      boolean InControl = FALSE;
      const EthIf_SwitchPortGroupIdxType PortGroupIdx = ETHIF_PORTGROUPIDX(CtrlIdx);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

      TS_AtomicAssign8(pEthIf_CtrlState->Mode, CtrlMode);
      TS_AtomicAssign8(pEthIf_CtrlState->ModeState, ETHIF_MODESTATE_REQUESTED);

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
      if(PortGroupIdx != ETHIF_INVALID_PORTGROUP_IDX)
      { /* switch as a logical port group is always under control of EthIfController */
        InControl = ((ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_CONTROL) ||
                     (ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_SWITCH));
      }

      /* EthIf must delay the shutdown of an EthIfPhysController referencing a switch or port (with
       * reference type CONTROL) group until all ports of the referenced switch are in state DOWN.
       * In case of ACTIVE transition - EthIfPhysController mode is set first. */
      if((CtrlMode == ETH_MODE_DOWN) && (InControl == TRUE))
      {
        EthIf_SetPortGroupMode(PortGroupIdx, CtrlMode);
        if(EthIf_CheckAllPortsDown(PortGroupIdx) == FALSE)
        {
          /* EthIfController is in REQUESTED state -> the check will be repeated in the next
           * MainFunctionState() call. */
          ContinueSetPhyCtrlMode = FALSE;
        }
      }
      if(ContinueSetPhyCtrlMode == TRUE)
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
      {
        /* ModeState of EthIfCtrl depends on a ModeState of EthPhyCtrl. */
        RetVal = EthIf_SetPhyCtrlMode(
                                       ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->PhysCtrlIdx,
                                       CtrlMode,
                                       &PhyCtrlModeState
                                     );

        /* At the moment EthIf simulates asynchronous communication between EthIf <-> EthDrv.
         * As a consequence EthIf sends indication to itself, which in consequence indicates SM.
         * Here we check if SM is already notified. */
        if((RetVal == E_OK) && (PhyCtrlModeState != pEthIf_CtrlState->ModeState))
        {
          EthSM_CtrlModeIndication(CtrlIdx, CtrlMode);
          EthIf_SetEthIfCtrlMode(CtrlIdx, CtrlMode);
        }
      }
#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
      else
      {
        EthSM_CtrlModeIndication(CtrlIdx, CtrlMode);
      }
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
    }
  }

  DBG_ETHIF_SETCONTROLLERMODE_EXIT(RetVal, CtrlIdx, CtrlMode);

  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetControllerMode
(
  uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) CtrlModePtr
)
{
  Std_ReturnType Result = E_NOT_OK;

  DBG_ETHIF_GETCONTROLLERMODE_ENTRY(CtrlIdx, CtrlModePtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if (EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCONTROLLERMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCONTROLLERMODE_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(CtrlModePtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCONTROLLERMODE_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(FALSE == EthIf_PhyCtrlState[ETHIF_PHYS_CTRLIDX(CtrlIdx)].IsInitialized)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCONTROLLERMODE_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);
    Result = ETHIF_ETH_GETCONTROLLERMODE(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), CtrlModePtr);
  }

  DBG_ETHIF_GETCONTROLLERMODE_EXIT(Result,CtrlIdx, CtrlModePtr);
  return Result;
}

FUNC(void, ETHIF_CODE) EthIf_GetPhysAddr
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
)
{
  DBG_ETHIF_GETPHYSADDR_ENTRY(CtrlIdx, PhysAddrPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYSADDR_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYSADDR_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYSADDR_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    ETHIF_ETH_GETPHYSADDR(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), PhysAddrPtr);
  }

  DBG_ETHIF_GETPHYSADDR_EXIT(CtrlIdx, PhysAddrPtr);
}

FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideTxBuffer
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHIF_APPL_DATA) BufIdxPtr,
  P2VAR(EthIf_Uint8TypePtr, AUTOMATIC, ETHIF_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) LenBytePtr
)
{
  BufReq_ReturnType Result = BUFREQ_E_NOT_OK;

  DBG_ETHIF_PROVIDETXBUFFER_ENTRY(CtrlIdx, FrameType, Priority, BufIdxPtr, BufPtr, LenBytePtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(BufIdxPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(BufPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(LenBytePtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_PARAM_POINTER);
  }
#if((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430) && \
    (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430_EB))
  else if(Priority > 7U)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_PROVIDETXBUFFER_SVCID, ETHIF_E_INV_PARAM);
  }
#endif /* (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430) &&
          (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430_EB) */
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    P2VAR(uint8,AUTOMATIC,ETHIF_APPL_DATA) LocalBufPtr;
    CONSTP2CONST(EthIf_PhysCtrlType, AUTOMATIC, ETHIF_APPL_DATA) PhysCtrlCfgPtr
      = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(CtrlIdx));

#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    /* get VID of this controller */
    const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->VID;
#endif /* ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF */
#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
    Eth_FrameType CurrentFrameType = FrameType;
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */
#if((ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF) && (ETHIF_ETHSWT_TX_ADAPTFRAME == STD_OFF))
    TS_PARAM_UNUSED(FrameType);
    TS_PARAM_UNUSED(Priority);
#endif /* (ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF && ETHIF_ETHSWT_TX_ADAPTFRAME == STD_OFF) */

#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    /* check if this is a VLAN controller */
    if(EthIfVID < ETHIF_VLAN_VID_UNUSED)
    {
      /* for adding the VLAN information, the requested payload length is increased by 4 */
      *LenBytePtr += ETHIF_VLAN_TPID_TCI_LENGTH;
      /* Set current frame type to VLAN type to be used in for possible prepended header. */
#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
      CurrentFrameType = ETHIF_VLAN_TPID;
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */
    }
#endif /* ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF */

#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
    {
      /* TmpFrameType shall be used to pass the FrameType to EthSwt. In this context the output
         of TmpFrameType shall not be used any further. */
      Eth_FrameType TmpFrameType = FrameType;
      ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN(CtrlIdx, &TmpFrameType, LenBytePtr);
    }
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */

    /* request transmit buffer from Eth */
#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
    (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
    Result = ETHIF_ETH_PROVIDETXBUFFER
      (
        PhysCtrlCfgPtr,
        Priority,
        BufIdxPtr,
        (P2VAR(EthIf_EthDataTypePtr, AUTOMATIC, ETHIF_APPL_DATA))&LocalBufPtr,
        LenBytePtr
      );
#else
    Result = Eth_ProvideTxBuffer
      (
        PhysCtrlCfgPtr->EthCtrlIdx,
        BufIdxPtr,
        (P2VAR(EthIf_EthDataTypePtr, AUTOMATIC, ETHIF_APPL_DATA))&LocalBufPtr,
        LenBytePtr
      );
#endif

    if(BUFREQ_OK == Result)
    {
#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
      (void) ETHIF_ETHSWT_PREPROCESSTXFRAME
             (
               CtrlIdx,
               CurrentFrameType,
               Priority,
               *BufIdxPtr,
               &LocalBufPtr,
               LenBytePtr
             );
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */
#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
      /* If buffer was granted, add VID, original frame type information and manipulate
         buffer information */
      if(EthIfVID < ETHIF_VLAN_VID_UNUSED)
      {
        /* set VID and priority */
        ETHIF_VLAN_TCI_SET(&LocalBufPtr[0U], Priority, EthIfVID);
        /* set Ethernet frame type */
        ETHIF_ETH_TYPE_SET(&LocalBufPtr[2U], FrameType);

        /* forward buffer pointer */
        LocalBufPtr = &LocalBufPtr[ETHIF_VLAN_TPID_TCI_LENGTH];

        /* return buffer length for upper layer - 4 */
        *LenBytePtr -= ETHIF_VLAN_TPID_TCI_LENGTH;
      }
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */
      *BufPtr = LocalBufPtr;
    }
  }

  DBG_ETHIF_PROVIDETXBUFFER_EXIT(Result, CtrlIdx, FrameType, Priority, BufIdxPtr, BufPtr, LenBytePtr);
  return Result;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Transmit
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  Eth_FrameType TmpFrameType = FrameType;
  uint16 TmpLenByte = LenByte;

  DBG_ETHIF_TRANSMIT_ENTRY(CtrlIdx, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr);


#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_TRANSMIT_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_TRANSMIT_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_TRANSMIT_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    {
      /* get VID of this controller */
      const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->VID;

      /* check if this is a VLAN controller */
      if(EthIfVID < ETHIF_VLAN_VID_UNUSED)
      {
        /* for adding the VLAN information, the transmitted payload length is increased by 4 */
        /* write TPID, TIC */
        TmpFrameType = ETHIF_VLAN_TPID;
        TmpLenByte += ETHIF_VLAN_TPID_TCI_LENGTH;
      }
    }
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */

#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
    {
      /* TmpFrameType shall be used to pass the FrameType to EthSwt. In this context the output
         of TmpFrameType shall not be used any further. */
      ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN(CtrlIdx, &TmpFrameType, &TmpLenByte);
    }
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */

    Result = ETHIF_ETH_TRANSMIT
               (
                 ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx),
                 BufIdx,
                 TmpFrameType,
                 TxConfirmation,
                 TmpLenByte,
                 PhysAddrPtr
               );
  }

  DBG_ETHIF_TRANSMIT_EXIT(Result, CtrlIdx, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr);
  return Result;
}

#if (ETHIF_RETRANSMIT_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Retransmit
(
  uint8 CtrlIdx,
  uint8 OrigCtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHIF_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  Eth_FrameType FrameType,
  uint16 LenByte,
  CONSTP2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETHIF_APPL_DATA) RetransmitInfoPtr
)
{
  Std_ReturnType Result = E_NOT_OK;
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) TmpDataPtr = DataPtr;
  Eth_FrameType TmpFrameType = FrameType;
  uint16 TmpLenByte = LenByte;
  boolean ComptbEthIfCtrls = FALSE;

  DBG_ETHIF_RETRANSMIT_ENTRY(CtrlIdx, OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, LenByte,
                             RetransmitInfoPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(OrigCtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(DataPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(RetransmitInfoPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(BufIdxPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RETRANSMIT_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {

#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    {
      uint8 Priority;

      /* get VID of the controller */
      const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->VID;
      const uint16 SrcEthIfVID = ETHIF_CFG_GET_CTRL_PTR(OrigCtrlIdx)->VID;

      /* check if this is a VLAN controller */
      if(EthIfVID < ETHIF_VLAN_VID_UNUSED)
      {
        /* make sure previously there was also a vlan tag - so buffer size is correct */
        if (SrcEthIfVID < ETHIF_VLAN_VID_UNUSED)
        {
          ComptbEthIfCtrls = TRUE;

          /* add VLAN header space to passed buffer */
          TmpDataPtr = &TmpDataPtr[-1*((sint8)ETHIF_VLAN_TPID_TCI_LENGTH)];
          if(RetransmitInfoPtr->Priority != 0xFFU)
          {
            /* set new VID and priority */
            ETHIF_VLAN_TCI_SET(&TmpDataPtr[0U], RetransmitInfoPtr->Priority, EthIfVID);
          }
          else
          {
            /* keep priority from previous frame */
            Priority = TmpDataPtr[0U]>>5U;
            ETHIF_VLAN_TCI_SET(&TmpDataPtr[0U], Priority,  EthIfVID);
          }

          /* set Ethernet frame type */
          ETHIF_ETH_TYPE_SET(&TmpDataPtr[2U], FrameType);

          /* for adding the VLAN information, the transmitted payload length is increased by 4 */
          /* write TPID, TIC */
          TmpFrameType = ETHIF_VLAN_TPID;
          TmpLenByte += ETHIF_VLAN_TPID_TCI_LENGTH;
        }
        else
        {
          /* non compatible EthIf controllers */
        }
      }
      else
      {
        if (SrcEthIfVID < ETHIF_VLAN_VID_UNUSED)
        {
          /* non compatible EthIf controllers */
        }
        else
        {
          ComptbEthIfCtrls = TRUE;
        }
      }
    }
#else
    ComptbEthIfCtrls = TRUE;
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */

#if(ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON)
    {
      if(ComptbEthIfCtrls)
      {
        /* TmpFrameType shall be used to pass the FrameType to EthSwt. In this context the output
           of TmpFrameType shall not be used any further. */
        ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN(CtrlIdx, &TmpFrameType, &TmpLenByte);
      }
    }
#endif /* ETHIF_ETHSWT_TX_ADAPTFRAME == STD_ON */

    if(ComptbEthIfCtrls)
    {
      CONSTP2CONST(EthIf_PhysCtrlType, AUTOMATIC, ETHIF_APPL_DATA) PhysCtrlCfgPtr
        = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(CtrlIdx));
      CONSTP2CONST(EthIf_PhysCtrlType, AUTOMATIC, ETHIF_APPL_DATA) OriginPhysCtrlCfgPtr
        = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(OrigCtrlIdx));

      Result = ETHIF_ETH_RETRANSMIT
                 (
                   PhysCtrlCfgPtr,
                   OriginPhysCtrlCfgPtr->EthCtrlIdx,
                   BufIdxPtr,
                   TmpDataPtr,
                   TmpFrameType,
                   TmpLenByte,
                   RetransmitInfoPtr
                 );
    }
  }

  DBG_ETHIF_RETRANSMIT_EXIT(Result, CtrlIdx, OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, LenByte,
                            RetransmitInfoPtr);
  return Result;
}
#endif /* ETHIF_RETRANSMIT_API == STD_ON */

#if(ETHIF_GET_VERSION_INFO == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHIF_APPL_DATA) VersionInfoPtr
)
{
  DBG_ETHIF_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(VersionInfoPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETVERSIONINFO_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = ETHIF_VENDOR_ID;
    VersionInfoPtr->moduleID         = ETHIF_MODULE_ID;
    VersionInfoPtr->sw_major_version = ETHIF_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = ETHIF_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = ETHIF_SW_PATCH_VERSION;

  }

  DBG_ETHIF_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

FUNC(void, ETHIF_CODE) EthIf_SetPhysAddr
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr
)
{
  DBG_ETHIF_SETPHYSADDR_ENTRY(CtrlIdx, PhysAddrPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYSADDR_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYSADDR_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYSADDR_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
#if(ETHIF_ETH_SETPHYSADDR_API_ENABLE == STD_ON)
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    ETHIF_ETH_SETPHYADDR(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), PhysAddrPtr);
#else /* ETHIF_ETH_SETPHYSADDR_API_ENABLE */
    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(PhysAddrPtr);
#endif /* ETHIF_ETH_SETPHYSADDR_API_ENABLE */
  }

  DBG_ETHIF_SETPHYSADDR_EXIT(CtrlIdx, PhysAddrPtr);
} /* eof - EthIf_SetPhysAddr */

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_UPDATEPHYSADDRFILTER_ENTRY(CtrlIdx, PhysAddrPtr, Action);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_UPDATEPHYSADDRFILTER_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_UPDATEPHYSADDRFILTER_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_UPDATEPHYSADDRFILTER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {

#if(ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE == STD_ON)
    uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    RetVal = ETHIF_ETH_UPDATEPHYADDRFILTER
             (ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), PhysAddrPtr, Action);
#else /* ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE */
    TS_PARAM_UNUSED(CtrlIdx);
    TS_PARAM_UNUSED(PhysAddrPtr);
    TS_PARAM_UNUSED(Action);
    RetVal = E_OK;
#endif /* ETHIF_ETH_UPDATEPHYSADDRFILTER_API_ENABLE */
  }

  DBG_ETHIF_UPDATEPHYSADDRFILTER_EXIT(RetVal, CtrlIdx, PhysAddrPtr, Action);

  return RetVal;
} /* eof - EthIf_UpdatePhysAddrFilter */

FUNC(void, ETHIF_CODE) EthIf_Cbk_RxIndication
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  DBG_ETHIF_CBK_RXINDICATION_ENTRY(CtrlIdx, DataPtr, LenByte);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)

  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->PhyCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(DataPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(LenByte < ETHIF_ETH_HEADER_LENGTH)
  {
     ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_INV_PARAM);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {

    /* cast DataPtr to a uint8* type */
    CONSTP2VAR(uint8,AUTOMATIC,ETHIF_APPL_DATA) LocalDataPtr =
      (P2VAR(uint8,AUTOMATIC,ETHIF_APPL_DATA)) DataPtr;

    /* variable for broadcast evaluation */
    boolean isBroadcast = TRUE;

    /* loop iterator */
    uint8_least i;

    /* check if destination is a broadcast address */
    for(i = ETHIF_ETH_DESTMAC_OFFSET; i < ETHIF_ETH_SRCMAC_OFFSET; i++)
    {
      /* if a byte from the destination mac address is != 0xFF */
      if(LocalDataPtr[i] != 0xFFU)
      {
        /* this is no broadcast address */
        isBroadcast = FALSE;
        break;
      }
    }

    EthIf_RxIndication
      (
        CtrlIdx,
        (Eth_FrameType)(ETHIF_ETH_TYPE_GET(&LocalDataPtr[ETHIF_ETH_TYPE_OFFSET])),
        isBroadcast,
        &LocalDataPtr[ETHIF_ETH_SRCMAC_OFFSET],
        &LocalDataPtr[ETHIF_ETH_PAYLOAD_OFFSET],
        (uint16)(LenByte - ETHIF_ETH_HEADER_LENGTH)
      );
  }

  DBG_ETHIF_CBK_RXINDICATION_EXIT(CtrlIdx, DataPtr, LenByte);
}

FUNC(void, ETHIF_CODE) EthIf_RxIndication
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  DBG_ETHIF_RXINDICATION_ENTRY(CtrlIdx, FrameType, isBroadcast, PhysAddrPtr, DataPtr, LenByte);

#if((ETHIF_DEV_ERROR_DETECT == STD_ON) || (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON))
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->PhyCtrlMax)
  {
#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
     EthIf_IncMeasurementCnt();
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(DataPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_RXINDICATION_SVCID, ETHIF_E_PARAM_POINTER);
  }
#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF)
  else if(FrameType == ETHIF_VLAN_TPID)
  {
    /* filter VLAN tagged message if VLAN is not enabled */
#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
     EthIf_IncMeasurementCnt();
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */
  }
#endif /* ETHIF_VLAN_SUPPORT_ENABLE == STD_OFF */
  else
#endif /* (ETHIF_DEV_ERROR_DETECT == STD_ON) || (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON) */
  {
    Eth_FrameType TmpFrameType = FrameType;
    uint16 TmpLenByte = LenByte;
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) TmpDataPtr = DataPtr;

#if(ETHIF_ETHSWT_RX_PREPROCESS == STD_ON)
    const boolean Proceed = ETHIF_ETHSWT_PREPROCESSRXFRAME
                             (CtrlIdx, &TmpDataPtr, &TmpLenByte, PhysAddrPtr, &TmpFrameType);
    if(Proceed)
#endif /* ETHIF_ETHSWT_RX_PREPROCESS */
    {
      uint8 EthIfCtrlIdx;
      boolean EthIfCtrlIdx_Found = FALSE;

#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
    (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
      uint8 MapIdx;
      const uint8 IdxStart = ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdxOffset;
      const uint8 IdxEnd = IdxStart +
                           ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdNumber;
      for(MapIdx = IdxStart; (MapIdx < IdxEnd) && (EthIfCtrlIdx_Found == FALSE); MapIdx++)
      {
        EthIfCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP(MapIdx);

        EthIfCtrlIdx_Found = EthIf_RxIndicationInternal
        (
          EthIfCtrlIdx,
          TmpFrameType,
          isBroadcast,
          PhysAddrPtr,
          TmpDataPtr,
          TmpLenByte
        );
      }
#else
      const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;

      for(
           EthIfCtrlIdx = 0U;
           (EthIfCtrlIdx < EthIfCtrlMax) && (EthIfCtrlIdx_Found == FALSE);
           EthIfCtrlIdx++
         )
      {
        if(ETHIF_CFG_GET_CTRL_PTR(EthIfCtrlIdx)->PhysCtrlIdx == CtrlIdx)
        {
          EthIfCtrlIdx_Found = EthIf_RxIndicationInternal
          (
            EthIfCtrlIdx,
            TmpFrameType,
            isBroadcast,
            PhysAddrPtr,
            TmpDataPtr,
            TmpLenByte
          );
        }
      }
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
          (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */

      /* Matching EthIfCtrlIdx was not found */
#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
      if(EthIfCtrlIdx_Found == FALSE)
      {
        /* Increment the corresponding measurement data counter if received frame does not
           correspond to the configured EthIfCtrlIdxs */
        EthIf_IncMeasurementCnt();
      }
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */
    }
  }

  DBG_ETHIF_RXINDICATION_EXIT(CtrlIdx, FrameType, isBroadcast, PhysAddrPtr, DataPtr, LenByte);
}

FUNC(void, ETHIF_CODE) EthIf_Cbk_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
)
{
  DBG_ETHIF_CBK_TXCONFIRMATION_ENTRY(CtrlIdx, BufIdx);

#if(ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430)
  EthIf_TxConfirmation(CtrlIdx, BufIdx, E_OK);
#else
  EthIf_TxConfirmation(CtrlIdx, BufIdx);
#endif

  DBG_ETHIF_CBK_TXCONFIRMATION_EXIT(CtrlIdx, BufIdx);
}

#if(ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430)
/* !LINKSTO EthIf.ASR43.SWS_EthIf_00091, 1 */
FUNC(void, ETHIF_CODE) EthIf_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Std_ReturnType Result
)
{
  DBG_ETHIF_TXCONFIRMATION_ENTRY(CtrlIdx, BufIdx, Result);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_TXCONFIRMATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->PhyCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_TXCONFIRMATION_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    if (Result == E_OK)
    {
        uint8 EthIfCtrlIdx;
        uint8 MapIdx;
        const uint8 IdxStart = ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdxOffset;
        const uint8 IdxEnd = IdxStart +
                             ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdNumber;
        for(MapIdx = IdxStart; MapIdx < IdxEnd; MapIdx++)
        {
          uint8 Idx;
          EthIfCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP(MapIdx);
          for(Idx = 0U; Idx < EthIf_Up_TxConfirmation_MaxNum; Idx++)
          {
            EthIf_Up_TxConfirmation_FpList[Idx](EthIfCtrlIdx, BufIdx);
          }
        }
    }
  }

  DBG_ETHIF_TXCONFIRMATION_EXIT(CtrlIdx, BufIdx, Result);
}
#else
/* !LINKSTO EthIf.ASR42.SWS_EthIf_00091, 1 */
FUNC(void, ETHIF_CODE) EthIf_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
)
{
  DBG_ETHIF_TXCONFIRMATION_ENTRY(CtrlIdx, BufIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_TXCONFIRMATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->PhyCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CBK_TXCONFIRMATION_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    uint8 EthIfCtrlIdx;
#if(ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB)
    uint8 MapIdx;
    const uint8 IdxStart = ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdxOffset;
    const uint8 IdxEnd = IdxStart +
                         ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdNumber;
    for(MapIdx = IdxStart; MapIdx < IdxEnd; MapIdx++)
    {
      uint8 Idx;
      EthIfCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP(MapIdx);
      for(Idx = 0U; Idx < EthIf_Up_TxConfirmation_MaxNum; Idx++)
      {
        EthIf_Up_TxConfirmation_FpList[Idx](EthIfCtrlIdx, BufIdx);
      }
    }
#else
    const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;
    for(EthIfCtrlIdx = 0U; EthIfCtrlIdx < EthIfCtrlMax; EthIfCtrlIdx++)
    {
      if(ETHIF_CFG_GET_CTRL_PTR(EthIfCtrlIdx)->PhysCtrlIdx == CtrlIdx)
      {
        uint8 Idx;
        for(Idx = 0U; Idx < EthIf_Up_TxConfirmation_MaxNum; Idx++)
        {
          EthIf_Up_TxConfirmation_FpList[Idx](EthIfCtrlIdx, BufIdx);
        }
      }
    }
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */
  }

  DBG_ETHIF_TXCONFIRMATION_EXIT(CtrlIdx, BufIdx);
}
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) */

FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx(void)
{
  DBG_ETHIF_MAINFUNCTIONRX_ENTRY();

  if(EthIf_Initialized == TRUE)
  {
#if((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430) && \
    (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430_EB))
    uint8 PhysCtrlIdx;
    const uint8 PhyCtrlMax = EthIf_RootPtr->PhyCtrlMax;

    for(PhysCtrlIdx = 0U; PhysCtrlIdx < PhyCtrlMax; PhysCtrlIdx++)
    {
      CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
                &EthIf_PhyCtrlState[PhysCtrlIdx];
      if((ETH_MODE_ACTIVE == pEthIf_PhyCtrlState->Mode) &&
         (ETHIF_MODESTATE_SET == pEthIf_PhyCtrlState->ModeState))
      {
#if(ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR403)
        Eth_Receive(ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX(PhysCtrlIdx));
#elif(ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR422)
        {
          Eth_RxStatusType RxStatus;
          uint16_least iRxIndications;
          const uint16 RxIndicationsMax = ETHIF_RX_INDICATION_ITERATIONS;
          for(
               iRxIndications = 0U;
               iRxIndications < RxIndicationsMax;
               iRxIndications++
             )
          {
            Eth_Receive(ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX(PhysCtrlIdx), &RxStatus);
            if(RxStatus != ETH_RECEIVED_MORE_DATA_AVAILABLE)
            {
              break;
            }
          }
        }
#endif /* ETHIF_ETH_SUPPORT_API */
      }
    }
#else
/* ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430 ||
   ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB */
    const uint8 EthIf_PhysCtrlIngrFifoStart = ETHIF_NUM_PRIO_INGR_FIFO;
    const uint8 EthIf_PhysCtrlIngrFifoEnd = ETHIF_NUM_TOTAL_INGR_FIFO;
    uint8 PhysCtrlIngrFifoIdx;

    for(
         PhysCtrlIngrFifoIdx = EthIf_PhysCtrlIngrFifoStart;
         PhysCtrlIngrFifoIdx < EthIf_PhysCtrlIngrFifoEnd;
         PhysCtrlIngrFifoIdx++
       )
    {
      const uint8 PhysCtrlIdx =
        ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PhysCtrlIngrFifoIdx)->PhysCtrlIdx;
      uint16 MaxIterations =
        ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PhysCtrlIngrFifoIdx)->EthIfCtrlRxIndicationIterations;
      uint8  EthIngressFifoIdx =
        ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PhysCtrlIngrFifoIdx)->EthFifoIdx;
      CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
        &EthIf_PhyCtrlState[PhysCtrlIdx];

      if((ETH_MODE_ACTIVE == pEthIf_PhyCtrlState->Mode) &&
         (ETHIF_MODESTATE_SET == pEthIf_PhyCtrlState->ModeState))
      {
        Eth_RxStatusType RxStatus;
        uint16_least iRxIndications;
        for(
            iRxIndications = 0U;
            iRxIndications < MaxIterations;
            iRxIndications++
           )
        {
          ETHIF_ETH_RECEIVE(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), EthIngressFifoIdx, &RxStatus);

          if(RxStatus != ETH_RECEIVED_MORE_DATA_AVAILABLE)
          {
            break;
          }
        }
      }
    }
#endif /* (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430) &&
          (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR430_EB) */

#if(ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_OFF)
    EthIf_MainFunctionState();
#endif /* ETHIF_MAINFUNCTIONSTATE_API_ENABLE == STD_OFF */
  }
  DBG_ETHIF_MAINFUNCTIONRX_EXIT();
}

#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
     (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRxPrio
(
  uint8 PCtrlIngrFifoIdx
)
{
  DBG_ETHIF_MAINFUNCTIONRXPRIO_ENTRY( PCtrlIngrFifoIdx);

  if(EthIf_Initialized == TRUE)
  {
    const uint8 PhysCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PCtrlIngrFifoIdx)->PhysCtrlIdx;
    const uint16 MaxIterations =
          ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PCtrlIngrFifoIdx)->EthIfCtrlRxIndicationIterations;
    const uint8  EthFifoIdx =
          ETHIF_CFG_GET_PHYSCTRL_INGR_FIFO_PTR(PCtrlIngrFifoIdx)->EthFifoIdx;
    CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
          &EthIf_PhyCtrlState[PhysCtrlIdx];

    if((ETH_MODE_ACTIVE == pEthIf_PhyCtrlState->Mode) &&
       (ETHIF_MODESTATE_SET == pEthIf_PhyCtrlState->ModeState))
    {
      Eth_RxStatusType RxStatus;
      uint16_least iRxIndications;
      for(
          iRxIndications = 0U;
          iRxIndications < MaxIterations;
          iRxIndications++
         )
      {
        ETHIF_ETH_RECEIVE(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), EthFifoIdx, &RxStatus);

        if(RxStatus != ETH_RECEIVED_MORE_DATA_AVAILABLE)
        {
          break;
        }
      }
    }
  }
  DBG_ETHIF_MAINFUNCTIONRXPRIO_EXIT( PCtrlIngrFifoIdx);
}
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
          (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */

FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx(void)
{
  DBG_ETHIF_MAINFUNCTIONTX_ENTRY();

  if(EthIf_Initialized == TRUE)
  {
    uint8 PhysCtrlIdx;
    const uint8 PhyCtrlMax = EthIf_RootPtr->PhyCtrlMax;

    for(PhysCtrlIdx = 0U; PhysCtrlIdx < PhyCtrlMax; PhysCtrlIdx++)
    {
      CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
      &EthIf_PhyCtrlState[PhysCtrlIdx];
      if((ETH_MODE_ACTIVE == pEthIf_PhyCtrlState->Mode) &&
         (ETHIF_MODESTATE_SET == pEthIf_PhyCtrlState->ModeState))
      {
        ETHIF_ETH_TXCONFIRMATON(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx));
      }
    }
  }

  DBG_ETHIF_MAINFUNCTIONTX_EXIT();
}

FUNC(void, ETHIF_CODE) EthIf_Up_RxIndicationDummy
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  DBG_ETHIF_UP_RXINDICATIONDUMMY_ENTRY(
                                        CtrlIdx,
                                        FrameType,
                                        IsBroadcast,
                                        PhysAddrPtr,
                                        DataPtr,
                                        LenByte
                                      );
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(FrameType);
  TS_PARAM_UNUSED(IsBroadcast);
  TS_PARAM_UNUSED(PhysAddrPtr);
  TS_PARAM_UNUSED(DataPtr);
  TS_PARAM_UNUSED(LenByte);

  DBG_ETHIF_UP_RXINDICATIONDUMMY_EXIT(
                                       CtrlIdx,
                                       FrameType,
                                       IsBroadcast,
                                       PhysAddrPtr,
                                       DataPtr,
                                       LenByte
                                     );
}

FUNC(void, ETHIF_CODE) EthIf_Up_TxConfirmationDummy
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
)
{
  DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_ENTRY(CtrlIdx, BufIdx);
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(BufIdx);
  DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_EXIT(CtrlIdx, BufIdx);
}

FUNC(void, ETHIF_CODE) EthIf_Up_TrcvLinkStateChgDummy
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType TransceiverLinkState
)
{
  DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_ENTRY(CtrlIdx, TransceiverLinkState);
  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(TransceiverLinkState);
  DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_EXIT(CtrlIdx, TransceiverLinkState);
}

#if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON)

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCurrentTime
(
  uint8 CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETCURRENTTIME_ENTRY(CtrlIdx, timeQualPtr, timeStampPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCURRENTTIME_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCURRENTTIME_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(timeQualPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCURRENTTIME_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(timeStampPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCURRENTTIME_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    RetVal = ETHIF_ETH_GETCURRENTTIME
             (ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), timeQualPtr, timeStampPtr);
  }

  DBG_ETHIF_GETCURRENTTIME_EXIT(RetVal, CtrlIdx, timeQualPtr, timeStampPtr);

  return RetVal;
}

FUNC(void, ETHIF_CODE) EthIf_EnableEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
)
{
  DBG_ETHIF_ENABLEEGRESSTIMESTAMP_ENTRY(CtrlIdx, BufIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_ENABLEEGRESSTIMESTAMP_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_ENABLEEGRESSTIMESTAMP_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);
    ETHIF_ETH_ENABLEEGRESSTIMESTAMP(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), BufIdx);
  }

  DBG_ETHIF_ENABLEEGRESSTIMESTAMP_EXIT(CtrlIdx, BufIdx);
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETEGRESSTIMESTAMP_ENTRY(CtrlIdx, BufIdx, timeQualPtr, timeStampPtr);


#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETEGRESSTIMESTAMP_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETEGRESSTIMESTAMP_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(timeQualPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETEGRESSTIMESTAMP_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(timeStampPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETEGRESSTIMESTAMP_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    ETHIF_ETH_GETEGRESSTIMESTAMP
      (ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), BufIdx, timeQualPtr, timeStampPtr);
    RetVal = E_OK;
  }

  DBG_ETHIF_GETEGRESSTIMESTAMP_EXIT(RetVal, CtrlIdx, BufIdx, timeQualPtr, timeStampPtr);

  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimeStamp
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETINGRESSTIMESTAMP_ENTRY(CtrlIdx, DataPtr, timeQualPtr, timeStampPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETINGRESSTIMESTAMP_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETINGRESSTIMESTAMP_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(DataPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETINGRESSTIMESTAMP_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(timeQualPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETINGRESSTIMESTAMP_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(timeStampPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETINGRESSTIMESTAMP_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) TmpDataPtr = DataPtr;
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)

    /* get VID of this controller */
    const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->VID;

    if(EthIfVID < ETHIF_VLAN_VID_UNUSED)
    {
      TmpDataPtr -= ETHIF_ETH_VLAN_HEADER_LENGTH;
    }
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */

    ETHIF_ETH_GETINGRESSTIMESTAMP
      (ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), TmpDataPtr, timeQualPtr, timeStampPtr);

    RetVal = E_OK;
  }

  DBG_ETHIF_GETINGRESSTIMESTAMP_EXIT(RetVal, CtrlIdx, DataPtr, timeQualPtr, timeStampPtr);

  return RetVal;
}

FUNC(void, ETHIF_CODE) EthIf_SetCorrectionTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeIntDiffType, AUTOMATIC, ETHIF_APPL_DATA) timeOffsetPtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETHIF_APPL_DATA) rateRatioPtr
)
{
  DBG_ETHIF_SETCORRECTIONTIME_ENTRY(CtrlIdx, timeOffsetPtr, rateRatioPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCORRECTIONTIME_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCORRECTIONTIME_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(timeOffsetPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCORRECTIONTIME_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(rateRatioPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCORRECTIONTIME_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    ETHIF_ETH_SETCORRECTIONTIME
      (ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), timeOffsetPtr, rateRatioPtr);
  }

  DBG_ETHIF_SETCORRECTIONTIME_EXIT(CtrlIdx, timeOffsetPtr, rateRatioPtr);
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetGlobalTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_DATA) timeStampPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  DBG_ETHIF_SETGLOBALTIME_ENTRY(CtrlIdx, timeStampPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETGLOBALTIME_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETGLOBALTIME_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(timeStampPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETGLOBALTIME_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

    RetVal = ETHIF_ETH_SETGLOBALTIME(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), timeStampPtr);
  }

  DBG_ETHIF_SETGLOBALTIME_EXIT(RetVal, CtrlIdx, timeStampPtr);
  return RetVal;
}
#endif /* #if(ETHIF_GLOBAL_TIME_SYNC_API_ENABLE == STD_ON) */

#if((ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) && ((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR403) && \
    (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR422)))
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyLoopbackMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyLoopbackModeType Mode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETPHYLOOPBACKMODE_ENTRY(TrcvIdx, Mode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYLOOPBACKMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYLOOPBACKMODE_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_SETPHYLOOPBACKMODE(ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), Mode);
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_SETPHYLOOPBACKMODE_EXIT(RetVal, TrcvIdx, Mode);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyTxMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyTxModeType Mode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETPHYTXMODE_ENTRY(TrcvIdx, Mode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYTXMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYTXMODE_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_SETPHYTXMODE(ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), Mode);
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_SETPHYTXMODE_EXIT(RetVal, TrcvIdx, Mode);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTrcvSignalQuality
(
  uint8 TrcvIdx,
  P2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETTRCVSIGNALQUALITY_ENTRY(TrcvIdx, ResultPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRCVSIGNALQUALITY_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRCVSIGNALQUALITY_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else if(ResultPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRCVSIGNALQUALITY_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;
    CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvSignalQual =
               &EthIf_TrcvSignalQual[TrcvIdx];

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_GETPHYSIGQUAL
                 (ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), &ResultPtr->ActualSignalQuality);
      ResultPtr->HighestSignalQuality = pEthIf_TrcvSignalQual->HighestSignalQuality;
      ResultPtr->LowestSignalQuality = pEthIf_TrcvSignalQual->LowestSignalQuality;
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_GETTRCVSIGNALQUALITY_EXIT(RetVal, TrcvIdx, ResultPtr);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ClearTrcvSignalQuality
(
  uint8 TrcvIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_CLEARTRCVSIGNALQUALITY_ENTRY(TrcvIdx);
#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CLEARTRCVSIGNALQUALITY_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CLEARTRCVSIGNALQUALITY_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else
#endif
  {

    CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvSignalQual =
    &EthIf_TrcvSignalQual[TrcvIdx];
    pEthIf_TrcvSignalQual->HighestSignalQuality = 0U;
    pEthIf_TrcvSignalQual->LowestSignalQuality = 0xFFFFFFFFU;
    pEthIf_TrcvSignalQual->ActualSignalQuality = 0U;
    RetVal = E_OK;
  }

  DBG_ETHIF_CLEARTRCVSIGNALQUALITY_EXIT(RetVal, TrcvIdx);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCableDiagnosticsResult
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_CableDiagResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_ENTRY(TrcvIdx, ResultPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else if(ResultPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCABLEDIAGNOSTICSRESULT_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_GETCABLEDIAGRESULT(ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), ResultPtr);
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_EXIT(RetVal, TrcvIdx, ResultPtr);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPhyIdentifier
(
  uint8 TrcvIdx,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) OrgUniqueIdPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) ModelNrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) RevisionNrPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETPHYIDENTIFIER_ENTRY(TrcvIdx, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYIDENTIFIER_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYIDENTIFIER_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else if(OrgUniqueIdPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYIDENTIFIER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(ModelNrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYIDENTIFIER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(RevisionNrPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPHYIDENTIFIER_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_GETPHYIDENTIFIER
                (ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_GETPHYIDENTIFIER_EXIT(RetVal, TrcvIdx, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyTestMode
(
  uint8 TrcvIdx,
  EthTrcv_PhyTestModeType Mode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETPHYTESTMODE_ENTRY(TrcvIdx, Mode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYTESTMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETPHYTESTMODE_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else
#endif
  {
    boolean localAccessFlag = FALSE;

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      RetVal = ETHIF_ETHTRCV_SETPHYTESTMODE(ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), Mode);
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_SETPHYTESTMODE_EXIT(RetVal, TrcvIdx, Mode);
  return RetVal;
}
#endif /*((ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) && ((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR403) && \
          (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR422)))*/


FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetSwitchPortSignalQuality
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx,
  P2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) ResultPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_ENTRY(SwitchIdx, SwitchPortIdx, ResultPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else if(ResultPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETSWITCHPORTSIGNALQUALITY_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif
  {

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    {
      uint8 PortListIdx;
      const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;
      const uint8 EthSwtIdx = ETHIF_CFG_GET_SWT_IDX(SwitchIdx);

      for(PortListIdx = 0; PortListIdx < SwtPortsMax; PortListIdx++)
      {
        if((ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortListIdx) == EthSwtIdx) &&
           (ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortListIdx) == SwitchPortIdx))
        {
          CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortSignalQual =
              &EthIf_PortSignalQual[PortListIdx];

          RetVal = ETHIF_ETHSWT_GETPORTSIGNALQUALITY(
                                                      EthSwtIdx,
                                                      SwitchPortIdx,
                                                      &ResultPtr->ActualSignalQuality
                                                    );

          ResultPtr->HighestSignalQuality = pEthIf_PortSignalQual->HighestSignalQuality;
          ResultPtr->LowestSignalQuality = pEthIf_PortSignalQual->LowestSignalQuality;
          break;
        }
      }
    }
#else
    TS_PARAM_UNUSED(SwitchIdx);
    TS_PARAM_UNUSED(SwitchPortIdx);
    TS_PARAM_UNUSED(ResultPtr);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
  }
  DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_EXIT(RetVal, SwitchIdx, SwitchPortIdx, ResultPtr);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ClearSwitchPortSignalQuality
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_ENTRY(SwitchIdx, SwitchPortIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CLEARSWITCHPORTSIGNALQUALITY_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CLEARSWITCHPORTSIGNALQUALITY_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else
#endif
  {
#if (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    {
      uint8 PortListIdx;
      const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;
      const uint8 EthSwtIdx = ETHIF_CFG_GET_SWT_IDX(SwitchIdx);

      for(PortListIdx = 0; PortListIdx < SwtPortsMax; PortListIdx++)
      {
        if((ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortListIdx) == EthSwtIdx) &&
           (ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortListIdx) == SwitchPortIdx))
        {
          CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortSignalQual =
              &EthIf_PortSignalQual[PortListIdx];
          pEthIf_PortSignalQual->HighestSignalQuality = 0U;
          pEthIf_PortSignalQual->LowestSignalQuality = 0xFFFFFFFFU;
          pEthIf_PortSignalQual->ActualSignalQuality = 0U;
          RetVal = E_OK;
          break;
        }
      }
    }
#else
    TS_PARAM_UNUSED(SwitchIdx);
    TS_PARAM_UNUSED(SwitchPortIdx);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
  }
  DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_EXIT(RetVal, SwitchIdx, SwitchPortIdx);
  return RetVal;
}

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
#if(ETHIF_MII_API_ENABLE == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_WriteMii
(
  uint8 CtrlIdx,
  uint8 RegIdx,
  uint16 RegVal
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean localAccessFlag = FALSE;

  DBG_ETHIF_WRITEMII_ENTRY(CtrlIdx, RegIdx,RegVal);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_WRITEMII_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_WRITEMII_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(FALSE == EthIf_PhyCtrlState[ETHIF_PHYS_CTRLIDX(CtrlIdx)].IsInitialized)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_WRITEMII_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* Transceiver index */
    const uint8 TrcvIdx = ETHIF_CTRLIDX_ETHIFTRCVIDX(CtrlIdx);
    CONSTP2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_IsMiiActive =
    &EthIf_IsMiiActive[TrcvIdx];
    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    /* Call Eth_WriteMii only if the MII is not active */
    if(*pEthIf_IsMiiActive != TRUE)
    {
      /* Set Access Flag before calling Eth_WriteMii and reset it after
         call of Eth_WriteMii */
      TS_AtomicAssign8(*pEthIf_IsMiiActive, TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

      (void)ETHIF_ETH_WRITEMII(
                                ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx),
                                ETHIF_ETHTRCV_TRCVMIIIDX(CtrlIdx), RegIdx, RegVal
                               );
      RetVal = E_OK;
      TS_AtomicAssign8(*pEthIf_IsMiiActive, FALSE);
    }
  }
  DBG_ETHIF_WRITEMII_EXIT(RetVal, CtrlIdx, RegIdx,RegVal);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ReadMii
(
  uint8 CtrlIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) RegValPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  boolean localAccessFlag = FALSE;

  DBG_ETHIF_READMII_ENTRY(CtrlIdx, RegIdx,RegValPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_READMII_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_READMII_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(FALSE == EthIf_PhyCtrlState[ETHIF_PHYS_CTRLIDX(CtrlIdx)].IsInitialized)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_READMII_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    const uint8 TrcvIdx = ETHIF_CTRLIDX_ETHIFTRCVIDX(CtrlIdx);

    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
    /* Call Eth_ReadMii only if the MII is not active */
    if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
    {
      /* Set Access Flag before calling Eth_ReadMii and reset it after
         call of Eth_ReadMii */
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      localAccessFlag = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    if (localAccessFlag == TRUE)
    {
      const uint8 PhysCtrlIdx = ETHIF_PHYS_CTRLIDX(CtrlIdx);

      (void)ETHIF_ETH_READMII(
                                ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx),
                                ETHIF_ETHTRCV_TRCVMIIIDX(CtrlIdx), RegIdx, RegValPtr
                              );
      RetVal = E_OK;
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }

  DBG_ETHIF_READMII_EXIT(RetVal, CtrlIdx, RegIdx,RegValPtr);
  return RetVal;
}
#endif /* ETHIF_MII_API_ENABLE == STD_ON */
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_GETPORTMACADDR_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPortMacAddr
(
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) MacAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) SwitchIdxPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PortIdxPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETPORTMACADDR_ENTRY(MacAddrPtr, SwitchIdxPtr, PortIdxPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPORTMACADDR_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if((MacAddrPtr == NULL_PTR) || (SwitchIdxPtr == NULL_PTR) || (PortIdxPtr == NULL_PTR))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPORTMACADDR_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(*SwitchIdxPtr >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETPORTMACADDR_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* Function not implemented */
    TS_PARAM_UNUSED(MacAddrPtr);
    TS_PARAM_UNUSED(SwitchIdxPtr);
    TS_PARAM_UNUSED(PortIdxPtr);
  }

  DBG_ETHIF_GETPORTMACADDR_EXIT(RetVal, MacAddrPtr, SwitchIdxPtr, PortIdxPtr);
  return RetVal;
}
#endif /* ETHIF_GETPORTMACADDR_API == STD_ON */

#if(ETHIF_GETARLTABLE_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetArlTable
(
  uint8 SwitchIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) numberOfElements,
  P2VAR(Eth_MacVlanType, AUTOMATIC, ETHIF_APPL_DATA) arlTableListPointer
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETARLTABLE_ENTRY(SwitchIdx, numberOfElements, arlTableListPointer);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETARLTABLE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETARLTABLE_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else if(numberOfElements == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETARLTABLE_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else if(arlTableListPointer == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETARLTABLE_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHSWT_GETARLTABLE
               (ETHIF_CFG_GET_SWT_IDX(SwitchIdx), numberOfElements, arlTableListPointer);
  }

  DBG_ETHIF_GETARLTABLE_EXIT(RetVal, SwitchIdx, numberOfElements, arlTableListPointer);
  return RetVal;
}
#endif /* ETHIF_GETARLTABLE_API == STD_ON */

#if(ETHIF_GETBUFFERLEVEL_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBufferLevel
(
  uint8 SwitchIdx,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) SwitchBufferLevelPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETBUFFERLEVEL_ENTRY(SwitchIdx, SwitchBufferLevelPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETBUFFERLEVEL_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETBUFFERLEVEL_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else if(SwitchBufferLevelPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETBUFFERLEVEL_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHSWT_GETBUFFLEVEL
               (ETHIF_CFG_GET_SWT_IDX(SwitchIdx), SwitchBufferLevelPtr);
  }

  DBG_ETHIF_GETBUFFERLEVEL_EXIT(RetVal, SwitchIdx, SwitchBufferLevelPtr);
  return RetVal;
}
#endif /* ETHIF_GETBUFFERLEVEL_API == STD_ON */

#if(ETHIF_SWTGETCOUNTERVALUES_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwtGetCounterValues
(
  uint8 SwitchIdx,
  uint8 SwitchPortIdx,
  P2VAR(Eth_CounterType, AUTOMATIC, ETHIF_APPL_DATA) CounterPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SWTGETCOUNTERVALUES_ENTRY(SwitchIdx, SwitchPortIdx, CounterPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWTGETCOUNTERVALUES_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWTGETCOUNTERVALUES_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else if(CounterPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWTGETCOUNTERVALUES_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHSWT_GETCOUNTERVALUES
               (ETHIF_CFG_GET_SWT_IDX(SwitchIdx), SwitchPortIdx, CounterPtr);
  }

  DBG_ETHIF_SWTGETCOUNTERVALUES_EXIT(RetVal, SwitchIdx, SwitchPortIdx, CounterPtr);
  return RetVal;
}
#endif /* ETHIF_SWTGETCOUNTERVALUES_API == STD_ON */

#if(ETHIF_STORECONFIGURATION_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StoreConfiguration
(
  uint8 SwitchIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_STORECONFIGURATION_ENTRY(SwitchIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_STORECONFIGURATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_STORECONFIGURATION_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHSWT_STORECONFIG(ETHIF_CFG_GET_SWT_IDX(SwitchIdx));
  }

  DBG_ETHIF_STORECONFIGURATION_EXIT(RetVal, SwitchIdx);
  return RetVal;
}
#endif /* ETHIF_STORECONFIGURATION_API == STD_ON */

#if(ETHIF_RESETCONFIGURATION_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ResetConfiguration
(
  uint8 SwitchIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_RESETCONFIGURATION_ENTRY(SwitchIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RESETCONFIGURATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(SwitchIdx >= EthIf_RootPtr->EthIfSwtMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_RESETCONFIGURATION_SVCID, ETHIF_E_INV_SWT_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHSWT_RESETCONFIG(ETHIF_CFG_GET_SWT_IDX(SwitchIdx));
  }

  DBG_ETHIF_RESETCONFIGURATION_EXIT(RetVal, SwitchIdx);
  return RetVal;
}
#endif /* ETHIF_RESETCONFIGURATION_API == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetTransceiverWakeupMode
(
  uint8 TrcvIdx,
  EthTrcv_WakeupModeType TrcvWakeupMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_ENTRY(TrcvIdx, TrcvWakeupMode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else if((TrcvWakeupMode != ETHTRCV_WUM_DISABLE) &&
          (TrcvWakeupMode != ETHTRCV_WUM_ENABLE) &&
          (TrcvWakeupMode != ETHTRCV_WUM_CLEAR))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_INV_PARAM);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
    &EthIf_TrcvState[TrcvIdx];
    if ((TrcvWakeupMode == ETHTRCV_WUM_CLEAR) &&
        (pEthIf_TrcvState->TrcvWakeupMode == ETHTRCV_WUM_ENABLE))
    {
      /* no call forward to EthTrcv */
    }
    else if(((TrcvWakeupMode == ETHTRCV_WUM_ENABLE) &&
             (pEthIf_TrcvState->TrcvWakeupMode == ETHTRCV_WUM_ENABLE)) ||
            ((TrcvWakeupMode == ETHTRCV_WUM_DISABLE) &&
             (pEthIf_TrcvState->TrcvWakeupMode == ETHTRCV_WUM_DISABLE)))
    {
      /* no change of mode -> nothing to be done */
      RetVal = E_OK;
    }
    else
    {
      RetVal = ETHIF_ETHTRCV_SETTRCVWAKEUPMODE
                 (ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), TrcvWakeupMode);

      if(RetVal == E_OK)
      {
        if((TrcvWakeupMode == ETHTRCV_WUM_DISABLE) || (TrcvWakeupMode == ETHTRCV_WUM_ENABLE))
        {
          pEthIf_TrcvState->TrcvWakeupMode = TrcvWakeupMode;
        }
      }
    }
  }

  DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_EXIT(RetVal, TrcvIdx, TrcvWakeupMode);

  return RetVal;
}

#if (ETHIF_GETTRANSCEIVERWAKEUPMODE_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverWakeupMode
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_DATA) TrcvWakeupModePtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_ENTRY(TrcvIdx, TrcvWakeupModePtr);


#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(TrcvIdx >= EthIf_RootPtr->EthIfTrcvMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_INV_TRCV_IDX);
  }
  else if(TrcvWakeupModePtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETTRANSCEIVERWAKEUPMODE_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    RetVal = ETHIF_ETHTRCV_GETTRCVWAKEUPMODE
               (ETHIF_CFG_GET_TRANSCEIVER_PTR(TrcvIdx), TrcvWakeupModePtr);
  }

  DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_EXIT(RetVal, TrcvIdx, TrcvWakeupModePtr);

  return RetVal;
}
#endif /* ETHIF_GETTRANSCEIVERWAKEUPMODE_API == STD_ON */

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_CheckWakeup
(
  EcuM_WakeupSourceType WakeupSource
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  TS_PARAM_UNUSED(WakeupSource);

  DBG_ETHIF_CHECKWAKEUP_ENTRY(WakeupSource);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CHECKWAKEUP_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    uint8 EthIfTrcvIdx;
    Std_ReturnType Res;
    const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

    /* Set it to E_OK for no Trcv configurations. */
    RetVal = E_OK;

    for(EthIfTrcvIdx = 0U; EthIfTrcvIdx < EthIfTrcvMax; EthIfTrcvIdx++)
    {
      Res = ETHIF_ETHTRCV_CHECKWAKEUP(ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx));
      if(Res == E_NOT_OK)
      {
        RetVal = E_NOT_OK;
      }
    }
  }

  DBG_ETHIF_CHECKWAKEUP_EXIT(RetVal, WakeupSource);

  return RetVal;
}
#endif /* ETHIF_WAKEUPSUPPORT_API == STD_ON */
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if (ETHIF_GETCTRLIDXLIST_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCtrlIdxList
(
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) NumberOfCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) CtrlIdxListPtr
)
{
  /* Function not implemented */
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETCTRLIDXLIST_ENTRY(NumberOfCtrlIdx, CtrlIdxListPtr);

  TS_PARAM_UNUSED(NumberOfCtrlIdx);
  TS_PARAM_UNUSED(CtrlIdxListPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETCTRLIDXLIST_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
#endif /* ETHIF_DEV_ERROR_DETECT */

  DBG_ETHIF_GETCTRLIDXLIST_EXIT(RetVal, NumberOfCtrlIdx, CtrlIdxListPtr);

  return RetVal;
}
#endif /* ETHIF_GETCTRLIDXLIST_API == STD_ON */

#if (ETHIF_GETVLANIDSUPPORT_API == STD_ON)
/* !LINKSTO EthIf.ASR43.SWS_EthIf_91052, 1 */
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetVlanId
(
  uint8 CtrlIdx,
  P2VAR(uint16, AUTOMATIC, ETHIF_APPL_DATA) VlanIdPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETVLANID_ENTRY(CtrlIdx, VlanIdPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETVLANID_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(VlanIdPtr == NULL_PTR)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETVLANID_SVCID, ETHIF_E_PARAM_POINTER);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->VID;

    if(ETHIF_VLAN_VID_UNUSED == EthIfVID)
    {
      *VlanIdPtr = 0U;
    }
    else
    {
      *VlanIdPtr = EthIfVID;
    }
#else
    *VlanIdPtr = 0U;
#endif /* ETHIF_VLAN_SUPPORT_ENABLE == STD_ON */

    RetVal = E_OK;
  }

  DBG_ETHIF_GETVLANID_EXIT(RetVal, CtrlIdx, VlanIdPtr);

  return RetVal;
}
#endif /* ETHIF_GETVLANIDSUPPORT_API == STD_ON */

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetAndResetMeasurementData
(
  EthIf_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_DATA) MeasurementDataPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_GETANDRESETMEASUREMENTDATA_ENTRY(
                                              MeasurementIdx,
                                              MeasurementResetNeeded,
                                              MeasurementDataPtr
                                            );

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_GETANDRESETMEASUREMENTDATA_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    switch(MeasurementIdx)
    {
      case ETHIF_MEAS_DROP_CRTLIDX:
      {
        if(MeasurementDataPtr != NULL_PTR)
        {
          TS_AtomicAssign32(*MeasurementDataPtr, EthIf_MeasurementDropData);
        }
        if(MeasurementResetNeeded)
        {
          TS_AtomicAssign32(EthIf_MeasurementDropData, 0U);
        }
        RetVal = E_OK;
        break;
      }
      case ETHIF_MEAS_ALL:
      {
        if(MeasurementResetNeeded)
        {
          TS_AtomicAssign32(EthIf_MeasurementDropData, 0U);
        }
        RetVal = E_OK;
        break;
      }
      default:
      {
        break;
      }
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
  }

  DBG_ETHIF_GETANDRESETMEASUREMENTDATA_EXIT(
                                             RetVal,
                                             MeasurementIdx,
                                             MeasurementResetNeeded,
                                             MeasurementDataPtr
                                           );

  return RetVal;
}
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */


FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchPortGroupRequestMode
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  EthTrcv_ModeType PortMode
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_ENTRY(PortGroupIdx, PortMode);

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  /* Logical PortGroups are not accessible from BswM */
  else if(PortGroupIdx >= (EthIf_RootPtr->SwtPortGroupsMax - EthIf_RootPtr->EthIfSwtMax))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID, ETHIF_E_INV_PORT_GROUP_IDX);
  }
  else if((ETHTRCV_MODE_ACTIVE != PortMode) && (ETHTRCV_MODE_DOWN != PortMode))
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID, ETHIF_E_INV_PARAM);
  }
  else if(ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_CONTROL)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_SWITCHPORTGROUPREQUESTMODE_SVCID, ETHIF_E_INV_PORT_GROUP_IDX);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortGroupState =
    &EthIf_PortGroupState[PortGroupIdx];
    RetVal = E_OK;
    /* Mode request is ignored if the mode does not change because of reference counting
     * on a port level */
    if(pEthIf_PortGroupState->Mode != TRCV2ETH_MODE(PortMode))
    {
      /* Mode is for port group handled only if it is managed by BswM, i.e. no EthIfCtrl
       * references the group. In case port group is under control of EthIfCtrl - it takes care
       * that a single instance does not call set operation twice. */
      pEthIf_PortGroupState->Mode = TRCV2ETH_MODE(PortMode);

      if((pEthIf_PortGroupState->Mode == ETHTRCV_MODE_DOWN) &&
         (ETHIF_SWITCH_OFF_PORT_TIMEDELAY > 0U))
      {
        /* DOWN transition of port groups under the control of BswM needs to be to delayed. */
        pEthIf_PortGroupState->SwitchOffTimer = ETHIF_SWITCH_OFF_PORT_TIMEDELAY;
        pEthIf_PortGroupState->ModeState = ETHIF_MODESTATE_REQUESTED;
      }
      else
      {
        if(pEthIf_PortGroupState->ModeState == ETHIF_MODESTATE_REQUESTED)
        {
          /* Switch-off timer was running when ACTIVE request was received -> ports are ACTIVE -
           * no need to activate them again. */
          pEthIf_PortGroupState->ModeState = ETHIF_MODESTATE_SET;
          pEthIf_PortGroupState->SwitchOffTimer = 0xFFFFU;
        }
        else
        {
          /* Propagate port group mode change to underlying switch ports. */
          EthIf_SetPortGroupMode(PortGroupIdx, TRCV2ETH_MODE(PortMode));
        }
      }
    }
  }
#else
  TS_PARAM_UNUSED(PortGroupIdx);
  TS_PARAM_UNUSED(PortMode);
#endif /* ETHIF_PORTGROUPSUPPORT */

  DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_EXIT(RetVal, PortGroupIdx, PortMode);

  return RetVal;
}


FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StartAllPorts
(
  void
)
{
  Std_ReturnType Res;

  DBG_ETHIF_STARTALLPORTS_ENTRY();

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    Res = E_NOT_OK;
    ETHIF_DET_REPORT_ERROR(ETHIF_STARTALLPORTS_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
#if(ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
    Std_ReturnType RetVal;
    EthIf_SwitchPortGroupIdxType PortGroupIdx;
    EthIf_SwtPortIdxType PortIdx;
    EthIf_EthSwtPortLockListType EthSwtPortLockList;
    boolean StartPort[ETHIF_PORTS_ARRAYSIZE];
    /* Searching of BswM controlled SwtPortGroups. SwtPortGroupsMax can be reduced by EthIfSwtMax
       since these port groups at the end of the array are always EthIf controlled.*/
    const EthIf_SwitchPortGroupIdxType UserDefinedPortGroupMax
      = EthIf_RootPtr->SwtPortGroupsMax - EthIf_RootPtr->EthIfSwtMax;
    const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;

    /* Clear port lock list */
    TS_MemBZero(&EthSwtPortLockList[0U], ETHIF_PORTS_ARRAYSIZE);

    TS_MemBZero(&StartPort[0U], ETHIF_PORTS_ARRAYSIZE);

    /* Filter out user defined (without logical) port groups not controlled by EthIfControllers. */
    for(PortGroupIdx = 0U; PortGroupIdx < UserDefinedPortGroupMax; PortGroupIdx++)
    {
      if((ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_NO_REF) ||
         (ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_LINK_INFO))
      {
        uint16 BeginIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
        uint8 NumPorts = ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);
        uint16 PortListIdx;

        /* Mark all ports that belong to a port group not referenced by EthIfController. */
        for(PortListIdx = BeginIdx; PortListIdx < (BeginIdx + NumPorts); PortListIdx++)
        {
          PortIdx = ETHIF_CFG_ETHIFPORTIDX(PortListIdx);

          StartPort[PortIdx] = TRUE;
          {
            /* If port is connected to a transceiver -> mark it for locking .*/
            const uint8 TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);
            if(TrcvIdx != ETHIF_INVALID_TRCV_IDX)
            {
              EthSwtPortLockList[PortIdx] = TRUE;
            }
          }
        }
      }
    }

    /* Lock Switch ports. */
    EthIf_LockSwitchPorts(&EthSwtPortLockList[0U]);

    Res = E_OK;

    /* Filter out marked ports that are in DOWN state. */

    for(PortIdx = 0U; PortIdx < SwtPortsMax; PortIdx++)
    {
      if((StartPort[PortIdx] == TRUE) && (EthIf_PortState[PortIdx].Mode == ETH_MODE_DOWN))
      {
        /* Put port mode to ACTIVE */
        RetVal =  EthIf_SetPortMode(PortIdx, ETH_MODE_ACTIVE, 0U, &EthSwtPortLockList[0U]);

#if(ETHIF_PORT_STARTUP_ACTIVE_TIME != 0U)
        if(RetVal == E_OK)
        {
          /* Put port mode to DOWN, with a configurable time delay */
          (void) EthIf_SetPortMode(
                                    PortIdx,
                                    ETH_MODE_DOWN,
                                    ETHIF_PORT_STARTUP_ACTIVE_TIME,
                                    &EthSwtPortLockList[0U]
                                  );
        }
        else
        {
          /* If any of port set mode operations fails we return E_NOT_OK */
          Res = E_NOT_OK;
        }
#else
      TS_PARAM_UNUSED(RetVal);
      Res = E_NOT_OK;
#endif /* ETHIF_PORT_STARTUP_ACTIVE_TIME != 0U */
      }
    }

    /* Unlock previously locked ports.  */
    EthIf_UnlockSwitchPorts(&EthSwtPortLockList[0U]);
#else
    Res = E_NOT_OK;
#endif /* ETHIF_SWT_SUPPORT_ENABLE */
  }

  DBG_ETHIF_STARTALLPORTS_EXIT(Res);

  return Res;
}


#if (ETHIF_VERIFYCONFIG_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VerifyConfig
(
  uint8 SwitchIdx,
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) Result
)
{
  /* Function not implemented */
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_VERIFYCONFIG_ENTRY(SwitchIdx, Result);

  TS_PARAM_UNUSED(SwitchIdx);
  TS_PARAM_UNUSED(Result);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
if(EthIf_Initialized != TRUE)
{
  ETHIF_DET_REPORT_ERROR(ETHIF_VERIFYCONFIG_SVCID, ETHIF_E_NOT_INITIALIZED);
}
#endif /* ETHIF_DEV_ERROR_DETECT */

  DBG_ETHIF_VERIFYCONFIG_EXIT(RetVal, SwitchIdx, Result);

  return RetVal;
}
#endif /* ETHIF_VERIFYCONFIG_API == STD_ON */


#if (ETHIF_SETFORWARDINGMODE_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetForwardingMode
(
  uint8 SwitchIdx,
  boolean mode
)
{
  /* Function not implemented */
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_SETFORWARDINGMODE_ENTRY(SwitchIdx, mode);

  TS_PARAM_UNUSED(SwitchIdx);
  TS_PARAM_UNUSED(mode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
if(EthIf_Initialized != TRUE)
{
  ETHIF_DET_REPORT_ERROR(ETHIF_SETFORWARDINGMODE_SVCID, ETHIF_E_NOT_INITIALIZED);
}
#endif /* ETHIF_DEV_ERROR_DETECT */

  DBG_ETHIF_SETFORWARDINGMODE_EXIT(RetVal, SwitchIdx, mode);

  return RetVal;
}
#endif /* ETHIF_SETFORWARDINGMODE_API == STD_ON */


STATIC FUNC(void, ETHIF_CODE) EthIf_SetEthIfCtrlMode
(
  uint8 EthIfCtrlIdx,
  Eth_ModeType CtrlMode
)
{
  CONSTP2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState =
  &EthIf_CtrlState[EthIfCtrlIdx];

  TS_AtomicAssign8(pEthIf_CtrlState->Mode, CtrlMode);
  TS_AtomicAssign8(pEthIf_CtrlState->ModeState, ETHIF_MODESTATE_SET);

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
  {
    const EthIf_SwitchPortGroupIdxType PortGroupIdx = ETHIF_PORTGROUPIDX(EthIfCtrlIdx);
    boolean InControl = FALSE;

    if(PortGroupIdx != ETHIF_INVALID_PORTGROUP_IDX)
    {
      InControl = ((ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_CONTROL) ||
                   (ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_SWITCH));
    }

    /* If EthIfCtrl references and controls port group or switch (logical port group) and
     * transition is to ACTIVE. For DOWN transition, port groups are handled before EthPhyCtrl
     * is set DOWN.*/
    if((CtrlMode == ETH_MODE_ACTIVE) && (InControl == TRUE))
    {
      EthIf_SetPortGroupMode(PortGroupIdx, CtrlMode);
    }
  }
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
}

FUNC(void, ETHIF_CODE) EthIf_CtrlModeIndication
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
)
{
  uint8 EthIfCtrlIdx;
  uint8 PhysCtrlIdx = CtrlIdx;
  P2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState;
  P2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState;

  /* CtrlIdx is physical controller index in the context of EthIf - EthIfPhysControllerIdx. */
  DBG_ETHIF_CTRLMODEINDICATION_ENTRY(CtrlIdx, CtrlMode);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_CTRLMODEINDICATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else
#endif
  {
#if((ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) || \
    (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB))
    {
      boolean EthIfCtrlIdx_Found = FALSE;
      uint8 MapIdx;

      const uint8 IdxStart = ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdxOffset;
      const uint8 IdxEnd = IdxStart +
                           ETHIF_CFG_GET_PHYSCTRL_MAP_DATA_PTR(CtrlIdx)->EthIfCtrlIdNumber;

      for(MapIdx = IdxStart; (MapIdx < IdxEnd) && (EthIfCtrlIdx_Found == FALSE); MapIdx++)
      {
        EthIfCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_ETHIFCTRLIDX_MAP(MapIdx);
        pEthIf_CtrlState = &EthIf_CtrlState[EthIfCtrlIdx];

        if(MapIdx == IdxStart)
        {
          /* Update physical controller only once. */
          pEthIf_PhyCtrlState = &EthIf_PhyCtrlState[PhysCtrlIdx];

          TS_AtomicAssign8(pEthIf_PhyCtrlState->Mode, CtrlMode);
          TS_AtomicAssign8(pEthIf_PhyCtrlState->ModeState, ETHIF_MODESTATE_SET);

          SET_TIMER_REPEAT_SETMODE(pEthIf_PhyCtrlState->SetRepeatTimer);
        }

        if((pEthIf_CtrlState->Mode == CtrlMode) &&
            (pEthIf_CtrlState->ModeState == ETHIF_MODESTATE_REQUESTED))
        {
          EthSM_CtrlModeIndication(EthIfCtrlIdx, CtrlMode);
          EthIf_SetEthIfCtrlMode(EthIfCtrlIdx, CtrlMode);
        }
      }
    }
#else
    {
      const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;
      pEthIf_PhyCtrlState = &EthIf_PhyCtrlState[PhysCtrlIdx];

      TS_AtomicAssign8(pEthIf_PhyCtrlState->Mode, CtrlMode);
      TS_AtomicAssign8(pEthIf_PhyCtrlState->ModeState, ETHIF_MODESTATE_SET);

      SET_TIMER_REPEAT_SETMODE(pEthIf_PhyCtrlState->SetRepeatTimer);

      /* Update EthIfCtrl(s) that reference indicated EthCtrl and notify SM in case of a change. */
      for(EthIfCtrlIdx = 0U; EthIfCtrlIdx < EthIfCtrlMax; EthIfCtrlIdx++)
      {
        if(ETHIF_CFG_GET_CTRL_PTR(EthIfCtrlIdx)->PhysCtrlIdx == PhysCtrlIdx)
        {
          pEthIf_CtrlState = &EthIf_CtrlState[EthIfCtrlIdx];
          if((pEthIf_CtrlState->Mode == CtrlMode) &&
              (pEthIf_CtrlState->ModeState == ETHIF_MODESTATE_REQUESTED))
          {
            EthSM_CtrlModeIndication(EthIfCtrlIdx, CtrlMode);
            EthIf_SetEthIfCtrlMode(EthIfCtrlIdx, CtrlMode);
          }
        }
      }
    }
#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) ||
          (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430_EB) */

  /* Since EthCtrl mode is set and confirmed->set mode for the transceiver (if one is configured.)*/
#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    {
      const uint8 EthIfTrcvIdx = ETHIF_CFG_GET_ETHIFTRCV_IDX(PhysCtrlIdx); /* PhysCtrlIdx -> EthIfTrcvIdx */
      EthIf_SetTrcvMode(EthIfTrcvIdx, CtrlMode, TRUE);
    }
#endif
  }
  DBG_ETHIF_CTRLMODEINDICATION_EXIT(CtrlIdx, CtrlMode);
}

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_TrcvModeIndication
(
  uint8 TrcvIdx,
  EthTrcv_ModeType TrcvMode
)
{
#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_TRCVMODEINDICATION_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else
#endif
  {
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
        &EthIf_TrcvState[TrcvIdx];
    /* TrcvIdx here is transceiver index in the context of EthIf - EthIfTrcvIdx. */
    TS_AtomicAssign8(pEthIf_TrcvState->Mode, TRCV2ETH_MODE(TrcvMode));
    TS_AtomicAssign8(pEthIf_TrcvState->ModeState, ETHIF_MODESTATE_SET);

    SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
  }
}
#endif /* (ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) */

#if (ETHIF_SWITCHMANAGEMENTSUPPORT == STD_ON)
FUNC(void, ETHIF_CODE) EthIf_SwitchMgmtInfoIndication
(
  uint8 CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  P2VAR(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_APPL_DATA) MgmtInfoPtr
)
{
  /* Function not implemented */

  DBG_ETHIF_SWITCHMGMTINFOINDICATION_ENTRY(CtrlIdx, DataPtr, MgmtInfoPtr);

  TS_PARAM_UNUSED(CtrlIdx);
  TS_PARAM_UNUSED(DataPtr);
  TS_PARAM_UNUSED(MgmtInfoPtr);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
if(EthIf_Initialized != TRUE)
{
  ETHIF_DET_REPORT_ERROR(ETHIF_SWITCHMGMTINFOINDICATION_SVCID, ETHIF_E_NOT_INITIALIZED);
}
#endif /* ETHIF_DEV_ERROR_DETECT */

  DBG_ETHIF_SWITCHMGMTINFOINDICATION_EXIT(CtrlIdx, DataPtr, MgmtInfoPtr);

}
#endif /* ETHIF_SWITCHMANAGEMENTSUPPORT == STD_ON */

/*==================[internal function definitions]=========================*/
STATIC FUNC(void, ETHIF_CODE) EthIf_InitControllers(void)
{
  uint8 PhysCtrlIdx;
  const uint8 PhyCtrlMax = EthIf_RootPtr->PhyCtrlMax;

  for(PhysCtrlIdx = 0U; PhysCtrlIdx < PhyCtrlMax; PhysCtrlIdx++)
  {
    CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
    &EthIf_PhyCtrlState[PhysCtrlIdx];
    /* initialize the controller */
#if (ETHIF_INIT_CTRL_TRCV == STD_ON)
    /* Error handling is not specified. */
    Std_ReturnType RetVal;
    RetVal = Eth_ControllerInit(ETHIF_CFG_GET_PHYSCTRL_ETHCTRLIDX(PhysCtrlIdx), 0U);

    if(RetVal == E_OK)
    {
      TS_AtomicAssign8(pEthIf_PhyCtrlState->IsInitialized, TRUE);
    }
#else
    TS_AtomicAssign8(pEthIf_PhyCtrlState->IsInitialized, TRUE);
#endif /* ETHIF_INIT_CTRL_TRCV == STD_ON */
  }
}

STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPhyCtrlMode
(
  uint8 PhysCtrlIdx,
  Eth_ModeType ReqMode,
  P2VAR(EthIf_ModeStateType, AUTOMATIC, ETHIF_APPL_DATA) NewState
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
  &EthIf_PhyCtrlState[PhysCtrlIdx];

  if(pEthIf_PhyCtrlState->IsInitialized == TRUE)
  {
    /* Update EthCtrl reference counter and check if the hardware needs to update. */
    const boolean UpdateHw = UpdateRefCnt(
                                            &pEthIf_PhyCtrlState->RefCnt,
                                            &pEthIf_PhyCtrlState->Mode,
                                            ReqMode
                                          );

    if(UpdateHw == TRUE)
    {
      TS_AtomicAssign8(pEthIf_PhyCtrlState->ModeState, ETHIF_MODESTATE_REQUESTED);

      RetVal = ETHIF_ETH_SETCONTROLLERMODE(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx), ReqMode);

      if(RetVal == E_OK)
      {
        /* In case of synchronous communication between EthIf and EthDrv -> simulate
         * asynchronous communication by sending indication from EthDrv Driver. */
#if(ETHIF_ETH_ASYNC_ENABLE == STD_OFF)
        EthIf_CtrlModeIndication(PhysCtrlIdx, ReqMode);
#endif /* ETHIF_ETH_ASYNC_ENABLE == STD_OFF */
      }
      else
      {
        /* Repeat operation in the proceeding MainFunction() call.*/
        ELAPSE_TIMER(pEthIf_PhyCtrlState->SetRepeatTimer);
      }
    }
    else
    {
      RetVal = E_OK;
    }
  }
  else
  {
#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
    ETHIF_DET_REPORT_ERROR(ETHIF_SETCONTROLLERMODE_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
#endif
  }

  /* Inform superior object (EthIfCtrl) about ModeState of underlying object - EthPhyCtrl. */
  *NewState = pEthIf_PhyCtrlState->ModeState;

  return RetVal;
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UpdatePhyCtrlState(void)
{
  uint8 PhysCtrlIdx;
  Std_ReturnType RetVal;
  const uint8 PhyCtrlMax = EthIf_RootPtr->PhyCtrlMax;

  /* Repeat set operations for PhyEthCtrl(s) that are in transitional state */
  for(PhysCtrlIdx = 0U; PhysCtrlIdx < PhyCtrlMax; PhysCtrlIdx++)
  {
    CONSTP2VAR(EthIf_PhyCtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PhyCtrlState =
    &EthIf_PhyCtrlState[PhysCtrlIdx];

    if(pEthIf_PhyCtrlState->IsInitialized == TRUE)
    {
      if(pEthIf_PhyCtrlState->ModeState == ETHIF_MODESTATE_REQUESTED)
      {
        RetVal = ETHIF_ETH_SETCONTROLLERMODE(ETHIF_CFG_GET_PHYSCTRL_PTR(PhysCtrlIdx),
                                                 pEthIf_PhyCtrlState->Mode);

        if(RetVal == E_OK)
        {
#if(ETHIF_ETH_ASYNC_ENABLE == STD_OFF)
          EthIf_CtrlModeIndication(PhysCtrlIdx, pEthIf_PhyCtrlState->Mode);
#endif /* ETHIF_ETH_ASYNC_ENABLE == STD_OFF */
        }
        else
        {
          /* Repeat operation in the proceeding MainFunction() call.*/
          ELAPSE_TIMER(pEthIf_PhyCtrlState->SetRepeatTimer);
        }
      }
    }
  }
}

ETHIF_MAINFUNCTIONSTATE_IS_STATIC FUNC(void, ETHIF_CODE) EthIf_MainFunctionState(void)
{
  EthIf_TrcvLockListType TrcvLockList;
  EthIf_EthSwtPortLockListType EthSwtPortLockList;

  DBG_ETHIF_MAINFUNCTIONSTATE_ENTRY();

  if(EthIf_Initialized == TRUE)
  {

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    /* Port group can be shared among several EthIfCtrls and port can be shared among several port
     * groups. Cycle counter is used to read object link state only once in a single
     * MainFunctionState() cycle. It is incremented here and when it overflows, cycle counters of
     * all port group and port objects are reseted to avoid incorrect readings.*/
    EthIf_MainFntCycleCnt++;
    if(EthIf_MainFntCycleCnt == 0xFFU)
    {
      uint8 Idx;
      const uint8 SwtPortGroupsMax = EthIf_RootPtr->SwtPortGroupsMax;
      const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;
      EthIf_MainFntCycleCnt = 0U;

      for(Idx = 0U; Idx < SwtPortGroupsMax; Idx++)
      {
        EthIf_PortGroupState[Idx].CycleCnt = 0xFFU;
      }

      for(Idx = 0U; Idx < SwtPortsMax; Idx++)
      {
        EthIf_PortState[Idx].CycleCnt = 0xFFU;
      }
    }
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

    /* Update physical controllers that are in REQUESTED state. */
    EthIf_UpdatePhyCtrlState();

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    /* Clear transceiver lock list */
    TS_MemBZero(&TrcvLockList[0U], EthIf_RootPtr->EthIfTrcvMax);

    /* Locate transceivers which are updated within this function. */
    EthIf_FindEthIfTrcvAccess(&TrcvLockList[0U]);

    /* Lock affected transceivers. */
    EthIf_LockTransceivers(&TrcvLockList[0U]);
#else
    TS_PARAM_UNUSED(TrcvLockList);
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)
    /* Update switch-off timer for port groups that are under control of BswM. */
    EthIf_UpdateBswMPortGroupSwitchOffTimer();
#endif /* ETHIF_PORTGROUPSUPPORT == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    /* Clear port lock list */
    TS_MemBZero(&EthSwtPortLockList[0U], ETHIF_PORTS_ARRAYSIZE);

    /* Locate ports which are updated within this function. */
    EthIf_FindPortTrcvAccess(&EthSwtPortLockList[0U]);

    /* Lock affected ports. */
    EthIf_LockSwitchPorts(&EthSwtPortLockList[0U]);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    /* Update transceivers that are in REQUESTED state and for active read-out LinkState. */
    EthIf_UpdateEthIfTrcvState(&TrcvLockList[0U]);
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    /* Update ports that are in REQUESTED state. */
    EthIf_UpdatePortState(&EthSwtPortLockList[0U]);

    /* Update interface controllers that are in REQUESTED state. */
    EthIf_UpdateEthIfCtrlState();
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)
    /* Update and report link state of port groups that are under control of BswM. */
    EthIf_UpdateBswMPortGroupLinkState(&EthSwtPortLockList[0U]);
#endif /* ETHIF_PORTGROUPSUPPORT == STD_ON */

    /* Update and report link state change for EthIfControllers. */
    EthIf_UpdateEthIfCtrlLinkState(&EthSwtPortLockList[0U]);

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    /* Unlock affected ports. */
    EthIf_UnlockSwitchPorts(&EthSwtPortLockList[0U]);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
    /* Unlock processed transceivers. */
    EthIf_UnlockTransceivers(&TrcvLockList[0U]);
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */
  }

  DBG_ETHIF_MAINFUNCTIONSTATE_EXIT();
}


STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateEthIfCtrlLinkState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  uint8 CtrlIdx;
  const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;

  /* For all active EthIfCtrls update LinkState by accessing underlying object LinkState. */
  for(CtrlIdx = 0U; CtrlIdx < EthIfCtrlMax; CtrlIdx++)
  {
    EthTrcv_LinkStateType NewTrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
    CONSTP2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState =
    &EthIf_CtrlState[CtrlIdx];

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
    EthIf_SwitchPortGroupIdxType PortGroupIdx;
#else
    TS_PARAM_UNUSED(EthSwtPortLockList);
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
#if (ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
    if(pEthIf_CtrlState->Disabled == FALSE)
    {
#endif /* ETHIF_DEVICEAUTHENTICATION_API == STD_ON */
      if((pEthIf_CtrlState->Mode == ETH_MODE_ACTIVE) &&
         (pEthIf_CtrlState->ModeState == ETHIF_MODESTATE_SET))
      {
#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
        /* If EthIfCtrl references port group or switch (logical port group) */
        PortGroupIdx = ETHIF_PORTGROUPIDX(CtrlIdx);
        if(PortGroupIdx != ETHIF_INVALID_PORTGROUP_IDX)
        {
          NewTrcvLinkState = EthIf_GetPortGroupLinkState(PortGroupIdx, EthSwtPortLockList);
        }
        else
#endif /* ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */
        {
#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
          const uint8 EthIfTrcvIdx = ETHIF_CTRLIDX_ETHIFTRCVIDX(CtrlIdx);

          if(EthIfTrcvIdx == ETHIF_INVALID_TRCV_IDX)
          {
            NewTrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
          }
          else
          {
            TS_AtomicAssign8(NewTrcvLinkState, EthIf_TrcvState[EthIfTrcvIdx].TrcvLinkState);
          }
#else
          NewTrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */
        }
      }
#if (ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
    }
#endif /* ETHIF_DEVICEAUTHENTICATION_API == STD_ON */
    if(pEthIf_CtrlState->TrcvLinkState != NewTrcvLinkState)
    {
      uint8 Idx;
      for(Idx = 0U; Idx < EthIf_Up_TrcvLinkStateChg_MaxNum; Idx++)
      {
        /* Call UL function with the current LinkState. */
        EthIf_Up_TrcvLinkStateChg_FpList[Idx](CtrlIdx, NewTrcvLinkState);
      }

      TS_AtomicAssign8(pEthIf_CtrlState->TrcvLinkState, NewTrcvLinkState);
    }
  }
}

STATIC FUNC(uint8, ETHIF_CODE) EthIf_GetFrameTypeOwner
(
  Eth_FrameType FrameType
)
{
  uint8 FrameTypeOwner;

  DBG_ETHIF_GETFRAMETYPEOWNER_ENTRY(FrameType);

  for(FrameTypeOwner = 0U; FrameTypeOwner < EthIf_FrameTypeOwner_MaxNum; FrameTypeOwner++)
  {
    if(EthIf_LOT_FrameTypeOwnerToFrameType[FrameTypeOwner] == FrameType)
    {
      /* Frame type owner found. */
      break;
    }
  }

  /* Note: If no frame type owner found, FrameTypeOwner = EthIf_Up_FrameTypeOwner_MaxNum is returned
           pointing to dummy entry in function list. No further invalid value protection needed. */

  DBG_ETHIF_GETFRAMETYPEOWNER_EXIT(FrameTypeOwner, FrameType);

  return FrameTypeOwner;
}

#if(ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(boolean, ETHIF_CODE) UpdateRefCnt
(
  P2VAR(EthIf_RefCntType, AUTOMATIC, ETHIF_APPL_DATA) RefCnt,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode,
  Eth_ModeType ReqMode
)
{
  boolean UpdateHardware = FALSE;

  if(ReqMode == ETH_MODE_ACTIVE)
  {
    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    ETHIF_PRECONDITION_ASSERT(*RefCnt < 255U, ETHIF_SETCONTROLLERMODE_SVCID);
    *RefCnt = *RefCnt + 1U;

    if(*RefCnt == 1U)
    {
      *Mode = ETH_MODE_ACTIVE;
      UpdateHardware = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
  }
  else
  {
    SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

    ETHIF_PRECONDITION_ASSERT(*RefCnt > 0, ETHIF_SETCONTROLLERMODE_SVCID);
    *RefCnt = *RefCnt - 1U;

    if(*RefCnt == 0U)
    {
      *Mode = ETH_MODE_DOWN;
      UpdateHardware = TRUE;
    }
    SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
  }

  return UpdateHardware;
}
#else
STATIC FUNC(boolean, ETHIF_CODE) UpdateRefCnt
(
  P2CONST(EthIf_RefCntType, AUTOMATIC, ETHIF_APPL_DATA) RefCnt,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode,
  Eth_ModeType ReqMode
)
{
  const boolean UpdateHardware = (*Mode != ReqMode) ? TRUE : FALSE;
  TS_PARAM_UNUSED(RefCnt);
  *Mode = ReqMode;

  return UpdateHardware;
}
#endif /* ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON */



#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_SetTrcvMode
(
  uint8 EthIfTrcvIdx,
  Eth_ModeType ReqMode,
  boolean LockTrcv
)
{
  if((EthIfTrcvIdx < EthIf_RootPtr->EthIfTrcvMax) && (EthIf_TrcvState[EthIfTrcvIdx].IsInitialized == TRUE))
  {
    Std_ReturnType Res = E_NOT_OK;
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
      &EthIf_TrcvState[EthIfTrcvIdx];

    TS_AtomicAssign8(pEthIf_TrcvState->Mode, ReqMode);
    TS_AtomicAssign8(pEthIf_TrcvState->ModeState, ETHIF_MODESTATE_REQUESTED);

    if(LockTrcv == TRUE)
    {
      boolean localAccessFlag = FALSE;
      CONSTP2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_IsMiiActive = &EthIf_IsMiiActive[EthIfTrcvIdx];
      SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
      if(*pEthIf_IsMiiActive != TRUE)
      {
        TS_AtomicAssign8(*pEthIf_IsMiiActive, TRUE);
        localAccessFlag = TRUE;
      }
      SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

      if(localAccessFlag == TRUE)
      {
        Res = ETHIF_ETHTRCV_SETTRCVMODE
                (ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx), ETH2TRCV_MODE(ReqMode));

        TS_AtomicAssign8(*pEthIf_IsMiiActive, FALSE);
      }
    }
    else
    {
      Res = ETHIF_ETHTRCV_SETTRCVMODE
              (ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx) , ETH2TRCV_MODE(ReqMode));
    }

    if(Res == E_OK)
    {
      SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
    }
    else
    {
      /* Repeat transceiver set operation in the next MainFunction() call. */
      ELAPSE_TIMER(pEthIf_TrcvState->SetRepeatTimer);
    }
  }
}

STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTrcvMode
(
  uint8 EthIfTrcvIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA) Mode
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  EthTrcv_ModeType TrcvMode;

  RetVal = ETHIF_ETHTRCV_GETTRCVMODE(ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx), &TrcvMode);
  *Mode = TRCV2ETH_MODE(TrcvMode);

   return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetLinkState
(
  uint8 EthIfTrcvIdx,
  P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHIF_APPL_DATA) TrcvLinkStatePtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  RetVal = ETHIF_ETHTRCV_GETLINKSTATE(ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx), TrcvLinkStatePtr);

  return RetVal;
}

STATIC FUNC(void, ETHIF_CODE) EthIf_FindEthIfTrcvAccess
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
)
{
  uint8 EthIfTrcvIdx;
  const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

  /* Here transceivers directly controlled by EthIfController are processed. Only those that
   * are not initialized or stabled DOWN will not be updated in MainFunction. */
  for(EthIfTrcvIdx = 0U; EthIfTrcvIdx < EthIfTrcvMax; EthIfTrcvIdx++)
  {
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
      &EthIf_TrcvState[EthIfTrcvIdx];
    if((!(pEthIf_TrcvState->IsInitialized)) ||
      ((pEthIf_TrcvState->Mode == ETH_MODE_DOWN) &&
       (pEthIf_TrcvState->ModeState == ETHIF_MODESTATE_SET)))
    {
      /* nothing to be done for this transceiver */
    }
    else
    {
      TrcvLockList[EthIfTrcvIdx] = TRUE;
    }
  }
}

STATIC FUNC(void, ETHIF_CODE) EthIf_InitTransceivers(void)
{
  uint8 EthIfTrcvIdx;
  const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

  for(EthIfTrcvIdx = 0U; EthIfTrcvIdx < EthIfTrcvMax; EthIfTrcvIdx++)
  {
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
      &EthIf_TrcvState[EthIfTrcvIdx];
#if (ETHIF_INIT_CTRL_TRCV == STD_ON)
    Std_ReturnType RetVal;
    const uint8 TrcvIdx = ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx)->EthTrcvIdx;
    /* Note: Since this call is executed only within EthIf_Init(), and no other function that
     * access transceivers can be called before initialization has finished, it is safe to
     * omit MII access protection mechanism. */
    RetVal = EthTrcv_TransceiverInit(TrcvIdx, 0U);

    if(RetVal == E_OK)
    {
      TS_AtomicAssign8(pEthIf_TrcvState->IsInitialized, TRUE);
    }
#else
    TS_AtomicAssign8(pEthIf_TrcvState->IsInitialized, TRUE);
#endif /* ETHIF_INIT_CTRL_TRCV == STD_ON */
  }
}

STATIC FUNC(void, ETHIF_CODE)  EthIf_UpdateEthIfTrcvState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
)
{
  uint8 EthIfTrcvIdx;
  Std_ReturnType RetVal;
  Eth_ModeType NewMode;
  EthTrcv_LinkStateType TrcvLinkState;
  const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

  for(EthIfTrcvIdx = 0U; EthIfTrcvIdx < EthIfTrcvMax; EthIfTrcvIdx++)
  {
    CONSTP2VAR(EthIf_TrcvStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvState =
      &EthIf_TrcvState[EthIfTrcvIdx];
    /* Process transceiver if initialized and locked. */
    if((pEthIf_TrcvState->IsInitialized) && (TrcvLockList[EthIfTrcvIdx] == TRUE))
    {
      /* Update transceiver mode state */
      if(pEthIf_TrcvState->ModeState == ETHIF_MODESTATE_REQUESTED)
      {
        RetVal = EthIf_GetTrcvMode(EthIfTrcvIdx, &NewMode);
        if(RetVal == E_OK)
        {
          if(NewMode == pEthIf_TrcvState->Mode)
          {
#if(ETHIF_TRCV_ASYNC_ENABLE == STD_OFF)
            EthIf_TrcvModeIndication(EthIfTrcvIdx, NewMode);
#endif /* ETHIF_TRCV_ASYNC_ENABLE == STD_OFF */
          }
          else
          {
            EthIf_TickTimer(&pEthIf_TrcvState->SetRepeatTimer);
            if(CHECK_TIMER(pEthIf_TrcvState->SetRepeatTimer))
            {
              /* Timer elapsed - repeat transceiver set operation. */
              EthIf_SetTrcvMode(EthIfTrcvIdx, pEthIf_TrcvState->Mode, FALSE);
              SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
            }
          }
        }
      }

      /* Update transceiver LinkState */
      if((pEthIf_TrcvState->Mode == ETH_MODE_ACTIVE) &&
         (pEthIf_TrcvState->ModeState == ETHIF_MODESTATE_SET))
      {
        RetVal = EthIf_GetLinkState(EthIfTrcvIdx, &TrcvLinkState);
        if(RetVal == E_OK)
        {
          pEthIf_TrcvState->TrcvLinkState = TrcvLinkState;
#if (ETHIF_SET_MODE_TIMEOUT > 0U)
          /* A mechanism to repeat set transceiver mode operation if link is DOWN for user
           * specified time duration. */
          if(TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
          {
            SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
          }
          else
          {
            EthIf_TickTimer(&pEthIf_TrcvState->SetRepeatTimer);
            if(CHECK_TIMER(pEthIf_TrcvState->SetRepeatTimer))
            {
              EthIf_SetTrcvMode(EthIfTrcvIdx, pEthIf_TrcvState->Mode, FALSE);
              SET_TIMER_REPEAT_SETMODE(pEthIf_TrcvState->SetRepeatTimer);
            }
          }
#endif
#if ((ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR403) && \
    (ETHIF_ETH_SUPPORT_API != ETHIF_ETH_SUPPORT_API_ASR422))
          if(TrcvLinkState == ETHTRCV_LINK_STATE_ACTIVE)
          {
            uint32 EthIf_PhysSigQual;
            CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_TrcvSignalQual =
              &EthIf_TrcvSignalQual[EthIfTrcvIdx];

            RetVal = ETHIF_ETHTRCV_GETPHYSIGQUAL
                       (ETHIF_CFG_GET_TRANSCEIVER_PTR(EthIfTrcvIdx), &EthIf_PhysSigQual);
            if(RetVal == E_OK)
            {
              pEthIf_TrcvSignalQual->HighestSignalQuality =
                                   ETHIF_MAX(
                                               EthIf_PhysSigQual,
                                               pEthIf_TrcvSignalQual->HighestSignalQuality
                                            );

              pEthIf_TrcvSignalQual->LowestSignalQuality =
                                   ETHIF_MIN(
                                               EthIf_PhysSigQual,
                                               pEthIf_TrcvSignalQual->LowestSignalQuality
                                             );
              pEthIf_TrcvSignalQual->ActualSignalQuality = EthIf_PhysSigQual;
            }
          }
#endif
        }
      }
      else
      {
        TS_AtomicAssign8(pEthIf_TrcvState->TrcvLinkState, ETHTRCV_LINK_STATE_DOWN);
      }
    }
  }
}
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */


#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_SetPortGroupMode
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  Eth_ModeType ReqMode
)
{
  uint16 BeginIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
  uint8 NumPorts = ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);
  uint16 PortListIdx;
  EthIf_EthSwtPortLockListType EthSwtPortLockList;

  /* Clear port lock list */
  TS_MemBZero(&EthSwtPortLockList[0U], ETHIF_PORTS_ARRAYSIZE);

  /* Find ports referenced by this port group. */
  EthIf_FindTrcvPortGroup(PortGroupIdx, &EthSwtPortLockList[0U]);

  /* Lock affected Ports. */
  EthIf_LockSwitchPorts(&EthSwtPortLockList[0U]);

  for(PortListIdx = BeginIdx; PortListIdx < (BeginIdx + NumPorts); PortListIdx++)
  {
    /* Return value is ignored, because there is protection mechanism in MainFunctionState()
     * that will take care to repeat set operation if unsuccessful. */
    (void) EthIf_SetPortMode(
                              ETHIF_CFG_ETHIFPORTIDX(PortListIdx),
                              ReqMode,
                              0U,
                              &EthSwtPortLockList[0U]
                            );
  }
  /* Unlock ports locked in this port group operation. */
  EthIf_UnlockSwitchPorts(&EthSwtPortLockList[0U]);

  return;
}

STATIC FUNC(boolean, ETHIF_CODE) EthIf_CheckAllPortsDown
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx
)
{
  boolean RetVal = FALSE;
  uint16 PortListIdx;

  const uint16 BeginIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
  const uint8 NumPorts = ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);

  for(PortListIdx = BeginIdx; PortListIdx < (BeginIdx + NumPorts); PortListIdx++)
  {
    const EthIf_SwtPortIdxType PortIdx = ETHIF_CFG_ETHIFPORTIDX(PortListIdx);
    CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
      &EthIf_PortState[PortIdx];

    if((pEthIf_PortState->Mode == ETH_MODE_ACTIVE) ||
       (pEthIf_PortState->ModeState == ETHIF_MODESTATE_REQUESTED))
    {
      break;
    }
  }

  if(PortListIdx == (BeginIdx+NumPorts))
  {
    /* Active port was not found */
    RetVal = TRUE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetPortMode
(
  EthIf_SwtPortIdxType PortIdx,
  Eth_ModeType ReqMode,
  uint16 ShutdownTimeout,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
    &EthIf_PortState[PortIdx];

  const boolean UpdateHw = UpdateRefCnt(
                                         &pEthIf_PortState->RefCnt,
                                         &pEthIf_PortState->Mode,
                                         ReqMode
                                       );

  if(UpdateHw == TRUE)
  {
    /* In case of DOWN transition, it needs to be checked if shut down needs to be delayed. */
    if((ReqMode == ETH_MODE_DOWN) && (ShutdownTimeout > 0))
    {
      SET_SWITCHOFF_TIMER(PortIdx, ShutdownTimeout);
      /* Actual hardware call will be done in MainFunctionState(), after timer elapses. */
      pEthIf_PortState->ModeState = ETHIF_MODESTATE_REQUESTED;
      RetVal = E_OK;
    }
    else
    {
      const uint8 TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);
      /* If a switch port is not connected to a transceiver, or transceiver has been successfully
       * locked it is safe to perform operation on it. */
      if((TrcvIdx == ETHIF_INVALID_TRCV_IDX) || (EthSwtPortLockList[PortIdx] == TRUE))
      {
        RetVal = ETHIF_ETHSWT_SETSWITCHPORTMODE
                 (
                   ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortIdx),
                   ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortIdx),
                   ETH2TRCV_MODE(ReqMode)
                  );
      }

      if(RetVal == E_OK)
      {
        TS_AtomicAssign8(pEthIf_PortState->ModeState, ETHIF_MODESTATE_SET);
      }
      else
      {
        TS_AtomicAssign8(pEthIf_PortState->ModeState, ETHIF_MODESTATE_REQUESTED);
        /* Repeat operation in the proceeding MainFunctionState() call. */
        ELAPSE_TIMER(pEthIf_PortState->SetRepeatTimer);
      }
    }
  }
  else
  {
    RetVal = E_OK;
  }

  return RetVal;
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateEthIfCtrlState(void)
{
  uint8 CtrlIdx;
  Std_ReturnType RetVal;

  /* Search for EthIfControllers that are in REQUESTED state of DOWN transition, i.e. waiting for
   * port group ports to go DOWN prior shutting down physical controller. */
  const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;
  for(CtrlIdx = 0U; CtrlIdx < EthIfCtrlMax; CtrlIdx++)
  {
    CONSTP2VAR(EthIf_CtrlStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_CtrlState =
      &EthIf_CtrlState[CtrlIdx];
    if(pEthIf_CtrlState->ModeState == ETHIF_MODESTATE_REQUESTED)
    {
      const EthIf_SwitchPortGroupIdxType PortGroupIdx = ETHIF_PORTGROUPIDX(CtrlIdx);

      if(ETHIF_INVALID_PORTGROUP_IDX != PortGroupIdx)
      {
        /* If all ports are in DOWN state, proceed with shutting down phy controller */
        if(EthIf_CheckAllPortsDown(PortGroupIdx) == TRUE)
        {
          EthIf_ModeStateType PhyCtrlModeState;
          /* ModeState of EthIfCtrl depends on a ModeState of EthPhyCtrl. */
          RetVal = EthIf_SetPhyCtrlMode(
                                         ETHIF_CFG_GET_CTRL_PTR(CtrlIdx)->PhysCtrlIdx,
                                         pEthIf_CtrlState->Mode,
                                         &PhyCtrlModeState
                                       );

          if(RetVal == E_OK)
          {
            TS_AtomicAssign8(pEthIf_CtrlState->ModeState, PhyCtrlModeState);
          }
        }
      }
    }
  }
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UpdatePortState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  EthIf_SwtPortIdxType PortIdx;
  Std_ReturnType RetVal;
  uint32 EthIf_PortSigQuality;
  const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;

  /* Perform delayed port set operations or operations that need to be repeated due to an error. */

  for(PortIdx = 0U; PortIdx < SwtPortsMax; PortIdx++)
  {
    CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
      &EthIf_PortState[PortIdx];
    /* Port SwitchOffTimer ticks down till 0 no matter of actual Mode or ModeState. It insures
     * that once port is ready to be switched off (Mode = DOWN, RefCnt = 0), it will not happen
     * before ETHIF_SWITCH_OFF_PORT_TIMEDELAY from the last call to
     * EthIf_SwitchPortGroupRequestMode(DOWN). */
    EthIf_Tick_SwitchoffTimer(PortIdx);

    if(pEthIf_PortState->ModeState == ETHIF_MODESTATE_REQUESTED)
    {
      if(pEthIf_PortState->Mode == ETH_MODE_ACTIVE)
      {
        /* Tick SETMODE timer only in case port is ACTIVE - only then port link state is relevant */
        EthIf_TickTimer(&pEthIf_PortState->SetRepeatTimer);
        /* Get port signal quality for an active port */
        RetVal = ETHIF_ETHSWT_GETPORTSIGNALQUALITY
                 (
                   ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortIdx),
                   ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortIdx),
                   &EthIf_PortSigQuality
                  );

        if(RetVal == E_OK)
        {
          CONSTP2VAR(EthIf_SignalQualityResultType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortSignalQual =
            &EthIf_PortSignalQual[PortIdx];
          pEthIf_PortSignalQual->HighestSignalQuality =
                                            ETHIF_MAX(
                                                        EthIf_PortSigQuality,
                                                        pEthIf_PortSignalQual->HighestSignalQuality
                                                      );
          pEthIf_PortSignalQual->LowestSignalQuality =
                                            ETHIF_MIN(
                                                        EthIf_PortSigQuality,
                                                        pEthIf_PortSignalQual->LowestSignalQuality
                                                      );
          pEthIf_PortSignalQual->ActualSignalQuality = EthIf_PortSigQuality;
        }
      }

      /* Call set mode operation if following cases:
       * 1) SetRepeatTimer elapsed - this can happen because previous set mode operation failed
       *    or port links state is DOWN for a configurable time period.
       * 2) Switch-off timer elapsed (set by EthIf_SwitchPortGroupRequestMode())*/
      if(CHECK_TIMER(pEthIf_PortState->SetRepeatTimer) || CHECK_SWITCHOFF_TIMER(PortIdx))
      {
        const uint8 TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);

        /* If a switch port is not connected to a transceiver, or transceiver has been successfully
         * locked it is safe to perform operation on it. */
        if((TrcvIdx == ETHIF_INVALID_TRCV_IDX) || (EthSwtPortLockList[PortIdx] == TRUE))
        {
          RetVal = ETHIF_ETHSWT_SETSWITCHPORTMODE
                   (
                     ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortIdx),
                     ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortIdx),
                     ETH2TRCV_MODE(pEthIf_PortState->Mode)
                    );
        }
        else
        {
          RetVal = E_NOT_OK;/* Otherwise repeat the operation in the next main function invocation*/
        }

        if(RetVal == E_OK)
        {
          SET_TIMER_REPEAT_SETMODE(pEthIf_PortState->SetRepeatTimer);
          TS_AtomicAssign8(pEthIf_PortState->ModeState, ETHIF_MODESTATE_SET);
        }
        else
        {
          /* Repeat operation in the next MainFunctionState() call.*/
          ELAPSE_TIMER(pEthIf_PortState->SetRepeatTimer);
        }
      }
    }
  }
}
#endif /*ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON */

#if(ETHIF_PORTGROUPSUPPORT == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateBswMPortGroupSwitchOffTimer(void)
{
  EthIf_SwitchPortGroupIdxType PortGroupIdx;

  for(PortGroupIdx = 0U; PortGroupIdx < EthIf_RootPtr->SwtPortGroupsMax; PortGroupIdx++)
  {
    if((ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_NO_REF) ||
       (ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_LINK_INFO))
    {
      CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) PortGroupStatePtr =
        &EthIf_PortGroupState[PortGroupIdx];

      /* Process port group switch-off timer. */
      if((PortGroupStatePtr->Mode == ETHTRCV_MODE_DOWN) &&
         (PortGroupStatePtr->ModeState == ETHIF_MODESTATE_REQUESTED))
      {
        SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

        if(PortGroupStatePtr->SwitchOffTimer == 0U)
        {
          /* Propagate port group mode DOWN transition to underlying switch ports. */
          EthIf_SetPortGroupMode(PortGroupIdx, ETH_MODE_DOWN);
          PortGroupStatePtr->ModeState = ETHIF_MODESTATE_SET;
        }
        else
        {
          PortGroupStatePtr->SwitchOffTimer--;
        }

        SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
      }
    }
  }
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateBswMPortGroupLinkState
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  EthIf_SwitchPortGroupIdxType PortGroupIdx;
  EthTrcv_LinkStateType NewLinkState;
  EthTrcv_LinkStateType OldLinkState;
  const uint8 SwtPortGroupsMax = EthIf_RootPtr->SwtPortGroupsMax;

  for(PortGroupIdx = 0U; PortGroupIdx < SwtPortGroupsMax; PortGroupIdx++)
  {
    if((ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_NO_REF) ||
       (ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_LINK_INFO))
    {
      CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) PortGroupStatePtr =
        &EthIf_PortGroupState[PortGroupIdx];

      /* Process port group link state accumulation. */
      OldLinkState = PortGroupStatePtr->TrcvLinkState;

      if((PortGroupStatePtr->Mode == ETHTRCV_MODE_DOWN) &&
         (PortGroupStatePtr->ModeState == ETHIF_MODESTATE_SET))
      {
        NewLinkState = ETHTRCV_LINK_STATE_DOWN;
        PortGroupStatePtr->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
      }
      else
      {
        NewLinkState = EthIf_GetPortGroupLinkState(PortGroupIdx, EthSwtPortLockList);
      }

      if(OldLinkState != NewLinkState)
      {
        BswM_EthIf_PortGroupLinkStateChg(PortGroupIdx, NewLinkState);
      }
    }
  }
}

#endif /* ETHIF_PORTGROUPSUPPORT == STD_ON */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
STATIC FUNC(EthTrcv_LinkStateType, ETHIF_CODE) EthIf_GetPortLinkState
(
  EthIf_SwtPortIdxType PortIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
    &EthIf_PortState[PortIdx];
  if(pEthIf_PortState->CycleCnt == EthIf_MainFntCycleCnt)
  {
    /* Link state is already updated for this port-> just return the value. */
  }
  else
  {
    if((pEthIf_PortState->Mode == ETH_MODE_ACTIVE) &&
       (pEthIf_PortState->ModeState == ETHIF_MODESTATE_SET))
    {
      EthTrcv_LinkStateType NewLinkState = ETHTRCV_LINK_STATE_DOWN;
      Std_ReturnType RetVal = E_NOT_OK;
      const uint8 TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);

      /* If switch port is not connected to a transceiver, or transceiver has been successfully
       * locked it is safe to perform operation on it. */
      if((TrcvIdx == ETHIF_INVALID_TRCV_IDX) || (EthSwtPortLockList[PortIdx] == TRUE))
      {
        RetVal = ETHIF_ETHSWT_GETLINKSTATE
                 (
                   ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortIdx),
                   ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortIdx),
                   &NewLinkState
                 );
      }

      if(RetVal == E_OK)
      {
        TS_AtomicAssign8(pEthIf_PortState->TrcvLinkState, NewLinkState);
        /* Link state is up-to-date -> mark it to avoid double reading */
        TS_AtomicAssign8(pEthIf_PortState->CycleCnt, EthIf_MainFntCycleCnt);
#if (ETHIF_SET_MODE_TIMEOUT > 0U)
        /* A mechanism to repeat set mode operation for a port if link state is DOWN for a user
         * specified time duration. */
        if(NewLinkState == ETHTRCV_LINK_STATE_ACTIVE)
        {
          SET_TIMER_REPEAT_SETMODE(pEthIf_PortState->SetRepeatTimer);
        }
        else
        {
          EthIf_TickTimer(&pEthIf_PortState->SetRepeatTimer);
          if(CHECK_TIMER(pEthIf_PortState->SetRepeatTimer))
          {
            (void) ETHIF_ETHSWT_SETSWITCHPORTMODE
                   (
                     ETHIF_ETHIFPORTIDX_ETHSWTIDX(PortIdx),
                     ETHIF_ETHIFPORTIDX_ETHSWTPORTIDX(PortIdx),
                     ETH2TRCV_MODE(pEthIf_PortState->Mode)
                    );
            SET_TIMER_REPEAT_SETMODE(pEthIf_PortState->SetRepeatTimer);
          }
        }
#endif
      }
      /* in case port link state read operation was unsuccessful -> old link state is returned */
    }
    else
    {
      TS_AtomicAssign8(pEthIf_PortState->TrcvLinkState, ETHTRCV_LINK_STATE_DOWN);
    }
  }

  return pEthIf_PortState->TrcvLinkState;
}

STATIC FUNC(EthTrcv_LinkStateType, ETHIF_CODE) EthIf_GetPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortGroupState =
    &EthIf_PortGroupState[PortGroupIdx];
  if(pEthIf_PortGroupState->CycleCnt == EthIf_MainFntCycleCnt)
  {
    /* Link state is already updated for this group -> just return the value. */
  }
  else
  {
    if(ETHIF_GET_PORTGORUP_REFTYPE(PortGroupIdx) == ETHIF_REFTYPE_SWITCH)
    {
      /* logical port group (switch) */
      EthIf_UpdateLogicalPortGroupLinkState(PortGroupIdx, EthSwtPortLockList);
    }
    else
    {
      /* user defined port group */
      EthIf_UpdateUserPortGroupLinkState(PortGroupIdx, EthSwtPortLockList);
    }
  }
  return pEthIf_PortGroupState->TrcvLinkState;
}


STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateLogicalPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  uint16 PortListIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
  uint16 LastPortListIdx = PortListIdx + ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);
  boolean ExitLoop = FALSE;
  CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortGroupState =
    &EthIf_PortGroupState[PortGroupIdx];

  /* If this is a logical port group that represents the complete Ethernet switch
   * then port group link state is a link state of a host port. */
  while((PortListIdx < LastPortListIdx) && (ExitLoop == FALSE))
  {
    const EthIf_SwtPortIdxType PortIdx = ETHIF_CFG_ETHIFPORTIDX(PortListIdx);

    if(ETHIF_ETHIFPORTIDX_PORTROLE(PortIdx) == ETHIF_PORTROLE_HOST)
    {
      pEthIf_PortGroupState->TrcvLinkState = EthIf_GetPortLinkState(PortIdx, EthSwtPortLockList);

      /* Link state is up-to-date -> mark it to avoid double reading */
      pEthIf_PortGroupState->CycleCnt = EthIf_MainFntCycleCnt;
      ExitLoop = TRUE;
    }
    PortListIdx++;
  }
}


STATIC FUNC(void, ETHIF_CODE) EthIf_UpdateUserPortGroupLinkState
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  uint16 PortListIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
  uint16 LastPortListIdx = PortListIdx + ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);
  boolean ExitLoop = FALSE;
  CONSTP2VAR(EthIf_PortGroupStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortGroupState =
    &EthIf_PortGroupState[PortGroupIdx];

  while((PortListIdx < LastPortListIdx) && (ExitLoop == FALSE))
  {
    const EthIf_SwtPortIdxType PortIdx = ETHIF_CFG_ETHIFPORTIDX(PortListIdx);
    const EthTrcv_LinkStateType PortLinkState = EthIf_GetPortLinkState(PortIdx, EthSwtPortLockList);

    /* Host and up-link ports come first in the port group */
    if((ETHIF_ETHIFPORTIDX_PORTROLE(PortIdx) == ETHIF_PORTROLE_HOST) ||
       (ETHIF_ETHIFPORTIDX_PORTROLE(PortIdx) == ETHIF_PORTROLE_UPLINK))
    {
      /* If either of host or uplink ports link state is DOWN -> port group link state is DOWN.*/
      if(PortLinkState == ETHTRCV_LINK_STATE_DOWN)
      {
        pEthIf_PortGroupState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
        pEthIf_PortGroupState->CycleCnt = EthIf_MainFntCycleCnt;
        ExitLoop = TRUE;
      }
    }
    else if ((ETHIF_ETHIFPORTIDX_PORTROLE(PortIdx) == ETHIF_PORTROLE_STANDARD) &&
             (PortLinkState == ETHTRCV_MODE_ACTIVE))
    {
      /* If host and uplink ports are ACTIVE, and there is at least one standard port ACTIVE ->
       * port group link state is ACTIVE. */
      pEthIf_PortGroupState->TrcvLinkState = ETHTRCV_LINK_STATE_ACTIVE;
      pEthIf_PortGroupState->CycleCnt = EthIf_MainFntCycleCnt;
      ExitLoop = TRUE;
    }
    else
    {
      /* nothing to do */
    }
    PortListIdx++;
  }

  if(pEthIf_PortGroupState->CycleCnt != EthIf_MainFntCycleCnt)
  {
    /* New LinkState have not been yet set -> host and uplink ports are active and all the other
     * (STANDARD) ports are DOWN -> port group link state is DOWN. */
    pEthIf_PortGroupState->TrcvLinkState = ETHTRCV_LINK_STATE_DOWN;
    pEthIf_PortGroupState->CycleCnt = EthIf_MainFntCycleCnt;
  }
}


STATIC FUNC(void, ETHIF_CODE) EthIf_FindPortTrcvAccess
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  EthIf_SwtPortIdxType PortIdx;
  uint8 TrcvIdx;
  const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;
  /* Only if port is stable DOWN, it will not be processed in MainFunction. */

  for(PortIdx = 0U; PortIdx < SwtPortsMax; PortIdx++)
  {
    CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
      &EthIf_PortState[PortIdx];
    if((pEthIf_PortState->Mode == ETH_MODE_DOWN) &&
       (pEthIf_PortState->ModeState == ETHIF_MODESTATE_SET))
    {
      /* nothing to be done for this port */
    }
    else
    {
      /* Mark port for locking if connected to Trcv */
      TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);

      if(TrcvIdx != ETHIF_INVALID_TRCV_IDX)
      {
        EthSwtPortLockList[PortIdx] = TRUE;
      }
    }
  }
}

STATIC FUNC(void, ETHIF_CODE) EthIf_FindTrcvPortGroup
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  const uint16 BeginIdx = ETHIF_GET_PORTGROUP_BEGINIDX(PortGroupIdx);
  const uint8 NumPorts = ETHIF_GET_PORTGORUP_NUMPORTS(PortGroupIdx);
  uint16 PortListIdx;

  /* mark transceivers referenced by ports of this port group */
  for(PortListIdx = BeginIdx; PortListIdx < (BeginIdx + NumPorts); PortListIdx++)
  {
    const EthIf_SwtPortIdxType PortIdx = ETHIF_CFG_ETHIFPORTIDX(PortListIdx);
    const uint8 TrcvIdx = ETHIF_ETHIFPORTIDX_ETHTRCVIDX(PortIdx);

    if(TrcvIdx != ETHIF_INVALID_TRCV_IDX)
    {
      EthSwtPortLockList[PortIdx] = TRUE;
    }
  }
}
#endif /* (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON) */


#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) || (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_TickTimer
(
  P2VAR(EthIf_TimerType, AUTOMATIC, ETHIF_APPL_DATA) Timer
)
{
#if(ETHIF_SET_MODE_TIMEOUT > 0)
  /* Ticks down port SETMODE timer */
  if(*Timer > 0U)
  {
    TS_AtomicAssign16((*Timer), (*Timer) - 1U);
  }
#else
  TS_PARAM_UNUSED(Timer);
#endif /* ETHIF_SET_MODE_TIMEOUT > 0 */
}
#endif /* (ETHIF_TRCV_SUPPORT_ENABLE == STD_ON) || (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON) */

#if(ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_Tick_SwitchoffTimer
(
  EthIf_SwtPortIdxType EthIfPortIdx
)
{
  CONSTP2VAR(EthIf_PortStateType, AUTOMATIC, ETHIF_APPL_DATA) pEthIf_PortState =
    &EthIf_PortState[EthIfPortIdx];
  if(pEthIf_PortState->SwitchOffTimer > 0U)
  {
    (TS_AtomicAssign16(pEthIf_PortState->SwitchOffTimer,
                      (pEthIf_PortState->SwitchOffTimer - 1U)));
  }
  else
  {
    TS_PARAM_UNUSED(EthIfPortIdx);
  }
}
#endif /* (ETHIF_PORT_STATE_MGMT_ENABLED == STD_ON) */


#if(ETHIF_TRCV_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_LockTransceivers
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
)
{
  uint8 TrcvIdx;
  const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

  SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

  /* Lock marked transceivers which are not already locked */
  for(TrcvIdx = 0U; TrcvIdx < EthIfTrcvMax; TrcvIdx++)
  {
    if(TrcvLockList[TrcvIdx] == TRUE)
    {
      if(EthIf_IsMiiActive[TrcvIdx] != TRUE)
      {
         TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], TRUE);
      }
      else
      {
        TrcvLockList[TrcvIdx] = FALSE;
      }
    }
  }

  SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UnlockTransceivers
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) TrcvLockList
)
{
  uint8 TrcvIdx;
  const uint8 EthIfTrcvMax = EthIf_RootPtr->EthIfTrcvMax;

  /* Unlock marked transceivers */
  for(TrcvIdx = 0U; TrcvIdx < EthIfTrcvMax; TrcvIdx++)
  {
    if(TrcvLockList[TrcvIdx] == TRUE)
    {
      TS_AtomicAssign8(EthIf_IsMiiActive[TrcvIdx], FALSE);
    }
  }
}
#endif /* ETHIF_TRCV_SUPPORT_ENABLE == STD_ON */

#if (ETHIF_SWT_SUPPORT_ENABLE == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_LockSwitchPorts
(
  P2VAR(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  uint8 PortIdx;
  const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;

  SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();

  /* Lock marked ports which are not already locked */
  for(PortIdx = 0U; PortIdx < SwtPortsMax; PortIdx++)
  {
    if(EthSwtPortLockList[PortIdx] == TRUE)
    {
      if(EthIf_IsPortActive[PortIdx] != TRUE)
      {
         TS_AtomicAssign8(EthIf_IsPortActive[PortIdx], TRUE);
      }
      else
      {
         EthSwtPortLockList[PortIdx] = FALSE;
      }
    }
  }

  SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
}

STATIC FUNC(void, ETHIF_CODE) EthIf_UnlockSwitchPorts
(
  P2CONST(boolean, AUTOMATIC, ETHIF_APPL_DATA) EthSwtPortLockList
)
{
  uint8 PortIdx;
  const uint8 SwtPortsMax =  EthIf_RootPtr->SwtPortsMax;

  /* Unlock marked ports */
  for(PortIdx = 0U; PortIdx < SwtPortsMax; PortIdx++)
  {
    if(EthSwtPortLockList[PortIdx] == TRUE)
    {
      TS_AtomicAssign8(EthIf_IsPortActive[PortIdx], FALSE);
    }
  }
}
#endif /* ETHIF_SWT_SUPPORT_ENABLE == STD_ON */

STATIC FUNC(boolean, ETHIF_CODE) EthIf_RxIndicationInternal
(
  uint8 EthIfCtrlIdx,
  Eth_FrameType FrameType,
  boolean isBroadcast,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
)
{
  Eth_FrameType TmpFrameType = FrameType;
  uint16 TmpLenByte = LenByte;
  boolean EthIfCtrlIdx_Found = FALSE;
  uint8 PayLoadOffSet = 0U;

#if(ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON)
  if(EthIf_CtrlState[EthIfCtrlIdx].Mode == ETH_MODE_ACTIVE)
#endif /* ETHIF_VIRTUALCTRL_SUPPORT_ENABLE == STD_ON */
  {
#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
    /* get VID of this controller */
    const uint16 EthIfVID = ETHIF_CFG_GET_CTRL_PTR(EthIfCtrlIdx)->VID;

    if((LenByte > ETHIF_ETH_VLAN_HEADER_LENGTH) && (ETHIF_VLAN_TPID_CHECK(FrameType)))
    {
      if(EthIfVID == ETHIF_VLAN_VID_GET(&DataPtr[ETHIF_ETH_VLAN_TCI_OFFSET]))
      {
        /* VLAN ID match: This frame corresponds to the current EthIfCtrlIdx. */
        EthIfCtrlIdx_Found = TRUE;

        /* manipulate, frame type buffer pointer and buffer length */
        TmpFrameType =
          (Eth_FrameType)(ETHIF_ETH_TYPE_GET(&DataPtr[ETHIF_ETH_VLAN_TYPE_OFFSET]));
        PayLoadOffSet = ETHIF_ETH_VLAN_PAYLOAD_OFFSET;
        TmpLenByte = LenByte - ETHIF_ETH_VLAN_HEADER_LENGTH;
      }
    }
    else
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */
    {
#if(ETHIF_VLAN_SUPPORT_ENABLE == STD_ON)
      /* check if this is a VLAN controller */
      if(ETHIF_VLAN_VID_UNUSED == EthIfVID)
#endif /* ETHIF_VLAN_SUPPORT_ENABLE */
      {
        EthIfCtrlIdx_Found = TRUE;
        /* Use initial value for PayLoadOffSet, TmpFrameType and TmpLenByte. */
      }
    }

    if(EthIfCtrlIdx_Found)
    {
      const uint8 FrameTypeOwner = EthIf_GetFrameTypeOwner(TmpFrameType);
      /* A temporary solution to keep pointer const prior [Up]_RxIndication() is updated
         to ASR 4.3.0 */
      uint8 PhysAddr_copy[ETHIF_MAC_ADDR_LEN];
      TS_MemCpy(&PhysAddr_copy[0U], PhysAddrPtr, ETHIF_MAC_ADDR_LEN);

      EthIf_Up_RxIndication_FpList[FrameTypeOwner]
      (
        EthIfCtrlIdx,
        TmpFrameType,
        isBroadcast,
        &PhysAddr_copy[0U],
        &DataPtr[PayLoadOffSet],
        TmpLenByte
      );
    }
  }
  return EthIfCtrlIdx_Found;
}

#if(ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_EnableRelatedEthIfCtrls
(
  uint8 CtrlIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_ENABLERELATEDETHIFCTRLS_ENTRY(CtrlIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_ENABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_ENABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(FALSE == EthIf_PhyCtrlState[ETHIF_PHYS_CTRLIDX(CtrlIdx)].IsInitialized)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_ENABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    uint8 Idx;
    const uint8 EthCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(CtrlIdx))->EthCtrlIdx;
    const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;
    P2CONST(EthIf_PhysCtrlType, AUTOMATIC, ETHIF_APPL_DATA) PhysCtrlCfgPtr;
    RetVal = E_OK;

    for(Idx = 0U; Idx < EthIfCtrlMax; Idx++)
    {
      PhysCtrlCfgPtr = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(Idx));
      if((EthCtrlIdx == PhysCtrlCfgPtr->EthCtrlIdx) && (Idx != CtrlIdx))
      {
        EthIf_CtrlState[Idx].Disabled = FALSE;
      }
    }
  }
  DBG_ETHIF_ENABLERELATEDETHIFCTRLS_EXIT(RetVal, CtrlIdx);
  return RetVal;
}

FUNC(Std_ReturnType, ETHIF_CODE) EthIf_DisableRelatedEthIfCtrls
(
  uint8 CtrlIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_ETHIF_DISABLERELATEDETHIFCTRLS_ENTRY(CtrlIdx);

#if(ETHIF_DEV_ERROR_DETECT == STD_ON)
  if(EthIf_Initialized != TRUE)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_DISABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_NOT_INITIALIZED);
  }
  else if(CtrlIdx >= EthIf_RootPtr->EthIfCtrlMax)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_DISABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_INV_CTRL_IDX);
  }
  else if(FALSE == EthIf_PhyCtrlState[ETHIF_PHYS_CTRLIDX(CtrlIdx)].IsInitialized)
  {
    ETHIF_DET_REPORT_ERROR(ETHIF_DISABLERELATEDETHIFCTRLS_SVCID, ETHIF_E_HW_NOT_INITIALIZED);
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    uint8 Idx;
    const uint8 EthCtrlIdx = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(CtrlIdx))->EthCtrlIdx;
    const uint8 EthIfCtrlMax = EthIf_RootPtr->EthIfCtrlMax;
    P2CONST(EthIf_PhysCtrlType, AUTOMATIC, ETHIF_APPL_DATA) PhysCtrlCfgPtr;
    RetVal = E_OK;

    for(Idx = 0U; Idx < EthIfCtrlMax; Idx++)
    {
      PhysCtrlCfgPtr = ETHIF_CFG_GET_PHYSCTRL_PTR(ETHIF_PHYS_CTRLIDX(Idx));
      if((EthCtrlIdx == PhysCtrlCfgPtr->EthCtrlIdx) && (Idx != CtrlIdx))
      {
        EthIf_CtrlState[Idx].Disabled = TRUE;
      }
    }

  }
  DBG_ETHIF_DISABLERELATEDETHIFCTRLS_EXIT(RetVal, CtrlIdx);
  return RetVal;
}
#endif /* ETHIF_DEVICEAUTHENTICATION_API == STD_ON */

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
STATIC FUNC(void, ETHIF_CODE) EthIf_IncMeasurementCnt
(
  void
)
{
  SchM_Enter_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
  /* !LINKSTO EthIf.ASR43.SWS_EthIf_00313, 1 */
  if(EthIf_MeasurementDropData < 0xFFFFFFFFU)
  {
    EthIf_MeasurementDropData++;
  }
  SchM_Exit_EthIf_SCHM_ETHIF_EXCLUSIVE_AREA_0();
}
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[version check]=========================================*/

/*------------------[AUTOSAR module version identification check]-----------*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef ETHIF_VENDOR_ID /* configuration check */
#error ETHIF_VENDOR_ID must be defined
#endif

#if (ETHIF_VENDOR_ID != 1U) /* vendor check */
#error ETHIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef ETHIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error ETHIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error ETHIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef ETHIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error ETHIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHIF_AR_RELEASE_MINOR_VERSION != 3U )
#error ETHIF_AR_RELEASE_MINOR_VERSION wrong (!= 3U)
#endif

#ifndef ETHIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error ETHIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (ETHIF_AR_RELEASE_REVISION_VERSION != 0U )
#error ETHIF_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef ETHIF_SW_MAJOR_VERSION /* configuration check */
#error ETHIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ETHIF_SW_MAJOR_VERSION != 1U)
#error ETHIF_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef ETHIF_SW_MINOR_VERSION /* configuration check */
#error ETHIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ETHIF_SW_MINOR_VERSION < 9U)
#error ETHIF_SW_MINOR_VERSION wrong (< 9U)
#endif

#ifndef ETHIF_SW_PATCH_VERSION /* configuration check */
#error ETHIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (ETHIF_SW_PATCH_VERSION < 18U)
#error ETHIF_SW_PATCH_VERSION wrong (< 18U)
#endif


/*==================[end of file]===========================================*/
