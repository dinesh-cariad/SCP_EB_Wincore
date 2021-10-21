/**
 * \file
 *
 * \brief AUTOSAR Tm
 *
 * This file contains the implementation of the AUTOSAR
 * module Tm.
 *
 * \version 1.0.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TM_CFG_H
#define TM_CFG_H

/* This file contains the generated Tm module configuration. */

/*==[Includes]================================================================*/

/*==[Macros]==================================================================*/

#if (defined TM_DEV_ERROR_DETECT)
#error TM_DEV_ERROR_DETECT already defined
#endif /* #if (defined TM_DEV_ERROR_DETECT) */
/* !LINKSTO Tm.swdd.Parameter.TmDevErrorDetect,1 */
/** \brief Enables development error detection */
#define TM_DEV_ERROR_DETECT                 [!//
[!IF "TmGeneral/TmDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TM_VERSION_INFO_API)
#error TM_VERSION_INFO_API already defined
#endif
/* !LINKSTO Tm.swdd.Parameter.TmVersionInfoApi,1 */
/** \brief Enables Tm module version API */
#define TM_VERSION_INFO_API                 [!//
[!IF "TmGeneral/TmVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TM_ENABLE_PREDEF_TIMER_1US_16BIT)
#error TM_ENABLE_PREDEF_TIMER_1US_16BIT already defined
#endif
/* !LINKSTO Tm.swdd.Parameter.TmEnablePredefTimer1us16bit,1, SWS_Tm_00026_0,1, SWS_Tm_00027_0,1 */
/** \brief Enables APIs for timer on 16 bits with 1us resolution */
#define TM_ENABLE_PREDEF_TIMER_1US_16BIT    [!//
[!IF "TmGeneral/TmEnablePredefTimer1us16bit = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TM_ENABLE_PREDEF_TIMER_1US_24BIT)
#error TM_ENABLE_PREDEF_TIMER_1US_24BIT already defined
#endif
/* !LINKSTO Tm.swdd.Parameter.TmEnablePredefTimer1us24bit,1, SWS_Tm_00026_1,1, SWS_Tm_00027_1,1 */
/** \brief Enables APIs for timer on 24 bits with 1us resolution */
#define TM_ENABLE_PREDEF_TIMER_1US_24BIT     [!//
[!IF "TmGeneral/TmEnablePredefTimer1us24bit = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TM_ENABLE_PREDEF_TIMER_1US_32BIT)
#error TM_ENABLE_PREDEF_TIMER_1US_32BIT already defined
#endif
/* !LINKSTO Tm.swdd.Parameter.TmEnablePredefTimer1us32bit,1, SWS_Tm_00026_2,1, SWS_Tm_00027_2,1 */
/** \brief Enables APIs for timer on 32 bits with 1us resolution */
#define TM_ENABLE_PREDEF_TIMER_1US_32BIT    [!//
[!IF "TmGeneral/TmEnablePredefTimer1us32bit = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined TM_ENABLE_PREDEF_TIMER_100US_32BIT)
#error TM_ENABLE_PREDEF_TIMER_100US_32BIT already defined
#endif
/* !LINKSTO Tm.swdd.Parameter.TmEnablePredefTimer100us32bit,1, SWS_Tm_00026_3,1 */
/** \brief Enables APIs for timer on 32 bits with 100us resolution */
#define TM_ENABLE_PREDEF_TIMER_100US_32BIT  [!//
[!IF "TmGeneral/TmEnablePredefTimer100us32bit = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* ifndef TM_CFG_H */

