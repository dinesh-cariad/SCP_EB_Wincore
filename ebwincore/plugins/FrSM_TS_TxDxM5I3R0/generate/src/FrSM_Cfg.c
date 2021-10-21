/* --------{ EB Automotive C Source File }-------- */

/*==================[includes]============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <FrSM_Cfg.h>              /* Generated configuration */

/*==================[internal constants]=====================================*/

/*==================[inclusions]============================================*/

[!LOOP "variant:all()"!][!//
#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_NAME
#undef TS_PB_CFG_LAYOUT_TYPE
#include "FrSM[!"."!]_PBcfg.c"

[!ENDLOOP!][!//

#if (defined FRSM_POSTBUILD_VARIANT_DISABLED)
#include "FrSM_PBcfg.c"
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]===========================================*/


