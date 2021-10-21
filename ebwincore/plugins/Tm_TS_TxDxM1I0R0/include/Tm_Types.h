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

#ifndef TM_TYPES_H
#define TM_TYPES_H

/*==[Includes]================================================================*/

#include <Tm_Cfg.h>

#include <Tm_Version.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

#if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON)

#ifndef RTE_TYPE_Tm_PredefTimer1us16bitType
#define RTE_TYPE_Tm_PredefTimer1us16bitType
/* !LINKSTO Tm.swdd.PredefTimer1us16bitType,1 */
/**  \brief Data type of Time Service Predef Timer 1us16bit. */
typedef struct {
  /** \brief Reference time. */
  uint16 TimeValue;
} Tm_PredefTimer1us16bitType;
#endif /* RTE_TYPE_Tm_PredefTimer1us16bitType */

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON)

#ifndef RTE_TYPE_Tm_PredefTimer1us24bitType
#define RTE_TYPE_Tm_PredefTimer1us24bitType
/* !LINKSTO Tm.swdd.PredefTimer1us24bitType,1 */
/**  \brief Data type of Time Service Predef Timer 1us24bit. */
typedef struct {
  /** \brief Reference time. */
  uint32 TimeValue;
} Tm_PredefTimer1us24bitType;
#endif /* RTE_TYPE_Tm_PredefTimer1us24bitType */

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON)

#ifndef RTE_TYPE_Tm_PredefTimer1us32bitType
#define RTE_TYPE_Tm_PredefTimer1us32bitType
/* !LINKSTO Tm.swdd.PredefTimer1us32bitType,1 */
/**  \brief Data type of Time Service Predef Timer 1us32bit. */
typedef struct {
  /** \brief Reference time. */
  uint32 TimeValue;
} Tm_PredefTimer1us32bitType;
#endif /* RTE_TYPE_Tm_PredefTimer1us32bitType */

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON) */

#if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON)

#ifndef RTE_TYPE_Tm_PredefTimer100us32bitType
#define RTE_TYPE_Tm_PredefTimer100us32bitType
/* !LINKSTO Tm.swdd.PredefTimer100us32bitType,1 */
/**  \brief Data type of Time Service Predef Timer 100μs32bit. */
typedef struct {
  /** \brief Reference time. */
  uint32 TimeValue;
} Tm_PredefTimer100us32bitType;
#endif /* RTE_TYPE_Tm_PredefTimer100us32bitType */

#endif /* #if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON) */

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* ifndef TM_TYPES_H */

