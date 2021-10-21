/**
 * \file
 *
 * \brief AUTOSAR Crc
 *
 * This file contains the implementation of the AUTOSAR
 * module Crc.
 *
 * \version 6.11.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CRC_TRACE_H
#define CRC_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Crc_Trace_Internal.h>

/*==================[macros]================================================*/

#ifndef DBG_CRC_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Crc_GetVersionInfo() */
#define DBG_CRC_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_CRC_GETVERSIONINFO_EXIT
/** \brief Exit point of function Crc_GetVersionInfo() */
#define DBG_CRC_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_CRC_CALCULATECRC8_ENTRY
/** \brief Entry point of function Crc_CalculateCRC8() */
#define DBG_CRC_CALCULATECRC8_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CRC_CALCULATECRC8_EXIT
/** \brief Exit point of function Crc_CalculateCRC8() */
#define DBG_CRC_CALCULATECRC8_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_CRC_CALCULATECRC8H2F_ENTRY
/** \brief Entry point of function Crc_CalculateCRC8H2F() */
#define DBG_CRC_CALCULATECRC8H2F_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CRC_CALCULATECRC8H2F_EXIT
/** \brief Exit point of function Crc_CalculateCRC8H2F() */
#define DBG_CRC_CALCULATECRC8H2F_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_CRC_CALCULATECRC16_ENTRY
/** \brief Entry point of function Crc_CalculateCRC16() */
#define DBG_CRC_CALCULATECRC16_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CRC_CALCULATECRC16_EXIT
/** \brief Exit point of function Crc_CalculateCRC16() */
#define DBG_CRC_CALCULATECRC16_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_CRC_CALCULATECRC32_ENTRY
/** \brief Entry point of function Crc_CalculateCRC32() */
#define DBG_CRC_CALCULATECRC32_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CRC_CALCULATECRC32_EXIT
/** \brief Exit point of function Crc_CalculateCRC32() */
#define DBG_CRC_CALCULATECRC32_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_CRC_CALCULATECRC64_ENTRY
/** \brief Entry point of function Crc_CalculateCRC64() */
#define DBG_CRC_CALCULATECRC64_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CRC_CALCULATECRC64_EXIT
/** \brief Exit point of function Crc_CalculateCRC64() */
#define DBG_CRC_CALCULATECRC64_EXIT(a,b,c,d,e)
#endif
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CRC_TRACE_H */
/*==================[end of file]===========================================*/
