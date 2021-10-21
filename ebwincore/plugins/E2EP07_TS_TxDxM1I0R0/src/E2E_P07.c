/**
 * \file
 *
 * \brief AUTOSAR E2EP07
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP07.
 *
 * \version 1.0.12
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
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

/*==================[inclusions]=============================================*/

#include <E2E_P07.h>       /* E2E Profile 7         */

/*==================[macros]=================================================*/
#if (defined SCRC_CRC64P7_INITIALVALUE) /* to prevent double declaration */
#error SCRC_CRC64P7_INITIALVALUE is already defined
#endif /* #if (defined SCRC_CRC64P7_INITIALVALUE) */

/** \brief Initial value used for Crc calculation with polynomial 0x1F4ACFB13 */
#define SCRC_CRC64P7_INITIALVALUE 0xFFFFFFFFFFFFFFFFULL

#if (defined SCRC_CRC64P7_XORVALUE) /* to prevent double declaration */
#error SCRC_CRC64P7_XORVALUE is already defined
#endif /* #if (defined SCRC_CRC64P7_XORVALUE) */

/** \brief Xor-value for Crc calculation with polynomial 0x1F4ACFB13 */
#define SCRC_CRC64P7_XORVALUE 0xFFFFFFFFFFFFFFFFULL

#if (defined E2EP07_EB_CRC_STARTVALUE) /* to prevent double declaration */
#error E2EP07_EB_CRC_STARTVALUE is already defined
#endif /* #if (defined E2EP07_EB_CRC_STARTVALUE) */

/** \brief Start value for Crc calculation */
#define E2EP07_EB_CRC_STARTVALUE (uint64)0xFFFFFFFFFFFFFFFFULL

#if (defined E2EP07_EB_COUNTER_MAXVALUE) /* to prevent double declaration */
#error E2EP07_EB_COUNTER_MAXVALUE is already defined
#endif /* #if (defined E2EP07_EB_COUNTER_MAXVALUE) */

/** \brief Maximum value for state counter is 32bits*/
#define E2EP07_EB_COUNTER_MAXVALUE 0xFFFFFFFFUL

#if (defined E2EP07_EB_E2EHEADER_LENGTH_OFFSET) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_LENGTH_OFFSET is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_LENGTH_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of Length coded. */
#define E2EP07_EB_E2EHEADER_LENGTH_OFFSET 8U

#if (defined E2EP07_EB_E2EHEADER_COUNTER_OFFSET) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_COUNTER_OFFSET is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_COUNTER_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of Counter coded. */
#define E2EP07_EB_E2EHEADER_COUNTER_OFFSET 12U

#if (defined E2EP07_EB_E2EHEADER_DATAID_OFFSET) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_DATAID_OFFSET is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_DATAID_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of DataID coded. */
#define E2EP07_EB_E2EHEADER_DATAID_OFFSET 16U

#if (defined E2EP07_EB_E2EHEADER_CRC_OFFSET) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_CRC_OFFSET is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_CRC_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of CRC coded. */
#define E2EP07_EB_E2EHEADER_CRC_OFFSET 0U

#if (defined E2EP07_EB_E2EHEADER_CRC_SIZE) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_CRC_SIZE is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_CRC_SIZE) */

/** \brief Crc size in bytes within E2E header. */
#define E2EP07_EB_E2EHEADER_CRC_SIZE 8U

#if (defined E2EP07_EB_E2EHEADER_SIZE_BITS) /* to prevent double declaration */
#error E2EP07_EB_E2EHEADER_SIZE_BITS is already defined
#endif /* #if (defined E2EP07_EB_E2EHEADER_SIZE_BITS) */

/** \brief E2E Profile 07 header size in bits. */
#define E2EP07_EB_E2EHEADER_SIZE_BITS 160U

#if (defined E2EP07_EB_WRITE8BYTE_VALUE) /* to prevent double declaration */
#error E2EP07_EB_WRITE8BYTE_VALUE is already defined
#endif /* #if (defined E2EP07_EB_WRITE8BYTE_VALUE) */

/** \brief Write a 8-byte value in Big Endian order to the passed Data Pointer */
#define E2EP07_EB_WRITE8BYTE_VALUE(DataPtr, Value)                          \
do {                                                                        \
    (DataPtr)[0] = (uint8)((uint64)((Value) >> 56U) & (uint8)0xFFU);        \
    (DataPtr)[1] = (uint8)((uint64)((Value) >> 48U) & (uint8)0xFFU);        \
    (DataPtr)[2] = (uint8)((uint64)((Value) >> 40U) & (uint8)0xFFU);        \
    (DataPtr)[3] = (uint8)((uint64)((Value) >> 32U) & (uint8)0xFFU);        \
    (DataPtr)[4] = (uint8)((uint64)((Value) >> 24U) & (uint8)0xFFU);        \
    (DataPtr)[5] = (uint8)((uint64)((Value) >> 16U) & (uint8)0xFFU);        \
    (DataPtr)[6] = (uint8)((uint64)((Value) >>  8U) & (uint8)0xFFU);        \
    (DataPtr)[7] = (uint8)((Value) & (uint8)0xFFU);                         \
} while (0)

#if (defined E2EP07_EB_READ4BYTE_VALUE) /* to prevent double declaration */
#error E2EP07_EB_READ4BYTE_VALUE is already defined
#endif /* #if (defined E2EP07_EB_READ4BYTE_VALUE) */

/** \brief Read a 4-byte value in Big Endian order from the passed Data Pointer */
#define E2EP07_EB_READ4BYTE_VALUE(DataPtr)                                  \
    (                                                                       \
      (uint32)(((uint32)((DataPtr)[3]))                                     \
    | (uint32)(((uint32)(((uint32)((DataPtr)[2]))<<8U ))                    \
    | (uint32)(((uint32)(((uint32)((DataPtr)[1]))<<16U))                    \
    | ((uint32)(((uint32)((DataPtr)[0]))<<24U)))))                          \
    )                                                                       \

#if (defined E2EP07_EB_READ8BYTE_VALUE) /* to prevent double declaration */
#error E2EP07_EB_READ8BYTE_VALUE is already defined
#endif /* #if (defined E2EP07_EB_READ8BYTE_VALUE) */

/** \brief Read a 8-byte value in Big Endian order from the passed Data Pointer */
#define E2EP07_EB_READ8BYTE_VALUE(DataPtr)                                  \
    (                                                                       \
      (uint64)(((uint64)((DataPtr)[7]))                                     \
    | (uint64)(((uint64)(((uint64)((DataPtr)[6])) <<8U))                    \
    | (uint64)(((uint64)(((uint64)((DataPtr)[5]))<<16U))                    \
    | (uint64)(((uint64)(((uint64)((DataPtr)[4]))<<24U))                    \
    | (uint64)(((uint64)(((uint64)((DataPtr)[3]))<<32U))                    \
    | (uint64)(((uint64)(((uint64)((DataPtr)[2]))<<40U))                    \
    | (uint64)(((uint64)(((uint64)((DataPtr)[1]))<<48U))                    \
    | ((uint64)(((uint64)((DataPtr)[0]))<<56U)))))))))                      \
    )

/*==================[type definitions]=======================================*/

#define E2E_START_SEC_CONST_UNSPECIFIED
#include <E2E_MemMap.h>


/* Table of pre-computed values for CRC64_P7.
   Used Polynomial is 0x42F0E1EBA9EA3693. */
/* Deviation MISRAC2012-1 */
static CONST(uint64, E2E_CONST) SCrc_Table64_P7[256] =
{
 0x0000000000000000ULL,   0xB32E4CBE03A75F6FULL,   0xF4843657A840A05BULL,   0x47AA7AE9ABE7FF34ULL,
 0x7BD0C384FF8F5E33ULL,   0xC8FE8F3AFC28015CULL,   0x8F54F5D357CFFE68ULL,   0x3C7AB96D5468A107ULL,
 0xF7A18709FF1EBC66ULL,   0x448FCBB7FCB9E309ULL,   0x0325B15E575E1C3DULL,   0xB00BFDE054F94352ULL,
 0x8C71448D0091E255ULL,   0x3F5F08330336BD3AULL,   0x78F572DAA8D1420EULL,   0xCBDB3E64AB761D61ULL,
 0x7D9BA13851336649ULL,   0xCEB5ED8652943926ULL,   0x891F976FF973C612ULL,   0x3A31DBD1FAD4997DULL,
 0x064B62BCAEBC387AULL,   0xB5652E02AD1B6715ULL,   0xF2CF54EB06FC9821ULL,   0x41E11855055BC74EULL,
 0x8A3A2631AE2DDA2FULL,   0x39146A8FAD8A8540ULL,   0x7EBE1066066D7A74ULL,   0xCD905CD805CA251BULL,
 0xF1EAE5B551A2841CULL,   0x42C4A90B5205DB73ULL,   0x056ED3E2F9E22447ULL,   0xB6409F5CFA457B28ULL,
 0xFB374270A266CC92ULL,   0x48190ECEA1C193FDULL,   0x0FB374270A266CC9ULL,   0xBC9D3899098133A6ULL,
 0x80E781F45DE992A1ULL,   0x33C9CD4A5E4ECDCEULL,   0x7463B7A3F5A932FAULL,   0xC74DFB1DF60E6D95ULL,
 0x0C96C5795D7870F4ULL,   0xBFB889C75EDF2F9BULL,   0xF812F32EF538D0AFULL,   0x4B3CBF90F69F8FC0ULL,
 0x774606FDA2F72EC7ULL,   0xC4684A43A15071A8ULL,   0x83C230AA0AB78E9CULL,   0x30EC7C140910D1F3ULL,
 0x86ACE348F355AADBULL,   0x3582AFF6F0F2F5B4ULL,   0x7228D51F5B150A80ULL,   0xC10699A158B255EFULL,
 0xFD7C20CC0CDAF4E8ULL,   0x4E526C720F7DAB87ULL,   0x09F8169BA49A54B3ULL,   0xBAD65A25A73D0BDCULL,
 0x710D64410C4B16BDULL,   0xC22328FF0FEC49D2ULL,   0x85895216A40BB6E6ULL,   0x36A71EA8A7ACE989ULL,
 0x0ADDA7C5F3C4488EULL,   0xB9F3EB7BF06317E1ULL,   0xFE5991925B84E8D5ULL,   0x4D77DD2C5823B7BAULL,
 0x64B62BCAEBC387A1ULL,   0xD7986774E864D8CEULL,   0x90321D9D438327FAULL,   0x231C512340247895ULL,
 0x1F66E84E144CD992ULL,   0xAC48A4F017EB86FDULL,   0xEBE2DE19BC0C79C9ULL,   0x58CC92A7BFAB26A6ULL,
 0x9317ACC314DD3BC7ULL,   0x2039E07D177A64A8ULL,   0x67939A94BC9D9B9CULL,   0xD4BDD62ABF3AC4F3ULL,
 0xE8C76F47EB5265F4ULL,   0x5BE923F9E8F53A9BULL,   0x1C4359104312C5AFULL,   0xAF6D15AE40B59AC0ULL,
 0x192D8AF2BAF0E1E8ULL,   0xAA03C64CB957BE87ULL,   0xEDA9BCA512B041B3ULL,   0x5E87F01B11171EDCULL,
 0x62FD4976457FBFDBULL,   0xD1D305C846D8E0B4ULL,   0x96797F21ED3F1F80ULL,   0x2557339FEE9840EFULL,
 0xEE8C0DFB45EE5D8EULL,   0x5DA24145464902E1ULL,   0x1A083BACEDAEFDD5ULL,   0xA9267712EE09A2BAULL,
 0x955CCE7FBA6103BDULL,   0x267282C1B9C65CD2ULL,   0x61D8F8281221A3E6ULL,   0xD2F6B4961186FC89ULL,
 0x9F8169BA49A54B33ULL,   0x2CAF25044A02145CULL,   0x6B055FEDE1E5EB68ULL,   0xD82B1353E242B407ULL,
 0xE451AA3EB62A1500ULL,   0x577FE680B58D4A6FULL,   0x10D59C691E6AB55BULL,   0xA3FBD0D71DCDEA34ULL,
 0x6820EEB3B6BBF755ULL,   0xDB0EA20DB51CA83AULL,   0x9CA4D8E41EFB570EULL,   0x2F8A945A1D5C0861ULL,
 0x13F02D374934A966ULL,   0xA0DE61894A93F609ULL,   0xE7741B60E174093DULL,   0x545A57DEE2D35652ULL,
 0xE21AC88218962D7AULL,   0x5134843C1B317215ULL,   0x169EFED5B0D68D21ULL,   0xA5B0B26BB371D24EULL,
 0x99CA0B06E7197349ULL,   0x2AE447B8E4BE2C26ULL,   0x6D4E3D514F59D312ULL,   0xDE6071EF4CFE8C7DULL,
 0x15BB4F8BE788911CULL,   0xA6950335E42FCE73ULL,   0xE13F79DC4FC83147ULL,   0x521135624C6F6E28ULL,
 0x6E6B8C0F1807CF2FULL,   0xDD45C0B11BA09040ULL,   0x9AEFBA58B0476F74ULL,   0x29C1F6E6B3E0301BULL,
 0xC96C5795D7870F42ULL,   0x7A421B2BD420502DULL,   0x3DE861C27FC7AF19ULL,   0x8EC62D7C7C60F076ULL,
 0xB2BC941128085171ULL,   0x0192D8AF2BAF0E1EULL,   0x4638A2468048F12AULL,   0xF516EEF883EFAE45ULL,
 0x3ECDD09C2899B324ULL,   0x8DE39C222B3EEC4BULL,   0xCA49E6CB80D9137FULL,   0x7967AA75837E4C10ULL,
 0x451D1318D716ED17ULL,   0xF6335FA6D4B1B278ULL,   0xB199254F7F564D4CULL,   0x02B769F17CF11223ULL,
 0xB4F7F6AD86B4690BULL,   0x07D9BA1385133664ULL,   0x4073C0FA2EF4C950ULL,   0xF35D8C442D53963FULL,
 0xCF273529793B3738ULL,   0x7C0979977A9C6857ULL,   0x3BA3037ED17B9763ULL,   0x888D4FC0D2DCC80CULL,
 0x435671A479AAD56DULL,   0xF0783D1A7A0D8A02ULL,   0xB7D247F3D1EA7536ULL,   0x04FC0B4DD24D2A59ULL,
 0x3886B22086258B5EULL,   0x8BA8FE9E8582D431ULL,   0xCC0284772E652B05ULL,   0x7F2CC8C92DC2746AULL,
 0x325B15E575E1C3D0ULL,   0x8175595B76469CBFULL,   0xC6DF23B2DDA1638BULL,   0x75F16F0CDE063CE4ULL,
 0x498BD6618A6E9DE3ULL,   0xFAA59ADF89C9C28CULL,   0xBD0FE036222E3DB8ULL,   0x0E21AC88218962D7ULL,
 0xC5FA92EC8AFF7FB6ULL,   0x76D4DE52895820D9ULL,   0x317EA4BB22BFDFEDULL,   0x8250E80521188082ULL,
 0xBE2A516875702185ULL,   0x0D041DD676D77EEAULL,   0x4AAE673FDD3081DEULL,   0xF9802B81DE97DEB1ULL,
 0x4FC0B4DD24D2A599ULL,   0xFCEEF8632775FAF6ULL,   0xBB44828A8C9205C2ULL,   0x086ACE348F355AADULL,
 0x34107759DB5DFBAAULL,   0x873E3BE7D8FAA4C5ULL,   0xC094410E731D5BF1ULL,   0x73BA0DB070BA049EULL,
 0xB86133D4DBCC19FFULL,   0x0B4F7F6AD86B4690ULL,   0x4CE50583738CB9A4ULL,   0xFFCB493D702BE6CBULL,
 0xC3B1F050244347CCULL,   0x709FBCEE27E418A3ULL,   0x3735C6078C03E797ULL,   0x841B8AB98FA4B8F8ULL,
 0xADDA7C5F3C4488E3ULL,   0x1EF430E13FE3D78CULL,   0x595E4A08940428B8ULL,   0xEA7006B697A377D7ULL,
 0xD60ABFDBC3CBD6D0ULL,   0x6524F365C06C89BFULL,   0x228E898C6B8B768BULL,   0x91A0C532682C29E4ULL,
 0x5A7BFB56C35A3485ULL,   0xE955B7E8C0FD6BEAULL,   0xAEFFCD016B1A94DEULL,   0x1DD181BF68BDCBB1ULL,
 0x21AB38D23CD56AB6ULL,   0x9285746C3F7235D9ULL,   0xD52F0E859495CAEDULL,   0x6601423B97329582ULL,
 0xD041DD676D77EEAAULL,   0x636F91D96ED0B1C5ULL,   0x24C5EB30C5374EF1ULL,   0x97EBA78EC690119EULL,
 0xAB911EE392F8B099ULL,   0x18BF525D915FEFF6ULL,   0x5F1528B43AB810C2ULL,   0xEC3B640A391F4FADULL,
 0x27E05A6E926952CCULL,   0x94CE16D091CE0DA3ULL,   0xD3646C393A29F297ULL,   0x604A2087398EADF8ULL,
 0x5C3099EA6DE60CFFULL,   0xEF1ED5546E415390ULL,   0xA8B4AFBDC5A6ACA4ULL,   0x1B9AE303C601F3CBULL,
 0x56ED3E2F9E224471ULL,   0xE5C372919D851B1EULL,   0xA26908783662E42AULL,   0x114744C635C5BB45ULL,
 0x2D3DFDAB61AD1A42ULL,   0x9E13B115620A452DULL,   0xD9B9CBFCC9EDBA19ULL,   0x6A978742CA4AE576ULL,
 0xA14CB926613CF817ULL,   0x1262F598629BA778ULL,   0x55C88F71C97C584CULL,   0xE6E6C3CFCADB0723ULL,
 0xDA9C7AA29EB3A624ULL,   0x69B2361C9D14F94BULL,   0x2E184CF536F3067FULL,   0x9D36004B35545910ULL,
 0x2B769F17CF112238ULL,   0x9858D3A9CCB67D57ULL,   0xDFF2A94067518263ULL,   0x6CDCE5FE64F6DD0CULL,
 0x50A65C93309E7C0BULL,   0xE388102D33392364ULL,   0xA4226AC498DEDC50ULL,   0x170C267A9B79833FULL,
 0xDCD7181E300F9E5EULL,   0x6FF954A033A8C131ULL,   0x28532E49984F3E05ULL,   0x9B7D62F79BE8616AULL,
 0xA707DB9ACF80C06DULL,   0x14299724CC279F02ULL,   0x5383EDCD67C06036ULL,   0xE0ADA17364673F59ULL
};

#define E2E_STOP_SEC_CONST_UNSPECIFIED
#include <E2E_MemMap.h>

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

static FUNC(uint64, SCRC_CODE) SCrc_CalculateCRC64P7
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint64                                    SCrc_StartValue64P7,
    boolean                                   SCrc_IsFirstCall
);

/**
 * \brief Perform null pointer checks of the input parameters of the Protect function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr  Pointer to port/data communication state.
 * \param[in] DataPtr   Pointer to Data to be protected.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least one input parameter is Null
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07ProtectNullPtrChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);

/**
 * \brief Perform input parameter checks of the Config parameter
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07ConfigParamChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
);


/**
 * \brief Perform null pointer checks of the input parameters of the Check function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr  Pointer to port/data communication state.
 * \param[in] DataPtr   Pointer to Data to be protected.
 * \param[in] Length    Length of data in Bytes.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07CheckNullPtrChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
);

/**
 * \brief Calculate the Crc value
 *
 * \param[in] DataPtr        Pointer to Data to be protected.
 * \param[in] CrcByteOffset  Offset of CRC value in bytes containing the most significant
 *                           byte of the CRC value (i.e. CrcByteOffset+3 contains the least
 *                           significant byte of the 4-byte CRC value).
 * \param[in] Length         Length of Data.
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
static FUNC(uint64, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 CrcByteOffset,
    uint32 Length
);

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile P07.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Length    Length of the Data in byte.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07Protect
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
);

/**
 * \brief Check the received Data using the E2E Profile P07.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to received Data .
 * \param[in]     Length    Length of Data in bytes .
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07Check
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
);

/**
 * \brief Write 4 byte of value into the memory.
 *
 * \param[in] DataPtr   Pointer to received Data.
 * \param[in] value     Value of data to be stored.
 *
 * \pre All input parameters are valid
 *
 * \return Function void
 */
static FUNC(void, E2E_CODE)E2EP07_EB_WRITE4BYTE_VALUE
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Value
);
/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

static FUNC(void, E2E_CODE)E2EP07_EB_WRITE4BYTE_VALUE
(
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
   uint32 Value
)
{
   (DataPtr)[0] = (uint8)((uint32)((Value) >> 24U) & (uint8)0xFFU);
   (DataPtr)[1] = (uint8)((uint32)((Value) >> 16U) & (uint8)0xFFU);
   (DataPtr)[2] = (uint8)((uint32)((Value) >> 8U) & (uint8)0xFFU);
   (DataPtr)[3] = (uint8)((Value) & (uint8)0xFFU);
}

FUNC(Std_ReturnType, E2E_CODE) E2E_P07Protect
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function
     * Note: Since State contains only a Counter where all values are valid, no
     *       check of State parameters is required. */
    if (E2E_EB_P07ProtectNullPtrChecks(ConfigPtr, StatePtr, DataPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P07ConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U == Length)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if(Length < (ConfigPtr->MinDataLength / 8U ))
    {
      RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (Length > (ConfigPtr->MaxDataLength / 8U ))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((uint32)((ConfigPtr->Offset / 8U) + (E2EP07_EB_E2EHEADER_SIZE_BITS / 8U)) > Length)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute protect function */
        RetVal = E2E_EB_P07Protect(ConfigPtr, StatePtr, DataPtr, Length);
    }
    return RetVal;
}


FUNC(Std_ReturnType, E2E_CODE) E2E_P07Check
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function. Checking the state parameters
     * (Counter, Status) makes no sense since all values are allowed and Status
     * value is not read. */
    /* Note: Null pointer is allowed for DataPtr if Length equals 0.
     *       This may happen if queued communication is used and no data is available. */
    if (E2E_EB_P07CheckNullPtrChecks(ConfigPtr, StatePtr, DataPtr, Length) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P07ConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((NULL_PTR != DataPtr) && (Length == 0U))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && (((uint32)(ConfigPtr->MinDataLength / 8U)) > Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && (((uint32)(ConfigPtr->MaxDataLength / 8U)) < Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && ((uint32)((ConfigPtr->Offset / 8U) + (E2EP07_EB_E2EHEADER_SIZE_BITS / 8U)) > Length))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute check function */
        RetVal = E2E_EB_P07Check(ConfigPtr, StatePtr, DataPtr, Length);
    }

    return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_P07ProtectInit
(
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    {
        StatePtr->Counter = 0U;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}
/*SWS_E2E_00549*/
FUNC(Std_ReturnType, E2E_CODE) E2E_P07CheckInit
(
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    { /*SWS_E2E_00552*/
        /* Counter is initialized to maximum value instead of 0. */
        StatePtr->Counter = E2EP07_EB_COUNTER_MAXVALUE;
        StatePtr->Status = E2E_P07STATUS_ERROR;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}

FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P07MapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_P07CheckStatusType Status
)
{
    E2E_PCheckStatusType RetVal;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
      case E2E_P07STATUS_OK: /* fall through */
      case E2E_P07STATUS_OKSOMELOST:
        RetVal = E2E_P_OK;
        break;
      case E2E_P07STATUS_ERROR:
        RetVal = E2E_P_ERROR;
        break;
      case E2E_P07STATUS_REPEATED:
        RetVal = E2E_P_REPEATED;
        break;
      case E2E_P07STATUS_NONEWDATA:
        RetVal = E2E_P_NONEWDATA;
        break;
      case E2E_P07STATUS_WRONGSEQUENCE:
        RetVal = E2E_P_WRONGSEQUENCE;
        break;
      default:
        RetVal = E2E_P_ERROR;
        break;
        }
    }
    else
    {
        RetVal = E2E_P_ERROR;
    }

    return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07ProtectNullPtrChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
)
{
    Std_ReturnType RetVal;

    /* null pointer checks */
    if (NULL_PTR == ConfigPtr)
    {
       RetVal = E_NOT_OK;
    }
    else if (NULL_PTR == StatePtr)
    {
       RetVal = E_NOT_OK;
    }
    else if (NULL_PTR == DataPtr)
    {
       RetVal = E_NOT_OK;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}


static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07ConfigParamChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal;

    /* sanity and input checks: E2E_P07ConfigType */
    /* ConfigPtr->MaxDeltaCounter requires no check since all values are valid */
    if (0U != (ConfigPtr->Offset % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->MinDataLength % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->MaxDataLength % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (E2EP07_EB_E2EHEADER_SIZE_BITS > ConfigPtr->MinDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (ConfigPtr->MinDataLength > ConfigPtr->MaxDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((uint32)(ConfigPtr->MaxDataLength - E2EP07_EB_E2EHEADER_SIZE_BITS) <= ConfigPtr->Offset)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07CheckNullPtrChecks
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
)
{
    Std_ReturnType RetVal;

    /* null pointer checks */
    if (NULL_PTR == ConfigPtr)
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (NULL_PTR == StatePtr)
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else if ((NULL_PTR == DataPtr) && (0U != Length))
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(uint64, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 CrcByteOffset,
    uint32 Length
)
{
    uint64 Crc;

    /*Compute CRC over bytes that are before CRC.
      Computation length: offset, where offset is number of bytes before the E2E header */
    if(CrcByteOffset > 0U)
    {
      Crc = SCrc_CalculateCRC64P7(&DataPtr[0],
                                  CrcByteOffset,
                                  E2EP07_EB_CRC_STARTVALUE,
                                  TRUE);

      /* Compute CRC over bytes that are after CRC.*/
      Crc = SCrc_CalculateCRC64P7(&DataPtr[CrcByteOffset + E2EP07_EB_E2EHEADER_CRC_SIZE],
                                  ((Length - CrcByteOffset) - E2EP07_EB_E2EHEADER_CRC_SIZE),
                                  Crc,
                                  FALSE);
    }
    else
    {
      /* Compute CRC over bytes that are after CRC.*/
      Crc = SCrc_CalculateCRC64P7(&DataPtr[E2EP07_EB_E2EHEADER_CRC_SIZE],
                                  (Length - E2EP07_EB_E2EHEADER_CRC_SIZE),
                                  E2EP07_EB_CRC_STARTVALUE,
                                  TRUE);
    }

    return Crc;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07Protect
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
)
{
    /* 1) Compute Offset in bytes and values written into data */
    const uint32 Offset = ConfigPtr->Offset / 8U;
    const uint32 Counter = StatePtr->Counter;
    const uint32 DataID = ConfigPtr->DataID;

    /* 2) Write Length in Big Endian order (first high byte) */
    E2EP07_EB_WRITE4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_LENGTH_OFFSET], Length);

    /* 3) Write Counter in Big Endian order */
    E2EP07_EB_WRITE4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_COUNTER_OFFSET], Counter);

    /* 4) Write 4-byte DataID in Big Endian order */
    E2EP07_EB_WRITE4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_DATAID_OFFSET], DataID);

    {
        const uint32 CrcByteOffset = Offset + E2EP07_EB_E2EHEADER_CRC_OFFSET;

        /* 5) Compute CRC */
        const uint64 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length);

        /* 6) Write CRC */
        E2EP07_EB_WRITE8BYTE_VALUE(&DataPtr[CrcByteOffset], ComputedCRC);
    }

    /* 7) Increment Counter */
    if (E2EP07_EB_COUNTER_MAXVALUE > StatePtr->Counter)
    {
       StatePtr->Counter++;
    }
    else
    {
       StatePtr->Counter = 0U;
    }

    return E2E_E_OK;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P07Check
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
)
{
    /* If Length equals 0 then DataPtr equals NULL and no data was received in this cycle. */
    const boolean NewDataAvailable = (Length != 0U) ? TRUE : FALSE;

    if (TRUE == NewDataAvailable)
    {
        /* 1) Compute offset */
        const uint32 Offset = ConfigPtr->Offset / 8U;
        const uint32 CrcByteOffset = Offset + E2EP07_EB_E2EHEADER_CRC_OFFSET;

        /* 2) Read Length */
        const uint32 ReceivedLength =
                E2EP07_EB_READ4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_LENGTH_OFFSET]);

        /* 3) Read Counter */
        const uint32 ReceivedCounter =
                E2EP07_EB_READ4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_COUNTER_OFFSET]);

        /* 4) Read DataID */
        const uint32 ReceivedDataID =
                E2EP07_EB_READ4BYTE_VALUE(&DataPtr[Offset+E2EP07_EB_E2EHEADER_DATAID_OFFSET]);

        /* 5) Read CRC */
        const uint64 ReceivedCRC =
                E2EP07_EB_READ8BYTE_VALUE(&DataPtr[CrcByteOffset]);

        /* 6) Compute CRC */
        const uint64 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length);

        /* 7) Do checks */
        if (ReceivedCRC != ComputedCRC)
        {
            StatePtr->Status = E2E_P07STATUS_ERROR;
        }
        else if (ReceivedDataID != ConfigPtr->DataID)
        {
            StatePtr->Status = E2E_P07STATUS_ERROR;
        }
        else if (ReceivedLength != Length)
        {
            StatePtr->Status = E2E_P07STATUS_ERROR;
        }
        else
        {
            /* correct CRC and E2E header content => check sequence counter */
            const uint32 DeltaCounter = (ReceivedCounter >= StatePtr->Counter) ?
                 (uint32)(ReceivedCounter - StatePtr->Counter) :
                 (uint32)((uint32)(E2EP07_EB_COUNTER_MAXVALUE - StatePtr->Counter)
                       + (ReceivedCounter + 1U));

            if (DeltaCounter > ConfigPtr->MaxDeltaCounter)
            {
                /* counter too big, i.e. too many data in the sequence
                 * have been probably lost since the last reception. */
                StatePtr->Status = E2E_P07STATUS_WRONGSEQUENCE;
            }
            else if (1U < DeltaCounter)
            {
                /* the Counter is incremented by DeltaCounter
                 * (1 <= DeltaCounter <= MaxDeltaCounter), i.e.
                 * some Data in the sequence have been probably lost since
                 * the last correct reception, but this is within
                 * the configured tolerance range. */
                StatePtr->Status = E2E_P07STATUS_OKSOMELOST;
            }
            else if (1U == DeltaCounter)
            {
                /* the Counter is incremented by 1, i.e. no Data has
                 * been lost since the last correct data reception. */
                StatePtr->Status = E2E_P07STATUS_OK;
            }
            else /* DeltaCounter == 0U */
            {
                /* the counter is identical, i.e. data has already been
                 * received (repeated message) */
                StatePtr->Status = E2E_P07STATUS_REPEATED;
            }

            /* Store counter value for next received message. */
            StatePtr->Counter = ReceivedCounter;
        }
    }
    else
    {
        /* no new data available since the last call */
        StatePtr->Status = E2E_P07STATUS_NONEWDATA;
    }

    return E2E_E_OK;
}

static FUNC(uint64, SCRC_CODE) SCrc_CalculateCRC64P7
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint64                                    SCrc_StartValue64P7,
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
        SCrc_StartValue64P7 = (SCrc_StartValue64P7 ^ SCRC_CRC64P7_XORVALUE);
    }
    else
    {
        SCrc_StartValue64P7 = SCRC_CRC64P7_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i < SCrc_Length; ++i)
    {
        /* Process one byte of data */
        SCrc_StartValue64P7
          = (SCrc_Table64_P7[((uint8)(SCrc_StartValue64P7 & 0xFFU)) ^ SCrc_DataPtr[i]]
            ^ (SCrc_StartValue64P7 >> 8U));
    }

    /* Apply XOR-value of 0xFFFFFFFFFFFFFFFFU */
    return (SCrc_StartValue64P7 ^ SCRC_CRC64P7_XORVALUE);
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
