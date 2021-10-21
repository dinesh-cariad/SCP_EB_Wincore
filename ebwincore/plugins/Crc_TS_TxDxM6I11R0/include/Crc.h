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
#ifndef CRC_H
#define CRC_H

/* !LINKSTO SWS_Crc_00024,1 */

/*==================[includes]===============================================*/

#include <Std_Types.h>   /* AUTOSAR standard types */

#include <Crc_Version.h> /* This modules' version declarations */

/* !LINKSTO SWS_Crc_00022,1 */
#include <Crc_Cfg.h>     /* Generated module configuration */

/*==================[macros]=================================================*/

#if (defined CRC_GET_VERSION_INFO_API_ID)
#error CRC_GET_VERSION_INFO_API_ID already defined
#endif
/** \brief Service ID for Crc_GetVersionInfo. */
#define CRC_GET_VERSION_INFO_API_ID           0x04U

#if (defined CRC_E_PARAM_DATA)
#error CRC_E_PARAM_DATA already defined
#endif
/** \brief DET Error: Invalid function parameter. */
#define CRC_E_PARAM_DATA        1U

#if (defined CRC_INSTANCE_ID)
#error CRC_INSTANCE_ID already defined
#endif
/** \brief Module's instance ID. */
#define CRC_INSTANCE_ID              0U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define CRC_START_SEC_CODE

/* !LINKSTO SWS_Crc_00022,1 */
#include <Crc_MemMap.h>

/** \brief Return the modules version information
 **
 ** This function provides the information to module vendor ID, module ID and
 ** software version major.minor.patch
 **
 **
 ** \param[out] VersionInfoPtr Pointer to struct to be filled with the version
 ** information
 **
 ** \ServiceID{4}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, CRC_CODE) Crc_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRC_APPL_DATA) VersionInfoPtr
);
#if (CRC_8_ENABLED == STD_ON)

/** \brief Calculation of CRC8
 **
 ** This function performs the calculation of a 8-bit SAE J1850 CRC value
 ** (runtime or table variant, depending on Configuration Parameter Crc8Mode)
 ** over the memory block referenced by \p Crc_DataPtr of byte length \p
 ** Crc_Length.
 **
 ** \param[in] Crc_DataPtr Valid pointer to start address of data block
 ** \param[in] Crc_Length  Length of data block in bytes
 ** \param[in] Crc_StartValue8 Initial Value
 ** \param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore Crc_StartValue8. FALSE:
 ** Subsequent call in a call sequence; Crc_StartValue8 is interpreted to be
 ** the return value of the previous function call.
 ** \return 8 bit result of CRC calculation.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint8, CRC_CODE) Crc_CalculateCRC8
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint8                                    Crc_StartValue8,
  boolean                                  Crc_IsFirstCall
);

#endif

#if (CRC_8H2F_ENABLED == STD_ON)

/** \brief Calculation of CRC8H2F
 **
 ** This function performs the calculation of a 8-bit CRC with polynom 0x2F
 ** (runtime or table variant, depending on Configuration Parameter
 ** Crc8H2FMode) over the memory block referenced by \p Crc_DataPtr of byte
 ** length \p Crc_Length.
 **
 ** \param[in] Crc_DataPtr Valid pointer to start address of data block
 ** \param[in] Crc_Length  Length of data block in bytes
 ** \param[in] Crc_StartValue8H2F Initial Value
 ** \param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore \p
 ** Crc_StartValue8H2F. FALSE: Subsequent call in a call sequence; \p
 ** Crc_StartValue8H2F is interpreted to be the return value of the previous
 ** function call.
 ** \return 8 bit result of CRC calculation.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint8, CRC_CODE) Crc_CalculateCRC8H2F
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint8                                    Crc_StartValue8H2F,
  boolean                                  Crc_IsFirstCall
);

#endif

#if (CRC_16_ENABLED == STD_ON)

/** \brief Calculation of CRC16
 **
 ** This function performs the calculation of a CRC16 value (runtime or table
 ** variant, depending on Configuration Parameter Crc16Mode) over the memory
 ** block referenced by \p Crc_DataPtr of byte length \p Crc_Length.
 **
 ** \param[in] Crc_DataPtr Valid pointer to start address of data block
 ** \param[in] Crc_Length  Length of data block in bytes
 ** \param[in] Crc_StartValue16  Initial Value
 ** \param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore \p
 ** Crc_StartValue16. FALSE: Subsequent call in a call sequence; \p
 ** Crc_StartValue16 is interpreted to be the return value of the previous
 ** function call.
 ** \return 16 bit result of CRC calculation
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint16, CRC_CODE) Crc_CalculateCRC16
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint16                                   Crc_StartValue16,
  boolean                                  Crc_IsFirstCall
);

#endif

#if (CRC_32_ENABLED == STD_ON)

/** \brief Calculation of CRC32
 **
 ** This function performs the calculation of a CRC32 value (runtime or table
 ** variant, depending on Configuration Parameter Crc32Mode) over the memory
 ** block referenced by \p Crc_DataPtr of byte length \p Crc_Length.
 **
 ** \param[in] Crc_DataPtr Valid pointer to start address of data block
 ** \param[in] Crc_Length  Length of data block in bytes
 ** \param[in] Crc_StartValue32  Initial Value
 ** \param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore Crc_StartValue32. FALSE:
 ** Subsequent call in a call sequence; Crc_StartValue32 is interpreted to be
 ** the return value of the previous function call.
 ** \return calculated CRC32 value
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint32, CRC_CODE) Crc_CalculateCRC32
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint32                                   Crc_StartValue32,
  boolean                                  Crc_IsFirstCall
);

#endif


#if (CRC_64_ENABLED == STD_ON)

/** \brief Calculation of CRC64
 **
 ** This function performs the calculation of a CRC64 value
 ** based on CRC-64-ECMA standard (runtime or table variant,
 ** depending on Configuration Parameter Crc64Mode) over the memory block
 ** referenced by \p Crc_DataPtr of byte length \p Crc_Length.
 **
 ** \param[in] Crc_DataPtr Valid pointer to start address of data block
 ** \param[in] Crc_Length  Length of data block in bytes
 ** \param[in] Crc_StartValue64  Initial Value
 ** \param[in] Crc_IsFirstCall TRUE: First call in a sequence or individual
 ** CRC calculation; start from initial value, ignore Crc_StartValue64. FALSE:
 ** Subsequent call in a call sequence; Crc_StartValue64 is interpreted to be
 ** the return value of the previous function call.
 ** \return calculated CRC64 value
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(uint64, CRC_CODE) Crc_CalculateCRC64
(
  P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataPtr,
  uint32                                   Crc_Length,
  uint64                                   Crc_StartValue64,
  boolean                                  Crc_IsFirstCall
);

#endif
#define CRC_STOP_SEC_CODE
#include <Crc_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CRC_H */
/*==================[end of file]============================================*/
