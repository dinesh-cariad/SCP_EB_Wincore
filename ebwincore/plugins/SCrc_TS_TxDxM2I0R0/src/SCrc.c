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

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in
 * a single function.
 *
 * Reason:
 * The AUTOSAR memory mapping requires that function are mapped to SEC_CODE
 * memory sections. Objects at block scope require a different mapping.
 * e.g. to a SEC_VAR section, which leads to nested memory sections, which is
 * not supported by some compilers.
 */

/*==================[inclusions]============================================*/

#include <SCrc.h>              /* Module public API                       */
#include <SCrc_CRC8.h>         /* public API for SCRC8 calculation        */
#include <SCrc_CRC8H2F.h>      /* public API for SCRC8H2F calculation     */
#include <SCrc_CRC16.h>        /* public API for SCRC16 calculation       */
#include <SCrc_CRC32P4.h>      /* public API for SCRC32P4 calculation     */

/*==================[macros]================================================*/

#if (defined SCRC_CRC8_INITIALVALUE) /* to prevent double declaration */
#error SCRC_CRC8_INITIALVALUE is already defined
#endif /* #if (defined SCRC_CRC8_INITIALVALUE) */

/** \brief Initial value used for Crc calculation with polynomial 0x11D (SAE-J1850) */
#define SCRC_CRC8_INITIALVALUE 0xFFU

#if (defined SCRC_CRC8_XORVALUE) /* to prevent double declaration */
#error SCRC_CRC8_XORVALUE is already defined
#endif /* #if (defined SCRC_CRC8_XORVALUE) */

/** \brief Xor-value for Crc calculation with polynomial 0x11D */
#define SCRC_CRC8_XORVALUE 0xFFU


#if (defined SCRC_CRC8H2F_INITIALVALUE) /* to prevent double declaration */
#error SCRC_CRC8H2F_INITIALVALUE is already defined
#endif /* #if (defined SCRC_CRC8H2F_INITIALVALUE) */

/** \brief Initial value used for Crc calculation with polynomial 0x12F */
#define SCRC_CRC8H2F_INITIALVALUE 0xFFU

#if (defined SCRC_CRC8H2F_XORVALUE) /* to prevent double declaration */
#error SCRC_CRC8H2F_XORVALUE is already defined
#endif /* #if (defined SCRC_CRC8H2F_XORVALUE) */

/** \brief Xor-value for Crc calculation with polynomial 0x12F */
#define SCRC_CRC8H2F_XORVALUE 0xFFU


#if (defined SCRC_CRC16_INITIALVALUE) /* to prevent double declaration */
#error SCRC_CRC16_INITIALVALUE is already defined
#endif /* #if (defined SCRC_CRC16_INITIALVALUE) */

/** \brief Initial value used for Crc calculation with polynomial 0x11021 */
#define SCRC_CRC16_INITIALVALUE 0xFFFFU

#if (defined SCRC_CRC16_XORVALUE) /* to prevent double declaration */
#error SCRC_CRC16_XORVALUE is already defined
#endif /* #if (defined SCRC_CRC16_XORVALUE) */

/** \brief Xor-value for Crc calculation with polynomial 0x11021 */
#define SCRC_CRC16_XORVALUE 0x0000U


#if (defined SCRC_CRC32P4_INITIALVALUE) /* to prevent double declaration */
#error SCRC_CRC32P4_INITIALVALUE is already defined
#endif /* #if (defined SCRC_CRC32P4_INITIALVALUE) */

/** \brief Initial value used for Crc calculation with polynomial 0x1F4ACFB13 */
#define SCRC_CRC32P4_INITIALVALUE 0xFFFFFFFFU

#if (defined SCRC_CRC32P4_XORVALUE) /* to prevent double declaration */
#error SCRC_CRC32P4_XORVALUE is already defined
#endif /* #if (defined SCRC_CRC32P4_XORVALUE) */

/** \brief Xor-value for Crc calculation with polynomial 0x1F4ACFB13 */
#define SCRC_CRC32P4_XORVALUE 0xFFFFFFFFU

/*==================[type definitions]======================================*/

#if !((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON))

/** \brief Dummy type definition in case SCrc.c contains no source code */
typedef void SCrcPreventEmptyTranslationUnitType;

#endif /* !((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

#if ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON))

#define SCRC_START_SEC_CONST_8BIT
#define SCRC_START_SEC_CONST_8
#include <SCrc_MemMap.h>

#endif /* ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)) */

#if (SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON)

/* Table of pre-computed values for CRC8 */
/* Deviation MISRAC2012-1 */
static CONST(uint8, SCRC_CONST) SCrc_Table8[256] =
{
    0x00U, 0x1DU, 0x3AU, 0x27U, 0x74U, 0x69U, 0x4EU, 0x53U, 0xE8U, 0xF5U, 0xD2U,
    0xCFU, 0x9CU, 0x81U, 0xA6U, 0xBBU,
    0xCDU, 0xD0U, 0xF7U, 0xEAU, 0xB9U, 0xA4U, 0x83U, 0x9EU, 0x25U, 0x38U, 0x1FU,
    0x02U, 0x51U, 0x4CU, 0x6BU, 0x76U, 0x87U, 0x9AU, 0xBDU, 0xA0U, 0xF3U, 0xEEU,
    0xC9U, 0xD4U, 0x6FU, 0x72U, 0x55U, 0x48U, 0x1BU, 0x06U, 0x21U, 0x3CU, 0x4AU,
    0x57U, 0x70U, 0x6DU, 0x3EU, 0x23U, 0x04U, 0x19U, 0xA2U, 0xBFU, 0x98U, 0x85U,
    0xD6U, 0xCBU, 0xECU, 0xF1U, 0x13U, 0x0EU, 0x29U, 0x34U, 0x67U, 0x7AU, 0x5DU,
    0x40U, 0xFBU, 0xE6U, 0xC1U, 0xDCU, 0x8FU, 0x92U, 0xB5U, 0xA8U, 0xDEU, 0xC3U,
    0xE4U, 0xF9U, 0xAAU, 0xB7U, 0x90U, 0x8DU, 0x36U, 0x2BU, 0x0CU, 0x11U, 0x42U,
    0x5FU, 0x78U, 0x65U, 0x94U, 0x89U, 0xAEU, 0xB3U, 0xE0U, 0xFDU, 0xDAU, 0xC7U,
    0x7CU, 0x61U, 0x46U, 0x5BU, 0x08U, 0x15U, 0x32U, 0x2FU, 0x59U, 0x44U, 0x63U,
    0x7EU, 0x2DU, 0x30U, 0x17U, 0x0AU, 0xB1U, 0xACU, 0x8BU, 0x96U, 0xC5U, 0xD8U,
    0xFFU, 0xE2U, 0x26U, 0x3BU, 0x1CU, 0x01U, 0x52U, 0x4FU, 0x68U, 0x75U, 0xCEU,
    0xD3U, 0xF4U, 0xE9U, 0xBAU, 0xA7U, 0x80U, 0x9DU, 0xEBU, 0xF6U, 0xD1U, 0xCCU,
    0x9FU, 0x82U, 0xA5U, 0xB8U, 0x03U, 0x1EU, 0x39U, 0x24U, 0x77U, 0x6AU, 0x4DU,
    0x50U, 0xA1U, 0xBCU, 0x9BU, 0x86U, 0xD5U, 0xC8U, 0xEFU, 0xF2U, 0x49U, 0x54U,
    0x73U, 0x6EU, 0x3DU, 0x20U, 0x07U, 0x1AU, 0x6CU, 0x71U, 0x56U, 0x4BU, 0x18U,
    0x05U, 0x22U, 0x3FU, 0x84U, 0x99U, 0xBEU, 0xA3U, 0xF0U, 0xEDU, 0xCAU, 0xD7U,
    0x35U, 0x28U, 0x0FU, 0x12U, 0x41U, 0x5CU, 0x7BU, 0x66U, 0xDDU, 0xC0U, 0xE7U,
    0xFAU, 0xA9U, 0xB4U, 0x93U, 0x8EU, 0xF8U, 0xE5U, 0xC2U, 0xDFU, 0x8CU, 0x91U,
    0xB6U, 0xABU, 0x10U, 0x0DU, 0x2AU, 0x37U, 0x64U, 0x79U, 0x5EU, 0x43U, 0xB2U,
    0xAFU, 0x88U, 0x95U, 0xC6U, 0xDBU, 0xFCU, 0xE1U, 0x5AU, 0x47U, 0x60U, 0x7DU,
    0x2EU, 0x33U, 0x14U, 0x09U, 0x7FU, 0x62U, 0x45U, 0x58U, 0x0BU, 0x16U, 0x31U,
    0x2CU, 0x97U, 0x8AU, 0xADU, 0xB0U, 0xE3U, 0xFEU, 0xD9U, 0xC4U
};

#endif /* SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON */

#if (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)

/* Table of pre-computed values for CRC8H2F */
/* Deviation MISRAC2012-1 */
static CONST(uint8, SCRC_CONST) SCrc_Table8H2F[256] =
{
    0x00U, 0x2FU, 0x5EU, 0x71U, 0xBCU, 0x93U, 0xE2U, 0xCDU, 0x57U, 0x78U, 0x09U,
    0x26U, 0xEBU, 0xC4U, 0xB5U, 0x9AU,
    0xAEU, 0x81U, 0xF0U, 0xDFU, 0x12U, 0x3DU, 0x4CU, 0x63U, 0xF9U, 0xD6U, 0xA7U,
    0x88U, 0x45U, 0x6AU, 0x1BU, 0x34U, 0x73U, 0x5CU, 0x2DU, 0x02U, 0xCFU, 0xE0U,
    0x91U, 0xBEU, 0x24U, 0x0BU, 0x7AU, 0x55U, 0x98U, 0xB7U, 0xC6U, 0xE9U, 0xDDU,
    0xF2U, 0x83U, 0xACU, 0x61U, 0x4EU, 0x3FU, 0x10U, 0x8AU, 0xA5U, 0xD4U, 0xFBU,
    0x36U, 0x19U, 0x68U, 0x47U, 0xE6U, 0xC9U, 0xB8U, 0x97U, 0x5AU, 0x75U, 0x04U,
    0x2BU, 0xB1U, 0x9EU, 0xEFU, 0xC0U, 0x0DU, 0x22U, 0x53U, 0x7CU, 0x48U, 0x67U,
    0x16U, 0x39U, 0xF4U, 0xDBU, 0xAAU, 0x85U, 0x1FU, 0x30U, 0x41U, 0x6EU, 0xA3U,
    0x8CU, 0xFDU, 0xD2U, 0x95U, 0xBAU, 0xCBU, 0xE4U, 0x29U, 0x06U, 0x77U, 0x58U,
    0xC2U, 0xEDU, 0x9CU, 0xB3U, 0x7EU, 0x51U, 0x20U, 0x0FU, 0x3BU, 0x14U, 0x65U,
    0x4AU, 0x87U, 0xA8U, 0xD9U, 0xF6U, 0x6CU, 0x43U, 0x32U, 0x1DU, 0xD0U, 0xFFU,
    0x8EU, 0xA1U, 0xE3U, 0xCCU, 0xBDU, 0x92U, 0x5FU, 0x70U, 0x01U, 0x2EU, 0xB4U,
    0x9BU, 0xEAU, 0xC5U, 0x08U, 0x27U, 0x56U, 0x79U, 0x4DU, 0x62U, 0x13U, 0x3CU,
    0xF1U, 0xDEU, 0xAFU, 0x80U, 0x1AU, 0x35U, 0x44U, 0x6BU, 0xA6U, 0x89U, 0xF8U,
    0xD7U, 0x90U, 0xBFU, 0xCEU, 0xE1U, 0x2CU, 0x03U, 0x72U, 0x5DU, 0xC7U, 0xE8U,
    0x99U, 0xB6U, 0x7BU, 0x54U, 0x25U, 0x0AU, 0x3EU, 0x11U, 0x60U, 0x4FU, 0x82U,
    0xADU, 0xDCU, 0xF3U, 0x69U, 0x46U, 0x37U, 0x18U, 0xD5U, 0xFAU, 0x8BU, 0xA4U,
    0x05U, 0x2AU, 0x5BU, 0x74U, 0xB9U, 0x96U, 0xE7U, 0xC8U, 0x52U, 0x7DU, 0x0CU,
    0x23U, 0xEEU, 0xC1U, 0xB0U, 0x9FU, 0xABU, 0x84U, 0xF5U, 0xDAU, 0x17U, 0x38U,
    0x49U, 0x66U, 0xFCU, 0xD3U, 0xA2U, 0x8DU, 0x40U, 0x6FU, 0x1EU, 0x31U, 0x76U,
    0x59U, 0x28U, 0x07U, 0xCAU, 0xE5U, 0x94U, 0xBBU, 0x21U, 0x0EU, 0x7FU, 0x50U,
    0x9DU, 0xB2U, 0xC3U, 0xECU, 0xD8U, 0xF7U, 0x86U, 0xA9U, 0x64U, 0x4BU, 0x3AU,
    0x15U, 0x8FU, 0xA0U, 0xD1U, 0xFEU, 0x33U, 0x1CU, 0x6DU, 0x42U,
};

#endif /* SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON */

#if ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON))

#define SCRC_STOP_SEC_CONST_8BIT
#define SCRC_STOP_SEC_CONST_8
#include <SCrc_MemMap.h>
#undef SCRC_START_SEC_CONST_8BIT
#undef SCRC_START_SEC_CONST_8
#undef SCRC_STOP_SEC_CONST_8BIT
#undef SCRC_STOP_SEC_CONST_8

#endif /* #if ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)) */


#if (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON)

#define SCRC_START_SEC_CONST_16BIT
#define SCRC_START_SEC_CONST_16
#include <SCrc_MemMap.h>

/* Table of pre-computed values for CRC16. Used Polynomial is 0x1021 */
/* Deviation MISRAC2012-1 */
static CONST(uint16, SCRC_CONST) SCrc_Table16[256] =
{
  0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
  0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
  0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
  0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
  0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
  0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
  0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
  0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
  0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
  0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
  0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
  0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
  0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
  0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
  0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
  0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
  0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
  0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
  0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
  0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
  0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
  0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
  0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
  0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
  0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
  0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
  0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
  0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
  0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
  0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
  0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
  0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
};

#define SCRC_STOP_SEC_CONST_16BIT
#define SCRC_STOP_SEC_CONST_16
#include <SCrc_MemMap.h>
#undef SCRC_START_SEC_CONST_16BIT
#undef SCRC_START_SEC_CONST_16
#undef SCRC_STOP_SEC_CONST_16BIT
#undef SCRC_STOP_SEC_CONST_16

#endif /* (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) */

#if (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON)

#define SCRC_START_SEC_CONST_32BIT
#define SCRC_START_SEC_CONST_32
#include <SCrc_MemMap.h>

/* Table of pre-computed values for CRC32P4.
   Used Polynomial is 0x1F4ACFB13. */
/* Deviation MISRAC2012-1 */
static CONST(uint32, SCRC_CONST) SCrc_Table32P4[256] =
{
  0x00000000U, 0x30850FF5U, 0x610A1FEAU, 0x518F101FU,
  0xC2143FD4U, 0xF2913021U, 0xA31E203EU, 0x939B2FCBU,
  0x159615F7U, 0x25131A02U, 0x749C0A1DU, 0x441905E8U,
  0xD7822A23U, 0xE70725D6U, 0xB68835C9U, 0x860D3A3CU,
  0x2B2C2BEEU, 0x1BA9241BU, 0x4A263404U, 0x7AA33BF1U,
  0xE938143AU, 0xD9BD1BCFU, 0x88320BD0U, 0xB8B70425U,
  0x3EBA3E19U, 0x0E3F31ECU, 0x5FB021F3U, 0x6F352E06U,
  0xFCAE01CDU, 0xCC2B0E38U, 0x9DA41E27U, 0xAD2111D2U,
  0x565857DCU, 0x66DD5829U, 0x37524836U, 0x07D747C3U,
  0x944C6808U, 0xA4C967FDU, 0xF54677E2U, 0xC5C37817U,
  0x43CE422BU, 0x734B4DDEU, 0x22C45DC1U, 0x12415234U,
  0x81DA7DFFU, 0xB15F720AU, 0xE0D06215U, 0xD0556DE0U,
  0x7D747C32U, 0x4DF173C7U, 0x1C7E63D8U, 0x2CFB6C2DU,
  0xBF6043E6U, 0x8FE54C13U, 0xDE6A5C0CU, 0xEEEF53F9U,
  0x68E269C5U, 0x58676630U, 0x09E8762FU, 0x396D79DAU,
  0xAAF65611U, 0x9A7359E4U, 0xCBFC49FBU, 0xFB79460EU,
  0xACB0AFB8U, 0x9C35A04DU, 0xCDBAB052U, 0xFD3FBFA7U,
  0x6EA4906CU, 0x5E219F99U, 0x0FAE8F86U, 0x3F2B8073U,
  0xB926BA4FU, 0x89A3B5BAU, 0xD82CA5A5U, 0xE8A9AA50U,
  0x7B32859BU, 0x4BB78A6EU, 0x1A389A71U, 0x2ABD9584U,
  0x879C8456U, 0xB7198BA3U, 0xE6969BBCU, 0xD6139449U,
  0x4588BB82U, 0x750DB477U, 0x2482A468U, 0x1407AB9DU,
  0x920A91A1U, 0xA28F9E54U, 0xF3008E4BU, 0xC38581BEU,
  0x501EAE75U, 0x609BA180U, 0x3114B19FU, 0x0191BE6AU,
  0xFAE8F864U, 0xCA6DF791U, 0x9BE2E78EU, 0xAB67E87BU,
  0x38FCC7B0U, 0x0879C845U, 0x59F6D85AU, 0x6973D7AFU,
  0xEF7EED93U, 0xDFFBE266U, 0x8E74F279U, 0xBEF1FD8CU,
  0x2D6AD247U, 0x1DEFDDB2U, 0x4C60CDADU, 0x7CE5C258U,
  0xD1C4D38AU, 0xE141DC7FU, 0xB0CECC60U, 0x804BC395U,
  0x13D0EC5EU, 0x2355E3ABU, 0x72DAF3B4U, 0x425FFC41U,
  0xC452C67DU, 0xF4D7C988U, 0xA558D997U, 0x95DDD662U,
  0x0646F9A9U, 0x36C3F65CU, 0x674CE643U, 0x57C9E9B6U,
  0xC8DF352FU, 0xF85A3ADAU, 0xA9D52AC5U, 0x99502530U,
  0x0ACB0AFBU, 0x3A4E050EU, 0x6BC11511U, 0x5B441AE4U,
  0xDD4920D8U, 0xEDCC2F2DU, 0xBC433F32U, 0x8CC630C7U,
  0x1F5D1F0CU, 0x2FD810F9U, 0x7E5700E6U, 0x4ED20F13U,
  0xE3F31EC1U, 0xD3761134U, 0x82F9012BU, 0xB27C0EDEU,
  0x21E72115U, 0x11622EE0U, 0x40ED3EFFU, 0x7068310AU,
  0xF6650B36U, 0xC6E004C3U, 0x976F14DCU, 0xA7EA1B29U,
  0x347134E2U, 0x04F43B17U, 0x557B2B08U, 0x65FE24FDU,
  0x9E8762F3U, 0xAE026D06U, 0xFF8D7D19U, 0xCF0872ECU,
  0x5C935D27U, 0x6C1652D2U, 0x3D9942CDU, 0x0D1C4D38U,
  0x8B117704U, 0xBB9478F1U, 0xEA1B68EEU, 0xDA9E671BU,
  0x490548D0U, 0x79804725U, 0x280F573AU, 0x188A58CFU,
  0xB5AB491DU, 0x852E46E8U, 0xD4A156F7U, 0xE4245902U,
  0x77BF76C9U, 0x473A793CU, 0x16B56923U, 0x263066D6U,
  0xA03D5CEAU, 0x90B8531FU, 0xC1374300U, 0xF1B24CF5U,
  0x6229633EU, 0x52AC6CCBU, 0x03237CD4U, 0x33A67321U,
  0x646F9A97U, 0x54EA9562U, 0x0565857DU, 0x35E08A88U,
  0xA67BA543U, 0x96FEAAB6U, 0xC771BAA9U, 0xF7F4B55CU,
  0x71F98F60U, 0x417C8095U, 0x10F3908AU, 0x20769F7FU,
  0xB3EDB0B4U, 0x8368BF41U, 0xD2E7AF5EU, 0xE262A0ABU,
  0x4F43B179U, 0x7FC6BE8CU, 0x2E49AE93U, 0x1ECCA166U,
  0x8D578EADU, 0xBDD28158U, 0xEC5D9147U, 0xDCD89EB2U,
  0x5AD5A48EU, 0x6A50AB7BU, 0x3BDFBB64U, 0x0B5AB491U,
  0x98C19B5AU, 0xA84494AFU, 0xF9CB84B0U, 0xC94E8B45U,
  0x3237CD4BU, 0x02B2C2BEU, 0x533DD2A1U, 0x63B8DD54U,
  0xF023F29FU, 0xC0A6FD6AU, 0x9129ED75U, 0xA1ACE280U,
  0x27A1D8BCU, 0x1724D749U, 0x46ABC756U, 0x762EC8A3U,
  0xE5B5E768U, 0xD530E89DU, 0x84BFF882U, 0xB43AF777U,
  0x191BE6A5U, 0x299EE950U, 0x7811F94FU, 0x4894F6BAU,
  0xDB0FD971U, 0xEB8AD684U, 0xBA05C69BU, 0x8A80C96EU,
  0x0C8DF352U, 0x3C08FCA7U, 0x6D87ECB8U, 0x5D02E34DU,
  0xCE99CC86U, 0xFE1CC373U, 0xAF93D36CU, 0x9F16DC99U
};

#define SCRC_STOP_SEC_CONST_32BIT
#define SCRC_STOP_SEC_CONST_32
#include <SCrc_MemMap.h>
#undef SCRC_START_SEC_CONST_32BIT
#undef SCRC_START_SEC_CONST_32
#undef SCRC_STOP_SEC_CONST_32BIT
#undef SCRC_STOP_SEC_CONST_32

#endif /* (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON) */

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON))

#define SCRC_START_SEC_CODE
#include <SCrc_MemMap.h>

#endif /* ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON)) */

#if (SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON)

FUNC(uint8, SCRC_CODE) SCrc_CalculateCRC8
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint8                                     SCrc_StartValue8,
    boolean                                   SCrc_IsFirstCall
)
{
    uint32 i;

    /* Check if this is the first call in a sequence or individual CRC calculation.
     * If so, then use the defined initial value. Otherwise, the start value
     * is interpreted as the return value of the previous function call.
     */
    if (FALSE == SCrc_IsFirstCall)
    {
        SCrc_StartValue8 = (uint8)(SCrc_StartValue8 ^ SCRC_CRC8_XORVALUE);
    }
    else
    {
        SCrc_StartValue8 = SCRC_CRC8_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i<SCrc_Length; ++i)
    {
        SCrc_StartValue8 = SCrc_Table8[(uint8)(SCrc_StartValue8 ^ SCrc_DataPtr[i])];
    }

    /* Apply xor-value of 0xFF as specified in ASR R4.0 CRC SWS. */
    return (uint8)(SCrc_StartValue8 ^ SCRC_CRC8_XORVALUE);
}

#endif /* (SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) */

#if (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON)

FUNC(uint8, SCRC_CODE) SCrc_CalculateCRC8H2F
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint8                                     SCrc_StartValue8H2F,
    boolean                                   SCrc_IsFirstCall
)
{
    uint32 i;

    /* Check if this is the first call in a sequence or individual CRC calculation.
     * If so, then use the defined initial value. Otherwise, the start value
     * is interpreted as the return value of the previous function call.
     */
    if (FALSE == SCrc_IsFirstCall)
    {
        SCrc_StartValue8H2F = (uint8)(SCrc_StartValue8H2F ^ SCRC_CRC8H2F_XORVALUE);
    }
    else
    {
        SCrc_StartValue8H2F = SCRC_CRC8H2F_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i<SCrc_Length; ++i)
    {
        SCrc_StartValue8H2F = SCrc_Table8H2F[(uint8)(SCrc_StartValue8H2F ^ SCrc_DataPtr[i])];
    }

    /* Apply xor-value of 0xFF as specified in ASR R4.0 CRC SWS. */
    return (uint8)(SCrc_StartValue8H2F ^ SCRC_CRC8H2F_XORVALUE);
}

#endif /* (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) */

#if (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON)

FUNC(uint16, SCRC_CODE) SCrc_CalculateCRC16
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint16                                    SCrc_StartValue16,
    boolean                                   SCrc_IsFirstCall
)
{
    uint32 i;

    /* Check if this is the first call in a sequence or individual CRC calculation.
     * If so, then use the defined initial value. Otherwise, the start value
     * is interpreted as the return value of the previous function call.
     */
    if (TRUE == SCrc_IsFirstCall)
    {
        SCrc_StartValue16 = SCRC_CRC16_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i<SCrc_Length; ++i)
    {
        /* Process one byte of data */
        SCrc_StartValue16 =
                SCrc_Table16[((uint8)(SCrc_StartValue16 >> 8U)) ^ SCrc_DataPtr[i]]
                ^ ((uint16)((SCrc_StartValue16 << 8U) & 0xFFFFU));
    }

    /* specified final XOR value for CRC16 is 0, no need to actually xor anything here */
    return SCrc_StartValue16;
}

#endif /* (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) */

#if (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON)

FUNC(uint32, SCRC_CODE) SCrc_CalculateCRC32P4
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint32                                    SCrc_StartValue32P4,
    boolean                                   SCrc_IsFirstCall
)
{
    uint32 i;

    /* Check if this is the first call in a sequence or individual CRC calculation.
     * If so, then use the defined initial value. Otherwise, the start value
     * is interpreted as the return value of the previous function call.
     */
    if (FALSE == SCrc_IsFirstCall)
    {
        SCrc_StartValue32P4 = (uint32)(SCrc_StartValue32P4 ^ SCRC_CRC32P4_XORVALUE);
    }
    else
    {
        SCrc_StartValue32P4 = SCRC_CRC32P4_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i<SCrc_Length; ++i)
    {
        /* Process one byte of data */
        SCrc_StartValue32P4
          = (SCrc_Table32P4[((uint8)(SCrc_StartValue32P4 & 0xFFU)) ^ SCrc_DataPtr[i]]
            ^ (SCrc_StartValue32P4 >> 8U));
    }

    /* Apply xor-value of 0xFFFFFFFF */
    return (uint32)(SCrc_StartValue32P4 ^ SCRC_CRC32P4_XORVALUE);
}

#endif /* (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON) */

#if ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON))

#define SCRC_STOP_SEC_CODE
#include <SCrc_MemMap.h>

#endif /* ((SCRC_FUNCENABLED_SCRC_CRC8 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC8H2F == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC16 == STD_ON) || \
     (SCRC_FUNCENABLED_SCRC_CRC32P4 == STD_ON)) */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
