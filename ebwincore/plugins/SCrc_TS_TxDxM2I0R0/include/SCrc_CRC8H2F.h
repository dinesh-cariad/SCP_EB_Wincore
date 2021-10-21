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

#ifndef SCRC_CRC8H2F_H
#define SCRC_CRC8H2F_H

/*==================[includes]===============================================*/

#include <SCrc.h>              /* Module public API      */
#if (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_OFF)
#include <Crc.h>
#endif /* SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_OFF */

/*==================[macros]=================================================*/

#if (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_OFF)

#if (defined SCrc_CalculateCRC8H2F) /* to prevent double declaration */
#error SCrc_CalculateCRC8H2F is already defined
#endif

/** \brief Map the function SCrc_CalculateCRC8H2F to Crc_CalculateCRC8H2F
 * of an external Crc module */
#define SCrc_CalculateCRC8H2F(data,length,startvalue,isfirstcall) \
   Crc_CalculateCRC8H2F((data),(length),(startvalue),(isfirstcall))

#endif /* SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_OFF */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)

#define SCRC_START_SEC_CODE
#include <SCrc_MemMap.h>

/** \brief Calculation of CRC8 with the Polynomial 0x2F
 **
 ** This function performs the calculation of a 8-bit CRC value with the Polynomial 0x2F
 ** over the memory block referenced by \p SCrc_DataPtr of byte length \p
 ** SCrc_Length.
 **
 ** \pre The data where \a SCrc_DataPtr points is persistent
 **      (i.e. does not change during the calculation of the CRC)
 ** \pre SCrc_DataPtr is a valid pointer (does not equal a Null Pointer)
 ** \pre SCrc_Length is valid with respect to the passed data pointer
 ** \pre SCrc_IsFirstCall is valid (only TRUE or FALSE)
 **
 ** \param[in] SCrc_DataPtr Valid pointer to start address of data block
 ** \param[in] SCrc_Length  Length of data block in bytes
 ** \param[in] SCrc_StartValue8 Initial Value
 ** \param[in] SCrc_IsFirstCall
 **    TRUE: First call in a sequence or individual CRC calculation;
 **          start from initial value, ignore Crc_StartValue8.
 **    FALSE: Subsequent call in a call sequence;
 **           Crc_StartValue8 is interpreted to be the return value
 **           of the previous function call.
 ** \return calculated CRC8 value
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint8, SCRC_CODE) SCrc_CalculateCRC8H2F
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint8                                     SCrc_StartValue8H2F,
    boolean                                   SCrc_IsFirstCall
);

#define SCRC_STOP_SEC_CODE
#include <SCrc_MemMap.h>

#endif /* SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef SCRC_CRC8H2F_H */
/*==================[end of file]============================================*/
