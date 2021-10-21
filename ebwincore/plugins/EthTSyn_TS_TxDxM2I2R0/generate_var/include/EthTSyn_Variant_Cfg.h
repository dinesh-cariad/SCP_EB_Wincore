/* --------{ EB Automotive C Source File }-------- */

/*==================[includes]==============================================*/
[!INCLUDE "../include/EthTSyn_Macros.m"!][!//
/*==================[macros]================================================*/
[!IF "var:defined('postBuildVariant')"!]
[!/* Current postbuild configuration name
*/!][!VAR "initPredefinedCfgName"="concat('EthTSyn','_',$postBuildVariant,'_')"!]
[!ELSE!]
[!/* Current postbuild name
*/!][!VAR "initPredefinedCfgName"="string("EthTSyn")"!]
[!ENDIF!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/


/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/


#define ETHTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>


extern CONST(EthTSyn_ConfigType, ETHTSYN_APPL_CONST) [!"concat($initPredefinedCfgName,'Config_0')"!];

#if(ETHTSYN_SWITCH_USED == STD_ON)
extern CONST(EthTSyn_SwitchConfigType, ETHTSYN_APPL_CONST) [!"concat($initPredefinedCfgName,'SwitchConfig_0')"!][[!"$SwtPortsConfigured"!]];
#endif
#define ETHTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


/*==================[end of file]===========================================*/