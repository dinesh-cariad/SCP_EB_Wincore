/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#if (!defined COMPILER_CFG_H)
#define COMPILER_CFG_H

/* This file is an user-specific file and will be delivered only as a template
 * which works together with all ACG-modules by EB.
 * The integrator needs to merge all Compiler_Cfg.h file of all module vendors.
 * Proposed process is specified in AUTOSAR_SWS_CompilerAbstraction
 * specification.
 */

/*==================[inclusions]=============================================*/

/*------------------[Compiler_Cfg extensions]--------------------------------*/
/** \def COMPILERCFG_EXTENSION_FILE
 ** \brief Includes an additional Compiler_CfgExt.h file if macro is available.
 **
 ** If the macro COMPILERCFG_EXTENSION_FILE is defined, the EB Compiler_Cfg.h
 ** file will include an additional file Compiler_CfgExt.h. This file can be
 ** used to add memory and pointer classes of 3rd-party modules.
 */
#if (defined COMPILERCFG_EXTENSION_FILE)
#include <Compiler_CfgExt.h>
#endif

/*==================[macros]=================================================*/

/* only for TS_ARCH_FAMILY == TS_WINDOWS */

/* The following macros are only necessary for WINDOWS architecture.
 * For other 32-bit architectures the EB Compiler.h file is optimized and do
 * not use these macros. When you use the EB Compiler.h file the following
 * macros can be deleted to increase the compilation time.
 */

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ADC_CODE)
#error ADC_CODE already defined
#endif
#define ADC_CODE

#if (defined ADC_CONST)
#error ADC_CONST already defined
#endif
#define ADC_CONST

#if (defined ADC_APPL_DATA)
#error ADC_APPL_DATA already defined
#endif
#define ADC_APPL_DATA

#if (defined ADC_APPL_CONST)
#error ADC_APPL_CONST already defined
#endif
#define ADC_APPL_CONST

#if (defined ADC_APPL_CODE)
#error ADC_APPL_CODE already defined
#endif
#define ADC_APPL_CODE

#if (defined ADC_VAR_NOINIT)
#error ADC_VAR_NOINIT already defined
#endif
#define ADC_VAR_NOINIT

#if (defined ADC_VAR_CLEARED)
#error ADC_VAR_CLEARED already defined
#endif
#define ADC_VAR_CLEARED

#if (defined ADC_VAR_POWER_ON_INIT)
#error ADC_VAR_POWER_ON_INIT already defined
#endif
#define ADC_VAR_POWER_ON_INIT

#if (defined ADC_VAR_FAST)
#error ADC_VAR_FAST already defined
#endif
#define ADC_VAR_FAST

#if (defined ADC_VAR)
#error ADC_VAR already defined
#endif
#define ADC_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined AUTOSAR_COMSTACKDATA)
#error AUTOSAR_COMSTACKDATA already defined
#endif
#define AUTOSAR_COMSTACKDATA


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined BASE_CODE)
#error BASE_CODE already defined
#endif
#define BASE_CODE

#if (defined BASE_CONST)
#error BASE_CONST already defined
#endif
#define BASE_CONST

#if (defined BASE_APPL_DATA)
#error BASE_APPL_DATA already defined
#endif
#define BASE_APPL_DATA

#if (defined BASE_APPL_CONST)
#error BASE_APPL_CONST already defined
#endif
#define BASE_APPL_CONST

#if (defined BASE_APPL_CODE)
#error BASE_APPL_CODE already defined
#endif
#define BASE_APPL_CODE

#if (defined BASE_VAR_NOINIT)
#error BASE_VAR_NOINIT already defined
#endif
#define BASE_VAR_NOINIT

#if (defined BASE_VAR_CLEARED)
#error BASE_VAR_CLEARED already defined
#endif
#define BASE_VAR_CLEARED

#if (defined BASE_VAR_POWER_ON_INIT)
#error BASE_VAR_POWER_ON_INIT already defined
#endif
#define BASE_VAR_POWER_ON_INIT

#if (defined BASE_VAR_FAST)
#error BASE_VAR_FAST already defined
#endif
#define BASE_VAR_FAST

#if (defined BASE_VAR)
#error BASE_VAR already defined
#endif
#define BASE_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined BSWM_CODE)
#error BSWM_CODE already defined
#endif
#define BSWM_CODE

#if (defined BSWM_CONST)
#error BSWM_CONST already defined
#endif
#define BSWM_CONST

#if (defined BSWM_APPL_DATA)
#error BSWM_APPL_DATA already defined
#endif
#define BSWM_APPL_DATA

#if (defined BSWM_APPL_CONST)
#error BSWM_APPL_CONST already defined
#endif
#define BSWM_APPL_CONST

#if (defined BSWM_APPL_CODE)
#error BSWM_APPL_CODE already defined
#endif
#define BSWM_APPL_CODE

#if (defined BSWM_VAR_NOINIT)
#error BSWM_VAR_NOINIT already defined
#endif
#define BSWM_VAR_NOINIT

#if (defined BSWM_VAR_CLEARED)
#error BSWM_VAR_CLEARED already defined
#endif
#define BSWM_VAR_CLEARED

#if (defined BSWM_VAR_POWER_ON_INIT)
#error BSWM_VAR_POWER_ON_INIT already defined
#endif
#define BSWM_VAR_POWER_ON_INIT

#if (defined BSWM_VAR_FAST)
#error BSWM_VAR_FAST already defined
#endif
#define BSWM_VAR_FAST

#if (defined BSWM_VAR)
#error BSWM_VAR already defined
#endif
#define BSWM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CAL_CODE)
#error CAL_CODE already defined
#endif
#define CAL_CODE

#if (defined CAL_CONST)
#error CAL_CONST already defined
#endif
#define CAL_CONST

#if (defined CAL_APPL_DATA)
#error CAL_APPL_DATA already defined
#endif
#define CAL_APPL_DATA

#if (defined CAL_APPL_CONST)
#error CAL_APPL_CONST already defined
#endif
#define CAL_APPL_CONST

#if (defined CAL_APPL_CODE)
#error CAL_APPL_CODE already defined
#endif
#define CAL_APPL_CODE

#if (defined CAL_VAR_NOINIT)
#error CAL_VAR_NOINIT already defined
#endif
#define CAL_VAR_NOINIT

#if (defined CAL_VAR_CLEARED)
#error CAL_VAR_CLEARED already defined
#endif
#define CAL_VAR_CLEARED

#if (defined CAL_VAR_POWER_ON_INIT)
#error CAL_VAR_POWER_ON_INIT already defined
#endif
#define CAL_VAR_POWER_ON_INIT

#if (defined CAL_VAR_FAST)
#error CAL_VAR_FAST already defined
#endif
#define CAL_VAR_FAST

#if (defined CAL_VAR)
#error CAL_VAR already defined
#endif
#define CAL_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CAN_CODE)
#error CAN_CODE already defined
#endif
#define CAN_CODE

#if (defined CAN_CONST)
#error CAN_CONST already defined
#endif
#define CAN_CONST

#if (defined CAN_APPL_DATA)
#error CAN_APPL_DATA already defined
#endif
#define CAN_APPL_DATA

#if (defined CAN_APPL_CONST)
#error CAN_APPL_CONST already defined
#endif
#define CAN_APPL_CONST

#if (defined CAN_APPL_CODE)
#error CAN_APPL_CODE already defined
#endif
#define CAN_APPL_CODE

#if (defined CAN_VAR_NOINIT)
#error CAN_VAR_NOINIT already defined
#endif
#define CAN_VAR_NOINIT

#if (defined CAN_VAR_CLEARED)
#error CAN_VAR_CLEARED already defined
#endif
#define CAN_VAR_CLEARED

#if (defined CAN_VAR_POWER_ON_INIT)
#error CAN_VAR_POWER_ON_INIT already defined
#endif
#define CAN_VAR_POWER_ON_INIT

#if (defined CAN_VAR_FAST)
#error CAN_VAR_FAST already defined
#endif
#define CAN_VAR_FAST

#if (defined CAN_VAR)
#error CAN_VAR already defined
#endif
#define CAN_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANIF_CODE)
#error CANIF_CODE already defined
#endif
#define CANIF_CODE

#if (defined CANIF_CONST)
#error CANIF_CONST already defined
#endif
#define CANIF_CONST

#if (defined CANIF_APPL_DATA)
#error CANIF_APPL_DATA already defined
#endif
#define CANIF_APPL_DATA

#if (defined CANIF_APPL_CONST)
#error CANIF_APPL_CONST already defined
#endif
#define CANIF_APPL_CONST

#if (defined CANIF_APPL_CODE)
#error CANIF_APPL_CODE already defined
#endif
#define CANIF_APPL_CODE

#if (defined CANIF_VAR_NOINIT)
#error CANIF_VAR_NOINIT already defined
#endif
#define CANIF_VAR_NOINIT

#if (defined CANIF_VAR_CLEARED)
#error CANIF_VAR_CLEARED already defined
#endif
#define CANIF_VAR_CLEARED

#if (defined CANIF_VAR_POWER_ON_INIT)
#error CANIF_VAR_POWER_ON_INIT already defined
#endif
#define CANIF_VAR_POWER_ON_INIT

#if (defined CANIF_VAR_FAST)
#error CANIF_VAR_FAST already defined
#endif
#define CANIF_VAR_FAST

#if (defined CANIF_VAR)
#error CANIF_VAR already defined
#endif
#define CANIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANNM_CODE)
#error CANNM_CODE already defined
#endif
#define CANNM_CODE

#if (defined CANNM_CONST)
#error CANNM_CONST already defined
#endif
#define CANNM_CONST

#if (defined CANNM_APPL_DATA)
#error CANNM_APPL_DATA already defined
#endif
#define CANNM_APPL_DATA

#if (defined CANNM_APPL_CONST)
#error CANNM_APPL_CONST already defined
#endif
#define CANNM_APPL_CONST

#if (defined CANNM_APPL_CODE)
#error CANNM_APPL_CODE already defined
#endif
#define CANNM_APPL_CODE

#if (defined CANNM_VAR_NOINIT)
#error CANNM_VAR_NOINIT already defined
#endif
#define CANNM_VAR_NOINIT

#if (defined CANNM_VAR_CLEARED)
#error CANNM_VAR_CLEARED already defined
#endif
#define CANNM_VAR_CLEARED

#if (defined CANNM_VAR_POWER_ON_INIT)
#error CANNM_VAR_POWER_ON_INIT already defined
#endif
#define CANNM_VAR_POWER_ON_INIT

#if (defined CANNM_VAR_FAST)
#error CANNM_VAR_FAST already defined
#endif
#define CANNM_VAR_FAST

#if (defined CANNM_VAR)
#error CANNM_VAR already defined
#endif
#define CANNM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANSM_CODE)
#error CANSM_CODE already defined
#endif
#define CANSM_CODE

#if (defined CANSM_CONST)
#error CANSM_CONST already defined
#endif
#define CANSM_CONST

#if (defined CANSM_APPL_DATA)
#error CANSM_APPL_DATA already defined
#endif
#define CANSM_APPL_DATA

#if (defined CANSM_APPL_CONST)
#error CANSM_APPL_CONST already defined
#endif
#define CANSM_APPL_CONST

#if (defined CANSM_APPL_CODE)
#error CANSM_APPL_CODE already defined
#endif
#define CANSM_APPL_CODE

#if (defined CANSM_VAR_NOINIT)
#error CANSM_VAR_NOINIT already defined
#endif
#define CANSM_VAR_NOINIT

#if (defined CANSM_VAR_CLEARED)
#error CANSM_VAR_CLEARED already defined
#endif
#define CANSM_VAR_CLEARED

#if (defined CANSM_VAR_POWER_ON_INIT)
#error CANSM_VAR_POWER_ON_INIT already defined
#endif
#define CANSM_VAR_POWER_ON_INIT

#if (defined CANSM_VAR_FAST)
#error CANSM_VAR_FAST already defined
#endif
#define CANSM_VAR_FAST

#if (defined CANSM_VAR)
#error CANSM_VAR already defined
#endif
#define CANSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTP_CODE)
#error CANTP_CODE already defined
#endif
#define CANTP_CODE

#if (defined CANTP_CONST)
#error CANTP_CONST already defined
#endif
#define CANTP_CONST

#if (defined CANTP_APPL_DATA)
#error CANTP_APPL_DATA already defined
#endif
#define CANTP_APPL_DATA

#if (defined CANTP_APPL_CONST)
#error CANTP_APPL_CONST already defined
#endif
#define CANTP_APPL_CONST

#if (defined CANTP_APPL_CODE)
#error CANTP_APPL_CODE already defined
#endif
#define CANTP_APPL_CODE

#if (defined CANTP_VAR_NOINIT)
#error CANTP_VAR_NOINIT already defined
#endif
#define CANTP_VAR_NOINIT

#if (defined CANTP_VAR_CLEARED)
#error CANTP_VAR_CLEARED already defined
#endif
#define CANTP_VAR_CLEARED

#if (defined CANTP_VAR_POWER_ON_INIT)
#error CANTP_VAR_POWER_ON_INIT already defined
#endif
#define CANTP_VAR_POWER_ON_INIT

#if (defined CANTP_VAR_FAST)
#error CANTP_VAR_FAST already defined
#endif
#define CANTP_VAR_FAST

#if (defined CANTP_VAR)
#error CANTP_VAR already defined
#endif
#define CANTP_VAR

#if (defined CANTP_ENTRY_JUMP_TABLE)
#error CANTP_ENTRY_JUMP_TABLE already defined
#endif
#define CANTP_ENTRY_JUMP_TABLE

#if (defined CANTP_EXIT_JUMP_TABLE)
#error CANTP_EXIT_JUMP_TABLE already defined
#endif
#define CANTP_EXIT_JUMP_TABLE


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTRCV_CODE)
#error CANTRCV_CODE already defined
#endif
#define CANTRCV_CODE

#if (defined CANTRCV_CONST)
#error CANTRCV_CONST already defined
#endif
#define CANTRCV_CONST

#if (defined CANTRCV_APPL_DATA)
#error CANTRCV_APPL_DATA already defined
#endif
#define CANTRCV_APPL_DATA

#if (defined CANTRCV_APPL_CONST)
#error CANTRCV_APPL_CONST already defined
#endif
#define CANTRCV_APPL_CONST

#if (defined CANTRCV_APPL_CODE)
#error CANTRCV_APPL_CODE already defined
#endif
#define CANTRCV_APPL_CODE

#if (defined CANTRCV_VAR_NOINIT)
#error CANTRCV_VAR_NOINIT already defined
#endif
#define CANTRCV_VAR_NOINIT

#if (defined CANTRCV_VAR_CLEARED)
#error CANTRCV_VAR_CLEARED already defined
#endif
#define CANTRCV_VAR_CLEARED

#if (defined CANTRCV_VAR_POWER_ON_INIT)
#error CANTRCV_VAR_POWER_ON_INIT already defined
#endif
#define CANTRCV_VAR_POWER_ON_INIT

#if (defined CANTRCV_VAR_FAST)
#error CANTRCV_VAR_FAST already defined
#endif
#define CANTRCV_VAR_FAST

#if (defined CANTRCV_VAR)
#error CANTRCV_VAR already defined
#endif
#define CANTRCV_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTRCV_1_T01_CODE)
#error CANTRCV_1_T01_CODE already defined
#endif
#define CANTRCV_1_T01_CODE

#if (defined CANTRCV_1_T01_CONST)
#error CANTRCV_1_T01_CONST already defined
#endif
#define CANTRCV_1_T01_CONST

#if (defined CANTRCV_1_T01_APPL_DATA)
#error CANTRCV_1_T01_APPL_DATA already defined
#endif
#define CANTRCV_1_T01_APPL_DATA

#if (defined CANTRCV_1_T01_APPL_CONST)
#error CANTRCV_1_T01_APPL_CONST already defined
#endif
#define CANTRCV_1_T01_APPL_CONST

#if (defined CANTRCV_1_T01_APPL_CODE)
#error CANTRCV_1_T01_APPL_CODE already defined
#endif
#define CANTRCV_1_T01_APPL_CODE

#if (defined CANTRCV_1_T01_VAR_NOINIT)
#error CANTRCV_1_T01_VAR_NOINIT already defined
#endif
#define CANTRCV_1_T01_VAR_NOINIT

#if (defined CANTRCV_1_T01_VAR_CLEARED)
#error CANTRCV_1_T01_VAR_CLEARED already defined
#endif
#define CANTRCV_1_T01_VAR_CLEARED

#if (defined CANTRCV_1_T01_VAR_POWER_ON_INIT)
#error CANTRCV_1_T01_VAR_POWER_ON_INIT already defined
#endif
#define CANTRCV_1_T01_VAR_POWER_ON_INIT

#if (defined CANTRCV_1_T01_VAR_FAST)
#error CANTRCV_1_T01_VAR_FAST already defined
#endif
#define CANTRCV_1_T01_VAR_FAST

#if (defined CANTRCV_1_T01_VAR)
#error CANTRCV_1_T01_VAR already defined
#endif
#define CANTRCV_1_T01_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTRCV_1_T02_CODE)
#error CANTRCV_1_T02_CODE already defined
#endif
#define CANTRCV_1_T02_CODE

#if (defined CANTRCV_1_T02_CONST)
#error CANTRCV_1_T02_CONST already defined
#endif
#define CANTRCV_1_T02_CONST

#if (defined CANTRCV_1_T02_APPL_DATA)
#error CANTRCV_1_T02_APPL_DATA already defined
#endif
#define CANTRCV_1_T02_APPL_DATA

#if (defined CANTRCV_1_T02_APPL_CONST)
#error CANTRCV_1_T02_APPL_CONST already defined
#endif
#define CANTRCV_1_T02_APPL_CONST

#if (defined CANTRCV_1_T02_APPL_CODE)
#error CANTRCV_1_T02_APPL_CODE already defined
#endif
#define CANTRCV_1_T02_APPL_CODE

#if (defined CANTRCV_1_T02_VAR_NOINIT)
#error CANTRCV_1_T02_VAR_NOINIT already defined
#endif
#define CANTRCV_1_T02_VAR_NOINIT

#if (defined CANTRCV_1_T02_VAR_CLEARED)
#error CANTRCV_1_T02_VAR_CLEARED already defined
#endif
#define CANTRCV_1_T02_VAR_CLEARED

#if (defined CANTRCV_1_T02_VAR_POWER_ON_INIT)
#error CANTRCV_1_T02_VAR_POWER_ON_INIT already defined
#endif
#define CANTRCV_1_T02_VAR_POWER_ON_INIT

#if (defined CANTRCV_1_T02_VAR_FAST)
#error CANTRCV_1_T02_VAR_FAST already defined
#endif
#define CANTRCV_1_T02_VAR_FAST

#if (defined CANTRCV_1_T02_VAR)
#error CANTRCV_1_T02_VAR already defined
#endif
#define CANTRCV_1_T02_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTRCV_1_T03_CODE)
#error CANTRCV_1_T03_CODE already defined
#endif
#define CANTRCV_1_T03_CODE

#if (defined CANTRCV_1_T03_CONST)
#error CANTRCV_1_T03_CONST already defined
#endif
#define CANTRCV_1_T03_CONST

#if (defined CANTRCV_1_T03_APPL_DATA)
#error CANTRCV_1_T03_APPL_DATA already defined
#endif
#define CANTRCV_1_T03_APPL_DATA

#if (defined CANTRCV_1_T03_APPL_CONST)
#error CANTRCV_1_T03_APPL_CONST already defined
#endif
#define CANTRCV_1_T03_APPL_CONST

#if (defined CANTRCV_1_T03_APPL_CODE)
#error CANTRCV_1_T03_APPL_CODE already defined
#endif
#define CANTRCV_1_T03_APPL_CODE

#if (defined CANTRCV_1_T03_VAR_NOINIT)
#error CANTRCV_1_T03_VAR_NOINIT already defined
#endif
#define CANTRCV_1_T03_VAR_NOINIT

#if (defined CANTRCV_1_T03_VAR_CLEARED)
#error CANTRCV_1_T03_VAR_CLEARED already defined
#endif
#define CANTRCV_1_T03_VAR_CLEARED

#if (defined CANTRCV_1_T03_VAR_POWER_ON_INIT)
#error CANTRCV_1_T03_VAR_POWER_ON_INIT already defined
#endif
#define CANTRCV_1_T03_VAR_POWER_ON_INIT

#if (defined CANTRCV_1_T03_VAR_FAST)
#error CANTRCV_1_T03_VAR_FAST already defined
#endif
#define CANTRCV_1_T03_VAR_FAST

#if (defined CANTRCV_1_T03_VAR)
#error CANTRCV_1_T03_VAR already defined
#endif
#define CANTRCV_1_T03_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTRCV_1_T04_CODE)
#error CANTRCV_1_T04_CODE already defined
#endif
#define CANTRCV_1_T04_CODE

#if (defined CANTRCV_1_T04_CONST)
#error CANTRCV_1_T04_CONST already defined
#endif
#define CANTRCV_1_T04_CONST

#if (defined CANTRCV_1_T04_APPL_DATA)
#error CANTRCV_1_T04_APPL_DATA already defined
#endif
#define CANTRCV_1_T04_APPL_DATA

#if (defined CANTRCV_1_T04_APPL_CONST)
#error CANTRCV_1_T04_APPL_CONST already defined
#endif
#define CANTRCV_1_T04_APPL_CONST

#if (defined CANTRCV_1_T04_APPL_CODE)
#error CANTRCV_1_T04_APPL_CODE already defined
#endif
#define CANTRCV_1_T04_APPL_CODE

#if (defined CANTRCV_1_T04_VAR_NOINIT)
#error CANTRCV_1_T04_VAR_NOINIT already defined
#endif
#define CANTRCV_1_T04_VAR_NOINIT

#if (defined CANTRCV_1_T04_VAR_CLEARED)
#error CANTRCV_1_T04_VAR_CLEARED already defined
#endif
#define CANTRCV_1_T04_VAR_CLEARED

#if (defined CANTRCV_1_T04_VAR_POWER_ON_INIT)
#error CANTRCV_1_T04_VAR_POWER_ON_INIT already defined
#endif
#define CANTRCV_1_T04_VAR_POWER_ON_INIT

#if (defined CANTRCV_1_T04_VAR_FAST)
#error CANTRCV_1_T04_VAR_FAST already defined
#endif
#define CANTRCV_1_T04_VAR_FAST

#if (defined CANTRCV_1_T04_VAR)
#error CANTRCV_1_T04_VAR already defined
#endif
#define CANTRCV_1_T04_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CANTSYN_CODE)
#error CANTSYN_CODE already defined
#endif
#define CANTSYN_CODE

#if (defined CANTSYN_CONST)
#error CANTSYN_CONST already defined
#endif
#define CANTSYN_CONST

#if (defined CANTSYN_APPL_DATA)
#error CANTSYN_APPL_DATA already defined
#endif
#define CANTSYN_APPL_DATA

#if (defined CANTSYN_APPL_CONST)
#error CANTSYN_APPL_CONST already defined
#endif
#define CANTSYN_APPL_CONST

#if (defined CANTSYN_APPL_CODE)
#error CANTSYN_APPL_CODE already defined
#endif
#define CANTSYN_APPL_CODE

#if (defined CANTSYN_VAR_NOINIT)
#error CANTSYN_VAR_NOINIT already defined
#endif
#define CANTSYN_VAR_NOINIT

#if (defined CANTSYN_VAR_CLEARED)
#error CANTSYN_VAR_CLEARED already defined
#endif
#define CANTSYN_VAR_CLEARED

#if (defined CANTSYN_VAR_POWER_ON_INIT)
#error CANTSYN_VAR_POWER_ON_INIT already defined
#endif
#define CANTSYN_VAR_POWER_ON_INIT

#if (defined CANTSYN_VAR_FAST)
#error CANTSYN_VAR_FAST already defined
#endif
#define CANTSYN_VAR_FAST

#if (defined CANTSYN_VAR)
#error CANTSYN_VAR already defined
#endif
#define CANTSYN_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CDD_CODE)
#error CDD_CODE already defined
#endif
#define CDD_CODE

#if (defined CDD_CONST)
#error CDD_CONST already defined
#endif
#define CDD_CONST

#if (defined CDD_APPL_DATA)
#error CDD_APPL_DATA already defined
#endif
#define CDD_APPL_DATA

#if (defined CDD_APPL_CONST)
#error CDD_APPL_CONST already defined
#endif
#define CDD_APPL_CONST

#if (defined CDD_APPL_CODE)
#error CDD_APPL_CODE already defined
#endif
#define CDD_APPL_CODE

#if (defined CDD_VAR_NOINIT)
#error CDD_VAR_NOINIT already defined
#endif
#define CDD_VAR_NOINIT

#if (defined CDD_VAR_CLEARED)
#error CDD_VAR_CLEARED already defined
#endif
#define CDD_VAR_CLEARED

#if (defined CDD_VAR_POWER_ON_INIT)
#error CDD_VAR_POWER_ON_INIT already defined
#endif
#define CDD_VAR_POWER_ON_INIT

#if (defined CDD_VAR_FAST)
#error CDD_VAR_FAST already defined
#endif
#define CDD_VAR_FAST

#if (defined CDD_VAR)
#error CDD_VAR already defined
#endif
#define CDD_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CNM_CODE)
#error CNM_CODE already defined
#endif
#define CNM_CODE

#if (defined CNM_CONST)
#error CNM_CONST already defined
#endif
#define CNM_CONST

#if (defined CNM_APPL_DATA)
#error CNM_APPL_DATA already defined
#endif
#define CNM_APPL_DATA

#if (defined CNM_APPL_CONST)
#error CNM_APPL_CONST already defined
#endif
#define CNM_APPL_CONST

#if (defined CNM_APPL_CODE)
#error CNM_APPL_CODE already defined
#endif
#define CNM_APPL_CODE

#if (defined CNM_VAR_NOINIT)
#error CNM_VAR_NOINIT already defined
#endif
#define CNM_VAR_NOINIT

#if (defined CNM_VAR_CLEARED)
#error CNM_VAR_CLEARED already defined
#endif
#define CNM_VAR_CLEARED

#if (defined CNM_VAR_POWER_ON_INIT)
#error CNM_VAR_POWER_ON_INIT already defined
#endif
#define CNM_VAR_POWER_ON_INIT

#if (defined CNM_VAR_FAST)
#error CNM_VAR_FAST already defined
#endif
#define CNM_VAR_FAST

#if (defined CNM_VAR)
#error CNM_VAR already defined
#endif
#define CNM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined COM_CODE)
#error COM_CODE already defined
#endif
#define COM_CODE

#if (defined COM_CONST)
#error COM_CONST already defined
#endif
#define COM_CONST

#if (defined COM_APPL_DATA)
#error COM_APPL_DATA already defined
#endif
#define COM_APPL_DATA

#if (defined COM_APPL_CONST)
#error COM_APPL_CONST already defined
#endif
#define COM_APPL_CONST

#if (defined COM_APPL_CODE)
#error COM_APPL_CODE already defined
#endif
#define COM_APPL_CODE

#if (defined COM_VAR_NOINIT)
#error COM_VAR_NOINIT already defined
#endif
#define COM_VAR_NOINIT

#if (defined COM_VAR_CLEARED)
#error COM_VAR_CLEARED already defined
#endif
#define COM_VAR_CLEARED

#if (defined COM_VAR_POWER_ON_INIT)
#error COM_VAR_POWER_ON_INIT already defined
#endif
#define COM_VAR_POWER_ON_INIT

#if (defined COM_VAR_FAST)
#error COM_VAR_FAST already defined
#endif
#define COM_VAR_FAST

#if (defined COM_VAR)
#error COM_VAR already defined
#endif
#define COM_VAR

#if (defined COM_RTE_CODE)
#error COM_RTE_CODE already defined
#endif
#define COM_RTE_CODE


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined COMM_CODE)
#error COMM_CODE already defined
#endif
#define COMM_CODE

#if (defined COMM_CONST)
#error COMM_CONST already defined
#endif
#define COMM_CONST

#if (defined COMM_APPL_DATA)
#error COMM_APPL_DATA already defined
#endif
#define COMM_APPL_DATA

#if (defined COMM_APPL_CONST)
#error COMM_APPL_CONST already defined
#endif
#define COMM_APPL_CONST

#if (defined COMM_APPL_CODE)
#error COMM_APPL_CODE already defined
#endif
#define COMM_APPL_CODE

#if (defined COMM_VAR_NOINIT)
#error COMM_VAR_NOINIT already defined
#endif
#define COMM_VAR_NOINIT

#if (defined COMM_VAR_CLEARED)
#error COMM_VAR_CLEARED already defined
#endif
#define COMM_VAR_CLEARED

#if (defined COMM_VAR_POWER_ON_INIT)
#error COMM_VAR_POWER_ON_INIT already defined
#endif
#define COMM_VAR_POWER_ON_INIT

#if (defined COMM_VAR_POWER_ON_CLEARED)
#error COMM_VAR_POWER_ON_CLEARED already defined
#endif
#define COMM_VAR_POWER_ON_CLEARED

#if (defined COMM_VAR_FAST)
#error COMM_VAR_FAST already defined
#endif
#define COMM_VAR_FAST

#if (defined COMM_VAR)
#error COMM_VAR already defined
#endif
#define COMM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined COMXF_CODE)
#error COMXF_CODE already defined
#endif
#define COMXF_CODE

#if (defined COMXF_CONST)
#error COMXF_CONST already defined
#endif
#define COMXF_CONST

#if (defined COMXF_APPL_DATA)
#error COMXF_APPL_DATA already defined
#endif
#define COMXF_APPL_DATA

#if (defined COMXF_APPL_CONST)
#error COMXF_APPL_CONST already defined
#endif
#define COMXF_APPL_CONST

#if (defined COMXF_APPL_CODE)
#error COMXF_APPL_CODE already defined
#endif
#define COMXF_APPL_CODE

#if (defined COMXF_VAR_NOINIT)
#error COMXF_VAR_NOINIT already defined
#endif
#define COMXF_VAR_NOINIT

#if (defined COMXF_VAR_CLEARED)
#error COMXF_VAR_CLEARED already defined
#endif
#define COMXF_VAR_CLEARED

#if (defined COMXF_VAR_POWER_ON_INIT)
#error COMXF_VAR_POWER_ON_INIT already defined
#endif
#define COMXF_VAR_POWER_ON_INIT

#if (defined COMXF_VAR_FAST)
#error COMXF_VAR_FAST already defined
#endif
#define COMXF_VAR_FAST

#if (defined COMXF_VAR)
#error COMXF_VAR already defined
#endif
#define COMXF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CPL_CODE)
#error CPL_CODE already defined
#endif
#define CPL_CODE

#if (defined CPL_CONST)
#error CPL_CONST already defined
#endif
#define CPL_CONST

#if (defined CPL_APPL_DATA)
#error CPL_APPL_DATA already defined
#endif
#define CPL_APPL_DATA

#if (defined CPL_APPL_CONST)
#error CPL_APPL_CONST already defined
#endif
#define CPL_APPL_CONST

#if (defined CPL_APPL_CODE)
#error CPL_APPL_CODE already defined
#endif
#define CPL_APPL_CODE

#if (defined CPL_VAR_NOINIT)
#error CPL_VAR_NOINIT already defined
#endif
#define CPL_VAR_NOINIT

#if (defined CPL_VAR_CLEARED)
#error CPL_VAR_CLEARED already defined
#endif
#define CPL_VAR_CLEARED

#if (defined CPL_VAR_POWER_ON_INIT)
#error CPL_VAR_POWER_ON_INIT already defined
#endif
#define CPL_VAR_POWER_ON_INIT

#if (defined CPL_VAR_FAST)
#error CPL_VAR_FAST already defined
#endif
#define CPL_VAR_FAST

#if (defined CPL_VAR)
#error CPL_VAR already defined
#endif
#define CPL_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CRC_CODE)
#error CRC_CODE already defined
#endif
#define CRC_CODE

#if (defined CRC_CONST)
#error CRC_CONST already defined
#endif
#define CRC_CONST

#if (defined CRC_APPL_DATA)
#error CRC_APPL_DATA already defined
#endif
#define CRC_APPL_DATA

#if (defined CRC_APPL_CONST)
#error CRC_APPL_CONST already defined
#endif
#define CRC_APPL_CONST

#if (defined CRC_APPL_CODE)
#error CRC_APPL_CODE already defined
#endif
#define CRC_APPL_CODE

#if (defined CRC_VAR_NOINIT)
#error CRC_VAR_NOINIT already defined
#endif
#define CRC_VAR_NOINIT

#if (defined CRC_VAR_CLEARED)
#error CRC_VAR_CLEARED already defined
#endif
#define CRC_VAR_CLEARED

#if (defined CRC_VAR_POWER_ON_INIT)
#error CRC_VAR_POWER_ON_INIT already defined
#endif
#define CRC_VAR_POWER_ON_INIT

#if (defined CRC_VAR_FAST)
#error CRC_VAR_FAST already defined
#endif
#define CRC_VAR_FAST

#if (defined CRC_VAR)
#error CRC_VAR already defined
#endif
#define CRC_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CRYIF_VAR)
#error CRYIF_VAR already defined
#endif
#define CRYIF_VAR
#if (defined CRYIF_CODE)
#error CRYIF_CODE already defined
#endif
#define CRYIF_CODE
#if (defined CRYIF_APPL_DATA)
#error CRYIF_APPL_DATA already defined
#endif
#define CRYIF_APPL_DATA
#if (defined CRYIF_CONST)
#error CRYIF_CONST already defined
#endif
#define CRYIF_CONST

#if (defined CRYIF_APPL_CONST)
#error CRYIF_APPL_CONST already defined
#endif
#define CRYIF_APPL_CONST

#if (defined CRYIF_APPL_CODE)
#error CRYIF_APPL_CODE already defined
#endif
#define CRYIF_APPL_CODE

#if (defined CRYIF_VAR_NOINIT)
#error CRYIF_VAR_NOINIT already defined
#endif
#define CRYIF_VAR_NOINIT

#if (defined CRYIF_VAR_CLEARED)
#error CRYIF_VAR_CLEARED already defined
#endif
#define CRYIF_VAR_CLEARED

#if (defined CRYIF_VAR_POWER_ON_INIT)
#error CRYIF_VAR_POWER_ON_INIT already defined
#endif
#define CRYIF_VAR_POWER_ON_INIT

#if (defined CRYIF_VAR_FAST)
#error CRYIF_VAR_FAST already defined
#endif
#define CRYIF_VAR_FAST
/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CRY_CODE)
#error CRY_CODE already defined
#endif
#define CRY_CODE

#if (defined CRY_CONST)
#error CRY_CONST already defined
#endif
#define CRY_CONST

#if (defined CRY_APPL_DATA)
#error CRY_APPL_DATA already defined
#endif
#define CRY_APPL_DATA

#if (defined CRY_APPL_CONST)
#error CRY_APPL_CONST already defined
#endif
#define CRY_APPL_CONST

#if (defined CRY_APPL_CODE)
#error CRY_APPL_CODE already defined
#endif
#define CRY_APPL_CODE

#if (defined CRY_VAR_NOINIT)
#error CRY_VAR_NOINIT already defined
#endif
#define CRY_VAR_NOINIT

#if (defined CRY_VAR_CLEARED)
#error CRY_VAR_CLEARED already defined
#endif
#define CRY_VAR_CLEARED

#if (defined CRY_VAR_POWER_ON_INIT)
#error CRY_VAR_POWER_ON_INIT already defined
#endif
#define CRY_VAR_POWER_ON_INIT

#if (defined CRY_VAR_FAST)
#error CRY_VAR_FAST already defined
#endif
#define CRY_VAR_FAST

#if (defined CRY_VAR)
#error CRY_VAR already defined
#endif
#define CRY_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CRYPTO_CODE)
#error CRYPTO_CODE already defined
#endif
#define CRYPTO_CODE

#if (defined CRYPTO_APPL_DATA)
#error CRYPTO_APPL_DATA already defined
#endif
#define CRYPTO_APPL_DATA

#if (defined CRYPTO_APPL_CONST)
#error CRYPTO_APPL_CONST already defined
#endif
#define CRYPTO_APPL_CONST

#if (defined CRYPTO_CONST)
#error CRYPTO_CONST already defined
#endif
#define CRYPTO_CONST

#if (defined CRYPTO_VAR)
#error CRYPTO_VAR already defined
#endif
#define CRYPTO_VAR

#if (defined CRYPTO_APPL_CODE)
#error CRYPTO_APPL_CODE already defined
#endif
#define CRYPTO_APPL_CODE

#if (defined CRYPTO_VAR_NOINIT)
#error CRYPTO_VAR_NOINIT already defined
#endif
#define CRYPTO_VAR_NOINIT

#if (defined CRYPTO_VAR_CLEARED)
#error CRYPTO_VAR_CLEARED already defined
#endif
#define CRYPTO_VAR_CLEARED

#if (defined CRYPTO_VAR_POWER_ON_INIT)
#error CRYPTO_VAR_POWER_ON_INIT already defined
#endif
#define CRYPTO_VAR_POWER_ON_INIT

#if (defined CRYPTO_VAR_FAST)
#error CRYPTO_VAR_FAST already defined
#endif
#define CRYPTO_VAR_FAST

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined CSM_CODE)
#error CSM_CODE already defined
#endif
#define CSM_CODE

#if (defined CSM_CONST)
#error CSM_CONST already defined
#endif
#define CSM_CONST

#if (defined CSM_APPL_DATA)
#error CSM_APPL_DATA already defined
#endif
#define CSM_APPL_DATA

#if (defined CSM_APPL_CONST)
#error CSM_APPL_CONST already defined
#endif
#define CSM_APPL_CONST

#if (defined CSM_APPL_CODE)
#error CSM_APPL_CODE already defined
#endif
#define CSM_APPL_CODE

#if (defined CSM_VAR_NOINIT)
#error CSM_VAR_NOINIT already defined
#endif
#define CSM_VAR_NOINIT

#if (defined CSM_VAR_CLEARED)
#error CSM_VAR_CLEARED already defined
#endif
#define CSM_VAR_CLEARED

#if (defined CSM_VAR_POWER_ON_INIT)
#error CSM_VAR_POWER_ON_INIT already defined
#endif
#define CSM_VAR_POWER_ON_INIT

#if (defined CSM_VAR_FAST)
#error CSM_VAR_FAST already defined
#endif
#define CSM_VAR_FAST

#if (defined CSM_VAR)
#error CSM_VAR already defined
#endif
#define CSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DBG_CODE)
#error DBG_CODE already defined
#endif
#define DBG_CODE

#if (defined DBG_CONST)
#error DBG_CONST already defined
#endif
#define DBG_CONST

#if (defined DBG_APPL_DATA)
#error DBG_APPL_DATA already defined
#endif
#define DBG_APPL_DATA

#if (defined DBG_APPL_CONST)
#error DBG_APPL_CONST already defined
#endif
#define DBG_APPL_CONST

#if (defined DBG_APPL_CODE)
#error DBG_APPL_CODE already defined
#endif
#define DBG_APPL_CODE

#if (defined DBG_VAR_NOINIT)
#error DBG_VAR_NOINIT already defined
#endif
#define DBG_VAR_NOINIT

#if (defined DBG_VAR_CLEARED)
#error DBG_VAR_CLEARED already defined
#endif
#define DBG_VAR_CLEARED

#if (defined DBG_VAR_POWER_ON_INIT)
#error DBG_VAR_POWER_ON_INIT already defined
#endif
#define DBG_VAR_POWER_ON_INIT

#if (defined DBG_VAR_FAST)
#error DBG_VAR_FAST already defined
#endif
#define DBG_VAR_FAST

#if (defined DBG_VAR)
#error DBG_VAR already defined
#endif
#define DBG_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DCM_CODE)
#error DCM_CODE already defined
#endif
#define DCM_CODE

#if (defined DCM_CONST)
#error DCM_CONST already defined
#endif
#define DCM_CONST

#if (defined DCM_APPL_DATA)
#error DCM_APPL_DATA already defined
#endif
#define DCM_APPL_DATA

#if (defined DCM_APPL_CONST)
#error DCM_APPL_CONST already defined
#endif
#define DCM_APPL_CONST

#if (defined DCM_APPL_CODE)
#error DCM_APPL_CODE already defined
#endif
#define DCM_APPL_CODE

#if (defined DCM_VAR_NOINIT)
#error DCM_VAR_NOINIT already defined
#endif
#define DCM_VAR_NOINIT

#if (defined DCM_VAR_CLEARED)
#error DCM_VAR_CLEARED already defined
#endif
#define DCM_VAR_CLEARED

#if (defined DCM_VAR_POWER_ON_INIT)
#error DCM_VAR_POWER_ON_INIT already defined
#endif
#define DCM_VAR_POWER_ON_INIT

#if (defined DCM_VAR_POWER_ON_CLEARED)
#error DCM_VAR_POWER_ON_CLEARED already defined
#endif
#define DCM_VAR_POWER_ON_CLEARED

#if (defined DCM_VAR_FAST)
#error DCM_VAR_FAST already defined
#endif
#define DCM_VAR_FAST

#if (defined DCM_VAR)
#error DCM_VAR already defined
#endif
#define DCM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DEM_CODE)
#error DEM_CODE already defined
#endif
#define DEM_CODE

#if (defined DEM_CONST)
#error DEM_CONST already defined
#endif
#define DEM_CONST

#if (defined DEM_APPL_DATA)
#error DEM_APPL_DATA already defined
#endif
#define DEM_APPL_DATA

#if (defined DEM_APPL_CONST)
#error DEM_APPL_CONST already defined
#endif
#define DEM_APPL_CONST

#if (defined DEM_APPL_CODE)
#error DEM_APPL_CODE already defined
#endif
#define DEM_APPL_CODE

#if (defined DEM_VAR_NOINIT)
#error DEM_VAR_NOINIT already defined
#endif
#define DEM_VAR_NOINIT

#if (defined DEM_VAR_CLEARED)
#error DEM_VAR_CLEARED already defined
#endif
#define DEM_VAR_CLEARED

#if (defined DEM_VAR_POWER_ON_INIT)
#error DEM_VAR_POWER_ON_INIT already defined
#endif
#define DEM_VAR_POWER_ON_INIT

#if (defined DEM_VAR_POWER_ON_CLEARED)
#error DEM_VAR_POWER_ON_CLEARED already defined
#endif
#define DEM_VAR_POWER_ON_CLEARED

#if (defined DEM_VAR_FAST)
#error DEM_VAR_FAST already defined
#endif
#define DEM_VAR_FAST

#if (defined DEM_VAR)
#error DEM_VAR already defined
#endif
#define DEM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DET_CODE)
#error DET_CODE already defined
#endif
#define DET_CODE

#if (defined DET_CONST)
#error DET_CONST already defined
#endif
#define DET_CONST

#if (defined DET_APPL_DATA)
#error DET_APPL_DATA already defined
#endif
#define DET_APPL_DATA

#if (defined DET_APPL_CONST)
#error DET_APPL_CONST already defined
#endif
#define DET_APPL_CONST

#if (defined DET_APPL_CODE)
#error DET_APPL_CODE already defined
#endif
#define DET_APPL_CODE

#if (defined DET_VAR_NOINIT)
#error DET_VAR_NOINIT already defined
#endif
#define DET_VAR_NOINIT

#if (defined DET_VAR_CLEARED)
#error DET_VAR_CLEARED already defined
#endif
#define DET_VAR_CLEARED

#if (defined DET_VAR_POWER_ON_INIT)
#error DET_VAR_POWER_ON_INIT already defined
#endif
#define DET_VAR_POWER_ON_INIT

#if (defined DET_VAR_FAST)
#error DET_VAR_FAST already defined
#endif
#define DET_VAR_FAST

#if (defined DET_VAR)
#error DET_VAR already defined
#endif
#define DET_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DIO_CODE)
#error DIO_CODE already defined
#endif
#define DIO_CODE

#if (defined DIO_CONST)
#error DIO_CONST already defined
#endif
#define DIO_CONST

#if (defined DIO_APPL_DATA)
#error DIO_APPL_DATA already defined
#endif
#define DIO_APPL_DATA

#if (defined DIO_APPL_CONST)
#error DIO_APPL_CONST already defined
#endif
#define DIO_APPL_CONST

#if (defined DIO_APPL_CODE)
#error DIO_APPL_CODE already defined
#endif
#define DIO_APPL_CODE

#if (defined DIO_VAR_NOINIT)
#error DIO_VAR_NOINIT already defined
#endif
#define DIO_VAR_NOINIT

#if (defined DIO_VAR_CLEARED)
#error DIO_VAR_CLEARED already defined
#endif
#define DIO_VAR_CLEARED

#if (defined DIO_VAR_POWER_ON_INIT)
#error DIO_VAR_POWER_ON_INIT already defined
#endif
#define DIO_VAR_POWER_ON_INIT

#if (defined DIO_VAR_FAST)
#error DIO_VAR_FAST already defined
#endif
#define DIO_VAR_FAST

#if (defined DIO_VAR)
#error DIO_VAR already defined
#endif
#define DIO_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DLT_CODE)
#error DLT_CODE already defined
#endif
#define DLT_CODE

#if (defined DLT_CONST)
#error DLT_CONST already defined
#endif
#define DLT_CONST

#if (defined DLT_APPL_DATA)
#error DLT_APPL_DATA already defined
#endif
#define DLT_APPL_DATA

#if (defined DLT_APPL_CONST)
#error DLT_APPL_CONST already defined
#endif
#define DLT_APPL_CONST

#if (defined DLT_APPL_CODE)
#error DLT_APPL_CODE already defined
#endif
#define DLT_APPL_CODE

#if (defined DLT_VAR_NOINIT)
#error DLT_VAR_NOINIT already defined
#endif
#define DLT_VAR_NOINIT

#if (defined DLT_VAR_CLEARED)
#error DLT_VAR_CLEARED already defined
#endif
#define DLT_VAR_CLEARED

#if (defined DLT_VAR_POWER_ON_INIT)
#error DLT_VAR_POWER_ON_INIT already defined
#endif
#define DLT_VAR_POWER_ON_INIT

#if (defined DLT_VAR_FAST)
#error DLT_VAR_FAST already defined
#endif
#define DLT_VAR_FAST

#if (defined DLT_VAR)
#error DLT_VAR already defined
#endif
#define DLT_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DOIP_CODE)
#error DOIP_CODE already defined
#endif
#define DOIP_CODE

#if (defined DOIP_CONST)
#error DOIP_CONST already defined
#endif
#define DOIP_CONST

#if (defined DOIP_APPL_DATA)
#error DOIP_APPL_DATA already defined
#endif
#define DOIP_APPL_DATA

#if (defined DOIP_APPL_CONST)
#error DOIP_APPL_CONST already defined
#endif
#define DOIP_APPL_CONST

#if (defined DOIP_APPL_CODE)
#error DOIP_APPL_CODE already defined
#endif
#define DOIP_APPL_CODE

#if (defined DOIP_VAR_NOINIT)
#error DOIP_VAR_NOINIT already defined
#endif
#define DOIP_VAR_NOINIT

#if (defined DOIP_VAR_CLEARED)
#error DOIP_VAR_CLEARED already defined
#endif
#define DOIP_VAR_CLEARED

#if (defined DOIP_VAR_POWER_ON_INIT)
#error DOIP_VAR_POWER_ON_INIT already defined
#endif
#define DOIP_VAR_POWER_ON_INIT

#if (defined DOIP_VAR_FAST)
#error DOIP_VAR_FAST already defined
#endif
#define DOIP_VAR_FAST

#if (defined DOIP_VAR)
#error DOIP_VAR already defined
#endif
#define DOIP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined DEVAUTH_CODE)
#error DEVAUTH_CODE already defined
#endif
#define DEVAUTH_CODE

#if (defined DEVAUTH_CONST)
#error DEVAUTH_CONST already defined
#endif
#define DEVAUTH_CONST

#if (defined DEVAUTH_APPL_DATA)
#error DEVAUTH_APPL_DATA already defined
#endif
#define DEVAUTH_APPL_DATA

#if (defined DEVAUTH_APPL_CONST)
#error DEVAUTH_APPL_CONST already defined
#endif
#define DEVAUTH_APPL_CONST

#if (defined DEVAUTH_APPL_CODE)
#error DEVAUTH_APPL_CODE already defined
#endif
#define DEVAUTH_APPL_CODE

#if (defined DEVAUTH_VAR_NOINIT)
#error DEVAUTH_VAR_NOINIT already defined
#endif
#define DEVAUTH_VAR_NOINIT

#if (defined DEVAUTH_VAR_CLEARED)
#error DEVAUTH_VAR_CLEARED already defined
#endif
#define DEVAUTH_VAR_CLEARED

#if (defined DEVAUTH_VAR_POWER_ON_INIT)
#error DEVAUTH_VAR_POWER_ON_INIT already defined
#endif
#define DEVAUTH_VAR_POWER_ON_INIT

#if (defined DEVAUTH_VAR_FAST)
#error DEVAUTH_VAR_FAST already defined
#endif
#define DEVAUTH_VAR_FAST

#if (defined DEVAUTH_VAR)
#error DEVAUTH_VAR already defined
#endif
#define DEVAUTH_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2E_CODE)
#error E2E_CODE already defined
#endif
#define E2E_CODE

#if (defined E2E_CONST)
#error E2E_CONST already defined
#endif
#define E2E_CONST

#if (defined E2E_APPL_DATA)
#error E2E_APPL_DATA already defined
#endif
#define E2E_APPL_DATA

#if (defined E2E_APPL_CONST)
#error E2E_APPL_CONST already defined
#endif
#define E2E_APPL_CONST

#if (defined E2E_APPL_CODE)
#error E2E_APPL_CODE already defined
#endif
#define E2E_APPL_CODE

#if (defined E2E_VAR_NOINIT)
#error E2E_VAR_NOINIT already defined
#endif
#define E2E_VAR_NOINIT

#if (defined E2E_VAR_CLEARED)
#error E2E_VAR_CLEARED already defined
#endif
#define E2E_VAR_CLEARED

#if (defined E2E_VAR_POWER_ON_INIT)
#error E2E_VAR_POWER_ON_INIT already defined
#endif
#define E2E_VAR_POWER_ON_INIT

#if (defined E2E_VAR_FAST)
#error E2E_VAR_FAST already defined
#endif
#define E2E_VAR_FAST

#if (defined E2E_VAR)
#error E2E_VAR already defined
#endif
#define E2E_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EP01_CODE)
#error E2EP01_CODE already defined
#endif
#define E2EP01_CODE

#if (defined E2EP01_CONST)
#error E2EP01_CONST already defined
#endif
#define E2EP01_CONST

#if (defined E2EP01_APPL_DATA)
#error E2EP01_APPL_DATA already defined
#endif
#define E2EP01_APPL_DATA

#if (defined E2EP01_APPL_CONST)
#error E2EP01_APPL_CONST already defined
#endif
#define E2EP01_APPL_CONST

#if (defined E2EP01_APPL_CODE)
#error E2EP01_APPL_CODE already defined
#endif
#define E2EP01_APPL_CODE

#if (defined E2EP01_VAR_NOINIT)
#error E2EP01_VAR_NOINIT already defined
#endif
#define E2EP01_VAR_NOINIT

#if (defined E2EP01_VAR_CLEARED)
#error E2EP01_VAR_CLEARED already defined
#endif
#define E2EP01_VAR_CLEARED

#if (defined E2EP01_VAR_POWER_ON_INIT)
#error E2EP01_VAR_POWER_ON_INIT already defined
#endif
#define E2EP01_VAR_POWER_ON_INIT

#if (defined E2EP01_VAR_FAST)
#error E2EP01_VAR_FAST already defined
#endif
#define E2EP01_VAR_FAST

#if (defined E2EP01_VAR)
#error E2EP01_VAR already defined
#endif
#define E2EP01_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EP02_CODE)
#error E2EP02_CODE already defined
#endif
#define E2EP02_CODE

#if (defined E2EP02_CONST)
#error E2EP02_CONST already defined
#endif
#define E2EP02_CONST

#if (defined E2EP02_APPL_DATA)
#error E2EP02_APPL_DATA already defined
#endif
#define E2EP02_APPL_DATA

#if (defined E2EP02_APPL_CONST)
#error E2EP02_APPL_CONST already defined
#endif
#define E2EP02_APPL_CONST

#if (defined E2EP02_APPL_CODE)
#error E2EP02_APPL_CODE already defined
#endif
#define E2EP02_APPL_CODE

#if (defined E2EP02_VAR_NOINIT)
#error E2EP02_VAR_NOINIT already defined
#endif
#define E2EP02_VAR_NOINIT

#if (defined E2EP02_VAR_CLEARED)
#error E2EP02_VAR_CLEARED already defined
#endif
#define E2EP02_VAR_CLEARED

#if (defined E2EP02_VAR_POWER_ON_INIT)
#error E2EP02_VAR_POWER_ON_INIT already defined
#endif
#define E2EP02_VAR_POWER_ON_INIT

#if (defined E2EP02_VAR_FAST)
#error E2EP02_VAR_FAST already defined
#endif
#define E2EP02_VAR_FAST

#if (defined E2EP02_VAR)
#error E2EP02_VAR already defined
#endif
#define E2EP02_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EP04_CODE)
#error E2EP04_CODE already defined
#endif
#define E2EP04_CODE

#if (defined E2EP04_CONST)
#error E2EP04_CONST already defined
#endif
#define E2EP04_CONST

#if (defined E2EP04_APPL_DATA)
#error E2EP04_APPL_DATA already defined
#endif
#define E2EP04_APPL_DATA

#if (defined E2EP04_APPL_CONST)
#error E2EP04_APPL_CONST already defined
#endif
#define E2EP04_APPL_CONST

#if (defined E2EP04_APPL_CODE)
#error E2EP04_APPL_CODE already defined
#endif
#define E2EP04_APPL_CODE

#if (defined E2EP04_VAR_NOINIT)
#error E2EP04_VAR_NOINIT already defined
#endif
#define E2EP04_VAR_NOINIT

#if (defined E2EP04_VAR_CLEARED)
#error E2EP04_VAR_CLEARED already defined
#endif
#define E2EP04_VAR_CLEARED

#if (defined E2EP04_VAR_POWER_ON_INIT)
#error E2EP04_VAR_POWER_ON_INIT already defined
#endif
#define E2EP04_VAR_POWER_ON_INIT

#if (defined E2EP04_VAR_FAST)
#error E2EP04_VAR_FAST already defined
#endif
#define E2EP04_VAR_FAST

#if (defined E2EP04_VAR)
#error E2EP04_VAR already defined
#endif
#define E2EP04_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EP05_CODE)
#error E2EP05_CODE already defined
#endif
#define E2EP05_CODE

#if (defined E2EP05_CONST)
#error E2EP05_CONST already defined
#endif
#define E2EP05_CONST

#if (defined E2EP05_APPL_DATA)
#error E2EP05_APPL_DATA already defined
#endif
#define E2EP05_APPL_DATA

#if (defined E2EP05_APPL_CONST)
#error E2EP05_APPL_CONST already defined
#endif
#define E2EP05_APPL_CONST

#if (defined E2EP05_APPL_CODE)
#error E2EP05_APPL_CODE already defined
#endif
#define E2EP05_APPL_CODE

#if (defined E2EP05_VAR_NOINIT)
#error E2EP05_VAR_NOINIT already defined
#endif
#define E2EP05_VAR_NOINIT

#if (defined E2EP05_VAR_CLEARED)
#error E2EP05_VAR_CLEARED already defined
#endif
#define E2EP05_VAR_CLEARED

#if (defined E2EP05_VAR_POWER_ON_INIT)
#error E2EP05_VAR_POWER_ON_INIT already defined
#endif
#define E2EP05_VAR_POWER_ON_INIT

#if (defined E2EP05_VAR_FAST)
#error E2EP05_VAR_FAST already defined
#endif
#define E2EP05_VAR_FAST

#if (defined E2EP05_VAR)
#error E2EP05_VAR already defined
#endif
#define E2EP05_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EP06_CODE)
#error E2EP06_CODE already defined
#endif
#define E2EP06_CODE

#if (defined E2EP06_CONST)
#error E2EP06_CONST already defined
#endif
#define E2EP06_CONST

#if (defined E2EP06_APPL_DATA)
#error E2EP06_APPL_DATA already defined
#endif
#define E2EP06_APPL_DATA

#if (defined E2EP06_APPL_CONST)
#error E2EP06_APPL_CONST already defined
#endif
#define E2EP06_APPL_CONST

#if (defined E2EP06_APPL_CODE)
#error E2EP06_APPL_CODE already defined
#endif
#define E2EP06_APPL_CODE

#if (defined E2EP06_VAR_NOINIT)
#error E2EP06_VAR_NOINIT already defined
#endif
#define E2EP06_VAR_NOINIT

#if (defined E2EP06_VAR_CLEARED)
#error E2EP06_VAR_CLEARED already defined
#endif
#define E2EP06_VAR_CLEARED

#if (defined E2EP06_VAR_POWER_ON_INIT)
#error E2EP06_VAR_POWER_ON_INIT already defined
#endif
#define E2EP06_VAR_POWER_ON_INIT

#if (defined E2EP06_VAR_FAST)
#error E2EP06_VAR_FAST already defined
#endif
#define E2EP06_VAR_FAST

#if (defined E2EP06_VAR)
#error E2EP06_VAR already defined
#endif
#define E2EP06_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EPDAI1_CODE)
#error E2EPDAI1_CODE already defined
#endif
#define E2EPDAI1_CODE

#if (defined E2EPDAI1_CONST)
#error E2EPDAI1_CONST already defined
#endif
#define E2EPDAI1_CONST

#if (defined E2EPDAI1_APPL_DATA)
#error E2EPDAI1_APPL_DATA already defined
#endif
#define E2EPDAI1_APPL_DATA

#if (defined E2EPDAI1_APPL_CONST)
#error E2EPDAI1_APPL_CONST already defined
#endif
#define E2EPDAI1_APPL_CONST

#if (defined E2EPDAI1_APPL_CODE)
#error E2EPDAI1_APPL_CODE already defined
#endif
#define E2EPDAI1_APPL_CODE

#if (defined E2EPDAI1_VAR_NOINIT)
#error E2EPDAI1_VAR_NOINIT already defined
#endif
#define E2EPDAI1_VAR_NOINIT

#if (defined E2EPDAI1_VAR_CLEARED)
#error E2EPDAI1_VAR_CLEARED already defined
#endif
#define E2EPDAI1_VAR_CLEARED

#if (defined E2EPDAI1_VAR_POWER_ON_INIT)
#error E2EPDAI1_VAR_POWER_ON_INIT already defined
#endif
#define E2EPDAI1_VAR_POWER_ON_INIT

#if (defined E2EPDAI1_VAR_FAST)
#error E2EPDAI1_VAR_FAST already defined
#endif
#define E2EPDAI1_VAR_FAST

#if (defined E2EPDAI1_VAR)
#error E2EPDAI1_VAR already defined
#endif
#define E2EPDAI1_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EPDAI2_CODE)
#error E2EPDAI2_CODE already defined
#endif
#define E2EPDAI2_CODE

#if (defined E2EPDAI2_CONST)
#error E2EPDAI2_CONST already defined
#endif
#define E2EPDAI2_CONST

#if (defined E2EPDAI2_APPL_DATA)
#error E2EPDAI2_APPL_DATA already defined
#endif
#define E2EPDAI2_APPL_DATA

#if (defined E2EPDAI2_APPL_CONST)
#error E2EPDAI2_APPL_CONST already defined
#endif
#define E2EPDAI2_APPL_CONST

#if (defined E2EPDAI2_APPL_CODE)
#error E2EPDAI2_APPL_CODE already defined
#endif
#define E2EPDAI2_APPL_CODE

#if (defined E2EPDAI2_VAR_NOINIT)
#error E2EPDAI2_VAR_NOINIT already defined
#endif
#define E2EPDAI2_VAR_NOINIT

#if (defined E2EPDAI2_VAR_CLEARED)
#error E2EPDAI2_VAR_CLEARED already defined
#endif
#define E2EPDAI2_VAR_CLEARED

#if (defined E2EPDAI2_VAR_POWER_ON_INIT)
#error E2EPDAI2_VAR_POWER_ON_INIT already defined
#endif
#define E2EPDAI2_VAR_POWER_ON_INIT

#if (defined E2EPDAI2_VAR_FAST)
#error E2EPDAI2_VAR_FAST already defined
#endif
#define E2EPDAI2_VAR_FAST

#if (defined E2EPDAI2_VAR)
#error E2EPDAI2_VAR already defined
#endif
#define E2EPDAI2_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2ESM_CODE)
#error E2ESM_CODE already defined
#endif
#define E2ESM_CODE

#if (defined E2ESM_CONST)
#error E2ESM_CONST already defined
#endif
#define E2ESM_CONST

#if (defined E2ESM_APPL_DATA)
#error E2ESM_APPL_DATA already defined
#endif
#define E2ESM_APPL_DATA

#if (defined E2ESM_APPL_CONST)
#error E2ESM_APPL_CONST already defined
#endif
#define E2ESM_APPL_CONST

#if (defined E2ESM_APPL_CODE)
#error E2ESM_APPL_CODE already defined
#endif
#define E2ESM_APPL_CODE

#if (defined E2ESM_VAR_NOINIT)
#error E2ESM_VAR_NOINIT already defined
#endif
#define E2ESM_VAR_NOINIT

#if (defined E2ESM_VAR_CLEARED)
#error E2ESM_VAR_CLEARED already defined
#endif
#define E2ESM_VAR_CLEARED

#if (defined E2ESM_VAR_POWER_ON_INIT)
#error E2ESM_VAR_POWER_ON_INIT already defined
#endif
#define E2ESM_VAR_POWER_ON_INIT

#if (defined E2ESM_VAR_FAST)
#error E2ESM_VAR_FAST already defined
#endif
#define E2ESM_VAR_FAST

#if (defined E2ESM_VAR)
#error E2ESM_VAR already defined
#endif
#define E2ESM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EPW_CODE)
#error E2EPW_CODE already defined
#endif
#define E2EPW_CODE

#if (defined E2EPW_CONST)
#error E2EPW_CONST already defined
#endif
#define E2EPW_CONST

#if (defined E2EPW_APPL_DATA)
#error E2EPW_APPL_DATA already defined
#endif
#define E2EPW_APPL_DATA

#if (defined E2EPW_APPL_CONST)
#error E2EPW_APPL_CONST already defined
#endif
#define E2EPW_APPL_CONST

#if (defined E2EPW_APPL_CODE)
#error E2EPW_APPL_CODE already defined
#endif
#define E2EPW_APPL_CODE

#if (defined E2EPW_VAR_NOINIT)
#error E2EPW_VAR_NOINIT already defined
#endif
#define E2EPW_VAR_NOINIT

#if (defined E2EPW_VAR_CLEARED)
#error E2EPW_VAR_CLEARED already defined
#endif
#define E2EPW_VAR_CLEARED

#if (defined E2EPW_VAR_POWER_ON_INIT)
#error E2EPW_VAR_POWER_ON_INIT already defined
#endif
#define E2EPW_VAR_POWER_ON_INIT

#if (defined E2EPW_VAR_FAST)
#error E2EPW_VAR_FAST already defined
#endif
#define E2EPW_VAR_FAST

#if (defined E2EPW_VAR)
#error E2EPW_VAR already defined
#endif
#define E2EPW_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined E2EXF_CODE)
#error E2EXF_CODE already defined
#endif
#define E2EXF_CODE

#if (defined E2EXF_CONST)
#error E2EXF_CONST already defined
#endif
#define E2EXF_CONST

#if (defined E2EXF_APPL_DATA)
#error E2EXF_APPL_DATA already defined
#endif
#define E2EXF_APPL_DATA

#if (defined E2EXF_APPL_CONST)
#error E2EXF_APPL_CONST already defined
#endif
#define E2EXF_APPL_CONST

#if (defined E2EXF_APPL_CODE)
#error E2EXF_APPL_CODE already defined
#endif
#define E2EXF_APPL_CODE

#if (defined E2EXF_VAR_NOINIT)
#error E2EXF_VAR_NOINIT already defined
#endif
#define E2EXF_VAR_NOINIT

#if (defined E2EXF_VAR_CLEARED)
#error E2EXF_VAR_CLEARED already defined
#endif
#define E2EXF_VAR_CLEARED

#if (defined E2EXF_VAR_POWER_ON_INIT)
#error E2EXF_VAR_POWER_ON_INIT already defined
#endif
#define E2EXF_VAR_POWER_ON_INIT

#if (defined E2EXF_VAR_FAST)
#error E2EXF_VAR_FAST already defined
#endif
#define E2EXF_VAR_FAST

#if (defined E2EXF_VAR)
#error E2EXF_VAR already defined
#endif
#define E2EXF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined EA_CODE)
#error EA_CODE already defined
#endif
#define EA_CODE

#if (defined EA_CONST)
#error EA_CONST already defined
#endif
#define EA_CONST

#if (defined EA_APPL_DATA)
#error EA_APPL_DATA already defined
#endif
#define EA_APPL_DATA

#if (defined EA_APPL_CONST)
#error EA_APPL_CONST already defined
#endif
#define EA_APPL_CONST

#if (defined EA_APPL_CODE)
#error EA_APPL_CODE already defined
#endif
#define EA_APPL_CODE

#if (defined EA_VAR_NOINIT)
#error EA_VAR_NOINIT already defined
#endif
#define EA_VAR_NOINIT

#if (defined EA_VAR_CLEARED)
#error EA_VAR_CLEARED already defined
#endif
#define EA_VAR_CLEARED

#if (defined EA_VAR_POWER_ON_INIT)
#error EA_VAR_POWER_ON_INIT already defined
#endif
#define EA_VAR_POWER_ON_INIT

#if (defined EA_VAR_FAST)
#error EA_VAR_FAST already defined
#endif
#define EA_VAR_FAST

#if (defined EA_VAR)
#error EA_VAR already defined
#endif
#define EA_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined EBSTUBS_CODE)
#error EBSTUBS_CODE already defined
#endif
#define EBSTUBS_CODE

#if (defined EBSTUBS_CONST)
#error EBSTUBS_CONST already defined
#endif
#define EBSTUBS_CONST

#if (defined EBSTUBS_APPL_DATA)
#error EBSTUBS_APPL_DATA already defined
#endif
#define EBSTUBS_APPL_DATA

#if (defined EBSTUBS_APPL_CONST)
#error EBSTUBS_APPL_CONST already defined
#endif
#define EBSTUBS_APPL_CONST

#if (defined EBSTUBS_APPL_CODE)
#error EBSTUBS_APPL_CODE already defined
#endif
#define EBSTUBS_APPL_CODE

#if (defined EBSTUBS_VAR_NOINIT)
#error EBSTUBS_VAR_NOINIT already defined
#endif
#define EBSTUBS_VAR_NOINIT

#if (defined EBSTUBS_VAR_CLEARED)
#error EBSTUBS_VAR_CLEARED already defined
#endif
#define EBSTUBS_VAR_CLEARED

#if (defined EBSTUBS_VAR_POWER_ON_INIT)
#error EBSTUBS_VAR_POWER_ON_INIT already defined
#endif
#define EBSTUBS_VAR_POWER_ON_INIT

#if (defined EBSTUBS_VAR_FAST)
#error EBSTUBS_VAR_FAST already defined
#endif
#define EBSTUBS_VAR_FAST

#if (defined EBSTUBS_VAR)
#error EBSTUBS_VAR already defined
#endif
#define EBSTUBS_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined EBTEST_CODE)
#error EBTEST_CODE already defined
#endif
#define EBTEST_CODE

#if (defined EBTEST_CONST)
#error EBTEST_CONST already defined
#endif
#define EBTEST_CONST

#if (defined EBTEST_APPL_DATA)
#error EBTEST_APPL_DATA already defined
#endif
#define EBTEST_APPL_DATA

#if (defined EBTEST_APPL_CONST)
#error EBTEST_APPL_CONST already defined
#endif
#define EBTEST_APPL_CONST

#if (defined EBTEST_APPL_CODE)
#error EBTEST_APPL_CODE already defined
#endif
#define EBTEST_APPL_CODE

#if (defined EBTEST_VAR_NOINIT)
#error EBTEST_VAR_NOINIT already defined
#endif
#define EBTEST_VAR_NOINIT

#if (defined EBTEST_VAR_CLEARED)
#error EBTEST_VAR_CLEARED already defined
#endif
#define EBTEST_VAR_CLEARED

#if (defined EBTEST_VAR_POWER_ON_INIT)
#error EBTEST_VAR_POWER_ON_INIT already defined
#endif
#define EBTEST_VAR_POWER_ON_INIT

#if (defined EBTEST_VAR_FAST)
#error EBTEST_VAR_FAST already defined
#endif
#define EBTEST_VAR_FAST

#if (defined EBTEST_VAR)
#error EBTEST_VAR already defined
#endif
#define EBTEST_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ECUC_CODE)
#error ECUC_CODE already defined
#endif
#define ECUC_CODE

#if (defined ECUC_CONST)
#error ECUC_CONST already defined
#endif
#define ECUC_CONST

#if (defined ECUC_APPL_DATA)
#error ECUC_APPL_DATA already defined
#endif
#define ECUC_APPL_DATA

#if (defined ECUC_APPL_CONST)
#error ECUC_APPL_CONST already defined
#endif
#define ECUC_APPL_CONST

#if (defined ECUC_APPL_CODE)
#error ECUC_APPL_CODE already defined
#endif
#define ECUC_APPL_CODE

#if (defined ECUC_VAR_NOINIT)
#error ECUC_VAR_NOINIT already defined
#endif
#define ECUC_VAR_NOINIT

#if (defined ECUC_VAR_CLEARED)
#error ECUC_VAR_CLEARED already defined
#endif
#define ECUC_VAR_CLEARED

#if (defined ECUC_VAR_POWER_ON_INIT)
#error ECUC_VAR_POWER_ON_INIT already defined
#endif
#define ECUC_VAR_POWER_ON_INIT

#if (defined ECUC_VAR_FAST)
#error ECUC_VAR_FAST already defined
#endif
#define ECUC_VAR_FAST

#if (defined ECUC_VAR)
#error ECUC_VAR already defined
#endif
#define ECUC_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ECUM_CODE)
#error ECUM_CODE already defined
#endif
#define ECUM_CODE

#if (defined ECUM_CONST)
#error ECUM_CONST already defined
#endif
#define ECUM_CONST

#if (defined ECUM_APPL_DATA)
#error ECUM_APPL_DATA already defined
#endif
#define ECUM_APPL_DATA

#if (defined ECUM_APPL_CONST)
#error ECUM_APPL_CONST already defined
#endif
#define ECUM_APPL_CONST

#if (defined ECUM_APPL_CODE)
#error ECUM_APPL_CODE already defined
#endif
#define ECUM_APPL_CODE

#if (defined ECUM_VAR_NOINIT)
#error ECUM_VAR_NOINIT already defined
#endif
#define ECUM_VAR_NOINIT

#if (defined ECUM_VAR_CLEARED)
#error ECUM_VAR_CLEARED already defined
#endif
#define ECUM_VAR_CLEARED

#if (defined ECUM_VAR_POWER_ON_INIT)
#error ECUM_VAR_POWER_ON_INIT already defined
#endif
#define ECUM_VAR_POWER_ON_INIT

#if (defined ECUM_VAR_POWER_ON_CLEARED)
#error ECUM_VAR_POWER_ON_CLEARED already defined
#endif
#define ECUM_VAR_POWER_ON_CLEARED

#if (defined ECUM_VAR_FAST)
#error ECUM_VAR_FAST already defined
#endif
#define ECUM_VAR_FAST

#if (defined ECUM_VAR)
#error ECUM_VAR already defined
#endif
#define ECUM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined EEP_CODE)
#error EEP_CODE already defined
#endif
#define EEP_CODE

#if (defined EEP_CONST)
#error EEP_CONST already defined
#endif
#define EEP_CONST

#if (defined EEP_APPL_DATA)
#error EEP_APPL_DATA already defined
#endif
#define EEP_APPL_DATA

#if (defined EEP_APPL_CONST)
#error EEP_APPL_CONST already defined
#endif
#define EEP_APPL_CONST

#if (defined EEP_APPL_CODE)
#error EEP_APPL_CODE already defined
#endif
#define EEP_APPL_CODE

#if (defined EEP_VAR_NOINIT)
#error EEP_VAR_NOINIT already defined
#endif
#define EEP_VAR_NOINIT

#if (defined EEP_VAR_CLEARED)
#error EEP_VAR_CLEARED already defined
#endif
#define EEP_VAR_CLEARED

#if (defined EEP_VAR_POWER_ON_INIT)
#error EEP_VAR_POWER_ON_INIT already defined
#endif
#define EEP_VAR_POWER_ON_INIT

#if (defined EEP_VAR_FAST)
#error EEP_VAR_FAST already defined
#endif
#define EEP_VAR_FAST

#if (defined EEP_VAR)
#error EEP_VAR already defined
#endif
#define EEP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined EEPIF_CODE)
#error EEPIF_CODE already defined
#endif
#define EEPIF_CODE

#if (defined EEPIF_CONST)
#error EEPIF_CONST already defined
#endif
#define EEPIF_CONST

#if (defined EEPIF_APPL_DATA)
#error EEPIF_APPL_DATA already defined
#endif
#define EEPIF_APPL_DATA

#if (defined EEPIF_APPL_CONST)
#error EEPIF_APPL_CONST already defined
#endif
#define EEPIF_APPL_CONST

#if (defined EEPIF_APPL_CODE)
#error EEPIF_APPL_CODE already defined
#endif
#define EEPIF_APPL_CODE

#if (defined EEPIF_VAR_NOINIT)
#error EEPIF_VAR_NOINIT already defined
#endif
#define EEPIF_VAR_NOINIT

#if (defined EEPIF_VAR_CLEARED)
#error EEPIF_VAR_CLEARED already defined
#endif
#define EEPIF_VAR_CLEARED

#if (defined EEPIF_VAR_POWER_ON_INIT)
#error EEPIF_VAR_POWER_ON_INIT already defined
#endif
#define EEPIF_VAR_POWER_ON_INIT

#if (defined EEPIF_VAR_FAST)
#error EEPIF_VAR_FAST already defined
#endif
#define EEPIF_VAR_FAST

#if (defined EEPIF_VAR)
#error EEPIF_VAR already defined
#endif
#define EEPIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETH_CODE)
#error ETH_CODE already defined
#endif
#define ETH_CODE

#if (defined ETH_CONST)
#error ETH_CONST already defined
#endif
#define ETH_CONST

#if (defined ETH_APPL_DATA)
#error ETH_APPL_DATA already defined
#endif
#define ETH_APPL_DATA

#if (defined ETH_APPL_CONST)
#error ETH_APPL_CONST already defined
#endif
#define ETH_APPL_CONST

#if (defined ETH_APPL_CODE)
#error ETH_APPL_CODE already defined
#endif
#define ETH_APPL_CODE

#if (defined ETH_VAR_NOINIT)
#error ETH_VAR_NOINIT already defined
#endif
#define ETH_VAR_NOINIT

#if (defined ETH_VAR_CLEARED)
#error ETH_VAR_CLEARED already defined
#endif
#define ETH_VAR_CLEARED

#if (defined ETH_VAR_POWER_ON_INIT)
#error ETH_VAR_POWER_ON_INIT already defined
#endif
#define ETH_VAR_POWER_ON_INIT

#if (defined ETH_VAR_FAST)
#error ETH_VAR_FAST already defined
#endif
#define ETH_VAR_FAST

#if (defined ETH_VAR)
#error ETH_VAR already defined
#endif
#define ETH_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHAVTP_CODE)
#error ETHAVTP_CODE already defined
#endif
#define ETHAVTP_CODE

#if (defined ETHAVTP_CONST)
#error ETHAVTP_CONST already defined
#endif
#define ETHAVTP_CONST

#if (defined ETHAVTP_APPL_DATA)
#error ETHAVTP_APPL_DATA already defined
#endif
#define ETHAVTP_APPL_DATA

#if (defined ETHAVTP_APPL_CONST)
#error ETHAVTP_APPL_CONST already defined
#endif
#define ETHAVTP_APPL_CONST

#if (defined ETHAVTP_APPL_CODE)
#error ETHAVTP_APPL_CODE already defined
#endif
#define ETHAVTP_APPL_CODE

#if (defined ETHAVTP_VAR_NOINIT)
#error ETHAVTP_VAR_NOINIT already defined
#endif
#define ETHAVTP_VAR_NOINIT

#if (defined ETHAVTP_VAR_CLEARED)
#error ETHAVTP_VAR_CLEARED already defined
#endif
#define ETHAVTP_VAR_CLEARED

#if (defined ETHAVTP_VAR_POWER_ON_INIT)
#error ETHAVTP_VAR_POWER_ON_INIT already defined
#endif
#define ETHAVTP_VAR_POWER_ON_INIT

#if (defined ETHAVTP_VAR_FAST)
#error ETHAVTP_VAR_FAST already defined
#endif
#define ETHAVTP_VAR_FAST

#if (defined ETHAVTP_VAR)
#error ETHAVTP_VAR already defined
#endif
#define ETHAVTP_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHRTP_CODE)
#error ETHRTP_CODE already defined
#endif
#define ETHRTP_CODE

#if (defined ETHRTP_CONST)
#error ETHRTP_CONST already defined
#endif
#define ETHRTP_CONST

#if (defined ETHRTP_DATA)
#error ETHRTP_DATA already defined
#endif
#define ETHRTP_DATA

#if (defined ETHRTP_APPL_CONST)
#error ETHRTP_APPL_CONST already defined
#endif
#define ETHRTP_APPL_CONST

#if (defined ETHRTP_APPL_CODE)
#error ETHRTP_APPL_CODE already defined
#endif
#define ETHRTP_APPL_CODE

#if (defined ETHRTP_VAR_NOINIT)
#error ETHRTP_VAR_NOINIT already defined
#endif
#define ETHRTP_VAR_NOINIT

#if (defined ETHRTP_VAR_CLEARED)
#error ETHRTP_VAR_CLEARED already defined
#endif
#define ETHRTP_VAR_CLEARED

#if (defined ETHRTP_VAR_POWER_ON_INIT)
#error ETHRTP_VAR_POWER_ON_INIT already defined
#endif
#define ETHRTP_VAR_POWER_ON_INIT

#if (defined ETHRTP_VAR_FAST)
#error ETHRTP_VAR_FAST already defined
#endif
#define ETHRTP_VAR_FAST

#if (defined ETHRTP_VAR)
#error ETHRTP_VAR already defined
#endif
#define ETHRTP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHIF_CODE)
#error ETHIF_CODE already defined
#endif
#define ETHIF_CODE

#if (defined ETHIF_CONST)
#error ETHIF_CONST already defined
#endif
#define ETHIF_CONST

#if (defined ETHIF_APPL_DATA)
#error ETHIF_APPL_DATA already defined
#endif
#define ETHIF_APPL_DATA

#if (defined ETHIF_APPL_CONST)
#error ETHIF_APPL_CONST already defined
#endif
#define ETHIF_APPL_CONST

#if (defined ETHIF_APPL_CODE)
#error ETHIF_APPL_CODE already defined
#endif
#define ETHIF_APPL_CODE

#if (defined ETHIF_VAR_NOINIT)
#error ETHIF_VAR_NOINIT already defined
#endif
#define ETHIF_VAR_NOINIT

#if (defined ETHIF_VAR_CLEARED)
#error ETHIF_VAR_CLEARED already defined
#endif
#define ETHIF_VAR_CLEARED

#if (defined ETHIF_VAR_POWER_ON_INIT)
#error ETHIF_VAR_POWER_ON_INIT already defined
#endif
#define ETHIF_VAR_POWER_ON_INIT

#if (defined ETHIF_VAR_FAST)
#error ETHIF_VAR_FAST already defined
#endif
#define ETHIF_VAR_FAST

#if (defined ETHIF_VAR)
#error ETHIF_VAR already defined
#endif
#define ETHIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHSM_CODE)
#error ETHSM_CODE already defined
#endif
#define ETHSM_CODE

#if (defined ETHSM_CONST)
#error ETHSM_CONST already defined
#endif
#define ETHSM_CONST

#if (defined ETHSM_APPL_DATA)
#error ETHSM_APPL_DATA already defined
#endif
#define ETHSM_APPL_DATA

#if (defined ETHSM_APPL_CONST)
#error ETHSM_APPL_CONST already defined
#endif
#define ETHSM_APPL_CONST

#if (defined ETHSM_APPL_CODE)
#error ETHSM_APPL_CODE already defined
#endif
#define ETHSM_APPL_CODE

#if (defined ETHSM_VAR_NOINIT)
#error ETHSM_VAR_NOINIT already defined
#endif
#define ETHSM_VAR_NOINIT

#if (defined ETHSM_VAR_CLEARED)
#error ETHSM_VAR_CLEARED already defined
#endif
#define ETHSM_VAR_CLEARED

#if (defined ETHSM_VAR_POWER_ON_INIT)
#error ETHSM_VAR_POWER_ON_INIT already defined
#endif
#define ETHSM_VAR_POWER_ON_INIT

#if (defined ETHSM_VAR_FAST)
#error ETHSM_VAR_FAST already defined
#endif
#define ETHSM_VAR_FAST

#if (defined ETHSM_VAR)
#error ETHSM_VAR already defined
#endif
#define ETHSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHTRCV_CODE)
#error ETHTRCV_CODE already defined
#endif
#define ETHTRCV_CODE

#if (defined ETHTRCV_CONST)
#error ETHTRCV_CONST already defined
#endif
#define ETHTRCV_CONST

#if (defined ETHTRCV_APPL_DATA)
#error ETHTRCV_APPL_DATA already defined
#endif
#define ETHTRCV_APPL_DATA

#if (defined ETHTRCV_APPL_CONST)
#error ETHTRCV_APPL_CONST already defined
#endif
#define ETHTRCV_APPL_CONST

#if (defined ETHTRCV_APPL_CODE)
#error ETHTRCV_APPL_CODE already defined
#endif
#define ETHTRCV_APPL_CODE

#if (defined ETHTRCV_VAR_NOINIT)
#error ETHTRCV_VAR_NOINIT already defined
#endif
#define ETHTRCV_VAR_NOINIT

#if (defined ETHTRCV_VAR_CLEARED)
#error ETHTRCV_VAR_CLEARED already defined
#endif
#define ETHTRCV_VAR_CLEARED

#if (defined ETHTRCV_VAR_POWER_ON_INIT)
#error ETHTRCV_VAR_POWER_ON_INIT already defined
#endif
#define ETHTRCV_VAR_POWER_ON_INIT

#if (defined ETHTRCV_VAR_FAST)
#error ETHTRCV_VAR_FAST already defined
#endif
#define ETHTRCV_VAR_FAST

#if (defined ETHTRCV_VAR)
#error ETHTRCV_VAR already defined
#endif
#define ETHTRCV_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHSWT_CODE)
#error ETHSWT_CODE already defined
#endif
#define ETHSWT_CODE

#if (defined ETHSWT_CONST)
#error ETHSWT_CONST already defined
#endif
#define ETHSWT_CONST

#if (defined ETHSWT_APPL_DATA)
#error ETHSWT_APPL_DATA already defined
#endif
#define ETHSWT_APPL_DATA

#if (defined ETHSWT_APPL_CONST)
#error ETHSWT_APPL_CONST already defined
#endif
#define ETHSWT_APPL_CONST

#if (defined ETHSWT_APPL_CODE)
#error ETHSWT_APPL_CODE already defined
#endif
#define ETHSWT_APPL_CODE

#if (defined ETHSWT_VAR_NOINIT)
#error ETHSWT_VAR_NOINIT already defined
#endif
#define ETHSWT_VAR_NOINIT

#if (defined ETHSWT_VAR_CLEARED)
#error ETHSWT_VAR_CLEARED already defined
#endif
#define ETHSWT_VAR_CLEARED

#if (defined ETHSWT_VAR_POWER_ON_INIT)
#error ETHSWT_VAR_POWER_ON_INIT already defined
#endif
#define ETHSWT_VAR_POWER_ON_INIT

#if (defined ETHSWT_VAR_FAST)
#error ETHSWT_VAR_FAST already defined
#endif
#define ETHSWT_VAR_FAST

#if (defined ETHSWT_VAR)
#error ETHSWT_VAR already defined
#endif
#define ETHSWT_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHSWT_1_BCM_CODE)
#error ETHSWT_1_BCM_CODE already defined
#endif
#define ETHSWT_1_BCM_CODE

#if (defined ETHSWT_1_BCM_CONST)
#error ETHSWT_1_BCM_CONST already defined
#endif
#define ETHSWT_1_BCM_CONST

#if (defined ETHSWT_1_BCM_APPL_DATA)
#error ETHSWT_1_BCM_APPL_DATA already defined
#endif
#define ETHSWT_1_BCM_APPL_DATA

#if (defined ETHSWT_1_BCM_APPL_CONST)
#error ETHSWT_1_BCM_APPL_CONST already defined
#endif
#define ETHSWT_1_BCM_APPL_CONST

#if (defined ETHSWT_1_BCM_APPL_CODE)
#error ETHSWT_1_BCM_APPL_CODE already defined
#endif
#define ETHSWT_1_BCM_APPL_CODE

#if (defined ETHSWT_1_BCM_VAR_NOINIT)
#error ETHSWT_1_BCM_VAR_NOINIT already defined
#endif
#define ETHSWT_1_BCM_VAR_NOINIT

#if (defined ETHSWT_1_BCM_VAR_CLEARED)
#error ETHSWT_1_BCM_VAR_CLEARED already defined
#endif
#define ETHSWT_1_BCM_VAR_CLEARED

#if (defined ETHSWT_1_BCM_VAR_POWER_ON_INIT)
#error ETHSWT_1_BCM_VAR_POWER_ON_INIT already defined
#endif
#define ETHSWT_1_BCM_VAR_POWER_ON_INIT

#if (defined ETHSWT_1_BCM_VAR_FAST)
#error ETHSWT_1_BCM_VAR_FAST already defined
#endif
#define ETHSWT_1_BCM_VAR_FAST

#if (defined ETHSWT_1_BCM_VAR)
#error ETHSWT_1_BCM_VAR already defined
#endif
#define ETHSWT_1_BCM_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHSWT_1_MARVELL_CODE)
#error ETHSWT_1_MARVELL_CODE already defined
#endif
#define ETHSWT_1_MARVELL_CODE

#if (defined ETHSWT_1_MARVELL_CONST)
#error ETHSWT_1_MARVELL_CONST already defined
#endif
#define ETHSWT_1_MARVELL_CONST

#if (defined ETHSWT_1_MARVELL_APPL_DATA)
#error ETHSWT_1_MARVELL_APPL_DATA already defined
#endif
#define ETHSWT_1_MARVELL_APPL_DATA

#if (defined ETHSWT_1_MARVELL_APPL_CONST)
#error ETHSWT_1_MARVELL_APPL_CONST already defined
#endif
#define ETHSWT_1_MARVELL_APPL_CONST

#if (defined ETHSWT_1_MARVELL_APPL_CODE)
#error ETHSWT_1_MARVELL_APPL_CODE already defined
#endif
#define ETHSWT_1_MARVELL_APPL_CODE

#if (defined ETHSWT_1_MARVELL_VAR_NOINIT)
#error ETHSWT_1_MARVELL_VAR_NOINIT already defined
#endif
#define ETHSWT_1_MARVELL_VAR_NOINIT

#if (defined ETHSWT_1_MARVELL_VAR_CLEARED)
#error ETHSWT_1_MARVELL_VAR_CLEARED already defined
#endif
#define ETHSWT_1_MARVELL_VAR_CLEARED

#if (defined ETHSWT_1_MARVELL_VAR_POWER_ON_INIT)
#error ETHSWT_1_MARVELL_VAR_POWER_ON_INIT already defined
#endif
#define ETHSWT_1_MARVELL_VAR_POWER_ON_INIT

#if (defined ETHSWT_1_MARVELL_VAR_FAST)
#error ETHSWT_1_MARVELL_VAR_FAST already defined
#endif
#define ETHSWT_1_MARVELL_VAR_FAST

#if (defined ETHSWT_1_MARVELL_VAR)
#error ETHSWT_1_MARVELL_VAR already defined
#endif
#define ETHSWT_1_MARVELL_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHSWT_1_NXP_CODE)
#error ETHSWT_1_NXP_CODE already defined
#endif
#define ETHSWT_1_NXP_CODE

#if (defined ETHSWT_1_NXP_CONST)
#error ETHSWT_1_NXP_CONST already defined
#endif
#define ETHSWT_1_NXP_CONST

#if (defined ETHSWT_1_NXP_APPL_DATA)
#error ETHSWT_1_NXP_APPL_DATA already defined
#endif
#define ETHSWT_1_NXP_APPL_DATA

#if (defined ETHSWT_1_NXP_APPL_CONST)
#error ETHSWT_1_NXP_APPL_CONST already defined
#endif
#define ETHSWT_1_NXP_APPL_CONST

#if (defined ETHSWT_1_NXP_APPL_CODE)
#error ETHSWT_1_NXP_APPL_CODE already defined
#endif
#define ETHSWT_1_NXP_APPL_CODE

#if (defined ETHSWT_1_NXP_VAR_NOINIT)
#error ETHSWT_1_NXP_VAR_NOINIT already defined
#endif
#define ETHSWT_1_NXP_VAR_NOINIT

#if (defined ETHSWT_1_NXP_VAR_CLEARED)
#error ETHSWT_1_NXP_VAR_CLEARED already defined
#endif
#define ETHSWT_1_NXP_VAR_CLEARED

#if (defined ETHSWT_1_NXP_VAR_POWER_ON_INIT)
#error ETHSWT_1_NXP_VAR_POWER_ON_INIT already defined
#endif
#define ETHSWT_1_NXP_VAR_POWER_ON_INIT

#if (defined ETHSWT_1_NXP_VAR_FAST)
#error ETHSWT_1_NXP_VAR_FAST already defined
#endif
#define ETHSWT_1_NXP_VAR_FAST

#if (defined ETHSWT_1_NXP_VAR)
#error ETHSWT_1_NXP_VAR already defined
#endif
#define ETHSWT_1_NXP_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHTRCV_1_T01_CODE)
#error ETHTRCV_1_T01_CODE already defined
#endif
#define ETHTRCV_1_T01_CODE

#if (defined ETHTRCV_1_T01_CONST)
#error ETHTRCV_1_T01_CONST already defined
#endif
#define ETHTRCV_1_T01_CONST

#if (defined ETHTRCV_1_T01_APPL_DATA)
#error ETHTRCV_1_T01_APPL_DATA already defined
#endif
#define ETHTRCV_1_T01_APPL_DATA

#if (defined ETHTRCV_1_T01_APPL_CONST)
#error ETHTRCV_1_T01_APPL_CONST already defined
#endif
#define ETHTRCV_1_T01_APPL_CONST

#if (defined ETHTRCV_1_T01_APPL_CODE)
#error ETHTRCV_1_T01_APPL_CODE already defined
#endif
#define ETHTRCV_1_T01_APPL_CODE

#if (defined ETHTRCV_1_T01_VAR_NOINIT)
#error ETHTRCV_1_T01_VAR_NOINIT already defined
#endif
#define ETHTRCV_1_T01_VAR_NOINIT

#if (defined ETHTRCV_1_T01_VAR_CLEARED)
#error ETHTRCV_1_T01_VAR_CLEARED already defined
#endif
#define ETHTRCV_1_T01_VAR_CLEARED

#if (defined ETHTRCV_1_T01_VAR_POWER_ON_INIT)
#error ETHTRCV_1_T01_VAR_POWER_ON_INIT already defined
#endif
#define ETHTRCV_1_T01_VAR_POWER_ON_INIT

#if (defined ETHTRCV_1_T01_VAR_FAST)
#error ETHTRCV_1_T01_VAR_FAST already defined
#endif
#define ETHTRCV_1_T01_VAR_FAST

#if (defined ETHTRCV_1_T01_VAR)
#error ETHTRCV_1_T01_VAR already defined
#endif
#define ETHTRCV_1_T01_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ETHTSYN_CODE)
#error ETHTSYN_CODE already defined
#endif
#define ETHTSYN_CODE

#if (defined ETHTSYN_CONST)
#error ETHTSYN_CONST already defined
#endif
#define ETHTSYN_CONST

#if (defined ETHTSYN_APPL_DATA)
#error ETHTSYN_APPL_DATA already defined
#endif
#define ETHTSYN_APPL_DATA

#if (defined ETHTSYN_APPL_CONST)
#error ETHTSYN_APPL_CONST already defined
#endif
#define ETHTSYN_APPL_CONST

#if (defined ETHTSYN_APPL_CODE)
#error ETHTSYN_APPL_CODE already defined
#endif
#define ETHTSYN_APPL_CODE

#if (defined ETHTSYN_VAR_NOINIT)
#error ETHTSYN_VAR_NOINIT already defined
#endif
#define ETHTSYN_VAR_NOINIT

#if (defined ETHTSYN_VAR_CLEARED)
#error ETHTSYN_VAR_CLEARED already defined
#endif
#define ETHTSYN_VAR_CLEARED

#if (defined ETHTSYN_VAR_POWER_ON_INIT)
#error ETHTSYN_VAR_POWER_ON_INIT already defined
#endif
#define ETHTSYN_VAR_POWER_ON_INIT

#if (defined ETHTSYN_VAR_FAST)
#error ETHTSYN_VAR_FAST already defined
#endif
#define ETHTSYN_VAR_FAST

#if (defined ETHTSYN_VAR)
#error ETHTSYN_VAR already defined
#endif
#define ETHTSYN_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FEE_CODE)
#error FEE_CODE already defined
#endif
#define FEE_CODE

#if (defined FEE_CONST)
#error FEE_CONST already defined
#endif
#define FEE_CONST

#if (defined FEE_APPL_DATA)
#error FEE_APPL_DATA already defined
#endif
#define FEE_APPL_DATA

#if (defined FEE_APPL_CONST)
#error FEE_APPL_CONST already defined
#endif
#define FEE_APPL_CONST

#if (defined FEE_APPL_CODE)
#error FEE_APPL_CODE already defined
#endif
#define FEE_APPL_CODE

#if (defined FEE_VAR_NOINIT)
#error FEE_VAR_NOINIT already defined
#endif
#define FEE_VAR_NOINIT

#if (defined FEE_VAR_CLEARED)
#error FEE_VAR_CLEARED already defined
#endif
#define FEE_VAR_CLEARED

#if (defined FEE_VAR_POWER_ON_INIT)
#error FEE_VAR_POWER_ON_INIT already defined
#endif
#define FEE_VAR_POWER_ON_INIT

#if (defined FEE_VAR_FAST)
#error FEE_VAR_FAST already defined
#endif
#define FEE_VAR_FAST

#if (defined FEE_VAR)
#error FEE_VAR already defined
#endif
#define FEE_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FIM_CODE)
#error FIM_CODE already defined
#endif
#define FIM_CODE

#if (defined FIM_CONST)
#error FIM_CONST already defined
#endif
#define FIM_CONST

#if (defined FIM_APPL_DATA)
#error FIM_APPL_DATA already defined
#endif
#define FIM_APPL_DATA

#if (defined FIM_APPL_CONST)
#error FIM_APPL_CONST already defined
#endif
#define FIM_APPL_CONST

#if (defined FIM_APPL_CODE)
#error FIM_APPL_CODE already defined
#endif
#define FIM_APPL_CODE

#if (defined FIM_VAR_NOINIT)
#error FIM_VAR_NOINIT already defined
#endif
#define FIM_VAR_NOINIT

#if (defined FIM_VAR_CLEARED)
#error FIM_VAR_CLEARED already defined
#endif
#define FIM_VAR_CLEARED

#if (defined FIM_VAR_POWER_ON_INIT)
#error FIM_VAR_POWER_ON_INIT already defined
#endif
#define FIM_VAR_POWER_ON_INIT

#if (defined FIM_VAR_POWER_ON_CLEARED)
#error FIM_VAR_POWER_ON_CLEARED already defined
#endif
#define FIM_VAR_POWER_ON_CLEARED

#if (defined FIM_VAR_FAST)
#error FIM_VAR_FAST already defined
#endif
#define FIM_VAR_FAST

#if (defined FIM_VAR)
#error FIM_VAR already defined
#endif
#define FIM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FLS_CODE)
#error FLS_CODE already defined
#endif
#define FLS_CODE

#if (defined FLS_CONST)
#error FLS_CONST already defined
#endif
#define FLS_CONST

#if (defined FLS_APPL_DATA)
#error FLS_APPL_DATA already defined
#endif
#define FLS_APPL_DATA

#if (defined FLS_APPL_CONST)
#error FLS_APPL_CONST already defined
#endif
#define FLS_APPL_CONST

#if (defined FLS_APPL_CODE)
#error FLS_APPL_CODE already defined
#endif
#define FLS_APPL_CODE

#if (defined FLS_VAR_NOINIT)
#error FLS_VAR_NOINIT already defined
#endif
#define FLS_VAR_NOINIT

#if (defined FLS_VAR_CLEARED)
#error FLS_VAR_CLEARED already defined
#endif
#define FLS_VAR_CLEARED

#if (defined FLS_VAR_POWER_ON_INIT)
#error FLS_VAR_POWER_ON_INIT already defined
#endif
#define FLS_VAR_POWER_ON_INIT

#if (defined FLS_VAR_FAST)
#error FLS_VAR_FAST already defined
#endif
#define FLS_VAR_FAST

#if (defined FLS_VAR)
#error FLS_VAR already defined
#endif
#define FLS_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FLSIF_CODE)
#error FLSIF_CODE already defined
#endif
#define FLSIF_CODE

#if (defined FLSIF_CONST)
#error FLSIF_CONST already defined
#endif
#define FLSIF_CONST

#if (defined FLSIF_APPL_DATA)
#error FLSIF_APPL_DATA already defined
#endif
#define FLSIF_APPL_DATA

#if (defined FLSIF_APPL_CONST)
#error FLSIF_APPL_CONST already defined
#endif
#define FLSIF_APPL_CONST

#if (defined FLSIF_APPL_CODE)
#error FLSIF_APPL_CODE already defined
#endif
#define FLSIF_APPL_CODE

#if (defined FLSIF_VAR_NOINIT)
#error FLSIF_VAR_NOINIT already defined
#endif
#define FLSIF_VAR_NOINIT

#if (defined FLSIF_VAR_CLEARED)
#error FLSIF_VAR_CLEARED already defined
#endif
#define FLSIF_VAR_CLEARED

#if (defined FLSIF_VAR_POWER_ON_INIT)
#error FLSIF_VAR_POWER_ON_INIT already defined
#endif
#define FLSIF_VAR_POWER_ON_INIT

#if (defined FLSIF_VAR_FAST)
#error FLSIF_VAR_FAST already defined
#endif
#define FLSIF_VAR_FAST

#if (defined FLSIF_VAR)
#error FLSIF_VAR already defined
#endif
#define FLSIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FR_1_A_CODE)
#error FR_1_A_CODE already defined
#endif
#define FR_1_A_CODE

#if (defined FR_1_A_CONST)
#error FR_1_A_CONST already defined
#endif
#define FR_1_A_CONST

#if (defined FR_1_A_APPL_DATA)
#error FR_1_A_APPL_DATA already defined
#endif
#define FR_1_A_APPL_DATA

#if (defined FR_1_A_APPL_CONST)
#error FR_1_A_APPL_CONST already defined
#endif
#define FR_1_A_APPL_CONST

#if (defined FR_1_A_APPL_CODE)
#error FR_1_A_APPL_CODE already defined
#endif
#define FR_1_A_APPL_CODE

#if (defined FR_1_A_VAR_NOINIT)
#error FR_1_A_VAR_NOINIT already defined
#endif
#define FR_1_A_VAR_NOINIT

#if (defined FR_1_A_VAR_CLEARED)
#error FR_1_A_VAR_CLEARED already defined
#endif
#define FR_1_A_VAR_CLEARED

#if (defined FR_1_A_VAR_POWER_ON_INIT)
#error FR_1_A_VAR_POWER_ON_INIT already defined
#endif
#define FR_1_A_VAR_POWER_ON_INIT

#if (defined FR_1_A_VAR_FAST)
#error FR_1_A_VAR_FAST already defined
#endif
#define FR_1_A_VAR_FAST

#if (defined FR_1_A_VAR)
#error FR_1_A_VAR already defined
#endif
#define FR_1_A_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FR_CODE)
#error FR_CODE already defined
#endif
#define FR_CODE

#if (defined FR_CONST)
#error FR_CONST already defined
#endif
#define FR_CONST

#if (defined FR_APPL_DATA)
#error FR_APPL_DATA already defined
#endif
#define FR_APPL_DATA

#if (defined FR_APPL_CONST)
#error FR_APPL_CONST already defined
#endif
#define FR_APPL_CONST

#if (defined FR_APPL_CODE)
#error FR_APPL_CODE already defined
#endif
#define FR_APPL_CODE

#if (defined FR_VAR_NOINIT)
#error FR_VAR_NOINIT already defined
#endif
#define FR_VAR_NOINIT

#if (defined FR_VAR_CLEARED)
#error FR_VAR_CLEARED already defined
#endif
#define FR_VAR_CLEARED

#if (defined FR_VAR_POWER_ON_INIT)
#error FR_VAR_POWER_ON_INIT already defined
#endif
#define FR_VAR_POWER_ON_INIT

#if (defined FR_VAR_FAST)
#error FR_VAR_FAST already defined
#endif
#define FR_VAR_FAST

#if (defined FR_VAR)
#error FR_VAR already defined
#endif
#define FR_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRARTP_CODE)
#error FRARTP_CODE already defined
#endif
#define FRARTP_CODE

#if (defined FRARTP_CONST)
#error FRARTP_CONST already defined
#endif
#define FRARTP_CONST

#if (defined FRARTP_APPL_DATA)
#error FRARTP_APPL_DATA already defined
#endif
#define FRARTP_APPL_DATA

#if (defined FRARTP_APPL_CONST)
#error FRARTP_APPL_CONST already defined
#endif
#define FRARTP_APPL_CONST

#if (defined FRARTP_APPL_CODE)
#error FRARTP_APPL_CODE already defined
#endif
#define FRARTP_APPL_CODE

#if (defined FRARTP_VAR_NOINIT)
#error FRARTP_VAR_NOINIT already defined
#endif
#define FRARTP_VAR_NOINIT

#if (defined FRARTP_VAR_CLEARED)
#error FRARTP_VAR_CLEARED already defined
#endif
#define FRARTP_VAR_CLEARED

#if (defined FRARTP_VAR_POWER_ON_INIT)
#error FRARTP_VAR_POWER_ON_INIT already defined
#endif
#define FRARTP_VAR_POWER_ON_INIT

#if (defined FRARTP_VAR_FAST)
#error FRARTP_VAR_FAST already defined
#endif
#define FRARTP_VAR_FAST

#if (defined FRARTP_VAR)
#error FRARTP_VAR already defined
#endif
#define FRARTP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRIF_CODE)
#error FRIF_CODE already defined
#endif
#define FRIF_CODE

#if (defined FRIF_CONST)
#error FRIF_CONST already defined
#endif
#define FRIF_CONST

#if (defined FRIF_APPL_DATA)
#error FRIF_APPL_DATA already defined
#endif
#define FRIF_APPL_DATA

#if (defined FRIF_APPL_CONST)
#error FRIF_APPL_CONST already defined
#endif
#define FRIF_APPL_CONST

#if (defined FRIF_APPL_CODE)
#error FRIF_APPL_CODE already defined
#endif
#define FRIF_APPL_CODE

#if (defined FRIF_VAR_NOINIT)
#error FRIF_VAR_NOINIT already defined
#endif
#define FRIF_VAR_NOINIT

#if (defined FRIF_VAR_CLEARED)
#error FRIF_VAR_CLEARED already defined
#endif
#define FRIF_VAR_CLEARED

#if (defined FRIF_VAR_POWER_ON_INIT)
#error FRIF_VAR_POWER_ON_INIT already defined
#endif
#define FRIF_VAR_POWER_ON_INIT

#if (defined FRIF_VAR_FAST)
#error FRIF_VAR_FAST already defined
#endif
#define FRIF_VAR_FAST

#if (defined FRIF_VAR)
#error FRIF_VAR already defined
#endif
#define FRIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRNM_CODE)
#error FRNM_CODE already defined
#endif
#define FRNM_CODE

#if (defined FRNM_CONST)
#error FRNM_CONST already defined
#endif
#define FRNM_CONST

#if (defined FRNM_APPL_DATA)
#error FRNM_APPL_DATA already defined
#endif
#define FRNM_APPL_DATA

#if (defined FRNM_APPL_CONST)
#error FRNM_APPL_CONST already defined
#endif
#define FRNM_APPL_CONST

#if (defined FRNM_APPL_CODE)
#error FRNM_APPL_CODE already defined
#endif
#define FRNM_APPL_CODE

#if (defined FRNM_VAR_NOINIT)
#error FRNM_VAR_NOINIT already defined
#endif
#define FRNM_VAR_NOINIT

#if (defined FRNM_VAR_CLEARED)
#error FRNM_VAR_CLEARED already defined
#endif
#define FRNM_VAR_CLEARED

#if (defined FRNM_VAR_POWER_ON_INIT)
#error FRNM_VAR_POWER_ON_INIT already defined
#endif
#define FRNM_VAR_POWER_ON_INIT

#if (defined FRNM_VAR_FAST)
#error FRNM_VAR_FAST already defined
#endif
#define FRNM_VAR_FAST

#if (defined FRNM_VAR)
#error FRNM_VAR already defined
#endif
#define FRNM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRSM_CODE)
#error FRSM_CODE already defined
#endif
#define FRSM_CODE

#if (defined FRSM_CONST)
#error FRSM_CONST already defined
#endif
#define FRSM_CONST

#if (defined FRSM_APPL_DATA)
#error FRSM_APPL_DATA already defined
#endif
#define FRSM_APPL_DATA

#if (defined FRSM_APPL_CONST)
#error FRSM_APPL_CONST already defined
#endif
#define FRSM_APPL_CONST

#if (defined FRSM_APPL_CODE)
#error FRSM_APPL_CODE already defined
#endif
#define FRSM_APPL_CODE

#if (defined FRSM_VAR_NOINIT)
#error FRSM_VAR_NOINIT already defined
#endif
#define FRSM_VAR_NOINIT

#if (defined FRSM_VAR_CLEARED)
#error FRSM_VAR_CLEARED already defined
#endif
#define FRSM_VAR_CLEARED

#if (defined FRSM_VAR_POWER_ON_INIT)
#error FRSM_VAR_POWER_ON_INIT already defined
#endif
#define FRSM_VAR_POWER_ON_INIT

#if (defined FRSM_VAR_FAST)
#error FRSM_VAR_FAST already defined
#endif
#define FRSM_VAR_FAST

#if (defined FRSM_VAR)
#error FRSM_VAR already defined
#endif
#define FRSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRTP_CODE)
#error FRTP_CODE already defined
#endif
#define FRTP_CODE

#if (defined FRTP_CONST)
#error FRTP_CONST already defined
#endif
#define FRTP_CONST

#if (defined FRTP_APPL_DATA)
#error FRTP_APPL_DATA already defined
#endif
#define FRTP_APPL_DATA

#if (defined FRTP_APPL_CONST)
#error FRTP_APPL_CONST already defined
#endif
#define FRTP_APPL_CONST

#if (defined FRTP_APPL_CODE)
#error FRTP_APPL_CODE already defined
#endif
#define FRTP_APPL_CODE

#if (defined FRTP_VAR_NOINIT)
#error FRTP_VAR_NOINIT already defined
#endif
#define FRTP_VAR_NOINIT

#if (defined FRTP_VAR_CLEARED)
#error FRTP_VAR_CLEARED already defined
#endif
#define FRTP_VAR_CLEARED

#if (defined FRTP_VAR_POWER_ON_INIT)
#error FRTP_VAR_POWER_ON_INIT already defined
#endif
#define FRTP_VAR_POWER_ON_INIT

#if (defined FRTP_VAR_FAST)
#error FRTP_VAR_FAST already defined
#endif
#define FRTP_VAR_FAST

#if (defined FRTP_VAR)
#error FRTP_VAR already defined
#endif
#define FRTP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRTRCV_CODE)
#error FRTRCV_CODE already defined
#endif
#define FRTRCV_CODE

#if (defined FRTRCV_CONST)
#error FRTRCV_CONST already defined
#endif
#define FRTRCV_CONST

#if (defined FRTRCV_APPL_DATA)
#error FRTRCV_APPL_DATA already defined
#endif
#define FRTRCV_APPL_DATA

#if (defined FRTRCV_APPL_CONST)
#error FRTRCV_APPL_CONST already defined
#endif
#define FRTRCV_APPL_CONST

#if (defined FRTRCV_APPL_CODE)
#error FRTRCV_APPL_CODE already defined
#endif
#define FRTRCV_APPL_CODE

#if (defined FRTRCV_VAR_NOINIT)
#error FRTRCV_VAR_NOINIT already defined
#endif
#define FRTRCV_VAR_NOINIT

#if (defined FRTRCV_VAR_CLEARED)
#error FRTRCV_VAR_CLEARED already defined
#endif
#define FRTRCV_VAR_CLEARED

#if (defined FRTRCV_VAR_POWER_ON_INIT)
#error FRTRCV_VAR_POWER_ON_INIT already defined
#endif
#define FRTRCV_VAR_POWER_ON_INIT

#if (defined FRTRCV_VAR_FAST)
#error FRTRCV_VAR_FAST already defined
#endif
#define FRTRCV_VAR_FAST

#if (defined FRTRCV_VAR)
#error FRTRCV_VAR already defined
#endif
#define FRTRCV_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRTRCV_1_T01_CODE)
#error FRTRCV_1_T01_CODE already defined
#endif
#define FRTRCV_1_T01_CODE

#if (defined FRTRCV_1_T01_CONST)
#error FRTRCV_1_T01_CONST already defined
#endif
#define FRTRCV_1_T01_CONST

#if (defined FRTRCV_1_T01_APPL_DATA)
#error FRTRCV_1_T01_APPL_DATA already defined
#endif
#define FRTRCV_1_T01_APPL_DATA

#if (defined FRTRCV_1_T01_APPL_CONST)
#error FRTRCV_1_T01_APPL_CONST already defined
#endif
#define FRTRCV_1_T01_APPL_CONST

#if (defined FRTRCV_1_T01_APPL_CODE)
#error FRTRCV_1_T01_APPL_CODE already defined
#endif
#define FRTRCV_1_T01_APPL_CODE

#if (defined FRTRCV_1_T01_VAR_NOINIT)
#error FRTRCV_1_T01_VAR_NOINIT already defined
#endif
#define FRTRCV_1_T01_VAR_NOINIT

#if (defined FRTRCV_1_T01_VAR_CLEARED)
#error FRTRCV_1_T01_VAR_CLEARED already defined
#endif
#define FRTRCV_1_T01_VAR_CLEARED

#if (defined FRTRCV_1_T01_VAR_POWER_ON_INIT)
#error FRTRCV_1_T01_VAR_POWER_ON_INIT already defined
#endif
#define FRTRCV_1_T01_VAR_POWER_ON_INIT

#if (defined FRTRCV_1_T01_VAR_FAST)
#error FRTRCV_1_T01_VAR_FAST already defined
#endif
#define FRTRCV_1_T01_VAR_FAST

#if (defined FRTRCV_1_T01_VAR)
#error FRTRCV_1_T01_VAR already defined
#endif
#define FRTRCV_1_T01_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRTRCV_1_TJA1080_CODE)
#error FRTRCV_1_TJA1080_CODE already defined
#endif
#define FRTRCV_1_TJA1080_CODE

#if (defined FRTRCV_1_TJA1080_CONST)
#error FRTRCV_1_TJA1080_CONST already defined
#endif
#define FRTRCV_1_TJA1080_CONST

#if (defined FRTRCV_1_TJA1080_APPL_DATA)
#error FRTRCV_1_TJA1080_APPL_DATA already defined
#endif
#define FRTRCV_1_TJA1080_APPL_DATA

#if (defined FRTRCV_1_TJA1080_APPL_CONST)
#error FRTRCV_1_TJA1080_APPL_CONST already defined
#endif
#define FRTRCV_1_TJA1080_APPL_CONST

#if (defined FRTRCV_1_TJA1080_APPL_CODE)
#error FRTRCV_1_TJA1080_APPL_CODE already defined
#endif
#define FRTRCV_1_TJA1080_APPL_CODE

#if (defined FRTRCV_1_TJA1080_VAR_NOINIT)
#error FRTRCV_1_TJA1080_VAR_NOINIT already defined
#endif
#define FRTRCV_1_TJA1080_VAR_NOINIT

#if (defined FRTRCV_1_TJA1080_VAR_CLEARED)
#error FRTRCV_1_TJA1080_VAR_CLEARED already defined
#endif
#define FRTRCV_1_TJA1080_VAR_CLEARED

#if (defined FRTRCV_1_TJA1080_VAR_POWER_ON_INIT)
#error FRTRCV_1_TJA1080_VAR_POWER_ON_INIT already defined
#endif
#define FRTRCV_1_TJA1080_VAR_POWER_ON_INIT

#if (defined FRTRCV_1_TJA1080_VAR_FAST)
#error FRTRCV_1_TJA1080_VAR_FAST already defined
#endif
#define FRTRCV_1_TJA1080_VAR_FAST

#if (defined FRTRCV_1_TJA1080_VAR)
#error FRTRCV_1_TJA1080_VAR already defined
#endif
#define FRTRCV_1_TJA1080_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FRTSYN_CODE)
#error FRTSYN_CODE already defined
#endif
#define FRTSYN_CODE

#if (defined FRTSYN_CONST)
#error FRTSYN_CONST already defined
#endif
#define FRTSYN_CONST

#if (defined FRTSYN_APPL_DATA)
#error FRTSYN_APPL_DATA already defined
#endif
#define FRTSYN_APPL_DATA

#if (defined FRTSYN_APPL_CONST)
#error FRTSYN_APPL_CONST already defined
#endif
#define FRTSYN_APPL_CONST

#if (defined FRTSYN_APPL_CODE)
#error FRTSYN_APPL_CODE already defined
#endif
#define FRTSYN_APPL_CODE

#if (defined FRTSYN_VAR_NOINIT)
#error FRTSYN_VAR_NOINIT already defined
#endif
#define FRTSYN_VAR_NOINIT

#if (defined FRTSYN_VAR_CLEARED)
#error FRTSYN_VAR_CLEARED already defined
#endif
#define FRTSYN_VAR_CLEARED

#if (defined FRTSYN_VAR_POWER_ON_INIT)
#error FRTSYN_VAR_POWER_ON_INIT already defined
#endif
#define FRTSYN_VAR_POWER_ON_INIT

#if (defined FRTSYN_VAR_FAST)
#error FRTSYN_VAR_FAST already defined
#endif
#define FRTSYN_VAR_FAST

#if (defined FRTSYN_VAR)
#error FRTSYN_VAR already defined
#endif
#define FRTSYN_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FR_1_ERAY_CODE)
#error FR_1_ERAY_CODE already defined
#endif
#define FR_1_ERAY_CODE

#if (defined FR_1_ERAY_CONST)
#error FR_1_ERAY_CONST already defined
#endif
#define FR_1_ERAY_CONST

#if (defined FR_1_ERAY_APPL_DATA)
#error FR_1_ERAY_APPL_DATA already defined
#endif
#define FR_1_ERAY_APPL_DATA

#if (defined FR_1_ERAY_APPL_CONST)
#error FR_1_ERAY_APPL_CONST already defined
#endif
#define FR_1_ERAY_APPL_CONST

#if (defined FR_1_ERAY_APPL_CODE)
#error FR_1_ERAY_APPL_CODE already defined
#endif
#define FR_1_ERAY_APPL_CODE

#if (defined FR_1_ERAY_VAR_NOINIT)
#error FR_1_ERAY_VAR_NOINIT already defined
#endif
#define FR_1_ERAY_VAR_NOINIT

#if (defined FR_1_ERAY_VAR_CLEARED)
#error FR_1_ERAY_VAR_CLEARED already defined
#endif
#define FR_1_ERAY_VAR_CLEARED

#if (defined FR_1_ERAY_VAR_POWER_ON_INIT)
#error FR_1_ERAY_VAR_POWER_ON_INIT already defined
#endif
#define FR_1_ERAY_VAR_POWER_ON_INIT

#if (defined FR_1_ERAY_VAR_FAST)
#error FR_1_ERAY_VAR_FAST already defined
#endif
#define FR_1_ERAY_VAR_FAST

#if (defined FR_1_ERAY_VAR)
#error FR_1_ERAY_VAR already defined
#endif
#define FR_1_ERAY_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined FR_1_MFR4300_CODE)
#error FR_1_MFR4300_CODE already defined
#endif
#define FR_1_MFR4300_CODE

#if (defined FR_1_MFR4300_CONST)
#error FR_1_MFR4300_CONST already defined
#endif
#define FR_1_MFR4300_CONST

#if (defined FR_1_MFR4300_APPL_DATA)
#error FR_1_MFR4300_APPL_DATA already defined
#endif
#define FR_1_MFR4300_APPL_DATA

#if (defined FR_1_MFR4300_APPL_CONST)
#error FR_1_MFR4300_APPL_CONST already defined
#endif
#define FR_1_MFR4300_APPL_CONST

#if (defined FR_1_MFR4300_APPL_CODE)
#error FR_1_MFR4300_APPL_CODE already defined
#endif
#define FR_1_MFR4300_APPL_CODE

#if (defined FR_1_MFR4300_VAR_NOINIT)
#error FR_1_MFR4300_VAR_NOINIT already defined
#endif
#define FR_1_MFR4300_VAR_NOINIT

#if (defined FR_1_MFR4300_VAR_CLEARED)
#error FR_1_MFR4300_VAR_CLEARED already defined
#endif
#define FR_1_MFR4300_VAR_CLEARED

#if (defined FR_1_MFR4300_VAR_POWER_ON_INIT)
#error FR_1_MFR4300_VAR_POWER_ON_INIT already defined
#endif
#define FR_1_MFR4300_VAR_POWER_ON_INIT

#if (defined FR_1_MFR4300_VAR_FAST)
#error FR_1_MFR4300_VAR_FAST already defined
#endif
#define FR_1_MFR4300_VAR_FAST

#if (defined FR_1_MFR4300_VAR)
#error FR_1_MFR4300_VAR already defined
#endif
#define FR_1_MFR4300_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined GPT_CODE)
#error GPT_CODE already defined
#endif
#define GPT_CODE

#if (defined GPT_CONST)
#error GPT_CONST already defined
#endif
#define GPT_CONST

#if (defined GPT_APPL_DATA)
#error GPT_APPL_DATA already defined
#endif
#define GPT_APPL_DATA

#if (defined GPT_APPL_CONST)
#error GPT_APPL_CONST already defined
#endif
#define GPT_APPL_CONST

#if (defined GPT_APPL_CODE)
#error GPT_APPL_CODE already defined
#endif
#define GPT_APPL_CODE

#if (defined GPT_VAR_NOINIT)
#error GPT_VAR_NOINIT already defined
#endif
#define GPT_VAR_NOINIT

#if (defined GPT_VAR_CLEARED)
#error GPT_VAR_CLEARED already defined
#endif
#define GPT_VAR_CLEARED

#if (defined GPT_VAR_POWER_ON_INIT)
#error GPT_VAR_POWER_ON_INIT already defined
#endif
#define GPT_VAR_POWER_ON_INIT

#if (defined GPT_VAR_FAST)
#error GPT_VAR_FAST already defined
#endif
#define GPT_VAR_FAST

#if (defined GPT_VAR)
#error GPT_VAR already defined
#endif
#define GPT_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined GW_CODE)
#error GW_CODE already defined
#endif
#define GW_CODE

#if (defined GW_CONST)
#error GW_CONST already defined
#endif
#define GW_CONST

#if (defined GW_APPL_DATA)
#error GW_APPL_DATA already defined
#endif
#define GW_APPL_DATA

#if (defined GW_APPL_CONST)
#error GW_APPL_CONST already defined
#endif
#define GW_APPL_CONST

#if (defined GW_APPL_CODE)
#error GW_APPL_CODE already defined
#endif
#define GW_APPL_CODE

#if (defined GW_VAR_NOINIT)
#error GW_VAR_NOINIT already defined
#endif
#define GW_VAR_NOINIT

#if (defined GW_VAR_CLEARED)
#error GW_VAR_CLEARED already defined
#endif
#define GW_VAR_CLEARED

#if (defined GW_VAR_POWER_ON_INIT)
#error GW_VAR_POWER_ON_INIT already defined
#endif
#define GW_VAR_POWER_ON_INIT

#if (defined GW_VAR_FAST)
#error GW_VAR_FAST already defined
#endif
#define GW_VAR_FAST

#if (defined GW_VAR)
#error GW_VAR already defined
#endif
#define GW_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ICU_CODE)
#error ICU_CODE already defined
#endif
#define ICU_CODE

#if (defined ICU_CONST)
#error ICU_CONST already defined
#endif
#define ICU_CONST

#if (defined ICU_APPL_DATA)
#error ICU_APPL_DATA already defined
#endif
#define ICU_APPL_DATA

#if (defined ICU_APPL_CONST)
#error ICU_APPL_CONST already defined
#endif
#define ICU_APPL_CONST

#if (defined ICU_APPL_CODE)
#error ICU_APPL_CODE already defined
#endif
#define ICU_APPL_CODE

#if (defined ICU_VAR_NOINIT)
#error ICU_VAR_NOINIT already defined
#endif
#define ICU_VAR_NOINIT

#if (defined ICU_VAR_CLEARED)
#error ICU_VAR_CLEARED already defined
#endif
#define ICU_VAR_CLEARED

#if (defined ICU_VAR_POWER_ON_INIT)
#error ICU_VAR_POWER_ON_INIT already defined
#endif
#define ICU_VAR_POWER_ON_INIT

#if (defined ICU_VAR_FAST)
#error ICU_VAR_FAST already defined
#endif
#define ICU_VAR_FAST

#if (defined ICU_VAR)
#error ICU_VAR already defined
#endif
#define ICU_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined IOHWAB_CODE)
#error IOHWAB_CODE already defined
#endif
#define IOHWAB_CODE

#if (defined IOHWAB_CONST)
#error IOHWAB_CONST already defined
#endif
#define IOHWAB_CONST

#if (defined IOHWAB_APPL_DATA)
#error IOHWAB_APPL_DATA already defined
#endif
#define IOHWAB_APPL_DATA

#if (defined IOHWAB_APPL_CONST)
#error IOHWAB_APPL_CONST already defined
#endif
#define IOHWAB_APPL_CONST

#if (defined IOHWAB_APPL_CODE)
#error IOHWAB_APPL_CODE already defined
#endif
#define IOHWAB_APPL_CODE

#if (defined IOHWAB_VAR_NOINIT)
#error IOHWAB_VAR_NOINIT already defined
#endif
#define IOHWAB_VAR_NOINIT

#if (defined IOHWAB_VAR_CLEARED)
#error IOHWAB_VAR_CLEARED already defined
#endif
#define IOHWAB_VAR_CLEARED

#if (defined IOHWAB_VAR_POWER_ON_INIT)
#error IOHWAB_VAR_POWER_ON_INIT already defined
#endif
#define IOHWAB_VAR_POWER_ON_INIT

#if (defined IOHWAB_VAR_FAST)
#error IOHWAB_VAR_FAST already defined
#endif
#define IOHWAB_VAR_FAST

#if (defined IOHWAB_VAR)
#error IOHWAB_VAR already defined
#endif
#define IOHWAB_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined IPDUM_CODE)
#error IPDUM_CODE already defined
#endif
#define IPDUM_CODE

#if (defined IPDUM_CONST)
#error IPDUM_CONST already defined
#endif
#define IPDUM_CONST

#if (defined IPDUM_APPL_DATA)
#error IPDUM_APPL_DATA already defined
#endif
#define IPDUM_APPL_DATA

#if (defined IPDUM_APPL_CONST)
#error IPDUM_APPL_CONST already defined
#endif
#define IPDUM_APPL_CONST

#if (defined IPDUM_APPL_CODE)
#error IPDUM_APPL_CODE already defined
#endif
#define IPDUM_APPL_CODE

#if (defined IPDUM_VAR_NOINIT)
#error IPDUM_VAR_NOINIT already defined
#endif
#define IPDUM_VAR_NOINIT

#if (defined IPDUM_VAR_CLEARED)
#error IPDUM_VAR_CLEARED already defined
#endif
#define IPDUM_VAR_CLEARED

#if (defined IPDUM_VAR_POWER_ON_INIT)
#error IPDUM_VAR_POWER_ON_INIT already defined
#endif
#define IPDUM_VAR_POWER_ON_INIT

#if (defined IPDUM_VAR_FAST)
#error IPDUM_VAR_FAST already defined
#endif
#define IPDUM_VAR_FAST

#if (defined IPDUM_VAR)
#error IPDUM_VAR already defined
#endif
#define IPDUM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939DM_CODE)
#error J1939DM_CODE already defined
#endif
#define J1939DM_CODE

#if (defined J1939DM_CONST)
#error J1939DM_CONST already defined
#endif
#define J1939DM_CONST

#if (defined J1939DM_APPL_DATA)
#error J1939DM_APPL_DATA already defined
#endif
#define J1939DM_APPL_DATA

#if (defined J1939DM_APPL_CONST)
#error J1939DM_APPL_CONST already defined
#endif
#define J1939DM_APPL_CONST

#if (defined J1939DM_APPL_CODE)
#error J1939DM_APPL_CODE already defined
#endif
#define J1939DM_APPL_CODE

#if (defined J1939DM_VAR_NOINIT)
#error J1939DM_VAR_NOINIT already defined
#endif
#define J1939DM_VAR_NOINIT

#if (defined J1939DM_VAR_CLEARED)
#error J1939DM_VAR_CLEARED already defined
#endif
#define J1939DM_VAR_CLEARED

#if (defined J1939DM_VAR_POWER_ON_INIT)
#error J1939DM_VAR_POWER_ON_INIT already defined
#endif
#define J1939DM_VAR_POWER_ON_INIT

#if (defined J1939DM_VAR_FAST)
#error J1939DM_VAR_FAST already defined
#endif
#define J1939DM_VAR_FAST

#if (defined J1939DM_VAR)
#error J1939DM_VAR already defined
#endif
#define J1939DM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939RSM_CODE)
#error J1939RSM_CODE already defined
#endif
#define J1939RSM_CODE

#if (defined J1939RSM_CONST)
#error J1939RSM_CONST already defined
#endif
#define J1939RSM_CONST

#if (defined J1939RSM_APPL_DATA)
#error J1939RSM_APPL_DATA already defined
#endif
#define J1939RSM_APPL_DATA

#if (defined J1939RSM_APPL_CONST)
#error J1939RSM_APPL_CONST already defined
#endif
#define J1939RSM_APPL_CONST

#if (defined J1939RSM_APPL_CODE)
#error J1939RSM_APPL_CODE already defined
#endif
#define J1939RSM_APPL_CODE

#if (defined J1939RSM_VAR_NOINIT)
#error J1939RSM_VAR_NOINIT already defined
#endif
#define J1939RSM_VAR_NOINIT

#if (defined J1939RSM_VAR_CLEARED)
#error J1939RSM_VAR_CLEARED already defined
#endif
#define J1939RSM_VAR_CLEARED

#if (defined J1939RSM_VAR_POWER_ON_INIT)
#error J1939RSM_VAR_POWER_ON_INIT already defined
#endif
#define J1939RSM_VAR_POWER_ON_INIT

#if (defined J1939RSM_VAR_FAST)
#error J1939RSM_VAR_FAST already defined
#endif
#define J1939RSM_VAR_FAST

#if (defined J1939RSM_VAR)
#error J1939RSM_VAR already defined
#endif
#define J1939RSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939STACK_CODE)
#error J1939STACK_CODE already defined
#endif
#define J1939STACK_CODE

#if (defined J1939STACK_CONST)
#error J1939STACK_CONST already defined
#endif
#define J1939STACK_CONST

#if (defined J1939STACK_APPL_DATA)
#error J1939STACK_APPL_DATA already defined
#endif
#define J1939STACK_APPL_DATA

#if (defined J1939STACK_APPL_CONST)
#error J1939STACK_APPL_CONST already defined
#endif
#define J1939STACK_APPL_CONST

#if (defined J1939STACK_APPL_CODE)
#error J1939STACK_APPL_CODE already defined
#endif
#define J1939STACK_APPL_CODE

#if (defined J1939STACK_VAR_NOINIT)
#error J1939STACK_VAR_NOINIT already defined
#endif
#define J1939STACK_VAR_NOINIT

#if (defined J1939STACK_VAR_CLEARED)
#error J1939STACK_VAR_CLEARED already defined
#endif
#define J1939STACK_VAR_CLEARED

#if (defined J1939STACK_VAR_POWER_ON_INIT)
#error J1939STACK_VAR_POWER_ON_INIT already defined
#endif
#define J1939STACK_VAR_POWER_ON_INIT

#if (defined J1939STACK_VAR_FAST)
#error J1939STACK_VAR_FAST already defined
#endif
#define J1939STACK_VAR_FAST

#if (defined J1939STACK_VAR)
#error J1939STACK_VAR already defined
#endif
#define J1939STACK_VAR

#if (defined J1939STACK_DATA)
#error J1939STACK_DATA already defined
#endif
#define J1939STACK_DATA


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939TP_CODE)
#error J1939TP_CODE already defined
#endif
#define J1939TP_CODE

#if (defined J1939TP_CONST)
#error J1939TP_CONST already defined
#endif
#define J1939TP_CONST

#if (defined J1939TP_APPL_DATA)
#error J1939TP_APPL_DATA already defined
#endif
#define J1939TP_APPL_DATA

#if (defined J1939TP_APPL_CONST)
#error J1939TP_APPL_CONST already defined
#endif
#define J1939TP_APPL_CONST

#if (defined J1939TP_APPL_CODE)
#error J1939TP_APPL_CODE already defined
#endif
#define J1939TP_APPL_CODE

#if (defined J1939TP_VAR_NOINIT)
#error J1939TP_VAR_NOINIT already defined
#endif
#define J1939TP_VAR_NOINIT

#if (defined J1939TP_VAR_CLEARED)
#error J1939TP_VAR_CLEARED already defined
#endif
#define J1939TP_VAR_CLEARED

#if (defined J1939TP_VAR_POWER_ON_INIT)
#error J1939TP_VAR_POWER_ON_INIT already defined
#endif
#define J1939TP_VAR_POWER_ON_INIT

#if (defined J1939TP_VAR_FAST)
#error J1939TP_VAR_FAST already defined
#endif
#define J1939TP_VAR_FAST

#if (defined J1939TP_VAR)
#error J1939TP_VAR already defined
#endif
#define J1939TP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LDCOM_CODE)
#error LDCOM_CODE already defined
#endif
#define LDCOM_CODE

#if (defined LDCOM_CONST)
#error LDCOM_CONST already defined
#endif
#define LDCOM_CONST

#if (defined LDCOM_APPL_DATA)
#error LDCOM_APPL_DATA already defined
#endif
#define LDCOM_APPL_DATA

#if (defined LDCOM_APPL_CONST)
#error LDCOM_APPL_CONST already defined
#endif
#define LDCOM_APPL_CONST

#if (defined LDCOM_APPL_CODE)
#error LDCOM_APPL_CODE already defined
#endif
#define LDCOM_APPL_CODE

#if (defined LDCOM_VAR_NOINIT)
#error LDCOM_VAR_NOINIT already defined
#endif
#define LDCOM_VAR_NOINIT

#if (defined LDCOM_VAR_CLEARED)
#error LDCOM_VAR_CLEARED already defined
#endif
#define LDCOM_VAR_CLEARED

#if (defined LDCOM_VAR_POWER_ON_INIT)
#error LDCOM_VAR_POWER_ON_INIT already defined
#endif
#define LDCOM_VAR_POWER_ON_INIT

#if (defined LDCOM_VAR_FAST)
#error LDCOM_VAR_FAST already defined
#endif
#define LDCOM_VAR_FAST

#if (defined LDCOM_VAR)
#error LDCOM_VAR already defined
#endif
#define LDCOM_VAR

#if (defined LDCOM_RTE_CODE)
#error LDCOM_RTE_CODE already defined
#endif
#define LDCOM_RTE_CODE


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LIN_CODE)
#error LIN_CODE already defined
#endif
#define LIN_CODE

#if (defined LIN_CONST)
#error LIN_CONST already defined
#endif
#define LIN_CONST

#if (defined LIN_APPL_DATA)
#error LIN_APPL_DATA already defined
#endif
#define LIN_APPL_DATA

#if (defined LIN_APPL_CONST)
#error LIN_APPL_CONST already defined
#endif
#define LIN_APPL_CONST

#if (defined LIN_APPL_CODE)
#error LIN_APPL_CODE already defined
#endif
#define LIN_APPL_CODE

#if (defined LIN_VAR_NOINIT)
#error LIN_VAR_NOINIT already defined
#endif
#define LIN_VAR_NOINIT

#if (defined LIN_VAR_CLEARED)
#error LIN_VAR_CLEARED already defined
#endif
#define LIN_VAR_CLEARED

#if (defined LIN_VAR_POWER_ON_INIT)
#error LIN_VAR_POWER_ON_INIT already defined
#endif
#define LIN_VAR_POWER_ON_INIT

#if (defined LIN_VAR_FAST)
#error LIN_VAR_FAST already defined
#endif
#define LIN_VAR_FAST

#if (defined LIN_VAR)
#error LIN_VAR already defined
#endif
#define LIN_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LINIF_CODE)
#error LINIF_CODE already defined
#endif
#define LINIF_CODE

#if (defined LINIF_CONST)
#error LINIF_CONST already defined
#endif
#define LINIF_CONST

#if (defined LINIF_APPL_DATA)
#error LINIF_APPL_DATA already defined
#endif
#define LINIF_APPL_DATA

#if (defined LINIF_APPL_CONST)
#error LINIF_APPL_CONST already defined
#endif
#define LINIF_APPL_CONST

#if (defined LINIF_APPL_CODE)
#error LINIF_APPL_CODE already defined
#endif
#define LINIF_APPL_CODE

#if (defined LINIF_VAR_NOINIT)
#error LINIF_VAR_NOINIT already defined
#endif
#define LINIF_VAR_NOINIT

#if (defined LINIF_VAR_CLEARED)
#error LINIF_VAR_CLEARED already defined
#endif
#define LINIF_VAR_CLEARED

#if (defined LINIF_VAR_POWER_ON_INIT)
#error LINIF_VAR_POWER_ON_INIT already defined
#endif
#define LINIF_VAR_POWER_ON_INIT

#if (defined LINIF_VAR_FAST)
#error LINIF_VAR_FAST already defined
#endif
#define LINIF_VAR_FAST

#if (defined LINIF_VAR)
#error LINIF_VAR already defined
#endif
#define LINIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LINSM_CODE)
#error LINSM_CODE already defined
#endif
#define LINSM_CODE

#if (defined LINSM_CONST)
#error LINSM_CONST already defined
#endif
#define LINSM_CONST

#if (defined LINSM_APPL_DATA)
#error LINSM_APPL_DATA already defined
#endif
#define LINSM_APPL_DATA

#if (defined LINSM_APPL_CONST)
#error LINSM_APPL_CONST already defined
#endif
#define LINSM_APPL_CONST

#if (defined LINSM_APPL_CODE)
#error LINSM_APPL_CODE already defined
#endif
#define LINSM_APPL_CODE

#if (defined LINSM_VAR_NOINIT)
#error LINSM_VAR_NOINIT already defined
#endif
#define LINSM_VAR_NOINIT

#if (defined LINSM_VAR_CLEARED)
#error LINSM_VAR_CLEARED already defined
#endif
#define LINSM_VAR_CLEARED

#if (defined LINSM_VAR_POWER_ON_INIT)
#error LINSM_VAR_POWER_ON_INIT already defined
#endif
#define LINSM_VAR_POWER_ON_INIT

#if (defined LINSM_VAR_FAST)
#error LINSM_VAR_FAST already defined
#endif
#define LINSM_VAR_FAST

#if (defined LINSM_VAR)
#error LINSM_VAR already defined
#endif
#define LINSM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LINTP_CODE)
#error LINTP_CODE already defined
#endif
#define LINTP_CODE

#if (defined LINTP_CONST)
#error LINTP_CONST already defined
#endif
#define LINTP_CONST

#if (defined LINTP_APPL_DATA)
#error LINTP_APPL_DATA already defined
#endif
#define LINTP_APPL_DATA

#if (defined LINTP_APPL_CONST)
#error LINTP_APPL_CONST already defined
#endif
#define LINTP_APPL_CONST

#if (defined LINTP_APPL_CODE)
#error LINTP_APPL_CODE already defined
#endif
#define LINTP_APPL_CODE

#if (defined LINTP_VAR_NOINIT)
#error LINTP_VAR_NOINIT already defined
#endif
#define LINTP_VAR_NOINIT

#if (defined LINTP_VAR_CLEARED)
#error LINTP_VAR_CLEARED already defined
#endif
#define LINTP_VAR_CLEARED

#if (defined LINTP_VAR_POWER_ON_INIT)
#error LINTP_VAR_POWER_ON_INIT already defined
#endif
#define LINTP_VAR_POWER_ON_INIT

#if (defined LINTP_VAR_FAST)
#error LINTP_VAR_FAST already defined
#endif
#define LINTP_VAR_FAST

#if (defined LINTP_VAR)
#error LINTP_VAR already defined
#endif
#define LINTP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined LINTRCV_1_T01_CODE)
#error LINTRCV_1_T01_CODE already defined
#endif
#define LINTRCV_1_T01_CODE

#if (defined LINTRCV_1_T01_CONST)
#error LINTRCV_1_T01_CONST already defined
#endif
#define LINTRCV_1_T01_CONST

#if (defined LINTRCV_1_T01_APPL_DATA)
#error LINTRCV_1_T01_APPL_DATA already defined
#endif
#define LINTRCV_1_T01_APPL_DATA

#if (defined LINTRCV_1_T01_APPL_CONST)
#error LINTRCV_1_T01_APPL_CONST already defined
#endif
#define LINTRCV_1_T01_APPL_CONST

#if (defined LINTRCV_1_T01_APPL_CODE)
#error LINTRCV_1_T01_APPL_CODE already defined
#endif
#define LINTRCV_1_T01_APPL_CODE

#if (defined LINTRCV_1_T01_VAR_NOINIT)
#error LINTRCV_1_T01_VAR_NOINIT already defined
#endif
#define LINTRCV_1_T01_VAR_NOINIT

#if (defined LINTRCV_1_T01_VAR_CLEARED)
#error LINTRCV_1_T01_VAR_CLEARED already defined
#endif
#define LINTRCV_1_T01_VAR_CLEARED

#if (defined LINTRCV_1_T01_VAR_POWER_ON_INIT)
#error LINTRCV_1_T01_VAR_POWER_ON_INIT already defined
#endif
#define LINTRCV_1_T01_VAR_POWER_ON_INIT

#if (defined LINTRCV_1_T01_VAR_FAST)
#error LINTRCV_1_T01_VAR_FAST already defined
#endif
#define LINTRCV_1_T01_VAR_FAST

#if (defined LINTRCV_1_T01_VAR)
#error LINTRCV_1_T01_VAR already defined
#endif
#define LINTRCV_1_T01_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined MCU_CODE)
#error MCU_CODE already defined
#endif
#define MCU_CODE

#if (defined MCU_CONST)
#error MCU_CONST already defined
#endif
#define MCU_CONST

#if (defined MCU_APPL_DATA)
#error MCU_APPL_DATA already defined
#endif
#define MCU_APPL_DATA

#if (defined MCU_APPL_CONST)
#error MCU_APPL_CONST already defined
#endif
#define MCU_APPL_CONST

#if (defined MCU_APPL_CODE)
#error MCU_APPL_CODE already defined
#endif
#define MCU_APPL_CODE

#if (defined MCU_VAR_NOINIT)
#error MCU_VAR_NOINIT already defined
#endif
#define MCU_VAR_NOINIT

#if (defined MCU_VAR_CLEARED)
#error MCU_VAR_CLEARED already defined
#endif
#define MCU_VAR_CLEARED

#if (defined MCU_VAR_POWER_ON_INIT)
#error MCU_VAR_POWER_ON_INIT already defined
#endif
#define MCU_VAR_POWER_ON_INIT

#if (defined MCU_VAR_FAST)
#error MCU_VAR_FAST already defined
#endif
#define MCU_VAR_FAST

#if (defined MCU_VAR)
#error MCU_VAR already defined
#endif
#define MCU_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined MEMACC_CODE)
#error MEMACC_CODE already defined
#endif
#define MEMACC_CODE

#if (defined MEMIF_CONST)
#error MEMACC_CONST already defined
#endif
#define MEMACC_CONST

#if (defined MEMIF_APPL_DATA)
#error MEMACC_APPL_DATA already defined
#endif
#define MEMACC_APPL_DATA

#if (defined MEMIF_APPL_CONST)
#error MEMACC_APPL_CONST already defined
#endif
#define MEMACC_APPL_CONST

#if (defined MEMIF_APPL_CODE)
#error MEMACC_APPL_CODE already defined
#endif
#define MEMACC_APPL_CODE

#if (defined MEMIF_VAR_NOINIT)
#error MEMIF_VAR_NOINIT already defined
#endif
#define MEMACC_VAR_NOINIT

#if (defined MEMIF_VAR_CLEARED)
#error MEMACC_VAR_CLEARED already defined
#endif
#define MEMACC_VAR_CLEARED

#if (defined MEMIF_VAR_POWER_ON_INIT)
#error MEMACC_VAR_POWER_ON_INIT already defined
#endif
#define MEMACC_VAR_POWER_ON_INIT

#if (defined MEMIF_VAR_FAST)
#error MEMACC_VAR_FAST already defined
#endif
#define MEMACC_VAR_FAST

#if (defined MEMIF_VAR)
#error MEMACC_VAR already defined
#endif
#define MEMACC_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined MEMIF_CODE)
#error MEMIF_CODE already defined
#endif
#define MEMIF_CODE

#if (defined MEMIF_CONST)
#error MEMIF_CONST already defined
#endif
#define MEMIF_CONST

#if (defined MEMIF_APPL_DATA)
#error MEMIF_APPL_DATA already defined
#endif
#define MEMIF_APPL_DATA

#if (defined MEMIF_APPL_CONST)
#error MEMIF_APPL_CONST already defined
#endif
#define MEMIF_APPL_CONST

#if (defined MEMIF_APPL_CODE)
#error MEMIF_APPL_CODE already defined
#endif
#define MEMIF_APPL_CODE

#if (defined MEMIF_VAR_NOINIT)
#error MEMIF_VAR_NOINIT already defined
#endif
#define MEMIF_VAR_NOINIT

#if (defined MEMIF_VAR_CLEARED)
#error MEMIF_VAR_CLEARED already defined
#endif
#define MEMIF_VAR_CLEARED

#if (defined MEMIF_VAR_POWER_ON_INIT)
#error MEMIF_VAR_POWER_ON_INIT already defined
#endif
#define MEMIF_VAR_POWER_ON_INIT

#if (defined MEMIF_VAR_FAST)
#error MEMIF_VAR_FAST already defined
#endif
#define MEMIF_VAR_FAST

#if (defined MEMIF_VAR)
#error MEMIF_VAR already defined
#endif
#define MEMIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined NM_CODE)
#error NM_CODE already defined
#endif
#define NM_CODE

#if (defined NM_CONST)
#error NM_CONST already defined
#endif
#define NM_CONST

#if (defined NM_APPL_DATA)
#error NM_APPL_DATA already defined
#endif
#define NM_APPL_DATA

#if (defined NM_APPL_CONST)
#error NM_APPL_CONST already defined
#endif
#define NM_APPL_CONST

#if (defined NM_APPL_CODE)
#error NM_APPL_CODE already defined
#endif
#define NM_APPL_CODE

#if (defined NM_VAR_NOINIT)
#error NM_VAR_NOINIT already defined
#endif
#define NM_VAR_NOINIT

#if (defined NM_VAR_CLEARED)
#error NM_VAR_CLEARED already defined
#endif
#define NM_VAR_CLEARED

#if (defined NM_VAR_POWER_ON_INIT)
#error NM_VAR_POWER_ON_INIT already defined
#endif
#define NM_VAR_POWER_ON_INIT

#if (defined NM_VAR_FAST)
#error NM_VAR_FAST already defined
#endif
#define NM_VAR_FAST

#if (defined NM_VAR)
#error NM_VAR already defined
#endif
#define NM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined NVM_CODE)
#error NVM_CODE already defined
#endif
#define NVM_CODE

#if (defined NVM_CONST)
#error NVM_CONST already defined
#endif
#define NVM_CONST

#if (defined NVM_APPL_DATA)
#error NVM_APPL_DATA already defined
#endif
#define NVM_APPL_DATA

#if (defined NVM_APPL_CONST)
#error NVM_APPL_CONST already defined
#endif
#define NVM_APPL_CONST

#if (defined NVM_APPL_CODE)
#error NVM_APPL_CODE already defined
#endif
#define NVM_APPL_CODE

#if (defined NVM_VAR_NOINIT)
#error NVM_VAR_NOINIT already defined
#endif
#define NVM_VAR_NOINIT

#if (defined NVM_VAR_CLEARED)
#error NVM_VAR_CLEARED already defined
#endif
#define NVM_VAR_CLEARED

#if (defined NVM_VAR_POWER_ON_INIT)
#error NVM_VAR_POWER_ON_INIT already defined
#endif
#define NVM_VAR_POWER_ON_INIT

#if (defined NVM_VAR_POWER_ON_CLEARED)
#error NVM_VAR_POWER_ON_CLEARED already defined
#endif
#define NVM_VAR_POWER_ON_CLEARED

#if (defined NVM_VAR_FAST)
#error NVM_VAR_FAST already defined
#endif
#define NVM_VAR_FAST

#if (defined NVM_VAR)
#error NVM_VAR already defined
#endif
#define NVM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined OS_CODE)
#error OS_CODE already defined
#endif
#define OS_CODE

#if (defined OS_CONST)
#error OS_CONST already defined
#endif
#define OS_CONST

#if (defined OS_APPL_DATA)
#error OS_APPL_DATA already defined
#endif
#define OS_APPL_DATA

#if (defined OS_APPL_CONST)
#error OS_APPL_CONST already defined
#endif
#define OS_APPL_CONST

#if (defined OS_APPL_CODE)
#error OS_APPL_CODE already defined
#endif
#define OS_APPL_CODE

#if (defined OS_VAR_NOINIT)
#error OS_VAR_NOINIT already defined
#endif
#define OS_VAR_NOINIT

#if (defined OS_VAR_CLEARED)
#error OS_VAR_CLEARED already defined
#endif
#define OS_VAR_CLEARED

#if (defined OS_VAR_POWER_ON_INIT)
#error OS_VAR_POWER_ON_INIT already defined
#endif
#define OS_VAR_POWER_ON_INIT

#if (defined OS_VAR_FAST)
#error OS_VAR_FAST already defined
#endif
#define OS_VAR_FAST

#if (defined OS_VAR)
#error OS_VAR already defined
#endif
#define OS_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined PBCFGM_CODE)
#error PBCFGM_CODE already defined
#endif
#define PBCFGM_CODE

#if (defined PBCFGM_CONST)
#error PBCFGM_CONST already defined
#endif
#define PBCFGM_CONST

#if (defined PBCFGM_APPL_DATA)
#error PBCFGM_APPL_DATA already defined
#endif
#define PBCFGM_APPL_DATA

#if (defined PBCFGM_APPL_CONST)
#error PBCFGM_APPL_CONST already defined
#endif
#define PBCFGM_APPL_CONST

#if (defined PBCFGM_APPL_CODE)
#error PBCFGM_APPL_CODE already defined
#endif
#define PBCFGM_APPL_CODE

#if (defined PBCFGM_VAR_NOINIT)
#error PBCFGM_VAR_NOINIT already defined
#endif
#define PBCFGM_VAR_NOINIT

#if (defined PBCFGM_VAR_CLEARED)
#error PBCFGM_VAR_CLEARED already defined
#endif
#define PBCFGM_VAR_CLEARED

#if (defined PBCFGM_VAR_POWER_ON_INIT)
#error PBCFGM_VAR_POWER_ON_INIT already defined
#endif
#define PBCFGM_VAR_POWER_ON_INIT

#if (defined PBCFGM_VAR_FAST)
#error PBCFGM_VAR_FAST already defined
#endif
#define PBCFGM_VAR_FAST

#if (defined PBCFGM_VAR)
#error PBCFGM_VAR already defined
#endif
#define PBCFGM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined PDUR_CODE)
#error PDUR_CODE already defined
#endif
#define PDUR_CODE

#if (defined PDUR_CONST)
#error PDUR_CONST already defined
#endif
#define PDUR_CONST

#if (defined PDUR_APPL_DATA)
#error PDUR_APPL_DATA already defined
#endif
#define PDUR_APPL_DATA

#if (defined PDUR_APPL_CONST)
#error PDUR_APPL_CONST already defined
#endif
#define PDUR_APPL_CONST

#if (defined PDUR_APPL_CODE)
#error PDUR_APPL_CODE already defined
#endif
#define PDUR_APPL_CODE

#if (defined PDUR_VAR_NOINIT)
#error PDUR_VAR_NOINIT already defined
#endif
#define PDUR_VAR_NOINIT

#if (defined PDUR_VAR_CLEARED)
#error PDUR_VAR_CLEARED already defined
#endif
#define PDUR_VAR_CLEARED

#if (defined PDUR_VAR_POWER_ON_INIT)
#error PDUR_VAR_POWER_ON_INIT already defined
#endif
#define PDUR_VAR_POWER_ON_INIT

#if (defined PDUR_VAR_FAST)
#error PDUR_VAR_FAST already defined
#endif
#define PDUR_VAR_FAST

#if (defined PDUR_VAR)
#error PDUR_VAR already defined
#endif
#define PDUR_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined PORT_CODE)
#error PORT_CODE already defined
#endif
#define PORT_CODE

#if (defined PORT_CONST)
#error PORT_CONST already defined
#endif
#define PORT_CONST

#if (defined PORT_APPL_DATA)
#error PORT_APPL_DATA already defined
#endif
#define PORT_APPL_DATA

#if (defined PORT_APPL_CONST)
#error PORT_APPL_CONST already defined
#endif
#define PORT_APPL_CONST

#if (defined PORT_APPL_CODE)
#error PORT_APPL_CODE already defined
#endif
#define PORT_APPL_CODE

#if (defined PORT_VAR_NOINIT)
#error PORT_VAR_NOINIT already defined
#endif
#define PORT_VAR_NOINIT

#if (defined PORT_VAR_CLEARED)
#error PORT_VAR_CLEARED already defined
#endif
#define PORT_VAR_CLEARED

#if (defined PORT_VAR_POWER_ON_INIT)
#error PORT_VAR_POWER_ON_INIT already defined
#endif
#define PORT_VAR_POWER_ON_INIT

#if (defined PORT_VAR_FAST)
#error PORT_VAR_FAST already defined
#endif
#define PORT_VAR_FAST

#if (defined PORT_VAR)
#error PORT_VAR already defined
#endif
#define PORT_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined PWM_CODE)
#error PWM_CODE already defined
#endif
#define PWM_CODE

#if (defined PWM_CONST)
#error PWM_CONST already defined
#endif
#define PWM_CONST

#if (defined PWM_APPL_DATA)
#error PWM_APPL_DATA already defined
#endif
#define PWM_APPL_DATA

#if (defined PWM_APPL_CONST)
#error PWM_APPL_CONST already defined
#endif
#define PWM_APPL_CONST

#if (defined PWM_APPL_CODE)
#error PWM_APPL_CODE already defined
#endif
#define PWM_APPL_CODE

#if (defined PWM_VAR_NOINIT)
#error PWM_VAR_NOINIT already defined
#endif
#define PWM_VAR_NOINIT

#if (defined PWM_VAR_CLEARED)
#error PWM_VAR_CLEARED already defined
#endif
#define PWM_VAR_CLEARED

#if (defined PWM_VAR_POWER_ON_INIT)
#error PWM_VAR_POWER_ON_INIT already defined
#endif
#define PWM_VAR_POWER_ON_INIT

#if (defined PWM_VAR_FAST)
#error PWM_VAR_FAST already defined
#endif
#define PWM_VAR_FAST

#if (defined PWM_VAR)
#error PWM_VAR already defined
#endif
#define PWM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined RAMTST_CODE)
#error RAMTST_CODE already defined
#endif
#define RAMTST_CODE

#if (defined RAMTST_CONST)
#error RAMTST_CONST already defined
#endif
#define RAMTST_CONST

#if (defined RAMTST_APPL_DATA)
#error RAMTST_APPL_DATA already defined
#endif
#define RAMTST_APPL_DATA

#if (defined RAMTST_APPL_CONST)
#error RAMTST_APPL_CONST already defined
#endif
#define RAMTST_APPL_CONST

#if (defined RAMTST_APPL_CODE)
#error RAMTST_APPL_CODE already defined
#endif
#define RAMTST_APPL_CODE

#if (defined RAMTST_VAR_NOINIT)
#error RAMTST_VAR_NOINIT already defined
#endif
#define RAMTST_VAR_NOINIT

#if (defined RAMTST_VAR_CLEARED)
#error RAMTST_VAR_CLEARED already defined
#endif
#define RAMTST_VAR_CLEARED

#if (defined RAMTST_VAR_POWER_ON_INIT)
#error RAMTST_VAR_POWER_ON_INIT already defined
#endif
#define RAMTST_VAR_POWER_ON_INIT

#if (defined RAMTST_VAR_FAST)
#error RAMTST_VAR_FAST already defined
#endif
#define RAMTST_VAR_FAST

#if (defined RAMTST_VAR)
#error RAMTST_VAR already defined
#endif
#define RAMTST_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined RTE_CODE)
#error RTE_CODE already defined
#endif
#define RTE_CODE

#if (defined RTE_CONST)
#error RTE_CONST already defined
#endif
#define RTE_CONST

#if (defined RTE_APPL_DATA)
#error RTE_APPL_DATA already defined
#endif
#define RTE_APPL_DATA

#if (defined RTE_APPL_CONST)
#error RTE_APPL_CONST already defined
#endif
#define RTE_APPL_CONST

#if (defined RTE_APPL_CODE)
#error RTE_APPL_CODE already defined
#endif
#define RTE_APPL_CODE

#if (defined RTE_VAR_NOINIT)
#error RTE_VAR_NOINIT already defined
#endif
#define RTE_VAR_NOINIT

#if (defined RTE_VAR_CLEARED)
#error RTE_VAR_CLEARED already defined
#endif
#define RTE_VAR_CLEARED

#if (defined RTE_VAR_POWER_ON_INIT)
#error RTE_VAR_POWER_ON_INIT already defined
#endif
#define RTE_VAR_POWER_ON_INIT

#if (defined RTE_VAR_FAST)
#error RTE_VAR_FAST already defined
#endif
#define RTE_VAR_FAST

#if (defined RTE_VAR)
#error RTE_VAR already defined
#endif
#define RTE_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SCHM_CODE)
#error SCHM_CODE already defined
#endif
#define SCHM_CODE

#if (defined SCHM_CONST)
#error SCHM_CONST already defined
#endif
#define SCHM_CONST

#if (defined SCHM_APPL_DATA)
#error SCHM_APPL_DATA already defined
#endif
#define SCHM_APPL_DATA

#if (defined SCHM_APPL_CONST)
#error SCHM_APPL_CONST already defined
#endif
#define SCHM_APPL_CONST

#if (defined SCHM_APPL_CODE)
#error SCHM_APPL_CODE already defined
#endif
#define SCHM_APPL_CODE

#if (defined SCHM_VAR_NOINIT)
#error SCHM_VAR_NOINIT already defined
#endif
#define SCHM_VAR_NOINIT

#if (defined SCHM_VAR_CLEARED)
#error SCHM_VAR_CLEARED already defined
#endif
#define SCHM_VAR_CLEARED

#if (defined SCHM_VAR_POWER_ON_INIT)
#error SCHM_VAR_POWER_ON_INIT already defined
#endif
#define SCHM_VAR_POWER_ON_INIT

#if (defined SCHM_VAR_FAST)
#error SCHM_VAR_FAST already defined
#endif
#define SCHM_VAR_FAST

#if (defined SCHM_VAR)
#error SCHM_VAR already defined
#endif
#define SCHM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SCRC_CODE)
#error SCRC_CODE already defined
#endif
#define SCRC_CODE

#if (defined SCRC_CONST)
#error SCRC_CONST already defined
#endif
#define SCRC_CONST

#if (defined SCRC_APPL_DATA)
#error SCRC_APPL_DATA already defined
#endif
#define SCRC_APPL_DATA

#if (defined SCRC_APPL_CONST)
#error SCRC_APPL_CONST already defined
#endif
#define SCRC_APPL_CONST

#if (defined SCRC_APPL_CODE)
#error SCRC_APPL_CODE already defined
#endif
#define SCRC_APPL_CODE

#if (defined SCRC_VAR_NOINIT)
#error SCRC_VAR_NOINIT already defined
#endif
#define SCRC_VAR_NOINIT

#if (defined SCRC_VAR_CLEARED)
#error SCRC_VAR_CLEARED already defined
#endif
#define SCRC_VAR_CLEARED

#if (defined SCRC_VAR_POWER_ON_INIT)
#error SCRC_VAR_POWER_ON_INIT already defined
#endif
#define SCRC_VAR_POWER_ON_INIT

#if (defined SCRC_VAR_FAST)
#error SCRC_VAR_FAST already defined
#endif
#define SCRC_VAR_FAST

#if (defined SCRC_VAR)
#error SCRC_VAR already defined
#endif
#define SCRC_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SD_CODE)
#error SD_CODE already defined
#endif
#define SD_CODE

#if (defined SD_CONST)
#error SD_CONST already defined
#endif
#define SD_CONST

#if (defined SD_APPL_DATA)
#error SD_APPL_DATA already defined
#endif
#define SD_APPL_DATA

#if (defined SD_APPL_CONST)
#error SD_APPL_CONST already defined
#endif
#define SD_APPL_CONST

#if (defined SD_APPL_CODE)
#error SD_APPL_CODE already defined
#endif
#define SD_APPL_CODE

#if (defined SD_VAR_NOINIT)
#error SD_VAR_NOINIT already defined
#endif
#define SD_VAR_NOINIT

#if (defined SD_VAR_CLEARED)
#error SD_VAR_CLEARED already defined
#endif
#define SD_VAR_CLEARED

#if (defined SD_VAR_POWER_ON_INIT)
#error SD_VAR_POWER_ON_INIT already defined
#endif
#define SD_VAR_POWER_ON_INIT

#if (defined SD_VAR_FAST)
#error SD_VAR_FAST already defined
#endif
#define SD_VAR_FAST

#if (defined SD_VAR)
#error SD_VAR already defined
#endif
#define SD_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SECOC_CODE)
#error SECOC_CODE already defined
#endif
#define SECOC_CODE

#if (defined SECOC_CONST)
#error SECOC_CONST already defined
#endif
#define SECOC_CONST

#if (defined SECOC_APPL_DATA)
#error SECOC_APPL_DATA already defined
#endif
#define SECOC_APPL_DATA

#if (defined SECOC_APPL_CONST)
#error SECOC_APPL_CONST already defined
#endif
#define SECOC_APPL_CONST

#if (defined SECOC_APPL_CODE)
#error SECOC_APPL_CODE already defined
#endif
#define SECOC_APPL_CODE

#if (defined SECOC_VAR_NOINIT)
#error SECOC_VAR_NOINIT already defined
#endif
#define SECOC_VAR_NOINIT

#if (defined SECOC_VAR_CLEARED)
#error SECOC_VAR_CLEARED already defined
#endif
#define SECOC_VAR_CLEARED

#if (defined SECOC_VAR_POWER_ON_INIT)
#error SECOC_VAR_POWER_ON_INIT already defined
#endif
#define SECOC_VAR_POWER_ON_INIT

#if (defined SECOC_VAR_FAST)
#error SECOC_VAR_FAST already defined
#endif
#define SECOC_VAR_FAST

#if (defined SECOC_VAR)
#error SECOC_VAR already defined
#endif
#define SECOC_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SOAD_CODE)
#error SOAD_CODE already defined
#endif
#define SOAD_CODE

#if (defined SOAD_CONST)
#error SOAD_CONST already defined
#endif
#define SOAD_CONST

#if (defined SOAD_APPL_DATA)
#error SOAD_APPL_DATA already defined
#endif
#define SOAD_APPL_DATA

#if (defined SOAD_APPL_CONST)
#error SOAD_APPL_CONST already defined
#endif
#define SOAD_APPL_CONST

#if (defined SOAD_APPL_CODE)
#error SOAD_APPL_CODE already defined
#endif
#define SOAD_APPL_CODE

#if (defined SOAD_VAR_NOINIT)
#error SOAD_VAR_NOINIT already defined
#endif
#define SOAD_VAR_NOINIT

#if (defined SOAD_VAR_CLEARED)
#error SOAD_VAR_CLEARED already defined
#endif
#define SOAD_VAR_CLEARED

#if (defined SOAD_VAR_POWER_ON_INIT)
#error SOAD_VAR_POWER_ON_INIT already defined
#endif
#define SOAD_VAR_POWER_ON_INIT

#if (defined SOAD_VAR_FAST)
#error SOAD_VAR_FAST already defined
#endif
#define SOAD_VAR_FAST

#if (defined SOAD_VAR)
#error SOAD_VAR already defined
#endif
#define SOAD_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SOADGENUL_CODE)
#error SOADGENUL_CODE already defined
#endif
#define SOADGENUL_CODE

#if (defined SOADGENUL_CONST)
#error SOADGENUL_CONST already defined
#endif
#define SOADGENUL_CONST

#if (defined SOADGENUL_APPL_DATA)
#error SOADGENUL_APPL_DATA already defined
#endif
#define SOADGENUL_APPL_DATA

#if (defined SOADGENUL_APPL_CONST)
#error SOADGENUL_APPL_CONST already defined
#endif
#define SOADGENUL_APPL_CONST

#if (defined SOADGENUL_APPL_CODE)
#error SOADGENUL_APPL_CODE already defined
#endif
#define SOADGENUL_APPL_CODE

#if (defined SOADGENUL_VAR_NOINIT)
#error SOADGENUL_VAR_NOINIT already defined
#endif
#define SOADGENUL_VAR_NOINIT

#if (defined SOADGENUL_VAR_CLEARED)
#error SOADGENUL_VAR_CLEARED already defined
#endif
#define SOADGENUL_VAR_CLEARED

#if (defined SOADGENUL_VAR_POWER_ON_INIT)
#error SOADGENUL_VAR_POWER_ON_INIT already defined
#endif
#define SOADGENUL_VAR_POWER_ON_INIT

#if (defined SOADGENUL_VAR_FAST)
#error SOADGENUL_VAR_FAST already defined
#endif
#define SOADGENUL_VAR_FAST

#if (defined SOADGENUL_VAR)
#error SOADGENUL_VAR already defined
#endif
#define SOADGENUL_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SOMEIPXF_CODE)
#error SOMEIPXF_CODE already defined
#endif
#define SOMEIPXF_CODE

#if (defined SOMEIPXF_CONST)
#error SOMEIPXF_CONST already defined
#endif
#define SOMEIPXF_CONST

#if (defined SOMEIPXF_APPL_DATA)
#error SOMEIPXF_APPL_DATA already defined
#endif
#define SOMEIPXF_APPL_DATA

#if (defined SOMEIPXF_APPL_CONST)
#error SOMEIPXF_APPL_CONST already defined
#endif
#define SOMEIPXF_APPL_CONST

#if (defined SOMEIPXF_APPL_CODE)
#error SOMEIPXF_APPL_CODE already defined
#endif
#define SOMEIPXF_APPL_CODE

#if (defined SOMEIPXF_VAR_NOINIT)
#error SOMEIPXF_VAR_NOINIT already defined
#endif
#define SOMEIPXF_VAR_NOINIT

#if (defined SOMEIPXF_VAR_CLEARED)
#error SOMEIPXF_VAR_CLEARED already defined
#endif
#define SOMEIPXF_VAR_CLEARED

#if (defined SOMEIPXF_VAR_POWER_ON_INIT)
#error SOMEIPXF_VAR_POWER_ON_INIT already defined
#endif
#define SOMEIPXF_VAR_POWER_ON_INIT

#if (defined SOMEIPXF_VAR_FAST)
#error SOMEIPXF_VAR_FAST already defined
#endif
#define SOMEIPXF_VAR_FAST

#if (defined SOMEIPXF_VAR)
#error SOMEIPXF_VAR already defined
#endif
#define SOMEIPXF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SOMEIPTP_CODE)
#error SOMEIPTP_CODE already defined
#endif
#define SOMEIPTP_CODE

#if (defined SOMEIPTP_CONST)
#error SOMEIPTP_CONST already defined
#endif
#define SOMEIPTP_CONST

#if (defined SOMEIPTP_APPL_DATA)
#error SOMEIPTP_APPL_DATA already defined
#endif
#define SOMEIPTP_APPL_DATA

#if (defined SOMEIPTP_APPL_CONST)
#error SOMEIPTP_APPL_CONST already defined
#endif
#define SOMEIPTP_APPL_CONST

#if (defined SOMEIPTP_APPL_CODE)
#error SOMEIPTP_APPL_CODE already defined
#endif
#define SOMEIPTP_APPL_CODE

#if (defined SOMEIPTP_VAR_NOINIT)
#error SOMEIPTP_VAR_NOINIT already defined
#endif
#define SOMEIPTP_VAR_NOINIT

#if (defined SOMEIPTP_VAR_CLEARED)
#error SOMEIPTP_VAR_CLEARED already defined
#endif
#define SOMEIPTP_VAR_CLEARED

#if (defined SOMEIPTP_VAR_POWER_ON_INIT)
#error SOMEIPTP_VAR_POWER_ON_INIT already defined
#endif
#define SOMEIPTP_VAR_POWER_ON_INIT

#if (defined SOMEIPTP_VAR_FAST)
#error SOMEIPTP_VAR_FAST already defined
#endif
#define SOMEIPTP_VAR_FAST

#if (defined SOMEIPTP_VAR)
#error SOMEIPTP_VAR already defined
#endif
#define SOMEIPTP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined MIRROR_CODE)
#error MIRROR_CODE already defined
#endif
#define MIRROR_CODE

#if (defined MIRROR_CONST)
#error MIRROR_CONST already defined
#endif
#define MIRROR_CONST

#if (defined MIRROR_APPL_DATA)
#error MIRROR_APPL_DATA already defined
#endif
#define MIRROR_APPL_DATA

#if (defined MIRROR_APPL_CONST)
#error MIRROR_APPL_CONST already defined
#endif
#define MIRROR_APPL_CONST

#if (defined MIRROR_APPL_CODE)
#error MIRROR_APPL_CODE already defined
#endif
#define MIRROR_APPL_CODE

#if (defined MIRROR_VAR_NOINIT)
#error MIRROR_VAR_NOINIT already defined
#endif
#define MIRROR_VAR_NOINIT

#if (defined MIRROR_VAR_CLEARED)
#error MIRROR_VAR_CLEARED already defined
#endif
#define MIRROR_VAR_CLEARED

#if (defined MIRROR_VAR_POWER_ON_INIT)
#error MIRROR_VAR_POWER_ON_INIT already defined
#endif
#define MIRROR_VAR_POWER_ON_INIT

#if (defined MIRROR_VAR_FAST)
#error MIRROR_VAR_FAST already defined
#endif
#define MIRROR_VAR_FAST

#if (defined MIRROR_VAR)
#error MIRROR_VAR already defined
#endif
#define MIRROR_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined SPI_CODE)
#error SPI_CODE already defined
#endif
#define SPI_CODE

#if (defined SPI_CONST)
#error SPI_CONST already defined
#endif
#define SPI_CONST

#if (defined SPI_APPL_DATA)
#error SPI_APPL_DATA already defined
#endif
#define SPI_APPL_DATA

#if (defined SPI_APPL_CONST)
#error SPI_APPL_CONST already defined
#endif
#define SPI_APPL_CONST

#if (defined SPI_APPL_CODE)
#error SPI_APPL_CODE already defined
#endif
#define SPI_APPL_CODE

#if (defined SPI_VAR_NOINIT)
#error SPI_VAR_NOINIT already defined
#endif
#define SPI_VAR_NOINIT

#if (defined SPI_VAR_CLEARED)
#error SPI_VAR_CLEARED already defined
#endif
#define SPI_VAR_CLEARED

#if (defined SPI_VAR_POWER_ON_INIT)
#error SPI_VAR_POWER_ON_INIT already defined
#endif
#define SPI_VAR_POWER_ON_INIT

#if (defined SPI_VAR_FAST)
#error SPI_VAR_FAST already defined
#endif
#define SPI_VAR_FAST

#if (defined SPI_VAR)
#error SPI_VAR already defined
#endif
#define SPI_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined STBM_CODE)
#error STBM_CODE already defined
#endif
#define STBM_CODE

#if (defined STBM_CONST)
#error STBM_CONST already defined
#endif
#define STBM_CONST

#if (defined STBM_APPL_DATA)
#error STBM_APPL_DATA already defined
#endif
#define STBM_APPL_DATA

#if (defined STBM_APPL_CONST)
#error STBM_APPL_CONST already defined
#endif
#define STBM_APPL_CONST

#if (defined STBM_APPL_CODE)
#error STBM_APPL_CODE already defined
#endif
#define STBM_APPL_CODE

#if (defined STBM_VAR_NOINIT)
#error STBM_VAR_NOINIT already defined
#endif
#define STBM_VAR_NOINIT

#if (defined STBM_VAR_CLEARED)
#error STBM_VAR_CLEARED already defined
#endif
#define STBM_VAR_CLEARED

#if (defined STBM_VAR_POWER_ON_INIT)
#error STBM_VAR_POWER_ON_INIT already defined
#endif
#define STBM_VAR_POWER_ON_INIT

#if (defined STBM_VAR_FAST)
#error STBM_VAR_FAST already defined
#endif
#define STBM_VAR_FAST

#if (defined STBM_VAR)
#error STBM_VAR already defined
#endif
#define STBM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined TCPIP_CODE)
#error TCPIP_CODE already defined
#endif
#define TCPIP_CODE

#if (defined TCPIP_CONST)
#error TCPIP_CONST already defined
#endif
#define TCPIP_CONST

#if (defined TCPIP_APPL_DATA)
#error TCPIP_APPL_DATA already defined
#endif
#define TCPIP_APPL_DATA

#if (defined TCPIP_APPL_CONST)
#error TCPIP_APPL_CONST already defined
#endif
#define TCPIP_APPL_CONST

#if (defined TCPIP_APPL_CODE)
#error TCPIP_APPL_CODE already defined
#endif
#define TCPIP_APPL_CODE

#if (defined TCPIP_VAR_NOINIT)
#error TCPIP_VAR_NOINIT already defined
#endif
#define TCPIP_VAR_NOINIT

#if (defined TCPIP_VAR_CLEARED)
#error TCPIP_VAR_CLEARED already defined
#endif
#define TCPIP_VAR_CLEARED

#if (defined TCPIP_VAR_POWER_ON_INIT)
#error TCPIP_VAR_POWER_ON_INIT already defined
#endif
#define TCPIP_VAR_POWER_ON_INIT

#if (defined TCPIP_VAR_FAST)
#error TCPIP_VAR_FAST already defined
#endif
#define TCPIP_VAR_FAST

#if (defined TCPIP_VAR)
#error TCPIP_VAR already defined
#endif
#define TCPIP_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined TLS_CODE)
#error TLS_CODE already defined
#endif
#define TLS_CODE

#if (defined TLS_CONST)
#error TLS_CONST already defined
#endif
#define TLS_CONST

#if (defined TLS_APPL_DATA)
#error TLS_APPL_DATA already defined
#endif
#define TLS_APPL_DATA

#if (defined TLS_APPL_CONST)
#error TLS_APPL_CONST already defined
#endif
#define TLS_APPL_CONST

#if (defined TLS_APPL_CODE)
#error TLS_APPL_CODE already defined
#endif
#define TLS_APPL_CODE

#if (defined TLS_VAR_NOINIT)
#error TLS_VAR_NOINIT already defined
#endif
#define TLS_VAR_NOINIT

#if (defined TLS_VAR_CLEARED)
#error TLS_VAR_CLEARED already defined
#endif
#define TLS_VAR_CLEARED

#if (defined TLS_VAR_POWER_ON_INIT)
#error TLS_VAR_POWER_ON_INIT already defined
#endif
#define TLS_VAR_POWER_ON_INIT

#if (defined TLS_VAR_FAST)
#error TLS_VAR_FAST already defined
#endif
#define TLS_VAR_FAST

#if (defined TLS_VAR)
#error TLS_VAR already defined
#endif
#define TLS_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined TS5ATL_CODE)
#error TS5ATL_CODE already defined
#endif
#define TS5ATL_CODE

#if (defined TS5ATL_CONST)
#error TS5ATL_CONST already defined
#endif
#define TS5ATL_CONST

#if (defined TS5ATL_APPL_DATA)
#error TS5ATL_APPL_DATA already defined
#endif
#define TS5ATL_APPL_DATA

#if (defined TS5ATL_APPL_CONST)
#error TS5ATL_APPL_CONST already defined
#endif
#define TS5ATL_APPL_CONST

#if (defined TS5ATL_APPL_CODE)
#error TS5ATL_APPL_CODE already defined
#endif
#define TS5ATL_APPL_CODE

#if (defined TS5ATL_VAR_NOINIT)
#error TS5ATL_VAR_NOINIT already defined
#endif
#define TS5ATL_VAR_NOINIT

#if (defined TS5ATL_VAR_CLEARED)
#error TS5ATL_VAR_CLEARED already defined
#endif
#define TS5ATL_VAR_CLEARED

#if (defined TS5ATL_VAR_POWER_ON_INIT)
#error TS5ATL_VAR_POWER_ON_INIT already defined
#endif
#define TS5ATL_VAR_POWER_ON_INIT

#if (defined TS5ATL_VAR_FAST)
#error TS5ATL_VAR_FAST already defined
#endif
#define TS5ATL_VAR_FAST

#if (defined TS5ATL_VAR)
#error TS5ATL_VAR already defined
#endif
#define TS5ATL_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined UDPNM_CODE)
#error UDPNM_CODE already defined
#endif
#define UDPNM_CODE

#if (defined UDPNM_CONST)
#error UDPNM_CONST already defined
#endif
#define UDPNM_CONST

#if (defined UDPNM_APPL_DATA)
#error UDPNM_APPL_DATA already defined
#endif
#define UDPNM_APPL_DATA

#if (defined UDPNM_APPL_CONST)
#error UDPNM_APPL_CONST already defined
#endif
#define UDPNM_APPL_CONST

#if (defined UDPNM_APPL_CODE)
#error UDPNM_APPL_CODE already defined
#endif
#define UDPNM_APPL_CODE

#if (defined UDPNM_VAR_NOINIT)
#error UDPNM_VAR_NOINIT already defined
#endif
#define UDPNM_VAR_NOINIT

#if (defined UDPNM_VAR_CLEARED)
#error UDPNM_VAR_CLEARED already defined
#endif
#define UDPNM_VAR_CLEARED

#if (defined UDPNM_VAR_POWER_ON_INIT)
#error UDPNM_VAR_POWER_ON_INIT already defined
#endif
#define UDPNM_VAR_POWER_ON_INIT

#if (defined UDPNM_VAR_FAST)
#error UDPNM_VAR_FAST already defined
#endif
#define UDPNM_VAR_FAST

#if (defined UDPNM_VAR)
#error UDPNM_VAR already defined
#endif
#define UDPNM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDG_CODE)
#error WDG_CODE already defined
#endif
#define WDG_CODE

#if (defined WDG_CONST)
#error WDG_CONST already defined
#endif
#define WDG_CONST

#if (defined WDG_APPL_DATA)
#error WDG_APPL_DATA already defined
#endif
#define WDG_APPL_DATA

#if (defined WDG_APPL_CONST)
#error WDG_APPL_CONST already defined
#endif
#define WDG_APPL_CONST

#if (defined WDG_APPL_CODE)
#error WDG_APPL_CODE already defined
#endif
#define WDG_APPL_CODE

#if (defined WDG_VAR_NOINIT)
#error WDG_VAR_NOINIT already defined
#endif
#define WDG_VAR_NOINIT

#if (defined WDG_VAR_CLEARED)
#error WDG_VAR_CLEARED already defined
#endif
#define WDG_VAR_CLEARED

#if (defined WDG_VAR_POWER_ON_INIT)
#error WDG_VAR_POWER_ON_INIT already defined
#endif
#define WDG_VAR_POWER_ON_INIT

#if (defined WDG_VAR_FAST)
#error WDG_VAR_FAST already defined
#endif
#define WDG_VAR_FAST

#if (defined WDG_VAR)
#error WDG_VAR already defined
#endif
#define WDG_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDGIF_CODE)
#error WDGIF_CODE already defined
#endif
#define WDGIF_CODE

#if (defined WDGIF_CONST)
#error WDGIF_CONST already defined
#endif
#define WDGIF_CONST

#if (defined WDGIF_APPL_DATA)
#error WDGIF_APPL_DATA already defined
#endif
#define WDGIF_APPL_DATA

#if (defined WDGIF_APPL_CONST)
#error WDGIF_APPL_CONST already defined
#endif
#define WDGIF_APPL_CONST

#if (defined WDGIF_APPL_CODE)
#error WDGIF_APPL_CODE already defined
#endif
#define WDGIF_APPL_CODE

#if (defined WDGIF_VAR_NOINIT)
#error WDGIF_VAR_NOINIT already defined
#endif
#define WDGIF_VAR_NOINIT

#if (defined WDGIF_VAR_CLEARED)
#error WDGIF_VAR_CLEARED already defined
#endif
#define WDGIF_VAR_CLEARED

#if (defined WDGIF_VAR_POWER_ON_INIT)
#error WDGIF_VAR_POWER_ON_INIT already defined
#endif
#define WDGIF_VAR_POWER_ON_INIT

#if (defined WDGIF_VAR_FAST)
#error WDGIF_VAR_FAST already defined
#endif
#define WDGIF_VAR_FAST

#if (defined WDGIF_VAR)
#error WDGIF_VAR already defined
#endif
#define WDGIF_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDGM_CODE)
#error WDGM_CODE already defined
#endif
#define WDGM_CODE

#if (defined WDGM_CONST)
#error WDGM_CONST already defined
#endif
#define WDGM_CONST

#if (defined WDGM_APPL_DATA)
#error WDGM_APPL_DATA already defined
#endif
#define WDGM_APPL_DATA

#if (defined WDGM_APPL_CONST)
#error WDGM_APPL_CONST already defined
#endif
#define WDGM_APPL_CONST

#if (defined WDGM_APPL_CODE)
#error WDGM_APPL_CODE already defined
#endif
#define WDGM_APPL_CODE

#if (defined WDGM_VAR_NOINIT)
#error WDGM_VAR_NOINIT already defined
#endif
#define WDGM_VAR_NOINIT

#if (defined WDGM_VAR_CLEARED)
#error WDGM_VAR_CLEARED already defined
#endif
#define WDGM_VAR_CLEARED

#if (defined WDGM_VAR_POWER_ON_INIT)
#error WDGM_VAR_POWER_ON_INIT already defined
#endif
#define WDGM_VAR_POWER_ON_INIT

#if (defined WDGM_VAR_POWER_ON_CLEARED)
#error WDGM_VAR_POWER_ON_CLEARED already defined
#endif
#define WDGM_VAR_POWER_ON_CLEARED

#if (defined WDGM_VAR_FAST)
#error WDGM_VAR_FAST already defined
#endif
#define WDGM_VAR_FAST

#if (defined WDGM_VAR)
#error WDGM_VAR already defined
#endif
#define WDGM_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDG_1_INT_CODE)
#error WDG_1_INT_CODE already defined
#endif
#define WDG_1_INT_CODE

#if (defined WDG_1_INT_CONST)
#error WDG_1_INT_CONST already defined
#endif
#define WDG_1_INT_CONST

#if (defined WDG_1_INT_APPL_DATA)
#error WDG_1_INT_APPL_DATA already defined
#endif
#define WDG_1_INT_APPL_DATA

#if (defined WDG_1_INT_APPL_CONST)
#error WDG_1_INT_APPL_CONST already defined
#endif
#define WDG_1_INT_APPL_CONST

#if (defined WDG_1_INT_APPL_CODE)
#error WDG_1_INT_APPL_CODE already defined
#endif
#define WDG_1_INT_APPL_CODE

#if (defined WDG_1_INT_VAR_NOINIT)
#error WDG_1_INT_VAR_NOINIT already defined
#endif
#define WDG_1_INT_VAR_NOINIT

#if (defined WDG_1_INT_VAR_CLEARED)
#error WDG_1_INT_VAR_CLEARED already defined
#endif
#define WDG_1_INT_VAR_CLEARED

#if (defined WDG_1_INT_VAR_POWER_ON_INIT)
#error WDG_1_INT_VAR_POWER_ON_INIT already defined
#endif
#define WDG_1_INT_VAR_POWER_ON_INIT

#if (defined WDG_1_INT_VAR_FAST)
#error WDG_1_INT_VAR_FAST already defined
#endif
#define WDG_1_INT_VAR_FAST

#if (defined WDG_1_INT_VAR)
#error WDG_1_INT_VAR already defined
#endif
#define WDG_1_INT_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDG_42_EXT_CODE)
#error WDG_42_EXT_CODE already defined
#endif
#define WDG_42_EXT_CODE

#if (defined WDG_42_EXT_CONST)
#error WDG_42_EXT_CONST already defined
#endif
#define WDG_42_EXT_CONST

#if (defined WDG_42_EXT_APPL_DATA)
#error WDG_42_EXT_APPL_DATA already defined
#endif
#define WDG_42_EXT_APPL_DATA

#if (defined WDG_42_EXT_APPL_CONST)
#error WDG_42_EXT_APPL_CONST already defined
#endif
#define WDG_42_EXT_APPL_CONST

#if (defined WDG_42_EXT_APPL_CODE)
#error WDG_42_EXT_APPL_CODE already defined
#endif
#define WDG_42_EXT_APPL_CODE

#if (defined WDG_42_EXT_VAR_NOINIT)
#error WDG_42_EXT_VAR_NOINIT already defined
#endif
#define WDG_42_EXT_VAR_NOINIT

#if (defined WDG_42_EXT_VAR_CLEARED)
#error WDG_42_EXT_VAR_CLEARED already defined
#endif
#define WDG_42_EXT_VAR_CLEARED

#if (defined WDG_42_EXT_VAR_POWER_ON_INIT)
#error WDG_42_EXT_VAR_POWER_ON_INIT already defined
#endif
#define WDG_42_EXT_VAR_POWER_ON_INIT

#if (defined WDG_42_EXT_VAR_FAST)
#error WDG_42_EXT_VAR_FAST already defined
#endif
#define WDG_42_EXT_VAR_FAST

#if (defined WDG_42_EXT_VAR)
#error WDG_42_EXT_VAR already defined
#endif
#define WDG_42_EXT_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined WDG_59_DRIVERA_CODE)
#error WDG_59_DRIVERA_CODE already defined
#endif
#define WDG_59_DRIVERA_CODE

#if (defined WDG_59_DRIVERA_CONST)
#error WDG_59_DRIVERA_CONST already defined
#endif
#define WDG_59_DRIVERA_CONST

#if (defined WDG_59_DRIVERA_APPL_DATA)
#error WDG_59_DRIVERA_APPL_DATA already defined
#endif
#define WDG_59_DRIVERA_APPL_DATA

#if (defined WDG_59_DRIVERA_APPL_CONST)
#error WDG_59_DRIVERA_APPL_CONST already defined
#endif
#define WDG_59_DRIVERA_APPL_CONST

#if (defined WDG_59_DRIVERA_APPL_CODE)
#error WDG_59_DRIVERA_APPL_CODE already defined
#endif
#define WDG_59_DRIVERA_APPL_CODE

#if (defined WDG_59_DRIVERA_VAR_NOINIT)
#error WDG_59_DRIVERA_VAR_NOINIT already defined
#endif
#define WDG_59_DRIVERA_VAR_NOINIT

#if (defined WDG_59_DRIVERA_VAR_CLEARED)
#error WDG_59_DRIVERA_VAR_CLEARED already defined
#endif
#define WDG_59_DRIVERA_VAR_CLEARED

#if (defined WDG_59_DRIVERA_VAR_POWER_ON_INIT)
#error WDG_59_DRIVERA_VAR_POWER_ON_INIT already defined
#endif
#define WDG_59_DRIVERA_VAR_POWER_ON_INIT

#if (defined WDG_59_DRIVERA_VAR_FAST)
#error WDG_59_DRIVERA_VAR_FAST already defined
#endif
#define WDG_59_DRIVERA_VAR_FAST

#if (defined WDG_59_DRIVERA_VAR)
#error WDG_59_DRIVERA_VAR already defined
#endif
#define WDG_59_DRIVERA_VAR


/*------------------[memory and pointer class of a module]-------------------*/

#if (defined XCP_CODE)
#error XCP_CODE already defined
#endif
#define XCP_CODE

#if (defined XCP_WRAPPER_CODE)
#error XCP_WRAPPER_CODE already defined
#endif
#define XCP_WRAPPER_CODE

#if (defined XCP_CONST)
#error XCP_CONST already defined
#endif
#define XCP_CONST

#if (defined XCP_APPL_DATA)
#error XCP_APPL_DATA already defined
#endif
#define XCP_APPL_DATA

#if (defined XCP_APPL_CONST)
#error XCP_APPL_CONST already defined
#endif
#define XCP_APPL_CONST

#if (defined XCP_APPL_CODE)
#error XCP_APPL_CODE already defined
#endif
#define XCP_APPL_CODE

#if (defined XCP_VAR_NOINIT)
#error XCP_VAR_NOINIT already defined
#endif
#define XCP_VAR_NOINIT

#if (defined XCP_VAR_CLEARED)
#error XCP_VAR_CLEARED already defined
#endif
#define XCP_VAR_CLEARED

#if (defined XCP_VAR_POWER_ON_INIT)
#error XCP_VAR_POWER_ON_INIT already defined
#endif
#define XCP_VAR_POWER_ON_INIT

#if (defined XCP_VAR_POWER_ON_CLEARED)
#error XCP_VAR_POWER_ON_CLEARED already defined
#endif
#define XCP_VAR_POWER_ON_CLEARED

#if (defined XCP_VAR_FAST)
#error XCP_VAR_FAST already defined
#endif
#define XCP_VAR_FAST

#if (defined XCP_VAR)
#error XCP_VAR already defined
#endif
#define XCP_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined XCPR_CODE)
#error XCPR_CODE already defined
#endif
#define XCPR_CODE

#if (defined XCPR_WRAPPER_CODE)
#error XCPR_WRAPPER_CODE already defined
#endif
#define XCPR_WRAPPER_CODE

#if (defined XCPR_CONST)
#error XCPR_CONST already defined
#endif
#define XCPR_CONST

#if (defined XCPR_APPL_DATA)
#error XCPR_APPL_DATA already defined
#endif
#define XCPR_APPL_DATA

#if (defined XCPR_APPL_CONST)
#error XCPR_APPL_CONST already defined
#endif
#define XCPR_APPL_CONST

#if (defined XCPR_APPL_CODE)
#error XCPR_APPL_CODE already defined
#endif
#define XCPR_APPL_CODE

#if (defined XCPR_VAR_NOINIT)
#error XCPR_VAR_NOINIT already defined
#endif
#define XCPR_VAR_NOINIT

#if (defined XCPR_VAR_CLEARED)
#error XCPR_VAR_CLEARED already defined
#endif
#define XCPR_VAR_CLEARED

#if (defined XCPR_VAR_POWER_ON_INIT)
#error XCPR_VAR_POWER_ON_INIT already defined
#endif
#define XCPR_VAR_POWER_ON_INIT

#if (defined XCPR_VAR_FAST)
#error XCPR_VAR_FAST already defined
#endif
#define XCPR_VAR_FAST

#if (defined XCPR_VAR)
#error XCPR_VAR already defined
#endif
#define XCPR_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined ATOMICS_CODE)
#error ATOMICS_CODE already defined
#endif
#define ATOMICS_CODE

#if (defined ATOMICS_WRAPPER_CODE)
#error ATOMICS_WRAPPER_CODE already defined
#endif
#define ATOMICS_WRAPPER_CODE

#if (defined ATOMICS_CONST)
#error ATOMICS_CONST already defined
#endif
#define ATOMICS_CONST

#if (defined ATOMICS_APPL_DATA)
#error ATOMICS_APPL_DATA already defined
#endif
#define ATOMICS_APPL_DATA

#if (defined ATOMICS_APPL_CONST)
#error ATOMICS_APPL_CONST already defined
#endif
#define ATOMICS_APPL_CONST

#if (defined ATOMICS_APPL_CODE)
#error ATOMICS_APPL_CODE already defined
#endif
#define ATOMICS_APPL_CODE

#if (defined ATOMICS_VAR_NOINIT)
#error ATOMICS_VAR_NOINIT already defined
#endif
#define ATOMICS_VAR_NOINIT

#if (defined ATOMICS_VAR_CLEARED)
#error ATOMICS_VAR_CLEARED already defined
#endif
#define ATOMICS_VAR_CLEARED

#if (defined ATOMICS_VAR_POWER_ON_INIT)
#error ATOMICS_VAR_POWER_ON_INIT already defined
#endif
#define ATOMICS_VAR_POWER_ON_INIT

#if (defined ATOMICS_VAR_FAST)
#error ATOMICS_VAR_FAST already defined
#endif
#define ATOMICS_VAR_FAST

#if (defined ATOMICS_VAR)
#error ATOMICS_VAR already defined
#endif
#define ATOMICS_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939NM_CODE)
#error J1939NM_CODE already defined
#endif
#define J1939NM_CODE

#if (defined J1939NM_CONST)
#error J1939NM_CONST already defined
#endif
#define J1939NM_CONST

#if (defined J1939NM_APPL_DATA)
#error J1939NM_APPL_DATA already defined
#endif
#define J1939NM_APPL_DATA

#if (defined J1939NM_APPL_CONST)
#error J1939NM_APPL_CONST already defined
#endif
#define J1939NM_APPL_CONST

#if (defined J1939NM_APPL_CODE)
#error J1939NM_APPL_CODE already defined
#endif
#define J1939NM_APPL_CODE

#if (defined J1939NM_VAR_NOINIT)
#error J1939NM_VAR_NOINIT already defined
#endif
#define J1939NM_VAR_NOINIT

#if (defined J1939NM_VAR_CLEARED)
#error J1939NM_VAR_CLEARED already defined
#endif
#define J1939NM_VAR_CLEARED

#if (defined J1939NM_VAR_POWER_ON_INIT)
#error J1939NM_VAR_POWER_ON_INIT already defined
#endif
#define J1939NM_VAR_POWER_ON_INIT

#if (defined J1939NM_VAR_FAST)
#error J1939NM_VAR_FAST already defined
#endif
#define J1939NM_VAR_FAST

#if (defined J1939NM_VAR)
#error J1939NM_VAR already defined
#endif
#define J1939NM_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939RM_CODE)
#error J1939RM_CODE already defined
#endif
#define J1939RM_CODE

#if (defined J1939RM_CONST)
#error J1939RM_CONST already defined
#endif
#define J1939RM_CONST

#if (defined J1939RM_APPL_DATA)
#error J1939RM_APPL_DATA already defined
#endif
#define J1939RM_APPL_DATA

#if (defined J1939RM_APPL_CONST)
#error J1939RM_APPL_CONST already defined
#endif
#define J1939RM_APPL_CONST

#if (defined J1939RM_APPL_CODE)
#error J1939RM_APPL_CODE already defined
#endif
#define J1939RM_APPL_CODE

#if (defined J1939RM_VAR_NOINIT)
#error J1939RM_VAR_NOINIT already defined
#endif
#define J1939RM_VAR_NOINIT

#if (defined J1939RM_VAR_CLEARED)
#error J1939RM_VAR_CLEARED already defined
#endif
#define J1939RM_VAR_CLEARED

#if (defined J1939RM_VAR_POWER_ON_INIT)
#error J1939RM_VAR_POWER_ON_INIT already defined
#endif
#define J1939RM_VAR_POWER_ON_INIT

#if (defined J1939RM_VAR_FAST)
#error J1939RM_VAR_FAST already defined
#endif
#define J1939RM_VAR_FAST

#if (defined J1939RM_VAR)
#error J1939RM_VAR already defined
#endif
#define J1939RM_VAR

/*------------------[memory and pointer class of a module]-------------------*/

#if (defined J1939DCM_CODE)
#error J1939DCM_CODE already defined
#endif
#define J1939DCM_CODE

#if (defined J1939DCM_CONST)
#error J1939DCM_CONST already defined
#endif
#define J1939DCM_CONST

#if (defined J1939DCM_APPL_DATA)
#error J1939DCM_APPL_DATA already defined
#endif
#define J1939DCM_APPL_DATA

#if (defined J1939DCM_APPL_CONST)
#error J1939DCM_APPL_CONST already defined
#endif
#define J1939DCM_APPL_CONST

#if (defined J1939DCM_APPL_CODE)
#error J1939DCM_APPL_CODE already defined
#endif
#define J1939DCM_APPL_CODE

#if (defined J1939DCM_VAR_NOINIT)
#error J1939DCM_VAR_NOINIT already defined
#endif
#define J1939DCM_VAR_NOINIT

#if (defined J1939DCM_VAR_CLEARED)
#error J1939DCM_VAR_CLEARED already defined
#endif
#define J1939DCM_VAR_CLEARED

#if (defined J1939DCM_VAR_POWER_ON_INIT)
#error J1939DCM_VAR_POWER_ON_INIT already defined
#endif
#define J1939DCM_VAR_POWER_ON_INIT

#if (defined J1939DCM_VAR_FAST)
#error J1939DCM_VAR_FAST already defined
#endif
#define J1939DCM_VAR_FAST

#if (defined J1939DCM_VAR)
#error J1939DCM_VAR already defined
#endif
#define J1939DCM_VAR

#if (defined TM_CODE)
#error TM_CODE already defined
#endif
#define TM_CODE
#if (defined TM_APPL_DATA)
#error TM_APPL_DATA already defined
#endif
#define TM_APPL_DATA
/* TS_ARCH_FAMILY == TS_WINDOWS end */

#endif /* if !defined( COMPILER_CFG_H ) */
/*==================[end of file]============================================*/
