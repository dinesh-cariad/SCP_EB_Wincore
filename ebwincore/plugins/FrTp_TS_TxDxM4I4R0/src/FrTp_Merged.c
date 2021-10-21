/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

/* include all foreign includes before inclusion of FrTp_guards.h
 * Special cases can be excluded in merged_included_ignore and must be handled here.
 * e.g. guarded includes like Det.h & PbcfgM.h
 * All APIs (content of /include/ directory must be added to merged_includes_foreign,
 * in order to include them here (Module prefix is inconclusive). */
#include <FrTp_merged_includes.h>

#include <FrTp_Cfg.h> /* defines FRTP_DEV_ERROR_DETECT */
#if (FRTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* get development error tracer interface */
#endif /* (FRTP_DEV_ERROR_DETECT == STD_ON) */

/* Now guard the collected internal macros against unsolicited spam defines from outside */
#include <FrTp_guards.h>

#include <FrTp_TxStateMachine.c>
#include <FrTp_RxStateMachine.c>
#include <FrTp_GetVersionInfo.c>
#include <FrTp_LL.c>
#include <FrTp.c>
