/* --------{ EB Automotive C Source File }-------- */

#if (!defined FRSM_CFGTYPES_H)
#define FRSM_CFGTYPES_H

/*==================[includes]==============================================*/
#include <FrSM_Types.h>

/*==================[macros]================================================*/
[!IF "var:defined('postBuildVariant')"!]
#define FRSM_POSTBUILD_VARIANT_ENABLED
[!ELSE!]
#define FRSM_POSTBUILD_VARIANT_DISABLED
[!ENDIF!]

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[type definitions]=========================*/

/** \brief Internal post-build configuration structure type */
typedef struct
{
    /** \brief Type passed to \a FrSM_Init() */
    const FrSM_ConfigType RootCfg;
[!IF "num:integer(count(FrSMConfig/*[1]/FrSMCluster/*)) > 1"!][!//
    /** \brief FlexRay cluster configurations, except the first one contained in \a RootCfg */
    const FrSM_ClstCfgType aClst[[!"num:integer(count(FrSMConfig/*[1]/FrSMCluster/*) - 1)"!]U];
[!ENDIF!][!//
}FrSM_CfgType;

/*==================[inclusions]==============================================*/
[!LOOP "variant:all()"!][!//
#include <FrSM[!"."!]_PBcfg.h>
[!ENDLOOP!][!//

#if (defined FRSM_POSTBUILD_VARIANT_DISABLED)
#include <FrSM_PBcfg.h>
#endif

#endif /* if !defined( FRSM_CFGTYPES_H ) */
/*==================[end of file]===========================================*/