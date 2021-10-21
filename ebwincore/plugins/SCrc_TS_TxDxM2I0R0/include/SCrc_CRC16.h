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

#ifndef SCRC_CRC16_H
#define SCRC_CRC16_H

/*==================[includes]===============================================*/

#include <SCrc.h>              /* Module public API      */
#if (SCRC_FUNCENABLED_SCRC_CRC16 == STD_OFF)
#include <Crc.h>
#endif /* SCRC_FUNCENABLED_SCRC_CRC16 == STD_OFF */

/*==================[macros]=================================================*/

#if (SCRC_FUNCENABLED_SCRC_CRC16 == STD_OFF)

#if (defined SCrc_CalculateCRC16) /* to prevent double declaration */
#error SCrc_CalculateCRC16 is already defined
#endif

/** \brief Map the function SCrc_CalculateCRC16 to Crc_CalculateCRC16
 * of an external Crc module */
#define SCrc_CalculateCRC16(data,length,startvalue,isfirstcall) \
   Crc_CalculateCRC16((data),(length),(startvalue),(isfirstcall))

#endif /* SCRC_FUNCENABLED_SCRC_CRC16 == STD_OFF */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON)

#define SCRC_START_SEC_CODE
#include <SCrc_MemMap.h>

/** \brief Calculation of CRC16
 **
 ** This function performs the calculation of a 16-bit CRC value
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
 ** \param[in] SCrc_StartValue16 Initial Value
 ** \param[in] SCrc_IsFirstCall
 **    TRUE: First call in a sequence or individual CRC calculation;
 **          start from initial value, ignore Crc_StartValue16.
 **    FALSE: Subsequent call in a call sequence;
 **           Crc_StartValue16 is interpreted to be the return value
 **           of the previous function call.
 ** \return calculated CRC16 value
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint16, SCRC_CODE) SCrc_CalculateCRC16
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint16                                    SCrc_StartValue16,
    boolean                                   SCrc_IsFirstCall
);

#define SCRC_STOP_SEC_CODE
#include <SCrc_MemMap.h>

#endif /* SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON */

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef SCRC_CRC16_H */
/*==================[end of file]============================================*/
