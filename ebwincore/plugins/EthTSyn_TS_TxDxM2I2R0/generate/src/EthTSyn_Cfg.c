/* --------{ EB Automotive C Source File }-------- */
[!CODE!]
[!AUTOSPACING!]
/*==================[includes]============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <EthTSyn_Cfg.h>              /* Generated configuration */
[!INCLUDE "../include/EthTSyn_Precompile.m"!][!//
/*==================[internal constants]=====================================*/

/*==================[inclusions]============================================*/

[!LOOP "variant:all()"!][!//
#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_NAME
#undef TS_PB_CFG_LAYOUT_TYPE
#include "EthTSyn[!"."!]_PBcfg.c"

[!ENDLOOP!][!//

#if (defined ETHTSYN_POSTBUILD_VARIANT_DISABLED)
#include "EthTSyn_PBcfg.c"
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/
#define ETHTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/* !LINKSTO EthTSyn.ASR151.PRS_TS_00002, 1 */
[!IF "node:exists(as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynDestPhyAddr)"!]
[!VAR "first"="0"!][!//
/* Configured destination MAC address for all EthTSyn frames. */
CONST(uint8, ETHTSYN_APPL_CONST) EthTSyn_DestMacAddr[6U] =
{[!LOOP "text:split(EthTSynGeneral/EthTSynDestPhyAddr,':')"!][!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]0x[!"."!]U[!ENDLOOP!]};  
[!ELSE!]
/* Default destination MAC address for all EthTSyn frames (See 802.1AS-2011 Ch 11.3.4). */
CONST(uint8, ETHTSYN_APPL_CONST) EthTSyn_DestMacAddr[6U] =
{0x01U, 0x80U, 0xC2U, 0x00U, 0x00U, 0x0EU};
[!ENDIF!]

#define ETHTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>
/*==================[external data]==========================================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]

