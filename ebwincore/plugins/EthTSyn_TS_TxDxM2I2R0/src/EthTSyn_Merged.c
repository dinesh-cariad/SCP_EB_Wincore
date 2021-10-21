/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (defined TS_MERGED_COMPILE)
#error TS_MERGED_COMPILE is already defined
#endif
/* Set macro that enables merged compilation macros. */
#define TS_MERGED_COMPILE STD_ON

/* First of all include all header files, the C-files depend on.
 * This prevents overlapping memory sections because all included files are protected for
 * further inclusion. */

#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h>
#include <EthTSyn_Types.h>
#include <EthTSyn_Cfg.h> /* Configuration */
#include <EthTSyn_Trace.h>
#include <EthTSyn.h> /* Module public header file. */
#include <EthTSyn_Int.h> /* Module internal header file. */
#include <EthTSyn_Version.h> /* Module Version Info */

#include <EthTSyn_Int01_Lib.h> /* Unit internal header file. */
#include <EthTSyn_Int02_Swt.h> /* Unit internal header file. */

#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */

/* All source files which contains sections other than *_SEC_CODE (or no sections) have to be
 * included before the memory mapping in this file to protect them for overlapping memory
 * sections. */
#include <EthTSyn_Cfg.c>
#include <EthTSyn.c>
#include <EthTSyn_Int01_Lib.c>
#include <EthTSyn_Int02_Swt.c>

/* List of files that include only memory abstraction CODE segments. */
/* Start code section declaration. */
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>



/* Stop code section declaration. */
#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
