/**
 * \file
 *
 * \brief AUTOSAR E2E
 *
 * This file contains the implementation of the AUTOSAR
 * module E2E.
 *
 * \version 2.0.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef E2E_COMXF_S_STC_H
#define E2E_COMXF_S_STC_H

#include <ComXf_Cfg.h>

#if (defined COMXF_XFRMBUFFERLENGTH_S_TYPE) /* To prevent double declaration */
#error COMXF_XFRMBUFFERLENGTH_S_TYPE already defined
#endif /* #if (defined COMXF_XFRMBUFFERLENGTH_S_TYPE) */

#if (COMXF_XFRMBUFFERLENGTHTYPE_ISUINT32 == STD_ON)
#define COMXF_XFRMBUFFERLENGTH_S_TYPE uint32
#else
#define COMXF_XFRMBUFFERLENGTH_S_TYPE uint16
#endif

#if (defined E2E_USER_PREFIX) /* To prevent double declaration */
#error E2E_USER_PREFIX already defined
#endif /* #if (defined E2E_USER_PREFIX) */

#define E2E_USER_PREFIX COMXF

#include <E2E_UnPackSig.h>          /* UnPack macros from E2E library */

/*==================[macros]================================================*/

#if (defined E2E_COMXF_S_TXPREPAREBUFFER) /* To prevent double declaration */
#error E2E_COMXF_S_TXPREPAREBUFFER already defined
#endif /* #if (defined E2E_COMXF_S_TXPREPAREBUFFER) */

#define E2E_COMXF_S_TXPREPAREBUFFER(bufferLength, buffer, bytes_to_copy, initValue)          \
do {                                                                                         \
    COMXF_XFRMBUFFERLENGTH_S_TYPE i;                                                         \
    *bufferLength = (COMXF_XFRMBUFFERLENGTH_S_TYPE) (bytes_to_copy);                         \
    /* copy the value of the Pdu to the signal group buffer */                               \
    for(i=0U;i<(COMXF_XFRMBUFFERLENGTH_S_TYPE) (bytes_to_copy);i++)                          \
    {                                                                                        \
      buffer[i] = (uint8) (initValue);                                                       \
    }                                                                                        \
} while ((0))                                                                                  \

#if (defined COMXF_S_DECL_FUNC_XF) /* To prevent double declaration */
#error COMXF_S_DECL_FUNC_XF already defined
#endif /* #if (defined COMXF_S_DECL_FUNC_XF) */

#define COMXF_S_DECL_FUNC_XF(XfrmName, DataType)             \
extern FUNC(uint8, COMXF_CODE) XfrmName                      \
(                                                            \
   P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,          \
   P2VAR(COMXF_XFRMBUFFERLENGTH_S_TYPE, AUTOMATIC, COMXF_APPL_DATA) bufferLength,   \
   P2CONST(DataType, AUTOMATIC, COMXF_APPL_DATA) dataElement \
)

#if (defined COMXF_S_DECL_FUNC_INVXF) /* To prevent double declaration */
#error COMXF_S_DECL_FUNC_INVXF already defined
#endif /* #if (defined COMXF_S_DECL_FUNC_INVXF) */

#define COMXF_S_DECL_FUNC_INVXF(XfrmName, DataType)        \
extern FUNC(uint8, COMXF_CODE) XfrmName                    \
(                                                          \
   P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,      \
   COMXF_XFRMBUFFERLENGTH_S_TYPE bufferLength,                                    \
   P2VAR(DataType, AUTOMATIC, COMXF_APPL_DATA) dataElement \
)

#if (defined COMXF_S_DEF_FUNC_XF_START) /* To prevent double declaration */
#error COMXF_S_DEF_FUNC_XF_START already defined
#endif /* #if (defined COMXF_S_DEF_FUNC_XF_START) */

#define COMXF_S_DEF_FUNC_XF_START(XfrmName, DataType, bytes_to_copy, initValue)                \
FUNC(uint8, COMXF_CODE) XfrmName                                                               \
(                                                                                              \
   P2VAR(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,                                            \
   P2VAR(COMXF_XFRMBUFFERLENGTH_S_TYPE, AUTOMATIC, COMXF_APPL_DATA) bufferLength,              \
   P2CONST(DataType, AUTOMATIC, COMXF_APPL_DATA) dataElement                                   \
)                                                                                              \
{                                                                                              \
    uint8 retval = E_OK;                                                                       \
    uint8 E2E_RangeCheckRetVal = E2E_RANGECHK_VALID;                                           \
    if ((buffer == NULL_PTR)||(bufferLength == NULL_PTR)||(dataElement == NULL_PTR))           \
    {                                                                                          \
      retval = E_SER_GENERIC_ERROR;                                                            \
    }                                                                                          \
    else                                                                                       \
    {                                                                                          \
      E2E_COMXF_S_TXPREPAREBUFFER(bufferLength, buffer, (bytes_to_copy), (initValue));

#if (defined COMXF_S_DEF_FUNC_XF_STOP) /* To prevent double declaration */
#error COMXF_S_DEF_FUNC_XF_STOP already defined
#endif /* #if (defined COMXF_S_DEF_FUNC_XF_STOP) */

#define COMXF_S_DEF_FUNC_XF_STOP          \
    }                                     \
    return retval | E2E_RangeCheckRetVal; \
}

#if (defined COMXF_S_DEF_FUNC_INVXF_START) /* To prevent double declaration */
#error COMXF_S_DEF_FUNC_INVXF_START already defined
#endif /* #if (defined COMXF_S_DEF_FUNC_INVXF_START) */

#define COMXF_S_DEF_FUNC_INVXF_START(XfrmName, DataType, ExpectedLength)                           \
FUNC(uint8, COMXF_CODE) XfrmName                                                                   \
(                                                                                                  \
   P2CONST(uint8, AUTOMATIC, COMXF_APPL_DATA) buffer,                                              \
   COMXF_XFRMBUFFERLENGTH_S_TYPE bufferLength,                                                     \
   P2VAR(DataType, AUTOMATIC, COMXF_APPL_DATA) dataElement                                         \
)                                                                                                  \
{                                                                                                  \
    uint8 retval = E_OK;                                                                           \
    if((buffer == NULL_PTR) && (bufferLength == 0U))                                               \
    {                                                                                              \
        retval = E_NO_DATA;                                                                        \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      if ((buffer == NULL_PTR)||(ExpectedLength > bufferLength)||(dataElement == NULL_PTR))        \
      {                                                                                            \
         retval = E_SER_GENERIC_ERROR;                                                             \
      }                                                                                            \
      else                                                                                         \
      {

#if (defined COMXF_S_DEF_FUNC_INVXF_STOP) /* To prevent double declaration */
#error COMXF_S_DEF_FUNC_INVXF_STOP already defined
#endif /* #if (defined COMXF_S_DEF_FUNC_INVXF_STOP) */

#define COMXF_S_DEF_FUNC_INVXF_STOP \
        }                           \
    }                               \
    return retval;                  \
}

#if (defined COMXF_S_PACKSIG) /* To prevent double declaration */
#error COMXF_S_PACKSIG already defined
#endif /* #if (defined COMXF_S_PACKSIG) */

#define COMXF_S_PACKSIG(structmember,bitpos,bitlength,sigtype,nbytes)       \
        E2E_PACKSIG(buffer,(structmember),(bitpos),bitlength,sigtype,nbytes)

#if (defined COMXF_S_UNPACKSIG) /* To prevent double declaration */
#error COMXF_S_UNPACKSIG already defined
#endif /* #if (defined COMXF_S_UNPACKSIG) */

#define COMXF_S_UNPACKSIG(structmember,bitpos,bitlength,sigtype,nbytes)         \
        E2E_UNPACKSIG(buffer,(structmember),(bitpos),bitlength,sigtype,nbytes)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* E2E_COMXF_S_STC_H */
