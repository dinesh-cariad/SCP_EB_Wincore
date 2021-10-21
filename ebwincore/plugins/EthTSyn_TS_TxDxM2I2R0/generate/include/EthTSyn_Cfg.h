/* --------{ EB Automotive C Source File }-------- */

#ifndef ETHTSYN_CFG_H
#define ETHTSYN_CFG_H

/*==================[includes]==============================================*/
#include <EthTSyn_CfgTypes.h>

/*==================[macros]================================================*/
[!IF "var:defined('postBuildVariant')"!]
#define ETHTSYN_POSTBUILD_VARIANT_ENABLED
[!ELSE!]
#define ETHTSYN_POSTBUILD_VARIANT_DISABLED
[!ENDIF!]

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[inclusions]==============================================*/
[!LOOP "variant:all()"!][!//
#include <EthTSyn[!"."!]_PBcfg.h>
[!ENDLOOP!][!//

#if (defined ETHTSYN_POSTBUILD_VARIANT_DISABLED)
#include <EthTSyn_PBcfg.h>
#endif
#define ETHTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>
/** \brief Destination MAC address for all EthTSyn frames */
extern CONST(uint8, ETHTSYN_APPL_CONST) EthTSyn_DestMacAddr[6U];

#define ETHTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>
#endif /* ifndef ETHTSYN_CFG_H */
/*==================[end of file]===========================================*/

