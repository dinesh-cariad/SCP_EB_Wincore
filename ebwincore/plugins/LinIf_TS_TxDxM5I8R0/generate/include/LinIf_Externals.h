/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


#ifndef LINIF_EXTERNALS_H
#define LINIF_EXTERNALS_H

[!NOCODE!]
[!VAR "errRespSigChg" = "node:fallback('->node:value(LinIfGeneral/LinIfResponseErrorSignalChangedCallout)', '')"!]
[!VAR "saveConf" = "node:fallback('->node:value(LinIfGeneral/LinIfSaveConfigurationCallout)', '')"!]
[!ENDNOCODE!]
/*==================[inclusions]=============================================*/
[!IF "($errRespSigChg != '') or ($saveConf != '')"!]
#include <ComStack_Types.h>
[!ELSE!]
#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT
[!ENDIF!]

/*==================[macros]================================================*/
#ifndef LINIF_NOTIF_CALLOUT_CODE
#define LINIF_NOTIF_CALLOUT_CODE LINIF_APPL_CODE
#endif

[!IF "$errRespSigChg != ''"!]
/** \brief LinIfResponseErrorSignalChangedCallout */
#if (defined LINIF_ERR_RES_CHG_CO_FUNC)
#error LINIF_ERR_RES_CHG_CO_FUNC already defined!
#endif /* #if (defined LINIF_ERR_RES_CHG_CO_FUNC)*/
#define LINIF_ERR_RES_CHG_CO_FUNC [!"$errRespSigChg"!]
[!ENDIF!]

[!IF "$saveConf != ''"!]
/** \brief LinIfSaveConfigurationCallout */
#if (defined LINIF_NC_SAVECONF_FUNC)
#error LINIF_NC_SAVECONF_FUNC already defined!
#endif /* #if (defined LINIF_NC_SAVECONF_FUNC)*/
#define LINIF_NC_SAVECONF_FUNC [!"$saveConf"!]
[!ENDIF!]
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
[!IF "($errRespSigChg != '') or ($saveConf != '')"!]
#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

[!IF "$errRespSigChg != ''"!]
extern FUNC(void, LINIF_NOTIF_CALLOUT_CODE) [!"$errRespSigChg"!]
(
  NetworkHandleType Channel,
  boolean RespErrSigValue
);
[!ENDIF!]

[!IF "$saveConf != ''"!]
extern FUNC(boolean, LINIF_NOTIF_CALLOUT_CODE) [!"$saveConf"!]
(
  NetworkHandleType Channel
);
[!ENDIF!]

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */
[!ENDIF!]
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]============================================*/

#endif /* #ifndef LINIF_EXTERNALS_H */
