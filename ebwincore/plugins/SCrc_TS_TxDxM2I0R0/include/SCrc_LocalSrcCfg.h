/**
 * \file
 *
 * \brief AUTOSAR SCrc
 *
 * This file contains the implementation of the AUTOSAR
 * module SCrc.
 *
 * \version 2.0.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SCRC_LOCALSRCCFG_H
#define SCRC_LOCALSRCCFG_H

/*==================[includes]===============================================*/

#include <Std_Types.h>   /* AUTOSAR standard types */

/*==================[macros]=================================================*/

#if (defined SCRC_FUNCENABLED_SCRC_CRC8) /* to prevent double declaration */
#error SCRC_FUNCENABLED_SCRC_CRC8 is already defined
#endif

/** \brief Defines whether or not SCrc_CalculateCRC8
 * shall map to SCrc_CalculateCRC8() of this SCrc module (STD_ON)
 * or shall map to an external function Crc_CalculateCRC8() declared in Crc.h (STD_OFF). */
#define SCRC_FUNCENABLED_SCRC_CRC8 STD_ON

#if (defined SCRC_FUNCENABLED_SCRC_CRC8H2F) /* to prevent double declaration */
#error SCRC_FUNCENABLED_SCRC_CRC8H2F is already defined
#endif

/** \brief Defines whether or not SCrc_CalculateCRC8H2F
 * shall map to SCrc_CalculateCRC8H2F() of this SCrc module (STD_ON)
 * or shall map to an external function Crc_CalculateCRC8H2F() declared in Crc.h (STD_OFF). */
#define SCRC_FUNCENABLED_SCRC_CRC8H2F STD_ON

#if (defined SCRC_FUNCENABLED_SCRC_CRC16) /* to prevent double declaration */
#error SCRC_FUNCENABLED_SCRC_CRC16 is already defined
#endif

/** \brief Defines whether or not SCrc_CalculateCRC16
 * shall map to SCrc_CalculateCRC16() of this SCrc module (STD_ON)
 * or shall map to an external function Crc_CalculateCRC16() declared in Crc.h (STD_OFF). */
#define SCRC_FUNCENABLED_SCRC_CRC16 STD_ON

#if (defined SCRC_FUNCENABLED_SCRC_CRC32P4) /* to prevent double declaration */
#error SCRC_FUNCENABLED_SCRC_CRC32P4 is already defined
#endif

/** \brief Defines whether or not SCrc_CalculateCRC32P4
 * shall map to SCrc_CalculateCRC32P4() of this SCrc module (STD_ON)
 * or shall map to an external function Crc_CalculateCRC32P4() declared in Crc.h (STD_OFF). */
#define SCRC_FUNCENABLED_SCRC_CRC32P4 STD_ON

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef SCRC_LOCALSRCCFG_H */
/*==================[end of file]============================================*/
